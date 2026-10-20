#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    MultipartAedificator* a = multipart_creare(p);
    char stackbuf[8];
    char* heap;
    chorda c, val;
    /* case 1: buffer reused before finire (no crash, shows referenced not copied) */
    memcpy(stackbuf, "ORIGINAL", 8);
    multipart_addere_fasciculum(a, "f", "f.bin", "text/plain", (const i8*)stackbuf, 8);
    val.datum = (i8*)stackbuf; val.mensura = 8;
    multipart_addere_textum(a, "t", val);
    memcpy(stackbuf, "CHANGED!", 8);
    c = multipart_corpus_finire(a);
    printf("contains ORIGINAL=%d CHANGED!=%d\n",
        strstr((const char*)c.datum, "ORIGINAL") != NULL && 0, 0);
    {
        unsigned int i; int o = 0, ch = 0;
        for (i = 0; i + 8 <= c.mensura; i++) {
            if (memcmp(c.datum + i, "ORIGINAL", 8) == 0) o++;
            if (memcmp(c.datum + i, "CHANGED!", 8) == 0) ch++;
        }
        printf("reuse: ORIGINAL x%d, CHANGED! x%d\n", o, ch);
    }
    /* case 2: heap buffer freed before finire */
    a = multipart_creare(p);
    heap = (char*)malloc(16);
    memcpy(heap, "0123456789abcdef", 16);
    multipart_addere_fasciculum(a, "f", "f.bin", "text/plain", (const i8*)heap, 16);
    free(heap);
    printf("freed; calling finire\n");
    fflush(stdout);
    c = multipart_corpus_finire(a);
    printf("finire returned %u\n", c.mensura);
    piscina_destruere(p);
    return 0;
}
