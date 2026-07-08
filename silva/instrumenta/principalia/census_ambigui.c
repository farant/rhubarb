/* census_ambigui.c - THROWAWAY (investigatio semantica M0): pro
 * plagula UNA, contextu pleno (idem quod percursus - omne caput
 * repositorii praebitum), omnem nodum AMBIGUUM retentum inspicere
 * et lexema DISCRIMINANS (typus-nominatus, aut primum identificator)
 * emittere. Histogramma ad stdout: quot vices quisque titulus
 * ambiguus manet.
 *
 * Usus: census_ambigui <via.c>
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
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_c89_semantica.h"
#include "silva_tabulae_c89.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>

hic_manens i32 capita_praebita = ZEPHYRUM;

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
    si (mensura_l < 0L) { fclose(pl); redde; }
    mensura = (i32)mensura_l;
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (textus == NIHIL || (mensura > ZEPHYRUM
        && fread(textus, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl); redde;
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
            >= magnitudo(via_plena)) perge;
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

/* Primum lexema TYPUS_NOMINATUS in subarbore (titulus discriminans
 * lectionis declarationis); si nullum, primum IDENTIFICATOR. */
hic_manens SilvaToken*
_discriminans_invenire (constans SilvaNodus* nodus, i32 prof,
    b32 solum_nominatus)
{
    i32 i;

    si (nodus == NIHIL || prof > LXIV) redde NIHIL;

    si (nodus->genus == (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS)
    {
        SilvaValor t = silva_c89_typus_nominatus_tok_titulus(nodus);

        si (t.genus == SILVA_VALOR_TOKEN && t.datum.token != NIHIL)
        {
            redde t.datum.token;
        }
    }

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (v->genus == SILVA_VALOR_NODUS)
        {
            SilvaToken* r = _discriminans_invenire(v->datum.nodus,
                prof + I, solum_nominatus);

            si (r != NIHIL) redde r;
        }
        alioquin si (v->genus == SILVA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM;
                 k < silva_valor_lista_numerus(*v); k++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(*v, k);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS)
                {
                    SilvaToken* r = _discriminans_invenire(
                        e->datum.nodus, prof + I, solum_nominatus);

                    si (r != NIHIL) redde r;
                }
                alioquin si (!solum_nominatus && e != NIHIL
                    && e->genus == SILVA_VALOR_TOKEN
                    && e->datum.token != NIHIL
                    && e->datum.token->genus
                        == SILVA_LEX_IDENTIFICATOR)
                {
                    redde e->datum.token;
                }
            }
        }
        alioquin si (!solum_nominatus
            && v->genus == SILVA_VALOR_TOKEN
            && v->datum.token != NIHIL
            && v->datum.token->genus == SILVA_LEX_IDENTIFICATOR)
        {
            redde v->datum.token;
        }
    }
    redde NIHIL;
}

int principale (int argc, char** argv)
{
    Piscina* piscina;
    Piscina* piscina_ctx;
    SilvaContextus* ctx;
    SilvaParsura* parsura;
    FILE* pl;
    constans character* via;
    i8* fons;
    long mensura_l;
    i32 mensura;
    i32 amb;
    i32 i;
    SilvaOraculum* oraculum;
    SilvaParsura* systema_parsura = NIHIL;
    SilvaSemantica* systema_semantica = NIHIL;

    si (argc < II)
    {
        fprintf(stderr, "usus: census_ambigui <via.c>\n");
        redde I;
    }
    via = argv[I];

    pl = fopen(via, "rb");
    si (pl == NIHIL) { fprintf(stderr, "aperire non potest\n"); redde I; }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L) { fclose(pl); redde I; }
    mensura = (i32)mensura_l;

    piscina = piscina_generare_dynamicum("census", 8388608);
    piscina_ctx = piscina_generare_dynamicum("census_ctx", 8388608);
    si (piscina == NIHIL || piscina_ctx == NIHIL) { fclose(pl); redde I; }
    fons = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl); redde I;
    }
    fclose(pl);

    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL) redde I;
    ctx->fines.lexemata = ZEPHYRUM;  /* sine tecto (ut -omnia) */
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa != NIHIL) _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    /* fistula plena (Chunk C): systema + praeoneratio + clausura -
     * residuum SOLUM imprimitur (indecisa post clausuram) */
    {
        FILE* pl_sys = fopen("silva/fontes/systema_c89.h", "rb");
        long m_sys;
        character* f_sys;
        SilvaParsura* p_sys;

        si (pl_sys == NIHIL)
        {
            fprintf(stderr, "systema deest (curre ex radice)\n");
            redde I;
        }
        fseek(pl_sys, 0L, SEEK_END);
        m_sys = ftell(pl_sys);
        fseek(pl_sys, 0L, SEEK_SET);
        f_sys = (character*)piscina_allocare(piscina_ctx,
            (memoriae_index)(m_sys + 1L));
        si (fread(f_sys, I, (memoriae_index)m_sys, pl_sys)
            != (memoriae_index)m_sys)
        {
            fclose(pl_sys); redde I;
        }
        fclose(pl_sys);
        p_sys = silva_c89_parsare(piscina_ctx, "systema_c89.h",
            f_sys, (i32)m_sys, NIHIL);
        si (p_sys == NIHIL) redde I;
        systema_semantica = silva_c89_semantica_analysare(
            piscina_ctx, p_sys);
        systema_parsura = p_sys;
    }
    oraculum = silva_oraculum_creare(piscina);
    (vacuum)silva_c89_semantica_oraculum_augere(systema_semantica,
        oraculum);

    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
        (constans character*)fons, mensura, oraculum);
    si (parsura == NIHIL || !parsura->successus)
    {
        imprimere("SINE ARBORE\n"); redde I;
    }
    {
        SilvaSemantica* sem =
            silva_c89_semantica_analysare_cum_systemate(piscina,
                parsura, systema_parsura);

        si (sem != NIHIL)
        {
            (vacuum)silva_c89_semantica_oraculum_augere(sem,
                oraculum);
        }
        silva_oraculum_responsa_vacare(oraculum);
    }
    amb = xar_numerus(parsura->commissio->ambigui);
    imprimere("# %s : %d capita praebita, %d retenta, residuum"
        " sequitur\n", via, (int)capita_praebita, (int)amb);

    per (i = ZEPHYRUM; i < amb; i++)
    {
        SilvaNodus** pp = (SilvaNodus**)xar_obtinere(
            parsura->commissio->ambigui, i);
        SilvaNodus* nodus;
        SilvaToken* disc;

        si (pp == NIHIL || *pp == NIHIL) perge;
        nodus = *pp;
        {
            SilvaResolutioResponsum responsum;

            responsum.victor = -I;
            responsum.discriminans = NIHIL;
            silva_c89_resolutor(nodus, oraculum, NIHIL, &responsum);
            si (responsum.victor >= ZEPHYRUM)
            {
                perge;   /* decisum - non residuum */
            }
        }
        disc = _discriminans_invenire(nodus, ZEPHYRUM, VERUM);
        si (disc == NIHIL)
        {
            disc = _discriminans_invenire(nodus, ZEPHYRUM, FALSUM);
        }
        {
            SilvaValor interp =
                silva_c89_ambiguus_interpretationes(nodus);
            i32 ni = silva_valor_lista_numerus(interp);
            i32 j;

            si (disc != NIHIL)
            {
                imprimere("%.*s\t:%u\t", (int)disc->valor.mensura,
                    (constans character*)disc->valor.datum,
                    disc->linea);
            }
            alioquin
            {
                imprimere("(SINE-DISC)\t:0\t");
            }
            per (j = ZEPHYRUM; j < ni; j++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(interp, j);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS
                    && e->datum.nodus != NIHIL)
                {
                    imprimere("g%d ", (int)e->datum.nodus->genus);
                }
            }
            imprimere("\n");
        }
    }
    redde ZEPHYRUM;
}
