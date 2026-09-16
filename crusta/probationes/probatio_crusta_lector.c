/* probatio_crusta_lector.c - Lector crustae: modi, regiones, heredoc
 *
 * Casus = fons + series modorum quos probatio ipsa eligit (parsator
 * nondum adest): quisque pulsus genus et octetos lexematis asserit,
 * deinde TEGUMENTUM OCTETORUM - valores omnium lexematum (trivia hic
 * lexemata sunt, nihil ligatum) concatenati fontem aequant, FINIS
 * ultimum. Regiones et queue heredoc per faciem publicam probantur.
 * Fontes in piscina allocantur (lexemata in fontem spectant).
 */

#include "latina.h"
#include "credo.h"
#include "crusta_lector.h"
#include "crusta_lexicon.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

nomen structura {
             Piscina* piscina;
        CrustaLector  lector;
                 Xar* lexemata;    /* MateriaToken* */
  constans character* fons;
                 i32  mensura;
} Cursus;

hic_manens i32 casus_numerus = ZEPHYRUM;
hic_manens i32 octeti_tecti  = ZEPHYRUM;

interior constans character*
_copia (
               Piscina* piscina,
    constans character* litterae,
                   i32* mensura)
{
          i32  n = (i32)strlen(litterae);
    character* m = (character*)piscina_allocare(piscina, n + I);

    memcpy(m, litterae, (size_t)n + I);
    *mensura = n;
    redde m;
}

interior vacuum
_incipere (
                Cursus* c,
               Piscina* piscina,
    constans character* litterae)
{
    c->piscina   = piscina;
    c->fons      = _copia(piscina, litterae, &c->mensura);
    c->lexemata  = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    CREDO_VERUM (crusta_lector_incipere(&c->lector, piscina, c->fons,
        c->mensura, &CRUSTA_BASH));
    casus_numerus++;
}

interior vacuum
_addere (
          Cursus* c,
    MateriaToken* t)
{
    MateriaToken** sedes = (MateriaToken**)xar_addere(c->lexemata);

    CREDO_NON_NIHIL (sedes);
    *sedes = t;
}

/* pulsus unus: genus et octeti asserti */
interior MateriaToken*
_pulsare (
                Cursus* c,
           CrustaModus  modus,
        CrustaLexGenus  genus,
    constans character* textus)
{
    MateriaToken* t = crusta_lector_proximum(&c->lector, modus);
             i32  n = (i32)strlen(textus);

    CREDO_NON_NIHIL (t);
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    _addere(c, t);
    si (   t->genus != (s32)genus || t->valor.mensura != n
        || memcmp(t->valor.datum, textus, (size_t)n) != ZEPHYRUM)
    {
        imprimere("    casus %d: exspectatum genus %d '%s', receptum "
            "genus %d '%.*s'\n", (integer)casus_numerus, (integer)genus,
            textus, (integer)t->genus, (integer)t->valor.mensura,
            (constans character*)t->valor.datum);
    }
    CREDO_AEQUALIS_S32 (t->genus, (s32)genus);
    CREDO_AEQUALIS_I32 (t->valor.mensura, n);
    CREDO_VERUM (memcmp(t->valor.datum, textus, (size_t)n) == ZEPHYRUM);
    redde t;
}

/* FINIS in modo dato, deinde tegumentum */
interior vacuum
_finire (
         Cursus* c,
    CrustaModus  modus)
{
    MateriaToken* t = crusta_lector_proximum(&c->lector, modus);
             i32  i;
             i32  offset  = ZEPHYRUM;
             b32  idem    = VERUM;

    CREDO_NON_NIHIL (t);
    CREDO_AEQUALIS_S32 (t->genus, (s32)CRUSTA_LEX_FINIS);
    CREDO_AEQUALIS_I32 (t->valor.mensura, ZEPHYRUM);
    per (i = ZEPHYRUM; i < xar_numerus(c->lexemata); i++)
    {
        MateriaToken* u = *(MateriaToken**)xar_obtinere(c->lexemata, i);

        si (   u->byte_offset != (s32)offset
            || offset + u->valor.mensura > c->mensura
            || memcmp(c->fons + offset, u->valor.datum,
                (size_t)u->valor.mensura) != ZEPHYRUM)
        {
            idem = FALSUM;
            imprimere("    casus %d: tegumentum ruptum ad offset %d "
                "(lexema %d)\n", (integer)casus_numerus,
                (integer)offset,
                (integer)i);
            frange;
        }
        offset += u->valor.mensura;
    }
    CREDO_VERUM (idem);
    CREDO_AEQUALIS_I32 (offset, c->mensura);
    octeti_tecti += offset;
}

/* pulsus per modum: involucra _pulsare, nomina modorum */
interior MateriaToken*
_initium (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_INITIUM, genus, textus);
}

interior MateriaToken*
_verba (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_VERBA, genus, textus);
}

interior MateriaToken*
_assignationes (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_ASSIGNATIONES, genus, textus);
}

interior MateriaToken*
_tabulatum (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_TABULATUM, genus, textus);
}

interior MateriaToken*
_post_titulum (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_POST_TITULUM, genus, textus);
}

interior MateriaToken*
_in_verbis (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_IN_VERBIS, genus, textus);
}

interior MateriaToken*
_exemplar (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_EXEMPLAR, genus, textus);
}

interior MateriaToken*
_gemina (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_GEMINA, genus, textus);
}

interior MateriaToken*
_expansio (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_EXPANSIO, genus, textus);
}

interior MateriaToken*
_expansio_verbum (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_EXPANSIO_VERBUM, genus, textus);
}

interior MateriaToken*
_expansio_exemplar (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_EXPANSIO_EXEMPLAR, genus, textus);
}

interior MateriaToken*
_expansio_sectio (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_EXPANSIO_SECTIO, genus, textus);
}

interior MateriaToken*
_arithmetica_summa (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_ARITHMETICA_SUMMA, genus, textus);
}

interior MateriaToken*
_arithmetica_intra (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_ARITHMETICA_INTRA, genus, textus);
}

interior MateriaToken*
_iudicium (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_IUDICIUM, genus, textus);
}

interior MateriaToken*
_regula (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_REGULA, genus, textus);
}

interior MateriaToken*
_heredoc (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_HEREDOC, genus, textus);
}

interior MateriaToken*
_heredoc_litterale (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_HEREDOC_LITTERALE, genus, textus);
}


                       s32
    principale (vacuum)
    {
        b32  praeteritus;
    Piscina* piscina;
     Cursus  c;

    piscina =
        piscina_generare_dynamicum("probatio_crusta_lector",
        262144);
    si (!piscina)
    {
    imprimere("FRACTA: piscina_generatio\n");
    redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: verba reservata et assignationes positionales
     * ================================================== */

    imprimere("\n--- Probans positionem ---\n");

    _incipere(&c, piscina, "if true; then :; fi");
    _initium(&c, CRUSTA_LEX_RESERVATUM, "if");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "true");
    _verba(&c, CRUSTA_LEX_SEPARATOR, ";");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_RESERVATUM, "then");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, ":");
    _verba(&c, CRUSTA_LEX_SEPARATOR, ";");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_RESERVATUM, "fi");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina, "echo if");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "echo");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "if");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina, "x=if");
    _initium(&c, CRUSTA_LEX_ASSIGNATIO_TITULUS, "x");
    _initium(&c, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "if");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    _incipere(&c, piscina, "a[1]+=v");
    _initium(&c, CRUSTA_LEX_ASSIGNATIO_TITULUS, "a");
    _initium(&c, CRUSTA_LEX_SUBSCRIPTUM, "[1]");
    _initium(&c, CRUSTA_LEX_ASSIGNATIO_OPERATOR,
        "+=");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "v");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    _incipere(&c, piscina, "A=1 if");
    _initium(&c, CRUSTA_LEX_ASSIGNATIO_TITULUS, "A");
    _initium(&c, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "1");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_RESERVATUM, "if");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    /* in VERBA nec reservata nec assignationes */
    _incipere(&c, piscina, "cmd a=b {");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "cmd");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "a=b");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "{");
    _finire(&c, CRUSTA_MODUS_VERBA);

    /* aedificator: assignationes post verbum */
    _incipere(&c, piscina, "x=(1 [2]=y)");
    _assignationes(&c, CRUSTA_LEX_ASSIGNATIO_TITULUS, "x");
    _assignationes(&c, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _assignationes(&c, CRUSTA_LEX_PARENTHESIS, "(");
    _tabulatum(&c, CRUSTA_LEX_LITTERALIS, "1");
    _tabulatum(&c, CRUSTA_LEX_SPATIUM, " ");
    _tabulatum(&c, CRUSTA_LEX_SUBSCRIPTUM, "[2]");
    _tabulatum(&c, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _tabulatum(&c, CRUSTA_LEX_LITTERALIS, "y");
    _tabulatum(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _finire(&c, CRUSTA_MODUS_ASSIGNATIONES);

    /* { } [[ ]] ! reservata; {fd} verbum; !x verbum */
    _incipere(&c, piscina, "{ ! [[ x ]] {fd} !x }");
    _initium(&c, CRUSTA_LEX_RESERVATUM, "{");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_RESERVATUM, "!");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_RESERVATUM, "[[");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_LITTERALIS, "x");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_IUDICIUM_CLAUSURA, "]]");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "{fd}");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "!x");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_RESERVATUM, "}");
    _finire(&c, CRUSTA_MODUS_INITIUM);


    /* ==================================================
     * PROBARE: linea nova per modum (C6), trivia, continuatio
     * ================================================== */

    imprimere("\n--- Probans lineam novam per modum ---\n");

    _incipere(&c, piscina, "a\n");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    _incipere(&c, piscina, "a\n");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _initium(&c, CRUSTA_LEX_LINEA, "\n");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    /* '\r' octetus ordinarius (bash idem): verbum 'a\r', linea '\n' */
    _incipere(&c, piscina, "a\r\n");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a\r");
    _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    _incipere(&c, piscina,
        "a # c\n#!/bin/bash\n  b");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_COMMENTUM, "# c");
    _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _initium(&c, CRUSTA_LEX_COMMENTUM, "#!/bin/bash");
    _initium(&c, CRUSTA_LEX_LINEA, "\n");
    _initium(&c, CRUSTA_LEX_SPATIUM, "  ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "b");
    _finire(&c, CRUSTA_MODUS_VERBA);

    /* commentum solum initio verbi: a#b verbum */
    _incipere(&c, piscina, "a#b");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a#b");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina, "ab\\\ncd");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "ab");
    _initium(&c, CRUSTA_LEX_CONTINUATIO, "\\\n");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "cd");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina, "a \\\n b");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LAMINA, "\\\n");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "b");
    _finire(&c, CRUSTA_MODUS_VERBA);

    /* effugia: paria, impar ante octetum, retroversum in fine */
    _incipere(&c, piscina, "a\\ b \\\\x \\");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _initium(&c, CRUSTA_LEX_EFFUGIUM, "\\ ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "b");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_EFFUGIUM, "\\\\");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "\\");
    _finire(&c, CRUSTA_MODUS_VERBA);


    /* ==================================================
     * PROBARE: operatores, separatores, glutinatio fd
     * ================================================== */

    imprimere("\n--- Probans operatores et fd ---\n");

    _incipere(&c, piscina,
        "a && b || c | d |& e & f ; g ;; h ;& i ;;& j");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_ET, "&&");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "b");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_AUT, "||");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "c");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PIPA, "|");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "d");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PIPA_ERRORIS, "|&");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "e");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_SEPARATOR, "&");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_SEPARATOR, ";");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "g");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_TERMINATOR_OPTIONIS,
        ";;");
    _exemplar(&c, CRUSTA_LEX_SPATIUM, " ");
    _exemplar(&c, CRUSTA_LEX_LITTERALIS, "h");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_TERMINATOR_OPTIONIS,
        ";&");
    _exemplar(&c, CRUSTA_LEX_SPATIUM, " ");
    _exemplar(&c, CRUSTA_LEX_LITTERALIS, "i");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_TERMINATOR_OPTIONIS,
        ";;&");
    _exemplar(&c, CRUSTA_LEX_SPATIUM, " ");
    _exemplar(&c, CRUSTA_LEX_LITTERALIS, "j");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina,
        "2>f 2 >f x2>f {fd}>f &>>f <<<w <<-E >|f <&-");
    _initium(&c, CRUSTA_LEX_REDIRECTIO_FD, "2");
    _initium(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR, ">");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "2");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR, ">");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "x2");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR, ">");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_FD, "{fd}");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR, ">");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        "&>>");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        "<<<");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "w");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        "<<-");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "E");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        ">|");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        "<&");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "-");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina,
        "(( a )) ( b ) <(c) >(d)");
    _initium(&c, CRUSTA_LEX_ARITHMETICA_APERTURA,
        "((");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_VARIABILIS, "a");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_CLAUSURA,
        "))");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_PARENTHESIS, "(");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "b");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PROCESSUS_APERTURA, "<(");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "c");
    _verba(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PROCESSUS_APERTURA, ">(");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "d");
    _verba(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _finire(&c, CRUSTA_MODUS_VERBA);


    /* ==================================================
     * PROBARE: partes verbi
     * ================================================== */

    imprimere("\n--- Probans partes verbi ---\n");

    _incipere(&c, piscina, "\"a$b${c}$(d)\\$\"");
    _initium(&c, CRUSTA_LEX_GEMINA_APERTURA, "\"");
    _gemina(&c, CRUSTA_LEX_LITTERALIS, "a");
    _gemina(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _gemina(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "b");
    _gemina(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "c");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _gemina(&c, CRUSTA_LEX_SUBSTITUTIO_APERTURA,
        "$(");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "d");
    _verba(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _gemina(&c, CRUSTA_LEX_EFFUGIUM, "\\$");
    _gemina(&c, CRUSTA_LEX_GEMINA_CLAUSURA, "\"");
    _finire(&c, CRUSTA_MODUS_VERBA);

    /* intra gemina: retroversum ante x litterale, par effugium, " */
    _incipere(&c, piscina, "\"\\x\\\\y\\\"\"");
    _initium(&c, CRUSTA_LEX_GEMINA_APERTURA, "\"");
    _gemina(&c, CRUSTA_LEX_LITTERALIS, "\\");
    _gemina(&c, CRUSTA_LEX_LITTERALIS, "x");
    _gemina(&c, CRUSTA_LEX_EFFUGIUM, "\\\\");
    _gemina(&c, CRUSTA_LEX_LITTERALIS, "y");
    _gemina(&c, CRUSTA_LEX_EFFUGIUM, "\\\"");
    _gemina(&c, CRUSTA_LEX_GEMINA_CLAUSURA, "\"");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina,
        "'a b'x$'\\t'$\"s\" ~/x a~b");
    _initium(&c, CRUSTA_LEX_SIMPLEX, "'a b'");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "x");
    _initium(&c, CRUSTA_LEX_EFFUGIA, "$'\\t'");
    _initium(&c, CRUSTA_LEX_VERSA_APERTURA, "$\"");
    _gemina(&c, CRUSTA_LEX_LITTERALIS, "s");
    _gemina(&c, CRUSTA_LEX_GEMINA_CLAUSURA, "\"");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_DOMUS, "~");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "/x");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "a~b");
    _finire(&c, CRUSTA_MODUS_VERBA);

    /* ~ initio verbi et post = ; post : litteralis (deviatio) */
    _incipere(&c, piscina, "P=~/a:~b");
    _initium(&c, CRUSTA_LEX_ASSIGNATIO_TITULUS, "P");
    _initium(&c, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _initium(&c, CRUSTA_LEX_DOMUS, "~");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "/a:~b");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    /* $'...' cum apice effugito; simplex ad EOF apertus */
    _incipere(&c, piscina, "$'a\\'b' 'x");
    _initium(&c, CRUSTA_LEX_EFFUGIA, "$'a\\'b'");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_SIMPLEX, "'x");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina,
        "$10 $@ $# $? $- $$ $! $0 $ x $$$y");
    _initium(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _initium(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "1");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "0");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "@");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "#");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "?");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "-");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "$");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "!");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "0");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "$");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "y");
    _finire(&c, CRUSTA_MODUS_VERBA);

    /* verbum unum trium partium: adiacentia per offset */
    {
    MateriaToken* a;
    MateriaToken* b;

    _incipere(&c, piscina, "a$(b)c");
    a = _initium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _initium(&c, CRUSTA_LEX_SUBSTITUTIO_APERTURA,
        "$(");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "b");
    _verba(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    b = _verba(&c, CRUSTA_LEX_LITTERALIS, "c");
    CREDO_AEQUALIS_S32 (b->byte_offset,
        a->byte_offset + (s32)V);
    _finire(&c, CRUSTA_MODUS_VERBA);
    }


    /* ==================================================
     * PROBARE: expansio ${ }
     * ================================================== */

    imprimere("\n--- Probans expansionem ---\n");

    _incipere(&c, piscina,
        "${#x}${#}${!x[@]}${x:-d}");
    _initium(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_PRAEFIXUM, "#");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "#");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_PRAEFIXUM, "!");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&c, CRUSTA_LEX_SUBSCRIPTUM, "[@]");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_OPERATOR, ":-");
    _expansio_verbum(&c, CRUSTA_LEX_LITTERALIS, "d");
    _expansio_verbum(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina,
        "${x//a/b}${x:1:2}${x@Q}${x^^}${x:-a b;}");
    _initium(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_OPERATOR, "//");
    _expansio_exemplar(&c, CRUSTA_LEX_LITTERALIS, "a");
    _expansio_exemplar(&c, CRUSTA_LEX_EXPANSIO_OPERATOR, "/");
    _expansio_verbum(&c, CRUSTA_LEX_LITTERALIS, "b");
    _expansio_verbum(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_OPERATOR, ":");
    _expansio_sectio(&c, CRUSTA_LEX_LITTERALIS, "1");
    _expansio_sectio(&c, CRUSTA_LEX_EXPANSIO_OPERATOR, ":");
    _expansio_sectio(&c, CRUSTA_LEX_LITTERALIS, "2");
    _expansio_sectio(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_OPERATOR, "@");
    _expansio_verbum(&c, CRUSTA_LEX_LITTERALIS, "Q");
    _expansio_verbum(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_OPERATOR, "^^");
    _expansio_verbum(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&c, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&c, CRUSTA_LEX_EXPANSIO_OPERATOR, ":-");
    _expansio_verbum(&c, CRUSTA_LEX_LITTERALIS, "a b;");
    _expansio_verbum(&c, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _finire(&c, CRUSTA_MODUS_VERBA);


    /* ==================================================
     * PROBARE: modi exemplaris, in verbis, post titulum
     * ================================================== */

    imprimere("\n--- Probans exemplar, in verbis, post titulum ---\n");

    _incipere(&c, piscina, "(a|b) x");
    _exemplar(&c, CRUSTA_LEX_PARENTHESIS, "(");
    _exemplar(&c, CRUSTA_LEX_LITTERALIS, "a");
    _exemplar(&c, CRUSTA_LEX_PIPA, "|");
    _exemplar(&c, CRUSTA_LEX_LITTERALIS, "b");
    _exemplar(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _initium(&c, CRUSTA_LEX_SPATIUM, " ");
    _initium(&c, CRUSTA_LEX_LITTERALIS, "x");
    _finire(&c, CRUSTA_MODUS_VERBA);

    _incipere(&c, piscina, "esac if)");
    _exemplar(&c, CRUSTA_LEX_RESERVATUM, "esac");
    _exemplar(&c, CRUSTA_LEX_SPATIUM, " ");
    _exemplar(&c, CRUSTA_LEX_LITTERALIS, "if");
    _exemplar(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    _incipere(&c, piscina, "a b\n");
    _in_verbis(&c, CRUSTA_LEX_LITTERALIS, "a");
    _in_verbis(&c, CRUSTA_LEX_SPATIUM, " ");
    _in_verbis(&c, CRUSTA_LEX_LITTERALIS, "b");
    _in_verbis(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    _incipere(&c, piscina, "x\nin ((");
    _post_titulum(&c, CRUSTA_LEX_LITTERALIS, "x");
    _post_titulum(&c, CRUSTA_LEX_LINEA, "\n");
    _post_titulum(&c, CRUSTA_LEX_RESERVATUM, "in");
    _post_titulum(&c, CRUSTA_LEX_SPATIUM, " ");
    _post_titulum(&c, CRUSTA_LEX_ARITHMETICA_APERTURA,
        "((");
    _finire(&c, CRUSTA_MODUS_ARITHMETICA_SUMMA);


    /* ==================================================
     * PROBARE: arithmetica
     * ================================================== */

    imprimere("\n--- Probans arithmeticam ---\n");

    _incipere(&c, piscina, "1 + 0x1f*(2)))");
    _arithmetica_summa(&c, CRUSTA_LEX_NUMERUS, "1");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "+");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_NUMERUS, "0x1f");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "*");
    _arithmetica_summa(&c, CRUSTA_LEX_PARENTHESIS, "(");
    _arithmetica_intra(&c, CRUSTA_LEX_NUMERUS, "2");
    _arithmetica_intra(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_CLAUSURA,
        "))");
    _finire(&c, CRUSTA_MODUS_VERBA);

    /* )) intra parenthesim: duae clausurae singulae */
    _incipere(&c, piscina, "))");
    _arithmetica_intra(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _arithmetica_intra(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _finire(&c, CRUSTA_MODUS_ARITHMETICA_INTRA);

    _incipere(&c, piscina,
        "16#ff a[i]++ x<<=2 2**3 a?b:c 1,2;$y\n");
    _arithmetica_summa(&c, CRUSTA_LEX_NUMERUS, "16#ff");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_VARIABILIS, "a");
    _arithmetica_summa(&c, CRUSTA_LEX_SUBSCRIPTUM, "[i]");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "++");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_VARIABILIS, "x");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "<<=");
    _arithmetica_summa(&c, CRUSTA_LEX_NUMERUS, "2");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_NUMERUS, "2");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "**");
    _arithmetica_summa(&c, CRUSTA_LEX_NUMERUS, "3");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_VARIABILIS, "a");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "?");
    _arithmetica_summa(&c, CRUSTA_LEX_VARIABILIS, "b");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        ":");
    _arithmetica_summa(&c, CRUSTA_LEX_VARIABILIS, "c");
    _arithmetica_summa(&c, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&c, CRUSTA_LEX_NUMERUS, "1");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        ",");
    _arithmetica_summa(&c, CRUSTA_LEX_NUMERUS, "2");
    _arithmetica_summa(&c, CRUSTA_LEX_ARITHMETICA_SEPARATOR,
        ";");
    _arithmetica_summa(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _arithmetica_summa(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "y");
    _arithmetica_summa(&c, CRUSTA_LEX_LINEA, "\n");
    _finire(&c, CRUSTA_MODUS_ARITHMETICA_SUMMA);


    /* ==================================================
     * PROBARE: iudicium et regula
     * ================================================== */

    imprimere("\n--- Probans iudicium et regulam ---\n");

    _incipere(&c, piscina,
        "-f $x && ( a == b* ) ]]");
    _iudicium(&c, CRUSTA_LEX_IUDICIUM_OPERATOR, "-f");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _iudicium(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_IUDICIUM_OPERATOR, "&&");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_PARENTHESIS, "(");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_IUDICIUM_OPERATOR, "==");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_LITTERALIS, "b*");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_IUDICIUM_CLAUSURA, "]]");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    /* a=b verbum unum; ! et < operatores soli; ]] non verbi */
    _incipere(&c, piscina, "a=b ! < x]]");
    _iudicium(&c, CRUSTA_LEX_LITTERALIS, "a=b");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_IUDICIUM_OPERATOR, "!");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_IUDICIUM_OPERATOR, "<");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_LITTERALIS, "x]]");
    _finire(&c, CRUSTA_MODUS_IUDICIUM);

    _incipere(&c, piscina, "^a(b c)+$ ]]");
    _regula(&c, CRUSTA_LEX_REGULA, "^a(b c)+$");
    _regula(&c, CRUSTA_LEX_SPATIUM, " ");
    _regula(&c, CRUSTA_LEX_IUDICIUM_CLAUSURA, "]]");
    _finire(&c, CRUSTA_MODUS_INITIUM);

    _incipere(&c, piscina, "a|b'c d'$v && ]]");
    _regula(&c, CRUSTA_LEX_REGULA, "a|b");
    _regula(&c, CRUSTA_LEX_SIMPLEX, "'c d'");
    _regula(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _regula(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "v");
    _regula(&c, CRUSTA_LEX_SPATIUM, " ");
    _regula(&c, CRUSTA_LEX_IUDICIUM_OPERATOR, "&&");
    _iudicium(&c, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&c, CRUSTA_LEX_IUDICIUM_CLAUSURA, "]]");
    _finire(&c, CRUSTA_MODUS_INITIUM);


    /* ==================================================
     * PROBARE: regiones backtick cum profunditate
     * ================================================== */

    imprimere("\n--- Probans regiones backtick ---\n");

    {
    s32 finis;

        /* `a \`b\` c` : clausura profunditatis I = backtick nudus
         * ultimus; profunditatis II ab intra = '\`' */
    _incipere(&c, piscina, "`a \\`b\\` c`");
    finis = crusta_lector_gravem_quaerere(&c.lector,
        (s32)I,
        (i32)I);
    CREDO_AEQUALIS_S32 (finis, (s32)X);
    finis = crusta_lector_gravem_quaerere(&c.lector,
        (s32)V,
        (i32)II);
    CREDO_AEQUALIS_S32 (finis, (s32)VI);
    CREDO_AEQUALIS_S32 (crusta_lector_gravem_quaerere(&c.lector,
        (s32)XII, (i32)I), (s32)-I);

    _initium(&c, CRUSTA_LEX_GRAVIS, "`");
    CREDO_VERUM (crusta_lector_regionem_aperire(&c.lector,
        (s32)X,
        (i32)I));
    _initium(&c, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_GRAVIS, "\\`");
    CREDO_VERUM (crusta_lector_regionem_aperire(&c.lector,
        (s32)VI,
        (i32)II));
    _initium(&c, CRUSTA_LEX_LITTERALIS, "b");
    {
    MateriaToken* f =
        crusta_lector_proximum(&c.lector, CRUSTA_MODUS_VERBA);

    CREDO_AEQUALIS_S32 (f->genus, CRUSTA_LEX_FINIS);
    CREDO_AEQUALIS_I32 (crusta_lector_cauda(f)->gravis,
        (i32)II);
    }
    crusta_lector_regionem_claudere(&c.lector);
    _verba(&c, CRUSTA_LEX_GRAVIS, "\\`");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "c");
    {
    MateriaToken* f =
        crusta_lector_proximum(&c.lector, CRUSTA_MODUS_VERBA);

    CREDO_AEQUALIS_S32 (f->genus, CRUSTA_LEX_FINIS);
    CREDO_AEQUALIS_I32 (crusta_lector_cauda(f)->gravis,
        (i32)I);
    }
    crusta_lector_regionem_claudere(&c.lector);
    _verba(&c, CRUSTA_LEX_GRAVIS, "`");
    _finire(&c, CRUSTA_MODUS_VERBA);
    }

    /* intra profunditatem I: '\$x' sigillum duorum octetorum, '\\$'
     * effugium, '\\\\$' effugium retroversorum + sigillum */
    {
    MateriaToken* t;

    _incipere(&c, piscina,
        "`\\$x \\\\$y \\\\\\\\$z`");
    _initium(&c, CRUSTA_LEX_GRAVIS, "`");
    CREDO_VERUM (crusta_lector_regionem_aperire(&c.lector,
        (s32)(c.mensura - I), (i32)I));
    t = _initium(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM,
        "\\$");
    CREDO_AEQUALIS_I32 (crusta_lector_cauda(t)->gravis,
        (i32)I);
    _initium(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_EFFUGIUM, "\\\\$");
    _verba(&c, CRUSTA_LEX_LITTERALIS, "y");
    _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    _verba(&c, CRUSTA_LEX_EFFUGIUM, "\\\\\\\\");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "z");
    {
    MateriaToken* f =
        crusta_lector_proximum(&c.lector, CRUSTA_MODUS_VERBA);

    CREDO_AEQUALIS_S32 (f->genus, CRUSTA_LEX_FINIS);
    }
    crusta_lector_regionem_claudere(&c.lector);
    _verba(&c, CRUSTA_LEX_GRAVIS, "`");
    _finire(&c, CRUSTA_MODUS_VERBA);
    }


    /* ==================================================
     * PROBARE: heredoc - queue, regio, delimitator, finis
     * ================================================== */

    imprimere("\n--- Probans heredoc ---\n");

    {
    CrustaHeredocPetitio p;
    MateriaToken* t;

    _incipere(&c, piscina, "x\nl1\n$v\nA\nrest");
    CREDO_FALSUM (crusta_lector_heredoca_pendent(&c.lector));
    CREDO_VERUM (crusta_lector_heredoc_petere(&c.lector,
        chorda_ex_literis("A", piscina), FALSUM,
        FALSUM));
    CREDO_VERUM (crusta_lector_heredoca_pendent(&c.lector));
    _initium(&c, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&c.lector,
        &p));
    CREDO_FALSUM (p.citatus);
    CREDO_FALSUM (crusta_lector_heredoca_pendent(&c.lector));
    _heredoc(&c, CRUSTA_LEX_LITTERALIS, "l1\n");
    _heredoc(&c, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _heredoc(&c, CRUSTA_LEX_PARAMETRUM_TITULUS, "v");
    _heredoc(&c, CRUSTA_LEX_LITTERALIS, "\n");
    t = crusta_lector_proximum(&c.lector, CRUSTA_MODUS_HEREDOC);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    t = crusta_lector_heredoc_delimitator(&c.lector);
    CREDO_NON_NIHIL (t);
    CREDO_AEQUALIS_S32 (t->genus,
        CRUSTA_LEX_HEREDOC_DELIMITATOR);
    CREDO_AEQUALIS_I32 (t->valor.mensura, (i32)I);
    _addere(&c, t);
    t = crusta_lector_heredoc_finis(&c.lector);
    CREDO_NON_NIHIL (t);
    CREDO_AEQUALIS_S32 (t->genus,
        CRUSTA_LEX_SEPARATOR_LINEAE);
    _addere(&c, t);
    _initium(&c, CRUSTA_LEX_LITTERALIS, "rest");
    _finire(&c, CRUSTA_MODUS_VERBA);

        /* delimitator citatus: corpus litterale unum */
    _incipere(&c, piscina, "x\nl1\n$v\nA\n");
    CREDO_VERUM (crusta_lector_heredoc_petere(&c.lector,
        chorda_ex_literis("A", piscina), VERUM,
        FALSUM));
    _initium(&c, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&c.lector,
        &p));
    CREDO_VERUM (p.citatus);
    _heredoc_litterale(&c, CRUSTA_LEX_LITTERALIS, "l1\n$v\n");
    t = crusta_lector_proximum(&c.lector,
        CRUSTA_MODUS_HEREDOC_LITTERALE);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    _addere(&c,
        crusta_lector_heredoc_delimitator(&c.lector));
    _addere(&c,
        crusta_lector_heredoc_finis(&c.lector));
    _finire(&c, CRUSTA_MODUS_INITIUM);

        /* <<- : tabulae exutae in comparatione, servatae in lexemate */
    _incipere(&c, piscina, "x\n\tb\n\t\tA\nz");
    CREDO_VERUM (crusta_lector_heredoc_petere(&c.lector,
        chorda_ex_literis("A", piscina), FALSUM,
        VERUM));
    _initium(&c, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&c.lector,
        &p));
    _heredoc(&c, CRUSTA_LEX_LITTERALIS, "\tb\n");
    t = crusta_lector_proximum(&c.lector, CRUSTA_MODUS_HEREDOC);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    t = crusta_lector_heredoc_delimitator(&c.lector);
    CREDO_AEQUALIS_I32 (t->valor.mensura, (i32)III);
    _addere(&c, t);
    _addere(&c,
        crusta_lector_heredoc_finis(&c.lector));
    _initium(&c, CRUSTA_LEX_LITTERALIS, "z");
    _finire(&c, CRUSTA_MODUS_VERBA);

        /* delimitator absens: corpus ad EOF, ambo NIHIL */
    _incipere(&c, piscina, "x\nbody\n");
    CREDO_VERUM (crusta_lector_heredoc_petere(&c.lector,
        chorda_ex_literis("A", piscina), FALSUM,
        FALSUM));
    _initium(&c, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&c.lector,
        &p));
    _heredoc(&c, CRUSTA_LEX_LITTERALIS, "body\n");
    t = crusta_lector_proximum(&c.lector, CRUSTA_MODUS_HEREDOC);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    CREDO_NIHIL (crusta_lector_heredoc_delimitator(&c.lector));
    CREDO_NIHIL (crusta_lector_heredoc_finis(&c.lector));
    _finire(&c, CRUSTA_MODUS_INITIUM);

        /* petitiones duae ordine */
    _incipere(&c, piscina, "x\n1\nA\n2\nB\n");
    CREDO_VERUM (crusta_lector_heredoc_petere(&c.lector,
        chorda_ex_literis("A", piscina), FALSUM,
        FALSUM));
    CREDO_VERUM (crusta_lector_heredoc_petere(&c.lector,
        chorda_ex_literis("B", piscina), VERUM,
        FALSUM));
    _initium(&c, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&c.lector,
        &p));
    CREDO_FALSUM (p.citatus);
    _heredoc(&c, CRUSTA_LEX_LITTERALIS, "1\n");
    t = crusta_lector_proximum(&c.lector, CRUSTA_MODUS_HEREDOC);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    _addere(&c,
        crusta_lector_heredoc_delimitator(&c.lector));
    _addere(&c,
        crusta_lector_heredoc_finis(&c.lector));
    CREDO_VERUM (crusta_lector_heredoca_pendent(&c.lector));
    CREDO_VERUM (crusta_lector_heredoc_aperire(&c.lector,
        &p));
    CREDO_VERUM (p.citatus);
    _heredoc_litterale(&c, CRUSTA_LEX_LITTERALIS, "2\n");
    t = crusta_lector_proximum(&c.lector,
        CRUSTA_MODUS_HEREDOC_LITTERALE);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    _addere(&c,
        crusta_lector_heredoc_delimitator(&c.lector));
    _addere(&c,
        crusta_lector_heredoc_finis(&c.lector));
    CREDO_FALSUM (crusta_lector_heredoca_pendent(&c.lector));
    CREDO_FALSUM (crusta_lector_heredoc_aperire(&c.lector,
        &p));
    _finire(&c, CRUSTA_MODUS_INITIUM);
    }


    /* ==================================================
     * PROBARE: positiones et situs reponendus
     * ================================================== */

    imprimere("\n--- Probans positiones ---\n");

    {
    MateriaToken* t;
    CrustaSitus  s;

    _incipere(&c, piscina, "ab\ncd ef\n\ng");
    t = _initium(&c, CRUSTA_LEX_LITTERALIS, "ab");
    CREDO_AEQUALIS_I32 (t->linea, (i32)I);
    CREDO_AEQUALIS_I32 (t->columna, (i32)I);
    s = crusta_lector_situs(&c.lector);
    t = _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE,
        "\n");
    CREDO_AEQUALIS_I32 (t->linea, (i32)I);
    CREDO_AEQUALIS_I32 (t->columna, (i32)III);
    t = _initium(&c, CRUSTA_LEX_LITTERALIS, "cd");
    CREDO_AEQUALIS_I32 (t->linea, (i32)II);
    CREDO_AEQUALIS_I32 (t->columna, (i32)I);
    t = _verba(&c, CRUSTA_LEX_SPATIUM, " ");
    t = _verba(&c, CRUSTA_LEX_LITTERALIS, "ef");
    CREDO_AEQUALIS_I32 (t->linea, (i32)II);
    CREDO_AEQUALIS_I32 (t->columna, (i32)IV);
    t = _verba(&c, CRUSTA_LEX_SEPARATOR_LINEAE,
        "\n");
    CREDO_AEQUALIS_I32 (t->columna, (i32)VI);
    t = _initium(&c, CRUSTA_LEX_LINEA, "\n");
    CREDO_AEQUALIS_I32 (t->linea, (i32)III);
    CREDO_AEQUALIS_I32 (t->columna, (i32)I);
    t = _initium(&c, CRUSTA_LEX_LITTERALIS, "g");
        CREDO_AEQUALIS_I32 (t->linea, (i32)IV);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)X);
        _finire(&c, CRUSTA_MODUS_VERBA);

        /* reponere: eadem lexemata iterum */
        crusta_lector_situm_reponere(&c.lector, s);
        t = crusta_lector_proximum(&c.lector, CRUSTA_MODUS_VERBA);
        CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_SEPARATOR_LINEAE);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)II);
        t = crusta_lector_proximum(&c.lector, CRUSTA_MODUS_INITIUM);
        CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_LITTERALIS);
        CREDO_AEQUALIS_I32 (t->linea, (i32)II);
    }


    imprimere("\n    casus %d, octeti tecti %d\n",
        (integer)casus_numerus,
        (integer)octeti_tecti);
    CREDO_MAIOR_I32 (casus_numerus, (i32)XL);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
