#ifndef FENESTRA_H
#define FENESTRA_H

/* <aedilis corpus="lib/fenestra_textus.c"/> */
/* <sutura/> interfacies fenestrarum portabilis - implementatio
 * per-platformam (fenestra.m Darwin; olim fenestra_linux.c) */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"   /* Mora: duratio millisecundorum */
#include "tabula_pixelorum.h"   /* typus, pixela, textus (puri) */
#include "eventus.h"            /* vocabularium initus (purum) */


/* ==================================================
 * FENESTRA - Creatio et Gestio Fenestrarum
 *
 * Bibliotheca pro creando fenestras nativas et
 * tractando eventus ab usore. Sustinet redditionem
 * per tabulam pixelorum (software rendering).
 *
 * PHILOSOPHIA:
 * - API simplex et clara
 * - Tabula pixelorum RGBA8888
 * - Eventus in cauda (ring buffer)
 * - Texti integrati (fons 6x8)
 *
 * EXEMPLUM:
 *   Piscina* piscina = piscina_generare_dynamicum("fenestra", M * M);
 *
 *   FenestraConfiguratio config = {
 *       .titulus = "Salve Munde",
 *       .x = C, .y = C,
 *       .latitudo = DCCC, .altitudo = DCCC,
 *       .vexilla = FENESTRA_ORDINARIA
 *   };
 *   Fenestra* fenestra = fenestra_creare(piscina, &config);
 *   TabulaPixelorum* tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
 *
 *   dum (!fenestra_debet_claudere(fenestra)) {
 *       fenestra_perscrutari_eventus(fenestra);
 *
 *       Eventus eventus;
 *       dum (fenestra_obtinere_eventus(fenestra, &eventus)) {
 *           // Tractare eventus
 *       }
 *
 *       tabula_pixelorum_vacare(tabula, RGB(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
 *       chorda textus = chorda_ex_literis("Salve!", piscina);
 *       tabula_pixelorum_pingere_chordam(tabula, X, X, textus, RGB(CCLV, CCLV, CCLV));
 *       fenestra_praesentare_pixela(fenestra, tabula);
 *   }
 *
 *   fenestra_destruere(fenestra);
 *   piscina_destruere(piscina);
 *
 * ================================================== */


/* ==================================================
 * Constantae - Vexilla Fenestrae
 * ================================================== */

/* Vexilla fenestrae */
nomen enumeratio {
    FENESTRA_MUTABILIS     = I << ZEPHYRUM,  /* Resizable */
    FENESTRA_CLAUDIBILIS   = I << I,          /* Closable */
    FENESTRA_MINUIBILIS    = I << II,         /* Minimizable */
    FENESTRA_MAXIMIZABILIS = I << III,        /* Maximizable */
    FENESTRA_CENTRATA      = I << IV,         /* Centered */
    FENESTRA_PLENA_VISIO   = I << V,          /* Fullscreen */
    /* RETRO: fenestra apparet sed applicatio focum systematis NON
     * rapit (activateIgnoringOtherApps omissum). Pro probationibus
     * fumi: XII generationes = XII raptus foci sine eo. Fenestra
     * intra applicationem CLAVIS manet (makeKeyAndOrderFront
     * app-localis est), ergo claves nativae per fistulam nostram
     * immissae adhuc perveniunt - MENSURATUM fumo laboratorii. */
    FENESTRA_RETRO         = I << VI,
    FENESTRA_ORDINARIA     = FENESTRA_MUTABILIS | FENESTRA_CLAUDIBILIS |
                             FENESTRA_MINUIBILIS
                                 | FENESTRA_MAXIMIZABILIS
} fenestra_vexilla_t;


/* ==================================================
 * Typi - Configuratio Fenestrae
 * ================================================== */

/* Configuratio fenestrae */
nomen structura {
    constans character* titulus;
                   i32  x;
                   i32  y;
                   i32  latitudo;
                   i32  altitudo;
                   i32  vexilla;
} FenestraConfiguratio;


/* ==================================================
 * Typi Opaci
 * ================================================== */

/* Fenestra opaca */
nomen structura Fenestra Fenestra;


/* ==================================================
 * Creatio / Destructio
 * ================================================== */

/* Creare fenestram novam
 *
 * piscina: piscina pro allocando memoriam
 * configuratio: configuratio fenestrae
 *
 * Reddit: fenestram novam vel NIHIL si error
 */
Fenestra*
fenestra_creare (
                          Piscina* piscina,
    constans FenestraConfiguratio* configuratio);

/* Destruere fenestram
 *
 * Liberat solum objecta systematis nativi (NSWindow, etc.).
 * Memoria allocata ex piscina liberabitur cum piscina_destruere().
 *
 * fenestra: fenestra destruenda
 */
vacuum
fenestra_destruere (
    Fenestra* fenestra);


/* ==================================================
 * Gestio Eventuum
 * ================================================== */

/* Verificare si fenestra debet claudere
 *
 * fenestra: fenestra
 *
 * Reddit: VERUM si debet claudere
 */
b32
fenestra_debet_claudere (
    constans Fenestra* fenestra);

/* Perscrutari eventus ex systemate operativo
 *
 * Haec functio debet vocari in cyclo principale
 * ante obtinere eventus.
 *
 * fenestra: fenestra
 */
vacuum
fenestra_perscrutari_eventus (
    Fenestra* fenestra);

/* Expectare eventus (pumpa obstructiva - alternativa perscrutari)
 *
 * Morari usque ad ms_maximae millisecundas donec eventus adveniat,
 * tum omnes praesentes exhaurire (delegat ad perscrutari). App
 * otiosa ~0% CPU pro apps sine ansa quadrorum (vitrea). Fontes
 * runloop inter moras serviuntur. Eventum syntheticum
 * ApplicationDefined (typus 15) pumpam expergefacit et a
 * translatore voratur - contractus excitationis vitreae.
 *
 * fenestra:   fenestra
 * ms_maximae: mora maxima in millisecundis (0 = statim, ut
 *             perscrutari; pabulum: tempestivum_proxima_meta_ms)
 */
vacuum
fenestra_expectare_eventus (
    Fenestra* fenestra,
        Mora  ms_maximae);

/* Obtinere eventum proximum ex cauda
 *
 * fenestra: fenestra
 * eventus: exitus - eventus proximus
 *
 * Reddit: VERUM si eventus inventus, FALSUM si cauda vacua
 */
b32
fenestra_obtinere_eventus (
    Fenestra* fenestra,
     Eventus* eventus);


/* ==================================================
 * Proprietates Fenestrae
 * ================================================== */

/* Ponere titulum fenestrae
 *
 * fenestra: fenestra
 * titulus: titulus novus
 */
vacuum
fenestra_ponere_titulum (
              Fenestra* fenestra,
    constans character* titulus);

/* Obtinere magnitudinem fenestrae
 *
 * fenestra: fenestra
 * latitudo: exitus - latitudo fenestrae
 * altitudo: exitus - altitudo fenestrae
 */
vacuum
fenestra_obtinere_magnitudinem (
    constans Fenestra* fenestra,
                  i32* latitudo,
                  i32* altitudo);

/* Ponere magnitudinem fenestrae
 *
 * fenestra: fenestra
 * latitudo: latitudo nova
 * altitudo: altitudo nova
 */
vacuum
fenestra_ponere_magnitudinem (
    Fenestra* fenestra,
         i32  latitudo,
         i32  altitudo);

/* Obtinere positum fenestrae
 *
 * fenestra: fenestra
 * x: exitus - coordinata x
 * y: exitus - coordinata y
 */
vacuum
fenestra_obtinere_positum (
    constans Fenestra* fenestra,
                  i32* x,
                  i32* y);

/* Ponere positum fenestrae
 *
 * fenestra: fenestra
 * x: coordinata x nova
 * y: coordinata y nova
 */
vacuum
fenestra_ponere_positum (
    Fenestra* fenestra,
         i32  x,
         i32  y);


/* ==================================================
 * Status Fenestrae
 * ================================================== */

/* Monstrare fenestram
 *
 * fenestra: fenestra
 */
vacuum
fenestra_monstrare (
    Fenestra* fenestra);

/* Celare fenestram
 *
 * fenestra: fenestra
 */
vacuum
fenestra_celare (
    Fenestra* fenestra);

/* Dare focus ad fenestram
 *
 * fenestra: fenestra
 */
vacuum
fenestra_focus (
    Fenestra* fenestra);

/* Verificare si fenestra visibilis est
 *
 * fenestra: fenestra
 *
 * Reddit: VERUM si visibilis
 */
b32
fenestra_est_visibilis (
    constans Fenestra* fenestra);

/* Verificare si fenestra habet focus
 *
 * fenestra: fenestra
 *
 * Reddit: VERUM si habet focus
 */
b32
fenestra_habet_focus (
    constans Fenestra* fenestra);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Centrare fenestram
 *
 * fenestra: fenestra
 */
vacuum
fenestra_centrare (
    Fenestra* fenestra);

/* Maximizare fenestram
 *
 * fenestra: fenestra
 */
vacuum
fenestra_maximizare (
    Fenestra* fenestra);

/* Minuere fenestram
 *
 * fenestra: fenestra
 */
vacuum
fenestra_minuere (
    Fenestra* fenestra);

/* Restituere fenestram
 *
 * fenestra: fenestra
 */
vacuum
fenestra_restituere (
    Fenestra* fenestra);

/* Commutare plenam visionem
 *
 * fenestra: fenestra
 */
vacuum
fenestra_commutare_plenam_visionem (
    Fenestra* fenestra);

/* Verificare si fenestra est in plena visione
 *
 * fenestra: fenestra
 *
 * Reddit: VERUM si plena visio
 */
b32
fenestra_est_plena_visio (
    constans Fenestra* fenestra);

/* Occultare cursorem muris systematis
 *
 * Pro modo plena visio - occultare cursor systematis
 * ut possimus reddere cursorem nostrum.
 *
 * fenestra: fenestra
 */
vacuum
fenestra_occultare_cursorem (
    Fenestra* fenestra);

/* Ostendere cursorem muris systematis
 *
 * Restituere cursorem systematis post modum plena visio.
 *
 * fenestra: fenestra
 */
vacuum
fenestra_ostendere_cursorem (
    Fenestra* fenestra);

/* Obtinere tractationem nativam
 *
 * Pro accessu ad platformam specificam (NSWindow in macOS)
 *
 * fenestra: fenestra
 *
 * Reddit: tractationem nativam (void*)
 */
vacuum*
fenestra_obtinere_tractationem_nativam (
    Fenestra* fenestra);

/* Obtinere NUMERUM fenestrae nativum (CGWindowID in macOS)
 *
 * Cur seorsum a tractatione: numerus per fines processuum transit,
 * monstrator non. Unde instrumentum EXTERIUS hanc solam fenestram
 * capere potest (screencapture -l<numerus>) loco scrinii totius -
 * quod et rectius est et privatius.
 *
 * Reddit: numerum, aut ZEPHYRUM si ignotus
 */
i32
fenestra_numerus_nativus (
    Fenestra* fenestra);


/* ==================================================
 * Claves IMMITTERE (agitatio, non usus)
 * ==================================================
 *
 * CUR NON IN JS: eventus per 'dispatchEvent(new KeyboardEvent(...))'
 * missus NON FIDUS est (isTrusted=false). Auditores applicationis eum
 * audiunt, sed textura ipsa eum IGNORAT: nullus character inseritur,
 * Tab focum non movet, forma non mittitur. Instrumentum ita structum
 * 'pressi' nuntiaret et NIHIL egisset - id genus mendacii quod haec
 * domus tollere studet.
 *
 * Eventus hic synthetizatus in caudam ipsius applicationis ponitur
 * ([NSApp postEvent:]), unde per viam ordinariam ad primum
 * respondentem venit - ergo WebKit eum ut VERUM tractat. Nulla
 * permissio accessibilitatis petitur: intra processum NOSTRUM manemus
 * (differt a CGEventPost, qui totum systema tangeret et permissionem
 * poasceret quam non habemus).
 *
 * FENESTRA CLAVIS ESSE DEBET: eventus ad fenestram clavem it. Si alia
 * applicatio focum tenet, hic eventus nusquam apparet. Vide
 * 'fenestra_clavem_capere'.
 *
 * codex: codex virtualis macOS (Enter XXXVI, Tab XLVIII, Esc LIII...)
 * modificatores: vexilla NSEventModifierFlags (0 = nulli)
 * characteres: UTF-8 quos clavis PARIT (NIHIL pro clavibus mutis)
 * depressa: VERUM = keyDown, FALSUM = keyUp
 *
 * Redde FALSUM si fenestra abest aut eventus fingi non potuit.
 */

b32
fenestra_clavem_immittere (
              Fenestra* fenestra,
                   i32  codex,
                   i32  modificatores,
    constans character* characteres,
                   b32  depressa);

/* Fenestram clavem facere (focum rapere). Agitatio focum poscit;
 * usus humanus eum iam habet. */
vacuum
fenestra_clavem_capere (
    Fenestra* fenestra);


/* Clavem NOMINATAM immittere (depressam ET liberatam - ictus unus).
 *
 * Forma suturae imperii par (vide imperium.h); 'datum' Fenestra* est.
 * Focum RAPIT: eventus ad fenestram clavem it, ergo agitatio eam
 * clavem facere debet.
 *
 * Nomina: Enter Tab Escape Space Backspace Delete
 *         ArrowUp ArrowDown ArrowLeft ArrowRight
 *         Home End PageUp PageDown F1..F12
 * Modificatores praefixi: 'Cmd+' 'Ctrl+' 'Shift+' 'Alt+' (aut 'Opt+'),
 * cumulabiles: "Cmd+Shift+ArrowLeft".
 *
 * LITTERA aut NUMERUS UNUS, cum modificatore aut sine: "a", "7",
 * "Cmd+c", "Cmd+Shift+z" - pressio UNA nativa (keydown + keyup), ut
 * digitus hominis: aequivalentiae menu et brevitates paginae ('a',
 * 'n', '1'-'9' in document; lapide feature-requests/024). Maiuscula
 * sola ("A") Shift implicat. Shift cum numero numerum servat (signum
 * dispositionis res est). TEXTUS (chorda) per 'manus_scribere' manet,
 * quod dispositionis omnino nescium est.
 *
 * Dispositio hic NON obstat, quamquam prima specie obstare videtur.
 * MENSURATUM 2026-08-15: AppKit aequivalentias per characteres
 * congruit, non per codicem - spica codicem PRAVUM omni litterae
 * dedit et 'Cmd+c'/'Cmd+v' nihilominus egerunt. Tabula codicum
 * adest ob 'e.code' solum, quod POSITIONEM ex definitione nominat
 * (idem quod kVK_ANSI_* nominat). Vide lib/fenestra_macos.m pro
 * limite honesto in dispositionibus non-US.
 *
 * Redde FALSUM si nomen ignotum est (RECUSATIO, non ictus mutus).
 */
b32
fenestra_claviarius (
                vacuum* datum,
    constans character* clavis);


/* ==================================================
 * Mus (agitatio)
 * ==================================================
 *
 * CUR NATIVUS, ut claves: 'dispatchEvent(new MouseEvent(...))'
 * auditores JS excitat sed CSS ':hover' NON tangit - quod in domo
 * quattuor locis adhibetur (thema, villa, forum, mensor). Eventus
 * nativus positionem VERAM muris movet, ergo ':hover' congruit, et
 * tractus (mousedown/move/up) sicut manus hominis apparet.
 *
 * COORDINATAE PAGINAE HIC DANTUR (CSS px, origo SUMMA sinistra -
 * quod getBoundingClientRect reddit). Stratum hoc solum eas in
 * systema AppKit vertit (origo IMA sinistra), quia solum hic
 * geometria visus nota est. Quisquis supra stat de paginis loquitur.
 */

nomen enumeratio {
    FENESTRA_MUS_MOTUS = 0,      /* sine pyxide depressa */
    FENESTRA_MUS_DEPRESSIO,      /* sinistra deprimitur */
    FENESTRA_MUS_TRACTUS,        /* motus DUM sinistra depressa */
    FENESTRA_MUS_LIBERATIO,      /* sinistra liberatur */
    FENESTRA_MUS_DEPRESSIO_DEXTRA,
    FENESTRA_MUS_LIBERATIO_DEXTRA
} FenestraMusGenus;

b32
fenestra_murem_immittere (
            Fenestra* fenestra,
    FenestraMusGenus  genus,
                 i32  x,             /* CSS px a sinistra */
                 i32  y,             /* CSS px a SUMMO */
                 i32  modificatores);

/* Mus per NOMEN generis (forma suturae imperii par; 'datum'
 * Fenestra* est). Nomina: "motus" "depressio" "tractus" "liberatio"
 * "depressio-dextra" "liberatio-dextra".
 *
 * Nomen transit, non enumeratio: imperium de AppKit nihil scit, ut
 * in claviario. Redde FALSUM si genus ignotum. */
b32
fenestra_musarius (
                vacuum* datum,
    constans character* genus,
                   i32  x,
                   i32  y);


/* Magnitudinem ponere per suturam (forma imperii par; 'datum'
 * Fenestra* est).
 *
 * MENSURAM FACTAM REDDIT, non petitam - et haec sola causa cur
 * sutura haec parametros exitus fert ubi claviarius et musarius
 * nullos ferunt.
 *
 * CUR: systema minimas suas imponit (latitudo tituli, ornamenta), et
 * fenestra quae CCCXX petita CDXXX manet TACITE id facit. Sine
 * reditu, probatio dispositionis angustae in fenestra LATA curreret
 * et viridis esset - genus mendacii quod haec domus iam nimis saepe
 * vidit. Ergo numerus VERUS redit, et qui vocat comparare potest.
 *
 * Mensurae CONTENTAE sunt (area utilis), non totius formae: id est
 * quod pagina accipit, ergo id est quod probator cogitat.
 *
 * Redde FALSUM si mensura non positiva aut fenestra abest. */
b32
fenestra_magnitudinator (
    vacuum* datum,
       i32  latitudo,
       i32  altitudo,
       i32* latitudo_facta,
       i32* altitudo_facta);


/* ==================================================
 * Menu applicationis
 * ==================================================
 *
 * Rem in menu APPLICATIONIS addere, supra separatorem et 'Exire'.
 *
 * Menu UNUM toti applicationi est. Pressio NON revocatio est: eventum
 * EVENTUS_MENU cum datum.menu.signum in caudam HUIUS fenestrae ponit,
 * per eandem caudam ac claves - app eum in gyro suo legit.
 *
 * clavis: forma claviarii ("Cmd+Shift+v": modificantes, deinde
 * littera UNA) aut NIHIL (sine aequivalente).
 *
 * Redde FALSUM si fenestra aut titulus NIHIL, si menu applicationis
 * abest (fenestra_creare id struit), aut si clavis prava est.
 */

b32
fenestra_menu_addere (
               Fenestra* fenestra,
     constans character* titulus,
     constans character* clavis,
                    i32  signum);


/* ==================================================
 * Tabula Pixelorum - Creatio / Destructio
 * ================================================== */

/* Creare tabulam pixelorum
 *
 * Creat tabulam pixelorum cum altitudine fixa et latitudine
 * calculata ex ratione aspectus fenestrae.
 *
 * piscina: piscina pro allocando memoriam
 * fenestra: fenestra
 * altitudo_fixa: altitudo tabulae in pixelis
 *
 * Reddit: tabulam pixelorum novam vel NIHIL si error
 */
TabulaPixelorum*
fenestra_creare_tabulam_pixelorum (
     Piscina* piscina,
    Fenestra* fenestra,
         i32  altitudo_fixa);

/* Praesentare pixela ad fenestram
 *
 * Blit tabulam pixelorum ad fenestram (scalat si necessarium)
 *
 * fenestra: fenestra
 * tabula: tabula pixelorum
 */
vacuum
fenestra_praesentare_pixela (
           Fenestra* fenestra,
    TabulaPixelorum* tabula);


/* ==================================================
 * Functiones Temporis Platformae
 * ================================================== */

/* Obtinere pulsus temporis ad altam praecisionem
 *
 * Reddit: numerus pulsuum ex tempore arbitrario
 */
i64
fenestra_tempus_obtinere_pulsus (
    vacuum);

/* Obtinere frequentiam horologii
 *
 * Reddit: pulsus per secundum
 */
f64
fenestra_tempus_obtinere_frequentiam (
    vacuum);

/* Tempus currens in millisecundis - pulsus * M / frequentia. Sedes
 * UNICA horologii pro eventibus (ludus: tempus est datum in eventu).
 *
 * Reddit: millisecundae ex tempore arbitrario (eodem ac pulsus)
 */
s64
fenestra_tempus_ms (
    vacuum);

/* Dormire pro microsecundis datis
 *
 * microsecundae: numerus microsecundarum dormire
 */
vacuum
fenestra_dormire (
    i32 microsecundae);

#endif /* FENESTRA_H */
