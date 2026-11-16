/*
 * plastica_membrum.h - membrum duorum ossium ad metam solvere
 *
 * Membrum in plano suo: os primum a radice pendet, secundum a medio.
 * In quiete rectum deorsum. Meta: ubi finis sit, 'ante' (antrorsum)
 * et 'infra' (deorsum) a radice. Nihil de plagulis aut statuis scit
 * (project-specs/plastica-motus-spec.md IV.4).
 *
 * angulus_radicis: gradus quibus os primum vertitur; NEGATIVUS
 *                  antrorsum (ut rotatio x articuli pendentis)
 * angulus_medii:   flexus medii; 0 = rectum; positivus finem retro
 *                  fert (genu antrorsum: flexus_ante)
 *
 * Finis ergo stat ad: os primum (0, -L1) versum, deinde os secundum
 * ab eo flexum - probatio id per cinematicam directam confirmat.
 *
 * RECUSAT (successus FALSUM et anguli 0; numquam numerus non
 * finitus): metam ultra longitudinem ambarum; metam propiorem quam
 * differentia earum; metam in radice ipsa; longitudinem non
 * positivam; numerum non finitum. Extensio plena licet (margo
 * PLASTICA_MEMBRUM_MARGO).
 */

#ifndef PLASTICA_MEMBRUM_H
#define PLASTICA_MEMBRUM_H

/* <aedilis corpus="lib/plastica_membrum.c"/> */

#include "latina.h"
#include "piscina.h"

/* meta tanto ultra ambitum adhuc ut extensio plena accipitur */
#define PLASTICA_MEMBRUM_MARGO  1.0e-4f

nomen structura {
    b32 successus;
    f32 angulus_radicis;
    f32 angulus_medii;
} PlasticaMembrumSolutio;

/*
 * plastica_membrum_solvere - angulos membri ad metam
 *
 * @longitudo_prima:   a radice ad medium; maior quam 0
 * @longitudo_secunda: a medio ad finem; maior quam 0
 * @meta_ante:         meta antrorsum a radice (negativa: retro)
 * @meta_infra:        meta deorsum a radice (negativa: supra)
 * @flexus_ante:       VERUM: medium antrorsum flectitur (genu);
 *                     FALSUM: retrorsum (cubitus)
 */
PlasticaMembrumSolutio
plastica_membrum_solvere (
    f32 longitudo_prima,
    f32 longitudo_secunda,
    f32 meta_ante,
    f32 meta_infra,
    b32 flexus_ante);

#endif /* PLASTICA_MEMBRUM_H */
