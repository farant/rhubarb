/* oratio_partes_la.c - Vide oratio_partes_la.h. */

#include "oratio_partes_la.h"
#include "oratio_glossarium.h"
#include "oratio_vocabularium.h"
#include <stdio.h>
#include <string.h>

constans character* constans ORATIO_CONIUNCTIONES_SUBORDINANTES[] = {
    "ut", "cum", "si", "ne", "quod", "quia", "quoniam", "dum",
        "quamquam",
    "etsi", "quamvis", "ubi", "postquam", "antequam", "priusquam",
        "donec",
    "quin", "nisi", "quando", "quotiens", "sicut", "tamquam", "quasi",
    "licet", "quum", "uti",
    NIHIL
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
    memcpy(d.datum, c.datum, (size_t)c.mensura);
    d.datum[c.mensura]  = ZEPHYRUM;
    d.mensura           = c.mensura;
    redde d;
}

interior chorda
_ex_literis (
    constans character* literae)
{
    unio {
        constans character* c;
                        i8* m;
    } u;

    u.c = literae;
    redde _chorda(u.m, (i32)strlen(literae));
}

interior b32
_aequalis (
                chorda  c,
    constans character* literae)
{
    i32 l = (i32)strlen(literae);

    redde (b32)(c.mensura == l
        && (l == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)l) == ZEPHYRUM));
}

/* lexema n-tum (0-basatum) chordae spatiis divisae; vacuum si absens */
interior chorda
_lexema (
    chorda c,
       i32 n)
{
    i32 i = ZEPHYRUM;
    i32 k = ZEPHYRUM;

    dum (i < c.mensura)
    {
        i32 a;

        dum (i < c.mensura && c.datum[i] == ' ')
        {
            i = i + I;
        }
        si (i >= c.mensura)
        {
            frange;
        }
        a = i;
        dum (i < c.mensura && c.datum[i] != ' ')
        {
            i = i + I;
        }
        si (k == n)
        {
            redde _chorda(c.datum + a, i - a);
        }
        k = k + I;
    }
    redde _chorda(NIHIL, ZEPHYRUM);
}

interior s32
_ex_tabula (
    constans character* constans* codices,
                           chorda  codex)
{
    i32 i;

    per (i = ZEPHYRUM; codices[i] != NIHIL; i++)
    {
        si (_aequalis(codex, codices[i]))
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}

/* codices WORDS ordine enumerationum oratio_registrum.h */
hic_manens constans character* constans CODICES_CASUUM[] = {
    "NOM", "GEN", "DAT", "ACC", "ABL", "LOC", "VOC", NIHIL
};
hic_manens constans character* constans CODICES_NUMERORUM[] = {
    "S", "P", NIHIL
};
hic_manens constans character* constans CODICES_GENERUM[] = {
    "M", "F", "N", "C", NIHIL
};
hic_manens constans character* constans CODICES_PERSONARUM[] = {
    "1", "2", "3", NIHIL
};
hic_manens constans character* constans CODICES_TEMPORUM[] = {
    "PRES", "IMPF", "FUT", "PERF", "PLUP", "FUTP", NIHIL
};
hic_manens constans character* constans CODICES_VOCUM[] = {
    "ACTIVE", "PASSIVE", NIHIL
};
hic_manens constans character* constans CODICES_MODORUM[] = {
    "IND", "SUB", "IMP", "INF", NIHIL
};
hic_manens constans character* constans CODICES_GRADUUM[] = {
    "POS", "COMP", "SUPER", NIHIL
};
hic_manens constans character* constans CODICES_SPECIERUM[] = {
    "CARD", "ORD", "DIST", "ADVERB", NIHIL
};

s32
oratio_partes_la_casus (
    chorda codex)
{
    redde _ex_tabula(CODICES_CASUUM, codex);
}

s32
oratio_partes_la_numerus (
    chorda codex)
{
    redde _ex_tabula(CODICES_NUMERORUM, codex);
}

s32
oratio_partes_la_genus (
    chorda codex)
{
    redde _ex_tabula(CODICES_GENERUM, codex);
}

s32
oratio_partes_la_persona (
    chorda codex)
{
    redde _ex_tabula(CODICES_PERSONARUM, codex);
}

s32
oratio_partes_la_tempus (
    chorda codex)
{
    redde _ex_tabula(CODICES_TEMPORUM, codex);
}

s32
oratio_partes_la_vox (
    chorda codex)
{
    redde _ex_tabula(CODICES_VOCUM, codex);
}

s32
oratio_partes_la_modus (
    chorda codex)
{
    redde _ex_tabula(CODICES_MODORUM, codex);
}

s32
oratio_partes_la_gradus (
    chorda codex)
{
    redde _ex_tabula(CODICES_GRADUUM, codex);
}

s32
oratio_partes_la_species (
    chorda codex)
{
    redde _ex_tabula(CODICES_SPECIERUM, codex);
}

s32
oratio_partes_index_tituli (
    constans character* constans* tituli,
                              i32  n,
                           chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        si (_aequalis(titulus, tituli[i]))
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}

vacuum
oratio_descriptio_vacare (
    OratioDescriptio* d)
{
    memset(d, ZEPHYRUM, magnitudo(*d));
    d->classis            = ORATIO_CLASSIS_IGNOTUM;
    d->casus_grammaticus  = (s32)-I;
    d->numerus            = (s32)-I;
    d->genus              = (s32)-I;
    d->persona            = (s32)-I;
    d->tempus             = (s32)-I;
    d->modus              = (s32)-I;
    d->vox                = (s32)-I;
    d->forma_verbi        = (s32)-I;
    d->gradus             = (s32)-I;
    d->species            = (s32)-I;
    d->declinatio         = (s32)-I;
    d->coniugatio         = (s32)-I;
    d->fons               = ORATIO_FONS_ANALYSIS_VOCABULARIUM_LA;
    d->lingua             = ORATIO_LINGUA_LATINA;
}

/* casus numerus genus ex lexematibus a, a+1, a+2 */
interior vacuum
_casum_numerum_genus_ponere (
    OratioDescriptio* d,
              chorda  accidentia,
                 i32  a)
{
    d->casus_grammaticus = oratio_partes_la_casus(_lexema(accidentia,
        a));
    d->numerus = oratio_partes_la_numerus(_lexema(accidentia, a + I));
    si (d->genus < ZEPHYRUM)
    {
        d->genus = oratio_partes_la_genus(_lexema(accidentia,
            a + (i32)II));
    }
}

interior b32
_subordinans (
    chorda lemma)
{
    redde (b32)(_ex_tabula(ORATIO_CONIUNCTIONES_SUBORDINANTES, lemma)
        >= ZEPHYRUM);
}

/* descriptio ex parte, accidentibus (chorda WORDS), stirpe (NIHIL pro
 * unicis) et lemmate */
interior vacuum
_describere_codicem (
           OratioDescriptio* d,
                 OratioPars  pars,
                     chorda  accidentia,
      constans OratioStirps* stirps,
                        s32  declinatio,
                     chorda  species)
{
    commutatio (pars)
    {
        casus ORATIO_PARS_N:
        {
            chorda natura = _lexema(species, I);

            d->classis = (_aequalis(natura, "N")
                || _aequalis(natura, "L"))
                ? ORATIO_CLASSIS_NOMEN_PROPRIUM
                : ORATIO_CLASSIS_SUBSTANTIVUM;
            d->genus = oratio_partes_la_genus(_lexema(species,
                ZEPHYRUM));
            si (d->genus == (s32)ORATIO_GENUS_GRAMMATICUM_COMMUNE)
            {
                d->genus = (s32)-I;   /* stirps C: flexio decernit */
            }
            _casum_numerum_genus_ponere(d, accidentia, ZEPHYRUM);
            d->declinatio = declinatio;
            frange;
        }
        casus ORATIO_PARS_PRON:
        casus ORATIO_PARS_PACK:
            d->classis = ORATIO_CLASSIS_PRONOMEN;
            _casum_numerum_genus_ponere(d, accidentia, ZEPHYRUM);
            frange;
        casus ORATIO_PARS_ADJ:
            d->classis = ORATIO_CLASSIS_ADIECTIVUM;
            _casum_numerum_genus_ponere(d, accidentia, ZEPHYRUM);
            d->gradus     = oratio_partes_la_gradus(_lexema(accidentia,
                (i32)III));
            d->declinatio = declinatio;
            frange;
        casus ORATIO_PARS_NUM:
            d->classis = ORATIO_CLASSIS_NUMERALE;
            _casum_numerum_genus_ponere(d, accidentia, ZEPHYRUM);
            d->species = oratio_partes_la_species(_lexema(accidentia,
                (i32)III));
            frange;
        casus ORATIO_PARS_ADV:
            d->classis = ORATIO_CLASSIS_ADVERBIUM;
            d->gradus  = oratio_partes_la_gradus(_lexema(accidentia,
                ZEPHYRUM));
            frange;
        casus ORATIO_PARS_V:
        {
            chorda natura = _lexema(species, ZEPHYRUM);

            d->classis  = ORATIO_CLASSIS_VERBUM;
            d->tempus   = oratio_partes_la_tempus(_lexema(accidentia,
                ZEPHYRUM));
            d->vox      = oratio_partes_la_vox(_lexema(accidentia, I));
            d->modus    = oratio_partes_la_modus(_lexema(accidentia,
                (i32)II));
            d->persona  = oratio_partes_la_persona(_lexema(accidentia,
                (i32)III));
            d->numerus  = oratio_partes_la_numerus(_lexema(accidentia,
                (i32)IV));
            d->forma_verbi = (d->modus == (s32)ORATIO_MODUS_INFINITIVUS)
                ? (s32)ORATIO_FORMA_VERBI_INFINITIVUM
                : (s32)ORATIO_FORMA_VERBI_FINITUM;
            si (   _aequalis(natura, "DEP")
                || _aequalis(natura, "SEMIDEP"))
            {
                d->vox = (s32)ORATIO_VOX_DEPONENS;
            }
            d->coniugatio = declinatio;
            frange;
        }
        casus ORATIO_PARS_VPAR:
        {
            chorda natura = _lexema(species, ZEPHYRUM);

            d->classis = ORATIO_CLASSIS_VERBUM;
            _casum_numerum_genus_ponere(d, accidentia, ZEPHYRUM);
            d->tempus = oratio_partes_la_tempus(_lexema(accidentia,
                (i32)III));
            d->vox    = oratio_partes_la_vox(_lexema(accidentia,
                (i32)IV));
            d->forma_verbi = (   d->tempus == (s32)ORATIO_TEMPUS_FUTURUM
                              && d->vox == (s32)ORATIO_VOX_PASSIVA)
                ? (s32)ORATIO_FORMA_VERBI_GERUNDIVUM
                : (s32)ORATIO_FORMA_VERBI_PARTICIPIUM;
            si (   _aequalis(natura, "DEP")
                || _aequalis(natura, "SEMIDEP"))
            {
                d->vox = (s32)ORATIO_VOX_DEPONENS;
            }
            d->coniugatio = declinatio;
            frange;
        }
        casus ORATIO_PARS_SUPINE:
            d->classis = ORATIO_CLASSIS_VERBUM;
            _casum_numerum_genus_ponere(d, accidentia, ZEPHYRUM);
            d->forma_verbi = (s32)ORATIO_FORMA_VERBI_SUPINUM;
            d->coniugatio  = declinatio;
            frange;
        casus ORATIO_PARS_PREP:
        {
            chorda c = _lexema(accidentia, ZEPHYRUM);

            d->classis = ORATIO_CLASSIS_ADPOSITIO;
            si (c.mensura == ZEPHYRUM)
            {
                c = _lexema(species, ZEPHYRUM);
            }
            d->casus_grammaticus = oratio_partes_la_casus(c);
            frange;
        }
        casus ORATIO_PARS_CONJ:
            d->classis = _subordinans(d->lemma)
                ? ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS
                : ORATIO_CLASSIS_CONIUNCTIO_COORDINANS;
            frange;
        casus ORATIO_PARS_INTERJ:
            d->classis = ORATIO_CLASSIS_INTERIECTIO;
            frange;
        casus ORATIO_PARS_TACKON:
            d->classis = _aequalis(d->lemma, "ne")
                ? ORATIO_CLASSIS_PARTICULA
                : ORATIO_CLASSIS_CONIUNCTIO_COORDINANS;
            frange;
        ordinarius:
            d->classis = ORATIO_CLASSIS_IGNOTUM;
            frange;
    }
    (vacuum)stirps;
}

/* nativum: "PARS decl var accidentia" (copia) */
interior chorda
_nativum (
       Piscina* piscina,
    OratioPars  pars,
           i32  declinatio,
           i32  varians,
        chorda  accidentia,
           b32  declinata)
{
     constans character* t = oratio_pars_titulus(pars);
              character  buffer[256];
                    i32  n;

    si (declinata)
    {
        n = (i32)sprintf(buffer, "%s %d %d ", t, (integer)declinatio,
            (integer)varians);
    }
    alioquin
    {
        n = (i32)sprintf(buffer, "%s ", t);
    }
    si (   accidentia.mensura > ZEPHYRUM
        && n + accidentia.mensura < (i32)255)
    {
        memcpy(buffer + n, accidentia.datum,
            (size_t)accidentia.mensura);
        n = n + accidentia.mensura;
    }
    dum (n > ZEPHYRUM && buffer[n - I] == ' ')
    {
        n = n - I;
    }
    buffer[n] = '\0';
    redde _copia(piscina, _ex_literis(buffer));
}

interior b32
_declinata (
    OratioPars pars)
{
    redde (b32)(pars == ORATIO_PARS_N || pars == ORATIO_PARS_PRON
        || pars == ORATIO_PARS_PACK || pars == ORATIO_PARS_ADJ
        || pars == ORATIO_PARS_NUM || pars == ORATIO_PARS_V
        || pars == ORATIO_PARS_VPAR || pars == ORATIO_PARS_SUPINE);
}

/* forma unica: descriptio "PARS [decl var] accidentia... codices" -
 * accidentia usque ad lexema quod natura aut codicem aetatis est: numerus
 * lexematum accidentium per partem fixus */
interior i32
_accidentia_numerus (
    OratioPars pars)
{
    commutatio (pars)
    {
        casus ORATIO_PARS_N: casus ORATIO_PARS_PRON: casus ORATIO_PARS_PACK:
        casus ORATIO_PARS_SUPINE:
            redde (i32)III;
        casus ORATIO_PARS_ADJ: casus ORATIO_PARS_NUM:
            redde (i32)IV;
        casus ORATIO_PARS_ADV: casus ORATIO_PARS_PREP:
            redde I;
        casus ORATIO_PARS_V:
            redde (i32)V;
        casus ORATIO_PARS_VPAR:
            redde (i32)VI;
        ordinarius:
            redde ZEPHYRUM;
    }
}

interior b32
_descriptionem_addere (
                 Xar* exitus,
    OratioDescriptio* d)
{
    OratioDescriptio* locus = (OratioDescriptio*)xar_addere(exitus);

    si (locus == NIHIL)
    {
        redde FALSUM;
    }
    *locus = *d;
    redde VERUM;
}

b32
oratio_partes_la_describere (
                          Piscina* piscina,
    constans OratioVocabulariumLa* voc,
          constans OratioAnalysis* a,
                              Xar* exitus)
{
    OratioDescriptio d;

    oratio_descriptio_vacare(&d);
    commutatio (a->genus)
    {
        casus ORATIO_ANALYSIS_STIRPS:
        {
            constans OratioStirps* stirps =
                oratio_vocabularium_la_stirps(voc,
                a->stirps);
            constans OratioFlexio* flexio =
                oratio_vocabularium_la_flexio(voc,
                a->flexio);

            d.lemma = oratio_vocabularium_la_lemma(piscina, voc,
                a->stirps);
            d.sensus = stirps->sensus;
            d.nativum = _nativum(piscina, flexio->pars,
                stirps->declinatio,
                stirps->varians, flexio->accidentia,
                _declinata(flexio->pars));
            _describere_codicem(&d, flexio->pars, flexio->accidentia,
                stirps,
                _declinata(flexio->pars) ? (s32)stirps->declinatio : (s32)-I,
                stirps->species);
            frange;
        }
        casus ORATIO_ANALYSIS_UNICUM:
        {
            constans OratioUnicum* u =
                oratio_vocabularium_la_unicum(voc,
                a->unicum);
            OratioPars pars =
                oratio_pars_ex_titulo(_lexema(u->descriptio,
                ZEPHYRUM));
               i32 a0          = I;
               s32 declinatio  = (s32)-I;
            chorda accidentia;
            chorda species;
               i32 n;

            d.lemma    = _copia(piscina, u->forma);
            d.sensus   = u->sensus;
            d.nativum  = _copia(piscina, u->descriptio);
            si (_declinata(pars))
            {
                chorda littera = _lexema(u->descriptio, I);

                declinatio = (littera.mensura == I
                    && littera.datum[ZEPHYRUM] >= '0'
                    && littera.datum[ZEPHYRUM] <= '9')
                    ? (s32)(littera.datum[ZEPHYRUM] - '0') : (s32)-I;
                a0 = (i32)III;
            }
            /* accidentia = lexemata a0 .. a0 + n - I, species = sequens */
            n = _accidentia_numerus(pars);
            {
                chorda primum = _lexema(u->descriptio, a0);
                chorda ultimum = n > ZEPHYRUM
                    ? _lexema(u->descriptio, a0 + n - I) : primum;

                si (   primum.datum != NIHIL && ultimum.datum != NIHIL
                    && n > ZEPHYRUM)
                {
                    accidentia = _chorda(primum.datum,
                        (i32)(ultimum.datum + ultimum.mensura
                            - primum.datum));
                }
                alioquin
                {
                    accidentia = _chorda(NIHIL, ZEPHYRUM);
                }
            }
            species = _lexema(u->descriptio, a0 + n);
            _describere_codicem(&d, pars, accidentia, NIHIL, declinatio,
                species);
            frange;
        }
        casus ORATIO_ANALYSIS_TACKON:
        {
            constans OratioAdditamentum* t =
                oratio_vocabularium_la_additamentum(voc, a->tackon);
            character buffer[128];

            d.lemma   = _copia(piscina, t->forma);
            d.sensus  = t->sensus;
            sprintf(buffer, "TACKON %.*s", (integer)(t->forma.mensura
                < (i32)100
                ? t->forma.mensura : (i32)100),
                (constans character*)t->forma.datum);
            d.nativum = _copia(piscina, _ex_literis(buffer));
            _describere_codicem(&d, ORATIO_PARS_TACKON, _chorda(NIHIL,
                ZEPHYRUM),
                NIHIL, (s32)-I, _chorda(NIHIL, ZEPHYRUM));
            frange;
        }
        casus ORATIO_ANALYSIS_GLOSSARIUM:
        {
            constans OratioGlossarium* gl =
                oratio_vocabularium_la_glossarium(
                voc);
            constans OratioGlossariumForma* f =
                oratio_glossarium_forma(gl,
                a->glossarium);
            constans OratioGlossariumEntrium* e =
                oratio_glossarium_entrium(gl,
                f->entrium);
            character buffer[128];

            d.classis = oratio_classis_ex_titulo(
                (constans character*)e->classis.datum,
                e->classis.mensura);
            si ((i32)d.classis >= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM)
            {
                d.classis = ORATIO_CLASSIS_IGNOTUM;   /* ignotum-permissum */
            }
            d.lemma   = _copia(piscina, e->lemma);
            d.sensus  = e->sensus;
            d.fons    = ORATIO_FONS_ANALYSIS_GLOSSARIUM;
            d.lingua = _aequalis(e->lingua,
                "anglica") ? ORATIO_LINGUA_ANGLICA
                : ORATIO_LINGUA_LATINA;
            sprintf(buffer, "glossarium %.*s",
                (integer)(e->classis.mensura
                < (i32)100 ? e->classis.mensura : (i32)100),
                (constans character*)e->classis.datum);
            d.nativum = _copia(piscina, _ex_literis(buffer));
            d.casus_grammaticus =
                oratio_partes_index_tituli(ORATIO_TITULI_CASUUM,
                (i32)ORATIO_CASUS_NUMERUS, f->casus_grammaticus);
            d.numerus =
                oratio_partes_index_tituli(ORATIO_TITULI_NUMERORUM,
                (i32)ORATIO_NUMERUS_GRAMMATICUS_NUMERUS, f->numerus);
            d.genus = oratio_partes_index_tituli(
                ORATIO_TITULI_GENERUM_GRAMMATICORUM,
                (i32)ORATIO_GENUS_GRAMMATICUM_NUMERUS, f->genus);
            d.persona =
                oratio_partes_index_tituli(ORATIO_TITULI_PERSONARUM,
                (i32)ORATIO_PERSONA_NUMERUS, f->persona);
            d.tempus =
                oratio_partes_index_tituli(ORATIO_TITULI_TEMPORUM,
                (i32)ORATIO_TEMPUS_NUMERUS, f->tempus);
            d.modus = oratio_partes_index_tituli(ORATIO_TITULI_MODORUM,
                (i32)ORATIO_MODUS_NUMERUS, f->modus);
            d.vox = oratio_partes_index_tituli(ORATIO_TITULI_VOCUM,
                (i32)ORATIO_VOX_NUMERUS, f->vox);
            d.forma_verbi = oratio_partes_index_tituli(
                ORATIO_TITULI_FORMARUM_VERBI,
                (i32)ORATIO_FORMA_VERBI_NUMERUS,
                f->forma_verbi);
            d.gradus = oratio_partes_index_tituli(ORATIO_TITULI_GRADUUM,
                (i32)ORATIO_GRADUS_NUMERUS, f->gradus);
            si (   d.classis == ORATIO_CLASSIS_VERBUM
                && d.forma_verbi < ZEPHYRUM
                && d.persona >= ZEPHYRUM)
            {
                d.forma_verbi = (s32)ORATIO_FORMA_VERBI_FINITUM;
            }
            frange;
        }
        ordinarius:
            frange;
    }
    redde _descriptionem_addere(exitus, &d);
}
