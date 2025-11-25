#include "nuntium.h"
#include <string.h>

/* ==================================================
 * Structurae Internae
 * ================================================== */

structura NuntiumScriptor {
	Piscina* piscina;
	     i8* datum;
	    i32  capacitas;
	    i32  positio;
	    b32  error;
};

structura NuntiumLector {
	Piscina* piscina;
	     i8* datum;
	    i32  mensura;
	    i32  positio;
	    b32  error;
};


/* ==================================================
 * Constantae
 * ================================================== */

#define CAPACITAS_INITIALIS_DEFAULT CCLVI  /* 256 */
#define VARINT_MAX_BYTES            X      /* 10 bytes max for 64-bit varint */


/* ==================================================
 * Functiones Auxiliares - Scriptor
 * ================================================== */

interior b32
_scriptor_ensure_capacitas(
	NuntiumScriptor* scriptor,
	             i32 bytes_necessarii)
{
	i32  nova_capacitas;
	i8*  novum_datum;

	si (scriptor->positio + bytes_necessarii <= scriptor->capacitas)
	{
		redde VERUM;
	}

	/* Duplicare capacitatem usque ad sufficientem */
	nova_capacitas = scriptor->capacitas;
	dum (nova_capacitas < scriptor->positio + bytes_necessarii)
	{
		nova_capacitas *= II;
	}

	novum_datum = (i8*)piscina_allocare(scriptor->piscina, (memoriae_index)nova_capacitas);
	si (!novum_datum)
	{
		scriptor->error = VERUM;
		redde FALSUM;
	}

	memcpy(novum_datum, scriptor->datum, (memoriae_index)scriptor->positio);
	scriptor->datum = novum_datum;
	scriptor->capacitas = nova_capacitas;

	redde VERUM;
}

interior b32
_scriptor_scribere_byte(
	NuntiumScriptor* scriptor,
	              i8 byte)
{
	si (!_scriptor_ensure_capacitas(scriptor, I))
	{
		redde FALSUM;
	}

	scriptor->datum[scriptor->positio] = byte;
	scriptor->positio++;

	redde VERUM;
}

interior b32
_scriptor_scribere_varint_raw(
	NuntiumScriptor* scriptor,
	             i64 valor)
{
	/* Varint: 7 bits per byte, high bit = continuation */
	fac
	{
		i8 byte = (i8)(valor & 0x7F);
		valor >>= VII;

		si (valor != 0)
		{
			byte |= (i8)0x80;  /* Set continuation bit */
		}

		si (!_scriptor_scribere_byte(scriptor, byte))
		{
			redde FALSUM;
		}
	}
	dum (valor != 0);

	redde VERUM;
}

interior b32
_scriptor_scribere_tag(
	NuntiumScriptor* scriptor,
	             i32 tag,
	             i32 wire_type)
{
	i64 tag_value = ((i64)tag << III) | (i64)wire_type;
	redde _scriptor_scribere_varint_raw(scriptor, tag_value);
}


/* ==================================================
 * Functiones Auxiliares - Lector
 * ================================================== */

interior b32
_lector_habet_bytes(
	NuntiumLector* lector,
	           i32 n)
{
	redde (lector->positio + n) <= lector->mensura;
}

interior i8
_lector_legere_byte(
	NuntiumLector* lector)
{
	si (!_lector_habet_bytes(lector, I))
	{
		lector->error = VERUM;
		redde 0;
	}

	redde lector->datum[lector->positio++];
}

interior i64
_lector_legere_varint_raw(
	NuntiumLector* lector)
{
	i64 valor = 0;
	i32 shift = 0;
	i8  byte;

	fac
	{
		si (shift >= LXIV)  /* Overflow protection */
		{
			lector->error = VERUM;
			redde 0;
		}

		byte = _lector_legere_byte(lector);
		si (lector->error)
		{
			redde 0;
		}

		valor |= ((i64)(byte & 0x7F)) << shift;
		shift += VII;
	}
	dum (byte & 0x80);

	redde valor;
}


/* ==================================================
 * Scriptor - Creatio
 * ================================================== */

NuntiumScriptor*
nuntium_scriptor_creare(
	Piscina* piscina,
	     i32 capacitas_initialis)
{
	NuntiumScriptor* scriptor;

	si (!piscina)
	{
		redde NIHIL;
	}

	si (capacitas_initialis <= ZEPHYRUM)
	{
		capacitas_initialis = CAPACITAS_INITIALIS_DEFAULT;
	}

	scriptor = (NuntiumScriptor*)piscina_allocare(piscina, magnitudo(NuntiumScriptor));
	si (!scriptor)
	{
		redde NIHIL;
	}

	scriptor->piscina = piscina;
	scriptor->capacitas = capacitas_initialis;
	scriptor->positio = ZEPHYRUM;
	scriptor->error = FALSUM;

	scriptor->datum = (i8*)piscina_allocare(piscina, (memoriae_index)capacitas_initialis);
	si (!scriptor->datum)
	{
		redde NIHIL;
	}

	redde scriptor;
}

vacuum
nuntium_scriptor_purgare(
	NuntiumScriptor* scriptor)
{
	si (!scriptor) redde;

	scriptor->positio = ZEPHYRUM;
	scriptor->error = FALSUM;
}


/* ==================================================
 * Scriptor - Scribere Valores
 * ================================================== */

b32
nuntium_scribere_varint(
	NuntiumScriptor* scriptor,
	             i32 tag,
	             i64 valor)
{
	si (!scriptor || scriptor->error)
	{
		redde FALSUM;
	}

	si (!_scriptor_scribere_tag(scriptor, tag, NUNTIUM_WIRE_VARINT))
	{
		redde FALSUM;
	}

	redde _scriptor_scribere_varint_raw(scriptor, valor);
}

b32
nuntium_scribere_svarint(
	NuntiumScriptor* scriptor,
	             i32 tag,
	             s64 valor)
{
	i64 encoded = nuntium_zigzag_encode(valor);
	redde nuntium_scribere_varint(scriptor, tag, encoded);
}

b32
nuntium_scribere_32bit(
	NuntiumScriptor* scriptor,
	             i32 tag,
	             i32 valor)
{
	si (!scriptor || scriptor->error)
	{
		redde FALSUM;
	}

	si (!_scriptor_scribere_tag(scriptor, tag, NUNTIUM_WIRE_32BIT))
	{
		redde FALSUM;
	}

	si (!_scriptor_ensure_capacitas(scriptor, IV))
	{
		redde FALSUM;
	}

	/* Little-endian */
	scriptor->datum[scriptor->positio++] = (i8)(valor & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> VIII) & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> XVI) & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> XXIV) & 0xFF);

	redde VERUM;
}

b32
nuntium_scribere_64bit(
	NuntiumScriptor* scriptor,
	             i32 tag,
	             i64 valor)
{
	si (!scriptor || scriptor->error)
	{
		redde FALSUM;
	}

	si (!_scriptor_scribere_tag(scriptor, tag, NUNTIUM_WIRE_64BIT))
	{
		redde FALSUM;
	}

	si (!_scriptor_ensure_capacitas(scriptor, VIII))
	{
		redde FALSUM;
	}

	/* Little-endian */
	scriptor->datum[scriptor->positio++] = (i8)(valor & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> VIII) & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> XVI) & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> XXIV) & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> XXXII) & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> XL) & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> XLVIII) & 0xFF);
	scriptor->datum[scriptor->positio++] = (i8)((valor >> LVI) & 0xFF);
	redde VERUM;
}

b32
nuntium_scribere_f32(
	NuntiumScriptor* scriptor,
	             i32 tag,
	             f32 valor)
{
	i32 bits;
	memcpy(&bits, &valor, IV);
	redde nuntium_scribere_32bit(scriptor, tag, bits);
}

b32
nuntium_scribere_f64(
	NuntiumScriptor* scriptor,
	             i32 tag,
	             f64 valor)
{
	i64 bits;
	memcpy(&bits, &valor, VIII);
	redde nuntium_scribere_64bit(scriptor, tag, bits);
}

b32
nuntium_scribere_bytes(
	NuntiumScriptor* scriptor,
	             i32 tag,
	constans    i8* datum,
	             i32 mensura)
{
	si (!scriptor || scriptor->error)
	{
		redde FALSUM;
	}

	si (!_scriptor_scribere_tag(scriptor, tag, NUNTIUM_WIRE_LENGTH_DELIMITED))
	{
		redde FALSUM;
	}

	/* Scribere longitudinem */
	si (!_scriptor_scribere_varint_raw(scriptor, (i64)mensura))
	{
		redde FALSUM;
	}

	/* Scribere datum */
	si (mensura > ZEPHYRUM)
	{
		si (!_scriptor_ensure_capacitas(scriptor, mensura))
		{
			redde FALSUM;
		}

		memcpy(scriptor->datum + scriptor->positio, datum, (memoriae_index)mensura);
		scriptor->positio += mensura;
	}

	redde VERUM;
}

b32
nuntium_scribere_chorda(
	NuntiumScriptor* scriptor,
	             i32 tag,
	          chorda valor)
{
	redde nuntium_scribere_bytes(scriptor, tag, valor.datum, valor.mensura);
}

b32
nuntium_scribere_nuntium(
	NuntiumScriptor* scriptor,
	             i32 tag,
	          chorda nuntium_datum)
{
	/* Nuntium nidum est idem quam bytes */
	redde nuntium_scribere_bytes(scriptor, tag, nuntium_datum.datum, nuntium_datum.mensura);
}

chorda
nuntium_scriptor_datum(
	NuntiumScriptor* scriptor)
{
	chorda result;

	si (!scriptor)
	{
		result.datum = NIHIL;
		result.mensura = ZEPHYRUM;
		redde result;
	}

	result.datum = scriptor->datum;
	result.mensura = scriptor->positio;

	redde result;
}


/* ==================================================
 * Lector - Creatio
 * ================================================== */

NuntiumLector*
nuntium_lector_creare(
	Piscina* piscina,
	  chorda datum)
{
	NuntiumLector* lector;

	si (!piscina)
	{
		redde NIHIL;
	}

	lector = (NuntiumLector*)piscina_allocare(piscina, magnitudo(NuntiumLector));
	si (!lector)
	{
		redde NIHIL;
	}

	lector->piscina = piscina;
	lector->datum = datum.datum;
	lector->mensura = datum.mensura;
	lector->positio = ZEPHYRUM;
	lector->error = FALSUM;

	redde lector;
}

b32
nuntium_lector_habet_plus(
	NuntiumLector* lector)
{
	si (!lector || lector->error)
	{
		redde FALSUM;
	}

	redde lector->positio < lector->mensura;
}


/* ==================================================
 * Lector - Legere Fields
 * ================================================== */

b32
nuntium_legere_field(
	NuntiumLector* lector,
	          i32* tag_out,
	          i32* wire_type_out)
{
	i64 tag_value;

	si (!lector || lector->error)
	{
		redde FALSUM;
	}

	si (!nuntium_lector_habet_plus(lector))
	{
		redde FALSUM;
	}

	tag_value = _lector_legere_varint_raw(lector);
	si (lector->error)
	{
		redde FALSUM;
	}

	si (tag_out)
	{
		*tag_out = (i32)(tag_value >> III);
	}

	si (wire_type_out)
	{
		*wire_type_out = (i32)(tag_value & 0x07);
	}

	redde VERUM;
}

b32
nuntium_saltare_field(
	NuntiumLector* lector,
	           i32 wire_type)
{
	i64 length;

	si (!lector || lector->error)
	{
		redde FALSUM;
	}

	commutatio (wire_type)
	{
		casus NUNTIUM_WIRE_VARINT:
			/* Legere et ignorare varint */
			_lector_legere_varint_raw(lector);
			frange;

		casus NUNTIUM_WIRE_64BIT:
			/* Saltare 8 bytes */
			si (!_lector_habet_bytes(lector, VIII))
			{
				lector->error = VERUM;
				redde FALSUM;
			}
			lector->positio += VIII;
			frange;

		casus NUNTIUM_WIRE_LENGTH_DELIMITED:
			/* Legere longitudinem, saltare bytes */
			length = _lector_legere_varint_raw(lector);
			si (lector->error)
			{
				redde FALSUM;
			}
			si (!_lector_habet_bytes(lector, (i32)length))
			{
				lector->error = VERUM;
				redde FALSUM;
			}
			lector->positio += (i32)length;
			frange;

		casus NUNTIUM_WIRE_32BIT:
			/* Saltare 4 bytes */
			si (!_lector_habet_bytes(lector, IV))
			{
				lector->error = VERUM;
				redde FALSUM;
			}
			lector->positio += IV;
			frange;

		ordinarius:
			/* Wire type ignotum */
			lector->error = VERUM;
			redde FALSUM;
	}

	redde !lector->error;
}


/* ==================================================
 * Lector - Legere Valores
 * ================================================== */

i64
nuntium_legere_varint(
	NuntiumLector* lector)
{
	si (!lector || lector->error)
	{
		redde 0;
	}

	redde _lector_legere_varint_raw(lector);
}

s64
nuntium_legere_svarint(
	NuntiumLector* lector)
{
	i64 encoded = nuntium_legere_varint(lector);
	redde nuntium_zigzag_decode(encoded);
}

i32
nuntium_legere_32bit(
	NuntiumLector* lector)
{
	i32 valor;

	si (!lector || lector->error)
	{
		redde 0;
	}

	si (!_lector_habet_bytes(lector, IV))
	{
		lector->error = VERUM;
		redde 0;
	}

	/* Little-endian */
	valor = (i32)((i8)lector->datum[lector->positio] & 0xFF);
	valor |= (i32)((i8)lector->datum[lector->positio + I] & 0xFF) << VIII;
	valor |= (i32)((i8)lector->datum[lector->positio + II] & 0xFF) << XVI;
	valor |= (i32)((i8)lector->datum[lector->positio + III] & 0xFF) << XXIV;

	lector->positio += IV;

	redde valor;
}

i64
nuntium_legere_64bit(
	NuntiumLector* lector)
{
	i64 valor;

	si (!lector || lector->error)
	{
		redde 0;
	}

	si (!_lector_habet_bytes(lector, VIII))
	{
		lector->error = VERUM;
		redde 0;
	}

	/* Little-endian */
	valor = (i64)((i8)lector->datum[lector->positio] & 0xFF);
	valor |= (i64)((i8)lector->datum[lector->positio + I] & 0xFF) << VIII;
	valor |= (i64)((i8)lector->datum[lector->positio + II] & 0xFF) << XVI;
	valor |= (i64)((i8)lector->datum[lector->positio + III] & 0xFF) << XXIV;
	valor |= (i64)((i8)lector->datum[lector->positio + IV] & 0xFF) << XXXII;
	valor |= (i64)((i8)lector->datum[lector->positio + V] & 0xFF) << XL;
	valor |= (i64)((i8)lector->datum[lector->positio + VI] & 0xFF) << XLVIII;
	valor |= (i64)((i8)lector->datum[lector->positio + VII] & 0xFF) << LVI;

	lector->positio += VIII;

	redde valor;
}

f32
nuntium_legere_f32(
	NuntiumLector* lector)
{
	i32 bits;
	f32 valor;

	bits = nuntium_legere_32bit(lector);
	memcpy(&valor, &bits, IV);

	redde valor;
}

f64
nuntium_legere_f64(
	NuntiumLector* lector)
{
	i64 bits;
	f64 valor;

	bits = nuntium_legere_64bit(lector);
	memcpy(&valor, &bits, VIII);

	redde valor;
}

chorda
nuntium_legere_bytes(
	NuntiumLector* lector)
{
	chorda result;
	   i64 length;

	result.datum = NIHIL;
	result.mensura = ZEPHYRUM;

	si (!lector || lector->error)
	{
		redde result;
	}

	length = _lector_legere_varint_raw(lector);
	si (lector->error)
	{
		redde result;
	}

	si (!_lector_habet_bytes(lector, (i32)length))
	{
		lector->error = VERUM;
		redde result;
	}

	/* Zero-copy: reference into original datum */
	result.datum = lector->datum + lector->positio;
	result.mensura = (i32)length;

	lector->positio += (i32)length;

	redde result;
}

chorda
nuntium_legere_chorda(
	NuntiumLector* lector)
{
	redde nuntium_legere_bytes(lector);
}

chorda
nuntium_legere_nuntium(
	NuntiumLector* lector)
{
	redde nuntium_legere_bytes(lector);
}


/* ==================================================
 * Utilitas - Varint
 * ================================================== */

i32
nuntium_varint_mensura(
	i64 valor)
{
	i32 mensura = I;

	dum (valor > 0x7F)
	{
		mensura++;
		valor >>= VII;
	}

	redde mensura;
}

i64
nuntium_zigzag_encode(
	s64 valor)
{
	/* (n << 1) ^ (n >> 63) */
	redde (i64)((valor << I) ^ (valor >> LXIII));
}

s64
nuntium_zigzag_decode(
	i64 valor)
{
	/* (n >> 1) ^ -(n & 1) */
	s64 shifted = (s64)(valor >> I);
	s64 mask = -((s64)(valor & I));
	redde shifted ^ mask;
}


/* ==================================================
 * Errores
 * ================================================== */

b32
nuntium_lector_error(
	NuntiumLector* lector)
{
	si (!lector)
	{
		redde VERUM;
	}

	redde lector->error;
}

b32
nuntium_scriptor_error(
	NuntiumScriptor* scriptor)
{
	si (!scriptor)
	{
		redde VERUM;
	}

	redde scriptor->error;
}
