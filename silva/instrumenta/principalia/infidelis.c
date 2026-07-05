/* infidelis.c - instrumentum diagnosticum plagulae UNIUS (M2d
 * Chunk C): parsare per fistulam plenam, deinde referre:
 *
 *   - errores: omnis nodus ERROR cum linea + fragmento lexematum
 *   - fidelitas: scriptura == fons octetim; si non, PRIMA
 *     divergentia cum contextu utriusque lateris; si scriptura
 *     ipsa fracta (deferral nominatum, e.g. pasta), CAUSA
 *
 * Natum ex investigatione INFIDELIS arbor2_glr_tabula.c: percursus
 * "INFIDELIS" dixit ubi vere deferral pasta notum clamabat -
 * instrumentum hoc discrimen uno visu dat.
 *
 * Usus: ./silva/infidelis.sh <via> [-nudus|-capita]
 *   -nudus  = sine lexico latino (plagulae C anglicae)
 *   -capita = INCLUSIO VERA: omne caput repositorii praebetur
 *             (idem quod percursus D2 - a radice repositorii/cwd)
 *   -omnia  = fines expansionis sine tecto (fluxus 1M lexemata)
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
#include <stdio.h>
#include <string.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens i32 errores_visi = ZEPHYRUM;
hic_manens i32 capita_praebita = ZEPHYRUM;

/* Praepassus capitum (idem exemplar quod percursus D2): omne .h
 * sub radice praebere sub basename, primus vincit */
hic_manens vacuum
_caput_praebere (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via_capitis,
    constans character* titulus)
{
    FILE* pl;
    long mensura_l;
    i32 mensura;
    character* textus;
    chorda clavis;

    clavis = chorda_ex_literis(titulus, piscina);
    si (tabula_dispersa_continet(visa, clavis)) redde;

    pl = fopen(via_capitis, "rb");
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
        capita_praebita++;
    }
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
_capita_praeparare (SilvaContextus* ctx, Piscina* piscina,
    TabulaDispersa* visa, constans character* via_dir)
{
    DIR* dir = opendir(via_dir);
    structura dirent* introitus;

    si (dir == NIHIL) redde;
    dum ((introitus = readdir(dir)) != NIHIL)
    {
        character via_plena[1024];
        memoriae_index m;

        si (introitus->d_name[ZEPHYRUM] == '.') perge;
        si (_praetermittendum(introitus->d_name)) perge;
        si (strlen(via_dir) + strlen(introitus->d_name) + II
            >= magnitudo(via_plena))
        {
            perge;
        }
        sprintf(via_plena, "%s/%s", via_dir, introitus->d_name);
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
_fragmentum_imprimere (constans SilvaNodus* nodus)
{
    i32 i;
    i32 emissa = ZEPHYRUM;

    per (i = ZEPHYRUM; i < nodus->numerus_locorum
        && emissa < XII; i++)
    {
        SilvaValor lexemata = nodus->loci[i];
        i32 k;

        si (lexemata.genus != SILVA_VALOR_LISTA) perge;
        per (k = ZEPHYRUM; k < silva_valor_lista_numerus(lexemata)
            && emissa < XII; k++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(
                lexemata, k);

            si (elem == NIHIL || elem->genus != SILVA_VALOR_TOKEN
                || elem->datum.token == NIHIL)
            {
                perge;
            }
            imprimere("%.*s ",
                (int)elem->datum.token->valor.mensura,
                (constans character*)
                    elem->datum.token->valor.datum);
            emissa++;
        }
    }
}

hic_manens SilvaToken*
_lexema_primum (constans SilvaNodus* nodus)
{
    i32 i;

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

            si (elem != NIHIL && elem->genus == SILVA_VALOR_TOKEN
                && elem->datum.token != NIHIL)
            {
                redde elem->datum.token;
            }
        }
    }
    redde NIHIL;
}

hic_manens vacuum
_errores_referre (SilvaValor valor, i32 profunditas)
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
                _errores_referre(*elem, profunditas + I);
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
            SilvaToken* primum = _lexema_primum(nodus);

            errores_visi++;
            imprimere("  [ERROR");
            si (primum != NIHIL)
            {
                imprimere(" :%u", primum->linea);
            }
            imprimere("]  ");
            _fragmentum_imprimere(nodus);
            imprimere("\n");
            redde;  /* intra errorem non descendimus ultra */
        }
        per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
        {
            _errores_referre(nodus->loci[i], profunditas + I);
        }
    }
}

hic_manens vacuum
_regionem_imprimere (
    constans character* titulus,
    constans i8*        datum,
                   i32  mensura,
                   i32  locus)
{
    i32 a = locus > CXX ? locus - CXX : ZEPHYRUM;
    i32 b = locus + CXX < mensura ? locus + CXX : mensura;
    i32 i;

    imprimere("--- %s [%d..%d] ---\n", titulus, (int)a, (int)b);
    per (i = a; i < b; i++)
    {
        character c = (character)datum[i];

        si (i == locus) imprimere("<<<HIC>>>");
        si (c == '\n') imprimere("\\n\n");
        alioquin si (c == '\t') imprimere("\\t");
        alioquin si (c < ' ' || c > '~') imprimere("\\x%02x",
            (insignatus integer)(insignatus character)c);
        alioquin imprimere("%c", c);
    }
    imprimere("\n");
}

s32 principale (integer argc, character** argv)
{
    Piscina*        piscina;
    Piscina*        piscina_ctx;
    SilvaContextus* ctx;
    SilvaParsura*   parsura;
    SilvaScriptura  scriptura;
    FILE*           pl;
    constans character* via = NIHIL;
    b32             cum_latina = VERUM;
    b32             cum_capitibus = FALSUM;
    b32             sine_finibus = FALSUM;
    i8*             fons;
    longus          mensura_l;
    i32             mensura;
    i32             i;
    s32             locus;
    integer         k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-nudus") == ZEPHYRUM)
        {
            cum_latina = FALSUM;
        }
        alioquin si (strcmp(argv[k], "-capita") == ZEPHYRUM)
        {
            cum_capitibus = VERUM;
            cum_latina = FALSUM;
        }
        alioquin si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            sine_finibus = VERUM;
        }
        alioquin
        {
            via = argv[k];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: infidelis <via> [-nudus]\n");
        redde I;
    }

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "infidelis: aperire non potest: %s\n", via);
        redde I;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde I;
    }
    mensura = (i32)mensura_l;

    piscina = piscina_generare_dynamicum("infidelis", 8388608);
    piscina_ctx = piscina_generare_dynamicum("infidelis_ctx",
        1048576);
    si (piscina == NIHIL || piscina_ctx == NIHIL)
    {
        fclose(pl);
        redde I;
    }
    fons = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fprintf(stderr, "infidelis: lectio fracta\n");
        fclose(pl);
        redde I;
    }
    fclose(pl);

    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL)
    {
        redde I;
    }
    si (sine_finibus)
    {
        ctx->fines.lexemata = ZEPHYRUM;
    }
    si (cum_capitibus)
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(
            piscina_ctx, DXII);

        si (visa != NIHIL)
        {
            _capita_praeparare(ctx, piscina_ctx, visa, ".");
        }
    }
    alioquin si (cum_latina)
    {
        (vacuum)silva_contextus_latinam_addere(ctx);
    }

    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
        (constans character*)fons, mensura, NIHIL);
    si (parsura == NIHIL || !parsura->successus)
    {
        imprimere("SINE ARBORE\n");
        redde I;
    }
    imprimere("%s (%s): errores %d%s\n", via,
        cum_capitibus ? "capita" : (cum_latina ? "latina" : "nudus"),
        (int)parsura->numerus_errorum,
        parsura->fines_tactae ? "  [FINES TACTAE]" : "");
    si (parsura->numerus_errorum > ZEPHYRUM)
    {
        _errores_referre(parsura->commissio->radix, ZEPHYRUM);
    }

    scriptura = silva_scribere_fontem(piscina, parsura,
        &SILVA_C89_REGISTRUM, parsura->fons_princeps);
    si (!scriptura.successus)
    {
        imprimere("SCRIPTURA FRACTA: %s\n",
            scriptura.causa ? scriptura.causa : "(sine causa)");
        redde II;
    }

    locus = -I;
    {
        i32 minima = mensura < scriptura.textus.mensura
            ? mensura : scriptura.textus.mensura;

        per (i = ZEPHYRUM; i < minima; i++)
        {
            si (fons[i] != scriptura.textus.datum[i])
            {
                locus = (s32)i;
                frange;
            }
        }
        si (locus < ZEPHYRUM && mensura != scriptura.textus.mensura)
        {
            locus = (s32)minima;
        }
    }

    si (locus < ZEPHYRUM)
    {
        imprimere("FIDELIS (octetim aequalis, %d octeti)\n",
            (int)mensura);
        redde ZEPHYRUM;
    }

    imprimere("PRIMA DIVERGENTIA: offset %d (fons %d octeti,"
        " scriptura %d octeti)\n", (int)locus, (int)mensura,
        (int)scriptura.textus.mensura);
    _regionem_imprimere("FONS", fons, mensura, (i32)locus);
    _regionem_imprimere("SCRIPTURA", scriptura.textus.datum,
        scriptura.textus.mensura, (i32)locus);
    redde II;
}
