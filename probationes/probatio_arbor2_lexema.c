/* probatio_arbor2_lexema.c - Probationes Arbor2 Lexematis
 * Phase 2.7: NOVA_LINEA is now in spatia, not main stream. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Helper: check if spatia array contains NOVA_LINEA */
hic_manens b32
_habet_nova_linea(Xar* spatia)
{
    i32 i;
    Arbor2Lexema* spatium;

    si (spatia == NIHIL)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < xar_numerus(spatia); i++)
    {
        spatium = *(Arbor2Lexema**)xar_obtinere(spatia, i);
        si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

/* Helper: count NOVA_LINEA in spatia array */
hic_manens i32
_numerare_nova_linea(Xar* spatia)
{
    i32 i;
    i32 count;
    Arbor2Lexema* spatium;

    si (spatia == NIHIL)
    {
        redde ZEPHYRUM;
    }

    count = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(spatia); i++)
    {
        spatium = *(Arbor2Lexema**)xar_obtinere(spatia, i);
        si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            count++;
        }
    }

    redde count;
}

/* Helper: get first NOVA_LINEA from spatia array */
hic_manens Arbor2Lexema*
_obtinere_nova_linea(Xar* spatia)
{
    i32 i;
    Arbor2Lexema* spatium;

    si (spatia == NIHIL)
    {
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < xar_numerus(spatia); i++)
    {
        spatium = *(Arbor2Lexema**)xar_obtinere(spatia, i);
        si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            redde spatium;
        }
    }

    redde NIHIL;
}

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

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
     * PROBARE: NOVA_LINEA in spatia (Phase 2.7)
     * Newlines now appear in spatia_post of preceding token,
     * or spatia_ante of following token.
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans NOVA_LINEA in spatia ---\n");

        fons = "int x;\nint y;";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* int */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INT);

        /* x */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* ; - should have NOVA_LINEA in spatia_post */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_SEMICOLON);
        CREDO_VERUM(_habet_nova_linea(tok->spatia_post));

        /* int - should have NOVA_LINEA in spatia_ante (from previous line) */
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
     * PROBARE: Multiple newlines in spatia
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans newlines multiplices in spatia ---\n");

        fons = "a\n\nb";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* a - should have one NOVA_LINEA in spatia_post */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(_numerare_nova_linea(tok->spatia_post), I);

        /* b - should have one NOVA_LINEA in spatia_ante (the second newline) */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(_numerare_nova_linea(tok->spatia_ante), I);

        /* EOF */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_EOF);
    }


    /* ========================================================
     * PROBARE: Line continuation (\\\n) - still works
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

        /* x - continuation should be consumed, NO NOVA_LINEA token */
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
     * PROBARE: CONTINUATIO in spatia
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;
        Arbor2Lexema* spatium;
        b32 found_continuation;

        imprimere("\n--- Probans spatia CONTINUATIO ---\n");

        /* Test that continuation is captured in spatia (Phase 2.6) */
        fons = "a \\\n b";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* a - may have trailing spatia with continuation */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        found_continuation = FALSUM;

        /* Check trailing spatia of 'a' for CONTINUATIO token */
        si (tok->spatia_post != NIHIL && xar_numerus(tok->spatia_post) > ZEPHYRUM)
        {
            i32 t;
            per (t = ZEPHYRUM; t < xar_numerus(tok->spatia_post); t++)
            {
                spatium = *(Arbor2Lexema**)xar_obtinere(tok->spatia_post, t);
                si (spatium->genus == ARBOR2_LEXEMA_CONTINUATIO)
                {
                    found_continuation = VERUM;
                }
            }
        }

        /* b */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* Check leading spatia of 'b' for CONTINUATIO token */
        si (tok->spatia_ante != NIHIL && xar_numerus(tok->spatia_ante) > ZEPHYRUM)
        {
            i32 t;
            per (t = ZEPHYRUM; t < xar_numerus(tok->spatia_ante); t++)
            {
                spatium = *(Arbor2Lexema**)xar_obtinere(tok->spatia_ante, t);
                si (spatium->genus == ARBOR2_LEXEMA_CONTINUATIO)
                {
                    found_continuation = VERUM;
                }
            }
        }

        CREDO_AEQUALIS_I32((i32)found_continuation, VERUM);
    }


    /* ========================================================
     * PROBARE: CRLF handling in spatia
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;
        Arbor2Lexema* nl;

        imprimere("\n--- Probans CRLF handling ---\n");

        fons = "a\r\nb";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* a - should have NOVA_LINEA in spatia_post */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_VERUM(_habet_nova_linea(tok->spatia_post));

        /* Check that NOVA_LINEA has length 2 (\r\n) */
        nl = _obtinere_nova_linea(tok->spatia_post);
        CREDO_NON_NIHIL(nl);
        si (nl != NIHIL)
        {
            CREDO_AEQUALIS_I32(nl->longitudo, II);  /* \r\n is 2 bytes */
        }

        /* b */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
    }


    /* ========================================================
     * PROBARE: Preprocessor directive pattern
     * Now checks for NOVA_LINEA in spatia, not main stream
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

        /* 1 - should have NOVA_LINEA in spatia_post (marks end of directive) */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_VERUM(_habet_nova_linea(tok->spatia_post));

        /* int */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INT);
    }


    /* ========================================================
     * PROBARE: arbor2_lexema_omnia (Phase 2.7)
     * NOVA_LINEA no longer in main stream, so fewer tokens
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

        /* Should have: a, b, EOF = 3 tokens (NOVA_LINEA is in spatia) */
        CREDO_AEQUALIS_I32(xar_numerus(omnia), III);

        tok = *(Arbor2Lexema**)xar_obtinere(omnia, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        /* Check that 'a' has NOVA_LINEA in spatia_post */
        CREDO_VERUM(_habet_nova_linea(tok->spatia_post));

        tok = *(Arbor2Lexema**)xar_obtinere(omnia, I);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        tok = *(Arbor2Lexema**)xar_obtinere(omnia, II);
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


    /* ========================================================
     * PROBARE: Leading newlines (BOF case)
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        imprimere("\n--- Probans leading newlines ---\n");

        fons = "\n\nint x;";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* int - should have 2 NOVA_LINEA in spatia_ante */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INT);
        CREDO_AEQUALIS_I32(_numerare_nova_linea(tok->spatia_ante), II);
    }


    /* Compendium */
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
