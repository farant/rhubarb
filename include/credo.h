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
									chorda  filum;
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

#define CREDO_AEQUALIS_PTR(a, b) \
	_credo_notare( \
			"credo_aequalis_ptr", \
			#a " == " #b, \
			#a, \
			#b, \
			__FILE__, \
			__LINE__, \
			(a) == (b) \
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
 * Macros - Chorda Convenientiae
 * ============================================ */

#define CREDO_CHORDA_AEQUALIS(a, b) \
	CREDO_VERUM(chorda_aequalis((a), (b)))

#define CREDO_CHORDA_CONTINET(fenum, acus) \
	CREDO_VERUM(chorda_continet((fenum), (acus)))

#define CREDO_CHORDA_INCIPIT(s, prefixum) \
	CREDO_VERUM(chorda_incipit((s), (prefixum)))

#define CREDO_CHORDA_TERMINATUR(s, suffixum) \
	CREDO_VERUM(chorda_terminatur((s), (suffixum)))


/* ============================================
 * Macros - Comparationes Integrae
 * ============================================ */

#define CREDO_MAIOR_S32(a, b) \
	_credo_notare_integrum_maior( \
			"credo_maior", \
			#a " > " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_MAIOR_I32(a, b) \
	_credo_notare_integrum_maior_i32( \
			"credo_maior", \
			#a " > " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_MINOR_S32(a, b) \
	_credo_notare_integrum_minor( \
			"credo_minor", \
			#a " < " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_MINOR_I32(a, b) \
	_credo_notare_integrum_minor_i32( \
			"credo_minor", \
			#a " < " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_MAIOR_AUT_AEQUALIS_S32(a, b) \
	_credo_notare_integrum_maior_aut_aequalis( \
			"credo_maior_aut_aequalis", \
			#a " >= " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_MAIOR_AUT_AEQUALIS_I32(a, b) \
	_credo_notare_integrum_maior_aut_aequalis_i32( \
			"credo_maior_aut_aequalis", \
			#a " >= " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_MINOR_AUT_AEQUALIS_S32(a, b) \
	_credo_notare_integrum_minor_aut_aequalis( \
			"credo_minor_aut_aequalis", \
			#a " <= " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)

#define CREDO_MINOR_AUT_AEQUALIS_I32(a, b) \
	_credo_notare_integrum_minor_aut_aequalis_i32( \
			"credo_minor_aut_aequalis", \
			#a " <= " #b, \
			(a), \
			(b), \
			__FILE__, \
			__LINE__ \
	)


/* ============================================
 * Macros - Comparationes Fluitantes
 * ============================================ */

#define CREDO_F64_PROXIMUS(a, b, epsilon) \
	_credo_notare_f64_proximus( \
			"credo_f64_proximus", \
			#a " ≈ " #b, \
			(a), \
			(b), \
			(epsilon), \
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

vacuum
_credo_notare_integrum_maior (
		constans character* genus,
		constans character* expressio,
									 s32  valor_primus,
									 s32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_maior_i32 (
		constans character* genus,
		constans character* expressio,
									 i32  valor_primus,
									 i32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_minor (
		constans character* genus,
		constans character* expressio,
									 s32  valor_primus,
									 s32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_minor_i32 (
		constans character* genus,
		constans character* expressio,
									 i32  valor_primus,
									 i32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_maior_aut_aequalis (
		constans character* genus,
		constans character* expressio,
									 s32  valor_primus,
									 s32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_maior_aut_aequalis_i32 (
		constans character* genus,
		constans character* expressio,
									 i32  valor_primus,
									 i32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_minor_aut_aequalis (
		constans character* genus,
		constans character* expressio,
									 s32  valor_primus,
									 s32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_integrum_minor_aut_aequalis_i32 (
		constans character* genus,
		constans character* expressio,
									 i32  valor_primus,
									 i32  valor_secundus,
		constans character* filum,
									 s32  versus);

vacuum
_credo_notare_f64_proximus (
		constans character* genus,
		constans character* expressio,
									 f64  valor_primus,
									 f64  valor_secundus,
									 f64  epsilon,
		constans character* filum,
									 s32  versus);

#endif /* CREDO_H */
