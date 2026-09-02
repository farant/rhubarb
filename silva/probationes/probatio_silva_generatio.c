/* probatio_silva_generatio.c - Probationes generationum expansionis (Chunk B) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_expandere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Praeparare: lexare, definitiones colligere, expandere ad fixum */
interior Xar*
_expandere_fontem (
               Piscina*  piscina,
    constans character*  fons,
                   Xar** strata_out)
{
    SilvaExpansio* exp;
              Xar* lexemata;
              Xar* reliqua;

    exp = silva_expansio_creare(piscina);
    silva_fons_addere(exp, "probatio.c", FALSUM);
    lexemata = silva_lexare(piscina, fons, (i32)strlen(fons), ZEPHYRUM);
    reliqua = silva_expansio_directivas_processare(exp, lexemata,
        NIHIL);
    redde silva_expansio_expandere(exp, reliqua, strata_out);
}

interior SilvaToken*
_ad (
    Xar* lexemata,
    i32  i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_generatio",
        524288);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: expansio obiecti-similis + provenientia
     * ======================================================== */

    {
               Xar* exitus;
        SilvaToken* token;

        imprimere("\n--- Probans expansionem simplicem ---\n");

        /* int y = X ; -> int y = 42 ; */
        exitus = _expandere_fontem(piscina, "#define X 42\nint y = X;",
            NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), VI);
        token = _ad(exitus, III);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "42");

        /* provenientia: EXPANSIO, nomen X, radix = invocatio in fonte */
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_EXPANSIO);
        CREDO_CHORDA_AEQUALIS_LITERIS (*token->origo.datum.expansio.nomen_macro,
            "X");
        CREDO_AEQUALIS_I32 (silva_token_profunditas(token), I);
        CREDO_VERUM (silva_token_est_fons(silva_token_radix(token)));
        CREDO_CHORDA_AEQUALIS_LITERIS (silva_token_radix(token)->valor,
            "X");
    }


    /* ========================================================
     * PROBARE: expansio nidificata + strata
     * ======================================================== */

    {
               Xar* exitus;
               Xar* strata;
        SilvaToken* token;

        imprimere("\n--- Probans nidificationem et strata ---\n");

        exitus = _expandere_fontem(piscina,
            "#define A B\n#define B 7\nx = A;", &strata);
        token = _ad(exitus, II);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "7");
        CREDO_AEQUALIS_I32 (silva_token_profunditas(token), II);

        /* duae generationes mutantes: A->B, B->7 */
        CREDO_AEQUALIS_I32 (xar_numerus(strata), II);
    }


    /* ========================================================
     * PROBARE: recursio - caecationes terminant (S12)
     * ======================================================== */

    {
               Xar* exitus;
               Xar* strata;
        SilvaToken* token;

        imprimere("\n--- Probans recursionem caecatam ---\n");

        /* directa: A -> A, caeca post generationem I */
        exitus = _expandere_fontem(piscina, "#define A A\nx = A;",
            &strata);
        token = _ad(exitus, II);
        CREDO_AEQUALIS_I32 ((i32)token->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "A");
        CREDO_AEQUALIS_I32 (xar_numerus(strata), I);  /* UNA generatio mutans */

        /* mutua: A -> B -> A, terminat */
        exitus = _expandere_fontem(piscina,
            "#define A B\n#define B A\nx = A;", &strata);
        token = _ad(exitus, II);
        CREDO_AEQUALIS_I32 ((i32)token->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "A");
        CREDO_AEQUALIS_I32 (xar_numerus(strata), II);
        CREDO_AEQUALIS_I32 (silva_token_profunditas(token), II);
    }


    /* ========================================================
     * PROBARE: functio-similis + prae-expansio argumentorum (S13)
     * ======================================================== */

    {
               Xar* exitus;
        SilvaToken* token;

        imprimere("\n--- Probans functionem-similem ---\n");

        /* y = F(1, 2) ; -> y = 1 + 2 ; */
        exitus = _expandere_fontem(piscina,
            "#define F(a, b) a + b\ny = F(1, 2);", NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), VII);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, II)->valor, "1");
        CREDO_AEQUALIS_I32 ((i32)_ad(exitus, III)->genus,
            (i32)SILVA_LEX_PLUS);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, IV)->valor, "2");

        /* argumentum prae-expansum: F(X) -> 5 */
        exitus = _expandere_fontem(piscina,
            "#define X 5\n#define ID(a) a\ny = ID(X);", NIHIL);
        token = _ad(exitus, II);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "5");

        /* invocatio cum spatio: ID (9) invocat */
        exitus = _expandere_fontem(piscina,
            "#define ID(a) a\ny = ID (9);", NIHIL);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, II)->valor, "9");

        /* sine parametris */
        exitus = _expandere_fontem(piscina,
            "#define Z() 3\nw = Z();", NIHIL);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, II)->valor, "3");

        /* argumentum nidificatum in parenthesibus: F((1, 2)) unum arg */
        exitus = _expandere_fontem(piscina,
            "#define UNUM(a) a\ny = UNUM((1, 2));", NIHIL);
        CREDO_AEQUALIS_I32 ((i32)_ad(exitus, II)->genus,
            (i32)SILVA_LEX_PAREN_APERTA);
    }


    /* ========================================================
     * PROBARE: non-invocationes et robustitas
     * ======================================================== */

    {
        Xar* exitus;

        imprimere("\n--- Probans non-invocationes ---\n");

        /* F sine ( non invocatur */
        exitus = _expandere_fontem(piscina, "#define F(a) a\nint F;",
            NIHIL);
        CREDO_AEQUALIS_I32 ((i32)_ad(exitus, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, I)->valor, "F");

        /* invocatio imperfecta (EOF ante ')'): nomen manet, nullus fragor */
        exitus = _expandere_fontem(piscina, "#define F(a) a\ny = F(1",
            NIHIL);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, II)->valor, "F");
    }


    /* ========================================================
     * PROBARE: via latina - macro ad verbum clausum
     * ======================================================== */

    {
               Xar* exitus;
        SilvaToken* token;

        imprimere("\n--- Probans viam latinam ---\n");

        /* si -> if : genus IF, provenientia nomen "si" */
        exitus = _expandere_fontem(piscina,
            "#define si if\nsi (x) y;", NIHIL);
        token = _ad(exitus, 0);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_IF);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "if");
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_EXPANSIO);
        CREDO_CHORDA_AEQUALIS_LITERIS (*token->origo.datum.expansio.nomen_macro,
            "si");
        CREDO_CHORDA_AEQUALIS_LITERIS (silva_token_radix(token)->valor,
            "si");
    }


    /* ========================================================
     * PROBARE: macros praedefinita (__FILE__/__LINE__/__STDC__)
     * ======================================================== */

    {
               Xar* exitus;
        SilvaToken* token;

        imprimere("\n--- Probans praedefinita ---\n");

        /* __LINE__ directum: linea 1 et linea 2 */
        exitus = _expandere_fontem(piscina, "__LINE__;\n__LINE__;",
            NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), V);
        token = _ad(exitus, 0);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "1");
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "2");
        /* provenientia: origo CHORDA, radix = identificator fontis,
         * profunditas I - stratum expansum VERUM dicit */
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_CHORDA);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            silva_token_radix(token)->valor, "__LINE__");
        CREDO_AEQUALIS_I32 (silva_token_profunditas(token), I);

        /* __LINE__ intra corpus macro: linea INVOCATIONIS (C89 -
         * radix catenae originis) */
        exitus = _expandere_fontem(piscina,
            "#define UBI() __LINE__\n\nUBI();", NIHIL);
        token = _ad(exitus, 0);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "3");

        /* __FILE__: littera chordae cum via fontis */
        exitus  = _expandere_fontem(piscina, "__FILE__;", NIHIL);
        token   = _ad(exitus, 0);
        CREDO_AEQUALIS_I32 ((i32)token->genus,
            (i32)SILVA_LEX_STRING_LIT);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor,
            "\"probatio.c\"");

        /* __STDC__: 1 */
        exitus  = _expandere_fontem(piscina, "__STDC__;", NIHIL);
        token   = _ad(exitus, 0);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "1");

        /* tabula usoris vincit (redefinitio - UB C89, sed politica
         * nostra: numquam praedefinitum super macro usoris) */
        exitus = _expandere_fontem(piscina,
            "#define __LINE__ 99\n__LINE__;", NIHIL);
        token = _ad(exitus, 0);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "99");

        /* CREDO-simile: # + __FILE__ + __LINE__ simul (exemplar
         * credo.h - corpus barrae officinae) */
        exitus = _expandere_fontem(piscina,
            "#define ADFIRMA(x) notare(#x, __FILE__, __LINE__)\n"
            "ADFIRMA(a > 0);", NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), X);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, II)->valor,
            "\"a > 0\"");
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, IV)->valor,
            "\"probatio.c\"");
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(exitus, VI)->valor, "2");
    }


    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

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
