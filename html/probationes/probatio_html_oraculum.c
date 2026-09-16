/* probatio_html_oraculum.c - Oraculum html5lib (O2): arbor nostra
 * COCTA contra '#document' exspectatum
 *
 * Quodque exemplum .dat (html_exempla) parsatur (html_arbor_parsare),
 * per html_coctum in formam html5lib scribitur et octetim cum arbore
 * exspectata confertur. Arbor exspectata html/head/body SYNTHESIZAT
 * quae aedificator simplex numquam fingit (H4): EXUITUR - elementum
 * quod initus tag apertionis non nominat ('<html' '<head' '<body',
 * titulo integro) ex arbore exspectata tollitur, liberi eius gradu uno
 * sursum; fragmenta nihil synthesizant (contextus contentum alienum
 * solum dat). Exempla '#script-on' OMITTUNTUR (aedificator = scriptum
 * clausum). PINNA CRESCENS: praeterita minui non possunt; tabula per
 * plagulam = mechanismi HTML5 per plagulas iam distributi (adoption,
 * tables, template, foreign, entities, doctype, comments...).
 * ORACULUM_OMNIA=1 omnes fracturas imprimit; ORACULUM_EXEMPLUM=
 * plagula:n initum, exspectatum crudum, exutum et nostrum imprimit.
 */

#include "latina.h"
#include "credo.h"
#include "html_arbor.h"
#include "html_coctum.h"
#include "html_exempla.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pinna: praeterita minima (nativitas: valor post cursum primum) */
#define PINNA_PRAETERITA   ((s32)887)
/* exempla iudicata = MDCCVIII - VIII script-on */
#define TOTALIS_PINNATUS   ((i32)1700)
#define LINEAE_MAXIMAE     ((i32)8192)

nomen structura {
    constans character* plagula;
                   i32  praeterita;
                   i32  totalis;
                   i32  fragmenta_praeterita;
                   i32  fragmenta;
} Summa;

/* Linea arboris exspectatae: [a, b) in documentum = textus post '| '
 * et indentationem, lineae continuationis (textus '\n' ferens)
 * inclusae; gradus = spatia / II. */
nomen structura {
    s32 a;
    s32 b;
    i32 gradus;
    b32 delenda;
} LineaArboris;

hic_manens character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

hic_manens constans character*
_titulus (
    constans character* via)
{
    constans character* t = strrchr(via, '/');

    redde t == NIHIL ? via : t + I;
}

hic_manens i8
_minuscula (
    i8 c)
{
    si (c >= 'A' && c <= 'Z')
    {
        redde (i8)(c + ('a' - 'A'));
    }
    redde c;
}

hic_manens b32
_littera_tituli (
    i8 c)
{
    redde (b32)((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
                || (c >= '0' && c <= '9') || c == '-');
}

/* An initus tag apertionis '<titulus' nominet (litteris neglectis,
 * titulo integro: '<head' non '<header'). */
hic_manens b32
_initus_nominat (
                 chorda  initus,
     constans character* titulus)
{
    i32 n = (i32)strlen(titulus);
    s32 i;

    per (i = ZEPHYRUM; i + I + (s32)n <= (s32)initus.mensura; i++)
    {
        i32 k;
        b32 par = VERUM;

        si (initus.datum[i] != '<')
        {
            perge;
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            si ((character)_minuscula(initus.datum[i + I + (s32)k])
                != titulus[k])
            {
                par = FALSUM;
                frange;
            }
        }
        si (!par)
        {
            perge;
        }
        si (   i + I + (s32)n < (s32)initus.mensura
            && _littera_tituli(initus.datum[i + I + (s32)n]))
        {
            perge;
        }
        redde VERUM;
    }
    redde FALSUM;
}

/* Arborem exspectatam in lineas scindere; ultra maximum lineae
 * omittuntur (comparatio tum cadit - tutum). */
hic_manens i32
_lineas_legere (
          chorda  doc,
    LineaArboris* lineae,
             i32  maximum)
{
    s32 i = ZEPHYRUM;
    i32 n = ZEPHYRUM;

    dum (i < (s32)doc.mensura)
    {
        s32 a = i;
        s32 b;
        s32 post;

        dum (i < (s32)doc.mensura && doc.datum[i] != '\n')
        {
            i = i + I;
        }
        b     = i;
        post  = (i < (s32)doc.mensura) ? i + I : i;
        si (   b - a            >= II && doc.datum[a] == '|'
            && doc.datum[a + I] == ' ')
        {
            s32 k       = a + II;
            i32 spatia  = ZEPHYRUM;

            dum (k < b && doc.datum[k] == ' ')
            {
                k       = k + I;
                spatia  = spatia + I;
            }
            si (n < maximum)
            {
                lineae[n].a        = k;
                lineae[n].b        = b;
                lineae[n].gradus   = spatia / II;
                lineae[n].delenda  = FALSUM;
                n                  = n + I;
            }
        }
        alioquin si (n > ZEPHYRUM)
        {
            lineae[n - I].b = b;   /* continuatio textus */
        }
        i = post;
    }
    redde n;
}

hic_manens b32
_linea_est (
                   chorda  doc,
    constans LineaArboris* l,
       constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde (b32)((i32)(l->b - l->a) == n
                && memcmp(doc.datum + l->a, litterae, (size_t)n)
                    == ZEPHYRUM);
}

/* html/head/body synthesizata tollere: elementum quod initus non
 * nominat deletur, subarbor eius gradu uno sursum. */
hic_manens vacuum
_exuere (
          chorda  doc,
    LineaArboris* l,
             i32  n,
          chorda  initus)
{
    b32 html         = _initus_nominat(initus, "html");
    b32 head         = _initus_nominat(initus, "head");
    b32 body         = _initus_nominat(initus, "body");
    i32 gradus_html  = ZEPHYRUM;
    i32 i;

    si (!html)
    {
        per (i = ZEPHYRUM; i < n; i++)
        {
            si (   l[i].gradus == ZEPHYRUM
                && _linea_est(doc, &l[i], "<html>"))
            {
                i32 k;

                l[i].delenda = VERUM;
                per (k = i + I; k < n && l[k].gradus > ZEPHYRUM; k++)
                {
                    l[k].gradus = l[k].gradus - I;
                }
                frange;
            }
        }
    }
    alioquin
    {
        gradus_html = I;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (l[i].delenda || l[i].gradus != gradus_html)
        {
            perge;
        }
        si (   (!head && _linea_est(doc, &l[i], "<head>"))
            || (!body && _linea_est(doc, &l[i], "<body>")))
        {
            i32 k;

            l[i].delenda = VERUM;
            per (k = i + I; k < n && l[k].gradus > gradus_html; k++)
            {
                l[k].gradus = l[k].gradus - I;
            }
        }
    }
}

hic_manens chorda
_lineas_scribere (
         Piscina* piscina,
          chorda  doc,
    LineaArboris* l,
             i32  n)
{
    ChordaAedificator* aed;
                  b32  prima = VERUM;
                  i32  i;

    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)doc.mensura + LXIV);
    per (i = ZEPHYRUM; i < n; i++)
    {
        chorda t;
           i32 k;

        si (l[i].delenda)
        {
            perge;
        }
        si (!prima)
        {
            chorda_aedificator_appendere_character(aed, '\n');
        }
        prima = FALSUM;
        chorda_aedificator_appendere_literis(aed, "| ");
        per (k = ZEPHYRUM; k < l[i].gradus * II; k++)
        {
            chorda_aedificator_appendere_character(aed, ' ');
        }
        t.datum    = doc.datum + l[i].a;
        t.mensura  = (i32)(l[i].b - l[i].a);
        chorda_aedificator_appendere_chorda(aed, t);
    }
    redde chorda_aedificator_finire(aed);
}

/* Contentum alienum contextus fragmenti: 'svg desc' -> liberi elementi
 * desc in svg (punctum integrationis -> HTML); 'td' -> HTML. */
hic_manens HtmlAlienum
_alienum_contextus (
    Piscina* piscina,
     chorda  contextus)
{
    HtmlAlienum parentis  = HTML_ALIENUM_NULLUM;
         chorda titulus   = contextus;
            i32 i;

    per (i = ZEPHYRUM; i < contextus.mensura; i++)
    {
        si (contextus.datum[i] == ' ')
        {
            chorda spatium;

            spatium.datum    = contextus.datum;
            spatium.mensura  = i;
            si (chorda_aequalis_literis(spatium, "svg"))
            {
                parentis = HTML_ALIENUM_SVG;
            }
            alioquin si (chorda_aequalis_literis(spatium, "math"))
            {
                parentis = HTML_ALIENUM_MATHEMATICA;
            }
            titulus.datum    = contextus.datum + i + I;
            titulus.mensura  = contextus.mensura - i - I;
            frange;
        }
    }
    titulus = html_coctum_titulus(piscina, titulus, parentis);
    redde html_coctum_alienum_liberorum(parentis, titulus, NIHIL);
}

/* linea k (ZEPHYRUM-basata) chordae; vacua ultra finem */
hic_manens chorda
_linea_k (
    chorda c,
       i32 k)
{
    chorda l;
       s32 i = ZEPHYRUM;
       i32 n = ZEPHYRUM;

    dum (n < k && i < (s32)c.mensura)
    {
        si (c.datum[i] == '\n')
        {
            n = n + I;
        }
        i = i + I;
    }
    l.datum    = c.datum + i;
    l.mensura  = ZEPHYRUM;
    si (n < k)
    {
        redde l;
    }
    dum (   i + (s32)l.mensura < (s32)c.mensura
         && c.datum[i + (s32)l.mensura] != '\n')
    {
        l.mensura = l.mensura + I;
    }
    redde l;
}

/* Linea prima divergens utriusque imprimere */
hic_manens vacuum
_differentiam_imprimere (
    chorda sperata,
    chorda nostra)
{
       i32 d = ZEPHYRUM;
       i32 k = ZEPHYRUM;
       i32 i;
    chorda a;
    chorda b;

    dum (   d < sperata.mensura && d < nostra.mensura
         && sperata.datum[d] == nostra.datum[d])
    {
        d = d + I;
    }
    per (i = ZEPHYRUM; i < d; i++)
    {
        si (sperata.datum[i] == '\n')
        {
            k = k + I;
        }
    }
    a = _linea_k(sperata, k);
    b = _linea_k(nostra, k);
    imprimere("\n    linea %d  sperata: %.*s\n"
              "    linea %d  nostra:  %.*s\n",
        (integer)(k + I), (integer)(a.mensura < C ? a.mensura : C),
        (constans character*)a.datum,
        (integer)(k + I), (integer)(b.mensura < C ? b.mensura : C),
        (constans character*)b.datum);
}

integer
principale (vacuum)
{
                Piscina* piscina;
     constans character* radix;
              character  via[DXII];
                  Summa* summae;
                    i32  numerus_summarum      = ZEPHYRUM;
                    i32  totalis               = ZEPHYRUM;
                    i32  praeterita            = ZEPHYRUM;
                    i32  fragmenta             = ZEPHYRUM;
                    i32  fragmenta_praeterita  = ZEPHYRUM;
                    i32  omissa                = ZEPHYRUM;
                    i32  nihil_reddita         = ZEPHYRUM;
                    i32  fractae_impressae     = ZEPHYRUM;
     constans character* exemplum_petitum =
         getenv("ORACULUM_EXEMPLUM");
                    b32 omnia             =
                        (b32)(getenv("ORACULUM_OMNIA")
                                                   != NIHIL);
                    i32 f;

    piscina = piscina_generare_dynamicum("probatio_html_oraculum",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    summae = (Summa*)piscina_allocare(piscina, magnitudo(Summa) * LXIV);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }

    imprimere("\n--- ORACULUM html5lib: arbor cocta contra "
              "exspectatam ---\n");
    per (f = ZEPHYRUM; HTML_EXEMPLA_PLAGULAE[f] != NIHIL; f++)
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
              Xar* exempla;
              i32  n;
              i32  i;
            Summa* s;

        sprintf(via, "%s/%s", radix, HTML_EXEMPLA_PLAGULAE[f]);
        textus = _plagulam_legere(piscina, via, &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            perge;
        }
        exempla = html_exempla_legere(piscina, textus, mensura);
        CREDO_NON_NIHIL (exempla);
        si (exempla == NIHIL)
        {
            perge;
        }
        s                 = &summae[numerus_summarum];
        numerus_summarum  = numerus_summarum + I;
        memset(s, ZEPHYRUM, magnitudo(*s));
        s->plagula = _titulus(HTML_EXEMPLA_PLAGULAE[f]);

        n = (i32)xar_numerus(exempla);
        per (i = ZEPHYRUM; i < n; i++)
        {
            HtmlExemplum* e = (HtmlExemplum*)xar_obtinere(exempla, i);
                 Piscina* p;
            MateriaNodus* arbor;
                  chorda  nostra;
                  chorda  sperata;
                     b32  sanum = FALSUM;
                     b32  petitum = FALSUM;

            si (e == NIHIL)
            {
                perge;
            }
            si (e->scriptum == HTML_EXEMPLUM_SCRIPTUM_APERTUM)
            {
                omissa = omissa + I;
                perge;
            }
            totalis     = totalis + I;
            s->totalis  = s->totalis + I;
            si (e->fragmentum)
            {
                fragmenta     = fragmenta + I;
                s->fragmenta  = s->fragmenta + I;
            }
            nostra.datum = NIHIL;
            nostra.mensura = ZEPHYRUM;
            sperata = nostra;
            p = piscina_generare_dynamicum("exemplum", 1048576);
            si (p == NIHIL)
            {
                perge;
            }
            arbor = html_arbor_parsare(p,
                (constans character*)e->datum.datum,
                e->datum.mensura);
            si (arbor == NIHIL)
            {
                nihil_reddita = nihil_reddita + I;
            }
            alioquin
            {
                LineaArboris* lineae;
                         i32  numerus_linearum;
                 HtmlAlienum  alienum = HTML_ALIENUM_NULLUM;

                si (e->fragmentum)
                {
                    alienum = _alienum_contextus(p, e->contextus);
                }
                nostra = html_coctum_scribere(p, arbor, alienum);
                lineae = (LineaArboris*)piscina_allocare(p,
                    magnitudo(LineaArboris)
                    * (memoriae_index)LINEAE_MAXIMAE);
                numerus_linearum = _lineas_legere(e->documentum, lineae,
                    LINEAE_MAXIMAE);
                si (!e->fragmentum)
                {
                    _exuere(e->documentum, lineae, numerus_linearum,
                        e->datum);
                }
                sperata = _lineas_scribere(p, e->documentum, lineae,
                    numerus_linearum);
                sanum   = (b32)(nostra.datum != NIHIL
                                && chorda_aequalis(nostra, sperata));
            }
            si (exemplum_petitum != NIHIL)
            {
                constans character* colon = strrchr(exemplum_petitum,
                    ':');

                si (   colon           != NIHIL
                    && atoi(colon + I) == (integer)e->numerus
                    && strncmp(exemplum_petitum, s->plagula,
                        (size_t)(colon - exemplum_petitum)) == ZEPHYRUM
                    && strlen(s->plagula) == (size_t)(colon
                        - exemplum_petitum))
                {
                    petitum = VERUM;
                }
            }
            si (petitum)
            {
                imprimere("\n=== EXEMPLUM %s:%d (linea %d%s%.*s) ===\n"
                          "--- initus (%d octeti) ---\n%.*s\n"
                          "--- exspectatum crudum ---\n%.*s\n"
                          "--- sperata (exuta) ---\n%.*s\n"
                          "--- nostra ---\n%.*s\n",
                    s->plagula, (integer)e->numerus, (integer)e->linea,
                    e->fragmentum ? ", fragmentum " : "",
                    (integer)e->contextus.mensura,
                    (constans character*)e->contextus.datum,
                    (integer)e->datum.mensura,
                    (integer)e->datum.mensura,
                    (constans character*)e->datum.datum,
                    (integer)e->documentum.mensura,
                    (constans character*)e->documentum.datum,
                    (integer)sperata.mensura,
                    (constans character*)sperata.datum,
                    (integer)nostra.mensura,
                    (constans character*)nostra.datum);
            }
            si (sanum)
            {
                praeterita     = praeterita + I;
                s->praeterita  = s->praeterita + I;
                si (e->fragmentum)
                {
                    fragmenta_praeterita = fragmenta_praeterita + I;
                    s->fragmenta_praeterita = s->fragmenta_praeterita
                        + I;
                }
            }
            alioquin si (fractae_impressae < XL || omnia)
            {
                fractae_impressae = fractae_impressae + I;
                imprimere("  %s #%d", s->plagula, (integer)e->numerus);
                si (e->fragmentum)
                {
                    imprimere(" [%.*s]", (integer)e->contextus.mensura,
                        (constans character*)e->contextus.datum);
                }
                si (arbor == NIHIL)
                {
                    imprimere("  PARSATOR NIHIL\n");
                }
                alioquin
                {
                    _differentiam_imprimere(sperata, nostra);
                }
            }
            piscina_destruere(p);
        }
    }

    imprimere("\n--- per plagulam (praeterita/totalis; "
              "fragmenta) ---\n");
    {
        i32 i;

        per (i = ZEPHYRUM; i < numerus_summarum; i++)
        {
            imprimere("  %4d/%-4d", (integer)summae[i].praeterita,
                (integer)summae[i].totalis);
            si (summae[i].fragmenta > ZEPHYRUM)
            {
                imprimere("  (%3d/%-3d fr)",
                    (integer)summae[i].fragmenta_praeterita,
                    (integer)summae[i].fragmenta);
            }
            alioquin
            {
                imprimere("               ");
            }
            imprimere("  %s\n", summae[i].plagula);
        }
    }
    imprimere("\n--- ORACULUM: %d/%d praeterita (pinna %d); fragmenta "
              "%d/%d; script-on omissa %d; parsator NIHIL %d ---\n",
        (integer)praeterita, (integer)totalis,
        (integer)PINNA_PRAETERITA,
        (integer)fragmenta_praeterita, (integer)fragmenta,
        (integer)omissa, (integer)nihil_reddita);
    CREDO_AEQUALIS_I32 (totalis, TOTALIS_PINNATUS);
    CREDO_AEQUALIS_I32 (nihil_reddita, ZEPHYRUM);
    CREDO_MAIOR_AUT_AEQUALIS_S32 ((s32)praeterita, PINNA_PRAETERITA);

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
