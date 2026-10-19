#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

static chorda mkc(i8* b, i32 n) { chorda c; c.datum = b; c.mensura = n; return c; }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    i8 b1[] = {'a'}; i8 b2[] = {'a',0}; i8 b3[] = {'a',0,'b'}; i8 b4[] = {'a',0,'c'};
    i8 z1[] = {0}; i8 z2[] = {0,0};
    i8 hi1[] = {0xC0,0x80}; i8 hi2[] = {0xFF}; i8 hi3[] = {0x80};
    chorda *pa, *pa0, *pa0b, *pa0c, *pz1, *pz2, *ph1, *ph2, *ph3, *pA, *pl;
    i8 copy[3];
    pa = chorda_internare(in, mkc(b1,1));
    pa0 = chorda_internare(in, mkc(b2,2));
    pa0b = chorda_internare(in, mkc(b3,3));
    pa0c = chorda_internare(in, mkc(b4,3));
    pz1 = chorda_internare(in, mkc(z1,1));
    pz2 = chorda_internare(in, mkc(z2,2));
    printf("a=%p a0=%p a0b=%p a0c=%p distinct=%d\n",(void*)pa,(void*)pa0,(void*)pa0b,(void*)pa0c,
        pa!=pa0 && pa0!=pa0b && pa0b!=pa0c && pa!=pa0b && pa!=pa0c && pa0!=pa0c);
    printf("\\0 (len1)=%p \\0\\0=%p distinct=%d nonNIHIL=%d\n",(void*)pz1,(void*)pz2,pz1!=pz2, pz1!=NIHIL&&pz2!=NIHIL);
    printf("numerus=%u (expect 6)\n", internamentum_numerus(in));
    memcpy(copy, b3, 3);
    printf("a\\0b dup from other buffer same=%d\n", chorda_internare(in, mkc(copy,3)) == pa0b);
    printf("est_internata a\\0 = %d (1), a\\0\\0 = %d (0)\n",
        (int)chorda_est_internata(in, mkc(b2,2)), (int)chorda_est_internata(in, mkc(z2,1)) );
    { i8 ab[] = {'a','b'}; i8 ab0[] = {'a','b',0};
      chorda_internare(in, mkc(ab,2));
      printf("est ab=%d(1) est a(first byte of ab)=%d(1 since a interned) est ab\\0=%d(0)\n",
        (int)chorda_est_internata(in, mkc(ab,2)), (int)chorda_est_internata(in, mkc(ab,1)),
        (int)chorda_est_internata(in, mkc(ab0,3))); }
    /* ex_literis equivalence */
    pl = chorda_internare_ex_literis(in, "a");
    printf("ex_literis(\"a\")==internare(a,1): %d\n", pl == pa);
    pl = chorda_internare_ex_literis(in, "neuter");
    { i8 nb[] = {'n','e','u','t','e','r'};
      printf("internare after ex_literis(\"neuter\") same=%d\n", chorda_internare(in, mkc(nb,6)) == pl); }
    { i8 nb[] = {'v','e','r','b','u','m'}; chorda* q = chorda_internare(in, mkc(nb,6));
      printf("ex_literis after internare(\"verbum\") same=%d\n", chorda_internare_ex_literis(in, "verbum") == q); }
    /* high bytes both directions */
    ph1 = chorda_internare(in, mkc(hi1,2));
    ph2 = chorda_internare(in, mkc(hi2,1));
    ph3 = chorda_internare(in, mkc(hi3,1));
    printf("hi distinct=%d ex_literis(\\xC0\\x80)==%d ex_literis(\\xFF)==%d ex_literis(\\x80)==%d\n",
        ph1!=ph2&&ph2!=ph3&&ph1!=ph3,
        chorda_internare_ex_literis(in, "\xC0\x80")==ph1,
        chorda_internare_ex_literis(in, "\xFF")==ph2,
        chorda_internare_ex_literis(in, "\x80")==ph3);
    { chorda* e1 = chorda_internare_ex_literis(in, "\xE9t\xE9");
      i8 eb[] = {0xE9,'t',0xE9};
      printf("ex_literis first then internare high: %d\n", chorda_internare(in, mkc(eb,3)) == e1); }
    pA = chorda_internare_ex_literis(in, "A");
    printf("case: A != a %d\n", pA != pa);
    printf("numerus=%u\n", internamentum_numerus(in));
    piscina_destruere(p);
    return 0;
}
