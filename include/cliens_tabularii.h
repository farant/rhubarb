/* cliens_tabularii.h - Cliens daemonis tabulariumd pro apps
 *
 * Dialectus MCP super TCP, connexio PER-PETITIONEM (contractus v1):
 * conectere, lineam mittere, responsum ad lineam novam legere,
 * claudere. Daemon se praeinitiat, ergo initialize non necessarium.
 *
 * START-IF-ABSENT: si daemon abest, launcher semel excitatur; post
 * conatum primum defectus CITUS redditur - anquisitio IS (II s)
 * tractatorem obstruentem V s congelaret.
 *
 * Extractum ex apps/forum/forum.c (2026-07-24) ut app secunda
 * (villa) eodem clienti uteretur SINE duplicatione: probationes
 * radicis apps/ non vident, ergo cliens in apps/ manens tegumentum
 * habere non poterat.
 *
 * USUS:
 *   ClientTabularii cliens;
 *   cliens_tabularii_incipere(&cliens, 8753);
 *   cliens.praefixum = "[villa]";
 *   ...
 *   JsonValor* args = json_objectum_creare(pn);
 *   json_objectum_ponere(args, "genus", json_chorda_creare(pn, g));
 *   chorda textus = cliens_tabularii_vocare(&cliens, pn, "legere",
 *                                           args, &culpa);
 */

#ifndef CLIENS_TABULARII_H
#define CLIENS_TABULARII_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"

/* ========================================================================
 * TYPI
 * ======================================================================== */

nomen structura {
    i32                 portus;
    s64                 petitio_index;   /* id JSON-RPC crescens */
    constans character* actor;           /* iniectus in transmittere */
    constans character* launcher;        /* via ad scriptum daemonis */
    constans character* praefixum;       /* nota stderr, e.g. "[villa]" */
    b32                 genitus;         /* launcher iam excitatus?
                                          * (in structura, non
                                          * hic_manens: duo clientes
                                          * in eodem processu portus
                                          * diversos servire possunt) */
} ClientTabularii;

/* ========================================================================
 * FUNCTIONES
 * ======================================================================== */

/* Clientem ad valores ordinarios ponere (portus datus; actor
 * "fran"; launcher "./gesta/tabulariumd.sh"; praefixum "[cliens]"). */
vacuum
cliens_tabularii_incipere(ClientTabularii* cliens, i32 portus);

/* Instrumentum daemonis vocare. Fructus = textus responsi (chorda
 * vacua + culpa posita in defectu). */
chorda
cliens_tabularii_vocare(
    ClientTabularii*    cliens,
    Piscina*            pn,
    constans character* nomen_instrumenti,
    JsonValor*          argumenta,
    chorda*             culpa);

/* legere {genus, quantum} -> tabulatum JSON parsatum (NIHIL = culpa). */
JsonValor*
cliens_tabularii_legere(
    ClientTabularii* cliens,
    Piscina*         pn,
    chorda           genus,
    i32              quantum,
    chorda*          culpa);

/* legere cum argumentis ARBITRARIIS (sine_campis, res, status...) -
 * idem fructus parsatus. Vocator obiectum aedificat, ergo campi novi
 * daemonis hic numquam reaperiunt. */
JsonValor*
cliens_tabularii_legere_cum(
    ClientTabularii* cliens,
    Piscina*         pn,
    JsonValor*       argumenta,
    chorda*          culpa);

/* "res <ID> creata" -> chorda ID (vacua si absens). */
chorda
cliens_tabularii_res_id(chorda textus, Piscina* pn);

/* chorda -> litterae NUL-terminatae in piscina. */
constans character*
cliens_tabularii_litterae(Piscina* pn, chorda c);

/* Tractator internuntii: {instrumentum, argumenta} ->
 * {bene, textus, res_id?}. Datum tractatoris = ClientTabularii*.
 *
 * ALLOWLIST FIXA: addere|gerere|legere|quaerere. Haec est superficies
 * "app mundum scribit" tota - genera et campi novi C numquam
 * reaperiunt. Actor ex cliente iniectus.
 *
 *   internuntius_praebere(inx, "transmittere",
 *       cliens_tabularii_transmittere, &cliens);
 */
JsonValor*
cliens_tabularii_transmittere(
    JsonValor* argumenta,
    Piscina*   piscina,
    vacuum*    datum,
    chorda*    culpa);

#endif /* CLIENS_TABULARII_H */
