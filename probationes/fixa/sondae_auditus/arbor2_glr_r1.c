/* sonda r1: GLR_DEBUG 1 imprimit in stdout pro omni vocante */
#include "arbor2_glr_communia.inc"

integer
principale(vacuum)
{
    Piscina* p;
    InternamentumChorda* intern;
    Arbor2Expansion* ex;
    Arbor2GLR* glr;
    Arbor2GLRResultus res;

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    ex = arbor2_expansion_creare(p, intern);
    glr = arbor2_glr_creare(p, intern, ex);
    imprimere("=== INITIUM parsere 'a + b' ===\n");
    res = arbor2_glr_parsere_expressio(glr, _lexare(p, intern, "a + b"));
    imprimere("=== FINIS successus=%d ===\n", (integer)res.successus);
    redde ZEPHYRUM;
}
