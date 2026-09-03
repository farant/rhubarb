/* probatio_silva_exemplaria_obsoleta.c - PORTA LINT II: codex 87
 *
 * Primus codex VERUS examinis ut documentum exemplarium
 * (fixa/exemplaria/obsoletum_usleep.stml): symbolum obsoletum
 * adhibitum (tabula <gradus> in fontes/systema_posix.h - hodie
 * introitus UNUS: usleep -> nanosleep). Oraculum = ANALYSATOR
 * IPSE: recipe examinis verbatim (lexicon compositum + capita
 * praebita + oraculum + bis-analysis post recanonicare), deinde
 * diagnostica ad codicem 87 cribrata - differentia multo fortior
 * quam ambulatio manu scripta portae L1.
 *
 * PARSURA UNA, IUDICES DUO: eadem parsura et semanticae datur et
 * scriptori arboris - latus lint documentum eiusdem arboris legit
 * quam analysator iudicat.
 *
 * CORPUS: plagulae .c in lib (156 - latitudo consensus zephyri:
 * unus situs vivus, tcp_posix.c) + sex probationes nominatae (XXIV
 * situs vivi - lex praesentiae; index fixus CLARE rumpitur si
 * plagula migrat). Gradus roundtrip OMISSUS consulto: situs
 * zephyrum ubique afferret quos lib iam praebet, LXXVIII cursus
 * recipe pro nihilo.
 *
 * NUMERI, non positiones: extenta expansorum foramen provenientiae
 * manent (spec par. 1.3) - aequalitas numerorum per plagulam, ut
 * in porta L1.
 *
 * TRANSPARENTIA MINIMA: involucra originis L1 + regio-cruda
 * (bracchia omissa in crudis latent - examen ea non ambulat).
 * Additiones ulteriores divergentia agantur, numquam praesumptione.
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

#define DIVERGENTIAE_MAX 8

nomen structura {
          i32 plagulae;
          i32 fracturae;
          i32 recusae;
          i32 divergentes;
          i32 diribitio_divergentes;
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

/* Caput repositorii SEMEL lectum: via + textus in piscina probationis.
 * Ante hoc quaeque plagula probata directoria omnia ambulabat et
 * capita omnia legebat (CLVI vicibus; XIV% temporis, 2026-09-02) -
 * nunc lectio una, praebitio per plagulam (contextus textum copiat). */
nomen structura {
             character  via[1024];
    constans        i8* textus;
                   i32  mensura;
} CaputLectum;

interior vacuum
_caput_legere (
           Piscina* piscina,
    TabulaDispersa* visa,
constans character* via,
               Xar* capita)
{
         chorda  clavis;
             i8* textus;
            i32  mensura;
    CaputLectum* locus;

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
    locus = (CaputLectum*)xar_addere(capita);
    si (locus == NIHIL)
    {
        redde;
    }
    strcpy(locus->via, via);
    locus->textus   = textus;
    locus->mensura  = mensura;
    (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
}

interior vacuum
_capita_legere (
           Piscina* piscina,
    TabulaDispersa* visa,
constans character* via,
               Xar* capita)
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
            _capita_legere(piscina, visa, via_plena, capita);
        }
        alioquin
        {
            m = strlen(introitus->d_name);
            si (   m                         >= III
                && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I]  == 'h')
            {
                _caput_legere(piscina, visa, via_plena, capita);
            }
        }
    }
    closedir(dir);
}

/* capita lecta contextui plagulae praebere (ordine lectionis) */
interior vacuum
_capita_praebere (
    SilvaContextus* ctx,
      constans Xar* capita)
{
    i32 i;
    i32 n;

    n = xar_numerus(capita);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans CaputLectum* c;

        c = (constans CaputLectum*)xar_obtinere(capita, i);
        (vacuum)silva_contextus_praebere(ctx, c->via,
            (constans character*)c->textus, c->mensura);
    }
}

/* FALSUM = apparatus fractus (fracturae); recusa = fines tactae */
interior b32
_recipe_examinis (
               Piscina* opus,
    constans character* radix,
    constans character* via,
          constans Xar* capita,
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

    /* capita repositorii (inclusio vera) - lecta semel, praebita hic */
    _capita_praebere(ctx, capita);

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
            && d->codex == (s32)EXAMEN_CODEX_OBSOLETUM_ADHIBITUM)
        {
            numerus++;
        }
    }
    redde numerus;
}


/* ==================================================
 * Latus lint (formae eaedem ac porta L1)
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

/* Documentum lint alterum contra textum arboris eundem expandere;
 * relatum expansum reddit (NIHIL = fractura, iam numerata). */
interior StmlNodus*
_relatum_alterum_expandere (
                Piscina* opus,
     constans character* via,
        constans chorda* textus_arboris,
        constans chorda* lint_textus,
             LintCensus* census)
{
                      i8* buffer;
                  chorda  textus_iunctus;
     InternamentumChorda* intern;
            StmlResultus  lectio;
    StmlExpansioResultus  expansio;
               StmlNodus* relatum;

    buffer = (i8*)piscina_allocare(opus,
        (memoriae_index)(textus_arboris->mensura
                         + lint_textus->mensura));
    si (buffer == NIHIL)
    {
        census->fracturae++;
        redde NIHIL;
    }
    memcpy(buffer, textus_arboris->datum,
           (memoriae_index)textus_arboris->mensura);
    memcpy(buffer + textus_arboris->mensura, lint_textus->datum,
           (memoriae_index)lint_textus->mensura);
    textus_iunctus.datum    = buffer;
    textus_iunctus.mensura  = textus_arboris->mensura
                            + lint_textus->mensura;

    intern = internamentum_creare(opus);
    si (intern == NIHIL)
    {
        census->fracturae++;
        redde NIHIL;
    }
    lectio = stml_legere(textus_iunctus, opus, intern);
    si (!lectio.successus)
    {
        census->fracturae++;
        imprimere("    LECTIO ALTERA FRACTA: %s (linea %d)\n", via,
                  (integer)lectio.linea_erroris);
        redde NIHIL;
    }
    expansio = stml_expandere(lectio.radix, opus, intern);
    si (!expansio.successus)
    {
        census->fracturae++;
        imprimere("    EXPANSIO ALTERA FRACTA: %s (vitium %d,"
                  " linea %d)\n", via, (integer)expansio.vitium,
                  (integer)expansio.linea);
        redde NIHIL;
    }
    census->invariata_violata +=
        _invariata_numerare(expansio.radix_expansa);
    relatum = _elementum_invenire(expansio.radix_expansa, "relatum");
    si (relatum == NIHIL)
    {
        census->fracturae++;
        imprimere("    RELATUM ALTERUM DEEST: %s\n", via);
    }
    redde relatum;
}


/* ==================================================
 * Plagula una: recipe -> oraculum + latus lint -> comparatio
 * ================================================== */

interior vacuum
_plagulam_probare (
     constans character* via,
     constans character* radix,
        constans chorda* lint_textus,
        constans chorda* lint_diribitio,
             LintCensus* census,
           constans Xar* capita)
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

    opus = piscina_generare_dynamicum("porta_lint_ii", 268435456);
    si (opus == NIHIL)
    {
        redde;
    }
    census->plagulae++;

    si (!_recipe_examinis(opus, radix, via, capita, &fructus))
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

    /* PORTA DIRIBITIONIS: catena sui-custodita byte-pro-byte */
    {
        StmlNodus* relatum_diribitionis;

        relatum_diribitionis = _relatum_alterum_expandere(opus,
            via, &scriptura.textus, lint_diribitio, census);
        si (relatum_diribitionis == NIHIL)
        {
            piscina_destruere(opus);
            redde;
        }
        {
            chorda scriptum_planum;
            chorda scriptum_diribitionis;

            scriptum_planum        = stml_scribere(relatum, opus,
                                                   FALSUM);
            scriptum_diribitionis  = stml_scribere(
                relatum_diribitionis, opus, FALSUM);
            si (!chorda_aequalis(scriptum_planum,
                                 scriptum_diribitionis))
            {
                census->diribitio_divergentes++;
                imprimere("    DIRIBITIO DIVERGIT: %s\n", via);
            }
        }
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

    imprimere("  [%s] plagulae %d | fracturae %d | recusae %d |"
        " situs lint %d / oraculi %d | divergentes %d |"
        " diribitio divergentes %d | invariata violata %d\n",
        titulus, (integer)c->plagulae, (integer)c->fracturae,
        (integer)c->recusae, (integer)c->summa_lint,
        (integer)c->summa_oraculi, (integer)c->divergentes,
        (integer)c->diribitio_divergentes,
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

/* Plagulae probationum cum situbus vivis (census 2026-08-31; index
 * fixus CLARE rumpitur - plagula amota = FRACTA, numquam tacite) */
interior constans character* PROBATIONES_VIVAE[] = {
    "probationes/probatio_tempestivum.c",
    "probationes/probatio_tcp_servus.c",
    "probationes/probatio_actor_integratio.c",
    "probationes/probatio_reactor.c",
    "probationes/probatio_uuid.c",
    "probationes/probatio_http.c"
};

s32
principale (vacuum)
{
                 Piscina* piscina;
      constans character* radix;
               character  via_corporis[512];
               character  via_plagulae[1024];
                    DIR* corpus;
       structura dirent* introitus;
              LintCensus census_bibliothecae;
              LintCensus census_probationum;
                     i8* lint_datum;
                     i32 lint_mensura;
                  chorda lint_textus;
                     i8* diribitio_datum;
                     i32 diribitio_mensura;
                  chorda lint_diribitio;
                     i32 i;
                     b32 praeteritus;
    Xar* capita;

    piscina = piscina_generare_dynamicum("probatio_lint_ii",
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
        "%s/silva/probationes/fixa/exemplaria/obsoletum_usleep.stml",
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

    /* capita repositorii SEMEL lecta (vide CaputLectum) */
    capita = xar_creare(piscina, (i32)magnitudo(CaputLectum));
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(piscina,
            DXII);

        si (capita != NIHIL && visa != NIHIL)
        {
            _capita_legere(piscina, visa, radix, capita);
        }
    }

    sprintf(via_plagulae,
        "%s/silva/probationes/fixa/exemplaria/"
        "obsoletum_usleep_diribitio.stml", radix);
    diribitio_datum = apparatus_plagulam_legere(piscina,
        via_plagulae, &diribitio_mensura);
    si (diribitio_datum == NIHIL || diribitio_mensura <= ZEPHYRUM)
    {
        imprimere("FRACTA: documentum diribitionis non lectum:"
                  " %s\n", via_plagulae);
        credo_imprimere_compendium();
        piscina_destruere(piscina);
        redde I;
    }
    lint_diribitio.datum    = diribitio_datum;
    lint_diribitio.mensura  = diribitio_mensura;

    imprimere("\n--- PORTA LINT II: obsoletum-adhibitum"
              " (codex 87) ---\n");

    /* gradus bibliothecae */
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
            &lint_diribitio, &census_bibliothecae, capita);
    }
    closedir(corpus);
    _censum_referre("bibliotheca", &census_bibliothecae);

    /* gradus probationum (index fixus - situs vivi) */
    memset(&census_probationum, 0, magnitudo(census_probationum));
    per (i = ZEPHYRUM;
         i < (i32)(magnitudo(PROBATIONES_VIVAE)
                   / magnitudo(PROBATIONES_VIVAE[ZEPHYRUM]));
         i++)
    {
        sprintf(via_plagulae, "%s/%s", radix, PROBATIONES_VIVAE[i]);
        _plagulam_probare(via_plagulae, radix, &lint_textus,
            &lint_diribitio, &census_probationum, capita);
    }
    _censum_referre("probationes", &census_probationum);


    /* ==================================================
     * PORTA: aequalitas per plagulam + praesentia
     * ================================================== */

    CREDO_AEQUALIS_I32 (census_bibliothecae.plagulae, 156);
    CREDO_AEQUALIS_I32 (census_bibliothecae.fracturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_bibliothecae.recusae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_bibliothecae.divergentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_bibliothecae.diribitio_divergentes,
                        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_bibliothecae.invariata_violata,
                        ZEPHYRUM);

    CREDO_AEQUALIS_I32 (census_probationum.plagulae, VI);
    CREDO_AEQUALIS_I32 (census_probationum.fracturae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_probationum.recusae, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_probationum.divergentes, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_probationum.diribitio_divergentes,
                        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (census_probationum.invariata_violata,
                        ZEPHYRUM);

    /* CUSTOS TEGUMENTI: praesentia utroque gradu - sine situbus
     * vivis consensus zephyri nihil probat */
    CREDO_VERUM (census_bibliothecae.summa_lint > ZEPHYRUM);
    CREDO_VERUM (census_probationum.summa_lint > ZEPHYRUM);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
