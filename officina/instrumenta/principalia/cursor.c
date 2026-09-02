/* cursor.c - Cursor corporis: instrumentum vectis corporis (M2a v0)
 *
 * M2a: NEXUS MUNDI solum - omnes plagulae plagulae .c sub lib/ demittuntur SEMEL
 * (piscina officinae communis - moduli cachati), tum + modulus
 * probationis (probatio_piscina.c) in UNAM conexionem nectuntur.
 * Vectis: 0 fracturae oneris; decipulae numeratae + NOMINATAE;
 * $main inventum; relocationes sarcitae.
 *
 * M2c (v2.3): exsecutio per machinulam fork-per-suite, tabula
 * classificationis, tempus per suitem, -custodia/-mora/-sola,
 * -enumerare (captare.sh consumit), columna stdout VIVA: pullus
 * effusionem in officina/build/capturae/ scribit, parens post
 * waitpid contra aureum fixum (fixa/stdout_nativa/, captare.sh
 * figit) octetim comparat - exclusiones nominatae supra. Vectis:
 * exitus != 0 si quaevis effusio DIFFERT.
 *
 * Fistula bis-analysans ex fusore exscripta (superficies publica
 * silva.h). Piscinae arborum per TU destructae (arbores numquam
 * cumulantur - responsum apici 9.7GB); piscina officinae UNA manet
 * (moduli vivunt trans nexum).
 *
 * Usus: ./officina/cursor.sh   (ex radice repositorii)
 */

#include "postulata_posix.h"
#include "officina_demissio.h"
#include "officina_conexio.h"
#include "officina_machinula.h"
#include "officina_medulla_textus.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>


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
hic_manens Xar* lineae_mundi = NIHIL;            /* MedullaLineae*
                                                  * parallelae */

hic_manens b32 custodia_petita = FALSUM;         /* -custodia */
hic_manens insignatus integer mora_secunda = 30; /* -mora <n> */
hic_manens constans character* sola_quaesita = NIHIL; /* -sola
    <substr>: suites congruentes solae + stdout interpretatum
    VISIBILE (diagnosis per-suite) */
hic_manens b32 enumerare_petita = FALSUM;        /* -enumerare:
    vias suitarum undae imprimere et exire (captare.sh consumit -
    UNUS fons veritatis exclusionum) */
hic_manens b32 census_petita = FALSUM;           /* -census:
    numeri_op per suitem in build/census/ (datum quaestionis
    dispatch parcatae - superinstructiones ex mensura, non fide) */

/* UNDAE 0+1: exclusiones nominatae (M2d Chunk C - Unda 1
 * filesystem INTRAVIT; quaeque exclusio causam habet) */
hic_manens constans character* exclusiones_undae[] = {
    /* Unda 1b nomine: system() + opendir/readdir */
    "probatio_iter_directoria.c", "probatio_generare.c",
    /* Unda 2 nomine: tempus/entropia */
    "probatio_uuid.c", "probatio_tempestivum.c",
    "probatio_actor.c", "probatio_actor_integratio.c",
    /* Unda 3 nomine: rete vivum */
    "probatio_tcp.c", "probatio_tcp_servus.c",
    "probatio_tls.c", "probatio_reactor.c",
    "probatio_http.c",
    /* Tier-2 .m: cursus vocat fenestra_tempus_* (stratum
     * machinae extra corpus per DECISUM) */
    "probatio_cursus.c",
    /* gravia: capsula_libri.c 12.8MB super tectum 4MB
     * (ordo nominatus cum biblia_dr) */
    "probatio_libri.c",
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

/* columna stdout: exclusiones nominatae (effusio non determinata -
 * captare.sh per cursum duplicem invenit; causa recordata) */
nomen structura {
    constans character* titulus;
    constans character* causa;
} ExclusioStdout;

hic_manens constans ExclusioStdout exclusiones_stdout[] = {
    /* inventae per captare.sh cursu duplici 2026-07-10 */
    { "probatio_arbor2_glr.c",      "monstratores DEBUG (ASLR)" },
    { "probatio_arbor2_scribere.c", "monstratores DEBUG (ASLR)" },
    { "probatio_arbor2_file_roundtrip.c",
                                    "monstratores DEBUG (ASLR)" },
    { "probatio_compound.c",        "monstratores DEBUG (ASLR)" },
    { "probatio_commandi.c",        "purgamentum pre-init impressum" },
    { "probatio_entitas.c",         "UUIDv7 (tempus+fors)" },
    /* inventa per columnam ipsam (cursu duplici latuit - secundum
     * idem; captare.sh nunc 1s inter cursus dormit) */
    { "probatio_multipart.c",       "limes ex semine time(NULL)" },
    { NIHIL, NIHIL }
};

hic_manens constans character*
_exclusio_stdout_causa (
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; exclusiones_stdout[i].titulus != NIHIL; i++)
    {
        si (strcmp(titulus, exclusiones_stdout[i].titulus)
            == ZEPHYRUM)
        {
            redde exclusiones_stdout[i].causa;
        }
    }
    redde NIHIL;
}

/* comparatio plagularum octetim (aureum vs captura) */
hic_manens b32
_plagulae_aequae (
    constans character* via_a,
    constans character* via_b)
{
    FILE* pa      = fopen(via_a, "rb");
    FILE* pb      = fopen(via_b, "rb");
     b32  aequae  = VERUM;

    si (pa == NIHIL || pb == NIHIL)
    {
        si (pa != NIHIL) fclose(pa);
        si (pb != NIHIL) fclose(pb);
        redde FALSUM;
    }
    dum (VERUM)
    {
             character ba[4096];
             character bb[4096];
        memoriae_index na = fread(ba, I, magnitudo(ba), pa);
        memoriae_index nb = fread(bb, I, magnitudo(bb), pb);

        si (   na != nb || (na > ZEPHYRUM
            && memcmp(ba, bb, na) != ZEPHYRUM))
        {
            aequae = FALSUM;
            frange;
        }
        si (na < magnitudo(ba))
        {
            frange;
        }
    }
    fclose(pa);
    fclose(pb);
    redde aequae;
}

/* viae capturae/aurei ex via suitae ("probationes/probatio_x.c") */
hic_manens vacuum
_vias_effusionis (
    constans character* via_suitae,
             character* via_capturae,
             character* via_aurei)
{
    constans character* titulus  = via_suitae + XII;
    integer mensura              = (integer)strlen(titulus) - II; /* sine ".c" */

    sprintf(via_capturae, "officina/build/capturae/%.*s.stdout",
        mensura, titulus);
    sprintf(via_aurei,
        "officina/probationes/fixa/stdout_nativa/%.*s.stdout",
        mensura, titulus);
}

/* comparator qsort pro ordinibus character[128] (-enumerare) */
hic_manens integer
_comparare_nomina (
    constans vacuum* a,
    constans vacuum* b)
{
    redde strcmp((constans character*)a, (constans character*)b);
}

/* plagulam systematis in piscinam legere */
hic_manens character*
_systema_legere (
               Piscina* piscina,
    constans character* via,
                  long* mensura_fructus)
{
         FILE* pl = fopen(via, "rb");
         long  mensura;
    character* fons;

    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (   fons == NIHIL
        || fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_fructus = mensura;
    redde fons;
}


/* ==================================================
 * Plagulam demittere (fistula fusoris; modulus CACHATUR)
 * ================================================== */

hic_manens MedullaModulus*
_plagulam_demittere (
    constans SilvaContextus*  ctx,
         constans character*  via,
              MedullaLineae** lineae_exitus)
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

    si (lineae_exitus != NIHIL)
    {
        *lineae_exitus = NIHIL;
    }
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
    si (   fons == NIHIL || (mensura > ZEPHYRUM
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
    c0   = clock();
    u.m  = fons;
    parsura = silva_c89_parsare_cum_contextu(piscina_arboris, ctx,
        via, u.c, (unsigned int)mensura, oraculum);
    si (   parsura == NIHIL || !parsura->successus
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

    u.c                     = via;
    titulus_moduli.datum    = u.m;
    titulus_moduli.mensura  = (i32)strlen(via);
    c0                      = clock();
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

    summa_functionum  += (long)xar_numerus(modulus->functiones);
    summa_datorum     += (long)xar_numerus(modulus->data);
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

    /* collige-ante-destrue (M3): catena originis solum dum parsura
     * vivit ambulabilis; lineae in piscinam officinae (vita moduli,
     * COW trans fork) */
    si (lineae_exitus != NIHIL)
    {
        *lineae_exitus = demissio_lineas_colligere(
            piscina_officinae, modulus, parsura);
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
_praetermittendum (
    constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

hic_manens vacuum
_caput_praebere (
        SilvaContextus* ctx,
               Piscina* piscina,
        TabulaDispersa* visa,
    constans character* via,
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
    si (   textus == NIHIL || (mensura > ZEPHYRUM
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
_capita_praeparare (
        SilvaContextus* ctx,
               Piscina* piscina,
        TabulaDispersa* visa,
    constans character* via)
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
            si (   m >= III && introitus->d_name[m - II] == '.'
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
_comparare_vias (
    constans vacuum* a,
    constans vacuum* b)
{
    redde strcmp(*(character* constans*)a, *(character* constans*)b);
}

hic_manens vacuum
_bibliothecam_demittere (
    constans SilvaContextus* ctx)
{
    DIR* dir = opendir("lib");
    structura dirent* introitus;
    character* viae[512];
      integer  numerus = 0;
      integer  i;

    si (dir == NIHIL)
    {
        fprintf(stderr, "cursor: lib/ deest (curre ex radice)\n");
        redde;
    }
    dum ((introitus = readdir(dir)) != NIHIL && numerus < 512)
    {
        memoriae_index m = strlen(introitus->d_name);

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (   m >= III && introitus->d_name[m - II] == '.'
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
         MedullaLineae* lineae = NIHIL;
        MedullaModulus* modulus = _plagulam_demittere(ctx, viae[i],
            &lineae);

        si (modulus != NIHIL)
        {
            MedullaModulus** locellus    = xar_addere(moduli_mundi);
             MedullaLineae** locellus_l  = xar_addere(lineae_mundi);

            *locellus    = modulus;
            *locellus_l  = lineae;
        }
    }
}


/* ==================================================
 * Principale
 * ================================================== */

s32
principale (
      integer   argc,
    character** argv)
{
    Piscina* piscina_ctx;
    SilvaPiscina* piscina_silvae_ctx;
    SilvaContextus* ctx;
    integer a;

    per (a = I; a < argc; a++)
    {
        si (strcmp(argv[a], "-custodia") == ZEPHYRUM)
        {
            custodia_petita = VERUM;
        }
        alioquin si (   strcmp(argv[a], "-mora") == ZEPHYRUM
                     && a + I < argc)
        {
            a++;
            mora_secunda = (insignatus integer)atoi(argv[a]);
        }
        alioquin si (   strcmp(argv[a], "-sola") == ZEPHYRUM
                     && a + I < argc)
        {
            a++;
            sola_quaesita = argv[a];
        }
        alioquin si (strcmp(argv[a], "-enumerare") == ZEPHYRUM)
        {
            enumerare_petita = VERUM;
        }
        alioquin si (strcmp(argv[a], "-census") == ZEPHYRUM)
        {
            census_petita = VERUM;
        }
    }

    /* -enumerare: vias undae imprimere et exire (sine mundo) */
    si (enumerare_petita)
    {
        DIR* dir = opendir("probationes");
        structura dirent* introitus;
        hic_manens character vias[256][128];
                     integer numerus = 0;
                     integer i;

        si (dir == NIHIL)
        {
            fprintf(stderr, "cursor: probationes/ deest\n");
            redde I;
        }
        dum ((introitus = readdir(dir)) != NIHIL && numerus < 256)
        {
                       b32 exclusa  = FALSUM;
            memoriae_index md       = strlen(introitus->d_name);

            si (strncmp(introitus->d_name, "probatio_", IX)
                != ZEPHYRUM)
            {
                perge;
            }
            si (   md < III || md > CX
                || introitus->d_name[md - II] != '.'
                || introitus->d_name[md - I]  != 'c')
            {
                perge;
            }
            per (i = 0; exclusiones_undae[i] != NIHIL; i++)
            {
                si (strcmp(introitus->d_name, exclusiones_undae[i])
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
            sprintf(vias[numerus], "probationes/%s",
                introitus->d_name);
            numerus++;
        }
        closedir(dir);
        qsort(vias, (memoriae_index)numerus, magnitudo(vias[0]),
            _comparare_nomina);
        per (i = 0; i < numerus; i++)
        {
            imprimere("%s\n", vias[i]);
        }
        redde ZEPHYRUM;
    }

    piscina_ctx = piscina_generare_dynamicum("cursor_ctx", 8388608);
    piscina_silvae_ctx = silva_piscina_generare_dynamicum(
        "cursor_silva_ctx", 8388608);
    piscina_officinae = piscina_generare_dynamicum(
        "cursor_officina", 16777216);
    si (   piscina_ctx       == NIHIL || piscina_silvae_ctx == NIHIL
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
    lineae_mundi = xar_creare(piscina_ctx,
        (i32)magnitudo(MedullaLineae*));

    /* systema semel parsatum + lexicon (canalis M0b) - textus
     * CONCATENATUS: plagula ISO + supplementum POSIX (M2d; formae
     * certificatae per auspex_posix.sh - oneratores officinae
     * soli concatenant, silva-latus plagulam ISO solam videt) */
    {
             long  m_iso    = 0L;
             long  m_posix  = 0L;
        character* fons_iso = _systema_legere(piscina_ctx,
            "silva/fontes/systema_c89.h", &m_iso);
        character* fons_posix = _systema_legere(piscina_ctx,
            "silva/fontes/systema_posix.h", &m_posix);
        character* fons_sys;
             long  mensura_sys;

        si (fons_iso == NIHIL || fons_posix == NIHIL)
        {
            fprintf(stderr, "cursor: systema deest (curre ex"
                " radice repositorii)\n");
            redde I;
        }
        mensura_sys = m_iso + 1L + m_posix;
        fons_sys = (character*)piscina_allocare(piscina_ctx,
            (memoriae_index)(mensura_sys + 1L));
        si (fons_sys == NIHIL)
        {
            fprintf(stderr, "cursor: systema non lectum\n");
            redde I;
        }
        memcpy(fons_sys, fons_iso, (memoriae_index)m_iso);
        fons_sys[m_iso] = '\n';
        memcpy(fons_sys + m_iso + 1L, fons_posix,
            (memoriae_index)m_posix);
        si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
                fons_sys, (unsigned int)mensura_sys))
        {
            fprintf(stderr, "cursor: lexicon non additum\n");
            redde I;
        }
        systema_parsura = silva_c89_parsare(piscina_silvae_ctx,
            "systema_c89.h", fons_sys, (unsigned int)mensura_sys,
            NIHIL);
        si (   systema_parsura == NIHIL
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

    /* comites probationum: plagulae .c sub probationes/ quae NON
     * probatio_* sunt (capsula_assets, persona_gen - data suitarum;
     * compile_tests.sh eas nominatim nectit - mundus noster quoque.
     * Sine his symbola eorum decipulae fiunt et LECTIO datorum
     * octetos descriptoris tacite servit - parca M2b in agro:
     * capsula/libri "monstrator humilis") */
    {
        DIR* dir_c = opendir("probationes");
        structura dirent* intro_c;

        si (dir_c != NIHIL)
        {
            dum ((intro_c = readdir(dir_c)) != NIHIL)
            {
                memoriae_index mc = strlen(intro_c->d_name);

                si (intro_c->d_name[ZEPHYRUM] == '.') perge;
                si (strncmp(intro_c->d_name, "probatio_", IX)
                    == ZEPHYRUM)
                {
                    perge;
                }
                si (   mc < III || intro_c->d_name[mc - II] != '.'
                    || intro_c->d_name[mc - I] != 'c')
                {
                    perge;
                }
                {
                         character  via_c[CCLVI];
                    MedullaModulus* modulus_c;
                     MedullaLineae* lineae_c = NIHIL;

                    sprintf(via_c, "probationes/%s",
                        intro_c->d_name);
                    modulus_c = _plagulam_demittere(ctx, via_c,
                        &lineae_c);
                    si (modulus_c != NIHIL)
                    {
                        MedullaModulus** locellus_c =
                            xar_addere(moduli_mundi);
                        MedullaLineae** locellus_cl =
                            xar_addere(lineae_mundi);

                        *locellus_c   = modulus_c;
                        *locellus_cl  = lineae_c;
                    }
                }
            }
            closedir(dir_c);
        }
    }
    fprintf(stderr, "mundus:      %ld moduli | %ld functiones |"
        " %ld instructiones (fractae %ld, ruinae %ld)\n",
        (long)xar_numerus(moduli_mundi), summa_functionum,
        summa_instructionum, parsurae_fractae, ruinae_demissionis);

    /* UNDA 0: omnes suites praeter exclusiones nominatas
     * (exclusiones_undae - tabula unica, -enumerare eadem) */
    {
        DIR* dir = opendir("probationes");
        structura dirent* introitus;
        character* suites[256];
          integer  numerus_suitarum = 0;
          integer  i;
             long  praeteritae             = 0L;
             long  exitus_mali             = 0L;
             long  sisterae                = 0L;
             long  decipulae_l             = 0L;
             long  vitia                   = 0L;
             long  nexus_fracti            = 0L;
             long  ruinae                  = 0L;
             long  tempora                 = 0L;
             long  effusiones_eaedem       = 0L;
             long  effusiones_differentes  = 0L;
             long  effusiones_sine_aureo   = 0L;
             long  effusiones_exclusae     = 0L;

        /* directorium capturarum (stdout interpretatum per suitem) */
        (vacuum)mkdir("officina/build/capturae", 0755);
        si (census_petita)
        {
            (vacuum)mkdir("officina/build/census", 0755);
        }

        si (dir == NIHIL)
        {
            fprintf(stderr, "cursor: probationes/ deest\n");
            redde I;
        }
        dum (   (introitus = readdir(dir)) != NIHIL
             && numerus_suitarum < 256)
        {
            b32 exclusa = FALSUM;

            si (strncmp(introitus->d_name, "probatio_", IX)
                != ZEPHYRUM)
            {
                perge;
            }
            {
                memoriae_index md = strlen(introitus->d_name);

                si (   md < III || introitus->d_name[md - II] != '.'
                    || introitus->d_name[md - I] != 'c')
                {
                    perge;
                }
            }
            per (i = 0; exclusiones_undae[i] != NIHIL; i++)
            {
                si (strcmp(introitus->d_name, exclusiones_undae[i])
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
            si (   sola_quaesita != NIHIL
                && strstr(introitus->d_name, sola_quaesita)
                    == NIHIL)
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
             MedullaLineae* lineae_suitae = NIHIL;
            MedullaModulus* modulus_suitae =
                _plagulam_demittere(ctx, suites[i],
                    &lineae_suitae);
              pid_t pullus;
            integer status;

            si (modulus_suitae == NIHIL)
            {
                fprintf(stderr, "%-44s RUINA (demissio)\n",
                    suites[i]);
                vitia++;
                perge;
            }

            /* pullus per suitem: ruina hospitis (segfault in
             * aedificato etc.) = versus RUINA, non mors sweep;
             * memoria pulli cum pullo perit (piscinae mundi COW) */
            pullus = fork();
            si (pullus == ZEPHYRUM)
            {
                          Regio* regio;
                        Conexio* conexio;
                      Machinula* machinula;
                MachinulaExitus  fructus;
                        clock_t  s0;
                        clock_t  s1;
                            i32  k;
                            b32  nexus_bene = VERUM;

                /* stdout interpretatum in capturam (columna diff
                 * in parente post exitum) - PRAETER -sola
                 * (diagnosis: asserta credo videnda) */
                si (sola_quaesita == NIHIL)
                {
                    character via_capturae[CCLVI];
                    character via_aurei[CCLVI];

                    _vias_effusionis(suites[i], via_capturae,
                        via_aurei);
                    si (freopen(via_capturae, "w", stdout) == NIHIL)
                    {
                        (vacuum)freopen("/dev/null", "w", stdout);
                    }
                }

                /* tectum temporis: SIGALRM = versus TEMPUS in
                 * parente (lapifex interpretatus minuta ardet) */
                si (mora_secunda > 0U)
                {
                    (vacuum)alarm(mora_secunda);
                }

                regio = regio_generare(piscina_ctx);
                si (custodia_petita)
                {
                    regio_custodiam_ponere(regio, VERUM);
                }
                conexio = conexio_creare(piscina_ctx, regio);
                per (k = ZEPHYRUM; k < xar_numerus(moduli_mundi);
                    k++)
                {
                    si (!conexio_modulum_addere(conexio,
                            *(MedullaModulus**)xar_obtinere(
                                moduli_mundi, k)))
                    {
                        nexus_bene = FALSUM;
                    }
                }
                si (   !conexio_modulum_addere(conexio,
                        modulus_suitae)
                    || !nexus_bene || !conexio_nectere(conexio))
                {
                    constans chorda* symbolum =
                        conexio_querela_symbolum(conexio);

                    fprintf(stderr, "%-44s NEXUS FRACTUS (%.*s)",
                        suites[i], (int)symbolum->mensura,
                        (constans character*)symbolum->datum);
                    _exit(44);
                }
                machinula = machinula_creare(piscina_ctx, conexio,
                    regio);
                /* lineae semper (M3 DECISUS 9): mundus + suita
                 * (index suitae = post mundum, ordo additionis) */
                per (k = ZEPHYRUM; k < xar_numerus(lineae_mundi);
                    k++)
                {
                    MedullaLineae* lineae_k = *(MedullaLineae**)
                        xar_obtinere(lineae_mundi, k);

                    si (lineae_k != NIHIL)
                    {
                        machinula_lineas_praebere(machinula,
                            (s32)k, lineae_k);
                    }
                }
                si (lineae_suitae != NIHIL)
                {
                    machinula_lineas_praebere(machinula,
                        (s32)xar_numerus(moduli_mundi),
                        lineae_suitae);
                }
                s0 = clock();
                {
                    chorda titulus_main;
                    unio { constans character* c; i8* mm; } um;

                    um.c                  = "main";
                    titulus_main.datum    = um.mm;
                    titulus_main.mensura  = IV;
                    fructus = machinula_currere(machinula,
                        titulus_main);
                }
                s1 = clock();
                (vacuum)alarm(0U);   /* versus sine SIGALRM medio */
                /* _exit stdio NON effundit - stdout interpretatum
                 * expresse effundendum (captura/-sola) */
                fflush(stdout);
                si (census_petita)
                {
                    character via_census[CCLVI];
                    FILE* pc;

                    sprintf(via_census,
                        "officina/build/census/%.*s.census",
                        (int)(strlen(suites[i] + XII) - II),
                        suites[i] + XII);
                    pc = fopen(via_census, "w");
                    si (pc != NIHIL)
                    {
                        s32 op_c;

                        per (op_c = ZEPHYRUM;
                            op_c < (s32)MEDULLA_OP_NUMERUS; op_c++)
                        {
                            i64 numerus = machinula_numerus_op(
                                machinula, op_c);

                            si (numerus > ZEPHYRUM)
                            {
                                fprintf(pc, "%-16s %llu\n",
                                    medulla_op_titulus(op_c),
                                    (insignatus longus longus)
                                        numerus);
                            }
                        }
                        fclose(pc);
                    }
                }
                {
                    duplex ms = (duplex)(s1 - s0) * 1000.0
                        / (duplex)CLOCKS_PER_SEC;

                    /* versus SINE novissima linea - parens columnam
                     * stdout appendit post waitpid */
                    si (   fructus.genus == MACHINULA_BENE
                        && fructus.codex == ZEPHYRUM)
                    {
                        fprintf(stderr, "%-44s PRAETERIIT"
                            "  %8.0f ms  %llu instr", suites[i],
                            ms, (insignatus longus longus)
                            machinula_numerus_instructionum(
                                machinula));
                        _exit(0);
                    }
                    alioquin si (fructus.genus == MACHINULA_BENE)
                    {
                        fprintf(stderr, "%-44s EXITUS %ld"
                            "  %8.0f ms", suites[i],
                            (long)fructus.codex, ms);
                        _exit(40);
                    }
                    alioquin
                    {
                        constans character* genera[] = { "BENE",
                            "SISTERE", "DECIPULA", "VITIUM",
                            "PAUSA", "RECUSATIO" };

                        fprintf(stderr, "%-44s %s (%.*s)"
                            "  %8.0f ms", suites[i],
                            genera[fructus.genus],
                            (int)fructus.nuntius.mensura,
                            (constans character*)
                                fructus.nuntius.datum, ms);
                        /* 41 sistere / 42 decipula / 43 vitium */
                        _exit(40 + (int)fructus.genus);
                    }
                }
            }
            /* PARENS: classis per codicem exitus pulli */
            si (pullus < ZEPHYRUM)
            {
                fprintf(stderr, "%-44s RUINA (fork fractum)\n",
                    suites[i]);
                vitia++;
                perge;
            }
            si (waitpid(pullus, &status, 0) < ZEPHYRUM)
            {
                fprintf(stderr, "%-44s RUINA (waitpid)\n",
                    suites[i]);
                vitia++;
                perge;
            }
            si (WIFSIGNALED(status))
            {
                si (WTERMSIG(status) == SIGALRM)
                {
                    fprintf(stderr, "%-44s TEMPUS (>%us)\n",
                        suites[i], mora_secunda);
                    tempora++;
                }
                alioquin
                {
                    fprintf(stderr, "%-44s RUINA (signum %d)\n",
                        suites[i], (int)WTERMSIG(status));
                    ruinae++;
                }
            }
            alioquin
            {
                integer codex_pulli = WEXITSTATUS(status);

                /* columna stdout (versus pulli sine novissima
                 * linea): praeteriit -> diff capturae vs aurei;
                 * ceteri -> linea sola finitur */
                si (   codex_pulli   == ZEPHYRUM
                    && sola_quaesita == NIHIL)
                {
                             character  via_capturae[CCLVI];
                             character  via_aurei[CCLVI];
                    constans character* causa;

                    _vias_effusionis(suites[i], via_capturae,
                        via_aurei);
                    causa = _exclusio_stdout_causa(
                        suites[i] + XII);
                    si (causa != NIHIL)
                    {
                        fprintf(stderr, "  exclusa (%s)\n", causa);
                        effusiones_exclusae++;
                    }
                    alioquin
                    {
                        FILE* pa = fopen(via_aurei, "rb");

                        si (pa == NIHIL)
                        {
                            fprintf(stderr, "  sine aureo\n");
                            effusiones_sine_aureo++;
                        }
                        alioquin
                        {
                            fclose(pa);
                            si (_plagulae_aequae(via_capturae,
                                    via_aurei))
                            {
                                fprintf(stderr, "  idem\n");
                                effusiones_eaedem++;
                            }
                            alioquin
                            {
                                fprintf(stderr, "  DIFFERT\n");
                                effusiones_differentes++;
                            }
                        }
                    }
                }
                alioquin si (   codex_pulli >= 40
                             && codex_pulli <= 44)
                {
                    fprintf(stderr, "\n");
                }
                alioquin si (codex_pulli == ZEPHYRUM)
                {
                    fprintf(stderr, "\n");   /* -sola: sine captura */
                }
                commutatio (codex_pulli)
                {
                    casus ZEPHYRUM: praeteritae++; frange;
                    casus 40: exitus_mali++; frange;
                    casus 41: sisterae++; frange;
                    casus 42: decipulae_l++; frange;
                    casus 43: vitia++; frange;
                    casus 44: nexus_fracti++; frange;
                    ordinarius:
                        fprintf(stderr, "%-44s RUINA (codex pulli"
                            " %d)\n", suites[i],
                            (int)codex_pulli);
                        ruinae++;
                        frange;
                }
            }
        }
        fprintf(stderr, "\n=== TABULA: %ld praeteritae | %ld exitus"
            " mali | %ld sisterae | %ld decipulae | %ld vitia |"
            " %ld ruinae | %ld tempora | %ld nexus fracti /"
            " %ld suites ===\n",
            praeteritae, exitus_mali, sisterae, decipulae_l, vitia,
            ruinae, tempora, nexus_fracti,
            (long)numerus_suitarum);
        si (sola_quaesita == NIHIL)
        {
            fprintf(stderr, "=== STDOUT: %ld eaedem |"
                " %ld DIFFERUNT | %ld sine aureo | %ld exclusae"
                " ===\n",
                effusiones_eaedem, effusiones_differentes,
                effusiones_sine_aureo, effusiones_exclusae);
        }
        /* vectis: virides omnes ET nulla effusio differens */
        redde (praeteritae == (long)numerus_suitarum
            && effusiones_differentes == 0L) ? ZEPHYRUM : I;
    }
}
