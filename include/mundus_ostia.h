/*
 * mundus_ostia.h - ostia loci: ubi persona ex mundo in scaenam intrat
 *
 * Ostium est punctum in solo loci cum radio et titulo prospectus
 * scaenae. Persona intra radium stans signum "[E]" videt et intrare
 * potest (latus: mundus_latus.h; project-specs/ostium-spec.md).
 *
 * Ostia in DISPOSITIONE loci scribuntur (mundus_locus.h):
 *
 *   <ostium locus="29 46" radius="1.5" scaena="hut-door"/>
 *
 * et in volumine IUXTA locum habitant, non in eo: actum proprium
 * quod locum nominat et ostia eius omnia fert. Statua loci ea non
 * novit; vicus vetus actum ignotum praeterit.
 *
 * Actum (datum JSON; numeri MILLESIMAE integrae unitatum mundi, ne
 * octeti a forma fluitantium pendeant):
 *   ostia-posita  {"locus": "clearing", "ostia": [
 *                    {"x": 29000, "z": 46000, "radius": 1500,
 *                     "scaena": "hut-door"}]}
 *
 * Actum novissimum loci vincit; index vacuus ostia tollit. Index
 * idem iterum positus nihil scribit. Solum acta PROPRIA huius
 * voluminis leguntur (volumen.h, lex communionis).
 *
 * PROBARE RECUSAT, lineam ostii culpati nominans (primum vincit):
 *   - titulum scaenae malum (regula scaena_titulus_validus: I ..
 *     LXIV litterae ex a-z, 0-9, '-' et '_', non "mundus");
 *   - radium extra 0.5 .. 8;
 *   - punctum extra tabulam gressus loci;
 *   - ostium cui nulla cella est in qua corpus stare potest
 *     (mundus_solum_stare) et cuius centrum intra radium iacet -
 *     signum numquam appareret;
 *   - duo ostia in puncto eodem (millesimis paribus);
 *   - plura quam MUNDUS_OSTIA_MAXIMA.
 * Titulus scaenae quae in volumine NON est non recusatur: loci et
 * scaenae instrumentis diversis conduntur; latus rem tractat.
 */

#ifndef MUNDUS_OSTIA_H
#define MUNDUS_OSTIA_H

/* <aedilis corpus="lib/mundus_ostia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "mundus_solum.h"
#include "mundus_locus.h"

#define MUNDUS_OSTIA_ACTUM           "ostia-posita"
#define MUNDUS_OSTIA_MAXIMA          LXIV
#define MUNDUS_OSTII_RADIUS_MINIMUS  0.5f
#define MUNDUS_OSTII_RADIUS_MAXIMUS  8.0f

nomen structura {
    chorda scaena;             /* titulus prospectus */
       s32 x_millesimae;
       s32 z_millesimae;
       s32 radius_millesimae;
} MundusOstium;

nomen structura {
             b32  successus;
    MundusOstium* ostia;       /* ordine dispositionis; NIHIL si
                                * recusata aut nulla */
             i32 numerus;
          chorda error;       /* Anglice; vacua si successus */
             i32 linea;       /* linea ostii culpati; 0 si nulla */
} MundusOstiaFructus;

/*
 * mundus_ostia_locus_proximus - cella ostio proxima ubi corpus stat
 *
 * Centrum cellae in qua corpus radii 'corpus' stare potest
 * (mundus_solum_stare), a puncto (x, z) non plus 'radius' distans,
 * ex talibus proximum; ex paribus prius ordine tabulae (z, deinde
 * x). Eadem regula qua mundus_ostia_probare ostium accipit: ostium
 * probatum semper locum habet.
 *
 * @locus_x, @locus_z: NIHIL licet (interrogatio sola)
 *
 * Redde: FALSUM (exitus non scripti) si nulla talis cella aut solum
 * NIHIL
 */
b32
mundus_ostia_locus_proximus (
    constans MundusSolum* solum,
                     f32  x,
                     f32  z,
                     f32  radius,
                     f32  corpus,
                     f32* locus_x,
                     f32* locus_z);

/*
 * mundus_ostia_probare - ostia dispositionis contra locum probare
 *
 * @ostia:   ut mundus_locus_legere ea dedit
 * @solum:   tabula gressus loci INSTRUCTI (res positae iam obstant)
 * @corpus:  radius corporis quo "stare" probatur
 *
 * Redde: successus VERUM cum ostiis in millesimas versis (numerus 0
 * licet), aut FALSUM cum errore et linea; linea 0 si argumenta
 * NIHIL aut memoria deficit
 */
MundusOstiaFructus
mundus_ostia_probare (
    constans MundusLocusOstium* ostia,
                           i32  numerus,
          constans MundusSolum* solum,
                           f32  corpus,
                       Piscina* piscina);

/*
 * mundus_ostia_ponere - ostia loci in volumine ponere
 *
 * Redde: VERUM si index hic nunc loci est (etiam si iam erat: nihil
 * scriptum; index vacuus loco sine ostiis: nihil scriptum); FALSUM
 * si volumen NIHIL, titulus loci vacuus, aut scriptura deficit
 * (nihil mutatum)
 */
b32
mundus_ostia_ponere (
                  Volumen* volumen,
                   chorda  locus,
    constans MundusOstium* ostia,
                      i32  numerus);

/*
 * mundus_ostia_legere - ostia loci ex volumine
 *
 * Redde: Xar de MundusOstium in piscina (vacuum si nulla aut locus
 * ignotus); NIHIL si memoria deficit. Actum cuius datum legi non
 * potest praeteritur (prius valet).
 */
Xar*
mundus_ostia_legere (
    Volumen* volumen,
     chorda  locus,
    Piscina* piscina);

#endif /* MUNDUS_OSTIA_H */
