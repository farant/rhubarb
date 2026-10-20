#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    MultipartAedificator* a = multipart_creare(p);
    chorda c, v;
    multipart_addere_fasciculum(a, "f", "f.bin", "text/plain", NIHIL, 0);
    c = multipart_corpus_finire(a);
    printf("NIHIL,0: ok mensura=%u\n", c.mensura);
    a = multipart_creare(p);
    v.datum = NIHIL; v.mensura = 5;
    multipart_addere_textum(a, "t", v);
    c = multipart_corpus_finire(a);
    printf("textum {NIHIL,5}: mensura=%u body=[%.*s]\n", c.mensura, (int)c.mensura, c.datum);
    a = multipart_creare(p);
    multipart_addere_fasciculum(a, "f", "f.bin", "text/plain", NIHIL, 5);
    printf("added NIHIL,5; finire...\n"); fflush(stdout);
    c = multipart_corpus_finire(a);
    printf("NIHIL,5: mensura=%u\n", c.mensura);
    piscina_destruere(p);
    return 0;
}
