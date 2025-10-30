#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ==================================================
 * Structura ChordaAedificator - Interna
 * ================================================== */

structura ChordaAedificator {
	           i8*  buffer;
	memoriae_index  capacitas;
	memoriae_index  offset;
	       Piscina* piscina;
	           i32  indentatio_gradus;
};


/* ==================================================
 * ADIUTORES INTERNI
 * ================================================== */

interior memoriae_index
_proxima_capacitas(memoriae_index nunc)
{
	/* Duplica capacitatem donec satis habeamus */
	redde nunc > ZEPHYRUM ? nunc * II : XVI;
}

interior b32
_crescere(ChordaAedificator* aedificator, memoriae_index necessaria)
{
	memoriae_index capacitas_nova;
	i8* buffer_novum;

	capacitas_nova = aedificator->capacitas;
	dum (capacitas_nova < necessaria)
	{
		capacitas_nova = _proxima_capacitas(capacitas_nova);
	}

	buffer_novum = (i8*)piscina_allocare(aedificator->piscina, capacitas_nova);
	si (!buffer_novum)
		redde FALSUM;

	si (aedificator->buffer && aedificator->offset > ZEPHYRUM)
	{
		memcpy(buffer_novum, aedificator->buffer, aedificator->offset);
	}

	aedificator->buffer = buffer_novum;
	aedificator->capacitas = capacitas_nova;

	redde VERUM;
}

interior b32
_appendere_interna(ChordaAedificator* aedificator, constans i8* datum, memoriae_index mensura)
{
	memoriae_index necessaria;

	si (!aedificator || !datum || mensura == ZEPHYRUM)
		redde mensura == ZEPHYRUM; /* Appendix vacua bona est */

	necessaria = aedificator->offset + mensura;

	si (necessaria > aedificator->capacitas)
	{
		si (!_crescere(aedificator, necessaria))
			redde FALSUM;
	}

	memcpy(aedificator->buffer + aedificator->offset, datum, mensura);
	aedificator->offset += mensura;

	redde VERUM;
}

/* Formata integrum signatum ad buffer
 * Vocans praebet buffer, debet esse >= 32 bytes (tutus pro s32) */
interior memoriae_index
_format_integer_s32(s32 n, i8* buffer, memoriae_index capacitas)
{
	character cstr[CXXXII];
	s32 mensura_signed;
	memoriae_index mensura;

	mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr), "%d", n);
	si (mensura_signed < ZEPHYRUM)
		redde ZEPHYRUM;

	mensura = (memoriae_index)mensura_signed;
	si (mensura >= capacitas)
		redde ZEPHYRUM;

	memcpy(buffer, cstr, mensura);
	redde mensura;
}

interior memoriae_index
_format_integer_i32(i32 n, i8* buffer, memoriae_index capacitas)
{
	character cstr[CXXXII];
	s32 mensura_signed;
	memoriae_index mensura;

	mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr), "%u", n);
	si (mensura_signed < ZEPHYRUM)
		redde ZEPHYRUM;

	mensura = (memoriae_index)mensura_signed;
	si (mensura >= capacitas)
		redde ZEPHYRUM;

	memcpy(buffer, cstr, mensura);
	redde mensura;
}

interior memoriae_index
_format_duplex(f64 n, i32 decimales, i8* buffer, memoriae_index capacitas)
{
	character cstr[CXXXII];
	character formatalis[XVI];
	s32 mensura_signed;
	memoriae_index mensura;

	snprintf(formatalis, (memoriae_index)magnitudo(formatalis), "%%.%df", decimales);
	mensura_signed = snprintf(cstr, (memoriae_index)magnitudo(cstr), formatalis, n);

	si (mensura_signed < ZEPHYRUM)
		redde ZEPHYRUM;

	mensura = (memoriae_index)mensura_signed;
	si (mensura >= capacitas)
		redde ZEPHYRUM;

	memcpy(buffer, cstr, mensura);
	redde mensura;
}

/* Effugium chordae JSON
 * Tractat: citationem, virgulam inversam, characteres imperantes
 * Vocans praebet buffer */
interior memoriae_index
_evadere_json(constans i8* input, memoriae_index mensura, i8* output, memoriae_index capacitas_output)
{
	memoriae_index index_input;
	memoriae_index index_output;
	character c;
	character hex_buffer[VII];
	s32 hex_len_signed;
	memoriae_index hex_len;

	index_input = ZEPHYRUM;
	index_output = ZEPHYRUM;

	dum (index_input < mensura && index_output < capacitas_output - I)
	{
		c = (character)input[index_input];

		si (c == '"')
		{
			si (index_output + II > capacitas_output)
				redde ZEPHYRUM;
			output[index_output++] = '\\';
			output[index_output++] = '"';
		}
		alioquin si (c == '\\')
		{
			si (index_output + II > capacitas_output)
				redde ZEPHYRUM;
			output[index_output++] = '\\';
			output[index_output++] = '\\';
		}
		alioquin si (c == '\n')
		{
			si (index_output + II > capacitas_output)
				redde ZEPHYRUM;
			output[index_output++] = '\\';
			output[index_output++] = 'n';
		}
		alioquin si (c == '\r')
		{
			si (index_output + II > capacitas_output)
				redde ZEPHYRUM;
			output[index_output++] = '\\';
			output[index_output++] = 'r';
		}
		alioquin si (c == '\t')
		{
			si (index_output + II > capacitas_output)
				redde ZEPHYRUM;
			output[index_output++] = '\\';
			output[index_output++] = 't';
		}
		alioquin si (iscntrl((signatus character)c))
		{
			/* Alii characteres imperantes ut \uXXXX */
			hex_len_signed = snprintf(hex_buffer, VII, "\\u%04x", (signatus character)c);
			si (hex_len_signed < ZEPHYRUM)
				redde ZEPHYRUM;

			hex_len = (memoriae_index)hex_len_signed;
			si (index_output + hex_len > capacitas_output)
				redde ZEPHYRUM;

			memcpy(output + index_output, hex_buffer, hex_len);
			index_output += hex_len;
		}
		alioquin
		{
			output[index_output++] = (i8)c;
		}

		index_input++;
	}

	redde index_output;
}


/* ==================================================
 * Creatio
 * ================================================== */

ChordaAedificator*
chorda_aedificator_creare(Piscina* piscina, memoriae_index capacitas_initialis)
{
	ChordaAedificator* aedificator;
	i8* buffer;

	si (!piscina || capacitas_initialis == ZEPHYRUM)
		redde NIHIL;

	aedificator = (ChordaAedificator*)piscina_allocare(piscina, magnitudo(ChordaAedificator));
	si (!aedificator)
		redde NIHIL;

	buffer = (i8*)piscina_allocare(piscina, capacitas_initialis);
	si (!buffer)
		redde NIHIL;

	aedificator->buffer = buffer;
	aedificator->capacitas = capacitas_initialis;
	aedificator->offset = ZEPHYRUM;
	aedificator->piscina = piscina;
	aedificator->indentatio_gradus = ZEPHYRUM;

	redde aedificator;
}


/* ==================================================
 * Destructio
 * ================================================== */

vacuum
chorda_aedificator_destruere(ChordaAedificator* aedificator)
{
	/* Piscina possidet memoriam; solum liberamus structuram */
	si (aedificator)
	{
		/* Nota: buffer etiam allocatus ex piscina,
		 * ergo liberabitur quando piscina destruitur */
	}
}


/* ==================================================
 * Appendere - Character
 * ================================================== */

b32
chorda_aedificator_appendere_character(ChordaAedificator* aedificator, character c)
{
	i8 ch = (i8)c;
	redde _appendere_interna(aedificator, &ch, I);
}


/* ==================================================
 * Appendere - Chordae
 * ================================================== */

b32
chorda_aedificator_appendere_literis(ChordaAedificator* aedificator, constans character* cstr)
{
	memoriae_index mensura;

	si (!aedificator || !cstr)
		redde FALSUM;

	mensura = strlen(cstr);
	redde _appendere_interna(aedificator, (constans i8*)cstr, mensura);
}

b32
chorda_aedificator_appendere_chorda(ChordaAedificator* aedificator, chorda s)
{
	si (!aedificator || !s.datum)
		redde FALSUM;

	redde _appendere_interna(aedificator, s.datum, s.mensura);
}


/* ==================================================
 * Appendere - Numeri
 * ================================================== */

b32
chorda_aedificator_appendere_integer(ChordaAedificator* aedificator, s32 n)
{
	i8 buffer[CXXXII];
	memoriae_index mensura;

	si (!aedificator)
		redde FALSUM;

	mensura = _format_integer_s32(n, buffer, magnitudo(buffer));
	si (mensura == ZEPHYRUM)
		redde FALSUM;

	redde _appendere_interna(aedificator, buffer, mensura);
}

b32
chorda_aedificator_appendere_i32(ChordaAedificator* aedificator, i32 n)
{
	i8 buffer[CXXXII];
	memoriae_index mensura;

	si (!aedificator)
		redde FALSUM;

	mensura = _format_integer_i32(n, buffer, magnitudo(buffer));
	si (mensura == ZEPHYRUM)
		redde FALSUM;

	redde _appendere_interna(aedificator, buffer, mensura);
}

b32
chorda_aedificator_appendere_duplex(ChordaAedificator* aedificator, f64 n, i32 decimales)
{
	i8 buffer[CXXXII];
	memoriae_index mensura;

	si (!aedificator || decimales < ZEPHYRUM || decimales > XXX)
		redde FALSUM;

	mensura = _format_duplex(n, decimales, buffer, magnitudo(buffer));
	si (mensura == ZEPHYRUM)
		redde FALSUM;

	redde _appendere_interna(aedificator, buffer, mensura);
}


/* ==================================================
 * Appendere - Evasus
 * ================================================== */

b32
chorda_aedificator_appendere_evasus_json(ChordaAedificator* aedificator, chorda s)
{
	i8 buffer[D];  /* Buffer in acervo pro effugiis parvis */
	i8* output_buffer;
	memoriae_index mensura_evasus;
	memoriae_index necessaria;

	si (!aedificator || !s.datum)
		redde FALSUM;

	/* Pessimus casus: omnis character fit \uXXXX (6 bytes) */
	necessaria = s.mensura * VI;

	si (necessaria <= magnitudo(buffer))
	{
		output_buffer = buffer;
	}
	alioquin
	{
		output_buffer = (i8*)piscina_allocare(aedificator->piscina, necessaria);
		si (!output_buffer)
			redde FALSUM;
	}

	mensura_evasus = _evadere_json(s.datum, s.mensura, output_buffer, necessaria);
	si (mensura_evasus == ZEPHYRUM)
	{
		redde FALSUM;
	}

	redde _appendere_interna(aedificator, output_buffer, mensura_evasus);
}

b32
chorda_aedificator_appendere_literis_evasus_json(ChordaAedificator* aedificator, constans character* cstr)
{
	memoriae_index mensura;
	i8* buffer_temporalis;
	chorda s;
	b32 result;

	si (!cstr || !aedificator)
		redde FALSUM;

	mensura = strlen(cstr);
	buffer_temporalis = (i8*)piscina_allocare(aedificator->piscina, mensura);
	si (!buffer_temporalis)
		redde FALSUM;

	memcpy(buffer_temporalis, cstr, mensura);
	s.datum = buffer_temporalis;
	s.mensura = (i32)mensura;

	result = chorda_aedificator_appendere_evasus_json(aedificator, s);
	redde result;
}


/* ==================================================
 * Appendere - Spatium Album/Structura
 * ================================================== */

b32
chorda_aedificator_appendere_lineam_novam(ChordaAedificator* aedificator)
{
	si (!aedificator)
		redde FALSUM;

	redde chorda_aedificator_appendere_character(aedificator, '\n');
}

b32
chorda_aedificator_appendere_indentationem(ChordaAedificator* aedificator, i32 gradus)
{
	i32 i;
	i32 spatia;

	si (!aedificator || gradus < ZEPHYRUM)
		redde FALSUM;

	spatia = gradus * CHORDA_AEDIFICATOR_INDENTATIO_SPATIA;

	per (i = ZEPHYRUM; i < spatia; i++)
	{
		si (!chorda_aedificator_appendere_character(aedificator, ' '))
			redde FALSUM;
	}

	redde VERUM;
}


/* ==================================================
 * Indentationis Observatio
 * ================================================== */

vacuum
chorda_aedificator_push_indentationem(ChordaAedificator* aedificator)
{
	si (aedificator && aedificator->indentatio_gradus < M)
	{
		aedificator->indentatio_gradus++;
	}
}

vacuum
chorda_aedificator_pop_indentationem(ChordaAedificator* aedificator)
{
	si (aedificator && aedificator->indentatio_gradus > ZEPHYRUM)
	{
		aedificator->indentatio_gradus--;
	}
}

i32
chorda_aedificator_indentatio_gradus(ChordaAedificator* aedificator)
{
	redde aedificator ? aedificator->indentatio_gradus : ZEPHYRUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

memoriae_index
chorda_aedificator_longitudo(ChordaAedificator* aedificator)
{
	redde aedificator ? aedificator->offset : ZEPHYRUM;
}

chorda
chorda_aedificator_spectare(ChordaAedificator* aedificator)
{
	chorda result;

	si (!aedificator || !aedificator->buffer)
	{
		result.mensura = ZEPHYRUM;
		result.datum = NIHIL;
	}
	alioquin
	{
		result.mensura = (i32)aedificator->offset;
		result.datum = aedificator->buffer;
	}

	redde result;
}


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

vacuum
chorda_aedificator_reset(ChordaAedificator* aedificator)
{
	si (!aedificator)
		redde;

	aedificator->offset = ZEPHYRUM;
	aedificator->indentatio_gradus = ZEPHYRUM;
}

chorda
chorda_aedificator_finire(ChordaAedificator* aedificator)
{
	chorda result;

	si (!aedificator)
	{
		result.mensura = ZEPHYRUM;
		result.datum = NIHIL;
		redde result;
	}

	result.mensura = (i32)aedificator->offset;
	result.datum = aedificator->buffer;

	redde result;
}
