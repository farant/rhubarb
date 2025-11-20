#ifndef TABULA_DISPERSA_H
#define TABULA_DISPERSA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ===================================================
 * Tabula Dispersa - Tabula Friationis per Addressing Apertum
 * "Tabula: series plana, per friationes indexata"
 * ================================================== */

/* Status cuiusque sloti in tabula
 * "Status sloti" */
nomen enumeratio {
    SLOT_VACUUM    = 0, /* Slotus vacuus */
    SLOT_OCCUPATUM = 1, /* Clavis-valor par continens */
    SLOT_DELETUM   = 2  /* Deletum (tumulus) */
} SlotusStatus;


/* Genus functionis friationis
 * Clavis chordae capit, friationes i32 reddit */
nomen i32 (*TabulaFriatio)(chorda clavis);


/* Genus functionis comparationis
 * 0 reddit si aequalis, <0 si a<b, >0 si a>b */
nomen s32 (*TabulaComparatio)(chorda a, chorda b);

nomen structura {
    SlotusStatus  status;
          chorda  clavis;
          vacuum* valor;
             i32  hash_cachatum;
} Slotus;

/* Ipsa tabula friationis */
nomen structura {
              Slotus* sloti;
                 i32  capacitas;
                 i32  capacitas_mask;
                 i32  numerus;
                 i32  numerus_deletorum;
             Piscina* piscina;
       TabulaFriatio  friatio;
    TabulaComparatio  comparatio;
                 f32  factor_maximus;
                 f32  factor_deletorum_maximus;
                 i32  collisiones_totales;
                 i32  distantia_maxima;
} TabulaDispersa;

/* Iterator ad omnes introitus ambulandos */
nomen structura {
    TabulaDispersa* tabula;
               i32  positus_currens;
} TabulaIterator;


/* ==================================================
 * Creatio
 * ================================================== */

TabulaDispersa*
tabula_dispersa_creare (
             Piscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio);

/* Commoditas: crea cum comparatione chordae (signum) */
TabulaDispersa*
tabula_dispersa_creare_chorda (
    Piscina* piscina,
        i32  capacitas_initialis);

/* ==================================================
 * Insertio / Quaestio / Deletio
 * ================================================== */

/* Clavis-valor par inserere
 * VERUM reddit in successu, FALSUM in allocationis fractura
 * Valor existens renovat si clavis iam existit */

b32
tabula_dispersa_inserere (
    TabulaDispersa* tabula,
            chorda  clavis,
            vacuum* valor);


/* Clavis per quaestio
 * VERUM reddit et *valor_exitus statuit si inventum
 * FALSUM reddit si non inventum */
b32
tabula_dispersa_invenire (
    TabulaDispersa*  tabula,
            chorda   clavis,
            vacuum** valor_out);

/* Celeris existentiae verificatio */
b32
tabula_dispersa_continet (
    TabulaDispersa* tabula,
            chorda  clavis);

/* Convenientia: quaestio per literis C directe
 * VERUM reddit et *valor_exitus statuit si inventum
 * FALSUM reddit si non inventum */
b32
tabula_dispersa_invenire_literis (
    TabulaDispersa*        tabula,
    constans character*    cstr,
            vacuum**       valor_out);

/* Convenientia: verificatio existentiae per literis C */
b32
tabula_dispersa_continet_literis (
    TabulaDispersa*     tabula,
    constans character* cstr);

/* Introitum delere per clavis
 * VERUM reddit si inventum et deletum, FALSUM alioquin */
b32
tabula_dispersa_delere (
    TabulaDispersa* tabula,
            chorda  clavis);


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

/* Tabulam vaccare sine deallocatione (arena-amica)
 * Omnes sloti ad VACUUM reficit, computationes ad nihil
 * piscina refectio memoriam veram purificabit */
vacuum
tabula_dispersa_vacare (
    TabulaDispersa* tabula);


/* =================================================
 * Iterator
 * ================================================== */

TabulaIterator
tabula_dispersa_iterator_initium (
    TabulaDispersa* tabula);

/* Sequens introitum in iteratore capere
 * VERUM reddit et *clavis_exitus et *valor_exitus implet si introitus inventus
 * FALSUM reddit in fine iterationis */

b32
tabula_dispersa_iterator_proximum (
    TabulaIterator*  iter,
            chorda*  clavis_out,
            vacuum** valor_out);

/* =================================================
 * Statisticae / Diagnostica
 * ================================================== */

f32
tabula_dispersa_factor_oneris (
    TabulaDispersa* tabula);

f32
tabula_dispersa_factor_deletorum (
    TabulaDispersa* tabula);

i32
tabula_dispersa_numerus(
    TabulaDispersa* tabula);

vacuum
tabula_dispersa_status_imprimere(
    TabulaDispersa* tabula);


/* =================================================
 * Functiones Friationis (in bibliotheca separata ideale)
 * ================================================== */

i32
tabula_friare_fnv1a (
    chorda clavis);

i32
tabula_friare_multiplicatio (
    chorda clavis);

#endif /* TABULA_DISPERSA_H */

