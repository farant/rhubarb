/*
 * probatio_machina.c - Probationes identitatis machinae
 *
 * SECTIO I CARDO EST. Officium quod constantem redderet omnes
 * probationes obvias praeteriret: stabilis esset, hex esset,
 * longitudinis rectae esset. SOLA probatio quae fontes DUOS confert
 * eam deprehendit - ideo nucleus purus seorsum expositus est.
 */

#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "machina.h"
#include <stdio.h>
#include <string.h>

interior b32
_omnes_hex (chorda c)
{
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character x = (character)c.datum[i];

        si (!((x >= '0' && x <= '9') || (x >= 'a' && x <= 'f')))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_machina", M * LXIV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ========================================================
     * I. Nucleus purus: fontes DIVERSI, identitates DIVERSAE
     * ======================================================== */

    imprimere("\n--- I. Fontes diversi -> identitates diversae ---\n");
    {
        chorda fons_a, fons_b;
        chorda id_a, id_b, id_a_iterum;

        fons_a = chorda_ex_literis("machina-alpha", piscina);
        fons_b = chorda_ex_literis("machina-beta", piscina);

        id_a        = machina_identitas_ex_fonte(fons_a, piscina);
        id_b        = machina_identitas_ex_fonte(fons_b, piscina);
        id_a_iterum = machina_identitas_ex_fonte(fons_a, piscina);

        /* HAEC probatio constantem deprehendit - nulla alia potest */
        CREDO_FALSUM(chorda_aequalis(id_a, id_b));

        /* Determinatio: idem fons, idem exitus */
        CREDO_VERUM(chorda_aequalis(id_a, id_a_iterum));

        /* OPACITAS: identitas fontem non repetit */
        CREDO_FALSUM(chorda_aequalis(id_a, fons_a));

        /* Forma */
        CREDO_AEQUALIS_I32(id_a.mensura, (i32)MACHINA_IDENTITAS_LONGITUDO);
        CREDO_AEQUALIS_I32(id_b.mensura, (i32)MACHINA_IDENTITAS_LONGITUDO);
        CREDO_VERUM(_omnes_hex(id_a));
        CREDO_VERUM(_omnes_hex(id_b));

        /* Fons unius characteris differens sufficit */
        {
            chorda fons_c = chorda_ex_literis("machina-alphb", piscina);
            chorda id_c   = machina_identitas_ex_fonte(fons_c, piscina);
            CREDO_FALSUM(chorda_aequalis(id_a, id_c));
        }
    }

    /* ========================================================
     * II. Fons vacuus RECUSATUR
     * ======================================================== */

    imprimere("\n--- II. Fons vacuus ---\n");
    {
        chorda vacua, exitus;

        vacua.datum   = NIHIL;
        vacua.mensura = ZEPHYRUM;

        exitus = machina_identitas_ex_fonte(vacua, piscina);
        CREDO_AEQUALIS_I32(exitus.mensura, ZEPHYRUM);

        exitus = machina_identitas_ex_fonte(
                     chorda_ex_literis("quidquid", piscina), NIHIL);
        CREDO_AEQUALIS_I32(exitus.mensura, ZEPHYRUM);
    }

    /* ========================================================
     * III. Identitas huius machinae
     * ======================================================== */

    imprimere("\n--- III. Identitas machinae ---\n");
    {
        chorda prima, altera;

        prima  = machina_identitas(piscina);
        altera = machina_identitas(piscina);

        /* Vacua hic significat fontem deesse - vitium verum in
         * machina quae aut gethostuuid aut /etc/machine-id habet */
        CREDO_AEQUALIS_I32(prima.mensura,
                           (i32)MACHINA_IDENTITAS_LONGITUDO);
        CREDO_VERUM(_omnes_hex(prima));

        /* STABILIS: bis vocata idem reddit */
        CREDO_VERUM(chorda_aequalis(prima, altera));

        imprimere("    identitas: %.*s\n", (integer)prima.mensura,
                  (constans character*)prima.datum);
    }

    /* ========================================================
     * IV. Nota plena
     * ======================================================== */

    imprimere("\n--- IV. Nota plena ---\n");
    {
        MachinaNota nota;

        nota = machina_nota(piscina);

        CREDO_VERUM(nota.valida);
        CREDO_AEQUALIS_I32(nota.identitas.mensura,
                           (i32)MACHINA_IDENTITAS_LONGITUDO);
        CREDO_VERUM(chorda_aequalis(nota.identitas,
                                    machina_identitas(piscina)));

        CREDO_CHORDA_NON_VACUA(nota.systema);
        CREDO_CHORDA_NON_VACUA(nota.architectura);
        CREDO_MAIOR_I32(nota.nuclei, ZEPHYRUM);

        /* Causa VACUA cum valida - aliter nuntius errorem simularet */
        CREDO_AEQUALIS_I32(nota.causa.mensura, ZEPHYRUM);

        imprimere("    systema: %.*s | architectura: %.*s | nuclei: %u\n",
                  (integer)nota.systema.mensura,
                  (constans character*)nota.systema.datum,
                  (integer)nota.architectura.mensura,
                  (constans character*)nota.architectura.datum,
                  nota.nuclei);
    }

    /* ========================================================
     * V. Piscina absens
     * ======================================================== */

    imprimere("\n--- V. Piscina absens ---\n");
    {
        MachinaNota nota;
        chorda      id;

        id = machina_identitas(NIHIL);
        CREDO_AEQUALIS_I32(id.mensura, ZEPHYRUM);

        nota = machina_nota(NIHIL);
        CREDO_FALSUM(nota.valida);
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
