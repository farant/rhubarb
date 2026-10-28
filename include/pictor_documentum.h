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
 *   <imago stratum x y latitudo altitudo massa/>  imago glutinata
 *                                              (L5): indices palettae
 *                                              deflati in massa
 *                                              voluminis (0xFF
 *                                              perspicuum)
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
#include "tabula_dispersa.h"

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

/* pinacotheca (spec D5): pictura et seq actus sui ultimi - ictus,
 * ramus, cursor, coniunctio historiae suae, aut conditio plagulae
 * 'documentum' (pictura vacua) */
nomen structura {
    chorda spatium;   /* "" = radix */
       s64 seq;
} PictorPictura;

/* picturae voluminis (ut pictor_documenta_enumerare), seq
 * DESCENDENTE (aequales: ordine viae); transitus unus actorum
 * voluminis. Aspicere (aperire) nihil appendit, ergo non movet;
 * revocatio cursorem appendit, ergo movet. Xar de PictorPictura */
Xar*
pictor_documenta_recentia (
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

/* GLUTINATIO PENDENS (Franus 2026-10-13): Cmd+V imaginem aptat et BIS
 * ditherat - A Atkinson, B dithering_exemplaria - in memoria documenti,
 * EXTRA acta (nihil in cauda, nulla revocatio). Tabula praevisionem
 * super compositum pingit; confirmare = imaginem_inserere electi
 * (acta coniuncta, revocatio una); abicere = acta intacta. */
nomen enumeratio {
    PICTOR_GLUTINATIO_ATKINSON = ZEPHYRUM,  /* "A" */
    PICTOR_GLUTINATIO_EXEMPLARIA,           /* "B" */
    PICTOR_GLUTINATIO_MODI
} PictorGlutinatioModus;

nomen structura {
          b32  pendens;
          s32  x;          /* locus in documento (centratus, contain) */
          s32  y;
          i32  latitudo;
          i32  altitudo;
           i8* indices[PICTOR_GLUTINATIO_MODI];    /* 0xFF perspicuum */
        Imago  praevisio[PICTOR_GLUTINATIO_MODI];  /* RGBA, alpha 0
                                                    * perspicuum */
    character sigillum[PICTOR_GLUTINATIO_MODI][SIGILLUM_HEX_MENSURA];
                                    /* identitas praevisionis */
           i8* larva;      /* latitudo x altitudo: 1 = modus ALTER hic
                            * (radendo, Franus 2026-10-13); praevisio
                            * [m] = larva ? indices alterius : [m] */
} PictorGlutinatio;

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
       PictorGlutinatio  glutinatio;     /* pendens, extra acta */
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

/* communio T4: pictura ALTERIUS machinae (acta importata, manifestum
 * sub "aliena/<replica>/..."): SOLUM LEGITUR (historia_aperire_
 * alienam) - actum, revocare, reficere recusantur. NIHIL si ignota */
PictorDocumentum*
pictor_documentum_aperire_alienum (
                 Piscina* piscina,
     InternamentumChorda* intern,
                 Volumen* volumen,
                  chorda  replica,
      constans character* spatium);

/* communio T5: acta advecta sequi (historia_sequi; strata et
 * compositum per proiectionem renovantur). VERUM si mutata */
b32
pictor_documentum_sequi (
    PictorDocumentum* doc);

/* picturae aliae machinae in volumine (manifestum "aliena/<replica>/
 * [<spatium>/]documentum", origo pictor:documentum) */
nomen structura {
    chorda replica;
    chorda spatium;   /* "" = radix illius machinae */
       s64 hora;      /* plan-5 T3: hora scriptoris actus ultimi (0 =
                       * ignota; solum pictor_aliena_recentia implet) */
} PictorPicturaAliena;

Xar*   /* de PictorPicturaAliena */
pictor_documenta_aliena_enumerare (
     Volumen* volumen,
     Piscina* piscina);

/* plan-5 T3: picturae aliae machinarum ("Shared pictures"), hora
 * actus ultimi SCRIPTORIS descendente (aequales: replica, deinde
 * spatium); actus "plagula-condita" picturam vacuam datat. Propriae
 * numquam. Xar de PictorPicturaAliena (hora impleta) */
Xar*
pictor_aliena_recentia (
     Volumen* volumen,
     Piscina* piscina);

/* plan-5 T4 (D4 "Edit a copy"): origo picturae propriae in manifesto
 * suo (attributum origo="<replica>:<spatium>") - ponere manifestum
 * iterum condit (actum plagula-condita); FALSUM si pictura ignota aut
 * signa mala (", <, &). legere: FALSUM si pictura ignota aut sine
 * origine; chordae in piscina. */
b32
pictor_originem_ponere (
               Volumen* volumen,
    constans character* spatium,
                chorda  replica,
    constans character* spatium_alienum,
               Piscina* piscina);

b32
pictor_originem_legere (
               Volumen* volumen,
    constans character* spatium,
               Piscina* piscina,
                chorda* replica,
                chorda* spatium_alienum);

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

/* imago RGBA -> indices palettae Aquinas, "contain" in tabulam
 * (latitudo x altitudo): scala (area deorsum, bilinearis sursum),
 * Atkinson colorum (XVI), alpha < CXXVIII perspicuum (0xFF),
 * centrata - (x, y) et dimensiones in exitibus. NIHIL si fons vacua. */
i8*
pictor_imaginem_aptare (
    constans Imago* fons,
               i32  latitudo,
               i32  altitudo,
           Piscina* piscina,
               s32* x,
               s32* y,
               i32* lat,
               i32* alt);

/* pinax (pinacotheca D4): compositum picturae 'spatium' (strata
 * visibilia) in latitudo x altitudo "contain" (pictor_imaginem_aptare:
 * scala et Atkinson in palettam), RGBA; extra picturam fundus
 * thematis (COLOR_BACKGROUND). Omnia in 'piscina' (scheda vocantis);
 * volumen non scribitur. FALSUM si pictura ignota aut magnitudo
 * nulla. */
b32
pictor_pinacem_facere (
                Volumen* volumen,
    InternamentumChorda* intern,
     constans character* spatium,
                    i32  latitudo,
                    i32  altitudo,
                Piscina* piscina,
                  Imago* exitus);

/* plan-5 T3: pinax picturae ALIENAE (replica, spatium) - idem ac
 * pictor_pinacem_facere apud scriptorem. FALSUM si ignota (aut
 * replica propria) */
b32
pictor_pinacem_alienum_facere (
                Volumen* volumen,
    InternamentumChorda* intern,
                 chorda  replica,
     constans character* spatium,
                    i32  latitudo,
                    i32  altitudo,
                Piscina* piscina,
                  Imago* exitus);

/* PINACOTHECA (spec D3-D5): status dum aperta, in montatione
 * habitans; compositio, actiones et figurae eam per indicem legunt.
 * scheda VACATUR quaque apertione (memoria = sessio una), pinaces in
 * ea facti per provenientiam memorantur. */
nomen structura {
                Volumen* volumen;
    InternamentumChorda* intern;
                Piscina* scheda;     /* NIHIL donec prima apertio */
                    Xar* picturae;   /* PictorPictura, recentia */
         TabulaDispersa* pinaces;    /* provenientia -> Imago* */
} PictorPinacotheca;

/* apertio: scheda vacatur (aut creatur), picturae relegitur
 * (pictor_documenta_recentia), pinaces vacui. FALSUM si memoria */
b32
pictor_pinacothecam_parare (
    PictorPinacotheca* p);

/* communio-plan-5 T6: "Shared pictures" - status ut pinacotheca
 * (dum aperta, in montatione): picturae aliae machinarum
 * (pictor_aliena_recentia) et pinaces earum in scheda */
nomen structura {
                Volumen* volumen;
    InternamentumChorda* intern;
                Piscina* scheda;     /* NIHIL donec prima apertio */
                    Xar* picturae;   /* PictorPicturaAliena, recentia */
         TabulaDispersa* pinaces;    /* provenientia -> Imago* */
} PictorCommunia;

/* apertio aut renovatio (acta advecta): scheda vacatur, picturae
 * relegitur, pinaces vacui. FALSUM si memoria */
b32
pictor_communia_parare (
    PictorCommunia* p);

/* provenientia "alienus:<lat>x<alt>:<hora>:<replica>:<spatium>" ->
 * pinax picturae alienae (hora in clave: pictura mutata pinacem novum
 * habet). NIHIL si forma mala, ignota, aut ante parare. */
constans Imago*
pictor_communia_pinax (
     PictorCommunia* p,
             chorda  provenientia);

/* index picturae (replica, spatium) in p->picturae; -1 si abest */
s32
pictor_communia_index (
    constans PictorCommunia* p,
                     chorda  replica,
                     chorda  spatium);

/* provenientia "pinax:<lat>x<alt>:<seq>:<spatium>" -> pinax (in
 * scheda factus si nondum; seq in clave, ergo pictura mutata pinacem
 * novum habet). NIHIL si forma mala, pictura ignota, aut ante
 * parare. */
constans Imago*
pictor_pinacotheca_pinax (
    PictorPinacotheca* p,
               chorda  provenientia);

/* indices (lat x alt, 0xFF perspicuum) in stratum NOVUM supra
 * 'supra' ad (x, y): massa deflata in volumine, acta <stratum
 * novum> + <imago> coniuncta (revocatio una). Redde id strati novi
 * aut 0. */
s32
pictor_documentum_imaginem_inserere (
     PictorDocumentum* doc,
          constans i8* indices,
                  i32  lat,
                  i32  alt,
                  s32  x,
                  s32  y,
                  s32  supra);

/* glutinatio pendens (vide supra): fons aptatur (ut
 * pictor_imaginem_aptare) et bis ditheratur; prior pendens abicitur.
 * FALSUM si fons vacua (tunc nihil mutatur) */
b32
pictor_documentum_glutinationem_parare (
    PictorDocumentum* doc,
      constans Imago* fons);

/* modus electus (per pixelum: larva ? alter : electus) in stratum
 * novum supra 'supra'; pendens finitur.
 * Redde id strati aut 0 (tunc pendens manet) */
s32
pictor_documentum_glutinationem_confirmare (
         PictorDocumentum* doc,
    PictorGlutinatioModus  modus,
                      s32  supra);

vacuum
pictor_documentum_glutinationem_abicere (
    PictorDocumentum* doc);

/* radere (Franus 2026-10-13): discus diametri n (ut penicillus) per
 * segmentum (x0, y0) - (x1, y1) coordinatis documenti in larvam;
 * restituere FALSUM = modum alterum revelat, VERUM = electum reponit.
 * Mutato A/B larva manet (vice versa); praevisiones ambae ad pixela
 * tacta renovantur; acta intacta. Extra glutinationem nihil. Redde
 * VERUM si pixelum ullum mutatum */
b32
pictor_documentum_glutinationem_radere (
    PictorDocumentum* doc,
                 s32  x0,
                 s32  y0,
                 s32  x1,
                 s32  y1,
                 s32  n,
                 b32  restituere);

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
