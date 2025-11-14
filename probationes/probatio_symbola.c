/* probatio_symbola.c - Probationes bibliothecae Symbola */

#include "latina.h"
#include "macho.h"
#include "symbola.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
	Piscina* piscina;
	    b32  praeteritus;

	/* Aperire credo et piscina */
	piscina = piscina_generare_dynamicum("test_symbola", 1024 * 1024);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ==================================================
	 * Probare tabula_symbolorum_ex_macho creatio
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		                  i32  numerus;

		imprimere("\n--- Probans tabula_symbolorum_ex_macho ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		/* Verificare habet symbola */
		numerus = tabula_symbolorum_numerus(tabula);
		imprimere("  Total symbols: %d\n", numerus);
		CREDO_MAIOR_I32(numerus, ZEPHYRUM);
	}


	/* ==================================================
	 * Probare tabula_symbolorum_symbolum (index access)
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		             Symbolum* sym;
		                  i32  numerus;

		imprimere("\n--- Probans tabula_symbolorum_symbolum ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		numerus = tabula_symbolorum_numerus(tabula);

		/* Obtinere primum symbolum */
		si (numerus > ZEPHYRUM)
		{
			sym = tabula_symbolorum_symbolum(tabula, ZEPHYRUM);
			CREDO_NON_NIHIL(sym);
		}

		/* Verificare index invalidus reddit NIHIL */
		sym = tabula_symbolorum_symbolum(tabula, numerus + X);
		CREDO_NIHIL(sym);

		sym = tabula_symbolorum_symbolum(tabula, (i32)(ZEPHYRUM - I));
		CREDO_NIHIL(sym);
	}


	/* ==================================================
	 * Probare symbolorum_iterator
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		  SymbolorumIterator  iter;
		             Symbolum* sym;
		                  i32  numerus_iteratus;
		                  i32  numerus_totalis;

		imprimere("\n--- Probans symbolorum_iterator ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		numerus_totalis  = tabula_symbolorum_numerus(tabula);
		numerus_iteratus = ZEPHYRUM;

		iter = symbolorum_iterator_initium(tabula);

		dum ((sym = symbolorum_iterator_proximum(&iter)) != NIHIL)
		{
			chorda appellatio = symbolum_nomen(sym, piscina);

			si (appellatio.mensura > ZEPHYRUM && appellatio.datum)
			{
				imprimere("  [%d] %.*s (addr=0x%lx, type=0x%x, sect=%d)\n",
				          numerus_iteratus,
				          appellatio.mensura, appellatio.datum,
				          (unsigned long)symbolum_addressa(sym),
				          symbolum_genus(sym),
				          symbolum_numerus_sectionis(sym));
			}

			numerus_iteratus++;
		}

		imprimere("  Total iterated: %d\n", numerus_iteratus);

		/* Verificare numerus iteratus == numerus totalis */
		CREDO_AEQUALIS_I32(numerus_iteratus, numerus_totalis);

		/* Verificare iterator finis */
		CREDO_VERUM(symbolorum_iterator_finis(&iter));
	}


	/* ==================================================
	 * Probare symbolum_invenire
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		             Symbolum* main_sym;
		             Symbolum* non_existens;
		                chorda appellatio;

		imprimere("\n--- Probans symbolum_invenire ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		/* Invenire _main (debet existere) */
		main_sym = symbolum_invenire(tabula, "_main", piscina);
		CREDO_NON_NIHIL(main_sym);

		appellatio = symbolum_nomen(main_sym, piscina);
		imprimere("  Found: %.*s at 0x%lx\n",
		          appellatio.mensura, appellatio.datum,
		          (unsigned long)symbolum_addressa(main_sym));

		/* Verificare nomen */
		CREDO_CHORDA_CONTINET(appellatio, chorda_ex_literis("main", piscina));

		/* Quaerere symbolum non existens */
		non_existens = symbolum_invenire(tabula, "_non_existens_xyz", piscina);
		CREDO_NIHIL(non_existens);
	}


	/* ==================================================
	 * Probare symbolum attributa
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		             Symbolum* main_sym;
		                chorda appellatio;
		        memoriae_index addressa;
		                   i32 genus;
		                   i32 sect;
		                   i32 desc;

		imprimere("\n--- Probans symbolum attributa ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		main_sym = symbolum_invenire(tabula, "_main", piscina);
		CREDO_NON_NIHIL(main_sym);

		/* Verificare nomen */
		appellatio = symbolum_nomen(main_sym, piscina);
		CREDO_VERUM(appellatio.mensura > ZEPHYRUM);
		imprimere("  Name: %.*s\n", appellatio.mensura, appellatio.datum);

		/* Verificare addressa */
		addressa = symbolum_addressa(main_sym);
		imprimere("  Address: 0x%lx\n", (unsigned long)addressa);
		CREDO_VERUM(addressa > ZEPHYRUM);

		/* Verificare genus */
		genus = symbolum_genus(main_sym);
		imprimere("  Type: 0x%x\n", genus);

		/* Verificare sectio */
		sect = symbolum_numerus_sectionis(main_sym);
		imprimere("  Section: %d\n", sect);
		CREDO_VERUM(sect > ZEPHYRUM);  /* _main debet esse in sectione */

		/* Verificare descriptor */
		desc = symbolum_descriptor(main_sym);
		imprimere("  Descriptor: 0x%x\n", desc);
	}


	/* ==================================================
	 * Probare symbolum_est_* predicates
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		             Symbolum* main_sym;

		imprimere("\n--- Probans symbolum_est_* predicates ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		main_sym = symbolum_invenire(tabula, "_main", piscina);
		CREDO_NON_NIHIL(main_sym);

		/* Verificare _main est functio */
		imprimere("  _main is function: %s\n",
		          symbolum_est_functio(main_sym) ? "sic" : "non");
		CREDO_VERUM(symbolum_est_functio(main_sym));

		/* Verificare _main est externum */
		imprimere("  _main is external: %s\n",
		          symbolum_est_externum(main_sym) ? "sic" : "non");
		CREDO_VERUM(symbolum_est_externum(main_sym));

		/* Verificare _main non est indefinitum */
		imprimere("  _main is undefined: %s\n",
		          symbolum_est_indefinitum(main_sym) ? "sic" : "non");
		CREDO_FALSUM(symbolum_est_indefinitum(main_sym));

		/* Verificare _main non est debug */
		imprimere("  _main is debug: %s\n",
		          symbolum_est_debug(main_sym) ? "sic" : "non");
		CREDO_FALSUM(symbolum_est_debug(main_sym));
	}


	/* ==================================================
	 * Probare cum_functione (plures symbola)
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/cum_functione";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		             Symbolum* main_sym;
		             Symbolum* calculare_sym;
		             Symbolum* printf_sym;
		                   i32 numerus;

		imprimere("\n--- Probans cum_functione (plures symbola) ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		numerus = tabula_symbolorum_numerus(tabula);
		imprimere("  Total symbols: %d\n", numerus);
		CREDO_MAIOR_I32(numerus, ZEPHYRUM);

		/* Invenire _main */
		main_sym = symbolum_invenire(tabula, "_main", piscina);
		CREDO_NON_NIHIL(main_sym);
		CREDO_VERUM(symbolum_est_functio(main_sym));
		CREDO_VERUM(symbolum_est_externum(main_sym));

		/* Invenire _calculare_summa */
		calculare_sym = symbolum_invenire(tabula, "_calculare_summa", piscina);
		CREDO_NON_NIHIL(calculare_sym);
		CREDO_VERUM(symbolum_est_functio(calculare_sym));
		CREDO_VERUM(symbolum_est_externum(calculare_sym));

		/* Invenire _printf (externum indefinitum) */
		printf_sym = symbolum_invenire(tabula, "_printf", piscina);
		CREDO_NON_NIHIL(printf_sym);
		CREDO_VERUM(symbolum_est_externum(printf_sym));
		CREDO_VERUM(symbolum_est_indefinitum(printf_sym));
		CREDO_FALSUM(symbolum_est_functio(printf_sym));  /* Indefinitum non est functio */
	}


	/* ==================================================
	 * Probare symbola_obtinere_functiones
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/cum_functione";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		            Symbolum** functiones;
		                   i32 numerus_functionum;
		                   i32 i;
		                   b32 invenit_main;
		                   b32 invenit_calculare;

		imprimere("\n--- Probans symbola_obtinere_functiones ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		functiones = symbola_obtinere_functiones(tabula, &numerus_functionum, piscina);
		imprimere("  Functions found: %d\n", numerus_functionum);

		si (functiones != NIHIL)
		{
			CREDO_MAIOR_I32(numerus_functionum, ZEPHYRUM);

			invenit_main      = FALSUM;
			invenit_calculare = FALSUM;

			per (i = ZEPHYRUM; i < numerus_functionum; i++)
			{
				chorda appellatio = symbolum_nomen(functiones[i], piscina);
				imprimere("    [%d] %.*s\n", i, appellatio.mensura, appellatio.datum);

				si (chorda_continet(appellatio, chorda_ex_literis("main", piscina)))
				{
					invenit_main = VERUM;
				}
				si (chorda_continet(appellatio, chorda_ex_literis("calculare", piscina)))
				{
					invenit_calculare = VERUM;
				}

				/* Verificare omnia sunt functiones */
				CREDO_VERUM(symbolum_est_functio(functiones[i]));
			}

			/* Verificare invenit functiones speratas */
			CREDO_VERUM(invenit_main);
			CREDO_VERUM(invenit_calculare);
		}
	}


	/* ==================================================
	 * Probare symbola_obtinere_externa_indefinita
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/cum_functione";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		            Symbolum** externa;
		                   i32 numerus_externa;
		                   i32 i;
		                   b32 invenit_printf;

		imprimere("\n--- Probans symbola_obtinere_externa_indefinita ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		externa = symbola_obtinere_externa_indefinita(tabula, &numerus_externa, piscina);
		imprimere("  External undefined symbols: %d\n", numerus_externa);

		si (externa != NIHIL)
		{
			CREDO_MAIOR_I32(numerus_externa, ZEPHYRUM);

			invenit_printf = FALSUM;

			per (i = ZEPHYRUM; i < numerus_externa; i++)
			{
				chorda appellatio = symbolum_nomen(externa[i], piscina);
				imprimere("    [%d] %.*s\n", i, appellatio.mensura, appellatio.datum);

				si (chorda_continet(appellatio, chorda_ex_literis("printf", piscina)))
				{
					invenit_printf = VERUM;
				}

				/* Verificare omnia sunt externa et indefinita */
				CREDO_VERUM(symbolum_est_externum(externa[i]));
				CREDO_VERUM(symbolum_est_indefinitum(externa[i]));
			}

			/* Verificare invenit printf */
			CREDO_VERUM(invenit_printf);
		}
	}


	/* ==================================================
	 * Probare symbolum_invenire_per_addressa
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		                 MachO* macho;
		    TabulaSymbolorum* tabula;
		             Symbolum* main_sym;
		             Symbolum* found_sym;
		        memoriae_index addressa;

		imprimere("\n--- Probans symbolum_invenire_per_addressa ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		tabula = tabula_symbolorum_ex_macho(macho, piscina);
		CREDO_NON_NIHIL(tabula);

		/* Obtinere addressa _main */
		main_sym = symbolum_invenire(tabula, "_main", piscina);
		CREDO_NON_NIHIL(main_sym);

		addressa = symbolum_addressa(main_sym);
		imprimere("  _main address: 0x%lx\n", (unsigned long)addressa);

		/* Invenire per addressa */
		found_sym = symbolum_invenire_per_addressa(tabula, addressa, piscina);
		CREDO_NON_NIHIL(found_sym);

		/* Verificare est idem symbolum */
		CREDO_VERUM(symbolum_addressa(found_sym) == addressa);

		/* Verificare addressa invalida reddit NIHIL */
		found_sym = symbolum_invenire_per_addressa(tabula, 0x123456789ABC, piscina);
		CREDO_NIHIL(found_sym);
	}


	/* ==================================================
	 * Probare NIHIL handling
	 * ================================================== */

	{
		TabulaSymbolorum* tabula;
		         Symbolum* sym;
		            chorda appellatio;

		imprimere("\n--- Probans NIHIL handling ---\n");

		/* tabula_symbolorum_ex_macho cum NIHIL */
		tabula = tabula_symbolorum_ex_macho(NIHIL, piscina);
		CREDO_NIHIL(tabula);

		/* tabula_symbolorum_numerus cum NIHIL */
		CREDO_VERUM(tabula_symbolorum_numerus(NIHIL) == ZEPHYRUM);

		/* symbolum_invenire cum NIHIL */
		sym = symbolum_invenire(NIHIL, "_main", piscina);
		CREDO_NIHIL(sym);

		/* symbolum_nomen cum NIHIL */
		appellatio = symbolum_nomen(NIHIL, piscina);
		CREDO_VERUM(appellatio.mensura == ZEPHYRUM);

		/* symbolum_addressa cum NIHIL */
		CREDO_VERUM(symbolum_addressa(NIHIL) == ZEPHYRUM);

		/* symbolum_est_* cum NIHIL */
		CREDO_FALSUM(symbolum_est_functio(NIHIL));
		CREDO_FALSUM(symbolum_est_externum(NIHIL));
		CREDO_FALSUM(symbolum_est_indefinitum(NIHIL));
		CREDO_FALSUM(symbolum_est_debug(NIHIL));
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
