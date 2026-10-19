#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "stml_macros.h"
#include <stdio.h>
#include <string.h>

static Piscina* pp;
static InternamentumChorda* it;

static void proba(const char* tag, const char* s)
{
    chorda in, w; StmlResultus r; StmlExpansioResultus e;
    in.datum = (i8*)(size_t)s; in.mensura = (i32)strlen(s);
    r = stml_legere(in, pp, it);
    printf("%s\tlegere=%d\n", tag, (int)r.successus);
    if (!r.successus) return;
    e = stml_expandere(r.radix, pp, it);
    printf("  successus=%d vitium=%d linea=%d", (int)e.successus, (int)e.vitium, (int)e.linea);
    if (e.radix_expansa) {
        w = stml_scribere(e.radix_expansa, pp, FALSUM);
        printf(" exitus=[%.*s]", (int)w.mensura, (char*)w.datum);
    }
    printf("\n");
}

#define PRIMA "<u><#tA><k n=\"a\"/></#><w><<#tA>></w></u>" \
              "<EXEMPLAR output=\"$mA\"><u><w><k n=\"$n\"/></w></u></EXEMPLAR>" \
              "<PER congruentia=\"$mA\"><hitA>&@n;</hitA></PER>"
#define SECUNDA "<v><#tB><k n=\"b\"/></#><w><<#tB>></w></v>" \
              "<EXEMPLAR output=\"$mB\"><v><w><k n=\"$n\"/></w></v></EXEMPLAR>" \
              "<PER congruentia=\"$mB\"><hitB>&@n;</hitB></PER>"
/* control: same second half but literal (no alias) */
#define SECUNDA_LIT "<v><w><k n=\"b\"/></w></v>" \
              "<EXEMPLAR output=\"$mB\"><v><w><k n=\"$n\"/></w></v></EXEMPLAR>" \
              "<PER congruentia=\"$mB\"><hitB>&@n;</hitB></PER>"

int main(void)
{
    pp = piscina_generare_dynamicum("sonda", 65536);
    it = internamentum_creare(pp);
    proba("PRIMA_SOLA", "<r>" PRIMA "</r>");
    proba("SECUNDA_SOLA", "<r>" SECUNDA "</r>");
    proba("AMBAE", "<r>" PRIMA SECUNDA "</r>");
    proba("AMBAE_LIT", "<r>" PRIMA SECUNDA_LIT "</r>");
    /* fragment tB defined before first resolution: both visible? */
    proba("TB_PRIUS", "<r><#tB><k n=\"b\"/></#>" PRIMA "<v><w><<#tB>></w></v>"
          "<EXEMPLAR output=\"$mB\"><v><w><k n=\"$n\"/></w></v></EXEMPLAR>"
          "<PER congruentia=\"$mB\"><hitB>&@n;</hitB></PER></r>");
    return 0;
}
