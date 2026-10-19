/* sonda r8: commentum fluitans post item in declarationes ponitur, scribere bis */
#include "arbor2_glr_communia.inc"

hic_manens vacuum
_probare(constans character* fons)
{
    Piscina* p;
    InternamentumChorda* intern;
    Arbor2Expansion* ex;
    Arbor2GLR* glr;
    Arbor2GLRResultus res;
    Xar* decl;
    chorda* o;
    i32 i;
    i32 k;

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    ex = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, ex);
    res = arbor2_glr_parsere_translation_unit(glr, _lexare(p, intern, fons));
    decl = res.radix->datum.translation_unit.declarationes;
    fprintf(stderr, "SONDA> fons=<");
    per (k = ZEPHYRUM; fons[k]; k++) fprintf(stderr, fons[k] == '\n' ? "\\n" : "%c", fons[k]);
    fprintf(stderr, "> ordo:");
    per (i = ZEPHYRUM; i < xar_numerus(decl); i++)
    {
        Arbor2Nodus* n = *(Arbor2Nodus**)xar_obtinere(decl, i);
        fprintf(stderr, " %s", arbor2_nodus_genus_nomen(n->genus));
        si (n->genus == ARBOR2_NODUS_COMMENTUM) fprintf(stderr, "(fluitans=%d)", (integer)n->datum.commentum.est_fluitans);
    }
    fprintf(stderr, "\n");
    o = arbor2_scribere(p, res.radix);
    fprintf(stderr, "SONDA>   aequale=%d scriptum=<", (integer)(o->mensura == (i32)strlen(fons) && memcmp(o->datum, fons, (size_t)o->mensura) == 0));
    per (k = ZEPHYRUM; k < o->mensura; k++) fprintf(stderr, o->datum[k] == '\n' ? "\\n" : "%c", (integer)o->datum[k]);
    fprintf(stderr, ">\n");
    piscina_destruere(p);
}

integer
principale(vacuum)
{
    _probare("int a;\n/* c */\nint b;\n");
    _probare("int a;\n\n/* c */\n\nint b;\n");
    _probare("int a;\n\n\n/* c */\n\n\nint b;\n");
    _probare("\n\n/* c */\n\nint b;\n");
    redde ZEPHYRUM;
}
