#ifndef SECTIO_H
#define SECTIO_H

#include "latina.h"
#include "chorda.h"
#include "macho.h"
#include "piscina.h"

/* ==================================================
 * SECTIO - Extractio Sectionum ex Mach-O
 *
 * Bibliotheca pro parsendo sectiones ex segmentis
 * Mach-O. Sustinet LC_SEGMENT_64 et LC_SEGMENT.
 *
 * PHILOSOPHIA:
 * - Zero-copy (datum puncta in buffer Mach-O)
 * - Iterator pattern (comprobatum in macho.h)
 * - Omnia ex piscina allocata
 *
 * EXEMPLUM:
 *   Piscina* p = piscina_generare_dynamicum("parse", 4*1024*1024);
 *   MachO* macho = macho_aperire("/bin/ls", p);
 *
 *   -- Iterare omnes sectiones
 *   SectioIterator iter = sectio_iterator_initium(macho);
 *   Sectio* sectio;
 *   dum ((sectio = sectio_iterator_proximum(&iter)) != NIHIL)
 *   {
 *       chorda nomen = sectio_nomen(sectio, p);
 *       chorda data  = sectio_datum(sectio);
 *       imprimere("Section: %.*s, size: %zu\n",
 *                 nomen.mensura, nomen.datum, data.mensura);
 *   }
 *
 *   -- Invenire sectionem specificam
 *   Sectio* text = sectio_invenire(macho, "__TEXT", "__text", p);
 *   si (text != NIHIL)
 *   {
 *       chorda code = sectio_datum(text);
 *       -- Processa code...
 *   }
 *
 *   piscina_destruere(p);
 *
 * ================================================== */


/* ==================================================
 * Typi Opaci
 * ================================================== */

nomen structura Sectio Sectio;


/* ==================================================
 * Iteratio - Omnes Sectiones
 * ================================================== */

/* Iterator pro ambulatione omnium sectionum in omnibus segmentis */
nomen structura SectioIterator {
	constans MachO* macho;
	           i32  mandatum_index;       /* Current load command */
	           i32  sectio_index;         /* Current section in segment */
	           i32  sectio_numerus;       /* Number of sections in current segment */
	memoriae_index  offset_currens;       /* Offset to current section header */
} SectioIterator;

/* Initializare iteratorem
 *
 * macho: imago Mach-O
 *
 * Reddit: iterator ad initium
 */
SectioIterator
sectio_iterator_initium(
	constans MachO* macho);

/* Obtinere sectionem proximam
 *
 * iter: iterator
 *
 * Reddit: sectionem proximam, vel NIHIL si finis
 *
 * NOTA: Sectio allocata ex piscina macho
 *
 * EXEMPLUM:
 *   SectioIterator iter = sectio_iterator_initium(macho);
 *   Sectio* sectio;
 *   dum ((sectio = sectio_iterator_proximum(&iter)) != NIHIL)
 *   {
 *       -- processa sectionem
 *   }
 */
Sectio*
sectio_iterator_proximum(
	SectioIterator* iter);

/* Verificare si iteratio completa
 *
 * iter: iterator
 *
 * Reddit: VERUM si finis, FALSUM alioquin
 */
b32
sectio_iterator_finis(
	SectioIterator* iter);


/* ==================================================
 * Quaestio - Invenire Sectiones
 * ================================================== */

/* Invenire sectionem per nomen segmenti et nomen sectionis
 *
 * macho:           imago Mach-O
 * nomen_segmenti:  nomen segmenti (e.g., "__TEXT")
 * nomen_sectionis: nomen sectionis (e.g., "__text")
 * piscina:         piscina pro allocatione
 *
 * Reddit: sectionem inventam, vel NIHIL si non inventa
 *
 * EXEMPLA:
 *   Sectio* text = sectio_invenire(macho, "__TEXT", "__text", p);
 *   Sectio* data = sectio_invenire(macho, "__DATA", "__data", p);
 *   Sectio* cstring = sectio_invenire(macho, "__TEXT", "__cstring", p);
 */
Sectio*
sectio_invenire(
	constans MachO* macho,
	constans character* nomen_segmenti,
	constans character* nomen_sectionis,
	       Piscina* piscina);


/* ==================================================
 * Interrogatio - Attributa Sectionis
 * ================================================== */

/* Obtinere nomen sectionis
 *
 * sectio:  sectio
 * piscina: piscina pro allocare chordam
 *
 * Reddit: chordam cum nomine sectionis
 *
 * NOTA: Nomen in formato Mach-O est char[16], potest non terminari cum \0
 */
chorda
sectio_nomen(
	constans Sectio* sectio,
	       Piscina* piscina);

/* Obtinere nomen segmenti
 *
 * sectio:  sectio
 * piscina: piscina pro allocare chordam
 *
 * Reddit: chordam cum nomine segmenti
 */
chorda
sectio_nomen_segmenti(
	constans Sectio* sectio,
	       Piscina* piscina);

/* Obtinere datum sectionis (ZERO-COPY)
 *
 * sectio: sectio
 *
 * Reddit: chordam punctantem in buffer Mach-O
 *
 * NOTA: Chorda punctat in datum originale Mach-O.
 *       Si vis servare ultra vitam piscinae, usa chorda_transcribere().
 *
 * EXEMPLUM:
 *   chorda data = sectio_datum(text);
 *   -- Usa data...
 *
 *   -- Si vis servare
 *   Piscina* cache = piscina_generare_dynamicum("cache", 64*1024);
 *   chorda saved = chorda_transcribere(data, cache);
 */
chorda
sectio_datum(
	constans Sectio* sectio);

/* Obtinere mensuram sectionis in bytes
 *
 * sectio: sectio
 *
 * Reddit: mensuram sectionis
 */
memoriae_index
sectio_mensura(
	constans Sectio* sectio);

/* Obtinere addressam virtualem sectionis
 *
 * sectio: sectio
 *
 * Reddit: addressam virtualem (VM address)
 */
memoriae_index
sectio_addressa(
	constans Sectio* sectio);

/* Obtinere ordinationem sectionis
 *
 * sectio: sectio
 *
 * Reddit: ordinationem (2^n bytes)
 *
 * NOTA: Valor 3 significat 2^3 = 8 byte alignment
 */
i32
sectio_ordinatio(
	constans Sectio* sectio);

/* Obtinere vexilla sectionis
 *
 * sectio: sectio
 *
 * Reddit: vexilla sectionis (S_* constantae)
 *
 * NOTA: Vexilla includunt genus sectionis et attributa
 */
i32
sectio_vexilla(
	constans Sectio* sectio);


/* ==================================================
 * Constantae - Genera Sectionum
 *
 * Ex mach-o/loader.h section flags
 * ================================================== */

/* Genus sectionis (vexilla & SECTION_TYPE) */
#define SECTIO_GENUS_REGULAR                    0x0  /* S_REGULAR */
#define SECTIO_GENUS_ZEROFILL                   0x1  /* S_ZEROFILL */
#define SECTIO_GENUS_CSTRING_LITERALS           0x2  /* S_CSTRING_LITERALS */
#define SECTIO_GENUS_4BYTE_LITERALS             0x3  /* S_4BYTE_LITERALS */
#define SECTIO_GENUS_8BYTE_LITERALS             0x4  /* S_8BYTE_LITERALS */
#define SECTIO_GENUS_LITERAL_POINTERS           0x5  /* S_LITERAL_POINTERS */
#define SECTIO_GENUS_SYMBOL_STUBS               0x8  /* S_SYMBOL_STUBS */
#define SECTIO_GENUS_MOD_INIT_FUNC_POINTERS     0x9  /* S_MOD_INIT_FUNC_POINTERS */
#define SECTIO_GENUS_MOD_TERM_FUNC_POINTERS     0xa  /* S_MOD_TERM_FUNC_POINTERS */

/* Attributa sectionis */
#define SECTIO_ATTR_PURE_INSTRUCTIONS      0x80000000  /* S_ATTR_PURE_INSTRUCTIONS */
#define SECTIO_ATTR_SOME_INSTRUCTIONS      0x00000400  /* S_ATTR_SOME_INSTRUCTIONS */
#define SECTIO_ATTR_DEBUG                  0x02000000  /* S_ATTR_DEBUG */
#define SECTIO_ATTR_LIVE_SUPPORT           0x08000000  /* S_ATTR_LIVE_SUPPORT */
#define SECTIO_ATTR_NO_DEAD_STRIP          0x10000000  /* S_ATTR_NO_DEAD_STRIP */

/* Mask pro extrahere genus */
#define SECTIO_GENUS_MASK                       0xff  /* SECTION_TYPE */


/* ==================================================
 * Utilitas - Genus Sectionis
 * ================================================== */

/* Extrahere genus sectionis ex vexillis
 *
 * vexilla: vexilla sectionis
 *
 * Reddit: genus sectionis (SECTIO_GENUS_*)
 */
i32
sectio_extrahere_genus(
	i32 vexilla);

/* Verificare si sectio continet instructiones
 *
 * sectio: sectio
 *
 * Reddit: VERUM si continet instructiones
 */
b32
sectio_est_instructiones(
	constans Sectio* sectio);

/* Verificare si sectio est debug info
 *
 * sectio: sectio
 *
 * Reddit: VERUM si est debug
 */
b32
sectio_est_debug(
	constans Sectio* sectio);


/* ==================================================
 * Errores
 * ================================================== */

/* Obtinere nuntium erroris recens
 *
 * Reddit: literas describens errorem, vel NIHIL si nullus error
 */
constans character*
sectio_error_recens(
	vacuum);

#endif /* SECTIO_H */
