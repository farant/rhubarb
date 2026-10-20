/* sonda r5: #if cum continuatione lineae - expressio truncata? */
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
    i32 r;

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    ex = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, ex);
    res = arbor2_glr_parsere_translation_unit(glr, _lexare(p, intern, fons));
    decl = res.radix->datum.translation_unit.declarationes;
    per (i = ZEPHYRUM; i < xar_numerus(decl); i++)
    {
        Arbor2Nodus* n = *(Arbor2Nodus**)xar_obtinere(decl, i);
        si (n->genus != ARBOR2_NODUS_CONDITIONALIS) perge;
        per (r = ZEPHYRUM; r < xar_numerus(n->datum.conditionalis.rami); r++)
        {
            Arbor2CondRamus* ra = *(Arbor2CondRamus**)xar_obtinere(n->datum.conditionalis.rami, r);
            fprintf(stderr, "SONDA> ramus %u genus=%d valor=%ld evaluatum=%d expr_toks=",
                (insignatus integer)r, (integer)ra->genus, (longus)ra->valor_evaluatus,
                (integer)ra->est_evaluatum);
            si (ra->expressio_lexemata != NIHIL)
            {
                i32 k;
                per (k = ZEPHYRUM; k < xar_numerus(ra->expressio_lexemata); k++)
                {
                    Arbor2Token* t = *(Arbor2Token**)xar_obtinere(ra->expressio_lexemata, k);
                    fprintf(stderr, "[%.*s]", (integer)t->lexema->valor.mensura,
                        (constans character*)t->lexema->valor.datum);
                }
            }
            fprintf(stderr, "\n");
        }
    }
    piscina_destruere(p);
}

integer
principale(vacuum)
{
    fprintf(stderr, "SONDA> casus sine continuatione: #if 0 || 1\n");
    _probare("#if 0 || 1\nint a;\n#endif\n");
    fprintf(stderr, "SONDA> casus cum continuatione: #if 0 \\\\ NL || 1\n");
    _probare("#if 0 \\\n || 1\nint a;\n#endif\n");
    fprintf(stderr, "SONDA> #elif cum continuatione: #if 0 / #elif 0 \\\\ NL || 1\n");
    _probare("#if 0\nint a;\n#elif 0 \\\n || 1\nint b;\n#endif\n");
    redde ZEPHYRUM;
}
