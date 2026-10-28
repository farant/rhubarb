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
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
    thema_initiare();
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordo: hora scriptoris", _sectio_ordo, _parare, _purgare, NIHIL,
        NIHIL },
    { "pinax alienus", _sectio_pinax_alienus, _parare, _purgare, NIHIL,
        NIHIL },
    { "pictura vetus (D9)", _sectio_pictura_vetus, _parare, _purgare,
        NIHIL, NIHIL },
    { "Edit a copy (T4)", _sectio_exemplum, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_communia", SECTIONES);
}
