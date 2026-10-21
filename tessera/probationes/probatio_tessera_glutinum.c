/* probatio_tessera_glutinum.c - Glutinum (?2004) magnum et limites
 *
 * Quae tabula vectorum (CCLVI octeti) capere non potest:
 *   - C milia octetorum: capacitas, truncatio, clavis post terminum
 *   - limes exactus (capacitas) et capacitas + I
 *   - M particulae VI octetorum: corpus integrum; mora lectionum
 *     intra glutinum = TESSERA_MORA_GLUTINI_MS (non mora vocantis)
 *   - MB unum: tempus impressum (lectiones per bufferem LXIV)
 *   - status stabilis: glutinum nihil allocat (collector praeparatus)
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_eventum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define INITIUM_GLUTINI "\033[200~"
#define TERMINUS_GLUTINI "\033[201~"
#define MARCA_LONGITUDO VI


/* ==================================================
 * Initum: INITIUM + numerus x octetus + TERMINUS + post
 * ================================================== */

interior i8*
_initum_parare (
               Piscina* piscina,
                   i32  numerus,
                    i8  octetus,
                   b32  cum_termino,
    constans character* post,
                   i32* mensura_out)
{
    i32 post_mensura = (i32)strlen(post);
    i32 mensura = MARCA_LONGITUDO + numerus
                + (cum_termino ? MARCA_LONGITUDO : ZEPHYRUM)
                + post_mensura;
     i8* o = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    i32  k = ZEPHYRUM;

    memcpy(o, INITIUM_GLUTINI, MARCA_LONGITUDO);
    k += MARCA_LONGITUDO;
    memset(o + k, octetus, (memoriae_index)numerus);
    k += numerus;
    si (cum_termino)
    {
        memcpy(o + k, TERMINUS_GLUTINI, MARCA_LONGITUDO);
        k += MARCA_LONGITUDO;
    }
    memcpy(o + k, post, (memoriae_index)post_mensura);
    *mensura_out = mensura;
    redde o;
}

/* Corpus totum ex octeto uno? */
interior b32
_corpus_uniforme (
    chorda corpus,
        i8 octetus)
{
    i32 k;

    per (k = ZEPHYRUM; k < corpus.mensura; k++)
    {
        si (corpus.datum[k] != octetus)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Pons particularum: octeti in particulis fixis; mora quaque
 * lectione notatur
 * ================================================== */

nomen structura {
    TesseraPons  pons;
    constans i8* octeti;
            i32  mensura;
            i32  cursor;
            i32  particula;
            i32  lectiones_glutini;   /* mora == TESSERA_MORA_GLUTINI_MS */
            i32  lectiones_aliae;
} PonsParticularum;

interior s32
_particulas_legere (
    vacuum* datum,
        i8* buffer,
       i32  capacitas,
       s32  mora_ms)
{
    PonsParticularum* pp = (PonsParticularum*)datum;
                 i32  n;

    si (mora_ms == TESSERA_MORA_GLUTINI_MS)
    {
        pp->lectiones_glutini++;
    }
    alioquin
    {
        pp->lectiones_aliae++;
    }
    si (pp->cursor >= pp->mensura)
    {
        redde ZEPHYRUM;
    }
    n = pp->mensura - pp->cursor;
    si (n > pp->particula)
    {
        n = pp->particula;
    }
    si (n > capacitas)
    {
        n = capacitas;
    }
    memcpy(buffer, pp->octeti + pp->cursor, (memoriae_index)n);
    pp->cursor += n;
    redde (s32)n;
}

interior b32
_particulas_scribere (
         vacuum* datum,
    constans i8* octeti,
            i32  numerus)
{
    (vacuum)datum;
    (vacuum)octeti;
    (vacuum)numerus;
    redde VERUM;
}

interior b32
_particulas_amplitudo (
    vacuum* datum,
       i32* latitudo_out,
       i32* altitudo_out)
{
    (vacuum)datum;
    *latitudo_out = LXXX;
    *altitudo_out = XXIV;
    redde VERUM;
}

interior b32
_particulas_status (
    vacuum* datum)
{
    (vacuum)datum;
    redde VERUM;
}


/* ==================================================
 * Mundus probationis: pons memoriae + lector. Sectio quaeque suum
 * aedificat (initum et status lectoris mutantur - status nullus
 * communis); creatio semel ASSERITUR in parare_suitae.
 * ================================================== */

/* FALSUM = creatio fracta */
interior b32
_mundum_parare (
                Piscina*  piscina,
    TesseraPonsMemoriae** pm,
          TesseraLector** lector)
{
    *pm = tessera_pons_memoriae_creare(piscina, LXXX, XXIV);
    si (*pm == NIHIL)
    {
        redde FALSUM;
    }
    *lector = tessera_lector_creare(piscina, &(*pm)->pons);
    redde *lector != NIHIL;
}

/* creatio mundi semel asserta (ut olim ante blocos) */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    TesseraPonsMemoriae* pm;
          TesseraLector* lector;

    pm      = tessera_pons_memoriae_creare(c->piscina, LXXX, XXIV);
    lector  = tessera_lector_creare(c->piscina, &pm->pons);
    CREDO_NON_NIHIL (pm);
    CREDO_NON_NIHIL (lector);
}


/* ==================================================
 * PROBARE: C milia octetorum - capacitas, truncatio, clavis post
 * ================================================== */

interior vacuum
_milia_probare (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
    TesseraPonsMemoriae* pm;
          TesseraLector* lector;
          TesseraEventum ev;
                     i8* initum;
                     i32 mensura;

    si (!_mundum_parare(piscina, &pm, &lector))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
             memoriae_index usus_ante;
             memoriae_index usus_post;
        TesseraEventumGenus genus;

        initum = _initum_parare(piscina, C * M, (i8)'x', VERUM, "z",
            &mensura);
        CREDO_VERUM (tessera_pons_memoriae_initum(pm, initum, mensura));

        /* status stabilis: collector praeparatus, nihil allocatum
         * (mensura circa vocationem SOLAM - credo fracturas in eadem
         * piscina scribit) */
        usus_ante  = piscina_summa_usus(piscina);
        genus      = tessera_eventum_expectare(lector, &ev, X);
        usus_post  = piscina_summa_usus(piscina);
        CREDO_AEQUALIS_I32 ((i32)usus_post, (i32)usus_ante);

        CREDO_AEQUALIS_I32 ((i32)genus, (i32)TESSERA_EVENTUM_GLUTINUM);
        CREDO_AEQUALIS_I32 (ev.glutinum.mensura,
            (i32)TESSERA_GLUTINUM_CAPACITAS);
        CREDO_VERUM (ev.glutinum_truncatum);
        CREDO_VERUM (_corpus_uniforme(ev.glutinum, (i8)'x'));

        CREDO_AEQUALIS_I32 ((i32)tessera_eventum_expectare(lector, &ev,
            X),
            (i32)TESSERA_EVENTUM_CLAVIS);
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'z');
    }
}


/* ==================================================
 * PROBARE: limes exactus et limes + I
 * ================================================== */

interior vacuum
_limitem_probare (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
    TesseraPonsMemoriae* pm;
          TesseraLector* lector;
          TesseraEventum ev;
                     i8* initum;
                     i32 mensura;

    si (!_mundum_parare(piscina, &pm, &lector))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        initum = _initum_parare(piscina,
            (i32)TESSERA_GLUTINUM_CAPACITAS,
            (i8)'e', VERUM, "", &mensura);
        CREDO_VERUM (tessera_pons_memoriae_initum(pm, initum, mensura));
        CREDO_AEQUALIS_I32 ((i32)tessera_eventum_expectare(lector, &ev,
            X),
            (i32)TESSERA_EVENTUM_GLUTINUM);
        CREDO_AEQUALIS_I32 (ev.glutinum.mensura,
            (i32)TESSERA_GLUTINUM_CAPACITAS);
        CREDO_FALSUM (ev.glutinum_truncatum);

        initum = _initum_parare(piscina,
            (i32)TESSERA_GLUTINUM_CAPACITAS + I, (i8)'f', VERUM, "",
            &mensura);
        CREDO_VERUM (tessera_pons_memoriae_initum(pm, initum, mensura));
        CREDO_AEQUALIS_I32 ((i32)tessera_eventum_expectare(lector, &ev,
            X),
            (i32)TESSERA_EVENTUM_GLUTINUM);
        CREDO_AEQUALIS_I32 (ev.glutinum.mensura,
            (i32)TESSERA_GLUTINUM_CAPACITAS);
        CREDO_VERUM (ev.glutinum_truncatum);
        CREDO_VERUM (_corpus_uniforme(ev.glutinum, (i8)'f'));
    }
}


/* ==================================================
 * PROBARE: M particulae VI octetorum; mora glutini
 * ================================================== */

interior vacuum
_particulas_probare (
    CredoContextus* c)
{
           Piscina* piscina = c->piscina;
    TesseraEventum  ev;
                i8* initum;
               i32  mensura;

    {
        PonsParticularum pp;
         TesseraLector* lector_particularum;
                    i32 k;

        memset(&pp, ZEPHYRUM, magnitudo(pp));
        mensura = MARCA_LONGITUDO + VI * M + MARCA_LONGITUDO;
        initum = (i8*)piscina_allocare(piscina,
            (memoriae_index)mensura);
        memcpy(initum, INITIUM_GLUTINI, MARCA_LONGITUDO);
        per (k = ZEPHYRUM; k < M; k++)
        {
            memcpy(initum + MARCA_LONGITUDO + k * VI, "chunk ", VI);
        }
        memcpy(initum + MARCA_LONGITUDO + VI * M, TERMINUS_GLUTINI,
            MARCA_LONGITUDO);

        pp.pons.datum      = &pp;
        pp.pons.legere     = _particulas_legere;
        pp.pons.scribere   = _particulas_scribere;
        pp.pons.amplitudo  = _particulas_amplitudo;
        pp.pons.intrare    = _particulas_status;
        pp.pons.egredi     = _particulas_status;
        pp.pons.resumptum  = NIHIL;
        pp.octeti          = initum;
        pp.mensura         = mensura;
        pp.particula       = VI;

        lector_particularum = tessera_lector_creare(piscina, &pp.pons);
        CREDO_NON_NIHIL (lector_particularum);
        CREDO_AEQUALIS_I32 ((i32)tessera_eventum_expectare(
            lector_particularum, &ev, X),
            (i32)TESSERA_EVENTUM_GLUTINUM);
        CREDO_AEQUALIS_I32 (ev.glutinum.mensura, VI * M);
        CREDO_FALSUM (ev.glutinum_truncatum);
        CREDO_VERUM (ev.glutinum.mensura == VI * M
            && memcmp(ev.glutinum.datum, initum + MARCA_LONGITUDO,
                   VI * M) == ZEPHYRUM);

        /* lectio prima (initium) mora vocantis; ceterae M + I (corpus
         * + terminus) mora glutini */
        CREDO_AEQUALIS_I32 (pp.lectiones_aliae, I);
        CREDO_AEQUALIS_I32 (pp.lectiones_glutini, MI);
    }
}


/* ==================================================
 * PROBARE: MB unum - tempus
 * ================================================== */

interior vacuum
_tempus_probare (
    CredoContextus* c)
{
                Piscina* piscina = c->piscina;
    TesseraPonsMemoriae* pm;
          TesseraLector* lector;
          TesseraEventum ev;
                     i8* initum;
                     i32 mensura;

    si (!_mundum_parare(piscina, &pm, &lector))
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        clock_t initium_temporis;
        clock_t finis_temporis;

        initum = _initum_parare(piscina, MXXIV * MXXIV,
            (i8)'y', VERUM, "q", &mensura);
        CREDO_VERUM (tessera_pons_memoriae_initum(pm, initum, mensura));
        initium_temporis = clock();
        CREDO_AEQUALIS_I32 ((i32)tessera_eventum_expectare(lector, &ev,
            X),
            (i32)TESSERA_EVENTUM_GLUTINUM);
        finis_temporis = clock();
        CREDO_VERUM (ev.glutinum_truncatum);
        CREDO_AEQUALIS_I32 (ev.glutinum.mensura,
            (i32)TESSERA_GLUTINUM_CAPACITAS);
        CREDO_AEQUALIS_I32 ((i32)tessera_eventum_expectare(lector, &ev,
            X),
            (i32)TESSERA_EVENTUM_CLAVIS);
        CREDO_AEQUALIS_S32 (ev.runa, (s32)'q');
        imprimere("  MB glutini (pons memoriae, lectiones LXIV): %.2f ms\n",
            (duplex)(finis_temporis - initium_temporis) * 1000.0
                / (duplex)CLOCKS_PER_SEC);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "glutinum C milium", _milia_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limitem capacitatis", _limitem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "M particulas", _particulas_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "MB unum (tempus)", _tempus_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tessera_glutinum", SECTIONES,
        _suitam_parare, NIHIL);
}
