#ifndef EVENTUS_H
#define EVENTUS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

/* eventus.h - Vocabularium initus commune: genera eventuum, claves,
 * modificantes, bottones, Eventus (project-specs/eventus-spec.md D1).
 *
 * Ex fenestra.h verbatim divisum (eventus A1, 2026-10-01): aedilis per
 * CAPUT nectit - fenestra.h includere fenestra_macos.m + -framework
 * Cocoa trahit. Ludus (dispensator, derivare, destinatio, actio,
 * eventus_stml) hoc caput solum includit, ergo applicationes ludi sine
 * fenestra nectuntur (scopus terminalis, probationes). fenestra.h hoc
 * includit: vocantes eius nihil mutatum vident. Typi soli - nullum
 * corpus.
 *
 * Vocabularium sine iactura (clavis logica + codex physicus, textus
 * separatus, facultates) in A2 crescit; hic adhuc forma vetus.
 */

#include "latina.h"


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
    EVENTUS_MUS_ROTULA,
    EVENTUS_MUS_DUPLEX,         /* Double-click (derivatum) */
    /* Derivata a dispensatore (ludus): numquam a fenestra
     * emissa. Ordo = tabula titulorum eventus_stml.c. */
    EVENTUS_MUS_INTRAVIT,
    EVENTUS_MUS_EXIIT,
    EVENTUS_FOCUS_CAPTUS,
    EVENTUS_FOCUS_AMISSUS,
    EVENTUS_FOCUS_PETITUS,
    /* res menu applicationis pressa (fenestra_menu_addere) */
    EVENTUS_MENU
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

    /* Tempus eventus in MILLISECUNDIS (s64). A fenestra stampatum in
     * productione, a plagula in replay. TEMPUS EST DATUM IN EVENTU -
     * nihil infra fenestram horologium vocat. ZEPHYRUM = nondum
     * stampatum: impellere_eventum id implet. */
    s64 tempus;
    unio {
        structura {
            i32 latitudo;
            i32 altitudo;
        } mutare_magnitudinem;
        structura {
             clavis_t clavis;
            character typus;          /* Character typed (0 if none/special key) */
                  i32 modificantes;
        } clavis;
        structura {
                     i32 x;
                     i32 y;
            mus_botton_t botton;
                     i32 modificantes;
        } mus;
        structura {
            f32 delta_x;
            f32 delta_y;
        } rotula;
        structura {
            i32 signum;         /* a fenestra_menu_addere datum */
        } menu;
    } datum;
} Eventus;

#endif /* EVENTUS_H */
