/* nexus_percursus.c - NEXUS PERCURSUS (instrumenta prima #4a):
 * corpus totum semel parsatur + analysatur, omnis symboli sedes
 * declarationis et omnis sedes usus in build/nexus.tsv funduntur.
 * CLI nexus (nexus.c) tabulam millisecundis legit.
 *
 * Tabula DISPONIBILIS consulto (DECISUS sim-2 #3): numquam
 * committitur, scaffolding prae-acta; usus cibi canini schema
 * actorum alet. Regeneratio: ./silva/nexus.sh -renovare.
 *
 * Forma ordinis (septem campi, tab separati):
 *   titulus  species(sedes|usus)  genus  via  linea  columna  prof
 * prof = profunditas symboli (0 = scopus fili); usus systematis
 * (printf, malloc...) INCLUSI - "quis vocat malloc" quaestio vera
 * officinae erat (auditus aedificatorum M2).
 *
 * Duplicata (capita a multis TU inclusa) tabula clavium deduplicata.
 * Duo-passus percursus exemplar: sem2 post recanonicare AUCTORITAS
 * (symbolum_nodi canonicae-RELATIVUM est).
 */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_expandere.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens i32 mensura_maxima = 4194304;  /* -omnia = sine tecto */

hic_manens FILE* effusio = NIHIL;
hic_manens TabulaDispersa* clavium_visa = NIHIL;
hic_manens Piscina* piscina_clavium = NIHIL;

hic_manens i32 plagulae = ZEPHYRUM;
hic_manens i32 ordines_scripti = ZEPHYRUM;
hic_manens i32 ordines_duplicati = ZEPHYRUM;
hic_manens i32 praetermissae = ZEPHYRUM;

hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;

hic_manens constans character*
_genus_titulus (s32 genus)
{
    commutatio (genus)
    {
        casus SYMBOLUM_VARIABILE:  redde "variabile";
        casus SYMBOLUM_FUNCTIO:    redde "functio";
        casus SYMBOLUM_TYPEDEF:    redde "typedef";
        casus SYMBOLUM_CONSTANS:   redde "constans";
        casus SYMBOLUM_PARAMETRUM: redde "parametrum";
        ordinarius:                redde "?";
    }
}

/* ordinem scribere (deduplicatum per clavem plenam) */
hic_manens vacuum
_ordinem_scribere (constans chorda* titulus,
    constans character* species, constans character* genus,
    constans chorda* via, i32 linea, i32 columna, i32 profunditas)
{
    character clavis_litterae[1024];
    chorda clavis;
    int scripti;

    chorda via_nuda;

    si (effusio == NIHIL || titulus == NIHIL || via == NIHIL)
    {
        redde;
    }
    /* praefixum "./" ambulationis detrahere (cosmetica viae;
     * via_nuda in stiva - via intra hanc functionem solum vivit) */
    si (via->mensura > II && via->datum[ZEPHYRUM] == '.'
        && via->datum[I] == '/')
    {
        via_nuda.datum = via->datum + II;
        via_nuda.mensura = (i32)(via->mensura - II);
        via = &via_nuda;
    }
    si (titulus->mensura == ZEPHYRUM
        || (memoriae_index)titulus->mensura
            + (memoriae_index)via->mensura + LXIV
            >= magnitudo(clavis_litterae))
    {
        redde;
    }
    scripti = sprintf(clavis_litterae, "%.*s\t%s\t%.*s\t%u\t%u",
        (int)titulus->mensura,
        (constans character*)titulus->datum,
        species,
        (int)via->mensura, (constans character*)via->datum,
        linea, columna);
    si (scripti <= ZEPHYRUM) redde;
    /* chorda_ex_literis COPIAT in piscinam (clavis stabilis -
     * buffer stivae reusatur) */
    clavis = chorda_ex_literis(clavis_litterae, piscina_clavium);
    si (tabula_dispersa_continet(clavium_visa, clavis))
    {
        ordines_duplicati++;
        redde;
    }
    (vacuum)tabula_dispersa_inserere(clavium_visa, clavis, NIHIL);

    fprintf(effusio, "%.*s\t%s\t%s\t%.*s\t%u\t%u\t%u\n",
        (int)titulus->mensura,
        (constans character*)titulus->datum,
        species, genus,
        (int)via->mensura, (constans character*)via->datum,
        linea, columna, profunditas);
    ordines_scripti++;
}

/* positio lexematis per radicem originis */
hic_manens b32
_positionem_capere (constans SilvaParsura* parsura, SilvaToken* t,
    constans chorda** via_exitus, i32* linea_exitus,
    i32* columna_exitus)
{
    SilvaToken* radix;
    constans chorda* via;

    si (t == NIHIL || parsura->expansio == NIHIL) redde FALSUM;
    radix = silva_token_radix(t);
    si (radix == NIHIL || radix->byte_offset < ZEPHYRUM)
    {
        redde FALSUM;
    }
    via = silva_fons_via(parsura->expansio, radix->fons_index);
    si (via == NIHIL) redde FALSUM;
    /* viae sine '/' = tituli praebiti (basename capitis intra TU)
     * - ambulatio directa .h ordinem canonicum via plena praebet;
     * copiae basename praetermissae (aliter omne caput BIS) */
    {
        b32 habet_separatorem = FALSUM;
        s32 k;

        per (k = ZEPHYRUM; k < (s32)via->mensura; k++)
        {
            si (via->datum[k] == '/')
            {
                habet_separatorem = VERUM;
                frange;
            }
        }
        si (!habet_separatorem) redde FALSUM;
    }
    *via_exitus = via;
    *linea_exitus = radix->linea;
    *columna_exitus = radix->columna;
    redde VERUM;
}

/* sedes declarationum: omnia symbola TU (systema exclusa -
 * sedes eorum in systema_c89.h synthetico nihil dicit) */
hic_manens vacuum
_symbola_fundere (constans SilvaParsura* parsura,
    constans SilvaSemantica* sem)
{
    i32 numerus = silva_c89_symbola_numerus(sem);
    i32 k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans SemanticaSymbolum* symbolum =
            silva_c89_symbolum_per_indicem(sem, k);
        constans chorda* via;
        i32 linea;
        i32 columna;

        si (symbolum == NIHIL) perge;
        si (symbolum->ex_systemate) perge;
        si (symbolum->est_implicitum) perge;
        si (symbolum->lexema == NIHIL) perge;
        si (!_positionem_capere(parsura, symbolum->lexema, &via,
                &linea, &columna))
        {
            perge;
        }
        _ordinem_scribere(&symbolum->titulus, "sedes",
            _genus_titulus(symbolum->genus), via, linea, columna,
            symbolum->profunditas);
    }
}

/* sedes usus: folia identificatorum (vocati vocationum FOLIA
 * quoque sunt - etiam vocati impliciti synthetizati ligant,
 * inventum M1a; VOCATIO ergo non separatim quaerenda) */
hic_manens vacuum
_usus_fundere (constans SilvaParsura* parsura,
    constans SilvaSemantica* sem, Piscina* piscina)
{
    Xar* series = xar_creare(piscina, magnitudo(SilvaValor));
    i32 cursor = ZEPHYRUM;

    si (series == NIHIL) redde;
    {
        SilvaValor* radix = (SilvaValor*)xar_addere(series);

        si (radix == NIHIL) redde;
        *radix = parsura->commissio->radix;
    }

    dum (cursor < xar_numerus(series))
    {
        SilvaValor v = *(SilvaValor*)xar_obtinere(series, cursor);

        cursor++;

        si (v.genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(v); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    SilvaValor* novus =
                        (SilvaValor*)xar_addere(series);

                    si (novus != NIHIL) *novus = *elem;
                }
            }
            perge;
        }
        si (v.genus != SILVA_VALOR_NODUS || v.datum.nodus == NIHIL)
        {
            perge;
        }

        {
            constans SilvaNodus* nodus = v.datum.nodus;
            i32 k;

            si (nodus->genus == (s32)SILVA_C89_GENUS_AMBIGUUS)
            {
                SilvaValor interp =
                    silva_c89_ambiguus_interpretationes(nodus);
                SilvaValor canonica =
                    silva_c89_ambiguus_canonica(nodus);

                si (canonica.genus == SILVA_VALOR_INDEX)
                {
                    SilvaValor* lectio = silva_valor_lista_obtinere(
                        interp, (i32)canonica.datum.index);

                    si (lectio != NIHIL)
                    {
                        SilvaValor* novus =
                            (SilvaValor*)xar_addere(series);

                        si (novus != NIHIL) *novus = *lectio;
                    }
                }
                perge;
            }
            si (nodus->genus == (s32)SILVA_C89_GENUS_ERROR
                || nodus->genus
                    == (s32)SILVA_C89_GENUS_RAMUS_OMISSUS)
            {
                perge;
            }

            si (nodus->genus
                == (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR)
            {
                constans SemanticaSymbolum* symbolum =
                    silva_c89_symbolum_nodi(sem, nodus);

                si (symbolum != NIHIL)
                {
                    SilvaValor tok =
                        silva_c89_folium_identificator_tok_valor(
                            nodus);

                    si (tok.genus == SILVA_VALOR_TOKEN
                        && tok.datum.token != NIHIL)
                    {
                        constans chorda* via;
                        i32 linea;
                        i32 columna;

                        si (_positionem_capere(parsura,
                                tok.datum.token, &via, &linea,
                                &columna))
                        {
                            _ordinem_scribere(&symbolum->titulus,
                                "usus",
                                _genus_titulus(symbolum->genus),
                                via, linea, columna,
                                symbolum->profunditas);
                        }
                    }
                }
            }

            per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
            {
                SilvaValor* novus = (SilvaValor*)xar_addere(series);

                si (novus != NIHIL) *novus = nodus->loci[k];
            }
        }
    }
}

/* --------------------------------------------------
 * per plagulam: duo-passus percursus exemplar
 * -------------------------------------------------- */

hic_manens vacuum
_plagulam_percurrere (constans SilvaContextus* ctx,
    constans character* via)
{
    Piscina* piscina;
    FILE* pl;
    i8* fons;
    long mensura_l;
    i32 mensura;
    SilvaParsura* parsura;
    SilvaOraculum* oraculum_plagulae = NIHIL;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde;
    }
    mensura = (i32)mensura_l;
    si (mensura_maxima > ZEPHYRUM && mensura > mensura_maxima)
    {
        fprintf(stderr, "[praetermissa mensura %.1f MB] %s\n",
            (duplex)mensura / 1048576.0, via);
        praetermissae++;
        fclose(pl);
        redde;
    }

    piscina = piscina_generare_dynamicum("nexus_percursus",
        8388608);
    si (piscina == NIHIL)
    {
        fclose(pl);
        redde;
    }
    fons = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        piscina_destruere(piscina);
        redde;
    }
    fclose(pl);

    plagulae++;

    oraculum_plagulae = silva_oraculum_creare(piscina);
    si (oraculum_plagulae != NIHIL && systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            systema_semantica, oraculum_plagulae);
    }
    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
        (constans character*)fons, mensura, oraculum_plagulae);

    si (parsura != NIHIL && parsura->successus)
    {
        SilvaSemantica* sem =
            silva_c89_semantica_analysare_cum_systemate(piscina,
                parsura, systema_parsura);

        si (sem != NIHIL && oraculum_plagulae != NIHIL)
        {
            (vacuum)silva_c89_semantica_oraculum_augere(sem,
                oraculum_plagulae);
            silva_oraculum_responsa_vacare(oraculum_plagulae);
            (vacuum)silva_recanonicare(parsura->commissio,
                oraculum_plagulae, silva_c89_resolutor, NIHIL);
            /* sem2 AUCTORITAS post recanonicare (canonicae-
             * relativum: symbolum_nodi contra arborem versam) */
            sem = silva_c89_semantica_analysare_cum_systemate(
                piscina, parsura, systema_parsura);
        }
        si (sem != NIHIL)
        {
            _symbola_fundere(parsura, sem);
            _usus_fundere(parsura, sem, piscina);
        }
        alioquin
        {
            fprintf(stderr, "[SEMANTICA FRACTA] %s\n", via);
        }
    }
    alioquin
    {
        fprintf(stderr, "[SINE ARBORE] %s\n", via);
    }
    piscina_destruere(piscina);
}

/* --------------------------------------------------
 * ambulatio + capita (exemplar percursus/censor)
 * -------------------------------------------------- */

hic_manens b32
_est_fons_c (constans character* titulus)
{
    memoriae_index m = strlen(titulus);

    si (m < III) redde FALSUM;
    si (titulus[m - II] != '.') redde FALSUM;
    redde (titulus[m - I] == 'c' || titulus[m - I] == 'h')
        ? VERUM : FALSUM;
}

hic_manens b32
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM
        /* flagrationes consulto plantatae censoris */
        || strcmp(titulus, "censoris") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

hic_manens vacuum
_directorium_percurrere (constans SilvaContextus* ctx,
    constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL) redde;
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (_praetermittendum(introitus->d_name)) perge;
        si (strlen(via) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via, introitus->d_name);

        si (introitus->d_type == DT_DIR)
        {
            _directorium_percurrere(ctx, via_plena);
        }
        alioquin si (_est_fons_c(introitus->d_name))
        {
            _plagulam_percurrere(ctx, via_plena);
        }
    }
    closedir(dir);
}

hic_manens vacuum
_caput_praebere (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via,
    constans character* titulus)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    character* textus;
    chorda clavis;

    clavis = chorda_ex_literis(titulus, piscina);
    si (tabula_dispersa_continet(visa, clavis)) redde;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde;
    }
    mensura = (i32)mensura_l;

    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (textus == NIHIL || (mensura > ZEPHYRUM
        && fread(textus, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        redde;
    }
    fclose(pl);

    si (silva_contextus_praebere(ctx, titulus, textus, mensura))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
    }
}

hic_manens vacuum
_capita_praeparare (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

    si (dir == NIHIL) redde;
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (_praetermittendum(introitus->d_name)) perge;
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
            si (m >= III && introitus->d_name[m - II] == '.'
                && introitus->d_name[m - I] == 'h')
            {
                _caput_praebere(ctx, piscina, visa, via_plena,
                    introitus->d_name);
            }
        }
    }
    closedir(dir);
}

s32 principale (integer argc, character** argv)
{
    Piscina* piscina_ctx;
    SilvaContextus* ctx;
    constans character* radix = ".";
    constans character* via_effusionis = "build/nexus.tsv";
    integer k;
    clock_t c0;
    clock_t c1;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;
        }
        alioquin
        {
            radix = argv[k];
        }
    }

    piscina_ctx = piscina_generare_dynamicum("nexus_ctx", 8388608);
    si (piscina_ctx == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: piscina deest\n");
        redde I;
    }
    piscina_clavium = piscina_generare_dynamicum("nexus_claves",
        8388608);
    si (piscina_clavium == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: piscina clavium deest\n");
        redde I;
    }
    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: contextus deest\n");
        redde I;
    }
    clavium_visa = tabula_dispersa_creare_chorda(piscina_clavium,
        DXII);
    si (clavium_visa == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: tabula deest\n");
        redde I;
    }

    /* systema semel parsatum (exemplar percursus -semantica) */
    {
        FILE* pl_sys = fopen("silva/fontes/systema_c89.h", "rb");
        long mensura_sys;
        character* fons_sys;

        si (pl_sys == NIHIL)
        {
            fprintf(stderr, "nexus_percursus: systema_c89.h deest"
                " (curre ex radice repositorii)\n");
            redde I;
        }
        fseek(pl_sys, 0L, SEEK_END);
        mensura_sys = ftell(pl_sys);
        fseek(pl_sys, 0L, SEEK_SET);
        fons_sys = (character*)piscina_allocare(piscina_ctx,
            (memoriae_index)(mensura_sys + 1L));
        si (fons_sys == NIHIL
            || fread(fons_sys, I, (memoriae_index)mensura_sys,
                   pl_sys) != (memoriae_index)mensura_sys)
        {
            fprintf(stderr,
                "nexus_percursus: systema non lectum\n");
            fclose(pl_sys);
            redde I;
        }
        fclose(pl_sys);
        si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
                fons_sys, (i32)mensura_sys))
        {
            fprintf(stderr,
                "nexus_percursus: lexicon non additum\n");
            redde I;
        }
        systema_parsura = silva_c89_parsare(piscina_ctx,
            "systema_c89.h", fons_sys, (i32)mensura_sys, NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr,
                "nexus_percursus: systema non parsatum\n");
            redde I;
        }
        systema_semantica = silva_c89_semantica_analysare(
            piscina_ctx, systema_parsura);
        si (systema_semantica == NIHIL)
        {
            fprintf(stderr,
                "nexus_percursus: systema non analysatum\n");
            redde I;
        }
    }

    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL)
        {
            fprintf(stderr, "nexus_percursus: tabula deest\n");
            redde I;
        }
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    effusio = fopen(via_effusionis, "w");
    si (effusio == NIHIL)
    {
        fprintf(stderr, "nexus_percursus: %s non aperta"
            " (build/ adest?)\n", via_effusionis);
        redde I;
    }
    {
        time_t nunc = time(NIHIL);

        fprintf(effusio, "# nexus.tsv GENERATUM %ld"
            " - DISPONIBILE, noli committere;"
            " regenera: ./silva/nexus.sh -renovare\n",
            (long)nunc);
    }

    c0 = clock();
    _directorium_percurrere(ctx, radix);
    c1 = clock();
    fclose(effusio);

    fprintf(stderr, "\n=== NEXUS PERCURSUS ===\n");
    fprintf(stderr, "plagulae: %d", (int)plagulae);
    si (praetermissae > ZEPHYRUM)
    {
        fprintf(stderr, "  [+%d praetermissae]",
            (int)praetermissae);
    }
    fprintf(stderr, "\nordines:  %d scripti; %d duplicati"
        " (capita inclusa)\n", (int)ordines_scripti,
        (int)ordines_duplicati);
    fprintf(stderr, "tempus:   %.0f s\n",
        (duplex)(c1 - c0) / (duplex)CLOCKS_PER_SEC);
    fprintf(stderr, "tabula:   %s\n", via_effusionis);
    redde ZEPHYRUM;
}
