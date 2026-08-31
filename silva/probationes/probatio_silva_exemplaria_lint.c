/* probatio_silva_exemplaria_lint.c - PORTA L1: lint declarativum
 *
 * Spec exemplarium par. 7.2: documentum lint (fixa/exemplaria/
 * conditio_nihilum.stml - TRANSPARENTIA + catena duorum graduum +
 * relatum per PER) contra plagulam quamque corporis:
 *
 *   parsare -> scribere <parsura> -> [textus arboris + textus lint]
 *   -> stml_legere -> stml_expandere (UNA vocatio - lint totum)
 *   -> <relatum> numerare -> contra ORACULUM conferre
 *
 * ORACULUM: ambulatio silvae INDEPENDENS - lexemata identificatoris
 * valore 'NULL' intra locos 'conditio', identitate primi usus
 * numerata (lexema communicatum in documento SEMEL scribitur -
 * definitio; transclusiones latus lint praeterit - ergo oraculum
 * lexema ad PRIMAM apparitionem ordinis ambulationis solum
 * numerat). Duo modi computandi independentes; aequalitas per
 * plagulam = porta differentialis (exemplar probationis Q3,
 * mensura B - nunc latus unum documentum declarativum est).
 *
 * PORTA APPARATUS (spec 6.5) INCLUSA, causa acuta: sine clausura
 * NIHIL numquam in NULL expanditur - lint 0, oraculum 0, porta
 * FALSO viridis utroque latere. Numerus confidenter falsus peior
 * est quam nullus.
 *
 * INVARIATUM (spec par. 0/par. 10): nullum EXEMPLAR/PER/
 * TRANSPARENTIA in arbore expansa - hic numeratum, non praesumptum.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "stml.h"
#include "stml_macros.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_parsare.h"
#include "silva_expandere.h"
#include "silva_arbor.h"
#include "apparatus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define DIVERGENTIAE_MAX 8

/* Par loci conditionis: genus + index localis */
nomen structura {
    s32 genus;
    i32 locus;
} ConditioPar;

nomen structura {
          i32 plagulae;
          i32 fracturae;
          i32 divergentes;
          i32 summa_lint;
          i32 summa_oraculi;
          i32 invariata_violata;
          i32 apparatus_fracti;
          i32 numerus_divergentiarum;
    character divergentiae[DIVERGENTIAE_MAX][160];
} LintCensus;

hic_manens SilvaLexemaGenus g_lex_identificator;


/* ==================================================
 * Oraculum: ambulatio silvae
 * ================================================== */

/* Loci 'conditio' omnium generum semel collecti */
interior vacuum
_conditio_pares_colligere (
    Piscina* piscina,
        Xar* pares)
{
    constans SilvaRegistrumCoctum* reg = &SILVA_C89_REGISTRUM;
                              s32  g;

    (vacuum)piscina;
    per (g = ZEPHYRUM; g < (s32)reg->numerus_generum; g++)
    {
        i32 i;

        per (i = ZEPHYRUM; i < reg->genera[g].loci_numerus; i++)
        {
            constans SilvaTabLocus* locus =
                &reg->loci[reg->genera[g].loci_offset + i];

            si (   locus->titulus                     != NIHIL
                && strcmp(locus->titulus, "conditio") == ZEPHYRUM)
            {
                ConditioPar* par = (ConditioPar*)xar_addere(pares);

                si (par != NIHIL)
                {
                    par->genus = g;
                    par->locus = i;
                }
            }
        }
    }
}

interior b32
_est_locus_conditionis (
             Xar* pares,
             s32  genus,
             i32  locus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(pares); i++)
    {
        constans ConditioPar* par =
            (constans ConditioPar*)xar_obtinere(pares, i);

        si (   par        != NIHIL && par->genus == genus
            && par->locus == locus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Lexema iam visum? (clavis = octeti monstratoris ipsius) */
interior b32
_lexema_notare (
            Piscina* piscina,
     TabulaDispersa* visa,
    constans vacuum* monstrator)
{
    chorda  clavis;
        i8* octeti;

    octeti = (i8*)piscina_allocare(piscina, magnitudo(monstrator));
    si (octeti == NIHIL)
    {
        redde VERUM;  /* memoria - noli bis numerare */
    }
    memcpy(octeti, &monstrator, magnitudo(monstrator));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(monstrator);
    si (tabula_dispersa_continet(visa, clavis))
    {
        redde VERUM;
    }
    (vacuum)tabula_dispersa_inserere(visa, clavis, (vacuum*)VERUM);
    redde FALSUM;  /* nunc primum visum */
}

/* Ambulatio ordine arboris: lexema quodque ad PRIMAM apparitionem
 * solum iudicatur (identitas documenti - vide caput); identificator
 * 'NULL' pro QUOQUE scopo conditionis includenti numeratur
 * (+= altitudo): latus lint ordines per elementum conditionis fert,
 * et NULL intra conditionem ternarii intra conditionem si in AMBOBUS
 * ordinibus est - mensuratum lib/stml.c (unus situs, alt=2). */
interior vacuum
_oraculum_ambulare (
           Piscina* piscina,
        SilvaValor  valor,
    TabulaDispersa* visa,
               Xar* pares,
               i32  altitudo,
               i32* numerus)
{
    si (valor.genus == SILVA_VALOR_TOKEN && valor.datum.token != NIHIL)
    {
        constans SilvaToken* tok = valor.datum.token;

        si (_lexema_notare(piscina, visa, tok))
        {
            /* indago: NULL suppressum intra conditionem - situs
             * divergentiae candidatus (LINT_INDAGO=1) */
            si (   getenv("LINT_INDAGO") != NIHIL
                && altitudo > ZEPHYRUM
                && tok->genus            == g_lex_identificator
                && chorda_aequalis_literis(tok->valor, "NULL"))
            {
                imprimere("    SUPPRESSUM: b=%d linea=%d\n",
                          (integer)tok->byte_offset,
                          (integer)tok->linea);
            }
            redde;  /* iam visum - documentum transclusionem fert */
        }
        si (   altitudo > ZEPHYRUM
            && tok->genus == g_lex_identificator
            && chorda_aequalis_literis(tok->valor, "NULL"))
        {
            si (getenv("LINT_INDAGO") != NIHIL && altitudo > I)
            {
                imprimere("    NIDIFICATUM (alt=%d): b=%d linea=%d"
                          " f=%d\n",
                          (integer)altitudo,
                          (integer)tok->byte_offset,
                          (integer)tok->linea,
                          (integer)tok->fons_index);
            }
            (*numerus) += altitudo;
        }
        redde;
    }
    si (valor.genus == SILVA_VALOR_NODUS && valor.datum.nodus != NIHIL)
    {
        constans SilvaNodus* n = valor.datum.nodus;
                        i32  i;

        per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
        {
            i32 altitudo_loci = altitudo;

            si (_est_locus_conditionis(pares, n->genus, i))
            {
                altitudo_loci = altitudo + I;
                si (   getenv("LINT_INDAGO") != NIHIL
                    && altitudo_loci > I)
                {
                    imprimere("    SCOPUS NIDIFICATUS: genus %s\n",
                        SILVA_C89_REGISTRUM.genera[n->genus].titulus);
                }
            }
            _oraculum_ambulare(piscina, n->loci[i], visa, pares,
                               altitudo_loci, numerus);
        }
        redde;
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;
        i32 num = silva_valor_lista_numerus(valor);

        per (i = ZEPHYRUM; i < num; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(valor, i);

            si (v != NIHIL)
            {
                _oraculum_ambulare(piscina, *v, visa, pares,
                                   altitudo, numerus);
            }
        }
    }
}


/* ==================================================
 * Latus lint: relatum numerare + invariatum
 * ================================================== */

interior StmlNodus*
_elementum_invenire (
             StmlNodus* n,
    constans character* titulus)
{
    i32 i;
    i32 num;

    si (n == NIHIL)
    {
        redde NIHIL;
    }
    si (   n->genus   == STML_NODUS_ELEMENTUM
        && n->titulus != NIHIL
        && chorda_aequalis_literis(*n->titulus, titulus))
    {
        redde n;
    }
    num = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* inventum = _elementum_invenire(
            stml_liberum_ad_indicem(n, i), titulus);

        si (inventum != NIHIL)
        {
            redde inventum;
        }
    }
    redde NIHIL;
}

interior i32
_liberos_titulo_numerare (
             StmlNodus* n,
    constans character* titulus)
{
    i32 i;
    i32 num;
    i32 numerus = ZEPHYRUM;

    num = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(n, i);

        si (   l          != NIHIL && l->genus == STML_NODUS_ELEMENTUM
            && l->titulus != NIHIL
            && chorda_aequalis_literis(*l->titulus, titulus))
        {
            numerus++;
        }
    }
    redde numerus;
}

/* Invariatum: EXEMPLAR/PER/TRANSPARENTIA in arbore expansa = 0 */
interior i32
_invariata_numerare (
    StmlNodus* n)
{
    i32 i;
    i32 num;
    i32 numerus = ZEPHYRUM;

    si (n == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (   n->genus   == STML_NODUS_ELEMENTUM
        && n->titulus != NIHIL
        && (   chorda_aequalis_literis(*n->titulus, "EXEMPLAR")
            || chorda_aequalis_literis(*n->titulus, "PER")
            || chorda_aequalis_literis(*n->titulus,
                   "TRANSPARENTIA")))
    {
        numerus++;
    }
    num = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < num; i++)
    {
        numerus += _invariata_numerare(
            stml_liberum_ad_indicem(n, i));
    }
    redde numerus;
}


/* ==================================================
 * Plagula una
 * ================================================== */

interior vacuum
_divergentiam_notare (
             LintCensus* census,
     constans character* via,
                    i32  n_lint,
                    i32  n_oraculi)
{
    si (census->numerus_divergentiarum < DIVERGENTIAE_MAX)
    {
        sprintf(census->divergentiae[census->numerus_divergentiarum],
            "%.110s: lint=%d oraculum=%d", via, (integer)n_lint,
            (integer)n_oraculi);
    }
    census->numerus_divergentiarum++;
}

interior vacuum
_plagulam_probare (
     constans character* via,
     constans character* radix,
      constans Clausura* clausura,
                    b32  praebere,
        constans chorda* lint_textus,
                    Xar* conditio_pares,
             LintCensus* census)
{
                 Piscina* opus;
                      i8* fons;
                     i32  mensura;
                     s32  fons_latina;
            SilvaParsura* origo;
     SilvaArborScriptura  scriptura;
            StmlResultus  lectio;
    StmlExpansioResultus  expansio;
     InternamentumChorda* intern;
                  chorda  textus_iunctus;
                      i8* buffer;
                     i32  n_lint;
                     i32  n_oraculi;
               StmlNodus* relatum;

    fons_latina  = -I;
    opus         = piscina_generare_dynamicum("porta_lint", 8388608);
    si (opus == NIHIL)
    {
        redde;
    }
    fons = apparatus_plagulam_legere(opus, via, &mensura);
    si (fons == NIHIL || mensura <= ZEPHYRUM)
    {
        piscina_destruere(opus);
        redde;
    }
    census->plagulae++;

    origo = apparatus_parsare(opus, via, fons, mensura, radix,
                              clausura, praebere, &fons_latina);
    si (origo == NIHIL || origo->commissio == NIHIL)
    {
        census->fracturae++;
        piscina_destruere(opus);
        redde;
    }
    {
        i32 expansa;

        si (!apparatus_sanus(origo, clausura, fons_latina, &expansa))
        {
            census->apparatus_fracti++;
            piscina_destruere(opus);
            redde;
        }
    }

    scriptura = silva_arbor_scribere_parsuram(opus, origo,
        &SILVA_C89_REGISTRUM, "c89", origo->fons_princeps, NIHIL);
    si (!scriptura.successus)
    {
        census->fracturae++;
        piscina_destruere(opus);
        redde;
    }

    /* documentum corporis + documentum lint = textus UNUS (lint
     * INFRA contentum - lex stratorum: exemplaria supra vident) */
    buffer = (i8*)piscina_allocare(opus,
        (memoriae_index)(scriptura.textus.mensura
                         + lint_textus->mensura));
    si (buffer == NIHIL)
    {
        census->fracturae++;
        piscina_destruere(opus);
        redde;
    }
    memcpy(buffer, scriptura.textus.datum,
           (memoriae_index)scriptura.textus.mensura);
    memcpy(buffer + scriptura.textus.mensura, lint_textus->datum,
           (memoriae_index)lint_textus->mensura);
    textus_iunctus.datum    = buffer;
    textus_iunctus.mensura  = scriptura.textus.mensura
                            + lint_textus->mensura;

    intern = internamentum_creare(opus);
    si (intern == NIHIL)
    {
        census->fracturae++;
        piscina_destruere(opus);
        redde;
    }
    lectio = stml_legere(textus_iunctus, opus, intern);
    si (!lectio.successus)
    {
        census->fracturae++;
        imprimere("    LECTIO FRACTA: %s (linea %d)\n", via,
                  (integer)lectio.linea_erroris);
        piscina_destruere(opus);
        redde;
    }
    expansio = stml_expandere(lectio.radix, opus, intern);
    si (!expansio.successus)
    {
        census->fracturae++;
        imprimere("    EXPANSIO FRACTA: %s (vitium %d, linea %d)\n",
                  via, (integer)expansio.vitium,
                  (integer)expansio.linea);
        piscina_destruere(opus);
        redde;
    }

    census->invariata_violata +=
        _invariata_numerare(expansio.radix_expansa);

    relatum = _elementum_invenire(expansio.radix_expansa, "relatum");
    si (relatum == NIHIL)
    {
        census->fracturae++;
        imprimere("    RELATUM DEEST: %s\n", via);
        piscina_destruere(opus);
        redde;
    }
    n_lint = _liberos_titulo_numerare(relatum, "situs");

    /* oraculum independens */
    n_oraculi = ZEPHYRUM;
    {
        TabulaDispersa* visa =
            tabula_dispersa_creare_chorda(opus, 4096);

        si (visa == NIHIL)
        {
            census->fracturae++;
            piscina_destruere(opus);
            redde;
        }
        _oraculum_ambulare(opus, origo->commissio->radix, visa,
                           conditio_pares, ZEPHYRUM, &n_oraculi);
    }

    census->summa_lint     += n_lint;
    census->summa_oraculi  += n_oraculi;
    si (n_lint != n_oraculi)
    {
        census->divergentes++;
        _divergentiam_notare(census, via, n_lint, n_oraculi);
    }
    si (getenv("LINT_VERBOSA") != NIHIL)
    {
        imprimere("  %-40s lint %3d | oraculum %3d\n", via,
                  (integer)n_lint, (integer)n_oraculi);
    }
    piscina_destruere(opus);
}

interior vacuum
_censum_referre (
     constans character* titulus,
    constans LintCensus* c)
{
    i32 i;
    i32 monstranda;

    imprimere("  [%s] plagulae %d | fracturae %d | apparatus fracti"
        " %d | situs lint %d / oraculi %d | divergentes %d |"
        " invariata violata %d\n",
        titulus, (integer)c->plagulae, (integer)c->fracturae,
        (integer)c->apparatus_fracti, (integer)c->summa_lint,
        (integer)c->summa_oraculi, (integer)c->divergentes,
        (integer)c->invariata_violata);
    monstranda = c->numerus_divergentiarum;
    si (monstranda > DIVERGENTIAE_MAX)
    {
        monstranda = DIVERGENTIAE_MAX;
    }
    per (i = ZEPHYRUM; i < monstranda; i++)
    {
        imprimere("    DIVERGENTIA %s\n", c->divergentiae[i]);
    }
}

s32
principale (vacuum)
{
                 Piscina* piscina;
      constans character* radix;
               character  via_corporis[512];
               character  via_plagulae[1024];
               character  via_relativa[1024];
                    DIR* corpus;
       structura dirent* introitus;
                Clausura clausura;
              LintCensus census_planum;
              LintCensus census_latinum;
                     i8* lint_datum;
                     i32 lint_mensura;
                  chorda lint_textus;
                    Xar* conditio_pares;
                     b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_lint", 4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = "..";
    }

    g_lex_identificator = silva_arbor_lexema_ex_tag(
        "lex-identificator", (i32)strlen("lex-identificator"));
    si (g_lex_identificator == SILVA_LEX_NUMERUS_GENERUM)
    {
        imprimere("FRACTA: genus lexematis ignotum\n");
        piscina_destruere(piscina);
        redde I;
    }

    /* documentum lint - ARTEFACTUM, non codex (spec par. 7.2) */
    sprintf(via_plagulae,
        "%s/silva/probationes/fixa/exemplaria/conditio_nihilum.stml",
        radix);
    lint_datum = apparatus_plagulam_legere(piscina, via_plagulae,
                                           &lint_mensura);
    si (lint_datum == NIHIL || lint_mensura <= ZEPHYRUM)
    {
        imprimere("FRACTA: documentum lint non lectum: %s\n",
                  via_plagulae);
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }
    lint_textus.datum    = lint_datum;
    lint_textus.mensura  = lint_mensura;

    conditio_pares = xar_creare(piscina, magnitudo(ConditioPar));
    si (conditio_pares == NIHIL)
    {
        piscina_destruere(piscina);
        redde I;
    }
    _conditio_pares_colligere(piscina, conditio_pares);
    imprimere("\n--- PORTA L1: lint conditio-contra-nihilum ---\n");
    imprimere("  loci conditionum in registro: %d\n",
              (integer)xar_numerus(conditio_pares));

    /* gradus planum */
    memset(&census_planum, 0, magnitudo(census_planum));
    sprintf(via_corporis, "%s/probationes/fixa/roundtrip", radix);
    corpus = opendir(via_corporis);
    si (corpus == NIHIL)
    {
        imprimere("FRACTA: corpus non apertum: %s\n", via_corporis);
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }
    dum ((introitus = readdir(corpus)) != NIHIL)
    {
        si (!apparatus_est_c_vel_h(introitus->d_name))
        {
            perge;
        }
        sprintf(via_plagulae, "%s/%s", via_corporis,
                introitus->d_name);
        _plagulam_probare(via_plagulae, radix, NIHIL, FALSUM,
                          &lint_textus, conditio_pares,
                          &census_planum);
    }
    closedir(corpus);
    _censum_referre("planum", &census_planum);

    /* gradus latinum */
    memset(&census_latinum, 0, magnitudo(census_latinum));
    sprintf(via_corporis, "%s/lib", radix);
    corpus = opendir(via_corporis);
    si (corpus == NIHIL)
    {
        imprimere("FRACTA: corpus latinum non apertum\n");
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }
    dum ((introitus = readdir(corpus)) != NIHIL)
    {
        si (!apparatus_est_c(introitus->d_name))
        {
            perge;
        }
        sprintf(via_relativa, "lib/%s", introitus->d_name);
        sprintf(via_plagulae, "%s/%s", radix, via_relativa);
        apparatus_clausuram_petere(radix, via_relativa, &clausura);
        _plagulam_probare(via_plagulae, radix, &clausura, VERUM,
                          &lint_textus, conditio_pares,
                          &census_latinum);
    }
    closedir(corpus);
    _censum_referre("latinum", &census_latinum);


    /* ==================================================
     * PORTA: aequalitas per plagulam + tegumentum
     * ================================================== */

    CREDO_AEQUALIS_I32 (census_planum.plagulae, 78);
    CREDO_AEQUALIS_I32 (census_planum.fracturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planum.divergentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_planum.invariata_violata, ZEPHYRUM);

    CREDO_AEQUALIS_I32 (census_latinum.plagulae, 155);
    CREDO_AEQUALIS_I32 (census_latinum.fracturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinum.apparatus_fracti, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinum.divergentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_latinum.invariata_violata, ZEPHYRUM);

    /* CUSTOS TEGUMENTI: 'N/N purae' sine apparitione nihil probat -
     * praesentia asserenda (latinum: NIHIL ubique in lib) */
    CREDO_VERUM (census_latinum.summa_lint > ZEPHYRUM);
    CREDO_VERUM (census_latinum.summa_oraculi > ZEPHYRUM);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
