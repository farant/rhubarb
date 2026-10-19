/* sonda r4: #ifdef sine #endif ramum ultimum amittit */
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
    chorda* ex_scr;
    i32 i;

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    ex = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, ex);
    res = arbor2_glr_parsere_translation_unit(glr, _lexare(p, intern, fons));
    fprintf(stderr, "SONDA> successus=%d errores=%s\n", (integer)res.successus,
        res.errores == NIHIL ? "NIHIL" : "non NIHIL");
    decl = res.radix->datum.translation_unit.declarationes;
    per (i = ZEPHYRUM; i < xar_numerus(decl); i++)
    {
        Arbor2Nodus* n = *(Arbor2Nodus**)xar_obtinere(decl, i);
        fprintf(stderr, "SONDA>   decl %u genus=%s", (insignatus integer)i, arbor2_nodus_genus_nomen(n->genus));
        si (n->genus == ARBOR2_NODUS_CONDITIONALIS)
            fprintf(stderr, " rami=%u", (insignatus integer)xar_numerus(n->datum.conditionalis.rami));
        fprintf(stderr, "\n");
    }
    ex_scr = arbor2_scribere(p, res.radix);
    fprintf(stderr, "SONDA> scriptum=<<%.*s>> (mensura %d, fons %d)\n", (integer)ex_scr->mensura,
        (constans character*)ex_scr->datum, (integer)ex_scr->mensura, (integer)strlen(fons));
    piscina_destruere(p);
}

integer
principale(vacuum)
{
    _probare("int z;\n#ifdef X\nint a;\n#endif\n");
    _probare("int z;\n#ifdef X\nint a;\n");
    _probare("int z;\n#ifdef X\nint a;\n#else\nint b;\n");
    redde ZEPHYRUM;
}
