/* sonda r7: P17 lista CORPUS communis inter furcas - sententiae duplicatae? */
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

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    ex = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, ex);
    res = arbor2_glr_parsere_translation_unit(glr, _lexare(p, intern, fons));
    decl = res.radix->datum.translation_unit.declarationes;
    fprintf(stderr, "SONDA> fons=<%s> furcae=%d mergae=%d errores=%s\n", fons,
        (integer)glr->num_furcae, (integer)glr->num_mergae, res.errores ? "non NIHIL" : "NIHIL");
    per (i = ZEPHYRUM; i < xar_numerus(decl); i++)
    {
        Arbor2Nodus* n = *(Arbor2Nodus**)xar_obtinere(decl, i);
        fprintf(stderr, "SONDA>   decl %u genus=%s", (insignatus integer)i, arbor2_nodus_genus_nomen(n->genus));
        si (n->genus == ARBOR2_NODUS_DEFINITIO_FUNCTI && n->datum.definitio_functi.corpus != NIHIL)
        {
            Arbor2Nodus* c = n->datum.definitio_functi.corpus;
            i32 k;
            fprintf(stderr, " corpus genus=%s", arbor2_nodus_genus_nomen(c->genus));
            si (c->genus == ARBOR2_NODUS_CORPUS && c->datum.corpus.sententiae != NIHIL)
            {
                fprintf(stderr, " sententiae=%u:", (insignatus integer)xar_numerus(c->datum.corpus.sententiae));
                per (k = ZEPHYRUM; k < xar_numerus(c->datum.corpus.sententiae); k++)
                {
                    Arbor2Nodus* s = *(Arbor2Nodus**)xar_obtinere(c->datum.corpus.sententiae, k);
                    fprintf(stderr, " %s", s ? arbor2_nodus_genus_nomen(s->genus) : "NIHIL");
                }
            }
        }
        fprintf(stderr, "\n");
    }
    {
        chorda* o = arbor2_scribere(p, res.radix);
        fprintf(stderr, "SONDA>   scriptum aequale fonti: %d\n",
            (integer)(o->mensura == (i32)strlen(fons) && memcmp(o->datum, fons, (size_t)o->mensura) == 0));
        fprintf(stderr, "SONDA>   scriptum=<%.*s>\n", (integer)o->mensura, (constans character*)o->datum);
    }
    piscina_destruere(p);
}

integer
principale(vacuum)
{
    _probare("void f() { x; }");
    _probare("void f() { a * b; }");
    _probare("void f() { x; a * b; y; }");
    _probare("void f() { a (b); }");
    _probare("void f() { x; a * b; a * c; }");
    _probare("void f() { if (x) { a * b; } }");
    redde ZEPHYRUM;
}
