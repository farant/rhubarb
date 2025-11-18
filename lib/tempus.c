#include "tempus.h"
#include "piscina.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* Horologium globale et statisticae */
hic_manens Tempus g_horologium;
hic_manens StatisticaeQuadri g_statisticae;
hic_manens b32 g_initiatus = FALSUM;

/* Declarationes functionum platformae (implementatae in fenestra_macos.m) */
externus i64 fenestra_tempus_obtinere_pulsus(vacuum);
externus f64 fenestra_tempus_obtinere_frequentiam(vacuum);
externus vacuum fenestra_dormire(i32 microsecundae);


/* ==================================================
 * Horologium Globale
 * ================================================== */

vacuum
tempus_initiare (
    vacuum)
{
    memset(&g_horologium, ZEPHYRUM, magnitudo(Tempus));
    memset(&g_statisticae, ZEPHYRUM, magnitudo(StatisticaeQuadri));

    g_horologium.frequentia = fenestra_tempus_obtinere_frequentiam();
    g_horologium.pulsus_initii = fenestra_tempus_obtinere_pulsus();
    g_horologium.pulsus_ultimi_quadri = g_horologium.pulsus_initii;
    g_horologium.pulsus_currentis = g_horologium.pulsus_initii;

    g_statisticae.quadra_destinata_per_secundum = 60.0;
    g_statisticae.tempus_minimum_quadri = 1000000.0;

    g_initiatus = VERUM;
}

f64
tempus_nunc (
    vacuum)
{
    i64 currens;

    si (!g_initiatus)
    {
        tempus_initiare();
    }

    currens = fenestra_tempus_obtinere_pulsus();
    redde (f64)(currens - g_horologium.pulsus_initii) / g_horologium.frequentia;
}

f64
tempus_delta (
    vacuum)
{
    si (!g_initiatus)
    {
        tempus_initiare();
    }

    redde g_horologium.delta_tempus;
}

vacuum
tempus_quadrum (
    vacuum)
{
    i64 currens;
    i64 elapsum;
    f64 summa;
    i32 numerus;
    i32 i;

    si (!g_initiatus)
    {
        tempus_initiare();
    }

    currens = fenestra_tempus_obtinere_pulsus();
    elapsum = currens - g_horologium.pulsus_ultimi_quadri;

    g_horologium.delta_tempus = (f64)elapsum / g_horologium.frequentia;
    g_horologium.tempus_totale = (f64)(currens - g_horologium.pulsus_initii) / g_horologium.frequentia;
    g_horologium.pulsus_ultimi_quadri = currens;
    g_horologium.pulsus_currentis = currens;

    /* Actualizare statisticas quadri */
    g_statisticae.numerus_quadrorum++;
    g_statisticae.tempora_quadrorum[g_statisticae.index_temporis_quadri] = g_horologium.delta_tempus;
    g_statisticae.index_temporis_quadri = (g_statisticae.index_temporis_quadri + I) % CXX;

    si (g_horologium.delta_tempus < g_statisticae.tempus_minimum_quadri && g_horologium.delta_tempus > 0.0)
    {
        g_statisticae.tempus_minimum_quadri = g_horologium.delta_tempus;
    }
    si (g_horologium.delta_tempus > g_statisticae.tempus_maximum_quadri)
    {
        g_statisticae.tempus_maximum_quadri = g_horologium.delta_tempus;
    }

    /* Calculare medium quadrorum per secundum super ultimos 120 quadros */
    summa = 0.0;
    numerus = g_statisticae.numerus_quadrorum < CXX ? g_statisticae.numerus_quadrorum : CXX;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        summa += g_statisticae.tempora_quadrorum[i];
    }
    g_statisticae.quadra_media_per_secundum = numerus > ZEPHYRUM ? (f64)numerus / summa : 0.0;
    g_statisticae.quadra_currentia_per_secundum = g_horologium.delta_tempus > 0.0 ? 1.0 / g_horologium.delta_tempus : 0.0;
}

Tempus*
tempus_obtinere_horologium (
    vacuum)
{
    si (!g_initiatus)
    {
        tempus_initiare();
    }

    redde &g_horologium;
}


/* ==================================================
 * Administratio Quadrorum per Secundum
 * ================================================== */

vacuum
tempus_ponere_quadra_destinata (
    f64 quadra_per_secundum)
{
    si (!g_initiatus)
    {
        tempus_initiare();
    }

    g_statisticae.quadra_destinata_per_secundum = quadra_per_secundum;
}

vacuum
tempus_exspectare_quadrum (
    vacuum)
{
    f64 tempus_destinatum_quadri;
    i64 currens;
    f64 elapsum;
    f64 remanens;
    i32 dormire_ms;

    si (!g_initiatus)
    {
        tempus_initiare();
    }

    si (g_statisticae.quadra_destinata_per_secundum <= 0.0)
    {
        redde;
    }

    tempus_destinatum_quadri = 1.0 / g_statisticae.quadra_destinata_per_secundum;
    currens = fenestra_tempus_obtinere_pulsus();
    elapsum = (f64)(currens - g_horologium.pulsus_ultimi_quadri) / g_horologium.frequentia;
    remanens = tempus_destinatum_quadri - elapsum;

    /* Si habemus plus quam 2ms remanens, dormire pro maior parte eius */
    dum (remanens > 0.002)
    {
        /* Dormire pro maior parte temporis remanentis, relinquere 2ms pro praecisione */
        dormire_ms = (i32)((remanens - 0.002) * 1000.0);
        si (dormire_ms > ZEPHYRUM)
        {
            fenestra_dormire(dormire_ms * M);
        }

        currens = fenestra_tempus_obtinere_pulsus();
        elapsum = (f64)(currens - g_horologium.pulsus_ultimi_quadri) / g_horologium.frequentia;
        remanens = tempus_destinatum_quadri - elapsum;
    }

    /* Busy-wait pro finali ~2ms pro praecisione */
    dum (elapsum < tempus_destinatum_quadri)
    {
        /* Parvum dormire ut vitare consumere CPU etiam in fine */
        fenestra_dormire(C);  /* Dormire pro 100 microsecundae */
        currens = fenestra_tempus_obtinere_pulsus();
        elapsum = (f64)(currens - g_horologium.pulsus_ultimi_quadri) / g_horologium.frequentia;
    }
}

StatisticaeQuadri*
tempus_obtinere_statisticas (
    vacuum)
{
    si (!g_initiatus)
    {
        tempus_initiare();
    }

    redde &g_statisticae;
}


/* ==================================================
 * Horologia Independentia
 * ================================================== */

Tempus*
tempus_creare (
    Piscina* piscina)
{
    Tempus* tempus;

    si (!piscina)
    {
        redde NIHIL;
    }

    tempus = (Tempus*)piscina_allocare(piscina, magnitudo(Tempus));
    si (!tempus)
    {
        redde NIHIL;
    }

    memset(tempus, ZEPHYRUM, magnitudo(Tempus));
    tempus->frequentia = fenestra_tempus_obtinere_frequentiam();
    tempus->pulsus_initii = fenestra_tempus_obtinere_pulsus();
    tempus->pulsus_ultimi_quadri = tempus->pulsus_initii;
    tempus->pulsus_currentis = tempus->pulsus_initii;

    redde tempus;
}

vacuum
tempus_restituere (
    Tempus* tempus)
{
    i64 pulsus_initii;

    si (!tempus)
    {
        redde;
    }

    pulsus_initii = fenestra_tempus_obtinere_pulsus();
    tempus->pulsus_initii = pulsus_initii;
    tempus->pulsus_ultimi_quadri = pulsus_initii;
    tempus->pulsus_currentis = pulsus_initii;
    tempus->delta_tempus = 0.0;
    tempus->tempus_totale = 0.0;
}

f64
tempus_elapsum (
    Tempus* tempus)
{
    i64 currens;

    si (!tempus)
    {
        redde 0.0;
    }

    currens = fenestra_tempus_obtinere_pulsus();
    redde (f64)(currens - tempus->pulsus_initii) / tempus->frequentia;
}

vacuum
tempus_actualizare (
    Tempus* tempus)
{
    i64 currens;
    i64 elapsum;

    si (!tempus)
    {
        redde;
    }

    currens = fenestra_tempus_obtinere_pulsus();
    elapsum = currens - tempus->pulsus_ultimi_quadri;
    tempus->delta_tempus = (f64)elapsum / tempus->frequentia;
    tempus->tempus_totale = (f64)(currens - tempus->pulsus_initii) / tempus->frequentia;
    tempus->pulsus_ultimi_quadri = currens;
    tempus->pulsus_currentis = currens;
}


/* ==================================================
 * Functiones Lenitionis
 * ================================================== */

f32
lenire_linearis (
    f32 t)
{
    redde t;
}

f32
lenire_quadraticus_ingressus (
    f32 t)
{
    redde t * t;
}

f32
lenire_quadraticus_egressus (
    f32 t)
{
    redde t * (2.0f - t);
}

f32
lenire_quadraticus_ingressus_egressus (
    f32 t)
{
    redde t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

f32
lenire_cubicus_ingressus (
    f32 t)
{
    redde t * t * t;
}

f32
lenire_cubicus_egressus (
    f32 t)
{
    f32 t1;

    t1 = t - 1.0f;
    redde t1 * t1 * t1 + 1.0f;
}

f32
lenire_cubicus_ingressus_egressus (
    f32 t)
{
    redde t < 0.5f ? 4.0f * t * t * t : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
}

f32
lenire_sinus_ingressus (
    f32 t)
{
    redde 1.0f - (f32)cos(t * M_PI / 2.0);
}

f32
lenire_sinus_egressus (
    f32 t)
{
    redde (f32)sin(t * M_PI / 2.0);
}

f32
lenire_sinus_ingressus_egressus (
    f32 t)
{
    redde -((f32)cos(M_PI * t) - 1.0f) / 2.0f;
}

f32
lenire_exponens_ingressus (
    f32 t)
{
    redde t == 0.0f ? 0.0f : (f32)pow(2.0, 10.0 * (t - 1.0));
}

f32
lenire_exponens_egressus (
    f32 t)
{
    redde t == 1.0f ? 1.0f : 1.0f - (f32)pow(2.0, -10.0 * t);
}

f32
lenire_exponens_ingressus_egressus (
    f32 t)
{
    si (t == 0.0f) redde 0.0f;
    si (t == 1.0f) redde 1.0f;
    si (t < 0.5f) redde (f32)pow(2.0, 20.0 * t - 10.0) / 2.0f;
    redde (2.0f - (f32)pow(2.0, -20.0 * t + 10.0)) / 2.0f;
}

f32
lenire_retro_ingressus (
    f32 t)
{
    constans f32 c1 = 1.70158f;
    constans f32 c3 = c1 + 1.0f;

    redde c3 * t * t * t - c1 * t * t;
}

f32
lenire_retro_egressus (
    f32 t)
{
    constans f32 c1 = 1.70158f;
    constans f32 c3 = c1 + 1.0f;
           f32 t1;

    t1 = t - 1.0f;
    redde 1.0f + c3 * t1 * t1 * t1 + c1 * t1 * t1;
}

f32
lenire_retro_ingressus_egressus (
    f32 t)
{
    constans f32 c1 = 1.70158f;
    constans f32 c2 = c1 * 1.525f;

    si (t < 0.5f)
    {
        redde ((f32)pow(2.0 * t, 2.0) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f;
    }
    redde ((f32)pow(2.0 * t - 2.0, 2.0) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}

f32
lenire_elasticus_ingressus (
    f32 t)
{
    constans f32 c4 = (2.0f * (f32)M_PI) / 3.0f;

    si (t == 0.0f) redde 0.0f;
    si (t == 1.0f) redde 1.0f;
    redde -(f32)pow(2.0, 10.0 * t - 10.0) * (f32)sin((t * 10.0f - 10.75f) * c4);
}

f32
lenire_elasticus_egressus (
    f32 t)
{
    constans f32 c4 = (2.0f * (f32)M_PI) / 3.0f;

    si (t == 0.0f) redde 0.0f;
    si (t == 1.0f) redde 1.0f;
    redde (f32)pow(2.0, -10.0 * t) * (f32)sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

f32
lenire_elasticus_ingressus_egressus (
    f32 t)
{
    constans f32 c5 = (2.0f * (f32)M_PI) / 4.5f;

    si (t == 0.0f) redde 0.0f;
    si (t == 1.0f) redde 1.0f;

    si (t < 0.5f)
    {
        redde -((f32)pow(2.0, 20.0 * t - 10.0) * (f32)sin((20.0f * t - 11.125f) * c5)) / 2.0f;
    }
    redde ((f32)pow(2.0, -20.0 * t + 10.0) * (f32)sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

f32
lenire_resiliens_egressus (
    f32 t)
{
    constans f32 n1 = 7.5625f;
    constans f32 d1 = 2.75f;

    si (t < 1.0f / d1)
    {
        redde n1 * t * t;
    }
    alioquin si (t < 2.0f / d1)
    {
        t -= 1.5f / d1;
        redde n1 * t * t + 0.75f;
    }
    alioquin si (t < 2.5f / d1)
    {
        t -= 2.25f / d1;
        redde n1 * t * t + 0.9375f;
    }
    alioquin
    {
        t -= 2.625f / d1;
        redde n1 * t * t + 0.984375f;
    }
}

f32
lenire_resiliens_ingressus (
    f32 t)
{
    redde 1.0f - lenire_resiliens_egressus(1.0f - t);
}

f32
lenire_resiliens_ingressus_egressus (
    f32 t)
{
    si (t < 0.5f)
    {
        redde (1.0f - lenire_resiliens_egressus(1.0f - 2.0f * t)) / 2.0f;
    }
    redde (1.0f + lenire_resiliens_egressus(2.0f * t - 1.0f)) / 2.0f;
}


/* ==================================================
 * Adiutores Animationis
 * ================================================== */

f32
animare_ad (
    f32 currens,
    f32 destinatum,
    f32 celeritas,
    f32 delta_tempus)
{
    f32 differentia;
    f32 gradus;

    differentia = destinatum - currens;
    gradus = celeritas * delta_tempus;

    si ((f32)fabs(differentia) <= gradus)
    {
        redde destinatum;
    }

    redde currens + (differentia > 0.0f ? gradus : -gradus);
}

f32
elastrum_ad (
    f32  currens,
    f32  destinatum,
    f32* velocitas,
    f32  rigiditas,
    f32  damnum,
    f32  delta_tempus)
{
    f32 vis;

    si (!velocitas)
    {
        redde currens;
    }

    vis = rigiditas * (destinatum - currens);
    *velocitas = *velocitas * (1.0f - damnum * delta_tempus) + vis * delta_tempus;
    redde currens + *velocitas * delta_tempus;
}


/* ==================================================
 * Interpolatio
 * ================================================== */

f32
interpolare (
    f32 a,
    f32 b,
    f32 t)
{
    redde a + (b - a) * t;
}

f32
interpolare_angulum (
    f32 a,
    f32 b,
    f32 t)
{
    f32 differentia;

    differentia = (f32)fmod(b - a + M_PI, 2.0 * M_PI) - (f32)M_PI;
    redde a + differentia * t;
}

f32
cohibere (
    f32 valor,
    f32 minimum,
    f32 maximum)
{
    si (valor < minimum) redde minimum;
    si (valor > maximum) redde maximum;
    redde valor;
}

f32
gradus_lenis (
    f32 limen_infimum,
    f32 limen_supremum,
    f32 x)
{
    f32 t;

    t = cohibere((x - limen_infimum) / (limen_supremum - limen_infimum), 0.0f, 1.0f);
    redde t * t * (3.0f - 2.0f * t);
}

i32
interpolare_colorem (
    i32 color_primus,
    i32 color_secundus,
    f32 t)
{
    i8 r1;
    i8 g1;
    i8 b1;
    i8 a1;
    i8 r2;
    i8 g2;
    i8 b2;
    i8 a2;
    i8 r;
    i8 g;
    i8 b;
    i8 a;

    r1 = (i8)((color_primus >> ZEPHYRUM) & CCLV);
    g1 = (i8)((color_primus >> VIII) & CCLV);
    b1 = (i8)((color_primus >> XVI) & CCLV);
    a1 = (i8)((color_primus >> XXIV) & CCLV);

    r2 = (i8)((color_secundus >> ZEPHYRUM) & CCLV);
    g2 = (i8)((color_secundus >> VIII) & CCLV);
    b2 = (i8)((color_secundus >> XVI) & CCLV);
    a2 = (i8)((color_secundus >> XXIV) & CCLV);

    r = (i8)((f32)r1 + (f32)(r2 - r1) * t);
    g = (i8)((f32)g1 + (f32)(g2 - g1) * t);
    b = (i8)((f32)b1 + (f32)(b2 - b1) * t);
    a = (i8)((f32)a1 + (f32)(a2 - a1) * t);

    redde (i32)((a << XXIV) | (b << XVI) | (g << VIII) | r);
}


/* ==================================================
 * Functiones Undae
 * ================================================== */

f32
unda_sinus (
    f32 t,
    f32 frequentia,
    f32 amplitudo)
{
    redde amplitudo * (f32)sin(2.0 * M_PI * frequentia * t);
}

f32
unda_triangulus (
    f32 t,
    f32 frequentia,
    f32 amplitudo)
{
    f32 phase;

    phase = (f32)fmod(t * frequentia, 1.0);
    si (phase < 0.5f)
    {
        redde amplitudo * (4.0f * phase - 1.0f);
    }
    alioquin
    {
        redde amplitudo * (3.0f - 4.0f * phase);
    }
}

f32
unda_dens_serrae (
    f32 t,
    f32 frequentia,
    f32 amplitudo)
{
    redde amplitudo * (2.0f * (f32)fmod(t * frequentia, 1.0) - 1.0f);
}

f32
unda_quadratus (
    f32 t,
    f32 frequentia,
    f32 amplitudo)
{
    redde (f32)fmod(t * frequentia, 1.0) < 0.5f ? amplitudo : -amplitudo;
}

f32
unda_tumultus (
    f32 t)
{
    s32 semen;

    /* Simplex tumultus pseudo-aleatorius basatus in tempore */
    semen = (s32)(t * 12345.6789);
    semen = (semen * 1103515245 + 12345) & 0x7fffffff;
    redde ((f32)semen / (f32)0x7fffffff) * 2.0f - 1.0f;
}


/* ==================================================
 * Utilitas
 * ================================================== */

f32
circumvolvere (
    f32 valor,
    f32 minimum,
    f32 maximum)
{
    f32 amplitudo;

    amplitudo = maximum - minimum;
    dum (valor < minimum)
    {
        valor += amplitudo;
    }
    dum (valor >= maximum)
    {
        valor -= amplitudo;
    }
    redde valor;
}

f32
oscillare (
    f32 t,
    f32 longitudo)
{
    t = (f32)fmod(t, longitudo * 2.0);
    redde longitudo - (f32)fabs(t - longitudo);
}

f32
remappare (
    f32 valor,
    f32 ex_minimum,
    f32 ex_maximum,
    f32 ad_minimum,
    f32 ad_maximum)
{
    f32 t;

    t = (valor - ex_minimum) / (ex_maximum - ex_minimum);
    redde ad_minimum + t * (ad_maximum - ad_minimum);
}
