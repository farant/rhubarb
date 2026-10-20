/* probatio_quaerere.c - Probationes pro fuzzy string matching
 */

#include "quaerere.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>


/* ==================================================
 * PROBATIONES - SIMPLEX CONGRUENTIA
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;
     chorda  query;
     chorda  target;

    piscina = c->piscina;

    /* Exact match */
    query   = chorda_ex_literis("abc", piscina);
    target  = chorda_ex_literis("abc", piscina);
    CREDO_VERUM(quaerere_congruit(query, target));

    /* Subsequence match */
    query   = chorda_ex_literis("abc", piscina);
    target  = chorda_ex_literis("aXbXc", piscina);
    CREDO_VERUM(quaerere_congruit(query, target));

    /* Case insensitive by default */
    query   = chorda_ex_literis("abc", piscina);
    target  = chorda_ex_literis("ABC", piscina);
    CREDO_VERUM(quaerere_congruit(query, target));

    /* No match */
    query   = chorda_ex_literis("xyz", piscina);
    target  = chorda_ex_literis("abc", piscina);
    CREDO_FALSUM(quaerere_congruit(query, target));

    /* Empty query matches everything */
    query   = chorda_ex_literis("", piscina);
    target  = chorda_ex_literis("anything", piscina);
    CREDO_VERUM(quaerere_congruit(query, target));

    /* Query longer than target */
    query   = chorda_ex_literis("abcdef", piscina);
    target  = chorda_ex_literis("abc", piscina);
    CREDO_FALSUM(quaerere_congruit(query, target));
}


interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
             Piscina* piscina;
              chorda  query;
              chorda  target;
    QuaerereOptiones  opt;

    piscina = c->piscina;

    opt.case_sensitivus    = VERUM;
    opt.intelligens_casus  = FALSUM;

    /* Exact case matches */
    query   = chorda_ex_literis("abc", piscina);
    target  = chorda_ex_literis("abc", piscina);
    CREDO_VERUM(quaerere_congruit_cum_optionibus(query, target, &opt));

    /* Different case does not match */
    query   = chorda_ex_literis("abc", piscina);
    target  = chorda_ex_literis("ABC", piscina);
    CREDO_FALSUM(quaerere_congruit_cum_optionibus(query, target, &opt));
}


interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
             Piscina* piscina;
              chorda  query;
              chorda  target;
    QuaerereOptiones  opt;

    piscina = c->piscina;

    opt.case_sensitivus    = FALSUM;
    opt.intelligens_casus  = VERUM;

    /* Lowercase query: case insensitive */
    query   = chorda_ex_literis("abc", piscina);
    target  = chorda_ex_literis("ABC", piscina);
    CREDO_VERUM(quaerere_congruit_cum_optionibus(query, target, &opt));

    /* Uppercase in query: case sensitive */
    query   = chorda_ex_literis("Abc", piscina);
    target  = chorda_ex_literis("ABC", piscina);
    CREDO_FALSUM(quaerere_congruit_cum_optionibus(query, target, &opt));

    /* Uppercase in query matches exact */
    query   = chorda_ex_literis("Abc", piscina);
    target  = chorda_ex_literis("Abc", piscina);
    CREDO_VERUM(quaerere_congruit_cum_optionibus(query, target, &opt));
}


/* ==================================================
 * PROBATIONES - CONCORDARE CUM SCORING
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
            Piscina* piscina;
             chorda  query;
             chorda  target;
    QuaerereFructus  res;

    piscina  = c->piscina;
    query    = chorda_ex_literis("abc", piscina);
    target   = chorda_ex_literis("abc", piscina);
    res      = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32(res.numerus, III);
    CREDO_NON_NIHIL(res.positiones);
    CREDO_AEQUALIS_I32(res.positiones[0], 0);
    CREDO_AEQUALIS_I32(res.positiones[I], I);
    CREDO_AEQUALIS_I32(res.positiones[II], II);

    printf("  Score pro exacta match: %d\n", res.puncta);
    CREDO_MAIOR_S32(res.puncta, 0);  /* Should have bonuses */
}


interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
            Piscina* piscina;
             chorda  query;
             chorda  target;
    QuaerereFructus  res;

    piscina  = c->piscina;
    query    = chorda_ex_literis("abc", piscina);
    target   = chorda_ex_literis("aXXbXXc", piscina);
    res      = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32(res.numerus, III);
    CREDO_AEQUALIS_I32(res.positiones[0], 0);
    CREDO_AEQUALIS_I32(res.positiones[I], III);
    CREDO_AEQUALIS_I32(res.positiones[II], VI);

    printf("  Score pro subsequence: %d\n", res.puncta);
}


interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
            Piscina* piscina;
             chorda  query;
             chorda  target;
    QuaerereFructus  res_consecutive;
    QuaerereFructus  res_separated;

    piscina  = c->piscina;
    query    = chorda_ex_literis("abc", piscina);

    /* Consecutive */
    target           = chorda_ex_literis("XXXabc", piscina);
    res_consecutive  = quaerere_concordare(query, target, piscina);

    /* Separated */
    target         = chorda_ex_literis("aXbXc", piscina);
    res_separated  = quaerere_concordare(query, target, piscina);

    printf("  Consecutive score: %d\n", res_consecutive.puncta);
    printf("  Separated score: %d\n", res_separated.puncta);

    CREDO_AEQUALIS_I32((i32)res_consecutive.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32((i32)res_separated.status, (i32)QUAERERE_OK);
    CREDO_MAIOR_S32(res_consecutive.puncta, res_separated.puncta);
}


interior vacuum
_sectio_septima (
    CredoContextus* c)
{
            Piscina* piscina;
             chorda  query;
             chorda  target;
    QuaerereFructus  res;

    piscina = c->piscina;
    /* foo_bar with 'fb' should get boundary bonus for 'b' */
    query   = chorda_ex_literis("fb", piscina);
    target  = chorda_ex_literis("foo_bar", piscina);
    res     = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32(res.positiones[0], 0);  /* 'f' at start */
    CREDO_AEQUALIS_I32(res.positiones[I], IV); /* 'b' after '_' */

    printf("  Score pro word boundary: %d\n", res.puncta);
}


interior vacuum
_sectio_octava (
    CredoContextus* c)
{
            Piscina* piscina;
             chorda  query;
             chorda  target;
    QuaerereFructus  res;

    piscina  = c->piscina;
    query    = chorda_ex_literis("cc", piscina);
    target   = chorda_ex_literis("CamelCase", piscina);
    res      = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_I32(res.positiones[0], 0);  /* 'C' at start */
    /* 'C' at CamelCase boundary */
    CREDO_AEQUALIS_I32(res.positiones[I], V);

    printf("  Score pro CamelCase: %d\n", res.puncta);
}


interior vacuum
_sectio_nona (
    CredoContextus* c)
{
            Piscina* piscina;
             chorda  query;
             chorda  target;
    QuaerereFructus  res;

    piscina  = c->piscina;
    query    = chorda_ex_literis("flth", piscina);
    target   = chorda_ex_literis("lib/flatura.h", piscina);
    res      = quaerere_concordare(query, target, piscina);

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
}


interior vacuum
_sectio_decima (
    CredoContextus* c)
{
            Piscina* piscina;
             chorda  query;
             chorda  target;
    QuaerereFructus  res;

    piscina  = c->piscina;
    query    = chorda_ex_literis("xyz", piscina);
    target   = chorda_ex_literis("abc", piscina);
    res      = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_NON_CONGRUIT);
    CREDO_NIHIL(res.positiones);
}


interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
            Piscina* piscina;
             chorda  query;
             chorda  target;
    QuaerereFructus  res;

    piscina  = c->piscina;
    query    = chorda_ex_literis("", piscina);
    target   = chorda_ex_literis("anything", piscina);
    res      = quaerere_concordare(query, target, piscina);

    CREDO_AEQUALIS_I32((i32)res.status, (i32)QUAERERE_OK);
    CREDO_AEQUALIS_S32(res.puncta, 0);
    CREDO_AEQUALIS_I32(res.numerus, 0);
}


/* ==================================================
 * PROBATIONES - BATCH FILTERING
 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
                   Piscina* piscina;
                    chorda  query;
                    chorda  candidati[V];
    QuaerereFiltrumFructus  res;

    piscina         = c->piscina;
    candidati[0]    = chorda_ex_literis("chorda.h", piscina);
    candidati[I]    = chorda_ex_literis("piscina.h", piscina);
    candidati[II]   = chorda_ex_literis("chorda.c", piscina);
    candidati[III]  = chorda_ex_literis("latina.h", piscina);
    candidati[IV]   = chorda_ex_literis("credo.h", piscina);

    query  = chorda_ex_literis("ch", piscina);
    res    = quaerere_filtrare(query, candidati, V, piscina);

    printf("  Query: \"ch\"\n");
    printf("  Matches: %d\n", res.numerus);

    /* chorda.h, chorda.c, credo.h, piscina.h */
    CREDO_AEQUALIS_I32(res.numerus, IV);

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
}


interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
                   Piscina* piscina;
                    chorda  query;
                    chorda  candidati[III];
    QuaerereFiltrumFructus  res;

    piscina = c->piscina;
    /* Same query, different match quality */
    /* Worst: big gap */
    candidati[0]   = chorda_ex_literis("axxxxb", piscina);
    /* Medium: boundary */
    candidati[I]   = chorda_ex_literis("a_b", piscina);
    /* Best: consecutive */
    candidati[II]  = chorda_ex_literis("ab", piscina);

    query  = chorda_ex_literis("ab", piscina);
    res    = quaerere_filtrare(query, candidati, III, piscina);

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
}


interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
                   Piscina* piscina;
                    chorda  query;
                    chorda  candidati[II];
    QuaerereFiltrumFructus  res;

    piscina       = c->piscina;
    candidati[0]  = chorda_ex_literis("abc", piscina);
    candidati[I]  = chorda_ex_literis("def", piscina);

    query  = chorda_ex_literis("xyz", piscina);
    res    = quaerere_filtrare(query, candidati, II, piscina);

    CREDO_AEQUALIS_I32(res.numerus, 0);
}


interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
                   Piscina* piscina;
                    chorda  query;
                    chorda  candidati[III];
    QuaerereFiltrumFructus  res;

    piscina        = c->piscina;
    candidati[0]   = chorda_ex_literis("a", piscina);
    candidati[I]   = chorda_ex_literis("b", piscina);
    candidati[II]  = chorda_ex_literis("c", piscina);

    query  = chorda_ex_literis("", piscina);
    res    = quaerere_filtrare(query, candidati, III, piscina);

    /* Empty query matches all */
    CREDO_AEQUALIS_I32(res.numerus, III);

    /* All scores should be 0 */
    CREDO_AEQUALIS_S32(res.puncta[0], 0);
    CREDO_AEQUALIS_S32(res.puncta[I], 0);
    CREDO_AEQUALIS_S32(res.puncta[II], 0);
}


/* ==================================================
 * PRINCIPALE
 * ================================================== */

hic_manens constans CredoSectio SECTIONES[] = {
    { "congruit simplex", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "congruit case sensitive", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "congruit smart case", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "concordare exacta", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "concordare subsequence", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "consecutive bonus", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "word boundary bonus", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "CamelCase boundary", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "path matching", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non congruit", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "query vacuus", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "filtrare simplex", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "filtrare sorted", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "filtrare empty result", _sectio_decima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "filtrare empty query", _sectio_decima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("quaerere", SECTIONES);
}
