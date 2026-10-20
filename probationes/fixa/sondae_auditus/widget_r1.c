#include "latina.h"
#include "piscina.h"
#include "widget.h"
#include <stdio.h>
#include <string.h>

/* Sonda: widgets reddunt cellas ad VI*scala x VIII*scala pixela;
 * click-to-focus convertit pixela per VI x VIII fixa. */
static int ultimus_tractans = -1;
static int sedes_px[2][2];

static void reddere_cb(struct Widget* w, TabulaPixelorum* t, i32 x, i32 y,
    i32 lat, i32 alt, i32 scala, b32 foc)
{
    int idx = (int)(long)w->datum;
    (void)t; (void)lat; (void)alt; (void)foc; (void)y;
    sedes_px[idx][0] = (int)(x * VI * scala);
    sedes_px[idx][1] = (int)((x + lat) * VI * scala);
}

static b32 tractare_cb(struct Widget* w, const Eventus* e)
{
    (void)e;
    ultimus_tractans = (int)(long)w->datum;
    return VERUM;
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    ManagerWidget* m = manager_widget_creare(p);
    TabulaPixelorum tab;
    Eventus ev;
    int scala;
    memset(&tab, 0, sizeof tab);
    manager_widget_registrare(m, (void*)0L, reddere_cb, tractare_cb, 0, 0, 10, 10);
    manager_widget_registrare(m, (void*)1L, reddere_cb, tractare_cb, 10, 0, 10, 10);
    for (scala = 1; scala <= 2; scala++) {
        manager_widget_ponere_focus(m, 0);
        manager_widget_reddere(m, &tab, scala);
        printf("scala %d: widget A pixela x [%d,%d), widget B pixela x [%d,%d)\n", scala,
            sedes_px[0][0], sedes_px[0][1], sedes_px[1][0], sedes_px[1][1]);
        memset(&ev, 0, sizeof ev);
        ev.genus = EVENTUS_MUS_DEPRESSUS;
        ev.datum.mus.x = (s32)(sedes_px[1][0] + 18); /* intra B ut redditus */
        ev.datum.mus.y = 20;
        m->tempus_ultimus_click = 0.0;
        manager_widget_tractare_eventum(m, &ev);
        printf("  click ad pixel x=%d (intra B redditum): focus_index=%d tractans=%d (expectatum 1)\n",
            (int)ev.datum.mus.x, (int)m->focus_index, ultimus_tractans);
    }
    piscina_destruere(p);
    return 0;
}
