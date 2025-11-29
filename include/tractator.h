#ifndef TRACTATOR_H
#define TRACTATOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_dispersa.h"
#include "internamentum.h"
#include "actor.h"
#include "entitas.h"
#include "entitas_repositorium.h"

/* ==================================================
 * TRACTATOR - Registrum Tractatoris Nuntiorum
 *
 * Registrum functionum tractandi nuntios per genus entitatis.
 * Similis RegistrumCommandi sed pro actoribus.
 *
 * PHILOSOPHIA:
 * - Genus entitatis definit comportamentum
 * - Tractator per (genus_entitatis, genus_nuntii) quaeritur
 * - Fallback tractator pro nuntibus ignotis
 *
 * EXEMPLUM:
 *   RegistrumTractatoris* reg = registrum_tractatoris_creare(piscina);
 *   registrum_tractatoris_registrare(reg, "Page", "open", page_open_handler);
 *   FunctioTractandi f = registrum_tractatoris_invenire(reg, genus, nuntius);
 *
 * ================================================== */

/* Forward declaration */
structura CursusNuntiorum;


/* ==================================================
 * ContextusTractandi - Contextus pro tractatione
 * ================================================== */

/* Contextus transmissus ad omnes tractatores
 *
 * entitas: Entitas recipiens nuntium
 * nuntius: Nuntius tractandus
 * repositorium: Pro quaestionibus et mutationibus
 * cursus: Pro mittendis nuntiis
 * piscina: Pro allocationibus temporariis
 * datum_custom: Datum applicatio-specificum
 */
nomen structura ContextusTractandi {
    Entitas*                    entitas;
    Nuntius*                    nuntius;
    EntitasRepositorium*        repositorium;
    structura CursusNuntiorum*  cursus;
    Piscina*                    piscina;
    vacuum*                     datum_custom;
} ContextusTractandi;


/* ==================================================
 * FunctioTractandi - Signatura functionis tractatoris
 * ================================================== */

/* Functio tractandi nuntium
 *
 * ctx: Contextus tractationis
 *
 * Redde: VERUM si nuntius tractatus, FALSUM si error
 */
nomen b32 (*FunctioTractandi)(ContextusTractandi* ctx);


/* ==================================================
 * RegistrumTractatoris
 * ================================================== */

/* Registrum tractatoris nuntiorum
 *
 * genera: TabulaDispersa (genus_entitatis -> TabulaDispersa (genus_nuntii -> FunctioTractandi))
 * fallbacks: TabulaDispersa (genus_entitatis -> FunctioTractandi) pro nuntibus ignotis
 * piscina: Piscina pro allocationibus
 * intern: Internamentum pro chordis
 */
nomen structura RegistrumTractatoris {
    TabulaDispersa*      genera;
    TabulaDispersa*      fallbacks;
    Piscina*             piscina;
    InternamentumChorda* intern;
} RegistrumTractatoris;


/* ==================================================
 * Creatio
 * ================================================== */

/* Creare registrum tractatoris
 *
 * piscina: Piscina pro allocationibus
 * intern: Internamentum pro chordis
 *
 * Redde: RegistrumTractatoris* si successus, NIHIL si fractura
 */
RegistrumTractatoris*
registrum_tractatoris_creare(
    Piscina*             piscina,
    InternamentumChorda* intern);


/* ==================================================
 * Registratio
 * ================================================== */

/* Registrare tractator pro (genus_entitatis, genus_nuntii)
 *
 * reg: Registrum
 * genus_entitatis: Genus entitatis (e.g., "Page", "Widget")
 * genus_nuntii: Genus nuntii (e.g., "open", "tick")
 * functio: Functio tractandi
 *
 * Redde: VERUM si successus
 */
b32
registrum_tractatoris_registrare(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    constans character*   genus_nuntii,
    FunctioTractandi      functio);

/* Ponere fallback tractator pro genere entitatis
 *
 * Fallback vocatur si nullus tractator specificus inventus
 */
b32
registrum_tractatoris_ponere_fallback(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    FunctioTractandi      functio);


/* ==================================================
 * Quaestio
 * ================================================== */

/* Invenire tractator pro (genus_entitatis, genus_nuntii)
 *
 * Primo quaerit tractator specificum, deinde fallback
 *
 * Redde: FunctioTractandi si inventum, NIHIL si non
 */
FunctioTractandi
registrum_tractatoris_invenire(
    RegistrumTractatoris* reg,
    chorda*               genus_entitatis,
    chorda*               genus_nuntii);

/* Invenire tractator per literis C */
FunctioTractandi
registrum_tractatoris_invenire_literis(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    constans character*   genus_nuntii);

/* Verificare si tractator registratus */
b32
registrum_tractatoris_habet(
    RegistrumTractatoris* reg,
    constans character*   genus_entitatis,
    constans character*   genus_nuntii);


/* ==================================================
 * Utilitas
 * ================================================== */

/* Numerus tractatoris registratorum */
i32
registrum_tractatoris_numerus(
    RegistrumTractatoris* reg);

/* Imprimere registrum pro depuratione */
vacuum
registrum_tractatoris_imprimere(
    RegistrumTractatoris* reg);

#endif /* TRACTATOR_H */
