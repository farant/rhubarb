/* probatio_pictor_pinacotheca.c - pinacotheca pictoris (spec
 * project-specs/pictor-pinacotheca-spec.md; plan T4-T7)
 *
 * Applicatio pictoris vera, superficies telephoni (CCXV x CDXIX),
 * volumen temporarium. T4: quadratum ⋯ (elenchus_praesto) et menu
 * (elenchus): ordo rerum, Import solum cum servitio, New picture
 * picturam novam "pictura-<n>" condit et ostendit, Open picture
 * pinacothecam aperit, ictus extra et Esc menu claudunt, sine
 * vexillo nihil. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "elenchus.h"
#include "xar.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO_TELEPHONI  CCXV
#define ALTITUDO_TELEPHONI  CDXIX

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* origo absoluta nodi (summa parentum) */
interior vacuum
origo (
    constans Componens* n,
                   s32* x,
                   s32* y)
{
    *x = ZEPHYRUM;
    *y = ZEPHYRUM;
    dum (n != NIHIL)
    {
        *x  += n->fines.x;
        *y  += n->fines.y;
        n   = n->parens;
    }
}

interior Componens*
nodus_arboris (
    PictorApplicatio* app,
  constans character* id)
{
    redde componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis(id, piscina));
}

interior chorda
ephemera_legere (
    PictorApplicatio* app,
  constans character* titulus)
{
    chorda* a;

    a = insula_attributum(app->repo, titulus);
    redde a ? *a : chorda_ex_literis("", piscina);
}

/* ictus in centro nodi 'id'; FALSUM si nodus abest */
interior b32
premere_nodum (
    PictorApplicatio* app,
          ManusLudus* m,
  constans character* id)
{
     Componens* n;
           s32  x;
           s32  y;

    n = nodus_arboris(app, id);
    si (!n)
    {
        imprimere("  nodus abest: %s\n", id);
        redde FALSUM;
    }
    origo(n, &x, &y);
    (vacuum)manus_ludus_premere_ad(m, x + n->fines.latitudo / II,
        y + n->fines.altitudo / II);
    dispensator_recomponere(app->d);
    redde VERUM;
}

interior i32
numerus_picturarum (
    Volumen* vol)
{
    Xar* l;

    l = pictor_documenta_enumerare(vol, piscina);
    redde l ? xar_numerus(l) : ZEPHYRUM;
}

interior b32
pictura_est (
               Volumen* vol,
    constans character* spatium)
{
    Xar* l;
    i32  i;

    l = pictor_documenta_enumerare(vol, piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (chorda_aequalis_literis(*(chorda*)xar_obtinere(l, i),
                spatium))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* servitium electionis fictum: vocationes numerat */
interior b32
eligere_fictum (
    vacuum* ctx)
{
    *(i32*)ctx += I;
    redde VERUM;
}

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    c->datum = volumen_temporarium(piscina,
        "probatio_pictor_pinacotheca");
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    si (c->datum)
    {
        volumen_claudere((Volumen*)c->datum);
        c->datum = NIHIL;
    }
    piscina  = NIHIL;
    intern   = NIHIL;
}

interior b32
aedificare (
     PictorApplicatio* app,
              Volumen* vol,
                  b32  praesto)
{
    thema_initiare();
    si (!pictor_applicatio_aedificare(app, piscina, intern, vol,
            LATITUDO_TELEPHONI, ALTITUDO_TELEPHONI))
    {
        redde FALSUM;
    }
    app->montatio.compositio.elenchus_praesto = praesto;
    dispensator_recomponere(app->d);
    redde VERUM;
}

/* T4 (spec D1, D2, D11): quadratum ⋯ et menu */
interior vacuum
_sectio_elenchus (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
            Componens* q;
            Componens* s;
            Componens* e;
             Elenchus  el;
          ElenchusRes  res[III];
                  s32  x_q;
                  s32  y_q;
                  s32  x_e;
                  s32  y_e;
                  i32  electiones;

    electiones  = ZEPHYRUM;
    vol         = (Volumen*)c->datum;

    CREDO_GRADUS("sine vexillo: nullum quadratum");
    CREDO_NECESSE(aedificare(&app, vol, FALSUM));
    CREDO_VERUM(nodus_arboris(&app, "quadratum.elenchus") == NIHIL);
    CREDO_VERUM(nodus_arboris(&app, "quadratum.imago") == NIHIL);

    CREDO_GRADUS("quadratum ⋯ ad sinistram stratorum");
    app.montatio.compositio.elenchus_praesto = VERUM;
    dispensator_recomponere(app.d);
    m = manus_ludus_creare(piscina, app.d);
    q = nodus_arboris(&app, "quadratum.elenchus");
    s = nodus_arboris(&app, "quadratum.strata");
    CREDO_NECESSE_NON_NIHIL(q);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_CHORDA_AEQUALIS_LITERIS(q->titulus, "elenchus:quadratum");
    CREDO_CHORDA_AEQUALIS_LITERIS(q->actio, "palette.aperire");
    origo(q, &x_q, &y_q);
    origo(s, &x_e, &y_e);
    CREDO_VERUM(x_q + q->fines.latitudo <= x_e);
    CREDO_AEQUALIS_I32((i32)y_q, (i32)y_e);
    CREDO_VERUM(nodus_arboris(&app, "quadratum.imago") == NIHIL);

    CREDO_GRADUS("menu: duae res, supra quadratum");
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "elenchus");
    e = nodus_arboris(&app, "elenchus");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32(e->liberi ? xar_numerus(e->liberi) : ZEPHYRUM,
        II);
    origo(e, &x_e, &y_e);
    CREDO_VERUM(y_e + e->fines.altitudo <= y_q);
    CREDO_VERUM(x_e >= ZEPHYRUM);
    CREDO_VERUM(x_e + e->fines.latitudo <= LATITUDO_TELEPHONI);

    CREDO_GRADUS("res et textus (spec D2)");
    pictor_elenchum_parare(VERUM, res, &el);
    CREDO_AEQUALIS_I32(el.numerus, III);
    CREDO_VERUM(strcmp(el.res[ZEPHYRUM].titulus, "pinacotheca.aperire")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[I].titulus, "pictura.nova") == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[II].titulus, "imago.eligere")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[ZEPHYRUM].textus,
        "Open picture\xE2\x80\xA6") == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[I].textus, "New picture") == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[II].textus, "Import image\xE2\x80\xA6")
        == ZEPHYRUM);
    pictor_elenchum_parare(FALSUM, res, &el);
    CREDO_AEQUALIS_I32(el.numerus, II);

    CREDO_GRADUS("cum servitio: tres res");
    app.montatio.compositio.imago_eligibilis        = VERUM;
    app.montatio.actiones_ctx.imaginem_eligere      = eligere_fictum;
    app.montatio.actiones_ctx.imaginem_eligere_ctx  = &electiones;
    dispensator_recomponere(app.d);
    e = nodus_arboris(&app, "elenchus");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32(e->liberi ? xar_numerus(e->liberi) : ZEPHYRUM,
        III);

    CREDO_GRADUS("Import image: servitium semel, menu clausum");
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.2"));
    CREDO_AEQUALIS_I32(electiones, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");

    CREDO_GRADUS("ictus extra menu: claudit, nihil pingit");
    {
        i32 vivi;

        vivi = pictor_documentum_numerus_vivorum(app.montatio.doc);
        CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "elenchus");
        CREDO_VERUM(premere_nodum(&app, m, "tabula"));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "");
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(
            app.montatio.doc), vivi);
    }

    CREDO_GRADUS("Esc claudit");
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    dispensator_recomponere(app.d);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");
}

/* T4 (spec D6): New picture - pictura nova vacua "pictura-<n>",
 * ostensa; Open picture - pinacotheca aperta */
interior vacuum
_sectio_nova (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* radix;
     PictorDocumentum* prima;
                  i32  n;

    vol = (Volumen*)c->datum;
    CREDO_NECESSE(aedificare(&app, vol, VERUM));
    m      = manus_ludus_creare(piscina, app.d);
    radix  = app.montatio.doc;
    n      = numerus_picturarum(vol);

    CREDO_GRADUS("New picture: pictura-1 ostensa");
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    CREDO_AEQUALIS_I32(numerus_picturarum(vol), n + I);
    CREDO_VERUM(pictura_est(vol, "pictura-1"));
    prima = app.montatio.doc;
    CREDO_VERUM(prima != NIHIL && prima != radix);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(prima),
        ZEPHYRUM);
    CREDO_VERUM(app.montatio.actiones_ctx.doc == prima);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "pictura"),
        "pictura-1");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");

    CREDO_GRADUS("iterum: pictura-2");
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    CREDO_AEQUALIS_I32(numerus_picturarum(vol), n + II);
    CREDO_VERUM(pictura_est(vol, "pictura-2"));
    CREDO_VERUM(app.montatio.doc != prima);

    CREDO_GRADUS("Open picture: pinacotheca aperta");
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.0"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "pinacotheca");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "T4: quadratum et menu", _sectio_elenchus, _parare, _purgare,
        NIHIL, NIHIL },
    { "T4: New picture, Open picture", _sectio_nova, _parare, _purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_pinacotheca", SECTIONES);
}
