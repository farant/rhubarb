#ifndef SERVITIUM_H
#define SERVITIUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "entitas.h"
#include "entitas_repositorium.h"

/* ==================================================
 * SERVITIUM - Service Registry
 *
 * Registrum servitorum (singleton actores).
 * Servitia sunt entitates cum nota "#service::nomen".
 *
 * PHILOSOPHIA:
 * - Servitia sunt actores singleton
 * - Registratio per nomen (e.g., "clock", "logger")
 * - Auto-discovery per notas "#service::"
 *
 * EXEMPLUM:
 *   RegistrumServitii* reg = registrum_servitii_creare(piscina, repo);
 *   servitium_registrare(reg, "clock", clock_entity_id);
 *   Entitas* clock = servitium_obtinere(reg, "clock");
 *
 * ================================================== */

/* Praefixum notae servitii */
#define SERVITIUM_NOTA_PRAEFIXUM "#service::"


/* ==================================================
 * RegistrumServitii
 * ================================================== */

/* Registrum servitii
 *
 * nomina: TabulaDispersa (service_name -> entity_id chorda*)
 * repositorium: Repositorium entitatum
 * piscina: Piscina pro allocationibus
 * intern: Internamentum pro chordis
 */
nomen structura RegistrumServitii {
    TabulaDispersa*      nomina;
    EntitasRepositorium* repositorium;
    InternamentumChorda* intern;
    Piscina*             piscina;
} RegistrumServitii;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare registrum servitii
 *
 * piscina: Piscina pro allocationibus
 * repositorium: Repositorium entitatum
 * intern: Internamentum pro chordis
 *
 * Redde: RegistrumServitii* si successus, NIHIL si fractura
 */
RegistrumServitii*
registrum_servitii_creare(
    Piscina*             piscina,
    EntitasRepositorium* repositorium,
    InternamentumChorda* intern);


/* ==================================================
 * Registratio
 * ================================================== */

/* Registrare servitium per nomen
 *
 * reg: Registrum servitii
 * titulus: Nomen servitii (e.g., "clock")
 * entitas_id: ID entitatis servitii
 *
 * Automatice addit notam "#service::nomen" ad entitatem.
 *
 * Redde: VERUM si successus
 */
b32
servitium_registrare(
    RegistrumServitii*  reg,
    constans character* titulus,
    chorda*             entitas_id);

/* Registrare servitium cum entitate iam creata */
b32
servitium_registrare_entitatem(
    RegistrumServitii*  reg,
    constans character* titulus,
    Entitas*            entitas);

/* Deregistrare servitium
 *
 * Redde: VERUM si deregistratum, FALSUM si non inventum
 */
b32
servitium_deregistrare(
    RegistrumServitii*  reg,
    constans character* titulus);


/* ==================================================
 * Quaestio
 * ================================================== */

/* Obtinere servitium per nomen
 *
 * Redde: Entitas* si inventum, NIHIL si non
 */
Entitas*
servitium_obtinere(
    RegistrumServitii*  reg,
    constans character* titulus);

/* Obtinere ID servitii per nomen
 *
 * Redde: chorda* ID si inventum, NIHIL si non
 */
chorda*
servitium_obtinere_id(
    RegistrumServitii*  reg,
    constans character* titulus);

/* Verificare si servitium registratum */
b32
servitium_habet(
    RegistrumServitii*  reg,
    constans character* titulus);

/* Numerus servitiorum registratorum */
i32
servitium_numerus(
    RegistrumServitii* reg);


/* ==================================================
 * Auto-Discovery
 * ================================================== */

/* Discoperire servitia ex entitates cum nota "#service::"
 *
 * Quaerit entitates cum praefixo notae "#service::",
 * extrahit nomen post praefixum, registrat.
 *
 * Redde: Numerus servitiorum discoopertorum
 */
i32
servitium_discoperire(
    RegistrumServitii* reg);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Imprimere registrum pro depuratione */
vacuum
servitium_imprimere(
    RegistrumServitii* reg);

#endif /* SERVITIUM_H */
