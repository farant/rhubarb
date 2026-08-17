/* probatio_silva_unitates.c - scansor unitatum supremarum
 * (mechanismus communis, extractus ex amalgamatore 2026-08-17;
 * porta extractionis = amalgama byte-identicum, HAEC probatio =
 * mores scansoris directi: tituli, vexilla, limites contigui,
 * directivae titulatae).
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_unitates.h"
#include "credo.h"

#include <stdio.h>
#include <string.h>

interior Xar*
_scandere_literis (Piscina* piscina, constans character* fons);

interior Xar*
_scandere_literis (Piscina* piscina, constans character* fons)
{
    Xar* lexemata = silva_lexare(piscina, fons,
        (i32)strlen(fons), ZEPHYRUM);

    si (lexemata == NIHIL)
    {
        redde NIHIL;
    }
    redde silva_unitates_scandere(piscina, lexemata);
}

interior SilvaUnitas*
_unitas (Xar* unitates, i32 index);

interior SilvaUnitas*
_unitas (Xar* unitates, i32 index)
{
    redde (SilvaUnitas*)xar_obtinere(unitates, index);
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_unitatum",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans functiones (definitio + prototypum) ---\n");
    {
        Xar* u = _scandere_literis(piscina,
            "int adde(int a, int b)\n"
            "{\n"
            "    return a + b;\n"
            "}\n"
            "int minue(int a, int b);\n");

        CREDO_NON_NIHIL(u);
        CREDO_AEQUALIS_I32((i32)xar_numerus(u), (i32)2);
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 0)->titulus,
            "adde");
        CREDO_VERUM(_unitas(u, 0)->est_functio);
        CREDO_VERUM(_unitas(u, 0)->est_definitio);
        CREDO_FALSUM(_unitas(u, 0)->est_statica);
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 1)->titulus,
            "minue");
        CREDO_VERUM(_unitas(u, 1)->est_functio);
        CREDO_FALSUM(_unitas(u, 1)->est_definitio);
        /* limites contigui: unitas sequens ubi prior desiit */
        CREDO_AEQUALIS_I32(_unitas(u, 1)->lexema_primum,
            _unitas(u, 0)->lexema_finis);
    }

    imprimere("\n--- Probans staticam Latinam + typedef indicis ---\n");
    {
        Xar* u = _scandere_literis(piscina,
            "interior int _celata(void) { return 1; }\n"
            "nomen int (*Vocator)(int);\n"
            "structura Res { int x; };\n");

        CREDO_NON_NIHIL(u);
        CREDO_AEQUALIS_I32((i32)xar_numerus(u), (i32)3);
        CREDO_VERUM(_unitas(u, 0)->est_statica);
        CREDO_VERUM(_unitas(u, 0)->est_definitio);
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 1)->titulus,
            "Vocator");
        CREDO_VERUM(_unitas(u, 1)->est_typedef);
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 2)->titulus,
            "Res");
        CREDO_VERUM(_unitas(u, 2)->est_tag_def);
    }

    imprimere("\n--- Probans directivas titulatas ---\n");
    {
        Xar* u = _scandere_literis(piscina,
            "#define MAXIMUS 10\n"
            "#include \"chorda.h\"\n"
            "#include <stdio.h>\n"
            "#ifndef CUSTOS\n"
            "#endif\n");

        CREDO_NON_NIHIL(u);
        CREDO_AEQUALIS_I32((i32)xar_numerus(u), (i32)5);
        CREDO_VERUM(_unitas(u, 0)->est_directiva);
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 0)->titulus,
            "MAXIMUS");
        CREDO_VERUM(_unitas(u, 1)->est_directiva);
        CREDO_CHORDA_CONTINET(_unitas(u, 1)->titulus,
            chorda_ex_literis("chorda.h", piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 2)->titulus,
            "<stdio.h>");
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 3)->titulus,
            "CUSTOS");
        /* #endif sine titulo - unitas tamen */
        CREDO_VERUM(_unitas(u, 4)->est_directiva);
        CREDO_AEQUALIS_I32(_unitas(u, 4)->titulus.mensura,
            (i32)0);
    }

    imprimere("\n--- Probans datum cum initiatore ---\n");
    {
        Xar* u = _scandere_literis(piscina,
            "int tabula[4] = { 1, 2, 3, 4 };\n"
            "static const char* salve = \"salve\";\n");

        CREDO_NON_NIHIL(u);
        CREDO_AEQUALIS_I32((i32)xar_numerus(u), (i32)2);
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 0)->titulus,
            "tabula");
        CREDO_FALSUM(_unitas(u, 0)->est_functio);
        CREDO_CHORDA_AEQUALIS_LITERIS(_unitas(u, 1)->titulus,
            "salve");
        CREDO_VERUM(_unitas(u, 1)->est_statica);
    }

    imprimere("\n--- Probans vacuum ---\n");
    {
        Xar* u = _scandere_literis(piscina, "");

        CREDO_NON_NIHIL(u);
        CREDO_AEQUALIS_I32((i32)xar_numerus(u), (i32)0);
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
