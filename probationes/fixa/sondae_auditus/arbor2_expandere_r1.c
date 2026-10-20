#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "arbor2_expandere.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void currere(const char* via, int persistens)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    Arbor2Expansion* e = arbor2_expansion_creare(p, in);
    FILE* f; static char buf[4096]; size_t n; Xar* out; int k, cnt, alpha = 0, sigma = 0;
    f = fopen(via, "rb"); n = fread(buf, 1, sizeof(buf) - 1, f); fclose(f); buf[n] = 0;
    if (persistens) arbor2_expansion_ponere_include_modum(e, ARBOR2_INCLUDE_PERSISTENT);
    out = arbor2_expansion_processare(e, buf, (i32)n, via);
    cnt = (int)xar_numerus(out);
    printf("%s (%s): %d tokens:", via, persistens ? "PERSISTENT" : "RESET_PER_PARSE", cnt);
    for (k = 0; k < cnt; k++) {
        Arbor2Token* t = *(Arbor2Token**)xar_obtinere(out, (i32)k);
        printf(" %.*s", (int)t->lexema->valor.mensura, (const char*)t->lexema->valor.datum);
        if (t->lexema->valor.mensura == 5 && memcmp(t->lexema->valor.datum, "alpha", 5) == 0) alpha++;
        if (t->lexema->valor.mensura == 5 && memcmp(t->lexema->valor.datum, "sigma", 5) == 0) sigma++;
    }
    printf("\n  alpha x%d  sigma x%d\n", alpha, sigma);
    piscina_destruere(p);
}

int main(int argc, char** argv)
{
    char via[1024];
    const char* dir = "build/sonda/arbor2_expandere_r1_dir";
    char cwd[900];
    if (getenv("PWD")) strcpy(cwd, getenv("PWD")); else strcpy(cwd, ".");
    sprintf(via, "%s/%s/main.c", cwd, dir);
    currere(via, 0);
    currere(via, 1);
    if (argc > 1) {
        sprintf(via, "%s/%s/selfmain.c", cwd, dir);
        if (atoi(argv[1]) == 1) currere(via, 1);
        else currere(via, 0);
    }
    return 0;
}
