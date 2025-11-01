/* xar.h - "Codex Tabulae Exponentialis" - Exponential Array
 *
 * Tabula crescens sine reallocatio - Growing array without reallocation
 *
 * PHILOSOPHIA:
 * - Segmenta duplicant magnitudinem: primi, primi, 2×primi, 4×primi, 8×primi...
 * - Duo prima segmenta eiusdem magnitudinis (optimizatio pro parvis tabulis)
 * - Elementa numquam movent (indices stabiles permanent)
 * - Omnia ex piscina allocata (destructio simplex)
 * - Algorithmus O(1) pro locatio indicis
 *
 * USUS:
 *   Xar* xar = xar_creare(piscina, sizeof(MeusTypus));
 *   MeusTypus* elem = xar_addere(xar);
 *   MeusTypus* get = xar_obtinere(xar, index);
 */

#ifndef XAR_H
#define XAR_H

#include "latina.h"
#include "piscina.h"

/* Constantae */
#define XAR_MAXIMUS_SEGMENTORUM     LXIV    /* 64 segmenta maxima */
#define XAR_PRIMUS_SEGMENTUM        XVI     /* 16 elementa in primo segmento */

/* Vexilla - Flags for Xar behavior */
#define XAR_VEXILLUM_ORDINARIUS     ZEPHYRUM    /* Default: zero memory */
#define XAR_VEXILLUM_SINE_ZEPHYRUM  I           /* Don't zero memory on allocation */

/* XarLocatio - Describes location of element within segmented structure
 * "Locatio elementi in structura segmentata"
 */
nomen structura XarLocatio {
		 i32  index_segmenti;         /* Quis segmentum? - Which segment? */
		 i32  offset_in_segmento;     /* Ubi in segmento? - Where in segment? */
		 i32  magnitudo_segmenti;     /* Quam magnus? - How big? */
	vacuum* basis_segmenti;         /* Principium segmenti - Segment start */
} XarLocatio;

/* XarIterator - For walking through all elements
 * "Iterator pro ambulatione omnium elementorum"
 */
nomen structura XarIterator {
	structura  Xar* xar;              /* Tabula iterata - Array being iterated */
			  i32  index_currens;         /* Index currens - Current index */
				i32  index_segmenti;        /* Segmentum currens - Current segment */
			  i32  offset_in_segmento;    /* Offset in segmento - Offset in segment */
		 vacuum* basis_segmenti;        /* Basis segmenti currentis */
			  i32  finis_segmenti;        /* Finis segmenti currentis */
} XarIterator;

/* XarComparator - Function pointer for comparisons
 * Redde: -1 si a < b, 0 si a == b, +1 si a > b
 */
nomen integer (*XarComparator)(constans vacuum* a, constans vacuum* b);

/* Xar - Main structure
 * "Tabula Exponentialis"
 */
nomen structura Xar {
				i32  numerus_elementorum; /* Quot elementa? - How many elements? */
			  i32  magnitudo_elementi;  /* Quam magnus elementum? - Element size in bytes */
				i32  magnitudo_primi;     /* Magnitudo primi segmenti - First segment size */
			  i32  numerus_segmentorum; /* Quot segmenta allocata? - Allocated segments */
			  i32  capacitas_totalis;   /* Capacitas totalis - Total capacity */
				i32  vexilla;             /* Vexilla - Behavior flags */
	  Piscina* piscina;             /* Piscina allocatio - Memory pool */
	   vacuum* segmenta[XAR_MAXIMUS_SEGMENTORUM];  /* Segmenta - Segment pointers */
	character  titulus[XXXII];        /* Titulus tabulae - Name for debugging */
} Xar;

/* ========================================================================
 * FUNCTIONES PRINCIPALES - Core Functions
 * ======================================================================== */

/* Xar Creare - Create new xar
 * "Genesis tabulae exponentialis"
 */
Xar* 
xar_creare(
		Piscina* piscina,
		 		i32  magnitudo_elementi);

/* Xar Creare Cum Magnitudine - Create with custom first segment size
 * "Creare cum magnitudine prima specifica"
 */
Xar* 
xar_creare_cum_magnitudine(
		Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi);

/* Xar Creare Cum Vexillis - Create with full control
 * "Creare cum omnibus optionibus"
 */
Xar* 
xar_creare_cum_vexillis(
		Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla);

/* Xar Destruere - Destroy xar (no-op with piscina)
 * "Cum piscina, nihil agendum!"
 */
vacuum 
xar_destruere(
		Xar* xar);

/* Xar Ponere Vexilla - Set behavior flags
 * "Ponere vexilla comportamenti"
 */
vacuum 
xar_ponere_vexilla(
		Xar* xar, 
		i32  vexilla);

/* ========================================================================
 * LOCATIO ET ACCESSUS - Location and Access
 * ======================================================================== */

/* Xar Locare - Locate element position (O(1) algorithm)
 * "Invenire locum indicis - algorithmus O(1)"
 *
 * Returns: VERUM si successus, FALSUM si index nimis magnus
 */
b32 
xar_locare(
					 Xar* xar, 
		       i32  index, 
		XarLocatio* locatio);

/* Xar Obtinere - Get element at index
 * "Obtinere elementum ad indicem"
 *
 * Returns: Pointer to element, or NIHIL if invalid
 */
vacuum* 
xar_obtinere(
		Xar* xar, 
		i32  index);

/* Xar Obtinere Vel Creare - Get element or create if needed (sparse arrays)
 * "Obtinere elementum vel creare si necessarium"
 *
 * Useful for sparse arrays - allocates all segments up to index
 */
vacuum* 
xar_obtinere_vel_creare(
		Xar* xar, 
		i32  index);

/* Xar Ponere - Set element at index
 * "Ponere elementum ad indicem"
 *
 * Returns: VERUM si successus, FALSUM si error
 */
b32 
xar_ponere(
								Xar* xar, 
								i32  index, 
		constans vacuum* elementum);

/* ========================================================================
 * ADDITIO - Addition
 * ======================================================================== */

/* Xar Addere - Append new element to end
 * "Addere elementum ad finem"
 *
 * Returns: Pointer to new element, or NIHIL if allocation failed
 */
vacuum* 
xar_addere(
		Xar* xar);

/* Xar Addere Multos - Append multiple elements (bulk operation)
 * "Addere elementa multa - operatio magna"
 *
 * Optimized for bulk inserts - uses block memcpy
 * Returns: Number of elements successfully added
 */
i32 
xar_addere_multos(
								Xar* xar, 
		constans vacuum* elementa, 
								i32  numerus);

/* ========================================================================
 * ITERATIO - Iteration
 * ======================================================================== */

/* Xar Iterator Initium - Initialize iterator
 * "Initium iterationis"
 */
XarIterator 
xar_iterator_initium(
		Xar* xar);

/* Xar Iterator Proximum - Get next element
 * "Obtinere elementum proximum"
 *
 * Returns: Pointer to next element, or NIHIL if done
 */
vacuum* 
xar_iterator_proximum(
		XarIterator* iter);

/* Xar Iterator Finis - Check if iteration is complete
 * "Verificare si iteratio completa"
 *
 * Returns: VERUM si finis, FALSUM si adhuc elementa
 */
b32 
xar_iterator_finis(
		XarIterator* iter);

/* ========================================================================
 * QUAESTIO - Searching
 * ======================================================================== */

/* Xar Invenire - Linear search for element
 * "Quaerere elementum linearie"
 *
 * Returns: Index of element, or -1 if not found
 */
s32 
xar_invenire(
								Xar* xar, 
		constans vacuum* clavis, 
		  XarComparator  comparator);

/* Xar Quaerere Binarie - Binary search (assumes sorted)
 * "Quaerere binarie - assumit ordinatum"
 *
 * Returns: Pointer to element, or NIHIL if not found
 */
vacuum* 
xar_quaerere_binarie(
		 						Xar* xar, 
		constans vacuum* clavis, 
		  XarComparator  comparator);

/* ========================================================================
 * UTILITAS - Utility Functions
 * ======================================================================== */

/* Xar Numerus - Get element count
 * "Obtinere numerum elementorum"
 */
i32 
xar_numerus(
		Xar* xar);

/* Xar Capacitas - Get total capacity
 * "Obtinere capacitatem totalem"
 */
i32 
xar_capacitas(
		Xar* xar);

/* Xar Vacuum Est - Check if empty
 * "Verificare si vacua"
 */
b32 
xar_vacuum_est(
		Xar* xar);

/* Xar Vacare - Clear all elements (keeps segments allocated)
 * "Vacuare omnia elementa"
 */
vacuum 
xar_vacare(
		Xar* xar);

/* Xar Truncare - Truncate to smaller size
 * "Truncare ad magnitudinem minorem"
 */
vacuum 
xar_truncare(
		Xar* xar, 
		i32  numerus_novus);

/* Xar Copiare Ad Tabulam - Copy elements to array
 * "Copiare elementa ad tabulam"
 *
 * Returns: Number of elements copied
 */
i32 
xar_copiare_ad_tabulam(
		   Xar* xar, 
		vacuum* destinatio, 
		   i32  initium, 
		   i32  numerus);

/* Xar Status Imprimere - Print statistics for debugging
 * "Imprimere statisticas pro depuratione"
 */
vacuum 
xar_status_imprimere(
		Xar* xar);

#endif /* XAR_H */
