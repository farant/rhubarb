/* Probatio pro command system */
#include "latina.h"
#include "credo.h"
#include "pagina.h"
#include "tabula_characterum.h"
#include "vim.h"
#include "piscina.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Test: Tag Detection
 * ================================================== */

hic_manens vacuum
probans_detectare_tag_simplex(vacuum)
{
    Pagina pagina;
    RegioClicca regio;
    b32 invenit;

    /* Initiare pagina et inserere textum cum $date */
    pagina_initiare(&pagina, NIHIL);
    pagina_inserere_textum(&pagina, "$date");

    /* Click on 'd' (columna 1) - should find $date */
    invenit = pagina_obtinere_regio_ad_punctum(&pagina, ZEPHYRUM, I, &regio);
    CREDO_VERUM(invenit);

    si (invenit)
    {
        CREDO_AEQUALIS_I32(regio.initium_columna, ZEPHYRUM);  /* $ at 0 */
        CREDO_AEQUALIS_I32(regio.finis_columna, V);           /* after "date" */
        CREDO_AEQUALIS_S32(strcmp(regio.datum, "date"), ZEPHYRUM);
        CREDO_AEQUALIS_S32(strcmp(regio.genus, "command"), ZEPHYRUM);
    }
}

hic_manens vacuum
probans_detectare_tag_in_medio(vacuum)
{
    Pagina pagina;
    RegioClicca regio;
    b32 invenit;

    /* Initiare pagina et inserere textum cum $date in medio */
    pagina_initiare(&pagina, NIHIL);
    pagina_inserere_textum(&pagina, "Click here: $date to insert");

    /* Click on 'a' in date (columna 14) */
    invenit = pagina_obtinere_regio_ad_punctum(&pagina, ZEPHYRUM, XIV, &regio);
    CREDO_VERUM(invenit);

    si (invenit)
    {
        CREDO_AEQUALIS_S32(strcmp(regio.datum, "date"), ZEPHYRUM);
    }
}

hic_manens vacuum
probans_detectare_non_tag(vacuum)
{
    Pagina pagina;
    RegioClicca regio;
    b32 invenit;

    /* Initiare pagina et inserere textum sine tag */
    pagina_initiare(&pagina, NIHIL);
    pagina_inserere_textum(&pagina, "hello world");

    /* Click on 'e' in hello - should not find tag */
    invenit = pagina_obtinere_regio_ad_punctum(&pagina, ZEPHYRUM, I, &regio);
    CREDO_FALSUM(invenit);

    /* Click on space - should not find tag */
    invenit = pagina_obtinere_regio_ad_punctum(&pagina, ZEPHYRUM, V, &regio);
    CREDO_FALSUM(invenit);
}


/* ==================================================
 * Test: Cursor Positioning and Text Insertion
 * ================================================== */

hic_manens vacuum
probans_ponere_cursor(vacuum)
{
    Pagina pagina;

    printf("  sizeof(Pagina)=%lu, sizeof(TabulaCharacterum)=%lu, sizeof(VimStatus)=%lu\n",
           (insignatus longus)sizeof(Pagina),
           (insignatus longus)sizeof(TabulaCharacterum),
           (insignatus longus)sizeof(VimStatus));

    printf("  Pre-init garbage: linea=%d, columna=%d\n",
           pagina.vim.cursor_linea, pagina.vim.cursor_columna);

    pagina_initiare(&pagina, NIHIL);

    printf("  Post pagina_initiare: linea=%d, columna=%d\n",
           pagina.vim.cursor_linea, pagina.vim.cursor_columna);

    /* Ponere cursor ad (5, 10) */
    pagina_ponere_cursor(&pagina, V, X);

    printf("  Post ponere: linea=%d, columna=%d (expected %d, %d)\n",
           pagina.vim.cursor_linea, pagina.vim.cursor_columna, V, X);

    CREDO_AEQUALIS_I32(pagina.vim.cursor_linea, V);
    CREDO_AEQUALIS_I32(pagina.vim.cursor_columna, X);
}

hic_manens vacuum
probans_inserere_textum_ad_cursor(vacuum)
{
    Pagina pagina;

    pagina_initiare(&pagina, NIHIL);

    /* Ponere cursor et inserere textum */
    pagina_ponere_cursor(&pagina, ZEPHYRUM, ZEPHYRUM);
    pagina_inserere_textum(&pagina, "hello");

    /* Verificare contentum */
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'h');
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][I], (i32)'e');
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][II], (i32)'l');
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][III], (i32)'l');
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][IV], (i32)'o');

    /* Cursor debet esse post textum */
    CREDO_AEQUALIS_I32(pagina.vim.cursor_columna, V);
}

hic_manens vacuum
probans_inserere_post_tag(vacuum)
{
    Pagina pagina;
    RegioClicca regio;
    b32 invenit;

    pagina_initiare(&pagina, NIHIL);
    pagina_inserere_textum(&pagina, "$date");

    /* Simulare click et obtinere regio */
    invenit = pagina_obtinere_regio_ad_punctum(&pagina, ZEPHYRUM, II, &regio);
    CREDO_VERUM(invenit);

    si (invenit)
    {
        /* Ponere cursor ad finem tag et inserere */
        pagina_ponere_cursor(&pagina, regio.finis_linea, regio.finis_columna);
        pagina_inserere_textum(&pagina, " 01/15/2025");

        /* Verificare cursor position */
        CREDO_AEQUALIS_I32(pagina.vim.cursor_linea, ZEPHYRUM);

        /* Verificare spatium post $date */
        CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][V], (i32)' ');
        /* Verificare '0' */
        CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][VI], (i32)'0');
    }
}


/* ==================================================
 * Test: Grid State After Operations
 * ================================================== */

hic_manens vacuum
probans_grid_post_newline(vacuum)
{
    Pagina pagina;

    pagina_initiare(&pagina, NIHIL);
    printf("  Initial cursor: (%d, %d), mode=%d\n",
           pagina.vim.cursor_linea, pagina.vim.cursor_columna, pagina.vim.modo);
    pagina_inserere_textum(&pagina, "line1");
    printf("  After 'line1': cursor at (%d, %d)\n",
           pagina.vim.cursor_linea, pagina.vim.cursor_columna);
    printf("  Row 0 so far: '%c%c%c%c%c'\n",
           pagina.tabula.cellulae[ZEPHYRUM][ZEPHYRUM],
           pagina.tabula.cellulae[ZEPHYRUM][I],
           pagina.tabula.cellulae[ZEPHYRUM][II],
           pagina.tabula.cellulae[ZEPHYRUM][III],
           pagina.tabula.cellulae[ZEPHYRUM][IV]);
    printf("  Indentatio[0]=%d\n", pagina.tabula.indentatio[ZEPHYRUM]);

    pagina_inserere_textum(&pagina, "\n");
    printf("  After newline: cursor at (%d, %d)\n",
           pagina.vim.cursor_linea, pagina.vim.cursor_columna);
    printf("  Indentatio[1]=%d\n", pagina.tabula.indentatio[I]);

    pagina_inserere_textum(&pagina, "line2");

    printf("  After insert: cursor at (%d, %d)\n",
           pagina.vim.cursor_linea, pagina.vim.cursor_columna);
    printf("  Row 0: '%c%c%c%c%c'\n",
           pagina.tabula.cellulae[ZEPHYRUM][ZEPHYRUM],
           pagina.tabula.cellulae[ZEPHYRUM][I],
           pagina.tabula.cellulae[ZEPHYRUM][II],
           pagina.tabula.cellulae[ZEPHYRUM][III],
           pagina.tabula.cellulae[ZEPHYRUM][IV]);
    printf("  Row 1: '%c%c%c%c%c'\n",
           pagina.tabula.cellulae[I][ZEPHYRUM],
           pagina.tabula.cellulae[I][I],
           pagina.tabula.cellulae[I][II],
           pagina.tabula.cellulae[I][III],
           pagina.tabula.cellulae[I][IV]);

    /* line1 in linea 0 */
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'l');
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][IV], (i32)'1');

    /* line2 in linea 1 */
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[I][ZEPHYRUM], (i32)'l');
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[I][IV], (i32)'2');
}


hic_manens vacuum
probans_sticky_indentatio(vacuum)
{
    Pagina pagina;

    /* Probatio: "         hello\n\n" debet habere indentationem in linea 2 */
    pagina_initiare(&pagina, NIHIL);

    /* Inserere 9 spatia */
    pagina_inserere_textum(&pagina, "         ");
    CREDO_AEQUALIS_S32(pagina.tabula.indentatio[ZEPHYRUM], -I);  /* Nondum posita */

    /* Inserere "hello" */
    pagina_inserere_textum(&pagina, "hello");
    CREDO_AEQUALIS_S32(pagina.tabula.indentatio[ZEPHYRUM], IX);  /* Posita ad 9 */

    /* Prima newline - cursor debet esse ad indentationem, sed metadata non posita */
    pagina_inserere_textum(&pagina, "\n");
    CREDO_AEQUALIS_I32(pagina.vim.cursor_linea, I);
    CREDO_AEQUALIS_I32(pagina.vim.cursor_columna, IX);  /* Cursor ad indentationem */
    CREDO_AEQUALIS_S32(pagina.tabula.indentatio[I], -I);  /* Non posita - permittit deeper indent */

    /* Secunda newline sine contentum - cursor ETIAM ad indentationem (sticky!) */
    pagina_inserere_textum(&pagina, "\n");
    CREDO_AEQUALIS_I32(pagina.vim.cursor_linea, II);
    CREDO_AEQUALIS_I32(pagina.vim.cursor_columna, IX);  /* Cursor ad indentationem (STICKY!) */
    CREDO_AEQUALIS_S32(pagina.tabula.indentatio[II], -I);  /* Non posita */

    /* Verificare contentum in linea 0 */
    CREDO_AEQUALIS_I32((i32)pagina.tabula.cellulae[ZEPHYRUM][IX], (i32)'h');
}


/* ==================================================
 * Test: Tab Handling (Vim Layer)
 * ================================================== */

hic_manens vacuum
probans_tab_inserere(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: empty grid, insert mode */
    tabula_initiare(&tabula);
    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);

    /* Insert tab at column 0 */
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);

    /* Verify: '\t' at 0, TAB_CONTINUATIO at 1, cursor at 2 */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);
}

hic_manens vacuum
probans_tab_inserere_ante_textum(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "hello" in grid */
    tabula_ex_literis(&tabula, "hello");
    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);
    status = vim_ponere_cursor(status, ZEPHYRUM, ZEPHYRUM);

    /* Insert tab - should push "hello" right by 2 */
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);

    /* Verify layout: '\t', TAB_CONTINUATIO, 'h', 'e', 'l', 'l', 'o' */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][II], (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][VI], (i32)'o');
    CREDO_AEQUALIS_I32(status.cursor_columna, II);
}

hic_manens vacuum
probans_backspace_tab(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: tab + "hi" in grid, cursor after tab */
    tabula_initiare(&tabula);
    tabula.cellulae[ZEPHYRUM][ZEPHYRUM] = '\t';
    tabula.cellulae[ZEPHYRUM][I] = TAB_CONTINUATIO;
    tabula.cellulae[ZEPHYRUM][II] = 'h';
    tabula.cellulae[ZEPHYRUM][III] = 'i';

    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);
    status = vim_ponere_cursor(status, ZEPHYRUM, II);  /* After tab */

    /* Backspace should delete both tab cells */
    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);

    /* Verify: "hi" at start, cursor at 0 */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][I], (i32)'i');
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
}

hic_manens vacuum
probans_backspace_tab_continuatio(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "ab" + tab + "cd", cursor on TAB_CONTINUATIO */
    tabula_initiare(&tabula);
    tabula.cellulae[ZEPHYRUM][ZEPHYRUM] = 'a';
    tabula.cellulae[ZEPHYRUM][I] = 'b';
    tabula.cellulae[ZEPHYRUM][II] = '\t';
    tabula.cellulae[ZEPHYRUM][III] = TAB_CONTINUATIO;
    tabula.cellulae[ZEPHYRUM][IV] = 'c';
    tabula.cellulae[ZEPHYRUM][V] = 'd';

    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);
    status = vim_ponere_cursor(status, ZEPHYRUM, IV);  /* After tab, at 'c' */

    /* Backspace should delete both tab cells */
    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);

    /* Verify: "abcd", cursor at 2 */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][I], (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][II], (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][III], (i32)'d');
    CREDO_AEQUALIS_I32(status.cursor_columna, II);
}

hic_manens vacuum
probans_navigatio_h_skip_tab(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "a" + tab + "b", cursor at 'b' (column 4) */
    tabula_initiare(&tabula);
    tabula.cellulae[ZEPHYRUM][ZEPHYRUM] = 'a';
    tabula.cellulae[ZEPHYRUM][I] = '\t';
    tabula.cellulae[ZEPHYRUM][II] = TAB_CONTINUATIO;
    tabula.cellulae[ZEPHYRUM][III] = 'b';

    status = vim_initiare(&tabula);
    status = vim_ponere_cursor(status, ZEPHYRUM, III);  /* At 'b' */

    /* Press 'h' - should skip TAB_CONTINUATIO and land on '\t' */
    status = vim_tractare_clavem(status, 'h');

    /* Should land on '\t' at column 1, not TAB_CONTINUATIO at column 2 */
    CREDO_AEQUALIS_I32(status.cursor_columna, I);
}

hic_manens vacuum
probans_indentatio_cum_tab(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: tab + "hello" (indentation is tab at col 0-1, content at col 2) */
    tabula_initiare(&tabula);
    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);

    /* Insert tab then "hello" */
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);
    status = vim_tractare_clavem(status, 'h');
    status = vim_tractare_clavem(status, 'e');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'o');

    /* Verify: '\t', TAB_CONTINUATIO, 'h', 'e', 'l', 'l', 'o' */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][II], (i32)'h');

    /* indentatio should be at column 2 (first non-whitespace) */
    CREDO_AEQUALIS_S32(tabula.indentatio[ZEPHYRUM], II);

    /* Press Enter */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    /* Cursor should be at line 1, column 2 */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    /* New line should have TAB for indentation, not spaces! */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][I], (i32)TAB_CONTINUATIO);
}

hic_manens vacuum
probans_indentatio_mixta(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: tab + 2 spaces + "hello" */
    tabula_initiare(&tabula);
    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);

    /* Insert tab, space, space, then "hi" */
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);  /* col 0-1 */
    status = vim_tractare_clavem(status, ' ');             /* col 2 */
    status = vim_tractare_clavem(status, ' ');             /* col 3 */
    status = vim_tractare_clavem(status, 'h');             /* col 4 */
    status = vim_tractare_clavem(status, 'i');             /* col 5 */

    /* Verify layout */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][II], (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][III], (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][IV], (i32)'h');

    /* indentatio should be at column 4 */
    CREDO_AEQUALIS_S32(tabula.indentatio[ZEPHYRUM], IV);

    /* Press Enter */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    /* Cursor should be at line 1, column 4 */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, IV);

    /* New line should have same mixed indentation: tab + 2 spaces */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][II], (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][III], (i32)' ');
}

hic_manens vacuum
probans_indentatio_tabs_multiplex(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: 2 tabs + "hi" */
    tabula_initiare(&tabula);
    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);

    /* Insert tab, tab, then "hi" */
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);  /* col 0-1 */
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);  /* col 2-3 */
    status = vim_tractare_clavem(status, 'h');             /* col 4 */
    status = vim_tractare_clavem(status, 'i');             /* col 5 */

    /* Verify layout: '\t', TC, '\t', TC, 'h', 'i' */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][II], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][III], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[ZEPHYRUM][IV], (i32)'h');

    /* indentatio should be at column 4 */
    CREDO_AEQUALIS_S32(tabula.indentatio[ZEPHYRUM], IV);

    /* Press Enter */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    /* Cursor should be at line 1, column 4 */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, IV);

    /* New line should have 2 tabs */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][II], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][III], (i32)TAB_CONTINUATIO);
}

hic_manens vacuum
probans_indentatio_sticky_tabs_per_lineas_vacuas(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: tab + "hi" */
    tabula_initiare(&tabula);
    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);

    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);
    status = vim_tractare_clavem(status, 'h');
    status = vim_tractare_clavem(status, 'i');

    /* indentatio at column 2 */
    CREDO_AEQUALIS_S32(tabula.indentatio[ZEPHYRUM], II);

    /* Press Enter 3 times (create 3 blank lines) */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);
    CREDO_AEQUALIS_I32(status.cursor_linea, II);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);
    CREDO_AEQUALIS_I32(status.cursor_linea, III);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    /* Line 3 should still have tab indentation (sticky through blank lines) */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][I], (i32)TAB_CONTINUATIO);

    /* Type "world" on line 3 */
    status = vim_tractare_clavem(status, 'w');
    status = vim_tractare_clavem(status, 'o');
    status = vim_tractare_clavem(status, 'r');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'd');

    /* Verify line 3 content */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][II], (i32)'w');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][VI], (i32)'d');

    /* indentatio on line 3 should now be set to 2 */
    CREDO_AEQUALIS_S32(tabula.indentatio[III], II);
}

hic_manens vacuum
probans_indentatio_post_backspace(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Scenario:
     * 1. Type "\t\thello\n" - indentation 4 (2 tabs)
     * 2. Type "\t\t\t\tworld\n" - indentation 8 (4 tabs)
     * 3. On blank line, backspace twice to reduce to 2 tabs
     * 4. Press Enter - should have 2 tabs indentation, not 4
     */
    tabula_initiare(&tabula);
    status = vim_initiare(&tabula);
    status = vim_ponere_modum(status, MODO_VIM_INSERERE);

    /* Line 0: \t\thello */
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);
    status = vim_tractare_clavem(status, 'h');
    status = vim_tractare_clavem(status, 'e');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'o');
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    CREDO_AEQUALIS_S32(tabula.indentatio[ZEPHYRUM], IV);  /* indentation at 4 */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, IV);  /* cursor at indentation */

    /* Line 1: \t\t\t\tworld */
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);
    status = vim_tractare_clavem(status, VIM_CLAVIS_TAB);
    status = vim_tractare_clavem(status, 'w');
    status = vim_tractare_clavem(status, 'o');
    status = vim_tractare_clavem(status, 'r');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    CREDO_AEQUALIS_S32(tabula.indentatio[I], VIII);  /* indentation at 8 */
    CREDO_AEQUALIS_I32(status.cursor_linea, II);
    CREDO_AEQUALIS_I32(status.cursor_columna, VIII);  /* cursor at indentation (4 tabs copied) */

    /* Line 2 now has 4 tabs copied. Backspace twice to remove 2 tabs. */
    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);
    CREDO_AEQUALIS_I32(status.cursor_columna, VI);  /* cursor at 6 */

    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);
    CREDO_AEQUALIS_I32(status.cursor_columna, IV);  /* cursor at 4 */

    /* Line 2 should now have 2 tabs at beginning */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[II][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[II][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[II][II], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[II][III], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[II][IV], (i32)' ');  /* spaces after */

    /* Now press Enter - should copy 2 tabs (4 columns), NOT 4 tabs (8 columns) */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    CREDO_AEQUALIS_I32(status.cursor_linea, III);
    CREDO_AEQUALIS_I32(status.cursor_columna, IV);  /* cursor at 4, not 8! */

    /* Line 3 should have 2 tabs, not 4 */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][ZEPHYRUM], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][I], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][II], (i32)'\t');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][III], (i32)TAB_CONTINUATIO);
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[III][IV], (i32)' ');  /* space, not tab */
}

hic_manens vacuum
probans_navigatio_l_skip_tab(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "a" + tab + "b", cursor at '\t' (column 1) */
    tabula_initiare(&tabula);
    tabula.cellulae[ZEPHYRUM][ZEPHYRUM] = 'a';
    tabula.cellulae[ZEPHYRUM][I] = '\t';
    tabula.cellulae[ZEPHYRUM][II] = TAB_CONTINUATIO;
    tabula.cellulae[ZEPHYRUM][III] = 'b';

    status = vim_initiare(&tabula);
    status = vim_ponere_cursor(status, ZEPHYRUM, I);  /* At '\t' */

    /* Press 'l' - should skip TAB_CONTINUATIO and land on 'b' */
    status = vim_tractare_clavem(status, 'l');

    /* Should land on 'b' at column 3, not TAB_CONTINUATIO at column 2 */
    CREDO_AEQUALIS_I32(status.cursor_columna, III);
}


/* ==================================================
 * Main
 * ================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("commandi", M * M);
    si (!piscina)
    {
        printf("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    printf("--- Probans detectare tag simplex ---\n");
    probans_detectare_tag_simplex();

    printf("--- Probans detectare tag in medio ---\n");
    probans_detectare_tag_in_medio();

    printf("--- Probans detectare non tag ---\n");
    probans_detectare_non_tag();

    printf("--- Probans ponere cursor ---\n");
    probans_ponere_cursor();

    printf("--- Probans inserere textum ad cursor ---\n");
    probans_inserere_textum_ad_cursor();

    printf("--- Probans inserere post tag ---\n");
    probans_inserere_post_tag();

    printf("--- Probans grid post newline ---\n");
    probans_grid_post_newline();

    printf("--- Probans sticky indentatio ---\n");
    probans_sticky_indentatio();

    printf("--- Probans tab inserere ---\n");
    probans_tab_inserere();

    printf("--- Probans tab inserere ante textum ---\n");
    probans_tab_inserere_ante_textum();

    printf("--- Probans backspace tab ---\n");
    probans_backspace_tab();

    printf("--- Probans backspace tab continuatio ---\n");
    probans_backspace_tab_continuatio();

    printf("--- Probans navigatio h skip tab ---\n");
    probans_navigatio_h_skip_tab();

    printf("--- Probans navigatio l skip tab ---\n");
    probans_navigatio_l_skip_tab();

    printf("--- Probans indentatio cum tab ---\n");
    probans_indentatio_cum_tab();

    printf("--- Probans indentatio mixta ---\n");
    probans_indentatio_mixta();

    printf("--- Probans indentatio tabs multiplex ---\n");
    probans_indentatio_tabs_multiplex();

    printf("--- Probans indentatio sticky tabs per lineas vacuas ---\n");
    probans_indentatio_sticky_tabs_per_lineas_vacuas();

    printf("--- Probans indentatio post backspace ---\n");
    probans_indentatio_post_backspace();

    printf("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
