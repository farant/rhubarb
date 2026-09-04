/* probatio_md_oraculum.c - Oraculum CommonMark/GFM programmatis html (B3.2)
 *
 * Quodque exemplum spec.txt (nucleus 0.31.2 + extensiones GFM) per
 * md_html_reddere in html vertitur; html nostrum et exspectatum ambo
 * per NORMAM eandem super html_lexare transeunt (oraculum generis
 * alieni: lexator html, non scriptor stml), deinde octetim
 * conferuntur. Norma IGNOSCIT - quodque genus NUMERATUM et impressum:
 *   - spatia inter tags blocorum (nostrum lineas novas inter bloca
 *     non fert - templum spatium album fingere nequit);
 *   - spatia marginalia textus iuxta tagum bloci (et post <input>);
 *   - cursus spatii albi extra pre/code in unum contracti;
 *   - ordo attributorum (nomine ordinati); forma vacuorum ('/>' = '>');
 *   - attributa booleana ('x=""' = 'x'); entia decocta utrimque.
 * Intra <pre>/<code> textus exactus (entia decocta). Praeterita
 * PINNATA: numerus crescere solum potest (custos regressionis);
 * fracturae per sectionem numeratae, primae impressae cum norma
 * utraque. Consilium B3.
 */
#include "latina.h"
#include "md_html.h"
#include "md_exempla.h"
#include "md_decoctum.h"
#include "html_lexema.h"
#include "credo.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pinna: praeterita minima (B3.2 nativitas: valor post cursum primum) */
#define PINNA_PRAETERITA 1248

hic_manens constans character* TAGI_BLOCORUM[] = {
    "p", "div", "ul", "ol", "li", "blockquote", "pre", "h1", "h2", "h3",
    "h4", "h5", "h6", "hr", "table", "thead", "tbody", "tr", "th", "td",
    "br", "html", "body", "head", "dl", "dt", "dd", "section", "figure",
    "details", "summary", "input", "img", NIHIL
};

nomen structura {
    i32 spatia_inter;      /* textus solis spatiis iuxta blocum omissus */
    i32 spatia_marginalia; /* spatia ducentia/caudalia iuxta blocum */
    i32 spatia_contracta;  /* cursus spatii albi -> unum */
    i32 attributa_ordinata;
    i32 vacua_forma;       /* '/>' */
    i32 booleana;          /* attributum sine valore */
} Ignoscentia;

nomen structura {
    chorda titulus;
    chorda valor;
       b32 habet_valorem;
} Attributum;

interior chorda
_plagulam_legere (
               Piscina* piscina,
    constans character* via)
{
        chorda c;
         FILE* f;
        longus longitudo;
    character* memoria;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    f          = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde c;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde c;
    }
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde c;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde c;
    }
    fclose(f);
    c.datum    = (i8*)memoria;
    c.mensura  = (i32)longitudo;
    redde c;
}

interior b32
_est_spatium (
    i8 c)
{
    redde (b32)(c == (i8)' ' || c == (i8)'\t' || c == (i8)'\n'
                || c == (i8)'\r');
}

interior b32
_tagus_bloci (
    chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM; TAGI_BLOCORUM[i] != NIHIL; i++)
    {
        si (chorda_aequalis_literis(titulus, TAGI_BLOCORUM[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* titulus tagi ex lexemate '<div' / '</div' (minusculae ASCII, copia) */
interior chorda
_nomen_tagi (
     Piscina* piscina,
      chorda  valor,
         i32  praefixum)
{
    chorda n;
       i32 i;

    n.mensura = valor.mensura > praefixum ? valor.mensura - praefixum
                                         : ZEPHYRUM;
    n.datum   = (i8*)piscina_allocare(piscina, (memoriae_index)n.mensura
        + I);
    per (i = ZEPHYRUM; i < n.mensura; i++)
    {
        i8 c = valor.datum[praefixum + i];

        si (c >= (i8)'A' && c <= (i8)'Z')
        {
            c = (i8)(c + XXXII);
        }
        n.datum[i] = c;
    }
    redde n;
}

/* entia decoquere in chorda (attributa, textus): copia nova */
interior vacuum
_decoctum_appendere (
    ChordaAedificator* aed,
               chorda  c)
{
    s32 i = ZEPHYRUM;

    dum (i < (s32)c.mensura)
    {
        si (c.datum[i] == (i8)'&')
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
        chorda_aedificator_appendere_character(aed,
            (character)c.datum[i]);
        i++;
    }
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

/* Norma: html -> chorda canonica (vide caput) */
interior chorda
_normare (
        Piscina* piscina,
         chorda  html,
    Ignoscentia* ig)
{
    ChordaAedificator* aed;
                  Xar* lexemata;
                  i32  i;
                  i32  num;
                  i32  cruda        = ZEPHYRUM;     /* profunditas pre/code */
                  b32  post_blocum  = VERUM;  /* initium documenti = limes */

    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)html.mensura + LXIV);
    lexemata = html_lexare(piscina, (constans character*)html.datum,
        html.mensura);
    si (aed == NIHIL || lexemata == NIHIL)
    {
        redde html;
    }
    num = xar_numerus(lexemata);
    per (i = ZEPHYRUM; i < num; i++)
    {
        HtmlLexema* t = (HtmlLexema*)xar_obtinere(lexemata, i);

        si (t == NIHIL || t->genus == HTML_LEX_FINIS)
        {
            frange;
        }
        si (   t->genus == HTML_LEX_TEXTUS
            || t->genus == HTML_LEX_TEXTUS_CRUDUS
            || t->genus == HTML_LEX_TEXTUS_CRUDUS_IMPERFECTUS)
        {
            si (cruda > ZEPHYRUM)
            {
                chorda_aedificator_appendere_chorda(aed, t->valor);
                post_blocum = FALSUM;
                perge;
            }
            {
                /* limes proximus: tagus bloci aut finis? */
                b32 ante_blocum   = FALSUM;
                b32 solum_spatia  = VERUM;
                i32 k;
                i32 a = ZEPHYRUM;
                i32 b = t->valor.mensura;

                si (i + I < num)
                {
                    HtmlLexema* u = (HtmlLexema*)xar_obtinere(lexemata,
                        i + I);

                    si (u == NIHIL || u->genus == HTML_LEX_FINIS)
                    {
                        ante_blocum = VERUM;
                    }
                    alioquin si (   u->genus == HTML_LEX_TAG_APERTURA
                                 || u->genus == HTML_LEX_TAG_CLAUSURA)
                    {
                        ante_blocum = _tagus_bloci(_nomen_tagi(piscina,
                            u->valor, u->genus == HTML_LEX_TAG_APERTURA
                                ? I : II));
                    }
                }
                per (k = ZEPHYRUM; k < b; k++)
                {
                    si (!_est_spatium(t->valor.datum[k]))
                    {
                        solum_spatia = FALSUM;
                        frange;
                    }
                }
                si (solum_spatia && (post_blocum || ante_blocum))
                {
                    ig->spatia_inter++;
                    perge;
                }
                si (post_blocum)
                {
                    dum (a < b && _est_spatium(t->valor.datum[a]))
                    {
                        a++;
                        ig->spatia_marginalia++;
                    }
                }
                si (ante_blocum)
                {
                    dum (b > a && _est_spatium(t->valor.datum[b - I]))
                    {
                        b--;
                        ig->spatia_marginalia++;
                    }
                }
                /* cursus spatii albi -> unum */
                k = a;
                dum (k < b)
                {
                    si (_est_spatium(t->valor.datum[k]))
                    {
                        i32 m = k;

                        dum (m < b && _est_spatium(t->valor.datum[m]))
                        {
                            m++;
                        }
                        si (m - k > I || t->valor.datum[k] != (i8)' ')
                        {
                            ig->spatia_contracta++;
                        }
                        chorda_aedificator_appendere_character(aed,
                            ' ');
                        k = m;
                        perge;
                    }
                    chorda_aedificator_appendere_character(aed,
                        (character)t->valor.datum[k]);
                    k++;
                }
                post_blocum = FALSUM;
            }
            perge;
        }
        si (t->genus == HTML_LEX_REFERENTIA)
        {
            _decoctum_appendere(aed, t->valor);
            post_blocum = FALSUM;
            perge;
        }
        si (t->genus == HTML_LEX_TAG_APERTURA)
        {
                chorda titulus = _nomen_tagi(piscina, t->valor, I);
            Attributum attributa[XXXII];
                   i32 n_attr = ZEPHYRUM;
                   i32 j;
                   b32 solus = FALSUM;

            i++;
            dum (i < num)
            {
                HtmlLexema* u = (HtmlLexema*)xar_obtinere(lexemata, i);

                si (u == NIHIL || u->genus == HTML_LEX_FINIS)
                {
                    frange;
                }
                si (u->genus == HTML_LEX_TAG_FINIS)
                {
                    frange;
                }
                si (u->genus == HTML_LEX_TAG_FINIS_SOLUS)
                {
                    solus = VERUM;
                    ig->vacua_forma++;
                    frange;
                }
                si (   u->genus == HTML_LEX_ATTRIBUTUM_NOMEN
                    && n_attr < XXXII)
                {
                    attributa[n_attr].titulus = _nomen_tagi(piscina,
                        u->valor, ZEPHYRUM);
                    attributa[n_attr].valor.datum    = NIHIL;
                    attributa[n_attr].valor.mensura  = ZEPHYRUM;
                    attributa[n_attr].habet_valorem  = FALSUM;
                    n_attr++;
                }
                alioquin si (   (   u->genus
                             == HTML_LEX_ATTRIBUTUM_VALOR
                                 || u->genus
                                     == HTML_LEX_ATTRIBUTUM_VALOR_IMPERFECTUS)
                             && n_attr > ZEPHYRUM)
                {
                    chorda v = u->valor;

                    si (   v.mensura              >= II
                        && (v.datum[ZEPHYRUM] == (i8)'"'
                        || v.datum[ZEPHYRUM] == (i8)'\'')
                        && v.datum[v.mensura - I] == v.datum[ZEPHYRUM])
                    {
                        v.datum    += I;
                        v.mensura  -= II;
                    }
                    attributa[n_attr - I].valor          = v;
                    attributa[n_attr - I].habet_valorem  = VERUM;
                }
                i++;
            }
            (vacuum)solus;
            /* ordinare nomine (insertio) */
            per (j = I; j < n_attr; j++)
            {
                Attributum x = attributa[j];
                       i32 k = j;

                dum (   k > ZEPHYRUM
                     && _ante(x.titulus, attributa[k - I].titulus))
                {
                    attributa[k] = attributa[k - I];
                    k--;
                }
                si (k != j)
                {
                    ig->attributa_ordinata++;
                }
                attributa[k] = x;
            }
            chorda_aedificator_appendere_character(aed, '<');
            chorda_aedificator_appendere_chorda(aed, titulus);
            per (j = ZEPHYRUM; j < n_attr; j++)
            {
                chorda_aedificator_appendere_character(aed, ' ');
                chorda_aedificator_appendere_chorda(aed,
                    attributa[j].titulus);
                chorda_aedificator_appendere_literis(aed, "=\"");
                si (attributa[j].habet_valorem)
                {
                    _decoctum_appendere(aed, attributa[j].valor);
                }
                alioquin
                {
                    ig->booleana++;
                }
                chorda_aedificator_appendere_character(aed, '"');
            }
            chorda_aedificator_appendere_character(aed, '>');
            si (   chorda_aequalis_literis(titulus, "pre")
                || chorda_aequalis_literis(titulus, "code"))
            {
                cruda++;
            }
            post_blocum = _tagus_bloci(titulus);
            perge;
        }
        si (t->genus == HTML_LEX_TAG_CLAUSURA)
        {
            chorda titulus = _nomen_tagi(piscina, t->valor, II);

            /* usque ad '>' */
            dum (i + I < num)
            {
                HtmlLexema* u = (HtmlLexema*)xar_obtinere(lexemata, i
                    + I);

                i++;
                si (   u == NIHIL || u->genus == HTML_LEX_TAG_FINIS
                    || u->genus == HTML_LEX_FINIS)
                {
                    frange;
                }
            }
            chorda_aedificator_appendere_literis(aed, "</");
            chorda_aedificator_appendere_chorda(aed, titulus);
            chorda_aedificator_appendere_character(aed, '>');
            si (   (   chorda_aequalis_literis(titulus, "pre")
                    || chorda_aequalis_literis(titulus, "code"))
                && cruda > ZEPHYRUM)
            {
                cruda--;
            }
            post_blocum = _tagus_bloci(titulus);
            perge;
        }
        si (t->genus == HTML_LEX_SPATIA || t->genus == HTML_LEX_DELIM)
        {
            perge;
        }
        /* commenta, doctype, cdata, cetera: verbatim */
        chorda_aedificator_appendere_chorda(aed, t->valor);
        post_blocum = FALSUM;
    }
    redde chorda_aedificator_finire(aed);
}

nomen structura {
    chorda sectio;
       i32 praeterita;
       i32 totalis;
} Sectio;

interior Sectio*
_sectio (
     Sectio* sectiones,
        i32* numerus,
     chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < *numerus; i++)
    {
        si (chorda_aequalis(sectiones[i].sectio, titulus))
        {
            redde &sectiones[i];
        }
    }
    si (*numerus >= CCLVI)
    {
        redde &sectiones[CCLVI - I];
    }
    sectiones[*numerus].sectio      = titulus;
    sectiones[*numerus].praeterita  = ZEPHYRUM;
    sectiones[*numerus].totalis     = ZEPHYRUM;
    (*numerus)++;
    redde &sectiones[*numerus - I];
}

s32
principale (
    vacuum)
{
                Piscina* piscina;
     constans character* radix_viae;
              character  via[4096];
                 chorda  programma;
                 Sectio* sectiones;
                    i32  numerus_sectionum  = ZEPHYRUM;
                    i32  totalis            = ZEPHYRUM;
                    i32  praeterita         = ZEPHYRUM;
                    i32  fractae_impressae  = ZEPHYRUM;
            Ignoscentia  ig;
     constans character* plagulae[] = {
        "md/probationes/fixa/commonmark/spec.txt",
        "md/probationes/fixa/gfm/spec.txt", NIHIL };
                    i32 f;

    piscina = piscina_generare_dynamicum("probatio_md_oraculum",
        67108864);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    memset(&ig, ZEPHYRUM, magnitudo(ig));
    sectiones = (Sectio*)piscina_allocare(piscina,
        magnitudo(Sectio) * CCLVI);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    sprintf(via, "%s/md/html/md-html.stml", radix_viae);
    programma = _plagulam_legere(piscina, via);
    CREDO_VERUM (programma.datum != NIHIL);
    si (programma.datum == NIHIL)
    {
        credo_imprimere_compendium();
        redde I;
    }

    per (f = ZEPHYRUM; plagulae[f] != NIHIL; f++)
    {
        chorda  spec;
           Xar* exempla;
           i32  i;
           i32  num;

        sprintf(via, "%s/%s", radix_viae, plagulae[f]);
        spec = _plagulam_legere(piscina, via);
        CREDO_VERUM (spec.datum != NIHIL);
        si (spec.datum == NIHIL)
        {
            perge;
        }
        exempla = md_exempla_legere(piscina,
            (constans character*)spec.datum,
            spec.mensura);
        CREDO_NON_NIHIL (exempla);
        si (exempla == NIHIL)
        {
            perge;
        }
        num = xar_numerus(exempla);
        imprimere("\n--- %s: %d exempla ---\n", plagulae[f],
            (integer)num);
        per (i = ZEPHYRUM; i < num; i++)
        {
             MdExemplum* ex = (MdExemplum*)xar_obtinere(exempla, i);
                Piscina* p;
    InternamentumChorda* intern;
         MdHtmlResultus  r;
                 chorda  nostra;
                 chorda  sperata;
                 Sectio* s;
                    b32  sanum;

            si (ex == NIHIL)
            {
                perge;
            }
            s = _sectio(sectiones, &numerus_sectionum, ex->sectio);
            s->totalis++;
            totalis++;
            p = piscina_generare_dynamicum("exemplum", 8388608);
            si (p == NIHIL)
            {
                perge;
            }
            intern = internamentum_creare(p);
            r = md_html_reddere(p, intern,
                (constans character*)ex->markdown.datum,
                ex->markdown.mensura, programma);
            sanum = FALSUM;
            si (r.successus)
            {
                nostra   = _normare(p, r.html, &ig);
                sperata  = _normare(p, ex->html, &ig);
                sanum    = chorda_aequalis(nostra, sperata);
            }
            si (sanum)
            {
                s->praeterita++;
                praeterita++;
            }
            alioquin si (   fractae_impressae < XL
                         || getenv("ORACULUM_OMNIA") != NIHIL)
            {
                fractae_impressae++;
                imprimere("  #%d [%.*s]", (integer)ex->numerus,
                    (integer)ex->sectio.mensura,
                    (constans character*)ex->sectio.datum);
                si (!r.successus)
                {
                    imprimere(" FRACTA: %.*s\n",
                        (integer)r.causa.mensura,
                        (constans character*)r.causa.datum);
                }
                alioquin
                {
                    imprimere("\n    sperata: %.*s\n    nostra:  %.*s\n",
                        (integer)(sperata.mensura
                            < CXX ? sperata.mensura : CXX),
                        (constans character*)sperata.datum,
                        (integer)(nostra.mensura
                            < CXX ? nostra.mensura : CXX),
                        (constans character*)nostra.datum);
                }
            }
            piscina_destruere(p);
        }
    }

    imprimere("\n--- sectiones (praeterita/totalis) ---\n");
    {
        i32 i;

        per (i = ZEPHYRUM; i < numerus_sectionum; i++)
        {
            imprimere("  %4d/%-4d %.*s\n",
                (integer)sectiones[i].praeterita,
                (integer)sectiones[i].totalis,
                (integer)sectiones[i].sectio.mensura,
                (constans character*)sectiones[i].sectio.datum);
        }
    }
    imprimere("\n--- ignoscentiae: inter %d, marginalia %d, contracta %d, "
              "attributa ordinata %d, vacua '/>' %d, booleana %d ---\n",
        (integer)ig.spatia_inter, (integer)ig.spatia_marginalia,
        (integer)ig.spatia_contracta, (integer)ig.attributa_ordinata,
        (integer)ig.vacua_forma, (integer)ig.booleana);
    imprimere("\n--- ORACULUM: %d/%d praeterita (pinna %d) ---\n",
        (integer)praeterita, (integer)totalis,
        (integer)PINNA_PRAETERITA);
    CREDO_MAIOR_I32 (totalis, (i32)1300);
    CREDO_MAIOR_AUT_AEQUALIS_S32 ((s32)praeterita,
        (s32)PINNA_PRAETERITA);

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        si (praeteritus)
        {
            redde ZEPHYRUM;
        }
    }
    redde I;
}
