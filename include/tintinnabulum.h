/* tintinnabulum.h - Notificationes sonantes: occasio -> cantilena libri
 *
 * Hospes (vicus) occasionem nuntiat ("finis", "permissio", "vitium" -
 * genera cantualis, eadem ac Admonitio.genus); tintinnabulum cantilenam
 * eius generis e cantuali sorte eligit (numquam eandem bis) et in
 * hospite soni communi (concentus.h) sonat.
 *
 * REGULAE (Franus 2026-10-22):
 *   UNA SIMUL - dum cantilena sonat, occasio nova EXSPECTAT; una sola
 *     exspectat: recentior priorem substituit (recentissima vincit).
 *     Cantilena in INITIO eligitur, non in nuntio - exspectata
 *     substituta nihil consumit.
 *   VOCES - cantus sedem per notam capit (prioritas I) in rectore
 *     concentus; concentus voces >= CANTUALE_NOTAE_MAXIMAE habeat, ne
 *     cantilena una deficiat. Vox altior (aetherophonum) non
 *     surripitur.
 *   EXITUS concentus est (piger, quies, nuntii): tintinnabulum eum
 *     solum aperire petit et sedes suas per concentus_vox_vivit
 *     interrogat.
 *   MUTA hospitis sunt (campi radicis): mutus non pulsat; tacere()
 *     voces SUAS evanescere facit et concentum quiescere iubet
 *     (clauditur cum nulla vox vivit - vox aliena exitum tenet).
 *
 * COLOR: notae sine '@' colore 'tonus' (sonitus), grex INTERFACIES,
 * tenere = !cantio->intactus.
 *
 * USUS (vicus_sonus_porta):
 *   k = concentus_creare(piscina, &forma, &exitus);
 *   t = tintinnabulum_creare(piscina, &liber, k, semen);
 *   si (nova && !mutus) (vacuum)tintinnabulum_pulsare(t, genus);
 *   (vacuum)concentus_agere(k, nunc);              (* omni pulsu, *)
 *   (vacuum)tintinnabulum_agere(t);                (* hoc ordine *)
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
#include "concentus.h"

/* textus cantilenae datae: octeti maximi (= nuntius praeconii) */
#define TINTINNABULUM_TEXTUS_MAXIMUS CC

nomen structura Tintinnabulum Tintinnabulum;

/* liber et concentus vivant dum tintinnabulum. Nihil aperitur. NIHIL
 * si memoria deest */
Tintinnabulum*
tintinnabulum_creare (
              Piscina* piscina,
    constans Cantuale* liber,
            Concentus* concentus,
                  i64  semen);

/* occasio: quieta -> concentum aperit (si clausus) et cantilenam
 * generis statim incipit; sonans -> genus exspectat (prius exspectatum
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

/* omni pulsu hospitis, POST concentus_agere (nuntii iam lecti):
 * cantilena finita -> exspectata incipit. VERUM dum sonat aut exspectat
 * (hospes pulsum celerem servet) */
b32
tintinnabulum_agere (
    Tintinnabulum* t);

/* silentium: voces suae sistuntur (evanescentia brevis, sine crepitu),
 * exspectata abicitur, concentus quiescere iubetur (clauditur cum
 * evanuerunt, nisi vox aliena vivit) */
vacuum
tintinnabulum_tacere (
    Tintinnabulum* t);

b32
tintinnabulum_sonat (
    constans Tintinnabulum* t);

/* octeti in piscinis propriis (cantilenae datae): per cantilenas non
 * crescit - sedes quaeque solam lectionem suam tenet */
memoriae_index
tintinnabulum_memoria (
    constans Tintinnabulum* t);

/* piscinas proprias liberat (exitus concentus est, non hic); t deinde
 * non adhibendum (memoria eius vocantis manet) */
vacuum
tintinnabulum_destruere (
    Tintinnabulum* t);

/* cantio sonans aut ultima sonata; NIHIL ante primam (cantilena data:
 * valet donec cantilena data proxima incipit) */
constans Cantio*
tintinnabulum_ultima (
    constans Tintinnabulum* t);

#endif /* TINTINNABULUM_H */
