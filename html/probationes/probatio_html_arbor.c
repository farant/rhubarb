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
 * vertex acervi) - tabulae par. IV.2 ordines ceteri. O2b-5: scopus
 * (p per b, li per div non per pre, button per b, object limes).
 *
 * O7a (2026-09-15): probationes structurae 'in body' per FRAGMENTUM
 * contextu 'body' currunt (_parsare) - mechanica elementorum sine
 * involucris fictis; involucra html/head/body, partes tabulae fictae
 * et fragmenta per _parsare_documentum / _parsare_fragmentum in
 * ordine suo (STRUCTURA O7a) asseruntur: locus synthesis, tok_apertura
 * absens, clausura vera in ficto.
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
hic_manens character*
_fons (
               Piscina* piscina,
    constans character* litterae,
                   i32* mensura)
{
    character* fons;

    *mensura  = (i32)strlen(litterae);
    fons      = (character*)piscina_allocare(piscina,
        (memoriae_index)*mensura + I);
    memcpy(fons, litterae, (size_t)*mensura + I);
    redde fons;
}

hic_manens MateriaNodus*
_parsare_documentum (
               Piscina* piscina,
    constans character* litterae)
{
           i32  mensura;
     character* fons = _fons(piscina, litterae, &mensura);

    redde html_arbor_parsare(piscina, fons, mensura);
}

hic_manens MateriaNodus*
_parsare_fragmentum (
               Piscina* piscina,
    constans character* contextus,
    constans character* litterae)
{
           i32  mensura;
     character* fons = _fons(piscina, litterae, &mensura);

    redde html_arbor_parsare_fragmentum(piscina, fons, mensura,
        chorda_ex_literis(contextus, piscina), HTML_ALIENUM_NULLUM);
}

/* Mechanica 'in body' sine involucris fictis (O7a): fragmentum
 * contextu 'body'. */
hic_manens MateriaNodus*
_parsare (
               Piscina* piscina,
    constans character* litterae)
{
    redde _parsare_fragmentum(piscina, "body", litterae);
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

/* Mala TOTIUS arboris numerare (vexillum 'frameset-ok': compages
 * neglecta malum unum dat, accepta nullum - ergo numerus verdictum
 * simplex et totum fert) */
hic_manens i32
_mala_numerare (
    constans MateriaNodus* nodus)
{
    i32 summa = ZEPHYRUM;
    i32 locus;

    si (nodus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (nodus->genus == (s32)HTML_GENUS_ELEMENTUM_MALUM)
    {
        summa = I;
    }
    per (locus = ZEPHYRUM; locus < nodus->numerus_locorum; locus++)
    {
        si (nodus->loci[locus].genus == MATERIA_VALOR_LISTA)
        {
            i32 n = materia_valor_lista_numerus(nodus->loci[locus]);
            i32 k;

            per (k = ZEPHYRUM; k < n; k++)
            {
                MateriaValor* v = materia_valor_lista_obtinere(
                    nodus->loci[locus], k);

                si (   v        != NIHIL
                    && v->genus == MATERIA_VALOR_NODUS)
                {
                    summa = summa + _mala_numerare(v->datum.nodus);
                }
            }
        }
        alioquin si (nodus->loci[locus].genus == MATERIA_VALOR_NODUS)
        {
            summa = summa
                + _mala_numerare(nodus->loci[locus].datum.nodus);
        }
    }
    redde summa;
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

/* Valor loci synthesis elementi (NULLA si absens, nodus non elementum
 * aut NIHIL) */
hic_manens s32
_synthesis (
    constans MateriaNodus* nodus)
{
    si (   nodus        == NIHIL
        || nodus->genus != (s32)HTML_GENUS_ELEMENTUM
        || nodus->loci[HTML_ELEMENTUM_SYNTHESIS].genus
               != MATERIA_VALOR_INDEX)
    {
        redde (s32)HTML_SYNTHESIS_NULLA;
    }
    redde nodus->loci[HTML_ELEMENTUM_SYNTHESIS].datum.index;
}

/* Sedes nodi (O7b): parens DOM alibi, aut NIHIL (elementum, textus,
 * referentia, commentarium; NIHIL pro nodo NIHIL) */
hic_manens MateriaNodus*
_sedes (
    constans MateriaNodus* nodus)
{
    i32 locus;

    si (nodus == NIHIL)
    { redde NIHIL;
    }
    si (nodus->genus == (s32)HTML_GENUS_ELEMENTUM)
    { locus = (i32)HTML_ELEMENTUM_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_TEXTUS)
    { locus = (i32)HTML_TEXTUS_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_REFERENTIA)
    { locus = (i32)HTML_REFERENTIA_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_COMMENTARIUM)
    { locus = (i32)HTML_COMMENTARIUM_SEDES;
    }
    alioquin
    { redde NIHIL;
    }
    si (nodus->loci[locus].genus != MATERIA_VALOR_REFERENTIA)
    { redde NIHIL;
    }
    redde nodus->loci[locus].datum.nodus;
}

/* Liber i elementi html (liber primus documenti) - involucra ficta
 * documenti: ZEPHYRUM head, I body (aut frameset) */
hic_manens MateriaNodus*
_involutum (
    constans MateriaNodus* documentum,
                      i32  i)
{
    MateriaNodus* html = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
        ZEPHYRUM);

    si (html == NIHIL)
    { redde NIHIL;
    }
    redde _liber(html, HTML_ELEMENTUM_LIBERI, i);
}

/* Originale exemplaris (O7c): scopus loci exemplar, aut NIHIL */
hic_manens MateriaNodus*
_exemplar (
    constans MateriaNodus* nodus)
{
    si (   nodus == NIHIL || nodus->genus != (s32)HTML_GENUS_ELEMENTUM
        || nodus->loci[HTML_ELEMENTUM_EXEMPLAR].genus
               != MATERIA_VALOR_REFERENTIA)
    { redde NIHIL;
    }
    redde nodus->loci[HTML_ELEMENTUM_EXEMPLAR].datum.nodus;
}

/* Praecedens (O7c): frater DOM prior nodi moti, aut NIHIL */
hic_manens MateriaNodus*
_praecedens (
    constans MateriaNodus* nodus)
{
    si (   nodus == NIHIL || nodus->genus != (s32)HTML_GENUS_ELEMENTUM
        || nodus->loci[HTML_ELEMENTUM_PRAECEDENS].genus
               != MATERIA_VALOR_REFERENTIA)
    { redde NIHIL;
    }
    redde nodus->loci[HTML_ELEMENTUM_PRAECEDENS].datum.nodus;
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

        imprimere("\n--- Probans '<p>a<b>x<div>y': scopus ---\n");
        /* O2b-5 (2026-09-15): p in scopo 'button' a div clauditur
         * PER b - lex verticis solius (H1) relicta; b et p sine
         * clausura (loci absentes), div frater. Olim I liber. */
        documentum = _parsare(piscina, "<p>a<b>x<div>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, I);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), I);
        CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, I)->genus,
            (s32)HTML_GENUS_ELEMENTUM);
        /* limes scopi: object p non tradit - p intra p manet */
        documentum = _parsare(piscina, "<p><object><p>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        /* li per b (non 'special'), non per div ('special') */
        documentum = _parsare(piscina, "<ul><li>a<b>b<li>c</ul>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        /* div EXCEPTUM a 'special' (spec: address/div/p) - li per div
         * quoque clauditur; pre 'special' sistit */
        documentum = _parsare(piscina, "<ul><li>a<div><li>c</ul>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        documentum = _parsare(piscina, "<ul><li>a<pre><li>c</ul>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        /* button in scopo per b */
        documentum = _parsare(piscina, "<button><b>x<button>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);

        /* O2b-6: contentum alienum - tag se-claudens claudit, tag HTML
         * rumpens erumpit, puncta integrationis HTML manent */
        imprimere("\n--- Probans '<svg><path/><circle/></svg>' ---\n");
        CREDO_VERUM (_octetos_probare(piscina,
            "<svg><path/><circle/></svg>", XXVII));
        documentum = _parsare(piscina, "<svg><path/><circle/></svg>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        /* in HTML '/>' numquam claudit (caput H4) */
        documentum = _parsare(piscina, "<div/><p/>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        imprimere("\n--- Probans '<svg><p>x': eruptio ---\n");
        /* documentum: in fragmento eruptio numquam (spec 'fragment
         * case', O7a) - p intra g manet */
        documentum  = _parsare_documentum(piscina, "<svg><g><p>x");
        elementum   = _involutum(documentum, I);   /* body fictum */
        CREDO_NON_NIHIL (elementum);
        si (elementum != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus(elementum,
                HTML_ELEMENTUM_LIBERI), II);
            elementum = _liber(elementum, HTML_ELEMENTUM_LIBERI,
                ZEPHYRUM);
            CREDO_VERUM (_absens(elementum,
                HTML_ELEMENTUM_TOK_CLAUSURA));
        }
        documentum = _parsare(piscina, "<svg><g><p>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        /* punctum integrationis: p intra desc manet, b intra mi */
        documentum = _parsare(piscina, "<svg><desc><p>x</desc></svg>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        documentum = _parsare(piscina, "<math><mi><b>x</mi></math>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        /* intra foreignObject regula HTML: '<div/>' non claudit */
        documentum = _parsare(piscina, "<svg><foreignObject><div/>x");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (elementum);
        elementum = elementum ? _liber(elementum, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM) : NIHIL;
        CREDO_NON_NIHIL (elementum);
        elementum = elementum ? _liber(elementum, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM) : NIHIL;
        CREDO_NON_NIHIL (elementum);
        si (elementum != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus(elementum,
                HTML_ELEMENTUM_LIBERI), I);
        }

        /* O5: lexemata a DOM neglecta -> elementum-malum in loco */
        imprimere("\n--- Probans doctype post contentum: malum ---\n");
        CREDO_VERUM (_octetos_probare(piscina, "<p>x<!DOCTYPE html>y",
            XX));
        documentum = _parsare(piscina, "<p>x<!DOCTYPE html>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            III);
        CREDO_AEQUALIS_S32 (
            _liber(elementum, HTML_ELEMENTUM_LIBERI, I)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
        imprimere("\n--- Probans '<html><html x>' iteratum ---\n");
        /* O7a: html verum, malum, head fictum (vacuum), body fictum
         * cum y */
        CREDO_VERUM (_octetos_probare(piscina, "<html><html x=1>y",
            XVII));
        documentum = _parsare_documentum(piscina, "<html><html x=1>y");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_APERTURA));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            III);
        CREDO_AEQUALIS_S32 (
            _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
        CREDO_AEQUALIS_S32 (_synthesis(_involutum(documentum, I)),
            (s32)HTML_SYNTHESIS_CAPUT);
        CREDO_AEQUALIS_S32 (_synthesis(_involutum(documentum, II)),
            (s32)HTML_SYNTHESIS_CORPUS);
        imprimere("\n--- Probans '<body></body>x': clausum, x per "
            "sedem ---\n");
        /* O7b: '</body>' CLAUDIT (O5 malum tenebat); x liber html per
         * octetos, sedes = body (spec 'after body': in body) */
        CREDO_VERUM (_octetos_probare(piscina, "<body></body>x", XIV));
        documentum = _parsare_documentum(piscina, "<body></body>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _involutum(documentum, I);   /* body verum */
        CREDO_NON_NIHIL (elementum);
        si (elementum != NIHIL)
        {
            CREDO_NON_NIHIL (_tok(elementum,
                HTML_ELEMENTUM_TOK_APERTURA));
            CREDO_NON_NIHIL (_tok(elementum,
                HTML_ELEMENTUM_TOK_CLAUSURA));
            CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_LIBERI));
            liber = _involutum(documentum, II);   /* x */
            CREDO_NON_NIHIL (liber);
            CREDO_AEQUALIS_PTR (_sedes(liber), elementum);
        }
        imprimere("\n--- Probans frameset: neglecta ---\n");
        CREDO_VERUM (_octetos_probare(piscina,
            "<frameset><p>x<frame></frameset>", XXXII));
        documentum = _parsare_documentum(piscina,
            "<frameset><p>x<frame></frameset>");
        elementum = _involutum(documentum, I);   /* frameset verum */
        CREDO_NON_NIHIL (elementum);
        si (elementum != NIHIL)
        {
            CREDO_NON_NIHIL (_tok(elementum,
                HTML_ELEMENTUM_TOK_CLAUSURA));
            CREDO_AEQUALIS_I32 (_numerus(elementum,
                HTML_ELEMENTUM_LIBERI), III);
            CREDO_AEQUALIS_S32 (
                _liber(elementum, HTML_ELEMENTUM_LIBERI, I)->genus,
                (s32)HTML_GENUS_ELEMENTUM_MALUM);
        }
        /* frameset post contentum neglectum (body fictum) */
        documentum  = _parsare_documentum(piscina, "<p>x<frameset>y");
        elementum   = _involutum(documentum, I);
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_CORPUS);
        elementum = elementum ? _liber(elementum, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM) : NIHIL;
        CREDO_NON_NIHIL (elementum);
        si (elementum != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus(elementum,
                HTML_ELEMENTUM_LIBERI), III);
        }
        imprimere("\n--- Probans select: div neglectum ---\n");
        documentum = _parsare(piscina,
            "<select><div>a<option>b</select>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            III);
        documentum = _parsare(piscina, "<select><input>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);

        /* O6: 'any other end tag' ad elementum 'special' sistit -
         * '</div>' intra template neglectum (malum), Hello intra
         * template manet; '</span>' per b claudit (b non special) */
        imprimere("\n--- Probans '<div><template></div>Hello' ---\n");
        CREDO_VERUM (_octetos_probare(piscina,
            "<div><template></div>Hello", XXVI));
        documentum = _parsare(piscina, "<div><template></div>Hello");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        elementum = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_NON_NIHIL (elementum);
        si (elementum != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus(elementum,
                HTML_ELEMENTUM_LIBERI), II);
        }
        documentum = _parsare(piscina, "<span><b>x</span>y");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        /* '</span>' per pre (special) NON claudit (O7a: td extra
         * tabulam iam neglectum est - pre eius vicem gerit) */
        documentum = _parsare(piscina, "<span><pre>x</span>y");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));

        imprimere("\n--- Probans tabulam: tr/td implicite ---\n");
        /* O7a: tbody FICTUM inter table et tr (spec 'in table') */
        documentum = _parsare(piscina,
            "<table><tr><td>1<td>2<tr><td>3</table>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        elementum = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_CORPUS_TABULAE);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_APERTURA));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
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

        imprimere("\n--- Probans '</br>' et '<br></br>': br fictum "
            "(O7c) ---\n");
        /* spec: '</br>' ut '<br>' - elementum br FICTUM (synthesis),
         * clausura vera in ficto; deviatio 'malum' H4 retirata */
        documentum = _parsare(piscina, "</br>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (elementum->genus,
            (s32)HTML_GENUS_ELEMENTUM);
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_FRACTURA);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_NON_NIHIL (_tok(elementum,
            HTML_ELEMENTUM_TOK_CLAUSURA_FINIS));
        documentum = _parsare(piscina, "<br></br>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (
            _synthesis(_liber(documentum, HTML_DOCUMENTUM_LIBERI, I)),
            (s32)HTML_SYNTHESIS_FRACTURA);
        /* '</p>' sine p in scopo: p fictum clausum (spec) */
        documentum = _parsare(piscina, "x</p>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            III);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI, I);
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_PARAGRAPHUS);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_LIBERI));

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
        documentum = _parsare_documentum(piscina, "<head><body>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);   /* html fictum: head verum, body verum */
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_RADIX);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        elementum = _involutum(documentum, ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_APERTURA));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_LIBERI));
        /* exceptio: title in head manet; x corpus fictum postulat */
        documentum = _parsare_documentum(piscina,
            "<head><title>t</title>x");
        elementum = _involutum(documentum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        elementum = _involutum(documentum, I);
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_CORPUS);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
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
        /* contextu 'table' (O7a: extra tabulam partes neglectae) */
        documentum = _parsare_fragmentum(piscina, "table",
            "<colgroup><col><tbody>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
    }


    /* ==================================================
     * STRUCTURA O7a: synthesis - involucra, partes tabulae, fragmenta
     * ================================================== */

    {
        MateriaNodus* documentum;
        MateriaNodus* elementum;
        MateriaNodus* liber;

        imprimere("\n--- Probans involucra ficta: '<p>a' ---\n");
        CREDO_VERUM (_octetos_probare(piscina, "<p>a", IV));
        documentum = _parsare_documentum(piscina, "<p>a");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_RADIX);
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_APERTURA));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_ATTRIBUTA));
        CREDO_VERUM (_absens(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        liber = _involutum(documentum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_CAPUT);
        CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_LIBERI));
        liber = _involutum(documentum, I);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_CORPUS);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), I);
        CREDO_AEQUALIS_PTR (liber->pater, elementum);
        /* elementum verum: locus synthesis ABSENS */
        liber = _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_NULLA);
        CREDO_NON_NIHIL (_tok(liber, HTML_ELEMENTUM_TOK_APERTURA));
        /* tabula titulorum */
        CREDO_VERUM (strcmp(html_arbor_synthesis_titulus(
            (s32)HTML_SYNTHESIS_CORPUS_TABULAE), "tbody") == ZEPHYRUM);
        CREDO_VERUM (strcmp(html_arbor_synthesis_titulus(
            (s32)HTML_SYNTHESIS_COLUMNAE), "colgroup") == ZEPHYRUM);
        CREDO_NIHIL (html_arbor_synthesis_titulus(ZEPHYRUM));
        CREDO_NIHIL (html_arbor_synthesis_titulus(
            (s32)HTML_SYNTHESIS_NUMERUS));

        imprimere("\n--- Probans '' vacuum: involucra ad EOF ---\n");
        documentum = _parsare_documentum(piscina, "");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (_synthesis(_involutum(documentum, I)),
            (s32)HTML_SYNTHESIS_CORPUS);

        imprimere("\n--- Probans '<title>t</title><p>a' ---\n");
        documentum = _parsare_documentum(piscina,
            "<title>t</title><p>a");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        liber = _involutum(documentum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_CAPUT);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), I);
        liber = _involutum(documentum, I);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_CORPUS);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), I);

        imprimere("\n--- Probans album ante html: omissum ---\n");
        CREDO_VERUM (_octetos_probare(piscina, "\n<p>x", V));
        documentum = _parsare_documentum(piscina, "\n<p>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        /* NON elementum-malum: DOM id abicit, sed auctor nihil
         * peccavit - genus 'spatium-omissum' distinctionem fert ut
         * diagnosticum in elementum-malum solum cadat (2026-09-19) */
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_SPATIUM_OMISSUM);
        /* textus NON albus ante html manet contentum, non omissum */
        documentum = _parsare_documentum(piscina, "z<p>x");
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_ELEMENTUM);

        /* NUL in modo involucri: spec eum NEGLEGIT (vitium parsurae),
         * ergo MALUM - non spatium omissum (culpa adest) neque
         * contentum. Contentum erat, et corpus fictum '<frameset>'
         * sequentem reiciebat: octo casus html5lib. */
        /* VEXILLUM 'frameset-ok': index spec BREVIS est et nominatus
         * ('not ok' ponunt li/dd/dt/pre/button/table/input/...),
         * cetera vexillum INTACTUM relinquunt. Tabula olim inversa
         * erat - sedecim tags capitis innocua, CETERA omnia
         * exstinguentia - ergo '<p>' et '<div>' id exstinguebant cum
         * non deberent. Casus tres polum utrumque tenent. */
        imprimere("\n--- Probans vexillum frameset-ok ---\n");
        {
            /* 'p' vexillum NON exstinguit: compages ACCEPTA, malum
             * nullum. Sub tabula vetere hic malum unum stabat. */
            documentum = _parsare_documentum(piscina, "<p><frameset>");
            CREDO_AEQUALIS_I32 (_mala_numerare(documentum), ZEPHYRUM);
            /* 'li' vexillum EXSTINGUIT (spec nominatim): compages
             * NEGLECTA, ergo malum unum */
            documentum = _parsare_documentum(piscina, "<li><frameset>");
            CREDO_AEQUALIS_I32 (_mala_numerare(documentum), (i32)I);
            /* '<body>' iteratum nodum non dat sed vexillum tamen
             * exstinguit - effectus lexematis NEGLECTI */
            documentum = _parsare_documentum(piscina,
                "<div><body><frameset>");
            CREDO_AEQUALIS_I32 (_mala_numerare(documentum), (i32)II);
            /* intra template compages NEGLECTA quamvis vexillum
             * 'ok' maneat: condicio acervi, non vexilli */
            documentum = _parsare_documentum(piscina,
                "<template><div><frameset></div></template>");
            CREDO_VERUM (_mala_numerare(documentum) > ZEPHYRUM);
        }

        imprimere("\n--- Probans NUL ante html: malum ---\n");
        {
            hic_manens constans character NUL_ANTE[] =
                "<html>\0<frameset></frameset>";
            constans MateriaNodus* radix_nul;
            constans MateriaNodus* html_nul;

            radix_nul = html_arbor_parsare(piscina, NUL_ANTE,
                (i32)XXVIII);
            CREDO_NON_NIHIL (radix_nul);
            html_nul = _liber(radix_nul, HTML_DOCUMENTUM_LIBERI,
                ZEPHYRUM);
            CREDO_NON_NIHIL (html_nul);
            si (html_nul != NIHIL)
            {
                CREDO_AEQUALIS_I32 (_numerus(html_nul,
                    HTML_ELEMENTUM_LIBERI), (i32)III);
                /* I. NUL ipse MALUM est, non textus (culpa adest) */
                CREDO_AEQUALIS_S32 (_liber(html_nul,
                    HTML_ELEMENTUM_LIBERI, ZEPHYRUM)->genus,
                    (s32)HTML_GENUS_ELEMENTUM_MALUM);
                /* II. EFFECTUS qui refert: corpus NON fingitur, ergo
                 * frameset VERUM superest. Cum NUL contentum erat,
                 * corpus fictum hic stabat et frameset peribat -
                 * octo casus html5lib. Synthesis NULLA = elementum
                 * verum, non fictum. */
                CREDO_AEQUALIS_S32 (_synthesis(_liber(html_nul,
                    HTML_ELEMENTUM_LIBERI, (i32)II)),
                    (s32)HTML_SYNTHESIS_NULLA);
            }
        }
        /* commentarium ante html non fingit: sub documento manet */
        documentum = _parsare_documentum(piscina, "<!--c--><p>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_COMMENTARIUM);

        imprimere("\n--- Probans '</head>x': head fictum clausum "
            "---\n");
        CREDO_VERUM (_octetos_probare(piscina, "</head>x", VIII));
        documentum = _parsare_documentum(piscina, "</head>x");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        liber = _involutum(documentum, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_CAPUT);
        CREDO_VERUM (_absens(liber, HTML_ELEMENTUM_TOK_APERTURA));
        CREDO_NON_NIHIL (_tok(liber, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_S32 (_synthesis(_involutum(documentum, I)),
            (s32)HTML_SYNTHESIS_CORPUS);

        imprimere("\n--- Probans partes tabulae fictas ---\n");
        CREDO_VERUM (_octetos_probare(piscina, "<table><td>x", XII));
        documentum = _parsare(piscina, "<table><td>x");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_CORPUS_TABULAE);
        liber = liber ? _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)
                      : NIHIL;
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_ORDO);
        liber = liber ? _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)
                      : NIHIL;
        CREDO_NON_NIHIL (liber);
        si (liber != NIHIL)
        {
            CREDO_AEQUALIS_S32 (_synthesis(liber),
                (s32)HTML_SYNTHESIS_NULLA);
            CREDO_NON_NIHIL (_tok(liber, HTML_ELEMENTUM_TOK_APERTURA));
        }
        documentum = _parsare(piscina, "<table><col><col>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            I);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_COLUMNAE);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), II);
        /* '</tbody>' fictum claudit (clausura vera in ficto); tr
         * sequens tbody novum */
        documentum = _parsare(piscina, "<table><tr></tbody><tr>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(liber, HTML_ELEMENTUM_TOK_CLAUSURA));
        /* partes tabulae extra tabulam neglectae */
        documentum = _parsare(piscina, "<td>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);

        imprimere("\n--- Probans fragmenta: contextus vertex sub "
            "acervo ---\n");
        documentum = _parsare_fragmentum(piscina, "table", "<tr><td>x");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_CORPUS_TABULAE);
        documentum = _parsare_fragmentum(piscina, "tbody", "<td>x");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(elementum),
            (s32)HTML_SYNTHESIS_ORDO);
        /* contextus html: head et body sub radice ficta */
        documentum = _parsare_fragmentum(piscina, "html", "<p>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (_synthesis(_liber(documentum,
            HTML_DOCUMENTUM_LIBERI, I)), (s32)HTML_SYNTHESIS_CORPUS);
        /* contextus body: nihil fictum, '<html>' neglectum */
        documentum = _parsare(piscina, "<html><p>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
        /* contextus frameset: p neglectum, frame acceptum */
        documentum = _parsare_fragmentum(piscina, "frameset",
            "<p><frame>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
    }


    /* ==================================================
     * STRUCTURA O7b: sedes - foster parenting, post body, post head
     * ================================================== */

    {
        MateriaNodus* documentum;
        MateriaNodus* elementum;
        MateriaNodus* liber;
        MateriaNodus* corpus;

        imprimere("\n--- Probans '<table><div>x</div><tr><td>1"
            "</table>': fotum ---\n");
        /* div liber tabulae per octetos, sedes = parens tabulae (radix
         * fragmenti); x intra div sine sede; tbody fictum sine sede */
        CREDO_VERUM (_octetos_probare(piscina,
            "<table><div>x</div><tr><td>1</table>", XXXVI));
        documentum = _parsare(piscina,
            "<table><div>x</div><tr><td>1</table>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_PTR (_sedes(liber), documentum);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), I);
        CREDO_NIHIL (_sedes(_liber(liber, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM)));
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, I);
        CREDO_AEQUALIS_S32 (_synthesis(liber),
            (s32)HTML_SYNTHESIS_CORPUS_TABULAE);
        CREDO_NIHIL (_sedes(liber));
        /* textus non albus fotus, albus in tabula manet */
        documentum = _parsare(piscina, "<table>a<tr>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (liber->genus, (s32)HTML_GENUS_TEXTUS);
        CREDO_AEQUALIS_PTR (_sedes(liber), documentum);
        documentum = _parsare(piscina, "<table> <tr>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NIHIL (_sedes(_liber(elementum, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM)));
        /* '<table><div><td>': td acervum ad tabulam purgat (div cadit),
         * tbody/tr ficta */
        documentum = _parsare(piscina, "<table><div><td>x");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (_synthesis(_liber(elementum,
            HTML_ELEMENTUM_LIBERI, I)),
            (s32)HTML_SYNTHESIS_CORPUS_TABULAE);
        /* '<table><table>': tabula tabulam claudit, fratres */
        documentum = _parsare(piscina, "<table><table>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        /* '<td>a<div><td>b': cella per div clausa (purgatio ad td) */
        documentum = _parsare(piscina, "<table><td>a<div><td>b");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        liber = _liber(elementum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        liber = liber ? _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)
                      : NIHIL;   /* tr fictum */
        CREDO_NON_NIHIL (liber);
        si (liber != NIHIL)
        {
            CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI),
                II);
        }
        /* input type=hidden in tabula manet, input alius fotus */
        documentum = _parsare(piscina,
            "<table><input type=hidden><input>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NIHIL (_sedes(_liber(elementum, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM)));
        CREDO_AEQUALIS_PTR (_sedes(_liber(elementum,
            HTML_ELEMENTUM_LIBERI, I)), documentum);
        /* form in tabula: inseritur, non impellitur (div fotum, non
         * intra form) */
        documentum = _parsare(piscina, "<table><form><div>");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            II);
        CREDO_VERUM (_absens(_liber(elementum, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM), HTML_ELEMENTUM_LIBERI));
        CREDO_AEQUALIS_PTR (_sedes(_liber(elementum,
            HTML_ELEMENTUM_LIBERI, I)), documentum);

        imprimere("\n--- Probans post body: '<body></body><!--c-->"
            "<p>x' ---\n");
        /* body clausum; commentarium in html (spec 'after body'); p
         * liber html per octetos, sedes = body */
        CREDO_VERUM (_octetos_probare(piscina,
            "<body></body><!--c--><p>x", XXV));
        documentum = _parsare_documentum(piscina,
            "<body></body><!--c--><p>x");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(elementum, HTML_ELEMENTUM_LIBERI),
            IV);
        corpus = _involutum(documentum, I);
        CREDO_NON_NIHIL (_tok(corpus, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_NIHIL (_sedes(_involutum(documentum, II)));
        CREDO_AEQUALIS_PTR (_sedes(_involutum(documentum, III)),
            corpus);
        /* post '</html>': html clausum; x et commentarium (contentum
         * iam rediit) in body per sedem, liberi documenti per
         * octetos */
        documentum = _parsare_documentum(piscina,
            "<html><body></body></html>x<!--c-->");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            III);
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(elementum, HTML_ELEMENTUM_TOK_CLAUSURA));
        corpus = _involutum(documentum, I);
        CREDO_AEQUALIS_PTR (_sedes(_liber(documentum,
            HTML_DOCUMENTUM_LIBERI, I)), corpus);
        CREDO_AEQUALIS_PTR (_sedes(_liber(documentum,
            HTML_DOCUMENTUM_LIBERI, II)), corpus);
        /* '</body>' cum elemento aperto: sedes = vertex tum apertus */
        documentum  = _parsare_documentum(piscina, "<bdy></body><br>");
        corpus      = _involutum(documentum, I);
        liber = corpus ? _liber(corpus, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)
                       : NIHIL;   /* bdy */
        CREDO_NON_NIHIL (liber);
        CREDO_AEQUALIS_PTR (_sedes(_involutum(documentum, II)), liber);

        imprimere("\n--- Probans caput post head: '</head><title>' "
            "---\n");
        documentum = _parsare_documentum(piscina,
            "<head></head><title>t</title><p>x");
        elementum  = _involutum(documentum, ZEPHYRUM);   /* head */
        liber      = _involutum(documentum, I);              /* title */
        CREDO_AEQUALIS_PTR (_sedes(liber), elementum);
        CREDO_AEQUALIS_S32 (_synthesis(_involutum(documentum, II)),
            (s32)HTML_SYNTHESIS_CORPUS);

        imprimere("\n--- Probans fragmenta contextu tabulae ---\n");
        /* contextus table: div intra tr fictum, sedes = radix */
        documentum = _parsare_fragmentum(piscina, "table",
            "<tr><div>x");
        elementum = _liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);   /* tbody fictum */
        liber = elementum ? _liber(elementum, HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM) : NIHIL;   /* tr fictum */
        liber = liber ? _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)
                      : NIHIL;   /* div */
        CREDO_NON_NIHIL (liber);
        CREDO_AEQUALIS_PTR (_sedes(liber), documentum);
        /* contextus tbody: caption neglectum (spec: nulla pars vera in
         * acervo, 'in table scope' fallit) */
        documentum = _parsare_fragmentum(piscina, "tbody",
            "<caption>x");
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
        /* contextus caption: table nova (in body), tbody neglectum */
        documentum = _parsare_fragmentum(piscina, "caption",
            "<table></table><tbody>");
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_ELEMENTUM);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, I)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);
    }


    /* ==================================================
     * STRUCTURA O7c: adoption agency - exemplar, praecedens,
     * involutio, reconstructio, arca Noe, signa
     * ================================================== */

    {
        MateriaNodus* documentum;
        MateriaNodus* formans;
        MateriaNodus* bloccum;
        MateriaNodus* exemplar;
        MateriaNodus* liber;

        imprimere("\n--- Probans '<a>1<p>2</a>3</p>': adoptio simplex "
            "---\n");
        /* octeti: a{1, p{2}}, '3', malum '</p>'; DOM: p in radicem
         * post a (sedes + praecedens), exemplar a in p liberos p
         * involvit, '3' in p per sedem (p octetis clausum) */
        CREDO_VERUM (_octetos_probare(piscina, "<a>1<p>2</a>3</p>",
            XVII));
        documentum = _parsare(piscina, "<a>1<p>2</a>3</p>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            III);
        formans = _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(formans, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_I32 (_numerus(formans, HTML_ELEMENTUM_LIBERI),
            II);
        bloccum = _liber(formans, HTML_ELEMENTUM_LIBERI, I);
        CREDO_AEQUALIS_PTR (_sedes(bloccum), documentum);
        CREDO_AEQUALIS_PTR (_praecedens(bloccum), formans);
        CREDO_AEQUALIS_I32 (_numerus(bloccum, HTML_ELEMENTUM_LIBERI),
            I);
        exemplar = _liber(bloccum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_PTR (_exemplar(exemplar), formans);
        CREDO_VERUM (_absens(exemplar, HTML_ELEMENTUM_TOK_APERTURA));
        CREDO_AEQUALIS_I32 (_numerus(exemplar, HTML_ELEMENTUM_LIBERI),
            I);
        liber = _liber(documentum, HTML_DOCUMENTUM_LIBERI, I);   /* 3 */
        CREDO_AEQUALIS_S32 (liber->genus, (s32)HTML_GENUS_TEXTUS);
        CREDO_AEQUALIS_PTR (_sedes(liber), bloccum);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, II)->genus,
            (s32)HTML_GENUS_ELEMENTUM_MALUM);

        imprimere("\n--- Probans '<b><a><div>x</b>y</a>': catena et "
            "praecedens ---\n");
        /* '</b>': div in exemplar a' (catena) in radicem; b' liberos
         * div involvit. 'y' in a' per octetos, sedes div. '</a>':
         * clausura in a' (octetis apertum), div in radicem POST a'
         * (praecedens - ordo octetorum id non daret), a'' liberos div
         * (b' et y) involvit */
        CREDO_VERUM (_octetos_probare(piscina, "<b><a><div>x</b>y</a>",
            XXI));
        documentum = _parsare(piscina, "<b><a><div>x</b>y</a>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        formans = _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM);
        CREDO_NON_NIHIL (_tok(formans, HTML_ELEMENTUM_TOK_CLAUSURA));
        /* liber = a, bloccum = div, exemplar = a' */
        liber     = _liber(formans, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        bloccum   = _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        exemplar  = _liber(documentum, HTML_DOCUMENTUM_LIBERI, I);
        CREDO_AEQUALIS_PTR (_exemplar(exemplar), liber);
        CREDO_NON_NIHIL (_tok(exemplar, HTML_ELEMENTUM_TOK_CLAUSURA));
        CREDO_AEQUALIS_PTR (_sedes(bloccum), documentum);
        CREDO_AEQUALIS_PTR (_praecedens(bloccum), exemplar);
        CREDO_AEQUALIS_I32 (_numerus(exemplar, HTML_ELEMENTUM_LIBERI),
            I);
        /* liber = y (in a' per octetos) */
        liber = _liber(exemplar, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (liber->genus, (s32)HTML_GENUS_TEXTUS);
        CREDO_AEQUALIS_I32 (_numerus(bloccum, HTML_ELEMENTUM_LIBERI),
            I);
        /* exemplar = a'' (liber div), y in a'' per sedem */
        exemplar = _liber(bloccum, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_PTR (_exemplar(exemplar),
            _liber(formans, HTML_ELEMENTUM_LIBERI, ZEPHYRUM));
        CREDO_AEQUALIS_PTR (_sedes(liber), exemplar);
        /* liber = b' (liber a'') */
        liber = _liber(exemplar, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_PTR (_exemplar(liber), formans);
        CREDO_AEQUALIS_S32 (
            _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_TEXTUS);

        imprimere("\n--- Probans '<p><b>x</p>y': reconstructio ---\n");
        /* b per '</p>' cadit, in lista manet: 'y' exemplar b aperit */
        documentum = _parsare(piscina, "<p><b>x</p>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        formans = _liber(_liber(documentum, HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM), HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        exemplar = _liber(documentum, HTML_DOCUMENTUM_LIBERI, I);
        CREDO_AEQUALIS_PTR (_exemplar(exemplar), formans);
        CREDO_NIHIL (_sedes(exemplar));
        CREDO_AEQUALIS_S32 (
            _liber(exemplar, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_TEXTUS);

        imprimere("\n--- Probans arcam Noe et signum ---\n");
        /* quartum b primum e lista tollit: '</b>' quater b omnia
         * claudit, exemplar nullum, 'y' in radice */
        documentum = _parsare(piscina,
            "<b><b><b><b>x</b></b></b></b>y");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_S32 (
            _liber(documentum, HTML_DOCUMENTUM_LIBERI, I)->genus,
            (s32)HTML_GENUS_TEXTUS);
        /* td signum impellit: b intra cellam post '</td>' e lista cadit
         * (signum lapsum purgat) - 'y' fotum sine exemplari */
        documentum = _parsare(piscina, "<table><td><b>x</td>y");
        /* table > tbody > tr: liberi td et y */
        liber = _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM);
        liber = _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        liber = _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_numerus(liber, HTML_ELEMENTUM_LIBERI), II);
        liber = _liber(liber, HTML_ELEMENTUM_LIBERI, I);
        CREDO_AEQUALIS_S32 (liber->genus, (s32)HTML_GENUS_TEXTUS);
        CREDO_AEQUALIS_PTR (_sedes(liber), documentum);

        imprimere("\n--- Probans '<a>' iteratum et '<nobr>' ---\n");
        /* a in lista: adoptio a prioris (sine blocco: clausura
         * implicita), deinde a novum frater */
        documentum = _parsare(piscina, "<a href=x>1<a href=y>2");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            II);
        CREDO_AEQUALIS_I32 (_numerus(_liber(documentum,
            HTML_DOCUMENTUM_LIBERI, ZEPHYRUM), HTML_ELEMENTUM_LIBERI),
            I);
        /* a non in scopo ('<table>' limes): e lista et acervo remotum,
         * a novum fotum in a prius (parens DOM tabulae) */
        documentum = _parsare(piscina, "<a><table><a>x");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            I);
        /* formans = a prius, liber = a novum (liber tabulae) */
        formans  = _liber(documentum, HTML_DOCUMENTUM_LIBERI, ZEPHYRUM);
        liber    = _liber(formans, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        liber    = _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_NON_NIHIL (liber);
        CREDO_AEQUALIS_PTR (_sedes(liber), formans);
        CREDO_AEQUALIS_S32 (
            _liber(liber, HTML_ELEMENTUM_LIBERI, ZEPHYRUM)->genus,
            (s32)HTML_GENUS_TEXTUS);
        /* nobr in scopo: adoptio, deinde nobr novum - tres fratres,
         * exemplar nullum (tests3 #21) */
        documentum = _parsare(piscina, "<nobr><nobr><nobr>");
        CREDO_AEQUALIS_I32 (_numerus(documentum,
            HTML_DOCUMENTUM_LIBERI),
            III);
        CREDO_NIHIL (_exemplar(_liber(documentum,
            HTML_DOCUMENTUM_LIBERI,
            II)));
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
