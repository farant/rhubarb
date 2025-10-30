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
									/* NOTA: Fortasse oportet chorda esse */
			 constans character* filum;
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
			#x, \
			"", \
			__FILE__, \
			__LINE__, \
			(x) != FALSUM \
	)

#define CREDO_FALSUM(x) \
	_credo_notare( \
			"credo_falsum", \
			#x, \
			#x, \
			"", \
			__FILE__, \
			__LINE__, \
			(x) == FALSUM \
	)

#define CREDO_NON_NIHIL(p) \
	_credo_notare( \
			"credo_non_nihil", \
			#p, \
			#p, \
			"", \
			__FILE__, \
			__LINE__, \
			(p) != NIHIL \
	)

#define CREDO_NIHIL(p) \
	_credo_notare( \
			"credo_nihil", \
			#p, \
			#p, \
			"", \
			__FILE__, \
			__LINE__, \
			(p) == NIHIL \
	)

#define CREDO_AEQUALIS_S32(a, b) \
	_credo_notare_integrum( \
			"credo_aequalis", \
			#a " == " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_AEQUALIS_I32(a, b) \
	_credo_notare_integrum_i32( \
			"credo_aequalis", \
			#a " == " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_INAEQUALITAS_S32(a, b) \
	_credo_notare_integrum_inaequalitas( \
			"credo_inaequalitas", \
			#a " != " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_INAEQUALITAS_I32(a, b) \
	_credo_notare_integrum_inaequalitas_i32( \
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
		constans character* genus,
		constans character* expressio,
		constans character* valor_primus,
		constans character* valor_secundus,
		constans character* filum,
									 s32  versus,
									 b32  praeteritus);


vacuum
_credo_notare_integrum (
		constans character* genus,
		constans character* expressio,
									 s32  valor_primus,
									 s32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_i32 (
		constans character* genus,
		constans character* expressio,
									 i32  valor_primus,
									 i32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_inaequalitas (
		constans character* genus,
		constans character* expressio,
									 s32  valor_primus,
									 s32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_inaequalitas_i32 (
		constans character* genus,
		constans character* expressio,
									 i32  valor_primus,
									 i32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_chorda (
		constans character* genus,
		constans character* expressio,
		constans character* valor_primus,
		constans character* valor_secundus,
		constans character* filum,
									 s32  versus);

#endif /* CREDO_H */
