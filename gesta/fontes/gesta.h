/* gesta.h - nucleus rerum gestarum (event-sourcing) super scrinium
 *
 * K1 (spec: project-specs/gesta-k1-spec-v2.md; decisiones huius
 * plagulae: gesta/phase-log.md chunk A INTENTIO). Acta = tabula
 * tessellarum solum-appende; status entium = PLICATURAE (folds)
 * persistae per consumptores aquae-altae (HWM); genera ipsa per
 * eventus definita (definitio-generis) - replicatio ex nihilo TOTUM
 * reficit, schema inclusum. Proiectio annalium: linea JSON compacta
 * una per eventum, APPENSA ANTE COMMIT (ruina => annales superset,
 * detectum per verificare, numquam historia tacite amissa).
 *
 * REDUCTOR ORDINARIUS (oraculum smaragda.ts:723-774): status entis
 * = obiectum JSON UNUM (clavibus titulus/status inclusis - columnae
 * res sunt proiectiones); mutatio = mersio superficialis clavium
 * summorum (acies/obiecta TOTA substituuntur); remotio delet clavem
 * (remotio != mutatio-ad-nihil!); genus_eventus ignotum = nihil agit
 * (lex progressiva). creatio in re exsistente RECUSATUR (identitas);
 * datum malformatum RECUSATUR (syntaxis non est record-don't-block).
 * Violatio machinae generis: eventus TAMEN scribitur + nota
 * violationis (actor machina) appenditur - iudicat, non obstat.
 *
 * ORDO PORTANS: consumptor generum ANTE consumptorem rerum in
 * quaque plicatura - eventus contra genera eiusdem fascis validant.
 * res_id eventuum generis = titulus generis (legibile, stabile).
 *
 * DISCIPLINA: scriptor unus (WAL scrinii); piscina mundi vivit
 * quamdiu mundus. */

#ifndef GESTA_H
#define GESTA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "scrinium.h"

nomen structura GestaMundus GestaMundus;

/* res_id = ULID (26) + terminator */
#define GESTA_RES_ID_MENSURA 27

/* Eventus scribendus. res_id NIHIL pro creatio (ULID cuditur) et
 * pro definitio-generis/emendatio-generis (titulus generis ex dato
 * sumitur). datum = textus JSON (obiectum; canonicalizatur). */
nomen structura {
    constans character* res_id;
    constans character* genus_eventus;   /* creatio|mutatio|remotio|
                                          * status|nota|
                                          * membrum-additum|
                                          * membrum-remotum|
                                          * definitio-generis|
                                          * emendatio-generis|...
                                          * (nexus/denexus vetera =
                                          * tumuli, K2 D2) */
    constans character* datum;           /* JSON obiectum */
    constans character* actor;           /* fran|claude|machina */
    constans character* origo;           /* sessio/scriptum/uncus; ""
                                          * licet */
} GestaEventum;

/* Aperire mundum: scrinium (migrationes gestae applicantur) +
 * annales (via plagulae JSONL; creatur si abest). NIHIL = fractum. */
GestaMundus*
gesta_aperire (
    Piscina*            piscina,
    constans character* via_scrinii,
    constans character* via_annalium);

vacuum
gesta_claudere (
    GestaMundus* mundus);

/* Scribere eventum: validatio -> transactio (INSERT tessellae +
 * linea annalium + nota sordida) -> plicatura avida (phase-log
 * decisio 1). res_id_out (>= GESTA_RES_ID_MENSURA octeti; NIHIL
 * licet) recipit res_id effectivum. FALSUM = recusatum/fractum
 * (gesta_error consule). */
b32
gesta_scribere (
    GestaMundus*         mundus,
    constans GestaEventum* eventum,
    character*           res_id_out);

/* Plicaturas provehere (genera PRIMUM, deinde res - ordo portans;
 * quaeque transactione sua cum provectione HWM = exacte-semel). */
b32
gesta_plicare (
    GestaMundus* mundus);

/* Replicatio: plicaturae + consumptores deletae, acta a seq I
 * replicata. Barra: fructus == tabulae stantes. */
b32
gesta_replicare (
    GestaMundus* mundus);

/* Annales <-> tessellae identitas 1:1 (seq/id/datum/... congruunt).
 * FALSUM: linea lacera (parse fractum in cauda), linea EXTRA
 * (annales superset - fenestra ruinae, sananda), ordo discrepans. */
b32
gesta_annales_verificare (
    GestaMundus* mundus);

/* Mundum NOVUM ex annalibus restituere (ordines verbatim, seq
 * conservata, deinde replicatio). NIHIL = fractum. */
GestaMundus*
gesta_ex_annalibus_restituere (
    Piscina*            piscina,
    constans character* via_annalium,
    constans character* via_scrinii_novi);

/* ==================================================
 * Quaestio + census (chunk B)
 * ================================================== */

nomen structura {
    chorda res_id;
    chorda genus;
    chorda titulus;
    chorda status;
} GestaInventum;

nomen structura {
    chorda genus;
    chorda status;
    s64    numerus;
} GestaCensusOrdo;

nomen structura {
    chorda tag;
    s64    numerus;
} GestaTagNumerus;

/* Sordidas exhaurire: quaeque res sordida relegi, in res_fts
 * delere-tunc-inserere (BM25 honestum), sordidae purgatae - una
 * transactione (TS: libraries.ts:1074-1098). Columna corpus =
 * state.corpus + paria "clavis: valor" attributorum chordarum +
 * tags iuncta (via 'extra' TS, smaragda.ts:919-924 adaptata). */
b32
gesta_fts_exhaurire (
    GestaMundus* mundus);

/* Quaestio FTS: exhaurit primum (pigra, TS ad-quaestionem);
 * textus NIHIL/vacuus = fructus vacuus (libraries.ts:1117);
 * praefixum = vocans '*' appendit (idioma Latinum); genus/status
 * NIHIL licent (filtra); fructus = Xar de GestaInventum (valore)
 * ordine bm25 (negativius = melius), tectum L ordinibus. Error
 * syntaxis MATCH = fructus vacuus (honestum; citatio = stratum
 * MCP). NIHIL = apparatus fractus. */
Xar*
gesta_quaerere (
    GestaMundus*        mundus,
    constans character* textus,
    constans character* genus,
    constans character* status,
    Piscina*            piscina);

/* Census generum x statuum (ex plicatura res) */
Xar*
gesta_census_generum (
    GestaMundus* mundus,
    Piscina*     piscina);

/* Census tagorum (datum omnium rerum percurritur - volumen parvum) */
Xar*
gesta_census_tagorum (
    GestaMundus* mundus,
    Piscina*     piscina);

/* ==================================================
 * Nexus generibus (K2 chunk A) - vincula ut res propriae
 * ==================================================
 * Genus cum specie "nexus" partes declarat (K2 spec par IV);
 * membra per eventus membrum-additum/-remotum adveniunt et in
 * tabulam membra proiciuntur (consumptor plicaturae - D1). Status
 * "solutum" in re nexus-speciei indicem purgat, res manet. */

nomen structura {
    chorda nexus_res;   /* res vinculi */
    chorda pars;        /* pars quam res quaesita implet */
    chorda genus;       /* genus vinculi ("" si res abest) */
} GestaNexusRei;

nomen structura {
    chorda membrum;     /* res ALTERA */
    chorda pars;        /* pars rei alterius */
    chorda nexus_res;
} GestaSocius;

/* Vincula rem continentia (TS getRelationshipsForEntity,
 * smaragda.ts:4008; ordines crudi - duplicata apparent) */
Xar*
gesta_nexus_rei (
    GestaMundus*        mundus,
    constans character* res_id,
    Piscina*            piscina);

/* Socii per vincula communia (TS getRelatedEntities,
 * smaragda.ts:4119) */
Xar*
gesta_socii_rei (
    GestaMundus*        mundus,
    constans character* res_id,
    Piscina*            piscina);

/* ==================================================
 * Salus (K2 chunk B) - aestimatio pura status contra genus
 * ==================================================
 * Oraculum: _evaluateHealthPure smaragda.ts:4368-4415. NIHIL
 * obstat - salus refert solum. Querelae typi:
 *   attributum-necessarium-absens (gravis) - absens/null/""
 *     (LEX CHORDAE VACUAE, TS :4378)
 *   typus-attributi-pravus (cautio) - typus ignotus TRANSIT
 *     (TS :1796, lex progressiva)
 *   status-ignotus (gravis) - genera sine machina praetereunt
 *   cardinalitas-violata (gravis) - species nexus solum
 * Notae custodiae NON numerantur (divergentia D4). */

nomen structura {
    chorda typus;     /* index querelae supra */
    chorda nuntius;
    b32    gravis;    /* VERUM erratum, FALSUM cautio */
} GestaQuerela;

nomen structura {
    b32           sanus;
    GestaQuerela* querelae;   /* tabulatum in piscina data */
    i32           numerus;
} GestaSalus;

nomen structura {
    chorda     res_id;
    GestaSalus salus;
} GestaInsalubris;

/* FALSUM solum si res ignota / apparatus fractus; genus rei
 * ignotum = sanum (nihil iudicandum) */
b32
gesta_salutem_aestimare (
    GestaMundus*        mundus,
    constans character* res_id,
    Piscina*            piscina,
    GestaSalus*         exitus);

/* Res insalubres solae redduntur (Xar de GestaInsalubris);
 * genus NIHIL = omnia. Percursus plenus - satis parvis
 * copiis (via sordidarum parcata). */
Xar*
gesta_insalubres_enumerare (
    GestaMundus*        mundus,
    constans character* genus,
    Piscina*            piscina);

/* Lectiones (pro probationibus et stratis superioribus; textus in
 * piscinam datam copiatur; chorda vacua = absens) */
chorda
gesta_res_datum (
    GestaMundus*        mundus,
    constans character* res_id,
    Piscina*            piscina);

chorda
gesta_res_status (
    GestaMundus*        mundus,
    constans character* res_id,
    Piscina*            piscina);

chorda
gesta_genus_datum (
    GestaMundus*        mundus,
    constans character* titulus,
    Piscina*            piscina);

s64
gesta_seq_ultima (
    GestaMundus* mundus);

s64
gesta_hwm (
    GestaMundus*        mundus,
    constans character* consumptor);   /* "genera" | "res" |
                                        * "nexus" | "membra" */

/* Scrinium subiacens (quaestiones liberae probationum - lectio;
 * scriptura per gesta_scribere SOLUM) */
Scrinium*
gesta_scrinium (
    GestaMundus* mundus);

constans character*
gesta_error (
    constans GestaMundus* mundus);

#endif /* GESTA_H */
