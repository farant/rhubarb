#ifndef CURSUS_H
#define CURSUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "actor.h"
#include "tractator.h"
#include "entitas_repositorium.h"

/* ==================================================
 * CURSUS NUNTIORUM - Message Bus
 *
 * Dispatcher centralis pro nuntiis inter actores.
 * Capsae allocantur pigre (cum primo nuntio).
 *
 * PHILOSOPHIA:
 * - Nuntii enqueue, non immediate tractantur
 * - cursus_processare() tractat nuntios cum limite
 * - Capsae pigre allocatae (non omnes entitates nuntios recipiunt)
 *
 * EXEMPLUM:
 *   CursusNuntiorum* cursus = cursus_creare(piscina, tractator, repo, intern);
 *   cursus_mittere(cursus, destinatio_id, "tick", NIHIL);
 *   i32 tractati = cursus_processare(cursus, C);  // max 100 nuntii
 *
 * ================================================== */


/* ==================================================
 * CursusNuntiorum - Message Bus
 * ================================================== */

/* Cursus Nuntiorum - Dispatcher nuntiorum
 *
 * capsae: TabulaDispersa (entity_id -> Capsa*)
 * tractator: Registrum tractatoris
 * repositorium: Pro quaestionibus entitatum
 * intern: Internamentum pro chordis
 * piscina: Piscina pro allocationibus
 * datum_custom: Datum applicatio-specificum
 */
nomen structura CursusNuntiorum {
    TabulaDispersa*       capsae;
    RegistrumTractatoris* tractator;
    EntitasRepositorium*  repositorium;
    InternamentumChorda*  intern;
    Piscina*              piscina;
    vacuum*               datum_custom;
} CursusNuntiorum;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare cursum nuntiorum
 *
 * piscina: Piscina pro allocationibus
 * tractator: Registrum tractatoris
 * repositorium: Repositorium entitatum
 * intern: Internamentum pro chordis
 *
 * Redde: CursusNuntiorum* si successus, NIHIL si fractura
 */
CursusNuntiorum*
cursus_creare(
    Piscina*              piscina,
    RegistrumTractatoris* tractator,
    EntitasRepositorium*  repositorium,
    InternamentumChorda*  intern);

/* Ponere datum custom */
vacuum
cursus_ponere_datum(
    CursusNuntiorum* cursus,
    vacuum*          datum);


/* ==================================================
 * Mittere Nuntios
 * ================================================== */

/* Mittere nuntium ad entitatem (enqueue)
 *
 * cursus: Cursus nuntiorum
 * mittens_id: ID mittentis (potest esse NIHIL)
 * destinatio_id: ID entitatis destinationis
 * genus_nuntii: Typus nuntii
 * datum: Payload (potest esse NIHIL)
 *
 * Redde: VERUM si successus
 */
b32
cursus_mittere(
    CursusNuntiorum*    cursus,
    chorda*             mittens_id,
    chorda*             destinatio_id,
    constans character* genus_nuntii,
    TabulaDispersa*     datum);

/* Mittere nuntium iam creatum */
b32
cursus_mittere_nuntium(
    CursusNuntiorum* cursus,
    chorda*          destinatio_id,
    Nuntius*         nuntius);

/* Mittere nuntium simplex (sine payload) */
b32
cursus_mittere_simplex(
    CursusNuntiorum*    cursus,
    chorda*             mittens_id,
    chorda*             destinatio_id,
    constans character* genus_nuntii);


/* ==================================================
 * Processare Nuntios
 * ================================================== */

/* Processare nuntios pendentes (cum limite)
 *
 * cursus: Cursus nuntiorum
 * maximus_nuntii: Maximus numerus nuntiorum tractandorum
 *
 * Redde: Numerus nuntiorum tractatorum
 */
i32
cursus_processare(
    CursusNuntiorum* cursus,
    i32              maximus_nuntii);

/* Processare omnes nuntios pro entitate specifica */
i32
cursus_processare_entitatem(
    CursusNuntiorum* cursus,
    chorda*          entitas_id,
    i32              maximus_nuntii);


/* ==================================================
 * Quaestio
 * ================================================== */

/* Numerus nuntiorum pendentium in toto */
i32
cursus_numerus_pendentium(
    CursusNuntiorum* cursus);

/* Numerus nuntiorum pendentium pro entitate */
i32
cursus_numerus_pendentium_entitatis(
    CursusNuntiorum* cursus,
    chorda*          entitas_id);

/* Verificare si entitas habet capsam */
b32
cursus_habet_capsam(
    CursusNuntiorum* cursus,
    chorda*          entitas_id);

/* Obtinere capsam entitatis (NIHIL si non existit) */
Capsa*
cursus_obtinere_capsam(
    CursusNuntiorum* cursus,
    chorda*          entitas_id);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Vacare omnes capsas (removere omnes nuntios) */
vacuum
cursus_vacare(
    CursusNuntiorum* cursus);

/* Imprimere status pro depuratione */
vacuum
cursus_imprimere(
    CursusNuntiorum* cursus);

#endif /* CURSUS_H */
