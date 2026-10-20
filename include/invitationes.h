/* invitationes.h - quis intrare possit: codices per personam
 * (ianua T3)
 *
 * Tabula invitationes in volumine applicationis (CREATE IF NOT
 * EXISTS - ianua P2): id (ULID), sigillum (SHA-256 hex codicis,
 * UNICUM), auctor (civis; vacuus = CLI), nota, creata_ms, expirat_ms,
 * usus_max, usus_numerus, usa_a (civis ultimus), revocata_ms.
 *
 * CODEX: XVI octeti fortuiti base64url (XXII signa), SEMEL redditus
 * ab invitationes_creare. Solum digestum conditur; codex numquam in
 * tabula, actis, erroribus. Valida = non expirata (expirat_ms > nunc),
 * non revocata, usus reliqui.
 *
 * TRANSACTIO VOCATORIS (ianua P3) pro creare, consumere, revocare.
 * consumere per UPDATE CUSTODITUM decernit (numerus < max, non
 * expirata, non revocata; lineae mutatae == I) - inspectio prior
 * nihil promittit: duae inscriptiones eodem codice, una vincit.
 *
 * Acta (sine codice nec digesto): invitatio.creata {id, auctor,
 * expirat_ms, usus_max}, invitatio.usa {id, civis},
 * invitatio.revocata {id}.
 */
#ifndef INVITATIONES_H
#define INVITATIONES_H

/* <aedilis corpus="lib/invitationes.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"

#define INVITATIONES_CODEX_MENSURA  XXII      /* XVI octeti base64url */
#define INVITATIONES_VITA_ORDINARIA ((s64)VII * XXIV * LX * LX * M)

/* chordae in piscina vocantis */
nomen structura {
    chorda id;
    chorda auctor;         /* vacua = CLI */
    chorda nota;
       s64 creata_ms;
       s64 expirat_ms;
       i32 usus_max;
       i32 usus_numerus;
    chorda usa_a;          /* vacua = nondum usa */
       s64 revocata_ms;    /* ZEPHYRUM = non revocata */
} Invitatio;

nomen structura Invitationes Invitationes;

/* tabulam creat si deest; nunc_ms NIHIL = horologium verum */
Invitationes*
invitationes_aperire (
     Volumen* volumen,
         s64  (*nunc_ms)(vacuum),
     Piscina* piscina);

/* INTRA TRANSACTIONEM. codex (SEMEL) et id in piscina. vita_ms
 * ZEPHYRUM = ordinaria (VII dies); usus_max ZEPHYRUM = I. */
b32
invitationes_creare (
    Invitationes* invitationes,
          chorda  auctor,
          chorda  nota,
             s64  vita_ms,
             i32  usus_max,
          chorda* codex_exitus,
          chorda* id_exitus,
         Piscina* piscina);

/* valida? nihil mutat; exitus in piscina */
b32
invitationes_inspicere (
    Invitationes* invitationes,
          chorda  codex,
       Invitatio* exitus,
         Piscina* piscina);

/* INTRA TRANSACTIONEM: usus unus pro civi; FALSUM si non valida */
b32
invitationes_consumere (
    Invitationes* invitationes,
          chorda  codex,
          chorda  civis);

/* INTRA TRANSACTIONEM: auctoris solum; FALSUM si aliena, ignota aut
 * iam revocata */
b32
invitationes_revocare (
    Invitationes* invitationes,
          chorda  auctor,
          chorda  id);

/* auctoris (Xar de Invitatio), novissimae primum */
Xar*
invitationes_enumerare (
    Invitationes* invitationes,
          chorda  auctor,
         Piscina* piscina);

#endif /* INVITATIONES_H */
