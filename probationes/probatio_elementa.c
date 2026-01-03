/* Probatio pro elementa GUI components */
#include "latina.h"
#include "fenestra.h"
#include "elementa.h"
#include "delineare.h"
#include "thema.h"
#include "color.h"
#include <stdio.h>

s32
principale(vacuum)
{
                 Piscina* piscina;
    FenestraConfiguratio  configuratio;
                Fenestra* fenestra;
         TabulaPixelorum* tabula;
                 Eventus  eventus;
                 Eventus* eventus_ptr;
                     b32  currens;
                  chorda  label_bottone;
                  chorda  label_capsa;
                  chorda  textus_input;

    /* Status componenta */
    i32 slider_valor;
    b32 capsa_valor;
    s32 cursor_pos;
    b32 text_focused;
    b32 colores_activi[16];
    i32 i;

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("elementa", M * M);
    si (piscina == NIHIL)
    {
        imprimere("Errore: non possum creare piscinam\n");
        redde 1;
    }

    /* Initiare thema */
    thema_initiare();

    /* Configurare fenestram */
    configuratio.titulus  = "Probatio Elementa";
    configuratio.x        = C;
    configuratio.y        = C;
    configuratio.latitudo = DCCC;
    configuratio.altitudo = DC;
    configuratio.vexilla  = FENESTRA_ORDINARIA;

    /* Creare fenestram */
    fenestra = fenestra_creare(piscina, &configuratio);
    si (fenestra == NIHIL)
    {
        imprimere("Errore: non possum creare fenestram\n");
        piscina_destruere(piscina);
        redde 1;
    }

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);
    si (tabula == NIHIL)
    {
        imprimere("Errore: non possum creare tabulam pixelorum\n");
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        redde 1;
    }

    /* Initiare status componenta */
    slider_valor = 50;
    capsa_valor = FALSUM;
    cursor_pos = 0;
    text_focused = FALSUM;
    per (i = 0; i < 16; i++)
    {
        colores_activi[i] = VERUM;
    }

    /* Creare labels */
    label_bottone = chorda_ex_literis("Pulsus", piscina);
    label_capsa = chorda_ex_literis("Optio", piscina);
    textus_input = chorda_ex_literis("Textus", piscina);

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    /* Ansa eventuum */
    currens = VERUM;
    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        FructusSlider slider_fructus;
        FructusBottone bottone_fructus;
        FructusCapsaOptandi capsa_fructus;
        FructusCampusTextus campus_fructus;
        FructusGraticulaColorum graticula_fructus;
        chorda titulus;

        /* Perscrutari eventus ex systemate */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        eventus_ptr = NIHIL;
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            commutatio (eventus.genus)
            {
                casus EVENTUS_CLAUDERE:
                    currens = FALSUM;
                    frange;

                casus EVENTUS_CLAVIS_DEPRESSUS:
                    si (eventus.datum.clavis.clavis == CLAVIS_EFFUGIUM)
                    {
                        currens = FALSUM;
                    }
                    eventus_ptr = &eventus;
                    frange;

                casus EVENTUS_MUS_DEPRESSUS:
                casus EVENTUS_MUS_LIBERATUS:
                casus EVENTUS_MUS_MOTUS:
                    eventus_ptr = &eventus;
                    frange;

                ordinarius:
                    frange;
            }
        }

        /* Purgare tabulam */
        tabula_pixelorum_vacare(tabula, color_ad_pixelum(thema_color(COLOR_BACKGROUND)));

        /* Titulus */
        titulus = chorda_ex_literis("ELEMENTA GUI TEST", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, 10, 10, titulus,
            color_ad_pixelum(thema_color(COLOR_TEXT)), 2);

        /* Slider */
        titulus = chorda_ex_literis("Slider:", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, 10, 60, titulus,
            color_ad_pixelum(thema_color(COLOR_TEXT)), 1);
        slider_fructus = elementa_slider(tabula, piscina, 15, 9, 20, slider_valor, 0, 100, eventus_ptr, 1.0f);
        si (slider_fructus.mutatum)
        {
            slider_valor = slider_fructus.valor;
            fprintf(stderr, "Slider valor: %u\n", slider_valor);
        }

        /* Bottone */
        titulus = chorda_ex_literis("Bottone:", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, 10, 100, titulus,
            color_ad_pixelum(thema_color(COLOR_TEXT)), 1);
        bottone_fructus = elementa_bottone(tabula, piscina, 15, 14, &label_bottone, eventus_ptr, 1.0f);
        si (bottone_fructus.clicked)
        {
            fprintf(stderr, "Bottone pulsatus!\n");
        }

        /* Capsa optandi */
        titulus = chorda_ex_literis("Checkbox:", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, 10, 150, titulus,
            color_ad_pixelum(thema_color(COLOR_TEXT)), 1);
        capsa_fructus = elementa_capsa_optandi(tabula, piscina, 15, 20, &label_capsa, capsa_valor, eventus_ptr, 1.0f);
        si (capsa_fructus.mutatum)
        {
            capsa_valor = capsa_fructus.valor;
            fprintf(stderr, "Capsa valor: %d\n", capsa_valor);
        }

        /* Campus textus */
        titulus = chorda_ex_literis("Text Input:", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, 10, 200, titulus,
            color_ad_pixelum(thema_color(COLOR_TEXT)), 1);
        campus_fructus = elementa_campus_textus(tabula, piscina, 15, 26, 15, &textus_input, cursor_pos, text_focused, eventus_ptr, 1.0f);
        text_focused = campus_fructus.focused;
        cursor_pos = campus_fructus.cursor;
        si (campus_fructus.mutatum && campus_fructus.textus != NIHIL)
        {
            textus_input = *campus_fructus.textus;
        }

        /* Graticula colorum */
        titulus = chorda_ex_literis("Palette:", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, 10, 250, titulus,
            color_ad_pixelum(thema_color(COLOR_TEXT)), 1);
        graticula_fructus = elementa_graticula_colorum(tabula, piscina, 15, 33, colores_activi, eventus_ptr, 1.0f);
        si (graticula_fructus.mutatum)
        {
            per (i = 0; i < 16; i++)
            {
                colores_activi[i] = graticula_fructus.colores[i];
            }
            fprintf(stderr, "Color toggled: %d\n", graticula_fructus.toggled_index);
        }

        /* Status info */
        titulus = chorda_ex_literis("ESC = claudere", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, 10, 350, titulus,
            color_ad_pixelum(thema_color(COLOR_TEXT_DIM)), 1);

        /* Praesentare pixela ad fenestram */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Delere fenestram et piscinam */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
