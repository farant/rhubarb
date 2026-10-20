#include "latina.h"
#include "piscina.h"
#include "widget.h"
#include <stdio.h>
#include <string.h>

/* Sonda: Shift-Tab non consumptum - manager focum movet PRORSUM. */
static b32 non_consumit(struct Widget* w, const Eventus* e)
{
    (void)w; (void)e;
    return FALSUM;
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    ManagerWidget* m = manager_widget_creare(p);
    Eventus ev;
    b32 r;
    manager_widget_registrare(m, NIHIL, NIHIL, non_consumit, 0, 0, 10, 10);
    manager_widget_registrare(m, NIHIL, NIHIL, non_consumit, 10, 0, 10, 10);
    manager_widget_registrare(m, NIHIL, NIHIL, non_consumit, 20, 0, 10, 10);
    manager_widget_ponere_focus(m, 0);
    memset(&ev, 0, sizeof ev);
    ev.genus = EVENTUS_CLAVIS_DEPRESSUS;
    ev.datum.clavis.clavis = CLAVIS_TABULA;
    ev.datum.clavis.modificantes = MOD_SHIFT;
    r = manager_widget_tractare_eventum(m, &ev);
    printf("Shift-Tab ex focus 0: redditum=%d focus_index=%d (praecedens daret 2)\n",
        (int)r, (int)m->focus_index);
    ev.datum.clavis.modificantes = 0;
    manager_widget_ponere_focus(m, 0);
    r = manager_widget_tractare_eventum(m, &ev);
    printf("Tab ex focus 0: redditum=%d focus_index=%d\n", (int)r, (int)m->focus_index);
    piscina_destruere(p);
    return 0;
}
