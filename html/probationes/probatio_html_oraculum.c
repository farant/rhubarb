/* probatio_html_oraculum.c - Oraculum html5lib (O2): arbor nostra
 * COCTA contra '#document' exspectatum
 *
 * Quodque exemplum .dat (html_exempla) parsatur (html_arbor_parsare;
 * fragmenta per html_arbor_parsare_fragmentum cum contextu suo), per
 * html_coctum in formam html5lib scribitur et octetim cum arbore
 * exspectata confertur INTEGRA. Involucra html/head/body quae arbor
 * exspectata fert aedificator ipse FINGIT (O7a, locus synthesis) -
 * regula exuendi O2 (elementum quod initus non nominat ex exspectata
 * sublatum) RETIRATA. Exempla '#script-on' OMITTUNTUR (aedificator =
 * scriptum clausum). PINNA CRESCENS: praeterita minui non possunt;
 * tabula per plagulam = mechanismi HTML5 per plagulas iam distributi
 * (adoption, tables, template, foreign, entities, doctype,
 * comments...). ORACULUM_OMNIA=1 omnes fracturas imprimit;
 * ORACULUM_EXEMPLUM=plagula:n initum, exspectatum et nostrum
 * imprimit.
 */

#include "latina.h"
#include "credo.h"
#include "html_arbor.h"
#include "html_coctum.h"
#include "html_alienum.h"
#include "html_exempla.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* pinna: praeterita minima (nativitas: valor post cursum primum; O7a
 * 2026-09-15: 1195 -> 1299, involucra et partes tabulae fictae; O7b
 * 2026-09-16: 1299 -> 1398, modi tabularum et sedes; 2026-09-19:
 * 1504 -> 1509, NUL in modis involucri NEGLECTUS ut spec iubet -
 * contentum erat, ergo corpus fingebatur et '<frameset>' sequens
 * peribat) */
#define PINNA_PRAETERITA   ((s32)1509)
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
                lineae[n].a       = k;
                lineae[n].b       = b;
                lineae[n].gradus  = spatia / II;
                n                 = n + I;
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

/* Lineas normatas rescribere ('| ' + gradus * II spatia + textus,
 * '\n' inter lineas, sine ultimo) - forma quam html_coctum scribit. */
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

/* Contentum contextus fragmenti: 'svg desc' -> parens SVG (desc),
 * liberi HTML (punctum integrationis); 'math ms' -> MATHEMATICA,
 * NULLUM; 'td' -> NULLUM, NULLUM. *titulus_contextus = titulus sine
 * spatio ('desc', 'td') pro aedificatore. */
hic_manens vacuum
_alienum_contextus (
        Piscina* piscina,
         chorda  contextus,
    HtmlAlienum* parentis,
    HtmlAlienum* liberorum,
         chorda* titulus_contextus)
{
    HtmlAlienum spatium;
         chorda titulus;

    html_exempla_contextus(piscina, contextus, &spatium, &titulus);
    *parentis = spatium;
    *liberorum = html_alienum_liberorum(spatium, titulus,
        NIHIL);
    *titulus_contextus = titulus;
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
            {
                HtmlAlienum parentis  = HTML_ALIENUM_NULLUM;
                HtmlAlienum alienum   = HTML_ALIENUM_NULLUM;
                     chorda titulus_contextus;

                titulus_contextus.datum    = NIHIL;
                titulus_contextus.mensura  = ZEPHYRUM;
                si (e->fragmentum)
                {
                    _alienum_contextus(p, e->contextus, &parentis,
                        &alienum, &titulus_contextus);
                    arbor = html_arbor_parsare_fragmentum(p,
                        (constans character*)e->datum.datum,
                        e->datum.mensura, titulus_contextus, parentis);
                }
                alioquin
                {
                    arbor = html_arbor_parsare(p,
                        (constans character*)e->datum.datum,
                        e->datum.mensura);
                }
                si (arbor == NIHIL)
                {
                    nihil_reddita = nihil_reddita + I;
                }
                alioquin
                {
                    LineaArboris* lineae;
                             i32  numerus_linearum;

                    nostra = html_coctum_scribere(p, arbor, parentis,
                        alienum);
                    lineae = (LineaArboris*)piscina_allocare(p,
                        magnitudo(LineaArboris)
                        * (memoriae_index)LINEAE_MAXIMAE);
                    numerus_linearum = _lineas_legere(e->documentum,
                        lineae, LINEAE_MAXIMAE);
                    sperata = _lineas_scribere(p, e->documentum, lineae,
                        numerus_linearum);
                    sanum   = (b32)(nostra.datum != NIHIL
                                    && chorda_aequalis(nostra,
                                    sperata));
                }
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
                          "--- sperata (normata) ---\n%.*s\n"
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
