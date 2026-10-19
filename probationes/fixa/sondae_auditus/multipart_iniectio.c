#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>
#include <string.h>

static void caso(Piscina* p, const char* tag, const char* nc, const char* nf, const char* ctp)
{
    char via[128];
    MultipartAedificator* a = multipart_creare(p);
    chorda ct, c;
    FILE* f;
    if (nf == NIHIL) multipart_addere_textum(a, nc, chorda_ex_literis("v", p));
    else multipart_addere_fasciculum(a, nc, nf, ctp, (const i8*)"DATA", 4);
    ct = multipart_content_type(a);
    c = multipart_corpus_finire(a);
    sprintf(via, "build/sonda/multipart_iniectio_%s.out", tag);
    f = fopen(via, "wb");
    fwrite(ct.datum, 1, ct.mensura, f); fputc('\n', f); fwrite(c.datum, 1, c.mensura, f); fclose(f);
    printf("%s: %u bytes\n", tag, c.mensura);
}

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    caso(p, "quote_name", "x\"; filename=\"evil.exe", NIHIL, NIHIL);
    caso(p, "crlf_filename", "file", "a\r\nContent-Type: text/html", "image/png");
    caso(p, "crlf_ctype", "file", "a.png", "image/png\r\nX-Injected: 1");
    caso(p, "ctype_endhdr", "file", "a.png", "image/png\r\n\r\nPREFIX");
    caso(p, "backslash", "a\\", "b\\\"c", "text/plain");
    piscina_destruere(p);
    return 0;
}
