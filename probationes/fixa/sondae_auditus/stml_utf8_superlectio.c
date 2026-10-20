#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void mon(const char* tag, const char* s, i32 len, Piscina* pp, InternamentumChorda* it)
{
    chorda in, w; StmlResultus r; i32 i, n;
    char* buf = (char*)malloc((size_t)len);   /* EXACT size: no trailing NUL */
    memcpy(buf, s, (size_t)len);
    in.datum = (i8*)buf; in.mensura = len;
    r = stml_legere(in, pp, it);
    n = stml_numerus_liberorum(r.radix);
    printf("%s\tstatus=%d\tliberi=%u\t", tag, (int)r.status, n);
    for (i = 0; i < n; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(r.radix, i);
        i32 j;
        printf("[g%d pos %u-%u len=%u:", (int)l->genus, l->positus_initium, l->positus_finis, l->valor ? l->valor->mensura : 0);
        if (l->valor) for (j = 0; j < l->valor->mensura; j++) printf("%02x", (unsigned)l->valor->datum[j]);
        printf("] ");
    }
    w = stml_scribere(r.radix, pp, FALSUM);
    printf("\tout_len=%u in_len=%u\n", w.mensura, len);
    free(buf);
}

int main(void)
{
    Piscina* pp = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(pp);
    mon("UTF8_TITULUS", "<\xc3\xa9>x</\xc3\xa9>", 11, pp, it);
    mon("CLAUSURA_MALA", "x</9>", 5, pp, it);
    mon("CLAUSURA_MALA_FINIS", "</9", 3, pp, it);
    mon("APERTURA_MALA_FINIS", "<9", 2, pp, it);
    mon("CLAUSURA_PUNCTUM", "<a>t</.>", 8, pp, it);
    return 0;
}
