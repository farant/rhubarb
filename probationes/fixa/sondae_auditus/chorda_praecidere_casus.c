#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
static void sh(const char* tag, chorda in, chorda r) {
    printf("%s -> len=%u datum=%s off=%ld\n", tag, r.mensura, r.datum ? "ptr" : "NIHIL",
        r.datum ? (long)(r.datum - in.datum) : -1L);
}
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda s, r;
    i8 ws[8] = {' ', '\t', '\n', '\r', '\v', '\f', 0, (i8)0xA0};
    i8 nb[3] = {(i8)0xC2, (i8)0xA0, 'x'};
    i32 k;
    setvbuf(stdout, NIHIL, _IONBF, 0);
    s = chorda_ex_literis("   ", p);
    sh("praecidere \"   \"", s, chorda_praecidere(s));
    sh("praecidere_sinistram \"   \"", s, chorda_praecidere_sinistram(s));
    sh("praecidere_dextram \"   \"", s, chorda_praecidere_dextram(s));
    sh("praecidi_laterale \"   \"", s, chorda_praecidi_laterale(s, p));
    s = chorda_ex_literis(" ", p);
    sh("praecidere_dextram \" \"", s, chorda_praecidere_dextram(s));
    sh("praecidere \" \"", s, chorda_praecidere(s));
    s = chorda_ex_literis("  ab  ", p);
    r = chorda_praecidi_laterale(s, p);
    printf("praecidi_laterale \"  ab  \" -> \"%.*s\" view_of_input=%d (allocates? piscina arg)\n", (int)r.mensura, (char*)r.datum,
        r.datum >= s.datum && r.datum < s.datum + s.mensura);
    for (k = 0; k < 8; k++) {
        i8 one[3]; chorda c; one[0] = ws[k]; one[1] = 'x'; one[2] = ws[k];
        c.datum = one; c.mensura = 3;
        r = chorda_praecidere(c);
        printf("byte 0x%02X counted as space: praecidere=%u laterale=%u\n", ws[k], r.mensura, chorda_praecidi_laterale(c, p).mensura);
    }
    s.datum = nb; s.mensura = 3;
    printf("UTF-8 NBSP C2A0 x: praecidere len=%u\n", chorda_praecidere(s).mensura);
    return 0;
}
