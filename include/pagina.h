#ifndef PAGINA_H
#define PAGINA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fenestra.h"
#include "tabula_characterum.h"
#include "vim.h"

/* ==================================================
 * PAGINA - Editor Textus 2D Grid
 *
 * Editor textus simplex basatus in TabulaCharacterum (2D grid)
 * et VimStatus (vim command interpreter).
 *
 * PHILOSOPHIA:
 * - 2D grid (68 columns × 56 rows)
 * - Cursor est (linea, columna)
 * - Vim logica delegata ad vim.c
 * - Pagina tractat UI/rendering tantum
 *
 * EXEMPLUM:
 *   Pagina pagina;
 *   pagina_initiare(&pagina, chorda_internare_ex_literis(intern, "page:0"));
 *
 *   pagina_tractare_eventum(&pagina, &eventus);
 *   pagina_reddere_cum_margine(piscina, tabula, &pagina, ...);
 *
 * ================================================== */


/* ==================================================
 * Typi
 * ================================================== */

/* Pagina textus */
nomen structura {
    /* 2D character grid */
    TabulaCharacterum tabula;

    /* Vim status (cursor, modo, etc.) */
    VimStatus vim;

    /* Identificator paginae */
    chorda* identificator;

    /* Cursor blink state */
    f64 tempus_cursor_ultimus;
    b32 cursor_visibilis;
} Pagina;


/* ==================================================
 * RegioClicca - Clickable Tag Region
 * ================================================== */

/* Regio clickable in textu (tag, command, link) */
nomen structura RegioClicca {
    i32 initium_linea;
    i32 initium_columna;
    i32 finis_linea;
    i32 finis_columna;
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
pagina_initiare(
    Pagina* pagina,
    chorda* identificator);

/* Vacare paginam
 *
 * Removet omnem textum ex pagina.
 *
 * pagina: pagina vacanda
 */
vacuum
pagina_vacare(
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
pagina_est_plena(
    constans Pagina* pagina);

/* Est vacua
 *
 * pagina: pagina
 *
 * Reddit: VERUM si vacua
 */
b32
pagina_est_vacua(
    constans Pagina* pagina);

/* Obtinere modum currentem
 *
 * pagina: pagina
 *
 * Reddit: modus vim (MODO_VIM_NORMALIS, MODO_VIM_INSERERE, etc.)
 */
ModoVim
pagina_obtinere_modum(
    constans Pagina* pagina);


/* ==================================================
 * Reddere
 * ================================================== */

/* Reddere paginam
 *
 * Pingit textum paginae ad tabulam pixelorum.
 *
 * tabula_pixelorum: tabula pixelorum
 * pagina: pagina reddenda
 * x: columna originis (in grid coordinates)
 * y: linea originis (in grid coordinates)
 * latitudo: latitudo in characteribus
 * altitudo: altitudo in lineis
 * scala: factor scalae fontis (1 = 6x8, 2 = 12x16)
 */
vacuum
pagina_reddere(
    TabulaPixelorum* tabula_pixelorum,
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
 * tabula_pixelorum: tabula pixelorum
 * pagina: pagina reddenda
 * x: columna originis (in grid coordinates)
 * y: linea originis (in grid coordinates)
 * latitudo: latitudo totalis in characteribus (includens border)
 * altitudo: altitudo totalis in lineis (includens border)
 * scala: factor scalae fontis (1 = 6x8, 2 = 12x16)
 * focused: VERUM si widget habet focus (mutat colorem border)
 */
vacuum
pagina_reddere_cum_margine(
    Piscina* piscina,
    TabulaPixelorum* tabula_pixelorum,
    Pagina* pagina,
    i32 x,
    i32 y,
    i32 latitudo,
    i32 altitudo,
    i32 scala,
    b32 focused);


/* ==================================================
 * Tractare Eventus
 * ================================================== */

/* Tractare eventum clavis
 *
 * Delegat ad vim_tractare_clavem_cum_tempore.
 *
 * pagina: pagina
 * eventus: eventus tractandus
 *
 * Reddit: VERUM si eventus tractatus, FALSUM si debet claudere (ESC in normal)
 */
b32
pagina_tractare_eventum(
    Pagina* pagina,
    constans Eventus* eventus);


/* ==================================================
 * Functiones Commodi (Convenience Functions)
 * ================================================== */

/* Ponere cursor ad positionem
 *
 * pagina: pagina
 * linea: linea (0 - TABULA_ALTITUDO-1)
 * columna: columna (0 - TABULA_LATITUDO-1)
 */
vacuum
pagina_ponere_cursor(
    Pagina* pagina,
    i32 linea,
    i32 columna);

/* Inserere chordam ad cursor (in modo inserere)
 *
 * Inserit characteres seriatim in tabulam ut typing.
 * Cursor movetur post singulum characterem.
 * '\n' incipit novam lineam.
 *
 * pagina: pagina
 * textus: textus inserendus (null-terminated)
 */
vacuum
pagina_inserere_textum(
    Pagina* pagina,
    constans character* textus);


/* ==================================================
 * Tag Detection
 * ================================================== */

/* Obtinere regio clickable ad coordinatas characterum
 *
 * Detectat tags ad coordinatas datae ($command, #link, <$block>).
 * Coordinatae sunt relative ad aream textus (non originem widget).
 *
 * pagina: pagina
 * linea, columna: coordinatae in grid
 * regio: exitus - regio detecta
 *
 * Reddit: VERUM si regio inventa, FALSUM si non
 */
b32
pagina_obtinere_regio_ad_punctum(
    constans Pagina* pagina,
    i32 linea,
    i32 columna,
    RegioClicca* regio);

#endif /* PAGINA_H */
