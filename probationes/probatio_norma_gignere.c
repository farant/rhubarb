/* probatio_norma_gignere.c - oraculum mutuum (norma-plan-2 N4) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "norma.h"
#include "norma_gignere.h"

#include <stdio.h>
#include <string.h>

#define SEMINA CCC

interior JsonValor*
_ex_functione (
       Sors* sors,
    Piscina* p,
     vacuum* datum)
{
    (vacuum)sors;
    (vacuum)datum;
    redde json_chorda_creare_literis(p, "ex functione");
}

interior JsonValor*
_functio_prava (
       Sors* sors,
    Piscina* p,
     vacuum* datum)
{
    (vacuum)sors;
    (vacuum)datum;
    redde json_integer_creare(p, VII);   /* textus exspectatur */
}

interior Norma*
_schema_simplex (
    Piscina* p)
{
    constans character* constans licita[]  = { "a", "b", "c", NIHIL };
    Norma* o                               = norma_objectum(p);

    norma_campus(o, "id", norma_intra(norma_integer(p), 0, MMXLVIII),
        VERUM);
    norma_campus(o, "nomen", norma_longitudo(norma_textus(p), I, XL),
        FALSUM);
    norma_campus(o, "email", norma_forma(norma_textus(p), "email"),
        VERUM);
    norma_campus(o, "codex",
        norma_longitudo(norma_forma(norma_textus(p), "uuid"),
                                              XXXVI, XXXVI), VERUM);
    norma_campus(o, "tags", norma_longitudo(norma_tabulatum(p,
        norma_electio(norma_textus(p), licita)), 0, IV), FALSUM);
    norma_campus(o, "ratio", norma_intra_fluitans(norma_numerus(p), 0.1,
        0.3), VERUM);
    norma_campus(o, "nota", norma_aut_nullum(norma_textus(p)), FALSUM);
    norma_campus(o, "datum", norma_forma(norma_textus(p), "date-time"),
        VERUM);
    norma_campus(o, "origo", norma_gignens(norma_textus(p),
        _ex_functione, NIHIL), VERUM);
    redde o;
}

interior Norma*
_schema_responsi (
    Piscina* p)
{
    constans character* constans fines[] = { "end_turn", "tool_use",
        "max_tokens", NIHIL };
    Norma* textus   = norma_objectum(p);
    Norma* petitum  = norma_objectum(p);
    Norma* blocus   = norma_discrimen(p, "type");
    Norma* usus     = norma_objectum(p);
    Norma* r        = norma_objectum(p);

    norma_campus(textus, "text", norma_textus(p), VERUM);
    norma_campus(petitum, "id", norma_textus(p), VERUM);
    norma_campus(petitum, "name", norma_textus(p), VERUM);
    norma_campus(petitum, "input", norma_modus(norma_objectum(p),
        NORMA_APERTUM), VERUM);
    norma_variatio(blocus, "text", textus);
    norma_variatio(blocus, "tool_use", petitum);
    norma_campus(usus, "input_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(usus, "output_tokens", norma_intra(norma_integer(p), 0,
        M * M), VERUM);
    norma_campus(r, "id", norma_textus(p), VERUM);
    norma_campus(r, "content", norma_tabulatum(p, blocus), VERUM);
    norma_campus(r, "stop_reason", norma_electio(norma_textus(p),
        fines), VERUM);
    norma_campus(r, "usage", usus, VERUM);
    redde r;
}

/* scribere + relegere: fines fluitantes et effugia per filum vera */
interior JsonValor*
_iterum (
    JsonValor* v,
      Piscina* p)
{
    redde json_legere(json_scribere(v, p), p).radix;
}

/* reddit causas INVALIDA visas (bit per NormaCausa): tegumentum
 * mensuratur, non creditur */
interior i32
_oraculum (Norma* n, constans character* titulus, Piscina* p)
{
    i32 visae = 0;
    s64 semen;
    i32 fracti = 0;

    imprimere("\n--- Oraculum mutuum: %s (%u semina) ---\n", titulus,
        SEMINA);
    per (semen = 0; semen < SEMINA; semen++)
    {
         NormaGenitum t = norma_gignere(n, NORMA_TYPICA, semen, p);
         NormaGenitum f = norma_gignere(n, NORMA_FINES, semen, p);
         NormaGenitum x = norma_gignere(n, NORMA_INVALIDA, semen, p);
        NormaIudicium iudicium_typicum = norma_iudicare(n,
            _iterum(t.valor, p), p);
        NormaIudicium iudicium_finium = norma_iudicare(n,
            _iterum(f.valor, p), p);
        NormaIudicium iudicium_invalidum;

        si (!iudicium_typicum.validum || !iudicium_finium.validum)
        {
            fracti++;
            si (fracti <= III)
            {
                NormaVitium* v = (NormaVitium*)xar_obtinere(
                    iudicium_typicum.validum ? iudicium_finium.vitia : iudicium_typicum.vitia,
                    0);

                imprimere("  semen %ld %s: %.*s %s\n", (longus)semen,
                          iudicium_typicum.validum ? "FINES" : "TYPICA",
                          (integer)v->via.mensura,
                          (constans character*)v->via.datum,
                          norma_causa_descriptio(v->causa));
            }
            perge;
        }
        CREDO_NON_NIHIL(x.valor);
        si (!x.valor)
        {
            perge;
        }
        visae               |= (i32)I << (i32)x.causa_fracta;
        iudicium_invalidum  = norma_iudicare(n, _iterum(x.valor, p), p);
        si (   xar_numerus(iudicium_invalidum.vitia) != I
            || !chorda_aequalis(((NormaVitium*)xar_obtinere(iudicium_invalidum.vitia,
            0))->via, x.via_fracta)
            || ((NormaVitium*)xar_obtinere(iudicium_invalidum.vitia,
            0))->causa != x.causa_fracta)
        {
            fracti++;
            si (fracti <= III)
            {
                imprimere("  semen %ld INVALIDA: fracta %.*s (%s), vitia %u\n",
                          (longus)semen, (integer)x.via_fracta.mensura,
                          (constans character*)x.via_fracta.datum,
                          norma_causa_descriptio(x.causa_fracta),
                          xar_numerus(iudicium_invalidum.vitia));
            }
        }
    }
    CREDO_AEQUALIS_I32(fracti, 0);
    redde visae;
}

interior vacuum
probatio_determinismus_et_stabilitas(Piscina* p)
{
    Norma* a = norma_objectum(p);
    Norma* b = norma_objectum(p);
      s64  semen;

    imprimere("\n--- Probans determinismus et stabilitas ---\n");
    CREDO_CHORDA_AEQUALIS(
        json_scribere(norma_gignere(_schema_simplex(p), NORMA_TYPICA,
        XLII, p).valor, p),
        json_scribere(norma_gignere(_schema_simplex(p), NORMA_TYPICA,
        XLII, p).valor, p));
    norma_campus(a, "x", norma_integer(p), VERUM);
    norma_campus(a, "y", norma_textus(p), FALSUM);
    norma_campus(b, "x", norma_integer(p), VERUM);
    norma_campus(b, "w", norma_textus(p), FALSUM);    /* campus additus */
    norma_campus(b, "y", norma_textus(p), FALSUM);
    norma_modus(b, NORMA_APERTUM);
    per (semen = 0; semen < L; semen++)
    {
        JsonValor* va = norma_gignere(a, NORMA_TYPICA, semen, p).valor;
        JsonValor* vb = norma_gignere(b, NORMA_TYPICA, semen, p).valor;

        CREDO_CHORDA_AEQUALIS(json_scribere(json_objectum_capere(va,
            "x"), p),
                              json_scribere(json_objectum_capere(vb,
                              "x"), p));
        CREDO_VERUM((json_objectum_capere(va, "y") == NIHIL)
                    == (json_objectum_capere(vb, "y") == NIHIL));
        si (json_objectum_capere(va, "y"))
        {
            CREDO_CHORDA_AEQUALIS(json_scribere(json_objectum_capere(va,
                "y"), p),
                                  json_scribere(json_objectum_capere(vb,
                                  "y"), p));
        }
    }
}

interior vacuum
probatio_functiones(Piscina* p)
{
           Norma* o = norma_objectum(p);
    NormaGenitum  g;

    imprimere("\n--- Probans functiones gignentes ---\n");
    g = norma_gignere(_schema_simplex(p), NORMA_TYPICA, VII, p);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_ad_chorda(json_objectum_capere(g.valor,
        "origo")),
                                  "ex functione");
    /* officina prava ab oraculo capitur */
    norma_campus(o, "t", norma_gignens(norma_textus(p), _functio_prava,
        NIHIL), VERUM);
    g = norma_gignere(o, NORMA_TYPICA, I, p);
    CREDO_FALSUM(norma_iudicare(o, g.valor, p).validum);
    /* nihil violabile */
    CREDO_NIHIL(norma_gignere(norma_liberum(p), NORMA_INVALIDA, I,
        p).valor);
}

/* titulus sine functione: generator ordinarius, octetis idem
 * (norma-plan-3 A1) */
interior vacuum
probatio_titulus_sine_functione(Piscina* p)
{
    s64 semen;
    i32 diversa = 0;

    imprimere("\n--- Probans titulum sine functione ---\n");
    per (semen = 0; semen < L; semen++)
    {
        NormaGenitum a = norma_gignere(norma_gignens_titulus(
            norma_textus(p), "sententia"), NORMA_TYPICA, semen, p);
        NormaGenitum b = norma_gignere(norma_textus(p), NORMA_TYPICA,
            semen, p);

        si (!chorda_aequalis(json_scribere(a.valor, p),
                             json_scribere(b.valor, p)))
        {
            diversa++;
        }
    }
    CREDO_AEQUALIS_I32(diversa, 0);
}

s32
principale (vacuum)
{
    Piscina* p = piscina_generare_dynamicum("probatio_norma_gignere",
        M * M);
        b32 successus;

    credo_aperire(p);
    {
        /* omnes X causae violabiles ab oraculo exercitae (non solum
         * "nulla fracta" - quod etiam nihil probatum significare posset) */
        i32 visae = _oraculum(_schema_simplex(p), "simplex", p)
                  | _oraculum(_schema_responsi(p), "responsum", p);
        i32 omnes = ((i32)I << NORMA_CAUSA_GENUS) | ((i32)I
            << NORMA_CAUSA_DEEST)
                  | ((i32)I << NORMA_CAUSA_EXTRA) | ((i32)I
                      << NORMA_CAUSA_MINIMUM)
                  | ((i32)I << NORMA_CAUSA_MAXIMUM) | ((i32)I
                      << NORMA_CAUSA_LONGITUDO)
                  | ((i32)I << NORMA_CAUSA_ELECTIO) | ((i32)I
                      << NORMA_CAUSA_FORMA)
                  | ((i32)I << NORMA_CAUSA_VARIATIO) | ((i32)I
                      << NORMA_CAUSA_DISCRIMEN);

        CREDO_AEQUALIS_I32(visae, omnes);
    }
    probatio_determinismus_et_stabilitas(p);
    probatio_functiones(p);
    probatio_titulus_sine_functione(p);
    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(p);
    redde successus ? 0 : I;
}
