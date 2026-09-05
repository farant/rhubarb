/* actio.h - Registrum actionum NOMINATARUM
 *
 * Actiones in marcatione NOMINANTUR, numquam computantur
 * (laboratorium 0011). Una actio per INTENTIONEM; phasis ex genere
 * eventus (DECISUS). Registrum utrimque resolvitur (lint L10): nomen
 * in arbore sine tractatore = error; tractator a nemine relatus =
 * monitum.
 *
 * <tractator/>-lint L5: tractatores I/O non faciunt; effectus ut
 * data.
 */

#ifndef ACTIO_H
#define ACTIO_H

/* <aedilis corpus="lib/actio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"
#include "destinatio.h"


/* ==================================================
 * Typi
 * ================================================== */

/* Redde VERUM si consumptus (ascensus sistit). nodus = componens
 * cuius actio congruit (potest esse maior destinati). */
nomen b32 (*ActioFn)(
    InsulaRepositorium* repo,
                Motus* motus,
   constans Destinatio* destinatio,
            Componens* nodus,
     constans Eventus* ev,
               vacuum* ctx);

nomen structura {
                    Xar* nomina;      /* Xar de chorda (internatae) */
                    Xar* functiones;  /* Xar de ActioFn */
                    Xar* contextus;   /* Xar de vacuum* */
                Piscina* piscina;
    InternamentumChorda* intern;
} ActioRegistrum;


/* ==================================================
 * Registrum
 * ================================================== */

ActioRegistrum*
actio_registrum_creare (
                Piscina* piscina,
    InternamentumChorda* intern);

/* FALSUM si titulus vacuus, fn NIHIL, aut titulus iam registratus. */
b32
actio_registrare (
        ActioRegistrum* reg,
    constans character* titulus,
               ActioFn  fn,
                vacuum* ctx);

b32
actio_invenire (
    constans ActioRegistrum*  reg,
                     chorda   titulus,
                    ActioFn*  fn_ex,
                     vacuum** ctx_ex);


/* ==================================================
 * Resolutio utrimque (L10)
 * ================================================== */

/* Nomina in arbore relata sine tractatore (semel quodque). */
Xar*
actio_non_registratae (
    constans ActioRegistrum* reg,
                  Componens* arbor,
                    Piscina* piscina);

/* Tractatores registrati a nullo componente relati. */
Xar*
actio_non_relatae (
    constans ActioRegistrum* reg,
                  Componens* arbor,
                    Piscina* piscina);

#endif /* ACTIO_H */
