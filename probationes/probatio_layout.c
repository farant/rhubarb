/* probatio_layout.c - Probationes Layout System */
#include "latina.h"
#include "layout.h"
#include "piscina.h"
#include "internamentum.h"
#include "persistentia.h"
#include "entitas_repositorium.h"
#include "widget.h"
#include "credo.h"
#include <stdio.h>

s32 principale(vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    ContextusWidget*     ctx_sine_repo;  /* Contextus sine repositorium */
    b32                  praeteritus;

    /* Aperire piscinam et credonem */
    piscina = piscina_generare_dynamicum("probatio_layout", M * II);  /* 2MB */
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

    /* Creare contextum sine repositorium pro probationes simplices */
    ctx_sine_repo = contextus_widget_creare(piscina, intern, NIHIL, NIHIL);
    si (!ctx_sine_repo)
    {
        imprimere("FRACTA: contextus_widget_creare\n");
        piscina_destruere(piscina);
        redde I;
    }

    credo_aperire(piscina);

    /* ==================================================
     * Probare layout_creare cum pagina simplex
     * ================================================== */

    imprimere("\n--- Probans layout_creare cum pagina simplex ---\n");

    {
        LayoutDom* dom;

        dom = layout_creare(ctx_sine_repo,
            "<layout>"
            "  <pagina id='editor' x=0 y=0 latitudo=71 altitudo=60/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);
        CREDO_NON_NIHIL(dom->manager);
        CREDO_NON_NIHIL(dom->widgets);

        imprimere("  Layout creatum: VERUM\n");
    }

    /* ==================================================
     * Probare layout_obtinere_pagina
     * ================================================== */

    imprimere("\n--- Probans layout_obtinere_pagina ---\n");

    {
        LayoutDom* dom;
        Pagina*    pagina;

        dom = layout_creare(ctx_sine_repo,
            "<layout>"
            "  <pagina id='mea-pagina' x=10 y=5 latitudo=80 altitudo=40/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        pagina = layout_obtinere_pagina(dom, "mea-pagina");
        CREDO_NON_NIHIL(pagina);

        /* Pagina non inventa */
        CREDO_NIHIL(layout_obtinere_pagina(dom, "non-existit"));

        imprimere("  Obtinere pagina: VERUM\n");
    }

    /* ==================================================
     * Probare layout cum navigator (requirit repositorium)
     * ================================================== */

    imprimere("\n--- Probans layout cum navigator ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        NavigatorEntitatum*  nav;

        /* Creare persistentia et repositorium */
        persistentia = persistentia_memoria_creare(piscina);
        CREDO_NON_NIHIL(persistentia);

        repo = entitas_repositorium_creare(piscina, persistentia);
        CREDO_NON_NIHIL(repo);

        /* Creare contextum cum repositorium */
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL);
        CREDO_NON_NIHIL(ctx);

        /* Creare layout cum navigator */
        dom = layout_creare(ctx,
            "<layout>"
            "  <navigator id='nav' x=0 y=0 latitudo=71 altitudo=60/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        nav = layout_obtinere_navigator(dom, "nav");
        CREDO_NON_NIHIL(nav);

        imprimere("  Navigator creatum: VERUM\n");
    }

    /* ==================================================
     * Probare layout cum pagina et navigator
     * ================================================== */

    imprimere("\n--- Probans layout cum pagina et navigator ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        Pagina*              pagina;
        NavigatorEntitatum*  nav;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL);

        dom = layout_creare(ctx,
            "<layout>"
            "  <pagina id='editor' x=0 y=0 latitudo=71 altitudo=60/>"
            "  <navigator id='browser' x=71 y=0 latitudo=71 altitudo=60/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        pagina = layout_obtinere_pagina(dom, "editor");
        CREDO_NON_NIHIL(pagina);

        nav = layout_obtinere_navigator(dom, "browser");
        CREDO_NON_NIHIL(nav);

        /* Verificare genus correctum */
        CREDO_NIHIL(layout_obtinere_navigator(dom, "editor"));   /* pagina, non navigator */
        CREDO_NIHIL(layout_obtinere_pagina(dom, "browser"));     /* navigator, non pagina */

        imprimere("  Duo widgets: VERUM\n");
    }

    /* ==================================================
     * Probare layout_obtinere (genericum)
     * ================================================== */

    imprimere("\n--- Probans layout_obtinere ---\n");

    {
        LayoutDom* dom;
        vacuum*    widget;

        dom = layout_creare(ctx_sine_repo,
            "<layout>"
            "  <pagina id='test' x=0 y=0 latitudo=50 altitudo=30/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        widget = layout_obtinere(dom, "test");
        CREDO_NON_NIHIL(widget);

        widget = layout_obtinere(dom, "non-existit");
        CREDO_NIHIL(widget);

        imprimere("  Obtinere genericum: VERUM\n");
    }

    /* ==================================================
     * Probare layout sine repositorium (navigator fallit)
     * ================================================== */

    imprimere("\n--- Probans layout sine repositorium ---\n");

    {
        LayoutDom*          dom;
        NavigatorEntitatum* nav;

        /* Navigator sine repositorium non debet creari */
        dom = layout_creare(ctx_sine_repo,
            "<layout>"
            "  <navigator id='nav' x=0 y=0 latitudo=71 altitudo=60/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);  /* Layout creatur */

        nav = layout_obtinere_navigator(dom, "nav");
        CREDO_NIHIL(nav);  /* Sed navigator non creatur sine repo */

        imprimere("  Navigator sine repo: VERUM\n");
    }

    /* ==================================================
     * Probare attributa defalta
     * ================================================== */

    imprimere("\n--- Probans attributa defalta ---\n");

    {
        LayoutDom* dom;
        Pagina*    pagina;

        /* Pagina sine attributa explicita */
        dom = layout_creare(ctx_sine_repo,
            "<layout>"
            "  <pagina id='minimal'/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        pagina = layout_obtinere_pagina(dom, "minimal");
        CREDO_NON_NIHIL(pagina);

        imprimere("  Attributa defalta: VERUM\n");
    }

    /* ==================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Purificatio */
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
