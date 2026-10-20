#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
static void pr(const char* fn, const char* in, chorda r) {
    printf("%s(\"%s\") -> \"%.*s\" len=%u%s\n", fn, in, (int)r.mensura, r.datum ? (char*)r.datum : "", r.mensura, r.datum ? "" : " NIHIL");
}
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    const char* in[] = {"HTTPServer", "XMLHttpRequest", "utf8Decode", "v2Api", "hello_world", "hello world", "__a", "a--b",
        " x ", "a", "A", "_", "", "caf\xc3\xa9 au lait", "ABC", "aB", "getHTTP", "x1y2", "ID", NIHIL};
    i32 k;
    setvbuf(stdout, NIHIL, _IONBF, 0);
    for (k = 0; in[k]; k++) {
        chorda s = chorda_ex_literis(in[k], p);
        pr("serpens", in[k], chorda_serpens(s, p));
        pr("pascalis", in[k], chorda_pascalis(s, p));
        pr("camelus", in[k], chorda_camelus(s, p));
        pr("kebab", in[k], chorda_kebab(s, p));
        pr("pascalis_serpens", in[k], chorda_pascalis_serpens(s, p));
    }
    return 0;
}
