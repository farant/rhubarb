/* probatio_macho_deps.c - Test dylib/entry/UUID extraction */

#include "latina.h"
#include "macho.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
	Piscina* piscina;
	MachO* macho;
	chorda* dylibs;
	i32 numerus_dylibs;
	i32 i;
	memoriae_index entry;
	i8 uuid[XVI];
	b32 praeteritus;

	/* Aperire credo et piscina */
	piscina = piscina_generare_dynamicum("test_macho_deps", 1024 * 1024);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);

	/* ==================================================
	 * Probare macho_obtinere_dylibs()
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";

		imprimere("\n--- Probans macho_obtinere_dylibs ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		/* Obtinere dylibs */
		dylibs = macho_obtinere_dylibs(macho, &numerus_dylibs, piscina);

		/* Debet habere saltem aliquas dylibs */
		CREDO_VERUM(numerus_dylibs > ZEPHYRUM);
		CREDO_NON_NIHIL(dylibs);

		imprimere("  Found %d dylibs:\n", numerus_dylibs);
		per (i = ZEPHYRUM; i < numerus_dylibs; i++)
		{
			imprimere("    [%d] %.*s\n", i,
			          dylibs[i].mensura, dylibs[i].datum);

			/* Omnes dylibs debent habere mensuram > 0 */
			CREDO_VERUM(dylibs[i].mensura > ZEPHYRUM);
		}

		/* Verificare communem dylib */
		{
			b32 invenit_libsystem = FALSUM;
			per (i = ZEPHYRUM; i < numerus_dylibs; i++)
			{
				si (chorda_continet(dylibs[i],
				                    chorda_ex_literis("libSystem", piscina)))
				{
					invenit_libsystem = VERUM;
					frange;
				}
			}
			CREDO_VERUM(invenit_libsystem);
		}
	}

	/* ==================================================
	 * Probare macho_obtinere_entry_point()
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		b32 habet_entry;

		imprimere("\n--- Probans macho_obtinere_entry_point ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		habet_entry = macho_obtinere_entry_point(macho, &entry);

		/* Non omnia fixa habent LC_MAIN - antiqua utuntur LC_UNIXTHREAD */
		si (habet_entry)
		{
			CREDO_VERUM(entry > ZEPHYRUM);
			imprimere("  Entry point offset: 0x%lx\n",
			          (unsigned long)entry);
		}
		alioquin
		{
			imprimere("  No LC_MAIN command (older binary or library)\n");
		}
	}

	/* ==================================================
	 * Probare macho_obtinere_uuid()
	 * ================================================== */

	{
		constans character* via = "probationes/fixa/simplex";
		b32 habet_uuid;

		imprimere("\n--- Probans macho_obtinere_uuid ---\n");

		macho = macho_aperire(via, piscina);
		CREDO_NON_NIHIL(macho);

		habet_uuid = macho_obtinere_uuid(macho, uuid);

		/* Omnes Mach-O filos modernos debent habere UUID */
		CREDO_VERUM(habet_uuid);

		imprimere("  UUID: ");
		per (i = ZEPHYRUM; i < XVI; i++)
		{
			imprimere("%02x", (insignatus character)uuid[i]);
		}
		imprimere("\n");

		/* UUID non debet esse omnia zephyra */
		{
			b32 omnia_zephyra = VERUM;
			per (i = ZEPHYRUM; i < XVI; i++)
			{
				si (uuid[i] != ZEPHYRUM)
				{
					omnia_zephyra = FALSUM;
					frange;
				}
			}
			CREDO_FALSUM(omnia_zephyra);
		}
	}

	/* ==================================================
	 * Probare NIHIL handling
	 * ================================================== */

	{
		chorda* dylibs_nil;
		i32 numerus_nil;
		memoriae_index entry_nil;
		i8 uuid_nil[XVI];

		imprimere("\n--- Probans NIHIL handling ---\n");

		/* macho_obtinere_dylibs cum NIHIL */
		dylibs_nil = macho_obtinere_dylibs(NIHIL, &numerus_nil, piscina);
		CREDO_NIHIL(dylibs_nil);
		CREDO_VERUM(numerus_nil == ZEPHYRUM);

		/* macho_obtinere_entry_point cum NIHIL */
		CREDO_FALSUM(macho_obtinere_entry_point(NIHIL, &entry_nil));

		/* macho_obtinere_uuid cum NIHIL */
		CREDO_FALSUM(macho_obtinere_uuid(NIHIL, uuid_nil));
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
