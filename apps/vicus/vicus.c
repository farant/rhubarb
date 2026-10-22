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
 * (ordinarie vicus.volumen); -quaestio adiutor: $question per
 * bin/quaestio (via A) loco portae in processu (via B, http-vocatio).
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
#include "praeconium.h"
#include "energia.h"
#include "clipboard_platform.h"
#include "vicus_quaestio_porta.h"
#include "vicus_quaestio_interna.h"
#include "reactor.h"
#include "vicus_sonus_porta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* applicationes CDLXXX x CDLXXX ut solae, linea tabularum supra */
#define VICUS_LATITUDO   CDLXXX
#define VICUS_ALTITUDO   (CDLXXX + VICUS_ALTITUDO_TABULARUM)
#define QUADRA_FUMI      XXX
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA            II

/* tabulae vivae (terminale) in omni quadro pulsantur, etiam in fundo
 * (vicus-latera S1c). Reactor: hospes eum SOLUS possidet et in omni
 * pulsu poscit (http-vocatio R1); nihil inscriptum = nihil agit. */
nomen structura {
      Vicus* vicus;
    Reactor* reactor;
} PulsusVici;

interior b32
vicum_pulsare (
    vacuum* ctx)
{
    PulsusVici* pv = (PulsusVici*)ctx;

    (vacuum)reactor_poll(pv->reactor, ZEPHYRUM);
    redde vicus_pulsare(pv->vicus);
}

/* L5: porta imaginis - clipboard systematis (Cmd+V in pictore) */
interior ImagoFructus
imaginem_glutinatam (
     vacuum* ctx,
    Piscina* piscina)
{
    (vacuum)ctx;
    redde clipboard_capere_imaginem(piscina);
}

/* S5: porta pilae - energia_status systematis */
interior b32
pilam_systematis (
    vacuum* ctx,
       s32* centesimae,
       b32* oneratur)
{
    EnergiaStatus s;

    (vacuum)ctx;
    si (!energia_status(&s))
    {
        redde FALSUM;
    }
    *centesimae  = s.centesimae;
    *oneratur    = s.oneratur;
    redde VERUM;
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
                      VicusPortaQuaestionis  porta_quaestionis;
                             VicusPortaSoni  porta_soni;
                                 PulsusVici  pulsus;
                                    Reactor* reactor;
                                        b32  per_adiutorem;
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
    reactor = reactor_creare(piscina);
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
    si (!vicus_applicatio_aedificare(&app, piscina, intern, vol,
            latitudo, altitudo))
    {
        redde I;
    }
    /* S4: horologium locale in linea tabularum */
    vicus_horologium_ponere(app.vicus, vicus_horologium_locale, NIHIL);
    /* S5: pila (IOKit; machina mensae: nulla) */
    vicus_pilam_ponere(app.vicus, pilam_systematis, NIHIL);
    /* T4a: admonitiones ex libro praeconii (colloquia Claude Code) */
    (vacuum)vicus_applicatio_admonitiones_aperire(&app,
        praeconium_via(piscina));
    /* sonus S4: admonitiones sonantes (tintinnabulum super auditus,
     * exitus piger); -fumus tacet - probationes numquam sonant */
    si (   !fumus && vicus_sonus_porta(&porta_soni, piscina,
            (i64)time(NIHIL)))
    {
        vicus_applicatio_soni_portam_ponere(&app, &porta_soni);
    }
    /* L5: Cmd+V in pictore: imago e clipboard in stratum novum */
    vicus_applicatio_imaginis_portam_ponere(&app, imaginem_glutinatam,
        NIHIL);
    /* $question: in processu (via B, http-vocatio; clavis per
     * petitionem in piscinam vocationis) nisi -quaestio adiutor (via A:
     * bin/quaestio per processum, R11) */
    per_adiutorem = FALSUM;
    per (k = I; k + I < (s32)argc; k++)
    {
        si (   strcmp(argv[k], "-quaestio")   == ZEPHYRUM
            && strcmp(argv[k + I], "adiutor") == ZEPHYRUM)
        {
            per_adiutorem = VERUM;
        }
    }
    porta_quaestionis = per_adiutorem
        ? vicus_quaestio_porta(vicus_quaestio_adiutoris_via(
              argc > ZEPHYRUM ? argv[ZEPHYRUM] : NIHIL, piscina),
              ZEPHYRUM, piscina)
        : vicus_quaestio_interna(reactor, NIHIL, piscina);
    vicus_applicatio_quaestionis_portam_ponere(&app,
        &porta_quaestionis);

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
    pulsus.vicus    = app.vicus;
    pulsus.reactor  = reactor;
    ludus_fenestra_pulsum_ponere(lf, vicum_pulsare, &pulsus);
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
