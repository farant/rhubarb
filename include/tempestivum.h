#ifndef TEMPESTIVUM_H
#define TEMPESTIVUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "entitas.h"
#include "entitas_repositorium.h"
#include "cursus.h"

/* ==================================================
 * TEMPESTIVUM - Timer Entities
 *
 * Timers sunt entitates cum genus "System::Tempestivum".
 * Administrator processare tempestiva per quadrum.
 *
 * PROPRIETATES TEMPESTIVI:
 * - destinatio_id: ID entitatis destinationis
 * - genus_nuntii: Typus nuntii mittendi
 * - intervallum: Intervallum in millisecondes (s64)
 * - repetere: Si timer repetit (b32)
 * - proximum_tempus: Proximum tempus activationis (s64 ms)
 * - activum: Si timer activum (b32)
 *
 * EXEMPLUM:
 *   AdministratorTempestivi* admin = administrator_tempestivi_creare(...);
 *   tempestivum_creare(admin, widget_id, "tick", 1000, VERUM);
 *   // In cyclo principali:
 *   tempestivum_processare(admin);
 *
 * ================================================== */

/* Genus tempestivi */
#define TEMPESTIVUM_GENUS "System::Tempestivum"

/* Proprietates tempestivi */
#define TEMPESTIVUM_PROP_DESTINATIO    "destinatio_id"
#define TEMPESTIVUM_PROP_GENUS_NUNTII  "genus_nuntii"
#define TEMPESTIVUM_PROP_INTERVALLUM   "intervallum"
#define TEMPESTIVUM_PROP_REPETERE      "repetere"
#define TEMPESTIVUM_PROP_PROXIMUM      "proximum_tempus"
#define TEMPESTIVUM_PROP_ACTIVUM       "activum"


/* ==================================================
 * AdministratorTempestivi
 * ================================================== */

/* Administrator tempestivorum
 *
 * Processare tempestiva per quadrum et mittere nuntios.
 */
nomen structura AdministratorTempestivi {
    EntitasRepositorium* repositorium;
    CursusNuntiorum*     cursus;
    InternamentumChorda* intern;
    Piscina*             piscina;
    chorda*              genus_tempestivi; /* Cached interned genus */
} AdministratorTempestivi;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare administratorem tempestivorum
 *
 * piscina: Piscina pro allocationibus
 * repositorium: Repositorium entitatum
 * cursus: Cursus nuntiorum
 * intern: Internamentum pro chordis
 *
 * Redde: AdministratorTempestivi* si successus, NIHIL si fractura
 */
AdministratorTempestivi*
administrator_tempestivi_creare(
    Piscina*             piscina,
    EntitasRepositorium* repositorium,
    CursusNuntiorum*     cursus,
    InternamentumChorda* intern);


/* ==================================================
 * Tempestiva - Creatio et Deletio
 * ================================================== */

/* Creare novum tempestivum
 *
 * admin: Administrator tempestivorum
 * destinatio_id: ID entitatis destinationis
 * genus_nuntii: Typus nuntii mittendi (e.g., "tick")
 * intervallum_ms: Intervallum in millisecondes
 * repetere: Si timer repetit
 *
 * Redde: Entitas* tempestivi si successus, NIHIL si fractura
 */
Entitas*
tempestivum_creare(
    AdministratorTempestivi* admin,
    chorda*                  destinatio_id,
    constans character*      genus_nuntii,
    s64                      intervallum_ms,
    b32                      repetere);

/* Creare tempestivum cum delay initiali
 *
 * delay_ms: Delay ante primam activationem
 */
Entitas*
tempestivum_creare_cum_delay(
    AdministratorTempestivi* admin,
    chorda*                  destinatio_id,
    constans character*      genus_nuntii,
    s64                      intervallum_ms,
    s64                      delay_ms,
    b32                      repetere);

/* Annullare tempestivum
 *
 * Redde: VERUM si annullatum, FALSUM si non inventum
 */
b32
tempestivum_annullare(
    AdministratorTempestivi* admin,
    chorda*                  tempestivum_id);

/* Pausare tempestivum */
b32
tempestivum_pausare(
    AdministratorTempestivi* admin,
    chorda*                  tempestivum_id);

/* Resumere tempestivum */
b32
tempestivum_resumere(
    AdministratorTempestivi* admin,
    chorda*                  tempestivum_id);


/* ==================================================
 * Processare
 * ================================================== */

/* Processare tempestiva - vocare per quadrum
 *
 * Quaerit omnes entitates cum genus "System::Tempestivum",
 * verificat si proximum_tempus <= tempus_nunc,
 * mittit nuntios, actualizat proximum_tempus.
 *
 * Redde: Numerus nuntiorum missorum
 */
i32
tempestivum_processare(
    AdministratorTempestivi* admin);


/* ==================================================
 * Quaestio
 * ================================================== */

/* Numerus tempestivorum activorum */
i32
tempestivum_numerus_activorum(
    AdministratorTempestivi* admin);

/* Obtinere tempestiva pro entitate destinationis */
Xar*
tempestivum_pro_destinatione(
    AdministratorTempestivi* admin,
    chorda*                  destinatio_id);

/* Verificare si tempestivum activum */
b32
tempestivum_est_activum(
    AdministratorTempestivi* admin,
    chorda*                  tempestivum_id);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Imprimere status pro depuratione */
vacuum
tempestivum_imprimere(
    AdministratorTempestivi* admin);

#endif /* TEMPESTIVUM_H */
