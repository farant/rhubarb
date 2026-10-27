/* credentialia.h - quomodo aliquis se probet: tesserae per personam
 * (ianua T2)
 *
 * Tabula credentialia in volumine applicationis (CREATE IF NOT
 * EXISTS - ianua P2): id (ULID), civis, genus ("tessera"; claves
 * transeuntes postea genus alterum), phc (argon2id), creatum_ms,
 * ultimus_usus_ms. Persona plura habere potest; tessera UNA per
 * personam (ponere priorem delet).
 *
 * OPERA DILATA (ex exemplare translata): hash et verificatio per
 * custodiam in reactore; CREDENTIALIA_OPERA loculi tesseram copiatam
 * tenent (post usum deletam), nomen, claves freni, civem, reditum.
 *
 * FRENUM (ianua P6): claves "par:<ip>" et "usor:<nomen>" - mora per
 * timer reactoris (vocatio numquam obstat), recusatio sine reditu.
 * Persona ignota, non activa, aut sine tessera: hash ficticium plenum
 * (custodia) - tempus idem, bona FALSUM.
 *
 * TRANSACTIO VOCATORIS (ianua P3) pro credentialia_tesseram_ponere.
 * Exceptio UNA: re-hash post verificationem bonam (parametri veteres)
 * transactionem propriam aperit - in reditu reactoris, post responsum.
 *
 * Acta: credentiale.positum {civis, genus} - numquam phc nec tessera.
 * Nomen credentiale.remotum reservatum (nondum usum - ianua P11).
 */
#ifndef CREDENTIALIA_H
#define CREDENTIALIA_H

/* <aedilis corpus="lib/credentialia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "volumen.h"
#include "custodia.h"
#include "cives.h"

#define CREDENTIALIA_OPERA  LXIV
#define CREDENTIALE_TESSERA "tessera"

nomen structura Credentialia Credentialia;

/* tabulam creat si deest; nunc_ms NIHIL = horologium verum */
Credentialia*
credentialia_aperire (
     Volumen* volumen,
       Cives* cives,
    Custodia* custodia,
     Reactor* reactor,
         s64  (*nunc_ms)(vacuum),
     Piscina* piscina);

/* hash in labore; reditus cum phc (vacua = error). FALSUM sine
 * reditu: tessera invalida (VIII..MXXIV), cauda plena. */
b32
credentialia_digerere (
           Credentialia* credentialia,
                 chorda  tessera,
    CustodiaHashReditus  reditus,
                 vacuum* datum);

/* INTRA TRANSACTIONEM: tessera civis (prior deletur) + actum.
 * FALSUM: civis ignotus, phc vacua, scriptura fracta. */
b32
credentialia_tesseram_ponere (
    Credentialia* credentialia,
          chorda  civis,
          chorda  phc);

nomen enumeratio {
    CREDENTIALIA_COEPTUM = ZEPHYRUM,  /* reditus postea vocabitur */
    CREDENTIALIA_FRENATUM,            /* conatus nimis multi (429) */
    CREDENTIALIA_OCCUPATUM,           /* loculi pleni (503) */
    CREDENTIALIA_PRAVUM               /* nomen/tessera/clavis longa */
} CredentialiaInitium;

/* civis (ULID) valida solum intra reditum; bona FALSUM -> vacua.
 * Reditus semper ex reactore, numquam intra vocationem. */
nomen vacuum (*CredentialiaReditus)(
    vacuum* datum,
       b32  bona,
    chorda  civis);

/* nomen_usoris iam normatum (cives_nomen_normare); clavis_paris = IP
 * clientis (<= LX). Reditus SOLUM post COEPTUM. */
CredentialiaInitium
credentialia_verificare (
           Credentialia* credentialia,
                 chorda  nomen_usoris,
                 chorda  tessera,
                 chorda  clavis_paris,
    CredentialiaReditus  reditus,
                 vacuum* datum);

#endif /* CREDENTIALIA_H */
