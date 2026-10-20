#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "tempus.h"
#include "credo.h"
#include <math.h>
#include <signal.h>
#include <unistd.h>

/* I. interpolare_angulum: via brevior etiam cum b - a < -pi */
interior vacuum
_sectio_angulum_via_brevior (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_F64_PROXIMUS((f64)interpolare_angulum(3.0f, -3.0f, 1.0f) - 3.0,
        0.2831853, 1e-5);
    CREDO_F64_PROXIMUS((f64)interpolare_angulum(3.0f, -3.0f, 0.5f),
        3.1415927, 1e-5);
    CREDO_F64_PROXIMUS((f64)interpolare_angulum(0.0f, -4.712389f, 1.0f),
        1.5707963, 1e-5);
    CREDO_F64_PROXIMUS((f64)interpolare_angulum(0.1f, -6.1831853f, 0.5f),
        0.1, 1e-5);
}

/* II. interpolare_colorem: t extra 0..1 canales vicinos non corrumpit */
interior vacuum
_sectio_colorem_extra_ambitum (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_AEQUALIS_I32(interpolare_colorem(0x00000000, 0x000000FF, 2.0f),
        0x000000FF);
    CREDO_AEQUALIS_I32(interpolare_colorem(0x00000000, 0x000000FF, -1.0f),
        0x00000000);
    CREDO_AEQUALIS_I32(interpolare_colorem(0x00000000, 0x000000FF,
        1.373f) & 0xFFFFFF00, ZEPHYRUM);
}

/* III. circumvolvere redit */
interior vacuum _volvere_nulla (vacuum) { (vacuum)circumvolvere(0.0f, 0.0f, 0.0f); }
interior vacuum _volvere_longe (vacuum) { (vacuum)circumvolvere(1.0e8f, 0.0f, 1.0f); }
interior vacuum _volvere_inversum (vacuum) { (vacuum)circumvolvere(0.5f, 1.0f, 0.0f); }

interior vacuum
_sectio_circumvolvere_redit (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_NON_PENDET(_volvere_nulla(), M);
    CREDO_NON_PENDET(_volvere_longe(), M);
    CREDO_NON_PENDET(_volvere_inversum(), M);
}

/* IV. elastrum_ad quadro longo */
interior vacuum
_sectio_elastrum_quadrum_longum (
    CredoContextus* c)
{
    f32 x;
    f32 v;
    f32 summum;
    i32 k;

    (vacuum)c;
    x = 0.0f; v = 0.0f; summum = 0.0f;
    per (k = ZEPHYRUM; k < XL; k++)
    {
        x = elastrum_ad(x, 1.0f, &v, 100.0f, 10.0f, 0.25f);
        si ((f32)fabs(x) > summum) summum = (f32)fabs(x);
    }
    CREDO_VERUM(summum < 2.0f);
    CREDO_F64_PROXIMUS((f64)x, 1.0, 0.05);

    x = 0.0f; v = 0.0f; summum = 0.0f;
    per (k = ZEPHYRUM; k < XL; k++)
    {
        x = elastrum_ad(x, 1.0f, &v, 10.0f, 30.0f, 0.1f);
        si ((f32)fabs(x) > summum) summum = (f32)fabs(x);
    }
    CREDO_VERUM(summum <= 1.0001f);
}

/* V. media finita */
interior vacuum
_sectio_statisticae_finitae (
    CredoContextus* c)
{
    StatisticaeQuadri* s;
    f64 media;
    i32 non_finitae;
    i32 k;

    (vacuum)c;
    non_finitae = ZEPHYRUM;
    per (k = ZEPHYRUM; k < M; k++)
    {
        tempus_initiare();
        tempus_quadrum();
        s = tempus_obtinere_statisticas();
        media = s->quadra_media_per_secundum;
        si (!(media == media && media < 1.0e300)) non_finitae++;
    }
    CREDO_AEQUALIS_I32(non_finitae, ZEPHYRUM);
}

/* VI. tempus_creare in piscina plena -> NIHIL */
interior vacuum
_creare_in_plena (vacuum)
{
    Piscina* f = piscina_generare_certae_magnitudinis("certa", XVI);
    si (tempus_creare(f) == NIHIL) raise(SIGUSR1);
}

interior vacuum
_sectio_creare_plena (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_RUIT_CUM(_creare_in_plena(), SIGUSR1);
}

/* VII. undae et oscillare tempore negativo */
interior vacuum
_sectio_undae_tempus_negativum (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_F64_PROXIMUS((f64)unda_triangulus(-0.25f, 1.0f, 1.0f), 0.0, 1e-6);
    CREDO_F64_PROXIMUS((f64)unda_triangulus(-0.75f, 1.0f, 1.0f), 0.0, 1e-6);
    CREDO_F64_PROXIMUS((f64)unda_dens_serrae(-0.25f, 1.0f, 1.0f), 0.5, 1e-6);
    CREDO_F64_PROXIMUS((f64)unda_quadratus(-0.25f, 1.0f, 1.0f), -1.0, 1e-6);
    CREDO_F64_PROXIMUS((f64)unda_triangulus(0.25f, -1.0f, 1.0f), 0.0, 1e-6);
    CREDO_F64_PROXIMUS((f64)oscillare(-0.5f, 1.0f), 0.5, 1e-6);
    CREDO_F64_PROXIMUS((f64)oscillare(-1.5f, 1.0f), 0.5, 1e-6);
}

/* VIII. interpolare(a, b, 1) == b */
interior vacuum
_sectio_interpolare_finis (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(interpolare(1.0e8f, 1.0f, 1.0f) == 1.0f);
    CREDO_VERUM(interpolare(3.0f, 0.1f, 1.0f) == 0.1f);
    CREDO_VERUM(interpolare(-1.0e7f, 0.3f, 1.0f) == 0.3f);
    CREDO_VERUM(interpolare(3.0f, 0.1f, 0.0f) == 3.0f);
}

/* IX. gradus_lenis limina aequalia */
interior vacuum
_sectio_gradus_lenis_limina_aequalia (
    CredoContextus* c)
{
    f32 g;

    (vacuum)c;
    g = gradus_lenis(1.0f, 1.0f, 1.0f);
    CREDO_VERUM(g == g);
    CREDO_VERUM(g >= 0.0f && g <= 1.0f);
}

/* X. animare_ad numquam recedit */
interior vacuum
_sectio_animare_non_recedit (
    CredoContextus* c)
{
    f32 x;
    i32 k;

    (vacuum)c;
    x = 0.0f;
    per (k = ZEPHYRUM; k < V; k++)
    {
        x = animare_ad(x, 10.0f, -2.0f, 1.0f);
    }
    CREDO_VERUM(x >= 0.0f && x <= 10.0f);
    x = animare_ad(0.0f, 10.0f, 1.0f, -0.1f);
    CREDO_VERUM(x >= 0.0f && x <= 10.0f);
}

/* XI. colorem alpha plenus (sub UBSan) */
interior vacuum
_sectio_colorem_alpha_plenus (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_AEQUALIS_I32(interpolare_colorem(0xFF000000, 0xFF0000FF, 0.5f),
        0xFF00007F);
    CREDO_AEQUALIS_I32(interpolare_colorem(0x80FFFFFF, 0xFF000000, 1.0f),
        0xFF000000);
}

/* XII. exspectare periodus longa */
interior vacuum
_exspectare_longe (vacuum)
{
    alarm(I);
    tempus_initiare();
    tempus_ponere_quadra_destinata(1e-10);
    tempus_exspectare_quadrum();
}

interior vacuum
_sectio_exspectare_periodus_longa (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_RUIT_CUM_INTRA(_exspectare_longe(), SIGALRM, MMM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "angulum", _sectio_angulum_via_brevior, NIHIL, NIHIL, NIHIL, NIHIL },
    { "colorem extra", _sectio_colorem_extra_ambitum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "circumvolvere", _sectio_circumvolvere_redit, NIHIL, NIHIL, NIHIL, NIHIL },
    { "elastrum", _sectio_elastrum_quadrum_longum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "statisticae", _sectio_statisticae_finitae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "creare plena", _sectio_creare_plena, NIHIL, NIHIL, NIHIL, NIHIL },
    { "undae", _sectio_undae_tempus_negativum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "interpolare", _sectio_interpolare_finis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "gradus lenis", _sectio_gradus_lenis_limina_aequalia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "animare", _sectio_animare_non_recedit, NIHIL, NIHIL, NIHIL, NIHIL },
    { "colorem alpha", _sectio_colorem_alpha_plenus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "exspectare", _sectio_exspectare_periodus_longa, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tempus_drafts", SECTIONES);
}
