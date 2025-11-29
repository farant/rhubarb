/* probatio_tractator.c - Probationes Registri Tractatoris */
#include "latina.h"
#include "tractator.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>


/* ==================================================
 * Tractatores pro probatione
 * ================================================== */

hic_manens i32 g_tractator_a_vocatus = ZEPHYRUM;
hic_manens i32 g_tractator_b_vocatus = ZEPHYRUM;
hic_manens i32 g_fallback_vocatus    = ZEPHYRUM;

interior b32
tractator_a(ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tractator_a_vocatus++;
    redde VERUM;
}

interior b32
tractator_b(ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tractator_b_vocatus++;
    redde VERUM;
}

interior b32
tractator_fallback(ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_fallback_vocatus++;
    redde VERUM;
}

s32 principale(vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    b32                  praeteritus;


    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_tractator", MMM * XVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("FRACTA: internamentum_creare\n");
        piscina_destruere(piscina);
        redde I;
    }

    credo_aperire(piscina);


    /* ==================================================
     * Probare registrum_tractatoris_creare
     * ================================================== */

    {
        RegistrumTractatoris* reg;

        imprimere("\n--- Probans registrum_tractatoris_creare ---\n");

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);
        CREDO_NON_NIHIL(reg->genera);
        CREDO_NON_NIHIL(reg->fallbacks);
        CREDO_VERUM(reg->piscina == piscina);
        CREDO_VERUM(reg->intern == intern);
        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), ZEPHYRUM);

        /* Argumenta invalida */
        reg = registrum_tractatoris_creare(NIHIL, intern);
        CREDO_NIHIL(reg);

        reg = registrum_tractatoris_creare(piscina, NIHIL);
        CREDO_NIHIL(reg);
    }


    /* ==================================================
     * Probare registrum_tractatoris_registrare
     * ================================================== */

    {
        RegistrumTractatoris* reg;

        imprimere("\n--- Probans registrum_tractatoris_registrare ---\n");

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Registrare tractator */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "open", tractator_a));
        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), I);

        /* Registrare secundum tractator pro eodem genere entitatis */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "close", tractator_b));
        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), II);

        /* Registrare tractator pro alio genere entitatis */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Widget", "tick", tractator_a));
        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), III);

        /* Argumenta invalida */
        CREDO_FALSUM(registrum_tractatoris_registrare(NIHIL, "Page", "test", tractator_a));
        CREDO_FALSUM(registrum_tractatoris_registrare(reg, NIHIL, "test", tractator_a));
        CREDO_FALSUM(registrum_tractatoris_registrare(reg, "Page", NIHIL, tractator_a));
        CREDO_FALSUM(registrum_tractatoris_registrare(reg, "Page", "test", NIHIL));
    }


    /* ==================================================
     * Probare registrum_tractatoris_habet
     * ================================================== */

    {
        RegistrumTractatoris* reg;

        imprimere("\n--- Probans registrum_tractatoris_habet ---\n");

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Sine registratione */
        CREDO_FALSUM(registrum_tractatoris_habet(reg, "Page", "open"));

        /* Post registrationem */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "open", tractator_a));
        CREDO_VERUM(registrum_tractatoris_habet(reg, "Page", "open"));

        /* Genus entitatis non existens */
        CREDO_FALSUM(registrum_tractatoris_habet(reg, "Widget", "open"));

        /* Genus nuntii non existens */
        CREDO_FALSUM(registrum_tractatoris_habet(reg, "Page", "close"));
    }


    /* ==================================================
     * Probare registrum_tractatoris_invenire_literis
     * ================================================== */

    {
        RegistrumTractatoris* reg;
        FunctioTractandi      functio;

        imprimere("\n--- Probans registrum_tractatoris_invenire_literis ---\n");

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "open", tractator_a));
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "close", tractator_b));

        /* Invenire tractator_a */
        functio = registrum_tractatoris_invenire_literis(reg, "Page", "open");
        CREDO_VERUM(functio == tractator_a);

        /* Invenire tractator_b */
        functio = registrum_tractatoris_invenire_literis(reg, "Page", "close");
        CREDO_VERUM(functio == tractator_b);

        /* Non inventum */
        functio = registrum_tractatoris_invenire_literis(reg, "Page", "unknown");
        CREDO_NIHIL((vacuum*)functio);

        functio = registrum_tractatoris_invenire_literis(reg, "Widget", "open");
        CREDO_NIHIL((vacuum*)functio);
    }


    /* ==================================================
     * Probare registrum_tractatoris_invenire (cum chorda*)
     * ================================================== */

    {
        RegistrumTractatoris* reg;
        FunctioTractandi      functio;
        chorda                genus_ent_chorda;
        chorda                genus_nun_chorda;
        chorda*               genus_ent;
        chorda*               genus_nun;

        imprimere("\n--- Probans registrum_tractatoris_invenire ---\n");

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "render", tractator_a));

        /* Internare chordas */
        genus_ent_chorda = chorda_ex_literis("Page", piscina);
        genus_ent = chorda_internare(intern, genus_ent_chorda);
        CREDO_NON_NIHIL(genus_ent);

        genus_nun_chorda = chorda_ex_literis("render", piscina);
        genus_nun = chorda_internare(intern, genus_nun_chorda);
        CREDO_NON_NIHIL(genus_nun);

        functio = registrum_tractatoris_invenire(reg, genus_ent, genus_nun);
        CREDO_VERUM(functio == tractator_a);
    }


    /* ==================================================
     * Probare fallback tractator
     * ================================================== */

    {
        RegistrumTractatoris* reg;
        FunctioTractandi      functio;

        imprimere("\n--- Probans fallback tractator ---\n");

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Registrare tractator specificum */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "open", tractator_a));

        /* Ponere fallback */
        CREDO_VERUM(registrum_tractatoris_ponere_fallback(reg, "Page", tractator_fallback));

        /* Quaerere tractator specificum - debet invenire tractator_a */
        functio = registrum_tractatoris_invenire_literis(reg, "Page", "open");
        CREDO_VERUM(functio == tractator_a);

        /* Quaerere nuntium ignotum - debet invenire fallback */
        functio = registrum_tractatoris_invenire_literis(reg, "Page", "unknown_message");
        CREDO_VERUM(functio == tractator_fallback);

        /* Quaerere genus entitatis sine fallback - debet reddere NIHIL */
        functio = registrum_tractatoris_invenire_literis(reg, "Widget", "test");
        CREDO_NIHIL((vacuum*)functio);
    }


    /* ==================================================
     * Probare multos tractatores
     * ================================================== */

    {
        RegistrumTractatoris* reg;
        i32                   i;
        character             genus_buffer[XXXII];
        character             nuntius_buffer[XXXII];

        imprimere("\n--- Probans multos tractatores ---\n");

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Registrare multos tractatores */
        per (i = ZEPHYRUM; i < L; i++)
        {
            sprintf(genus_buffer, "Genus_%d", (integer)i);
            sprintf(nuntius_buffer, "nuntius_%d", (integer)i);

            CREDO_VERUM(registrum_tractatoris_registrare(
                reg, genus_buffer, nuntius_buffer, tractator_a));
        }

        CREDO_AEQUALIS_I32(registrum_tractatoris_numerus(reg), L);

        /* Verificare omnes */
        per (i = ZEPHYRUM; i < L; i++)
        {
            sprintf(genus_buffer, "Genus_%d", (integer)i);
            sprintf(nuntius_buffer, "nuntius_%d", (integer)i);

            CREDO_VERUM(registrum_tractatoris_habet(reg, genus_buffer, nuntius_buffer));
        }
    }


    /* ==================================================
     * Probare overwrite tractator
     * ================================================== */

    {
        RegistrumTractatoris* reg;
        FunctioTractandi      functio;

        imprimere("\n--- Probans overwrite tractator ---\n");

        reg = registrum_tractatoris_creare(piscina, intern);
        CREDO_NON_NIHIL(reg);

        /* Registrare tractator_a */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "update", tractator_a));

        functio = registrum_tractatoris_invenire_literis(reg, "Page", "update");
        CREDO_VERUM(functio == tractator_a);

        /* Overwrite cum tractator_b */
        CREDO_VERUM(registrum_tractatoris_registrare(reg, "Page", "update", tractator_b));

        functio = registrum_tractatoris_invenire_literis(reg, "Page", "update");
        CREDO_VERUM(functio == tractator_b);
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
