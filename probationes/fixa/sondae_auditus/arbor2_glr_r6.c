/* sonda r6: '}' + ',' in summo gradu ut finis functionis tractatur */
#include "arbor2_glr_communia.inc"

hic_manens vacuum
_probare(constans character* fons, b32 tu)
{
    Piscina* p;
    InternamentumChorda* intern;
    Arbor2Expansion* ex;
    Arbor2GLR* glr;
    Arbor2GLRResultus res;
    i32 i;

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    ex = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, ex);
    si (!tu)
    {
        res = arbor2_glr_parsere(glr, _lexare(p, intern, fons));
        fprintf(stderr, "SONDA> parsere(%s): successus=%d radix=%s\n", fons, (integer)res.successus,
            res.radix ? arbor2_nodus_genus_nomen(res.radix->genus) : "NIHIL");
    }
    alioquin
    {
        Xar* decl;
        res = arbor2_glr_parsere_translation_unit(glr, _lexare(p, intern, fons));
        decl = res.radix->datum.translation_unit.declarationes;
        fprintf(stderr, "SONDA> translation_unit(%s): errores=%s decl=%u\n", fons,
            res.errores ? "non NIHIL" : "NIHIL", (insignatus integer)xar_numerus(decl));
        per (i = ZEPHYRUM; i < xar_numerus(decl); i++)
        {
            Arbor2Nodus* n = *(Arbor2Nodus**)xar_obtinere(decl, i);
            fprintf(stderr, "SONDA>   decl %u genus=%s\n", (insignatus integer)i, arbor2_nodus_genus_nomen(n->genus));
        }
    }
    piscina_destruere(p);
}

integer
principale(vacuum)
{
    _probare("int a[] = {1}, b[] = {2};", FALSUM);
    _probare("int a[] = {1}, b[] = {2};", VERUM);
    _probare("int a = 1, b = 2;", VERUM);
    _probare("int a[] = {1}, b;", VERUM);
    redde ZEPHYRUM;
}
