/* probatio_pictor_exemplar.c - exemplaria (P3) in pictore
 *
 * Applicatio pictoris vera (sola, volumen temporarium). Exemplar
 * TESSELLATUM (III) ad coordinatas TABULAE. I: penicillus, color
 * secundus nullus - bitus positi colorem primum, ceteri intacti. II:
 * color secundus III - ceteri colorem secundum. III: actum attributa
 * non ordinaria sola fert. IV: color primus nullus, solidus - nihil
 * pingitur. V: guttae aspergilli bitum suum sequuntur. VI:
 * reproiectio eadem. VII: quadratum lineae, palette XXXVIII optionum,
 * optio exemplar ponit; praevisio guttarum colore exemplaris. */
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
#include "exemplaria.h"
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
#define TESSELLA ((i32)EXEMPLAR_TESSELLATUM)

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

/* rectangula exemplaris (exemplar != 0) coloris (genus, valor) */
interior i32
exemplaria_picta (
     PictorApplicatio* app,
    ColorMandatiGenus  genus,
                  i32  valor)
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
        si (   x->genus       == MANDATUM_RECTANGULUM
            && x->exemplar    != ZEPHYRUM
            && x->color.genus == genus && x->color.valor == valor)
        {
            n++;
        }
    }
    redde n;
}

/* status: exemplar, color primus, color secundus */
interior vacuum
statum_ponere (
    PictorApplicatio* app,
                 s32  exemplar,
                 s32  primus,
                 s32  secundus)
{
    ponere(app, "exemplar.ponere", "exemplar", exemplar);
    ponere(app, "color_primus.ponere", "color_primus", primus);
    ponere(app, "color_secundus.ponere", "color_secundus", secundus);
}

s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* doc;
                  i32  fundus;

    piscina = piscina_generare_dynamicum("probatio_pictor_exemplar",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_exemplar");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m       = manus_ludus_creare(piscina, app.d);
    doc     = app.doc;
    fundus  = (i32)color_ad_pixelum(thema_color(COLOR_BACKGROUND));

    imprimere("\n--- I: penicillus, secundus nullus ---\n");
    {
        s32 x;
        i32 mala;

        statum_ponere(&app, ZEPHYRUM, V, -I);
        ictus(&app, XX, C, CCC, C, M);
        CREDO_AEQUALIS_I32(pixelum(doc, XXI, C), palettae(V));
        statum_ponere(&app, (s32)TESSELLA, ZEPHYRUM, -I);
        ictus(&app, L, C, CCL, C, MM);
        mala = ZEPHYRUM;
        per (x = L; x <= CCL; x++)
        {
            si (pixelum(doc, x, C) != (exemplar_punctum(TESSELLA, x, C)
                ? palettae(ZEPHYRUM) : palettae(V)))
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
        CREDO_AEQUALIS_I32(pixelum(doc, XL, C), palettae(V));
        CREDO_AEQUALIS_I32(pixelum(doc, CCLXX, C), palettae(V));
    }

    imprimere("\n--- II: secundus III ---\n");
    {
        s32 x;
        i32 mala;

        statum_ponere(&app, (s32)TESSELLA, ZEPHYRUM, III);
        ictus(&app, L, CXX, CCL, CXX, MMM);
        mala = ZEPHYRUM;
        per (x = L; x <= CCL; x++)
        {
            si (pixelum(doc, x, CXX) != (exemplar_punctum(TESSELLA, x,
                CXX) ? palettae(ZEPHYRUM) : palettae(III)))
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
        CREDO_AEQUALIS_I32(pixelum(doc, XL, CXX), fundus);
    }

    imprimere("\n--- III: actum ---\n");
    {
        chorda a;

        a = actum_ultimum(vol);
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "exemplar=\"3\"", piscina)));
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "color_secundus=\"3\"", piscina)));
        statum_ponere(&app, ZEPHYRUM, ZEPHYRUM, -I);
        ictus(&app, L, CXXX, LX, CXXX, IV * M);
        a = actum_ultimum(vol);
        CREDO_FALSUM(chorda_continet(a, chorda_ex_literis("exemplar=",
            piscina)));
        CREDO_FALSUM(chorda_continet(a, chorda_ex_literis(
            "color_secundus=", piscina)));
    }

    imprimere("\n--- IV: primus nullus, solidus ---\n");
    {
        i32* ante;

        ante = photographia(doc);
        statum_ponere(&app, ZEPHYRUM, -I, III);
        ictus(&app, XX, CXL, CCC, CXL, V * M);
        CREDO_VERUM(photographia_eadem(doc, ante));
        CREDO_VERUM(chorda_continet(actum_ultimum(vol),
            chorda_ex_literis("color=\"-1\"", piscina)));
    }

    imprimere("\n--- V: guttae exemplar sequuntur ---\n");
    {
        s32 x;
        s32 y;
        i32 mala;
        i32 primae;
        i32 secundae;
        i32 p;

        CREDO_VERUM(manus_ludus_clavem(m, 'a', ZEPHYRUM));
        statum_ponere(&app, (s32)TESSELLA, ZEPHYRUM, -I);
        ictus(&app, CC, CCC, CCXX, CCC, VI * M);
        mala    = ZEPHYRUM;
        primae  = ZEPHYRUM;
        per (y = CCC - XX; y <= CCC + XX; y++)
        {
            per (x = CC - XX; x <= CCXX + XX; x++)
            {
                p = pixelum(doc, x, y);
                si (p == fundus)
                {
                    perge;
                }
                si (   p != palettae(ZEPHYRUM)
                    || !exemplar_punctum(TESSELLA, x, y))
                {
                    mala++;
                }
                primae++;
            }
        }
        CREDO_VERUM(primae > ZEPHYRUM);
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
        statum_ponere(&app, (s32)TESSELLA, ZEPHYRUM, III);
        ictus(&app, CC, CCCL, CCXX, CCCL, VII * M);
        mala      = ZEPHYRUM;
        secundae  = ZEPHYRUM;
        per (y = CCCL - XX; y <= CCCL + XX; y++)
        {
            per (x = CC - XX; x <= CCXX + XX; x++)
            {
                p = pixelum(doc, x, y);
                si (p == fundus)
                {
                    perge;
                }
                si (p == palettae(III))
                {
                    secundae++;
                    si (exemplar_punctum(TESSELLA, x, y))
                    {
                        mala++;
                    }
                }
                alioquin si (   p != palettae(ZEPHYRUM)
                             || !exemplar_punctum(TESSELLA, x, y))
                {
                    mala++;
                }
            }
        }
        CREDO_VERUM(secundae > ZEPHYRUM);
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    }

    imprimere("\n--- VI: reproiectio eadem ---\n");
    {
                     i32* ante;
        PictorDocumentum* alter;

        ante = photographia(doc);
        CREDO_VERUM(pictor_documentum_verificare(doc));
        alter = pictor_documentum_aperire(piscina, intern, vol, "");
        CREDO_NON_NIHIL(alter);
        si (alter)
        {
            CREDO_VERUM(photographia_eadem(alter, ante));
        }
    }

    imprimere("\n--- VII: quadratum, palette, praevisio ---\n");
    {
        Componens* p;
        Componens* t;
          Mandata* md;
         Mandatum* x;
           longus  semen;
          integer  radius;
              s32  dx;
              s32  dy;
              i32  i;
              i32  k;
              i32  exspectatae;
              i32  primae;
              i32  secundae;

        statum_ponere(&app, (s32)TESSELLA, ZEPHYRUM, -I);
        dispensator_recomponere(app.d);
        CREDO_NON_NIHIL(nodus_arboris(&app, "quadratum.exemplar"));
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "quadratum.exemplar")->titulus, "exemplar:3:0:-1");
        /* quadratum exemplaris: x IV x VI + III x XX = LXXXIV */
        CREDO_VERUM(manus_ludus_premere_ad(m, XCIV, CDLXVIII));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "exemplar");
        p = nodus_arboris(&app, "palette");
        CREDO_NON_NIHIL(p);
        CREDO_VERUM(p && xar_numerus(p->liberi)
            == (i32)EXEMPLAR_NUMERUS);
        /* Franus: optiones coloribus electis */
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "optio.exemplar.3")->titulus, "exemplar:3:0:-1:electum");
        CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
            "optio.exemplar.10")->titulus, "exemplar:10:0:-1");
        /* optiones I..XXXVII + quadratum (III) colore primo; nulla
         * 1-bit */
        CREDO_AEQUALIS_I32(exemplaria_picta(&app, COLOR_MANDATI_RGBA,
            palettae(ZEPHYRUM)), XXXVIII);
        CREDO_AEQUALIS_I32(exemplaria_picta(&app, COLOR_MANDATI_THEMA,
            (i32)COLOR_TEXT), ZEPHYRUM);
        /* ambo nulli: 1-bit colore textus, ut legibilia maneant */
        statum_ponere(&app, (s32)TESSELLA, -I, -I);
        dispensator_recomponere(app.d);
        CREDO_AEQUALIS_I32(exemplaria_picta(&app, COLOR_MANDATI_THEMA,
            (i32)COLOR_TEXT), XXXVIII);
        statum_ponere(&app, (s32)TESSELLA, ZEPHYRUM, -I);
        dispensator_recomponere(app.d);
        /* palette X per lineam, IV lineae (XCIV alta), y CCCLXII:
         * optio X = linea II, columna I */
        CREDO_VERUM(manus_ludus_premere_ad(m, LXXXIV + IV + X,
            CCCLXII + IV + XXII + X));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "exemplar"),
            "10");
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "");

        /* praevisio guttarum: colore exemplaris (secundus III) */
        statum_ponere(&app, (s32)TESSELLA, ZEPHYRUM, III);
        mus(app.d, EVENTUS_MUS_DEPRESSUS, CC, CC, VIII * M);
        mus(app.d, EVENTUS_MUS_MOTUS, CCX, CC, VIII * M + XVI);
        dispensator_recomponere(app.d);
        t = nodus_arboris(&app, "tabula");
        CREDO_VERUM(chorda_continet(t->titulus, chorda_ex_literis(
            " 8 0 3 3", piscina)));
        semen   = ZEPHYRUM;
        radius  = ZEPHYRUM;
        (vacuum)sscanf(chorda_ut_cstr(t->titulus, piscina), "%ld %d",
            &semen, &radius);
        exspectatae = ZEPHYRUM;
        per (i = ZEPHYRUM; i < t->numerus_punctorum; i++)
        {
            per (k = ZEPHYRUM; k < PICTOR_GUTTAE_PUNCTO; k++)
            {
                pictor_gutta((s64)semen, i, k, (s32)radius, &dx, &dy);
                si (exemplar_punctum(TESSELLA, t->puncta[i].x + dx,
                    t->puncta[i].y + dy))
                {
                    exspectatae++;
                }
            }
        }
        md = mandata_creare(piscina, intern);
        pingere(dispensator_arbor(app.d), app.figurae, ZEPHYRUM, md);
        primae    = ZEPHYRUM;
        secundae  = ZEPHYRUM;
        per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
        {
            x = mandata_obtinere(md, i);
            si (   x->genus          != MANDATUM_RECTANGULUM
                || x->fines.latitudo != I
                || x->fines.altitudo != I
                || x->color.genus    != COLOR_MANDATI_RGBA)
            {
                perge;
            }
            si (x->color.valor == palettae(ZEPHYRUM))
            {
                primae++;
            }
            si (x->color.valor == palettae(III))
            {
                secundae++;
            }
        }
        CREDO_AEQUALIS_I32(primae, exspectatae);
        CREDO_AEQUALIS_I32(primae + secundae,
            II * PICTOR_GUTTAE_PUNCTO);
        mus(app.d, EVENTUS_MUS_LIBERATUS, CCX, CC, VIII * M + XXXII);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
