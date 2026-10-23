/* testa.c - testa fenestrae (macOS; quaevis fenestra perscrutabilis):
 * principale possidet, applicatio_initium vocat, ansam currit
 * (testa-spec II.3). Ansa = ludus_fenestra_currere olim (sedes
 * mutata, verbatim; mensurae in testa_cursum_finire). */

#include "testa.h"
#include "fenestra.h"
#include "clipboard_platform.h"

#include <stdio.h>
#include <string.h>

#define QUADRA_FUMI  XXX
/* S1c: exspectatio maxima cum applicatione viva (ut terminale) */
#define MORA_PULSUS  XVI

/* L5: porta imaginis - clipboard systematis (Cmd+V in pictore) */
interior ImagoFructus
_imaginem_glutinatam (
     vacuum* ctx,
    Piscina* piscina)
{
    (vacuum)ctx;
    redde clipboard_capere_imaginem(piscina);
}

/* ansa vera: exspectare (ludus_fenestra_mora), tractare, quadrum,
 * praesentare; finis in EVENTUS_CLAUDERE aut post quadra_maxima */
interior vacuum
_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima)
{
    Eventus e;
        s64 nunc;
        b32 claudendum;

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
}

s32
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
            TestaAmbitus  a;
           TestaOptiones  o;
             TestaCursus* cursus;
     constans TestaForma* f;
    FenestraConfiguratio  cfg;
                Fenestra* fenestra;
         TabulaPixelorum* tabula;
      constans character* causa;
                     i32  latitudo_fenestrae;
                     i32  altitudo_fenestrae;
                     s32  exitus;

    piscina = piscina_generare_dynamicum(
        testa_titulus_processus(argc
            > ZEPHYRUM ? argv[ZEPHYRUM] : NIHIL),
        VIII * M * M);
    si (!piscina)
    {
        redde I;
    }
    testa_optiones_legere((s32)argc, argv, &o);
    memset(&a, ZEPHYRUM, magnitudo(TestaAmbitus));
    a.piscina          = piscina;
    a.argc             = (s32)argc;
    a.argv             = argv;
    a.fumus            = o.fumus;
    a.sedes_datorum    = ".";
    a.imago_glutinata  = _imaginem_glutinatam;
    /* R5: schirmus solum extra fumum (ut vicus olim): fumus magnitudine
     * fixa, imagines certae */
    si (   !o.fumus
        && !fenestra_spatium_schirmi(&a.schirmi_latitudo,
               &a.schirmi_altitudo))
    {
        a.schirmi_latitudo = ZEPHYRUM;
        a.schirmi_altitudo = ZEPHYRUM;
    }
    si (   !o.fumus
        && !fenestra_spatium_utile(&a.utilis_x, &a.utilis_y,
               &a.utilis_latitudo, &a.utilis_altitudo))
    {
        a.utilis_latitudo = ZEPHYRUM;
        a.utilis_altitudo = ZEPHYRUM;
    }

    cursus = (TestaCursus*)piscina_allocare(piscina,
        magnitudo(TestaCursus));
    si (!cursus)
    {
        redde I;
    }
    si (!testa_cursum_incipere(cursus, &a, applicatio_initium, &causa))
    {
        fprintf(stderr, "%s: %s\n",
            testa_titulus_processus(argc > ZEPHYRUM ? argv[ZEPHYRUM]
                                                    : NIHIL), causa);
        testa_cursum_dimittere(cursus);
        redde I;
    }
    f = &cursus->forma;

    /* S2c (vicus): plena visio -> fenestra in area utili, altitudine
     * pixelis nostris integra (scala tabulae exacte f->scala: 1147/573
     * = 2.0017 schirmum 599 non 600 daret), plena visio POST tabulam
     * (scala in creatione tabulae ex altitudine CURRENTE figitur) */
    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.titulus         = f->titulus;
    cfg.x               = C;
    cfg.y               = C;
    cfg.vexilla         = FENESTRA_ORDINARIA;
    latitudo_fenestrae  = f->latitudo;
    altitudo_fenestrae  = f->altitudo;
    si (   f->plena_visio && a.utilis_latitudo > ZEPHYRUM
        && a.utilis_altitudo > ZEPHYRUM)
    {
        cfg.x               = a.utilis_x;
        cfg.y               = a.utilis_y;
        latitudo_fenestrae  = a.utilis_latitudo / f->scala;
        altitudo_fenestrae  = a.utilis_altitudo / f->scala;
    }
    cfg.latitudo  = latitudo_fenestrae * f->scala;
    cfg.altitudo  = altitudo_fenestrae * f->scala;
    fenestra      = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "%s: fenestra\n", f->titulus);
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               altitudo_fenestrae);
    si (tabula && f->plena_visio)
    {
        fenestra_commutare_plenam_visionem(fenestra);
    }
    si (!tabula || !testa_cursum_ludum_struere(cursus, piscina, tabula))
    {
        redde I;
    }

    _currere(cursus->lf, fenestra, o.fumus ? QUADRA_FUMI : ZEPHYRUM);
    testa_cursum_finire(cursus);
    exitus = ZEPHYRUM;
    si (   o.via_imaginis
        && !ludus_fenestra_imaginem_scribere(cursus->lf,
        o.via_imaginis))
    {
        fprintf(stderr, "%s: imago non scripta: %s\n", f->titulus,
            o.via_imaginis);
        exitus = I;
    }
    fenestra_destruere(fenestra);
    testa_cursum_dimittere(cursus);
    piscina_destruere(piscina);
    redde exitus;
}
