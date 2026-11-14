/* probatio_mundus_ostia.c - ostia loci (mundus_ostia.h) et elementum
 * <ostium> dispositionis (mundus_locus.h)
 *
 * legere: campi, radius ordinarius, lineae, recusationes plagulae.
 * probare in loco ficto: millesimae rotundatae; ostium in re solida
 * licet si iuxta stare licet; titulus, radius, extra locum, sine
 * solo intra radium, duo in puncto eodem, nimis multa - cum linea,
 * primum vincit. volumen: posita redeunt; eadem nihil scribunt;
 * novissima vincunt; vacua tollunt; loci separati; acta mala
 * praetereuntur; transactio vocatoris iungitur; index statuarum et
 * scaenarum nihil videt. proximus: cella ostio proxima - centrum
 * ipsum, proxima, paria prius, radius circulus, aqua praeterita,
 * extra locum, exitus non scripti si nulla.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "statua.h"
#include "statuae_repositorium.h"
#include "scaena_repositorium.h"
#include "mundus_solum.h"
#include "mundus.h"
#include "mundus_locus.h"
#include "mundus_ostia.h"
#include "credo.h"
#include "mundus_locus_fictus.h"
#include <stdio.h>
#include <string.h>

#define RECUSATUR(fructus, linea_sperata, fragmentum) \
    fac { \
        CREDO_FALSUM ((fructus).successus); \
        CREDO_AEQUALIS_I32 ((fructus).linea, (linea_sperata)); \
        si (!chorda_continet((fructus).error, \
            chorda_ex_literis((fragmentum), c->piscina))) \
        { \
            printf("nuntius: %.*s\n", \
                (integer)(fructus).error.mensura, \
                (constans character*)(fructus).error.datum); \
        } \
        CREDO_VERUM (chorda_continet((fructus).error, \
            chorda_ex_literis((fragmentum), c->piscina))); \
    } dum (FALSUM)

interior chorda
_ch (
        CredoContextus* c,
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, c->piscina);
}

interior MundusLocusFructus
_lege (
        CredoContextus* c,
    constans character* litterae)
{
    redde mundus_locus_legere(_ch(c, litterae), c->piscina);
}

interior MundusLocusOstium
_scriptum (
        CredoContextus* c,
    constans character* scaena,
                   f32  x,
                   f32  z,
                   f32  radius,
                   i32  linea)
{
    MundusLocusOstium o;

    o.scaena  = _ch(c, scaena);
    o.x       = x;
    o.z       = z;
    o.radius  = radius;
    o.linea   = linea;
    redde o;
}

interior MundusOstium
_ostium (
        CredoContextus* c,
    constans character* scaena,
                   s32  x,
                   s32  z,
                   s32  radius)
{
    MundusOstium o;

    o.scaena             = _ch(c, scaena);
    o.x_millesimae       = x;
    o.z_millesimae       = z;
    o.radius_millesimae  = radius;
    redde o;
}

interior MundusOstium*
_lectum (
    Xar* index,
    i32  k)
{
    redde (MundusOstium*)xar_obtinere(index, k);
}

interior b32
_par (
    constans MundusOstium* a,
    constans MundusOstium* b)
{
    redde chorda_aequalis(a->scaena, b->scaena)
        && a->x_millesimae == b->x_millesimae
        && a->z_millesimae == b->z_millesimae
        && a->radius_millesimae == b->radius_millesimae;
}

interior MundusSolum*
_solum (
    CredoContextus* c)
{
    redde mundus_solum_struere(loci_ficti_locus(c), c->piscina);
}


/* ==================================================
 * Dispositio
 * ================================================== */

interior vacuum
_sectio_legere (
    CredoContextus* c)
{
    MundusLocusFructus f;

    /* nulla ostia */
    f = _lege(c, "<locus titulus=\"a\" basis=\"b.glb\"/>");
    CREDO_NECESSE (f.successus);
    CREDO_AEQUALIS_I32 (f.ostiorum, ZEPHYRUM);
    /* unum, radius ordinarius */
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"29 46.5\" scaena=\"hut-door\"/>\n"
        "</locus>\n");
    CREDO_NECESSE (f.successus);
    CREDO_NECESSE (f.ostiorum == I);
    CREDO_AEQUALIS_I32 (f.numerus, ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(f.ostia[ZEPHYRUM].scaena,
        "hut-door"));
    CREDO_F64_PROXIMUS ((f64)f.ostia[ZEPHYRUM].x, 29.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.ostia[ZEPHYRUM].z, 46.5, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.ostia[ZEPHYRUM].radius, 1.5, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)MUNDUS_LOCUS_OSTII_RADIUS, 1.5, 1.0e-9);
    CREDO_AEQUALIS_I32 (f.ostia[ZEPHYRUM].linea, II);
    /* plura inter res et sparsiones: ordo plagulae, lineae suae;
     * res et sparsiones non turbantur */
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <res fons=\"p.stml\" locus=\"6 15\"/>\n"
        "  <ostium locus=\"1 2\" radius=\"3\" scaena=\"cave_1\"/>\n"
        "  <sparsio fons=\"g.stml\" numerus=\"4\""
        " area=\"0 0 8 8\"/>\n"
        "\n"
        "  <ostium scaena=\"hut-door\" locus=\"-4.25 7\""
        " radius=\"0.5\"/>\n"
        "  <res fons=\"q.stml\" locus=\"1 2\"/>\n"
        "</locus>\n");
    CREDO_NECESSE (f.successus);
    CREDO_NECESSE (f.ostiorum == II);
    CREDO_AEQUALIS_I32 (f.numerus, II);
    CREDO_AEQUALIS_I32 (f.sparsionum, I);
    CREDO_VERUM (chorda_aequalis_literis(f.res[I].fons, "q.stml"));
    CREDO_VERUM (chorda_aequalis_literis(f.ostia[ZEPHYRUM].scaena,
        "cave_1"));
    CREDO_F64_PROXIMUS ((f64)f.ostia[ZEPHYRUM].x, 1.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.ostia[ZEPHYRUM].z, 2.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.ostia[ZEPHYRUM].radius, 3.0, 1.0e-9);
    CREDO_AEQUALIS_I32 (f.ostia[ZEPHYRUM].linea, III);
    CREDO_VERUM (chorda_aequalis_literis(f.ostia[I].scaena,
        "hut-door"));
    CREDO_F64_PROXIMUS ((f64)f.ostia[I].x, -4.25, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.ostia[I].z, 7.0, 1.0e-9);
    CREDO_F64_PROXIMUS ((f64)f.ostia[I].radius, 0.5, 1.0e-9);
    CREDO_AEQUALIS_I32 (f.ostia[I].linea, VI);
    /* titulus et radius hic NON probantur (mundus_ostia_probare) */
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1 2\" radius=\"99\" scaena=\"Not Valid\"/>\n"
        "</locus>\n");
    CREDO_NECESSE (f.successus);
    CREDO_NECESSE (f.ostiorum == I);
    CREDO_F64_PROXIMUS ((f64)f.ostia[ZEPHYRUM].radius, 99.0, 1.0e-9);
    CREDO_VERUM (chorda_aequalis_literis(f.ostia[ZEPHYRUM].scaena,
        "Not Valid"));
}

interior vacuum
_sectio_legere_recusat (
    CredoContextus* c)
{
    MundusLocusFructus f;

    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium scaena=\"hut-door\"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "'locus'");
    CREDO_VERUM (f.ostia == NIHIL);
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "\n"
        "  <ostium locus=\"1 2\"/>\n"
        "</locus>\n");
    RECUSATUR (f, III, "'scaena'");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1 2\" scaena=\"  \"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "'scaena'");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1\" scaena=\"hut-door\"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "2 numbers");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1 2 3\" scaena=\"hut-door\"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "2 numbers");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1 z\" scaena=\"hut-door\"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "2 numbers");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1 2\" radius=\"latus\""
        " scaena=\"hut-door\"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "'radius'");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1 2\" radius=\"1 2\""
        " scaena=\"hut-door\"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "'radius'");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1 2\" scaena=\"hut-door\""
        " gyrus=\"4\"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "unknown attribute 'gyrus'");
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <ostium locus=\"1 2\" scaena=\"hut-door\">\n"
        "    <res fons=\"p.stml\" locus=\"1 2\"/>\n"
        "  </ostium>\n"
        "</locus>\n");
    RECUSATUR (f, II, "<ostium> cannot have children");
    /* elementum ignotum ostium nunc nominat */
    f = _lege(c,
        "<locus titulus=\"a\" basis=\"b.glb\">\n"
        "  <porta locus=\"1 2\"/>\n"
        "</locus>\n");
    RECUSATUR (f, II, "<ostium>");
}


/* ==================================================
 * Probare
 * ================================================== */

interior vacuum
_sectio_probare (
    CredoContextus* c)
{
           MundusSolum* solum = _solum(c);
     MundusLocusOstium  o[IV];
    MundusOstiaFructus  f;

    CREDO_NECESSE (solum != NIHIL);
    /* nulla: successus, index vacuus */
    f = mundus_ostia_probare(NIHIL, ZEPHYRUM, solum,
        MUNDUS_CORPORIS_RADIUS, c->piscina);
    CREDO_VERUM (f.successus);
    CREDO_AEQUALIS_I32 (f.numerus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (f.error.mensura, ZEPHYRUM);
    /* in solo plano; millesimae ad proximum rotundantur (3.9996 ->
     * 4000, non 3999) */
    o[ZEPHYRUM]  = _scriptum(c, "hut-door", 2.25f, 10.0f, 1.5f, III);
    o[I]         = _scriptum(c, "cave_1", 3.9996f, 5.0004f, 0.5f, V);
    /* in saxo solido ipso: licet, iuxta stare licet */
    o[II]        = _scriptum(c, "rock", 5.5f, 1.5f, 1.5f, VII);
    /* in aqua, sed radius solum siccum attingit */
    o[III]       = _scriptum(c, "pond", 2.0f, 2.0f, 3.0f, IX);
    f = mundus_ostia_probare(o, IV, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    si (!f.successus)
    {
        printf("nuntius: %.*s\n", (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    CREDO_NECESSE (f.numerus == IV);
    CREDO_AEQUALIS_I32 (f.linea, ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(f.ostia[ZEPHYRUM].scaena,
        "hut-door"));
    CREDO_VERUM (f.ostia[ZEPHYRUM].x_millesimae == 2250);
    CREDO_VERUM (f.ostia[ZEPHYRUM].z_millesimae == 10000);
    CREDO_VERUM (f.ostia[ZEPHYRUM].radius_millesimae == 1500);
    CREDO_VERUM (chorda_aequalis_literis(f.ostia[I].scaena,
        "cave_1"));
    CREDO_VERUM (f.ostia[I].x_millesimae == 4000);
    CREDO_VERUM (f.ostia[I].z_millesimae == 5000);
    CREDO_VERUM (f.ostia[I].radius_millesimae == 500);
    CREDO_VERUM (f.ostia[II].x_millesimae == 5500);
    CREDO_VERUM (f.ostia[II].z_millesimae == 1500);
    CREDO_VERUM (chorda_aequalis_literis(f.ostia[III].scaena,
        "pond"));
    CREDO_VERUM (f.ostia[III].radius_millesimae == 3000);
    /* fines radii ipsi licent */
    o[ZEPHYRUM] = _scriptum(c, "a", 2.0f, 10.0f,
        MUNDUS_OSTII_RADIUS_MINIMUS, I);
    o[I]        = _scriptum(c, "b", 2.0f, 12.0f,
        MUNDUS_OSTII_RADIUS_MAXIMUS, II);
    f = mundus_ostia_probare(o, II, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    CREDO_VERUM (f.successus);
    CREDO_AEQUALIS_I32 (f.numerus, II);
}

interior vacuum
_sectio_probare_recusat (
    CredoContextus* c)
{
           MundusSolum* solum = _solum(c);
     MundusLocusOstium  o[III];
     MundusLocusOstium* multa;
    MundusOstiaFructus  f;
                   i32  k;

    CREDO_NECESSE (solum != NIHIL);
    /* titulus scaenae */
    o[ZEPHYRUM] = _scriptum(c, "Hut Door", 2.0f, 10.0f, 1.5f, IV);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, IV, "Hut Door");
    CREDO_VERUM (f.ostia == NIHIL);
    CREDO_AEQUALIS_I32 (f.numerus, ZEPHYRUM);
    o[ZEPHYRUM] = _scriptum(c, "mundus", 2.0f, 10.0f, 1.5f, IV);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, IV, "scene");
    o[ZEPHYRUM] = _scriptum(c, "", 2.0f, 10.0f, 1.5f, IV);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, IV, "scene");
    /* radius */
    o[ZEPHYRUM] = _scriptum(c, "hut-door", 2.0f, 10.0f, 0.49f, VI);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, VI, "radius");
    o[ZEPHYRUM] = _scriptum(c, "hut-door", 2.0f, 10.0f, 8.01f, VI);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, VI, "radius");
    o[ZEPHYRUM] = _scriptum(c, "hut-door", 2.0f, 10.0f, -1.5f, VI);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, VI, "radius");
    /* extra locum */
    o[ZEPHYRUM] = _scriptum(c, "hut-door", 40.0f, 40.0f, 1.5f, VIII);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, VIII, "outside");
    o[ZEPHYRUM] = _scriptum(c, "hut-door", -3.0f, 10.0f, 8.0f, VIII);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, VIII, "outside");
    /* in media aqua, radius parvus: nusquam stare licet */
    o[ZEPHYRUM] = _scriptum(c, "pond", 2.0f, 2.0f, 0.5f, IX);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, IX, "stand");
    /* idem radius in solo sicco licet (radius, non locus, decidit) */
    o[ZEPHYRUM] = _scriptum(c, "pond", 2.0f, 10.0f, 0.5f, IX);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    CREDO_VERUM (f.successus);
    /* corpus latius ibidem stare non potest: radius corporis
     * adhibetur */
    o[ZEPHYRUM]  = _scriptum(c, "pond", 2.0f, 2.0f, 3.0f, IX);
    f            = mundus_ostia_probare(o, I, solum, 30.0f, c->piscina);
    RECUSATUR (f, IX, "stand");
    /* crux aquae: solum siccum solum in angulis. Radius circulus
     * est, non quadratum: 2 angulos non attingit (distant 2.3),
     * 2.5 attingit */
    {
        LociFictiQuadratum  q[III];
               MundusSolum* crux;

        q[ZEPHYRUM]  = loci_ficti_quadratum(0.0f, 8.0f, 0.0f, 8.0f,
            0.0f, 0.0f, LOCI_FICTI_GRESSIBILE);
        q[I]         = loci_ficti_quadratum(3.0f, 5.0f, 0.0f, 8.0f,
            0.0f, 0.0f, STATUA_SIGNUM_AQUA);
        q[II]        = loci_ficti_quadratum(0.0f, 8.0f, 3.0f, 5.0f,
            0.0f, 0.0f, STATUA_SIGNUM_AQUA);
        crux = mundus_solum_struere(loci_ficti_statua(c, q, III),
            c->piscina);
        CREDO_NECESSE (crux != NIHIL);
        o[ZEPHYRUM] = _scriptum(c, "ford", 4.0f, 4.0f, 2.0f, XI);
        f = mundus_ostia_probare(o, I, crux, MUNDUS_CORPORIS_RADIUS,
            c->piscina);
        RECUSATUR (f, XI, "stand");
        o[ZEPHYRUM] = _scriptum(c, "ford", 4.0f, 4.0f, 2.5f, XI);
        f = mundus_ostia_probare(o, I, crux, MUNDUS_CORPORIS_RADIUS,
            c->piscina);
        CREDO_VERUM (f.successus);
    }
    /* duo in puncto eodem: linea SECUNDI */
    o[ZEPHYRUM]  = _scriptum(c, "hut-door", 2.0f, 10.0f, 1.5f, III);
    o[I]         = _scriptum(c, "cave_1", 2.0f, 12.0f, 1.5f, IV);
    o[II]        = _scriptum(c, "cave_2", 2.0f, 10.0f, 2.0f, V);
    f = mundus_ostia_probare(o, III, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, V, "same spot");
    /* primum culpatum vincit */
    o[ZEPHYRUM]  = _scriptum(c, "hut-door", 2.0f, 10.0f, 1.5f, III);
    o[I]         = _scriptum(c, "cave_1", 40.0f, 40.0f, 1.5f, IV);
    o[II]        = _scriptum(c, "Bad", 2.0f, 12.0f, 1.5f, V);
    f = mundus_ostia_probare(o, III, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    RECUSATUR (f, IV, "outside");
    /* nimis multa: LXIV licent, LXV non */
    multa = (MundusLocusOstium*)piscina_allocare(c->piscina,
        (MUNDUS_OSTIA_MAXIMA + I) * magnitudo(MundusLocusOstium));
    CREDO_NECESSE (multa != NIHIL);
    per (k = ZEPHYRUM; k <= MUNDUS_OSTIA_MAXIMA; k++)
    {
        multa[k] = _scriptum(c, "hut-door",
            0.75f + 0.0625f * (f32)k, 10.0f, 1.5f, k + X);
    }
    f = mundus_ostia_probare(multa, MUNDUS_OSTIA_MAXIMA, solum,
        MUNDUS_CORPORIS_RADIUS, c->piscina);
    CREDO_VERUM (f.successus);
    CREDO_AEQUALIS_I32 (f.numerus, MUNDUS_OSTIA_MAXIMA);
    f = mundus_ostia_probare(multa, MUNDUS_OSTIA_MAXIMA + I, solum,
        MUNDUS_CORPORIS_RADIUS, c->piscina);
    RECUSATUR (f, MUNDUS_OSTIA_MAXIMA + X, "too many");
    /* argumenta NIHIL: FALSUM, linea 0 */
    o[ZEPHYRUM] = _scriptum(c, "hut-door", 2.0f, 10.0f, 1.5f, III);
    f = mundus_ostia_probare(o, I, NIHIL, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    CREDO_FALSUM (f.successus);
    CREDO_AEQUALIS_I32 (f.linea, ZEPHYRUM);
    f = mundus_ostia_probare(NIHIL, I, solum, MUNDUS_CORPORIS_RADIUS,
        c->piscina);
    CREDO_FALSUM (f.successus);
    f = mundus_ostia_probare(o, I, solum, MUNDUS_CORPORIS_RADIUS,
        NIHIL);
    CREDO_FALSUM (f.successus);
}


/* ==================================================
 * Volumen
 * ================================================== */

interior vacuum
_sectio_volumen (
    CredoContextus* c)
{
         Volumen* v = volumen_temporarium(c->piscina, "ostia_vol");
             Xar* index;
    MundusOstium  o[III];
             s64  seq;

    CREDO_NECESSE (v != NIHIL);
    /* locus ignotus: index vacuus, non NIHIL */
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (index != NIHIL);
    CREDO_AEQUALIS_I32 (xar_numerus(index), ZEPHYRUM);
    /* index vacuus loco sine ostiis: VERUM, nihil scriptum */
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), NIHIL,
        ZEPHYRUM));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    /* posita redeunt, ordine et campis omnibus (etiam negativis) */
    o[ZEPHYRUM]  = _ostium(c, "hut-door", 29000, 46500, 1500);
    o[I]         = _ostium(c, "cave_1", -4250, 7001, 500);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, II));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (index != NIHIL);
    CREDO_NECESSE (xar_numerus(index) == II);
    CREDO_VERUM (_par(_lectum(index, ZEPHYRUM), &o[ZEPHYRUM]));
    CREDO_VERUM (_par(_lectum(index, I), &o[I]));
    /* eadem iterum: nihil scriptum */
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, II));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    /* campus UNUS mutatus (numero eodem) scribitur: quisque
     * quattuor */
    o[ZEPHYRUM]  = _ostium(c, "hut-door", 29000, 46500, 1501);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, II));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    seq          = volumen_seq_ultimum(v);
    o[ZEPHYRUM]  = _ostium(c, "hut-door", 29001, 46500, 1501);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, II));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    seq          = volumen_seq_ultimum(v);
    o[ZEPHYRUM]  = _ostium(c, "hut-door", 29001, 46501, 1501);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, II));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    seq          = volumen_seq_ultimum(v);
    o[ZEPHYRUM]  = _ostium(c, "hut-gate", 29001, 46501, 1501);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, II));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (xar_numerus(index) == II);
    CREDO_VERUM (_par(_lectum(index, ZEPHYRUM), &o[ZEPHYRUM]));
    CREDO_VERUM (_par(_lectum(index, I), &o[I]));
    /* novissimum vincit TOTUM */
    seq          = volumen_seq_ultimum(v);
    o[ZEPHYRUM]  = _ostium(c, "hut-door", 29000, 46500, 1501);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, I));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (xar_numerus(index) == I);
    CREDO_VERUM (_par(_lectum(index, ZEPHYRUM), &o[ZEPHYRUM]));
    /* ordo mutatus mutatio est */
    o[ZEPHYRUM]  = _ostium(c, "a", 1000, 1000, 1500);
    o[I]         = _ostium(c, "b", 2000, 2000, 1500);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, II));
    seq          = volumen_seq_ultimum(v);
    o[II]        = o[ZEPHYRUM];
    o[ZEPHYRUM]  = o[I];
    o[I]         = o[II];
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, II));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (xar_numerus(index) == II);
    CREDO_VERUM (chorda_aequalis_literis(
        _lectum(index, ZEPHYRUM)->scaena, "b"));
    /* locus alius sua habet, nec huius videt */
    o[ZEPHYRUM] = _ostium(c, "cellar", 5000, 5000, 2000);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "cave"), o, I));
    index = mundus_ostia_legere(v, _ch(c, "cave"), c->piscina);
    CREDO_NECESSE (xar_numerus(index) == I);
    CREDO_VERUM (chorda_aequalis_literis(
        _lectum(index, ZEPHYRUM)->scaena, "cellar"));
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), II);
    /* vacua tollunt (scripta: prius erant), deinde nihil scribunt */
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), NIHIL,
        ZEPHYRUM));
    CREDO_VERUM (volumen_seq_ultimum(v) > seq);
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (index != NIHIL);
    CREDO_AEQUALIS_I32 (xar_numerus(index), ZEPHYRUM);
    seq = volumen_seq_ultimum(v);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), NIHIL,
        ZEPHYRUM));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    index = mundus_ostia_legere(v, _ch(c, "cave"), c->piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(index), I);
    /* argumenta mala: FALSUM, nihil scriptum */
    seq          = volumen_seq_ultimum(v);
    o[ZEPHYRUM]  = _ostium(c, "cellar", 5000, 5000, 2000);
    CREDO_FALSUM (mundus_ostia_ponere(NIHIL, _ch(c, "cave"), o, I));
    CREDO_FALSUM (mundus_ostia_ponere(v, _ch(c, ""), o, I));
    CREDO_FALSUM (mundus_ostia_ponere(v, _ch(c, "cave"), NIHIL, I));
    CREDO_VERUM (volumen_seq_ultimum(v) == seq);
    index = mundus_ostia_legere(NIHIL, _ch(c, "cave"), c->piscina);
    CREDO_VERUM (index == NIHIL || xar_numerus(index) == ZEPHYRUM);
    volumen_claudere(v);
}

interior vacuum
_sectio_acta_mala (
    CredoContextus* c)
{
         Volumen* v = volumen_temporarium(c->piscina, "ostia_mala");
             Xar* index;
    MundusOstium  o[I];

    CREDO_NECESSE (v != NIHIL);
    o[ZEPHYRUM] = _ostium(c, "hut-door", 29000, 46500, 1500);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, I));
    /* acta quae legi non possunt praetereuntur: prius valet */
    CREDO_VERUM (volumen_actum_appendere(v, MUNDUS_OSTIA_ACTUM,
        _ch(c, "non json {")) > ZEPHYRUM);
    CREDO_VERUM (volumen_actum_appendere(v, MUNDUS_OSTIA_ACTUM,
        _ch(c, "{\"locus\":\"clearing\"}")) > ZEPHYRUM);
    CREDO_VERUM (volumen_actum_appendere(v, MUNDUS_OSTIA_ACTUM,
        _ch(c, "{\"locus\":\"clearing\",\"ostia\":7}")) > ZEPHYRUM);
    /* ostium unum malum actum TOTUM tollit */
    CREDO_VERUM (volumen_actum_appendere(v, MUNDUS_OSTIA_ACTUM,
        _ch(c, "{\"locus\":\"clearing\",\"ostia\":["
            "{\"x\":1000,\"z\":1000,\"radius\":1500,"
            "\"scaena\":\"good\"},"
            "{\"x\":1000,\"z\":2000,\"radius\":1500,"
            "\"scaena\":\"Not Good\"}]}")) > ZEPHYRUM);
    CREDO_VERUM (volumen_actum_appendere(v, MUNDUS_OSTIA_ACTUM,
        _ch(c, "{\"locus\":\"clearing\",\"ostia\":["
            "{\"x\":1000,\"z\":1000,\"radius\":9000,"
            "\"scaena\":\"good\"}]}")) > ZEPHYRUM);
    CREDO_VERUM (volumen_actum_appendere(v, MUNDUS_OSTIA_ACTUM,
        _ch(c, "{\"locus\":\"clearing\",\"ostia\":["
            "{\"x\":\"1000\",\"z\":1000,\"radius\":1500,"
            "\"scaena\":\"good\"}]}")) > ZEPHYRUM);
    CREDO_VERUM (volumen_actum_appendere(v, MUNDUS_OSTIA_ACTUM,
        _ch(c, "{\"locus\":\"clearing\",\"ostia\":["
            "{\"z\":1000,\"radius\":1500,"
            "\"scaena\":\"good\"}]}")) > ZEPHYRUM);
    /* actum generis alieni cum forma eadem nihil mutat */
    CREDO_VERUM (volumen_actum_appendere(v, "ostia-aliena",
        _ch(c, "{\"locus\":\"clearing\",\"ostia\":[]}"))
        > ZEPHYRUM);
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (index != NIHIL);
    CREDO_NECESSE (xar_numerus(index) == I);
    CREDO_VERUM (_par(_lectum(index, ZEPHYRUM), &o[ZEPHYRUM]));
    /* actum bonum manu scriptum legitur */
    CREDO_VERUM (volumen_actum_appendere(v, MUNDUS_OSTIA_ACTUM,
        _ch(c, "{\"locus\":\"clearing\",\"ostia\":["
            "{\"x\":-1000,\"z\":2000,\"radius\":500,"
            "\"scaena\":\"good\"}]}")) > ZEPHYRUM);
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (xar_numerus(index) == I);
    CREDO_VERUM (chorda_aequalis_literis(
        _lectum(index, ZEPHYRUM)->scaena, "good"));
    CREDO_VERUM (_lectum(index, ZEPHYRUM)->x_millesimae == -1000);
    CREDO_VERUM (_lectum(index, ZEPHYRUM)->z_millesimae == 2000);
    CREDO_VERUM (_lectum(index, ZEPHYRUM)->radius_millesimae == 500);
    volumen_claudere(v);
}

interior vacuum
_sectio_separata (
    CredoContextus* c)
{
         Volumen* v = volumen_temporarium(c->piscina, "ostia_sep");
             Xar* index;
    MundusOstium  o[I];

    CREDO_NECESSE (v != NIHIL);
    o[ZEPHYRUM] = _ostium(c, "hut-door", 29000, 46500, 1500);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, I));
    /* indices statuarum et scaenarum ostia non vident */
    CREDO_AEQUALIS_I32 (xar_numerus(statuae_enumerare(v, c->piscina)),
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (xar_numerus(scaena_enumerare(v, c->piscina)),
        ZEPHYRUM);
    /* nec ostia scaenas: scaena addita nihil mutat */
    CREDO_VERUM (scaena_addere(v, _ch(c, "hut-door"),
        _ch(c, "octeti")));
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (xar_numerus(index) == I);
    CREDO_VERUM (_par(_lectum(index, ZEPHYRUM), &o[ZEPHYRUM]));
    /* transactio vocatoris iungitur: revoluta nihil relinquit */
    CREDO_VERUM (volumen_transactionem_incipere(v));
    o[ZEPHYRUM] = _ostium(c, "cellar", 1000, 1000, 1500);
    CREDO_VERUM (mundus_ostia_ponere(v, _ch(c, "clearing"), o, I));
    CREDO_VERUM (volumen_in_transactione(v));
    CREDO_VERUM (volumen_transactionem_revolvere(v));
    index = mundus_ostia_legere(v, _ch(c, "clearing"), c->piscina);
    CREDO_NECESSE (xar_numerus(index) == I);
    CREDO_VERUM (chorda_aequalis_literis(
        _lectum(index, ZEPHYRUM)->scaena, "hut-door"));
    volumen_claudere(v);
}

interior vacuum
_sectio_proximus (
    CredoContextus* c)
{
    LociFictiQuadratum  q[II];
           MundusSolum* s;
                   f32  x = -9.0f;
                   f32  z = -9.0f;

    q[ZEPHYRUM]  = loci_ficti_quadratum(0.0f, 8.0f, 0.0f, 8.0f, 0.0f,
        0.0f, LOCI_FICTI_GRESSIBILE);
    q[I]         = loci_ficti_quadratum(5.0f, 8.0f, 0.0f, 8.0f, 0.0f,
        0.0f, STATUA_SIGNUM_AQUA);
    s = mundus_solum_struere(loci_ficti_statua(c, q, II), c->piscina);
    CREDO_NECESSE (s != NIHIL);
    /* punctum in centro cellae: ipsum (cellae quartae unitatis) */
    CREDO_VERUM (mundus_ostia_locus_proximus(s, 2.125f, 4.125f, 1.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_F32_PROXIMUS (x, 2.125f, 0.0001f);
    CREDO_F32_PROXIMUS (z, 4.125f, 0.0001f);
    /* radius 0 in centro licet; extra centrum nulla */
    CREDO_VERUM (mundus_ostia_locus_proximus(s, 2.125f, 4.125f, 0.0f,
        MUNDUS_CORPORIS_RADIUS, NIHIL, NIHIL));
    x = -9.0f;
    z = -9.0f;
    CREDO_FALSUM (mundus_ostia_locus_proximus(s, 2.2f, 4.125f, 0.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_F32_PROXIMUS (x, -9.0f, 0.0001f);
    CREDO_F32_PROXIMUS (z, -9.0f, 0.0001f);
    /* radius negativus, etiam parvus (quadratum eius cellam
     * centri caperet) */
    CREDO_FALSUM (mundus_ostia_locus_proximus(s, 2.125f, 4.125f,
        -0.05f, MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_FALSUM (mundus_ostia_locus_proximus(s, 2.125f, 4.125f,
        -1.0f, MUNDUS_CORPORIS_RADIUS, &x, &z));
    /* proxima, non prima intra radium */
    CREDO_VERUM (mundus_ostia_locus_proximus(s, 2.2f, 4.3f, 1.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_F32_PROXIMUS (x, 2.125f, 0.0001f);
    CREDO_F32_PROXIMUS (z, 4.375f, 0.0001f);
    /* paria: prius ordine tabulae (z, deinde x) */
    CREDO_VERUM (mundus_ostia_locus_proximus(s, 2.25f, 4.125f, 1.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_F32_PROXIMUS (x, 2.125f, 0.0001f);
    CREDO_F32_PROXIMUS (z, 4.125f, 0.0001f);
    CREDO_VERUM (mundus_ostia_locus_proximus(s, 2.125f, 4.25f, 1.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_F32_PROXIMUS (x, 2.125f, 0.0001f);
    CREDO_F32_PROXIMUS (z, 4.125f, 0.0001f);
    /* in aqua (x 5 .. 8): cella sicca proxima ubi corpus TOTUM stat
     * (dimidia unitas a ripa) */
    CREDO_FALSUM (mundus_ostia_locus_proximus(s, 6.125f, 4.125f, 1.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_VERUM (mundus_ostia_locus_proximus(s, 6.125f, 4.125f, 3.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_VERUM (x < 5.0f && x > 4.0f);
    CREDO_F32_PROXIMUS (z, 4.125f, 0.0001f);
    CREDO_VERUM (mundus_solum_stare(s, x, z, MUNDUS_CORPORIS_RADIUS));
    CREDO_FALSUM (mundus_solum_stare(s, x + 0.25f, z,
        MUNDUS_CORPORIS_RADIUS));
    /* radius circulus: ab (-1, -1) cella anguli proxima oblique
     * iacet - radius lateri par eam non attingit, diagonali par
     * attingit */
    CREDO_NECESSE (mundus_ostia_locus_proximus(s, -1.0f, -1.0f, 8.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_F32_PROXIMUS (x, z, 0.0001f);
    CREDO_VERUM (x > 0.0f && x < 1.0f);
    CREDO_FALSUM (mundus_ostia_locus_proximus(s, -1.0f, -1.0f,
        x + 1.0f + 0.01f, MUNDUS_CORPORIS_RADIUS, NIHIL, NIHIL));
    CREDO_VERUM (mundus_ostia_locus_proximus(s, -1.0f, -1.0f,
        (x + 1.0f) * 1.4143f, MUNDUS_CORPORIS_RADIUS, NIHIL, NIHIL));
    /* extra locum: procul nulla, radio magno margo */
    CREDO_FALSUM (mundus_ostia_locus_proximus(s, -3.0f, 4.125f, 1.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_FALSUM (mundus_ostia_locus_proximus(s, 999.0f, 999.0f, 8.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_VERUM (mundus_ostia_locus_proximus(s, -3.0f, 4.125f, 5.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
    CREDO_VERUM (x > 0.0f && x < 1.0f);
    CREDO_F32_PROXIMUS (z, 4.125f, 0.0001f);
    /* corpus latius alibi stat */
    CREDO_VERUM (mundus_ostia_locus_proximus(s, -3.0f, 4.125f, 6.0f,
        1.5f, &z, NIHIL));
    CREDO_VERUM (z > x);
    CREDO_FALSUM (mundus_ostia_locus_proximus(NIHIL, 2.0f, 2.0f, 1.0f,
        MUNDUS_CORPORIS_RADIUS, &x, &z));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere", _sectio_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legere_recusat", _sectio_legere_recusat, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "probare", _sectio_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "probare_recusat", _sectio_probare_recusat, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "volumen", _sectio_volumen, NIHIL, NIHIL, NIHIL, NIHIL },
    { "acta_mala", _sectio_acta_mala, NIHIL, NIHIL, NIHIL, NIHIL },
    { "separata", _sectio_separata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "proximus", _sectio_proximus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("mundus_ostia", SECTIONES);
}
