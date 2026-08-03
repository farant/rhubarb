/* probatio_officina_vindex.c - SIGILLUM FORMALE M3 (spec §VII,
 * DECISUS 10): punctum/gradus/inspectio in PROBATIONE CREDO VERA
 * (probatio_piscina) per PLAGULAM INDICII SOLAM.
 *
 * Circuitus: mundus scopo dato (piscina/chorda/chorda_aedificator/
 * credo + probationes/probatio_piscina.c) per viam oneris
 * interpretare; indicium collectum IN oneratore (disciplina C12),
 * scriptum, RECLUSUM - ab eo puncto quaestiones omnes per lectorem.
 * Phases: punctum per lineam RETRO (linea CREDO 37!) -> PAUSA situ
 * exacto + acies macronum + "usus" per nomen fontis == 0 + pactum
 * ambulationis; gradus-super-lineam -> linea 39; punctum
 * per-functionem trans-modulum (piscina_summa_usus) -> PAUSA
 * profunditate 2 + FINI; tollere -> BENE codex 0, suite interpretata
 * perfecta (effusio interior fluit - exemplar cursor -sola).
 *
 * Onerator = geminus tertius (interpretare/cursor) CONSULTO -
 * refactor oneratoris communis parcatum nominatim (tractio viva). */

#include "postulata_posix.h"
#include "officina_indicium.h"
#include "officina_demissio.h"
#include "officina_machinula.h"
#include "officina_regio.h"
#include "officina_medulla_textus.h"
#include "credo.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define VIA_SUITAE    "probationes/probatio_piscina.c"
#define LINEA_CREDO   37          /* CREDO_AEQUALIS_I32 ((i32)usus, */
#define LINEA_POST    39          /* piscina_destruere(p); */
#define CUSTOS_GRADUUM 200000L

hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;
hic_manens Piscina* piscina_officinae = NIHIL;
hic_manens Xar* moduli_mundi = NIHIL;
hic_manens Xar* lineae_mundi = NIHIL;
hic_manens IndiciumScriptor* scriptor_indicii = NIHIL;
hic_manens constans character* radix_repositorii = ".";

interior chorda
_ch (constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

interior character*
_plagulam_legere_cruda (Piscina* piscina, constans character* via,
    long* mensura_fructus)
{
    FILE* pl = fopen(via, "rb");
    long mensura;
    character* fons;

    si (pl == NIHIL) redde NIHIL;
    fseek(pl, 0L, SEEK_END);
    mensura = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (fons == NIHIL
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
 * capita praeparare (exemplar interpretare)
 * ================================================== */

interior b32
_praetermittendum (constans character* titulus)
{
    redde (strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "results") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior vacuum
_caput_praebere (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via,
    constans character* titulus)
{
    long mensura_l = 0L;
    character* textus;
    chorda clavis;

    clavis = chorda_ex_literis(titulus, piscina);
    si (tabula_dispersa_continet(visa, clavis)) redde;
    textus = _plagulam_legere_cruda(piscina, via, &mensura_l);
    si (textus == NIHIL) redde;
    si (silva_contextus_praebere(ctx, titulus, textus,
            (insignatus integer)mensura_l))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
    }
}

interior vacuum
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
 * demissio unius plagulae + collectio indicii (onerator vindicis:
 * via oneris interpretare + coctio in fenestra collige-ante-destrue)
 * ================================================== */

interior MedullaModulus*
_plagulam_demittere (constans SilvaContextus* ctx,
    constans character* via_parsurae)
{
    character via_lecta[CCLVI];
    long mensura_l = 0L;
    SilvaPiscina* piscina_arboris;
    character* fons;
    SilvaParsura* parsura;
    SilvaOraculum* oraculum;
    SilvaSemantica* sem;
    MedullaModulus* modulus;
    MedullaLineae* lineae = NIHIL;

    sprintf(via_lecta, "%s/%s", radix_repositorii, via_parsurae);
    piscina_arboris = silva_piscina_generare_dynamicum(
        "vindex_arbor", 8388608);
    si (piscina_arboris == NIHIL) redde NIHIL;
    fons = _plagulam_legere_cruda(piscina_officinae, via_lecta,
        &mensura_l);
    si (fons == NIHIL)
    {
        silva_piscina_destruere(piscina_arboris);
        redde NIHIL;
    }
    oraculum = silva_oraculum_creare(piscina_arboris);
    si (oraculum != NIHIL && systema_semantica != NIHIL)
    {
        (vacuum)silva_c89_semantica_oraculum_augere(
            systema_semantica, oraculum);
    }
    parsura = silva_c89_parsare_cum_contextu(piscina_arboris, ctx,
        via_parsurae, fons, (insignatus integer)mensura_l,
        oraculum);
    si (parsura == NIHIL || !parsura->successus
        || parsura->numerus_errorum > ZEPHYRUM)
    {
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
    si (sem == NIHIL)
    {
        silva_piscina_destruere(piscina_arboris);
        redde NIHIL;
    }
    modulus = demissio_currere(piscina_officinae, parsura, sem,
        _ch(via_parsurae));
    si (modulus != NIHIL)
    {
        /* fenestra collige-ante-destrue: lineae + INDICIUM */
        lineae = demissio_lineas_colligere(piscina_officinae,
            modulus, parsura);
        (vacuum)indicium_modulum_colligere(scriptor_indicii,
            modulus, parsura, sem);
    }
    silva_piscina_destruere(piscina_arboris);
    si (modulus != NIHIL)
    {
        MedullaModulus** locellus = xar_addere(moduli_mundi);
        MedullaLineae** locellus_l = xar_addere(lineae_mundi);

        *locellus = modulus;
        *locellus_l = lineae;
    }
    redde modulus;
}

s32
principale (vacuum)
{
    Piscina* piscina;
    Piscina* piscina_ctx;
    SilvaPiscina* piscina_silvae_ctx;
    SilvaContextus* ctx;
    b32 successus;
    constans character* radix_ambitus = getenv("RHUBARB_RADIX");

    si (radix_ambitus != NIHIL)
    {
        radix_repositorii = radix_ambitus;
    }

    imprimere("\n========================================\n");
    imprimere("PROBATIO OFFICINA VINDEX (SIGILLUM M3)\n");
    imprimere("========================================\n");

    piscina = piscina_generare_dynamicum("probatio_vindex",
        LXIV * M * M);
    piscina_ctx = piscina_generare_dynamicum("vindex_ctx", 8388608);
    piscina_officinae = piscina_generare_dynamicum("vindex_off",
        33554432);
    piscina_silvae_ctx = silva_piscina_generare_dynamicum(
        "vindex_silva", 8388608);
    si (piscina == NIHIL || piscina_ctx == NIHIL
        || piscina_officinae == NIHIL
        || piscina_silvae_ctx == NIHIL)
    {
        imprimere("FRACTA: piscinae\n");
        redde I;
    }
    credo_aperire(piscina);

    ctx = silva_contextus_creare(piscina_silvae_ctx);
    moduli_mundi = xar_creare(piscina_ctx,
        (i32)magnitudo(MedullaModulus*));
    lineae_mundi = xar_creare(piscina_ctx,
        (i32)magnitudo(MedullaLineae*));
    scriptor_indicii = indicium_scriptor_creare(piscina_officinae);
    CREDO_NON_NIHIL (ctx);
    CREDO_NON_NIHIL (scriptor_indicii);

    /* --- systema ISO+POSIX (exemplar interpretare) --- */
    imprimere("\n--- Praeparans systema + capita ---\n");
    {
        character via_iso[CCLVI];
        character via_posix[CCLVI];
        long m_iso = 0L;
        long m_posix = 0L;
        character* fons_iso;
        character* fons_posix;
        character* fons_sys;
        long mensura_sys;

        sprintf(via_iso, "%s/silva/fontes/systema_c89.h",
            radix_repositorii);
        sprintf(via_posix, "%s/silva/fontes/systema_posix.h",
            radix_repositorii);
        fons_iso = _plagulam_legere_cruda(piscina_ctx, via_iso,
            &m_iso);
        fons_posix = _plagulam_legere_cruda(piscina_ctx, via_posix,
            &m_posix);
        CREDO_NON_NIHIL (fons_iso);
        CREDO_NON_NIHIL (fons_posix);
        si (fons_iso == NIHIL || fons_posix == NIHIL)
        {
            redde I;
        }
        mensura_sys = m_iso + 1L + m_posix;
        fons_sys = (character*)piscina_allocare(piscina_ctx,
            (memoriae_index)(mensura_sys + 1L));
        memcpy(fons_sys, fons_iso, (memoriae_index)m_iso);
        fons_sys[m_iso] = '\n';
        memcpy(fons_sys + m_iso + 1L, fons_posix,
            (memoriae_index)m_posix);
        CREDO_VERUM (silva_contextus_lexicon_addere(ctx,
            "systema_c89.h", fons_sys,
            (insignatus integer)mensura_sys));
        systema_parsura = silva_c89_parsare(piscina_silvae_ctx,
            "systema_c89.h", fons_sys,
            (insignatus integer)mensura_sys, NIHIL);
        CREDO_NON_NIHIL (systema_parsura);
        systema_semantica = silva_c89_semantica_analysare(
            piscina_silvae_ctx, systema_parsura);
        CREDO_NON_NIHIL (systema_semantica);
    }
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        _capita_praeparare(ctx, piscina_ctx, visa,
            radix_repositorii);
    }

    /* --- mundus scopo dato + suita --- */
    imprimere("\n--- Demittens mundum (V TUs) ---\n");
    CREDO_NON_NIHIL (_plagulam_demittere(ctx, "lib/piscina.c"));
    CREDO_NON_NIHIL (_plagulam_demittere(ctx, "lib/chorda.c"));
    CREDO_NON_NIHIL (_plagulam_demittere(ctx,
        "lib/chorda_aedificator.c"));
    CREDO_NON_NIHIL (_plagulam_demittere(ctx, "lib/credo.c"));
    CREDO_NON_NIHIL (_plagulam_demittere(ctx, VIA_SUITAE));

    /* --- nexus + machinula + indicium scriptum-RECLUSUM --- */
    {
        Regio* regio = regio_generare(piscina_ctx);
        Conexio* conexio = conexio_creare(piscina_ctx, regio);
        Machinula* machinula;
        IndiciumLector* lector;
        character via_indicii[CCLVI];
        i32 k;
        s32 functio_main = -I;
        i32 instructio_puncti = ZEPHYRUM;
        chorda via_suitae = _ch(VIA_SUITAE);

        CREDO_NON_NIHIL (regio);
        CREDO_NON_NIHIL (conexio);
        per (k = ZEPHYRUM; k < xar_numerus(moduli_mundi); k++)
        {
            CREDO_VERUM (conexio_modulum_addere(conexio,
                *(MedullaModulus**)xar_obtinere(moduli_mundi, k)));
        }
        CREDO_VERUM (conexio_nectere(conexio));
        machinula = machinula_creare(piscina_ctx, conexio, regio);
        CREDO_NON_NIHIL (machinula);
        si (machinula == NIHIL)
        {
            redde I;
        }
        per (k = ZEPHYRUM; k < xar_numerus(lineae_mundi); k++)
        {
            MedullaLineae* lineae_k = *(MedullaLineae**)
                xar_obtinere(lineae_mundi, k);

            si (lineae_k != NIHIL)
            {
                machinula_lineas_praebere(machinula, (s32)k,
                    lineae_k);
            }
        }

        imprimere("\n--- Scribens + recludens indicium ---\n");
        sprintf(via_indicii,
            "%s/officina/build/probatio_vindex.indicium",
            radix_repositorii);
        CREDO_VERUM (indicium_scribere(scriptor_indicii, conexio,
            via_indicii));
        lector = indicium_aperire(piscina_ctx, via_indicii);
        CREDO_NON_NIHIL (lector);
        si (lector == NIHIL)
        {
            redde I;
        }
        /* ab hoc puncto: quaestiones per LECTOREM SOLUM */

        /* --- PHASIS 1: punctum per lineam RETRO (linea CREDO) --- */
        imprimere("\n--- Phasis 1: punctum linea %d (CREDO) ---\n",
            (int)LINEA_CREDO);
        {
            constans IndiciumSitus* situs = NIHIL;
            i32 numerus = indicium_situs_de_linea(lector,
                via_suitae, (i32)LINEA_CREDO, &situs);

            CREDO_VERUM (numerus > ZEPHYRUM);
            si (numerus == ZEPHYRUM)
            {
                redde I;
            }
            functio_main = (s32)situs[ZEPHYRUM].functio;
            instructio_puncti = situs[ZEPHYRUM].instructio;
            /* linea CREDO in main: cross-probatio nominis */
            CREDO_AEQUALIS_I32 ((i32)indicium_functionem_quaerere(
                lector, _ch("main")), (i32)functio_main);
            CREDO_VERUM (machinula_punctum_ponere(machinula,
                functio_main, instructio_puncti));
        }
        CREDO_VERUM (machinula_aperire(machinula, _ch("main")));
        imprimere("\n[effusio suitae interpretatae sequitur...]\n");
        CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
            (i32)MACHINULA_PAUSA);
        {
            s32 functio_visa = -I;
            i32 instructio_visa = (i32)-I;

            CREDO_AEQUALIS_I32 (machinula_tabulata_numerus(
                machinula), I);
            CREDO_VERUM (machinula_positionem_inspicere(machinula,
                ZEPHYRUM, &functio_visa, &instructio_visa));
            CREDO_AEQUALIS_I32 ((i32)functio_visa,
                (i32)functio_main);
            CREDO_AEQUALIS_I32 (instructio_visa, instructio_puncti);
        }

        /* acies macronum in situ pausae ("macro-aware" vectis) */
        {
            constans IndiciumLinea* acies = NIHIL;
            i32 n = indicium_lineas_de_instructione(lector,
                (i32)functio_main, instructio_puncti, &acies);

            CREDO_VERUM (n >= II);
            si (n >= II)
            {
                chorda via_radicis = indicium_via_chorda(lector,
                    acies[ZEPHYRUM].via);
                chorda titulus_macronis = indicium_chorda(lector,
                    acies[I].nomen_macro);

                CREDO_AEQUALIS_I32 (acies[ZEPHYRUM].profunditas,
                    ZEPHYRUM);
                CREDO_AEQUALIS_I32 (acies[ZEPHYRUM].linea,
                    (i32)LINEA_CREDO);
                CREDO_VERUM (via_radicis.mensura
                    == via_suitae.mensura
                    && memcmp(via_radicis.datum, via_suitae.datum,
                           (memoriae_index)via_suitae.mensura)
                        == 0);
                CREDO_AEQUALIS_I32 (acies[I].profunditas, I);
                CREDO_VERUM (titulus_macronis.mensura == XVIII
                    && memcmp(titulus_macronis.datum,
                           "CREDO_AEQUALIS_I32", XVIII) == 0);
                CREDO_VERUM (acies[I].linea > ZEPHYRUM);
            }
        }

        /* "usus" per NOMEN FONTIS e VARIABILIA -> registrum == 0 */
        {
            constans IndiciumVariabile* varia = NIHIL;
            i32 numerus = indicium_variabilia_functionis(lector,
                (i32)functio_main, &varia);
            b32 inventum = FALSUM;
            i32 v;

            CREDO_VERUM (numerus > ZEPHYRUM);
            per (v = ZEPHYRUM; v < numerus; v++)
            {
                chorda titulus = indicium_chorda(lector,
                    varia[v].titulus);

                si (titulus.mensura == IV
                    && memcmp(titulus.datum, "usus", IV) == 0)
                {
                    i64 valor = (i64)-I;

                    inventum = VERUM;
                    CREDO_VERUM (machinula_registrum_legere(
                        machinula, ZEPHYRUM,
                        varia[v].index_registri, &valor));
                    CREDO_AEQUALIS_I32 ((i32)valor, ZEPHYRUM);
                    frange;
                }
            }
            CREDO_VERUM (inventum);
        }

        /* pactum ambulationis pro main (vectis trans-phasicus) */
        {
            constans IndiciumFunctio* ifu = indicium_functio(
                lector, (i32)functio_main);

            CREDO_NON_NIHIL (ifu);
            CREDO_AEQUALIS_I32 (ifu->instructiones_numerus,
                machinula_numerus_instructionum_planarum(machinula,
                    functio_main));
        }

        /* --- PHASIS 2: gradus-super-lineam -> linea 39 --- */
        imprimere("\n--- Phasis 2: gradus super lineam ---\n");
        {
            i32 profunditas_introitus = machinula_tabulata_numerus(
                machinula);
            long gradus_facti = 0L;
            i32 linea_nova = ZEPHYRUM;
            s32 functio_visa = -I;
            i32 instructio_visa = ZEPHYRUM;

            dum (gradus_facti < CUSTOS_GRADUUM)
            {
                si (!machinula_gradus(machinula))
                {
                    frange;   /* halitus - non speratum hic */
                }
                gradus_facti++;
                si (machinula_tabulata_numerus(machinula)
                    > profunditas_introitus)
                {
                    perge;   /* custos profunditatis (super) */
                }
                CREDO_VERUM (machinula_positionem_inspicere(
                    machinula, machinula_tabulata_numerus(
                        machinula) - I, &functio_visa,
                    &instructio_visa));
                {
                    constans IndiciumLinea* acies = NIHIL;
                    i32 n = indicium_lineas_de_instructione(lector,
                        (i32)functio_visa, instructio_visa,
                        &acies);

                    si (n > ZEPHYRUM
                        && acies[ZEPHYRUM].linea
                            != (i32)LINEA_CREDO)
                    {
                        linea_nova = acies[ZEPHYRUM].linea;
                        frange;
                    }
                }
            }
            CREDO_AEQUALIS_I32 (linea_nova, (i32)LINEA_POST);
            CREDO_AEQUALIS_I32 ((i32)functio_visa,
                (i32)functio_main);
            CREDO_AEQUALIS_I32 (machinula_tabulata_numerus(
                machinula), I);
        }

        /* --- PHASIS 3: punctum per-functionem trans-modulum
         * (piscina_summa_usus, lib/piscina.c) + FINI --- */
        imprimere("\n--- Phasis 3: punctum in vocato + fini ---\n");
        {
            s32 functio_su = (s32)indicium_functionem_quaerere(
                lector, _ch("piscina_summa_usus"));
            s32 functio_visa = -I;

            CREDO_VERUM (functio_su >= ZEPHYRUM);
            /* modulus alienus (lib/piscina.c, non suita) */
            {
                constans IndiciumFunctio* ifu = indicium_functio(
                    lector, (i32)functio_su);
                constans IndiciumFunctio* ifu_main =
                    indicium_functio(lector, (i32)functio_main);

                CREDO_VERUM (ifu != NIHIL && ifu_main != NIHIL
                    && ifu->modulus != ifu_main->modulus);
            }
            CREDO_VERUM (machinula_punctum_ponere(machinula,
                functio_su, ZEPHYRUM));
            CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
                (i32)MACHINULA_PAUSA);
            CREDO_AEQUALIS_I32 (machinula_tabulata_numerus(
                machinula), II);
            CREDO_VERUM (machinula_positionem_inspicere(machinula,
                I, &functio_visa, NIHIL));
            CREDO_AEQUALIS_I32 ((i32)functio_visa, (i32)functio_su);
            CREDO_VERUM (machinula_positionem_inspicere(machinula,
                ZEPHYRUM, &functio_visa, NIHIL));
            CREDO_AEQUALIS_I32 ((i32)functio_visa,
                (i32)functio_main);
            /* FINI: gradus dum profunditas < II */
            {
                long gradus_facti = 0L;

                dum (machinula_tabulata_numerus(machinula) >= II
                    && gradus_facti < CUSTOS_GRADUUM)
                {
                    si (!machinula_gradus(machinula))
                    {
                        frange;
                    }
                    gradus_facti++;
                }
            }
            CREDO_AEQUALIS_I32 (machinula_tabulata_numerus(
                machinula), I);
            CREDO_VERUM (machinula_positionem_inspicere(machinula,
                ZEPHYRUM, &functio_visa, NIHIL));
            CREDO_AEQUALIS_I32 ((i32)functio_visa,
                (i32)functio_main);
            CREDO_VERUM (machinula_punctum_tollere(machinula,
                functio_su, ZEPHYRUM));
        }

        /* --- PHASIS 4: tollere -> BENE codex 0 --- */
        imprimere("\n--- Phasis 4: perfectio suitae ---\n");
        CREDO_VERUM (machinula_punctum_tollere(machinula,
            functio_main, instructio_puncti));
        CREDO_AEQUALIS_I32 ((i32)machinula_pergere(machinula),
            (i32)MACHINULA_BENE);
        CREDO_AEQUALIS_I32 ((i32)machinula_halitus_codex(machinula),
            ZEPHYRUM);
        CREDO_VERUM (machinula_numerus_instructionum(machinula)
            > (i64)40000);

        regio_destruere(regio);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    imprimere("========================================\n");
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? ZEPHYRUM : I;
}
