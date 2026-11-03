/* xml_corpus_ambulare.c - vide xml_corpus_ambulare.h */

#include "xml_corpus_ambulare.h"
#include "filum.h"
#include "sigillum.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA (IV * MXXIV)

/* lineam proximam ex textu [*k, ...) in 'linea' (NUL); FALSUM in
 * fine */
interior b32
_linea (
         chorda  textus,
            i32* k,
      character* linea,
            i32  capacitas)
{
    i32 n = ZEPHYRUM;

    si (*k >= textus.mensura)
    {
        redde FALSUM;
    }
    dum (*k < textus.mensura && textus.datum[*k] != '\n')
    {
        si (n + I < capacitas)
        {
            linea[n++] = (character)textus.datum[*k];
        }
        (*k)++;
    }
    (*k)++;
    linea[n] = '\0';
    redde VERUM;
}

/* campos TSV lineae in casum (linea in loco scinditur); FALSUM si
 * campi pauciores quam VII */
interior b32
_campi (
           character* linea,
    XmlCasusSelectus* selectus)
{
     constans character** campi[VII];
                    i32   i;
              character*  p = linea;

    campi[ZEPHYRUM]  = &selectus->id;
    campi[I]         = &selectus->genus;
    campi[II]        = &selectus->via;
    campi[III]       = &selectus->exitus;
    campi[IV]        = &selectus->entitates;
    campi[V]         = &selectus->spatia;
    campi[VI]        = &selectus->exclusio;
    per (i = ZEPHYRUM; i < (i32)VII; i++)
    {
        character* tab;

        *campi[i]  = p;
        tab        = strchr(p, '\t');
        si (i < (i32)VI)
        {
            si (tab == NIHIL)
            {
                redde FALSUM;
            }
            *tab  = '\0';
            p     = tab + I;
        }
    }
    redde VERUM;
}

constans character*
xml_corpus_radix (vacuum)
{
    constans character* radix = lectiones_ambitus("RHUBARB_RADIX");

    redde radix != NIHIL ? radix : ".";
}

chorda
xml_corpus_xmlts_legere (
                Piscina* piscina,
     constans character* radix,
     constans character* via)
{
    character plena[VIA_MAXIMA];

    sprintf(plena, "%s/xml/build/xmlts/xmlconf/%s", radix, via);
    redde filum_legere_totum(plena, piscina);
}

vacuum
xml_corpus_ambulare (
                Piscina* piscina,
                Piscina* opus,
     constans character* radix,
                    i32  fontes,
          XmlCorpusVisor visor,
                 vacuum* datum,
        XmlCorpusNumeri* numeri)
{
    character via[VIA_MAXIMA];
    character linea[VIA_MAXIMA];
       chorda index;
          i32 k;

    memset(numeri, ZEPHYRUM, magnitudo(*numeri));

    /* I. xmlts */
    sprintf(via, "%s/xml/build/xmlts/xmlconf/xmlconf.xml", radix);
    numeri->xmlts_absens = !filum_existit(via);
    si (   (fontes & (i32)(I << XML_CORPUS_XMLTS))
        && !numeri->xmlts_absens)
    {
        sprintf(via, "%s/xml/probationes/fixa/xmlts/selectio.tsv",
            radix);
        index  = filum_legere_totum(via, piscina);
        k      = ZEPHYRUM;
        dum (_linea(index, &k, linea, VIA_MAXIMA))
        {
            XmlCasusSelectus selectus;

            si (linea[ZEPHYRUM] == '#' || !_campi(linea, &selectus))
            {
                perge;
            }
            visor(datum, XML_CORPUS_XMLTS, selectus.via, &selectus,
                xml_corpus_xmlts_legere(opus, radix, selectus.via),
                opus);
            numeri->plagulae[XML_CORPUS_XMLTS]++;
            piscina_vacare(opus);
        }
    }

    /* II. silvestria (vivae, sigillo iudicatae) */
    si (!(fontes & (i32)(I << XML_CORPUS_SILVESTRIA)))
    {
        redde;
    }
    sprintf(via, "%s/xml/probationes/fixa/silvestria.manifestum",
        radix);
    index  = filum_legere_totum(via, piscina);
    k      = ZEPHYRUM;
    dum (_linea(index, &k, linea, VIA_MAXIMA))
    {
                  character* tab        = strchr(linea, '\t');
         constans character* domus_via  = lectiones_ambitus("HOME");
                     chorda  textus;
                   Sigillum  s;
                  character  hex[SIGILLUM_HEX_MENSURA];

        si (linea[ZEPHYRUM] == '#' || tab == NIHIL)
        {
            perge;
        }
        *tab = '\0';
        si (tab[I] == '~' && domus_via != NIHIL)
        {
            sprintf(via, "%s%s", domus_via, tab + II);
        }
        alioquin
        {
            sprintf(via, "%s", tab + I);
        }
        textus = filum_legere_totum(via, opus);
        s = sigillum_computare(textus.datum,
            (memoriae_index)textus.mensura);
        sigillum_hex(&s, hex);
        si (textus.datum == NIHIL || strcmp(hex, linea) != ZEPHYRUM)
        {
            numeri->omissae++;
            piscina_vacare(opus);
            perge;
        }
        visor(datum, XML_CORPUS_SILVESTRIA, via, NIHIL, textus, opus);
        numeri->plagulae[XML_CORPUS_SILVESTRIA]++;
        piscina_vacare(opus);
    }
}


/* Casus inlinei communes: Review Focus plani (P4-P5) et casus auditus
 * 2026-10-18 (parca ...4WRW, ...MVY9) */
constans character* constans XML_CASUS[] = {
    "", "<a/>", "<a></a>", "<a>x</a>", "<a b=\"c\" d='e'/>",
    "<?xml version=\"1.0\"?>\n<a/>\n",
    "\xef\xbb\xbf<?xml version=\"1.0\" encoding=\"UTF-8\"?><a/>",
    "<!DOCTYPE a>\n<a/>",
    "<!DOCTYPE a SYSTEM \"a.dtd\">\n<a/>",
    "<!DOCTYPE a PUBLIC \"-//x//y\" 'a.dtd' [\n"
        "  <!ELEMENT a (#PCDATA|b)*>\n"
        "  <!ATTLIST a x CDATA #IMPLIED y (p|q) 'p'>\n"
        "  <!ENTITY e \"<b/>\">\n"
        "  <!ENTITY % p SYSTEM 'p.ent'>\n"
        "  %p;\n"
        "  <!NOTATION n PUBLIC 'n'>\n"
        "  <!-- c --><?pi x?>\n"
        "]>\n<a>&e;</a>",
    "<a><!-- c --><?p d?><![CDATA[<x>&]]></a>",
    "<a>&amp;&#60;&#x3C;&e;</a>",
    "<a>\n  <b/>\n  <c>t</c>\n</a>\n",
    "<a> </a>", "<a>\n x <b/>\n y</a>\n",
    /* errata */
    "<a>]]></a>", "<a><!-- x -- y --></a>", "<a b=\"<\"/>",
    "<a b=\"x/>", "<a b='x' b='y'/>", "<a>&</a>", "<a>&#;</a>",
    "<a><b></a>", "<a></b>", "</a>", "<a>", "<a", "<a b", "<a b=",
    "<a/><b/>", "x<a/>", "<a/>x", " <?xml version='1.0'?><a/>",
    "<a><?xml version='1.0'?></a>", "<1a/>", "< a/>", "<a></ a>",
    "<a></a >", "<ab></a>", "<a></ab>", "<!-- unclosed",
    "<a><![CDATA[ unclosed", "<?pi unclosed", "<!DOCTYPE a [",
    "<!DOCTYPE a [ <!ELEMENT a ANY", "<!DOCTYPE a [ <![INCLUDE[ ]]> ]>",
    "<!DOCTYPE a [ %e; <!BOGUS> ]><a/>", "<!FOO><a/>",
    "<a b=\"x\"c=\"y\"/>", "<a\tb\n=\r\n'c'\n/>",
    "<a>\r\nx\r\n</a>\r\n", "<a>\rx\r</a>",
    "<a>\xff\xfe</a>", "<a>\x01</a>", "<a>\xc3</a>",
    "<e\xcc\x81t/>", "<\xc3\xa9t\xc3\xa9/>",
    /* STML fugare debet */
    "<a><![CDATA[</xml-cdata>]]></a>",
        "<a><!-- </xml-commentarium> --></a>",
    "<a b=\"</xml-valor>\"/>", "<?p </xml-instructio>?><a/>",
    NIHIL
};

i32
xml_casus_numerus (vacuum)
{
    i32 n = ZEPHYRUM;

    dum (XML_CASUS[n] != NIHIL)
    {
        n++;
    }
    redde n;
}
