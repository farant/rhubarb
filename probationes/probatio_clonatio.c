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

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_clonatio", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    (vacuum)filum_arborem_delere(AREA);
    CREDO_VERUM(filum_directorium_creare_cum_parentibus(FONS "/sub"));
    CREDO_VERUM(filum_scribere_literis(FONS "/a.txt", "alpha\n"));
    CREDO_VERUM(filum_scribere_literis(FONS "/sub/b.txt", "beta\n"));


    /* ========================================================
     * PROBARE: arbor tota uno vocamine
     * ======================================================== */

    imprimere("\n--- arbor directorii ---\n");
    CREDO_VERUM(clonatio_facere(FONS, DESTINATA));
    CREDO_VERUM(_textus_est(DESTINATA "/a.txt", "alpha\n", piscina));
    CREDO_VERUM(_textus_est(DESTINATA "/sub/b.txt", "beta\n", piscina));

    /* copy-on-write: scriptura in clone fontem non tangit */
    CREDO_VERUM(filum_scribere_literis(DESTINATA "/a.txt",
        "mutatum\n"));
    CREDO_VERUM(_textus_est(FONS "/a.txt", "alpha\n", piscina));
    CREDO_VERUM(_textus_est(DESTINATA "/a.txt", "mutatum\n", piscina));


    /* ========================================================
     * PROBARE: plagula singula
     * ======================================================== */

    imprimere("\n--- plagula singula ---\n");
    CREDO_VERUM(clonatio_facere(FONS "/sub/b.txt",
        AREA "/b_clonata.txt"));
    CREDO_VERUM(_textus_est(AREA "/b_clonata.txt", "beta\n", piscina));


    /* ========================================================
     * PROBARE: recusationes
     * ======================================================== */

    imprimere("\n--- recusationes ---\n");
    errno = ZEPHYRUM;
    CREDO_FALSUM(clonatio_facere(FONS, DESTINATA));
    CREDO_AEQUALIS_S32((s32)errno, (s32)EEXIST);
    errno = ZEPHYRUM;
    CREDO_FALSUM(clonatio_facere(AREA "/nusquam", AREA "/alibi"));
    CREDO_AEQUALIS_S32((s32)errno, (s32)ENOENT);
    CREDO_FALSUM(filum_existit(AREA "/alibi"));
    CREDO_FALSUM(clonatio_facere(NIHIL, AREA "/alibi"));

    (vacuum)filum_arborem_delere(AREA);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
