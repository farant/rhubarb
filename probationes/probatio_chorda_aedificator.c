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
	 * Probare chorda_aedificator_appendere_s32
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_s32 ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "answer="));
		CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator, XLII));

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
	 * Probare chorda_aedificator_appendere_f64
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_f64 ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "pi="));
		CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator, 3.14159, II));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("pi=3.14", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_lineam_novam
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_lineam_novam ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "primus"));
		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "secundus"));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("primus\nsecundus", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_indentation
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_indentation ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		/* Gradus 0: nullum indentationem */
		CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator, ZEPHYRUM));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "no_indent"));

		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
		/* Gradus 1: duo spatia */
		CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator, I));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "indented"));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("no_indent\n  indented", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
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

		/* Spectare non debet finire, aedificator adhuc validus */
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

		/* Non potest reficere post finire, sed potest creare aedificatorem novum */
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
		           chorda  input, result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_evasus_json ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		input = chorda_ex_literis("hello\"world", piscina);
		CREDO_VERUM(chorda_aedificator_appendere_evasus_json(aedificator, input));

		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("\"hello\\\"world\"", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare chorda_aedificator_appendere_literis_evasus_json
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans chorda_aedificator_appendere_literis_evasus_json ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		CREDO_VERUM(chorda_aedificator_appendere_literis_evasus_json(
			aedificator, "test\\value"));

		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("\"test\\\\value\"", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
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

		/* Appendere satis ut crescentiam cogat */
		per (i = ZEPHYRUM; i < XX; i++)
		{
			snprintf(buffer, VII, "k%d,", i);
			CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, buffer));
		}

		result = chorda_aedificator_finire(aedificator);
		/* Debet accumulavisse omnes introitus (XX × ~3-4 bytes) */
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
		CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator, XLII));
		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));

		chorda_aedificator_pop_indentationem(aedificator);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "}"));

		result = chorda_aedificator_finire(aedificator);
		/* Obiectum JSON simplex: ~40 bytes */
		CREDO_VERUM(result.mensura > XXX);
	}


	/* ==================================================
	 * Probare NIHIL input handling
	 * ================================================== */

	{
		ChordaAedificator* aedificator;

		imprimere("\n--- Probans tutelae initus NIHIL ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		/* appendere_literis cum cstr NIHIL debet esse tutum */
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

		/* Appendere CCLVI tempora ad aedificandam chordam magnam */
		per (i = ZEPHYRUM; i < CCLVI; i++)
		{
			CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, 'x'));
		}

		result = chorda_aedificator_finire(aedificator);
		CREDO_AEQUALIS_I32((i32)result.mensura, CCLVI);
	}


	/* ==================================================
	 * Probare lineam novam contentum verificatio
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans lineam_novam_contentum ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "linea1"));
		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "linea2"));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("linea1\nlinea2", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare numeri negativi et zephyrum
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans numeri_negativi_et_zephyrum ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator, -XLII));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, ','));
		CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator, ZEPHYRUM));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, ','));
		CREDO_VERUM(chorda_aedificator_appendere_s32(aedificator, XLII));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("-42,0,42", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare omnes evasus JSON
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  input, result, speratus;

		imprimere("\n--- Probans omnes_evasus_json ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		input = chorda_ex_literis("test\n\r\t\"\\end", piscina);
		CREDO_VERUM(chorda_aedificator_appendere_evasus_json(aedificator, input));

		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, '"'));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("\"test\\n\\r\\t\\\"\\\\end\"", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare reset verum
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans reset_verum ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "first"));
		CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator), V);

		chorda_aedificator_reset(aedificator);
		CREDO_AEQUALIS_I32((i32)chorda_aedificator_longitudo(aedificator), ZEPHYRUM);

		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "second"));
		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("second", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);

		chorda_aedificator_destruere(aedificator);
	}


	/* ==================================================
	 * Probare spectare multiplex
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  view1, view2, view3;

		imprimere("\n--- Probans spectare_multiplex ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		view1 = chorda_aedificator_spectare(aedificator);
		CREDO_AEQUALIS_I32((i32)view1.mensura, ZEPHYRUM);

		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "a"));
		view2 = chorda_aedificator_spectare(aedificator);
		CREDO_AEQUALIS_I32((i32)view2.mensura, I);

		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "bc"));
		view3 = chorda_aedificator_spectare(aedificator);
		CREDO_AEQUALIS_I32((i32)view3.mensura, III);

		chorda_aedificator_destruere(aedificator);
	}


	/* ==================================================
	 * Probare indentatio cum verificatione
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans indentatio_cum_verificatione ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "a"));
		CREDO_VERUM(chorda_aedificator_appendere_lineam_novam(aedificator));
		CREDO_VERUM(chorda_aedificator_appendere_indentationem(aedificator, II));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "b"));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("a\n    b", piscina);  /* IV spatia pro gradus II */

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare f64 decimales varii
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;

		imprimere("\n--- Probans f64_decimales_varii ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator, 3.14159, ZEPHYRUM));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, ','));
		CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator, -2.5, I));
		CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, ','));
		CREDO_VERUM(chorda_aedificator_appendere_f64(aedificator, 1.23456789, V));

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("3,-2.5,1.23457", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare contentum post crescentiam
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, speratus;
		              i32  i;

		imprimere("\n--- Probans contentum_post_crescentiam ---\n");

		aedificator = chorda_aedificator_creare(piscina, VIII);  /* Capacitas parva */

		/* Aedificare formam notam quae crescentias multiplas coget */
		per (i = ZEPHYRUM; i < X; i++)
		{
			CREDO_VERUM(chorda_aedificator_appendere_character(aedificator, 'x'));
		}

		result   = chorda_aedificator_finire(aedificator);
		speratus = chorda_ex_literis("xxxxxxxxxx", piscina);

		CREDO_CHORDA_AEQUALIS(result, speratus);
	}


	/* ==================================================
	 * Probare indentatio limites
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		              i32  level;

		imprimere("\n--- Probans indentatio_limites ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);

		/* Pop ad gradum zero tutum */
		chorda_aedificator_pop_indentationem(aedificator);
		level = chorda_aedificator_indentatio_gradus(aedificator);
		CREDO_AEQUALIS_I32(level, ZEPHYRUM);

		/* Push ad maximum M */
		chorda_aedificator_push_indentationem(aedificator);
		CREDO_AEQUALIS_I32(chorda_aedificator_indentatio_gradus(aedificator), I);

		chorda_aedificator_destruere(aedificator);
	}


	/* ==================================================
	 * Probare chorda continet
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, needle;

		imprimere("\n--- Probans chorda_continet ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "Hello "));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "beautiful "));
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "world"));

		result = chorda_aedificator_finire(aedificator);
		needle = chorda_ex_literis("beautiful", piscina);

		CREDO_CHORDA_CONTINET(result, needle);
	}


	/* ==================================================
	 * Probare chorda incipit et terminatur
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		           chorda  result, prefixum, suffixum;

		imprimere("\n--- Probans chorda_incipit_et_terminatur ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "prefix_middle_suffix"));

		result   = chorda_aedificator_finire(aedificator);
		prefixum = chorda_ex_literis("prefix", piscina);
		suffixum = chorda_ex_literis("suffix", piscina);

		CREDO_CHORDA_INCIPIT(result, prefixum);
		CREDO_CHORDA_TERMINATUR(result, suffixum);
	}


	/* ==================================================
	 * Probare comparationes integrae
	 * ================================================== */

	{
		ChordaAedificator* aedificator;
		     memoriae_index  longitudo;

		imprimere("\n--- Probans comparationes_integrae ---\n");

		aedificator = chorda_aedificator_creare(piscina, CXXVIII);
		CREDO_VERUM(chorda_aedificator_appendere_literis(aedificator, "test"));

		longitudo = chorda_aedificator_longitudo(aedificator);

		CREDO_MAIOR_I32((i32)longitudo, III);
		CREDO_MINOR_I32((i32)longitudo, V);
		CREDO_MAIOR_AUT_AEQUALIS_I32((i32)longitudo, IV);
		CREDO_MINOR_AUT_AEQUALIS_I32((i32)longitudo, IV);

		chorda_aedificator_destruere(aedificator);
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
