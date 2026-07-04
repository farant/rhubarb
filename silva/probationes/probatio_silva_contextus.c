/* probatio_silva_contextus.c - Contextus hospitis (Phase 7 Chunk A)
 *
 * SilvaContextus: fines (limina dura - volumen, generationes,
 * profunditas regionum/includendi, frons GSS), intermissio (pergere),
 * lexica (definitiones ante fontem - latina compilata!), praebenda.
 * PROPRIETAS REGENS: fines expansionem DEGRADANT, numquam
 * totalitatem - quaeque parsura sub finibus/intermissione arborem
 * completam reddit et octetim exacte reconstruitur.
 *
 * Clausura cursor.c per contextum (lexicon latinum compilatum +
 * praebenda) = vectis M1 via nova.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"
#include "silva_tabulae_sceleti.h"
#include "silva_glr.h"
#include "silva_expandere.h"
#include "silva_commissio.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_scribere.h"
#include "silva_latina_datum.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA 1024

hic_manens constans SilvaGrammatica GRAMMATICA_SCELETI = {
    &SILVA_SCELETUM_TABULA,
    &SILVA_SCELETUM_REGISTRUM,
    silva_sceletum_construere,
    silva_sceletum_ambiguum_fabricare,
    NIHIL
};

/* Plagulam integram legere */
interior i8*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl;
    i8* buffer;
    signatus longus mensura;

    pl = fopen(via, "rb");
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(pl);
        redde NIHIL;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    rewind(pl);

    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    {
        fclose(pl);
        redde NIHIL;
    }
    si (mensura > 0L
        && fread(buffer, I, (memoriae_index)mensura, pl)
            != (memoriae_index)mensura)
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);

    *mensura_out = (i32)mensura;
    redde buffer;
}

/* Parsare per contextum; parsuram reddit (NIHIL in fractura) */
interior SilvaParsura*
_parsare_ctx (Piscina* piscina, constans SilvaContextus* contextus,
    constans character* fons)
{
    redde silva_parsare_cum_contextu(piscina, contextus, "probatio.c",
        fons, (i32)strlen(fons), &GRAMMATICA_SCELETI, NIHIL, NIHIL,
        NIHIL);
}

/* Fidelitas per contextum: parsare -> scribere == fons.
 * parsura_out (si non NIHIL) parsuram recipit pro assertionibus. */
interior b32
_fidelis_ctx (Piscina* piscina, constans SilvaContextus* contextus,
    constans character* fons, SilvaParsura** parsura_out)
{
    SilvaParsura*  parsura;
    SilvaScriptura scriptura;
    i32 m = (i32)strlen(fons);

    parsura = _parsare_ctx(piscina, contextus, fons);
    si (parsura_out != NIHIL)
    {
        *parsura_out = parsura;
    }
    si (parsura == NIHIL || !parsura->successus)
    {
        redde FALSUM;
    }
    scriptura = silva_scribere_fontem(piscina, parsura,
        &SILVA_SCELETUM_REGISTRUM, parsura->fons_princeps);
    si (!scriptura.successus || scriptura.textus.mensura != m)
    {
        redde FALSUM;
    }
    si (m > ZEPHYRUM
        && memcmp(scriptura.textus.datum, fons,
               (memoriae_index)m) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde VERUM;
}

/* Interrogatio numerans: post N vocationes FALSUM (monotona) */
interior b32
_pergere_numerans (vacuum* datum)
{
    i32* reliquae = (i32*)datum;

    si (*reliquae == ZEPHYRUM)
    {
        redde FALSUM;
    }
    (*reliquae)--;
    redde VERUM;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_contextus",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: creare + defaltae (fines generosae SEMPER activae)
     * ======================================================== */
    {
        SilvaContextus* ctx;

        imprimere("\n--- Probans creare + defaltas ---\n");

        ctx = silva_contextus_creare(piscina);
        CREDO_NON_NIHIL (ctx);
        CREDO_AEQUALIS_I32 (ctx->fines.lexemata,
            SILVA_LIMEN_LEXEMATUM_DEFALTUM);
        CREDO_AEQUALIS_I32 (ctx->fines.generationes,
            SILVA_LIMEN_GENERATIONUM_DEFALTUM);
        CREDO_AEQUALIS_I32 (ctx->fines.profunditas_includendi,
            SILVA_LIMEN_INCLUDENDI_DEFALTUM);
        CREDO_AEQUALIS_I32 (ctx->fines.profunditas_regionum,
            SILVA_LIMEN_REGIONUM_DEFALTUM);
        CREDO_AEQUALIS_I32 (ctx->fines.frons,
            SILVA_GLR_LIMEN_FRONTIS_DEFALTUM);
        CREDO_AEQUALIS_I32 (xar_numerus(ctx->lexica), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(ctx->praebenda), ZEPHYRUM);
    }

    /* ========================================================
     * PROBARE: lexicon - definitiones ante fontem valent
     * (a positione 0; provenientia ad plagulam lexici)
     * ======================================================== */
    {
        SilvaContextus* ctx;
        SilvaParsura* parsura;

        imprimere("\n--- Probans lexicon ---\n");

        ctx = silva_contextus_creare(piscina);
        CREDO_VERUM (silva_contextus_lexicon_addere(ctx, "probata.h",
            "#define X 42\n", (i32)strlen("#define X 42\n")));

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, "X;", &parsura));
        CREDO_NON_NIHIL (parsura);
        si (parsura != NIHIL)
        {
            /* X vere expansum (definitio ex lexico) */
            CREDO_NON_NIHIL (silva_expansio_quaerere(parsura->expansio,
                chorda_ex_literis("X", piscina)));
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_FALSUM (parsura->est_intermissa);
            CREDO_FALSUM (parsura->fines_tactae);
        }
    }

    /* ========================================================
     * PROBARE: lexicon latinum COMPILATUM (silva_latina_datum) -
     * "integer x;" declaratio fit; datum == plagula in disco
     * ======================================================== */
    {
        SilvaContextus* ctx;
        SilvaParsura* parsura;
        constans character* radix_env;
        character via[VIA_MAXIMA];
        i8* latina_disco;
        i32 m_disco;

        imprimere("\n--- Probans latinam compilatam ---\n");

        ctx = silva_contextus_creare(piscina);
        CREDO_VERUM (silva_contextus_latinam_addere(ctx));

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, "integer x;",
            &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
            CREDO_NON_NIHIL (silva_expansio_quaerere(parsura->expansio,
                chorda_ex_literis("si", piscina)));
        }

        /* Datum compilatum == include/latina.h in disco (custos
         * derivae - amalgamator regenerat) */
        radix_env = getenv("RHUBARB_RADIX");
        si (radix_env == NIHIL)
        {
            radix_env = "..";
        }
        sprintf(via, "%s/include/latina.h", radix_env);
        latina_disco = _plagulam_legere(piscina, via, &m_disco);
        CREDO_NON_NIHIL (latina_disco);
        si (latina_disco != NIHIL)
        {
            CREDO_AEQUALIS_I32 (silva_latina_mensura, m_disco);
            CREDO_VERUM (memcmp(silva_latina_textus, latina_disco,
                (memoriae_index)m_disco) == ZEPHYRUM);
        }
    }

    /* ========================================================
     * PROBARE: limen lexematum - expansio exponentialis decisa,
     * arbor completa, reconstructio octetim exacta
     * ======================================================== */
    {
        SilvaContextus* ctx;
        SilvaParsura* parsura;
        constans character* fons =
            "#define A0 A1 A1\n"
            "#define A1 A2 A2\n"
            "#define A2 A3 A3\n"
            "#define A3 A4 A4\n"
            "#define A4 A5 A5\n"
            "#define A5 A6 A6\n"
            "#define A6 A7 A7\n"
            "#define A7 A8 A8\n"
            "#define A8 A9 A9\n"
            "#define A9 x\n"
            "A0;";

        imprimere("\n--- Probans limen lexematum ---\n");

        ctx = silva_contextus_creare(piscina);
        ctx->fines.lexemata = L;  /* 50 - decisio certa */

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, fons, &parsura));
        si (parsura != NIHIL)
        {
            CREDO_VERUM (parsura->expansio_decisa);
            CREDO_VERUM (parsura->fines_tactae);
            CREDO_FALSUM (parsura->est_intermissa);
        }
    }

    /* ========================================================
     * PROBARE: limen generationum - catena longa decisa
     * ======================================================== */
    {
        SilvaContextus* ctx;
        SilvaParsura* parsura;
        constans character* fons =
            "#define B0 B1\n"
            "#define B1 B2\n"
            "#define B2 B3\n"
            "#define B3 99\n"
            "B0;";

        imprimere("\n--- Probans limen generationum ---\n");

        ctx = silva_contextus_creare(piscina);
        ctx->fines.generationes = II;

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, fons, &parsura));
        si (parsura != NIHIL)
        {
            CREDO_VERUM (parsura->expansio_decisa);
        }

        /* Eadem catena sine limine arto: nulla decisio */
        ctx = silva_contextus_creare(piscina);
        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, fons, &parsura));
        si (parsura != NIHIL)
        {
            CREDO_FALSUM (parsura->expansio_decisa);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        }
    }

    /* ========================================================
     * PROBARE: limen regionum - regio ultra modum cruda manet,
     * reconstructio tenet; nidificatio profunda acervum non
     * perfodit (defalta LXIV)
     * ======================================================== */
    {
        SilvaContextus* ctx;
        SilvaParsura* parsura;
        constans character* fons =
            "#if 1\n"
            "#if 1\n"
            "#if 1\n"
            "int x;\n"
            "#endif\n"
            "#endif\n"
            "#endif\n";

        imprimere("\n--- Probans limen regionum ---\n");

        ctx = silva_contextus_creare(piscina);
        ctx->fines.profunditas_regionum = II;

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, fons, &parsura));
        si (parsura != NIHIL && parsura->expansio != NIHIL)
        {
            SilvaRegio* r1;
            SilvaRegio* r2;
            SilvaRegio* r3;

            CREDO_VERUM (parsura->fines_tactae);
            CREDO_AEQUALIS_I32 (
                xar_numerus(parsura->expansio->regiones), I);
            r1 = *(SilvaRegio**)xar_obtinere(
                parsura->expansio->regiones, ZEPHYRUM);
            CREDO_FALSUM (r1->est_ultra_modum);
            CREDO_AEQUALIS_I32 (xar_numerus(r1->filiae), I);
            r2 = *(SilvaRegio**)xar_obtinere(r1->filiae, ZEPHYRUM);
            CREDO_FALSUM (r2->est_ultra_modum);
            CREDO_AEQUALIS_I32 (xar_numerus(r2->filiae), I);
            r3 = *(SilvaRegio**)xar_obtinere(r2->filiae, ZEPHYRUM);
            CREDO_VERUM (r3->est_ultra_modum);
            /* interior crudus: int x; NUMQUAM parsatum. Post Chunk B
             * regiones TEXUNTUR: radix elementum unum fert (nodum
             * conditionalem exteriorem), non zephyrum */
            CREDO_AEQUALIS_I32 (
                silva_valor_lista_numerus(
                    parsura->commissio->radix), I);
            CREDO_AEQUALIS_I32 (parsura->regiones_textae, III);
            CREDO_AEQUALIS_I32 (parsura->regiones_omissae, ZEPHYRUM);
        }

        /* Nidificatio 2000 profunda sub defaltis: terminat, arbor
         * completa, octetim exacta (ante limen: acervus perfossus
         * esset) */
        {
            Piscina* piscina_nidi = piscina_generare_dynamicum(
                "nidificatio", 67108864);
            ChordaAedificator* aed;
            chorda textus;
            SilvaParsura* p;
            SilvaScriptura scriptura;
            i32 k;

            CREDO_NON_NIHIL (piscina_nidi);
            aed = chorda_aedificator_creare(piscina_nidi, 65536);
            per (k = ZEPHYRUM; k < MM; k++)
            {
                chorda_aedificator_appendere_literis(aed, "#if 1\n");
            }
            chorda_aedificator_appendere_literis(aed, "int x;\n");
            per (k = ZEPHYRUM; k < MM; k++)
            {
                chorda_aedificator_appendere_literis(aed, "#endif\n");
            }
            textus = chorda_aedificator_finire(aed);

            ctx = silva_contextus_creare(piscina_nidi);
            p = silva_parsare_cum_contextu(piscina_nidi, ctx,
                "nidus.c", (constans character*)textus.datum,
                (i32)textus.mensura, &GRAMMATICA_SCELETI, NIHIL,
                NIHIL, NIHIL);
            CREDO_NON_NIHIL (p);
            si (p != NIHIL)
            {
                CREDO_VERUM (p->successus);
                CREDO_VERUM (p->fines_tactae);
                scriptura = silva_scribere_fontem(piscina_nidi, p,
                    &SILVA_SCELETUM_REGISTRUM, p->fons_princeps);
                CREDO_VERUM (scriptura.successus);
                CREDO_AEQUALIS_I32 (scriptura.textus.mensura,
                    textus.mensura);
                CREDO_VERUM (memcmp(scriptura.textus.datum,
                    textus.datum,
                    (memoriae_index)textus.mensura) == ZEPHYRUM);
            }
            piscina_destruere(piscina_nidi);
        }
    }

    /* ========================================================
     * PROBARE: limen includendi per contextum
     * ======================================================== */
    {
        SilvaContextus* ctx;
        SilvaParsura* parsura;

        imprimere("\n--- Probans limen includendi ---\n");

        ctx = silva_contextus_creare(piscina);
        ctx->fines.profunditas_includendi = I;
        CREDO_VERUM (silva_contextus_praebere(ctx, "a.h",
            "#include \"b.h\"\n#define EX_A 1\n",
            (i32)strlen("#include \"b.h\"\n#define EX_A 1\n")));
        CREDO_VERUM (silva_contextus_praebere(ctx, "b.h",
            "#define EX_B 1\n", (i32)strlen("#define EX_B 1\n")));

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx,
            "#include \"a.h\"\nint x;\n", &parsura));
        si (parsura != NIHIL)
        {
            /* a.h processata (profunditas 1); b.h praetermissa */
            CREDO_NON_NIHIL (silva_expansio_quaerere(parsura->expansio,
                chorda_ex_literis("EX_A", piscina)));
            CREDO_NIHIL (silva_expansio_quaerere(parsura->expansio,
                chorda_ex_literis("EX_B", piscina)));
            CREDO_VERUM (parsura->fines_tactae);
        }
    }

    /* ========================================================
     * PROBARE: limen frontis - furca ultra limen = nodus ERROR
     * (totalitas), numeratus
     * ======================================================== */
    {
        SilvaContextus* ctx;
        SilvaParsura* parsura;

        imprimere("\n--- Probans limen frontis ---\n");

        ctx = silva_contextus_creare(piscina);
        ctx->fines.frons = I;  /* furca foo*bar frontem 2 petit */

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, "foo * bar;",
            &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->segmenta_ultra_limen, I);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, I);
            CREDO_VERUM (parsura->fines_tactae);
        }

        /* Defalta generosa: eadem furca sine fractura */
        ctx = silva_contextus_creare(piscina);
        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, "foo * bar;",
            &parsura));
        si (parsura != NIHIL)
        {
            CREDO_AEQUALIS_I32 (parsura->segmenta_ultra_limen,
                ZEPHYRUM);
            CREDO_AEQUALIS_I32 (parsura->numerus_errorum, ZEPHYRUM);
        }
    }

    /* ========================================================
     * PROBARE: intermissio - cauda nodus ERROR unus, arbor
     * completa, reconstructio octetim exacta
     * ======================================================== */
    {
        SilvaContextus* ctx;
        SilvaParsura* parsura;
        i32 reliquae;

        imprimere("\n--- Probans intermissionem ---\n");

        ctx = silva_contextus_creare(piscina);
        reliquae = V;
        silva_contextus_pergere_ponere(ctx, _pergere_numerans,
            &reliquae, I);

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx,
            "int a; int b; int c; int d;", &parsura));
        si (parsura != NIHIL)
        {
            CREDO_VERUM (parsura->est_intermissa);
            CREDO_VERUM (parsura->numerus_errorum >= I);
        }

        /* Intermissio statim (in expansione iam): cauda tota */
        ctx = silva_contextus_creare(piscina);
        reliquae = ZEPHYRUM;
        silva_contextus_pergere_ponere(ctx, _pergere_numerans,
            &reliquae, I);

        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, "int a; int b;",
            &parsura));
        si (parsura != NIHIL)
        {
            CREDO_VERUM (parsura->est_intermissa);
        }

        /* Sine interrogatione: nulla intermissio */
        ctx = silva_contextus_creare(piscina);
        parsura = NIHIL;
        CREDO_VERUM (_fidelis_ctx(piscina, ctx, "int a;", &parsura));
        si (parsura != NIHIL)
        {
            CREDO_FALSUM (parsura->est_intermissa);
        }
    }

    /* ========================================================
     * PROBARE: clausura cursor.c PER CONTEXTUM (vectis M1 via
     * nova) - lexicon latinum COMPILATUM + praebenda; #include
     * "latina.h" ignotum manet (linea reinserta) sed macros iam
     * valent; octetim exacta
     * ======================================================== */
    {
        constans character* radix_env;
        character via[VIA_MAXIMA];
        Piscina* piscina_cursoris;
        i8* cursor_c;
        i8* cursor_h;
        i8* color_h;
        i32 m_cursor_c;
        i32 m_cursor_h;
        i32 m_color_h;

        imprimere("\n--- Probans clausuram cursor.c per contextum ---\n");

        radix_env = getenv("RHUBARB_RADIX");
        si (radix_env == NIHIL)
        {
            radix_env = "..";
        }

        piscina_cursoris = piscina_generare_dynamicum(
            "clausura_contextus", 33554432);
        CREDO_NON_NIHIL (piscina_cursoris);

        sprintf(via, "%s/probationes/fixa/roundtrip/cursor.c",
            radix_env);
        cursor_c = _plagulam_legere(piscina_cursoris, via,
            &m_cursor_c);
        sprintf(via, "%s/probationes/fixa/roundtrip/cursor.h",
            radix_env);
        cursor_h = _plagulam_legere(piscina_cursoris, via,
            &m_cursor_h);
        sprintf(via, "%s/probationes/fixa/roundtrip/color.h",
            radix_env);
        color_h = _plagulam_legere(piscina_cursoris, via, &m_color_h);
        CREDO_NON_NIHIL (cursor_c);
        CREDO_NON_NIHIL (cursor_h);
        CREDO_NON_NIHIL (color_h);

        si (cursor_c != NIHIL && cursor_h != NIHIL && color_h != NIHIL)
        {
            SilvaContextus* ctx;
            SilvaParsura*   parsura;
            SilvaScriptura  scriptura;

            ctx = silva_contextus_creare(piscina_cursoris);
            CREDO_VERUM (silva_contextus_latinam_addere(ctx));
            CREDO_VERUM (silva_contextus_praebere(ctx, "cursor.h",
                (constans character*)cursor_h, m_cursor_h));
            CREDO_VERUM (silva_contextus_praebere(ctx, "color.h",
                (constans character*)color_h, m_color_h));

            parsura = silva_parsare_cum_contextu(piscina_cursoris,
                ctx, "cursor.c", (constans character*)cursor_c,
                m_cursor_c, &GRAMMATICA_SCELETI, NIHIL, NIHIL, NIHIL);
            CREDO_NON_NIHIL (parsura);
            si (parsura != NIHIL)
            {
                CREDO_VERUM (parsura->successus);
                CREDO_FALSUM (parsura->fines_tactae);

                /* verba clausa latina vere expansa (ex lexico
                 * compilato, non ex includendo) */
                CREDO_NON_NIHIL (silva_expansio_quaerere(
                    parsura->expansio,
                    chorda_ex_literis("si", piscina_cursoris)));

                scriptura = silva_scribere_fontem(piscina_cursoris,
                    parsura, &SILVA_SCELETUM_REGISTRUM,
                    parsura->fons_princeps);
                CREDO_VERUM (scriptura.successus);
                CREDO_AEQUALIS_I32 (scriptura.textus.mensura,
                    (i32)m_cursor_c);
                CREDO_VERUM (memcmp(scriptura.textus.datum, cursor_c,
                    (memoriae_index)m_cursor_c) == ZEPHYRUM);
            }
        }
        piscina_destruere(piscina_cursoris);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
