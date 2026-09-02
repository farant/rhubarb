/* probatio_credo.c - PORTA NATIVA contractus exitus credo.
 *
 * Probatio quae NIHIL asserit viridis erat: fractae nullae, ergo
 * 'omnia praeterierunt'. Corpus absens, plica mota, gyrus qui nullam
 * plagulam invenit - lumen viride, nihil visum (2026-09-02: porta
 * corporis silvae e plica aliena lata 'FRACTA: corpus non apertum'
 * supra 'Conditio: OMNIA PRAETERIERUNT' imprimebat). Nunc totalis
 * ZEPHYRUM = NON praeteritum ('NIHIL PROBATUM').
 *
 * Assertio PRIMA huius probationis est interrogatio ANTE ullam
 * assertionem - sola sedes ubi totalis ZEPHYRUM legi potest sine
 * refectione tabulae. CREDO_CULPA in filio furcato probatur ne
 * tabula parentis fractura consulto declarata polluatur.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include <stdio.h>
#include <unistd.h>

integer
principale (vacuum)
{
                      b32  praeteritus;
                      b32  vacua_praeterit;
           memoriae_index  ante;
                  Piscina* piscina;
    CredoProcessusFructus  filius;

    piscina = piscina_generare_dynamicum("probatio_credo", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* I. Nihil probatum = non praeteritum. Lectio ANTE assertionem
     * primam: totalis ZEPHYRUM hic et nusquam alibi. */
    vacua_praeterit = credo_omnia_praeterierunt();

    imprimere("\n--- Contractus exitus ---\n");
    CREDO_FALSUM(vacua_praeterit);
    CREDO_VERUM(credo_numerus_totalis() == I);
    CREDO_VERUM(credo_omnia_praeterierunt());

    /* II. CREDO_CULPA fracturam unam numerat et praeteritum negat -
     * in filio furcato: 'F' et relatio FRACTA infra ex filio veniunt
     * et EXSPECTANTUR. */
    imprimere("\n--- CREDO_CULPA (fractura in filio exspectata) ---\n");
    filius = credo_processus_incipere();
    si (filius.in_filio)
    {
        ante = credo_numerus_fracti();
        CREDO_CULPA("culpa consulto declarata");
        si (   credo_numerus_fracti() == ante + I
            && !credo_omnia_praeterierunt())
        {
            credo_processus_filium_finire();
        }
        _exit(I);
    }
    credo_processus_iudicare(&filius, "credo_culpa",
        "CREDO_CULPA fracturam unam numerat et praeteritum negat",
        CREDO_MORA_ORDINARIA_MS, ZEPHYRUM, __FILE__, __LINE__);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
