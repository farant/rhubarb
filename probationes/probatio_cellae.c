/* probatio_cellae.c - probationes cellarum (project-specs/cellae-plan.md)
 *
 * T2 catalogus: clausura ordine D82/D84 contra clausuram fluitantem
 * eorum (in hac probatione rescriptam) elementum per elementum: ordo,
 * valores (decimales certi extensionis), profunditates, torsiones;
 * zeta_8 (XXIV) et zeta_12 truncatus ad MMMMXCVI (D82). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "extensio.h"
#include "quaternio.h"
#include "cellae.h"
#include "surdus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

interior Quaternio
_q (
       constans Anulus* anulus,
    constans character* textus,
               Piscina* piscina)
{
    Quaternio q;

    si (!quaternio_ex_chorda(anulus, chorda_ex_literis(textus, piscina),
        piscina, &q))
    {
        imprimere("FRACTA: quaternio malus: %s\n", textus);
        q.anulus = NIHIL;
        q.partes = NIHIL;
    }
    redde q;
}

/* generatores D82/D84: (g1, g1*, g2, g2*) */
interior vacuum
_generatores (
       constans Anulus* anulus,
    constans character* co,
    constans character* si_textus,
    constans character* si_negatum,
               Piscina* piscina,
             Quaternio* g)
{
    character textus[128];

    sprintf(textus, "[%s, %s, 0, 0]", co, si_textus);
    g[ZEPHYRUM] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, %s, 0, 0]", co, si_negatum);
    g[I] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, 0, 0, %s]", co, si_negatum);
    g[II] = _q(anulus, textus, piscina);
    sprintf(textus, "[%s, 0, 0, %s]", co, si_textus);
    g[III] = _q(anulus, textus, piscina);
}

hic_manens constans s32 TORSIONES[IV] = { I, -(s32)I, I, -(s32)I };


/* ==================================================
 * Clausura fluitans D82/D84 (rescripta)
 * ================================================== */

#define F_MAX 4096

nomen structura { duplex a, b, c, d; } FQ;

hic_manens FQ  f_cat[F_MAX];
hic_manens i32 f_prof[F_MAX];
hic_manens s32 f_torsio[F_MAX];
hic_manens i32 f_n;

interior s32
_f_invenire (
    constans FQ* q)
{
    i32 i;

    per (i = ZEPHYRUM; i < f_n; i++)
    {
        si (   fabs(f_cat[i].a - q->a) < 1e-10
            && fabs(f_cat[i].b - q->b)
            < 1e-10 && fabs(f_cat[i].c - q->c) < 1e-10
            && fabs(f_cat[i].d - q->d) < 1e-10)
        {
            redde (s32)i;
        }
        si (   fabs(f_cat[i].a + q->a) < 1e-10
            && fabs(f_cat[i].b + q->b)
            < 1e-10 && fabs(f_cat[i].c + q->c) < 1e-10
            && fabs(f_cat[i].d + q->d) < 1e-10)
        {
            redde (s32)i;
        }
    }
    redde -(s32)I;
}

interior vacuum
_f_clausura (
    duplex angulus,
       i32 limes)
{
        FQ g[IV];
    duplex co = cos(angulus);
    duplex sn = sin(angulus);
       i32 prior;
       i32 i;
       i32 k;
       i32 gradus;
       i32 r;

    g[ZEPHYRUM].a = co; g[ZEPHYRUM].b = sn; g[ZEPHYRUM].c = 0;
    g[ZEPHYRUM].d = 0;
    g[I] = g[ZEPHYRUM]; g[I].b = -sn;
    g[II].a = co; g[II].b = 0; g[II].c = 0; g[II].d = -sn;
    g[III] = g[II]; g[III].d = sn;
    f_cat[ZEPHYRUM].a = 1; f_cat[ZEPHYRUM].b = 0; f_cat[ZEPHYRUM].c = 0;
    f_cat[ZEPHYRUM].d = 0;
    f_prof[ZEPHYRUM] = ZEPHYRUM;
    f_torsio[ZEPHYRUM] = ZEPHYRUM;
    f_n = I;
    per (k = ZEPHYRUM; k < IV; k++)
    {
        si (_f_invenire(&g[k]) < 0 && f_n < limes)
        {
            f_cat[f_n]     = g[k];
            f_prof[f_n]    = ZEPHYRUM;
            f_torsio[f_n]  = TORSIONES[k];
            f_n++;
        }
    }
    gradus  = I;
    r       = I;
    fac
    {
        prior = f_n;
        per (i = ZEPHYRUM; i < prior; i++)
        {
            per (k = ZEPHYRUM; k < IV; k++)
            {
                constans FQ* p = &f_cat[i];
                constans FQ* h = &g[k];
                         FQ  pr;

                pr.a = p->a * h->a - p->b * h->b - p->c * h->c
                    - p->d * h->d;
                pr.b = p->a * h->b + p->b * h->a + p->c * h->d
                    - p->d * h->c;
                pr.c = p->a * h->c - p->b * h->d + p->c * h->a
                    + p->d * h->b;
                pr.d = p->a * h->d + p->b * h->c - p->c * h->b
                    + p->d * h->a;
                si (_f_invenire(&pr) < 0 && f_n < limes)
                {
                    f_cat[f_n]     = pr;
                    f_prof[f_n]    = r;
                    f_torsio[f_n]  = f_torsio[i] + TORSIONES[k];
                    f_n++;
                }
            }
        }
        si (f_n > prior)
        {
            gradus++;
        }
        r++;
    } dum (f_n > prior && gradus < XVI);
}

/* pars k exacta ad duplicem per decimales certos */
interior duplex
_ad_duplicem (
    Quaternio  q,
          i32  k,
      Piscina* piscina)
{
    constans Algebraicus* a = (constans Algebraicus*)quaternio_pars(q,
        k);
                  chorda textus = algebraicus_ad_ostendendum(*a, XVIII,
                      piscina);
            character alveus[64];

    memcpy(alveus, textus.datum, (size_t)textus.mensura);
    alveus[textus.mensura] = '\0';
    redde strtod(alveus, NIHIL);
}

/* exactus = fluitans D82/D84: ordo, valores, profunditates, torsiones */
interior b32
_concordat (
    constans Catalogus* c,
               Piscina* piscina)
{
    i32 i;

    si ((i32)catalogus_numerus(c) != f_n)
    {
        imprimere("  numerus %d contra %d\n", (int)catalogus_numerus(c),
            (int)f_n);
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < f_n; i++)
    {
        Quaternio q          = catalogus_elementum(c, i);
        PiscinaNotatio nota  = piscina_notare(piscina);
        b32 bene = fabs(_ad_duplicem(q, ZEPHYRUM, piscina) - f_cat[i].a)
            < 1e-12 && fabs(_ad_duplicem(q, I, piscina) - f_cat[i].b)
            < 1e-12 && fabs(_ad_duplicem(q, II, piscina) - f_cat[i].c)
            < 1e-12 && fabs(_ad_duplicem(q, III, piscina) - f_cat[i].d)
            < 1e-12 && catalogus_profunditas(c, i) == f_prof[i]
            && catalogus_torsio(c, i) == f_torsio[i];

        piscina_reficere(piscina, nota);
        si (!bene)
        {
            imprimere("  discrepantia in elemento %d\n", (int)i);
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * PROBARE
 * ================================================== */

interior vacuum
_probare_zeta_octo (
    CredoContextus* c)
{
            Piscina* piscina  = c->piscina;
           Extensio* k2       = extensio_quadratica(II, piscina);
    constans Anulus* q2       = extensio_anulus(k2);
          Quaternio  g[IV];
          Catalogus* cat              = NIHIL;
                i32  distributio[IV]  = { 0, 0, 0, 0 };
                i32  nulla            = ZEPHYRUM;
                i32  i;

    imprimere("\n--- Clausura zeta_8 (D84) ---\n");
    _generatores(q2, "a/2", "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, (i32)F_MAX, XVI,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), XXIV);
    CREDO_AEQUALIS_I32 (catalogus_gradus(cat), IV);
    CREDO_FALSUM (catalogus_truncatus(cat));
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
        distributio[catalogus_profunditas(cat, i)]++;
        nulla += catalogus_nullum(cat, i) ? I : ZEPHYRUM;
    }
    CREDO_VERUM (distributio[ZEPHYRUM] == V && distributio[I] == X
        && distributio[II] == VIII && distributio[III] == I);
    CREDO_AEQUALIS_I32 (nulla, IX);
    _f_clausura(M_PI / 4.0, (i32)F_MAX);
    CREDO_VERUM (_concordat(cat, piscina));
}

interior vacuum
_probare_zeta_duodecim (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                   Extensio* k3 = extensio_quadratica(III,
                       piscina);
            constans Anulus* q3 = extensio_anulus(k3);
                  Quaternio  g[IV];
                  Catalogus* cat = NIHIL;
                        i32  distributio[XVI];
    hic_manens constans i32  D82[IX] = { 5, 12, 34, 76, 148, 289, 576,
        1152, 1804 };
    i32 i;
    b32 bene = VERUM;

    imprimere("\n--- Clausura zeta_12 truncata (D82) ---\n");
    _generatores(q3, "a/2", "1/2", "-1/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, (i32)F_MAX, XVI,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), (i32)F_MAX);
    CREDO_VERUM (catalogus_truncatus(cat));
    per (i = ZEPHYRUM; i < XVI; i++) distributio[i] = ZEPHYRUM;
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
        distributio[catalogus_profunditas(cat, i)]++;
    }
    per (i = ZEPHYRUM; i < IX; i++)
    {
        bene = bene && distributio[i] == D82[i];
    }
    CREDO_VERUM (bene);
    _f_clausura(M_PI / 6.0, (i32)F_MAX);
    CREDO_VERUM (_concordat(cat, piscina));
}

interior vacuum
_probare_limites (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                   Extensio* k2 = extensio_quadratica(II,
                       piscina);
            constans Anulus* q2 = extensio_anulus(k2);
                  Quaternio  g[IV];
                  Catalogus* cat           = NIHIL;
                  Catalogus* pars          = NIHIL;
    hic_manens constans i32  INDICES[III]  = { 7, 0, 3 };
    hic_manens constans i32  MALI[II]      = { 2, 2 };

    imprimere("\n--- Limes, gradus, pars, refutationes ---\n");
    _generatores(q2, "a/2", "a/2", "-a/2", piscina, g);
    /* limes X in medio gradus I: truncatus */
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, X, ZEPHYRUM,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), X);
    CREDO_VERUM (catalogus_truncatus(cat));
    /* gradus maximi II: gradus 0 et I (V + X), non truncatus */
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, II,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), XV);
    CREDO_FALSUM (catalogus_truncatus(cat));
    /* pars: ordo indicum servatur */
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM,
        ZEPHYRUM, piscina, &cat));
    CREDO_NECESSE (catalogus_pars(cat, INDICES, III, piscina, &pars));
    CREDO_AEQUALIS_I32 (catalogus_numerus(pars), III);
    CREDO_VERUM (quaternio_aequalis(catalogus_elementum(pars, ZEPHYRUM),
        catalogus_elementum(cat, VII)));
    CREDO_VERUM (catalogus_profunditas(pars, II)
        == catalogus_profunditas(cat, III));
    CREDO_FALSUM (catalogus_pars(cat, MALI, II, piscina, &pars));
    CREDO_FALSUM (catalogus_pars(cat, INDICES, ZEPHYRUM, piscina,
        &pars));
    /* extra: anulus NIHIL, profunditas -1 */
    CREDO_VERUM (catalogus_elementum(cat, C).anulus == NIHIL);
    CREDO_AEQUALIS_S32 ((s32)catalogus_profunditas(cat, C), -(s32)I);
    /* refutationes: numerus 0; anuli mixti; anulus sine ordine */
    CREDO_FALSUM (catalogus_clausura(g, TORSIONES, ZEPHYRUM, ZEPHYRUM,
        ZEPHYRUM, piscina, &cat));
    {
         Extensio* k3 = extensio_quadratica(III, piscina);
        Quaternio  mixti[II];

        mixti[ZEPHYRUM] = g[ZEPHYRUM];
        mixti[I] = _q(extensio_anulus(k3), "[a/2, 1/2, 0, 0]", piscina);
        CREDO_FALSUM (catalogus_clausura(mixti, TORSIONES, II, ZEPHYRUM,
            ZEPHYRUM, piscina, &cat));
        mixti[ZEPHYRUM] = _q(&ANULUS_POLYNOMIORUM, "[0, t, 0, 0]",
            piscina);
        CREDO_FALSUM (catalogus_clausura(mixti, TORSIONES, I, ZEPHYRUM,
            ZEPHYRUM, piscina, &cat));
    }
}


/* ==================================================
 * T3: oraculum integrum Z[sqrt2] (sine cellis, sine radicibus)
 * ================================================== */

nomen structura { s64 p; s64 q; } Binomium;     /* p + q sqrt(o_d) */

/* campus et scala oraculi: zeta_8 (II, II), zeta_12 (III, XXXII:
 * denominatores catalogi MMMMXCVI <= XXXII, mensuratum a recensore) */
hic_manens s64 o_d      = II;
hic_manens s64 o_scala  = II;
hic_manens b32 o_vitium = FALSUM;    /* exundatio aut scala falsa */

interior Binomium
binomium (
    s64 p,
    s64 q)
{ Binomium r; r.p = p; r.q = q; redde r;
}

interior Binomium
binomium_adde (
    Binomium x,
    Binomium y)
{ redde binomium(x.p + y.p, x.q + y.q);
}

interior Binomium
binomium_subtrahe (
    Binomium x,
    Binomium y)
{ redde binomium(x.p - y.p, x.q - y.q);
}

interior Binomium
binomium_multiplica (
    Binomium x,
    Binomium y)
{
    /* custodia: nullum productum > 2^60 (duplices, generose) */
    duplex limes = 1152921504606846976.0;

    si (   fabs((duplex)x.p) * fabs((duplex)y.p) > limes
        || fabs((duplex)x.q) * fabs((duplex)y.q) * (duplex)o_d > limes
        || fabs((duplex)x.p) * fabs((duplex)y.q) > limes
        || fabs((duplex)x.q) * fabs((duplex)y.p) > limes)
    {
        o_vitium = VERUM;
    }
    redde binomium(x.p * y.p + o_d * x.q * y.q, x.p * y.q + x.q * y.p);
}

interior b32
binomium_nullum (
    Binomium x)
{ redde x.p == 0 && x.q == 0;
}

/* productum plenum 64 x 64 -> (alta, infima), per dimidia XXXII bitorum */
interior vacuum
_productum_latum (
    i64  x,
    i64  y,
    i64* alta,
    i64* infima)
{
    i64 larva   = 0xFFFFFFFFUL;
    i64 p00     = (x & larva) * (y & larva);
    i64 p01     = (x & larva) * (y >> 32);
    i64 p10     = (x >> 32) * (y & larva);
    i64 p11     = (x >> 32) * (y >> 32);
    i64 medium  = (p00 >> 32) + (p01 & larva) + (p10 & larva);

    *infima  = (medium << 32) | (p00 & larva);
    *alta    = p11 + (p01 >> 32) + (p10 >> 32) + (medium >> 32);
}

/* signum(a^2 - d b^2) exacte (a, b < 2^62, d parvum) */
interior s32
_quadrata_comparare (
    i64 a,
    i64 b)
{
    i64 a_alta;
    i64 a_infima;
    i64 b_alta;
    i64 b_infima;
    i64 d_alta;
    i64 d_infima;

    _productum_latum(a, a, &a_alta, &a_infima);
    _productum_latum(b, b, &b_alta, &b_infima);
    _productum_latum(b_infima, (i64)o_d, &d_alta, &d_infima);
    d_alta += b_alta * (i64)o_d;
    si (a_alta != d_alta) redde a_alta > d_alta ? I : -(s32)I;
    si (a_infima != d_infima) redde a_infima > d_infima ? I : -(s32)I;
    redde ZEPHYRUM;
}

interior s32
binomium_signum (
    Binomium x)
{
    i64 modulus_p = (i64)(x.p < 0 ? -x.p : x.p);
    i64 modulus_q = (i64)(x.q < 0 ? -x.q : x.q);

    si (x.p >= 0 && x.q >= 0) redde (x.p != 0 || x.q != 0) ? I : 0;
    si (x.p <= 0 && x.q <= 0) redde -(s32)I;
    /* signa mixta: p + q sqrt d != 0 (d non quadratum); |p| vs |q| sqrt d */
    si (x.p > 0) redde _quadrata_comparare(modulus_p, modulus_q)
                     > 0 ? I : -(s32)I;
    redde _quadrata_comparare(modulus_p, modulus_q) < 0 ? I : -(s32)I;
}

interior duplex
binomium_valor (
    Binomium x)
{ redde (duplex)x.p + (duplex)x.q * sqrt((duplex)o_d);
}

/* pars exacta x o_scala ut integrum Z[sqrt d] (zeta_8: (p + q sqrt2)/2;
 * zeta_12: denominatores <= XXXII); scala falsa -> o_vitium */
interior Binomium
_binomium_ex (
    Quaternio  q,
          i32  k,
      Piscina* piscina)
{
    constans Algebraicus* a = (constans Algebraicus*)quaternio_pars(q,
        k);
    Fractio f0 = fractio_multiplica(algebraicus_coefficiens(*a,
        ZEPHYRUM, piscina), fractio_ex_s64(o_scala), piscina);
    Fractio f1 = fractio_multiplica(algebraicus_coefficiens(*a, I,
        piscina), fractio_ex_s64(o_scala), piscina);
    s64 p   = ZEPHYRUM;
    s64 r   = ZEPHYRUM;
    s64 d0  = ZEPHYRUM;
    s64 d1  = ZEPHYRUM;

    si (   !magnus_ad_s64(fractio_numerator(f0), &p)
        || !magnus_ad_s64(fractio_numerator(f1), &r)
        || !magnus_ad_s64(fractio_denominator(f0), &d0)
        || !magnus_ad_s64(fractio_denominator(f1), &d1)
        || d0 != I || d1 != I)
    {
        o_vitium = VERUM;
    }
    redde binomium(p, r);
}

#define O_MAX 512
hic_manens Binomium  o_axis[O_MAX][III];
hic_manens Binomium  o_n2[O_MAX];
hic_manens i32 o_nd;

interior vacuum
_o_axes (
    constans Catalogus* cat,
               Piscina* piscina)
{
    i32 i;
    i32 j;

    o_nd = ZEPHYRUM;
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
              Quaternio q = catalogus_elementum(cat, i);
               Binomium v[III];
                    b32 inventus = FALSUM;

        v[0] = _binomium_ex(q, I, piscina);
        v[1] = _binomium_ex(q, II, piscina);
        v[2] = _binomium_ex(q, III, piscina);
        si (   binomium_nullum(v[0]) && binomium_nullum(v[1])
            && binomium_nullum(v[2])) perge;
        per (j = ZEPHYRUM; j < o_nd && !inventus; j++)
        {
            inventus =
                binomium_nullum(binomium_subtrahe(binomium_multiplica(v[1],
                o_axis[j][2]),
                binomium_multiplica(v[2], o_axis[j][1])))
                    && binomium_nullum(binomium_subtrahe(binomium_multiplica(v[2],
                o_axis[j][0]), binomium_multiplica(v[0], o_axis[j][2])))
                    && binomium_nullum(binomium_subtrahe(
                binomium_multiplica(v[0], o_axis[j][1]),
                binomium_multiplica(v[1], o_axis[j][0])));
        }
        si (!inventus && o_nd == O_MAX)
        {
            o_vitium = VERUM;
            redde;
        }
        si (!inventus)
        {
            o_axis[o_nd][0] = v[0]; o_axis[o_nd][1] =
                                        v[1]; o_axis[o_nd][2] =
                                                                v[2];
            o_n2[o_nd] =
                binomium_adde(binomium_adde(binomium_multiplica(v[0],
                v[0]),
                binomium_multiplica(v[1],
                v[1])), binomium_multiplica(v[2], v[2]));
            o_nd++;
        }
    }
}

/* tan^2(m pi/k) in Q(sqrt d)? (x + y sqrt d)/z */
interior b32
_o_terminus_exactus (
    i32  m,
    i32  k,
    s64* x,
    s64* y,
    s64* z)
{
    /* m/k reducta: rationales {1/6, 1/4, 1/3} omni campo; Q(sqrt2):
     * {1/8, 3/8} = 3 -+ 2 sqrt2; Q(sqrt3): {1/12, 5/12} = 7 -+ 4 sqrt3 */
    i32 a = m;
    i32 b = k;
    i32 g;

    per (g = a; g > I; g--)
    {
        si (a % g == 0 && b % g == 0)
        { a /= g; b /= g; g = a + I;
        }
    }
    *y = 0;
    *z = 1;
    si (a == I && b == VI)
    { *x = 1; *z = 3; redde VERUM;
    }
    si (a == I && b == IV)
    { *x = 1; redde VERUM;
    }
    si (a == I && b == III)
    { *x = 3; redde VERUM;
    }
    si (o_d == II && (a == I || a == III) && b == VIII)
    { *x = 3; *y = a == I ? -2 : 2; redde VERUM;
    }
    si (o_d == III && (a == I || a == V) && b == XII)
    { *x = 7; *y = a == I ? -4 : 4; redde VERUM;
    }
    redde FALSUM;
}

/* cella oraculi: regula et ligatae (ordinatae), FALSUM si margo
 * fluitans < 1e-9 (nunquam exspectatur) */
interior b32
_o_cella (
     Binomium* s,
          i32  k,
          i32* regula,
          i32* ligatae,
          i32* n)
{
    i32 nv = o_nd + I;
    b32 a0 = binomium_nullum(s[0]);
    b32 v0 = binomium_nullum(s[1]) && binomium_nullum(s[2])
        && binomium_nullum(s[3]);
    i32 sectiones[II];
    i32 n_sec = I;
    i32 axes[O_MAX];
    i32 numerus_axium_o = ZEPHYRUM;
    i32 j;
    i32 m;

    *n = ZEPHYRUM;
    si (v0)
    {
        *regula          = a0 ? (k - I) * nv + o_nd : o_nd;
        ligatae[(*n)++]  = *regula;
        redde VERUM;
    }
    {
        Binomium bn = binomium(-1, 0);
        Binomium bd = binomium(1, 0);

        per (j = ZEPHYRUM; j < o_nd; j++)
        {
            Binomium scalare =
                binomium_adde(binomium_adde(binomium_multiplica(s[1],
                o_axis[j][0]),
                binomium_multiplica(s[2],
                o_axis[j][1])), binomium_multiplica(s[3],
                o_axis[j][2]));
             Binomium num = binomium_multiplica(scalare, scalare);
                  s32 c = numerus_axium_o
                      == 0 ? I : binomium_signum(binomium_subtrahe(binomium_multiplica(num,
                      bd),
                      binomium_multiplica(bn, o_n2[j])));

            si (c > 0)
            { bn = num; bd = o_n2[j]; numerus_axium_o = 0;
            }
            si (c >= 0) axes[numerus_axium_o++] = j;
        }
    }
    si (a0)
    {
        sectiones[0] = k / II;
        si (k % II == 0)
        { sectiones[1] = k / II - I; n_sec = II;
        }
    } alioquin
    {
        Binomium P =
            binomium_adde(binomium_adde(binomium_multiplica(s[1], s[1]),
            binomium_multiplica(s[2], s[2])),
            binomium_multiplica(s[3], s[3]));
         Binomium Q    = binomium_multiplica(s[0], s[0]);
              b32 par  = FALSUM;

        sectiones[0] = 0;
        per (m = I; 2 * m < k; m++)
        {
            s64 x;
            s64 y;
            s64 z;
            s32 c;

            si (_o_terminus_exactus(m, k, &x, &y, &z))
            {
                c =
                    binomium_signum(binomium_subtrahe(binomium_multiplica(binomium(z,
                    0), P), binomium_multiplica(binomium(x,
                    y),
                    Q)));
            } alioquin
            {
                duplex T = binomium_valor(P) / binomium_valor(Q);
                duplex t = tan((duplex)m * M_PI / (duplex)k);

                t = t * t;
                si (fabs(T - t) < 1e-9 * t) redde FALSUM;
                c = T > t ? I : -(s32)I;
            }
            si (c < 0) frange;
            sectiones[0]  = m;
            par           = c == 0;
        }
        si (par)
        { sectiones[1] = sectiones[0] - I; n_sec = II;
        }
    }
    *regula = sectiones[0] * nv + axes[0];
    per (m = 0; m < n_sec; m++)
    {
        per (j = 0; j < numerus_axium_o; j++) ligatae[(*n)++] =
                                                  sectiones[m] * nv
                                                  + axes[j];
    }
    redde VERUM;
}

interior vacuum
_ordina (
    i32* x,
    i32  n)
{
    i32 i;
    s32 j;

    per (i = I; i < n; i++)
    {
        i32 v = x[i];

        j = (s32)i - I;
        dum (j >= 0 && x[j] > v)
        { x[j + 1] = x[j]; j--;
        }
        x[j + 1] = v;
    }
}

interior vacuum
_probare_axes (
    CredoContextus* c)
{
      Piscina* piscina = c->piscina;
    Quaternio  g[IV];
    Catalogus* cat = NIHIL;
    Activatio* act = NIHIL;

    imprimere("\n--- Tabula axium ---\n");
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2",
        "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina, &act));
    CREDO_AEQUALIS_I32 (activatio_axes(act), XIII);
    CREDO_FALSUM (activatio_truncata(act));
    CREDO_NECESSE (activatio_ex_catalogo(cat, V, piscina, &act));
    CREDO_AEQUALIS_I32 (activatio_axes(act), V);
    CREDO_VERUM (activatio_truncata(act));
    /* zeta_12 (D117): MMXLIII axes, D82 tabula D (truncata) */
    _generatores(extensio_anulus(extensio_quadratica(III, piscina)),
        "a/2", "1/2", "-1/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, (i32)F_MAX, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina, &act));
    CREDO_AEQUALIS_I32 (activatio_axes(act), (i32)MMXLIII);
    CREDO_NECESSE (activatio_ex_catalogo(cat, D, piscina, &act));
    CREDO_AEQUALIS_I32 (activatio_axes(act), D);
    CREDO_VERUM (activatio_truncata(act));
}

interior vacuum
_probare_cellas (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
    hic_manens constans i32  VALORES_K[X] = { 1, 2, 4, 6, 8, 10, 12, 16,
        20,
        24 };
    Quaternio  g[IV];
    Catalogus* cat = NIHIL;
    Activatio* act = NIHIL;
          i32  n;
          i32  i;
          i32  j;
          i32  l;
          i32  index_k;
          i32  v;
          i64  comparata  = 0;
          i64  discordes  = 0;
          i64  ligata     = 0;
          i64  nullae     = 0;
          i64  refutata   = 0;

    imprimere("\n--- Cellae contra oraculum Z[sqrt2] ---\n");
    o_d       = II;
    o_scala   = II;
    o_vitium  = FALSUM;
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2",
        "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina, &act));
    _o_axes(cat, piscina);
    CREDO_AEQUALIS_I32 (o_nd, XIII);
    n = catalogus_numerus(cat);
    /* summae c_i q_i + c_j q_j + c_l q_l, c in {-1, 0, 1}: singula,
     * paria omnia, triplices inter XII primos */
    per (i = ZEPHYRUM; i < n; i++) per (j = i; j < n; j++) per (l = j; l
                                                               < (j
                                                               < XII ? XII : j
                                                                   + I); l++)
                                                           {
        per (v = ZEPHYRUM; v < XXVII; v++)
        {
                             s32 cf[III];
                             i32 idx[III];
                       Quaternio s;
                        Binomium so[IV];
                             i32 t;
                             i32 r;
                  PiscinaNotatio nota = piscina_notare(piscina);

            cf[0] = (s32)(v % 3) - 1; cf[1] = (s32)((v / 3) % 3) - 1;
            cf[2]   = (s32)(v / 9) - 1;
            idx[0]  = i; idx[1] = j; idx[2] = l;
            si ((j == i && cf[1] != 0) || (l == j && cf[2] != 0)) perge;
            (vacuum)quaternio_nullum(catalogus_elementum(cat, 0).anulus,
                piscina, &s);
            per (r = 0; r < IV; r++) so[r] = binomium(0, 0);
            per (t = 0; t < III; t++)
            {
                Quaternio q = catalogus_elementum(cat, idx[t]);

                si (cf[t] == 0) perge;
                si (cf[t] > 0) (vacuum)quaternio_adde(s, q, piscina,
                                   &s);
                alioquin (vacuum)quaternio_subtrahe(s, q, piscina, &s);
                per (r = 0; r < IV; r++)
                {
                    Binomium e = _binomium_ex(q, r, piscina);

                    so[r] = cf[t] > 0 ? binomium_adde(so[r],
                        e) : binomium_subtrahe(so[r], e);
                }
            }
            per (index_k = ZEPHYRUM; index_k < X; index_k++)
            {
                Cella cella;
                  i32 o_regula = 0;
                  i32 o_ligatae[2 * O_MAX];
                  i32 on = 0;
                  i32 elig[2 * O_MAX];
                  b32 bene;

                si (   !activatio_cella(act, s, VALORES_K[index_k],
                    &cella)
                    || !_o_cella(so, VALORES_K[index_k], &o_regula,
                    o_ligatae, &on))
                {
                    refutata++;
                    perge;
                }
                bene = cella.regula == o_regula && cella.numerus == on;
                si (bene)
                {
                    per (t = 0; t < on; t++) elig[t] = cella.ligatae[t];
                    _ordina(elig, on);
                    _ordina(o_ligatae, on);
                    per (t = 0; t < on; t++) bene = bene
                                                 && elig[t]
                                                     == o_ligatae[t];
                }
                comparata++;
                si (!bene) discordes++;
                si (on > 1) ligata++;
                si (   binomium_nullum(so[0])
                    && !(binomium_nullum(so[1])
                    && binomium_nullum(so[2])
                    && binomium_nullum(so[3]))) nullae++;
            }
            (vacuum)nota;
        }
                                                           }
    imprimere("    comparata %ld, discordes %ld, ligata %ld (nullae %ld), "
        "refutata %ld\n", (long)comparata, (long)discordes,
        (long)ligata,
        (long)nullae, (long)refutata);
    /* summae: singula, paria, triplices inter XII primos, X valores k */
    CREDO_VERUM (comparata == 90900 && discordes == 0 && refutata == 0);
    CREDO_VERUM (ligata == 28850 && nullae == 13620);
    CREDO_FALSUM (o_vitium);
}

interior vacuum
_probare_cellas_manu (
    CredoContextus* c)
{
      Piscina* piscina = c->piscina;
    Quaternio  g[IV];
    Catalogus* cat = NIHIL;
    Activatio* act = NIHIL;
        Cella  cella;
    Quaternio  s;
          s32  nulla = -1;
          i32  i;

    imprimere("\n--- Cellae manu, refutationes ---\n");
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2",
        "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina, &act));
    /* nulla summa: q - q -> (k - 1)(XIV) + XIII */
    CREDO_NECESSE (quaternio_subtrahe(catalogus_elementum(cat, I),
        catalogus_elementum(cat, I), piscina, &s));
    CREDO_VERUM (activatio_cella(act, s, VI, &cella) && cella.regula
        == V * XIV + XIII && cella.numerus == I);
    /* realis: 1 + 1 -> XIII */
    CREDO_NECESSE (quaternio_adde(catalogus_elementum(cat, 0),
        catalogus_elementum(cat, 0), piscina, &s));
    CREDO_VERUM (activatio_cella(act, s, VI, &cella) && cella.regula
        == XIII && cella.numerus == I);
    /* elementum nullum (Re 0): k = 6 sector III cum paritate II; k = 5
     * sector II sine paritate */
    per (i = ZEPHYRUM; i < catalogus_numerus(cat) && nulla < 0; i++)
    {
        si (catalogus_nullum(cat, i)) nulla = (s32)i;
    }
    CREDO_NECESSE (nulla >= 0);
    CREDO_VERUM (activatio_cella(act, catalogus_elementum(cat,
        (i32)nulla), VI,
        &cella) && cella.regula / XIV == III && cella.numerus >= II);
    CREDO_VERUM (activatio_cella(act, catalogus_elementum(cat,
        (i32)nulla), V,
        &cella) && cella.regula / XIV == II);
    /* k = 0 et anulus alienus refutantur */
    CREDO_FALSUM (activatio_cella(act, s, ZEPHYRUM, &cella));
    CREDO_FALSUM (activatio_cella(act, _q(&ANULUS_RATIONALIUM,
        "[0, 1, 0, 0]", piscina), VI, &cella));
    /* catalogus super Q (unitates Hurwitz): non quadraticus */
    {
        Quaternio h[II];
              s32 torsio_duo[II];

        torsio_duo[0] = I; torsio_duo[1] = I;
        h[0] = _q(&ANULUS_RATIONALIUM, "[0, 1, 0, 0]", piscina);
        h[1] = _q(&ANULUS_RATIONALIUM, "[1/2, 1/2, 1/2, 1/2]", piscina);
        CREDO_NECESSE (catalogus_clausura(h, torsio_duo, II, ZEPHYRUM,
            ZEPHYRUM,
            piscina, &cat));
        CREDO_AEQUALIS_I32 (catalogus_numerus(cat), XII);
        CREDO_FALSUM (activatio_ex_catalogo(cat, ZEPHYRUM, piscina,
            &act));
    }
}


/* ==================================================
 * T4: separabilitas contra D124 (machina exacta independens)
 * ================================================== */

/* triplices separabiles subcatalogi ad k, modo */
interior i32
_triplices (
    Activatio* act,
          i32  n,
          i32  k,
          i32  modus)
{
    i32 a;
    i32 b;
    i32 c;
    i32 numerus = ZEPHYRUM;

    per (a = ZEPHYRUM; a < n; a++) per (b = a + I; b < n; b++) per (c =
                                                                   b
                                                                       + I; c
                                                                       < n; c++)
                                                               {
        i32 w[III];
        b32 verdictum = FALSUM;

        w[0] = a; w[1] = b; w[2] = c;
        si (!activatio_separat(act, w, III, k, modus, &verdictum))
        {
            redde (i32)999999;
        }
        si (verdictum) numerus++;
                                                               }
    redde numerus;
}

interior vacuum
_probare_separationem (
    CredoContextus* c)
{
                    Piscina* piscina       = c->piscina;
    hic_manens constans i32  VALORES_K[V]  = { 1, 2, 4, 6, 8 };
    /* D124 Pars F (oraculum D124 concordat): plenum, non nulla, nulla;
     * regula, robusta, possibilis */
    hic_manens constans i32 D124[III][III][V] = {
        { { 36, 168, 180, 324, 484 }, { 36, 36, 36, 68, 176 },
          { 60, 400, 540, 975, 903 } },
        { { 0, 12, 14, 70, 46 }, { 0, 0, 0, 12, 44 },
          { 0, 12, 28, 112, 76 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 80, 80, 80,
            80 } } };
    Quaternio  g[IV];
    Catalogus* cat = NIHIL;
    Catalogus* partes[III];
          i32  indices_nn[XXIV];
          i32  indices_n[XXIV];
          i32  nn = ZEPHYRUM;
          i32  nu = ZEPHYRUM;
          i32  i;
          i32  si_idx;
          i32  index_k;
          i32  modus;
          b32  bene = VERUM;

    imprimere("\n--- Separabilitas contra D124 ---\n");
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2",
        "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
        si (catalogus_nullum(cat, i)) indices_n[nu++] = i;
        alioquin indices_nn[nn++] = i;
    }
    partes[0] = cat;
    CREDO_NECESSE (catalogus_pars(cat, indices_nn, nn, piscina,
        &partes[1]));
    CREDO_NECESSE (catalogus_pars(cat, indices_n, nu, piscina,
        &partes[2]));
    per (si_idx = ZEPHYRUM; si_idx < III; si_idx++)
    {
        Activatio* act  = NIHIL;
              i32  n    = catalogus_numerus(partes[si_idx]);

        CREDO_NECESSE (activatio_ex_catalogo(partes[si_idx], ZEPHYRUM,
            piscina, &act));
        per (modus = ZEPHYRUM; modus < III; modus++)
        {
            imprimere("    pars %d modus %d:", (int)si_idx, (int)modus);
            per (index_k = ZEPHYRUM; index_k < V; index_k++)
            {
                i32 numerus = _triplices(act, n, VALORES_K[index_k],
                    modus);

                imprimere(" %d", (int)numerus);
                bene = bene && numerus == D124[si_idx][modus][index_k];
            }
            imprimere("\n");
        }
    }
    CREDO_VERUM (bene);
}

interior vacuum
_probare_regulas_fixas (
    CredoContextus* c)
{
                    Piscina* piscina        = c->piscina;
    hic_manens constans i32  VALORES_K[VI]  = { 1, 4, 6, 8, 12, 24 };
                  Quaternio  g[IV];
                  Catalogus* cat = NIHIL;
                  Activatio* act = NIHIL;
                        i32  a;
                        i32  b;
                        i32  cc;
                        i32  index_k;
                        i32  fr;
                        i64  regula_aequat      = 0;
                        i64  implicatio_fracta  = 0;
                        i64  probata            = 0;

    imprimere("\n--- Regulae fixae: FIXA(1,0) = regula; robusta => fixa => "
        "possibilis ---\n");
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2",
        "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina, &act));
    per (a = ZEPHYRUM; a < XXIV; a++) per (b = a + I; b
                                          < XXIV; b++) per (cc = b
                                                           + I; cc
                                                           < XXIV; cc++) per (index_k =
                                                                             ZEPHYRUM; index_k
                                                                                 < VI; index_k++)
                                                                         {
        i32 w[III];
        b32 v[VII];
        i32 m;

        w[0] = a; w[1] = b; w[2] = cc;
        per (m = ZEPHYRUM; m < VII; m++)
        {
            v[m] = FALSUM;
            (vacuum)activatio_separat(act, w, III, VALORES_K[index_k],
                (i32)m,
                &v[m]);
        }
        probata++;
        si (v[CELLAE_FIXA(1, 0)] == v[CELLAE_REGULA]) regula_aequat++;
        per (fr = ZEPHYRUM; fr < IV; fr++)
        {
            b32 fixa = v[III + fr];

            si (v[CELLAE_ROBUSTA] && !fixa) implicatio_fracta++;
            si (fixa && !v[CELLAE_POSSIBILIS]) implicatio_fracta++;
        }
                                                                         }
    imprimere("    probata %ld, FIXA(1,0) = regula %ld, implicationes "
        "fractae %ld\n", (long)probata, (long)regula_aequat,
        (long)implicatio_fracta);
    CREDO_VERUM (probata == 12144 && regula_aequat == probata
        && implicatio_fracta == 0);
}

interior vacuum
_probare_collisionem (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                  Quaternio  g[IV];
                  Catalogus* cat = NIHIL;
                  Activatio* act = NIHIL;
                  Quaternio  summa;
    hic_manens constans i32  W[III] = { 15, 17, 22 };
                        i32  modus;
                        b32  nulla      = VERUM;
                        b32  verdictum  = VERUM;

    imprimere("\n--- Collisio: q15 + q22 = q17 (paritates diversae) ---\n");
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2",
        "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina, &act));
    CREDO_NECESSE (quaternio_adde(catalogus_elementum(cat, 15),
        catalogus_elementum(cat, 22), piscina, &summa));
    CREDO_VERUM (quaternio_aequalis(summa, catalogus_elementum(cat,
        17)));
    CREDO_VERUM (catalogus_nullum(cat, 15) && catalogus_nullum(cat, 17)
        && catalogus_nullum(cat, 22));
    /* summa q15 - q17 + q22 = 0 (paritas impar) et vector nullus (par):
     * idem punctum, omni modo non separat */
    per (modus = ZEPHYRUM; modus < VII; modus++)
    {
        verdictum = VERUM;
        nulla = nulla && activatio_separat(act, W, III, VI, modus,
            &verdictum) && !verdictum;
    }
    CREDO_VERUM (nulla);
    /* refutationes */
    CREDO_FALSUM (activatio_separat(act, W, ZEPHYRUM, VI, ZEPHYRUM,
        &verdictum));
    CREDO_FALSUM (activatio_separat(act, W, VIII, VI, ZEPHYRUM,
        &verdictum));
    CREDO_FALSUM (activatio_separat(act, W, III, ZEPHYRUM, ZEPHYRUM,
        &verdictum));
    CREDO_FALSUM (activatio_separat(act, W, III, VI, VII, &verdictum));
    {
        hic_manens constans i32 EXTRA[II] = { 3, 24 };

        CREDO_FALSUM (activatio_separat(act, EXTRA, II, VI, ZEPHYRUM,
            &verdictum));
    }
}


/* ==================================================
 * T5: quaestiones contra D124
 * ================================================== */

hic_manens constans i32 SCALA_D84[VIII] = { 1, 6, 8, 10, 12, 16, 20,
    24 };

/* victores D82 profundi (DLXIV post MMMDXXXII, regula exacta, quaestio
 * avida D82) ex codice INDEPENDENTI recensoris (Python, Q(sqrt3) per
 * Fractiones, nec cellae nec radices nec extensio; recensio c996526d,
 * 2026-10-12): n, primus k, pondera (indices partis). Ordo et k idem
 * ac activatio_avida - hic singuli pinguntur: separat ad k, ad nullum k
 * prius scalae. */
hic_manens constans i32 VICTORES_D82[] = {
    3, 1, 0, 1, 2,
    3, 1, 0, 1, 3,
    3, 1, 0, 1, 4,
    3, 1, 0, 1, 5,
    3, 1, 0, 1, 6,
    3, 1, 0, 1, 7,
    3, 1, 0, 1, 8,
    3, 6, 0, 1, 10,
    3, 1, 0, 1, 11,
    3, 1, 0, 1, 12,
    3, 8, 0, 1, 13,
    3, 1, 0, 1, 14,
    3, 1, 0, 1, 15,
    3, 1, 0, 1, 16,
    3, 6, 0, 1, 17,
    3, 1, 0, 1, 18,
    3, 1, 0, 1, 19,
    3, 1, 0, 1, 20,
    3, 1, 0, 1, 21,
    3, 1, 0, 1, 22,
    3, 1, 0, 1, 23,
    3, 1, 0, 1, 24,
    3, 1, 0, 1, 25,
    3, 1, 0, 1, 26,
    3, 8, 0, 1, 27,
    3, 1, 0, 1, 28,
    3, 1, 0, 1, 29,
    3, 1, 0, 1, 30,
    3, 1, 0, 1, 31,
    3, 1, 0, 1, 32,
    3, 1, 0, 1, 33,
    3, 6, 0, 1, 34,
    4, 1, 0, 1, 2, 3,
    4, 8, 0, 1, 2, 4,
    4, 10, 0, 1, 2, 5,
    4, 8, 0, 1, 2, 7,
    4, 1, 0, 1, 2, 8,
    4, 6, 0, 1, 2, 10,
    4, 1, 0, 1, 2, 11,
    4, 10, 0, 1, 2, 12,
    4, 24, 0, 1, 2, 13,
    4, 12, 0, 1, 2, 14,
    4, 1, 0, 1, 2, 16,
    4, 6, 0, 1, 2, 17,
    4, 1, 0, 1, 2, 18,
    4, 1, 0, 1, 2, 19,
    4, 1, 0, 1, 2, 20,
    4, 1, 0, 1, 2, 21,
    4, 8, 0, 1, 2, 22,
    4, 6, 0, 1, 2, 23,
    4, 8, 0, 1, 2, 24,
    4, 8, 0, 1, 2, 25,
    4, 1, 0, 1, 2, 26,
    4, 8, 0, 1, 2, 27,
    4, 6, 0, 1, 2, 28,
    4, 12, 0, 1, 2, 29,
    4, 6, 0, 1, 2, 30,
    4, 8, 0, 1, 2, 32,
    4, 1, 0, 1, 2, 33,
    4, 6, 0, 1, 2, 34,
    4, 1, 0, 1, 2, 35,
    4, 8, 0, 1, 2, 36,
    4, 10, 0, 1, 2, 37,
    4, 20, 0, 1, 2, 38,
    5, 8, 0, 1, 2, 3, 8,
    5, 12, 0, 1, 2, 3, 10,
    5, 12, 0, 1, 2, 3, 11,
    5, 24, 0, 1, 2, 3, 12,
    5, 12, 0, 1, 2, 3, 14,
    5, 6, 0, 1, 2, 3, 16,
    5, 8, 0, 1, 2, 3, 19,
    5, 20, 0, 1, 2, 3, 20,
    5, 6, 0, 1, 2, 3, 21,
    5, 24, 0, 1, 2, 3, 22,
    5, 16, 0, 1, 2, 3, 23,
    5, 8, 0, 1, 2, 3, 24,
    5, 8, 0, 1, 2, 3, 25,
    5, 10, 0, 1, 2, 3, 26,
    5, 10, 0, 1, 2, 3, 27,
    5, 16, 0, 1, 2, 3, 28,
    5, 20, 0, 1, 2, 3, 30,
    5, 10, 0, 1, 2, 3, 32,
    5, 10, 0, 1, 2, 3, 33,
    5, 20, 0, 1, 2, 3, 34,
    5, 10, 0, 1, 2, 3, 35,
    5, 12, 0, 1, 2, 3, 36,
    5, 12, 0, 1, 2, 3, 37,
    5, 8, 0, 1, 2, 3, 39,
    5, 8, 0, 1, 2, 3, 41,
    5, 16, 0, 1, 2, 3, 42,
    5, 6, 0, 1, 2, 3, 43,
    5, 20, 0, 1, 2, 3, 44,
    5, 10, 0, 1, 2, 3, 45,
    5, 12, 0, 1, 2, 3, 46,
    5, 16, 0, 1, 2, 3, 49,
    5, 16, 0, 1, 2, 3, 52,
    6, 20, 0, 1, 2, 3, 8, 33,
    6, 20, 0, 1, 2, 3, 8, 81,
    6, 24, 0, 1, 2, 3, 8, 129,
    6, 24, 0, 1, 2, 3, 8, 170,
    6, 24, 0, 1, 2, 3, 8, 176,
    6, 20, 0, 1, 2, 3, 8, 272,
    6, 24, 0, 1, 2, 3, 10, 81,
    6, 24, 0, 1, 2, 3, 10, 461,
    6, 24, 0, 1, 2, 3, 16, 105,
    6, 24, 0, 1, 2, 3, 16, 203,
    6, 20, 0, 1, 2, 3, 16, 241,
    6, 24, 0, 1, 2, 3, 16, 282,
    6, 20, 0, 1, 2, 3, 21, 340,
    6, 16, 0, 1, 2, 3, 23, 394,
    6, 24, 0, 1, 2, 3, 32, 312,
    6, 20, 0, 1, 2, 3, 33, 238,
    6, 16, 0, 1, 2, 3, 33, 315,
    6, 24, 0, 1, 2, 3, 36, 94,
    6, 20, 0, 1, 2, 3, 39, 343,
    6, 24, 0, 1, 2, 3, 42, 238,
    6, 20, 0, 1, 2, 3, 44, 196,
    0
};

interior vacuum
_quaestio (
     Quaestio* q,
          i32  modus)
{
    q->scala_k          = SCALA_D84;
    q->numerus_k        = VIII;
    q->modus            = modus;
    q->n_maximum        = VII;
    q->victores_maximi  = XXXII;
    q->limes_triplicum  = C;
    q->cellae_maximae   = (i32)16384;
    q->limes_servandi   = (i32)4096;
    q->indices          = NIHIL;
    q->numerus_indicum  = ZEPHYRUM;
}

interior b32
_numeri (
    constans Capacitas* c,
          constans i64* exspectati)
{
    i32 n;

    per (n = III; n <= VII; n++)
    {
        si (c->numerus[n] != exspectati[n - III])
        {
            imprimere("    n %d: %ld contra %ld\n", (int)n,
                (long)c->numerus[n], (long)exspectati[n - III]);
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_probare_exhaustivam (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;
    /* D124 Pars D: [pars][modus][n = 3..7] */
    hic_manens constans i64 D124[III][III][V] = {
        { { 1132, 402, 0, 0, 0 }, { 992, 218, 0, 0, 0 },
          { 1785, 2589, 236, 25, 3 } },
        { { 304, 106, 0, 0, 0 }, { 280, 36, 0, 0, 0 },
          { 424, 353, 38, 1, 0 } },
        { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
          { 80, 102, 72, 25, 3 } } };
    hic_manens constans i32 MAXIMA[III][III] = {
        { 8, 8, 14 }, { 8, 8, 12 }, { 0, 0, 14 } };
    Quaternio  g[IV];
    Catalogus* cat = NIHIL;
    Catalogus* partes[III];
          i32  indices_nn[XXIV];
          i32  indices_n[XXIV];
          i32  nn = ZEPHYRUM;
          i32  nu = ZEPHYRUM;
          i32  i;
          i32  pi;
          i32  modus;
          b32  bene = VERUM;

    imprimere("\n--- Quaestio exhaustiva contra D124 ---\n");
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2",
        "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
        si (catalogus_nullum(cat, i)) indices_n[nu++] = i;
        alioquin indices_nn[nn++] = i;
    }
    partes[0] = cat;
    CREDO_NECESSE (catalogus_pars(cat, indices_nn, nn, piscina,
        &partes[1]));
    CREDO_NECESSE (catalogus_pars(cat, indices_n, nu, piscina,
        &partes[2]));
    per (pi = ZEPHYRUM; pi < III; pi++)
    {
        Activatio* act = NIHIL;

        CREDO_NECESSE (activatio_ex_catalogo(partes[pi], ZEPHYRUM,
            piscina,
            &act));
        per (modus = ZEPHYRUM; modus < III; modus++)
        {
             Quaestio q;
            Capacitas cap;

            _quaestio(&q, modus);
            CREDO_NECESSE (activatio_exhaustiva(act, &q, piscina,
                &cap));
            imprimere("    pars %d modus %d: max XOR%d, %ld %ld %ld %ld %ld\n",
                (int)pi, (int)modus, (int)cap.maximum,
                (long)cap.numerus[3],
                (long)cap.numerus[4], (long)cap.numerus[5],
                (long)cap.numerus[6], (long)cap.numerus[7]);
            bene = bene && _numeri(&cap, D124[pi][modus])
                && cap.maximum == MAXIMA[pi][modus];
            si (pi == 0 && modus == CELLAE_REGULA)
            {
                /* nulla inter victores omnes: 1056 et 300 (D124) */
                i64 nulla[II] = { 0, 0 };
                i32 n;
                i32 w;
                i32 t;

                per (n = III; n <= IV; n++)
                {
                    bene = bene
                        && cap.servati[n] == (i32)cap.numerus[n];
                    per (w = ZEPHYRUM; w < cap.servati[n]; w++)
                    {
                        per (t = ZEPHYRUM; t < n; t++)
                        {
                            si (catalogus_nullum(cat,
                                cap.victores[n][w].pondera[t]))
                            {
                                nulla[n - III]++;
                            }
                        }
                    }
                }
                bene = bene && nulla[0] == 1056 && nulla[1] == 300;
            }
        }
    }
    CREDO_VERUM (bene);
}

/* D84 find_xor6 / find_recursive ad litteram (separabilitas per
 * activatio_separat): referens ordinis victorum quaestionis avidae */
#define REF_MAX XXXII

hic_manens i32 ref_victor[VII][REF_MAX][VII];
hic_manens i32 ref_k[VII][REF_MAX];
hic_manens i32 ref_numerus[VII];

interior i32
_ref_primus_k (
    Activatio* act,
          i32* w,
          i32  n)
{
    i32 index_k;

    per (index_k = ZEPHYRUM; index_k < VIII; index_k++)
    {
        b32 verdictum = FALSUM;

        si (   activatio_separat(act, w, n, SCALA_D84[index_k],
            CELLAE_REGULA,
                &verdictum) && verdictum)
        {
            redde SCALA_D84[index_k];
        }
    }
    redde ZEPHYRUM;
}

interior vacuum
_avida_referens (
    Activatio* act,
          i32  m)
{
    i32 a;
    i32 b;
    i32 cc;
    i32 n;

    per (n = ZEPHYRUM; n < VII; n++) ref_numerus[n] = ZEPHYRUM;
    per (a = ZEPHYRUM; a < m
        && ref_numerus[III] < REF_MAX; a++) per (b = a + I; b < m
                                                && ref_numerus[III]
                                                    < REF_MAX; b++) per (cc =
                                                                        b
                                                                            + I; cc
                                                                            < m
                                                                            && ref_numerus[III]
                                                                                < REF_MAX; cc++)
                                                                    {
        i32 w[III];
        i32 k;

        w[0] = a; w[1] = b; w[2]  = cc;
        k                         = _ref_primus_k(act, w, III);
        si (k > ZEPHYRUM)
        {
            ref_victor[III][ref_numerus[III]][0]  = a;
            ref_victor[III][ref_numerus[III]][1]  = b;
            ref_victor[III][ref_numerus[III]][2]  = cc;
            ref_k[III][ref_numerus[III]++]        = k;
        }
                                                                    }
    per (n = IV; n <= VI; n++)
    {
        i32 prior = n - I;
        i32 wi;
        i32 ae;

        si (ref_numerus[prior] == ZEPHYRUM) frange;
        per (wi = ZEPHYRUM; wi < ref_numerus[prior]
            && ref_numerus[n] < REF_MAX; wi++) per (ae = ZEPHYRUM; ae
                                                   < m
                                                   && ref_numerus[n]
                                                       < REF_MAX; ae++)
                                               {
            i32 idx[VII];
            i32 pos = prior;
            i32 i;
            i32 j;
            b32 inest  = FALSUM;
            b32 dup    = FALSUM;
            i32 k;

            per (i = ZEPHYRUM; i < prior; i++)
            {
                si (ref_victor[prior][wi][i] == ae) inest = VERUM;
            }
            si (inest) perge;
            per (i = ZEPHYRUM; i < prior; i++)
            {
                si (ae < ref_victor[prior][wi][i])
                { pos = i; frange;
                }
            }
            per (i = ZEPHYRUM; i < pos; i++) idx[i] =
                                                 ref_victor[prior][wi][i];
            idx[pos] = ae;
            per (i = pos; i < prior; i++) idx[i + I] =
                                              ref_victor[prior][wi][i];
            per (j = ZEPHYRUM; j < ref_numerus[n] && !dup; j++)
            {
                b32 par = VERUM;

                per (i = ZEPHYRUM; i < n && par; i++)
                {
                    par = ref_victor[n][j][i] == idx[i];
                }
                dup = par;
            }
            si (dup) perge;
            k = _ref_primus_k(act, idx, n);
            si (k > ZEPHYRUM)
            {
                per (i = ZEPHYRUM; i < n; i++)
                {
                    ref_victor[n][ref_numerus[n]][i] = idx[i];
                }
                ref_k[n][ref_numerus[n]++] = k;
            }
                                               }
    }
}

/* victores quaestionis avidae == referens D84: idem ordo, idem k */
interior b32
_avida_ut_d84 (
             Activatio* act,
                   i32  m,
    constans Capacitas* cap)
{
    i32 n;
    i32 w;
    i32 t;

    _avida_referens(act, m);
    per (n = III; n <= VI; n++)
    {
        si (   cap->numerus[n] != (i64)ref_numerus[n]
            || cap->servati[n] != ref_numerus[n])
        {
            redde FALSUM;
        }
        per (w = ZEPHYRUM; w < ref_numerus[n]; w++)
        {
            si (cap->victores[n][w].k != ref_k[n][w]) redde FALSUM;
            per (t = ZEPHYRUM; t < n; t++)
            {
                si (cap->victores[n][w].pondera[t]
                    != ref_victor[n][w][t])
                {
                    redde FALSUM;
                }
            }
        }
    }
    redde VERUM;
}

interior vacuum
_probare_quaestiones_alias (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                  Quaternio  g[IV];
                  Catalogus* cat = NIHIL;
                  Catalogus* partes[III];
                  Activatio* plena = NIHIL;
                        i32  indices_nn[XXIV];
                        i32  indices_n[XXIV];
                        i32  nn = ZEPHYRUM;
                        i32  nu = ZEPHYRUM;
                        i32  i;
                        i32  modus;
                        i32  fr;
                        i32  pi;
                   Quaestio  q;
                  Capacitas  cap;
    hic_manens constans i64  TREDECIM[II][II][V] = {
        { { 328, 184, 0, 0, 0 }, { 4, 0, 0, 0, 0 } },
        { { 328, 88, 0, 0, 0 }, { 4, 0, 0, 0, 0 } } };
    b32 bene = VERUM;

    imprimere("\n--- Tabula XIII axium, regulae fixae, avida, servati ---\n");
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2",
        "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
        si (catalogus_nullum(cat, i)) indices_n[nu++] = i;
        alioquin indices_nn[nn++] = i;
    }
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina,
        &plena));
    /* pondera subcopiae, tabula plena (XIII axes): Quaestio.indices */
    per (modus = ZEPHYRUM; modus < II; modus++)
    {
        _quaestio(&q, modus);
        q.indices          = indices_nn;
        q.numerus_indicum  = nn;
        CREDO_NECESSE (activatio_exhaustiva(plena, &q, piscina, &cap));
        bene               = bene && _numeri(&cap, TREDECIM[modus][0]);
        q.indices          = indices_n;
        q.numerus_indicum  = nu;
        CREDO_NECESSE (activatio_exhaustiva(plena, &q, piscina, &cap));
        bene = bene && _numeri(&cap, TREDECIM[modus][1]);
    }
    CREDO_VERUM (bene);
    /* regulae fixae: maxima (8, 8, 0) omnes */
    partes[0] = cat;
    CREDO_NECESSE (catalogus_pars(cat, indices_nn, nn, piscina,
        &partes[1]));
    CREDO_NECESSE (catalogus_pars(cat, indices_n, nu, piscina,
        &partes[2]));
    bene = VERUM;
    per (pi = ZEPHYRUM; pi < III; pi++)
    {
        Activatio* act = NIHIL;

        CREDO_NECESSE (activatio_ex_catalogo(partes[pi], ZEPHYRUM,
            piscina,
            &act));
        per (fr = ZEPHYRUM; fr < IV; fr++)
        {
            _quaestio(&q, III + fr);
            CREDO_NECESSE (activatio_exhaustiva(act, &q, piscina,
                &cap));
            bene = bene && cap.maximum == (pi == II ? ZEPHYRUM : VIII);
        }
        /* avida D84 cum cellis exactis (regula): 8, 8, 0 */
        _quaestio(&q, CELLAE_REGULA);
        q.n_maximum = VI;
        CREDO_NECESSE (activatio_avida(act, &q, piscina, &cap));
        imprimere("    avida pars %d: max XOR%d, inventi %ld %ld %ld %ld\n",
            (int)pi, (int)cap.maximum, (long)cap.numerus[3],
            (long)cap.numerus[4], (long)cap.numerus[5],
            (long)cap.numerus[6]);
        bene = bene && cap.maximum == (pi == II ? ZEPHYRUM : VIII)
            && cap.numerus[3] <= XXXII && cap.numerus[4] <= XXXII;
        bene = bene && _avida_ut_d84(act, catalogus_numerus(partes[pi]),
            &cap);
        /* omnis victor servatus ad k suum separat */
        {
            i32 n;
            i32 w;

            per (n = III; n <= VI; n++)
            {
                per (w = ZEPHYRUM; w < cap.servati[n]; w++)
                {
                    b32 verdictum = FALSUM;
                    i32 index_k;

                    bene = bene && activatio_separat(act,
                        cap.victores[n][w].pondera, n,
                        cap.victores[n][w].k,
                        CELLAE_REGULA, &verdictum) && verdictum;
                    /* k servatus = primus scalae qui transit */
                    per (index_k = ZEPHYRUM; SCALA_D84[index_k]
                        != cap.victores[n][w].k; index_k++)
                    {
                        bene = bene && activatio_separat(act,
                            cap.victores[n][w].pondera, n,
                            SCALA_D84[index_k],
                            CELLAE_REGULA, &verdictum) && !verdictum;
                    }
                }
            }
        }
    }
    CREDO_VERUM (bene);
    /* numerus verus contra servatos: limes servandi V */
    _quaestio(&q, CELLAE_REGULA);
    q.limes_servandi = V;
    CREDO_NECESSE (activatio_exhaustiva(plena, &q, piscina, &cap));
    CREDO_VERUM (cap.numerus[III] == 1132 && cap.servati[III] == V
        && cap.numerus[IV] == 402 && cap.servati[IV] == V);
    q.limes_servandi = ZEPHYRUM;
    CREDO_NECESSE (activatio_exhaustiva(plena, &q, piscina, &cap));
    CREDO_VERUM (cap.numerus[III] == 1132
        && cap.servati[III] == ZEPHYRUM);
    /* refutationes */
    q.n_maximum = VIII;
    CREDO_FALSUM (activatio_exhaustiva(plena, &q, piscina, &cap));
    _quaestio(&q, CELLAE_REGULA);
    q.numerus_k = ZEPHYRUM;
    CREDO_FALSUM (activatio_avida(plena, &q, piscina, &cap));
    {
        hic_manens constans i32 MALI[II] = { 5, 3 };

        _quaestio(&q, CELLAE_REGULA);
        q.indices          = MALI;
        q.numerus_indicum  = II;
        CREDO_FALSUM (activatio_exhaustiva(plena, &q, piscina, &cap));
    }
}


/* ==================================================
 * T6: filtrum sine iactura (zeta_12)
 * ================================================== */

interior Catalogus*
_zeta_duodecim (
     Piscina* piscina,
         i32  primi)
{
    Quaternio  g[IV];
    Catalogus* cat   = NIHIL;
    Catalogus* pars  = NIHIL;
          i32  indices[C];
          i32  i;

    _generatores(extensio_anulus(extensio_quadratica(III, piscina)),
        "a/2", "1/2", "-1/2", piscina, g);
    si (!catalogus_clausura(g, TORSIONES, IV, (i32)F_MAX, XVI, piscina,
        &cat))
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < primi; i++) indices[i] = i;
    si (!catalogus_pars(cat, indices, primi, piscina, &pars))
    {
        redde NIHIL;
    }
    redde pars;
}

interior vacuum
_probare_filtrum (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
    hic_manens constans i32  VALORES_K[III] = { 6, 12, 24 };
                  Catalogus* cat = _zeta_duodecim(piscina, XXX);
                  Activatio* cum = NIHIL;
                  Activatio* sine = NIHIL;
           CellaeStatistica  st_cum;
           CellaeStatistica  st_sine;
                        i32  a;
                        i32  b;
                        i32  cc;
                        i32  index_k;
                        i32  modus;
                        i64  comparata  = 0;
                        i64  discordes  = 0;
                        i64  verae      = 0;

    imprimere("\n--- Filtrum: verdicta eadem cum et sine (zeta_12, XXX) ---\n");
    CREDO_NECESSE (cat != NIHIL);
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina, &cum));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina,
        &sine));
    activatio_filtrum(sine, FALSUM);
    per (a = ZEPHYRUM; a < XXX; a++) per (b = a + I; b
                                         < XXX; b++) per (cc = b + I; cc
                                                         < XXX; cc++) per (index_k =
                                                                          ZEPHYRUM; index_k
                                                                              < III; index_k++) per (modus =
                                                                                                    ZEPHYRUM; modus
                                                                                                        < III; modus++)
                                                                                                {
        i32 w[III];
        b32 v1 = FALSUM;
        b32 v2 = FALSUM;
        b32 b1;
        b32 b2;

        w[0] = a; w[1] = b; w[2] = cc;
        b1 = activatio_separat(cum, w, III, VALORES_K[index_k], modus,
            &v1);
        b2 = activatio_separat(sine, w, III, VALORES_K[index_k], modus,
            &v2);
        comparata++;
        si (b1 != b2 || v1 != v2) discordes++;
        si (v1) verae++;
                                                                                                }
    activatio_statistica(cum, &st_cum);
    activatio_statistica(sine, &st_sine);
    imprimere("    comparata %ld, discordes %ld, verae %ld\n",
        (long)comparata,
        (long)discordes, (long)verae);
    imprimere("    cum filtro: filtro %ld exacte %ld ligatae %ld; sine: filtro "
        "%ld exacte %ld ligatae %ld\n", (long)st_cum.filtro,
        (long)st_cum.exacte, (long)st_cum.ligatae, (long)st_sine.filtro,
        (long)st_sine.exacte, (long)st_sine.ligatae);
    /* C(XXX, III) x III k x III modi */
    CREDO_VERUM (comparata == 36540 && discordes == 0 && verae > 0);
    /* sine filtro nihil filtratum; cum filtro pars maior; paritates solae
     * viae exactae (filtrum paritatem numquam decidit) */
    CREDO_VERUM (st_sine.filtro == 0 && st_sine.exacte > 0);
    CREDO_VERUM (st_cum.filtro > st_cum.exacte && st_cum.ligatae
        <= st_cum.exacte);
    /* numeri decisionum differre possunt (ordo terminationis in modo
     * possibili: punctum filtratum identitatem non notat, SAT eandem
     * contradictionem postea invenit) - verdicta non */
}

interior vacuum
_probare_filtrum_avidum (
    CredoContextus* c)
{
      Piscina* piscina  = c->piscina;
    Catalogus* cat      = _zeta_duodecim(piscina, LI);
    Activatio* cum      = NIHIL;
    Activatio* sine     = NIHIL;
    Capacitas  c1;
    Capacitas  c2;
     Quaestio  q;
          i32  modus;
          b32  bene = VERUM;

    imprimere("\n--- Filtrum: quaestio avida eadem (zeta_12, LI) ---\n");
    CREDO_NECESSE (cat != NIHIL);
    per (modus = ZEPHYRUM; modus < II; modus++)
    {
        i32 n;

        CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina,
            &cum));
        CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina,
            &sine));
        activatio_filtrum(sine, FALSUM);
        _quaestio(&q, modus);
        q.n_maximum = VI;
        CREDO_NECESSE (activatio_avida(cum, &q, piscina, &c1));
        CREDO_NECESSE (activatio_avida(sine, &q, piscina, &c2));
        imprimere("    modus %d: max XOR%d, inventi %ld %ld %ld %ld\n",
            (int)modus, (int)c1.maximum, (long)c1.numerus[3],
            (long)c1.numerus[4], (long)c1.numerus[5],
            (long)c1.numerus[6]);
        bene = bene && c1.maximum == c2.maximum;
        per (n = III; n <= VI; n++)
        {
            i32 w;
            i32 t;

            bene = bene && c1.numerus[n] == c2.numerus[n]
                && c1.servati[n] == c2.servati[n];
            per (w = ZEPHYRUM; bene && w < c1.servati[n]; w++)
            {
                bene = bene
                    && c1.victores[n][w].k == c2.victores[n][w].k;
                per (t = ZEPHYRUM; t < n; t++)
                {
                    bene = bene && c1.victores[n][w].pondera[t]
                        == c2.victores[n][w].pondera[t];
                }
            }
        }
    }
    CREDO_VERUM (bene);
}

/* catalogus profundus D82: DLXIV elementa post MMMDXXXII clausurae
 * zeta_12 ad MMMMXCVI truncatae (CDLXXVI axes) */
interior Catalogus*
_zeta_duodecim_profundus (
    Piscina* piscina)
{
    Quaternio  g[IV];
    Catalogus* cat   = NIHIL;
    Catalogus* pars  = NIHIL;
          i32* indices;
          i32  t;

    _generatores(extensio_anulus(extensio_quadratica(III, piscina)),
        "a/2", "1/2", "-1/2", piscina, g);
    si (   !catalogus_clausura(g, TORSIONES, IV, (i32)F_MAX, XVI,
        piscina,
        &cat)
        || catalogus_numerus(cat) < MMMDXXXII + DLXIV)
    {
        redde NIHIL;
    }
    indices = (i32*)piscina_allocare(piscina,
        (memoriae_index)DLXIV * magnitudo(i32));
    si (indices == NIHIL)
    {
        redde NIHIL;
    }
    per (t = ZEPHYRUM; t < DLXIV; t++) indices[t] = MMMDXXXII + t;
    si (!catalogus_pars(cat, indices, DLXIV, piscina, &pars))
    {
        redde NIHIL;
    }
    redde pars;
}

/* scrutinium axium in II gradibus (T7): catalogus profundus (DLXIV
 * elementa post MMMDXXXII, CDLXXVI axes) - gradus I axes certe inferiores
 * abicit; verdicta cum filtro (scrutinium binum) aequant verdicta sine
 * (quaternio_proximi super omnes axes) in omnibus modis, ligaturis
 * axium inclusis (modus robustus omnes maximas videt) */
interior vacuum
_probare_scrutinium (
    CredoContextus* c)
{
                     Piscina* piscina         = c->piscina;
     hic_manens constans i32  VALORES_K[III]  = { 6, 12, 24 };
                   Catalogus* pars            = NIHIL;
                   Activatio* cum             = NIHIL;
                   Activatio* sine            = NIHIL;
            CellaeStatistica  st_cum;
                         i32  numerus;
                         i32  t;
                         i32  index_k;
                         i32  modus;
                         i64  comparata  = 0;
                         i64  discordes  = 0;
                         i64  verae      = 0;

    imprimere("\n--- Scrutinium axium: binum = lineare (zeta_12 profundus)"
        " ---\n");
    pars = _zeta_duodecim_profundus(piscina);
    CREDO_NECESSE (pars != NIHIL);
    numerus = catalogus_numerus(pars);
    CREDO_NECESSE (activatio_ex_catalogo(pars, ZEPHYRUM, piscina,
        &cum));
    CREDO_NECESSE (activatio_ex_catalogo(pars, ZEPHYRUM, piscina,
        &sine));
    activatio_filtrum(sine, FALSUM);
    imprimere("    axes %d\n", (int)activatio_axes(cum));
    CREDO_VERUM (activatio_axes(cum) == CDLXXVI);
    /* triplices per gradus primos (deterministice) */
    per (t = ZEPHYRUM; t < CC; t++)
    {
        i32 w[III];

        w[0] = (t * VII) % (i32)numerus;
        w[1] = (w[0] + I + (t * XIII) % (i32)(numerus - II))
            % (i32)numerus;
        w[2] = (w[1] + I + (t * XXIX) % (i32)(numerus - III))
            % (i32)numerus;
        si (w[0] == w[1] || w[1] == w[2] || w[0] == w[2])
        {
            perge;
        }
        per (index_k = ZEPHYRUM; index_k < III; index_k++)
        {
            per (modus = ZEPHYRUM; modus < III; modus++)
            {
                b32 v1 = FALSUM;
                b32 v2 = FALSUM;
                b32 b1 = activatio_separat(cum, w, III,
                    VALORES_K[index_k], modus, &v1);
                b32 b2 = activatio_separat(sine, w, III,
                    VALORES_K[index_k], modus, &v2);

                comparata++;
                si (b1 != b2 || v1 != v2) discordes++;
                si (v1) verae++;
            }
        }
    }
    activatio_statistica(cum, &st_cum);
    imprimere("    comparata %ld, discordes %ld, verae %ld; filtro %ld "
        "exacte %ld ligatae %ld\n", (long)comparata, (long)discordes,
        (long)verae, (long)st_cum.filtro, (long)st_cum.exacte,
        (long)st_cum.ligatae);
    CREDO_VERUM (comparata > D && discordes == 0 && verae > 0
        && verae < comparata);
    CREDO_VERUM (st_cum.filtro > ZEPHYRUM && st_cum.ligatae > ZEPHYRUM);
}

/* alea simplex deterministica (LCG XXXII bitorum) */
hic_manens i32 o_status_aleae = I;

interior i32
_o_alea (
    i32 modulus)
{
    o_status_aleae = o_status_aleae * 1103515245U + 12345U;
    redde ((o_status_aleae >> 16) & 0x7FFFU) % modulus;
}

/* L1 recensionis: cellae zeta_12 contra oraculum Z[sqrt3] (scala XXXII)
 * - summae fortuitae I-VII elementorum profundorum signis fortuitis,
 * regula et ligaturae omnes per k */
interior vacuum
_probare_cellas_duodecim (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
    hic_manens constans i32  VALORES_K[XII] = { 1, 2, 4, 5, 6, 7, 8, 10,
        12, 16, 20, 24 };
                   Catalogus* pars  = NIHIL;
                   Activatio* act   = NIHIL;
                         i32  numerus;
                         i32  t;
                         i32  index_k;
                         i64  comparata  = 0;
                         i64  discordes  = 0;
                         i64  ligata     = 0;
                         i64  refutata   = 0;

    imprimere("\n--- Cellae zeta_12 contra oraculum Z[sqrt3] ---\n");
    o_d             = III;
    o_scala         = XXXII;
    o_vitium        = FALSUM;
    o_status_aleae  = I;
    pars            = _zeta_duodecim_profundus(piscina);
    CREDO_NECESSE (pars != NIHIL);
    CREDO_NECESSE (activatio_ex_catalogo(pars, ZEPHYRUM, piscina,
        &act));
    _o_axes(pars, piscina);
    CREDO_AEQUALIS_I32 (o_nd, CDLXXVI);
    CREDO_NECESSE (!o_vitium);
    numerus = catalogus_numerus(pars);
    per (t = ZEPHYRUM; t < M; t++)
    {
              i32 longitudo = I + _o_alea(VII);
        Quaternio s;
         Binomium so[IV];
              i32 u;
              i32 r;

        (vacuum)quaternio_nullum(catalogus_elementum(pars, 0).anulus,
            piscina, &s);
        per (r = 0; r < IV; r++) so[r] = binomium(0, 0);
        per (u = ZEPHYRUM; u < longitudo; u++)
        {
                  i32 index      = _o_alea(numerus);
                  b32 positivum  = _o_alea(II) == ZEPHYRUM;
            Quaternio q          = catalogus_elementum(pars, index);

            si (positivum) (vacuum)quaternio_adde(s, q, piscina, &s);
            alioquin (vacuum)quaternio_subtrahe(s, q, piscina, &s);
            per (r = 0; r < IV; r++)
            {
                Binomium e = _binomium_ex(q, r, piscina);

                so[r] = positivum ? binomium_adde(so[r], e)
                    : binomium_subtrahe(so[r], e);
            }
        }
        per (index_k = ZEPHYRUM; index_k < XII; index_k++)
        {
            Cella cella;
              i32 o_regula = 0;
              i32 o_ligatae[2 * O_MAX];
              i32 on = 0;
              i32 elig[2 * O_MAX];
              b32 bene;

            si (   !activatio_cella(act, s, VALORES_K[index_k], &cella)
                || !_o_cella(so, VALORES_K[index_k], &o_regula,
                o_ligatae,
                &on))
            {
                refutata++;
                perge;
            }
            bene = cella.regula == o_regula && cella.numerus == on;
            si (bene)
            {
                per (u = 0; u < on; u++) elig[u] = cella.ligatae[u];
                _ordina(elig, on);
                _ordina(o_ligatae, on);
                per (u = 0; u < on; u++) bene = bene
                                             && elig[u] == o_ligatae[u];
            }
            comparata++;
            si (!bene) discordes++;
            si (on > 1) ligata++;
        }
    }
    imprimere("    comparata %ld, discordes %ld, ligata %ld, refutata %ld\n",
        (long)comparata, (long)discordes, (long)ligata, (long)refutata);
    CREDO_FALSUM (o_vitium);
    CREDO_VERUM (comparata == M * XII && discordes == 0
        && refutata == 0);
    CREDO_VERUM (ligata > 0);
}

/* L1 recensionis: victores D82 profundi (codex independens) - singuli
 * separant ad suum k (regula exacta, per filtrum), ad nullum k prius */
interior vacuum
_probare_victores_d82 (
    CredoContextus* c)
{
      Piscina* piscina  = c->piscina;
    Catalogus* pars     = NIHIL;
    Activatio* act      = NIHIL;
          i32  numeri[VIII];
          i32  positio  = ZEPHYRUM;
          i32  falsi    = ZEPHYRUM;
          i32  n;

    imprimere("\n--- Victores D82 profundi (codex independens) ---\n");
    pars = _zeta_duodecim_profundus(piscina);
    CREDO_NECESSE (pars != NIHIL);
    CREDO_NECESSE (activatio_ex_catalogo(pars, ZEPHYRUM, piscina,
        &act));
    per (n = ZEPHYRUM; n < VIII; n++) numeri[n] = ZEPHYRUM;
    dum (VICTORES_D82[positio] != ZEPHYRUM)
    {
         constans i32* w = &VICTORES_D82[positio + II];
                  i32  k = VICTORES_D82[positio + I];
                  i32  index_k;

        n = VICTORES_D82[positio];
        numeri[n]++;
        per (index_k = ZEPHYRUM; index_k < VIII; index_k++)
        {
            b32 verum = FALSUM;

            si (SCALA_D84[index_k] > k) frange;
            si (   !activatio_separat(act, w, n, SCALA_D84[index_k],
                CELLAE_REGULA, &verum)
                || verum != (SCALA_D84[index_k] == k))
            {
                falsi++;
            }
        }
        positio += II + n;
    }
    imprimere("    victores %d %d %d %d, falsi %d\n", (int)numeri[3],
        (int)numeri[4], (int)numeri[5], (int)numeri[6], (int)falsi);
    CREDO_VERUM (numeri[3] == XXXII && numeri[4] == XXXII
        && numeri[5] == XXXII && numeri[6] == XXI);
    CREDO_AEQUALIS_I32 (falsi, ZEPHYRUM);
}

/* L2 recensionis: cellae_maximae (D82 MAX_ACT) et limes_triplicum (D82
 * bf_limit) ligant - zeta_8 (XIII axes, XIV cellae per sectorem) */
interior vacuum
_probare_limites_quaestionis (
    CredoContextus* c)
{
                     Piscina* piscina           = c->piscina;
     hic_manens constans i32  SCALA_BREVIS[II]  = { 1, 6 };
                   Quaternio  g[IV];
                   Catalogus* cat = NIHIL;
                   Activatio* act = NIHIL;
                    Quaestio  q;
                   Capacitas  plena;
                   Capacitas  capta;
                   Capacitas  sine;
                   Capacitas  avida;
                   Capacitas  angusta;
                         i32  n;
                         i32  w;
                         i32  t;
                         b32  aequales  = VERUM;
                         b32  ultra     = FALSUM;
                         b32  intra     = VERUM;

    imprimere("\n--- Limites quaestionis (cellae_maximae, limes_triplicum)"
        " ---\n");
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2", "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina, &act));
    CREDO_NECESSE (activatio_axes(act) == XIII);
    /* k >= VIII omittitur: VIII x XIV = CXII > LXXXIV = VI x XIV (k XXIV
     * solus nihil novi in zeta_8 invenit - mensuratum) */
    _quaestio(&q, CELLAE_ROBUSTA);
    q.cellae_maximae = ZEPHYRUM;
    CREDO_NECESSE (activatio_exhaustiva(act, &q, piscina, &plena));
    q.cellae_maximae = LXXXIV;
    CREDO_NECESSE (activatio_exhaustiva(act, &q, piscina, &capta));
    q.cellae_maximae  = ZEPHYRUM;
    q.scala_k         = SCALA_BREVIS;
    q.numerus_k       = II;
    CREDO_NECESSE (activatio_exhaustiva(act, &q, piscina, &sine));
    per (n = III; n <= VII; n++)
    {
        aequales = aequales && capta.numerus[n] == sine.numerus[n]
            && capta.servati[n] == sine.servati[n];
        per (w = ZEPHYRUM; aequales && w < capta.servati[n]; w++)
        {
            aequales = aequales
                && capta.victores[n][w].k == sine.victores[n][w].k;
        }
        per (w = ZEPHYRUM; w < plena.servati[n]; w++)
        {
            si (plena.victores[n][w].k > VI) ultra = VERUM;
        }
    }
    imprimere("    plena max XOR%d (%ld %ld %ld), capta max XOR%d (%ld %ld "
        "%ld), k > VI in plena: %d\n", (int)plena.maximum,
        (long)plena.numerus[3], (long)plena.numerus[4],
        (long)plena.numerus[5], (int)capta.maximum,
        (long)capta.numerus[3], (long)capta.numerus[4],
        (long)capta.numerus[5], (int)ultra);
    CREDO_VERUM (aequales && capta.maximum == sine.maximum);
    CREDO_VERUM (ultra);
    /* limes_triplicum V: triplices ex primis V ponderibus (C(V,III) = X) */
    _quaestio(&q, CELLAE_REGULA);
    CREDO_NECESSE (activatio_avida(act, &q, piscina, &avida));
    q.limes_triplicum = V;
    CREDO_NECESSE (activatio_avida(act, &q, piscina, &angusta));
    per (w = ZEPHYRUM; w < angusta.servati[3]; w++)
    {
        per (t = ZEPHYRUM; t < III; t++)
        {
            intra = intra && angusta.victores[3][w].pondera[t] < V;
        }
    }
    imprimere("    avida triplices %ld, limes V triplices %ld\n",
        (long)avida.numerus[3], (long)angusta.numerus[3]);
    CREDO_VERUM (intra && angusta.numerus[3] <= X);
    CREDO_VERUM (avida.numerus[3] > X);
    /* pondera duplicata refutantur (recensio P2) */
    {
        i32 duplicata[III];
        b32 verum = FALSUM;

        duplicata[0] = I; duplicata[1] = IV; duplicata[2] = I;
        CREDO_FALSUM (activatio_separat(act, duplicata, III, VI,
            CELLAE_REGULA, &verum));
        duplicata[2] = V;
        CREDO_VERUM (activatio_separat(act, duplicata, III, VI,
            CELLAE_REGULA, &verum));
    }
}

/* cellae II T1: catalogi ex punctis et tabulae axium expressae. Via
 * altera ad numeros D124 XIII axium: subcopia ut catalogus PROPRIUS
 * (catalogus_ex_punctis / catalogus_pars) cum tabula plenae expressa,
 * non pondera restricta (Quaestio.indices) intra catalogum plenum. */
interior vacuum
_probare_puncta_et_axes (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                  Quaternio  g[IV];
                  Catalogus* cat         = NIHIL;
                  Catalogus* ex_punctis  = NIHIL;
                  Catalogus* non_nulla   = NIHIL;
                  Catalogus* nulla       = NIHIL;
                  Activatio* plena       = NIHIL;
                  Activatio* creata      = NIHIL;
                  Activatio* act         = NIHIL;
                  Quaternio  puncta[XXIV];
                  Quaternio  puncta_nn[XXIV];
                  Quaternio  axes[XXIV];
                        i32  indices_n[XXIV];
                        i32  nn = ZEPHYRUM;
                        i32  nu = ZEPHYRUM;
                        i32  i;
                        i32  modus;
                   Quaestio  q;
                  Capacitas  cap;
                        b32  bene = VERUM;
    hic_manens constans i64  PLENA[II][V] = {
        { 1132, 402, 0, 0, 0 }, { 992, 218, 0, 0, 0 } };
    hic_manens constans i64 TREDECIM[II][II][V] = {
        { { 328, 184, 0, 0, 0 }, { 4, 0, 0, 0, 0 } },
        { { 328, 88, 0, 0, 0 }, { 4, 0, 0, 0, 0 } } };
    hic_manens constans i64 PROPRIA_NN[V] = { 304, 106, 0, 0, 0 };

    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2", "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_AEQUALIS_I32 (catalogus_numerus(cat), XXIV);
    per (i = ZEPHYRUM; i < catalogus_numerus(cat); i++)
    {
        puncta[i] = catalogus_elementum(cat, i);
        si (catalogus_nullum(cat, i)) indices_n[nu++] = i;
        alioquin puncta_nn[nn++] = puncta[i];
    }
    /* catalogus ex punctis clausurae: idem elementum per elementum */
    CREDO_NECESSE (catalogus_ex_punctis(puncta, XXIV, piscina,
        &ex_punctis));
    CREDO_AEQUALIS_I32 (catalogus_numerus(ex_punctis), XXIV);
    per (i = ZEPHYRUM; i < XXIV; i++)
    {
        bene = bene
            && quaternio_aequalis(catalogus_elementum(ex_punctis,
            i), puncta[i]) && catalogus_profunditas(ex_punctis, i) == 0
            && catalogus_torsio(ex_punctis, i) == 0
            && catalogus_nullum(ex_punctis, i) == catalogus_nullum(cat,
            i);
    }
    CREDO_VERUM (bene);
    CREDO_AEQUALIS_I32 (catalogus_gradus(ex_punctis), ZEPHYRUM);
    CREDO_FALSUM (catalogus_truncatus(ex_punctis));
    /* tabula propria: creare(NIHIL) == ex_catalogo, axes puri */
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina,
        &plena));
    CREDO_NECESSE (activatio_creare(ex_punctis, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D82, ZEPHYRUM, piscina, &creata));
    CREDO_AEQUALIS_I32 (activatio_axes(plena), XIII);
    CREDO_AEQUALIS_I32 (activatio_axes(creata), XIII);
    bene = VERUM;
    per (i = ZEPHYRUM; i < XIII; i++)
    {
        axes[i] = activatio_axis(plena, i);
        bene = bene
            && quaternio_aequalis(axes[i], activatio_axis(creata,
            i)) && algebraicus_est_nullum(*(constans Algebraicus*)
            quaternio_pars(axes[i], ZEPHYRUM));
    }
    CREDO_VERUM (bene);
    CREDO_NIHIL (activatio_axis(plena, XIII).anulus);
    /* tabula expressa == propria: numeri D124 (plena) non mutantur */
    CREDO_NECESSE (activatio_creare(ex_punctis, axes, XIII,
        CELLAE_CELLA_D82, ZEPHYRUM, piscina, &act));
    bene = VERUM;
    per (modus = ZEPHYRUM; modus < II; modus++)
    {
        _quaestio(&q, modus);
        CREDO_NECESSE (activatio_exhaustiva(act, &q, piscina, &cap));
        bene = bene && _numeri(&cap, PLENA[modus])
            && cap.maximum == VIII;
    }
    CREDO_VERUM (bene);
    /* subcopiae ut catalogi proprii cum tabula plenae: numeri XIII
     * axium D124, via altera quam Quaestio.indices */
    CREDO_NECESSE (catalogus_ex_punctis(puncta_nn, nn, piscina,
        &non_nulla));
    CREDO_NECESSE (catalogus_pars(cat, indices_n, nu, piscina, &nulla));
    bene = VERUM;
    per (modus = ZEPHYRUM; modus < II; modus++)
    {
        Activatio* a_nn  = NIHIL;
        Activatio* a_n   = NIHIL;

        _quaestio(&q, modus);
        CREDO_NECESSE (activatio_creare(non_nulla, axes, XIII,
            CELLAE_CELLA_D82, ZEPHYRUM, piscina, &a_nn));
        CREDO_NECESSE (activatio_exhaustiva(a_nn, &q, piscina, &cap));
        bene = bene && _numeri(&cap, TREDECIM[modus][0]);
        CREDO_NECESSE (activatio_creare(nulla, axes, XIII,
            CELLAE_CELLA_D82, ZEPHYRUM, piscina, &a_n));
        CREDO_NECESSE (activatio_exhaustiva(a_n, &q, piscina, &cap));
        bene = bene && _numeri(&cap, TREDECIM[modus][1]);
    }
    CREDO_VERUM (bene);
    /* tabula propria subcopiae non nullae: numeri D124 (304) - tabula
     * refert */
    {
        Activatio* propria = NIHIL;

        CREDO_NECESSE (activatio_creare(non_nulla, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D82, ZEPHYRUM, piscina, &propria));
        _quaestio(&q, CELLAE_REGULA);
        CREDO_NECESSE (activatio_exhaustiva(propria, &q, piscina,
            &cap));
        CREDO_VERUM (_numeri(&cap, PROPRIA_NN));
        CREDO_MINOR_I32 (activatio_axes(propria), XIII);
    }
    /* axes duplicati, antipodes, multipla: unus; axes_maximae */
    {
        Quaternio  tres[IV];
        Activatio* a3 = NIHIL;
        Quaternio  duplum;
        Quaternio  negatum;
        Quaternio  nullum;

        CREDO_NECESSE (quaternio_adde(axes[I], axes[I], piscina,
            &duplum));
        CREDO_NECESSE (quaternio_nullum(quaternio_anulus(axes[I]),
            piscina,
            &nullum));
        CREDO_NECESSE (quaternio_subtrahe(nullum, axes[I], piscina,
            &negatum));
        tres[ZEPHYRUM]  = axes[I];
        tres[I]         = duplum;
        tres[II]        = negatum;
        tres[III]       = axes[II];
        CREDO_NECESSE (activatio_creare(ex_punctis, tres, IV,
            CELLAE_CELLA_D82, ZEPHYRUM, piscina, &a3));
        CREDO_AEQUALIS_I32 (activatio_axes(a3), II);
        CREDO_FALSUM (activatio_truncata(a3));
        CREDO_NECESSE (activatio_creare(ex_punctis, axes, XIII,
            CELLAE_CELLA_D82, V, piscina, &a3));
        CREDO_AEQUALIS_I32 (activatio_axes(a3), V);
        CREDO_VERUM (activatio_truncata(a3));
    }
}

/* refutationes T1: exitus non tanguntur */
interior vacuum
_probare_puncta_refutationes (
    CredoContextus* c)
{
            Piscina* piscina = c->piscina;
          Quaternio  g[IV];
          Quaternio  duo[II];
          Quaternio  axis_peregrinus[I];
          Quaternio  axis_nullus[I];
          Catalogus* cat     = NIHIL;
          Catalogus* exitus  = NIHIL;
          Activatio* act     = NIHIL;
          Quaternio  nullum;
          Quaternio  negatum;
    constans Anulus* anulus = extensio_anulus(extensio_quadratica(II,
        piscina));
    constans Anulus* alius = extensio_anulus(extensio_quadratica(III,
        piscina));

    _generatores(anulus, "a/2", "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_FALSUM (catalogus_ex_punctis(g, ZEPHYRUM, piscina, &exitus));
    /* q et -q: repetitum ad signum */
    CREDO_NECESSE (quaternio_nullum(anulus, piscina, &nullum));
    CREDO_NECESSE (quaternio_subtrahe(nullum, g[ZEPHYRUM], piscina,
        &negatum));
    duo[ZEPHYRUM]  = g[ZEPHYRUM];
    duo[I]         = negatum;
    CREDO_FALSUM (catalogus_ex_punctis(duo, II, piscina, &exitus));
    duo[I] = g[ZEPHYRUM];
    CREDO_FALSUM (catalogus_ex_punctis(duo, II, piscina, &exitus));
    duo[I] = nullum;
    CREDO_FALSUM (catalogus_ex_punctis(duo, II, piscina, &exitus));
    duo[I] = _q(alius, "[1, 0, 0, 0]", piscina);
    CREDO_FALSUM (catalogus_ex_punctis(duo, II, piscina, &exitus));
    CREDO_NIHIL (exitus);
    /* activatio: axis peregrinus, axis sine parte vectoria, numerus 0,
     * cella nondum facta */
    axis_peregrinus[ZEPHYRUM]  = _q(alius, "[0, 1, 0, 0]", piscina);
    axis_nullus[ZEPHYRUM]      = _q(anulus, "[1, 0, 0, 0]", piscina);
    CREDO_FALSUM (activatio_creare(cat, axis_peregrinus, I,
        CELLAE_CELLA_D82, ZEPHYRUM, piscina, &act));
    CREDO_FALSUM (activatio_creare(cat, axis_nullus, I,
        CELLAE_CELLA_D82,
        ZEPHYRUM, piscina, &act));
    CREDO_FALSUM (activatio_creare(cat, axis_nullus, ZEPHYRUM,
        CELLAE_CELLA_D82, ZEPHYRUM, piscina, &act));
    CREDO_FALSUM (activatio_creare(cat, NIHIL, ZEPHYRUM, VII, ZEPHYRUM,
        piscina, &act));
    CREDO_NIHIL (act);
}

/* cellae II T2: cella phasis (D94) contra oraculum ALIA VIA: signum
 * (a/|s| - cos(m pi/k)) per quadrata in Z[sqrt2, sqrt3, sqrt6] (surdus,
 * ut D120), sector = numerus terminorum cum signo <= 0 (definitio D119
 * ipsa, sine reflexione nec radicibus S_k); axes per oraculum integrum
 * Z[sqrt2] supra. */

/* 4 cos(j pi/12) in Z[sqrt2, sqrt3] (bitum 1 sqrt2, 2 sqrt3, 3 sqrt6) */
interior Surdus
_o_cos_quater (
    s32 j)
{
    Surdus r        = surdus_ex_s64(0);
       b32 negatum  = FALSUM;
       i32 s;

    j %= XXIV;
    si (j < 0) j += XXIV;
    si (j > XII) j = XXIV - j;
    si (j > VI)
    { j = XII - j; negatum = VERUM;
    }
    si (j == 0) r.c[0] = 4;
    si (j == 1)
    { r.c[3] = 1; r.c[1] = 1;
    }
    si (j == 2) r.c[2] = 2;
    si (j == 3) r.c[1] = 2;
    si (j == 4) r.c[0] = 2;
    si (j == 5)
    { r.c[3] = 1; r.c[1] = -1;
    }
    si (negatum)
    {
        per (s = 0; s < VIII; s++) r.c[s] = -r.c[s];
    }
    redde r;
}

/* signum(a/|s| - cos(m pi/k)); A = scala a, N = scala^2 |s|^2 */
interior s32
_o_terminus_phasis (
    constans SurdiSpatium* sp,
                   Surdus  a_surdus,
                   Surdus  n_surdus,
                      s32  signum_a,
                      i32  m,
                      i32  k)
{
       s32 sb = (k > 2 * m) - (k < 2 * m);
    Surdus cc;
    Surdus sinistra;
    Surdus dextra;
    Surdus d;
       s32 r = ZEPHYRUM;

    si (sb == 0) redde signum_a;
    si (signum_a == 0) redde -sb;
    si (signum_a != sb) redde signum_a;
    /* 8 cos^2(m pi/k) = 4 + 4 cos(j pi/12), j = 24 m/k */
    cc       = _o_cos_quater((s32)(XXIV * m / k));
    cc.c[0]  += 4;
    si (   !surdus_multiplica(sp, a_surdus, a_surdus, &sinistra)
        || !surdus_scala(sinistra, 8, &sinistra)
        || !surdus_multiplica(sp, cc, n_surdus, &dextra)
        || !surdus_subtrahe(sinistra, dextra, &d)
        || !surdus_signum(sp, d, &r))
    {
        o_vitium = VERUM;
        redde 0;
    }
    redde signum_a * r;
}

/* axes oraculi (ut _o_cella): regula prima maxima, paritas omnes */
interior vacuum
_o_axes_summae (
    Binomium* s,
         i32* axes,
         i32* numerus)
{
    Binomium bn = binomium(-1, 0);
    Binomium bd = binomium(1, 0);
         i32 j;

    *numerus = 0;
    per (j = ZEPHYRUM; j < o_nd; j++)
    {
        Binomium scalare = binomium_adde(binomium_adde(
            binomium_multiplica(s[1], o_axis[j][0]),
            binomium_multiplica(s[2], o_axis[j][1])),
            binomium_multiplica(s[3], o_axis[j][2]));
        Binomium num = binomium_multiplica(scalare, scalare);
             s32 c = *numerus == 0 ? I
                 : binomium_signum(binomium_subtrahe(binomium_multiplica(
                 num, bd), binomium_multiplica(bn, o_n2[j])));

        si (c > 0)
        { bn = num; bd = o_n2[j]; *numerus = 0;
        }
        si (c >= 0) axes[(*numerus)++] = j;
    }
}

interior b32
_o_cella_phasis (
    constans SurdiSpatium* sp,
                 Binomium* s,
                      i32  k,
                      i32* regula,
                      i32* ligatae,
                      i32* n)
{
       i32 nv = o_nd + I;
       b32 v0 = binomium_nullum(s[1]) && binomium_nullum(s[2])
           && binomium_nullum(s[3]);
       i32 axes[O_MAX];
       i32 numerus_axium_o  = ZEPHYRUM;
       i32 sector           = ZEPHYRUM;
       i32 par              = ZEPHYRUM;
       i32 m;
       i32 j;
    Surdus a_surdus;
    Surdus n_surdus = surdus_ex_s64(0);
    Surdus t;
       s32 signum_a = ZEPHYRUM;

    *n = ZEPHYRUM;
    si (v0 && binomium_nullum(s[0]))
    {
        *regula          = (k - I) * nv + o_nd;
        ligatae[(*n)++]  = *regula;
        redde VERUM;
    }
    a_surdus       = surdus_ex_s64(s[0].p);
    a_surdus.c[1]  = s[0].q;
    per (j = ZEPHYRUM; j < IV; j++)
    {
        Surdus x = surdus_ex_s64(s[j].p);

        x.c[1] = s[j].q;
        si (   !surdus_multiplica(sp, x, x, &t)
            || !surdus_adde(n_surdus, t, &n_surdus))
        {
            redde FALSUM;
        }
    }
    si (!surdus_signum(sp, a_surdus, &signum_a))
    {
        redde FALSUM;
    }
    per (m = I; m < k; m++)
    {
        s32 c = _o_terminus_phasis(sp, a_surdus, n_surdus, signum_a, m,
            k);

        si (c <= 0) sector = m;
        si (c == 0) par = m;
    }
    si (v0)
    {
        axes[0]          = o_nd;
        numerus_axium_o  = I;
    } alioquin
    {
        _o_axes_summae(s, axes, &numerus_axium_o);
    }
    *regula = sector * nv + axes[0];
    per (j = 0; j < numerus_axium_o; j++)
    {
        si (par > 0)
        {
            ligatae[(*n)++] = (par - I) * nv + axes[j];
        }
        ligatae[(*n)++] = (par > 0 ? par : sector) * nv + axes[j];
    }
    redde !o_vitium;
}

#define PHASIS_N_MAXIMUS IV

interior vacuum
_probare_cellas_phasis (
    CredoContextus* c)
{
                    Piscina* piscina = c->piscina;
                  Quaternio  g[IV];
                  Catalogus* cat = NIHIL;
                  Activatio* act = NIHIL;
               SurdiSpatium  sp;
                        s32  primi[III];
                        i32  idx[PHASIS_N_MAXIMUS];
                        i32  n;
                        i32  i;
    /* k | XXIV (oraculum cos(j pi/12)); k {1, 2, 3, 4, 8} solum n <=
     * III (recensio L2: k impares a < 0 et reales negativae olim non
     * probatae) */
    hic_manens constans i32 SCALA_PHASIS[VIII] = { VI, XII, XXIV, I,
        II,
                                                   III, IV, VIII };
                        i64 comparatae                   = 0;
                        i64 discordes                    = 0;
                        i64 ligatae_sectionis_negativae  = 0;
                        i64 ligatae_axis                 = 0;
                        i64 reales_negativae             = 0;
                        i64 nullae                       = 0;
                        i64 impares_negativae            = 0;
                        i64 impares_reales               = 0;
                        i32 n_cat;

    primi[0] = II; primi[1] = III; primi[2] = V;
    CREDO_NECESSE (surdi_spatium(primi, III, &sp));
    o_d       = II;
    o_scala   = II;
    o_vitium  = FALSUM;
    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2", "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_creare(cat, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, piscina, &act));
    _o_axes(cat, piscina);
    CREDO_AEQUALIS_I32 (activatio_axes(act), o_nd);
    n_cat = catalogus_numerus(cat);
    per (n = I; n <= PHASIS_N_MAXIMUS; n++)
    {
        per (i = 0; i < n; i++) idx[i] = i;
        fac
        {
            i32 larva;

            per (larva = 0; larva < (i32)(I << n); larva++)
            {
                Quaternio summa;
                 Binomium s[IV];
                      i32 j;
                      i32 t;

                (vacuum)quaternio_nullum(quaternio_anulus(g[0]),
                    piscina,
                    &summa);
                per (t = 0; t < IV; t++) s[t] = binomium(0, 0);
                per (j = 0; j < n; j++)
                {
                    Quaternio e     = catalogus_elementum(cat, idx[j]);
                          b32 plus  = (larva >> j) & I;

                    si (plus) (vacuum)quaternio_adde(summa, e, piscina,
                                  &summa);
                    alioquin (vacuum)quaternio_subtrahe(summa, e,
                                 piscina,
                                 &summa);
                    per (t = 0; t < IV; t++)
                    {
                        Binomium x = _binomium_ex(e, t, piscina);

                        s[t] = plus ? binomium_adde(s[t], x)
                            : binomium_subtrahe(s[t], x);
                    }
                }
                per (t = 0; t < (n <= III ? VIII : III); t++)
                {
                    Cella cl;
                      i32 regula_o = 0;
                      i32 ligatae_o[2 * O_MAX];
                      i32 n_o = 0;
                      i32 ligatae_l[2 * O_MAX];
                      i32 q;
                      b32 aequales;

                    si (   !activatio_cella(act, summa, SCALA_PHASIS[t],
                        &cl)
                        || !_o_cella_phasis(&sp, s, SCALA_PHASIS[t],
                        &regula_o,
                            ligatae_o, &n_o))
                    {
                        discordes++;
                        perge;
                    }
                    per (q = 0; q < cl.numerus; q++) ligatae_l[q] =
                                                         cl.ligatae[q];
                    _ordina(ligatae_l, cl.numerus);
                    _ordina(ligatae_o, n_o);
                    aequales = (i32)cl.regula == regula_o
                        && (i32)cl.numerus == n_o;
                    per (q = 0; aequales
                        && q < n_o; q++) aequales = ligatae_l[q]
                                             == ligatae_o[q];
                    si (!aequales) discordes++;
                    comparatae++;
                    /* regimina: paritas sectoris cum a < 0, axis */
                    {
                        b32 v0 = binomium_nullum(s[1])
                            && binomium_nullum(s[2])
                            && binomium_nullum(s[3]);

                        b32 impar = SCALA_PHASIS[t] % II == I;

                        si (v0 && binomium_nullum(s[0])) nullae++;
                        alioquin si (   v0
                                     && binomium_signum(s[0]) < 0)
                        {
                            reales_negativae++;
                            si (impar) impares_reales++;
                        }
                        alioquin si (!v0)
                        {
                            i32 axes_n[O_MAX];
                            i32 na = 0;

                            _o_axes_summae(s, axes_n, &na);
                            si (na > I) ligatae_axis++;
                            si (   binomium_signum(s[0]) < 0
                                && n_o
                                    > na) ligatae_sectionis_negativae++;
                            si (   binomium_signum(s[0]) < 0
                                && impar) impares_negativae++;
                        }
                    }
                }
            }
            /* combinatio sequens */
            i = n;
            dum (i > 0 && idx[i - 1] == n_cat - n + i - 1) i--;
            si (i == 0) frange;
            idx[i - 1]++;
            per (; i < n; i++) idx[i] = idx[i - 1] + 1;
        } dum (VERUM);
    }
    imprimere("    cellae phasis contra oraculum: %ld (discordes %ld); "
        "paritas sectoris a < 0: %ld, paritas axis: %ld, reales "
        "negativae: %ld, nullae: %ld; k impar: a < 0 %ld, reales "
        "negativae %ld\n", (long)comparatae,
        (long)discordes, (long)ligatae_sectionis_negativae,
        (long)ligatae_axis, (long)reales_negativae, (long)nullae,
        (long)impares_negativae, (long)impares_reales);
    CREDO_FALSUM (o_vitium);
    CREDO_AEQUALIS_S64 ((s64)discordes, (s64)0);
    CREDO_VERUM (comparatae > (L * M));
    CREDO_VERUM (ligatae_sectionis_negativae > C);
    CREDO_VERUM (ligatae_axis > C);
    CREDO_VERUM (reales_negativae > ZEPHYRUM);
    CREDO_VERUM (nullae > ZEPHYRUM);
    CREDO_VERUM (impares_negativae > C);
    CREDO_VERUM (impares_reales > ZEPHYRUM);
}

/* cellae phasis manu (D120 Pars A) et speculum; s et -s; refutationes
 * quaestionum antipodalium */
interior vacuum
_probare_cellas_phasis_manu (
    CredoContextus* c)
{
       Piscina* piscina = c->piscina;
     Quaternio  g[IV];
     Catalogus* cat        = NIHIL;
     Activatio* phasis     = NIHIL;
     Activatio* combinata  = NIHIL;
     Quaternio  sigma;
     Quaternio  minus_sigma;
     Quaternio  minus_unum;
     Quaternio  nullum;
         Cella  cl;
         Cella  cella_altera;
           i32  nv;
           i32  pondera[III];
           b32  verdictum = FALSUM;
      Quaestio  q;
     Capacitas  cap;

    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2", "a/2", "-a/2", piscina, g);
    CREDO_NECESSE (catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI,
        piscina, &cat));
    CREDO_NECESSE (activatio_creare(cat, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, piscina, &phasis));
    CREDO_NECESSE (activatio_ex_catalogo(cat, ZEPHYRUM, piscina,
        &combinata));
    nv     = activatio_axes(phasis) + I;
    sigma  = catalogus_elementum(cat, I);    /* (sqrt2/2)(1 + i) */
    CREDO_NECESSE (quaternio_nullum(quaternio_anulus(sigma), piscina,
        &nullum));
    CREDO_NECESSE (quaternio_subtrahe(nullum, sigma, piscina,
        &minus_sigma));
    CREDO_NECESSE (quaternio_subtrahe(nullum, catalogus_elementum(cat,
        ZEPHYRUM), piscina, &minus_unum));
    /* D120: sigma_1 in termino k = 12, m = 3: sector 3, paritas {2, 3};
     * k = 6: sector 1, sine paritate */
    CREDO_NECESSE (activatio_cella(phasis, sigma, XII, &cl));
    CREDO_AEQUALIS_I32 (cl.regula / nv, III);
    CREDO_AEQUALIS_I32 (cl.numerus, II);
    CREDO_NECESSE (activatio_cella(phasis, sigma, VI, &cl));
    CREDO_AEQUALIS_I32 (cl.regula / nv, I);
    CREDO_AEQUALIS_I32 (cl.numerus, I);
    /* -sigma_1: h = 3 pi/4, in termino m = 9 (k = 12): sector 9, paritas
     * {8, 9}; k = 6: h k/pi = 4.5, sector 4 */
    CREDO_NECESSE (activatio_cella(phasis, minus_sigma, XII, &cl));
    CREDO_AEQUALIS_I32 (cl.regula / nv, IX);
    CREDO_AEQUALIS_I32 (cl.numerus, II);
    CREDO_AEQUALIS_I32 (cl.ligatae[0] / nv + cl.ligatae[1] / nv,
        XVII);
    CREDO_NECESSE (activatio_cella(phasis, minus_sigma, VI, &cl));
    CREDO_AEQUALIS_I32 (cl.regula / nv, IV);
    /* k impar (recensio L2): sigma h k/pi = 0.75 (k = 3), 1.25 (k =
     * 5); -sigma 2.25, 3.75 - sector floor, non (k + 1)/2 + ... */
    CREDO_NECESSE (activatio_cella(phasis, sigma, III, &cl));
    CREDO_AEQUALIS_I32 (cl.regula / nv, ZEPHYRUM);
    CREDO_NECESSE (activatio_cella(phasis, sigma, V, &cl));
    CREDO_AEQUALIS_I32 (cl.regula / nv, I);
    CREDO_NECESSE (activatio_cella(phasis, minus_sigma, III, &cl));
    CREDO_AEQUALIS_I32 (cl.regula / nv, II);
    CREDO_AEQUALIS_I32 (cl.numerus, I);
    CREDO_NECESSE (activatio_cella(phasis, minus_sigma, V, &cl));
    CREDO_AEQUALIS_I32 (cl.regula / nv, III);
    /* D82 plicat: sigma et -sigma eandem cellam; D94 non */
    CREDO_NECESSE (activatio_cella(combinata, sigma, XII, &cl));
    CREDO_NECESSE (activatio_cella(combinata, minus_sigma, XII,
        &cella_altera));
    CREDO_AEQUALIS_I32 (cl.regula, cella_altera.regula);
    CREDO_NECESSE (activatio_cella(phasis, sigma, XII, &cl));
    CREDO_NECESSE (activatio_cella(phasis, minus_sigma, XII,
        &cella_altera));
    CREDO_VERUM (cl.regula != cella_altera.regula);
    /* summa realis: +1 cella nd; -1 = cella summae nullae */
    CREDO_NECESSE (activatio_cella(phasis, catalogus_elementum(cat,
        ZEPHYRUM), XII, &cl));
    CREDO_AEQUALIS_I32 (cl.regula, nv - I);
    CREDO_NECESSE (activatio_cella(phasis, minus_unum, XII, &cl));
    CREDO_AEQUALIS_I32 (cl.regula, XI * nv + nv - I);
    CREDO_NECESSE (activatio_cella(phasis, nullum, XII, &cella_altera));
    CREDO_AEQUALIS_I32 (cl.regula, cella_altera.regula);
    /* h = pi etiam ad k imparem: sector k - 1 */
    CREDO_NECESSE (activatio_cella(phasis, minus_unum, III, &cl));
    CREDO_AEQUALIS_I32 (cl.regula, II * nv + nv - I);
    CREDO_NECESSE (activatio_cella(phasis, minus_unum, V, &cl));
    CREDO_AEQUALIS_I32 (cl.regula, IV * nv + nv - I);
    /* quaestiones antipodales: cella D82 sola */
    pondera[0] = I; pondera[1] = II; pondera[2] = III;
    CREDO_FALSUM (activatio_separat(phasis, pondera, III, VI,
        CELLAE_REGULA, &verdictum));
    _quaestio(&q, CELLAE_REGULA);
    CREDO_FALSUM (activatio_exhaustiva(phasis, &q, piscina, &cap));
    CREDO_FALSUM (activatio_avida(phasis, &q, piscina, &cap));
}


/* ==================================================
 * cellae II T3: encodatio signata (activatio_iudica) contra tabulas
 * exactas D119 congelatas (knotapel/demo_120_surd_capacity/main.c:
 * D119_B zeta_8, D119_C 2I primi XXIV). Columnae: copiae, fluitans (non
 * hic), regula, robusta, possibilis, ligatae; ordines per n = III..VI:
 * XOR, AND, MAJ. Verdicta per k {6, 12, 24} unita; D119 nullam copiam
 * indecisam habet.
 * ================================================== */

hic_manens constans i64 D119_ZETA[XII][VI] = {
    { 2024, 1480, 1456, 1456, 1938, 1217 },
    { 2024, 1907, 1799, 1799, 1970, 1217 },
    { 2024, 1494, 1462, 1462, 1968, 1217 },
    { 10626, 8010, 7908, 7696, 8678, 6770 },
    { 10626, 9723, 9486, 9402, 10046, 6770 },
    { 10626, 7156, 6797, 6611, 8666, 6770 },
    { 42504, 17201, 16479, 15497, 24714, 30822 },
    { 42504, 37835, 37309, 36707, 39072, 30822 },
    { 42504, 18368, 17282, 16514, 25625, 30822 },
    { 134596, 12983, 12268, 10070, 22979, 107035 },
    { 134596, 111290, 110495, 107663, 116323, 107035 },
    { 134596, 10031, 9635, 7911, 18245, 107035 } };
hic_manens constans i64 D119_AUREA[XII][VI] = {
    { 2024, 1580, 1580, 1580, 1960, 1181 },
    { 2024, 1930, 1930, 1930, 1976, 1181 },
    { 2024, 1596, 1596, 1596, 1994, 1181 },
    { 10626, 9114, 9030, 8958, 9399, 7225 },
    { 10626, 10055, 10050, 9983, 10252, 7225 },
    { 10626, 8089, 8031, 7953, 9408, 7225 },
    { 42504, 20805, 20453, 19693, 27983, 33885 },
    { 42504, 38688, 38702, 38044, 39557, 33885 },
    { 42504, 24333, 24022, 23499, 31738, 33885 },
    { 134596, 21679, 20936, 17034, 28850, 117888 },
    { 134596, 112556, 112469, 109058, 116439, 117888 },
    { 134596, 21237, 20998, 17547, 28484, 117888 } };
hic_manens constans i32 SCALA_D119[III] = { VI, XII, XXIV };
/* ordo inversus: k ultimum (VI) non subtilissimum - unio per k ab
 * ordine independens esse debet (in { VI, XII, XXIV } cellae XXIV
 * ceteras subdividunt, ergo regula ad k ultimum = unio: planta
 * 'ultimum k solum' ibi muta) */
hic_manens constans i32 SCALA_INVERSA[III] = { XXIV, XII, VI };
/* scalae k imparium et mixtae (recensio L2) */
hic_manens constans i32 SCALA_IMPARIS[II] = { III, V };
hic_manens constans i32 SCALA_SCALARIS[III] = { VII, II, I };

interior b32
_iudicia_aequalia (
    constans Iudicium* x,
    constans Iudicium* y)
{
    redde x->regula == y->regula && x->robusta == y->robusta
        && x->possibilis == y->possibilis && x->ligata == y->ligata
        && x->indecisa == y->indecisa && x->concursus == y->concursus;
}

/* zeta_8: XXIV elementa (ut sectio zeta_8) */
interior Catalogus*
_zeta_octo_catalogus (
    Piscina* piscina)
{
     Quaternio  g[IV];
     Catalogus* cat = NIHIL;

    _generatores(extensio_anulus(extensio_quadratica(II, piscina)),
        "a/2", "a/2", "-a/2", piscina, g);
    si (!catalogus_clausura(g, TORSIONES, IV, ZEPHYRUM, XVI, piscina,
        &cat))
    {
        redde NIHIL;
    }
    redde cat;
}

/* 2I: generatores (s, s*, t, t*) ordine D94/D120 build_2i, clausura
 * eadem BFS (1, generatores, deinde producta dextra; +-q idem, ut
 * find_2i: LX puncta); primi XXIV */
interior Catalogus*
_aurea_prima (
    Piscina* piscina)
{
         constans Anulus* q5 = extensio_anulus(extensio_quadratica(V,
             piscina));
                  Quaternio  g[IV];
                  Catalogus* plenus  = NIHIL;
                  Catalogus* pars    = NIHIL;
                        i32  indices[XXIV];
                        i32  i;
    hic_manens constans s32  NULLAE[IV] = { 0, 0, 0, 0 };

    g[ZEPHYRUM]  = _q(q5, "[1/2, 1/2, 1/2, 1/2]", piscina);
    g[I]         = _q(q5, "[1/2, -1/2, -1/2, -1/2]", piscina);
    g[II]        = _q(q5, "[(a + 1)/4, (a - 1)/4, 1/2, 0]", piscina);
    g[III]       = _q(q5, "[(a + 1)/4, (1 - a)/4, -1/2, 0]", piscina);
    si (   !catalogus_clausura(g, NULLAE, IV, ZEPHYRUM, ZEPHYRUM,
            piscina, &plenus)
        || catalogus_numerus(plenus) != LX)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < XXIV; i++)
    {
        indices[i] = i;
    }
    si (!catalogus_pars(plenus, indices, XXIV, piscina, &pars))
    {
        redde NIHIL;
    }
    redde pars;
}

/* oraculum concursus (via altera): 2^n summae per quaternio_adde/
 * _subtrahe, paria valoris diversi exacte aequalia (D94: summa eadem;
 * D82 etiam opposita) */
interior b32
_o_concursus (
    constans Catalogus* cat,
          constans i32* pondera,
                   i32  n,
           constans i8* tabula,
                   b32  plicata,
               Piscina* piscina,
                   b32* exitus)
{
    Quaternio summae[CCLVI];
          i32 numerus = (i32)(I << n);
          i32 v;
          i32 w;
          i32 j;

    *exitus = FALSUM;
    per (v = ZEPHYRUM; v < numerus; v++)
    {
        si (!quaternio_nullum(quaternio_anulus(catalogus_elementum(cat,
            ZEPHYRUM)), piscina, &summae[v]))
        {
            redde FALSUM;
        }
        per (j = ZEPHYRUM; j < n; j++)
        {
            Quaternio q = catalogus_elementum(cat, pondera[j]);

            si (!(((v >> j) & I)
                ? quaternio_adde(summae[v], q, piscina, &summae[v])
                : quaternio_subtrahe(summae[v], q, piscina,
                &summae[v])))
            {
                redde FALSUM;
            }
        }
    }
    per (v = ZEPHYRUM; v < numerus; v++)
    {
        per (w = v + I; w < numerus; w++)
        {
            Quaternio summa;

            si (tabula[v] == tabula[w])
            {
                perge;
            }
            si (quaternio_aequalis(summae[v], summae[w]))
            {
                *exitus = VERUM;
                redde VERUM;
            }
            si (!plicata)
            {
                perge;
            }
            si (!quaternio_adde(summae[v], summae[w], piscina, &summa))
            {
                redde FALSUM;
            }
            si (quaternio_est_nullum(summa))
            {
                *exitus = VERUM;
                redde VERUM;
            }
        }
    }
    redde VERUM;
}

nomen structura {
    i64 copiae;
    i64 regula;
    i64 robusta;
    i64 possibilis;
    i64 ligata;
    i64 indecisa;
    i64 concursus;
    i64 concursus_oraculi;    /* oraculo (si adhibitum) */
    i64 discrepantiae;        /* concursus != oraculum */
    i64 contradictiones;      /* concursus cum verdicto vero */
} NumeriIudicii;

/* copia proxima n ponderum ex [0, numerus) ordine lexicographico
 * (D119); FALSUM post ultimam */
interior b32
_copia_proxima (
     i32* pondera,
     i32  n,
     i32  numerus)
{
    i32 j = n;
    i32 l;

    dum (j > ZEPHYRUM && pondera[j - I] == numerus - n + j - I)
    {
        j--;
    }
    si (j == ZEPHYRUM)
    {
        redde FALSUM;
    }
    pondera[j - I]++;
    per (l = j; l < n; l++)
    {
        pondera[l] = pondera[l - I] + I;
    }
    redde VERUM;
}

/* omnes copiae n ponderum ex [0, numerus) ordine lexicographico (D119);
 * 'oraculum' (non NIHIL) = catalogus pro _o_concursus */
interior b32
_iudicare_omnes (
             Activatio* act,
                   i32  numerus,
                   i32  n,
           constans i8* tabula,
          constans i32* scala,
    constans Catalogus* oraculum,
               Piscina* piscina,
         NumeriIudicii* x)
{
    i32 pondera[VII];
    i32 j;

    memset(x, ZEPHYRUM, magnitudo(*x));
    per (j = ZEPHYRUM; j < n; j++)
    {
        pondera[j] = j;
    }
    fac
    {
        Iudicium iu;

        si (!activatio_iudica(act, pondera, n, scala, III, tabula, &iu))
        {
            redde FALSUM;
        }
        x->copiae++;
        x->regula      += iu.regula ? I : ZEPHYRUM;
        x->robusta     += iu.robusta ? I : ZEPHYRUM;
        x->possibilis  += iu.possibilis ? I : ZEPHYRUM;
        x->ligata      += iu.ligata ? I : ZEPHYRUM;
        x->indecisa    += iu.indecisa ? I : ZEPHYRUM;
        x->concursus   += iu.concursus ? I : ZEPHYRUM;
        si (iu.concursus && (iu.regula || iu.robusta || iu.possibilis))
        {
            x->contradictiones++;
        }
        si (oraculum != NIHIL)
        {
            PiscinaNotatio nota   = piscina_notare(piscina);
                       b32 verum  = FALSUM;

            si (!_o_concursus(oraculum, pondera, n, tabula, FALSUM,
                piscina, &verum))
            {
                redde FALSUM;
            }
            piscina_reficere(piscina, nota);
            x->concursus_oraculi  += verum ? I : ZEPHYRUM;
            x->discrepantiae      += verum != iu.concursus ? I
                : ZEPHYRUM;
        }
    } dum (_copia_proxima(pondera, n, numerus));
    redde VERUM;
}

interior constans i8*
_tabula_functionis (
         i32  n,
         i32  functio,
     Piscina* piscina)
{
    redde functio == ZEPHYRUM ? cellae_paritas(n, piscina)
        : functio == I ? cellae_et(n, piscina) : cellae_maior(n,
        piscina);
}

/* ordines tabulae D119 n_infra..n_supra contra activatio_iudica */
interior vacuum
_contra_d119 (
    CredoContextus* c,
         Activatio* act,
    constans i64 (*tabula)[VI],
       constans i32* scala,
         Catalogus* oraculum,
                i32 n_infra,
                i32 n_supra)
{
    i32 n;
    i32 functio;

    per (n = n_infra; n <= n_supra; n++)
    {
        per (functio = ZEPHYRUM; functio < III; functio++)
        {
             constans i64* ordo = tabula[(n - III) * III + functio];
              constans i8* tv = _tabula_functionis(n, functio,
                  c->piscina);
            NumeriIudicii x;

            CREDO_NECESSE (tv != NIHIL);
            CREDO_NECESSE (_iudicare_omnes(act, XXIV, n, tv, scala,
                oraculum, c->piscina, &x));
            CREDO_AEQUALIS_S64 ((s64)x.copiae, (s64)ordo[ZEPHYRUM]);
            CREDO_AEQUALIS_S64 ((s64)x.regula, (s64)ordo[II]);
            CREDO_AEQUALIS_S64 ((s64)x.robusta, (s64)ordo[III]);
            CREDO_AEQUALIS_S64 ((s64)x.possibilis, (s64)ordo[IV]);
            CREDO_AEQUALIS_S64 ((s64)x.ligata, (s64)ordo[V]);
            CREDO_AEQUALIS_S64 ((s64)x.indecisa, ZEPHYRUM);
            CREDO_AEQUALIS_S64 ((s64)x.contradictiones, ZEPHYRUM);
            /* concursus <= copiae sine possibili */
            CREDO_VERUM (x.concursus <= x.copiae - x.possibilis);
            si (oraculum != NIHIL)
            {
                CREDO_AEQUALIS_S64 ((s64)x.discrepantiae, ZEPHYRUM);
                CREDO_AEQUALIS_S64 ((s64)x.concursus,
                    (s64)x.concursus_oraculi);
            }
        }
    }
}

interior vacuum
_probare_iudicium_d119 (
    CredoContextus* c)
{
    Catalogus* zeta   = _zeta_octo_catalogus(c->piscina);
    Catalogus* aurea  = _aurea_prima(c->piscina);
    Activatio* act    = NIHIL;

    CREDO_NECESSE_NON_NIHIL (zeta);
    CREDO_NECESSE_NON_NIHIL (aurea);
    CREDO_AEQUALIS_I32 (catalogus_numerus(zeta), XXIV);
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
    CREDO_AEQUALIS_I32 (activatio_axes(act), XIII);
    _contra_d119(c, act, D119_ZETA, SCALA_D119, zeta, III, IV);
    _contra_d119(c, act, D119_ZETA, SCALA_INVERSA, NIHIL, III, III);
    CREDO_NECESSE (activatio_creare(aurea, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
    CREDO_AEQUALIS_I32 (activatio_axes(act), XII);
    _contra_d119(c, act, D119_AUREA, SCALA_D119, aurea, III, IV);
    _contra_d119(c, act, D119_AUREA, SCALA_INVERSA, NIHIL, III, III);
}

/* n = V, VI (CLXXVII milia copiarum per catalogum et functionem) */
interior vacuum
_probare_iudicium_d119_lentum (
    CredoContextus* c)
{
    Catalogus* zeta   = _zeta_octo_catalogus(c->piscina);
    Catalogus* aurea  = _aurea_prima(c->piscina);
    Activatio* act    = NIHIL;

    CREDO_NECESSE_NON_NIHIL (zeta);
    CREDO_NECESSE_NON_NIHIL (aurea);
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
    _contra_d119(c, act, D119_ZETA, SCALA_D119, NIHIL, V, VI);
    CREDO_NECESSE (activatio_creare(aurea, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
    _contra_d119(c, act, D119_AUREA, SCALA_D119, NIHIL, V, VI);
}

/* filtrum sine iactura: Iudicium idem cum filtro et sine, copia per
 * copiam (n = III omnes, n = IV omnis quinta, n = VIII omnis 36000-a:
 * XXI copiae per C(24, 8) sparsae), XOR et MAJ; n = VIII etiam
 * concursus contra oraculum */
interior vacuum
_probare_iudicium_filtrum (
    CredoContextus* c)
{
                  Catalogus* zeta  = _zeta_octo_catalogus(c->piscina);
                  Activatio* cum   = NIHIL;
                  Activatio* sine  = NIHIL;
                        i32  n;
                        i64  comparatae        = ZEPHYRUM;
                        i64  diversae          = ZEPHYRUM;
                        i64  oraculo_diversae  = ZEPHYRUM;
    hic_manens constans i32  GRADUS[III]       = { III, IV, VIII };
    hic_manens constans i64  PASSUS[III]       = { 1, 5, 36000 };
                        i32  g;

    CREDO_NECESSE_NON_NIHIL (zeta);
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &cum));
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &sine));
    activatio_filtrum(sine, FALSUM);
    per (g = ZEPHYRUM; g < III; g++)
    {
        i32 functio;

        n = GRADUS[g];

        per (functio = ZEPHYRUM; functio < III; functio += II)
        {
            constans i8* tv = _tabula_functionis(n, functio,
                c->piscina);
                    i32 pondera[VIII];
                    i32 j;
                    i64 ordo = ZEPHYRUM;

            CREDO_NECESSE (tv != NIHIL);
            per (j = ZEPHYRUM; j < n; j++)
            {
                pondera[j] = j;
            }
            fac
            {
                Iudicium x;
                Iudicium y;

                si ((ordo++ % PASSUS[g]) != ZEPHYRUM)
                {
                    perge;
                }
                CREDO_NECESSE (activatio_iudica(cum, pondera, n,
                    SCALA_D119, III, tv, &x));
                CREDO_NECESSE (activatio_iudica(sine, pondera, n,
                    SCALA_D119, III, tv, &y));
                comparatae++;
                si (   x.regula != y.regula || x.robusta != y.robusta
                    || x.possibilis != y.possibilis
                    || x.ligata != y.ligata || x.indecisa != y.indecisa
                    || x.concursus != y.concursus)
                {
                    diversae++;
                }
                /* scalae k imparium (recensio L2): sector a < 0 per
                 * filtrum = exacte */
                si (n <= IV)
                {
                    CREDO_NECESSE (activatio_iudica(cum, pondera, n,
                        SCALA_IMPARIS, II, tv, &x));
                    CREDO_NECESSE (activatio_iudica(sine, pondera, n,
                        SCALA_IMPARIS, II, tv, &y));
                    diversae += _iudicia_aequalia(&x, &y) ? ZEPHYRUM
                        : I;
                    CREDO_NECESSE (activatio_iudica(cum, pondera, n,
                        SCALA_SCALARIS, III, tv, &x));
                    CREDO_NECESSE (activatio_iudica(sine, pondera, n,
                        SCALA_SCALARIS, III, tv, &y));
                    diversae += _iudicia_aequalia(&x, &y) ? ZEPHYRUM
                        : I;
                }
                si (n == VIII)
                {
                    PiscinaNotatio nota   = piscina_notare(c->piscina);
                               b32 verum  = FALSUM;

                    CREDO_NECESSE (_o_concursus(zeta, pondera, n, tv,
                        FALSUM, c->piscina, &verum));
                    piscina_reficere(c->piscina, nota);
                    oraculo_diversae += verum != x.concursus ? I
                        : ZEPHYRUM;
                }
            } dum (_copia_proxima(pondera, n, XXIV));
        }
    }
    CREDO_AEQUALIS_S64 ((s64)diversae, ZEPHYRUM);
    CREDO_AEQUALIS_S64 ((s64)oraculo_diversae, ZEPHYRUM);
    /* III: 2024; IV: ceil(10626/5); VIII: ceil(735471/36000); x 2 */
    CREDO_AEQUALIS_S64 ((s64)comparatae, (s64)((2024 + 2126
        + XXI) * II));
}

/* memoria activationis (cellae II T4): combinationes per vocationem,
 * puncta in activatione. Omnes copiae zeta_8 N = IV (XOR) semel:
 * piscina activationis < V MB (combinationes in activatione: XV MB);
 * iterum eaedem: nihil novi (puncta et cellae in memoria, nihil per
 * vocationem in activatione) */
interior vacuum
_probare_iudicium_memoriam (
    CredoContextus* c)
{
      Catalogus* zeta = _zeta_octo_catalogus(c->piscina);
        Piscina* propria;
      Activatio* act = NIHIL;
    constans i8* tv;
            i32  transitus;
 memoriae_index  usus[III];
            b32  creata;

    CREDO_NECESSE_NON_NIHIL (zeta);
    propria = piscina_generare_dynamicum("probatio_memoriae",
        (memoriae_index)65536);
    CREDO_NECESSE_NON_NIHIL (propria);
    tv = cellae_paritas(IV, c->piscina);
    CREDO_NECESSE_NON_NIHIL (tv);
    /* piscina propria destruenda: CREDO_NECESSE sectionem abrumperet */
    creata = activatio_creare(zeta, NIHIL, ZEPHYRUM, CELLAE_CELLA_D94,
        ZEPHYRUM, propria, &act);
    CREDO_VERUM (creata);
    si (creata)
    {
        usus[ZEPHYRUM] = piscina_summa_usus(propria);
        per (transitus = ZEPHYRUM; transitus < II; transitus++)
        {
            i32 pondera[IV];
            i32 j;
            b32 bene = VERUM;

            per (j = ZEPHYRUM; j < IV; j++)
            {
                pondera[j] = j;
            }
            fac
            {
                Iudicium iu;

                bene = bene && activatio_iudica(act, pondera, IV,
                    SCALA_D119, III, tv, &iu);
            } dum (_copia_proxima(pondera, IV, XXIV));
            CREDO_VERUM (bene);
            usus[transitus + I] = piscina_summa_usus(propria);
        }
        CREDO_VERUM (usus[I] - usus[ZEPHYRUM] < (memoriae_index)V
            * 1048576);
        CREDO_AEQUALIS_S64 ((s64)(usus[II] - usus[I]), ZEPHYRUM);
    }
    piscina_destruere(propria);
}

hic_manens constans i8 XOR_I[II]          = { 0, 1 };
hic_manens constans i8 ET_II[IV]          = { 0, 0, 0, 1 };
hic_manens constans i8 TABULA_MALA[II]    = { 0, 2 };
hic_manens constans i8 PARITAS_III[VIII]  = { 0, 1, 1, 0, 1, 0, 0, 1 };
hic_manens constans i8 MAIOR_III[VIII]    = { 0, 0, 0, 1, 0, 1, 1, 1 };
hic_manens constans i32 SCALA_MALA[II]    = { VI, 0 };

/* casus manu: D82 contra D94 sub encodatione signata, concursus,
 * tabulae veritatis, refutationes */
interior vacuum
_probare_iudicium_manu (
    CredoContextus* c)
{
      Catalogus* zeta             = _zeta_octo_catalogus(c->piscina);
      Catalogus* aurea            = _aurea_prima(c->piscina);
      Activatio* phasis           = NIHIL;
      Activatio* combinata        = NIHIL;
      Activatio* phasis_aurea     = NIHIL;
      Activatio* combinata_aurea  = NIHIL;
            i32  pondera[III];
       Iudicium  iu;
       Iudicium  intactum;
    constans i8* t;
    CREDO_NECESSE_NON_NIHIL (zeta);
    CREDO_NECESSE_NON_NIHIL (aurea);
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &phasis));
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D82, ZEPHYRUM, c->piscina, &combinata));
    CREDO_VERUM (quaternio_aequalis(catalogus_elementum(zeta, ZEPHYRUM),
        _q(quaternio_anulus(catalogus_elementum(zeta, ZEPHYRUM)),
        "[1, 0, 0, 0]", c->piscina)));

    /* n = 1, pondus 1, XOR {-1 -> 0, +1 -> 1}: D94 cella +1 (sector 0)
     * et -1 (cella summae nullae) diversae, regula transit; D82 plicat
     * +-1 in punctum unum: concursus, nihil transit */
    pondera[ZEPHYRUM] = ZEPHYRUM;
    CREDO_NECESSE (activatio_iudica(phasis, pondera, I, SCALA_D119, III,
        XOR_I, &iu));
    CREDO_VERUM (iu.regula && iu.robusta && iu.possibilis);
    CREDO_FALSUM (iu.concursus || iu.indecisa || iu.ligata);
    CREDO_NECESSE (activatio_iudica(combinata, pondera, I, SCALA_D119,
        III, XOR_I, &iu));
    CREDO_FALSUM (iu.regula || iu.robusta || iu.possibilis);
    CREDO_VERUM (iu.concursus);
    CREDO_FALSUM (iu.indecisa);

    /* D82, n impar, XOR: larva complementaria summam oppositam habet
     * (plicatur) et paritatem diversam - concursus semper (pondera 0,
     * 1, 2 zeta_8). D94 eadem copia: summae oppositae cellas diversas.
     */
    pondera[ZEPHYRUM]  = ZEPHYRUM;
    pondera[I]         = I;
    pondera[II]        = II;
    CREDO_NECESSE (activatio_iudica(combinata, pondera, III, SCALA_D119,
        III, cellae_paritas(III, c->piscina), &iu));
    CREDO_VERUM (iu.concursus);
    CREDO_FALSUM (iu.regula || iu.robusta || iu.possibilis
        || iu.indecisa);
    CREDO_NECESSE (activatio_iudica(phasis, pondera, III, SCALA_D119,
        III, cellae_paritas(III, c->piscina), &iu));
    CREDO_FALSUM (iu.concursus);

    /* relatio signata 2I (D126): s + s* = 1, s = (1 + i + j + k)/2.
     * Pondera {1, s, s*}: larva 001 (1 - s - s*) et 110 (-1 + s + s*)
     * ambae summa 0; XOR valores 1 et 0: concursus D94 et D82. AND
     * (valores 0, 0): nullus concursus D94 (relationes {-1,0,1} solae
     * +-(1, -1, -1)). */
    CREDO_VERUM (quaternio_aequalis(catalogus_elementum(aurea, I),
        _q(quaternio_anulus(catalogus_elementum(aurea, ZEPHYRUM)),
        "[1/2, 1/2, 1/2, 1/2]", c->piscina)));
    CREDO_VERUM (quaternio_aequalis(catalogus_elementum(aurea, II),
        _q(quaternio_anulus(catalogus_elementum(aurea, ZEPHYRUM)),
        "[1/2, -1/2, -1/2, -1/2]", c->piscina)));
    CREDO_NECESSE (activatio_creare(aurea, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &phasis_aurea));
    CREDO_NECESSE (activatio_creare(aurea, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D82, ZEPHYRUM, c->piscina, &combinata_aurea));
    CREDO_NECESSE (activatio_iudica(phasis_aurea, pondera, III,
        SCALA_D119, III, cellae_paritas(III, c->piscina), &iu));
    CREDO_VERUM (iu.concursus);
    CREDO_FALSUM (iu.regula || iu.robusta || iu.possibilis
        || iu.indecisa);
    CREDO_NECESSE (activatio_iudica(phasis_aurea, pondera, III,
        SCALA_D119, III, cellae_et(III, c->piscina), &iu));
    CREDO_FALSUM (iu.concursus);
    CREDO_NECESSE (activatio_iudica(combinata_aurea, pondera, III,
        SCALA_D119, III, cellae_paritas(III, c->piscina), &iu));
    CREDO_VERUM (iu.concursus);

    /* tabulae veritatis */
    t = cellae_paritas(III, c->piscina);
    CREDO_NECESSE_NON_NIHIL (t);
    CREDO_VERUM (memcmp(t, PARITAS_III, VIII) == ZEPHYRUM);
    t = cellae_maior(III, c->piscina);
    CREDO_NECESSE_NON_NIHIL (t);
    CREDO_VERUM (memcmp(t, MAIOR_III, VIII) == ZEPHYRUM);
    t = cellae_et(III, c->piscina);
    CREDO_NECESSE_NON_NIHIL (t);
    CREDO_VERUM (t[VII] == I && t[VI] == ZEPHYRUM && t[ZEPHYRUM]
        == ZEPHYRUM);
    t = cellae_maior(IV, c->piscina);
    CREDO_NECESSE_NON_NIHIL (t);
    CREDO_VERUM (t[VII] == I && t[XV] == I && t[XIV] == I
        && t[III] == ZEPHYRUM && t[XII] == ZEPHYRUM);
    CREDO_VERUM (cellae_paritas(ZEPHYRUM, c->piscina) == NIHIL);
    CREDO_VERUM (cellae_et(IX, c->piscina) == NIHIL);
    CREDO_VERUM (cellae_maior(IX, c->piscina) == NIHIL);
    t = cellae_maior(VIII, c->piscina);
    CREDO_NECESSE_NON_NIHIL (t);
    CREDO_VERUM (t[XV] == ZEPHYRUM && t[XXXI] == I && t[CCLV] == I);
    t = cellae_paritas(VII, c->piscina);
    CREDO_NECESSE_NON_NIHIL (t);
    CREDO_VERUM (t[CXXVII] == I && t[CXXVI] == ZEPHYRUM);

    /* refutationes: exitus non tangitur */
    memset(&intactum, 0x5A, magnitudo(intactum));
    iu                 = intactum;
    pondera[ZEPHYRUM]  = ZEPHYRUM;
    pondera[I]         = ZEPHYRUM;
    CREDO_FALSUM (activatio_iudica(phasis, pondera, ZEPHYRUM,
        SCALA_D119,
        III, XOR_I, &iu));
    CREDO_FALSUM (activatio_iudica(phasis, pondera, IX, SCALA_D119,
        III, XOR_I, &iu));
    CREDO_FALSUM (activatio_iudica(phasis, pondera, II, SCALA_D119,
        III, ET_II, &iu));                     /* pondus iteratum */
    pondera[I] = XXIV;
    CREDO_FALSUM (activatio_iudica(phasis, pondera, II, SCALA_D119,
        III, ET_II, &iu));                     /* index extra */
    CREDO_FALSUM (activatio_iudica(phasis, pondera, I, SCALA_MALA, II,
        XOR_I, &iu));                          /* k = 0 */
    CREDO_FALSUM (activatio_iudica(phasis, pondera, I, SCALA_D119,
        ZEPHYRUM, XOR_I, &iu));                /* scala vacua */
    CREDO_FALSUM (activatio_iudica(phasis, pondera, I, NIHIL, III,
        XOR_I, &iu));
    CREDO_FALSUM (activatio_iudica(phasis, pondera, I, SCALA_D119, III,
        TABULA_MALA, &iu));                           /* valor 2 */
    CREDO_FALSUM (activatio_iudica(phasis, pondera, I, SCALA_D119, III,
        NIHIL, &iu));
    CREDO_FALSUM (activatio_iudica(phasis, NIHIL, I, SCALA_D119, III,
        XOR_I, &iu));
    CREDO_VERUM (memcmp(&iu, &intactum, magnitudo(iu)) == ZEPHYRUM);
}

/* imago integra sine iactura (cellae II T4b): Iudicium idem cum
 * imagine et sine, copia per copiam, SINE filtro (omnis summa per
 * punctum): zeta_8 et 2I (D94), zeta_8 (D82); n = III omnes, IV omnis
 * quinta, VIII omnis 36000-a; XOR et MAJ. Statistica: cum imagine
 * nulla summa algebraica, sine imagine nulla per imaginem. */
interior vacuum
_probare_imaginem (
    CredoContextus* c)
{
                  Catalogus* catalogi[III];
                        i64  comparatae  = ZEPHYRUM;
                        i64  diversae    = ZEPHYRUM;
    hic_manens constans i32  CELLAE[III] = { CELLAE_CELLA_D94,
                                             CELLAE_CELLA_D94,
                                             CELLAE_CELLA_D82 };
    hic_manens constans i32 GRADUS[III] = { III, IV, VIII };
    hic_manens constans i64 PASSUS[III] = { 1, 5, 36000 };
                        i32 r;

    catalogi[ZEPHYRUM]  = _zeta_octo_catalogus(c->piscina);
    catalogi[I]         = _aurea_prima(c->piscina);
    catalogi[II]        = catalogi[ZEPHYRUM];
    CREDO_NECESSE_NON_NIHIL (catalogi[ZEPHYRUM]);
    CREDO_NECESSE_NON_NIHIL (catalogi[I]);
    per (r = ZEPHYRUM; r < III; r++)
    {
               Activatio* cum   = NIHIL;
               Activatio* sine  = NIHIL;
        CellaeStatistica  cum_ante;
        CellaeStatistica  cum_post;
        CellaeStatistica  sine_ante;
        CellaeStatistica  sine_post;
                     i32  g;

        CREDO_NECESSE (activatio_creare(catalogi[r], NIHIL, ZEPHYRUM,
            CELLAE[r], ZEPHYRUM, c->piscina, &cum));
        CREDO_NECESSE (activatio_creare(catalogi[r], NIHIL, ZEPHYRUM,
            CELLAE[r], ZEPHYRUM, c->piscina, &sine));
        activatio_filtrum(cum, FALSUM);
        activatio_filtrum(sine, FALSUM);
        activatio_imago(sine, FALSUM);
        activatio_statistica(cum, &cum_ante);
        activatio_statistica(sine, &sine_ante);
        per (g = ZEPHYRUM; g < III; g++)
        {
            i32 n = GRADUS[g];
            i32 functio;

            per (functio = ZEPHYRUM; functio < III; functio += II)
            {
                constans i8* tv = _tabula_functionis(n, functio,
                    c->piscina);
                        i32 pondera[VIII];
                        i32 j;
                        i64 ordo = ZEPHYRUM;

                CREDO_NECESSE (tv != NIHIL);
                per (j = ZEPHYRUM; j < n; j++)
                {
                    pondera[j] = j;
                }
                fac
                {
                    Iudicium x;
                    Iudicium y;

                    si ((ordo++ % PASSUS[g]) != ZEPHYRUM)
                    {
                        perge;
                    }
                    CREDO_NECESSE (activatio_iudica(cum, pondera, n,
                        SCALA_D119, III, tv, &x));
                    CREDO_NECESSE (activatio_iudica(sine, pondera, n,
                        SCALA_D119, III, tv, &y));
                    comparatae++;
                    diversae += _iudicia_aequalia(&x,
                        &y) ? ZEPHYRUM : I;
                } dum (_copia_proxima(pondera, n, XXIV));
            }
        }
        activatio_statistica(cum, &cum_post);
        activatio_statistica(sine, &sine_post);
        CREDO_VERUM (cum_post.imagine > cum_ante.imagine);
        CREDO_AEQUALIS_S64 ((s64)(cum_post.algebraice
            - cum_ante.algebraice), ZEPHYRUM);
        CREDO_AEQUALIS_S64 ((s64)(sine_post.imagine
            - sine_ante.imagine),
            ZEPHYRUM);
        CREDO_VERUM (sine_post.algebraice > sine_ante.algebraice);
    }
    CREDO_AEQUALIS_S64 ((s64)diversae, ZEPHYRUM);
    /* (2024 + ceil(10626/5) + ceil(735471/36000)) x 2 functiones x 3 */
    CREDO_AEQUALIS_S64 ((s64)comparatae, (s64)((2024 + 2126 + XXI) * II
        * III));
}

/* catalogus Q(sqrt 2) quinque punctorum, pars prima puncti primi
 * 'prima' */
interior Catalogus*
_catalogus_magnus (
    constans character* prima,
               Piscina* piscina)
{
    constans Anulus* q2 = extensio_anulus(extensio_quadratica(II,
        piscina));
           Quaternio  puncta[V];
           Catalogus* cat = NIHIL;
           character  textus[CXXVIII];

    sprintf(textus, "[%s, 1, 0, 0]", prima);
    puncta[ZEPHYRUM]  = _q(q2, textus, piscina);
    puncta[I]         = _q(q2, "[1, a, 1, 0]", piscina);
    puncta[II]        = _q(q2, "[0, 1, a, 1]", piscina);
    puncta[III]       = _q(q2, "[1, 0, 1, a]", piscina);
    puncta[IV]        = _q(q2, "[a, 1, 0, 1]", piscina);
    si (!catalogus_ex_punctis(puncta, V, piscina, &cat))
    {
        redde NIHIL;
    }
    redde cat;
}

/* limes imaginis: coefficiens 2^40 accipitur; 2^40 + 1 et D = 3^40
 * (extra s64) imaginem non habent - via algebraica, nihil tacite.
 * Verdicta eadem cum imagine et sine (omnes C(5, 3) copiae, XOR et
 * MAJ, sine filtro). */
interior vacuum
_probare_imaginis_limites (
    CredoContextus* c)
{
    hic_manens constans character* PRIMAE[III] = {
        "1099511627776",               /* 2^40: imago */
        "1099511627777",               /* 2^40 + 1: nulla */
        "1/12157665459056928801"       /* 3^40 > 2^63: nulla */
    };
    i32 r;

    per (r = ZEPHYRUM; r < III; r++)
    {
              Catalogus* cat = _catalogus_magnus(PRIMAE[r],
                  c->piscina);
               Activatio* cum   = NIHIL;
               Activatio* sine  = NIHIL;
        CellaeStatistica  st;
                     i32  functio;
                     i64  diversae = ZEPHYRUM;

        CREDO_NECESSE_NON_NIHIL (cat);
        CREDO_NECESSE (activatio_creare(cat, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &cum));
        CREDO_NECESSE (activatio_creare(cat, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &sine));
        activatio_filtrum(cum, FALSUM);
        activatio_filtrum(sine, FALSUM);
        activatio_imago(sine, FALSUM);
        per (functio = ZEPHYRUM; functio < III; functio += II)
        {
            constans i8* tv = _tabula_functionis(III, functio,
                c->piscina);
                    i32 pondera[III];

            CREDO_NECESSE (tv != NIHIL);
            pondera[ZEPHYRUM]  = ZEPHYRUM;
            pondera[I]         = I;
            pondera[II]        = II;
            fac
            {
                Iudicium x;
                Iudicium y;

                CREDO_NECESSE (activatio_iudica(cum, pondera, III,
                    SCALA_D119, III, tv, &x));
                CREDO_NECESSE (activatio_iudica(sine, pondera, III,
                    SCALA_D119, III, tv, &y));
                diversae += _iudicia_aequalia(&x, &y) ? ZEPHYRUM : I;
            } dum (_copia_proxima(pondera, III, V));
        }
        CREDO_AEQUALIS_S64 ((s64)diversae, ZEPHYRUM);
        activatio_statistica(cum, &st);
        si (r == ZEPHYRUM)
        {
            CREDO_VERUM (st.imagine > ZEPHYRUM);
            CREDO_AEQUALIS_S64 ((s64)st.algebraice, ZEPHYRUM);
        } alioquin
        {
            CREDO_AEQUALIS_S64 ((s64)st.imagine, ZEPHYRUM);
            CREDO_VERUM (st.algebraice > ZEPHYRUM);
        }
    }
}

/* nomen unum: activatio_cella summam repraesentabilem per imaginem
 * nominat, ergo summae quaestionis idem punctum. Summae VIII copiae {0,
 * 1, 2} zeta_8 per activatio_cella, deinde activatio_iudica eiusdem
 * copiae (sine filtro): puncta nova nulla. Commutatio inter duas
 * (cella sine imagine, iudica cum) puncta nova facit - probatio
 * differentiam videt. Summa non repraesentabilis (1/3; D = 2): via
 * algebraica. Cellae eaedem ac sine imagine. */
interior vacuum
_probare_imaginis_nomina (
    CredoContextus* c)
{
            Catalogus* zeta = _zeta_octo_catalogus(c->piscina);
            Quaternio  summae[VIII];
            Quaternio  tertia;
                  i32  pondera[III];
                  i32  modus;
                  i32  v;
                  i32  j;

    CREDO_NECESSE_NON_NIHIL (zeta);
    per (v = ZEPHYRUM; v < VIII; v++)
    {
        CREDO_NECESSE (quaternio_nullum(quaternio_anulus(
            catalogus_elementum(zeta, ZEPHYRUM)), c->piscina,
            &summae[v]));
        per (j = ZEPHYRUM; j < III; j++)
        {
            Quaternio q = catalogus_elementum(zeta, (i32)j);

            CREDO_NECESSE (((v >> j) & I)
                ? quaternio_adde(summae[v], q, c->piscina, &summae[v])
                : quaternio_subtrahe(summae[v], q, c->piscina,
                &summae[v]));
        }
    }
    pondera[ZEPHYRUM]  = ZEPHYRUM;
    pondera[I]         = I;
    pondera[II]        = II;
    /* modus 0: cella et iudica cum imagine; 1: ambae sine; 2: cella
     * sine, iudica cum (nomina diversa: puncta nova) */
    per (modus = ZEPHYRUM; modus < III; modus++)
    {
               Activatio* act   = NIHIL;
               Activatio* sine  = NIHIL;
        CellaeStatistica  ante;
        CellaeStatistica  post_cellae;
        CellaeStatistica  post;
                Iudicium  iu;
                     b32  eaedem    = VERUM;

        CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
        CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &sine));
        activatio_filtrum(act, FALSUM);
        activatio_imago(sine, FALSUM);
        activatio_imago(act, modus == ZEPHYRUM);
        activatio_statistica(act, &ante);
        per (v = ZEPHYRUM; v < VIII; v++)
        {
            Cella x;
            Cella y;

            CREDO_NECESSE (activatio_cella(act, summae[v], XXIV, &x));
            CREDO_NECESSE (activatio_cella(sine, summae[v], XXIV, &y));
            eaedem = eaedem && x.regula == y.regula
                && x.numerus == y.numerus
                && memcmp(x.ligatae, y.ligatae,
                (memoriae_index)x.numerus
                * magnitudo(i32)) == ZEPHYRUM;
        }
        CREDO_VERUM (eaedem);
        activatio_statistica(act, &post_cellae);
        activatio_imago(act, modus != I);
        CREDO_NECESSE (activatio_iudica(act, pondera, III, SCALA_D119,
            III, cellae_paritas(III, c->piscina), &iu));
        activatio_statistica(act, &post);
        si (modus == ZEPHYRUM)
        {
            CREDO_AEQUALIS_S64 ((s64)(post_cellae.imagine
                - ante.imagine),
                VIII);
            CREDO_AEQUALIS_S64 ((s64)(post_cellae.algebraice
                - ante.algebraice), ZEPHYRUM);
        }
        si (modus == I)
        {
            CREDO_AEQUALIS_S64 ((s64)(post_cellae.algebraice
                - ante.algebraice), VIII);
        }
        CREDO_VERUM (post_cellae.puncta > ante.puncta);
        si (modus == II)
        {
            CREDO_VERUM (post.puncta > post_cellae.puncta);
        } alioquin
        {
            CREDO_AEQUALIS_S64 ((s64)post.puncta,
                (s64)post_cellae.puncta);
        }
    }
    /* non repraesentabilis: 1/3 x D (= 2) non integer */
    tertia = _q(quaternio_anulus(catalogus_elementum(zeta, ZEPHYRUM)),
        "[1/3, 0, 0, 0]", c->piscina);
    {
               Activatio* act   = NIHIL;
               Activatio* sine  = NIHIL;
        CellaeStatistica  ante;
        CellaeStatistica  post;
                   Cella  x;
                   Cella  y;

        CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
        CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &sine));
        activatio_imago(sine, FALSUM);
        activatio_statistica(act, &ante);
        CREDO_NECESSE (activatio_cella(act, tertia, XXIV, &x));
        CREDO_NECESSE (activatio_cella(sine, tertia, XXIV, &y));
        activatio_statistica(act, &post);
        CREDO_AEQUALIS_S64 ((s64)(post.imagine - ante.imagine),
            ZEPHYRUM);
        CREDO_AEQUALIS_S64 ((s64)(post.algebraice - ante.algebraice),
            I);
        CREDO_AEQUALIS_I32 (x.regula, y.regula);
        CREDO_AEQUALIS_I32 (x.numerus, y.numerus);
    }
}

/* q x c, c scalaris rationalis (textus) */
interior Quaternio
_scalatum (
               Quaternio  q,
      constans character* scala,
                 Piscina* piscina)
{
    Quaternio c;
    Quaternio exitus;
    character textus[CXXVIII];

    sprintf(textus, "[%s, 0, 0, 0]", scala);
    c = _q(quaternio_anulus(q), textus, piscina);
    si (   c.partes == NIHIL
        || !quaternio_multiplica(c, q, piscina, &exitus))
    {
        exitus.anulus = NIHIL;
        exitus.partes = NIHIL;
    }
    redde exitus;
}

/* omnes copiae n = III ex catalogo XXIV (passu dato), XOR et MAJ:
 * Iudicium activationis 'act' idem ac 'ref'; *diversae, *comparatae */
interior b32
_iudicia_comparare (
     Activatio* act,
     Activatio* ref,
           i64  passus,
       Piscina* piscina,
           i64* diversae,
           i64* comparatae)
{
    i32 functio;

    per (functio = ZEPHYRUM; functio < III; functio += II)
    {
        constans i8* tv = _tabula_functionis(III, functio, piscina);
                i32  pondera[III];
                i64  ordo = ZEPHYRUM;

        si (tv == NIHIL)
        {
            redde FALSUM;
        }
        pondera[ZEPHYRUM]  = ZEPHYRUM;
        pondera[I]         = I;
        pondera[II]        = II;
        fac
        {
            Iudicium x;
            Iudicium y;

            si ((ordo++ % passus) != ZEPHYRUM)
            {
                perge;
            }
            si (   !activatio_iudica(act, pondera, III, SCALA_D119, III,
                    tv, &x)
                || !activatio_iudica(ref, pondera, III, SCALA_D119, III,
                    tv, &y))
            {
                redde FALSUM;
            }
            (*comparatae)++;
            *diversae += _iudicia_aequalia(&x, &y) ? ZEPHYRUM : I;
        } dum (_copia_proxima(pondera, III, XXIV));
    }
    redde VERUM;
}

/* cellae II T5 (recensio H1, M1): filtrum ad scalam quamvis. Axes
 * zeta_8 expressae per 10^-17, 10^-20, 10^-25, 10^45 multiplicatae
 * (directiones eaedem, cellae eaedem); catalogus per 10^45 (elementa
 * magna, imago nulla). Cum filtro = sine filtro scalae unitatis (n =
 * III, XOR et MAJ). Olim axes parvae in duplices XX decimalium:
 * directio falsa aut NaN, maximum verum praetermissum (cella extra
 * alveum); magnae et elementa magna: conversio deficit, iudicium
 * FALSUM.
 * Nunc axes normatae (filtrum decidit); elementa magna: filtrum
 * abstinet, via exacta. */
interior vacuum
_probare_filtrum_scalas (
    CredoContextus* c)
{
    hic_manens constans character* SCALAE[IV] = {
        "1/100000000000000000",
        "1/100000000000000000000",
        "1/10000000000000000000000000",
        "1000000000000000000000000000000000000000000000"
    };
     Catalogus* zeta  = _zeta_octo_catalogus(c->piscina);
     Activatio* ref   = NIHIL;
     Quaternio* axes;
           i32  nd;
           i32  r;
           i32  j;
           i64  diversae    = ZEPHYRUM;
           i64  comparatae  = ZEPHYRUM;

    CREDO_NECESSE_NON_NIHIL (zeta);
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &ref));
    activatio_filtrum(ref, FALSUM);
    nd    = activatio_axes(ref);
    axes  = (Quaternio*)piscina_allocare(c->piscina,
        (memoriae_index)nd * magnitudo(Quaternio));
    CREDO_NECESSE_NON_NIHIL (axes);
    per (r = ZEPHYRUM; r < IV; r++)
    {
               Activatio* act = NIHIL;
        CellaeStatistica  st;

        per (j = ZEPHYRUM; j < nd; j++)
        {
            axes[j] = _scalatum(activatio_axis(ref, j), SCALAE[r],
                c->piscina);
            CREDO_NECESSE_NON_NIHIL (axes[j].partes);
        }
        CREDO_NECESSE (activatio_creare(zeta, axes, nd,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
        CREDO_NECESSE (_iudicia_comparare(act, ref, I, c->piscina,
            &diversae, &comparatae));
        /* filtrum axes scalatas vere adhibet (non tacite abstinet) */
        activatio_statistica(act, &st);
        CREDO_VERUM (st.filtro > ZEPHYRUM);
    }
    {
               Quaternio  puncta[XXIV];
               Catalogus* magnus  = NIHIL;
               Activatio* act     = NIHIL;
        CellaeStatistica  st;

        per (j = ZEPHYRUM; j < XXIV; j++)
        {
            puncta[j] = _scalatum(catalogus_elementum(zeta, j),
                SCALAE[III], c->piscina);
            CREDO_NECESSE_NON_NIHIL (puncta[j].partes);
        }
        CREDO_NECESSE (catalogus_ex_punctis(puncta, XXIV, c->piscina,
            &magnus));
        CREDO_NECESSE (activatio_creare(magnus, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
        CREDO_AEQUALIS_I32 (activatio_axes(act), nd);
        CREDO_NECESSE (_iudicia_comparare(act, ref, VII, c->piscina,
            &diversae, &comparatae));
        /* elementa extra duplices (XX decimales > alveus): filtrum
         * abstinet, omnis cella exacta - nihil aestimatum */
        activatio_statistica(act, &st);
        CREDO_AEQUALIS_S64 ((s64)st.filtro, ZEPHYRUM);
        CREDO_VERUM (st.exacte > ZEPHYRUM);
    }
    CREDO_AEQUALIS_S64 ((s64)diversae, ZEPHYRUM);
    /* IV x 2024 x II + ceil(2024 / VII) x II */
    CREDO_AEQUALIS_S64 ((s64)comparatae, (s64)(IV * 2024 * II
        + 290 * II));
}

/* testes recensionis (H1): catalogi parvi Q(sqrt 2). (i) axis
 * expressa 314159 x 10^-30 et (1, 1, 0) - olim SIGBUS; (ii, iii) axes
 * propriae ex elemento parte vectoria ~10^-25 et ~10^-20 (olim verdicta
 * falsa, crash). Omnes copiae n = II, III, omnes tabulae, k {1, 2, 3,
 * 4, 6, 12}, D82 et D94: cum filtro = sine. */
interior vacuum
_probare_filtri_testes (
    CredoContextus* c)
{
    hic_manens constans character* PUNCTA[III][IV] = {
        { "[2, 1, 0, 0]", "[1, 0, 1, 0]", NIHIL, NIHIL },
        { "[1, 1/5000000000000000000000000, "
          "-7/10000000000000000000000000, "
          "1/5000000000000000000000000]",
          "[2, 3, -1, -1]", "[1, -3, 1, 2]", "[0, 21, -70, 21]" },
        { "[1, 9/100000000000000000000, -1/100000000000000000000, "
          "-3/50000000000000000000]",
          "[2, 91, -9, -59]", "[-3, 89, -9, -61]", "[0, -3, 1, -1]" }
    };
    hic_manens constans i32 NUMERI[III]  = { II, IV, IV };
    hic_manens constans i32 SCALA[VI]    = { I, II, III, IV, VI, XII };
    constans Anulus* q2 = extensio_anulus(extensio_quadratica(II,
        c->piscina));
    i64 diversae    = ZEPHYRUM;
    i64 comparatae  = ZEPHYRUM;
    i32 t;

    per (t = ZEPHYRUM; t < III; t++)
    {
        Quaternio  puncta[IV];
        Catalogus* cat = NIHIL;
              i32  cella;
              i32  j;

        per (j = ZEPHYRUM; j < NUMERI[t]; j++)
        {
            puncta[j] = _q(q2, PUNCTA[t][j], c->piscina);
            CREDO_NECESSE_NON_NIHIL (puncta[j].partes);
        }
        CREDO_NECESSE (catalogus_ex_punctis(puncta, NUMERI[t],
            c->piscina, &cat));
        per (cella = CELLAE_CELLA_D82; cella
            <= CELLAE_CELLA_D94; cella++)
        {
            Activatio* cum   = NIHIL;
            Activatio* sine  = NIHIL;
                  i32  n;

            si (t == ZEPHYRUM)
            {
                Quaternio axes[II];

                axes[ZEPHYRUM] = _q(q2,
                    "[0, 314159/1000000000000000000000000000000, 0, 0]",
                    c->piscina);
                axes[I] = _q(q2, "[0, 1, 1, 0]", c->piscina);
                CREDO_NECESSE (activatio_creare(cat, axes, II, cella,
                    ZEPHYRUM, c->piscina, &cum));
                CREDO_NECESSE (activatio_creare(cat, axes, II, cella,
                    ZEPHYRUM, c->piscina, &sine));
            } alioquin
            {
                CREDO_NECESSE (activatio_creare(cat, NIHIL, ZEPHYRUM,
                    cella, ZEPHYRUM, c->piscina, &cum));
                CREDO_NECESSE (activatio_creare(cat, NIHIL, ZEPHYRUM,
                    cella, ZEPHYRUM, c->piscina, &sine));
            }
            activatio_filtrum(sine, FALSUM);
            per (n = II; n <= III && n <= NUMERI[t]; n++)
            {
                i32 pondera[III];
                i32 tabulae = (i32)I << (I << n);

                per (j = ZEPHYRUM; j < n; j++)
                {
                    pondera[j] = j;
                }
                fac
                {
                    i32 v;

                    per (v = ZEPHYRUM; v < tabulae; v++)
                    {
                              i8 tv[VIII];
                        Iudicium x;
                        Iudicium y;
                             i32 b;

                        per (b = ZEPHYRUM; b < (i32)(I << n); b++)
                        {
                            tv[b] = (i8)((v >> b) & I);
                        }
                        CREDO_NECESSE (activatio_iudica(cum, pondera, n,
                            SCALA, VI, tv, &x));
                        CREDO_NECESSE (activatio_iudica(sine, pondera,
                            n, SCALA, VI, tv, &y));
                        comparatae++;
                        diversae += _iudicia_aequalia(&x, &y)
                            ? ZEPHYRUM : I;
                    }
                } dum (_copia_proxima(pondera, n, NUMERI[t]));
            }
        }
    }
    CREDO_AEQUALIS_S64 ((s64)diversae, ZEPHYRUM);
    /* (16 + 2 x (6 x 16 + 4 x 256)) x II cellae */
    CREDO_AEQUALIS_S64 ((s64)comparatae, (s64)((XVI + II * (VI * XVI
        + IV * CCLVI)) * II));
}

/* recensio M2: nullus limes k. Olim termini S_k LXIV k distincti per
 * activationem, memoria puncti LXIV (k, signum), scala avida LXIV
 * prima: ultra, FALSUM aut k tacite omissi, secundum historiam
 * vocationum. */
interior vacuum
_probare_k_sine_limite (
    CredoContextus* c)
{
    Catalogus* zeta  = _zeta_octo_catalogus(c->piscina);
    Activatio* act   = NIHIL;
    Activatio* sine  = NIHIL;
    Quaternio  sigma;
    Quaternio  minus_sigma;
    Quaternio  nullum;
          i32  scala[LXX];
          i32  pondera[III];
          i32  k;
          b32  eaedem = VERUM;
     Iudicium  x;
     Iudicium  y;

    CREDO_NECESSE_NON_NIHIL (zeta);
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
    sigma = catalogus_elementum(zeta, I);
    CREDO_NECESSE (quaternio_nullum(quaternio_anulus(sigma), c->piscina,
        &nullum));
    CREDO_NECESSE (quaternio_subtrahe(nullum, sigma, c->piscina,
        &minus_sigma));
    /* punctum unum, XL k x II signa = LXXX in memoria puncti; cellae
     * eaedem ac activationis novae ad k solum */
    per (k = I; k <= XL; k++)
    {
        Activatio* nova = NIHIL;
            Cella  cl;
            Cella  cn;
              i32  s;

        CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &nova));
        per (s = ZEPHYRUM; s < II; s++)
        {
            Quaternio q = s == ZEPHYRUM ? sigma : minus_sigma;

            CREDO_NECESSE (activatio_cella(act, q, k, &cl));
            CREDO_NECESSE (activatio_cella(nova, q, k, &cn));
            eaedem = eaedem && cl.regula == cn.regula
                && cl.numerus == cn.numerus;
        }
    }
    CREDO_VERUM (eaedem);
    /* LXX k distincti in una activatione (termini) */
    per (k = ZEPHYRUM; k < LXX; k++)
    {
        scala[k] = k + I;
    }
    CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
        CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &sine));
    activatio_filtrum(sine, FALSUM);
    pondera[ZEPHYRUM]  = ZEPHYRUM;
    pondera[I]         = I;
    pondera[II]        = II;
    CREDO_NECESSE (activatio_iudica(act, pondera, III, scala, LXX,
        cellae_paritas(III, c->piscina), &x));
    CREDO_NECESSE (activatio_iudica(sine, pondera, III, scala, LXX,
        cellae_paritas(III, c->piscina), &y));
    CREDO_VERUM (_iudicia_aequalia(&x, &y));
    /* avida: LXIV x k = 1, deinde VI - eadem ac {1, VI} */
    {
        hic_manens constans i32  BREVIS[II] = { I, VI };
                            i32  longa[LXV];
                      Activatio* d82 = NIHIL;
                      Capacitas  c1;
                      Capacitas  c2;
                       Quaestio  q;
                            i32  n;
                            b32  bene = VERUM;

        per (k = ZEPHYRUM; k < LXIV; k++)
        {
            longa[k] = I;
        }
        longa[LXIV] = VI;
        CREDO_NECESSE (activatio_ex_catalogo(zeta, ZEPHYRUM, c->piscina,
            &d82));
        _quaestio(&q, CELLAE_REGULA);
        q.n_maximum        = IV;
        q.limes_triplicum  = XII;
        q.scala_k          = longa;
        q.numerus_k        = LXV;
        CREDO_NECESSE (activatio_avida(d82, &q, c->piscina, &c1));
        q.scala_k    = BREVIS;
        q.numerus_k  = II;
        CREDO_NECESSE (activatio_avida(d82, &q, c->piscina, &c2));
        CREDO_VERUM (c2.numerus[III] > ZEPHYRUM);
        per (n = III; n <= IV; n++)
        {
            i32 w;

            bene = bene && c1.numerus[n] == c2.numerus[n]
                && c1.servati[n] == c2.servati[n];
            per (w = ZEPHYRUM; bene && w < c1.servati[n]; w++)
            {
                bene = bene
                    && c1.victores[n][w].k == c2.victores[n][w].k;
            }
        }
        CREDO_VERUM (bene);
    }
}

/* recensio L1: activatio_cella, summa in imagine sse |coefficiens x D|
 * <= 2^62 (negatio in s64 semper; olim -2^63 x -1 = mores indefiniti).
 * zeta_8: D = 2. 2^61 -> 2^62 imago; 2^61 + 1/2 et -2^62 (-> -2^63):
 * via algebraica. Cellae eaedem ac sine imagine. */
interior vacuum
_probare_imaginis_summas (
    CredoContextus* c)
{
    hic_manens constans character* SUMMAE[III] = {
        "[2305843009213693952, 1, 0, 0]",
        "[4611686018427387905/2, 1, 0, 0]",
        "[-4611686018427387904, 1, 0, 0]"
    };
    hic_manens constans s64  IMAGINE[III] = { 1, 0, 0 };
                  Catalogus* zeta = _zeta_octo_catalogus(c->piscina);
                        i32  r;

    CREDO_NECESSE_NON_NIHIL (zeta);
    per (r = ZEPHYRUM; r < III; r++)
    {
               Activatio* act   = NIHIL;
               Activatio* sine  = NIHIL;
        CellaeStatistica  ante;
        CellaeStatistica  post;
               Quaternio  s     =
                   _q(quaternio_anulus(catalogus_elementum(
                   zeta, ZEPHYRUM)), SUMMAE[r], c->piscina);
        Cella x;
        Cella y;

        CREDO_NECESSE_NON_NIHIL (s.partes);
        CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &act));
        CREDO_NECESSE (activatio_creare(zeta, NIHIL, ZEPHYRUM,
            CELLAE_CELLA_D94, ZEPHYRUM, c->piscina, &sine));
        activatio_imago(sine, FALSUM);
        activatio_statistica(act, &ante);
        CREDO_NECESSE (activatio_cella(act, s, XXIV, &x));
        CREDO_NECESSE (activatio_cella(sine, s, XXIV, &y));
        activatio_statistica(act, &post);
        CREDO_AEQUALIS_S64 ((s64)(post.imagine - ante.imagine),
            IMAGINE[r]);
        CREDO_AEQUALIS_S64 ((s64)(post.algebraice - ante.algebraice),
            I - IMAGINE[r]);
        CREDO_AEQUALIS_I32 (x.regula, y.regula);
        CREDO_AEQUALIS_I32 (x.numerus, y.numerus);
    }
}

/* PURGARE (credo examen 92-95; cursores ad gradus T5, ex fusione main):
 * status moduli - catalogus fluitans f_*, campus et clausura oraculi
 * o_*, tabula victorum ref_*, status aleae - inter sectiones NON manet. Sectio quae o_d/o_scala pro zeta_12
 * ponit (III, XXXII) alias ad initium (II, II) reddit; quaeque sectio a
 * statu initiali incipit, ordine quovis (CREDO_ORDO=mixtus). */
interior vacuum
_statum_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    memset(f_cat, ZEPHYRUM, magnitudo(f_cat));
    memset(f_prof, ZEPHYRUM, magnitudo(f_prof));
    memset(f_torsio, ZEPHYRUM, magnitudo(f_torsio));
    f_n       = ZEPHYRUM;
    o_d       = II;
    o_scala   = II;
    o_vitium  = FALSUM;
    memset(o_axis, ZEPHYRUM, magnitudo(o_axis));
    memset(o_n2, ZEPHYRUM, magnitudo(o_n2));
    o_nd      = ZEPHYRUM;
    memset(ref_victor, ZEPHYRUM, magnitudo(ref_victor));
    memset(ref_k, ZEPHYRUM, magnitudo(ref_k));
    memset(ref_numerus, ZEPHYRUM, magnitudo(ref_numerus));
    o_status_aleae = I;
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "zeta_8", _probare_zeta_octo, NIHIL, _statum_purgare, NIHIL,
        NIHIL },
    { "zeta_12", _probare_zeta_duodecim, NIHIL, _statum_purgare, NIHIL,
        NIHIL },
    { "limites", _probare_limites, NIHIL, _statum_purgare, NIHIL,
        NIHIL },
    { "axes", _probare_axes, NIHIL, _statum_purgare, NIHIL, NIHIL },
    { "cellae manu", _probare_cellas_manu, NIHIL, _statum_purgare,
        NIHIL, NIHIL },
    { "cellae contra oraculum", _probare_cellas, NIHIL, _statum_purgare,
        NIHIL,
      NIHIL },
    { "separatio contra D124", _probare_separationem, NIHIL,
        _statum_purgare,
        NIHIL,
      NIHIL },
    { "regulae fixae", _probare_regulas_fixas, NIHIL, _statum_purgare,
        NIHIL,
        NIHIL },
    { "collisio", _probare_collisionem, NIHIL, _statum_purgare, NIHIL,
        NIHIL },
    { "exhaustiva contra D124", _probare_exhaustivam, NIHIL,
        _statum_purgare,
        NIHIL,
      NIHIL },
    { "quaestiones aliae", _probare_quaestiones_alias, NIHIL,
        _statum_purgare,
      NIHIL, NIHIL },
    { "filtrum sine iactura", _probare_filtrum, NIHIL, _statum_purgare,
        NIHIL,
      NIHIL },
    { "filtrum avidum", _probare_filtrum_avidum, NIHIL, _statum_purgare,
        NIHIL,
      NIHIL },
    { "scrutinium axium", _probare_scrutinium, NIHIL, _statum_purgare,
        NIHIL,
      NIHIL },
    { "cellae zeta_12 contra oraculum", _probare_cellas_duodecim, NIHIL,
      _statum_purgare, NIHIL, NIHIL },
    { "victores D82 profundi", _probare_victores_d82, NIHIL,
        _statum_purgare,
        NIHIL,
      NIHIL },
    { "limites quaestionis", _probare_limites_quaestionis, NIHIL,
        _statum_purgare,
      NIHIL, NIHIL },
    { "puncta et axes expressi", _probare_puncta_et_axes, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "puncta refutationes", _probare_puncta_refutationes, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "cellae phasis contra oraculum", _probare_cellas_phasis, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "cellae phasis manu", _probare_cellas_phasis_manu, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "iudicium contra D119", _probare_iudicium_d119, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "iudicium contra D119 lentum", _probare_iudicium_d119_lentum,
        NIHIL, _statum_purgare, "lentus", NIHIL },
    { "iudicium filtrum sine iactura", _probare_iudicium_filtrum, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "iudicium manu", _probare_iudicium_manu, NIHIL, _statum_purgare,
        NIHIL, NIHIL },
    { "iudicium memoria", _probare_iudicium_memoriam, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "imago sine iactura", _probare_imaginem, NIHIL, _statum_purgare,
        NIHIL, NIHIL },
    { "imago limites", _probare_imaginis_limites, NIHIL,
        _statum_purgare,
        NIHIL, NIHIL },
    { "imago nomina", _probare_imaginis_nomina, NIHIL, _statum_purgare,
        NIHIL, NIHIL },
    { "imago summae limites", _probare_imaginis_summas, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "filtrum scalae", _probare_filtrum_scalas, NIHIL, _statum_purgare,
        NIHIL, NIHIL },
    { "filtrum testes recensionis", _probare_filtri_testes, NIHIL,
        _statum_purgare, NIHIL, NIHIL },
    { "k sine limite", _probare_k_sine_limite, NIHIL, _statum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cellae", SECTIONES);
}
