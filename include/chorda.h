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

/* chorda_fissio_chorda - Dividere chordam per delimitatorem chorda
 * Similis chorda_fissio sed delimitator est chorda, non character
 */
chorda_fissio_fructus
chorda_fissio_chorda (
		chorda   s,
		chorda   delim,
		Piscina* piscina);

/* chorda_iungere - Iungere array chordarum cum separatore
 * elementa: array chordarum
 * numerus: numerus elementorum in array
 * separator: chorda inter elementa (potest esse vacua)
 * Redde: Nova chorda iuncta (allocata ex piscina)
 */
chorda
chorda_iungere (
		chorda*  elementa,
		    i32  numerus,
		 chorda  separator,
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

/* chorda_invenire_index - Invenire positionem acus in fenum
 * Redde: Index (0-based) primae occurrentiae, vel -1 si non inventus
 */
s32
chorda_invenire_index (
		chorda fenum,
		chorda acus);

/* chorda_invenire_ultimum - Invenire ultimam occurrentiam acus in fenum
 * Redde: Vista ad ultimam occurrentiam, vel chorda vacua si non inventus
 */
chorda
chorda_invenire_ultimum (
		chorda fenum,
		chorda acus);

/* chorda_invenire_ultimum_index - Invenire positionem ultimae occurrentiae
 * Redde: Index (0-based) ultimae occurrentiae, vel -1 si non inventus
 */
s32
chorda_invenire_ultimum_index (
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

/* chorda_praecidere_sinistram - Praecidere spatium album a sinistra
 * Redde: Vista (non allocatio) ad chordam sine spatio album initiali
 */
chorda
chorda_praecidere_sinistram (
		chorda s);

/* chorda_praecidere_dextram - Praecidere spatium album a dextra
 * Redde: Vista (non allocatio) ad chordam sine spatio album finali
 */
chorda
chorda_praecidere_dextram (
		chorda s);

/* chorda_substituere - Substituere omnes occurrentias antiqui cum novo
 * Redde: Nova chorda cum substitutionibus (allocata ex piscina)
 */
chorda
chorda_substituere (
		  chorda  s,
		  chorda  antiquum,
		  chorda  novum,
		Piscina* piscina);

/* chorda_invertere - Invertere ordinem characterum
 * Redde: Nova chorda inversa (allocata ex piscina)
 */
chorda
chorda_invertere (
		 chorda  s,
		Piscina* piscina);

/* chorda_duplicare - Repetere chordam n vicibus
 * Redde: Nova chorda cum repetitionibus (allocata ex piscina)
 */
chorda
chorda_duplicare (
		 chorda  s,
		    i32  numerus,
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

b32
chorda_ut_f64 (
		chorda  s,
		   f64* fructus);

/* chorda_ex_s32 - Convertere integrum signatum ad chordam
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_s32 (
		   s32  numerus,
		Piscina* piscina);

/* chorda_ex_f64 - Convertere numerum puncta fluitantia ad chordam
 * praecisio: numerus locorum decimalium
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_f64 (
		   f64  numerus,
		   i32  praecisio,
		Piscina* piscina);

/* chorda_character_ad - Extrahere singularem characterem ut chordam
 * Redde: Chorda longitudinis 1, vel vacua si index extra limites
 */
chorda
chorda_character_ad (
		chorda   s,
		   i32   index,
		Piscina* piscina);

/* chorda_vacua - Verificare si chorda vacua est
 * Redde: VERUM si datum == NIHIL vel mensura == 0
 */
b32
chorda_vacua (
		chorda s);


/* ==================================================
 * Conversio Casus
 * ================================================= */

/* chorda_pascalis - Convertere ad PascalCase
 * "hello world" → "HelloWorld"
 * "getElementById" → "GetElementById"
 */
chorda
chorda_pascalis (
		 chorda  s,
		Piscina* piscina);

/* chorda_camelus - Convertere ad camelCase
 * "hello world" → "helloWorld"
 * "getElementById" → "getElementById"
 */
chorda
chorda_camelus (
		 chorda  s,
		Piscina* piscina);

/* chorda_serpens - Convertere ad snake_case
 * "HelloWorld" → "hello_world"
 * "getElementById" → "get_element_by_id"
 */
chorda
chorda_serpens (
		 chorda  s,
		Piscina* piscina);

/* chorda_kebab - Convertere ad kebab-case
 * "HelloWorld" → "hello-world"
 * "getElementById" → "get-element-by-id"
 */
chorda
chorda_kebab (
		 chorda  s,
		Piscina* piscina);

/* chorda_pascalis_serpens - Convertere ad Pascal_Snake_Case
 * "hello world" → "Hello_World"
 * "getElementById" → "Get_Element_By_Id"
 */
chorda
chorda_pascalis_serpens (
		 chorda  s,
		Piscina* piscina);


/* ==================================================
 * Friatio
 * ================================================= */

i32
chorda_friare (
		chorda s);

#endif /* CHORDA_H */
