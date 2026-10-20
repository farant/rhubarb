/* ianua.h - ianua rationum in hospitio (ianua T4)
 *
 * Componit cives, credentialia, invitationes et sessiones; montat
 * paginas (praefixum P, "" = radix):
 *
 *   GET,POST P/intrare       ?redire= (via LOCALIS solum) servatur
 *   GET,POST P/inscribere    ?invitatio= -> forma; POST: hash, deinde
 *                            transactio UNA (invitatio consumpta,
 *                            civis creatus, tessera posita) - ulla
 *                            refutatio = revolutio; deinde
 *                            post_inscriptionem, 303 P/intrare
 *   POST     P/exire
 *   GET,POST P/tessera       actualis + nova: verificatio actualis et
 *                            hash novae SIMUL (tessera nova numquam
 *                            in loculo ianuae); bona -> transactio:
 *                            tessera posita + sessiones ALIAE necatae
 *   GET,POST P/invitationes  invitationes personae; POST creat et
 *                            codicem + nexum SEMEL ostendit (no-store)
 *   POST     P/invitationes/:id/revocare
 *
 * Custodes: omnis POST Origin congruens; post sessionem etiam CSRF.
 * Sessio: signum crustuli "sid" (custodia), servatur SOLUM sigillum
 * (SHA-256 hex) in tabula ianua_sessiones; civis non activus = non
 * intratus. Nomen usoris minusculis normatur (ianua P13). Textus omnis
 * HTML evaditur.
 *
 * pagina (optiones): applicatio corpus (iam evasum) in fabricam suam
 * involvit et respondet, capita tuta ipsa ponens; NIHIL = pagina
 * simplex ianuae cum capitibus tutis.
 *
 * Acta: sessio.creata {sigillum, civis, expirat}, sessio.deleta
 * {sigillum} - numquam signum, tessera, codex.
 */
#ifndef IANUA_H
#define IANUA_H

/* <aedilis corpus="lib/ianua.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "hospitium.h"
#include "volumen.h"
#include "custodia.h"
#include "cives.h"
#include "credentialia.h"
#include "invitationes.h"

#define IANUA_SESSIO_VITA_S  2592000      /* XXX dies */
#define IANUA_OPERA          LXIV
#define IANUA_TITULUS_MAXIMUS CXXVIII
#define IANUA_REDIRE_MAXIMUM  MMXLVIII

nomen structura {
     constans character* praefixum;              /* NIHIL = "" */
                    s64  sessio_vita_s;          /* 0 = ordinaria */
     constans character* via_post_intrationem;   /* NIHIL = "/" */
                    b32  invitatio_necessaria;
                    s64  vita_invitationis_ms;   /* 0 = ordinaria */
    vacuum (*pagina)(vacuum* datum, HospitiumColloquium* col,
        i32 status, constans character* titulus, chorda corpus);
    vacuum (*post_inscriptionem)(vacuum* datum, chorda civis);
    /* NIHIL = quivis civis activus */
    b32 (*potest_invitare)(vacuum* datum, constans Civis* civis);
                vacuum* datum;
                    s64 (*nunc_ms)(vacuum);
} IanuaOptiones;

nomen structura Ianua Ianua;

/* cives, credentialia, invitationes, tabulam sessionum aperit;
 * optiones copiantur */
Ianua*
ianua_creare (
                   Volumen* volumen,
                  Custodia* custodia,
                   Reactor* reactor,
    constans IanuaOptiones* optiones,
                   Piscina* piscina);

b32
ianua_montare (
        Ianua* ianua,
    Hospitium* hospitium);

Cives*
ianua_cives (
    Ianua* ianua);

Credentialia*
ianua_credentialia (
    Ianua* ianua);

Invitationes*
ianua_invitationes (
    Ianua* ianua);

/* sessio valida et civis ACTIVUS: civis (piscina colloquii) + signum
 * CSRF; csrf NIHIL licet */
b32
ianua_intratus (
                  Ianua* ianua,
    HospitiumColloquium* col,
                  Civis* civis,
                 chorda* csrf);

/* POST tutum: Origin congruens + intratus + CSRF formae ("csrf").
 * FALSUM = refutatum (vocator 403 respondet) */
b32
ianua_custos_postis (
                  Ianua* ianua,
    HospitiumColloquium* col,
                  Civis* civis);

/* AdmissioIntratus: datum = Ianua*; usor = civis ULID */
b32
ianua_admissionis_intratus (
                 vacuum* ianua,
    HospitiumColloquium* col,
                 chorda* usor,
                 chorda* csrf);

#endif /* IANUA_H */
