/* probatio_silva_compat_lexema.c - Vectis compatibilitatis: arbor2 lexema
 *
 * Portatum ex probationes/probatio_arbor2_lexema.c (41 assertiones) -
 * "carry the tests": casus difficulter inventi generationis prioris.
 * API mutata (silva_lexare = fluxus integer, non fluens), semantica
 * eadem. Sectiones ordine originali.
 */
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

/* Habetne series triviarum NOVA_LINEA? */

/* Trivia lateris per accessores (phasis V T7a) */
interior i32
_trivia_numerus (
    constans SilvaToken* tok,
                    b32  post)
{
    redde post ? silva_token_post_numerus(tok)
               : silva_token_ante_numerus(tok);
}

interior SilvaToken*
_trivium (
    constans SilvaToken* tok,
                    b32  post,
                    i32  i)
{
    redde post ? silva_token_post(tok, i) : silva_token_ante(tok, i);
}

interior b32
_habet_nova_linea (
    constans SilvaToken* tok,
                    b32  post)
{
    i32 i;

    per (i = ZEPHYRUM; i < _trivia_numerus(tok, post); i++)
    {
        si (_trivium(tok, post, i)->genus == SILVA_LEX_NOVA_LINEA)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior i32
_numerare_nova_linea (
    constans SilvaToken* tok,
                    b32  post)
{
    i32 i;
    i32 numerus;

    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < _trivia_numerus(tok, post); i++)
    {
        si (_trivium(tok, post, i)->genus == SILVA_LEX_NOVA_LINEA)
        {
            numerus++;
        }
    }
    redde numerus;
}

interior SilvaToken*
_obtinere_nova_linea (
    constans SilvaToken* tok,
                    b32  post)
{
    i32 i;

    per (i = ZEPHYRUM; i < _trivia_numerus(tok, post); i++)
    {
        SilvaToken* t;

        t = _trivium(tok, post, i);
        si (t->genus == SILVA_LEX_NOVA_LINEA)
        {
            redde t;
        }
    }
    redde NIHIL;
}

interior b32
_habet_continuationem (
    constans SilvaToken* tok,
                    b32  post)
{
    i32 i;

    per (i = ZEPHYRUM; i < _trivia_numerus(tok, post); i++)
    {
        si (_trivium(tok, post, i)->genus == SILVA_LEX_CONTINUATIO)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_compat_lexema",
        262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: NOVA_LINEA in spatia
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* tok;

        imprimere("\n--- Probans NOVA_LINEA in spatia ---\n");

        lexemata = _lexare(piscina, "int x;\nint y;");

        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_INT);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);

        /* ; - NOVA_LINEA in spatia_post */
        tok = _ad(lexemata, II);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_SEMICOLON);
        CREDO_VERUM (_habet_nova_linea(tok, VERUM));

        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, III)->genus,
            (i32)SILVA_LEX_INT);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, IV)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, V)->genus,
            (i32)SILVA_LEX_SEMICOLON);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, VI)->genus,
            (i32)SILVA_LEX_EOF);
    }


    /* ========================================================
     * PROBARE: newlines multiplices in spatia
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* tok;

        imprimere("\n--- Probans newlines multiplices ---\n");

        lexemata = _lexare(piscina, "a\n\nb");

        /* a: UNA NOVA_LINEA in spatia_post (prima) */
        tok = _ad(lexemata, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 (_numerare_nova_linea(tok, VERUM), I);

        /* b: UNA in spatia_ante (secunda) */
        tok = _ad(lexemata, I);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 (_numerare_nova_linea(tok, FALSUM), I);

        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, II)->genus,
            (i32)SILVA_LEX_EOF);
    }


    /* ========================================================
     * PROBARE: continuatio lineae (\<nl>)
     * ======================================================== */

    {
        Xar* lexemata;

        imprimere("\n--- Probans continuationem lineae ---\n");

        lexemata = _lexare(piscina, "int \\\nx;");

        /* continuatio consumpta: nullum NOVA_LINEA lexema in fluxu */
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_INT);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, II)->genus,
            (i32)SILVA_LEX_SEMICOLON);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, III)->genus,
            (i32)SILVA_LEX_EOF);
    }


    /* ========================================================
     * PROBARE: CONTINUATIO in spatia
     * ======================================================== */

    {
        Xar* lexemata;
        b32  inventa;

        imprimere("\n--- Probans spatia CONTINUATIO ---\n");

        lexemata = _lexare(piscina, "a \\\n b");

        inventa = _habet_continuationem(_ad(lexemata, 0), VERUM)
            || _habet_continuationem(_ad(lexemata, I), FALSUM);

        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 ((i32)inventa, (i32)VERUM);
    }


    /* ========================================================
     * PROBARE: CRLF in spatia
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* tok;
        SilvaToken* nl;

        imprimere("\n--- Probans CRLF ---\n");

        lexemata = _lexare(piscina, "a\r\nb");

        tok = _ad(lexemata, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_VERUM (_habet_nova_linea(tok, VERUM));

        /* NOVA_LINEA longitudo II (\r\n) */
        nl = _obtinere_nova_linea(tok, VERUM);
        CREDO_NON_NIHIL (nl);
        si (nl != NIHIL)
        {
            CREDO_AEQUALIS_I32 (silva_token_longitudo(nl), II);
        }

        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
    }


    /* ========================================================
     * PROBARE: forma directivae praeprocessoris
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* tok;

        imprimere("\n--- Probans formam directivae ---\n");

        lexemata = _lexare(piscina, "#define FOO 1\nint x;");

        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, 0)->genus,
            (i32)SILVA_LEX_CANCELLUM);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, II)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);

        /* 1: NOVA_LINEA in spatia_post (finis directivae) */
        tok = _ad(lexemata, III);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_VERUM (_habet_nova_linea(tok, VERUM));

        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, IV)->genus,
            (i32)SILVA_LEX_INT);
    }


    /* ========================================================
     * PROBARE: fluxus integer - numerus lexematum
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* tok;

        imprimere("\n--- Probans fluxum integrum ---\n");

        lexemata = _lexare(piscina, "a\nb");

        /* a, b, EOF = III (NOVA_LINEA in spatia) */
        CREDO_AEQUALIS_I32 (xar_numerus(lexemata), III);

        tok = _ad(lexemata, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_VERUM (_habet_nova_linea(tok, VERUM));

        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 ((i32)_ad(lexemata, II)->genus,
            (i32)SILVA_LEX_EOF);
    }


    /* ========================================================
     * PROBARE: silva_lexema_genus_nomen
     * ======================================================== */

    {
        imprimere("\n--- Probans genus_nomen ---\n");

        CREDO_AEQUALIS_I32 ((i32)strcmp(
            silva_lexema_genus_nomen(SILVA_LEX_NOVA_LINEA),
            "NOVA_LINEA"),
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)strcmp(
            silva_lexema_genus_nomen(SILVA_LEX_INT), "INT"), ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)strcmp(
            silva_lexema_genus_nomen(SILVA_LEX_EOF), "EOF"), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: newlines ducentes (initium plagulae)
     * ======================================================== */

    {
               Xar* lexemata;
        SilvaToken* tok;

        imprimere("\n--- Probans newlines ducentes ---\n");

        lexemata = _lexare(piscina, "\n\nint x;");

        /* int: II NOVA_LINEA in spatia_ante */
        tok = _ad(lexemata, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_INT);
        CREDO_AEQUALIS_I32 (_numerare_nova_linea(tok, FALSUM), II);
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
