#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>
#include <string.h>

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
    MultipartAedificator* a = multipart_creare(p);
    static const unsigned char bin[] = {0x00, 0x0D, 0x0A, 0xFF, 0x80, 0x00, '-', '-', 0x0D};
    char big[64];
    chorda slice, withnul, empty, cr;
    chorda ct, corpus;
    memset(big, 'Q', sizeof big);
    memcpy(big, "abcdefXYZ", 9);
    slice.datum = (i8*)big + 3; slice.mensura = 3;   /* "def", not NUL terminated, followed by XYZQQQ */
    withnul.datum = (i8*)"a\0b"; withnul.mensura = 3;
    empty.datum = NIHIL; empty.mensura = 0;
    cr.datum = (i8*)"\r"; cr.mensura = 1;
    multipart_addere_textum(a, "slice", slice);
    multipart_addere_textum(a, "nul", withnul);
    multipart_addere_textum(a, "vacuum", empty);
    multipart_addere_textum(a, "cr", cr);
    multipart_addere_textum(a, "dup", chorda_ex_literis("1", p));
    multipart_addere_textum(a, "dup", chorda_ex_literis("2", p));
    multipart_addere_textum(a, "", chorda_ex_literis("emptyname", p));
    multipart_addere_fasciculum(a, "file", "bin.dat", NIHIL, (const i8*)bin, (i32)sizeof bin);
    multipart_addere_fasciculum(a, "f0", "zero.dat", "text/plain", NIHIL, 0);
    multipart_addere_fasciculum(a, "f0b", "", "text/plain", (const i8*)"x", 0);
    multipart_addere_fasciculum(a, "utf", "\xc3\xa9t\xc3\xa9.txt", "text/plain", (const i8*)"e", 1);
    multipart_addere_fasciculum(a, "badutf", "\xff\xfe.txt", "text/plain", (const i8*)"b", 1);
    ct = multipart_content_type(a);
    corpus = multipart_corpus_finire(a);
    printf("ct=%.*s\n", (int)ct.mensura, ct.datum);
    printf("corpus mensura=%u\n", corpus.mensura);
    scribere("build/sonda/multipart_forma_basica.out", ct, corpus);
    piscina_destruere(p);
    return 0;
}
