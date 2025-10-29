#ifndef TABULA_DISPERSA_H
#define TABULA_DISPERSA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ===================================================
 * Tabula Dispersa - Hash Table via Open Addressing
 * "Tabula: flat array, randomly indexed by hash"
 * ================================================== */

/* Status of each slot in the table
 * "Status sloti" */
nomen enumeratio {
    SLOT_VACUUM    = 0, /* Empty slot */
    SLOT_OCCUPATUM = 1, /* Contains a key-value pair */
    SLOT_DELETUM   = 2  /* Deleted (tombstone) */
} SlotusStatus;


/* Hash function type
 * Takes a chorda key, returns i32 hash */
nomen i32 (*TabulaFriatio)(chorda clavis);


/* Comparison function type
 * Returns 0 if equal, <0 if a<b, >0 if a>b */
nomen s32 (*TabulaComparatio)(chorda a, chorda b);

nomen structura {
    SlotusStatus  status;
          chorda  clavis;
          vacuum* valor;
             i32  hash_cachatum;
} Slotus;

/* The hash table itself */
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

/* Iterator for walking all entries */
nomen structura {
    TabulaDispersa* tabula;
               i32  index_currens;
} TabulaIterator;


/* ==================================================
 * Creation
 * ================================================== */

TabulaDispersa*
tabula_dispersa_creare (
             Piscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio);

/* Convenience: create with chorda comparison (standard) */
TabulaDispersa*
tabula_dispersa_creare_chorda (
    Piscina* piscina,
        i32  capacitas_initialis);

/* ==================================================
 * Insertion / Lookup / Deletion
 * ================================================== */

/* Insert key-value pair
 * Returns VERUM on success, FALSUM on allocation failure
 * Updates existing value if key already exists */

b32
tabula_dispersa_inserere (
    TabulaDispersa* tabula,
            chorda  clavis,
            vacuum* valor);


/* Lookup by key
 * Returns VERUM and sets *valor_out if found
 * Returns FALSUM if not found */
b32
tabula_dispersa_invenire (
    TabulaDispersa*  tabula,
            chorda   clavis,
            vacuum** valor_out);

/* Quick existence check */
b32 
tabula_dispersa_continet (
    TabulaDispersa* tabula,
            chorda  clavis);

/* Delete entry by key
 * Returns VERUM if found and deleted, FALSUM otherwise */
b32
tabula_dispersa_delere (
    TabulaDispersa* tabula,
            chorda  clavis);


/* ==================================================
 * Lifecycle
 * ================================================== */

/* Clear table without deallocating (arena-friendly)
 * Resets all slots to VACUUM, counts to zero
 * piscina reset will clean up actual memory */
vacuum
tabula_dispersa_vacare (
    TabulaDispersa* tabula);


/* =================================================
 * Iterator
 * ================================================== */

TabulaIterator
tabula_dispersa_iterator_initium (
    TabulaDispersa* tabula);

/* Get next entry in iter
 * Returns VERUM and fills *clavis_out and *valor_out if entry found
 * Returns FALSUM at end of iteration */

b32
tabula_dispersa_iterator_proximum (
    TabulaIterator*  iter,
            chorda*  clavis_out,
            vacuum** valor_out);

/* =================================================
 * Statistics / Diagnostics
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
 * Hash Functions (in separate library ideally)
 * ================================================== */

i32
tabula_friare_fnv1a (
    chorda clavis);

i32
tabula_friare_multiplicatio (
    chorda clavis);

#endif /* TABULA_DISPERSA_H */

