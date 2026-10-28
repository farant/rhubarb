/* probatio_pictor_pinacotheca.c - pinacotheca pictoris (spec
 * project-specs/pictor-pinacotheca-spec.md; plan T4-T7)
 *
 * Applicatio pictoris vera, superficies telephoni (CCXV x CDXIX),
 * volumen temporarium. T4: quadratum ⋯ (elenchus_praesto) et menu
 * (elenchus): ordo rerum, Import solum cum servitio, New picture
 * picturam novam (ULID, plan-5 T3) condit et ostendit, Open picture
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
#include "color.h"
#include "lectiones.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "pictor_figurae.h"
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

/* plan-5 T3 (D9): spatium picturae novae = ULID (XXVI Crockford) */
interior b32
ulid_est (
    chorda s)
{
    hic_manens constans character CROCKFORD[] =
        "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
    i32 i;

    si (s.mensura != XXVI)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < XXVI; i++)
    {
        si (!strchr(CROCKFORD, (integer)s.datum[i]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* copia campi (insula mutatur postea) */
interior chorda
pictura_ostensa (
    PictorApplicatio* app)
{
    redde chorda_ex_literis(chorda_ut_cstr(ephemera_legere(app,
        "pictura"), piscina), piscina);
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
    /* ut apps/pictor/pictor.c: pictura ultima (D8) post aedificare */
    (vacuum)pictor_picturam_restituere(app);
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
                 ElenchusRes  res[V];
    PictorElenchiCondiciones  k;
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
    memset(&k, ZEPHYRUM, magnitudo(k));
    k.importare = VERUM;
    pictor_elenchum_parare(&k, res, &el);
    CREDO_AEQUALIS_I32(el.numerus, III);
    CREDO_VERUM(strcmp(el.res[ZEPHYRUM].titulus, "pinacotheca.aperire")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[I].titulus, "pictura.nova") == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[II].titulus, "imago.eligere")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[ZEPHYRUM].textus,
        "My pictures\xE2\x80\xA6") == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[I].textus, "New picture") == ZEPHYRUM);
    CREDO_VERUM(strcmp(el.res[II].textus, "Import image\xE2\x80\xA6")
        == ZEPHYRUM);
    k.importare = FALSUM;
    pictor_elenchum_parare(&k, res, &el);
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

/* T4 (spec D6; plan-5 D9): New picture - pictura nova vacua (ULID),
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
                chorda  spatium_primum;
                   i32  n;

    vol = (Volumen*)c->datum;
    CREDO_NECESSE(aedificare(&app, vol, VERUM));
    m      = manus_ludus_creare(piscina, app.d);
    radix  = app.montatio.doc;
    n      = numerus_picturarum(vol);

    CREDO_GRADUS("New picture: pictura nova (ULID) ostensa");
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    CREDO_AEQUALIS_I32(numerus_picturarum(vol), n + I);
    spatium_primum = pictura_ostensa(&app);
    CREDO_VERUM(ulid_est(spatium_primum));
    CREDO_VERUM(pictura_est(vol, chorda_ut_cstr(spatium_primum,
        piscina)));
    prima = app.montatio.doc;
    CREDO_VERUM(prima != NIHIL && prima != radix);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(prima),
        ZEPHYRUM);
    CREDO_VERUM(app.montatio.actiones_ctx.doc == prima);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");

    CREDO_GRADUS("iterum: ULID alia");
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    CREDO_AEQUALIS_I32(numerus_picturarum(vol), n + II);
    CREDO_VERUM(ulid_est(pictura_ostensa(&app)));
    CREDO_FALSUM(chorda_aequalis(pictura_ostensa(&app),
        spatium_primum));
    CREDO_VERUM(pictura_est(vol, chorda_ut_cstr(pictura_ostensa(&app),
        piscina)));
    CREDO_VERUM(app.montatio.doc != prima);

    CREDO_GRADUS("Open picture: pinacotheca aperta");
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.0"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "pinacotheca");
}

/* ===== T5: pinacotheca ===== */

/* picturae "p0".."p<n-1>" (actum unum quaeque), ante applicationem */
interior vacuum
picturas_facere (
    Volumen* vol,
        i32  n)
{
     PictorDocumentum* d;
            character  spatium[XVI];
                  i32  k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        sprintf(spatium, "p%lu", (insignatus longus)k);
        d = pictor_documentum_creare(piscina, intern, vol, spatium, C,
            CC, LXIV);
        si (d)
        {
            (vacuum)pictor_documentum_actum(d, chorda_ex_literis(
                "<ictus instrumentum=\"penicillus\" color=\"0\""
                " magnitudo=\"1\"><punctum x=\"2\" y=\"2\"/></ictus>",
                piscina));
        }
    }
}

interior b32
pinacothecam_aperire (
    PictorApplicatio* app,
          ManusLudus* m)
{
    redde premere_nodum(app, m, "quadratum.elenchus")
        && premere_nodum(app, m, "elenchus.0");
}

/* "pinax:<lat>x<alt>:<seq>:<spatium>[:electum]" -> spatium */
interior chorda
spatium_pinacis (
    chorda titulus)
{
    chorda s;
       i32 i;
       i32 signa;

    s.datum    = titulus.datum;
    s.mensura  = ZEPHYRUM;
    signa      = ZEPHYRUM;
    per (i = ZEPHYRUM; i < titulus.mensura; i++)
    {
        si (titulus.datum[i] == ':' && ++signa == III)
        {
            s.datum    = titulus.datum + i + I;
            s.mensura  = titulus.mensura - i - I;
            frange;
        }
    }
    si (   s.mensura >= VIII
        && memcmp(s.datum + s.mensura - VIII, ":electum", VIII)
           == ZEPHYRUM)
    {
        s.mensura -= VIII;
    }
    redde s;
}

interior chorda
titulus_nodi (
    PictorApplicatio* app,
  constans character* id)
{
    Componens* n;

    n = nodus_arboris(app, id);
    redde n ? n->titulus : chorda_ex_literis("(abest)", piscina);
}

/* pinax.<k> ad spatium in 'id'; FALSUM si nullus */
interior b32
pinax_spatii (
     PictorApplicatio* app,
   constans character* spatium,
            character  id[XVI])
{
    i32 k;

    per (k = ZEPHYRUM; k < XII; k++)
    {
        sprintf(id, "pinax.%lu", (insignatus longus)k);
        si (   nodus_arboris(app, id)
            && chorda_aequalis_literis(spatium_pinacis(titulus_nodi(app,
                   id)), spatium))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* rete: k columnae (pinax.0..k-1 eadem linea, pinax.k infra),
 * omnia intra superficiem, disiuncta */
interior b32
rete_est (
     PictorApplicatio* app,
                  i32  columnae,
                  s32  latitudo,
                  s32  altitudo)
{
     Componens* a;
     Componens* b;
           s32  x_a;
           s32  y_a;
           s32  x_b;
           s32  y_b;
           i32  i;
           i32  j;
     character  id[XVI];
     character  id2[XVI];

    per (i = ZEPHYRUM; i < XII; i++)
    {
        sprintf(id, "pinax.%lu", (insignatus longus)i);
        a = nodus_arboris(app, id);
        si (!a)
        {
            redde FALSUM;
        }
        origo(a, &x_a, &y_a);
        si (   x_a < ZEPHYRUM || y_a < ZEPHYRUM
            || x_a + a->fines.latitudo > latitudo
            || y_a + a->fines.altitudo > altitudo)
        {
            redde FALSUM;
        }
        per (j = i + I; j < XII; j++)
        {
            sprintf(id2, "pinax.%lu", (insignatus longus)j);
            b = nodus_arboris(app, id2);
            si (!b)
            {
                redde FALSUM;
            }
            origo(b, &x_b, &y_b);
            si (!(   x_a + a->fines.latitudo <= x_b
                  || x_b + b->fines.latitudo <= x_a
                  || y_a + a->fines.altitudo <= y_b
                  || y_b + b->fines.altitudo <= y_a))
            {
                redde FALSUM;
            }
            /* eadem linea sicut i ssi eodem ordine retis */
            si ((i / columnae == j / columnae) != (y_a == y_b))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

/* T5 (spec D3, D5, D7): rete telephoni III x IV, ordo recentiae,
 * pictura aperta notata */
interior vacuum
_sectio_rete (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
                  Xar* l;
                  i32  k;
            character  id[XVI];

    vol = (Volumen*)c->datum;
    picturas_facere(vol, XII);
    CREDO_NECESSE(aedificare(&app, vol, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(pinacothecam_aperire(&app, m));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "pinacotheca");
    CREDO_NON_NIHIL(nodus_arboris(&app, "pinacotheca"));

    CREDO_GRADUS("XII pinaces, ordo recentiae");
    l = pictor_documenta_recentia(vol, piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), XIII);
    per (k = ZEPHYRUM; k < XII; k++)
    {
        sprintf(id, "pinax.%lu", (insignatus longus)k);
        CREDO_VERUM(chorda_aequalis(spatium_pinacis(titulus_nodi(&app,
            id)), ((PictorPictura*)xar_obtinere(l, k))->spatium));
    }
    CREDO_VERUM(nodus_arboris(&app, "pinax.12") == NIHIL);

    CREDO_GRADUS("pictura aperta notata");
    CREDO_CHORDA_AEQUALIS_LITERIS(spatium_pinacis(titulus_nodi(&app,
        "pinax.0")), "");
    {
        chorda t;

        t = titulus_nodi(&app, "pinax.0");
        CREDO_VERUM(t.mensura > VIII && memcmp(t.datum + t.mensura
            - VIII, ":electum", VIII) == ZEPHYRUM);
        t = titulus_nodi(&app, "pinax.1");
        CREDO_FALSUM(t.mensura > VIII && memcmp(t.datum + t.mensura
            - VIII, ":electum", VIII) == ZEPHYRUM);
    }

    CREDO_GRADUS("III x IV in superficie telephoni");
    CREDO_VERUM(rete_est(&app, III, LATITUDO_TELEPHONI,
        ALTITUDO_TELEPHONI));
    CREDO_NON_NIHIL(nodus_arboris(&app, "pinacotheca.posterior"));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&app,
        "pinacotheca.pagina"), "pagina:1 / 2");
}

/* T5 (spec D3): superficies lata (Fire 7, DXII x CCC) -> IV x III */
interior vacuum
_sectio_rete_latum (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;

    vol = (Volumen*)c->datum;
    picturas_facere(vol, XII);
    thema_initiare();
    CREDO_NECESSE(pictor_applicatio_aedificare(&app, piscina, intern,
        vol, DXII, CCC));
    app.montatio.compositio.elenchus_praesto = VERUM;
    dispensator_recomponere(app.d);
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(pinacothecam_aperire(&app, m));
    CREDO_VERUM(rete_est(&app, IV, DXII, CCC));
}

/* T5 Review Focus 3: limites paginarum - XII = pagina una sine
 * sagittis; XIII = duae, pagina II unam habet, ultra non it */
interior vacuum
_sectio_paginae (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;

    vol = (Volumen*)c->datum;
    picturas_facere(vol, XI);
    CREDO_NECESSE(aedificare(&app, vol, VERUM));
    m = manus_ludus_creare(piscina, app.d);

    CREDO_GRADUS("XII: pagina una, sine sagittis");
    CREDO_VERUM(pinacothecam_aperire(&app, m));
    CREDO_NON_NIHIL(nodus_arboris(&app, "pinax.11"));
    CREDO_VERUM(nodus_arboris(&app, "pinacotheca.prior") == NIHIL);
    CREDO_VERUM(nodus_arboris(&app, "pinacotheca.posterior") == NIHIL);
    CREDO_VERUM(premere_nodum(&app, m, "pinacotheca.claudere"));

    CREDO_GRADUS("XIII: pagina II pinacem unum habet");
    picturas_facere(vol, XII);   /* p11 nova (p0..p10 iam exstant) */
    CREDO_VERUM(pinacothecam_aperire(&app, m));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&app,
        "pinacotheca.pagina"), "pagina:1 / 2");
    CREDO_VERUM(premere_nodum(&app, m, "pinacotheca.posterior"));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&app,
        "pinacotheca.pagina"), "pagina:2 / 2");
    CREDO_NON_NIHIL(nodus_arboris(&app, "pinax.0"));
    CREDO_VERUM(nodus_arboris(&app, "pinax.1") == NIHIL);

    CREDO_GRADUS("ultra ultimam non it");
    CREDO_VERUM(premere_nodum(&app, m, "pinacotheca.posterior"));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&app,
        "pinacotheca.pagina"), "pagina:2 / 2");
    CREDO_VERUM(premere_nodum(&app, m, "pinacotheca.prior"));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&app,
        "pinacotheca.pagina"), "pagina:1 / 2");

    CREDO_GRADUS("apertio iterum: pagina I");
    CREDO_VERUM(premere_nodum(&app, m, "pinacotheca.posterior"));
    CREDO_VERUM(premere_nodum(&app, m, "pinacotheca.claudere"));
    CREDO_VERUM(pinacothecam_aperire(&app, m));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&app,
        "pinacotheca.pagina"), "pagina:1 / 2");
}

/* ictus (pressio et solutio) in centro tabulae */
interior vacuum
in_tabula_pingere (
    PictorApplicatio* app,
          ManusLudus* m)
{
    (vacuum)premere_nodum(app, m, "tabula");
}

interior vacuum
clavis_z (
    PictorApplicatio* app)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                     = M;
    e.datum.clavis.clavis        = (clavis_t)'z';
    e.datum.clavis.runa          = 'z';
    e.datum.clavis.modificantes  = MOD_SUPER;
    e.datum.clavis.actio         = EVENTUS_ACTIO_PRESSA;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
}

/* picturam per pinacothecam aperire */
interior b32
per_pinacothecam_aperire (
    PictorApplicatio* app,
          ManusLudus* m,
  constans character* spatium)
{
    character id[XVI];

    redde pinacothecam_aperire(app, m)
        && pinax_spatii(app, spatium, id)
        && premere_nodum(app, m, id);
}

/* T5 (spec D7, II.3; Review Focus 1, 2, 4) */
interior vacuum
_sectio_aperire (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
     PictorDocumentum* radix;
     PictorDocumentum* b;
                  i32  vivi_radicis;
                  i32  vivi_b;

    vol = (Volumen*)c->datum;
    picturas_facere(vol, I);   /* "p0" */
    CREDO_NECESSE(aedificare(&app, vol, VERUM));
    m      = manus_ludus_creare(piscina, app.d);
    radix  = app.montatio.doc;

    CREDO_GRADUS("pinax ictus picturam aperit");
    CREDO_VERUM(per_pinacothecam_aperire(&app, m, "p0"));
    b = app.montatio.doc;
    CREDO_VERUM(b != radix);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "pictura"),
        "p0");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");

    CREDO_GRADUS("ictus in pictura nova");
    vivi_radicis  = pictor_documentum_numerus_vivorum(radix);
    vivi_b        = pictor_documentum_numerus_vivorum(b);
    in_tabula_pingere(&app, m);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(b),
        vivi_b + I);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(radix),
        vivi_radicis);

    CREDO_GRADUS("revocatio post reditum");
    CREDO_VERUM(per_pinacothecam_aperire(&app, m, ""));
    CREDO_VERUM(app.montatio.doc == radix);
    CREDO_VERUM(per_pinacothecam_aperire(&app, m, "p0"));
    CREDO_VERUM(app.montatio.doc == b);
    clavis_z(&app);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(b), vivi_b);

    CREDO_GRADUS("transitus mundus (linea pendens)");
    vivi_b = pictor_documentum_numerus_vivorum(b);
    CREDO_VERUM(manus_ludus_clavem(m, 'l', ZEPHYRUM));
    in_tabula_pingere(&app, m);
    CREDO_VERUM(!chorda_vacua(dispensator_motus(app.d)->captura));
    /* ictus extra tabulam lineam pendentem abicit (linea.ictus) -
     * ictus primus in ⋯ eam solum abicit, menu non aperit */
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(chorda_vacua(dispensator_motus(app.d)->captura));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(b), vivi_b);
    CREDO_VERUM(per_pinacothecam_aperire(&app, m, ""));
    CREDO_VERUM(app.montatio.doc == radix);
    CREDO_VERUM(chorda_vacua(dispensator_motus(app.d)->captura));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(b), vivi_b);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(radix),
        vivi_radicis);

    CREDO_GRADUS("claudere: nihil mutat");
    CREDO_VERUM(pinacothecam_aperire(&app, m));
    CREDO_VERUM(premere_nodum(&app, m, "pinacotheca.claudere"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");
    CREDO_VERUM(app.montatio.doc == radix);
}

/* T5 (plan Global Constraints): scheda pinacothecae - apertiones
 * quinque, usus non crescit */
interior vacuum
_sectio_memoria (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
                  i32  k;
                  i32  j;
               chorda  t;
       memoriae_index  usus[V];
            character  id[XVI];

    vol = (Volumen*)c->datum;
    picturas_facere(vol, V);
    CREDO_NECESSE(aedificare(&app, vol, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    per (k = ZEPHYRUM; k < V; k++)
    {
        CREDO_VERUM(pinacothecam_aperire(&app, m));
        per (j = ZEPHYRUM; j < VI; j++)
        {
            sprintf(id, "pinax.%lu", (insignatus longus)j);
            t = titulus_nodi(&app, id);
            si (   t.mensura > VIII
                && memcmp(t.datum + t.mensura - VIII, ":electum", VIII)
                   == ZEPHYRUM)
            {
                t.mensura -= VIII;
            }
            CREDO_NON_NIHIL(pictor_pinacotheca_pinax(
                &app.montatio.pinacotheca, t));
        }
        usus[k] = piscina_summa_usus(app.montatio.pinacotheca.scheda);
        CREDO_VERUM(premere_nodum(&app, m, "pinacotheca.claudere"));
    }
    CREDO_VERUM(usus[ZEPHYRUM] > ZEPHYRUM);
    per (k = I; k < V; k++)
    {
        CREDO_VERUM(usus[k] == usus[ZEPHYRUM]);
    }
}

/* ===== T6: reditus (spec D8) ===== */

/* via voluminis veri in area sectionis (volumen temporarium in
 * claudendo deletur - hic reapertio probatur) */
interior constans character*
via_voluminis (
    CredoContextus* c)
{
    redde chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        credo_area(c), piscina), chorda_ex_literis("/reditus.vol",
        piscina), piscina), piscina);
}

/* T6: pictura ultima post reaperturam; ictus in eam scribitur */
interior vacuum
_sectio_reditus (
    CredoContextus* c)
{
     constans character* via;
                Volumen* v;
       PictorApplicatio  app;
       PictorApplicatio  app_secunda;
             ManusLudus* m;
                 chorda  spatium;

    via = via_voluminis(c);
    CREDO_NECESSE_NON_NIHIL(credo_area(c));

    CREDO_GRADUS("sessio prima: New picture, ictus");
    v = volumen_aperire_aut_creare(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_NECESSE(aedificare(&app, v, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    in_tabula_pingere(&app, m);
    spatium = pictura_ostensa(&app);
    CREDO_VERUM(ulid_est(spatium));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(
        app.montatio.doc), I);
    volumen_claudere(v);

    CREDO_GRADUS("sessio secunda: pictura eadem redit");
    v = volumen_aperire(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_NECESSE(aedificare(&app_secunda, v, VERUM));
    m = manus_ludus_creare(piscina, app_secunda.d);
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app_secunda, "pictura"),
        spatium);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(
        app_secunda.montatio.doc), I);
    CREDO_VERUM(app_secunda.doc == app_secunda.montatio.doc);
    CREDO_VERUM(app_secunda.montatio.actiones_ctx.doc
        == app_secunda.montatio.doc);
    in_tabula_pingere(&app_secunda, m);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(
        app_secunda.montatio.doc), II);
    volumen_claudere(v);
}

/* T6 (Review Focus 5): numquam mutata - radix ut ante */
interior vacuum
_sectio_radix_manet (
    CredoContextus* c)
{
     constans character* via;
                Volumen* v;
       PictorApplicatio  app;
             ManusLudus* m;

    via  = via_voluminis(c);
    v    = volumen_aperire_aut_creare(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_NECESSE(aedificare(&app, v, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    in_tabula_pingere(&app, m);
    volumen_claudere(v);
    v = volumen_aperire(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_NECESSE(aedificare(&app, v, VERUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "pictura"), "");
    CREDO_AEQUALIS_I32(numerus_picturarum(v), I);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(
        app.montatio.doc), I);
    volumen_claudere(v);
}

/* T6 (spec D8): pictura servata absens -> recentissima */
interior vacuum
_sectio_absens (
    CredoContextus* c)
{
     constans character* via;
                Volumen* v;
       PictorApplicatio  app;
             ManusLudus* m;
                 chorda  prima;
                 chorda  secunda;

    via  = via_voluminis(c);
    v    = volumen_aperire_aut_creare(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_NECESSE(aedificare(&app, v, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    prima = pictura_ostensa(&app);
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    secunda = pictura_ostensa(&app);
    CREDO_VERUM(ulid_est(prima) && ulid_est(secunda));
    volumen_claudere(v);
    /* pictura secunda e bibliotheca tollitur (nulla deletio in app: casus
     * corruptionis) */
    v = volumen_aperire(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_VERUM(volumen_plagulam_removere(v, chorda_concatenare(
        secunda, chorda_ex_literis("/documentum", piscina), piscina)));
    volumen_claudere(v);
    v = volumen_aperire(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_NECESSE(aedificare(&app, v, VERUM));
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "pictura"), prima);
    CREDO_VERUM(app.doc == app.montatio.doc);
    volumen_claudere(v);
}

/* recensio (Important 3, spec D11): pictor_applicatio_aedificare
 * sola (via terminalis) picturam ultimam NON restituit;
 * pictor_picturam_restituere (applicatio fenestrae) eam ponit */
interior vacuum
_sectio_terminalis (
    CredoContextus* c)
{
     constans character* via;
                Volumen* v;
       PictorApplicatio  app;
             ManusLudus* m;

    via  = via_voluminis(c);
    v    = volumen_aperire_aut_creare(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_NECESSE(aedificare(&app, v, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    in_tabula_pingere(&app, m);
    volumen_claudere(v);

    CREDO_GRADUS("aedificare solum: radix (terminalis immutata)");
    v = volumen_aperire(piscina, via);
    CREDO_NECESSE_NON_NIHIL(v);
    thema_initiare();
    CREDO_NECESSE(pictor_applicatio_aedificare(&app, piscina, intern, v,
        LATITUDO_TELEPHONI, ALTITUDO_TELEPHONI));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(
        app.montatio.doc), ZEPHYRUM);

    CREDO_GRADUS("restituere: pictura-1 cum ictu suo");
    CREDO_VERUM(pictor_picturam_restituere(&app));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(
        app.montatio.doc), I);
    CREDO_VERUM(app.doc == app.montatio.doc);
    CREDO_VERUM(app.montatio.actiones_ctx.doc == app.montatio.doc);
    volumen_claudere(v);
}

/* recensio (Important 2): fons imaginum "pinax:..." memoriam
 * picturae non consumit (pingitur per eventum) */
interior vacuum
_sectio_fons_sine_sumptu (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
           ManusLudus* m;
               chorda  t;
       memoriae_index  usus;
                  i32  k;

    vol = (Volumen*)c->datum;
    picturas_facere(vol, II);
    CREDO_NECESSE(aedificare(&app, vol, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(pinacothecam_aperire(&app, m));
    t = titulus_nodi(&app, "pinax.1");
    CREDO_NON_NIHIL(pictor_imago_fons(t, &app.montatio.figurae_ctx));
    usus = piscina_summa_usus(app.montatio.doc->piscina);
    per (k = ZEPHYRUM; k < XIII; k++)
    {
        CREDO_NON_NIHIL(pictor_imago_fons(t,
            &app.montatio.figurae_ctx));
    }
    CREDO_VERUM(piscina_summa_usus(app.montatio.doc->piscina) == usus);
}

/* ===== T7: imagines (spec II.6) ===== */

/* quadratum coloris 'index' (lat x alt) in picturam apertam ad
 * (x, y) */
interior vacuum
quadratum_pingere (
     PictorApplicatio* app,
                  i32  index,
                  i32  lat,
                  i32  alt,
                  s32  x,
                  s32  y)
{
    i8* indices;

    indices = (i8*)piscina_allocare(piscina, (memoriae_index)(lat
        * alt));
    si (!indices)
    {
        redde;
    }
    memset(indices, (integer)index, (size_t)(lat * alt));
    (vacuum)pictor_documentum_imaginem_inserere(app->montatio.doc,
        indices, lat, alt, x, y, I);
}

/* pixela intra pinacem (margine excluso) a fundo diversa */
interior i32
pixela_picta (
            PictorApplicatio* app,
    constans TabulaPixelorum* tabula,
          constans character* id)
{
     Componens* n;
           s32  x0;
           s32  y0;
           s32  x;
           s32  y;
           i32  fundus;
           i32  numerus;

    n = nodus_arboris(app, id);
    si (!n)
    {
        redde ZEPHYRUM;
    }
    origo(n, &x0, &y0);
    fundus   = color_ad_pixelum(thema_color(COLOR_BACKGROUND));
    numerus  = ZEPHYRUM;
    per (y = y0 + III; y < y0 + n->fines.altitudo - III; y++)
    {
        per (x = x0 + III; x < x0 + n->fines.latitudo - III; x++)
        {
            si ((i32)tabula->pixela[y * (s32)tabula->latitudo + x]
                != fundus)
            {
                numerus++;
            }
        }
    }
    redde numerus;
}

/* XII picturae novae (per menu) + radix, quaeque quadrato suo; rete
 * pingitur sine fenestra; PNG solum si PINACOTHECA_IMAGINES
 * (directorium) - porta numquam extra vestigium scribit */
interior vacuum
imaginem_probare (
    CredoContextus* c,
               s32  latitudo,
               s32  altitudo,
               i32  columnae)
{
               Volumen* vol;
      PictorApplicatio  app;
            ManusLudus* m;
       TabulaPixelorum* tabula;
         LudusFenestra* lf;
    constans character* directorium;
                   i32  k;
                   i32  dl;
                   i32  da;
             character  via[DXII];

    vol = (Volumen*)c->datum;
    thema_initiare();
    CREDO_NECESSE(pictor_applicatio_aedificare(&app, piscina, intern,
        vol, (i32)latitudo, (i32)altitudo));
    app.montatio.compositio.elenchus_praesto = VERUM;
    dispensator_recomponere(app.d);
    m = manus_ludus_creare(piscina, app.d);
    per (k = ZEPHYRUM; k <= XII; k++)
    {
        si (k > ZEPHYRUM)
        {
            CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
            CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
        }
        dl = (i32)app.montatio.doc->latitudo;
        da = (i32)app.montatio.doc->altitudo;
        quadratum_pingere(&app, (k * V) % XVI, dl / II, da / III,
            (s32)((k % III) * dl / IV), (s32)((k / III) * da / VI));
        quadratum_pingere(&app, ZEPHYRUM, dl / VI, da / X,
            (s32)(dl - dl / IV), (s32)(da - da / V));
    }
    CREDO_VERUM(pinacothecam_aperire(&app, m));
    CREDO_VERUM(rete_est(&app, columnae, latitudo, altitudo));

    tabula = tabula_pixelorum_creare_nuda(piscina, (i32)latitudo,
        (i32)altitudo);
    lf = ludus_fenestra_creare(piscina, app.d, app.figurae, ZEPHYRUM,
        pictor_imago_fons, &app.montatio.figurae_ctx, tabula);
    CREDO_NECESSE_NON_NIHIL(lf);
    ludus_quadrum(lf, ZEPHYRUM);
    ludus_quadrum(lf, C);
    per (k = ZEPHYRUM; k < XII; k++)
    {
        sprintf(via, "pinax.%lu", (insignatus longus)k);
        CREDO_VERUM(pixela_picta(&app, tabula, via) > X);
    }
    directorium = lectiones_ambitus("PINACOTHECA_IMAGINES");
    si (directorium && directorium[ZEPHYRUM])
    {
        sprintf(via, "%s/pinacotheca_%ldx%ld.png", directorium,
            (longus)latitudo, (longus)altitudo);
        CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf, via));
        imprimere("  imago: %s\n", via);
    }
}

interior vacuum
_sectio_imago_telephoni (
    CredoContextus* c)
{
    imaginem_probare(c, LATITUDO_TELEPHONI, ALTITUDO_TELEPHONI, III);
}

interior vacuum
_sectio_imago_tabellae (
    CredoContextus* c)
{
    imaginem_probare(c, DXII, CCC, IV);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "T4: quadratum et menu", _sectio_elenchus, _parare, _purgare,
        NIHIL, NIHIL },
    { "T4: New picture, Open picture", _sectio_nova, _parare, _purgare,
        NIHIL, NIHIL },
    { "T5: rete telephoni", _sectio_rete, _parare, _purgare, NIHIL,
        NIHIL },
    { "T5: rete latum", _sectio_rete_latum, _parare, _purgare, NIHIL,
        NIHIL },
    { "T5: paginae", _sectio_paginae, _parare, _purgare, NIHIL, NIHIL },
    { "T5: aperire, ictus, revocatio, transitus", _sectio_aperire,
        _parare, _purgare, NIHIL, NIHIL },
    { "T5: memoria schedae", _sectio_memoria, _parare, _purgare, NIHIL,
        NIHIL },
    { "T6: reditus picturae ultimae", _sectio_reditus, _parare,
        _purgare, NIHIL, NIHIL },
    { "T6: radix manet", _sectio_radix_manet, _parare, _purgare, NIHIL,
        NIHIL },
    { "T6: pictura servata absens", _sectio_absens, _parare, _purgare,
        NIHIL, NIHIL },
    { "recensio: terminalis non restituit", _sectio_terminalis,
        _parare, _purgare, NIHIL, NIHIL },
    { "recensio: fons sine sumptu", _sectio_fons_sine_sumptu, _parare,
        _purgare, NIHIL, NIHIL },
    { "T7: imago telephoni CCXV x CDXIX", _sectio_imago_telephoni,
        _parare, _purgare, NIHIL, NIHIL },
    { "T7: imago tabellae DXII x CCC", _sectio_imago_tabellae, _parare,
        _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_pinacotheca", SECTIONES);
}
