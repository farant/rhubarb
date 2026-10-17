/* dependentia.h - Arbores dependentiae: cruces, proiectivitas, lacunae,
 * nidificatio, nucleus planaris
 *
 * Arbor dependentiae sententiae: verba positionibus 1..n, caput
 * cuiusque verbi aliud verbum aut radix artificialis (positio 0).
 * Arcus (h, d) intervallum inter h et d tegit; arcus REALES sunt quorum
 * caput verbum est, arcus RADICIS (0, r) ad verbum radicis ducit. Nomen
 * ex grammatica Modistarum (Thomas Erfordiensis, Grammatica
 * speculativa): constructio omnis ex DEPENDENTE et TERMINANTE.
 *
 * Definitiones (Kuhlmann et Nivre, "Mildly Non-Projective Dependency
 * Structures", COLING/ACL 2006, pp. 507-514; numeri definitionum
 * eorum):
 *   CRUX: arcus {a, b} et {c, e} se secant sicut a < c < b < e
 *         (termini stricte alternantes); arcus verbum commune habentes
 *         numquam se secant.
 *   PLANARIS (Def. 4): nulli duo arcus REALES se secant (arcus radicis
 *         NON additur).
 *   PROIECTIVA (Def. 3): proventus (yield, verba a verbo dominata,
 *         ipso incluso) cuiusque verbi intervallum est. Aequivalenter:
 *         nulla crux arcu radicis addito. Duae viae in functionibus
 *         diversis computantur (dependentia_cruces,
 *         dependentia_mensurare), ut probatio eas comparet.
 *   LACUNA (Def. 6): par verborum vicinorum in proventu ordinato
 *         quorum positiones plus quam I differunt; gradus lacunarum
 *         verbi = numerus earum, arboris = maximum (Def. 7).
 *   BENE NIDIFICATA (Def. 8): nulli duo subarbores disiunctae (neutra
 *         radix alteram dominatur) alternant (l1 < l2 < r1 < r2, l1, r1
 *         in una, l2, r2 in altera).
 *   GRADUS ARCUS (Def. 9): numerus componentium nexarum subgraphi ab
 *         intervallo arcus inducti (termini inclusi) quarum radix a
 *         capite arcus non dominatur; arboris = maximum.
 *   NUCLEUS PLANARIS: plurimi arcus reales quorum nulli duo se secant
 *         (programma dynamicum intervallorum, exactum).
 *
 * Lex nulla exacta: ordine verborum fortuito (uniformi) duo arcus sine
 * verbo communi se secant probabilitate EXACTE 1/3 (ordo relativus
 * quattuor terminorum uniformis in XXIV, figurae tres - nidus, latus,
 * crux - in VIII quaeque); ergo E[cruces] = paria_disiuncta / 3.
 *
 * Arbor non copiatur; functiones mensurae arborem SANAM postulant
 * (dependentia_validare) et FALSUM / -1 reddunt si non est. Memoria
 * temporaria in piscina vocantis (O(n) aut O(n^2) octeti; vocans
 * piscinam per sententiam reficere potest).
 */

#ifndef DEPENDENTIA_H
#define DEPENDENTIA_H

#include "latina.h"
#include "piscina.h"

nomen structura {
                  i32  numerus;   /* verba 1..numerus */
    constans      i32* capita;    /* capita[k - 1] = caput verbi k; 0 =
                                   * radix artificialis */
} DependentiaArbor;

nomen enumeratio {
    DEPENDENTIA_SANA = ZEPHYRUM,
    DEPENDENTIA_VACUA,            /* numerus 0 */
    DEPENDENTIA_CAPUT_EXTRA,      /* caput extra 0..numerus, aut sui */
    DEPENDENTIA_RADIX_NULLA,      /* nullum verbum caput 0 habet */
    DEPENDENTIA_RADICES_PLURES,   /* plura verba caput 0 habent */
    DEPENDENTIA_CYCLUS            /* via capitum radicem non attingit */
} DependentiaStatus;

/* Arborem iudicare (ordine: vacua, caput extra, radices, cyclus). */
DependentiaStatus
dependentia_validare (
    constans DependentiaArbor* arbor);

/* Arbor sine verbis notatis (omittere[k - 1] VERUM): reliqua ordine
 * renumerantur; filii verbi omissi capiti eius proximo non omisso
 * adhaerent (*contractiones = verba quorum caput sic mutatum est).
 * FALSUM si arbor non sana, si radix omittitur (arbor non maneret), aut
 * memoria; capita exitus in piscina. */
b32
dependentia_omittere (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                 constans b32* omittere,
             DependentiaArbor* exitus,
                          i32* contractiones);

nomen structura {
    s64 cruces;            /* paria arcuum realium se secantium */
    s64 cruces_radicis;    /* arcus reales arcum radicis secantes */
    i32 arcus_secti;       /* arcus reales in cruce aliqua (cum realibus) */
    s64 paria_disiuncta;   /* paria arcuum realium sine verbo communi:
                            * ordine fortuito E[cruces] = paria / 3 */
    b32 planaris;          /* cruces == 0 (Def. 4) */
    b32 proiectiva;        /* et cruces_radicis == 0 (Def. 3 per cruces) */
} DependentiaCruces;

/* Cruces numerare. FALSUM si arbor non sana aut memoria. */
b32
dependentia_cruces (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
            DependentiaCruces* exitus);

nomen structura {
    i32 gradus_lacunarum;   /* Def. 7 */
    i32 gradus_arcuum;      /* Def. 9 */
    b32 bene_nidificata;    /* Def. 8 */
    b32 proiectiva;         /* proventus omnes intervalla (Def. 3) */
} DependentiaMensura;

/* Lacunas, gradum arcuum, nidificationem metiri; lacunae[k - 1] =
 * lacunae proventus verbi k (lacunae NIHIL = non redduntur). FALSUM si
 * arbor non sana aut memoria. */
b32
dependentia_mensurare (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                          i32* lacunae,
           DependentiaMensura* exitus);

/* Nucleus planaris: plurimi arcus reales sine cruce inter se. -1 si
 * arbor non sana aut memoria. */
s32
dependentia_nucleus_planaris (
                      Piscina* piscina,
    constans DependentiaArbor* arbor);

/* Graphum crucium (nodi = arcus reales, margines = cruces):
 * componens[k - 1] = index componentis arcus in verbum k (0, 1, ...
 * ordine primi verbi), -1 pro verbo radicis (arcus realis nullus);
 * singuli inclusi. Redde numerum componentium; -1 si arbor non sana
 * aut memoria. */
s32
dependentia_componentes (
                      Piscina* piscina,
    constans DependentiaArbor* arbor,
                          s32* componens);

#endif /* DEPENDENTIA_H */
