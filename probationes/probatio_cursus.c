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
tractator_tick (
    ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tractati_tick++;
    redde VERUM;
}

interior b32
tractator_update (
    ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tractati_update++;
    redde VERUM;
}

interior b32
tractator_render (
    ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tractati_render++;
    redde VERUM;
}

interior vacuum
resettere_contadores (
    CredoContextus* c)
{
    (vacuum)c;
    g_tractati_tick    = ZEPHYRUM;
    g_tractati_update  = ZEPHYRUM;
    g_tractati_render  = ZEPHYRUM;
}


/* ==================================================
 * Auxiliares
 * ================================================== */

interior EntitasRepositorium*
_creare_repo (
    Piscina* piscina)
{
    Persistentia* pers;

    pers = persistentia_memoria_creare(piscina);
    si (!pers)
    {
        redde NIHIL;
    }

    redde entitas_repositorium_creare(piscina, pers);
}


    /* ==================================================
     * Probare cursus_creare
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
             CursusNuntiorum* cursus;
        RegistrumTractatoris* tractator;
         EntitasRepositorium* repo;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
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
}


    /* ==================================================
     * Probare cursus_mittere et capsa pigra
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
             CursusNuntiorum* cursus;
        RegistrumTractatoris* tractator;
         EntitasRepositorium* repo;
                     Entitas* entitas;
                      chorda* entitas_id;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        /* Creare entitatem */
        entitas = repo->entitas_scaffoldare(repo->datum, "Widget",
            "widget-1");
        CREDO_NON_NIHIL(entitas);
        entitas_id = entitas->id;

        /* Ante mittere - nulla capsa */
        CREDO_FALSUM(cursus_habet_capsam(cursus, entitas_id));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);

        /* Mittere nuntium - creat capsam pigre */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id,
            "tick"));

        CREDO_VERUM(cursus_habet_capsam(cursus, entitas_id));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), I);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus,
            entitas_id), I);

        /* Mittere secundum nuntium */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id,
            "update"));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), II);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus,
            entitas_id), II);
    }
}


    /* ==================================================
     * Probare cursus_processare
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
             CursusNuntiorum* cursus;
        RegistrumTractatoris* tractator;
         EntitasRepositorium* repo;
                     Entitas* entitas;
                      chorda* entitas_id;
                         i32  tractati;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        /* Registrare tractatores */
        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "tick", tractator_tick));
        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "update", tractator_update));

        /* Creare entitatem */
        entitas = repo->entitas_scaffoldare(repo->datum, "Widget",
            "proc-widget");
        CREDO_NON_NIHIL(entitas);
        entitas_id = entitas->id;

        /* Mittere nuntios */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id,
            "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id,
            "update"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id,
            "tick"));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), III);

        /* Processare omnes */
        tractati = cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(tractati, III);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);

        /* Verificare contadores */
        CREDO_AEQUALIS_I32(g_tractati_tick, II);
        CREDO_AEQUALIS_I32(g_tractati_update, I);
    }
}


    /* ==================================================
     * Probare limite processandi
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
             CursusNuntiorum* cursus;
        RegistrumTractatoris* tractator;
         EntitasRepositorium* repo;
                     Entitas* entitas;
                      chorda* entitas_id;
                         i32  tractati;
                         i32  i;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Counter", "tick", tractator_tick));

        entitas = repo->entitas_scaffoldare(repo->datum, "Counter",
            "limit-counter");
        CREDO_NON_NIHIL(entitas);
        entitas_id = entitas->id;

        /* Mittere X nuntios */
        per (i = ZEPHYRUM; i < X; i++)
        {
            CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL,
                entitas_id, "tick"));
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
}


    /* ==================================================
     * Probare multas entitates
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
             CursusNuntiorum* cursus;
        RegistrumTractatoris* tractator;
         EntitasRepositorium* repo;
                     Entitas* ent1;
                     Entitas* ent2;
                      chorda* id1;
                      chorda* id2;
                         i32  tractati;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "tick", tractator_tick));
        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "render", tractator_render));

        ent1 = repo->entitas_scaffoldare(repo->datum, "Widget",
            "multi-1");
        ent2 = repo->entitas_scaffoldare(repo->datum, "Widget",
            "multi-2");
        CREDO_NON_NIHIL(ent1);
        CREDO_NON_NIHIL(ent2);
        id1 = ent1->id;
        id2 = ent2->id;

        /* Mittere ad ambas entitates */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id1, "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id2, "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id1,
            "render"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, id2,
            "render"));

        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), IV);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus,
            id1), II);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus,
            id2), II);

        /* Processare omnes */
        tractati = cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(tractati, IV);
        CREDO_AEQUALIS_I32(g_tractati_tick, II);
        CREDO_AEQUALIS_I32(g_tractati_render, II);
    }
}


    /* ==================================================
     * Probare cursus_processare_entitatem
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
             CursusNuntiorum* cursus;
        RegistrumTractatoris* tractator;
         EntitasRepositorium* repo;
                     Entitas* ent1;
                     Entitas* ent2;
                      chorda* id1;
                      chorda* id2;
                         i32  tractati;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Item",
            "tick", tractator_tick));

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
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus,
            id1), ZEPHYRUM);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium_entitatis(cursus,
            id2), I);

        /* Processare entitatem 2 */
        tractati = cursus_processare_entitatem(cursus, id2, M);
        CREDO_AEQUALIS_I32(tractati, I);
        CREDO_AEQUALIS_I32(g_tractati_tick, III);
    }
}


    /* ==================================================
     * Probare cursus_vacare
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
             CursusNuntiorum* cursus;
        RegistrumTractatoris* tractator;
         EntitasRepositorium* repo;
                     Entitas* entitas;
                      chorda* entitas_id;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        entitas = repo->entitas_scaffoldare(repo->datum, "Widget",
            "clear-widget");
        CREDO_NON_NIHIL(entitas);
        entitas_id = entitas->id;

        /* Mittere nuntios */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id,
            "tick"));
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, entitas_id,
            "tick"));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), II);

        /* Vacare */
        cursus_vacare(cursus);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);
    }
}


    /* ==================================================
     * Probare cursus_ponere_datum
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
             CursusNuntiorum* cursus;
        RegistrumTractatoris* tractator;
         EntitasRepositorium* repo;
                         i32  datum_custom;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(cursus);

        datum_custom = XLII;
        cursus_ponere_datum(cursus, &datum_custom);

        CREDO_VERUM(cursus->datum_custom == &datum_custom);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "cursus_creare", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cursus_mittere", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cursus_processare", _sectio_tertia, resettere_contadores, NIHIL,
        NIHIL, NIHIL },
    { "limite processandi", _sectio_quarta, resettere_contadores, NIHIL,
        NIHIL, NIHIL },
    { "multas entitates", _sectio_quinta, resettere_contadores, NIHIL,
        NIHIL, NIHIL },
    { "cursus_processare_entitatem", _sectio_sexta,
        resettere_contadores, NIHIL, NIHIL, NIHIL },
    { "cursus_vacare", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cursus_ponere_datum", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cursus", SECTIONES);
}
