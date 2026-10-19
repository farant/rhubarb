#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

static chorda ch(const char* s, int n) { chorda c; c.datum = (i8*)s; c.mensura = (i32)n; return c; }

int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    TabulaDispersa* t = tabula_dispersa_creare_chorda(p, 16);
    void* v; b32 r;
    char other[4];
    static const char hb[] = "\xC3\xA9t\xFF";

    /* empty keys */
    r = tabula_dispersa_inserere(t, ch(NULL, 0), (void*)7);
    printf("inserere {NIHIL,0} -> %d numerus=%u\n", r, t->numerus);
    r = tabula_dispersa_inserere(t, ch("x", 0), (void*)7);
    printf("inserere {\"x\",0} -> %d numerus=%u\n", r, t->numerus);
    printf("invenire {NIHIL,0} -> %d\n", tabula_dispersa_invenire(t, ch(NULL,0), &v));
    printf("continet_literis \"\" -> %d\n", tabula_dispersa_continet_literis(t, ""));
    printf("continet_literis NIHIL -> %d\n", tabula_dispersa_continet_literis(t, NULL));
    printf("delere {NIHIL,0} -> %d\n", tabula_dispersa_delere(t, ch(NULL,0)));

    /* prefix and NUL */
    tabula_dispersa_inserere(t, ch("ab", 2), (void*)1);
    tabula_dispersa_inserere(t, ch("abc", 3), (void*)2);
    tabula_dispersa_inserere(t, ch("a\0b", 3), (void*)3);
    tabula_dispersa_inserere(t, ch("a", 1), (void*)4);
    tabula_dispersa_inserere(t, ch("a\0c", 3), (void*)5);
    printf("numerus post 5 claves distinctas = %u (exsp 5)\n", t->numerus);
    v = 0; r = tabula_dispersa_invenire(t, ch("ab",2), &v); printf("ab -> %d %ld\n", r, (long)v);
    v = 0; r = tabula_dispersa_invenire(t, ch("abc",3), &v); printf("abc -> %d %ld\n", r, (long)v);
    v = 0; r = tabula_dispersa_invenire(t, ch("a\0b",3), &v); printf("a\\0b -> %d %ld\n", r, (long)v);
    v = 0; r = tabula_dispersa_invenire(t, ch("a\0c",3), &v); printf("a\\0c -> %d %ld\n", r, (long)v);
    v = 0; r = tabula_dispersa_invenire_literis(t, "a\0b", &v); printf("literis a\\0b (=\"a\") -> %d %ld\n", r, (long)v);
    v = 0; r = tabula_dispersa_invenire(t, ch("abcd",3), &v); printf("abcd/3 -> %d %ld\n", r, (long)v);
    /* same bytes different address */
    memcpy(other, "abc", 3);
    v = 0; r = tabula_dispersa_invenire(t, ch(other,3), &v); printf("abc alia sedes -> %d %ld\n", r, (long)v);

    /* high bytes via literis */
    tabula_dispersa_inserere(t, ch(hb, 4), (void*)6);
    v = 0; r = tabula_dispersa_invenire_literis(t, hb, &v); printf("literis altos octetos -> %d %ld\n", r, (long)v);

    /* NIHIL value */
    tabula_dispersa_inserere(t, ch("nil", 3), NULL);
    v = (void*)0x1234; r = tabula_dispersa_invenire(t, ch("nil",3), &v);
    printf("valor NIHIL: invenire -> %d v=%p continet=%d\n", r, v, tabula_dispersa_continet(t, ch("nil",3)));
    /* miss leaves out-param untouched? */
    v = (void*)0x1234; r = tabula_dispersa_invenire(t, ch("zz",2), &v);
    printf("miss: %d v=%p (non tactum)\n", r, v);
    /* duplicate delete / absent delete */
    printf("delere ab -> %d, iterum -> %d, absens -> %d; numerus=%u del=%u\n",
        tabula_dispersa_delere(t, ch("ab",2)), tabula_dispersa_delere(t, ch("ab",2)),
        tabula_dispersa_delere(t, ch("qq",2)), t->numerus, t->numerus_deletorum);

    /* fnv1a oracle */
    printf("fnv1a '' = %08x\n", tabula_friare_fnv1a(ch(NULL,0)));
    printf("fnv1a 'a' = %08x\n", tabula_friare_fnv1a(ch("a",1)));
    printf("fnv1a 'foobar' = %08x\n", tabula_friare_fnv1a(ch("foobar",6)));
    printf("fnv1a 'a\\0b' = %08x\n", tabula_friare_fnv1a(ch("a\0b",3)));
    printf("fnv1a hb = %08x\n", tabula_friare_fnv1a(ch(hb,4)));
    printf("djb2 'a' = %08x hb = %08x '' = %08x\n", tabula_friare_multiplicatio(ch("a",1)), tabula_friare_multiplicatio(ch(hb,4)), tabula_friare_multiplicatio(ch(NULL,0)));
    tabula_dispersa_status_imprimere(NULL);
    tabula_dispersa_status_imprimere(t);
    return 0;
}
