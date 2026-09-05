/* pictor_actiones.h - tractatores pictoris (P3: penicillus.ictus,
 * instrumentum.eligere)
 *
 * <tractator/>: nullum I/O; scripturae per portas solas (insulae:
 * mutare_*, documentum: pictor_documentum_actum - porta documenti).
 * Una actio per INTENTIONEM, phasis ex genere eventus (spec §5.2).
 */

#ifndef PICTOR_ACTIONES_H
#define PICTOR_ACTIONES_H

/* <aedilis corpus="lib/pictor_actiones.c"/> */

#include "latina.h"
#include "actio.h"
#include "pictor_documentum.h"

nomen structura {
    PictorDocumentum* doc;
} PictorActiones;

vacuum
pictor_actiones_registrare (
    ActioRegistrum* reg,
    PictorActiones* ctx);

/* <tractator/> */
b32
pictor_penicillus_ictus (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

/* <tractator/> */
b32
pictor_instrumentum_eligere (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

#endif /* PICTOR_ACTIONES_H */
