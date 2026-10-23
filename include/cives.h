/* cives.h - personae rationum: quis aliquis sit (ianua T1)
 *
 * Applicatio cives in volumine suo habet (tabulae propriae per
 * volumen_scrinium, CREATE IF NOT EXISTS - ianua P2):
 *
 *   cives        id (ULID, stabilis in perpetuum), nomen_usoris
 *                (handle praesens, UNICUM), titulus (nomen ostensum),
 *                status, creatus_ms
 *   cives_nomina nomen_usoris -> civis: omne nomen umquam usum.
 *                Nomen semel usum RESERVATUR in perpetuum (renominatio
 *                et deletio id non liberant - ianua P4), ne linea
 *                actorum vetus personam novam significet.
 *
 * Nomen usoris: I..XXXII signa a-z 0-9 . _ - (minusculae; ingressus
 * per cives_nomen_normare prius - ianua P13).
 *
 * TRANSACTIO VOCATORIS (ianua P3): omnis functio scribens intra
 * volumen_transactionem_incipere/committere vocanda est; cives
 * transactionem propriam numquam aperit. FALSUM = nihil probandum,
 * vocator revolvit.
 *
 * Acta (sine secretis): civis.creatus {id, nomen_usoris, titulus},
 * civis.renominatus {id, nomen_usoris}, civis.status {id, status},
 * civis.deletus {id}. Deletio = status DELETUS (sepulcrum); deletio
 * vera (destructio cryptographica) desideratum ...MK0 pars II.
 */
#ifndef CIVES_H
#define CIVES_H

/* <aedilis corpus="lib/cives.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "volumen.h"
#include "xar.h"

#define CIVES_NOMEN_MAXIMUM XXXII
#define CIVES_ID_MENSURA    XXVI       /* ULID */

nomen enumeratio {
    CIVIS_ACTIVUS = ZEPHYRUM,
    CIVIS_SUSPENSUS,
    CIVIS_DELETUS
} CivisStatus;

/* chordae in piscina vocantis */
nomen structura {
         chorda id;
         chorda nomen_usoris;   /* handle praesens */
         chorda titulus;
    CivisStatus status;
            s64 creatus_ms;
} Civis;

nomen structura Cives Cives;

/* tabulas creat si desunt; nunc_ms NIHIL = horologium verum */
Cives*
cives_aperire (
     Volumen* volumen,
         s64  (*nunc_ms)(vacuum),
     Piscina* piscina);

/* I..XXXII signa a-z 0-9 . _ - */
b32
cives_nomen_validum (
    chorda nomen_usoris);

/* minusculae ASCII (copia in piscina); cetera intacta */
chorda
cives_nomen_normare (
      chorda  nomen_usoris,
     Piscina* piscina);

/* INTRA TRANSACTIONEM. FALSUM: nomen invalidum aut iam (etiam olim)
 * usum. id_exitus (XXVI signa) in piscina. */
b32
cives_creare (
        Cives* cives,
       chorda  nomen_usoris,
       chorda  titulus,
       chorda* id_exitus,
      Piscina* piscina);

b32
cives_per_id (
        Cives* cives,
       chorda  id,
        Civis* exitus,
      Piscina* piscina);

/* handle PRAESENS solum (nomina vetera non inveniuntur) */
b32
cives_per_nomen (
        Cives* cives,
       chorda  nomen_usoris,
        Civis* exitus,
      Piscina* piscina);

/* INTRA TRANSACTIONEM. id manet; nomen vetus reservatum manet.
 * FALSUM: id ignotum, nomen invalidum aut iam usum. */
b32
cives_nomen_mutare (
      Cives* cives,
     chorda  id,
     chorda  nomen_novum);

/* INTRA TRANSACTIONEM. FALSUM: id ignotum. */
b32
cives_statum_ponere (
           Cives* cives,
          chorda  id,
     CivisStatus  status);

/* INTRA TRANSACTIONEM: status DELETUS + civis.deletus */
b32
cives_delere (
     Cives* cives,
    chorda  id);

/* omnes (etiam suspensi et deleti), ordine creationis - eodem ms
 * ordine inscriptionis (rowid). Xar de Civis in piscina; tabula
 * vacua, non NIHIL, si nemo; NIHIL solum si error (canalis T3) */
Xar*
cives_enumerare (
      Cives* cives,
    Piscina* piscina);

#endif /* CIVES_H */
