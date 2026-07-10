/* officina_conexio.h - Conexio: nexor moduli medullae (M2a)
 *
 * DECISUS (simulatio-4 C1-C6 + interview Q2/Q3):
 * - Tabula symbolorum GLOBALIS + ordinatio translationis per modulum
 *   (index localis -> index globalis). Moduli IMMUTABILES manent -
 *   nulla instructio rescribitur (1.15M instructiones intactae).
 * - NECTERE MUNDUM: omnes moduli bibliothecae + modulus probationis
 *   (manifesta per plagulam non existunt nec computari possunt -
 *   fundus-corpus). Symbola insoluta -> DECIPULAE pigrae (ruunt
 *   SOLUM SI vocata - sedes sisterae in commissura nexus).
 * - DESCRIPTORES: 16 octeti in globalibus regionis per functionem -
 *   inscriptio descriptoris EST valor monstratoris functionis
 *   (relocationes symbola functionum petere possunt - aureum
 *   statica.medulla). signum = commissura nativa M5.
 * - CELLAE externae: stdin/stdout/stderr (ansae opacae 0/1/2) +
 *   errno (0) - symbola data externa nota; pons aedificatorum eas
 *   curat (M2b).
 * - RELOCATIO ADDITIVA: locellus = inscriptio symboli + octeti
 *   priores locelli (addendum). Unus mechanismus: plana/membrum/
 *   elementum/functio.
 *
 * Ordo: creare -> modulum_addere (iterum) -> nectere -> quaerere.
 * Conflictus (definitio duplex) = fractura honesta cum querela.
 */

#ifndef OFFICINA_CONEXIO_H
#define OFFICINA_CONEXIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "officina_medulla.h"
#include "officina_regio.h"

/* ==================================================
 * Genera symbolorum globalium
 * ================================================== */

nomen enumeratio {
    CONEXIO_SYMBOLUM_IGNOTUM = 0,  /* ante nectere: nondum resolutum */
    CONEXIO_SYMBOLUM_FUNCTIO,      /* definitum -> descriptor */
    CONEXIO_SYMBOLUM_DATUM,        /* definitum -> sedes in globalibus */
    CONEXIO_SYMBOLUM_CELLA,        /* externum notum -> cella pontis */
    CONEXIO_SYMBOLUM_DECIPULA      /* insolutum -> descriptor decipulae */
} ConexioSymbolumGenus;

/* signa descriptorum - numquam 0 (memoria zephyrata numquam
 * descriptor validus); M5 addet NATIVUM. AEDIFICATUM: machinula
 * decipulas notas in aedificata IN SITU rescribit (signum solum -
 * inscriptiones descriptorum, iam in relocationibus sarcitae,
 * immotae manent) */
#define CONEXIO_SIGNUM_INTERPRETATUM 1
#define CONEXIO_SIGNUM_DECIPULA      2
#define CONEXIO_SIGNUM_AEDIFICATUM   3

/* in regione residens; inscriptio huius EST valor monstratoris
 * functionis (16 octeti, XVI-ordinata) */
nomen structura {
    s64 signum;
    s64 index;   /* INTERPRETATUM: in functiones nexas;
                  * DECIPULA: in decipulas (titulos) */
} ConexioDescriptor;

nomen structura {
    chorda  titulus;        /* copia in piscina conexionis */
    s32     genus;          /* ConexioSymbolumGenus */
    s32     modulus_index;  /* definitor; -I si nullus */
    s32     index_localis;  /* functio/datum in definitore; -I */
    vacuum* sedes;          /* inscriptio finalis (post nectere) */
} ConexioSymbolum;

/* machinula per descriptor.index hanc invenit: functio + modulus
 * (operanda symbolorum instructionum indices LOCALES sunt -
 * translatio moduli eos resolvit) */
nomen structura {
    MedullaFunctio* functio;
    s32             modulus_index;
} ConexioFunctioNexa;

nomen structura Conexio Conexio;

/* ==================================================
 * Vita + nexus
 * ================================================== */

Conexio* conexio_creare (Piscina* piscina, Regio* regio);

/* FALSUM: post nectere, aut definitio duplex (querela ponitur;
 * conexio post fracturam invalida) */
b32 conexio_modulum_addere (Conexio* conexio, MedullaModulus* modulus);

/* collocat data, descriptores, cellas, decipulas; relocationes
 * ADDITIVE sarcit. Decipulae NON sunt fracturae (numeratae +
 * nominatae). FALSUM = fractura vera (querela). */
b32 conexio_nectere (Conexio* conexio);

constans chorda* conexio_querela (constans Conexio* conexio);
/* symbolum querelae (mensura 0 si nullum) */
constans chorda* conexio_querela_symbolum (constans Conexio* conexio);

/* ==================================================
 * Quaestio (post nectere)
 * ================================================== */

/* -I si absens */
s32 conexio_symbolum_quaerere (constans Conexio* conexio,
    chorda titulus);
constans ConexioSymbolum* conexio_symbolum_obtinere (
    constans Conexio* conexio, s32 index);
i32 conexio_numerus_symbolorum (constans Conexio* conexio);

/* sedes per titulum; NIHIL si absens ($main -> descriptor) */
vacuum* conexio_sedes_quaerere (constans Conexio* conexio,
    chorda titulus);

/* (modulus, index localis) -> index globalis; -I si invalidus */
s32 conexio_symbolum_globale (constans Conexio* conexio,
    s32 modulus_index, s32 index_localis);

constans ConexioFunctioNexa* conexio_functionem_obtinere (
    constans Conexio* conexio, s64 index);
i32 conexio_numerus_functionum (constans Conexio* conexio);

i32 conexio_numerus_modulorum (constans Conexio* conexio);
constans MedullaModulus* conexio_modulum_obtinere (
    constans Conexio* conexio, s32 index);

/* ==================================================
 * Census (vectis M2a: decipulae numeratae + nominatae)
 * ================================================== */

i32 conexio_numerus_datorum (constans Conexio* conexio);
i32 conexio_numerus_cellarum (constans Conexio* conexio);
i32 conexio_numerus_decipularum (constans Conexio* conexio);
constans chorda* conexio_decipulam_obtinere (
    constans Conexio* conexio, s32 index);

#endif /* OFFICINA_CONEXIO_H */
