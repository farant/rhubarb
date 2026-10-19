#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "organum_montatio.h"
#include <stdio.h>

integer
principale (vacuum)
{
    Piscina* p;
    InternamentumChorda* in;
    Organum* o;

    p  = piscina_generare_dynamicum("sonda", 65536);
    in = internamentum_creare(p);
    fprintf(stderr, "--- bona (sine collisione) ---\n");
    o = organum_legere_viam(p, in, "build/sonda/om_r1_radix",
        "apps/bona.stml", "sonda");
    imprimere("bona: %s\n", o ? "Organum" : "NIHIL");
    fprintf(stderr, "--- proba (campus 'focus' bis) ---\n");
    o = organum_legere_viam(p, in, "build/sonda/om_r1_radix",
        "apps/proba.stml", "sonda");
    imprimere("proba: %s\n", o ? "Organum" : "NIHIL");
    fprintf(stderr, "--- finis stderr ---\n");
    redde ZEPHYRUM;
}
