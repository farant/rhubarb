/* html_coctum.c - Vide html_coctum.h. */

#include "html_coctum.h"
#include "html_registrum.h"
#include "html_lexema.h"
#include "materia_token.h"
#include "md_decoctum.h"
#include "chorda_aedificator.h"
#include <string.h>

#define ATTRIBUTA_MAXIMA ((i32)64)

nomen structura {
              Piscina* piscina;
    ChordaAedificator* aed;
    /* nondum linea ulla scripta: '\n' ante lineam omittendum */
    b32 prima;
} Scriptor;

nomen structura {
    chorda titulus;
    chorda valor;
} Attributum;

/* Tituli svg quorum litterae maiusculae in DOM restituuntur (WHATWG
 * 'adjust SVG tag names'): minusculus -> accommodatus. */
hic_manens constans character* constans SVG_ACCOMMODATI[][2] = {
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

#define SVG_ACCOMMODATI_NUMERUS \
    ((i32)(magnitudo(SVG_ACCOMMODATI) / magnitudo(SVG_ACCOMMODATI[0])))


/* ==================================================
 * Adiumenta
 * ================================================== */

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

interior chorda
_copia_minuscula (
    Piscina* piscina,
     chorda  c)
{
    chorda d;
       i32 i;

    d.datum    = (i8*)piscina_allocare(piscina,
        (memoriae_index)c.mensura + I);
    d.mensura  = ZEPHYRUM;
    si (d.datum == NIHIL)
    {
        redde d;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        d.datum[i] = _minuscula(c.datum[i]);
    }
    d.datum[c.mensura]  = ZEPHYRUM;
    d.mensura           = c.mensura;
    redde d;
}

interior b32
_ante (
    chorda a,
    chorda b)
{
        i32 n = a.mensura < b.mensura ? a.mensura : b.mensura;
    integer r = memcmp(a.datum, b.datum, (size_t)n);

    si (r != ZEPHYRUM)
    {
        redde (b32)(r < ZEPHYRUM);
    }
    redde (b32)(a.mensura < b.mensura);
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

interior b32
_terminatur_literis (
                 chorda  c,
     constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde (b32)(c.mensura >= n
                && memcmp(c.datum + (c.mensura - n), litterae,
                (size_t)n)
                    == ZEPHYRUM);
}

interior b32
_spatium_est (
    i8 c)
{
    redde (b32)(c == ' ' || c == '\t' || c == '\n' || c == '\r'
                || c == '\f');
}

interior MateriaToken*
_tok (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde (nodus->loci[locus].genus == MATERIA_VALOR_TOKEN)
        ? nodus->loci[locus].datum.token : NIHIL;
}

interior i32
_numerus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(nodus->loci[locus]);
}

interior MateriaNodus*
_liber (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v;

    si (nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(nodus->loci[locus], i);
    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS)
        ? v->datum.nodus : NIHIL;
}

/* Octetos textus appendere: CR LF -> LF, CR -> LF (praeparatio fluxi
 * HTML5); '&...;' decoctum si iussum (referentia ignota cruda manet);
 * NUL aut deletus (textus corporis HTML: 'in body' eum ignorat) aut
 * U+FFFD (lexator: textus crudus, alieni, attributa, commentaria). */
interior vacuum
_textum_appendere (
    ChordaAedificator* aed,
               chorda  c,
                  b32  decoquere,
                  b32  nul_delere)
{
    s32 i = ZEPHYRUM;

    dum (i < (s32)c.mensura)
    {
        i8 ch = c.datum[i];

        si (ch == ZEPHYRUM)
        {
            si (!nul_delere)
            {
                chorda_aedificator_appendere_literis(aed,
                    "\xEF\xBF\xBD");
            }
            i = i + I;
            perge;
        }
        si (ch == '\r')
        {
            chorda_aedificator_appendere_character(aed, '\n');
            i = i + I;
            si (i < (s32)c.mensura && c.datum[i] == '\n')
            {
                i = i + I;
            }
            perge;
        }
        si (ch == '&' && decoquere)
        {
            character exitus[XVI];
                  i32 longitudo;
                  s32 post;

            si (md_ens_decoquere((constans character*)c.datum, i,
                    (s32)c.mensura, exitus, &longitudo, &post))
            {
                chorda d;

                d.datum    = (i8*)exitus;
                d.mensura  = longitudo;
                chorda_aedificator_appendere_chorda(aed, d);
                i = post;
                perge;
            }
        }
        chorda_aedificator_appendere_character(aed, (character)ch);
        i = i + I;
    }
}

/* Valor attributi coctus: citationes exutae ('"x"', "'x'", citatus ad
 * EOF sine clausura), deinde textus normatus et decoctus. */
interior chorda
_valor_coctus (
    Piscina* piscina,
     chorda  v)
{
    ChordaAedificator* aed;

    si (   v.mensura >= I
        && (v.datum[ZEPHYRUM] == '"' || v.datum[ZEPHYRUM] == '\''))
    {
        si (   v.mensura              >= II
            && v.datum[v.mensura - I] == v.datum[ZEPHYRUM])
        {
            v.mensura = v.mensura - II;
        }
        alioquin
        {
            v.mensura = v.mensura - I;
        }
        v.datum = v.datum + I;
    }
    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)v.mensura + XVI);
    _textum_appendere(aed, v, VERUM, FALSUM);
    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Tituli et contentum alienum
 * ================================================== */

chorda
html_coctum_titulus (
       Piscina* piscina,
        chorda  crudus,
    HtmlAlienum  alienum)
{
    chorda titulus = _copia_minuscula(piscina, crudus);
       i32 k;

    si (alienum != HTML_ALIENUM_SVG || titulus.datum == NIHIL)
    {
        redde titulus;
    }
    per (k = ZEPHYRUM; k < SVG_ACCOMMODATI_NUMERUS; k++)
    {
        si (chorda_aequalis_literis(titulus,
            SVG_ACCOMMODATI[k][ZEPHYRUM]))
        {
            redde _copia_literis(piscina, SVG_ACCOMMODATI[k][I]);
        }
    }
    redde titulus;
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

/* attributum 'encoding' annotation-xml: text/html |
 * application/xhtml+xml (citationes exutae, sine copia) */
interior b32
_encoding_html (
    constans MateriaNodus* elementum)
{
    i32 n = _numerus(elementum, (i32)HTML_ELEMENTUM_ATTRIBUTA);
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaNodus* a = _liber(elementum,
            (i32)HTML_ELEMENTUM_ATTRIBUTA,
            i);
        MateriaToken* titulus;
        MateriaToken* valor;
              chorda  v;

        si (a == NIHIL)
        {
            perge;
        }
        titulus  = _tok(a, (i32)HTML_ATTRIBUTUM_TOK_NOMEN);
        valor    = _tok(a, (i32)HTML_ATTRIBUTUM_TOK_VALOR);
        si (   titulus == NIHIL || valor == NIHIL
            || !_aequalis_neglectis(titulus->valor, "encoding"))
        {
            perge;
        }
        v = valor->valor;
        si (   v.mensura              >= II
            && (v.datum[ZEPHYRUM] == '"' || v.datum[ZEPHYRUM] == '\'')
            && v.datum[v.mensura - I] == v.datum[ZEPHYRUM])
        {
            v.datum    = v.datum + I;
            v.mensura  = v.mensura - II;
        }
        redde (b32)(_aequalis_neglectis(v, "text/html")
            || _aequalis_neglectis(v, "application/xhtml+xml"));
    }
    redde FALSUM;
}

HtmlAlienum
html_coctum_alienum_liberorum (
              HtmlAlienum  parentis,
                   chorda  titulus,
    constans MateriaNodus* elementum)
{
    si (parentis == HTML_ALIENUM_NULLUM)
    {
        si (chorda_aequalis_literis(titulus, "svg"))
        {
            redde HTML_ALIENUM_SVG;
        }
        si (chorda_aequalis_literis(titulus, "math"))
        {
            redde HTML_ALIENUM_MATHEMATICA;
        }
        redde HTML_ALIENUM_NULLUM;
    }
    si (parentis == HTML_ALIENUM_SVG)
    {
        si (   chorda_aequalis_literis(titulus, "foreignObject")
            || chorda_aequalis_literis(titulus, "desc")
            || chorda_aequalis_literis(titulus, "title"))
        {
            redde HTML_ALIENUM_NULLUM;
        }
        redde HTML_ALIENUM_SVG;
    }
    si (   chorda_aequalis_literis(titulus, "mi")
        || chorda_aequalis_literis(titulus, "mo")
        || chorda_aequalis_literis(titulus, "mn")
        || chorda_aequalis_literis(titulus, "ms")
        || chorda_aequalis_literis(titulus, "mtext"))
    {
        redde HTML_ALIENUM_NULLUM;
    }
    si (   chorda_aequalis_literis(titulus, "annotation-xml")
        && elementum != NIHIL && _encoding_html(elementum))
    {
        redde HTML_ALIENUM_NULLUM;
    }
    redde HTML_ALIENUM_MATHEMATICA;
}

/* Titulus attributi coctus: minusculus; in alienis praefixa
 * 'xlink:' 'xml:' 'xmlns:' in spatia nominum ('xlink href'), 'xmlns'
 * nudum 'xmlns xmlns'. */
interior chorda
_titulus_attributi (
        Piscina* piscina,
         chorda  crudus,
    HtmlAlienum  alienum)
{
    chorda minusculus = _copia_minuscula(piscina, crudus);

    si (alienum == HTML_ALIENUM_NULLUM || minusculus.datum == NIHIL)
    {
        redde minusculus;
    }
    si (chorda_aequalis_literis(minusculus, "xmlns"))
    {
        redde _copia_literis(piscina, "xmlns xmlns");
    }
    si (   _incipit_literis(minusculus, "xlink:")
        || _incipit_literis(minusculus, "xml:")
        || _incipit_literis(minusculus, "xmlns:"))
    {
        i32 i;

        per (i = ZEPHYRUM; i < minusculus.mensura; i++)
        {
            si (minusculus.datum[i] == ':')
            {
                minusculus.datum[i] = ' ';
                frange;
            }
        }
    }
    redde minusculus;
}


/* ==================================================
 * Scriptor
 * ================================================== */

interior vacuum
_lineam_incipere (
    Scriptor* s,
         i32  gradus)
{
    i32 i;

    si (!s->prima)
    {
        chorda_aedificator_appendere_character(s->aed, '\n');
    }
    s->prima = FALSUM;
    chorda_aedificator_appendere_literis(s->aed, "| ");
    per (i = ZEPHYRUM; i < gradus * II; i++)
    {
        chorda_aedificator_appendere_character(s->aed, ' ');
    }
}

/* Textum in cumulum (natum in primo) appendere */
interior vacuum
_cumulare (
              Scriptor*  s,
     ChordaAedificator** cumulus,
                chorda   c,
                   b32   decoquere,
                   b32   nul_delere)
{
    si (*cumulus == NIHIL)
    {
        *cumulus = chorda_aedificator_creare(s->piscina,
            (memoriae_index)c.mensura + LXIV);
        si (*cumulus == NIHIL)
        {
            redde;
        }
    }
    _textum_appendere(*cumulus, c, decoquere, nul_delere);
}

/* Cumulum ut nodum textus scribere; linea nova prima dempta si
 * iussum (pre/listing/textarea, liber primus) - textus tum vacuus
 * nodum nullum dat. */
interior vacuum
_cumulum_effundere (
              Scriptor*  s,
     ChordaAedificator** cumulus,
                   i32   gradus,
                   b32   lf_demendum)
{
    chorda t;

    si (*cumulus == NIHIL)
    {
        redde;
    }
    t         = chorda_aedificator_finire(*cumulus);
    *cumulus  = NIHIL;
    si (lf_demendum && t.mensura >= I && t.datum[ZEPHYRUM] == '\n')
    {
        t.datum    = t.datum + I;
        t.mensura  = t.mensura - I;
        si (t.mensura == ZEPHYRUM)
        {
            redde;
        }
    }
    _lineam_incipere(s, gradus);
    chorda_aedificator_appendere_character(s->aed, '"');
    chorda_aedificator_appendere_chorda(s->aed, t);
    chorda_aedificator_appendere_character(s->aed, '"');
}

/* '<!-- datum -->': datum ex lexemate commentarii - '<!--' ... '-->'
 * ('<!-->' et '<!--->' vacua, imperfectum ad EOF sine '-->'); prava
 * '<!x>' '</x>' datum post II, '<?x>' post I ('?' servatur). */
interior vacuum
_commentarium_scribere (
    Scriptor* s,
      chorda  v,
         i32  gradus)
{
    chorda datum = v;

    si (_incipit_literis(v, "<!--"))
    {
        datum.datum    = v.datum + IV;
        datum.mensura  = v.mensura - IV;
        si (   chorda_aequalis_literis(datum, ">")
            || chorda_aequalis_literis(datum, "->"))
        {
            datum.mensura = ZEPHYRUM;
        }
        alioquin si (_terminatur_literis(datum, "--!>"))
        {
            datum.mensura = datum.mensura - IV;   /* clausura prava */
        }
        alioquin si (_terminatur_literis(datum, "-->"))
        {
            datum.mensura = datum.mensura - III;
        }
    }
    alioquin si (_incipit_literis(v, "<!") || _incipit_literis(v, "</"))
    {
        datum.datum    = v.datum + II;
        datum.mensura  = v.mensura - II;
        si (_terminatur_literis(datum, ">"))
        {
            datum.mensura = datum.mensura - I;
        }
    }
    alioquin si (_incipit_literis(v, "<?"))
    {
        datum.datum    = v.datum + I;
        datum.mensura  = v.mensura - I;
        si (_terminatur_literis(datum, ">"))
        {
            datum.mensura = datum.mensura - I;
        }
    }
    _lineam_incipere(s, gradus);
    chorda_aedificator_appendere_literis(s->aed, "<!-- ");
    _textum_appendere(s->aed, datum, FALSUM, FALSUM);
    chorda_aedificator_appendere_literis(s->aed, " -->");
}

/* Chorda citata ab *i ('"' aut '\''): valor sine citationibus; *i post
 * citationem clausam. Vacua si citatio nulla. */
interior chorda
_citatum (
    chorda  v,
       s32* i)
{
    chorda c;
        i8 q;
       s32 a;
       s32 k;

    c.datum    = v.datum;
    c.mensura  = ZEPHYRUM;
    si (*i >= (s32)v.mensura)
    {
        redde c;
    }
    q = v.datum[*i];
    si (q != '"' && q != '\'')
    {
        redde c;
    }
    a = *i + I;
    k = a;
    dum (k < (s32)v.mensura && v.datum[k] != q && v.datum[k] != '>')
    {
        k = k + I;
    }
    c.datum    = v.datum + a;
    c.mensura  = (i32)(k - a);
    *i         = (k < (s32)v.mensura && v.datum[k] == q) ? k + I : k;
    redde c;
}

interior b32
_litterae_ibi (
                chorda  v,
                   s32  i,
    constans character* litterae)
{
    i32 n = (i32)strlen(litterae);
    i32 k;

    si (i + (s32)n > (s32)v.mensura)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si ((character)_minuscula(v.datum[i + (s32)k]) != litterae[k])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* '<!DOCTYPE titulus>' aut '<!DOCTYPE titulus "publicus" "systema">'
 * (html5lib: identificatores scripti si alteruter non vacat). Titulus
 * minusculus (lexator HTML5 eum plicat), identificatores crudi. */
interior vacuum
_doctype_scribere (
    Scriptor* s,
      chorda  v,
         i32  gradus)
{
       s32 i = II;
       s32 a;
    chorda titulus;
    chorda publicus;
    chorda systema;

    publicus.datum    = v.datum;
    publicus.mensura  = ZEPHYRUM;
    systema           = publicus;
    si (_litterae_ibi(v, i, "doctype"))
    {
        i = i + VII;
    }
    dum (i < (s32)v.mensura && _spatium_est(v.datum[i]))
    {
        i = i + I;
    }
    a = i;
    dum (   i < (s32)v.mensura && !_spatium_est(v.datum[i])
         && v.datum[i] != '>')
    {
        i = i + I;
    }
    titulus.datum    = v.datum + a;
    titulus.mensura  = (i32)(i - a);
    titulus          = _copia_minuscula(s->piscina, titulus);
    dum (i < (s32)v.mensura && _spatium_est(v.datum[i]))
    {
        i = i + I;
    }
    si (_litterae_ibi(v, i, "public"))
    {
        i = i + VI;
        dum (i < (s32)v.mensura && _spatium_est(v.datum[i]))
        {
            i = i + I;
        }
        publicus = _citatum(v, &i);
        dum (i < (s32)v.mensura && _spatium_est(v.datum[i]))
        {
            i = i + I;
        }
        systema = _citatum(v, &i);
    }
    alioquin si (_litterae_ibi(v, i, "system"))
    {
        i = i + VI;
        dum (i < (s32)v.mensura && _spatium_est(v.datum[i]))
        {
            i = i + I;
        }
        systema = _citatum(v, &i);
    }
    _lineam_incipere(s, gradus);
    chorda_aedificator_appendere_literis(s->aed, "<!DOCTYPE ");
    chorda_aedificator_appendere_chorda(s->aed, titulus);
    si (publicus.mensura > ZEPHYRUM || systema.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(s->aed, " \"");
        chorda_aedificator_appendere_chorda(s->aed, publicus);
        chorda_aedificator_appendere_literis(s->aed, "\" \"");
        chorda_aedificator_appendere_chorda(s->aed, systema);
        chorda_aedificator_appendere_character(s->aed, '"');
    }
    chorda_aedificator_appendere_character(s->aed, '>');
}

/* Attributa ordinata: titulus coctus, valor coctus; titulus iam visus
 * omittitur (HTML5 primum servat). Tectum LXIV. */
interior vacuum
_attributa_scribere (
                 Scriptor* s,
    constans MateriaNodus* e,
                      i32  gradus,
              HtmlAlienum  alienum)
{
    Attributum tabula[ATTRIBUTA_MAXIMA];
           i32 n        = ZEPHYRUM;
           i32 numerus  = _numerus(e, (i32)HTML_ELEMENTUM_ATTRIBUTA);
           i32 i;
           i32 j;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        MateriaNodus* a = _liber(e, (i32)HTML_ELEMENTUM_ATTRIBUTA, i);
        MateriaToken* titulus;
        MateriaToken* valor;
          Attributum  x;
                 b32  iam_visum = FALSUM;

        si (a == NIHIL || n >= ATTRIBUTA_MAXIMA)
        {
            perge;
        }
        titulus = _tok(a, (i32)HTML_ATTRIBUTUM_TOK_NOMEN);
        si (titulus == NIHIL)
        {
            perge;
        }
        x.titulus = _titulus_attributi(s->piscina, titulus->valor,
            alienum);
        per (j = ZEPHYRUM; j < n; j++)
        {
            si (chorda_aequalis(tabula[j].titulus, x.titulus))
            {
                iam_visum = VERUM;
                frange;
            }
        }
        si (iam_visum)
        {
            perge;
        }
        valor = _tok(a, (i32)HTML_ATTRIBUTUM_TOK_VALOR);
        si (valor == NIHIL)
        {
            x.valor = _copia_literis(s->piscina, "");
        }
        alioquin
        {
            x.valor = _valor_coctus(s->piscina, valor->valor);
        }
        /* insertio ordinata */
        j = n;
        dum (j > ZEPHYRUM && _ante(x.titulus, tabula[j - I].titulus))
        {
            tabula[j]  = tabula[j - I];
            j          = j - I;
        }
        tabula[j]  = x;
        n          = n + I;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        _lineam_incipere(s, gradus);
        chorda_aedificator_appendere_chorda(s->aed, tabula[i].titulus);
        chorda_aedificator_appendere_literis(s->aed, "=\"");
        chorda_aedificator_appendere_chorda(s->aed, tabula[i].valor);
        chorda_aedificator_appendere_character(s->aed, '"');
    }
}

interior vacuum
_liberos_scribere (
                 Scriptor* s,
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  gradus,
              HtmlAlienum  alienum,
                      b32  lf_demendum,
                      b32  decoquendum);

interior vacuum
_elementum_scribere (
                 Scriptor* s,
    constans MateriaNodus* e,
                      i32  gradus,
              HtmlAlienum  alienum)
{
    MateriaToken* apertura = _tok(e, (i32)HTML_ELEMENTUM_TOK_APERTURA);
          chorda  crudus;
          chorda  titulus;
     HtmlAlienum  proprium;
     HtmlAlienum  liberorum;
             b32  lf;
             b32  decoquendum;

    si (apertura == NIHIL)
    {
        redde;
    }
    crudus = apertura->valor;
    si (crudus.mensura >= I)
    {
        crudus.datum    = crudus.datum + I;
        crudus.mensura  = crudus.mensura - I;
    }
    titulus = html_coctum_titulus(s->piscina, crudus, alienum);
    /* spatium nominum PROPRIUM: svg/math ipsa alieni sunt, non solum
     * liberi eorum ('<svg svg>') */
    proprium = alienum;
    si (alienum == HTML_ALIENUM_NULLUM)
    {
        si (chorda_aequalis_literis(titulus, "svg"))
        {
            proprium = HTML_ALIENUM_SVG;
        }
        alioquin si (chorda_aequalis_literis(titulus, "math"))
        {
            proprium = HTML_ALIENUM_MATHEMATICA;
        }
    }
    _lineam_incipere(s, gradus);
    chorda_aedificator_appendere_character(s->aed, '<');
    si (proprium == HTML_ALIENUM_SVG)
    {
        chorda_aedificator_appendere_literis(s->aed, "svg ");
    }
    alioquin si (proprium == HTML_ALIENUM_MATHEMATICA)
    {
        chorda_aedificator_appendere_literis(s->aed, "math ");
    }
    chorda_aedificator_appendere_chorda(s->aed, titulus);
    chorda_aedificator_appendere_character(s->aed, '>');
    _attributa_scribere(s, e, gradus + I, proprium);

    liberorum = html_coctum_alienum_liberorum(alienum, titulus, e);
    lf =(b32)(alienum == HTML_ALIENUM_NULLUM
               && (chorda_aequalis_literis(titulus, "pre")
                   || chorda_aequalis_literis(titulus, "listing")
                   || chorda_aequalis_literis(titulus, "textarea")));
    decoquendum = (b32)(alienum == HTML_ALIENUM_NULLUM
                   && (chorda_aequalis_literis(titulus, "title")
                       || chorda_aequalis_literis(titulus,
                       "textarea")));
    si (   alienum == HTML_ALIENUM_NULLUM
        && chorda_aequalis_literis(titulus, "template"))
    {
        _lineam_incipere(s, gradus + I);
        chorda_aedificator_appendere_literis(s->aed, "content");
        gradus = gradus + I;
    }
    _liberos_scribere(s, e, (i32)HTML_ELEMENTUM_LIBERI, gradus + I,
        liberorum, lf, decoquendum);
}

interior vacuum
_liberos_scribere (
                 Scriptor* s,
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  gradus,
              HtmlAlienum  alienum,
                      b32  lf_demendum,
                      b32  decoquendum)
{
    ChordaAedificator* cumulus         = NIHIL;
                  b32  cumulus_primus  = FALSUM;
                  b32  primus          = VERUM;
                  i32  n               = _numerus(nodus, locus);
                  i32  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        MateriaNodus* liber = _liber(nodus, locus, i);
        MateriaToken* tok;

        si (liber == NIHIL)
        {
            perge;
        }
        si (liber->genus == (s32)HTML_GENUS_ELEMENTUM_MALUM)
        {
            perge;   /* DOM nihil retinet; textus circa coniungitur */
        }
        tok = (liber->genus == (s32)HTML_GENUS_ELEMENTUM)
            ? NIHIL : _tok(liber, ZEPHYRUM);
        si (   liber->genus == (s32)HTML_GENUS_TEXTUS
            || liber->genus == (s32)HTML_GENUS_REFERENTIA
            || liber->genus == (s32)HTML_GENUS_TEXTUS_CRUDUS)
        {
            si (tok != NIHIL)
            {
                si (cumulus == NIHIL)
                {
                    cumulus_primus = primus;
                }
                /* NUL: in textu corporis HTML ignoratur, in crudo et
                 * alienis U+FFFD */
                _cumulare(s, &cumulus, tok->valor,
                    (b32)(liber->genus == (s32)HTML_GENUS_REFERENTIA
                          || (liber->genus
                              == (s32)HTML_GENUS_TEXTUS_CRUDUS
                              && decoquendum)),
                    (b32)(liber->genus != (s32)HTML_GENUS_TEXTUS_CRUDUS
                          && alienum == HTML_ALIENUM_NULLUM));
            }
        }
        alioquin si (   liber->genus == (s32)HTML_GENUS_CDATA
                     && alienum      != HTML_ALIENUM_NULLUM)
        {
            si (tok != NIHIL)
            {
                chorda c = tok->valor;

                si (_incipit_literis(c, "<![CDATA["))
                {
                    c.datum    = c.datum + IX;
                    c.mensura  = c.mensura - IX;
                }
                si (_terminatur_literis(c, "]]>"))
                {
                    c.mensura = c.mensura - III;
                }
                si (cumulus == NIHIL)
                {
                    cumulus_primus = primus;
                }
                _cumulare(s, &cumulus, c, FALSUM, FALSUM);
            }
        }
        alioquin
        {
            _cumulum_effundere(s, &cumulus, gradus,
                (b32)(lf_demendum && cumulus_primus));
            si (liber->genus == (s32)HTML_GENUS_ELEMENTUM)
            {
                _elementum_scribere(s, liber, gradus, alienum);
            }
            alioquin si (   liber->genus == (s32)HTML_GENUS_COMMENTARIUM
                         && tok          != NIHIL)
            {
                _commentarium_scribere(s, tok->valor, gradus);
            }
            alioquin si (   liber->genus == (s32)HTML_GENUS_CDATA
                         && tok          != NIHIL)
            {
                /* extra alienos: commentarium pravum '[CDATA[...]]' */
                chorda c = tok->valor;

                si (c.mensura >= II)
                {
                    c.datum    = c.datum + II;
                    c.mensura  = c.mensura - II;
                }
                si (_terminatur_literis(c, ">"))
                {
                    c.mensura = c.mensura - I;
                }
                _lineam_incipere(s, gradus);
                chorda_aedificator_appendere_literis(s->aed, "<!-- ");
                _textum_appendere(s->aed, c, FALSUM, FALSUM);
                chorda_aedificator_appendere_literis(s->aed, " -->");
            }
            alioquin si (   liber->genus == (s32)HTML_GENUS_DOCTYPE
                         && tok          != NIHIL)
            {
                _doctype_scribere(s, tok->valor, gradus);
            }
        }
        primus = FALSUM;
    }
    _cumulum_effundere(s, &cumulus, gradus,
        (b32)(lf_demendum && cumulus_primus));
}

chorda
html_coctum_scribere (
                  Piscina* piscina,
    constans MateriaNodus* radix,
              HtmlAlienum  alienum)
{
    Scriptor s;
         i32 locus;

    s.piscina = piscina;
    s.aed = chorda_aedificator_creare(piscina,
        (memoriae_index)MXXIV);
    s.prima = VERUM;
    si (s.aed == NIHIL)
    {
        chorda c;

        c.datum    = NIHIL;
        c.mensura  = ZEPHYRUM;
        redde c;
    }
    locus = (radix->genus == (s32)HTML_GENUS_DOCUMENTUM)
          ? (i32)HTML_DOCUMENTUM_LIBERI : (i32)HTML_ELEMENTUM_LIBERI;
    _liberos_scribere(&s, radix, locus, ZEPHYRUM, alienum, FALSUM,
        FALSUM);
    redde chorda_aedificator_finire(s.aed);
}
