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
    chorda s;
    e = (Eventus*)xar_addere(x);
    memset(e, 0, magnitudo(Eventus));
    e->genus = EVENTUS_CLAVIS_DEPRESSUS;
    e->tempus = 1050;
    e->datum.clavis.producta = 112;
    e->datum.clavis.codex = EVENTUS_CODEX_IGNOTUS;
    e = (Eventus*)xar_addere(x);
    memset(e, 0, magnitudo(Eventus));
    e->genus = EVENTUS_MUS_ROTULA;
    e->tempus = 1100;
    e->datum.rotula.dy = -2;
    s = eventus_scribere_stml(x, p, in, FALSUM);
    imprimere("%s\n", chorda_ut_cstr(s, p));
    redde 0;
}
