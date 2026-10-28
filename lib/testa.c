/* testa.c - testa fenestrae (macOS; quaevis fenestra perscrutabilis):
 * testa_principale (principale applicationis eum vocat) possidet,
 * applicatio_initium vocat, ansam currit
 * (testa-spec II.3). Ansa = ludus_fenestra_currere (API publica;
 * programmata briar eam quoque vocant). */

#include "testa.h"
#include "fenestra.h"
#include "clipboard_platform.h"
#include "veredarius_domesticus.h"

#include <stdio.h>
#include <string.h>

#define QUADRA_FUMI  XXX
/* L5: porta imaginis - clipboard systematis (Cmd+V in pictore) */
interior ImagoFructus
_imaginem_glutinatam (
     vacuum* ctx,
    Piscina* piscina)
{
    (vacuum)ctx;
    redde clipboard_capere_imaginem(piscina);
}

/* porta textus - registrum scribae in clipboard systematis */
interior b32
_textum_exportare (
    vacuum* ctx,
    chorda  textus)
{
    (vacuum)ctx;
    redde clipboard_textum_ponere((constans character*)textus.datum,
        textus.mensura);
}

s32
testa_principale (
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
    a.piscina           = piscina;
    a.argc              = (s32)argc;
    a.argv              = argv;
    a.fumus             = o.fumus;
    a.sedes_datorum     = ".";
    a.imago_glutinata   = _imaginem_glutinatam;
    a.textum_exportare  = _textum_exportare;
    /* plan-5 T5: reactor testae + veredarius domesticus (HTTP domus) */
    a.machina = "Mac";
    a.reactor = reactor_creare(piscina);
    a.veredarius        =
        a.reactor ? veredarium_domesticum_creare(piscina,
        a.reactor) : NIHIL;
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
    /* plan-5 T5: reactor quem applicatio reddit (sine quiete) in ansa
     * agitur; quies (vicus) suum iam habet */
    si (f->reactor && !f->mora_proxima)
    {
        ludus_fenestra_reactorem_ponere(cursus->lf, f->reactor);
    }

    /* ansa publica (dispensator_finire + linea mensurarum intus) -
     * testa_cursum_finire solum testae Android (sine ansa propria) */
    (vacuum)ludus_fenestra_currere(cursus->lf, fenestra,
        o.fumus ? QUADRA_FUMI : ZEPHYRUM);
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
    si (a.reactor)
    {
        reactor_destruere(a.reactor);
    }
    piscina_destruere(piscina);
    redde exitus;
}
