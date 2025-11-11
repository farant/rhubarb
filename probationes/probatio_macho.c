/* probatio_macho.c - Probationes bibliothecae Mach-O */

#include "latina.h"
#include "macho.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
	Piscina* piscina;
	    b32  praeteritus;

	/* Aperire credo et piscina */
	piscina = piscina_generare_dynamicum("test_macho", 1024 * 1024);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ==================================================
	 * Probare macho_aperire cum fixture "simplex"
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		             MachO* macho;
		               i32  genus_cpu;
		               i32  numerus_mandatorum;

		imprimere("\n--- Probans macho_aperire (simplex) ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		/* Verificare est 64-bit */
		CREDO_VERUM(macho_est_64bit(macho));

		/* Verificare genus processoris (ARM64 in Apple Silicon) */
		genus_cpu = macho_genus_processoris(macho);
		imprimere("  CPU type: 0x%x\n", genus_cpu);
		CREDO_VERUM(genus_cpu == MACHO_CPU_TYPE_X86_64 ||
		            genus_cpu == MACHO_CPU_TYPE_ARM64);

		/* Verificare genus filum est executabile */
		CREDO_AEQUALIS_I32(macho_genus_filum(macho), MACHO_GENUS_EXECUTABILE);

		/* Verificare habet mandata oneris */
		numerus_mandatorum = macho_numerus_mandatorum(macho);
		imprimere("  Load commands: %d\n", numerus_mandatorum);
		CREDO_VERUM(numerus_mandatorum > ZEPHYRUM);
	}


	/* ==================================================
	 * Probare macho_aperire cum fixture "cum_functione"
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/cum_functione";
		             MachO* macho;
		               i32  numerus_mandatorum;

		imprimere("\n--- Probans macho_aperire (cum_functione) ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		/* Verificare est 64-bit */
		CREDO_VERUM(macho_est_64bit(macho));

		/* Verificare genus filum */
		CREDO_AEQUALIS_I32(macho_genus_filum(macho), MACHO_GENUS_EXECUTABILE);

		/* Verificare habet mandata oneris */
		numerus_mandatorum = macho_numerus_mandatorum(macho);
		imprimere("  Load commands: %d\n", numerus_mandatorum);
		CREDO_VERUM(numerus_mandatorum > ZEPHYRUM);
	}


	/* ==================================================
	 * Probare iterationem mandatorum oneris
	 * ================================================== */

	{
		   constans character* via = "probationes/fixa/simplex";
		                MachO* macho;
		MachoIteratorMandatum  iter;
		      MandatumOnustum* cmd;
		                  i32  numerus_iteratus;
		                  b32  invenit_segment_64;
		                  b32  invenit_symtab;
		                  b32  invenit_dysymtab;

		imprimere("\n--- Probans iterationem mandatorum oneris ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		numerus_iteratus   = ZEPHYRUM;
		invenit_segment_64 = FALSUM;
		invenit_symtab     = FALSUM;
		invenit_dysymtab   = FALSUM;

		iter = macho_iterator_mandatorum_initium(macho);
		dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
		{
			           i32 genus   = mandatum_genus(cmd);
			memoriae_index mensura = mandatum_magnitudo(cmd);

			imprimere("  [%d] genus=0x%x mensura=%zu\n",
			          numerus_iteratus, genus, mensura);

			numerus_iteratus++;

			/* Verificare genus mandati */
			si (genus == MACHO_LC_SEGMENT_64) invenit_segment_64 = VERUM;
			si (genus == MACHO_LC_SYMTAB) invenit_symtab         = VERUM;
			si (genus == MACHO_LC_DYSYMTAB) invenit_dysymtab     = VERUM;

			/* Verificare mensura rationabilis */
			CREDO_VERUM(mensura >= VIII);  /* Minimum load command size */

			/* Verificare datum non NIHIL */
			CREDO_NON_NIHIL(mandatum_datum(cmd));
		}

		/* Verificare numerus iteratus concordat cum header */
		CREDO_AEQUALIS_I32(numerus_iteratus, macho_numerus_mandatorum(macho));

		/* Verificare invenit mandata communia */
		CREDO_VERUM(invenit_segment_64);
		CREDO_VERUM(invenit_symtab);

		imprimere("  Totalis iteratus: %d\n", numerus_iteratus);
		imprimere("  Invenit LC_SEGMENT_64: %s\n", invenit_segment_64 ? "sic" : "non");
		imprimere("  Invenit LC_SYMTAB: %s\n", invenit_symtab ? "sic" : "non");
		imprimere("  Invenit LC_DYSYMTAB: %s\n", invenit_dysymtab ? "sic" : "non");
	}


	/* ==================================================
	 * Probare macho_iterator_mandatorum_finis
	 * ================================================== */

	{
		   constans character* via = "probationes/fixa/simplex";
		                MachO* macho;
		MachoIteratorMandatum  iter;
		      MandatumOnustum* cmd;

		imprimere("\n--- Probans macho_iterator_mandatorum_finis ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		iter = macho_iterator_mandatorum_initium(macho);

		/* Iterator non debet esse finis ad initium */
		CREDO_FALSUM(macho_iterator_mandatorum_finis(&iter));

		/* Iterat omnes */
		dum ((cmd = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
		{
			/* nihil */
		}

		/* Nunc debet esse finis */
		CREDO_VERUM(macho_iterator_mandatorum_finis(&iter));

		/* Proximum debet reddere NIHIL */
		cmd = macho_iterator_mandatorum_proximum(&iter);
		CREDO_NIHIL(cmd);
	}


	/* ==================================================
	 * Probare macho_filum_aperire
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		        MachoFilum* filum;
		             MachO* imago;
		               i32  numerus;
		               i32  genus_cpu;
		               i32  subgenus_cpu;

		imprimere("\n--- Probans macho_filum_aperire ---\n");

		filum = macho_filum_aperire(via, piscina);
		CREDO_NON_NIHIL(filum);

		/* Verificare numerus imaginum */
		numerus = macho_filum_numerus_imago(filum);
		imprimere("  Numerus imaginum: %d\n", numerus);
		CREDO_AEQUALIS_I32(numerus, I);

		/* Obtinere imaginem */
		imago = macho_filum_imago(filum, ZEPHYRUM);
		CREDO_NON_NIHIL(imago);

		/* Verificare genus processoris */
		genus_cpu = macho_filum_genus_processoris(filum, ZEPHYRUM);
		imprimere("  CPU type: 0x%x\n", genus_cpu);
		CREDO_VERUM(genus_cpu == MACHO_CPU_TYPE_X86_64 ||
		            genus_cpu == MACHO_CPU_TYPE_ARM64);

		/* Verificare subgenus */
		subgenus_cpu = macho_filum_subgenus_processoris(filum, ZEPHYRUM);
		imprimere("  CPU subtype: 0x%x\n", subgenus_cpu);

		/* Verificare imago ex filum concordat cum macho_aperire */
		CREDO_AEQUALIS_I32(macho_genus_processoris(imago), genus_cpu);
	}


	/* ==================================================
	 * Probare NIHIL handling
	 * ================================================== */

	{
		     MachO* macho;
		MachoFilum* filum;

		imprimere("\n--- Probans NIHIL handling ---\n");

		/* macho_aperire cum via NIHIL */
		macho = macho_aperire(NIHIL, piscina);
		CREDO_NIHIL(macho);

		/* macho_filum_aperire cum via NIHIL */
		filum = macho_filum_aperire(NIHIL, piscina);
		CREDO_NIHIL(filum);

		/* macho_ex_memoria cum datum NIHIL */
		macho = macho_ex_memoria(NIHIL, C, piscina);
		CREDO_NIHIL(macho);

		/* Interrogatio cum macho NIHIL */
		CREDO_AEQUALIS_I32(macho_genus_processoris(NIHIL), ZEPHYRUM);
		CREDO_AEQUALIS_I32(macho_numerus_mandatorum(NIHIL), ZEPHYRUM);
		CREDO_FALSUM(macho_est_64bit(NIHIL));
	}


	/* ==================================================
	 * Probare filum non existens
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/non_existens";
		             MachO* macho;
		constans character* error;

		imprimere("\n--- Probans filum non existens ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NIHIL(macho);

		error = macho_error_recens();
		imprimere("  Error: %s\n", error ? error : "(nullus)");
		CREDO_NON_NIHIL(error);
	}


	/* ==================================================
	 * Probare ambos fixtures habent structuram similem
	 * ================================================== */

	{
		MachO* simplex;
		MachO* cum_functione;
		  i32  num_simplex;
		  i32  num_functione;

		imprimere("\n--- Comparans ambos fixtures ---\n");

		simplex = macho_aperire("probationes/fixa/simplex", piscina);
		cum_functione = macho_aperire("probationes/fixa/cum_functione", piscina);

		CREDO_NON_NIHIL(simplex);
		CREDO_NON_NIHIL(cum_functione);

		/* Ambo debent esse 64-bit */
		CREDO_VERUM(macho_est_64bit(simplex));
		CREDO_VERUM(macho_est_64bit(cum_functione));

		/* Ambo debent esse executabiles */
		CREDO_AEQUALIS_I32(macho_genus_filum(simplex), MACHO_GENUS_EXECUTABILE);
		CREDO_AEQUALIS_I32(macho_genus_filum(cum_functione), MACHO_GENUS_EXECUTABILE);

		/* Ambo debent habere eundem genus processoris */
		CREDO_AEQUALIS_I32(macho_genus_processoris(simplex),
		                   macho_genus_processoris(cum_functione));

		/* cum_functione debet habere plura mandata (habet symbola, etc) */
		num_simplex = macho_numerus_mandatorum(simplex);
		num_functione = macho_numerus_mandatorum(cum_functione);

		imprimere("  simplex mandata: %d\n", num_simplex);
		imprimere("  cum_functione mandata: %d\n", num_functione);

		/* Nota: non verificamus num_functione > num_simplex quia
		 * compiler potest optimizare similiter */
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
