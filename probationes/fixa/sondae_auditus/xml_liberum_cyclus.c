#include "xml_commune.h"
/* argv[1]: "reparens" | "se" | "avus" */
int main(int argc, char** argv)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(p);
    XmlNodus *a = xml_elementum_creare(p, it, "a");
    XmlNodus *b = xml_elementum_creare(p, it, "b");
    XmlNodus *c = xml_elementum_creare(p, it, "c");
    const char* m = argc > 1 ? argv[1] : "reparens";
    b32 rv; chorda s;
    if (strcmp(m, "reparens") == 0) {
        xml_liberum_addere(a, c);
        rv = xml_liberum_addere(b, c);
        printf("reparens addere -> %d; c->parens=%s; a liberi=%u b liberi=%u\n", (int)rv,
               c->parens == b ? "b" : (c->parens == a ? "a" : "?"),
               xml_numerus_liberorum(a), xml_numerus_liberorum(b));
        rv = xml_liberum_addere(b, c);
        printf("idem bis -> %d; b liberi=%u\n", (int)rv, xml_numerus_liberorum(b));
        s = xml_scribere(b, p, FALSUM); printf("b: "); sd_chorda(&s); printf("\n");
        return 0;
    }
    if (strcmp(m, "se") == 0) {
        rv = xml_liberum_addere(a, a);
        printf("se addere -> %d\n", (int)rv); fflush(stdout);
    } else {
        xml_liberum_addere(a, b);
        rv = xml_liberum_addere(b, a);
        printf("avus addere -> %d\n", (int)rv); fflush(stdout);
    }
    s = xml_scribere(a, p, FALSUM);
    printf("scriptum %u\n", s.mensura);
    return 0;
}
