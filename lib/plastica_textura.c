/*
 * plastica_textura.c - texturae genitae (plastica_textura.h)
 *
 * Omne genus functio est (tegula, x, y) -> index coloris (0 obscurus,
 * I medius, II clarus). Arithmetica integra sola. Tegula margines
 * convenientes habet quia omnis cella per numerum integrum cellarum
 * in latitudinem et altitudinem dividitur et omnis vicinus per
 * modulum quaeritur.
 */

#include "plastica_textura.h"
#include <string.h>

#define MAGNITUDO_MAXIMA  CCLVI
#define OBSCURUS          ZEPHYRUM
#define MEDIUS            I
#define CLARUS            II

nomen structura {
    i32 latitudo;
    i32 altitudo;
    i32 semen;
} Tegula;

nomen i32 (*Pictor)(
    constans Tegula* t,
                i32  x,
                i32  y);


/* ==================================================
 * Sortes
 * ================================================== */

interior i32
_miscere (
    i32 x)
{
    x ^= x >> XVI;
    x *= 0x85EBCA6Bu;
    x ^= x >> XIII;
    x *= 0xC2B2AE35u;
    x ^= x >> XVI;
    redde x;
}

/* sors ex semine, rivo (quisque usus rivum suum habet) et duobus
 * indicibus */
interior i32
_sors (
    constans Tegula* t,
                i32  rivus,
                i32  a,
                i32  b)
{
    i32 x = _miscere(t->semen * 0x9E3779B1u + rivus * 0x7F4A7C15u
        + I);

    x = _miscere(x ^ (a * 0x85EBCA77u + I));
    x = _miscere(x ^ (b * 0xC2B2AE3Du + II));
    redde x >> VIII;
}

/* [0, C) */
interior i32
_centesima (
    constans Tegula* t,
                i32  rivus,
                i32  a,
                i32  b)
{
    redde _sors(t, rivus, a, b) % C;
}

/* quot cellae gradus fere dati in mensuram cadunt (minimum I) */
interior i32
_cellae (
    i32 mensura,
    i32 gradus)
{
    i32 n = mensura / gradus;

    redde n < I ? I : n;
}

/* cella in qua x iacet, mensura in 'numerus' cellas divisa */
interior i32
_cella (
    i32 x,
    i32 mensura,
    i32 numerus)
{
    redde x * numerus / mensura;
}

/* x primum pixelum cellae suae est? */
interior b32
_limen (
    i32 x,
    i32 mensura,
    i32 numerus)
{
    redde numerus > I
        && _cella(x, mensura, numerus)
            != _cella((x + mensura - I) % mensura, mensura, numerus);
}


/* ==================================================
 * Genera
 * ================================================== */

interior i32
_granum (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 c = _centesima(t, I, x, y);

    redde c < XII ? OBSCURUS : (c < XXII ? CLARUS : MEDIUS);
}

interior i32
_humus (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 nx = _cellae(t->latitudo, IV);
    i32 ny = _cellae(t->altitudo, IV);
    i32 gleba  = _centesima(t, I, _cella(x, t->latitudo, nx),
        _cella(y, t->altitudo, ny));
    i32 c      = _centesima(t, II, x, y);

    si (c < IV)
    {
        redde CLARUS;
    }
    si (gleba < XXVIII && c < LXXV)
    {
        redde OBSCURUS;
    }
    redde c < XIV ? OBSCURUS : MEDIUS;
}

interior i32
_muscus (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 nx = _cellae(t->latitudo, V);
    i32 ny = _cellae(t->altitudo, V);
    /* margo laceratus: pixelum cellam vicini interdum sumit */
    i32 mx  = (x + _sors(t, I, x, y) % III + t->latitudo - I)
        % t->latitudo;
    i32 my  = (y + _sors(t, II, x, y) % III + t->altitudo - I)
        % t->altitudo;
    i32 macula = _centesima(t, III, _cella(mx, t->latitudo, nx),
        _cella(my, t->altitudo, ny));
    i32 c   = _centesima(t, IV, x, y);

    si (macula < XLV)
    {
        redde c < XL ? CLARUS : MEDIUS;
    }
    redde c < XV ? MEDIUS : OBSCURUS;
}

interior i32
_frondes (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 nx    = _cellae(t->latitudo, IV);
    i32 ny    = _cellae(t->altitudo, III);
    i32 ordo  = _cella(y, t->altitudo, ny);
    /* ordines alterni motu fortuito: craticula non apparet */
    i32 mx    = (x + _sors(t, I, ordo, ZEPHYRUM) % t->latitudo)
        % t->latitudo;
    i32 folium  = _cella(mx, t->latitudo, nx);
    i32 c       = _centesima(t, II, folium, ordo);

    /* umbra sub folio et ad latus eius */
    si (   (_limen(y, t->altitudo, ny)
            && _centesima(t, III, x, y) < LXX)
        || (_limen(mx, t->latitudo, nx)
            && _centesima(t, IV, x, y) < L))
    {
        redde OBSCURUS;
    }
    redde c < XXV ? OBSCURUS : (c < LXX ? MEDIUS : CLARUS);
}

interior i32
_cortex (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 ny    = _cellae(t->altitudo, VIII);
    i32 my    = (y + _sors(t, I, x, ZEPHYRUM) % t->altitudo)
        % t->altitudo;
    i32 c     = _centesima(t, II, x, _cella(my, t->altitudo, ny));

    redde c < XXXII ? OBSCURUS : (c < LXXVIII ? MEDIUS : CLARUS);
}

interior i32
_tabulae (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 nx      = _cellae(t->latitudo, VIII);
    i32 ny      = _cellae(t->altitudo, X);
    i32 tabula  = _cella(x, t->latitudo, nx);
    i32 finis   = _sors(t, I, tabula, ZEPHYRUM) % t->altitudo;
    i32 my      = (y + _sors(t, II, x, ZEPHYRUM) % t->altitudo)
        % t->altitudo;
    i32 c       = _centesima(t, III, x, _cella(my, t->altitudo, ny));

    /* iunctura inter tabulas; finis tabulae (linea una per
     * tabulam) */
    si (_limen(x, t->latitudo, nx) || y == finis)
    {
        redde OBSCURUS;
    }
    redde c < XXII ? CLARUS : MEDIUS;
}

/* saxum cuius semen pixelo proximum est (Voronoi integer cum
 * involutione) */
interior i32
_saxum_proximum (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 nx       = _cellae(t->latitudo, VIII);
    i32 ny       = _cellae(t->altitudo, VIII);
    i32 cx       = _cella(x, t->latitudo, nx);
    i32 cy       = _cella(y, t->altitudo, ny);
    i32 optimum  = ZEPHYRUM;
    i32 minima   = ZEPHYRUM;
    b32 primum   = VERUM;
    i32 di;
    i32 dj;

    per (dj = ZEPHYRUM; dj < III; dj++)
    {
        per (di = ZEPHYRUM; di < III; di++)
        {
            i32 i        = (cx + nx - I + di) % nx;
            i32 j        = (cy + ny - I + dj) % ny;
            i32 origo_x  = i * t->latitudo / nx;
            i32 origo_y  = j * t->altitudo / ny;
            i32 px  = origo_x + _sors(t, I, i, j)
                % ((i + I) * t->latitudo / nx - origo_x);
            i32 py  = origo_y + _sors(t, II, i, j)
                % ((j + I) * t->altitudo / ny - origo_y);
            i32 dx = x > px ? x - px : px - x;
            i32 dy = y > py ? y - py : py - y;
            i32 d;

            si (dx > t->latitudo - dx)
            {
                dx = t->latitudo - dx;
            }
            si (dy > t->altitudo - dy)
            {
                dy = t->altitudo - dy;
            }
            d = dx * dx + dy * dy;
            si (primum || d < minima)
            {
                primum   = FALSUM;
                minima   = d;
                optimum  = j * nx + i;
            }
        }
    }
    redde optimum;
}

interior i32
_saxum (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 hoc = _saxum_proximum(t, x, y);
    i32 c;

    /* rima ubi saxum aliud dextrorsum aut deorsum incipit */
    si (   hoc != _saxum_proximum(t, (x + I) % t->latitudo, y)
        || hoc != _saxum_proximum(t, x, (y + I) % t->altitudo))
    {
        redde OBSCURUS;
    }
    c = _centesima(t, III, hoc, ZEPHYRUM) < XXXV ? CLARUS : MEDIUS;
    si (_centesima(t, IV, x, y) < VII)
    {
        c = c == CLARUS ? MEDIUS : CLARUS;
    }
    redde c;
}

interior i32
_lateres (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 nx = _cellae(t->latitudo, VIII);
    i32 ny = _cellae(t->altitudo, IV);
    i32 ordo;
    i32 mx;
    i32 later;
    i32 c;

    /* ordines alterni: numerus par, ne sutura deorsum fiat */
    si (ny > I && ny % II)
    {
        ny--;
    }
    ordo   = _cella(y, t->altitudo, ny);
    mx     = (x + (ordo % II) * (t->latitudo / (nx * II)))
        % t->latitudo;
    later  = _cella(mx, t->latitudo, nx);
    si (_limen(y, t->altitudo, ny) || _limen(mx, t->latitudo, nx))
    {
        redde OBSCURUS;
    }
    c = _centesima(t, I, later, ordo) < XXX ? CLARUS : MEDIUS;
    si (_centesima(t, II, x, y) < VI)
    {
        c = c == CLARUS ? MEDIUS : CLARUS;
    }
    redde c;
}

interior i32
_stramen (
    constans Tegula* t,
                i32  x,
                i32  y)
{
    i32 ny     = _cellae(t->altitudo, VI);
    i32 ordo   = _cella(y, t->altitudo, ny);
    i32 c      = _centesima(t, I, x, ordo);
    i32 infra  = (y + I) % t->altitudo;
    i32 bis    = (y + II) % t->altitudo;

    /* umbra ad imum ordinis, margine inaequali */
    si (   _limen(infra, t->altitudo, ny)
        || (_limen(bis, t->altitudo, ny)
            && _sors(t, II, x, ordo) % III == ZEPHYRUM))
    {
        redde OBSCURUS;
    }
    redde c < XVIII ? OBSCURUS : (c < LXVIII ? MEDIUS : CLARUS);
}


/* ==================================================
 * Publica
 * ================================================== */

nomen structura {
    constans character* titulus;
                Pictor  pictor;
} Genus;

hic_manens constans Genus GENERA[PLASTICA_TEXTURA_GENERA] = {
    { "cortex", _cortex },
    { "tabulae", _tabulae },
    { "saxum", _saxum },
    { "lateres", _lateres },
    { "stramen", _stramen },
    { "frondes", _frondes },
    { "muscus", _muscus },
    { "humus", _humus },
    { "granum", _granum }
};

constans character*
plastica_textura_genus (
    i32 index)
{
    redde index < PLASTICA_TEXTURA_GENERA ? GENERA[index].titulus
        : NIHIL;
}

b32
plastica_textura_generare (
           chorda  genus,
              i32  latitudo,
              i32  altitudo,
              i32  semen,
      constans i8* colores,
            Imago* exitus,
          Piscina* piscina)
{
    Tegula  t;
        i8* pixela;
       i32  g;
       i32  x;
       i32  y;

    si (   !colores || !exitus || !piscina || latitudo < I
        || latitudo > MAGNITUDO_MAXIMA || altitudo < I
        || altitudo > MAGNITUDO_MAXIMA)
    {
        redde FALSUM;
    }
    per (g = ZEPHYRUM; g < PLASTICA_TEXTURA_GENERA; g++)
    {
        si (chorda_aequalis_literis(genus, GENERA[g].titulus))
        {
            frange;
        }
    }
    si (g == PLASTICA_TEXTURA_GENERA)
    {
        redde FALSUM;
    }
    pixela = (i8*)piscina_allocare(piscina,
        (memoriae_index)latitudo * altitudo * IV);
    si (!pixela)
    {
        redde FALSUM;
    }
    t.latitudo  = latitudo;
    t.altitudo  = altitudo;
    t.semen     = semen;
    per (y = ZEPHYRUM; y < altitudo; y++)
    {
        per (x = ZEPHYRUM; x < latitudo; x++)
        {
            i32  c = GENERA[g].pictor(&t, x, y);
             i8* p = pixela + ((memoriae_index)y * latitudo + x) * IV;

            memcpy(p, colores + c * III, III);
            p[III] = CCLV;
        }
    }
    exitus->pixela    = pixela;
    exitus->latitudo  = latitudo;
    exitus->altitudo  = altitudo;
    redde VERUM;
}
