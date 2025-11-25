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
_chorda_ptr_eq_literis(chorda* ch, constans character* cstr)
{
    si (!ch)
    {
        redde FALSUM;
    }
    redde chorda_aequalis_literis(*ch, cstr);
}

s32 principale(vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    b32                  praeteritus;

    /* Aperire piscinam et credonem */
    piscina = piscina_generare_dynamicum("probatio_xml", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        piscina_destruere(piscina);
        redde I;
    }

    credo_aperire(piscina);

    /* ==================================================
     * Probare xml_elementum_creare
     * ================================================== */

    imprimere("\n--- Probans xml_elementum_creare ---\n");

    {
        XmlNodus* nodus;

        nodus = xml_elementum_creare(piscina, intern, "root");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_I32(nodus->genus, XML_NODUS_ELEMENTUM);
        CREDO_NON_NIHIL(nodus->titulus);
        CREDO_VERUM(_chorda_ptr_eq_literis(nodus->titulus, "root"));

        imprimere("  Elementum 'root' creatum: VERUM\n");
    }

    /* ==================================================
     * Probare xml_textum_creare
     * ================================================== */

    imprimere("\n--- Probans xml_textum_creare ---\n");

    {
        XmlNodus* nodus;

        nodus = xml_textum_creare(piscina, intern, "Hello World");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_I32(nodus->genus, XML_NODUS_TEXTUS);
        CREDO_NON_NIHIL(nodus->valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(nodus->valor, "Hello World"));

        imprimere("  Textus 'Hello World' creatum: VERUM\n");
    }

    /* ==================================================
     * Probare xml_attributum_addere
     * ================================================== */

    imprimere("\n--- Probans xml_attributum_addere ---\n");

    {
        XmlNodus* nodus;
        chorda*   valor;

        nodus = xml_elementum_creare(piscina, intern, "element");
        CREDO_NON_NIHIL(nodus);

        CREDO_VERUM(xml_attributum_addere(nodus, piscina, intern, "id", "123"));
        CREDO_VERUM(xml_attributum_addere(nodus, piscina, intern, "class", "main"));

        valor = xml_attributum_capere(nodus, "id");
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(valor, "123"));

        valor = xml_attributum_capere(nodus, "class");
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(valor, "main"));

        imprimere("  Attributa addita et obtenta: VERUM\n");
    }

    /* ==================================================
     * Probare xml_liberum_addere
     * ================================================== */

    imprimere("\n--- Probans xml_liberum_addere ---\n");

    {
        XmlNodus* parens;
        XmlNodus* liberum;

        parens = xml_elementum_creare(piscina, intern, "parent");
        liberum = xml_elementum_creare(piscina, intern, "child");

        CREDO_VERUM(xml_liberum_addere(parens, liberum));
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(parens), I);
        CREDO_AEQUALIS_PTR(xml_liberum_ad_indicem(parens, ZEPHYRUM), liberum);
        CREDO_AEQUALIS_PTR(liberum->parens, parens);

        imprimere("  Liberum additum: VERUM\n");
    }

    /* ==================================================
     * Probare xml_invenire_liberum
     * ================================================== */

    imprimere("\n--- Probans xml_invenire_liberum ---\n");

    {
        XmlNodus* parens;
        XmlNodus* lib1;
        XmlNodus* lib2;
        XmlNodus* inventum;

        parens = xml_elementum_creare(piscina, intern, "parent");
        lib1 = xml_elementum_creare(piscina, intern, "alpha");
        lib2 = xml_elementum_creare(piscina, intern, "beta");

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

    /* ==================================================
     * Probare xml_scribere simplex
     * ================================================== */

    imprimere("\n--- Probans xml_scribere simplex ---\n");

    {
        XmlNodus* nodus;
        chorda    output;

        nodus = xml_elementum_creare(piscina, intern, "empty");

        output = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<empty/>");
    }

    /* ==================================================
     * Probare xml_scribere cum attributis
     * ================================================== */

    imprimere("\n--- Probans xml_scribere cum attributis ---\n");

    {
        XmlNodus* nodus;
        chorda    output;

        nodus = xml_elementum_creare(piscina, intern, "item");
        xml_attributum_addere(nodus, piscina, intern, "id", "42");

        output = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<item id=\"42\"/>");
    }

    /* ==================================================
     * Probare xml_scribere cum liberis
     * ================================================== */

    imprimere("\n--- Probans xml_scribere cum liberis ---\n");

    {
        XmlNodus* parens;
        XmlNodus* liberum;
        chorda    output;

        parens = xml_elementum_creare(piscina, intern, "parent");
        liberum = xml_elementum_creare(piscina, intern, "child");
        xml_liberum_addere(parens, liberum);

        output = xml_scribere(parens, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<parent><child/></parent>");
    }

    /* ==================================================
     * Probare xml_scribere cum textu
     * ================================================== */

    imprimere("\n--- Probans xml_scribere cum textu ---\n");

    {
        XmlNodus* nodus;
        chorda    output;

        nodus = xml_elementum_creare(piscina, intern, "greeting");
        xml_textum_addere(nodus, piscina, intern, "Hello");

        output = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<greeting>Hello</greeting>");
    }

    /* ==================================================
     * Probare xml_scribere effugium
     * ================================================== */

    imprimere("\n--- Probans xml_scribere effugium ---\n");

    {
        XmlNodus* nodus;
        chorda    output;

        nodus = xml_elementum_creare(piscina, intern, "data");
        xml_textum_addere(nodus, piscina, intern, "<>&");

        output = xml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<data>&lt;&gt;&amp;</data>");
    }

    /* ==================================================
     * Probare xml_scribere pulchrum
     * ================================================== */

    imprimere("\n--- Probans xml_scribere pulchrum ---\n");

    {
        XmlNodus* radix;
        XmlNodus* lib1;
        XmlNodus* lib2;
        chorda    output;

        radix = xml_elementum_creare(piscina, intern, "root");
        lib1 = xml_elementum_creare(piscina, intern, "item");
        lib2 = xml_elementum_creare(piscina, intern, "item");

        xml_attributum_addere(lib1, piscina, intern, "id", "1");
        xml_attributum_addere(lib2, piscina, intern, "id", "2");

        xml_liberum_addere(radix, lib1);
        xml_liberum_addere(radix, lib2);

        output = xml_scribere(radix, piscina, VERUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output:\n%.*s", output.mensura, output.datum);
    }

    /* ==================================================
     * Probare xml_legere simplex
     * ================================================== */

    imprimere("\n--- Probans xml_legere simplex ---\n");

    {
        XmlResultus res;

        res = xml_legere_ex_literis("<root/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(res.radix->genus, XML_NODUS_ELEMENTUM);
        CREDO_VERUM(_chorda_ptr_eq_literis(res.radix->titulus, "root"));

        imprimere("  Parsatio '<root/>': VERUM\n");
    }

    /* ==================================================
     * Probare xml_legere cum attributis
     * ================================================== */

    imprimere("\n--- Probans xml_legere cum attributis ---\n");

    {
        XmlResultus res;
        chorda*     val;

        res = xml_legere_ex_literis("<item id=\"42\" name=\"test\"/>", piscina, intern);
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

    /* ==================================================
     * Probare xml_legere cum liberis
     * ================================================== */

    imprimere("\n--- Probans xml_legere cum liberis ---\n");

    {
        XmlResultus res;
        XmlNodus*   lib;

        res = xml_legere_ex_literis("<parent><child/></parent>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32(xml_numerus_liberorum(res.radix), I);

        lib = xml_invenire_liberum(res.radix, "child");
        CREDO_NON_NIHIL(lib);

        imprimere("  Parsatio liberis: VERUM\n");
    }

    /* ==================================================
     * Probare xml_legere cum textu
     * ================================================== */

    imprimere("\n--- Probans xml_legere cum textu ---\n");

    {
        XmlResultus res;
        chorda      textus;

        res = xml_legere_ex_literis("<greeting>Hello World</greeting>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        textus = xml_textus_internus(res.radix, piscina);
        CREDO_CHORDA_NON_VACUA(textus);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "Hello World");

        imprimere("  Parsatio textus: VERUM\n");
    }

    /* ==================================================
     * Probare xml_legere entity references
     * ================================================== */

    imprimere("\n--- Probans xml_legere entity references ---\n");

    {
        XmlResultus res;
        chorda      textus;

        res = xml_legere_ex_literis("<data>&lt;tag&gt; &amp; &quot;test&quot;</data>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        textus = xml_textus_internus(res.radix, piscina);
        CREDO_CHORDA_NON_VACUA(textus);

        imprimere("  Decoded: '%.*s'\n", textus.mensura, textus.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "<tag> & \"test\"");
    }

    /* ==================================================
     * Probare xml_legere complexum
     * ================================================== */

    imprimere("\n--- Probans xml_legere complexum ---\n");

    {
        constans character* xml_input =
            "<entitas id=\"abc123\" genus=\"Pagina\">\n"
            "  <proprietas clavis=\"titulus\" valor=\"Domus\"/>\n"
            "  <proprietas clavis=\"descriptio\" valor=\"Main page\"/>\n"
            "  <nota>#activus</nota>\n"
            "</entitas>";

        XmlResultus res;
        XmlNodus*   prop;
        XmlNodus*   nota;
        chorda*     val;
        chorda      text;

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

    /* ==================================================
     * Probare roundtrip (parse -> serialize -> parse)
     * ================================================== */

    imprimere("\n--- Probans roundtrip ---\n");

    {
        constans character* xml_original =
            "<root attr=\"value\"><child>text</child></root>";

        XmlResultus res1;
        chorda      serialized;
        XmlResultus res2;
        chorda*     val1;
        chorda*     val2;
        chorda      text1;
        chorda      text2;

        /* Parse original */
        res1 = xml_legere_ex_literis(xml_original, piscina, intern);
        CREDO_VERUM(res1.successus);

        /* Serialize */
        serialized = xml_scribere(res1.radix, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(serialized);

        imprimere("  Serialized: %.*s\n", serialized.mensura, serialized.datum);

        /* Parse again */
        res2 = xml_legere(serialized, piscina, intern);
        CREDO_VERUM(res2.successus);

        /* Compare */
        val1 = xml_attributum_capere(res1.radix, "attr");
        val2 = xml_attributum_capere(res2.radix, "attr");
        CREDO_NON_NIHIL(val1);
        CREDO_NON_NIHIL(val2);
        CREDO_VERUM(chorda_aequalis(*val1, *val2));

        text1 = xml_textus_internus(xml_invenire_liberum(res1.radix, "child"), piscina);
        text2 = xml_textus_internus(xml_invenire_liberum(res2.radix, "child"), piscina);
        CREDO_VERUM(chorda_aequalis(text1, text2));

        imprimere("  Roundtrip: VERUM\n");
    }

    /* ==================================================
     * Probare xml_legere errores
     * ================================================== */

    imprimere("\n--- Probans xml_legere errores ---\n");

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

    /* ==================================================
     * Probare xml_legere cum declaratione XML
     * ================================================== */

    imprimere("\n--- Probans xml_legere cum declaratione ---\n");

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

    /* ==================================================
     * Probare xml_legere cum commentis
     * ================================================== */

    imprimere("\n--- Probans xml_legere cum commentis ---\n");

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

    /* ==================================================
     * Probare xml_invenire_omnes_liberos
     * ================================================== */

    imprimere("\n--- Probans xml_invenire_omnes_liberos ---\n");

    {
        XmlResultus res;
        Xar*        items;

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

        imprimere("  Invenire omnes 'item': %d (expected 3)\n", xar_numerus(items));
    }

    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Purificatio */
    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
