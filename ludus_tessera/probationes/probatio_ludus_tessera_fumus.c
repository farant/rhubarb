/* probatio_ludus_tessera_fumus.c - fumus aedificationis (A0; A2: per
 * fontes tesserae)
 *
 * tessera per fontes suos (ut musivum) iuxta bibliothecas radicis in
 * eadem plagula et nexu. Quadrum unum per pontem memoriae: cellula
 * 'Z' ad (II, I) positionem CUP "\033[2;3H" et signum emittit.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include <stdio.h>
#include <string.h>

/* Octeti capti continentne literas? */
interior b32
_continet (
                 chorda  captum,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);
    i32 k;

    per (k = ZEPHYRUM; k + n <= captum.mensura; k++)
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

    piscina = piscina_generare_dynamicum("probatio_ludus_tessera_fumus",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I. tessera per fontes, quadrum unum ---\n");
    {
        TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(piscina,
            VIII, IV);
        TesseraOpus* opus = tessera_aperire(piscina, &pm->pons);

        CREDO_VERUM (opus != NIHIL);
        si (opus)
        {
            tessera_cellulam_ponere(opus, II, I, (i32)'Z',
                tessera_stilus_nativus());
            CREDO_VERUM (tessera_praesentare(opus));
            CREDO_VERUM (_continet(tessera_pons_memoriae_captum(pm),
                "\033[2;3H"));
            CREDO_VERUM (_continet(tessera_pons_memoriae_captum(pm),
                "Z"));
            tessera_claudere(opus);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
