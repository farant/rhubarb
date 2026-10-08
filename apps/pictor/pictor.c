/* pictor.c - pictor: editor rasterum (P3: tabula + penicillus) in
 * FENESTRA
 *
 * Compositio communis (pictor_applicatio: volumen, documentum,
 * canones, insulae, registra, dispensator) + glutinum fenestrae;
 * gemellus pictor_terminalis.c (A4 moduli 013). -fumus: volumen
 * temporarium, XXX quadra, exitus.
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "fenestra.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "pictor_applicatio.h"
#include <stdio.h>
#include <string.h>

#define PICTOR_LATITUDO   (DC + XL)
#define PICTOR_ALTITUDO   CDLXXX
#define QUADRA_FUMI       XXX
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA             II

s32
principale (
      integer   argc,
    character** argv)
{
                                    Piscina* piscina;
                        InternamentumChorda* intern;
                                    Volumen* vol;
                           PictorApplicatio  app;
                                   Fenestra* fenestra;
                            TabulaPixelorum* tabula;
                              LudusFenestra* lf;
                       FenestraConfiguratio  cfg;
                                        b32  fumus;
                                        s32  exitus;
                         constans character* via_imaginis;
                                        s32  k;

    piscina = piscina_generare_dynamicum("pictor", IV * M * M);
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

    vol = pictor_volumen_aperire(piscina, (s32)argc, argv, &fumus);
    si (!vol)
    {
        fprintf(stderr, "pictor: volumen aperiri non potuit\n");
        redde I;
    }
    si (!pictor_applicatio_aedificare(&app, piscina, intern, vol, NIHIL,
            PICTOR_LATITUDO, PICTOR_ALTITUDO))
    {
        redde I;
    }

    /* fenestra */
    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.titulus   = "pictor";
    cfg.x         = C;
    cfg.y         = C;
    cfg.latitudo  = PICTOR_LATITUDO * SCALA;
    cfg.altitudo  = PICTOR_ALTITUDO * SCALA;
    cfg.vexilla   = FENESTRA_ORDINARIA;
    fenestra      = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "pictor: fenestra\n");
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               PICTOR_ALTITUDO);
    lf = ludus_fenestra_creare(piscina, app.d, app.figurae, ZEPHYRUM,
                               pictor_imago_fons,
                               &app.montatio.figurae_ctx,
                               tabula);
    si (!tabula || !lf)
    {
        redde I;
    }
    exitus = ludus_fenestra_currere(lf, fenestra,
                                    fumus ? QUADRA_FUMI : ZEPHYRUM);
    si (   via_imaginis
        && !ludus_fenestra_imaginem_scribere(lf, via_imaginis))
    {
        fprintf(stderr, "pictor: imago non scripta: %s\n",
            via_imaginis);
        exitus = I;
    }
    fenestra_destruere(fenestra);
    volumen_claudere(vol);
    piscina_destruere(piscina);
    redde exitus;
}
