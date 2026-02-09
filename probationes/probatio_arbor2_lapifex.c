/* probatio_arbor2_lapifex.c - Probationes Adaptatoris Arbor2-Lapifex
 *
 * Probat conversionem inter Arbor2Lexema* flumen et LapifexSignum[].
 */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor2_lapifex_adaptator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ================================================
 * Grammatica parva pro probationibus
 *
 * Continet subset terminalium quae in probationibus usantur.
 * ================================================ */

hic_manens constans character* GRAMMATICA_PARVA =
    "<grammatica>"
    "  <terminalia>"
    "    <terminalis titulus=\"IDENTIFICATOR\"/>"
    "    <terminalis titulus=\"PLUS\"/>"
    "    <terminalis titulus=\"ASTERISCUS\"/>"
    "    <terminalis titulus=\"INTEGER\"/>"
    "    <terminalis titulus=\"SEMICOLON\"/>"
    "    <terminalis titulus=\"EOF\"/>"
    "  </terminalia>"
    "  <regulae>"
    "    <regula titulus=\"E\">"
    "      <productio>IDENTIFICATOR</productio>"
    "    </regula>"
    "  </regulae>"
    "  <initium>E</initium>"
    "</grammatica>";

/* ================================================
 * Auxiliaris: Construere grammaticam ex STML
 * ================================================ */

hic_manens LapifexGrammatica*
grammaticam_parare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    constans character*  stml)
{
    LapifexGrammatica* g;

    g = lapifex_grammaticam_legere(piscina, intern, stml);
    si (!g) redde NIHIL;

    lapifex_first_computare(g);
    lapifex_follow_computare(g);

    redde g;
}

/* Auxiliaris: invenire index terminalis per titulum */
hic_manens s32
terminalem_invenire(
    LapifexGrammatica*  grammatica,
    constans character*  titulis)
{
    i32 i;
    i32 num = (i32)xar_numerus(grammatica->symbola);

    per (i = ZEPHYRUM; i < num; i++)
    {
        LapifexSymbolum* sym =
            (LapifexSymbolum*)xar_obtinere(grammatica->symbola, i);
        si (sym && sym->est_terminale)
        {
            si ((i32)sym->titulus->mensura == (i32)strlen(titulis) &&
                memcmp(sym->titulus->datum, titulis,
                    (memoriae_index)sym->titulus->mensura) == ZEPHYRUM)
            {
                redde sym->index;
            }
        }
    }
    redde -I;
}

/* ================================================
 * Probationes
 * ================================================ */

s32 principale(vacuum)
{
    b32                  praeteritus;
    Piscina*             piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_arbor2_lapifex", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina non creata\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Mappa completudo
     *
     * Creare adaptatorem cum GRAMMATICA_PARVA.
     * Verificare quod genera mappata sunt ad correctos indices.
     * ======================================================== */
    {
        LapifexGrammatica*      grammatica;
        Arbor2LapifexAdaptator* adapt;
        s32 idx_identificator;
        s32 idx_plus;
        s32 idx_asteriscus;
        s32 idx_integer;
        s32 idx_semicolon;
        s32 idx_eof;

        imprimere("\n--- Probans I: Mappa completudo ---\n");

        grammatica = grammaticam_parare(piscina, intern, GRAMMATICA_PARVA);
        CREDO_NON_NIHIL(grammatica);

        adapt = arbor2_lapifex_adaptator_creare(piscina, grammatica);
        CREDO_NON_NIHIL(adapt);

        /* Invenire indices terminales in grammatica */
        idx_identificator = terminalem_invenire(grammatica, "IDENTIFICATOR");
        idx_plus          = terminalem_invenire(grammatica, "PLUS");
        idx_asteriscus    = terminalem_invenire(grammatica, "ASTERISCUS");
        idx_integer       = terminalem_invenire(grammatica, "INTEGER");
        idx_semicolon     = terminalem_invenire(grammatica, "SEMICOLON");
        idx_eof           = terminalem_invenire(grammatica, "EOF");

        /* Verificare quod terminalia inventa sunt */
        CREDO_INAEQUALITAS_S32(idx_identificator, -I);
        CREDO_INAEQUALITAS_S32(idx_plus, -I);
        CREDO_INAEQUALITAS_S32(idx_asteriscus, -I);
        CREDO_INAEQUALITAS_S32(idx_integer, -I);
        CREDO_INAEQUALITAS_S32(idx_semicolon, -I);
        CREDO_INAEQUALITAS_S32(idx_eof, -I);

        /* Verificare mappa */
        CREDO_AEQUALIS_S32(
            adapt->mappa[(s32)ARBOR2_LEXEMA_IDENTIFICATOR],
            idx_identificator);
        CREDO_AEQUALIS_S32(
            adapt->mappa[(s32)ARBOR2_LEXEMA_PLUS],
            idx_plus);
        CREDO_AEQUALIS_S32(
            adapt->mappa[(s32)ARBOR2_LEXEMA_ASTERISCUS],
            idx_asteriscus);
        CREDO_AEQUALIS_S32(
            adapt->mappa[(s32)ARBOR2_LEXEMA_INTEGER],
            idx_integer);
        CREDO_AEQUALIS_S32(
            adapt->mappa[(s32)ARBOR2_LEXEMA_SEMICOLON],
            idx_semicolon);
        CREDO_AEQUALIS_S32(
            adapt->mappa[(s32)ARBOR2_LEXEMA_EOF],
            idx_eof);

        /* Trivia debet esse -1 */
        CREDO_AEQUALIS_S32(adapt->mappa[(s32)ARBOR2_LEXEMA_SPATIA], -I);
        CREDO_AEQUALIS_S32(adapt->mappa[(s32)ARBOR2_LEXEMA_TABULAE], -I);
        CREDO_AEQUALIS_S32(adapt->mappa[(s32)ARBOR2_LEXEMA_COMMENTUM_CLAUSUM], -I);
        CREDO_AEQUALIS_S32(adapt->mappa[(s32)ARBOR2_LEXEMA_NOVA_LINEA], -I);

        arbor2_lapifex_mappam_imprimere(adapt);
    }

    /* ========================================================
     * PROBARE II: Conversio basica
     *
     * Lexere "x + 3;" et convertere ad signa.
     * Verificare 5 signa: ID, PLUS, INTEGER, SEMICOLON, EOF
     * ======================================================== */
    {
        LapifexGrammatica*      grammatica;
        Arbor2LapifexAdaptator* adapt;
        Arbor2Lexator*          lexator;
        Xar*                    lexemata;
        Arbor2LapifexFructus    fructus;
        s32 idx_id;
        s32 idx_plus;
        s32 idx_integer;
        s32 idx_semicolon;
        s32 idx_eof;

        imprimere("\n--- Probans II: Conversio basica ---\n");

        grammatica = grammaticam_parare(piscina, intern, GRAMMATICA_PARVA);
        CREDO_NON_NIHIL(grammatica);

        adapt = arbor2_lapifex_adaptator_creare(piscina, grammatica);
        CREDO_NON_NIHIL(adapt);

        /* Lexere fontem */
        lexator = arbor2_lexator_creare(piscina, intern, "x + 3;", VII);
        CREDO_NON_NIHIL(lexator);

        lexemata = arbor2_lexema_omnia(lexator);
        CREDO_NON_NIHIL(lexemata);

        /* Convertere */
        fructus = arbor2_lapifex_convertere(adapt, lexemata);
        CREDO_VERUM(fructus.successus);
        CREDO_AEQUALIS_S32(fructus.numerus, V);

        /* Obtinere indices expectatos */
        idx_id        = terminalem_invenire(grammatica, "IDENTIFICATOR");
        idx_plus      = terminalem_invenire(grammatica, "PLUS");
        idx_integer   = terminalem_invenire(grammatica, "INTEGER");
        idx_semicolon = terminalem_invenire(grammatica, "SEMICOLON");
        idx_eof       = terminalem_invenire(grammatica, "EOF");

        /* Verificare indices terminales */
        CREDO_AEQUALIS_S32(fructus.signa[ZEPHYRUM].terminalis, idx_id);
        CREDO_AEQUALIS_S32(fructus.signa[I].terminalis, idx_plus);
        CREDO_AEQUALIS_S32(fructus.signa[II].terminalis, idx_integer);
        CREDO_AEQUALIS_S32(fructus.signa[III].terminalis, idx_semicolon);
        CREDO_AEQUALIS_S32(fructus.signa[IV].terminalis, idx_eof);
    }

    /* ========================================================
     * PROBARE III: Valor preservatio
     *
     * Verificare quod valor in signo continet pointer ad
     * lexema originale cum genere et valore correcto.
     * ======================================================== */
    {
        LapifexGrammatica*      grammatica;
        Arbor2LapifexAdaptator* adapt;
        Arbor2Lexator*          lexator;
        Xar*                    lexemata;
        Arbor2LapifexFructus    fructus;
        Arbor2Lexema*           recuperatum;

        imprimere("\n--- Probans III: Valor preservatio ---\n");

        grammatica = grammaticam_parare(piscina, intern, GRAMMATICA_PARVA);
        CREDO_NON_NIHIL(grammatica);

        adapt = arbor2_lapifex_adaptator_creare(piscina, grammatica);
        CREDO_NON_NIHIL(adapt);

        lexator = arbor2_lexator_creare(piscina, intern, "x + 3;", VII);
        CREDO_NON_NIHIL(lexator);

        lexemata = arbor2_lexema_omnia(lexator);
        CREDO_NON_NIHIL(lexemata);

        fructus = arbor2_lapifex_convertere(adapt, lexemata);
        CREDO_VERUM(fructus.successus);

        /* Recuperare lexema ex primo signo (x) */
        recuperatum = (Arbor2Lexema*)(vacuum*)(longus)fructus.signa[ZEPHYRUM].valor;
        CREDO_NON_NIHIL(recuperatum);
        CREDO_AEQUALIS_S32((s32)recuperatum->genus, (s32)ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(recuperatum->valor.mensura, I);
        CREDO_VERUM(recuperatum->valor.datum[ZEPHYRUM] == 'x');

        /* Recuperare lexema ex secundo signo (+) */
        recuperatum = (Arbor2Lexema*)(vacuum*)(longus)fructus.signa[I].valor;
        CREDO_NON_NIHIL(recuperatum);
        CREDO_AEQUALIS_S32((s32)recuperatum->genus, (s32)ARBOR2_LEXEMA_PLUS);

        /* Recuperare lexema ex tertio signo (3) */
        recuperatum = (Arbor2Lexema*)(vacuum*)(longus)fructus.signa[II].valor;
        CREDO_NON_NIHIL(recuperatum);
        CREDO_AEQUALIS_S32((s32)recuperatum->genus, (s32)ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(recuperatum->valor.mensura, I);
        CREDO_VERUM(recuperatum->valor.datum[ZEPHYRUM] == '3');
    }

    /* ========================================================
     * PROBARE IV: Trivia filtratio
     *
     * Lexere "x  +  3 ;" (spatia extra) et verificare quod
     * trivia filtrantur et output est idem.
     * ======================================================== */
    {
        LapifexGrammatica*      grammatica;
        Arbor2LapifexAdaptator* adapt;
        Arbor2Lexator*          lexator;
        Xar*                    lexemata;
        Arbor2LapifexFructus    fructus;

        imprimere("\n--- Probans IV: Trivia filtratio ---\n");

        grammatica = grammaticam_parare(piscina, intern, GRAMMATICA_PARVA);
        CREDO_NON_NIHIL(grammatica);

        adapt = arbor2_lapifex_adaptator_creare(piscina, grammatica);
        CREDO_NON_NIHIL(adapt);

        lexator = arbor2_lexator_creare(piscina, intern,
            "x  +  3 ;", X);
        CREDO_NON_NIHIL(lexator);

        lexemata = arbor2_lexema_omnia(lexator);
        CREDO_NON_NIHIL(lexemata);

        fructus = arbor2_lapifex_convertere(adapt, lexemata);
        CREDO_VERUM(fructus.successus);
        /* Debet habere idem 5 signa (ID, PLUS, INTEGER, SEMICOLON, EOF) */
        CREDO_AEQUALIS_S32(fructus.numerus, V);
        /* Et aliqua trivia filtrata */
        CREDO_MAIOR_S32(fructus.numerus_ignoratorum, ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE V: Vacuum input
     *
     * Input vacuum (solum EOF) -> unum signum EOF.
     * ======================================================== */
    {
        LapifexGrammatica*      grammatica;
        Arbor2LapifexAdaptator* adapt;
        Arbor2Lexator*          lexator;
        Xar*                    lexemata;
        Arbor2LapifexFructus    fructus;

        imprimere("\n--- Probans V: Vacuum input ---\n");

        grammatica = grammaticam_parare(piscina, intern, GRAMMATICA_PARVA);
        CREDO_NON_NIHIL(grammatica);

        adapt = arbor2_lapifex_adaptator_creare(piscina, grammatica);
        CREDO_NON_NIHIL(adapt);

        lexator = arbor2_lexator_creare(piscina, intern, "", ZEPHYRUM);
        CREDO_NON_NIHIL(lexator);

        lexemata = arbor2_lexema_omnia(lexator);
        CREDO_NON_NIHIL(lexemata);

        fructus = arbor2_lapifex_convertere(adapt, lexemata);
        CREDO_VERUM(fructus.successus);
        /* Solum EOF signum */
        CREDO_AEQUALIS_S32(fructus.numerus, I);
    }

    /* ========================================================
     * PROBARE VI: est_trivia functio
     *
     * Verificare quod est_trivia correcte classificat genera.
     * ======================================================== */
    {
        imprimere("\n--- Probans VI: est_trivia ---\n");

        /* Trivia */
        CREDO_VERUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_SPATIA));
        CREDO_VERUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_TABULAE));
        CREDO_VERUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_CONTINUATIO));
        CREDO_VERUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_COMMENTUM_CLAUSUM));
        CREDO_VERUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_COMMENTUM_LINEA));
        CREDO_VERUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_NOVA_LINEA));
        CREDO_VERUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_ERROR));
        CREDO_VERUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_TYPEDEF_MACRO));

        /* Non trivia */
        CREDO_FALSUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_IDENTIFICATOR));
        CREDO_FALSUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_PLUS));
        CREDO_FALSUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_SEMICOLON));
        CREDO_FALSUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_INT));
        CREDO_FALSUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_EOF));
        CREDO_FALSUM(arbor2_lapifex_est_trivia(ARBOR2_LEXEMA_ASTERISCUS));
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
