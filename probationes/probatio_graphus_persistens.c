/* probatio_graphus_persistens.c - Probationes GraphusPersistens */
#include "latina.h"
#include "graphus_persistens.h"
#include "entitas_repositorium.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_LOG_PROBATIO "/tmp/probatio_graphus_persistens.log"

/* Auxiliaris: Delere filum si existit */
interior vacuum
_delere_log_si_existit(vacuum)
{
	si (filum_existit(VIA_LOG_PROBATIO))
	{
		filum_delere(VIA_LOG_PROBATIO);
	}
}

s32 principale(vacuum)
{
	Piscina* piscina;
	    b32  praeteritus;

	/* Aperire piscinam et credonem */
	piscina = piscina_generare_dynamicum("probatio_gp", M * II);  /* 2MB */
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);

	/* ==================================================
	 * Probare graphus_persistens_creare (novum)
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;

		imprimere("\n--- Probans graphus_persistens_creare (novum) ---\n");

		_delere_log_si_existit();

		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		CREDO_NON_NIHIL(gp);

		repo = graphus_persistens_repositorium(gp);
		CREDO_NON_NIHIL(repo);

		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), ZEPHYRUM);

		/* Log file debet existere */
		CREDO_VERUM(filum_existit(VIA_LOG_PROBATIO));

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Probare entitas_creare per repositorium
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;
		Entitas*             entitas;

		imprimere("\n--- Probans entitas_creare per repositorium ---\n");

		_delere_log_si_existit();

		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		CREDO_NON_NIHIL(gp);

		repo = graphus_persistens_repositorium(gp);

		/* Creare entitatem */
		entitas = repo->entitas_creare(repo->datum, "Page");
		CREDO_NON_NIHIL(entitas);
		CREDO_NON_NIHIL(entitas->id);

		/* Debet habere unum eventum */
		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), I);

		/* Log file debet habere datum */
		CREDO_MAIOR_I32((i32)graphus_persistens_mensura_log(gp), ZEPHYRUM);

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Probare proprietas_ponere per repositorium
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;
		Entitas*             entitas;
		GraphusEntitatum*    graphus;
		InternamentumChorda* intern;
		chorda*              clavis;
		chorda*              valor;

		imprimere("\n--- Probans proprietas_ponere per repositorium ---\n");

		_delere_log_si_existit();

		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);
		graphus = graphus_persistens_graphus(gp);
		intern = graphus_entitatum_internamentum(graphus);

		/* Creare entitatem */
		entitas = repo->entitas_creare(repo->datum, "Note");
		CREDO_NON_NIHIL(entitas);

		/* Ponere proprietatem */
		CREDO_VERUM(repo->proprietas_ponere(repo->datum, entitas, "title", "Mea Nota"));

		/* Verificare proprietatem in memoria */
		clavis = chorda_internare_ex_literis(intern, "title");
		valor = entitas_proprietas_capere(entitas, clavis);
		CREDO_NON_NIHIL(valor);

		/* Debet habere duo eventus (creare + ponere) */
		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), II);

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Probare nota_addere per repositorium
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;
		Entitas*             entitas;
		GraphusEntitatum*    graphus;
		InternamentumChorda* intern;
		chorda*              nota;

		imprimere("\n--- Probans nota_addere per repositorium ---\n");

		_delere_log_si_existit();

		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);
		graphus = graphus_persistens_graphus(gp);
		intern = graphus_entitatum_internamentum(graphus);

		/* Creare entitatem */
		entitas = repo->entitas_creare(repo->datum, "Task");
		CREDO_NON_NIHIL(entitas);

		/* Addere notam */
		CREDO_VERUM(repo->nota_addere(repo->datum, entitas, "#root"));

		/* Verificare notam in memoria */
		nota = chorda_internare_ex_literis(intern, "#root");
		CREDO_VERUM(entitas_nota_habet(entitas, nota));

		/* Debet habere duo eventus */
		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), II);

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Probare relatio_addere per repositorium
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;
		Entitas*             entitas1;
		Entitas*             entitas2;
		Relatio*             relatio;

		imprimere("\n--- Probans relatio_addere per repositorium ---\n");

		_delere_log_si_existit();

		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);

		/* Creare duas entitates */
		entitas1 = repo->entitas_creare(repo->datum, "Folder");
		entitas2 = repo->entitas_creare(repo->datum, "File");
		CREDO_NON_NIHIL(entitas1);
		CREDO_NON_NIHIL(entitas2);

		/* Addere relationem */
		relatio = repo->relatio_addere(repo->datum, entitas1, "contains", entitas2->id);
		CREDO_NON_NIHIL(relatio);
		CREDO_NON_NIHIL(relatio->id);

		/* Verificare relationem in memoria */
		CREDO_AEQUALIS_I32(entitas_numerus_relationum(entitas1), I);

		/* Debet habere tres eventus (2x creare + relatio) */
		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), III);

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Probare replay - creare, claudere, aperire
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;
		Entitas*             entitas;
		chorda*              entitas_id;
		GraphusEntitatum*    graphus;
		InternamentumChorda* intern;
		chorda*              clavis;
		chorda*              valor;
		chorda*              nota;

		imprimere("\n--- Probans replay (creare, claudere, aperire) ---\n");

		_delere_log_si_existit();

		/* Prima sessio: creare datum */
		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);

		entitas = repo->entitas_creare(repo->datum, "Document");
		CREDO_NON_NIHIL(entitas);

		/* Salvare ID pro verificare post replay */
		entitas_id = entitas->id;

		repo->proprietas_ponere(repo->datum, entitas, "name", "Test Document");
		repo->proprietas_ponere(repo->datum, entitas, "author", "Fran");
		repo->nota_addere(repo->datum, entitas, "#important");

		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), IV);

		graphus_persistens_claudere(gp);

		/* Secunda sessio: aperire et verificare replay */
		gp = graphus_persistens_aperire(piscina, VIA_LOG_PROBATIO);
		CREDO_NON_NIHIL(gp);

		repo = graphus_persistens_repositorium(gp);
		graphus = graphus_persistens_graphus(gp);
		intern = graphus_entitatum_internamentum(graphus);

		/* Eventus debet esse IV (replayed) */
		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), IV);

		/* Graphus debet habere I entitatem */
		CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), I);

		/* Capere entitatem per ID */
		entitas = repo->capere_entitatem(repo->datum, entitas_id);
		CREDO_NON_NIHIL(entitas);

		/* Verificare proprietates */
		clavis = chorda_internare_ex_literis(intern, "name");
		valor = entitas_proprietas_capere(entitas, clavis);
		CREDO_NON_NIHIL(valor);

		clavis = chorda_internare_ex_literis(intern, "author");
		valor = entitas_proprietas_capere(entitas, clavis);
		CREDO_NON_NIHIL(valor);

		/* Verificare notam */
		nota = chorda_internare_ex_literis(intern, "#important");
		CREDO_VERUM(entitas_nota_habet(entitas, nota));

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Probare replay cum relationibus
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;
		Entitas*             folder;
		Entitas*             file1;
		Entitas*             file2;
		chorda*              folder_id;
		GraphusEntitatum*    graphus;
		InternamentumChorda* intern;
		chorda*              genus_contains;
		Xar*                 relatae;

		imprimere("\n--- Probans replay cum relationibus ---\n");

		_delere_log_si_existit();

		/* Prima sessio */
		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);

		folder = repo->entitas_creare(repo->datum, "Folder");
		file1 = repo->entitas_creare(repo->datum, "File");
		file2 = repo->entitas_creare(repo->datum, "File");

		folder_id = folder->id;

		repo->relatio_addere(repo->datum, folder, "contains", file1->id);
		repo->relatio_addere(repo->datum, folder, "contains", file2->id);

		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), V);

		graphus_persistens_claudere(gp);

		/* Secunda sessio */
		gp = graphus_persistens_aperire(piscina, VIA_LOG_PROBATIO);
		CREDO_NON_NIHIL(gp);

		repo = graphus_persistens_repositorium(gp);
		graphus = graphus_persistens_graphus(gp);
		intern = graphus_entitatum_internamentum(graphus);

		/* Verificare entitates */
		CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), III);

		folder = repo->capere_entitatem(repo->datum, folder_id);
		CREDO_NON_NIHIL(folder);

		/* Verificare relationes */
		CREDO_AEQUALIS_I32(entitas_numerus_relationum(folder), II);

		genus_contains = chorda_internare_ex_literis(intern, "contains");
		relatae = repo->capere_entitates_relatae(repo->datum, folder, genus_contains);
		CREDO_NON_NIHIL(relatae);
		CREDO_AEQUALIS_I32(xar_numerus(relatae), II);

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Probare continuare scribere post replay
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;
		Entitas*             entitas;
		GraphusEntitatum*    graphus;

		imprimere("\n--- Probans continuare scribere post replay ---\n");

		_delere_log_si_existit();

		/* Prima sessio */
		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);

		repo->entitas_creare(repo->datum, "Entity1");
		repo->entitas_creare(repo->datum, "Entity2");

		graphus_persistens_claudere(gp);

		/* Secunda sessio - aperire et addere plus */
		gp = graphus_persistens_aperire(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);
		graphus = graphus_persistens_graphus(gp);

		CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), II);

		/* Addere novam entitatem */
		entitas = repo->entitas_creare(repo->datum, "Entity3");
		CREDO_NON_NIHIL(entitas);

		CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), III);
		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), III);

		graphus_persistens_claudere(gp);

		/* Tertia sessio - verificare omnia */
		gp = graphus_persistens_aperire(piscina, VIA_LOG_PROBATIO);
		graphus = graphus_persistens_graphus(gp);

		CREDO_AEQUALIS_I32(graphus_entitatum_numerus(graphus), III);
		CREDO_AEQUALIS_I32(graphus_persistens_numerus_eventuum(gp), III);

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Probare capere_radices
	 * ================================================== */

	{
		GraphusPersistens*   gp;
		EntitasRepositorium* repo;
		Entitas*             root1;
		Entitas*             root2;
		Xar*                 radices;

		imprimere("\n--- Probans capere_radices ---\n");

		_delere_log_si_existit();

		gp = graphus_persistens_creare(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);

		/* Creare entitates */
		root1 = repo->entitas_creare(repo->datum, "Root");
		root2 = repo->entitas_creare(repo->datum, "Root");
		(vacuum)repo->entitas_creare(repo->datum, "Child");  /* non_root - non usatum */

		/* Addere nota #root ad radices */
		repo->nota_addere(repo->datum, root1, "#root");
		repo->nota_addere(repo->datum, root2, "#root");

		/* Capere radices */
		radices = repo->capere_radices(repo->datum);
		CREDO_NON_NIHIL(radices);
		CREDO_AEQUALIS_I32(xar_numerus(radices), II);

		graphus_persistens_claudere(gp);

		/* Verificare post replay */
		gp = graphus_persistens_aperire(piscina, VIA_LOG_PROBATIO);
		repo = graphus_persistens_repositorium(gp);

		radices = repo->capere_radices(repo->datum);
		CREDO_NON_NIHIL(radices);
		CREDO_AEQUALIS_I32(xar_numerus(radices), II);

		graphus_persistens_claudere(gp);
	}

	/* ==================================================
	 * Purificare
	 * ================================================== */

	_delere_log_si_existit();

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
