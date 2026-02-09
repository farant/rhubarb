/* probatio_lapifex_c89_expr.c - Probationes Expressionum C89 via Lapifex GLR
 *
 * Probat parsationem expressionum C89 per pipeline:
 *   lexer -> adaptator -> GLR parser -> AST
 *
 * Verificat structuram AST (genera nodorum, operatores, liberos).
 */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "lapifex_c89.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ================================================
 * Auxiliaris: parsare expressionem
 * ================================================ */

hic_manens Arbor2Nodus*
_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons)
{
    redde lapifex_c89_expressionem_parsare(
        piscina, intern, fons, (i32)strlen(fons));
}

/* ================================================
 * Probationes
 * ================================================ */

s32 principale(vacuum)
{
    b32                  praeteritus;
    Piscina*             piscina;
    InternamentumChorda* intern;
    Arbor2Nodus*         nodus;

    piscina = piscina_generare_dynamicum("probatio_lapifex_c89_expr", MMMMXCVI * IV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina non creata\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Folia - IDENTIFICATOR
     * ======================================================== */
    {
        imprimere("\n--- Probans I: Folium IDENTIFICATOR ---\n");

        nodus = _parsare(piscina, intern, "x");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(nodus->datum.folium.valor.mensura, I);
        CREDO_VERUM(nodus->datum.folium.valor.datum[ZEPHYRUM] == 'x');
    }

    /* ========================================================
     * PROBARE II: Folium INTEGER
     * ======================================================== */
    {
        imprimere("\n--- Probans II: Folium INTEGER ---\n");

        nodus = _parsare(piscina, intern, "42");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_INTEGER);
    }

    /* ========================================================
     * PROBARE III: Folium FLOAT
     * ======================================================== */
    {
        imprimere("\n--- Probans III: Folium FLOAT ---\n");

        nodus = _parsare(piscina, intern, "3.14");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_FLOAT);
    }

    /* ========================================================
     * PROBARE IV: Folium CHAR_LIT
     * ======================================================== */
    {
        imprimere("\n--- Probans IV: Folium CHAR ---\n");

        nodus = _parsare(piscina, intern, "'a'");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_CHAR);
    }

    /* ========================================================
     * PROBARE V: Folium STRING_LIT
     * ======================================================== */
    {
        imprimere("\n--- Probans V: Folium STRING ---\n");

        nodus = _parsare(piscina, intern, "\"hello\"");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_STRING);
    }

    /* ========================================================
     * PROBARE VI: Binarium arithmeticum - 2+3
     * ======================================================== */
    {
        imprimere("\n--- Probans VI: Binarium 2+3 ---\n");

        nodus = _parsare(piscina, intern, "2+3");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_PLUS);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.sinister->genus,
            (s32)ARBOR2_NODUS_INTEGER);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.dexter->genus,
            (s32)ARBOR2_NODUS_INTEGER);
    }

    /* ========================================================
     * PROBARE VII: Praecedentia - 2+3*4
     *
     * Debet esse: PLUS(2, MULT(3, 4))
     * quia * habet maiorem praecedentiam quam +
     * ======================================================== */
    {
        Arbor2Nodus* dex;

        imprimere("\n--- Probans VII: Praecedentia 2+3*4 ---\n");

        nodus = _parsare(piscina, intern, "2+3*4");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_PLUS);

        /* Sinister = 2 (INTEGER) */
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.sinister->genus,
            (s32)ARBOR2_NODUS_INTEGER);

        /* Dexter = 3*4 (BINARIUM ASTERISCUS) */
        dex = nodus->datum.binarium.dexter;
        CREDO_AEQUALIS_S32((s32)dex->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)dex->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_ASTERISCUS);
        CREDO_AEQUALIS_S32((s32)dex->datum.binarium.sinister->genus,
            (s32)ARBOR2_NODUS_INTEGER);
        CREDO_AEQUALIS_S32((s32)dex->datum.binarium.dexter->genus,
            (s32)ARBOR2_NODUS_INTEGER);
    }

    /* ========================================================
     * PROBARE VIII: Associativitas sinistra - 1-2-3
     *
     * Debet esse: MINUS(MINUS(1, 2), 3)
     * ======================================================== */
    {
        Arbor2Nodus* sin;

        imprimere("\n--- Probans VIII: Assoc. sinistra 1-2-3 ---\n");

        nodus = _parsare(piscina, intern, "1-2-3");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_MINUS);

        /* Sinister = 1-2 (BINARIUM MINUS) */
        sin = nodus->datum.binarium.sinister;
        CREDO_AEQUALIS_S32((s32)sin->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)sin->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_MINUS);

        /* Dexter = 3 */
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.dexter->genus,
            (s32)ARBOR2_NODUS_INTEGER);
    }

    /* ========================================================
     * PROBARE IX: Associativitas dextra - a=b=c
     *
     * Debet esse: ASSIGN(a, ASSIGN(b, c))
     * ======================================================== */
    {
        Arbor2Nodus* dex;

        imprimere("\n--- Probans IX: Assoc. dextra a=b=c ---\n");

        nodus = _parsare(piscina, intern, "a=b=c");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_ASSIGNATIO);

        /* Sinister = a (IDENTIFICATOR) */
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.sinister->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);

        /* Dexter = b=c (BINARIUM ASSIGNATIO) */
        dex = nodus->datum.binarium.dexter;
        CREDO_AEQUALIS_S32((s32)dex->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)dex->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_ASSIGNATIO);
    }

    /* ========================================================
     * PROBARE X: Unarium praefixum - -x
     * ======================================================== */
    {
        imprimere("\n--- Probans X: Unarium -x ---\n");

        nodus = _parsare(piscina, intern, "-x");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_UNARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.unarium.operator,
            (s32)ARBOR2_LEXEMA_MINUS);
        CREDO_AEQUALIS_S32((s32)nodus->datum.unarium.operandum->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);
    }

    /* ========================================================
     * PROBARE XI: Unarium - !flag, &var, *ptr, ++i, --i, ~mask
     * ======================================================== */
    {
        imprimere("\n--- Probans XI: Unaria varia ---\n");

        nodus = _parsare(piscina, intern, "!flag");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_UNARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.unarium.operator,
            (s32)ARBOR2_LEXEMA_EXCLAMATIO);

        nodus = _parsare(piscina, intern, "&var");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_UNARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.unarium.operator,
            (s32)ARBOR2_LEXEMA_AMPERSAND);

        nodus = _parsare(piscina, intern, "*ptr");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_UNARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.unarium.operator,
            (s32)ARBOR2_LEXEMA_ASTERISCUS);

        nodus = _parsare(piscina, intern, "++i");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_UNARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.unarium.operator,
            (s32)ARBOR2_LEXEMA_DUPLUS);

        nodus = _parsare(piscina, intern, "~mask");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_UNARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.unarium.operator,
            (s32)ARBOR2_LEXEMA_TILDE);
    }

    /* ========================================================
     * PROBARE XII: Postfixum - x++, p--
     * ======================================================== */
    {
        imprimere("\n--- Probans XII: Postfixum x++, p-- ---\n");

        nodus = _parsare(piscina, intern, "x++");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_POST_UNARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.post_unarium.operator,
            (s32)ARBOR2_LEXEMA_DUPLUS);
        CREDO_AEQUALIS_S32((s32)nodus->datum.post_unarium.operandum->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);

        nodus = _parsare(piscina, intern, "p--");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_POST_UNARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.post_unarium.operator,
            (s32)ARBOR2_LEXEMA_DUMINUS);
    }

    /* ========================================================
     * PROBARE XIII: Accessus membrorum - s.x et p->y
     * ======================================================== */
    {
        imprimere("\n--- Probans XIII: Membrum s.x et p->y ---\n");

        nodus = _parsare(piscina, intern, "s.x");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_MEMBRUM);
        CREDO_FALSUM(nodus->datum.membrum.est_sagitta);
        CREDO_AEQUALIS_S32((s32)nodus->datum.membrum.basis->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);

        nodus = _parsare(piscina, intern, "p->y");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_MEMBRUM);
        CREDO_VERUM(nodus->datum.membrum.est_sagitta);
    }

    /* ========================================================
     * PROBARE XIV: Subscriptio - a[i]
     * ======================================================== */
    {
        imprimere("\n--- Probans XIV: Subscriptio a[i] ---\n");

        nodus = _parsare(piscina, intern, "a[i]");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SUBSCRIPTIO);
        CREDO_AEQUALIS_S32((s32)nodus->datum.subscriptio.basis->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);
        CREDO_AEQUALIS_S32((s32)nodus->datum.subscriptio.index->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);
    }

    /* ========================================================
     * PROBARE XV: Vocatio - f() et f(a,b)
     * ======================================================== */
    {
        imprimere("\n--- Probans XV: Vocatio f() et f(a,b) ---\n");

        /* Vocatio vacua */
        nodus = _parsare(piscina, intern, "f()");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_VOCATIO);
        CREDO_NIHIL(nodus->datum.vocatio.argumenta);

        /* Vocatio cum argumentis */
        nodus = _parsare(piscina, intern, "f(a,b)");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_VOCATIO);
        CREDO_NON_NIHIL(nodus->datum.vocatio.argumenta);
        CREDO_AEQUALIS_S32((s32)nodus->datum.vocatio.argumenta->genus,
            (s32)ARBOR2_NODUS_LISTA_SEPARATA);
    }

    /* ========================================================
     * PROBARE XVI: Parenthesized - (a+b)*c
     * ======================================================== */
    {
        Arbor2Nodus* sin;

        imprimere("\n--- Probans XVI: Parenthesized (a+b)*c ---\n");

        nodus = _parsare(piscina, intern, "(a+b)*c");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_ASTERISCUS);

        /* Sinister = (a+b) -> PARENTHESIZED */
        sin = nodus->datum.binarium.sinister;
        CREDO_AEQUALIS_S32((s32)sin->genus, (s32)ARBOR2_NODUS_PARENTHESIZED);
        CREDO_AEQUALIS_S32((s32)sin->datum.parenthesized.expressio->genus,
            (s32)ARBOR2_NODUS_BINARIUM);

        /* Dexter = c */
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.dexter->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);
    }

    /* ========================================================
     * PROBARE XVII: Ternarius - a?b:c
     * ======================================================== */
    {
        imprimere("\n--- Probans XVII: Ternarius a?b:c ---\n");

        nodus = _parsare(piscina, intern, "a?b:c");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TERNARIUS);
        CREDO_AEQUALIS_S32((s32)nodus->datum.ternarius.conditio->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);
        CREDO_AEQUALIS_S32((s32)nodus->datum.ternarius.verum->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);
        CREDO_AEQUALIS_S32((s32)nodus->datum.ternarius.falsum->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);
    }

    /* ========================================================
     * PROBARE XVIII: Comma - a,b,c
     *
     * Debet esse: LISTA_SEPARATA cum III elementis
     * ======================================================== */
    {
        imprimere("\n--- Probans XVIII: Comma a,b,c ---\n");

        nodus = _parsare(piscina, intern, "a,b,c");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_LISTA_SEPARATA);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.lista_separata.elementa), III);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.lista_separata.separatores), II);
    }

    /* ========================================================
     * PROBARE XIX: Cast - (int)x
     * ======================================================== */
    {
        imprimere("\n--- Probans XIX: Cast (int)x ---\n");

        nodus = _parsare(piscina, intern, "(int)x");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_CONVERSIO);
        CREDO_NON_NIHIL(nodus->datum.conversio.typus);
        CREDO_NON_NIHIL(nodus->datum.conversio.expressio);
        CREDO_AEQUALIS_S32((s32)nodus->datum.conversio.expressio->genus,
            (s32)ARBOR2_NODUS_IDENTIFICATOR);
    }

    /* ========================================================
     * PROBARE XX: Cast cum pointer - (char*)p
     * ======================================================== */
    {
        imprimere("\n--- Probans XX: Cast (char*)p ---\n");

        nodus = _parsare(piscina, intern, "(char*)p");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_CONVERSIO);
        CREDO_NON_NIHIL(nodus->datum.conversio.expressio);
    }

    /* ========================================================
     * PROBARE XXI: Sizeof expr - sizeof x
     * ======================================================== */
    {
        imprimere("\n--- Probans XXI: Sizeof x ---\n");

        nodus = _parsare(piscina, intern, "sizeof x");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SIZEOF);
        CREDO_FALSUM(nodus->datum.sizeof_expr.est_typus);
        CREDO_NON_NIHIL(nodus->datum.sizeof_expr.operandum);
    }

    /* ========================================================
     * PROBARE XXII: Sizeof(type) - sizeof(int)
     * ======================================================== */
    {
        imprimere("\n--- Probans XXII: Sizeof(int) ---\n");

        nodus = _parsare(piscina, intern, "sizeof(int)");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SIZEOF);
        CREDO_VERUM(nodus->datum.sizeof_expr.est_typus);
        CREDO_NON_NIHIL(nodus->datum.sizeof_expr.operandum);
    }

    /* ========================================================
     * PROBARE XXIII: Catena postfixa - a->b.c[i]()
     * ======================================================== */
    {
        imprimere("\n--- Probans XXIII: Catena a->b.c[i]() ---\n");

        nodus = _parsare(piscina, intern, "a->b.c[i]()");
        CREDO_NON_NIHIL(nodus);
        /* Extremum est vocatio () */
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_VOCATIO);
    }

    /* ========================================================
     * PROBARE XXIV: Omnes gradus praecedentiae
     *
     * a||b&&c|d^e&f==g<h<<i+j*k
     *
     * Debet parsari sine errore et radix debet esse
     * BINARIUM cum || operatore.
     * ======================================================== */
    {
        imprimere("\n--- Probans XXIV: Omnes praecedentiae ---\n");

        nodus = _parsare(piscina, intern, "a||b&&c|d^e&f==g<h<<i+j*k");
        CREDO_NON_NIHIL(nodus);
        /* Radix debet esse || (infima praecedentia binaria) */
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_DUPIPA);
    }

    /* ========================================================
     * PROBARE XXV: Expressio complexa - *p++ + sizeof(int*)
     * ======================================================== */
    {
        imprimere("\n--- Probans XXV: *p++ + sizeof(int*) ---\n");

        nodus = _parsare(piscina, intern, "*p++ + sizeof(int*)");
        CREDO_NON_NIHIL(nodus);
        /* Radix debet esse + */
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_PLUS);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
