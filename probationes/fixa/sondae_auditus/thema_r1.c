#include "latina.h"
#include "thema.h"
#include <stdio.h>

integer
principale (vacuum)
{
    i8    rubra[XLVIII];
    i32   i;
    Color ante;
    Color post;
    Color post_idx;

    thema_initiare();
    ante = thema_color(COLOR_BACKGROUND);

    /* palette tota rubra: omnes colores (0x3F,0,0) */
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        rubra[i * III]     = (i8)0x3F;
        rubra[i * III + I] = (i8)ZEPHYRUM;
        rubra[i * III + II] = (i8)ZEPHYRUM;
    }
    thema_ponere_palette(rubra);
    post = thema_color(COLOR_BACKGROUND);
    post_idx = thema_color_ex_indice_colorationis((i8)I);

    imprimere("ante: %u %u %u\n", (insignatus)ante.r, (insignatus)ante.g, (insignatus)ante.b);
    imprimere("post: %u %u %u (expectatum 255 0 0)\n", (insignatus)post.r, (insignatus)post.g, (insignatus)post.b);
    imprimere("post_idx: %u %u %u (expectatum 255 0 0)\n", (insignatus)post_idx.r, (insignatus)post_idx.g, (insignatus)post_idx.b);
    imprimere("%s\n", (post.r == CCLV && post.g == ZEPHYRUM) ? "PALETTE APPLICATA" : "PALETTE IGNORATA");
    redde ZEPHYRUM;
}
