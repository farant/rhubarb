#include "latina.h"
#include "thema.h"

integer
principale (vacuum)
{
    thema_initiare();
    {
    i32 bg = thema_color(COLOR_BACKGROUND);
    thema_ponere_palette(aquinas_palette);
    (vacuum)bg;
    }
    redde ZEPHYRUM;
}
