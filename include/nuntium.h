#ifndef NUNTIUM_H
#define NUNTIUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

/* ==================================================
 * Nuntium - Protobuf-style Binary Serialization
 * ==================================================
 *
 * Wire format:
 *   [tag << 3 | wire_type][payload...]
 *
 * Wire types:
 *   0 = varint (i32, i64, b32, enum)
 *   1 = 64-bit fixed (f64, fixed i64)
 *   2 = length-delimited (chorda, bytes, nested message)
 *   5 = 32-bit fixed (f32, fixed i32)
 *
 * Varint encoding:
 *   Each byte uses 7 bits for data, high bit indicates continuation.
 *   Small values use fewer bytes.
 */


/* ==================================================
 * Wire Types
 * ================================================== */

nomen enumeratio {
	NUNTIUM_WIRE_VARINT           = 0,  /* Variable-length integer */
	NUNTIUM_WIRE_64BIT            = 1,  /* Fixed 64-bit */
	NUNTIUM_WIRE_LENGTH_DELIMITED = 2,  /* Length-prefixed bytes */
	NUNTIUM_WIRE_32BIT            = 5   /* Fixed 32-bit */
} NuntiumWireGenus;


/* ==================================================
 * Scriptor - Writing Messages
 * ================================================== */

nomen structura NuntiumScriptor NuntiumScriptor;

/* Creare scriptor novum
 * Capacitas initialis est suggestio; crescit automatice */
NuntiumScriptor*
nuntium_scriptor_creare (
	Piscina* piscina,
	    i32  capacitas_initialis);

/* Purgare scriptor pro reuse (sine nova allocatio) */
vacuum
nuntium_scriptor_purgare (
	NuntiumScriptor* scriptor);

/* Scribere varint (i32/i64/b32/enum) */
b32
nuntium_scribere_varint (
	NuntiumScriptor* scriptor,
	             i32 tag,
	             i64 valor);

/* Scribere varint signatum (zigzag encoding pro numeris negativis) */
b32
nuntium_scribere_svarint (
	NuntiumScriptor* scriptor,
	             i32 tag,
	             s64 valor);

/* Scribere 32-bit fixum (f32 vel i32 fixum) */
b32
nuntium_scribere_32bit (
	NuntiumScriptor* scriptor,
	             i32 tag,
	             i32 valor);

/* Scribere 64-bit fixum (f64 vel i64 fixum) */
b32
nuntium_scribere_64bit (
	NuntiumScriptor* scriptor,
	             i32 tag,
	             i64 valor);

/* Scribere f32 */
b32
nuntium_scribere_f32 (
	NuntiumScriptor* scriptor,
	             i32 tag,
	             f32 valor);

/* Scribere f64 */
b32
nuntium_scribere_f64 (
	NuntiumScriptor* scriptor,
	             i32 tag,
	             f64 valor);

/* Scribere chorda (length-delimited) */
b32
nuntium_scribere_chorda (
	NuntiumScriptor* scriptor,
	             i32 tag,
	          chorda valor);

/* Scribere bytes crudos (length-delimited) */
b32
nuntium_scribere_bytes (
	NuntiumScriptor* scriptor,
	             i32 tag,
	constans    i8* datum,
	             i32 mensura);

/* Scribere nuntium nidum (nested message, length-delimited) */
b32
nuntium_scribere_nuntium (
	NuntiumScriptor* scriptor,
	             i32 tag,
	          chorda nuntium_datum);

/* Capere datum finale
 * Reddit chordam cum bytes serialisatis
 * Chorda valida usque ad purgare/destruere */
chorda
nuntium_scriptor_datum (
	NuntiumScriptor* scriptor);


/* ==================================================
 * Lector - Reading Messages
 * ================================================== */

nomen structura NuntiumLector NuntiumLector;

/* Creare lector ex datum */
NuntiumLector*
nuntium_lector_creare (
	Piscina* piscina,
	  chorda datum);

/* Verificare si plus datum ad legere */
b32
nuntium_lector_habet_plus (
	NuntiumLector* lector);

/* Legere proximum field header
 * Reddit VERUM si field lectum, FALSUM si finis
 * Ponit tag et wire_type */
b32
nuntium_legere_field (
	NuntiumLector* lector,
	          i32* tag_out,
	          i32* wire_type_out);

/* Saltare field currentem (si wire type ignotum) */
b32
nuntium_saltare_field (
	NuntiumLector* lector,
	           i32 wire_type);

/* Legere varint (post legere_field cum wire_type = 0) */
i64
nuntium_legere_varint (
	NuntiumLector* lector);

/* Legere varint signatum (zigzag decoding) */
s64
nuntium_legere_svarint (
	NuntiumLector* lector);

/* Legere 32-bit fixum */
i32
nuntium_legere_32bit (
	NuntiumLector* lector);

/* Legere 64-bit fixum */
i64
nuntium_legere_64bit (
	NuntiumLector* lector);

/* Legere f32 */
f32
nuntium_legere_f32 (
	NuntiumLector* lector);

/* Legere f64 */
f64
nuntium_legere_f64 (
	NuntiumLector* lector);

/* Legere chorda (length-delimited)
 * Reddit chordam referens datum originale (non copiatum) */
chorda
nuntium_legere_chorda (
	NuntiumLector* lector);

/* Legere bytes crudos (length-delimited)
 * Reddit chordam referens datum originale */
chorda
nuntium_legere_bytes (
	NuntiumLector* lector);

/* Legere nuntium nidum
 * Reddit chordam pro creare sub-lector */
chorda
nuntium_legere_nuntium (
	NuntiumLector* lector);


/* ==================================================
 * Utilitas - Varint Encoding
 * ================================================== */

/* Computare bytes necessarios pro varint */
i32
nuntium_varint_mensura (
	i64 valor);

/* Zigzag encode (pro signed integers) */
i64
nuntium_zigzag_encode (
	s64 valor);

/* Zigzag decode */
s64
nuntium_zigzag_decode (
	i64 valor);


/* ==================================================
 * Errores
 * ================================================== */

/* Verificare si error occurrit */
b32
nuntium_lector_error (
	NuntiumLector* lector);

b32
nuntium_scriptor_error (
	NuntiumScriptor* scriptor);

#endif /* NUNTIUM_H */
