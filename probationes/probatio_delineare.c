/* Probatio pro delineare - functiones graphicae */
#include "latina.h"
#include "fenestra.h"
#include "delineare.h"
#include "color.h"
#include "chorda.h"
#include <stdio.h>

int
main (
    void)
{
    Piscina* piscina;
    FenestraConfiguratio configuratio;
    Fenestra* fenestra;
    TabulaPixelorum* tabula;
    ContextusDelineandi* ctx;
    Eventus eventus;
    b32 currens;
    i32 puncta_trianguli[VI];
    i32 puncta_polygoni[VIII];
    chorda textus;

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("delineare", M * M);
    si (piscina == NIHIL)
    {
        imprimere("Errore: non possum creare piscinam\n");
        return 1;
    }

    /* Configurare fenestram */
    configuratio.titulus = "Probatio Delineare";
    configuratio.x = C;
    configuratio.y = C;
    configuratio.latitudo = DCCC;
    configuratio.altitudo = DC;
    configuratio.vexilla = FENESTRA_ORDINARIA;

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

    /* Praeparare puncta pro polygono */
    puncta_trianguli[ZEPHYRUM] = C;        /* x0 */
    puncta_trianguli[I] = L;                /* y0 */
    puncta_trianguli[II] = CL;              /* x1 */
    puncta_trianguli[III] = CL;             /* y1 */
    puncta_trianguli[IV] = L;               /* x2 */
    puncta_trianguli[V] = CL;               /* y2 */

    puncta_polygoni[ZEPHYRUM] = CD;         /* x0 */
    puncta_polygoni[I] = CC;                /* y0 */
    puncta_polygoni[II] = CDLXXX;           /* x1 */
    puncta_polygoni[III] = CC;              /* y1 */
    puncta_polygoni[IV] = CDLXXX;           /* x2 */
    puncta_polygoni[V] = CD;                /* y2 */
    puncta_polygoni[VI] = CD;               /* x3 */
    puncta_polygoni[VII] = CD;              /* y3 */

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

        /* Purgare tabulam pixelorum (color caeruleus obscurus) */
        delineare_vacare(ctx, color_ex_rgb((i8)XXVI, (i8)XXVI, (i8)XLVI));

        /* Delineare formas varias */

        /* Circulus ruber */
        delineare_circulum(ctx, C, C, XL, color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM));

        /* Circulus plenus viridis */
        delineare_circulum_plenum(ctx, CC, C, XXX, color_ex_rgb((i8)ZEPHYRUM, (i8)CCLV, (i8)ZEPHYRUM));

        /* Rectangulum caeruleus */
        delineare_rectangulum(ctx, CCC, L, C, LXX, color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)CCLV));

        /* Rectangulum plenum flavus */
        delineare_rectangulum_plenum(ctx, CDLXXX, L, LXXX, LX, color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)ZEPHYRUM));

        /* Linea alba */
        delineare_lineam(ctx, L, CC, DC, CCC, color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)CCLV));

        /* Triangulum magenta */
        delineare_triangulum(ctx, puncta_trianguli[ZEPHYRUM], puncta_trianguli[I],
                                   puncta_trianguli[II], puncta_trianguli[III],
                                   puncta_trianguli[IV], puncta_trianguli[V],
                                   color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)CCLV));

        /* Triangulum plenum cyaneus */
        delineare_triangulum_plenum(ctx, CCC, CCC, CD, CCC, CCC, CDXX,
                                          color_ex_rgb((i8)ZEPHYRUM, (i8)CCLV, (i8)CCLV));

        /* Ellipsis aurantius */
        delineare_ellipsin(ctx, CD, CD, LX, XL, color_ex_rgb((i8)CCLV, (i8)CLXV, (i8)ZEPHYRUM));

        /* Ellipsis plena purpurea */
        delineare_ellipsin_plenam(ctx, DC, CD, XL, XXX, color_ex_rgb((i8)CXXX, (i8)L, (i8)CXXX));

        /* Polygonum albus */
        delineare_polygonum(ctx, puncta_polygoni, IV, color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)CCLV));

        /* Polygonum plenum viridis obscurus */
        delineare_polygonum_plenum(ctx, puncta_polygoni, IV, color_ex_rgb((i8)ZEPHYRUM, (i8)C, (i8)ZEPHYRUM));

        /* Rectangulum cum exemplar - tessellatum */
        delineare_ponere_modum(ctx, MODUS_EXEMPLAR);
        delineare_ponere_exemplar_internum(ctx, EXEMPLAR_TESSELLATUM);
        delineare_rectangulum_plenum(ctx, DC, CDXX, LX, L, color_ex_rgb((i8)CCLV, (i8)CCLV, (i8)ZEPHYRUM));
        delineare_ponere_modum(ctx, MODUS_SOLIDUS);

        /* Pixela singularia rubra */
        delineare_pixelum(ctx, X, X, color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM));
        delineare_pixelum(ctx, XII, X, color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM));
        delineare_pixelum(ctx, X, XII, color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM));
        delineare_pixelum(ctx, XII, XII, color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM));

        /* Lineae horizontalis et verticalis */
        delineare_lineam_horizontalem(ctx, DCC, DCCL, XXX, color_ex_rgb((i8)ZEPHYRUM, (i8)CCLV, (i8)CCLV));
        delineare_lineam_verticalem(ctx, DCCL, XXX, LXX, color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)CCLV));

        /* Crates griseus */
        delineare_cratem(ctx, L, color_ex_rgb((i8)LXIV, (i8)LXIV, (i8)LXIV));

        /* Pingere textum */
        textus = chorda_ex_literis("Probatio Delineare\n\nPremere EFFUGIUM ut claudas.", piscina);
        tabula_pixelorum_pingere_chordam(tabula, X, X, textus,
            color_ad_pixelum(color_ex_rgb((i8)CCXXXVIII, (i8)CCXXXVIII, (i8)CCXXXVIII)));

        /* Pingere textum scalatum (magnum titulum) */
        textus = chorda_ex_literis("DELINEARE", piscina);
        tabula_pixelorum_pingere_chordam_scalatam(tabula, CCC, CD, textus,
            color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)C, (i8)CCLV)), IV);

        /* Praesentare pixela ad fenestram */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Delere fenestram et piscinam */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
