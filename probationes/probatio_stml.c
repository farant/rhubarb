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

/* Adiutor: formam pulchram probare ET stabilitatem per cyclos duos.
 *
 * Cyclus UNUS non sufficit. Vitium contenti mixti (2026-08-19) non
 * semel corrumpebat sed CUMULABAT: valor "n" fiebat "\nn\n" transitu
 * primo, deinde "\n\nn\n\n" secundo. Probatio unius cycli id "sanum"
 * videre potuisset. Ergo scriptio->lectio->scriptio idem octetim
 * reddere DEBET - haec assertio omni probationi pulchrae addenda est,
 * non his solis. */
interior vacuum
_pulchrum_probare(Piscina*             piscina,
                  InternamentumChorda* intern,
                  constans character*  input,
                  constans character*  exspectatum,
                  constans character*  titulus)
{
    StmlResultus primum;
    StmlResultus secundum;
    chorda       scriptum;
    chorda       rescriptum;

    primum = stml_legere_ex_literis(input, piscina, intern);
    CREDO_VERUM(primum.successus);

    scriptum = stml_scribere(primum.elementum_radix, piscina, VERUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(scriptum, exspectatum);

    secundum = stml_legere(scriptum, piscina, intern);
    CREDO_VERUM(secundum.successus);
    rescriptum = stml_scribere(secundum.elementum_radix, piscina,
                               VERUM);
    /* Contra EXSPECTATUM iterum, non contra scriptum: aequalitas
     * chordarum nudarum octetos in fractura non ostendit, et
     * (scriptum == exspectatum && rescriptum == exspectatum)
     * idem probat fortius */
    CREDO_CHORDA_AEQUALIS_LITERIS(rescriptum, exspectatum);

    imprimere("  %s\n", titulus);
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
     * Probare lineas nodorum (metadatum parsationis)
     * ================================================== */

    imprimere("\n--- Probans lineas nodorum ---\n");

    {
        StmlResultus res;
        StmlNodus*   radix_e;
        StmlNodus*   liberum;
        i32          i;
        i32          n;

        res = stml_legere_ex_literis(
            "<radix>\n"
            "  <primum/>\n"
            "  <alterum>\n"
            "    <intus/>\n"
            "  </alterum>\n"
            "</radix>\n", piscina, intern);
        CREDO_VERUM(res.successus);
        radix_e = res.elementum_radix;
        CREDO_AEQUALIS_I32(radix_e->linea, I);

        n = stml_numerus_liberorum(radix_e);
        per (i = ZEPHYRUM; i < n; i++)
        {
            liberum = stml_liberum_ad_indicem(radix_e, i);
            si (!liberum || liberum->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            si (_chorda_ptr_eq_literis(liberum->titulus, "primum"))
            {
                CREDO_AEQUALIS_I32(liberum->linea, II);
            }
            si (_chorda_ptr_eq_literis(liberum->titulus, "alterum"))
            {
                StmlNodus* intus;

                CREDO_AEQUALIS_I32(liberum->linea, III);
                intus = stml_invenire_liberum(liberum, "intus");
                CREDO_NON_NIHIL(intus);
                CREDO_AEQUALIS_I32(intus->linea, IV);
            }
        }

        /* nodi fabricati (non e parsatione): linea 0 */
        CREDO_AEQUALIS_I32(
            stml_elementum_creare(piscina, intern, "fabricatum")
                ->linea, ZEPHYRUM);

        imprimere("  Lineae nodorum: VERUM\n");
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
     * Probare stml_legere cum apostrophis (single quotes)
     * ================================================== */

    imprimere("\n--- Probans stml_legere cum apostrophis ---\n");

    {
        StmlResultus res;
        chorda*      val;

        res = stml_legere_ex_literis("<item id='42' name='test'/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        val = stml_attributum_capere(res.elementum_radix, "id");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "42"));

        val = stml_attributum_capere(res.elementum_radix, "name");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "test"));

        imprimere("  Parsatio cum apostrophis: VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere cum valoribus nudis (bare values)
     * ================================================== */

    imprimere("\n--- Probans stml_legere cum valoribus nudis ---\n");

    {
        StmlResultus res;
        chorda*      val;

        res = stml_legere_ex_literis("<pagina x=0 y=10 latitudo=71/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        val = stml_attributum_capere(res.elementum_radix, "x");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "0"));

        val = stml_attributum_capere(res.elementum_radix, "y");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "10"));

        val = stml_attributum_capere(res.elementum_radix, "latitudo");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "71"));

        imprimere("  Parsatio valoribus nudis: VERUM\n");
    }

    /* ==================================================
     * Probare stml_legere cum valoribus mixtis
     * ================================================== */

    imprimere("\n--- Probans stml_legere cum valoribus mixtis ---\n");

    {
        StmlResultus res;
        chorda*      val;

        res = stml_legere_ex_literis("<widget id='main' x=0 name=\"test\" active/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        val = stml_attributum_capere(res.elementum_radix, "id");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "main"));

        val = stml_attributum_capere(res.elementum_radix, "x");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "0"));

        val = stml_attributum_capere(res.elementum_radix, "name");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "test"));

        val = stml_attributum_capere(res.elementum_radix, "active");
        CREDO_NON_NIHIL(val);
        CREDO_VERUM(_chorda_ptr_eq_literis(val, "true"));

        imprimere("  Parsatio valoribus mixtis: VERUM\n");
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

    {
        StmlResultus res;
        chorda       textus;

        /* Raw content starting with TAB (uppercase word) */
        res = stml_legere_ex_literis("<pagina!>TAB = test</pagina>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.elementum_radix->crudus);

        textus = stml_textus_internus(res.elementum_radix, piscina);
        imprimere("  TAB raw content: [%.*s]\n", textus.mensura, textus.datum);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "TAB = test");
        imprimere("  Raw content with TAB word: VERUM\n");
    }

    /* ==================================================
     * Probare serialization edge cases
     * ================================================== */

    imprimere("\n--- Probans serialization edge cases ---\n");

    {
        StmlResultus res;
        chorda       output;

        /* Pretty print: spina unigena COLLABITUR (§4 T3c) - forma
         * capturae in linea una intra tectum columnarum */
        res = stml_legere_ex_literis("<root><child><inner/></child></root>", piscina, intern);
        CREDO_VERUM(res.successus);

        output = stml_scribere(res.elementum_radix, piscina, VERUM);
        CREDO_CHORDA_NON_VACUA(output);

        CREDO_CHORDA_AEQUALIS_LITERIS(output,
            "<root(> <child(> <inner/>");
        imprimere("  Pretty print (collapsus spinae): VERUM\n");
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
     * Probare smart whitespace trimming
     * ================================================== */

    imprimere("\n--- Probans smart whitespace trimming ---\n");

    {
        StmlResultus res;
        StmlNodus*   textus;

        /* Simple inline text - should just trim ends */
        res = stml_legere_ex_literis("<p>  hello world  </p>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_AEQUALIS_I32(xar_numerus(res.elementum_radix->liberi), I);

        textus = *(StmlNodus**)xar_obtinere(res.elementum_radix->liberi, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_AEQUALIS_I32(textus->genus, STML_NODUS_TEXTUS);
        /* PARSER SERVAT (2026-08-06): arbor documentum fideliter
         * refert - normalizatio ad lectionem migravit */
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "  hello world  "));
        CREDO_VERUM(chorda_aequalis_literis(
            stml_textus_normalizatus(res.elementum_radix, piscina),
            "hello world"));
        /* textus internus VERBATIM, ut textContent/string() */
        CREDO_VERUM(chorda_aequalis_literis(
            stml_textus_internus(res.elementum_radix, piscina),
            "  hello world  "));

        imprimere("  Simple trim (ad lectionem): VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   textus;

        /* Multiline with indentation - should normalize */
        res = stml_legere_ex_literis(
            "<pre>\n"
            "    line one\n"
            "    line two\n"
            "</pre>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_AEQUALIS_I32(xar_numerus(res.elementum_radix->liberi), I);

        textus = *(StmlNodus**)xar_obtinere(res.elementum_radix->liberi, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);

        /* normalizatio ad LECTIONEM: linea vacua initialis remota,
         * indentatio communis abscisa */
        CREDO_VERUM(chorda_aequalis_literis(
            stml_textus_normalizatus(res.elementum_radix, piscina),
            "line one\nline two"));
        /* EXEMPLAR TRIVIAE (spec §1.3): margines lineam-ferentes e
         * valore exeunt et ut trivia iuxta eum vivunt - internus
         * eos reassuit, ergo octeti fontis immoti manent */
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "line one\n    line two"));
        CREDO_NON_NIHIL(textus->spatia_ante);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->spatia_ante,
            "\n    "));
        CREDO_NON_NIHIL(textus->spatia_post);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->spatia_post,
            "\n"));
        CREDO_VERUM(chorda_aequalis_literis(
            stml_textus_internus(res.elementum_radix, piscina),
            "\n    line one\n    line two\n"));

        imprimere("  Multiline normalization (ad lectionem): VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   textus;

        /* Raw content should NOT be normalized */
        res = stml_legere_ex_literis(
            "<code!>\n"
            "    preserved indent\n"
            "</code>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_AEQUALIS_I32(xar_numerus(res.elementum_radix->liberi), I);

        textus = *(StmlNodus**)xar_obtinere(res.elementum_radix->liberi, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);

        /* Raw content preserves original whitespace */
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "\n    preserved indent\n"));

        imprimere("  Raw content preserved: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus*   textus;

        /* Relative indentation should be preserved */
        res = stml_legere_ex_literis(
            "<div>\n"
            "        outer\n"
            "            inner\n"
            "        outer again\n"
            "</div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        textus = *(StmlNodus**)xar_obtinere(res.elementum_radix->liberi, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);

        /* basis VIII spatiorum remota, 'inner' IV relativa servat */
        CREDO_VERUM(chorda_aequalis_literis(
            stml_textus_normalizatus(res.elementum_radix, piscina),
            "outer\n    inner\nouter again"));

        imprimere("  Relative indentation (ad lectionem): VERUM\n");
    }

    {
        StmlResultus res;
        chorda       scriptum;

        /* EXEMPLAR TRIVIAE (spec §1.3, supersedet 2026-08-06):
         * cursus totus albus lineam-ferens NULLUM nodum parit -
         * octeti in spatia_clausurae parentis vivunt et circuitus
         * eos verbatim reddit. Fratres non conglutinantur quia
         * spatium in dispositione servatur, non in nodo. */
        res = stml_legere_ex_literis("<root>   \n   \n   </root>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        CREDO_AEQUALIS_I32(xar_numerus(res.elementum_radix->liberi),
            ZEPHYRUM);
        CREDO_NON_NIHIL(res.elementum_radix->spatia_clausurae);
        CREDO_VERUM(_chorda_ptr_eq_literis(
            res.elementum_radix->spatia_clausurae, "   \n   \n   "));
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<root>   \n   \n   </root>");
        /* lectio normalizata: nihil (internus octetos clausurae
         * exacte reddit - "   \n   \n   " - quos normalizatio ad
         * nihil praecidit) */
        CREDO_AEQUALIS_I32(
            stml_textus_normalizatus(res.elementum_radix, piscina).mensura,
            ZEPHYRUM);

        imprimere("  Spatium album -> clausurae, circuitus exactus: VERUM\n");
    }

    /* ==================================================
     * Probare tituli (labels)
     * ================================================== */

    imprimere("\n--- Probans tituli (labels) ---\n");

    {
        StmlNodus* nodus;

        /* Create element with labels attribute */
        nodus = stml_elementum_creare(piscina, intern, "div");
        CREDO_NON_NIHIL(nodus);

        stml_attributum_addere(nodus, piscina, intern, "labels", "foo bar baz");

        /* Test stml_titulum_habet */
        CREDO_VERUM(stml_titulum_habet(nodus, "foo"));
        CREDO_VERUM(stml_titulum_habet(nodus, "bar"));
        CREDO_VERUM(stml_titulum_habet(nodus, "baz"));
        CREDO_FALSUM(stml_titulum_habet(nodus, "qux"));
        CREDO_FALSUM(stml_titulum_habet(nodus, "fo"));   /* Partial match */
        CREDO_FALSUM(stml_titulum_habet(nodus, "foobar"));

        imprimere("  stml_titulum_habet: VERUM\n");
    }

    {
        StmlNodus* nodus;

        /* Test stml_titulos_numerus */
        nodus = stml_elementum_creare(piscina, intern, "div");
        CREDO_NON_NIHIL(nodus);

        stml_attributum_addere(nodus, piscina, intern, "labels", "one two three");

        CREDO_AEQUALIS_I32((i32)stml_titulos_numerus(nodus), III);

        /* Empty labels */
        nodus = stml_elementum_creare(piscina, intern, "span");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_I32((i32)stml_titulos_numerus(nodus), ZEPHYRUM);

        imprimere("  stml_titulos_numerus: VERUM\n");
    }

    {
        StmlNodus* nodus;
        Xar* tituli;
        chorda* label;

        /* Test stml_titulos_capere */
        nodus = stml_elementum_creare(piscina, intern, "div");
        CREDO_NON_NIHIL(nodus);

        stml_attributum_addere(nodus, piscina, intern, "labels", "alpha beta gamma");

        tituli = stml_titulos_capere(nodus, piscina);
        CREDO_NON_NIHIL(tituli);
        CREDO_AEQUALIS_I32(xar_numerus(tituli), III);

        label = (chorda*)xar_obtinere(tituli, ZEPHYRUM);
        CREDO_NON_NIHIL(label);
        CREDO_VERUM(chorda_aequalis_literis(*label, "alpha"));

        label = (chorda*)xar_obtinere(tituli, I);
        CREDO_NON_NIHIL(label);
        CREDO_VERUM(chorda_aequalis_literis(*label, "beta"));

        label = (chorda*)xar_obtinere(tituli, II);
        CREDO_NON_NIHIL(label);
        CREDO_VERUM(chorda_aequalis_literis(*label, "gamma"));

        imprimere("  stml_titulos_capere: VERUM\n");
    }

    {
        StmlNodus* nodus;

        /* Test stml_titulum_addere */
        nodus = stml_elementum_creare(piscina, intern, "div");
        CREDO_NON_NIHIL(nodus);

        /* Add first label */
        CREDO_VERUM(stml_titulum_addere(nodus, piscina, intern, "first"));
        CREDO_VERUM(stml_titulum_habet(nodus, "first"));

        /* Add second label */
        CREDO_VERUM(stml_titulum_addere(nodus, piscina, intern, "second"));
        CREDO_VERUM(stml_titulum_habet(nodus, "first"));
        CREDO_VERUM(stml_titulum_habet(nodus, "second"));

        /* Adding duplicate returns FALSUM */
        CREDO_FALSUM(stml_titulum_addere(nodus, piscina, intern, "first"));

        CREDO_AEQUALIS_I32((i32)stml_titulos_numerus(nodus), II);

        imprimere("  stml_titulum_addere: VERUM\n");
    }

    {
        StmlNodus* nodus;

        /* Test stml_titulum_removere */
        nodus = stml_elementum_creare(piscina, intern, "div");
        CREDO_NON_NIHIL(nodus);

        stml_attributum_addere(nodus, piscina, intern, "labels", "keep remove also");

        /* Remove middle label */
        CREDO_VERUM(stml_titulum_removere(nodus, piscina, intern, "remove"));
        CREDO_FALSUM(stml_titulum_habet(nodus, "remove"));
        CREDO_VERUM(stml_titulum_habet(nodus, "keep"));
        CREDO_VERUM(stml_titulum_habet(nodus, "also"));

        /* Removing non-existent returns FALSUM */
        CREDO_FALSUM(stml_titulum_removere(nodus, piscina, intern, "nothere"));

        CREDO_AEQUALIS_I32((i32)stml_titulos_numerus(nodus), II);

        imprimere("  stml_titulum_removere: VERUM\n");
    }

    {
        StmlNodus* nodus;

        /* Test stml_titulum_commutare */
        nodus = stml_elementum_creare(piscina, intern, "div");
        CREDO_NON_NIHIL(nodus);

        /* Toggle on (returns VERUM = now has it) */
        CREDO_VERUM(stml_titulum_commutare(nodus, piscina, intern, "toggle"));
        CREDO_VERUM(stml_titulum_habet(nodus, "toggle"));

        /* Toggle off (returns FALSUM = now doesn't have it) */
        CREDO_FALSUM(stml_titulum_commutare(nodus, piscina, intern, "toggle"));
        CREDO_FALSUM(stml_titulum_habet(nodus, "toggle"));

        /* Toggle back on */
        CREDO_VERUM(stml_titulum_commutare(nodus, piscina, intern, "toggle"));
        CREDO_VERUM(stml_titulum_habet(nodus, "toggle"));

        imprimere("  stml_titulum_commutare: VERUM\n");
    }

    {
        StmlResultus res;

        /* Test labels from parsed content */
        res = stml_legere_ex_literis(
            "<div labels=\"parsed one two\"/>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        CREDO_VERUM(stml_titulum_habet(res.elementum_radix, "parsed"));
        CREDO_VERUM(stml_titulum_habet(res.elementum_radix, "one"));
        CREDO_VERUM(stml_titulum_habet(res.elementum_radix, "two"));
        CREDO_AEQUALIS_I32((i32)stml_titulos_numerus(res.elementum_radix), III);

        imprimere("  Parsed labels: VERUM\n");
    }

    {
        StmlResultus res;

        /* Test class attribute (HTML compat) */
        res = stml_legere_ex_literis(
            "<div class=\"html style classes\"/>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        CREDO_VERUM(stml_titulum_habet(res.elementum_radix, "html"));
        CREDO_VERUM(stml_titulum_habet(res.elementum_radix, "style"));
        CREDO_VERUM(stml_titulum_habet(res.elementum_radix, "classes"));

        imprimere("  HTML class compat: VERUM\n");
    }

    /* ==================================================
     * Probare Navigatio (Traversal)
     * ================================================== */

    imprimere("\n--- Probans navigatio ---\n");

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* child1;
        StmlNodus* child2;
        StmlNodus* child3;

        /* Tree: <div><a/><b/><c/></div> */
        res = stml_legere_ex_literis(
            "<div><a/><b/><c/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        div = res.elementum_radix;
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), III);

        child1 = stml_liberum_ad_indicem(div, ZEPHYRUM);
        child2 = stml_liberum_ad_indicem(div, I);
        child3 = stml_liberum_ad_indicem(div, II);

        CREDO_NON_NIHIL(child1);
        CREDO_NON_NIHIL(child2);
        CREDO_NON_NIHIL(child3);

        /* Test stml_index_inter_fratres */
        CREDO_AEQUALIS_S32(stml_index_inter_fratres(child1), ZEPHYRUM);
        CREDO_AEQUALIS_S32(stml_index_inter_fratres(child2), I);
        CREDO_AEQUALIS_S32(stml_index_inter_fratres(child3), II);
        CREDO_AEQUALIS_S32(stml_index_inter_fratres(div), ZEPHYRUM);  /* div est primus liberum documenti */
        CREDO_AEQUALIS_S32(stml_index_inter_fratres(res.radix), -I);  /* documentum non habet parentem */

        imprimere("  stml_index_inter_fratres: VERUM\n");

        /* Test stml_frater_proximus */
        CREDO_VERUM(stml_frater_proximus(child1) == child2);
        CREDO_VERUM(stml_frater_proximus(child2) == child3);
        CREDO_NIHIL(stml_frater_proximus(child3));  /* ultimus non habet proximum */

        imprimere("  stml_frater_proximus: VERUM\n");

        /* Test stml_frater_prior */
        CREDO_NIHIL(stml_frater_prior(child1));  /* primus non habet priorem */
        CREDO_VERUM(stml_frater_prior(child2) == child1);
        CREDO_VERUM(stml_frater_prior(child3) == child2);

        imprimere("  stml_frater_prior: VERUM\n");

        /* Test stml_primus_liberum / stml_ultimus_liberum */
        CREDO_VERUM(stml_primus_liberum(div) == child1);
        CREDO_VERUM(stml_ultimus_liberum(div) == child3);
        CREDO_NIHIL(stml_primus_liberum(child1));  /* <a/> vacuum est */
        CREDO_NIHIL(stml_ultimus_liberum(child1));

        imprimere("  stml_primus_liberum/ultimus_liberum: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* child2;
        Xar* fratres;

        /* Test stml_fratres */
        res = stml_legere_ex_literis(
            "<div><a/><b/><c/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        child2 = stml_liberum_ad_indicem(div, I);  /* <b/> */

        fratres = stml_fratres(child2, piscina);
        CREDO_NON_NIHIL(fratres);
        CREDO_AEQUALIS_I32(xar_numerus(fratres), II);  /* <a/> et <c/> */

        imprimere("  stml_fratres: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* inner;
        StmlNodus* deep;
        Xar* maiores;

        /* Test stml_maiores: <div><inner><deep/></inner></div> */
        res = stml_legere_ex_literis(
            "<div><inner><deep/></inner></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        inner = stml_primus_liberum(div);
        CREDO_NON_NIHIL(inner);
        deep = stml_primus_liberum(inner);
        CREDO_NON_NIHIL(deep);

        maiores = stml_maiores(deep, piscina);
        CREDO_NON_NIHIL(maiores);
        CREDO_AEQUALIS_I32(xar_numerus(maiores), III);  /* inner, div, documentum */

        /* Verificare ordo: primus est parens directus */
        CREDO_VERUM(*((StmlNodus**)xar_obtinere(maiores, ZEPHYRUM)) == inner);
        CREDO_VERUM(*((StmlNodus**)xar_obtinere(maiores, I)) == div);

        imprimere("  stml_maiores: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* inner;
        StmlNodus* deep;
        StmlNodus* found;

        /* Test stml_proximus_maior */
        res = stml_legere_ex_literis(
            "<div><inner><deep/></inner></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        inner = stml_primus_liberum(div);
        deep = stml_primus_liberum(inner);

        /* Invenire "div" ex deep */
        found = stml_proximus_maior(deep, "div", piscina, intern);
        CREDO_VERUM(found == div);

        /* Invenire "inner" ex deep */
        found = stml_proximus_maior(deep, "inner", piscina, intern);
        CREDO_VERUM(found == inner);

        /* Invenire "deep" ex deep (includit se) */
        found = stml_proximus_maior(deep, "deep", piscina, intern);
        CREDO_VERUM(found == deep);

        /* Non invenire "nonexistent" */
        found = stml_proximus_maior(deep, "nonexistent", piscina, intern);
        CREDO_NIHIL(found);

        imprimere("  stml_proximus_maior: VERUM\n");
    }

    /* ==================================================
     * Probare Mutatio (Mutation)
     * ================================================== */

    imprimere("\n--- Probans mutatio ---\n");

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* novum;

        /* Test stml_praeponere */
        res = stml_legere_ex_literis(
            "<div><a/><b/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), II);

        novum = stml_elementum_creare(piscina, intern, "first");
        CREDO_NON_NIHIL(novum);

        CREDO_VERUM(stml_praeponere(div, novum, piscina));
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), III);
        CREDO_VERUM(stml_primus_liberum(div) == novum);
        CREDO_VERUM(novum->parens == div);

        imprimere("  stml_praeponere: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* child2;

        /* Test stml_removere */
        res = stml_legere_ex_literis(
            "<div><a/><b/><c/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), III);

        child2 = stml_liberum_ad_indicem(div, I);  /* <b/> */
        CREDO_NON_NIHIL(child2);

        CREDO_VERUM(stml_removere(child2, piscina));
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), II);
        CREDO_NIHIL(child2->parens);

        /* Nunc child at index 1 should be <c/> */
        CREDO_VERUM(_chorda_ptr_eq_literis(
            stml_liberum_ad_indicem(div, I)->titulus, "c"));

        imprimere("  stml_removere: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* child1;
        StmlNodus* child2;

        /* Test stml_vacare_liberos */
        res = stml_legere_ex_literis(
            "<div><a/><b/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        child1 = stml_liberum_ad_indicem(div, ZEPHYRUM);
        child2 = stml_liberum_ad_indicem(div, I);

        stml_vacare_liberos(div);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), ZEPHYRUM);
        CREDO_NIHIL(child1->parens);
        CREDO_NIHIL(child2->parens);

        imprimere("  stml_vacare_liberos: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* child1;
        StmlNodus* novum;

        /* Test stml_inserere_ante */
        res = stml_legere_ex_literis(
            "<div><a/><b/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        child1 = stml_liberum_ad_indicem(div, ZEPHYRUM);  /* <a/> */

        novum = stml_elementum_creare(piscina, intern, "before-a");
        CREDO_VERUM(stml_inserere_ante(child1, novum, piscina));

        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), III);
        CREDO_VERUM(stml_primus_liberum(div) == novum);
        CREDO_VERUM(stml_frater_proximus(novum) == child1);

        imprimere("  stml_inserere_ante: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* child1;
        StmlNodus* novum;

        /* Test stml_inserere_post */
        res = stml_legere_ex_literis(
            "<div><a/><b/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        child1 = stml_liberum_ad_indicem(div, ZEPHYRUM);  /* <a/> */

        novum = stml_elementum_creare(piscina, intern, "after-a");
        CREDO_VERUM(stml_inserere_post(child1, novum, piscina));

        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), III);
        CREDO_VERUM(stml_frater_proximus(child1) == novum);
        CREDO_VERUM(_chorda_ptr_eq_literis(
            stml_liberum_ad_indicem(div, I)->titulus, "after-a"));

        imprimere("  stml_inserere_post: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* child1;
        StmlNodus* novum;

        /* Test stml_substituere */
        res = stml_legere_ex_literis(
            "<div><a/><b/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;
        child1 = stml_liberum_ad_indicem(div, ZEPHYRUM);  /* <a/> */

        novum = stml_elementum_creare(piscina, intern, "replaced");
        CREDO_VERUM(stml_substituere(child1, novum, piscina));

        CREDO_AEQUALIS_I32(stml_numerus_liberorum(div), II);
        CREDO_NIHIL(child1->parens);
        CREDO_VERUM(novum->parens == div);
        CREDO_VERUM(_chorda_ptr_eq_literis(
            stml_primus_liberum(div)->titulus, "replaced"));

        imprimere("  stml_substituere: VERUM\n");
    }

    /* ==================================================
     * Probare Duplicatio (Cloning)
     * ================================================== */

    imprimere("\n--- Probans duplicatio ---\n");

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* clone;

        /* Test stml_duplicare (deep) */
        res = stml_legere_ex_literis(
            "<div id=\"orig\"><a/><b/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;

        clone = stml_duplicare(div, piscina, intern);
        CREDO_NON_NIHIL(clone);

        /* Clone should have same structure */
        CREDO_VERUM(_chorda_ptr_eq_literis(clone->titulus, "div"));
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(clone), II);
        CREDO_NIHIL(clone->parens);  /* Clone non habet parentem */

        /* Clone children should have clone as parent */
        CREDO_VERUM(stml_primus_liberum(clone)->parens == clone);

        /* Clone is independent */
        CREDO_VERUM(clone != div);
        CREDO_VERUM(stml_primus_liberum(clone) != stml_primus_liberum(div));

        /* Clone should have same attributes */
        CREDO_VERUM(_chorda_ptr_eq_literis(
            stml_attributum_capere(clone, "id"), "orig"));

        imprimere("  stml_duplicare (profunde): VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* div;
        StmlNodus* shallow;

        /* Test stml_duplicare_superficialiter */
        res = stml_legere_ex_literis(
            "<div id=\"orig\"><a/><b/></div>",
            piscina, intern);

        CREDO_VERUM(res.successus);
        div = res.elementum_radix;

        shallow = stml_duplicare_superficialiter(div, piscina, intern);
        CREDO_NON_NIHIL(shallow);

        /* Shallow clone has same tag and attributes but NO children */
        CREDO_VERUM(_chorda_ptr_eq_literis(shallow->titulus, "div"));
        CREDO_NIHIL(shallow->liberi);
        CREDO_VERUM(_chorda_ptr_eq_literis(
            stml_attributum_capere(shallow, "id"), "orig"));

        imprimere("  stml_duplicare_superficialiter: VERUM\n");
    }

    /* ==================================================
     * Probare Entity Unescaping
     * ================================================== */

    imprimere("\n--- Probans Entity Unescaping ---\n");

    {
        StmlResultus res;
        StmlNodus* p;
        chorda textus;

        /* Basic entity unescaping: &lt; and &gt; */
        res = stml_legere_ex_literis("<p>&lt;test&gt;</p>", piscina, intern);
        CREDO_VERUM(res.successus);
        p = res.elementum_radix;
        CREDO_NON_NIHIL(p);
        textus = stml_textus_internus(p, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "<test>");

        imprimere("  &lt; &gt; unescaping: VERUM\n");
    }

    {
        StmlResultus res;
        chorda textus;

        /* Ampersand unescaping: &amp; */
        res = stml_legere_ex_literis("<p>A &amp; B</p>", piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "A & B");

        imprimere("  &amp; unescaping: VERUM\n");
    }

    {
        StmlResultus res;
        chorda textus;

        /* Quote unescaping: &quot; */
        res = stml_legere_ex_literis("<p>&quot;quoted&quot;</p>", piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "\"quoted\"");

        imprimere("  &quot; unescaping: VERUM\n");
    }

    {
        StmlResultus res;
        chorda textus;

        /* Apostrophe unescaping: &apos; */
        res = stml_legere_ex_literis("<p>&apos;single&apos;</p>", piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "'single'");

        imprimere("  &apos; unescaping: VERUM\n");
    }

    {
        StmlResultus res;
        StmlNodus* code;
        chorda textus;

        /* Raw tags preserve entities literally (no unescaping) */
        res = stml_legere_ex_literis("<code!>&lt;not unescaped&gt;</code>",
                                     piscina, intern);
        CREDO_VERUM(res.successus);
        code = res.elementum_radix;
        CREDO_NON_NIHIL(code);
        textus = stml_textus_internus(code, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "&lt;not unescaped&gt;");

        imprimere("  Raw tags preserve entities: VERUM\n");
    }

    {
        StmlResultus res;
        chorda serialized;

        /* Roundtrip equality: parse -> serialize -> same as input */
        res = stml_legere_ex_literis("<p>&lt;hello&gt;</p>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<p>&lt;hello&gt;</p>");

        imprimere("  Roundtrip equality: VERUM\n");
    }

    {
        StmlResultus res;
        chorda textus;

        /* Multiple entities in one text node */
        res = stml_legere_ex_literis("<p>&lt;a&gt; &amp;&amp; &lt;b&gt;</p>",
                                     piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "<a> && <b>");

        imprimere("  Multiple entities: VERUM\n");
    }

    {
        StmlResultus res;
        chorda textus;

        /* Unknown entities preserved as-is */
        res = stml_legere_ex_literis("<p>&foo; stays</p>", piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_textus_internus(res.elementum_radix, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus, "&foo; stays");

        imprimere("  Unknown entities preserved: VERUM\n");
    }

    /* ==================================================
     * Probare Capture Operator Serialization (Roundtrip)
     * ================================================== */

    imprimere("\n--- Probans Capture Operator Serialization ---\n");

    {
        StmlResultus res;
        chorda serialized;

        /* Forward capture roundtrip */
        res = stml_legere_ex_literis("<root><wrapper (><item/></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<root><wrapper (><item/></root>");

        imprimere("  Forward capture roundtrip: VERUM\n");
    }

    {
        StmlResultus res;
        chorda serialized;

        /* Multiple forward capture roundtrip */
        res = stml_legere_ex_literis("<root><wrapper ((><a/><b/></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<root><wrapper ((><a/><b/></root>");

        imprimere("  Multiple forward capture roundtrip: VERUM\n");
    }

    {
        StmlResultus res;
        chorda serialized;

        /* Backward capture roundtrip - ORDO FLUMINIS (spec triviae
         * §6): liberi capti retro in fonte ANTE tagum stant; emissio
         * non-pulchra ordinem authoris nunc reddit OCTETIM (olim ad
         * captor-primum reordinabat - circuitus non erat) */
        res = stml_legere_ex_literis("<root><item/><) wrapper></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<root><item/><) wrapper></root>");

        imprimere("  Backward capture roundtrip: VERUM\n");
    }

    {
        StmlResultus res;
        chorda serialized;

        /* Sandwich capture roundtrip - ordo fluminis (§6): liberum
         * primum ante tagum, reliqua post - octetim exactus nunc */
        res = stml_legere_ex_literis("<root><a/><= wrapper =><b/></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<root><a/><= wrapper =><b/></root>");

        imprimere("  Sandwich capture roundtrip: VERUM\n");
    }

    {
        StmlResultus res;
        chorda serialized;

        /* Forward capture with text */
        res = stml_legere_ex_literis("<wrapper (>hello", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<wrapper (>hello");

        imprimere("  Forward capture with text: VERUM\n");
    }

    /* ==================================================
     * Regula capturae (§1.2 emendatum) + parens glutinata (§1.6)
     * - collapsus T3, 2026-08-24
     * ================================================== */

    imprimere("\n--- Probans regulam capturae + parentheses"
              " glutinatas ---\n");

    {
        /* spatium post captorem = spatia_post captoris, elementum
         * capitur (non spatium) */
        StmlResultus res;
        StmlNodus* captor;
        chorda serialized;

        res = stml_legere_ex_literis("<radix><t(> <a/></radix>",
                                     piscina, intern);
        CREDO_VERUM(res.successus);
        captor = stml_invenire_liberum(res.elementum_radix, "t");
        CREDO_NON_NIHIL(captor);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(captor), I);
        CREDO_NON_NIHIL(stml_invenire_liberum(captor, "a"));
        CREDO_NON_NIHIL(captor->spatia_post);
        CREDO_CHORDA_AEQUALIS_LITERIS(*captor->spatia_post, " ");

        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<radix><t(> <a/></radix>");

        imprimere("  regula capturae: elementum capitur:"
                  " PRAETERITUM\n");
    }

    {
        /* margo ducens post captorem: valor sine spatio */
        StmlResultus res;
        StmlNodus* captor;
        StmlNodus* textus;
        chorda serialized;

        res = stml_legere_ex_literis("<radix><t(> foo</radix>",
                                     piscina, intern);
        CREDO_VERUM(res.successus);
        captor = stml_invenire_liberum(res.elementum_radix, "t");
        CREDO_NON_NIHIL(captor);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(captor), I);
        textus = stml_liberum_ad_indicem(captor, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_NON_NIHIL(textus->valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*textus->valor, "foo");
        CREDO_NON_NIHIL(captor->spatia_post);
        CREDO_CHORDA_AEQUALIS_LITERIS(*captor->spatia_post, " ");

        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<radix><t(> foo</radix>");

        imprimere("  regula capturae: valor purus: PRAETERITUM\n");
    }

    {
        /* spina glutinata: t1 -> t2 -> t3 -> textus */
        StmlResultus res;
        StmlNodus* t1;
        StmlNodus* t2;
        StmlNodus* t3;
        StmlNodus* textus;
        chorda serialized;

        res = stml_legere_ex_literis(
            "<radix><t1(> <t2(> <t3(> foo</radix>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        t1 = stml_invenire_liberum(res.elementum_radix, "t1");
        CREDO_NON_NIHIL(t1);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(t1), I);
        t2 = stml_invenire_liberum(t1, "t2");
        CREDO_NON_NIHIL(t2);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(t2), I);
        t3 = stml_invenire_liberum(t2, "t3");
        CREDO_NON_NIHIL(t3);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(t3), I);
        textus = stml_liberum_ad_indicem(t3, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_NON_NIHIL(textus->valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*textus->valor, "foo");

        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<radix><t1(> <t2(> <t3(> foo</radix>");

        imprimere("  spina glutinata octetim: PRAETERITUM\n");
    }

    {
        /* formae prae parenthesibus: spatiata et cum attributis -
         * octeti conditi redduntur (§1.6 emendatum) */
        StmlResultus res;
        chorda serialized;

        res = stml_legere_ex_literis("<radix><t (><a/></radix>",
                                     piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<radix><t (><a/></radix>");

        res = stml_legere_ex_literis(
            "<radix><t attr=\"v\" (><a/></radix>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<radix><t attr=\"v\" (><a/></radix>");

        imprimere("  spatium prae parenthesibus conditum:"
                  " PRAETERITUM\n");
    }

    {
        /* limen tecti collapsus (§0.2): LXXII columnae exactae
         * collabuntur; LXXIII (verbum unum infrangibile) formam
         * bloci re-fluxam capit (M3 - nihil ultra tectum manet
         * quod frangi potest; verbum ipsum in linea impletionis
         * LXX columnarum cadit) */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<t>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaa</t>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<t(> aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaa");

        res = stml_legere_ex_literis(
            "<t>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaa</t>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<t(>\n  aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaa");

        imprimere("  limen tecti LXXII/LXXIII: PRAETERITUM\n");
    }

    /* ==================================================
     * Collapsus verticalis (§0.2 decretum alterum, M2b):
     * forma capturae UNIVERSALIS liberis elementaribus -
     * latitudo dispositionem eligit, non formam
     * ================================================== */

    imprimere("\n--- Probans collapsum verticalem ---\n");

    {
        /* exemplar §0.2: catena captorum + terminalis bloci in
         * linea recenti, gradu uno altius, clausura suo gradu
         * (optio 2: captor tagum aperiens bloci numquam sorbet) */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<a><b><c><d/><e/></c></b></a>", piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<a(> <b(>\n  <c((>\n    <d/>\n    <e/>");

        imprimere("  exemplar verticale: PRAETERITUM\n");
    }

    {
        /* catena unius vinculi cum terminali bloci: etiam vinculum
         * solitarium formam capturae capit (linea '</>' deletur) */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<a><b><c/><d/></b></a>", piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<a(>\n  <b((>\n    <c/>\n    <d/>");

        imprimere("  vinculum solitarium + blocus: PRAETERITUM\n");
    }

    {
        /* sarcinatio avara ultra tectum: vincula in linea currenti
         * accumulantur dum tectum tenet; fractio lineae gradum UNUM
         * addit (per LINEAM); terminalis inline in fine lineae
         * ultimae vehitur */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<vinculumprimum><vinculumsecundum><vinculumtertium>"
            "<vinculumquartum>foo</vinculumquartum></vinculumtertium>"
            "</vinculumsecundum></vinculumprimum>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<vinculumprimum(> <vinculumsecundum(> <vinculumtertium(>"
            "\n  <vinculumquartum(> foo");

        imprimere("  sarcinatio avara: PRAETERITUM\n");
    }

    {
        /* captor auctoris dispositionem suam re-derivat via eadem
         * (unificatio §4): captee lineae novae in lineam redit si
         * cadit; forma stackata canonica sese reproducit */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis("<a (>\n<x/>", piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum, "<a(> <x/>");

        res = stml_legere_ex_literis(
            "<a(> <b(>\n  <c((>\n    <d/>\n    <e/>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<a(> <b(>\n  <c((>\n    <d/>\n    <e/>");

        imprimere("  re-derivatio auctoris: PRAETERITUM\n");
    }

    /* ==================================================
     * Lineae vacuae (§4): separatio paragraphorum superstes
     * - una aut duae servantur, plures ad duas cadunt
     * ================================================== */

    imprimere("\n--- Probans lineas vacuas ---\n");

    {
        /* linea vacua inter fratres blocorum (et ante commentum)
         * servatur */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<radix>\n  <a/>\n\n  <!-- nota -->\n  <b/>\n</radix>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<radix>\n  <a/>\n\n  <!-- nota -->\n  <b/>\n</>");

        imprimere("  linea vacua servata: PRAETERITUM\n");
    }

    {
        /* tres vacuae ad duas cadunt (tectum) */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<radix>\n  <a/>\n\n\n\n  <b/>\n</radix>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<radix((>\n  <a/>\n\n\n  <b/>");

        imprimere("  vacuae plures ad duas: PRAETERITUM\n");
    }

    {
        /* post tagum apertum et ante tagum claudentem: eadem
         * clausula (basis = linea prima ordinaria) */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<radix>\n\n  <a/>\n  <b/>\n\n</radix>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<radix((>\n\n  <a/>\n  <b/>");

        imprimere("  vacuae apud tagos: PRAETERITUM\n");
    }

    {
        /* gradus documenti: vacua inter processionem et radicem */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<?xml version=\"1.0\"?>\n\n<radix><a/><b/></radix>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<?xml version=\"1.0\"?>\n\n<radix((>\n  <a/>\n  <b/>");

        imprimere("  vacua gradus documenti: PRAETERITUM\n");
    }

    /* ==================================================
     * Tectum profundum (§0.2 decretum tertium): sub
     * indentatione gravi minimum XL columnarum contenti -
     * tectum = max(LXXII, indentatio + XL)
     * ================================================== */

    imprimere("\n--- Probans tectum profundum ---\n");

    {
        /* gradus XX (XL columnae indentationis): spina '<t(> '
         * + XXX litterae = XXXV columnae contenti - tectum
         * planum LXXII (spatium XXXII) recusaret, fundus XL
         * capit. Fons programmatice: XX gradus '<n><z/>'
         * (liberi bini = blocus quisque gradus), tum spina */
        ChordaAedificator* aed;
        StmlResultus res;
        StmlResultus res2;
        chorda fons;
        chorda scriptum;
        chorda rescriptum;
        i32 k;

        aed = chorda_aedificator_creare(piscina, 4096);
        CREDO_NON_NIHIL(aed);
        per (k = ZEPHYRUM; k < XX; k++)
        {
            chorda_aedificator_appendere_literis(aed, "<n><z/>");
        }
        chorda_aedificator_appendere_literis(aed,
            "<t>aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa</t>");
        per (k = ZEPHYRUM; k < XX; k++)
        {
            chorda_aedificator_appendere_literis(aed, "</n>");
        }
        fons = chorda_aedificator_finire(aed);

        res = stml_legere(fons, piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_CONTINET(scriptum, chorda_ex_literis(
            "<t(> aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", piscina));

        /* punctum fixum profundum */
        res2 = stml_legere(scriptum, piscina, intern);
        CREDO_VERUM(res2.successus);
        rescriptum = stml_scribere(res2.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS(rescriptum, scriptum);

        imprimere("  fundus XL sub gradu XX: PRAETERITUM\n");
    }

    /* ==================================================
     * Accessores sensus M3 (§2): stml_textus_valor +
     * stml_textus_fluxus - genus declaratum accessorem vincit
     * ================================================== */

    imprimere("\n--- Probans accessores valor/fluxus (M3) ---\n");

    {
        /* VALOR: concatenatio valorum textus, sine triviis.
         * Contentum '\' iam dedentatum in exemplari vivit */
        StmlResultus res;

        res = stml_legere_ex_literis(
            "<carmen\\>\n  prima\n    altior\n  ultima\n</>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_valor(res.elementum_radix, piscina),
            "prima\n  altior\nultima");

        res = stml_legere_ex_literis(
            "<p>salve <b>munde</b> iterum</p>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_valor(res.elementum_radix, piscina),
            "salve munde iterum");

        res = stml_legere_ex_literis("<t>\n  salve\n</t>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_valor(res.elementum_radix, piscina),
            "salve");

        res = stml_legere_ex_literis("<code!>a < b && c > d</code>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_valor(res.elementum_radix, piscina),
            "a < b && c > d");

        res = stml_legere_ex_literis("<p>a<br/>b</p>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_valor(res.elementum_radix, piscina), "ab");

        imprimere("  valor: exemplar legitur: PRAETERITUM\n");
    }

    {
        /* FLUXUS: lectio prosae. Margines nihil (trivia orae),
         * limites molles consecutivi in unum, recursio formae
         * textContent, cursus eiusdem lineae LITTERALES */
        StmlResultus res;

        res = stml_legere_ex_literis("<p>\n salve\n</p>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_fluxus(res.elementum_radix, piscina),
            "salve");

        res = stml_legere_ex_literis(
            "<radix>salve\n<!-- nota -->\nmunde</radix>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_fluxus(res.elementum_radix, piscina),
            "salve munde");

        res = stml_legere_ex_literis("<p>a<br/>b</p>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_fluxus(res.elementum_radix, piscina),
            "ab");

        res = stml_legere_ex_literis("<t>a  b</t>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_fluxus(res.elementum_radix, piscina),
            "a  b");

        res = stml_legere_ex_literis("<t>salve\nmunde</t>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_fluxus(res.elementum_radix, piscina),
            "salve munde");

        /* genus declaratum vincit: '\' per fluxum = valor intactus */
        res = stml_legere_ex_literis(
            "<carmen\\>\n  prima\n    altior\n  ultima\n</>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_fluxus(res.elementum_radix, piscina),
            "prima\n  altior\nultima");

        /* recursio + limites: liberum elementare fluxum suum fert */
        res = stml_legere_ex_literis(
            "<p>alpha <b>beta\ngamma</b>\ndelta</p>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            stml_textus_fluxus(res.elementum_radix, piscina),
            "alpha beta gamma delta");

        imprimere("  fluxus: lectio prosae: PRAETERITUM\n");
    }

    /* ==================================================
     * Re-involutio fluxus (§4 M3): pulcher prosam fluminis
     * POSSIDET - semper canonica, fracturae authoratae non
     * servatae ('\' = exceptio authoris)
     * ================================================== */

    imprimere("\n--- Probans re-involutionem fluxus ---\n");

    {
        /* iunctio + cascas capturae: valor multilineus iunctus
         * intra tectum cadit -> forma capturae */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis("<t>prima\nsecunda</t>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<t(> prima secunda");

        imprimere("  iunctio + captura: PRAETERITUM\n");
    }

    {
        /* linea vacua INTRA valorem fluminis prosa est, non
         * separatio paragraphorum (decretum §4 vacuas INTER nodos
         * regit) */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<t>pars una\n\npars altera</t>", piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<t(> pars una pars altera");

        imprimere("  vacua intra fluxum iungitur: PRAETERITUM\n");
    }

    {
        /* cursus multiplicium spatiorum INFRANGIBILIS et
         * litteralis (fluxus eum servat) */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis("<t>foo  bar baz</t>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<t(> foo  bar baz");

        imprimere("  spatia multiplicia litteralia: PRAETERITUM\n");
    }

    {
        /* impletio capturae stackatae (decretum quartum §4,
         * emendatum: impletio SUSPENSA): textus ultra tectum ->
         * textus clausuram in linea eadem sequitur, lineae
         * sequentes sub textu primo alineatae, SINE clausura -
         * cursus textus uno nodo (fracturae solum apud
         * notationem), regula capturae limitem ducentem
         * possidet. Fons laceratus consulto (semper canonica) */
        _pulchrum_probare(piscina, intern,
            "<t>aaaa bbbb cccc dddd eeee ffff gggg hhhh\n"
            "iiii jjjj kkkk llll mmmm nnnn oooo pppp\n"
            "qqqq rrrr ssss tttt</t>",
            "<t(> aaaa bbbb cccc dddd eeee ffff gggg hhhh iiii"
            " jjjj kkkk llll mmmm\n"
            "     nnnn oooo pppp qqqq rrrr ssss tttt",
            "impletio suspensa: PRAETERITUM");
    }

    {
        /* eadem cum attributis: columna suspensionis post
         * clausuram latiorem cadit */
        _pulchrum_probare(piscina, intern,
            "<t a=\"1\">aaaa bbbb cccc dddd eeee ffff gggg hhhh"
            " iiii jjjj kkkk llll mmmm nnnn oooo pppp qqqq rrrr"
            " ssss tttt</t>",
            "<t a=\"1\" (> aaaa bbbb cccc dddd eeee ffff gggg hhhh"
            " iiii jjjj kkkk llll\n"
            "            mmmm nnnn oooo pppp qqqq rrrr ssss tttt",
            "impletio suspensa cum attributis: PRAETERITUM");
    }

    {
        /* custos columnae latae: captor inline latus (columna
         * suspensionis XL columnas non relinquit) formam
         * VERTICALEM tenet - impletio gradu uno altius */
        _pulchrum_probare(piscina, intern,
            "<t a=\"0123456789012345678901234567890123456789\">"
            "textus primus secundus tertius</t>",
            "<t a=\"0123456789012345678901234567890123456789\" (>\n"
            "  textus primus secundus tertius",
            "suspensio recusata (columna lata): PRAETERITUM");
    }

    {
        /* margines sordidi: spatium litterale ducens aut caudale
         * re-involutionem ET capturam vetat (praecisio marginum in
         * relectione id ederet) - forma inline verbatim manet */
        StmlResultus res;
        chorda scriptum;

        res = stml_legere_ex_literis(
            "<radix>\n  <t>foo </t>\n  <u/>\n</radix>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<radix((>\n  <t>foo </>\n  <u/>");

        imprimere("  margines sordidi verbatim: PRAETERITUM\n");
    }

    /* ==================================================
     * Attributa multilinea (§0.2 decretum quintum)
     *
     * Tagum positione bloci quod tectum inline fallit attributum
     * per lineam frangit, nomina dextro-alineata in columnam '='
     * communem (aesthetica columnarum declarationum C89 domus).
     * Columna post titulum tagi cadit (margine minimo duorum
     * spatiorum a '<'); nomen longissimum aut clausura capturae
     * lata eam dextrorsum trudit. Clausurae glutinantes ('>', '/>')
     * valori ultimo agglutinatae manent; clausurae CAPTURAE lineam
     * propriam capiunt, '>' in columna '='. Latitudine ambabus
     * directionibus (semper canonica): forma multilinea authorata
     * quae inline cadit recolligitur.
     * ================================================== */

    imprimere("\n--- Probans attributa multilinea ---\n");

    {
        /* columna basalis: post titulum tagi ('=' col. XI),
         * clausura '/>' glutinata */
        _pulchrum_probare(piscina, intern,
            "<terminalis titulus=\"QUADRA_CLAUSA\""
            " genus=\"SILVA_LEX_QUADRA_CLAUSA\""
            " alterum=\"salve\"/>",
            "<terminalis\n"
            "    titulus=\"QUADRA_CLAUSA\"\n"
            "      genus=\"SILVA_LEX_QUADRA_CLAUSA\"\n"
            "    alterum=\"salve\"/>",
            "columna basalis + '/>' glutinata: PRAETERITUM");
    }

    {
        /* nomen longum columnam trudit: margine minimo duorum
         * spatiorum sedet, ceteri ad columnam eius alineantur */
        _pulchrum_probare(piscina, intern,
            "<terminalis titulus-longissimus-valde=\"QUADRA_CLAUSA\""
            " genus=\"SILVA_LEX_QUADRA_CLAUSA\"/>",
            "<terminalis\n"
            "  titulus-longissimus-valde=\"QUADRA_CLAUSA\"\n"
            "                      genus=\"SILVA_LEX_QUADRA_CLAUSA\"/>",
            "nomen longum columnam trudit: PRAETERITUM");
    }

    {
        /* attributum boolean nomine solo in columna participat */
        _pulchrum_probare(piscina, intern,
            "<terminalis titulus=\"QUADRA_CLAUSA_LONGA_SATIS\""
            " genus=\"SILVA_LEX_QUADRA_CLAUSA\" clausus/>",
            "<terminalis\n"
            "    titulus=\"QUADRA_CLAUSA_LONGA_SATIS\"\n"
            "      genus=\"SILVA_LEX_QUADRA_CLAUSA\"\n"
            "    clausus/>",
            "attributum boolean in columna: PRAETERITUM");
    }

    {
        /* intra tectum: forma inline manet (custos) */
        _pulchrum_probare(piscina, intern,
            "<t a=\"1\" b=\"2\"/>",
            "<t a=\"1\" b=\"2\"/>",
            "intra tectum inline manet (custos): PRAETERITUM");
    }

    {
        /* semper canonica: forma multilinea authorata quae inline
         * cadit recolligitur */
        _pulchrum_probare(piscina, intern,
            "<t\n  a=\"1\"\n  b=\"2\"/>",
            "<t a=\"1\" b=\"2\"/>",
            "multilinea authorata recolligitur: PRAETERITUM");
    }

    {
        /* captor re-involutionis: clausura '(>' linea propria, '>'
         * in columna '='; textus clausuram in linea eadem sequitur
         * (impletio suspensa - linea clausurae linea tagi est) */
        _pulchrum_probare(piscina, intern,
            "<productio genus=\"per-clausula-longissima-valde-nimis\""
            " id=\"per-clausula-p-longissima-item\">"
            "alfa beta gamma delta</productio>",
            "<productio\n"
            "     genus=\"per-clausula-longissima-valde-nimis\"\n"
            "        id=\"per-clausula-p-longissima-item\"\n"
            "         (> alfa beta gamma delta",
            "captor refluxus multilineus: PRAETERITUM");
    }

    {
        /* vinculum spinae ultra tectum in linea prima: forma
         * multilinea in situ, spina in linea clausurae pergit */
        _pulchrum_probare(piscina, intern,
            "<productio genus=\"per-clausula-longissima-valde-nimis\""
            " id=\"per-clausula-p-longissima-item\">"
            "<b><c/></b></productio>",
            "<productio\n"
            "     genus=\"per-clausula-longissima-valde-nimis\"\n"
            "        id=\"per-clausula-p-longissima-item\"\n"
            "         (> <b(> <c/>",
            "vinculum spinae multilineum: PRAETERITUM");
    }

    {
        /* captor crudus (ramus ANTE nuclei - spina crudum
         * recusat): '(>' linea propria, linea capta '>' directe
         * sequitur */
        _pulchrum_probare(piscina, intern,
            "<v! genus=\"per-clausula-longissima-valde-nimis\""
            " id=\"per-clausula-p-longissima-item\" (>lineacapta",
            "<v!\n"
            "  genus=\"per-clausula-longissima-valde-nimis\"\n"
            "     id=\"per-clausula-p-longissima-item\"\n"
            "      (>lineacapta",
            "captor crudus multilineus: PRAETERITUM");
    }

    {
        /* liberi bloci: '>' valori ultimo glutinata, liberi forma
         * bloci solita sub tago (IV liberi - captura multiplex
         * tres non excedit, §0.2 decretum sextum) */
        _pulchrum_probare(piscina, intern,
            "<productio genus=\"per-clausula-longissima-valde-nimis\""
            " id=\"per-clausula-p-longissima-item\">"
            "<a/><b/><c/><d/></productio>",
            "<productio\n"
            "     genus=\"per-clausula-longissima-valde-nimis\"\n"
            "        id=\"per-clausula-p-longissima-item\">\n"
            "  <a/>\n"
            "  <b/>\n"
            "  <c/>\n"
            "  <d/>\n"
            "</>",
            "'>' glutinata + liberi bloci: PRAETERITUM");
    }

    /* ==================================================
     * Captura multiplex (§0.2 decretum sextum): II aut III
     * liberi elementares intra tectum altitudinis -> '((>' /
     * '(((>', liberi lineis sequentibus, SINE clausura.
     * Numquam in linea tagi (spatium inter captos eiusdem
     * lineae nodus textus fieret - numeratio corrumperetur);
     * numquam ultra tres.
     * ================================================== */

    imprimere("\n--- Probans capturam multiplicem ---\n");

    {
        /* tres liberi -> '(((>' */
        _pulchrum_probare(piscina, intern,
            "<t><a/><b/><c/></t>",
            "<t(((>\n  <a/>\n  <b/>\n  <c/>",
            "tres liberi '(((>' : PRAETERITUM");
    }

    {
        /* quattuor liberi: forma bloci manet (tectum numeri) */
        _pulchrum_probare(piscina, intern,
            "<t><a/><b/><c/><d/></t>",
            "<t>\n  <a/>\n  <b/>\n  <c/>\n  <d/>\n</>",
            "quattuor liberi blocus manet: PRAETERITUM");
    }

    {
        /* captor multiplex authoratus planus -> canonicus
         * (re-derivatio; basis vacuarum ZEPHYRUM sub captore
         * authorato - lex proprietatis lineam primam post
         * captoris dat) */
        _pulchrum_probare(piscina, intern,
            "<t((>\n<a/>\n<b/>",
            "<t((>\n  <a/>\n  <b/>",
            "authoratus re-derivatur: PRAETERITUM");
    }

    {
        /* lineae vacuae inter captos servatae - post captorem ET
         * inter fratres; punctum fixum per cyclos duos (basis
         * formae authoratae pendet: tagum apertum I, captor 0) */
        _pulchrum_probare(piscina, intern,
            "<t>\n\n  <a/>\n\n  <b/>\n</t>",
            "<t((>\n\n  <a/>\n\n  <b/>",
            "vacuae inter captos: PRAETERITUM");
    }

    {
        /* attributa lata + captura multiplex: clausura '((>'
         * linea propria alineata (decretum quintum componitur) */
        _pulchrum_probare(piscina, intern,
            "<productio genus=\"per-clausula-longissima-valde-nimis\""
            " id=\"per-clausula-p-longissima-item\">"
            "<a/><b/></productio>",
            "<productio\n"
            "     genus=\"per-clausula-longissima-valde-nimis\"\n"
            "        id=\"per-clausula-p-longissima-item\"\n"
            "        ((>\n"
            "  <a/>\n"
            "  <b/>",
            "attributa lata + '((>' alineata: PRAETERITUM");
    }

    {
        /* tectum altitudinis: liberi ultra XII lineas formam
         * bloci tenent (liberum interius XII liberorum = XIV
         * lineae subtree) */
        _pulchrum_probare(piscina, intern,
            "<t><a><x/><x/><x/><x/><x/><x/><x/><x/><x/><x/><x/>"
            "<x/></a><b/></t>",
            "<t>\n"
            "  <a>\n"
            "    <x/>\n    <x/>\n    <x/>\n    <x/>\n"
            "    <x/>\n    <x/>\n    <x/>\n    <x/>\n"
            "    <x/>\n    <x/>\n    <x/>\n    <x/>\n"
            "  </>\n"
            "  <b/>\n"
            "</>",
            "tectum altitudinis blocus manet: PRAETERITUM");
    }

    {
        /* fidelitas: tagum multilineum authoratum octetim
         * redditur (trivia M1 - custos, non novum) */
        StmlResultus res;
        chorda scriptum;
        constans character* fons = "<t\n  a=\"1\"\n  b=\"2\"/>";

        res = stml_legere_ex_literis(fons, piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum, fons);

        imprimere("  fidelitas multilinea (custos): PRAETERITUM\n");
    }

    /* ==================================================
     * Fragment Tests
     * ================================================== */

    imprimere("\n--- Probans Fragmenta ---\n");

    {
        /* Anonymous fragment parsing */
        StmlResultus res;
        StmlNodus* frag;

        res = stml_legere_ex_literis("<root><#>content</#></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        frag = stml_invenire_liberum(res.elementum_radix, "#");
        CREDO_NON_NIHIL(frag);
        CREDO_VERUM(frag->fragmentum);
        CREDO_NIHIL(frag->fragmentum_id);

        imprimere("  Anonymous fragment: PRAETERITUM\n");
    }

    {
        /* Named fragment parsing */
        StmlResultus res;
        StmlNodus* frag;

        res = stml_legere_ex_literis("<root><#header>Title</#></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        frag = stml_invenire_liberum(res.elementum_radix, "#");
        CREDO_NON_NIHIL(frag);
        CREDO_VERUM(frag->fragmentum);
        CREDO_NON_NIHIL(frag->fragmentum_id);
        CREDO_CHORDA_AEQUALIS_LITERIS(*frag->fragmentum_id, "header");

        imprimere("  Named fragment: PRAETERITUM\n");
    }

    {
        /* Self-closing fragment */
        StmlResultus res;
        chorda serialized;

        res = stml_legere_ex_literis("<root><#placeholder/></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<root><#placeholder/></root>");

        imprimere("  Self-closing fragment: PRAETERITUM\n");
    }

    {
        /* Fragment roundtrip */
        StmlResultus res;
        chorda serialized;

        res = stml_legere_ex_literis("<doc><#sidebar><item/></#></doc>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<doc><#sidebar><item/></#></doc>");

        imprimere("  Fragment roundtrip: PRAETERITUM\n");
    }

    {
        /* Spatium templi '#@' (macros v1): id fragmenti '@' DUCENS
         * ferre potest - '<#@f>' = definitio templi (instantiatio),
         * '<#f>' = fragmentum contenti (alias/identitas). Sigillum
         * partem id est - '@f' internatur, non 'f'. */
        StmlResultus res;
        StmlNodus* frag;
        chorda serialized;

        res = stml_legere_ex_literis(
            "<doc><#@post-spatia n=\"@n\"><x/></#></doc>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        frag = stml_invenire_liberum(res.elementum_radix, "#");
        CREDO_NON_NIHIL(frag);
        si (frag != NIHIL)
        {
            CREDO_VERUM(frag->fragmentum);
            CREDO_NON_NIHIL(frag->fragmentum_id);
            si (frag->fragmentum_id != NIHIL)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS(*frag->fragmentum_id,
                    "@post-spatia");
            }
        }
        serialized = stml_scribere(res.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<doc><#@post-spatia n=\"@n\"><x/></#></doc>");

        imprimere("  Fragmentum spatii templi (#@): PRAETERITUM\n");
    }

    {
        /* Transclusionem creare (constructio manualis, macros v1):
         * scriptor arboris vocationes templi auctorat sine
         * parsatione - antea nodi transclusionis a parsatore solo
         * nascebantur */
        StmlNodus* trans;
        chorda serialized;

        trans = stml_transclusionem_creare(piscina, intern,
            chorda_ex_literis("#@post-spatia n=\"1\"", piscina));
        CREDO_NON_NIHIL(trans);
        si (trans != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)trans->genus,
                (i32)STML_NODUS_TRANSCLUSIO);
            serialized = stml_scribere(trans, piscina, FALSUM);
            CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
                "<<#@post-spatia n=\"1\">>");
        }

        imprimere("  Transclusionem creare: PRAETERITUM\n");
    }

    {
        /* Fragment with attributes */
        StmlResultus res;
        StmlNodus* frag;
        chorda* valor;

        res = stml_legere_ex_literis("<root><#comp visible=\"true\">content</#></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        frag = stml_invenire_liberum(res.elementum_radix, "#");
        CREDO_NON_NIHIL(frag);
        valor = stml_attributum_capere(frag, "visible");
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "true");

        imprimere("  Fragment with attributes: PRAETERITUM\n");
    }

    {
        /* Anonymous fragment roundtrip */
        StmlResultus res;
        chorda serialized;

        res = stml_legere_ex_literis("<doc><#>content</#></doc>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<doc><#>content</#></doc>");

        imprimere("  Anonymous fragment roundtrip: PRAETERITUM\n");
    }

    {
        /* Nested fragments */
        StmlResultus res;
        chorda serialized;

        res = stml_legere_ex_literis("<doc><#outer><#inner>text</#></#></doc>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<doc><#outer><#inner>text</#></#></doc>");

        imprimere("  Nested fragments: PRAETERITUM\n");
    }

    /* ==================================================
     * Fragmenta Capturantia: <#id (> / <# (> / saccharum <(>
     * (natalis 2026-07-30: bloci capturarum in articulis fori -
     * linea stampata '<#01KYRF (> textus' res tabularii gignit)
     * ================================================== */

    imprimere("\n--- Probans fragmenta capturantia ---\n");

    {
        /* Fragmentum nominatum capturans: textus frater captus,
         * circuitus octetim identicus (forma stampata) */
        StmlResultus res;
        StmlNodus* frag;
        StmlNodus* textus;
        chorda serialized;

        res = stml_legere_ex_literis(
            "<doc><#01KYRF (>eat a cake</doc>", piscina, intern);
        CREDO_VERUM(res.successus);
        frag = stml_invenire_liberum(res.elementum_radix, "#");
        CREDO_NON_NIHIL(frag);
        CREDO_VERUM(frag->fragmentum);
        CREDO_NON_NIHIL(frag->fragmentum_id);
        CREDO_CHORDA_AEQUALIS_LITERIS(*frag->fragmentum_id,
            "01KYRF");
        CREDO_AEQUALIS_I32(frag->captio_directio, STML_CAPTIO_ANTE);
        CREDO_AEQUALIS_I32(frag->captio_numerus, I);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(frag), I);
        textus = stml_liberum_ad_indicem(frag, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_AEQUALIS_I32(textus->genus, STML_NODUS_TEXTUS);

        serialized = stml_scribere(res.elementum_radix, piscina,
            FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<doc><#01KYRF (>eat a cake</doc>");

        imprimere("  Fragmentum nominatum capturans: PRAETERITUM\n");
    }

    {
        /* Parentheses binae: duos fratres capit */
        StmlResultus res;
        StmlNodus* frag;
        chorda serialized;

        res = stml_legere_ex_literis(
            "<doc><#a ((><x/><y/></doc>", piscina, intern);
        CREDO_VERUM(res.successus);
        frag = stml_invenire_liberum(res.elementum_radix, "#");
        CREDO_NON_NIHIL(frag);
        CREDO_AEQUALIS_I32(frag->captio_numerus, II);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(frag), II);
        CREDO_NON_NIHIL(stml_invenire_liberum(frag, "x"));
        CREDO_NON_NIHIL(stml_invenire_liberum(frag, "y"));

        serialized = stml_scribere(res.elementum_radix, piscina,
            FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<doc><#a ((><x/><y/></doc>");

        imprimere("  Parentheses binae fragmenti: PRAETERITUM\n");
    }

    {
        /* Saccharum <(>: fragmentum anonymum capturans; scriptor
         * ad <#(> normalizat (forma authoris ephemera; NIHIL =
         * glutinata canonica, §1.6 emendatum) */
        StmlResultus res;
        StmlResultus relectum;
        StmlNodus* frag;
        chorda serialized;
        chorda rescriptum;

        res = stml_legere_ex_literis(
            "<doc><(>textus</doc>", piscina, intern);
        CREDO_VERUM(res.successus);
        frag = stml_invenire_liberum(res.elementum_radix, "#");
        CREDO_NON_NIHIL(frag);
        CREDO_VERUM(frag->fragmentum);
        CREDO_NIHIL(frag->fragmentum_id);
        CREDO_AEQUALIS_I32(frag->captio_directio, STML_CAPTIO_ANTE);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(frag), I);

        serialized = stml_scribere(res.elementum_radix, piscina,
            FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<doc><#(>textus</doc>");

        /* forma normalizata = punctum fixum */
        relectum = stml_legere(serialized, piscina, intern);
        CREDO_VERUM(relectum.successus);
        rescriptum = stml_scribere(relectum.elementum_radix,
            piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS(serialized, rescriptum);

        imprimere("  Saccharum <(> normalizatum: PRAETERITUM\n");
    }

    {
        /* Blocus articuli (figura destinata): lineae mixtae -
         * anonyma et stampata - fragmenta bina fiunt */
        StmlResultus res;
        StmlResultus relectum;
        StmlNodus* radix;
        StmlNodus* primus;
        StmlNodus* secundus;
        chorda serialized;
        chorda rescriptum;
        i32 i;
        i32 num;
        i32 fragmenta;

        res = stml_legere_ex_literis(
            "<ideas>\n"
            "  <# (> eat a cake\n"
            "  <#01KYRG (> alter cibus\n"
            "</ideas>\n", piscina, intern);
        CREDO_VERUM(res.successus);
        radix = res.elementum_radix;
        CREDO_NON_NIHIL(radix);

        primus = NIHIL;
        secundus = NIHIL;
        fragmenta = ZEPHYRUM;
        num = stml_numerus_liberorum(radix);
        per (i = ZEPHYRUM; i < num; i++)
        {
            StmlNodus* l = stml_liberum_ad_indicem(radix, i);
            si (l && l->fragmentum)
            {
                fragmenta++;
                si (fragmenta == I) primus = l;
                alioquin secundus = l;
            }
        }
        CREDO_AEQUALIS_I32(fragmenta, II);
        CREDO_NON_NIHIL(primus);
        CREDO_NON_NIHIL(secundus);
        CREDO_NIHIL(primus->fragmentum_id);
        CREDO_NON_NIHIL(secundus->fragmentum_id);
        CREDO_CHORDA_AEQUALIS_LITERIS(*secundus->fragmentum_id,
            "01KYRG");
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(primus), I);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(secundus), I);

        /* circuitus: scriptio -> relectio -> rescriptio stabilis */
        serialized = stml_scribere(radix, piscina, FALSUM);
        relectum = stml_legere(serialized, piscina, intern);
        CREDO_VERUM(relectum.successus);
        rescriptum = stml_scribere(relectum.elementum_radix,
            piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS(serialized, rescriptum);

        imprimere("  Blocus articuli fragmentis binis: PRAETERITUM\n");
    }

    /* ==================================================
     * Clausura anonyma </> (01KYSPRF9R): elementum apertum
     * proximum claudit; forma authoris octetim servatur
     * ================================================== */

    imprimere("\n--- Probans clausuram anonymam ---\n");

    {
        /* proximum claudit - recursio ipsa 'proximum' dat */
        StmlResultus res;
        StmlNodus* b;
        chorda serialized;

        res = stml_legere_ex_literis("<r><b>x</></r>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        b = stml_invenire_liberum(res.elementum_radix, "b");
        CREDO_NON_NIHIL(b);
        CREDO_VERUM(b->clausura_anonyma);
        CREDO_FALSUM(res.elementum_radix->clausura_anonyma);

        serialized = stml_scribere(res.elementum_radix, piscina,
            FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<r><b>x</></r>");

        imprimere("  Clausura anonyma proxima: PRAETERITUM\n");
    }

    {
        /* nidificatio bina: quaeque clausura elementum suum */
        StmlResultus res;
        StmlNodus* a;
        StmlNodus* b;
        chorda serialized;

        res = stml_legere_ex_literis("<r><a><b>x</></></r>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        a = stml_invenire_liberum(res.elementum_radix, "a");
        CREDO_NON_NIHIL(a);
        b = stml_invenire_liberum(a, "b");
        CREDO_NON_NIHIL(b);
        CREDO_VERUM(a->clausura_anonyma);
        CREDO_VERUM(b->clausura_anonyma);

        serialized = stml_scribere(res.elementum_radix, piscina,
            FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized,
            "<r><a><b>x</></></r>");

        imprimere("  Clausura anonyma nidificata: PRAETERITUM\n");
    }

    {
        /* orba sine aperto = error (ut clausura nominata orba) */
        StmlResultus res;

        res = stml_legere_ex_literis("<r>x</r></>", piscina,
            intern);
        CREDO_FALSUM(res.successus);

        imprimere("  Clausura anonyma orba errat: PRAETERITUM\n");
    }

    /* ==================================================
     * Transclusion Tests
     * ================================================== */

    imprimere("\n--- Probans Transclusio ---\n");

    {
        /* Basic transclusion parsing */
        StmlResultus res;
        StmlNodus* trans;

        res = stml_legere_ex_literis("<root><<#header>></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        trans = stml_liberum_ad_indicem(res.elementum_radix, ZEPHYRUM);
        CREDO_NON_NIHIL(trans);
        CREDO_AEQUALIS_I32(trans->genus, STML_NODUS_TRANSCLUSIO);
        CREDO_CHORDA_AEQUALIS_LITERIS(*trans->valor, "#header");

        imprimere("  Basic transclusion: PRAETERITUM\n");
    }

    {
        /* Transclusion with complex selector */
        StmlResultus res;
        StmlNodus* trans;

        res = stml_legere_ex_literis("<root><<article .featured>></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        trans = stml_liberum_ad_indicem(res.elementum_radix, ZEPHYRUM);
        CREDO_NON_NIHIL(trans);
        CREDO_CHORDA_AEQUALIS_LITERIS(*trans->valor, "article .featured");

        imprimere("  Complex selector transclusion: PRAETERITUM\n");
    }

    {
        /* Transclusion roundtrip */
        StmlResultus res;
        chorda serialized;

        res = stml_legere_ex_literis("<doc>Before<<#nav>>After</doc>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<doc>Before<<#nav>>After</doc>");

        imprimere("  Transclusion roundtrip: PRAETERITUM\n");
    }

    {
        /* Scan '>>' citationum-conscius (macros v1): valores
         * argumentorum '>>' continere possunt - antea lexema in
         * '>>' interiore praemature secabatur */
        StmlResultus res;
        StmlNodus* trans;
        chorda serialized;

        res = stml_legere_ex_literis("<radix><<#f a=\"x>>y\">></radix>", piscina, intern);
        CREDO_VERUM(res.successus);
        si (res.successus)
        {
            trans = stml_liberum_ad_indicem(res.elementum_radix, ZEPHYRUM);
            CREDO_NON_NIHIL(trans);
            si (trans != NIHIL)
            {
                CREDO_AEQUALIS_I32(trans->genus, STML_NODUS_TRANSCLUSIO);
                CREDO_CHORDA_AEQUALIS_LITERIS(*trans->valor, "#f a=\"x>>y\"");
            }
            serialized = stml_scribere(res.radix, piscina, FALSUM);
            CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<radix><<#f a=\"x>>y\">></radix>");
        }

        imprimere("  Transclusio citationum-conscia: PRAETERITUM\n");
    }

    {
        /* Multiple transclusions */
        StmlResultus res;

        res = stml_legere_ex_literis("<page><<#header>><<#content>><<#footer>></page>",
                                      piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(res.elementum_radix), III);

        imprimere("  Multiple transclusions: PRAETERITUM\n");
    }

    {
        /* Transclusion with whitespace in selector */
        StmlResultus res;
        StmlNodus* trans;

        res = stml_legere_ex_literis("<root><<  .item  >></root>", piscina, intern);
        CREDO_VERUM(res.successus);
        trans = stml_liberum_ad_indicem(res.elementum_radix, ZEPHYRUM);
        CREDO_NON_NIHIL(trans);
        CREDO_CHORDA_AEQUALIS_LITERIS(*trans->valor, ".item");  /* Trimmed */

        imprimere("  Whitespace trimmed selector: PRAETERITUM\n");
    }

    {
        /* Mixed content with transclusion */
        StmlResultus res;
        chorda serialized;

        res = stml_legere_ex_literis("<doc><header/><<#nav>><footer/></doc>", piscina, intern);
        CREDO_VERUM(res.successus);
        serialized = stml_scribere(res.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(serialized, "<doc><header/><<#nav>><footer/></doc>");

        imprimere("  Mixed content transclusion: PRAETERITUM\n");
    }

    /* ==================================================
     * Probare capturam lineae crudae <tag! (>
     * (reliquum lineae CRUDUM - sine tags, sine entiis)
     * ================================================== */

    imprimere("\n--- Probans capturam lineae crudae ---\n");

    {
        /* Asperitates crudae: anguli, ampersand, quotae -
         * verbatim ad finem lineae */
        StmlResultus res;
        StmlNodus*   vexillum;
        StmlNodus*   textus;

        res = stml_legere_ex_literis(
            "<res>\n"
            "  <vexillum! (>-DVIA=<a&b> \"c\" -O2\n"
            "</res>\n", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        vexillum = stml_invenire_liberum(res.elementum_radix,
            "vexillum");
        CREDO_NON_NIHIL(vexillum);
        CREDO_VERUM(vexillum->crudus);
        CREDO_AEQUALIS_I32(vexillum->captio_directio,
            STML_CAPTIO_ANTE);
        CREDO_AEQUALIS_I32(vexillum->captio_numerus, I);
        CREDO_AEQUALIS_I32(xar_numerus(vexillum->liberi), I);

        textus = stml_liberum_ad_indicem(vexillum, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_AEQUALIS_I32(textus->genus, STML_NODUS_TEXTUS);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "-DVIA=<a&b> \"c\" -O2"));

        imprimere("  Captura cruda asperitatum: PRAETERITUM\n");
    }

    {
        /* Regressio vitii devorantis: crudae binae in ordine +
         * parens recte clauditur (pristinum: prima </parentis>
         * devorabat -> TAG_NON_CLAUSUM) */
        StmlResultus res;
        Xar*         vexilla;
        StmlNodus*   secundum;
        StmlNodus*   textus;

        res = stml_legere_ex_literis(
            "<nexus>\n"
            "  <vexillum! (>-framework Cocoa\n"
            "  <vexillum! (>-framework WebKit\n"
            "</nexus>\n", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);

        vexilla = stml_invenire_omnes_liberos(res.elementum_radix,
            "vexillum", piscina);
        CREDO_NON_NIHIL(vexilla);
        CREDO_AEQUALIS_I32(xar_numerus(vexilla), II);

        secundum = *(StmlNodus**)xar_obtinere(vexilla, I);
        textus = stml_liberum_ad_indicem(secundum, ZEPHYRUM);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "-framework WebKit"));

        imprimere("  Crudae binae + parens clausus: PRAETERITUM\n");
    }

    {
        /* Entia in linea cruda NON solvuntur (crudum = crudum) */
        StmlResultus res;
        StmlNodus*   vexillum;
        StmlNodus*   textus;

        res = stml_legere_ex_literis(
            "<res><vexillum! (>a&amp;b\n</res>", piscina, intern);
        CREDO_VERUM(res.successus);
        vexillum = stml_invenire_liberum(res.elementum_radix,
            "vexillum");
        CREDO_NON_NIHIL(vexillum);
        textus = stml_liberum_ad_indicem(vexillum, ZEPHYRUM);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "a&amp;b"));

        imprimere("  Entia cruda intacta: PRAETERITUM\n");
    }

    {
        /* Captura vacua: elementum sine liberis */
        StmlResultus res;
        StmlNodus*   vexillum;

        res = stml_legere_ex_literis(
            "<res><vexillum! (>\n</res>", piscina, intern);
        CREDO_VERUM(res.successus);
        vexillum = stml_invenire_liberum(res.elementum_radix,
            "vexillum");
        CREDO_NON_NIHIL(vexillum);
        CREDO_VERUM(vexillum->crudus);
        CREDO_AEQUALIS_I32(xar_numerus(vexillum->liberi), ZEPHYRUM);

        imprimere("  Captura vacua: PRAETERITUM\n");
    }

    {
        /* ((> numerum notat sed adhuc UNAM lineam capit (v1) */
        StmlResultus res;
        StmlNodus*   vexillum;

        res = stml_legere_ex_literis(
            "<res>\n"
            "  <vexillum! ((>prima linea\n"
            "  <alia/>\n"
            "</res>\n", piscina, intern);
        CREDO_VERUM(res.successus);
        vexillum = stml_invenire_liberum(res.elementum_radix,
            "vexillum");
        CREDO_NON_NIHIL(vexillum);
        CREDO_AEQUALIS_I32(vexillum->captio_numerus, II);
        CREDO_AEQUALIS_I32(xar_numerus(vexillum->liberi), I);
        CREDO_NON_NIHIL(stml_invenire_liberum(res.elementum_radix,
            "alia"));

        imprimere("  ((> notatum, una linea (v1): PRAETERITUM\n");
    }

    {
        /* Circuitus: lectio -> scriptio -> relectio -> rescriptio,
         * ambabus posturis stabilis */
        StmlResultus res;
        StmlResultus relectum;
        chorda       scriptum;
        chorda       rescriptum;

        res = stml_legere_ex_literis(
            "<nexus>\n"
            "  <vexillum! (>-DSQLITE_LIKE=\"x\" -O2\n"
            "  <alia/>\n"
            "</nexus>\n", piscina, intern);
        CREDO_VERUM(res.successus);

        /* postura non-pulchra: '\n' post lineam captam scribi
         * debet ne <alia/> in relectione devoretur */
        scriptum = stml_scribere(res.elementum_radix, piscina,
            FALSUM);
        relectum = stml_legere(scriptum, piscina, intern);
        CREDO_VERUM(relectum.successus);
        CREDO_NON_NIHIL(stml_invenire_liberum(
            relectum.elementum_radix, "alia"));
        rescriptum = stml_scribere(relectum.elementum_radix,
            piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS(scriptum, rescriptum);

        /* postura pulchra */
        scriptum = stml_scribere(res.elementum_radix, piscina,
            VERUM);
        relectum = stml_legere(scriptum, piscina, intern);
        CREDO_VERUM(relectum.successus);
        rescriptum = stml_scribere(relectum.elementum_radix,
            piscina, VERUM);
        CREDO_CHORDA_AEQUALIS(scriptum, rescriptum);

        imprimere("  Circuitus crudae ambabus posturis: PRAETERITUM\n");
    }

    {
        /* Via constructoris (semita manifesti aedilis): nodus manu
         * structus -> scriptio -> lectio -> valor idem */
        StmlNodus*   nexus;
        StmlNodus*   vexillum;
        StmlNodus*   textus;
        StmlResultus relectum;
        chorda       scriptum;

        nexus = stml_elementum_creare(piscina, intern, "nexus");
        vexillum = stml_elementum_creare(piscina, intern,
            "vexillum");
        CREDO_NON_NIHIL(nexus);
        CREDO_NON_NIHIL(vexillum);
        vexillum->crudus = VERUM;
        vexillum->captio_directio = STML_CAPTIO_ANTE;
        vexillum->captio_numerus = I;
        CREDO_VERUM(stml_textum_addere(vexillum, piscina, intern,
            "-DFOO=\"bar\" -framework Cocoa"));
        CREDO_VERUM(stml_liberum_addere(nexus, vexillum));

        scriptum = stml_scribere(nexus, piscina, FALSUM);
        relectum = stml_legere(scriptum, piscina, intern);
        CREDO_VERUM(relectum.successus);
        vexillum = stml_invenire_liberum(relectum.elementum_radix,
            "vexillum");
        CREDO_NON_NIHIL(vexillum);
        textus = stml_liberum_ad_indicem(vexillum, ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "-DFOO=\"bar\" -framework Cocoa"));

        imprimere("  Via constructoris cruda: PRAETERITUM\n");
    }

    /* ==================================================
     * PROBARE: stml_strictum - forma bene formata
     *
     * Parser consulto lenis est; hae probationes ea nominant quae
     * parsationem transeunt sed documentum TACITE ambiguum
     * reddunt. Quaeque VITIO PLANTATO probatur - porta silens et
     * porta mortua idem spectantur.
     * ================================================== */

    {
        StmlResultus        r;
        Xar*                vitia;
        StmlStrictumVitium* v;

        imprimere("\n--- Probans stml_strictum ---\n");

        /* sanum: nihil */
        r = stml_legere_ex_literis("<a n=\"1\"><b>x</b></a>",
                                   piscina, intern);
        CREDO_VERUM(r.successus);
        vitia = stml_strictum(r.radix, piscina);
        CREDO_NON_NIHIL(vitia);
        CREDO_AEQUALIS_I32(xar_numerus(vitia), ZEPHYRUM);

        /* attributum duplicatum - periculosissimum: ambo servantur,
         * capere PRIMUM reddit */
        r = stml_legere_ex_literis("<a n=\"1\" n=\"2\">x</a>",
                                   piscina, intern);
        CREDO_VERUM(r.successus);
        vitia = stml_strictum(r.radix, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(vitia), I);
        v = (StmlStrictumVitium*)xar_obtinere(vitia, ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)v->genus,
            (i32)STML_STRICTUM_ATTRIBUTUM_DUPLICATUM);
        CREDO_NON_NIHIL(v->causa);
        CREDO_VERUM(chorda_aequalis_literis(*v->causa, "n"));

        /* radices plures */
        r = stml_legere_ex_literis("<a/><b/>", piscina, intern);
        CREDO_VERUM(r.successus);
        vitia = stml_strictum(r.radix, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(vitia), I);
        v = (StmlStrictumVitium*)xar_obtinere(vitia, ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)v->genus,
            (i32)STML_STRICTUM_RADICES_PLURES);

        /* radices TRES = vitium unum (non duo) */
        r = stml_legere_ex_literis("<a/><b/><c/>", piscina, intern);
        CREDO_VERUM(r.successus);
        vitia = stml_strictum(r.radix, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(vitia), I);

        /* textus extra radicem */
        r = stml_legere_ex_literis("textus<a/>", piscina, intern);
        CREDO_VERUM(r.successus);
        vitia = stml_strictum(r.radix, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(vitia), I);
        v = (StmlStrictumVitium*)xar_obtinere(vitia, ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)v->genus,
            (i32)STML_STRICTUM_TEXTUS_EXTRA_RADICEM);

        /* spatium album extra radicem NON est vitium */
        r = stml_legere_ex_literis("\n  <a/>\n", piscina, intern);
        CREDO_VERUM(r.successus);
        vitia = stml_strictum(r.radix, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(vitia), ZEPHYRUM);

        /* titulus vacuus: '<>' */
        r = stml_legere_ex_literis("<>x</>", piscina, intern);
        CREDO_VERUM(r.successus);
        vitia = stml_strictum(r.radix, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(vitia), I);
        v = (StmlStrictumVitium*)xar_obtinere(vitia, ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)v->genus,
            (i32)STML_STRICTUM_TITULUS_VACUUS);

        /* LENITAS SERVATA - haec mensurata et ACCEPTA sunt
         * (2026-08-06), non vitia: */
        r = stml_legere_ex_literis("<a disabled>x</a>", piscina,
                                   intern);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32(
            xar_numerus(stml_strictum(r.radix, piscina)), ZEPHYRUM);

        r = stml_legere_ex_literis("<a nomen=valor>x</a>", piscina,
                                   intern);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32(
            xar_numerus(stml_strictum(r.radix, piscina)), ZEPHYRUM);

        r = stml_legere_ex_literis("<a>&ignotum;</a>", piscina,
                                   intern);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32(
            xar_numerus(stml_strictum(r.radix, piscina)), ZEPHYRUM);

        /* fragmentum titulum vacuum habere LICET */
        r = stml_legere_ex_literis("<a><#>x</#></a>", piscina,
                                   intern);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32(
            xar_numerus(stml_strictum(r.radix, piscina)), ZEPHYRUM);

        imprimere("  stml_strictum: PRAETERITUM\n");
    }

    /* ==================================================
     * Probans titulum illegaliter incipientem (vitium 2026-08-10)
     *
     * Olim: '<.x>' vel '<9bad>' nomen VACUUM legebat, positus non
     * progrediebatur, et clausura anonyma vitium TACITE devorabat
     * (successus=VERUM, arbor corrupta). Nunc: vitium clarum.
     * '<>' lenis MANET (strictum TITULUS_VACUUS iudicat, supra).
     * ================================================== */
    imprimere("\n--- Probans titulum illegaliter incipientem ---\n");
    {
        StmlResultus r;

        /* corruptio tacita pristina - nunc vitium clarum
         * (puncta ducentia postea legalia facta - sectio sequens;
         * hic characteres PERPETUO illegales) */
        r = stml_legere_ex_literis("<@x>y</@x>", piscina, intern);
        CREDO_VERUM(!r.successus);
        CREDO_AEQUALIS_I32((i32)r.status,
                           (i32)STML_ERROR_SYNTAXIS);

        r = stml_legere_ex_literis("<a><9b>x</9b></a>", piscina,
                                   intern);
        CREDO_VERUM(!r.successus);
        CREDO_AEQUALIS_I32((i32)r.status,
                           (i32)STML_ERROR_SYNTAXIS);

        /* initium numericum - eadem familia (olim SIGSEGV apud
         * consumptores arboris per titulos vacuos) */
        r = stml_legere_ex_literis("<9bad/>", piscina, intern);
        CREDO_VERUM(!r.successus);

        /* clausura mala intra elementum sanum */
        r = stml_legere_ex_literis("<a>x</@a>", piscina, intern);
        CREDO_VERUM(!r.successus);

        /* lenitates pinnatae MANENT */
        r = stml_legere_ex_literis("<>x</>", piscina, intern);
        CREDO_VERUM(r.successus);
        r = stml_legere_ex_literis("<a>x</>", piscina, intern);
        CREDO_VERUM(r.successus);

        imprimere("  titulus illegalis: PRAETERITUM\n");
    }

    /* ==================================================
     * Probans titulos punctatos (spatium generum, 2026-08-10)
     *
     * '<.species>' generat quod '.species' citat - signum '.' in
     * positione tituli, pars NOMINIS ipsius. Punctum unum ducens,
     * dein character initialis normalis. Attributa numquam.
     * ================================================== */
    imprimere("\n--- Probans titulos punctatos ---\n");
    {
        StmlResultus r;
        chorda scripta;

        /* parse + structura + punctum in nomine servatum */
        r = stml_legere_ex_literis(
            "<.species nomen=\"apis\"><.b>x</.b></.species>",
            piscina, intern);
        CREDO_VERUM(r.successus);
        CREDO_NON_NIHIL(r.elementum_radix);
        CREDO_VERUM(_chorda_ptr_eq_literis(r.elementum_radix->titulus,
                                           ".species"));

        /* iter rotundum octetim */
        scripta = stml_scribere(r.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scripta,
            "<.species nomen=\"apis\"><.b>x</.b></.species>");

        /* auto-clausum */
        r = stml_legere_ex_literis("<.s/>", piscina, intern);
        CREDO_VERUM(r.successus);

        /* contentum crudum punctatum (clausura crudi punctata) */
        r = stml_legere_ex_literis("<.cru!>a<b</.cru!>",
                                   piscina, intern);
        CREDO_VERUM(r.successus);

        /* clausura anonyma cum titulo punctato */
        r = stml_legere_ex_literis("<.a>x</>", piscina, intern);
        CREDO_VERUM(r.successus);

        /* NEGATIVA: punctum solum, geminatum, attributum punctatum */
        r = stml_legere_ex_literis("<.>x</.>", piscina, intern);
        CREDO_VERUM(!r.successus);
        r = stml_legere_ex_literis("<..x>y</..x>", piscina, intern);
        CREDO_VERUM(!r.successus);
        r = stml_legere_ex_literis("<a .attr=\"x\"/>",
                                   piscina, intern);
        CREDO_VERUM(!r.successus);

        imprimere("  tituli punctati: PRAETERITUM\n");
    }

    /* ==================================================
     * Probans augmentationem <% &clavis;> (librarium W3, 2026-08-10)
     *
     * '<% &y;> ... </%>' contentum additivum individuo bibliothecae
     * &y; apponit. Clavis destinata sola - verbum in signo latet
     * (unum verbum legale, appositio). Clavis VERBATIM sigillata
     * servatur ('&c;' - eaedem litterae quibus citationes
     * comparantur). Ante hoc '<%' vitium clarum erat (titulus
     * illegaliter incipiens) - grammatica aperta, non mutata.
     * ================================================== */
    imprimere("\n--- Probans augmentationem ---\n");
    {
        StmlResultus r;
        StmlNodus*   aug;
        chorda       scripta;

        /* parse + structura: elementum titulo "%" cum clave */
        r = stml_legere_ex_literis(
            "<res><% &c;><nota>x</nota></%></res>", piscina, intern);
        CREDO_VERUM(r.successus);
        CREDO_NON_NIHIL(r.elementum_radix);
        aug = stml_primus_liberum(r.elementum_radix);
        CREDO_NON_NIHIL(aug);
        CREDO_AEQUALIS_I32((i32)aug->genus,
                           (i32)STML_NODUS_ELEMENTUM);
        CREDO_VERUM(_chorda_ptr_eq_literis(aug->titulus, "%"));
        CREDO_NON_NIHIL(aug->augmentum_clavis);
        CREDO_VERUM(_chorda_ptr_eq_literis(aug->augmentum_clavis,
                                           "&c;"));
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(aug), I);

        /* iter rotundum octetim */
        scripta = stml_scribere(r.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scripta,
            "<res><% &c;><nota>x</nota></%></res>");

        /* clavis composita (lineola subter) + vacua augmentatio */
        r = stml_legere_ex_literis("<% &quercus_prima;></%>",
                                   piscina, intern);
        CREDO_VERUM(r.successus);
        CREDO_VERUM(_chorda_ptr_eq_literis(
            r.elementum_radix->augmentum_clavis, "&quercus_prima;"));
        scripta = stml_scribere(r.elementum_radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scripta,
            "<% &quercus_prima;></%>");

        /* duplicatio clavem fert */
        aug = stml_duplicare(r.elementum_radix, piscina, intern);
        CREDO_NON_NIHIL(aug);
        CREDO_VERUM(_chorda_ptr_eq_literis(aug->augmentum_clavis,
                                           "&quercus_prima;"));

        /* NEGATIVA: sine clave, clavis nuda, clavis generis,
         * clavis vacua (sine </%> - vitium UNUM quodque, status
         * exactus ita adfirmabilis: vitium posterius priorem
         * superscribit) */
        r = stml_legere_ex_literis("<%>", piscina, intern);
        CREDO_VERUM(!r.successus);
        CREDO_AEQUALIS_I32((i32)r.status,
                           (i32)STML_ERROR_SYNTAXIS);
        r = stml_legere_ex_literis("<% laika>", piscina, intern);
        CREDO_VERUM(!r.successus);
        CREDO_AEQUALIS_I32((i32)r.status,
                           (i32)STML_ERROR_SYNTAXIS);
        r = stml_legere_ex_literis("<% .canis>", piscina, intern);
        CREDO_VERUM(!r.successus);
        r = stml_legere_ex_literis("<% &;>", piscina, intern);
        CREDO_VERUM(!r.successus);

        /* non clausum ad finem = vitium (ambitum tacite devorare
         * vetitum - augmentatio quae reliquum documenti caperet) */
        r = stml_legere_ex_literis("<% &c;><nota>x</nota>",
                                   piscina, intern);
        CREDO_VERUM(!r.successus);
        CREDO_AEQUALIS_I32((i32)r.status,
                           (i32)STML_ERROR_TAG_NON_CLAUSUM);

        /* clausura orba */
        r = stml_legere_ex_literis("<a>x</%></a>", piscina, intern);
        CREDO_VERUM(!r.successus);

        imprimere("  augmentatio: PRAETERITUM\n");
    }

    /* ==================================================
     * Scriptio pulchra: contentum MIXTUM (textus + elementa)
     *
     * CONTRACTUS M2 (spec triviae §4, TERMINI deleti): decisio UNA
     * per elementum - liberum textus QUODVIS -> INLINE (liberi
     * omnes in linea tagi, sine terminis additis; textus
     * solum-albus eiusdem lineae CONTENTUM est et SERVATUR);
     * aliter -> BLOCUS (liberum per lineam, indentatum).
     *
     * Liberi inline pulchrum=FALSUM accipiunt ('ne te indentes') -
     * fidelitas SEORSUM fluit (vexillum proprium ab M1), ergo
     * pretium vetus 'forma interna plana' pretium manet sed fides
     * numquam laeditur. */

    imprimere("\n--- Probans scriptionem pulchram contenti mixti ---\n");

    /* NOTA DE CLAUSURIS: modus pulcher clausuram TACITAM ('</>')
     * eligit dum contentum breve sit (vide STML_CLAUSURA_TACITA_
     * LINEAE). Liberi inline pulchrum=FALSUM ferunt, ergo formam
     * AUTHORIS retinent - unde '</ante>'/'</post>' iuxta '</>'
     * stat. */

    /* I. elementum, deinde textus -> inline totum */
    _pulchrum_probare(piscina, intern,
        "<t><ante>x</ante>n</t>",
        "<t><ante>x</ante>n</>",
        "I elementum-tum-textus: PRAETERITUM");

    /* II. textus, deinde elementum - speculum casus I */
    _pulchrum_probare(piscina, intern,
        "<t>n<post>y</post></t>",
        "<t>n<post>y</post></>",
        "II textus-tum-elementum: PRAETERITUM");

    /* III. textus utrimque: elementum interpositum indentationem
     * SUAM omnino perdit - lectio sola sibi constans contractus */
    _pulchrum_probare(piscina, intern,
        "<t>a<b/>c</t>",
        "<t>a<b/>c</>",
        "III textus-elementum-textus: PRAETERITUM");

    /* IV. CUSTOS: nullus textus - dispositio lineata manet (liberum
     * per lineam, indentatum); duo liberi -> captura multiplex
     * (§0.2 decretum sextum), clausura perit */
    _pulchrum_probare(piscina, intern,
        "<t><a/><b/></t>",
        "<t((>\n  <a/>\n  <b/>",
        "IV omnia-elementa (custos): PRAETERITUM");

    /* V. CUSTOS: textus unicus - casum specialem praeexsistentem
     * tangit; forma brevissima quam Fran petivit ('<foo>abc</>') */
    _pulchrum_probare(piscina, intern,
        "<t>textus solus</t>",
        "<t(> textus solus",
        "V textus-unicus (custos): PRAETERITUM");

    /* VI. albus solus eiusdem lineae inter elementa: CONTENTUM
     * deliberatum est (§1.3) et SERVATUR - elementum inline fit.
     * (Transparentia vetus TERMINI eum delebat - classis vitii
     * quam porta arborum in T1 deprehendit.) */
    _pulchrum_probare(piscina, intern,
        "<t><a/>  <b/></t>",
        "<t><a/>  <b/></>",
        "VI albus-eiusdem-lineae servatus: PRAETERITUM");

    /* VII. termini plures: omnis terminus textum tangit */
    _pulchrum_probare(piscina, intern,
        "<t>a<b/>c<d/>e</t>",
        "<t>a<b/>c<d/>e</>",
        "VII termini plures: PRAETERITUM");

    /* VIII. nidificatio: radix vinculum spinae (liberum unicum) ->
     * forma capturae (M2b); 't' terminalis bloci gradu uno altius,
     * ipse INLINE (textum fert) - regulae per gradum componuntur,
     * linea '</>' radicis deleta */
    _pulchrum_probare(piscina, intern,
        "<root><t><ante>x</ante>n</t></root>",
        "<root(>\n  <t><ante>x</ante>n</>",
        "VIII nidificatio componens: PRAETERITUM");

    /* IX. elementum-TEXTUS-elementum: forma quam ARBOR ipsa fert
     * (lexema = <ante> + litterae + <post>). Casus consumptoris
     * veri, ab octo prioribus NON tactus */
    _pulchrum_probare(piscina, intern,
        "<t><a/>n<b/></t>",
        "<t><a/>n<b/></>",
        "IX elementum-textus-elementum (forma arboris): PRAETERITUM");

    /* X. PRETIUM NOMINATUM: liberus inline pulchrum=FALSUM accipit,
     * ergo formam suam INTERNAM planam reddit ET clausuram
     * nominatam retinet. Mensuratum */
    _pulchrum_probare(piscina, intern,
        "<t>n<post><alta><altior/></alta></post></t>",
        "<t>n<post><alta><altior/></alta></post></>",
        "X planatio vicini (pretium fixum): PRAETERITUM");

    /* XI. effugium entium per semitam mixtam */
    _pulchrum_probare(piscina, intern,
        "<t><a/>x &amp; y</t>",
        "<t><a/>x &amp; y</>",
        "XI effugium entium in contento mixto: PRAETERITUM");

    /* XII. COMMENTUM textui adiacens - inline cum ceteris */
    _pulchrum_probare(piscina, intern,
        "<t><!--c-->n</t>",
        "<t><!--c-->n</>",
        "XII commentum textui adiacens: PRAETERITUM");

    {
        /* XIII. LIMEN clausurae tacitae - altera pars regulae,
         * ab omnibus supra NON tacta (omnes breves sunt).
         * Ultra limen nomen RETINETUR quia tag apertum extra
         * schermum abiit et nomen tunc auxilium est.
         *
         * INVARIANS quem hoc figit: contentum parentis contentum
         * liberi CONTINET, ergo parens numquam brevior est quam
         * liberus. Unde clausura tacita parentis clausuram tacitam
         * omnium posterorum IMPLICAT - catenae '</>' semper
         * contiguae, numquam intermixtae. */
        ChordaAedificator* aed;
        chorda             ingressus;
        chorda             exitus;
        StmlResultus       res;
        i32                j;

        aed = chorda_aedificator_creare(piscina, MMMMXCVI);
        chorda_aedificator_appendere_literis(aed, "<longus>");
        per (j = ZEPHYRUM; j < (XXX + X); j++)
        {
            chorda_aedificator_appendere_literis(aed, "<a/>");
        }
        chorda_aedificator_appendere_literis(aed, "</longus>");
        ingressus = chorda_aedificator_finire(aed);

        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        exitus = stml_scribere(res.elementum_radix, piscina, VERUM);

        /* XL liberi => XLI lineae > XXX: nomen RETINETUR */
        CREDO_VERUM(chorda_continet(exitus,
            chorda_ex_literis("</longus>", piscina)));

        imprimere("  XIII limen: nomen ultra limen retentum: PRAETERITUM\n");
    }

    /* ==================================================
     * XIV. LEXEMATA - fluxus publicus sine arbore
     * ================================================== */

    imprimere("\nXIV. Lexemata (stml_lexemata_colligere)\n");

    {
        /* A. TOTALITAS: lexemata input CONTIGUE et TOTUM tegunt.
         *
         * Oraculum fortius quam numerus lexematum: hiatus inter
         * finem unius et initium proximi = octeti quos pictor
         * numquam coloraret et nemo desideraret - vitium quod
         * numerando invisibile est. */
        chorda      ingressus;
        Xar*        fluxus;
        i32         k;
        i32         tectum;
        StmlLexema* lx;

        ingressus = chorda_ex_literis(
            "<radix a=\"1\">textus<#f/><<#f>></radix>", piscina);
        fluxus = stml_lexemata_colligere(ingressus, piscina, intern);
        CREDO_VERUM(fluxus != NIHIL);
        CREDO_VERUM(xar_numerus(fluxus) > ZEPHYRUM);

        tectum = ZEPHYRUM;
        per (k = ZEPHYRUM; k < xar_numerus(fluxus); k++)
        {
            lx = (StmlLexema*)xar_obtinere(fluxus, k);
            CREDO_AEQUALIS_I32(lx->initium, tectum);
            CREDO_VERUM(lx->finis > lx->initium);
            tectum = lx->finis;
        }
        CREDO_AEQUALIS_I32(tectum, ingressus.mensura);

        imprimere("  A totalitas: %d lexemata, %d octeti tecti\n",
            (integer)xar_numerus(fluxus), (integer)tectum);
    }

    {
        /* B. SIGILLA DISCERNUNTUR - id ipsum propter quod lexator
         * HTML STML colorare non potest: ille '<#f/>' et '<<#f>>'
         * in 'tag' unum planaret. */
        chorda      ingressus;
        Xar*        fluxus;
        i32         k;
        b32         fragmentum;
        b32         transclusio;
        b32         crudus;
        b32         percentum;
        StmlLexema* lx;

        ingressus = chorda_ex_literis(
            "<#f/><<#f>><x!>crudum</x><% &clavis;></%>", piscina);
        fluxus = stml_lexemata_colligere(ingressus, piscina, intern);
        CREDO_VERUM(fluxus != NIHIL);

        fragmentum  = FALSUM;
        transclusio = FALSUM;
        crudus      = FALSUM;
        percentum   = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(fluxus); k++)
        {
            lx = (StmlLexema*)xar_obtinere(fluxus, k);
            si (lx->genus == STML_TOKEN_FRAGMENTUM_AUTO)
            {
                fragmentum = VERUM;
            }
            si (lx->genus == STML_TOKEN_TRANSCLUSIO)
            {
                transclusio = VERUM;
            }
            si (lx->genus == STML_TOKEN_CRUDUS)
            {
                crudus = VERUM;
            }
            si (lx->genus == STML_TOKEN_PERCENTUM_APERIRE)
            {
                percentum = VERUM;
            }
        }
        CREDO_VERUM(fragmentum);
        CREDO_VERUM(transclusio);
        CREDO_VERUM(crudus);
        CREDO_VERUM(percentum);

        imprimere("  B sigilla: fragmentum/transclusio/crudus/"
            "percentum omnia discreta\n");
    }

    {
        /* C. CONTENTUM CRUDUM SINE PARSATORE: '<b>' intra '<x!>'
         * textus est, non tag. Haec est proprietas quae gyrum
         * nudum tokenum omnino possibilem facit - modum crudum
         * '_tok_proximus' ipse regit. Si parsator eum regeret,
         * haec assertio caderet. */
        chorda      ingressus;
        Xar*        fluxus;
        i32         k;
        i32         aperturae;
        StmlLexema* lx;

        ingressus = chorda_ex_literis("<x!>a<b>c</x>", piscina);
        fluxus = stml_lexemata_colligere(ingressus, piscina, intern);
        CREDO_VERUM(fluxus != NIHIL);

        aperturae = ZEPHYRUM;
        per (k = ZEPHYRUM; k < xar_numerus(fluxus); k++)
        {
            lx = (StmlLexema*)xar_obtinere(fluxus, k);
            si (lx->genus == STML_TOKEN_APERIRE)
            {
                aperturae++;
            }
        }
        CREDO_AEQUALIS_I32(aperturae, ZEPHYRUM);

        imprimere("  C crudum: '<b>' intra '<x!>' tag NON est\n");
    }

    {
        /* D. PARSATIO NULLA: input quem 'stml_legere' iure
         * RECUSAT fluxum tamen reddit - unde instrumenta textum
         * semiplenum (qualis est dum scribitur) tractare possunt.
         * Par oraculorum: alterum recusat, alterum non. */
        chorda       ingressus;
        Xar*         fluxus;
        StmlResultus res;

        ingressus = chorda_ex_literis("<a><b>", piscina);

        res = stml_legere(ingressus, piscina, intern);
        CREDO_FALSUM(res.successus);

        fluxus = stml_lexemata_colligere(ingressus, piscina, intern);
        CREDO_VERUM(fluxus != NIHIL);
        CREDO_AEQUALIS_I32(xar_numerus(fluxus), II);

        imprimere("  D sine parsatione: arbor recusat, fluxus "
            "II lexemata reddit\n");
    }

    {
        /* E. RECUSATIONES: argumentum absens NIHIL reddit (non
         * Xar vacuum - 'nihil dedi' et 'nihil inveni' duo sunt). */
        chorda vacua;

        vacua.datum   = NIHIL;
        vacua.mensura = ZEPHYRUM;

        CREDO_VERUM(stml_lexemata_colligere(vacua, piscina, intern)
            == NIHIL);
        CREDO_VERUM(stml_lexemata_colligere(
            chorda_ex_literis("<a/>", piscina), NIHIL, intern)
            == NIHIL);

        imprimere("  E recusatio: input vacuum / piscina absens "
            "-> NIHIL\n");
    }

    /* ==================================================
     * XV. EXTENSIONES NODORUM (positus_initium/finis)
     * ================================================== */

    imprimere("\nXV. Extensiones nodorum\n");

    {
        /* A. EXTENSIO = FETTA VERA FONTIS.
         *
         * Oraculum quod numeros solos vincit: extensione fontem
         * SECAMUS et cum textu exspectato comparamus. Numerus
         * falsus qui 'probabilis' videtur (off-by-one, clausura
         * omissa) hic CADIT; numerus contra numerum non caderet. */
        chorda       ingressus;
        StmlResultus res;
        StmlNodus*   radix;
        StmlNodus*   a;
        chorda       fetta;

        ingressus = chorda_ex_literis(
            "<radix><a>x</a><b/></radix>", piscina);
        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        radix = res.elementum_radix;
        CREDO_VERUM(radix != NIHIL);

        /* radix totum documentum tegit */
        CREDO_AEQUALIS_I32(radix->positus_initium, ZEPHYRUM);
        CREDO_AEQUALIS_I32(radix->positus_finis, ingressus.mensura);

        /* liberus primus: '<a>x</a>' EXACTE - tag, contentum,
           clausura. Fetta comparata, non numeri. */
        a = stml_liberum_ad_indicem(radix, ZEPHYRUM);
        CREDO_VERUM(a != NIHIL);
        fetta = chorda_ex_buffer(
            ingressus.datum + a->positus_initium,
            a->positus_finis - a->positus_initium);
        CREDO_CHORDA_AEQUALIS_LITERIS(fetta, "<a>x</a>");

        /* liberus alter: '<b/>' auto-claudens */
        a = stml_liberum_ad_indicem(radix, I);
        CREDO_VERUM(a != NIHIL);
        fetta = chorda_ex_buffer(
            ingressus.datum + a->positus_initium,
            a->positus_finis - a->positus_initium);
        CREDO_CHORDA_AEQUALIS_LITERIS(fetta, "<b/>");

        imprimere("  A fetta: '<a>x</a>' et '<b/>' exacte\n");
    }

    {
        /* B. NIDIFICATIO: extensio liberi INTRA parentem iacet, et
         * fratres non se intersecant. Invarians quem lector
         * 'nodum ex positione quaerens' PRAESUMIT - ergo
         * asserendus, non speratus. */
        chorda       ingressus;
        StmlResultus res;
        StmlNodus*   radix;
        StmlNodus*   p;
        StmlNodus*   q;

        ingressus = chorda_ex_literis(
            "<r>\n  <p a=\"1\">textus</p>\n  <q/>\n</r>", piscina);
        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        radix = res.elementum_radix;

        p = stml_invenire_liberum(radix, "p");
        q = stml_invenire_liberum(radix, "q");
        CREDO_VERUM(p != NIHIL);
        CREDO_VERUM(q != NIHIL);

        CREDO_VERUM(p->positus_initium >= radix->positus_initium);
        CREDO_VERUM(p->positus_finis <= radix->positus_finis);
        CREDO_VERUM(p->positus_finis > p->positus_initium);
        /* fratres disiuncti, ordine documenti */
        CREDO_VERUM(p->positus_finis <= q->positus_initium);

        imprimere("  B nidificatio: liberus intra parentem, "
            "fratres disiuncti\n");
    }

    {
        /* C. QUAESTIO CONTINENTIAE - usus verus.
         *
         * 'Quis nodus octetum hunc tenet?' est quaestio quam
         * inspector STML ponit cum usor in textu premit. Lineae
         * solae eam solvere NON possunt: hic 'p' et 'q' in LINEA
         * EADEM iacent, ergo quaesitio per lineam ambiguum
         * redderet. Extensio discernit. */
        chorda       ingressus;
        StmlResultus res;
        StmlNodus*   radix;
        StmlNodus*   p;
        StmlNodus*   q;
        i32          sedes_q;

        ingressus = chorda_ex_literis(
            "<r><p>aa</p><q>bb</q></r>", piscina);
        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        radix = res.elementum_radix;
        p = stml_invenire_liberum(radix, "p");
        q = stml_invenire_liberum(radix, "q");
        CREDO_VERUM(p != NIHIL && q != NIHIL);

        /* eadem linea uterque - probatio quod linea non sufficit */
        CREDO_AEQUALIS_I32(p->linea, q->linea);

        /* octetus 'b' primi intra q, non intra p */
        sedes_q = q->positus_initium + III;   /* '<q>' -> 'b' */
        CREDO_VERUM(sedes_q >= q->positus_initium
                 && sedes_q <  q->positus_finis);
        CREDO_FALSUM(sedes_q >= p->positus_initium
                  && sedes_q <  p->positus_finis);

        imprimere("  C continentia: 'p' et 'q' linea EADEM, "
            "extensione discreti\n");
    }

    {
        /* D. NODI NON E PARSATIONE: ZEPHYRUM utrumque - eadem
         * conventio quam 'linea' iam tenet. */
        StmlNodus* manu;

        manu = stml_elementum_creare(piscina, intern, "manus");
        CREDO_VERUM(manu != NIHIL);
        CREDO_AEQUALIS_I32(manu->linea, ZEPHYRUM);
        CREDO_AEQUALIS_I32(manu->positus_initium, ZEPHYRUM);
        CREDO_AEQUALIS_I32(manu->positus_finis, ZEPHYRUM);

        imprimere("  D manu creatus: extensio ZEPHYRUM "
            "(conventio lineae servata)\n");
    }

    {
        /* E. FIDELITAS INTACTA: extensiones metadatum PARSATIONIS
         * sunt, ergo scriptor eas numquam legit - documentum
         * rescriptum octetim idem manet. Assertio quae campum
         * novum ab emissore separat. */
        chorda       ingressus;
        StmlResultus res;
        chorda       scriptum;

        ingressus = chorda_ex_literis(
            "<r><p a=\"1\">x</p><q/></r>", piscina);
        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.elementum_radix, piscina,
            FALSUM);
        CREDO_CHORDA_AEQUALIS(scriptum, ingressus);

        imprimere("  E fidelitas: scriptio octetim eadem\n");
    }

    /* ==================================================
     * Canonicalizatio CRLF (spec triviae §3)
     * ================================================== */

    imprimere("\n--- Probans canonicalizationem CRLF ---\n");

    {
        /* A. DOCUMENTUM CRLF: vexillum positum, arbor formam LF
         * videt, scriptio non-pulchra formam LF octetim reddit -
         * contractus fidelitatis super octetos CANONICALIZATOS. */
        chorda       ingressus;
        chorda       expectatum;
        StmlResultus res;
        chorda       scriptum;

        ingressus = chorda_ex_literis(
            "<r>\r\n  <p>salve\r\nmunde</p>\r\n</r>", piscina);
        expectatum = chorda_ex_literis(
            "<r>\n  <p>salve\nmunde</p>\n</r>", piscina);

        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.crlf_canonicalizatum);

        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS(scriptum, expectatum);

        imprimere("  A CRLF: vexillum + scriptio forma LF\n");
    }

    {
        /* B. DOCUMENTUM LF PURUM: vexillum FALSUM, nulla copia
         * facta (fidelitas octetim intacta). */
        chorda       ingressus;
        StmlResultus res;
        chorda       scriptum;

        ingressus = chorda_ex_literis("<r>\n  <p>x</p>\n</r>",
            piscina);
        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_FALSUM(res.crlf_canonicalizatum);

        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS(scriptum, ingressus);

        imprimere("  B LF purum: vexillum FALSUM, octetim idem\n");
    }

    {
        /* C. '\r' SOLIVAGUM (sine '\n'): CONTENTUM manet - regula
         * angusta CRLF sola est, nulla purgatio generalis. */
        chorda       ingressus;
        StmlResultus res;
        chorda       scriptum;

        ingressus = chorda_ex_literis("<r>a\rb</r>", piscina);
        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_FALSUM(res.crlf_canonicalizatum);

        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS(scriptum, ingressus);

        imprimere("  C '\\r' solivagum: contentum servatum\n");
    }

    {
        /* D. CAPTURA LINEAE CRUDAE sub CRLF: praecisio '\r'
         * per-genus DELETA est - canonicalizatio introitus eandem
         * rem uno loco facit. Valor captus sine '\r'. */
        chorda       ingressus;
        StmlResultus res;
        StmlNodus*   imp;
        chorda       textus;

        ingressus = chorda_ex_literis(
            "<r><imp! (>via/crudi\r\n<post/></r>", piscina);
        res = stml_legere(ingressus, piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.crlf_canonicalizatum);

        imp = stml_invenire_liberum(res.elementum_radix, "imp");
        CREDO_VERUM(imp != NIHIL);
        si (imp != NIHIL)
        {
            textus = stml_textus_internus(imp, piscina);
            CREDO_CHORDA_AEQUALIS_LITERIS(textus, "via/crudi");
        }

        imprimere("  D captura cruda sub CRLF: valor sine '\\r'\n");
    }

    /* ==================================================
     * Scala generum textus: '<tag\>' (spec triviae §1.4)
     * ================================================== */

    imprimere("\n--- Probans multilineam '<tag\\>' ---\n");

    {
        /* A. BLOCUS VERSUUM: dedentatio in parsatione, structura
         * relativa servata, circuitus octetim, internus exactus */
        StmlResultus res;
        StmlNodus*   textus;
        chorda       scriptum;

        res = stml_legere_ex_literis(
            "<versus\\>\n  prima\n    altior\n  ultima\n</>",
            piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.elementum_radix);
        CREDO_VERUM(res.elementum_radix->multilinea);
        CREDO_NON_NIHIL(res.elementum_radix->indentatio);
        CREDO_VERUM(_chorda_ptr_eq_literis(
            res.elementum_radix->indentatio, "  "));

        textus = stml_liberum_ad_indicem(res.elementum_radix,
            ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "prima\n  altior\nultima"));
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->spatia_ante,
            "\n"));
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->spatia_post,
            "\n"));

        CREDO_VERUM(chorda_aequalis_literis(
            stml_textus_internus(res.elementum_radix, piscina),
            "\n  prima\n    altior\n  ultima\n"));

        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<versus\\>\n  prima\n    altior\n  ultima\n</>");

        imprimere("  A versus: dedentatio + circuitus: VERUM\n");
    }

    {
        /* B. AEQUIVALENTIA CAUDALIS (decretum Franis): '123</>',
         * '123\n</>', '123\n  </>' - valor IDEM, octeti sui cuique
         * circuitu redditi */
        constans character* fontes[III];
        i32                 f;

        fontes[ZEPHYRUM] = "<m\\>\n  abc\n  123</>";
        fontes[I]        = "<m\\>\n  abc\n  123\n</>";
        fontes[II]       = "<m\\>\n  abc\n  123\n  </>";

        per (f = ZEPHYRUM; f < III; f++)
        {
            StmlResultus res;
            StmlNodus*   textus;
            chorda       scriptum;

            res = stml_legere_ex_literis(fontes[f], piscina,
                intern);
            CREDO_VERUM(res.successus);
            textus = stml_liberum_ad_indicem(res.elementum_radix,
                ZEPHYRUM);
            CREDO_NON_NIHIL(textus);
            CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
                "abc\n123"));

            scriptum = stml_scribere(res.radix, piscina, FALSUM);
            CREDO_VERUM(chorda_aequalis_literis(scriptum,
                fontes[f]));
        }

        imprimere("  B aequivalentia caudalis (III formae): VERUM\n");
    }

    {
        /* C. LINEA PRIMA PROFUNDIOR: praecisio ducens = '\n' SOLA,
         * ergo indentatio lineae primae in dedentationem intrat et
         * structura relativa superest */
        StmlResultus res;
        StmlNodus*   textus;

        res = stml_legere_ex_literis(
            "<m\\>\n      alta\n  ima\n</>", piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_liberum_ad_indicem(res.elementum_radix,
            ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "    alta\nima"));
        CREDO_VERUM(_chorda_ptr_eq_literis(
            res.elementum_radix->indentatio, "  "));

        imprimere("  C linea prima profundior servata: VERUM\n");
    }

    {
        /* D. CONTENTUM IN LINEA TAGI: legale, a dedentatione
         * exclusum (regula PEP-257), circuitus octetim */
        StmlResultus res;
        StmlNodus*   textus;
        chorda       scriptum;

        res = stml_legere_ex_literis(
            "<m\\>abc\n  def\n</>", piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_liberum_ad_indicem(res.elementum_radix,
            ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "abc\ndef"));
        CREDO_NIHIL(textus->spatia_ante);
        CREDO_VERUM(_chorda_ptr_eq_literis(
            res.elementum_radix->indentatio, "  "));

        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<m\\>abc\n  def\n</>");

        imprimere("  D contentum lineae tagi exclusum: VERUM\n");
    }

    {
        /* E. CRUDUM MULTILINEA '<code!\>': entia mortua, tags
         * mortui, dedentatio viva, circuitus octetim */
        StmlResultus res;
        StmlNodus*   textus;
        chorda       scriptum;

        res = stml_legere_ex_literis(
            "<code!\\>\n  a < b && c > d\n</code>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.elementum_radix->crudus);
        CREDO_VERUM(res.elementum_radix->multilinea);
        textus = stml_liberum_ad_indicem(res.elementum_radix,
            ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "a < b && c > d"));

        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<code!\\>\n  a < b && c > d\n</code>");

        imprimere("  E crudum multilinea: VERUM\n");
    }

    {
        /* F. RECUSATIONES NOMINATAE (§1.4): quaeque causam suam
         * fert (status MULTILINEA); ordo '\!' = SYNTAXIS (lexatio
         * ipsa mutaretur) */
        StmlResultus res;

        res = stml_legere_ex_literis(
            "<m\\>\n  a\n   \n  b\n</m>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32((i32)res.status,
            (i32)STML_ERROR_MULTILINEA);
        CREDO_VERUM(res.error.mensura > ZEPHYRUM);

        res = stml_legere_ex_literis(
            "<m\\>\n  <b/>\n</m>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32((i32)res.status,
            (i32)STML_ERROR_MULTILINEA);

        res = stml_legere_ex_literis(
            "<m\\ (><x/></m>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32((i32)res.status,
            (i32)STML_ERROR_MULTILINEA);

        res = stml_legere_ex_literis(
            "<c!\\ (>linea\n<post/>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32((i32)res.status,
            (i32)STML_ERROR_MULTILINEA);

        res = stml_legere_ex_literis(
            "<m\\!>x</m>", piscina, intern);
        CREDO_FALSUM(res.successus);
        CREDO_AEQUALIS_I32((i32)res.status,
            (i32)STML_ERROR_SYNTAXIS);

        imprimere("  F recusationes V nominatae: VERUM\n");
    }

    {
        /* G. TAB/SPATIUM MIXTA: praefixum commune octetim nullum -
         * nihil demptum, deterministice */
        StmlResultus res;
        StmlNodus*   textus;

        res = stml_legere_ex_literis(
            "<m\\>\n\tuna\n  duo\n</>", piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_liberum_ad_indicem(res.elementum_radix,
            ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "\tuna\n  duo"));
        CREDO_NIHIL(res.elementum_radix->indentatio);

        imprimere("  G tab/spatium mixta - nihil demptum: VERUM\n");
    }

    {
        /* H. FORMAE: signum cum attributis; auto-clausum; contentum
         * totum album -> elementum vacuum genuinum (clausurae
         * octetos fert); lineae interiores VACUAE manent */
        StmlResultus res;
        StmlNodus*   textus;
        chorda       scriptum;

        res = stml_legere_ex_literis(
            "<m\\ clavis=\"v\">\n  x\n</>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.elementum_radix->multilinea);
        CREDO_NON_NIHIL(stml_attributum_capere(
            res.elementum_radix, "clavis"));
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<m\\ clavis=\"v\">\n  x\n</>");

        res = stml_legere_ex_literis("<m\\/>", piscina, intern);
        CREDO_VERUM(res.successus);
        CREDO_VERUM(res.elementum_radix->multilinea);
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum, "<m\\/>");

        res = stml_legere_ex_literis("<m\\>\n</m>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        CREDO_AEQUALIS_I32(stml_numerus_liberorum(
            res.elementum_radix), ZEPHYRUM);
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum, "<m\\>\n</m>");

        res = stml_legere_ex_literis(
            "<m\\>\n  a\n\n  b\n</>", piscina, intern);
        CREDO_VERUM(res.successus);
        textus = stml_liberum_ad_indicem(res.elementum_radix,
            ZEPHYRUM);
        CREDO_NON_NIHIL(textus);
        CREDO_VERUM(_chorda_ptr_eq_literis(textus->valor,
            "a\n\nb"));
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<m\\>\n  a\n\n  b\n</>");

        imprimere("  H formae (attributa/auto/vacuum/lineae "
                  "vacuae): VERUM\n");
    }

    /* ==================================================
     * Trivia intra tagum (spec triviae §1.6)
     * ================================================== */

    imprimere("\n--- Probans trivia intra tagum ---\n");

    {
        /* A. TAG MULTILINEARIS: dispositio attributorum circuitum
         * octetim superest (foramen fidelitatis vetus clausum) */
        StmlResultus res;
        chorda       scriptum;

        res = stml_legere_ex_literis(
            "<a\n  b=\"1\"\n  c=\"2\">x</a>", piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum,
            "<a\n  b=\"1\"\n  c=\"2\">x</a>");

        imprimere("  A tag multilinearis octetim: VERUM\n");
    }

    {
        /* B. SPATIUM ANTE FINEM: '<a b="1" >' et '<a />' formam
         * suam tenent; spatium unicum canonicum NIHIL manet */
        StmlResultus res;
        chorda       scriptum;

        res = stml_legere_ex_literis("<a b=\"1\" >x</a>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum, "<a b=\"1\" >x</a>");

        res = stml_legere_ex_literis("<a />", piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum, "<a />");

        res = stml_legere_ex_literis("<a b=\"1\">x</a>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        {
            StmlAttributum* attr;

            attr = (StmlAttributum*)xar_obtinere(
                res.elementum_radix->attributa, ZEPHYRUM);
            CREDO_NON_NIHIL(attr);
            CREDO_NIHIL(attr->spatia_ante);
        }
        CREDO_NIHIL(res.elementum_radix->spatia_intra_tagum);

        imprimere("  B spatium prae finem + canonicum: VERUM\n");
    }

    {
        /* C. SPATIUM CIRCA '=' NORMALIZATUM (exceptio angusta
         * nominata §1.6): 'attr = "v"' fit 'attr="v"' ambobus
         * modis */
        StmlResultus res;
        chorda       scriptum;

        res = stml_legere_ex_literis("<a b = \"1\">x</a>", piscina,
            intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, FALSUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(scriptum, "<a b=\"1\">x</a>");

        imprimere("  C '=' normalizatum: VERUM\n");
    }

    {
        /* D. PULCHER REGENERAT: dispositio multilinearis tagi ad
         * spatium unicum redit (§1.6 - regula dispositionis
         * multilinearis formatoris RESERVATA) */
        StmlResultus res;
        chorda       scriptum;

        res = stml_legere_ex_literis(
            "<r><a\n  b=\"1\">x</a></r>", piscina, intern);
        CREDO_VERUM(res.successus);
        scriptum = stml_scribere(res.radix, piscina, VERUM);
        /* collapsus formam capturae dat; dispositio attributi ad
         * spatium unicum rediit intra eam */
        CREDO_VERUM(chorda_continet(scriptum,
            chorda_ex_literis("<a b=\"1\" (>", piscina)));

        imprimere("  D pulcher dispositionem regenerat: VERUM\n");
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
