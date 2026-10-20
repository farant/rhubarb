#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "arbor2_expandere.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char* s = "#define X 1\n\nint a = X;\n\n#undef X\n\nint b;\n\n#define Y 2\n\nint c;\n";
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* in = internamentum_creare(p);
    Arbor2Expansion* e = arbor2_expansion_creare(p, in);
    Xar* segs; int k, n;
    chorda cx, cy;
    cx = chorda_ex_literis("X", p); cy = chorda_ex_literis("Y", p);
    arbor2_expansion_processare(e, s, (i32)strlen(s), "t.c");
    segs = arbor2_expansion_obtinere_segmenta(e);
    n = (int)xar_numerus(segs);
    for (k = 0; k < n; k++) {
        Arbor2Segmentum* g = *(Arbor2Segmentum**)xar_obtinere(segs, (i32)k);
        printf("seg %d: lines [%u,%d) macros=%p X=%d Y=%d\n", k, (unsigned)g->linea_initium, (int)g->linea_finis,
            (void*)g->macros, tabula_dispersa_continet(g->macros, cx) ? 1 : 0, tabula_dispersa_continet(g->macros, cy) ? 1 : 0);
    }
    for (k = 1; k <= 11; k++) {
        TabulaDispersa* t = arbor2_expansion_macros_ad_lineam(e, (i32)k);
        printf("ad_lineam(%d): X=%d Y=%d\n", k, tabula_dispersa_continet(t, cx) ? 1 : 0, tabula_dispersa_continet(t, cy) ? 1 : 0);
    }
    piscina_destruere(p);
    return 0;
}
