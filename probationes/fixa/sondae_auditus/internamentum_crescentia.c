#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 200000
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    chorda** ptrs = (chorda**)malloc(sizeof(chorda*) * NUM);
    char buf[32]; int k, bad_dup = 0, bad_content = 0, bad_est = 0, bad_dist = 0, bad_lit = 0;
    for (k = 0; k < NUM; k++) {
        chorda c; sprintf(buf, "k%d", k);
        c.datum = (i8*)buf; c.mensura = (i32)strlen(buf);
        ptrs[k] = chorda_internare(in, c);
        memset(buf, 'Z', sizeof buf); /* clobber caller buffer */
    }
    printf("numerus=%u (expect %d)\n", internamentum_numerus(in), NUM);
    for (k = 0; k < NUM; k++) {
        chorda c; chorda* r; sprintf(buf, "k%d", k);
        c.datum = (i8*)buf; c.mensura = (i32)strlen(buf);
        if (ptrs[k]->mensura != c.mensura || memcmp(ptrs[k]->datum, buf, c.mensura)) bad_content++;
        r = chorda_internare(in, c);
        if (r != ptrs[k]) bad_dup++;
        if (!chorda_est_internata(in, c)) bad_est++;
        if (chorda_internare_ex_literis(in, buf) != ptrs[k]) bad_lit++;
        if (k > 0 && ptrs[k] == ptrs[k-1]) bad_dist++;
        /* self-intern canonical */
        if (chorda_internare(in, *ptrs[k]) != ptrs[k]) bad_dup++;
    }
    printf("bad_content=%d bad_dup=%d bad_est=%d bad_lit=%d bad_dist=%d numerus=%u\n",
        bad_content, bad_dup, bad_est, bad_lit, bad_dist, internamentum_numerus(in));
    /* non-interned queries don't insert */
    { int miss = 0; for (k = NUM; k < NUM + 1000; k++) { chorda c; sprintf(buf, "k%d", k);
        c.datum = (i8*)buf; c.mensura = (i32)strlen(buf); if (chorda_est_internata(in, c)) miss++; }
      printf("false_positive_est=%d numerus_after_queries=%u\n", miss, internamentum_numerus(in)); }
    /* sub-slice of canonical */
    { chorda sub; chorda* r; sub.datum = ptrs[12345]->datum; sub.mensura = 3; /* "k12" */
      r = chorda_internare(in, sub);
      printf("subslice k12: same as ptrs[12]=%d datum_copied=%d\n", r == ptrs[12], r->datum != sub.datum); }
    piscina_destruere(p); free(ptrs);
    return 0;
}
