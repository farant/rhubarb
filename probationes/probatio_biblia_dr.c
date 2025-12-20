/* probatio_biblia_dr.c - Probationes Bibliae Douay-Rheims */

#include "latina.h"
#include "biblia.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


s32 principale(vacuum)
{
    Piscina* piscina;
    b32 praeteritus;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_biblia", 4096);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Probare biblia_obtinere_dr
     * ================================================== */

    {
        constans Biblia* b;

        imprimere("\n--- Probans biblia_obtinere_dr ---\n");

        b = biblia_obtinere_dr();

        CREDO_NON_NIHIL(b);
        CREDO_NON_NIHIL(b->titulus);
        CREDO_NON_NIHIL(b->annus);
        CREDO_AEQUALIS_S32(strcmp(b->titulus, "Douay-Rheims"), ZEPHYRUM);
        CREDO_AEQUALIS_S32(strcmp(b->annus, "1899"), ZEPHYRUM);
        CREDO_AEQUALIS_I32(b->libri_numerus, 73);
        CREDO_VERUM(b->versus_numerus > 30000);  /* circa 35811 versus */
        CREDO_NON_NIHIL(b->textus);
        CREDO_VERUM(b->textus_longitudo > 4000000);  /* circa 4.6MB */
    }


    /* ==================================================
     * Probare biblia_versus - versus celebres
     * ================================================== */

    {
        constans Biblia* b;
        chorda v;

        imprimere("\n--- Probans biblia_versus ---\n");

        b = biblia_obtinere_dr();

        /* Genesis 1:1 */
        v = biblia_versus(b, LIBER_GENESIS, I, I);
        CREDO_VERUM(v.mensura > ZEPHYRUM);
        CREDO_NON_NIHIL(v.datum);
        /* Textus incipit cum "In the beginning" */
        CREDO_VERUM(v.mensura >= XVIII);
        CREDO_VERUM(memcmp(v.datum, "In the beginning", XVI) == ZEPHYRUM);

        /* Joannes 1:1 */
        v = biblia_versus(b, LIBER_JOANNES, I, I);
        CREDO_VERUM(v.mensura > ZEPHYRUM);
        CREDO_NON_NIHIL(v.datum);
        /* Textus incipit cum "In the beginning" */
        CREDO_VERUM(memcmp(v.datum, "In the beginning", XVI) == ZEPHYRUM);

        /* Psalmus 23:1 (Dominus regit me) */
        v = biblia_versus(b, LIBER_PSALMORUM, XXIII, I);
        CREDO_VERUM(v.mensura > ZEPHYRUM);
        CREDO_NON_NIHIL(v.datum);

        /* Apocalypsis 22:21 (ultimus versus) */
        v = biblia_versus(b, LIBER_APOCALYPSIS, XXII, XXI);
        CREDO_VERUM(v.mensura > ZEPHYRUM);
        CREDO_NON_NIHIL(v.datum);
    }


    /* ==================================================
     * Probare biblia_versus - casus invalidi
     * ================================================== */

    {
        constans Biblia* b;
        chorda v;

        imprimere("\n--- Probans biblia_versus invalidi ---\n");

        b = biblia_obtinere_dr();

        /* Liber invalidus */
        v = biblia_versus(b, (i32)(-I), I, I);
        CREDO_AEQUALIS_I32(v.mensura, ZEPHYRUM);

        v = biblia_versus(b, 100, I, I);
        CREDO_AEQUALIS_I32(v.mensura, ZEPHYRUM);

        /* Capitulum invalidum */
        v = biblia_versus(b, LIBER_GENESIS, 100, I);
        CREDO_AEQUALIS_I32(v.mensura, ZEPHYRUM);

        /* Versus invalidus */
        v = biblia_versus(b, LIBER_GENESIS, I, 100);
        CREDO_AEQUALIS_I32(v.mensura, ZEPHYRUM);

        /* Biblia NIHIL */
        v = biblia_versus(NIHIL, LIBER_GENESIS, I, I);
        CREDO_AEQUALIS_I32(v.mensura, ZEPHYRUM);
    }


    /* ==================================================
     * Probare biblia_nomen_libri
     * ================================================== */

    {
        constans Biblia* b;
        constans character* titulus;

        imprimere("\n--- Probans biblia_nomen_libri ---\n");

        b = biblia_obtinere_dr();

        titulus = biblia_nomen_libri(b, LIBER_GENESIS);
        CREDO_NON_NIHIL(titulus);
        CREDO_AEQUALIS_S32(strcmp(titulus, "Genesis"), ZEPHYRUM);

        titulus = biblia_nomen_libri(b, LIBER_JOANNES);
        CREDO_NON_NIHIL(titulus);
        CREDO_AEQUALIS_S32(strcmp(titulus, "John"), ZEPHYRUM);

        titulus = biblia_nomen_libri(b, LIBER_APOCALYPSIS);
        CREDO_NON_NIHIL(titulus);
        CREDO_AEQUALIS_S32(strcmp(titulus, "Apocalypse"), ZEPHYRUM);

        titulus = biblia_nomen_libri(b, LIBER_PSALMORUM);
        CREDO_NON_NIHIL(titulus);
        CREDO_AEQUALIS_S32(strcmp(titulus, "Psalms"), ZEPHYRUM);

        /* Index invalidus */
        titulus = biblia_nomen_libri(b, (i32)(-I));
        CREDO_NIHIL(titulus);

        titulus = biblia_nomen_libri(b, 100);
        CREDO_NIHIL(titulus);

        /* Biblia NIHIL */
        titulus = biblia_nomen_libri(NIHIL, LIBER_GENESIS);
        CREDO_NIHIL(titulus);
    }


    /* ==================================================
     * Probare biblia_capitula_in_libro
     * ================================================== */

    {
        constans Biblia* b;
        i32 capitula;

        imprimere("\n--- Probans biblia_capitula_in_libro ---\n");

        b = biblia_obtinere_dr();

        /* Genesis habet L capitula */
        capitula = biblia_capitula_in_libro(b, LIBER_GENESIS);
        CREDO_AEQUALIS_I32(capitula, L);

        /* Psalmi habent CL capitula */
        capitula = biblia_capitula_in_libro(b, LIBER_PSALMORUM);
        CREDO_AEQUALIS_I32(capitula, CL);

        /* Joannes habet XXI capitula */
        capitula = biblia_capitula_in_libro(b, LIBER_JOANNES);
        CREDO_AEQUALIS_I32(capitula, XXI);

        /* Index invalidus */
        capitula = biblia_capitula_in_libro(b, (i32)(-I));
        CREDO_AEQUALIS_I32(capitula, ZEPHYRUM);

        capitula = biblia_capitula_in_libro(b, 100);
        CREDO_AEQUALIS_I32(capitula, ZEPHYRUM);

        /* Biblia NIHIL */
        capitula = biblia_capitula_in_libro(NIHIL, LIBER_GENESIS);
        CREDO_AEQUALIS_I32(capitula, ZEPHYRUM);
    }


    /* ==================================================
     * Probare biblia_versus_in_capitulo
     * ================================================== */

    {
        constans Biblia* b;
        i32 versus;

        imprimere("\n--- Probans biblia_versus_in_capitulo ---\n");

        b = biblia_obtinere_dr();

        /* Genesis 1 habet XXXI versus */
        versus = biblia_versus_in_capitulo(b, LIBER_GENESIS, I);
        CREDO_AEQUALIS_I32(versus, XXXI);

        /* Joannes 1 habet LI versus */
        versus = biblia_versus_in_capitulo(b, LIBER_JOANNES, I);
        CREDO_AEQUALIS_I32(versus, LI);

        /* Capitulum invalidum */
        versus = biblia_versus_in_capitulo(b, LIBER_GENESIS, 100);
        CREDO_AEQUALIS_I32(versus, ZEPHYRUM);

        /* Liber invalidus */
        versus = biblia_versus_in_capitulo(b, (i32)(-I), I);
        CREDO_AEQUALIS_I32(versus, ZEPHYRUM);

        /* Biblia NIHIL */
        versus = biblia_versus_in_capitulo(NIHIL, LIBER_GENESIS, I);
        CREDO_AEQUALIS_I32(versus, ZEPHYRUM);
    }


    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Destructio */
    credo_claudere();
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
