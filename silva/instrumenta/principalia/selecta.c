/* selecta.c - SELECTA (instrumenta prima): quaestio in subsellio.
 * Selector super plagulam unam, congruentia octetim exacta cum
 * capite [via:linea-linea] - grep structura-formatum + ancorae
 * Edit unicae (postulatio 2026-07-11, parcum 01KY53MP5X).
 *
 * Usus: ./silva/selecta.sh <plagula> '<selector>'
 *           [-intra <symbolum>] [-omnia] [-proba]
 *
 * -intra: congruentia ad subarborem definitionis nominatae scopata
 *   (filtrum: maior-aut-ipse congruit
 *   "definitio-functionis:definit(<symbolum>)" - zuccharum QC,
 *   nulla ambulatio propria).
 * -omnia: tectum mensurae plagulae sublatum.
 * -proba: porta nativa (fixtura inclusa, probationes numeratae).
 *
 * Arbor cum erroribus: CAUTIO + PERGE (nodi error quaeribiles -
 * vinculum robustitatis; dissimile emitte, quod definitionem
 * puram exigit).
 *
 * Exitus: 0 congruentia | 1 nulla | 2 fractura (usus/arbor/selector)
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_quaestio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens i32 mensura_maxima = 4194304;  /* -omnia = sine tecto */

/* --------------------------------------------------
 * extentus: lineae fontis integrae subarboris (ut in emitte.c,
 * sed series LOCIS subiecti seminatur - subiectum constans ex
 * resultatis quaestionis venit, nulla abiectio const)
 * -------------------------------------------------- */

nomen structura {
    b32 validus;
    s32 initium;
    s32 finis;
    i32 linea_prima;
    i32 linea_ultima;
} Extentus;

hic_manens Extentus
_extentum_metiri (constans SilvaParsura* parsura,
    constans SilvaNodus* subiectum, Piscina* piscina)
{
    Extentus ext;
    Xar* series = xar_creare(piscina, magnitudo(SilvaValor));
    i32 cursor = ZEPHYRUM;
    i32 k;

    ext.validus = FALSUM;
    ext.initium = ZEPHYRUM;
    ext.finis = ZEPHYRUM;
    ext.linea_prima = ZEPHYRUM;
    ext.linea_ultima = ZEPHYRUM;

    si (series == NIHIL) redde ext;
    per (k = ZEPHYRUM; k < subiectum->numerus_locorum; k++)
    {
        SilvaValor* novus = (SilvaValor*)xar_addere(series);

        si (novus != NIHIL) *novus = subiectum->loci[k];
    }

    dum (cursor < xar_numerus(series))
    {
        SilvaValor v = *(SilvaValor*)xar_obtinere(series, cursor);

        cursor++;

        si (v.genus == SILVA_VALOR_LISTA)
        {
            i32 m;

            per (m = ZEPHYRUM;
                 m < silva_valor_lista_numerus(v); m++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, m);
                SilvaValor* novus;

                si (elem == NIHIL) perge;
                novus = (SilvaValor*)xar_addere(series);
                si (novus != NIHIL) *novus = *elem;
            }
            perge;
        }
        si (v.genus == SILVA_VALOR_TOKEN && v.datum.token != NIHIL)
        {
            SilvaToken* radix_lex = silva_token_radix(v.datum.token);

            si (radix_lex != NIHIL
                && radix_lex->fons_index == parsura->fons_princeps
                && radix_lex->byte_offset >= ZEPHYRUM)
            {
                s32 finis_lex = radix_lex->byte_offset
                    + (s32)radix_lex->longitudo;

                si (!ext.validus
                    || radix_lex->byte_offset < ext.initium)
                {
                    ext.initium = radix_lex->byte_offset;
                    ext.linea_prima = radix_lex->linea;
                }
                si (!ext.validus || finis_lex > ext.finis)
                {
                    ext.finis = finis_lex;
                    ext.linea_ultima = radix_lex->linea;
                }
                ext.validus = VERUM;
            }
            perge;
        }
        si (v.genus != SILVA_VALOR_NODUS || v.datum.nodus == NIHIL)
        {
            perge;
        }
        {
            constans SilvaNodus* nodus = v.datum.nodus;
            i32 m;

            per (m = ZEPHYRUM; m < nodus->numerus_locorum; m++)
            {
                SilvaValor* novus = (SilvaValor*)xar_addere(series);

                si (novus != NIHIL) *novus = nodus->loci[m];
            }
        }
    }
    redde ext;
}

/* -intra: maior-aut-ipse subiecti scopo congruit? */
hic_manens b32
_intra_scopum (constans SilvaQuaestio* q_scopus,
    constans SilvaNodus* nodus)
{
    constans SilvaNodus* n = nodus;

    dum (n != NIHIL)
    {
        si (silva_quaestio_congruit(q_scopus, n)) redde VERUM;
        n = n->pater;
    }
    redde FALSUM;
}

/* --------------------------------------------------
 * porta nativa (-proba): fixtura inclusa, numeri exacti,
 * selector malus reiectus. TENET aut exitus 2.
 * -------------------------------------------------- */

hic_manens s32
_numerus_probae (Piscina* piscina, constans SilvaParsura* parsura,
    constans character* selector)
{
    constans character* causa = NIHIL;
    SilvaQuaestio* q = silva_quaestio_compilare(piscina,
        &SILVA_C89_REGISTRUM, selector, &causa);
    Xar* resultata;

    si (q == NIHIL) redde -I;
    resultata = silva_quaestio_exsequi(q,
        parsura->commissio->radix, piscina);
    si (resultata == NIHIL) redde -I;
    redde (s32)xar_numerus(resultata);
}

hic_manens s32
_proba (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("selecta_proba",
        4194304);
    constans character* fons =
        "int a;\n"
        "int quadratum(int x) { return x * x; }\n"
        "void vacua(void) { int z; z = 1; }\n";
    SilvaParsura* parsura;
    constans character* causa = NIHIL;
    b32 fracta = FALSUM;
    structura {
        constans character* selector;
        s32                 numerus;
    } probanda[] = {
        { "definitio-functionis",         II },
        { "definitio-functionis:reddit",  I },
        { "declaratio",                   II },
        { "definitio-functionis:definit(quadratum) binarium", I },
        { "definitio-functionis:vocat(quadratum)", ZEPHYRUM }
    };
    i32 k;

    si (piscina == NIHIL)
    {
        fprintf(stderr, "selecta -proba: piscina deest\n");
        redde II;
    }
    parsura = silva_c89_parsare(piscina, "proba.c", fons,
        (i32)strlen(fons), NIHIL);
    si (parsura == NIHIL || !parsura->successus)
    {
        fprintf(stderr, "selecta -proba: fixtura sine arbore\n");
        redde II;
    }
    per (k = ZEPHYRUM;
         k < (i32)(magnitudo(probanda) / magnitudo(probanda[ZEPHYRUM]));
         k++)
    {
        s32 n = _numerus_probae(piscina, parsura,
            probanda[k].selector);

        si (n != probanda[k].numerus)
        {
            fprintf(stderr, "selecta -proba: %s -> %ld "
                "(exspectatum %ld)\n", probanda[k].selector,
                (longus)n, (longus)probanda[k].numerus);
            fracta = VERUM;
        }
    }
    /* selector malus = fractura clara */
    si (silva_quaestio_compilare(piscina, &SILVA_C89_REGISTRUM,
            "tag-fictum", &causa) != NIHIL
        || causa == NIHIL)
    {
        fprintf(stderr, "selecta -proba: selector malus "
            "non reiectus\n");
        fracta = VERUM;
    }
    piscina_destruere(piscina);
    si (fracta) redde II;
    imprimere("SELECTA PROBA TENET\n");
    redde ZEPHYRUM;
}

/* --------------------------------------------------
 * praeparatio capitum: exemplar emitte/censor (inclusio vera)
 * -------------------------------------------------- */

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
    Piscina* piscina;
    SilvaContextus* ctx;
    constans character* via = NIHIL;
    constans character* selector = NIHIL;
    constans character* symbolum_intra = NIHIL;
    FILE* pl;
    i8* fons;
    long mensura_l;
    i32 mensura;
    SilvaParsura* parsura;
    SilvaQuaestio* quaestio;
    SilvaQuaestio* q_scopus = NIHIL;
    Xar* resultata;
    constans character* causa = NIHIL;
    i32 congruentia = ZEPHYRUM;
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;
        }
        alioquin si (strcmp(argv[k], "-proba") == ZEPHYRUM)
        {
            redde _proba();
        }
        alioquin si (strcmp(argv[k], "-intra") == ZEPHYRUM
            && k + I < argc)
        {
            k++;
            symbolum_intra = argv[k];
        }
        alioquin si (via == NIHIL)
        {
            via = argv[k];
        }
        alioquin si (selector == NIHIL)
        {
            selector = argv[k];
        }
    }
    si (via == NIHIL || selector == NIHIL)
    {
        fprintf(stderr, "usus: selecta <plagula> '<selector>'"
            " [-intra <symbolum>] [-omnia] [-proba]"
            "  (ex radice repositorii)\n");
        redde II;
    }

    piscina_ctx = piscina_generare_dynamicum("selecta_ctx",
        8388608);
    si (piscina_ctx == NIHIL)
    {
        fprintf(stderr, "selecta: piscina deest\n");
        redde II;
    }
    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "selecta: contextus deest\n");
        redde II;
    }
    si (mensura_maxima == ZEPHYRUM)
    {
        ctx->fines.lexemata = ZEPHYRUM;
    }
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa == NIHIL)
        {
            fprintf(stderr, "selecta: tabula deest\n");
            redde II;
        }
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "selecta: plagula non lecta: %s\n", via);
        redde II;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde II;
    }
    mensura = (i32)mensura_l;
    si (mensura_maxima > ZEPHYRUM && mensura > mensura_maxima)
    {
        fprintf(stderr, "selecta: plagula super tectum %.1f MB "
            "(-omnia tollit)\n", (duplex)mensura / 1048576.0);
        redde II;
    }

    piscina = piscina_generare_dynamicum("selecta", 8388608);
    si (piscina == NIHIL)
    {
        fclose(pl);
        redde II;
    }
    fons = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        fprintf(stderr, "selecta: plagula non lecta: %s\n", via);
        redde II;
    }
    fclose(pl);

    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
        (constans character*)fons, mensura, NIHIL);
    si (parsura == NIHIL)
    {
        fprintf(stderr, "selecta: sine arbore: %s\n", via);
        redde II;
    }
    si (!parsura->successus)
    {
        /* arbor manet - nodi error quaeribiles (vinculum
         * robustitatis); cautio sola */
        fprintf(stderr, "selecta CAUTIO: arbor cum erroribus "
            "(%ld) in %s\n", (longus)parsura->numerus_errorum,
            via);
    }

    quaestio = silva_quaestio_compilare(piscina,
        &SILVA_C89_REGISTRUM, selector, &causa);
    si (quaestio == NIHIL)
    {
        fprintf(stderr, "selecta: selector reiectus: %s\n",
            causa != NIHIL ? causa : "(sine causa)");
        redde II;
    }
    si (symbolum_intra != NIHIL)
    {
        character selector_scopi[DXII];

        si (strlen(symbolum_intra) + XL >= DXII)
        {
            fprintf(stderr, "selecta: symbolum -intra nimis "
                "longum\n");
            redde II;
        }
        sprintf(selector_scopi,
            "definitio-functionis:definit(%s)", symbolum_intra);
        q_scopus = silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, selector_scopi, &causa);
        si (q_scopus == NIHIL)
        {
            fprintf(stderr, "selecta: -intra reiectum: %s\n",
                causa != NIHIL ? causa : "(sine causa)");
            redde II;
        }
    }

    resultata = silva_quaestio_exsequi(quaestio,
        parsura->commissio->radix, piscina);
    si (resultata == NIHIL)
    {
        fprintf(stderr, "selecta: exsecutio fracta\n");
        redde II;
    }

    {
        i32 m;

        per (m = ZEPHYRUM; m < xar_numerus(resultata); m++)
        {
            constans SilvaQuaestioResultatum* r =
                (constans SilvaQuaestioResultatum*)xar_obtinere(
                    resultata, m);
            Extentus ext;
            s32 finis_lineae;

            si (r == NIHIL || r->nodus == NIHIL) perge;
            si (q_scopus != NIHIL
                && !_intra_scopum(q_scopus, r->nodus))
            {
                perge;
            }
            ext = _extentum_metiri(parsura, r->nodus, piscina);
            si (!ext.validus) perge;

            /* initium ad initium lineae retrahere */
            dum (ext.initium > ZEPHYRUM
                && fons[ext.initium - I] != '\n')
            {
                ext.initium--;
            }
            /* finem ad finem lineae extendere */
            finis_lineae = ext.finis;
            dum (finis_lineae < (s32)mensura
                && fons[finis_lineae] != '\n')
            {
                finis_lineae++;
            }
            si (finis_lineae < (s32)mensura) finis_lineae++;

            si (congruentia > ZEPHYRUM) imprimere("\n");
            imprimere("[%s:%u-%u]\n", via,
                ext.linea_prima, ext.linea_ultima);
            fwrite(fons + ext.initium, I,
                (memoriae_index)(finis_lineae - ext.initium),
                stdout);
            congruentia++;
        }
    }
    redde congruentia > ZEPHYRUM ? ZEPHYRUM : I;
}
