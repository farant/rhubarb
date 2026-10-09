/* probatio_pictor_aspergillum.c - aspergillum MacPaint in pictore
 *
 * Applicatio pictoris vera (sola, volumen temporarium). I: pictor_gutta
 * determinata, intra discum, guttae diversae. II: 'a' aspergillum, 'p'
 * penicillus (ephemera, actio tabulae, linea status). III: ictus
 * scriptus: instrumentum, semen, t cuiusque puncti, punctum solutionis.
 * IV: guttae solae, intra discos punctorum. V: revocare / reficere /
 * reapertio - pixela EADEM. VI: mora (tenere immotum) guttas addit.
 * VII: praevisio - guttae ante solutionem (titulus tabulae, figura). */
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
#include "imago_png.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tabula in applicatione sola ad cellulam (VI, VIII) */
#define ORIGO_X VI
#define ORIGO_Y VIII

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* eventus muris ad punctum TABULAE (x, y) tempore t */
interior vacuum
mus (
      Dispensator* d,
  eventus_genus_t  genus,
              s32  x,
              s32  y,
              s64  t)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = genus;
    e.tempus            = t;
    e.datum.mus.x       = ORIGO_X + x;
    e.datum.mus.y       = ORIGO_Y + y;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(d, &e);
}

/* (x, y) in disco radii VIII circa (cx, cy)? */
interior b32
in_disco (
    s32 x,
    s32 y,
    s32 cx,
    s32 cy)
{
    redde (x - cx) * (x - cx) + (y - cy) * (y - cy) <= LXIV;
}

/* pixela tabulae coloris non albi */
interior i32
picta (
    PictorDocumentum* doc)
{
    i32 n;
    i32 x;
    i32 y;
    i32 album;

    album = (i32)tabula_pixelorum_obtinere_pixelum(doc->tabula,
        ZEPHYRUM,
        ZEPHYRUM);
    n = ZEPHYRUM;
    per (y = ZEPHYRUM; y < doc->tabula->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < doc->tabula->latitudo; x++)
        {
            si ((i32)tabula_pixelorum_obtinere_pixelum(doc->tabula, x,
                y)
                != album)
            {
                n++;
            }
        }
    }
    redde n;
}

/* photographia pixelorum tabulae */
interior i32*
photographia (
    PictorDocumentum* doc)
{
    i32* f;
    i32  x;
    i32  y;

    f = (i32*)piscina_allocare(piscina, (memoriae_index)(doc->tabula
        ->latitudo * doc->tabula->altitudo) * magnitudo(i32));
    per (y = ZEPHYRUM; y < doc->tabula->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < doc->tabula->latitudo; x++)
        {
            f[y * doc->tabula->latitudo + x] = (i32)
                tabula_pixelorum_obtinere_pixelum(doc->tabula, x, y);
        }
    }
    redde f;
}

interior b32
photographia_eadem (
    PictorDocumentum* doc,
                 i32* f)
{
    i32* g;

    g = photographia(doc);
    redde memcmp(f, g, (size_t)(doc->tabula->latitudo
        * doc->tabula->altitudo) * magnitudo(i32)) == ZEPHYRUM;
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

interior Componens*
nodus_arboris (
    PictorApplicatio* app,
  constans character* id)
{
    redde componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis(id, piscina));
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

/* guttae praevisionis (I x I, RGBA) coloris palettae 'color'; -1 =
 * quaelibet RGBA I x I */
interior i32
guttae_praevisae (
    PictorApplicatio* app,
                 s32  color)
{
     Mandata* md;
    Mandatum* x;
         i32  i;
         i32  n;

    md = mandata_creare(piscina, intern);
    pingere(dispensator_arbor(app->d), app->figurae, ZEPHYRUM, md);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
    {
        x = mandata_obtinere(md, i);
        si (   x->genus          == MANDATUM_RECTANGULUM
            && x->fines.latitudo == I
            && x->fines.altitudo == I
            && x->color.genus    == COLOR_MANDATI_RGBA
            && (color < ZEPHYRUM || x->color.valor
                == color_ad_pixelum(color_ex_palette((i32)color))))
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
color_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "color_primus",
        chorda_ut_cstr(chorda_ex_s32(*(s32*)ctx, p), p));
}

/* color primus per dominum suum (color_primus.ponere) */
interior vacuum
colorem_primum_ponere (
    PictorApplicatio* app,
                 s32  color)
{
    InsulaRamus r;

    r = insula_ramus_radix(app->repo);
    insula_scriptorem_ponere(app->repo, chorda_ex_literis(
        "color_primus.ponere", piscina));
    (vacuum)mutare_ramum(&r, INSULA_EPHEMERA, color_mutator, &color);
    insula_scriptorem_ponere(app->repo, chorda_ex_literis("", piscina));
}

/* ictus aspergilli: pressio (x, y) t0, motus per puncta, solutio */
interior vacuum
aspergere (
    PictorApplicatio* app,
                 s32  x,
                 s32  y,
                 s64  t0,
                 s64  mora_solutionis)
{
    mus(app->d, EVENTUS_MUS_DEPRESSUS, x, y, t0);
    mus(app->d, EVENTUS_MUS_MOTUS, x + X, y, t0 + XVI);
    mus(app->d, EVENTUS_MUS_MOTUS, x + XX, y + V, t0 + XXXII);
    mus(app->d, EVENTUS_MUS_LIBERATUS, x + XX, y + V,
        t0 + XXXII + mora_solutionis);
}

s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* doc;
                  i32* ante;
                  i32  n_brevis;
                  i32  n_longus;

    piscina = piscina_generare_dynamicum("probatio_pictor_aspergillum",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();

    imprimere("\n--- I: pictor_gutta ---\n");
    {
        s32 dx;
        s32 dy;
        s32 alter_x;
        s32 alter_y;
        i32 i;
        i32 k;
        i32 extra;
        i32 diversae;

        extra     = ZEPHYRUM;
        diversae  = ZEPHYRUM;
        per (i = ZEPHYRUM; i < L; i++)
        {
            per (k = ZEPHYRUM; k < X; k++)
            {
                pictor_gutta((s64)CCCXXI, i, k, VIII, &dx, &dy);
                pictor_gutta((s64)CCCXXI, i, k, VIII, &alter_x,
                    &alter_y);
                si (dx != alter_x || dy != alter_y)
                {
                    extra++;
                }
                si (dx * dx + dy * dy > LXIV)
                {
                    extra++;
                }
                pictor_gutta((s64)CCCXXI, i, k + I, VIII, &alter_x,
                    &alter_y);
                si (dx != alter_x || dy != alter_y)
                {
                    diversae++;
                }
            }
        }
        CREDO_AEQUALIS_I32(extra, ZEPHYRUM);
        CREDO_VERUM(diversae > CD);
        pictor_gutta((s64)CCCXXI, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, &dx,
            &dy);
        CREDO_VERUM(dx == ZEPHYRUM && dy == ZEPHYRUM);
        pictor_gutta((s64)CCCXXII, ZEPHYRUM, ZEPHYRUM, VIII, &alter_x,
            &alter_y);
        pictor_gutta((s64)CCCXXI, ZEPHYRUM, ZEPHYRUM, VIII, &dx, &dy);
        CREDO_VERUM(dx != alter_x || dy != alter_y);
    }

    vol = volumen_temporarium(piscina, "probatio_pictor_aspergillum");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m    = manus_ludus_creare(piscina, app.d);
    doc  = app.doc;

    imprimere("\n--- II: 'a' et 'p' ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, 'a', ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "instrumentum"),
        "aspergillum");
    dispensator_recomponere(app.d);
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app, "tabula")->actio,
        "aspergillum.ictus");
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "status")->titulus,
        "aspergillum");
    CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "instrumentum"),
        "penicillus");
    CREDO_VERUM(manus_ludus_clavem(m, 'a', ZEPHYRUM));

    imprimere("\n--- VIII: linea instrumentorum (P1a) ---\n");
    {
        Mandata* md;
            i32  i;
            i32  icon;
            i32  crux;
            i32  impletum;
            s32  textus_x;
       Mandatum* x;

        dispensator_recomponere(app.d);
        CREDO_NON_NIHIL(nodus_arboris(&app, "quadratum.instrumentum"));
        CREDO_NON_NIHIL(nodus_arboris(&app, "quadratum.color_primus"));
        CREDO_NON_NIHIL(nodus_arboris(&app,
            "quadratum.color_secundus"));
        si (   !nodus_arboris(&app, "quadratum.instrumentum")
            || !nodus_arboris(&app, "quadratum.color_primus")
            || !nodus_arboris(&app, "quadratum.color_secundus"))
        {
            credo_imprimere_compendium();
            redde I;
        }
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "quadratum.instrumentum")->titulus,
            "instrumentum:aspergillum");
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "quadratum.color_primus")->titulus, "color:0");
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "quadratum.color_secundus")->titulus, "color:-1");
        md = mandata_creare(piscina, intern);
        pingere(dispensator_arbor(app.d), app.figurae, ZEPHYRUM, md);
        icon      = ZEPHYRUM;
        crux      = ZEPHYRUM;
        impletum  = ZEPHYRUM;
        textus_x  = -I;
        per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
        {
            x = mandata_obtinere(md, i);
            si (   x->genus          == MANDATUM_RECTANGULUM
                && x->fines.latitudo == I
                && x->color.genus    == COLOR_MANDATI_THEMA
                && x->color.valor    == (i32)COLOR_TEXT)
            {
                icon++;
            }
            si (   x->genus       == MANDATUM_LINEA
                && x->color.valor == (i32)COLOR_TEXT)
            {
                crux++;
            }
            /* palette Aquinas ipsa (RGBA), non indices colorationis */
            si (   x->genus       == MANDATUM_RECTANGULUM && x->impletum
                && x->color.genus == COLOR_MANDATI_RGBA
                && x->color.valor
                   == color_ad_pixelum(color_ex_palette(ZEPHYRUM)))
            {
                impletum++;
            }
            si (   x->genus == MANDATUM_TEXTUS
                && chorda_aequalis_literis(x->textus, "aspergillum"))
            {
                textus_x = x->fines.x;
            }
        }
        /* icon aspergilli: XLIV pixela ('#'); crux coloris nulli: II
         * lineae; color primus 0 impletus; textus post quadrata
         * (LVIII + XX + VI) */
        CREDO_AEQUALIS_I32(icon, XLIV);
        CREDO_AEQUALIS_I32(crux, II);
        CREDO_AEQUALIS_I32(impletum, I);
        CREDO_AEQUALIS_S32(textus_x, LXXXIV);
    }

    imprimere("\n--- VII: praevisio ante solutionem ---\n");
    {
        Componens* t;

        mus(app.d, EVENTUS_MUS_DEPRESSUS, XL, XL, M);
        mus(app.d, EVENTUS_MUS_MOTUS, L, XL, M + XVI);
        dispensator_recomponere(app.d);
        t = nodus_arboris(&app, "tabula");
        CREDO_AEQUALIS_I32(t->numerus_punctorum, II);
        /* titulus = "semen radius color" */
        CREDO_VERUM(chorda_continet(t->titulus,
            chorda_ex_literis(" 8 0",
            piscina)));
        /* guttae colore VERO (color primus 0, palette Aquinas; Franus
         * - olim accentus) */
        CREDO_AEQUALIS_I32(guttae_praevisae(&app, ZEPHYRUM),
            II * PICTOR_GUTTAE_PUNCTO);
        /* color primus nullus: praevisio nulla (ut ictus) */
        colorem_primum_ponere(&app, -I);
        dispensator_recomponere(app.d);
        CREDO_AEQUALIS_I32(guttae_praevisae(&app, -I), ZEPHYRUM);
        colorem_primum_ponere(&app, ZEPHYRUM);
        dispensator_recomponere(app.d);
        /* nihil in documento ante solutionem */
        CREDO_AEQUALIS_I32(picta(doc), ZEPHYRUM);
        mus(app.d, EVENTUS_MUS_LIBERATUS, L, XL, M + XXXII);
        (vacuum)pictor_documentum_revocare(doc);
    }

    imprimere("\n--- III: ictus scriptus ---\n");
    aspergere(&app, LX, LX, MM, XVI);
    {
        chorda a;

        a = actum_ultimum(vol);
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "instrumentum=\"aspergillum\"", piscina)));
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis("semen=\"",
            piscina)));
        /* t ab initio ictus; punctum solutionis (XXXII + XVI) */
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "x=\"60\" y=\"60\" t=\"0\"", piscina)));
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "x=\"70\" y=\"60\" t=\"16\"", piscina)));
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "x=\"80\" y=\"65\" t=\"48\"", piscina)));
        CREDO_AEQUALIS_I32(chorda_numerare_occurrentia(a,
            chorda_ex_literis("<punctum", piscina)), IV);
    }

    imprimere("\n--- IV: guttae intra discos ---\n");
    {
        i32 x;
        i32 y;
        i32 extra;
        i32 album;
        i32 pixelum;

        CREDO_VERUM(picta(doc) > ZEPHYRUM);
        /* IV puncta (t 0, 16, 32, 48): VI + (VI + II) x III = XXX
         * guttae ad summum (coincidentes minus) */
        CREDO_VERUM(picta(doc) <= IV * PICTOR_GUTTAE_PUNCTO
            + III * (XVI / PICTOR_GUTTA_MS));
        CREDO_VERUM(picta(doc) > IV * PICTOR_GUTTAE_PUNCTO);
        album = (i32)tabula_pixelorum_obtinere_pixelum(doc->tabula,
            ZEPHYRUM, ZEPHYRUM);
        extra = ZEPHYRUM;
        per (y = ZEPHYRUM; y < doc->tabula->altitudo; y++)
        {
            per (x = ZEPHYRUM; x < doc->tabula->latitudo; x++)
            {
                pixelum = (i32)tabula_pixelorum_obtinere_pixelum(
                    doc->tabula, x, y);
                si (   pixelum != album
                    && !in_disco((s32)x, (s32)y, LX, LX)
                    && !in_disco((s32)x, (s32)y, LXX, LX)
                    && !in_disco((s32)x, (s32)y, LXXX, LXV))
                {
                    extra++;
                }
            }
        }
        CREDO_AEQUALIS_I32(extra, ZEPHYRUM);
    }

    imprimere("\n--- V: revocare, reficere, reapertio ---\n");
    ante = photographia(doc);
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_I32(picta(doc), ZEPHYRUM);
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_VERUM(photographia_eadem(doc, ante));
    {
        PictorDocumentum* alter;

        alter = pictor_documentum_aperire(piscina, intern, vol, "");
        CREDO_NON_NIHIL(alter);
        si (alter)
        {
            CREDO_VERUM(photographia_eadem(alter, ante));
        }
    }

    imprimere("\n--- VI: mora guttas addit ---\n");
    CREDO_VERUM(pictor_documentum_revocare(doc));
    aspergere(&app, LX, LX, MMM, XVI);
    n_brevis = picta(doc);
    /* idem ictus, tempore alio: semen aliud, guttae aliae */
    ante = photographia(doc);
    CREDO_VERUM(pictor_documentum_revocare(doc));
    aspergere(&app, LX, LX, MMM + C, XVI);
    CREDO_FALSUM(photographia_eadem(doc, ante));
    CREDO_VERUM(pictor_documentum_revocare(doc));
    aspergere(&app, LX, LX, MMM, XVI);
    CREDO_VERUM(photographia_eadem(doc, ante));
    CREDO_VERUM(pictor_documentum_revocare(doc));
    aspergere(&app, LX, LX, IV * M, M);
    n_longus = picta(doc);
    imprimere("  guttae: brevis %d, longa mora %d\n", (int)n_brevis,
        (int)n_longus);
    CREDO_VERUM(n_longus > n_brevis + L);
    /* imago (oculis inspicienda): ictus longae morae */
    (vacuum)imago_png_scribere(pictor_documentum_proiectio(doc),
        "build/probatio_pictor_aspergillum.png", piscina);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
