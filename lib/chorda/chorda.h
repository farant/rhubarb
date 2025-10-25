#ifndef CHORDA_H
#define CHORDA_H

#include "latina.h"

/* ==================================================
 * Creatio
 * ================================================== */

nomen structura chorda {
	memoriae_index  len;
	    				i8* data;
} chorda;

/* Literal constructor - compile-time length calculation */
#define CHORDA(string_literal) \
	(chorda){ magnitudo(string_literal) - I, (i8*)(string_literal) }

/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literis (
		constans i8* cstr);

chorda
chorda_ex_buffer (
		 constans i8* buffer,
	memoriae_index  len);

chorda
chorda_sectio (
						chorda s,
		memoriae_index initium,
		memoriae_index finis);

chorda
chorda_transcribere (
		 chorda  s,
		Piscina* arena);


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
		chorda hay,
		chorda needle);

b32
chorda_incipit (
		chorda s,
		chorda prefixum);

b32
chorda_terminatur (
		chorda s,
		chorda suffixum);

character*
chorda_invenire (
		chorda hay,
		chorda needle);

memoriae_index
chorda_numerare_occurrentia (
		chorda hay,
		chorda needle);


/* ==================================================
 * Manipulatio
 * ================================================= */

chorda 
chorda_praecidere (
		chorda s);

chorda
chorda_minuscula (
		 chorda  s,
		Piscina* arena);

chorda 
chorda_maiuscula (
		 chorda  s,
		Piscina* arena)


/* ==================================================
 * Convenientia
 * ================================================= */

character*
chorda_ut_cstr (
		 chorda  s,
		Piscina* arena);

b32
chorda_ut_integer (
		 chorda  s,
		integer* output);

b32
chorda_ut_size (
						chorda  s,
		memoriae_index* output);


/* ==================================================
 * Hashing
 * ================================================= */

memoriae_index
chorda_hash (
		chorda s);

#endif /* CHORDA_H */
