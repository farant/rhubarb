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

nomen structura { s64 p; s64 q; } Binomium;     /* p + q sqrt2 */

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
    redde binomium(x.p * y.p + II * x.q * y.q, x.p * y.q + x.q * y.p);
}

interior b32
binomium_nullum (
    Binomium x)
{ redde x.p == 0 && x.q == 0;
}

interior s32
binomium_signum (
    Binomium x)
{
    si (x.p >= 0 && x.q >= 0) redde (x.p != 0 || x.q != 0) ? I : 0;
    si (x.p <= 0 && x.q <= 0) redde -(s32)I;
    si (x.p > 0) redde x.p * x.p > II * x.q * x.q ? I : -(s32)I;
    redde II * x.q * x.q > x.p * x.p ? I : -(s32)I;
}

interior duplex
binomium_valor (
    Binomium x)
{ redde (duplex)x.p + (duplex)x.q * sqrt(2.0);
}

/* pars exacta x 2 ut integrum Z[sqrt2] (catalogus zeta_8: (p + q
 * sqrt2)/2) */
interior Binomium
_binomium_ex (
    Quaternio  q,
          i32  k,
      Piscina* piscina)
{
    constans Algebraicus* a = (constans Algebraicus*)quaternio_pars(q,
        k);
    Fractio f0 = fractio_multiplica(algebraicus_coefficiens(*a,
        ZEPHYRUM,
        piscina), fractio_ex_s64(II), piscina);
    Fractio f1 = fractio_multiplica(algebraicus_coefficiens(*a, I,
        piscina), fractio_ex_s64(II), piscina);
    s64 p = ZEPHYRUM;
    s64 r = ZEPHYRUM;

    (vacuum)magnus_ad_s64(fractio_numerator(f0), &p);
    (vacuum)magnus_ad_s64(fractio_numerator(f1), &r);
    redde binomium(p, r);
}

#define O_MAX 64
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

/* tan^2(m pi/k) in Q(sqrt2)? (x + y sqrt2)/z */
interior b32
_o_terminus_exactus (
    i32  m,
    i32  k,
    s64* x,
    s64* y,
    s64* z)
{
    /* m/k reducta in {1/8, 1/6, 1/4, 1/3, 3/8} */
    i32 a = m;
    i32 b = k;
    i32 g;

    per (g = a; g > I; g--)
    {
        si (a % g == 0 && b % g == 0)
        { a /= g; b /= g; g = a + I;
        }
    }
    si (a == I && b == VIII)
    { *x = 3; *y = -2; *z = 1; redde VERUM;
    }
    si (a == I && b == VI)
    { *x = 1; *y = 0; *z = 3; redde VERUM;
    }
    si (a == I && b == IV)
    { *x = 1; *y = 0; *z = 1; redde VERUM;
    }
    si (a == I && b == III)
    { *x = 3; *y = 0; *z = 1; redde VERUM;
    }
    si (a == III && b == VIII)
    { *x = 3; *y = 2; *z = 1; redde VERUM;
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "zeta_8", _probare_zeta_octo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "zeta_12", _probare_zeta_duodecim, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limites", _probare_limites, NIHIL, NIHIL, NIHIL, NIHIL },
    { "axes", _probare_axes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cellae manu", _probare_cellas_manu, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cellae contra oraculum", _probare_cellas, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cellae", SECTIONES);
}
