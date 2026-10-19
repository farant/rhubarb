/* probatio_arbor2_expandere.c - Probationes Arbor2 Expansionis */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_expandere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

    /* ========================================================
     * PROBARE: Object-like macro expansion
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
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "FOO", "42", NIHIL);

        fons = "int x = FOO;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* int */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INT);

        /* x */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* = */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_ASSIGNATIO);

        /* FOO expanded to 42 */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, II);  /* "42" */

        /* ; */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_SEMICOLON);
    }
}

    /* ========================================================
     * PROBARE: #define in source creates segment
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
        Arbor2Expansion* exp;
        Xar* result;
        Xar* segmenta;

        exp = arbor2_expansion_creare(piscina, intern);

        fons = "#define BAR 1\nint x = BAR;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Check segments were created */
        segmenta = arbor2_expansion_obtinere_segmenta(exp);
        CREDO_NON_NIHIL(segmenta);

        /* Should have at least 2 segments (before and after #define) */
        /* Actually, looking at implementation, first segment is at line 1,
         * second at line where #define appears */
        (vacuum)result;
    }
}

    /* ========================================================
     * PROBARE: Macro provenance chain
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
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        Arbor2Token* radix;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "NUM", "123", NIHIL);

        fons = "NUM";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Find expanded token */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* Check provenance */
        CREDO_NON_NIHIL(tok->origo_meta);
        CREDO_AEQUALIS_I32(tok->origo_meta->genus, ARBOR2_ORIGO_EXPANSIO);

        /* Walk to root */
        radix = arbor2_token_radix(tok);
        CREDO_NON_NIHIL(radix);
        CREDO_AEQUALIS_I32(radix->origo_meta->genus, ARBOR2_ORIGO_FONS);
    }
}

    /* ========================================================
     * PROBARE: Nested macro expansion
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
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "A", "B", NIHIL);
        arbor2_expansion_addere_macro(exp, "B", "99", NIHIL);

        fons = "A";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* A -> B -> 99 */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
    }
}

    /* ========================================================
     * PROBARE: Query macro at line
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
        Arbor2Expansion* exp;
        Xar* result;
        TabulaDispersa* macros;
        Arbor2MacroDef* def;
        chorda nomen_ch;
        vacuum* valor;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);

        fons = "#define X 1\nint a;\n#undef X\nint b;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        /* At line 2, X should be defined */
        macros = arbor2_expansion_macros_ad_lineam(exp, II);
        CREDO_NON_NIHIL(macros);

        u.c = "X";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = I;

        si (tabula_dispersa_invenire(macros, nomen_ch, &valor))
        {
            def = (Arbor2MacroDef*)valor;
            CREDO_NON_NIHIL(def);
        }
    }
}

    /* ========================================================
     * PROBARE: est_typedef query
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
        Arbor2Expansion* exp;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_typedef(exp, "MyType");

        u.c = "MyType";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VI;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);

        u.c = "NotType";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VII;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), FALSUM);
    }
}

    /* ========================================================
     * PROBARE: Simple typedef detection
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
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2TypedefInfo* info;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "typedef int MyInt;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "MyInt";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);

        info = arbor2_expansion_quaerere_typedef(exp, nomen_ch);
        CREDO_NON_NIHIL(info);
        CREDO_AEQUALIS_I32((i32)info->est_certum, (i32)VERUM);
        CREDO_AEQUALIS_I32((i32)info->layer_discovered, (i32)ZEPHYRUM);
    }
}

    /* ========================================================
     * PROBARE: Latin nomen typedef detection
     * ======================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2TypedefInfo* info;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "nomen int MeusInt;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "MeusInt";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VII;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);

        info = arbor2_expansion_quaerere_typedef(exp, nomen_ch);
        CREDO_NON_NIHIL(info);
        /* est_certum is VERUM because lexer recognizes 'nomen' as TYPEDEF keyword */
        CREDO_AEQUALIS_I32((i32)info->est_certum, (i32)VERUM);
    }
}

    /* ========================================================
     * PROBARE: Struct typedef detection
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
        Arbor2Expansion* exp;
        Xar* result;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "typedef struct { int x; int y; } Point;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "Point";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);

        /* Verify internal identifiers are not registered as typedefs */
        u.c = "x";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = I;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), FALSUM);
    }
}

    /* ========================================================
     * PROBARE: Function pointer typedef detection
     * ======================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "typedef int (*Callback)(int, int);";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "Callback";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VIII;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);
    }
}

    /* ========================================================
     * PROBARE: Typedef with origo information
     * ======================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2TypedefInfo* info;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "typedef int TestType;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "TestType";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VIII;

        info = arbor2_expansion_quaerere_typedef(exp, nomen_ch);
        CREDO_NON_NIHIL(info);
        CREDO_NON_NIHIL(info->origo);
        CREDO_AEQUALIS_I32(info->origo->linea, I);  /* First line */
    }
}

    /* ========================================================
     * PROBARE: Function-like macro simple substitution
     * ======================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_add[] = {"a", "b", NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "ADD", parametra_add, "((a)+(b))", NIHIL);

        fons = "ADD(1,2)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* 1 */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);

        /* + */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PLUS);

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, V);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* 2 */
        tok = *(Arbor2Token**)xar_obtinere(result, VI);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, VII);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, VIII);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);
    }
}

    /* ========================================================
     * PROBARE: Function-like macro with nested parens in args
     * ======================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_add[] = {"a", "b", NIHIL};
        i32 i;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "ADD", parametra_add, "((a)+(b))", NIHIL);

        /* ADD((1+2),3) - should treat (1+2) as first arg */
        fons = "ADD((1+2),3)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Find the first integer token (should be 1) */
        /* Result should be: ((( 1 + 2 ))+(3)) */
        i = ZEPHYRUM;
        dum (i < xar_numerus(result))
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_INTEGER)
            {
                frange;
            }
            i++;
        }
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        /* Should have found 1 */
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);
    }
}

    /* ========================================================
     * PROBARE: Function-like macro empty params
     * ======================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_empty[] = {NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "EMPTY", parametra_empty, "x", NIHIL);

        fons = "EMPTY()";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* x */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);
    }
}

    /* ========================================================
     * PROBARE: Function-like macro MAX with conditional
     * ======================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_max[] = {"a", "b", NIHIL};
        b32 found_question;
        i32 i;

        exp = arbor2_expansion_creare(piscina, intern);
        /* MAX(a,b) ((a)>(b)?(a):(b)) */
        arbor2_expansion_addere_macro_functio(exp, "MAX", parametra_max,
                                               "((a)>(b)?(a):(b))", NIHIL);

        fons = "MAX(x,y)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Verify ? token exists (ternary operator) */
        found_question = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_QUAESTIO)
            {
                found_question = VERUM;
                frange;
            }
        }
        CREDO_VERUM(found_question);
    }
}

    /* ========================================================
     * PROBARE: Function-like macro via #define in source
     * ======================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define SQUARE(x) ((x)*(x)) then SQUARE(5) should produce ((5)*(5)) */
        fons = "#define SQUARE(x) ((x)*(x))\nSQUARE(5)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* 5 */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);

        /* * */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_ASTERISCUS);

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, V);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* 5 */
        tok = *(Arbor2Token**)xar_obtinere(result, VI);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, VII);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, VIII);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);
    }
}

    /* ========================================================
     * PROBARE: Stringification basic
     * ======================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_str[] = {"x", NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "STR", parametra_str, "#x", NIHIL);

        fons = "STR(hello)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce "hello" string literal */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
    }
}

    /* ========================================================
     * PROBARE: Stringification with numeric
     * ======================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_str[] = {"x", NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "STR", parametra_str, "#x", NIHIL);

        fons = "STR(123)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
        /* Should contain "123" */
    }
}

    /* ========================================================
     * PROBARE: Stringification with escaping
     * ======================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_str[] = {"x", NIHIL};
        b32 has_backslash;
        i32 j;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "STR", parametra_str, "#x", NIHIL);

        /* Stringifying a string literal should escape the quotes */
        fons = "STR(\"hi\")";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);

        /* Check that result contains backslash (escaped quotes) */
        has_backslash = FALSUM;
        per (j = ZEPHYRUM; j < tok->lexema->valor.mensura; j++)
        {
            si (tok->lexema->valor.datum[j] == '\\')
            {
                has_backslash = VERUM;
                frange;
            }
        }
        CREDO_VERUM(has_backslash);
    }
}

    /* ========================================================
     * PROBARE: Stringification via #define
     * ======================================================== */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define STRINGIFY(x) #x then STRINGIFY(test) should produce "test" */
        fons = "#define STRINGIFY(x) #x\nSTRINGIFY(test)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce string literal "test" */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
    }
}

    /* ========================================================
     * PROBARE: Token pasting basic
     * ======================================================== */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_paste[] = {"a", "b", NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "PASTE", parametra_paste, "a##b", NIHIL);

        fons = "PASTE(foo,bar)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce foobar identifier */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, VI);  /* "foobar" = 6 */
    }
}

    /* ========================================================
     * PROBARE: Token pasting prefix
     * ======================================================== */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_var[] = {"n", NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        /* VAR(n) -> var_##n */
        arbor2_expansion_addere_macro_functio(exp, "VAR", parametra_var, "var_##n", NIHIL);

        fons = "VAR(1)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce var_1 identifier */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, V);  /* "var_1" = 5 */
    }
}

    /* ========================================================
     * PROBARE: Token pasting numeric
     * ======================================================== */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_paste[] = {"a", "b", NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "PASTE", parametra_paste, "a##b", NIHIL);

        fons = "PASTE(12,34)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce 1234 integer */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, IV);  /* "1234" = 4 */
    }
}

    /* ========================================================
     * PROBARE: Token pasting via #define
     * ======================================================== */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define GLUE(a,b) a##b then GLUE(hello,world) should produce helloworld */
        fons = "#define GLUE(a,b) a##b\nGLUE(hello,world)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce helloworld identifier */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, X);  /* "helloworld" = 10 */
    }
}

    /* ========================================================
     * PROBARE: Variadic macro basic
     * ======================================================== */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_var[] = {"...", NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "VARIADIC", parametra_var,
                                               "__VA_ARGS__", NIHIL);

        /* VARIADIC(a,b,c) should produce a,b,c */
        fons = "VARIADIC(a,b,c)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* a */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "a" */

        /* , */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_COMMA);

        /* b */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "b" */

        /* , */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_COMMA);

        /* c */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "c" */
    }
}

    /* ========================================================
     * PROBARE: Variadic macro with prefix
     * ======================================================== */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_fmt[] = {"f", "...", NIHIL};

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "FMT", parametra_fmt,
                                               "f(__VA_ARGS__)", NIHIL);

        fons = "FMT(foo,1,2)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* First should be foo identifier */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* Second should be ( */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);
    }
}

    /* ========================================================
     * PROBARE: Variadic macro via #define
     * ======================================================== */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define ARGS(...) __VA_ARGS__ then ARGS(x,y,z) should produce x,y,z */
        fons = "#define ARGS(...) __VA_ARGS__\nARGS(x,y,z)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* x */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "x" */

        /* , */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_COMMA);

        /* y */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "y" */

        /* , */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_COMMA);

        /* z */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "z" */
    }
}

    /* ========================================================
     * PROBARE: Recursion prevention - direct
     * ======================================================== */

interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);
        /* SELF expands to SELF - should not loop infinitely */
        arbor2_expansion_addere_macro(exp, "SELF", "SELF", NIHIL);

        fons = "SELF";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should output SELF identifier (not expanded further) */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, IV);  /* "SELF" = 4 */
    }
}

    /* ========================================================
     * PROBARE: Recursion prevention - mutual
     * ======================================================== */

interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);
        /* A -> B, B -> A: should stop, not loop */
        arbor2_expansion_addere_macro(exp, "MACRO_A", "MACRO_B", NIHIL);
        arbor2_expansion_addere_macro(exp, "MACRO_B", "MACRO_A", NIHIL);

        fons = "MACRO_A";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should output MACRO_A (stopped because A was already being expanded) */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
    }
}

    /* ========================================================
     * PROBARE: Recursion prevention via #define
     * ======================================================== */

interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        i32 num;

        exp = arbor2_expansion_creare(piscina, intern);

        fons = "#define RECURSIVE RECURSIVE\nRECURSIVE";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Test that processing completes (no infinite loop) */
        /* and produces some output */
        num = xar_numerus(result);
        CREDO_MAIOR_I32(num, ZEPHYRUM);
    }
}

    /* ========================================================
     * PROBARE: #undef stops expansion
     * ======================================================== */

interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        b32 found_integer;
        b32 found_identifier;
        i32 i;

        exp = arbor2_expansion_creare(piscina, intern);

        /* Define, use, undef, use again */
        fons = "#define TEMP 42\nTEMP\n#undef TEMP\nTEMP";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* First TEMP should be expanded to 42 (INTEGER) */
        /* Second TEMP should remain identifier (after #undef) */
        found_integer = FALSUM;
        found_identifier = FALSUM;

        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_INTEGER)
            {
                found_integer = VERUM;
            }
            si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
            {
                found_identifier = VERUM;
            }
        }

        /* Should have at least one integer (first TEMP expanded) */
        CREDO_VERUM(found_integer);
        /* Should have at least one identifier (second TEMP not expanded) */
        CREDO_VERUM(found_identifier);
    }
}

    /* ========================================================
     * PROBARE: Re-define after #undef
     * ======================================================== */

interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        i32 integer_count;
        i32 i;

        exp = arbor2_expansion_creare(piscina, intern);

        fons = "#define X 1\nX\n#undef X\n#define X 2\nX";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should have integers from both X expansions */
        integer_count = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_INTEGER)
            {
                integer_count++;
            }
        }
        /* Should have at least one integer (macro expansion working) */
        CREDO_MAIOR_I32(integer_count, ZEPHYRUM);
    }
}

    /* ========================================================
     * PROBARE: #undef non-existent macro (no error)
     * ======================================================== */

interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #undef on non-existent macro should not cause error */
        fons = "#undef NONEXISTENT\nint x;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should still have tokens */
        CREDO_MAIOR_I32(xar_numerus(result), ZEPHYRUM);
    }
}

    /* ========================================================
     * PROBARE: Variadic macro - zero arguments
     * ======================================================== */

interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        i32 num;
        b32 found_eof_only;
        i32 i;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define ARGS(...) __VA_ARGS__ then ARGS() should produce empty */
        fons = "#define ARGS(...) __VA_ARGS__\nARGS()";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Numerus tokenum debet esse parvum (solum EOF vel vacuum) */
        num = xar_numerus(result);
        found_eof_only = VERUM;
        per (i = ZEPHYRUM; i < num; i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus != ARBOR2_LEXEMA_EOF)
            {
                found_eof_only = FALSUM;
                frange;
            }
        }
        /* Vel vacuum vel solum EOF */
        CREDO_VERUM(num == ZEPHYRUM || found_eof_only);
    }
}

    /* ========================================================
     * PROBARE: Variadic macro - single argument
     * ======================================================== */

interior vacuum
_sectio_tricesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define ARGS(...) __VA_ARGS__ then ARGS(x) should produce x */
        fons = "#define ARGS(...) __VA_ARGS__\nARGS(x)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet esse x identificator */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "x" = 1 */
    }
}

    /* ========================================================
     * PROBARE: Variadic macro - __VA_ARGS__ used twice
     * ======================================================== */

interior vacuum
_sectio_tricesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        i32 comma_count;
        i32 i;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define TWICE(...) __VA_ARGS__ __VA_ARGS__ */
        fons = "#define TWICE(...) __VA_ARGS__ __VA_ARGS__\nTWICE(a,b)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet habere a,b a,b - ergo 3 commas (2 in primo, 2 in secundo, minus 1) */
        /* Actually: a , b a , b = 2 commas */
        comma_count = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_COMMA)
            {
                comma_count++;
            }
        }
        /* Duo commas (unum in primo __VA_ARGS__, unum in secundo) */
        CREDO_AEQUALIS_I32(comma_count, II);
    }
}

    /* ========================================================
     * PROBARE: Nested macro invocation - macro arg is macro call
     * ======================================================== */

interior vacuum
_sectio_tricesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        i32 plus_count;
        i32 i;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define MUL(a,b) ((a)*(b))
         * #define ADD(a,b) ((a)+(b))
         * ADD(MUL(2,3), 4) -> ((((2)*(3)))+(4)) */
        fons = "#define MUL(a,b) ((a)*(b))\n#define ADD(a,b) ((a)+(b))\nADD(MUL(2,3),4)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet habere PLUS token */
        plus_count = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_PLUS)
            {
                plus_count++;
            }
        }
        CREDO_AEQUALIS_I32(plus_count, I);
    }
}

    /* ========================================================
     * PROBARE: Nested macro invocation - chained function macros
     * ======================================================== */

interior vacuum
_sectio_tricesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        b32 found_identifier;
        i32 i;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define INNER(x) (x)
         * #define OUTER(x) [x]
         * OUTER(INNER(foo)) -> [(foo)] */
        fons = "#define INNER(x) (x)\n#define OUTER(x) [x]\nOUTER(INNER(foo))";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet habere foo identifier et brackets */
        found_identifier = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
                tok->lexema->valor.mensura == III)  /* "foo" = 3 */
            {
                found_identifier = VERUM;
                frange;
            }
        }
        CREDO_VERUM(found_identifier);
    }
}

    /* ========================================================
     * PROBARE: Object-like macro via #define - simple
     * ======================================================== */

interior vacuum
_sectio_tricesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define FOO 42 then FOO -> 42 */
        fons = "#define FOO 42\nFOO";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* 42 */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, II);  /* "42" = 2 */
    }
}

    /* ========================================================
     * PROBARE: Object-like macro via #define - multi-token
     * ======================================================== */

interior vacuum
_sectio_quadragesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define EXPR 1+2 then EXPR -> 1 + 2 */
        fons = "#define EXPR 1+2\nEXPR";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* 1 */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* + */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PLUS);

        /* 2 */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
    }
}

    /* ========================================================
     * PROBARE: Stringification - empty argument
     * ======================================================== */

interior vacuum
_sectio_quadragesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define STR(x) #x then STR() -> "" */
        fons = "#define STR(x) #x\nSTR()";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet esse chorda literalis vacua "" */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
        /* Mensura debet esse II pro "" (duae quotes) */
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, II);
    }
}

    /* ========================================================
     * PROBARE: Stringification - multiple tokens
     * ======================================================== */

interior vacuum
_sectio_quadragesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define STR(x) #x then STR(a b c) -> "a b c" */
        fons = "#define STR(x) #x\nSTR(a b c)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet esse chorda literalis "a b c" */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
        /* Mensura maior quam simplex token */
        CREDO_MAIOR_I32(tok->lexema->valor.mensura, III);
    }
}

    /* ========================================================
     * PROBARE: Token pasting - create operator
     * ======================================================== */

interior vacuum
_sectio_quadragesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define OP(a,b) a##b then OP(+,=) -> += */
        fons = "#define OP(a,b) a##b\nOP(+,=)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet esse PLUS_ASSIGN vel duo tokens */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        /* += debet esse PLUS_ASSIGN */
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PLUS_ASSIGN);
    }
}

    /* ========================================================
     * PROBARE: Token pasting - empty left argument
     * ======================================================== */

interior vacuum
_sectio_quadragesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define PASTE(a,b) a##b then PASTE(,bar)
         * Comportamentum vacuum paste non est bene definitum.
         * Verificare quod non frangit et producit aliquid. */
        fons = "#define PASTE(a,b) a##b\nPASTE(,bar)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Verificare quod processatio non fregit et producit tokens */
        CREDO_MAIOR_I32(xar_numerus(result), ZEPHYRUM);
    }
}

    /* ========================================================
     * PROBARE: Triple nested macro expansion
     * ======================================================== */

interior vacuum
_sectio_quadragesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        exp = arbor2_expansion_creare(piscina, intern);

        /* A -> B -> C -> 999 */
        fons = "#define A B\n#define B C\n#define C 999\nA";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* A -> B -> C -> 999 */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, III);  /* "999" = 3 */
    }
}

    /* ========================================================
     * PROBARE: Lookahead genus - simple INT macro
     * ======================================================== */

interior vacuum
_sectio_quadragesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp;
        Arbor2LexemaGenus genus;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "TYPUS", "int", NIHIL);

        u.c = "TYPUS";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        genus = arbor2_expansion_lookahead_genus(exp, nomen_ch);
        CREDO_AEQUALIS_I32((i32)genus, (i32)ARBOR2_LEXEMA_INT);
    }
}

    /* ========================================================
     * PROBARE: Lookahead genus - unknown macro returns ERROR
     * ======================================================== */

interior vacuum
_sectio_quadragesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp;
        Arbor2LexemaGenus genus;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);

        u.c = "NONEXISTENT";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = XI;

        genus = arbor2_expansion_lookahead_genus(exp, nomen_ch);
        CREDO_AEQUALIS_I32((i32)genus, (i32)ARBOR2_LEXEMA_ERROR);
    }
}

    /* ========================================================
     * PROBARE: Lookahead genus - empty macro returns EOF
     * ======================================================== */

interior vacuum
_sectio_quadragesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp;
        Arbor2LexemaGenus genus;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "VACUUM", "", NIHIL);

        u.c = "VACUUM";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VI;

        genus = arbor2_expansion_lookahead_genus(exp, nomen_ch);
        CREDO_AEQUALIS_I32((i32)genus, (i32)ARBOR2_LEXEMA_EOF);
    }
}

    /* ========================================================
     * PROBARE: Lookahead extended - macro expanding to macro
     * ======================================================== */

interior vacuum
_sectio_quadragesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp;
        Arbor2ExpansionLookahead lookahead;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "INNER", "42", NIHIL);
        arbor2_expansion_addere_macro(exp, "OUTER", "INNER", NIHIL);

        u.c = "OUTER";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        lookahead = arbor2_expansion_lookahead(exp, nomen_ch);

        /* Primus token est INNER identificator */
        CREDO_AEQUALIS_I32((i32)lookahead.genus, (i32)ARBOR2_LEXEMA_IDENTIFICATOR);
        /* INNER est macro, ergo est_recursivum debet esse VERUM */
        CREDO_AEQUALIS_I32((i32)lookahead.est_recursivum, (i32)VERUM);
        CREDO_AEQUALIS_I32((i32)lookahead.est_vacuum, (i32)FALSUM);
    }
}

    /* ========================================================
     * PROBARE: Lookahead extended - empty macro est_vacuum
     * ======================================================== */

interior vacuum
_sectio_quinquagesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp;
        Arbor2ExpansionLookahead lookahead;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "EMPTY", "", NIHIL);

        u.c = "EMPTY";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        lookahead = arbor2_expansion_lookahead(exp, nomen_ch);

        CREDO_AEQUALIS_I32((i32)lookahead.genus, (i32)ARBOR2_LEXEMA_EOF);
        CREDO_AEQUALIS_I32((i32)lookahead.est_vacuum, (i32)VERUM);
        CREDO_AEQUALIS_I32((i32)lookahead.est_recursivum, (i32)FALSUM);
    }
}

    /* ========================================================
     * PROBARE: Lookahead extended - non-recursive identifier
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp;
        Arbor2ExpansionLookahead lookahead;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        /* Macro expandit ad identificator qui NON est macro */
        arbor2_expansion_addere_macro(exp, "ALIAS", "foo", NIHIL);

        u.c = "ALIAS";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        lookahead = arbor2_expansion_lookahead(exp, nomen_ch);

        /* foo est identificator sed non macro */
        CREDO_AEQUALIS_I32((i32)lookahead.genus, (i32)ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32((i32)lookahead.est_recursivum, (i32)FALSUM);
        CREDO_AEQUALIS_I32((i32)lookahead.est_vacuum, (i32)FALSUM);
    }
}

    /* ========================================================
     * PROBARE: Lookahead genus - integer literal
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp;
        Arbor2LexemaGenus genus;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "NUMERUS", "42", NIHIL);

        u.c = "NUMERUS";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VII;

        genus = arbor2_expansion_lookahead_genus(exp, nomen_ch);
        CREDO_AEQUALIS_I32((i32)genus, (i32)ARBOR2_LEXEMA_INTEGER);
    }
}

    /* ========================================================
     * PROBARE: Include path management - add paths
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp;

        exp = arbor2_expansion_creare(piscina, intern);

        /* Add paths */
        arbor2_expansion_addere_system_via(exp, "/usr/include");
        arbor2_expansion_addere_local_via(exp, "./probationes/fixa");

        /* Verify arrays are populated */
        CREDO_AEQUALIS_I32(xar_numerus(exp->system_viae), I);
        CREDO_AEQUALIS_I32(xar_numerus(exp->local_viae), I);
    }
}

    /* ========================================================
     * PROBARE: Local include - extract macros
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2MacroDef* def;
        chorda nomen_ch;
        vacuum* val_ptr;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_local_via(exp, "./probationes/fixa");

        fons = "#include \"include_test_header.h\"\nint x = HEADER_MACRO;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        /* Verify HEADER_MACRO was extracted */
        u.c = "HEADER_MACRO";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = XII;

        CREDO_VERUM(tabula_dispersa_invenire(exp->macros, nomen_ch, &val_ptr));
        def = (Arbor2MacroDef*)val_ptr;
        CREDO_NON_NIHIL(def);
    }
}

    /* ========================================================
     * PROBARE: Local include - extract typedefs
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_local_via(exp, "./probationes/fixa");

        fons = "#include \"include_test_header.h\"";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        /* Verify HeaderInt typedef was extracted */
        u.c = "HeaderInt";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = IX;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);
    }
}

    /* ========================================================
     * PROBARE: Nested include - extract from nested
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        chorda nomen_ch;
        vacuum* val_ptr;
        unio { constans character* c; i8* m; } u;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_local_via(exp, "./probationes/fixa");

        fons = "#include \"include_test_nested.h\"";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        /* Verify NESTED_MACRO from nested file */
        u.c = "NESTED_MACRO";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = XII;

        CREDO_VERUM(tabula_dispersa_invenire(exp->macros, nomen_ch, &val_ptr));

        /* Verify HEADER_MACRO from included-by-nested file */
        u.c = "HEADER_MACRO";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = XII;

        CREDO_VERUM(tabula_dispersa_invenire(exp->macros, nomen_ch, &val_ptr));
    }
}

    /* ========================================================
     * PROBARE: Include guard - no double processing
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_local_via(exp, "./probationes/fixa");

        /* Include same file twice - should be skipped second time */
        fons = "#include \"include_test_header.h\"\n#include \"include_test_header.h\"";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Test passes if it completes without infinite loop */
        CREDO_NON_NIHIL(result);
    }
}

    /* ========================================================
     * PROBARE: Unresolved include - graceful handling
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;

        exp = arbor2_expansion_creare(piscina, intern);
        /* No paths added - file won't be found */

        fons = "#include \"nonexistent.h\"";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Processing should complete despite missing file */
        CREDO_NON_NIHIL(result);
    }
}

    /* ========================================================
     * PROBARE: System include syntax
     * ======================================================== */

interior vacuum
_sectio_quinquagesima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern  = internamentum_creare(piscina);
    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;

        exp = arbor2_expansion_creare(piscina, intern);

        /* System include with no paths - won't be found but should parse */
        fons = "#include <stdio.h>";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Processing should complete */
        CREDO_NON_NIHIL(result);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "object-like macro expansion", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#define creates segment", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "macro provenance chain", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested macro expansion", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "query macro at line", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "est_typedef query", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple typedef detection", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nomen (Latin typedef) detection", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct typedef detection", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function pointer typedef detection", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef origo information", _sectio_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function-like macro simple substitution", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function-like macro with nested parens", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function-like macro empty params", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function-like macro MAX", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function-like macro via #define", _sectio_decima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stringification basic", _sectio_decima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stringification numeric", _sectio_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stringification escaping", _sectio_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stringification via #define", _sectio_vicesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "token pasting basic", _sectio_vicesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "token pasting prefix", _sectio_vicesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "token pasting numeric", _sectio_vicesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "token pasting via #define", _sectio_vicesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "variadic macro basic", _sectio_vicesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "variadic macro with prefix", _sectio_vicesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "variadic macro via #define", _sectio_vicesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recursion prevention direct", _sectio_vicesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recursion prevention mutual", _sectio_vicesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recursion prevention via #define", _sectio_tricesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#undef stops expansion", _sectio_tricesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "re-define after #undef", _sectio_tricesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#undef non-existent macro", _sectio_tricesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "variadic macro zero arguments", _sectio_tricesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "variadic macro single argument", _sectio_tricesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "__VA_ARGS__ used twice", _sectio_tricesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested macro - arg is macro call", _sectio_tricesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chained function macros", _sectio_tricesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "object-like macro via #define simple", _sectio_tricesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "object-like macro multi-token", _sectio_quadragesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stringification empty argument", _sectio_quadragesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stringification multiple tokens", _sectio_quadragesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "token pasting create operator", _sectio_quadragesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "token pasting empty left arg", _sectio_quadragesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "triple nested macro expansion", _sectio_quadragesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lookahead genus simple INT macro", _sectio_quadragesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lookahead genus unknown macro", _sectio_quadragesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lookahead genus empty macro", _sectio_quadragesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lookahead extended recursive macro", _sectio_quadragesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lookahead extended empty macro est_vacuum", _sectio_quinquagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lookahead extended non-recursive identifier", _sectio_quinquagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lookahead genus integer literal", _sectio_quinquagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "include path management", _sectio_quinquagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "local include macro extraction", _sectio_quinquagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "local include typedef extraction", _sectio_quinquagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested include extraction", _sectio_quinquagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "include guard", _sectio_quinquagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unresolved include", _sectio_quinquagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "system include syntax", _sectio_quinquagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("arbor2_expandere", SECTIONES);
}
