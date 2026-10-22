/* tintinnabulum.h - Notificationes sonantes: occasio -> cantilena libri
 *
 * Hospes (vicus) occasionem nuntiat ("finis", "permissio", "vitium" -
 * genera cantualis, eadem ac Admonitio.genus); tintinnabulum cantilenam
 * eius generis e cantuali sorte eligit (numquam eandem bis) et per
 * exitum sonat.
 *
 * REGULAE (Franus 2026-10-22):
 *   UNA SIMUL - dum cantilena sonat, occasio nova EXSPECTAT; una sola
 *     exspectat: recentior priorem substituit (recentissima vincit).
 *     Cantilena in INITIO eligitur, non in nuntio - exspectata
 *     substituta nihil consumit.
 *   VOCES = CANTUALE_NOTAE_MAXIMAE - cantilena una sedes numquam deficit
 *     (cantus sedem per notam capit, rector vetustissimam surripit).
 *   EXITUS PIGER - primo sono aperitur, post quietem
 *     TINTINNABULUM_QUIES clauditur: unitas exitus aperta reditum
 *     semper vocat (pila), etiam tacens.
 *   MUTA hospitis sunt (campi radicis): mutus non pulsat; tacere()
 *     sonantem evanescere facit et exitum deinde claudit.
 *
 * COLOR: notae sine '@' colore 'tonus' (sonitus), grex INTERFACIES,
 * tenere = !cantio->intactus.
 *
 * EXITUS per tabulam functionum, ut tintinnabulum auditum non noverit:
 * hospes auditus.h adaptat (vivum); probationes mixturam offline cum
 * horologio manuali dant - eadem logica, sine sono.
 *
 * TEMPUS duplex: quadra exitus (momentum, iussa) et ms hospitis
 * (agere 'nunc': quies) - exitu clauso quadra non procedunt.
 *
 * USUS (vicus):
 *   t = tintinnabulum_creare(piscina, &liber, &exitus, semen);
 *   si (nova && !mutus) (vacuum)tintinnabulum_pulsare(t, genus);
 *   (vacuum)tintinnabulum_agere(t, nunc);          (* omni pulsu *)
 *   si (mutus_factus) tintinnabulum_tacere(t);
 */
/* <aedilis corpus="lib/tintinnabulum.c"/> */
#ifndef TINTINNABULUM_H
#define TINTINNABULUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "mixtura.h"
#include "cantuale.h"

/* quies ante exitum clausum, ms */
#define TINTINNABULUM_QUIES (XXX * M)

/* textus cantilenae datae: octeti maximi (= nuntius praeconii) */
#define TINTINNABULUM_TEXTUS_MAXIMUS CC

nomen structura {
      b32 (*aperire)        (vacuum* ctx, Mixtura* mixtura);
    vacuum (*claudere)      (vacuum* ctx);
      s64 (*momentum)       (vacuum* ctx);    /* quadra reddita */
      i32 (*mora)           (vacuum* ctx);    /* latentia, quadra */
      b32 (*iubere)         (vacuum* ctx,
          constans MixturaIussum* iussum);
      b32 (*nuntium_legere) (vacuum* ctx, MixturaNuntius* nuntius);
    vacuum* ctx;
} TintinnabuliExitus;

nomen structura Tintinnabulum Tintinnabulum;

/* mixtura (CANTUALE_NOTAE_MAXIMAE voces) et rector e piscina; liber et
 * exitus vivant dum tintinnabulum. Exitus NON aperitur. NIHIL si memoria
 * deest */
Tintinnabulum*
tintinnabulum_creare (
                        Piscina* piscina,
              constans Cantuale* liber,
    constans TintinnabuliExitus* exitus,
                            i64  semen);

/* occasio: quieta -> exitum aperit (si clausus) et cantilenam generis
 * statim incipit; sonans -> genus exspectat (prius exspectatum
 * substituitur). FALSUM: nulla cantio generis, aut exitus non apertus
 * (sine sono - hospes pergit) */
b32
tintinnabulum_pulsare (
    Tintinnabulum* t,
           chorda  genus);

/* cantilena DATA (praeco cantus), non e libro: textus copiatur et
 * iudicatur ut liber (cantionem_legere); deinde ut occasio - quieta
 * statim, sonans exspectat (recentissima vincit: genus exspectatum
 * quoque substituit, et vice versa). Lectio in piscinis PROPRIIS
 * tintinnabuli (sonans, exspectans, probanda - permutantur, non
 * copiantur): piscina vocantis per cantilenas non crescit. FALSUM:
 * textus fractus aut > TINTINNABULUM_TEXTUS_MAXIMUS (vitium impletum
 * si non NIHIL), aut exitus non apertus - nihil mutatur */
b32
tintinnabulum_canere (
     Tintinnabulum* t,
            chorda  textus,
               b32  intactus,
    CantualeVitium* vitium);

/* omni pulsu hospitis: nuntios legit (sedes liberantur); cantilena
 * finita -> exspectata incipit; quies >= TINTINNABULUM_QUIES ->
 * exitus clauditur. VERUM dum sonat aut exspectat (hospes pulsum
 * celerem servet) */
b32
tintinnabulum_agere (
    Tintinnabulum* t,
              s64  nunc);   /* ms hospitis */

/* silentium: voces sistuntur (evanescentia brevis, sine crepitu),
 * exspectata abicitur; exitus clauditur cum evanuerunt (agere proximo),
 * non post quietem - unitas statim clausa media unda crepitaret */
vacuum
tintinnabulum_tacere (
    Tintinnabulum* t);

b32
tintinnabulum_sonat (
    constans Tintinnabulum* t);

b32
tintinnabulum_apertum (
    constans Tintinnabulum* t);

/* octeti in piscinis propriis (cantilenae datae): per cantilenas non
 * crescit - sedes quaeque solam lectionem suam tenet */
memoriae_index
tintinnabulum_memoria (
    constans Tintinnabulum* t);

/* exitum claudit (si apertus, statim) et piscinas proprias liberat;
 * t deinde non adhibendum (memoria eius vocantis manet) */
vacuum
tintinnabulum_destruere (
    Tintinnabulum* t);

/* cantio sonans aut ultima sonata; NIHIL ante primam (cantilena data:
 * valet donec cantilena data proxima incipit) */
constans Cantio*
tintinnabulum_ultima (
    constans Tintinnabulum* t);

#endif /* TINTINNABULUM_H */
