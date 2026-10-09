/* pictor_componentia.h - componere pictoris (P3: prospectus, tabula,
 * status). P1a (2026-10-09): status = linea instrumentorum III
 * cellularum: quadrata 'quadratum.instrumentum',
 * 'quadratum.color_primus', 'quadratum.color_secundus' (titulus = quid
 * pingatur, figura_quadrati), deinde nomen instrumenti.
 *
 * <componens/> <purus/>: arbor logica ex insulis (lectio) et motu
 * (lectio) in piscinam datam; nihil scribit. Partes sunt DATA
 * (brainstorm XVI §5): nullus typus per componens, actio nominata
 * ex statu (tabula: ex ephemera.instrumentum - nomen functio pura
 * status, tractator numquam de instrumento ramificat, spec §5.1).
 * Ictus pendens in tabula.puncta COPIATUR ut pingere arborem solam
 * legat (decisum 2026-09-05).
 */

#ifndef PICTOR_COMPONENTIA_H
#define PICTOR_COMPONENTIA_H

/* <aedilis corpus="lib/pictor_componentia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"

nomen structura {
    /* superficies si superficies_* absunt (ante nuntium glutini
     * primum) */
            i32 fenestra_latitudo;
            i32 fenestra_altitudo;
    /* Modulus (013 B3): dispositio in cellulis, pixela = cellulae x
     * cellula */
            i32 cellula_latitudo;
            i32 cellula_altitudo;
    /* linea status in cellulis (Franus: I) */
            i32 status_lineae;
    /* R3: status pictoris; repo NIHIL = radix repositorii componenti
     * dati - structuram TOTAM nulla (memset) ante campos */
    InsulaRamus ramus;
} PictorCompositio;

/* "penicillus" -> "penicillus.ictus"; ignotum -> "" */
constans character*
pictor_actio_instrumenti (
    chorda instrumentum);

/* Componere-formata (dispensator.h): ctx = PictorCompositio* */
Componens*
pictor_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

#endif /* PICTOR_COMPONENTIA_H */
