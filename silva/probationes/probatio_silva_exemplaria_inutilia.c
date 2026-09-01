/* probatio_silva_exemplaria_inutilia.c - PORTA CODEX 69: SINE
 *
 * Prima porta ANTIIUNCTIONIS: codex 69 (variabilis inutilis,
 * paritas -Wunused-variable) ut documentum exemplarium SINE ferens
 * (fixa/exemplaria/inutilis_localis.stml). Absentia usus = ordines
 * declarationum quos nulla congruentia folii interioris necat -
 * pons '&@n;' nodum identificatoris capti in formam folii implet
 * (PONS IPSE IUNCTIO EST), subarbor CORPORIS (ordo scopi) petitur.
 *
 * VIA REFINAMENTI (decretum 2026-08-31: divergentia agatur,
 * numquam praesumptione) - cursus primus 865 situs falsos in lib
 * dedit; tria refinamenta machinae eos ad NIHILUM redegerunt:
 * (1) caecitas SYMMETRICA (perspicua in liberis FORMAE quoque
 * cadunt - forma splicata trivia fert); (2) TRANSPARENTIA
 * attributa= (provenientia b/linea/columna/f in nodis captis
 * littera congruentiae non fit); (3) aliasa contenti '<<#id>>'
 * in liberis effectivis resoluta (communicatio lexematum =
 * repraesentatio). GENERATIO TERTIA documenti (2026-09-01):
 * ordines scoporum = DEFINITIO-FUNCTIONIS (in C89 functiones non
 * nidificant - classis ordinum duplicatorum quae versionem
 * corporum-ut-scopi ad mortem memoriae duxit [arbor2_glr: ordines
 * x maiores x subarbores = SIGKILL] structura linguae ipsius
 * dissolvitur), retentio radicis ordinem PER DECLARATIONEM dat
 * (residuum primae-per-scopum NECATUM - formae.c 4/4 exacte),
 * descensus '<**>' formas declaratorum omnes tegit, SINE per
 * _alicubi_congruere (exitus maturus - existentia sola;
 * arbor2_glr 31s -> 8s). Residua nominata: declarator secundus
 * declarationis unius ('int a, b;'), membra structurarum in
 * functione definitarum (declaratio fluitans eas videret),
 * umbratio - in corpore hoc NON occurrunt, divergentes==0
 * pinnatum adventum clamabit.
 *
 * Oraculum = ANALYSATOR IPSE: recipe examinis verbatim (ut porta
 * lint II) - lexicon compositum + capita praebita + oraculum +
 * bis-analysis post recanonicare, diagnostica ad codicem 69
 * cribrata. PARSURA UNA, IUDICES DUO.
 *
 * CORPUS: fixa/examinis (fixturae verdictorum quattuor - situs
 * vivi codicis 69 ibi habitant, census 2026-08-31) + lib
 * (latitudo). Fixturae quae recipe frangunt (C invalidum quod
 * parsare nequit) PRAETERMISSAE numerantur, numquam tacite.
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
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include "silva_lexicon.h"
#include "silva_arbor.h"
#include "apparatus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define DIVERGENTIAE_MAX 12

nomen structura {
          i32 plagulae;
          i32 fracturae;
          i32 recusae;
          i32 excedentes;   /* lint > oraculum (soliditas fracta) */
          i32 divergentes;  /* lint != oraculum (subcopia aperta) */
          i32 summa_lint;
          i32 summa_oraculi;
          i32 invariata_violata;
          i32 numerus_divergentiarum;
    character divergentiae[DIVERGENTIAE_MAX][160];
} LintCensus;

nomen structura {
      SilvaParsura* parsura;
    SilvaSemantica* sem;
               b32  recusa;
} RecipeFructus;


/* ==================================================
 * Recipe examinis (verbatim ex instrumenta/principalia/examen.c -
 * fidelitas oraculi: quod binarium facit, hoc facimus)
 * ================================================== */

interior b32
_praetermittendum (
    constans character* titulus)
{
    redde strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM;
}

interior vacuum
_caput_praebere (
        SilvaContextus* ctx,
               Piscina* piscina,
        TabulaDispersa* visa,
    constans character* via)
{
       chorda  clavis;
           i8* textus;
          i32  mensura;

    clavis = chorda_ex_literis(via, piscina);
    si (tabula_dispersa_continet(visa, clavis))
    {
        redde;
    }
    textus = apparatus_plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        redde;
    }
    si (silva_contextus_praebere(ctx, via,
            (constans character*)textus, mensura))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
    }
}

interior vacuum
_capita_praeparare (
        SilvaContextus* ctx,
               Piscina* piscina,
        TabulaDispersa* visa,
    constans character* via)
{
                 DIR* dir;
    structura dirent* introitus;

    dir = opendir(via);
    si (dir == NIHIL)
    {
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL)
    {
             character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.')
        {
            perge;
        }
        si (_praetermittendum(introitus->d_name))
        {
            perge;
        }
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);
        si (introitus->d_type == DT_DIR)
        {
            _capita_praeparare(ctx, piscina, visa, via_plena);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (   m                         >= III
                && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I]  == 'h')
            {
                _caput_praebere(ctx, piscina, visa, via_plena);
            }
        }
    }
    closedir(dir);
}

/* FALSUM = apparatus fractus (fracturae); recusa = fines tactae */
interior b32
_recipe_examinis (
               Piscina* opus,
    constans character* radix,
    constans character* via,
         RecipeFructus* fructus)
{
     SilvaContextus* ctx;
       SilvaParsura* systema_parsura;
          character* fons_plagulae;
                i32  mensura_plagulae;
          character  via_systematis[1024];
                 i8* fons_iso;
                i32  mensura_iso;
                 i8* fons_px;
                i32  mensura_px;
          character* fons_sys;
                i32  mensura_sys;
                b32  ext_fractum;
      SilvaOraculum* oraculum;
       SilvaParsura* parsura;
     SilvaSemantica* sem;

    fructus->parsura  = NIHIL;
    fructus->sem      = NIHIL;
    fructus->recusa   = FALSUM;

    ctx = silva_contextus_creare(opus);
    si (ctx == NIHIL)
    {
        redde FALSUM;
    }
    fons_plagulae = (character*)apparatus_plagulam_legere(opus, via,
        &mensura_plagulae);
    si (fons_plagulae == NIHIL || mensura_plagulae <= ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* systema (lexicon): ISO + POSIX derivatum + bloci externa */
    sprintf(via_systematis, "%s/silva/fontes/systema_c89.h", radix);
    fons_iso = apparatus_plagulam_legere(opus, via_systematis,
                                         &mensura_iso);
    sprintf(via_systematis, "%s/silva/fontes/systema_posix.h",
            radix);
    fons_px = apparatus_plagulam_legere(opus, via_systematis,
                                        &mensura_px);
    si (fons_iso == NIHIL || fons_px == NIHIL)
    {
        redde FALSUM;
    }
    ext_fractum = FALSUM;
    fons_sys = silva_lexicon_componere(
        (constans character*)fons_iso, mensura_iso,
        (constans character*)fons_px, mensura_px,
        fons_plagulae, mensura_plagulae, FALSUM, opus,
        &mensura_sys, via, &ext_fractum);
    si (ext_fractum || fons_sys == NIHIL)
    {
        redde FALSUM;
    }
    si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
            fons_sys, mensura_sys))
    {
        redde FALSUM;
    }
    systema_parsura = silva_c89_parsare(opus, "systema_c89.h",
        fons_sys, mensura_sys, NIHIL);
    si (   systema_parsura == NIHIL
        || systema_parsura->numerus_errorum > ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (silva_c89_semantica_analysare(opus, systema_parsura)
        == NIHIL)
    {
        redde FALSUM;
    }

    /* capita repositorii (inclusio vera) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(opus,
            DXII);

        si (visa != NIHIL)
        {
            _capita_praeparare(ctx, opus, visa, radix);
        }
    }

    oraculum = silva_oraculum_creare(opus);
    parsura  = silva_c89_parsare_cum_contextu(opus, ctx, via,
        fons_plagulae, mensura_plagulae, oraculum);
    si (   parsura            == NIHIL || !parsura->successus
        || parsura->commissio == NIHIL)
    {
        redde FALSUM;
    }
    si (parsura->fines_tactae)
    {
        fructus->recusa = VERUM;
        redde VERUM;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(opus, parsura,
        systema_parsura);
    si (sem != NIHIL && oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        (vacuum)silva_recanonicare(parsura->commissio, oraculum,
            silva_c89_resolutor, NIHIL);
        sem = silva_c89_semantica_analysare_cum_systemate(opus,
            parsura, systema_parsura);
    }
    si (sem == NIHIL)
    {
        redde FALSUM;
    }
    fructus->parsura  = parsura;
    fructus->sem      = sem;
    redde VERUM;
}

interior i32
_oraculum_numerare (
    constans SilvaSemantica* sem)
{
    i32 i;
    i32 m;
    i32 numerus = ZEPHYRUM;

    m = (i32)silva_c89_diagnostica_numerus(sem);
    per (i = ZEPHYRUM; i < m; i++)
    {
        constans SemanticaDiagnosticum* d =
            silva_c89_diagnosticum_per_indicem(sem, i);

        si (   d        != NIHIL
            && d->codex == (s32)EXAMEN_CODEX_VARIABILIS_INUTILIS)
        {
            numerus++;
        }
    }
    redde numerus;
}


/* ==================================================
 * Latus lint (formae eaedem ac portae L1 / lint II)
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
            || chorda_aequalis_literis(*n->titulus, "CATENA")
            || chorda_aequalis_literis(*n->titulus, "DIRIBITIO")
            || chorda_aequalis_literis(*n->titulus, "SINE")
            || chorda_aequalis_literis(*n->titulus, "INDAGO")
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


/* ==================================================
 * Plagula una: recipe -> oraculum + latus lint -> comparatio
 * ================================================== */

interior vacuum
_plagulam_probare (
     constans character* via,
     constans character* radix,
        constans chorda* lint_textus,
             LintCensus* census)
{
                 Piscina* opus;
           RecipeFructus  fructus;
     SilvaArborScriptura  scriptura;
     InternamentumChorda* intern;
            StmlResultus  lectio;
    StmlExpansioResultus  expansio;
                      i8* buffer;
                  chorda  textus_iunctus;
               StmlNodus* relatum;
                     i32  n_lint;
                     i32  n_oraculi;

    opus = piscina_generare_dynamicum("porta_codex_69", 268435456);
    si (opus == NIHIL)
    {
        redde;
    }
    census->plagulae++;

    si (!_recipe_examinis(opus, radix, via, &fructus))
    {
        census->fracturae++;
        imprimere("    RECIPE FRACTUM: %s\n", via);
        piscina_destruere(opus);
        redde;
    }
    si (fructus.recusa)
    {
        census->recusae++;
        piscina_destruere(opus);
        redde;
    }
    n_oraculi = _oraculum_numerare(fructus.sem);

    scriptura = silva_arbor_scribere_parsuram(opus, fructus.parsura,
        &SILVA_C89_REGISTRUM, "c89", fructus.parsura->fons_princeps,
        NIHIL);
    si (!scriptura.successus)
    {
        census->fracturae++;
        imprimere("    SCRIPTURA FRACTA: %s\n", via);
        piscina_destruere(opus);
        redde;
    }
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

    census->summa_lint     += n_lint;
    census->summa_oraculi  += n_oraculi;
    si (n_lint > n_oraculi)
    {
        /* soliditas fracta: situs quem oraculum non habet */
        census->excedentes++;
    }
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

    imprimere("  [%s] plagulae %d | fracturae %d | recusae %d |"
        " situs lint %d / oraculi %d | excedentes %d |"
        " divergentes %d | invariata violata %d\n",
        titulus, (integer)c->plagulae, (integer)c->fracturae,
        (integer)c->recusae, (integer)c->summa_lint,
        (integer)c->summa_oraculi, (integer)c->excedentes,
        (integer)c->divergentes, (integer)c->invariata_violata);
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

/* Fixturae examinis: suffixa verdictorum quattuor */
interior b32
_est_fixtura_examinis (
    constans character* titulus)
{
    constans character* punctum = strrchr(titulus, '.');

    si (punctum == NIHIL)
    {
        redde FALSUM;
    }
    redde strcmp(punctum, ".invalidum") == ZEPHYRUM
        || strcmp(punctum, ".domesticum") == ZEPHYRUM
        || strcmp(punctum, ".severum") == ZEPHYRUM
        || strcmp(punctum, ".suspectum") == ZEPHYRUM;
}

s32
principale (vacuum)
{
                 Piscina* piscina;
      constans character* radix;
               character  via_corporis[512];
               character  via_plagulae[1024];
                    DIR* corpus;
       structura dirent* introitus;
              LintCensus census_examinis;
              LintCensus census_bibliothecae;
                     i8* lint_datum;
                     i32 lint_mensura;
                  chorda lint_textus;
                     b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_codex_69",
                                         4194304);
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

    sprintf(via_plagulae,
        "%s/silva/probationes/fixa/exemplaria/inutilis_localis.stml",
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

    imprimere("\n--- PORTA CODEX 69: variabilis-inutilis"
              " (SINE, antiiunctio) ---\n");

    /* gradus fixturarum examinis (situs vivi) */
    memset(&census_examinis, 0, magnitudo(census_examinis));
    sprintf(via_corporis, "%s/silva/probationes/fixa/examinis",
            radix);
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
        si (introitus->d_type == DT_DIR)
        {
            perge;
        }
        si (!_est_fixtura_examinis(introitus->d_name))
        {
            perge;
        }
        sprintf(via_plagulae,
                "%s/silva/probationes/fixa/examinis/%s", radix,
                introitus->d_name);
        _plagulam_probare(via_plagulae, radix, &lint_textus,
                          &census_examinis);
    }
    closedir(corpus);
    _censum_referre("examinis", &census_examinis);

    /* gradus bibliothecae (latitudo) */
    memset(&census_bibliothecae, 0,
           magnitudo(census_bibliothecae));
    sprintf(via_corporis, "%s/lib", radix);
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
        si (!apparatus_est_c(introitus->d_name))
        {
            perge;
        }
        sprintf(via_plagulae, "%s/lib/%s", radix,
                introitus->d_name);
        _plagulam_probare(via_plagulae, radix, &lint_textus,
                          &census_bibliothecae);
    }
    closedir(corpus);
    _censum_referre("bibliotheca", &census_bibliothecae);


    /* ==================================================
     * PORTA: soliditas + mensurae pinnatae + praesentia
     * ================================================== */

    /* MENSURATUM 2026-08-31 (post refinamenta tria: caecitas
     * symmetrica, TRANSPARENTIA attributa=, aliasa resoluta):
     * consensus EXACTUS - examinis 1/1, bibliotheca 0/0,
     * divergentiae NULLAE. Residua nominata (declaratio secunda
     * formae eiusdem in scopo uno; formae ordinatio/monstrator-
     * initiatus; umbratio) in corpore hoc non occurrunt -
     * divergentes==0 adventum eorum clamabit. */
    CREDO_AEQUALIS_I32 (census_examinis.plagulae, 51);
    CREDO_AEQUALIS_I32 (census_examinis.fracturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_examinis.recusae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_examinis.excedentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_examinis.divergentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_examinis.invariata_violata,
                        ZEPHYRUM);

    CREDO_AEQUALIS_I32 (census_bibliothecae.plagulae, 156);
    CREDO_AEQUALIS_I32 (census_bibliothecae.fracturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_bibliothecae.recusae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_bibliothecae.excedentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_bibliothecae.divergentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_bibliothecae.invariata_violata,
                        ZEPHYRUM);

    /* CUSTOS TEGUMENTI: sine situbus vivis consensus zephyri
     * nihil probat */
    CREDO_VERUM (census_examinis.summa_oraculi > ZEPHYRUM);
    CREDO_VERUM (census_examinis.summa_lint > ZEPHYRUM);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
