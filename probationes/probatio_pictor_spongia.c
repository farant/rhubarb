/* probatio_pictor_spongia.c - spongia (P2) in pictore
 *
 * Applicatio pictoris vera (sola, volumen temporarium). I: 'e'
 * spongia (ephemera, actio tabulae, linea status, quadratum). II:
 * praevisio - quadrata colore fundi in quoque puncto lineae, margo
 * cursoris. III: spongia lineam penicilli delet intra quadratum solum.
 * IV: tractus celer (puncta II remota) fasciam sine lacunis delet. V:
 * actum scriptum sine colore. VI: revocare / reficere / reapertio -
 * pixela EADEM. */
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

/* ictus a (x0, y0) ad (x1, y1): pressio, motus, solutio */
interior vacuum
ictus (
    PictorApplicatio* app,
                 s32  x0,
                 s32  y0,
                 s32  x1,
                 s32  y1,
                 s64  t)
{
    mus(app->d, EVENTUS_MUS_DEPRESSUS, x0, y0, t);
    mus(app->d, EVENTUS_MUS_MOTUS, x1, y1, t + XVI);
    mus(app->d, EVENTUS_MUS_LIBERATUS, x1, y1, t + XXXII);
}

interior i32
pixelum (
    PictorDocumentum* doc,
                 s32  x,
                 s32  y)
{
    redde (i32)tabula_pixelorum_obtinere_pixelum(doc->tabula, (i32)x,
        (i32)y);
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

s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* doc;
                  i32  fundus;
                  i32  atramentum;
                  i32* ante;

    piscina = piscina_generare_dynamicum("probatio_pictor_spongia",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_spongia");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m           = manus_ludus_creare(piscina, app.d);
    doc         = app.doc;
    fundus      = (i32)color_ad_pixelum(thema_color(COLOR_BACKGROUND));
    atramentum  = (i32)color_ad_pixelum(color_ex_palette(ZEPHYRUM));
    CREDO_AEQUALIS_I32(pixelum(doc, ZEPHYRUM, ZEPHYRUM), fundus);

    imprimere("\n--- I: 'e' spongia ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, 'e', ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "instrumentum"),
        "spongia");
    dispensator_recomponere(app.d);
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app, "tabula")->actio,
        "spongia.ictus");
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "status")->titulus, "spongia");
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "quadratum.instrumentum")->titulus, "instrumentum:spongia");

    imprimere("\n--- II: praevisio ante solutionem ---\n");
    {
         Componens* t;
           Mandata* md;
          Mandatum* x;
               i32  i;
               i32  quadrata;
               i32  cursor;

        mus(app.d, EVENTUS_MUS_DEPRESSUS, XL, XL, M);
        mus(app.d, EVENTUS_MUS_MOTUS, L, XL, M + XVI);
        dispensator_recomponere(app.d);
        t = nodus_arboris(&app, "tabula");
        CREDO_AEQUALIS_I32(t->numerus_punctorum, II);
        /* titulus = latus quadrati (XVI x magnitudo) */
        CREDO_CHORDA_AEQUALIS_LITERIS(t->titulus, "16");
        md = mandata_creare(piscina, intern);
        pingere(dispensator_arbor(app.d), app.figurae, ZEPHYRUM, md);
        quadrata  = ZEPHYRUM;
        cursor    = ZEPHYRUM;
        per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
        {
            x = mandata_obtinere(md, i);
            si (   x->genus          != MANDATUM_RECTANGULUM
                || x->fines.latitudo != XVI
                || x->fines.altitudo != XVI
                || x->color.genus    != COLOR_MANDATI_THEMA)
            {
                perge;
            }
            si (x->impletum && x->color.valor == (i32)COLOR_BACKGROUND)
            {
                quadrata++;
            }
            si (!x->impletum && x->color.valor == (i32)COLOR_BORDER)
            {
                cursor++;
                CREDO_AEQUALIS_S32(x->fines.x, L - VIII);
                CREDO_AEQUALIS_S32(x->fines.y, XL - VIII);
            }
        }
        /* puncta lineae XL..L: XI quadrata; cursor unus in fine */
        CREDO_AEQUALIS_I32(quadrata, XI);
        CREDO_AEQUALIS_I32(cursor, I);
        mus(app.d, EVENTUS_MUS_LIBERATUS, L, XL, M + XXXII);
        (vacuum)pictor_documentum_revocare(doc);
    }

    imprimere("\n--- III: linea penicilli deleta ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));
    ictus(&app, XX, C, CCC, C, MM);
    CREDO_AEQUALIS_I32(pixelum(doc, XC, C), atramentum);
    CREDO_VERUM(manus_ludus_clavem(m, 'e', ZEPHYRUM));
    ictus(&app, C, C, CC, C, MMM);
    {
        s32 x;
        i32 deleta;

        deleta = ZEPHYRUM;
        per (x = C - VIII; x < CC + VIII; x++)
        {
            si (pixelum(doc, x, C) == fundus)
            {
                deleta++;
            }
        }
        /* [C - VIII, CC + VIII) totum */
        CREDO_AEQUALIS_I32(deleta, CXVI);
        CREDO_AEQUALIS_I32(pixelum(doc, C - IX, C), atramentum);
        CREDO_AEQUALIS_I32(pixelum(doc, CC + VIII, C), atramentum);
        CREDO_AEQUALIS_I32(pixelum(doc, XX, C), atramentum);
    }

    imprimere("\n--- IV: tractus celer sine lacunis ---\n");
    {
        s32 x;
        s32 y;
        s32 t;
        s32 cx;
        s32 cy;
        i32 lacunae;

        CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));
        per (y = CL; y <= CCL; y++)
        {
            ictus(&app, CL, y, CCC, y, IV * M + (s64)y * L);
        }
        CREDO_AEQUALIS_I32(pixelum(doc, CC, CC), atramentum);
        CREDO_VERUM(manus_ludus_clavem(m, 'e', ZEPHYRUM));
        /* puncta II tantum: (CLX, CLX) -> (CCLXXX, CCXL) */
        ictus(&app, CLX, CLX, CCLXXX, CCXL, XX * M);
        lacunae = ZEPHYRUM;
        per (t = ZEPHYRUM; t <= CXX; t++)
        {
            cx = CLX + t;
            cy = CLX + (t * LXXX + LX) / CXX;
            per (y = -VII; y <= VII; y++)
            {
                per (x = -VII; x <= VII; x++)
                {
                    si (pixelum(doc, cx + x, cy + y) != fundus)
                    {
                        lacunae++;
                    }
                }
            }
        }
        CREDO_AEQUALIS_I32(lacunae, ZEPHYRUM);
        /* extra fasciam: atramentum manet */
        CREDO_AEQUALIS_I32(pixelum(doc, CCXC, CLX), atramentum);
        CREDO_AEQUALIS_I32(pixelum(doc, CLX, CCXL), atramentum);
    }

    imprimere("\n--- V: actum sine colore ---\n");
    {
        chorda a;

        a = actum_ultimum(vol);
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "instrumentum=\"spongia\"", piscina)));
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "magnitudo=\"1\"", piscina)));
        CREDO_FALSUM(chorda_continet(a, chorda_ex_literis("color=",
            piscina)));
        CREDO_AEQUALIS_I32(chorda_numerare_occurrentia(a,
            chorda_ex_literis("<punctum", piscina)), II);
    }

    imprimere("\n--- VI: revocare, reficere, reapertio ---\n");
    ante = photographia(doc);
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_I32(pixelum(doc, CC, CC), atramentum);
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_VERUM(photographia_eadem(doc, ante));
    CREDO_VERUM(pictor_documentum_verificare(doc));
    {
        PictorDocumentum* alter;

        alter = pictor_documentum_aperire(piscina, intern, vol, "");
        CREDO_NON_NIHIL(alter);
        si (alter)
        {
            CREDO_VERUM(photographia_eadem(alter, ante));
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
