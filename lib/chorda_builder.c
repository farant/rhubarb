#include "chorda_builder.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ==================================================
 * Structura ChordaBuilder - Interna
 * ================================================== */

structura ChordaBuilder {
	           i8*  buffer;
	memoriae_index  capacitas;
	memoriae_index  offset;
	Piscina*  piscina;
	i32 indentatio_level;
};


/* ==================================================
 * ADIUTORES INTERNI
 * ================================================== */

interior memoriae_index
_proxima_capacitas(memoriae_index nunc)
{
	/* Double capacity until we have enough */
	redde nunc > ZEPHYRUM ? nunc * II : XVI;
}

interior b32
_crescere(ChordaBuilder* builder, memoriae_index necessaria)
{
	memoriae_index capacitas_nova;
	i8* buffer_novum;

	capacitas_nova = builder->capacitas;
	dum (capacitas_nova < necessaria)
	{
		capacitas_nova = _proxima_capacitas(capacitas_nova);
	}

	buffer_novum = (i8*)piscina_allocare(builder->piscina, capacitas_nova);
	si (!buffer_novum)
		redde FALSUM;

	si (builder->buffer && builder->offset > ZEPHYRUM)
	{
		memcpy(buffer_novum, builder->buffer, builder->offset);
	}

	builder->buffer = buffer_novum;
	builder->capacitas = capacitas_nova;

	redde VERUM;
}

interior b32
_appendere_interna(ChordaBuilder* builder, constans i8* datum, memoriae_index mensura)
{
	memoriae_index necessaria;

	si (!builder || !datum || mensura == ZEPHYRUM)
		redde mensura == ZEPHYRUM; /* Empty append is OK */

	necessaria = builder->offset + mensura;

	si (necessaria > builder->capacitas)
	{
		si (!_crescere(builder, necessaria))
			redde FALSUM;
	}

	memcpy(builder->buffer + builder->offset, datum, mensura);
	builder->offset += mensura;

	redde VERUM;
}

/* Format signed integer to buffer
 * Caller provides buffer, must be >= 32 bytes (safe for s32) */
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

/* JSON string escaping
 * Handles: quote, backslash, control characters
 * Caller provides buffer */
interior memoriae_index
_escape_json(constans i8* input, memoriae_index mensura, i8* output, memoriae_index capacitas_output)
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
			/* Other control characters as \uXXXX */
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

ChordaBuilder*
chorda_builder_creare(Piscina* piscina, memoriae_index capacitas_initialis)
{
	ChordaBuilder* builder;
	i8* buffer;

	si (!piscina || capacitas_initialis == ZEPHYRUM)
		redde NIHIL;

	builder = (ChordaBuilder*)piscina_allocare(piscina, magnitudo(ChordaBuilder));
	si (!builder)
		redde NIHIL;

	buffer = (i8*)piscina_allocare(piscina, capacitas_initialis);
	si (!buffer)
		redde NIHIL;

	builder->buffer = buffer;
	builder->capacitas = capacitas_initialis;
	builder->offset = ZEPHYRUM;
	builder->piscina = piscina;
	builder->indentatio_level = ZEPHYRUM;

	redde builder;
}


/* ==================================================
 * Destructio
 * ================================================== */

vacuum
chorda_builder_destruere(ChordaBuilder* builder)
{
	/* Piscina owns the memory; we just release the struct */
	si (builder)
	{
		/* Note: buffer is also allocated from piscina,
		 * so it will be freed when piscina is destroyed */
	}
}


/* ==================================================
 * Appendere - Character
 * ================================================== */

b32
chorda_builder_appendere_character(ChordaBuilder* builder, character c)
{
	i8 ch = (i8)c;
	redde _appendere_interna(builder, &ch, I);
}


/* ==================================================
 * Appendere - Strings
 * ================================================== */

b32
chorda_builder_appendere_literis(ChordaBuilder* builder, constans character* cstr)
{
	memoriae_index mensura;

	si (!builder || !cstr)
		redde FALSUM;

	mensura = strlen(cstr);
	redde _appendere_interna(builder, (constans i8*)cstr, mensura);
}

b32
chorda_builder_appendere_chorda(ChordaBuilder* builder, chorda s)
{
	si (!builder || !s.datum)
		redde FALSUM;

	redde _appendere_interna(builder, s.datum, s.mensura);
}


/* ==================================================
 * Appendere - Numbers
 * ================================================== */

b32
chorda_builder_appendere_integer(ChordaBuilder* builder, s32 n)
{
	i8 buffer[CXXXII];
	memoriae_index mensura;

	si (!builder)
		redde FALSUM;

	mensura = _format_integer_s32(n, buffer, magnitudo(buffer));
	si (mensura == ZEPHYRUM)
		redde FALSUM;

	redde _appendere_interna(builder, buffer, mensura);
}

b32
chorda_builder_appendere_i32(ChordaBuilder* builder, i32 n)
{
	i8 buffer[CXXXII];
	memoriae_index mensura;

	si (!builder)
		redde FALSUM;

	mensura = _format_integer_i32(n, buffer, magnitudo(buffer));
	si (mensura == ZEPHYRUM)
		redde FALSUM;

	redde _appendere_interna(builder, buffer, mensura);
}

b32
chorda_builder_appendere_duplex(ChordaBuilder* builder, f64 n, i32 decimales)
{
	i8 buffer[CXXXII];
	memoriae_index mensura;

	si (!builder || decimales < ZEPHYRUM || decimales > XXX)
		redde FALSUM;

	mensura = _format_duplex(n, decimales, buffer, magnitudo(buffer));
	si (mensura == ZEPHYRUM)
		redde FALSUM;

	redde _appendere_interna(builder, buffer, mensura);
}


/* ==================================================
 * Appendere - Escaped
 * ================================================== */

b32
chorda_builder_appendere_escaped_json(ChordaBuilder* builder, chorda s)
{
	i8 buffer[D];  /* Stack buffer for small escapes */
	i8* output_buffer;
	memoriae_index mensura_escapatus;
	memoriae_index necessaria;

	si (!builder || !s.datum)
		redde FALSUM;

	/* Worst case: every char becomes \uXXXX (6 bytes) */
	necessaria = s.mensura * VI;

	si (necessaria <= magnitudo(buffer))
	{
		output_buffer = buffer;
	}
	alioquin
	{
		output_buffer = (i8*)piscina_allocare(builder->piscina, necessaria);
		si (!output_buffer)
			redde FALSUM;
	}

	mensura_escapatus = _escape_json(s.datum, s.mensura, output_buffer, necessaria);
	si (mensura_escapatus == ZEPHYRUM)
	{
		redde FALSUM;
	}

	redde _appendere_interna(builder, output_buffer, mensura_escapatus);
}

b32
chorda_builder_appendere_literis_escaped_json(ChordaBuilder* builder, constans character* cstr)
{
	memoriae_index mensura;
	i8* buffer_temporalis;
	chorda s;
	b32 result;

	si (!cstr || !builder)
		redde FALSUM;

	mensura = strlen(cstr);
	buffer_temporalis = (i8*)piscina_allocare(builder->piscina, mensura);
	si (!buffer_temporalis)
		redde FALSUM;

	memcpy(buffer_temporalis, cstr, mensura);
	s.datum = buffer_temporalis;
	s.mensura = (i32)mensura;

	result = chorda_builder_appendere_escaped_json(builder, s);
	redde result;
}


/* ==================================================
 * Appendere - Whitespace/Structure
 * ================================================== */

b32
chorda_builder_appendere_newline(ChordaBuilder* builder)
{
	si (!builder)
		redde FALSUM;

	redde chorda_builder_appendere_character(builder, '\n');
}

b32
chorda_builder_appendere_indentation(ChordaBuilder* builder, i32 level)
{
	i32 i;
	i32 spatia;

	si (!builder || level < ZEPHYRUM)
		redde FALSUM;

	spatia = level * CHORDA_BUILDER_INDENTATIO_SPATIA;

	per (i = ZEPHYRUM; i < spatia; i++)
	{
		si (!chorda_builder_appendere_character(builder, ' '))
			redde FALSUM;
	}

	redde VERUM;
}


/* ==================================================
 * Indentation Tracking
 * ================================================== */

vacuum
chorda_builder_push_indentation(ChordaBuilder* builder)
{
	si (builder && builder->indentatio_level < M)
	{
		builder->indentatio_level++;
	}
}

vacuum
chorda_builder_pop_indentation(ChordaBuilder* builder)
{
	si (builder && builder->indentatio_level > ZEPHYRUM)
	{
		builder->indentatio_level--;
	}
}

i32
chorda_builder_indentation_level(ChordaBuilder* builder)
{
	redde builder ? builder->indentatio_level : ZEPHYRUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

memoriae_index
chorda_builder_longitudo(ChordaBuilder* builder)
{
	redde builder ? builder->offset : ZEPHYRUM;
}

chorda
chorda_builder_spectare(ChordaBuilder* builder)
{
	chorda result;

	si (!builder || !builder->buffer)
	{
		result.mensura = ZEPHYRUM;
		result.datum = NIHIL;
	}
	alioquin
	{
		result.mensura = (i32)builder->offset;
		result.datum = builder->buffer;
	}

	redde result;
}


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

vacuum
chorda_builder_reset(ChordaBuilder* builder)
{
	si (!builder)
		redde;

	builder->offset = ZEPHYRUM;
	builder->indentatio_level = ZEPHYRUM;
}

chorda
chorda_builder_finire(ChordaBuilder* builder)
{
	chorda result;

	si (!builder)
	{
		result.mensura = ZEPHYRUM;
		result.datum = NIHIL;
		redde result;
	}

	result.mensura = (i32)builder->offset;
	result.datum = builder->buffer;

	redde result;
}
