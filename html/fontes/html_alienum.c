/* html_alienum.c - Vide html_alienum.h. */

#include "html_alienum.h"
#include "html_registrum.h"
#include "materia_token.h"
#include <string.h>

/* WHATWG 'adjust SVG tag names': minusculus -> accommodatus */
hic_manens constans character* constans SVG_TITULI[][2] = {
    { "altglyph",            "altGlyph" },
    { "altglyphdef",         "altGlyphDef" },
    { "altglyphitem",        "altGlyphItem" },
    { "animatecolor",        "animateColor" },
    { "animatemotion",       "animateMotion" },
    { "animatetransform",    "animateTransform" },
    { "clippath",            "clipPath" },
    { "feblend",             "feBlend" },
    { "fecolormatrix",       "feColorMatrix" },
    { "fecomponenttransfer", "feComponentTransfer" },
    { "fecomposite",         "feComposite" },
    { "feconvolvematrix",    "feConvolveMatrix" },
    { "fediffuselighting",   "feDiffuseLighting" },
    { "fedisplacementmap",   "feDisplacementMap" },
    { "fedistantlight",      "feDistantLight" },
    { "fedropshadow",        "feDropShadow" },
    { "feflood",             "feFlood" },
    { "fefunca",             "feFuncA" },
    { "fefuncb",             "feFuncB" },
    { "fefuncg",             "feFuncG" },
    { "fefuncr",             "feFuncR" },
    { "fegaussianblur",      "feGaussianBlur" },
    { "feimage",             "feImage" },
    { "femerge",             "feMerge" },
    { "femergenode",         "feMergeNode" },
    { "femorphology",        "feMorphology" },
    { "feoffset",            "feOffset" },
    { "fepointlight",        "fePointLight" },
    { "fespecularlighting",  "feSpecularLighting" },
    { "fespotlight",         "feSpotLight" },
    { "fetile",              "feTile" },
    { "feturbulence",        "feTurbulence" },
    { "foreignobject",       "foreignObject" },
    { "glyphref",            "glyphRef" },
    { "lineargradient",      "linearGradient" },
    { "radialgradient",      "radialGradient" },
    { "textpath",            "textPath" }
};

/* WHATWG 'adjust SVG attributes' */
hic_manens constans character* constans SVG_ATTRIBUTA[][2] = {
    { "attributename",       "attributeName" },
    { "attributetype",       "attributeType" },
    { "basefrequency",       "baseFrequency" },
    { "baseprofile",         "baseProfile" },
    { "calcmode",            "calcMode" },
    { "clippathunits",       "clipPathUnits" },
    { "diffuseconstant",     "diffuseConstant" },
    { "edgemode",            "edgeMode" },
    { "filterunits",         "filterUnits" },
    { "glyphref",            "glyphRef" },
    { "gradienttransform",   "gradientTransform" },
    { "gradientunits",       "gradientUnits" },
    { "kernelmatrix",        "kernelMatrix" },
    { "kernelunitlength",    "kernelUnitLength" },
    { "keypoints",           "keyPoints" },
    { "keysplines",          "keySplines" },
    { "keytimes",            "keyTimes" },
    { "lengthadjust",        "lengthAdjust" },
    { "limitingconeangle",   "limitingConeAngle" },
    { "markerheight",        "markerHeight" },
    { "markerunits",         "markerUnits" },
    { "markerwidth",         "markerWidth" },
    { "maskcontentunits",    "maskContentUnits" },
    { "maskunits",           "maskUnits" },
    { "numoctaves",          "numOctaves" },
    { "pathlength",          "pathLength" },
    { "patterncontentunits", "patternContentUnits" },
    { "patterntransform",    "patternTransform" },
    { "patternunits",        "patternUnits" },
    { "pointsatx",           "pointsAtX" },
    { "pointsaty",           "pointsAtY" },
    { "pointsatz",           "pointsAtZ" },
    { "preservealpha",       "preserveAlpha" },
    { "preserveaspectratio", "preserveAspectRatio" },
    { "primitiveunits",      "primitiveUnits" },
    { "refx",                "refX" },
    { "refy",                "refY" },
    { "repeatcount",         "repeatCount" },
    { "repeatdur",           "repeatDur" },
    { "requiredextensions",  "requiredExtensions" },
    { "requiredfeatures",    "requiredFeatures" },
    { "specularconstant",    "specularConstant" },
    { "specularexponent",    "specularExponent" },
    { "spreadmethod",        "spreadMethod" },
    { "startoffset",         "startOffset" },
    { "stddeviation",        "stdDeviation" },
    { "stitchtiles",         "stitchTiles" },
    { "surfacescale",        "surfaceScale" },
    { "systemlanguage",      "systemLanguage" },
    { "tablevalues",         "tableValues" },
    { "targetx",             "targetX" },
    { "targety",             "targetY" },
    { "textlength",          "textLength" },
    { "viewbox",             "viewBox" },
    { "viewtarget",          "viewTarget" },
    { "xchannelselector",    "xChannelSelector" },
    { "ychannelselector",    "yChannelSelector" },
    { "zoomandpan",          "zoomAndPan" }
};

/* WHATWG 'in foreign content': tag apertionis quod contentum
 * alienum rumpit */
hic_manens constans character* constans RUMPENTIA[] = {
    "b", "big", "blockquote", "body", "br", "center", "code", "dd",
    "div", "dl", "dt", "em", "embed", "h1", "h2", "h3", "h4", "h5",
        "h6",
    "head", "hr", "i", "img", "li", "listing", "menu", "meta", "nobr",
    "ol", "p", "pre", "ruby", "s", "small", "span", "strong", "strike",
    "sub", "sup", "table", "tt", "u", "ul", "var"
};

#define TABULAE_NUMERUS(t) ((i32)(magnitudo(t) / magnitudo((t)[0])))

interior chorda
_copia_literis (
               Piscina* piscina,
    constans character* litterae)
{
    chorda c;
       i32 n = (i32)strlen(litterae);

    c.datum    = (i8*)piscina_allocare(piscina, (memoriae_index)n + I);
    c.mensura  = ZEPHYRUM;
    si (c.datum == NIHIL)
    {
        redde c;
    }
    memcpy(c.datum, litterae, (size_t)n + I);
    c.mensura = n;
    redde c;
}

interior i8
_minuscula (
    i8 c)
{
    si (c >= 'A' && c <= 'Z')
    {
        redde (i8)(c + ('a' - 'A'));
    }
    redde c;
}

/* chorda == litterae litteris neglectis (ASCII) */
interior b32
_aequalis_neglectis (
                 chorda  c,
     constans character* litterae)
{
    i32 n = (i32)strlen(litterae);
    i32 i;

    si (c.mensura != n)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si ((character)_minuscula(c.datum[i]) != litterae[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_incipit_literis (
                 chorda  c,
     constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde (b32)(c.mensura >= n
                && memcmp(c.datum, litterae, (size_t)n) == ZEPHYRUM);
}

interior MateriaToken*
_tok (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (nodus->loci[locus].genus == MATERIA_VALOR_TOKEN)
        ? nodus->loci[locus].datum.token : NIHIL;
}

/* attributum 'encoding' annotation-xml: text/html |
 * application/xhtml+xml (citationes exutae, sine copia) */
interior b32
_encoding_html (
    constans MateriaNodus* elementum)
{
    constans MateriaValor* lista =
        &elementum->loci[HTML_ELEMENTUM_ATTRIBUTA];
                      i32 n;
                      i32 i;

    si (lista->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    n = materia_valor_lista_numerus(*lista);
    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaValor* v = materia_valor_lista_obtinere(*lista, i);
        MateriaNodus* a;
        MateriaToken* titulus;
        MateriaToken* valor;
              chorda  c;

        si (v == NIHIL || v->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        a        = v->datum.nodus;
        titulus  = _tok(a, (i32)HTML_ATTRIBUTUM_TOK_NOMEN);
        valor    = _tok(a, (i32)HTML_ATTRIBUTUM_TOK_VALOR);
        si (   titulus == NIHIL || valor == NIHIL
            || !_aequalis_neglectis(titulus->valor, "encoding"))
        {
            perge;
        }
        c = valor->valor;
        si (   c.mensura              >= II
            && (c.datum[ZEPHYRUM] == '"' || c.datum[ZEPHYRUM] == '\'')
            && c.datum[c.mensura - I] == c.datum[ZEPHYRUM])
        {
            c.datum    = c.datum + I;
            c.mensura  = c.mensura - II;
        }
        redde (b32)(_aequalis_neglectis(c, "text/html")
            || _aequalis_neglectis(c, "application/xhtml+xml"));
    }
    redde FALSUM;
}

interior b32
_punctum_textus_mathematici (
    chorda titulus)
{
    redde (b32)(_aequalis_neglectis(titulus, "mi")
                || _aequalis_neglectis(titulus, "mo")
                || _aequalis_neglectis(titulus, "mn")
                || _aequalis_neglectis(titulus, "ms")
                || _aequalis_neglectis(titulus, "mtext"));
}

HtmlAlienum
html_alienum_proprium (
    HtmlAlienum parentis,
    HtmlAlienum liberorum_parentis,
         chorda titulus)
{
    si (liberorum_parentis != HTML_ALIENUM_NULLUM)
    {
        redde liberorum_parentis;
    }
    si (_aequalis_neglectis(titulus, "svg"))
    {
        redde HTML_ALIENUM_SVG;
    }
    si (_aequalis_neglectis(titulus, "math"))
    {
        redde HTML_ALIENUM_MATHEMATICA;
    }
    /* in puncto integrationis textus MathML mglyph/malignmark MathML
     * manent (spec 'in body', tag mglyph/malignmark) */
    si (   parentis == HTML_ALIENUM_MATHEMATICA
        && (_aequalis_neglectis(titulus, "mglyph")
            || _aequalis_neglectis(titulus, "malignmark")))
    {
        redde HTML_ALIENUM_MATHEMATICA;
    }
    redde HTML_ALIENUM_NULLUM;
}

HtmlAlienum
html_alienum_liberorum (
              HtmlAlienum  proprium,
                   chorda  titulus,
    constans MateriaNodus* elementum)
{
    si (proprium == HTML_ALIENUM_NULLUM)
    {
        redde HTML_ALIENUM_NULLUM;
    }
    si (proprium == HTML_ALIENUM_SVG)
    {
        si (   _aequalis_neglectis(titulus, "foreignobject")
            || _aequalis_neglectis(titulus, "desc")
            || _aequalis_neglectis(titulus, "title"))
        {
            redde HTML_ALIENUM_NULLUM;
        }
        redde HTML_ALIENUM_SVG;
    }
    si (_punctum_textus_mathematici(titulus))
    {
        redde HTML_ALIENUM_NULLUM;
    }
    si (   _aequalis_neglectis(titulus, "annotation-xml")
        && elementum != NIHIL && _encoding_html(elementum))
    {
        redde HTML_ALIENUM_NULLUM;
    }
    redde HTML_ALIENUM_MATHEMATICA;
}

chorda
html_alienum_titulus (
       Piscina* piscina,
        chorda  minusculus,
    HtmlAlienum  proprium)
{
    i32 k;

    si (proprium != HTML_ALIENUM_SVG || minusculus.datum == NIHIL)
    {
        redde minusculus;
    }
    per (k = ZEPHYRUM; k < TABULAE_NUMERUS(SVG_TITULI); k++)
    {
        si (_aequalis_neglectis(minusculus,
            SVG_TITULI[k][ZEPHYRUM]))
        {
            redde _copia_literis(piscina, SVG_TITULI[k][I]);
        }
    }
    redde minusculus;
}

chorda
html_alienum_attributum (
       Piscina* piscina,
        chorda  minusculus,
    HtmlAlienum  proprium)
{
    i32 k;

    si (proprium == HTML_ALIENUM_NULLUM || minusculus.datum == NIHIL)
    {
        redde minusculus;
    }
    si (proprium == HTML_ALIENUM_SVG)
    {
        per (k = ZEPHYRUM; k < TABULAE_NUMERUS(SVG_ATTRIBUTA); k++)
        {
            si (_aequalis_neglectis(minusculus,
                    SVG_ATTRIBUTA[k][ZEPHYRUM]))
            {
                redde _copia_literis(piscina, SVG_ATTRIBUTA[k][I]);
            }
        }
    }
    alioquin si (_aequalis_neglectis(minusculus, "definitionurl"))
    {
        redde _copia_literis(piscina, "definitionURL");
    }
    si (_aequalis_neglectis(minusculus, "xmlns"))
    {
        redde _copia_literis(piscina, "xmlns xmlns");
    }
    si (   _incipit_literis(minusculus, "xlink:")
        || _incipit_literis(minusculus, "xml:")
        || _incipit_literis(minusculus, "xmlns:"))
    {
        chorda c = _copia_literis(piscina, "");
           i32 i;

        c.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)minusculus.mensura + I);
        si (c.datum == NIHIL)
        {
            redde minusculus;
        }
        memcpy(c.datum, minusculus.datum, (size_t)minusculus.mensura);
        c.datum[minusculus.mensura]  = ZEPHYRUM;
        c.mensura                    = minusculus.mensura;
        per (i = ZEPHYRUM; i < c.mensura; i++)
        {
            si (c.datum[i] == ':')
            {
                c.datum[i] = ' ';
                frange;
            }
        }
        redde c;
    }
    redde minusculus;
}

b32
html_alienum_rumpit (
    chorda titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < TABULAE_NUMERUS(RUMPENTIA); k++)
    {
        si (_aequalis_neglectis(titulus, RUMPENTIA[k]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}
