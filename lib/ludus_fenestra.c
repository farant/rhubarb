/* ludus_fenestra.c - glutinum fenestrae */

#include "ludus_fenestra.h"
#include "thema.h"
#include "color.h"

#include <stdio.h>
#include <string.h>

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
    lf->piscina_quadri = piscina_generare_dynamicum("ludus_quadrum",
                                                    LXIV * M);
    si (!lf->piscina_quadri)
    {
        redde NIHIL;
    }
    redde lf;
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
    e = *ev;
    si (e.tempus == ZEPHYRUM)
    {
        e.tempus = nunc;
    }
    dispensator_tractare(lf->d, &e);
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
    lf->mensurae.ms_compositionis  += t1 - t0;
    lf->mensurae.ms_pingendi       += t2 - t1;
    lf->mensurae.ms_delineandi     += t3 - t2;
    si (t3 - t0 > lf->mensurae.ms_quadri_maximum)
    {
        lf->mensurae.ms_quadri_maximum = t3 - t0;
    }
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
        fenestra_perscrutari_eventus(fenestra);
        nunc = fenestra_tempus_ms();
        dum (fenestra_obtinere_eventus(fenestra, &e))
        {
            si (e.genus == EVENTUS_CLAUDERE)
            {
                claudendum = VERUM;
                frange;
            }
            ludus_fenestra_tractare(lf, &e, nunc);
        }
        ludus_quadrum(lf, nunc);
        fenestra_praesentare_pixela(fenestra, lf->tabula);
        si (   quadra_maxima > ZEPHYRUM
            && lf->mensurae.quadra >= quadra_maxima)
        {
            claudendum = VERUM;
        }
    }
    imprimere("ludus: quadra=%d compositio=%ldms pingere=%ldms"
              " delineare=%ldms maximum=%ldms\n",
              (int)lf->mensurae.quadra,
              (long)lf->mensurae.ms_compositionis,
              (long)lf->mensurae.ms_pingendi,
              (long)lf->mensurae.ms_delineandi,
              (long)lf->mensurae.ms_quadri_maximum);
    redde ZEPHYRUM;
}
