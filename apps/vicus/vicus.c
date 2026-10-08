/* vicus.c - vicus: hospes applicationum (scriba, pictor, terminale) in
 * FENESTRA
 *
 * Compositio communis (vicus_applicatio) + glutinum fenestrae;
 * fenestra ad SCALA II (pixelum nostrum = II puncta, ut terminale);
 * tabulae vivae pulsantur (vicus-latera S1c);
 * gemellus vicus_terminalis.c (insula-rami-plan T4). Ctrl-A, deinde
 * n / p / 1-9 aut Ctrl-A (tabula prior); ictus in tabulam. -fumus:
 * volumen temporarium, XXX quadra, exitus; -volumen <via>
 * (ordinarie vicus.volumen).
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "fenestra.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "vicus_applicatio.h"
#include <stdio.h>
#include <string.h>

/* applicationes CDLXXX x CDLXXX ut solae, linea tabularum supra */
#define VICUS_LATITUDO   CDLXXX
#define VICUS_ALTITUDO   (CDLXXX + VICUS_ALTITUDO_TABULARUM)
#define QUADRA_FUMI      XXX
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA            II

/* tabulae vivae (terminale) in omni quadro pulsantur, etiam in fundo
 * (vicus-latera S1c) */
interior b32
vicum_pulsare (
    vacuum* ctx)
{
    redde vicus_pulsare((Vicus*)ctx);
}

s32
principale (
      integer   argc,
    character** argv)
{
                                    Piscina* piscina;
                        InternamentumChorda* intern;
                                    Volumen* vol;
                            VicusApplicatio  app;
                                   Fenestra* fenestra;
                            TabulaPixelorum* tabula;
                              LudusFenestra* lf;
                       FenestraConfiguratio  cfg;
                                        b32  fumus;
                                        s32  exitus;
                         constans character* via_imaginis;
                                        s32  k;

    piscina = piscina_generare_dynamicum("vicus", VIII * M * M);
    si (!piscina)
    {
        redde I;
    }
    /* -imago <via>: quadrum ultimum in PNG (screenshot) */
    via_imaginis = NIHIL;
    per (k = I; k + I < (s32)argc; k++)
    {
        si (strcmp(argv[k], "-imago") == ZEPHYRUM)
        {
            via_imaginis = argv[k + I];
        }
    }
    intern = internamentum_creare(piscina);
    thema_initiare();

    vol = vicus_volumen_aperire(piscina, (s32)argc, argv, &fumus);
    si (!vol)
    {
        fprintf(stderr, "vicus: volumen aperiri non potuit\n");
        redde I;
    }
    si (!vicus_applicatio_aedificare(&app, piscina, intern, vol, NIHIL,
            VICUS_LATITUDO, VICUS_ALTITUDO))
    {
        redde I;
    }

    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.titulus   = "vicus";
    cfg.x         = C;
    cfg.y         = C;
    cfg.latitudo  = VICUS_LATITUDO * SCALA;
    cfg.altitudo  = VICUS_ALTITUDO * SCALA;
    cfg.vexilla   = FENESTRA_ORDINARIA;
    fenestra      = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "vicus: fenestra\n");
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               VICUS_ALTITUDO);
    lf = ludus_fenestra_creare(piscina, app.d, vicus_figurae(app.vicus),
                               ZEPHYRUM, vicus_imago_fons, app.vicus,
                               tabula);
    si (!tabula || !lf)
    {
        redde I;
    }
    ludus_fenestra_pulsum_ponere(lf, vicum_pulsare, app.vicus);
    exitus = ludus_fenestra_currere(lf, fenestra,
                                    fumus ? QUADRA_FUMI : ZEPHYRUM);
    si (   via_imaginis
        && !ludus_fenestra_imaginem_scribere(lf, via_imaginis))
    {
        fprintf(stderr, "vicus: imago non scripta: %s\n", via_imaginis);
        exitus = I;
    }
    fenestra_destruere(fenestra);
    volumen_claudere(vol);
    piscina_destruere(piscina);
    redde exitus;
}
