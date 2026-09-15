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
 *
 * H3: sine elementis vacuis et clausuris implicitis (H4). Casus
 * qui eas poscunt HIC NON asseruntur - non tacentur: H4 eos addit.
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


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
