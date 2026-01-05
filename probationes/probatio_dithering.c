/* Probatio pro dithering library */
#include "latina.h"
#include "fenestra.h"
#include "imago.h"
#include "imago_opus.h"
#include "dithering.h"
#include "elementa.h"
#include "thema.h"
#include "color.h"
#include "clipboard_platform.h"
#include "importatio_visus.h"
#include "dialogus_importatio.h"
#include "dialogus.h"
#include <stdio.h>

/*
 * Presets:
 * 1 = Omnes (all 16)
 * 2 = Griseum (grayscale)
 * 3 = Calidus (warm)
 * 4 = Frigidus (cool)
 * 5 = Monochromaticus (black & white)
 * C = Color mode
 * G = Grayscale bucketing mode
 * V = Paste from clipboard
 * D = Open import dialog (when image loaded)
 */

s32
principale(s32 argc, character** argv)
{
                 Piscina* piscina;
    FenestraConfiguratio  configuratio;
                Fenestra* fenestra;
         TabulaPixelorum* tabula;
                 Eventus  eventus;
                 Eventus* eventus_ptr;
                     b32  currens;
            ImagoFructus  imago_fructus;
                  Imago*  imago;
                   Imago  imago_scalata;
                     b32  colores_activi[XVI];
                     b32  modus_coloris;  /* VERUM = color, FALSUM = griseum */
                     i32  puncta[III];
        DitheringFructus  dithering_fructus;
                     i8*  rgba_dithered;
                     b32  necesse_redithering;
                     b32  necesse_rescalare;
                     b32  habet_imaginem;
                     s32  x, y;
      DialogusImportatio*  dialogus_importatio;
          ImportatioVisus*  importatio_visus;
                     b32  dialogus_activus;

    /* Monstrare usus */
    fprintf(stderr, "Usus: %s [via_ad_imaginem]\n", argv[0]);
    fprintf(stderr, "Claves:\n");
    fprintf(stderr, "  1 = Omnes colores\n");
    fprintf(stderr, "  2 = Griseum\n");
    fprintf(stderr, "  3 = Calidus\n");
    fprintf(stderr, "  4 = Frigidus\n");
    fprintf(stderr, "  5 = Monochromaticus\n");
    fprintf(stderr, "  C = Modus coloris\n");
    fprintf(stderr, "  G = Modus griseus\n");
    fprintf(stderr, "  V = Paste ex clipboard\n");
    fprintf(stderr, "  D = Aperire dialogum importationis\n");
    fprintf(stderr, "  Esc = Exire\n");

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("dithering", M * M * L);  /* 50MB */
    si (piscina == NIHIL)
    {
        fprintf(stderr, "Errore: non possum creare piscinam\n");
        redde 1;
    }

    /* Initiare status imaginis */
    habet_imaginem = FALSUM;
    imago = NIHIL;
    imago_scalata.pixela = NIHIL;
    imago_scalata.latitudo = 0;
    imago_scalata.altitudo = 0;
    rgba_dithered = NIHIL;
    necesse_rescalare = FALSUM;

    /* Caricare imaginem si argumentum datum */
    si (argc >= 2)
    {
        fprintf(stderr, "Caricare: %s\n", argv[1]);
        imago_fructus = imago_caricare_ex_file(argv[1], piscina);
        si (imago_fructus.successus)
        {
            imago = &imago_fructus.imago;
            habet_imaginem = VERUM;
            necesse_rescalare = VERUM;
            fprintf(stderr, "Imago carricata: %u x %u pixela\n",
                imago->latitudo, imago->altitudo);
        }
        alioquin
        {
            fprintf(stderr, "Errore: %.*s\n",
                (integer)imago_fructus.error.mensura,
                imago_fructus.error.datum);
        }
    }
    alioquin
    {
        fprintf(stderr, "Nulla imago. Preme V pro paste ex clipboard.\n");
    }

    /* Initiare thema */
    thema_initiare();

    /* Configurare fenestram (same size as probatio_combinado) */
    configuratio.titulus  = "Probatio Dithering";
    configuratio.x        = 0;
    configuratio.y        = 0;
    configuratio.latitudo = DCCCLIII;  /* 853 */
    configuratio.altitudo = CDLXXX;    /* 480 */
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
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, CDLXXX);  /* 480 - 1:1 */
    si (tabula == NIHIL)
    {
        fprintf(stderr, "Errore: non possum creare tabulam pixelorum\n");
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        redde 1;
    }

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    /* Initiare colores (omnes) */
    dithering_praeparare_omnes(colores_activi);
    modus_coloris = VERUM;
    puncta[0] = 64;
    puncta[1] = 128;
    puncta[2] = 192;
    necesse_redithering = FALSUM;

    /* Initiare dithering fructus */
    dithering_fructus.successus = FALSUM;
    dithering_fructus.indices = NIHIL;

    /* Initiare dialog state */
    dialogus_importatio = NIHIL;
    importatio_visus = NIHIL;
    dialogus_activus = FALSUM;

    /* Ansa eventuum */
    currens = VERUM;
    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        /* Perscrutari eventus */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        eventus_ptr = NIHIL;
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            /* Si dialogus activus, tractare eventus per dialogum */
            si (dialogus_activus && dialogus_importatio != NIHIL)
            {
                Dialogus* dlg;
                DialogusFructus fructus;

                dlg = dialogus_importatio_obtinere_dialogum(dialogus_importatio);
                si (dlg != NIHIL && dlg->tractare_eventum != NIHIL)
                {
                    fructus = dlg->tractare_eventum(dlg, &eventus);

                    si (fructus == DIALOGUS_CONFIRMATUS)
                    {
                        constans i8* indices;
                        i32 lat, alt;
                        chorda tit;

                        si (dialogus_importatio_obtinere_fructum(
                                dialogus_importatio, &indices, &lat, &alt, &tit))
                        {
                            fprintf(stderr, "Imago salvata: %.*s (%d x %d)\n",
                                (integer)tit.mensura, tit.datum, lat, alt);
                        }

                        dialogus_activus = FALSUM;
                        dialogus_importatio = NIHIL;
                        importatio_visus = NIHIL;
                    }
                    alioquin si (fructus == DIALOGUS_ABORTUS)
                    {
                        fprintf(stderr, "Dialogus abortus\n");
                        dialogus_activus = FALSUM;
                        dialogus_importatio = NIHIL;
                        importatio_visus = NIHIL;
                    }
                }
                perge;  /* Non tractare eventus per normale via */
            }

            commutatio (eventus.genus)
            {
                casus EVENTUS_CLAUDERE:
                    currens = FALSUM;
                    frange;

                casus EVENTUS_CLAVIS_DEPRESSUS:
                    {
                        s32 k = (s32)eventus.datum.clavis.clavis;

                        si (k == CLAVIS_EFFUGIUM)
                        {
                            currens = FALSUM;
                        }
                        alioquin si (k == '1')
                        {
                            dithering_praeparare_omnes(colores_activi);
                            necesse_redithering = VERUM;
                            fprintf(stderr, "Preset: Omnes\n");
                        }
                        alioquin si (k == '2')
                        {
                            dithering_praeparare_griseum(colores_activi);
                            necesse_redithering = VERUM;
                            fprintf(stderr, "Preset: Griseum\n");
                        }
                        alioquin si (k == '3')
                        {
                            dithering_praeparare_calidus(colores_activi);
                            necesse_redithering = VERUM;
                            fprintf(stderr, "Preset: Calidus\n");
                        }
                        alioquin si (k == '4')
                        {
                            dithering_praeparare_frigidus(colores_activi);
                            necesse_redithering = VERUM;
                            fprintf(stderr, "Preset: Frigidus\n");
                        }
                        alioquin si (k == '5')
                        {
                            dithering_praeparare_monochromaticus(colores_activi);
                            necesse_redithering = VERUM;
                            fprintf(stderr, "Preset: Monochromaticus\n");
                        }
                        alioquin si (k == 'c' || k == 'C')
                        {
                            modus_coloris = VERUM;
                            necesse_redithering = VERUM;
                            fprintf(stderr, "Modus: Coloris\n");
                        }
                        alioquin si (k == 'g' || k == 'G')
                        {
                            modus_coloris = FALSUM;
                            necesse_redithering = VERUM;
                            fprintf(stderr, "Modus: Griseus\n");
                        }
                        alioquin si (k == 'v' || k == 'V')
                        {
                            /* Paste ex clipboard */
                            si (clipboard_habet_imaginem())
                            {
                                ImagoFructus clipboard_fructus;
                                clipboard_fructus = clipboard_capere_imaginem(piscina);
                                si (clipboard_fructus.successus)
                                {
                                    imago_fructus = clipboard_fructus;
                                    imago = &imago_fructus.imago;
                                    habet_imaginem = VERUM;
                                    necesse_rescalare = VERUM;
                                    fprintf(stderr, "Clipboard: %u x %u pixela\n",
                                        imago->latitudo, imago->altitudo);
                                }
                                alioquin
                                {
                                    fprintf(stderr, "Errore clipboard: %.*s\n",
                                        (integer)clipboard_fructus.error.mensura,
                                        clipboard_fructus.error.datum);
                                }
                            }
                            alioquin
                            {
                                fprintf(stderr, "Nulla imago in clipboard\n");
                            }
                        }
                        alioquin si ((k == 'd' || k == 'D') && habet_imaginem && !dialogus_activus)
                        {
                            /* Aperire dialogum importationis */
                            /* Creare ImportatioVisus et initiare sessionem */
                            importatio_visus = importatio_visus_creare(piscina);
                            si (importatio_visus != NIHIL)
                            {
                                importatio_visus_initiare_sessionem(importatio_visus, imago);
                                dialogus_importatio = dialogus_importatio_creare(piscina, importatio_visus);
                                si (dialogus_importatio != NIHIL)
                                {
                                    dialogus_activus = VERUM;
                                    fprintf(stderr, "Dialogus apertus\n");
                                }
                            }
                        }
                    }
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

        /* Rescalare imaginem si necesse */
        si (necesse_rescalare && habet_imaginem && imago != NIHIL)
        {
            imago_scalata = imago_scalare_ad_limites(
                imago,
                (i32)((s32)tabula->latitudo - 320),  /* Leave 300 for palette + margins */
                CDLXXX,  /* 480 */
                IMAGO_SCALA_BILINEARIS,
                piscina);

            si (imago_scalata.pixela != NIHIL)
            {
                fprintf(stderr, "Imago scalata: %u x %u\n",
                    imago_scalata.latitudo, imago_scalata.altitudo);

                /* Allocare buffer pro RGBA dithered */
                rgba_dithered = (i8*)piscina_allocare(piscina,
                    (memoriae_index)(imago_scalata.latitudo * imago_scalata.altitudo * 4));

                necesse_redithering = VERUM;
            }
            alioquin
            {
                fprintf(stderr, "Errore: non possum scalare imaginem\n");
                habet_imaginem = FALSUM;
            }
            necesse_rescalare = FALSUM;
        }

        /* Redithering si necesse */
        si (necesse_redithering && habet_imaginem)
        {
            si (modus_coloris)
            {
                dithering_fructus = dithering_atkinson_colorum(
                    imago_scalata.pixela,
                    imago_scalata.latitudo,
                    imago_scalata.altitudo,
                    colores_activi,
                    piscina);
            }
            alioquin
            {
                dithering_fructus = dithering_atkinson_griseum(
                    imago_scalata.pixela,
                    imago_scalata.latitudo,
                    imago_scalata.altitudo,
                    colores_activi,
                    puncta,
                    piscina);
            }

            si (dithering_fructus.successus)
            {
                dithering_indices_ad_rgba(
                    dithering_fructus.indices,
                    rgba_dithered,
                    (i32)imago_scalata.latitudo,
                    (i32)imago_scalata.altitudo);
            }

            necesse_redithering = FALSUM;
        }

        /* Purgare tabulam */
        tabula_pixelorum_vacare(tabula, color_ad_pixelum(thema_color(COLOR_BACKGROUND)));

        /* Delineare graticula colorum (sinistra) */
        {
            FructusGraticulaColorum fructus_grid;

            fructus_grid = elementa_graticula_colorum(
                tabula,
                piscina,
                20,
                20,
                colores_activi,
                eventus_ptr,
                1.0f);

            si (fructus_grid.mutatum)
            {
                s32 i;
                per (i = 0; i < XVI; i++)
                {
                    colores_activi[i] = fructus_grid.colores[i];
                }
                necesse_redithering = VERUM;
            }
        }

        /* Delineare imaginem dithered (post palette grid) */
        si (habet_imaginem && dithering_fructus.successus && rgba_dithered != NIHIL)
        {
            s32 dest_x = 300;  /* Post graticula colorum */
            s32 dest_y = ((s32)tabula->altitudo - (s32)imago_scalata.altitudo) / 2;

            per (y = 0; y < (s32)imago_scalata.altitudo; y++)
            {
                per (x = 0; x < (s32)imago_scalata.latitudo; x++)
                {
                    s32 src_idx = (y * (s32)imago_scalata.latitudo + x) * 4;
                    s32 dx = dest_x + x;
                    s32 dy = dest_y + y;

                    si (dx >= 0 && dx < (s32)tabula->latitudo &&
                        dy >= 0 && dy < (s32)tabula->altitudo)
                    {
                        s32 dest_idx = dy * (s32)tabula->latitudo + dx;
                        insignatus character r = (insignatus character)rgba_dithered[src_idx + 0];
                        insignatus character g = (insignatus character)rgba_dithered[src_idx + 1];
                        insignatus character b = (insignatus character)rgba_dithered[src_idx + 2];
                        tabula->pixela[dest_idx] = (i32)RGB(r, g, b);
                    }
                }
            }
        }

        /* Reddere dialogum si activus */
        si (dialogus_activus && dialogus_importatio != NIHIL)
        {
            Dialogus* dlg;
            dlg = dialogus_importatio_obtinere_dialogum(dialogus_importatio);
            si (dlg != NIHIL)
            {
                /* Reddere dialog overlay super totam fenestram */
                dialogus_reddere_overlay(
                    dlg,
                    tabula,
                    ZEPHYRUM, ZEPHYRUM,   /* panel position */
                    LXXI, LX,              /* panel size (71x60 chars) */
                    1.0f);
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
