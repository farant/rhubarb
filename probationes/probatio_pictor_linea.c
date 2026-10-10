/* probatio_pictor_linea.c - instrumentum linea (Franus)
 *
 * Applicatio pictoris vera (sola, volumen temporarium). I: 'l' linea,
 * palette instrumentorum IV optionum. II: ictus primus initium figit,
 * motus sine botone praevisionem movet colore vero, nihil scriptum.
 * III: ictus secundus segmentum scribit (puncta II; pixela =
 * delineare_lineam, oraculum), captura soluta. IV: Shift+ictus -
 * series: punctum initium novum. V: Esc pendentem abicit. VI: ictus
 * extra tabulam abicit (palette non aperitur). VII: latitudo VIII -
 * actum, penicillus intactus; quadratum et palette lineae (V). VIII:
 * instrumentum mutatum pendentem abicit. */
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

/* eventus muris ad punctum TABULAE (x, y) tempore t, modificantibus */
interior vacuum
mus (
      Dispensator* d,
  eventus_genus_t  genus,
              s32  x,
              s32  y,
              s64  t,
              i32  modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                   = genus;
    e.tempus                  = t;
    e.datum.mus.x             = ORIGO_X + x;
    e.datum.mus.y             = ORIGO_Y + y;
    e.datum.mus.botton        = MUS_SINISTER;
    e.datum.mus.modificantes  = modificantes;
    dispensator_tractare(d, &e);
}

/* ictus (pressio + solutio) ad punctum tabulae */
interior vacuum
premere (
    Dispensator* d,
            s32  x,
            s32  y,
            s64  t,
            i32  modificantes)
{
    mus(d, EVENTUS_MUS_DEPRESSUS, x, y, t, modificantes);
    mus(d, EVENTUS_MUS_LIBERATUS, x, y, t + X, modificantes);
}

/* motus sine botone */
interior vacuum
movere (
    Dispensator* d,
            s32  x,
            s32  y,
            s64  t)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_MOTUS;
    e.tempus       = t;
    e.datum.mus.x  = ORIGO_X + x;
    e.datum.mus.y  = ORIGO_Y + y;
    dispensator_tractare(d, &e);
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


#define QY CDLXVIII   /* centrum quadratorum lineae status */

interior i32
ictus_vivi (
    PictorDocumentum* doc)
{
    redde pictor_documentum_numerus_vivorum(doc);
}

interior b32
pendens (
    PictorApplicatio* app)
{
    redde !chorda_vacua(dispensator_motus(app->d)->captura);
}

s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* doc;
                  i32  atramentum;

    piscina = piscina_generare_dynamicum("probatio_pictor_linea",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_linea");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m           = manus_ludus_creare(piscina, app.d);
    doc         = app.doc;
    atramentum  = palettae(ZEPHYRUM);

    imprimere("\n--- I: 'l' linea ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, 'l', ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "instrumentum"),
        "linea");
    dispensator_recomponere(app.d);
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app, "tabula")->actio,
        "linea.ictus");
    CREDO_VERUM(manus_ludus_premere_ad(m, XVI, QY));
    CREDO_VERUM(nodus_arboris(&app, "palette")
        && xar_numerus(nodus_arboris(&app, "palette")->liberi) == IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "optio.instrumentum.linea")->titulus,
        "instrumentum:linea:electum");
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));

    imprimere("\n--- II: initium, praevisio, nihil scriptum ---\n");
    {
          Mandata* md;
         Mandatum* x;
        Componens* t;
              i32  i;
              i32  fasciae;

        premere(app.d, L, L, M, ZEPHYRUM);
        CREDO_VERUM(pendens(&app));
        movere(app.d, CL, LXXX, M + C);
        dispensator_recomponere(app.d);
        t = nodus_arboris(&app, "tabula");
        CREDO_AEQUALIS_I32(t->numerus_punctorum, II);
        CREDO_VERUM(t->numerus_punctorum == II && t->puncta[I].x == CL
            && t->puncta[I].y == LXXX);
        CREDO_CHORDA_AEQUALIS_LITERIS(t->titulus, "1 0");
        md = mandata_creare(piscina, intern);
        pingere(dispensator_arbor(app.d), app.figurae, ZEPHYRUM, md);
        fasciae = ZEPHYRUM;
        per (i = ZEPHYRUM; i < mandata_numerus(md); i++)
        {
            x = mandata_obtinere(md, i);
            /* disci I praevisionis (non quadrata lineae status) */
            si (   x->genus == MANDATUM_RECTANGULUM && x->impletum
                && x->fines.latitudo == I && x->fines.altitudo == I
                && x->color.genus == COLOR_MANDATI_RGBA
                && x->color.valor == atramentum)
            {
                fasciae++;
            }
        }
        /* L..CL: punctum primum + C gradus */
        CREDO_AEQUALIS_I32(fasciae, CI);
        CREDO_AEQUALIS_I32(ictus_vivi(doc), ZEPHYRUM);
    }

    imprimere("\n--- III: ictus secundus - segmentum ---\n");
    {
             TabulaPixelorum* oraculum;
         ContextusDelineandi* dc;
                      chorda  a;
                         s32  x;
                         s32  y;
                         i32  mala;

        premere(app.d, CL, LXXX, M + CC, ZEPHYRUM);
        CREDO_AEQUALIS_I32(ictus_vivi(doc), I);
        CREDO_FALSUM(pendens(&app));
        a = actum_ultimum(vol);
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "instrumentum=\"linea\"", piscina)));
        CREDO_AEQUALIS_I32(chorda_numerare_occurrentia(a,
            chorda_ex_literis("<punctum", piscina)), II);
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "x=\"50\" y=\"50\"", piscina)));
        CREDO_VERUM(chorda_continet(a, chorda_ex_literis(
            "x=\"150\" y=\"80\"", piscina)));
        oraculum = tabula_pixelorum_creare_nuda(piscina,
            doc->tabula->latitudo, doc->tabula->altitudo);
        dc = delineare_creare_contextum(piscina, oraculum);
        delineare_lineam(dc, L, L, CL, LXXX,
            color_ex_palette(ZEPHYRUM));
        mala = ZEPHYRUM;
        per (y = XL; y < XC; y++)
        {
            per (x = XL; x < CLX; x++)
            {
                si ((pixelum(doc, x, y) == atramentum)
                    != ((i32)tabula_pixelorum_obtinere_pixelum(oraculum,
                        (i32)x, (i32)y) == atramentum))
                {
                    mala++;
                }
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
    }

    imprimere("\n--- IV: Shift - series ---\n");
    premere(app.d, L, CL, MM, ZEPHYRUM);
    movere(app.d, C, CL, MM + C);
    premere(app.d, C, CL, MM + CC, MOD_SHIFT);
    CREDO_AEQUALIS_I32(ictus_vivi(doc), II);
    CREDO_VERUM(pendens(&app));
    dispensator_recomponere(app.d);
    CREDO_VERUM(   nodus_arboris(&app, "tabula")->numerus_punctorum >= I
                && nodus_arboris(&app, "tabula")->puncta[ZEPHYRUM].x
                    == C
                && nodus_arboris(&app, "tabula")->puncta[ZEPHYRUM].y
                    == CL);
    movere(app.d, C, CC, MM + CCC);
    premere(app.d, C, CC, MM + CD, ZEPHYRUM);
    CREDO_AEQUALIS_I32(ictus_vivi(doc), III);
    CREDO_FALSUM(pendens(&app));
    CREDO_VERUM(chorda_continet(actum_ultimum(vol), chorda_ex_literis(
        "x=\"100\" y=\"150\"/><punctum x=\"100\" y=\"200\"",
        piscina)));
    CREDO_AEQUALIS_I32(pixelum(doc, C, CLXXV), atramentum);

    imprimere("\n--- V: Esc abicit ---\n");
    premere(app.d, CC, L, MMM, ZEPHYRUM);
    movere(app.d, CCL, L, MMM + C);
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_FALSUM(pendens(&app));
    CREDO_AEQUALIS_I32(ictus_vivi(doc), III);
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(nodus_arboris(&app, "tabula")->numerus_punctorum,
        ZEPHYRUM);

    imprimere("\n--- VI: ictus extra tabulam abicit ---\n");
    premere(app.d, CC, C, IV * M, ZEPHYRUM);
    CREDO_VERUM(pendens(&app));
    /* quadratum instrumenti in linea status (extra tabulam) */
    CREDO_VERUM(manus_ludus_premere_ad(m, XVI, QY));
    CREDO_FALSUM(pendens(&app));
    CREDO_AEQUALIS_I32(ictus_vivi(doc), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");

    imprimere("\n--- VII: latitudo VIII ---\n");
    ponere(&app, "magnitudo.ponere", "magnitudo_lineae", VIII);
    premere(app.d, CCC, CCC, V * M, ZEPHYRUM);
    premere(app.d, CD, CCCXX, V * M + C, ZEPHYRUM);
    CREDO_VERUM(chorda_continet(actum_ultimum(vol), chorda_ex_literis(
        "magnitudo=\"8\"", piscina)));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
        "magnitudo_penicilli"), "1");
    dispensator_recomponere(app.d);
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "quadratum.magnitudo")->titulus, "magnitudo:linea:8");
    CREDO_VERUM(manus_ludus_premere_ad(m, CXX, QY));
    CREDO_VERUM(nodus_arboris(&app, "palette")
        && xar_numerus(nodus_arboris(&app, "palette")->liberi) == V);
    CREDO_CHORDA_AEQUALIS_LITERIS(nodus_arboris(&app,
        "optio.magnitudo.16")->titulus, "magnitudo:linea:16");
    /* optio IV (XVI): x CX + IV + IV x XXII + X, y CDXLII */
    CREDO_VERUM(manus_ludus_premere_ad(m, CX + IV + IV * XXII + X,
        CDXXVIII + IV + X));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
        "magnitudo_lineae"), "16");

    imprimere("\n--- VIII: instrumentum mutatum abicit ---\n");
    premere(app.d, CC, CCC, VI * M, ZEPHYRUM);
    CREDO_VERUM(pendens(&app));
    CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));
    CREDO_FALSUM(pendens(&app));
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(nodus_arboris(&app, "tabula")->numerus_punctorum,
        ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
