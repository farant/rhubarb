/* probatio_tempestivum.c - Probationes Administratoris Tempestivi */
#include "postulata_posix.h"
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
tractator_tick (
    ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_tick_contador++;
    redde VERUM;
}

interior b32
tractator_update (
    ContextusTractandi* ctx)
{
    (vacuum)ctx;
    g_update_contador++;
    redde VERUM;
}

interior vacuum
resettere_contadores (
    CredoContextus* c)
{
    (vacuum)c;
    g_tick_contador    = ZEPHYRUM;
    g_update_contador  = ZEPHYRUM;
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
     * Probare administrator_tempestivi_creare
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        CREDO_NON_NIHIL(tractator);
        CREDO_NON_NIHIL(repo);
        CREDO_NON_NIHIL(cursus);

        admin = administrator_tempestivi_creare(piscina, repo, cursus,
            intern);
        CREDO_NON_NIHIL(admin);
        CREDO_VERUM(admin->repositorium == repo);
        CREDO_VERUM(admin->cursus == cursus);
        CREDO_VERUM(admin->intern == intern);
        CREDO_NON_NIHIL(admin->genus_tempestivi);

        /* Argumenta invalida */
        admin = administrator_tempestivi_creare(NIHIL, repo, cursus,
            intern);
        CREDO_NIHIL(admin);

        admin = administrator_tempestivi_creare(piscina, NIHIL, cursus,
            intern);
        CREDO_NIHIL(admin);

        admin = administrator_tempestivi_creare(piscina, repo, NIHIL,
            intern);
        CREDO_NIHIL(admin);

        admin = administrator_tempestivi_creare(piscina, repo, cursus,
            NIHIL);
        CREDO_NIHIL(admin);
    }
}


    /* ==================================================
     * Probare tempestivum_creare
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget;
                        Entitas* tempestivum;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        /* Creare entitatem destinationis */
        widget = repo->entitas_scaffoldare(repo->datum, "Widget",
            "timer-widget");
        CREDO_NON_NIHIL(widget);

        /* Creare tempestivum */
        tempestivum = tempestivum_creare(admin, widget->id, "tick", C,
            VERUM);
        CREDO_NON_NIHIL(tempestivum);
        CREDO_NON_NIHIL(tempestivum->id);

        /* Verificare numerus */
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), I);

        /* Verificare est activum */
        CREDO_VERUM(tempestivum_est_activum(admin, tempestivum->id));

        /* Argumenta invalida */
        tempestivum = tempestivum_creare(NIHIL, widget->id, "tick", C,
            VERUM);
        CREDO_NIHIL(tempestivum);

        tempestivum = tempestivum_creare(admin, NIHIL, "tick", C,
            VERUM);
        CREDO_NIHIL(tempestivum);

        tempestivum = tempestivum_creare(admin, widget->id, NIHIL, C,
            VERUM);
        CREDO_NIHIL(tempestivum);

        tempestivum = tempestivum_creare(admin, widget->id, "tick",
            ZEPHYRUM, VERUM);
        CREDO_NIHIL(tempestivum);

        tempestivum = tempestivum_creare(admin, widget->id, "tick", -C,
            VERUM);
        CREDO_NIHIL(tempestivum);
    }
}


    /* ==================================================
     * Probare tempestivum_creare_cum_delay
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget;
                        Entitas* tempestivum;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        widget = repo->entitas_scaffoldare(repo->datum, "Widget",
            "delay-widget");
        CREDO_NON_NIHIL(widget);

        /* Creare tempestivum cum delay */
        tempestivum = tempestivum_creare_cum_delay(
            admin, widget->id, "tick", C, L, VERUM);
        CREDO_NON_NIHIL(tempestivum);
        CREDO_VERUM(tempestivum_est_activum(admin, tempestivum->id));
    }
}


    /* ==================================================
     * Probare tempestivum_pausare et resumere
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget;
                        Entitas* tempestivum;
                         chorda* temp_id;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        widget = repo->entitas_scaffoldare(repo->datum, "Widget",
            "pause-widget");
        CREDO_NON_NIHIL(widget);

        tempestivum = tempestivum_creare(admin, widget->id, "tick", C,
            VERUM);
        CREDO_NON_NIHIL(tempestivum);
        temp_id = tempestivum->id;

        /* Ante pausare */
        CREDO_VERUM(tempestivum_est_activum(admin, temp_id));
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin), I);

        /* Pausare */
        CREDO_VERUM(tempestivum_pausare(admin, temp_id));
        CREDO_FALSUM(tempestivum_est_activum(admin, temp_id));
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin),
            ZEPHYRUM);

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
}


    /* ==================================================
     * Probare tempestivum_annullare
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget;
                        Entitas* tempestivum;
                         chorda* temp_id;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        widget = repo->entitas_scaffoldare(repo->datum, "Widget",
            "cancel-widget");
        CREDO_NON_NIHIL(widget);

        tempestivum = tempestivum_creare(admin, widget->id, "tick", C,
            VERUM);
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
}


    /* ==================================================
     * Probare tempestivum_pro_destinatione
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget1;
                        Entitas* widget2;
                            Xar* tempestiva;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        widget1 = repo->entitas_scaffoldare(repo->datum, "Widget",
            "dest-widget-1");
        widget2 = repo->entitas_scaffoldare(repo->datum, "Widget",
            "dest-widget-2");
        CREDO_NON_NIHIL(widget1);
        CREDO_NON_NIHIL(widget2);

        /* Creare II tempestiva pro widget1, I pro widget2 */
        CREDO_NON_NIHIL(tempestivum_creare(admin, widget1->id, "tick",
            C, VERUM));
        CREDO_NON_NIHIL(tempestivum_creare(admin, widget1->id, "update",
            CC, VERUM));
        CREDO_NON_NIHIL(tempestivum_creare(admin, widget2->id, "tick",
            CL, VERUM));

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
}


    /* ==================================================
     * Probare tempestivum_processare (cum brevi intervallo)
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget;
                            i32  missi;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        /* Registrare tractator */
        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "tick", tractator_tick));

        widget = repo->entitas_scaffoldare(repo->datum, "Widget",
            "proc-widget");
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
}


    /* ==================================================
     * Probare tempestivum repetens
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget;
                        Entitas* tempestivum;
                            i32  missi;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "tick", tractator_tick));

        widget = repo->entitas_scaffoldare(repo->datum, "Widget",
            "repeat-widget");
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
}


    /* ==================================================
     * Probare tempestivum non repetens
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget;
                        Entitas* tempestivum;
                            i32  missi;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "update", tractator_update));

        widget = repo->entitas_scaffoldare(repo->datum, "Widget",
            "oneshot-widget");
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
        CREDO_AEQUALIS_I32(tempestivum_numerus_activorum(admin),
            ZEPHYRUM);

        /* Processare denuo - nihil */
        usleep(15000);
        missi = tempestivum_processare(admin);
        CREDO_AEQUALIS_I32(missi, ZEPHYRUM);

        /* Processare nuntios */
        cursus_processare(cursus, M);
        CREDO_AEQUALIS_I32(g_update_contador, I);
    }
}


    /* ==================================================
     * Probare multa tempestiva
     * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget1;
                        Entitas* widget2;
                            i32  missi;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "tick", tractator_tick));
        CREDO_VERUM(registrum_tractatoris_registrare(tractator,
            "Widget", "update", tractator_update));

        widget1 = repo->entitas_scaffoldare(repo->datum, "Widget",
            "multi-widget-1");
        widget2 = repo->entitas_scaffoldare(repo->datum, "Widget",
            "multi-widget-2");
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
}


    /* ==================================================
     * Probare tempestivum_proxima_meta_ms (pabulum bracchii
     * temporis pumpae obstructivae - vitrea Phasis B')
     * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_globale();
    {
        AdministratorTempestivi* admin;
           RegistrumTractatoris* tractator;
            EntitasRepositorium* repo;
                CursusNuntiorum* cursus;
                        Entitas* widget;
                        Entitas* lentum;
                        Entitas* celere;
                            s64  meta;

        tractator  = registrum_tractatoris_creare(piscina, intern);
        repo       = _creare_repo(piscina);
        cursus     = cursus_creare(piscina, tractator, repo, intern);
        admin = administrator_tempestivi_creare(piscina, repo,
            cursus, intern);
        CREDO_NON_NIHIL(admin);

        /* nullum tempestivum -> -1 (nihil paratum) */
        CREDO_VERUM(tempestivum_proxima_meta_ms(admin) == (s64)(-I));
        CREDO_VERUM(tempestivum_proxima_meta_ms(NIHIL) == (s64)(-I));

        widget = repo->entitas_scaffoldare(repo->datum, "Widget",
            "meta-widget");
        CREDO_NON_NIHIL(widget);

        /* tempestivum lentum (5000 ms) -> meta in (0, 5000] */
        lentum = tempestivum_creare(admin, widget->id, "tick", 5000,
            VERUM);
        CREDO_NON_NIHIL(lentum);
        meta = tempestivum_proxima_meta_ms(admin);
        CREDO_VERUM(meta > (s64)ZEPHYRUM && meta <= (s64)5000);

        /* alterum celerius (50 ms) -> minima vincit */
        celere = tempestivum_creare(admin, widget->id, "update", L,
            VERUM);
        CREDO_NON_NIHIL(celere);
        meta = tempestivum_proxima_meta_ms(admin);
        CREDO_VERUM(meta >= (s64)ZEPHYRUM && meta <= (s64)L);

        /* maturum praeteritum -> 0 (numquam negativum) */
        usleep(60000);   /* 60 ms - celere iam maturum */
        meta = tempestivum_proxima_meta_ms(admin);
        CREDO_VERUM(meta == (s64)ZEPHYRUM);

        /* celere pausatum -> lentum solum numerat */
        CREDO_VERUM(tempestivum_pausare(admin, celere->id));
        meta = tempestivum_proxima_meta_ms(admin);
        CREDO_VERUM(meta > (s64)L);

        /* omnia pausata/annullata -> -1 iterum */
        CREDO_VERUM(tempestivum_annullare(admin, lentum->id));
        meta = tempestivum_proxima_meta_ms(admin);
        CREDO_VERUM(meta == (s64)(-I));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "administrator_tempestivi_creare", _sectio_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "tempestivum_creare", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tempestivum_creare_cum_delay", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "tempestivum_pausare et resumere", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "tempestivum_annullare", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tempestivum_pro_destinatione", _sectio_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "tempestivum_processare", _sectio_septima, resettere_contadores,
        NIHIL, NIHIL,
        NIHIL },
    { "tempestivum repetens", _sectio_octava, resettere_contadores,
        NIHIL, NIHIL,
        NIHIL },
    { "tempestivum non repetens", _sectio_nona, resettere_contadores,
        NIHIL, NIHIL,
        NIHIL },
    { "multa tempestiva", _sectio_decima, resettere_contadores, NIHIL,
        NIHIL, NIHIL },
    { "tempestivum_proxima_meta_ms", _sectio_decima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    /* IMPORTANT: Usare internamentum_globale quia entitas_repositorium
     * usare internamentum_globale pro genus internatis */
    si (!internamentum_globale())
    {
        imprimere("FRACTA: internamentum_globale\n");
        redde I;
    }

    redde credo_suitam_currere("tempestivum", SECTIONES);
}
