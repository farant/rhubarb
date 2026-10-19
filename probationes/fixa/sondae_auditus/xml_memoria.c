#include "xml_commune.h"
/* Piscina certae magnitudinis parva: XML_ERROR_MEMORIA promissum, an
 * processus exit()? argv[1] = mensura piscinae parsationis. */
int main(int argc, char** argv)
{
    long m = argc > 1 ? atol(argv[1]) : 2048;
    Piscina* pi = piscina_generare_dynamicum("intern", 65536);
    InternamentumChorda* it = internamentum_creare(pi);
    Piscina* p = piscina_generare_certae_magnitudinis("certa", (memoriae_index)m);
    char doc[8192]; int k, o = 0; XmlResultus r; chorda in;
    o += sprintf(doc + o, "<r>");
    for (k = 0; k < 200; k++) o += sprintf(doc + o, "<e%d k=\"v%d\">t%d</e%d>", k, k, k, k);
    o += sprintf(doc + o, "</r>");
    in.datum = (i8*)doc; in.mensura = (i32)o;
    printf("piscina %ld, doc %d bytes; legere...\n", m, o); fflush(stdout);
    r = xml_legere(in, p, it);
    printf("redditum: succ=%d status=%d radix=%p\n", (int)r.successus, (int)r.status, (void*)r.radix);
    return 0;
}
