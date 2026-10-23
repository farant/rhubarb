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
#include "scrinium.h"

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
               Piscina* piscina,
    constans character* via);

/* aperire: plagula exsistat */
Volumen*
volumen_aperire (
               Piscina* piscina,
    constans character* via);

/* aperire si exsistit, alioquin creare - pro apps quae volumen
 * status "semper habere" volunt (mensa, ambulatio prima).
 * (Natum ex ambulatione red-team 2026-08-04: tutorial hoc
 * pollicitum erat quod nulla functio praestabat.) */
Volumen*
volumen_aperire_aut_creare (
               Piscina* piscina,
    constans character* via);

/* optiones (memset 0 = mos domus) */
nomen structura {
    /* synchronous=FULL: "servatum" superstat defectum potentiae
     * (applicationes cum usoribus; tardius ad commissionem) */
    b32 durabilitas_plena;
} VolumenOptiones;

/* aperire_aut_creare cum optionibus; NIHIL = sine */
Volumen*
volumen_aperire_aut_creare_cum (
                     Piscina* piscina,
          constans character* via,
    constans VolumenOptiones* optiones);

/* Scrinium subiectum: pragmata, copia (VACUUM INTO), quaestiones
 * legendi propriae. Transactiones voluminis NE praeterieris. */
Scrinium*
volumen_scrinium (
    Volumen* volumen);

/* VOLUMEN SCRATCH - status qui probationem non superstat.
 *
 * Plagula unica sub /tmp, DELETA cum clauditur. Pro probationibus
 * integrantibus, quae aliter in volumen VERUM usoris scribunt.
 *
 * CUR HOC EXSTAT (mensuratum 2026-08-13): probatio fumi laboratorii
 * ~/.rhubarb/laboratorium.volumen adhibebat, quia applicatio viam
 * FIXAM tenebat. Post diem unum acta XLII 'salutatio' et XXI
 * 'experimentum' in volumine VERO Frannis sedebant - res quas nemo
 * scripsit et nemo vult.
 *
 * Peius quam sordes: probatio talis REPETIBILIS non est. Numerus
 * visitationum omni cursu crescit, ergo assertum quodlibet de eo
 * hodie transit et cras cadit. Status inter cursus manens fontem
 * fragilitatis facit quem nulla mora sanat.
 *
 * 'praefixum' plagulam nominat ut, si quid pendeat, cuius sit
 * appareat (e.g. "fumus_manus" -> /tmp/fumus_manus-1.volumen).
 *
 * DELETIO IN CLAUDENDO: qui non claudit plagulam relinquit - quod
 * ipsum indicium est (probatio quae claudere omisit id in /tmp
 * ostendit). */
Volumen*
volumen_temporarium (
               Piscina* piscina,
    constans character* praefixum);

/* Volumen temporarium plagulam suam hic delet. */
vacuum
volumen_claudere (
    Volumen* volumen);

/* Via plagulae (litterae - vita voluminis). Utilis in actis et
 * probationibus: 'quod volumen re vera adhibetur?' */
constans character*
volumen_via (
    constans Volumen* volumen);

/* nuntius erroris ultimi (litterae - vita voluminis); NIHIL si nullus */
constans character*
volumen_error (
    constans Volumen* volumen);

/* identitas actus trans machinas (D2): hora ZEPHYRUM = nulla */
nomen structura {
       s64 hora;
    chorda machina;
} VolumenIdentitas;

/* actum in caudam appendere (datum = JSON); seq redditum, 0 = vitium */
s64
volumen_actum_appendere (
               Volumen* volumen,
    constans character* genus,
                chorda  datum);

/* ut volumen_actum_appendere, sed actum BASIM suam nominat: statum
 * contra quem factum est (genera-datorum VIII-c, D2b). Acta duo
 * eiusdem basis = furca (revocatione locali aut machinis duabus). */
s64
volumen_actum_super_appendere (
                 Volumen* volumen,
      constans character* genus,
                  chorda  datum,
        VolumenIdentitas  basis);

/* identitas actus per seq; FALSUM si seq ignotum */
b32
volumen_actum_identitas (
             Volumen* volumen,
                 s64  seq,
             Piscina* piscina,
    VolumenIdentitas* exitus);

/* plagulam condere: transactione una - massa (sigillo dedup) +
 * actum 'plagula-condita' + manifestum (upsert per viam).
 * origo e.g. "vendicata:lib/chorda.c" | "genita" */
b32
volumen_plagulam_condere (
               Volumen* volumen,
                chorda  via_relativa,
                chorda  contentum,
    constans character* origo);

/* plagulam e manifesto removere: actum 'plagula-remota' + DELETE.
 * Massa manet (contentum sigillo addressatum - historia eam adhuc
 * spectat); VERUM etiam si via iam aberat (idempotens). */
b32
volumen_plagulam_removere (
    Volumen* volumen,
     chorda  via_relativa);

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

/* vocatorne transactionem tenet? (bibliothecae quae transactionem
 * propriam aliter aperirent eam iungunt - admissio_revocare, canalis
 * T4). NIHIL -> FALSUM */
b32
volumen_in_transactione (
    Volumen* volumen);

/* contentum plagulae ex manifesto + massis; *inventum FALSUM si via
 * ignota (chorda vacua legitima est - plagula vacua) */
chorda
volumen_plagulam_promere (
    Volumen* volumen,
     chorda  via_relativa,
    Piscina* piscina,
        b32* inventum);

/* PRAESENTIA (genera-datorum D1): status applicationis - clavis ->
 * valor, scriptor ultimus vincit, EXTRA acta: nec diarium nec massas
 * auget (paginae-proba 2026-10-20: 58 % actorum erant instantanea
 * status/, 11 % index vici). Veritas sine historia, non proiectio.
 * Clientes: conservatio ("status/<clavis>"), vicus ("vicus/latera").
 * Migratio: plagulae eorum semel copiatae (migratio VI). */
b32
volumen_praesens_ponere (
    Volumen* volumen,
     chorda  clavis,
     chorda  valor);

/* valor clavis; *inventum FALSUM si nulla (valor vacuus legitimus) */
chorda
volumen_praesens_legere (
    Volumen* volumen,
     chorda  clavis,
    Piscina* piscina,
        b32* inventum);

/* massam nudam condere (contentum sigillo addressatum, dedup) -
 * massa CONTENTUM est, non eventus: veritas de USU eius in actis
 * vivit (e.g. mensa:creatum cum clave imago). sigillum_hex_exitus
 * >= SIGILLUM_HEX_MENSURA octeti. */
b32
volumen_massam_condere (
      Volumen* volumen,
       chorda  contentum,
    character* sigillum_hex_exitus);

/* MASSA EX FILO PER RIVUM: sigillum_notum (64 hex minusculae),
 * in itinere computatum (hospitium). Si massa iam est: VERUM, filum
 * NON legitur (dedup). Aliter, in savepoint (intra transactionem
 * vocatoris quoque tutum): zeroblob(mensura), filum per partes LXIV
 * KB in massam, SHA-256 iterum computatum et comparatum - discrepantia
 * aut mensura mendax = FALSUM, nihil manet (volumen_error causam
 * nominat). */
b32
volumen_massam_condere_ex_filo (
               Volumen* volumen,
    constans character* via_fili,
                   i64  mensura,
    constans character* sigillum_notum);

/* MASSA PER PARTES LEGERE (fons secundus hospitii): NIHIL si sigillum
 * ignotum; *mensura = longitudo. Ansa sqlite per partem aperitur et
 * clauditur: download longus nullum snapshot WAL tenet (checkpoint
 * interim perficitur, WAL non crescit). */
nomen structura VolumenMassaLector VolumenMassaLector;

VolumenMassaLector*
volumen_massam_aperire (
    Volumen* volumen,
     chorda  sigillum_hex,
        i64* mensura,
    Piscina* piscina);

/* >0 octeti, 0 finis, -1 error */
s32
volumen_massam_legere (
    VolumenMassaLector* lector,
                    i8* capsa,
                   i32  n);

vacuum
volumen_massam_claudere (
    VolumenMassaLector* lector);

/* massam promere sigillo (hex); *inventum FALSUM si ignotum */
chorda
volumen_massam_promere (
    Volumen* volumen,
     chorda  sigillum_hex,
    Piscina* piscina,
        b32* inventum);

/* manifestum totum, ordine viae; Xar de VolumenPlagula */
Xar*
volumen_plagulas_enumerare (
    Volumen* volumen,
    Piscina* piscina);

/* actum lectum (chordae ex piscina vocantis). Identitas actus =
 * (hora, machina) - D2: hora hybrida (hora_hybrida.h) = ordo EVENTUS,
 * seq = ordo ADVENTUS in hoc volumine; machina = machina_identitas()
 * scriptoris. Acta vetera (ante migrationem VII, aut a codice vetere
 * scripta) in aperiendo supplentur: ordine seq, momentum suum per
 * hora_hybrida_proxima, machina huius machinae. Sine identitate
 * machinae acta non appenduntur (recusat, non degenerat).
 * basis (D2b): actum contra quem factum est - hora ZEPHYRUM = sine
 * basi (acta non documentorum, actum primum, acta ante D2b). */
nomen structura {
                 s64 seq;
              chorda momentum;
              chorda genus;
              chorda datum;
    VolumenIdentitas identitas;
    VolumenIdentitas basis;
} VolumenActum;

/* acta POST seq datum (exclusivum; 0 = omnia), ordine seq -
 * forma "acta post seq N" quam et plicae et replicatio volunt */
Xar*
volumen_acta_legere (
    Volumen* volumen,
        s64  post_seq,
    Piscina* piscina);

/* acta generum datorum post seq, ordine seq (genera: tabula
 * NIHIL-terminata; per indicem acta(genus, seq)) - historia documenti
 * acta sua sola legit, non volumen totum */
Xar*
volumen_acta_generum_legere (
                         Volumen* volumen,
                             s64  post_seq,
    constans character* constans* genera,
                        Piscina* piscina);

/* plica ad seq: manifestum RECONSTRUCTUM replicando acta plagularum
 * (plagula-condita ponit, plagula-remota tollit) cum seq <= ad_seq;
 * ad_seq <= 0 = omnia (plica praesens - enumerationi aequalis, quod
 * probatio pinnat). Xar de VolumenPlagula, ordine viae. Primitivum
 * itineris temporis (proicere -ad) et replicationis (G8). */
Xar*
volumen_plicam_ad (
    Volumen* volumen,
        s64  ad_seq,
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
