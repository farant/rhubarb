/* probatio_via.c - Probationes Viae */
#include "latina.h"
#include "via.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

	/* Aperire credo et piscina */
	piscina = piscina_generare_dynamicum("probatio_via", MMMMXCVI);
	si (!piscina)
	{
		imprimere("FRACTA: piscina_generatio\n");
		redde I;
	}
	credo_aperire(piscina);


	/* ========================================================
	 * PROBARE: via_iungere - iunctio simplex
	 * ======================================================== */

	{
		chorda partes[II];
		chorda fructus;

		imprimere("\n--- Probans via_iungere (simplex) ---\n");

		partes[ZEPHYRUM] = chorda_ex_literis("foo", piscina);
		partes[I]        = chorda_ex_literis("bar", piscina);

		fructus = via_iungere(partes, II, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_iungere - partes multiplices
	 * ======================================================== */

	{
		chorda partes[IV];
		chorda fructus;

		imprimere("\n--- Probans via_iungere (multiplex) ---\n");

		partes[ZEPHYRUM] = chorda_ex_literis("usr", piscina);
		partes[I]        = chorda_ex_literis("local", piscina);
		partes[II]       = chorda_ex_literis("bin", piscina);
		partes[III]      = chorda_ex_literis("test", piscina);

		fructus = via_iungere(partes, IV, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, 
                              chorda_ex_literis("usr/local/bin/test", piscina));
	}


	/* ========================================================
	 * PROBARE: via_iungere - cum separator terminali
	 * ======================================================== */

	{
		chorda partes[II];
		chorda fructus;

		imprimere("\n--- Probans via_iungere (separator terminalis) ---\n");

		partes[ZEPHYRUM] = chorda_ex_literis("foo/", piscina);
		partes[I]        = chorda_ex_literis("bar", piscina);

		fructus          = via_iungere(partes, II, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_iungere - cum separator initiali
	 * ======================================================== */

	{
		chorda partes[II];
		chorda fructus;

		imprimere("\n--- Probans via_iungere (separator initialis) ---\n");

		partes[ZEPHYRUM] = chorda_ex_literis("foo", piscina);
		partes[I]        = chorda_ex_literis("/bar", piscina);

		fructus          = via_iungere(partes, II, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_iungere - cum parte vacua
	 * ======================================================== */

	{
		chorda partes[III];
		chorda fructus;
		chorda vacua;

		imprimere("\n--- Probans via_iungere (pars vacua) ---\n");

		vacua.datum      = NIHIL;
		vacua.mensura    = ZEPHYRUM;

		partes[ZEPHYRUM] = chorda_ex_literis("foo", piscina);
		partes[I]        = vacua;
		partes[II]       = chorda_ex_literis("bar", piscina);

		fructus          = via_iungere(partes, III, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_iungere - via absoluta
	 * ======================================================== */

	{
		chorda partes[III];
		chorda fructus;

		imprimere("\n--- Probans via_iungere (absoluta) ---\n");

		partes[ZEPHYRUM] = chorda_ex_literis("/usr", piscina);
		partes[I]        = chorda_ex_literis("local", piscina);
		partes[II]       = chorda_ex_literis("bin", piscina);

		fructus          = via_iungere(partes, III, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/usr/local/bin", piscina));
	}


	/* ========================================================
	 * PROBARE: via_normalizare - punctum singularis
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_normalizare (punctum) ---\n");

		via     = chorda_ex_literis("foo/./bar", piscina);
		fructus = via_normalizare(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_normalizare - puncta duplicia
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_normalizare (puncta duplicia) ---\n");

		via     = chorda_ex_literis("foo/../bar", piscina);
		fructus = via_normalizare(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_normalizare - separatores duplicati
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_normalizare (separatores duplicati) ---\n");

		via     = chorda_ex_literis("foo//bar", piscina);
		fructus = via_normalizare(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_normalizare - via absoluta cum ..
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_normalizare (absoluta cum ..) ---\n");

		via     = chorda_ex_literis("/foo/../bar", piscina);
		fructus = via_normalizare(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_normalizare - .. supra radicem
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_normalizare (.. supra radicem) ---\n");

		via     = chorda_ex_literis("/../foo", piscina);
		fructus = via_normalizare(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/foo", piscina));
	}


	/* ========================================================
	 * PROBARE: via_normalizare - via relativa cum ..
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_normalizare (relativa cum ..) ---\n");

		via     = chorda_ex_literis("../foo", piscina);
		fructus = via_normalizare(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("../foo", piscina));
	}


	/* ========================================================
	 * PROBARE: via_normalizare - radix solum
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_normalizare (radix) ---\n");

		via     = chorda_ex_literis("/", piscina);
		fructus = via_normalizare(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/", piscina));
	}


	/* ========================================================
	 * PROBARE: via_normalizare - vacuum ad punctum
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_normalizare (vacuum) ---\n");

		via     = chorda_ex_literis("./.", piscina);
		fructus = via_normalizare(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".", piscina));
	}


	/* ========================================================
	 * PROBARE: via_directorium - via normalis
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_directorium (normalis) ---\n");

		via     = chorda_ex_literis("/foo/bar/baz.txt", piscina);
		fructus = via_directorium(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/foo/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_directorium - radix
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_directorium (radix) ---\n");

		via     = chorda_ex_literis("/foo", piscina);
		fructus = via_directorium(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/", piscina));
	}


	/* ========================================================
	 * PROBARE: via_directorium - nullus directorium
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_directorium (nullus) ---\n");

		via     = chorda_ex_literis("foo.txt", piscina);
		fructus = via_directorium(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".", piscina));
	}


	/* ========================================================
	 * PROBARE: via_directorium - via relativa
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_directorium (relativa) ---\n");

		via     = chorda_ex_literis("foo/bar", piscina);
		fructus = via_directorium(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo", piscina));
	}


	/* ========================================================
	 * PROBARE: via_nomen - via absoluta
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_nomen (absoluta) ---\n");

		via     = chorda_ex_literis("/foo/bar/baz.txt", piscina);
		fructus = via_nomen(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("baz.txt", piscina));
	}


	/* ========================================================
	 * PROBARE: via_nomen - sine directorium
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_nomen (sine directorium) ---\n");

		via     = chorda_ex_literis("foo.txt", piscina);
		fructus = via_nomen(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo.txt", piscina));
	}


	/* ========================================================
	 * PROBARE: via_nomen - cum separator terminali
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_nomen (separator terminalis) ---\n");

		via     = chorda_ex_literis("/foo/bar/", piscina);
		fructus = via_nomen(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("", piscina));
	}


	/* ========================================================
	 * PROBARE: via_nomen_radix - cum extensione
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_nomen_radix (cum extensione) ---\n");

		via     = chorda_ex_literis("foo.txt", piscina);
		fructus = via_nomen_radix(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo", piscina));
	}


	/* ========================================================
	 * PROBARE: via_nomen_radix - sine extensione
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_nomen_radix (sine extensione) ---\n");

		via     = chorda_ex_literis("foo", piscina);
		fructus = via_nomen_radix(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo", piscina));
	}


	/* ========================================================
	 * PROBARE: via_nomen_radix - extensio composita
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_nomen_radix (extensio composita) ---\n");

		via     = chorda_ex_literis("archive.tar.gz", piscina);
		fructus = via_nomen_radix(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("archive.tar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_nomen_radix - dotfile
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_nomen_radix (dotfile) ---\n");

		via     = chorda_ex_literis(".bashrc", piscina);
		fructus = via_nomen_radix(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".bashrc", piscina));
	}


	/* ========================================================
	 * PROBARE: via_nomen_radix - cum via
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_nomen_radix (cum via) ---\n");

		via     = chorda_ex_literis("/foo/bar/baz.txt", piscina);
		fructus = via_nomen_radix(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("baz", piscina));
	}


	/* ========================================================
	 * PROBARE: via_extensio - singularis
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_extensio (singularis) ---\n");

		via     = chorda_ex_literis("foo.txt", piscina);
		fructus = via_extensio(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".txt", piscina));
	}


	/* ========================================================
	 * PROBARE: via_extensio - composita
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_extensio (composita) ---\n");

		via     = chorda_ex_literis("archive.tar.gz", piscina);
		fructus = via_extensio(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".gz", piscina));
	}


	/* ========================================================
	 * PROBARE: via_extensio - dotfile
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_extensio (dotfile) ---\n");

		via     = chorda_ex_literis(".bashrc", piscina);
		fructus = via_extensio(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("", piscina));
	}


	/* ========================================================
	 * PROBARE: via_extensio - sine extensione
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_extensio (sine) ---\n");

		via     = chorda_ex_literis("foo", piscina);
		fructus = via_extensio(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("", piscina));
	}


	/* ========================================================
	 * PROBARE: via_extensio - cum via
	 * ======================================================== */

	{
		chorda via;
		chorda fructus;

		imprimere("\n--- Probans via_extensio (cum via) ---\n");

		via     = chorda_ex_literis("/foo/bar/baz.txt", piscina);
		fructus = via_extensio(via, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".txt", piscina));
	}


	/* ========================================================
	 * PROBARE: via_est_absoluta
	 * ======================================================== */

	{
		chorda via_abs;
		chorda via_rel;

		imprimere("\n--- Probans via_est_absoluta ---\n");

		via_abs = chorda_ex_literis("/foo/bar", piscina);
		via_rel = chorda_ex_literis("foo/bar", piscina);

		CREDO_VERUM(via_est_absoluta(via_abs));
		CREDO_FALSUM(via_est_absoluta(via_rel));
	}


	/* ========================================================
	 * PROBARE: via_est_relativa
	 * ======================================================== */

	{
		chorda via_abs;
		chorda via_rel;

		imprimere("\n--- Probans via_est_relativa ---\n");

		via_abs = chorda_ex_literis("/foo/bar", piscina);
		via_rel = chorda_ex_literis("foo/bar", piscina);

		CREDO_FALSUM(via_est_relativa(via_abs));
		CREDO_VERUM(via_est_relativa(via_rel));
	}


	/* ========================================================
	 * PROBARE: via_directorium_currens
	 * ======================================================== */

	{
		chorda cwd;

		imprimere("\n--- Probans via_directorium_currens ---\n");

		cwd = via_directorium_currens(piscina);

		CREDO_NON_NIHIL(cwd.datum);
		CREDO_MAIOR_I32(cwd.mensura, ZEPHYRUM);
		CREDO_VERUM(via_est_absoluta(cwd));
	}


	/* ========================================================
	 * PROBARE: via_absoluta - via relativa
	 * ======================================================== */

	{
		chorda via_rel;
		chorda fructus;

		imprimere("\n--- Probans via_absoluta (relativa) ---\n");

		via_rel = chorda_ex_literis("foo/bar", piscina);
		fructus = via_absoluta(via_rel, piscina);

		CREDO_NON_NIHIL(fructus.datum);
		CREDO_VERUM(via_est_absoluta(fructus));
		CREDO_CHORDA_TERMINATUR(fructus, chorda_ex_literis("foo/bar", piscina));
	}


	/* ========================================================
	 * PROBARE: via_absoluta - via iam absoluta
	 * ======================================================== */

	{
		chorda via_abs;
		chorda fructus;

		imprimere("\n--- Probans via_absoluta (iam absoluta) ---\n");

		via_abs = chorda_ex_literis("/usr/local/bin", piscina);
		fructus = via_absoluta(via_abs, piscina);

		CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/usr/local/bin", piscina));
	}


	/* ========================================================
	 * PROBARE: via_absoluta - cum .. relativa
	 * ======================================================== */

	{
		chorda via_rel;
		chorda fructus;

		imprimere("\n--- Probans via_absoluta (cum ..) ---\n");

		via_rel = chorda_ex_literis("foo/../bar", piscina);
		fructus = via_absoluta(via_rel, piscina);

		CREDO_NON_NIHIL(fructus.datum);
		CREDO_VERUM(via_est_absoluta(fructus));
		CREDO_CHORDA_TERMINATUR(fructus, chorda_ex_literis("/bar", piscina));
	}


	/* ========================================================
	 * Compendium
	 * ======================================================== */

	imprimere("\n");
	credo_imprimere_compendium();

	praeteritus = credo_omnia_praeterierunt();

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
