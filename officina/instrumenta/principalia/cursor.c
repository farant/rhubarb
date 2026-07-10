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
hic_manens Conexio* conexio_mundi = NIHIL;

/* ==================================================
 * Plagulam demittere (fistula fusoris; modulus CACHATUR)
 * ================================================== */

hic_manens vacuum
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
    si (mensura_maxima > ZEPHYRUM && mensura > (i32)mensura_maxima)
    {
        fprintf(stderr, "[PRAETERMISSA mensura] %s\n", via);
        fclose(pl);
        redde;
    }

    piscina_arboris = silva_piscina_generare_dynamicum(
        "cursor_arbor", 8388608);
    si (piscina_arboris == NIHIL)
    {
        fclose(pl);
        redde;
    }
    fons = (i8*)piscina_allocare(piscina_officinae,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        redde;
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
        redde;
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
        redde;
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
        redde;
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

    si (!conexio_modulum_addere(conexio_mundi, modulus))
    {
        constans chorda* querela = conexio_querela(conexio_mundi);
        constans chorda* symbolum =
            conexio_querela_symbolum(conexio_mundi);

        fprintf(stderr, "[CONEXIO FRACTA] %s: %.*s (%.*s)\n", via,
            (int)querela->mensura,
            (constans character*)querela->datum,
            (int)symbolum->mensura,
            (constans character*)symbolum->datum);
        ruinae_demissionis++;
    }

    /* arbores numquam cumulantur; origo pendens - clavis opaca
     * solum, numquam dereferenda (distillatio linearum M2b) */
    silva_piscina_destruere(piscina_arboris);
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
        _plagulam_demittere(ctx, viae[i]);
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
    Regio* regio;
    clock_t c0;
    clock_t c1;
    duplex ms_nexus;

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

    regio = regio_generare(piscina_ctx);
    si (regio == NIHIL)
    {
        fprintf(stderr, "cursor: regio deest (basis fixa non"
            " data)\n");
        redde I;
    }
    conexio_mundi = conexio_creare(piscina_ctx, regio);
    si (conexio_mundi == NIHIL)
    {
        fprintf(stderr, "cursor: conexio deest\n");
        redde I;
    }

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

    /* mundus: plagulae .c sub lib/ + modulus probationis */
    _bibliothecam_demittere(ctx);
    _plagulam_demittere(ctx, "probationes/probatio_piscina.c");

    /* NEXUS MUNDI */
    c0 = clock();
    si (!conexio_nectere(conexio_mundi))
    {
        constans chorda* querela = conexio_querela(conexio_mundi);

        fprintf(stderr, "cursor: nexus fractus: %.*s\n",
            (int)querela->mensura,
            (constans character*)querela->datum);
        redde I;
    }
    c1 = clock();
    ms_nexus = (duplex)(c1 - c0) * 1000.0 / (duplex)CLOCKS_PER_SEC;

    fprintf(stderr, "\n=== CURSOR: NEXUS MUNDI (M2a) ===\n");
    fprintf(stderr, "plagulae:    %ld (fractae %ld, ruinae %ld)\n",
        plagulae, parsurae_fractae, ruinae_demissionis);
    fprintf(stderr, "functiones:  %ld   data: %ld   instructiones: %ld\n",
        summa_functionum, summa_datorum, summa_instructionum);
    fprintf(stderr, "symbola:     %ld globalia\n",
        (long)conexio_numerus_symbolorum(conexio_mundi));
    fprintf(stderr, "nexae:       %ld functiones | %ld data | %ld"
        " cellae\n",
        (long)conexio_numerus_functionum(conexio_mundi),
        (long)conexio_numerus_datorum(conexio_mundi),
        (long)conexio_numerus_cellarum(conexio_mundi));
    fprintf(stderr, "globalia:    %.1f MB collocata\n",
        (duplex)regio_globalia_usus(regio) / 1048576.0);
    fprintf(stderr, "tempus:      arbor %.0f ms | demissio %.0f ms |"
        " nexus %.0f ms\n", ms_arboris, ms_demissionis, ms_nexus);

    /* decipulae: numeratae + NOMINATAE (vectis M2a) */
    {
        i32 numerus = conexio_numerus_decipularum(conexio_mundi);
        i32 i;

        fprintf(stderr, "decipulae:   %ld (ruunt solum si vocatae)\n",
            (long)numerus);
        per (i = ZEPHYRUM; i < numerus; i += I)
        {
            constans chorda* titulus = conexio_decipulam_obtinere(
                conexio_mundi, (s32)i);

            fprintf(stderr, "    %.*s\n", (int)titulus->mensura,
                (constans character*)titulus->datum);
        }
    }

    /* $main inventum (probatio_piscina) */
    {
        chorda titulus_main;
        unio { constans character* c; i8* m; } u;
        vacuum* sedes;

        u.c = "main";
        titulus_main.datum = u.m;
        titulus_main.mensura = IV;
        sedes = conexio_sedes_quaerere(conexio_mundi, titulus_main);
        si (sedes == NIHIL)
        {
            fprintf(stderr, "cursor: $main NON inventum\n");
            redde I;
        }
        fprintf(stderr, "$main:       inventum (descriptor %p)\n", sedes);

        si (parsurae_fractae > 0L || ruinae_demissionis > 0L)
        {
            redde I;
        }

        /* M2b: EXSECUTIO - machinula $main currit; stdout =
         * exitus programmatis PURUS (vectis diff) */
        {
            Machinula* machinula = machinula_creare(piscina_ctx,
                conexio_mundi, regio);
            MachinulaExitus fructus_exsecutionis;
            duplex ms_exsecutionis;

            si (machinula == NIHIL)
            {
                fprintf(stderr, "cursor: machinula deest\n");
                redde I;
            }
            c0 = clock();
            fructus_exsecutionis = machinula_currere(machinula,
                titulus_main);
            c1 = clock();
            ms_exsecutionis = (duplex)(c1 - c0) * 1000.0
                / (duplex)CLOCKS_PER_SEC;
            fflush(stdout);
            fprintf(stderr, "exsecutio:   genus %d, codex %ld,"
                " %.0f ms\n", (int)fructus_exsecutionis.genus,
                (long)fructus_exsecutionis.codex, ms_exsecutionis);
            fprintf(stderr, "machinula:   %llu instructiones |"
                " %llu vocationes | %llu aedificata | apex stivae"
                " %.1f KB\n",
                (insignatus longus longus)
                    machinula_numerus_instructionum(machinula),
                (insignatus longus longus)
                    machinula_numerus_vocationum(machinula),
                (insignatus longus longus)
                    machinula_numerus_aedificatorum(machinula),
                (duplex)machinula_stiva_apex(machinula) / 1024.0);
            si (fructus_exsecutionis.genus != MACHINULA_BENE)
            {
                redde I;
            }
            redde (s32)fructus_exsecutionis.codex;
        }
    }
}
