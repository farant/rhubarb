/* probatio_pictor_communia.c - picturae communes (communio-plan-5
 * T3-T4): index picturarum aliarum machinarum (recentissima primum,
 * ex hora scriptoris), pinax picturae alienae, picturae veteres
 * "pictura-<n>" adhuc aperiuntur. Duo volumina per communio_transferre
 * (sine rete). */
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "ordo: hora scriptoris", _sectio_ordo, _parare, _purgare, NIHIL,
        NIHIL },
    { "pinax alienus", _sectio_pinax_alienus, _parare, _purgare, NIHIL,
        NIHIL },
    { "pictura vetus (D9)", _sectio_pictura_vetus, _parare, _purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_communia", SECTIONES);
}
