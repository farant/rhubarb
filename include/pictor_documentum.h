/* pictor_documentum.h - Documentum pictoris = cauda ictuum
 *
 * Veritas est ACTA (volumen: solum-appende); proiectio (bitmap) est
 * derivata: checkpoint proximus + acta post eum. Undo/redo = cursor
 * in memoria; ictus novus post revocationem RAMUM appendit
 * (<ramus ab="seq"/>): acta inter ab et ramum mortua sunt - cauda
 * numquam truncatur, historia numquam mentitur. Checkpoint omni
 * intervallo: massa (pixela RGBA) sigillo addressata + plagula
 * 'checkpoint/<seq>' -> sigillum hex. Gradus VIII: verificare =
 * reproicere ex nihilo et sigilla conferre.
 *
 * Acta v1: <ictus instrumentum color magnitudo><punctum x y/>...
 * </ictus>, <ramus ab/>. Cetera (§4) ignorantur cum nota.
 */

#ifndef PICTOR_DOCUMENTUM_H
#define PICTOR_DOCUMENTUM_H

/* <aedilis corpus="lib/pictor_documentum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "sigillum.h"
#include "fenestra.h"
#include "imago_typus.h"

nomen structura {
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
                    i32  latitudo;
                    i32  altitudo;
                    i32  intervallum;    /* acta per checkpoint */
        TabulaPixelorum* tabula;         /* proiectio (memoria) */
                  Imago  proiectio;      /* eadem memoria */
                    s64  cursor;         /* acta applicata (seq) */
                    s64  finis;          /* seq ultimum vivum */
                    i32  numerus_vivorum; /* ictus vivi ad cursor */
               Sigillum  sigillum;       /* proiectionis currentis */
} PictorDocumentum;

PictorDocumentum*
pictor_documentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum);

/* ex volumine exsistente: dimensiones ex plagula 'documentum',
 * proiectio ex checkpoint proximo + actis */
PictorDocumentum*
pictor_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen);

/* appendit (ramus prius si cursor < finis), applicat, checkpoint si
 * debetur. Redde seq (> 0) aut 0 si recusatum. */
s64
pictor_documentum_actum (
    PictorDocumentum* doc,
              chorda  actum_stml);

b32
pictor_documentum_revocare (
    PictorDocumentum* doc);

b32
pictor_documentum_reficere (
    PictorDocumentum* doc);

constans Imago*
pictor_documentum_proiectio (
    constans PictorDocumentum* doc);

chorda
pictor_documentum_sigillum_hex (
    constans PictorDocumentum* doc,
                      Piscina* piscina);

/* reproicere ex nihilo (nullo checkpoint) et sigilla conferre */
b32
pictor_documentum_verificare (
    PictorDocumentum* doc);

s64
pictor_documentum_cursor (
    constans PictorDocumentum* doc);

s64
pictor_documentum_finis (
    constans PictorDocumentum* doc);

#endif /* PICTOR_DOCUMENTUM_H */
