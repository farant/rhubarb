/* scriba.c - scriba: pagina vim (folium LXVIII x LVI) in FENESTRA
 *
 * Compositio communis (scriba_applicatio) + glutinum fenestrae;
 * gemellus scriba_terminalis.c (scriba-plan S3). -fumus: volumen
 * temporarium, XXX quadra, exitus; -volumen <via> (ordinarie
 * scriba.volumen).
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "fenestra.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "scriba_applicatio.h"
#include <stdio.h>
#include <string.h>

/* LXXX x LX cellulae: folium cum margine et linea status */
#define SCRIBA_LATITUDO   CDLXXX
#define SCRIBA_ALTITUDO   CDLXXX
#define QUADRA_FUMI       XXX

s32
principale (
      integer   argc,
    character** argv)
{
                                    Piscina* piscina;
                        InternamentumChorda* intern;
                                    Volumen* vol;
                           ScribaApplicatio  app;
                                   Fenestra* fenestra;
                            TabulaPixelorum* tabula;
                              LudusFenestra* lf;
                       FenestraConfiguratio  cfg;
                                        b32  fumus;
                                        s32  exitus;
                         constans character* via_imaginis;
                                        s32  k;

    piscina = piscina_generare_dynamicum("scriba", IV * M * M);
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

    vol = scriba_volumen_aperire(piscina, (s32)argc, argv, &fumus);
    si (!vol)
    {
        fprintf(stderr, "scriba: volumen aperiri non potuit\n");
        redde I;
    }
    si (!scriba_applicatio_aedificare(&app, piscina, intern, vol, NIHIL,
            SCRIBA_LATITUDO, SCRIBA_ALTITUDO))
    {
        redde I;
    }

    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.titulus   = "scriba";
    cfg.x         = C;
    cfg.y         = C;
    cfg.latitudo  = SCRIBA_LATITUDO;
    cfg.altitudo  = SCRIBA_ALTITUDO;
    cfg.vexilla   = FENESTRA_ORDINARIA;
    fenestra      = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "scriba: fenestra\n");
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               SCRIBA_ALTITUDO);
    lf = ludus_fenestra_creare(piscina, app.d, app.figurae, ZEPHYRUM,
                               NIHIL, NIHIL, tabula);
    si (!tabula || !lf)
    {
        redde I;
    }
    exitus = ludus_fenestra_currere(lf, fenestra,
                                    fumus ? QUADRA_FUMI : ZEPHYRUM);
    si (   via_imaginis
        && !ludus_fenestra_imaginem_scribere(lf, via_imaginis))
    {
        fprintf(stderr, "scriba: imago non scripta: %s\n",
            via_imaginis);
        exitus = I;
    }
    fenestra_destruere(fenestra);
    volumen_claudere(vol);
    piscina_destruere(piscina);
    redde exitus;
}
