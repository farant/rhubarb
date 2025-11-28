/* probatio_vim.c - Probationes Vim Interpres */
#include "latina.h"
#include "vim.h"
#include "tabula_characterum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


/* ==================================================
 * Test: Initiatio
 * ================================================== */

hic_manens vacuum
probans_initiare(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_initiare(&tabula);
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

    tabula_ex_literis(&tabula, "hello\nworld\ntest");
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
}

hic_manens vacuum
probans_motus_limites(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "ab");
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
}

hic_manens vacuum
probans_motus_0_dollar(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "hello world");
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

    tabula_ex_literis(&tabula, "   hello");
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

    tabula_ex_literis(&tabula, "hello");
    status = vim_initiare(&tabula);

    /* i - inserere ante cursor */
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
}

hic_manens vacuum
probans_a_appendo(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "hello");
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

    tabula_ex_literis(&tabula, "hello");
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

    tabula_ex_literis(&tabula, "   hello");
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

    tabula_initiare(&tabula);
    status = vim_initiare(&tabula);

    /* Transire ad modum inserere */
    status = vim_tractare_clavem(status, 'i');

    /* Inserere characteres */
    status = vim_tractare_clavem(status, 'H');
    status = vim_tractare_clavem(status, 'i');

    tabula_asserere(&tabula, "Hi", "inserere Hi");
    CREDO_AEQUALIS_I32(status.cursor_columna, II);
}

hic_manens vacuum
probans_inserere_in_medio(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "hllo");
    status = vim_initiare(&tabula);

    /* Movere ad 'l' et inserere 'e' */
    status = vim_tractare_clavem(status, 'l');  /* ad 'l' */
    status = vim_tractare_clavem(status, 'i');  /* insert mode */
    status = vim_tractare_clavem(status, 'e');

    tabula_asserere(&tabula, "hello", "inserere e in medio");
}


/* ==================================================
 * Test: Deletio
 * ================================================== */

hic_manens vacuum
probans_x_delere(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "hello");
    status = vim_initiare(&tabula);

    /* x - delere characterem sub cursore */
    status = vim_tractare_clavem(status, 'x');
    tabula_asserere(&tabula, "ello", "x delere h");
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);
}

hic_manens vacuum
probans_dd_delere_lineam(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "line1\nline2\nline3");
    status = vim_initiare(&tabula);

    /* Movere ad linea 1 */
    status = vim_tractare_clavem(status, 'j');

    /* dd - delere lineam */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'd');

    tabula_asserere(&tabula, "line1\nline3", "dd delere line2");
}

hic_manens vacuum
probans_backspace(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "hello");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode et movere ad 'e' */
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'l');
    status = vim_tractare_clavem(status, 'i');
    CREDO_AEQUALIS_I32(status.cursor_columna, II);

    /* Backspace */
    status = vim_tractare_clavem(status, VIM_CLAVIS_BACKSPACE);
    tabula_asserere(&tabula, "hllo", "backspace delere e");
    CREDO_AEQUALIS_I32(status.cursor_columna, I);
}


/* ==================================================
 * Test: o et O - Nova Linea
 * ================================================== */

hic_manens vacuum
probans_o_nova_linea_infra(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "line1\nline2");
    status = vim_initiare(&tabula);

    /* o - inserere lineam infra */
    status = vim_tractare_clavem(status, 'o');

    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    tabula_asserere(&tabula, "line1\n\nline2", "o inserere lineam vacuam");
}

hic_manens vacuum
probans_O_nova_linea_supra(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "line1\nline2");
    status = vim_initiare(&tabula);

    /* Movere ad linea 1 */
    status = vim_tractare_clavem(status, 'j');

    /* O - inserere lineam supra */
    status = vim_tractare_clavem(status, 'O');

    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);
    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, ZEPHYRUM);

    tabula_asserere(&tabula, "line1\n\nline2", "O inserere lineam supra");
}


/* ==================================================
 * Test: Escape
 * ================================================== */

hic_manens vacuum
probans_escape_ex_inserere(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "hello");
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

    tabula_initiare(&tabula);
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

    tabula_ex_literis(&tabula, "hello world test");
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

    tabula_ex_literis(&tabula, "hello world test");
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

    tabula_ex_literis(&tabula, "line1\nline2\nline3");
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

    tabula_ex_literis(&tabula, "line1\nline2\nline3");
    status = vim_initiare(&tabula);

    /* G - ad finem */
    status = vim_tractare_clavem(status, 'G');
    CREDO_AEQUALIS_I32(status.cursor_linea, II);  /* ultima linea */
    CREDO_AEQUALIS_I32(status.cursor_columna, IV);  /* '3' in "line3" */
}


/* ==================================================
 * Test: d$ et dG
 * ================================================== */

hic_manens vacuum
probans_d_dollar_delere_ad_finem_lineae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "hello world");
    status = vim_initiare(&tabula);

    /* Movere ad 'w' */
    status.cursor_columna = VI;

    /* d$ - delere ad finem lineae */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, '$');

    tabula_asserere(&tabula, "hello", "d$ delere world");
}

hic_manens vacuum
probans_dG_delere_ad_finem_tabulae(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "line1\nline2\nline3");
    status = vim_initiare(&tabula);

    /* Movere ad linea 1 */
    status.cursor_linea = I;

    /* dG - delere ad finem */
    status = vim_tractare_clavem(status, 'd');
    status = vim_tractare_clavem(status, 'G');

    tabula_asserere(&tabula, "line1", "dG delere line2 et line3");
}


/* ==================================================
 * Test: >> et <<
 * ================================================== */

hic_manens vacuum
probans_indent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "hello");
    status = vim_initiare(&tabula);

    /* >> - indent */
    status = vim_tractare_clavem(status, '>');
    status = vim_tractare_clavem(status, '>');

    tabula_asserere(&tabula, "  hello", ">> indent hello");
}

hic_manens vacuum
probans_dedent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "  hello");
    status = vim_initiare(&tabula);

    /* << - dedent */
    status = vim_tractare_clavem(status, '<');
    status = vim_tractare_clavem(status, '<');

    tabula_asserere(&tabula, "hello", "<< dedent hello");
}


/* ==================================================
 * Test: Auto-indent
 * ================================================== */

hic_manens vacuum
probans_o_auto_indent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "  hello");
    status = vim_initiare(&tabula);

    /* o - nova linea infra cum auto-indent */
    status = vim_tractare_clavem(status, 'o');

    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);  /* post indentationem */
    CREDO_AEQUALIS_I32((i32)status.modo, (i32)MODO_VIM_INSERERE);

    /* Nova linea debet habere duo spatia */
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][ZEPHYRUM], (i32)' ');
    CREDO_AEQUALIS_I32((i32)tabula.cellulae[I][I], (i32)' ');
}

hic_manens vacuum
probans_O_auto_indent(vacuum)
{
    TabulaCharacterum tabula;
    VimStatus status;

    tabula_ex_literis(&tabula, "  hello");
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

    tabula_ex_literis(&tabula, "  hello");
    status = vim_initiare(&tabula);

    /* Transire ad insert mode ad finem */
    status = vim_tractare_clavem(status, 'A');

    /* Enter - nova linea cum auto-indent */
    status = vim_tractare_clavem(status, VIM_CLAVIS_ENTER);

    CREDO_AEQUALIS_I32(status.cursor_linea, I);
    CREDO_AEQUALIS_I32(status.cursor_columna, II);  /* post indentationem */
}


/* ==================================================
 * Main
 * ================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("test_vim", 4096);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
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
