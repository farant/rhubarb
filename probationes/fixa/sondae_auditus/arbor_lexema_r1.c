#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor_lexema.h"
#include <stdio.h>
#include <string.h>

interior vacuum
monstrare(constans character* titulus, constans character* fons)
{
    Piscina* p;
    InternamentumChorda* intern;
    ArborLexator* lex;
    Xar* omnia;
    i32 i;
    i32 n;
    ArborLexema** lp;

    p = piscina_generare_dynamicum("sonda", 65536);
    intern = internamentum_creare(p);
    lex = arbor_lexator_creare(p, intern, fons, (i32)strlen(fons));
    omnia = arbor_lexema_omnia(lex);
    n = xar_numerus(omnia);
    imprimere("%s: %d lexemata:", titulus, (integer)n);
    per (i = 0; i < n; i++)
    {
        lp = xar_obtinere(omnia, i);
        imprimere(" %s[%.*s]", arbor_lexema_genus_nomen((*lp)->genus),
                  (integer)(*lp)->valor.mensura,
                  (*lp)->valor.datum ? (constans character*)(*lp)->valor.datum : "");
    }
    imprimere("\n");
    piscina_destruere(p);
}

integer
principale(vacuum)
{
    monstrare("continuatio", "#define X 1 \\\n + 2\nint y;");
    monstrare("formfeed", "int a;\fint b;");
    monstrare("utf8", "int a; \xc3\xa9 int b;");
    monstrare("unum_punctum", "x = 1.;");
    monstrare("punctum_quinque", "x = .5;");
    monstrare("unum_punctum_f", "x = 1.f;");
    monstrare("octo", "x = 08;");
    monstrare("octalis_fractio", "x = 01.5;");
    monstrare("ctrl_tres_quattuor", "x = 3.14;");
    redde 0;
}
