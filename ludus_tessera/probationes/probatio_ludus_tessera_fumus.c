/* probatio_ludus_tessera_fumus.c - fumus aedificationis (A0)
 *
 * Limes amalgamatis tenet: tessera per tessera.h amalgamatis SOLUM
 * (piscina et pons sui, renominati) iuxta bibliothecas radicis
 * (piscina, credo) in eadem plagula - nulla collisio typorum aut
 * symbolorum. Quadrum unum per pontem memoriae: cellula 'Z' ad (II, I)
 * positionem CUP "\033[2;3H" et signum emittit.
 */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "tessera.h"
#include <stdio.h>
#include <string.h>

/* Octeti capti continentne literas? */
interior b32
_continet (
           TesseraChorda  captum,
      constans character* literae)
{
    i32 n = (i32)strlen(literae);
    i32 k;

    per (k = ZEPHYRUM; k + n <= (i32)captum.mensura; k++)
    {
        si (memcmp(captum.datum + k, literae, (memoriae_index)n)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
    TesseraPiscina* tp;

    piscina = piscina_generare_dynamicum("probatio_ludus_tessera_fumus",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I. tessera per amalgama, quadrum unum ---\n");
    tp = tessera_piscina_generare_dynamicum("fumus_tesserae", 1048576);
    CREDO_VERUM (tp != NIHIL);
    si (tp)
    {
        TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(tp, VIII,
            IV);
        TesseraOpus* opus = tessera_aperire(tp, &pm->pons);

        CREDO_VERUM (opus != NIHIL);
        si (opus)
        {
            tessera_cellulam_ponere(opus, II, I, (integer)'Z',
                tessera_stilus_nativus());
            CREDO_VERUM (tessera_praesentare(opus));
            CREDO_VERUM (_continet(tessera_pons_memoriae_captum(pm),
                "\033[2;3H"));
            CREDO_VERUM (_continet(tessera_pons_memoriae_captum(pm),
                "Z"));
            tessera_claudere(opus);
        }
        tessera_piscina_destruere(tp);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
