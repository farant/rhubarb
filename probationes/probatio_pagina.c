/* Probatio pro pagina - editor textus */
#include "latina.h"
#include "fenestra.h"
#include "pagina.h"
#include "piscina.h"
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
    pagina_initiare(&pagina, "page:test");

    /* Inserere textum initialem */
    pagina_inserere_chordam(&pagina, "Salve! Hoc est probatio paginae.\n");
    pagina_inserere_chordam(&pagina, "Preme EFFUGIUM ut claudas.\n\n");
    pagina_inserere_chordam(&pagina, "Scribe characteres...\n");

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    imprimere("Probatio Pagina\n");
    imprimere("Scribe textum\n");
    imprimere("Sagittae pro movere cursorem\n");
    imprimere("Retrorsum pro delere\n");
    imprimere("Premere EFFUGIUM ut claudas\n\n");

    /* Ansa eventuum */
    currens = VERUM;
    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        /* Perscrutari eventus ex systemate */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            commutatio (eventus.genus)
            {
                casus EVENTUS_CLAUDERE:
                    currens = FALSUM;
                    frange;

                casus EVENTUS_CLAVIS_DEPRESSUS:
                {
                    clavis_t clavis;
                    i32 modificantes;

                    clavis = eventus.datum.clavis.clavis;
                    modificantes = eventus.datum.clavis.modificantes;

                    si (clavis == CLAVIS_EFFUGIUM)
                    {
                        currens = FALSUM;
                    }
                    alioquin si (clavis == CLAVIS_RETRORSUM)
                    {
                        pagina_delere_characterem(&pagina);
                    }
                    alioquin si (clavis == CLAVIS_DELERE)
                    {
                        pagina_delere_characterem_ante(&pagina);
                    }
                    alioquin si (clavis == CLAVIS_SINISTER)
                    {
                        pagina_movere_cursor_sinistram(&pagina);
                    }
                    alioquin si (clavis == CLAVIS_DEXTER)
                    {
                        pagina_movere_cursor_dextram(&pagina);
                    }
                    alioquin si (clavis == CLAVIS_SURSUM)
                    {
                        pagina_movere_cursor_sursum(&pagina);
                    }
                    alioquin si (clavis == CLAVIS_DEORSUM)
                    {
                        pagina_movere_cursor_deorsum(&pagina);
                    }
                    alioquin si (clavis == CLAVIS_DOMUS)
                    {
                        pagina_movere_cursor_domus(&pagina);
                    }
                    alioquin si (clavis == CLAVIS_FINIS)
                    {
                        pagina_movere_cursor_finis(&pagina);
                    }
                    alioquin si (clavis == CLAVIS_REDITUS)
                    {
                        pagina_inserere_characterem(&pagina, '\n');
                    }
                    alioquin si (clavis >= XXXII && clavis <= CXXVI)
                    {
                        character c;

                        c = (character)clavis;

                        /* Claves litterarum semper veniunt ut maiusculae, convertere si necesse */
                        si (c >= 'A' && c <= 'Z')
                        {
                            si (!(modificantes & MOD_SHIFT))
                            {
                                c = (character)(c + XXXII);  /* Convertere ad minusculam */
                            }
                        }
                        /* Numerorum et punctuationis cum shift */
                        alioquin si (modificantes & MOD_SHIFT)
                        {
                            commutatio (c)
                            {
                                casus '1': c = '!'; frange;
                                casus '2': c = '@'; frange;
                                casus '3': c = '#'; frange;
                                casus '4': c = '$'; frange;
                                casus '5': c = '%'; frange;
                                casus '6': c = '^'; frange;
                                casus '7': c = '&'; frange;
                                casus '8': c = '*'; frange;
                                casus '9': c = '('; frange;
                                casus '0': c = ')'; frange;
                                casus '-': c = '_'; frange;
                                casus '=': c = '+'; frange;
                                casus '[': c = '{'; frange;
                                casus ']': c = '}'; frange;
                                casus '\\': c = '|'; frange;
                                casus ';': c = ':'; frange;
                                casus '\'': c = '"'; frange;
                                casus ',': c = '<'; frange;
                                casus '.': c = '>'; frange;
                                casus '/': c = '?'; frange;
                                casus '`': c = '~'; frange;
                                ordinarius: frange;
                            }
                        }

                        pagina_inserere_characterem(&pagina, c);
                    }

                    frange;
                }

                ordinarius:
                    frange;
            }
        }

        /* Purgare tabulam pixelorum */
        tabula_pixelorum_vacare(tabula, RGB(XX, XX, XXX));

        /* Reddere paginam */
        pagina_reddere(tabula, &pagina, X, X, LXX, LV, I);

        /* Praesentare pixela ad fenestram */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Delere fenestram et piscinam */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
