/* probatio_xml.c - Probationes XML Parser et Scriptor */
#include "latina.h"
#include "xml.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Adiutor: comparare chorda* cum literis C */
interior b32
_chorda_ptr_eq_literis (
                chorda* ch,
    constans character* cstr)
{
    si (!ch)
    {
        redde FALSUM;
    }
    redde chorda_aequalis_literis(*ch, cstr);
}


/* ==================================================
 * Probare xml_elementum_creare
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* nodus;

        nodus = xml_elementum_creare(piscina, intern, "root");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_I32(nodus->genus, XML_NODUS_ELEMENTUM);
        CREDO_NON_NIHIL(nodus->titulus);
        CREDO_VERUM(_chorda_ptr_eq_literis(nodus->titulus, "root"));

        imprimere("  Elementum 'root' creatum: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_textum_creare
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* nodus;

        nodus = xml_textum_creare(piscina, intern, "Hello World");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_I32(nodus->genus, XML_NODUS_TEXTUS);
        CREDO_NON_NIHIL(nodus->valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(nodus->valor,
            "Hello World"));

        imprimere("  Textus 'Hello World' creatum: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_attributum_addere
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* nodus;
          chorda* valor;

        nodus = xml_elementum_creare(piscina, intern, "element");
        CREDO_NON_NIHIL(nodus);

        CREDO_VERUM(xml_attributum_addere(nodus, piscina, intern, "id",
            "123"));
        CREDO_VERUM(xml_attributum_addere(nodus, piscina, intern,
            "class", "main"));

        valor = xml_attributum_capere(nodus, "id");
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(valor, "123"));

        valor = xml_attributum_capere(nodus, "class");
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(valor, "main"));

        imprimere("  Attributa addita et obtenta: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_liberum_addere
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* parens;
        XmlNodus* liberum;

        parens   = xml_elementum_creare(piscina, intern, "parent");
        liberum  = xml_elementum_creare(piscina, intern, "child");

        CREDO_VERUM(xml_liberum_addere(parens, liberum));
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(parens), I);
        CREDO_AEQUALIS_PTR(xml_liberum_ad_indicem(parens, ZEPHYRUM),
            liberum);
        CREDO_AEQUALIS_PTR(liberum->parens, parens);

        imprimere("  Liberum additum: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_invenire_liberum
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* parens;
        XmlNodus* lib1;
        XmlNodus* lib2;
        XmlNodus* inventum;

        parens  = xml_elementum_creare(piscina, intern, "parent");
        lib1    = xml_elementum_creare(piscina, intern, "alpha");
        lib2    = xml_elementum_creare(piscina, intern, "beta");

        xml_liberum_addere(parens, lib1);
        xml_liberum_addere(parens, lib2);

        inventum = xml_invenire_liberum(parens, "alpha");
        CREDO_AEQUALIS_PTR(inventum, lib1);

        inventum = xml_invenire_liberum(parens, "beta");
        CREDO_AEQUALIS_PTR(inventum, lib2);

        inventum = xml_invenire_liberum(parens, "gamma");
        CREDO_NIHIL(inventum);

        imprimere("  Invenire liberum: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_scribere simplex
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* nodus;
          chorda  output;

        nodus = xml_elementum_creare(piscina, intern, "empty");

        output = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<empty/>");
    }
}


/* ==================================================
 * Probare xml_scribere cum attributis
 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* nodus;
          chorda  output;

        nodus = xml_elementum_creare(piscina, intern, "item");
        xml_attributum_addere(nodus, piscina, intern, "id", "42");

        output = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<item id=\"42\"/>");
    }
}


/* ==================================================
 * Probare xml_scribere cum liberis
 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* parens;
        XmlNodus* liberum;
          chorda  output;

        parens   = xml_elementum_creare(piscina, intern, "parent");
        liberum  = xml_elementum_creare(piscina, intern, "child");
        xml_liberum_addere(parens, liberum);

        output = xml_scribere(parens, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output,
            "<parent><child/></parent>");
    }
}


/* ==================================================
 * Probare xml_scribere cum textu
 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* nodus;
          chorda  output;

        nodus = xml_elementum_creare(piscina, intern, "greeting");
        xml_textum_addere(nodus, piscina, intern, "Hello");

        output = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output,
            "<greeting>Hello</greeting>");
    }
}


/* ==================================================
 * Probare xml_scribere effugium
 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* nodus;
          chorda  output;

        nodus = xml_elementum_creare(piscina, intern, "data");
        xml_textum_addere(nodus, piscina, intern, "<>&");

        output = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output,
            "<data>&lt;&gt;&amp;</data>");
    }
}


/* ==================================================
 * Probare xml_scribere pulchrum
 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlNodus* radix;
        XmlNodus* lib1;
        XmlNodus* lib2;
          chorda  output;

        radix  = xml_elementum_creare(piscina, intern, "root");
        lib1   = xml_elementum_creare(piscina, intern, "item");
        lib2   = xml_elementum_creare(piscina, intern, "item");

        xml_attributum_addere(lib1, piscina, intern, "id", "1");
        xml_attributum_addere(lib2, piscina, intern, "id", "2");

        xml_liberum_addere(radix, lib1);
        xml_liberum_addere(radix, lib2);

        output = xml_scribere(radix, piscina, VERUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output:\n%.*s", output.mensura, output.datum);
    }
}


/* ==================================================
 * Probare xml_legere simplex
 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus res;

        res = xml_legere_ex_literis("<root/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(res.radix->genus, XML_NODUS_ELEMENTUM);
        CREDO_VERUM(_chorda_ptr_eq_literis(res.radix->titulus, "root"));

        imprimere("  Parsatio '<root/>': VERUM\n");
    }
}


/* ==================================================
 * Probare xml_legere cum attributis
 * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus  res;
             chorda* val;

        res = xml_legere_ex_literis("<item id=\"42\" name=\"test\"/>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        val = xml_attributum_capere(res.radix, "id");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "42"));

        val = xml_attributum_capere(res.radix, "name");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "test"));

        imprimere("  Parsatio attributa: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_legere cum liberis
 * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus  res;
           XmlNodus* lib;

        res = xml_legere_ex_literis("<parent><child/></parent>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(res.radix), I);

        lib = xml_invenire_liberum(res.radix, "child");
        CREDO_NON_NIHIL(lib);

        imprimere("  Parsatio liberis: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_legere cum textu
 * ================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus res;
             chorda textus;

        res = xml_legere_ex_literis("<greeting>Hello World</greeting>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        textus = xml_textus_internus(res.radix, piscina);
        CREDO_CHORDA_NON_VACUA(textus);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "Hello World");

        imprimere("  Parsatio textus: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_legere entity references
 * ================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus res;
             chorda textus;

        res =
            xml_legere_ex_literis("<data>&lt;tag&gt; &amp; &quot;test&quot;</data>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        textus = xml_textus_internus(res.radix, piscina);
        CREDO_CHORDA_NON_VACUA(textus);

        imprimere("  Decoded: '%.*s'\n", textus.mensura, textus.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "<tag> & \"test\"");
    }
}


/* ==================================================
 * Probare xml_legere complexum
 * ================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        constans character* xml_input =
            "<entitas id=\"abc123\" genus=\"Pagina\">\n"
            "  <proprietas clavis=\"titulus\" valor=\"Domus\"/>\n"
            "  <proprietas clavis=\"descriptio\" valor=\"Main page\"/>\n"
            "  <nota>#activus</nota>\n"
            "</entitas>";

        XmlResultus  res;
           XmlNodus* prop;
           XmlNodus* nota;
             chorda* val;
             chorda  text;

        res = xml_legere_ex_literis(xml_input, piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Verificare radix */
        val = xml_attributum_capere(res.radix, "id");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "abc123"));

        val = xml_attributum_capere(res.radix, "genus");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "Pagina"));

        /* Verificare proprietates */
        prop = xml_invenire_liberum(res.radix, "proprietas");
        CREDO_NON_NIHIL(prop);
        val = xml_attributum_capere(prop, "clavis");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "titulus"));

        /* Verificare nota */
        nota = xml_invenire_liberum(res.radix, "nota");
        CREDO_NON_NIHIL(nota);
        text = xml_textus_internus(nota, piscina);
        CREDO_CHORDA_NON_VACUA(text);
        CREDO_CHORDA_AEQUALIS_LITERIS(text, "#activus");

        imprimere("  Parsatio complexa: VERUM\n");
    }
}


/* ==================================================
 * Probare roundtrip (parse -> serialize -> parse)
 * ================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        constans character* xml_original =
            "<root attr=\"value\"><child>text</child></root>";

        XmlResultus  res1;
             chorda  serialized;
        XmlResultus  res2;
             chorda* val1;
             chorda* val2;
             chorda  text1;
             chorda  text2;

        /* Parse original */
        res1 = xml_legere_ex_literis(xml_original, piscina, intern);
        CREDO_VERUM(res1.successus);

        /* Serialize */
        serialized = xml_scribere(res1.radix, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(serialized);

        imprimere("  Serialized: %.*s\n", serialized.mensura,
            serialized.datum);

        /* Parse again */
        res2 = xml_legere(serialized, piscina, intern);
        CREDO_VERUM(res2.successus);

        /* Compare */
        val1 = xml_attributum_capere(res1.radix, "attr");
        val2 = xml_attributum_capere(res2.radix, "attr");
        CREDO_NON_NIHIL(val1);
        CREDO_NON_NIHIL(val2);
        CREDO_VERUM(chorda_aequalis(*val1, *val2));

        text1 = xml_textus_internus(xml_invenire_liberum(res1.radix,
            "child"), piscina);
        text2 = xml_textus_internus(xml_invenire_liberum(res2.radix,
            "child"), piscina);
        CREDO_VERUM(chorda_aequalis(text1, text2));

        imprimere("  Roundtrip: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_legere errores
 * ================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus res;

        /* Empty input */
        res = xml_legere_ex_literis("", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32(res.status, XML_ERROR_VACUUM_INPUT);
        imprimere("  Empty input detectum: VERUM\n");

        /* Unclosed tag */
        res = xml_legere_ex_literis("<root>", piscina, intern);
        CREDO_FALSUM(res.successus);
        imprimere("  Unclosed tag detectum: VERUM\n");

        /* Mismatched tags */
        res = xml_legere_ex_literis("<root></other>", piscina, intern);
        CREDO_FALSUM(res.successus);
        imprimere("  Mismatched tags detectum: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_legere cum declaratione XML
 * ================================================== */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus res;

        res = xml_legere_ex_literis(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<root/>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_VERUM(_chorda_ptr_eq_literis(res.radix->titulus, "root"));

        imprimere("  Declaratio XML saltata: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_legere cum commentis
 * ================================================== */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus res;

        res = xml_legere_ex_literis(
            "<!-- This is a comment -->\n"
            "<root>\n"
            "  <!-- Inner comment -->\n"
            "  <child/>\n"
            "</root>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(res.radix), I);

        imprimere("  Commenta saltata: VERUM\n");
    }
}


/* ==================================================
 * Probare xml_invenire_omnes_liberos
 * ================================================== */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
        XmlResultus  res;
                Xar* items;

        res = xml_legere_ex_literis(
            "<list>"
            "<item id=\"1\"/>"
            "<other/>"
            "<item id=\"2\"/>"
            "<item id=\"3\"/>"
            "</list>",
            piscina, intern);
        CREDO_VERUM(res.successus);

        items = xml_invenire_omnes_liberos(res.radix, "item", piscina);
        CREDO_NON_NIHIL(items);
        CREDO_AEQUALIS_I32(xar_numerus(items), III);

        imprimere("  Invenire omnes 'item': %d (expected 3)\n",
            xar_numerus(items));
    }
}


/* ==================================================
 * Compendium
 * ================================================== */


/* ==================================================
 * Probare DOCTYPE in prologo (plists eam semper ferunt)
 * ================================================== */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
               XmlResultus  r;
        constans character* cum_doctype =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
            "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\"><dict/></plist>";
        constans character* cum_parte_interna =
            "<!DOCTYPE x [ <!ENTITY a \"b\"> ]>\n<x/>";
        constans character* per_lineas =
            "<!DOCTYPE plist PUBLIC\n"
            "  \"-//Apple//DTD PLIST 1.0//EN\"\n"
            "  \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\"><array/></plist>";

        r = xml_legere_ex_literis(cum_doctype, piscina, intern);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)XML_SUCCESSUS);
        CREDO_NON_NIHIL(r.radix);
        CREDO_CHORDA_AEQUALIS_LITERIS(*r.radix->titulus, "plist");

        /* declaratio per lineas plures extensa */
        r = xml_legere_ex_literis(per_lineas, piscina, intern);
        CREDO_VERUM(r.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(*r.radix->titulus, "plist");

        /* pars interna: ad '>' primum saltare intra eam resumeret
         * et corpus frangeret, ergo RECUSATUR nominatim */
        r = xml_legere_ex_literis(cum_parte_interna, piscina, intern);
        CREDO_FALSUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.status,
                           (i32)XML_ERROR_PARS_INTERNA);
    }
}

/* referentiae numericae (&#D; &#xH;) in textu et attributis, SEMEL
 * decodificatae: "&amp;#67;" textus litteralis "&#67;" manet
 * (recensio horrei I3) */
interior vacuum
_sectio_referentiae_numerorum (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
    InternamentumChorda* intern   = internamentum_creare(piscina);
            XmlResultus  res;

    res = xml_legere_ex_literis(
        "<d a=\"&#65;&#x42;&amp;#67;\">&#65;&#x42;&amp;#67;"
        "&#x1234;&#13;|&#;&#xZZ;&#1114112;&#65</d>", piscina, intern);
    CREDO_NECESSE(res.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(xml_textus_internus(res.radix,
        piscina),
        "AB&#67;\xE1\x88\xB4\r|&#;&#xZZ;&#1114112;&#65");
    CREDO_CHORDA_AEQUALIS_LITERIS(*xml_attributum_capere(res.radix,
        "a"),
        "AB&#67;");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "xml_elementum_creare", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_textum_creare", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_attributum_addere", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_liberum_addere", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_invenire_liberum", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_scribere simplex", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_scribere cum attributis", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "xml_scribere cum liberis", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_scribere cum textu", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_scribere effugium", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_scribere pulchrum", _sectio_decima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "xml_legere simplex", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_legere cum attributis", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "xml_legere cum liberis", _sectio_decima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "xml_legere cum textu", _sectio_decima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "xml_legere entity references", _sectio_decima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "xml_legere complexum", _sectio_decima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "roundtrip", _sectio_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "xml_legere errores", _sectio_decima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "xml_legere cum declaratione", _sectio_vicesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "xml_legere cum commentis", _sectio_vicesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "xml_invenire_omnes_liberos", _sectio_vicesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "DOCTYPE in prologo", _sectio_vicesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "referentiae_numerorum", _sectio_referentiae_numerorum, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("xml", SECTIONES);
}
