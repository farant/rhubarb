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
                                          * emendatio-generis|
                                          * actio-facta|
                                          * actio-recusata|...
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

/* ==================================================
 * Fascis atomicus (K3 chunk A) - eventus N, transactio una
 * ==================================================
 * Omnes eventus praeparantur et validantur ANTE scripturam; error
 * mechanicus (datum malformatum, creatio duplicata, res_id absens)
 * totum fascem recusat - NIHIL scribitur, nullae lineae annalium.
 * Violationes machinae MORE DOMUS: eventus cadit + nota custodiae
 * INTRA fascem (transactione eadem, sequenter adiacens). Lineae
 * annalium N ordine seq ANTE COMMIT (ruina => annales superset,
 * lex K1 servata). Creatum UNUM per fascem (strftime semel lectum
 * - $nunc actionum stabile). Validatio res in eodem fasce creatas
 * per obumbras videt (exsistentia + genus; status = initialis
 * generis). Plicatura semel post scripturam. */

nomen structura {
    constans character* event_id;   /* NIHIL = ULID cuditur */
    GestaEventum        eventum;
} GestaFascisEventum;

/* res_ids_out: NIHIL aut tabulatum numerus * GESTA_RES_ID_MENSURA
 * octetorum - res_id effectivum cuiusque eventus DATI (notae
 * custodiae interpositae locum non occupant). */
b32
gesta_fascis_scribere (
    GestaMundus*                 mundus,
    constans GestaFascisEventum* eventa,
    i32                          numerus,
    character*                   res_ids_out);

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

/* ==================================================
 * Actiones (K3 chunk A) - recepta ut data
 * ==================================================
 * Definitio = genus cum specie "actio" (definitio-generis /
 * emendatio-generis solita - emendatio in loco, historia in
 * annalibus): opes (ligamina rerum: titulus/genus/
 * status_necessarius), argumenta (titulus/typus K2/necessarium),
 * effectus (verba clausa 1:1 in eventus: creatio [cum 'ut'],
 * mutatio, remotio, status, nota, membrum-additum,
 * membrum-remotum). Signa substitutionis in effectibus:
 *   $arg.X   (signum solum = typus JSON servatur; intextum =
 *             coercitur in chordam - paritas TS :3376-3391)
 *   $res.X   (res_id ligaminis X)
 *   $novus.N (res creata 'ut':N effectu PRIORE - referentia
 *             antrorsum = error mechanicus)
 *   $nunc    (creatum fascis - timestamp unum stabile)
 * PORTA OBSTAT (recusatio = receptum negans, non machina mentiens);
 * effectus MORE DOMUS scribunt (violationes = notae custodiae
 * intra fascem). Executio = eventus 'actio-facta' {ligamina,
 * argumenta, eventus:[ids]} in flumine rei actionis (res_id =
 * titulus actionis - flumen ipsum tabularium executionum est);
 * recusationes (porta ET mechanicae) = 'actio-recusata'
 * {ligamina, argumenta, causa} ibidem. Ordo rei pro actione NON
 * materializatur (eventus ignoti reductori = nihil agunt). */

nomen structura {
    b32                 facta;         /* FALSUM = recusata */
    constans character* causa;         /* si recusata */
    b32                 mechanica;     /* recusata: FALSUM = porta
                                        * (retenta - K3 decisio 16),
                                        * VERUM = error mechanicus */
    constans character* facta_id;      /* id eventus actio-facta
                                        * (prae-cusum; "" si
                                        * recusata) */
    chorda*             res_novae;     /* res creatae, ordine
                                        * effectuum */
    i32                 novae_numerus;
} GestaActioFructus;

/* FALSUM solum apparatu fracto aut actione ignota / non-actione;
 * recusatio = VERUM cum exitus->facta FALSUM et causa posita. */
b32
gesta_agere (
    GestaMundus*        mundus,
    constans character* actio_titulus,
    constans character* ligamina_json,
    constans character* argumenta_json,
    Piscina*            piscina,
    GestaActioFructus*  exitus);

/* Affordantiae: tituli actionum quarum ops aliquis rem datam NUNC
 * ligare potest (genus et status_necessarius congruunt). Xar de
 * chorda; percursus generum plenus (copiae parvae - E2 par 5). */
Xar*
gesta_actiones_rei (
    GestaMundus*        mundus,
    constans character* res_id,
    Piscina*            piscina);

/* ==================================================
 * Processus (K3 chunk B) - orchestratio ut data
 * ==================================================
 * Definitio = genus cum specie "processus": opes/argumenta (porta
 * eadem ac actionum), ordines (nomina cursuum sancita), gradus
 * [{titulus, ordo, genus_gradus: opus|actio|porta, positio,
 * condiciones? (portae: tituli graduum, omnes-ex), actio?/
 * ligamina?/argumenta? (actio-gradus; signa $ops.X/$arg.X duabus
 * stratis - processus solvit, actio deinde sua), opus_titulus?/
 * descriptio?/prioritas? (opus-gradus)}]. Intra cursum stricte
 * sequentia; cursus concurrentes; iunctio trans cursus PER PORTAS
 * SOLAS (decisio 15).
 *
 * Instantia = res ordinaria cuius genus = titulus processūs;
 * creatio definitionem PHOTOGRAPHAT (gradus/ordines/opes soluta/
 * argumenta - D9: emendatio generis instantias currentes non
 * tangit). Machina: currens -> perfectus | defectus | omissus.
 * Opera generata + ligamina opum per membra nectuntur (instantia
 * --pars--> res; ligamina opum pars "ops:<titulus>" - excitatio
 * per indicem membrorum, decisio 10/G16).
 *
 * Provectio AVIDA anguste-icta: eventus status in re quae membrum
 * instantiae currentis est -> ansa exhauriendi ad fixum (fascis
 * micro + plicatura per iterationem; sine recursione - scrinium
 * BEGIN nidificatum non fert). Actio-gradus recusata = gradus
 * pendens manet, iterum temptatur excitatione proxima (decisio 16
 * - se-sanans); error mechanicus = gradus-defectus. Eventus
 * instantiae: gradus-incohatus/-perfectus/-defectus,
 * porta-perfecta (semel - portae obstructae NIHIL emittunt, D10),
 * opus-genitum, actio-acta, processus-perfectus/-defectus (+
 * eventus status ordinarius). Motor in strato scripturae SOLO
 * vivit - replicatio eventus ut data replicat (decisio 10). */

nomen structura {
    b32                 facta;        /* FALSUM = recusata */
    constans character* causa;
    character           instantia[GESTA_RES_ID_MENSURA];
} GestaProcessusFructus;

/* Instantiam incipere: porta (opes + argumenta) -> photographia ->
 * creatio -> provectio prima (gradus primi cuiusque cursūs
 * generantur). FALSUM solum apparatu fracto / processu ignoto. */
b32
gesta_processum_incipere (
    GestaMundus*        mundus,
    constans character* processus_titulus,
    constans character* ligamina_json,
    constans character* argumenta_json,
    Piscina*            piscina,
    GestaProcessusFructus* exitus);

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
                                        * "membra" */

/* Scrinium subiacens (quaestiones liberae probationum - lectio;
 * scriptura per gesta_scribere SOLUM) */
Scrinium*
gesta_scrinium (
    GestaMundus* mundus);

constans character*
gesta_error (
    constans GestaMundus* mundus);

#endif /* GESTA_H */
