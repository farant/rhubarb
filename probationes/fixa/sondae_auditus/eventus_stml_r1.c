#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "eventus.h"
#include "eventus_stml.h"
#include <stdio.h>
#include <string.h>

integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    Xar* x = xar_creare(p, (i32)magnitudo(Eventus));
    Eventus* e;
    Eventus* r;
    Xar* lecta;
    chorda s;
    character* c;
    integer valores[3];
    integer k;
    valores[0] = 0; valores[1] = -1; valores[2] = 7;
    per (k = 0; k < 3; k++)
    {
        e = (Eventus*)xar_addere(x);
        memset(e, 0, magnitudo(Eventus));
        e->genus = EVENTUS_MUS_MOTUS;
        e->tempus = 1000 + k;
        e->datum.mus.x = 5;
        e->datum.mus.indicator_genus = EVENTUS_INDICATOR_STILUS;
        e->datum.mus.pressio = valores[k];
    }
    s = eventus_scribere_stml(x, p, in, FALSUM);
    c = chorda_ut_cstr(s, p);
    imprimere("%s\n", c);
    lecta = eventus_legere_stml(c, p, in);
    per (k = 0; k < 3; k++)
    {
        r = (Eventus*)xar_obtinere(lecta, (i32)k);
        imprimere("in pressio=%d -> out pressio=%d\n", valores[k],
            (integer)r->datum.mus.pressio);
    }
    redde 0;
}
