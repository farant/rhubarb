/* probatio_paginatio.c - numeri Romani + designationes paginarum
 *
 * Probationes numerorum Romanorum ex probatio_capitula.c HUC MOTAE
 * cum parsor in bibliothecam propriam promotus est (consumptor
 * secundus: paginae). Casus ipsi intacti manent - migratio ea
 * servat, ne promotio regulam strictam tacite laxet.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "numerus_romanus.h"
#include "paginatio.h"
#include "credo.h"
#include <stdio.h>

interior Piscina* arena = NIHIL;

interior b32
_romanus_valet (
    constans character* s)
{
    i32 valor = ZEPHYRUM;
    redde numerus_romanus_legere(chorda_ex_literis(s, arena), &valor);
}

interior i32
_romanus_valor (
    constans character* s)
{
    i32 valor = ZEPHYRUM;
    (vacuum)numerus_romanus_legere(chorda_ex_literis(s, arena), &valor);
    redde valor;
}

interior Paginatio
_pag (
    constans character* s)
{
    redde paginatio_legere(chorda_ex_literis(s, arena));
}

interior i64
_clavis (
    constans character* s)
{
    redde paginatio_clavis_chordae(chorda_ex_literis(s, arena));
}

/* arena = piscina sectionis (adiutores supra eam legunt); purgare
 * reponit ne status inter sectiones maneat */
interior vacuum
_parare (
    CredoContextus* c)
{
    arena = c->piscina;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    arena = NIHIL;
}


    /* ========================================================
     * PROBARE: numeri Romani - forma subtractiva stricta
     * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        CREDO_VERUM (_romanus_valet("I"));
        CREDO_VERUM (_romanus_valet("IV"));
        CREDO_VERUM (_romanus_valet("XIV"));
        CREDO_VERUM (_romanus_valet("XXXIX"));
        CREDO_VERUM (_romanus_valet("MCMXCIV"));
        CREDO_VERUM (_romanus_valet("MMMCMXCIX"));

        CREDO_AEQUALIS_I32 (_romanus_valor("I"), I);
        CREDO_AEQUALIS_I32 (_romanus_valor("IV"), IV);
        CREDO_AEQUALIS_I32 (_romanus_valor("XIV"), XIV);
        CREDO_AEQUALIS_I32 (_romanus_valor("XXXIX"), XXXIX);
        CREDO_AEQUALIS_I32 (_romanus_valor("MCMXCIV"), 1994);

        /* formae illicitae */
        CREDO_FALSUM (_romanus_valet(""));
        CREDO_FALSUM (_romanus_valet("IIII"));   /* cursus nimius */
        CREDO_FALSUM (_romanus_valet("VV"));     /* V bis */
        CREDO_FALSUM (_romanus_valet("IL"));     /* par illicitum */
        CREDO_FALSUM (_romanus_valet("IXX"));    /* post IX nihil */
        CREDO_FALSUM (_romanus_valet("XIIX"));   /* grex crescens */
        CREDO_FALSUM (_romanus_valet("A"));

        /* VERBA ANGLICA ex litteris Romanis - causa ipsa regulae
         * strictae. Omnia respui debent. */
        CREDO_FALSUM (_romanus_valet("DID"));
        CREDO_FALSUM (_romanus_valet("MILD"));
        CREDO_FALSUM (_romanus_valet("CIVIC"));
        CREDO_FALSUM (_romanus_valet("VIM"));
        CREDO_FALSUM (_romanus_valet("DIM"));
        CREDO_FALSUM (_romanus_valet("LIVID"));

        /* MIX numerus VERUS est (1009) - superest consulto */
        CREDO_VERUM (_romanus_valet("MIX"));

        /* MINUSCULA nunc accipiuntur (paginae prooemii 'xii'), sed
         * casus MIXTUS non - verbum est, non numerus */
        CREDO_VERUM (_romanus_valet("xii"));
        CREDO_AEQUALIS_I32 (_romanus_valor("xii"), XII);
        CREDO_VERUM (_romanus_valet("xlii"));
        CREDO_AEQUALIS_I32 (_romanus_valor("xlii"), XLII);
        CREDO_FALSUM (_romanus_valet("Xii"));
        CREDO_FALSUM (_romanus_valet("xIi"));
        /* regula stricta minusculis quoque valet */
        CREDO_FALSUM (_romanus_valet("iiii"));
        CREDO_FALSUM (_romanus_valet("did"));
    }
}


    /* ========================================================
     * PROBARE: designatio paginae
     * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        CREDO_AEQUALIS_I32 ((i32)_pag("xii").genus,
            (i32)PAGINATIO_ROMANA);
        CREDO_AEQUALIS_I32 (_pag("xii").valor, XII);
        CREDO_AEQUALIS_I32 ((i32)_pag("XIV").genus,
            (i32)PAGINATIO_ROMANA);
        CREDO_AEQUALIS_I32 ((i32)_pag("42").genus,
            (i32)PAGINATIO_ARABICA);
        CREDO_AEQUALIS_I32 (_pag("42").valor, XLII);
        CREDO_AEQUALIS_I32 ((i32)_pag(" 42 ").genus,
            (i32)PAGINATIO_ARABICA);   /* spatia praeciduntur */

        /* RESPUENDA - laxitas paginam in ordine falso poneret */
        CREDO_AEQUALIS_I32 ((i32)_pag("").genus, (i32)PAGINATIO_NULLA);
        CREDO_AEQUALIS_I32 ((i32)_pag("0").genus,
            (i32)PAGINATIO_NULLA);     /* pagina nulla non est */
        CREDO_AEQUALIS_I32 ((i32)_pag("12a").genus,
            (i32)PAGINATIO_NULLA);
        CREDO_AEQUALIS_I32 ((i32)_pag("p. 12").genus,
            (i32)PAGINATIO_NULLA);
        CREDO_AEQUALIS_I32 ((i32)_pag("-5").genus,
            (i32)PAGINATIO_NULLA);
        CREDO_AEQUALIS_I32 ((i32)_pag("did").genus,
            (i32)PAGINATIO_NULLA);
        CREDO_AEQUALIS_I32 ((i32)_pag("   ").genus,
            (i32)PAGINATIO_NULLA);
    }
}


    /* ========================================================
     * PROBARE: ORDO - prooemium ANTE corpus
     *
     * Haec causa ipsa typi est: numerus integer solus 'xii' et '12'
     * distinguere non potest, et ordo falsus notam ad locum falsum
     * ligat.
     * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
        /* intra seriem Romanam: numerice */
        CREDO_VERUM (_clavis("i") < _clavis("ii"));
        CREDO_VERUM (_clavis("ix") < _clavis("x"));
        CREDO_VERUM (_clavis("xlii") > _clavis("xli"));

        /* intra seriem Arabicam: numerice, NON lexice
         * ('9' < '10' quamquam chorda "10" < "9") */
        CREDO_VERUM (_clavis("9") < _clavis("10"));
        CREDO_VERUM (_clavis("99") < _clavis("100"));

        /* TRANS series: omnis Romana ANTE omnem Arabicam */
        CREDO_VERUM (_clavis("xlii") < _clavis("1"));
        CREDO_VERUM (_clavis("m") < _clavis("1"));

        /* pagina nulla ULTIMA (nota sine pagina ordinem non
         * interrumpit) */
        CREDO_VERUM (_clavis("") > _clavis("380"));
        CREDO_VERUM (_clavis("nugae") > _clavis("380"));

        /* casus non mutat ordinem */
        CREDO_VERUM (_clavis("XII") == _clavis("xii"));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "numerus_romanus_legere", _sectio_prima, _parare, _purgare, NIHIL,
        NIHIL },
    { "paginatio_legere", _sectio_secunda, _parare, _purgare, NIHIL,
        NIHIL },
    { "paginatio_clavis", _sectio_tertia, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("paginatio", SECTIONES);
}
