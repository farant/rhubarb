/* Probatio pro imago library */
#include "latina.h"
#include "fenestra.h"
#include "imago.h"
#include "imago_opus.h"
#include "thema.h"
#include "color.h"
#include <stdio.h>

s32
principale(s32 argc, character** argv)
{
                 Piscina* piscina;
    FenestraConfiguratio  configuratio;
                Fenestra* fenestra;
         TabulaPixelorum* tabula;
                 Eventus  eventus;
                     b32  currens;
            ImagoFructus  imago_fructus;
                  Imago*  imago;
                   Imago  imago_scalata;
                  Imago*  imago_visendum;
                     s32  x, y;
                     s32  dest_x, dest_y;

    /* Requirere via ad imaginem */
    si (argc < 2)
    {
        fprintf(stderr, "Usus: %s <via_ad_imaginem>\n", argv[0]);
        fprintf(stderr, "Exemplum: %s ~/Pictures/test.png\n", argv[0]);
        redde 1;
    }

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("imago", M * M * X);  /* 10MB */
    si (piscina == NIHIL)
    {
        fprintf(stderr, "Errore: non possum creare piscinam\n");
        redde 1;
    }

    /* Caricare imaginem */
    fprintf(stderr, "Caricare: %s\n", argv[1]);
    imago_fructus = imago_caricare_ex_file(argv[1], piscina);
    si (!imago_fructus.successus)
    {
        fprintf(stderr, "Errore: %.*s\n",
            (integer)imago_fructus.error.mensura,
            imago_fructus.error.datum);
        piscina_destruere(piscina);
        redde 1;
    }

    imago = &imago_fructus.imago;
    fprintf(stderr, "Imago carricata: %u x %u pixela\n",
        imago->latitudo, imago->altitudo);

    /* Initiare thema */
    thema_initiare();

    /* Configurare fenestram - magna sed non plena visio */
    configuratio.titulus  = "Probatio Imago";
    configuratio.x        = 0;
    configuratio.y        = 0;
    configuratio.latitudo = 1600;
    configuratio.altitudo = 1000;
    configuratio.vexilla  = FENESTRA_ORDINARIA | FENESTRA_CENTRATA;

    /* Creare fenestram */
    fenestra = fenestra_creare(piscina, &configuratio);
    si (fenestra == NIHIL)
    {
        fprintf(stderr, "Errore: non possum creare fenestram\n");
        piscina_destruere(piscina);
        redde 1;
    }

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CCCXX);
    si (tabula == NIHIL)
    {
        fprintf(stderr, "Errore: non possum creare tabulam pixelorum\n");
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        redde 1;
    }

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    /* Scalare imaginem ad tabulam (post creationem ut habeamus dimensiones) */
    imago_scalata = imago_scalare_ad_limites(
        imago,
        (i32)tabula->latitudo,
        (i32)tabula->altitudo,
        IMAGO_SCALA_BILINEARIS,
        piscina);

    si (imago_scalata.pixela != NIHIL)
    {
        imago_visendum = &imago_scalata;
        fprintf(stderr, "Imago scalata: %u x %u ad %u x %u\n",
            imago->latitudo, imago->altitudo,
            imago_scalata.latitudo, imago_scalata.altitudo);
    }
    alioquin
    {
        imago_visendum = imago;
        fprintf(stderr, "Non possum scalare, monstrare originalem\n");
    }

    /* Ansa eventuum */
    currens = VERUM;
    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        /* Perscrutari eventus */
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
                    si (eventus.datum.clavis.clavis == CLAVIS_EFFUGIUM)
                    {
                        currens = FALSUM;
                    }
                    frange;

                ordinarius:
                    frange;
            }
        }

        /* Purgare tabulam cum colore background */
        tabula_pixelorum_vacare(tabula, color_ad_pixelum(thema_color(COLOR_BACKGROUND)));

        /* Centrare imaginem in fenestra */
        dest_x = ((s32)tabula->latitudo - (s32)imago_visendum->latitudo) / 2;
        dest_y = ((s32)tabula->altitudo - (s32)imago_visendum->altitudo) / 2;
        si (dest_x < 0) dest_x = 0;
        si (dest_y < 0) dest_y = 0;

        /* Copiare pixela imaginis ad tabulam */
        per (y = 0; y < (s32)imago_visendum->altitudo && dest_y + y < (s32)tabula->altitudo; y++)
        {
            per (x = 0; x < (s32)imago_visendum->latitudo && dest_x + x < (s32)tabula->latitudo; x++)
            {
                /* Skip si extra bounds */
                si (dest_x + x < 0 || dest_y + y < 0)
                {
                    perge;
                }

                /* Obtinere pixel ex imagine (RGBA format) */
                {
                    s32 src_idx = (y * (s32)imago_visendum->latitudo + x) * 4;
                    s32 dest_idx = (dest_y + y) * (s32)tabula->latitudo + (dest_x + x);
                    insignatus character r = (insignatus character)imago_visendum->pixela[src_idx + 0];
                    insignatus character g = (insignatus character)imago_visendum->pixela[src_idx + 1];
                    insignatus character b = (insignatus character)imago_visendum->pixela[src_idx + 2];

                    /* Scribere ad tabulam */
                    tabula->pixela[dest_idx] = (i32)RGB(r, g, b);
                }
            }
        }

        /* Praesentare */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Cleanup */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    fprintf(stderr, "Finis.\n");
    redde ZEPHYRUM;
}
