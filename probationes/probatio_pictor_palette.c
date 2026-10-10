/* probatio_pictor_palette.c - palettae lineae instrumentorum (P1b)
 *
 * Applicatio pictoris vera, CDLXXX x CDLXXX: linea status y CDLVI,
 * quadrata y CDLVIII: instrumentum x VI, color primus x XXXII, color
 * secundus x LVIII (XX x XX). I: ictus in quadratum palettam aperit
 * (nodus 'palette' filius ULTIMUS radicis, electa notata). II: idem
 * claudit, aliud commutat. III-IV: optio coloris ponit (etiam nullus,
 * -1) et claudit. V: optio instrumenti. VI: Esc claudit. VII: ictus in
 * tabula dum aperta: claudit solum, nihil pingitur. VIII: palette
 * super tabulam pingitur, electa margine accentus. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "figura.h"
#include "mandatum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "xar.h"
#include "pictor_documentum.h"
#include "pictor_applicatio.h"
#include "pictor_figurae.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QY CDLXVIII   /* centrum quadratorum (CDLVIII + X) */

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior chorda
ephemera_legere (
      PictorApplicatio* app,
    constans character* titulus)
{
    chorda* a;

    a = insula_attributum(app->repo, INSULA_EPHEMERA, titulus);
    redde a ? *a : chorda_ex_literis("", piscina);
}

interior Componens*
nodus_arboris (
      PictorApplicatio* app,
    constans character* id)
{
    redde componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis(id, piscina));
}

/* optio k palettae coloris (k = valor + I) quadrati x0: palette
 * LXXII alta, y CCCLXXXIV */
interior vacuum
optio_coloris (
    ManusLudus* m,
           s32  x0,
           s32  valor)
{
    s32 k;

    k = valor + I;
    (vacuum)manus_ludus_premere_ad(m, x0 + IV + (k % VI) * XXII + X,
        CCCLXXXIV + IV + (k / VI) * XXII + X);
}

s32 principale (vacuum)
{
           Volumen* vol;
  PictorApplicatio  app;
        ManusLudus* m;
         Componens* radix;
         Componens* p;

    piscina = piscina_generare_dynamicum("probatio_pictor_palette",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_palette");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m = manus_ludus_creare(piscina, app.d);
    dispensator_recomponere(app.d);
    CREDO_NIHIL(nodus_arboris(&app, "palette"));

    imprimere("\n--- I: quadratum instrumenti - palette aperta ---\n");
    CREDO_VERUM(manus_ludus_premere_ad(m, XVI, QY));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "instrumentum");
    p = nodus_arboris(&app, "palette");
    CREDO_NON_NIHIL(p);
    radix = dispensator_arbor(app.d);
    CREDO_VERUM(p && *(Componens**)xar_obtinere(radix->liberi,
        xar_numerus(radix->liberi) - I) == p);
    CREDO_VERUM(p && xar_numerus(p->liberi) == IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "optio.instrumentum.penicillus")->titulus,
        "instrumentum:penicillus:electum");
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "optio.instrumentum.aspergillum")->titulus,
        "instrumentum:aspergillum");
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "optio.instrumentum.spongia")->titulus,
        "instrumentum:spongia");

    imprimere("\n--- II: idem claudit, aliud commutat ---\n");
    CREDO_VERUM(manus_ludus_premere_ad(m, XVI, QY));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");
    CREDO_NIHIL(nodus_arboris(&app, "palette"));
    CREDO_VERUM(manus_ludus_premere_ad(m, XLII, QY));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "color_primus");
    CREDO_VERUM(manus_ludus_premere_ad(m, LXVIII, QY));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "color_secundus");
    CREDO_VERUM(xar_numerus(nodus_arboris(&app, "palette")->liberi)
        == XVII);
    /* nullus (-1) electus ordinarie */
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "optio.color_secundus.-1")->titulus, "color:-1:electum");

    imprimere("\n--- III: color secundus VII ---\n");
    optio_coloris(m, LVIII, VII);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
        "color_secundus"),
        "7");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "quadratum.color_secundus")->titulus, "color:7");

    imprimere("\n--- IV: color primus nullus, deinde III ---\n");
    CREDO_VERUM(manus_ludus_premere_ad(m, XLII, QY));
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "optio.color_primus.0")->titulus, "color:0:electum");
    optio_coloris(m, XXXII, -I);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "color_primus"),
        "-1");
    CREDO_VERUM(manus_ludus_premere_ad(m, XLII, QY));
    optio_coloris(m, XXXII, III);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "color_primus"),
        "3");

    imprimere("\n--- V: optio instrumenti ---\n");
    CREDO_VERUM(manus_ludus_premere_ad(m, XVI, QY));
    /* palette instrumentorum XXVIII alta, y CDXXVIII; aspergillum I */
    CREDO_VERUM(manus_ludus_premere_ad(m, VI + IV + XXII + X,
        CDXXVIII + IV + X));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "instrumentum"),
        "aspergillum");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");

    imprimere("\n--- VI: Esc claudit ---\n");
    CREDO_VERUM(manus_ludus_premere_ad(m, XLII, QY));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "instrumentum"),
        "aspergillum");

    imprimere("\n--- VII: ictus in tabula dum aperta ---\n");
    {
        s64 ante;

        ante = pictor_documentum_cursor(app.doc);
        CREDO_VERUM(manus_ludus_premere_ad(m, XLII, QY));
        CREDO_VERUM(manus_ludus_premere_ad(m, C, C));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "");
        CREDO_AEQUALIS_S64(pictor_documentum_cursor(app.doc), ante);
        /* palette clausa: ictus iterum pingit */
        CREDO_VERUM(manus_ludus_premere_ad(m, C, C));
        CREDO_VERUM(pictor_documentum_cursor(app.doc) > ante);
    }

    imprimere("\n--- VIII: palette super tabulam, electa notata ---\n");
    {
        Mandata* md;
            i32  i;
            s32  imago;
            s32  palette;
            i32  accentus;
       Mandatum* x;

        CREDO_VERUM(manus_ludus_premere_ad(m, XLII, QY));
        md = mandata_creare(piscina, intern);
        pingere(dispensator_arbor(app.d), app.figurae, ZEPHYRUM, md);
        imago     = -I;
        palette   = -I;
        accentus  = ZEPHYRUM;
        per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
        {
            x = mandata_obtinere(md, i);
            si (x->genus == MANDATUM_IMAGO)
            {
                imago = (s32)i;
            }
            si (   x->genus == MANDATUM_RECTANGULUM && x->impletum
                && x->color.genus == COLOR_MANDATI_THEMA
                && x->color.valor == (i32)COLOR_SUPERFICIES
                && x->fines.latitudo == CXXXVIII)
            {
                palette = (s32)i;
            }
            si (   x->genus == MANDATUM_RECTANGULUM && !x->impletum
                && x->color.valor == (i32)COLOR_ACCENT_PRIMARY)
            {
                accentus++;
            }
        }
        CREDO_VERUM(imago >= ZEPHYRUM && palette > imago);
        /* imago sine fenestra (oculis inspicienda) */
        {
            TabulaPixelorum* tp;
              LudusFenestra* lf;

            tp = tabula_pixelorum_creare_nuda(piscina, CDLXXX, CDLXXX);
            lf = ludus_fenestra_creare(piscina, app.d, app.figurae,
                ZEPHYRUM, pictor_imago_fons, &app.montatio.figurae_ctx,
                tp);
            si (lf)
            {
                ludus_quadrum(lf, M);
                (vacuum)ludus_fenestra_imaginem_scribere(lf,
                    "build/probatio_pictor_palette.png");
            }
        }
        /* electa (color primus III): margines duo accentus */
        CREDO_AEQUALIS_I32(accentus, II);
        /* XVI colores optionum DISTINCTI (palette Aquinas ipsa; olim
         * indices colorationis colores repetebant) */
        {
            i32 colores[XX];
            i32 n;
            i32 j;
            i32 k;
            i32 duplicata;

            n = ZEPHYRUM;
            per (i = ZEPHYRUM; i < mandata_numerus(md) && n < XX; i++)
            {
                x = mandata_obtinere(md, i);
                si (   x->genus == MANDATUM_RECTANGULUM && x->impletum
                    && x->color.genus == COLOR_MANDATI_RGBA
                    && x->fines.latitudo == XVI)
                {
                    colores[n] = x->color.valor;
                    n++;
                }
            }
            /* quadrata in linea (primus III, secundus VII, exemplar
             * solidum colore III - P3) + XVI optiones */
            CREDO_AEQUALIS_I32(n, XIX);
            duplicata = ZEPHYRUM;
            per (j = ZEPHYRUM; j < n; j++)
            {
                per (k = j + I; k < n; k++)
                {
                    si (colores[j] == colores[k])
                    {
                        duplicata++;
                    }
                }
            }
            /* III ter (paria III: linea, exemplar, palette), VII bis
             * (par I), ceteri semel */
            CREDO_AEQUALIS_I32(duplicata, IV);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
