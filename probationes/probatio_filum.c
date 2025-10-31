/* probatio_filum.c - Probationes Fili */
#include "latina.h"
#include "filum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
	Piscina* piscina;
	    b32  praeteritus;

	/* Aperire credo et piscina */
	piscina = piscina_generare_dynamicum("test_filum", 8192);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ==================================================
	 * Probare filum_scribere_literis et filum_legere_totum
	 * ================================================== */

	{
		constans character* test_via        = "/tmp/test_rhubarb_filum.txt";
		constans character* test_contentum  = "salve munde\nhoc est probatio\n";
		            chorda  legere_contentum;
		            chorda  speratus;

		imprimere("\n--- Probans filum_scribere et filum_legere_totum ---\n");

		/* Scribere */
		CREDO_VERUM(filum_scribere_literis(test_via, test_contentum));

		/* Legere */
		legere_contentum = filum_legere_totum(test_via, piscina);
		CREDO_VERUM(legere_contentum.mensura > ZEPHYRUM);
		CREDO_NON_NIHIL(legere_contentum.datum);

		/* Verificare contentum */
		speratus = chorda_ex_literis(test_contentum, piscina);
		CREDO_VERUM(chorda_aequalis(legere_contentum, speratus));

		/* Purgare */
		filum_delere(test_via);
	}


	/* ==================================================
	 * Probare filum_existit
	 * ================================================== */

	{
		constans character* test_via = "/tmp/test_rhubarb_exist.txt";

		imprimere("\n--- Probans filum_existit ---\n");

		/* Non existit ante creationem */
		filum_delere(test_via); /* Purga si existit */
		CREDO_FALSUM(filum_existit(test_via));

		/* Existit post creationem */
		CREDO_VERUM(filum_scribere_literis(test_via, "test"));
		CREDO_VERUM(filum_existit(test_via));

		/* Non existit post deletionem */
		CREDO_VERUM(filum_delere(test_via));
		CREDO_FALSUM(filum_existit(test_via));
	}


	/* ==================================================
	 * Probare filum_mensura
	 * ================================================== */

	{
		constans character* test_via  = "/tmp/test_rhubarb_size.txt";
		constans character* contentum = "12345678";
		    memoriae_index  mensura;

		imprimere("\n--- Probans filum_mensura ---\n");

		filum_scribere_literis(test_via, contentum);
		mensura = filum_mensura(test_via);

		CREDO_AEQUALIS_I32((i32)mensura, VIII);

		filum_delere(test_via);
	}


	/* ==================================================
	 * Probare filum_lector_aperire et lineam_proximam
	 * ================================================== */

	{
		constans character* test_via  = "/tmp/test_rhubarb_lines.txt";
		constans character* contentum = "linea prima\nlinea secunda\nlinea tertia\n";
		       FilumLector* lector;
		            chorda  linea;
		            chorda  speratus1, speratus2, speratus3;

		imprimere("\n--- Probans filum_lector lineam_proximam ---\n");

		/* Crea filum cum tribus lineis */
		filum_scribere_literis(test_via, contentum);

		/* Aperi lector */
		lector = filum_lector_aperire(test_via, piscina);
		CREDO_NON_NIHIL(lector);

		/* Lege lineas */
		speratus1 = chorda_ex_literis("linea prima", piscina);
		speratus2 = chorda_ex_literis("linea secunda", piscina);
		speratus3 = chorda_ex_literis("linea tertia", piscina);

		/* Prima linea */
		CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
		CREDO_AEQUALIS_I32(filum_lector_numerus_versus(lector), I);
		CREDO_VERUM(chorda_aequalis(linea, speratus1));

		/* Secunda linea */
		CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
		CREDO_AEQUALIS_I32(filum_lector_numerus_versus(lector), II);
		CREDO_VERUM(chorda_aequalis(linea, speratus2));

		/* Tertia linea */
		CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
		CREDO_AEQUALIS_I32(filum_lector_numerus_versus(lector), III);
		CREDO_VERUM(chorda_aequalis(linea, speratus3));

		/* EOF */
		CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));
		CREDO_VERUM(filum_lector_finis(lector));

		filum_lector_claudere(lector);
		filum_delere(test_via);
	}


	/* ==================================================
	 * Probare filum_appendere
	 * ================================================== */

	{
		constans character* test_via = "/tmp/test_rhubarb_append.txt";
		            chorda  contentum_initial;
		            chorda  contentum_append;
		            chorda  contentum_final;
		            chorda  speratus;

		imprimere("\n--- Probans filum_appendere ---\n");

		/* Scribe initial */
		contentum_initial = chorda_ex_literis("prima ", piscina);
		CREDO_VERUM(filum_scribere(test_via, contentum_initial));

		/* Appende */
		contentum_append = chorda_ex_literis("secunda", piscina);
		CREDO_VERUM(filum_appendere(test_via, contentum_append));

		/* Verifica */
		contentum_final = filum_legere_totum(test_via, piscina);
		speratus        = chorda_ex_literis("prima secunda", piscina);
		CREDO_VERUM(chorda_aequalis(contentum_final, speratus));

		filum_delere(test_via);
	}


	/* ==================================================
	 * Probare filum_movere
	 * ================================================== */

	{
		constans character* via_vetus = "/tmp/test_rhubarb_old.txt";
		constans character* via_nova  = "/tmp/test_rhubarb_new.txt";
		constans character* contentum = "movere test";

		imprimere("\n--- Probans filum_movere ---\n");

		/* Crea filum */
		filum_scribere_literis(via_vetus, contentum);
		CREDO_VERUM(filum_existit(via_vetus));

		/* Move */
		CREDO_VERUM(filum_movere(via_vetus, via_nova));
		CREDO_FALSUM(filum_existit(via_vetus));
		CREDO_VERUM(filum_existit(via_nova));

		/* Purga */
		filum_delere(via_nova);
	}


	/* ==================================================
	 * Probare filum_copiare
	 * ================================================== */

	{
		constans character* via_fons    = "/tmp/test_rhubarb_source.txt";
		constans character* via_dest    = "/tmp/test_rhubarb_dest.txt";
		constans character* contentum   = "copiare test contentum";
		            chorda  legere_fons;
		            chorda  legere_dest;

		imprimere("\n--- Probans filum_copiare ---\n");

		/* Crea fons */
		filum_scribere_literis(via_fons, contentum);

		/* Copia */
		CREDO_VERUM(filum_copiare(via_fons, via_dest));

		/* Verifica ambo existent */
		CREDO_VERUM(filum_existit(via_fons));
		CREDO_VERUM(filum_existit(via_dest));

		/* Verifica contentum aequalis */
		legere_fons = filum_legere_totum(via_fons, piscina);
		legere_dest = filum_legere_totum(via_dest, piscina);
		CREDO_VERUM(chorda_aequalis(legere_fons, legere_dest));

		/* Purga */
		filum_delere(via_fons);
		filum_delere(via_dest);
	}


	/* ==================================================
	 * Probare filum_status
	 * ================================================== */

	{
		constans character* test_via    = "/tmp/test_rhubarb_status.txt";
		constans character* contentum   = "status test";
		       FilumStatus  status;

		imprimere("\n--- Probans filum_status ---\n");

		/* Crea filum */
		filum_scribere_literis(test_via, contentum);

		/* Cape status */
		CREDO_VERUM(filum_status(test_via, &status));
		CREDO_VERUM(status.est_filum);
		CREDO_FALSUM(status.est_directorium);
		CREDO_VERUM(status.potest_legere);
		CREDO_VERUM(status.potest_scribere);
		CREDO_VERUM(status.mensura > ZEPHYRUM);

		/* Purga */
		filum_delere(test_via);
	}


	/* ==================================================
	 * Probare empty file handling
	 * ================================================== */

	{
		constans character* test_via = "/tmp/test_rhubarb_empty.txt";
		            chorda  contentum;

		imprimere("\n--- Probans empty file ---\n");

		/* Crea filum vacuum */
		filum_scribere_literis(test_via, "");

		/* Lege */
		contentum = filum_legere_totum(test_via, piscina);
		CREDO_AEQUALIS_I32(contentum.mensura, ZEPHYRUM);

		/* Mensura debet esse zero */
		CREDO_AEQUALIS_I32((i32)filum_mensura(test_via), ZEPHYRUM);

		filum_delere(test_via);
	}


	/* ==================================================
	 * Probare NIHIL handling
	 * ================================================== */

	{
		chorda result;

		imprimere("\n--- Probans NIHIL handling ---\n");

		result = filum_legere_totum(NIHIL, piscina);
		CREDO_AEQUALIS_I32(result.mensura, ZEPHYRUM);

		CREDO_FALSUM(filum_existit(NIHIL));
		CREDO_AEQUALIS_I32((i32)filum_mensura(NIHIL), ZEPHYRUM);
		CREDO_FALSUM(filum_scribere_literis(NIHIL, "test"));
	}


	/* ==================================================
	 * Probare line length limits - 4096 byte buffer
	 * ================================================== */

	{
		constans character* test_via = "/tmp/test_rhubarb_long_lines.txt";
		       FilumLector* lector;
		            chorda  linea;
		               i32  i;
		         character* buffer_longus;

		imprimere("\n--- Probans line length limits (4096 buffer) ---\n");

		/* Genera lineam 4095 bytes longam (sine newline) - ex piscina */
		buffer_longus = (character*)piscina_allocare(piscina, 5001);
		si (!buffer_longus)
		{
			imprimere("FRACTA: piscina_allocare\n");
			redde I;
		}

		/* Test 1: Line 4095 bytes (should fit exactly with null terminator) */
		imprimere("\n  Test: 4095 byte line\n");
		per (i = ZEPHYRUM; i < 4095; i++)
		{
			buffer_longus[i] = 'A';
		}
		buffer_longus[4095] = '\0';

		filum_scribere_literis(test_via, buffer_longus);
		lector = filum_lector_aperire(test_via, piscina);
		CREDO_NON_NIHIL(lector);

		/* Debet legere totam lineam semel */
		CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
		CREDO_AEQUALIS_I32(linea.mensura, 4095);

		/* Debet esse EOF */
		CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));
		CREDO_VERUM(filum_lector_finis(lector));

		filum_lector_claudere(lector);


		/* Test 2: Line 5000 bytes - dynamic growth handles transparently */
		imprimere("  Test: 5000 byte line (dynamic growth)\n");
		per (i = ZEPHYRUM; i < 5000; i++)
		{
			buffer_longus[i] = 'B';
		}
		buffer_longus[5000] = '\0';

		filum_scribere_literis(test_via, buffer_longus);
		lector = filum_lector_aperire(test_via, piscina);
		CREDO_NON_NIHIL(lector);

		/* Debet legere totam lineam semel (5000 bytes) */
		CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
		imprimere("    Linea legere: %d bytes\n", linea.mensura);
		CREDO_AEQUALIS_I32(linea.mensura, 5000);

		/* Debet esse EOF */
		CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));

		filum_lector_claudere(lector);


		/* Test 3: Multiple lines with newlines, one long */
		imprimere("  Test: Mixed line lengths with newlines\n");
		{
			FILUM* f = fopen(test_via, "w");
			si (!f)
			{
				imprimere("FRACTA: fopen\n");
				redde I;
			}

			/* Prima linea: brevis */
			fprintf(f, "short line\n");

			/* Secunda linea: 5000 bytes */
			per (i = ZEPHYRUM; i < 5000; i++)
			{
				fputc('C', f);
			}
			fprintf(f, "\n");

			/* Tertia linea: brevis */
			fprintf(f, "another short line\n");

			fclose(f);
		}

		lector = filum_lector_aperire(test_via, piscina);
		CREDO_NON_NIHIL(lector);

		/* Prima linea: brevis */
		CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
		imprimere("    Linea 1: %d bytes\n", linea.mensura);
		CREDO_AEQUALIS_I32(linea.mensura, X); /* "short line" */

		/* Secunda linea: 5000 bytes (longa, sed transparenter tractata) */
		CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
		imprimere("    Linea 2: %d bytes\n", linea.mensura);
		CREDO_AEQUALIS_I32(linea.mensura, 5000);

		/* Tertia linea: brevis */
		CREDO_VERUM(filum_lector_lineam_proximam(lector, &linea));
		imprimere("    Linea 3: %d bytes\n", linea.mensura);
		CREDO_AEQUALIS_I32(linea.mensura, XVIII); /* "another short line" */

		/* EOF */
		CREDO_FALSUM(filum_lector_lineam_proximam(lector, &linea));

		filum_lector_claudere(lector);
		filum_delere(test_via);

		imprimere("\n  NOTA: Lineae > 4095 bytes transparenter tractantur\n");
		imprimere("        ChordaAedificator crescit dynamice pro lineis longis\n");
		imprimere("        (buffer_longus ex piscina, liberabitur cum piscina_destruere)\n");
	}


	/* =================================================
	 * Compendium
	 * ================================================== */

	imprimere("\n");
	credo_imprimere_compendium();

	praeteritus = credo_omnia_praeterierunt();

	/* Destructio */
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
