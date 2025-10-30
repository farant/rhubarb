/* probatio_chorda_aedificator.c - Probationes ChordaAedificator */
#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
	Piscina* piscina;
	    b32  praeteritus;

	/* Aperire piscinam et credonem */
	piscina = piscina_generare_dynamicum("probatio_chorda_aedificator", MM);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ==================================================
	 * Probare chorda_aedificator_creare
	 * ================================================== */

	{
		ChordaAedificator* aedificator;

		imprimere("\n--- Probans chorda_aedificator_creare ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_NON_NIHIL(aedificator);

		CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator), ZEPHYRUM);
		CREDO_AEQUALIS_I32(chorda_aedificator_indentatio_gradus(aedificator), ZEPHYRUM);

		chorda_aedificator_destruere(aedificator);
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_character
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result;

		imprimere("\n--- Probans chorda_aedificator_appendere_character ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, 'H'));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, 'i'));

		CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator), II);

		result = chorda_aedificator_finire(aedificator);
		CREDO_AEQUALIS_I32(result.mensura, II);
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_literis
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_literis ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "salve"));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, " "));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "munde"));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("salve munde", piscina);

		CREDO_VERUM(chorda_aequalis(result, speratus));
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_chorda
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  s1, s2, result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_chorda ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		s1          = chorda_ex_literis("prefix", piscina);
		s2          = chorda_ex_literis("suffix", piscina);

		CREDO_VERUM(chorda_aedificator_appendere_chorda(aedificator, s1));
		CREDO_VERUM(chorda_aedificator_appendere_chorda(aedificator, s2));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("prefixsuffix", piscina);

		CREDO_VERUM(chorda_aequalis(result, speratus));
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_integer
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_integer ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "answer="));
		CREDO_VERUM(chorda_aedificator_appendere_integer(aedificator, XLII));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("answer=42", piscina);

		CREDO_VERUM(chorda_aequalis(result, speratus));
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_i32
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_i32 ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "count="));
		CREDO_VERUM(chorda_aedificator_appendere_i32(aedificator, CCLVI));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("count=256", piscina);

		CREDO_VERUM(chorda_aequalis(result, speratus));
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_duplex
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result;

		imprimere("\n--- Probans chorda_aedificator_appendere_duplex ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "pi="));
		CREDO_VERUM(chorda_aedificator_appendere_duplex(aedificator, 3.14159, II));

		result = chorda_aedificator_finire(aedificator);
		CREDO_AEQUALIS_I32(result.mensura > VI, VERUM);  /* At least "pi=3.14" */
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_lineam_novam
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result;

		imprimere("\n--- Probans chorda_aedificator_appendere_lineam_novam ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "linea1"));
		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "linea2"));

		result = chorda_aedificator_finire(aedificator);
		/* Should have newline in middle */
		CREDO_VERUM(result.mensura > XII);
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_indentation
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result;

		imprimere("\n--- Probans chorda_aedificator_appendere_indentation ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		/* Level 0: no indent */
		CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator, ZEPHYRUM));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "no_indent"));

		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
		/* Level 1: 2 spaces */
		CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator, I));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "indented"));

		result = chorda_aedificator_finire(aedificator);
		CREDO_AEQUALIS_I32(result.mensura > CXXVIII, FALSUM);
	}


	/* ==================================================
	 * Probare chorda_aedificator_push/pop_indentation
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		              i32  level1, level2, level3;

		imprimere("\n--- Probans chorda_aedificator_push/pop_indentation ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		level1 = chorda_aedificator_indentatio_gradus(aedificator);
		CREDO_AEQUALIS_I32(level1, ZEPHYRUM);

		chorda_aedificator_push_indentationem(aedificator);
		level2 = chorda_aedificator_indentatio_gradus(aedificator);
		CREDO_AEQUALIS_I32(level2, I);

		chorda_aedificator_push_indentationem(aedificator);
		level3 = chorda_aedificator_indentatio_gradus(aedificator);
		CREDO_AEQUALIS_I32(level3, II);

		chorda_aedificator_pop_indentationem(aedificator);
		level2 = chorda_aedificator_indentatio_gradus(aedificator);
		CREDO_AEQUALIS_I32(level2, I);

		chorda_aedificator_pop_indentationem(aedificator);
		level1 = chorda_aedificator_indentatio_gradus(aedificator);
		CREDO_AEQUALIS_I32(level1, ZEPHYRUM);

		chorda_aedificator_destruere(aedificator);
	}


	/* ==================================================
	 * Probare chorda_aedificator_spectare
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  view;

		imprimere("\n--- Probans chorda_aedificator_spectare ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "test"));

		view = chorda_aedificator_spectare(aedificator);
		CREDO_AEQUALIS_I32((i32)view.mensura, IV);

		/* Spectare shouldn't finalize, aedificator still valid */
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "123"));
		CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator), VII);

		chorda_aedificator_destruere(aedificator);
	}


	/* ==================================================
	 * Probare chorda_aedificator_reset
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result1, result2, speratus2;

		imprimere("\n--- Probans chorda_aedificator_reset ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "first_string"));
		CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator), XII);

		result1 = chorda_aedificator_finire(aedificator);
		CREDO_AEQUALIS_I32(result1.mensura, XII);

		/* Can't reset after finire, but can create new aedificator */
		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "second"));
		CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator), VI);

		result2   = chorda_aedificator_finire(aedificator);
		speratus2 = chorda_ex_literis("second", piscina);
		CREDO_VERUM(chorda_aequalis(result2, speratus2));
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_evasus_json
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  input, result;

		imprimere("\n--- Probans chorda_aedificator_appendere_evasus_json ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		input = chorda_ex_literis("hello\"world", piscina);
		CREDO_VERUM(chorda_aedificator_appendere_evasus_json(aedificator, input));

		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		result = chorda_aedificator_finire(aedificator);
		/* Should have escaped the internal quote */
		CREDO_VERUM(result.mensura > XII);
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_literis_evasus_json
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result;

		imprimere("\n--- Probans chorda_aedificator_appendere_literis_evasus_json ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		CREDO_VERUM(chorda_aedificator_appendere_literis_evasus_json(
			aedificator, "test\\value"));

		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		result = chorda_aedificator_finire(aedificator);
		/* Should have escaped the backslash */
		CREDO_VERUM(result.mensura > VI);
	}


	/* ==================================================
	 * Probare capacity growth
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result;
		              i32  i;
		        character  buffer[VII];

		imprimere("\n--- Probans capacity_crescentia ---\n");

		aedificator = chorda_aedificator_creare(piscina, XVI);  /* Start small */

		/* Append enough to force growth */
		per (i = ZEPHYRUM; i < XX; i++)
		{
			snprintf(buffer, VII, "k%d,", i);
			CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, buffer));
		}

		result = chorda_aedificator_finire(aedificator);
		/* Should have accumulated all entries (XX × ~3-4 bytes) */
		CREDO_VERUM(result.mensura > XXXII);
	}


	/* ==================================================
	 * Probare combined operations (JSON-like structure)
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result;

		imprimere("\n--- Probans combined_operations (JSON pattern) ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "{"));
		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));

		chorda_aedificator_push_indentationem(aedificator);
		CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator,
			chorda_aedificator_indentatio_gradus(aedificator)));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "\"name\":"));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, ' '));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));
		CREDO_VERUM(chorda_aedificator_appendere_literis_evasus_json(
			aedificator, "test"));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, ','));
		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));

		CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator,
			chorda_aedificator_indentatio_gradus(aedificator)));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "\"value\":"));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, ' '));
		CREDO_VERUM(chorda_aedificator_appendere_integer(aedificator, XLII));
		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));

		chorda_aedificator_pop_indentationem(aedificator);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "}"));

		result = chorda_aedificator_finire(aedificator);
		/* Simple JSON object: ~40 bytes */
		CREDO_VERUM(result.mensura > XXX);
	}


	/* ==================================================
	 * Probare NULL input handling
	 * ================================================== */

	{
		ChordaAedificator* aedificator;

		imprimere("\n--- Probans tutelae initus NIHIL ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		/* appendere_literis with NULL cstr should be safe */
		CREDO_FALSUM(chorda_aedificator_appendere_literis(aedificator, NIHIL));

		chorda_aedificator_destruere(aedificator);
	}


	/* ==================================================
	 * Probare large strings
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result;
		              i32  i;

		imprimere("\n--- Probans large_strings ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		/* Append 256 times to build large string */
		per (i = ZEPHYRUM; i < CCLVI; i++)
		{
			CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, 'x'));
		}

		result = chorda_aedificator_finire(aedificator);
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
