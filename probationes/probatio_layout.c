/* probatio_layout.c - Probationes Layout System */
#include "latina.h"
#include "layout.h"
#include "piscina.h"
#include "internamentum.h"
#include "persistentia.h"
#include "entitas_repositorium.h"
#include "widget.h"
#include "xar.h"
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

    /* Usare internamentum globale - idem quod repositorium utitur */
    intern = internamentum_globale();
    si (!intern)
    {
        imprimere("FRACTA: internamentum_globale\n");
        piscina_destruere(piscina);
        redde I;
    }

    /* Creare contextum sine repositorium pro probationes simplices */
    ctx_sine_repo = contextus_widget_creare(piscina, intern, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL);
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
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);
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
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

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
     * ENTITAS TESTS
     * ================================================== */

    /* === Entitas Simplex === */
    imprimere("\n--- Probans layout_entitas_simplex ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        Entitas*             entitas;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

        dom = layout_creare(ctx,
            "<layout>"
            "  <entitas genus='Page' slug='test'/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        /* Verificare entitas creata - usare scaffoldare quod est idempotens */
        entitas = repo->entitas_scaffoldare(repo->datum, "Page", "test");
        CREDO_NON_NIHIL(entitas);
        CREDO_CHORDA_AEQUALIS_LITERIS(*entitas->genus, "Page");

        imprimere("  Entitas simplex: VERUM\n");
    }

    /* === Entitas cum Proprietatibus === */
    imprimere("\n--- Probans layout_entitas_proprietates ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        Entitas*             entitas;
        chorda*              valor;
        chorda*              clavis_name;
        chorda*              clavis_author;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

        dom = layout_creare(ctx,
            "<layout>"
            "  <entitas genus='Page' slug='intro'>"
            "    <proprietas clavis='name' valor='Introduction'/>"
            "    <proprietas clavis='author' valor='Fran'/>"
            "  </entitas>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        entitas = repo->entitas_scaffoldare(repo->datum, "Page", "intro");
        CREDO_NON_NIHIL(entitas);

        /* Verificare proprietates - clavis debet esse pointer internata */
        clavis_name = chorda_internare_ex_literis(intern, "name");
        clavis_author = chorda_internare_ex_literis(intern, "author");

        valor = entitas_proprietas_capere(entitas, clavis_name);
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "Introduction");

        valor = entitas_proprietas_capere(entitas, clavis_author);
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "Fran");

        imprimere("  Entitas proprietates: VERUM\n");
    }

    /* === Entitas cum Notae === */
    imprimere("\n--- Probans layout_entitas_notae ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        Entitas*             entitas;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

        dom = layout_creare(ctx,
            "<layout>"
            "  <entitas genus='Note' slug='important'>"
            "    <nota>#urgent</nota>"
            "    <nota>#project::alpha</nota>"
            "  </entitas>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        entitas = repo->entitas_scaffoldare(repo->datum, "Note", "important");
        CREDO_NON_NIHIL(entitas);
        CREDO_NON_NIHIL(entitas->notae);
        CREDO_AEQUALIS_I32(xar_numerus(entitas->notae), II);

        imprimere("  Entitas notae: VERUM\n");
    }

    /* === Entitas cum Relationes === */
    imprimere("\n--- Probans layout_entitas_relationes ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        Entitas*             entitas_one;
        Entitas*             entitas_two;
        Xar*                 relatae;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

        dom = layout_creare(ctx,
            "<layout>"
            "  <entitas genus='Page' slug='one'>"
            "    <relatio genus='next' ad='Page::two'/>"
            "  </entitas>"
            "  <entitas genus='Page' slug='two'>"
            "    <relatio genus='prev' ad='Page::one'/>"
            "  </entitas>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        entitas_one = repo->entitas_scaffoldare(repo->datum, "Page", "one");
        entitas_two = repo->entitas_scaffoldare(repo->datum, "Page", "two");
        CREDO_NON_NIHIL(entitas_one);
        CREDO_NON_NIHIL(entitas_two);

        /* Verificare relationes - nota: entitas habet "est" relatio automatica ad genus */
        CREDO_NON_NIHIL(entitas_one->relationes);
        CREDO_AEQUALIS_I32(xar_numerus(entitas_one->relationes), II);  /* "next" + "est" */

        CREDO_NON_NIHIL(entitas_two->relationes);
        CREDO_AEQUALIS_I32(xar_numerus(entitas_two->relationes), II);  /* "prev" + "est" */

        /* Verificare relatio resolvit ad entitas correcta */
        {
            chorda* genus_next;
            genus_next = chorda_internare_ex_literis(intern, "next");
            relatae = repo->capere_entitates_relatae(repo->datum, entitas_one, genus_next);
            CREDO_NON_NIHIL(relatae);
            CREDO_AEQUALIS_I32(xar_numerus(relatae), I);
        }

        imprimere("  Entitas relationes: VERUM\n");
    }

    /* === Entitas Hierarchicum (Namespaced Genus) === */
    imprimere("\n--- Probans layout_entitas_hierarchicum ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        Entitas*             entitas;
        chorda*              valor;
        chorda*              clavis_title;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

        dom = layout_creare(ctx,
            "<layout>"
            "  <entitas genus='Content::Document::Article' slug='post'>"
            "    <proprietas clavis='title' valor='My Post'/>"
            "  </entitas>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        entitas = repo->entitas_scaffoldare(repo->datum, "Content::Document::Article", "post");
        CREDO_NON_NIHIL(entitas);
        CREDO_CHORDA_AEQUALIS_LITERIS(*entitas->genus, "Content::Document::Article");

        clavis_title = chorda_internare_ex_literis(intern, "title");
        valor = entitas_proprietas_capere(entitas, clavis_title);
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "My Post");

        imprimere("  Entitas hierarchicum: VERUM\n");
    }

    /* === Entitas Idempotens === */
    imprimere("\n--- Probans layout_entitas_idempotens ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        Entitas*             entitas;
        chorda*              valor;
        chorda*              clavis_a;
        chorda*              clavis_b;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

        /* Duo declarationes pro eadem entitas */
        dom = layout_creare(ctx,
            "<layout>"
            "  <entitas genus='Page' slug='same'>"
            "    <proprietas clavis='a' valor='1'/>"
            "  </entitas>"
            "  <entitas genus='Page' slug='same'>"
            "    <proprietas clavis='b' valor='2'/>"
            "  </entitas>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        entitas = repo->entitas_scaffoldare(repo->datum, "Page", "same");
        CREDO_NON_NIHIL(entitas);

        /* Ambae proprietates debent existere */
        clavis_a = chorda_internare_ex_literis(intern, "a");
        clavis_b = chorda_internare_ex_literis(intern, "b");

        valor = entitas_proprietas_capere(entitas, clavis_a);
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "1");

        valor = entitas_proprietas_capere(entitas, clavis_b);
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "2");

        imprimere("  Entitas idempotens: VERUM\n");
    }

    /* === Layout Mixta (Widgets et Entitates) === */
    imprimere("\n--- Probans layout_mixta ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;
        Pagina*              pagina;
        Entitas*             entitas;
        chorda*              valor;
        chorda*              clavis_title;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

        dom = layout_creare(ctx,
            "<layout>"
            "  <pagina id='editor' x=0 y=0 latitudo=80 altitudo=40/>"
            "  <entitas genus='Page' slug='content'>"
            "    <proprietas clavis='title' valor='Test'/>"
            "  </entitas>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        /* Verificare widget creatum */
        pagina = layout_obtinere_pagina(dom, "editor");
        CREDO_NON_NIHIL(pagina);

        /* Verificare entitas creata */
        entitas = repo->entitas_scaffoldare(repo->datum, "Page", "content");
        CREDO_NON_NIHIL(entitas);

        clavis_title = chorda_internare_ex_literis(intern, "title");
        valor = entitas_proprietas_capere(entitas, clavis_title);
        CREDO_NON_NIHIL(valor);
        CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "Test");

        imprimere("  Layout mixta: VERUM\n");
    }

    /* === Entitas Error Cases === */
    imprimere("\n--- Probans layout_entitas_errores ---\n");

    {
        LayoutDom*           dom;
        Persistentia*        persistentia;
        EntitasRepositorium* repo;
        ContextusWidget*     ctx;

        persistentia = persistentia_memoria_creare(piscina);
        repo = entitas_repositorium_creare(piscina, persistentia);
        ctx = contextus_widget_creare(piscina, intern, repo, NIHIL, NIHIL, NIHIL, NIHIL);

        /* Entitas sine genus - debet ignorari */
        dom = layout_creare(ctx,
            "<layout>"
            "  <entitas slug='orphan'/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);  /* Layout creatur sed entitas non */

        /* Entitas sine slug - debet ignorari */
        dom = layout_creare(ctx,
            "<layout>"
            "  <entitas genus='Page'/>"
            "</layout>");

        CREDO_NON_NIHIL(dom);

        imprimere("  Entitas errores: VERUM\n");
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
