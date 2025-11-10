/* probatio_argumenta.c - Probationes Argumenta */
#include "latina.h"
#include "argumenta.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
	Piscina* piscina;
	     b32  praeteritus;

	/* Aperire credo et piscina */
	piscina = piscina_generare_dynamicum("test_argumenta", 8192);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ==================================================
	 * Probare argumenta_creare
	 * ================================================== */

	{
		ArgumentaParser* parser;

		imprimere("\n--- Probans argumenta_creare ---\n");

		parser = argumenta_creare(piscina);
		CREDO_NON_NIHIL(parser);
	}


	/* ==================================================
	 * Probare vexilla simplicia
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", "-v", "--help"};
		               i32  argc   = III;

		imprimere("\n--- Probans vexilla simplicia ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose output");
		argumenta_addere_vexillum(parser, "-h", "--help", "Show help");

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		CREDO_VERUM(argumenta_habet_vexillum(fructus, "-v"));
		CREDO_VERUM(argumenta_habet_vexillum(fructus, "--verbose"));
		CREDO_VERUM(argumenta_habet_vexillum(fructus, "--help"));
		CREDO_FALSUM(argumenta_habet_vexillum(fructus, "-q"));
	}


	/* ==================================================
	 * Probare optiones cum valoribus
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", 
                                      "-o", 
                                      "output.txt", 
                                      "--config=settings.ini"};
		               i32  argc = IV;
		            chorda  output;
		            chorda  config;

		imprimere("\n--- Probans optiones cum valoribus ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_optionem(parser, "-o", "--output", "Output file");
		argumenta_addere_optionem(parser, "-c", "--config", "Config file");

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		output = argumenta_obtinere_optionem(fructus, "-o", piscina);
		CREDO_INAEQUALITAS_I32(output.mensura, ZEPHYRUM);
		CREDO_VERUM(chorda_aequalis(output, chorda_ex_literis("output.txt", piscina)));

		config = argumenta_obtinere_optionem(fructus, "--config", piscina);
		CREDO_INAEQUALITAS_I32(config.mensura, ZEPHYRUM);
		CREDO_VERUM(chorda_aequalis(config, chorda_ex_literis("settings.ini", piscina)));
	}


	/* ==================================================
	 * Probare optiones cum syntax equals
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", 
                                      "--output=file.txt", 
                                      "--name=test"};
		               i32  argc = III;
		            chorda  output;
		            chorda  titulus;

		imprimere("\n--- Probans optiones cum syntax equals ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_optionem(parser, "-o", "--output", "Output file");
		argumenta_addere_optionem(parser, "-n", "--name", "Name");

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		output = argumenta_obtinere_optionem(fructus, "--output", piscina);
		CREDO_VERUM(chorda_aequalis(output, chorda_ex_literis("file.txt", piscina)));

		titulus = argumenta_obtinere_optionem(fructus, "--name", piscina);
		CREDO_VERUM(chorda_aequalis(titulus, chorda_ex_literis("test", piscina)));
	}


	/* ==================================================
	 * Probare argumenta positionalia
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", "input.txt", "output.txt"};
		               i32  argc = III;
		            chorda  input;
		            chorda  output;

		imprimere("\n--- Probans argumenta positionalia ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_positionalem(parser, "input", "Input file", VERUM);
		argumenta_addere_positionalem(parser, "output", "Output file", FALSUM);

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		CREDO_AEQUALIS_I32(argumenta_numerus_positionalium(fructus), II);

		input = argumenta_obtinere_positionalem(fructus, ZEPHYRUM, piscina);
		CREDO_VERUM(chorda_aequalis(input, chorda_ex_literis("input.txt", piscina)));

		output = argumenta_obtinere_positionalem(fructus, I, piscina);
		CREDO_VERUM(chorda_aequalis(output, chorda_ex_literis("output.txt", piscina)));
	}


	/* ==================================================
	 * Probare mixtura vexilla, optiones, et positionalia
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", 
                                      "-v", 
                                      "--output", 
                                      "out.txt", 
                                      "input.txt", 
                                      "--debug"};
		               i32  argc = VI;
		            chorda  output;
		            chorda  input;

		imprimere("\n--- Probans mixtura vexilla, optiones, et positionalia ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");
		argumenta_addere_vexillum(parser, "-d", "--debug", "Debug mode");
		argumenta_addere_optionem(parser, "-o", "--output", "Output file");
		argumenta_addere_positionalem(parser, "input", "Input file", VERUM);

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		CREDO_VERUM(argumenta_habet_vexillum(fructus, "-v"));
		CREDO_VERUM(argumenta_habet_vexillum(fructus, "--debug"));

		output = argumenta_obtinere_optionem(fructus, "--output", piscina);
		CREDO_VERUM(chorda_aequalis(output, chorda_ex_literis("out.txt", piscina)));

		CREDO_AEQUALIS_I32(argumenta_numerus_positionalium(fructus), I);
		input = argumenta_obtinere_positionalem(fructus, ZEPHYRUM, piscina);
		CREDO_VERUM(chorda_aequalis(input, chorda_ex_literis("input.txt", piscina)));
	}


	/* ==================================================
	 * Probare vexillum solum (sine nomen longum)
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", "-q"};
		               i32  argc = II;

		imprimere("\n--- Probans vexillum solum breve ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_vexillum(parser, "-q", NIHIL, "Quiet mode");

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		CREDO_VERUM(argumenta_habet_vexillum(fructus, "-q"));
	}


	/* ==================================================
	 * Probare optio solum (sine nomen breve)
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", "--config", "test.ini"};
		               i32  argc = III;
		            chorda  config;

		imprimere("\n--- Probans optio solum longum ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_optionem(parser, NIHIL, "--config", "Config file");

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		config = argumenta_obtinere_optionem(fructus, "--config", piscina);
		CREDO_VERUM(chorda_aequalis(config, chorda_ex_literis("test.ini", piscina)));
	}


	/* ==================================================
	 * Probare optio non data
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa"};
		               i32  argc = I;
		            chorda  output;

		imprimere("\n--- Probans optio non data ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_optionem(parser, "-o", "--output", "Output file");

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		output = argumenta_obtinere_optionem(fructus, "--output", piscina);
		CREDO_AEQUALIS_I32(output.mensura, ZEPHYRUM);
	}


	/* ==================================================
	 * Probare errore: optio ignota
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", "--unknown"};
		               i32  argc = II;

		imprimere("\n--- Probans errore: optio ignota ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");

		fructus = argumenta_conari_parsere(parser, argc, argv);
		CREDO_NIHIL(fructus);
	}


	/* ==================================================
	 * Probare errore: optio sine valore
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", "--output"};
		               i32  argc = II;

		imprimere("\n--- Probans errore: optio sine valore ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_optionem(parser, "-o", "--output", "Output file");

		fructus = argumenta_conari_parsere(parser, argc, argv);
		CREDO_NIHIL(fructus);
	}


	/* ==================================================
	 * Probare positionalia multa
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", 
                                      "file1.txt", 
                                      "file2.txt", 
                                      "file3.txt"};
		               i32  argc = IV;
		            chorda  file1;
		            chorda  file2;
		            chorda  file3;

		imprimere("\n--- Probans positionalia multa ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_positionalem(parser, "file1", "First file", VERUM);
		argumenta_addere_positionalem(parser, "file2", "Second file", VERUM);
		argumenta_addere_positionalem(parser, "file3", "Third file", FALSUM);

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		CREDO_AEQUALIS_I32(argumenta_numerus_positionalium(fructus), III);

		file1 = argumenta_obtinere_positionalem(fructus, ZEPHYRUM, piscina);
		file2 = argumenta_obtinere_positionalem(fructus, I, piscina);
		file3 = argumenta_obtinere_positionalem(fructus, II, piscina);

		CREDO_VERUM(chorda_aequalis(file1, chorda_ex_literis("file1.txt", piscina)));
		CREDO_VERUM(chorda_aequalis(file2, chorda_ex_literis("file2.txt", piscina)));
		CREDO_VERUM(chorda_aequalis(file3, chorda_ex_literis("file3.txt", piscina)));
	}


	/* ==================================================
	 * Probare vexillum cum valor (errore)
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa", "--verbose=true"};
		               i32  argc = II;

		imprimere("\n--- Probans vexillum cum valor (errore) ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");

		fructus = argumenta_conari_parsere(parser, argc, argv);
		CREDO_NIHIL(fructus);
	}


	/* ==================================================
	 * Probare sine argumenta
	 * ================================================== */

	{
		   ArgumentaParser* parser;
		  ArgumentaFructus* fructus;
		constans character* argv[] = {"programa"};
		               i32  argc = I;

		imprimere("\n--- Probans sine argumenta ---\n");

		parser = argumenta_creare(piscina);
		argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose");
		argumenta_addere_optionem(parser, "-o", "--output", "Output");

		fructus = argumenta_parsere(parser, argc, argv);
		CREDO_NON_NIHIL(fructus);

		CREDO_FALSUM(argumenta_habet_vexillum(fructus, "-v"));
		CREDO_AEQUALIS_I32(argumenta_numerus_positionalium(fructus), ZEPHYRUM);
	}


	/* ==================================================
	 * Probare descriptio et exempla
	 * ================================================== */

	{
		ArgumentaParser* parser;

		imprimere("\n--- Probans descriptio et exempla ---\n");

		parser = argumenta_creare(piscina);
		argumenta_ponere_descriptionem(parser, "Test program - does testing");
		argumenta_addere_exemplum(parser, "programa -v input.txt");
		argumenta_addere_exemplum(parser, "programa --output=out.txt file.txt");
		argumenta_addere_vexillum(parser, "-v", "--verbose", "Verbose output");

		/* Non fracta */
		CREDO_NON_NIHIL(parser);
	}


	/* ==================================================
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
