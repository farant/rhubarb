/*
 * probatio_selectio.c - Probationes pro Selectore CSS Parsore et Congruente
 */

#include <stdio.h>
#include <string.h>
#include "selectio.h"
#include "stml.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"

/* Auxilium ad STML parsandum et elementum radix obtinendum */
interior StmlNodus*
_legere_stml (
    constans character*  input,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    StmlResultus resultus;
    resultus = stml_legere_ex_literis(input, piscina, intern);
    si (!resultus.successus) {
        redde NIHIL;
    }
    redde resultus.elementum_radix;
}

s32
principale (vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    b32                  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_selectio", 65536);
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ==================================================
     * PROBATIONES PARSATIONIS
     * ================================================== */

    imprimere("\n--- Probans Selectio Parsing ---\n");

    /* Probatio: Parsare selectorem tituli simplicem */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis("div", piscina, intern);

        CREDO_VERUM(resultus.successus);
        CREDO_NON_NIHIL(resultus.selectio);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_AEQUALIS_I32(simplex->genus, SELECTIO_TITULUS);
        CREDO_CHORDA_AEQUALIS_LITERIS(*simplex->valor, "div");
    }

    /* Probatio: Parsare selectorem ID */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis("#myid", piscina, intern);

        CREDO_VERUM(resultus.successus);
        CREDO_NON_NIHIL(resultus.selectio);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_AEQUALIS_I32(simplex->genus, SELECTIO_ID);
        CREDO_CHORDA_AEQUALIS_LITERIS(*simplex->valor, "myid");
    }

    /* Probatio: Parsare selectorem classis */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis(".myclass", piscina, intern);

        CREDO_VERUM(resultus.successus);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_AEQUALIS_I32(simplex->genus, SELECTIO_CLASSIS);
        CREDO_CHORDA_AEQUALIS_LITERIS(*simplex->valor, "myclass");
    }

    /* Probatio: Parsare selectorem compositum div.class#id */
    {
        SelectioResultus resultus;
        SelectioSimplex* s0;
        SelectioSimplex* s1;
        SelectioSimplex* s2;

        resultus = selectio_legere_ex_literis("div.class#id", piscina, intern);

        CREDO_VERUM(resultus.successus);
        CREDO_AEQUALIS_I32(xar_numerus(resultus.selectio->composita->partes), 3);

        s0 = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        s1 = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 1);
        s2 = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 2);

        CREDO_AEQUALIS_I32(s0->genus, SELECTIO_TITULUS);
        CREDO_AEQUALIS_I32(s1->genus, SELECTIO_CLASSIS);
        CREDO_AEQUALIS_I32(s2->genus, SELECTIO_ID);
    }

    /* Probatio: Parsare selectorem attributi [attr] */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis("[disabled]", piscina, intern);

        CREDO_VERUM(resultus.successus);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_AEQUALIS_I32(simplex->genus, SELECTIO_ATTRIBUTUM);
        CREDO_AEQUALIS_I32(simplex->attr_op, ATTR_OP_EXISTIT);
        CREDO_CHORDA_AEQUALIS_LITERIS(*simplex->valor, "disabled");
    }

    /* Probatio: Parsare selectorem attributi [attr=val] */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis("[type=text]", piscina, intern);

        CREDO_VERUM(resultus.successus);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_AEQUALIS_I32(simplex->attr_op, ATTR_OP_AEQUALIS);
        CREDO_CHORDA_AEQUALIS_LITERIS(*simplex->valor, "type");
        CREDO_CHORDA_AEQUALIS_LITERIS(*simplex->attr_valor, "text");
    }

    /* Probatio: Parsare selectorem attributi cum quotationibus [attr="val"] */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis("[data-value=\"hello world\"]", piscina, intern);

        CREDO_VERUM(resultus.successus);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_CHORDA_AEQUALIS_LITERIS(*simplex->attr_valor, "hello world");
    }

    /* Probatio: Parsare selectorem attributi incipit-cum [attr^=val] */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis("[href^=https]", piscina, intern);

        CREDO_VERUM(resultus.successus);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_AEQUALIS_I32(simplex->attr_op, ATTR_OP_INCIPIT);
    }

    /* Probatio: Parsare pseudo-classem :first-child */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis(":first-child", piscina, intern);

        CREDO_VERUM(resultus.successus);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_AEQUALIS_I32(simplex->genus, SELECTIO_PSEUDO_CLASSIS);
        CREDO_AEQUALIS_I32(simplex->pseudo, PSEUDO_PRIMUS_LIBERUM);
    }

    /* Probatio: Parsare pseudo-classem :empty */
    {
        SelectioResultus resultus;
        SelectioSimplex* simplex;

        resultus = selectio_legere_ex_literis(":empty", piscina, intern);

        CREDO_VERUM(resultus.successus);
        simplex = *(SelectioSimplex**)xar_obtinere(resultus.selectio->composita->partes, 0);
        CREDO_AEQUALIS_I32(simplex->pseudo, PSEUDO_VACUUS);
    }

    /* Probatio: Parsare combinatorem descendentem div p */
    {
        SelectioResultus resultus;

        resultus = selectio_legere_ex_literis("div p", piscina, intern);

        CREDO_VERUM(resultus.successus);
        CREDO_NON_NIHIL(resultus.selectio->sequens);
        CREDO_AEQUALIS_I32(resultus.selectio->sequens->combinator, COMBINATOR_DESCENDENS);
    }

    /* Probatio: Parsare combinatorem liberi div > p */
    {
        SelectioResultus resultus;

        resultus = selectio_legere_ex_literis("div > p", piscina, intern);

        CREDO_VERUM(resultus.successus);
        CREDO_NON_NIHIL(resultus.selectio->sequens);
        CREDO_AEQUALIS_I32(resultus.selectio->sequens->combinator, COMBINATOR_LIBERUM);
    }

    /* Probatio: Parsare combinatorem fratris adjacentis div + p */
    {
        SelectioResultus resultus;

        resultus = selectio_legere_ex_literis("div + p", piscina, intern);

        CREDO_VERUM(resultus.successus);
        CREDO_NON_NIHIL(resultus.selectio->sequens);
        CREDO_AEQUALIS_I32(resultus.selectio->sequens->combinator, COMBINATOR_FRATER_ADJ);
    }

    /* Probatio: Parsare combinatorem fratris generalis div ~ p */
    {
        SelectioResultus resultus;

        resultus = selectio_legere_ex_literis("div ~ p", piscina, intern);

        CREDO_VERUM(resultus.successus);
        CREDO_NON_NIHIL(resultus.selectio->sequens);
        CREDO_AEQUALIS_I32(resultus.selectio->sequens->combinator, COMBINATOR_FRATER_GEN);
    }

    /* ==================================================
     * PROBATIONES CONGRUENTIAE
     * ================================================== */

    imprimere("\n--- Probans Selectio Matching ---\n");

    /* Probatio: Congruere selectorem tituli */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div><span>text</span></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "span", piscina, intern);
        CREDO_NON_NIHIL(found);
        CREDO_CHORDA_AEQUALIS_LITERIS(*found->titulus, "span");
    }

    /* Probatio: Congruere selectorem ID */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div><span id=\"target\">text</span></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "#target", piscina, intern);
        CREDO_NON_NIHIL(found);
        CREDO_CHORDA_AEQUALIS_LITERIS(*found->titulus, "span");
    }

    /* Probatio: Congruere selectorem classis cum labellis */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div><span labels=\"foo bar\">text</span></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, ".foo", piscina, intern);
        CREDO_NON_NIHIL(found);
        CREDO_CHORDA_AEQUALIS_LITERIS(*found->titulus, "span");

        found = stml_quaerere(root, ".bar", piscina, intern);
        CREDO_NON_NIHIL(found);

        found = stml_quaerere(root, ".baz", piscina, intern);
        CREDO_NIHIL(found);
    }

    /* Probatio: Congruere selectorem classis cum attributo classis (compatibilitas HTML) */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div><span class=\"highlight\">text</span></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, ".highlight", piscina, intern);
        CREDO_NON_NIHIL(found);
    }

    /* Probatio: Congruere selectorem existentiae attributi */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<form><input disabled/><input/></form>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "[disabled]", piscina, intern);
        CREDO_NON_NIHIL(found);
    }

    /* Probatio: Congruere selectorem attributi aequalis */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<form><input type=\"text\"/><input type=\"password\"/></form>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "[type=password]", piscina, intern);
        CREDO_NON_NIHIL(found);

        found = stml_quaerere(root, "[type=email]", piscina, intern);
        CREDO_NIHIL(found);
    }

    /* Probatio: Congruere selectorem compositum */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div><span id=\"x\" labels=\"a b\">1</span><span id=\"y\">2</span></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "span#x.a", piscina, intern);
        CREDO_NON_NIHIL(found);

        found = stml_quaerere(root, "span#x.c", piscina, intern);
        CREDO_NIHIL(found);
    }

    /* Probatio: Congruere combinatorem descendentem */
    {
        StmlNodus* root;
        Xar*       all;

        root = _legere_stml("<div><section><p>1</p></section><p>2</p></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        all = stml_quaerere_omnes(root, "div p", piscina, intern);
        CREDO_NON_NIHIL(all);
        CREDO_AEQUALIS_I32(xar_numerus(all), 2);
    }

    /* Probatio: Congruere combinatorem liberi */
    {
        StmlNodus* root;
        Xar*       all;

        root = _legere_stml("<div><section><p>nested</p></section><p>direct</p></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        all = stml_quaerere_omnes(root, "div > p", piscina, intern);
        CREDO_NON_NIHIL(all);
        CREDO_AEQUALIS_I32(xar_numerus(all), 1);
    }

    /* Probatio: Congruere combinatorem fratris adjacentis */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div><h1>Title</h1><p>First para</p><p>Second para</p></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "h1 + p", piscina, intern);
        CREDO_NON_NIHIL(found);
    }

    /* Probatio: Congruere combinatorem fratris generalis */
    {
        StmlNodus* root;
        Xar*       all;

        root = _legere_stml("<div><h1>Title</h1><p>1</p><p>2</p></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        all = stml_quaerere_omnes(root, "h1 ~ p", piscina, intern);
        CREDO_NON_NIHIL(all);
        CREDO_AEQUALIS_I32(xar_numerus(all), 2);
    }

    /* Probatio: Congruere pseudo-classem :first-child */
    {
        StmlNodus* root;
        StmlNodus* found;
        Xar*       all;

        root = _legere_stml("<ul><li>1</li><li>2</li><li>3</li></ul>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "li:first-child", piscina, intern);
        CREDO_NON_NIHIL(found);

        all = stml_quaerere_omnes(root, "li:first-child", piscina, intern);
        CREDO_AEQUALIS_I32(xar_numerus(all), 1);
    }

    /* Probatio: Congruere pseudo-classem :last-child */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<ul><li>1</li><li>2</li><li>3</li></ul>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "li:last-child", piscina, intern);
        CREDO_NON_NIHIL(found);
    }

    /* Probatio: Congruere pseudo-classem :empty */
    {
        StmlNodus* root;
        Xar*       all;

        root = _legere_stml("<div><span></span><span>text</span><span/></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        all = stml_quaerere_omnes(root, ":empty", piscina, intern);
        CREDO_NON_NIHIL(all);
        CREDO_AEQUALIS_I32(xar_numerus(all), 2);  /* Duo spans vacui */
    }

    /* Probatio: Nulla congruentia reddit xar vacuum */
    {
        StmlNodus* root;
        Xar*       all;

        root = _legere_stml("<div><span>text</span></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        all = stml_quaerere_omnes(root, "p", piscina, intern);
        CREDO_NON_NIHIL(all);
        CREDO_AEQUALIS_I32(xar_numerus(all), 0);
    }

    /* Probatio: Catena selectoris complexi */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml(
            "<div id=\"container\">"
            "  <ul labels=\"list\">"
            "    <li labels=\"item\">First</li>"
            "    <li labels=\"item active\">Second</li>"
            "  </ul>"
            "</div>",
            piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "#container .list .active", piscina, intern);
        CREDO_NON_NIHIL(found);
        CREDO_CHORDA_AEQUALIS_LITERIS(*found->titulus, "li");
    }

    /* Probatio: Congruere attributum incipit-cum */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div><a href=\"https://example.com\">link</a></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "[href^=https]", piscina, intern);
        CREDO_NON_NIHIL(found);
    }

    /* Probatio: Congruere attributum terminat-cum */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div><img src=\"photo.png\"/></div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "[src$=png]", piscina, intern);
        CREDO_NON_NIHIL(found);
    }

    /* Probatio: Congruere attributum continet */
    {
        StmlNodus* root;
        StmlNodus* found;

        root = _legere_stml("<div data-info=\"the quick brown fox\">text</div>", piscina, intern);
        CREDO_NON_NIHIL(root);

        found = stml_quaerere(root, "[data-info*=quick]", piscina, intern);
        CREDO_NON_NIHIL(found);
    }

    /* ==================================================
     * COMPENDIUM
     * ================================================== */

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
