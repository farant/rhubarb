/* probatio_stml_macros.c - expansio fragmentorum parametrizatorum
 *
 * CONTRACTUS DUARUM ARBORUM: expansio arborem NOVAM reddit,
 * originalis intacta manet (plagula formam macroneam servat;
 * scriptor arborem expansam numquam videt). Documentum sine
 * macris: expansio == clonatio - emissio octetim aequalis,
 * tabula expansionum vacua.
 *
 * Spec: project-specs/stml-macros-spec.md; planum:
 * stml-macros-plan.md.
 */
#include "latina.h"
#include "stml.h"
#include "stml_macros.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include <stdio.h>

s32
principale (
    vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_stml_macros",
                                         1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    /* --- documentum sine macris: expansio == clonatio --- */
    {
        StmlResultus res;
              chorda fons;

        imprimere("\n--- documentum sine macris ---\n");
        fons = chorda_ex_literis("<radix><a x=\"1\"/>t</radix>",
                                 piscina);
        res  = stml_legere(fons, piscina, intern);
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            StmlExpansioResultus expansio;

            expansio = stml_expandere(res.radix, piscina, intern);
            CREDO_VERUM (expansio.successus);
            CREDO_NON_NIHIL (expansio.radix_expansa);
            si (expansio.successus && expansio.radix_expansa != NIHIL)
            {
                CREDO_VERUM (expansio.radix_expansa != res.radix);
                CREDO_NON_NIHIL (expansio.tabula_expansionum);
                si (expansio.tabula_expansionum != NIHIL)
                {
                    CREDO_AEQUALIS_I32 (
                        xar_numerus(expansio.tabula_expansionum),
                        ZEPHYRUM);
                }
                CREDO_CHORDA_AEQUALIS (
                    stml_scribere(expansio.radix_expansa, piscina,
                                  FALSUM),
                    stml_scribere(res.radix, piscina, FALSUM));
            }
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    si (credo_omnia_praeterierunt())
    {
        credo_claudere();
        redde ZEPHYRUM;
    }
    credo_claudere();
    redde I;
}
