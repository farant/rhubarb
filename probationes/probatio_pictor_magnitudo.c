/* probatio_pictor_magnitudo.c - magnitudines penicilli (P4a)
 *
 * Applicatio pictoris vera (sola, volumen temporarium). I: penicillus
 * VIII - pixela = discus VIII per lineam tractus (regula exacta). II:
 * tractus celer (puncta II remota) sine lacunis. III: discus IV -
 * anguli omissi, XII pixela; III quadratum plenum. IV: actum
 * magnitudo="8". V: aspergillum non sequitur (magnitudo sua, "1").
 * VI: quadratum lineae, palette X optionum (I..LXIV), optio ponit.
 * VII: praevisio - fasciae colore vero, nulla linea accentus. VIII:
 * penicillus I = delineare_lineam ipsa (oraculum; pares Bresenham
 * inclusi) - picturae ante P4 eaedem. */
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

/* attributum ephemerum per dominum suum */
nomen structura {
     constans character* attributum;
                    s32  valor;
} Ponendum;

interior vacuum
ponendum_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    Ponendum* q;

    q = (Ponendum*)ctx;
    insula_attributum_ponere(radix, p, in, q->attributum,
        chorda_ut_cstr(chorda_ex_s32(q->valor, p), p));
}

interior vacuum
ponere (
      PictorApplicatio* app,
    constans character* scriptor,
    constans character* attributum,
                   s32  valor)
{
    InsulaRamus r;
       Ponendum q;

    q.attributum  = attributum;
    q.valor       = valor;
    r             = insula_ramus_radix(app->repo);
    insula_scriptorem_ponere(app->repo, chorda_ex_literis(scriptor,
        piscina));
    (vacuum)mutare_ramum(&r, INSULA_EPHEMERA, ponendum_mutator, &q);
    insula_scriptorem_ponere(app->repo, chorda_ex_literis("", piscina));
}


s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* doc;
                  i32  fundus;
                  i32  atramentum;

    piscina = piscina_generare_dynamicum("probatio_pictor_magnitudo",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_magnitudo");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m           = manus_ludus_creare(piscina, app.d);
    doc         = app.doc;
    fundus      = (i32)color_ad_pixelum(thema_color(COLOR_BACKGROUND));
    atramentum  = palettae(ZEPHYRUM);

    imprimere("\n--- III: discus IV et III ---\n");
    {
        s32 x;
        s32 y;
        i32 n;

        ponere(&app, "magnitudo.ponere", "magnitudo_penicilli", IV);
        ictus(&app, L, CCC, L, CCC, M);
        n = ZEPHYRUM;
        per (y = CCC - V; y <= CCC + V; y++)
        {
            per (x = L - V; x <= L + V; x++)
            {
                si (pixelum(doc, x, y) == atramentum)
                {
                    n++;
                }
            }
        }
        /* [XLVIII, LII) x [CCXCVIII, CCCII) sine angulis IV */
        CREDO_AEQUALIS_I32(n, XII);
        CREDO_AEQUALIS_I32(pixelum(doc, XLVIII, CCXCVIII), fundus);
        CREDO_AEQUALIS_I32(pixelum(doc, LI, CCCI), fundus);
        CREDO_AEQUALIS_I32(pixelum(doc, XLIX, CCXCVIII), atramentum);
        CREDO_FALSUM(pictor_disci_pixelum(IV, ZEPHYRUM, ZEPHYRUM));
        CREDO_VERUM(pictor_disci_pixelum(III, ZEPHYRUM, ZEPHYRUM));
        CREDO_VERUM(pictor_disci_pixelum(I, ZEPHYRUM, ZEPHYRUM));
    }

    imprimere("\n--- I: penicillus VIII, regula exacta ---\n");
    {
        s32 x;
        s32 y;
        s32 cx;
        s32 i;
        s32 j;
        b32 intus;
        i32 mala;

        ponere(&app, "magnitudo.ponere", "magnitudo_penicilli", VIII);
        ictus(&app, C, C, CXL, C, MM);
        mala = ZEPHYRUM;
        per (y = C - X; y <= C + X; y++)
        {
            per (x = C - X; x <= CXL + X; x++)
            {
                intus = FALSUM;
                per (cx = C; cx <= CXL && !intus; cx++)
                {
                    i = x - (cx - IV);
                    j = y - (C - IV);
                    si (   i >= ZEPHYRUM && i < VIII && j >= ZEPHYRUM
                        && j < VIII && pictor_disci_pixelum(VIII, i, j))
                    {
                        intus = VERUM;
                    }
                }
                si ((pixelum(doc, x, y) == atramentum) != intus)
                {
                    mala++;
                }
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    }

    imprimere("\n--- II: tractus celer sine lacunis ---\n");
    {
        s32 t;
        s32 cx;
        s32 cy;
        s32 x;
        s32 y;
        i32 lacunae;

        ictus(&app, CC, CC, CCC, CCLX, MMM);
        lacunae = ZEPHYRUM;
        per (t = ZEPHYRUM; t <= C; t++)
        {
            cx = CC + t;
            cy = CC + (t * LX + L) / C;
            per (y = -III; y <= III; y++)
            {
                per (x = -III; x <= III; x++)
                {
                    /* discus radii III circa lineam (discus VIII in
                     * fine anguli caret) */
                    si (x * x + y * y > IX)
                    {
                        perge;
                    }
                    si (pixelum(doc, cx + x, cy + y) != atramentum)
                    {
                        lacunae++;
                    }
                }
            }
        }
        CREDO_AEQUALIS_I32(lacunae, ZEPHYRUM);
    }

    imprimere("\n--- IV: actum ---\n");
    CREDO_VERUM(chorda_continet(actum_ultimum(vol), chorda_ex_literis(
        "magnitudo=\"8\"", piscina)));

    imprimere("\n--- V: aspergillum magnitudinem suam ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, 'a', ZEPHYRUM));
    ictus(&app, CCC, CCCL, CCCX, CCCL, IV * M);
    CREDO_VERUM(chorda_continet(actum_ultimum(vol), chorda_ex_literis(
        "magnitudo=\"1\"", piscina)));
    CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));

    imprimere("\n--- VI: quadratum, palette, optio ---\n");
    {
        Componens* p;

        dispensator_recomponere(app.d);
        CREDO_NON_NIHIL(nodus_arboris(&app, "quadratum.magnitudo"));
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "quadratum.magnitudo")->titulus, "magnitudo:8");
        /* quadratum quintum: x V x VI + IV x XX = CX */
        CREDO_VERUM(manus_ludus_premere_ad(m, CXX, CDLXVIII));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "magnitudo");
        p = nodus_arboris(&app, "palette");
        CREDO_VERUM(p && xar_numerus(p->liberi) == X);
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "optio.magnitudo.8")->titulus, "magnitudo:8:electum");
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "optio.magnitudo.64")->titulus, "magnitudo:64");
        /* palette XXVIII alta, y CDXXVIII; optio IX (LXIV) */
        CREDO_VERUM(manus_ludus_premere_ad(m, CX + IV + IX * XXII + X,
            CDXXVIII + IV + X));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "magnitudo_penicilli"), "64");
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "");
    }

    imprimere("\n--- VII: praevisio colore vero ---\n");
    {
          Mandata* md;
         Mandatum* x;
              i32  i;
              i32  fasciae;
              i32  accentus;

        ponere(&app, "magnitudo.ponere", "magnitudo_penicilli", VIII);
        mus(app.d, EVENTUS_MUS_DEPRESSUS, XL, XL, V * M);
        mus(app.d, EVENTUS_MUS_MOTUS, LX, XL, V * M + XVI);
        dispensator_recomponere(app.d);
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "tabula")->titulus, "8 0");
        md = mandata_creare(piscina, intern);
        pingere(dispensator_arbor(app.d), app.figurae, ZEPHYRUM, md);
        fasciae   = ZEPHYRUM;
        accentus  = ZEPHYRUM;
        per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
        {
            x = mandata_obtinere(md, i);
            si (   x->genus == MANDATUM_RECTANGULUM && x->impletum
                && x->fines.altitudo == I
                && x->color.genus == COLOR_MANDATI_RGBA
                && x->color.valor == atramentum)
            {
                fasciae++;
            }
            si (   x->genus       == MANDATUM_LINEA
                && x->color.valor == (i32)COLOR_ACCENT_PRIMARY)
            {
                accentus++;
            }
        }
        CREDO_VERUM(fasciae >= VIII);
        CREDO_AEQUALIS_I32(accentus, ZEPHYRUM);
        mus(app.d, EVENTUS_MUS_LIBERATUS, LX, XL, V * M + XXXII);
    }

    imprimere("\n--- VIII: penicillus I = delineare_lineam ---\n");
    {
        /* pares Bresenham utriusque generis: (II:I) gradus y, (I:II)
         * gradus x - probatum ex omnibus declivitatibus */
        hic_manens constans s32 lineae[VIII][IV] = {
            { CCC, X, CCCXL, XXX },
            { CCCXL, XL, CCC, LX },
            { CCC, LXX, CCCXX, LXXX },
            { CCCX, CXX, CCC, LXXXIX },
            { CCC, CXXX, CCCXII, CXLV },
            { CCCL, CL, CCCLXIII, CLVI },
            { CCCLV, X, CCCLX, XX },
            { CCCLX, XL, CCCLXI, LX }
        };
             TabulaPixelorum* oraculum;
         ContextusDelineandi* dc;
                         i32  k;
                         s32  x;
                         s32  y;
                         i32  mala;
                         i32  pictae;

        ponere(&app, "magnitudo.ponere", "magnitudo_penicilli", I);
        oraculum = tabula_pixelorum_creare_nuda(piscina,
            doc->tabula->latitudo, doc->tabula->altitudo);
        dc = delineare_creare_contextum(piscina, oraculum);
        per (k = ZEPHYRUM; k < VIII; k++)
        {
            ictus(&app, lineae[k][ZEPHYRUM], lineae[k][I],
                lineae[k][II],
                lineae[k][III], VI * M + (s64)k * C);
            delineare_lineam(dc, (i32)lineae[k][ZEPHYRUM],
                (i32)lineae[k][I], (i32)lineae[k][II],
                (i32)lineae[k][III], color_ex_palette(ZEPHYRUM));
        }
        mala    = ZEPHYRUM;
        pictae  = ZEPHYRUM;
        per (y = ZEPHYRUM; y < CLXX; y++)
        {
            per (x = CCXC; x < CCCLXX; x++)
            {
                si ((pixelum(doc, x, y) == atramentum)
                    != ((i32)tabula_pixelorum_obtinere_pixelum(oraculum,
                        (i32)x, (i32)y) == atramentum))
                {
                    mala++;
                }
                si (pixelum(doc, x, y) == atramentum)
                {
                    pictae++;
                }
            }
        }
        CREDO_VERUM(pictae > C);
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
