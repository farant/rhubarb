/* probatio_plastica_membrum.c - membrum duorum ossium ad metam
 * (plastica_membrum.h)
 *
 * trianguli noti; finis in meta per cinematicam directam (rete
 * metarum, uterque flexus); medium ad latus declaratum; fines
 * ambitus (extensio plena, plicatio, radix ipsa); numeri mali.
 */
#include "latina.h"
#include "piscina.h"
#include "plastica_membrum.h"
#include "credo.h"
#include <math.h>
#include <stdio.h>

#define PI_GRADUS  (3.14159265358979323846 / 180.0)

/* cinematica directa: os primum (0, -L1) angulo radicis versum,
 * secundum ab eo angulo medii flexum; rotatio circa x: y' = y cos -
 * z sin, z' = y sin + z cos. *ante = z finis, *infra = -y finis;
 * *genu = z medii */
interior vacuum
_finis (
    f32  prima,
    f32  secunda,
    f32  radicis,
    f32  medii,
    f64* ante,
    f64* infra,
    f64* genu)
{
    f64 a = (f64)radicis * PI_GRADUS;
    f64 m = (f64)medii * PI_GRADUS;
    f64 y = -(f64)prima - (f64)secunda * cos(m);
    f64 z = -(f64)secunda * sin(m);

    *ante   = y * sin(a) + z * cos(a);
    *infra  = -(y * cos(a) - z * sin(a));
    *genu   = -(f64)prima * sin(a);
}

interior vacuum
_sectio_trianguli (
    CredoContextus* c)
{
    PlasticaMembrumSolutio s;

    (vacuum)c;
    /* extensio plena recta deorsum */
    s = plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 2.0f, VERUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_radicis, 0.0, 1.0e-2);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 0.0, 1.0e-2);
    /* triangulum rectum: meta sqrt 2 deorsum */
    s = plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 1.41421356f, VERUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_radicis, -45.0, 1.0e-3);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 90.0, 1.0e-3);
    s = plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 1.41421356f,
        FALSUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_radicis, 45.0, 1.0e-3);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, -90.0, 1.0e-3);
    /* extensio plena antrorsum: radix -90 */
    s = plastica_membrum_solvere(1.0f, 0.5f, 1.5f, 0.0f, VERUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_radicis, -90.0, 1.0e-2);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 0.0, 1.0e-2);
    /* III IV V: ossa III et IV, meta V deorsum - angulus rectus in
     * medio; radix atan(IV / III) */
    s = plastica_membrum_solvere(3.0f, 4.0f, 0.0f, 5.0f, VERUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 90.0, 1.0e-3);
    CREDO_F64_PROXIMUS ((f64)s.angulus_radicis, -53.130102, 1.0e-3);
}

interior vacuum
_sectio_rete (
    CredoContextus* c)
{
    hic_manens constans f32 OSSA[][II] = {
        { 1.0f, 1.0f }, { 0.24f, 0.22f }, { 0.5f, 1.25f },
        { 2.0f, 0.75f }
    };
    PlasticaMembrumSolutio s;
                       f64 ante;
                       f64 infra;
                       f64 genu;
                       i32 k;
                       i32 x;
                       i32 y;
                       i32 flexus;
                       i32 soluta = ZEPHYRUM;

    (vacuum)c;
    per (k = ZEPHYRUM; k < IV; k++)
    {
        f32 prima    = OSSA[k][ZEPHYRUM];
        f32 secunda  = OSSA[k][I];
        f32 ambitus  = prima + secunda;

        per (y = ZEPHYRUM; y <= XX; y++)
        {
            per (x = ZEPHYRUM; x <= XX; x++)
            {
                f32 ma  = ambitus * ((f32)x - 10.0f) / 9.0f;
                f32 mi  = ambitus * ((f32)y - 10.0f) / 9.0f;
                f64 d   = sqrt((f64)ma * ma + (f64)mi * mi);

                per (flexus = ZEPHYRUM; flexus < II; flexus++)
                {
                    s = plastica_membrum_solvere(prima, secunda, ma,
                        mi, flexus ? VERUM : FALSUM);
                    CREDO_VERUM (s.angulus_radicis
                        == s.angulus_radicis);
                    CREDO_VERUM (s.angulus_medii == s.angulus_medii);
                    /* bene intra ambitum: solvitur; bene extra:
                     * recusatur */
                    si (   d < (f64)ambitus - 1.0e-3
                        && d > fabs((f64)prima - (f64)secunda)
                            + 1.0e-3)
                    {
                        CREDO_VERUM (s.successus);
                    }
                    si (   d > (f64)ambitus + 1.0e-3
                        || d < fabs((f64)prima - (f64)secunda)
                            - 1.0e-3)
                    {
                        CREDO_FALSUM (s.successus);
                    }
                    si (!s.successus)
                    {
                        CREDO_F64_PROXIMUS ((f64)s.angulus_radicis,
                            0.0, 1.0e-9);
                        CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 0.0,
                            1.0e-9);
                        perge;
                    }
                    soluta++;
                    _finis(prima, secunda, s.angulus_radicis,
                        s.angulus_medii, &ante, &infra, &genu);
                    CREDO_F64_PROXIMUS (ante, (f64)ma, 1.0e-3);
                    CREDO_F64_PROXIMUS (infra, (f64)mi, 1.0e-3);
                    /* flexus signum tenet */
                    si (flexus)
                    {
                        CREDO_VERUM (s.angulus_medii >= 0.0f);
                    }
                    alioquin
                    {
                        CREDO_VERUM (s.angulus_medii <= 0.0f);
                    }
                    CREDO_VERUM (s.angulus_medii <= 180.0f);
                    CREDO_VERUM (s.angulus_medii >= -180.0f);
                }
            }
        }
    }
    CREDO_VERUM (soluta > M);
    /* medium ad latus declaratum: meta recta deorsum, genu ante
     * (z positivum) aut retro */
    s = plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 1.5f, VERUM);
    _finis(1.0f, 1.0f, s.angulus_radicis, s.angulus_medii, &ante,
        &infra, &genu);
    CREDO_VERUM (genu > 0.5);
    s = plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 1.5f, FALSUM);
    _finis(1.0f, 1.0f, s.angulus_radicis, s.angulus_medii, &ante,
        &infra, &genu);
    CREDO_VERUM (genu < -0.5);
    /* meta supra radicem et retro */
    s = plastica_membrum_solvere(1.0f, 1.0f, -0.5f, -1.0f, VERUM);
    CREDO_NECESSE (s.successus);
    _finis(1.0f, 1.0f, s.angulus_radicis, s.angulus_medii, &ante,
        &infra, &genu);
    CREDO_F64_PROXIMUS (ante, -0.5, 1.0e-4);
    CREDO_F64_PROXIMUS (infra, -1.0, 1.0e-4);
}

interior vacuum
_sectio_fines (
    CredoContextus* c)
{
    PlasticaMembrumSolutio s;
                       f32 nullum       = 0.0f;
                       f32 non_numerus  = nullum / nullum;
                       f32 infinitum    = 1.0f / nullum;
                       f64 ante;
                       f64 infra;
                       f64 genu;

    (vacuum)c;
    /* ultra ambitum; intra marginem extensio plena */
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 2.1f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 2.001f,
        VERUM).successus);
    s = plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 2.00005f, VERUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 0.0, 1.0e-3);
    CREDO_F64_PROXIMUS ((f64)s.angulus_radicis, 0.0, 1.0e-3);
    /* propior quam differentia; plicatio plena ipsa licet */
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 0.5f, 0.0f, 0.4f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(0.5f, 1.0f, 0.0f, 0.4f,
        VERUM).successus);
    s = plastica_membrum_solvere(1.0f, 0.5f, 0.0f, 0.5f, VERUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 180.0, 1.0e-2);
    _finis(1.0f, 0.5f, s.angulus_radicis, s.angulus_medii, &ante,
        &infra, &genu);
    CREDO_F64_PROXIMUS (infra, 0.5, 1.0e-3);
    /* in radice ipsa (ossa paria: differentia 0) */
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 0.0f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 1.0f, 0.0f, 0.0f,
        FALSUM).successus);
    /* longitudines */
    CREDO_FALSUM (plastica_membrum_solvere(0.0f, 1.0f, 0.0f, 0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 0.0f, 0.0f, 0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(-1.0f, 1.0f, 0.0f, 0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, -1.0f, 0.0f, 0.5f,
        VERUM).successus);
    /* numeri non finiti */
    CREDO_FALSUM (plastica_membrum_solvere(non_numerus, 1.0f, 0.0f,
        0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, non_numerus, 0.0f,
        0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 1.0f, non_numerus,
        0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 1.0f, 0.0f,
        non_numerus,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(infinitum, 1.0f, 0.0f, 0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, infinitum, 0.0f, 0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 1.0f, infinitum, 0.5f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 1.0f, 0.0f, -infinitum,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(infinitum, infinitum, 0.0f,
        0.5f, VERUM).successus);
    /* longitudo nulla cum meta ad distantiam alterius */
    CREDO_FALSUM (plastica_membrum_solvere(0.0f, 1.0f, 0.0f, 1.0f,
        VERUM).successus);
    CREDO_FALSUM (plastica_membrum_solvere(1.0f, 0.0f, 0.0f, 1.0f,
        VERUM).successus);
    /* plicatio plena intra marginem: anguli finiti, finis prope */
    s = plastica_membrum_solvere(1.0f, 0.5f, 0.0f, 0.49995f, VERUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 180.0, 1.0e-2);
    CREDO_F64_PROXIMUS ((f64)s.angulus_radicis, 0.0, 1.0e-2);
    s = plastica_membrum_solvere(0.5f, 1.0f, 0.0f, 0.49995f, VERUM);
    CREDO_NECESSE (s.successus);
    CREDO_F64_PROXIMUS ((f64)s.angulus_medii, 180.0, 1.0e-2);
    _finis(0.5f, 1.0f, s.angulus_radicis, s.angulus_medii, &ante,
        &infra, &genu);
    CREDO_F64_PROXIMUS (infra, 0.5, 1.0e-3);
    CREDO_F64_PROXIMUS (ante, 0.0, 1.0e-3);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "trianguli", _sectio_trianguli, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rete", _sectio_rete, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fines", _sectio_fines, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("plastica_membrum", SECTIONES);
}
