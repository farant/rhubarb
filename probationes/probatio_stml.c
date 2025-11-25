/* probatio_stml.c - Probationes STML Parser et Scriptor */
#include "latina.h"
#include "stml.h"
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
    piscina = piscina_generare_dynamicum("probatio_stml", MMMMXCVI);
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
     * Probare stml_elementum_creare
     * ================================================== */

    imprimere("\n--- Probans stml_elementum_creare ---\n");

    {
        StmlNodus* nodus;

        nodus = stml_elementum_creare(piscina, intern, "root");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_I32(nodus->genus, STML_NODUS_ELEMENTUM);
        CREDO_NON_NIHIL(nodus->titulus);
        CREDO_VERUM(_chorda_ptr_eq_literis(nodus->titulus, "root"));

        imprimere("  Elementum 'root' creatum: VERUM\n");
    }

    /* ==================================================
     * Probare stml_textum_creare
     * ================================================== */

    imprimere("\n--- Probans stml_textum_creare ---\n");

    {
        StmlNodus* nodus;

        nodus = stml_textum_creare(piscina, intern, "Hello World");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_I32(nodus->genus, STML_NODUS_TEXTUS);
        CREDO_NON_NIHIL(nodus->valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(nodus->valor, "Hello World"));

        imprimere("  Textus 'Hello World' creatum: VERUM\n");
    }

    /* ==================================================
     * Probare stml_attributum_addere
     * ================================================== */

    imprimere("\n--- Probans stml_attributum_addere ---\n");

    {
        StmlNodus* nodus;
        chorda*    valor;

        nodus = stml_elementum_creare(piscina, intern, "element");
        CREDO_NON_NIHIL(nodus);

        CREDO_VERUM(stml_attributum_addere(nodus, piscina, intern, "id", "123"));
        CREDO_VERUM(stml_attributum_addere(nodus, piscina, intern, "class", "main"));

        valor = stml_attributum_capere(nodus, "id");
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(valor, "123"));

        valor = stml_attributum_capere(nodus, "class");
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(valor, "main"));

        imprimere("  Attributa addita et obtenta: VERUM\n");
    }

    /* ==================================================
     * Probare stml_attributum_boolean_addere
     * ================================================== */

    imprimere("\n--- Probans stml_attributum_boolean_addere ---\n");

    {
        StmlNodus* nodus;
        chorda*    valor;

        nodus = stml_elementum_creare(piscina, intern, "button");
        CREDO_NON_NIHIL(nodus);

        CREDO_VERUM(stml_attributum_boolean_addere(nodus, piscina, intern, "disabled"));

        valor = stml_attributum_capere(nodus, "disabled");
        CREDO_NON_NIHIL(valor);
        CREDO_VERUM(_chorda_ptr_eq_literis(valor, "true"));
        CREDO_VERUM(stml_attributum_habet(nodus, "disabled"));

        imprimere("  Boolean attributum 'disabled': VERUM\n");
    }

    /* ==================================================
     * Probare stml_liberum_addere
     * ================================================== */

    imprimere("\n--- Probans stml_liberum_addere ---\n");

    {
        StmlNodus* parens;
        StmlNodus* liberum;

        parens = stml_elementum_creare(piscina, intern, "parent");
        liberum = stml_elementum_creare(piscina, intern, "child");

        CREDO_VERUM(stml_liberum_addere(parens, liberum));
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(parens), I);
        CREDO_AEQUALIS_PTR(stml_liberum_ad_indicem(parens, ZEPHYRUM), liberum);
        CREDO_AEQUALIS_PTR(liberum->parens, parens);

        imprimere("  Liberum additum: VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere simplex
     * ================================================== */

    imprimere("\n--- Probans stml_legere simplex ---\n");

    {
        StmlResultus res;

        res = stml_legere_ex_literis("<root/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_AEQUALIS_I32(res.elementum_radix->genus, STML_NODUS_ELEMENTUM);
        CREDO_VERUM(_chorda_ptr_eq_literis(res.elementum_radix->titulus, "root"));

        imprimere("  Parsatio '<root/>': VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere cum attributis
     * ================================================== */

    imprimere("\n--- Probans stml_legere cum attributis ---\n");

    {
        StmlResultus res;
        chorda*      val;

        res = stml_legere_ex_literis("<item id=\"42\" name=\"test\"/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        val = stml_attributum_capere(res.elementum_radix, "id");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "42"));

        val = stml_attributum_capere(res.elementum_radix, "name");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "test"));

        imprimere("  Parsatio attributa: VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere boolean attributes
     * ================================================== */

    imprimere("\n--- Probans stml_legere boolean attributes ---\n");

    {
        StmlResultus res;

        res = stml_legere_ex_literis("<button disabled type=\"submit\"/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        CREDO_VERUM(stml_attributum_habet(res.elementum_radix, "disabled"));
        CREDO_VERUM(_chorda_ptr_eq_literis(
            stml_attributum_capere(res.elementum_radix, "disabled"), "true"));
        CREDO_VERUM(_chorda_ptr_eq_literis(
            stml_attributum_capere(res.elementum_radix, "type"), "submit"));

        imprimere("  Boolean attribute 'disabled': VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere cum liberis
     * ================================================== */

    imprimere("\n--- Probans stml_legere cum liberis ---\n");

    {
        StmlResultus res;
        StmlNodus*   lib;

        res = stml_legere_ex_literis("<parent><child/></parent>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(res.elementum_radix), I);

        lib = stml_invenire_liberum(res.elementum_radix, "child");
        CREDO_NON_NIHIL(lib);

        imprimere("  Parsatio liberis: VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere cum textu
     * ================================================== */

    imprimere("\n--- Probans stml_legere cum textu ---\n");

    {
        StmlResultus res;
        chorda       textus;

        res = stml_legere_ex_literis("<greeting>Hello World</greeting>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_NON_VACUA(textus);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "Hello World");

        imprimere("  Parsatio textus: VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere raw content tags (!)
     * ================================================== */

    imprimere("\n--- Probans stml_legere raw content ---\n");

    {
        StmlResultus res;
        chorda       textus;

        res = stml_legere_ex_literis("<code!><div>literal &amp;</div></code>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_VERUM(res.elementum_radix->crudus);

        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_NON_VACUA(textus);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "<div>literal &amp;</div>");

        imprimere("  Raw content '<code!>': VERUM\n");
    }

    /* ==================================================
     * Probare forward capture operators
     * ================================================== */

    imprimere("\n--- Probans forward capture operator ---\n");

    {
        StmlResultus res;
        StmlNodus*   wrapper;
        StmlNodus*   captured;

        res = stml_legere_ex_literis("<root><wrapper (><item/></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        /* After capture processing, item should be child of wrapper */
        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);
        CREDO_AEQUALIS_I32(wrapper->captio_directio, STML_CAPTIO_ANTE);

        captured = stml_invenire_liberum(wrapper, "item");
        CREDO_NON_NIHIL(captured);

        imprimere("  Forward capture <wrapper (>: VERUM\n");
    }

    /* ==================================================
     * Probare backward capture operators
     * ================================================== */

    imprimere("\n--- Probans backward capture operator ---\n");

    {
        StmlResultus res;
        StmlNodus*   wrapper;
        StmlNodus*   captured;

        res = stml_legere_ex_literis("<root><item/><) wrapper></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        /* After capture processing, item should be child of wrapper */
        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);
        CREDO_AEQUALIS_I32(wrapper->captio_directio, STML_CAPTIO_RETRO);

        captured = stml_invenire_liberum(wrapper, "item");
        CREDO_NON_NIHIL(captured);

        imprimere("  Backward capture <) wrapper>: VERUM\n");
    }

    /* ==================================================
     * Probare sandwich capture operators
     * ================================================== */

    imprimere("\n--- Probans sandwich capture operator ---\n");

    {
        StmlResultus res;
        StmlNodus*   wrapper;
        i32          num_children;

        res = stml_legere_ex_literis("<root><prev/><= wrapper =><next/></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        /* After capture processing, prev and next should be children of wrapper */
        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);
        CREDO_AEQUALIS_I32(wrapper->captio_directio, STML_CAPTIO_FARCIMEN);

        num_children = stml_numerus_liberorum(wrapper);
        CREDO_AEQUALIS_I32(num_children, II);

        CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "prev"));
        CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "next"));

        imprimere("  Sandwich capture <= wrapper =>: VERUM\n");
    }

    /* ==================================================
     * Probare stml_scribere simplex
     * ================================================== */

    imprimere("\n--- Probans stml_scribere simplex ---\n");

    {
        StmlNodus* nodus;
        chorda     output;

        nodus = stml_elementum_creare(piscina, intern, "empty");

        output = stml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<empty/>");
    }

    /* ==================================================
     * Probare stml_scribere cum attributis
     * ================================================== */

    imprimere("\n--- Probans stml_scribere cum attributis ---\n");

    {
        StmlNodus* nodus;
        chorda     output;

        nodus = stml_elementum_creare(piscina, intern, "item");
        stml_attributum_addere(nodus, piscina, intern, "id", "42");

        output = stml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<item id=\"42\"/>");
    }

    /* ==================================================
     * Probare stml_scribere boolean attributes
     * ================================================== */

    imprimere("\n--- Probans stml_scribere boolean attr ---\n");

    {
        StmlNodus* nodus;
        chorda     output;

        nodus = stml_elementum_creare(piscina, intern, "button");
        stml_attributum_boolean_addere(nodus, piscina, intern, "disabled");

        output = stml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        /* Boolean attributes should be serialized without ="true" */
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<button disabled/>");
    }

    /* ==================================================
     * Probare stml_scribere cum liberis
     * ================================================== */

    imprimere("\n--- Probans stml_scribere cum liberis ---\n");

    {
        StmlNodus* parens;
        StmlNodus* liberum;
        chorda     output;

        parens = stml_elementum_creare(piscina, intern, "parent");
        liberum = stml_elementum_creare(piscina, intern, "child");
        stml_liberum_addere(parens, liberum);

        output = stml_scribere(parens, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<parent><child/></parent>");
    }

    /* ==================================================
     * Probare stml_scribere cum textu
     * ================================================== */

    imprimere("\n--- Probans stml_scribere cum textu ---\n");

    {
        StmlNodus* nodus;
        chorda     output;

        nodus = stml_elementum_creare(piscina, intern, "greeting");
        stml_textum_addere(nodus, piscina, intern, "Hello");

        output = stml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<greeting>Hello</greeting>");
    }

    /* ==================================================
     * Probare stml_scribere raw content
     * ================================================== */

    imprimere("\n--- Probans stml_scribere raw content ---\n");

    {
        StmlNodus* nodus;
        StmlNodus* textus;
        chorda     output;

        nodus = stml_elementum_crudum_creare(piscina, intern, "code");
        textus = stml_textum_creare(piscina, intern, "<div>&amp;</div>");
        stml_liberum_addere(nodus, textus);

        output = stml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        imprimere("  Output: %.*s\n", output.mensura, output.datum);
        /* Raw content should NOT be escaped */
        CREDO_CHORDA_AEQUALIS_LITERIS(output, "<code!><div>&amp;</div></code>");
    }

    /* ==================================================
     * Probare roundtrip (parse -> serialize)
     * ================================================== */

    imprimere("\n--- Probans roundtrip ---\n");

    {
        constans character* stml_original =
            "<root attr=\"value\"><child>text</child></root>";

        StmlResultus res1;
        chorda       serialized;
        StmlResultus res2;
        chorda*      val1;
        chorda*      val2;
        chorda       text1;
        chorda       text2;

        /* Parse original */
        res1 = stml_legere_ex_literis(stml_original, piscina, intern);
        CREDO_VERUM(res1.successus);

        /* Serialize */
        serialized = stml_scribere(res1.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(serialized);

        imprimere("  Serialized: %.*s\n", serialized.mensura, serialized.datum);

        /* Parse again */
        res2 = stml_legere(serialized, piscina, intern);
        CREDO_VERUM(res2.successus);

        /* Compare */
        val1 = stml_attributum_capere(res1.elementum_radix, "attr");
        val2 = stml_attributum_capere(res2.elementum_radix, "attr");
        CREDO_NON_NIHIL(val1);
        CREDO_NON_NIHIL(val2);
        CREDO_VERUM(chorda_aequalis(*val1, *val2));

        text1 = stml_textus_internus(stml_invenire_liberum(res1.elementum_radix, "child"), piscina);
        text2 = stml_textus_internus(stml_invenire_liberum(res2.elementum_radix, "child"), piscina);
        CREDO_VERUM(chorda_aequalis(text1, text2));

        imprimere("  Roundtrip: VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere errores
     * ================================================== */

    imprimere("\n--- Probans stml_legere errores ---\n");

    {
        StmlResultus res;

        /* Empty input */
        res = stml_legere_ex_literis("", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32(res.status, STML_ERROR_VACUUM_INPUT);
        imprimere("  Empty input detectum: VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere cum commento
     * ================================================== */

    imprimere("\n--- Probans stml_legere cum commento ---\n");

    {
        StmlResultus res;

        res = stml_legere_ex_literis(
            "<!-- This is a comment -->\n"
            "<root>\n"
            "  <!-- Inner comment -->\n"
            "  <child/>\n"
            "</root>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        imprimere("  Commenta parsata: VERUM\n");
    }

    /* ==================================================
     * Probare stml_invenire_omnes_liberos
     * ================================================== */

    imprimere("\n--- Probans stml_invenire_omnes_liberos ---\n");

    {
        StmlResultus res;
        Xar*         items;

        res = stml_legere_ex_literis(
            "<list>"
            "<item id=\"1\"/>"
            "<other/>"
            "<item id=\"2\"/>"
            "<item id=\"3\"/>"
            "</list>",
            piscina, intern);
        CREDO_VERUM(res.successus);

        items = stml_invenire_omnes_liberos(res.elementum_radix, "item", piscina);
        CREDO_NON_NIHIL(items);
        CREDO_AEQUALIS_I32(xar_numerus(items), III);

        imprimere("  Invenire omnes 'item': %d (expected 3)\n", xar_numerus(items));
    }

    /* ==================================================
     * Probare multiple forward captures
     * ================================================== */

    imprimere("\n--- Probans multiple forward captures ---\n");

    {
        StmlResultus res;
        StmlNodus*   wrapper;
        i32          num_children;

        /* (( captures 2 siblings */
        res = stml_legere_ex_literis("<root><wrapper (( ><a/><b/><c/></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);
        CREDO_AEQUALIS_I32(wrapper->captio_numerus, II);

        /* Should have captured a and b, but not c */
        num_children = stml_numerus_liberorum(wrapper);
        CREDO_AEQUALIS_I32(num_children, II);

        CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "a"));
        CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "b"));

        /* c should be sibling of wrapper, not child */
        CREDO_NON_NIHIL(stml_invenire_liberum(res.elementum_radix, "c"));

        imprimere("  Multiple forward capture ((: VERUM\n");
    }

    /* ==================================================
     * Probare errores: malformed tags
     * ================================================== */

    imprimere("\n--- Probans errores malformed tags ---\n");

    {
        StmlResultus res;

        /* Mismatched tags */
        res = stml_legere_ex_literis("<div>content</span>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32(res.status, STML_ERROR_TAG_IMPROPRIE);
        imprimere("  Mismatched tags detectum: VERUM\n");
    }

    {
        StmlResultus res;

        /* Overlapping tags (invalid nesting) */
        res = stml_legere_ex_literis("<b><i>text</b></i>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32(res.status, STML_ERROR_TAG_IMPROPRIE);
        imprimere("  Overlapping tags detectum: VERUM\n");
    }

    {
        StmlResultus res;

        /* Unclosed tag at EOF */
        res = stml_legere_ex_literis("<div>unclosed content", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32(res.status, STML_ERROR_TAG_NON_CLAUSUM);
        imprimere("  Unclosed tag detectum: VERUM\n");
    }

    {
        StmlResultus res;

        /* Closing tag without opening */
        res = stml_legere_ex_literis("</div>", piscina, intern);
        CREDO_FALSUM(res.successus);
        imprimere("  Closing without opening detectum: VERUM\n");
    }

    {
        StmlResultus res;

        /* Multiple closing tags */
        res = stml_legere_ex_literis("<div>content</div></div>", piscina, intern);
        CREDO_FALSUM(res.successus);
        imprimere("  Multiple closing tags detectum: VERUM\n");
    }

    /* ==================================================
     * Probare errores: edge cases
     * ================================================== */

    imprimere("\n--- Probans errores edge cases ---\n");

    {
        StmlResultus res;

        /* Whitespace only - valid but no elements */
        res = stml_legere_ex_literis("   \n\t  ", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_NIHIL(res.elementum_radix);  /* No elements */
        imprimere("  Whitespace only detectum: VERUM\n");
    }

    {
        StmlResultus res;

        /* Only comment - should succeed */
        res = stml_legere_ex_literis("<!-- only comment -->", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        /* No root element */
        CREDO_NIHIL(res.elementum_radix);
        imprimere("  Only comment: VERUM\n");
    }

    {
        StmlResultus res;

        /* Multiple root elements - should succeed */
        res = stml_legere_ex_literis("<root1>a</root1><root2>b</root2>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        /* First element is rootElement */
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_VERUM(_chorda_ptr_eq_literis(res.elementum_radix->titulus, "root1"));
        imprimere("  Multiple roots: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   nodus;
        i32          depth;

        /* Deeply nested structure */
        res = stml_legere_ex_literis(
            "<a><b><c><d><e><f><g><h><i><j>"
            "deep"
            "</j></i></h></g></f></e></d></c></b></a>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        /* Count nesting depth */
        depth = ZEPHYRUM;
        nodus = res.elementum_radix;
        dum (nodus && stml_numerus_liberorum(nodus) > ZEPHYRUM)
        {
            nodus = stml_liberum_ad_indicem(nodus, ZEPHYRUM);
            si (nodus && nodus->genus == STML_NODUS_ELEMENTUM)
            {
                depth++;
            }
        }
        CREDO_AEQUALIS_I32(depth, IX);  /* 9 nested elements (b through j) */
        imprimere("  Deeply nested (%d levels): VERUM\n", depth + I);
    }

    {
        StmlResultus res;

        /* Many siblings */
        res = stml_legere_ex_literis(
            "<root>"
            "<i/><i/><i/><i/><i/><i/><i/><i/><i/><i/>"
            "<i/><i/><i/><i/><i/><i/><i/><i/><i/><i/>"
            "</root>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(res.elementum_radix), XX);
        imprimere("  Many siblings (20): VERUM\n");
    }

    /* ==================================================
     * Probare capture edge cases
     * ================================================== */

    imprimere("\n--- Probans capture edge cases ---\n");

    {
        StmlResultus res;
        StmlNodus*   outer;
        StmlNodus*   inner;

        /* Nested captures: <outer (> captures <inner /> */
        res = stml_legere_ex_literis("<root><outer (><inner/></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        outer = stml_invenire_liberum(res.elementum_radix, "outer");
        CREDO_NON_NIHIL(outer);

        /* Inner should be captured by outer */
        inner = stml_invenire_liberum(outer, "inner");
        CREDO_NON_NIHIL(inner);

        imprimere("  Nested captures: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   wrapper;

        /* Comments should be transparent to capture */
        res = stml_legere_ex_literis(
            "<root><!-- comment --><wrapper (><!-- comment2 --><item/></root>",
            piscina, intern);
        CREDO_VERUM(res.successus);

        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);

        /* Item should be captured (comments are transparent) */
        CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "item"));
        imprimere("  Comments transparent to capture: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   wrapper;
        chorda       textus;

        /* Text nodes with captures */
        res = stml_legere_ex_literis("<root><wrapper (>captured text</root>", piscina, intern);
        CREDO_VERUM(res.successus);

        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);

        textus = stml_textus_internus(wrapper, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "captured text");
        imprimere("  Text capture: VERUM\n");
    }

    /* ==================================================
     * Probare PI and DOCTYPE
     * ================================================== */

    imprimere("\n--- Probans PI et DOCTYPE ---\n");

    {
        StmlResultus res;

        /* Processing instruction */
        res = stml_legere_ex_literis("<?xml version=\"1.0\"?><root/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_VERUM(_chorda_ptr_eq_literis(res.elementum_radix->titulus, "root"));
        imprimere("  Processing instruction: VERUM\n");
    }

    {
        StmlResultus res;

        /* DOCTYPE */
        res = stml_legere_ex_literis("<!DOCTYPE html><html/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_VERUM(_chorda_ptr_eq_literis(res.elementum_radix->titulus, "html"));
        imprimere("  DOCTYPE: VERUM\n");
    }

    /* ==================================================
     * Probare raw content edge cases
     * ================================================== */

    imprimere("\n--- Probans raw content edge cases ---\n");

    {
        StmlResultus res;
        chorda       textus;

        /* Raw content preserves special chars */
        res = stml_legere_ex_literis("<code!>< > & \" '</code>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.elementum_radix->crudus);

        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "< > & \" '");
        imprimere("  Raw special chars: VERUM\n");
    }

    {
        StmlResultus res;
        chorda       textus;

        /* Raw content preserves nested tag-like content */
        res = stml_legere_ex_literis("<template!><div class=\"test\"><span>inner</span></div></template>", piscina, intern);
        CREDO_VERUM(res.successus);

        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "<div class=\"test\"><span>inner</span></div>");
        imprimere("  Raw nested tags: VERUM\n");
    }

    {
        StmlResultus res;

        /* Empty raw content */
        res = stml_legere_ex_literis("<code!></code>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.elementum_radix->crudus);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(res.elementum_radix), ZEPHYRUM);
        imprimere("  Empty raw content: VERUM\n");
    }

    /* ==================================================
     * Probare serialization edge cases
     * ================================================== */

    imprimere("\n--- Probans serialization edge cases ---\n");

    {
        StmlResultus res;
        chorda       output;

        /* Pretty print with indentation */
        res = stml_legere_ex_literis("<root><child><inner/></child></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        output = stml_scribere(res.elementum_radix, piscina, VERUM);
        CREDO_CHORDA_NON_VACUA(output);

        /* Should contain newlines and indentation */
        CREDO_VERUM(chorda_continet(output, chorda_ex_literis("\n", piscina)));
        imprimere("  Pretty print: VERUM\n");
    }

    {
        StmlNodus* nodus;
        chorda     output;

        /* Multiple attributes */
        nodus = stml_elementum_creare(piscina, intern, "el");
        stml_attributum_addere(nodus, piscina, intern, "a", "1");
        stml_attributum_addere(nodus, piscina, intern, "b", "2");
        stml_attributum_addere(nodus, piscina, intern, "c", "3");

        output = stml_scribere(nodus, piscina, FALSUM);
        CREDO_CHORDA_NON_VACUA(output);

        /* Should contain all attributes */
        CREDO_VERUM(chorda_continet(output, chorda_ex_literis("a=\"1\"", piscina)));
        CREDO_VERUM(chorda_continet(output, chorda_ex_literis("b=\"2\"", piscina)));
        CREDO_VERUM(chorda_continet(output, chorda_ex_literis("c=\"3\"", piscina)));
        imprimere("  Multiple attributes: VERUM\n");
    }

    /* ==================================================
     * Probare parser.test.ts equivalents
     * ================================================== */

    imprimere("\n--- Probans parser features ---\n");

    {
        StmlResultus res;
        StmlNodus*   p;
        i32          num_children;

        /* Mixed content: text + elements interleaved */
        res = stml_legere_ex_literis("<p>Text before <b>bold</b> text after</p>", piscina, intern);
        CREDO_VERUM(res.successus);

        p = res.elementum_radix;
        CREDO_NON_NIHIL(p);

        /* Should have 3 children: text, element, text */
        num_children = stml_numerus_liberorum(p);
        CREDO_AEQUALIS_I32(num_children, III);

        /* First child is text */
        CREDO_AEQUALIS_I32(stml_liberum_ad_indicem(p, ZEPHYRUM)->genus, STML_NODUS_TEXTUS);

        /* Second child is <b> element */
        CREDO_AEQUALIS_I32(stml_liberum_ad_indicem(p, I)->genus, STML_NODUS_ELEMENTUM);
        CREDO_VERUM(_chorda_ptr_eq_literis(stml_liberum_ad_indicem(p, I)->titulus, "b"));

        /* Third child is text */
        CREDO_AEQUALIS_I32(stml_liberum_ad_indicem(p, II)->genus, STML_NODUS_TEXTUS);

        imprimere("  Mixed content: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   root;
        chorda*      xmlns_attr;

        /* Namespace attributes */
        res = stml_legere_ex_literis(
            "<root xmlns:custom=\"http://example.com\"><custom:element>value</custom:element></root>",
            piscina, intern);
        CREDO_VERUM(res.successus);

        root = res.elementum_radix;
        CREDO_NON_NIHIL(root);

        /* Check namespace attribute */
        xmlns_attr = stml_attributum_capere(root, "xmlns:custom");
        CREDO_NON_NIHIL(xmlns_attr);
        CREDO_CHORDA_AEQUALIS_LITERIS(*xmlns_attr, "http://example.com");

        /* Check namespaced child element */
        CREDO_NON_NIHIL(stml_invenire_liberum(root, "custom:element"));

        imprimere("  Namespace attributes: VERUM\n");
    }

    {
        StmlResultus res;
        Xar*         items;

        /* Recursive getElementsByTagName equivalent */
        res = stml_legere_ex_literis(
            "<root><item>1</item><nested><item>2</item></nested></root>",
            piscina, intern);
        CREDO_VERUM(res.successus);

        si (res.successus)
        {
            /* stml_invenire_omnes_liberos only finds direct children */
            items = stml_invenire_omnes_liberos(res.elementum_radix, "item", piscina);
            CREDO_AEQUALIS_I32(xar_numerus(items), I);  /* Only 1 direct child */
        }

        imprimere("  Direct children search: VERUM\n");
    }

    /* ==================================================
     * Probare capture-operators.test.ts equivalents
     * ================================================== */

    imprimere("\n--- Probans capture operators (extended) ---\n");

    {
        StmlResultus res;
        StmlNodus*   div;
        chorda       textus;

        /* Text node capture: <div (>hello world */
        res = stml_legere_ex_literis("<root><div (>hello world</root>", piscina, intern);
        CREDO_VERUM(res.successus);

        div = stml_invenire_liberum(res.elementum_radix, "div");
        CREDO_NON_NIHIL(div);

        /* Text should be captured as child of div */
        textus = stml_textus_internus(div, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "hello world");

        imprimere("  Text node capture: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   div;

        /* Multiple parens capture multiple nodes: <div (((> captures 3 siblings */
        res = stml_legere_ex_literis(
            "<root><div (((><a/><b/><c/><d/></root>",
            piscina, intern);
        CREDO_VERUM(res.successus);

        div = stml_invenire_liberum(res.elementum_radix, "div");
        CREDO_NON_NIHIL(div);

        /* Should have captured a, b, c (3 nodes) but not d */
        CREDO_NON_NIHIL(stml_invenire_liberum(div, "a"));
        CREDO_NON_NIHIL(stml_invenire_liberum(div, "b"));
        CREDO_NON_NIHIL(stml_invenire_liberum(div, "c"));
        CREDO_NIHIL(stml_invenire_liberum(div, "d"));

        /* d should still be in root */
        CREDO_NON_NIHIL(stml_invenire_liberum(res.elementum_radix, "d"));

        imprimere("  Multiple parens capture: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   wrapper;
        chorda       textus;

        /* Backward capture of text: text content <) wrapper> */
        res = stml_legere_ex_literis("<root>some text<) wrapper></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);

        /* Text should be captured by wrapper */
        textus = stml_textus_internus(wrapper, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "some text");

        imprimere("  Backward text capture: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   wrapper;

        /* Multiple backward capture: <)) wrapper> captures 2 previous siblings */
        res = stml_legere_ex_literis("<root><a/><b/><c/><)) wrapper></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);

        /* Should have captured b and c (2 nodes) but not a */
        CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "b"));
        CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "c"));
        CREDO_NIHIL(stml_invenire_liberum(wrapper, "a"));

        /* a should still be in root */
        CREDO_NON_NIHIL(stml_invenire_liberum(res.elementum_radix, "a"));

        imprimere("  Multiple backward capture: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   forward;
        StmlNodus*   backward;

        /* Mixed operators: <forward (> <) backward> */
        res = stml_legere_ex_literis("<root><forward (><) backward></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        forward = stml_invenire_liberum(res.elementum_radix, "forward");
        CREDO_NON_NIHIL(forward);

        /* backward should be captured by forward */
        backward = stml_invenire_liberum(forward, "backward");
        CREDO_NON_NIHIL(backward);

        /* backward has no children (nothing before it to capture) */
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(backward), ZEPHYRUM);

        imprimere("  Mixed operators: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   wrapper;

        /* Simple sandwich with both siblings */
        res = stml_legere_ex_literis("<root><a/><= wrapper =><b/></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);

        si (wrapper)
        {
            /* wrapper should have captured a (before) and b (after) */
            CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "a"));
            CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "b"));
        }

        imprimere("  Sandwich with both siblings: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   wrapper;

        /* Self-closing tag capture */
        res = stml_legere_ex_literis("<root><wrapper (><img/></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        wrapper = stml_invenire_liberum(res.elementum_radix, "wrapper");
        CREDO_NON_NIHIL(wrapper);

        CREDO_NON_NIHIL(stml_invenire_liberum(wrapper, "img"));

        imprimere("  Self-closing capture: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   div;

        /* Not enough nodes for multiple capture - should capture what's available */
        res = stml_legere_ex_literis("<root><div (((><only/></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        div = stml_invenire_liberum(res.elementum_radix, "div");
        CREDO_NON_NIHIL(div);

        /* Should have captured the only available node */
        CREDO_NON_NIHIL(stml_invenire_liberum(div, "only"));

        imprimere("  Partial capture: VERUM\n");
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
