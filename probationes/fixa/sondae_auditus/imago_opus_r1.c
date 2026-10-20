/* sonda: imago_rectangulum crassitudine > latere pingit extra rectangulum? */
#include "latina.h"
#include "imago.h"
#include "imago_opus.h"
#include "piscina.h"
#include <stdio.h>

interior i32
_extra_numerare (Imago* im, s32 x, s32 y, s32 w, s32 h)
{
    s32 px, py;
    i32 numerus = 0;
    per (py = 0; py < (s32)im->altitudo; py++)
    {
        per (px = 0; px < (s32)im->latitudo; px++)
        {
            i8* p = im->pixela + (py * (s32)im->latitudo + px) * 4;
            si (p[0] == 255)
            {
                si (px < x || px >= x + w || py < y || py >= y + h)
                {
                    numerus++;
                }
            }
        }
    }
    redde numerus;
}

interior i32
_intra_vacua (Imago* im, s32 x, s32 y, s32 w, s32 h)
{
    s32 px, py;
    i32 numerus = 0;
    per (py = y; py < y + h; py++)
    {
        per (px = x; px < x + w; px++)
        {
            i8* p = im->pixela + (py * (s32)im->latitudo + px) * 4;
            si (p[0] != 255) numerus++;
        }
    }
    redde numerus;
}

integer
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("sonda", 65536);
    Imago im;

    im = imago_creare(12, 24, 0, 0, 0, 255, piscina);
    imago_rectangulum(&im, 4, 2, 4, 20, 5, 255, 0, 0, 255);
    imprimere("w=4 h=20 c=5 @(4,2): extra=%u intra_vacua=%u\n",
        _extra_numerare(&im, 4, 2, 4, 20), _intra_vacua(&im, 4, 2, 4, 20));

    im = imago_creare(12, 24, 0, 0, 0, 255, piscina);
    imago_rectangulum(&im, 1, 10, 10, 4, 10, 255, 0, 0, 255);
    imprimere("w=10 h=4 c=10 @(1,10): extra=%u intra_vacua=%u\n",
        _extra_numerare(&im, 1, 10, 10, 4), _intra_vacua(&im, 1, 10, 10, 4));

    im = imago_creare(12, 24, 0, 0, 0, 255, piscina);
    imago_rectangulum(&im, 1, 1, 10, 5, 3, 255, 0, 0, 255);
    imprimere("w=10 h=5 c=3 (>= 2.5): extra=%u intra_vacua=%u\n",
        _extra_numerare(&im, 1, 1, 10, 5), _intra_vacua(&im, 1, 1, 10, 5));

    im = imago_creare(12, 24, 0, 0, 0, 255, piscina);
    imago_rectangulum(&im, 1, 1, 10, 5, 2, 255, 0, 0, 255);
    imprimere("w=10 h=5 c=2 (< 2.5): extra=%u intra_vacua=%u\n",
        _extra_numerare(&im, 1, 1, 10, 5), _intra_vacua(&im, 1, 1, 10, 5));

    piscina_destruere(piscina);
    redde 0;
}
