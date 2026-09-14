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
 *   -lites     cum -machina (T32 e): ordines LIS, contentio singula cum
 *              notis - victor victa dependens caput ante casus-aureus
 *              casus-victae casus-victoris deprel caput-aureum-idem
 *              caput-victoris-idem clausula primum nominativi
 *              nominativi-certi nominativi-concordes accusativi-certi
 *              genus-victae numerus-capitis persona-capitis vox-capitis
 *              numerus-victae numerus-victoris (T32 f)
 *   -nota T    cum -errata (T30): errata accidentis T (numerus genus
 *              persona modus vox forma-verbi tempus | omnes) - verba
 *              classis rectae cuius lectio prima valorem falsum fert:
 *              forma, classis, valor aureus, noster, decisio/auctor,
 *              socius ligationis, attingibile (lectio classis valorem
 *              aureum fert) | absens, numerus; -machina ordines
 *              'plagula ERRATUM-NOTA accidens classis aurea nostra
 *              decisio auctor forma socius distantia attingibile numerus';
 *              ordo NOTA quartum 'conventione' fert (recti per alterum
 *              solum: vox deponens, genus commune)
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
    imprimere("    catena (T20b): discordiae %d  catenatae %d\n",
        (integer)c->catena_discordiae, (integer)c->catena_catenatae);
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
                              s32  nota_petita,
                              b32  clausulae)
{
    i32 i;


    imprimere("--- %s: sententiae %d (fractae %d, proiectiones gradus"
        " fractae %d)  verba %d  rangae %d ---\n",
        titulus, (integer)c->sententiae, (integer)c->sententiae_fractae,
        (integer)c->resolutiones_fractae,
        (integer)c->verba, (integer)c->rangae);
    imprimere("  ordines ex umbra iam scripta recusati %d (T34)\n",
        (integer)c->ordines_scripti_recusati);
                imprimere("  TECTA %.1f%%  primaria %.1f%%  lemmata %.1f%%  ignota %.1f%%  inalignata %d\n",
                    _pars(c->tecta, c->verba), _pars(c->primaria,
                    c->verba),
                    _pars(c->lemmata, c->verba), _pars(c->ignota,
                    c->verba),
                    (integer)c->inalignata);
        /* T23: casus - classis recta, aurum casum fert, lectio prima
         * Latina casum fert */
                imprimere("  CASUS %.1f%% (%d de %d: classis recta, casus aureus,"
                    " lectio Latina)\n", _pars(c->casus_recti,
                    c->casus_verba),
                    (integer)c->casus_recti, (integer)c->casus_verba);
        /* T29: accidentia praeter casum, eadem condicio */
        {
            i32 k;

            per (k = ZEPHYRUM; k < ORATIO_ORACULUM_NOTAE; k++)
            {
                si (c->notae_verba[k] > ZEPHYRUM)
                {
                    imprimere("  NOTA %-12s %.1f%% (%d de %d; conventione %d)\n",
                        oratio_oraculum_nota_titulus(k),
                        _pars(c->notae_recti[k], c->notae_verba[k]),
                        (integer)c->notae_recti[k],
                        (integer)c->notae_verba[k],
                        (integer)c->notae_conventione[k]);
                }
            }
        }
        /* T26: ligationes contra capita aurea */
        imprimere("  LIGATIO praecisio %.1f%% (%d de %d)  revocatio %.1f%%"
            " (arcus aurei %d)  capitis %.1f%% (%d)  vicinae %.1f%% (%d)\n",
            _pars(c->ligationes_rectae, c->ligationes_nostrae),
            (integer)c->ligationes_rectae,
            (integer)c->ligationes_nostrae,
            _pars(c->ligationes_rectae, c->arcus_aurei),
            (integer)c->arcus_aurei,
            _pars(c->ligationes_capitis_rectae, c->ligationes_capitis),
            (integer)c->ligationes_capitis,
            _pars(c->ligationes_vicinae_rectae, c->ligationes_vicinae),
            (integer)c->ligationes_vicinae);
        /* T31 a: per relationem */
        {
            i32 r;

            per (r = ZEPHYRUM; r < (i32)ORATIO_RELATIO_NUMERUS; r++)
            {
                si (c->ligationes_nostrae_relationum[r] > ZEPHYRUM)
                {
                    imprimere("    %-16s praecisio %.1f%% (%d de %d)%s\n",
                        ORATIO_TITULI_RELATIONUM[r],
                        _pars(c->ligationes_rectae_relationum[r],
                            c->ligationes_nostrae_relationum[r]),
                        (integer)c->ligationes_rectae_relationum[r],
                        (integer)c->ligationes_nostrae_relationum[r],
                        r == (i32)ORATIO_RELATIO_SUBIECTUM ? "" : "");
                }
            }
            imprimere("    subiectum revocatio %.1f%% (arcus aurei subiecti %d)\n",
                _pars(c->ligationes_rectae_relationum[ORATIO_RELATIO_SUBIECTUM],
                    c->arcus_aurei_subiecti),
                (integer)c->arcus_aurei_subiecti);
            imprimere("    obiectum-verbi revocatio %.1f%% (arcus aurei obiecti %d)\n",
                _pars(c->ligationes_rectae_relationum[
                    ORATIO_RELATIO_OBIECTUM_VERBI],
                    c->arcus_aurei_obiecti),
                (integer)c->arcus_aurei_obiecti);
        }
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
    /* T22 b: dialectus documenti (thesaurus totus) */
    imprimere("  dialectus documenti: %s (recuperata %d de %d)\n",
        ORATIO_TITULI_DIALECTORUM[(i32)c->dialectus],
        (integer)c->dialectus_recuperata, (integer)c->dialectus_verba);
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
    /* T32 b: tectum arcuum et contentiones */
    si (c->ligationes_petitae > ZEPHYRUM)
    {
        Xar* contentiones = oratio_oraculum_contentiones(piscina, c);
        i32  j;

        imprimere("  arcus tecti %d de petitis %d = %5.1f%% (recti %d = %5.1f%%;"
            " alternae %d, rectae %d)\n",
            (integer)c->ligationes_tectae,
            (integer)c->ligationes_petitae,
            _pars(c->ligationes_tectae, c->ligationes_petitae),
            (integer)c->ligationes_rectae,
            _pars(c->ligationes_rectae, c->ligationes_petitae),
            (integer)c->alternae_numerus, (integer)c->alternae_rectae);
        per (j = ZEPHYRUM; contentiones != NIHIL
            && j < xar_numerus(contentiones) && j < (i32)XII; j++)
        {
            constans OratioOraculumContentio* ct =
                *(OratioOraculumContentio**)xar_obtinere(contentiones,
                j);

            imprimere("    contentio %-40.*s > %-40.*s %5d  victa recta %5.1f%%"
                "  casu iudicatae %d victa sola %d victor solus %d\n",
                (integer)ct->victor.mensura,
                (constans character*)ct->victor.datum,
                (integer)ct->victa.mensura,
                (constans character*)ct->victa.datum,
                (integer)ct->numerus,
                _pars(ct->victae_rectae, ct->numerus),
                (integer)ct->casus_iudicati,
                (integer)ct->victa_casu_sola,
                (integer)ct->victor_casu_solus);
        }
    }
    /* T32 a: dependentes contesti (caput unum per verbum violatum) */
    si (c->dependentes_contesti > ZEPHYRUM)
    {
        imprimere("  dependentes contesti %d (petitiones %d, rectae %d)\n",
            (integer)c->dependentes_contesti,
            (integer)c->petitiones_contestae,
            (integer)c->petitiones_contestae_rectae);
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
                                                                            "  vicina %5d %5.1f%%  remota %5d %5.1f%%"
                                                                            "  casus %5d %5.1f%%  arcus %5d %5.1f%%\n",
                                                                            (integer)a->titulus.mensura,
                                                                            (constans character*)a->titulus.datum,
                                                                            (integer)a->verba,
                                                                            _pars(a->primaria,
                                                                            a->verba),
                                                                            (integer)a->vicina,
                                                                            _pars(a->vicina_primaria,
                                                                            a->vicina),
                                                                            (integer)a->remota,
                                                                            _pars(a->remota_primaria,
                                                                            a->remota),
                                                                            (integer)a->casus_verba,
                                                                            _pars(a->casus_recti,
                                                                            a->casus_verba),
                                                                            (integer)a->ligationes,
                                                                            _pars(a->ligationes_rectae,
                                                                            a->ligationes));
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
    /* T30: errata notarum - accidens petitum (aut omnia), frequentissima
     * prima, ERRATA_IMPRESSA quaeque */
    si (errata && nota_petita >= ZEPHYRUM)
    {
        i32 k;

        per (k = ZEPHYRUM; k < ORATIO_ORACULUM_NOTAE; k++)
        {
            Xar* es;
            i32  m;
            i32  aliae = ZEPHYRUM;

            si (   nota_petita < (s32)ORATIO_ORACULUM_NOTAE
                && (s32)k != nota_petita)
            {
                perge;
            }
            es = oratio_oraculum_errata_notarum(piscina, c, (s32)k);
            si (es == NIHIL || xar_numerus(es) == ZEPHYRUM)
            {
                perge;
            }
            imprimere("  errata notae %s (%d verba, %d falsa):\n",
                oratio_oraculum_nota_titulus(k),
                (integer)c->notae_verba[k],
                (integer)(c->notae_verba[k] - c->notae_recti[k]));
            per (m = ZEPHYRUM; m < xar_numerus(es); m++)
            {
                constans OratioOraculumErratumNotae* d =
                    *(OratioOraculumErratumNotae**)xar_obtinere(es, m);
                constans character* aurea =
                    oratio_oraculum_nota_valor_titulus(k, d->aurea);
                constans character* nostra =
                    oratio_oraculum_nota_valor_titulus(k, d->nostra);

                si (m >= (i32)ERRATA_IMPRESSA)
                {
                    aliae = aliae + d->numerus;
                    perge;
                }
                imprimere("      %-16.*s %-12s aurea %-11s nostra %-11s"
                    " %-9s %-30.*s socius %-14.*s @%-3d %-11s %5d\n",
                    (integer)d->forma.mensura,
                    (constans character*)d->forma.datum,
                    (i32)d->classis
                        < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                        ? oratio_classis_titulus(d->classis) : "?",
                    aurea != NIHIL ? aurea : "?",
                    nostra != NIHIL ? nostra : "?",
                    d->decisio >= ZEPHYRUM
                        && d->decisio < (s32)ORATIO_DECISIO_NUMERUS
                        ? ORATIO_TITULI_DECISIONUM[d->decisio] : "nemo",
                    (integer)d->auctor.mensura,
                    d->auctor.mensura > ZEPHYRUM
                        ? (constans character*)d->auctor.datum : "",
                    (integer)d->socius.mensura,
                    d->socius.mensura > ZEPHYRUM
                        ? (constans character*)d->socius.datum : "",
                    (integer)d->distantia,
                    d->attingibile ? "attingibile" : "absens",
                    (integer)d->numerus);
            }
            si (aliae > ZEPHYRUM)
            {
                imprimere("      ... aliae %d\n", (integer)aliae);
            }
        }
    }
    /* T20a: clausulae */
    si (clausulae)
    {
        _clausulas_imprimere(piscina, c, errata);
    }
    imprimere("  %-24s %6s %7s %8s %8s %7s %14s\n", "classis aurea",
        "verba", "tecta", "primaria", "lemmata", "ignota", "casus (n)");
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
        imprimere("  %-24s %6d %6.1f%% %7.1f%% %7.1f%% %6.1f%% %6.1f%% (%d)\n",
            t, (integer)k->verba, _pars(k->tecta, k->verba),
            _pars(k->primaria, k->verba), _pars(k->lemmata, k->verba),
            _pars(k->ignota, k->verba),
            _pars(k->casus_recti, k->casus_verba),
            (integer)k->casus_verba);
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

/* T32 e: titulus valoris enumerati aut '-' (absens) */
interior constans character*
_titulus_valoris (
    constans character* constans* tituli,
                              s32  valor,
                              i32  numerus)
{
    redde valor >= ZEPHYRUM
        && valor < (s32)numerus ? tituli[valor] : "-";
}

interior vacuum
_machinam_imprimere (
                          Piscina* piscina,
    constans OratioOraculumCensus* c,
               constans character* titulus,
                              b32  discrepantiae,
                              b32  errata,
                           chorda  auctor_petitus,
                              s32  nota_petita,
                              b32  lites)
{
    i32 i;


    per (i = ZEPHYRUM; i <= (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
    {
        constans OratioOraculumClassis* k = &c->classes[i];

        si (k->verba == ZEPHYRUM)
        {
            perge;
        }
        imprimere("%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            titulus,
            i < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                ? oratio_classis_titulus((OratioClassis)i) : "extra",
            (integer)k->verba, (integer)k->tecta, (integer)k->primaria,
            (integer)k->lemmata, (integer)k->ignota,
            (integer)k->inalignata, (integer)k->casus_verba,
            (integer)k->casus_recti);
    }
        /* T23: columnae casus_verba casus_recti appensae (ordines
         * SUMMA et classium) */
        imprimere("%s\tSUMMA\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            titulus, (integer)c->verba, (integer)c->tecta,
            (integer)c->primaria, (integer)c->lemmata,
            (integer)c->ignota,
            (integer)c->inalignata, (integer)c->casus_verba,
            (integer)c->casus_recti);
        /* 2026-09-11: proiectiones gradus resolutionis fractae (pinna 0) */
        imprimere("%s\tRESOLUTIO-FRACTAE\t%d\n", titulus,
            (integer)c->resolutiones_fractae);
        /* T34: ordines ex umbra gradu priore scripta recusati (relatum) */
        imprimere("%s\tORDINES-SCRIPTI\t%d\n", titulus,
            (integer)c->ordines_scripti_recusati);
        /* T22 b: ordo DIALECTUS titulus recuperata verba */
        imprimere("%s\tDIALECTUS\t%s\t%d\t%d\n", titulus,
            ORATIO_TITULI_DIALECTORUM[(i32)c->dialectus],
            (integer)c->dialectus_recuperata,
            (integer)c->dialectus_verba);
        /* T29: ordines NOTA accidens verba recti */
        {
            i32 k;

            per (k = ZEPHYRUM; k < ORATIO_ORACULUM_NOTAE; k++)
            {
                imprimere("%s\tNOTA\t%s\t%d\t%d\t%d\n", titulus,
                    oratio_oraculum_nota_titulus(k),
                    (integer)c->notae_verba[k],
                    (integer)c->notae_recti[k],
                    (integer)c->notae_conventione[k]);
            }
        }
                /* T26: ordo LIGATIO nostrae rectae aurei capitis capitis-rectae
         * vicinae vicinae-rectae */
        imprimere("%s\tLIGATIO\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", titulus,
            (integer)c->ligationes_nostrae,
            (integer)c->ligationes_rectae,
            (integer)c->arcus_aurei, (integer)c->ligationes_capitis,
            (integer)c->ligationes_capitis_rectae,
            (integer)c->ligationes_vicinae,
            (integer)c->ligationes_vicinae_rectae);
        /* T31 a: ordines LIGATIO-RELATIO titulus nostrae rectae aurei (subiecti) */
        {
            i32 r;

            per (r = ZEPHYRUM; r < (i32)ORATIO_RELATIO_NUMERUS; r++)
            {
                imprimere("%s\tLIGATIO-RELATIO\t%s\t%d\t%d\t%d\n",
                    titulus,
                    ORATIO_TITULI_RELATIONUM[r],
                    (integer)c->ligationes_nostrae_relationum[r],
                    (integer)c->ligationes_rectae_relationum[r],
                    (integer)(r == (i32)ORATIO_RELATIO_SUBIECTUM
                        ? c->arcus_aurei_subiecti
                        : r == (i32)ORATIO_RELATIO_OBIECTUM_VERBI
                        ? c->arcus_aurei_obiecti : ZEPHYRUM));
            }
        }
        /* T32 b: ordo TECTUM petitae tectae rectae alternae alternae-rectae;
         * ordines CONTENTIO victor victa numerus victae-rectae victa-sola
         * victor-solus (T32 c) casus-iudicati victa-casu-sola
         * victor-casu-solus (T32 d) */
        imprimere("%s\tTECTUM\t%d\t%d\t%d\t%d\t%d\n", titulus,
            (integer)c->ligationes_petitae,
            (integer)c->ligationes_tectae,
            (integer)c->ligationes_rectae, (integer)c->alternae_numerus,
            (integer)c->alternae_rectae);
        {
            Xar* contentiones = oratio_oraculum_contentiones(piscina,
                c);
            i32 j;

            per (j = ZEPHYRUM; contentiones != NIHIL
                && j < xar_numerus(contentiones); j++)
            {
                constans OratioOraculumContentio* ct =
                    *(OratioOraculumContentio**)xar_obtinere(contentiones,
                    j);

                imprimere("%s\tCONTENTIO\t%.*s\t%.*s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                    titulus,
                    (integer)ct->victor.mensura,
                    (constans character*)ct->victor.datum,
                    (integer)ct->victa.mensura,
                    (constans character*)ct->victa.datum,
                    (integer)ct->numerus, (integer)ct->victae_rectae,
                    (integer)ct->victa_sola, (integer)ct->victor_solus,
                    (integer)ct->casus_iudicati,
                    (integer)ct->victa_casu_sola,
                    (integer)ct->victor_casu_solus);
            }
        }
        /* T32 e: ordines LIS (cum -lites) - contentio singula cum notis */
        per (i = ZEPHYRUM; lites && c->lites != NIHIL
            && i < xar_numerus(c->lites); i++)
        {
            constans OratioOraculumLis* l =
                *(OratioOraculumLis**)xar_obtinere(c->lites, i);

            imprimere("%s\tLIS\t%.*s\t%.*s\t%.*s\t%.*s\t%d\t%s\t%s\t%s"
                "\t%.*s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\n",
                titulus,
                (integer)l->victor.mensura,
                (constans character*)l->victor.datum,
                (integer)l->victa.mensura,
                (constans character*)l->victa.datum,
                (integer)l->dependens.mensura,
                (constans character*)l->dependens.datum,
                (integer)l->caput.mensura,
                (constans character*)l->caput.datum,
                (integer)l->ante,
                _titulus_valoris(ORATIO_TITULI_CASUUM, l->casus_aureus,
                    (i32)ORATIO_CASUS_NUMERUS),
                _titulus_valoris(ORATIO_TITULI_CASUUM, l->casus_victae,
                    (i32)ORATIO_CASUS_NUMERUS),
                _titulus_valoris(ORATIO_TITULI_CASUUM,
                l->casus_victoris,
                    (i32)ORATIO_CASUS_NUMERUS),
                (integer)l->deprel.mensura,
                (constans character*)l->deprel.datum,
                (integer)l->caput_aureum_idem,
                (integer)l->caput_victoris_idem,
                (integer)l->clausula,
                (integer)l->primum_clausulae,
                (integer)l->nominativi,
                (integer)l->nominativi_certi,
                (integer)l->nominativi_concordes,
                (integer)l->accusativi_certi,
                _titulus_valoris(ORATIO_TITULI_GENERUM_GRAMMATICORUM,
                    l->genus_victae,
                    (i32)ORATIO_GENUS_GRAMMATICUM_NUMERUS),
                _titulus_valoris(ORATIO_TITULI_NUMERORUM,
                    l->numerus_capitis,
                    (i32)ORATIO_NUMERUS_GRAMMATICUS_NUMERUS),
                _titulus_valoris(ORATIO_TITULI_PERSONARUM,
                    l->persona_capitis, (i32)ORATIO_PERSONA_NUMERUS),
                _titulus_valoris(ORATIO_TITULI_VOCUM, l->vox_capitis,
                    (i32)ORATIO_VOX_NUMERUS),
                _titulus_valoris(ORATIO_TITULI_NUMERORUM,
                l->numerus_victae,
                    (i32)ORATIO_NUMERUS_GRAMMATICUS_NUMERUS),
                _titulus_valoris(ORATIO_TITULI_NUMERORUM,
                    l->numerus_victoris,
                    (i32)ORATIO_NUMERUS_GRAMMATICUS_NUMERUS));
        }
        /* T32 a: ordo CONTESTA dependentes petitiones rectae */
        imprimere("%s\tCONTESTA\t%d\t%d\t%d\n", titulus,
            (integer)c->dependentes_contesti,
            (integer)c->petitiones_contestae,
            (integer)c->petitiones_contestae_rectae);
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

                                    imprimere("%s\tAUCTOR\t%.*s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                                        titulus,
                                        (integer)a->titulus.mensura,
                                        (constans character*)a->titulus.datum,
                                        (integer)a->verba,
                                        (integer)a->primaria,
                                        (integer)a->vicina,
                                        (integer)a->vicina_primaria,
                                        (integer)a->remota,
                                        (integer)a->remota_primaria,
                                        (integer)a->casus_verba,
                                        (integer)a->casus_recti,
                                        (integer)a->ligationes,
                                        (integer)a->ligationes_rectae,
                                        (integer)a->ligationes_vicinae,
                                        (integer)a->ligationes_vicinae_rectae);
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
            imprimere("%s\tCLAUSULAE\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                titulus,
                (integer)c->clausulae_sententiae,
                (integer)c->clausulae_nostrae,
                (integer)c->clausulae_aureae,
                (integer)c->clausulae_pares,
                (integer)c->clausulae_paria,
                (integer)c->clausulae_paria_concordia,
                (integer)c->catena_discordiae,
                (integer)c->catena_catenatae);
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
    /* T30: ordines ERRATUM-NOTA accidens classis aurea nostra decisio
     * auctor forma socius distantia attingibile numerus */
    si (errata && nota_petita >= ZEPHYRUM)
    {
        Xar* es = oratio_oraculum_errata_notarum(piscina, c,
            nota_petita < (s32)ORATIO_ORACULUM_NOTAE ? nota_petita
            : (s32)-I);
        i32 m;

        per (m = ZEPHYRUM; es != NIHIL && m < xar_numerus(es); m++)
        {
            constans OratioOraculumErratumNotae* d =
                *(OratioOraculumErratumNotae**)xar_obtinere(es, m);
            constans character* aurea =
                oratio_oraculum_nota_valor_titulus(d->nota, d->aurea);
            constans character* nostra =
                oratio_oraculum_nota_valor_titulus(d->nota, d->nostra);

            imprimere("%s\tERRATUM-NOTA\t%s\t%s\t%s\t%s\t%s\t%.*s\t%.*s"
                "\t%.*s\t%d\t%d\t%d\n",
                titulus, oratio_oraculum_nota_titulus(d->nota),
                (i32)d->classis < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM
                    ? oratio_classis_titulus(d->classis) : "?",
                aurea != NIHIL ? aurea : "?",
                nostra != NIHIL ? nostra : "?",
                d->decisio >= ZEPHYRUM
                    && d->decisio < (s32)ORATIO_DECISIO_NUMERUS
                    ? ORATIO_TITULI_DECISIONUM[d->decisio] : "nemo",
                (integer)d->auctor.mensura,
                d->auctor.mensura > ZEPHYRUM
                    ? (constans character*)d->auctor.datum : "",
                (integer)d->forma.mensura,
                (constans character*)d->forma.datum,
                (integer)d->socius.mensura,
                d->socius.mensura > ZEPHYRUM
                    ? (constans character*)d->socius.datum : "",
                (integer)d->distantia,
                (integer)(d->attingibile ? I : ZEPHYRUM),
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
                                    b32 semina  = FALSUM;   /* T20a */
                                    b32 lites   = FALSUM;   /* T32 e */
                                    i32 sententiae_ostendendae =
                                        ZEPHYRUM;   /* T20a quater */
                                    s32 causa_ostendenda = (s32)-I;
                                integer argumentum_sententiarum = -I;
                                integer argumentum_causae = -I;

                                integer argumentum_auctoris  = -I;
                                 chorda auctor_petitus;
                                integer argumentum_notae  = -I;   /* T30 */
                                    s32 nota_petita       = (s32)-I;

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
        alioquin si (strcmp(argv[i], "-lites") == ZEPHYRUM)
        {
            lites = VERUM;
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
            argumentum_auctoris   = i + I;
            auctor_petitus.datum  = (i8*)argv[argumentum_auctoris];
            auctor_petitus.mensura  =
                (i32)strlen(argv[argumentum_auctoris]);
            i                       = argumentum_auctoris;
        }
        alioquin si (   strcmp(argv[i], "-nota") == ZEPHYRUM
                     && i + I < argc)
        {
            /* T30: accidens titulo aut 'omnes' */
            i32 t;

            argumentum_notae = i + I;
            nota_petita = strcmp(argv[argumentum_notae], "omnes")
                == ZEPHYRUM ? (s32)ORATIO_ORACULUM_NOTAE : (s32)-I;
            per (t = ZEPHYRUM; t < ORATIO_ORACULUM_NOTAE; t++)
            {
                si (strcmp(argv[argumentum_notae],
                        oratio_oraculum_nota_titulus(t)) == ZEPHYRUM)
                {
                    nota_petita = (s32)t;
                }
            }
            si (nota_petita < ZEPHYRUM)
            {
                fprintf(stderr, "oraculum: -nota ignota: %s\n",
                    argv[argumentum_notae]);
                redde II;
            }
            i = argumentum_notae;
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
                                                        != argumentum_causae
                                                    && i
                                                        != argumentum_notae)
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
                                                                        == argumentum_causae
                                                                    || (integer)k
                                                                        == argumentum_notae)
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
                                    errata, auctor_petitus,
                                    nota_petita, lites);
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
                                                                                                    nota_petita,
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
