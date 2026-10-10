/* vicus.c - vicus: hospes applicationum (scriba, pictor, terminale) in
 * FENESTRA
 *
 * Compositio communis (vicus_applicatio) + glutinum fenestrae;
 * fenestra ad SCALA II (pixelum nostrum = II puncta, ut terminale)
 * in plena visione (vicus-latera S2c; -fumus: magnitudo fixa);
 * tabulae vivae pulsantur (vicus-latera S1c);
 * gemellus vicus_terminalis.c (insula-rami-plan T4). Cmd+1..9, Cmd+0
 * = tabula (Ctrl-A ad latus focatum - tmux); ictus in tabulam. -fumus:
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
                                        i32  latitudo;
                                        i32  altitudo;
                                        i32  spatium_lat;
                                        i32  spatium_alt;
                                        i32  latitudo_fenestrae;
                                        i32  altitudo_fenestrae;
                                        b32  plena;

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
    /* S2c: vicus in PLENA VISIONE aperitur (Franus 2026-10-08: sic
     * laborat) - compositio et documenta nova magnitudine schirmi
     * totius; fenestra ante transitum spatium utile tenet, altitudine
     * pixelis nostris integra, ut scala tabulae II exacte sit (scala in
     * creatione figitur: 1147/573 = 2.0017 schirmum 599 non 600
     * daret). fumus: magnitudo fixa, sine plena visione (imagines
     * certae). */
    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.x               = C;
    cfg.y               = C;
    cfg.vexilla         = FENESTRA_ORDINARIA;
    latitudo            = VICUS_LATITUDO;
    altitudo            = VICUS_ALTITUDO;
    altitudo_fenestrae  = VICUS_ALTITUDO;
    latitudo_fenestrae  = VICUS_LATITUDO;
    si (   !fumus
        && fenestra_spatium_schirmi(&spatium_lat, &spatium_alt))
    {
        latitudo            = spatium_lat / SCALA;
        altitudo            = spatium_alt / SCALA;
        latitudo_fenestrae  = latitudo;
        altitudo_fenestrae  = altitudo;
        si (fenestra_spatium_utile(&cfg.x, &cfg.y, &spatium_lat,
                &spatium_alt))
        {
            latitudo_fenestrae = spatium_lat / SCALA;
            altitudo_fenestrae = spatium_alt / SCALA;
        }
        plena = VERUM;
    }
    si (!vicus_applicatio_aedificare(&app, piscina, intern, vol, NIHIL,
            latitudo, altitudo))
    {
        redde I;
    }
    /* S4: horologium locale in linea tabularum */
    vicus_horologium_ponere(app.vicus, vicus_horologium_locale, NIHIL);

    cfg.titulus   = "vicus";
    cfg.latitudo  = latitudo_fenestrae * SCALA;
    cfg.altitudo  = altitudo_fenestrae * SCALA;
    fenestra      = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "vicus: fenestra\n");
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               altitudo_fenestrae);
    /* plena visio POST tabulam: scala in creatione tabulae ex
     * altitudine fenestrae CURRENTE figitur - vexillum PLENA_VISIO
     * transitum in fenestra_creare incipit, et fenestra iam crescens
     * scalam > II dabat (spatium logicum minus documentis: columna et
     * linea nimiae, Franus) */
    si (plena)
    {
        fenestra_commutare_plenam_visionem(fenestra);
    }
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
