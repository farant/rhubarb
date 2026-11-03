/* probatio_xml_arbor.c - Aedificator xml: structura, diagnostica, lex
 * octetorum
 *
 * I.   STRUCTURA: casus manu scripti - genera liberorum, mala,
 *      diagnostica emissa (codices) contra exspectata.
 * II.  LEX OCTETORUM: emissio(parsatio(x)) == x pro omni casu inlineo,
 *      casu suitae W3C (bene formato aut non) et plagula silvestri.
 *      Suite absente: OMITTITUR CUM NUMERO.
 * III. PROFUNDITAS: C milia elementorum nidificatorum parsantur
 *      (aedificator iterativus); emissio ad X milia (emissio materiae
 *      recursiva - limes eius in parco ...FAD8, non hic).
 */

#include "latina.h"
#include "credo.h"
#include "xml_arbor.h"
#include "xml_registrum.h"
#include "xml_lexicon.h"
#include "xml_corpus_ambulare.h"
#include "materia_scribere.h"
#include "materia_diagnosticum.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* parsare + emissio octetim idem; FALSUM si differt */
interior b32
_lex (
               Piscina*  piscina,
    constans character*  fons,
                   s32   mensura,
            XmlParsura*  relatio,
          MateriaNodus** radix_out)
{
                 MateriaNodus* radix;
             MateriaScriptura  emissa;
    MateriaScripturaConsilium  consilium;

    radix = xml_arbor_parsare(piscina, fons, mensura, relatio);
    si (radix_out != NIHIL)
    {
        *radix_out = radix;
    }
    si (radix == NIHIL)
    {
        redde FALSUM;
    }
    materia_scriptura_consilium_nudum(&consilium, &XML_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    redde emissa.successus && emissa.textus.mensura == (i32)mensura
        && (mensura == ZEPHYRUM
            || memcmp(emissa.textus.datum, fons, (size_t)mensura)
                == ZEPHYRUM);
}

interior i32
_numerus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    constans MateriaValor* v = &nodus->loci[locus];

    redde v->genus == MATERIA_VALOR_LISTA
        ? materia_valor_lista_numerus(*v) : ZEPHYRUM;
}

interior MateriaNodus*
_filius (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  k)
{
    redde materia_valor_lista_obtinere(nodus->loci[locus], k)
        ->datum.nodus;
}

/* codices diagnosticorum emissorum, '|' iuncti, in buffer */
interior vacuum
_codices (
    constans XmlParsura* r,
              character* b,
                    i32  capacitas)
{
    i32 k;

    b[ZEPHYRUM] = '\0';
    per (k = ZEPHYRUM; r->diagnostica != NIHIL
         && k < xar_numerus(r->diagnostica); k++)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)
            xar_obtinere(r->diagnostica, k);

        si (strlen(b) + strlen(d->codex) + II < (size_t)capacitas)
        {
            si (k > ZEPHYRUM)
            {
                strcat(b, "|");
            }
            strcat(b, d->codex);
        }
    }
}

nomen structura {
    constans character* fons;
    constans character* genera;    /* genera liberorum documenti, ' ' */
                   i32  mala;
    constans character* codices;   /* emissa, '|' */
} CasusStructurae;

hic_manens constans CasusStructurae CASUS[] = {
    { "<a/>", "elementum", ZEPHYRUM, "" },
    { "<a>x<b/></a>", "elementum", ZEPHYRUM, "" },
    { "<?xml version='1.0'?>\n<!-- c -->\n<a/>\n",
        "declaratio-xml textus commentarium textus elementum textus",
        ZEPHYRUM, "" },
    { "\xef\xbb\xbf<a/>", "signum-ordinis elementum", ZEPHYRUM, "" },
    { "<!DOCTYPE a [<!ELEMENT a ANY>]><a/>", "doctype elementum",
        ZEPHYRUM, "" },
    { "<a><b></a>", "elementum", ZEPHYRUM,
        "elementum/tag-finalis-deest" },
    { "<a></b>", "elementum", I,
        "elementum/tag-finalis-solus|elementum/tag-finalis-deest" },
    { "</a>", "malum", I,
        "elementum/tag-finalis-solus|documentum/radix-deest" },
    { "<a/><b/>", "elementum elementum", ZEPHYRUM,
        "documentum/radix-altera" },
    { "x<a/>", "textus elementum", ZEPHYRUM, "documentum/textus" },
    { "<a/>&amp;", "elementum referentia", ZEPHYRUM,
        "documentum/contentum" },
    { "<a/><!DOCTYPE a>", "elementum doctype", ZEPHYRUM,
        "documentum/doctype" },
    { "<a></a b>", "elementum malum", I,
        "elementum/tok_clausura_finis" },
    { "", "", ZEPHYRUM, "documentum/radix-deest" },
    { "<a", "elementum", ZEPHYRUM, "elementum/tag-finalis-deest" },
    { "<a <b/></a>", "elementum", ZEPHYRUM, "" },
    /* ']]>' in contento: lexema IGNOTUM in malum */
    { "<a>]]></a>", "elementum", I, "" },
    { "<!DOCTYPE a %x>", "doctype malum", I, "documentum/radix-deest" },
    { NIHIL, NIHIL, ZEPHYRUM, NIHIL }
};

interior vacuum
_structuram_probare (
    CredoContextus* c)
{
    i32 i;

    per (i = ZEPHYRUM; CASUS[i].fons != NIHIL; i++)
    {
          XmlParsura r;
        MateriaNodus* radix;
           character  genera[CCLVI];
           character  codices[CCLVI];
                 i32  k;
                 i32  n;
                 b32  idem;

        idem = _lex(c->piscina, CASUS[i].fons,
            (s32)strlen(CASUS[i].fons),
            &r, &radix);
        CREDO_VERUM (idem);
        si (radix == NIHIL)
        {
            perge;
        }
        genera[ZEPHYRUM]  = '\0';
        n                 = _numerus(radix, (i32)XML_DOCUMENTUM_LIBERI);
        per (k = ZEPHYRUM; k < n; k++)
        {
            MateriaNodus* f = _filius(radix, (i32)XML_DOCUMENTUM_LIBERI,
                k);

            si (k > ZEPHYRUM)
            {
                strcat(genera, " ");
            }
            strcat(genera, XML_REGISTRUM.genera[f->genus].titulus);
        }
        _codices(&r, codices, CCLVI);
        si (   strcmp(genera, CASUS[i].genera)   != ZEPHYRUM
            || r.mala                            != CASUS[i].mala
            || strcmp(codices, CASUS[i].codices) != ZEPHYRUM || !idem)
        {
            imprimere("    casus '%s': genera '%s', mala %u, codices"
                " '%s'\n", CASUS[i].fons, genera,
                (insignatus integer)r.mala, codices);
        }
        CREDO_VERUM (strcmp(genera, CASUS[i].genera) == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r.mala, CASUS[i].mala);
        CREDO_VERUM (strcmp(codices, CASUS[i].codices) == ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)r.sana, (s32)(CASUS[i].mala == ZEPHYRUM
            && CASUS[i].codices[ZEPHYRUM] == '\0'));
    }
}

interior vacuum
_elementum_probare (
    CredoContextus* c)
{
      XmlParsura r;
    MateriaNodus* radix;
    MateriaNodus* e;
    MateriaNodus* a;

    CREDO_VERUM (_lex(c->piscina, "<a x = 'y' z=\"w\">t<b/></a >",
        (s32)strlen("<a x = 'y' z=\"w\">t<b/></a >"), &r, &radix));
    CREDO_NON_NIHIL (radix);
    si (radix == NIHIL)
    {
        redde;
    }
    e = _filius(radix, (i32)XML_DOCUMENTUM_LIBERI, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (e->genus, (s32)XML_GENUS_ELEMENTUM);
    CREDO_AEQUALIS_I32 (_numerus(e, (i32)XML_ELEMENTUM_ATTRIBUTA),
        (i32)II);
    CREDO_AEQUALIS_I32 (_numerus(e, (i32)XML_ELEMENTUM_LIBERI),
        (i32)II);
    a = _filius(e, (i32)XML_ELEMENTUM_ATTRIBUTA, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (a->genus, (s32)XML_GENUS_ATTRIBUTUM);
    CREDO_AEQUALIS_S32 ((s32)a->loci[XML_ATTRIBUTUM_TOK_VALOR].genus,
        (s32)MATERIA_VALOR_TOKEN);
    CREDO_AEQUALIS_S32 ((s32)e->loci[XML_ELEMENTUM_TOK_CLAUSURA_FINIS]
        .genus, (s32)MATERIA_VALOR_TOKEN);
    CREDO_VERUM (r.sana);

    /* '<a><b></a>': diagnosticum in '<b' (offset III), tag finalis
     * elementi EXTERIORIS, b sine tag finali */
    CREDO_VERUM (_lex(c->piscina, "<a><b></a>", X, &r, &radix));
    CREDO_NON_NIHIL (radix);
    si (   radix                      == NIHIL || r.diagnostica == NIHIL
        || xar_numerus(r.diagnostica) != I)
    {
        CREDO_CULPA ("diagnosticum unum exspectatum");
        redde;
    }
    CREDO_AEQUALIS_S32 (((constans MateriaDiagnosticum*)xar_obtinere(
        r.diagnostica, ZEPHYRUM))->tractus.initium, (s32)III);
    e = _filius(radix, (i32)XML_DOCUMENTUM_LIBERI, ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)e->loci[XML_ELEMENTUM_TOK_CLAUSURA].genus,
        (s32)MATERIA_VALOR_TOKEN);
    a = _filius(e, (i32)XML_ELEMENTUM_LIBERI, ZEPHYRUM);
    CREDO_AEQUALIS_S32 ((s32)a->loci[XML_ELEMENTUM_TOK_CLAUSURA].genus,
        (s32)MATERIA_VALOR_NIHIL);
}


/* ==================================================
 * LEX OCTETORUM
 * ================================================== */

nomen structura {
    i32 plagulae;
    i32 fractae;
    i32 sanae;
    i32 maximae;    /* supra XML_MAXIMA_ARBORIS: omissae cum numero */
} StatusLegis;

/* Arbor ~XIII-XXXVI x fontis (mensuratum 2026-10-30 super plagulas
 * silvestres > I MB: lexema LXIV octeti, nodus et listae per
 * elementum); douay-rheims.usfx.xml LX x (XVI,7 MB -> I,0 GB), expat
 * ns_att_test / recset (XXX-XXXV MB) ~XX x - tectum I GiB sectionis
 * excedunt. Porta arboris plagulas supra XII MB OMITTIT CUM NUMERO
 * (nominatim, quattuor hodie); porta lectoris eas totas legit;
 * sumptus per octetum in computo (P14). */
#define XML_MAXIMA_ARBORIS ((i32)XII * MXXIV * MXXIV)

interior vacuum
_visor (
                   vacuum* datum,
             XmlCorpusFons fons,
       constans character* via,
    constans XmlCasusSelectus* selectus,
                    chorda textus,
                  Piscina* opus)
{
    StatusLegis* st = (StatusLegis*)datum;
     XmlParsura  r;

    (vacuum)fons;
    (vacuum)selectus;
    si (textus.mensura > XML_MAXIMA_ARBORIS)
    {
        st->maximae++;
        imprimere("    MAXIMA OMISSA (%u octeti): %s\n",
            (insignatus integer)textus.mensura, via);
        redde;
    }
    st->plagulae++;
    si (!_lex(opus, (constans character*)textus.datum,
        (s32)textus.mensura, &r, NIHIL))
    {
        st->fractae++;
        si (st->fractae <= (i32)XX)
        {
            imprimere("    LEX FRACTA: %s\n", via);
        }
        redde;
    }
    si (r.sana)
    {
        st->sanae++;
    }
}

interior vacuum
_legem_probare (
    CredoContextus* c)
{
        StatusLegis  st;
    XmlCorpusNumeri  nn;
            Piscina* opus;
                i32  i;

    memset(&st, ZEPHYRUM, magnitudo(st));
    opus = piscina_generare_dynamicum("xml_arbor_opus",
        (memoriae_index)(I * MXXIV * MXXIV));
    CREDO_NON_NIHIL (opus);
    si (opus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; XML_CASUS[i] != NIHIL; i++)
    {
        _visor(&st, XML_CORPUS_XMLTS, XML_CASUS[i], NIHIL,
            chorda_ex_literis(XML_CASUS[i], opus), opus);
        piscina_vacare(opus);
    }
    imprimere("  casus inlinei: %u, fracti %u, sani %u\n",
        (insignatus integer)st.plagulae, (insignatus integer)st.fractae,
        (insignatus integer)st.sanae);
    CREDO_AEQUALIS_I32 (st.fractae, ZEPHYRUM);

    memset(&st, ZEPHYRUM, magnitudo(st));
    xml_corpus_ambulare(c->piscina, opus, xml_corpus_radix(),
        (i32)((I << XML_CORPUS_XMLTS) | (I << XML_CORPUS_SILVESTRIA)),
        _visor, &st, &nn);
    si (nn.xmlts_absens)
    {
        imprimere("  OMISSA: suite W3C abest (./xml/xmlts_arcessere.sh)"
            "\n");
    }
    imprimere("  xmlts %u, silvestria %u (omissae %u, maximae %u):"
        " fractae %u, sanae %u\n",
        (insignatus integer)nn.plagulae[XML_CORPUS_XMLTS],
        (insignatus integer)nn.plagulae[XML_CORPUS_SILVESTRIA],
        (insignatus integer)nn.omissae, (insignatus integer)st.maximae,
        (insignatus integer)st.fractae, (insignatus integer)st.sanae);
    CREDO_MINOR_I32 (st.maximae, (i32)V);
    CREDO_AEQUALIS_I32 (st.fractae, ZEPHYRUM);
    piscina_destruere(opus);
}


/* ==================================================
 * PROFUNDITAS
 * ================================================== */

interior character*
_nidi (
    Piscina* piscina,
        i32  n,
        s32* mensura)
{
    character* b = (character*)piscina_allocare(piscina,
        (memoriae_index)(n * VII + I));
          i32 k;
          s32 m = ZEPHYRUM;

    si (b == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        memcpy(b + m, "<a>", III);
        m += III;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        memcpy(b + m, "</a>", IV);
        m += IV;
    }
    *mensura = m;
    redde b;
}

interior vacuum
_profunditatem_probare (
    CredoContextus* c)
{
      XmlParsura  r;
             s32  m;
       character* b;
    MateriaNodus* radix;

    b = _nidi(c->piscina, (i32)C * M, &m);
    CREDO_NON_NIHIL (b);
    si (b == NIHIL)
    {
        redde;
    }
    radix = xml_arbor_parsare(c->piscina, b, m, &r);
    CREDO_NON_NIHIL (radix);
    CREDO_VERUM (r.sana);
    CREDO_MAIOR_I32 (r.profunditas_maxima, (i32)C * M);

    b = _nidi(c->piscina, (i32)X * M, &m);
    CREDO_VERUM (_lex(c->piscina, b, m, &r, NIHIL));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "structura et diagnostica emissa", _structuram_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "elementum: attributa, liberi, tag finalis", _elementum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "lex octetorum: casus, suite W3C, silvestria", _legem_probare,
        NIHIL, NIHIL, "gravis", NIHIL },
    { "profunditas: C milia parsata, X milia emissa",
        _profunditatem_probare, NIHIL, NIHIL, "gravis", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_arbor", SECTIONES, NIHIL,
        NIHIL);
}
