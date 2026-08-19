/* formator.c - lint formae domus (G1)
 *
 * Usus:
 *   formator <via.c> [viae ...] [-machina]
 *
 * Regulae: project-specs/formator-regulae.md. Machina in
 * instrumenta/silva_formator.{h,c} (probabilis, silici
 * portabilis) - hic modi et CLI soli, exemplar differre.
 *
 * Exitus: 0 conformis | 1 divergentiae | 2 usus aut plagula
 * illegibilis
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "silva_formator.h"

#include <stdio.h>
#include <string.h>

integer
principale (
     integer  numerus,
    character** argumenta)
{
    Piscina* piscina;
         b32  machina;
         b32  ulla_plagula;
         i32  summa;
     integer  i;

    machina      = FALSUM;
    ulla_plagula = FALSUM;
    summa        = ZEPHYRUM;

    per (i = I; i < numerus; i += I)
    {
        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
    }

    piscina = piscina_generare_dynamicum("formator", 67108864);
    si (!piscina)
    {
        fprintf(stderr, "formator: piscina fracta\n");
        redde II;
    }

    si (machina)
    {
        imprimere("# via\tlinea\tcolumna\tregula\tinventum\t"
            "exspectatum\tnuntius\n");
    }

    per (i = I; i < numerus; i += I)
    {
        constans character* via;
                    chorda  textus;
                       Xar* divergentiae;
                       i32  n;
                       i32  j;

        via = argumenta[i];
        si (via[ZEPHYRUM] == '-') perge;

        si (!filum_existit(via))
        {
            fprintf(stderr,
                "formator: plagula illegibilis: %s\n", via);
            piscina_destruere(piscina);
            redde II;
        }
        textus = filum_legere_totum(via, piscina);
        divergentiae = formator_lint(piscina,
            (constans character*)textus.datum, textus.mensura);
        ulla_plagula = VERUM;

        n = divergentiae ? xar_numerus(divergentiae)
            : (i32)ZEPHYRUM;
        summa += n;

        per (j = ZEPHYRUM; j < n; j += I)
        {
            FormatorDivergentia* d;

            d = (FormatorDivergentia*)xar_obtinere(
                divergentiae, j);
            si (machina)
            {
                imprimere("%s\t%u\t%u\t%s\t%d\t%d\t%s\n", via,
                    (insignatus integer)d->linea,
                    (insignatus integer)d->columna, d->regula,
                    (integer)d->inventum,
                    (integer)d->exspectatum, d->nuntius);
            }
            alioquin
            {
                imprimere("%s:%u:%u\t%s\t%s (%d pro %d)\n",
                    via, (insignatus integer)d->linea,
                    (insignatus integer)d->columna, d->regula,
                    d->nuntius, (integer)d->inventum,
                    (integer)d->exspectatum);
            }
        }
    }

    si (!ulla_plagula)
    {
        fprintf(stderr,
            "usus: formator <via.c> [viae ...] [-machina]\n");
        piscina_destruere(piscina);
        redde II;
    }

    si (!machina)
    {
        si (summa == (i32)ZEPHYRUM)
        {
            imprimere("CONFORMIS\n");
        }
        alioquin
        {
            imprimere("DIVERGENTIAE\t%u\n",
                (insignatus integer)summa);
        }
    }

    piscina_destruere(piscina);
    redde summa != (i32)ZEPHYRUM ? I : ZEPHYRUM;
}
