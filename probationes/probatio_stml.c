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
