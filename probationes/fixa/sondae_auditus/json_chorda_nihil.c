/* sonda: chorda {NIHIL, n>0} inputs; pretty indentatio wrap. argv[1]: legere|capere|ponere|indent */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "json.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    const char* m = argc > 1 ? argv[1] : "legere";
    Piscina* pisc = piscina_generare_dynamicum("sonda", 65536);
    chorda k; JsonResultus r;
    setvbuf(stdout, NULL, _IONBF, 0);
    k.datum = NULL; k.mensura = 3;
    r = json_legere_literis("{\"a\":1}", pisc);
    if (!strcmp(m, "legere")) {
        printf("json_legere({NIHIL,3})...\n");
        r = json_legere(k, pisc);
        printf("succ=%d err=[%.*s]\n", (int)r.successus, (int)r.error.mensura, (const char*)r.error.datum);
    } else if (!strcmp(m, "capere")) {
        printf("capere_chorda(obj,{NIHIL,3})...\n");
        printf("-> %s\n", json_objectum_capere_chorda(r.radix, k) ? "ptr" : "NIHIL");
    } else if (!strcmp(m, "ponere")) {
        printf("ponere_chorda(obj,{NIHIL,3})...\n");
        json_objectum_ponere_chorda(r.radix, k, NULL);
        printf("-> numerus %u\n", (unsigned)json_objectum_numerus(r.radix));
    } else {
        Piscina* certa = piscina_generare_certae_magnitudinis("sonda_certa", 1 << 22);
        ChordaAedificator* aed = chorda_aedificator_creare(certa, 64);
        chorda w;
        printf("pulchrum indentatio=(i32)-1 into 4MB certa...\n");
        json_scribere_ad_aedificator_pulchrum(r.radix, aed, (i32)0xFFFFFFFFu);
        w = chorda_aedificator_finire(aed);
        printf("-> mensura %u\n", (unsigned)w.mensura);
    }
    return 0;
}
