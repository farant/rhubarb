/* Probatio pro fenestra et tabula pixelorum */
#include "latina.h"
#include "fenestra.h"
#include <stdio.h>

int
main (
    void)
{
    FenestraConfiguratio configuratio;
    Fenestra* fenestra;
    TabulaPixelorum* tabula;
    Eventus eventus;
    b32 currens;

    /* Configurare fenestram */
    configuratio.titulus = "Probatio Fenestrae";
    configuratio.x = C;
    configuratio.y = C;
    configuratio.latitudo = DCCC;
    configuratio.altitudo = DC;
    configuratio.vexilla = FENESTRA_ORDINARIA;

    /* Creare fenestram */
    fenestra = fenestra_creare(&configuratio);
    si (fenestra == NIHIL) {
        imprimere("Errore: non possum creare fenestram\n");
        return 1;
    }

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(fenestra, CDLXXX);
    si (tabula == NIHIL) {
        imprimere("Errore: non possum creare tabulam pixelorum\n");
        fenestra_destruere(fenestra);
        return 1;
    }

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    /* Ansa eventuum */
    currens = VERUM;
    dum (currens && !fenestra_debet_claudere(fenestra)) {
        /* Perscrutari eventus ex systemate */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        dum (fenestra_obtinere_eventus(fenestra, &eventus)) {
            commutatio (eventus.genus) {
                casus EVENTUS_CLAUDERE:
                    currens = FALSUM;
                    frange;

                casus EVENTUS_CLAVIS_DEPRESSUS:
                    si (eventus.datum.clavis.clavis == CLAVIS_EFFUGIUM) {
                        currens = FALSUM;
                    }
                    frange;

                ordinarius:
                    frange;
            }
        }

        /* Purgare tabulam pixelorum (color caeruleus obscurus) */
        tabula_pixelorum_vacare(tabula, RGB(XXVI, XXVI, XLVI));

        /* Pingere textum */
        tabula_pixelorum_pingere_chordam(tabula, X, X,
            "Salve Mundi!\n\nPremere EFFUGIUM ut claudas.",
            RGB(CCXXXVIII, CCXXXVIII, CCXXXVIII));

        /* Praesentare pixela ad fenestram */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Delere tabulam pixelorum et fenestram */
    fenestra_destruere_tabulam_pixelorum(tabula);
    fenestra_destruere(fenestra);

    redde ZEPHYRUM;
}
