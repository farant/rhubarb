/* probatio_via.c - Probationes Viae */
#include "latina.h"
#include "via.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


    /* ========================================================
	 * PROBARE: via_iungere - iunctio simplex
	 * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda partes[II];
        chorda fructus;

        partes[ZEPHYRUM]  = chorda_ex_literis("foo", piscina);
        partes[I]         = chorda_ex_literis("bar", piscina);

        fructus = via_iungere(partes, II, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_iungere - partes multiplices
	 * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda partes[IV];
        chorda fructus;

        partes[ZEPHYRUM]  = chorda_ex_literis("usr", piscina);
        partes[I]         = chorda_ex_literis("local", piscina);
        partes[II]        = chorda_ex_literis("bin", piscina);
        partes[III]       = chorda_ex_literis("test", piscina);

        fructus = via_iungere(partes, IV, piscina);

        CREDO_CHORDA_AEQUALIS(fructus,
                              chorda_ex_literis("usr/local/bin/test",
                              piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_iungere - cum separator terminali
	 * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda partes[II];
        chorda fructus;

        partes[ZEPHYRUM]  = chorda_ex_literis("foo/", piscina);
        partes[I]         = chorda_ex_literis("bar", piscina);

        fructus          = via_iungere(partes, II, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_iungere - cum separator initiali
	 * ======================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda partes[II];
        chorda fructus;

        partes[ZEPHYRUM]  = chorda_ex_literis("foo", piscina);
        partes[I]         = chorda_ex_literis("/bar", piscina);

        fructus          = via_iungere(partes, II, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_iungere - cum parte vacua
	 * ======================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda partes[III];
        chorda fructus;
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;

        partes[ZEPHYRUM]  = chorda_ex_literis("foo", piscina);
        partes[I]         = vacua;
        partes[II]        = chorda_ex_literis("bar", piscina);

        fructus          = via_iungere(partes, III, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_iungere - via absoluta
	 * ======================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda partes[III];
        chorda fructus;

        partes[ZEPHYRUM]  = chorda_ex_literis("/usr", piscina);
        partes[I]         = chorda_ex_literis("local", piscina);
        partes[II]        = chorda_ex_literis("bin", piscina);

        fructus          = via_iungere(partes, III, piscina);

        CREDO_CHORDA_AEQUALIS(fructus,
            chorda_ex_literis("/usr/local/bin",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_normalizare - punctum singularis
	 * ======================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo/./bar", piscina);
        fructus  = via_normalizare(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_normalizare - puncta duplicia
	 * ======================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo/../bar", piscina);
        fructus  = via_normalizare(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_normalizare - separatores duplicati
	 * ======================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo//bar", piscina);
        fructus  = via_normalizare(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_normalizare - via absoluta cum ..
	 * ======================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("/foo/../bar", piscina);
        fructus  = via_normalizare(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_normalizare - .. supra radicem
	 * ======================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("/../foo", piscina);
        fructus  = via_normalizare(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/foo",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_normalizare - via relativa cum ..
	 * ======================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("../foo", piscina);
        fructus  = via_normalizare(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("../foo",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_normalizare - radix solum
	 * ======================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("/", piscina);
        fructus  = via_normalizare(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/", piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_normalizare - vacuum ad punctum
	 * ======================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("./.", piscina);
        fructus  = via_normalizare(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".", piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_directorium - via normalis
	 * ======================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("/foo/bar/baz.txt", piscina);
        fructus  = via_directorium(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/foo/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_directorium - radix
	 * ======================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("/foo", piscina);
        fructus  = via_directorium(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("/", piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_directorium - nullus directorium
	 * ======================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo.txt", piscina);
        fructus  = via_directorium(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".", piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_directorium - via relativa
	 * ======================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo/bar", piscina);
        fructus  = via_directorium(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_nomen - via absoluta
	 * ======================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("/foo/bar/baz.txt", piscina);
        fructus  = via_nomen(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("baz.txt",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_nomen - sine directorium
	 * ======================================================== */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo.txt", piscina);
        fructus  = via_nomen(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo.txt",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_nomen - cum separator terminali
	 * ======================================================== */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        /* POSIX basename - olim "" (lapide briar-feedback bugs/012) */
        via      = chorda_ex_literis("/foo/bar/", piscina);
        fructus  = via_nomen(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: par nomen/directorium - casus limitum (POSIX)
	 * ======================================================== */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        /* via, directorium, nomen - POSIX dirname/basename praeter
         * "/" cuius nomen "" est consulto (radix nomen non habet) */
        constans character* casus_limitum[][III] = {
            { "/foo/bar/",  "/foo", "bar" },
            { "/foo/bar//", "/foo", "bar" },
            { "/foo//bar",  "/foo", "bar" },
            { "foo/",       ".",    "foo" },
            { "/foo",       "/",    "foo" },
            { "/",          "/",    ""    },
            { "//",         "/",    ""    }
        };
        i32 k;

        per (k = 0; k < (i32)(magnitudo(casus_limitum)
                              / magnitudo(casus_limitum[0])); k++)
        {
            chorda via = chorda_ex_literis(casus_limitum[k][0],
                piscina);

            CREDO_CHORDA_AEQUALIS(via_directorium(via, piscina),
                chorda_ex_literis(casus_limitum[k][I], piscina));
            CREDO_CHORDA_AEQUALIS(via_nomen(via, piscina),
                chorda_ex_literis(casus_limitum[k][II], piscina));
        }
    }
}


    /* ========================================================
	 * PROBARE: via_nomen_radix - cum extensione
	 * ======================================================== */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo.txt", piscina);
        fructus  = via_nomen_radix(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_nomen_radix - sine extensione
	 * ======================================================== */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo", piscina);
        fructus  = via_nomen_radix(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("foo",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_nomen_radix - extensio composita
	 * ======================================================== */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("archive.tar.gz", piscina);
        fructus  = via_nomen_radix(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("archive.tar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_nomen_radix - dotfile
	 * ======================================================== */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis(".bashrc", piscina);
        fructus  = via_nomen_radix(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".bashrc",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_nomen_radix - cum via
	 * ======================================================== */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("/foo/bar/baz.txt", piscina);
        fructus  = via_nomen_radix(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("baz",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_extensio - singularis
	 * ======================================================== */

interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo.txt", piscina);
        fructus  = via_extensio(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".txt",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_extensio - composita
	 * ======================================================== */

interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("archive.tar.gz", piscina);
        fructus  = via_extensio(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".gz",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_extensio - dotfile
	 * ======================================================== */

interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis(".bashrc", piscina);
        fructus  = via_extensio(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("", piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_extensio - sine extensione
	 * ======================================================== */

interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("foo", piscina);
        fructus  = via_extensio(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis("", piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_extensio - cum via
	 * ======================================================== */

interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via;
        chorda fructus;

        via      = chorda_ex_literis("/foo/bar/baz.txt", piscina);
        fructus  = via_extensio(via, piscina);

        CREDO_CHORDA_AEQUALIS(fructus, chorda_ex_literis(".txt",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_est_absoluta
	 * ======================================================== */

interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via_abs;
        chorda via_rel;

        via_abs = chorda_ex_literis("/foo/bar", piscina);
        via_rel = chorda_ex_literis("foo/bar", piscina);

        CREDO_VERUM(via_est_absoluta(via_abs));
        CREDO_FALSUM(via_est_absoluta(via_rel));
    }
}


    /* ========================================================
	 * PROBARE: via_est_relativa
	 * ======================================================== */

interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via_abs;
        chorda via_rel;

        via_abs = chorda_ex_literis("/foo/bar", piscina);
        via_rel = chorda_ex_literis("foo/bar", piscina);

        CREDO_FALSUM(via_est_relativa(via_abs));
        CREDO_VERUM(via_est_relativa(via_rel));
    }
}


    /* ========================================================
	 * PROBARE: via_directorium_currens
	 * ======================================================== */

interior vacuum
_sectio_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda cwd;

        cwd = via_directorium_currens(piscina);

        CREDO_NON_NIHIL(cwd.datum);
        CREDO_MAIOR_I32(cwd.mensura, ZEPHYRUM);
        CREDO_VERUM(via_est_absoluta(cwd));
    }
}


    /* ========================================================
	 * PROBARE: via_absoluta - via relativa
	 * ======================================================== */

interior vacuum
_sectio_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via_rel;
        chorda fructus;

        via_rel = chorda_ex_literis("foo/bar", piscina);
        fructus = via_absoluta(via_rel, piscina);

        CREDO_NON_NIHIL(fructus.datum);
        CREDO_VERUM(via_est_absoluta(fructus));
        CREDO_CHORDA_TERMINATUR(fructus, chorda_ex_literis("foo/bar",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_absoluta - via iam absoluta
	 * ======================================================== */

interior vacuum
_sectio_tricesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via_abs;
        chorda fructus;

        via_abs = chorda_ex_literis("/usr/local/bin", piscina);
        fructus = via_absoluta(via_abs, piscina);

        CREDO_CHORDA_AEQUALIS(fructus,
            chorda_ex_literis("/usr/local/bin",
            piscina));
    }
}


    /* ========================================================
	 * PROBARE: via_absoluta - cum .. relativa
	 * ======================================================== */

interior vacuum
_sectio_tricesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        chorda via_rel;
        chorda fructus;

        via_rel = chorda_ex_literis("foo/../bar", piscina);
        fructus = via_absoluta(via_rel, piscina);

        CREDO_NON_NIHIL(fructus.datum);
        CREDO_VERUM(via_est_absoluta(fructus));
        CREDO_CHORDA_TERMINATUR(fructus, chorda_ex_literis("/bar",
            piscina));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "via_iungere (simplex)", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_iungere (multiplex)", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_iungere (separator terminalis)", _sectio_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "via_iungere (separator initialis)", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_iungere (pars vacua)", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_iungere (absoluta)", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_normalizare (punctum)", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_normalizare (puncta duplicia)", _sectio_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_normalizare (separatores duplicati)", _sectio_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "via_normalizare (absoluta cum ..)", _sectio_decima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_normalizare (.. supra radicem)", _sectio_decima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "via_normalizare (relativa cum ..)", _sectio_decima_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "via_normalizare (radix)", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_normalizare (vacuum)", _sectio_decima_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_directorium (normalis)", _sectio_decima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_directorium (radix)", _sectio_decima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_directorium (nullus)", _sectio_decima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_directorium (relativa)", _sectio_decima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_nomen (absoluta)", _sectio_decima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_nomen (sine directorium)", _sectio_vicesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_nomen (separator terminalis)", _sectio_vicesima_prima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "par nomen/directorium (limites)", _sectio_vicesima_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "via_nomen_radix (cum extensione)", _sectio_vicesima_tertia,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "via_nomen_radix (sine extensione)", _sectio_vicesima_quarta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "via_nomen_radix (extensio composita)", _sectio_vicesima_quinta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "via_nomen_radix (dotfile)", _sectio_vicesima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_nomen_radix (cum via)", _sectio_vicesima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "via_extensio (singularis)", _sectio_vicesima_octava, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "via_extensio (composita)", _sectio_vicesima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_extensio (dotfile)", _sectio_tricesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_extensio (sine)", _sectio_tricesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_extensio (cum via)", _sectio_tricesima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_est_absoluta", _sectio_tricesima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_est_relativa", _sectio_tricesima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "via_directorium_currens", _sectio_tricesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_absoluta (relativa)", _sectio_tricesima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "via_absoluta (iam absoluta)", _sectio_tricesima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "via_absoluta (cum ..)", _sectio_tricesima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("via", SECTIONES);
}
