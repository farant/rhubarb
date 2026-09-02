/* probatio_silva_pasta.c - Probationes # et ## (Chunk C) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_expandere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Xar*
_expandere_fontem (
               Piscina* piscina,
    constans character* fons)
{
    SilvaExpansio* exp;
              Xar* lexemata;
              Xar* reliqua;

    exp = silva_expansio_creare(piscina);
    silva_fons_addere(exp, "probatio.c", FALSUM);
    lexemata = silva_lexare(piscina, fons, (i32)strlen(fons), ZEPHYRUM);
    reliqua = silva_expansio_directivas_processare(exp, lexemata,
        NIHIL);
    redde silva_expansio_expandere(exp, reliqua, NIHIL);
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

    piscina = piscina_generare_dynamicum("probatio_silva_pasta",
        524288);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: stringificatio (#)
     * ======================================================== */

    {
               Xar* exitus;
        SilvaToken* token;

        imprimere("\n--- Probans stringificationem ---\n");

        /* S(abc) -> "abc" */
        exitus = _expandere_fontem(piscina,
            "#define S(x) #x\ns = S(abc);");
        token = _ad(exitus, II);
        CREDO_AEQUALIS_I32 ((i32)token->genus,
            (i32)SILVA_LEX_STRING_LIT);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "\"abc\"");
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_CHORDA);

        /* cursus albi -> unum spatium */
        exitus = _expandere_fontem(piscina,
            "#define S(x) #x\ns = S(1   +  2);");
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "\"1 + 2\"");

        /* CRUDUM: argumentum NON expanditur */
        exitus = _expandere_fontem(piscina,
            "#define X 5\n#define S(x) #x\ns = S(X);");
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "\"X\"");

        /* effugium: " et \\ in litteris */
        exitus = _expandere_fontem(piscina,
            "#define S(x) #x\ns = S(\"q\");");
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "\"\\\"q\\\"\"");
    }


    /* ========================================================
     * PROBARE: pasta (##)
     * ======================================================== */

    {
               Xar* exitus;
        SilvaToken* token;

        imprimere("\n--- Probans pastam ---\n");

        /* GLUE(sil, va) -> silva */
        exitus = _expandere_fontem(piscina,
            "#define GLUE(a, b) a ## b\nn = GLUE(sil, va);");
        token = _ad(exitus, II);
        CREDO_AEQUALIS_I32 ((i32)token->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "silva");
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_PASTA);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            token->origo.datum.pasta.sinister->valor, "sil");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            token->origo.datum.pasta.dexter->valor, "va");

        /* operanda CRUDA: P(X) -> X7, non 57 */
        exitus = _expandere_fontem(piscina,
            "#define X 5\n#define P(a) a ## 7\nn = P(X);");
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "X7");

        /* operator factus: << ex < ## < */
        exitus = _expandere_fontem(piscina,
            "#define OP(a, b) a ## b\nn = OP(<, <);");
        token = _ad(exitus, II);
        CREDO_AEQUALIS_I32 ((i32)token->genus,
            (i32)SILVA_LEX_SINISTRORSUM);

        /* catena: TRI(x, y, z) -> xyz */
        exitus = _expandere_fontem(piscina,
            "#define TRI(a, b, c) a ## b ## c\nn = TRI(x, y, z);");
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "xyz");

        /* obiectum-simile: pre ## fix -> prefix */
        exitus = _expandere_fontem(piscina,
            "#define K pre ## fix\nn = K;");
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "prefix");

        /* latus vacuum: E() -> x solum */
        exitus = _expandere_fontem(piscina,
            "#define E(a) x ## a\nn = E();");
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "x");
        CREDO_AEQUALIS_I32 ((i32)_ad(exitus, III)->genus,
            (i32)SILVA_LEX_SEMICOLON);

        /* ORDO-SENSITIVITAS (debitum Chunk C): operanda multi-lexematum.
         * MT(a b, c d) -> a b##c d -> a bc d: lexemata circumdantia
         * involvuntur ex_expansione, lexema pasta transmittitur - ordo
         * emissionis et provenientia AMBO probantur */
        exitus = _expandere_fontem(piscina,
            "#define MT(x, y) x ## y\nn = MT(a b, c d);");
        /* n = a bc d ; EOF */
        token = _ad(exitus, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "a");
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_EXPANSIO);
        token = _ad(exitus, III);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "bc");
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_PASTA);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            token->origo.datum.pasta.sinister->valor, "b");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            token->origo.datum.pasta.dexter->valor, "c");
        token = _ad(exitus, IV);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "d");
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_EXPANSIO);
        CREDO_AEQUALIS_I32 ((i32)_ad(exitus, V)->genus,
            (i32)SILVA_LEX_SEMICOLON);
    }


    /* ========================================================
     * PROBARE: pasta invalida - best effort, nullus fragor
     * ======================================================== */

    {
        Xar* exitus;

        imprimere("\n--- Probans pastam invalidam ---\n");

        /* "1" ## "+" -> "1+" relexatur: INTEGER PLUS (duo lexemata) */
        exitus = _expandere_fontem(piscina,
            "#define BAD(a) a ## +\nn = BAD(1);");
        CREDO_AEQUALIS_I32 ((i32)_ad(exitus, II)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(exitus, III)->genus,
            (i32)SILVA_LEX_PLUS);
    }


    /* ========================================================
     * PROBARE: pasta deinde rescan - generationes cooperantur
     * ======================================================== */

    {
               Xar* exitus;
        SilvaToken* token;

        imprimere("\n--- Probans pastam cum rescan ---\n");

        /* J(A, B) -> AB (pasta) -> 9 (generatio sequens) */
        exitus = _expandere_fontem(piscina,
            "#define AB 9\n#define J(a, b) a ## b\nn = J(A, B);");
        token = _ad(exitus, II);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "9");
        /* provenientia: 9 <- expansio AB <- pasta */
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus,
            (i32)SILVA_ORIGO_EXPANSIO);
        CREDO_AEQUALIS_I32 (
            (i32)token->origo.datum.expansio.invocatio->origo.genus,
            (i32)SILVA_ORIGO_PASTA);
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
