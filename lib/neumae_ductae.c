/* neumae_ductae.c - partes neumarum quadratarum ut larvae (gabc pars
 * II, P2). Regulae in include/neumae_ductae.h; picturae et leges in
 * probationes/probatio_neumae_ductae.c.
 *
 * Duo modi pingendi: rectangula (quadrum, plena) et figurae
 * obliquae aut curvae (polygonum aut via crassa) per exempla IV x IV
 * in pixelo, tegumentum ad quartas rotundatum (0, 0x40, 0x80, 0xC0,
 * 0xFF) - eadem legenda ac glyphae_ductae, ergo picturae ASCII omnia
 * exprimunt. Figurae plures maximo coniunguntur. */
#include "latina.h"
#include "neumae_ductae.h"
#include <math.h>
#include <string.h>

#define PLENUM          0xFF
#define PUNCTA_MAXIMA   LXIV
#define CURVA_GRADUS    XII
#define EXEMPLA         IV            /* per latus pixeli */
#define CIRCULI_GRADUS  XXIV

nomen structura {
     i8* larva;
    s32  latitudo;
    s32  altitudo;
} Pictor;

nomen structura {
    f64 x[PUNCTA_MAXIMA];
    f64 y[PUNCTA_MAXIMA];
    s32 numerus;
} Via;


/* ==================================================
 * Instrumenta
 * ================================================== */

/* [x0, x1) x [y0, y1) valore, intra larvam */
interior vacuum
quadrum_valore (
    constans Pictor* p,
                s32  x0,
                s32  y0,
                s32  x1,
                s32  y1,
                 i8  valor)
{
    s32 x;
    s32 y;

    x0 = x0 < ZEPHYRUM ? ZEPHYRUM : x0;
    y0 = y0 < ZEPHYRUM ? ZEPHYRUM : y0;
    x1 = x1 > p->latitudo ? p->latitudo : x1;
    y1 = y1 > p->altitudo ? p->altitudo : y1;
    per (y = y0; y < y1; y++)
    {
        per (x = x0; x < x1; x++)
        {
            p->larva[y * p->latitudo + x] = valor;
        }
    }
}

interior vacuum
quadrum (
    constans Pictor* p,
                s32  x0,
                s32  y0,
                s32  x1,
                s32  y1)
{
    quadrum_valore(p, x0, y0, x1, y1, (i8)PLENUM);
}

interior vacuum
punctum_addere (
    Via* via,
    f64  x,
    f64  y)
{
    si (via->numerus < PUNCTA_MAXIMA)
    {
        via->x[via->numerus] = x;
        via->y[via->numerus] = y;
        via->numerus++;
    }
}

/* curva cubica a puncto ultimo per (x1,y1) (x2,y2) ad (x3,y3) */
interior vacuum
curvam_addere (
    Via* via,
    f64  x1,
    f64  y1,
    f64  x2,
    f64  y2,
    f64  x3,
    f64  y3)
{
    f64 x0;
    f64 y0;
    f64 t;
    f64 u;
    s32 k;

    x0 = via->x[via->numerus - I];
    y0 = via->y[via->numerus - I];
    per (k = I; k <= CURVA_GRADUS; k++)
    {
        t = (f64)k / (f64)CURVA_GRADUS;
        u = 1.0 - t;
        punctum_addere(via,
            u * u * u * x0 + 3.0 * u * u * t * x1 + 3.0 * u * t * t * x2
                + t * t * t * x3,
            u * u * u * y0 + 3.0 * u * u * t * y1 + 3.0 * u * t * t * y2
                + t * t * t * y3);
    }
}

/* regula par-impar (polygonum clausum, simplex aut non) */
interior b32
intra_polygonum (
    constans Via* via,
             f64  px,
             f64  py)
{
    s32 i;
    s32 j;
    b32 intra;

    intra  = FALSUM;
    j      = via->numerus - I;
    per (i = ZEPHYRUM; i < via->numerus; i++)
    {
        si (   (via->y[i] > py) != (via->y[j] > py)
            && px < (via->x[j] - via->x[i]) * (py - via->y[i])
                    / (via->y[j] - via->y[i]) + via->x[i])
        {
            intra = !intra;
        }
        j = i;
    }
    redde intra;
}

interior f64
distantia_segmenti (
    f64 px,
    f64 py,
    f64 x0,
    f64 y0,
    f64 x1,
    f64 y1)
{
    f64 dx;
    f64 dy;
    f64 t;
    f64 longitudo;

    dx         = x1 - x0;
    dy         = y1 - y0;
    longitudo  = dx * dx + dy * dy;
    t          = longitudo > 0.0
               ? ((px - x0) * dx + (py - y0) * dy) / longitudo : 0.0;
    t   = t < 0.0 ? 0.0 : (t > 1.0 ? 1.0 : t);
    dx  = x0 + t * dx - px;
    dy  = y0 + t * dy - py;
    redde sqrt(dx * dx + dy * dy);
}

/* exemplum intra figuram? polygonum (crassitudo 0) aut via crassa */
interior b32
exemplum_intra (
    constans Via* via,
             f64  crassitudo,
             f64  px,
             f64  py)
{
    s32 k;

    si (crassitudo <= 0.0)
    {
        redde intra_polygonum(via, px, py);
    }
    per (k = I; k < via->numerus; k++)
    {
        si (distantia_segmenti(px, py, via->x[k - I], via->y[k - I],
                via->x[k], via->y[k]) < crassitudo / 2.0)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* figura in larvam: tegumentum per exempla, ad quartas, maximo */
interior vacuum
tegere (
    constans Pictor* p,
       constans Via* via,
                f64  crassitudo)
{
    s32  x;
    s32  y;
    s32  i;
    s32  j;
    s32  numerus;
    s32  quarta;
     i8  valor;
     i8* locus;

    per (y = ZEPHYRUM; y < p->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < p->latitudo; x++)
        {
            numerus = ZEPHYRUM;
            per (j = ZEPHYRUM; j < EXEMPLA; j++)
            {
                per (i = ZEPHYRUM; i < EXEMPLA; i++)
                {
                    si (exemplum_intra(via, crassitudo,
                            (f64)x + ((f64)i + 0.5) / (f64)EXEMPLA,
                            (f64)y + ((f64)j + 0.5) / (f64)EXEMPLA))
                    {
                        numerus++;
                    }
                }
            }
            quarta = (numerus * IV + EXEMPLA * EXEMPLA / II)
                   / (EXEMPLA * EXEMPLA);
            valor = quarta >= IV ? (i8)PLENUM : (i8)(quarta * 0x40);
            locus = &p->larva[y * p->latitudo + x];
            si ((i32)valor > (i32)*locus)
            {
                *locus = valor;
            }
        }
    }
}

interior vacuum
polygonum (
    constans Pictor* p,
       constans Via* via)
{
    tegere(p, via, 0.0);
}

/* arcus circuli a angulus_initii ad angulus_finis (radianes) in viam */
interior vacuum
arcum_addere (
    Via* via,
    f64  cx,
    f64  cy,
    f64  radius,
    f64  angulus_initii,
    f64  angulus_finis)
{
    s32 k;
    f64 angulus;

    per (k = ZEPHYRUM; k <= CIRCULI_GRADUS; k++)
    {
        angulus = angulus_initii + (angulus_finis
            - angulus_initii) * (f64)k
              / (f64)CIRCULI_GRADUS;
        punctum_addere(via, cx + radius * cos(angulus),
            cy + radius * sin(angulus));
    }
}


/* ==================================================
 * Ambitus
 * ================================================== */

interior b32
ambitus_validus (
    NeumaePars pars,
           i32 spatium,
           s32 parametrum)
{
    si ((s32)pars < ZEPHYRUM || (s32)pars >= (s32)NEUMAE_NUMERUS_PARTIUM
        || spatium < VI || spatium > XLVIII || spatium % II != ZEPHYRUM)
    {
        redde FALSUM;
    }
    commutatio (pars)
    {
        casus NEUMAE_VIRGA:
        casus NEUMAE_VIRGA_REVERSA:
            redde parametrum >= II && parametrum <= VI;
        casus NEUMAE_PORRECTUS:
            redde parametrum >= I && parametrum <= V;
        casus NEUMAE_CUSTOS:
            redde parametrum == I || parametrum == -I;
        casus NEUMAE_EPISEMA:
            redde parametrum >= I && parametrum <= CCLVI;
        ordinarius:
            redde parametrum == ZEPHYRUM;
    }
}


/* ==================================================
 * Partes: mensura et pictura una regula
 * ================================================== */

interior vacuum
mensuram_ponere (
    NeumaeMensura* m,
              s32  latitudo,
              s32  altitudo,
              s32  origo_x,
              s32  origo_y,
              s32  capitis_latitudo)
{
    m->latitudo          = (i32)latitudo;
    m->altitudo          = (i32)altitudo;
    m->origo_x           = origo_x;
    m->origo_y           = origo_y;
    m->capitis_latitudo  = (i32)capitis_latitudo;
}

/* C clavis ad x0: duo quadrata b, intervallum g circa lineam, caulis
 * sinister */
interior vacuum
clavem_c_pingere (
    constans Pictor* p,
                s32  x0,
                s32  b,
                s32  g,
                s32  k)
{
    quadrum(p, x0, ZEPHYRUM, x0 + b, b);
    quadrum(p, x0, b + II * g + I, x0 + b, II * b + II * g + I);
    quadrum(p, x0, ZEPHYRUM, x0 + k, II * b + II * g + I);
}

/* mensuram computat; si p, etiam pingit (larva iam vacua) */
interior vacuum
partem_ducere (
         NeumaePars  pars,
                s32  s,
                s32  parametrum,
      NeumaeMensura* m,
    constans Pictor* p)
{
    s32 h;      /* gradus */
    s32 k;      /* crassitudo lineae */
    s32 H;      /* altitudo capitis */
    s32 d;      /* caput parvum */
    s32 b;      /* quadratum clavis, latitudo signi */
    s32 q;      /* spatium/IV */
    s32 c;      /* ordo originis */
    s32 w;
    s32 alt;
    s32 trabs;
    f64 t;
    f64 r;
    f64 cy;
    Via via;

    h            = s / II;
    k            = s / VIII > I ? s / VIII : I;
    H            = s - I;
    d            = II * (s / IV) + I;
    b            = s * III / IV;
    q            = s / IV;
    via.numerus  = ZEPHYRUM;

    commutatio (pars)
    {
        casus NEUMAE_PUNCTUM:
            mensuram_ponere(m, s, H, ZEPHYRUM, h - I, s);
            si (p) quadrum(p, ZEPHYRUM, ZEPHYRUM, s, H);
            frange;

        casus NEUMAE_CAVUM:
            mensuram_ponere(m, s, H, ZEPHYRUM, h - I, s);
            si (p)
            {
                quadrum(p, ZEPHYRUM, ZEPHYRUM, s, H);
                quadrum_valore(p, k, k, s - k, H - k, ZEPHYRUM);
            }
            frange;

        casus NEUMAE_STROPHA:
            mensuram_ponere(m, s, H, ZEPHYRUM, h - I, s);
            si (p)
            {
                punctum_addere(&via, 0.0, 0.0);
                punctum_addere(&via, (f64)s * 0.5, 0.0);
                punctum_addere(&via, (f64)s, (f64)H * 0.5);
                punctum_addere(&via, (f64)s, (f64)H);
                punctum_addere(&via, 0.0, (f64)H);
                polygonum(p, &via);
            }
            frange;

        casus NEUMAE_ORISCUS:
            /* fascia undosa: margo superior et inferior curvae
             * parallelae */
            mensuram_ponere(m, s, H, ZEPHYRUM, h - I, s);
            si (p)
            {
                punctum_addere(&via, 0.0, (f64)H * 0.35);
                curvam_addere(&via, (f64)s * 0.3, (f64)H * -0.45,
                    (f64)s * 0.6, (f64)H * 0.9, (f64)s, 0.0);
                punctum_addere(&via, (f64)s, (f64)H * 0.65);
                curvam_addere(&via, (f64)s * 0.7, (f64)H * 1.45,
                    (f64)s * 0.4, (f64)H * 0.1, 0.0, (f64)H);
                polygonum(p, &via);
            }
            frange;

        casus NEUMAE_QUILISMA:
            mensuram_ponere(m, s, H, ZEPHYRUM, h - I, s);
            si (p)
            {
                punctum_addere(&via, 0.0, (f64)H);
                punctum_addere(&via, 0.0, (f64)H * 0.45);
                punctum_addere(&via, (f64)s / 6.0, 0.0);
                punctum_addere(&via, (f64)s / 3.0, (f64)H * 0.45);
                punctum_addere(&via, (f64)s / 2.0, 0.0);
                punctum_addere(&via, (f64)s * 2.0 / 3.0, (f64)H * 0.45);
                punctum_addere(&via, (f64)s * 5.0 / 6.0, 0.0);
                punctum_addere(&via, (f64)s, (f64)H * 0.45);
                punctum_addere(&via, (f64)s, (f64)H);
                polygonum(p, &via);
            }
            frange;

        casus NEUMAE_INCLINATUM:
            mensuram_ponere(m, s, H, ZEPHYRUM, h - I, s);
            si (p)
            {
                punctum_addere(&via, 0.0, (f64)H * 0.5);
                punctum_addere(&via, (f64)s * 0.5, 0.0);
                punctum_addere(&via, (f64)s, (f64)H * 0.5);
                punctum_addere(&via, (f64)s * 0.5, (f64)H);
                polygonum(p, &via);
            }
            frange;

        casus NEUMAE_INCLINATUM_DEMINUTUM:
            mensuram_ponere(m, d, d, ZEPHYRUM, d / II, d);
            si (p)
            {
                punctum_addere(&via, 0.0, (f64)d * 0.5);
                punctum_addere(&via, (f64)d * 0.5, 0.0);
                punctum_addere(&via, (f64)d, (f64)d * 0.5);
                punctum_addere(&via, (f64)d * 0.5, (f64)d);
                polygonum(p, &via);
            }
            frange;

        casus NEUMAE_LINEA_PUNCTUM:
            w = s + II * q;
            mensuram_ponere(m, w, H, ZEPHYRUM, h - I, w);
            si (p)
            {
                quadrum(p, ZEPHYRUM, ZEPHYRUM, k, H);
                quadrum(p, w - k, ZEPHYRUM, w, H);
                quadrum(p, q, ZEPHYRUM, q + s, H);
                quadrum_valore(p, q + k, k, q + s - k, H - k, ZEPHYRUM);
            }
            frange;

        casus NEUMAE_DEMINUTUM:
            mensuram_ponere(m, d, d, ZEPHYRUM, d / II, d);
            si (p) quadrum(p, ZEPHYRUM, ZEPHYRUM, d, d);
            frange;

        casus NEUMAE_VIRGA:
        casus NEUMAE_VIRGA_REVERSA:
            c    = h - I;
            alt  = c + (h > parametrum * h + I ? h : parametrum * h
                + I);
            mensuram_ponere(m, s, alt, ZEPHYRUM, c, s);
            si (p)
            {
                quadrum(p, ZEPHYRUM, ZEPHYRUM, s, H);
                si (pars == NEUMAE_VIRGA)
                {
                    quadrum(p, s - k, c, s, c + parametrum * h + I);
                }
                alioquin
                {
                    quadrum(p, ZEPHYRUM, c, k, c + parametrum * h + I);
                }
            }
            frange;

        casus NEUMAE_AUCTUM_ASCENDENS:
            c = h - I + q;
            mensuram_ponere(m, s, H + q, ZEPHYRUM, c, s);
            si (p)
            {
                quadrum(p, ZEPHYRUM, q, s, q + H);
                quadrum(p, s - k, ZEPHYRUM, s, c + I);
            }
            frange;

        casus NEUMAE_AUCTUM_DESCENDENS:
            c = h - I;
            mensuram_ponere(m, s, H + q, ZEPHYRUM, c, s);
            si (p)
            {
                quadrum(p, ZEPHYRUM, ZEPHYRUM, s, H);
                quadrum(p, s - k, c, s, H + q);
            }
            frange;

        casus NEUMAE_PORRECTUS:
            mensuram_ponere(m, II * s, H + parametrum * h, ZEPHYRUM,
                h - I, II * s);
            si (p)
            {
                punctum_addere(&via, 0.0, 0.0);
                punctum_addere(&via, (f64)(II * s),
                    (f64)(parametrum * h));
                punctum_addere(&via, (f64)(II * s),
                    (f64)(parametrum * h + H));
                punctum_addere(&via, 0.0, (f64)H);
                polygonum(p, &via);
            }
            frange;

        casus NEUMAE_LINEA:
            mensuram_ponere(m, II * s, d, ZEPHYRUM, d / II, II * s);
            si (p) quadrum(p, ZEPHYRUM, ZEPHYRUM, II * s, d);
            frange;

        casus NEUMAE_BEMOLLE:
            /* caulis a spatio supra ad finem ventris; venter curva a
             * caule ad dextram et retro, in positione */
            c = s;
            mensuram_ponere(m, b, s + h, ZEPHYRUM, c, b);
            si (p)
            {
                quadrum(p, ZEPHYRUM, ZEPHYRUM, k, s + h);
                t = (f64)s / 6.0 > 1.2 ? (f64)s / 6.0 : 1.2;
                punctum_addere(&via, (f64)k * 0.5, (f64)(c - q));
                curvam_addere(&via, (f64)b * 1.05, (f64)c
                    - (f64)h * 1.2,
                    (f64)b * 1.05, (f64)c + (f64)h * 0.4,
                    (f64)k * 0.5, (f64)(c + h) - 0.5);
                tegere(p, &via, t);
            }
            frange;

        casus NEUMAE_NATURALE:
            w   = h + k;
            c   = s * III / IV;
            trabs  = q > II ? q : II;
            mensuram_ponere(m, w, II * c + I, ZEPHYRUM, c, w);
            si (p)
            {
                quadrum(p, ZEPHYRUM, ZEPHYRUM, k, c + q + I);
                quadrum(p, w - k, c - q, w, II * c + I);
                quadrum(p, ZEPHYRUM, c - q, w, c - q + trabs);
                quadrum(p, ZEPHYRUM, c + q - trabs + I, w, c + q + I);
            }
            frange;

        casus NEUMAE_DIESIS:
            w   = b;
            c   = h;
            trabs  = q > II ? q : II;
            mensuram_ponere(m, w, s + I, ZEPHYRUM, c, w);
            si (p)
            {
                quadrum(p, w / IV, ZEPHYRUM, w / IV + k, s + I);
                quadrum(p, w - w / IV - k, ZEPHYRUM, w - w / IV, s + I);
                quadrum(p, ZEPHYRUM, c - q, w, c - q + trabs);
                quadrum(p, ZEPHYRUM, c + q - trabs + I, w, c + q + I);
            }
            frange;

        casus NEUMAE_CLAVIS_C:
            mensuram_ponere(m, b, II * b + II * k + I, ZEPHYRUM, b + k,
                b);
            si (p) clavem_c_pingere(p, ZEPHYRUM, b, k, k);
            frange;

        casus NEUMAE_CLAVIS_F:
            /* caput cum caule deorsum in linea, deinde clavis C */
            c    = b + k;
            w    = II * b + q;
            alt  = II * b + II * k + I;
            alt  = alt > c + s + I ? alt : c + s + I;
            mensuram_ponere(m, w, alt, ZEPHYRUM, c, w);
            si (p)
            {
                quadrum(p, ZEPHYRUM, c - b / II, b, c - b / II + b);
                quadrum(p, b - k, c, b, c + s + I);
                clavem_c_pingere(p, b + q, b, k, k);
            }
            frange;

        casus NEUMAE_CUSTOS:
            c = parametrum > ZEPHYRUM ? s : d / II;
            mensuram_ponere(m, h, s + d / II + I, ZEPHYRUM, c, h);
            si (p)
            {
                quadrum(p, ZEPHYRUM, c - d / II, h, c + d / II + I);
                si (parametrum > ZEPHYRUM)
                {
                    quadrum(p, h - k, ZEPHYRUM, h, c + I);
                }
                alioquin
                {
                    quadrum(p, h - k, c, h, c + s + I);
                }
            }
            frange;

        casus NEUMAE_VIRGULA:
            trabs   = q > II ? q : II;
            w    = trabs + k;
            alt  = s * III / IV;
            mensuram_ponere(m, w, alt, ZEPHYRUM, alt / II, w);
            si (p)
            {
                t = (f64)trabs;
                punctum_addere(&via, (f64)w - t * 0.5, t * 0.5);
                punctum_addere(&via, (f64)w - t * 0.5, (f64)alt * 0.45);
                punctum_addere(&via, t * 0.4, (f64)alt - 0.5);
                tegere(p, &via, t);
            }
            frange;

        casus NEUMAE_PUNCTUM_MORAE:
            w = q > II ? q : II;
            mensuram_ponere(m, w, w, ZEPHYRUM, w / II, w);
            si (p) quadrum(p, ZEPHYRUM, ZEPHYRUM, w, w);
            frange;

        casus NEUMAE_ICTUS:
            mensuram_ponere(m, k, h, ZEPHYRUM, h / II, k);
            si (p) quadrum(p, ZEPHYRUM, ZEPHYRUM, k, h);
            frange;

        casus NEUMAE_EPISEMA:
            mensuram_ponere(m, parametrum, k, ZEPHYRUM, k / II,
                parametrum);
            si (p) quadrum(p, ZEPHYRUM, ZEPHYRUM, parametrum, k);
            frange;

        casus NEUMAE_ACCENTUS:
        casus NEUMAE_ACCENTUS_REVERSUS:
            mensuram_ponere(m, h, h, ZEPHYRUM, h / II, h);
            si (p)
            {
                t = (f64)k + 0.5;
                si (pars == NEUMAE_ACCENTUS)
                {
                    punctum_addere(&via, 0.5, (f64)h - 0.5);
                    punctum_addere(&via, (f64)h - 0.5, 0.5);
                }
                alioquin
                {
                    punctum_addere(&via, 0.5, 0.5);
                    punctum_addere(&via, (f64)h - 0.5, (f64)h - 0.5);
                }
                tegere(p, &via, t);
            }
            frange;

        casus NEUMAE_CIRCULUS:
            mensuram_ponere(m, h, h, ZEPHYRUM, h / II, h);
            si (p)
            {
                t = (f64)k + 0.5;
                arcum_addere(&via, (f64)h * 0.5, (f64)h * 0.5,
                    ((f64)h - t) * 0.5, 0.0, 2.0 * 3.14159265358979);
                tegere(p, &via, t);
            }
            frange;

        casus NEUMAE_SEMICIRCULUS:
        casus NEUMAE_SEMICIRCULUS_REVERSUS:
            alt = h / II + I;
            mensuram_ponere(m, h, alt, ZEPHYRUM, alt / II, h);
            si (p)
            {
                t   = (f64)k + 0.5;
                r   = ((f64)h - t) * 0.5;
                cy  = (f64)alt - t * 0.5 - r;
                si (pars == NEUMAE_SEMICIRCULUS)
                {
                    arcum_addere(&via, (f64)h * 0.5, cy, r, 0.0,
                        3.14159265358979);
                }
                alioquin
                {
                    arcum_addere(&via, (f64)h * 0.5, (f64)alt - cy, r,
                        3.14159265358979, 2.0 * 3.14159265358979);
                }
                tegere(p, &via, t);
            }
            frange;

        ordinarius:
            frange;
    }
}


/* ==================================================
 * Functiones
 * ================================================== */

b32
neumae_ductae_metiri (
       NeumaePars  pars,
              i32  spatium,
              s32  parametrum,
    NeumaeMensura* mensura)
{
    si (mensura == NIHIL || !ambitus_validus(pars, spatium, parametrum))
    {
        redde FALSUM;
    }
    partem_ducere(pars, (s32)spatium, parametrum, mensura, NIHIL);
    redde VERUM;
}

b32
neumae_ductae_pingere (
    NeumaePars  pars,
           i32  spatium,
           s32  parametrum,
           i32  latitudo,
           i32  altitudo,
            i8* larva)
{
    NeumaeMensura m;
           Pictor p;

    si (larva == NIHIL || !ambitus_validus(pars, spatium, parametrum))
    {
        redde FALSUM;
    }
    partem_ducere(pars, (s32)spatium, parametrum, &m, NIHIL);
    si (m.latitudo != latitudo || m.altitudo != altitudo)
    {
        redde FALSUM;
    }
    memset(larva, ZEPHYRUM, (size_t)(latitudo * altitudo));
    p.larva     = larva;
    p.latitudo  = (s32)latitudo;
    p.altitudo  = (s32)altitudo;
    partem_ducere(pars, (s32)spatium, parametrum, &m, &p);
    redde VERUM;
}

constans character*
neumae_partis_titulus (
    NeumaePars pars)
{
    hic_manens constans character* tituli[] = {
        "punctum", "inclinatum", "inclinatum_deminutum", "virga",
        "virga_reversa", "stropha", "oriscus", "quilisma", "cavum",
        "linea_punctum", "deminutum", "auctum_ascendens",
        "auctum_descendens", "porrectus", "linea", "bemolle",
        "naturale", "diesis", "clavis_c", "clavis_f", "custos",
        "virgula", "punctum_morae", "ictus", "episema", "accentus",
        "accentus_reversus", "circulus", "semicirculus",
        "semicirculus_reversus" };

    si (   (s32)pars < ZEPHYRUM
        || (s32)pars >= (s32)NEUMAE_NUMERUS_PARTIUM)
    {
        redde "ignota";
    }
    redde tituli[pars];
}
