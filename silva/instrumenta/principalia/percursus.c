/* percursus.c - PERCURSUS REPOSITORII (M2d Chunk C): omnem
 * plagulam .c/.h monorepositorii per fistulam plenam (contextus
 * latinus + grammatica c89 + saltatio) parsare et censum referre:
 *
 *   - totalitas: omnis plagula arborem dat (norma tree-sitter)
 *   - fidelitas: scriptura == fons octetim (oraculum implacabile)
 *   - nodi ERROR per plagulam (metrum comprehensionis M2d)
 *   - NUMERATIONES EVIDENTIAE (portae decisionum nominatarum):
 *       typedef intra corpora functionum (decisiones 13)
 *       CASE intra nodos ERROR (machina Duffii, pretium gregum)
 *       identificatores "__" intra ERROR (extensiones gcc)
 *
 * Instrumentum dev-time (non probatio - percursus 38MB minuta
 * capit); numeri in phase-log / mensura historia referuntur.
 * Usus: ./silva/percursus.sh [radix] [-v]
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
#include "silva_scribere.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens b32 verbosa = FALSUM;
hic_manens i32 mensura_maxima = 4194304;  /* -omnia = sine tecto */
hic_manens i32 praetermissae = ZEPHYRUM;

hic_manens i32 plagulae = ZEPHYRUM;
hic_manens i32 arbores = ZEPHYRUM;
hic_manens i32 fideles = ZEPHYRUM;
hic_manens i32 summa_errorum = ZEPHYRUM;
hic_manens i32 plagulae_cum_erroribus = ZEPHYRUM;
hic_manens i32 typedef_in_corpore = ZEPHYRUM;
hic_manens i32 duffiana = ZEPHYRUM;
hic_manens i32 gcc_in_errore = ZEPHYRUM;
hic_manens duplex summa_octetorum = 0.0;
hic_manens duplex summa_ms = 0.0;
hic_manens duplex apex_maximus = 0.0;
hic_manens character plagula_apicis[1024];
hic_manens i32 capita_praebita = ZEPHYRUM;
hic_manens i32 capita_collisiones = ZEPHYRUM;
hic_manens i32 fines_tactae_plagulae = ZEPHYRUM;
hic_manens i32 summa_ambiguorum = ZEPHYRUM;
hic_manens i32 plagulae_cum_ambiguis = ZEPHYRUM;

/* -semantica (M0a Chunk B; DEFALTUM DEMPTUM - basis pristina) */
hic_manens b32 cum_semantica = FALSUM;
hic_manens i32 summa_symbolorum = ZEPHYRUM;
hic_manens i32 summa_diagnosticorum = ZEPHYRUM;
hic_manens i32 plagulae_cum_diagnosticis = ZEPHYRUM;
/* Chunk C: systema semel parsatum (piscina longaeva) + clausura */
hic_manens SilvaParsura*   systema_parsura = NIHIL;
hic_manens SilvaSemantica* systema_semantica = NIHIL;
hic_manens i32 summa_versorum = ZEPHYRUM;
hic_manens i32 summa_indecisorum = ZEPHYRUM;

hic_manens b32 _praetermittendum (constans character* titulus);

/* typedef intra corpus definitionis (ambulatio generica: nodi
 * definitio-functionis -> subarbor corporis -> declarationes cum
 * lexemate TYPEDEF in specificatoribus) */
hic_manens vacuum
_typedefs_numerare (SilvaValor valor, b32 intra_corpus,
    constans character* via, i32 profunditas)
{
    si (profunditas > LXIV) redde;

    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                _typedefs_numerare(*elem, intra_corpus, via,
                    profunditas + I);
            }
        }
        redde;
    }
    si (valor.genus != SILVA_VALOR_NODUS
        || valor.datum.nodus == NIHIL)
    {
        redde;
    }

    {
        constans SilvaNodus* nodus = valor.datum.nodus;
        i32 i;

        si (intra_corpus
            && nodus->genus == (s32)SILVA_C89_GENUS_DECLARATIO)
        {
            SilvaValor specificatores =
                silva_c89_declaratio_specificatores(nodus);
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(specificatores); k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(
                    specificatores, k);

                si (elem != NIHIL
                    && elem->genus == SILVA_VALOR_TOKEN
                    && elem->datum.token != NIHIL
                    && elem->datum.token->genus
                        == SILVA_LEX_TYPEDEF)
                {
                    typedef_in_corpore++;
                    imprimere("  [typedef in corpore] %s:%u\n",
                        via, elem->datum.token->linea);
                }
            }
        }
        si (nodus->genus
            == (s32)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
        {
            intra_corpus = VERUM;
        }
        per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
        {
            _typedefs_numerare(nodus->loci[i], intra_corpus, via,
                profunditas + I);
        }
    }
}

/* Lexemata nodorum ERROR inspicere: CASE (Duffii) + "__" (gcc) */
hic_manens vacuum
_errores_inspicere (SilvaValor valor, constans character* via,
    i32 profunditas)
{
    si (profunditas > LXIV) redde;

    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                _errores_inspicere(*elem, via, profunditas + I);
            }
        }
        redde;
    }
    si (valor.genus != SILVA_VALOR_NODUS
        || valor.datum.nodus == NIHIL)
    {
        redde;
    }

    {
        constans SilvaNodus* nodus = valor.datum.nodus;
        i32 i;

        si (nodus->genus == (s32)SILVA_C89_GENUS_ERROR)
        {
            per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
            {
                SilvaValor lexemata = nodus->loci[i];
                i32 k;

                si (lexemata.genus != SILVA_VALOR_LISTA) perge;
                per (k = ZEPHYRUM;
                     k < silva_valor_lista_numerus(lexemata); k++)
                {
                    SilvaValor* elem = silva_valor_lista_obtinere(
                        lexemata, k);
                    SilvaToken* t;

                    si (elem == NIHIL
                        || elem->genus != SILVA_VALOR_TOKEN)
                    {
                        perge;
                    }
                    t = elem->datum.token;
                    si (t == NIHIL) perge;
                    si (t->genus == SILVA_LEX_CASE)
                    {
                        duffiana++;
                        imprimere("  [casus in errore] %s:%u\n",
                            via, t->linea);
                    }
                    si (t->genus == SILVA_LEX_IDENTIFICATOR
                        && t->valor.mensura >= II
                        && t->valor.datum[ZEPHYRUM] == '_'
                        && t->valor.datum[I] == '_')
                    {
                        gcc_in_errore++;
                        si (verbosa)
                        {
                            imprimere("  [gcc in errore] %s:%u "
                                "%.*s\n", via, t->linea,
                                (int)t->valor.mensura,
                                (constans character*)
                                    t->valor.datum);
                        }
                    }
                }
            }
            redde;  /* intra errorem non descendimus ultra */
        }
        per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
        {
            _errores_inspicere(nodus->loci[i], via,
                profunditas + I);
        }
    }
}

/* EXPANSIO PER INCLUSIONEM VERAM (M2d Chunk D): praepassus omne
 * caput (.h) repositorii sub BASENAME praebet; quaeque plagula
 * suam catenam inclusionum VERAM sequitur (transitive, custodes
 * honorati) - plagulae latina.h includentes latinam accipiunt,
 * ceterae (hospes canariae! knotapel! raqiya) lexica SUA VERA.
 * Heuristica directorii "knotapel -> nudus" RETIRATA: expansio
 * nunc idem videt quod clang - error superstes = divergentia
 * vera. Collisio basename: primus vincit (exemplar saltuarii). */
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
        capita_collisiones++;
        si (verbosa)
        {
            imprimere("  [collisio capitis] %s\n", via);
        }
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

    /* textus in piscina contextus vivit - lexemata praebiti in
     * octetos fontis monstrant, vita = vita contextus */
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
        capita_praebita++;
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
_plagulam_percurrere (constans SilvaContextus* ctx,
    constans character* via)
{
    Piscina* piscina;
    FILE* pl;
    i8* fons;
    long mensura_l;
    i32 mensura;
    SilvaParsura* parsura;
    SilvaOraculum* oraculum_clausurae = NIHIL;
    clock_t c0;
    clock_t c1;

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

    /* TECTUM MENSURAE (inventum M2d C): plagulae datorum densorum
     * (biblia_dr.c 6MB) arenam ad 49 GB inflant - multiplicator
     * ~8000x, pathologia NOMINATA (investigatio propria; lustrum
     * telemetria per subsystema). Praetermissio ALTA voce. */
    si (mensura_maxima > ZEPHYRUM && mensura > mensura_maxima)
    {
        imprimere("[PRAETERMISSA mensura %.1f MB] %s\n",
            (duplex)mensura / 1048576.0, via);
        praetermissae++;
        fclose(pl);
        redde;
    }

    piscina = piscina_generare_dynamicum("percursus", 8388608);
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
    summa_octetorum += (duplex)mensura;

    {
        SilvaOraculum* oraculum_plagulae = NIHIL;

        si (cum_semantica)
        {
            oraculum_plagulae = silva_oraculum_creare(piscina);
            si (oraculum_plagulae != NIHIL
                && systema_semantica != NIHIL)
            {
                (vacuum)silva_c89_semantica_oraculum_augere(
                    systema_semantica, oraculum_plagulae);
            }
        }
        c0 = clock();
        parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
            (constans character*)fons, mensura, oraculum_plagulae);
        c1 = clock();
        oraculum_clausurae = oraculum_plagulae;
    }
    summa_ms += (duplex)(c1 - c0) * 1000.0
        / (duplex)CLOCKS_PER_SEC;

    si (parsura != NIHIL && parsura->successus)
    {
        SilvaScriptura scriptura;

        arbores++;
        /* degradatio picta, non silens (praetermissio ALTA voce):
         * fines_tactae = limen aliquod tactum, expansio decisa */
        si (parsura->fines_tactae)
        {
            fines_tactae_plagulae++;
            imprimere("[fines tactae] %s\n", via);
        }
        si (parsura->numerus_errorum > ZEPHYRUM)
        {
            plagulae_cum_erroribus++;
            summa_errorum += (i32)parsura->numerus_errorum;
            imprimere("[errores %d] %s\n",
                (int)parsura->numerus_errorum, via);
            _errores_inspicere(parsura->commissio->radix, via,
                ZEPHYRUM);
        }
        _typedefs_numerare(parsura->commissio->radix, FALSUM, via,
            ZEPHYRUM);
        {
            i32 amb = xar_numerus(parsura->commissio->ambigui);

            si (amb > ZEPHYRUM)
            {
                summa_ambiguorum += amb;
                plagulae_cum_ambiguis++;
                imprimere("[ambigui %d] %s\n", (int)amb, via);
            }
        }
        si (cum_semantica)
        {
            SilvaSemantica* sem =
                silva_c89_semantica_analysare_cum_systemate(
                    piscina, parsura, systema_parsura);

            si (sem != NIHIL)
            {
                i32 diag = xar_numerus(sem->diagnostica);

                summa_symbolorum += (i32)xar_numerus(sem->symbola);
                si (diag > ZEPHYRUM)
                {
                    summa_diagnosticorum += diag;
                    plagulae_cum_diagnosticis++;
                    imprimere("[semantica diagnostica %d] %s\n",
                        (int)diag, via);
                }
                /* clausura: oraculum auctum symbolis plagulae
                 * (localibus!) -> recanonicare + residuum */
                si (oraculum_clausurae != NIHIL)
                {
                    i32 versae;
                    i32 indecisa;

                    (vacuum)silva_c89_semantica_oraculum_augere(
                        sem, oraculum_clausurae);
                    silva_oraculum_responsa_vacare(
                        oraculum_clausurae);
                    versae = (i32)silva_recanonicare(
                        parsura->commissio, oraculum_clausurae,
                        silva_c89_resolutor, NIHIL);
                    indecisa = (i32)
                        silva_c89_ambigua_indecisa_numerare(
                            parsura->commissio, oraculum_clausurae);
                    summa_versorum += versae;
                    summa_indecisorum += indecisa;
                    si (indecisa > ZEPHYRUM)
                    {
                        imprimere("[indecisa %d] %s\n",
                            (int)indecisa, via);
                    }
                }
            }
            alioquin
            {
                imprimere("[SEMANTICA FRACTA] %s\n", via);
            }
        }

        scriptura = silva_scribere_fontem(piscina, parsura,
            &SILVA_C89_REGISTRUM, parsura->fons_princeps);
        si (scriptura.successus
            && scriptura.textus.mensura == mensura
            && (mensura == ZEPHYRUM
                || memcmp(scriptura.textus.datum, fons,
                       (memoriae_index)mensura) == ZEPHYRUM))
        {
            fideles++;
        }
        alioquin si (!scriptura.successus)
        {
            /* Fractura CLARA (deferral nominatum, e.g. pasta) -
             * non divergentia octetim; discrimen refert (M2d C:
             * arbor2_glr_tabula.c "INFIDELIS" investigationem
             * integram consumpsit quae vere deferral notum erat) */
            imprimere("[SCRIPTURA FRACTA: %s] %s\n",
                scriptura.causa ? scriptura.causa : "?", via);
        }
        alioquin
        {
            imprimere("[INFIDELIS octetim] %s\n", via);
        }
    }
    alioquin
    {
        imprimere("[SINE ARBORE] %s\n", via);
    }

    {
        duplex apex = (duplex)piscina_summa_apex_usus(piscina);

        si (apex > apex_maximus)
        {
            apex_maximus = apex;
            strncpy(plagula_apicis, via,
                magnitudo(plagula_apicis) - I);
            plagula_apicis[magnitudo(plagula_apicis) - I] = '\0';
        }
    }
    si (verbosa)
    {
        imprimere("  %s: %d octeti\n", via, (int)mensura);
    }
    piscina_destruere(piscina);
}

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
        || strcmp(titulus, "node_modules") == ZEPHYRUM)
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

s32 principale (integer argc, character** argv)
{
    Piscina* piscina_ctx;
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
            mensura_maxima = ZEPHYRUM;  /* sine tecto - cave! */
        }
        alioquin si (strcmp(argv[k], "-semantica") == ZEPHYRUM)
        {
            cum_semantica = VERUM;
        }
        alioquin
        {
            radix = argv[k];
        }
    }

    piscina_ctx = piscina_generare_dynamicum("percursus_ctx",
        8388608);
    si (piscina_ctx == NIHIL)
    {
        fprintf(stderr, "percursus: piscina deest\n");
        redde I;
    }
    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "percursus: contextus deest\n");
        redde I;
    }
    si (mensura_maxima == ZEPHYRUM)
    {
        /* -omnia = sine tecto, ETIAM fluxus expansus (defaltum
         * 1M lexemata expansionem in plagulis giganteis decidit -
         * capsula_libri 3.7M lexemata, inventum Chunk D) */
        ctx->fines.lexemata = ZEPHYRUM;
    }
    plagula_apicis[ZEPHYRUM] = '\0';

    /* Chunk C: systema semel parsatum (piscina_ctx longaeva - typi
     * eius trans plagulas vivunt) */
    si (cum_semantica)
    {
        FILE* pl_sys = fopen("silva/fontes/systema_c89.h", "rb");
        long mensura_sys;
        character* fons_sys;

        si (pl_sys == NIHIL)
        {
            fprintf(stderr, "percursus: systema_c89.h deest"
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
            fprintf(stderr, "percursus: systema non lectum\n");
            fclose(pl_sys);
            redde I;
        }
        fclose(pl_sys);
        systema_parsura = silva_c89_parsare(piscina_ctx,
            "systema_c89.h", fons_sys, (i32)mensura_sys, NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr, "percursus: systema non parsatum\n");
            redde I;
        }
        systema_semantica = silva_c89_semantica_analysare(
            piscina_ctx, systema_parsura);
        si (systema_semantica == NIHIL)
        {
            fprintf(stderr, "percursus: systema non analysatum\n");
            redde I;
        }
    }

    /* Praepassus: omne caput repositorii praebere - expansio per
     * inclusionem VERAM (nullum lexicon incondicionale) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL)
        {
            fprintf(stderr, "percursus: tabula deest\n");
            redde I;
        }
        /* SEMPER a radice repositorii (cwd), non a radice
         * percursus: plagulae sub radix capita EXTRA radicem
         * includunt (silva/fontes -> include/latina.h) */
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    _directorium_percurrere(ctx, radix);

    imprimere("\n=== PERCURSUS REPOSITORII (c89 + inclusio"
        " vera) ===\n");
    imprimere("capita:    %d praebita", (int)capita_praebita);
    si (capita_collisiones > ZEPHYRUM)
    {
        imprimere("  [%d collisiones basename - primus vicit]",
            (int)capita_collisiones);
    }
    imprimere("\n");
    imprimere("plagulae:  %d (%.1f MB)", (int)plagulae,
        summa_octetorum / 1048576.0);
    si (praetermissae > ZEPHYRUM)
    {
        imprimere("  [+%d praetermissae super tectum]",
            (int)praetermissae);
    }
    imprimere("\n");
    imprimere("arbores:   %d (totalitas %s)\n", (int)arbores,
        (arbores == plagulae) ? "TENET" : "FRACTA!");
    imprimere("fideles:   %d/%d octetim%s\n", (int)fideles,
        (int)plagulae,
        (fideles == plagulae) ? "" : "  <- INSPICE");
    imprimere("errores:   %d nodi in %d plagulis\n",
        (int)summa_errorum, (int)plagulae_cum_erroribus);
    imprimere("ambigui:   %d retenti in %d plagulis\n",
        (int)summa_ambiguorum, (int)plagulae_cum_ambiguis);
    si (cum_semantica)
    {
        imprimere("semantica: %d symbola; %d diagnostica in %d"
            " plagulis\n",
            (int)summa_symbolorum, (int)summa_diagnosticorum,
            (int)plagulae_cum_diagnosticis);
        imprimere("clausura:  %d versae; %d indecisa (residuum)\n",
            (int)summa_versorum, (int)summa_indecisorum);
    }
    si (fines_tactae_plagulae > ZEPHYRUM)
    {
        imprimere("fines:     %d plagulae limen tactae\n",
            (int)fines_tactae_plagulae);
    }
    imprimere("tempus:    %.0f ms (%.2f ms/KB)\n", summa_ms,
        summa_octetorum > 0.0
            ? summa_ms / (summa_octetorum / 1024.0) : 0.0);
    imprimere("apex:      %.1f MB (%s)\n",
        apex_maximus / 1048576.0, plagula_apicis);
    imprimere("\n--- numerationes evidentiae ---\n");
    imprimere("typedef in corpore functionis: %d (decisiones 13)\n",
        (int)typedef_in_corpore);
    imprimere("casus intra ERROR (Duffiana):  %d\n",
        (int)duffiana);
    imprimere("\"__\" intra ERROR (gcc):        %d\n",
        (int)gcc_in_errore);
    redde (arbores == plagulae) ? ZEPHYRUM : I;
}
