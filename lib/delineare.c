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
    ctx->piscina = piscina;
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
    s32 x;
    s32 y;
    s32 rx2;
    s32 ry2;
    s32 tworx2;
    s32 twory2;
    s32 p;
    s32 px;
    s32 py;

    si (!ctx || radius_x < ZEPHYRUM || radius_y < ZEPHYRUM) redde;

    x = 0;
    y = (s32)radius_y;
    rx2 = (s32)radius_x * (s32)radius_x;
    ry2 = (s32)radius_y * (s32)radius_y;
    tworx2 = 2 * rx2;
    twory2 = 2 * ry2;
    px = 0;
    py = tworx2 * y;

    /* Delineare puncta initialia in extremis */
    ponere_pixelum_internum(ctx, (i32)((s32)centrum_x + (s32)radius_x), centrum_y, color);
    ponere_pixelum_internum(ctx, (i32)((s32)centrum_x - (s32)radius_x), centrum_y, color);
    ponere_pixelum_internum(ctx, centrum_x, (i32)((s32)centrum_y + (s32)radius_y), color);
    ponere_pixelum_internum(ctx, centrum_x, (i32)((s32)centrum_y - (s32)radius_y), color);

    /* Regio 1 */
    p = ry2 - (rx2 * (s32)radius_y) + (rx2 / 4);
    dum (px < py)
    {
        x++;
        px += twory2;
        si (p < 0)
        {
            p += ry2 + px;
        }
        alioquin
        {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }

        ponere_pixelum_internum(ctx, (i32)((s32)centrum_x + x), (i32)((s32)centrum_y + y), color);
        ponere_pixelum_internum(ctx, (i32)((s32)centrum_x - x), (i32)((s32)centrum_y + y), color);
        ponere_pixelum_internum(ctx, (i32)((s32)centrum_x + x), (i32)((s32)centrum_y - y), color);
        ponere_pixelum_internum(ctx, (i32)((s32)centrum_x - x), (i32)((s32)centrum_y - y), color);
    }

    /* Regio 2 */
    p = ry2 * (x * x + x) + rx2 * ((y - 1) * (y - 1)) - rx2 * ry2;
    dum (y > 0)
    {
        y--;
        py -= tworx2;
        si (p > 0)
        {
            p += rx2 - py;
        }
        alioquin
        {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }

        ponere_pixelum_internum(ctx, (i32)((s32)centrum_x + x), (i32)((s32)centrum_y + y), color);
        ponere_pixelum_internum(ctx, (i32)((s32)centrum_x - x), (i32)((s32)centrum_y + y), color);
        ponere_pixelum_internum(ctx, (i32)((s32)centrum_x + x), (i32)((s32)centrum_y - y), color);
        ponere_pixelum_internum(ctx, (i32)((s32)centrum_x - x), (i32)((s32)centrum_y - y), color);
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
    s32 x;
    s32 y;
    s32 rx2;
    s32 ry2;
    s32 tworx2;
    s32 twory2;
    s32 p;
    s32 px;
    s32 py;

    si (!ctx || radius_x < ZEPHYRUM || radius_y < ZEPHYRUM) redde;

    x = 0;
    y = (s32)radius_y;
    rx2 = (s32)radius_x * (s32)radius_x;
    ry2 = (s32)radius_y * (s32)radius_y;
    tworx2 = 2 * rx2;
    twory2 = 2 * ry2;
    px = 0;
    py = tworx2 * y;

    /* Delineare lineam horizontalem initialem */
    delineare_lineam_horizontalem(ctx, (i32)((s32)centrum_x - (s32)radius_x), (i32)((s32)centrum_x + (s32)radius_x), centrum_y, color);

    /* Regio 1 */
    p = ry2 - (rx2 * (s32)radius_y) + (rx2 / 4);
    dum (px < py)
    {
        x++;
        px += twory2;
        si (p < 0)
        {
            p += ry2 + px;
        }
        alioquin
        {
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }

        delineare_lineam_horizontalem(ctx, (i32)((s32)centrum_x - x), (i32)((s32)centrum_x + x), (i32)((s32)centrum_y + y), color);
        delineare_lineam_horizontalem(ctx, (i32)((s32)centrum_x - x), (i32)((s32)centrum_x + x), (i32)((s32)centrum_y - y), color);
    }

    /* Regio 2 */
    p = ry2 * (x * x + x) + rx2 * ((y - 1) * (y - 1)) - rx2 * ry2;
    dum (y > 0)
    {
        y--;
        py -= tworx2;
        si (p > 0)
        {
            p += rx2 - py;
        }
        alioquin
        {
            x++;
            px += twory2;
            p += rx2 - py + px;
        }

        delineare_lineam_horizontalem(ctx, (i32)((s32)centrum_x - x), (i32)((s32)centrum_x + x), (i32)((s32)centrum_y + y), color);
        delineare_lineam_horizontalem(ctx, (i32)((s32)centrum_x - x), (i32)((s32)centrum_x + x), (i32)((s32)centrum_y - y), color);
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
    s32 min_y;
    s32 max_y;
    s32 y;
    s32 intersectiones[256];
    s32 numerus_intersectionum;
    s32 i;
    s32 j;
    s32 y0;
    s32 y1;
    s32 x0;
    s32 x1;
    s32 x;
    s32 temp;

    si (!ctx || !puncta || numerus_punctorum < III) redde;

    /* Invenire valores y minimos/maximos */
    min_y = (s32)puncta[1];
    max_y = (s32)puncta[1];
    per (i = 1; i < (s32)numerus_punctorum; i++)
    {
        y = (s32)puncta[i * 2 + 1];
        si (y < min_y) min_y = y;
        si (y > max_y) max_y = y;
    }

    /* Implere scanline */
    per (y = min_y; y <= max_y; y++)
    {
        numerus_intersectionum = 0;

        /* Invenire omnes intersectiones cum scanline */
        per (i = 0; i < (s32)numerus_punctorum; i++)
        {
            j = (i + 1) % (s32)numerus_punctorum;
            y0 = (s32)puncta[i * 2 + 1];
            y1 = (s32)puncta[j * 2 + 1];

            si ((y0 <= y && y1 > y) || (y1 <= y && y0 > y))
            {
                x0 = (s32)puncta[i * 2];
                x1 = (s32)puncta[j * 2];
                x = x0 + (x1 - x0) * (y - y0) / (y1 - y0);

                si (numerus_intersectionum < 256)
                {
                    intersectiones[numerus_intersectionum++] = x;
                }
            }
        }

        /* Ordinare intersectiones */
        per (i = 0; i < numerus_intersectionum - 1; i++)
        {
            per (j = i + 1; j < numerus_intersectionum; j++)
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
        per (i = 0; i < numerus_intersectionum; i += 2)
        {
            si (i + 1 < numerus_intersectionum)
            {
                delineare_lineam_horizontalem(ctx, (i32)intersectiones[i], (i32)intersectiones[i + 1], (i32)y, color);
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


/* ==================================================
 * Gradientia Dithered - Multi-Algorithm & Palette Support
 * ================================================== */

/* Bayer matrix 4x4 pro ordered dithering */
interior constans i8 bayer_matrix_4x4[IV][IV] = {
    {  0,  8,  2, 10 },
    { 12,  4, 14,  6 },
    {  3, 11,  1,  9 },
    { 15,  7, 13,  5 }
};

/* Bayer matrix 8x8 pro ordered dithering (smoother) */
interior constans i8 bayer_matrix_8x8[VIII][VIII] = {
    {  0, 32,  8, 40,  2, 34, 10, 42 },
    { 48, 16, 56, 24, 50, 18, 58, 26 },
    { 12, 44,  4, 36, 14, 46,  6, 38 },
    { 60, 28, 52, 20, 62, 30, 54, 22 },
    {  3, 35, 11, 43,  1, 33,  9, 41 },
    { 51, 19, 59, 27, 49, 17, 57, 25 },
    { 15, 47,  7, 39, 13, 45,  5, 37 },
    { 63, 31, 55, 23, 61, 29, 53, 21 }
};

/* Extrahere componentes RGB ex colore */
interior vacuum
extrahere_rgb(
    i32 color,
    i32* r,
    i32* g,
    i32* b)
{
    *r = (color >> XVI) & CCLV;
    *g = (color >> VIII) & CCLV;
    *b = color & CCLV;
}

/* Creare colorem ex componentibus RGB */
interior i32
componere_rgb(
    i32 r,
    i32 g,
    i32 b)
{
    redde ((i32)CCLV << XXIV) | ((i32)r << XVI) | ((i32)g << VIII) | (i32)b;
}

/* Cohibere valorem intra limites 0-255 */
interior i32
cohibere(
    i32 valor)
{
    si (valor < ZEPHYRUM) redde ZEPHYRUM;
    si (valor > CCLV) redde CCLV;
    redde valor;
}

/* Integer square root using Newton's method */
interior i32
isqrt(i32 n)
{
    i32 x, y;

    si (n <= ZEPHYRUM) redde ZEPHYRUM;
    si (n == I) redde I;

    /* Initial guess */
    x = n;
    y = (x + I) / II;

    /* Iterate until convergence */
    dum (y < x)
    {
        x = y;
        y = (x + n / x) / II;
    }

    redde x;
}

/* Quantizare componentem ad 0 vel 255 */
interior i32
quantizare_component(
    i32 valor)
{
    redde (valor >= CXXVIII) ? CCLV : ZEPHYRUM;
}

/* Interpolate inter duo valores
 *
 * a: valor initialis
 * b: valor finalis
 * t: positio (0-256, ubi 0=a, 256=b)
 */
interior i32
interpolate(
    i32 a,
    i32 b,
    i32 t)
{
    redde a + ((b - a) * t) / CCLVI;
}

/* Convertere RGB ad cinereum (grayscale) cum ponderibus luminantiae
 *
 * r, g, b: componentes RGB (0-255)
 *
 * Reddit: valor cinereus (0-255)
 *
 * Ponderes luminantiae ITU-R BT.601:
 * - R: 30% (XXX/C = 30/100)
 * - G: 59% (LIX/C = 59/100)
 * - B: 11% (XI/C = 11/100)
 */
interior i32
ad_cinereum(
    i32 r,
    i32 g,
    i32 b)
{
    /* (r * 30 + g * 59 + b * 11) / 100 */
    redde (r * XXX + g * LIX + b * XI) / C;
}

/* Invenire colorem proximum in palette per valorem cinereum
 *
 * cinereus: valor cinereus quaeritus (0-255)
 * palette: array colorum (RGBA8888)
 * numerus_colorum: numerus colorum in palette
 *
 * Reddit: color proximus ex palette (per distantiam cineream)
 */
interior i32
invenire_colorem_per_cinereum(
    i32 cinereus,
    constans i32* palette,
    i32 numerus_colorum)
{
    i32 i;
    i32 distantia_minima;
    i32 color_proximus;
    i32 pr, pg, pb;
    i32 cinereus_palette;
    i32 distantia;

    si (!palette || numerus_colorum <= ZEPHYRUM)
    {
        redde componere_rgb(cinereus, cinereus, cinereus);
    }

    distantia_minima = CCLVI;  /* Maximum distantia possibilis */
    color_proximus = palette[ZEPHYRUM];

    per (i = ZEPHYRUM; i < numerus_colorum; i++)
    {
        /* Extrahere RGB ex palette */
        extrahere_rgb(palette[i], &pr, &pg, &pb);

        /* Convertere ad cinereum */
        cinereus_palette = ad_cinereum(pr, pg, pb);

        /* Computare distantiam cineream */
        distantia = cinereus - cinereus_palette;
        si (distantia < ZEPHYRUM) distantia = -distantia;  /* Valor absolutus */

        si (distantia < distantia_minima)
        {
            distantia_minima = distantia;
            color_proximus = palette[i];
        }
    }

    redde color_proximus;
}

/* Delineare gradientum linearem cum Floyd-Steinberg dithering */
vacuum
delineare_gradientum_linearem_dithered(
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    i32                  color_initium,
    i32                  color_finis,
    b32                  horizontalis)
{
    i32 r0, g0, b0;
    i32 r1, g1, b1;
    s32* error_r;
    s32* error_g;
    s32* error_b;
    s32* error_r_next;
    s32* error_g_next;
    s32* error_b_next;
    s32* temp;
    i32 px, py;
    i32 dimension;

    si (!ctx || latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM) redde;

    /* Extrahere componentes RGB */
    extrahere_rgb(color_initium, &r0, &g0, &b0);
    extrahere_rgb(color_finis, &r1, &g1, &b1);

    /* Determinare dimensionem pro buffers erroris */
    dimension = MAXIMUM(latitudo, altitudo);

    /* Allocare buffers erroris (duo buffers per component pro scanlines) */
    error_r = (s32*)piscina_allocare(ctx->piscina, (i32)(dimension + I) * magnitudo(s32));
    error_g = (s32*)piscina_allocare(ctx->piscina, (i32)(dimension + I) * magnitudo(s32));
    error_b = (s32*)piscina_allocare(ctx->piscina, (i32)(dimension + I) * magnitudo(s32));
    error_r_next = (s32*)piscina_allocare(ctx->piscina, (i32)(dimension + I) * magnitudo(s32));
    error_g_next = (s32*)piscina_allocare(ctx->piscina, (i32)(dimension + I) * magnitudo(s32));
    error_b_next = (s32*)piscina_allocare(ctx->piscina, (i32)(dimension + I) * magnitudo(s32));

    si (!error_r || !error_g || !error_b || !error_r_next || !error_g_next || !error_b_next)
    {
        redde;
    }

    /* Initiare buffers erroris ad 0 */
    per (px = ZEPHYRUM; px <= dimension; px++)
    {
        error_r[px] = ZEPHYRUM;
        error_g[px] = ZEPHYRUM;
        error_b[px] = ZEPHYRUM;
        error_r_next[px] = ZEPHYRUM;
        error_g_next[px] = ZEPHYRUM;
        error_b_next[px] = ZEPHYRUM;
    }

    /* Implere rectangulum cum gradiente dithered */
    per (py = ZEPHYRUM; py < altitudo; py++)
    {
        per (px = ZEPHYRUM; px < latitudo; px++)
        {
            i32 t;
            i32 r_ideal, g_ideal, b_ideal;
            i32 r_actual, g_actual, b_actual;
            i32 r_quant, g_quant, b_quant;
            s32 error_r_val, error_g_val, error_b_val;
            i32 color_dithered;

            /* Computare factorem interpolationis (0-256) */
            si (horizontalis)
            {
                t = (px * CCLVI) / MAXIMUM(I, latitudo - I);
            }
            alioquin
            {
                t = (py * CCLVI) / MAXIMUM(I, altitudo - I);
            }

            /* Interpolate componentes RGB */
            r_ideal = interpolate(r0, r1, t);
            g_ideal = interpolate(g0, g1, t);
            b_ideal = interpolate(b0, b1, t);

            /* Applicare error ex pixelis praecedentibus */
            r_actual = cohibere(r_ideal + (i32)error_r[px]);
            g_actual = cohibere(g_ideal + (i32)error_g[px]);
            b_actual = cohibere(b_ideal + (i32)error_b[px]);

            /* Quantizare ad 0 vel 255 */
            r_quant = quantizare_component(r_actual);
            g_quant = quantizare_component(g_actual);
            b_quant = quantizare_component(b_actual);

            /* Computare error */
            error_r_val = (s32)(r_actual - r_quant);
            error_g_val = (s32)(g_actual - g_quant);
            error_b_val = (s32)(b_actual - b_quant);

            /* Diffundere error ad pixela vicina (Floyd-Steinberg) */
            /*        X   7/16
             *    3/16 5/16 1/16  */

            /* Dextram: 7/16 */
            si (px + I < latitudo)
            {
                error_r[px + I] += (error_r_val * VII) / XVI;
                error_g[px + I] += (error_g_val * VII) / XVI;
                error_b[px + I] += (error_b_val * VII) / XVI;
            }

            /* Deorsum-sinistram: 3/16 */
            si (py + I < altitudo && px > ZEPHYRUM)
            {
                error_r_next[px - I] += (error_r_val * III) / XVI;
                error_g_next[px - I] += (error_g_val * III) / XVI;
                error_b_next[px - I] += (error_b_val * III) / XVI;
            }

            /* Deorsum: 5/16 */
            si (py + I < altitudo)
            {
                error_r_next[px] += (error_r_val * V) / XVI;
                error_g_next[px] += (error_g_val * V) / XVI;
                error_b_next[px] += (error_b_val * V) / XVI;
            }

            /* Deorsum-dextram: 1/16 */
            si (py + I < altitudo && px + I < latitudo)
            {
                error_r_next[px + I] += error_r_val / XVI;
                error_g_next[px + I] += error_g_val / XVI;
                error_b_next[px + I] += error_b_val / XVI;
            }

            /* Componere colorem */
            color_dithered = componere_rgb(r_quant, g_quant, b_quant);

            /* Delineare pixelum */
            ponere_pixelum_internum(ctx, x + px, y + py, color_dithered);
        }

        /* Commutare buffers erroris pro proxima scanline */
        temp = error_r;
        error_r = error_r_next;
        error_r_next = temp;

        temp = error_g;
        error_g = error_g_next;
        error_g_next = temp;

        temp = error_b;
        error_b = error_b_next;
        error_b_next = temp;

        /* Vacare buffer "next" pro proxima scanline */
        per (px = ZEPHYRUM; px <= dimension; px++)
        {
            error_r_next[px] = ZEPHYRUM;
            error_g_next[px] = ZEPHYRUM;
            error_b_next[px] = ZEPHYRUM;
        }
    }
}

/* Delineare gradientum radialem cum Floyd-Steinberg dithering */
vacuum
delineare_gradientum_radialem_dithered(
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius,
    i32                  color_centrum,
    i32                  color_peripheria)
{
    i32 r0, g0, b0;
    i32 r1, g1, b1;
    s32* error_r;
    s32* error_g;
    s32* error_b;
    s32* error_r_next;
    s32* error_g_next;
    s32* error_b_next;
    s32* temp;
    i32 y_start, y_end;
    i32 x_start, x_end;
    i32 diameter;
    i32 px, py;

    si (!ctx || radius <= ZEPHYRUM) redde;

    /* Extrahere componentes RGB */
    extrahere_rgb(color_centrum, &r0, &g0, &b0);
    extrahere_rgb(color_peripheria, &r1, &g1, &b1);

    /* Computare rectangulum quo gradiens contineatur */
    y_start = centrum_y - radius;
    y_end = centrum_y + radius;
    x_start = centrum_x - radius;
    x_end = centrum_x + radius;
    diameter = radius * II + I;

    /* Allocare buffers erroris */
    error_r = (s32*)piscina_allocare(ctx->piscina, diameter * magnitudo(s32));
    error_g = (s32*)piscina_allocare(ctx->piscina, diameter * magnitudo(s32));
    error_b = (s32*)piscina_allocare(ctx->piscina, diameter * magnitudo(s32));
    error_r_next = (s32*)piscina_allocare(ctx->piscina, diameter * magnitudo(s32));
    error_g_next = (s32*)piscina_allocare(ctx->piscina, diameter * magnitudo(s32));
    error_b_next = (s32*)piscina_allocare(ctx->piscina, diameter * magnitudo(s32));

    si (!error_r || !error_g || !error_b || !error_r_next || !error_g_next || !error_b_next)
    {
        redde;
    }

    /* Initiare buffers erroris */
    per (px = ZEPHYRUM; px < diameter; px++)
    {
        error_r[px] = ZEPHYRUM;
        error_g[px] = ZEPHYRUM;
        error_b[px] = ZEPHYRUM;
        error_r_next[px] = ZEPHYRUM;
        error_g_next[px] = ZEPHYRUM;
        error_b_next[px] = ZEPHYRUM;
    }

    /* Implere circulum cum gradiente dithered */
    per (py = y_start; py <= y_end; py++)
    {
        i32 x_rel;

        x_rel = ZEPHYRUM;
        per (px = x_start; px <= x_end; px++)
        {
            i32 dx, dy;
            i32 distance_sq;
            i32 radius_sq;
            i32 distance;
            i32 t;
            i32 r_ideal, g_ideal, b_ideal;
            i32 r_actual, g_actual, b_actual;
            i32 r_quant, g_quant, b_quant;
            s32 error_r_val, error_g_val, error_b_val;
            i32 color_dithered;

            /* Computare distantiam a centro */
            dx = px - centrum_x;
            dy = py - centrum_y;
            distance_sq = dx * dx + dy * dy;
            radius_sq = radius * radius;

            /* Si extra circulum, saltare */
            si (distance_sq > radius_sq)
            {
                x_rel++;
                perge;
            }

            /* Computare distantiam linearem using integer square root */
            distance = isqrt(distance_sq);

            /* Computare factorem interpolationis (0-256) */
            si (radius == ZEPHYRUM)
            {
                t = ZEPHYRUM;
            }
            alioquin
            {
                t = (distance * CCLVI) / radius;
                si (t > CCLVI) t = CCLVI;
            }

            /* Interpolate componentes RGB */
            r_ideal = interpolate(r0, r1, t);
            g_ideal = interpolate(g0, g1, t);
            b_ideal = interpolate(b0, b1, t);

            /* Applicare error */
            r_actual = cohibere(r_ideal + (i32)error_r[x_rel]);
            g_actual = cohibere(g_ideal + (i32)error_g[x_rel]);
            b_actual = cohibere(b_ideal + (i32)error_b[x_rel]);

            /* Quantizare */
            r_quant = quantizare_component(r_actual);
            g_quant = quantizare_component(g_actual);
            b_quant = quantizare_component(b_actual);

            /* Computare error */
            error_r_val = (s32)(r_actual - r_quant);
            error_g_val = (s32)(g_actual - g_quant);
            error_b_val = (s32)(b_actual - b_quant);

            /* Diffundere error (Floyd-Steinberg) */
            si (x_rel + I < diameter)
            {
                error_r[x_rel + I] += (error_r_val * VII) / XVI;
                error_g[x_rel + I] += (error_g_val * VII) / XVI;
                error_b[x_rel + I] += (error_b_val * VII) / XVI;
            }

            si (x_rel > ZEPHYRUM)
            {
                error_r_next[x_rel - I] += (error_r_val * III) / XVI;
                error_g_next[x_rel - I] += (error_g_val * III) / XVI;
                error_b_next[x_rel - I] += (error_b_val * III) / XVI;
            }

            error_r_next[x_rel] += (error_r_val * V) / XVI;
            error_g_next[x_rel] += (error_g_val * V) / XVI;
            error_b_next[x_rel] += (error_b_val * V) / XVI;

            si (x_rel + I < diameter)
            {
                error_r_next[x_rel + I] += error_r_val / XVI;
                error_g_next[x_rel + I] += error_g_val / XVI;
                error_b_next[x_rel + I] += error_b_val / XVI;
            }

            /* Componere et delineare */
            color_dithered = componere_rgb(r_quant, g_quant, b_quant);
            ponere_pixelum_internum(ctx, px, py, color_dithered);

            x_rel++;
        }

        /* Commutare buffers erroris */
        temp = error_r;
        error_r = error_r_next;
        error_r_next = temp;

        temp = error_g;
        error_g = error_g_next;
        error_g_next = temp;

        temp = error_b;
        error_b = error_b_next;
        error_b_next = temp;

        /* Vacare next buffer */
        per (px = ZEPHYRUM; px < diameter; px++)
        {
            error_r_next[px] = ZEPHYRUM;
            error_g_next[px] = ZEPHYRUM;
            error_b_next[px] = ZEPHYRUM;
        }
    }
}


/* ==================================================
 * Gradientia cum Palette et Multi-Algorithmus
 * ================================================== */

/* Delineare gradientum linearem cum palette et algorithmo (grayscale-based) */
vacuum
delineare_gradientum_linearem_dithered_cum_palette(
    ContextusDelineandi* ctx,
    i32                  x,
    i32                  y,
    i32                  latitudo,
    i32                  altitudo,
    i32                  color_initium,
    i32                  color_finis,
    b32                  horizontalis,
    AlgorithusDithering  algorithmus,
    constans i32*        palette,
    i32                  numerus_colorum)
{
    i32 r0, g0, b0, r1, g1, b1;
    i32 gray0, gray1;
    i32 px, py;
    i32 dimension;

    si (!ctx || latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM) redde;

    extrahere_rgb(color_initium, &r0, &g0, &b0);
    extrahere_rgb(color_finis, &r1, &g1, &b1);

    /* Convertere endpoints ad cinereum */
    gray0 = ad_cinereum(r0, g0, b0);
    gray1 = ad_cinereum(r1, g1, b1);

    dimension = MAXIMUM(latitudo, altitudo);

    /* Bayer dithering (simpler, no error accumulation) */
    si (algorithmus == DITHERING_BAYER_4X4 || algorithmus == DITHERING_BAYER_8X8)
    {
        per (py = ZEPHYRUM; py < altitudo; py++)
        {
            per (px = ZEPHYRUM; px < latitudo; px++)
            {
                i32 t, gray_ideal, color_final;
                i32 actual_x, actual_y;
                i32 threshold, bayer_x, bayer_y;

                si (horizontalis)
                    t = (px * CCLVI) / MAXIMUM(I, latitudo - I);
                alioquin
                    t = (py * CCLVI) / MAXIMUM(I, altitudo - I);

                gray_ideal = interpolate(gray0, gray1, t);

                actual_x = x + px;
                actual_y = y + py;

                /* Get Bayer threshold and add to grayscale */
                si (algorithmus == DITHERING_BAYER_4X4)
                {
                    bayer_x = actual_x & III;
                    bayer_y = actual_y & III;
                    threshold = (bayer_matrix_4x4[bayer_y][bayer_x] * XVI) - CXXVIII;
                }
                alioquin  /* DITHERING_BAYER_8X8 */
                {
                    bayer_x = actual_x & VII;
                    bayer_y = actual_y & VII;
                    threshold = (bayer_matrix_8x8[bayer_y][bayer_x] * IV) - CXXVIII;
                }

                /* Apply threshold dithering, then find nearest palette color */
                gray_ideal = cohibere(gray_ideal + threshold);

                /* Map grayscale to nearest palette color */
                si (palette && numerus_colorum > ZEPHYRUM)
                    color_final = invenire_colorem_per_cinereum(gray_ideal, palette, numerus_colorum);
                alioquin
                {
                    i32 gray_quant = quantizare_component(gray_ideal);
                    color_final = componere_rgb(gray_quant, gray_quant, gray_quant);
                }

                ponere_pixelum_internum(ctx, actual_x, actual_y, color_final);
            }
        }
        redde;
    }

    /* Error diffusion (Floyd-Steinberg or Atkinson) */
    {
        s32 *error, *error_next, *temp;
        s32 offsets_x[VI], offsets_y[VI];
        i32 offsets_count, q;

        /* Allocare buffers erroris pro cinereum */
        error = (s32*)piscina_allocare(ctx->piscina, (i32)(dimension + I) * magnitudo(s32));
        error_next = (s32*)piscina_allocare(ctx->piscina, (i32)(dimension + I) * magnitudo(s32));

        si (!error || !error_next) redde;

        /* Initiare */
        per (px = ZEPHYRUM; px <= dimension; px++)
        {
            error[px] = ZEPHYRUM;
            error_next[px] = ZEPHYRUM;
        }

        /* Configurare error diffusion pattern */
        si (algorithmus == DITHERING_ATKINSON)
        {
            /* Atkinson: distributes error to 6 neighbors, 1/8 each */
            offsets_count = VI;
            offsets_x[ZEPHYRUM] = I; offsets_y[ZEPHYRUM] = ZEPHYRUM;  /* [1, 0] */
            offsets_x[I] = II; offsets_y[I] = ZEPHYRUM;  /* [2, 0] */
            offsets_x[II] = -I; offsets_y[II] = I;  /* [-1, 1] */
            offsets_x[III] = ZEPHYRUM; offsets_y[III] = I;  /* [0, 1] */
            offsets_x[IV] = I; offsets_y[IV] = I;  /* [1, 1] */
            offsets_x[V] = ZEPHYRUM; offsets_y[V] = II;  /* [0, 2] */
        }
        alioquin  /* DITHERING_FLOYD_STEINBERG */
        {
            offsets_count = III;
            /* Floyd-Steinberg handled inline below */
        }

        /* Process pixels */
        per (py = ZEPHYRUM; py < altitudo; py++)
        {
            per (px = ZEPHYRUM; px < latitudo; px++)
            {
                i32 t, gray_ideal, gray_actual, color_final;
                i32 gray_chosen, pr, pg, pb;
                s32 err;

                si (horizontalis)
                    t = (px * CCLVI) / MAXIMUM(I, latitudo - I);
                alioquin
                    t = (py * CCLVI) / MAXIMUM(I, altitudo - I);

                gray_ideal = interpolate(gray0, gray1, t);
                gray_actual = cohibere(gray_ideal + (i32)error[px]);

                /* Find nearest palette color by grayscale */
                si (palette && numerus_colorum > ZEPHYRUM)
                {
                    color_final = invenire_colorem_per_cinereum(gray_actual, palette, numerus_colorum);
                    /* Get actual grayscale of chosen color for error calculation */
                    extrahere_rgb(color_final, &pr, &pg, &pb);
                    gray_chosen = ad_cinereum(pr, pg, pb);
                }
                alioquin
                {
                    gray_chosen = quantizare_component(gray_actual);
                    color_final = componere_rgb(gray_chosen, gray_chosen, gray_chosen);
                }

                /* Compute error between ideal and actual grayscale */
                err = (s32)(gray_actual - gray_chosen);

                /* Diffundere error */
                si (algorithmus == DITHERING_ATKINSON)
                {
                    /* Atkinson: divide by 8 */
                    per (q = ZEPHYRUM; q < offsets_count; q++)
                    {
                        s32 nx = (s32)px + offsets_x[q];
                        s32 ny = offsets_y[q];
                        si (nx >= ZEPHYRUM && nx < (s32)latitudo)
                        {
                            si (ny == ZEPHYRUM)
                                error[nx] += err / VIII;
                            alioquin si (ny == I && py + I < altitudo)
                                error_next[nx] += err / VIII;
                        }
                    }
                }
                alioquin  /* Floyd-Steinberg */
                {
                    /* Right: 7/16 */
                    si (px + I < latitudo)
                        error[px + I] += (err * VII) / XVI;
                    /* Down-left: 3/16 */
                    si (py + I < altitudo && px > ZEPHYRUM)
                        error_next[px - I] += (err * III) / XVI;
                    /* Down: 5/16 */
                    si (py + I < altitudo)
                        error_next[px] += (err * V) / XVI;
                    /* Down-right: 1/16 */
                    si (py + I < altitudo && px + I < latitudo)
                        error_next[px + I] += err / XVI;
                }

                ponere_pixelum_internum(ctx, x + px, y + py, color_final);
            }

            /* Swap error buffers */
            temp = error;
            error = error_next;
            error_next = temp;

            /* Clear next buffer */
            per (px = ZEPHYRUM; px <= dimension; px++)
                error_next[px] = ZEPHYRUM;
        }
    }
}

/* Delineare gradientum radialem cum palette et algorithmo (grayscale-based) */
vacuum
delineare_gradientum_radialem_dithered_cum_palette(
    ContextusDelineandi* ctx,
    i32                  centrum_x,
    i32                  centrum_y,
    i32                  radius,
    i32                  color_centrum,
    i32                  color_peripheria,
    AlgorithusDithering  algorithmus,
    constans i32*        palette,
    i32                  numerus_colorum)
{
    i32 r0, g0, b0, r1, g1, b1;
    i32 gray0, gray1;
    i32 x_start, x_end, y_start, y_end;
    i32 px, py;

    si (!ctx || radius <= ZEPHYRUM) redde;

    extrahere_rgb(color_centrum, &r0, &g0, &b0);
    extrahere_rgb(color_peripheria, &r1, &g1, &b1);

    /* Convertere ad cinereum */
    gray0 = ad_cinereum(r0, g0, b0);
    gray1 = ad_cinereum(r1, g1, b1);

    /* Radial gradients work best with Bayer (error diffusion less effective) */
    y_start = centrum_y - radius;
    y_end = centrum_y + radius;
    x_start = centrum_x - radius;
    x_end = centrum_x + radius;

    per (py = y_start; py <= y_end; py++)
    {
        per (px = x_start; px <= x_end; px++)
        {
            i32 dx, dy, distance_sq, radius_sq;
            i32 distance, t, gray_ideal, gray_dithered, color_final;

            dx = px - centrum_x;
            dy = py - centrum_y;
            distance_sq = dx * dx + dy * dy;
            radius_sq = radius * radius;

            si (distance_sq > radius_sq) perge;

            /* Compute actual distance using integer square root */
            distance = isqrt(distance_sq);

            /* Compute interpolation factor (0-256) */
            si (radius == ZEPHYRUM)
                t = ZEPHYRUM;
            alioquin
            {
                t = (distance * CCLVI) / radius;
                si (t > CCLVI) t = CCLVI;
            }

            /* Interpolate grayscale */
            gray_ideal = interpolate(gray0, gray1, t);

            /* Apply Bayer threshold dithering
             * Note: Radial gradients always use Bayer (error diffusion doesn't work well) */
            {
                i32 threshold, bayer_x, bayer_y;

                si (algorithmus == DITHERING_BAYER_8X8)
                {
                    bayer_x = px & VII;
                    bayer_y = py & VII;
                    threshold = (bayer_matrix_8x8[bayer_y][bayer_x] * IV) - CXXVIII;
                }
                alioquin  /* Default to Bayer 4x4 for all other algorithms */
                {
                    bayer_x = px & III;
                    bayer_y = py & III;
                    threshold = (bayer_matrix_4x4[bayer_y][bayer_x] * XVI) - CXXVIII;
                }

                gray_ideal = cohibere(gray_ideal + threshold);
            }

            /* Map grayscale to palette */
            si (palette && numerus_colorum > ZEPHYRUM)
                color_final = invenire_colorem_per_cinereum(gray_ideal, palette, numerus_colorum);
            alioquin
            {
                gray_dithered = quantizare_component(gray_ideal);
                color_final = componere_rgb(gray_dithered, gray_dithered, gray_dithered);
            }

            ponere_pixelum_internum(ctx, px, py, color_final);
        }
    }
}
