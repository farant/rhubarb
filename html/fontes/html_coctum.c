/* html_coctum.c - Vide html_coctum.h. */

#include "html_coctum.h"
#include "html_alienum.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include "html_lexema.h"
#include "materia_token.h"
#include "entitates_html.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include <string.h>

#define ATTRIBUTA_MAXIMA ((i32)64)

nomen structura {
              Piscina* piscina;
    ChordaAedificator* aed;
    /* nondum linea ulla scripta: '\n' ante lineam omittendum */
    b32 prima;
    /* O7b: nodi cum sede (parens DOM alibi), ordine documenti - Xar
     * de constans MateriaNodus* */
    Xar* sedentes;
} Scriptor;

nomen structura {
    chorda titulus;
    chorda valor;
} Attributum;

/* decoctio referentiarum characterum in textu appendendo: nulla
 * (script/style, commentaria), textus (formae veteres inclusae),
 * attributi (exceptio ante '=' aut litteram) */
nomen enumeratio {
    DECOCTIO_NULLA = 0,
    DECOCTIO_TEXTUS,
    DECOCTIO_ATTRIBUTI
} Decoctio;


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


/* ==================================================
 * Sedes (O7b): parens DOM alibi
 * ================================================== */

/* Sedes nodi: parens DOM alibi, aut NIHIL (elementum locus
 * HTML_ELEMENTUM_SEDES, textus et referentia locus I). */
interior MateriaNodus*
_sedes (
    constans MateriaNodus* nodus)
{
    i32 locus;

    si (nodus->genus == (s32)HTML_GENUS_ELEMENTUM)
    {
        locus = (i32)HTML_ELEMENTUM_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_TEXTUS)
    {
        locus = (i32)HTML_TEXTUS_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_REFERENTIA)
    {
        locus = (i32)HTML_REFERENTIA_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_COMMENTARIUM)
    {
        locus = (i32)HTML_COMMENTARIUM_SEDES;
    }
    alioquin
    {
        redde NIHIL;
    }
    si (   locus                    >= nodus->numerus_locorum
        || nodus->loci[locus].genus != MATERIA_VALOR_REFERENTIA)
    {
        redde NIHIL;
    }
    redde nodus->loci[locus].datum.nodus;
}

/* Nodos cum sede colligere, ordine documenti (ambulatio recursiva -
 * fixurae oraculi, non paginae M gradus profundae). */
interior vacuum
_sedentes_colligere (
                      Xar* sedentes,
    constans MateriaNodus* nodus)
{
    i32 locus;
    i32 n;
    i32 i;

    si (_sedes(nodus) != NIHIL)
    {
        constans MateriaNodus** l =
            (constans MateriaNodus**)xar_addere(sedentes);

        si (l != NIHIL)
        {
            *l = nodus;
        }
    }
    si (nodus->genus == (s32)HTML_GENUS_DOCUMENTUM)
    {
        locus = (i32)HTML_DOCUMENTUM_LIBERI;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_ELEMENTUM)
    {
        locus = (i32)HTML_ELEMENTUM_LIBERI;
    }
    alioquin
    {
        redde;
    }
    n = _numerus(nodus, locus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaNodus* liber = _liber(nodus, locus, i);

        si (liber != NIHIL)
        {
            _sedentes_colligere(sedentes, liber);
        }
    }
}

/* Liber parentis dati qui nodum continet (per patres); NIHIL si nodus
 * extra parentem iacet. */
interior constans MateriaNodus*
_liber_continens (
    constans MateriaNodus* parens,
    constans MateriaNodus* nodus)
{
    constans MateriaNodus* n = nodus;

    dum (n != NIHIL && n->pater != parens)
    {
        n = n->pater;
    }
    redde n;
}

/* Praecedens nodi (O7c): frater DOM prior nodi ab adoption agency
 * moti, aut NIHIL (elementa sola). */
interior MateriaNodus*
_praecedens (
    constans MateriaNodus* nodus)
{
    si (   nodus->genus                   != (s32)HTML_GENUS_ELEMENTUM
        || (i32)HTML_ELEMENTUM_PRAECEDENS >= nodus->numerus_locorum
        || nodus->loci[HTML_ELEMENTUM_PRAECEDENS].genus
               != MATERIA_VALOR_REFERENTIA)
    {
        redde NIHIL;
    }
    redde nodus->loci[HTML_ELEMENTUM_PRAECEDENS].datum.nodus;
}

/* Index nodi in ordine; -I si abest */
interior s32
_ordinis_index (
             constans Xar* ordo,
    constans MateriaNodus* nodus)
{
    i32 i;
    i32 n = xar_numerus(ordo);

    per (i = ZEPHYRUM; i < n; i++)
    {
        si (*(constans MateriaNodus* constans*)xar_obtinere(ordo, i)
                == nodus)
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}

/* Nodum in ordinem ad indicem inserere (superiora moventur) */
interior vacuum
_ordini_inserere (
                      Xar* ordo,
                      i32  index,
    constans MateriaNodus* nodus)
{
    constans MateriaNodus** l =
        (constans MateriaNodus**)xar_addere(ordo);
                       i32 n = xar_numerus(ordo);
                       i32 i;

    si (l == NIHIL)
    {
        redde;
    }
    per (i = n - I; i > index; i--)
    {
        *(constans MateriaNodus**)xar_obtinere(ordo, i) =
            *(constans MateriaNodus**)xar_obtinere(ordo, i - I);
    }
    *(constans MateriaNodus**)xar_obtinere(ordo, index) = nodus;
}

/* Elementum verum 'table' (litteris neglectis)? */
interior b32
_tabula_est (
    constans MateriaNodus* nodus)
{
    MateriaToken* apertura;
          chorda  t;
             i32  i;

    si (nodus == NIHIL || nodus->genus != (s32)HTML_GENUS_ELEMENTUM)
    {
        redde FALSUM;
    }
    apertura = _tok(nodus, (i32)HTML_ELEMENTUM_TOK_APERTURA);
    si (apertura == NIHIL)
    {
        redde FALSUM;
    }
    t = apertura->valor;
    si (t.mensura != VI)
    {
        redde FALSUM;
    }
    per (i = I; i < VI; i++)
    {
        si ((character)_minuscula(t.datum[i]) != "<table"[i])
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Octetos textus appendere: CR LF -> LF, CR -> LF (praeparatio fluxi
 * HTML5); '&...;' decoctum si iussum (referentia ignota cruda manet);
 * NUL aut deletus (textus corporis HTML: 'in body' eum ignorat) aut
 * U+FFFD (lexator: textus crudus, alieni, attributa, commentaria). */
interior vacuum
_textum_appendere (
    ChordaAedificator* aed,
               chorda  c,
             Decoctio  decoctio,
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
        si (ch == '&' && decoctio != DECOCTIO_NULLA)
        {
            character exitus[XVI];
                  i32 longitudo;
                  s32 post;

            si (entitates_html_decoquere((constans character*)c.datum,
                i,
                    (s32)c.mensura, (b32)(decoctio
                        == DECOCTIO_ATTRIBUTI),
                    exitus, &longitudo, &post))
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
    _textum_appendere(aed, v, DECOCTIO_ATTRIBUTI, FALSUM);
    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Tituli et contentum alienum: html_alienum (O2b-6)
 * ================================================== */

/* Titulus attributi coctus: minusculus, in alienis accommodatus
 * (svg camelCase, definitionURL, praefixa spatiorum). */
interior chorda
_titulus_attributi (
        Piscina* piscina,
         chorda  crudus,
    HtmlAlienum  proprium)
{
    redde html_alienum_attributum(piscina, _copia_minuscula(piscina,
        crudus), proprium);
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
              Decoctio   decoctio,
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
    _textum_appendere(*cumulus, c, decoctio, nul_delere);
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
    _textum_appendere(s->aed, datum, DECOCTIO_NULLA, FALSUM);
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
              HtmlAlienum  parentis,
              HtmlAlienum  alienum,
                      b32  lf_demendum,
                      b32  decoquendum);

interior vacuum
_elementum_scribere (
                 Scriptor* s,
    constans MateriaNodus* e,
                      i32  gradus,
              HtmlAlienum  parentis,
              HtmlAlienum  alienum)
{
    constans MateriaNodus* originale = e;
             MateriaToken* apertura;
                   chorda  crudus;
                   chorda  titulus;
              HtmlAlienum  proprium;
              HtmlAlienum  liberorum;
                      b32  lf;
                      b32  decoquendum;

    /* EXEMPLAR (O7c): elementum formans iterum apertum - titulus et
     * attributa originalis per referentiam */
    si (   e->numerus_locorum > (i32)HTML_ELEMENTUM_EXEMPLAR
        && e->loci[HTML_ELEMENTUM_EXEMPLAR].genus
               == MATERIA_VALOR_REFERENTIA
        && e->loci[HTML_ELEMENTUM_EXEMPLAR].datum.nodus != NIHIL)
    {
        originale = e->loci[HTML_ELEMENTUM_EXEMPLAR].datum.nodus;
    }
    apertura = _tok(originale, (i32)HTML_ELEMENTUM_TOK_APERTURA);
    si (apertura == NIHIL)
    {
        /* elementum FICTUM (O7a): titulus ex loco synthesis, attributa
         * nulla; sine eo nodus mutilus - nihil */
        constans MateriaValor* synthesis =
            &e->loci[HTML_ELEMENTUM_SYNTHESIS];
           constans character* fictum = NIHIL;

        si (synthesis->genus == MATERIA_VALOR_INDEX)
        {
            fictum =
                html_arbor_synthesis_titulus(synthesis->datum.index);
        }
        si (fictum == NIHIL)
        {
            redde;
        }
        titulus = _copia_literis(s->piscina, fictum);
    }
    alioquin
    {
        crudus = apertura->valor;
        si (crudus.mensura >= I)
        {
            crudus.datum    = crudus.datum + I;
            crudus.mensura  = crudus.mensura - I;
        }
        titulus = _copia_minuscula(s->piscina, crudus);
    }
    /* spatium PROPRIUM ex parente (svg/math ipsa aliena: '<svg svg>';
     * mglyph in mi MathML), titulus svg accommodatus in eo */
    proprium  = html_alienum_proprium(parentis, alienum, titulus);
    titulus   = html_alienum_titulus(s->piscina, titulus, proprium);
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
    _attributa_scribere(s, originale, gradus + I, proprium);

    liberorum = html_alienum_liberorum(proprium, titulus, e);
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
        proprium, liberorum, lf, decoquendum);
}

interior vacuum
_liberos_scribere (
                 Scriptor* s,
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  gradus,
              HtmlAlienum  parentis,
              HtmlAlienum  alienum,
                      b32  lf_demendum,
                      b32  decoquendum)
{
    ChordaAedificator* cumulus         = NIHIL;
                  b32  cumulus_primus  = FALSUM;
                  b32  primus          = VERUM;
                  i32  n               = _numerus(nodus, locus);
                  i32  m               = xar_numerus(s->sedentes);
                  Xar* ordo;
                  i32  numerus;
                  i32  i;
                  i32  j;

    /* ORDO DOM (O7b): liberi octetorum sine sede in ordine suo;
     * sedentes huius parentis ANTE liberum tabulam quae eos continet
     * (foster parenting: 'ante tabulam in parente eius'), aliter POST
     * omnes (contentum post body, caput post head, fragmentum contextu
     * tabulae - spec: appenditur). Ordo inter sedentes = documenti.
     * O7c: sedens cum PRAECEDENTE (adoption agency) post praecedentem
     * suum in ordine - inseritur cum praecedens iam adest (rotae usque
     * ad quietem: praecedens ipse sedens esse potest), post sedentes
     * eiusdem praecedentis priores. */
    ordo = xar_creare(s->piscina, magnitudo(constans MateriaNodus*));
    si (ordo == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans MateriaNodus*  liber = _liber(nodus, locus, i);
        constans MateriaNodus** l;

        si (liber == NIHIL || _sedes(liber) != NIHIL)
        {
            perge;
        }
        si (_tabula_est(liber))
        {
            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaNodus* sedens =
                    *(constans MateriaNodus**)xar_obtinere(s->sedentes,
                        j);

                si (   _sedes(sedens)                  == nodus
                    && _praecedens(sedens)             == NIHIL
                    && _liber_continens(nodus, sedens) == liber)
                {
                    l = (constans MateriaNodus**)xar_addere(ordo);
                    si (l != NIHIL)
                    {
                        *l = sedens;
                    }
                }
            }
        }
        l = (constans MateriaNodus**)xar_addere(ordo);
        si (l != NIHIL)
        {
            *l = liber;
        }
    }
    per (j = ZEPHYRUM; j < m; j++)
    {
        constans MateriaNodus* sedens =
            *(constans MateriaNodus**)xar_obtinere(s->sedentes, j);
        constans MateriaNodus* continens;

        si (_sedes(sedens) != nodus || _praecedens(sedens) != NIHIL)
        {
            perge;
        }
        continens = _liber_continens(nodus, sedens);
        si (continens == NIHIL || !_tabula_est(continens))
        {
            constans MateriaNodus** l =
                (constans MateriaNodus**)xar_addere(ordo);

            si (l != NIHIL)
            {
                *l = sedens;
            }
        }
    }
    {
        b32 motum = VERUM;

        dum (motum)
        {
            motum = FALSUM;
            per (j = ZEPHYRUM; j < m; j++)
            {
                constans MateriaNodus* sedens =
                    *(constans MateriaNodus**)xar_obtinere(s->sedentes,
                        j);
                constans MateriaNodus* prior = _praecedens(sedens);
                                  s32  q;

                si (   _sedes(sedens) != nodus || prior == NIHIL
                    || _ordinis_index(ordo, sedens) >= ZEPHYRUM)
                {
                    perge;
                }
                q = _ordinis_index(ordo, prior);
                si (q < ZEPHYRUM)
                {
                    perge;   /* praecedens nondum in ordine */
                }
                /* post sedentes priores eiusdem praecedentis */
                dum (   (i32)q + I < xar_numerus(ordo)
                     && _praecedens(*(constans MateriaNodus* constans*)
                            xar_obtinere(ordo, (i32)q + I)) == prior)
                {
                    q = q + I;
                }
                _ordini_inserere(ordo, (i32)q + I, sedens);
                motum = VERUM;
            }
        }
        /* praecedens numquam inventus (extra parentem): post omnes */
        per (j = ZEPHYRUM; j < m; j++)
        {
            constans MateriaNodus* sedens =
                *(constans MateriaNodus**)xar_obtinere(s->sedentes, j);

            si (   _sedes(sedens)      == nodus
                && _praecedens(sedens) != NIHIL
                && _ordinis_index(ordo, sedens) < ZEPHYRUM)
            {
                constans MateriaNodus** l =
                    (constans MateriaNodus**)xar_addere(ordo);

                si (l != NIHIL)
                {
                    *l = sedens;
                }
            }
        }
    }

    numerus = xar_numerus(ordo);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        constans MateriaNodus* liber =
            *(constans MateriaNodus**)xar_obtinere(ordo, i);
              MateriaToken* tok;

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
                    (liber->genus != (s32)HTML_GENUS_TEXTUS_CRUDUS
                     || decoquendum) ? DECOCTIO_TEXTUS : DECOCTIO_NULLA,
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
                _cumulare(s, &cumulus, c, DECOCTIO_NULLA, FALSUM);
            }
        }
        alioquin
        {
            _cumulum_effundere(s, &cumulus, gradus,
                (b32)(lf_demendum && cumulus_primus));
            si (liber->genus == (s32)HTML_GENUS_ELEMENTUM)
            {
                _elementum_scribere(s, liber, gradus, parentis,
                    alienum);
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
                _textum_appendere(s->aed, c, DECOCTIO_NULLA, FALSUM);
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
              HtmlAlienum  parentis,
              HtmlAlienum  alienum)
{
    Scriptor s;
         i32 locus;

    s.piscina = piscina;
    s.aed = chorda_aedificator_creare(piscina,
        (memoriae_index)MXXIV);
    s.prima = VERUM;
    s.sedentes = xar_creare(piscina, magnitudo(constans MateriaNodus*));
    si (s.aed == NIHIL || s.sedentes == NIHIL)
    {
        chorda c;

        c.datum    = NIHIL;
        c.mensura  = ZEPHYRUM;
        redde c;
    }
    _sedentes_colligere(s.sedentes, radix);
    locus = (radix->genus == (s32)HTML_GENUS_DOCUMENTUM)
          ? (i32)HTML_DOCUMENTUM_LIBERI : (i32)HTML_ELEMENTUM_LIBERI;
    _liberos_scribere(&s, radix, locus, ZEPHYRUM, parentis, alienum,
        FALSUM, FALSUM);
    redde chorda_aedificator_finire(s.aed);
}
