#ifndef ACTOR_H
#define ACTOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "xar.h"

/* ==================================================
 * ACTOR - Systema Actorum
 *
 * Nuntius: Epistula inter actores
 * Capsa: Receptaculum nuntiorum pro actore
 *
 * PHILOSOPHIA:
 * - Omnis entitas est actor
 * - Actores communicant per nuntios
 * - Capsae allocantur pigre (cum primo nuntio)
 * - Datum nuntii est TabulaDispersa (clavis -> valor)
 *
 * ================================================== */


/* ==================================================
 * Nuntius (Message)
 * ================================================== */

/* Nuntius - Epistula inter actores
 *
 * mittens_id: ID entitatis mittentis (NIHIL pro systemate)
 * genus: Typus nuntii (internatum)
 * datum: Payload ut TabulaDispersa (clavis chorda -> valor chorda*)
 * tempus_creatus: Quando nuntius creatus est
 */
nomen structura Nuntius {
    chorda*              mittens_id;
    chorda*              genus;
    TabulaDispersa*      datum;
    f64                  tempus_creatus;
    Piscina*             piscina;
    InternamentumChorda* intern;
} Nuntius;


/* ==================================================
 * Nuntius - Creatio
 * ================================================== */

/* Creare nuntium novum
 *
 * piscina: Piscina pro allocationibus
 * intern: Internamentum pro chordis
 * mittens_id: ID mittentis (potest esse NIHIL)
 * genus: Typus nuntii (e.g., "tick", "click")
 *
 * Redde: Nuntius* si successus, NIHIL si fractura
 */
Nuntius*
nuntius_creare(
    Piscina*             piscina,
    InternamentumChorda* intern,
    chorda*              mittens_id,
    constans character*  genus);


/* ==================================================
 * Nuntius - Datum (Payload)
 * ================================================== */

/* Ponere valorem in datum nuntii
 *
 * nuntius: Nuntius
 * clavis: Clavis (C string)
 * valor: Valor ut chorda*
 *
 * Redde: VERUM si successus
 */
b32
nuntius_datum_ponere(
    Nuntius*            nuntius,
    constans character* clavis,
    chorda*             valor);

/* Ponere valorem ex C string
 *
 * Creat chorda internata ex cstr et ponit in datum
 */
b32
nuntius_datum_ponere_literis(
    Nuntius*            nuntius,
    constans character* clavis,
    constans character* valor);

/* Ponere valorem s32 (convertit ad chorda)
 */
b32
nuntius_datum_ponere_s32(
    Nuntius*            nuntius,
    constans character* clavis,
    s32                 valor);

/* Capere valorem ex datum nuntii
 *
 * Redde: chorda* si inventum, NIHIL si non
 */
chorda*
nuntius_datum_capere(
    Nuntius*            nuntius,
    constans character* clavis);

/* Capere valorem ut s32
 *
 * Redde: VERUM si inventum et conversio successit
 */
b32
nuntius_datum_capere_s32(
    Nuntius*            nuntius,
    constans character* clavis,
    s32*                valor);

/* Capere valorem ut s64
 */
b32
nuntius_datum_capere_s64(
    Nuntius*            nuntius,
    constans character* clavis,
    s64*                valor);

/* Capere valorem ut f64
 */
b32
nuntius_datum_capere_f64(
    Nuntius*            nuntius,
    constans character* clavis,
    f64*                valor);

/* Capere valorem ut b32
 */
b32
nuntius_datum_capere_b32(
    Nuntius*            nuntius,
    constans character* clavis,
    b32*                valor);

/* Verificare si datum habet clavem
 */
b32
nuntius_datum_habet(
    Nuntius*            nuntius,
    constans character* clavis);


/* ==================================================
 * Nuntius - Utilitas
 * ================================================== */

/* Imprimere nuntium pro depuratione
 */
vacuum
nuntius_imprimere(
    Nuntius* nuntius);


/* ==================================================
 * Capsa (Mailbox)
 * ================================================== */

/* Capsa - Receptaculum nuntiorum pro actore
 *
 * possessor_id: ID entitatis possidentis
 * nuntii: Xar de Nuntius*
 */
nomen structura Capsa {
    chorda*  possessor_id;
    Xar*     nuntii;
    Piscina* piscina;
} Capsa;


/* ==================================================
 * Capsa - Creatio
 * ================================================== */

/* Creare capsam novam
 *
 * piscina: Piscina pro allocationibus
 * possessor_id: ID entitatis possidentis
 *
 * Redde: Capsa* si successus, NIHIL si fractura
 */
Capsa*
capsa_creare(
    Piscina* piscina,
    chorda*  possessor_id);


/* ==================================================
 * Capsa - Operationes
 * ================================================== */

/* Addere nuntium ad capsam
 *
 * Redde: VERUM si successus
 */
b32
capsa_addere(
    Capsa*   capsa,
    Nuntius* nuntius);

/* Capere et removere primum nuntium (FIFO)
 *
 * Redde: Nuntius* si habuit, NIHIL si vacua
 */
Nuntius*
capsa_tollere(
    Capsa* capsa);

/* Inspicere primum nuntium sine remotione
 *
 * Redde: Nuntius* si habuit, NIHIL si vacua
 */
Nuntius*
capsa_inspicere(
    Capsa* capsa);

/* Numerus nuntiorum in capsa
 */
i32
capsa_numerus(
    Capsa* capsa);

/* Verificare si capsa vacua
 */
b32
capsa_vacua_est(
    Capsa* capsa);

/* Vacare capsam (removere omnes nuntios)
 */
vacuum
capsa_vacare(
    Capsa* capsa);

#endif /* ACTOR_H */
