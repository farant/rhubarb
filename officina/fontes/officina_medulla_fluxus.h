/* officina_medulla_fluxus.h - analyses medullae (M5 G24)
 *
 * Praedecessores, attingibilitas, ordo dorsi (RPO cum corporibus
 * ansarum contiguis), profunditas ansae, vita registrorum
 * (intervalla super positiones lineares). Consumptor primus: dorsum
 * nativum (M5 T7-T9).
 *
 * Analyses SOLUM LEGUNT: functio post medulla_fluxus_computare
 * octetim eadem est (pactum ambulationis canonicae,
 * officina_medulla.h). Ordo dorsi est VISUS, non reordinatio.
 *
 * Positiones: in ordine dorsi, binae per instructionem - 2k usus
 * (operanda legit), 2k+1 definitio (destinatio scribitur). Blocci
 * non attingibiles positiones non habent. Parametra in positione 0
 * definiuntur; registrum lectum ante omnem definitionem a positione 0
 * vivit (F13: tabula registrorum zephyris impleta). Intervallum =
 * [min, max] positionum ubi registrum vivit aut scribitur (sine
 * foraminibus in M5).
 */

#ifndef OFFICINA_MEDULLA_FLUXUS_H
#define OFFICINA_MEDULLA_FLUXUS_H

#include "officina_medulla.h"

nomen structura MedullaFluxus MedullaFluxus;

/* Analyses SOLUM LEGUNT - functio post computare octetim eadem
 * (pactum ambulationis canonicae). NIHIL si functio sine bloccis. */
MedullaFluxus*
medulla_fluxus_computare (
                    Piscina* piscina,
    constans MedullaFunctio* functio);

/* praedecessores blocci (ordine indicum crescente, sine duplicibus;
 * etiam ex bloccis non attingibilibus); reddit numerum */
s32
medulla_fluxus_praedecessores (
    constans MedullaFluxus*  f,
                       s32   bloccus,
              constans s32** indices_out);

b32
medulla_fluxus_attingibilis (
    constans MedullaFluxus* f,
                       s32  bloccus);

/* ordo dorsi: RPO bloccorum attingibilium, corporibus ansarum
 * contiguis; numerus_out */
constans s32*
medulla_fluxus_ordo (
    constans MedullaFluxus* f,
                       s32* numerus_out);

/* numerus ansarum naturalium bloccum continentium (0 extra ansas) */
s32
medulla_fluxus_profunditas_ansae (
    constans MedullaFluxus* f,
                       s32  bloccus);

/* positio linearis -> index planus (pactum); 0xFFFFFFFF si extra
 * fines */
i32
medulla_fluxus_index_planus (
    constans MedullaFluxus* f,
                       i32  positio);

/* intervallum vitae registri super positiones (multi-def et
 * iuncturae comprehensae); FALSUM si registrum numquam vivit */
b32
medulla_fluxus_intervallum (
    constans MedullaFluxus* f,
                       s32  index_registri,
                       i32* initium_out,
                       i32* finis_out);

/* VERUM si intervallum vocationem transit (vivit ante usum et post
 * definitionem vocationis; dorsum: registrum callee-saved) */
b32
medulla_fluxus_transit_vocationem (
    constans MedullaFluxus* f,
                       s32  index_registri);

#endif /* OFFICINA_MEDULLA_FLUXUS_H */
