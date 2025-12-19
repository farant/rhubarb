#include "piscina.h"
#include "persistentia.h"
#include "entitas_repositorium.h"
#include "navigator_entitatum.h"
#include "fenestra.h"
#include "thema.h"
#include "internamentum.h"
#include "widget.h"
#include <stdio.h>

#define LATITUDO_FENESTRA  853
#define ALTITUDO_FENESTRA  480

/* Creare graphum probationis cum aliquot entitates */
interior vacuum
creare_graphum_probationis(
    EntitasRepositorium* repo)
{
    Entitas* radix;
    Entitas* pagina_1;
    Entitas* pagina_2;
    Entitas* pagina_3;
    Entitas* nota;

    /* === Entitas Radix === */
    radix = repo->entitas_scaffoldare(repo->datum, "Root", "system");
    repo->proprietas_ponere(repo->datum, radix, "name", "System Root");
    repo->proprietas_ponere(repo->datum, radix, "description", "Root of the entity graph");
    repo->nota_addere(repo->datum, radix, "#root");

    /* === Pagina 1 === */
    pagina_1 = repo->entitas_scaffoldare(repo->datum, "Page", "introduction");
    repo->proprietas_ponere(repo->datum, pagina_1, "name", "Introduction");
    repo->proprietas_ponere(repo->datum, pagina_1, "content", "Welcome to the entity navigator");
    repo->proprietas_ponere(repo->datum, pagina_1, "author", "Fran");

    /* === Pagina 2 === */
    pagina_2 = repo->entitas_scaffoldare(repo->datum, "Page", "getting-started");
    repo->proprietas_ponere(repo->datum, pagina_2, "name", "Getting Started");
    repo->proprietas_ponere(repo->datum, pagina_2, "content", "Use j/k to navigate, l to enter, h to go back");

    /* === Pagina 3 === */
    pagina_3 = repo->entitas_scaffoldare(repo->datum, "Page", "navigation");
    repo->proprietas_ponere(repo->datum, pagina_3, "name", "Navigation");
    repo->proprietas_ponere(repo->datum, pagina_3, "content", "The graph structure allows bidirectional traversal");

    /* === Nota === */
    nota = repo->entitas_scaffoldare(repo->datum, "Note", "important");
    repo->proprietas_ponere(repo->datum, nota, "name", "Important");
    repo->proprietas_ponere(repo->datum, nota, "text", "Remember to test thoroughly");

    /* === Relationes === */
    /* Radix -> Paginae */
    repo->relatio_addere(repo->datum, radix, "pages", pagina_1->id);
    repo->relatio_addere(repo->datum, radix, "pages", pagina_2->id);
    repo->relatio_addere(repo->datum, radix, "pages", pagina_3->id);

    /* Radix -> Notae */
    repo->relatio_addere(repo->datum, radix, "notes", nota->id);

    /* Pagina 1 -> Pagina 2 */
    repo->relatio_addere(repo->datum, pagina_1, "next", pagina_2->id);

    /* Pagina 2 -> Pagina 1, Pagina 3 */
    repo->relatio_addere(repo->datum, pagina_2, "prev", pagina_1->id);
    repo->relatio_addere(repo->datum, pagina_2, "next", pagina_3->id);

    /* Pagina 3 -> Pagina 2 */
    repo->relatio_addere(repo->datum, pagina_3, "prev", pagina_2->id);

    /* Pagina 1 -> Nota */
    repo->relatio_addere(repo->datum, pagina_1, "references", nota->id);

    imprimere("Graphus probationis creatus\n");
}

int
main(void)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    Persistentia*        persistentia;
    EntitasRepositorium* repositorium;
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

    /* Creare internamentum */
    intern = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("Fractura: non potest creare internamentum\n");
        redde I;
    }

    /* Creare persistentia (in memoria) */
    persistentia = persistentia_memoria_creare(piscina);
    si (!persistentia)
    {
        imprimere("Fractura: non potest creare persistentiam\n");
        redde I;
    }

    /* Creare repositorium */
    repositorium = entitas_repositorium_creare(piscina, persistentia);
    si (!repositorium)
    {
        imprimere("Fractura: non potest creare repositorium\n");
        redde I;
    }

    /* Creare entitates probationis */
    creare_graphum_probationis(repositorium);

    /* Creare contextum et navigator */
    {
        ContextusWidget* ctx;

        ctx = contextus_widget_creare(piscina, intern, repositorium, NIHIL, NIHIL);
        si (!ctx)
        {
            imprimere("Fractura: non potest creare contextum\n");
            redde I;
        }

        navigator = navigator_entitatum_creare(ctx);
    }
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
