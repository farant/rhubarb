/* probatio_pictor_strata_ui.c - palette stratorum (pictor-strata L3)
 *
 * Applicatio pictoris vera, CDLXXX x CDLXXX. I: quadratum stratorum
 * ("strata:stratum 1") palettam aperit: ordines summo primo, currens
 * notatum. II: '+' stratum novum supra currens, fit currens; palette
 * aperta manet. III: ictus in strato currenti (stratum="2"). IV:
 * oculus occultat (actum), revocatio restituit. V: ordo electus ictus
 * dirigit (stratum I: sine attributo). VI: '-' currens delet, inferius
 * currens; ultimum numquam. VII: Esc palettam claudit. */
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


#define QY CDLXVIII   /* centrum quadratorum lineae status */

/* centrum nodi palettae (filii palettae, fines relativi) */
interior vacuum
premere_in_palette (
           PictorApplicatio* app,
                 ManusLudus* m,
         constans character* id)
{
    Componens* p;
    Componens* c;

    p = nodus_arboris(app, "palette");
    c = nodus_arboris(app, id);
    si (!p || !c)
    {
        imprimere("  nodus abest: %s\n", id);
        redde;
    }
    (vacuum)manus_ludus_premere_ad(m, p->fines.x + c->fines.x
        + c->fines.latitudo / II, p->fines.y + c->fines.y
        + c->fines.altitudo / II);
    dispensator_recomponere(app->d);
}

/* centrum quadrati stratorum */
interior vacuum
premere_quadratum (
           PictorApplicatio* app,
                 ManusLudus* m,
         constans character* id)
{
    Componens* s;
    Componens* c;

    /* quadratum stratorum filius radicis (fines absoluti) */
    s = nodus_arboris(app, "radix");
    c = nodus_arboris(app, id);
    si (!s || !c)
    {
        imprimere("  nodus abest: %s\n", id);
        redde;
    }
    (vacuum)manus_ludus_premere_ad(m, s->fines.x + c->fines.x
        + c->fines.latitudo / II, s->fines.y + c->fines.y
        + c->fines.altitudo / II);
    dispensator_recomponere(app->d);
}

interior b32
titulus_est (
           PictorApplicatio* app,
         constans character* id,
         constans character* titulus)
{
    Componens* c;

    c = nodus_arboris(app, id);
    redde c && chorda_aequalis_literis(c->titulus, titulus);
}

s32 principale (vacuum)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* doc;

    piscina = piscina_generare_dynamicum("probatio_pictor_strata_ui",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_strata_ui");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX));
    m    = manus_ludus_creare(piscina, app.d);
    doc  = app.doc;
    dispensator_recomponere(app.d);

    imprimere("\n--- I: quadratum, palette ---\n");
    CREDO_VERUM(titulus_est(&app, "quadratum.strata",
        "strata:stratum 1"));
    premere_quadratum(&app, m, "quadratum.strata");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "strata");
    CREDO_VERUM(titulus_est(&app, "stratum.1", "stratum:1:electum"));
    CREDO_VERUM(titulus_est(&app, "oculus.1", "oculus:1"));
    CREDO_VERUM(titulus_est(&app, "strata.novum", "strata:novum"));
    CREDO_VERUM(titulus_est(&app, "strata.deletum", "strata:deletum"));

    imprimere("\n--- II: '+' ---\n");
    premere_in_palette(&app, m, "strata.novum");
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
        "stratum_activum"), "2");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "strata");
    CREDO_VERUM(titulus_est(&app, "stratum.2", "stratum:2:electum"));
    CREDO_VERUM(titulus_est(&app, "stratum.1", "stratum:1"));
    /* summum primum: ordo strati 2 supra ordinem strati 1 */
    CREDO_VERUM(   nodus_arboris(&app, "stratum.2")
                && nodus_arboris(&app, "stratum.1")
                && nodus_arboris(&app, "stratum.2")->fines.y
                   < nodus_arboris(&app, "stratum.1")->fines.y);
    CREDO_VERUM(titulus_est(&app, "quadratum.strata",
        "strata:stratum 2"));

    imprimere("\n--- III: ictus in strato currenti ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");
    premere(app.d, L, L, M, ZEPHYRUM);
    CREDO_VERUM(chorda_continet(actum_ultimum(vol), chorda_ex_literis(
        "stratum=\"2\"", piscina)));
    CREDO_AEQUALIS_I32(pixelum(doc, L, L), palettae(ZEPHYRUM));

    imprimere("\n--- IV: oculus ---\n");
    premere_quadratum(&app, m, "quadratum.strata");
    premere_in_palette(&app, m, "oculus.2");
    CREDO_VERUM(chorda_continet(actum_ultimum(vol), chorda_ex_literis(
        "actio=\"visibile\" id=\"2\" valor=\"0\"", piscina)));
    CREDO_VERUM(titulus_est(&app, "oculus.2", "oculus:0"));
    CREDO_AEQUALIS_I32(pixelum(doc, L, L),
        (i32)color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
    CREDO_VERUM(pictor_documentum_revocare(doc));
    dispensator_recomponere(app.d);
    CREDO_VERUM(titulus_est(&app, "oculus.2", "oculus:1"));
    CREDO_AEQUALIS_I32(pixelum(doc, L, L), palettae(ZEPHYRUM));

    imprimere("\n--- V: ordo electus ---\n");
    premere_in_palette(&app, m, "stratum.1");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
        "stratum_activum"), "1");
    CREDO_VERUM(titulus_est(&app, "stratum.1", "stratum:1:electum"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    premere(app.d, C, C, MM, ZEPHYRUM);
    CREDO_FALSUM(chorda_continet(actum_ultimum(vol), chorda_ex_literis(
        "stratum=", piscina)));
    CREDO_VERUM(titulus_est(&app, "quadratum.strata",
        "strata:stratum 1"));

    imprimere("\n--- VI: '-' ---\n");
    premere_quadratum(&app, m, "quadratum.strata");
    premere_in_palette(&app, m, "stratum.2");
    premere_in_palette(&app, m, "strata.deletum");
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
        "stratum_activum"), "1");
    CREDO_VERUM(nodus_arboris(&app, "stratum.2") == NIHIL);
    premere_in_palette(&app, m, "strata.deletum");
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "strata");

    imprimere("\n--- VII: Esc claudit ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
