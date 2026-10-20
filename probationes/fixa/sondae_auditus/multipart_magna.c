#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void)
{
    static const unsigned long sizes[] = {1, 3999, 4000, 4001, 4096, 65536, 1048576, 16777219};
    unsigned int k;
    for (k = 0; k < sizeof sizes / sizeof sizes[0]; k++) {
        unsigned long n = sizes[k], i;
        Piscina* p = piscina_generare_dynamicum("sonda", 65536);
        MultipartAedificator* a = multipart_creare(p);
        unsigned char* buf = (unsigned char*)malloc(n);
        chorda c;
        unsigned long hdr, expect;
        int ok;
        clock_t t0;
        for (i = 0; i < n; i++) buf[i] = (unsigned char)((i * 2654435761UL) >> 13);
        multipart_addere_fasciculum(a, "f", "f.bin", "application/octet-stream", (const i8*)buf, (i32)n);
        t0 = clock();
        c = multipart_corpus_finire(a);
        /* header: --B\r\n + CD line + CT line + \r\n */
        hdr = 2 + 32 + 2 + strlen("Content-Disposition: form-data; name=\"f\"; filename=\"f.bin\"\r\n")
            + strlen("Content-Type: application/octet-stream\r\n") + 2;
        expect = hdr + n + 2 + 2 + 32 + 2 + 2;
        ok = c.mensura == expect && memcmp(c.datum + hdr, buf, n) == 0 && c.datum[hdr + n] == '\r';
        printf("n=%lu mensura=%u expect=%lu data_ok=%d ms=%ld\n", n, c.mensura, expect, ok,
            (long)((clock() - t0) * 1000 / CLOCKS_PER_SEC));
        free(buf);
        piscina_destruere(p);
    }
    return 0;
}
