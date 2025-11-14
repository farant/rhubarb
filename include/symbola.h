#ifndef SYMBOLA_H
#define SYMBOLA_H

#include "latina.h"
#include "chorda.h"
#include "macho.h"
#include "piscina.h"

/* ==================================================
 * SYMBOLA - Extractio Tabulae Symbolorum ex Mach-O
 *
 * Bibliotheca pro parsendo symbola ex LC_SYMTAB.
 * Sustinet symbola localia, globalia, externa, et debug.
 *
 * PHILOSOPHIA:
 * - Zero-copy (nomina punctant in string table)
 * - Iterator pattern (comprobatum in macho.h et sectio.h)
 * - Omnia ex piscina allocata
 * - Symbola ordinata per indice in tabula originali
 *
 * EXEMPLUM SIMPLEX:
 *   Piscina* p = piscina_generare_dynamicum("symbola", 4*1024*1024);
 *   MachO* macho = macho_aperire("/bin/ls", p);
 *
 *   TabulaSymbolorum* tabula = tabula_symbolorum_ex_macho(macho, p);
 *   si (tabula == NIHIL)
 *   {
 *       imprimere("Error: %s\n", symbola_error_recens());
 *       piscina_destruere(p);
 *       redde I;
 *   }
 *
 *   -- Iterare omnia symbola
 *   SymbolorumIterator iter = symbolorum_iterator_initium(tabula);
 *   Symbolum* sym;
 *   dum ((sym = symbolorum_iterator_proximum(&iter)) != NIHIL)
 *   {
 *       chorda nomen = symbolum_nomen(sym, p);
 *       si (symbolum_est_functio(sym))
 *       {
 *           imprimere("Function: %.*s at 0x%llx\n",
 *                     nomen.mensura, nomen.datum,
 *                     symbolum_addressa(sym));
 *       }
 *   }
 *
 *   -- Invenire symbolum specificum
 *   Symbolum* main_sym = symbolum_invenire(tabula, "_main", p);
 *   si (main_sym != NIHIL)
 *   {
 *       imprimere("_main at 0x%llx\n", symbolum_addressa(main_sym));
 *   }
 *
 *   piscina_destruere(p);
 *
 * ================================================== */


/* ==================================================
 * Typi Opaci
 * ================================================== */

/* TabulaSymbolorum - Tabula symbolorum ex LC_SYMTAB */
nomen structura TabulaSymbolorum TabulaSymbolorum;

/* Symbolum - Symbolum singulum ex tabula */
nomen structura Symbolum Symbolum;


/* ==================================================
 * Constantae - Typi Symbolorum (n_type)
 * ================================================== */

/* Type mask et flags (ex mach-o/nlist.h) */
#define SYMBOLUM_TYPE_UNDEF       0x0   /* N_UNDF - Undefined, externum */
#define SYMBOLUM_TYPE_ABS         0x2   /* N_ABS - Absolute, n_sect == NO_SECT */
#define SYMBOLUM_TYPE_SECT        0xe   /* N_SECT - Defined in sectione n_sect */
#define SYMBOLUM_TYPE_PBUD        0xc   /* N_PBUD - Prebound undefined */
#define SYMBOLUM_TYPE_INDR        0xa   /* N_INDR - Indirect */

#define SYMBOLUM_TYPE_MASK        0x0e  /* N_TYPE - Mask pro typum */

/* Scope flags */
#define SYMBOLUM_EXT              0x01  /* N_EXT - External symbol */
#define SYMBOLUM_PEXT             0x10  /* N_PEXT - Private external */

/* Stab flags (debug symbols) */
#define SYMBOLUM_STAB             0xe0  /* N_STAB - Mask pro debug */


/* ==================================================
 * Constantae - Reference Flags (n_desc)
 * ================================================== */

#define SYMBOLUM_WEAK_REF         0x0040  /* REFERENCED_DYNAMICALLY */
#define SYMBOLUM_WEAK_DEF         0x0080  /* N_WEAK_DEF */
#define SYMBOLUM_NO_DEAD_STRIP    0x0020  /* N_NO_DEAD_STRIP */


/* ==================================================
 * Creatio - Tabula Symbolorum
 * ================================================== */

/* Creare tabulam symbolorum ex Mach-O
 *
 * Extrahit LC_SYMTAB ex macho et creat tabulam.
 * Omnia symbola (localia, globalia, externa, debug) inclusa.
 *
 * macho:   imago Mach-O
 * piscina: piscina pro allocationibus
 *
 * Reddit: TabulaSymbolorum* vel NIHIL si error
 *
 * NOTA: Requirit LC_SYMTAB mandatum in macho.
 *       Si nullum LC_SYMTAB, reddit NIHIL.
 */
TabulaSymbolorum*
tabula_symbolorum_ex_macho(
	constans MachO* macho,
	       Piscina* piscina);


/* ==================================================
 * Interrogatio - Tabula
 * ================================================== */

/* Obtinere numerum symbolorum totalem
 *
 * tabula: tabula symbolorum
 *
 * Reddit: numerus symbolorum in tabula
 */
i32
tabula_symbolorum_numerus(
	constans TabulaSymbolorum* tabula);

/* Obtinere symbolum per indicem
 *
 * tabula: tabula symbolorum
 * index:  index in tabula (0-based)
 *
 * Reddit: Symbolum* vel NIHIL si index invalidus
 *
 * NOTA: Symbola ordinata per indicem originalem in binario
 */
Symbolum*
tabula_symbolorum_symbolum(
	constans TabulaSymbolorum* tabula,
	                      i32  index);


/* ==================================================
 * Iteratio - Omnia Symbola
 * ================================================== */

/* Iterator pro ambulatione omnium symbolorum */
nomen structura SymbolorumIterator {
	constans TabulaSymbolorum* tabula;
	                      i32  index_currens;
} SymbolorumIterator;

/* Initializare iteratorem
 *
 * tabula: tabula symbolorum
 *
 * Reddit: iterator ad initium
 */
SymbolorumIterator
symbolorum_iterator_initium(
	constans TabulaSymbolorum* tabula);

/* Obtinere symbolum proximum
 *
 * iter: iterator
 *
 * Reddit: symbolum proximum, vel NIHIL si finis
 *
 * EXEMPLUM:
 *   SymbolorumIterator iter = symbolorum_iterator_initium(tabula);
 *   Symbolum* sym;
 *   dum ((sym = symbolorum_iterator_proximum(&iter)) != NIHIL)
 *   {
 *       -- processa symbolum
 *   }
 */
Symbolum*
symbolorum_iterator_proximum(
	SymbolorumIterator* iter);

/* Verificare si iteratio completa
 *
 * iter: iterator
 *
 * Reddit: VERUM si finis, FALSUM alioquin
 */
b32
symbolorum_iterator_finis(
	SymbolorumIterator* iter);


/* ==================================================
 * Quaestio - Invenire Symbola
 * ================================================== */

/* Invenire symbolum per nomen
 *
 * Quaerit symbolum cum nomine exacto.
 * Si plura symbola cum eodem nomine, reddit primum inventum.
 *
 * tabula:  tabula symbolorum
 * nomen:   nomen symboli quaerendi (e.g., "_main", "_printf")
 * piscina: piscina pro chorda temporaria (si necessaria)
 *
 * Reddit: Symbolum* vel NIHIL si non inventum
 *
 * NOTA: Quaestio linearis - O(n) in numero symbolorum
 *
 * EXEMPLA:
 *   Symbolum* main = symbolum_invenire(tabula, "_main", p);
 *   Symbolum* printf = symbolum_invenire(tabula, "_printf", p);
 */
Symbolum*
symbolum_invenire(
	constans TabulaSymbolorum* tabula,
	constans character* appellatio,
	       Piscina* piscina);

/* Invenire symbolum per addressam
 *
 * Quaerit symbolum cuius addressa == addressa data.
 * Utile pro conversione addressae ad nomen.
 *
 * tabula:   tabula symbolorum
 * addressa: addressa virtualis quaerenda
 * piscina:  piscina (non usata nunc, pro futuro)
 *
 * Reddit: Symbolum* vel NIHIL si non inventum
 *
 * NOTA: Quaestio linearis - O(n)
 *       Non invenit symbola "proxima" - requirit aequalitatem exactam
 */
Symbolum*
symbolum_invenire_per_addressa(
	constans TabulaSymbolorum* tabula,
	           memoriae_index  addressa,
	                  Piscina* piscina);


/* ==================================================
 * Interrogatio - Attributa Symboli
 * ================================================== */

/* Obtinere nomen symboli
 *
 * symbolum: symbolum
 * piscina:  piscina pro allocare chordam
 *
 * Reddit: chordam cum nomine symboli (ZERO-COPY in string table)
 *
 * NOTA: Chorda punctat in string table Mach-O.
 *       Si vis servare ultra vitam piscinae, usa chorda_transcribere().
 */
chorda
symbolum_nomen(
	constans Symbolum* symbolum,
	          Piscina* piscina);

/* Obtinere addressam symboli
 *
 * symbolum: symbolum
 *
 * Reddit: addressa virtualis (n_value)
 *
 * NOTA: Pro symbolis externis (undefined), potest esse ZEPHYRUM
 */
memoriae_index
symbolum_addressa(
	constans Symbolum* symbolum);

/* Obtinere typum symboli
 *
 * symbolum: symbolum
 *
 * Reddit: n_type valor (usa SYMBOLUM_TYPE_MASK pro extrahere typum)
 *
 * EXEMPLUM:
 *   i32 type = symbolum_genus(sym);
 *   i32 base_type = type & SYMBOLUM_TYPE_MASK;
 *   si (base_type == SYMBOLUM_TYPE_SECT) { ... }
 */
i32
symbolum_genus(
	constans Symbolum* symbolum);

/* Obtinere numerum sectionis
 *
 * symbolum: symbolum
 *
 * Reddit: numerus sectionis (1-based), vel ZEPHYRUM si non in sectione
 *
 * NOTA: Symbola SYMBOLUM_TYPE_SECT habent n_sect valorem validum
 *       Symbola externa habent n_sect == ZEPHYRUM
 */
i32
symbolum_numerus_sectionis(
	constans Symbolum* symbolum);

/* Obtinere descriptorem symboli
 *
 * symbolum: symbolum
 *
 * Reddit: n_desc valor (flags varii)
 */
i32
symbolum_descriptor(
	constans Symbolum* symbolum);


/* ==================================================
 * Utilitas - Interrogationes Booleanas
 * ================================================== */

/* Verificare si symbolum est functio
 *
 * Heuristicae:
 * - Symbolum est in sectione __TEXT,__text, vel
 * - Nomen incipit cum "_" et est in sectione codicis
 *
 * symbolum: symbolum
 *
 * Reddit: VERUM si probabiliter functio
 *
 * NOTA: Mach-O non distinguit functiones a datis directe
 *       Haec functio usat heuristicas communes
 */
b32
symbolum_est_functio(
	constans Symbolum* symbolum);

/* Verificare si symbolum est externum (globale)
 *
 * symbolum: symbolum
 *
 * Reddit: VERUM si SYMBOLUM_EXT situs
 */
b32
symbolum_est_externum(
	constans Symbolum* symbolum);

/* Verificare si symbolum est privatum externum
 *
 * symbolum: symbolum
 *
 * Reddit: VERUM si SYMBOLUM_PEXT situs
 */
b32
symbolum_est_privatum_externum(
	constans Symbolum* symbolum);

/* Verificare si symbolum est undefined (externum non definitum)
 *
 * symbolum: symbolum
 *
 * Reddit: VERUM si SYMBOLUM_TYPE_UNDEF
 */
b32
symbolum_est_indefinitum(
	constans Symbolum* symbolum);

/* Verificare si symbolum est debug (stab)
 *
 * symbolum: symbolum
 *
 * Reddit: VERUM si symbolum debug (SYMBOLUM_STAB)
 */
b32
symbolum_est_debug(
	constans Symbolum* symbolum);

/* Verificare si symbolum est debilis (weak)
 *
 * symbolum: symbolum
 *
 * Reddit: VERUM si SYMBOLUM_WEAK_DEF vel SYMBOLUM_WEAK_REF
 */
b32
symbolum_est_debile(
	constans Symbolum* symbolum);


/* ==================================================
 * Convenientia - Filtrationes
 * ================================================== */

/* Obtinere omnes functiones ex tabula
 *
 * Filtrat symbola quae symbolum_est_functio() reddit VERUM.
 * Non includit symbola externa indefinita.
 *
 * tabula:  tabula symbolorum
 * numerus: exitus - numerus functionum inventarum
 * piscina: piscina pro allocare tabulam
 *
 * Reddit: tabulam Symbolum* (functiones), vel NIHIL si error
 *
 * EXEMPLUM:
 *   i32 numerus;
 *   Symbolum** functiones = symbola_obtinere_functiones(tabula, &numerus, p);
 *   per (i32 i = ZEPHYRUM; i < numerus; i++)
 *   {
 *       chorda nomen = symbolum_nomen(functiones[i], p);
 *       imprimere("  %.*s\n", nomen.mensura, nomen.datum);
 *   }
 */
Symbolum**
symbola_obtinere_functiones(
	constans TabulaSymbolorum* tabula,
	                      i32* numerus,
	                  Piscina* piscina);

/* Obtinere omnia symbola externa indefinita
 *
 * Filtrat symbola quae sunt SYMBOLUM_TYPE_UNDEF et SYMBOLUM_EXT.
 * Haec sunt symbola quae binary requirit ex bibliothecis aliis.
 *
 * tabula:  tabula symbolorum
 * numerus: exitus - numerus symbolorum inventorum
 * piscina: piscina pro allocare tabulam
 *
 * Reddit: tabulam Symbolum* (externa), vel NIHIL si error
 */
Symbolum**
symbola_obtinere_externa_indefinita(
	constans TabulaSymbolorum* tabula,
	                      i32* numerus,
	                  Piscina* piscina);


/* ==================================================
 * Errores
 * ================================================== */

/* Obtinere nuntium erroris recens
 *
 * Reddit: literas describens errorem, vel NIHIL si nullus error
 *
 * NOTA: Nuntius non possessus a vocante (static buffer)
 */
constans character*
symbola_error_recens(
	vacuum);

#endif /* SYMBOLA_H */
