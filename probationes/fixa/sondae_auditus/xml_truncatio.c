#include "xml_commune.h"
/* Omnis praefixus documenti divitis in alveo malloc EXACTO (sine NUL):
 * ASan quamvis lectionem ultra mensuram capit. */
static const char* docs[] = {
    "<?xml version=\"1.0\"?>\n<!-- c --><!DOCTYPE plist PUBLIC \"x\" \"y\">\n"
    "<r a=\"1&amp;2&lt;&gt;&quot;&apos;\" b='q'>t&amp;x&lt;&gt;&quot;&apos;&#65;"
    "<s/><!-- k --><?pi z?><u v=\"w\">y</u ></r>",
    "<a x=\"&am",
    "<a>&quo",
    "<a><!--",
    "<!--",
    "<!DOCTYPE",
    "<a><?",
    "<?",
    "<a></a",
    "<a/",
    NULL
};
int main(void)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(p);
    int d; i32 n, len; int ok = 0, tot = 0;
    for (d = 0; docs[d]; d++) {
        len = (i32)strlen(docs[d]);
        for (n = 1; n <= len; n++) {
            chorda in; XmlResultus r; char* buf = (char*)malloc((size_t)n);
            memcpy(buf, docs[d], (size_t)n);
            in.datum = (i8*)buf; in.mensura = n;
            r = xml_legere(in, p, it);
            tot++; if (r.successus) ok++;
            if (n == len) printf("doc %d len %u: succ=%d status=%d\n", d, len, (int)r.successus, (int)r.status);
            free(buf);
        }
    }
    printf("praefixa %d, successus %d\n", tot, ok);
    return 0;
}
