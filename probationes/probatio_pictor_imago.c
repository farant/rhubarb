/* probatio_pictor_imago.c - imago glutinata in stratum novum (L5)
 *
 * I: pictor_imaginem_aptare - "contain": lata (C x XX in CDLXVIII x
 * CDXL), alta, parva (sursum); centrata; indices palettae (0-15) aut
 * 0xFF; alpha < CXXVIII perspicuum. II: Cmd+V cum porta ficta - stratum
 * novum supra currens, currens fit, pixela palettae in strato, margo
 * perspicuus; revocatio una stratum et imaginem tollit; reficere
 * reddit. III: reapertio - massa ex volumine, pixela eadem. IV: sine
 * porta aut imagine vacua nihil. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "figura.h"
#include "mandatum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "xar.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include "imago.h"
#include "imago_typus.h"
#include "pictor_actiones.h"
#include "delineare.h"
#include "tabula_pixelorum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tabula in applicatione sola ad cellulam (VI, VIII) */
#define ORIGO_X VI
#define ORIGO_Y VIII

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;


interior i32
pixelum (
    PictorDocumentum* doc,
                 s32  x,
                 s32  y)
{
    redde (i32)tabula_pixelorum_obtinere_pixelum(doc->tabula, (i32)x,
        (i32)y);
}

interior i32
palettae (
    s32 index)
{
    redde (i32)color_ad_pixelum(color_ex_palette((i32)index));
}

interior chorda
ephemera_legere (
    PictorApplicatio* app,
  constans character* titulus)
{
    chorda* a;

    a = insula_attributum(app->repo, INSULA_EPHEMERA, titulus);
    redde a ? *a : chorda_ex_literis("", piscina);
}

/* datum acti 'ictus' ultimi in volumine */
interior chorda
actum_ultimum (
    Volumen* vol)
{
             Xar* acta;
    VolumenActum* a;
          chorda  ultimum;
             i32  i;

    ultimum  = chorda_ex_literis("", piscina);
    acta     = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        a = (VolumenActum*)xar_obtinere(acta, i);
        si (chorda_aequalis_literis(a->genus, "ictus"))
        {
            ultimum = a->datum;
        }
    }
    redde ultimum;
}

/* porta ficta: imago C x L, dimidium sinistrum rubrum, dextrum
 * caeruleum, quadrans infimus dexter perspicuus */
interior ImagoFructus
porta_ficta (
    vacuum* ctx,
   Piscina* p)
{
    ImagoFructus  f;
             i32  x;
             i32  y;
              i8* q;

    (vacuum)ctx;
    memset(&f, ZEPHYRUM, magnitudo(f));
    f.imago.latitudo = C;
    f.imago.altitudo = L;
    f.imago.pixela = (i8*)piscina_allocare(p,
        (memoriae_index)(C * L * IV));
    per (y = ZEPHYRUM; y < L; y++)
    {
        per (x = ZEPHYRUM; x < C; x++)
        {
            q            = f.imago.pixela + (y * C + x) * IV;
            q[ZEPHYRUM]  = x < L ? (i8)CCLV : ZEPHYRUM;
            q[I]         = x < L ? ZEPHYRUM : (i8)C;
            q[II]        = x < L ? ZEPHYRUM : (i8)CCLV;
            q[III]       = (x >= L && y >= XXV) ? ZEPHYRUM : (i8)CCLV;
        }
    }
    f.successus = VERUM;
    redde f;
}

interior ImagoFructus
porta_vacua (
    vacuum* ctx,
   Piscina* p)
{
    ImagoFructus f;

    (vacuum)ctx;
    (vacuum)p;
    memset(&f, ZEPHYRUM, magnitudo(f));
    redde f;
}

/* imago plena colore uno (RGBA) */
interior Imago
imago_plena (
    i32 lat,
    i32 alt,
     i8 alpha)
{
    Imago im;
      i32 i;

    im.latitudo = lat;
    im.altitudo = alt;
    im.pixela = (i8*)piscina_allocare(piscina,
        (memoriae_index)(lat * alt * IV));
    per (i = ZEPHYRUM; i < lat * alt; i++)
    {
        im.pixela[i * IV]        = (i8)CC;
        im.pixela[i * IV + I]    = (i8)L;
        im.pixela[i * IV + II]   = (i8)L;
        im.pixela[i * IV + III]  = alpha;
    }
    redde im;
}

interior b32
cmd_v (
    PictorApplicatio* app)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                     = M;
    e.datum.clavis.clavis        = (clavis_t)'v';
    e.datum.clavis.runa          = 'v';
    e.datum.clavis.modificantes  = MOD_SUPER;
    e.datum.clavis.actio         = EVENTUS_ACTIO_PRESSA;
    dispensator_tractare(app->d, &e);
    redde VERUM;
}

s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
     PictorDocumentum* doc;
                  i32  fundus;

    piscina = piscina_generare_dynamicum("probatio_pictor_imago",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    fundus = (i32)color_ad_pixelum(thema_color(COLOR_BACKGROUND));

    imprimere("\n--- I: aptare (contain) ---\n");
    {
        Imago  im;
           i8* ix;
          s32  x;
          s32  y;
          i32  w;
          i32  h;
          i32  i;
          i32  mala;

        /* lata C x XX in CDLXVIII x CDXL: latitudo plena, altitudo
         * XX x CDLXVIII / C = XCIII, centrata */
        im = imago_plena(C, XX, (i8)CCLV);
        ix = pictor_imaginem_aptare(&im, CDLXVIII, CDXL, piscina, &x,
            &y,
            &w, &h);
        CREDO_NON_NIHIL(ix);
        CREDO_AEQUALIS_I32(w, CDLXVIII);
        CREDO_AEQUALIS_I32(h, XCIII);
        CREDO_AEQUALIS_S32(x, ZEPHYRUM);
        CREDO_AEQUALIS_S32(y, (CDXL - XCIII) / II);
        mala = ZEPHYRUM;
        per (i = ZEPHYRUM; ix && i < w * h; i++)
        {
            si ((i32)ix[i] >= XVI)
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
        /* alta XX x C: altitudo plena */
        im = imago_plena(XX, C, (i8)CCLV);
        ix = pictor_imaginem_aptare(&im, CDLXVIII, CDXL, piscina, &x,
            &y,
            &w, &h);
        CREDO_AEQUALIS_I32(h, CDXL);
        CREDO_AEQUALIS_I32(w, LXXXVIII);
        CREDO_AEQUALIS_S32(y, ZEPHYRUM);
        CREDO_AEQUALIS_S32(x, (CDLXVIII - LXXXVIII) / II);
        /* parva X x X: sursum ad CDXL */
        im = imago_plena(X, X, (i8)CCLV);
        ix = pictor_imaginem_aptare(&im, CDLXVIII, CDXL, piscina, &x,
            &y,
            &w, &h);
        CREDO_AEQUALIS_I32(w, CDXL);
        CREDO_AEQUALIS_I32(h, CDXL);
        /* alpha nullum: omnia perspicua */
        im = imago_plena(X, X, ZEPHYRUM);
        ix = pictor_imaginem_aptare(&im, X, X, piscina, &x, &y, &w, &h);
        mala = ZEPHYRUM;
        per (i = ZEPHYRUM; ix && i < w * h; i++)
        {
            si (ix[i] != (i8)CCLV)
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
        /* fons vacua */
        im.latitudo = ZEPHYRUM;
        CREDO_VERUM(pictor_imaginem_aptare(&im, X, X, piscina, &x, &y,
            &w, &h) == NIHIL);
    }

    vol = volumen_temporarium(piscina, "probatio_pictor_imago");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    doc = app.doc;

    imprimere("\n--- IV: sine porta, porta vacua - nihil ---\n");
    CREDO_VERUM(cmd_v(&app));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    app.montatio.actiones_ctx.imago_capere = porta_vacua;
    CREDO_VERUM(cmd_v(&app));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);

    imprimere("\n--- II: Cmd+V - stratum novum ---\n");
    {
        s32 lat;
        s32 alt;
        s32 h;
        s32 y0;
        i32 rubri;
        i32 perspicui;
        i32 x;
        i32 y;
        i32 p;
        i32 r;

        app.montatio.actiones_ctx.imago_capere = porta_ficta;
        CREDO_VERUM(cmd_v(&app));
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            II);
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "stratum_activum"), "2");
        CREDO_VERUM(chorda_continet(actum_ultimum(vol),
            chorda_ex_literis(
            "<imago stratum=\"2\"", piscina)));
        /* C x L in tabula: latitudo plena, centrata verticaliter */
        lat        = (s32)doc->latitudo;
        alt        = (s32)doc->altitudo;
        h          = L * lat / C;
        y0         = (alt - h) / II;
        r          = palettae(VI) ;
        rubri      = ZEPHYRUM;
        perspicui  = ZEPHYRUM;
        per (y = ZEPHYRUM; y < (i32)alt; y++)
        {
            per (x = ZEPHYRUM; x < (i32)lat; x++)
            {
                p = (i32)tabula_pixelorum_obtinere_pixelum(
                    pictor_documentum_stratum(doc, I)->pixela, x, y);
                si ((p >> XXIV) == ZEPHYRUM)
                {
                    perspicui++;
                }
                alioquin si (x < (i32)(lat / II))
                {
                    rubri++;
                }
            }
        }
        (vacuum)r;
        /* margines supra et infra + quadrans perspicuus */
        CREDO_VERUM(perspicui > (i32)(lat * y0 * II));
        CREDO_VERUM(rubri > (i32)(lat / II * h / II));
        /* margo summus: fundus in composito */
        CREDO_AEQUALIS_I32(pixelum(doc, X, I), fundus);
        CREDO_VERUM(pixelum(doc, X, y0 + h / II) != fundus);
        /* revocatio una: stratum et imago */
        CREDO_VERUM(pictor_documentum_revocare(doc));
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
        CREDO_AEQUALIS_I32(pixelum(doc, X, y0 + h / II), fundus);
        CREDO_VERUM(pictor_documentum_reficere(doc));
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            II);
        CREDO_VERUM(pixelum(doc, X, y0 + h / II) != fundus);
    }

    imprimere("\n--- III: reapertio ---\n");
    {
        PictorDocumentum* alter;

        alter = pictor_documentum_aperire(piscina, intern, vol, "");
        CREDO_NON_NIHIL(alter);
        CREDO_VERUM(alter && memcmp(alter->tabula->pixela,
            doc->tabula->pixela, (size_t)(doc->latitudo * doc->altitudo)
            * magnitudo(i32)) == ZEPHYRUM);
        CREDO_VERUM(alter && pictor_documentum_verificare(alter));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
