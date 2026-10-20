/* sonda r2: nuntius nodi ERROR = primus error totius TU? */
#include "arbor2_glr_communia.inc"

hic_manens vacuum
_imprimere_chordam(constans character* titulus, chorda* c)
{
    si (c == NIHIL) { fprintf(stderr, "SONDA> %s: (NIHIL)\n", titulus); redde; }
    fprintf(stderr, "SONDA> %s: '%.*s'\n", titulus, (integer)c->mensura, (constans character*)c->datum);
}

integer
principale(vacuum)
{
    Piscina* p;
    InternamentumChorda* intern;
    Arbor2Expansion* ex;
    Arbor2GLR* glr;
    Arbor2GLRResultus res;
    Xar* decl;
    i32 i;
    constans character* fons = "int 5;\nint x = );\nint y = ] ;\n";

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    ex = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, ex);
    res = arbor2_glr_parsere_translation_unit(glr, _lexare(p, intern, fons));
    fprintf(stderr, "SONDA> successus=%d\n", (integer)res.successus);
    decl = res.radix->datum.translation_unit.declarationes;
    per (i = ZEPHYRUM; i < xar_numerus(decl); i++)
    {
        Arbor2Nodus* n = *(Arbor2Nodus**)xar_obtinere(decl, i);
        fprintf(stderr, "SONDA> decl %u genus=%s linea=%d\n", (insignatus integer)i,
            arbor2_nodus_genus_nomen(n->genus), (integer)n->linea_initium);
        si (n->genus == ARBOR2_NODUS_ERROR) _imprimere_chordam("  nuntius", n->datum.error.nuntius);
    }
    si (res.errores != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(res.errores); i++)
        {
            character tit[32];
            sprintf(tit, "errores[%u]", (insignatus integer)i);
            _imprimere_chordam(tit, *(chorda**)xar_obtinere(res.errores, i));
        }
    }
    redde ZEPHYRUM;
}
