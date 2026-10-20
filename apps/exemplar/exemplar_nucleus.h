/* exemplar_nucleus.h - applicatio probans hospitii v3: rationes per
 * ianuam (cives, invitationes per personam, sessiones), notae cum
 * filis, salus
 *
 * Status totus in volumine UNO (data/exemplar.volumen, durabilitas
 * plena): tabulae ianuae (cives, credentialia, invitationes,
 * ianua_sessiones) et admissionis; acta notarum (nota.scripta cum
 * civi, nota.deleta, filum.receptum) ad initium in memoria replicantur;
 * massae = fila. Volumen ante ianuam (usor.creatus, tessera.mutata cum
 * phc) SEMEL migratur (ianua T5). Secretum servi data/secretum (0600,
 * extra volumen), fila temporaria data/tmp (0700), sera scriptoris
 * unius data/sera (ianua P1).
 *
 * Viae ianuae: GET,POST /intrare | GET,POST /inscribere | POST /exire
 * | GET,POST /tessera | GET,POST /invitationes. Exemplaris: GET / |
 * GET,POST /notae | POST /notae/:id/delere | GET /fila/:sigillum |
 * GET /salus. Omnis POST: Origin congruens; post sessionem etiam CSRF.
 * Textus usoris omnis per unum auxilium HTML evaditur. Fila: attachment
 * + nosniff, dominus solus (alienus = 404). */
#ifndef EXEMPLAR_NUCLEUS_H
#define EXEMPLAR_NUCLEUS_H

#include "latina.h"
#include "piscina.h"
#include "reactor.h"
#include "hospitium.h"
#include "chorda.h"

nomen structura {
    constans character* via_datorum;      /* --data (creatur 0700) */
    constans character* hospes;           /* NIHIL = 127.0.0.1 */
                   i32  portus;           /* 0 = auto */
                   b32  procurator_fidus; /* VERUM post nginx */
                   b32  acta_accessus;
                   i32  frenum_conatus;   /* 0 = V */
               Reactor* reactor;          /* NIHIL = proprius */
    /* basis publica: "https://exemplar.golden.earth"; NIHIL =
     * http://<hospes>:<portus> (mcp-remotus P10) */
    constans character* basis;
    /* sonda: acta petitionum /mcp, /.well-known/..., /oauth/... (capita
     * redacta); scriptor NIHIL = stderr */
                   b32 sonda;
                vacuum (*sonda_scriptor)(vacuum* datum, chorda linea);
                vacuum* sonda_datum;
} ExemplarConfiguratio;

nomen structura Exemplar Exemplar;

/* NIHIL si data, sera (alius processus currit), volumen, secretum,
 * migratio aut servus deficiunt (causa ad stderr) */
Exemplar*
exemplar_creare (
    constans ExemplarConfiguratio* configuratio,
                          Piscina* piscina);

Hospitium*
exemplar_hospitium (
    Exemplar* exemplar);

Reactor*
exemplar_reactor (
    Exemplar* exemplar);

/* servus destruitur, volumen (checkpoint) clauditur, sera liberatur */
vacuum
exemplar_claudere (
    Exemplar* exemplar);

/* CLI (ianua P1): seram data/sera capit - servus currens -> II;
 * volumen aperit, invitationem (auctor vacuus, VII dies) creat; codex
 * in piscina, SEMEL - vocator ad stdout scribit, numquam ad acta.
 * 0 bene | I error | II servus currit */
s32
exemplar_invitare (
     constans character* via_datorum,
     constans character* nota,
                 chorda* codex_exitus,
                Piscina* piscina);

#ifdef EXEMPLAR_PROBATIO
/* uncus probationis: invitatio in exemplare currente (auctor vacuus) */
b32
exemplar_invitationem_probationis (
    Exemplar* e,
      chorda* codex_exitus,
     Piscina* piscina);
#endif

#endif /* EXEMPLAR_NUCLEUS_H */
