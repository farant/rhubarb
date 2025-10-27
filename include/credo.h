/* credo.h */
#ifndef CREDO_H
#define CREDO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ============================================
 * Creatio Recodationis
 * ============================================ */

nomen structura CredoNotatio {
									chorda  genus;
									chorda  expressio;
		 							chorda  valor_primus;
		 						  chorda  valor_secundus;
							 character* filum;
			  						 s32  versus;
										 b32  praeteritus;
				  memoriae_index  ordo;
	structura CredoNotatio* sequens;
} CredoNotatio;


/* ============================================
 * Globalis Apparatus
 * ============================================ */

vacuum
credo_aperire (
		Piscina* piscina);

vacuum
credo_claudere (
		vacuum);


/* ============================================
 * Interrogatio
 * ============================================ */

memoriae_index
credo_numerus_totalis (
		vacuum);

memoriae_index
credo_numerus_praeteriti (
		vacuum);

memoriae_index
credo_numerus_fracti (
		vacuum);
	
b32
credo_omnia_praeterierunt (
		vacuum);

vacuum
credo_imprimere_compendium (
		vacuum);

CredoNotatio*
credo_invenire_fractos (
		memoriae_index* numerus_fructus);


/* ============================================
 * Macros - Nucleum
 * ============================================ */

#define CREDO_VERUM(x) \
	_credo_notare( \
			"credo_verum", \
			#x, \
			chorda_ex_literis(#x), \
			chorda_ex_literis(""), \
			__FILE__, \
			__LINE__, \
			(x) != FALSUM \
	)

#define CREDO_FALSUM(x) \
	_credo_notare( \
			"credo_falsum", \
			#x, \
			chorda_ex_literis(#x), \
			chorda_ex_literis(""), \
			__FILE__, \
			__LINE__, \
			(x) == FALSUM \
	)

#define CREDO_NON_NIHIL(p) \
	_credo_notare( \
			"credo_non_nihil", \
			#p, \
			chorda_ex_literis(#p), \
			chorda_ex_literis(""), \
			__FILE__, \
			__LINE__, \
			(p) != NIHIL \
	)

#define CREDO_NIHIL(p) \
	_credo_notare( \
			"credo_nihil", \
			#p, \
			chorda_ex_literis(#p), \
			chorda_ex_literis(""), \
			__FILE__, \
			__LINE__, \
			(p) == NIHIL \
	)

#define CREDO_AEQUALIS(a, b) \
	_credo_notare_integrum( \
			"credo_aequalis", \
			#a " == " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_INAEQUALITAS(a, b) \
	_credo_notare_integrum( \
			"credo_inaequalitas", \
			#a " != " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_CHORDAE_AEQUALES(s1, s2) \
	_credo_notare_chorda( \
			"credo_chordae_aequales", \
			#s1 " == " #s2, \
			(s1), \
			(s2), \
			__FILE__, \
			__LINE__ \
	)

/* ============================================
 * Adiutores Interni
 * ============================================ */

vacuum 
_credo_notare (
		character* genus,
		character* expressio,
			 chorda  valor_primus,
			 chorda  valor_secundus,
		character* filum,
					s32  versus,
					b32  praeteritus);


vacuum
_credo_notare_integrum (
		character* genus,
		character* expressio,
					s32  valor_primus,
					s32  valor_secundus,
		character* filum,
					s32  versus);

vacuum
_credo_notare_chorda (
		character* genus,
		character* expressio,
			 chorda  valor_primus,
			 chorda  valor_secundus,
		character* filum,
					s32  versus);

#endif /* CREDO_H */
