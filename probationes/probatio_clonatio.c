/* probatio_clonatio.c - Probationes clonationis (clonefile, pythonica
 * S3). Area in /tmp (radix temporaria: eadem area APFS ac arbor, et
 * fabrica eam ingressum non putat). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "credo.h"
#include "clonatio.h"
#include <stdio.h>
#include <errno.h>

#define AREA      "/tmp/probatio_clonatio"
#define FONS      AREA "/fons"
#define DESTINATA AREA "/destinata"

interior b32
_textus_est (
       constans character* via,
       constans character* exspectatum,
                  Piscina* piscina)
{
    chorda contentum = filum_legere_totum(via, piscina);

    redde contentum.datum != NIHIL
        && chorda_aequalis_literis(contentum, exspectatum);
}

/* fixura suitae: fons communis (sectiones eum legunt, numquam mutant) */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    (vacuum)c;
    (vacuum)filum_arborem_delere(AREA);
    CREDO_VERUM(filum_directorium_creare_cum_parentibus(FONS "/sub"));
    CREDO_VERUM(filum_scribere_literis(FONS "/a.txt", "alpha\n"));
    CREDO_VERUM(filum_scribere_literis(FONS "/sub/b.txt", "beta\n"));
}

interior vacuum
_suitam_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    (vacuum)filum_arborem_delere(AREA);
}


/* ========================================================
 * PROBARE: arbor tota uno vocamine
 * ======================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    /* ordine liber: destinata sectionis tertiae (si prior cucurrit)
     * removetur */
    (vacuum)filum_arborem_delere(DESTINATA);
    CREDO_VERUM(clonatio_facere(FONS, DESTINATA));
    CREDO_VERUM(_textus_est(DESTINATA "/a.txt", "alpha\n", piscina));
    CREDO_VERUM(_textus_est(DESTINATA "/sub/b.txt", "beta\n", piscina));

    /* copy-on-write: scriptura in clone fontem non tangit */
    CREDO_VERUM(filum_scribere_literis(DESTINATA "/a.txt",
        "mutatum\n"));
    CREDO_VERUM(_textus_est(FONS "/a.txt", "alpha\n", piscina));
    CREDO_VERUM(_textus_est(DESTINATA "/a.txt", "mutatum\n", piscina));
}


/* ========================================================
 * PROBARE: plagula singula
 * ======================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    CREDO_VERUM(clonatio_facere(FONS "/sub/b.txt",
        AREA "/b_clonata.txt"));
    CREDO_VERUM(_textus_est(AREA "/b_clonata.txt", "beta\n", piscina));
}


/* ========================================================
 * PROBARE: recusationes
 * ======================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    /* ordine liber: destinata iam exstans (olim a sectione prima) */
    si (!filum_existit(DESTINATA))
    {
        (vacuum)clonatio_facere(FONS, DESTINATA);
    }
    errno = ZEPHYRUM;
    CREDO_FALSUM(clonatio_facere(FONS, DESTINATA));
    CREDO_AEQUALIS_S32((s32)errno, (s32)EEXIST);
    errno = ZEPHYRUM;
    CREDO_FALSUM(clonatio_facere(AREA "/nusquam", AREA "/alibi"));
    CREDO_AEQUALIS_S32((s32)errno, (s32)ENOENT);
    CREDO_FALSUM(filum_existit(AREA "/alibi"));
    CREDO_FALSUM(clonatio_facere(NIHIL, AREA "/alibi"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "arbor directorii", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "plagula singula", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("clonatio", SECTIONES,
        _suitam_parare,
        _suitam_purgare);
}
