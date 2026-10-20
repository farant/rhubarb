#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
#include <string.h>
static void hx(const char* tag, chorda r) { i32 k; printf("%s len=%u%s [", tag, r.mensura, r.datum ? "" : " NIHIL");
    for (k = 0; k < r.mensura; k++) printf("%02X", r.datum[k]); printf("]\n"); }
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    i8 hi[6] = {'A', (i8)0xC3, (i8)0x89, 0, (i8)0xFF, 'z'};
    i8 hi2[6] = {'a', (i8)0xC3, (i8)0x89, 0, (i8)0xFF, 'Z'};
    i8 e9[1] = {(i8)0xE9}; i8 c9[1] = {(i8)0xC9};
    chorda h, h2, a, b, en, ev, el[3];
    static i8 dummy[1] = {'q'};
    setvbuf(stdout, NIHIL, _IONBF, 0);
    h.datum = hi; h.mensura = 6; h2.datum = hi2; h2.mensura = 6;
    en.datum = NIHIL; en.mensura = 0; ev.datum = dummy; ev.mensura = 0;
    hx("minuscula A C3 89 00 FF z", chorda_minuscula(h, p));
    hx("maiuscula A C3 89 00 FF z", chorda_maiuscula(h, p));
    printf("case_insens(h,h2)=%d (expect 1)\n", chorda_aequalis_case_insensitivus(h, h2));
    a.datum = e9; a.mensura = 1; b.datum = c9; b.mensura = 1;
    printf("case_insens(0xE9,0xC9)=%d (ASCII-only expect 0)\n", chorda_aequalis_case_insensitivus(a, b));
    /* FNV-1a 32 oracle values computed by python */
    printf("friare(\"\")=%u friare(en)=%u friare(ev)=%u\n", chorda_friare(chorda_ex_literis("", p)), chorda_friare(en), chorda_friare(ev));
    printf("friare(\"hello\")=%u\n", chorda_friare(chorda_ex_literis("hello", p)));
    printf("friare(h)=%u\n", chorda_friare(h));
    printf("aequalis_literis(\"ab\\0\",\"ab\")=%d\n", (a.datum = (i8*)"ab\0", a.mensura = 3, chorda_aequalis_literis(a, "ab")));
    printf("aequalis_literis(en,\"\")=%d aequalis_literis(h,NIHIL)=%d\n", chorda_aequalis_literis(en, ""), chorda_aequalis_literis(h, NIHIL));
    printf("aequalis(en,ev)=%d\n", chorda_aequalis(en, ev));
    /* iungere */
    el[0] = chorda_ex_literis("a", p); el[1] = en; el[2] = chorda_ex_literis("c", p);
    hx("iungere [a,{0,NIHIL},c] sep ','", chorda_iungere(el, 3, chorda_ex_literis(",", p), p));
    hx("iungere [a] sep ','", chorda_iungere(el, 1, chorda_ex_literis(",", p), p));
    hx("iungere [en] sep ','", chorda_iungere(el + 1, 1, chorda_ex_literis(",", p), p));
    hx("iungere NIHIL,0", chorda_iungere(NIHIL, 0, chorda_ex_literis(",", p), p));
    hx("iungere [a,c] sep=el[0] aliased", chorda_iungere(el, 3, el[0], p));
    /* misc */
    hx("transcribere \"a\\0b\"", chorda_transcribere(chorda_ex_buffer((i8*)"a\0b", 3), p));
    hx("transcribere ev", chorda_transcribere(ev, p));
    hx("ex_buffer(NIHIL,5)", chorda_ex_buffer(NIHIL, 5));
    hx("ex_literis(\"\")", chorda_ex_literis("", p));
    hx("ex_literis(NIHIL)", chorda_ex_literis(NIHIL, p));
    hx("sectio(\"abc\",2,1)", chorda_sectio(chorda_ex_literis("abc", p), 2, 1));
    hx("sectio(\"abc\",3,3)", chorda_sectio(chorda_ex_literis("abc", p), 3, 3));
    hx("sectio(\"abc\",1,4)", chorda_sectio(chorda_ex_literis("abc", p), 1, 4));
    hx("sectio(en,0,0)", chorda_sectio(en, 0, 0));
    hx("character_ad(\"abc\",3)", chorda_character_ad(chorda_ex_literis("abc", p), 3, p));
    hx("character_ad(\"abc\",0xFFFFFFFF)", chorda_character_ad(chorda_ex_literis("abc", p), 0xFFFFFFFFU, p));
    hx("invertere C3 A9 (é)", chorda_invertere(chorda_ex_literis("\xc3\xa9", p), p));
    hx("duplicare(\"ab\",0)", chorda_duplicare(chorda_ex_literis("ab", p), 0, p));
    hx("duplicare(\"ab\",0xFFFFFFFF) [numerus<=0 on unsigned]", chorda_duplicare(en, 0xFFFFFFFFU, p));
    hx("concatenare(a, a) aliased", chorda_concatenare(el[0], el[0], p));
    {
        character* c = chorda_ut_cstr(chorda_ex_buffer((i8*)"a\0b", 3), p);
        printf("ut_cstr(\"a\\0b\") strlen=%lu ; ut_cstr(en)=%s ut_cstr(ev)=%s\n", (unsigned long)strlen(c),
            chorda_ut_cstr(en, p) ? "ptr" : "NIHIL", chorda_ut_cstr(ev, p) ? "ptr" : "NIHIL");
    }
    /* substituere edges */
    hx("substituere(aaa, aa, b)", chorda_substituere(chorda_ex_literis("aaa", p), chorda_ex_literis("aa", p), chorda_ex_literis("b", p), p));
    hx("substituere(aXa, a, aa)", chorda_substituere(chorda_ex_literis("aXa", p), chorda_ex_literis("a", p), chorda_ex_literis("aa", p), p));
    hx("substituere(aaa, a, '')", chorda_substituere(chorda_ex_literis("aaa", p), chorda_ex_literis("a", p), en, p));
    hx("substituere(abc, '', X)", chorda_substituere(chorda_ex_literis("abc", p), en, chorda_ex_literis("X", p), p));
    return 0;
}
