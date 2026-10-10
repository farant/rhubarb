/* historia.h - cauda actorum cum proiectione: revocare, reficere, rami
 *
 * Machina documenti cuius VERITAS est acta (volumen: solum-appende)
 * et cuius proiectio (memoria fixae mensurae, a cliente possessa)
 * derivata est: checkpoint proximus VIVUS + acta viva post eum.
 * Ex pictor_documentum extracta (scriba-plan H1); clientes:
 * pictor_documentum (pixela), scriba (craticula textus).
 *
 * LEGES (ex pictore, aurum fixa/pictor_documentum/aurum.txt):
 * - Undo/redo = cursor in memoria; actum novum post revocationem
 *   RAMUM prius appendit (<ramus ab="seq"/>): acta inter ab et ramum
 *   mortua sunt - cauda numquam truncatur, historia numquam mentitur.
 * - Checkpoint omni 'intervallo' actorum vivorum: massa (memoria
 *   proiectionis) sigillo addressata + plagula 'checkpoint/<seq>' ->
 *   sigillum hex, origine clientis. Checkpoint in spatio MORTUO
 *   numquam basis est.
 * - Volumen acta sua interserit (volumen-creatum, plagula-condita):
 *   solum acta generis clientis applicantur; seqs checkpointorum
 *   multipla intervalli non sunt.
 * - Verificare = reproicere ex nihilo (sine checkpoint) et cum
 *   sigillo NOTATO (post actum ultimum applicatum) conferre: proiectio
 *   incrementalis ab actis discors deprehenditur si signata est;
 *   memoria extra machinam POST signationem mutata non videtur
 *   (reproiectio eam tacite reficit).
 *
 * - LOCUS REVOCANDI SERVATUR (R5, Franus): revocare et reficere notam
 *   <cursor ad="seq"/> appendunt; aperire ad notam ultimam redit nisi
 *   actum, ramus aut coniunctio eam secutum est (tum ad finem).
 * - SPATIUM NOMINUM (insula-rami-plan R2): documenta plura in volumine
 *   uno - genus actorum, notae (ramus, coniunctio) et claves
 *   checkpointorum praefixum 'spatium/' ferunt; historia sua sola
 *   legit. Spatium vacuum = nomina nuda (octeti veteres).
 *
 * Historia volumen non describit: manifestum documenti (dimensiones,
 * intervallum) res clientis est. */

#ifndef HISTORIA_H
#define HISTORIA_H

/* <aedilis corpus="lib/historia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "sigillum.h"


/* ==================================================
 * Typi
 * ================================================== */

/* Proiectio clientis: memoria fixae mensurae quam historia
 * checkpointis implet et sigillo signat; vacare = status ante actum
 * primum; applicare = actum unum (datum crudum, genere clientis).
 *
 * pictor-strata L1: proiectio SINE memoria fixa - cliens ipse statum
 * signat (sigillare: status crudus canonicus, post omne actum) et
 * checkpoint codificat (codificare; mensura 0 = nullus) et restituit
 * (decodificare; FALSUM = codex inutilis, e.g. formae veteris -
 * reproiectio ex nihilo). Aut omnes tres aut nullus; cum eis memoria
 * NIHIL et mensura 0 licent. Sine eis: memoria cruda, ut ante. */
nomen structura {
                i8* memoria;
    memoriae_index  mensura;
            vacuum (*vacare)(vacuum* ctx);
            vacuum (*applicare)(vacuum* ctx, chorda actum);
            vacuum* ctx;
            vacuum (*sigillare)(vacuum* ctx, Sigillum* exitus);
            chorda (*codificare)(vacuum* ctx, Piscina* piscina);
               b32 (*decodificare)(vacuum* ctx, chorda codex);
} HistoriaProiectio;

nomen structura {
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
      HistoriaProiectio  proiectio;
     constans character* spatium;          /* R2; "" = nullum */
     constans character* genus;            /* actorum (cum spatio) */
     constans character* genus_rami;
     constans character* genus_coniunctionis;
     constans character* genus_cursoris;   /* R5 */
     constans character* praefixum_checkpoint;
     constans character* origo_checkpoint; /* "pictor:checkpoint" */
                    i32  intervallum;      /* acta viva inter cp. */
                    s64  cursor;           /* acta applicata (seq) */
                    s64  finis;            /* seq ultimum vivum */
                    i32  numerus_vivorum;  /* acta viva ad cursor */
               Sigillum  sigillum;         /* proiectionis currentis */
} Historia;


/* ==================================================
 * Vita
 * ================================================== */

/* historia nova: proiectio vacatur, cursor 0. Volumen non tangitur.
 * intervallum <= 0 -> LXIV. NIHIL si argumenta mala. */
Historia*
historia_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
     constans character* genus,
     constans character* origo_checkpoint,
                    i32  intervallum,
      HistoriaProiectio  proiectio);

/* ex volumine exsistente: finis = actum vivum ultimum, cursor =
 * finis, proiectio ex checkpoint proximo vivo + actis */
Historia*
historia_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
     constans character* genus,
     constans character* origo_checkpoint,
                    i32  intervallum,
      HistoriaProiectio  proiectio);


/* ==================================================
 * Acta, revocare, reficere
 * ================================================== */

/* appendit (ramus prius si cursor < finis), applicat, checkpoint si
 * debetur. Redde seq (> 0) aut 0 si recusatum. */
s64
historia_actum (
    Historia* h,
      chorda  actum);

/* ut historia_actum, sed actum PRIORI coniunctum (S1b): revocare et
 * reficere gregem coniunctum gradu uno transeunt. Nota historiae
 * (<coniunctio/>, genus "coniunctio") ante actum appenditur; sine
 * actu priore vivo actum simplex est. */
s64
historia_actum_coniunctum (
    Historia* h,
      chorda  actum);

/* cursor ad actum vivum ante gregem currentem (actum unum si non
 * coniunctum); FALSUM ad initium */
b32
historia_revocare (
    Historia* h);

/* cursor ad actum vivum proximum et coniuncta ei; FALSUM ad finem
 * (et post ramum) */
b32
historia_reficere (
    Historia* h);


/* ==================================================
 * Lectio et verificatio
 * ================================================== */

/* reproicere ex nihilo (nullo checkpoint) et sigilla conferre */
b32
historia_verificare (
    Historia* h);

chorda
historia_sigillum_hex (
    constans Historia* h,
              Piscina* piscina);

s64
historia_cursor (
    constans Historia* h);

s64
historia_finis (
    constans Historia* h);

i32
historia_numerus_vivorum (
    constans Historia* h);

#endif /* HISTORIA_H */
