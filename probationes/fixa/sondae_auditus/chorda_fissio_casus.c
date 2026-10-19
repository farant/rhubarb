#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
static void show(const char* tag, chorda_fissio_fructus f, chorda s) {
    i32 k;
    printf("%s numerus=%u [", tag, f.numerus);
    for (k = 0; k < f.numerus; k++) {
        printf("%s\"%.*s\"(len%u,%s)", k ? "," : "", (int)f.elementa[k].mensura,
            f.elementa[k].datum ? (char*)f.elementa[k].datum : "", f.elementa[k].mensura,
            f.elementa[k].datum == NIHIL ? "NIHIL" :
            (f.elementa[k].datum >= s.datum && f.elementa[k].datum <= s.datum + s.mensura ? "view" : "copy"));
    }
    printf("]\n");
}
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    setvbuf(stdout, NIHIL, _IONBF, 0);
    i8 hb[5]; i8 nb[5];
    chorda s, d;
    s = chorda_ex_literis("", p); show("empty \",\"", chorda_fissio(s, ',', p), s);
    s = chorda_ex_literis(",", p); show("\",\" by ','", chorda_fissio(s, ',', p), s);
    s = chorda_ex_literis("a,,b,", p); show("\"a,,b,\" by ','", chorda_fissio(s, ',', p), s);
    s = chorda_ex_literis("abc", p); show("\"abc\" by ','", chorda_fissio(s, ',', p), s);
    hb[0]='a'; hb[1]=(i8)0xFF; hb[2]='b'; hb[3]=(i8)0xFF; hb[4]='c';
    s.datum = hb; s.mensura = 5;
    show("a\\xFFb\\xFFc by (char)0xFF (expect 3)", chorda_fissio(s, (char)0xFF, p), s);
    hb[1]=(i8)0xC3; hb[3]=(i8)0xC3;
    show("a\\xC3b\\xC3c by (char)0xC3 (expect 3)", chorda_fissio(s, (char)0xC3, p), s);
    nb[0]='a'; nb[1]=0; nb[2]='b'; nb[3]=0; nb[4]='c';
    s.datum = nb; s.mensura = 5;
    show("a\\0b\\0c by '\\0' (expect 3)", chorda_fissio(s, '\0', p), s);
    /* fissio_chorda overlap & equal */
    s = chorda_ex_literis("aaa", p); d = chorda_ex_literis("aa", p);
    show("fissio_chorda \"aaa\" by \"aa\"", chorda_fissio_chorda(s, d, p), s);
    s = chorda_ex_literis("ab", p); d = chorda_ex_literis("ab", p);
    show("fissio_chorda \"ab\" by \"ab\"", chorda_fissio_chorda(s, d, p), s);
    return 0;
}
