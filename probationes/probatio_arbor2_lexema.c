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

/* ========================================================
 * PROBARE: NOVA_LINEA in spatia (Phase 2.7)
 * Newlines now appear in spatia_post of preceding token,
 * or spatia_ante of following token.
 * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

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
}

/* ========================================================
 * PROBARE: Multiple newlines in spatia
 * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

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
}

/* ========================================================
 * PROBARE: Line continuation (\\\n) - still works
 * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

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
}

/* ========================================================
 * PROBARE: CONTINUATIO in spatia
 * ======================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;
        Arbor2Lexema* spatium;
        b32 found_continuation;

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
}

/* ========================================================
 * PROBARE: CRLF handling in spatia
 * ======================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;
        Arbor2Lexema* nl;

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
}

/* ========================================================
 * PROBARE: Preprocessor directive pattern
 * Now checks for NOVA_LINEA in spatia, not main stream
 * ======================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

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
}

/* ========================================================
 * PROBARE: arbor2_lexema_omnia (Phase 2.7)
 * NOVA_LINEA no longer in main stream, so fewer tokens
 * ======================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Lexator* lex;
        Xar* omnia;
        Arbor2Lexema* tok;

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
}

/* ========================================================
 * PROBARE: arbor2_lexema_genus_nomen
 * ======================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* titulus;

        titulus = arbor2_lexema_genus_nomen(ARBOR2_LEXEMA_NOVA_LINEA);
        CREDO_AEQUALIS_I32((i32)strcmp(titulus, "NOVA_LINEA"), ZEPHYRUM);

        titulus = arbor2_lexema_genus_nomen(ARBOR2_LEXEMA_INT);
        CREDO_AEQUALIS_I32((i32)strcmp(titulus, "INT"), ZEPHYRUM);

        titulus = arbor2_lexema_genus_nomen(ARBOR2_LEXEMA_EOF);
        CREDO_AEQUALIS_I32((i32)strcmp(titulus, "EOF"), ZEPHYRUM);
    }
}

/* ========================================================
 * PROBARE: Leading newlines (BOF case)
 * ======================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Lexator* lex;
        Arbor2Lexema* tok;

        fons = "\n\nint x;";
        lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

        /* int - should have 2 NOVA_LINEA in spatia_ante */
        tok = arbor2_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR2_LEXEMA_INT);
        CREDO_AEQUALIS_I32(_numerare_nova_linea(tok->spatia_ante), II);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "NOVA_LINEA in spatia", _sectio_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "newlines multiplices in spatia", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "line continuation", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "spatia CONTINUATIO", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "CRLF handling", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "preprocessor pattern", _sectio_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "arbor2_lexema_omnia", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "arbor2_lexema_genus_nomen", _sectio_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "leading newlines", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("arbor2_lexema", SECTIONES);
}
