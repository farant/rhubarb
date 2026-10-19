#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "multipart.h"
#include <stdio.h>

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    MultipartAedificator* a = multipart_creare(p);
    chorda c;
    multipart_addere_fasciculum(a, "file", NIHIL, "image/png", (const i8*)"PNGDATA", 7);
    multipart_addere_textum(a, NIHIL, chorda_ex_literis("v", p));
    c = multipart_corpus_finire(a);
    printf("after filename=NIHIL file + name=NIHIL text: body=[%.*s] mensura=%u\n", (int)c.mensura - 2, c.datum, c.mensura);
    piscina_destruere(p);
    return 0;
}
