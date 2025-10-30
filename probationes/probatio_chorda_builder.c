/* probatio_chorda_builder.c - Probationes ChordaBuilder */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "chorda_builder.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
	Piscina* piscina;
	b32  praeteritus;

	/* Aperire piscinam et credonem */
	piscina = piscina_generare_dynamicum("probatio_chorda_builder", MM);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ==================================================
	 * Probare chorda_builder_creare
	 * ================================================== */

	{
		ChordaBuilder* builder;

		imprimere("\n--- Probans chorda_builder_creare ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(builder);

		CREDO_AEQUALIS_I32((i32)chorda_builder_longitudo(builder), ZEPHYRUM);
		CREDO_AEQUALIS_I32(chorda_builder_indentation_level(builder), ZEPHYRUM);

		chorda_builder_destruere(builder);
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_character
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result;

		imprimere("\n--- Probans chorda_builder_appendere_character ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_character(builder, 'H'));
		CREDO_VERUM(chorda_builder_appendere_character(builder, 'i'));

		CREDO_AEQUALIS_I32((i32)chorda_builder_longitudo(builder), II);

		result = chorda_builder_finire(builder);
		CREDO_AEQUALIS_I32(result.mensura, II);
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_literis
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result, speratus;

		imprimere("\n--- Probans chorda_builder_appendere_literis ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "salve"));
		CREDO_VERUM(chorda_builder_appendere_literis(builder, " "));
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "munde"));

		result = chorda_builder_finire(builder);
		speratus = chorda_ex_literis("salve munde", piscina);

		CREDO_VERUM(chorda_aequalis(result, speratus));
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_chorda
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda s1, s2, result, speratus;

		imprimere("\n--- Probans chorda_builder_appendere_chorda ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		s1 = chorda_ex_literis("prefix", piscina);
		s2 = chorda_ex_literis("suffix", piscina);

		CREDO_VERUM(chorda_builder_appendere_chorda(builder, s1));
		CREDO_VERUM(chorda_builder_appendere_chorda(builder, s2));

		result = chorda_builder_finire(builder);
		speratus = chorda_ex_literis("prefixsuffix", piscina);

		CREDO_VERUM(chorda_aequalis(result, speratus));
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_integer
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result, speratus;

		imprimere("\n--- Probans chorda_builder_appendere_integer ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "answer="));
		CREDO_VERUM(chorda_builder_appendere_integer(builder, XLII));

		result = chorda_builder_finire(builder);
		speratus = chorda_ex_literis("answer=42", piscina);

		CREDO_VERUM(chorda_aequalis(result, speratus));
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_i32
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result, speratus;

		imprimere("\n--- Probans chorda_builder_appendere_i32 ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "count="));
		CREDO_VERUM(chorda_builder_appendere_i32(builder, CCLVI));

		result = chorda_builder_finire(builder);
		speratus = chorda_ex_literis("count=256", piscina);

		CREDO_VERUM(chorda_aequalis(result, speratus));
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_duplex
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result;

		imprimere("\n--- Probans chorda_builder_appendere_duplex ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "pi="));
		CREDO_VERUM(chorda_builder_appendere_duplex(builder, 3.14159, II));

		result = chorda_builder_finire(builder);
		CREDO_AEQUALIS_I32(result.mensura > VI, VERUM);  /* At least "pi=3.14" */
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_newline
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result;

		imprimere("\n--- Probans chorda_builder_appendere_newline ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "linea1"));
		CREDO_VERUM(chorda_builder_appendere_newline(builder));
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "linea2"));

		result = chorda_builder_finire(builder);
		/* Should have newline in middle */
		CREDO_VERUM(result.mensura > XII);
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_indentation
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result;

		imprimere("\n--- Probans chorda_builder_appendere_indentation ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		/* Level 0: no indent */
		CREDO_VERUM(chorda_builder_appendere_indentation(builder, ZEPHYRUM));
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "no_indent"));

		CREDO_VERUM(chorda_builder_appendere_newline(builder));
		/* Level 1: 2 spaces */
		CREDO_VERUM(chorda_builder_appendere_indentation(builder, I));
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "indented"));

		result = chorda_builder_finire(builder);
		CREDO_AEQUALIS_I32(result.mensura > CXXVIII, FALSUM);
	}


	/* ==================================================
	 * Probare chorda_builder_push/pop_indentation
	 * ================================================== */

	{
		ChordaBuilder* builder;
		i32 level1, level2, level3;

		imprimere("\n--- Probans chorda_builder_push/pop_indentation ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);

		level1 = chorda_builder_indentation_level(builder);
		CREDO_AEQUALIS_I32(level1, ZEPHYRUM);

		chorda_builder_push_indentation(builder);
		level2 = chorda_builder_indentation_level(builder);
		CREDO_AEQUALIS_I32(level2, I);

		chorda_builder_push_indentation(builder);
		level3 = chorda_builder_indentation_level(builder);
		CREDO_AEQUALIS_I32(level3, II);

		chorda_builder_pop_indentation(builder);
		level2 = chorda_builder_indentation_level(builder);
		CREDO_AEQUALIS_I32(level2, I);

		chorda_builder_pop_indentation(builder);
		level1 = chorda_builder_indentation_level(builder);
		CREDO_AEQUALIS_I32(level1, ZEPHYRUM);

		chorda_builder_destruere(builder);
	}


	/* ==================================================
	 * Probare chorda_builder_spectare
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda view;

		imprimere("\n--- Probans chorda_builder_spectare ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "test"));

		view = chorda_builder_spectare(builder);
		CREDO_AEQUALIS_I32((i32)view.mensura, IV);

		/* Spectare shouldn't finalize, builder still valid */
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "123"));
		CREDO_AEQUALIS_I32((i32)chorda_builder_longitudo(builder), VII);

		chorda_builder_destruere(builder);
	}


	/* ==================================================
	 * Probare chorda_builder_reset
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result1, result2, speratus2;

		imprimere("\n--- Probans chorda_builder_reset ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);

		CREDO_VERUM(chorda_builder_appendere_literis(builder, "first_string"));
		CREDO_AEQUALIS_I32((i32)chorda_builder_longitudo(builder), XII);

		result1 = chorda_builder_finire(builder);
		CREDO_AEQUALIS_I32(result1.mensura, XII);

		/* Can't reset after finire, but can create new builder */
		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "second"));
		CREDO_AEQUALIS_I32((i32)chorda_builder_longitudo(builder), VI);

		result2 = chorda_builder_finire(builder);
		speratus2 = chorda_ex_literis("second", piscina);
		CREDO_VERUM(chorda_aequalis(result2, speratus2));
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_escaped_json
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda input, result;

		imprimere("\n--- Probans chorda_builder_appendere_escaped_json ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_character(builder, '"'));

		input = chorda_ex_literis("hello\"world", piscina);
		CREDO_VERUM(chorda_builder_appendere_escaped_json(builder, input));

		CREDO_VERUM(chorda_builder_appendere_character(builder, '"'));

		result = chorda_builder_finire(builder);
		/* Should have escaped the internal quote */
		CREDO_VERUM(result.mensura > XII);
	}


	/* ==================================================
	 * Probare chorda_builder_appendere_literis_escaped_json
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result;

		imprimere("\n--- Probans chorda_builder_appendere_literis_escaped_json ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_builder_appendere_character(builder, '"'));

		CREDO_VERUM(chorda_builder_appendere_literis_escaped_json(
			builder, "test\\value"));

		CREDO_VERUM(chorda_builder_appendere_character(builder, '"'));

		result = chorda_builder_finire(builder);
		/* Should have escaped the backslash */
		CREDO_VERUM(result.mensura > VI);
	}


	/* ==================================================
	 * Probare capacity growth
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result;
		i32 i;
		character buffer[VII];

		imprimere("\n--- Probans capacity_crescentia ---\n");

		builder = chorda_builder_creare(piscina, XVI);  /* Start small */

		/* Append enough to force growth */
		per (i = ZEPHYRUM; i < XX; i++)
		{
			snprintf(buffer, VII, "k%d,", i);
			CREDO_VERUM(chorda_builder_appendere_literis(builder, buffer));
		}

		result = chorda_builder_finire(builder);
		/* Should have accumulated all entries (XX × ~3-4 bytes) */
		CREDO_VERUM(result.mensura > XXXII);
	}


	/* ==================================================
	 * Probare combined operations (JSON-like structure)
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result;

		imprimere("\n--- Probans combined_operations (JSON pattern) ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);

		CREDO_VERUM(chorda_builder_appendere_literis(builder, "{"));
		CREDO_VERUM(chorda_builder_appendere_newline(builder));

		chorda_builder_push_indentation(builder);
		CREDO_VERUM(chorda_builder_appendere_indentation(builder,
			chorda_builder_indentation_level(builder)));
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "\"name\":"));
		CREDO_VERUM(chorda_builder_appendere_character(builder, ' '));
		CREDO_VERUM(chorda_builder_appendere_character(builder, '"'));
		CREDO_VERUM(chorda_builder_appendere_literis_escaped_json(
			builder, "test"));
		CREDO_VERUM(chorda_builder_appendere_character(builder, '"'));
		CREDO_VERUM(chorda_builder_appendere_character(builder, ','));
		CREDO_VERUM(chorda_builder_appendere_newline(builder));

		CREDO_VERUM(chorda_builder_appendere_indentation(builder,
			chorda_builder_indentation_level(builder)));
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "\"value\":"));
		CREDO_VERUM(chorda_builder_appendere_character(builder, ' '));
		CREDO_VERUM(chorda_builder_appendere_integer(builder, XLII));
		CREDO_VERUM(chorda_builder_appendere_newline(builder));

		chorda_builder_pop_indentation(builder);
		CREDO_VERUM(chorda_builder_appendere_literis(builder, "}"));

		result = chorda_builder_finire(builder);
		/* Simple JSON object: ~40 bytes */
		CREDO_VERUM(result.mensura > XXX);
	}


	/* ==================================================
	 * Probare NULL input handling
	 * ================================================== */

	{
		ChordaBuilder* builder;

		imprimere("\n--- Probans tutelae initus NIHIL ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);

		/* appendere_literis with NULL cstr should be safe */
		CREDO_FALSUM(chorda_builder_appendere_literis(builder, NIHIL));

		chorda_builder_destruere(builder);
	}


	/* ==================================================
	 * Probare large strings
	 * ================================================== */

	{
		ChordaBuilder* builder;
		chorda result;
		i32 i;

		imprimere("\n--- Probans large_strings ---\n");

		builder = chorda_builder_creare(piscina, CXXVIII);

		/* Append 256 times to build large string */
		per (i = ZEPHYRUM; i < CCLVI; i++)
		{
			CREDO_VERUM(chorda_builder_appendere_character(builder, 'x'));
		}

		result = chorda_builder_finire(builder);
		CREDO_AEQUALIS_I32((i32)result.mensura, CCLVI);
	}


	/* ==================================================
	 * Compendium
	 * ================================================== */

	imprimere("\n");
	credo_imprimere_compendium();

	praeteritus = credo_omnia_praeterierunt();

	/* Purificatio */
	credo_claudere();
	piscina_destruere(piscina);

	si (praeteritus)
	{
		redde ZEPHYRUM;
	}
	alioquin
	{
		redde I;
	}
}
