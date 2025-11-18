#ifndef FENESTRA_H
#define FENESTRA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

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
    FENESTRA_ORDINARIA     = FENESTRA_MUTABILIS | FENESTRA_CLAUDIBILIS |
                             FENESTRA_MINUIBILIS | FENESTRA_MAXIMIZABILIS
} fenestra_vexilla_t;


/* ==================================================
 * Constantae - Genera Eventuum
 * ================================================== */

/* Genera eventuum */
nomen enumeratio {
    EVENTUS_NIHIL = ZEPHYRUM,
    EVENTUS_CLAUDERE,
    EVENTUS_MUTARE_MAGNITUDINEM,
    EVENTUS_FOCUS,
    EVENTUS_DEFOCUS,
    EVENTUS_EXPONERE,
    EVENTUS_CLAVIS_DEPRESSUS,
    EVENTUS_CLAVIS_LIBERATUS,
    EVENTUS_MUS_DEPRESSUS,
    EVENTUS_MUS_LIBERATUS,
    EVENTUS_MUS_MOTUS,
    EVENTUS_MUS_ROTULA
} eventus_genus_t;


/* ==================================================
 * Constantae - Codices Clavium
 * ================================================== */

/* Codices clavium */
nomen enumeratio {
    CLAVIS_IGNOTA = ZEPHYRUM,

    /* Characteres ASCII imprimibiles (32-126) sunt valores ASCII eorum */
    CLAVIS_SPATIUM = XXXII,

    /* Characteres imperantes */
    CLAVIS_EFFUGIUM = XXVII,
    CLAVIS_REDITUS = XIII,
    CLAVIS_TABULA = IX,
    CLAVIS_RETRORSUM = VIII,
    CLAVIS_DELERE = CXXVII,

    /* Claves navigationis (256+) */
    CLAVIS_SINISTER = CCLVI,
    CLAVIS_DEXTER,
    CLAVIS_SURSUM,
    CLAVIS_DEORSUM,
    CLAVIS_DOMUS,
    CLAVIS_FINIS,
    CLAVIS_PAGINA_SURSUM,
    CLAVIS_PAGINA_DEORSUM,

    /* Claves functionis */
    CLAVIS_F1 = CCXC,
    CLAVIS_F2,
    CLAVIS_F3,
    CLAVIS_F4,
    CLAVIS_F5,
    CLAVIS_F6,
    CLAVIS_F7,
    CLAVIS_F8,
    CLAVIS_F9,
    CLAVIS_F10,
    CLAVIS_F11,
    CLAVIS_F12,

    /* Claves modificantes */
    CLAVIS_SINISTER_SHIFT = CCCXL,
    CLAVIS_DEXTER_SHIFT,
    CLAVIS_SINISTER_IMPERIUM,
    CLAVIS_DEXTER_IMPERIUM,
    CLAVIS_SINISTER_ALT,
    CLAVIS_DEXTER_ALT,
    CLAVIS_SINISTER_SUPER,
    CLAVIS_DEXTER_SUPER,
    CLAVIS_CAPS_LOCK,
    CLAVIS_NUM_LOCK
} clavis_t;


/* ==================================================
 * Constantae - Vexilla Modificantium
 * ================================================== */

/* Vexilla modificantium pro eventibus clavis/muris */
nomen enumeratio {
    MOD_SHIFT     = 0x020000,
    MOD_IMPERIUM  = 0x040000,
    MOD_ALT       = 0x080000,
    MOD_SUPER     = 0x100000,
    MOD_CAPS_LOCK = 0x010000,
    MOD_NUM_LOCK  = 0x200000
} mod_vexilla_t;


/* ==================================================
 * Constantae - Bottones Muris
 * ================================================== */

/* Bottones muris */
nomen enumeratio {
    MUS_SINISTER = I,
    MUS_DEXTER   = II,
    MUS_MEDIUS   = III
} mus_botton_t;


/* ==================================================
 * Typi - Eventus
 * ================================================== */

/* Structura eventi fenestrae */
nomen structura {
    eventus_genus_t genus;
    unio {
        structura {
            i32 latitudo;
            i32 altitudo;
        } mutare_magnitudinem;
        structura {
            clavis_t clavis;
            i32      modificantes;
        } clavis;
        structura {
            i32          x;
            i32          y;
            mus_botton_t botton;
            i32          modificantes;
        } mus;
        structura {
            f32 delta_x;
            f32 delta_y;
        } rotula;
    } datum;
} Eventus;


/* ==================================================
 * Typi - Configuratio Fenestrae
 * ================================================== */

/* Configuratio fenestrae */
nomen structura {
    constans character* titulus;
    i32 x;
    i32 y;
    i32 latitudo;
    i32 altitudo;
    i32 vexilla;
} FenestraConfiguratio;


/* ==================================================
 * Typi - Tabula Pixelorum
 * ================================================== */

/* Tabula pixelorum */
nomen structura {
    i32* pixela;              /* Forma RGBA8888 */
    i32  latitudo;            /* Latitudo tabulae in pixelis */
    i32  altitudo;            /* Altitudo tabulae */
    i32  fenestra_latitudo;   /* Latitudo fenestrae actualis */
    i32  fenestra_altitudo;   /* Altitudo fenestrae actualis */
    f32  scala;               /* Factor scalae pro reddendo */
} TabulaPixelorum;


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
    Piscina*                       piscina,
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
    Piscina*  piscina,
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
 * Tabula Pixelorum - Manipulatio
 * ================================================== */

/* Vacare tabulam pixelorum
 *
 * Implet tabulam cum colore dato
 *
 * tabula: tabula pixelorum
 * color: color (RGBA8888)
 */
vacuum
tabula_pixelorum_vacare (
    TabulaPixelorum* tabula,
                i32  color);

/* Ponere pixelum
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * color: color (RGBA8888)
 */
vacuum
tabula_pixelorum_ponere_pixelum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
                i32  color);

/* Obtinere pixelum
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 *
 * Reddit: color (RGBA8888)
 */
i32
tabula_pixelorum_obtinere_pixelum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y);


/* ==================================================
 * Macros - Colores
 * ================================================== */

/* Creare colorem RGBA */
#define RGBA(r, g, b, a) (((i32)(a) << XXIV) | ((i32)(b) << XVI) | ((i32)(g) << VIII) | (i32)(r))

/* Creare colorem RGB (alpha = 255) */
#define RGB(r, g, b) RGBA(r, g, b, CCLV)


/* ==================================================
 * Reddere Textus
 * ================================================== */

/* Pingere characterem
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * c: character pingendus
 * color: color textus (RGBA8888)
 */
vacuum
tabula_pixelorum_pingere_characterem (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
          character  c,
                i32  color);

/* Pingere chordam
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * chorda: chorda pingenda
 * color: color textus (RGBA8888)
 */
vacuum
tabula_pixelorum_pingere_chordam (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color);

/* Pingere characterem scalatum
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * c: character pingendus
 * color: color textus (RGBA8888)
 * scala: factor scalae (1 = normalis, 2 = duplex, etc.)
 */
vacuum
tabula_pixelorum_pingere_characterem_scalatum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
          character  c,
                i32  color,
                i32  scala);

/* Pingere chordam scalatam
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * chorda: chorda pingenda
 * color: color textus (RGBA8888)
 * scala: factor scalae (1 = normalis, 2 = duplex, etc.)
 */
vacuum
tabula_pixelorum_pingere_chordam_scalatam (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color,
                i32  scala);

/* Pingere chordam praecisum
 *
 * tabula: tabula pixelorum
 * x: coordinata x
 * y: coordinata y
 * chorda: chorda pingenda
 * color: color textus (RGBA8888)
 * praecisio_x: x rectanguli praecisionis
 * praecisio_y: y rectanguli praecisionis
 * praecisio_latitudo: latitudo rectanguli praecisionis
 * praecisio_altitudo: altitudo rectanguli praecisionis
 */
vacuum
tabula_pixelorum_pingere_chordam_praecisum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color,
                i32  praecisio_x,
                i32  praecisio_y,
                i32  praecisio_latitudo,
                i32  praecisio_altitudo);


/* ==================================================
 * Mensura Textus
 * ================================================== */

/* Obtinere latitudinem chordae
 *
 * chorda: chorda mensuranda
 * scala: factor scalae
 *
 * Reddit: latitudo in pixelis
 */
i32
fons_latitudo_chordae (
             chorda  textus,
                i32  scala);

/* Obtinere altitudinem chordae
 *
 * scala: factor scalae
 *
 * Reddit: altitudo in pixelis
 */
i32
fons_altitudo_chordae (
    i32 scala);


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

/* Dormire pro microsecundis datis
 *
 * microsecundae: numerus microsecundarum dormire
 */
vacuum
fenestra_dormire (
    i32 microsecundae);

#endif /* FENESTRA_H */
