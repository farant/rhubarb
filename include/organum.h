/* organum.h - manifestum organi (widget): status classificatus per
 * campum (vita, rhythmus, sedes, aspectus, referentia), ligamina,
 * portae, actiones; canon insulae EX CAMPIS GENERATUR (definitio
 * una). project-specs/status-plan.md S1; census
 * project-specs/status-genera-census.md IX-XI.
 *
 * Iudicium duplex: canon organi (organum.canon, infixus per
 * organum_canon.h) formam iudicat; onerator leges quas canon
 * consulto non tenet (attributa plura simul, valor fissus):
 *   - sedes motus -> rhythmus mobilis et vita transiens
 *   - typus series -> sedes motus
 *   - orbatus -> referentia
 *   - referentia 'ligamen/genus' -> ligamen declaratum
 *   - absens -> valor typi sui
 *   - <optio> solum sub typo electio, qui unam saltem poscit
 *
 * Onerator plagulam numquam legit: fons a vocante, basis a vocante
 * addita (organum_basim_addere). */

#ifndef ORGANUM_H
#define ORGANUM_H

/* <aedilis corpus="lib/organum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "stml.h"
#include "canon.h"


/* ==================================================
 * Typi
 * ================================================== */

nomen enumeratio {
    ORGANUM_VITA_PERSISTENS = I,
    ORGANUM_VITA_TRANSIENS,
    ORGANUM_VITA_PROBATIO
} OrganumVita;

nomen enumeratio {
    ORGANUM_RHYTHMUS_DISCRETUS = I,
    ORGANUM_RHYTHMUS_MOBILIS
} OrganumRhythmus;

nomen enumeratio {
    ORGANUM_SEDES_INSULA = I,
    ORGANUM_SEDES_MOTUS
} OrganumSedes;

nomen structura {
             chorda  titulus;
             chorda  typus;        /* "numerus", "electio", ... */
        OrganumVita  vita;
    OrganumRhythmus  rhythmus;     /* absens = DISCRETUS */
       OrganumSedes  sedes;        /* absens = INSULA */
                b32  habet_absens;
             chorda  absens;
             chorda  scriptor;     /* vacua = nullus */
             chorda  aspectus;     /* vacua = nullus; "popover" */
             chorda  referentia;   /* "ligamen/genus"; vacua = nulla */
             chorda  orbatus;      /* vacua = nullus */
          StmlNodus* nodus;        /* in manifesto (optiones, nota) */
} OrganumCampus;

nomen structura {
    chorda titulus;
    chorda genus;
       b32 a_parente;
} OrganumLigamen;

nomen structura {
    chorda titulus;
       b32 systema;
} OrganumActio;

nomen structura {
    chorda titulus;
    chorda modus;                 /* "petitio" | "eventus" */
} OrganumPorta;

nomen structura Organum Organum;   /* opacum */


/* ==================================================
 * Lectio
 * ================================================== */

/* manifestum legere et iudicare: canon organi + leges oneratoris.
 * NIHIL si vitia. *vitia = Xar de chorda (nuntii nominati, linea
 * manifesti praefixa), SEMPER impletum (vacuum si sanum). */
Organum*
organum_legere (
                 chorda   fons,
                Piscina*  piscina,
    InternamentumChorda*  intern,
                    Xar** vitia);

/* basis (organum-commune) campos suos ante proprios addit; FALSUM
 * si titulus campi bis (nihil additum) */
b32
organum_basim_addere (
             Organum* organum,
    constans Organum* basis);


/* ==================================================
 * Quaestiones
 * ================================================== */

chorda
organum_genus (
    constans Organum* organum);

i32
organum_numerus_camporum (
    constans Organum* organum);

constans OrganumCampus*
organum_campus_index (
     constans Organum* organum,
                  i32  index);

/* NIHIL si nullus */
constans OrganumCampus*
organum_campus (
     constans Organum* organum,
               chorda  titulus);

i32
organum_numerus_ligaminum (
    constans Organum* organum);

constans OrganumLigamen*
organum_ligamen_index (
     constans Organum* organum,
                  i32  index);

i32
organum_numerus_actionum (
    constans Organum* organum);

constans OrganumActio*
organum_actio_index (
     constans Organum* organum,
                  i32  index);

i32
organum_numerus_portarum (
    constans Organum* organum);

constans OrganumPorta*
organum_porta_index (
     constans Organum* organum,
                  i32  index);


/* ==================================================
 * Generatio
 * ================================================== */

/* valores initiales (status-plan S4): attributa ' titulus="absens"'
 * camporum sedis INSULAE cum absente, ordine manifesti - textus pro
 * insula_ramum_initiare. Vacua si nullus. */
chorda
organum_initium (
    constans Organum* organum,
             Piscina* piscina);

/* canon insulae GENERATUS: campi sedis INSULAE soli (motus extra),
 * radix = elementum nominatum 'genus' organi (mos per-ramum);
 * NIHIL si generatio deficit (causa) */
Canon*
organum_canonem_generare (
       constans Organum* organum,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* causa);

/* <domini> generatus pro insula_ramus_dominos_legere: campi sedis
 * INSULAE cum scriptore (C2: sine 'genus' - insula una). S5a: campus
 * cum referentia systemata="relatio" fert (systema relationum
 * reparat); S5c: aspectus popover systemata="dimissio" (scriptor =
 * aperiens, systema claudit). */
StmlNodus*
organum_dominos_generare (
       constans Organum* organum,
                Piscina* piscina,
    InternamentumChorda* intern);

#endif /* ORGANUM_H */
