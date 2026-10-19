/* Paste exprimere's own output for n = 2147483649 and for
 * n = 2147483649 * 1024 as C89 code: does the compiler/UBSan see int
 * overflow? Also print what exprimere emits for both now. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i64 qs[3];
    int k;
    volatile i32 m = M;   /* runtime copy so UBSan checks at run time */
    s32 a, b;
    i64 lit1, lit2;
    qs[0] = 2147483649ULL; qs[1] = 2147483649ULL * 1024ULL; qs[2] = 2147999999ULL;
    for (k = 0; k < 3; k++)
    {
        chorda e = numerus_romanus_exprimere(qs[k], NIHIL, p);
        printf("exprimere(%llu) = %.*s\n", (unsigned long long)qs[k], (int)e.mensura, (char*)e.datum);
    }
    /* literal transcription (compile-time): */
    lit1 = MMCXLVII * M * M + CDLXXXIII * M + DCXLIX;
    lit2 = (i64)(MMCXLVII * M * M + CDLXXXIII * M + DCXLIX) * MXXIV;
    printf("literal1 = %llu (want 2147483649)\n", (unsigned long long)lit1);
    printf("literal2 = %llu (want %llu)\n", (unsigned long long)lit2, 2147483649ULL * 1024ULL);
    /* same shape at run time, with the signed int the literal has */
    a = (s32)MMCXLVII * (s32)m * (s32)m + (s32)CDLXXXIII * (s32)m;
    b = a + (s32)DCXLIX;
    printf("runtime int sum = %d\n", b);
    piscina_destruere(p);
    return 0;
}
