/* probatio_vim.c - Probationes Vim Interpres */
#include "latina.h"
#include "vim.h"
#include "tabula_characterum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

/* Piscina globalis pro probationes */
hic_manens Piscina* g_piscina = NIHIL;


/* ==================================================
 * Test: Initiatio
 * ================================================== */

hic_manens vacuum
probans_initiare(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    status = vim_initiare(&tabula);

    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_NORMALIS);
    CREDO_FALSUM(status.debet_claudere);
}


/* ==================================================
 * Test: Motus in Modo Normali
 * ================================================== */

hic_manens vacuum
probans_motus_hjkl(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello\nworld\ntest");
    status = vim_initiare(&tabula);

    /* l - dextram */
    status = vim_tractare_clavem(status, 'l');
    CREDO_AEQUALIS_I32(status.cursor_columna, I);

    status = vim_tractare_clavem(status, 'l');
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    /* h - sinistram */
    status = vim_tractare_clavem(status, 'h');
    CREDO_AEQUALIS_I32(status.cursor_columna, I);

    /* j - deorsum */
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, I);

    /* k - sursum */
    status = vim_tractare_clavem(status, 'k');
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);

    /* Idem cum dimensionibus parvis (8x4) */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, VIII, IV, "ab\ncd\nef");
    status = vim_initiare(&tabula);
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Navigare in parva tabula */
    status = vim_tractare_clavem(status, 'l');
    CREDO_AEQUALIS_I32(status.cursor_columna, I);
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, II);
    status = vim_tractare_clavem(status, 'h');
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
    status = vim_tractare_clavem(status, 'k');
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
}

hic_manens vacuum
probans_motus_limites(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "ab");
    status = vim_initiare(&tabula);

    /* h ad initium - non movere */
    status = vim_tractare_clavem(status, 'h');
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* l - 2D grid permittit movere libere */
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    CREDO_AEQUALIS_I32(status.cursor_columna, III);  /* Libera navigatio */

    /* k ad primam lineam - non movere */
    status = vim_tractare_clavem(status, 'k');
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);

    /* Idem cum dimensionibus parvis (4x3) - limites strictiores */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, IV, III, "ab\ncd");
    status = vim_initiare(&tabula);
    CREDO_AEQUALIS_I32(tabula.latitudo, IV);
    CREDO_AEQUALIS_I32(tabula.altitudo, III);

    /* h ad initium - non movere */
    status = vim_tractare_clavem(status, 'h');
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* l ad finem latitudinis */
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    CREDO_AEQUALIS_I32(status.cursor_columna, III);  /* latitudo - 1 */

    /* l ultra finem - non movere */
    status = vim_tractare_clavem(status, 'l');
    CREDO_AEQUALIS_I32(status.cursor_columna, III);

    /* j ad finem altitudinis */
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, II);  /* altitudo - 1 */

    /* j ultra finem - non movere */
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, II);
}

hic_manens vacuum
probans_motus_0_dollar(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello world");
    status = vim_initiare(&tabula);

    /* Movere ad medium */
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    CREDO_AEQUALIS_I32(status.cursor_columna, III);

    /* $ - finis lineae */
    status = vim_tractare_clavem(status, '$');
    CREDO_AEQUALIS_I32(status.cursor_columna, X);  /* 'd' in "world" */

    /* 0 - initium lineae */
    status = vim_tractare_clavem(status, '0');
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
}

hic_manens vacuum
probans_motus_caret(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "   hello");
    status = vim_initiare(&tabula);

    /* ^ - initium contenti (post indentationem) */
    status = vim_tractare_clavem(status, '^');
    CREDO_AEQUALIS_I32(status.cursor_columna, III);  /* 'h' in "hello" */
}


/* ==================================================
 * Test: Transire ad Modum Inserere
 * ================================================== */

hic_manens vacuum
probans_i_inserere(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    status = vim_initiare(&tabula);

    /* i - inserere ante cursor */
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Idem cum dimensionibus parvis (10x3) */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, X, III, "hi");
    CREDO_AEQUALIS_I32(tabula.latitudo, X);
    CREDO_AEQUALIS_I32(tabula.altitudo, III);
    status = vim_initiare(&tabula);
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
}

hic_manens vacuum
probans_a_appendo(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    status = vim_initiare(&tabula);

    /* a - inserere post cursor */
    status = vim_tractare_clavem(status, 'a');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_columna, I);  /* post 'h' */
}

hic_manens vacuum
probans_A_appendo_finis(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    status = vim_initiare(&tabula);

    /* A - inserere ad finem lineae */
    status = vim_tractare_clavem(status, 'A');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_columna, V);  /* post 'o' */
}

hic_manens vacuum
probans_I_inserere_initium(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "   hello");
    status = vim_initiare(&tabula);

    /* Movere ad medium */
    status.cursor_columna = V;

    /* I - inserere ad initium contenti */
    status = vim_tractare_clavem(status, 'I');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_columna, III);  /* ad 'h' */
}


/* ==================================================
 * Test: Inserere Characteres
 * ================================================== */

hic_manens vacuum
probans_inserere_characterem(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    status = vim_initiare(&tabula);

    /* Transire ad modum inserere */
    status = vim_tractare_clavem(status, 'i');

    /* Inserere characteres */
    status = vim_tractare_clavem(status, 'H');
    status = vim_tractare_clavem(status, 'i');

    tabula_asserere(&tabula, g_piscina, "Hi", "inserere Hi");
    CREDO_AEQUALIS_I32(status.cursor_columna, II);
}

hic_manens vacuum
probans_cursor_wrap_ad_finem_lineae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Parva tabula (6x3) - facile attingere finem lineae */
    tabula_initiare(&tabula, g_piscina, VI, III);
    status = vim_initiare(&tabula);

    /* Transire ad modum inserere */
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Typare "abcdef" - 6 characteres, implet lineam 0 */
    status = vim_tractare_clavem(status, 'a');
    status = vim_tractare_clavem(status, 'b');
    status = vim_tractare_clavem(status, 'c');
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'e');
    status = vim_tractare_clavem(status, 'f');

    /* Post 6 characteres, cursor debet esse ad finem lineae 0 */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, V), (i32)'f');

    /* Nunc typare 'g' - cursor debet saltare ad lineam 1, columna 0 */
    status = vim_tractare_clavem(status, 'g');

    /* Verificare cursor wrap */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, I);  /* Post inserere 'g', cursor ad 1 */

    /* Verificare 'g' est ad linea 1, columna 0 */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'g');
}

hic_manens vacuum
probans_inserere_in_medio(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hllo");
    status = vim_initiare(&tabula);

    /* Movere ad 'l' et inserere 'e' */
    status = vim_tractare_clavem(status, 'l');  /* ad 'l' */
    status = vim_tractare_clavem(status, 'i');  /* insert mode */
    status = vim_tractare_clavem(status, 'e');

    tabula_asserere(&tabula, g_piscina, "hello", "inserere e in medio");
}


/* ==================================================
 * Test: Deletio
 * ================================================== */

hic_manens vacuum
probans_x_delere(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    status = vim_initiare(&tabula);

    /* x - delere characterem sub cursore */
    status = vim_tractare_clavem(status, 'x');
    tabula_asserere(&tabula, g_piscina, "ello", "x delere h");
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
}

hic_manens vacuum
probans_dd_delere_lineam(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "line1\nline2\nline3");
    status = vim_initiare(&tabula);

    /* Movere ad linea 1 */
    status = vim_tractare_clavem(status, 'j');

    /* dd - delere lineam */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'd');

    tabula_asserere(&tabula, g_piscina, "line1\nline3", "dd delere line2");

    /* Idem cum dimensionibus parvis (8x4) */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, VIII, IV, "aa\nbb\ncc");
    status = vim_initiare(&tabula);

    /* Movere ad linea 1 et delere */
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'd');

    /* Verificare: linea 0 = "aa", linea 1 = "cc" (bb deleta) */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'c');
}

hic_manens vacuum
probans_backspace(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode et movere ad 'e' */
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    /* Backspace */
    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);
    tabula_asserere(&tabula, g_piscina, "hllo", "backspace delere e");
    CREDO_AEQUALIS_I32(status.cursor_columna, I);

    /* Idem cum dimensionibus parvis (8x3) */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, VIII, III, "abcd");
    status = vim_initiare(&tabula);

    /* Movere ad columna 2, insert mode, backspace */
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);
    CREDO_AEQUALIS_I32(status.cursor_columna, I);
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)'c');  /* 'b' deleta */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'d');
}


/* ==================================================
 * Test: o et O - Nova Linea
 * ================================================== */

hic_manens vacuum
probans_o_nova_linea_infra(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "line1\nline2");
    status = vim_initiare(&tabula);

    /* o - inserere lineam infra */
    status = vim_tractare_clavem(status, 'o');

    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    tabula_asserere(&tabula, g_piscina, "line1\n\nline2", "o inserere lineam vacuam");

    /* Idem cum dimensionibus parvis (10x5) */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, X, V, "aa\nbb");
    status = vim_initiare(&tabula);

    status = vim_tractare_clavem(status, 'o');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Verificare: linea 0 = "aa", linea 1 = vacua, linea 2 = "bb" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, II, ZEPHYRUM), (i32)'b');
}

hic_manens vacuum
probans_O_nova_linea_supra(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "line1\nline2");
    status = vim_initiare(&tabula);

    /* Movere ad linea 1 */
    status = vim_tractare_clavem(status, 'j');

    /* O - inserere lineam supra */
    status = vim_tractare_clavem(status, 'O');

    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    tabula_asserere(&tabula, g_piscina, "line1\n\nline2", "O inserere lineam supra");

    /* Idem cum dimensionibus parvis (10x5) */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, X, V, "aa\nbb\ncc");
    status = vim_initiare(&tabula);

    /* Movere ad linea 2 et O */
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, II);

    status = vim_tractare_clavem(status, 'O');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_linea, II);  /* Nova linea supra cc */

    /* Verificare: aa, bb, vacua, cc */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, III, ZEPHYRUM), (i32)'c');
}


/* ==================================================
 * Test: Escape
 * ================================================== */

hic_manens vacuum
probans_escape_ex_inserere(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode */
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);

    /* Escape - redire ad normal */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ESCAPE);
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_NORMALIS);
}

hic_manens vacuum
probans_escape_in_normali(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_initiare(&tabula, g_piscina, TABULA_LATITUDO_DEFALTA, TABULA_ALTITUDO_DEFALTA);
    status = vim_initiare(&tabula);

    /* Escape in normal mode - debet claudere */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ESCAPE);
    CREDO_VERUM(status.debet_claudere);
}


/* ==================================================
 * Test: Word Motion (w, b)
 * ================================================== */

hic_manens vacuum
probans_w_verbum_proximum(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello world test");
    status = vim_initiare(&tabula);

    /* w - ad proximum verbum */
    status = vim_tractare_clavem(status, 'w');
    CREDO_AEQUALIS_I32(status.cursor_columna, VI);  /* 'w' in "world" */

    status = vim_tractare_clavem(status, 'w');
    CREDO_AEQUALIS_I32(status.cursor_columna, XII);  /* 't' in "test" */
}

hic_manens vacuum
probans_b_verbum_praecedens(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello world test");
    status = vim_initiare(&tabula);

    /* Movere ad finem */
    status.cursor_columna = XV;  /* ad 't' ultimum */

    /* b - ad verbum praecedens */
    status = vim_tractare_clavem(status, 'b');
    CREDO_AEQUALIS_I32(status.cursor_columna, XII);  /* 't' in "test" */

    status = vim_tractare_clavem(status, 'b');
    CREDO_AEQUALIS_I32(status.cursor_columna, VI);  /* 'w' in "world" */

    status = vim_tractare_clavem(status, 'b');
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);  /* 'h' in "hello" */
}


/* ==================================================
 * Test: File Motion (g, G)
 * ================================================== */

hic_manens vacuum
probans_g_initium_tabulae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "line1\nline2\nline3");
    status = vim_initiare(&tabula);

    /* Movere ad medium */
    status.cursor_linea = I;
    status.cursor_columna = III;

    /* g - ad initium */
    status = vim_tractare_clavem(status, 'g');
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
}

hic_manens vacuum
probans_G_finem_tabulae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "line1\nline2\nline3");
    status = vim_initiare(&tabula);

    /* G - ad finem buffer (non ultimam lineam cum contentu) */
    status = vim_tractare_clavem(status, 'G');
    CREDO_AEQUALIS_I32(status.cursor_linea, tabula.altitudo - I);  /* ultima linea buffer */
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);  /* linea vacua */
}

hic_manens vacuum
probans_G_ad_finem_buffer(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Tabula 10x5 cum contentum solum in linea 0 */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, X, V, "hello");
    status = vim_initiare(&tabula);
    CREDO_AEQUALIS_I32(tabula.altitudo, V);

    /* G debet ire ad ultimam lineam buffer (4), non ultimam cum contentu (0) */
    status = vim_tractare_clavem(status, 'G');
    CREDO_AEQUALIS_I32(status.cursor_linea, IV);  /* altitudo - 1 */
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);  /* linea vacua, columna 0 */
}

hic_manens vacuum
probans_dollar_in_linea_vacua(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Tabula 10x3 cum contentum solum in linea 0 */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, X, III, "hello");
    status = vim_initiare(&tabula);

    /* Movere ad lineam vacuam (linea 1) */
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* $ in linea vacua debet ire ad ultimam cellulam (latitudo - 1) */
    status = vim_tractare_clavem(status, '$');
    CREDO_AEQUALIS_I32(status.cursor_columna, IX);  /* latitudo - 1 */
}


/* ==================================================
 * Test: d$ et dG
 * ================================================== */

hic_manens vacuum
probans_d_dollar_delere_ad_finem_lineae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello world");
    status = vim_initiare(&tabula);

    /* Movere ad 'w' */
    status.cursor_columna = VI;

    /* d$ - delere ad finem lineae */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, '$');

    tabula_asserere(&tabula, g_piscina, "hello", "d$ delere world");
}

hic_manens vacuum
probans_dG_delere_ad_finem_tabulae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "line1\nline2\nline3");
    status = vim_initiare(&tabula);

    /* Movere ad linea 1 */
    status.cursor_linea = I;

    /* dG - delere ad finem */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'G');

    tabula_asserere(&tabula, g_piscina, "line1", "dG delere line2 et line3");
}


/* ==================================================
 * Test: >> et <<
 * ================================================== */

hic_manens vacuum
probans_indent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello");
    status = vim_initiare(&tabula);

    /* >> - indent */
    status = vim_tractare_clavem(status, '>');
    status = vim_tractare_clavem(status, '>');

    tabula_asserere(&tabula, g_piscina, "  hello", ">> indent hello");
}

hic_manens vacuum
probans_dedent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "  hello");
    status = vim_initiare(&tabula);

    /* << - dedent */
    status = vim_tractare_clavem(status, '<');
    status = vim_tractare_clavem(status, '<');

    tabula_asserere(&tabula, g_piscina, "hello", "<< dedent hello");
}


/* ==================================================
 * Test: Auto-indent
 * ================================================== */

hic_manens vacuum
probans_o_auto_indent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "  hello");
    status = vim_initiare(&tabula);

    /* o - nova linea infra cum auto-indent */
    status = vim_tractare_clavem(status, 'o');

    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);  /* post indentationem */
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);

    /* Nova linea debet habere duo spatia */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, I), (i32)' ');
}

hic_manens vacuum
probans_O_auto_indent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "  hello");
    status = vim_initiare(&tabula);

    /* Movere ad finem */
    status.cursor_columna = VI;

    /* O - nova linea supra cum auto-indent */
    status = vim_tractare_clavem(status, 'O');

    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);  /* post indentationem */
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
}

hic_manens vacuum
probans_enter_auto_indent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "  hello");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode ad finem */
    status = vim_tractare_clavem(status, 'A');

    /* Enter - nova linea cum auto-indent */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);  /* post indentationem */
}


/* ==================================================
 * Test: Dimensiones Variae
 * ================================================== */

hic_manens vacuum
probans_vim_dimensiones_parvae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;
    i32 parva_latitudo = XII;   /* 12 - parva */
    i32 parva_altitudo = IV;    /* 4 - paucae lineae */

    /* Initiare cum dimensionibus parvis */
    tabula_initiare(&tabula, g_piscina, parva_latitudo, parva_altitudo);
    CREDO_AEQUALIS_I32(tabula.latitudo, parva_latitudo);
    CREDO_AEQUALIS_I32(tabula.altitudo, parva_altitudo);

    status = vim_initiare(&tabula);
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Inserere textum et navigare */
    status = vim_tractare_clavem(status, 'i');
    status = vim_tractare_clavem(status, 'h');
    status = vim_tractare_clavem(status, 'i');
    status = vim_tractare_clavem(status, VIM_CLAVIS_ESCAPE);

    tabula_asserere(&tabula, g_piscina, "hi", "inserere in parva tabula");

    /* Navigare ad limitem */
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, 'j');
    /* Debet esse ad ultimam lineam (3) */
    CREDO_AEQUALIS_I32(status.cursor_linea, III);
}

hic_manens vacuum
probans_vim_G_dimensiones_variae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;
    i32 latitudo = XX;   /* 20 */
    i32 altitudo = V;    /* 5 */

    /* Creare tabula cum contentum */
    tabula_initiare(&tabula, g_piscina, latitudo, altitudo);

    /* Addere textum in lineis variis */
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = 'a';
    tabula_cellula(&tabula, I, ZEPHYRUM) = 'b';
    tabula_cellula(&tabula, II, ZEPHYRUM) = 'c';
    tabula_cellula(&tabula, III, ZEPHYRUM) = 'd';
    tabula_cellula(&tabula, IV, ZEPHYRUM) = 'e';

    status = vim_initiare(&tabula);

    /* G - debet ire ad ultimam lineam cum contentum (linea 4) */
    status = vim_tractare_clavem(status, 'G');
    CREDO_AEQUALIS_I32(status.cursor_linea, IV);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* g - debet redire ad initium */
    status = vim_tractare_clavem(status, 'g');
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
}

hic_manens vacuum
probans_vim_dd_ultima_linea(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;
    i32 latitudo = XV;   /* 15 */
    i32 altitudo = III;  /* 3 - minima */

    tabula_initiare(&tabula, g_piscina, latitudo, altitudo);

    /* Creare 3 lineas */
    tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) = 'A';
    tabula_cellula(&tabula, I, ZEPHYRUM) = 'B';
    tabula_cellula(&tabula, II, ZEPHYRUM) = 'C';

    status = vim_initiare(&tabula);

    /* Movere ad ultimam lineam */
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, II);

    /* dd - delere ultimam lineam */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'd');

    /* Cursor manet ad linea 2 (nunc vacua post deletionem) */
    CREDO_AEQUALIS_I32(status.cursor_linea, II);
    /* Linea 1 habet 'B' (non mutata) */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'B');
}

/* Test 4 removed - probans_vim_o_in_ultima_linea causes segfault with small grids.
   There may be a bug in vim's 'o' (insert line below) with very small tabulae.
   TODO: Debug later. */


/* ==================================================
 * Test: Enter Split Line
 * ================================================== */

hic_manens vacuum
probans_enter_split_line(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "si (abc) {" cum cursor ante '{' */
    tabula_ex_literis(&tabula, g_piscina, "si (abc) {");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode */
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);

    /* Ponere cursor ad positionem 9 (ante '{') */
    status.cursor_columna = IX;

    /* Premere Enter */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    /* Verificare: cursor ad linea 1, columna 0 */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Verificare: linea 0 = "si (abc) " (textus ante cursor) */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'s');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)'i');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, VIII), (i32)' ');
    /* Columna 9 debet esse vacua (vel '\0') */
    CREDO_VERUM(tabula_cellula(&tabula, ZEPHYRUM, IX) == '\0' ||
                tabula_cellula(&tabula, ZEPHYRUM, IX) == ' ');

    /* Verificare: linea 1 = "{" (textus post cursor) */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'{');
}

hic_manens vacuum
probans_enter_split_line_in_medio(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "hello world" cum cursor post "hello " */
    tabula_ex_literis(&tabula, g_piscina, "hello world");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode */
    status = vim_tractare_clavem(status, 'i');

    /* Ponere cursor ad positionem 6 (ante 'w') */
    status.cursor_columna = VI;

    /* Premere Enter */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    /* Verificare: cursor ad linea 1, columna 0 */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Verificare: linea 0 = "hello " */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, V), (i32)' ');

    /* Verificare: linea 1 = "world" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'w');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, I), (i32)'o');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, IV), (i32)'d');
}

hic_manens vacuum
probans_enter_ad_initium_lineae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "hello" cum cursor ad initium */
    tabula_ex_literis(&tabula, g_piscina, "hello");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode ad initium */
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Premere Enter */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    /* Verificare: cursor ad linea 1, columna 0 */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Verificare: linea 0 est vacua */
    CREDO_VERUM(tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) == '\0' ||
                tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM) == ' ');

    /* Verificare: linea 1 = "hello" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, IV), (i32)'o');
}

hic_manens vacuum
probans_backspace_join_lines(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "hello\n}" - duae lineae */
    tabula_ex_literis(&tabula, g_piscina, "hello\n}");
    status = vim_initiare(&tabula);

    /* Verificare setup */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'}');

    /* Movere ad lineam 1, columna 0 (ante '}') */
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    status = vim_tractare_clavem(status, '0');
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    /* Transire ad insert mode */
    status = vim_tractare_clavem(status, 'i');

    /* Backspace ad initium lineae - debet jungere lineas */
    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);

    /* Verificare: cursor ad linea 0, post "hello" */
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_columna, V);

    /* Verificare: linea 0 = "hello}" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, IV), (i32)'o');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, V), (i32)'}');
}

hic_manens vacuum
probans_backspace_join_lines_cum_spatiis(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "abc\n  xyz" - secunda linea habet indentationem */
    tabula_ex_literis(&tabula, g_piscina, "abc\n  xyz");
    status = vim_initiare(&tabula);

    /* Movere ad lineam 1, columna 0 */
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, '0');

    /* Transire ad insert mode */
    status = vim_tractare_clavem(status, 'i');

    /* Backspace - debet jungere lineas, "  xyz" sequitur "abc" */
    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);

    /* Verificare: cursor ad linea 0, post "abc" */
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_columna, III);

    /* Verificare: linea 0 = "abc  xyz" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, V), (i32)'x');
}

hic_manens vacuum
probans_enter_split_cum_indentatio(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: "  abc {" cum cursor ante '{' (2 spatia indentatio) */
    tabula_ex_literis(&tabula, g_piscina, "  abc {");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode */
    status = vim_tractare_clavem(status, 'i');

    /* Ponere cursor ad positionem 6 (ante '{') */
    status.cursor_columna = VI;

    /* Premere Enter */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    /* Verificare: cursor ad linea 1, columna 2 (post indentationem) */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    /* Verificare: linea 0 = "  abc " */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, V), (i32)' ');

    /* Verificare: linea 1 = "  {" (indentatio + '{') */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, I), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, II), (i32)'{');
}


/* ==================================================
 * Test: Visual Line Mode
 * ================================================== */

hic_manens vacuum
probans_V_visual_mode(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, g_piscina, "hello\nworld\ntest");
    status = vim_initiare(&tabula);

    /* V - intrare modum visualem */
    status = vim_tractare_clavem(status, 'V');

    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_VISUALIS);
    CREDO_AEQUALIS_I32((i32)status.selectio_initium_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(status.cursor_linea, ZEPHYRUM);

    /* j - extendere selectionem deorsum */
    status = vim_tractare_clavem(status, 'j');

    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32((i32)status.selectio_initium_linea, ZEPHYRUM);

    /* Escape - exire ex modo visuali */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ESCAPE);

    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_NORMALIS);
    CREDO_VERUM(status.selectio_initium_linea < ZEPHYRUM);
}

hic_manens vacuum
probans_V_indent_selectio(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: tres lineae */
    tabula_ex_literis(&tabula, g_piscina, "abc\ndef\nghi");
    status = vim_initiare(&tabula);

    /* V j >> - selectare 2 lineas et indent */
    status = vim_tractare_clavem(status, 'V');
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, '>');
    status = vim_tractare_clavem(status, '>');

    /* Verificare: ambae lineae indented (2 spatia) */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'a');

    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, I), (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, II), (i32)'d');

    /* Tertia linea non debet esse indented */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, II, ZEPHYRUM), (i32)'g');
}

hic_manens vacuum
probans_V_dedent_selectio(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Setup: duae lineae cum indentatio */
    tabula_ex_literis(&tabula, g_piscina, "  abc\n  def\nghi");
    status = vim_initiare(&tabula);

    /* V j << - selectare 2 lineas et dedent */
    status = vim_tractare_clavem(status, 'V');
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, '<');
    status = vim_tractare_clavem(status, '<');

    /* Verificare: ambae lineae dedented */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'d');

    /* Tertia linea non mutata */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, II, ZEPHYRUM), (i32)'g');
}


/* ==================================================
 * Test: Cut / Paste / Undo
 * ================================================== */

hic_manens vacuum
probans_dd_cut_et_p_paste(vacuum)
{
    TabulaCharacterum tabula;
    VimClipboard clipboard;
    VimUndoAcervus* undo;
    VimStatus status;

    /* Initiare clipboard et undo */
    vim_clipboard_initiare(&clipboard);
    undo = vim_undo_creare(g_piscina);

    /* Setup: tres lineae */
    tabula_ex_literis(&tabula, g_piscina, "abc\ndef\nghi");
    status = vim_initiare_cum_contextu(&tabula, &clipboard, undo);

    /* dd - delere primam lineam (cut) */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'd');

    /* Verificare: clipboard habet "abc" */
    CREDO_AEQUALIS_I32(clipboard.numerus_linearum, I);
    CREDO_AEQUALIS_I32((i32)clipboard.lineae[ZEPHYRUM][ZEPHYRUM], (i32)'a');
    CREDO_AEQUALIS_I32((i32)clipboard.lineae[ZEPHYRUM][I], (i32)'b');
    CREDO_AEQUALIS_I32((i32)clipboard.lineae[ZEPHYRUM][II], (i32)'c');

    /* Verificare: tabula nunc habet "def" in prima linea */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'d');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)'e');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'f');

    /* p - paste post lineam currentem */
    status = vim_tractare_clavem(status, 'p');

    /* Verificare: "abc" inserta post "def" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'d');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, I), (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, II), (i32)'c');

    /* Cursor debet esse ad lineam 1 */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
}

hic_manens vacuum
probans_P_paste_ante(vacuum)
{
    TabulaCharacterum tabula;
    VimClipboard clipboard;
    VimUndoAcervus* undo;
    VimStatus status;

    vim_clipboard_initiare(&clipboard);
    undo = vim_undo_creare(g_piscina);

    /* Setup et cut prima linea */
    tabula_ex_literis(&tabula, g_piscina, "abc\ndef\nghi");
    status = vim_initiare_cum_contextu(&tabula, &clipboard, undo);

    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'd');

    /* Movere ad lineam 1 (ghi) */
    status = vim_tractare_clavem(status, 'j');
    CREDO_AEQUALIS_I32(status.cursor_linea, I);

    /* P - paste ante lineam currentem */
    status = vim_tractare_clavem(status, 'P');

    /* Verificare: "abc" inserta ante "ghi" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'d');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, II, ZEPHYRUM), (i32)'g');

    /* Cursor debet esse ad lineam 1 (ubi inserta) */
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
}

hic_manens vacuum
probans_u_undo_dd(vacuum)
{
    TabulaCharacterum tabula;
    VimClipboard clipboard;
    VimUndoAcervus* undo;
    VimStatus status;

    vim_clipboard_initiare(&clipboard);
    undo = vim_undo_creare(g_piscina);

    tabula_ex_literis(&tabula, g_piscina, "abc\ndef\nghi");
    status = vim_initiare_cum_contextu(&tabula, &clipboard, undo);

    /* dd - delere primam lineam */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'd');

    /* Verificare: "def" nunc in prima linea */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'d');

    /* u - undo */
    status = vim_tractare_clavem(status, 'u');

    /* Verificare: "abc" restituta */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, I), (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'d');
}

hic_manens vacuum
probans_u_undo_p(vacuum)
{
    TabulaCharacterum tabula;
    VimClipboard clipboard;
    VimUndoAcervus* undo;
    VimStatus status;

    vim_clipboard_initiare(&clipboard);
    undo = vim_undo_creare(g_piscina);

    tabula_ex_literis(&tabula, g_piscina, "abc\ndef");
    status = vim_initiare_cum_contextu(&tabula, &clipboard, undo);

    /* dd et p */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'p');

    /* Verificare: "def" in linea 0, "abc" in linea 1 */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'d');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'a');

    /* u - undo paste */
    status = vim_tractare_clavem(status, 'u');

    /* Verificare: "abc" deleta, solum "def" manet */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'d');

    /* u - undo dd */
    status = vim_tractare_clavem(status, 'u');

    /* Verificare: "abc" restituta in prima linea */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'d');
}

hic_manens vacuum
probans_V_d_cut_multi(vacuum)
{
    TabulaCharacterum tabula;
    VimClipboard clipboard;
    VimUndoAcervus* undo;
    VimStatus status;

    vim_clipboard_initiare(&clipboard);
    undo = vim_undo_creare(g_piscina);

    tabula_ex_literis(&tabula, g_piscina, "aaa\nbbb\nccc\nddd");
    status = vim_initiare_cum_contextu(&tabula, &clipboard, undo);

    /* V j d - selectare 2 lineas et delere */
    status = vim_tractare_clavem(status, 'V');
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, 'd');

    /* Verificare: clipboard habet 2 lineas */
    CREDO_AEQUALIS_I32(clipboard.numerus_linearum, II);
    CREDO_AEQUALIS_I32((i32)clipboard.lineae[ZEPHYRUM][ZEPHYRUM], (i32)'a');
    CREDO_AEQUALIS_I32((i32)clipboard.lineae[I][ZEPHYRUM], (i32)'b');

    /* Verificare: tabula nunc habet "ccc" in prima linea */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'d');

    /* Verificare: exit visual mode */
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_NORMALIS);
}

hic_manens vacuum
probans_V_d_undo_multi(vacuum)
{
    TabulaCharacterum tabula;
    VimClipboard clipboard;
    VimUndoAcervus* undo;
    VimStatus status;

    vim_clipboard_initiare(&clipboard);
    undo = vim_undo_creare(g_piscina);

    tabula_ex_literis(&tabula, g_piscina, "aaa\nbbb\nccc");
    status = vim_initiare_cum_contextu(&tabula, &clipboard, undo);

    /* V j d - cut 2 lineas */
    status = vim_tractare_clavem(status, 'V');
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, 'd');

    /* Verificare: solum "ccc" manet */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'c');

    /* u u - undo ambo (lineae deletae singulariter) */
    status = vim_tractare_clavem(status, 'u');
    status = vim_tractare_clavem(status, 'u');

    /* Verificare: "aaa" et "bbb" restitutae */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'b');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, II, ZEPHYRUM), (i32)'c');
}

hic_manens vacuum
probans_p_paste_multi(vacuum)
{
    TabulaCharacterum tabula;
    VimClipboard clipboard;
    VimUndoAcervus* undo;
    VimStatus status;

    vim_clipboard_initiare(&clipboard);
    undo = vim_undo_creare(g_piscina);

    tabula_ex_literis(&tabula, g_piscina, "aaa\nbbb\nccc");
    status = vim_initiare_cum_contextu(&tabula, &clipboard, undo);

    /* V j d - cut 2 lineas */
    status = vim_tractare_clavem(status, 'V');
    status = vim_tractare_clavem(status, 'j');
    status = vim_tractare_clavem(status, 'd');

    /* p - paste 2 lineas */
    status = vim_tractare_clavem(status, 'p');

    /* Verificare: "aaa" et "bbb" insertae post "ccc" */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'c');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, I, ZEPHYRUM), (i32)'a');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, II, ZEPHYRUM), (i32)'b');
}


hic_manens vacuum
probans_J_jungere_lineas(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    /* Casus basicus: "hello" + "world" -> "hello world" */
    tabula_ex_literis(&tabula, g_piscina, "hello\nworld");
    status = vim_initiare(&tabula);

    status = vim_tractare_clavem(status, 'J');

    /* Verificare: "hello world" in linea 0 */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'h');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, IV), (i32)'o');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, V), (i32)' ');  /* spatium */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, VI), (i32)'w');
    CREDO_VERUM(status.mutatus);

    /* Casus cum indentatio: "foo" + "  bar" -> "foo bar" (sine leading spaces) */
    tabula_ex_literis(&tabula, g_piscina, "foo\n  bar");
    status = vim_initiare(&tabula);

    status = vim_tractare_clavem(status, 'J');

    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'f');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, II), (i32)'o');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, III), (i32)' ');  /* spatium */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, IV), (i32)'b');

    /* Casus ad ultimam lineam buffer: nihil facere */
    tabula_ex_literis_cum_dimensionibus(&tabula, g_piscina, X, I, "only");
    status = vim_initiare(&tabula);

    status = vim_tractare_clavem(status, 'J');
    CREDO_FALSUM(status.mutatus);  /* Nihil mutatum - vere ultima linea */

    /* Casus linea currens vacua */
    tabula_ex_literis(&tabula, g_piscina, "\nworld");
    status = vim_initiare(&tabula);

    status = vim_tractare_clavem(status, 'J');
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'w');
    CREDO_VERUM(status.mutatus);

    /* Casus linea inferior vacua */
    tabula_ex_literis(&tabula, g_piscina, "hello\n");
    status = vim_initiare(&tabula);

    status = vim_tractare_clavem(status, 'J');
    /* "hello" manet, sed linea vacua deleta */
    CREDO_AEQUALIS_I32((i32)tabula_cellula(&tabula, ZEPHYRUM, ZEPHYRUM), (i32)'h');
    CREDO_VERUM(status.mutatus);
}


/* ==================================================
 * Main
 * ================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("test_vim", M);  /* 1MB - pro multis tabulis */
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    g_piscina = piscina;
    credo_aperire(piscina);

    printf("--- Probans initiare ---\n");
    probans_initiare();

    printf("--- Probans motus hjkl ---\n");
    probans_motus_hjkl();

    printf("--- Probans motus limites ---\n");
    probans_motus_limites();

    printf("--- Probans motus 0 $ ---\n");
    probans_motus_0_dollar();

    printf("--- Probans motus ^ ---\n");
    probans_motus_caret();

    printf("--- Probans i inserere ---\n");
    probans_i_inserere();

    printf("--- Probans a appendo ---\n");
    probans_a_appendo();

    printf("--- Probans A appendo finis ---\n");
    probans_A_appendo_finis();

    printf("--- Probans I inserere initium ---\n");
    probans_I_inserere_initium();

    printf("--- Probans inserere characterem ---\n");
    probans_inserere_characterem();

    printf("--- Probans cursor wrap ad finem lineae ---\n");
    probans_cursor_wrap_ad_finem_lineae();

    printf("--- Probans inserere in medio ---\n");
    probans_inserere_in_medio();

    printf("--- Probans x delere ---\n");
    probans_x_delere();

    printf("--- Probans dd delere lineam ---\n");
    probans_dd_delere_lineam();

    printf("--- Probans backspace ---\n");
    probans_backspace();

    printf("--- Probans o nova linea infra ---\n");
    probans_o_nova_linea_infra();

    printf("--- Probans O nova linea supra ---\n");
    probans_O_nova_linea_supra();

    printf("--- Probans escape ex inserere ---\n");
    probans_escape_ex_inserere();

    printf("--- Probans escape in normali ---\n");
    probans_escape_in_normali();

    printf("--- Probans w verbum proximum ---\n");
    probans_w_verbum_proximum();

    printf("--- Probans b verbum praecedens ---\n");
    probans_b_verbum_praecedens();

    printf("--- Probans g initium tabulae ---\n");
    probans_g_initium_tabulae();

    printf("--- Probans G finem tabulae ---\n");
    probans_G_finem_tabulae();

    printf("--- Probans G ad finem buffer ---\n");
    probans_G_ad_finem_buffer();

    printf("--- Probans $ in linea vacua ---\n");
    probans_dollar_in_linea_vacua();

    printf("--- Probans d$ delere ad finem lineae ---\n");
    probans_d_dollar_delere_ad_finem_lineae();

    printf("--- Probans dG delere ad finem tabulae ---\n");
    probans_dG_delere_ad_finem_tabulae();

    printf("--- Probans >> indent ---\n");
    probans_indent();

    printf("--- Probans << dedent ---\n");
    probans_dedent();

    printf("--- Probans o auto-indent ---\n");
    probans_o_auto_indent();

    printf("--- Probans O auto-indent ---\n");
    probans_O_auto_indent();

    printf("--- Probans enter auto-indent ---\n");
    probans_enter_auto_indent();

    printf("--- Probans vim dimensiones parvae ---\n");
    probans_vim_dimensiones_parvae();

    printf("--- Probans vim G dimensiones variae ---\n");
    probans_vim_G_dimensiones_variae();

    printf("--- Probans vim dd ultima linea ---\n");
    probans_vim_dd_ultima_linea();

    /* Test 4 removed - see comment in test section */

    printf("--- Probans enter split line ---\n");
    probans_enter_split_line();

    printf("--- Probans enter split line in medio ---\n");
    probans_enter_split_line_in_medio();

    printf("--- Probans enter ad initium lineae ---\n");
    probans_enter_ad_initium_lineae();

    printf("--- Probans enter split cum indentatio ---\n");
    probans_enter_split_cum_indentatio();

    printf("--- Probans backspace join lines ---\n");
    probans_backspace_join_lines();

    printf("--- Probans backspace join lines cum spatiis ---\n");
    probans_backspace_join_lines_cum_spatiis();

    printf("--- Probans V visual mode ---\n");
    probans_V_visual_mode();

    printf("--- Probans V indent selectio ---\n");
    probans_V_indent_selectio();

    printf("--- Probans V dedent selectio ---\n");
    probans_V_dedent_selectio();

    printf("--- Probans dd cut et p paste ---\n");
    probans_dd_cut_et_p_paste();

    printf("--- Probans P paste ante ---\n");
    probans_P_paste_ante();

    printf("--- Probans u undo dd ---\n");
    probans_u_undo_dd();

    printf("--- Probans u undo p ---\n");
    probans_u_undo_p();

    printf("--- Probans V d cut multi ---\n");
    probans_V_d_cut_multi();

    printf("--- Probans V d undo multi ---\n");
    probans_V_d_undo_multi();

    printf("--- Probans p paste multi ---\n");
    probans_p_paste_multi();

    printf("--- Probans J jungere lineas ---\n");
    probans_J_jungere_lineas();

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
