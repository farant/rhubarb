/* probatio_html_arbor.c - Aedificator HTML: octeti et structura
 *
 * DUO ORDINES PROBATIONIS, consulto distincti:
 *
 *  I. OCTETI    - emissio DIRECTA ex parsura (materia_scribere).
 *                 ORACULUM SEPARANS: si porta STML (H6) postea
 *                 cadat, haec dicet utrum aedificator an proiectio
 *                 culpanda sit.
 * II. STRUCTURA - tabula locorum absentium spec par. IV.2 ordine
 *                 per ordinem asserta; parentela; clausura litteris
 *                 neglectis; malum sine pari.
  * H4: elementa vacua (tabula XIII) et clausurae implicitae (tabula,
 * vertex acervi solus) - tabulae par. IV.2 ordines ceteri.
 */

#include "latina.h"
#include "credo.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include "html_lexicon.h"
#include "html_lexema.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ORACULUM SEPARANS: emissio DIRECTA ex parsura, sine STML. */
hic_manens b32
_octetos_probare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    MateriaNodus* radix;
    MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;

    radix = html_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    { redde FALSUM;
    }

    materia_scriptura_consilium_nudum(&consilium, &HTML_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus)
    { redde FALSUM;
    }
    si (emissa.textus.mensura != mensura)
    { redde FALSUM;
    }
    si (mensura == ZEPHYRUM)
    { redde VERUM;
    }
    redde (b32)(memcmp(emissa.textus.datum, fons,
                       (size_t)mensura) == ZEPHYRUM);
}

/* Fontem ex piscina allocare: parsator octetos NON copiat, ergo
 * litterae staticae sufficiunt, sed haec via lectionem css
 * (buffer acervi = lexemata in tabulatum mortuum) documentat. */
hic_manens MateriaNodus*
_parsare (
               Piscina* piscina,
    constans character* litterae)
{
          i32  mensura = (i32)strlen(litterae);
    character* fons = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);

    memcpy(fons, litterae, (size_t)mensura + I);
    redde html_arbor_parsare(piscina, fons, mensura);
}

hic_manens i32
_numerus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    { redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(nodus->loci[locus]);
}

hic_manens MateriaNodus*
_liber (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v;

    si (nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    { redde NIHIL;
    }
    v = materia_valor_lista_obtinere(nodus->loci[locus], i);
    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS)
        ? v->datum.nodus : NIHIL;
}

hic_manens b32
_absens (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (b32)(nodus->loci[locus].genus == MATERIA_VALOR_NIHIL);
}

hic_manens MateriaToken*
_tok (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (nodus->loci[locus].genus == MATERIA_VALOR_TOKEN)
        ? nodus->loci[locus].datum.token : NIHIL;
}

integer
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_html_arbor", 65536);
    credo_aperire(piscina);


    /* ==================================================
     * OCTETI: emissio directa == fons
     * ================================================== */

    {
        imprimere("\n--- Probans circuitum octetorum ---\n");
        CREDO_VERUM (_octetos_probare(piscina, "<p>a</p>", VIII));
        CREDO_VERUM (_octetos_probare(piscina,
            "<div id=\"x\" class='y' z>t</div>", XXXI));
        CREDO_VERUM (_octetos_probare(piscina,
            "<a  href = x\n  >b</A >", XXII));
        CREDO_VERUM (_octetos_probare(piscina,
            "x &amp; y <!-- c --> <![CDATA[z]]>", XXXIV));
        CREDO_VERUM (_octetos_probare(piscina,
            "<!DOCTYPE html><html></html>", XXVIII));
        CREDO_VERUM (_octetos_probare(piscina, "<div cl", VII));
        CREDO_VERUM (_octetos_probare(piscina, "</x>a", V));
        CREDO_VERUM (_octetos_probare(piscina, "<div>", V));
        CREDO_VERUM (_octetos_probare(piscina, "<img/>", VI));
        CREDO_VERUM (_octetos_probare(piscina, "<p>a</P>", VIII));
        CREDO_VERUM (_octetos_probare(piscina, "<a><b>x</a>", XI));
        CREDO_VERUM (_octetos_probare(piscina, "<a = x>", VII));
        CREDO_VERUM (_octetos_probare(piscina, "</div", V));
        CREDO_VERUM (_octetos_probare(piscina, "<a /\n href=x>", XIII));
        CREDO_VERUM (_octetos_probare(piscina,
            "<script>a</b></script>", XXII));
        CREDO_VERUM (_octetos_probare(piscina, "", ZEPHYRUM));
        /* H4 */
        CREDO_VERUM (_octetos_probare(piscina, "<p>a<p>b", VIII));
        CREDO_VERUM (_octetos_probare(piscina, "<ul><li>a<li>b</ul>",
            XIX));
        CREDO_VERUM (_octetos_probare(piscina, "<br><br/><BR>x", XIV));
        CREDO_VERUM (_octetos_probare(piscina, "<p>a<div>b</div>",
            XVI));
        CREDO_VERUM (_octetos_probare(piscina,
            "<table><tr><td>1<td>2<tr><td>3</table>", XXXVIII));
        CREDO_VERUM (_octetos_probare(piscina,
            "<textarea><p></textarea>", XXIV));
        CREDO_VERUM (_octetos_probare(piscina, "</br>", V));
        CREDO_VERUM (_octetos_probare(piscina, "<div><p>x</div>", XV));
        CREDO_VERUM (_octetos_probare(piscina, "<p>a<b>x<div>y", XIV));
        CREDO_VERUM (_octetos_probare(piscina,
            "<dl><dt>a<dd>b<dt>c</dl>", XXIV));
        CREDO_VERUM (_octetos_probare(piscina,
            "<input disabled><img src=x>y", XXVIII));
        CREDO_VERUM (_octetos_probare(piscina, "<br></br>", IX));
        /* H8: tag clausurae cum attributis (totalitas invenit) */
        CREDO_VERUM (_octetos_probare(piscina, "<h3>a</h3\t x=y>z",
            XVI));
        CREDO_VERUM (_octetos_probare(piscina, "<p>a</p b", IX));
    }


    /* ==================================================
     * STRUCTURA: tabula locorum absentium (spec par. IV.2)
     * ================================================== */

    {
        MateriaNodus* documentum;
        MateriaNodus* elementum;

        imprimere("\n--- Probans '<div>' apertum ad EOF ---\n");
        documentum = _parsare(piscina, "<div>");
        CREDO_NON_NIHIL (documentum);
        CREDO_AEQUALIS_S32 (documentum->genus,
            (s32)HTML_GENUS_DOCUMENTUM);
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (elementum);
        CREDO_AEQUALIS_S32 (elementum->genus,
            (s32)HTML_GENUS_ELEMENTUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_APERTURA));
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_FINIS));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_VERUM (_absens(elementum,
            HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_ATTRIBUTA));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_LIBERI));
        /* pater a verbo ponente fixus */
        CREDO_AEQUALIS_PTR (elementum->pater, documentum);
        /* cauda: FINIS */
        CREDO_NON_NIHIL (_tok(documentum, HTML_DOCUMENTUM_CAUDA));
        CREDO_AEQUALIS_S32 (
            _tok(documentum, HTML_DOCUMENTUM_CAUDA)->genus,
            (s32)HTML_LEX_FINIS);

        imprimere("\n--- Probans '<img/>': finis '/>' ---\n");
        documentum = _parsare(piscina, "<img/>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (elementum);
        CREDO_AEQUALIS_S32 (
            _tok(elementum, HTML_ELEMENTUM_TOK_FINIS)->genus,
            (s32)HTML_LEX_TAG_FINIS_SOLUS);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_VERUM (_absens(elementum,
            HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));

        imprimere("\n--- Probans '<div cl': tag ad EOF scissum ---\n");
        documentum = _parsare(piscina, "<div cl");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (elementum);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_FINIS));
        CREDO_AEQUALIS_I32 (_numerus(elementum,
            HTML_ELEMENTUM_ATTRIBUTA),
            I);
        {
            MateriaNodus* attributum = _liber(elementum,
                HTML_ELEMENTUM_ATTRIBUTA, ZEPHYRUM);
            CREDO_NON_NIHIL (attributum);
            CREDO_AEQUALIS_S32 (attributum->genus,
                (s32)HTML_GENUS_ATTRIBUTUM);
            CREDO_NON_NIHIL (_tok(attributum,
                HTML_ATTRIBUTUM_TOK_NOMEN));
            CREDO_VERUM (_absens(attributum,
                HTML_ATTRIBUTUM_TOK_AEQUALE));
            CREDO_VERUM (_absens(attributum,
                HTML_ATTRIBUTUM_TOK_VALOR));
        }

        imprimere("\n--- Probans attributa tria, tertium nudum ---\n");
        documentum = _parsare(piscina,
            "<div id=\"x\" class='y' z>t</div>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (elementum);
        CREDO_AEQUALIS_I32 (_numerus(elementum,
            HTML_ELEMENTUM_ATTRIBUTA),
            III);
        {
            MateriaNodus* primum = _liber(elementum,
                HTML_ELEMENTUM_ATTRIBUTA, ZEPHYRUM);
            MateriaNodus* tertium = _liber(elementum,
                HTML_ELEMENTUM_ATTRIBUTA, II);
            CREDO_NON_NIHIL (_tok(primum, HTML_ATTRIBUTUM_TOK_NOMEN));
            CREDO_NON_NIHIL (_tok(primum, HTML_ATTRIBUTUM_TOK_AEQUALE));
            CREDO_NON_NIHIL (_tok(primum, HTML_ATTRIBUTUM_TOK_VALOR));
            CREDO_NON_NIHIL (_tok(tertium, HTML_ATTRIBUTUM_TOK_NOMEN));
            CREDO_VERUM (_absens(tertium, HTML_ATTRIBUTUM_TOK_AEQUALE));
            CREDO_VERUM (_absens(tertium, HTML_ATTRIBUTUM_TOK_VALOR));
        }
        /* liberi: textus 't'; clausura plena */
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        CREDO_AEQUALIS_S32 (
            _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_TEXTUS);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_NON_NIHIL (_tok(elementum,
            HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));

        imprimere("\n--- Probans '<a = x>': attributum anonymum ---\n");
        documentum = _parsare(piscina, "<a = x>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum,
            HTML_ELEMENTUM_ATTRIBUTA),
            I);
        {
            MateriaNodus* attributum = _liber(elementum,
                HTML_ELEMENTUM_ATTRIBUTA, ZEPHYRUM);
            CREDO_VERUM (_absens(attributum,
                HTML_ATTRIBUTUM_TOK_NOMEN));
            CREDO_NON_NIHIL (_tok(attributum,
                HTML_ATTRIBUTUM_TOK_AEQUALE));
            CREDO_NON_NIHIL (_tok(attributum,
                HTML_ATTRIBUTUM_TOK_VALOR));
        }
    }


    /* ==================================================
     * STRUCTURA: clausura, casus litterarum, parentela, malum
     * ================================================== */

    {
        MateriaNodus* documentum;
        MateriaNodus* elementum;

        imprimere("\n--- Probans '<p>a</P>': litteris neglectis ---\n");
        documentum = _parsare(piscina, "<p>a</P>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_NON_NIHIL (_tok(elementum,
            HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);

        imprimere("\n--- Probans '<a><b>x</a>': b implicite ---\n");
        documentum = _parsare(piscina, "<a><b>x</a>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        {
            MateriaNodus* b = _liber(elementum, HTML_ELEMENTUM_LIBERI,
                ZEPHYRUM);
            CREDO_AEQUALIS_S32 (b->genus, (s32)HTML_GENUS_ELEMENTUM);
            CREDO_VERUM (_absens(b, HTML_ELEMENTUM_TOK_CLAUSURA));
            CREDO_VERUM (_absens(b, HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));
            CREDO_AEQUALIS_I32 (_numerus(b, HTML_ELEMENTUM_LIBERI), I);
            CREDO_AEQUALIS_PTR (b->pater, elementum);
        }

        imprimere("\n--- Probans '</x>a': malum sine pari ---\n");
        documentum = _parsare(piscina, "</x>a");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elementum->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_MALUM_TOKENS), II);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, I)->genus,
            (s32)HTML_GENUS_TEXTUS);

        imprimere("\n--- Probans '</div' ad EOF: malum unius ---\n");
        documentum = _parsare(piscina, "</div");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elementum->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_MALUM_TOKENS), I);

        imprimere("\n--- Probans contentum: genera per lexema ---\n");
        documentum = _parsare(piscina,
            "<!DOCTYPE html>x &amp; y <!-- c --> <![CDATA[z]]>");
        /* doctype, textus, referentia, textus, commentarium, textus,
         * cdata = VII */
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            VII);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_DOCTYPE);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, II)->genus,
            (s32)HTML_GENUS_REFERENTIA);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, IV)->genus,
            (s32)HTML_GENUS_COMMENTARIUM);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, VI)->genus,
            (s32)HTML_GENUS_CDATA);

        imprimere("\n--- Probans textum crudum: lexator dominus ---\n");
        documentum = _parsare(piscina, "<script>a</b></script>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        CREDO_AEQUALIS_S32 (
            _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_TEXTUS_CRUDUS);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
    }


    /* ==================================================
     * STRUCTURA H4: elementa vacua, clausurae implicitae
     * ================================================== */

    {
        MateriaNodus* documentum;
        MateriaNodus* elementum;
        MateriaNodus* liber;

        imprimere("\n--- Probans '<p>a<p>b': p implicite ---\n");
        documentum = _parsare(piscina, "<p>a<p>b");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elementum->genus,
            (s32)HTML_GENUS_ELEMENTUM);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_VERUM (_absens(elementum,
            HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        liber = _liber(documentum, HTML_DOCUMENTUM_LIBERI, I);
        CREDO_AEQUALIS_S32 (liber->genus, (s32)HTML_GENUS_ELEMENTUM);
        CREDO_AEQUALIS_PTR (liber->pater, documentum);

        imprimere("\n--- Probans '<ul><li>a<li>b</ul>' ---\n");
        documentum = _parsare(piscina, "<ul><li>a<li>b</ul>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), I);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, I);
        CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_TOK_CLAUSURA));

        imprimere("\n--- Probans '<br><br/><BR>x': vacua ---\n");
        documentum = _parsare(piscina, "<br><br/><BR>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            IV);
        {
            i32 i;
            per (i = ZEPHYRUM; i < III; i++)
            {
                liber = _liber(documentum, HTML_DOCUMENTUM_LIBERI, i);
                CREDO_AEQUALIS_S32 (liber->genus,
                    (s32)HTML_GENUS_ELEMENTUM);
                CREDO_NON_NIHIL (_tok(liber, HTML_ELEMENTUM_TOK_FINIS));
                CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_LIBERI));
                CREDO_VERUM (_absens(liber,
                    HTML_ELEMENTUM_TOK_CLAUSURA));
                CREDO_VERUM (_absens(liber,
                    HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));
            }
        }
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, III)->genus,
            (s32)HTML_GENUS_TEXTUS);

        imprimere("\n--- Probans '<input disabled><img src=x>y' ---\n");
        documentum = _parsare(piscina, "<input disabled><img src=x>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            III);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum,
            HTML_ELEMENTUM_ATTRIBUTA),
            I);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_LIBERI));
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI, I);
        CREDO_AEQUALIS_I32 (_numerus(elementum,
            HTML_ELEMENTUM_ATTRIBUTA),
            I);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_LIBERI));

        imprimere("\n--- Probans '<p>a<div>b</div>': fratres ---\n");
        documentum = _parsare(piscina, "<p>a<div>b</div>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI, I);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));

        imprimere("\n--- Probans '<p>a<b>x<div>y': vertex solus ---\n");
        /* b in vertice: p NON clauditur (aedificator simplex) - div
         * intra b intra p. Nominatum, non celatum. */
        documentum = _parsare(piscina, "<p>a<b>x<div>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, I);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), II);
        CREDO_AEQUALIS_S32 (
            _liber(liber, HTML_ELEMENTUM_LIBERI, I)->genus,
            (s32)HTML_GENUS_ELEMENTUM);

        imprimere("\n--- Probans tabulam: tr/td implicite ---\n");
        documentum = _parsare(piscina,
            "<table><tr><td>1<td>2<tr><td>3</table>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), II);
        CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_VERUM (_absens(_liber(liber, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM),
            HTML_ELEMENTUM_TOK_CLAUSURA));
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, I);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), I);
        CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_TOK_CLAUSURA));

        imprimere("\n--- Probans '<dl><dt>a<dd>b<dt>c</dl>' ---\n");
        documentum = _parsare(piscina, "<dl><dt>a<dd>b<dt>c</dl>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            III);

        imprimere("\n--- Probans '<textarea><p></textarea>' ---\n");
        documentum = _parsare(piscina, "<textarea><p></textarea>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        CREDO_AEQUALIS_S32 (
            _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_TEXTUS_CRUDUS);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));

        imprimere("\n--- Probans '</br>' et '<br></br>': malum ---\n");
        documentum = _parsare(piscina, "</br>");
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
        documentum = _parsare(piscina, "<br></br>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, I)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);

        imprimere("\n--- Probans '</h3 x=y>' clausura attributa ---\n");
        /* Porta totalitatis H8 invenit: '>' post '</h3' mutatum,
         * lexator
         * in modo tagi manet; lexemata attributorum malum UNUM pendens
         * fiunt, h3 clausum sine fine, '>' serius malo (non h3) datur -
         * ordo octetorum servatur. */
        documentum = _parsare(piscina, "<h3>a</h3\t x=y>z");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            III);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_VERUM (_absens(elementum,
            HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));
        liber = _liber(documentum, HTML_DOCUMENTUM_LIBERI, I);
        CREDO_AEQUALIS_S32 (liber->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_MALUM_TOKENS), IV);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, II)->genus,
            (s32)HTML_GENUS_TEXTUS);

        imprimere("\n--- Probans '<div><p>x</div>' ---\n");
        documentum = _parsare(piscina, "<div><p>x</div>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), I);

        /* O2b (oraculum html5lib): tabulae impletae */
        imprimere("\n--- Probans '<head><body>': head ab omni "
            "nisi ---\n");
        CREDO_VERUM (_octetos_probare(piscina, "<head><body>", XII));
        documentum = _parsare(piscina, "<head><body>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_LIBERI));
        /* exceptio: title in head manet */
        documentum = _parsare(piscina, "<head><title>t</title>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);

        imprimere("\n--- Probans '<ruby>a<rb>b<rt>c</ruby>' ---\n");
        CREDO_VERUM (_octetos_probare(piscina,
            "<ruby>a<rb>b<rt>c</ruby>", XXIV));
        documentum = _parsare(piscina, "<ruby>a<rb>b<rt>c</ruby>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            III);

        imprimere("\n--- Probans '<p>x<dir>y' et '<h1>a<h2>b' ---\n");
        documentum = _parsare(piscina, "<p>x<dir>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        documentum = _parsare(piscina, "<h1>a<h2>b");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);

        imprimere("\n--- Probans '<a>1<a>2': a se ipso clausum ---\n");
        documentum = _parsare(piscina, "<a>1<a>2");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);

        imprimere("\n--- Probans '<colgroup><col><tbody>' ---\n");
        documentum = _parsare(piscina, "<colgroup><col><tbody>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
