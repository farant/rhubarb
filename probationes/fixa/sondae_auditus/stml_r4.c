#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static vacuum mon(constans character* tag, constans character* s)
{
    StmlResultus r; chorda w;
    r = stml_legere_ex_literis(s, pp, it);
    w = stml_scribere(r.radix, pp, FALSUM);
    printf("%s\tin=[%s]\tstatus=%d\tscriptum=[%.*s]\n", tag, s, (int)r.status, (int)w.mensura, (char*)w.datum);
}

int main(void)
{
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    mon("FARCIMEN_COMM", "<r><a/><= s =><!--c--><b/></r>");
    mon("FARCIMEN_PLAIN", "<r><a/><= s =><b/></r>");
    mon("RETRO_COMM", "<r><a/><) s><!--c--><b/></r>");
    mon("FARCIMEN_COMM_ANTE", "<r><a/><!--d--><= s =><b/></r>");
    redde 0;
}
