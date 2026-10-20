#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include <stdio.h>
static chorda lit(const char* c, Piscina* p) { return chorda_ex_literis(c, p); }
int main(void) {
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    chorda f, a, r, ev, en, sp;
    static i8 dummy[1] = {'z'};
    setvbuf(stdout, NIHIL, _IONBF, 0);
    ev.datum = dummy; ev.mensura = 0;      /* empty, valid ptr */
    en.datum = NIHIL; en.mensura = 0;      /* empty, NIHIL */
    f = lit("aaa", p); a = lit("aa", p);
    printf("invenire_ultimum_index(aaa,aa)=%d (expect 1)\n", chorda_invenire_ultimum_index(f, a));
    r = chorda_invenire_ultimum(f, a);
    printf("invenire_ultimum(aaa,aa) off=%ld len=%u (expect 1,2)\n", (long)(r.datum - f.datum), r.mensura);
    printf("invenire_index(aaab,aab)=%d (expect 1)\n", chorda_invenire_index(lit("aaab",p), lit("aab",p)));
    printf("numerare(aaaa,aa)=%u\n", chorda_numerare_occurrentia(lit("aaaa",p), lit("aa",p)));
    printf("invenire_ultimum_index(a,a)=%d (expect 0)\n", chorda_invenire_ultimum_index(lit("a",p), lit("a",p)));
    printf("invenire_ultimum_index(ab,abc)=%d (expect -1)\n", chorda_invenire_ultimum_index(lit("ab",p), lit("abc",p)));
    printf("continet(ab,abc)=%d incipit(ab,abc)=%d terminatur(ab,abc)=%d\n",
        chorda_continet(lit("ab",p), lit("abc",p)), chorda_incipit(lit("ab",p), lit("abc",p)), chorda_terminatur(lit("ab",p), lit("abc",p)));
    /* empty acus: two representations */
    f = lit("abc", p);
    r = chorda_invenire(f, ev);
    printf("acus {0,ptr}: invenire len=%u off=%ld  index=%d  ultimum_index=%d  ultimum off=%ld len=%u continet=%d incipit=%d terminatur=%d numerare=%u\n",
        r.mensura, (long)(r.datum - f.datum), chorda_invenire_index(f, ev), chorda_invenire_ultimum_index(f, ev),
        (long)(chorda_invenire_ultimum(f, ev).datum - f.datum), chorda_invenire_ultimum(f, ev).mensura,
        chorda_continet(f, ev), chorda_incipit(f, ev), chorda_terminatur(f, ev), chorda_numerare_occurrentia(f, ev));
    r = chorda_invenire(f, en);
    printf("acus {0,NIHIL}: invenire len=%u datum=%s  index=%d  ultimum_index=%d continet=%d incipit=%d terminatur=%d\n",
        r.mensura, r.datum ? "ptr" : "NIHIL", chorda_invenire_index(f, en), chorda_invenire_ultimum_index(f, en),
        chorda_continet(f, en), chorda_incipit(f, en), chorda_terminatur(f, en));
    printf("chorda_vacua(ev)=%d chorda_vacua(en)=%d aequalis(ev,en)=%d\n", chorda_vacua(ev), chorda_vacua(en), chorda_aequalis(ev, en));
    /* realistic: empty produced by praecidere vs ex_literis("") */
    sp = chorda_praecidere(lit("   ", p));
    printf("praecidere(\"   \") len=%u datum=%s ; ex_literis(\"\") datum=%s\n", sp.mensura, sp.datum ? "ptr" : "NIHIL",
        lit("", p).datum ? "ptr" : "NIHIL");
    printf("continet(\"abc\", praecidere(\"   \"))=%d  continet(\"abc\", ex_literis(\"\"))=%d\n",
        chorda_continet(f, sp), chorda_continet(f, lit("", p)));
    printf("incipit(\"abc\", praecidere(\"   \"))=%d  incipit(\"abc\", ex_literis(\"\"))=%d\n",
        chorda_incipit(f, sp), chorda_incipit(f, lit("", p)));
    /* empty fenum, empty acus */
    printf("continet({0,ptr},{0,ptr})=%d incipit({0,NIHIL},{0,ptr})=%d\n", chorda_continet(ev, ev), chorda_incipit(en, ev));
    /* comparare */
    {
        i8 x80[1] = {0x80}; i8 x7f[1] = {0x7f};
        chorda c80, c7f; c80.datum = x80; c80.mensura = 1; c7f.datum = x7f; c7f.mensura = 1;
        printf("comparare(0x80,0x7f)=%d (unsigned: >0)\n", chorda_comparare(c80, c7f));
        printf("comparare(ab,abc)=%d comparare(en,ev)=%d comparare(en,en)=%d\n", chorda_comparare(lit("ab",p), lit("abc",p)),
            chorda_comparare(en, ev), chorda_comparare(en, en));
    }
    return 0;
}
