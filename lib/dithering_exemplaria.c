/* dithering_exemplaria.c - dithering per exemplaria (vide caput)
 *
 * Ortus: pericula 2026-10-13 (tabularium: desideratum ...GKGNJT8QM,
 * notae I-X); forma generationis VIII a Fran electa (decretum ...DVAB).
 * Constantae infra ex periculis mensuratae, ad imagines ~512 pixelorum
 * latas. Exemplaria eodem ordine bitorum ac exemplaria.h (bitus summus
 * 0x80 = x 0), ut pictogrammata eo migrare possint.
 */
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "dithering_exemplaria.h"
#include "color.h"


/* ============================================================
 * Constantae (ex periculis)
 * ============================================================ */

#define TEGULA               VIII   /* latus exemplaris */
#define GRADUS_MIXTI         XXXII  /* quantizatio ponderis obscurati */
#define GRADUS_AREAE         II     /* basis: electio per areas 2x2 */
#define AREA_HACHURARUM      IV     /* par hachurarum per areas 4x4 */
#define BAYER_NUMERUS        XVI
#define CANDIDATI_MAX        XII

#define SIGMA_OCULI          1.0    /* oculus basis */
#define SIGMA_OCULI_LATI     2.0    /* oculus regionum et hachurarum */
#define SIGMA_STRUCTURAE       2.0    /* classes detalii */
#define SIGMA_CAMPI          4.0    /* campus directionis hachurarum */
#define SIGMA_UMBRAE         3.5    /* charta umbrae aspersionis */

#define ENERGIA_INFIMA       0.05   /* infra: planum */
#define ENERGIA_SUMMA        0.10   /* supra: detalium */
#define COHAERENTIA_LINEAE   0.50
#define LIMEN_PLANI          0.25   /* limina residui Atkinson */
#define LIMEN_LINEAE         0.08
#define LIMEN_TEXTURAE       0.0
#define POENA_CONTRASTUS    0.004  /* poena paris longinqui in basi */

#define HACHURAE_ENERGIA_INFIMA  0.03
#define HACHURAE_ENERGIA_SUMMA   0.06
#define HACHURAE_COHAERENTIA     0.35
#define HACHURAE_LUMEN_INFIMUM   0.30   /* toni medii soli */
#define HACHURAE_LUMEN_SUMMUM    0.85
#define HACHURAE_AREA_MINIMA     CCC
#define HACHURAE_LIMEN           0.30
#define HACHURAE_PERIODUS        4.0
#define HACHURAE_ITERATIONES     VIII
#define HACHURAE_DISTANTIA_MIN   0.15
#define ANGULI_NUCLEORUM     XXXII

#define PICTOGRAMMATUM_AREA_MINIMA     CD
#define PICTOGRAMMATUM_DISTANTIA_MIN   0.12
#define PICTOGRAMMATUM_FASCIA          0.06   /* latitudo fasciae L intra colorem */
#define PICTOGRAMMATUM_UMBRA           0.35   /* regio obscurior: par submissum */
#define PICTOGRAMMATUM_DISTANTIA_UMBRAE 0.04
#define PICTOGRAMMATUM_POENA_UMBRAE    4.0    /* poena chromatis in umbra */
#define PICTOGRAMMATUM_POENA_COLORIS   1.0    /* fidelitas coloris in lucibus */
#define PICTOGRAMMATUM_LIMEN           0.30

#define PI_VALOR             3.141592653589793


/* ============================================================
 * Typi interni
 * ============================================================ */

nomen structura {
     i8 ordo[TEGULA];
    s32 densitas;
     i8 pondus[TEGULA * TEGULA];       /* oculo SIGMA_OCULI, 0..GRADUS_MIXTI */
     i8 pondus_latum[TEGULA * TEGULA]; /* oculo SIGMA_OCULI_LATI */
    DitheringPictogramma familia;      /* NULLUM pro Bayer */
} Exemplar;

nomen structura {
    f64 energia;      /* sqrt(trace J) */
    f64 cohaerentia;  /* ((l1 - l2) / (l1 + l2))^2 */
    f64 angulus;      /* directio lineae, radiani [0, pi) */
} Structura;

nomen structura {
    f64 energia;
    s32 index;
} RegioOrdo;

nomen structura {
         s32  lat;
         s32  alt;
         s32  n;
         s32  activi[XVI];
         s32  numerus_activorum;
         f64  pal_lin[XVI][III];
         f64  pal_lab[XVI][III];
         f64  distantia[XVI][XVI];
         f64* mixtio;                 /* [a][b][w][III] */
         i32  semen;
     Piscina* piscina;
} Opus;

nomen structura {
    DitheringPictogramma  familia;
      constans character* lineae[TEGULA];
} PictogrammaFons;

hic_manens constans s32 BAYER[TEGULA][TEGULA] = {
    { 0, 32,  8, 40,  2, 34, 10, 42},
    {48, 16, 56, 24, 50, 18, 58, 26},
    {12, 44,  4, 36, 14, 46,  6, 38},
    {60, 28, 52, 20, 62, 30, 54, 22},
    { 3, 35, 11, 43,  1, 33,  9, 41},
    {51, 19, 59, 27, 49, 17, 57, 25},
    {15, 47,  7, 39, 13, 45,  5, 37},
    {63, 31, 55, 23, 61, 29, 53, 21}
};

#define PICTOGRAMMATA_FONTES XI

hic_manens constans PictogrammaFons PICTOGRAMMATA[PICTOGRAMMATA_FONTES] =
    {
    {DITHERING_PICTOGRAMMA_LATERES,
     {"########", "#.......", "#.......", "#.......", "########",
         "....#...", "....#...", "....#..."}},
    {DITHERING_PICTOGRAMMA_SQUAMAE,
     {"#......#", ".#....#.", "..####..", "........", "...##...",
         "..#..#..", "##....##", "........"}},
    {DITHERING_PICTOGRAMMA_UNDAE,
     {"........", "##....##", "..#..#..", "...##...", "........",
         "........", "........", "........"}},
    {DITHERING_PICTOGRAMMA_UNDAE,
     {"........", "##....##", "..#..#..", "...##...", "........",
         "##....##", "..#..#..", "...##..."}},
    {DITHERING_PICTOGRAMMA_RHOMBI,
     {"#.......", ".#.....#", "..#...#.", "...#.#..", "....#...",
         "...#.#..", "..#...#.", ".#.....#"}},
    {DITHERING_PICTOGRAMMA_TEXTUM,
     {"#####.#.", "....#.#.", "#####.#.", "....#.#.", "#.#.####",
         "#.#.....", "#.#.####", "#.#....."}},
    {DITHERING_PICTOGRAMMA_CRUCES,
     {"........", "..#.....", ".###....", "..#.....", "........",
         "......#.", ".....###", "......#."}},
    {DITHERING_PICTOGRAMMA_HERBA,
     {"........", "#.#.#...", ".###....", "..#.....", "........",
         "#...#.#.", ".....###", "......#."}},
    {DITHERING_PICTOGRAMMA_SAGITTAE,
     {"#......#", ".#....#.", "..#..#..", "...##...", "#......#",
         ".#....#.", "..#..#..", "...##..."}},
    {DITHERING_PICTOGRAMMA_ANULI,
     {"........", "..###...", ".#...#..", ".#...#..", ".#...#..",
         "..###...", "........", "........"}},
    {DITHERING_PICTOGRAMMA_PUNCTA,
     {"........", "........", "..##....", "..##....", "........",
         "........", "......##", "......##"}}
};


/* ============================================================
 * Auxiliares parvi
 * ============================================================ */

interior f64
_d2 (
    constans f64* a,
    constans f64* b)
{
    f64 x = a[0] - b[0];
    f64 y = a[1] - b[1];
    f64 z = a[2] - b[2];
    redde x * x + y * y + z * z;
}

interior f64
_gradus_lenis (
    f64 a,
    f64 b,
    f64 x)
{
    f64 t = (x - a) / (b - a);
    si (t < 0.0) t = 0.0;
    si (t > 1.0) t = 1.0;
    redde t * t * (3.0 - 2.0 * t);
}

interior s32
_punctum (
    constans i8* ordo,
            s32  x,
            s32  y)
{
    redde (s32)((ordo[y & VII] >> (VII - (x & VII))) & I);
}

/* strepitus albus determinatus per pixelum */
interior f64
_strepitus (
    s32 x,
    s32 y,
    i32 semen)
{
    i32 h = ((i32)x * 73856093u) ^ ((i32)y * 19349663u)
        ^ (semen * 83492791u);
    h ^= h >> XIII;
    h *= 0x5bd1e995u;
    h ^= h >> XV;
    redde (f64)(h & 0xFFFFFFu) / 16777216.0;
}

interior f64*
_mixtio (
     constans Opus* o,
               s32  a,
               s32  b,
               s32  w)
{
    redde &o->mixtio[(((a * XVI) + b) * (GRADUS_MIXTI + I) + w) * III];
}

/* colores activi ordine distantiae ad lab; reddit numerum (<= maximum) */
interior s32
_proximi (
    constans Opus* o,
     constans f64* lab,
              s32  maximum,
              s32* ordinati)
{
    f64 dist[XVI];
    s32 i, j, numerus = o->numerus_activorum;

    per (i = 0; i < numerus; i++)
    {
        ordinati[i]  = o->activi[i];
        dist[i]      = _d2(lab, o->pal_lab[o->activi[i]]);
    }
    per (i = 1; i < numerus; i++)
    {
        per (j = i; j > 0 && dist[j] < dist[j - I]; j--)
        {
            f64 td           = dist[j];
            s32 ti           = ordinati[j];
            dist[j]          = dist[j - I];
            ordinati[j]      = ordinati[j - I];
            dist[j - I]      = td;
            ordinati[j - I]  = ti;
        }
    }
    redde numerus < maximum ? numerus : maximum;
}

interior s32
_proximus (
    constans Opus* o,
     constans f64* lab)
{
    s32 ordinati[XVI];
    _proximi(o, lab, I, ordinati);
    redde ordinati[0];
}

interior integer
_comparare_regiones (
    constans vacuum* a,
    constans vacuum* b)
{
    f64 x = ((constans RegioOrdo*)a)->energia;
    f64 y = ((constans RegioOrdo*)b)->energia;
    si (x < y) redde -1;
    si (x > y) redde 1;
    /* aequales: ordo stabilis per indicem (qsort non stabilis) */
    redde ((constans RegioOrdo*)a)->index
        < ((constans RegioOrdo*)b)->index ? -1 : 1;
}


/* ============================================================
 * Obscuratio Gaussiana separabilis (limites fixi)
 * ============================================================ */

interior vacuum
_gauss (
    constans f64* in,
             f64* out,
             s32  lat,
             s32  alt,
             s32  canales,
             f64  sigma,
             f64* tmp)
{
    f64 nucleus[XXXIII];
    s32 radius = (s32)ceil(sigma * 2.5), x, y, k, c, i;
    f64 summa = 0.0;

    si (radius > XVI) radius = XVI;
    per (i = -radius; i <= radius; i++)
    {
        nucleus[i + radius] = exp(-(f64)(i * i)
            / (2.0 * sigma * sigma));
        summa += nucleus[i + radius];
    }
    per (i = 0; i <= 2 * radius; i++) nucleus[i] /= summa;

    per (y = 0; y < alt; y++) per (x = 0; x < lat; x++) per (c = 0; c
                                                            < canales; c++)
                                                        {
                f64 s = 0.0;
                per (k = -radius; k <= radius; k++)
                {
                    s32 sx = x + k < 0 ? 0 : x + k >= lat ? lat - I : x
                        + k;
                    s += nucleus[k + radius] * in[(y * lat
                        + sx) * canales + c];
                }
                tmp[(y * lat + x) * canales + c] = s;
                                                        }
    per (y = 0; y < alt; y++) per (x = 0; x < lat; x++) per (c = 0; c
                                                            < canales; c++)
                                                        {
                f64 s = 0.0;
                per (k = -radius; k <= radius; k++)
                {
                    s32 sy = y + k < 0 ? 0 : y + k >= alt ? alt - I : y
                        + k;
                    s += nucleus[k + radius] * tmp[(sy * lat
                        + x) * canales + c];
                }
                out[(y * lat + x) * canales + c] = s;
                                                        }
}


/* ============================================================
 * Praeparatio: palette, mixtiones, exemplaria
 * ============================================================ */

interior b32
_opus_parare (
                 Opus* o,
                  s32  lat,
                  s32  alt,
         constans b32* colores_activi,
                  i32  semen,
              Piscina* piscina)
{
    s32 a, b, w, k;
    f64 lin[III];

    o->lat                = lat;
    o->alt                = alt;
    o->n                  = lat * alt;
    o->semen              = semen;
    o->piscina            = piscina;
    o->numerus_activorum  = 0;
    per (a = 0; a < XVI; a++)
    {
        si (colores_activi[a]) o->activi[o->numerus_activorum++] = a;
        per (k = 0; k < III; k++) o->pal_lin[a][k] =
                                      color_octetus_ad_linearem(AQUINAS_PALETTE[a][k]);
        color_linearis_ad_oklab(o->pal_lin[a], o->pal_lab[a]);
    }
    si (o->numerus_activorum == 0) redde FALSUM;

    o->mixtio = (f64*)piscina_allocare(piscina,
        (memoriae_index)(XVI * XVI * (GRADUS_MIXTI
            + I) * III) * magnitudo(f64));
    si (o->mixtio == NIHIL) redde FALSUM;
    per (a = 0; a < XVI; a++)
    {
        per (b = 0; b < XVI; b++)
        {
            o->distantia[a][b] = color_oklab_distantia(o->pal_lab[a],
                o->pal_lab[b]);
            per (w = 0; w <= GRADUS_MIXTI; w++)
            {
                per (k = 0; k < III; k++)
                {
                    lin[k] = o->pal_lin[a][k] * (f64)(GRADUS_MIXTI - w)
                        / GRADUS_MIXTI
                           + o->pal_lin[b][k] * (f64)w / GRADUS_MIXTI;
                }
                color_linearis_ad_oklab(lin, _mixtio(o, a, b, w));
            }
        }
    }
    redde VERUM;
}

/* pondera: Gaussiana in toro (exemplar totam imaginem tegit) */
interior vacuum
_exemplar_finire (
                Exemplar* e,
             constans i8* ordo,
    DitheringPictogramma  familia)
{
    s32 x, y, dx, dy;

    memcpy(e->ordo, ordo, TEGULA);
    e->familia   = familia;
    e->densitas  = 0;
    per (y = 0; y < TEGULA; y++)
    {
        per (x = 0; x < TEGULA; x++)
        {
            f64 summa = 0.0, pondera = 0.0, summa_lata = 0.0,
                pondera_lata = 0.0;
            e->densitas += _punctum(ordo, x, y);
            per (dy = -VI; dy <= VI; dy++)
            {
                per (dx = -VI; dx <= VI; dx++)
                {
                    f64 g = exp(-(f64)(dx * dx + dy * dy)
                        / (2.0 * SIGMA_OCULI * SIGMA_OCULI));
                    f64 gl = exp(-(f64)(dx * dx + dy * dy)
                        / (2.0 * SIGMA_OCULI_LATI * SIGMA_OCULI_LATI));
                    f64 valor = (f64)_punctum(ordo, x + dx + LXIV, y
                        + dy + LXIV);
                    summa         += g * valor;
                    pondera       += g;
                    summa_lata    += gl * valor;
                    pondera_lata  += gl;
                }
            }
            e->pondus[y * TEGULA + x] = (i8)(s32)(summa
                / pondera * GRADUS_MIXTI + 0.5);
            e->pondus_latum[y * TEGULA + x] = (i8)(s32)(summa_lata
                / pondera_lata * GRADUS_MIXTI + 0.5);
        }
    }
}

/* Bayer: densitas k/64, k = 0 (solidum) .. 60 */
interior vacuum
_bayer_parare (
    Exemplar* bayer)
{
    s32 k, x, y;
    i8 ordo[TEGULA];

    per (k = 0; k < BAYER_NUMERUS; k++)
    {
        memset(ordo, 0, TEGULA);
        per (y = 0; y < TEGULA; y++) per (x = 0; x
                                         < TEGULA; x++) si (BAYER[y][x]
                                                            < k * IV) ordo[y] =
                                                                          (i8)(ordo[y]
                                                                              | (0x80
                                                                                                >> x));
        _exemplar_finire(&bayer[k], ordo, DITHERING_PICTOGRAMMA_NULLUM);
    }
}

interior vacuum
_pictogrammata_parare (
    Exemplar* figurae)
{
    s32 i, x, y;
    i8 ordo[TEGULA];

    per (i = 0; i < PICTOGRAMMATA_FONTES; i++)
    {
        memset(ordo, 0, TEGULA);
        per (y = 0; y < TEGULA; y++) per (x = 0; x
                                         < TEGULA; x++) si (PICTOGRAMMATA[i].lineae[y][x]
                                                            == '#') ordo[y] =
                                                                                                                (i8)(ordo[y]
                                                                                                      | (0x80
                                                                        >> x));
        _exemplar_finire(&figurae[i], ordo, PICTOGRAMMATA[i].familia);
    }
}


/* ============================================================
 * Tensor structurae super L
 * ============================================================ */

interior vacuum
_structuram_computare (
    constans Opus* o,
     constans f64* lab,
              f64  sigma,
        Structura* st)
{
    s32 lat = o->lat, alt = o->alt, x, y;
    memoriae_index  m    = (memoriae_index)o->n * III * magnitudo(f64);
               f64* j    = (f64*)piscina_allocare(o->piscina,
                   m);
               f64* js   = (f64*)piscina_allocare(o->piscina,
                   m);
               f64* tmp  = (f64*)piscina_allocare(o->piscina,
                   m);

    per (y = 0; y < alt; y++)
    {
        per (x = 0; x < lat; x++)
        {
            s32 x_ante = x > 0 ? x - I : 0, x_post = x + I < lat ? x
                + I : x;
            s32 y_ante = y > 0 ? y - I : 0, y_post = y + I < alt ? y
                + I : y;
            f64 a00 = lab[(y_ante * lat + x_ante) * III], a01 =
                lab[(y_ante * lat + x) * III], a02 = lab[(y_ante * lat
                    + x_post) * III];
            f64 a10 = lab[(y * lat + x_ante) * III],
                a12 = lab[(y * lat + x_post) * III];
            f64 a20 = lab[(y_post * lat + x_ante) * III], a21 =
                lab[(y_post * lat + x) * III], a22 = lab[(y_post * lat
                    + x_post) * III];
            f64 gradiens_x = (a02 + 2.0 * a12 + a22 - a00 - 2.0 * a10
                - a20) / 8.0;
            f64 gradiens_y = (a20 + 2.0 * a21 + a22 - a00 - 2.0 * a01
                - a02) / 8.0;
            s32 p           = y * lat + x;
            j[p * III + 0]  = gradiens_x * gradiens_x;
            j[p * III + 1]  = gradiens_y * gradiens_y;
            j[p * III + 2]  = gradiens_x * gradiens_y;
        }
    }
    _gauss(j, js, lat, alt, III, sigma, tmp);
    per (x = 0; x < o->n; x++)
    {
        f64 a = js[x * III + 0], b = js[x * III + 1], c = js[x * III
            + 2];
        f64 tr   = a + b;
        f64 di   = sqrt((a - b) * (a - b) + 4.0 * c * c);
        f64 psi  = 0.5 * atan2(2.0 * c, a - b) + PI_VALOR / 2.0;

        si (psi < 0.0) psi += PI_VALOR;
        si (psi >= PI_VALOR) psi -= PI_VALOR;
        st[x].energia      = sqrt(tr);
        st[x].cohaerentia  = tr > 1e-12 ? (di / tr) * (di / tr) : 0.0;
        st[x].angulus      = psi;
    }
}


/* ============================================================
 * Stratum I: basis Bayer per areas 2x2
 * ============================================================ */

interior vacuum
_basis_bayer (
        constans Opus* o,
         constans f64* lin,
         constans f64* lab_oculi,
    constans Exemplar* bayer,
                   i8* indices)
{
    s32 lat = o->lat, alt = o->alt, x_areae, y_areae;

    per (y_areae = 0; y_areae < alt; y_areae += GRADUS_AREAE)
    {
        per (x_areae = 0; x_areae < lat; x_areae += GRADUS_AREAE)
        {
            s32 al = x_areae + GRADUS_AREAE <= lat ? GRADUS_AREAE : lat
                - x_areae;
            s32 aa = y_areae + GRADUS_AREAE <= alt ? GRADUS_AREAE : alt
                - y_areae;
            s32 area = al * aa, i, j, k, e, nc;
            f64 medium_lin[III] = {0.0, 0.0, 0.0};
            f64 medium_lab[III];
            s32 cand[CANDIDATI_MAX];
            s32 ordinati[XVI];
            s32 pixelum_obscurum = 0, pixelum_lucidum = 0;
            f64 lumen_minimum = 9.0, lumen_maximum = -9.0, optimum =
                0.0;
            s32 opt_a, opt_b, opt_e = 0;

            per (i = 0; i < area; i++)
            {
                s32 p = (y_areae + i / al) * lat + x_areae + i % al;
                per (k = 0; k < III; k++) medium_lin[k] += lin[p * III
                                              + k];
                si (lab_oculi[p * III] < lumen_minimum)
                { lumen_minimum = lab_oculi[p * III]; pixelum_obscurum =
                                                          p;
                }
                si (lab_oculi[p * III] > lumen_maximum)
                { lumen_maximum = lab_oculi[p * III]; pixelum_lucidum =
                                                          p;
                }
            }
            per (k = 0; k < III; k++) medium_lin[k] /= (f64)area;
            color_linearis_ad_oklab(medium_lin, medium_lab);

            /* candidati: VI proximi medio + II proximi extremis utrisque
             * (_proximi ordinem TOTUM scribit - in XVI, deinde copia) */
            nc = _proximi(o, medium_lab, VI, ordinati);
            per (i = 0; i < nc; i++) cand[i] = ordinati[i];
            per (j = 0; j < II; j++)
            {
                s32 nx = _proximi(o, &lab_oculi[(j
                    == 0 ? pixelum_obscurum : pixelum_lucidum) * III],
                    II, ordinati);
                per (i = 0; i < nx; i++)
                {
                    b32 adest = FALSUM;
                    per (k = 0; k < nc; k++) si (cand[k]
                                                 == ordinati[i]) adest =
                                                                     VERUM;
                    si (   !adest
                        && nc < CANDIDATI_MAX) cand[nc++] = ordinati[i];
                }
            }

            /* initium: solidum proximi (etiam si color unus activus) */
            opt_a = cand[0];
            opt_b = cand[0];
            per (i = 0; i < area; i++)
            {
                s32 p    = (y_areae + i / al) * lat + x_areae + i % al;
                optimum  += _d2(o->pal_lab[opt_a], &lab_oculi[p * III]);
            }

            per (i = 0; i < nc; i++)
            {
                s32 a = cand[i];
                per (j = 0; j < nc; j++)
                {
                    s32 b = cand[j];
                    si (a == b) perge;
                    per (e = 0; e < BAYER_NUMERUS; e++)
                    {
                        constans Exemplar* ex = &bayer[e];
                                      s32  minor = ex->densitas < 64
                                          - ex->densitas ? ex->densitas : 64
                                          - ex->densitas;
                        f64 err =
                            POENA_CONTRASTUS * (f64)area * o->distantia[a][b] * o->distantia[a][b]
                                * (f64)minor / 32.0;
                        s32 q;
                        per (q = 0; q < area && err < optimum; q++)
                        {
                            s32 sx = x_areae + q % al, sy = y_areae + q
                                / al;
                            err += _d2(_mixtio(o, a, b,
                                (s32)ex->pondus[(sy & VII) * TEGULA
                                    + (sx & VII)]),
                                       &lab_oculi[(sy * lat
                                           + sx) * III]);
                        }
                        si (err < optimum)
                        { optimum = err; opt_a = a; opt_b = b; opt_e =
                                                                   e;
                        }
                    }
                }
            }

            per (i = 0; i < area; i++)
            {
                s32 sx = x_areae + i % al, sy = y_areae + i / al;
                indices[sy * lat + sx] =
                    (i8)(_punctum(bayer[opt_e].ordo, sx,
                    sy) ? opt_b : opt_a);
            }
        }
    }
}


/* ============================================================
 * Stratum V: residuum Atkinson cum liminibus per pixelum
 * ============================================================ */

interior s32
_residuum (
    constans Opus* o,
     constans f64* lin,
      constans i8* basis,
     constans f64* limina,
              f64* error,
               i8* indices)
{
    hic_manens constans s32 VICINI[VI][II] = {
        {1, 0}, {2, 0}, {-1, 1}, {0, 1}, {1, 1}, {0, 2}
    };
    s32 lat = o->lat, alt = o->alt, x, y, k, nn, mutati = 0;

    memset(error, 0, (memoriae_index)o->n * III * magnitudo(f64));
    per (y = 0; y < alt; y++)
    {
        per (x = 0; x < lat; x++)
        {
            s32 p = y * lat + x;
            f64 t[III], lab[III], e[III];
            s32 b = (s32)basis[p], proximus, electus;

            per (k = 0; k < III; k++) t[k] = lin[p * III + k]
                                          + error[p * III + k];
            color_linearis_ad_oklab(t, lab);
            proximus = _proximus(o, lab);
            electus = (sqrt(_d2(lab, o->pal_lab[proximus])) + limina[p]
                < sqrt(_d2(lab, o->pal_lab[b])))
                    ? proximus : b;
            si (electus != b) mutati++;
            indices[p] = (i8)electus;
            per (k = 0; k < III; k++) e[k] = (t[k]
                                          - o->pal_lin[electus][k])
                                              / 8.0;
            per (nn = 0; nn < VI; nn++)
            {
                s32 nx = x + VICINI[nn][0], ny = y + VICINI[nn][1];
                si (nx < 0 || nx >= lat || ny >= alt) perge;
                per (k = 0; k < III; k++) error[(ny * lat + nx) * III
                                              + k] += e[k];
            }
        }
    }
    redde mutati;
}


/* ============================================================
 * Regiones connexae (communia hachuris et pictographicis)
 * ============================================================ */

/* Regiones 4-connexae ubi titulus[p] aequalis et eligibilis[p];
 * area < area_min omittuntur. membra contigua per regionem in
 * 'membra'; reddit numerum regionum. */
interior s32
_regiones_colligere (
    constans Opus* o,
     constans s32* titulus,
     constans b32* eligibilis,
              s32  area_min,
              s32* membra,
              s32* initium,
              s32* numerus_membrorum)
{
    s32* regio = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)o->n * magnitudo(s32));
    s32* acervus = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)o->n * magnitudo(s32));
    s32 lat = o->lat, alt = o->alt, p, nr = 0, positus = 0;

    per (p = 0; p < o->n; p++) regio[p] = -1;
    per (p = 0; p < o->n; p++)
    {
        s32 vertex = 0, nm = 0;
        si (!eligibilis[p] || regio[p] >= 0) perge;
        acervus[vertex++]  = p;
        regio[p]           = p;
        dum (vertex > 0)
        {
            s32 q = acervus[--vertex], qx = q % lat, qy = q / lat, d;
            membra[positus + nm++] = q;
            per (d = 0; d < IV; d++)
            {
                s32 rx = qx + (d == 0) - (d == 1), ry = qy + (d == 2)
                    - (d == 3), t;
                si (rx < 0 || ry < 0 || rx >= lat || ry >= alt) perge;
                t = ry * lat + rx;
                si (   regio[t]   >= 0 || !eligibilis[t]
                    || titulus[t] != titulus[p]) perge;
                regio[t]           = p;
                acervus[vertex++]  = t;
            }
        }
        si (nm < area_min) perge;
        initium[nr]            = positus;
        numerus_membrorum[nr]  = nm;
        nr++;
        positus += nm;
    }
    redde nr;
}


/* ============================================================
 * Stratum II: hachurae curvae
 * ============================================================ */

/* regiones hachuratae MAXIMAE primum dum praesupputatio non exceditur;
 * ceterae basi redduntur */
interior vacuum
_hachuras_eligere (
    constans Opus* o,
              b32* hachuratus,
              f64  praesupputatio,
              f64* tegmen,
              i32* numerus)
{
    memoriae_index  n = (memoriae_index)o->n;
               s32* titulus = (s32*)piscina_allocare(o->piscina,
                   n * magnitudo(s32));
    s32* membra = (s32*)piscina_allocare(o->piscina,
        n * magnitudo(s32));
    s32* initium = (s32*)piscina_allocare(o->piscina,
        n * magnitudo(s32));
    s32* nm = (s32*)piscina_allocare(o->piscina,
        n * magnitudo(s32));
    b32* servatus = (b32*)piscina_allocare(o->piscina,
        n * magnitudo(b32));
    RegioOrdo* ordo = (RegioOrdo*)piscina_allocare(o->piscina,
        n * magnitudo(RegioOrdo));
    f64 limes = praesupputatio * (f64)o->n, usum = 0.0;
    s32 p, r, i, nr;

    *numerus = 0;
    per (p = 0; p < o->n; p++)
    { titulus[p] = 0; servatus[p] = FALSUM;
    }
    nr = _regiones_colligere(o, titulus, hachuratus,
        HACHURAE_AREA_MINIMA, membra, initium, nm);
    per (r = 0; r < nr; r++)
    { ordo[r].energia = -(f64)nm[r]; ordo[r].index = r;
    }
    qsort(ordo, (memoriae_index)nr, magnitudo(RegioOrdo),
        _comparare_regiones);
    per (r = 0; r < nr; r++)
    {
        s32 ri = ordo[r].index;
        si (usum + (f64)nm[ri] > limes) perge;
        per (i = 0; i < nm[ri]; i++) servatus[membra[initium[ri] + i]] =
                                         VERUM;
        usum += (f64)nm[ri];
        (*numerus)++;
    }
    per (p = 0; p < o->n; p++) hachuratus[p] = servatus[p];
    *tegmen = usum / (f64)o->n;
}

/* Campus striarum: filtrum Gabor orientatum iteratum super strepitum
 * (more emendationis digitorum); reddit campum ~sinusoidalem [-1, 1] */
interior vacuum
_campum_striarum (
         constans Opus* o,
    constans Structura* st_campi,
          constans b32* activus,
                   f64* campus)
{
    s32 lat = o->lat, alt = o->alt;
    s32 radius = (s32)ceil(HACHURAE_PERIODUS * 1.25) + I;
    s32 latus = 2 * radius + I, k, dx, dy, p, it, x, y;
    f64* nuclei = (f64*)piscina_allocare(o->piscina,
        (memoriae_index)(ANGULI_NUCLEORUM * latus * latus) * magnitudo(f64));
    f64* g = (f64*)piscina_allocare(o->piscina,
        (memoriae_index)o->n * magnitudo(f64));
    f64 su = HACHURAE_PERIODUS * 0.75, sv = HACHURAE_PERIODUS * 0.5;

    per (k = 0; k < ANGULI_NUCLEORUM; k++)
    {
        f64 angulus_nuclei = PI_VALOR * (f64)k / ANGULI_NUCLEORUM;
        f64 ct = cos(angulus_nuclei), sn = sin(angulus_nuclei), se =
            0.0, sc = 0.0;
        f64* nu = &nuclei[k * latus * latus];
        per (dy = -radius; dy <= radius; dy++) per (dx = -radius; dx
                                                   <= radius; dx++)
                                               {
                f64 u = dx * ct + dy * sn, v = -dx * sn + dy * ct;
                f64 env = exp(-u * u / (2.0 * su * su) - v * v
                    / (2.0 * sv * sv));
                se += env;
                sc += env * cos(2.0 * PI_VALOR * v / HACHURAE_PERIODUS);
                                               }
        /* sine componente continua: campus non derivat */
        per (dy = -radius; dy <= radius; dy++) per (dx = -radius; dx
                                                   <= radius; dx++)
                                               {
                f64 u = dx * ct + dy * sn, v = -dx * sn + dy * ct;
                f64 env = exp(-u * u / (2.0 * su * su) - v * v
                    / (2.0 * sv * sv));
                nu[(dy + radius) * latus + dx + radius] =
                    env * (cos(2.0 * PI_VALOR * v / HACHURAE_PERIODUS)
                        - sc / se);
                                               }
    }

    per (p = 0; p < o->n; p++) campus[p] =
                                   activus[p] ? 2.0 * _strepitus(p
                                       % lat, p / lat,
                                   o->semen + I) - 1.0 : 0.0;
    per (it = 0; it < HACHURAE_ITERATIONES; it++)
    {
        f64 sq = 0.0, media_quadratica;
        s32 na = 0;
        per (y = 0; y < alt; y++)
        {
            per (x = 0; x < lat; x++)
            {
                         s32  q = y * lat + x;
                         f64  s = 0.0;
                constans f64* nu;
                si (!activus[q])
                { g[q] = 0.0; perge;
                }
                k = (s32)floor(st_campi[q].angulus
                    / PI_VALOR * ANGULI_NUCLEORUM + 0.5)
                    % ANGULI_NUCLEORUM;
                nu = &nuclei[k * latus * latus];
                per (dy = -radius; dy <= radius; dy++)
                {
                    s32 sy = y + dy < 0 ? 0 : y + dy >= alt ? alt
                        - I : y + dy;
                    per (dx = -radius; dx <= radius; dx++)
                    {
                        s32 sx = x + dx < 0 ? 0 : x + dx >= lat ? lat
                            - I : x + dx;
                        s += nu[(dy + radius) * latus + dx
                            + radius] * campus[sy * lat + sx];
                    }
                }
                g[q]  = s;
                sq    += s * s;
                na++;
            }
        }
        media_quadratica = na > 0 ? sqrt(sq / na) : 1.0;
        si (media_quadratica < 1e-12) media_quadratica = 1.0;
        per (p = 0; p < o->n; p++) campus[p] = it + I
                                       < HACHURAE_ITERATIONES ? tanh(1.5 * g[p]
                                       / media_quadratica) : g[p]
                                       / (1.41421356 * media_quadratica);
    }
    per (p = 0; p < o->n; p++)
    {
        si (campus[p] > 1.0) campus[p] = 1.0;
        si (campus[p] < -1.0) campus[p] = -1.0;
    }
}

/* per areas 4x4: par (atramentum obscurius, charta) + tegmen per pixelum
 * ex tono; atramentum ubi campus > cos(pi * tegmen) */
interior vacuum
_hachuras_pingere (
    constans Opus* o,
     constans f64* lab_oculi_lati,
     constans f64* campus,
     constans b32* hachuratus,
               i8* indices)
{
    s32 lat = o->lat, alt = o->alt, x_areae, y_areae;
    s32 tegmen[AREA_HACHURARUM * AREA_HACHURARUM];
    s32 tegmen_opt[AREA_HACHURARUM * AREA_HACHURARUM];

    per (y_areae = 0; y_areae < alt; y_areae += AREA_HACHURARUM)
    {
        per (x_areae = 0; x_areae < lat; x_areae += AREA_HACHURARUM)
        {
            s32 al = x_areae + AREA_HACHURARUM
                <= lat ? AREA_HACHURARUM : lat - x_areae;
            s32 aa = y_areae + AREA_HACHURARUM
                <= alt ? AREA_HACHURARUM : alt - y_areae;
            f64 medium[III] = {0.0, 0.0, 0.0};
            s32 ordinati[XVI], na = 0, nc, i, j, q, w;
            f64 optimum = 1e30;
            s32 opt_a = -1, opt_b = -1;

            per (q = 0; q < al * aa; q++)
            {
                s32 p = (y_areae + q / al) * lat + x_areae + q % al;
                si (!hachuratus[p]) perge;
                per (i = 0; i
                    < III; i++) medium[i] += lab_oculi_lati[p * III
                                    + i];
                na++;
            }
            si (na == 0) perge;
            per (i = 0; i < III; i++) medium[i] /= (f64)na;
            nc = _proximi(o, medium, VIII, ordinati);

            per (i = 0; i < nc; i++)
            {
                per (j = 0; j < nc; j++)
                {
                    s32 a = ordinati[i], b = ordinati[j];   /* a atramentum, b charta */
                    f64 err = 0.0;
                    si (a == b || o->pal_lab[a][0] >= o->pal_lab[b][0]
                        || o->distantia[a][b]
                            < HACHURAE_DISTANTIA_MIN) perge;
                    per (q = 0; q < al * aa && err < optimum; q++)
                    {
                        s32 p = (y_areae + q / al) * lat + x_areae + q
                            % al;
                        f64 dm = 1e30;
                        si (!hachuratus[p]) perge;
                        per (w = 0; w <= GRADUS_MIXTI; w++)
                        {
                            f64 d = _d2(_mixtio(o, b, a, w),
                                &lab_oculi_lati[p * III]);
                            si (d < dm)
                            { dm = d; tegmen[q] = w;
                            }
                        }
                        err += dm;
                    }
                    si (err < optimum)
                    {
                        optimum = err; opt_a = a; opt_b = b;
                        memcpy(tegmen_opt, tegmen, magnitudo(tegmen));
                    }
                }
            }
            si (opt_a < 0) perge;
            per (q = 0; q < al * aa; q++)
            {
                s32 p = (y_areae + q / al) * lat + x_areae + q % al;
                si (!hachuratus[p]) perge;
                indices[p] = (i8)(campus[p]
                    > cos(PI_VALOR * (f64)tegmen_opt[q]
                        / GRADUS_MIXTI) ? opt_a : opt_b);
            }
        }
    }
}


/* ============================================================
 * Stratum III: pictographica per regiones
 * ============================================================ */

/* Regiones eiusdem coloris proximi et fasciae L (4-connexae), area >=
 * PICTOGRAMMATUM_AREA_MINIMA, ordine energiae mediae ascendente; quaeque UNO
 * pictogrammate familiae materiae et UNO pari (colorem regionis
 * continente) impletur dum pixela stilizata praesupputationem non
 * excedunt. Regio quae excederet omittitur; minores adhuc intrant. */
interior vacuum
_regiones_stilizare (
    constans Opus*      o,
    constans f64*       lin,
    constans f64*       lab_oculi_lati,
    constans Structura* st,
    constans b32*       eligibilis,
    constans DitheringPictogramma* materia,
    constans Exemplar*  figurae,
              f64       praesupputatio,
               i8*      indices,
              b32*      larva,
               i8*      atramentum,
               i8*      charta,
              s32*      regio_pixeli,  /* ordinalis regionis stilizatae aut -1 */
              f64*      tegmen,
              i32*      numerus)
{
    memoriae_index  n = (memoriae_index)o->n;
               s32* titulus = (s32*)piscina_allocare(o->piscina,
                   n * magnitudo(s32));
    s32* membra = (s32*)piscina_allocare(o->piscina,
        n * magnitudo(s32));
    s32* initium = (s32*)piscina_allocare(o->piscina,
        n * magnitudo(s32));
    s32* nm = (s32*)piscina_allocare(o->piscina,
        n * magnitudo(s32));
    RegioOrdo* ordo = (RegioOrdo*)piscina_allocare(o->piscina,
        n * magnitudo(RegioOrdo));
    f64 limes = praesupputatio * (f64)o->n, usum = 0.0;
    s32 p, r, nr, numerus_ordinis = 0;

    *numerus = 0;
    per (p = 0; p < o->n; p++)
    {
        titulus[p] = _proximus(o, &lab_oculi_lati[p * III]) * LXIV
                   + (s32)(lab_oculi_lati[p * III]
                       / PICTOGRAMMATUM_FASCIA);
        larva[p]         = FALSUM;
        regio_pixeli[p]  = -1;
    }
    nr = _regiones_colligere(o, titulus, eligibilis,
        PICTOGRAMMATUM_AREA_MINIMA, membra, initium, nm);
    per (r = 0; r < nr; r++)
    {
        s32 col = titulus[membra[initium[r]]] / LXIV, i;
        f64 energia = 0.0;
        si (materia[col] == DITHERING_PICTOGRAMMA_NULLUM) perge;
        per (i = 0; i < nm[r]; i++) energia += st[membra[initium[r]
                                        + i]].energia;
        ordo[numerus_ordinis].energia  = energia / (f64)nm[r];
        ordo[numerus_ordinis].index    = r;
        numerus_ordinis++;
    }
    qsort(ordo, (memoriae_index)numerus_ordinis, magnitudo(RegioOrdo),
        _comparare_regiones);

    per (r = 0; r < numerus_ordinis; r++)
    {
        s32 ri = ordo[r].index, numerus_r = nm[ri];
        constans s32* mb               = &membra[initium[ri]];
                 s32  col              = titulus[mb[0]] / LXIV;
                 f64  medium_lin[III]  = {0.0, 0.0, 0.0};
                 f64  medium_lab[III];
        s32 ordinati[XVI], nc, i, j, k, e;
        f64 optimum = 1e30, distantia_minima;
        s32 opt_a = -1, opt_b = -1, opt_e = -1;
        b32 umbra;

        si (usum + (f64)numerus_r > limes) perge;
        per (i = 0; i < numerus_r; i++) per (k = 0; k
                                            < III; k++) medium_lin[k] += lin[mb[i] * III
                                                                      + k];
        per (k = 0; k < III; k++) medium_lin[k] /= (f64)numerus_r;
        color_linearis_ad_oklab(medium_lin, medium_lab);
        umbra = medium_lab[0] < PICTOGRAMMATUM_UMBRA;
        distantia_minima =
            umbra ? PICTOGRAMMATUM_DISTANTIA_UMBRAE : PICTOGRAMMATUM_DISTANTIA_MIN;
        nc = _proximi(o, medium_lab, IX, ordinati);

        per (e = 0; e < PICTOGRAMMATA_FONTES; e++)
        {
            si (figurae[e].familia != materia[col]) perge;
            per (i = 0; i < nc; i++)
            {
                per (j = 0; j < nc; j++)
                {
                    s32 a = ordinati[i], b = ordinati[j], q;
                    s32 alter = a == col ? b : a;
                    f64 err;
                    si (   a == b || (a != col && b != col)
                        || o->distantia[a][b] < distantia_minima) perge;
                    si (umbra)
                    {
                        f64 chroma_quadratum =
                            o->pal_lab[alter][1] * o->pal_lab[alter][1]
                                    + o->pal_lab[alter][2] * o->pal_lab[alter][2];
                        err =
                            (f64)numerus_r * PICTOGRAMMATUM_POENA_UMBRAE * chroma_quadratum;
                    }
                    alioquin
                    {
                        f64 da = o->pal_lab[alter][1] - medium_lab[1];
                        f64 db = o->pal_lab[alter][2] - medium_lab[2];
                        err =
                            (f64)numerus_r * PICTOGRAMMATUM_POENA_COLORIS * (da * da
                                + db * db);
                    }
                    per (q = 0; q < numerus_r && err < optimum; q++)
                    {
                        s32 m = mb[q];
                        err += _d2(_mixtio(o, a, b,
                            (s32)figurae[e].pondus_latum[((m
                                / o->lat) & VII) * TEGULA + ((m
                                % o->lat) & VII)]),
                                   &lab_oculi_lati[m * III]);
                    }
                    si (err < optimum)
                    { optimum = err; opt_a = a; opt_b = b; opt_e = e;
                    }
                }
            }
        }
        si (opt_e < 0) perge;
        per (i = 0; i < numerus_r; i++)
        {
            s32 m = mb[i];
            indices[m] = (i8)(_punctum(figurae[opt_e].ordo, m % o->lat,
                m / o->lat) ? opt_b : opt_a);
            larva[m]         = VERUM;
            atramentum[m]    = (i8)opt_b;
            charta[m]        = (i8)opt_a;
            regio_pixeli[m]  = (s32)*numerus;
        }
        usum += (f64)numerus_r;
        (*numerus)++;
    }
    *tegmen = usum / (f64)o->n;
}


/* ============================================================
 * Stratum IV: aspersio (situla pigmenti variae pressionis)
 * ============================================================ */

/* Umbra amissa = L fontis minus L redditionis (ambo oculo lato), MINUS
 * media eius per regionem (tonus regionis planus consulto est: aspersio
 * variationem umbrae reddit, non tonum totum corrigit - aliter exemplar
 * dissolvit), deinde late obscurata INTRA regiones (per larvam
 * normalizata). Tegmen =
 * pressio * |differentia| / (L lucidior - L obscurior paris), <= tectum.
 * Obscurare: color obscurior in pixela lucidiora; lucidare: contra. */
interior s32
_aspergere (
    constans Opus* o,
      constans i8* basis,
     constans b32* larva,
     constans s32* regio_pixeli,
              s32  numerus_regionum,
      constans i8* atramentum,
      constans i8* charta,
     constans f64* lab_oculi_lati,
              f64  pressio,
              f64  tectum,
               i8* exitus)
{
    memoriae_index m3 = (memoriae_index)o->n * III * magnitudo(f64);
    memoriae_index m2 = (memoriae_index)o->n * II * magnitudo(f64);
    f64* r = (f64*)piscina_allocare(o->piscina, m3);
    f64* rb = (f64*)piscina_allocare(o->piscina, m3);
    f64* tmp = (f64*)piscina_allocare(o->piscina, m3);
    f64* differentia = (f64*)piscina_allocare(o->piscina, m2);
    f64* differentia_lata = (f64*)piscina_allocare(o->piscina, m2);
    f64* summa = (f64*)piscina_allocare(o->piscina,
        (memoriae_index)(numerus_regionum + I) * magnitudo(f64));
    s32* numeri = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)(numerus_regionum + I) * magnitudo(s32));
    s32 p, k, mutata = 0;

    memcpy(exitus, basis, (memoriae_index)o->n);
    per (p = 0; p < o->n; p++) per (k = 0; k < III; k++) r[p * III
                                                             + k] =
                                                             o->pal_lin[(s32)basis[p]][k];
    _gauss(r, rb, o->lat, o->alt, III, SIGMA_OCULI_LATI, tmp);
    per (p = 0; p < o->n; p++)
    {
        f64 lab[III];
        color_linearis_ad_oklab(&rb[p * III], lab);
        differentia[p * II] = larva[p] ? (lab_oculi_lati[p * III]
            - lab[0]) : 0.0;
        differentia[p * II + I] = larva[p] ? 1.0 : 0.0;
    }
    per (k = 0; k < numerus_regionum; k++)
    { summa[k] = 0.0; numeri[k] = 0;
    }
    per (p = 0; p < o->n; p++)
    {
        si (regio_pixeli[p] < 0) perge;
        summa[regio_pixeli[p]] += differentia[p * II];
        numeri[regio_pixeli[p]]++;
    }
    per (p = 0; p < o->n; p++)
    {
        si (regio_pixeli[p] < 0) perge;
        differentia[p * II] -= summa[regio_pixeli[p]]
            / (f64)numeri[regio_pixeli[p]];
    }
    _gauss(differentia, differentia_lata, o->lat, o->alt, II,
        SIGMA_UMBRAE, tmp);

    per (p = 0; p < o->n; p++)
    {
        s32 obscurus, lucidus;
        f64 differentia_loci, dl, tegmen;

        si (!larva[p] || differentia_lata[p * II + I] < 1e-6) perge;
        differentia_loci = differentia_lata[p * II]
            / differentia_lata[p * II + I];
        si (o->pal_lab[(s32)atramentum[p]][0]
            < o->pal_lab[(s32)charta[p]][0])
        {
            obscurus  = (s32)atramentum[p];
            lucidus   = (s32)charta[p];
        }
        alioquin
        {
            obscurus  = (s32)charta[p];
            lucidus   = (s32)atramentum[p];
        }
        dl = o->pal_lab[lucidus][0] - o->pal_lab[obscurus][0];
        si (dl < 0.02) perge;
        tegmen = pressio * (differentia_loci
            < 0.0 ? -differentia_loci : differentia_loci) / dl;
        si (tegmen > tectum) tegmen = tectum;
        si (_strepitus(p % o->lat, p / o->lat, o->semen)
            >= tegmen) perge;
        si (differentia_loci < 0.0 && (s32)basis[p] == lucidus)
        { exitus[p] = (i8)obscurus; mutata++;
        }
        alioquin si (   differentia_loci > 0.0
                     && (s32)basis[p] == obscurus)
        { exitus[p] = (i8)lucidus; mutata++;
        }
    }
    redde mutata;
}


/* ============================================================
 * Functiones publicae
 * ============================================================ */

vacuum
dithering_exemplaria_optiones_ordinariae (
    DitheringExemplariaOptiones* optiones)
{
    si (optiones == NIHIL) redde;
    optiones->praesupputatio_pictographica = 0.30;
    optiones->praesupputatio_hachurarum = 0.15;
    optiones->pressio_aspersionis = 2.0;
    optiones->tectum_aspersionis = 0.50;
    optiones->semen = 12345u;
    optiones->materia[0] = DITHERING_PICTOGRAMMA_NULLUM;     /* niger */
    optiones->materia[I] = DITHERING_PICTOGRAMMA_SQUAMAE;    /* griseus obscurus (oliva) */
    optiones->materia[II] = DITHERING_PICTOGRAMMA_SAGITTAE;   /* malva */
    optiones->materia[III] = DITHERING_PICTOGRAMMA_TEXTUM;     /* griseus medius */
    optiones->materia[IV] = DITHERING_PICTOGRAMMA_PUNCTA;     /* griseus lucidus */
    optiones->materia[V] = DITHERING_PICTOGRAMMA_NULLUM;     /* albus */
    optiones->materia[VI] = DITHERING_PICTOGRAMMA_LATERES;    /* ruber obscurus */
    optiones->materia[VII] = DITHERING_PICTOGRAMMA_UNDAE;      /* caeruleus */
    optiones->materia[VIII] = DITHERING_PICTOGRAMMA_CRUCES;     /* roseus */
    optiones->materia[IX] = DITHERING_PICTOGRAMMA_RHOMBI;     /* aurum obscurum */
    optiones->materia[X] = DITHERING_PICTOGRAMMA_RHOMBI;     /* aurum medium */
    optiones->materia[XI] = DITHERING_PICTOGRAMMA_ANULI;      /* aurum lucidum */
    optiones->materia[XII] = DITHERING_PICTOGRAMMA_SQUAMAE;    /* folium obscurum */
    optiones->materia[XIII] = DITHERING_PICTOGRAMMA_SQUAMAE;    /* folium medium */
    optiones->materia[XIV] = DITHERING_PICTOGRAMMA_HERBA;      /* folium lucidum */
    optiones->materia[XV] = DITHERING_PICTOGRAMMA_TEXTUM;     /* griseus calidus */
}

DitheringFructus
dithering_exemplaria_colorum (
                             constans i8* rgba,
                                     i32  latitudo,
                                     i32  altitudo,
                            constans b32  colores_activi[XVI],
    constans DitheringExemplariaOptiones* optiones,
               DitheringExemplariaCensus* census,
                                 Piscina* piscina)
{
                        DitheringFructus  fructus;
             DitheringExemplariaOptiones  ordinariae;
    constans DitheringExemplariaOptiones* opt = optiones;
                                    Opus  o;
                                Exemplar  bayer[BAYER_NUMERUS];
                                Exemplar  figurae[PICTOGRAMMATA_FONTES];
    memoriae_index m1, m3;
    f64 *lin, *lab, *lab_oculi, *lab_oculi_lati, *error, *tmp, *limina,
        *campus;
    b32 *hachuratus, *activus, *eligibilis, *larva;
    s32* regio_pixeli;
    i8 *basis, *pictura_hachurarum, *pictura_stili, *atramentum,
        *charta, *sparsum;
    Structura *st, *st_campi;
    f64 tegmen_h = 0.0, tegmen_p = 0.0;
    i32 regiones_h = 0, regiones_p = 0;
    s32 p, k, aspersa = 0, stilizata = 0;

    fructus.successus  = FALSUM;
    fructus.indices    = NIHIL;
    fructus.latitudo   = latitudo;
    fructus.altitudo   = altitudo;
    si (census != NIHIL) memset(census, 0, magnitudo(*census));

    si (   rgba == NIHIL || piscina == NIHIL || colores_activi == NIHIL
        || latitudo == 0 || altitudo == 0)
    {
        redde fructus;
    }
    si (opt == NIHIL)
    {
        dithering_exemplaria_optiones_ordinariae(&ordinariae);
        opt = &ordinariae;
    }
    si (!_opus_parare(&o, (s32)latitudo, (s32)altitudo, colores_activi,
        opt->semen, piscina))
    {
        redde fructus;
    }
    _bayer_parare(bayer);
    _pictogrammata_parare(figurae);

    m1 = (memoriae_index)o.n;
    m3 = (memoriae_index)o.n * III * magnitudo(f64);
    lin = (f64*)piscina_allocare(piscina, m3);
    lab = (f64*)piscina_allocare(piscina, m3);
    lab_oculi = (f64*)piscina_allocare(piscina, m3);
    lab_oculi_lati = (f64*)piscina_allocare(piscina, m3);
    error = (f64*)piscina_allocare(piscina, m3);
    tmp = (f64*)piscina_allocare(piscina, m3);
    limina = (f64*)piscina_allocare(piscina, m1 * magnitudo(f64));
    campus = (f64*)piscina_allocare(piscina, m1 * magnitudo(f64));
    hachuratus = (b32*)piscina_allocare(piscina, m1 * magnitudo(b32));
    activus = (b32*)piscina_allocare(piscina, m1 * magnitudo(b32));
    eligibilis = (b32*)piscina_allocare(piscina, m1 * magnitudo(b32));
    larva = (b32*)piscina_allocare(piscina, m1 * magnitudo(b32));
    regio_pixeli = (s32*)piscina_allocare(piscina, m1 * magnitudo(s32));
    basis = (i8*)piscina_allocare(piscina, m1);
    pictura_hachurarum = (i8*)piscina_allocare(piscina, m1);
    pictura_stili = (i8*)piscina_allocare(piscina, m1);
    atramentum = (i8*)piscina_allocare(piscina, m1);
    charta = (i8*)piscina_allocare(piscina, m1);
    sparsum = (i8*)piscina_allocare(piscina, m1);
    st = (Structura*)piscina_allocare(piscina,
        m1 * magnitudo(Structura));
    st_campi = (Structura*)piscina_allocare(piscina,
        m1 * magnitudo(Structura));
    fructus.indices = (i8*)piscina_allocare(piscina, m1);
    si (   lin == NIHIL || lab == NIHIL || lab_oculi == NIHIL
        || lab_oculi_lati == NIHIL
        || error == NIHIL || tmp == NIHIL || limina == NIHIL
        || campus == NIHIL
        || hachuratus == NIHIL || activus == NIHIL
        || eligibilis == NIHIL || larva == NIHIL
        || regio_pixeli == NIHIL
        || basis == NIHIL || pictura_hachurarum == NIHIL
        || pictura_stili == NIHIL || atramentum == NIHIL
        || charta == NIHIL || sparsum == NIHIL || st == NIHIL
        || st_campi == NIHIL
        || fructus.indices == NIHIL)
    {
        fructus.indices = NIHIL;
        redde fructus;
    }

    /* lux linearis, OKLab, oculi duo */
    per (p = 0; p < o.n; p++)
    {
        per (k = 0; k < III; k++) lin[p * III + k] =
                                      color_octetus_ad_linearem(rgba[p * IV
                                      + k]);
        color_linearis_ad_oklab(&lin[p * III], &lab[p * III]);
    }
    _gauss(lin, tmp, o.lat, o.alt, III, SIGMA_OCULI, error);
    per (p = 0; p < o.n; p++) color_linearis_ad_oklab(&tmp[p * III],
                                  &lab_oculi[p * III]);
    _gauss(lin, tmp, o.lat, o.alt, III, SIGMA_OCULI_LATI, error);
    per (p = 0; p < o.n; p++) color_linearis_ad_oklab(&tmp[p * III],
                                  &lab_oculi_lati[p * III]);

    /* classes detalii et campus hachurarum */
    _structuram_computare(&o, lab, SIGMA_STRUCTURAE, st);
    _structuram_computare(&o, lab, SIGMA_CAMPI, st_campi);
    per (p = 0; p < o.n; p++)
    {
        f64 d = _gradus_lenis(ENERGIA_INFIMA, ENERGIA_SUMMA,
            st[p].energia);
        f64 cw = _gradus_lenis(COHAERENTIA_LINEAE - 0.15,
            COHAERENTIA_LINEAE + 0.15, st[p].cohaerentia);
        f64 h  = _gradus_lenis(HACHURAE_ENERGIA_INFIMA,
            HACHURAE_ENERGIA_SUMMA, st_campi[p].energia)
               * _gradus_lenis(HACHURAE_COHAERENTIA - 0.1,
               HACHURAE_COHAERENTIA + 0.1, st_campi[p].cohaerentia);
        f64 lumen = lab_oculi_lati[p * III];
        limina[p] = LIMEN_PLANI * (1.0 - d) + d * (cw * LIMEN_LINEAE
            + (1.0 - cw) * LIMEN_TEXTURAE);
        hachuratus[p] = opt->praesupputatio_hachurarum > 0.0 && h > 0.5
                     && lumen > HACHURAE_LUMEN_INFIMUM
                     && lumen < HACHURAE_LUMEN_SUMMUM;
        activus[p]    = h > 0.2;
    }

    /* I basis */
    _basis_bayer(&o, lin, lab_oculi, bayer, basis);

    /* II hachurae: regiones maximae sub praesupputatione */
    _hachuras_eligere(&o, hachuratus, opt->praesupputatio_hachurarum,
        &tegmen_h, &regiones_h);
    memcpy(pictura_hachurarum, basis, m1);
    si (regiones_h > 0)
    {
        _campum_striarum(&o, st_campi, activus, campus);
        _hachuras_pingere(&o, lab_oculi_lati, campus, hachuratus,
            pictura_hachurarum);
    }

    /* III pictographica extra hachuras */
    per (p = 0; p < o.n; p++) eligibilis[p] = !hachuratus[p];
    memcpy(pictura_stili, basis, m1);
    si (opt->praesupputatio_pictographica > 0.0)
    {
        _regiones_stilizare(&o, lin, lab_oculi_lati, st, eligibilis,
            opt->materia, figurae,
                            opt->praesupputatio_pictographica,
                            pictura_stili, larva, atramentum, charta,
                            regio_pixeli, &tegmen_p, &regiones_p);
    }
    alioquin
    {
        per (p = 0; p < o.n; p++)
        { larva[p] = FALSUM; regio_pixeli[p] = -1;
        }
    }

    /* compositio + limina stratorum */
    per (p = 0; p < o.n; p++)
    {
        si (hachuratus[p])
        {
            basis[p]   = pictura_hachurarum[p];
            limina[p]  = HACHURAE_LIMEN;
        }
        alioquin si (larva[p])
        {
            basis[p]   = pictura_stili[p];
            limina[p]  = PICTOGRAMMATUM_LIMEN;
            stilizata++;
        }
    }

    /* IV aspersio */
    si (opt->pressio_aspersionis > 0.0 && regiones_p > 0)
    {
        aspersa = _aspergere(&o, basis, larva, regio_pixeli,
            (s32)regiones_p, atramentum, charta, lab_oculi_lati,
                             opt->pressio_aspersionis,
                             opt->tectum_aspersionis, sparsum);
        memcpy(basis, sparsum, m1);
    }

    /* V residuum */
    _residuum(&o, lin, basis, limina, error, fructus.indices);

    si (census != NIHIL)
    {
        census->tegmen_hachurarum      = tegmen_h;
        census->tegmen_pictographicum  = tegmen_p;
        census->tegmen_aspersionis = stilizata > 0 ? (f64)aspersa
            / (f64)stilizata : 0.0;
        census->regiones_hachuratae      = regiones_h;
        census->regiones_pictographicae  = regiones_p;
    }
    fructus.successus = VERUM;
    redde fructus;
}
