/* oratio_partes.c - Vide oratio_partes.h. */

#include "oratio_partes.h"
#include "oratio_partes_en.h"
#include "oratio_lexicon.h"
#include "oratio_lexema.h"
#include "materia_arbor.h"
#include "xar.h"
#include <stdio.h>
#include <string.h>

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

s32
oratio_partes_locus (
           OratioClassis  classis,
      constans character* titulus)
{
    constans MateriaTabGenus* genus;
                         i32  j;

    si ((i32)classis >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde (s32)-I;
    }
    genus = &ORATIO_REGISTRUM.genera[oratio_classis_genus(classis)];
    per (j = ZEPHYRUM; j < genus->loci_numerus; j++)
    {
        si (strcmp(ORATIO_REGISTRUM.loci[genus->loci_offset
            + j].titulus,
                titulus) == ZEPHYRUM)
        {
            redde (s32)j;
        }
    }
    redde (s32)-I;
}

chorda
oratio_partes_textus_vocabuli (
                  Piscina* piscina,
    constans MateriaNodus* vocabulum)
{
    constans MateriaValor* partes =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
                      i32 n;
                      i32 k;
                      i32 mensura = ZEPHYRUM;
                   chorda c;

    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    n = materia_valor_lista_numerus(*partes);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);

        si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
        {
            mensura = mensura + e->datum.token->valor.mensura;
        }
    }
    c.datum = (i8*)piscina_allocare(piscina, (memoriae_index)mensura
        + I);
    si (c.datum == NIHIL)
    {
        redde _chorda(NIHIL, ZEPHYRUM);
    }
    c.mensura = ZEPHYRUM;
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);

        si (e != NIHIL && e->genus == MATERIA_VALOR_TOKEN)
        {
            memcpy(c.datum + c.mensura, e->datum.token->valor.datum,
                (size_t)e->datum.token->valor.mensura);
            c.mensura = c.mensura + e->datum.token->valor.mensura;
        }
    }
    c.datum[c.mensura] = ZEPHYRUM;
    redde c;
}

/* lexema derivatum in locum TOKEN ponere (valor copiatur) */
interior b32
_derivatum_ponere (
                  Piscina* piscina,
             MateriaNodus* nodus,
                      i32  locus,
                   chorda  valor,
    constans MateriaToken* origo)
{
    MateriaToken* t;

    si (valor.mensura == ZEPHYRUM)
    {
        redde VERUM;   /* absens */
    }
    t = oratio_lexema_derivatum(piscina, (s32)ORATIO_LEX_DERIVATUM,
        _copia(piscina, valor), origo);
    si (t == NIHIL)
    {
        redde FALSUM;
    }
    redde materia_nodus_ponere(nodus, locus, materia_valor_token(t),
        MATERIA_LOCUS_TOKEN);
}

interior b32
_accidens_ponere (
          MateriaNodus* nodus,
         OratioClassis  classis,
    constans character* titulus,
                   s32  valor)
{
    s32 locus;

    si (valor < ZEPHYRUM)
    {
        redde VERUM;   /* non datum */
    }
    locus = oratio_partes_locus(classis, titulus);
    si (locus < ZEPHYRUM)
    {
        redde VERUM;   /* genus accidens non fert: omittitur */
    }
    redde materia_nodus_ponere(nodus, (i32)locus,
        materia_valor_index(valor),
        MATERIA_LOCUS_INDEX);
}

MateriaNodus*
oratio_partes_nodum_struere (
                      Piscina* piscina,
    constans OratioDescriptio* d,
        constans MateriaToken* origo)
{
    OratioGenus genus = oratio_classis_genus(d->classis);
    MateriaNodus* nodus;

    si ((i32)d->classis >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
    {
        redde NIHIL;
    }
    nodus = materia_nodus_creare(piscina, (s32)genus,
        ORATIO_REGISTRUM.genera[genus].loci_numerus);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    si (   !_derivatum_ponere(piscina, nodus,
        (i32)ORATIO_ANALYSIS_LEMMA,
            d->lemma, origo)
        || !materia_nodus_ponere(nodus, (i32)ORATIO_ANALYSIS_LINGUA,
            materia_valor_index((s32)d->lingua), MATERIA_LOCUS_INDEX)
        || !materia_nodus_ponere(nodus, (i32)ORATIO_ANALYSIS_FONS,
            materia_valor_index((s32)d->fons), MATERIA_LOCUS_INDEX)
        || !_derivatum_ponere(piscina, nodus,
        (i32)ORATIO_ANALYSIS_NATIVUM,
            d->nativum, origo)
        || !_derivatum_ponere(piscina, nodus,
        (i32)ORATIO_ANALYSIS_SENSUS,
            d->sensus, origo))
    {
        redde NIHIL;
    }
    si (   !_accidens_ponere(nodus, d->classis, "casus",
        d->casus_grammaticus)
        || !_accidens_ponere(nodus, d->classis, "numerus", d->numerus)
        || !_accidens_ponere(nodus, d->classis, "genus", d->genus)
        || !_accidens_ponere(nodus, d->classis, "persona", d->persona)
        || !_accidens_ponere(nodus, d->classis, "tempus", d->tempus)
        || !_accidens_ponere(nodus, d->classis, "modus", d->modus)
        || !_accidens_ponere(nodus, d->classis, "vox", d->vox)
        || !_accidens_ponere(nodus, d->classis, "forma-verbi",
            d->forma_verbi)
        || !_accidens_ponere(nodus, d->classis, "gradus", d->gradus)
        || !_accidens_ponere(nodus, d->classis, "species", d->species)
        || !_accidens_ponere(nodus, d->classis, "declinatio",
            d->declinatio > ZEPHYRUM ? d->declinatio : (s32)-I)
        || !_accidens_ponere(nodus, d->classis, "coniugatio",
            d->coniugatio > ZEPHYRUM ? d->coniugatio : (s32)-I))
    {
        redde NIHIL;
    }
    redde nodus;
}

b32
oratio_partes_vocabulum_annotare (
                          Piscina* piscina,
                          Piscina* scratch,
       constans OratioVocabularia* vocabularia,
                     MateriaNodus* vocabulum,
               OratioPartesCensus* census)
{
    constans MateriaValor* partes =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
    constans MateriaToken* origo;
                   chorda  textus;
                      Xar* analyses;
                      Xar* descriptiones;
                      i32  k;
                      b32  classis_visa[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
            OratioClassis  ordo_classium[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
                      i32  numerus_classium = ZEPHYRUM;
                      b32  lingua_visa[ORATIO_LINGUA_NUMERUS];
             OratioLingua  ordo_linguarum[ORATIO_LINGUA_NUMERUS];
                      i32  numerus_linguarum = ZEPHYRUM;

    si (census != NIHIL)
    {
        census->vocabula = census->vocabula + I;
    }
    si (   vocabulum->loci[ORATIO_VOCABULUM_CLASSES].genus
            != MATERIA_VALOR_NIHIL
        || partes->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
    {
        redde VERUM;   /* iam annotatum aut sine partibus */
    }
    origo = materia_valor_lista_obtinere(*partes,
        ZEPHYRUM)->datum.token;
    textus = oratio_partes_textus_vocabuli(scratch, vocabulum);
    si (textus.datum == NIHIL)
    {
        redde FALSUM;
    }
    memset(classis_visa, ZEPHYRUM, magnitudo(classis_visa));
    memset(lingua_visa, ZEPHYRUM, magnitudo(lingua_visa));
    analyses = oratio_vocabularium_la_quaerere(scratch, vocabularia->la,
        textus);
    descriptiones = xar_creare(scratch,
        (i32)magnitudo(OratioDescriptio));
    si (analyses == NIHIL || descriptiones == NIHIL)
    {
        redde FALSUM;
    }
        per (k = ZEPHYRUM; k < xar_numerus(analyses); k++)
        {
        si (!oratio_partes_la_describere(piscina, vocabularia->la,
                (constans OratioAnalysis*)xar_obtinere(analyses, k),
                descriptiones))
        {
            redde FALSUM;
        }
        }
    /* T16: lectiones Anglicae (Moby + regulae) post Latinas appensae */
    si (vocabularia->en != NIHIL)
    {
        Xar* analyses_en = oratio_vocabularium_en_analysare(scratch,
            vocabularia->en, textus);

        si (analyses_en == NIHIL)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < xar_numerus(analyses_en); k++)
        {
            constans OratioAnalysisEn* a =
                (constans OratioAnalysisEn*)xar_obtinere(analyses_en,
                k);

            si (!oratio_partes_en_describere(piscina, vocabularia->en,
                a,
                    descriptiones))
            {
                redde FALSUM;
            }
        }
    }
    si (   vocabularia->en != NIHIL
        && !oratio_partes_en_secundariae(piscina, textus,
        descriptiones))
    {
        redde FALSUM;
    }
    /* regula (T13/T16): vocabulum capitale quod nullus fons Latinus
     * novit - ignotum (Karolus, Taenari) aut Moby soli notum ut
     * substantivum (Bush, Hercules) - nomen proprium candidatum fonte
     * 'regula'; lingua anglica si lectiones Anglicae adsunt */
    si (   textus.datum[ZEPHYRUM] >= 'A'
        && textus.datum[ZEPHYRUM] <= 'Z')
    {
        b32 fons_latinus  = FALSUM;
        b32 proprium      = FALSUM;
        b32 substantivum  = FALSUM;
        b32 anglicum      = FALSUM;

        per (k = ZEPHYRUM; k < xar_numerus(descriptiones); k++)
        {
            constans OratioDescriptio* d =
                (constans OratioDescriptio*)xar_obtinere(descriptiones,
                k);

            si (d->fons != ORATIO_FONS_ANALYSIS_VOCABULARIUM_EN)
            {
                fons_latinus = VERUM;
            }
            si (d->classis == ORATIO_CLASSIS_NOMEN_PROPRIUM)
            {
                proprium = VERUM;
            }
            si (d->classis == ORATIO_CLASSIS_SUBSTANTIVUM)
            {
                substantivum = VERUM;
            }
            si (d->lingua == ORATIO_LINGUA_ANGLICA)
            {
                anglicum = VERUM;
            }
        }
        si (   !fons_latinus && !proprium
            && (xar_numerus(descriptiones) == ZEPHYRUM || substantivum))
        {
            OratioDescriptio* d =
                (OratioDescriptio*)xar_addere(descriptiones);

            si (d == NIHIL)
            {
                redde FALSUM;
            }
            oratio_partes_la_capitalis(piscina, textus, d);
            si (anglicum)
            {
                d->lingua = ORATIO_LINGUA_ANGLICA;
            }
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(descriptiones); k++)
    {
        constans OratioDescriptio* d =
            (constans OratioDescriptio*)xar_obtinere(descriptiones, k);
        MateriaNodus* nodus = oratio_partes_nodum_struere(piscina, d,
            origo);

        si (   nodus == NIHIL
            || !materia_nodus_appendere(piscina, vocabulum,
                (i32)ORATIO_VOCABULUM_ANALYSES,
                materia_valor_nodus(nodus),
                MATERIA_LOCUS_LISTA_NODUS))
        {
            redde FALSUM;
        }
        si (!classis_visa[d->classis])
        {
            classis_visa[d->classis]         = VERUM;
            ordo_classium[numerus_classium]  = d->classis;
            numerus_classium                 = numerus_classium + I;
        }
        si (!lingua_visa[d->lingua])
        {
            lingua_visa[d->lingua]             = VERUM;
            ordo_linguarum[numerus_linguarum]  = d->lingua;
            numerus_linguarum                  = numerus_linguarum + I;
        }
        si (census != NIHIL)
        {
            census->analyses = census->analyses + I;
            census->classes[d->classis] = census->classes[d->classis]
                + I;
            census->linguae[d->lingua] = census->linguae[d->lingua] + I;
        }
    }
    /* compendia: classes (semper), linguae (si analyses) */
    {
        character buffer[512];
              i32 n = ZEPHYRUM;

        si (numerus_classium == ZEPHYRUM)
        {
            n = (i32)sprintf(buffer, "%s",
                oratio_classis_titulus(ORATIO_CLASSIS_IGNOTUM));
            si (census != NIHIL)
            {
                census->ignota = census->ignota + I;
            }
        }
        alioquin
        {
            per (k = ZEPHYRUM; k < numerus_classium; k++)
            {
                constans character* t = oratio_classis_titulus(
                    ordo_classium[k]);

                si (n + (i32)strlen(t) + (i32)II >= (i32)512)
                {
                    frange;
                }
                n = n + (i32)sprintf(buffer + n, "%s%s", k
                    > ZEPHYRUM ? " " : "",
                    t);
            }
        }
        {
            unio {
                character* c;
                       i8* m;
            } u;

            u.c = buffer;
            si (!_derivatum_ponere(piscina, vocabulum,
                    (i32)ORATIO_VOCABULUM_CLASSES, _chorda(u.m, n),
                    origo))
            {
                redde FALSUM;
            }
        }
        si (numerus_linguarum > ZEPHYRUM)
        {
            n = ZEPHYRUM;
            per (k = ZEPHYRUM; k < numerus_linguarum; k++)
            {
                n = n + (i32)sprintf(buffer + n, "%s%s", k
                    > ZEPHYRUM ? " " : "",
                    ORATIO_TITULI_LINGUARUM[ordo_linguarum[k]]);
            }
            {
                unio {
                    character* c;
                           i8* m;
                } u;

                u.c = buffer;
                si (!_derivatum_ponere(piscina, vocabulum,
                        (i32)ORATIO_VOCABULUM_LINGUAE, _chorda(u.m, n),
                        origo))
                {
                    redde FALSUM;
                }
            }
        }
    }
    si (census != NIHIL)
    {
        census->annotata = census->annotata + I;
    }
    redde VERUM;
}

interior b32
_annotare (
                          Piscina* piscina,
                          Piscina* scratch,
       constans OratioVocabularia* vocabularia,
                     MateriaNodus* n,
               OratioPartesCensus* census)
{
    i32 i;

    si (n == NIHIL)
    {
        redde VERUM;
    }
    si (n->genus == (s32)ORATIO_GENUS_VOCABULUM)
    {
        redde oratio_partes_vocabulum_annotare(piscina, scratch,
            vocabularia,
            n, census);
    }
    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            si (!_annotare(piscina, scratch, vocabularia,
                v->datum.nodus,
                census))
            {
                redde FALSUM;
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 m = materia_valor_lista_numerus(*v);
            i32 j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                MateriaValor* e = materia_valor_lista_obtinere(*v, j);

                si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
                    && !_annotare(piscina, scratch, vocabularia,
                        e->datum.nodus, census))
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}

b32
oratio_partes_annotare (
                          Piscina* piscina,
       constans OratioVocabularia* vocabularia,
                     MateriaNodus* radix,
               OratioPartesCensus* census)
{
    Piscina* scratch = piscina_generare_dynamicum("partes_annotatio",
        67108864);
    b32 sanum;

    si (scratch == NIHIL)
    {
        redde FALSUM;
    }
    si (census != NIHIL)
    {
        memset(census, ZEPHYRUM, magnitudo(*census));
    }
        sanum = _annotare(piscina, scratch, vocabularia, radix, census);
    piscina_destruere(scratch);
    /* nodi novi patres accipiunt (lex materiae: comparator patres confert) */
    si (sanum)
    {
        materia_arbor_patres_figere(piscina, radix);
    }
    redde sanum;
}

b32
oratio_partes_compendia_reponere (
         Piscina* piscina,
    MateriaNodus* vocabulum)
{
    constans MateriaValor* analyses =
        &vocabulum->loci[ORATIO_VOCABULUM_ANALYSES];
    constans MateriaValor* partes =
        &vocabulum->loci[ORATIO_VOCABULUM_PARTES];
    constans MateriaToken* origo;
                character  buffer[512];
                      i32  n;
                      i32  k;
                      i32  m;
                      b32  classis_visa[ORATIO_CLASSIS_NUMERUS_CLASSIUM];
                      b32  lingua_visa[ORATIO_LINGUA_NUMERUS];
                    unio {
        character* c;
               i8* m;
    } u;

    si (   vocabulum->loci[ORATIO_VOCABULUM_CLASSES].genus
            != MATERIA_VALOR_TOKEN
        || analyses->genus                      != MATERIA_VALOR_LISTA
        || partes->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*partes) == ZEPHYRUM)
    {
        redde VERUM;
    }
    m = materia_valor_lista_numerus(*analyses);
    si (m == ZEPHYRUM)
    {
        redde VERUM;   /* 'ignotum' manet */
    }
    origo = materia_valor_lista_obtinere(*partes,
        ZEPHYRUM)->datum.token;
    memset(classis_visa, ZEPHYRUM, magnitudo(classis_visa));
    memset(lingua_visa, ZEPHYRUM, magnitudo(lingua_visa));
    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaNodus* a =
            materia_valor_lista_obtinere(*analyses, k)->datum.nodus;
                OratioClassis c =
                    oratio_genus_classis((OratioGenus)a->genus);
           constans character* t;

        si (c >= ORATIO_CLASSIS_NUMERUS_CLASSIUM || classis_visa[c])
        {
            perge;
        }
        t = oratio_classis_titulus(c);
        si (n + (i32)strlen(t) + (i32)II >= (i32)512)
        {
            frange;
        }
        n = n + (i32)sprintf(buffer + n, "%s%s", n
            > ZEPHYRUM ? " " : "", t);
        classis_visa[c] = VERUM;
    }
    u.c = buffer;
    {
        MateriaToken* t = oratio_lexema_derivatum(piscina,
            (s32)ORATIO_LEX_DERIVATUM, _copia(piscina, _chorda(u.m, n)),
            origo);

        si (   t == NIHIL
            || !materia_nodus_reponere(vocabulum,
                (i32)ORATIO_VOCABULUM_CLASSES, materia_valor_token(t),
                MATERIA_LOCUS_TOKEN))
        {
            redde FALSUM;
        }
    }
    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < m; k++)
    {
        constans MateriaNodus* a =
            materia_valor_lista_obtinere(*analyses, k)->datum.nodus;
        constans MateriaValor* l = &a->loci[ORATIO_ANALYSIS_LINGUA];
                          s32  lingua = l->genus == MATERIA_VALOR_INDEX
                              ? l->datum.index : (s32)-I;

        si (   lingua < ZEPHYRUM || lingua >= (s32)ORATIO_LINGUA_NUMERUS
            || lingua_visa[lingua])
        {
            perge;
        }
        n = n + (i32)sprintf(buffer + n, "%s%s", n
            > ZEPHYRUM ? " " : "",
            ORATIO_TITULI_LINGUARUM[lingua]);
        lingua_visa[lingua] = VERUM;
    }
    si (n > ZEPHYRUM)
    {
        MateriaToken* t = oratio_lexema_derivatum(piscina,
            (s32)ORATIO_LEX_DERIVATUM, _copia(piscina, _chorda(u.m, n)),
            origo);
        b32 scriptum = vocabulum->loci[ORATIO_VOCABULUM_LINGUAE].genus
            != MATERIA_VALOR_NIHIL;

        si (   t == NIHIL
            || !(scriptum
                ? materia_nodus_reponere(vocabulum,
                    (i32)ORATIO_VOCABULUM_LINGUAE,
                    materia_valor_token(t),
                    MATERIA_LOCUS_TOKEN)
                : materia_nodus_ponere(vocabulum,
                    (i32)ORATIO_VOCABULUM_LINGUAE,
                    materia_valor_token(t),
                    MATERIA_LOCUS_TOKEN)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
