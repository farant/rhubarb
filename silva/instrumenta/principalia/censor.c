/* censor.c - CENSOR LATINAE (instrumenta prima #1): custos contra
 * macra latinae in positionibus ligaminis nominum.
 *
 * Classis A (SILENS - ratio essendi): macrum latinae expansum in
 * loco "tok_titulus" (declarator, membrum, parametrum, enumerator,
 * titulus, tag, accessus) - identificator VALIDUS alius quam
 * scriptus. Exemplar historicum: campus "exire" tacite "exit"
 * factus (flagratio quarta, tessera Phase C) - nulla diagnostica
 * usquam, solum catena originis novit.
 *
 * Classis B (FRAGOR OBSCURUS, monitum solum): macrum latinae
 * expansum intra nodum ERROR (exemplar: "nomen" ut parametrum ->
 * typedef -> parsatio fracta). Compilator clamat sed obscure;
 * censor causam probabilem plane nominat. Ordines B exitum NON
 * mutant (fractura ipsa iam clamat).
 *
 * MECHANICA, NON LISTAE MANU SCRIPTAE (lectio AEDIFICATA):
 *   - inventarium locorum: SILVA_C89_REGISTRUM percurritur; omnis
 *     locus specie TOKEN titulo "tok_titulus" = positio nominis.
 *     Loci "tok_valor" (folium: expressio) et "tok_destinatio"
 *     (salta: usus tituli) CONSULTO omissi - positio expressionis
 *     est ipsum consilium latinae (imprimere, NIHIL, numeri).
 *   - copia macrorum latinae: acta macrorum expansionis
 *     (silva_macro_vista) - macra quorum fons in "latina.h"
 *     definitus est. Plagulae sine latina.h (knotapel, anglicae)
 *     copiam vacuam habent et gratis praetermittuntur.
 *
 * Exceptiones nominatae (causa recordata, genere circumscriptae):
 *   principale -> main  (declarator-titulus: omne programma id
 *                        consulto ligat)
 *   FILUM -> FILE       (typus-nominatus: usus typi deliberatus
 *                        ubique; "int FILUM;" tamen flagrat quia
 *                        declarator-titulus, non typus-nominatus)
 *
 * Usus:  ./silva/censor.sh <plagula.c> [plagula2 ...]
 *        ./silva/censor.sh -corpus [radix]
 * Vexilla: -omnia (tectum mensurae sublatum), -v (verbosa)
 * Exitus: 0 purus | 1 flagrationes (aut truncatio) | 2 sine arbore
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
#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens b32 verbosa = FALSUM;
hic_manens i32 mensura_maxima = 4194304;  /* -omnia = sine tecto */

hic_manens i32 plagulae = ZEPHYRUM;
hic_manens i32 plagulae_cum_latina = ZEPHYRUM;
hic_manens i32 flagrationes = ZEPHYRUM;
hic_manens i32 suspecta = ZEPHYRUM;
hic_manens i32 sine_arbore = ZEPHYRUM;
hic_manens i32 praetermissae = ZEPHYRUM;

/* --------------------------------------------------
 * auxilia chordarum
 * -------------------------------------------------- */

/* cauda viae post ultimum '/' == titulus? */
hic_manens b32
_basename_aequalis (constans chorda* via, constans character* titulus)
{
    s32 initium = ZEPHYRUM;
    s32 k;
    chorda cauda;

    si (via == NIHIL || via->datum == NIHIL) redde FALSUM;
    per (k = ZEPHYRUM; k < (s32)via->mensura; k++)
    {
        si (via->datum[k] == '/') initium = k + I;
    }
    cauda.datum = via->datum + initium;
    cauda.mensura = (i32)((s32)via->mensura - initium);
    redde chorda_aequalis_literis(cauda, titulus);
}

/* --------------------------------------------------
 * copia macrorum latinae (per TU, ex actis expansionis)
 * -------------------------------------------------- */

hic_manens TabulaDispersa*
_latina_macra_colligere (constans SilvaExpansio* exp,
    Piscina* piscina)
{
    TabulaDispersa* copia;
    i32 numerus;
    i32 k;
    b32 ulla = FALSUM;

    si (exp == NIHIL) redde NIHIL;
    numerus = silva_macros_numerus(exp);
    si (numerus == ZEPHYRUM) redde NIHIL;

    copia = tabula_dispersa_creare_chorda(piscina, DXII);
    si (copia == NIHIL) redde NIHIL;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        SilvaMacroVista vista;

        si (!silva_macro_vista(exp, k, &vista)) perge;
        si (vista.titulus == NIHIL) perge;
        si (_basename_aequalis(
                silva_fons_via(exp, vista.fons_index), "latina.h"))
        {
            (vacuum)tabula_dispersa_inserere(copia, *vista.titulus,
                NIHIL);
            ulla = VERUM;
        }
    }
    redde ulla ? copia : NIHIL;
}

/* --------------------------------------------------
 * catena originis: nomen macri latinae si catena expansionis
 * macrum in latina.h definitum tangit; NIHIL aliter
 * -------------------------------------------------- */

hic_manens constans chorda*
_macrum_latinum (constans SilvaToken* t, TabulaDispersa* copia)
{
    dum (t != NIHIL)
    {
        constans chorda* nm = NIHIL;
        constans SilvaToken* sequens = NIHIL;

        commutatio (t->origo.genus)
        {
            casus SILVA_ORIGO_EXPANSIO:
                nm = t->origo.datum.expansio.nomen_macro;
                sequens = t->origo.datum.expansio.invocatio;
                frange;
            casus SILVA_ORIGO_PASTA:
                nm = t->origo.datum.pasta.nomen_macro;
                sequens = t->origo.datum.pasta.sinister;
                frange;
            casus SILVA_ORIGO_CHORDA:
                nm = t->origo.datum.stringificatio.nomen_macro;
                sequens = t->origo.datum.stringificatio.primus;
                frange;
            casus SILVA_ORIGO_API:
                nm = t->origo.datum.api.nomen_macro;
                sequens = NIHIL;
                frange;
            ordinarius: /* FONS */
                redde NIHIL;
        }
        si (nm != NIHIL && tabula_dispersa_continet(copia, *nm))
        {
            redde nm;
        }
        t = sequens;
    }
    redde NIHIL;
}

/* --------------------------------------------------
 * exceptiones nominatae (genere circumscriptae)
 * -------------------------------------------------- */

hic_manens b32
_exceptum (s32 genus_nodi, constans chorda* nm)
{
    /* principale -> main: omne programma id in declaratore ligat */
    si (genus_nodi == (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS
        && chorda_aequalis_literis(*nm, "principale"))
    {
        redde VERUM;
    }
    /* FILUM -> FILE: usus typi deliberatus (FILUM* pl = ...) */
    si (genus_nodi == (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS
        && chorda_aequalis_literis(*nm, "FILUM"))
    {
        redde VERUM;
    }
    redde FALSUM;
}

/* --------------------------------------------------
 * ordines relati
 * -------------------------------------------------- */

hic_manens vacuum
_flagrationem_referre (constans SilvaExpansio* exp,
    constans character* via_tu, SilvaToken* t,
    constans chorda* nm, constans character* titulus_generis)
{
    SilvaToken* radix = silva_token_radix(t);
    constans chorda* via_fontis = NIHIL;

    si (radix != NIHIL && exp != NIHIL)
    {
        via_fontis = silva_fons_via(exp, radix->fons_index);
    }
    flagrationes++;
    si (via_fontis != NIHIL)
    {
        imprimere("[CENSURA] %.*s:%u:%u ", (int)via_fontis->mensura,
            (constans character*)via_fontis->datum,
            radix != NIHIL ? radix->linea : 0u,
            radix != NIHIL ? radix->columna : 0u);
    }
    alioquin
    {
        imprimere("[CENSURA] %s:?:? ", via_tu);
    }
    si (radix != NIHIL)
    {
        imprimere("'%.*s'", (int)radix->valor.mensura,
            (constans character*)radix->valor.datum);
    }
    imprimere(" -> '%.*s' per macrum latinae '%.*s' (%s)\n",
        (int)t->valor.mensura,
        (constans character*)t->valor.datum,
        (int)nm->mensura, (constans character*)nm->datum,
        titulus_generis != NIHIL ? titulus_generis : "?");
}

#define SUSPECTA_MAXIMA VIII

hic_manens vacuum
_suspecta_referre (constans SilvaExpansio* exp,
    constans character* via_tu, SilvaToken* primus,
    constans chorda** macra, i32 numerus_macrorum, b32 plura)
{
    SilvaToken* radix = silva_token_radix(primus);
    constans chorda* via_fontis = NIHIL;
    i32 k;

    si (radix != NIHIL && exp != NIHIL)
    {
        via_fontis = silva_fons_via(exp, radix->fons_index);
    }
    suspecta++;
    si (via_fontis != NIHIL)
    {
        imprimere("[suspectum in errore] %.*s:%u:%u ",
            (int)via_fontis->mensura,
            (constans character*)via_fontis->datum,
            radix != NIHIL ? radix->linea : 0u,
            radix != NIHIL ? radix->columna : 0u);
    }
    alioquin
    {
        imprimere("[suspectum in errore] %s:?:? ", via_tu);
    }
    imprimere("macra latinae intra fracturam parsationis:");
    per (k = ZEPHYRUM; k < numerus_macrorum; k++)
    {
        imprimere("%s '%.*s'", k > ZEPHYRUM ? "," : "",
            (int)macra[k]->mensura,
            (constans character*)macra[k]->datum);
    }
    /* tectum tactum = ALTA voce, numquam trunca tacita */
    si (plura) imprimere(", ...");
    imprimere("\n");
}

/* --------------------------------------------------
 * ambulatio arboris: series laborum (cursor in xar crescente -
 * sine tecto profunditatis, sine recursione)
 * -------------------------------------------------- */

hic_manens vacuum
_arborem_censere (constans SilvaParsura* parsura,
    constans character* via_tu, TabulaDispersa* copia,
    Piscina* piscina)
{
    constans SilvaExpansio* exp = parsura->expansio;
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

            /* AMBIGUUS: lectio canonica SOLA (exemplar percursus) */
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

            /* ERROR: classis B - macra latinae DISTINCTA intra
             * nodum colligere et uno ordine referre (homo listam
             * percurrit, landmine inter verba communia exsilit);
             * monitum, non flagratio; intra errorem non
             * descendimus ultra */
            si (nodus->genus == (s32)SILVA_C89_GENUS_ERROR)
            {
                constans chorda* macra[SUSPECTA_MAXIMA];
                i32 numerus_macrorum = ZEPHYRUM;
                b32 plura = FALSUM;
                SilvaToken* primus = NIHIL;

                per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
                {
                    SilvaValor lexemata = nodus->loci[k];
                    i32 m;

                    si (lexemata.genus != SILVA_VALOR_LISTA) perge;
                    per (m = ZEPHYRUM;
                         m < silva_valor_lista_numerus(lexemata);
                         m++)
                    {
                        SilvaValor* elem =
                            silva_valor_lista_obtinere(lexemata, m);
                        constans chorda* nm;
                        i32 d;
                        b32 iam = FALSUM;

                        si (elem == NIHIL
                            || elem->genus != SILVA_VALOR_TOKEN
                            || elem->datum.token == NIHIL)
                        {
                            perge;
                        }
                        nm = _macrum_latinum(elem->datum.token,
                            copia);
                        si (nm == NIHIL) perge;
                        si (primus == NIHIL)
                        {
                            primus = elem->datum.token;
                        }
                        per (d = ZEPHYRUM;
                             d < numerus_macrorum; d++)
                        {
                            si (chorda_aequalis(*macra[d], *nm))
                            {
                                iam = VERUM;
                                frange;
                            }
                        }
                        si (!iam)
                        {
                            si (numerus_macrorum < SUSPECTA_MAXIMA)
                            {
                                macra[numerus_macrorum] = nm;
                                numerus_macrorum++;
                            }
                            alioquin
                            {
                                plura = VERUM;
                            }
                        }
                    }
                }
                si (numerus_macrorum > ZEPHYRUM)
                {
                    _suspecta_referre(exp, via_tu, primus, macra,
                        numerus_macrorum, plura);
                }
                perge;
            }

            /* inventarium mechanicum: loci "tok_titulus" ex
             * registro cocto (numquam lista manu) */
            si (nodus->genus >= ZEPHYRUM
                && nodus->genus
                    < (s32)SILVA_C89_REGISTRUM.numerus_generum)
            {
                constans SilvaTabGenus* tg =
                    &SILVA_C89_REGISTRUM.genera[nodus->genus];

                per (k = ZEPHYRUM;
                     k < nodus->numerus_locorum
                     && k < tg->loci_numerus; k++)
                {
                    constans SilvaTabLocus* tl =
                        &SILVA_C89_REGISTRUM.loci[tg->loci_offset
                            + k];

                    si (tl->species == (s32)SILVA_LOCUS_TOKEN
                        && strcmp(tl->titulus, "tok_titulus")
                            == ZEPHYRUM
                        && nodus->loci[k].genus == SILVA_VALOR_TOKEN
                        && nodus->loci[k].datum.token != NIHIL)
                    {
                        constans chorda* nm = _macrum_latinum(
                            nodus->loci[k].datum.token, copia);

                        si (nm != NIHIL
                            && !_exceptum(nodus->genus, nm))
                        {
                            _flagrationem_referre(exp, via_tu,
                                nodus->loci[k].datum.token, nm,
                                tg->titulus);
                        }
                    }
                }
            }

            /* descende in omnes locos (lexemata a serie ignota) */
            per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
            {
                SilvaValor* novus = (SilvaValor*)xar_addere(series);

                si (novus != NIHIL) *novus = nodus->loci[k];
            }
        }
    }
}

/* --------------------------------------------------
 * per plagulam
 * -------------------------------------------------- */

hic_manens vacuum
_plagulam_censere (constans SilvaContextus* ctx,
    constans character* via)
{
    Piscina* piscina;
    FILE* pl;
    i8* fons;
    long mensura_l;
    i32 mensura;
    SilvaParsura* parsura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "censor: plagula non lecta: %s\n", via);
        sine_arbore++;
        redde;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        sine_arbore++;
        redde;
    }
    mensura = (i32)mensura_l;

    /* tectum mensurae (pathologia arenae plagularum datorum
     * densorum - exemplar percursus; -omnia tollit) */
    si (mensura_maxima > ZEPHYRUM && mensura > mensura_maxima)
    {
        imprimere("[praetermissa mensura %.1f MB] %s\n",
            (duplex)mensura / 1048576.0, via);
        praetermissae++;
        fclose(pl);
        redde;
    }

    piscina = piscina_generare_dynamicum("censor", 8388608);
    si (piscina == NIHIL)
    {
        fclose(pl);
        sine_arbore++;
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
        sine_arbore++;
        redde;
    }
    fclose(pl);

    plagulae++;
    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
        (constans character*)fons, mensura, NIHIL);

    si (parsura == NIHIL || !parsura->successus)
    {
        imprimere("[SINE ARBORE] %s\n", via);
        sine_arbore++;
        piscina_destruere(piscina);
        redde;
    }

    {
        TabulaDispersa* copia = _latina_macra_colligere(
            parsura->expansio, piscina);

        si (copia != NIHIL)
        {
            plagulae_cum_latina++;
            _arborem_censere(parsura, via, copia, piscina);
        }
        alioquin si (verbosa)
        {
            imprimere("  [sine latina] %s\n", via);
        }
    }
    piscina_destruere(piscina);
}

/* --------------------------------------------------
 * modus corporis (ambulatio directoriorum - exemplar percursus)
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
        /* corpus regressionis censoris ipsius: flagrationes
         * CONSULTO plantatae - sweep corporis eas praetermittit,
         * porta eas via explicita dat et exitum 1 postulat */
        || strcmp(titulus, "censoris") == ZEPHYRUM)
        ? VERUM : FALSUM;
}

hic_manens vacuum
_directorium_censere (constans SilvaContextus* ctx,
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
            _directorium_censere(ctx, via_plena);
        }
        alioquin si (_est_fons_c(introitus->d_name))
        {
            _plagulam_censere(ctx, via_plena);
        }
    }
    closedir(dir);
}

/* --------------------------------------------------
 * praeparatio capitum (exemplar percursus: omne caput sub
 * basename praebere - inclusio VERA per plagulam)
 * -------------------------------------------------- */

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

/* -------------------------------------------------- */

s32 principale (integer argc, character** argv)
{
    Piscina* piscina_ctx;
    SilvaContextus* ctx;
    b32 modus_corporis = FALSUM;
    constans character* radix = ".";
    integer k;
    integer plagulae_datae = ZEPHYRUM;

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
        alioquin si (strcmp(argv[k], "-corpus") == ZEPHYRUM)
        {
            modus_corporis = VERUM;
        }
        alioquin si (modus_corporis)
        {
            radix = argv[k];
        }
        alioquin
        {
            plagulae_datae++;
        }
    }
    si (!modus_corporis && plagulae_datae == ZEPHYRUM)
    {
        fprintf(stderr, "usus: censor <plagula.c> [..] | "
            "censor -corpus [radix]  (ex radice repositorii)\n");
        redde II;
    }

    piscina_ctx = piscina_generare_dynamicum("censor_ctx", 8388608);
    si (piscina_ctx == NIHIL)
    {
        fprintf(stderr, "censor: piscina deest\n");
        redde II;
    }
    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "censor: contextus deest\n");
        redde II;
    }
    si (mensura_maxima == ZEPHYRUM)
    {
        ctx->fines.lexemata = ZEPHYRUM;  /* sine tecto - cave */
    }

    /* omne caput repositorii praebere (inclusio vera; SEMPER a
     * radice repositorii = cwd) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL)
        {
            fprintf(stderr, "censor: tabula deest\n");
            redde II;
        }
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    si (modus_corporis)
    {
        _directorium_censere(ctx, radix);
    }
    alioquin
    {
        per (k = I; k < argc; k++)
        {
            si (argv[k][ZEPHYRUM] == '-') perge;
            _plagulam_censere(ctx, argv[k]);
        }
    }

    imprimere("\n=== CENSOR LATINAE ===\n");
    imprimere("plagulae:     %d (%d cum latina", (int)plagulae,
        (int)plagulae_cum_latina);
    si (praetermissae > ZEPHYRUM)
    {
        imprimere("; %d praetermissae", (int)praetermissae);
    }
    imprimere(")\n");
    imprimere("flagrationes: %d\n", (int)flagrationes);
    si (suspecta > ZEPHYRUM)
    {
        imprimere("suspecta:     %d (in fracturis parsationis)\n",
            (int)suspecta);
    }
    si (sine_arbore > ZEPHYRUM)
    {
        imprimere("sine arbore:  %d  <- INSPICE\n",
            (int)sine_arbore);
    }
    imprimere("conditio:     %s\n",
        (flagrationes == ZEPHYRUM && sine_arbore == ZEPHYRUM)
            ? "PURUS" : "CENSURA");

    si (sine_arbore > ZEPHYRUM) redde II;
    redde (flagrationes > ZEPHYRUM) ? I : ZEPHYRUM;
}
