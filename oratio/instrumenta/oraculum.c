/* oraculum.c - Instrumentum oraculi treebank (T13, oratio/oraculum.sh)
 *
 * Usus: oraculum [-machina] [-exempla] [-discrepantiae] [-regulae]
 *              [-crudus] [plagula.conllu ...]
 *   -regulae   tabula CUMULATIVA: primarium per thesaurum post regulas
 *              0, 1, ... N programmatis resolutionis (T17)
  *   -crudus    sine resolutione (ordo fontis)
 *   -ab N      cum -regulae: ordines a regulis N solum (priores omissi)
 *   sine plagulis: fixturae venditae oratio/probationes/fixa/ud
 *   -machina   TSV (plagula, classis, verba, tecta, primaria, lemmata,
 *              ignota, inalignata); cum -discrepantiae ordines
 *              'plagula DISCREPANTIA aurea forma nostra numerus'
 *   -exempla   verba non tecta prima V per classem
 *   -errata    per auctorem (regula decidens, T19g bis): decisiones
 *              FALSAE - forma, classis aurea, nostra prima, socius
 *              ligationis, numerus (data emendationis regularum);
   *              -machina ordines 'plagula ERRATUM auctor forma aurea
 *              nostra socius distantia numerus'; ordines AUCTOR
 *              'titulus verba primaria vicina vicina-primaria remota
 *              remota-primaria' (socius ad distantiam I | ultra)
 *   -auctor T  cum -errata: auctoris T solius
  *   -discrepantiae  per classem auream: formae TECTAE sed non primae
 *              cum classe nostra prima et numero, XII frequentissimae
 *              (T19a: data regularum priorum classium)
 *   -clausulae tabula CLAUSULARUM (T20a): puritas per causam (semen
 *              extentum clausura catena unica; apertae), coactio,
 *              numerus clausularum nostrarum/aurearum, sententiae
 *              paribus; cum -errata: verba male posita (causa, forma,
 *              species nostra, radix aurea, numerus); -machina ordines
 *              CLAUSULA causa verba rectae, CLAUSULAE sententiae
 *              nostrae aureae pares, ERRATUM-CLAUSULAE ...
  *   -sententiae N  cum -clausulae: N sententiae cum verbo male posito
 *              (-causa T: causae T solius - semen extentum clausura
 *              catena unica verbum), textus et verba aurea ordine
 *              'forma/clausula:causa:radix' ('!' malum) - contextus
 *              quem index aggregatus celat (T20a quater)
 *   -semina    census AUREUS candidatorum seminis (T20a; sine parsura):

 *              textus, upos, n, limes %, finita %, initia, ff, ff-limes
 *              - data listarum ORATIO_SEMINA_*; -machina ordines SEMEN
 *
 * Vocabularia (la.bin + glossarium + Moby) ex RHUBARB_RADIX. Exitus:
 * 0 relatio scripta | 2 usus/tabula/plagula absens
 */

#include "latina.h"
#include "oratio_conllu.h"
#include "oratio_oraculum.h"
#include "oratio_registrum.h"
#include "oratio_vocabularia.h"
#include "oratio_resolutio.h"
#include "internamentum.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (i8*)piscina_allocare(piscina, (memoriae_index)longitudo
        + I);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

interior duplex
_pars (
    i32 a,
    i32 b)
{
    redde b > ZEPHYRUM ? 100.0 * (duplex)a / (duplex)b : 0.0;
}

#define DISCREPANTIAE_IMPRESSAE 12
#define ERRATA_IMPRESSA         24
#define SEMINA_IMPRESSA         48

/* T20a: tabula clausularum (puritas per causam, numeri) */
interior vacuum
_clausulas_imprimere (
                          Piscina* piscina,
    constans OratioOraculumCensus* c,
                              b32  errata)
{
    i32 i;
    i32 positae = ZEPHYRUM;

    si (c->clausulae_iudicata == ZEPHYRUM)
    {
        redde;
    }
    imprimere("  clausulae (puritas per causam; verba aurea alignata"
        " non interpunctio):\n");
    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS; i++)
    {
        si (c->clausulae_verba[i] == ZEPHYRUM)
        {
            perge;
        }
        si (i < (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS)
        {
            positae = positae + c->clausulae_verba[i];
        }
        imprimere("    %-10s %6d %5.1f%%  puritas %5.1f%%\n",
            i < (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS
                ? ORATIO_TITULI_CAUSARUM_CLAUSULAE[i] : "apertae",
            (integer)c->clausulae_verba[i],
            _pars(c->clausulae_verba[i], c->clausulae_iudicata),
            _pars(c->clausulae_rectae[i], c->clausulae_verba[i]));
    }
            /* ut porta: permille truncatum et numeri exacti (T20a sexies) */
    imprimere("    concordia parium %d permille - %d de %d paria\n",
        (integer)(c->clausulae_paria > ZEPHYRUM
            ? (s64)c->clausulae_paria_concordia * (s64)1000
                / (s64)c->clausulae_paria : (s64)ZEPHYRUM),
        (integer)c->clausulae_paria_concordia,
        (integer)c->clausulae_paria);
    imprimere("    coactio %5.1f%% (%d/%d)  clausulae nostrae %d aureae %d"
        " (%.2f / %.2f per sententiam)  sententiae pares %5.1f%%\n",
        _pars(positae, c->clausulae_iudicata), (integer)positae,
        (integer)c->clausulae_iudicata, (integer)c->clausulae_nostrae,
        (integer)c->clausulae_aureae,
        c->clausulae_sententiae > ZEPHYRUM
            ? (duplex)c->clausulae_nostrae
                / (duplex)c->clausulae_sententiae : 0.0,
        c->clausulae_sententiae > ZEPHYRUM
            ? (duplex)c->clausulae_aureae
                / (duplex)c->clausulae_sententiae : 0.0,
        _pars(c->clausulae_pares, c->clausulae_sententiae));
    si (errata)
    {
        Xar* es = oratio_oraculum_errata_clausularum(piscina, c);
        i32  m;
        i32  aliae = ZEPHYRUM;

        per (m = ZEPHYRUM; es != NIHIL && m < xar_numerus(es); m++)
        {
            constans OratioOraculumErratumClausulae* d =
                *(OratioOraculumErratumClausulae**)xar_obtinere(es, m);

            si (m >= (i32)ERRATA_IMPRESSA)
            {
                aliae = aliae + d->numerus;
                perge;
            }
            imprimere("      %-9s %-16.*s species %-12s radix aurea %-16.*s"
                " %5d\n",
                d->causa >= ZEPHYRUM
                    && d->causa < (s32)ORATIO_CLAUSULA_CAUSA_NUMERUS
                    ? ORATIO_TITULI_CAUSARUM_CLAUSULAE[d->causa] : "?",
                (integer)d->forma.mensura,
                (constans character*)d->forma.datum,
                d->species >= ZEPHYRUM
                    && d->species
                        < (s32)ORATIO_SPECIES_CLAUSULAE_NUMERUS
                    ? ORATIO_TITULI_SPECIERUM_CLAUSULAE[d->species]
                    : "(nulla)",
                (integer)d->radix.mensura,
                (constans character*)d->radix.datum,
                (integer)d->numerus);
        }
        si (aliae > ZEPHYRUM)
        {
            imprimere("      ... alia %d verba\n", (integer)aliae);
        }
    }
}

/* T20a quater: sententiae ostensae (verba male posita in contextu) */
interior vacuum
_sententias_imprimere (
    constans OratioOraculumCensus* c)
{
    i32 i;

    si (c->ostensae == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(c->ostensae); i++)
    {
        constans OratioOraculumSententiaOstensa* o =
            (constans OratioOraculumSententiaOstensa*)xar_obtinere(
            c->ostensae, i);
        i32 k;

        imprimere("  === %.*s\n", (integer)o->textus.mensura,
            (constans character*)o->textus.datum);
        imprimere("     ");
        per (k = ZEPHYRUM; k < xar_numerus(o->verba); k++)
        {
            constans OratioOraculumVerbumOstensum* v =
                (constans OratioOraculumVerbumOstensum*)xar_obtinere(
                o->verba, k);

            imprimere(" %s%.*s/", v->malum ? "!" : "",
                (integer)v->forma.mensura,
                (constans character*)v->forma.datum);
            si (v->nostra >= ZEPHYRUM)
            {
                imprimere("%d", (integer)v->nostra);
            }
            alioquin
            {
                imprimere("-");
            }
            imprimere(":%.2s:%.*s",
                v->causa >= ZEPHYRUM
                    && v->causa < (s32)ORATIO_CLAUSULA_CAUSA_NUMERUS
                    ? ORATIO_TITULI_CAUSARUM_CLAUSULAE[v->causa] : "--",
                (integer)v->radix.mensura,
                (constans character*)v->radix.datum);
        }
        imprimere("\n");
    }
}

/* T20a: census aureus seminum (sine parsura) */
interior vacuum
_semina_imprimere (
               Piscina* piscina,
                   Xar* sententiae,
                   b32  machina,
    constans character* titulus)
{
    Xar* semina = oratio_oraculum_semina(piscina, sententiae);
    i32  m;

    si (semina == NIHIL)
    {
        redde;
    }
    si (!machina)
    {
        imprimere("  semina aurea (textus upos n limes%% finita%% initia"
            " ff ff-limes%%):\n");
    }
    per (m = ZEPHYRUM; m < xar_numerus(semina); m++)
    {
        constans OratioOraculumSemen* s =
            *(OratioOraculumSemen**)xar_obtinere(semina, m);

        si (machina)
        {
            imprimere("%s\tSEMEN\t%.*s\t%.*s\t%d\t%d\t%d\t%d\t%d\t%d\n",
                titulus, (integer)s->upos.mensura,
                (constans character*)s->upos.datum,
                (integer)s->textus.mensura,
                (constans character*)s->textus.datum, (integer)s->n,
                (integer)s->limes, (integer)s->finita,
                (integer)s->initia, (integer)s->ff,
                (integer)s->ff_limes);
            perge;
        }
        si (m >= (i32)SEMINA_IMPRESSA || s->n < (i32)III)
        {
            perge;
        }
        imprimere("    %-14.*s %-6.*s %6d %5.1f%% %5.1f%% %4d %5d %5.1f%%\n",
            (integer)s->textus.mensura,
            (constans character*)s->textus.datum,
            (integer)s->upos.mensura,
            (constans character*)s->upos.datum, (integer)s->n,
            _pars(s->limes, s->n), _pars(s->finita, s->n),
            (integer)s->initia, (integer)s->ff,
            _pars(s->ff_limes, s->ff));
    }
}

interior vacuum
_discrepantias_imprimere (
                          Piscina* piscina,
    constans OratioOraculumCensus* c,
                    OratioClassis  aurea)
{
    Xar* ds = oratio_oraculum_discrepantiae(piscina, c, aurea);
    i32  n;
    i32  j;
    i32  aliae_formae  = ZEPHYRUM;
    i32  aliae_verba   = ZEPHYRUM;

    si (ds == NIHIL)
    {
        redde;
    }
    n = xar_numerus(ds);
    per (j = ZEPHYRUM; j < n; j++)
    {
        constans OratioOraculumDiscrepantia* d =
            *(OratioOraculumDiscrepantia**)xar_obtinere(ds, j);

        si (j < (i32)DISCREPANTIAE_IMPRESSAE)
        {
            imprimere("      %-20.*s  nostra prima: %-24s %6d\n",
                (integer)d->forma.mensura,
                (constans character*)d->forma.datum,
                (i32)d->nostra < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                    ? oratio_classis_titulus(d->nostra) : "(nulla)",
                (integer)d->numerus);
        }
        alioquin
        {
            aliae_formae  = aliae_formae + I;
            aliae_verba   = aliae_verba + d->numerus;
        }
    }
    si (aliae_formae > ZEPHYRUM)
    {
        imprimere("      ... aliae formae %d (verba %d)\n",
            (integer)aliae_formae, (integer)aliae_verba);
    }
}

interior vacuum
_tabulam_imprimere (
                          Piscina* piscina,
    constans OratioOraculumCensus* c,
               constans character* titulus,
                              b32  exempla,
                              b32  discrepantiae,
                              b32  errata,
                           chorda  auctor_petitus,
                              b32  clausulae)
{
    i32 i;


    imprimere("--- %s: sententiae %d (fractae %d)  verba %d  rangae %d ---\n",
        titulus, (integer)c->sententiae, (integer)c->sententiae_fractae,
        (integer)c->verba, (integer)c->rangae);
        imprimere("  TECTA %.1f%%  primaria %.1f%%  lemmata %.1f%%  ignota %.1f%%  inalignata %d\n",
            _pars(c->tecta, c->verba), _pars(c->primaria, c->verba),
            _pars(c->lemmata, c->verba), _pars(c->ignota, c->verba),
            (integer)c->inalignata);
        imprimere("  sententiae censae: latina %d  anglica %d\n",
            (integer)c->sententiae_linguae[ORATIO_LINGUA_LATINA],
            (integer)c->sententiae_linguae[ORATIO_LINGUA_ANGLICA]);
    si (c->lingua_documenti >= ZEPHYRUM)
    {
        imprimere("  lingua documenti: %s (suffragia latina %d  anglica %d)\n",
            ORATIO_TITULI_LINGUARUM[(i32)c->lingua_documenti],
            (integer)c->suffragia_linguarum[ORATIO_LINGUA_LATINA],
            (integer)c->suffragia_linguarum[ORATIO_LINGUA_ANGLICA]);
    }
        /* T19g: partitio decisionum (decretum SUDOKU XL) */
    imprimere("  partitio decisionum (elementum primum verbi aurei):\n");
    per (i = ZEPHYRUM; i < ORATIO_ORACULUM_PARTITIO_NUMERUS; i++)
    {
        si (c->partitio_verba[i] == ZEPHYRUM)
        {
            perge;
        }
        imprimere("    %-12s %6d %5.1f%%  primaria %5.1f%%\n",
            ORATIO_ORACULUM_TITULI_PARTITIONIS[i],
            (integer)c->partitio_verba[i],
            _pars(c->partitio_verba[i], c->verba),
            _pars(c->partitio_primaria[i], c->partitio_verba[i]));
    }
    {
        i32 coactae = c->partitio_verba[ORATIO_DECISIO_IMPLETIO]
            + c->partitio_verba[ORATIO_DECISIO_UMBRA];
        i32 rectae = c->partitio_primaria[ORATIO_DECISIO_IMPLETIO]
            + c->partitio_primaria[ORATIO_DECISIO_UMBRA];

                imprimere("    %-12s %6d %5.1f%%  primaria %5.1f%%\n",
                    "coactae",
                    (integer)coactae, _pars(coactae, c->verba),
                    _pars(rectae, coactae));
    }
    /* T19g: accuratio per auctorem (regula decidens) */
    {
        Xar* auctores = oratio_oraculum_auctores(piscina, c);
        i32  j;

                si (   auctores != NIHIL
                    && xar_numerus(auctores) > ZEPHYRUM)
                {
            imprimere("  auctores (regula decidens: verba, primaria; socius"
                " vicinus | remotus):\n");
                }

        per (j = ZEPHYRUM; auctores != NIHIL
            && j < xar_numerus(auctores);
             j++)
        {
            constans OratioOraculumAuctor* a =
                *(OratioOraculumAuctor**)xar_obtinere(auctores, j);

                                    imprimere("    %-44.*s %6d  primaria %5.1f%%"
                                        "  vicina %5d %5.1f%%  remota %5d %5.1f%%\n",
                                        (integer)a->titulus.mensura,
                                        (constans character*)a->titulus.datum,
                                        (integer)a->verba,
                                        _pars(a->primaria, a->verba),
                                        (integer)a->vicina,
                                        _pars(a->vicina_primaria,
                                        a->vicina),
                                        (integer)a->remota,
                                        _pars(a->remota_primaria,
                                        a->remota));
        }
        /* T19g bis: errata per auctorem - decisiones falsae cum
         * socio ligationis, frequentissimae primae */
        per (j = ZEPHYRUM; errata && auctores != NIHIL
             && j < xar_numerus(auctores); j++)
        {
            constans OratioOraculumAuctor* a =
                *(OratioOraculumAuctor**)xar_obtinere(auctores, j);
            Xar* es;
            i32  m;
            i32  aliae_formae  = ZEPHYRUM;
            i32  aliae_verba   = ZEPHYRUM;

            si (   auctor_petitus.mensura > ZEPHYRUM
                && chorda_comparare(a->titulus, auctor_petitus)
                    != ZEPHYRUM)
            {
                perge;
            }
            es = oratio_oraculum_errata(piscina, c, a->titulus);
            si (es == NIHIL || xar_numerus(es) == ZEPHYRUM)
            {
                perge;
            }
            imprimere("  errata %.*s (%d verba, %d falsa):\n",
                (integer)a->titulus.mensura,
                (constans character*)a->titulus.datum,
                (integer)a->verba, (integer)(a->verba - a->primaria));
            per (m = ZEPHYRUM; m < xar_numerus(es); m++)
            {
                constans OratioOraculumErratum* d =
                    *(OratioOraculumErratum**)xar_obtinere(es, m);

                si (m < (i32)ERRATA_IMPRESSA)
                {
                                        imprimere("      %-18.*s aurea %-14s nostra %-14s"
                                            " socius %-16.*s @%-2d %5d\n",
                                            (integer)d->forma.mensura,
                                            (constans character*)d->forma.datum,
                                            oratio_classis_titulus(d->aurea)
                                                != NIHIL
                                            ? oratio_classis_titulus(d->aurea) : "(extra)",
                                            (i32)d->nostra
                                            < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                                            ? oratio_classis_titulus(d->nostra) : "(nulla)",
                                            d->socius.mensura > ZEPHYRUM
                                            ? (integer)d->socius.mensura : (integer)I,
                                                d->socius.mensura
                                                    > ZEPHYRUM
                                            ? (constans character*)d->socius.datum : "-",
                                            (integer)d->distantia,
                                            (integer)d->numerus);
                }
                alioquin
                {
                    aliae_formae  = aliae_formae + I;
                    aliae_verba   = aliae_verba + d->numerus;
                }
            }
            si (aliae_formae > ZEPHYRUM)
            {
                imprimere("      ... aliae %d formae, %d verba\n",
                    (integer)aliae_formae, (integer)aliae_verba);
            }
        }
    }


        /* T20a: clausulae */
    si (clausulae)
    {
        _clausulas_imprimere(piscina, c, errata);
    }
    imprimere("  %-24s %6s %7s %8s %8s %7s\n", "classis aurea", "verba",
        "tecta", "primaria", "lemmata", "ignota");
    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        constans OratioOraculumClassis* k = &c->classes[i];
        constans character* t = i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
            ? oratio_classis_titulus((OratioClassis)i) : "(extra)";
        i32 j;

        si (k->verba == ZEPHYRUM)
        {
            perge;
        }
        imprimere("  %-24s %6d %6.1f%% %7.1f%% %7.1f%% %6.1f%%\n", t,
            (integer)k->verba, _pars(k->tecta, k->verba),
            _pars(k->primaria, k->verba), _pars(k->lemmata, k->verba),
            _pars(k->ignota, k->verba));
        si (exempla)
        {
            per (j = ZEPHYRUM; j < k->numerus_exemplorum; j++)
            {
                constans OratioOraculumExemplum* e = &k->exempla[j];

                imprimere("      %-20.*s  nostrum: %-40.*s  lemma aureum: %.*s\n",
                    (integer)e->forma.mensura,
                    (constans character*)e->forma.datum,
                    (integer)e->classes.mensura,
                    (constans character*)e->classes.datum,
                    (integer)e->lemma.mensura,
                    (constans character*)e->lemma.datum);
            }
        }
        si (discrepantiae && k->tecta > k->primaria)
        {
            _discrepantias_imprimere(piscina, c, (OratioClassis)i);
        }
    }
}

interior vacuum
_machinam_imprimere (
                          Piscina* piscina,
    constans OratioOraculumCensus* c,
               constans character* titulus,
                              b32  discrepantiae,
                              b32  errata,
                           chorda  auctor_petitus)
{
    i32 i;


    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        constans OratioOraculumClassis* k = &c->classes[i];

        si (k->verba == ZEPHYRUM)
        {
            perge;
        }
        imprimere("%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", titulus,
            i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                ? oratio_classis_titulus((OratioClassis)i) : "extra",
            (integer)k->verba, (integer)k->tecta, (integer)k->primaria,
            (integer)k->lemmata, (integer)k->ignota,
            (integer)k->inalignata);
    }
        imprimere("%s\tSUMMA\t%d\t%d\t%d\t%d\t%d\t%d\n", titulus,
            (integer)c->verba, (integer)c->tecta, (integer)c->primaria,
            (integer)c->lemmata, (integer)c->ignota,
            (integer)c->inalignata);
        /* T19g: ordines PARTITIO genus verba primaria; AUCTOR titulus verba
     * primaria */
    per (i = ZEPHYRUM; i < ORATIO_ORACULUM_PARTITIO_NUMERUS; i++)
    {
        imprimere("%s\tPARTITIO\t%s\t%d\t%d\n", titulus,
            ORATIO_ORACULUM_TITULI_PARTITIONIS[i],
            (integer)c->partitio_verba[i],
            (integer)c->partitio_primaria[i]);
    }
    {
        Xar* auctores = oratio_oraculum_auctores(piscina, c);
        i32  j;

        per (j = ZEPHYRUM; auctores != NIHIL
            && j < xar_numerus(auctores);
             j++)
        {
            constans OratioOraculumAuctor* a =
                *(OratioOraculumAuctor**)xar_obtinere(auctores, j);

                                    imprimere("%s\tAUCTOR\t%.*s\t%d\t%d\t%d\t%d\t%d\t%d\n",
                                        titulus,
                                        (integer)a->titulus.mensura,
                                        (constans character*)a->titulus.datum,
                                        (integer)a->verba,
                                        (integer)a->primaria,
                                        (integer)a->vicina,
                                        (integer)a->vicina_primaria,
                                        (integer)a->remota,
                                        (integer)a->remota_primaria);
        }
    }
        /* T20a: ordines CLAUSULA causa verba rectae; CLAUSULAE sententiae
     * nostrae aureae pares; ERRATUM-CLAUSULAE causa forma species radix
     * numerus */
    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS; i++)
    {
        imprimere("%s\tCLAUSULA\t%s\t%d\t%d\n", titulus,
            i < (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS
                ? ORATIO_TITULI_CAUSARUM_CLAUSULAE[i] : "apertae",
            (integer)c->clausulae_verba[i],
            (integer)c->clausulae_rectae[i]);
    }
        imprimere("%s\tCLAUSULAE\t%d\t%d\t%d\t%d\t%d\t%d\n", titulus,
            (integer)c->clausulae_sententiae,
            (integer)c->clausulae_nostrae,
            (integer)c->clausulae_aureae, (integer)c->clausulae_pares,
            (integer)c->clausulae_paria,
            (integer)c->clausulae_paria_concordia);
    si (errata)
    {
        Xar* es = oratio_oraculum_errata_clausularum(piscina, c);
        i32  m;

        per (m = ZEPHYRUM; es != NIHIL && m < xar_numerus(es); m++)
        {
            constans OratioOraculumErratumClausulae* d =
                *(OratioOraculumErratumClausulae**)xar_obtinere(es, m);

            imprimere("%s\tERRATUM-CLAUSULAE\t%s\t%.*s\t%s\t%.*s\t%d\n",
                titulus,
                d->causa >= ZEPHYRUM
                    && d->causa < (s32)ORATIO_CLAUSULA_CAUSA_NUMERUS
                    ? ORATIO_TITULI_CAUSARUM_CLAUSULAE[d->causa] : "?",
                (integer)d->forma.mensura,
                (constans character*)d->forma.datum,
                d->species >= ZEPHYRUM
                    && d->species
                        < (s32)ORATIO_SPECIES_CLAUSULAE_NUMERUS
                    ? ORATIO_TITULI_SPECIERUM_CLAUSULAE[d->species]
                    : "nulla",
                (integer)d->radix.mensura,
                (constans character*)d->radix.datum,
                (integer)d->numerus);
        }
    }
    /* T19g bis: ordines ERRATUM auctor forma aurea nostra socius numerus */
    si (errata)
    {
        Xar* es = oratio_oraculum_errata(piscina, c, auctor_petitus);
        i32  m;

        per (m = ZEPHYRUM; es != NIHIL && m < xar_numerus(es); m++)
        {
            constans OratioOraculumErratum* d =
                *(OratioOraculumErratum**)xar_obtinere(es, m);

                        imprimere("%s\tERRATUM\t%.*s\t%.*s\t%s\t%s\t%.*s\t%d\t%d\n",
                            titulus, (integer)d->auctor.mensura,
                            (constans character*)d->auctor.datum,
                            (integer)d->forma.mensura,
                            (constans character*)d->forma.datum,
                            oratio_classis_titulus(d->aurea) != NIHIL
                            ? oratio_classis_titulus(d->aurea) : "extra",
                            (i32)d->nostra
                                < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                            ? oratio_classis_titulus(d->nostra) : "nulla",
                            (integer)d->socius.mensura,
                                d->socius.mensura > ZEPHYRUM
                            ? (constans character*)d->socius.datum : "",
                            (integer)d->distantia, (integer)d->numerus);
        }
    }


    si (!discrepantiae)
    {
        redde;
    }
    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        Xar* ds = oratio_oraculum_discrepantiae(piscina, c,
            (OratioClassis)i);
        i32 j;

        si (ds == NIHIL)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(ds); j++)
        {
            constans OratioOraculumDiscrepantia* d =
                *(OratioOraculumDiscrepantia**)xar_obtinere(ds, j);

            imprimere("%s\tDISCREPANTIA\t%s\t%.*s\t%s\t%d\n", titulus,
                i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                    ? oratio_classis_titulus((OratioClassis)i) : "extra",
                (integer)d->forma.mensura,
                (constans character*)d->forma.datum,
                (i32)d->nostra < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                    ? oratio_classis_titulus(d->nostra) : "nulla",
                (integer)d->numerus);
        }
    }
}

integer
principale (
      integer   argc,
    character** argv)
{
                                Piscina* piscina;
                     constans character* radix;
                              character  via[1024];
                      OratioVocabularia  vocabularia;
               OratioVocabulariumVitium  vitium;
                                    b32  machina        = FALSUM;
                                    b32  exempla        = FALSUM;
                                    b32  discrepantiae  = FALSUM;
                                    b32  regulae        = FALSUM;
                                    i32  regulae_ab     = ZEPHYRUM;
                                integer  argumentum_ab  = -I;
                                                                        b32  crudus =
                                                                            FALSUM;
                                                                        b32 errata =
                                                                            FALSUM;
                                                                        b32 clausulae =
                                                                            FALSUM;   /* T20a */
                                    b32 semina = FALSUM;   /* T20a */
                                    i32 sententiae_ostendendae =
                                        ZEPHYRUM;   /* T20a quater */
                                    s32 causa_ostendenda = (s32)-I;
                                integer argumentum_sententiarum = -I;
                                integer argumentum_causae = -I;

                                integer argumentum_auctoris  = -I;
                                 chorda auctor_petitus;

                        OratioProgramma* programma      = NIHIL;
                                integer  i;
                                    i32  plagulae       = ZEPHYRUM;
    hic_manens constans character* constans venditae[] = {
        "oratio/probationes/fixa/ud/la_circse-ud-test.conllu",
        "oratio/probationes/fixa/ud/la_llct-ud-dev.conllu",
        "oratio/probationes/fixa/ud/la_llct-ud-test.conllu",
        "oratio/probationes/fixa/ud/en_ewt-ud-dev.conllu",
        "oratio/probationes/fixa/ud/en_ewt-ud-test.conllu"
    };

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    piscina = piscina_generare_dynamicum("oratio_oraculum", 536870912);
    si (!oratio_vocabularia_onerare(piscina, radix, &vocabularia,
        &vitium))
    {
        fprintf(stderr, "oraculum: vocabularia non onerata: %s:%d %s\n",
            vitium.plagula ? vitium.plagula : "?",
            (integer)vitium.linea,
            vitium.causa ? vitium.causa : "-");
        redde II;
    }
    /* T17: programma resolutionis (absens = sine resolutione) */
    si (!crudus)
    {
        InternamentumChorda* intern = internamentum_creare(piscina);

        programma = intern == NIHIL ? NIHIL
            : oratio_resolutio_programma_onerare(piscina, intern, radix,
                &vitium);
        si (   programma == NIHIL && vitium.causa != NIHIL
            && strcmp(vitium.causa, "plagula absens") != ZEPHYRUM)
        {
            fprintf(stderr,
                "oraculum: programma resolutionis: %s:%d %s\n",
                vitium.plagula ? vitium.plagula : "?",
                (integer)vitium.linea, vitium.causa);
            redde II;
        }
    }
        auctor_petitus.datum  = NIHIL;
    auctor_petitus.mensura    = ZEPHYRUM;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)

        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-exempla") == ZEPHYRUM)
        {
            exempla = VERUM;
        }
        alioquin si (strcmp(argv[i], "-discrepantiae") == ZEPHYRUM)
        {
            discrepantiae = VERUM;
        }
                alioquin si (strcmp(argv[i], "-regulae") == ZEPHYRUM)
        {
            regulae = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-ab") == ZEPHYRUM
                     && i + I < argc)
        {
            /* T19d gamma: tabula cumulativa a regulis N (ordines
             * priores omissi - cursus quisque thesaurum totum
             * iudicat, XXIX regulae x V thesauri = IX min) */
            argumentum_ab  = i + I;
            regulae_ab     = (i32)atoi(argv[argumentum_ab]);
            i              = argumentum_ab;
        }

                alioquin si (strcmp(argv[i], "-crudus") == ZEPHYRUM)
        {
            crudus = VERUM;
        }
                alioquin si (strcmp(argv[i], "-errata") == ZEPHYRUM)
        {
            errata = VERUM;
        }
        alioquin si (strcmp(argv[i], "-clausulae") == ZEPHYRUM)
        {
            clausulae = VERUM;
        }
                alioquin si (strcmp(argv[i], "-semina") == ZEPHYRUM)
        {
            semina = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-sententiae") == ZEPHYRUM
                     && i + I < argc)
        {
            argumentum_sententiarum  = i + I;
            sententiae_ostendendae   =
                (i32)atoi(argv[argumentum_sententiarum]);
            i                        = argumentum_sententiarum;
        }
        alioquin si (   strcmp(argv[i], "-causa") == ZEPHYRUM
                     && i + I < argc)
        {
            i32 t;

            argumentum_causae = i + I;
            per (t = ZEPHYRUM; t
                < (i32)ORATIO_CLAUSULA_CAUSA_NUMERUS; t++)
            {
                si (strcmp(argv[argumentum_causae],
                        ORATIO_TITULI_CAUSARUM_CLAUSULAE[t])
                            == ZEPHYRUM)
                {
                    causa_ostendenda = (s32)t;
                }
            }
            i = argumentum_causae;
        }
        alioquin si (   strcmp(argv[i], "-auctor") == ZEPHYRUM
                     && i + I < argc)
        {
                        argumentum_auctoris = i + I;
            auctor_petitus.datum = (i8*)argv[argumentum_auctoris];
            auctor_petitus.mensura  =
                (i32)strlen(argv[argumentum_auctoris]);
            i                       = argumentum_auctoris;
        }
    }

    si (machina)
    {
        imprimere("# plagula\tclassis\tverba\ttecta\tprimaria\tlemmata\tignota\tinalignata\n");
    }
    {
        i32 k;
        i32 numerus_argumentorum = ZEPHYRUM;

                per (i = I; i < argc; i++)
                {
                                                si (   argv[i][ZEPHYRUM]
                                                    != '-'
                                                    && i
                                                        != argumentum_ab
                                                    && i
                                                        != argumentum_auctoris
                                                    && i
                                                    != argumentum_sententiarum
                                                    && i
                                                        != argumentum_causae)
                                                {
                numerus_argumentorum = numerus_argumentorum + I;
                                                }
                }

        per (k = ZEPHYRUM;
             k < (numerus_argumentorum
                 > ZEPHYRUM ? (i32)argc
                 : (i32)(magnitudo(venditae)
                     / magnitudo(venditae[ZEPHYRUM]))
                     + I);
             k++)
        {
              constans character* plagula;
                          chorda  fons;
                         Piscina* p;
                             Xar* sententiae;
            OratioOraculumCensus  census;
                         clock_t  ante;

            si (numerus_argumentorum > ZEPHYRUM)
            {
                                                                si (   k
                                                                    == ZEPHYRUM
                                                                    || argv[k][ZEPHYRUM]
                                                                        == '-'
                                                                    || (integer)k
                                                                        == argumentum_ab
                                                                    || (integer)k
                                                                        == argumentum_auctoris
                                                                    || (integer)k
                                                                    == argumentum_sententiarum
                                                                    || (integer)k
                                                                        == argumentum_causae)
                                                                {
                    perge;
                                                                }

                plagula = argv[k];
                si (plagula[ZEPHYRUM] == '/')
                {
                    strcpy(via, plagula);
                }
                alioquin
                {
                    sprintf(via, "%s/%s", radix, plagula);
                }
            }
            alioquin
            {
                si (k == ZEPHYRUM)
                {
                    perge;
                }
                plagula = venditae[k - I];
                sprintf(via, "%s/%s", radix, plagula);
            }
            p = piscina_generare_dynamicum("oraculum_plagula",
                268435456);
            si (!_plagulam_legere(p, via, &fons))
            {
                fprintf(stderr, "oraculum: plagula absens: %s\n", via);
                redde II;
            }
            sententiae = oratio_conllu_legere(p, fons, &vitium);
            si (sententiae == NIHIL)
            {
                fprintf(stderr, "oraculum: %s: linea %d: %s\n", plagula,
                    (integer)vitium.linea,
                    vitium.causa ? vitium.causa : "?");
                redde II;
            }
                        /* T20a: census aureus seminum - sine parsura, ante iudicium */
            si (semina)
            {
                si (!machina)
                {
                    imprimere("--- %s: semina aurea ---\n", plagula);
                }
                _semina_imprimere(p, sententiae, machina, plagula);
            }
            /* T17: cum programmate primarium ante (0 regulae) et post
             * (omnes);
             * cum -regulae tabula cumulativa post quamque regulam */
            si (programma != NIHIL)
            {
                i32 summa = xar_numerus(programma->regulae);
                i32 r;

                per (r = ZEPHYRUM; r < summa; r++)
                {
                                        si (   (!regulae
                                            && r != ZEPHYRUM)
                                            || (regulae
                                                && r < regulae_ab))
                                        {
                        perge;
                                        }

                    oratio_oraculum_census_vacare(&census);
                    si (!oratio_oraculum_iudicare_resolutum(p,
                        &vocabularia,
                            programma, (s32)r, sententiae, &census))
                    {
                        fprintf(stderr,
                            "oraculum: iudicium fractum: %s\n",
                            plagula);
                        redde II;
                    }
                    imprimere("%s  regulae %d%s: primaria %.1f%%"
                        "  tecta %.1f%%\n",
                        machina ? "#" : "",
                        (integer)r, r == ZEPHYRUM ? " (crudus)" : "",
                        census.verba > ZEPHYRUM ? 100.0
                        * (duplex)census.primaria / (duplex)census.verba
                        : 0.0,
                        census.verba > ZEPHYRUM ? 100.0
                        * (duplex)census.tecta
                            / (duplex)census.verba : 0.0);
                }
                imprimere("%s  regulae %d (omnes):\n",
                    machina ? "#" : "",
                    (integer)summa);
            }
                        oratio_oraculum_census_vacare(&census);
            census.ostendendae       = sententiae_ostendendae;
            census.causa_ostendenda  = causa_ostendenda;
            ante                     = clock();
            si (!oratio_oraculum_iudicare_resolutum(p, &vocabularia,
                    programma, (s32)-I, sententiae, &census))
            {
                fprintf(stderr, "oraculum: iudicium fractum: %s\n",
                    plagula);
                redde II;
            }
            si (machina)
            {
                                _machinam_imprimere(p, &census, plagula,
                                    discrepantiae,
                                    errata, auctor_petitus);
            }
            alioquin
            {
                                                                                                _tabulam_imprimere(p,
                                                                                                    &census,
                                                                                                    plagula,
                                                                                                    exempla,
                                                                                                    discrepantiae,
                                                                                                    errata,
                                                                                                    auctor_petitus,
                                                                                                    clausulae);
                _sententias_imprimere(&census);
                imprimere("  %.0f ms\n\n", 1000.0 * (duplex)(clock()
                    - ante)
                    / (duplex)CLOCKS_PER_SEC);
            }
            plagulae = plagulae + I;
            piscina_destruere(p);
        }
    }
    (vacuum)plagulae;
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
