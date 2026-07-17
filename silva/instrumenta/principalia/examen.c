/* examen.c - effusor iudicii unius plagulae (M4a chunk D)
 *
 * Recipe percursus FIDELE: contextus + capita repositorii praebita
 * (inclusio vera) + lexicon systematis (canalis macrorum) + oraculum
 * praeseminatum + BIS-ANALYSIS post recanonicare (sem2
 * auctoritativum - typatio relativa canonicae).
 *
 * Usus:  examen <plagula.c> [-posix|-nudum] [-machina] [-v]
 * Exitus: 0 ACCIPE | 1 REICE | 2 INFRA
 *
 * Verdictum: REICE = violatio non-provisionalis ULLA aut nodi
 * erroris (syntaxis fracta = reiectio C89); INFRA = apparatus
 * solum (plagula illegibilis, systema deest).
 *
 * Geminus tertius oneratorum (percursus + vindex_onerator + hic) -
 * migratio ad fontem communem = parca nominata, tactu proximo.
 */

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "vigilia.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "silva_c89_semantica.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens b32 verbosa = FALSUM;
hic_manens b32 machina = FALSUM;
hic_manens b32 cum_posix = FALSUM;
hic_manens b32 nudum = FALSUM;

hic_manens i32 capita_praebita = ZEPHYRUM;

interior constans character*
_severitatis_titulus (s32 severitas)
{
    commutatio (severitas)
    {
        casus (s32)EXAMEN_VIOLATIO:   redde "violatio";
        casus (s32)EXAMEN_SUSPECTUM:  redde "suspectum";
        casus (s32)EXAMEN_DOMESTICUM: redde "domesticum";
        ordinarius:                   redde "infra";
    }
}

interior character*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    character* textus;

    *mensura_out = ZEPHYRUM;
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l > 0L ? mensura_l + 1L : I));
    si (textus == NIHIL
        || (mensura_l > 0L
            && fread(textus, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_out = (i32)mensura_l;
    redde textus;
}

interior b32
_praetermittendum (constans character* titulus)
{
    redde strcmp(titulus, "build") == ZEPHYRUM
        || strcmp(titulus, ".git") == ZEPHYRUM
        || strcmp(titulus, "node_modules") == ZEPHYRUM;
}

interior vacuum
_caput_praebere (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via,
    constans character* titulus)
{
    chorda clavis;
    character* textus;
    i32 mensura;

    clavis = chorda_ex_literis(titulus, piscina);
    si (tabula_dispersa_continet(visa, clavis))
    {
        redde;   /* collisio basename - primus vicit */
    }
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        redde;
    }
    si (silva_contextus_praebere(ctx, titulus, textus, mensura))
    {
        (vacuum)tabula_dispersa_inserere(visa, clavis, NIHIL);
        capita_praebita++;
    }
}

interior vacuum
_capita_praeparare (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via)
{
    DIR* dir = opendir(via);
    structura dirent* introitus;

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
    constans character* via = NIHIL;
    Piscina* piscina;
    SilvaContextus* ctx;
    SilvaParsura* systema_parsura = NIHIL;
    SilvaSemantica* systema_semantica = NIHIL;
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-v") == ZEPHYRUM)
        {
            verbosa = VERUM;
        }
        alioquin si (strcmp(argv[k], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[k], "-posix") == ZEPHYRUM)
        {
            cum_posix = VERUM;
        }
        alioquin si (strcmp(argv[k], "-nudum") == ZEPHYRUM)
        {
            nudum = VERUM;
        }
        alioquin
        {
            via = argv[k];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: examen <plagula.c> [-posix|-nudum]"
            " [-machina] [-v]\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("examen", 268435456);
    si (piscina != NIHIL)
    {
        /* vigilia sui (2026-07-17): binarium hoc SOLUM examen.sh
         * religat - vocatio directa (circuli celeres) post
         * editionem fontium binarium VETUSTUM currit tacite
         * (decipula 2026-07-16: "custos alienitatis non operatur"
         * = custos in binario numquam erat). Manifestum involucrum
         * scribit (invarians aedificatoris); absentia = quieta.
         * Cautio in STDERR solum - effusum -machina intactum. */
        VigiliaConfiguratio vc;
        Vigilia* vigilia;

        vc.signum = NIHIL;
        vc.via_binarii = argv[ZEPHYRUM];
        vc.via_manifesti = "silva/build/examen.manifestum";
        vigilia = vigilia_creare(piscina, &vc);
        si (vigilia != NIHIL
            && vigilia_inspicere(vigilia, piscina)
                == VIGILIA_FONTES_SUPERANT)
        {
            /* nuntius proprius (cautio bibliothecae residentibus
             * scripta est - "/mcp reconnect" hic fallax) */
            fprintf(stderr, "CAUTIO examen: binarium vetustius"
                " fontibus (%s) - per ./silva/examen.sh recurre\n",
                vigilia_causa(vigilia));
        }
    }
    si (piscina == NIHIL)
    {
        fprintf(stderr, "examen: piscina deest\n");
        redde II;
    }
    ctx = silva_contextus_creare(piscina);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "examen: contextus deest\n");
        redde II;
    }

    /* systema (lexicon): ISO ordinarius; -posix concatenat
     * (exemplar vindex_onerator); -nudum = sine (LEXICON_DEEST
     * probandus) */
    si (!nudum)
    {
        character* fons_sys;
        i32 mensura_sys = ZEPHYRUM;

        fons_sys = _plagulam_legere(piscina,
            "silva/fontes/systema_c89.h", &mensura_sys);
        si (fons_sys == NIHIL)
        {
            fprintf(stderr, "examen: systema_c89.h deest (curre"
                " ex radice repositorii)\n");
            redde II;
        }
        si (cum_posix)
        {
            i32 mensura_px = ZEPHYRUM;
            character* fons_px = _plagulam_legere(piscina,
                "silva/fontes/systema_posix.h", &mensura_px);

            si (fons_px == NIHIL)
            {
                fprintf(stderr,
                    "examen: systema_posix.h deest\n");
                redde II;
            }
            {
                character* iunctum = (character*)piscina_allocare(
                    piscina, (memoriae_index)(mensura_sys
                        + mensura_px + I));

                si (iunctum == NIHIL)
                {
                    redde II;
                }
                memcpy(iunctum, fons_sys,
                    (memoriae_index)mensura_sys);
                iunctum[mensura_sys] = '\n';
                memcpy(iunctum + mensura_sys + I, fons_px,
                    (memoriae_index)mensura_px);
                fons_sys = iunctum;
                mensura_sys = mensura_sys + I + mensura_px;
            }
        }
        si (!silva_contextus_lexicon_addere(ctx, "systema_c89.h",
                fons_sys, mensura_sys))
        {
            fprintf(stderr, "examen: lexicon non additum\n");
            redde II;
        }
        systema_parsura = silva_c89_parsare(piscina,
            "systema_c89.h", fons_sys, mensura_sys, NIHIL);
        si (systema_parsura == NIHIL
            || systema_parsura->numerus_errorum > ZEPHYRUM)
        {
            fprintf(stderr, "examen: systema non parsatum\n");
            redde II;
        }
        systema_semantica = silva_c89_semantica_analysare(piscina,
            systema_parsura);
        si (systema_semantica == NIHIL)
        {
            fprintf(stderr, "examen: systema non analysatum\n");
            redde II;
        }
    }

    /* capita repositorii (inclusio vera) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina, DXII);

        si (visa != NIHIL)
        {
            _capita_praeparare(ctx, piscina, visa, ".");
        }
        si (verbosa)
        {
            fprintf(stderr, "examen: capita %d praebita\n",
                (int)capita_praebita);
        }
    }

    /* plagula ipsa: parsare + bis-analysis (recipe percursus) */
    {
        i32 mensura = ZEPHYRUM;
        character* fons = _plagulam_legere(piscina, via, &mensura);
        SilvaOraculum* oraculum;
        SilvaParsura* parsura;
        SilvaSemantica* sem;
        i32 violationes = ZEPHYRUM;
        i32 suspecta = ZEPHYRUM;
        i32 domestica = ZEPHYRUM;
        i32 infra = ZEPHYRUM;
        i32 provisionalia = ZEPHYRUM;
        b32 reice = FALSUM;

        si (fons == NIHIL)
        {
            fprintf(stderr, "examen: plagula illegibilis: %s\n",
                via);
            redde II;
        }
        oraculum = silva_oraculum_creare(piscina);
        si (oraculum != NIHIL && systema_semantica != NIHIL)
        {
            (vacuum)silva_c89_semantica_oraculum_augere(
                systema_semantica, oraculum);
        }
        parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
            fons, mensura, oraculum);
        si (parsura == NIHIL || !parsura->successus
            || parsura->commissio == NIHIL)
        {
            fprintf(stderr, "examen: apparatus fractus: %s\n", via);
            redde II;
        }
        sem = silva_c89_semantica_analysare_cum_systemate(piscina,
            parsura, systema_parsura);
        si (sem != NIHIL && oraculum != NIHIL)
        {
            /* clausura: symbola plagulae in oraculum ->
             * recanonicare -> analysis auctoritativa */
            (vacuum)silva_c89_semantica_oraculum_augere(sem,
                oraculum);
            silva_oraculum_responsa_vacare(oraculum);
            (vacuum)silva_recanonicare(parsura->commissio, oraculum,
                silva_c89_resolutor, NIHIL);
            sem = silva_c89_semantica_analysare_cum_systemate(
                piscina, parsura, systema_parsura);
        }
        si (sem == NIHIL)
        {
            fprintf(stderr, "examen: semantica fracta: %s\n", via);
            redde II;
        }

        si (parsura->numerus_errorum > ZEPHYRUM)
        {
            reice = VERUM;   /* syntaxis fracta = reiectio C89 */
            si (machina)
            {
                imprimere("%s\t0\t0\tviolatio\t-1\t0\t"
                    "nodi erroris (syntaxis) %d\n", via,
                    (int)parsura->numerus_errorum);
            }
            alioquin
            {
                imprimere("%s: [violatio] nodi erroris (syntaxis)"
                    " %d\n", via, (int)parsura->numerus_errorum);
            }
        }

        {
            i32 i;
            i32 m = (i32)silva_c89_diagnostica_numerus(sem);

            per (i = ZEPHYRUM; i < m; i++)
            {
                constans SemanticaDiagnosticum* d =
                    silva_c89_diagnosticum_per_indicem(sem, i);

                si (d == NIHIL)
                {
                    perge;
                }
                si (d->provisionale)
                {
                    provisionalia++;
                }
                commutatio (d->severitas)
                {
                    casus (s32)EXAMEN_VIOLATIO:
                        violationes++;
                        si (!d->provisionale)
                        {
                            reice = VERUM;
                        }
                        frange;
                    casus (s32)EXAMEN_SUSPECTUM:
                        suspecta++;
                        frange;
                    casus (s32)EXAMEN_DOMESTICUM:
                        domestica++;
                        frange;
                    ordinarius:
                        infra++;
                        frange;
                }
                si (machina)
                {
                    imprimere("%.*s\t%d\t%d\t%s\t%d\t%d\t%s\n",
                        (int)d->via.mensura,
                        (constans character*)d->via.datum,
                        (int)d->linea, (int)d->columna,
                        _severitatis_titulus(d->severitas),
                        (int)d->codex,
                        (int)(d->provisionale ? I : ZEPHYRUM),
                        d->causa);
                }
                alioquin
                {
                    imprimere("%.*s:%d:%d: [%s] %s%s\n",
                        (int)d->via.mensura,
                        (constans character*)d->via.datum,
                        (int)d->linea, (int)d->columna,
                        _severitatis_titulus(d->severitas),
                        d->causa,
                        d->provisionale ? " (provisionale)" : "");
                }
            }
        }

        si (machina)
        {
            imprimere("VERDICTUM\t%s\n",
                reice ? "REICE" : "ACCIPE");
        }
        alioquin
        {
            imprimere("verdictum: %s  (violationes %d, suspecta %d,"
                " domestica %d, infra %d; provisionalia %d)\n",
                reice ? "REICE" : "ACCIPE", (int)violationes,
                (int)suspecta, (int)domestica, (int)infra,
                (int)provisionalia);
        }
        piscina_destruere(piscina);
        redde reice ? I : ZEPHYRUM;
    }
}
