/* probatio_pictor_communia.c - picturae communes (communio-plan-5
 * T3-T4): index picturarum aliarum machinarum (recentissima primum,
 * ex hora scriptoris), pinax picturae alienae, picturae veteres
 * "pictura-<n>" adhuc aperiuntur; T4 "Edit a copy" (pictura propria ex
 * aliena, originem memorans). Duo volumina per communio_transferre
 * (sine rete). */

#define LATITUDO_TELEPHONI  CCXV
#define ALTITUDO_TELEPHONI  CDXIX
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "communio.h"
#include "internamentum.h"
#include "thema.h"
#include "imago.h"
#include "pictor_documentum.h"
#include "pictor_applicatio.h"
#include "pictor_componentia.h"
#include "pictor_figurae.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "elenchus.h"
#include "lectiones.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern_app;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina     = c->piscina;
    intern_app  = NIHIL;
    thema_initiare();
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina     = NIHIL;
    intern_app  = NIHIL;
}

interior s64
ictus (
    PictorDocumentum* doc,
                 i32  x,
                 i32  y,
                 i32  color)
{
    character t[CXXVIII];

    sprintf(t, "<ictus instrumentum=\"penicillus\" color=\"%lu\""
        " magnitudo=\"3\"><punctum x=\"%lu\" y=\"%lu\"/></ictus>",
        (insignatus longus)color, (insignatus longus)x,
        (insignatus longus)y);
    redde pictor_documentum_actum(doc, chorda_ex_literis(t, piscina));
}

/* acta B (post cursorem) in A */
interior b32
transferre (
    Volumen* b,
    Volumen* a,
        s64* cursor)
{
    CommunioRelatio r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    redde communio_transferre(b, a, cursor, &r, piscina);
}

interior PictorPicturaAliena*
aliena (
    Xar* l,
    i32  i)
{
    redde (PictorPicturaAliena*)xar_obtinere(l, i);
}

/* ordo: hora actus ultimi scriptoris, recentissima primum; propriae
 * numquam */
interior vacuum
_sectio_ordo (
    CredoContextus* c)
{
                Volumen* a;
                Volumen* b;
    InternamentumChorda* intern;
       PictorDocumentum* propria;
       PictorDocumentum* p;
       PictorDocumentum* q;
                    Xar* l;
                    s64  cursor;

    (vacuum)c;
    intern = internamentum_creare(piscina);
    a = volumen_temporarium(piscina,
        "probatio_pictor_communia_a");
    b = volumen_temporarium(piscina,
        "probatio_pictor_communia_b");
    CREDO_NECESSE(intern && a && b);
    propria = pictor_documentum_creare(piscina, intern, a, "mea", C, LX,
        II);
    CREDO_NECESSE(propria && ictus(propria, X, X, I) > ZEPHYRUM);
    p = pictor_documentum_creare(piscina, intern, b, "p", C, LX, II);
    CREDO_NECESSE(p && ictus(p, X, X, II) > ZEPHYRUM);
    q = pictor_documentum_creare(piscina, intern, b, "q", C, LX, II);
    CREDO_NECESSE(q && ictus(q, XX, XX, III) > ZEPHYRUM);
    cursor = ZEPHYRUM;
    CREDO_NECESSE(transferre(b, a, &cursor));
    l = pictor_aliena_recentia(a, piscina);
    CREDO_NECESSE(l && xar_numerus(l) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(aliena(l, ZEPHYRUM)->spatium, "q");
    CREDO_CHORDA_AEQUALIS_LITERIS(aliena(l, I)->spatium, "p");
    CREDO_CHORDA_AEQUALIS(aliena(l, ZEPHYRUM)->replica,
        volumen_replica(b));
    CREDO_VERUM(aliena(l, ZEPHYRUM)->hora > aliena(l, I)->hora);

    CREDO_GRADUS("ictus novus in p: p primum");
    CREDO_NECESSE(ictus(p, XXX, XXX, IV) > ZEPHYRUM);
    CREDO_NECESSE(transferre(b, a, &cursor));
    l = pictor_aliena_recentia(a, piscina);
    CREDO_NECESSE(l && xar_numerus(l) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(aliena(l, ZEPHYRUM)->spatium, "p");
    CREDO_CHORDA_AEQUALIS_LITERIS(aliena(l, I)->spatium, "q");

    CREDO_GRADUS("ictus proprius ordinem non movet");
    CREDO_NECESSE(ictus(propria, L, L, V) > ZEPHYRUM);
    l = pictor_aliena_recentia(a, piscina);
    CREDO_NECESSE(l && xar_numerus(l) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(aliena(l, ZEPHYRUM)->spatium, "p");

    CREDO_GRADUS("volumen sine alienis: index vacuus");
    l = pictor_aliena_recentia(b, piscina);
    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), ZEPHYRUM);
    volumen_claudere(a);
    volumen_claudere(b);
}

/* pinax alienus = pinax quem scriptor ipse faceret */
interior vacuum
_sectio_pinax_alienus (
    CredoContextus* c)
{
                Volumen* a;
                Volumen* b;
    InternamentumChorda* intern;
       PictorDocumentum* p;
                  Imago  domi;
                  Imago  alienus;
                    s64  cursor;

    (vacuum)c;
    intern = internamentum_creare(piscina);
    a = volumen_temporarium(piscina,
        "probatio_pictor_communia_c");
    b = volumen_temporarium(piscina,
        "probatio_pictor_communia_d");
    CREDO_NECESSE(intern && a && b);
    p = pictor_documentum_creare(piscina, intern, b, "p", C, LX, II);
    CREDO_NECESSE(p && ictus(p, X, X, II) > ZEPHYRUM
        && ictus(p, LX, XL, IX) > ZEPHYRUM);
    cursor = ZEPHYRUM;
    CREDO_NECESSE(transferre(b, a, &cursor));
    memset(&domi, ZEPHYRUM, magnitudo(domi));
    memset(&alienus, ZEPHYRUM, magnitudo(alienus));
    CREDO_VERUM(pictor_pinacem_facere(b, intern, "p", L, XL, piscina,
        &domi));
    CREDO_VERUM(pictor_pinacem_alienum_facere(a, intern,
        volumen_replica(b), "p", L, XL, piscina, &alienus));
    CREDO_AEQUALIS_I32(alienus.latitudo, L);
    CREDO_AEQUALIS_I32(alienus.altitudo, XL);
    CREDO_AEQUALIS_I32((i32)memcmp(domi.pixela, alienus.pixela,
        (size_t)(L * XL * IV)), ZEPHYRUM);

    CREDO_GRADUS("ignota: FALSUM");
    CREDO_FALSUM(pictor_pinacem_alienum_facere(a, intern,
        volumen_replica(b), "nulla", L, XL, piscina, &alienus));
    CREDO_FALSUM(pictor_pinacem_alienum_facere(a, intern,
        volumen_replica(a), "p", L, XL, piscina, &alienus));
    volumen_claudere(a);
    volumen_claudere(b);
}

/* D9: picturae veteres nomen suum servant */
interior vacuum
_sectio_pictura_vetus (
    CredoContextus* c)
{
                Volumen* a;
    InternamentumChorda* intern;
       PictorDocumentum* vetus;

    (vacuum)c;
    intern = internamentum_creare(piscina);
    a = volumen_temporarium(piscina,
        "probatio_pictor_communia_e");
    CREDO_NECESSE(intern && a);
    vetus = pictor_documentum_creare(piscina, intern, a, "pictura-3", C,
        LX, II);
    CREDO_NECESSE(vetus && ictus(vetus, X, X, II) > ZEPHYRUM);
    CREDO_NON_NIHIL(pictor_documentum_aperire(piscina, intern, a,
        "pictura-3"));
    volumen_claudere(a);
}

/* T4: exemplum B picturae in A - magnitudo A, compositum = alienum
 * aptatum (ut referentia eadem via facta), origo memorata */
interior b32
exemplum_probare (
                Volumen* a,
                Volumen* b,
    InternamentumChorda* intern,
     constans character* spatium,
                    i32  latitudo,
                    i32  altitudo)
{
       PictorApplicatio  app;
       PictorDocumentum* fons;
       PictorDocumentum* copia;
       PictorDocumentum* aliena;
       PictorDocumentum* referentia;
                Volumen* vr;
                    Xar* propria;
                     i8* indices;
                    s32  x;
                    s32  y;
                    i32  lat;
                    i32  alt;
                    s64  cursor;
                    i32  ictus_alieni;
                 chorda  replica;
                 chorda  spatium_alienum;
              character  novum[XXVIII];

    fons = pictor_documentum_creare(piscina, intern, b, spatium,
        latitudo, altitudo, II);
    si (   !fons || ictus(fons, latitudo / II, altitudo / II, II)
               <= ZEPHYRUM
        || ictus(fons, I, I, IX) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    cursor = ZEPHYRUM;
    si (!transferre(b, a, &cursor))
    {
        redde FALSUM;
    }
    si (!pictor_applicatio_aedificare(&app, piscina, intern, a,
            LATITUDO_TELEPHONI, ALTITUDO_TELEPHONI))
    {
        redde FALSUM;
    }
    ictus_alieni = pictor_documentum_numerus_vivorum(
        pictor_documentum_aperire_alienum(piscina, intern, a,
        volumen_replica(b), spatium));
    CREDO_VERUM(pictor_exemplum_facere(&app.montatio,
        volumen_replica(b),
        spatium, novum));
    copia = app.montatio.doc;
    CREDO_NECESSE_NON_NIHIL(copia);
    CREDO_AEQUALIS_I32((i32)strlen(novum), XXVI);
    /* magnitudo tabulae A (ut New picture) */
    CREDO_VERUM(copia->latitudo != latitudo
        || copia->altitudo != altitudo);
    /* referentia: tabula eadem, alienum aptatum eadem via */
    aliena = pictor_documentum_aperire_alienum(piscina, intern, a,
        volumen_replica(b), spatium);
    CREDO_NECESSE_NON_NIHIL(aliena);
    indices =
        pictor_imaginem_aptare(pictor_documentum_proiectio(aliena),
        copia->latitudo, copia->altitudo, piscina, &x, &y, &lat, &alt);
    CREDO_NECESSE_NON_NIHIL(indices);
    CREDO_VERUM(lat <= copia->latitudo && alt <= copia->altitudo);
    CREDO_VERUM(lat == copia->latitudo || alt == copia->altitudo);
    vr = volumen_temporarium(piscina, "probatio_pictor_communia_r");
    referentia = vr ? pictor_documentum_creare(piscina, intern, vr, "r",
        copia->latitudo, copia->altitudo, LXIV) : NIHIL;
    CREDO_NECESSE_NON_NIHIL(referentia);
    CREDO_NECESSE(pictor_documentum_imaginem_inserere(referentia,
        indices,
        lat, alt, x, y, pictor_documentum_stratum(referentia,
        pictor_documentum_numerus_stratorum(referentia) - I)->id)
        > ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(pictor_documentum_sigillum_hex(copia,
        piscina),
        pictor_documentum_sigillum_hex(referentia, piscina));
    /* in recentibus propriis primum */
    propria = pictor_documenta_recentia(a, piscina);
    CREDO_NECESSE(propria && xar_numerus(propria) > ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(((PictorPictura*)xar_obtinere(propria,
        ZEPHYRUM))->spatium, novum);
    /* origo */
    CREDO_VERUM(pictor_originem_legere(a, novum, piscina, &replica,
        &spatium_alienum));
    CREDO_CHORDA_AEQUALIS(replica, volumen_replica(b));
    CREDO_CHORDA_AEQUALIS_LITERIS(spatium_alienum, spatium);
    /* pingere in copia: actum appenditur; alienum intactum */
    CREDO_VERUM(ictus(copia, V, V, IV) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_vivorum(
        pictor_documentum_aperire_alienum(piscina, intern, a,
        volumen_replica(b), spatium)), ictus_alieni);
    volumen_claudere(vr);
    redde VERUM;
}

interior vacuum
_sectio_exemplum (
    CredoContextus* c)
{
                Volumen* a;
                Volumen* b;
    InternamentumChorda* intern;
       PictorApplicatio  app;
              character  novum[XXVIII];
                    i32  ante;
                 chorda  replica;
                 chorda  spatium;

    (vacuum)c;
    intern = internamentum_creare(piscina);
    a = volumen_temporarium(piscina,
        "probatio_pictor_communia_f");
    b = volumen_temporarium(piscina,
        "probatio_pictor_communia_g");
    CREDO_NECESSE(intern && a && b);
    CREDO_GRADUS("pictura maior quam tabula A");
    CREDO_VERUM(exemplum_probare(a, b, intern, "magna", CCC, CC));
    CREDO_GRADUS("pictura minor quam tabula A");
    CREDO_VERUM(exemplum_probare(a, b, intern, "parva", XL, XXX));

    CREDO_GRADUS("ignota: FALSUM, nihil creatum");
    CREDO_NECESSE(pictor_applicatio_aedificare(&app, piscina, intern, a,
        LATITUDO_TELEPHONI, ALTITUDO_TELEPHONI));
    ante = (i32)xar_numerus(pictor_documenta_enumerare(a, piscina));
    CREDO_FALSUM(pictor_exemplum_facere(&app.montatio,
        volumen_replica(b), "nulla", novum));
    CREDO_AEQUALIS_I32((i32)xar_numerus(pictor_documenta_enumerare(a,
        piscina)), ante);

    CREDO_GRADUS("pictura sine origine: FALSUM");
    CREDO_FALSUM(pictor_originem_legere(a, "nulla", piscina, &replica,
        &spatium));
    volumen_claudere(a);
    volumen_claudere(b);
}

/* ===== T6: menu, Shared pictures, spectator (compositio) ===== */


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
    redde a ? chorda_ex_literis(chorda_ut_cstr(*a, piscina), piscina)
            : chorda_ex_literis("", piscina);
}

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

/* tractus in nodo: a (initium, fy) ad (terminus, fy) in fractionibus
 * centesimis latitudinis */
interior b32
trahere_nodum (
    PictorApplicatio* app,
          ManusLudus* m,
  constans character* id,
                 s32  initium,
                 s32  terminus,
                 s32  dy)
{
    Componens* n;
      Punctum  puncta[VI];
          s32  k;
    character  selector[LXIV];
          b32  factum;

    n = nodus_arboris(app, id);
    si (!n)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < VI; k++)
    {
        puncta[k].x = n->fines.latitudo * (initium + (terminus
            - initium) * k / V)
            / C;
        puncta[k].y = n->fines.altitudo / II + dy * k / V;
    }
    sprintf(selector, "#%s", id);
    factum = manus_ludus_trahere(m, selector, puncta, VI);
    dispensator_recomponere(app->d);
    redde factum;
}

/* clavis sagittae (manus_ludus_clavem characterem solum fert) */
interior vacuum
sagittam_premere (
    PictorApplicatio* app,
                 i32  clavis)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus               = M * M;
    e.datum.clavis.clavis  = (clavis_t)clavis;
    e.datum.clavis.codex   = clavis == (i32)CLAVIS_DEXTER
        ? EVENTUS_CODEX_SAGITTA_DEXTRA : EVENTUS_CODEX_SAGITTA_SINISTRA;
    e.datum.clavis.actio   = EVENTUS_ACTIO_PRESSA;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
}

interior chorda
auctor_fictus (
    vacuum* ctx,
    chorda  replica,
   Piscina* p)
{
    (vacuum)ctx;
    (vacuum)replica;
    redde chorda_ex_literis("Amanda \xC2\xB7 iPhone", p);
}

/* app in A cum picturis alienis B; communio et inscripta datae */
interior b32
app_communem (
     PictorApplicatio* app,
              Volumen* a,
                  b32  inscripta)
{
    thema_initiare();
    si (!pictor_applicatio_aedificare(app, piscina, intern_app, a,
            LATITUDO_TELEPHONI, ALTITUDO_TELEPHONI))
    {
        redde FALSUM;
    }
    app->montatio.compositio.elenchus_praesto  = VERUM;
    app->montatio.compositio.communio_praesto  = VERUM;
    app->montatio.compositio.inscripta         = inscripta;
    app->montatio.compositio.auctor            = auctor_fictus;
    app->montatio.compositio.auctor_ctx        = NIHIL;
    dispensator_recomponere(app->d);
    redde VERUM;
}

/* n picturae in B (actum unum quaeque), in A translatae */
interior b32
alienas_facere (
                Volumen* a,
                Volumen* b,
    InternamentumChorda* in,
                    i32  n,
                    s64* cursor)
{
    PictorDocumentum* p;
           character  s[XVI];
                 i32  k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        sprintf(s, "q%02ld", (longus)k);
        p = pictor_documentum_creare(piscina, in, b, s, C, LX, II);
        si (!p || ictus(p, X + k, X, (i32)(k % XV + I)) <= ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde transferre(b, a, cursor);
}

/* "replica:spatium" picturae k in indice communi A */
interior chorda
identitas_alieni (
     Volumen* a,
         i32  k)
{
                    Xar* l;
    PictorPicturaAliena* p;

    l = pictor_aliena_recentia(a, piscina);
    p = (l && k < xar_numerus(l)) ? aliena(l, k) : NIHIL;
    redde p ? chorda_concatenare(chorda_concatenare(p->replica,
        chorda_ex_literis(":", piscina), piscina), p->spatium, piscina)
            : chorda_ex_literis("", piscina);
}

interior i32
acta_propria (
    Volumen* v)
{
    Xar* l;

    l = volumen_acta_propria_legere(v, ZEPHYRUM, piscina);
    redde l ? xar_numerus(l) : ZEPHYRUM;
}

interior vacuum
_sectio_menu (
    CredoContextus* c)
{
    PictorElenchiCondiciones k;
                 ElenchusRes res[V];
                    Elenchus e;

    (vacuum)c;
    CREDO_GRADUS("Mac, non inscriptus");
    k.importare  = FALSUM;
    k.communio   = VERUM;
    k.inscripta  = FALSUM;
    pictor_elenchum_parare(&k, res, &e);
    CREDO_AEQUALIS_I32(e.numerus, IV);
    CREDO_VERUM(strcmp(e.res[ZEPHYRUM].textus,
        "My pictures\xE2\x80\xA6")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(e.res[I].textus, "Shared pictures\xE2\x80\xA6")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(e.res[II].textus, "New picture") == ZEPHYRUM);
    CREDO_VERUM(strcmp(e.res[III].textus, "Sign in\xE2\x80\xA6")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(e.res[I].titulus, "communia.aperire")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(e.res[III].titulus, "inscriptio.aperire")
        == ZEPHYRUM);

    CREDO_GRADUS("iPhone inscriptus");
    k.importare = VERUM;
    k.inscripta = VERUM;
    pictor_elenchum_parare(&k, res, &e);
    CREDO_AEQUALIS_I32(e.numerus, IV);
    CREDO_VERUM(strcmp(e.res[III].textus, "Import image\xE2\x80\xA6")
        == ZEPHYRUM);

    CREDO_GRADUS("Android: sine communione");
    k.importare  = FALSUM;
    k.communio   = FALSUM;
    k.inscripta  = FALSUM;
    pictor_elenchum_parare(&k, res, &e);
    CREDO_AEQUALIS_I32(e.numerus, II);
    CREDO_VERUM(strcmp(e.res[ZEPHYRUM].titulus, "pinacotheca.aperire")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(e.res[I].titulus, "pictura.nova") == ZEPHYRUM);
}

interior vacuum
_sectio_communia (
    CredoContextus* c)
{
            Volumen* a;
            Volumen* b;
   PictorApplicatio  app;
         ManusLudus* m;
          Componens* n;
                s64  cursor;

    (vacuum)c;
    intern_app = internamentum_creare(piscina);
    a = volumen_temporarium(piscina, "probatio_pictor_communia_h");
    b = volumen_temporarium(piscina, "probatio_pictor_communia_i");
    CREDO_NECESSE(intern_app && a && b);

    CREDO_GRADUS("vacua: non inscriptus, deinde inscriptus");
    CREDO_NECESSE(app_communem(&app, a, FALSUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    n = nodus_arboris(&app, "elenchus");
    CREDO_NECESSE_NON_NIHIL(n);
    CREDO_AEQUALIS_I32(n->liberi ? xar_numerus(n->liberi) : ZEPHYRUM,
        IV);
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "communia");
    n = nodus_arboris(&app, "communia.vacua");
    CREDO_NECESSE_NON_NIHIL(n);
    CREDO_CHORDA_AEQUALIS_LITERIS(n->titulus,
        "pagina:Sign in to see shared pictures.");
    app.montatio.compositio.inscripta = VERUM;
    dispensator_recomponere(app.d);
    n = nodus_arboris(&app, "communia.vacua");
    CREDO_NECESSE_NON_NIHIL(n);
    CREDO_CHORDA_AEQUALIS_LITERIS(n->titulus,
        "pagina:No shared pictures yet.");

    CREDO_GRADUS("XIII alienae: XII in pagina, recentissima primum");
    cursor = ZEPHYRUM;
    CREDO_NECESSE(alienas_facere(a, b, intern_app, XIII, &cursor));
    CREDO_NECESSE(app_communem(&app, a, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    CREDO_VERUM(nodus_arboris(&app, "communia.pinax.11") != NIHIL);
    CREDO_VERUM(nodus_arboris(&app, "communia.pinax.12") == NIHIL);
    n = nodus_arboris(&app, "communia.pinax.0");
    CREDO_NECESSE_NON_NIHIL(n);
    CREDO_VERUM(chorda_continet(n->titulus, chorda_ex_literis(":q12",
        piscina)));
    CREDO_VERUM(premere_nodum(&app, m, "communia.posterior"));
    CREDO_VERUM(nodus_arboris(&app, "communia.pinax.0") != NIHIL);
    CREDO_VERUM(nodus_arboris(&app, "communia.pinax.1") == NIHIL);
    CREDO_VERUM(premere_nodum(&app, m, "communia.claudere"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");
    volumen_claudere(a);
    volumen_claudere(b);
}

interior vacuum
_sectio_spectator (
    CredoContextus* c)
{
            Volumen* a;
            Volumen* b;
   PictorApplicatio  app;
         ManusLudus* m;
          Componens* n;
                s64  cursor;
                i32  acta;
             chorda  replica;
             chorda  spatium;
             chorda  titulus_prior;
   PictorDocumentum* p;

    (vacuum)c;
    intern_app = internamentum_creare(piscina);
    a = volumen_temporarium(piscina, "probatio_pictor_communia_j");
    b = volumen_temporarium(piscina, "probatio_pictor_communia_k");
    CREDO_NECESSE(intern_app && a && b);
    cursor = ZEPHYRUM;
    CREDO_NECESSE(alienas_facere(a, b, intern_app, XIII, &cursor));
    CREDO_NECESSE(app_communem(&app, a, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));

    CREDO_GRADUS("pinax -> spectator, auctor");
    CREDO_VERUM(premere_nodum(&app, m, "communia.pinax.0"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "spectator");
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, ZEPHYRUM));
    n = nodus_arboris(&app, "spectator.auctor");
    CREDO_NECESSE_NON_NIHIL(n);
    CREDO_CHORDA_AEQUALIS_LITERIS(n->titulus,
        "pagina:by Amanda \xC2\xB7 iPhone");
    CREDO_VERUM(nodus_arboris(&app, "spectator.imago") != NIHIL);
    acta = acta_propria(a);

    CREDO_GRADUS("tractus sinister: proxima; circulus");
    CREDO_VERUM(trahere_nodum(&app, m, "spectator.imago", LXXX, X,
        ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, I));
    CREDO_VERUM(trahere_nodum(&app, m, "spectator.imago", X, LXXX,
        ZEPHYRUM));
    CREDO_VERUM(trahere_nodum(&app, m, "spectator.imago", X, LXXX,
        ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, XII));
    CREDO_VERUM(trahere_nodum(&app, m, "spectator.imago", LXXX, X,
        ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, ZEPHYRUM));

    CREDO_GRADUS("tractus brevis aut verticalis: nihil");
    CREDO_VERUM(trahere_nodum(&app, m, "spectator.imago", L, LV,
        ZEPHYRUM));
    CREDO_VERUM(trahere_nodum(&app, m, "spectator.imago", L, LIV, C));
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, ZEPHYRUM));

    CREDO_GRADUS("claves sagittarum");
    sagittam_premere(&app, (i32)CLAVIS_DEXTER);
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, I));
    sagittam_premere(&app, (i32)CLAVIS_SINISTER);
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, ZEPHYRUM));

    CREDO_GRADUS("lectio sola: nullum actum");
    CREDO_AEQUALIS_I32(acta_propria(a), acta);

    CREDO_GRADUS("X: rete, pagina eadem; ictus extra imaginem");
    CREDO_VERUM(premere_nodum(&app, m, "spectator.claudere"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "communia");
    CREDO_VERUM(premere_nodum(&app, m, "communia.posterior"));
    CREDO_VERUM(premere_nodum(&app, m, "communia.pinax.0"));
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, XII));
    /* ictus extra imaginem: in margine dialogi (angulus) */
    n = nodus_arboris(&app, "spectator");
    CREDO_NECESSE_NON_NIHIL(n);
    {
        s32 x;
        s32 y;

        origo(n, &x, &y);
        (vacuum)manus_ludus_premere_ad(m, x + II, y + II);
        dispensator_recomponere(app.d);
    }
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
        "communia");
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
        "pagina_communium"), "1");

    CREDO_GRADUS("vivus: pictura nova et ictus novi");
    CREDO_VERUM(premere_nodum(&app, m, "communia.prior"));
    CREDO_VERUM(premere_nodum(&app, m, "communia.pinax.1"));
    titulus_prior = chorda_ex_literis(chorda_ut_cstr(nodus_arboris(&app,
        "spectator.imago")->titulus, piscina), piscina);
    CREDO_CHORDA_AEQUALIS(ephemera_legere(&app, "spectata"),
        identitas_alieni(a, I));
    replica  = volumen_replica(b);
    p        = pictor_documentum_aperire(piscina, intern_app, b, "q11");
    CREDO_NECESSE(p && ictus(p, L, L, II) > ZEPHYRUM);
    p = pictor_documentum_creare(piscina, intern_app, b, "nova", C, LX,
        II);
    CREDO_NECESSE(p && ictus(p, X, X, IV) > ZEPHYRUM);
    CREDO_NECESSE(transferre(b, a, &cursor));
    CREDO_VERUM(pictor_communia_parare(&app.montatio.communia));
    dispensator_recomponere(app.d);
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "spectata"),
        chorda_ut_cstr(chorda_concatenare(replica, chorda_ex_literis(
        ":q11", piscina), piscina), piscina));
    n = nodus_arboris(&app, "spectator.imago");
    CREDO_NECESSE_NON_NIHIL(n);
    CREDO_FALSUM(chorda_aequalis(n->titulus, titulus_prior));
    CREDO_VERUM(trahere_nodum(&app, m, "spectator.imago", X, LXXX,
        ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "spectata"),
        chorda_ut_cstr(chorda_concatenare(replica, chorda_ex_literis(
        ":nova", piscina), piscina), piscina));

    CREDO_GRADUS("Edit a copy");
    CREDO_VERUM(premere_nodum(&app, m, "spectator.exemplum"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"), "");
    CREDO_VERUM(pictor_originem_legere(a,
        chorda_ut_cstr(ephemera_legere(
        &app, "pictura"), piscina), piscina, &replica, &spatium));
    CREDO_CHORDA_AEQUALIS_LITERIS(spatium, "nova");
    volumen_claudere(a);
    volumen_claudere(b);
}

/* rete et spectator sine fenestra pinguntur; PNG si
 * PINACOTHECA_IMAGINES */
interior vacuum
_sectio_imagines (
    CredoContextus* c)
{
                 Volumen* a;
                 Volumen* b;
        PictorApplicatio  app;
              ManusLudus* m;
         TabulaPixelorum* tabula;
           LudusFenestra* lf;
      constans character* directorium;
                     s64  cursor;
               character  via[DXII];

    (vacuum)c;
    intern_app = internamentum_creare(piscina);
    a = volumen_temporarium(piscina, "probatio_pictor_communia_l");
    b = volumen_temporarium(piscina, "probatio_pictor_communia_m");
    CREDO_NECESSE(intern_app && a && b);
    cursor = ZEPHYRUM;
    CREDO_NECESSE(alienas_facere(a, b, intern_app, V, &cursor));
    CREDO_NECESSE(app_communem(&app, a, VERUM));
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(premere_nodum(&app, m, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&app, m, "elenchus.1"));
    tabula = tabula_pixelorum_creare_nuda(piscina, LATITUDO_TELEPHONI,
        ALTITUDO_TELEPHONI);
    lf = ludus_fenestra_creare(piscina, app.d, app.figurae, ZEPHYRUM,
        pictor_imago_fons, &app.montatio.figurae_ctx, tabula);
    CREDO_NECESSE_NON_NIHIL(lf);
    ludus_quadrum(lf, ZEPHYRUM);
    ludus_quadrum(lf, C);
    directorium = lectiones_ambitus("PINACOTHECA_IMAGINES");
    si (directorium && directorium[ZEPHYRUM])
    {
        sprintf(via, "%s/communia.png", directorium);
        CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf, via));
        imprimere("  imago: %s\n", via);
    }
    CREDO_VERUM(premere_nodum(&app, m, "communia.pinax.0"));
    ludus_quadrum(lf, CC);
    si (directorium && directorium[ZEPHYRUM])
    {
        sprintf(via, "%s/spectator.png", directorium);
        CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf, via));
        imprimere("  imago: %s\n", via);
    }
    volumen_claudere(a);
    volumen_claudere(b);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordo: hora scriptoris", _sectio_ordo, _parare, _purgare, NIHIL,
        NIHIL },
    { "pinax alienus", _sectio_pinax_alienus, _parare, _purgare, NIHIL,
        NIHIL },
    { "pictura vetus (D9)", _sectio_pictura_vetus, _parare, _purgare,
        NIHIL, NIHIL },
    { "Edit a copy (T4)", _sectio_exemplum, _parare, _purgare, NIHIL,
        NIHIL },
    { "menu (T6)", _sectio_menu, _parare, _purgare, NIHIL, NIHIL },
    { "Shared pictures (T6)", _sectio_communia, _parare, _purgare,
        NIHIL,
        NIHIL },
    { "spectator (T6)", _sectio_spectator, _parare, _purgare, NIHIL,
        NIHIL },
    { "imagines (T6)", _sectio_imagines, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_communia", SECTIONES);
}
