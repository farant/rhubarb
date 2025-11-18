/* Functiones graphicae pro delineando formas */
#include "delineare.h"
#include <string.h>

/* Exemplaria classica Mac conversa */
interior constans i8 exemplaria_interna[EXEMPLAR_NUMERUS][VIII] = {
    /* Exemplar 00: SOLIDUS - omnia nigra */
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},

    /* Exemplar 01: PUNCTA_DISPERSA */
    {0xDD, 0xFF, 0x77, 0xFF, 0xDD, 0xFF, 0x77, 0xFF},

    /* Exemplar 02: PUNCTA_REGULARIA */
    {0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77},

    /* Exemplar 03: TESSELLATUM */
    {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55},

    /* Exemplar 04: LINEOLAE_HORIZONTALES */
    {0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF},

    /* Exemplar 05: LINEAE_VERTICALES */
    {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},

    /* Exemplar 06: DIAGONALIS */
    {0xEE, 0xDD, 0xBB, 0x77, 0xEE, 0xDD, 0xBB, 0x77},

    /* Exemplar 07: VERTICALIS_DISPERSUS */
    {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88},

    /* Exemplar 08: DISPERSUS */
    {0xB1, 0x30, 0x03, 0x1B, 0xD8, 0xC0, 0x0C, 0x8D},

    /* Exemplar 09: PUNCTA_DIAGONALIA */
    {0x80, 0x10, 0x02, 0x20, 0x01, 0x08, 0x40, 0x04},

    /* Exemplar 10: LATERICIUM */
    {0xFF, 0x88, 0x88, 0x88, 0xFF, 0x88, 0x88, 0x88},

    /* Exemplar 11: LATERICIUM_OFFSET */
    {0xFF, 0x80, 0x80, 0x80, 0xFF, 0x08, 0x08, 0x08},

    /* Exemplar 12: CINEREUM_LEVE (punctum singulare) */
    {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    /* Exemplar 13: DIAGONALIS_DISPERSUS */
    {0x80, 0x40, 0x20, 0x00, 0x02, 0x04, 0x08, 0x00},

    /* Exemplar 14: RHOMBUS_DISPERSUS */
    {0x82, 0x44, 0x39, 0x44, 0x82, 0x01, 0x01, 0x01},

    /* Exemplar 15: TEXTUM */
    {0xF8, 0x74, 0x22, 0x47, 0x8F, 0x17, 0x22, 0x71},

    /* Exemplar 16: MIXTUS */
    {0x55, 0xA0, 0x40, 0x40, 0x55, 0x0A, 0x04, 0x04},

    /* Exemplar 17: ANGULUS */
    {0x20, 0x50, 0x88, 0x88, 0x88, 0x88, 0x05, 0x02},

    /* Exemplar 18: LATERICIUM_COMPLEXUS */
    {0xAF, 0x00, 0xAF, 0xAF, 0xA0, 0xA0, 0xA0, 0xA0},

    /* Exemplar 19: ALBUS - omnia clara */
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    /* Exemplar 20: RARISSIMUS */
    {0x80, 0x00, 0x08, 0x00, 0x80, 0x00, 0x08, 0x00},

    /* Exemplar 21: PUNCTA_DUPLICIA_DISPERSA */
    {0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00},

    /* Exemplar 22: PUNCTA_DUPLICIA */
    {0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22},

    /* Exemplar 23: HORIZONTALIS_DISPERSUS */
    {0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x00},

    /* Exemplar 24: LINEAE_HORIZONTALES */
    {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00},

    /* Exemplar 25: DIAGONALIS_ANTERIOR */
    {0x11, 0x22, 0x44, 0x88, 0x11, 0x22, 0x44, 0x88},

    /* Exemplar 26: HORIZONTALIS_LATUS */
    {0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00},

    /* Exemplar 27: DIAGONALIS_POSTERIOR */
    {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80},

    /* Exemplar 28: DISPERSUS_MIXTUS */
    {0xAA, 0x00, 0x80, 0x00, 0x88, 0x00, 0x80, 0x00},

    /* Exemplar 29: MARGO_SINISTER */
    {0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80},

    /* Exemplar 30: ARCUS */
    {0x08, 0x1C, 0x22, 0xC1, 0x80, 0x01, 0x02, 0x04},

    /* Exemplar 31: LINEA_RHOMBUS */
    {0x88, 0x14, 0x22, 0x41, 0x88, 0x00, 0xAA, 0x00},

    /* Exemplar 32: ANGULI_PARVI */
    {0x40, 0xA0, 0x00, 0x00, 0x08, 0x14, 0x00, 0x00},

    /* Exemplar 33: DIAGONALIS_UNDULATUS */
    {0x03, 0x84, 0x48, 0x30, 0x0C, 0x02, 0x01, 0x01},

    /* Exemplar 34: CRUX */
    {0x80, 0x80, 0x41, 0x3E, 0x08, 0x08, 0x14, 0xE3},

    /* Exemplar 35: MIXTUS_GRAVIS */
    {0x10, 0x20, 0x54, 0xAA, 0xFF, 0x02, 0x04, 0x08},

    /* Exemplar 36: CAPSAE_COMPLEXAE */
    {0x77, 0x89, 0x8F, 0x8F, 0x77, 0x98, 0xF8, 0xF8},

    /* Exemplar 37: RHOMBUS_LINEATUS */
    {0x00, 0x08, 0x14, 0x2A, 0x55, 0x2A, 0x14, 0x08}
};

/* Macros adiutorii */
#define MINIMUM(a, b) ((a) < (b) ? (a) : (b))
#define MAXIMUM(a, b) ((a) > (b) ? (a) : (b))
#define ABSOLUTUM(x) ((x) < 0 ? -(x) : (x))
#define COMMUTARE(a, b) do { i32 temporarium = a; a = b; b = temporarium; } while(ZEPHYRUM)

/* Verificare si punctum intra rectangulum praecisionis */
interior b32
est_praecisus (
    constans ContextusDelineandi* ctx,
    i32                           x,
    i32                           y)
{
    redde (x < ctx->praecisio_x || x >= ctx->praecisio_x + ctx->praecisio_latitudo ||
            y < ctx->praecisio_y || y >= ctx->praecisio_y + ctx->praecisio_altitudo);
}

/* Applicare modum delineandi et exemplar */
interior vacuum
ponere_pixelum_internum (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  color)
{
    i32* pixelum;

    si (!ctx->tabula || !ctx->tabula->pixela) redde;
    si (x < ZEPHYRUM || x >= ctx->tabula->latitudo || y < ZEPHYRUM || y >= ctx->tabula->altitudo) redde;
    si (est_praecisus(ctx, x, y)) redde;

    pixelum = &ctx->tabula->pixela[y * ctx->tabula->latitudo + x];

    commutatio (ctx->modus)
    {
        casus MODUS_SOLIDUS:
            *pixelum = color;
            frange;

        casus MODUS_EXEMPLAR:
        {
            i32 px;
            i32 py;

            px = (x - ctx->exemplar_offset_x) & VII;
            py = (y - ctx->exemplar_offset_y) & VII;
            si (ctx->exemplar[py] & (I << (VII - px)))
            {
                *pixelum = color;
            }
            frange;
        }

        casus MODUS_XOR:
            *pixelum ^= color;
            frange;
    }
}

/* Gestio contextus */
ContextusDelineandi*
delineare_creare_contextum (
    Piscina*         piscina,
    TabulaPixelorum* tabula)
{
    ContextusDelineandi *ctx;

    si (!tabula) redde NIHIL;

    ctx = piscina_allocare(piscina, magnitudo(ContextusDelineandi));
    si (!ctx) redde NIHIL;

    ctx->tabula = tabula;
    ctx->praecisio_x = ZEPHYRUM;
    ctx->praecisio_y = ZEPHYRUM;
    ctx->praecisio_latitudo = tabula->latitudo;
    ctx->praecisio_altitudo = tabula->altitudo;
    ctx->modus = MODUS_SOLIDUS;

    /* Initializare cum exemplar solido */
    memset(ctx->exemplar, 0xFF, magnitudo(ctx->exemplar));

    redde ctx;
}

vacuum
delineare_restituere_contextum (
    ContextusDelineandi* ctx)
{
    si (!ctx || !ctx->tabula) redde;

    ctx->praecisio_x = ZEPHYRUM;
    ctx->praecisio_y = ZEPHYRUM;
    ctx->praecisio_latitudo = ctx->tabula->latitudo;
    ctx->praecisio_altitudo = ctx->tabula->altitudo;
    ctx->modus = MODUS_SOLIDUS;
    ctx->exemplar_offset_x = ZEPHYRUM;
    ctx->exemplar_offset_y = ZEPHYRUM;
    memset(ctx->exemplar, 0xFF, magnitudo(ctx->exemplar));
}

/* Modus et exemplar */
vacuum
delineare_ponere_modum (
    ContextusDelineandi* ctx,
    modus_delineandi_t   modus)
{
    si (ctx) ctx->modus = modus;
}

vacuum
delineare_ponere_exemplar (
    ContextusDelineandi* ctx,
    constans i8          exemplar[VIII])
{
    si (ctx && exemplar)
    {
        memcpy(ctx->exemplar, exemplar, VIII);
    }
}

vacuum
delineare_ponere_exemplar_internum (
    ContextusDelineandi* ctx,
    i32                  exemplar_id)
{
    si (ctx && exemplar_id >= ZEPHYRUM && exemplar_id < EXEMPLAR_NUMERUS)
    {
        memcpy(ctx->exemplar, exemplaria_interna[exemplar_id], VIII);
    }
}

vacuum
delineare_ponere_offset_exemplaris (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y)
{
    si (ctx)
    {
        ctx->exemplar_offset_x = x;
        ctx->exemplar_offset_y = y;
    }
}

/* Praecisio */
vacuum
delineare_ponere_praecisionem (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo)
{
    si (!ctx || !ctx->tabula) redde;

    /* Constringere ad limites tabulae */
    ctx->praecisio_x = MAXIMUM(ZEPHYRUM, x);
    ctx->praecisio_y = MAXIMUM(ZEPHYRUM, y);
    ctx->praecisio_latitudo = MINIMUM(latitudo, ctx->tabula->latitudo - ctx->praecisio_x);
    ctx->praecisio_altitudo = MINIMUM(altitudo, ctx->tabula->altitudo - ctx->praecisio_y);
}

vacuum
delineare_tollere_praecisionem (
    ContextusDelineandi* ctx)
{
    si (!ctx || !ctx->tabula) redde;

    ctx->praecisio_x = ZEPHYRUM;
    ctx->praecisio_y = ZEPHYRUM;
    ctx->praecisio_latitudo = ctx->tabula->latitudo;
    ctx->praecisio_altitudo = ctx->tabula->altitudo;
}

/* Pixelum singulare */
vacuum
delineare_pixelum (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  color)
{
    ponere_pixelum_internum(ctx, x, y, color);
}

/* Linea horizontalis - optimizata */
vacuum
delineare_lineam_horizontalem (
    ContextusDelineandi* ctx,
    i32                  x0,
    i32                  x1,
    i32                  y,
    i32                  color)
{
    i32 x;

    si (!ctx) redde;

    /* Assecurare x0 <= x1 */
    si (x0 > x1) COMMUTARE(x0, x1);

    per (x = x0; x <= x1; x++)
    {
        ponere_pixelum_internum(ctx, x, y, color);
    }
}

/* Linea verticalis - optimizata */
vacuum
delineare_lineam_verticalem (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y0,
    i32                  y1,
    i32                  color)
{
    i32 y;

    si (!ctx) redde;

    /* Assecurare y0 <= y1 */
    si (y0 > y1) COMMUTARE(y0, y1);

    per (y = y0; y <= y1; y++)
    {
        ponere_pixelum_internum(ctx, x, y, color);
    }
}

/* Linea generalis utendo algorithmo Bresenham */
vacuum
delineare_lineam (
    ContextusDelineandi* ctx,
    i32                  x0,
    i32                  y0,
    i32                  x1,
    i32                  y1,
    i32                  color)
{
    s32 dx;
    s32 dy;
    s32 sx;
    s32 sy;
    s32 error;
    s32 e2;
    s32 cx;
    s32 cy;

    si (!ctx) redde;

    cx = (s32)x0;
    cy = (s32)y0;
    dx = (s32)x1 - (s32)x0;
    si (dx < 0) dx = -dx;
    dy = (s32)y1 - (s32)y0;
    si (dy < 0) dy = -dy;
    sx = x0 < x1 ? 1 : -1;
    sy = y0 < y1 ? 1 : -1;
    error = dx - dy;

    dum (I)
    {
        ponere_pixelum_internum(ctx, (i32)cx, (i32)cy, color);

        si (cx == (s32)x1 && cy == (s32)y1) frange;

        e2 = 2 * error;
        si (e2 > -dy)
        {
            error -= dy;
            cx += sx;
        }
        si (e2 < dx)
        {
            error += dx;
            cy += sy;
        }
    }
}

/* Rectangulum delineatum */
vacuum
delineare_rectangulum (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    i32                  color)
{
    si (!ctx || latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM) redde;

    delineare_lineam_horizontalem(ctx, x, x + latitudo - I, y, color);
    delineare_lineam_horizontalem(ctx, x, x + latitudo - I, y + altitudo - I, color);
    delineare_lineam_verticalem(ctx, x, y + I, y + altitudo - II, color);
    delineare_lineam_verticalem(ctx, x + latitudo - I, y + I, y + altitudo - II, color);
}

/* Rectangulum plenum */
vacuum
delineare_rectangulum_plenum (
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    i32                  color)
{
    i32 py;
    i32 px;

    si (!ctx || latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM) redde;

    per (py = y; py < y + altitudo; py++)
    {
        per (px = x; px < x + latitudo; px++)
        {
            ponere_pixelum_internum(ctx, px, py, color);
        }
    }
}

/* Circulus utendo algorithmo circuli puncti medii */
vacuum
delineare_circulum (
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius,
    i32                  color)
{
    i32 x;
    i32 y;
    i32 error;

    si (!ctx || radius < ZEPHYRUM) redde;

    x = radius;
    y = ZEPHYRUM;
    error = ZEPHYRUM;

    dum (x >= y)
    {
        ponere_pixelum_internum(ctx, centrum_x + x, centrum_y + y, color);
        ponere_pixelum_internum(ctx, centrum_x + y, centrum_y + x, color);
        ponere_pixelum_internum(ctx, centrum_x - y, centrum_y + x, color);
        ponere_pixelum_internum(ctx, centrum_x - x, centrum_y + y, color);
        ponere_pixelum_internum(ctx, centrum_x - x, centrum_y - y, color);
        ponere_pixelum_internum(ctx, centrum_x - y, centrum_y - x, color);
        ponere_pixelum_internum(ctx, centrum_x + y, centrum_y - x, color);
        ponere_pixelum_internum(ctx, centrum_x + x, centrum_y - y, color);

        si (error <= ZEPHYRUM)
        {
            y += I;
            error += II * y + I;
        }
        si (error > ZEPHYRUM)
        {
            x -= I;
            error -= II * x + I;
        }
    }
}

/* Circulus plenus */
vacuum
delineare_circulum_plenum (
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius,
    i32                  color)
{
    i32 x;
    i32 y;
    i32 error;

    si (!ctx || radius < ZEPHYRUM) redde;

    x = radius;
    y = ZEPHYRUM;
    error = ZEPHYRUM;

    dum (x >= y)
    {
        delineare_lineam_horizontalem(ctx, centrum_x - x, centrum_x + x, centrum_y + y, color);
        delineare_lineam_horizontalem(ctx, centrum_x - y, centrum_x + y, centrum_y + x, color);
        delineare_lineam_horizontalem(ctx, centrum_x - x, centrum_x + x, centrum_y - y, color);
        delineare_lineam_horizontalem(ctx, centrum_x - y, centrum_x + y, centrum_y - x, color);

        si (error <= ZEPHYRUM)
        {
            y += I;
            error += II * y + I;
        }
        si (error > ZEPHYRUM)
        {
            x -= I;
            error -= II * x + I;
        }
    }
}

/* Triangulum delineatum */
vacuum
delineare_triangulum (
    ContextusDelineandi* ctx,
    i32                  x0,
    i32                  y0,
    i32                  x1,
    i32                  y1,
    i32                  x2,
    i32                  y2,
    i32                  color)
{
    si (!ctx) redde;

    delineare_lineam(ctx, x0, y0, x1, y1, color);
    delineare_lineam(ctx, x1, y1, x2, y2, color);
    delineare_lineam(ctx, x2, y2, x0, y0, color);
}

/* Triangulum plenum utendo algorithmo scanline */
vacuum
delineare_triangulum_plenum (
    ContextusDelineandi* ctx,
    i32                  x0,
    i32                  y0,
    i32                  x1,
    i32                  y1,
    i32                  x2,
    i32                  y2,
    i32                  color)
{
    s32 sx0, sy0, sx1, sy1, sx2, sy2;
    s32 y;
    s32 xa;
    s32 xb;
    s32 denom;

    si (!ctx) redde;

    sx0 = (s32)x0; sy0 = (s32)y0;
    sx1 = (s32)x1; sy1 = (s32)y1;
    sx2 = (s32)x2; sy2 = (s32)y2;

    /* Ordinare vertices per coordinata y */
    si (sy0 > sy1)
    {
        s32 temp;
        temp = sx0; sx0 = sx1; sx1 = temp;
        temp = sy0; sy0 = sy1; sy1 = temp;
    }
    si (sy1 > sy2)
    {
        s32 temp;
        temp = sx1; sx1 = sx2; sx2 = temp;
        temp = sy1; sy1 = sy2; sy2 = temp;
    }
    si (sy0 > sy1)
    {
        s32 temp;
        temp = sx0; sx0 = sx1; sx1 = temp;
        temp = sy0; sy0 = sy1; sy1 = temp;
    }

    /* Delineare dimidium superius */
    per (y = sy0; y <= sy1; y++)
    {
        denom = sy1 - sy0;
        si (denom == 0) denom = 1;
        xa = sx0 + (sx1 - sx0) * (y - sy0) / denom;

        denom = sy2 - sy0;
        si (denom == 0) denom = 1;
        xb = sx0 + (sx2 - sx0) * (y - sy0) / denom;

        delineare_lineam_horizontalem(ctx, (i32)MINIMUM(xa, xb), (i32)MAXIMUM(xa, xb), (i32)y, color);
    }

    /* Delineare dimidium inferius */
    per (y = sy1; y <= sy2; y++)
    {
        denom = sy2 - sy1;
        si (denom == 0) denom = 1;
        xa = sx1 + (sx2 - sx1) * (y - sy1) / denom;

        denom = sy2 - sy0;
        si (denom == 0) denom = 1;
        xb = sx0 + (sx2 - sx0) * (y - sy0) / denom;

        delineare_lineam_horizontalem(ctx, (i32)MINIMUM(xa, xb), (i32)MAXIMUM(xa, xb), (i32)y, color);
    }
}

/* Ellipsis utendo algorithmo ellipsis puncti medii */
vacuum
delineare_ellipsin (
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius_x,
    i32                  radius_y,
    i32                  color)
{
    i32 x;
    i32 y;
    i32 rx2;
    i32 ry2;
    i32 tworx2;
    i32 twory2;
    i32 p;
    i32 px;
    i32 py;

    si (!ctx || radius_x < ZEPHYRUM || radius_y < ZEPHYRUM) redde;

    x = ZEPHYRUM;
    y = radius_y;
    rx2 = radius_x * radius_x;
    ry2 = radius_y * radius_y;
    tworx2 = II * rx2;
    twory2 = II * ry2;
    px = ZEPHYRUM;
    py = tworx2 * y;

    /* Delineare puncta initialia in extremis */
    ponere_pixelum_internum(ctx, centrum_x + radius_x, centrum_y, color);
    ponere_pixelum_internum(ctx, centrum_x - radius_x, centrum_y, color);
    ponere_pixelum_internum(ctx, centrum_x, centrum_y + radius_y, color);
    ponere_pixelum_internum(ctx, centrum_x, centrum_y - radius_y, color);

    /* Regio 1 */
    p = (i32)(ry2 - (rx2 * radius_y) + (rx2 / IV));
    dum (px < py)
    {
        x++;
        px += twory2;
        si (p < ZEPHYRUM)
        {
            p += ry2 + px;
        }
        alioquin
        {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }

        ponere_pixelum_internum(ctx, centrum_x + x, centrum_y + y, color);
        ponere_pixelum_internum(ctx, centrum_x - x, centrum_y + y, color);
        ponere_pixelum_internum(ctx, centrum_x + x, centrum_y - y, color);
        ponere_pixelum_internum(ctx, centrum_x - x, centrum_y - y, color);
    }

    /* Regio 2 */
    p = (i32)(ry2 * (x * x + x) + rx2 * ((y - I) * (y - I)) - rx2 * ry2);
    dum (y > ZEPHYRUM)
    {
        y--;
        py -= tworx2;
        si (p > ZEPHYRUM)
        {
            p += rx2 - py;
        }
        alioquin
        {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }

        ponere_pixelum_internum(ctx, centrum_x + x, centrum_y + y, color);
        ponere_pixelum_internum(ctx, centrum_x - x, centrum_y + y, color);
        ponere_pixelum_internum(ctx, centrum_x + x, centrum_y - y, color);
        ponere_pixelum_internum(ctx, centrum_x - x, centrum_y - y, color);
    }
}

/* Ellipsis plena */
vacuum
delineare_ellipsin_plenam (
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius_x,
    i32                  radius_y,
    i32                  color)
{
    i32 x;
    i32 y;
    i32 rx2;
    i32 ry2;
    i32 tworx2;
    i32 twory2;
    i32 p;
    i32 px;
    i32 py;

    si (!ctx || radius_x < ZEPHYRUM || radius_y < ZEPHYRUM) redde;

    x = ZEPHYRUM;
    y = radius_y;
    rx2 = radius_x * radius_x;
    ry2 = radius_y * radius_y;
    tworx2 = II * rx2;
    twory2 = II * ry2;
    px = ZEPHYRUM;
    py = tworx2 * y;

    /* Delineare lineam horizontalem initialem */
    delineare_lineam_horizontalem(ctx, centrum_x - radius_x, centrum_x + radius_x, centrum_y, color);

    /* Regio 1 */
    p = (i32)(ry2 - (rx2 * radius_y) + (rx2 / IV));
    dum (px < py)
    {
        x++;
        px += twory2;
        si (p < ZEPHYRUM)
        {
            p += ry2 + px;
        }
        alioquin
        {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }

        delineare_lineam_horizontalem(ctx, centrum_x - x, centrum_x + x, centrum_y + y, color);
        delineare_lineam_horizontalem(ctx, centrum_x - x, centrum_x + x, centrum_y - y, color);
    }

    /* Regio 2 */
    p = (i32)(ry2 * (x * x + x) + rx2 * ((y - I) * (y - I)) - rx2 * ry2);
    dum (y > ZEPHYRUM)
    {
        y--;
        py -= tworx2;
        si (p > ZEPHYRUM)
        {
            p += rx2 - py;
        }
        alioquin
        {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }

        delineare_lineam_horizontalem(ctx, centrum_x - x, centrum_x + x, centrum_y + y, color);
        delineare_lineam_horizontalem(ctx, centrum_x - x, centrum_x + x, centrum_y - y, color);
    }
}

/* Polygonum delineatum */
vacuum
delineare_polygonum (
    ContextusDelineandi* ctx,
    constans i32*        puncta,
    i32                  numerus_punctorum,
    i32                  color)
{
    i32 i;
    i32 j;

    si (!ctx || !puncta || numerus_punctorum < III) redde;

    per (i = ZEPHYRUM; i < numerus_punctorum; i++)
    {
        j = (i + I) % numerus_punctorum;
        delineare_lineam(ctx, puncta[i * II], puncta[i * II + I],
                      puncta[j * II], puncta[j * II + I], color);
    }
}

/* Polygonum plenum utendo algorithmo scanline */
vacuum
delineare_polygonum_plenum (
    ContextusDelineandi* ctx,
    constans i32*        puncta,
    i32                  numerus_punctorum,
    i32                  color)
{
    i32 min_y;
    i32 max_y;
    i32 y;
    i32 intersectiones[CCLVI];
    i32 numerus_intersectionum;
    i32 i;
    i32 j;
    i32 y0;
    i32 y1;
    i32 x0;
    i32 x1;
    i32 x;
    i32 temp;

    si (!ctx || !puncta || numerus_punctorum < III) redde;

    /* Invenire valores y minimos/maximos */
    min_y = puncta[I];
    max_y = puncta[I];
    per (i = I; i < numerus_punctorum; i++)
    {
        y = puncta[i * II + I];
        si (y < min_y) min_y = y;
        si (y > max_y) max_y = y;
    }

    /* Implere scanline */
    per (y = min_y; y <= max_y; y++)
    {
        numerus_intersectionum = ZEPHYRUM;

        /* Invenire omnes intersectiones cum scanline */
        per (i = ZEPHYRUM; i < numerus_punctorum; i++)
        {
            j = (i + I) % numerus_punctorum;
            y0 = puncta[i * II + I];
            y1 = puncta[j * II + I];

            si ((y0 <= y && y1 > y) || (y1 <= y && y0 > y))
            {
                x0 = puncta[i * II];
                x1 = puncta[j * II];
                x = x0 + (x1 - x0) * (y - y0) / (y1 - y0);

                si (numerus_intersectionum < CCLVI)
                {
                    intersectiones[numerus_intersectionum++] = x;
                }
            }
        }

        /* Ordinare intersectiones */
        per (i = ZEPHYRUM; i < numerus_intersectionum - I; i++)
        {
            per (j = i + I; j < numerus_intersectionum; j++)
            {
                si (intersectiones[i] > intersectiones[j])
                {
                    temp = intersectiones[i];
                    intersectiones[i] = intersectiones[j];
                    intersectiones[j] = temp;
                }
            }
        }

        /* Delineare lineas horizontales inter paria intersectionum */
        per (i = ZEPHYRUM; i < numerus_intersectionum; i += II)
        {
            si (i + I < numerus_intersectionum)
            {
                delineare_lineam_horizontalem(ctx, intersectiones[i], intersectiones[i + I], y, color);
            }
        }
    }
}

/* Functiones utilitatis */
vacuum
delineare_vacare (
    ContextusDelineandi* ctx,
    i32                  color)
{
    i32 conservare_x;
    i32 conservare_y;
    i32 conservare_latitudo;
    i32 conservare_altitudo;

    si (!ctx || !ctx->tabula) redde;

    /* Temporaliter disablere praecisionem pro vacando */
    conservare_x = ctx->praecisio_x;
    conservare_y = ctx->praecisio_y;
    conservare_latitudo = ctx->praecisio_latitudo;
    conservare_altitudo = ctx->praecisio_altitudo;

    ctx->praecisio_x = ZEPHYRUM;
    ctx->praecisio_y = ZEPHYRUM;
    ctx->praecisio_latitudo = ctx->tabula->latitudo;
    ctx->praecisio_altitudo = ctx->tabula->altitudo;

    delineare_rectangulum_plenum(ctx, ZEPHYRUM, ZEPHYRUM, ctx->tabula->latitudo, ctx->tabula->altitudo, color);

    /* Restituere praecisionem */
    ctx->praecisio_x = conservare_x;
    ctx->praecisio_y = conservare_y;
    ctx->praecisio_latitudo = conservare_latitudo;
    ctx->praecisio_altitudo = conservare_altitudo;
}

vacuum
delineare_cratem (
    ContextusDelineandi* ctx,
    i32                  spatium,
    i32                  color)
{
    i32 x;
    i32 y;

    si (!ctx || !ctx->tabula || spatium <= ZEPHYRUM) redde;

    /* Delineare lineas verticales */
    per (x = ZEPHYRUM; x < ctx->tabula->latitudo; x += spatium)
    {
        delineare_lineam_verticalem(ctx, x, ZEPHYRUM, ctx->tabula->altitudo - I, color);
    }

    /* Delineare lineas horizontales */
    per (y = ZEPHYRUM; y < ctx->tabula->altitudo; y += spatium)
    {
        delineare_lineam_horizontalem(ctx, ZEPHYRUM, ctx->tabula->latitudo - I, y, color);
    }
}
