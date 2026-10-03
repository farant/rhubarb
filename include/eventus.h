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
#include "chorda.h"


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
    EVENTUS_MENU,
    /* Vocabularium sine iactura (eventus A2; spec par. III) - ADDITA
     * ad finem: tituli in plagulis, ordo tabulae titulorum crescit. */
    EVENTUS_TEXTUS,          /* textus commissus aut componens */
    EVENTUS_DEPOSITIO,       /* viae depositae (aut glutinum promotum) */
    EVENTUS_SUSPENSIO,       /* processus suspensus (terminalis) */
    EVENTUS_RESUMPTIO,       /* processus resumptus */
    EVENTUS_FACULTATES,      /* facultates fontis (primus; et mutatae) */
    /* Tractus DERIVATI (eventus A5; spec D4): a ludo (derivare) solum,
     * numquam a fonte. INCIPIT: x/y = ORIGO (ubi pressio fuit), tempus
     * motus qui limen transiit; TRACTUS: positio currens; FINIT:
     * positio liberationis. botton = botton pressionis. */
    EVENTUS_TRACTUS_INCIPIT,
    EVENTUS_TRACTUS,
    EVENTUS_TRACTUS_FINIT
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
 * Modificantes LATERUM (eventus A2)
 * ==================================================
 * Valores = larvae macOS 'device-dependent' (NX_DEVICE*): fenestra
 * modifierFlags crudos iam fert, ergo latera iam in eventibus et
 * plagulis sunt. Conventio vocabularii: fontes alii (terminalis per
 * kitty) in has vertunt. Solum si facultas 'latera'. */
#define MOD_IMPERIUM_SINISTER  0x0001
#define MOD_SHIFT_SINISTER     0x0002
#define MOD_SHIFT_DEXTER       0x0004
#define MOD_SUPER_SINISTER     0x0008
#define MOD_SUPER_DEXTER       0x0010
#define MOD_ALT_SINISTER       0x0020
#define MOD_ALT_DEXTER         0x0040
#define MOD_IMPERIUM_DEXTER    0x2000


/* ==================================================
 * Codex PHYSICUS clavis (spec D3: subsectio W3C 'code')
 * ==================================================
 * Positio clavis, non dispositio (clavis logica in 'clavis' et
 * 'runa'). Tituli plagularum = nomina W3C ("KeyA", "ArrowLeft") per
 * eventus_codex_titulus (eventus_stml.h). Litterae et numeri ut
 * ORDINES: EVENTUS_CODEX_LITTERAE + (c - 'A'), EVENTUS_CODEX_NUMERI
 * + d, EVENTUS_CODEX_FUNCTIONES + (n - 1) pro F1..F12. */

nomen enumeratio {
    EVENTUS_CODEX_IGNOTUS = ZEPHYRUM,
    EVENTUS_CODEX_LITTERAE,                          /* KeyA .. KeyZ */
    EVENTUS_CODEX_NUMERI = EVENTUS_CODEX_LITTERAE + XXVI, /* Digit0..9 */
    EVENTUS_CODEX_FUNCTIONES = EVENTUS_CODEX_NUMERI + X,  /* F1..F12 */
    EVENTUS_CODEX_SPATIUM = EVENTUS_CODEX_FUNCTIONES + XII,
    EVENTUS_CODEX_REDITUS,
    EVENTUS_CODEX_TABULA,
    EVENTUS_CODEX_RETRORSUM,
    EVENTUS_CODEX_EFFUGIUM,
    EVENTUS_CODEX_SAGITTA_SINISTRA,
    EVENTUS_CODEX_SAGITTA_DEXTRA,
    EVENTUS_CODEX_SAGITTA_SURSUM,
    EVENTUS_CODEX_SAGITTA_DEORSUM,
    EVENTUS_CODEX_DOMUS,
    EVENTUS_CODEX_FINIS,
    EVENTUS_CODEX_PAGINA_SURSUM,
    EVENTUS_CODEX_PAGINA_DEORSUM,
    EVENTUS_CODEX_DELERE,
    EVENTUS_CODEX_INSERERE,
    EVENTUS_CODEX_GRAVIS,             /* Backquote */
    EVENTUS_CODEX_MINUS,
    EVENTUS_CODEX_AEQUALE,
    EVENTUS_CODEX_UNCUS_SINISTER,     /* BracketLeft */
    EVENTUS_CODEX_UNCUS_DEXTER,
    EVENTUS_CODEX_VIRGULA_INVERSA,    /* Backslash */
    EVENTUS_CODEX_PUNCTUM_VIRGULA,    /* Semicolon */
    EVENTUS_CODEX_APOSTROPHUS,        /* Quote */
    EVENTUS_CODEX_VIRGULA,            /* Comma */
    EVENTUS_CODEX_PUNCTUM,            /* Period */
    EVENTUS_CODEX_VIRGULA_OBLIQUA,    /* Slash */
    EVENTUS_CODEX_MAIUSCULA_SINISTRA, /* ShiftLeft */
    EVENTUS_CODEX_MAIUSCULA_DEXTRA,
    EVENTUS_CODEX_IMPERIUM_SINISTRUM, /* ControlLeft */
    EVENTUS_CODEX_IMPERIUM_DEXTRUM,
    EVENTUS_CODEX_ALTERUM_SINISTRUM,  /* AltLeft */
    EVENTUS_CODEX_ALTERUM_DEXTRUM,
    EVENTUS_CODEX_SUPER_SINISTRUM,    /* MetaLeft */
    EVENTUS_CODEX_SUPER_DEXTRUM,
    EVENTUS_CODEX_SERA_MAIUSCULARUM,  /* CapsLock */
    EVENTUS_CODICES_NUMERUS
} EventusCodex;

/* Actio clavis: ITERATA = auto-repetitio (genus DEPRESSUS manet);
 * SOLUTA in LIBERATUS. */
nomen enumeratio {
    EVENTUS_ACTIO_PRESSA = ZEPHYRUM,
    EVENTUS_ACTIO_ITERATA,
    EVENTUS_ACTIO_SOLUTA
} EventusActio;

nomen enumeratio {
    EVENTUS_INDICATOR_MUS = ZEPHYRUM,
    EVENTUS_INDICATOR_STILUS,
    EVENTUS_INDICATOR_TACTUS
} EventusIndicatorGenus;

#define EVENTUS_PRESSIO_IGNOTA (-1)   /* pressio 0..M; mus, terminalis */
#define EVENTUS_EXEMPLA_MAXIMA LXIV   /* spec D5 */

/* Exemplum motus coaliti (spec Q13): positio + tempus */
nomen structura {
    s32 x;
    s32 y;
    s64 tempus;
} EventusExemplum;

nomen enumeratio {
    EVENTUS_ROTULA_IGNOTA = ZEPHYRUM,  /* plagulae veteres (f32 sola) */
    EVENTUS_ROTULA_PRAECISA,            /* trackpad: pixela */
    EVENTUS_ROTULA_GRADATA             /* rota, terminalis: gradus */
} EventusRotulaGenus;

nomen enumeratio {
    EVENTUS_TEXTUS_COMMISSUM = ZEPHYRUM,
    EVENTUS_TEXTUS_COMPONENS           /* praeeditio IME */
} EventusTextusGenus;

nomen enumeratio {
    EVENTUS_ORIGO_SCRIPTA = ZEPHYRUM,  /* typed */
    EVENTUS_ORIGO_GLUTINATA,           /* pasted */
    EVENTUS_ORIGO_COMPOSITA            /* IME commit, dead keys */
} EventusOrigo;

/* Facultates trinae */
#define EVENTUS_FACULTAS_NULLA       ZEPHYRUM
#define EVENTUS_FACULTAS_FORTASSE    I    /* scriptura copiae: OSC 52 */
#define EVENTUS_FACULTAS_CERTA       II
#define EVENTUS_DEPOSITIO_NULLA      ZEPHYRUM
#define EVENTUS_DEPOSITIO_HEURISTICA I    /* glutinum viarum promotum */
#define EVENTUS_DEPOSITIO_NATIVA     II

/* Quod fons NARRARE potest (spec Q4, Q17): semel primus, iterum si
 * discit (per observationem, numquam per quaestionem). */
nomen structura {
    b32 liberationes;       /* SOLUTA nuntiatur */
    b32 codex_physicus;     /* 'codex' verus */
    b32 tabula_distincta;   /* Tab != Ctrl+I */
    b32 latera;             /* MOD_*_SINISTER/DEXTER */
    b32 super;              /* hover (motus sine botone) */
    b32 praeeditio;         /* TEXTUS COMPONENS */
    i32 scriptura_copiae;   /* EVENTUS_FACULTAS_* */
    i32 depositio;          /* EVENTUS_DEPOSITIO_* */
    s32 gradus_rotulae;     /* pixela nostra per gradum (GRADATA) */
    b32 pressio;            /* pressio indicatoris vera */
    /* B4: modificantes clavium imprimibilium narrantur (Shift+A non
     * solum 'A'). Legacy FALSUM (Shift ut Caps Lock videtur); kitty
     * cum vexillo OMNES VERUM; fenestra VERUM. Ad finem additum. */
    b32 modificantes_textus;
} EventusFacultates;


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
             clavis_t clavis;        /* LOGICA (nominata; ASCII ut olim) */
                  s32 producta;      /* character a clave PRODUCTUS sub
                                      * modificantibus (Unicode, 0 =
                                      * nullus): solus sub Alt/Ctrl/Cmd,
                                      * ubi TEXTUS deest (S3a; olim
                                      * 'typus', character ASCII) */
                  i32 modificantes;  /* MOD_* + latera */
                  s32 runa;          /* runa sine maiuscula (logica) */
         EventusCodex codex;         /* PHYSICUS; IGNOTUS si nescitur */
         EventusActio actio;
        } clavis;
        structura {
                     s32 x;          /* pixela NOSTRA; extra fenestram
                                      * negativa aut >= latitudo (A3c) */
                     s32 y;
            mus_botton_t botton;
                     i32 modificantes;
                     s32 indicator;           /* 0 = mus */
    EventusIndicatorGenus indicator_genus;
                     s32 pressio;             /* 0..M; IGNOTA = -1 */
    constans EventusExemplum* exempla;        /* VISUS (Q16) */
                     i32 numerus_exemplorum;
        } mus;
        structura {
            s32 dx;                  /* pixela nostra, integra */
            s32 dy;
            EventusRotulaGenus genus;
            /* B3a: positio indicatoris (pixela nostra, ut mus) et
             * modificantes - shift+rota, ctrl+rota (zoom); rotula ad
             * tabulam sub indicatore destinari potest */
            s32 x;
            s32 y;
            i32 modificantes;
        } rotula;
        structura {
                chorda contentum;    /* VISUS usque ad lectionem proximam */
    EventusTextusGenus genus;
                   s32 cursor;       /* COMPONENS: octetus */
          EventusOrigo origo;
                   b32 truncatum;
        } textus;
        structura {
               s32 x;
               s32 y;
            chorda viae;             /* VISUS: viae absolutae, '\n' */
               i32 numerus;
               b32 promota;          /* glutinum terminalis promotum */
        } depositio;
        EventusFacultates facultates;
        structura {
            i32 signum;         /* a fenestra_menu_addere datum */
        } menu;
    } datum;
} Eventus;

/* Eventus NOTATUS (eventus A6; spec D6): eventus crudus + destinatum
 * eius a dispensatore resolutum - notarius (dispensator_notarium_
 * ponere) scribit, non fons. scopus = id componentis (vacuus: nullus,
 * e.g. clavis), scopus_x/y = punctum in spatio eius. Plagula:
 * eventus_notata_scribere_stml. Eventus ipse scopum non fert: scopus
 * datum fontis non est. */
nomen structura {
    Eventus eventus;
     chorda scopus;
        s32 scopus_x;
        s32 scopus_y;
} EventusNotatum;

#endif /* EVENTUS_H */
