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

hic_manens Piscina* PISCINA;

hic_manens chorda
_c (
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, PISCINA);
}

integer
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_html_alienum",
        65536);
    PISCINA = piscina;
    credo_aperire(piscina);

    imprimere("\n--- spatium proprium ---\n");
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(HTML_ALIENUM_NULLUM,
        HTML_ALIENUM_NULLUM, _c("svg")), (i32)HTML_ALIENUM_SVG);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(HTML_ALIENUM_NULLUM,
        HTML_ALIENUM_NULLUM, _c("MATH")),
        (i32)HTML_ALIENUM_MATHEMATICA);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(HTML_ALIENUM_NULLUM,
        HTML_ALIENUM_NULLUM, _c("div")), (i32)HTML_ALIENUM_NULLUM);
    /* liberi parentis alieni: hereditas */
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(HTML_ALIENUM_SVG,
        HTML_ALIENUM_SVG, _c("path")), (i32)HTML_ALIENUM_SVG);
    /* mglyph in puncto textus MathML (parens MathML, liberi HTML) */
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(
        HTML_ALIENUM_MATHEMATICA, HTML_ALIENUM_NULLUM, _c("mglyph")),
        (i32)HTML_ALIENUM_MATHEMATICA);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_proprium(
        HTML_ALIENUM_MATHEMATICA, HTML_ALIENUM_NULLUM, _c("b")),
        (i32)HTML_ALIENUM_NULLUM);

    imprimere("\n--- spatium liberorum ---\n");
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(HTML_ALIENUM_SVG,
        _c("foreignObject"), NIHIL), (i32)HTML_ALIENUM_NULLUM);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(HTML_ALIENUM_SVG,
        _c("DESC"), NIHIL), (i32)HTML_ALIENUM_NULLUM);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(HTML_ALIENUM_SVG,
        _c("g"), NIHIL), (i32)HTML_ALIENUM_SVG);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(
        HTML_ALIENUM_MATHEMATICA, _c("mi"), NIHIL),
        (i32)HTML_ALIENUM_NULLUM);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(
        HTML_ALIENUM_MATHEMATICA, _c("mrow"), NIHIL),
        (i32)HTML_ALIENUM_MATHEMATICA);
    /* annotation-xml sine elemento (attributa ignota) MathML */
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(
        HTML_ALIENUM_MATHEMATICA, _c("annotation-xml"), NIHIL),
        (i32)HTML_ALIENUM_MATHEMATICA);
    CREDO_AEQUALIS_I32 ((i32)html_alienum_liberorum(HTML_ALIENUM_NULLUM,
        _c("svg"), NIHIL), (i32)HTML_ALIENUM_NULLUM);

    imprimere("\n--- tituli et attributa ---\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_titulus(piscina,
        _c("foreignobject"), HTML_ALIENUM_SVG), "foreignObject");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_titulus(piscina,
        _c("foreignobject"), HTML_ALIENUM_NULLUM), "foreignobject");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_titulus(piscina,
        _c("path"), HTML_ALIENUM_SVG), "path");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c("viewbox"), HTML_ALIENUM_SVG), "viewBox");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c("viewbox"), HTML_ALIENUM_NULLUM), "viewbox");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c("definitionurl"), HTML_ALIENUM_MATHEMATICA),
        "definitionURL");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c("xlink:href"), HTML_ALIENUM_SVG), "xlink href");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c("xmlns"), HTML_ALIENUM_SVG), "xmlns xmlns");
    CREDO_CHORDA_AEQUALIS_LITERIS (html_alienum_attributum(piscina,
        _c("xlink:href"), HTML_ALIENUM_NULLUM), "xlink:href");

    imprimere("\n--- rumpentia ---\n");
    CREDO_VERUM (html_alienum_rumpit(_c("p")));
    CREDO_VERUM (html_alienum_rumpit(_c("H1")));
    CREDO_VERUM (html_alienum_rumpit(_c("table")));
    CREDO_FALSUM (html_alienum_rumpit(_c("g")));
    CREDO_FALSUM (html_alienum_rumpit(_c("font")));
    CREDO_FALSUM (html_alienum_rumpit(_c("span1")));

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
