/* probatio_sectio.c - Probationes bibliothecae Sectio */

#include "latina.h"
#include "macho.h"
#include "sectio.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
	Piscina* piscina;
	    b32  praeteritus;

	/* Aperire credo et piscina */
	piscina = piscina_generare_dynamicum("test_sectio", 1024 * 1024);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ==================================================
	 * Probare macho_piscina et macho_datum (novi getters)
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		             MachO* macho;
		           Piscina* macho_pisc;
		      constans i8* datum;

		imprimere("\n--- Probans macho_piscina et macho_datum ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		/* Verificare piscina getter */
		macho_pisc = macho_piscina(macho);
		CREDO_NON_NIHIL(macho_pisc);
		CREDO_VERUM(macho_pisc == piscina);

		/* Verificare datum getter */
		datum = macho_datum(macho);
		CREDO_NON_NIHIL(datum);
	}


	/* ==================================================
	 * Probare sectio_invenire
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		             MachO* macho;
		            Sectio* text;
		            Sectio* unwind;
		            Sectio* non_existens;

		imprimere("\n--- Probans sectio_invenire ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		/* Invenire __TEXT.__text */
		text = sectio_invenire(macho, "__TEXT", "__text", piscina);
		CREDO_NON_NIHIL(text);

		/* Invenire __TEXT.__unwind_info */
		unwind = sectio_invenire(macho, "__TEXT", "__unwind_info", piscina);
		CREDO_NON_NIHIL(unwind);

		/* Quaerere sectionem non existentem */
		non_existens = sectio_invenire(macho, "__TEXT", "__non_existens", piscina);
		CREDO_NIHIL(non_existens);

		/* Quaerere segmentum non existens */
		non_existens = sectio_invenire(macho, "__FAKE", "__text", piscina);
		CREDO_NIHIL(non_existens);
	}


	/* ==================================================
	 * Probare sectio attributa
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		             MachO* macho;
		            Sectio* text;
		            chorda  titulis;
		            chorda  titulis_seg;
		    memoriae_index  mensura;
		    memoriae_index  addressa;
		               i32  ordinatio;
		               i32  vexilla;

		imprimere("\n--- Probans sectio attributa ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		text = sectio_invenire(macho, "__TEXT", "__text", piscina);
		CREDO_NON_NIHIL(text);

		/* Verificare titulis */
		titulis = sectio_nomen(text, piscina);
		CREDO_VERUM(titulis.mensura > ZEPHYRUM);
		imprimere("  Section name: %.*s\n", titulis.mensura, titulis.datum);
		CREDO_CHORDA_CONTINET(titulis, chorda_ex_literis("text", piscina));

		/* Verificare titulis segmenti */
		titulis_seg = sectio_nomen_segmenti(text, piscina);
		CREDO_VERUM(titulis_seg.mensura > ZEPHYRUM);
		imprimere("  Segment name: %.*s\n", titulis_seg.mensura, titulis_seg.datum);
		CREDO_CHORDA_CONTINET(titulis_seg, chorda_ex_literis("TEXT", piscina));

		/* Verificare mensura */
		mensura = sectio_mensura(text);
		imprimere("  Size: %zu\n", mensura);
		CREDO_VERUM(mensura > ZEPHYRUM);

		/* Verificare addressa */
		addressa = sectio_addressa(text);
		imprimere("  Address: 0x%lx\n", (unsigned long)addressa);
		CREDO_VERUM(addressa > ZEPHYRUM);

		/* Verificare ordinatio */
		ordinatio = sectio_ordinatio(text);
		imprimere("  Align: 2^%d\n", ordinatio);
		CREDO_VERUM(ordinatio >= ZEPHYRUM);

		/* Verificare vexilla */
		vexilla = sectio_vexilla(text);
		imprimere("  Flags: 0x%x\n", vexilla);
	}


	/* ==================================================
	 * Probare sectio_datum (zero-copy)
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		             MachO* macho;
		            Sectio* text;
		            chorda  data;
		      constans i8* macho_buf;

		imprimere("\n--- Probans sectio_datum (zero-copy) ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		text = sectio_invenire(macho, "__TEXT", "__text", piscina);
		CREDO_NON_NIHIL(text);

		/* Obtinere datum */
		data = sectio_datum(text);
		CREDO_NON_NIHIL(data.datum);
		CREDO_VERUM(data.mensura > ZEPHYRUM);
		CREDO_VERUM(data.mensura == sectio_mensura(text));

		imprimere("  Data size: %d bytes\n", data.mensura);

		/* Verificare punctat in buffer macho */
		macho_buf = macho_datum(macho);
		CREDO_VERUM(data.datum >= macho_buf);
	}


	/* ==================================================
	 * Probare sectio helper functions
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		             MachO* macho;
		            Sectio* text;
		               b32  est_inst;
		               b32  est_debug;
		               i32  genus;

		imprimere("\n--- Probans sectio helper functions ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		text = sectio_invenire(macho, "__TEXT", "__text", piscina);
		CREDO_NON_NIHIL(text);

		/* Verificare est instructiones */
		est_inst = sectio_est_instructiones(text);
		imprimere("  Is instructions: %s\n", est_inst ? "sic" : "non");
		CREDO_VERUM(est_inst);  /* __text debet habere instructiones */

		/* Verificare est debug */
		est_debug = sectio_est_debug(text);
		imprimere("  Is debug: %s\n", est_debug ? "sic" : "non");
		CREDO_FALSUM(est_debug);  /* __text non est debug */

		/* Verificare genus extraction */
		genus = sectio_extrahere_genus(sectio_vexilla(text));
		imprimere("  Section type: 0x%x\n", genus);
		CREDO_VERUM(genus == SECTIO_GENUS_REGULAR);
	}


	/* ==================================================
	 * Probare sectio_iterator
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		             MachO* macho;
		    SectioIterator  iter;
		            Sectio* sectio;
		               i32  numerus_sectiones;
		               b32  invenit_text;
		               b32  invenit_unwind;

		imprimere("\n--- Probans sectio_iterator ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		numerus_sectiones = ZEPHYRUM;
		invenit_text      = FALSUM;
		invenit_unwind    = FALSUM;

		iter = sectio_iterator_initium(macho);

		dum ((sectio = sectio_iterator_proximum(&iter)) != NIHIL)
		{
			chorda titulis = sectio_nomen(sectio, piscina);
			chorda seg     = sectio_nomen_segmenti(sectio, piscina);

			imprimere("  [%d] %.*s.%.*s (size=%zu)\n",
			          numerus_sectiones,
			          seg.mensura, seg.datum,
			          titulis.mensura, titulis.datum,
			          sectio_mensura(sectio));

			numerus_sectiones++;

			/* Verificare inventio sectionum cognitarum */
			si (chorda_continet(titulis, chorda_ex_literis("text", piscina)))
			{
				invenit_text = VERUM;
			}
			si (chorda_continet(titulis, chorda_ex_literis("unwind", piscina)))
			{
				invenit_unwind = VERUM;
			}
		}

		imprimere("  Total sections: %d\n", numerus_sectiones);

		/* Verificare invenit saltem aliquas sectiones */
		CREDO_VERUM(numerus_sectiones > ZEPHYRUM);
		CREDO_VERUM(invenit_text);
		CREDO_VERUM(invenit_unwind);
	}


	/* ==================================================
	 * Probare cum_functione (plures sectiones)
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/cum_functione";
		             MachO* macho;
		    SectioIterator  iter;
		            Sectio* sectio;
		               i32  numerus_sectiones;

		imprimere("\n--- Probans cum_functione (plures sectiones) ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		numerus_sectiones = ZEPHYRUM;
		iter = sectio_iterator_initium(macho);

		dum ((sectio = sectio_iterator_proximum(&iter)) != NIHIL)
		{
			numerus_sectiones++;
		}

		imprimere("  Total sections: %d\n", numerus_sectiones);
		CREDO_VERUM(numerus_sectiones > ZEPHYRUM);
	}


	/* ==================================================
	 * Probare NIHIL handling
	 * ================================================== */

	{
		Sectio* sectio;
		chorda  titulis;

		imprimere("\n--- Probans NIHIL handling ---\n");

		/* sectio_invenire cum NIHIL */
		sectio = sectio_invenire(NIHIL, "__TEXT", "__text", piscina);
		CREDO_NIHIL(sectio);

		/* sectio_nomen cum NIHIL */
		titulis = sectio_nomen(NIHIL, piscina);
		CREDO_VERUM(titulis.mensura == ZEPHYRUM);

		/* sectio_mensura cum NIHIL */
		CREDO_VERUM(sectio_mensura(NIHIL) == ZEPHYRUM);

		/* sectio_est_instructiones cum NIHIL */
		CREDO_FALSUM(sectio_est_instructiones(NIHIL));
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
