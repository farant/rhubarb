/* communio_servus.h - servus actorum communionis (project-specs/
 * communio-spec.md II.4, II.5, II.2; plan communio-plan-2 T3-T5)
 *
 * Servus est volumen aliud (C1): acta apparatuum importat (identitate
 * et numeratione scriptoris servatis) et aliis apparatibus eiusdem
 * spatii ministrat. Registrum apparatuum in eodem volumine:
 *
 *   communio_membra    (spatium, civis) - quis in spatio sit
 *   communio_apparatus id (ULID), civis, spatium, titulus, digestum
 *                      (SHA-256 hex tesserae - tessera ipsa NUMQUAM),
 *                      replica (ligata in contactu primo, C4),
 *                      creatus_ms, usus_ultimus_ms, revocatus
 *
 * TESSERA (C2): apparatus ipse eam cudit (communio_tesseram_facere:
 * XXXII octeti fortuiti, base64url, XLIII signa) et solum digestum
 * ostendit; administrator digestum inscribit. Phase 3 (nexus per
 * epistulam) idem digestum per epistulam inscribet.
 *
 * Acta registri (sine secretis): communio.membrum-additum {spatium,
 * civis}, communio.apparatus-additus {id, civis, spatium, titulus},
 * communio.apparatus-revocatus {id}, communio.replica-ligata {id,
 * replica}. Scripturae transactionem vocatoris iungunt, aliter
 * propriam aperiunt.
 */

#ifndef COMMUNIO_SERVUS_H
#define COMMUNIO_SERVUS_H

/* <aedilis corpus="lib/communio_servus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "cives.h"
#include "hospitium.h"

#define COMMUNIO_TESSERA_MENSURA XLIII

nomen structura CommunioServus CommunioServus;

/* chordae in piscina vocantis */
nomen structura {
    chorda id;
    chorda civis;
    chorda spatium;
    chorda titulus;
    chorda replica;      /* vacua donec ligata */
       b32 revocatus;
} CommunioApparatus;

/* tabulas creat si desunt; nunc_ms NIHIL = horologium verum. NIHIL si
 * argumenta mala aut schema deficit. */
CommunioServus*
communio_servum_creare (
    Piscina* piscina,
    Volumen* volumen,
      Cives* cives,
         s64 (*nunc_ms)(vacuum));

/* arenas servi reddit et horas exspectantium (T5) cancellat; volumen
 * et cives vocatoris manent. ANTE hospitium_destruere vocandum. NIHIL
 * = nihil agit. */
vacuum
communio_servum_destruere (
    CommunioServus* servus);

/* octeti in piscina scripturarum servi (recensio 1: stabilis esse
 * debet per petitiones - mensura et vigilia) */
memoriae_index
communio_servi_usus (
    constans CommunioServus* servus);

/* civem (activum) in spatium; iteratum = VERUM sine actu novo */
b32
communio_membrum_addere (
    CommunioServus* servus,
            chorda  spatium,
            chorda  civis);

/* apparatum per digestum tesserae (LXIV hex minuscula) inscribere.
 * FALSUM si persona non activa, non membrum spatii, digestum malum aut
 * iam usum. id_exitus >= MONETA_ULID_MENSURA (XXVI signa + NUL). */
b32
communio_apparatum_addere (
    CommunioServus* servus,
            chorda  civis,
            chorda  spatium,
            chorda  titulus,
            chorda  digestum,
         character* id_exitus);

/* revocatus numquam iterum verificatur; acta eius manent (C3).
 * FALSUM si id ignotum. */
b32
communio_apparatum_revocare (
    CommunioServus* servus,
            chorda  id);

/* omnes, ordine inscriptionis: Xar de CommunioApparatus (digestum
 * numquam redditur) */
Xar*
communio_apparatus_enumerare (
    CommunioServus* servus,
           Piscina* piscina);

/* tessera (XLIII signa exacte) -> apparatus: digestum quaeritur,
 * comparatio temporis constantis, non revocatus, persona CIVIS_ACTIVUS;
 * usus_ultimus_ms renovatur */
b32
communio_apparatum_verificare (
       CommunioServus* servus,
               chorda  tessera,
    CommunioApparatus* exitus,
              Piscina* piscina);

/* replicam apparatui ligare (C4): 0 = nunc ligata aut iam haec;
 * 1 = apparatus alii replicae ligatus; 2 = replica alii apparatui;
 * -1 = id ignotum aut replica invalida */
s32
communio_replicam_ligare (
    CommunioServus* servus,
            chorda  id,
            chorda  replica);

/* pro apparatu (instrumentum, probationes): tesseram novam cudere -
 * exitus XLIII signa + NUL, digestum LXIV hex + NUL. FALSUM si fons
 * fortuitus deficit. */
b32
communio_tesseram_facere (
    character exitus[XLIV],
    character digestum[LXV]);


/* ==================================================
 * Protocollum (T4) - omnia sub /communio/, omnia cum
 * "Authorization: Bearer <tessera>" (401 aliter)
 * ==================================================
 *
 * POST /communio/apparatus {"replica":R} -> 200 {"apparatus",
 *      "spatium", "servus"};
 *      409 replica alia aut aliena; 400 invalida
 * GET  /communio/apparatus -> {"apparatus":[{replica, titulus, civis,
 *      persona}]} spatii vocantis (ligati soli; digesta numquam)
 * POST /communio/massae/desunt {"massae":[hex]} -> {"desunt":[hex]}
 * PUT  /communio/massae/:sigillum (octeti; > 1 MB per filum) -> 201;
 *      413 supra COMMUNIO_MASSA_MAXIMA (XVI MB, limes lectoris http);
 *      400 si SHA-256 octetorum non :sigillum (nihil servatur)
 * GET  /communio/massae/:sigillum -> 200 octeti (per fontem); 404
 * POST /communio/acta {"acta":[...]} (CC ad plurimum, omnia valida,
 *      omnia replicae ligatae vocantis - aliter 400 / 403, nihil
 *      importatum) -> ordine importat, ad refusum primum sistit:
 *      {"accepta":n,"refusum":null | {"index":i,"causa":"hora-futura|
 *      conflictus|massa-deest|invalidum"}} (iam praesentia accepta)
 * GET  /communio/acta?post=N -> {"acta":[...],"cursor":M,"plura":b,
 *      "massae":[hex]}: acta (seq servi > N) apparatuum ALIORUM spatii,
 *      CC ad plurimum; cursor = seq servi ultimi (N si nulla); massae =
 *      quas acta nominant et servus habet; "servus" = replica voluminis
 *      servi (recensio 3: cursores clientis ad eam ligati).
 *      &mora=S (T5, XXV s ad
 *      plurimum): si nihil est, petitio exspectat donec apparatus alius
 *      spatii acta nova mittit (responsum idem ac tractio recens) aut S
 *      secundae transeunt (acta vacua, cursor N); loci LXIV, pleni =
 *      responsum statim */

b32
communio_servum_montare (
    CommunioServus* servus,
         Hospitium* hospitium);

#endif /* COMMUNIO_SERVUS_H */
