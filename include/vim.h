#ifndef VIM_H
#define VIM_H

#include "latina.h"
#include "tabula_characterum.h"

/* ==================================================
 * VIM - Editor Imperare Interpres
 *
 * Purus interpres pro vim key commandis.
 * Separat vim logica ex UI/rendering.
 *
 * PHILOSOPHIA:
 * - Status in, status ex (pura functio)
 * - Nulla dependentia ad fenestrare/reddere
 * - Facile probandus sine GUI
 *
 * EXEMPLUM:
 *   TabulaCharacterum tabula;
 *   VimStatus status;
 *
 *   tabula_ex_literis(&tabula, "hello world");
 *   status = vim_initiare(&tabula);
 *
 *   status = vim_tractare_clavem(status, 'x');
 *   // tabula nunc habet "ello world"
 *
 *   status = vim_tractare_clavem(status, 'A');
 *   // status.modo nunc est MODO_VIM_INSERERE
 *
 * ================================================== */


/* ==================================================
 * Constantae
 * ================================================== */

/* Claves speciales (valores negativi ad distinguendum a characteribus) */
#define VIM_CLAVIS_SURSUM      (-I)
#define VIM_CLAVIS_DEORSUM     (-II)
#define VIM_CLAVIS_SINISTRAM   (-III)
#define VIM_CLAVIS_DEXTRAM     (-IV)
#define VIM_CLAVIS_BACKSPACE   (-V)
#define VIM_CLAVIS_DELETE      (-VI)
#define VIM_CLAVIS_ENTER       (-VII)
#define VIM_CLAVIS_TAB         (-VIII)
#define VIM_CLAVIS_ESCAPE      (-IX)
#define VIM_CLAVIS_HOME        (-X)
#define VIM_CLAVIS_END         (-XI)


/* ==================================================
 * Typi
 * ================================================== */

/* Modi vim */
nomen enumeratio {
    MODO_VIM_NORMALIS = ZEPHYRUM,
    MODO_VIM_INSERERE,
    MODO_VIM_VISUALIS
} ModoVim;

/* Tipi visualis modi (pro futuro character-level selection) */
nomen enumeratio {
    MODO_VIM_VISUALIS_LINEA = ZEPHYRUM,
    MODO_VIM_VISUALIS_CHARACTER
} ModoVimVisualisTipo;

/* Status vim - omnia necessaria pro tractare clavem */
nomen structura {
    /* Tabula characterum (non possessa, referentia externa) */
    TabulaCharacterum* tabula;

    /* Cursor positio */
    i32 cursor_linea;
    i32 cursor_columna;

    /* Modus currens */
    ModoVim modo;

    /* Selectio (pro modo visuali) */
    s32 selectio_initium_linea;
    s32 selectio_initium_columna;

    /* Tipo visualis modi (linea vel character) */
    ModoVimVisualisTipo visualis_tipo;

    /* Status pro multi-key commands (dd, dG, d$, etc.) */
    character clavis_praecedens;

    /* Status pro fd escape sequence */
    b32 esperans_fd;
    f64 tempus_f;

    /* Flags */
    b32 debet_claudere;   /* ESC in normal mode */
    b32 mutatus;          /* Tabula mutata hac clave */
    b32 sine_auto_indent; /* Disable auto-indent (pro programmatic insertion) */
} VimStatus;


/* ==================================================
 * Initiatio
 * ================================================== */

/* Initiare vim status
 *
 * tabula: tabula characterum (non copiata, referentia servata)
 *
 * Reddit: status initiatus
 */
VimStatus
vim_initiare(
    TabulaCharacterum* tabula);


/* ==================================================
 * Tractare Claves
 * ================================================== */

/* Tractare clavem
 *
 * Pura functio: accipit status et clavem, reddit novum status.
 * Non mutat status directe - reddit novum.
 *
 * status: status currens
 * clavis: clavis tractanda (character vel VIM_CLAVIS_* constans)
 *
 * Reddit: novus status post tractare clavem
 */
VimStatus
vim_tractare_clavem(
    VimStatus status,
    s32 clavis);

/* Tractare clavem cum tempore (pro fd timing)
 *
 * Sicut vim_tractare_clavem sed cum tempore pro fd escape.
 *
 * status: status currens
 * clavis: clavis tractanda
 * tempus: tempus currens (secondae)
 *
 * Reddit: novus status
 */
VimStatus
vim_tractare_clavem_cum_tempore(
    VimStatus status,
    s32 clavis,
    f64 tempus);


/* ==================================================
 * Interrogationes
 * ================================================== */

/* Obtinere nomen modi
 *
 * modo: modus
 *
 * Reddit: nomen modi ("NORMAL", "INSERT", "VISUAL")
 */
constans character*
vim_nomen_modi(
    ModoVim modo);


/* ==================================================
 * Utilitas (pro testibus)
 * ================================================== */

/* Ponere cursor directe
 *
 * status: status
 * linea: nova linea (0 - TABULA_ALTITUDO-1)
 * columna: nova columna (0 - TABULA_LATITUDO-1)
 *
 * Reddit: status cum novo cursore
 */
VimStatus
vim_ponere_cursor(
    VimStatus status,
    i32 linea,
    i32 columna);

/* Ponere modum directe
 *
 * status: status
 * modo: novus modus
 *
 * Reddit: status cum novo modo
 */
VimStatus
vim_ponere_modum(
    VimStatus status,
    ModoVim modo);

#endif /* VIM_H */
