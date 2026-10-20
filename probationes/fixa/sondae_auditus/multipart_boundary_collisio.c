#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

static void scribere(const char* via, chorda ct, chorda c)
{
    FILE* f = fopen(via, "wb");
    fwrite(ct.datum, 1, ct.mensura, f);
    fputc('\n', f);
    fwrite(c.datum, 1, c.mensura, f);
    fclose(f);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    MultipartAedificator *a, *b, *c;
    chorda cta, ctb, ctc, ca, cb, cc, fake;
    char fk[256];
    unsigned long t0 = (unsigned long)time(NIHIL);
    a = multipart_creare(p);
    b = multipart_creare(p);
    cta = multipart_content_type(a);
    ctb = multipart_content_type(b);
    printf("t=%lu\n", t0);
    printf("A: %.*s\n", (int)cta.mensura, cta.datum);
    printf("B: %.*s\n", (int)ctb.mensura, ctb.datum);
    printf("idem: %d\n", cta.mensura == ctb.mensura && memcmp(cta.datum, ctb.datum, cta.mensura) == 0);
    /* case 1: upload a previously built form as file in a new form */
    multipart_addere_textum(a, "inner", chorda_ex_literis("secret", p));
    ca = multipart_corpus_finire(a);
    multipart_addere_textum(b, "outer", chorda_ex_literis("x", p));
    multipart_addere_fasciculum(b, "file", "form.txt", "text/plain", ca.datum, ca.mensura);
    cb = multipart_corpus_finire(b);
    scribere("build/sonda/multipart_boundary_collisio_nidus.out", ctb, cb);
    /* case 2: text value containing CRLF--<boundary> forging a part (boundary predicted from body of A) */
    c = multipart_creare(p);
    ctc = multipart_content_type(c);
    sprintf(fk, "hello\r\n--%.*s\r\nContent-Disposition: form-data; name=\"admin\"\r\n\r\ntrue",
            32, (const char*)ctc.datum + 30);
    fake = chorda_ex_literis(fk, p);
    multipart_addere_textum(c, "comment", fake);
    cc = multipart_corpus_finire(c);
    scribere("build/sonda/multipart_boundary_collisio_falsum.out", ctc, cc);
    printf("corpus B mensura=%u, C mensura=%u\n", cb.mensura, cc.mensura);
    piscina_destruere(p);
    return 0;
}
