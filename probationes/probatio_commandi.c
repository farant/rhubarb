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
