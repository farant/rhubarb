/* probatio_quaerere.c - Probationes pro fuzzy string matching
 */

#include "quaerere.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>


/* ========================================================================
 * PROBATIONES - SIMPLEX CONGRUENTIA
 * ======================================================================== */

interior vacuum
probatio_congruit_simplex(Piscina* piscina)
{
    chorda query;
    chorda target;

    (vacuum)piscina;

    printf("--- Probans congruit simplex ---\n");

    /* Exact match */
    query = chorda_ex_literis("abc", piscina);
    target = chorda_ex_literis("abc", piscina);
    CREDO_VERUM(quaerere_congruit(query, target));

    /* Subsequence match */
    query = chorda_ex_literis("abc", piscina);
    target = chorda_ex_literis("aXbXc", piscina);
    CREDO_VERUM(quaerere_congruit(query, target));

    /* Case insensitive by default */
    query = chorda_ex_literis("abc", piscina);
    target = chorda_ex_literis("ABC", piscina);
    CREDO_VERUM(quaerere_congruit(query, target));

    /* No match */
    query = chorda_ex_literis("xyz", piscina);
    target = chorda_ex_literis("abc", piscina);
    CREDO_FALSUM(quaerere_congruit(query, target));

    /* Empty query matches everything */
    query = chorda_ex_literis("", piscina);
    target = chorda_ex_literis("anything", piscina);
    CREDO_VERUM(quaerere_congruit(query, target));

    /* Query longer than target */
    query = chorda_ex_literis("abcdef", piscina);
    target = chorda_ex_literis("abc", piscina);
    CREDO_FALSUM(quaerere_congruit(query, target));

    printf("\n");
}


interior vacuum
probatio_congruit_case_sensitive(Piscina* piscina)
{
    chorda           query;
    chorda           target;
    QuaerereOptiones opt;

    (vacuum)piscina;

    printf("--- Probans congruit case sensitive ---\n");

    opt.case_sensitivus = VERUM;
    opt.intelligens_casus = FALSUM;

    /* Exact case matches */
    query = chorda_ex_literis("abc", piscina);
    target = chorda_ex_literis("abc", piscina);
    CREDO_VERUM(quaerere_congruit_cum_optionibus(query, target, &opt));

    /* Different case does not match */
    query = chorda_ex_literis("abc", piscina);
    target = chorda_ex_literis("ABC", piscina);
    CREDO_FALSUM(quaerere_congruit_cum_optionibus(query, target, &opt));

    printf("\n");
}


interior vacuum
probatio_congruit_smart_case(Piscina* piscina)
{
    chorda           query;
    chorda           target;
    QuaerereOptiones opt;

    (vacuum)piscina;

    printf("--- Probans congruit smart case ---\n");

    opt.case_sensitivus = FALSUM;
    opt.intelligens_casus = VERUM;

    /* Lowercase query: case insensitive */
    query = chorda_ex_literis("abc", piscina);
    target = chorda_ex_literis("ABC", piscina);
    CREDO_VERUM(quaerere_congruit_cum_optionibus(query, target, &opt));

    /* Uppercase in query: case sensitive */
    query = chorda_ex_literis("Abc", piscina);
    target = chorda_ex_literis("ABC", piscina);
    CREDO_FALSUM(quaerere_congruit_cum_optionibus(query, target, &opt));

    /* Uppercase in query matches exact */
    query = chorda_ex_literis("Abc", piscina);
    target = chorda_ex_literis("Abc", piscina);
    CREDO_VERUM(quaerere_congruit_cum_optionibus(query, target, &opt));

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - CONCORDARE CUM SCORING
 * ======================================================================== */

interior vacuum
probatio_concordare_exacta(Piscina* piscina)
{
    chorda          query;
    chorda          target;
    QuaerereFructus res;

    printf("--- Probans concordare exacta ---\n");

    query = chorda_ex_literis("abc", piscina);
    target = chorda_ex_literis("abc", piscina);
    res = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32(res.numerus, III);
    CREDO_NON_NIHIL(res.positiones);
    CREDO_AEQUALIS_I32(res.positiones[0], 0);
    CREDO_AEQUALIS_I32(res.positiones[I], I);
    CREDO_AEQUALIS_I32(res.positiones[II], II);

    printf("  Score pro exacta match: %d\n", res.puncta);
    CREDO_MAIOR_S32(res.puncta, 0);  /* Should have bonuses */

    printf("\n");
}


interior vacuum
probatio_concordare_subsequence(Piscina* piscina)
{
    chorda          query;
    chorda          target;
    QuaerereFructus res;

    printf("--- Probans concordare subsequence ---\n");

    query = chorda_ex_literis("abc", piscina);
    target = chorda_ex_literis("aXXbXXc", piscina);
    res = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32(res.numerus, III);
    CREDO_AEQUALIS_I32(res.positiones[0], 0);
    CREDO_AEQUALIS_I32(res.positiones[I], III);
    CREDO_AEQUALIS_I32(res.positiones[II], VI);

    printf("  Score pro subsequence: %d\n", res.puncta);

    printf("\n");
}


interior vacuum
probatio_concordare_consecutive_bonus(Piscina* piscina)
{
    chorda          query;
    chorda          target;
    QuaerereFructus res_consecutive;
    QuaerereFructus res_separated;

    printf("--- Probans consecutive bonus ---\n");

    query = chorda_ex_literis("abc", piscina);

    /* Consecutive */
    target = chorda_ex_literis("XXXabc", piscina);
    res_consecutive = quaerere_concordare(query, target, piscina);

    /* Separated */
    target = chorda_ex_literis("aXbXc", piscina);
    res_separated = quaerere_concordare(query, target, piscina);

    printf("  Consecutive score: %d\n", res_consecutive.puncta);
    printf("  Separated score: %d\n", res_separated.puncta);

    CREDO_AEQUALIS_I32((i32)res_consecutive.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32((i32)res_separated.status, (i32)QUAERERE_OK);
    CREDO_MAIOR_S32(res_consecutive.puncta, res_separated.puncta);

    printf("\n");
}


interior vacuum
probatio_concordare_word_boundary(Piscina* piscina)
{
    chorda          query;
    chorda          target;
    QuaerereFructus res;

    printf("--- Probans word boundary bonus ---\n");

    /* foo_bar with 'fb' should get boundary bonus for 'b' */
    query = chorda_ex_literis("fb", piscina);
    target = chorda_ex_literis("foo_bar", piscina);
    res = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32(res.positiones[0], 0);  /* 'f' at start */
    CREDO_AEQUALIS_I32(res.positiones[I], IV); /* 'b' after '_' */

    printf("  Score pro word boundary: %d\n", res.puncta);

    printf("\n");
}


interior vacuum
probatio_concordare_camelcase(Piscina* piscina)
{
    chorda          query;
    chorda          target;
    QuaerereFructus res;

    printf("--- Probans CamelCase boundary ---\n");

    query = chorda_ex_literis("cc", piscina);
    target = chorda_ex_literis("CamelCase", piscina);
    res = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32(res.positiones[0], 0);  /* 'C' at start */
    CREDO_AEQUALIS_I32(res.positiones[I], V);  /* 'C' at CamelCase boundary */

    printf("  Score pro CamelCase: %d\n", res.puncta);

    printf("\n");
}


interior vacuum
probatio_concordare_path(Piscina* piscina)
{
    chorda          query;
    chorda          target;
    QuaerereFructus res;

    printf("--- Probans path matching ---\n");

    query = chorda_ex_literis("flth", piscina);
    target = chorda_ex_literis("lib/flatura.h", piscina);
    res = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);

    printf("  Score pro path: %d\n", res.puncta);
    printf("  Positions: ");
    {
        i32 i;
        per (i = 0; i < res.numerus; i++)
        {
            printf("%d ", res.positiones[i]);
        }
    }
    printf("\n");

    printf("\n");
}


interior vacuum
probatio_concordare_non_congruit(Piscina* piscina)
{
    chorda          query;
    chorda          target;
    QuaerereFructus res;

    printf("--- Probans non congruit ---\n");

    query = chorda_ex_literis("xyz", piscina);
    target = chorda_ex_literis("abc", piscina);
    res = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_NON_CONGRUIT);
    CREDO_NIHIL(res.positiones);

    printf("\n");
}


interior vacuum
probatio_concordare_vacuus(Piscina* piscina)
{
    chorda          query;
    chorda          target;
    QuaerereFructus res;

    printf("--- Probans query vacuus ---\n");

    query = chorda_ex_literis("", piscina);
    target = chorda_ex_literis("anything", piscina);
    res = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_S32(res.puncta, 0);
    CREDO_AEQUALIS_I32(res.numerus, 0);

    printf("\n");
}


/* ========================================================================
 * PROBATIONES - BATCH FILTERING
 * ======================================================================== */

interior vacuum
probatio_filtrare_simplex(Piscina* piscina)
{
    chorda                 query;
    chorda                 candidati[V];
    QuaerereFiltrumFructus res;

    printf("--- Probans filtrare simplex ---\n");

    candidati[0] = chorda_ex_literis("chorda.h", piscina);
    candidati[I] = chorda_ex_literis("piscina.h", piscina);
    candidati[II] = chorda_ex_literis("chorda.c", piscina);
    candidati[III] = chorda_ex_literis("latina.h", piscina);
    candidati[IV] = chorda_ex_literis("credo.h", piscina);

    query = chorda_ex_literis("ch", piscina);
    res = quaerere_filtrare(query, candidati, V, piscina);

    printf("  Query: \"ch\"\n");
    printf("  Matches: %d\n", res.numerus);

    CREDO_AEQUALIS_I32(res.numerus, IV);  /* chorda.h, chorda.c, credo.h, piscina.h */

    {
        i32 i;
        per (i = 0; i < res.numerus; i++)
        {
            printf("    [%d] %.*s (score=%d)\n",
                   i,
                   res.elementa[i].mensura,
                   res.elementa[i].datum,
                   res.puncta[i]);
        }
    }

    printf("\n");
}


interior vacuum
probatio_filtrare_sorted(Piscina* piscina)
{
    chorda                 query;
    chorda                 candidati[III];
    QuaerereFiltrumFructus res;

    printf("--- Probans filtrare sorted ---\n");

    /* Same query, different match quality */
    candidati[0] = chorda_ex_literis("axxxxb", piscina);  /* Worst: big gap */
    candidati[I] = chorda_ex_literis("a_b", piscina);     /* Medium: boundary */
    candidati[II] = chorda_ex_literis("ab", piscina);     /* Best: consecutive */

    query = chorda_ex_literis("ab", piscina);
    res = quaerere_filtrare(query, candidati, III, piscina);

    CREDO_AEQUALIS_I32(res.numerus, III);

    /* Should be sorted by score descending */
    printf("  Sorted results:\n");
    {
        i32 i;
        per (i = 0; i < res.numerus; i++)
        {
            printf("    [%d] %.*s (score=%d)\n",
                   i,
                   res.elementa[i].mensura,
                   res.elementa[i].datum,
                   res.puncta[i]);
        }
    }

    /* Best match should be first */
    CREDO_VERUM(res.puncta[0] >= res.puncta[I]);
    CREDO_VERUM(res.puncta[I] >= res.puncta[II]);

    printf("\n");
}


interior vacuum
probatio_filtrare_empty_result(Piscina* piscina)
{
    chorda                 query;
    chorda                 candidati[II];
    QuaerereFiltrumFructus res;

    printf("--- Probans filtrare empty result ---\n");

    candidati[0] = chorda_ex_literis("abc", piscina);
    candidati[I] = chorda_ex_literis("def", piscina);

    query = chorda_ex_literis("xyz", piscina);
    res = quaerere_filtrare(query, candidati, II, piscina);

    CREDO_AEQUALIS_I32(res.numerus, 0);

    printf("\n");
}


interior vacuum
probatio_filtrare_empty_query(Piscina* piscina)
{
    chorda                 query;
    chorda                 candidati[III];
    QuaerereFiltrumFructus res;

    printf("--- Probans filtrare empty query ---\n");

    candidati[0] = chorda_ex_literis("a", piscina);
    candidati[I] = chorda_ex_literis("b", piscina);
    candidati[II] = chorda_ex_literis("c", piscina);

    query = chorda_ex_literis("", piscina);
    res = quaerere_filtrare(query, candidati, III, piscina);

    /* Empty query matches all */
    CREDO_AEQUALIS_I32(res.numerus, III);

    /* All scores should be 0 */
    CREDO_AEQUALIS_S32(res.puncta[0], 0);
    CREDO_AEQUALIS_S32(res.puncta[I], 0);
    CREDO_AEQUALIS_S32(res.puncta[II], 0);

    printf("\n");
}


/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

integer
principale(vacuum)
{
    Piscina* piscina;
    b32      successus;

    printf("\n");
    printf("========================================\n");
    printf("PROBATIONES QUAERERE (Fuzzy Matching)\n");
    printf("========================================\n");
    printf("\n");

    piscina = piscina_generare_dynamicum("probatio_quaerere", CXXVIII * M);
    credo_aperire(piscina);

    /* Simplex congruentia */
    probatio_congruit_simplex(piscina);
    probatio_congruit_case_sensitive(piscina);
    probatio_congruit_smart_case(piscina);

    /* Concordare cum scoring */
    probatio_concordare_exacta(piscina);
    probatio_concordare_subsequence(piscina);
    probatio_concordare_consecutive_bonus(piscina);
    probatio_concordare_word_boundary(piscina);
    probatio_concordare_camelcase(piscina);
    probatio_concordare_path(piscina);
    probatio_concordare_non_congruit(piscina);
    probatio_concordare_vacuus(piscina);

    /* Batch filtering */
    probatio_filtrare_simplex(piscina);
    probatio_filtrare_sorted(piscina);
    probatio_filtrare_empty_result(piscina);
    probatio_filtrare_empty_query(piscina);

    credo_imprimere_compendium();

    printf("========================================\n");
    printf("\n");

    successus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    redde successus ? 0 : I;
}
