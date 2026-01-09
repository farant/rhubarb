/* probatio_arbor2_lexema.c - Probationes Arbor2 Lexematis */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("probatio_arbor2_lexema", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: NOVA_LINEA emitted for newlines
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans NOVA_LINEA emission ---\n");

        fons = "int x;\nint y;";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* int */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INT);

        /* x */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* ; */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_SEMICOLON);

        /* \n -> NOVA_LINEA */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_NOVA_LINEA);

        /* int */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INT);

        /* y */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* ; */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_SEMICOLON);

        /* EOF */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_EOF);
    }


    /* ========================================================
     * PROBARE: Multiple newlines
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans newlines multiplices ---\n");

        fons = "a\n\nb";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* a */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* \n */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_NOVA_LINEA);

        /* \n */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_NOVA_LINEA);

        /* b */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* EOF */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_EOF);
    }


    /* ========================================================
     * PROBARE: Line continuation (\\\n)
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans line continuation ---\n");

        fons = "int \\\nx;";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* int */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INT);

        /* x - continuation should be consumed, NOT emit NOVA_LINEA */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* ; */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_SEMICOLON);

        /* EOF */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_EOF);
    }


    /* ========================================================
     * PROBARE: Trivia with continuation marked
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;
        Arbor2Trivia* trivia;
        b32 found_continuation;

        imprimere("\n--- Probans trivia est_continuatio ---\n");

        /* Test that continuation is captured in trivia */
        fons = "a \\\n b";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* a - may have trailing trivia with continuation */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        found_continuation = FALSUM;

        /* Check trailing trivia of 'a' for continuation */
        si (tok->trivia_post != NIHIL && xar_numerus(tok->trivia_post) > ZEPHYRUM)
        {
            i32 t;
            per (t = ZEPHYRUM; t < xar_numerus(tok->trivia_post); t++)
            {
                trivia = *(Arbor2Trivia**)xar_obtinere(tok->trivia_post, t);
                si (trivia->est_continuatio)
                {
                    found_continuation = VERUM;
                }
            }
        }

        /* b */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* Check leading trivia of 'b' for continuation */
        si (tok->trivia_ante != NIHIL && xar_numerus(tok->trivia_ante) > ZEPHYRUM)
        {
            i32 t;
            per (t = ZEPHYRUM; t < xar_numerus(tok->trivia_ante); t++)
            {
                trivia = *(Arbor2Trivia**)xar_obtinere(tok->trivia_ante, t);
                si (trivia->est_continuatio)
                {
                    found_continuation = VERUM;
                }
            }
        }

        CREDO_AEQUALIS_I32((i32)found_continuation, VERUM);
    }


    /* ========================================================
     * PROBARE: CRLF handling
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans CRLF handling ---\n");

        fons = "a\r\nb";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* a */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* \r\n -> single NOVA_LINEA */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_NOVA_LINEA);
        CREDO_AEQUALIS_I32(tok->longitudo, II);  /* \r\n is 2 bytes */

        /* b */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
    }


    /* ========================================================
     * PROBARE: Preprocessor directive pattern
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans preprocessor pattern ---\n");

        fons = "#define FOO 1\nint x;";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* # */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_HASH);

        /* define */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* FOO */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* 1 */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INTEGER);

        /* \n - marks end of directive */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_NOVA_LINEA);

        /* int */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INT);
    }


    /* ========================================================
     * PROBARE: arbor2_lexema_omnia
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Xar* omnia;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans arbor2_lexema_omnia ---\n");

        fons = "a\nb";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        omnia = arbor2_lexema_omnia(lex);

        /* Should have: a, \n, b, EOF = 4 tokens */
        CREDO_AEQUALIS_I32(xar_numerus(omnia), IV);

        tok = *(Arbor2Lexema**)xar_obtinere(omnia, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        tok = *(Arbor2Lexema**)xar_obtinere(omnia, I);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_NOVA_LINEA);

        tok = *(Arbor2Lexema**)xar_obtinere(omnia, II);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        tok = *(Arbor2Lexema**)xar_obtinere(omnia, III);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_EOF);
    }


    /* ========================================================
     * PROBARE: arbor2_lexema_genus_nomen
     * ======================================================== */

    {
        constans character* titulus;

        imprimere("\n--- Probans arbor2_lexema_genus_nomen ---\n");

        titulus = arbor2_lexema_genus_nomen(ARBOR2_LEXEMA_NOVA_LINEA);
        CREDO_AEQUALIS_I32((i32)strcmp(titulus, "NOVA_LINEA"), ZEPHYRUM);

        titulus = arbor2_lexema_genus_nomen(ARBOR2_LEXEMA_INT);
        CREDO_AEQUALIS_I32((i32)strcmp(titulus, "INT"), ZEPHYRUM);

        titulus = arbor2_lexema_genus_nomen(ARBOR2_LEXEMA_EOF);
        CREDO_AEQUALIS_I32((i32)strcmp(titulus, "EOF"), ZEPHYRUM);
    }


    /* Claudere credo */
    credo_claudere();
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
