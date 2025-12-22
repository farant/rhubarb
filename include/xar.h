/* xar.h - "Codex Tabulae Exponentialis"
 *
 * Tabula crescens sine reallocatio
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

/* Vexilla */
#define XAR_VEXILLUM_ORDINARIUS     ZEPHYRUM    /* Ordinarius: memoria ad zephyrum */
#define XAR_VEXILLUM_SINE_ZEPHYRUM  I           /* Non zephyrum facere memoriam in allocatio */

/* XarIterator - Iterator pro ambulatione omnium elementorum
 */
nomen structura XarIterator {
	constans structura Xar* xar;     /* Tabula iterata */
			               i32  index_currens;         /* Index currens */
				             i32  index_segmenti;        /* Segmentum currens */
			               i32  offset_in_segmento;    /* Offset in segmento */
		            vacuum* basis_segmenti;        /* Basis segmenti currentis */
			               i32  finis_segmenti;        /* Finis segmenti currentis */
} XarIterator;

/* XarComparator - Index functionis pro comparationibus
 * Redde: -1 si a < b, 0 si a == b, +1 si a > b
 */
nomen s32 (*XarComparator)(constans vacuum* a, constans vacuum* b);

/* Xar - Structura principalis
 * "Tabula Exponentialis"
 */
nomen structura Xar {
				i32  numerus_elementorum; /* Quot elementa? */
			  i32  magnitudo_elementi;  /* Quam magnus elementum? (in bytes) */
				i32  magnitudo_primi;     /* Magnitudo primi segmenti */
			  i32  numerus_segmentorum; /* Quot segmenta allocata? */
			  i32  capacitas_totalis;   /* Capacitas totalis */
				i32  vexilla;             /* Signa comportamenti */
	  Piscina* piscina;             /* Piscina memoriae */
	   vacuum* segmenta[XAR_MAXIMUS_SEGMENTORUM];  /* Indices segmentorum */
	character  titulus[XXXII];        /* Nomen pro depuratione */
} Xar;

/* ========================================================================
 * FUNCTIONES PRINCIPALES
 * ======================================================================== */

/* Xar Creare - Creare novum xar
 * "Genesis tabulae exponentialis"
 */
Xar*
xar_creare(
		Piscina* piscina,
		 		i32  magnitudo_elementi);

/* Xar Creare Cum Magnitudine
 * "Creare cum magnitudine prima specifica"
 */
Xar*
xar_creare_cum_magnitudine(
		Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi);

/* Xar Creare Cum Vexillis
 * "Creare cum omnibus optionibus"
 */
Xar*
xar_creare_cum_vexillis(
		Piscina* piscina,
        i32  magnitudo_elementi,
        i32  magnitudo_primi,
        i32  vexilla);

/* Xar Destruere
 * "Cum piscina, nihil agendum!"
 */
vacuum
xar_destruere(
		Xar* xar);

/* Xar Ponere Vexilla
 * "Ponere vexilla comportamenti"
 */
vacuum
xar_ponere_vexilla(
		Xar* xar,
		i32  vexilla);

/* ========================================================================
 * LOCATIO ET ACCESSUS
 * ======================================================================== */

/* Xar Obtinere - Obtinere elementum ad indicem
 * "Obtinere elementum ad indicem"
 *
 * Redde: Index ad elementum, vel NIHIL si invalidus
 */
vacuum*
xar_obtinere(
		constans Xar* xar,
		         i32  index);

/* Xar Obtinere Signatum - Obtinere cum indice signato
 * "Pro iterationibus cum decrementis"
 *
 * Redde: Index ad elementum, vel NIHIL si invalidus vel negativus
 */
vacuum*
xar_obtinere_s(
		constans Xar* xar,
		         s32  index);

/* Xar Obtinere Vel Creare - Obtinere elementum vel creare si necessarium
 * "Obtinere elementum vel creare si necessarium"
 *
 * Utile pro tabulis dispersis - allocat omnia segmenta usque ad indicem
 */
vacuum*
xar_obtinere_vel_creare(
		Xar* xar,
		i32  index);

/* Xar Ponere - Ponere elementum ad indicem
 * "Ponere elementum ad indicem"
 *
 * Redde: VERUM si successus, FALSUM si error
 */
b32
xar_ponere(
								Xar* xar,
								i32  index,
		constans vacuum* elementum);

/* ========================================================================
 * ADDITIO
 * ======================================================================== */

/* Xar Addere - Addere elementum ad finem
 * "Addere elementum ad finem"
 *
 * Redde: Index ad elementum novum, vel NIHIL si allocatio fracta
 */
vacuum*
xar_addere(
		Xar* xar);

/* Xar Addere Multos - Addere elementa multa
 * "Addere elementa multa - operatio moles"
 *
 * Optimizatum pro insertionibus magnis - usat memcpy per blocos
 * Redde: Numerus elementorum cum successu additorum
 */
i32
xar_addere_multos(
								Xar* xar,
		constans vacuum* elementa,
								i32  numerus);

/* ========================================================================
 * ITERATIO
 * ======================================================================== */

/* Xar Iterator Initium - Initializare iteratorem
 * "Initium iterationis"
 */
XarIterator
xar_iterator_initium(
		constans Xar* xar);

/* Xar Iterator Proximum - Obtinere elementum proximum
 * "Obtinere elementum proximum"
 *
 * Redde: Index ad elementum proximum, vel NIHIL si completus
 */
vacuum*
xar_iterator_proximum(
		XarIterator* iter);

/* Xar Iterator Finis - Verificare si iteratio completa
 * "Verificare si iteratio completa"
 *
 * Redde: VERUM si finis, FALSUM si adhuc elementa
 */
b32
xar_iterator_finis(
		XarIterator* iter);

/* ========================================================================
 * QUAESTIO
 * ======================================================================== */

/* Xar Invenire - Quaerere elementum linearie
 * "Quaerere elementum linearie"
 *
 * Redde: Index ad elementum, vel NIHIL si non inventus
 */
vacuum*
xar_invenire(
		constans         Xar* xar,
		constans      vacuum* clavis,
		          XarComparator  comparator);

/* Xar Invenire Index - Quaerere elementum linearie, reddere indicem
 * "Quaerere elementum linearie, reddere indicem"
 *
 * Redde: Index elementi, vel -1 si non inventus
 */
s32
xar_invenire_index(
		constans         Xar* xar,
		constans      vacuum* clavis,
		          XarComparator  comparator);

/* Xar Quaerere Binarie - Quaerere binarie (assumit ordinatum)
 * "Quaerere binarie - assumit ordinatum"
 *
 * Redde: Index ad elementum, vel NIHIL si non inventus
 */
vacuum*
xar_quaerere_binarie(
		constans         Xar* xar,
		constans      vacuum* clavis,
		          XarComparator  comparator);

/* Xar Quaerere Binarie Index - Quaerere binarie, reddere indicem
 * "Quaerere binarie, reddere indicem"
 *
 * Redde: Index elementi, vel -1 si non inventus
 */
s32
xar_quaerere_binarie_index(
		constans         Xar* xar,
		constans      vacuum* clavis,
		          XarComparator  comparator);

/* ========================================================================
 * UTILITAS
 * ======================================================================== */

/* Xar Numerus - Obtinere numerum elementorum
 * "Obtinere numerum elementorum"
 */
i32
xar_numerus(
		constans Xar* xar);

/* Xar Capacitas - Obtinere capacitatem totalem
 * "Obtinere capacitatem totalem"
 */
i32
xar_capacitas(
		constans Xar* xar);

/* Xar Vacuum Est - Verificare si vacua
 * "Verificare si vacua"
 */
b32
xar_vacuum_est(
		constans Xar* xar);

/* Xar Vacare - Vacuare omnia elementa
 * "Vacuare omnia elementa (segmenta manent allocata)"
 */
vacuum
xar_vacare(
		Xar* xar);

/* Xar Truncare - Truncare ad magnitudinem minorem
 * "Truncare ad magnitudinem minorem"
 */
vacuum
xar_truncare(
		Xar* xar,
		i32  numerus_novus);

/* Xar Copiare Ad Tabulam - Copiare elementa ad tabulam
 * "Copiare elementa ad tabulam"
 *
 * Redde: Numerus elementorum copiatorum
 */
i32
xar_copiare_ad_tabulam(
		constans Xar* xar,
		      vacuum* destinatio,
		         i32  initium,
		         i32  numerus);

/* Xar Status Imprimere - Imprimere statisticas pro depuratione
 * "Imprimere statisticas pro depuratione"
 */
vacuum
xar_status_imprimere(
		constans Xar* xar);


/* ========================================================================
 * REMOTIO
 * ======================================================================== */

/* Xar Removere Ultimum - Removere elementum ultimum
 * "Removere ultimum - O(1)"
 *
 * Redde: VERUM si successus, FALSUM si xar vacuum
 */
b32
xar_removere_ultimum(
		Xar* xar);

/* Xar Removere Cum Ultimo - Swap-remove O(1)
 * "Removere elementum ad indicem mutando cum ultimo"
 *
 * Nota: Non servat ordinem! Utile si ordo non refert.
 * Redde: VERUM si successus, FALSUM si index invalidus
 */
b32
xar_removere_cum_ultimo(
		Xar* xar,
		i32  index);

/* Xar Tollere - Pop ultimum elementum (alias removere_ultimum + obtinere)
 * "Tollere ultimum elementum"
 *
 * Copiare ultimum elementum ad destinationem et removere
 * Redde: VERUM si successus, FALSUM si xar vacuum
 */
b32
xar_tollere(
		    Xar* xar,
		 vacuum* destinatio);


/* ========================================================================
 * ORDINATIO ET MANIPULATIO
 * ======================================================================== */

/* Xar Ordinare - Ordinare in loco
 * "Ordinare tabulam in loco usans comparatorem"
 *
 * Usans selection sort (simplex, O(n²), sed stabilis pro parvis tabulis)
 */
vacuum
xar_ordinare(
		          Xar* xar,
		XarComparator  comparator);

/* Xar Invertere - Invertere ordinem in loco
 * "Invertere ordinem elementorum"
 */
vacuum
xar_invertere(
		Xar* xar);

/* Xar Mutare - Mutare duo elementa
 * "Mutare elementa ad indices"
 *
 * Redde: VERUM si successus, FALSUM si indices invalidi
 */
b32
xar_mutare(
		Xar* xar,
		i32  index_a,
		i32  index_b);

/* Xar Continet - Verificare si elementum exsistit
 * "Verificare si elementum exsistit"
 *
 * Redde: VERUM si inventus, FALSUM si non
 */
b32
xar_continet(
		constans         Xar* xar,
		constans      vacuum* clavis,
		          XarComparator  comparator);

#endif /* XAR_H */
