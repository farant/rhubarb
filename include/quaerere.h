/* quaerere.h - Fuzzy String Matching (fzf-style)
 *
 * Bibliotheca pro fuzzy/approximate string matching.
 * Nomen ex Latina "quaerere" (to search/seek).
 *
 * Algorithmus: Subsequence matching cum scoring heuristics.
 * Query characters debent apparere in ordine in target, sed non contigue.
 *
 * USUS:
 *   b32 congruit = quaerere_congruit(query, target);
 *   QuaerereFructus res = quaerere_concordare(query, target, piscina);
 *   QuaerereFiltrumFructus filtered = quaerere_filtrare(query, candidati, n, piscina);
 */

#ifndef QUAERERE_H
#define QUAERERE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"


/* ========================================================================
 * CONSTANTIAE - SCORING
 * ======================================================================== */

#define QUAERERE_BONUS_CONSECUTIVUS   XVI    /* Consecutive character match */
#define QUAERERE_BONUS_LIMES_VERBI    VIII   /* Match at word boundary */
#define QUAERERE_BONUS_INITIUM        VIII   /* Match at start of string */
#define QUAERERE_BONUS_CASUS_EXACTUS  IV     /* Exact case match */
#define QUAERERE_POENA_LACUNA         I      /* Gap penalty per character */


/* ========================================================================
 * TYPI - STATUS
 * ======================================================================== */

nomen enumeratio {
    QUAERERE_OK = 0,
    QUAERERE_NON_CONGRUIT,        /* No match found */
    QUAERERE_FRACTA_ALLOCATIO     /* Allocation failed */
} QuaerereStatus;


/* ========================================================================
 * TYPI - FRUCTUS
 * ======================================================================== */

/* Result of single match */
nomen structura {
    QuaerereStatus status;
    s32            puncta;        /* Score (higher = better match) */
    i32*           positiones;    /* Match positions in target (allocatum ex piscina) */
    i32            numerus;       /* Number of matched characters */
} QuaerereFructus;


/* Result of batch filtering */
nomen structura {
    chorda* elementa;             /* Sorted array of matching strings */
    s32*    puncta;               /* Corresponding scores */
    i32     numerus;              /* Number of matches */
} QuaerereFiltrumFructus;


/* ========================================================================
 * TYPI - OPTIONES
 * ======================================================================== */

nomen structura {
    b32 case_sensitivus;          /* Case sensitive matching (default: FALSUM) */
    b32 intelligens_casus;        /* Smart case: sensitive if query has uppercase */
} QuaerereOptiones;


/* ========================================================================
 * FUNCTIONES - OPTIONES
 * ======================================================================== */

/* Obtinere optiones default
 *
 * Redde: Optiones cum case_sensitivus=FALSUM, intelligens_casus=VERUM
 */
QuaerereOptiones
quaerere_optiones_default(vacuum);


/* ========================================================================
 * FUNCTIONES - SIMPLEX CONGRUENTIA
 * ======================================================================== */

/* Verificare si query congruit cum target (sine scoring)
 *
 * quaestio: Query string
 * scopus:   Target string to match against
 *
 * Redde: VERUM si query est subsequence de target
 */
b32
quaerere_congruit(
    chorda quaestio,
    chorda scopus);


/* Verificare cum optionibus
 *
 * quaestio: Query string
 * scopus:   Target string
 * optiones: Matching options (case sensitivity)
 *
 * Redde: VERUM si congruit
 */
b32
quaerere_congruit_cum_optionibus(
    chorda            quaestio,
    chorda            scopus,
    QuaerereOptiones* optiones);


/* ========================================================================
 * FUNCTIONES - CONCORDARE CUM SCORING
 * ======================================================================== */

/* Concordare query cum target, calculare score et positiones
 *
 * quaestio: Query string
 * scopus:   Target string
 * piscina:  Arena pro allocatio positionum
 *
 * Redde: QuaerereFructus cum status, puncta, positiones
 */
QuaerereFructus
quaerere_concordare(
    chorda   quaestio,
    chorda   scopus,
    Piscina* piscina);


/* Concordare cum optionibus
 *
 * quaestio: Query string
 * scopus:   Target string
 * optiones: Matching options
 * piscina:  Arena pro allocatio
 *
 * Redde: QuaerereFructus
 */
QuaerereFructus
quaerere_concordare_cum_optionibus(
    chorda            quaestio,
    chorda            scopus,
    QuaerereOptiones* optiones,
    Piscina*          piscina);


/* ========================================================================
 * FUNCTIONES - BATCH FILTERING
 * ======================================================================== */

/* Filtrare et ordinare array de candidatis per match score
 *
 * quaestio:             Query string
 * candidati:            Array de strings ad filtrare
 * numerus_candidatorum: Numerus elementorum in array
 * piscina:              Arena pro allocatio resultorum
 *
 * Redde: QuaerereFiltrumFructus cum elementa ordinata per score (descending)
 */
QuaerereFiltrumFructus
quaerere_filtrare(
    chorda   quaestio,
    chorda*  candidati,
    i32      numerus_candidatorum,
    Piscina* piscina);


/* Filtrare cum optionibus
 *
 * quaestio:             Query string
 * candidati:            Array de strings
 * numerus_candidatorum: Numerus elementorum
 * optiones:             Matching options
 * piscina:              Arena pro allocatio
 *
 * Redde: QuaerereFiltrumFructus
 */
QuaerereFiltrumFructus
quaerere_filtrare_cum_optionibus(
    chorda            quaestio,
    chorda*           candidati,
    i32               numerus_candidatorum,
    QuaerereOptiones* optiones,
    Piscina*          piscina);


#endif /* QUAERERE_H */
