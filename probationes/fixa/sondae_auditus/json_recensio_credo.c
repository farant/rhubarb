/* sonda recensionis: credo drafts for confirmed json findings - each should FAIL today */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "json.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
probatio_via_index_involutus(Piscina* piscina)
{
    JsonResultus res;

    imprimere("--- Probans json_via: index ultra 2^32 non involvitur ---\n");
    res = json_legere_literis("{\"a\":[\"zero\"],\"b\":[0,1]}", piscina);
    CREDO_VERUM(res.successus);
    /* 4294967296 = 2^32: hodie index mod 2^32 = 0 -> "zero" */
    CREDO_NIHIL(json_via(res.radix, "a.4294967296"));
    CREDO_NIHIL(json_via(res.radix, "a.42949672960"));
    /* intra fines et extra fines parvi immutati */
    CREDO_NON_NIHIL(json_via(res.radix, "a.0"));
    CREDO_NIHIL(json_via(res.radix, "a.1"));
    CREDO_AEQUALIS_I32((i32)json_ad_integer(json_via(res.radix, "b.1")), I);
}

interior vacuum
probatio_capere_non_internat(Piscina* piscina)
{
    JsonResultus res;
             i32 ante;
             i32 postea;
       character buffer[XXXII];
          chorda clavis;
             i32 k;

    imprimere("--- Probans capere clavis absentis tabulam internam non auget ---\n");
    res = json_legere_literis("{\"praesens\":1}", piscina);
    CREDO_VERUM(res.successus);
    ante = internamentum_numerus(internamentum_globale());
    per (k = ZEPHYRUM; k < C; k++)
    {
        sprintf(buffer, "absens_recensio_%u", (unsigned)k);
        CREDO_NIHIL(json_objectum_capere(res.radix, buffer));
        CREDO_FALSUM(json_objectum_habet(res.radix, buffer));
        clavis = chorda_ex_literis(buffer, piscina);
        CREDO_NIHIL(json_objectum_capere_chorda(res.radix, clavis));
    }
    postea = internamentum_numerus(internamentum_globale());
    CREDO_AEQUALIS_I32(postea - ante, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)json_ad_integer(
        json_objectum_capere(res.radix, "praesens")), I);
}

hic_manens chorda textus_multarum_clavium;

interior vacuum
_multas_claves_legere(Piscina* piscina)
{
    (vacuum)json_legere(textus_multarum_clavium, piscina);
}

interior vacuum
probatio_multae_claves_lineares(Piscina* piscina)
{
    ChordaAedificator* aed;
            character  buffer[XXXII];
                  i32  k;
         JsonResultus  res;

    imprimere("--- Probans objectum XL milium clavium cito legitur ---\n");
    aed = chorda_aedificator_creare(piscina, (memoriae_index)(XL * M * XVI));
    chorda_aedificator_appendere_character(aed, '{');
    per (k = ZEPHYRUM; k < XL * M; k++)
    {
        sprintf(buffer, "%s\"q%u\":0", k ? "," : "", (unsigned)k);
        chorda_aedificator_appendere_literis(aed, buffer);
    }
    chorda_aedificator_appendere_character(aed, '}');
    textus_multarum_clavium = chorda_aedificator_finire(aed);
    /* hodie quadraticum: -O2 ~2.8 s; linearis << 1 s */
    CREDO_NON_PENDET(_multas_claves_legere(piscina), M);
    /* duplicata adhuc refutatur post emendationem */
    res = json_legere_literis("{\"q0\":0,\"q1\":1,\"q0\":2}", piscina);
    CREDO_FALSUM(res.successus);
}

interior vacuum
probatio_fluitans_subnormalis_brevissimus(Piscina* piscina)
{
    imprimere("--- Probans scriptor: subnormales forma brevissima ---\n");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_scribere(json_fluitans_creare(
        piscina, 4.9406564584124654e-324), piscina), "5e-324");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_scribere(json_fluitans_creare(
        piscina, 9.8813129168249309e-324), piscina), "1e-323");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_scribere(json_fluitans_creare(
        piscina, 4.9406564584124654e-321), piscina), "4.94e-321");
    /* normales immutati */
    CREDO_CHORDA_AEQUALIS_LITERIS(json_scribere(json_fluitans_creare(
        piscina, 0.1), piscina), "0.1");
    CREDO_CHORDA_AEQUALIS_LITERIS(json_scribere(json_fluitans_creare(
        piscina, 2.2250738585072014e-308), piscina),
        "2.2250738585072014e-308");
}

interior vacuum
probatio_habet_valor_nihil(Piscina* piscina)
{
    JsonValor* obj;

    imprimere("--- Probans json_objectum_habet: clavis cum valore NIHIL existit ---\n");
    obj = json_objectum_creare(piscina);
    json_objectum_ponere(obj, "vacuus", NIHIL);
    CREDO_AEQUALIS_I32(json_objectum_numerus(obj), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(json_scribere(obj, piscina),
        "{\"vacuus\":null}");
    CREDO_VERUM(json_objectum_habet(obj, "vacuus"));
    CREDO_FALSUM(json_objectum_habet(obj, "absens"));
}

interior vacuum
probatio_genus_nihil_congruit(Piscina* piscina)
{
    JsonResultus res;
      JsonValor* absens;

    imprimere("--- Probans json_genus et json_est_nullum de NIHIL concordant ---\n");
    res = json_legere_literis("{\"n\":null}", piscina);
    CREDO_VERUM(res.successus);
    absens = json_objectum_capere(res.radix, "abest");
    CREDO_NIHIL(absens);
    CREDO_VERUM((json_genus(absens) == JSON_NULLUM)
        == (json_est_nullum(absens) != FALSUM));
    CREDO_VERUM(json_est_nullum(json_objectum_capere(res.radix, "n")));
    CREDO_VERUM(json_genus(json_objectum_capere(res.radix, "n"))
        == JSON_NULLUM);
}

int
main (void)
{
    Piscina* piscina = piscina_generare_dynamicum("sonda_recensio", 65536);
    credo_aperire(piscina);
    probatio_via_index_involutus(piscina);
    probatio_capere_non_internat(piscina);
    probatio_multae_claves_lineares(piscina);
    probatio_fluitans_subnormalis_brevissimus(piscina);
    probatio_habet_valor_nihil(piscina);
    probatio_genus_nihil_congruit(piscina);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? 0 : 1;
}
