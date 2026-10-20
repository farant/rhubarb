/* sonda r3: ramus conditionalis errores TU exterioris delet */
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
    i32 i;
    i32 num_err_nodi = ZEPHYRUM;

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    ex = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, ex);
    res = arbor2_glr_parsere_translation_unit(glr, _lexare(p, intern, fons));
    decl = res.radix->datum.translation_unit.declarationes;
    per (i = ZEPHYRUM; i < xar_numerus(decl); i++)
    {
        Arbor2Nodus* n = *(Arbor2Nodus**)xar_obtinere(decl, i);
        fprintf(stderr, "SONDA>   decl %u genus=%s\n", (insignatus integer)i, arbor2_nodus_genus_nomen(n->genus));
        si (n->genus == ARBOR2_NODUS_ERROR) num_err_nodi++;
    }
    fprintf(stderr, "SONDA> fons=[%s] nodi ERROR=%u res.errores=%s (%u) glr->num_errores=%u\n",
        fons, (insignatus integer)num_err_nodi,
        res.errores == NIHIL ? "NIHIL" : "non NIHIL",
        res.errores == NIHIL ? 0u : (insignatus integer)xar_numerus(res.errores),
        (insignatus integer)glr->num_errores);
    piscina_destruere(p);
}

integer
principale(vacuum)
{
    _probare("int 5;\nint b;\n");
    _probare("int 5;\n#ifdef X\nint a;\n#endif\nint b;\n");
    _probare("int a;\n#ifdef X\nint 5;\n#endif\nint b;\n");
    redde ZEPHYRUM;
}
