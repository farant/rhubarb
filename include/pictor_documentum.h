/* pictor_documentum.h - Documentum pictoris = cauda ictuum
 *
 * Veritas est ACTA (volumen: solum-appende); proiectio (bitmap) est
 * derivata. Machina caudae - rami, checkpoints, cursor, revocare,
 * reficere, verificare - est `historia` (scriba-plan H2; leges ibi);
 * hic pars pictoris sola: pixela RGBA, vacatio colore fundi thematis
 * (COLOR_BACKGROUND), ictus pingere,
 * manifestum 'documentum' (dimensiones, intervallum).
 *
 * Acta v1: <ictus instrumentum color magnitudo [color_secundus]
 * [exemplar] [semen]><punctum x y [t]/>...</ictus>, <ramus ab/>.
 * Cetera (§4) ignorantur cum nota.
 * instrumentum "aspergillum" (MacPaint): guttae (pixela singula) in
 * disco radii ASPERGILLI_RADIUS x magnitudo circa quodque punctum;
 * GUTTAE_PUNCTO x magnitudo per punctum et magnitudo plus per GUTTA_MS
 * morae (t, ms ab initio ictus) - eaedem semper ex semine.
 * instrumentum "spongia": quadratum SPONGIAE_LATUS x magnitudo
 * centratum in quoque puncto lineae inter puncta, colore fundi
 * thematis (ut vacatio); color ignoratur. Instrumentum absens aut
 * aliud: penicillus: discus diametri magnitudo (pixela) in puncto primo
 * et in quoque puncto lineae inter puncta (pictor_lineam_ambulare).
 * instrumentum "linea": ut penicillus (puncta II, segmentum).
 * exemplar (P3; exemplaria.h, absens = 0 solidus): pixelum quod
 * penicillus aut aspergillum pingit colorem 'color' accipit ubi bitus
 * exemplaris ad (x, y) TABULAE positus est, 'color_secundus' ubi non;
 * color -1 (nullus; color_secundus absens = -1) = pixelum intactum.
 * Spongia exemplar ignorat.
 *
 * STRATA (pictor-strata L2; project-specs/pictor-strata-plan.md):
 *   <ictus ... stratum="2">...</ictus>          ictus in strato 2
 *                                              (absens = 1)
 *   <stratum actio="novum" id="2" supra="1"/>   stratum 2 supra 1
 *   <stratum actio="deletum" id="2"/>           ultimum numquam
 *   <stratum actio="ordo" ids="1 3 2"/>         imum primum;
 *                                              permutatio sola
 *   <stratum actio="visibile" id="2" valor="0"/>  occultare (1:
 *                                              ostendere)
 * Strata perspicua incipiunt (alpha 0); tabula = COMPOSITUM: color
 * fundi thematis, deinde strata visibilia ab imo. Spongia stratum
 * suum ad perspicuum purgat. Checkpoint codificatus ("STRATA1",
 * flatura); checkpoint vetus (massa cruda) recusatur -> reproiectio ex
 * actis.
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
#define PICTOR_STRATA_MAXIMA      XVI    /* Franus: augebitur - nulla
                                          * forma servata hunc numerum
                                          * supponit */

/* vestigium lineae: vocatur in puncto (x, y) */
nomen vacuum (*PictorVestigium) (s32 x, s32 y, vacuum* ctx);

/* puncta lineae (Bresenham) a (x0, y0) ad (x1, y1), initio EXCLUSO,
 * fine incluso - regula penicilli et spongiae (documentum et
 * praevisio eandem sequuntur) */
vacuum
pictor_lineam_ambulare (
                s32  x0,
                s32  y0,
                s32  x1,
                s32  y1,
    PictorVestigium  vestigium,
             vacuum* ctx);

/* discus penicilli diametri n: pixelum (i, j) quadrati n x n (origo
 * [x - n/2, y - n/2]) intra discum? (n <= III: quadratum plenum) */
b32
pictor_disci_pixelum (
    s32 n,
    s32 i,
    s32 j);

/* S3e: bibliotheca - spatia omnium documentorum pictoris in volumine
 * (plagulae originis "pictor:documentum"), ordine viae; Xar de chorda
 * ("" = documentum nudum radicis) */
Xar*
pictor_documenta_enumerare (
     Volumen* volumen,
     Piscina* piscina);

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

/* stratum (pictor-strata L2): id (1, 2, ... - nomen "stratum <id>"),
 * visibile, pixela RGBA (alpha 0 = perspicuum) */
nomen structura {
                s32  id;
                b32  visibile;
    TabulaPixelorum* pixela;
} PictorStratum;

nomen structura {
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
                    i32  latitudo;
                    i32  altitudo;
                    i32  intervallum;    /* acta per checkpoint */
        TabulaPixelorum* tabula;         /* COMPOSITUM (L2) */
                  Imago  proiectio;      /* eadem memoria */
          PictorStratum  strata[PICTOR_STRATA_MAXIMA]; /* ordine: imum
                                                        * primum */
                    i32  numerus_stratorum;
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

/* strata ordine (imum primum); NIHIL si index extra */
constans PictorStratum*
pictor_documentum_stratum (
    constans PictorDocumentum* doc,
                          i32  index);

i32
pictor_documentum_numerus_stratorum (
    constans PictorDocumentum* doc);

/* sigillum compositi (pixela visa) - picturae ante strata: idem ac
 * sigillum vetus */
chorda
pictor_documentum_sigillum_compositi_hex (
    constans PictorDocumentum* doc,
                      Piscina* piscina);

#endif /* PICTOR_DOCUMENTUM_H */
