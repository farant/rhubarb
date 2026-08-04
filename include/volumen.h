/* volumen.h - volumen (rotulus documentorum): documentum = plagula
 * sqlite una, veritas eventilis intra
 *
 * NOMEN NONDUM SIGILLATUM. Semen G2 (tabula lacunarum
 * project-specs/compages-rationes-tutorial.md): documentum totum in
 * plagula UNA vivit - cp voluminis = cp documenti. Intra:
 *
 *   acta      veritas: cauda eventuum solum-appende (disciplina
 *             gestarum, sed per-plagulam non per-repositorium;
 *             momentum in SQL stampatur - exemplar gesta.c)
 *   massae    contenta sigillo addressata (SHA-256 hex; contentum
 *             idem semel conditur)
 *   plagulae  proiectio: manifestum viarum -> sigilla. Reficibilis
 *             ex actis + massis - corruptio proiectionis numquam
 *             amissio datorum.
 *
 * Consumptor primus: silex (novum). Crescit tractione sola -
 * genera/custodia/personae hic OLIM habitabunt (visio), non nunc.
 *
 * Usus:
 *   Volumen* vol = volumen_creare(piscina, "001/001.volumen");
 *   volumen_plagulam_condere(vol, via, contentum,
 *       "vendicata:lib/chorda.c");
 *   chorda c = volumen_plagulam_promere(vol, via, piscina,
 *       &inventum);
 *   volumen_claudere(vol);
 */

#ifndef VOLUMEN_H
#define VOLUMEN_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "sigillum.h"

nomen structura Volumen Volumen;

/* ordo manifesti (plagulae enumeratae); chordae ex piscina vocantis */
nomen structura {
    chorda via;
    chorda sigillum_hex;
    chorda origo;
} VolumenPlagula;

/* creare: plagula NON exsistat (recusatio clara si exsistit);
 * migrationes applicantur, actum 'volumen-creatum' scribitur */
Volumen*
volumen_creare (
    Piscina*            piscina,
    constans character* via);

/* aperire: plagula exsistat */
Volumen*
volumen_aperire (
    Piscina*            piscina,
    constans character* via);

vacuum
volumen_claudere (
    Volumen* volumen);

/* nuntius erroris ultimi (litterae - vita voluminis); NIHIL si nullus */
constans character*
volumen_error (
    constans Volumen* volumen);

/* actum in caudam appendere (datum = JSON); seq redditum, 0 = vitium */
s64
volumen_actum_appendere (
    Volumen*            volumen,
    constans character* genus,
    chorda              datum);

/* plagulam condere: transactione una - massa (sigillo dedup) +
 * actum 'plagula-condita' + manifestum (upsert per viam).
 * origo e.g. "vendicata:lib/chorda.c" | "genita" */
b32
volumen_plagulam_condere (
    Volumen*            volumen,
    chorda              via_relativa,
    chorda              contentum,
    constans character* origo);

/* plagulam e manifesto removere: actum 'plagula-remota' + DELETE.
 * Massa manet (contentum sigillo addressatum - historia eam adhuc
 * spectat); VERUM etiam si via iam aberat (idempotens). */
b32
volumen_plagulam_removere (
    Volumen* volumen,
    chorda   via_relativa);

/* transactio vocatoris (conditio multi-plagularis atomica):
 * inter incipere/committere, plagulam_condere/removere transactiones
 * proprias NON aperiunt (sqlite nidificationem vetat) - vocator
 * possidet. Sine his: quaeque operatio transactionem propriam habet. */
b32
volumen_transactionem_incipere (
    Volumen* volumen);

b32
volumen_transactionem_committere (
    Volumen* volumen);

b32
volumen_transactionem_revolvere (
    Volumen* volumen);

/* contentum plagulae ex manifesto + massis; *inventum FALSUM si via
 * ignota (chorda vacua legitima est - plagula vacua) */
chorda
volumen_plagulam_promere (
    Volumen* volumen,
    chorda   via_relativa,
    Piscina* piscina,
    b32*     inventum);

/* massam nudam condere (contentum sigillo addressatum, dedup) -
 * massa CONTENTUM est, non eventus: veritas de USU eius in actis
 * vivit (e.g. mensa:creatum cum clave imago). sigillum_hex_exitus
 * >= SIGILLUM_HEX_MENSURA octeti. */
b32
volumen_massam_condere (
    Volumen*   volumen,
    chorda     contentum,
    character* sigillum_hex_exitus);

/* massam promere sigillo (hex); *inventum FALSUM si ignotum */
chorda
volumen_massam_promere (
    Volumen* volumen,
    chorda   sigillum_hex,
    Piscina* piscina,
    b32*     inventum);

/* manifestum totum, ordine viae; Xar de VolumenPlagula */
Xar*
volumen_plagulas_enumerare (
    Volumen* volumen,
    Piscina* piscina);

/* actum lectum (chordae ex piscina vocantis) */
nomen structura {
    s64    seq;
    chorda momentum;
    chorda genus;
    chorda datum;
} VolumenActum;

/* acta POST seq datum (exclusivum; 0 = omnia), ordine seq -
 * forma "acta post seq N" quam et plicae et replicatio volunt */
Xar*
volumen_acta_legere (
    Volumen* volumen,
    s64      post_seq,
    Piscina* piscina);

s64
volumen_summa_actorum (
    Volumen* volumen);

s64
volumen_summa_plagularum (
    Volumen* volumen);

/* massae distinctae (dedup sigillo probabilis) */
s64
volumen_summa_massarum (
    Volumen* volumen);

#endif /* VOLUMEN_H */
