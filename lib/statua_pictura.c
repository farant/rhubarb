/* statua_pictura.c - pictor statuae (vide statua_pictura.h)
 *
 * Per triangulum: vertices in spatium oculi -> facies (tergum?
 * claritas) -> sectio ad planum propinquum -> proiectio -> schirmus
 * in punctis fixis (XVI partes pixeli) -> functiones marginum
 * integrae cum regula summi-sinistri (margo communis duorum
 * triangulorum semel pingitur, numquam bis, numquam foramen) ->
 * profunditas -> textura perspectiva correcta.
 */

#include "statua_pictura.h"
#include <math.h>
#include <string.h>

#define PARTES_PIXELI   XVI          /* puncta fixa per pixelum */
#define SCHIRMI_LIMES   1000000.0f   /* coordinatae ultra coguntur */
#define AREA_MINIMA     0.000000000001f
/* profunditas initialis: ultra planum longinquum quoque pingitur
 * (pars in cyclo longe volans), ergo limes non II sed immanis */
#define PROFUNDITAS_VACUA 1.0e30f

/* directio AD lucem in spatio oculi: sinistra, supra, ante */
#define LUX_X   (-0.5f)
#define LUX_Y   0.7f
#define LUX_Z   0.5f

/* vertex in spatio oculi cum coordinatis texturae */
nomen structura {
    f32 x;
    f32 y;
    f32 z;
    f32 s;
    f32 t;
} VertexOculi;

/* vertex in schirmo: puncta fixa, profunditas, 1/w, s/w, t/w */
nomen structura {
    s32 x;
    s32 y;
    f32 z;
    f32 iw;
    f32 sw;
    f32 tw;
} VertexSchirmi;

nomen structura {
     i8* rgba;
     i8* tectum;
    f32* profunditas;
    i32  latitudo;
    i32  altitudo;
    f32  fx;            /* proiectio: x' = fx * x / -z */
    f32  fy;
    f32  za;            /* z' = (za * z + zb) / -z */
    f32  zb;
    f32  prope;
} Tabula;

/* quod triangulum pingit: materia soluta */
nomen structura {
    constans Imago* textura;      /* NIHIL = alba */
               f32  color[IV];    /* factor * claritas; [III] alpha */
               f32  limen_alpha;  /* < 0 = opaca */
               b32  repetit_s;
               b32  repetit_t;
               b32  speculat_s;
               b32  speculat_t;
} Pigmentum;


/* ==================================================
 * Textura
 * ================================================== */

/* coordinata in [0, 1] secundum involutionem materiae */
interior f32
_involvere (
    f32 u,
    b32 repetit,
    b32 speculat)
{
    si (!repetit)
    {
        redde u < 0.0f ? 0.0f : (u > 1.0f ? 1.0f : u);
    }
    si (speculat)
    {
        u = u - 2.0f * (f32)floor((f64)(u * 0.5f));
        redde u > 1.0f ? 2.0f - u : u;
    }
    redde u - (f32)floor((f64)u);
}

/* pixelum texturae proximum; FALSUM si excisum */
interior b32
_colorem_sumere (
    constans Pigmentum* p,
                   f32  s,
                   f32  t,
                    i8* exitus)
{
    constans i8* pixelum;
            i32  x;
            i32  y;
            i32  k;
            f32  c[IV];
            f32  v;

    c[ZEPHYRUM]  = 255.0f;
    c[I]         = 255.0f;
    c[II]        = 255.0f;
    c[III]       = 255.0f;
    si (p->textura)
    {
        s = _involvere(s, p->repetit_s, p->speculat_s);
        t = _involvere(t, p->repetit_t, p->speculat_t);
        x = (i32)(s * (f32)p->textura->latitudo);
        y = (i32)(t * (f32)p->textura->altitudo);
        si (x >= p->textura->latitudo)
        {
            x = p->textura->latitudo - I;
        }
        si (y >= p->textura->altitudo)
        {
            y = p->textura->altitudo - I;
        }
        pixelum = p->textura->pixela
            + ((memoriae_index)y * p->textura->latitudo + x) * IV;
        per (k = ZEPHYRUM; k < IV; k++)
        {
            c[k] = (f32)pixelum[k];
        }
    }
    si (   p->limen_alpha >= 0.0f
        && c[III] * p->color[III] < p->limen_alpha * 255.0f)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        v          = c[k] * p->color[k] + 0.5f;
        exitus[k]  = (i8)(v > 255.0f ? 255.0f : (v < 0.0f ? 0.0f : v));
    }
    exitus[III] = 255;
    redde VERUM;
}


/* ==================================================
 * Triangulum in schirmo
 * ================================================== */

/* functio marginis a -> b ad punctum (px, py), puncta fixa */
interior s64
_margo (
    constans VertexSchirmi* a,
    constans VertexSchirmi* b,
                       s64  px,
                       s64  py)
{
    redde (s64)(b->x - a->x) * (py - (s64)a->y)
        - (s64)(b->y - a->y) * (px - (s64)a->x);
}

/* margo summus aut sinister (orientatio areae positivae): pixelum
 * exacte in eo sedens triangulo huic pertinet */
interior b32
_summus_sinister (
    constans VertexSchirmi* a,
    constans VertexSchirmi* b)
{
    redde (b->y == a->y && b->x > a->x) || b->y < a->y;
}

interior s32
_minimum_trium (
    s32 a,
    s32 b,
    s32 d)
{
    s32 m = a < b ? a : b;

    redde m < d ? m : d;
}

interior s32
_maximum_trium (
    s32 a,
    s32 b,
    s32 d)
{
    s32 m = a > b ? a : b;

    redde m > d ? m : d;
}

interior vacuum
_triangulum_schirmi (
                    Tabula* tab,
    constans VertexSchirmi* v0,
    constans VertexSchirmi* v1,
    constans VertexSchirmi* v2,
        constans Pigmentum* pigmentum)
{
    constans VertexSchirmi* permutatus;
                       s64  area;
                       s64  e0;
                       s64  e1;
                       s64  e2;
                       s64  r0;
                       s64  r1;
                       s64  r2;
                       s32  x_min;
                       s32  x_max;
                       s32  y_min;
                       s32  y_max;
                       s32  x;
                       s32  y;
                       b32  s0;
                       b32  s1;
                       b32  s2;
                       f32  inversa;
                       f32  b0;
                       f32  b1;
                       f32  b2;
                       f32  z;
                       f32  q;
                        i8  color[IV];
            memoriae_index  locus;

    area = _margo(v0, v1, (s64)v2->x, (s64)v2->y);
    si (area == ZEPHYRUM)
    {
        redde;
    }
    si (area < ZEPHYRUM)
    {
        permutatus  = v1;
        v1          = v2;
        v2          = permutatus;
        area        = -area;
    }
    x_min = _minimum_trium(v0->x, v1->x, v2->x) / PARTES_PIXELI - I;
    x_max = _maximum_trium(v0->x, v1->x, v2->x) / PARTES_PIXELI + I;
    y_min = _minimum_trium(v0->y, v1->y, v2->y) / PARTES_PIXELI - I;
    y_max = _maximum_trium(v0->y, v1->y, v2->y) / PARTES_PIXELI + I;
    si (x_min < ZEPHYRUM)
    {
        x_min = ZEPHYRUM;
    }
    si (y_min < ZEPHYRUM)
    {
        y_min = ZEPHYRUM;
    }
    si (x_max > (s32)tab->latitudo - I)
    {
        x_max = (s32)tab->latitudo - I;
    }
    si (y_max > (s32)tab->altitudo - I)
    {
        y_max = (s32)tab->altitudo - I;
    }
    si (x_min > x_max || y_min > y_max)
    {
        redde;
    }
    s0       = _summus_sinister(v1, v2);
    s1       = _summus_sinister(v2, v0);
    s2       = _summus_sinister(v0, v1);
    inversa  = 1.0f / (f32)area;
    /* functiones ad centrum pixeli primi; deinde gradibus */
    r0 = _margo(v1, v2, (s64)x_min * PARTES_PIXELI + VIII,
        (s64)y_min * PARTES_PIXELI + VIII);
    r1 = _margo(v2, v0, (s64)x_min * PARTES_PIXELI + VIII,
        (s64)y_min * PARTES_PIXELI + VIII);
    r2 = _margo(v0, v1, (s64)x_min * PARTES_PIXELI + VIII,
        (s64)y_min * PARTES_PIXELI + VIII);
    per (y = y_min; y <= y_max; y++)
    {
        e0 = r0;
        e1 = r1;
        e2 = r2;
        per (x = x_min; x <= x_max; x++)
        {
            si (   (e0 > ZEPHYRUM || (e0 == ZEPHYRUM && s0))
                && (e1 > ZEPHYRUM || (e1 == ZEPHYRUM && s1))
                && (e2 > ZEPHYRUM || (e2 == ZEPHYRUM && s2)))
            {
                b0  = (f32)e0 * inversa;
                b1  = (f32)e1 * inversa;
                b2  = (f32)e2 * inversa;
                z   = b0 * v0->z + b1 * v1->z + b2 * v2->z;
                locus  = (memoriae_index)y * tab->latitudo
                    + (memoriae_index)x;
                si (z < tab->profunditas[locus])
                {
                    q = b0 * v0->iw + b1 * v1->iw + b2 * v2->iw;
                    si (   q > 0.0f
                        && _colorem_sumere(pigmentum,
                               (b0 * v0->sw + b1 * v1->sw
                                   + b2 * v2->sw) / q,
                               (b0 * v0->tw + b1 * v1->tw
                                   + b2 * v2->tw) / q, color))
                    {
                        tab->profunditas[locus]  = z;
                        tab->tectum[locus]       = I;
                        memcpy(tab->rgba + locus * IV, color, IV);
                    }
                }
            }
            e0 -= (s64)(v2->y - v1->y) * PARTES_PIXELI;
            e1 -= (s64)(v0->y - v2->y) * PARTES_PIXELI;
            e2 -= (s64)(v1->y - v0->y) * PARTES_PIXELI;
        }
        r0 += (s64)(v2->x - v1->x) * PARTES_PIXELI;
        r1 += (s64)(v0->x - v2->x) * PARTES_PIXELI;
        r2 += (s64)(v1->x - v0->x) * PARTES_PIXELI;
    }
}

/* vertex oculi (ante planum propinquum) in schirmum */
interior VertexSchirmi
_proicere (
         constans Tabula* tab,
    constans VertexOculi* v)
{
    VertexSchirmi r;
              f32 w = -v->z;
              f32 x;
              f32 y;

    x = (tab->fx * v->x / w * 0.5f + 0.5f) * (f32)tab->latitudo;
    y = (0.5f - tab->fy * v->y / w * 0.5f) * (f32)tab->altitudo;
    si (x > SCHIRMI_LIMES)
    {
        x = SCHIRMI_LIMES;
    }
    si (x < -SCHIRMI_LIMES)
    {
        x = -SCHIRMI_LIMES;
    }
    si (y > SCHIRMI_LIMES)
    {
        y = SCHIRMI_LIMES;
    }
    si (y < -SCHIRMI_LIMES)
    {
        y = -SCHIRMI_LIMES;
    }
    r.x   = (s32)floor((f64)x * PARTES_PIXELI + 0.5);
    r.y   = (s32)floor((f64)y * PARTES_PIXELI + 0.5);
    r.z   = (tab->za * v->z + tab->zb) / w;
    r.iw  = 1.0f / w;
    r.sw  = v->s / w;
    r.tw  = v->t / w;
    redde r;
}

/* punctum in margine a -> b ubi z = -prope */
interior VertexOculi
_secare (
    constans VertexOculi* a,
    constans VertexOculi* b,
                     f32  prope)
{
    VertexOculi r;
            f32 t = (-prope - a->z) / (b->z - a->z);

    r.x = a->x + (b->x - a->x) * t;
    r.y = a->y + (b->y - a->y) * t;
    r.z = -prope;
    r.s = a->s + (b->s - a->s) * t;
    r.t = a->t + (b->t - a->t) * t;
    redde r;
}

/* triangulum in spatio oculi: ad planum propinquum sectum (0, III aut
 * IV vertices), proiectum, pictum */
interior vacuum
_triangulum_oculi (
                  Tabula* tab,
    constans VertexOculi* v,
      constans Pigmentum* pigmentum)
{
      VertexOculi secti[IV];
    VertexSchirmi s[IV];
              i32 n = ZEPHYRUM;
              i32 k;
              i32 proximus;
              b32 intra;
              b32 intra_proximus;

    per (k = ZEPHYRUM; k < III; k++)
    {
        proximus        = (k + I) % III;
        intra           = v[k].z <= -tab->prope;
        intra_proximus  = v[proximus].z <= -tab->prope;
        si (intra)
        {
            secti[n++] = v[k];
        }
        si (intra != intra_proximus)
        {
            secti[n++] = _secare(&v[k], &v[proximus], tab->prope);
        }
    }
    si (n < III)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        s[k] = _proicere(tab, &secti[k]);
    }
    _triangulum_schirmi(tab, &s[ZEPHYRUM], &s[I], &s[II], pigmentum);
    si (n == IV)
    {
        _triangulum_schirmi(tab, &s[ZEPHYRUM], &s[II], &s[III],
            pigmentum);
    }
}


/* ==================================================
 * Statua
 * ================================================== */

interior VertexOculi
_verticem_ferre (
       constans Statua* s,
    StereometriaMatrix  m,
                   i32  vertex)
{
    StereometriaVector p;
           VertexOculi v;

    p = stereometria_punctum_transformare(m,
        stereometria_vector(s->positiones[vertex * III],
            s->positiones[vertex * III + I],
            s->positiones[vertex * III + II]), NIHIL);
    v.x = p.x;
    v.y = p.y;
    v.z = p.z;
    v.s = s->coordinatae[vertex * II];
    v.t = s->coordinatae[vertex * II + I];
    redde v;
}

interior vacuum
_partem_pingere (
                Tabula* tab,
       constans Statua* s,
    constans StatuaPars* pars,
    StereometriaMatrix  m,
                   b32  speculum,
                   b32  illuminatum)
{
    constans StatuaMateria* materia = NIHIL;
        StereometriaVector  lux;
        StereometriaVector  normalis;
        StereometriaVector  a;
               VertexOculi  v[III];
                 Pigmentum  p;
                       f32  claritas;
                       f32  longitudo;
                       f32  cosinus;
                       b32  frons;
                       i32  k;
                       i32  j;

    memset(&p, ZEPHYRUM, magnitudo(p));
    p.limen_alpha = -1.0f;
    si (pars->materia >= ZEPHYRUM)
    {
        materia        = &s->materiae[pars->materia];
        p.limen_alpha  = materia->limen_alpha;
        p.repetit_s    = materia->repetit_s;
        p.repetit_t    = materia->repetit_t;
        p.speculat_s   = materia->speculat_s;
        p.speculat_t   = materia->speculat_t;
        si (materia->textura >= ZEPHYRUM)
        {
            p.textura = &s->texturae[materia->textura];
        }
    }
    lux = stereometria_normalizare(stereometria_vector(LUX_X, LUX_Y,
        LUX_Z));
    per (k = ZEPHYRUM; k + II < pars->indicum; k += III)
    {
        per (j = ZEPHYRUM; j < III; j++)
        {
            v[j] = _verticem_ferre(s, m,
                s->indices[pars->index_primus + k + j]);
        }
        a         = stereometria_vector(v[ZEPHYRUM].x, v[ZEPHYRUM].y,
            v[ZEPHYRUM].z);
        normalis  = stereometria_crux(
            stereometria_subtrahere(stereometria_vector(v[I].x, v[I].y,
                v[I].z), a),
            stereometria_subtrahere(stereometria_vector(v[II].x,
                v[II].y, v[II].z), a));
        si (speculum)
        {
            normalis = stereometria_scalare(normalis, -1.0f);
        }
        longitudo = stereometria_longitudo(normalis);
        si (!(longitudo * longitudo > AREA_MINIMA))
        {
            perge;
        }
        /* frons: normalis ad oculum (in origine) spectat */
        frons = stereometria_productum(normalis, a) < 0.0f;
        si (!frons)
        {
            si (!materia || !materia->bilateralis)
            {
                perge;
            }
            normalis = stereometria_scalare(normalis, -1.0f);
        }
        claritas = 1.0f;
        si (illuminatum && !(materia && materia->emissiva))
        {
            cosinus = stereometria_productum(normalis, lux) / longitudo;
            claritas = STATUA_LUX_FUNDI + (1.0f - STATUA_LUX_FUNDI)
                * (cosinus > 0.0f ? cosinus : 0.0f);
        }
        per (j = ZEPHYRUM; j < III; j++)
        {
            p.color[j] = (materia ? materia->color[j] : 1.0f)
                * claritas;
        }
        p.color[III] = materia ? materia->color[III] : 1.0f;
        _triangulum_oculi(tab, v, &p);
    }
}

/* determinans partis superioris III x III: negativus = speculum */
interior b32
_est_speculum (
    StereometriaMatrix m)
{
    StereometriaVector a = stereometria_vector(m.e[ZEPHYRUM], m.e[I],
        m.e[II]);
    StereometriaVector b = stereometria_vector(m.e[IV], m.e[V],
        m.e[VI]);
    StereometriaVector d = stereometria_vector(m.e[VIII], m.e[IX],
        m.e[X]);

    redde stereometria_productum(stereometria_crux(a, b), d) < 0.0f;
}

b32
statua_pingere (
                constans Statua* statua,
    constans StereometriaMatrix* matrices,
           constans StatuaVisus* visus,
                            i32  latitudo,
                            i32  altitudo,
                  StatuaPictura* exitus,
                        Piscina* piscina)
{
    constans StatuaRete* rete;
     StereometriaMatrix  oculus;
     StereometriaMatrix  m;
                 Tabula  tab;
         memoriae_index  pixela;
         memoriae_index  k;
                    f32  gyrus        = STATUA_GYRUS_ORDINARIUS;
                    f32  inclinatio   = STATUA_INCLINATIO_ORDINARIA;
                    b32  illuminatum  = VERUM;
                    f32  radius;
                    f32  dimidium;
                    f32  tangens;
                    f32  distantia;
                    f32  procul;
                    f32  ratio;
                    i32  n;
                    i32  j;

    si (   !statua || !exitus || !piscina
        || (statua->nodorum && !matrices))
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(StatuaPictura));
    exitus->latitudo = latitudo;
    exitus->altitudo = altitudo;
    si (latitudo == ZEPHYRUM || altitudo == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (visus)
    {
        gyrus        = visus->gyrus;
        inclinatio   = visus->inclinatio;
        illuminatum  = visus->illuminatum;
    }
    si (!(gyrus == gyrus) || (gyrus - gyrus) != 0.0f)
    {
        gyrus = 0.0f;
    }
    si (!(inclinatio <= STATUA_INCLINATIO_MAXIMA))
    {
        inclinatio = STATUA_INCLINATIO_MAXIMA;
    }
    si (inclinatio < -STATUA_INCLINATIO_MAXIMA)
    {
        inclinatio = -STATUA_INCLINATIO_MAXIMA;
    }
    pixela          = (memoriae_index)latitudo * altitudo;
    exitus->rgba    = (i8*)piscina_allocare(piscina, pixela * IV);
    exitus->tectum  = (i8*)piscina_allocare(piscina, pixela);
    tab.profunditas = (f32*)piscina_allocare(piscina,
        pixela * magnitudo(f32));
    memset(exitus->rgba, 0xFF, pixela * IV);
    memset(exitus->tectum, ZEPHYRUM, pixela);
    per (k = ZEPHYRUM; k < pixela; k++)
    {
        tab.profunditas[k] = PROFUNDITAS_VACUA;
    }
    tab.rgba      = exitus->rgba;
    tab.tectum    = exitus->tectum;
    tab.latitudo  = latitudo;
    tab.altitudo  = altitudo;
    /* oculus: campus per latus brevius; sphaera cum margine intra */
    radius    = statua->radius > 0.000001f ? statua->radius : 1.0f;
    dimidium  = STATUA_CAMPUS_GRADUS * 0.5f * STEREOMETRIA_PI / 180.0f;
    tangens   = (f32)tan((f64)dimidium);
    distantia  = radius / (f32)sin((f64)dimidium)
        * (1.0f + 2.0f * STATUA_MARGO);
    ratio      = (f32)latitudo / (f32)altitudo;
    si (ratio >= 1.0f)
    {
        tab.fy = 1.0f / tangens;
        tab.fx = tab.fy / ratio;
    }
    alioquin
    {
        tab.fx = 1.0f / tangens;
        tab.fy = tab.fx * ratio;
    }
    tab.prope = distantia - radius * 1.5f;
    si (tab.prope < distantia * 0.01f)
    {
        tab.prope = distantia * 0.01f;
    }
    procul = distantia + radius * 1.5f;
    tab.za = (procul + tab.prope) / (tab.prope - procul);
    tab.zb = 2.0f * procul * tab.prope / (tab.prope - procul);
    oculus = stereometria_multiplicare(
        stereometria_translatio(stereometria_vector(0.0f, 0.0f,
            -distantia)),
        stereometria_multiplicare(
            stereometria_rotatio_x(inclinatio * STEREOMETRIA_PI
                / 180.0f),
            stereometria_multiplicare(
                stereometria_rotatio_y(gyrus * STEREOMETRIA_PI
                    / 180.0f),
                stereometria_translatio(stereometria_scalare(
                    statua->centrum, -1.0f)))));
    per (n = ZEPHYRUM; n < statua->nodorum; n++)
    {
        si (statua->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        rete  = &statua->retia[statua->nodi[n].rete];
        m     = stereometria_multiplicare(oculus, matrices[n]);
        per (j = ZEPHYRUM; j < rete->partium; j++)
        {
            _partem_pingere(&tab, statua,
                &statua->partes[rete->pars_prima + j], m,
                _est_speculum(m), illuminatum);
        }
    }
    redde VERUM;
}
