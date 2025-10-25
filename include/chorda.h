#ifndef CHORDA_H
#define CHORDA_H

#include "latina.h"
#include "piscina.h"

/* ==================================================
 * Creatio
 * ================================================== */

nomen structura chorda {
	memoriae_index  mensura;
	    				i8* datum;
} chorda;


/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literis (
		character* cstr);

chorda
chorda_ex_buffer (
		 					i8* buffer,
	memoriae_index  mensura);

chorda
chorda_sectio (
						chorda s,
		memoriae_index initium,
		memoriae_index finis);

chorda
chorda_transcribere (
		 chorda  s,
		Piscina* piscina);


/* ==================================================
 * Comparatio
 * ================================================= */

b32
chorda_aequalis (
		chorda a,
		chorda b);

b32
chorda_aequalis_case_insensitivus (
		chorda a,
		chorda b);

integer
chorda_comparare (
		chorda a,
		chorda b);


/* ==================================================
 * Quaestio
 * ================================================= */

b32
chorda_continet (
		chorda fenum,
		chorda acus);

b32
chorda_incipit (
		chorda s,
		chorda prefixum);

b32
chorda_terminatur (
		chorda s,
		chorda suffixum);

chorda
chorda_invenire (
		chorda fenum,
		chorda acus);

memoriae_index
chorda_numerare_occurrentia (
		chorda fenum,
		chorda acus);


/* ==================================================
 * Manipulatio
 * ================================================= */

chorda 
chorda_praecidere (
		chorda s);

chorda
chorda_minuscula (
		 chorda  s,
		Piscina* piscina);

chorda 
chorda_maiuscula (
		 chorda  s,
		Piscina* piscina);


/* ==================================================
 * Convenientia
 * ================================================= */

character*
chorda_ut_cstr (
		 chorda  s,
		Piscina* piscina);

b32
chorda_ut_integer (
		 chorda  s,
		integer* fructus);

b32
chorda_ut_size (
						chorda  s,
		memoriae_index* fructus);


/* ==================================================
 * Hashing
 * ================================================= */

memoriae_index
chorda_friare (
		chorda s);

#endif /* CHORDA_H */
