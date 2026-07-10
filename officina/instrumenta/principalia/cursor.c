/* cursor.c - Cursor corporis: instrumentum vectis corporis (M2a v0)
 *
 * M2a: NEXUS MUNDI solum - omnes plagulae plagulae .c sub lib/ demittuntur SEMEL
 * (piscina officinae communis - moduli cachati), tum + modulus
 * probationis (probatio_piscina.c) in UNAM conexionem nectuntur.
 * Vectis: 0 fracturae oneris; decipulae numeratae + NOMINATAE;
 * $main inventum; relocationes sarcitae.
 *
 * M2c crescet: exsecutio per machinulam, tabula classificationis,
 * columna stdout-diff, tempus per plagulam probationis.
 *
 * Fistula bis-analysans ex fusore exscripta (superficies publica
 * silva.h). Piscinae arborum per TU destructae (arbores numquam
 * cumulantur - responsum apici 9.7GB); piscina officinae UNA manet
 * (moduli vivunt trans nexum).
 *
 * Usus: ./officina/cursor.sh   (ex radice repositorii)
 */

#include "officina_demissio.h"
#include "officina_conexio.h"
#include "officina_machinula.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

/* ==================================================
 * Status globalis
 * ================================================== */

hic_manens i32 mensura_maxima = 4194304;

hic_manens long plagulae = 0L;
hic_manens long parsurae_fractae = 0L;
hic_manens long ruinae_demissionis = 0L;
hic_manens long summa_functionum = 0L;
hic_manens long summa_datorum = 0L;
hic_manens long summa_instructionum = 0L;
hic_manens duplex ms_arboris = 0.0;
hic_manens duplex ms_demissionis = 0.0;

hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;

hic_manens Piscina* piscina_officinae = NIHIL;   /* moduli cachati */
hic_manens Xar* moduli_mundi = NIHIL;            /* MedullaModulus* */

/* ==================================================
 * Plagulam demittere (fistula fusoris; modulus CACHATUR)
 * ================================================== */

hic_manens MedullaModulus*
_plagulam_demittere (constans SilvaContextus* ctx,
    constans character* via)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    SilvaPiscina* piscina_arboris;
    i8* fons;
    SilvaParsura* parsura;
    SilvaOraculum* oraculum;
    SilvaSemantica* sem;
    MedullaModulus* modulus;
    clock_t c0;
    clock_t c1;
    chorda titulus_moduli;
    unio { constans character* c; i8* m; } u;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde NIHIL;
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = (i32)mensura_l;
    si (mensura_maxima > ZEPHYRUM && mensura > (i32)mensura_maxima)
    {
        fprintf(stderr, "[PRAETERMISSA mensura] %s\n", via);
        fclose(pl);
        redde NIHIL;
    }

    piscina_arboris = silva_piscina_generare_dynamicum(
        "cursor_arbor", 8388608);
    si (piscina_arboris == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    fons = (i8*)piscina_allocare(piscina_officinae,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);

    plagulae++;

    oraculum = silva_oraculum_creare(piscina_arboris);
    si (oraculum != NIHIL && systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            systema_semantica, oraculum);
    }
    c0 = clock();
    u.m = fons;
    parsura = silva_c89_parsare_cum_contextu(piscina_arboris, ctx,
        via, u.c, (unsigned int)mensura, oraculum);
    si (parsura == NIHIL || !parsura->successus
        || parsura->numerus_errorum > ZEPHYRUM)
    {
        fprintf(stderr, "[PARSURA FRACTA] %s\n", via);
        parsurae_fractae++;
        silva_piscina_destruere(piscina_arboris);
        redde NIHIL;
    }
    sem = silva_c89_semantica_analysare_cum_systemate(
        piscina_arboris, parsura, systema_parsura);
    si (sem != NIHIL && oraculum != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(sem, oraculum);
        silva_oraculum_responsa_vacare(oraculum);
        (vacuum)silva_recanonicare(parsura->commissio, oraculum,
            silva_c89_resolutor, NIHIL);
        sem = silva_c89_semantica_analysare_cum_systemate(
            piscina_arboris, parsura, systema_parsura);
    }
    c1 = clock();
    ms_arboris += (duplex)(c1 - c0) * 1000.0
        / (duplex)CLOCKS_PER_SEC;
    si (sem == NIHIL)
    {
        fprintf(stderr, "[SEMANTICA FRACTA] %s\n", via);
        parsurae_fractae++;
        silva_piscina_destruere(piscina_arboris);
        redde NIHIL;
    }

    u.c = via;
    titulus_moduli.datum = u.m;
    titulus_moduli.mensura = (i32)strlen(via);
    c0 = clock();
    modulus = demissio_currere(piscina_officinae, parsura, sem,
        titulus_moduli);
    c1 = clock();
    ms_demissionis += (duplex)(c1 - c0) * 1000.0
        / (duplex)CLOCKS_PER_SEC;
    si (modulus == NIHIL)
    {
        fprintf(stderr, "[DEMISSIO RUIT] %s\n", via);
        ruinae_demissionis++;
        silva_piscina_destruere(piscina_arboris);
        redde NIHIL;
    }

    summa_functionum += (long)xar_numerus(modulus->functiones);
    summa_datorum += (long)xar_numerus(modulus->data);
    {
        i32 f;
        i32 numerus_functionum = xar_numerus(modulus->functiones);

        per (f = ZEPHYRUM; f < numerus_functionum; f++)
        {
            MedullaFunctio** functio =
                (MedullaFunctio**)xar_obtinere(modulus->functiones,
                    (i32)f);
            i32 b;
            i32 numerus_bloccorum = xar_numerus((*functio)->blocci);

            per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
            {
                constans MedullaBloccus* bloccus =
                    (constans MedullaBloccus*)xar_obtinere(
                        (*functio)->blocci, (i32)b);

                summa_instructionum +=
                    (long)xar_numerus(bloccus->instructiones);
            }
        }
    }

    /* arbores numquam cumulantur; origo pendens - clavis opaca
     * solum, numquam dereferenda */
    silva_piscina_destruere(piscina_arboris);
    redde modulus;
}

/* ==================================================
 * Capita praeparare (fusoris exscriptum)
 * ================================================== */

hic_manens b32
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
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
    si (tabula_dispersa_continet(visa, clavis))
    {
        redde;
    }
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
    si (silva_contextus_praebere(ctx, titulus, textus,
            (unsigned int)mensura))
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

/* ==================================================
 * Mundus: plagulae .c sub lib/ (ordine alphabetico - determinismus)
 * ================================================== */

hic_manens integer
_comparare_vias (constans vacuum* a, constans vacuum* b)
{
    redde strcmp(*(character* constans*)a, *(character* constans*)b);
}

hic_manens vacuum
_bibliothecam_demittere (constans SilvaContextus* ctx)
{
    DIR* dir = opendir("lib");
    structura dirent* introitus;
    character* viae[512];
    integer numerus = 0;
    integer i;

    si (dir == NIHIL)
    {
        fprintf(stderr, "cursor: lib/ deest (curre ex radice)\n");
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL && numerus < 512)
    {
        memoriae_index m = strlen(introitus->d_name);

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (m >= III && introitus->d_name[m - II] == '.'
            && introitus->d_name[m - I] == 'c')
        {
            character* via = piscina_allocare(piscina_officinae,
                m + V);

            sprintf(via, "lib/%s", introitus->d_name);
            viae[numerus] = via;
            numerus++;
        }
    }
    closedir(dir);

    /* readdir ordo indeterminatus - ordinare (determinismus) */
    qsort(viae, (memoriae_index)numerus, magnitudo(character*),
        _comparare_vias);
    per (i = 0; i < numerus; i++)
    {
        MedullaModulus* modulus = _plagulam_demittere(ctx, viae[i]);

        si (modulus != NIHIL)
        {
            MedullaModulus** locellus = xar_addere(moduli_mundi);

            *locellus = modulus;
        }
    }
}

/* ==================================================
 * Principale
 * ================================================== */

s32 principale (vacuum)
{
    Piscina* piscina_ctx;
    SilvaPiscina* piscina_silvae_ctx;
    SilvaContextus* ctx;

    piscina_ctx = piscina_generare_dynamicum("cursor_ctx", 8388608);
    piscina_silvae_ctx = silva_piscina_generare_dynamicum(
        "cursor_silva_ctx", 8388608);
    piscina_officinae = piscina_generare_dynamicum(
        "cursor_officina", 16777216);
    si (piscina_ctx == NIHIL || piscina_silvae_ctx == NIHIL
        || piscina_officinae == NIHIL)
    {
        fprintf(stderr, "cursor: piscina deest\n");
        redde I;
    }
    ctx = silva_contextus_creare(piscina_silvae_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "cursor: contextus deest\n");
        redde I;
    }

    moduli_mundi = xar_creare(piscina_ctx,
        (i32)magnitudo(MedullaModulus*));

    /* systema semel parsatum + lexicon (canalis M0b) */
    {
        FILE* pl_sys = fopen("silva/fontes/systema_c89.h", "rb");
        long mensura_sys;
        character* fons_sys;

        si (pl_sys == NIHIL)
        {
            fprintf(stderr, "cursor: systema_c89.h deest"
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
            fprintf(stderr, "cursor: systema non lectum\n");
            fclose(pl_sys);
            redde I;
        }
        fclose(pl_sys);
        si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
                fons_sys, (unsigned int)mensura_sys))
        {
            fprintf(stderr, "cursor: lexicon non additum\n");
            redde I;
        }
        systema_parsura = silva_c89_parsare(piscina_silvae_ctx,
            "systema_c89.h", fons_sys, (unsigned int)mensura_sys,
            NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr, "cursor: systema non parsatum\n");
            redde I;
        }
        systema_semantica = silva_c89_semantica_analysare(
            piscina_silvae_ctx, systema_parsura);
        si (systema_semantica == NIHIL)
        {
            fprintf(stderr, "cursor: systema non analysatum\n");
            redde I;
        }
    }

    /* praepassus capitum (inclusio vera) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL)
        {
            fprintf(stderr, "cursor: tabula deest\n");
            redde I;
        }
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    /* mundus: plagulae .c sub lib/ (semel, cachatae) */
    _bibliothecam_demittere(ctx);
    fprintf(stderr, "mundus:      %ld moduli | %ld functiones |"
        " %ld instructiones (fractae %ld, ruinae %ld)\n",
        (long)xar_numerus(moduli_mundi), summa_functionum,
        summa_instructionum, parsurae_fractae, ruinae_demissionis);

    /* UNDA 0: omnes suites praeter exclusiones nominatas */
    {
        constans character* exclusiones[] = {
            /* Unda 1 filesystem */
            "probatio_arbor2_file_roundtrip.c", "probatio_arbor2_expandere.c",
            "probatio_arbor.c", "probatio_arbor_formator.c",
            "probatio_arbor_index.c", "probatio_entitas_repositorium.c",
            "probatio_filum.c", "probatio_iter_directoria.c",
            "probatio_macho.c", "probatio_generare.c",
            "probatio_eventus_inspector.c",
            /* Unda 1b/2/3 */
            "probatio_uuid.c", "probatio_tempestivum.c",
            "probatio_actor.c", "probatio_actor_integratio.c",
            "probatio_tcp.c", "probatio_tcp_servus.c",
            "probatio_tls.c", "probatio_reactor.c",
            "probatio_http.c",
            /* GUI/asset/benchmark */
            "probatio_fenestra.c", "probatio_delineare.c",
            "probatio_tempus.c", "probatio_pagina.c",
            "probatio_navigator.c", "probatio_combinado.c",
            "probatio_gradientum.c", "probatio_capsula_caudae.c",
            "probatio_elementa.c", "probatio_imago.c",
            "probatio_dithering.c", "probatio_flatura_benchmark.c",
            "probatio_biblia_dr.c", "probatio_biblia_visus.c",
            NIHIL
        };
        DIR* dir = opendir("probationes");
        structura dirent* introitus;
        character* suites[256];
        integer numerus_suitarum = 0;
        integer i;
        long praeteritae = 0L;
        long exitus_mali = 0L;
        long sisterae = 0L;
        long decipulae_l = 0L;
        long vitia = 0L;
        long nexus_fracti = 0L;

        si (dir == NIHIL)
        {
            fprintf(stderr, "cursor: probationes/ deest\n");
            redde I;
        }
        dum ((introitus = readdir(dir)) != NIHIL
            && numerus_suitarum < 256)
        {
            b32 exclusa = FALSUM;

            si (strncmp(introitus->d_name, "probatio_", IX)
                != ZEPHYRUM)
            {
                perge;
            }
            per (i = 0; exclusiones[i] != NIHIL; i++)
            {
                si (strcmp(introitus->d_name, exclusiones[i])
                    == ZEPHYRUM)
                {
                    exclusa = VERUM;
                    frange;
                }
            }
            si (exclusa)
            {
                perge;
            }
            {
                memoriae_index ml = strlen(introitus->d_name);
                character* via = piscina_allocare(piscina_officinae,
                    ml + XVI);

                sprintf(via, "probationes/%s", introitus->d_name);
                suites[numerus_suitarum] = via;
                numerus_suitarum++;
            }
        }
        closedir(dir);
        qsort(suites, (memoriae_index)numerus_suitarum,
            magnitudo(character*), _comparare_vias);

        fprintf(stderr, "\n=== CURSOR: UNDA 0 (%ld suites) ===\n",
            (long)numerus_suitarum);
        per (i = 0; i < numerus_suitarum; i++)
        {
            MedullaModulus* modulus_suitae =
                _plagulam_demittere(ctx, suites[i]);
            Regio* regio;
            Conexio* conexio;
            Machinula* machinula;
            MachinulaExitus fructus;
            clock_t s0;
            clock_t s1;
            i32 k;
            b32 nexus_bene = VERUM;

            si (modulus_suitae == NIHIL)
            {
                fprintf(stderr, "%-44s RUINA (demissio)\n",
                    suites[i]);
                vitia++;
                perge;
            }
            regio = regio_generare(piscina_ctx);
            conexio = conexio_creare(piscina_ctx, regio);
            per (k = ZEPHYRUM; k < xar_numerus(moduli_mundi); k++)
            {
                si (!conexio_modulum_addere(conexio,
                        *(MedullaModulus**)xar_obtinere(
                            moduli_mundi, k)))
                {
                    nexus_bene = FALSUM;
                }
            }
            si (!conexio_modulum_addere(conexio, modulus_suitae)
                || !nexus_bene || !conexio_nectere(conexio))
            {
                constans chorda* symbolum =
                    conexio_querela_symbolum(conexio);

                fprintf(stderr, "%-44s NEXUS FRACTUS (%.*s)\n",
                    suites[i], (int)symbolum->mensura,
                    (constans character*)symbolum->datum);
                nexus_fracti++;
                regio_destruere(regio);
                perge;
            }
            machinula = machinula_creare(piscina_ctx, conexio,
                regio);
            s0 = clock();
            {
                chorda titulus_main;
                unio { constans character* c; i8* mm; } um;

                um.c = "main";
                titulus_main.datum = um.mm;
                titulus_main.mensura = IV;
                fructus = machinula_currere(machinula,
                    titulus_main);
            }
            s1 = clock();
            fflush(stdout);
            {
                duplex ms = (duplex)(s1 - s0) * 1000.0
                    / (duplex)CLOCKS_PER_SEC;

                si (fructus.genus == MACHINULA_BENE
                    && fructus.codex == ZEPHYRUM)
                {
                    fprintf(stderr, "%-44s PRAETERIIT  %8.0f ms"
                        "  %llu instr\n", suites[i], ms,
                        (insignatus longus longus)
                        machinula_numerus_instructionum(machinula));
                    praeteritae++;
                }
                alioquin si (fructus.genus == MACHINULA_BENE)
                {
                    fprintf(stderr, "%-44s EXITUS %ld  %8.0f ms\n",
                        suites[i], (long)fructus.codex, ms);
                    exitus_mali++;
                }
                alioquin
                {
                    constans character* genera[] = { "BENE",
                        "SISTERE", "DECIPULA", "VITIUM" };

                    fprintf(stderr, "%-44s %s (%.*s)  %8.0f ms\n",
                        suites[i], genera[fructus.genus],
                        (int)fructus.nuntius.mensura,
                        (constans character*)fructus.nuntius.datum,
                        ms);
                    si (fructus.genus == MACHINULA_SISTERE)
                    {
                        sisterae++;
                    }
                    alioquin si (fructus.genus == MACHINULA_DECIPULA)
                    {
                        decipulae_l++;
                    }
                    alioquin
                    {
                        vitia++;
                    }
                }
            }
            regio_destruere(regio);
        }
        fprintf(stderr, "\n=== TABULA: %ld praeteritae | %ld exitus"
            " mali | %ld sisterae | %ld decipulae | %ld vitia |"
            " %ld nexus fracti / %ld suites ===\n", praeteritae,
            exitus_mali, sisterae, decipulae_l, vitia, nexus_fracti,
            (long)numerus_suitarum);
        redde (praeteritae == (long)numerus_suitarum)
            ? ZEPHYRUM : I;
    }
}
