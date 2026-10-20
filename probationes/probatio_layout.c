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


/* ==================================================
 * Probare layout_creare cum pagina simplex
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
              LayoutDom* dom;
    InternamentumChorda* intern;
        ContextusWidget* ctx_sine_repo;

    intern = internamentum_globale();
    ctx_sine_repo = contextus_widget_creare(c->piscina, intern, NIHIL,
        NIHIL, NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx_sine_repo,
        "<layout>"
        "  <pagina id=\"editor\" x=0 y=0 latitudo=71 altitudo=60/>"
        "</layout>");

    CREDO_NON_NIHIL(dom);
    CREDO_NON_NIHIL(dom->manager);
    CREDO_NON_NIHIL(dom->widgets);

    imprimere("  Layout creatum: VERUM\n");
}


/* ==================================================
 * Probare layout_obtinere_pagina
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
              LayoutDom* dom;
                 Pagina* pagina;
    InternamentumChorda* intern;
        ContextusWidget* ctx_sine_repo;

    intern = internamentum_globale();
    ctx_sine_repo = contextus_widget_creare(c->piscina, intern, NIHIL,
        NIHIL, NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx_sine_repo,
        "<layout>"
        "  <pagina id=\"mea-pagina\" x=10 y=5 latitudo=80 altitudo=40/>"
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

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
     NavigatorEntitatum* nav;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    /* Creare persistentia et repositorium */
    persistentia = persistentia_memoria_creare(c->piscina);
    CREDO_NON_NIHIL(persistentia);

    repo = entitas_repositorium_creare(c->piscina, persistentia);
    CREDO_NON_NIHIL(repo);

    /* Creare contextum cum repositorium */
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);
    CREDO_NON_NIHIL(ctx);

    /* Creare layout cum navigator */
    dom = layout_creare(ctx,
        "<layout>"
        "  <navigator id=\"nav\" x=0 y=0 latitudo=71 altitudo=60/>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    nav = layout_obtinere_navigator(dom, "nav");
    CREDO_NON_NIHIL(nav);

    imprimere("  Navigator creatum: VERUM\n");
}


/* ==================================================
 * Probare layout cum pagina et navigator
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
                 Pagina* pagina;
     NavigatorEntitatum* nav;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx,
        "<layout>"
        "  <pagina id=\"editor\" x=0 y=0 latitudo=71 altitudo=60/>"
        "  <navigator id=\"browser\" x=71 y=0 latitudo=71 altitudo=60/>"
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

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
              LayoutDom* dom;
                 vacuum* widget;
    InternamentumChorda* intern;
        ContextusWidget* ctx_sine_repo;

    intern = internamentum_globale();
    ctx_sine_repo = contextus_widget_creare(c->piscina, intern, NIHIL,
        NIHIL, NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx_sine_repo,
        "<layout>"
        "  <pagina id=\"test\" x=0 y=0 latitudo=50 altitudo=30/>"
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

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
              LayoutDom* dom;
     NavigatorEntitatum* nav;
    InternamentumChorda* intern;
        ContextusWidget* ctx_sine_repo;

    intern = internamentum_globale();
    ctx_sine_repo = contextus_widget_creare(c->piscina, intern, NIHIL,
        NIHIL, NIHIL, NIHIL, NIHIL);

    /* Navigator sine repositorium non debet creari */
    dom = layout_creare(ctx_sine_repo,
        "<layout>"
        "  <navigator id=\"nav\" x=0 y=0 latitudo=71 altitudo=60/>"
        "</layout>");

    CREDO_NON_NIHIL(dom);  /* Layout creatur */

    nav = layout_obtinere_navigator(dom, "nav");
    CREDO_NIHIL(nav);  /* Sed navigator non creatur sine repo */

    imprimere("  Navigator sine repo: VERUM\n");
}


/* ==================================================
 * Probare attributa defalta
 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
              LayoutDom* dom;
                 Pagina* pagina;
    InternamentumChorda* intern;
        ContextusWidget* ctx_sine_repo;

    intern = internamentum_globale();
    ctx_sine_repo = contextus_widget_creare(c->piscina, intern, NIHIL,
        NIHIL, NIHIL, NIHIL, NIHIL);

    /* Pagina sine attributa explicita */
    dom = layout_creare(ctx_sine_repo,
        "<layout>"
        "  <pagina id=\"minimal\"/>"
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
interior vacuum
_sectio_octava (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
                Entitas* entitas;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx,
        "<layout>"
        "  <entitas genus=\"Page\" slug=\"test\"/>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    /* Verificare entitas creata - usare scaffoldare quod est idempotens */
    entitas = repo->entitas_scaffoldare(repo->datum, "Page",
        "test");
    CREDO_NON_NIHIL(entitas);
    CREDO_CHORDA_AEQUALIS_LITERIS(*entitas->genus, "Page");

    imprimere("  Entitas simplex: VERUM\n");
}


/* === Entitas cum Proprietatibus === */
interior vacuum
_sectio_nona (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
                Entitas* entitas;
                 chorda* valor;
                 chorda* clavis_name;
                 chorda* clavis_author;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx,
        "<layout>"
        "  <entitas genus=\"Page\" slug=\"intro\">"
        "    <proprietas clavis=\"name\" valor=\"Introduction\"/>"
        "    <proprietas clavis=\"author\" valor=\"Fran\"/>"
        "  </entitas>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    entitas = repo->entitas_scaffoldare(repo->datum, "Page",
        "intro");
    CREDO_NON_NIHIL(entitas);

    /* Verificare proprietates - clavis debet esse pointer internata */
    clavis_name    = chorda_internare_ex_literis(intern, "name");
    clavis_author  = chorda_internare_ex_literis(intern, "author");

    valor = entitas_proprietas_capere(entitas, clavis_name);
    CREDO_NON_NIHIL(valor);
    CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "Introduction");

    valor = entitas_proprietas_capere(entitas, clavis_author);
    CREDO_NON_NIHIL(valor);
    CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "Fran");

    imprimere("  Entitas proprietates: VERUM\n");
}


/* === Entitas cum Notae === */
interior vacuum
_sectio_decima (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
                Entitas* entitas;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx,
        "<layout>"
        "  <entitas genus=\"Note\" slug=\"important\">"
        "    <nota>#urgent</nota>"
        "    <nota>#project::alpha</nota>"
        "  </entitas>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    entitas = repo->entitas_scaffoldare(repo->datum, "Note",
        "important");
    CREDO_NON_NIHIL(entitas);
    CREDO_NON_NIHIL(entitas->notae);
    CREDO_AEQUALIS_I32(xar_numerus(entitas->notae), II);

    imprimere("  Entitas notae: VERUM\n");
}


/* === Entitas cum Relationes === */
interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
                Entitas* entitas_one;
                Entitas* entitas_two;
                    Xar* relatae;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx,
        "<layout>"
        "  <entitas genus=\"Page\" slug=\"one\">"
        "    <relatio genus=\"next\" ad=\"Page::two\"/>"
        "  </entitas>"
        "  <entitas genus=\"Page\" slug=\"two\">"
        "    <relatio genus=\"prev\" ad=\"Page::one\"/>"
        "  </entitas>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    entitas_one = repo->entitas_scaffoldare(repo->datum, "Page",
        "one");
    entitas_two = repo->entitas_scaffoldare(repo->datum, "Page",
        "two");
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
        relatae = repo->capere_entitates_relatae(repo->datum,
            entitas_one, genus_next);
        CREDO_NON_NIHIL(relatae);
        CREDO_AEQUALIS_I32(xar_numerus(relatae), I);
    }

    imprimere("  Entitas relationes: VERUM\n");
}


/* === Entitas Hierarchicum (Namespaced Genus) === */
interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
                Entitas* entitas;
                 chorda* valor;
                 chorda* clavis_title;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx,
        "<layout>"
        "  <entitas genus=\"Content::Document::Article\" slug=\"post\">"
        "    <proprietas clavis=\"title\" valor=\"My Post\"/>"
        "  </entitas>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    entitas = repo->entitas_scaffoldare(repo->datum,
        "Content::Document::Article", "post");
    CREDO_NON_NIHIL(entitas);
    CREDO_CHORDA_AEQUALIS_LITERIS(*entitas->genus,
        "Content::Document::Article");

    clavis_title  = chorda_internare_ex_literis(intern, "title");
    valor         = entitas_proprietas_capere(entitas, clavis_title);
    CREDO_NON_NIHIL(valor);
    CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "My Post");

    imprimere("  Entitas hierarchicum: VERUM\n");
}


/* === Entitas Idempotens === */
interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
                Entitas* entitas;
                 chorda* valor;
                 chorda* clavis_a;
                 chorda* clavis_b;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    /* Duo declarationes pro eadem entitas */
    dom = layout_creare(ctx,
        "<layout>"
        "  <entitas genus=\"Page\" slug=\"same\">"
        "    <proprietas clavis=\"a\" valor=\"1\"/>"
        "  </entitas>"
        "  <entitas genus=\"Page\" slug=\"same\">"
        "    <proprietas clavis=\"b\" valor=\"2\"/>"
        "  </entitas>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    entitas = repo->entitas_scaffoldare(repo->datum, "Page",
        "same");
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
interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
                 Pagina* pagina;
                Entitas* entitas;
                 chorda* valor;
                 chorda* clavis_title;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    dom = layout_creare(ctx,
        "<layout>"
        "  <pagina id=\"editor\" x=0 y=0 latitudo=80 altitudo=40/>"
        "  <entitas genus=\"Page\" slug=\"content\">"
        "    <proprietas clavis=\"title\" valor=\"Test\"/>"
        "  </entitas>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    /* Verificare widget creatum */
    pagina = layout_obtinere_pagina(dom, "editor");
    CREDO_NON_NIHIL(pagina);

    /* Verificare entitas creata */
    entitas = repo->entitas_scaffoldare(repo->datum, "Page",
        "content");
    CREDO_NON_NIHIL(entitas);

    clavis_title  = chorda_internare_ex_literis(intern, "title");
    valor         = entitas_proprietas_capere(entitas, clavis_title);
    CREDO_NON_NIHIL(valor);
    CREDO_CHORDA_AEQUALIS_LITERIS(*valor, "Test");

    imprimere("  Layout mixta: VERUM\n");
}


/* === Entitas Error Cases === */
interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
              LayoutDom* dom;
           Persistentia* persistentia;
    EntitasRepositorium* repo;
        ContextusWidget* ctx;
    InternamentumChorda* intern;

    intern = internamentum_globale();

    persistentia = persistentia_memoria_creare(c->piscina);
    repo = entitas_repositorium_creare(c->piscina, persistentia);
    ctx = contextus_widget_creare(c->piscina, intern, repo, NIHIL,
        NIHIL, NIHIL, NIHIL);

    /* Entitas sine genus - debet ignorari */
    dom = layout_creare(ctx,
        "<layout>"
        "  <entitas slug=\"orphan\"/>"
        "</layout>");

    CREDO_NON_NIHIL(dom);  /* Layout creatur sed entitas non */

    /* Entitas sine slug - debet ignorari */
    dom = layout_creare(ctx,
        "<layout>"
        "  <entitas genus=\"Page\"/>"
        "</layout>");

    CREDO_NON_NIHIL(dom);

    imprimere("  Entitas errores: VERUM\n");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "layout_creare cum pagina simplex", _sectio_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "layout_obtinere_pagina", _sectio_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "layout cum navigator", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "layout cum pagina et navigator", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "layout_obtinere", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "layout sine repositorium", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "attributa defalta", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "layout_entitas_simplex", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "layout_entitas_proprietates", _sectio_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "layout_entitas_notae", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "layout_entitas_relationes", _sectio_decima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "layout_entitas_hierarchicum", _sectio_decima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "layout_entitas_idempotens", _sectio_decima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "layout_mixta", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "layout_entitas_errores", _sectio_decima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("layout", SECTIONES);
}
