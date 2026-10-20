/* sonda r1: regula cuius titulus = terminalis */
#include "latina.h"
#include "lapifex_generare.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include <stdio.h>

hic_manens constans character* G =
    "<grammatica>"
    "  <terminalia>"
    "    <terminalis titulus=\"A\" genus=\"G_A\"/>"
    "    <terminalis titulus=\"X\" genus=\"G_X\"/>"
    "    <terminalis titulus=\"EOF\" genus=\"G_EOF\"/>"
    "  </terminalia>"
    "  <regulae>"
    "    <regula titulus=\"s\"><productio>X</productio></regula>"
    "    <regula titulus=\"X\"><productio>A</productio></regula>"
    "  </regulae>"
    "  <initium>s</initium>"
    "</grammatica>";

integer principale(vacuum)
{
    Piscina* p = piscina_generare_dynamicum("sonda", 65536);
    InternamentumChorda* it = internamentum_creare(p);
    LapifexGrammatica* g = lapifex_grammaticam_legere(p, it, G);
    i32 i;
    si (!g) { imprimere("grammatica NIHIL (recusata)\n"); redde 0; }
    imprimere("grammatica accepta; terminalia=%u non_terminalia=%u\n",
        (unsigned)g->numerus_terminalium, (unsigned)g->numerus_non_terminalium);
    per (i = 0; i < (i32)xar_numerus(g->productiones); i++)
    {
        LapifexProductio* pr = (LapifexProductio*)xar_obtinere(g->productiones, i);
        LapifexSymbolum* s = (LapifexSymbolum*)xar_obtinere(g->symbola, (i32)pr->sinistrum);
        imprimere("prod %d: sinistrum=%.*s est_terminale=%d\n", (int)i,
            (int)s->titulus->mensura, (constans character*)s->titulus->datum,
            (int)s->est_terminale);
    }
    lapifex_first_computare(g);
    {
        Xar* f = lapifex_first_obtinere(g, 1);
        imprimere("FIRST(X terminalis) numerus=%u:", (unsigned)xar_numerus(f));
        per (i = 0; i < (i32)xar_numerus(f); i++)
            imprimere(" %d", (int)*(s32*)xar_obtinere(f, i));
        imprimere("\n");
    }
    {
        LapifexCollectio* c = lapifex_collectio_construere(g);
        LapifexTabula* t = c ? lapifex_tabulam_construere(c) : NIHIL;
        si (t) { imprimere("tabula conflictus=%d\n", (int)t->numerus_conflictuum); }
    }
    redde 0;
}
