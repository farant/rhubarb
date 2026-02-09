/* probatio_lapifex_c89_sent.c - Probationes Sententiarum C89 via Lapifex GLR
 *
 * Probat parsationem sententiarum C89 per pipeline:
 *   lexer -> adaptator -> GLR parser -> AST
 *
 * Verificat structuram AST (genera nodorum, campi sententiarum).
 */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "lapifex_c89.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ================================================
 * Auxiliaris: parsare sententiam
 * ================================================ */

hic_manens Arbor2Nodus*
_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons)
{
    redde lapifex_c89_sententiam_parsare(
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

    piscina = piscina_generare_dynamicum("probatio_lapifex_c89_sent", MMMMXCVI * IV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina non creata\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Sententia expressionis - x = 5;
     * ======================================================== */
    {
        imprimere("\n--- Probans I: x = 5; ---\n");

        nodus = _parsare(piscina, intern, "x = 5;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SENTENTIA);
        CREDO_NON_NIHIL(nodus->datum.sententia.expressio);
        CREDO_NON_NIHIL(nodus->datum.sententia.tok_semicolon);
    }

    /* ========================================================
     * PROBARE II: Sententia vacua - ;
     * ======================================================== */
    {
        imprimere("\n--- Probans II: ; ---\n");

        nodus = _parsare(piscina, intern, ";");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SENTENTIA_VACUA);
    }

    /* ========================================================
     * PROBARE III: Corpus vacuum - { }
     * ======================================================== */
    {
        imprimere("\n--- Probans III: { } ---\n");

        nodus = _parsare(piscina, intern, "{ }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_CORPUS);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.corpus.sententiae == NIHIL), I);
    }

    /* ========================================================
     * PROBARE IV: Corpus cum una sententia - { x = 1; }
     * ======================================================== */
    {
        Arbor2Nodus** elementa;

        imprimere("\n--- Probans IV: { x = 1; } ---\n");

        nodus = _parsare(piscina, intern, "{ x = 1; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_CORPUS);
        CREDO_NON_NIHIL(nodus->datum.corpus.sententiae);
        CREDO_AEQUALIS_I32((i32)xar_numerus(nodus->datum.corpus.sententiae), I);
        elementa = (Arbor2Nodus**)xar_obtinere(nodus->datum.corpus.sententiae, ZEPHYRUM);
        CREDO_NON_NIHIL(elementa);
        CREDO_AEQUALIS_S32((s32)(*elementa)->genus, (s32)ARBOR2_NODUS_SENTENTIA);
    }

    /* ========================================================
     * PROBARE V: Corpus cum multis sententiis - { x = 1; y = 2; z = 3; }
     * ======================================================== */
    {
        imprimere("\n--- Probans V: { x = 1; y = 2; z = 3; } ---\n");

        nodus = _parsare(piscina, intern, "{ x = 1; y = 2; z = 3; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_CORPUS);
        CREDO_NON_NIHIL(nodus->datum.corpus.sententiae);
        CREDO_AEQUALIS_I32((i32)xar_numerus(nodus->datum.corpus.sententiae), III);
    }

    /* ========================================================
     * PROBARE VI: Corpus cum declaratione - { int x; x = 1; }
     * ======================================================== */
    {
        Arbor2Nodus** elem0;
        Arbor2Nodus** elem1;

        imprimere("\n--- Probans VI: { int x; x = 1; } ---\n");

        nodus = _parsare(piscina, intern, "{ int x; x = 1; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_CORPUS);
        CREDO_NON_NIHIL(nodus->datum.corpus.sententiae);
        CREDO_AEQUALIS_I32((i32)xar_numerus(nodus->datum.corpus.sententiae), II);
        elem0 = (Arbor2Nodus**)xar_obtinere(nodus->datum.corpus.sententiae, ZEPHYRUM);
        elem1 = (Arbor2Nodus**)xar_obtinere(nodus->datum.corpus.sententiae, I);
        CREDO_AEQUALIS_S32((s32)(*elem0)->genus, (s32)ARBOR2_NODUS_DECLARATIO);
        CREDO_AEQUALIS_S32((s32)(*elem1)->genus, (s32)ARBOR2_NODUS_SENTENTIA);
    }

    /* ========================================================
     * PROBARE VII: break;
     * ======================================================== */
    {
        imprimere("\n--- Probans VII: break; ---\n");

        nodus = _parsare(piscina, intern, "break;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_FRANGE);
        CREDO_NON_NIHIL(nodus->datum.frangendum.tok_frange);
        CREDO_NON_NIHIL(nodus->datum.frangendum.tok_semicolon);
    }

    /* ========================================================
     * PROBARE VIII: continue;
     * ======================================================== */
    {
        imprimere("\n--- Probans VIII: continue; ---\n");

        nodus = _parsare(piscina, intern, "continue;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_PERGE);
        CREDO_NON_NIHIL(nodus->datum.pergendum.tok_perge);
        CREDO_NON_NIHIL(nodus->datum.pergendum.tok_semicolon);
    }

    /* ========================================================
     * PROBARE IX: goto end;
     * ======================================================== */
    {
        imprimere("\n--- Probans IX: goto end; ---\n");

        nodus = _parsare(piscina, intern, "goto end;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SALTA);
        CREDO_NON_NIHIL(nodus->datum.saltus.tok_salta);
        CREDO_NON_NIHIL(nodus->datum.saltus.tok_destinatio);
        CREDO_NON_NIHIL(nodus->datum.saltus.tok_semicolon);
    }

    /* ========================================================
     * PROBARE X: return;
     * ======================================================== */
    {
        imprimere("\n--- Probans X: return; ---\n");

        nodus = _parsare(piscina, intern, "return;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_REDDE);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.reditio.valor == NIHIL), I);
        CREDO_NON_NIHIL(nodus->datum.reditio.tok_semicolon);
    }

    /* ========================================================
     * PROBARE XI: return x + 1;
     * ======================================================== */
    {
        imprimere("\n--- Probans XI: return x + 1; ---\n");

        nodus = _parsare(piscina, intern, "return x + 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_REDDE);
        CREDO_NON_NIHIL(nodus->datum.reditio.valor);
        CREDO_NON_NIHIL(nodus->datum.reditio.tok_semicolon);
    }

    /* ========================================================
     * PROBARE XII: if (x) y = 1;
     * ======================================================== */
    {
        imprimere("\n--- Probans XII: if (x) y = 1; ---\n");

        nodus = _parsare(piscina, intern, "if (x) y = 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SI);
        CREDO_NON_NIHIL(nodus->datum.conditionale.tok_si);
        CREDO_NON_NIHIL(nodus->datum.conditionale.conditio);
        CREDO_NON_NIHIL(nodus->datum.conditionale.consequens);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.conditionale.alternans == NIHIL), I);
    }

    /* ========================================================
     * PROBARE XIII: if (x) y = 1; else y = 2;
     * ======================================================== */
    {
        imprimere("\n--- Probans XIII: if (x) y = 1; else y = 2; ---\n");

        nodus = _parsare(piscina, intern, "if (x) y = 1; else y = 2;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SI);
        CREDO_NON_NIHIL(nodus->datum.conditionale.conditio);
        CREDO_NON_NIHIL(nodus->datum.conditionale.consequens);
        CREDO_NON_NIHIL(nodus->datum.conditionale.tok_alioquin);
        CREDO_NON_NIHIL(nodus->datum.conditionale.alternans);
    }

    /* ========================================================
     * PROBARE XIV: Dangling-else - if (a) if (b) x = 1; else y = 2;
     * else debet ligare ad interiorem si
     * ======================================================== */
    {
        Arbor2Nodus* interior_si;

        imprimere("\n--- Probans XIV: Dangling-else ---\n");

        nodus = _parsare(piscina, intern, "if (a) if (b) x = 1; else y = 2;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SI);
        /* Exterior si non habet else */
        CREDO_AEQUALIS_S32((s32)(nodus->datum.conditionale.alternans == NIHIL), I);
        /* Consequens est interior si */
        interior_si = nodus->datum.conditionale.consequens;
        CREDO_NON_NIHIL(interior_si);
        CREDO_AEQUALIS_S32((s32)interior_si->genus, (s32)ARBOR2_NODUS_SI);
        /* Interior si HABET else */
        CREDO_NON_NIHIL(interior_si->datum.conditionale.alternans);
    }

    /* ========================================================
     * PROBARE XV: while (x) x = x - 1;
     * ======================================================== */
    {
        imprimere("\n--- Probans XV: while (x) x = x - 1; ---\n");

        nodus = _parsare(piscina, intern, "while (x) x = x - 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DUM);
        CREDO_NON_NIHIL(nodus->datum.iteratio.tok_dum);
        CREDO_NON_NIHIL(nodus->datum.iteratio.conditio);
        CREDO_NON_NIHIL(nodus->datum.iteratio.corpus);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.iteratio.tok_fac == NIHIL), I);
    }

    /* ========================================================
     * PROBARE XVI: do x = x + 1; while (x < 10);
     * ======================================================== */
    {
        imprimere("\n--- Probans XVI: do-while ---\n");

        nodus = _parsare(piscina, intern, "do x = x + 1; while (x < 10);");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_FAC);
        CREDO_NON_NIHIL(nodus->datum.iteratio.tok_fac);
        CREDO_NON_NIHIL(nodus->datum.iteratio.tok_dum);
        CREDO_NON_NIHIL(nodus->datum.iteratio.conditio);
        CREDO_NON_NIHIL(nodus->datum.iteratio.corpus);
        CREDO_NON_NIHIL(nodus->datum.iteratio.tok_semicolon);
    }

    /* ========================================================
     * PROBARE XVII: for (i = 0; i < 10; i = i + 1) x = x + 1;
     * ======================================================== */
    {
        imprimere("\n--- Probans XVII: for (init;cond;incr) ---\n");

        nodus = _parsare(piscina, intern, "for (i = 0; i < 10; i = i + 1) x = x + 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_PER);
        CREDO_NON_NIHIL(nodus->datum.circuitus.tok_per);
        CREDO_NON_NIHIL(nodus->datum.circuitus.initium);
        CREDO_NON_NIHIL(nodus->datum.circuitus.conditio);
        CREDO_NON_NIHIL(nodus->datum.circuitus.incrementum);
        CREDO_NON_NIHIL(nodus->datum.circuitus.corpus);
    }

    /* ========================================================
     * PROBARE XVIII: for (;;) x = 1;
     * ======================================================== */
    {
        imprimere("\n--- Probans XVIII: for (;;) ---\n");

        nodus = _parsare(piscina, intern, "for (;;) x = 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_PER);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.initium == NIHIL), I);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.conditio == NIHIL), I);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.incrementum == NIHIL), I);
        CREDO_NON_NIHIL(nodus->datum.circuitus.corpus);
    }

    /* ========================================================
     * PROBARE XIX: for (i = 0;;) x = 1;  (init only)
     * ======================================================== */
    {
        imprimere("\n--- Probans XIX: for (init;;) ---\n");

        nodus = _parsare(piscina, intern, "for (i = 0;;) x = 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_PER);
        CREDO_NON_NIHIL(nodus->datum.circuitus.initium);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.conditio == NIHIL), I);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.incrementum == NIHIL), I);
    }

    /* ========================================================
     * PROBARE XX: for (;i < 10;) x = 1;  (cond only)
     * ======================================================== */
    {
        imprimere("\n--- Probans XX: for (;cond;) ---\n");

        nodus = _parsare(piscina, intern, "for (;i < 10;) x = 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_PER);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.initium == NIHIL), I);
        CREDO_NON_NIHIL(nodus->datum.circuitus.conditio);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.incrementum == NIHIL), I);
    }

    /* ========================================================
     * PROBARE XXI: switch (x) { case 1: y = 1; break; default: y = 0; break; }
     * ======================================================== */
    {
        imprimere("\n--- Probans XXI: switch/case/default ---\n");

        nodus = _parsare(piscina, intern,
            "switch (x) { case 1: y = 1; break; default: y = 0; break; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_COMMUTATIO);
        CREDO_NON_NIHIL(nodus->datum.selectivum.expressio);
        CREDO_NON_NIHIL(nodus->datum.selectivum.corpus);
        CREDO_AEQUALIS_S32((s32)nodus->datum.selectivum.corpus->genus,
            (s32)ARBOR2_NODUS_CORPUS);
    }

    /* ========================================================
     * PROBARE XXII: label: x = 1;
     * ======================================================== */
    {
        imprimere("\n--- Probans XXII: label: x = 1; ---\n");

        nodus = _parsare(piscina, intern, "label: x = 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TITULATUM);
        CREDO_NON_NIHIL(nodus->datum.titulatum.tok_titulus);
        CREDO_NON_NIHIL(nodus->datum.titulatum.tok_colon);
        CREDO_NON_NIHIL(nodus->datum.titulatum.sententia);
    }

    /* ========================================================
     * PROBARE XXIII: Complexa imbricata -
     *   if (x) { while (y) { if (z) break; else continue; } }
     * ======================================================== */
    {
        Arbor2Nodus* corpus_ext;
        Arbor2Nodus** elem0;
        Arbor2Nodus* dum_nodus;

        imprimere("\n--- Probans XXIII: Imbricata complexa ---\n");

        nodus = _parsare(piscina, intern,
            "if (x) { while (y) { if (z) break; else continue; } }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SI);

        /* consequens est corpus */
        corpus_ext = nodus->datum.conditionale.consequens;
        CREDO_NON_NIHIL(corpus_ext);
        CREDO_AEQUALIS_S32((s32)corpus_ext->genus, (s32)ARBOR2_NODUS_CORPUS);
        CREDO_AEQUALIS_I32((i32)xar_numerus(corpus_ext->datum.corpus.sententiae), I);

        /* primum elementum est while */
        elem0 = (Arbor2Nodus**)xar_obtinere(corpus_ext->datum.corpus.sententiae, ZEPHYRUM);
        dum_nodus = *elem0;
        CREDO_AEQUALIS_S32((s32)dum_nodus->genus, (s32)ARBOR2_NODUS_DUM);
    }

    /* ========================================================
     * PROBARE XXIV: for cum corpore - for (i = 0; i < 5; i = i + 1) { x = x + i; }
     * ======================================================== */
    {
        imprimere("\n--- Probans XXIV: for cum corpore ---\n");

        nodus = _parsare(piscina, intern,
            "for (i = 0; i < 5; i = i + 1) { x = x + i; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_PER);
        CREDO_NON_NIHIL(nodus->datum.circuitus.initium);
        CREDO_NON_NIHIL(nodus->datum.circuitus.conditio);
        CREDO_NON_NIHIL(nodus->datum.circuitus.incrementum);
        CREDO_AEQUALIS_S32((s32)nodus->datum.circuitus.corpus->genus,
            (s32)ARBOR2_NODUS_CORPUS);
    }

    /* ========================================================
     * PROBARE XXV: Declaratio et reditus in corpore
     *   { int x = 1; int y = 2; if (x < y) return x; else return y; }
     * ======================================================== */
    {
        Arbor2Nodus** elems;

        imprimere("\n--- Probans XXV: Declarationes et sententiae ---\n");

        nodus = _parsare(piscina, intern,
            "{ int x = 1; int y = 2; if (x < y) return x; else return y; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_CORPUS);
        CREDO_NON_NIHIL(nodus->datum.corpus.sententiae);
        CREDO_AEQUALIS_I32((i32)xar_numerus(nodus->datum.corpus.sententiae), III);

        /* elem 0, 1 = declarationes, elem 2 = if */
        elems = (Arbor2Nodus**)xar_obtinere(nodus->datum.corpus.sententiae, ZEPHYRUM);
        CREDO_AEQUALIS_S32((s32)(*elems)->genus, (s32)ARBOR2_NODUS_DECLARATIO);
        elems = (Arbor2Nodus**)xar_obtinere(nodus->datum.corpus.sententiae, II);
        CREDO_AEQUALIS_S32((s32)(*elems)->genus, (s32)ARBOR2_NODUS_SI);
    }

    /* ========================================================
     * PROBARE XXVI: Expressio regressio 2 + 3 (per sententia)
     * ======================================================== */
    {
        imprimere("\n--- Probans XXVI: Expressio regressio 2 + 3; ---\n");

        nodus = _parsare(piscina, intern, "2 + 3;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SENTENTIA);
        CREDO_NON_NIHIL(nodus->datum.sententia.expressio);
        CREDO_AEQUALIS_S32((s32)nodus->datum.sententia.expressio->genus,
            (s32)ARBOR2_NODUS_BINARIUM);
    }

    /* ========================================================
     * PROBARE XXVII: for (;; i = i + 1) x = 1; (incr only)
     * ======================================================== */
    {
        imprimere("\n--- Probans XXVII: for (;;incr) ---\n");

        nodus = _parsare(piscina, intern, "for (;; i = i + 1) x = 1;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_PER);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.initium == NIHIL), I);
        CREDO_AEQUALIS_S32((s32)(nodus->datum.circuitus.conditio == NIHIL), I);
        CREDO_NON_NIHIL(nodus->datum.circuitus.incrementum);
    }

    /* ========================================================
     * PROBARE XXVIII: do { x = x + 1; y = y - 1; } while (x < y);
     * ======================================================== */
    {
        imprimere("\n--- Probans XXVIII: do-while cum corpore ---\n");

        nodus = _parsare(piscina, intern,
            "do { x = x + 1; y = y - 1; } while (x < y);");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_FAC);
        CREDO_AEQUALIS_S32((s32)nodus->datum.iteratio.corpus->genus,
            (s32)ARBOR2_NODUS_CORPUS);
    }

    /* ========================================================
     * PROBARE XXIX: while cum corpore - while (1) { break; }
     * ======================================================== */
    {
        imprimere("\n--- Probans XXIX: while (1) { break; } ---\n");

        nodus = _parsare(piscina, intern, "while (1) { break; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.iteratio.corpus->genus,
            (s32)ARBOR2_NODUS_CORPUS);
    }

    /* ========================================================
     * PROBARE XXX: if-else cum corporibus
     * ======================================================== */
    {
        imprimere("\n--- Probans XXX: if-else cum corporibus ---\n");

        nodus = _parsare(piscina, intern,
            "if (x) { y = 1; } else { y = 2; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_SI);
        CREDO_AEQUALIS_S32((s32)nodus->datum.conditionale.consequens->genus,
            (s32)ARBOR2_NODUS_CORPUS);
        CREDO_NON_NIHIL(nodus->datum.conditionale.alternans);
        CREDO_AEQUALIS_S32((s32)nodus->datum.conditionale.alternans->genus,
            (s32)ARBOR2_NODUS_CORPUS);
    }

    /* ======================================================== */

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
