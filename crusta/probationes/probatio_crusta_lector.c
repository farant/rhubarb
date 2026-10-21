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
                 i32  casus_numerus;   /* trans casus sectionis */
                 i32  octeti_tecti;    /* (_incipere non reficit) */
} Cursus;

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
    c->casus_numerus++;
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
            "genus %d '%.*s'\n", (integer)c->casus_numerus,
            (integer)genus,
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
                "(lexema %d)\n", (integer)c->casus_numerus,
                (integer)offset,
                (integer)i);
            frange;
        }
        offset += u->valor.mensura;
    }
    CREDO_VERUM (idem);
    CREDO_AEQUALIS_I32 (offset, c->mensura);
    c->octeti_tecti += offset;
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
_post_compositum (
                Cursus* c,
        CrustaLexGenus  genus,
    constans character* textus)
{
    redde _pulsare(c, CRUSTA_MODUS_POST_COMPOSITUM, genus, textus);
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

/* Sectio UNA: pinna 'casus > XL' casus OMNIUM blocorum numerat
 * (assertio aggregata) - divisio numeros mutaret; gradus
 * (CREDO_GRADUS) fracturam nominant. */
interior vacuum
_casus_probare (
    CredoContextus* c)
{
            Piscina* piscina = c->piscina;
             Cursus  cursus;

    memset(&cursus, ZEPHYRUM, magnitudo(cursus));


    /* ==================================================
     * PROBARE: verba reservata et assignationes positionales
     * ================================================== */

    CREDO_GRADUS ("positionem");

    _incipere(&cursus, piscina, "if true; then :; fi");
    _initium(&cursus, CRUSTA_LEX_RESERVATUM, "if");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "true");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR, ";");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_RESERVATUM, "then");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, ":");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR, ";");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_RESERVATUM, "fi");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina, "echo if");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "echo");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "if");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina, "x=if");
    _initium(&cursus, CRUSTA_LEX_ASSIGNATIO_TITULUS, "x");
    _initium(&cursus, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "if");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    _incipere(&cursus, piscina, "a[1]+=v");
    _initium(&cursus, CRUSTA_LEX_ASSIGNATIO_TITULUS, "a");
    _initium(&cursus, CRUSTA_LEX_SUBSCRIPTUM, "[1]");
    _initium(&cursus, CRUSTA_LEX_ASSIGNATIO_OPERATOR,
        "+=");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "v");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    _incipere(&cursus, piscina, "A=1 if");
    _initium(&cursus, CRUSTA_LEX_ASSIGNATIO_TITULUS, "A");
    _initium(&cursus, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "1");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_RESERVATUM, "if");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    /* in VERBA nec reservata nec assignationes */
    _incipere(&cursus, piscina, "cmd a=b {");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "cmd");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "a=b");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "{");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    /* aedificator: assignationes post verbum */
    _incipere(&cursus, piscina, "x=(1 [2]=y)");
    _assignationes(&cursus, CRUSTA_LEX_ASSIGNATIO_TITULUS, "x");
    _assignationes(&cursus, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _assignationes(&cursus, CRUSTA_LEX_PARENTHESIS, "(");
    _tabulatum(&cursus, CRUSTA_LEX_LITTERALIS, "1");
    _tabulatum(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _tabulatum(&cursus, CRUSTA_LEX_SUBSCRIPTUM, "[2]");
    _tabulatum(&cursus, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _tabulatum(&cursus, CRUSTA_LEX_LITTERALIS, "y");
    _tabulatum(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _finire(&cursus, CRUSTA_MODUS_ASSIGNATIONES);

    /* { } [[ ]] ! reservata; {fd} verbum; !x verbum */
    _incipere(&cursus, piscina, "{ ! [[ x ]] {fd} !x }");
    _initium(&cursus, CRUSTA_LEX_RESERVATUM, "{");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_RESERVATUM, "!");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_RESERVATUM, "[[");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_IUDICIUM_CLAUSURA, "]]");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "{fd}");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "!x");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_RESERVATUM, "}");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);


    /* ==================================================
     * PROBARE: linea nova per modum (C6), trivia, continuatio
     * ================================================== */

    CREDO_GRADUS ("lineam novam per modum");

    _incipere(&cursus, piscina, "a\n");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    _incipere(&cursus, piscina, "a\n");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _initium(&cursus, CRUSTA_LEX_LINEA, "\n");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    /* '\r' octetus ordinarius (bash idem): verbum 'a\r', linea '\n' */
    _incipere(&cursus, piscina, "a\r\n");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a\r");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    _incipere(&cursus, piscina,
        "a # c\n#!/bin/bash\n  b");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_COMMENTUM, "# c");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _initium(&cursus, CRUSTA_LEX_COMMENTUM, "#!/bin/bash");
    _initium(&cursus, CRUSTA_LEX_LINEA, "\n");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, "  ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    /* commentum solum initio verbi: a#b verbum */
    _incipere(&cursus, piscina, "a#b");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a#b");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina, "ab\\\ncd");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "ab");
    _initium(&cursus, CRUSTA_LEX_CONTINUATIO, "\\\n");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "cd");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina, "a \\\n b");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LAMINA, "\\\n");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    /* effugia: paria, impar ante octetum, retroversum in fine */
    _incipere(&cursus, piscina, "a\\ b \\\\x \\");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _initium(&cursus, CRUSTA_LEX_EFFUGIUM, "\\ ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_EFFUGIUM, "\\\\");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "\\");
    _finire(&cursus, CRUSTA_MODUS_VERBA);


    /* ==================================================
     * PROBARE: operatores, separatores, glutinatio fd
     * ================================================== */

    CREDO_GRADUS ("operatores et fd");

    _incipere(&cursus, piscina,
        "a && b || c | d |& e & f ; g ;; h ;& i ;;& j");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_ET, "&&");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_AUT, "||");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "c");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PIPA, "|");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "d");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PIPA_ERRORIS, "|&");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "e");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR, "&");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR, ";");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "g");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_TERMINATOR_OPTIONIS,
        ";;");
    _exemplar(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _exemplar(&cursus, CRUSTA_LEX_LITTERALIS, "h");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_TERMINATOR_OPTIONIS,
        ";&");
    _exemplar(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _exemplar(&cursus, CRUSTA_LEX_LITTERALIS, "i");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_TERMINATOR_OPTIONIS,
        ";;&");
    _exemplar(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _exemplar(&cursus, CRUSTA_LEX_LITTERALIS, "j");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina,
        "2>f 2 >f x2>f {fd}>f &>>f <<<w <<-E >|f <&-");
    _initium(&cursus, CRUSTA_LEX_REDIRECTIO_FD, "2");
    _initium(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR, ">");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "2");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR, ">");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "x2");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR, ">");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_FD, "{fd}");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR, ">");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        "&>>");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        "<<<");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "w");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        "<<-");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "E");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        ">|");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "f");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_REDIRECTIO_OPERATOR,
        "<&");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "-");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina,
        "(( a )) ( b ) <(c) >(d)");
    _initium(&cursus, CRUSTA_LEX_ARITHMETICA_APERTURA,
        "((");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_VARIABILIS, "a");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_CLAUSURA,
        "))");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_PARENTHESIS, "(");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PROCESSUS_APERTURA, "<(");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "c");
    _verba(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PROCESSUS_APERTURA, ">(");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "d");
    _verba(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _finire(&cursus, CRUSTA_MODUS_VERBA);


    /* ==================================================
     * PROBARE: partes verbi
     * ================================================== */

    CREDO_GRADUS ("partes verbi");

    _incipere(&cursus, piscina, "\"a$b${c}$(d)\\$\"");
    _initium(&cursus, CRUSTA_LEX_GEMINA_APERTURA, "\"");
    _gemina(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _gemina(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _gemina(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "b");
    _gemina(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "c");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _gemina(&cursus, CRUSTA_LEX_SUBSTITUTIO_APERTURA,
        "$(");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "d");
    _verba(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _gemina(&cursus, CRUSTA_LEX_EFFUGIUM, "\\$");
    _gemina(&cursus, CRUSTA_LEX_GEMINA_CLAUSURA, "\"");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    /* intra gemina: retroversum ante x litterale, par effugium, " */
    _incipere(&cursus, piscina, "\"\\x\\\\y\\\"\"");
    _initium(&cursus, CRUSTA_LEX_GEMINA_APERTURA, "\"");
    _gemina(&cursus, CRUSTA_LEX_LITTERALIS, "\\");
    _gemina(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _gemina(&cursus, CRUSTA_LEX_EFFUGIUM, "\\\\");
    _gemina(&cursus, CRUSTA_LEX_LITTERALIS, "y");
    _gemina(&cursus, CRUSTA_LEX_EFFUGIUM, "\\\"");
    _gemina(&cursus, CRUSTA_LEX_GEMINA_CLAUSURA, "\"");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina,
        "'a b'x$'\\t'$\"s\" ~/x a~b");
    _initium(&cursus, CRUSTA_LEX_SIMPLEX, "'a b'");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _initium(&cursus, CRUSTA_LEX_EFFUGIA, "$'\\t'");
    _initium(&cursus, CRUSTA_LEX_VERSA_APERTURA, "$\"");
    _gemina(&cursus, CRUSTA_LEX_LITTERALIS, "s");
    _gemina(&cursus, CRUSTA_LEX_GEMINA_CLAUSURA, "\"");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_DOMUS, "~");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "/x");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "a~b");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    /* ~ initio verbi et post = ; post : litteralis (deviatio) */
    _incipere(&cursus, piscina, "P=~/a:~b");
    _initium(&cursus, CRUSTA_LEX_ASSIGNATIO_TITULUS, "P");
    _initium(&cursus, CRUSTA_LEX_ASSIGNATIO_OPERATOR, "=");
    _initium(&cursus, CRUSTA_LEX_DOMUS, "~");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "/a:~b");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    /* $'...' cum apice effugito; simplex ad EOF apertus */
    _incipere(&cursus, piscina, "$'a\\'b' 'x");
    _initium(&cursus, CRUSTA_LEX_EFFUGIA, "$'a\\'b'");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_SIMPLEX, "'x");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina,
        "$10 $@ $# $? $- $$ $! $0 $ x $$$y");
    _initium(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _initium(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "1");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "0");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "@");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "#");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "?");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "-");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "$");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "!");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "0");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "$");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "y");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    /* verbum unum trium partium: adiacentia per offset */
    {
    MateriaToken* a;
    MateriaToken* b;

    _incipere(&cursus, piscina, "a$(b)c");
    a = _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _initium(&cursus, CRUSTA_LEX_SUBSTITUTIO_APERTURA,
        "$(");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _verba(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    b = _verba(&cursus, CRUSTA_LEX_LITTERALIS, "c");
    CREDO_AEQUALIS_S32 (b->byte_offset,
        a->byte_offset + (s32)V);
    _finire(&cursus, CRUSTA_MODUS_VERBA);
    }


    /* ==================================================
     * PROBARE: expansio ${ }
     * ================================================== */

    CREDO_GRADUS ("expansionem");

    _incipere(&cursus, piscina,
        "${#x}${#}${!x[@]}${x:-d}");
    _initium(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_PRAEFIXUM, "#");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "#");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_PRAEFIXUM, "!");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&cursus, CRUSTA_LEX_SUBSCRIPTUM, "[@]");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_OPERATOR, ":-");
    _expansio_verbum(&cursus, CRUSTA_LEX_LITTERALIS, "d");
    _expansio_verbum(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina,
        "${x//a/b}${x:1:2}${x@Q}${x^^}${x:-a b;}");
    _initium(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_OPERATOR, "//");
    _expansio_exemplar(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _expansio_exemplar(&cursus, CRUSTA_LEX_EXPANSIO_OPERATOR, "/");
    _expansio_verbum(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _expansio_verbum(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_OPERATOR, ":");
    _expansio_sectio(&cursus, CRUSTA_LEX_LITTERALIS, "1");
    _expansio_sectio(&cursus, CRUSTA_LEX_EXPANSIO_OPERATOR, ":");
    _expansio_sectio(&cursus, CRUSTA_LEX_LITTERALIS, "2");
    _expansio_sectio(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_OPERATOR, "@");
    _expansio_verbum(&cursus, CRUSTA_LEX_LITTERALIS, "Q");
    _expansio_verbum(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_OPERATOR, "^^");
    _expansio_verbum(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _verba(&cursus, CRUSTA_LEX_EXPANSIO_APERTURA, "${");
    _expansio(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _expansio(&cursus, CRUSTA_LEX_EXPANSIO_OPERATOR, ":-");
    _expansio_verbum(&cursus, CRUSTA_LEX_LITTERALIS, "a b;");
    _expansio_verbum(&cursus, CRUSTA_LEX_EXPANSIO_CLAUSURA, "}");
    _finire(&cursus, CRUSTA_MODUS_VERBA);


    /* ==================================================
     * PROBARE: modi exemplaris, in verbis, post titulum
     * ================================================== */

    CREDO_GRADUS ("exemplar, in verbis, post titulum");

    _incipere(&cursus, piscina, "(a|b) x");
    _exemplar(&cursus, CRUSTA_LEX_PARENTHESIS, "(");
    _exemplar(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _exemplar(&cursus, CRUSTA_LEX_PIPA, "|");
    _exemplar(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _exemplar(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _initium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    _incipere(&cursus, piscina, "esac if)");
    _exemplar(&cursus, CRUSTA_LEX_RESERVATUM, "esac");
    _exemplar(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _exemplar(&cursus, CRUSTA_LEX_LITTERALIS, "if");
    _exemplar(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    _incipere(&cursus, piscina, "a b\n");
    _in_verbis(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _in_verbis(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _in_verbis(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    _in_verbis(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    _incipere(&cursus, piscina, "x\nin ((");
    _post_titulum(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _post_titulum(&cursus, CRUSTA_LEX_LINEA, "\n");
    _post_titulum(&cursus, CRUSTA_LEX_RESERVATUM, "in");
    _post_titulum(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _post_titulum(&cursus, CRUSTA_LEX_ARITHMETICA_APERTURA,
        "((");
    _finire(&cursus, CRUSTA_MODUS_ARITHMETICA_SUMMA);

    /* post clausuram compositi: '}' 'fi' reservata, linea nova
     * terminat, 'x=1' verbum nudum, '((' non arithmetica */
    _incipere(&cursus, piscina, "} fi\nx=1 ((");
    _post_compositum(&cursus, CRUSTA_LEX_RESERVATUM, "}");
    _post_compositum(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _post_compositum(&cursus, CRUSTA_LEX_RESERVATUM, "fi");
    _post_compositum(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    _post_compositum(&cursus, CRUSTA_LEX_LITTERALIS, "x=1");
    _post_compositum(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _post_compositum(&cursus, CRUSTA_LEX_PARENTHESIS, "(");
    _post_compositum(&cursus, CRUSTA_LEX_PARENTHESIS, "(");
    _finire(&cursus, CRUSTA_MODUS_POST_COMPOSITUM);


    /* ==================================================
     * PROBARE: arithmetica
     * ================================================== */

    CREDO_GRADUS ("arithmeticam");

    _incipere(&cursus, piscina, "1 + 0x1f*(2)))");
    _arithmetica_summa(&cursus, CRUSTA_LEX_NUMERUS, "1");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "+");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_NUMERUS, "0x1f");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "*");
    _arithmetica_summa(&cursus, CRUSTA_LEX_PARENTHESIS, "(");
    _arithmetica_intra(&cursus, CRUSTA_LEX_NUMERUS, "2");
    _arithmetica_intra(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_CLAUSURA,
        "))");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

    /* )) intra parenthesim: duae clausurae singulae */
    _incipere(&cursus, piscina, "))");
    _arithmetica_intra(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _arithmetica_intra(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _finire(&cursus, CRUSTA_MODUS_ARITHMETICA_INTRA);

    _incipere(&cursus, piscina,
        "16#ff a[i]++ x<<=2 2**3 a?b:c 1,2;$y\n");
    _arithmetica_summa(&cursus, CRUSTA_LEX_NUMERUS, "16#ff");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_VARIABILIS, "a");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SUBSCRIPTUM, "[i]");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "++");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_VARIABILIS, "x");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "<<=");
    _arithmetica_summa(&cursus, CRUSTA_LEX_NUMERUS, "2");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_NUMERUS, "2");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "**");
    _arithmetica_summa(&cursus, CRUSTA_LEX_NUMERUS, "3");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_VARIABILIS, "a");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        "?");
    _arithmetica_summa(&cursus, CRUSTA_LEX_VARIABILIS, "b");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        ":");
    _arithmetica_summa(&cursus, CRUSTA_LEX_VARIABILIS, "c");
    _arithmetica_summa(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _arithmetica_summa(&cursus, CRUSTA_LEX_NUMERUS, "1");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_OPERATOR,
        ",");
    _arithmetica_summa(&cursus, CRUSTA_LEX_NUMERUS, "2");
    _arithmetica_summa(&cursus, CRUSTA_LEX_ARITHMETICA_SEPARATOR,
        ";");
    _arithmetica_summa(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _arithmetica_summa(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "y");
    _arithmetica_summa(&cursus, CRUSTA_LEX_LINEA, "\n");
    _finire(&cursus, CRUSTA_MODUS_ARITHMETICA_SUMMA);


    /* ==================================================
     * PROBARE: iudicium et regula
     * ================================================== */

    CREDO_GRADUS ("iudicium et regulam");

    _incipere(&cursus, piscina,
        "-f $x && ( a == b* ) ]]");
    _iudicium(&cursus, CRUSTA_LEX_IUDICIUM_OPERATOR, "-f");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _iudicium(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_IUDICIUM_OPERATOR, "&&");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_PARENTHESIS, "(");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_IUDICIUM_OPERATOR, "==");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_LITTERALIS, "b*");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_PARENTHESIS_CLAUSURA,
        ")");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_IUDICIUM_CLAUSURA, "]]");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    /* a=b verbum unum; ! et < operatores soli; ]] non verbi */
    _incipere(&cursus, piscina, "a=b ! < x]]");
    _iudicium(&cursus, CRUSTA_LEX_LITTERALIS, "a=b");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_IUDICIUM_OPERATOR, "!");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_IUDICIUM_OPERATOR, "<");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_LITTERALIS, "x]]");
    _finire(&cursus, CRUSTA_MODUS_IUDICIUM);

    _incipere(&cursus, piscina, "^a(b c)+$ ]]");
    _regula(&cursus, CRUSTA_LEX_REGULA, "^a(b c)+$");
    _regula(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _regula(&cursus, CRUSTA_LEX_IUDICIUM_CLAUSURA, "]]");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

    _incipere(&cursus, piscina, "a|b'c d'$v && ]]");
    _regula(&cursus, CRUSTA_LEX_REGULA, "a|b");
    _regula(&cursus, CRUSTA_LEX_SIMPLEX, "'c d'");
    _regula(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _regula(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "v");
    _regula(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _regula(&cursus, CRUSTA_LEX_IUDICIUM_OPERATOR, "&&");
    _iudicium(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _iudicium(&cursus, CRUSTA_LEX_IUDICIUM_CLAUSURA, "]]");
    _finire(&cursus, CRUSTA_MODUS_INITIUM);


    /* ==================================================
     * PROBARE: regiones backtick cum profunditate
     * ================================================== */

    CREDO_GRADUS ("regiones backtick");

    {
    s32 finis;

        /* `a \`b\` c` : clausura profunditatis I = backtick nudus
         * ultimus; profunditatis II ab intra = '\`' */
    _incipere(&cursus, piscina, "`a \\`b\\` c`");
    finis = crusta_lector_gravem_quaerere(&cursus.lector,
        (s32)I,
        (i32)I);
    CREDO_AEQUALIS_S32 (finis, (s32)X);
    finis = crusta_lector_gravem_quaerere(&cursus.lector,
        (s32)V,
        (i32)II);
    CREDO_AEQUALIS_S32 (finis, (s32)VI);
    CREDO_AEQUALIS_S32 (crusta_lector_gravem_quaerere(&cursus.lector,
        (s32)XII, (i32)I), (s32)-I);

    _initium(&cursus, CRUSTA_LEX_GRAVIS, "`");
    CREDO_VERUM (crusta_lector_regionem_aperire(&cursus.lector,
        (s32)X,
        (i32)I));
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "a");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_GRAVIS, "\\`");
    CREDO_VERUM (crusta_lector_regionem_aperire(&cursus.lector,
        (s32)VI,
        (i32)II));
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "b");
    {
    MateriaToken* f =
        crusta_lector_proximum(&cursus.lector, CRUSTA_MODUS_VERBA);

    CREDO_AEQUALIS_S32 (f->genus, CRUSTA_LEX_FINIS);
    CREDO_AEQUALIS_I32 (crusta_lector_cauda(f)->gravis,
        (i32)II);
    }
    crusta_lector_regionem_claudere(&cursus.lector);
    _verba(&cursus, CRUSTA_LEX_GRAVIS, "\\`");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "c");
    {
    MateriaToken* f =
        crusta_lector_proximum(&cursus.lector, CRUSTA_MODUS_VERBA);

    CREDO_AEQUALIS_S32 (f->genus, CRUSTA_LEX_FINIS);
    CREDO_AEQUALIS_I32 (crusta_lector_cauda(f)->gravis,
        (i32)I);
    }
    crusta_lector_regionem_claudere(&cursus.lector);
    _verba(&cursus, CRUSTA_LEX_GRAVIS, "`");
    _finire(&cursus, CRUSTA_MODUS_VERBA);
    }

    /* intra profunditatem I: '\$x' sigillum duorum octetorum, '\\$'
     * effugium, '\\\\$' effugium retroversorum + sigillum */
    {
    MateriaToken* t;

    _incipere(&cursus, piscina,
        "`\\$x \\\\$y \\\\\\\\$z`");
    _initium(&cursus, CRUSTA_LEX_GRAVIS, "`");
    CREDO_VERUM (crusta_lector_regionem_aperire(&cursus.lector,
        (s32)(cursus.mensura - I), (i32)I));
    t = _initium(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM,
        "\\$");
    CREDO_AEQUALIS_I32 (crusta_lector_cauda(t)->gravis,
        (i32)I);
    _initium(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "x");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_EFFUGIUM, "\\\\$");
    _verba(&cursus, CRUSTA_LEX_LITTERALIS, "y");
    _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    _verba(&cursus, CRUSTA_LEX_EFFUGIUM, "\\\\\\\\");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _verba(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "z");
    {
    MateriaToken* f =
        crusta_lector_proximum(&cursus.lector, CRUSTA_MODUS_VERBA);

    CREDO_AEQUALIS_S32 (f->genus, CRUSTA_LEX_FINIS);
    }
    crusta_lector_regionem_claudere(&cursus.lector);
    _verba(&cursus, CRUSTA_LEX_GRAVIS, "`");
    _finire(&cursus, CRUSTA_MODUS_VERBA);
    }


    /* ==================================================
     * PROBARE: heredoc - queue, regio, delimitator, finis
     * ================================================== */

    CREDO_GRADUS ("heredoc");

    {
    CrustaHeredocPetitio p;
    MateriaToken* t;

    _incipere(&cursus, piscina, "x\nl1\n$v\nA\nrest");
    CREDO_FALSUM (crusta_lector_heredoca_pendent(&cursus.lector));
    CREDO_VERUM (crusta_lector_heredoc_petere(&cursus.lector,
        chorda_ex_literis("A", piscina), FALSUM,
        FALSUM));
    CREDO_VERUM (crusta_lector_heredoca_pendent(&cursus.lector));
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&cursus.lector,
        &p));
    CREDO_FALSUM (p.citatus);
    CREDO_FALSUM (crusta_lector_heredoca_pendent(&cursus.lector));
    _heredoc(&cursus, CRUSTA_LEX_LITTERALIS, "l1\n");
    _heredoc(&cursus, CRUSTA_LEX_PARAMETRUM_SIGILLUM, "$");
    _heredoc(&cursus, CRUSTA_LEX_PARAMETRUM_TITULUS, "v");
    _heredoc(&cursus, CRUSTA_LEX_LITTERALIS, "\n");
    t = crusta_lector_proximum(&cursus.lector, CRUSTA_MODUS_HEREDOC);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    t = crusta_lector_heredoc_delimitator(&cursus.lector);
    CREDO_NON_NIHIL (t);
    CREDO_AEQUALIS_S32 (t->genus,
        CRUSTA_LEX_HEREDOC_DELIMITATOR);
    CREDO_AEQUALIS_I32 (t->valor.mensura, (i32)I);
    _addere(&cursus, t);
    t = crusta_lector_heredoc_finis(&cursus.lector);
    CREDO_NON_NIHIL (t);
    CREDO_AEQUALIS_S32 (t->genus,
        CRUSTA_LEX_SEPARATOR_LINEAE);
    _addere(&cursus, t);
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "rest");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

        /* delimitator citatus: corpus litterale unum */
    _incipere(&cursus, piscina, "x\nl1\n$v\nA\n");
    CREDO_VERUM (crusta_lector_heredoc_petere(&cursus.lector,
        chorda_ex_literis("A", piscina), VERUM,
        FALSUM));
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&cursus.lector,
        &p));
    CREDO_VERUM (p.citatus);
    _heredoc_litterale(&cursus, CRUSTA_LEX_LITTERALIS, "l1\n$v\n");
    t = crusta_lector_proximum(&cursus.lector,
        CRUSTA_MODUS_HEREDOC_LITTERALE);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    _addere(&cursus,
        crusta_lector_heredoc_delimitator(&cursus.lector));
    _addere(&cursus,
        crusta_lector_heredoc_finis(&cursus.lector));
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

        /* <<- : tabulae exutae in comparatione, servatae in lexemate */
    _incipere(&cursus, piscina, "x\n\tb\n\t\tA\nz");
    CREDO_VERUM (crusta_lector_heredoc_petere(&cursus.lector,
        chorda_ex_literis("A", piscina), FALSUM,
        VERUM));
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&cursus.lector,
        &p));
    _heredoc(&cursus, CRUSTA_LEX_LITTERALIS, "\tb\n");
    t = crusta_lector_proximum(&cursus.lector, CRUSTA_MODUS_HEREDOC);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    t = crusta_lector_heredoc_delimitator(&cursus.lector);
    CREDO_AEQUALIS_I32 (t->valor.mensura, (i32)III);
    _addere(&cursus, t);
    _addere(&cursus,
        crusta_lector_heredoc_finis(&cursus.lector));
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "z");
    _finire(&cursus, CRUSTA_MODUS_VERBA);

        /* delimitator absens: corpus ad EOF, ambo NIHIL */
    _incipere(&cursus, piscina, "x\nbody\n");
    CREDO_VERUM (crusta_lector_heredoc_petere(&cursus.lector,
        chorda_ex_literis("A", piscina), FALSUM,
        FALSUM));
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&cursus.lector,
        &p));
    _heredoc(&cursus, CRUSTA_LEX_LITTERALIS, "body\n");
    t = crusta_lector_proximum(&cursus.lector, CRUSTA_MODUS_HEREDOC);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    CREDO_NIHIL (crusta_lector_heredoc_delimitator(&cursus.lector));
    CREDO_NIHIL (crusta_lector_heredoc_finis(&cursus.lector));
    _finire(&cursus, CRUSTA_MODUS_INITIUM);

        /* petitiones duae ordine */
    _incipere(&cursus, piscina, "x\n1\nA\n2\nB\n");
    CREDO_VERUM (crusta_lector_heredoc_petere(&cursus.lector,
        chorda_ex_literis("A", piscina), FALSUM,
        FALSUM));
    CREDO_VERUM (crusta_lector_heredoc_petere(&cursus.lector,
        chorda_ex_literis("B", piscina), VERUM,
        FALSUM));
    _initium(&cursus, CRUSTA_LEX_LITTERALIS, "x");
    _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE, "\n");
    CREDO_VERUM (crusta_lector_heredoc_aperire(&cursus.lector,
        &p));
    CREDO_FALSUM (p.citatus);
    _heredoc(&cursus, CRUSTA_LEX_LITTERALIS, "1\n");
    t = crusta_lector_proximum(&cursus.lector, CRUSTA_MODUS_HEREDOC);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    _addere(&cursus,
        crusta_lector_heredoc_delimitator(&cursus.lector));
    _addere(&cursus,
        crusta_lector_heredoc_finis(&cursus.lector));
    CREDO_VERUM (crusta_lector_heredoca_pendent(&cursus.lector));
    CREDO_VERUM (crusta_lector_heredoc_aperire(&cursus.lector,
        &p));
    CREDO_VERUM (p.citatus);
    _heredoc_litterale(&cursus, CRUSTA_LEX_LITTERALIS, "2\n");
    t = crusta_lector_proximum(&cursus.lector,
        CRUSTA_MODUS_HEREDOC_LITTERALE);
    CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_FINIS);
    _addere(&cursus,
        crusta_lector_heredoc_delimitator(&cursus.lector));
    _addere(&cursus,
        crusta_lector_heredoc_finis(&cursus.lector));
    CREDO_FALSUM (crusta_lector_heredoca_pendent(&cursus.lector));
    CREDO_FALSUM (crusta_lector_heredoc_aperire(&cursus.lector,
        &p));
    _finire(&cursus, CRUSTA_MODUS_INITIUM);
    }


    /* ==================================================
     * PROBARE: positiones et situs reponendus
     * ================================================== */

    CREDO_GRADUS ("positiones");

    {
    MateriaToken* t;
    CrustaSitus  s;

    _incipere(&cursus, piscina, "ab\ncd ef\n\ng");
    t = _initium(&cursus, CRUSTA_LEX_LITTERALIS, "ab");
    CREDO_AEQUALIS_I32 (t->linea, (i32)I);
    CREDO_AEQUALIS_I32 (t->columna, (i32)I);
    s = crusta_lector_situs(&cursus.lector);
    t = _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE,
        "\n");
    CREDO_AEQUALIS_I32 (t->linea, (i32)I);
    CREDO_AEQUALIS_I32 (t->columna, (i32)III);
    t = _initium(&cursus, CRUSTA_LEX_LITTERALIS, "cd");
    CREDO_AEQUALIS_I32 (t->linea, (i32)II);
    CREDO_AEQUALIS_I32 (t->columna, (i32)I);
    t = _verba(&cursus, CRUSTA_LEX_SPATIUM, " ");
    t = _verba(&cursus, CRUSTA_LEX_LITTERALIS, "ef");
    CREDO_AEQUALIS_I32 (t->linea, (i32)II);
    CREDO_AEQUALIS_I32 (t->columna, (i32)IV);
    t = _verba(&cursus, CRUSTA_LEX_SEPARATOR_LINEAE,
        "\n");
    CREDO_AEQUALIS_I32 (t->columna, (i32)VI);
    t = _initium(&cursus, CRUSTA_LEX_LINEA, "\n");
    CREDO_AEQUALIS_I32 (t->linea, (i32)III);
    CREDO_AEQUALIS_I32 (t->columna, (i32)I);
    t = _initium(&cursus, CRUSTA_LEX_LITTERALIS, "g");
        CREDO_AEQUALIS_I32 (t->linea, (i32)IV);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)X);
        _finire(&cursus, CRUSTA_MODUS_VERBA);

        /* reponere: eadem lexemata iterum */
        crusta_lector_situm_reponere(&cursus.lector, s);
        t = crusta_lector_proximum(&cursus.lector, CRUSTA_MODUS_VERBA);
        CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_SEPARATOR_LINEAE);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)II);
        t = crusta_lector_proximum(&cursus.lector,
            CRUSTA_MODUS_INITIUM);
        CREDO_AEQUALIS_S32 (t->genus, CRUSTA_LEX_LITTERALIS);
        CREDO_AEQUALIS_I32 (t->linea, (i32)II);
    }


    imprimere("\n    casus %d, octeti tecti %d\n",
        (integer)cursus.casus_numerus,
        (integer)cursus.octeti_tecti);
    CREDO_MAIOR_I32 (cursus.casus_numerus, (i32)XL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lector: casus per modos", _casus_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("crusta_lector", SECTIONES);
}
