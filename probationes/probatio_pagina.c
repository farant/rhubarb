/* Probatio pro pagina - editor textus */
#include "latina.h"
#include "fenestra.h"
#include "pagina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "tempus.h"
#include <stdio.h>

int
main (
    void)
{
                 Piscina* piscina;
    FenestraConfiguratio  configuratio;
                Fenestra* fenestra;
         TabulaPixelorum* tabula;
                 Eventus  eventus;
                     b32  currens;
                  Pagina  pagina;

    /* Initiare thema */
    thema_initiare();

    /* Initiare tempus */
    tempus_initiare();

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("pagina", M * M);
    si (piscina == NIHIL)
    {
        imprimere("Errore: non possum creare piscinam\n");
        return 1;
    }

    /* Configurare fenestram */
    configuratio.titulus  = "Probatio Pagina";
    configuratio.x        = C;
    configuratio.y        = C;
    configuratio.latitudo = DCCC;
    configuratio.altitudo = CDLXXX;
    configuratio.vexilla  = FENESTRA_ORDINARIA;

    /* Creare fenestram */
    fenestra = fenestra_creare(piscina, &configuratio);
    si (fenestra == NIHIL)
    {
        imprimere("Errore: non possum creare fenestram\n");
        piscina_destruere(piscina);
        return 1;
    }

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
    si (tabula == NIHIL)
    {
        imprimere("Errore: non possum creare tabulam pixelorum\n");
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        return 1;
    }

    /* Initiare paginam */
    pagina_initiare(&pagina, chorda_internare_ex_literis(internamentum_globale(), "Page::Test"));

    /* Inserere textum initialem */
    pagina_inserere_chordam(&pagina, "Salve! Hoc est probatio paginae.\n");
    pagina_inserere_chordam(&pagina, "Preme EFFUGIUM ut claudas.\n\n");
    pagina_inserere_chordam(&pagina, "i = insert mode, ESC = normal mode\n");
    pagina_inserere_chordam(&pagina, "Normal: hjkl, wb, o/O, a/i, g/G, dd, dG, d$\n");

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    imprimere("Probatio Pagina - Vim Mode\n");
    imprimere("Normal mode: hjkl navigation, w/b word nav, o/O new line, a/i insert\n");
    imprimere("Insert mode: ESC to normal\n");
    imprimere("Commands: g/G jump, dd/dG/d$ delete\n");
    imprimere("Premere EFFUGIUM ut claudas\n\n");

    /* Ansa eventuum */
    currens = VERUM;
    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        /* Actualizare tempus */
        tempus_quadrum();

        /* Perscrutari eventus ex systemate */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            si (eventus.genus == EVENTUS_CLAUDERE)
            {
                currens = FALSUM;
            }
            alioquin
            {
                /* Tractare eventus per pagina (vim mode) */
                si (!pagina_tractare_eventum(&pagina, &eventus))
                {
                    currens = FALSUM;  /* ESC in normal mode = quit */
                }
            }
        }

        /* Purgare tabulam pixelorum */
        tabula_pixelorum_vacare(tabula, color_ad_pixelum(thema_color(COLOR_BACKGROUND)));

        /* Reddere paginam cum margine (border + status) */
        pagina_reddere_cum_margine(piscina, tabula, &pagina, ZEPHYRUM, ZEPHYRUM, LXX, XL, I, VERUM);

        /* Praesentare pixela ad fenestram */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Delere fenestram et piscinam */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
