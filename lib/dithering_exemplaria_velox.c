/* dithering_exemplaria_velox.c - dithering per exemplaria, velox
 * (vide caput)
 *
 * Ortus (exemplaria-velox T1, 2026-11-07): copia
 * lib/dithering_exemplaria.c, nomine publico mutato; optiones
 * ordinariae classici adhibentur, non copiantur. Gradatim
 * reaedificatur (project-specs/exemplaria-velox-plan.md); classicus
 * intactus manet.
 *
 * T3: plana f32 per canalem (Plana, Structura), larvae ut bitsets,
 * OKLab f32 sine pow (_radix_cubica), mixtio paribus activis solis.
 * Mathesis eadem; output a classico pixelatim differre potest.
 *
 * T5: _proximus minimum simplex, _proximi_k electio partialis; basis
 * tabulas Bayer ipsas legit, putatio exacta (output idem octetim ac
 * T4).
 *
 * T6: campus striarum - nucleus per pixelum semel, columnae interiores
 * sine limitibus, tanh solum activis; tensor detalii sine angulo.
 *
 * T7 (ex recensione, exacta): basis post strata II et III areas pictas
 * omittit; limes capsulae per par; terminus primus sine ramis; nebula
 * ordinum per ordinem totum; campus striarum quaterni pixela.
 */
#include "dithering_exemplaria.h"
#include "dithering_exemplaria_fontes.h"
#include "dithering_exemplaria_tabulae.h"
#include "dithering_exemplaria_velox.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>


/* ==================================================
 * Constantae (ex periculis)
 * ================================================== */

#define GRADUS_AREAE         II     /* basis: electio per areas 2x2 */
#define AREA_HACHURARUM      IV     /* par hachurarum per areas 4x4 */
#define CANDIDATI_MAX        XII

#define SIGMA_STRUCTURAE     2.0    /* classes detalii */
#define SIGMA_CAMPI          4.0    /* campus directionis hachurarum */
#define SIGMA_UMBRAE         3.5    /* charta umbrae aspersionis */

#define ENERGIA_INFIMA       0.05   /* infra: planum */
#define ENERGIA_SUMMA        0.10   /* supra: detalium */
#define COHAERENTIA_LINEAE   0.50
#define LIMEN_PLANI          0.25   /* limina residui Atkinson */
#define LIMEN_LINEAE         0.08
#define LIMEN_TEXTURAE       0.0
#define POENA_CONTRASTUS     0.004  /* poena paris longinqui in basi */

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

#define PICTOGRAMMATUM_AREA_MINIMA      CD
#define PICTOGRAMMATUM_DISTANTIA_MIN    0.12
#define PICTOGRAMMATUM_FASCIA           0.06  /* fascia L in colore */
#define PICTOGRAMMATUM_UMBRA            0.35  /* obscurior: submissum */
#define PICTOGRAMMATUM_DISTANTIA_UMBRAE 0.04
#define PICTOGRAMMATUM_POENA_UMBRAE     4.0   /* chroma in umbra */
#define PICTOGRAMMATUM_POENA_COLORIS    1.0   /* fidelitas coloris */
#define PICTOGRAMMATUM_LIMEN            0.30

#define PI_VALOR             3.141592653589793


/* ==================================================
 * Typi interni
 * ================================================== */

nomen structura {
                      i8 ordo[DITHERING_TEGULA];
                     s32 densitas;
                      i8 pondus[LXIV];        /* oculus */
                      i8 pondus_latum[LXIV];  /* oculus latus */
    DitheringPictogramma familia;  /* NULLUM pro Bayer */
} Exemplar;

/* Plana: imago per canales, f32, unum planum per canalem (n = lat *
 * alt) - lin r/g/b aut L/a/b; ansa quaeque sola plana quae legit
 * tangit (basis L oculi solum pro minimo/maximo) */
nomen structura {
    f32* c[III];
} Plana;

/* tensor structurae per plana */
nomen structura {
    f32* energia;      /* sqrt(trace J) */
    f32* cohaerentia;  /* ((l1 - l2) / (l1 + l2))^2 */
    f32* angulus;      /* directio lineae, radiani [0, pi); NIHIL =
                        * non computatur */
} Structura;

nomen structura {
    f32 energia;
    s32 index;
} RegioOrdo;

nomen structura {
         s32  lat;
         s32  alt;
         s32  n;
         s32  activi[XVI];
         s32  numerus_activorum;
         i32  larva_activorum;      /* bit k: color k activus */
         f32  pal_lin[XVI][III];
         f32  pal_lab[XVI][III];
         f32  distantia[XVI][XVI];
         f32* mixtio;   /* [a][b][w][III], paria activa sola scripta */
    /* capsula paris (paria activa): segmentum mixtio(w 0) ->
     * mixtio(w XXXII) et radius quo omnes XXXIII mixtiones
     * continentur (paulo auctus) - terminus inferior basis */
         f64  capsula_origo[XVI][XVI][III];
         f64  capsula_via[XVI][XVI][III];
         f64  capsula_inversa[XVI][XVI];  /* 1 / |via|^2, 0 si nulla */
         f64  capsula_radius[XVI][XVI];
         i32  semen;
     Piscina* piscina;
} Opus;


/* ==================================================
 * Auxiliares parvi
 * ================================================== */

/* larvae: bitset, verba i32, bit p & XXXI verbi p >> V */
#define VERBA_LARVAE(n)  (((n) + XXXI) >> V)

interior b32
_bit (
     constans i32* larva,
              s32  p)
{
    redde (b32)((larva[p >> V] >> (p & XXXI)) & I);
}

interior vacuum
_bit_ponere (
    i32* larva,
    s32  p)
{
    larva[p >> V] |= (i32)I << (p & XXXI);
}

/* radix cubica f32 sine pow: conjectura ex bitis (divisio exponentis
 * per III), deinde II gradus Newtoni y -= (y*y*y - x) / (3*y*y);
 * x <= 0 -> 0. Error relativus < 1e-5 (et supra 1: residuum lucem
 * > 1 dare potest) */
interior f32
_radix_cubica (
    f32 x)
{
    i32 bits;
    f32 y;

    si (x <= 0.0f) redde 0.0f;
    memcpy(&bits, &x, magnitudo(bits));
    bits = bits / III + 709921077u;
    memcpy(&y, &bits, magnitudo(y));
    y -= (y * y * y - x) / (3.0f * y * y);
    y -= (y * y * y - x) / (3.0f * y * y);
    redde y;
}

/* OKLab ex luce lineari (matrices color.c), f32; negativa -> 0 */
interior vacuum
_oklab (
    f32 r,
    f32 g,
    f32 b,
    f32 lab[III])
{
    f32 l, m, s;

    si (r < 0.0f) r = 0.0f;
    si (g < 0.0f) g = 0.0f;
    si (b < 0.0f) b = 0.0f;
    l = _radix_cubica(0.4122214708f * r + 0.5363325363f * g
        + 0.0514459929f * b);
    m = _radix_cubica(0.2119034982f * r + 0.6806995451f * g
        + 0.1073969566f * b);
    s = _radix_cubica(0.0883024619f * r + 0.2817188376f * g
        + 0.6299787005f * b);
    lab[0] = 0.2104542553f * l + 0.7936177850f * m - 0.0040720468f * s;
    lab[1] = 1.9779984951f * l - 2.4285922050f * m + 0.4505937099f * s;
    lab[2] = 0.0259040371f * l + 0.7827717662f * m - 0.8086757660f * s;
}

/* OKLab planorum: lab->c[k][p] ex lin->c[.][p] */
interior vacuum
_oklab_plana (
    constans Plana* lin,
             Plana* lab,
               s32  n)
{
    f32 t[III];
    s32 p;

    per (p = 0; p < n; p++)
    {
        _oklab(lin->c[0][p], lin->c[1][p], lin->c[2][p], t);
        lab->c[0][p] = t[0];
        lab->c[1][p] = t[1];
        lab->c[2][p] = t[2];
    }
}

interior f32
_d2 (
    constans f32* a,
    constans f32* b)
{
    f32 x = a[0] - b[0];
    f32 y = a[1] - b[1];
    f32 z = a[2] - b[2];
    redde x * x + y * y + z * z;
}

/* distantia quadrata tripli a pixelo p planorum */
interior f32
_d2_plani (
      constans f32* a,
    constans Plana* pl,
               s32  p)
{
    f32 x = a[0] - pl->c[0][p];
    f32 y = a[1] - pl->c[1][p];
    f32 z = a[2] - pl->c[2][p];
    redde x * x + y * y + z * z;
}

interior vacuum
_triplum (
    constans Plana* pl,
               s32  p,
               f32  t[III])
{
    t[0] = pl->c[0][p];
    t[1] = pl->c[1][p];
    t[2] = pl->c[2][p];
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

interior f32*
_mixtio (
    constans Opus* o,
              s32  a,
              s32  b,
              s32  w)
{
    redde &o->mixtio[(((a * XVI) + b) * (DITHERING_GRADUS_MIXTI + I)
        + w) * III];
}

/* k colores activi proximi lab, ordine distantiae: electio partialis
 * (k transitus "primum minimum reliquorum"). Paritas -> index minor
 * (ordo activi), ut ordinatio per insertionem stabilis. Scribit et
 * reddit min(k, numerus activorum). */
interior s32
_proximi_k (
    constans Opus* o,
     constans f32* lab,
              s32  k,
              s32* indices)
{
    f32 dist[XVI];
    i32 sumpti = 0;
    s32 i, r, numerus = o->numerus_activorum;
    s32 m = numerus < k ? numerus : k;

    per (i = 0; i < numerus; i++)
    {
        dist[i] = _d2(lab, o->pal_lab[o->activi[i]]);
    }
    per (r = 0; r < m; r++)
    {
        s32 optimus = -1;
        per (i = 0; i < numerus; i++)
        {
            si ((sumpti >> i) & I) perge;
            si (optimus < 0 || dist[i] < dist[optimus]) optimus = i;
        }
        sumpti      |= (i32)I << optimus;
        indices[r]  = o->activi[optimus];
    }
    redde m;
}

/* color activus proximus: minimum simplex, paritas -> index minor */
interior s32
_proximus (
    constans Opus* o,
     constans f32* lab)
{
    f32 optimum = _d2(lab, o->pal_lab[o->activi[0]]);
    s32 optimus = o->activi[0], i;

    per (i = 1; i < o->numerus_activorum; i++)
    {
        f32 d = _d2(lab, o->pal_lab[o->activi[i]]);
        si (d < optimum)
        {
            optimum = d;
            optimus = o->activi[i];
        }
    }
    redde optimus;
}

interior integer
_comparare_regiones (
    constans vacuum* a,
    constans vacuum* b)
{
    f32 x = ((constans RegioOrdo*)a)->energia;
    f32 y = ((constans RegioOrdo*)b)->energia;
    si (x < y) redde -1;
    si (x > y) redde 1;
    /* aequales: ordo stabilis per indicem (qsort non stabilis) */
    redde ((constans RegioOrdo*)a)->index
        < ((constans RegioOrdo*)b)->index ? -1 : 1;
}

interior f32*
_planum (
     Piscina* piscina,
         s32  n)
{
    redde (f32*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(f32));
}

interior b32
_plana_allocare (
       Plana* pl,
     Piscina* piscina,
         s32  n)
{
    s32 k;

    per (k = 0; k < III; k++)
    {
        pl->c[k] = _planum(piscina, n);
        si (pl->c[k] == NIHIL) redde FALSUM;
    }
    redde VERUM;
}

/* larva nova, omnes bits nulli */
interior i32*
_larvam_allocare (
    Piscina* piscina,
         s32  n)
{
    memoriae_index  m;
               i32* larva;

    m      = magnitudo(i32) * (memoriae_index)VERBA_LARVAE(n);
    larva  = (i32*)piscina_allocare(piscina, m);

    si (larva != NIHIL) memset(larva, 0, m);
    redde larva;
}


/* ==================================================
 * Obscuratio Gaussiana separabilis (limites fixi)
 * ================================================== */

/* pixelum x ordinis cum limitibus fixis (ora) */
interior f32
_gauss_orae (
    constans f32* linea,
    constans f32* nucleus,
             s32  radius,
             s32  lat,
             s32  x)
{
    f32 s = 0.0f;
    s32 k;

    per (k = -radius; k <= radius; k++)
    {
        s32 sx  = x + k < 0 ? 0 : x + k >= lat ? lat - I : x + k;
        s       += nucleus[k + radius] * linea[sx];
    }
    redde s;
}

/* Gaussiana separabilis per planum, f32. Nucleus semel per
 * vocationem (radius ceil(2.5 sigma), <= XVI, normalizatus).
 * Ordines et columnae in oras (limites fixi, ut classicus) et
 * interius (sine limitibus, radius .. latus - radius) divisa; si
 * latus < 2 radius + 1, totum ora est. Summae eodem ordine ac ansa
 * simplex: exitus idem bit pro bit. meta == fons licet. */
interior vacuum
_gauss_planum (
    constans f32* fons,
             f32* meta,
             f32* tmp,
             s32  lat,
             s32  alt,
             f64  sigma)
{
    f64 pondera[XXXIII];
    f32 nucleus[XXXIII];
    s32 radius = (s32)ceil(sigma * 2.5), x, y, k, i;
    s32 initium, finis;
    f64 summa = 0.0;

    si (radius > XVI) radius = XVI;
    per (i = -radius; i <= radius; i++)
    {
        pondera[i + radius] = exp(-(f64)(i * i)
            / (2.0 * sigma * sigma));
        summa += pondera[i + radius];
    }
    per (i = 0; i <= 2 * radius; i++)
    {
        nucleus[i] = (f32)(pondera[i] / summa);
    }

    /* ordines: interius [initium, finis) sine limitibus */
    initium  = radius < lat ? radius : lat;
    finis    = lat - radius;
    si (finis < initium) finis = initium;
    per (y = 0; y < alt; y++)
    {
        constans f32* linea   = &fons[y * lat];
                 f32* exitus  = &tmp[y * lat];
        per (x = 0; x < initium; x++)
        {
            exitus[x] = _gauss_orae(linea, nucleus, radius, lat, x);
        }
        /* interius per ordinem totum, k exterius (ut columnae): ordo
         * summationis per pixelum idem (0, k crescens), sed pixela
         * independentia - vectorizari potest */
        per (x = initium; x < finis; x++) exitus[x] = 0.0f;
        per (k = -radius; k <= radius; k++)
        {
                     f32  pondus = nucleus[k + radius];
            constans f32* fons_k = &linea[k];
            per (x = initium; x < finis; x++)
            {
                exitus[x] += pondus * fons_k[x];
            }
        }
        per (x = finis; x < lat; x++)
        {
            exitus[x] = _gauss_orae(linea, nucleus, radius, lat, x);
        }
    }

    /* columnae: limes per ordinem (non per pixelum), ergo ordines
     * toti summantur - ora sine divisione */
    per (y = 0; y < alt; y++)
    {
        f32* exitus = &meta[y * lat];
        per (x = 0; x < lat; x++) exitus[x] = 0.0f;
        per (k = -radius; k <= radius; k++)
        {
                     s32  sy      = y + k;
                     f32  pondus  = nucleus[k + radius];
            constans f32* ordo;

            si (sy < 0) sy = 0;
            si (sy >= alt) sy = alt - I;
            ordo = &tmp[sy * lat];
            per (x = 0; x < lat; x++) exitus[x] += pondus * ordo[x];
        }
    }
}


/* ==================================================
 * Praeparatio: palette, mixtiones, exemplaria
 * ================================================== */

/* distantia puncti a segmento origo -> origo + via (inversa = 1 /
 * |via|^2; via nulla: distantia ab origine) */
interior f64
_distantia_segmenti (
     constans f64* punctum,
     constans f64* origo,
     constans f64* via,
              f64  inversa)
{
    f64 v[III], t = 0.0, d = 0.0;
    s32 k;

    per (k = 0; k < III; k++)
    {
        v[k]  = punctum[k] - origo[k];
        t     += v[k] * via[k];
    }
    t *= inversa;
    t = t < 0.0 ? 0.0 : t > 1.0 ? 1.0 : t;
    per (k = 0; k < III; k++)
    {
        f64 u  = v[k] - t * via[k];
        d      += u * u;
    }
    redde sqrt(d);
}

/* capsula paris ex mixtionibus f32 scriptis (f64 exacte lectis):
 * radius = distantia maxima a segmento * (1 + 1e-6) + 1e-6 - error
 * f64 (~1e-16) longe intra augmentum */
interior vacuum
_capsulam_parare (
    Opus* o,
     s32  a,
     s32  b)
{
    f64 longitudo = 0.0, radius = 0.0, punctum[III];
    s32 k, w;

    per (k = 0; k < III; k++)
    {
        o->capsula_origo[a][b][k] = (f64)_mixtio(o, a, b, 0)[k];
        o->capsula_via[a][b][k] = (f64)_mixtio(o, a, b,
            DITHERING_GRADUS_MIXTI)[k] - o->capsula_origo[a][b][k];
        longitudo += o->capsula_via[a][b][k]
            * o->capsula_via[a][b][k];
    }
    o->capsula_inversa[a][b] = longitudo > 0.0 ? 1.0 / longitudo : 0.0;
    per (w = 0; w <= DITHERING_GRADUS_MIXTI; w++)
    {
        f64 d;
        per (k = 0; k < III; k++)
        {
            punctum[k] = (f64)_mixtio(o, a, b, w)[k];
        }
        d = _distantia_segmenti(punctum, o->capsula_origo[a][b],
            o->capsula_via[a][b], o->capsula_inversa[a][b]);
        si (d > radius) radius = d;
    }
    o->capsula_radius[a][b] = radius * (1.0 + 1e-6) + 1e-6;
}

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
    f32 lin[III];

    o->lat                = lat;
    o->alt                = alt;
    o->n                  = lat * alt;
    o->semen              = semen;
    o->piscina            = piscina;
    o->numerus_activorum  = 0;
    o->larva_activorum    = 0;
    per (a = 0; a < XVI; a++)
    {
        si (colores_activi[a])
        {
            o->activi[o->numerus_activorum++]  = a;
            o->larva_activorum                 |= (i32)I << a;
        }
        per (k = 0; k < III; k++)
        {
            o->pal_lin[a][k] =
                DITHERING_SRGB_LINEARIS[AQUINAS_PALETTE[a][k]];
        }
        _oklab(o->pal_lin[a][0], o->pal_lin[a][1], o->pal_lin[a][2],
            o->pal_lab[a]);
    }
    si (o->numerus_activorum == 0) redde FALSUM;

    o->mixtio = (f32*)piscina_allocare(piscina,
        (memoriae_index)(XVI * XVI * (DITHERING_GRADUS_MIXTI + I) * III)
            * magnitudo(f32));
    si (o->mixtio == NIHIL) redde FALSUM;
    per (a = 0; a < XVI; a++)
    {
        per (b = 0; b < XVI; b++)
        {
            o->distantia[a][b] = (f32)sqrt((f64)_d2(o->pal_lab[a],
                o->pal_lab[b]));
            si (   !((o->larva_activorum >> a) & I)
                || !((o->larva_activorum >> b) & I)) perge;
            per (w = 0; w <= DITHERING_GRADUS_MIXTI; w++)
            {
                per (k = 0; k < III; k++)
                {
                    lin[k] = (f32)((f64)o->pal_lin[a][k]
                        * (f64)(DITHERING_GRADUS_MIXTI - w)
                        / DITHERING_GRADUS_MIXTI
                        + (f64)o->pal_lin[b][k] * (f64)w
                        / DITHERING_GRADUS_MIXTI);
                }
                _oklab(lin[0], lin[1], lin[2], _mixtio(o, a, b, w));
            }
            _capsulam_parare(o, a, b);
        }
    }
    redde VERUM;
}

/* pictogrammata ex tabulis generatis (dithering_exemplaria_tabulae.h,
 * valores classici _exemplar_finire): nihil hic computatur. Basis
 * Bayer tabulas ipsas legit (T5). */
interior vacuum
_exemplaria_ex_tabulis (
    Exemplar* figurae)
{
    s32 k;

    per (k = 0; k < DITHERING_PICTOGRAMMATA_FONTIUM; k++)
    {
        memcpy(figurae[k].ordo, DITHERING_PICTOGRAMMATUM_ORDINES[k],
            DITHERING_TEGULA);
        memcpy(figurae[k].pondus, DITHERING_PICTOGRAMMATUM_PONDERA[k],
            LXIV);
        memcpy(figurae[k].pondus_latum,
            DITHERING_PICTOGRAMMATUM_PONDERA_LATA[k], LXIV);
        figurae[k].densitas  = DITHERING_PICTOGRAMMATUM_DENSITATES[k];
        figurae[k].familia   = DITHERING_PICTOGRAMMATA[k].familia;
    }
}


/* ==================================================
 * Tensor structurae super L
 * ================================================== */

interior vacuum
_structuram_computare (
    constans Opus* o,
     constans f32* lumen,
              f64  sigma,
        Structura* st)
{
    s32 lat = o->lat, alt = o->alt, x, y;
    f32* quadratum_x  = _planum(o->piscina, o->n);
    f32* quadratum_y  = _planum(o->piscina, o->n);
    f32* productum    = _planum(o->piscina, o->n);
    f32* tmp          = _planum(o->piscina, o->n);

    per (y = 0; y < alt; y++)
    {
        s32 y_ante = y > 0 ? y - I : 0, y_post = y + I < alt ? y
            + I : y;
        constans f32* supra = &lumen[y_ante * lat];
        constans f32* media = &lumen[y * lat];
        constans f32* infra = &lumen[y_post * lat];
        per (x = 0; x < lat; x++)
        {
            s32 x_ante = x > 0 ? x - I : 0, x_post = x + I < lat ? x
                + I : x;
            f32 gradiens_x = (supra[x_post] + 2.0f * media[x_post]
                + infra[x_post] - supra[x_ante]
                - 2.0f * media[x_ante] - infra[x_ante]) / 8.0f;
            f32 gradiens_y = (infra[x_ante] + 2.0f * infra[x]
                + infra[x_post] - supra[x_ante]
                - 2.0f * supra[x] - supra[x_post]) / 8.0f;
            s32 p           = y * lat + x;
            quadratum_x[p]  = gradiens_x * gradiens_x;
            quadratum_y[p]  = gradiens_y * gradiens_y;
            productum[p]    = gradiens_x * gradiens_y;
        }
    }
    _gauss_planum(quadratum_x, quadratum_x, tmp, lat, alt, sigma);
    _gauss_planum(quadratum_y, quadratum_y, tmp, lat, alt, sigma);
    _gauss_planum(productum, productum, tmp, lat, alt, sigma);
    per (x = 0; x < o->n; x++)
    {
        f64 a = (f64)quadratum_x[x], b = (f64)quadratum_y[x], c =
            (f64)productum[x];
        f64 tr = a + b;
        f64 di = sqrt((a - b) * (a - b) + 4.0 * c * c);

        st->energia[x]      = (f32)sqrt(tr);
        st->cohaerentia[x]  = (f32)(tr > 1e-12 ? (di / tr) * (di / tr)
            : 0.0);
        si (st->angulus != NIHIL)
        {
            f64 psi = 0.5 * atan2(2.0 * c, a - b) + PI_VALOR / 2.0;
            si (psi < 0.0) psi += PI_VALOR;
            si (psi >= PI_VALOR) psi -= PI_VALOR;
            st->angulus[x] = (f32)psi;
        }
    }
}


/* ==================================================
 * Stratum I: basis Bayer per areas 2x2
 * ================================================== */

/* Termini inferiores parium areae (omnes, sine ramis). Mixtiones paris
 * omnes intra capsulam (segmentum + radius); distantia a capsula
 * convexa est, ergo pro omni gradu e >= 1:
 *   err_e >= poena_e + area * (dist(medium, segmentum) - radius)_+^2.
 * Diminutio: d2 f32 computata >= vera * (1 - 3e-7), summa f64 ~5e-16;
 * poena minima * (1 - 1e-12) + limes * (1 - 1e-5) ergo stricte infra
 * omnem err_e computatum - par omissum vincere non potuit. */
interior vacuum
_limites_parium (
     constans Opus* o,
      constans s32* cand,
               s32  nc,
      constans f64* medium,
               s32  area,
               s32  minor_minimus,
               f64  limites[CANDIDATI_MAX][CANDIDATI_MAX])
{
    s32 i, j;

    per (i = 0; i < nc; i++)
    {
        per (j = 0; j < nc; j++)
        {
            s32 a = cand[i], b = cand[j];
            f64 d = _distantia_segmenti(medium, o->capsula_origo[a][b],
                o->capsula_via[a][b], o->capsula_inversa[a][b])
                - o->capsula_radius[a][b];
            f64 poena = POENA_CONTRASTUS * (f64)area
                * o->distantia[a][b] * o->distantia[a][b];

            d = d > 0.0 ? d : 0.0;
            limites[i][j] = poena * (f64)minor_minimus / 32.0
                * (1.0 - 1e-12) + (f64)area * d * d * (1.0 - 1e-5);
        }
    }
}

/* Par optimum (a, b, gradus) per aream, in opt_a / opt_b / opt_e
 * separatis: clavis una (a | b << IV | gradus << VIII) basim LX %
 * tardiorem fecit (T5, mensum; vide worklog). Pondera ex
 * DITHERING_BAYER_PONDERA directe. Areae totae in tectum (pictae ab
 * hachuris aut pictographicis) omittuntur: compositio eas superscribit.
 * Putatio exacta (nihil putatum stricte melius esse potest, ergo
 * electio eadem):
 *   - gradus 0 (densitas 0, DENSITATES[0] == 0, pondera omnia 0) est
 *     solidum a pro omni b: semel per a aestimatur, loco suo primo;
 *   - gradus 1..XV paris omittuntur cum limes paris (_limites_parium:
 *     capsula, poena minima) iam >= optimum;
 *   - terminus primus graduum 1..XV sine ramis (post_primum), deinde
 *     soli gradus sub optimo ambulantur. */
interior vacuum
_basis_bayer (
     constans Opus* o,
    constans Plana* lin,
    constans Plana* oculi,
      constans i32* tectum,
                i8* indices)
{
    s32 lat = o->lat, alt = o->alt, x_areae, y_areae, e;
    s32 minores[DITHERING_BAYER_NUMERUS], minor_minimus = LXIV;

    per (e = 0; e < DITHERING_BAYER_NUMERUS; e++)
    {
        s32 densitas  = DITHERING_BAYER_DENSITATES[e];
        minores[e]    = densitas < XXXII ? densitas : LXIV - densitas;
        si (densitas > 0 && minores[e] < minor_minimus)
        {
            minor_minimus = minores[e];
        }
    }

    per (y_areae = 0; y_areae < alt; y_areae += GRADUS_AREAE)
    {
        per (x_areae = 0; x_areae < lat; x_areae += GRADUS_AREAE)
        {
            s32 al = x_areae + GRADUS_AREAE <= lat ? GRADUS_AREAE : lat
                - x_areae;
            s32 aa = y_areae + GRADUS_AREAE <= alt ? GRADUS_AREAE : alt
                - y_areae;
            s32 area = al * aa, i, j, k, nc;
            f64 medium_lin[III] = {0.0, 0.0, 0.0};
            f32 medium_lab[III];
            f32 loci[GRADUS_AREAE * GRADUS_AREAE][III];
            s32 positio[GRADUS_AREAE * GRADUS_AREAE];
            s32 cand[CANDIDATI_MAX];
            s32 extrema[II];
            s32 obscurum = 0, lucidum = 0;
            f32 lumen_minimum = 9.0f, lumen_maximum = -9.0f;
            f64 optimum = 0.0;
            s32 opt_a, opt_b, opt_e = 0;
            b32 tota_tecta = VERUM;
            f64 medium_oculi[III];
            f64 limites[CANDIDATI_MAX][CANDIDATI_MAX];
            f64 post_primum[DITHERING_BAYER_NUMERUS];
            s32 positio_prima[DITHERING_BAYER_NUMERUS];

            /* area tota picta: compositio eam superscribit */
            per (i = 0; i < area && tota_tecta; i++)
            {
                s32 p = (y_areae + i / al) * lat + x_areae + i % al;
                si (!_bit(tectum, p)) tota_tecta = FALSUM;
            }
            si (tota_tecta) perge;

            per (i = 0; i < area; i++)
            {
                s32 sx = x_areae + i % al, sy = y_areae + i / al;
                s32 p = sy * lat + sx;
                positio[i] = (sy & VII) * DITHERING_TEGULA + (sx & VII);
                per (k = 0; k < III; k++) medium_lin[k] += lin->c[k][p];
                _triplum(oculi, p, loci[i]);
                si (loci[i][0] < lumen_minimum)
                {
                    lumen_minimum  = loci[i][0];
                    obscurum       = i;
                }
                si (loci[i][0] > lumen_maximum)
                {
                    lumen_maximum  = loci[i][0];
                    lucidum        = i;
                }
            }
            _oklab((f32)(medium_lin[0] / (f64)area),
                (f32)(medium_lin[1] / (f64)area),
                (f32)(medium_lin[2] / (f64)area), medium_lab);

            /* candidati: VI proximi medio + II proximi extremis
             * utrisque */
            nc = _proximi_k(o, medium_lab, VI, cand);
            per (j = 0; j < II; j++)
            {
                s32 extremum = j == 0 ? obscurum : lucidum;
                s32 nx = _proximi_k(o, loci[extremum], II, extrema);
                per (i = 0; i < nx; i++)
                {
                    b32 adest = FALSUM;
                    per (k = 0; k < nc; k++)
                    {
                        si (cand[k] == extrema[i]) adest = VERUM;
                    }
                    si (!adest && nc < CANDIDATI_MAX)
                    {
                        cand[nc++] = extrema[i];
                    }
                }
            }

            per (k = 0; k < III; k++)
            {
                medium_oculi[k] = 0.0;
                per (i = 0; i < area; i++)
                {
                    medium_oculi[k] += (f64)loci[i][k];
                }
                medium_oculi[k] /= (f64)area;
            }
            _limites_parium(o, cand, nc, medium_oculi, area,
                minor_minimus, limites);
            per (e = 0; e < DITHERING_BAYER_NUMERUS; e++)
            {
                positio_prima[e] = (s32)DITHERING_BAYER_PONDERA[e][
                    positio[0]] * III;
            }

            /* initium: solidum proximi (etiam si color unus activus) */
            opt_a = cand[0];
            opt_b = cand[0];
            per (i = 0; i < area; i++)
            {
                optimum += _d2(o->pal_lab[opt_a], loci[i]);
            }

            per (i = 0; i < nc; i++)
            {
                s32 a               = cand[i];
                b32 solidum_factum  = FALSUM;
                per (j = 0; j < nc; j++)
                {
                             s32  b = cand[j];
                    constans f32* mixtio_paris;
                             f64  poena;
                    si (a == b) perge;
                    mixtio_paris = _mixtio(o, a, b, 0);
                    poena = POENA_CONTRASTUS * (f64)area
                        * o->distantia[a][b] * o->distantia[a][b];

                    /* gradus 0 (densitas 0): solidum a, semel per a */
                    si (!solidum_factum)
                    {
                        f64 err = poena * (f64)minores[0] / 32.0;
                        s32 q;
                        solidum_factum = VERUM;
                        per (q = 0; q < area && err < optimum; q++)
                        {
                            err += _d2(&mixtio_paris[(s32)
                                DITHERING_BAYER_PONDERA[0][positio[q]]
                                * III], loci[q]);
                        }
                        si (err < optimum)
                        {
                            optimum  = err;
                            opt_a    = a;
                            opt_b    = b;
                            opt_e    = 0;
                        }
                    }
                    si (limites[i][j] >= optimum) perge;

                    /* terminus primus graduum I..XV sine ramis: summa
                     * eadem ac in ansa (poena + d2 pixeli 0), ergo
                     * post_primum >= optimum gradum certe excludit */
                    per (e = I; e < DITHERING_BAYER_NUMERUS; e++)
                    {
                        post_primum[e] = poena * (f64)minores[e] / 32.0
                            + (f64)_d2(&mixtio_paris[positio_prima[e]],
                                loci[0]);
                    }
                    per (e = I; e < DITHERING_BAYER_NUMERUS; e++)
                    {
                        constans i8* pondera;
                                f64  err;
                                s32  q;

                        si (post_primum[e] >= optimum) perge;
                        pondera  = DITHERING_BAYER_PONDERA[e];
                        err      = post_primum[e];
                        per (q = I; q < area && err < optimum; q++)
                        {
                            err += _d2(&mixtio_paris[(s32)pondera[
                                positio[q]] * III], loci[q]);
                        }
                        si (err < optimum)
                        {
                            optimum  = err;
                            opt_a    = a;
                            opt_b    = b;
                            opt_e    = e;
                        }
                    }
                }
            }

            per (i = 0; i < area; i++)
            {
                s32 sx = x_areae + i % al, sy = y_areae + i / al;
                s32 punctum = _punctum(DITHERING_BAYER_ORDINES[opt_e],
                    sx,
                    sy);
                indices[sy * lat + sx] = (i8)(punctum ? opt_b : opt_a);
            }
        }
    }
}


/* ==================================================
 * Stratum V: residuum Atkinson cum liminibus per pixelum
 * ================================================== */

interior s32
_residuum (
     constans Opus* o,
    constans Plana* lin,
       constans i8* basis,
      constans f32* limina,
             Plana* error,
                i8* indices)
{
    hic_manens constans s32 VICINI[VI][II] = {
        {1, 0}, {2, 0}, {-1, 1}, {0, 1}, {1, 1}, {0, 2}
    };
    s32 lat = o->lat, alt = o->alt, x, y, k, nn, mutati = 0;

    per (k = 0; k < III; k++)
    {
        memset(error->c[k], 0, (memoriae_index)o->n * magnitudo(f32));
    }
    per (y = 0; y < alt; y++)
    {
        per (x = 0; x < lat; x++)
        {
            s32 p = y * lat + x;
            f32 t[III], lab[III], e[III];
            s32 b = (s32)basis[p], proximus, electus;

            per (k = 0; k < III; k++) t[k] = lin->c[k][p]
                                          + error->c[k][p];
            _oklab(t[0], t[1], t[2], lab);
            proximus = _proximus(o, lab);
            electus = (sqrt((f64)_d2(lab, o->pal_lab[proximus]))
                + (f64)limina[p] < sqrt((f64)_d2(lab, o->pal_lab[b])))
                    ? proximus : b;
            si (electus != b) mutati++;
            indices[p] = (i8)electus;
            per (k = 0; k < III; k++)
            {
                e[k] = (t[k] - o->pal_lin[electus][k]) / 8.0f;
            }
            per (nn = 0; nn < VI; nn++)
            {
                s32 nx = x + VICINI[nn][0], ny = y + VICINI[nn][1];
                si (nx < 0 || nx >= lat || ny >= alt) perge;
                per (k = 0; k < III; k++)
                {
                    error->c[k][ny * lat + nx] += e[k];
                }
            }
        }
    }
    redde mutati;
}


/* ==================================================
 * Regiones connexae (communia hachuris et pictographicis)
 * ================================================== */

/* Regiones 4-connexae ubi titulus[p] aequalis (titulus NIHIL: omnes
 * aequales) et bit p eligibilis; area < area_min omittuntur. membra
 * contigua per regionem in 'membra'; reddit numerum regionum. */
interior s32
_regiones_colligere (
    constans Opus* o,
     constans s32* titulus,
     constans i32* eligibilis,
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
        si (!_bit(eligibilis, p) || regio[p] >= 0) perge;
        acervus[vertex++]  = p;
        regio[p]           = p;
        dum (vertex > 0)
        {
            s32 q = acervus[--vertex], qx = q % lat, qy = q / lat, d;
            membra[positus + nm++] = q;
            per (d = 0; d < IV; d++)
            {
                s32 rx = qx + (d == 0) - (d == 1);
                s32 ry = qy + (d == 2) - (d == 3), t;
                si (rx < 0 || ry < 0 || rx >= lat || ry >= alt) perge;
                t = ry * lat + rx;
                si (regio[t] >= 0 || !_bit(eligibilis, t)) perge;
                si (titulus != NIHIL && titulus[t] != titulus[p]) perge;
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


/* ==================================================
 * Stratum II: hachurae curvae
 * ================================================== */

/* regiones hachuratae MAXIMAE primum dum praesupputatio non exceditur;
 * ceterae basi redduntur */
interior vacuum
_hachuras_eligere (
    constans Opus* o,
              i32* hachuratus,
              f64  praesupputatio,
              f64* tegmen,
              i32* numerus)
{
    s32  maximum = o->n / HACHURAE_AREA_MINIMA + I;
    s32* membra = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)o->n * magnitudo(s32));
    s32* initium = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)maximum * magnitudo(s32));
    s32* nm = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)maximum * magnitudo(s32));
          i32* servatus = _larvam_allocare(o->piscina, o->n);
    RegioOrdo* ordo = (RegioOrdo*)piscina_allocare(o->piscina,
        (memoriae_index)maximum * magnitudo(RegioOrdo));
    f64 limes = praesupputatio * (f64)o->n, usum = 0.0;
    s32 r, i, nr;

    *numerus = 0;
    nr = _regiones_colligere(o, NIHIL, hachuratus,
        HACHURAE_AREA_MINIMA, membra, initium, nm);
    per (r = 0; r < nr; r++)
    {
        ordo[r].energia  = -(f32)nm[r];
        ordo[r].index    = r;
    }
    qsort(ordo, (memoriae_index)nr, magnitudo(RegioOrdo),
        _comparare_regiones);
    per (r = 0; r < nr; r++)
    {
        s32 ri = ordo[r].index;
        si (usum + (f64)nm[ri] > limes) perge;
        per (i = 0; i < nm[ri]; i++)
        {
            _bit_ponere(servatus, membra[initium[ri] + i]);
        }
        usum += (f64)nm[ri];
        (*numerus)++;
    }
    memcpy(hachuratus, servatus,
        magnitudo(i32) * (memoriae_index)VERBA_LARVAE(o->n));
    *tegmen = usum / (f64)o->n;
}

/* summa nuclei (latus x latus, centrum radius) ad pixelum (x, y) campi,
 * limitibus ad oras; ordo summationis: dy, deinde dx crescens */
interior f32
_campi_summa (
    constans f32* campus,
    constans f32* nucleus,
             s32  lat,
             s32  alt,
             s32  radius,
             s32  latus,
             s32  x,
             s32  y)
{
    f32 summa = 0.0f;
    s32 dx, dy;

    per (dy = -radius; dy <= radius; dy++)
    {
                 s32  sy = y + dy;
        constans f32* linea;
        constans f32* nl;

        si (sy < 0) sy = 0;
        si (sy >= alt) sy = alt - I;
        linea  = &campus[sy * lat];
        nl     = &nucleus[(dy + radius) * latus + radius];
        per (dx = -radius; dx <= radius; dx++)
        {
            s32 sx  = x + dx < 0 ? 0 : x + dx >= lat ? lat - I : x + dx;
            summa   += nl[dx] * linea[sx];
        }
    }
    redde summa;
}

/* idem pro pixelis x..x+III interioribus (x >= radius, x + IV <= lat -
 * radius): catenae summationis IV independentes, nucleus et ordo sui
 * cuique - summae eaedem ac _campi_summa */
interior vacuum
_campi_quattuor (
     constans f32*  campus,
     constans f32** nuclei,
              s32   lat,
              s32   alt,
              s32   radius,
              s32   latus,
              s32   x,
              s32   y,
              f32*  summae)
{
    f32 s0 = 0.0f, s1 = 0.0f, s2 = 0.0f, s3 = 0.0f;
    s32 dx, dy;

    per (dy = -radius; dy <= radius; dy++)
    {
                 s32  sy      = y + dy;
                 s32  offset  = (dy + radius) * latus + radius;
        constans f32* c;
        constans f32 *n0, *n1, *n2, *n3;

        si (sy < 0) sy = 0;
        si (sy >= alt) sy = alt - I;
        c   = &campus[sy * lat + x];
        n0  = &nuclei[0][offset];
        n1  = &nuclei[I][offset];
        n2  = &nuclei[II][offset];
        n3  = &nuclei[III][offset];
        per (dx = -radius; dx <= radius; dx++)
        {
            s0 += n0[dx] * c[dx];
            s1 += n1[dx] * c[dx + I];
            s2 += n2[dx] * c[dx + II];
            s3 += n3[dx] * c[dx + III];
        }
    }
    summae[0]    = s0;
    summae[I]    = s1;
    summae[II]   = s2;
    summae[III]  = s3;
}

/* Campus striarum: filtrum Gabor orientatum iteratum super strepitum
 * (more emendationis digitorum); reddit campum ~sinusoidalem [-1, 1] */
interior vacuum
_campum_striarum (
         constans Opus* o,
    constans Structura* st_campi,
          constans i32* activus,
                   f32* campus)
{
    s32 lat = o->lat, alt = o->alt;
    s32 radius = (s32)ceil(HACHURAE_PERIODUS * 1.25) + I;
    s32 latus = 2 * radius + I, k, dx, dy, p, it, x, y;
    f32* nuclei = (f32*)piscina_allocare(o->piscina,
        (memoriae_index)(ANGULI_NUCLEORUM * latus * latus)
            * magnitudo(f32));
    f32* g = _planum(o->piscina, o->n);
     i8* index_nuclei = (i8*)piscina_allocare(o->piscina,
         (memoriae_index)o->n);
    f64 su = HACHURAE_PERIODUS * 0.75, sv = HACHURAE_PERIODUS * 0.5;

    per (k = 0; k < ANGULI_NUCLEORUM; k++)
    {
        f64 angulus_nuclei = PI_VALOR * (f64)k / ANGULI_NUCLEORUM;
        f64 ct = cos(angulus_nuclei), sn = sin(angulus_nuclei);
        f64 se = 0.0, sc = 0.0;
        f32* nu = &nuclei[k * latus * latus];
        per (dy = -radius; dy <= radius; dy++)
        {
            per (dx = -radius; dx <= radius; dx++)
            {
                f64 u = dx * ct + dy * sn, v = -dx * sn + dy * ct;
                f64 env = exp(-u * u / (2.0 * su * su) - v * v
                    / (2.0 * sv * sv));
                se += env;
                sc += env * cos(2.0 * PI_VALOR * v / HACHURAE_PERIODUS);
            }
        }
        /* sine componente continua: campus non derivat */
        per (dy = -radius; dy <= radius; dy++)
        {
            per (dx = -radius; dx <= radius; dx++)
            {
                f64 u = dx * ct + dy * sn, v = -dx * sn + dy * ct;
                f64 env = exp(-u * u / (2.0 * su * su) - v * v
                    / (2.0 * sv * sv));
                nu[(dy + radius) * latus + dx + radius] =
                    (f32)(env * (cos(2.0 * PI_VALOR * v
                        / HACHURAE_PERIODUS) - sc / se));
            }
        }
    }

    /* nucleus per pixelum activum semel: angulus inter iterationes
     * non mutatur */
    per (p = 0; p < o->n; p++)
    {
        si (!_bit(activus, p))
        {
            campus[p] = 0.0f;
            perge;
        }
        campus[p] = (f32)(2.0 * _strepitus(p % lat, p / lat, o->semen
            + I) - 1.0);
        index_nuclei[p] = (i8)((s32)floor((f64)st_campi->angulus[p]
            / PI_VALOR * ANGULI_NUCLEORUM + 0.5) % ANGULI_NUCLEORUM);
    }
    per (it = 0; it < HACHURAE_ITERATIONES; it++)
    {
        f64 sq = 0.0, media_quadratica;
        s32 na = 0;
        per (y = 0; y < alt; y++)
        {
            x = 0;
            dum (x < lat)
            {
                s32 q = y * lat + x, t;

                /* quattuor pixela interiora simul */
                si (x >= radius && x + IV <= lat - radius)
                {
                             b32  activa[IV];
                    constans f32* nu[IV];
                             f32  summae[IV];
                    per (t = 0; t < IV; t++)
                    {
                        activa[t] = _bit(activus, q + t);
                        nu[t] = &nuclei[(activa[t] ? (s32)index_nuclei[q
                            + t] : 0) * latus * latus];
                    }
                    si (!(activa[0] | activa[I] | activa[II]
                          | activa[III]))
                    {
                        g[q] = g[q + I] = g[q + II] = g[q + III] = 0.0f;
                        x += IV;
                        perge;
                    }
                    _campi_quattuor(campus, nu, lat, alt, radius, latus,
                        x, y, summae);
                    per (t = 0; t < IV; t++)
                    {
                        g[q + t] = activa[t] ? summae[t] : 0.0f;
                        si (!activa[t]) perge;
                        sq += (f64)summae[t] * (f64)summae[t];
                        na++;
                    }
                    x += IV;
                    perge;
                }
                si (_bit(activus, q))
                {
                    f32 summa = _campi_summa(campus, &nuclei[
                        (s32)index_nuclei[q] * latus * latus], lat, alt,
                        radius, latus, x, y);
                    g[q]  = summa;
                    sq    += (f64)summa * (f64)summa;
                    na++;
                }
                alioquin g[q] = 0.0f;
                x++;
            }
        }
        media_quadratica = na > 0 ? sqrt(sq / na) : 1.0;
        si (media_quadratica < 1e-12) media_quadratica = 1.0;
        /* inactivi: g 0, tanh(0) = 0 */
        per (p = 0; p < o->n; p++)
        {
            si (!_bit(activus, p)) perge;
            campus[p] = (f32)(it + I < HACHURAE_ITERATIONES
                ? tanh(1.5 * g[p] / media_quadratica)
                : g[p] / (1.41421356 * media_quadratica));
        }
    }
    per (p = 0; p < o->n; p++)
    {
        si (campus[p] > 1.0f) campus[p] = 1.0f;
        si (campus[p] < -1.0f) campus[p] = -1.0f;
    }
}

/* per areas 4x4: par (atramentum obscurius, charta) + tegmen per
 * pixelum ex tono; atramentum ubi campus > cos(pi * tegmen). pictus:
 * bit pixeli vere picti (area sine pari idonea basi manet, non
 * pingitur) */
interior vacuum
_hachuras_pingere (
     constans Opus* o,
    constans Plana* oculi_lati,
      constans f32* campus,
      constans i32* hachuratus,
                i8* indices,
               i32* pictus)
{
    s32 lat = o->lat, alt = o->alt, x_areae, y_areae;
    s32 tegmen[AREA_HACHURARUM * AREA_HACHURARUM];
    s32 tegmen_opt[AREA_HACHURARUM * AREA_HACHURARUM];
    f32 loci[AREA_HACHURARUM * AREA_HACHURARUM][III];
    b32 intra[AREA_HACHURARUM * AREA_HACHURARUM];

    per (y_areae = 0; y_areae < alt; y_areae += AREA_HACHURARUM)
    {
        per (x_areae = 0; x_areae < lat; x_areae += AREA_HACHURARUM)
        {
            s32 al = x_areae + AREA_HACHURARUM
                <= lat ? AREA_HACHURARUM : lat - x_areae;
            s32 aa = y_areae + AREA_HACHURARUM
                <= alt ? AREA_HACHURARUM : alt - y_areae;
            f64 medium[III] = {0.0, 0.0, 0.0};
            f32 medium_lab[III];
            s32 ordinati[VIII], na = 0, nc, i, j, q, w;
            f64 optimum = 1e30;
            s32 opt_a = -1, opt_b = -1;

            per (q = 0; q < al * aa; q++)
            {
                s32 p     = (y_areae + q / al) * lat + x_areae + q % al;
                intra[q]  = _bit(hachuratus, p);
                si (!intra[q]) perge;
                _triplum(oculi_lati, p, loci[q]);
                per (i = 0; i < III; i++) medium[i] += loci[q][i];
                na++;
            }
            si (na == 0) perge;
            per (i = 0; i < III; i++)
            {
                medium_lab[i] = (f32)(medium[i] / (f64)na);
            }
            nc = _proximi_k(o, medium_lab, VIII, ordinati);

            per (i = 0; i < nc; i++)
            {
                per (j = 0; j < nc; j++)
                {
                    /* a atramentum, b charta */
                    s32 a = ordinati[i], b = ordinati[j];
                    f64 err = 0.0;
                    si (   a                == b
                        || o->pal_lab[a][0] >= o->pal_lab[b][0]
                        || o->distantia[a][b]
                            < HACHURAE_DISTANTIA_MIN) perge;
                    per (q = 0; q < al * aa && err < optimum; q++)
                    {
                        f32 dm = 1e30f;
                        si (!intra[q]) perge;
                        per (w = 0; w <= DITHERING_GRADUS_MIXTI; w++)
                        {
                            f32 d = _d2(_mixtio(o, b, a, w), loci[q]);
                            si (d < dm)
                            {
                                dm         = d;
                                tegmen[q]  = w;
                            }
                        }
                        err += dm;
                    }
                    si (err < optimum)
                    {
                        optimum  = err;
                        opt_a    = a;
                        opt_b    = b;
                        memcpy(tegmen_opt, tegmen, magnitudo(tegmen));
                    }
                }
            }
            si (opt_a < 0) perge;
            per (q = 0; q < al * aa; q++)
            {
                s32 p = (y_areae + q / al) * lat + x_areae + q % al;
                si (!intra[q]) perge;
                indices[p] = (i8)(campus[p]
                    > cos(PI_VALOR * (f64)tegmen_opt[q]
                        / DITHERING_GRADUS_MIXTI) ? opt_a : opt_b);
                _bit_ponere(pictus, p);
            }
        }
    }
}


/* ==================================================
 * Stratum III: pictographica per regiones
 * ================================================== */

/* Regiones eiusdem coloris proximi et fasciae L (4-connexae), area >=
 * PICTOGRAMMATUM_AREA_MINIMA, ordine energiae mediae ascendente;
 * quaeque UNO pictogrammate familiae materiae et UNO pari (colorem
 * regionis continente) impletur dum pixela stilizata praesupputationem
 * non excedunt. Regio quae excederet omittitur; minores adhuc
 * intrant. */
interior vacuum
_regiones_stilizare (
                 constans Opus* o,
                constans Plana* lin,
                constans Plana* oculi_lati,
                  constans f32* energia,
                  constans i32* eligibilis,
    constans DitheringPictogramma* materia,
             constans Exemplar* figurae,
                           f64  praesupputatio,
                            i8* indices,
                           i32* larva,
                            i8* atramentum,
                            i8* charta,
                           s32* regio_pixeli,  /* ordinalis aut -1 */
                           f64* tegmen,
                           i32* numerus)
{
    s32  maximum = o->n / PICTOGRAMMATUM_AREA_MINIMA + I;
    s32* titulus = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)o->n * magnitudo(s32));
    s32* membra = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)o->n * magnitudo(s32));
    s32* initium = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)maximum * magnitudo(s32));
    s32* nm = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)maximum * magnitudo(s32));
    RegioOrdo* ordo = (RegioOrdo*)piscina_allocare(o->piscina,
        (memoriae_index)maximum * magnitudo(RegioOrdo));
    f64 limes = praesupputatio * (f64)o->n, usum = 0.0;
    s32 p, r, nr, numerus_ordinis = 0;

    *numerus = 0;
    memset(larva, 0,
        magnitudo(i32) * (memoriae_index)VERBA_LARVAE(o->n));
    per (p = 0; p < o->n; p++)
    {
        f32 lab[III];
        _triplum(oculi_lati, p, lab);
        titulus[p] = _proximus(o, lab) * LXIV
                   + (s32)((f64)lab[0] / PICTOGRAMMATUM_FASCIA);
        regio_pixeli[p] = -1;
    }
    nr = _regiones_colligere(o, titulus, eligibilis,
        PICTOGRAMMATUM_AREA_MINIMA, membra, initium, nm);
    per (r = 0; r < nr; r++)
    {
        s32 col = titulus[membra[initium[r]]] / LXIV, i;
        f64 summa = 0.0;
        si (materia[col] == DITHERING_PICTOGRAMMA_NULLUM) perge;
        per (i = 0; i < nm[r]; i++) summa += energia[membra[initium[r]
                                        + i]];
        ordo[numerus_ordinis].energia  = (f32)(summa / (f64)nm[r]);
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
                 f32  medium_lab[III];
        s32 ordinati[IX], nc, i, j, k, e;
        f64 optimum = 1e30, distantia_minima;
        s32 opt_a = -1, opt_b = -1, opt_e = -1;
        b32 umbra;

        si (usum + (f64)numerus_r > limes) perge;
        per (i = 0; i < numerus_r; i++)
        {
            per (k = 0; k < III; k++) medium_lin[k] += lin->c[k][mb[i]];
        }
        _oklab((f32)(medium_lin[0] / (f64)numerus_r),
            (f32)(medium_lin[1] / (f64)numerus_r),
            (f32)(medium_lin[2] / (f64)numerus_r), medium_lab);
        umbra = medium_lab[0] < PICTOGRAMMATUM_UMBRA;
        distantia_minima = umbra ? PICTOGRAMMATUM_DISTANTIA_UMBRAE
            : PICTOGRAMMATUM_DISTANTIA_MIN;
        nc = _proximi_k(o, medium_lab, IX, ordinati);

        per (e = 0; e < DITHERING_PICTOGRAMMATA_FONTIUM; e++)
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
                        f64 ca = o->pal_lab[alter][1];
                        f64 cb = o->pal_lab[alter][2];
                        err = (f64)numerus_r
                            * PICTOGRAMMATUM_POENA_UMBRAE
                            * (ca * ca + cb * cb);
                    }
                    alioquin
                    {
                        f64 da = (f64)o->pal_lab[alter][1]
                            - medium_lab[1];
                        f64 db = (f64)o->pal_lab[alter][2]
                            - medium_lab[2];
                        err = (f64)numerus_r
                            * PICTOGRAMMATUM_POENA_COLORIS
                            * (da * da + db * db);
                    }
                    per (q = 0; q < numerus_r && err < optimum; q++)
                    {
                        s32 m = mb[q];
                        err += _d2_plani(_mixtio(o, a, b,
                            (s32)figurae[e].pondus_latum[((m / o->lat)
                                & VII) * DITHERING_TEGULA
                                + ((m % o->lat) & VII)]),
                            oculi_lati, m);
                    }
                    si (err < optimum)
                    {
                        optimum  = err;
                        opt_a    = a;
                        opt_b    = b;
                        opt_e    = e;
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
            _bit_ponere(larva, m);
            atramentum[m]    = (i8)opt_b;
            charta[m]        = (i8)opt_a;
            regio_pixeli[m]  = (s32)*numerus;
        }
        usum += (f64)numerus_r;
        (*numerus)++;
    }
    *tegmen = usum / (f64)o->n;
}


/* ==================================================
 * Stratum IV: aspersio (situla pigmenti variae pressionis)
 * ================================================== */

/* Umbra amissa = L fontis minus L redditionis (ambo oculo lato), MINUS
 * media eius per regionem (tonus regionis planus consulto est: aspersio
 * variationem umbrae reddit, non tonum totum corrigit - aliter exemplar
 * dissolvit), deinde late obscurata INTRA regiones (per larvam
 * normalizata). Tegmen = pressio * |differentia| / (L lucidior - L
 * obscurior paris), <= tectum.
 * Obscurare: color obscurior in pixela lucidiora; lucidare: contra. */
interior s32
_aspergere (
     constans Opus* o,
       constans i8* basis,
      constans i32* larva,
      constans s32* regio_pixeli,
               s32  numerus_regionum,
       constans i8* atramentum,
       constans i8* charta,
    constans Plana* oculi_lati,
               f64  pressio,
               f64  tectum,
                i8* exitus)
{
    Plana  r;
      f32* tmp          = _planum(o->piscina, o->n);
      f32* differentia  = _planum(o->piscina, o->n);
      f32* pondus       = _planum(o->piscina, o->n);
      f64* summa = (f64*)piscina_allocare(o->piscina,
          (memoriae_index)(numerus_regionum + I) * magnitudo(f64));
    s32* numeri = (s32*)piscina_allocare(o->piscina,
        (memoriae_index)(numerus_regionum + I) * magnitudo(s32));
    s32 p, k, mutata = 0;

    memcpy(exitus, basis, (memoriae_index)o->n);
    si (!_plana_allocare(&r, o->piscina, o->n)) redde 0;
    per (k = 0; k < III; k++)
    {
        per (p = 0; p < o->n; p++)
        {
            r.c[k][p] = o->pal_lin[(s32)basis[p]][k];
        }
        _gauss_planum(r.c[k], r.c[k], tmp, o->lat, o->alt,
            DITHERING_SIGMA_OCULI_LATI);
    }
    per (p = 0; p < o->n; p++)
    {
        f32 lab[III];
        b32 intus = _bit(larva, p);
        _oklab(r.c[0][p], r.c[1][p], r.c[2][p], lab);
        differentia[p]  = intus ? oculi_lati->c[0][p] - lab[0] : 0.0f;
        pondus[p]       = intus ? 1.0f : 0.0f;
    }
    per (k = 0; k < numerus_regionum; k++)
    {
        summa[k]   = 0.0;
        numeri[k]  = 0;
    }
    per (p = 0; p < o->n; p++)
    {
        si (regio_pixeli[p] < 0) perge;
        summa[regio_pixeli[p]] += differentia[p];
        numeri[regio_pixeli[p]]++;
    }
    per (p = 0; p < o->n; p++)
    {
        si (regio_pixeli[p] < 0) perge;
        differentia[p] = (f32)((f64)differentia[p]
            - summa[regio_pixeli[p]] / (f64)numeri[regio_pixeli[p]]);
    }
    _gauss_planum(differentia, differentia, tmp, o->lat, o->alt,
        SIGMA_UMBRAE);
    _gauss_planum(pondus, pondus, tmp, o->lat, o->alt, SIGMA_UMBRAE);

    per (p = 0; p < o->n; p++)
    {
        s32 obscurus, lucidus;
        f64 differentia_loci, dl, tegmen;

        si (!_bit(larva, p) || pondus[p] < 1e-6f) perge;
        differentia_loci = (f64)differentia[p] / (f64)pondus[p];
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
        dl = (f64)o->pal_lab[lucidus][0] - (f64)o->pal_lab[obscurus][0];
        si (dl < 0.02) perge;
        tegmen = pressio * (differentia_loci < 0.0 ? -differentia_loci
            : differentia_loci) / dl;
        si (tegmen > tectum) tegmen = tectum;
        si (_strepitus(p % o->lat, p / o->lat, o->semen)
            >= tegmen) perge;
        si (differentia_loci < 0.0 && (s32)basis[p] == lucidus)
        {
            exitus[p] = (i8)obscurus;
            mutata++;
        }
        alioquin si (   differentia_loci > 0.0
                     && (s32)basis[p] == obscurus)
        {
            exitus[p] = (i8)lucidus;
            mutata++;
        }
    }
    redde mutata;
}


/* ==================================================
 * Functiones publicae
 * ================================================== */

DitheringFructus
dithering_exemplaria_velox_colorum (
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
                          memoriae_index  m1;

    Exemplar figurae[DITHERING_PICTOGRAMMATA_FONTIUM];
    Plana lin, oculi, oculi_lati, error;
    Structura st, st_campi;
    f32 *lumen, *tmp, *limina, *campus;
    i32 *hachuratus, *activus, *eligibilis, *larva, *pictus, *tectum;
    s32* regio_pixeli;
    i8 *basis, *pictura_hachurarum, *pictura_stili, *atramentum,
        *charta, *sparsum;
    f64 tegmen_h = 0.0, tegmen_p = 0.0;
    i32 regiones_h = 0, regiones_p = 0;
    s32 p, k, verba, aspersa = 0, stilizata = 0;
    b32 sana;

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
    _exemplaria_ex_tabulis(figurae);

    m1     = (memoriae_index)o.n;
    verba  = VERBA_LARVAE(o.n);
    sana   = _plana_allocare(&lin, piscina, o.n)
          && _plana_allocare(&oculi, piscina, o.n)
          && _plana_allocare(&oculi_lati, piscina, o.n)
          && _plana_allocare(&error, piscina, o.n);
    lumen = _planum(piscina, o.n);
    tmp = _planum(piscina, o.n);
    limina = _planum(piscina, o.n);
    campus = _planum(piscina, o.n);
    st.energia = _planum(piscina, o.n);
    st.cohaerentia = _planum(piscina, o.n);
    st.angulus = NIHIL;  /* nemo legit: sine atan2 */
    st_campi.energia = _planum(piscina, o.n);
    st_campi.cohaerentia = _planum(piscina, o.n);
    st_campi.angulus = _planum(piscina, o.n);
    hachuratus = _larvam_allocare(piscina, o.n);
    activus = _larvam_allocare(piscina, o.n);
    eligibilis = _larvam_allocare(piscina, o.n);
    larva = _larvam_allocare(piscina, o.n);
    pictus = _larvam_allocare(piscina, o.n);
    tectum = _larvam_allocare(piscina, o.n);
    regio_pixeli = (s32*)piscina_allocare(piscina, m1 * magnitudo(s32));
    basis = (i8*)piscina_allocare(piscina, m1);
    pictura_hachurarum = (i8*)piscina_allocare(piscina, m1);
    pictura_stili = (i8*)piscina_allocare(piscina, m1);
    atramentum = (i8*)piscina_allocare(piscina, m1);
    charta = (i8*)piscina_allocare(piscina, m1);
    sparsum = (i8*)piscina_allocare(piscina, m1);
    fructus.indices = (i8*)piscina_allocare(piscina, m1);
    si (   !sana || lumen == NIHIL || tmp == NIHIL || limina == NIHIL
        || campus           == NIHIL || st.energia == NIHIL
        || st.cohaerentia   == NIHIL
        || st_campi.energia == NIHIL || st_campi.cohaerentia == NIHIL
        || st_campi.angulus == NIHIL
        || hachuratus       == NIHIL || activus == NIHIL
        || eligibilis       == NIHIL || larva == NIHIL
        || pictus           == NIHIL || tectum == NIHIL
        || regio_pixeli     == NIHIL
        || basis            == NIHIL || pictura_hachurarum == NIHIL
        || pictura_stili    == NIHIL || atramentum == NIHIL
        || charta           == NIHIL || sparsum == NIHIL
        || fructus.indices  == NIHIL)
    {
        fructus.indices = NIHIL;
        redde fructus;
    }

    /* lux linearis, L fontis, oculi duo */
    per (p = 0; p < o.n; p++)
    {
        f32 lab[III];
        per (k = 0; k < III; k++)
        {
            lin.c[k][p] = DITHERING_SRGB_LINEARIS[rgba[p * IV + k]];
        }
        _oklab(lin.c[0][p], lin.c[1][p], lin.c[2][p], lab);
        lumen[p] = lab[0];
    }
    per (k = 0; k < III; k++)
    {
        _gauss_planum(lin.c[k], error.c[k], tmp, o.lat, o.alt,
            DITHERING_SIGMA_OCULI);
    }
    _oklab_plana(&error, &oculi, o.n);
    per (k = 0; k < III; k++)
    {
        _gauss_planum(lin.c[k], error.c[k], tmp, o.lat, o.alt,
            DITHERING_SIGMA_OCULI_LATI);
    }
    _oklab_plana(&error, &oculi_lati, o.n);

    /* classes detalii et campus hachurarum */
    _structuram_computare(&o, lumen, SIGMA_STRUCTURAE, &st);
    _structuram_computare(&o, lumen, SIGMA_CAMPI, &st_campi);
    per (p = 0; p < o.n; p++)
    {
        f64 d = _gradus_lenis(ENERGIA_INFIMA, ENERGIA_SUMMA,
            st.energia[p]);
        f64 cw = _gradus_lenis(COHAERENTIA_LINEAE - 0.15,
            COHAERENTIA_LINEAE + 0.15, st.cohaerentia[p]);
        f64 h  = _gradus_lenis(HACHURAE_ENERGIA_INFIMA,
            HACHURAE_ENERGIA_SUMMA, st_campi.energia[p])
               * _gradus_lenis(HACHURAE_COHAERENTIA - 0.1,
               HACHURAE_COHAERENTIA + 0.1, st_campi.cohaerentia[p]);
        f64 lux = oculi_lati.c[0][p];
        limina[p] = (f32)(LIMEN_PLANI * (1.0 - d) + d * (cw
            * LIMEN_LINEAE + (1.0 - cw) * LIMEN_TEXTURAE));
        si (   opt->praesupputatio_hachurarum > 0.0 && h > 0.5
            && lux > HACHURAE_LUMEN_INFIMUM
            && lux < HACHURAE_LUMEN_SUMMUM)
        {
            _bit_ponere(hachuratus, p);
        }
        si (h > 0.2) _bit_ponere(activus, p);
    }

    /* II hachurae: regiones maximae sub praesupputatione. Strata II
     * et III basim non legunt: basis post ea venit et areas totas
     * pictas omittit */
    _hachuras_eligere(&o, hachuratus, opt->praesupputatio_hachurarum,
        &tegmen_h, &regiones_h);
    si (regiones_h > 0)
    {
        _campum_striarum(&o, &st_campi, activus, campus);
        _hachuras_pingere(&o, &oculi_lati, campus, hachuratus,
            pictura_hachurarum, pictus);
    }

    /* III pictographica extra hachuras */
    per (k = 0; k < verba; k++) eligibilis[k] = ~hachuratus[k];
    si (opt->praesupputatio_pictographica > 0.0)
    {
        _regiones_stilizare(&o, &lin, &oculi_lati, st.energia,
            eligibilis, opt->materia, figurae,
            opt->praesupputatio_pictographica, pictura_stili, larva,
            atramentum, charta, regio_pixeli, &tegmen_p, &regiones_p);
    }
    alioquin
    {
        per (p = 0; p < o.n; p++) regio_pixeli[p] = -1;
    }

    /* I basis: pixela picta (hachurae vere pictae, pictographica)
     * compositio superscribit, ergo areae totae tectae omittuntur */
    per (k = 0; k < verba; k++) tectum[k] = pictus[k] | larva[k];
    _basis_bayer(&o, &lin, &oculi, tectum, basis);

    /* compositio + limina stratorum (hachuratus sine pari idoneo:
     * basis manet, limen hachurarum tamen - ut classicus) */
    per (p = 0; p < o.n; p++)
    {
        si (_bit(hachuratus, p))
        {
            si (_bit(pictus, p)) basis[p] = pictura_hachurarum[p];
            limina[p] = (f32)HACHURAE_LIMEN;
        }
        alioquin si (_bit(larva, p))
        {
            basis[p]   = pictura_stili[p];
            limina[p]  = (f32)PICTOGRAMMATUM_LIMEN;
            stilizata++;
        }
    }

    /* IV aspersio */
    si (opt->pressio_aspersionis > 0.0 && regiones_p > 0)
    {
        aspersa = _aspergere(&o, basis, larva, regio_pixeli,
            (s32)regiones_p, atramentum, charta, &oculi_lati,
            opt->pressio_aspersionis, opt->tectum_aspersionis, sparsum);
        memcpy(basis, sparsum, m1);
    }

    /* V residuum */
    _residuum(&o, &lin, basis, limina, &error, fructus.indices);

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
