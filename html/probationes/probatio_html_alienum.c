/* probatio_html_alienum.c - Spatia nominum aliena (O2b-6)
 *
 * Tabulae communes aedificatoris et visionis coctae: spatium
 * proprium ex parente (svg/math aperiunt; mglyph in puncto textus
 * MathML manet; alieni hereditant), spatium liberorum (puncta
 * integrationis HTML), tituli svg camelCase, attributa accommodata
 * (svg, definitionURL, praefixa spatiorum), tabula rumpentium.
 * Litteris neglectis ubique (lexator titulos non plicat).
 */

#include "latina.h"
#include "credo.h"
#include "html_alienum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens chorda
_c (
               Piscina* piscina,
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, piscina);
}


interior vacuum
_spatium_proprium_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(HTML_ALIENUM_NULLUM,
        HTML_ALIENUM_NULLUM, _c(piscina, "svg")),
        (i32)HTML_ALIENUM_SVG);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(HTML_ALIENUM_NULLUM,
        HTML_ALIENUM_NULLUM, _c(piscina, "MATH")),
        (i32)HTML_ALIENUM_MATHEMATICA);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(HTML_ALIENUM_NULLUM,
        HTML_ALIENUM_NULLUM, _c(piscina, "div")),
        (i32)HTML_ALIENUM_NULLUM);
    /* liberi parentis alieni: hereditas */
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(HTML_ALIENUM_SVG,
        HTML_ALIENUM_SVG, _c(piscina, "path")), (i32)HTML_ALIENUM_SVG);
    /* mglyph in puncto textus MathML (parens MathML, liberi HTML) */
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(
        HTML_ALIENUM_MATHEMATICA, HTML_ALIENUM_NULLUM, _c(piscina,
        "mglyph")),
        (i32)HTML_ALIENUM_MATHEMATICA);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(
        HTML_ALIENUM_MATHEMATICA, HTML_ALIENUM_NULLUM, _c(piscina,
        "b")),
        (i32)HTML_ALIENUM_NULLUM);
}


interior vacuum
_spatium_liberorum_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(HTML_ALIENUM_SVG,
        _c(piscina, "foreignObject"), NIHIL), (i32)HTML_ALIENUM_NULLUM);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(HTML_ALIENUM_SVG,
        _c(piscina, "DESC"), NIHIL), (i32)HTML_ALIENUM_NULLUM);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(HTML_ALIENUM_SVG,
        _c(piscina, "g"), NIHIL), (i32)HTML_ALIENUM_SVG);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(
        HTML_ALIENUM_MATHEMATICA, _c(piscina, "mi"), NIHIL),
        (i32)HTML_ALIENUM_NULLUM);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(
        HTML_ALIENUM_MATHEMATICA, _c(piscina, "mrow"), NIHIL),
        (i32)HTML_ALIENUM_MATHEMATICA);
    /* annotation-xml sine elemento (attributa ignota) MathML */
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(
        HTML_ALIENUM_MATHEMATICA, _c(piscina, "annotation-xml"), NIHIL),
        (i32)HTML_ALIENUM_MATHEMATICA);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(HTML_ALIENUM_NULLUM,
        _c(piscina, "svg"), NIHIL), (i32)HTML_ALIENUM_NULLUM);
}


interior vacuum
_titulos_et_attributa_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_titulus(piscina,
        _c(piscina, "foreignobject"), HTML_ALIENUM_SVG),
        "foreignObject");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_titulus(piscina,
        _c(piscina, "foreignobject"), HTML_ALIENUM_NULLUM),
        "foreignobject");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_titulus(piscina,
        _c(piscina, "path"), HTML_ALIENUM_SVG), "path");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c(piscina, "viewbox"), HTML_ALIENUM_SVG), "viewBox");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c(piscina, "viewbox"), HTML_ALIENUM_NULLUM), "viewbox");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c(piscina, "definitionurl"), HTML_ALIENUM_MATHEMATICA),
        "definitionURL");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c(piscina, "xlink:href"), HTML_ALIENUM_SVG), "xlink href");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c(piscina, "xmlns"), HTML_ALIENUM_SVG), "xmlns xmlns");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c(piscina, "xlink:href"), HTML_ALIENUM_NULLUM), "xlink:href");
}


interior vacuum
_rumpentia_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    CREDO_VERUM (html_alienum_rumpit(_c(piscina, "p")));
    CREDO_VERUM (html_alienum_rumpit(_c(piscina, "H1")));
    CREDO_VERUM (html_alienum_rumpit(_c(piscina, "table")));
    CREDO_FALSUM (html_alienum_rumpit(_c(piscina, "g")));
    CREDO_FALSUM (html_alienum_rumpit(_c(piscina, "font")));
    CREDO_FALSUM (html_alienum_rumpit(_c(piscina, "span1")));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "spatium proprium", _spatium_proprium_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "spatium liberorum", _spatium_liberorum_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "tituli et attributa", _titulos_et_attributa_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "rumpentia", _rumpentia_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("html_alienum", SECTIONES);
}
