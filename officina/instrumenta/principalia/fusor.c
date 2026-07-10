/* fusor.c - Instrumentum fusionis corporis (M1b D)
 *
 * "Percursus officinae": omnem plagulam .c repositorii per fistulam
 * plenam fundit - parse (contextu + lexico systematis) -> sem1 ->
 * clausura -> recanonicare -> sem2 -> DEMISSIO -> scriptura ->
 * lector -> scriptura (circulus byte-idem). Fistula bis-analysans
 * ex percurso exscripta (silva.h superficie publica).
 *
 * Vectis M1b: 0 ruinae; sistere OMNES classificatae (per causam
 * numeratae - hoc instrumentum EST classificatio); circuli
 * byte-idem. Praetermissio ALTA voce (tectum mensurae percursus).
 *
 * Usus: ./officina/fusor.sh [radix] [-omnia] [-v]
 *       (ex radice repositorii)
 */

#include "officina_demissio.h"
#include "officina_medulla_textus.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

/* ==================================================
 * Status globalis
 * ================================================== */

hic_manens b32 verbosa = FALSUM;
hic_manens i32 mensura_maxima = 4194304;   /* -omnia = sine tecto */
hic_manens constans character* causa_quaesita = NIHIL;

/* linea primi lexematis subarboris (probatio -causa) */
hic_manens unsigned int
_linea_nodi (constans SilvaNodus* nodus, i32 profunditas)
{
    i32 i;

    si (nodus == NIHIL || profunditas > XXXII)
    {
        redde 0;
    }
    per (i = ZEPHYRUM; i < (i32)nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (v->genus == SILVA_VALOR_TOKEN && v->datum.token != NIHIL)
        {
            redde v->datum.token->linea;
        }
        si (v->genus == SILVA_VALOR_NODUS)
        {
            unsigned int linea = _linea_nodi(v->datum.nodus,
                profunditas + I);

            si (linea > 0)
            {
                redde linea;
            }
        }
    }
    redde 0;
}

hic_manens long plagulae = 0L;
hic_manens long praetermissae = 0L;
hic_manens long parsurae_fractae = 0L;
hic_manens long ruinae_demissionis = 0L;
hic_manens long summa_functionum = 0L;
hic_manens long summa_datorum = 0L;
hic_manens long summa_instructionum = 0L;
hic_manens long summa_sistentium = 0L;
hic_manens long plagulae_cum_sistere = 0L;
hic_manens long circuli = 0L;
hic_manens long circuli_fracti = 0L;
hic_manens duplex summa_octetorum = 0.0;
hic_manens duplex ms_arboris = 0.0;
hic_manens duplex ms_demissionis = 0.0;
hic_manens duplex ms_circuli = 0.0;
hic_manens memoriae_index apex_memoriae = 0;
hic_manens character plagula_apicis[512];

hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;

/* causae aggregatae (textu) */
nomen structura {
    character textus[LXIV];
    i32       longitudo;
    long      sistentia;   /* instructiones sistere */
    long      notae;       /* moduli cum causa sine instructione */
} FusorCausa;

hic_manens FusorCausa causae_summae[LXIV];
hic_manens i32 causae_numerus = ZEPHYRUM;

hic_manens vacuum
_causam_aggregare (constans chorda* causa, b32 est_sistere,
    long numerus)
{
    i32 i;
    i32 longitudo = (causa->mensura < (i32)LXIII)
        ? (i32)causa->mensura : (i32)LXIII;

    per (i = ZEPHYRUM; i < causae_numerus; i++)
    {
        si (causae_summae[i].longitudo == (i32)longitudo
            && memcmp(causae_summae[i].textus, causa->datum,
                   (memoriae_index)longitudo) == ZEPHYRUM)
        {
            frange;
        }
    }
    si (i == causae_numerus)
    {
        si (causae_numerus >= (i32)LXIV)
        {
            redde;
        }
        memcpy(causae_summae[i].textus, causa->datum,
            (memoriae_index)longitudo);
        causae_summae[i].longitudo = (i32)longitudo;
        causae_summae[i].sistentia = 0L;
        causae_summae[i].notae = 0L;
        causae_numerus++;
    }
    si (est_sistere)
    {
        causae_summae[i].sistentia += numerus;
    }
    alioquin
    {
        causae_summae[i].notae += numerus;
    }
}

/* ==================================================
 * Modulum numerare (instructiones + sistere per causam)
 * ================================================== */

hic_manens vacuum
_modulum_numerare (constans MedullaModulus* modulus,
    constans character* via)
{
    i32 f;
    i32 numerus_functionum = xar_numerus(modulus->functiones);
    long usus_causarum[CXXVIII];
    i32 numerus_causarum = xar_numerus(modulus->causae);
    long sistentia_plagulae = 0L;
    i32 i;

    si (numerus_causarum > (i32)CXXVIII)
    {
        numerus_causarum = CXXVIII;
    }
    per (i = ZEPHYRUM; i < numerus_causarum; i++)
    {
        usus_causarum[i] = 0L;
    }

    summa_functionum += (long)numerus_functionum;
    summa_datorum += (long)xar_numerus(modulus->data);
    per (f = ZEPHYRUM; f < numerus_functionum; f++)
    {
        MedullaFunctio** functio = (MedullaFunctio**)xar_obtinere(
            modulus->functiones, (i32)f);
        i32 b;
        i32 numerus_bloccorum = xar_numerus((*functio)->blocci);

        per (b = ZEPHYRUM; b < numerus_bloccorum; b++)
        {
            constans MedullaBloccus* bloccus =
                (constans MedullaBloccus*)xar_obtinere(
                    (*functio)->blocci, (i32)b);
            i32 n = xar_numerus(bloccus->instructiones);
            i32 k;

            summa_instructionum += (long)n;
            per (k = ZEPHYRUM; k < n; k++)
            {
                constans MedullaInstructio* instructio =
                    (constans MedullaInstructio*)xar_obtinere(
                        bloccus->instructiones, (i32)k);

                si (instructio->op == (s32)MEDULLA_OP_SISTERE
                    && instructio->a.genus
                        == (s32)MEDULLA_OPERANDUM_CAUSA
                    && instructio->a.datum.index >= ZEPHYRUM
                    && instructio->a.datum.index
                        < (s32)numerus_causarum)
                {
                    usus_causarum[instructio->a.datum.index]++;
                    sistentia_plagulae++;
                    si (causa_quaesita != NIHIL)
                    {
                        constans chorda* causa =
                            medulla_causam_obtinere(modulus,
                                instructio->a.datum.index);

                        si (causa != NIHIL
                            && causa->mensura
                                >= (i32)strlen(causa_quaesita)
                            && memcmp(causa->datum, causa_quaesita,
                                   strlen(causa_quaesita))
                                == ZEPHYRUM)
                        {
                            imprimere("  [causa] %s:%u\n", via,
                                _linea_nodi(instructio->origo,
                                    ZEPHYRUM));
                        }
                    }
                }
            }
        }
    }
    per (i = ZEPHYRUM; i < numerus_causarum; i++)
    {
        constans chorda* causa = medulla_causam_obtinere(modulus,
            (s32)i);

        si (causa == NIHIL)
        {
            perge;
        }
        si (usus_causarum[i] > 0L)
        {
            _causam_aggregare(causa, VERUM, usus_causarum[i]);
            si (verbosa)
            {
                imprimere("    %6ld  %.*s  <- %s\n",
                    usus_causarum[i], (int)causa->mensura,
                    (constans character*)causa->datum, via);
            }
        }
        alioquin
        {
            _causam_aggregare(causa, FALSUM, 1L);
            si (verbosa)
            {
                imprimere("    nota  %.*s  <- %s\n",
                    (int)causa->mensura,
                    (constans character*)causa->datum, via);
            }
        }
    }
    si (sistentia_plagulae > 0L || numerus_causarum > ZEPHYRUM)
    {
        plagulae_cum_sistere++;
        summa_sistentium += sistentia_plagulae;
        si (verbosa || sistentia_plagulae > 0L)
        {
            imprimere("[sistere %ld] %s\n", sistentia_plagulae,
                via);
        }
    }
}

/* ==================================================
 * Plagulam fundere (fistula bis-analysans percursus)
 * ================================================== */

hic_manens vacuum
_plagulam_fundere (constans SilvaContextus* ctx,
    constans character* via)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    SilvaPiscina* piscina_arboris;
    Piscina* piscina_officinae;
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
    si (mensura_maxima > ZEPHYRUM
        && mensura > (i32)mensura_maxima)
    {
        imprimere("[PRAETERMISSA mensura %.1f MB] %s\n",
            (duplex)mensura / 1048576.0, via);
        praetermissae++;
        fclose(pl);
        redde;
    }

    piscina_arboris = silva_piscina_generare_dynamicum(
        "fusor_arbor", 8388608);
    piscina_officinae = piscina_generare_dynamicum(
        "fusor_officina", 8388608);
    si (piscina_arboris == NIHIL || piscina_officinae == NIHIL)
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
    summa_octetorum += (duplex)mensura;

    /* fistula bis-analysans (percursus exscriptus, superficies
     * publica) */
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
        imprimere("[PARSURA FRACTA] %s\n", via);
        parsurae_fractae++;
        silva_piscina_destruere(piscina_arboris);
        piscina_destruere(piscina_officinae);
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
        /* analysi secunda contra arborem versam - typatio
         * canonicae-relativa (contractus demissionis) */
        sem = silva_c89_semantica_analysare_cum_systemate(
            piscina_arboris, parsura, systema_parsura);
    }
    c1 = clock();
    ms_arboris += (duplex)(c1 - c0) * 1000.0
        / (duplex)CLOCKS_PER_SEC;
    si (sem == NIHIL)
    {
        imprimere("[SEMANTICA FRACTA] %s\n", via);
        parsurae_fractae++;
        silva_piscina_destruere(piscina_arboris);
        piscina_destruere(piscina_officinae);
        redde;
    }

    /* demissio */
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
        imprimere("[DEMISSIO RUIT] %s\n", via);
        ruinae_demissionis++;
        silva_piscina_destruere(piscina_arboris);
        piscina_destruere(piscina_officinae);
        redde;
    }
    _modulum_numerare(modulus, via);

    /* circulus: scriptura -> lector -> scriptura byte-idem */
    {
        chorda scriptura;
        chorda secunda;
        MedullaModulus* iterum;
        unsigned int linea = 0;

        c0 = clock();
        scriptura = medulla_textum_scribere(piscina_officinae,
            modulus);
        iterum = medulla_textum_legere(piscina_officinae,
            scriptura, &linea);
        circuli++;
        si (iterum == NIHIL)
        {
            imprimere("[CIRCULUS FRACTUS linea %u] %s\n", linea,
                via);
            circuli_fracti++;
        }
        alioquin
        {
            secunda = medulla_textum_scribere(piscina_officinae,
                iterum);
            si (secunda.mensura != scriptura.mensura
                || (scriptura.mensura > ZEPHYRUM
                    && memcmp(secunda.datum, scriptura.datum,
                           (memoriae_index)scriptura.mensura)
                        != ZEPHYRUM))
            {
                imprimere("[CIRCULUS DIVERGENS] %s\n", via);
                circuli_fracti++;
            }
        }
        c1 = clock();
        ms_circuli += (duplex)(c1 - c0) * 1000.0
            / (duplex)CLOCKS_PER_SEC;
    }

    {
        memoriae_index apex = piscina_summa_apex_usus(
            piscina_officinae);

        si (apex > apex_memoriae)
        {
            apex_memoriae = apex;
            strncpy(plagula_apicis, via,
                magnitudo(plagula_apicis) - I);
            plagula_apicis[magnitudo(plagula_apicis) - I] = '\0';
        }
    }
    silva_piscina_destruere(piscina_arboris);
    piscina_destruere(piscina_officinae);
}

/* ==================================================
 * Ambulatio repositorii (percursus exscriptus)
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

hic_manens b32
_est_fons_c (constans character* titulus)
{
    memoriae_index m = strlen(titulus);

    redde (m >= III && titulus[m - II] == '.'
        && titulus[m - I] == 'c') ? VERUM : FALSUM;
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

hic_manens vacuum
_directorium_fundere (constans SilvaContextus* ctx,
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
            _directorium_fundere(ctx, via_plena);
        }
        alioquin si (_est_fons_c(introitus->d_name))
        {
            _plagulam_fundere(ctx, via_plena);
        }
    }
    closedir(dir);
}

/* ==================================================
 * Principale
 * ================================================== */

s32 principale (integer argc, character** argv)
{
    Piscina* piscina_ctx;
    SilvaPiscina* piscina_silvae_ctx;
    SilvaContextus* ctx;
    constans character* radix = ".";
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-v") == ZEPHYRUM)
        {
            verbosa = VERUM;
        }
        alioquin si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;
        }
        alioquin si (strcmp(argv[k], "-causa") == ZEPHYRUM
            && k + I < argc)
        {
            k++;
            causa_quaesita = argv[k];
        }
        alioquin
        {
            radix = argv[k];
        }
    }

    piscina_ctx = piscina_generare_dynamicum("fusor_ctx", 8388608);
    piscina_silvae_ctx = silva_piscina_generare_dynamicum(
        "fusor_silva_ctx", 8388608);
    si (piscina_ctx == NIHIL || piscina_silvae_ctx == NIHIL)
    {
        fprintf(stderr, "fusor: piscina deest\n");
        redde I;
    }
    ctx = silva_contextus_creare(piscina_silvae_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "fusor: contextus deest\n");
        redde I;
    }
    plagula_apicis[ZEPHYRUM] = '\0';

    /* systema semel parsatum + lexicon (canalis M0b) */
    {
        FILE* pl_sys = fopen("silva/fontes/systema_c89.h", "rb");
        long mensura_sys;
        character* fons_sys;

        si (pl_sys == NIHIL)
        {
            fprintf(stderr, "fusor: systema_c89.h deest"
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
            fprintf(stderr, "fusor: systema non lectum\n");
            fclose(pl_sys);
            redde I;
        }
        fclose(pl_sys);
        si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
                fons_sys, (unsigned int)mensura_sys))
        {
            fprintf(stderr, "fusor: lexicon non additum\n");
            redde I;
        }
        systema_parsura = silva_c89_parsare(piscina_silvae_ctx,
            "systema_c89.h", fons_sys, (unsigned int)mensura_sys,
            NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr, "fusor: systema non parsatum\n");
            redde I;
        }
        systema_semantica = silva_c89_semantica_analysare(
            piscina_silvae_ctx, systema_parsura);
        si (systema_semantica == NIHIL)
        {
            fprintf(stderr, "fusor: systema non analysatum\n");
            redde I;
        }
    }

    /* praepassus capitum (inclusio vera) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL)
        {
            fprintf(stderr, "fusor: tabula deest\n");
            redde I;
        }
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    _directorium_fundere(ctx, radix);

    imprimere("\n=== FUSOR CORPORIS (demissio -> medulla) ===\n");
    imprimere("plagulae:      %ld (%.1f MB)", plagulae,
        summa_octetorum / 1048576.0);
    si (praetermissae > 0L)
    {
        imprimere("  [+%ld praetermissae super tectum]",
            praetermissae);
    }
    imprimere("\n");
    imprimere("fractae:       %ld parsurae/semanticae\n",
        parsurae_fractae);
    imprimere("ruinae:        %ld demissionis\n",
        ruinae_demissionis);
    imprimere("functiones:    %ld   data: %ld   instructiones:"
        " %ld\n", summa_functionum, summa_datorum,
        summa_instructionum);
    imprimere("sistere:       %ld instructiones in %ld plagulis\n",
        summa_sistentium, plagulae_cum_sistere);
    {
        i32 i;

        per (i = ZEPHYRUM; i < causae_numerus; i++)
        {
            imprimere("  %8ld sistere %8ld notae  %.*s\n",
                causae_summae[i].sistentia, causae_summae[i].notae,
                (int)causae_summae[i].longitudo,
                causae_summae[i].textus);
        }
    }
    imprimere("circuli:       %ld/%ld byte-idem (fracti %ld)\n",
        circuli - circuli_fracti, circuli, circuli_fracti);
    imprimere("tempus:        arbor %.0f ms | demissio %.0f ms |"
        " circulus %.0f ms\n", ms_arboris, ms_demissionis,
        ms_circuli);
    imprimere("apex:          %.1f MB (%s)\n",
        (duplex)apex_memoriae / 1048576.0, plagula_apicis);

    si (parsurae_fractae > 0L || ruinae_demissionis > 0L
        || circuli_fracti > 0L)
    {
        redde I;
    }
    redde ZEPHYRUM;
}
