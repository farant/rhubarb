/* Probatio pro tempus - functiones temporis et animationis */
#include "latina.h"
#include "fenestra.h"
#include "delineare.h"
#include "color.h"
#include "tempus.h"
#include "chorda.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int
main (
    void)
{
                 Piscina* piscina;
    FenestraConfiguratio  configuratio;
                Fenestra* fenestra;
         TabulaPixelorum* tabula;
      ContextusDelineandi* ctx;
                 Eventus  eventus;
                     b32  currens;
                  chorda  textus;
                     f32  tempus_currens;
       StatisticaeQuadri* statisticae;
                     i32  i;
                     i32  x;
                     i32  y;
                     i32  radius;
                   Color  color;
                    character buffer_textus[LXIV];

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("tempus", M * M);
    si (piscina == NIHIL)
    {
        imprimere("Errore: non possum creare piscinam\n");
        return 1;
    }

    /* Initiare systema temporis */
    tempus_initiare();
    tempus_ponere_quadra_destinata(60.0);

    /* Configurare fenestram */
    configuratio.titulus  = "Probatio Tempus";
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

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (ctx == NIHIL)
    {
        imprimere("Errore: non possum creare contextum delineandi\n");
        fenestra_destruere(fenestra);
        piscina_destruere(piscina);
        return 1;
    }

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    imprimere("Probatio Tempus\n");
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
                    si (eventus.datum.clavis.clavis == CLAVIS_EFFUGIUM)
                    {
                        currens = FALSUM;
                    }
                    frange;

                ordinarius:
                    frange;
            }
        }

        /* Actualizare tempus */
        tempus_quadrum();
        tempus_currens = (f32)tempus_nunc();
        statisticae = tempus_obtinere_statisticas();

        /* Purgare tabulam pixelorum */
        delineare_vacare(ctx, color_ex_rgb((i8)XX, (i8)XX, (i8)XXX));

        /* Delineare titulum */
        textus = chorda_ex_literis("PROBATIO TEMPUS", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, X, X, textus,
            color_ad_pixelum(color_ex_rgb((i8)CCXXXVIII, (i8)CCXXXVIII, (i8)CCLV)), II);

        /* Delineare FPS */
        sprintf(buffer_textus, "FPS: %.1f (avg: %.1f)",
                statisticae->quadra_currentia_per_secundum,
                statisticae->quadra_media_per_secundum);
        textus = chorda_ex_literis(buffer_textus, piscina);
        tabula_pixelorum_pingere_chordam(tabula, X, XXV, textus,
            color_ad_pixelum(color_ex_rgb((i8)C, (i8)CCLV, (i8)C)));

        /* Sectio I: Functiones Lenitionis - Pilae Resilientes */
        /* Quinque pilae cum lenitionibus variis */
        per (i = ZEPHYRUM; i < V; i++)
        {
            f32 phase;
            f32 t;
            i32 base_y;

            phase = tempus_currens + (f32)i * 0.3f;
            t = oscillare(phase * 0.8f, 1.0f);

            /* Usare lenitionem resilientem pro effectu pilae resilientis */
            base_y = CCC - (i32)(lenire_resiliens_egressus(t) * C);

            x = L + i * C;
            y = base_y;
            radius = XV;

            /* Color diversus pro quaque pila */
            color = color_ex_rgb((i8)(C + i * XXX), (i8)(CC - i * XX), (i8)(CL + i * XX));
            delineare_circulum_plenum(ctx, x, y, radius, color);
            delineare_circulum(ctx, x, y, radius, color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)CCLV));
        }

        /* Sectio II: Undae Sinusoidales - Circuli Oscillantes */
        per (i = ZEPHYRUM; i < IV; i++)
        {
            f32 offset_x;
            f32 offset_y;

            offset_x = unda_sinus(tempus_currens + (f32)i * 0.5f, 0.5f, 30.0f);
            offset_y = unda_sinus(tempus_currens + (f32)i * 0.7f, 0.3f, 20.0f);

            x = CD + i * LX + (i32)offset_x;
            y = CL + (i32)offset_y;
            radius = X + i * III;

            color = color_ex_rgb((i8)(CCLV - i * XL), (i8)(C + i * L), (i8)(CCLV - i * XXX));
            delineare_circulum_plenum(ctx, x, y, radius, color);
        }

        /* Sectio III: Unda Triangularis - Motus Linearis */
        {
            f32 triangulus_x;

            triangulus_x = unda_triangulus(tempus_currens, 0.3f, 80.0f);
            x = DC + (i32)triangulus_x;
            y = CCC;

            delineare_rectangulum_plenum(ctx, x - XV, y - XV, XXX, XXX,
                color_ex_rgb((i8)CCLV, (i8)C, (i8)CCLV));
            delineare_rectangulum(ctx, x - XV, y - XV, XXX, XXX,
                color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)CCLV));
        }

        /* Sectio IV: Lenire Elasticum - Effectus Elasticus */
        {
            f32 elasticus_t;
            i32 elasticus_y;

            elasticus_t = oscillare(tempus_currens * 0.5f, 1.0f);
            elasticus_y = CC + (i32)(lenire_elasticus_egressus(elasticus_t) * L);

            x = C;
            y = elasticus_y;
            radius = XX;

            delineare_circulum_plenum(ctx, x, y, radius, color_ex_rgb((i8)C, (i8)CCLV, (i8)CCLV));
            delineare_circulum(ctx, x, y, radius, color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)CCLV));
        }

        /* Sectio V: Interpolatio Coloris - Transitus Coloris */
        {
            f32 color_t;
            Color color_interpolatus;
            Color color_initialis;
            Color color_finalis;
            i32 t_scaled;

            color_t = (unda_sinus(tempus_currens, 0.5f, 1.0f) + 1.0f) / 2.0f;
            color_initialis = color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM);
            color_finalis = color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)CCLV);
            t_scaled = (i32)(color_t * 256.0f);
            color_interpolatus = color_interpolate(color_initialis, color_finalis, t_scaled);

            delineare_rectangulum_plenum(ctx, DC, CCC, LX, XL, color_interpolatus);
            delineare_rectangulum(ctx, DC, CCC, LX, XL, color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)CCLV));
        }

        /* Sectio VI: Linea Rotans - Interpolatio Anguli */
        {
            f32 angulus;
            i32 centrum_x;
            i32 centrum_y;
            i32 x1;
            i32 y1;
            i32 x2;
            i32 y2;

            angulus = tempus_currens * 2.0f;
            centrum_x = DCCL;
            centrum_y = C;

            x1 = centrum_x + (i32)(cos(angulus) * XL);
            y1 = centrum_y + (i32)(sin(angulus) * XL);
            x2 = centrum_x - (i32)(cos(angulus) * XL);
            y2 = centrum_y - (i32)(sin(angulus) * XL);

            delineare_lineam(ctx, x1, y1, x2, y2, color_ex_rgb((i8)CCLV, (i8)CC, (i8)ZEPHYRUM));
            delineare_circulum_plenum(ctx, centrum_x, centrum_y, V,
                color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)ZEPHYRUM));
        }

        /* Sectio VII: Unda Dens Serrae - Motus Linearis Reciprocus */
        {
            f32 dens_x;

            dens_x = unda_dens_serrae(tempus_currens, 0.4f, 60.0f);
            x = DCCL + (i32)dens_x;
            y = CC;

            delineare_triangulum_plenum(ctx, x, y - XX, x - XV, y + XV, x + XV, y + XV,
                color_ex_rgb((i8)ZEPHYRUM, (i8)CCLV, (i8)ZEPHYRUM));
        }

        /* Delineare instructiones */
        textus = chorda_ex_literis("Premere EFFUGIUM ut claudas", piscina);
        tabula_pixelorum_pingere_chordam(tabula, X, CDLX, textus,
            color_ad_pixelum(color_ex_rgb((i8)CLXXX, (i8)CLXXX, (i8)CLXXX)));

        /* Descriptiones effectuum */
        textus = chorda_ex_literis("Resiliens", piscina);
        tabula_pixelorum_pingere_chordam(tabula, L, CCCXL, textus,
            color_ad_pixelum(color_ex_rgb((i8)CC, (i8)CC, (i8)CC)));

        textus = chorda_ex_literis("Sinus", piscina);
        tabula_pixelorum_pingere_chordam(tabula, CD, CCCXL, textus,
            color_ad_pixelum(color_ex_rgb((i8)CC, (i8)CC, (i8)CC)));

        textus = chorda_ex_literis("Triangulus", piscina);
        tabula_pixelorum_pingere_chordam(tabula, DC, CCCXL, textus,
            color_ad_pixelum(color_ex_rgb((i8)CC, (i8)CC, (i8)CC)));

        textus = chorda_ex_literis("Elasticus", piscina);
        tabula_pixelorum_pingere_chordam(tabula, C, CCCXL, textus,
            color_ad_pixelum(color_ex_rgb((i8)CC, (i8)CC, (i8)CC)));

        textus = chorda_ex_literis("Color", piscina);
        tabula_pixelorum_pingere_chordam(tabula, DC, CCCXL, textus,
            color_ad_pixelum(color_ex_rgb((i8)CC, (i8)CC, (i8)CC)));

        /* Praesentare pixela ad fenestram */
        fenestra_praesentare_pixela(fenestra, tabula);

        /* Exspectare pro quadro destinato */
        tempus_exspectare_quadrum();
    }

    /* Delere fenestram et piscinam */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
