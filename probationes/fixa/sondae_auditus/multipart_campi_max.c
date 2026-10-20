#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>
#include <string.h>

static unsigned int numerare(chorda c, const char* pat)
{
    unsigned int i, n = 0, l = (unsigned int)strlen(pat);
    for (i = 0; i + l <= c.mensura; i++) if (memcmp(c.datum + i, pat, l) == 0) n++;
    return n;
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    MultipartAedificator* a = multipart_creare(p);
    chorda c1, c2, c3, ct1, ct2;
    char nm[16];
    int k;
    for (k = 0; k < 40; k++) {
        sprintf(nm, "f%02d", k);
        if (k == 35) multipart_addere_fasciculum(a, "IMPORTANT_FILE", "x.bin", "text/plain", (const i8*)"x", 1);
        else multipart_addere_textum(a, nm, chorda_ex_literis("v", p));
    }
    c1 = multipart_corpus_finire(a);
    printf("added 40 (file at #35): parts in body=%u, f31 present=%u, f32 present=%u, IMPORTANT_FILE present=%u\n",
        numerare(c1, "Content-Disposition"), numerare(c1, "name=\"f31\""), numerare(c1, "name=\"f32\""), numerare(c1, "IMPORTANT_FILE"));

    a = multipart_creare(p);
    ct1 = multipart_content_type(a);
    multipart_addere_textum(a, "a", chorda_ex_literis("1", p));
    c1 = multipart_corpus_finire(a);
    c2 = multipart_corpus_finire(a);
    printf("finire bis: idem=%d mensura %u %u, closings=%u\n",
        c1.mensura == c2.mensura && memcmp(c1.datum, c2.datum, c1.mensura) == 0, c1.mensura, c2.mensura, numerare(c2, "--\r\n"));
    multipart_addere_textum(a, "b", chorda_ex_literis("2", p));
    c3 = multipart_corpus_finire(a);
    ct2 = multipart_content_type(a);
    printf("addere post finire: c1 parts=%u, c3 parts=%u, c1 still intact=%d\n",
        numerare(c1, "Content-Disposition"), numerare(c3, "Content-Disposition"), memcmp(c1.datum, c2.datum, c1.mensura) == 0);
    printf("ct ante/post idem=%d; boundary in body=%d\n",
        ct1.mensura == ct2.mensura && memcmp(ct1.datum, ct2.datum, ct1.mensura) == 0,
        memcmp(c3.datum + 2, ct1.datum + 30, 32) == 0);
    a = multipart_creare(p);
    c1 = multipart_corpus_finire(a);
    printf("zero parts: [%.*s] mensura=%u\n", (int)c1.mensura - 2, c1.datum, c1.mensura);
    piscina_destruere(p);
    return 0;
}
