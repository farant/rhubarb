/* ludus_fenestra.c - glutinum fenestrae */

#include "ludus_fenestra.h"
#include "imago_png.h"
#include "thema.h"
#include "color.h"

#include <stdio.h>
#include <string.h>

/* S1c: exspectatio maxima cum applicatione viva (ut terminale) */
#define MORA_PULSUS  XVI

/* summa versionum repositorii (durabilis + ephemera) */
interior i32
versionem_summare (
    constans LudusFenestra* lf)
{
    redde insula_versio(lf->d->repo, INSULA_DURABILIS)
        + insula_versio(lf->d->repo, INSULA_EPHEMERA);
}

LudusFenestra*
ludus_fenestra_creare (
            Piscina* piscina,
        Dispensator* d,
    FiguraRegistrum* figurae,
                i32  thema,
          ImagoFons  fons,
             vacuum* fons_ctx,
    TabulaPixelorum* tabula)
{
    LudusFenestra* lf;

    si (!piscina || !d || !figurae || !tabula)
    {
        redde NIHIL;
    }
    lf = (LudusFenestra*)piscina_allocare(piscina,
        magnitudo(LudusFenestra));
    si (!lf)
    {
        redde NIHIL;
    }
    memset(lf, ZEPHYRUM, magnitudo(LudusFenestra));
    lf->d         = d;
    lf->figurae   = figurae;
    lf->thema     = thema;
    lf->fons      = fons;
    lf->fons_ctx  = fons_ctx;
    lf->tabula    = tabula;
    lf->piscina   = piscina;
    lf->piscina_quadri = piscina_generare_dynamicum("ludus_quadrum",
                                                    LXIV * M);
    si (!lf->piscina_quadri)
    {
        redde NIHIL;
    }
    redde lf;
}

/* Tempus stampare et tradere (sine conversione) */
interior vacuum
_tradere (
    LudusFenestra* lf,
          Eventus* e,
              s64  nunc)
{
    si (e->tempus == ZEPHYRUM)
    {
        e->tempus = nunc;
    }
    dispensator_tractare(lf->d, e);
}

/* Magnitudo initialis semel (013 B1), IAM in pixelis nostris
 * (tabulae) - directe traditur, sine conversione B3b. */
interior vacuum
_magnitudinem_nuntiare (
    LudusFenestra* lf,
              s64  nunc)
{
    Eventus e;

    si (lf->magnitudo_nuntiata)
    {
        redde;
    }
    lf->magnitudo_nuntiata = VERUM;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                               = EVENTUS_MUTARE_MAGNITUDINEM;
    e.tempus                              = nunc;
    e.datum.mutare_magnitudinem.latitudo  = lf->tabula->latitudo;
    e.datum.mutare_magnitudinem.altitudo  = lf->tabula->altitudo;
    _tradere(lf, &e, nunc);
}

vacuum
ludus_fenestra_tractare (
       LudusFenestra* lf,
    constans Eventus* ev,
                 s64  nunc)
{
    Eventus e;

    si (!lf || !ev)
    {
        redde;
    }
    _magnitudinem_nuntiare(lf, nunc);
    e = *ev;
    si (e.genus == EVENTUS_MUTARE_MAGNITUDINEM && lf->piscina)
    {
        /* 013 B3b: puncta fenestrae -> tabula aptata (scala servata)
         * -> eventus in pixela nostra: superficies idem significat in
         * fenestra ac in terminali */
        (vacuum)tabula_pixelorum_ad_fenestram(lf->tabula, lf->piscina,
            e.datum.mutare_magnitudinem.latitudo,
            e.datum.mutare_magnitudinem.altitudo);
        e.datum.mutare_magnitudinem.latitudo = lf->tabula->latitudo;
        e.datum.mutare_magnitudinem.altitudo = lf->tabula->altitudo;
    }
    _tradere(lf, &e, nunc);
}

vacuum
ludus_quadrum (
    LudusFenestra* lf,
              s64  nunc)
{
    s64 t0;
    s64 t1;
    s64 t2;
    s64 t3;

    si (!lf)
    {
        redde;
    }
    _magnitudinem_nuntiare(lf, nunc);
    t0 = fenestra_tempus_ms();
    dispensator_pulsare(lf->d, nunc);
    t1 = fenestra_tempus_ms();
    piscina_vacare(lf->piscina_quadri);
    lf->mandata = mandata_creare(lf->piscina_quadri, lf->d->intern);
    pingere(dispensator_arbor(lf->d), lf->figurae, lf->thema,
        lf->mandata);
    t2 = fenestra_tempus_ms();
    tabula_pixelorum_vacare(lf->tabula,
        color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
    delineare_mandata(lf->mandata, lf->tabula, lf->fons, lf->fons_ctx);
    t3 = fenestra_tempus_ms();
    lf->mensurae.quadra++;
    lf->versio_picta               = versionem_summare(lf);
    lf->mensurae.ms_compositionis  += t1 - t0;
    lf->mensurae.ms_pingendi       += t2 - t1;
    lf->mensurae.ms_delineandi     += t3 - t2;
    si (t3 - t0 > lf->mensurae.ms_quadri_maximum)
    {
        lf->mensurae.ms_quadri_maximum = t3 - t0;
    }
}

vacuum
ludus_fenestra_pulsum_ponere (
    LudusFenestra* lf,
    LudusPulsator  fn,
           vacuum* ctx)
{
    si (!lf)
    {
        redde;
    }
    lf->pulsator      = fn;
    lf->pulsator_ctx  = fn ? ctx : NIHIL;
}

b32
ludus_fenestra_pingendum (
    LudusFenestra* lf,
              b32  eventa,
              s64  nunc)
{
    b32 mutatum;

    si (!lf || !lf->pulsator)
    {
        redde VERUM;
    }
    mutatum = lf->pulsator(lf->pulsator_ctx);
    si (eventa || mutatum || lf->mensurae.quadra == ZEPHYRUM)
    {
        redde VERUM;
    }
    /* nihil novi: dispensator SOLUS pulsatur (horologia, e.g.
     * scriptura differta). Signum = VERSIO repositorii contra quadrum
     * ultimum (compositiones non: dispensator omni eventu recomponit,
     * 'regula staleness') */
    dispensator_pulsare(lf->d, nunc);
    redde versionem_summare(lf) != lf->versio_picta;
}

Mora
ludus_fenestra_mora (
    constans LudusFenestra* lf)
{
    si (!lf || lf->mensurae.quadra == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (Mora)lf->d->quies_ms;
}

s32
ludus_fenestra_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima)
{
    Eventus e;
        s64 nunc;
        b32 claudendum;

    si (!lf || !fenestra)
    {
        redde I;
    }
    claudendum = FALSUM;
    dum (!claudendum && !fenestra_debet_claudere(fenestra))
    {
        Mora mora;
         b32 eventa;

        /* S1c: applicatio viva (pulsus) - XVI ms ad summum */
        mora = ludus_fenestra_mora(lf);
        si (lf->pulsator && (mora == ZEPHYRUM || mora > MORA_PULSUS))
        {
            mora = MORA_PULSUS;
        }
        fenestra_expectare_eventus(fenestra, mora);
        nunc    = fenestra_tempus_ms();
        eventa  = FALSUM;
        dum (fenestra_obtinere_eventus(fenestra, &e))
        {
            si (e.genus == EVENTUS_CLAUDERE)
            {
                claudendum = VERUM;
                frange;
            }
            ludus_fenestra_tractare(lf, &e, nunc);
            eventa = VERUM;
        }
        /* fumus (quadra_maxima): omne quadrum pingitur - aliter
         * applicatio viva otiosa post quadrum primum numquam exit */
        si (   quadra_maxima == ZEPHYRUM
            && !ludus_fenestra_pingendum(lf, eventa, nunc))
        {
            perge;
        }
        ludus_quadrum(lf, nunc);
        fenestra_praesentare_pixela(fenestra, lf->tabula);
        si (   quadra_maxima > ZEPHYRUM
            && lf->mensurae.quadra >= quadra_maxima)
        {
            claudendum = VERUM;
        }
    }
    /* S1a: pendentia (gestus, pan/zoom) ante exitum effunduntur */
    dispensator_finire(lf->d);
    imprimere("ludus: quadra=%d compositio=%ldms pingere=%ldms"
              " delineare=%ldms maximum=%ldms\n",
              (int)lf->mensurae.quadra,
              (long)lf->mensurae.ms_compositionis,
              (long)lf->mensurae.ms_pingendi,
              (long)lf->mensurae.ms_delineandi,
              (long)lf->mensurae.ms_quadri_maximum);
    redde ZEPHYRUM;
}

b32
ludus_fenestra_imaginem_scribere (
    constans LudusFenestra* lf,
        constans character* via)
{
         Imago imago;
    PngFructus fructus;

    si (!lf || !via || lf->mensurae.quadra == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (!tabula_pixelorum_in_imaginem(lf->tabula, lf->piscina_quadri,
            &imago))
    {
        redde FALSUM;
    }
    fructus = imago_png_scribere(&imago, via, lf->piscina_quadri);
    redde fructus.successus;
}
