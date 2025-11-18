#ifndef TEMPUS_H
#define TEMPUS_H

#include "latina.h"
#include "piscina.h"

/* ==================================================
 * Typi Fundamentales
 * ================================================== */

/* Status horologium temporis */
nomen structura {
    i64 pulsus_initii;
    i64 pulsus_ultimi_quadri;
    i64 pulsus_currentis;
    f64 frequentia;
    f64 delta_tempus;
    f64 tempus_totale;
    f64 accumulator;
} Tempus;

/* Statisticae quadrorum */
nomen structura {
    f64 quadra_destinata_per_secundum;
    f64 quadra_currentia_per_secundum;
    f64 quadra_media_per_secundum;
    i32 numerus_quadrorum;
    f64 tempus_minimum_quadri;
    f64 tempus_maximum_quadri;
    f64 tempora_quadrorum[CXX];
    i32 index_temporis_quadri;
} StatisticaeQuadri;


/* ==================================================
 * Horologium Globale
 * ================================================== */

vacuum
tempus_initiare (
    vacuum);

f64
tempus_nunc (
    vacuum);

f64
tempus_delta (
    vacuum);

vacuum
tempus_quadrum (
    vacuum);

Tempus*
tempus_obtinere_horologium (
    vacuum);


/* ==================================================
 * Administratio Quadrorum per Secundum
 * ================================================== */

vacuum
tempus_ponere_quadra_destinata (
    f64 quadra_per_secundum);

vacuum
tempus_exspectare_quadrum (
    vacuum);

StatisticaeQuadri*
tempus_obtinere_statisticas (
    vacuum);


/* ==================================================
 * Horologia Independentia
 * ================================================== */

Tempus*
tempus_creare (
    Piscina* piscina);

vacuum
tempus_restituere (
    Tempus* tempus);

f64
tempus_elapsum (
    Tempus* tempus);

vacuum
tempus_actualizare (
    Tempus* tempus);


/* ==================================================
 * Functiones Lenitionis (t debet esse 0.0 ad 1.0)
 * ================================================== */

f32 lenire_linearis(f32 t);
f32 lenire_quadraticus_ingressus(f32 t);
f32 lenire_quadraticus_egressus(f32 t);
f32 lenire_quadraticus_ingressus_egressus(f32 t);
f32 lenire_cubicus_ingressus(f32 t);
f32 lenire_cubicus_egressus(f32 t);
f32 lenire_cubicus_ingressus_egressus(f32 t);
f32 lenire_sinus_ingressus(f32 t);
f32 lenire_sinus_egressus(f32 t);
f32 lenire_sinus_ingressus_egressus(f32 t);
f32 lenire_exponens_ingressus(f32 t);
f32 lenire_exponens_egressus(f32 t);
f32 lenire_exponens_ingressus_egressus(f32 t);
f32 lenire_retro_ingressus(f32 t);
f32 lenire_retro_egressus(f32 t);
f32 lenire_retro_ingressus_egressus(f32 t);
f32 lenire_elasticus_ingressus(f32 t);
f32 lenire_elasticus_egressus(f32 t);
f32 lenire_elasticus_ingressus_egressus(f32 t);
f32 lenire_resiliens_ingressus(f32 t);
f32 lenire_resiliens_egressus(f32 t);
f32 lenire_resiliens_ingressus_egressus(f32 t);


/* ==================================================
 * Adiutores Animationis
 * ================================================== */

f32
animare_ad (
    f32 currens,
    f32 destinatum,
    f32 celeritas,
    f32 delta_tempus);

f32
elastrum_ad (
    f32  currens,
    f32  destinatum,
    f32* velocitas,
    f32  rigiditas,
    f32  damnum,
    f32  delta_tempus);


/* ==================================================
 * Interpolatio
 * ================================================== */

f32
interpolare (
    f32 a,
    f32 b,
    f32 t);

f32
interpolare_angulum (
    f32 a,
    f32 b,
    f32 t);

f32
cohibere (
    f32 valor,
    f32 minimum,
    f32 maximum);

f32
gradus_lenis (
    f32 limen_infimum,
    f32 limen_supremum,
    f32 x);

i32
interpolare_colorem (
    i32 color_primus,
    i32 color_secundus,
    f32 t);


/* ==================================================
 * Functiones Undae
 * ================================================== */

f32
unda_sinus (
    f32 t,
    f32 frequentia,
    f32 amplitudo);

f32
unda_triangulus (
    f32 t,
    f32 frequentia,
    f32 amplitudo);

f32
unda_dens_serrae (
    f32 t,
    f32 frequentia,
    f32 amplitudo);

f32
unda_quadratus (
    f32 t,
    f32 frequentia,
    f32 amplitudo);

f32
unda_tumultus (
    f32 t);


/* ==================================================
 * Utilitas
 * ================================================== */

f32
circumvolvere (
    f32 valor,
    f32 minimum,
    f32 maximum);

f32
oscillare (
    f32 t,
    f32 longitudo);

f32
remappare (
    f32 valor,
    f32 ex_minimum,
    f32 ex_maximum,
    f32 ad_minimum,
    f32 ad_maximum);

#endif /* TEMPUS_H */
