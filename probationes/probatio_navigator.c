#include "piscina.h"
#include "graphus_entitatum.h"
#include "navigator_entitatum.h"
#include "fenestra.h"
#include "thema.h"
#include "internamentum.h"
#include <stdio.h>

#define LATITUDO_FENESTRA  853
#define ALTITUDO_FENESTRA  480

/* Creare graphum probationis cum aliquot entitates */
interior vacuum
creare_graphum_probationis(
    GraphusEntitatum* graphus)
{
    InternamentumChorda* intern;
    Entitas*             radix;
    Entitas*             pagina_1;
    Entitas*             pagina_2;
    Entitas*             pagina_3;
    Entitas*             nota;
    chorda*              id;
    chorda*              genus;
    chorda*              clavis;
    chorda*              valor;
    chorda*              rel_genus;
    chorda*              nota_root;

    intern = graphus_entitatum_internamentum(graphus);

    /* === Entitas Radix === */
    id = chorda_internare_ex_literis(intern, "root");
    genus = chorda_internare_ex_literis(intern, "Root");
    radix = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    /* Proprietates radix */
    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "System Root");
    entitas_proprietas_addere(radix, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "description");
    valor = chorda_internare_ex_literis(intern, "Root of the entity graph");
    entitas_proprietas_addere(radix, clavis, valor);

    /* Nota radix */
    nota_root = chorda_internare_ex_literis(intern, "#root");
    entitas_nota_addere(radix, nota_root);

    graphus_entitatum_addere_entitatem(graphus, radix);

    /* === Pagina 1 === */
    id = chorda_internare_ex_literis(intern, "page:001");
    genus = chorda_internare_ex_literis(intern, "Page");
    pagina_1 = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "Introduction");
    entitas_proprietas_addere(pagina_1, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "content");
    valor = chorda_internare_ex_literis(intern, "Welcome to the entity navigator");
    entitas_proprietas_addere(pagina_1, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "author");
    valor = chorda_internare_ex_literis(intern, "Fran");
    entitas_proprietas_addere(pagina_1, clavis, valor);

    graphus_entitatum_addere_entitatem(graphus, pagina_1);

    /* === Pagina 2 === */
    id = chorda_internare_ex_literis(intern, "page:002");
    genus = chorda_internare_ex_literis(intern, "Page");
    pagina_2 = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "Getting Started");
    entitas_proprietas_addere(pagina_2, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "content");
    valor = chorda_internare_ex_literis(intern, "Use j/k to navigate, l to enter, h to go back");
    entitas_proprietas_addere(pagina_2, clavis, valor);

    graphus_entitatum_addere_entitatem(graphus, pagina_2);

    /* === Pagina 3 === */
    id = chorda_internare_ex_literis(intern, "page:003");
    genus = chorda_internare_ex_literis(intern, "Page");
    pagina_3 = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "Navigation");
    entitas_proprietas_addere(pagina_3, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "content");
    valor = chorda_internare_ex_literis(intern, "The graph structure allows bidirectional traversal");
    entitas_proprietas_addere(pagina_3, clavis, valor);

    graphus_entitatum_addere_entitatem(graphus, pagina_3);

    /* === Nota === */
    id = chorda_internare_ex_literis(intern, "note:001");
    genus = chorda_internare_ex_literis(intern, "Note");
    nota = entitas_creare(graphus_entitatum_internamentum(graphus)->piscina, id, genus);

    clavis = chorda_internare_ex_literis(intern, "name");
    valor = chorda_internare_ex_literis(intern, "Important");
    entitas_proprietas_addere(nota, clavis, valor);

    clavis = chorda_internare_ex_literis(intern, "text");
    valor = chorda_internare_ex_literis(intern, "Remember to test thoroughly");
    entitas_proprietas_addere(nota, clavis, valor);

    graphus_entitatum_addere_entitatem(graphus, nota);

    /* === Relationes === */
    /* Radix -> Paginae */
    rel_genus = chorda_internare_ex_literis(intern, "pages");
    entitas_relatio_addere(radix, rel_genus, pagina_1->id);
    entitas_relatio_addere(radix, rel_genus, pagina_2->id);
    entitas_relatio_addere(radix, rel_genus, pagina_3->id);

    /* Radix -> Notae */
    rel_genus = chorda_internare_ex_literis(intern, "notes");
    entitas_relatio_addere(radix, rel_genus, nota->id);

    /* Pagina 1 -> Pagina 2 */
    rel_genus = chorda_internare_ex_literis(intern, "next");
    entitas_relatio_addere(pagina_1, rel_genus, pagina_2->id);

    /* Pagina 2 -> Pagina 1, Pagina 3 */
    rel_genus = chorda_internare_ex_literis(intern, "prev");
    entitas_relatio_addere(pagina_2, rel_genus, pagina_1->id);
    rel_genus = chorda_internare_ex_literis(intern, "next");
    entitas_relatio_addere(pagina_2, rel_genus, pagina_3->id);

    /* Pagina 3 -> Pagina 2 */
    rel_genus = chorda_internare_ex_literis(intern, "prev");
    entitas_relatio_addere(pagina_3, rel_genus, pagina_2->id);

    /* Pagina 1 -> Nota */
    rel_genus = chorda_internare_ex_literis(intern, "references");
    entitas_relatio_addere(pagina_1, rel_genus, nota->id);

    imprimere("Graphus probationis creatus:\n");
    imprimere("  Radix: %d entitates\n", graphus_entitatum_numerus(graphus));
}

int
main(void)
{
    Piscina*             piscina;
    GraphusEntitatum*    graphus;
    EntitasProvidor*     providor;
    NavigatorEntitatum*  navigator;
    Fenestra*            fenestra;
    TabulaPixelorum*     tabula;
    FenestraConfiguratio configuratio;
    Eventus              eventus;
    b32                  currens;

    /* Initiare thema */
    thema_initiare();

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("navigator", M * M);  /* 1MB */
    si (!piscina)
    {
        imprimere("Fractura: non potest creare piscinam\n");
        redde I;
    }

    /* Creare graphum */
    graphus = graphus_entitatum_creare(piscina);
    si (!graphus)
    {
        imprimere("Fractura: non potest creare graphum\n");
        redde I;
    }

    /* Creare entitates probationis */
    creare_graphum_probationis(graphus);

    /* Creare providor */
    providor = graphus_entitatum_providor_creare(graphus);
    si (!providor)
    {
        imprimere("Fractura: non potest creare providor\n");
        redde I;
    }

    /* Creare navigator */
    navigator = navigator_entitatum_creare(piscina, providor);
    si (!navigator)
    {
        imprimere("Fractura: non potest creare navigator\n");
        redde I;
    }

    /* Configurare fenestram */
    configuratio.titulus = "Navigator Entitatum - Probatio";
    configuratio.x = C;
    configuratio.y = C;
    configuratio.latitudo = LATITUDO_FENESTRA;
    configuratio.altitudo = ALTITUDO_FENESTRA;
    configuratio.vexilla = FENESTRA_ORDINARIA;

    /* Creare fenestram */
    fenestra = fenestra_creare(piscina, &configuratio);
    si (!fenestra)
    {
        imprimere("Fractura: non potest creare fenestram\n");
        redde I;
    }

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, ALTITUDO_FENESTRA);
    si (!tabula)
    {
        imprimere("Fractura: non potest creare tabulam\n");
        fenestra_destruere(fenestra);
        redde I;
    }

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    /* Cyclus principalis */
    currens = VERUM;

    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        /* Perscrutari eventus */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            si (eventus.genus == EVENTUS_CLAUDERE)
            {
                currens = FALSUM;
            }
            alioquin si (eventus.genus == EVENTUS_CLAVIS_DEPRESSUS)
            {
                /* ESC vel q = exire */
                si (eventus.datum.clavis.typus == '\x1B' || eventus.datum.clavis.typus == 'q')
                {
                    currens = FALSUM;
                }
                alioquin
                {
                    /* Passare ad navigator */
                    navigator_entitatum_tractare_eventum(navigator, &eventus);
                }
            }
        }

        /* Vacare fondum */
        tabula_pixelorum_vacare(tabula, color_ad_pixelum(thema_color(COLOR_BACKGROUND)));

        /* Reddere navigator */
        navigator_entitatum_reddere(
            navigator,
            tabula,
            ZEPHYRUM,      /* x */
            ZEPHYRUM,      /* y */
            CXLII,         /* latitudo (142 chars) */
            LX,            /* altitudo (60 lines) */
            I,             /* scala */
            VERUM);        /* focused */

        /* Praesentare pixela */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Purgare */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
