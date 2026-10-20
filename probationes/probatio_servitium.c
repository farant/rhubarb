/* probatio_servitium.c - Probationes Registri Servitii */
#include "latina.h"
#include "servitium.h"
#include "entitas_repositorium.h"
#include "persistentia.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>


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
     * Probare registrum_servitii_creare
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
          RegistrumServitii* reg;
        EntitasRepositorium* repo;

        repo = _creare_repo(piscina);
        CREDO_NON_NIHIL(repo);

        reg = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);
        CREDO_NON_NIHIL(reg->nomina);
        CREDO_VERUM(reg->repositorium == repo);
        CREDO_VERUM(reg->intern == intern);
        CREDO_AEQUALIS_I32(servitium_numerus(reg), ZEPHYRUM);

        /* Argumenta invalida */
        reg = registrum_servitii_creare(NIHIL, repo, intern);
        CREDO_NIHIL(reg);

        reg = registrum_servitii_creare(piscina, NIHIL, intern);
        CREDO_NIHIL(reg);

        reg = registrum_servitii_creare(piscina, repo, NIHIL);
        CREDO_NIHIL(reg);
    }
}


    /* ==================================================
     * Probare servitium_registrare
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
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* clock;
                    Entitas* logger;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        /* Creare entitates */
        clock = repo->entitas_scaffoldare(repo->datum, "System::Clock",
            "system::clock");
        logger = repo->entitas_scaffoldare(repo->datum,
            "System::Logger", "system::logger");
        CREDO_NON_NIHIL(clock);
        CREDO_NON_NIHIL(logger);

        /* Registrare servitia */
        CREDO_VERUM(servitium_registrare(reg, "clock", clock->id));
        CREDO_AEQUALIS_I32(servitium_numerus(reg), I);

        CREDO_VERUM(servitium_registrare(reg, "logger", logger->id));
        CREDO_AEQUALIS_I32(servitium_numerus(reg), II);

        /* Argumenta invalida */
        CREDO_FALSUM(servitium_registrare(NIHIL, "test", clock->id));
        CREDO_FALSUM(servitium_registrare(reg, NIHIL, clock->id));
        CREDO_FALSUM(servitium_registrare(reg, "test", NIHIL));
    }
}


    /* ==================================================
     * Probare servitium_registrare_entitatem
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
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* timer;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        timer = repo->entitas_scaffoldare(repo->datum, "System::Timer",
            "system::timer");
        CREDO_NON_NIHIL(timer);

        CREDO_VERUM(servitium_registrare_entitatem(reg, "timer",
            timer));
        CREDO_AEQUALIS_I32(servitium_numerus(reg), I);

        /* Argumenta invalida */
        CREDO_FALSUM(servitium_registrare_entitatem(NIHIL, "test",
            timer));
        CREDO_FALSUM(servitium_registrare_entitatem(reg, NIHIL, timer));
        CREDO_FALSUM(servitium_registrare_entitatem(reg, "test",
            NIHIL));
    }
}


    /* ==================================================
     * Probare servitium_habet
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
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* service;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        service = repo->entitas_scaffoldare(repo->datum, "Service",
            "test::service");
        CREDO_NON_NIHIL(service);

        /* Ante registrationem */
        CREDO_FALSUM(servitium_habet(reg, "my_service"));

        /* Post registrationem */
        CREDO_VERUM(servitium_registrare_entitatem(reg, "my_service",
            service));
        CREDO_VERUM(servitium_habet(reg, "my_service"));

        /* Servitium non existens */
        CREDO_FALSUM(servitium_habet(reg, "unknown"));

        /* Argumenta invalida */
        CREDO_FALSUM(servitium_habet(NIHIL, "test"));
        CREDO_FALSUM(servitium_habet(reg, NIHIL));
    }
}


    /* ==================================================
     * Probare servitium_obtinere
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
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* clock;
                    Entitas* obtentum;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        clock = repo->entitas_scaffoldare(repo->datum, "System::Clock",
            "services::clock");
        CREDO_NON_NIHIL(clock);

        CREDO_VERUM(servitium_registrare_entitatem(reg, "clock",
            clock));

        /* Obtinere servitium */
        obtentum = servitium_obtinere(reg, "clock");
        CREDO_NON_NIHIL(obtentum);
        CREDO_VERUM(obtentum == clock);

        /* Servitium non existens */
        obtentum = servitium_obtinere(reg, "unknown");
        CREDO_NIHIL(obtentum);

        /* Argumenta invalida */
        obtentum = servitium_obtinere(NIHIL, "clock");
        CREDO_NIHIL(obtentum);

        obtentum = servitium_obtinere(reg, NIHIL);
        CREDO_NIHIL(obtentum);
    }
}


    /* ==================================================
     * Probare servitium_obtinere_id
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
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* logger;
                     chorda* id;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        logger = repo->entitas_scaffoldare(repo->datum,
            "System::Logger", "services::logger");
        CREDO_NON_NIHIL(logger);

        CREDO_VERUM(servitium_registrare_entitatem(reg, "logger",
            logger));

        /* Obtinere ID */
        id = servitium_obtinere_id(reg, "logger");
        CREDO_NON_NIHIL(id);
        CREDO_VERUM(id == logger->id);

        /* Servitium non existens */
        id = servitium_obtinere_id(reg, "unknown");
        CREDO_NIHIL(id);
    }
}


    /* ==================================================
     * Probare servitium_deregistrare
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
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* service;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        service = repo->entitas_scaffoldare(repo->datum, "Service",
            "temp::service");
        CREDO_NON_NIHIL(service);

        CREDO_VERUM(servitium_registrare_entitatem(reg, "temp",
            service));
        CREDO_AEQUALIS_I32(servitium_numerus(reg), I);
        CREDO_VERUM(servitium_habet(reg, "temp"));

        /* Deregistrare */
        CREDO_VERUM(servitium_deregistrare(reg, "temp"));
        CREDO_AEQUALIS_I32(servitium_numerus(reg), ZEPHYRUM);
        CREDO_FALSUM(servitium_habet(reg, "temp"));

        /* Deregistrare non existens */
        CREDO_FALSUM(servitium_deregistrare(reg, "unknown"));

        /* Argumenta invalida */
        CREDO_FALSUM(servitium_deregistrare(NIHIL, "test"));
        CREDO_FALSUM(servitium_deregistrare(reg, NIHIL));
    }
}


    /* ==================================================
     * Probare multa servitia
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
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* servitia[X];
                  character  titulus_buffer[XXXII];
                  character  via_buffer[LXIV];
                        i32  i;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        /* Creare et registrare X servitia */
        per (i = ZEPHYRUM; i < X; i++)
        {
            sprintf(titulus_buffer, "service_%d", (integer)i);
            sprintf(via_buffer, "multi::service_%d", (integer)i);

            servitia[i] = repo->entitas_scaffoldare(repo->datum,
                "Service", via_buffer);
            CREDO_NON_NIHIL(servitia[i]);

            CREDO_VERUM(servitium_registrare_entitatem(reg,
                titulus_buffer, servitia[i]));
        }

        CREDO_AEQUALIS_I32(servitium_numerus(reg), X);

        /* Verificare omnes */
        per (i = ZEPHYRUM; i < X; i++)
        {
            Entitas* obtentum;

            sprintf(titulus_buffer, "service_%d", (integer)i);
            obtentum = servitium_obtinere(reg, titulus_buffer);
            CREDO_NON_NIHIL(obtentum);
            CREDO_VERUM(obtentum == servitia[i]);
        }
    }
}


    /* ==================================================
     * Probare nota servitii
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* service;
                        i32  numerus_notarum;
                        b32  invenit_notam;
                        i32  i;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        service = repo->entitas_scaffoldare(repo->datum, "Service",
            "noted::service");
        CREDO_NON_NIHIL(service);

        /* Registrare - debet addere notam "#service::clock" */
        CREDO_VERUM(servitium_registrare_entitatem(reg, "clock",
            service));

        /* Verificare notam in entitate */
        numerus_notarum  = xar_numerus(service->notae);
        invenit_notam    = FALSUM;

        per (i = ZEPHYRUM; i < numerus_notarum; i++)
        {
            chorda** slot;
            chorda*  nota;

            slot = (chorda**)xar_obtinere(service->notae, i);
            si (slot && *slot)
            {
                nota = *slot;
                si (chorda_aequalis_literis(*nota, "#service::clock"))
                {
                    invenit_notam = VERUM;
                    frange;
                }
            }
        }

        CREDO_VERUM(invenit_notam);
    }
}


    /* ==================================================
     * Probare servitium_discoperire
     * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* clock;
                    Entitas* logger;
                        i32  discooperti;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        /* Creare entitates cum notis servitii manuali */
        clock = repo->entitas_scaffoldare(repo->datum, "Clock",
            "disco::clock");
        CREDO_NON_NIHIL(clock);
        repo->nota_addere(repo->datum, clock, "#service::clock");

        logger = repo->entitas_scaffoldare(repo->datum, "Logger",
            "disco::logger");
        CREDO_NON_NIHIL(logger);
        repo->nota_addere(repo->datum, logger, "#service::logger");

        /* Ante discoperire */
        CREDO_AEQUALIS_I32(servitium_numerus(reg), ZEPHYRUM);

        /* Discoperire */
        discooperti = servitium_discoperire(reg);
        CREDO_AEQUALIS_I32(discooperti, II);
        CREDO_AEQUALIS_I32(servitium_numerus(reg), II);

        /* Verificare servitia */
        CREDO_VERUM(servitium_habet(reg, "clock"));
        CREDO_VERUM(servitium_habet(reg, "logger"));

        CREDO_VERUM(servitium_obtinere(reg, "clock") == clock);
        CREDO_VERUM(servitium_obtinere(reg, "logger") == logger);
    }
}


    /* ==================================================
     * Probare discoperire sine duplicatis
     * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    {
          RegistrumServitii* reg;
        EntitasRepositorium* repo;
                    Entitas* service;
                        i32  discooperti;

        repo  = _creare_repo(piscina);
        reg   = registrum_servitii_creare(piscina, repo, intern);
        CREDO_NON_NIHIL(reg);

        /* Creare entitas cum nota */
        service = repo->entitas_scaffoldare(repo->datum, "Service",
            "dup::service");
        CREDO_NON_NIHIL(service);
        repo->nota_addere(repo->datum, service,
            "#service::dup_service");

        /* Prima discoperitio */
        discooperti = servitium_discoperire(reg);
        CREDO_AEQUALIS_I32(discooperti, I);
        CREDO_AEQUALIS_I32(servitium_numerus(reg), I);

        /* Secunda discoperitio - non debet addere duplicatum */
        discooperti = servitium_discoperire(reg);
        CREDO_AEQUALIS_I32(discooperti, ZEPHYRUM);
        CREDO_AEQUALIS_I32(servitium_numerus(reg), I);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "registrum_servitii_creare", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "servitium_registrare", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "servitium_registrare_entitatem", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "servitium_habet", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "servitium_obtinere", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "servitium_obtinere_id", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "servitium_deregistrare", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "multa servitia", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nota servitii", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "servitium_discoperire", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "discoperire sine duplicatis", _sectio_decima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("servitium", SECTIONES);
}
