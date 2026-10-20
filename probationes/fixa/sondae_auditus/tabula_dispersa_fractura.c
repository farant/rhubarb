#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char bufs[64][8];
static chorda mk(int idx) { chorda c; sprintf(bufs[idx], "k%02d", idx); c.datum = (i8*)bufs[idx]; c.mensura = (i32)strlen(bufs[idx]); return c; }

int main(int argc, char** argv)
{
    Piscina* p;
    TabulaDispersa* t;
    int idx;
    int scen = argc > 1 ? atoi(argv[1]) : 0;
    b32 r;
    p = piscina_generare_certae_magnitudinis("fixa", 2048);
    t = tabula_dispersa_creare_chorda(p, 16);
    printf("creata cap=%u\n", t->capacitas);
    for (idx = 0; idx < 25; idx++) {
        r = tabula_dispersa_inserere(t, mk(idx), (void*)(long)(idx + 1));
        if (!r) printf("insert %d FALSUM\n", idx);
    }
    printf("post 25 inserta: cap=%u numerus=%u\n", t->capacitas, t->numerus);
    fflush(stdout);
    if (scen == 0) {
        printf("CASUS novum: inserere k25 (crescentia debet fallere -> FALSUM exspectatum)\n");
        fflush(stdout);
        r = tabula_dispersa_inserere(t, mk(25), (void*)1);
        printf("redditum %d numerus=%u\n", r, t->numerus);
    } else {
        printf("CASUS renovatio: inserere k00 iam praesens (nulla allocatio necessaria)\n");
        fflush(stdout);
        r = tabula_dispersa_inserere(t, mk(0), (void*)99);
        printf("redditum %d numerus=%u\n", r, t->numerus);
    }
    return 0;
}
