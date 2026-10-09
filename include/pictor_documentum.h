/* pictor_documentum.h - Documentum pictoris = cauda ictuum
 *
 * Veritas est ACTA (volumen: solum-appende); proiectio (bitmap) est
 * derivata. Machina caudae - rami, checkpoints, cursor, revocare,
 * reficere, verificare - est `historia` (scriba-plan H2; leges ibi);
 * hic pars pictoris sola: pixela RGBA, vacatio colore fundi thematis
 * (COLOR_BACKGROUND), ictus pingere,
 * manifestum 'documentum' (dimensiones, intervallum).
 *
 * Acta v1: <ictus instrumentum color magnitudo [semen]><punctum x y
 * [t]/>...</ictus>, <ramus ab/>. Cetera (§4) ignorantur cum nota.
 * instrumentum "aspergillum" (MacPaint): guttae (pixela singula) in
 * disco circa quodque punctum; GUTTAE_PUNCTO per punctum et una plus
 * per GUTTA_MS morae (t, ms ab initio ictus) - eaedem semper ex
 * semine. instrumentum "spongia": quadratum SPONGIAE_LATUS x magnitudo
 * centratum in quoque puncto lineae inter puncta, colore fundi
 * thematis (ut vacatio); color ignoratur. Instrumentum absens aut
 * aliud: penicillus.
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
#include "historia.h"
#include "tabula_pixelorum.h"   /* typus solus: fenestra.h Cocoa
                                  * in terminalem trahebat (013 A4) */
#include "imago_typus.h"

#define PICTOR_ASPERGILLI_RADIUS  VIII   /* x magnitudo */
#define PICTOR_GUTTAE_PUNCTO      VI
#define PICTOR_GUTTA_MS           VIII
#define PICTOR_SPONGIAE_LATUS     XVI    /* x magnitudo */

/* gutta k puncti i ictus: offsetus (dx, dy) in disco radii r,
 * determinatus ex (semen, i, k) per sors */
vacuum
pictor_gutta (
    s64  semen,
    i32  i,
    i32  k,
    s32  radius,
    s32* dx,
    s32* dy);

nomen structura {
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
                    i32  latitudo;
                    i32  altitudo;
                    i32  intervallum;    /* acta per checkpoint */
        TabulaPixelorum* tabula;         /* proiectio (memoria) */
                  Imago  proiectio;      /* eadem memoria */
               Historia* historia;       /* cauda, cursor, sigillum */
} PictorDocumentum;

PictorDocumentum*
pictor_documentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum);

/* ex volumine exsistente: dimensiones ex plagula 'documentum',
 * proiectio ex checkpoint proximo + actis */
PictorDocumentum*
pictor_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium);

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

/* ictus vivi ad cursor */
i32
pictor_documentum_numerus_vivorum (
    constans PictorDocumentum* doc);

#endif /* PICTOR_DOCUMENTUM_H */
