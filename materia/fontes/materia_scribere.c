/* materia_scribere.c - Emissio arborum ad octetos
 *
 * Vide materia_scribere.h pro duabus correctionibus ad spec.
 *
 * UNIFICATIO QUAM FURCA PERMISIT. silva_scribere.c:286 duas semitas
 * per _lexema_scribere fert: ramum FONS (lexema se emittit) et
 * ramum EXPANSIONIS (radix loco eius, cum extento et deduplicatione).
 * Hic UNA semita est: radix quaeritur (sine unco = lexema ipsum),
 * deinde 'an radix lexema ipsum sit' semitas distinguit. Ramus FONS
 * silvae est casus in quo radix == lexema. Nihil amissum, ramus unus
 * minus, et lingua sine derivatione semitam eandem currit.
 */

#include "materia_scribere.h"

#include <string.h>


/* ==================================================
 * Status scriptoris
 * ================================================== */

nomen structura {
    ChordaAedificator*               aed;
    constans MateriaScripturaConsilium* consilium;
    s32                              genus_ambigui;   /* -I = absens */
    i32                              locus_interpretationum;
    i32                              locus_canonicae;
    i32                              reinserenda_index;
    s32                              fons_ultimus;    /* -II = nulla adhuc */
    s32                              emissum_usque;
    b32                              successus;
    constans character*              causa;
    constans MateriaNodus*           sedes;
} MateriaScriptor;

interior vacuum
_valorem_scribere (
    MateriaScriptor* st,
       MateriaValor  valor);
interior vacuum
_nodum_scribere (
          MateriaScriptor* st,
    constans MateriaNodus* nodus);

interior vacuum
_fractura (
          MateriaScriptor* st,
       constans character* causa,
    constans MateriaNodus* sedes)
{
    si (st->successus)
    {
        st->successus  = FALSUM;
        st->causa      = causa;
        st->sedes      = sedes;
    }
}


/* ==================================================
 * Forma ambigui - OPTIONALIS (correctio I)
 * ================================================== */

/* VERUM = registrum sanum (ambiguo praesente et integro, AUT
 * omnino absente). FALSUM = ambiguus adest sed locis caret -
 * distinctio inter 'non habeo' et 'fractum habeo'. */
interior b32
_formam_ambigui_invenire (
    MateriaScriptor* st)
{
    constans MateriaRegistrumCoctum* tab = st->consilium->tabularium;
                                i32  g;

    st->genus_ambigui           = (s32)-I;
    st->locus_interpretationum  = ZEPHYRUM;
    st->locus_canonicae         = ZEPHYRUM;

    per (g = ZEPHYRUM; g < tab->numerus_generum; g++)
    {
        constans MateriaTabGenus* def = &tab->genera[g];
                             i32  k;
                             s32  interp;
                             s32  canon;

        si (   def->titulus                     == NIHIL
            || strcmp(def->titulus, "ambiguus") != ZEPHYRUM)
        {
            perge;
        }

        interp  = (s32)-I;
        canon   = (s32)-I;
        per (k = ZEPHYRUM; k < def->loci_numerus; k++)
        {
            constans MateriaTabLocus* locus =
                &tab->loci[def->loci_offset + k];

            si (locus->titulus == NIHIL) perge;
            si (strcmp(locus->titulus, "interpretationes") == ZEPHYRUM)
            {
                interp = (s32)k;
            }
            si (strcmp(locus->titulus, "canonica") == ZEPHYRUM)
            {
                canon = (s32)k;
            }
        }
        si (interp < ZEPHYRUM || canon < ZEPHYRUM)
        {
            redde FALSUM;   /* adest sed fractum - RECUSA */
        }
        st->genus_ambigui           = (s32)g;
        st->locus_interpretationum  = (i32)interp;
        st->locus_canonicae         = (i32)canon;
        redde VERUM;
    }
    redde VERUM;   /* absens - lingua ambiguitatem non habet */
}


/* ==================================================
 * Emissio octetorum
 * ================================================== */

interior vacuum
_lexema_emittere (
          MateriaScriptor* st,
    constans MateriaToken* token)
{
    i32 i;

    si (token == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < token->numerus_ante; i++)
    {
        constans MateriaToken* t = token->spatia_ante[i];

        si (t != NIHIL)
        {
            chorda_aedificator_appendere_chorda(st->aed, t->valor);
        }
    }
    si (st->consilium->valorem_emittere != NIHIL)
    {
        si (!st->consilium->valorem_emittere(
                 st->consilium->valorem_datum, st->aed, token))
        {
            _fractura(st, "frons valorem lexematis emittere non potuit",
                NIHIL);
            redde;
        }
    }
    alioquin
    {
        chorda_aedificator_appendere_chorda(st->aed, token->valor);
    }
    per (i = ZEPHYRUM; i < token->numerus_post; i++)
    {
        constans MateriaToken* t = token->spatia_post[i];

        si (t != NIHIL)
        {
            chorda_aedificator_appendere_chorda(st->aed, t->valor);
        }
    }
}

interior vacuum
_laminam_emittere (
    MateriaScriptor* st,
                Xar* lamina)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(lamina); k++)
    {
        _lexema_emittere(st, *(MateriaToken**)xar_obtinere(lamina, k));
    }
}

/* Fundere reinserenda quorum offset < ante */
interior vacuum
_reinserenda_fundere (
    MateriaScriptor* st,
                s32  ante)
{
    Xar* r_xar = st->consilium->reinserenda;

    si (r_xar == NIHIL)
    {
        redde;
    }
    dum (st->reinserenda_index < xar_numerus(r_xar))
    {
        MateriaReinserendum* r = (MateriaReinserendum*)xar_obtinere(
            r_xar, st->reinserenda_index);

        si (r == NIHIL || r->offset >= ante)
        {
            frange;
        }
        _laminam_emittere(st, r->lamina);
        st->reinserenda_index++;
    }
}

s32
materia_reinserenda_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans MateriaReinserendum* ra = (constans MateriaReinserendum*)a;
    constans MateriaReinserendum* rb = (constans MateriaReinserendum*)b;

    si (ra->offset < rb->offset)
    { redde (s32)-I;
    }
    si (ra->offset > rb->offset)
    { redde (s32)I;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Lexema - SEMITA UNA
 * ================================================== */

interior vacuum
_lexema_scribere (
    MateriaScriptor* st,
       MateriaToken* token)
{
    constans MateriaOrigoUncus* uncus;
                  MateriaToken* radix;
            constans character* causa;

    si (!st->successus || token == NIHIL)
    {
        redde;
    }

    uncus = st->consilium->origo;
    radix = token;
    si (uncus != NIHIL && uncus->radix_quaerere != NIHIL)
    {
        causa = NIHIL;
        radix = uncus->radix_quaerere(uncus->datum, token, &causa);
        si (radix == NIHIL)
        {
            _fractura(st, (causa != NIHIL)
                ? causa
                : "radix emissionis recuperari non potest", NIHIL);
            redde;
        }
    }

    /* Plagula aliena (limes includendi): octeti eius reconstructioni
     * SUAE pertinent. */
    si (   st->consilium->fons_index >= ZEPHYRUM
        && radix->fons_index         != st->consilium->fons_index)
    {
        redde;
    }

    si (radix != token)
    {
        /* Lexema DERIVATUM. Intra unitatem iam emissam? (corpus
         * multi-lexematis eiusdem invocationis) */
        si (   radix->fons_index == st->fons_ultimus
            && radix->byte_offset < st->emissum_usque)
        {
            redde;
        }
        si (uncus->extentum_quaerere != NIHIL)
        {
            Xar* extentum = uncus->extentum_quaerere(uncus->datum,
                radix);

            si (extentum != NIHIL && xar_numerus(extentum) > ZEPHYRUM)
            {
                MateriaToken* t = NIHIL;
                         i32  k;

                /* Fusio reinserendorum INTRA extentum manet
                 * possibilis (C89: directiva intra argumenta). */
                per (k = ZEPHYRUM; k < xar_numerus(extentum); k++)
                {
                    t = *(MateriaToken**)xar_obtinere(extentum, k);
                    si (t == NIHIL)
                    { perge;
                    }
                    _reinserenda_fundere(st, t->byte_offset);
                    _lexema_emittere(st, t);
                }
                si (t != NIHIL)
                {
                    st->fons_ultimus  = radix->fons_index;
                    st->emissum_usque = t->byte_offset
                                      + (s32)t->valor.mensura;
                }
                redde;
            }
        }
    }

    _reinserenda_fundere(st, radix->byte_offset);
    _lexema_emittere(st, radix);
    st->fons_ultimus   = radix->fons_index;
    st->emissum_usque  = radix->byte_offset + (s32)radix->valor.mensura;
}


/* ==================================================
 * Ambulatio
 * ================================================== */

interior vacuum
_ambiguum_scribere (
          MateriaScriptor* st,
    constans MateriaNodus* nodus)
{
    MateriaValor  interp;
    MateriaValor  canonica;
    MateriaValor* electa;

    si (   st->locus_interpretationum >= nodus->numerus_locorum
        || st->locus_canonicae        >= nodus->numerus_locorum)
    {
        _fractura(st, "forma ambigui extra loci nodi", nodus);
        redde;
    }
    interp    = nodus->loci[st->locus_interpretationum];
    canonica  = nodus->loci[st->locus_canonicae];

    si (   interp.genus   != MATERIA_VALOR_LISTA
        || canonica.genus != MATERIA_VALOR_INDEX)
    {
        _fractura(st, "forma ambigui corrupta", nodus);
        redde;
    }
    si (   canonica.datum.index < ZEPHYRUM
        || canonica.datum.index
               >= (s32)materia_valor_lista_numerus(interp))
    {
        _fractura(st, "canonica extra fines interpretationum", nodus);
        redde;
    }
    electa = materia_valor_lista_obtinere(interp,
        (i32)canonica.datum.index);
    si (electa == NIHIL)
    {
        _fractura(st, "interpretatio canonica deest", nodus);
        redde;
    }
    _valorem_scribere(st, *electa);
}

interior vacuum
_nodum_scribere (
          MateriaScriptor* st,
    constans MateriaNodus* nodus)
{
    i32 k;

    si (!st->successus || nodus == NIHIL)
    {
        redde;
    }
    si (   nodus->genus < ZEPHYRUM
        || nodus->genus
               >= (s32)st->consilium->tabularium->numerus_generum)
    {
        _fractura(st, "genus ignotum", nodus);
        redde;
    }

    /* AMBIGUUS: sola interpretatio canonica - emissio omnium
     * octetos duplicaret (lexemata communia). genus_ambigui = -I
     * (lingua sine ambiguitate) huic numquam congruit, quia genus
     * nodi validum >= ZEPHYRUM est. */
    si (nodus->genus == st->genus_ambigui)
    {
        _ambiguum_scribere(st, nodus);
        redde;
    }

    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _valorem_scribere(st, nodus->loci[k]);
    }
}

interior vacuum
_valorem_scribere (
    MateriaScriptor* st,
       MateriaValor  valor)
{
    si (!st->successus)
    {
        redde;
    }
    commutatio (valor.genus)
    {
        casus MATERIA_VALOR_NIHIL:
        casus MATERIA_VALOR_INDEX:
            frange;
        casus MATERIA_VALOR_TOKEN:
            _lexema_scribere(st, valor.datum.token);
            frange;
        casus MATERIA_VALOR_NODUS:
            _nodum_scribere(st, valor.datum.nodus);
            frange;
        casus MATERIA_VALOR_LISTA:
        {
            i32 i;
            i32 n = materia_valor_lista_numerus(valor);

            per (i = ZEPHYRUM; i < n; i++)
            {
                MateriaValor* elem = materia_valor_lista_obtinere(valor,
                    i);

                si (elem != NIHIL)
                {
                    _valorem_scribere(st, *elem);
                }
            }
            frange;
        }
        ordinarius:
            _fractura(st, "signum valoris ignotum", NIHIL);
            frange;
    }
}


/* ==================================================
 * Compositio
 * ================================================== */

vacuum
materia_scriptura_consilium_nudum (
          MateriaScripturaConsilium* consilium,
    constans MateriaRegistrumCoctum* tabularium)
{
    si (consilium == NIHIL)
    {
        redde;
    }
    /* MEMSET PRIMUM: campus consilio additus sed hic oblitus
     * garbage fert. Deinde campi quorum ordinarium NON zephyrum est. */
    memset(consilium, ZEPHYRUM, magnitudo(*consilium));
    consilium->tabularium = tabularium;
    consilium->fons_index = (s32)-I;
}

interior MateriaScriptura
_scriptura_fracta (
    constans character* causa)
{
    MateriaScriptura s;

    s.successus       = FALSUM;
    s.textus.datum    = NIHIL;
    s.textus.mensura  = ZEPHYRUM;
    s.causa           = causa;
    s.sedes           = NIHIL;
    redde s;
}

interior b32
_scriptor_parare (
                       MateriaScriptor* st,
                               Piscina* piscina,
    constans MateriaScripturaConsilium* consilium)
{
    st->aed = chorda_aedificator_creare(piscina,
        1024);
    st->consilium               = consilium;
    st->reinserenda_index       = ZEPHYRUM;
    st->fons_ultimus            = (s32)-II;
    st->emissum_usque           = ZEPHYRUM;
    st->successus               = VERUM;
    st->causa                   = NIHIL;
    st->sedes                   = NIHIL;
    st->genus_ambigui           = (s32)-I;
    st->locus_interpretationum  = ZEPHYRUM;
    st->locus_canonicae         = ZEPHYRUM;

    si (st->aed == NIHIL)
    {
        redde FALSUM;
    }
    redde _formam_ambigui_invenire(st);
}

interior MateriaScriptura
_scriptura_finire (
    MateriaScriptor* st)
{
    MateriaScriptura s;

    s.successus  = st->successus;
    s.causa      = st->causa;
    s.sedes      = st->sedes;
    si (st->successus)
    {
        s.textus = chorda_aedificator_finire(st->aed);
    }
    alioquin
    {
        s.textus.datum    = NIHIL;
        s.textus.mensura  = ZEPHYRUM;
    }
    redde s;
}

MateriaScriptura
materia_scribere_valorem (
                               Piscina* piscina,
                          MateriaValor  valor,
    constans MateriaScripturaConsilium* consilium)
{
    MateriaScriptor st;

    si (   piscina               == NIHIL || consilium == NIHIL
        || consilium->tabularium == NIHIL)
    {
        redde _scriptura_fracta("argumenta nulla");
    }
    si (!_scriptor_parare(&st, piscina, consilium))
    {
        redde _scriptura_fracta(
            "registrum genus 'ambiguus' fractum fert (locis carens)");
    }
    _valorem_scribere(&st, valor);
    redde _scriptura_finire(&st);
}

MateriaScriptura
materia_scribere_nodum (
                               Piscina* piscina,
                 constans MateriaNodus* nodus,
    constans MateriaScripturaConsilium* consilium)
{
    MateriaScriptor st;

    si (   piscina == NIHIL || nodus == NIHIL || consilium == NIHIL
        || consilium->tabularium == NIHIL)
    {
        redde _scriptura_fracta("argumenta nulla");
    }
    si (!_scriptor_parare(&st, piscina, consilium))
    {
        redde _scriptura_fracta(
            "registrum genus 'ambiguus' fractum fert (locis carens)");
    }
    _nodum_scribere(&st, nodus);
    redde _scriptura_finire(&st);
}
