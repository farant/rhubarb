/* probatio_cursus.c - Probationes Cursus Nuntiorum */
#include "latina.h"
#include "cursus.h"
#include "tractator.h"
#include "entitas_repositorium.h"
#include "persistentia.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>


/* ==================================================
 * Contadores pro tractatione
 * ================================================== */

hic_manens i32 g_tractati_tick   = ZEPHYRUM;
hic_manens i32 g_tractati_update = ZEPHYRUM;
hic_manens i32 g_tractati_render = ZEPHYRUM;

interior b32
tractator_tick(ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tractati_tick++;
    redde VERUM;
}

interior b32
tractator_update(ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tractati_update++;
    redde VERUM;
}

interior b32
tractator_render(ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tractati_render++;
    redde VERUM;
}

interior vacuum
resettere_contadores(vacuum)
{
    g_tractati_tick   = ZEPHYRUM;
    g_tractati_update = ZEPHYRUM;
    g_tractati_render = ZEPHYRUM;
}


/* ==================================================
 * Auxiliares
 * ================================================== */

interior EntitasRepositorium*
_creare_repo(Piscina* piscina)
{
    Persistentia* pers;

    pers = persistentia_memoria_creare(piscina);
    si (!pers)
    {
        redde NIHIL;
    }

    redde entitas_repositorium_creare(piscina, pers);
}


s32 principale(vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    b32                  praeteritus;


    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_cursus", MMM * LXIV);
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
     * Probare cursus_creare
     * ================================================== */

    {
        CursusNuntiorum*      cursus;
        RegistrumTractatoris* tractator;
        EntitasRepositorium*  repo;

        imprimere("\n--- Probans cursus_creare ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        CREDO_NON_NIHIL(tractator);
        CREDO_NON_NIHIL(repo);

        cursus = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);
        CREDO_NON_NIHIL(cursus->capsae);
        CREDO_VERUM(cursus->tractator == tractator);
        CREDO_VERUM(cursus->repositorium == repo);
        CREDO_VERUM(cursus->intern == intern);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);

        /* Argumenta invalida */
        cursus = cursus_creare(NIHIL, tractator, repo, intern);
        CREDO_NIHIL(cursus);

        cursus = cursus_creare(piscina, NIHIL, repo, intern);
        CREDO_NIHIL(cursus);

        cursus = cursus_creare(piscina, tractator, NIHIL, intern);
        CREDO_NIHIL(cursus);

        cursus = cursus_creare(piscina, tractator, repo, NIHIL);
        CREDO_NIHIL(cursus);
    }


    /* ==================================================
     * Probare cursus_mittere et capsa pigra
     * ================================================== */

    {
        CursusNuntiorum*      cursus;
        RegistrumTractatoris* tractator;
        EntitasRepositorium*  repo;
        Entitas*              entitas;
        chorda*               entitas_id;

        imprimere("\n--- Probans cursus_mittere ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        /* Creare entitatem */
        entitas = repo->entitas_scaffoldare(repo->datum, "Widget", "widget-1");
        CREDO_NON_NIHIL(entitas);
        entitas_id = entitas->id;

        /* Ante mittere - nulla capsa */
        CREDO_FALSUM(cursus_habet_capsam(cursus, entitas_id));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);

        /* Mittere nuntium - creat capsam pigre */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id, "tick"));

        CREDO_VERUM(cursus_habet_capsam(cursus, entitas_id));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), I);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus, entitas_id), I);

        /* Mittere secundum nuntium */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id, "update"));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), II);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus, entitas_id), II);
    }


    /* ==================================================
     * Probare cursus_processare
     * ================================================== */

    {
        CursusNuntiorum*      cursus;
        RegistrumTractatoris* tractator;
        EntitasRepositorium*  repo;
        Entitas*              entitas;
        chorda*               entitas_id;
        i32                   tractati;

        imprimere("\n--- Probans cursus_processare ---\n");

        resettere_contadores();

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        /* Registrare tractatores */
        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "tick", tractator_tick));
        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "update", tractator_update));

        /* Creare entitatem */
        entitas = repo->entitas_scaffoldare(repo->datum, "Widget", "proc-widget");
        CREDO_NON_NIHIL(entitas);
        entitas_id = entitas->id;

        /* Mittere nuntios */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id, "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id, "update"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id, "tick"));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), III);

        /* Processare omnes */
        tractati = cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(tractati, III);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);

        /* Verificare contadores */
        CREDO_AEQUALIS_I32(g_tractati_tick, II);
        CREDO_AEQUALIS_I32(g_tractati_update, I);
    }


    /* ==================================================
     * Probare limite processandi
     * ================================================== */

    {
        CursusNuntiorum*      cursus;
        RegistrumTractatoris* tractator;
        EntitasRepositorium*  repo;
        Entitas*              entitas;
        chorda*               entitas_id;
        i32                   tractati;
        i32                   i;

        imprimere("\n--- Probans limite processandi ---\n");

        resettere_contadores();

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Counter", "tick", tractator_tick));

        entitas = repo->entitas_scaffoldare(repo->datum, "Counter", "limit-counter");
        CREDO_NON_NIHIL(entitas);
        entitas_id = entitas->id;

        /* Mittere X nuntios */
        per (i = ZEPHYRUM; i < X; i++)
        {
            CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id, "tick"));
        }
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), X);

        /* Processare solum III */
        tractati = cursus_processare(cursus, III);
        CREDO_AEQUALIS_I32(tractati, III);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), VII);
        CREDO_AEQUALIS_I32(g_tractati_tick, III);

        /* Processare residuum */
        tractati = cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(tractati, VII);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);
        CREDO_AEQUALIS_I32(g_tractati_tick, X);
    }


    /* ==================================================
     * Probare multas entitates
     * ================================================== */

    {
        CursusNuntiorum*      cursus;
        RegistrumTractatoris* tractator;
        EntitasRepositorium*  repo;
        Entitas*              ent1;
        Entitas*              ent2;
        chorda*               id1;
        chorda*               id2;
        i32                   tractati;

        imprimere("\n--- Probans multas entitates ---\n");

        resettere_contadores();

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "tick", tractator_tick));
        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "render", tractator_render));

        ent1 = repo->entitas_scaffoldare(repo->datum, "Widget", "multi-1");
        ent2 = repo->entitas_scaffoldare(repo->datum, "Widget", "multi-2");
        CREDO_NON_NIHIL(ent1);
        CREDO_NON_NIHIL(ent2);
        id1 = ent1->id;
        id2 = ent2->id;

        /* Mittere ad ambas entitates */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id1, "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id2, "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id1, "render"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id2, "render"));

        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), IV);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus, id1), II);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus, id2), II);

        /* Processare omnes */
        tractati = cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(tractati, IV);
        CREDO_AEQUALIS_I32(g_tractati_tick, II);
        CREDO_AEQUALIS_I32(g_tractati_render, II);
    }


    /* ==================================================
     * Probare cursus_processare_entitatem
     * ================================================== */

    {
        CursusNuntiorum*      cursus;
        RegistrumTractatoris* tractator;
        EntitasRepositorium*  repo;
        Entitas*              ent1;
        Entitas*              ent2;
        chorda*               id1;
        chorda*               id2;
        i32                   tractati;

        imprimere("\n--- Probans cursus_processare_entitatem ---\n");

        resettere_contadores();

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Item", "tick", tractator_tick));

        ent1 = repo->entitas_scaffoldare(repo->datum, "Item", "item-1");
        ent2 = repo->entitas_scaffoldare(repo->datum, "Item", "item-2");
        CREDO_NON_NIHIL(ent1);
        CREDO_NON_NIHIL(ent2);
        id1 = ent1->id;
        id2 = ent2->id;

        /* Mittere nuntios */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id1, "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id1, "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id2, "tick"));

        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), III);

        /* Processare solum entitatem 1 */
        tractati = cursus_processare_entitatem(cursus, id1, M);
        CREDO_AEQUALIS_I32(tractati, II);
        CREDO_AEQUALIS_I32(g_tractati_tick, II);

        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), I);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus, id1), ZEPHYRUM);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus, id2), I);

        /* Processare entitatem 2 */
        tractati = cursus_processare_entitatem(cursus, id2, M);
        CREDO_AEQUALIS_I32(tractati, I);
        CREDO_AEQUALIS_I32(g_tractati_tick, III);
    }


    /* ==================================================
     * Probare cursus_vacare
     * ================================================== */

    {
        CursusNuntiorum*      cursus;
        RegistrumTractatoris* tractator;
        EntitasRepositorium*  repo;
        Entitas*              entitas;
        chorda*               entitas_id;

        imprimere("\n--- Probans cursus_vacare ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        entitas = repo->entitas_scaffoldare(repo->datum, "Widget", "clear-widget");
        CREDO_NON_NIHIL(entitas);
        entitas_id = entitas->id;

        /* Mittere nuntios */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id, "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id, "tick"));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), II);

        /* Vacare */
        cursus_vacare(cursus);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);
    }


    /* ==================================================
     * Probare cursus_ponere_datum
     * ================================================== */

    {
        CursusNuntiorum*      cursus;
        RegistrumTractatoris* tractator;
        EntitasRepositorium*  repo;
        i32                   datum_custom;

        imprimere("\n--- Probans cursus_ponere_datum ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        datum_custom = XLII;
        cursus_ponere_datum(cursus, &datum_custom);

        CREDO_VERUM(cursus->datum_custom == &datum_custom);
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
