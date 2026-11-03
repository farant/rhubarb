/* probatio_xml_lector.c - Lector xml: modi, transitus, totalitas
 *
 * I.  TRANSITUS: tabula casuum (fons -> lexemata exspectata, genus et
 *     octeti) - quisque transitus modorum xml_lector.h et Review
 *     Focus plani (']]>', '--', '<?xml' loco, '<' in valore, BOM).
 * II. TOTALITAS: pro omni casu inlineo, casu suitae W3C et plagula
 *     silvestri - lexemata contigua, omnem octetum tegunt, concatenatio
 *     = fons, FINIS ultimum ad mensuram, linea et columna ex octetis
 *     recte. Suite absente: OMITTITUR CUM NUMERO (clamose).
 */

#include "latina.h"
#include "credo.h"
#include "xml_lector.h"
#include "xml_lexicon.h"
#include "xml_corpus_ambulare.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

#define FINIS_CASUS (-I)

nomen structura {
                   s32  genus;
    constans character* textus;
} LexemaExspectatum;

nomen structura {
    constans character* fons;
     LexemaExspectatum  lexemata[XXIV];
} CasusTransitus;

#define LEX(g, t) { (s32)XML_LEX_##g, (t) }
#define F { FINIS_CASUS, NIHIL }
/* praefixum solum probatur (casus longi) */
#define PRAEFIXUM_CASUS (-II)
#define P { PRAEFIXUM_CASUS, NIHIL }

hic_manens constans CasusTransitus CASUS[] = {
    { "<a/>", { LEX(TAG_APERTURA, "<a"), LEX(TAG_FINIS_SOLUS, "/>"),
        F } },
    { "<a b=\"c\" d='e'>x</a>", { LEX(TAG_APERTURA, "<a"),
        LEX(SPATIA, " "), LEX(TITULUS, "b"), LEX(AEQUALE, "="),
        LEX(VALOR, "\"c\""), LEX(SPATIA, " "), LEX(TITULUS, "d"),
        LEX(AEQUALE, "="), LEX(VALOR, "'e'"), LEX(TAG_FINIS, ">"),
        LEX(TEXTUS, "x"), LEX(TAG_CLAUSURA, "</a"), LEX(TAG_FINIS, ">"),
            F } },
    { "\xef\xbb\xbf<?xml version='1.0'?>\n<a/>",
        { LEX(SIGNUM_ORDINIS, "\xef\xbb\xbf"), LEX(DECLARATIO_XML,
            "<?xml"),
          LEX(SPATIA, " "), LEX(TITULUS, "version"), LEX(AEQUALE, "="),
          LEX(VALOR, "'1.0'"), LEX(INSTRUCTIO_FINIS, "?>"),
          LEX(TEXTUS, "\n"), LEX(TAG_APERTURA, "<a"),
          LEX(TAG_FINIS_SOLUS, "/>"), F } },
    /* '<?xml' non ad initium: instructio (scopus reservatus) */
    { " <?xml version='1.0'?>", { LEX(TEXTUS, " "),
        LEX(INSTRUCTIO, "<?xml version='1.0'?>"), F } },
    { "<?xml?>", { LEX(INSTRUCTIO, "<?xml?>"), F } },
    { "<a>x&amp;&#60;&#x3c;y</a>", { LEX(TAG_APERTURA, "<a"),
        LEX(TAG_FINIS, ">"), LEX(TEXTUS, "x"), LEX(REFERENTIA, "&amp;"),
        LEX(REFERENTIA, "&#60;"), LEX(REFERENTIA, "&#x3c;"),
        LEX(TEXTUS, "y"), LEX(TAG_CLAUSURA, "</a"), LEX(TAG_FINIS, ">"),
            F } },
    /* referentiae sine forma */
    { "& &#; &#x; &a b", { LEX(IGNOTUM, "&"), LEX(TEXTUS, " "),
        LEX(IGNOTUM, "&"), LEX(TEXTUS, "#; "), LEX(IGNOTUM, "&"),
        LEX(TEXTUS, "#x; "), LEX(IGNOTUM, "&"), LEX(TEXTUS, "a b"),
            F } },
    { "a]]>b]]c", { LEX(TEXTUS, "a"), LEX(IGNOTUM, "]]>"),
        LEX(TEXTUS, "b]]c"), F } },
    { "<!-- a -- b --><![CDATA[<x>]]><?p d?>",
        { LEX(COMMENTARIUM, "<!-- a -- b -->"),
          LEX(CDATA, "<![CDATA[<x>]]>"), LEX(INSTRUCTIO, "<?p d?>"),
              F } },
    /* aperta usque ad finem */
    { "<!-- x", { LEX(COMMENTARIUM, "<!-- x"), F } },
    { "<![CDATA[x", { LEX(CDATA, "<![CDATA[x"), F } },
    /* '<' sine forma */
    { "< a<1<!x</ <a", { LEX(IGNOTUM, "<"), LEX(TEXTUS, " a"),
        LEX(IGNOTUM, "<"), LEX(TEXTUS, "1"), LEX(IGNOTUM, "<!"),
        LEX(TEXTUS, "x"), LEX(IGNOTUM, "</"), LEX(TEXTUS, " "),
        LEX(TAG_APERTURA, "<a"), F } },
    /* '<' in valore attributi: valor ante '<' desinit */
    { "<a b=\"x<c/>", { LEX(TAG_APERTURA, "<a"), LEX(SPATIA, " "),
        LEX(TITULUS, "b"), LEX(AEQUALE, "="), LEX(VALOR, "\"x"),
        LEX(TAG_APERTURA, "<c"), LEX(TAG_FINIS_SOLUS, "/>"), F } },
    /* tag non clausus: '<' modum contentum reddit */
    { "<a <b>", { LEX(TAG_APERTURA, "<a"), LEX(SPATIA, " "),
        LEX(TAG_APERTURA, "<b"), LEX(TAG_FINIS, ">"), F } },
    { "<a\tb\n=\r\n'c'/>", { LEX(TAG_APERTURA, "<a"), LEX(SPATIA, "\t"),
        LEX(TITULUS, "b"), LEX(SPATIA, "\n"), LEX(AEQUALE, "="),
        LEX(SPATIA, "\r\n"), LEX(VALOR, "'c'"), LEX(TAG_FINIS_SOLUS,
            "/>"),
        F } },
    { "<a 1=x>", { LEX(TAG_APERTURA, "<a"), LEX(SPATIA, " "),
        LEX(IGNOTUM, "1"), LEX(AEQUALE, "="), LEX(TITULUS, "x"),
        LEX(TAG_FINIS, ">"), F } },
    { "</a >", { LEX(TAG_CLAUSURA, "</a"), LEX(SPATIA, " "),
        LEX(TAG_FINIS, ">"), F } },
    { "<!DOCTYPE a PUBLIC \"p\" 's'>", { LEX(DOCTYPE, "<!DOCTYPE"),
        LEX(SPATIA, " "), LEX(TITULUS, "a"), LEX(SPATIA, " "),
        LEX(TITULUS, "PUBLIC"), LEX(SPATIA, " "), LEX(VALOR, "\"p\""),
        LEX(SPATIA, " "), LEX(VALOR, "'s'"), LEX(TAG_FINIS, ">"), F } },
    { "<!DOCTYPE a [<!ELEMENT a (#PCDATA|b)*>]>",
        { LEX(DOCTYPE, "<!DOCTYPE"), LEX(SPATIA, " "), LEX(TITULUS,
            "a"),
          LEX(SPATIA, " "), LEX(PARS_INTERNA_APERTURA, "["),
          LEX(DECLARATIO, "<!ELEMENT"), LEX(SPATIA, " "), LEX(TITULUS,
              "a"),
          LEX(SPATIA, " "), LEX(DELIM, "("),
          LEX(VERBUM_PRAEDEFINITUM, "#PCDATA"), LEX(DELIM, "|"),
          LEX(TITULUS, "b"), LEX(DELIM, ")"), LEX(DELIM, "*"), P } },
    { "<!DOCTYPE a [<!ENTITY % p \"<x>\">%p;]>", {
        LEX(DOCTYPE, "<!DOCTYPE"), LEX(SPATIA, " "), LEX(TITULUS, "a"),
        LEX(SPATIA, " "), LEX(PARS_INTERNA_APERTURA, "["),
        LEX(DECLARATIO, "<!ENTITY"), LEX(SPATIA, " "), LEX(DELIM, "%"),
        LEX(SPATIA, " "), LEX(TITULUS, "p"), LEX(SPATIA, " "),
        LEX(VALOR, "\"<x>\""), LEX(TAG_FINIS, ">"),
        LEX(REFERENTIA_PARAMETRI, "%p;"), LEX(PARS_INTERNA_CLAUSURA,
            "]"),
        LEX(TAG_FINIS, ">"), F } },
    /* declaratio non clausa: '<' et ']' partem internam reddunt */
    { "<!DOCTYPE a [<!ELEMENT a ANY <!ATTLIST a b CDATA #IMPLIED]>", {
        LEX(DOCTYPE, "<!DOCTYPE"), LEX(SPATIA, " "), LEX(TITULUS, "a"),
        LEX(SPATIA, " "), LEX(PARS_INTERNA_APERTURA, "["),
        LEX(DECLARATIO, "<!ELEMENT"), LEX(SPATIA, " "), LEX(TITULUS,
            "a"),
        LEX(SPATIA, " "), LEX(TITULUS, "ANY"), LEX(SPATIA, " "),
        LEX(DECLARATIO, "<!ATTLIST"), LEX(SPATIA, " "), LEX(TITULUS,
            "a"),
        LEX(SPATIA, " "), LEX(TITULUS, "b"), P } },
    { "<!DOCTYPE a [<![INCLUDE[ ]]>]>", { LEX(DOCTYPE, "<!DOCTYPE"),
        LEX(SPATIA, " "), LEX(TITULUS, "a"), LEX(SPATIA, " "),
        LEX(PARS_INTERNA_APERTURA, "["), LEX(IGNOTUM, "<!["),
        LEX(IGNOTUM, "I"), LEX(IGNOTUM, "N"), LEX(IGNOTUM, "C"),
        LEX(IGNOTUM, "L"), LEX(IGNOTUM, "U"), LEX(IGNOTUM, "D"),
        LEX(IGNOTUM, "E"), LEX(IGNOTUM, "["), LEX(SPATIA, " "), P } },
    /* Nmtoken in declaratione */
    { "<!DOCTYPE a [<!ATTLIST a b (1|x.y) '1'>]>", {
        LEX(DOCTYPE, "<!DOCTYPE"), LEX(SPATIA, " "), LEX(TITULUS, "a"),
        LEX(SPATIA, " "), LEX(PARS_INTERNA_APERTURA, "["),
        LEX(DECLARATIO, "<!ATTLIST"), LEX(SPATIA, " "), LEX(TITULUS,
            "a"),
        LEX(SPATIA, " "), LEX(TITULUS, "b"), LEX(SPATIA, " "),
        LEX(DELIM, "("), LEX(TITULUS, "1"), LEX(DELIM, "|"), P } },
    { "<\xc3\xa9t\xc3\xa9/>", { LEX(TAG_APERTURA, "<\xc3\xa9t\xc3\xa9"),
        LEX(TAG_FINIS_SOLUS, "/>"), F } },
    { "<a \xff/>", { LEX(TAG_APERTURA, "<a"), LEX(SPATIA, " "),
        LEX(IGNOTUM, "\xff"), LEX(TAG_FINIS_SOLUS, "/>"), F } },
    { "", { F } }
};

interior b32
_aequat (
    constans MateriaToken* t,
                      s32  genus,
       constans character* textus)
{
    si (t->genus != genus)
    {
        redde FALSUM;
    }
    si (textus == NIHIL)
    {
        redde t->valor.mensura == ZEPHYRUM;
    }
    redde t->valor.mensura == (i32)strlen(textus)
        && memcmp(t->valor.datum, textus, (size_t)t->valor.mensura)
            == ZEPHYRUM;
}

interior vacuum
_transitus_probare (
    CredoContextus* c)
{
    i32 i;
    i32 n = (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));

    per (i = ZEPHYRUM; i < n; i++)
    {
        XmlLector lector;
              i32 k;

        CREDO_VERUM (xml_lector_incipere(&lector, c->piscina,
            CASUS[i].fons, (s32)strlen(CASUS[i].fons)));
        per (k = ZEPHYRUM; k < (i32)XXIV; k++)
        {
            constans LexemaExspectatum* e = &CASUS[i].lexemata[k];
                         MateriaToken*  t;
                                   s32  g = e->genus == FINIS_CASUS
                                       ? (s32)XML_LEX_FINIS : e->genus;

            si (e->genus == PRAEFIXUM_CASUS)
            {
                frange;
            }
            t = xml_lector_proximum(&lector);
            CREDO_NON_NIHIL (t);
            si (t == NIHIL)
            {
                frange;
            }
            si (!_aequat(t, g, e->textus))
            {
                imprimere("    casus %u lexema %u: genus %d '%.*s',"
                    " exspectatum %d '%s'\n", (insignatus integer)i,
                    (insignatus integer)k, (integer)t->genus,
                    (integer)t->valor.mensura,
                    (constans character*)t->valor.datum, (integer)g,
                    e->textus != NIHIL ? e->textus : "");
            }
            CREDO_VERUM (_aequat(t, g, e->textus));
            si (e->genus == FINIS_CASUS)
            {
                frange;
            }
        }
    }
}


/* ==================================================
 * TOTALITAS
 * ================================================== */

nomen structura {
    i32 plagulae;
    i32 fractae;
    i32 lexemata;
} StatusTotalitatis;

/* VERUM si lexemata textum totum tegunt et FINIS ultimum est */
interior b32
_totalis (
               Piscina* opus,
                chorda  textus,
    constans character* via,
                   i32* numerus)
{
    XmlLector  lector;
      Piscina* scrutinium;
          s32  cursor         = ZEPHYRUM;
          i32  linea          = I;
          s32  linea_initium  = ZEPHYRUM;
          i32  n              = ZEPHYRUM;

    /* lexemata non servantur: piscina scrutinii per MMMMXCVI lexemata
     * vacatur (plagula XXXV MB silvestris milliones gignit) */
    scrutinium = piscina_generare_dynamicum("xml_lector_scrutinium",
        (memoriae_index)(LXIV * MXXIV));
    si (   scrutinium == NIHIL || !xml_lector_incipere(&lector,
        scrutinium, (constans character*)textus.datum,
        (s32)textus.mensura))
    {
        redde FALSUM;
    }
    (vacuum)opus;
    dum (VERUM)
    {
        MateriaToken* t;
                  s32 j;

        si ((n & (i32)0xFFF) == ZEPHYRUM)
        {
            piscina_vacare(scrutinium);
        }
        t = xml_lector_proximum(&lector);
        si (t == NIHIL)
        {
            piscina_destruere(scrutinium);
            redde FALSUM;
        }
        n++;
        si (   t->byte_offset != cursor || t->linea != linea
            || t->columna     != (i32)(cursor - linea_initium) + I
            || (textus.mensura > ZEPHYRUM
                && (constans character*)t->valor.datum
                    != (constans character*)textus.datum + cursor))
        {
            imprimere("    %s: lexema %u ad %d (exspectatum %d,"
                " linea %u/%u)\n", via, (insignatus integer)n,
                (integer)t->byte_offset, (integer)cursor,
                (insignatus integer)t->linea,
                (insignatus integer)linea);
            piscina_destruere(scrutinium);
            redde FALSUM;
        }
        si (t->genus == (s32)XML_LEX_FINIS)
        {
            b32 bene = t->valor.mensura == ZEPHYRUM
                && cursor == (s32)textus.mensura;

            *numerus += n;
            piscina_destruere(scrutinium);
            redde bene;
        }
        si (t->valor.mensura == ZEPHYRUM)
        {
            imprimere("    %s: lexema vacuum ad %d\n", via,
                (integer)cursor);
            piscina_destruere(scrutinium);
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < (s32)t->valor.mensura; j++)
        {
            si (textus.datum[cursor + j] == '\n')
            {
                linea++;
                linea_initium = cursor + j + I;
            }
        }
        cursor += (s32)t->valor.mensura;
    }
}

interior vacuum
_visor (
                       vacuum* datum,
                XmlCorpusFons  fons,
           constans character* via,
    constans XmlCasusSelectus* selectus,
                       chorda  textus,
                      Piscina* opus)
{
    StatusTotalitatis* st = (StatusTotalitatis*)datum;

    (vacuum)fons;
    (vacuum)selectus;
    st->plagulae++;
    si (!_totalis(opus, textus, via, &st->lexemata))
    {
        st->fractae++;
    }
}

interior vacuum
_totalitatem_probare (
    CredoContextus* c)
{
    StatusTotalitatis  st;
      XmlCorpusNumeri  nn;
              Piscina* opus;
                  i32  i;

    memset(&st, ZEPHYRUM, magnitudo(st));
    opus = piscina_generare_dynamicum("xml_lector_opus",
        (memoriae_index)(I * MXXIV * MXXIV));
    CREDO_NON_NIHIL (opus);
    si (opus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; XML_CASUS[i] != NIHIL; i++)
    {
        chorda t;

        t = chorda_ex_literis(XML_CASUS[i], opus);
        _visor(&st, XML_CORPUS_XMLTS, XML_CASUS[i], NIHIL, t, opus);
        piscina_vacare(opus);
    }
    CREDO_AEQUALIS_I32 (st.fractae, ZEPHYRUM);
    imprimere("  casus inlinei: %u, fracti %u\n",
        (insignatus integer)st.plagulae,
        (insignatus integer)st.fractae);

    memset(&st, ZEPHYRUM, magnitudo(st));
    xml_corpus_ambulare(c->piscina, opus, xml_corpus_radix(),
        (i32)((I << XML_CORPUS_XMLTS) | (I << XML_CORPUS_SILVESTRIA)),
        _visor, &st, &nn);
    si (nn.xmlts_absens)
    {
        imprimere("  OMISSA: suite W3C abest (./xml/xmlts_arcessere.sh)"
            "\n");
    }
    imprimere("  xmlts %u, silvestria %u (omissae %u): lexemata %u,"
        " fractae %u\n",
        (insignatus integer)nn.plagulae[XML_CORPUS_XMLTS],
        (insignatus integer)nn.plagulae[XML_CORPUS_SILVESTRIA],
        (insignatus integer)nn.omissae, (insignatus integer)st.lexemata,
        (insignatus integer)st.fractae);
    CREDO_AEQUALIS_I32 (st.fractae, ZEPHYRUM);
    si (!nn.xmlts_absens)
    {
        CREDO_AEQUALIS_I32 (nn.plagulae[XML_CORPUS_XMLTS], (i32)MMI);
    }
    piscina_destruere(opus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "transitus modorum (tabula casuum)", _transitus_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "totalitas: casus, suite W3C, silvestria", _totalitatem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_lector", SECTIONES, NIHIL,
        NIHIL);
}
