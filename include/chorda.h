#ifndef CHORDA_H
#define CHORDA_H

#include "latina.h"
#include "piscina.h"

/* ==================================================
 * Creatio
 * ================================================== */

nomen structura chorda {
	i32  mensura;
	 i8* datum;
} chorda;


/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literis (
		constans character* cstr,
							 Piscina* piscina);

chorda
chorda_ex_buffer (
	 i8* buffer,
	i32  mensura);

chorda
chorda_sectio (
		chorda s,
			 i32 initium,
			 i32 finis);

chorda
chorda_transcribere (
		 chorda  s,
		Piscina* piscina);

chorda
chorda_concatenare (
		chorda  a,
		chorda  b,
		Piscina* piscina);

chorda
chorda_praecidi_laterale (
		 chorda  s,
		Piscina* piscina);


/* ==================================================
 * Divisio
 * ================================================= */

nomen structura {
	chorda* elementa;
	i32     numerus;
} chorda_fissio_fructus;

chorda_fissio_fructus
chorda_fissio (
		chorda  s,
		character delim,
		Piscina* piscina);


/* ==================================================
 * Comparatio
 * ================================================= */

b32
chorda_aequalis (
		chorda a,
		chorda b);

b32
chorda_aequalis_literis (
		chorda              s,
		constans character* cstr);

b32
chorda_aequalis_case_insensitivus (
		chorda a,
		chorda b);

s32
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

i32
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
chorda_ut_s32 (
		chorda  s,
		   s32* fructus);

b32
chorda_ut_i32 (
		chorda  s,
			 i32* fructus);


/* ==================================================
 * Friatio
 * ================================================= */

i32
chorda_friare (
		chorda s);

#endif /* CHORDA_H */
