#include "latina.h"
#include "piscina.h"
#include "dithering.h"
#include "importatio_visus.h"
#include "delineare_mandata.h"
#include "thema.h"
#include <stdio.h>

s32 principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    ImportatioVisus* v;
    TabulaPixelorum* t;
    Imago im;
    s32 k, iter;

    thema_initiare();
    im.latitudo = 64; im.altitudo = 64;
    im.pixela = (i8*)piscina_allocare(p, (memoriae_index)(64 * 64 * 4));
    per (k = 0; k < 64 * 64 * 4; k++) im.pixela[k] = (i8)(k * 7);
    t = tabula_pixelorum_creare_nuda(p, 800, 600);
    v = importatio_visus_creare(p);
    importatio_visus_initiare_sessionem(v, &im);
    importatio_visus_reddere(v, t, 0, 0, 100, 60, 1, VERUM);
    imprimere("post initium: scratch usus=%lu\n", (unsigned long)piscina_summa_usus(v->piscina_scratch));
    per (iter = 0; iter < 200; iter++)
    {
        importatio_visus_ponere_modus(v, (b32)(iter % 2));
        importatio_visus_reddere(v, t, 0, 0, 100, 60, 1, VERUM);
        si (iter % 50 == 49)
            imprimere("post %d redithering: scratch usus=%lu, piscina principalis usus=%lu, preview %dx%d\n",
                (int)(iter + 1), (unsigned long)piscina_summa_usus(v->piscina_scratch),
                (unsigned long)piscina_summa_usus(p), (int)v->preview_latitudo, (int)v->preview_altitudo);
    }
    redde 0;
}
