/* probatio_saltuarius_res.c - Status app-latus (Chunk A1)
 *
 * Probantur: creatio (via_currens = radix), navigatio viae PURA
 * (intrare/ascendere per bibliothecam via - discus non tangitur),
 * fixum "/" in ascensu, cyclus vitae nuntii transitorii.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "saltuarius_res.h"
#include <stdio.h>

s32
principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_salt_res",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: creatio + navigatio viae
     * ======================================================== */

    {
        SaltuariusRes* res;

        imprimere("\n--- Probans creationem et vias ---\n");

        res = saltuarius_res_creare(piscina, "/a/b");
        CREDO_NON_NIHIL (res);
        CREDO_AEQUALIS_I32 ((i32)res->modus,
            (i32)SALT_MODUS_COLUMNAE);
        CREDO_VERUM (res->currens);
        CREDO_CHORDA_AEQUALIS_LITERIS (res->radix, "/a/b");
        CREDO_CHORDA_AEQUALIS_LITERIS (res->via_currens, "/a/b");

        saltuarius_res_intrare(res,
            chorda_ex_literis("fontes", piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS (res->via_currens,
            "/a/b/fontes");

        saltuarius_res_ascendere(res);
        CREDO_CHORDA_AEQUALIS_LITERIS (res->via_currens, "/a/b");

        /* supra radicem licet (radix initium, non carcer) */
        saltuarius_res_ascendere(res);
        CREDO_CHORDA_AEQUALIS_LITERIS (res->via_currens, "/a");
        saltuarius_res_ascendere(res);
        CREDO_CHORDA_AEQUALIS_LITERIS (res->via_currens, "/");

        /* "/" fixum manet */
        saltuarius_res_ascendere(res);
        CREDO_CHORDA_AEQUALIS_LITERIS (res->via_currens, "/");
    }


    /* ========================================================
     * PROBARE: nuntius transitorius
     * ======================================================== */

    {
        SaltuariusRes* res;

        imprimere("\n--- Probans nuntium transitorium ---\n");

        res = saltuarius_res_creare(piscina, "/x");
        CREDO_AEQUALIS_S32 (res->nuntius_aetas, ZEPHYRUM);

        saltuarius_res_nuntiare(res, "silva legit...", II);
        CREDO_CHORDA_AEQUALIS_LITERIS (res->nuntius,
            "silva legit...");
        CREDO_AEQUALIS_S32 (res->nuntius_aetas, II);

        /* quadrum I: adhuc visibilis */
        CREDO_VERUM (saltuarius_res_nuntius_senescere(res));
        CREDO_AEQUALIS_S32 (res->nuntius_aetas, I);
        /* quadrum II: evanescit */
        CREDO_FALSUM (saltuarius_res_nuntius_senescere(res));
        CREDO_AEQUALIS_S32 (res->nuntius_aetas, ZEPHYRUM);
        /* stabilis ad ZEPHYRUM */
        CREDO_FALSUM (saltuarius_res_nuntius_senescere(res));
        CREDO_AEQUALIS_S32 (res->nuntius_aetas, ZEPHYRUM);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
