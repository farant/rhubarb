/* oratio_partes_en.c - Mappatio Moby et regularum Anglicarum (T16) */

#include "oratio_partes_en.h"
#include "oratio_registrum.h"
#include <stdio.h>
#include <string.h>

constans character* constans ORATIO_AUXILIARIA_EN[] = {
    "be", "am", "is", "are", "was", "were", "been", "being",
    "have", "has", "had", "having", "do", "does", "did", "done",
        "doing",
    "will", "would", "shall", "should", "can", "could", "may", "might",
    "must", "ought", NIHIL
};
constans character* constans ORATIO_SUBORDINANTES_EN[] = {
    "that", "because", "since", "if", "although", "though", "while",
    "whereas", "unless", "until", "as", "than", "whether", "after",
    "before", "when", "whenever", "where", "wherever", "once", "so",
    "lest", "till", "like", NIHIL
};
constans character* constans ORATIO_PARTICULAE_EN[] = {
    "to", "not", NIHIL
};
constans character* constans ORATIO_NUMERALIA_EN[] = {
    "zero", "one", "two", "three", "four", "five", "six", "seven",
    "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty",
    "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety",
    "hundred", "thousand", "million", "billion", "trillion", NIHIL
};
constans character* constans ORATIO_INTERIECTIONES_EN[] = {
    "yes", "no", "please", "welcome", "hello", "hi", "hey", "oh", "ah",
    "wow", "thanks", "ok", "okay", "sorry", "yeah", "yep", "nope",
        "bye",
    "goodbye", NIHIL
};

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

interior chorda
_copia (
    Piscina* piscina,
     chorda  c)
{
    chorda d;

    d.datum = (i8*)piscina_allocare(piscina, (memoriae_index)c.mensura
        + I);
    si (d.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(d.datum, c.datum, (size_t)c.mensura);
    }
    d.datum[c.mensura]  = ZEPHYRUM;
    d.mensura           = c.mensura;
    redde d;
}

/* an titulus praefixo incipiat */
interior b32
_praefixum (
    constans character* titulus,
    constans character* praefixum)
{
    redde (b32)(strncmp(titulus, praefixum, strlen(praefixum))
        == ZEPHYRUM);
}

/* classis litterae codicis Moby; NUMERUS_CLASSIUM extra legendam */
interior OratioClassis
_classis_litterae (
    i8 codex)
{
    constans character* t = oratio_vocabularium_en_classis(codex);

    si (t == NIHIL)
    {
        redde ORATIO_CLASSIS_NUMERUS_CLASSIUM;
    }
    redde oratio_classis_ex_titulo(t, (i32)strlen(t));
}

/* classis tituli analysis (NIHIL aut ignotus -> ignotum) */
interior OratioClassis
_classis_tituli (
    constans character* t)
{
    OratioClassis c;

    si (t == NIHIL)
    {
        redde ORATIO_CLASSIS_IGNOTUM;
    }
    c = oratio_classis_ex_titulo(t, (i32)strlen(t));
    redde c
        < ORATIO_CLASSIS_NUMERUS_CLASSIUM ? c : ORATIO_CLASSIS_IGNOTUM;
}

/* descriptio nova appensa: classis, lemma, nativum, fons, lingua */
interior OratioDescriptio*
_nova (
          Piscina* piscina,
              Xar* exitus,
    OratioClassis  classis,
           chorda  lemma,
           chorda  nativum)
{
    OratioDescriptio* d = (OratioDescriptio*)xar_addere(exitus);

    si (d == NIHIL)
    {
        redde NIHIL;
    }
    oratio_descriptio_vacare(d);
    d->classis  = classis;
    d->lemma    = _copia(piscina, lemma);
    d->nativum  = _copia(piscina, nativum);
    d->fons     = ORATIO_FONS_ANALYSIS_VOCABULARIUM_EN;
    d->lingua   = ORATIO_LINGUA_ANGLICA;
    redde d;
}

/* forma exacta: una descriptio per classem litterarum, ordine Moby */
interior b32
_exacta (
    Piscina* piscina,
        Xar* exitus,
     chorda  codices,
     chorda  lemma,
     chorda  nativum)
{
    b32 visa[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
    b32 visa_pluralis[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
    i32 k;
    i32 appensae = ZEPHYRUM;

    memset(visa, ZEPHYRUM, magnitudo(visa));
    memset(visa_pluralis, ZEPHYRUM, magnitudo(visa_pluralis));
    per (k = ZEPHYRUM; k < codices.mensura; k++)
    {
                      i8  littera   = codices.datum[k];
           OratioClassis  c         = _classis_litterae(littera);
                     b32  pluralis  = (b32)(littera == 'p');
        OratioDescriptio* d;

        si (c >= ORATIO_CLASSIS_NUMERUS_CLASSIUM)
        {
            perge;   /* littera extra legendam */
        }
        si (pluralis ? visa_pluralis[c] : visa[c])
        {
            perge;
        }
        d = _nova(piscina, exitus, c, lemma, nativum);
        si (d == NIHIL)
        {
            redde FALSUM;
        }
        si (pluralis)
        {
            d->numerus =
                (s32)ORATIO_NUMERUS_GRAMMATICUS_PLURALIS;
            visa_pluralis[c] = VERUM;
        }
        alioquin
        {
            visa[c] = VERUM;
        }
        appensae = appensae + I;
    }
    si (appensae == ZEPHYRUM)
    {
        /* recordum totum extra legendam (cowardic\Ne): ignotum */
        redde (b32)(_nova(piscina, exitus, ORATIO_CLASSIS_IGNOTUM,
            lemma,
            nativum) != NIHIL);
    }
    redde VERUM;
}

/* regula pluralis-*: una per classem basis (substantivum | verbum) */
interior b32
_pluralis (
               Piscina* piscina,
                   Xar* exitus,
                chorda  codices,
                chorda  lemma,
                chorda  nativum,
    constans character* classis_analysis)
{
    b32 substantivum_visum  = FALSUM;
    b32 verbum_visum        = FALSUM;
    i32 k;

    per (k = ZEPHYRUM; k < codices.mensura; k++)
    {
           OratioClassis  c = _classis_litterae(codices.datum[k]);
        OratioDescriptio* d;

        si (c == ORATIO_CLASSIS_SUBSTANTIVUM && !substantivum_visum)
        {
            d = _nova(piscina, exitus, c, lemma, nativum);
            si (d == NIHIL)
            {
                redde FALSUM;
            }
            d->numerus =
                (s32)ORATIO_NUMERUS_GRAMMATICUS_PLURALIS;
            substantivum_visum = VERUM;
        }
        alioquin si (c == ORATIO_CLASSIS_VERBUM && !verbum_visum)
        {
            d = _nova(piscina, exitus, c, lemma, nativum);
            si (d == NIHIL)
            {
                redde FALSUM;
            }
            d->persona = (s32)ORATIO_PERSONA_TERTIA;
            d->numerus =
                (s32)ORATIO_NUMERUS_GRAMMATICUS_SINGULARIS;
            d->tempus       = (s32)ORATIO_TEMPUS_PRAESENS;
            d->modus        = (s32)ORATIO_MODUS_INDICATIVUS;
            d->forma_verbi  = (s32)ORATIO_FORMA_VERBI_FINITUM;
            verbum_visum    = VERUM;
        }
    }
    si (!substantivum_visum && !verbum_visum)
    {
        /* basis sine N/V (regula admisit): classis analysis pluralis */
        OratioDescriptio* d = _nova(piscina, exitus,
            _classis_tituli(classis_analysis), lemma, nativum);

        si (d == NIHIL)
        {
            redde FALSUM;
        }
        d->numerus = (s32)ORATIO_NUMERUS_GRAMMATICUS_PLURALIS;
    }
    redde VERUM;
}

b32
oratio_partes_en_describere (
                          Piscina* piscina,
    constans OratioVocabulariumEn* en,
        constans OratioAnalysisEn* analysis,
                              Xar* exitus)
{
    constans OratioVocabulumEn* r = analysis->recordum >= ZEPHYRUM
        ? oratio_vocabularium_en_recordum(en,
        analysis->recordum) : NIHIL;
                        chorda codices = r != NIHIL ? r->codices
                            : _chorda(NIHIL, ZEPHYRUM);
                     character buffer[128];
                        chorda nativum;
                           i32 n = ZEPHYRUM;
                           i32 k;

    per (k = ZEPHYRUM; k < codices.mensura && n < (i32)32; k++)
    {
        buffer[n]  = (character)codices.datum[k];
        n          = n + I;
    }
    si (analysis->regula < ZEPHYRUM)
    {
        nativum = _chorda((i8*)buffer, n);
        redde _exacta(piscina, exitus, codices, analysis->basis,
            nativum);
    }
    {
        constans OratioRegulaEn* regula =
            &ORATIO_REGULAE_EN[analysis->regula];
             constans character* t = regula->titulus;
               OratioDescriptio* d;

        n = n + (i32)sprintf(buffer + n, "%s%s", n
            > ZEPHYRUM ? " " : "",
            t);
        nativum = _chorda((i8*)buffer, n);
        si (_praefixum(t, "pluralis-"))
        {
            redde _pluralis(piscina, exitus, codices, analysis->basis,
                nativum, analysis->classis);
        }
        si (_praefixum(t, "praeteritum-"))
        {
            d = _nova(piscina, exitus, ORATIO_CLASSIS_VERBUM,
                analysis->basis, nativum);
            si (d == NIHIL)
            {
                redde FALSUM;
            }
            d->tempus       = (s32)ORATIO_TEMPUS_PRAETERITUM;
            d->modus        = (s32)ORATIO_MODUS_INDICATIVUS;
            d->forma_verbi  = (s32)ORATIO_FORMA_VERBI_FINITUM;
            d = _nova(piscina, exitus, ORATIO_CLASSIS_VERBUM,
                analysis->basis, nativum);
            si (d == NIHIL)
            {
                redde FALSUM;
            }
            d->tempus       = (s32)ORATIO_TEMPUS_PRAETERITUM;
            d->forma_verbi  = (s32)ORATIO_FORMA_VERBI_PARTICIPIUM;
            redde VERUM;
        }
        si (_praefixum(t, "participium-"))
        {
            d = _nova(piscina, exitus, ORATIO_CLASSIS_VERBUM,
                analysis->basis, nativum);
            si (d == NIHIL)
            {
                redde FALSUM;
            }
            d->tempus       = (s32)ORATIO_TEMPUS_PRAESENS;
            d->forma_verbi  = (s32)ORATIO_FORMA_VERBI_PARTICIPIUM;
            redde VERUM;
        }
        si (_praefixum(t, "possessivum"))
        {
            /* classis basis casu genitivo (it's pronomen, Fran's
             * substantivum), deinde particula (POS) et auxiliare ('s =
             * is, has) - rangae EWT */
            b32 aliqua = FALSUM;
            b32 visa_s = FALSUM;
            b32 visa_p = FALSUM;

            per (k = ZEPHYRUM; k < codices.mensura; k++)
            {
                OratioClassis c = _classis_litterae(codices.datum[k]);

                si (   (c == ORATIO_CLASSIS_SUBSTANTIVUM && !visa_s)
                    || (c == ORATIO_CLASSIS_PRONOMEN && !visa_p))
                {
                    d = _nova(piscina, exitus, c, analysis->basis,
                        nativum);
                    si (d == NIHIL)
                    {
                        redde FALSUM;
                    }
                    d->casus_grammaticus  = (s32)ORATIO_CASUS_GENITIVUS;
                    aliqua                = VERUM;
                    si (c == ORATIO_CLASSIS_SUBSTANTIVUM)
                    {
                        visa_s = VERUM;
                    }
                    alioquin
                    {
                        visa_p = VERUM;
                    }
                }
            }
            si (!aliqua)
            {
                d = _nova(piscina, exitus, ORATIO_CLASSIS_SUBSTANTIVUM,
                    analysis->basis, nativum);
                si (d == NIHIL)
                {
                    redde FALSUM;
                }
                d->casus_grammaticus = (s32)ORATIO_CASUS_GENITIVUS;
            }
            si (   _nova(piscina, exitus, ORATIO_CLASSIS_PARTICULA,
                    analysis->basis, nativum) == NIHIL
                || _nova(piscina, exitus, ORATIO_CLASSIS_AUXILIARE,
                    analysis->basis, nativum) == NIHIL)
            {
                redde FALSUM;
            }
            redde VERUM;
        }
        si (_praefixum(t, "contractio-"))
        {
            /* basis (pronomen | verbum) + pars altera: n't particula,
             * 're 'm 'll 've 'd auxiliare */
            si (   _nova(piscina, exitus,
                _classis_tituli(analysis->classis),
                    analysis->basis, nativum) == NIHIL
                || _nova(piscina, exitus, _praefixum(t,
                "contractio-n't")
                    ? ORATIO_CLASSIS_PARTICULA
                    : ORATIO_CLASSIS_AUXILIARE,
                    analysis->basis, nativum) == NIHIL)
            {
                redde FALSUM;
            }
            redde VERUM;
        }
        si (_praefixum(t, "adverbium-"))
        {
            redde (b32)(_nova(piscina, exitus, ORATIO_CLASSIS_ADVERBIUM,
                analysis->basis, nativum) != NIHIL);
        }
        si (   _praefixum(t, "comparativus-")
            || _praefixum(t, "superlativus-"))
        {
            d = _nova(piscina, exitus, ORATIO_CLASSIS_ADIECTIVUM,
                analysis->basis, nativum);
            si (d == NIHIL)
            {
                redde FALSUM;
            }
            d->gradus = _praefixum(t, "comparativus-")
                ? (s32)ORATIO_GRADUS_COMPARATIVUS
                : (s32)ORATIO_GRADUS_SUPERLATIVUS;
            redde VERUM;
        }
        /* contractio-*, compositum, ceterae: classis analysis */
        redde (b32)(_nova(piscina, exitus,
            _classis_tituli(analysis->classis),
            analysis->basis, nativum) != NIHIL);
    }
}

/* an forma plicata in lista NIHIL terminata sit */
interior b32
_in_lista (
                       chorda  plicata,
    constans character* constans* lista)
{
    i32 k;

    per (k = ZEPHYRUM; lista[k] != NIHIL; k++)
    {
        i32 l = (i32)strlen(lista[k]);

        si (   l == plicata.mensura
            && memcmp(lista[k], plicata.datum, (size_t)l) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lectio secundaria fonte regula (nativum = titulus listae) */
interior b32
_secundaria (
               Piscina* piscina,
                   Xar* exitus,
         OratioClassis  classis,
                chorda  lemma,
    constans character* titulus)
{
    unio {
        constans character* c;
                        i8* m;
    } u;
    OratioDescriptio* d;

    u.c = titulus;
    d = _nova(piscina, exitus, classis, lemma, _chorda(u.m,
        (i32)strlen(titulus)));
    si (d == NIHIL)
    {
        redde FALSUM;
    }
    d->fons = ORATIO_FONS_ANALYSIS_REGULA;
    redde VERUM;
}

b32
oratio_partes_en_secundariae (
    Piscina* piscina,
     chorda  forma,
        Xar* exitus)
{
    character buffer[64];
       chorda plicata;
          i32 k;
          i32 n = ZEPHYRUM;

    per (k = ZEPHYRUM; k < forma.mensura && n < (i32)63; k++)
    {
        character c = (character)forma.datum[k];

        buffer[n] = (c >= 'A'
            && c <= 'Z') ? (character)(c + ('a' - 'A'))
            : c;
        n = n + I;
    }
    plicata = _chorda((i8*)buffer, n);
    si (   _in_lista(plicata, ORATIO_AUXILIARIA_EN)
        && !_secundaria(piscina, exitus, ORATIO_CLASSIS_AUXILIARE,
        plicata,
            "auxiliaria"))
    {
        redde FALSUM;
    }
    si (   _in_lista(plicata, ORATIO_SUBORDINANTES_EN)
        && !_secundaria(piscina, exitus,
            ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS, plicata,
            "subordinantes"))
    {
        redde FALSUM;
    }
    si (   _in_lista(plicata, ORATIO_PARTICULAE_EN)
        && !_secundaria(piscina, exitus, ORATIO_CLASSIS_PARTICULA,
        plicata,
            "particulae"))
    {
        redde FALSUM;
    }
    si (   _in_lista(plicata, ORATIO_NUMERALIA_EN)
        && !_secundaria(piscina, exitus, ORATIO_CLASSIS_NUMERALE,
        plicata,
            "numeralia"))
    {
        redde FALSUM;
    }
    si (   _in_lista(plicata, ORATIO_INTERIECTIONES_EN)
        && !_secundaria(piscina, exitus, ORATIO_CLASSIS_INTERIECTIO,
            plicata, "interiectiones"))
    {
        redde FALSUM;
    }
    redde VERUM;
}
