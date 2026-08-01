/* internuntius.h - RPC purum trans suturam missoris (vitrea Phasis A)
 *
 * Involucrum JSON + registrum methodorum + dispatch + pulsus
 * eventuum C->JS. De WebKit NIHIL scit: omnis effusio per callback
 * missoris unum fluit (sutura transportūs - vitreae missor est
 * "effugere + evaluateJavaScript", probationis est "appendere ad
 * indicem captum"). Idem involucrum postea super tcp aut stdio
 * currere posset. Spec: project-specs/vitrea-spec-v2.md par VI.
 *
 * INVOLUCRUM (forma filaris - json, non nuntium: alter finis
 * navigatrum est, JSON ibi nativum et in Inspectore legibile):
 *
 *   JS -> C   { "id": 7, "methodus": "salve", "argumenta": {...} }
 *   C -> JS   { "id": 7, "fructus": {...} }              (successus)
 *             { "id": 7, "culpa": { "nuntius": "..." } } (defectus)
 *   C -> JS   { "eventus": "horologium", "datum": {...} }  (pulsus)
 *
 * Responsa per id itinerantur: involucrum responsa tarda et extra
 * ordinem PERMITTIT - tractatores synchroni v1 sunt consilium
 * implementationis, non promissum API. Vocatio sine id responsu
 * caret itinere sed responsum tamen mittitur (JS ignota ignorat).
 *
 * MEMORIA: bibliotheca pura arenas crescentes latentes NON possidet.
 * internuntius_creare piscinam diuturnam accipit (structura +
 * registrum); tractare/eventum_mittere piscinam VOCATIONIS
 * explicitam accipiunt - arbor parsata, fructus tractatoris,
 * responsum serializatum omnia ibi vivunt, vocans notat/reficit.
 * CONTRACTUS MISSORIS: textus validus SOLUM intra vocationem
 * missoris - copia si retinendum (aestimatio WebKit statim copiat;
 * missor memoriae probationis copiare debet).
 *
 * Culpae numquam fragunt: JSON malformatum -> culpa sine id;
 * methodus ignota / culpa tractatoris -> culpa cum id; numerator
 * culpae crescit. */

#ifndef INTERNUNTIUS_H
#define INTERNUNTIUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"

nomen structura Internuntius Internuntius;

/* numeratores fructus (mensura est productum - decisiones perf ex
 * his numeris fiunt) */
nomen structura {
    i64 vocationes_tractatae;   /* dispatches felices */
    i64 culpae;                 /* responsa culpae missa */
    i64 eventus_missi;          /* pulsus C->JS */
} InternuntiusFructus;

/* tractator methodi: argumenta = nodus 'argumenta' parsatus (NIHIL
 * si absens - tractator id ferre debet); piscina = piscina
 * vocationis (fructum IBI aedifica); datum = datum usoris
 * praebitionis. Redde valorem fructus, aut NIHIL: si *culpa_exitus
 * positum (mensura > 0) responsum culpae fit, alioquin fructus
 * null (JSON) - vocatio felix sine valore. */
nomen JsonValor* (*InternuntiusTractator)(
    JsonValor* argumenta,
    Piscina*   piscina,
    vacuum*    datum,
    chorda*    culpa_exitus);

/* missor: effusio una omnium responsorum et eventuum */
nomen vacuum (*InternuntiusMissor)(
    vacuum* datum,
    chorda  textus);

/* modus transportus - applicatio decernit quas methodos praebeat.
 * LOCALIS = fenestra in machina propria (fides plena: harnesium
 * probationis, modus-debug, quidquid); PUBLICUS = socket, ubi
 * methodi harnesii EXCLUDENDAE sunt.
 *
 * Parametrum est, non campus configurationis, de industria: si
 * praebitor modum in signatura videt, oblivisci NON potest. Porta
 * quae in oculis sedet melior est quam porta quam meminisse
 * oportet. */
nomen enumeratio {
    INTERNUNTIUS_MODUS_LOCALIS = 0,
    INTERNUNTIUS_MODUS_PUBLICUS
} InternuntiusModus;

/* praebitor: registratio methodorum applicationis - SEMEL scripta,
 * a quocumque transportu vocata (fenestra vitreae aut servus HTTP).
 * Sedes una registrationis per FORMAM API, non per disciplinam:
 * methodus fenestrae addita in telephono statim adest, quia locus
 * alter ubi obliviscaris non est. */
nomen vacuum (*InternuntiusPraebitor)(
    Internuntius*     inx,
    InternuntiusModus modus,
    vacuum*           datum);

Internuntius*
internuntius_creare (
    Piscina*           piscina,
    InternuntiusMissor mittere,
    vacuum*            mittere_datum);

/* methodum praebere: titulus copiatur in piscinam diuturnam.
 * Duplicatum RECUSATUR (FALSUM) - registrum codex est, semel
 * definitum; recusatio errores compaginis capit. */
b32
internuntius_praebere (
    Internuntius*         inx,
    constans character*   methodus,
    InternuntiusTractator tractator,
    vacuum*               datum);

/* nuntium unum tractare: parsare -> invenire -> vocare ->
 * respondere per missorem. Numquam fragit (vide supra). */
vacuum
internuntius_tractare (
    Internuntius* inx,
    chorda        nuntium,
    Piscina*      piscina_vocationis);

/* eventum pellere: { "eventus": titulus, "datum": datum|null } */
vacuum
internuntius_eventum_mittere (
    Internuntius*       inx,
    constans character* eventus,
    JsonValor*          datum,
    Piscina*            piscina_vocationis);

/* introspectio registri: Xar de chorda (valore), ordine
 * praebitionis - semen generationis ligaminum (dilatio nominata) */
Xar*
internuntius_methodi (
    Internuntius* inx,
    Piscina*      piscina);

InternuntiusFructus
internuntius_fructus (
    constans Internuntius* inx);

/* textum in corpus litterae chordae JS effugere (vocans ipse
 * circumdat citationibus). Effugit: '\\' '"' moderatores (<0x20:
 * \n \r \t breviter, ceteri \uXXXX), '<' (contra '</script'), et
 * U+2028/U+2029 (tripla E2 80 A8/A9 - in JSON licita, in littera
 * JS cruda illicita). Octeti multibyte ceteri transeunt intacti.
 * Functio pura; probationes aureae. */
chorda
internuntius_effugere_js (
    chorda   textus,
    Piscina* piscina);

#endif /* INTERNUNTIUS_H */
