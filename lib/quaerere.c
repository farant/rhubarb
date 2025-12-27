/* quaerere.c - Fuzzy String Matching Implementation
 *
 * Algorithmus inspiratus ab fzf.
 * Subsequence matching cum scoring heuristics.
 */

#include "quaerere.h"


/* ========================================================================
 * HELPER FUNCTIONES
 * ======================================================================== */

/* Verificare si character est minuscula */
interior b32
_quaerere_est_minuscula(i8 c)
{
    redde c >= 'a' && c <= 'z';
}


/* Verificare si character est maiuscula */
interior b32
_quaerere_est_maiuscula(i8 c)
{
    redde c >= 'A' && c <= 'Z';
}


/* Convertere ad minusculam */
interior i8
_quaerere_ad_minusculam(i8 c)
{
    si (_quaerere_est_maiuscula(c))
    {
        redde (i8)(c + ('a' - 'A'));
    }
    redde c;
}


/* Verificare si character est separator (word boundary) */
interior b32
_quaerere_est_separator(i8 c)
{
    redde c == '/' || c == '_' || c == '-' ||
           c == '.' || c == ' ' || c == '\\';
}


/* Verificare si positio est limes verbi (word boundary) */
interior b32
_quaerere_est_limes_verbi(constans i8* datum, i32 pos)
{
    si (pos == 0)
    {
        redde VERUM;  /* Start of string */
    }

    /* After separator */
    si (_quaerere_est_separator(datum[pos - 1]))
    {
        redde VERUM;
    }

    /* CamelCase transition: lowercase followed by uppercase */
    si (_quaerere_est_minuscula(datum[pos - 1]) &&
        _quaerere_est_maiuscula(datum[pos]))
    {
        redde VERUM;
    }

    redde FALSUM;
}


/* Verificare si query habet maiusculam (pro smart case) */
interior b32
_quaerere_habet_maiusculam(chorda quaestio)
{
    i32 i;

    per (i = 0; i < quaestio.mensura; i++)
    {
        si (_quaerere_est_maiuscula(quaestio.datum[i]))
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}


/* Comparare characters cum case handling */
interior b32
_quaerere_char_congruit(i8 q, i8 s, b32 case_sensitivus)
{
    si (case_sensitivus)
    {
        redde q == s;
    }

    redde _quaerere_ad_minusculam(q) == _quaerere_ad_minusculam(s);
}


/* ========================================================================
 * OPTIONES
 * ======================================================================== */

QuaerereOptiones
quaerere_optiones_default(vacuum)
{
    QuaerereOptiones opt;

    opt.case_sensitivus = FALSUM;
    opt.intelligens_casus = VERUM;

    redde opt;
}


/* ========================================================================
 * SIMPLEX CONGRUENTIA
 * ======================================================================== */

b32
quaerere_congruit_cum_optionibus(
    chorda            quaestio,
    chorda            scopus,
    QuaerereOptiones* optiones)
{
    i32 qi;  /* Query index */
    i32 si_idx;  /* Scopus index */
    b32 case_sens;

    /* Empty query matches everything */
    si (quaestio.mensura == 0)
    {
        redde VERUM;
    }

    /* Query longer than target cannot match */
    si (quaestio.mensura > scopus.mensura)
    {
        redde FALSUM;
    }

    /* Determine case sensitivity */
    case_sens = FALSUM;
    si (optiones != NIHIL)
    {
        si (optiones->case_sensitivus)
        {
            case_sens = VERUM;
        }
        alioquin si (optiones->intelligens_casus)
        {
            case_sens = _quaerere_habet_maiusculam(quaestio);
        }
    }

    /* Forward scan: check if query is subsequence */
    qi = 0;
    per (si_idx = 0; si_idx < scopus.mensura && qi < quaestio.mensura; si_idx++)
    {
        si (_quaerere_char_congruit(quaestio.datum[qi], scopus.datum[si_idx], case_sens))
        {
            qi++;
        }
    }

    redde qi == quaestio.mensura;
}


b32
quaerere_congruit(
    chorda quaestio,
    chorda scopus)
{
    QuaerereOptiones opt;

    opt = quaerere_optiones_default();

    redde quaerere_congruit_cum_optionibus(quaestio, scopus, &opt);
}


/* ========================================================================
 * SCORING
 * ======================================================================== */

/* Calculare score pro positiones */
interior s32
_quaerere_calculare_puncta(
    chorda      quaestio,
    chorda      scopus,
    i32*        positiones,
    i32         numerus,
    b32         case_sensitivus)
{
    s32 puncta;
    i32 i;
    i32 pos;

    puncta = 0;

    per (i = 0; i < numerus; i++)
    {
        pos = positiones[i];

        /* Consecutive bonus */
        si (i > 0 && pos == positiones[i - 1] + 1)
        {
            puncta += QUAERERE_BONUS_CONSECUTIVUS;
        }

        /* Word boundary bonus */
        si (_quaerere_est_limes_verbi(scopus.datum, pos))
        {
            puncta += QUAERERE_BONUS_LIMES_VERBI;
        }

        /* Exact case bonus (only in case-insensitive mode) */
        si (!case_sensitivus &&
            quaestio.datum[i] == scopus.datum[pos])
        {
            puncta += QUAERERE_BONUS_CASUS_EXACTUS;
        }

        /* Gap penalty */
        si (i > 0)
        {
            s32 gap;
            gap = (s32)pos - (s32)positiones[i - 1] - 1;
            si (gap > 0)
            {
                puncta -= gap * (s32)QUAERERE_POENA_LACUNA;
            }
        }
    }

    redde puncta;
}


/* ========================================================================
 * FORWARD PASS
 * ======================================================================== */

/* Forward pass: find first matching positions (greedy) */
interior b32
_quaerere_passus_anterior(
    chorda quaestio,
    chorda scopus,
    i32*   positiones,
    b32    case_sensitivus)
{
    i32 qi;
    i32 si_idx;

    qi = 0;
    per (si_idx = 0; si_idx < scopus.mensura && qi < quaestio.mensura; si_idx++)
    {
        si (_quaerere_char_congruit(quaestio.datum[qi], scopus.datum[si_idx], case_sensitivus))
        {
            positiones[qi] = si_idx;
            qi++;
        }
    }

    redde qi == quaestio.mensura;
}


/* ========================================================================
 * BACKWARD PASS (OPTIMIZATION)
 * ======================================================================== */

/* Backward pass: try to find tighter clustering from end */
interior vacuum
_quaerere_passus_posterior(
    chorda quaestio,
    chorda scopus,
    i32*   positiones,
    b32    case_sensitivus)
{
    i32 qi;
    i32 si_idx;

    si (quaestio.mensura <= 1)
    {
        redde;  /* No optimization needed for 0-1 chars */
    }

    /* Start from last match position, scan backward */
    qi = quaestio.mensura - 1;
    si_idx = positiones[qi];

    dum (qi > 0)
    {
        si_idx--;

        si (si_idx < 0)
        {
            frange;
        }

        si (_quaerere_char_congruit(quaestio.datum[qi - 1],
                                    scopus.datum[si_idx],
                                    case_sensitivus))
        {
            /* Check if this position is better (tighter clustering) */
            si (si_idx > positiones[qi - 1])
            {
                positiones[qi - 1] = si_idx;
            }
            qi--;
        }
    }
}


/* ========================================================================
 * CONCORDARE
 * ======================================================================== */

QuaerereFructus
quaerere_concordare_cum_optionibus(
    chorda            quaestio,
    chorda            scopus,
    QuaerereOptiones* optiones,
    Piscina*          piscina)
{
    QuaerereFructus fructus;
    b32             case_sens;
    i32*            positiones;

    /* Handle empty query */
    si (quaestio.mensura == 0)
    {
        fructus.status = QUAERERE_OK;
        fructus.puncta = 0;
        fructus.positiones = NIHIL;
        fructus.numerus = 0;
        redde fructus;
    }

    /* Check for null piscina */
    si (piscina == NIHIL)
    {
        fructus.status = QUAERERE_FRACTA_ALLOCATIO;
        fructus.puncta = 0;
        fructus.positiones = NIHIL;
        fructus.numerus = 0;
        redde fructus;
    }

    /* Determine case sensitivity */
    case_sens = FALSUM;
    si (optiones != NIHIL)
    {
        si (optiones->case_sensitivus)
        {
            case_sens = VERUM;
        }
        alioquin si (optiones->intelligens_casus)
        {
            case_sens = _quaerere_habet_maiusculam(quaestio);
        }
    }

    /* Allocate positions array */
    positiones = (i32*)piscina_allocare(piscina,
                                        (memoriae_index)quaestio.mensura * (memoriae_index)magnitudo(i32));

    si (positiones == NIHIL)
    {
        fructus.status = QUAERERE_FRACTA_ALLOCATIO;
        fructus.puncta = 0;
        fructus.positiones = NIHIL;
        fructus.numerus = 0;
        redde fructus;
    }

    /* Forward pass */
    si (!_quaerere_passus_anterior(quaestio, scopus, positiones, case_sens))
    {
        fructus.status = QUAERERE_NON_CONGRUIT;
        fructus.puncta = 0;
        fructus.positiones = NIHIL;
        fructus.numerus = 0;
        redde fructus;
    }

    /* Backward pass optimization */
    _quaerere_passus_posterior(quaestio, scopus, positiones, case_sens);

    /* Calculate score */
    fructus.status = QUAERERE_OK;
    fructus.puncta = _quaerere_calculare_puncta(quaestio, scopus, positiones,
                                                 quaestio.mensura, case_sens);
    fructus.positiones = positiones;
    fructus.numerus = quaestio.mensura;

    redde fructus;
}


QuaerereFructus
quaerere_concordare(
    chorda   quaestio,
    chorda   scopus,
    Piscina* piscina)
{
    QuaerereOptiones opt;

    opt = quaerere_optiones_default();

    redde quaerere_concordare_cum_optionibus(quaestio, scopus, &opt, piscina);
}


/* ========================================================================
 * BATCH FILTERING
 * ======================================================================== */

/* Internal structure for sorting */
nomen structura {
    s32 puncta;
    i32 index;
} _QuaererePar;


/* Sort pairs by score descending (simple insertion sort) */
interior vacuum
_quaerere_ordinare(_QuaererePar* paria, i32 numerus)
{
    i32 i;
    s32 j;  /* Debet esse signatus pro comparatio j >= 0 */
    _QuaererePar temp;

    per (i = 1; i < numerus; i++)
    {
        temp = paria[i];
        j = (s32)i - 1;

        dum (j >= 0 && paria[j].puncta < temp.puncta)
        {
            paria[j + 1] = paria[j];
            j--;
        }

        paria[j + 1] = temp;
    }
}


QuaerereFiltrumFructus
quaerere_filtrare_cum_optionibus(
    chorda            quaestio,
    chorda*           candidati,
    i32               numerus_candidatorum,
    QuaerereOptiones* optiones,
    Piscina*          piscina)
{
    QuaerereFiltrumFructus fructus;
    _QuaererePar*          paria;
    i32                    numerus_congruentium;
    i32                    i;
    QuaerereFructus        match_result;

    /* Initialize result */
    fructus.elementa = NIHIL;
    fructus.puncta = NIHIL;
    fructus.numerus = 0;

    si (piscina == NIHIL || candidati == NIHIL || numerus_candidatorum <= 0)
    {
        redde fructus;
    }

    /* Allocate temporary array for matching pairs */
    paria = (_QuaererePar*)piscina_allocare(piscina,
                                            (memoriae_index)numerus_candidatorum * (memoriae_index)magnitudo(_QuaererePar));

    si (paria == NIHIL)
    {
        redde fructus;
    }

    /* Match each candidate */
    numerus_congruentium = 0;
    per (i = 0; i < numerus_candidatorum; i++)
    {
        si (quaestio.mensura == 0)
        {
            /* Empty query matches everything with score 0 */
            paria[numerus_congruentium].puncta = 0;
            paria[numerus_congruentium].index = i;
            numerus_congruentium++;
        }
        alioquin
        {
            match_result = quaerere_concordare_cum_optionibus(
                quaestio, candidati[i], optiones, piscina);

            si (match_result.status == QUAERERE_OK)
            {
                paria[numerus_congruentium].puncta = match_result.puncta;
                paria[numerus_congruentium].index = i;
                numerus_congruentium++;
            }
        }
    }

    si (numerus_congruentium == 0)
    {
        redde fructus;
    }

    /* Sort by score descending */
    _quaerere_ordinare(paria, numerus_congruentium);

    /* Allocate result arrays */
    fructus.elementa = (chorda*)piscina_allocare(piscina,
                                                  (memoriae_index)numerus_congruentium * (memoriae_index)magnitudo(chorda));
    fructus.puncta = (s32*)piscina_allocare(piscina,
                                             (memoriae_index)numerus_congruentium * (memoriae_index)magnitudo(s32));

    si (fructus.elementa == NIHIL || fructus.puncta == NIHIL)
    {
        fructus.elementa = NIHIL;
        fructus.puncta = NIHIL;
        fructus.numerus = 0;
        redde fructus;
    }

    /* Build result in sorted order */
    per (i = 0; i < numerus_congruentium; i++)
    {
        fructus.elementa[i] = candidati[paria[i].index];
        fructus.puncta[i] = paria[i].puncta;
    }

    fructus.numerus = numerus_congruentium;

    redde fructus;
}


QuaerereFiltrumFructus
quaerere_filtrare(
    chorda   quaestio,
    chorda*  candidati,
    i32      numerus_candidatorum,
    Piscina* piscina)
{
    QuaerereOptiones opt;

    opt = quaerere_optiones_default();

    redde quaerere_filtrare_cum_optionibus(quaestio, candidati,
                                           numerus_candidatorum, &opt, piscina);
}
