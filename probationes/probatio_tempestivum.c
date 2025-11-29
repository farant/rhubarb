/* probatio_tempestivum.c - Probationes Administratoris Tempestivi */
#include "latina.h"
#include "tempestivum.h"
#include "cursus.h"
#include "tractator.h"
#include "entitas_repositorium.h"
#include "persistentia.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>
#include <unistd.h>  /* Pro usleep */


/* ==================================================
 * Contadores pro tractatione
 * ================================================== */

hic_manens i32 g_tick_contador   = ZEPHYRUM;
hic_manens i32 g_update_contador = ZEPHYRUM;

interior b32
tractator_tick(ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tick_contador++;
    redde VERUM;
}

interior b32
tractator_update(ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_update_contador++;
    redde VERUM;
}

interior vacuum
resettere_contadores(vacuum)
{
    g_tick_contador   = ZEPHYRUM;
    g_update_contador = ZEPHYRUM;
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
    piscina = piscina_generare_dynamicum("test_tempestivum", MMM * LXIV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }

    /* IMPORTANT: Usare internamentum_globale quia entitas_repositorium
     * usare internamentum_globale pro genus internatis */
    intern = internamentum_globale();
    si (!intern)
    {
        imprimere("FRACTA: internamentum_globale\n");
        piscina_destruere(piscina);
        redde I;
    }

    credo_aperire(piscina);


    /* ==================================================
     * Probare administrator_tempestivi_creare
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;

        imprimere("\n--- Probans administrator_tempestivi_creare ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(tractator);
        CREDO_NON_NIHIL(repo);
        CREDO_NON_NIHIL(cursus);

        admin = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);
        CREDO_VERUM(admin->repositorium == repo);
        CREDO_VERUM(admin->cursus == cursus);
        CREDO_VERUM(admin->intern == intern);
        CREDO_NON_NIHIL(admin->genus_tempestivi);

        /* Argumenta invalida */
        admin = administrator_tempestivi_creare(NIHIL, repo, cursus, intern);
        CREDO_NIHIL(admin);

        admin = administrator_tempestivi_creare(piscina, NIHIL, cursus, intern);
        CREDO_NIHIL(admin);

        admin = administrator_tempestivi_creare(piscina, repo, NIHIL, intern);
        CREDO_NIHIL(admin);

        admin = administrator_tempestivi_creare(piscina, repo, cursus, NIHIL);
        CREDO_NIHIL(admin);
    }


    /* ==================================================
     * Probare tempestivum_creare
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget;
        Entitas*                 tempestivum;

        imprimere("\n--- Probans tempestivum_creare ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        /* Creare entitatem destinationis */
        widget = repo->entitas_scaffoldare(repo->datum, "Widget", "timer-widget");
        CREDO_NON_NIHIL(widget);

        /* Creare tempestivum */
        tempestivum = tempestivum_creare(admin, widget->id, "tick", C, VERUM);
        CREDO_NON_NIHIL(tempestivum);
        CREDO_NON_NIHIL(tempestivum->id);

        /* Verificare numerus */
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), I);

        /* Verificare est activum */
        CREDO_VERUM(tempestivum_est_activum(admin, tempestivum->id));

        /* Argumenta invalida */
        tempestivum = tempestivum_creare(NIHIL, widget->id, "tick", C, VERUM);
        CREDO_NIHIL(tempestivum);

        tempestivum = tempestivum_creare(admin, NIHIL, "tick", C, VERUM);
        CREDO_NIHIL(tempestivum);

        tempestivum = tempestivum_creare(admin, widget->id, NIHIL, C, VERUM);
        CREDO_NIHIL(tempestivum);

        tempestivum = tempestivum_creare(admin, widget->id, "tick", ZEPHYRUM, VERUM);
        CREDO_NIHIL(tempestivum);

        tempestivum = tempestivum_creare(admin, widget->id, "tick", -C, VERUM);
        CREDO_NIHIL(tempestivum);
    }


    /* ==================================================
     * Probare tempestivum_creare_cum_delay
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget;
        Entitas*                 tempestivum;

        imprimere("\n--- Probans tempestivum_creare_cum_delay ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        widget = repo->entitas_scaffoldare(repo->datum, "Widget", "delay-widget");
        CREDO_NON_NIHIL(widget);

        /* Creare tempestivum cum delay */
        tempestivum = tempestivum_creare_cum_delay(
            admin, widget->id, "tick", C, L, VERUM);
        CREDO_NON_NIHIL(tempestivum);
        CREDO_VERUM(tempestivum_est_activum(admin, tempestivum->id));
    }


    /* ==================================================
     * Probare tempestivum_pausare et resumere
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget;
        Entitas*                 tempestivum;
        chorda*                  temp_id;

        imprimere("\n--- Probans tempestivum_pausare et resumere ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        widget = repo->entitas_scaffoldare(repo->datum, "Widget", "pause-widget");
        CREDO_NON_NIHIL(widget);

        tempestivum = tempestivum_creare(admin, widget->id, "tick", C, VERUM);
        CREDO_NON_NIHIL(tempestivum);
        temp_id = tempestivum->id;

        /* Ante pausare */
        CREDO_VERUM(tempestivum_est_activum(admin, temp_id));
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), I);

        /* Pausare */
        CREDO_VERUM(tempestivum_pausare(admin, temp_id));
        CREDO_FALSUM(tempestivum_est_activum(admin, temp_id));
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), ZEPHYRUM);

        /* Resumere */
        CREDO_VERUM(tempestivum_resumere(admin, temp_id));
        CREDO_VERUM(tempestivum_est_activum(admin, temp_id));
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), I);

        /* Argumenta invalida */
        CREDO_FALSUM(tempestivum_pausare(NIHIL, temp_id));
        CREDO_FALSUM(tempestivum_pausare(admin, NIHIL));
        CREDO_FALSUM(tempestivum_resumere(NIHIL, temp_id));
        CREDO_FALSUM(tempestivum_resumere(admin, NIHIL));
    }


    /* ==================================================
     * Probare tempestivum_annullare
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget;
        Entitas*                 tempestivum;
        chorda*                  temp_id;

        imprimere("\n--- Probans tempestivum_annullare ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        widget = repo->entitas_scaffoldare(repo->datum, "Widget", "cancel-widget");
        CREDO_NON_NIHIL(widget);

        tempestivum = tempestivum_creare(admin, widget->id, "tick", C, VERUM);
        CREDO_NON_NIHIL(tempestivum);
        temp_id = tempestivum->id;

        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), I);

        /* Annullare */
        CREDO_VERUM(tempestivum_annullare(admin, temp_id));

        /* Post annullationem */
        CREDO_FALSUM(tempestivum_est_activum(admin, temp_id));

        /* Argumenta invalida */
        CREDO_FALSUM(tempestivum_annullare(NIHIL, temp_id));
        CREDO_FALSUM(tempestivum_annullare(admin, NIHIL));
    }


    /* ==================================================
     * Probare tempestivum_pro_destinatione
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget1;
        Entitas*                 widget2;
        Xar*                     tempestiva;

        imprimere("\n--- Probans tempestivum_pro_destinatione ---\n");

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        widget1 = repo->entitas_scaffoldare(repo->datum, "Widget", "dest-widget-1");
        widget2 = repo->entitas_scaffoldare(repo->datum, "Widget", "dest-widget-2");
        CREDO_NON_NIHIL(widget1);
        CREDO_NON_NIHIL(widget2);

        /* Creare II tempestiva pro widget1, I pro widget2 */
        CREDO_NON_NIHIL(tempestivum_creare(admin, widget1->id, "tick", C, VERUM));
        CREDO_NON_NIHIL(tempestivum_creare(admin, widget1->id, "update", CC, VERUM));
        CREDO_NON_NIHIL(tempestivum_creare(admin, widget2->id, "tick", CL, VERUM));

        /* Quaerere pro widget1 */
        tempestiva = tempestivum_pro_destinatione(admin, widget1->id);
        CREDO_NON_NIHIL(tempestiva);
        CREDO_AEQUALIS_I32(xar_numerus(tempestiva), II);

        /* Quaerere pro widget2 */
        tempestiva = tempestivum_pro_destinatione(admin, widget2->id);
        CREDO_NON_NIHIL(tempestiva);
        CREDO_AEQUALIS_I32(xar_numerus(tempestiva), I);

        /* Argumenta invalida */
        tempestiva = tempestivum_pro_destinatione(NIHIL, widget1->id);
        CREDO_NIHIL(tempestiva);

        tempestiva = tempestivum_pro_destinatione(admin, NIHIL);
        CREDO_NIHIL(tempestiva);
    }


    /* ==================================================
     * Probare tempestivum_processare (cum brevi intervallo)
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget;
        i32                      missi;

        imprimere("\n--- Probans tempestivum_processare ---\n");

        resettere_contadores();

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        /* Registrare tractator */
        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "tick", tractator_tick));

        widget = repo->entitas_scaffoldare(repo->datum, "Widget", "proc-widget");
        CREDO_NON_NIHIL(widget);

        /* Creare tempestivum cum delay initiali brevis (10ms) */
        CREDO_NON_NIHIL(tempestivum_creare_cum_delay(
            admin, widget->id, "tick", C, X, VERUM));

        /* Immediatim processare - debet non mittere (nondum tempus) */
        missi = tempestivum_processare(admin);
        CREDO_AEQUALIS_I32(missi, ZEPHYRUM);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), ZEPHYRUM);

        /* Exspectare 15ms */
        usleep(15000);

        /* Nunc processare - debet mittere */
        missi = tempestivum_processare(admin);
        CREDO_AEQUALIS_I32(missi, I);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), I);

        /* Processare nuntios */
        cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(g_tick_contador, I);
    }


    /* ==================================================
     * Probare tempestivum repetens
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget;
        Entitas*                 tempestivum;
        i32                      missi;

        imprimere("\n--- Probans tempestivum repetens ---\n");

        resettere_contadores();

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "tick", tractator_tick));

        widget = repo->entitas_scaffoldare(repo->datum, "Widget", "repeat-widget");
        CREDO_NON_NIHIL(widget);

        /* Creare tempestivum repetens cum delay 10ms et intervallo 10ms */
        tempestivum = tempestivum_creare_cum_delay(
            admin, widget->id, "tick", X, X, VERUM);
        CREDO_NON_NIHIL(tempestivum);

        /* Exspectare 15ms */
        usleep(15000);

        /* Prima activatio */
        missi = tempestivum_processare(admin);
        CREDO_AEQUALIS_I32(missi, I);

        /* Timer debet esse adhuc activum */
        CREDO_VERUM(tempestivum_est_activum(admin, tempestivum->id));

        /* Exspectare 15ms */
        usleep(15000);

        /* Secunda activatio */
        missi = tempestivum_processare(admin);
        CREDO_AEQUALIS_I32(missi, I);

        /* Processare nuntios */
        cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(g_tick_contador, II);
    }


    /* ==================================================
     * Probare tempestivum non repetens
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget;
        Entitas*                 tempestivum;
        i32                      missi;

        imprimere("\n--- Probans tempestivum non repetens ---\n");

        resettere_contadores();

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "update", tractator_update));

        widget = repo->entitas_scaffoldare(repo->datum, "Widget", "oneshot-widget");
        CREDO_NON_NIHIL(widget);

        /* Creare tempestivum non repetens cum delay 10ms */
        tempestivum = tempestivum_creare_cum_delay(
            admin, widget->id, "update", C, X, FALSUM);  /* Non repetere */
        CREDO_NON_NIHIL(tempestivum);

        /* Exspectare 15ms */
        usleep(15000);

        /* Prima activatio */
        missi = tempestivum_processare(admin);
        CREDO_AEQUALIS_I32(missi, I);

        /* Timer debet esse non activum post activationem */
        CREDO_FALSUM(tempestivum_est_activum(admin, tempestivum->id));
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), ZEPHYRUM);

        /* Processare denuo - nihil */
        usleep(15000);
        missi = tempestivum_processare(admin);
        CREDO_AEQUALIS_I32(missi, ZEPHYRUM);

        /* Processare nuntios */
        cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(g_update_contador, I);
    }


    /* ==================================================
     * Probare multa tempestiva
     * ================================================== */

    {
        AdministratorTempestivi* admin;
        RegistrumTractatoris*    tractator;
        EntitasRepositorium*     repo;
        CursusNuntiorum*         cursus;
        Entitas*                 widget1;
        Entitas*                 widget2;
        i32                      missi;

        imprimere("\n--- Probans multa tempestiva ---\n");

        resettere_contadores();

        tractator = registrum_tractatoris_creare(piscina, intern);
        repo      = _creare_repo(piscina);
        cursus    = cursus_creare(piscina, tractator, repo, intern);
        admin     = administrator_tempestivi_creare(piscina, repo, cursus, intern);
        CREDO_NON_NIHIL(admin);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "tick", tractator_tick));
        CREDO_VERUM(registrum_tractatoris_registrare(tractator, "Widget", "update", tractator_update));

        widget1 = repo->entitas_scaffoldare(repo->datum, "Widget", "multi-widget-1");
        widget2 = repo->entitas_scaffoldare(repo->datum, "Widget", "multi-widget-2");
        CREDO_NON_NIHIL(widget1);
        CREDO_NON_NIHIL(widget2);

        /* Creare III tempestiva cum delay 10ms */
        CREDO_NON_NIHIL(tempestivum_creare_cum_delay(
            admin, widget1->id, "tick", C, X, VERUM));
        CREDO_NON_NIHIL(tempestivum_creare_cum_delay(
            admin, widget1->id, "update", C, X, VERUM));
        CREDO_NON_NIHIL(tempestivum_creare_cum_delay(
            admin, widget2->id, "tick", C, X, VERUM));

        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), III);

        /* Exspectare 15ms */
        usleep(15000);

        /* Processare - omnes III debent activari */
        missi = tempestivum_processare(admin);
        CREDO_AEQUALIS_I32(missi, III);
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), III);

        /* Processare nuntios */
        cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(g_tick_contador, II);
        CREDO_AEQUALIS_I32(g_update_contador, I);
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
