/* origo.c - ORIGO (instrumenta prima #3): fabula expansionis
 * lexematum ad positionem datam - "quid factum est ex eo quod
 * scripsi?" (promotum per sim-2: gressus macro-conscius M3 hoc
 * SEMPER rogat; ante hoc instrumentum responsum = fontes
 * expansoris legere).
 *
 * Usus: ./silva/origo.sh <plagula> <linea>[:<columna>]
 *   sine columna: omnia lexemata lineae (relationes halitus
 *   machinulae via:linea dant, columna absente!)
 *
 * Effusio per gregem (lexemata eiusdem invocationis radicis):
 *   [via:linea:col] 'scriptum' -> N lexemata:
 *     'expansum' <- macro_intimum <- macro_extimum
 *   aut '(fons - sine expansione)'.
 *
 * Solum plagula princeps quaeritur (positiones capitum inclusorum
 * = dilatio nominata v1).
 *
 * Exitus: 0 inventum | 1 nihil ad positionem | 2 sine arbore/usus
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "silva_token.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_c89_oraculum.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>   /* instrumentum solum - macOS/POSIX */

hic_manens i32 mensura_maxima = 4194304;  /* -omnia = sine tecto */

/* --------------------------------------------------
 * catena originis unius lexematis: 'valor' <- macra intima..extima
 * -------------------------------------------------- */

hic_manens vacuum
_catenam_imprimere (
    SilvaToken* t)
{
    SilvaToken* gradus    = t;
           b32  expansum  = FALSUM;

    imprimere("  '%.*s'", (int)t->valor.mensura,
        (constans character*)t->valor.datum);
    dum (gradus != NIHIL)
    {
        constans chorda* nm       = NIHIL;
             SilvaToken* sequens  = NIHIL;

        commutatio (gradus->origo.genus)
        {
            casus SILVA_ORIGO_EXPANSIO:
                nm = gradus->origo.datum.expansio.nomen_macro;
                sequens = gradus->origo.datum.expansio.invocatio;
                frange;
            casus SILVA_ORIGO_PASTA:
                nm = gradus->origo.datum.pasta.nomen_macro;
                sequens = gradus->origo.datum.pasta.sinister;
                frange;
            casus SILVA_ORIGO_CHORDA:
                nm = gradus->origo.datum.stringificatio.nomen_macro;
                sequens = gradus->origo.datum.stringificatio.primus;
                frange;
            casus SILVA_ORIGO_API:
                nm = gradus->origo.datum.api.nomen_macro;
                sequens = NIHIL;
                frange;
            ordinarius: /* FONS */
                nm = NIHIL;
                sequens = NIHIL;
                frange;
        }
        si (nm != NIHIL)
        {
            constans character* species = "?";

            commutatio (gradus->origo.genus)
            {
                casus SILVA_ORIGO_EXPANSIO: species = ""; frange;
                casus SILVA_ORIGO_PASTA:
                    species = " (pasta ##)"; frange;
                casus SILVA_ORIGO_CHORDA:
                    species = " (chorda #)"; frange;
                casus SILVA_ORIGO_API:
                    species = " (api)"; frange;
                ordinarius: frange;
            }
            imprimere(" <- %.*s%s", (int)nm->mensura,
                (constans character*)nm->datum, species);
            expansum = VERUM;
        }
        gradus = sequens;
    }
    si (!expansum)
    {
        imprimere(" (fons - sine expansione)");
    }
    imprimere("\n");
}

/* -------------------------------------------------- */

/* praeparatio capitum: exemplar percursus/censor/emitte */
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
    si (   textus == NIHIL || (mensura > ZEPHYRUM
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

s32
principale (
      integer   argc,
    character** argv)
{
    Piscina* piscina_ctx;
    Piscina* piscina;
    SilvaContextus* ctx;
    constans character* via      = NIHIL;
    constans character* positio  = NIHIL;
    i32 linea                    = ZEPHYRUM;
    i32 columna                  = ZEPHYRUM;          /* 0 = tota linea */
    FILE* pl;
    i8* fons;
    long mensura_l;
    i32 mensura;
    SilvaParsura* parsura;
    integer k;
    i32 inventa = ZEPHYRUM;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-omnia") == ZEPHYRUM)
        {
            mensura_maxima = ZEPHYRUM;
        }
        alioquin si (via == NIHIL)
        {
            via = argv[k];
        }
        alioquin si (positio == NIHIL)
        {
            positio = argv[k];
        }
    }
    si (via == NIHIL || positio == NIHIL)
    {
        fprintf(stderr, "usus: origo <plagula> <linea>[:<columna>]"
            "  (ex radice repositorii)\n");
        redde II;
    }
    {
        character* post = NIHIL;

        linea = (i32)strtol(positio, &post, X);
        si (post != NIHIL && *post == ':')
        {
            columna = (i32)strtol(post + I, NIHIL, X);
        }
    }
    si (linea == ZEPHYRUM)
    {
        fprintf(stderr, "origo: linea invalida: %s\n", positio);
        redde II;
    }

    piscina_ctx = piscina_generare_dynamicum("origo_ctx", 8388608);
    si (piscina_ctx == NIHIL)
    {
        fprintf(stderr, "origo: piscina deest\n");
        redde II;
    }
    ctx = silva_contextus_creare(piscina_ctx);
    si (ctx == NIHIL)
    {
        fprintf(stderr, "origo: contextus deest\n");
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
            fprintf(stderr, "origo: tabula deest\n");
            redde II;
        }
        _capita_praeparare(ctx, piscina_ctx, visa, ".");
    }

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        fprintf(stderr, "origo: plagula non lecta: %s\n", via);
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
        fprintf(stderr, "origo: plagula super tectum %.1f MB "
            "(-omnia tollit)\n", (duplex)mensura / 1048576.0);
        redde II;
    }

    piscina = piscina_generare_dynamicum("origo", 8388608);
    si (piscina == NIHIL)
    {
        fclose(pl);
        redde II;
    }
    fons = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (   fons == NIHIL || (mensura > ZEPHYRUM
        && fread(fons, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura))
    {
        fclose(pl);
        fprintf(stderr, "origo: plagula non lecta: %s\n", via);
        redde II;
    }
    fclose(pl);

    parsura = silva_c89_parsare_cum_contextu(piscina, ctx, via,
        (constans character*)fons, mensura, NIHIL);
    si (   parsura           == NIHIL || !parsura->successus
        || parsura->lexemata == NIHIL)
    {
        fprintf(stderr, "origo: sine arbore: %s\n", via);
        redde II;
    }

    /* fluxus expansus percurritur; lexemata per radicem originis
     * ad positionem quaesitam reducta - grex = invocatio radicis
     * eadem (caput gregis semel impressum) */
    {
        SilvaToken* radix_prior  = NIHIL;
               i32  n             =
                   xar_numerus(parsura->lexemata);
               i32 t;

        per (t = ZEPHYRUM; t < n; t++)
        {
            SilvaToken** cella =
                (SilvaToken**)xar_obtinere(parsura->lexemata, t);
            SilvaToken* lexema;
            SilvaToken* radix;

            si (cella == NIHIL || *cella == NIHIL) perge;
            lexema  = *cella;
            radix   = silva_token_radix(lexema);
            si (radix == NIHIL) perge;
            si (radix->fons_index != parsura->fons_princeps) perge;
            si (radix->linea != linea) perge;
            si (   columna != ZEPHYRUM
                && !(columna >= radix->columna
                     && columna < radix->columna
                         + radix->longitudo))
            {
                perge;
            }

            si (radix != radix_prior)
            {
                imprimere("[%s:%u:%u] '%.*s'\n", via, radix->linea,
                    radix->columna, (int)radix->valor.mensura,
                    (constans character*)radix->valor.datum);
                radix_prior = radix;
            }
            _catenam_imprimere(lexema);
            inventa++;
        }
    }

    si (inventa == ZEPHYRUM)
    {
        fprintf(stderr, "origo: nihil ad %s:%u%s\n", via,
            linea, columna != ZEPHYRUM ? " (columna data)" : "");
        redde I;
    }
    redde ZEPHYRUM;
}
