/* probatio_actor_integratio.c - Probatio Integrationis Systematis Actorum
 *
 * Hoc test demonstrat omnia componenta systematis actorum
 * operantia simul: Clock widget cum timer que mittit "tick"
 * nuntios periodice.
 *
 * Componenta:
 * - EntitasRepositorium: Persistentia entitatum
 * - CursusNuntiorum: Message bus
 * - RegistrumTractatoris: Handler registry
 * - AdministratorTempestivi: Timer management
 * - RegistrumServitii: Service discovery
 */

#include "latina.h"
#include "cursus.h"
#include "tractator.h"
#include "tempestivum.h"
#include "servitium.h"
#include "entitas_repositorium.h"
#include "persistentia.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>
#include <unistd.h>


/* ==================================================
 * Clock Widget Statum
 * ================================================== */

nomen structura ClockState {
    i32 tick_contador;
    i32 ultimus_tick;
} ClockState;

hic_manens ClockState g_clock_state = {ZEPHYRUM, ZEPHYRUM};


/* ==================================================
 * Clock Widget Tractator
 * ================================================== */

interior b32
clock_tractare_tick(ContextusTractandi* ctx)
{
    g_clock_state.tick_contador++;
    g_clock_state.ultimus_tick = g_clock_state.tick_contador;

    /* Actualizare proprietatem "ticks" in entitate */
    {
        character buffer[XXXII];
        sprintf(buffer, "%d", g_clock_state.tick_contador);

        ctx->repositorium->proprietas_ponere(
            ctx->repositorium->datum,
            ctx->entitas,
            "ticks",
            buffer);
    }

    redde VERUM;
}


/* ==================================================
 * Simulatio "Game Loop"
 * ================================================== */

interior vacuum
simulare_quadrum(
    AdministratorTempestivi* admin,
    CursusNuntiorum*         cursus)
{
    /* Processare tempestiva (mittit nuntios si tempus) */
    tempestivum_processare(admin);

    /* Processare nuntios (invocat tractatores) */
    cursus_processare(cursus, C);
}


s32 principale(vacuum)
{
    Piscina*                 piscina;
    InternamentumChorda*     intern;
    Persistentia*            pers;
    EntitasRepositorium*     repo;
    RegistrumTractatoris*    tractator;
    CursusNuntiorum*         cursus;
    AdministratorTempestivi* tempestivum_admin;
    RegistrumServitii*       servitium_reg;
    b32                      praeteritus;


    /* ==================================================
     * Initium
     * ================================================== */

    imprimere("\n=== PROBATIO INTEGRATIONIS ACTORUM ===\n\n");

    piscina = piscina_generare_dynamicum("test_integratio", MMM * CXXVIII);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }

    intern = internamentum_globale();
    si (!intern)
    {
        imprimere("FRACTA: internamentum_globale\n");
        piscina_destruere(piscina);
        redde I;
    }

    credo_aperire(piscina);


    /* ==================================================
     * Creare Infrastructuram
     * ================================================== */

    imprimere("--- Creans infrastructuram ---\n");

    /* Persistentia */
    pers = persistentia_memoria_creare(piscina);
    CREDO_NON_NIHIL(pers);

    /* Repositorium */
    repo = entitas_repositorium_creare(piscina, pers);
    CREDO_NON_NIHIL(repo);

    /* Tractator Registry */
    tractator = registrum_tractatoris_creare(piscina, intern);
    CREDO_NON_NIHIL(tractator);

    /* Message Bus */
    cursus = cursus_creare(piscina, tractator, repo, intern);
    CREDO_NON_NIHIL(cursus);

    /* Timer Administrator */
    tempestivum_admin = administrator_tempestivi_creare(piscina, repo, cursus, intern);
    CREDO_NON_NIHIL(tempestivum_admin);

    /* Service Registry */
    servitium_reg = registrum_servitii_creare(piscina, repo, intern);
    CREDO_NON_NIHIL(servitium_reg);

    imprimere("  Infrastructura creata\n");


    /* ==================================================
     * Registrare Tractatores
     * ================================================== */

    imprimere("\n--- Registrans tractatores ---\n");

    CREDO_VERUM(registrum_tractatoris_registrare(
        tractator, "Clock", "tick", clock_tractare_tick));

    imprimere("  Tractator Clock::tick registratus\n");


    /* ==================================================
     * Creare Clock Widget
     * ================================================== */

    {
        Entitas* clock;
        Entitas* timer;

        imprimere("\n--- Creans Clock widget ---\n");

        /* Creare entitatem Clock */
        clock = repo->entitas_scaffoldare(repo->datum, "Clock", "system::clock");
        CREDO_NON_NIHIL(clock);
        CREDO_NON_NIHIL(clock->id);

        /* Ponere proprietatem initialem */
        repo->proprietas_ponere(repo->datum, clock, "ticks", "0");

        /* Registrare ut servitium */
        CREDO_VERUM(servitium_registrare_entitatem(servitium_reg, "clock", clock));
        imprimere("  Clock creatum et registratum ut servitium\n");

        /* Creare timer pro tick (10ms intervallum, delay initialis 10ms) */
        timer = tempestivum_creare_cum_delay(
            tempestivum_admin,
            clock->id,
            "tick",
            X,   /* intervallum: 10ms */
            X,   /* delay: 10ms */
            VERUM);  /* repetere */

        CREDO_NON_NIHIL(timer);
        imprimere("  Timer creatum (10ms intervallo)\n");


        /* ==================================================
         * Simulare Cyclos
         * ================================================== */

        imprimere("\n--- Simulans cyclos ---\n");

        /* Reset state */
        g_clock_state.tick_contador = ZEPHYRUM;
        g_clock_state.ultimus_tick  = ZEPHYRUM;

        /* Cyclus 1: Exspectare et processare */
        usleep(15000);  /* 15ms */
        simulare_quadrum(tempestivum_admin, cursus);
        imprimere("  Cyclus 1: ticks = %d\n", g_clock_state.tick_contador);
        CREDO_AEQUALIS_I32(g_clock_state.tick_contador, I);

        /* Cyclus 2 */
        usleep(15000);
        simulare_quadrum(tempestivum_admin, cursus);
        imprimere("  Cyclus 2: ticks = %d\n", g_clock_state.tick_contador);
        CREDO_AEQUALIS_I32(g_clock_state.tick_contador, II);

        /* Cyclus 3 */
        usleep(15000);
        simulare_quadrum(tempestivum_admin, cursus);
        imprimere("  Cyclus 3: ticks = %d\n", g_clock_state.tick_contador);
        CREDO_AEQUALIS_I32(g_clock_state.tick_contador, III);


        /* ==================================================
         * Verificare Statum Entitatis
         * ================================================== */

        imprimere("\n--- Verificans statum entitatis ---\n");

        {
            chorda* ticks_clavis;
            chorda* ticks_valor;

            ticks_clavis = chorda_internare_ex_literis(intern, "ticks");
            CREDO_NON_NIHIL(ticks_clavis);

            ticks_valor = entitas_proprietas_capere(clock, ticks_clavis);
            CREDO_NON_NIHIL(ticks_valor);

            imprimere("  Proprietas 'ticks' = %.*s\n",
                      ticks_valor->mensura, ticks_valor->datum);

            /* Verificare valor est "3" */
            CREDO_VERUM(chorda_aequalis_literis(*ticks_valor, "3"));
        }


        /* ==================================================
         * Verificare Servitium
         * ================================================== */

        imprimere("\n--- Verificans servitium ---\n");

        {
            Entitas* clock_servitium;

            clock_servitium = servitium_obtinere(servitium_reg, "clock");
            CREDO_NON_NIHIL(clock_servitium);
            CREDO_VERUM(clock_servitium == clock);

            imprimere("  Servitium 'clock' verificatum\n");
        }


        /* ==================================================
         * Pausare Timer
         * ================================================== */

        imprimere("\n--- Pausans timer ---\n");

        CREDO_VERUM(tempestivum_pausare(tempestivum_admin, timer->id));
        imprimere("  Timer pausatum\n");

        /* Cyclus 4: Timer pausatum - non debet incrementare */
        usleep(15000);
        simulare_quadrum(tempestivum_admin, cursus);
        imprimere("  Cyclus 4 (post pausam): ticks = %d\n", g_clock_state.tick_contador);
        CREDO_AEQUALIS_I32(g_clock_state.tick_contador, III);  /* Adhuc 3 */


        /* ==================================================
         * Resumere Timer
         * ================================================== */

        imprimere("\n--- Resumens timer ---\n");

        CREDO_VERUM(tempestivum_resumere(tempestivum_admin, timer->id));
        imprimere("  Timer resumptum\n");

        /* Cyclus 5: Timer resumptum */
        usleep(15000);
        simulare_quadrum(tempestivum_admin, cursus);
        imprimere("  Cyclus 5 (post resumptionem): ticks = %d\n",
                  g_clock_state.tick_contador);
        CREDO_AEQUALIS_I32(g_clock_state.tick_contador, IV);


        /* ==================================================
         * Annullare Timer
         * ================================================== */

        imprimere("\n--- Annullans timer ---\n");

        CREDO_VERUM(tempestivum_annullare(tempestivum_admin, timer->id));
        imprimere("  Timer annullatum\n");

        /* Cyclus 6: Timer annullatum - non debet incrementare */
        usleep(15000);
        simulare_quadrum(tempestivum_admin, cursus);
        imprimere("  Cyclus 6 (post annullationem): ticks = %d\n",
                  g_clock_state.tick_contador);
        CREDO_AEQUALIS_I32(g_clock_state.tick_contador, IV);  /* Adhuc 4 */
    }


    /* ==================================================
     * Probare Multae Entitates
     * ================================================== */

    {
        Entitas* widget1;
        Entitas* widget2;
        i32      tractati_ante;

        imprimere("\n--- Probans multas entitates ---\n");

        /* Reset state */
        g_clock_state.tick_contador = ZEPHYRUM;

        /* Creare duo widgets */
        widget1 = repo->entitas_scaffoldare(repo->datum, "Clock", "multi::clock1");
        widget2 = repo->entitas_scaffoldare(repo->datum, "Clock", "multi::clock2");
        CREDO_NON_NIHIL(widget1);
        CREDO_NON_NIHIL(widget2);

        /* Creare timers */
        CREDO_NON_NIHIL(tempestivum_creare_cum_delay(
            tempestivum_admin, widget1->id, "tick", X, X, VERUM));
        CREDO_NON_NIHIL(tempestivum_creare_cum_delay(
            tempestivum_admin, widget2->id, "tick", X, X, VERUM));

        /* Simulare */
        usleep(15000);
        tractati_ante = g_clock_state.tick_contador;
        simulare_quadrum(tempestivum_admin, cursus);

        /* Ambae entitates debent recepisse tick */
        imprimere("  Ticks post simulationem: %d (ante: %d)\n",
                  g_clock_state.tick_contador, tractati_ante);
        CREDO_AEQUALIS_I32(g_clock_state.tick_contador - tractati_ante, II);
    }


    /* ==================================================
     * Probare Nuntium Directum (sine timer)
     * ================================================== */

    {
        Entitas* receiver;
        i32      tractati_ante;

        imprimere("\n--- Probans nuntium directum ---\n");

        receiver = repo->entitas_scaffoldare(repo->datum, "Clock", "direct::receiver");
        CREDO_NON_NIHIL(receiver);

        /* Reset */
        g_clock_state.tick_contador = ZEPHYRUM;
        tractati_ante = g_clock_state.tick_contador;

        /* Mittere nuntium directe */
        CREDO_VERUM(cursus_mittere_simplex(cursus, NIHIL, receiver->id, "tick"));
        CREDO_AEQUALIS_I32(cursus_numerus_pendentium(cursus), I);

        /* Processare */
        cursus_processare(cursus, M);

        imprimere("  Ticks post nuntium directum: %d\n", g_clock_state.tick_contador);
        CREDO_AEQUALIS_I32(g_clock_state.tick_contador - tractati_ante, I);
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

    imprimere("\n=== FINIS PROBATIONIS ===\n\n");

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
