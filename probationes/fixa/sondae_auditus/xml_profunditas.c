#include "xml_commune.h"
/* argv[1] = profunditas; argv[2] = "legere" | "scribere" */
int main(int argc, char** argv)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 1 << 20);
    InternamentumChorda* it = internamentum_creare(p);
    long d = argc > 1 ? atol(argv[1]) : 100000;
    long k;
    if (argc > 2 && strcmp(argv[2], "scribere") == 0) {
        XmlNodus *rad, *cur, *nov; chorda s;
        rad = cur = xml_elementum_creare(p, it, "a");
        for (k = 1; k < d; k++) { nov = xml_elementum_creare(p, it, "a"); xml_liberum_addere(cur, nov); cur = nov; }
        printf("arbor %ld aedificata; scribere...\n", d); fflush(stdout);
        s = xml_scribere(rad, p, FALSUM);
        printf("scriptum mensura %u\n", s.mensura);
        return 0;
    } else {
        char* buf = (char*)malloc((size_t)(d * 7));
        chorda in; XmlResultus r; long o = 0;
        for (k = 0; k < d; k++) { memcpy(buf + o, "<a>", 3); o += 3; }
        for (k = 0; k < d; k++) { memcpy(buf + o, "</a>", 4); o += 4; }
        in.datum = (i8*)buf; in.mensura = (i32)o;
        printf("legere profunditas %ld...\n", d); fflush(stdout);
        r = xml_legere(in, p, it);
        printf("succ=%d status=%d\n", (int)r.successus, (int)r.status);
        return 0;
    }
}
