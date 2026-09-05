/* probatio_delineare_mandata.c - mandata -> pixela (gradus VII):
 * asserta pixelorum sine exemplari, deinde specimen */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "color.h"
#include "thema.h"
#include "fenestra.h"
#include "mandatum.h"
#include "delineare_mandata.h"
#include "imago_typus.h"
#include "specimen.h"
#include "credo.h"
#include <stdio.h>

/* Fons imaginum probationis: "quadrum" = 4x4, ruber cum angulo
 * transparente */
interior constans Imago*
fons_probationis (
    chorda  provenientia,
    vacuum* ctx)
{
    si (chorda_aequalis_literis(provenientia, "quadrum"))
    {
        redde (constans Imago*)ctx;
    }
    redde NIHIL;
}

interior ColorMandati
rgba (
    i8 r,
    i8 g,
    i8 b)
{
    ColorMandati c;

    c.genus = COLOR_MANDATI_RGBA;
    c.valor = color_ad_pixelum(color_ex_rgba(r, g, b, (i8)CCLV));
    redde c;
}

s32 principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
         TabulaPixelorum* t;
                 Mandata* m;
                   Fines  f;
                 Punctum  a;
                 Punctum  b;
                   Imago  quadrum;
                   Imago  captura;
                     i32  coetus;
                     i32  i;
         SpecimenFructus  sf;
                  chorda  provenientia;

    piscina = piscina_generare_dynamicum("probatio_delineare_mandata",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();

    imprimere("\n--- Tabula nuda ---\n");
    t = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    CREDO_NON_NIHIL(t);
    CREDO_AEQUALIS_I32(t->latitudo, LXIV);
    CREDO_AEQUALIS_I32(t->altitudo, XLVIII);
    tabula_pixelorum_vacare(t, color_ad_pixelum(color_ex_rgb(
        (i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));

    /* quadrum 4x4 ruber, pixelum (0,0) transparens */
    quadrum.latitudo  = IV;
    quadrum.altitudo  = IV;
    quadrum.pixela    = (i8*)piscina_allocare(piscina, LXIV);
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        quadrum.pixela[i * IV]        = (i8)CCLV;
        quadrum.pixela[i * IV + I]    = ZEPHYRUM;
        quadrum.pixela[i * IV + II]   = ZEPHYRUM;
        quadrum.pixela[i * IV + III]  = (i8)CCLV;
    }
    quadrum.pixela[III] = ZEPHYRUM;   /* alpha (0,0) = 0 */

    imprimere("\n--- Mandata: rectangulum, linea, imago scalata, textus"
              " in coetu translato ---\n");
    m    = mandata_creare(piscina, intern);
    f.x  = II; f.y = II; f.latitudo = X; f.altitudo = VI;
    mandata_rectangulum(m, f, rgba((i8)ZEPHYRUM, (i8)CCLV,
        (i8)ZEPHYRUM),
                        VERUM);
    a.x = ZEPHYRUM; a.y = XX; b.x = XX; b.y = XX;
    mandata_linea(m, a, b, I, rgba((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)CCLV));
    /* coetus: origo (30,10), scala II, sectio ad (30,10,20,20) */
    f.x = XXX; f.y = X; f.latitudo = XX; f.altitudo = XX;
    provenientia = chorda_ex_literis("coetus_a", piscina);
    coetus = mandata_coetus_incipere(m, f, VERUM, ZEPHYRUM, ZEPHYRUM,
        II,
                                     provenientia);
    f.x = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = IV; f.altitudo = IV;
    mandata_imago(m, chorda_ex_literis("quadrum", piscina), f);
    /* rectangulum extra sectionem: (15,15,10,10) locale -> schirmo
     * (60,40)..(80,60), praecisum ad 50,30 */
    f.x = XV; f.y = XV; f.latitudo = X; f.altitudo = X;
    mandata_rectangulum(m, f, rgba((i8)CCLV, (i8)CCLV, (i8)ZEPHYRUM),
                        VERUM);
    mandata_coetus_finire(m, coetus);
    mandata_textus(m, II, XXX, chorda_ex_literis("Ok", piscina),
        ZEPHYRUM,
                   rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));

    delineare_mandata(m, t, fons_probationis, &quadrum);

    imprimere("\n--- Asserta pixelorum (sine exemplari) ---\n");
    /* rectangulum plenum viride: intra (5,5), extra (13,5) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, V, V),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)CCLV,
        (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XIII, V),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    /* linea caerulea per (10,20) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, X, XX),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)CCLV)));
    /* imago scalata II: pixelum (1,1) imaginis -> schirmo
     * (32..33, 12..13) ruber; pixelum (0,0) transparens -> schirmo
     * (30,10) manet niger */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XXXIII,
        XIII),
        color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XXX, X),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    /* sectio: flavum (60..80) praecisum ad 50: nihil flavum */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XLV, XXV),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, LV, XXXV),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM,
        (i8)ZEPHYRUM)));
    /* textus: aliquid album in linea 30..37 */
    {
        b32 album;
        i32 x;
        album = FALSUM;
        per (x = II; x < XIV; x++)
        {
            i32 y;
            per (y = XXX; y < XXXVIII; y++)
            {
                si (tabula_pixelorum_obtinere_pixelum(t, x, y)
                    == color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)CCLV,
                                                     (i8)CCLV)))
                {
                    album = VERUM;
                }
            }
        }
        CREDO_VERUM(album);
    }

    imprimere("\n--- Specimen (gradus VII) ---\n");
    captura = imago_ex_tabula(t);
    CREDO_AEQUALIS_I32(captura.latitudo, LXIV);
    sf = specimen_iudicare(&captura, "mandata_prima",
        specimen_regula_solita("probationes/pictor/specimina"),
        piscina);
    si (sf.sententia != SPECIMEN_CONGRUIT)
    {
        imprimere("SPECIMEN %s: %.*s\n",
                  specimen_sententia_nomen(sf.sententia),
                  (int)sf.causa.mensura, sf.causa.datum);
    }
    CREDO_VERUM(sf.sententia == SPECIMEN_CONGRUIT);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
