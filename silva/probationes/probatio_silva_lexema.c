/* probatio_silva_lexema.c - Probationes lexatoris totalis */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Xar*
_lexare (
               Piscina* piscina,
    constans character* fons)
{
    redde silva_lexare(piscina, fons, (i32)strlen(fons), ZEPHYRUM);
}

interior SilvaToken*
_ad (
    Xar* lexemata,
    i32  i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

interior chorda
_chorda_cruda (
    constans character* fons,
                   i32  mensura)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = fons;
    c.datum    = u.m;
    c.mensura  = mensura;
    redde c;
}

/* Fidelitas: emittere(lexare(x)) == x */
interior b32
_fidelis (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
       Xar* lexemata;
    chorda  emissum;
    chorda  original;

    lexemata  = silva_lexare(piscina, fons, mensura, ZEPHYRUM);
    emissum   = silva_lexemata_emittere(piscina, lexemata);
    original  = _chorda_cruda(fons, mensura);
    si (emissum.mensura != original.mensura)
    {
        redde FALSUM;
    }
    si (emissum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (memcmp(emissum.datum, original.datum,
        (memoriae_index)emissum.mensura) == ZEPHYRUM) ? VERUM : FALSUM;
}

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_lexema",
        262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: series generum fundamentalis
     * ======================================================== */

    {
        Xar* lexemata;

        imprimere("\n--- Probans seriem fundamentalem ---\n");

        lexemata = _lexare(piscina, "int x = 42;");
        CREDO_AEQUALIS_I32 (xar_numerus(lexemata), VI);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_INT);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, II)->genus,
            (i32)SILVA_LEX_ASSIGNATIO);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, III)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, IV)->genus,
            (i32)SILVA_LEX_SEMICOLON);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, V)->genus,
            (i32)SILVA_LEX_EOF);

        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(lexemata, I)->valor, "x");
        CREDO_AEQUALIS_I32 (_ad(lexemata, 0)->linea, I);
        CREDO_AEQUALIS_I32 (_ad(lexemata, I)->columna, V);
        CREDO_VERUM (silva_token_initium_lineae(_ad(lexemata, 0)));
        CREDO_FALSUM (silva_token_initium_lineae(_ad(lexemata, I)));
    }


    /* ========================================================
     * PROBARE: attachmentum triviae
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* a;
        SilvaToken* b;
        SilvaToken* c;

        imprimere("\n--- Probans attachmentum triviae ---\n");

        lexemata  = _lexare(piscina, "a b\n  c");
        a         = _ad(lexemata, 0);
        b         = _ad(lexemata, I);
        c         = _ad(lexemata, II);

        /* inter a et b: [SPATIA] - eadem linea, totum trailing */
        CREDO_VERUM (silva_token_post_numerus(a) > ZEPHYRUM);
        CREDO_AEQUALIS_I32 (silva_token_post_numerus(a), I);
        CREDO_AEQUALIS_I32 (silva_token_ante_numerus(b), ZEPHYRUM);

        /* inter b et c: [NOVA_LINEA, SPATIA] - trailing per novam
         * lineam inclusive, indentatio ad leading */
        CREDO_VERUM (silva_token_post_numerus(b) > ZEPHYRUM);
        CREDO_AEQUALIS_I32 (silva_token_post_numerus(b), I);
        CREDO_AEQUALIS_I32 ((i32)(silva_token_post(b, 0))->genus,
            (i32)SILVA_LEX_NOVA_LINEA);
        CREDO_VERUM (silva_token_ante_numerus(c) > ZEPHYRUM);
        CREDO_AEQUALIS_I32 (silva_token_ante_numerus(c), I);
        CREDO_AEQUALIS_I32 ((i32)(silva_token_ante(c, 0))->genus,
            (i32)SILVA_LEX_SPATIA);
        CREDO_VERUM (silva_token_initium_lineae(c));
    }


    /* ========================================================
     * PROBARE: commentum finis lineae ad trailing
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* x;
        SilvaToken* y;

        imprimere("\n--- Probans commentum finis lineae ---\n");

        lexemata  = _lexare(piscina, "x /* c */\ny");
        x         = _ad(lexemata, 0);
        y         = _ad(lexemata, I);

        /* x.post = [SPATIA, COMMENTUM, NOVA_LINEA]; y.ante = NIHIL */
        CREDO_VERUM (silva_token_post_numerus(x) > ZEPHYRUM);
        CREDO_AEQUALIS_I32 (silva_token_post_numerus(x), III);
        CREDO_AEQUALIS_I32 ((i32)(silva_token_post(x, I))->genus,
            (i32)SILVA_LEX_COMMENTUM_CLAUSUM);
        CREDO_AEQUALIS_I32 (silva_token_ante_numerus(y), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: CRLF - valor verbatim
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* a;
        SilvaToken* nl;

        imprimere("\n--- Probans CRLF ---\n");

        lexemata  = _lexare(piscina, "a\r\nb");
        a         = _ad(lexemata, 0);
        CREDO_VERUM (silva_token_post_numerus(a) > ZEPHYRUM);
        nl = silva_token_post(a, 0);
        CREDO_AEQUALIS_I32 ((i32)nl->genus, (i32)SILVA_LEX_NOVA_LINEA);
        CREDO_AEQUALIS_I32 (nl->valor.mensura, II);
        CREDO_CHORDA_AEQUALIS_LITERIS (nl->valor, "\r\n");
        CREDO_VERUM (silva_token_initium_lineae(_ad(lexemata, I)));
    }


    /* ========================================================
     * PROBARE: initium_lineae - linea LOGICA
     * ======================================================== */

    {
        Xar* lexemata;

        imprimere("\n--- Probans initium lineae logicae ---\n");

        /* '#' post lineam normalem: initium (lexemata: x = 1 # d EOF) */
        lexemata = _lexare(piscina, "x = 1\n# d");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, III)->genus,
            (i32)SILVA_LEX_CANCELLUM);
        CREDO_VERUM (silva_token_initium_lineae(_ad(lexemata, III)));

        /* '#' post continuationem: NON initium (linea logica continuat) */
        lexemata = _lexare(piscina, "x \\\n# y");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_CANCELLUM);
        CREDO_FALSUM (silva_token_initium_lineae(_ad(lexemata, I)));

        /* continuatio ante lexema primum: adhuc initium lineae logicae I */
        lexemata = _lexare(piscina, "\\\n#x");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_CANCELLUM);
        CREDO_VERUM (silva_token_initium_lineae(_ad(lexemata, 0)));
    }


    /* ========================================================
     * PROBARE: scissurae - laminae intra lexemata
     * ======================================================== */

    {
                  Xar* lexemata;
           SilvaToken* token;
        SilvaScissura* sc;

        imprimere("\n--- Probans scissuras ---\n");

        /* identificator laminatus */
        lexemata = _lexare(piscina, "ab\\\ncd");
        CREDO_AEQUALIS_I32 (xar_numerus(lexemata), II);  /* id + EOF */
        token = _ad(lexemata, 0);
        CREDO_AEQUALIS_I32 ((i32)token->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "abcd");
        CREDO_NON_NIHIL (silva_token_scissurae(token));
        CREDO_AEQUALIS_I32 (xar_numerus(silva_token_scissurae(token)),
            I);
        sc = (SilvaScissura*)xar_obtinere(silva_token_scissurae(token),
            0);
        CREDO_AEQUALIS_S32 (sc->offset, II);
        CREDO_FALSUM (sc->crlf);

        /* verbum clausum laminatum: textus mundus quaeritur */
        lexemata  = _lexare(piscina, "i\\\nnt x;");
        token     = _ad(lexemata, 0);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_INT);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "int");

        /* fidelitas cum laminis */
        CREDO_VERUM (_fidelis(piscina, "ab\\\ncd",
            (i32)strlen("ab\\\ncd")));
        CREDO_VERUM (_fidelis(piscina, "i\\\r\nnt x;",
            (i32)strlen("i\\\r\nnt x;")));
    }


    /* ========================================================
     * PROBARE: litterae imperfectae - numquam frangere
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* token;

        imprimere("\n--- Probans litteras imperfectas ---\n");

        lexemata  = _lexare(piscina, "\"abc");
        token     = _ad(lexemata, 0);
        CREDO_AEQUALIS_I32 ((i32)token->genus,
            (i32)SILVA_LEX_STRING_IMPERFECTUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "\"abc");

        lexemata = _lexare(piscina, "'a");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_CHARACTER_IMPERFECTUM);

        /* lexatio pergit post imperfectum */
        lexemata = _lexare(piscina, "\"ab\nx");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_STRING_IMPERFECTUM);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_VERUM (silva_token_initium_lineae(_ad(lexemata, I)));

        /* chorda perfecta cum effugio */
        lexemata = _lexare(piscina, "\"a\\\"b\"");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_STRING_LIT);
    }


    /* ========================================================
     * PROBARE: numeri - punctum initiale, suffixa, ellipsis
     * ======================================================== */

    {
        Xar* lexemata;

        imprimere("\n--- Probans numeros ---\n");

        lexemata = _lexare(piscina, ".5");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_FLOAT);

        lexemata = _lexare(piscina, "3.");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_FLOAT);

        lexemata = _lexare(piscina, "1.5e-3f");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_FLOAT);
        CREDO_AEQUALIS_I32 (_ad(lexemata, 0)->valor.mensura, VII);

        lexemata = _lexare(piscina, "0x1F 042 7UL");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, II)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 (_ad(lexemata, II)->valor.mensura, III);

        /* Cursus suffixorum avidus (M2d Chunk D): LL/ULL/llu UNUM
         * lexema - scissura vetus "12L"+"L" errores 8 in 5 plagulis
         * repositorii fecit (fasti, entitas, probationes 3) */
        lexemata = _lexare(piscina, "12LL 0x1FULL 42ull 7LLU");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 (_ad(lexemata, 0)->valor.mensura, IV);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 (_ad(lexemata, I)->valor.mensura, VII);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, II)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 (_ad(lexemata, II)->valor.mensura, V);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, III)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 (_ad(lexemata, III)->valor.mensura, IV);

        /* fluitans suffixum singulare manet (C89) */
        lexemata = _lexare(piscina, "1.5f");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_FLOAT);
        CREDO_AEQUALIS_I32 (_ad(lexemata, 0)->valor.mensura, IV);

        lexemata = _lexare(piscina, "...");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_ELLIPSIS);

        lexemata = _lexare(piscina, "..");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_PUNCTUM);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_PUNCTUM);
    }


    /* ========================================================
     * PROBARE: octeti ignoti - totalitas
     * ======================================================== */

    {
              Xar* lexemata;
        character  fons_nul[3];

        imprimere("\n--- Probans octetos ignotos ---\n");

        lexemata = _lexare(piscina, "@a");
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_OCTETUS_IGNOTUS);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);

        /* NUL in medio - chorda mensuram fert, non terminationem */
        fons_nul[0]   = 'a';
        fons_nul[I]   = '\0';
        fons_nul[II]  = 'b';
        lexemata      = silva_lexare(piscina, fons_nul, III, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_OCTETUS_IGNOTUS);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, II)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
    }


    /* ========================================================
     * PROBARE: commentum clausum non terminatum
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* ultimum;

        imprimere("\n--- Probans commentum non terminatum ---\n");

        lexemata  = _lexare(piscina, "x /* abc");
        ultimum   = _ad(lexemata, xar_numerus(lexemata) - I);
        CREDO_AEQUALIS_I32 ((i32)ultimum->genus, (i32)SILVA_LEX_EOF);
        CREDO_VERUM (_fidelis(piscina, "x /* abc",
            (i32)strlen("x /* abc")));
    }


    /* ========================================================
     * PROBARE: fidelitas octetorum - proprietas centralis
     * ======================================================== */

    {
        character fons_nul[5];

        imprimere("\n--- Probans fidelitatem octetorum ---\n");

        CREDO_VERUM (_fidelis(piscina, "", ZEPHYRUM));
        CREDO_VERUM (_fidelis(piscina,
            "int x = 42; /* c */\n\tfor (;;) x++;\n",
            (i32)strlen("int x = 42; /* c */\n\tfor (;;) x++;\n")));
        CREDO_VERUM (_fidelis(piscina, "a\r\nb\rc\nd",
            (i32)strlen("a\r\nb\rc\nd")));
        CREDO_VERUM (_fidelis(piscina, "x \\\n y \\\r\n z",
            (i32)strlen("x \\\n y \\\r\n z")));
        CREDO_VERUM (_fidelis(piscina, "@#$%^&*()!~`@@",
            (i32)strlen("@#$%^&*()!~`@@")));
        CREDO_VERUM (_fidelis(piscina, "\"imperfectum\n'x /* etiam",
            (i32)strlen("\"imperfectum\n'x /* etiam")));
        CREDO_VERUM (_fidelis(piscina, "// commentum lineae\nx",
            (i32)strlen("// commentum lineae\nx")));

        /* garbage cum NUL */
        fons_nul[0]    = (character)0x01;
        fons_nul[I]    = '\0';
        fons_nul[II]   = 'a';
        fons_nul[III]  = (character)0xFF;
        fons_nul[IV]   = '\n';
        CREDO_VERUM (_fidelis(piscina, fons_nul, V));
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
