#ifndef PAGINA_H
#define PAGINA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fenestra.h"

/* ==================================================
 * PAGINA - Editor Textus Fixae Magnitudinis
 *
 * Editor textus simplex pro paginis fixae magnitudinis.
 * Paginae sunt 70 characteres × 55 lineae = 3,850 characteres.
 *
 * PHILOSOPHIA:
 * - Magnitudo fixa (non crescit)
 * - Cursor est offset in bytes
 * - Lineae fractae computantur in reddendo
 * - Selectio pro tags et copiando
 *
 * EXEMPLUM:
 *   Pagina pagina;
 *   pagina_initiare(&pagina, chorda_internare_ex_literis(intern, "page:0"));
 *
 *   pagina_inserere_characterem(&pagina, 'H');
 *   pagina_inserere_characterem(&pagina, 'i');
 *   pagina_inserere_characterem(&pagina, '\n');
 *
 *   pagina_reddere(tabula, &pagina, 0, 0, 70, 55, 1);
 *
 * ================================================== */


/* ==================================================
 * Constantae
 * ================================================== */

#define PAGINA_LATITUDO_CHARACTERA LXX          /* 70 chars per line */
#define PAGINA_ALTITUDO_LINEA LV                /* 55 lines per page */
#define PAGINA_CAPACITAS (LXX * LV)             /* 3,850 total chars */


/* ==================================================
 * Typi
 * ================================================== */

/* Modi editoris (vim-style) */
nomen enumeratio {
    MODO_NORMAL = ZEPHYRUM,
    MODO_INSERT
} ModoEditor;

/* Pagina textus */
nomen structura {
    character buffer[PAGINA_CAPACITAS];                     /* Text buffer */
    i32 longitudo;                                          /* Current length */
    i32 cursor;                                             /* Cursor position */
    s32 selectio_initium;                                   /* Selection start (-1 if none) */
    s32 selectio_finis;                                     /* Selection end */
    chorda* identificator; /* "page:5" or "page:repl" - owned by caller's piscina/internamentum */

    /* Vim state */
    ModoEditor modo;                                        /* Normal or Insert mode */
    character clavis_praecedens;                            /* For dd, dG, d$ commands */
    b32 esperans_fd;                                        /* fd escape sequence */
    f64 tempus_f;                                           /* Timing for fd */

    /* Cursor blink state */
    f64 tempus_cursor_ultimus;                              /* Last cursor blink time */
    b32 cursor_visibilis;                                   /* Cursor visibility state */
} Pagina;


/* ==================================================
 * RegioClicca - Clickable Tag Region
 * ================================================== */

/* Regio clickable in textu (tag, command, link) */
nomen structura RegioClicca {
    i32 initium;             /* Start index in buffer */
    i32 finis;               /* End index in buffer */
    character genus[XVI];    /* Tag type: "command", "link", "block" */
    character datum[LXIV];   /* Command/link name or content */
} RegioClicca;


/* ==================================================
 * Creatio / Initiatio
 * ================================================== */

/* Initiare paginam
 *
 * Ponit paginam ad statum initialem cum identificatore dato.
 *
 * pagina: pagina initianda
 * identificator: identificator paginae (e.g. "page:5", "page:repl")
 *                debet esse chorda* ex internamento vel piscina - Pagina non copiat
 */
vacuum
pagina_initiare (
    Pagina* pagina,
    chorda* identificator);

/* Vacare paginam
 *
 * Removet omnem textum ex pagina.
 *
 * pagina: pagina vacanda
 */
vacuum
pagina_vacare (
    Pagina* pagina);


/* ==================================================
 * Editio - Insertio
 * ================================================== */

/* Inserere characterem ad cursorem
 *
 * Inserit characterem ad positionem currentem cursoris.
 * Si pagina plena est, nihil facit.
 *
 * pagina: pagina
 * c: character inserendus
 *
 * Reddit: VERUM si insertus, FALSUM si plena
 */
b32
pagina_inserere_characterem (
    Pagina* pagina,
    character c);

/* Inserere chordam ad cursorem
 *
 * Inserit chordam ad positionem currentem cursoris.
 * Si pagina non habet spatium sufficiens, inserit quantum potest.
 *
 * pagina: pagina
 * textus: chorda inserenda
 *
 * Reddit: numerus characterum insertorum
 */
i32
pagina_inserere_chordam (
    Pagina* pagina,
    constans character* textus);


/* ==================================================
 * Editio - Deletio
 * ================================================== */

/* Delere characterem ante cursorem (backspace)
 *
 * Removet characterem ante cursorem et movet cursorem sinistram.
 *
 * pagina: pagina
 *
 * Reddit: VERUM si deletum, FALSUM si cursor ad initium
 */
b32
pagina_delere_characterem (
    Pagina* pagina);

/* Delere characterem ad cursorem (delete key)
 *
 * Removet characterem ad cursorem, cursor manet.
 *
 * pagina: pagina
 *
 * Reddit: VERUM si deletum, FALSUM si cursor ad finem
 */
b32
pagina_delere_characterem_ante (
    Pagina* pagina);

/* Delere selectionem
 *
 * Removet textum selectum. Si nulla selectio, nihil facit.
 *
 * pagina: pagina
 *
 * Reddit: VERUM si deletum, FALSUM si nulla selectio
 */
b32
pagina_delere_selectionem (
    Pagina* pagina);


/* ==================================================
 * Motus Cursoris
 * ================================================== */

/* Movere cursorem sinistram
 *
 * pagina: pagina
 *
 * Reddit: VERUM si motus, FALSUM si ad initium
 */
b32
pagina_movere_cursor_sinistram (
    Pagina* pagina);

/* Movere cursorem dextram
 *
 * pagina: pagina
 *
 * Reddit: VERUM si motus, FALSUM si ad finem
 */
b32
pagina_movere_cursor_dextram (
    Pagina* pagina);

/* Movere cursorem sursum
 *
 * Movet cursorem ad lineam superiorem, tenens columnam si possibile.
 *
 * pagina: pagina
 *
 * Reddit: VERUM si motus, FALSUM si ad primam lineam
 */
b32
pagina_movere_cursor_sursum (
    Pagina* pagina);

/* Movere cursorem deorsum
 *
 * Movet cursorem ad lineam inferiorem, tenens columnam si possibile.
 *
 * pagina: pagina
 *
 * Reddit: VERUM si motus, FALSUM si ad ultimam lineam
 */
b32
pagina_movere_cursor_deorsum (
    Pagina* pagina);

/* Movere cursorem ad initium lineae (Home)
 *
 * pagina: pagina
 */
vacuum
pagina_movere_cursor_domus (
    Pagina* pagina);

/* Movere cursorem ad finem lineae (End)
 *
 * pagina: pagina
 */
vacuum
pagina_movere_cursor_finis (
    Pagina* pagina);

/* Ponere cursorem directe
 *
 * pagina: pagina
 * positio: nova positio cursoris (0 ad longitudo)
 */
vacuum
pagina_ponere_cursor (
    Pagina* pagina,
    i32 positio);


/* ==================================================
 * Selectio
 * ================================================== */

/* Ponere selectionem
 *
 * pagina: pagina
 * initium: initium selectionis
 * finis: finis selectionis
 */
vacuum
pagina_ponere_selectionem (
    Pagina* pagina,
    i32 initium,
    i32 finis);

/* Vacare selectionem
 *
 * pagina: pagina
 */
vacuum
pagina_vacare_selectionem (
    Pagina* pagina);

/* Obtinere textum selectum
 *
 * Copiat textum selectum ad buffer exitus.
 *
 * pagina: pagina
 * exitus: buffer pro textu (debet esse magnitudo sufficiens)
 * maxima_longitudo: magnitudo maxima buffer exitus
 *
 * Reddit: longitudo textus copiati
 */
i32
pagina_obtinere_textum_selectum (
    constans Pagina* pagina,
    character* exitus,
    i32 maxima_longitudo);

/* Habet selectionem
 *
 * pagina: pagina
 *
 * Reddit: VERUM si habet selectionem
 */
b32
pagina_habet_selectionem (
    constans Pagina* pagina);


/* ==================================================
 * Indentatio
 * ================================================== */

/* Obtinere indentationem lineae currentis
 *
 * Copiat spatia et tabs initiales lineae currentis ad buffer.
 *
 * pagina: pagina
 * exitus: buffer pro indentation (debet esse magnitudo sufficiens)
 * maxima_longitudo: magnitudo maxima buffer exitus
 *
 * Reddit: longitudo indentationis
 */
i32
pagina_obtinere_indentationem_lineae (
    constans Pagina* pagina,
    character* exitus,
    i32 maxima_longitudo);


/* ==================================================
 * Utilitas Linearum
 * ================================================== */

/* Invenire initium lineae currentis
 *
 * pagina: pagina
 * offset: positio in textu
 *
 * Reddit: offset initii lineae
 */
i32
pagina_invenire_initium_lineae (
    constans Pagina* pagina,
    i32 offset);

/* Invenire finem lineae currentis
 *
 * pagina: pagina
 * offset: positio in textu
 *
 * Reddit: offset finis lineae (ante newline, vel finis textus)
 */
i32
pagina_invenire_finem_lineae (
    constans Pagina* pagina,
    i32 offset);

/* Movere ad initium verbi proximi (w)
 *
 * pagina: pagina
 */
vacuum
pagina_movere_ad_verbum_proximum (
    Pagina* pagina);

/* Movere ad initium verbi praecedentis (b)
 *
 * pagina: pagina
 */
vacuum
pagina_movere_ad_verbum_praecedens (
    Pagina* pagina);


/* ==================================================
 * Interrogationes
 * ================================================== */

/* Est plena
 *
 * pagina: pagina
 *
 * Reddit: VERUM si plena
 */
b32
pagina_est_plena (
    constans Pagina* pagina);

/* Est vacua
 *
 * pagina: pagina
 *
 * Reddit: VERUM si vacua
 */
b32
pagina_est_vacua (
    constans Pagina* pagina);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere paginam
 *
 * Pingit textum paginae ad tabulam pixelorum.
 * Computat fractiones linearum durante reddendo.
 *
 * tabula: tabula pixelorum
 * pagina: pagina reddenda
 * x: columna originis (in grid coordinates)
 * y: linea originis (in grid coordinates)
 * latitudo: latitudo in characteribus
 * altitudo: altitudo in lineis
 * scala: factor scalae fontis (1 = 6x8, 2 = 12x16)
 */
vacuum
pagina_reddere (
    TabulaPixelorum* tabula,
    Pagina* pagina,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 scala);

/* Reddere paginam cum margine (border et status)
 *
 * Pingit paginam cum border, titulo (identificator) in summo,
 * et indicatore modi (NORMAL/INSERT) in imo.
 *
 * piscina: piscina pro allocare contextum delineandi
 * tabula: tabula pixelorum
 * pagina: pagina reddenda
 * x: columna originis (in grid coordinates)
 * y: linea originis (in grid coordinates)
 * latitudo: latitudo totalis in characteribus (includens border)
 * altitudo: altitudo totalis in lineis (includens border)
 * scala: factor scalae fontis (1 = 6x8, 2 = 12x16)
 * focused: VERUM si widget habet focus (mutat colorem border)
 */
vacuum
pagina_reddere_cum_margine (
    Piscina* piscina,
    TabulaPixelorum* tabula,
    Pagina* pagina,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 scala,
    b32 focused);


/* ==================================================
 * Tractare Eventus (Vim Mode)
 * ================================================== */

/* Tractare eventum clavis (vim mode)
 *
 * Tractat eventus clavis in modo vim (normal et insert).
 * Includit omnia commandos vim: hjkl, dd, dG, d$, o/O, etc.
 *
 * pagina: pagina
 * eventus: eventus tractandus
 *
 * Reddit: VERUM si eventus tractatus, FALSUM si debet claudere (ESC in normal)
 */
b32
pagina_tractare_eventum (
    Pagina* pagina,
    constans Eventus* eventus);


/* ==================================================
 * Tag Detection
 * ================================================== */

/* Obtinere regio clickable ad coordinatas characterum
 *
 * Detectat tags ad coordinatas datae ($command, #link, <$block>).
 * Coordinatae sunt relative ad aream textus (non originem widget).
 *
 * pagina: pagina
 * x_char, y_char: coordinatae characterum (relative to text area)
 * regio: exitus - regio detecta
 *
 * Reddit: VERUM si regio inventa, FALSUM si non
 */
b32
pagina_obtinere_regio_ad_punctum(
    constans Pagina* pagina,
    i32 x_char,
    i32 y_char,
    RegioClicca* regio);

#endif /* PAGINA_H */
