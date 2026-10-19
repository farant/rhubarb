#include "xml_commune.h"
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(p);
    XmlResultus r;
    r = xml_legere_ex_literis("", p, it);
    printf("ex_literis \"\": succ=%d status=%d\n", (int)r.successus, (int)r.status);
    r = xml_legere_ex_literis("<a/>", NIHIL, it);
    printf("piscina NIHIL: status=%d\n", (int)r.status);
    r = xml_legere_ex_literis("<a/>", p, NIHIL);
    printf("intern NIHIL: status=%d\n", (int)r.status);
    fflush(stdout);
    printf("ex_literis(NIHIL) ...\n"); fflush(stdout);
    r = xml_legere_ex_literis(NIHIL, p, it);
    printf("ex_literis NIHIL: succ=%d status=%d\n", (int)r.successus, (int)r.status);
    return 0;
}
