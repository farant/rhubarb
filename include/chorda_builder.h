#ifndef CHORDA_BUILDER_H
#define CHORDA_BUILDER_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ==================================================
 * ChordaBuilder - Efficient string accumulation
 *
 * For building strings when final size is unknown.
 * Designed for structured text generation:
 * - INI/JSON/XML builders (primary use cases)
 * - Log formatting
 * - Text generation from data structures
 *
 * PATTERN:
 *   Piscina* p = piscina_generare_dynamicum("test", 4096);
 *   ChordaBuilder* b = chorda_builder_creare(p, 256);
 *
 *   chorda_builder_appendere_literis(b, "name");
 *   chorda_builder_appendere_character(b, ':');
 *   chorda_builder_appendere_integer(b, 42);
 *   chorda_builder_appendere_newline(b);
 *
 *   chorda result = chorda_builder_finire(b);
 *   piscina_destruere(p);
 *
 * FEATURES:
 * - Automatic capacity growth (doubles when full)
 * - Indentation tracking (for pretty-printing)
 * - Format-specific escaping (JSON escaping built-in)
 * - Reusable via reset()
 * - All allocations from passed Piscina
 *
 * ================================================== */

nomen structura ChordaBuilder ChordaBuilder;

/* ==================================================
 * Creatio / Destructio
 * ================================================== */

ChordaBuilder*
chorda_builder_creare (
	       Piscina* piscina,
	memoriae_index  capacitas_initialis);

vacuum
chorda_builder_destruere (
	ChordaBuilder* builder);


/* ==================================================
 * Appendere - Singularis Character
 * ================================================== */

b32
chorda_builder_appendere_character (
	ChordaBuilder* builder,
	     character  c);


/* ==================================================
 * Appendere - Strings (Chordae and C-strings)
 * ================================================== */

b32
chorda_builder_appendere_literis (
	ChordaBuilder* builder,
	constans character* cstr);

b32
chorda_builder_appendere_chorda (
	ChordaBuilder* builder,
	          chorda  s);


/* ==================================================
 * Appendere - Numeri (Integers)
 * ================================================== */

b32
chorda_builder_appendere_integer (
	ChordaBuilder* builder,
	           s32  n);

b32
chorda_builder_appendere_i32 (
	ChordaBuilder* builder,
	          i32  n);


/* ==================================================
 * Appendere - Numeri (Floating Point)
 *
 * decimals: number of decimal places to show
 * ================================================== */

b32
chorda_builder_appendere_duplex (
	ChordaBuilder* builder,
	           f64  n,
	           i32  decimales);


/* ==================================================
 * Appendere - Escaped (Format-Specific)
 *
 * appendere_escaped_json: JSON string escaping
 *   - Handles: quote, backslash, control chars
 *   - Properly escapes for use in JSON strings
 * ================================================== */

b32
chorda_builder_appendere_escaped_json (
	ChordaBuilder* builder,
	         chorda  s);

b32
chorda_builder_appendere_literis_escaped_json (
	ChordaBuilder* builder,
	constans character* cstr);


/* ==================================================
 * Appendere - Whitespace and Structure
 * ================================================== */

b32
chorda_builder_appendere_newline (
	ChordaBuilder* builder);

/* appendere_indentation: append N spaces (for current level)
 * where N = level * spaces_per_level (typically 2 or 4)
 *
 * Pass indentation_level from builder, or specify manually */
b32
chorda_builder_appendere_indentation (
	ChordaBuilder* builder,
	           i32  level);


/* ==================================================
 * Indentation Tracking (for pretty-printing)
 *
 * Builders track an internal indentation level.
 * push_indentation(): increment level
 * pop_indentation(): decrement level
 * indentation_level(): query current level
 *
 * Common pattern:
 *   chorda_builder_appendere_literis(b, "{");
 *   chorda_builder_appendere_newline(b);
 *   chorda_builder_push_indentation(b);
 *   chorda_builder_appendere_indentation(b,
 *       chorda_builder_indentation_level(b));
 *   ... append content ...
 *   chorda_builder_pop_indentation(b);
 * ================================================== */

vacuum
chorda_builder_push_indentation (
	ChordaBuilder* builder);

vacuum
chorda_builder_pop_indentation (
	ChordaBuilder* builder);

i32
chorda_builder_indentation_level (
	ChordaBuilder* builder);


/* ==================================================
 * Quaestio - State Query
 * ================================================== */

/* longitudo: current length of accumulated string */
memoriae_index
chorda_builder_longitudo (
	ChordaBuilder* builder);

/* spectare: view current content without finalizing
 * Returns a chorda view of current buffer.
 * Valid only until next modification. */
chorda
chorda_builder_spectare (
	ChordaBuilder* builder);


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

/* reset: clear content, keep allocated capacity
 * Useful for reusing builder for multiple strings */
vacuum
chorda_builder_reset (
	ChordaBuilder* builder);

/* finire: convert builder to chorda
 * Transfers ownership of accumulated string to caller.
 * Builder is destroyed after finire.
 * Caller must eventually deallocate via piscina. */
chorda
chorda_builder_finire (
	ChordaBuilder* builder);


/* ==================================================
 * Configuration Constants
 * ================================================== */

/* CHORDA_BUILDER_INDENTATIO_SPATIA
 * Number of spaces per indentation level (typically 2 or 4) */
#define CHORDA_BUILDER_INDENTATIO_SPATIA II


#endif /* CHORDA_BUILDER_H */
