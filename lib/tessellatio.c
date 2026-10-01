/* tessellatio.c - Vide tessellatio.h */

#include "tessellatio.h"
#include "color.h"
#include "thema.h"
#include "runae.h"

#define SCAENAE_MAXIMAE LXIV

/* Scaena: transformatio (rasterizatoris nativi exacte) + sectio IN
 * CELLULIS [c0, c1) x [l0, l1); radix = rete totum. */
nomen structura {
    s32 origo_x;
    s32 origo_y;
    s32 scala;
    s32 reliqua;          /* elementa reliqua sub hoc coetu */
    s32 c0;
    s32 c1;
    s32 l0;
    s32 l1;
} Scaena;

nomen structura {
    TessellatioCellula* cellulae;
                   s32  columnae;
                   s32  lineae;
      constans Modulus* modulus;
         RunaePolitica  politica;
} Contextus;

/* Codepoints delineandi per bita (SURSUM I, DEXTRA II, DEORSUM IV,
 * SINISTRA VIII); bitum solum = linea plena axis sui. */
interior constans s32 JUNCTURAE[XVI] = {
    0x0000, 0x2502, 0x2500, 0x2514,    /*   -  │  ─  └ */
    0x2502, 0x2502, 0x250C, 0x251C,    /*   │  │  ┌  ├ */
    0x2500, 0x2518, 0x2500, 0x2534,    /*   ─  ┘  ─  ┴ */
    0x2510, 0x2524, 0x252C, 0x253C     /*   ┐  ┤  ┬  ┼ */
};

ModulusMensor
tessellatio_mensor (
    RunaePolitica politica)
{
    ModulusMensor r;

    r.genus     = MODULUS_MENSOR_RUNARUM;
    r.politica  = politica;
    redde r;
}

s32
tessellatio_runa_juncturae (
    i32 juncturae)
{
    redde JUNCTURAE[juncturae & 0x0F];
}

/* ColorMandati -> 0x00RRGGBB (thema resolvit; idem ac color_ex_mandato
 * rasterizatoris nativi, sine fenestra) */
interior i32
_color (
    ColorMandati color)
{
    Color c;

    commutatio (color.genus)
    {
        casus COLOR_MANDATI_THEMA:
            c = thema_color((ColorThema)color.valor);
            frange;
        casus COLOR_MANDATI_INDEX:
            c = thema_color_ex_indice_colorationis((i8)color.valor);
            frange;
        ordinarius:
            c = color_ex_pixelum(color.valor);
            frange;
    }
    redde ((i32)c.r << XVI) | ((i32)c.g << VIII) | (i32)c.b;
}

/* Cellula (columna, linea) si intra sectionem scaenae; aliter NIHIL. */
interior TessellatioCellula*
_cellula (
    constans Contextus* ctx,
       constans Scaena* s,
                   s32  columna,
                   s32  linea)
{
    si (   columna < s->c0 || columna >= s->c1
        || linea < s->l0 || linea >= s->l1)
    {
        redde NIHIL;
    }
    redde &ctx->cellulae[linea * ctx->columnae + columna];
}

interior vacuum
_vacare (
    TessellatioCellula* c)
{
    c->unitas     = NIHIL;
    c->mensura    = ZEPHYRUM;
    c->latitudo   = I;
    c->juncturae  = ZEPHYRUM;
}

/* Contentum cellulae tollere ante scripturam: dimidium unitatis latae
 * tectum -> dimidium alterum spatium fit (regula tesserae). Rete
 * totum, non sectio: dimidium alterum extra sectionem quoque purgatur. */
interior vacuum
_tollere (
    constans Contextus* ctx,
                   s32  columna,
                   s32  linea)
{
    TessellatioCellula* c;

    c = &ctx->cellulae[linea * ctx->columnae + columna];
    si (c->latitudo == ZEPHYRUM && columna > ZEPHYRUM)
    {
        _vacare(c - I);
    }
    alioquin si (c->latitudo == II && columna + I < ctx->columnae)
    {
        _vacare(c + I);
    }
    _vacare(c);
}

interior vacuum
_textum_ponere (
    constans Contextus* ctx,
       constans Scaena* s,
     constans Mandatum* x,
                   s32  px,
                   s32  py)
{
           constans i8* p;
           constans i8* q;
           constans i8* finis;
                   s32  columna_initii;
                   s32  columna;
                   s32  linea;
                   i32  lat;
                   i32  color;
    TessellatioCellula* a;
    TessellatioCellula* b;

    color           = _color(x->color);
    columna_initii  = modulus_columna(ctx->modulus, px);
    columna         = columna_initii;
    linea           = modulus_linea(ctx->modulus, py);
    p               = x->textus.datum;
    finis           = p + x->textus.mensura;
    dum (p != NIHIL && p < finis)
    {
        si (*p == '\n')
        {
            linea++;
            columna = columna_initii;
            p++;
            perge;
        }
        q = runae_unitas_proxima(p, finis, ctx->politica, &lat);
        si (lat == ZEPHYRUM)
        {
            p = q;
            perge;
        }
        a = _cellula(ctx, s, columna, linea);
        b = (lat == II) ? _cellula(ctx, s, columna + I, linea) : NIHIL;
        si (a != NIHIL && (lat == I || b != NIHIL))
        {
            _tollere(ctx, columna, linea);
            si (b != NIHIL)
            {
                _tollere(ctx, columna + I, linea);
                b->latitudo        = ZEPHYRUM;
                b->color_litterae  = color;
            }
            a->unitas          = p;
            a->mensura         = (i32)(q - p);
            a->latitudo        = lat;
            a->color_litterae  = color;
        }
        columna  += (s32)lat;
        p        = q;
    }
}

/* Bita juncturarum in cellulam: textum tollit, aliter bita OR-antur. */
interior vacuum
_juncturas_ponere (
    constans Contextus* ctx,
       constans Scaena* s,
                   s32  columna,
                   s32  linea,
                   i32  bita,
                   i32  color)
{
    TessellatioCellula* c;

    c = _cellula(ctx, s, columna, linea);
    si (c == NIHIL || bita == ZEPHYRUM)
    {
        redde;
    }
    si (c->unitas != NIHIL || c->latitudo != I)
    {
        _tollere(ctx, columna, linea);
    }
    c->juncturae       |= bita;
    c->color_litterae  = color;
}

interior vacuum
_rectangulum_ponere (
    constans Contextus* ctx,
       constans Scaena* s,
     constans Mandatum* x,
                   s32  px,
                   s32  py)
{
                   s32  c0;
                   s32  c1;
                   s32  l0;
                   s32  l1;
                   s32  c;
                   s32  l;
                   i32  bita;
                   i32  color;
    TessellatioCellula* cellula;

    color  = _color(x->color);
    c0     = modulus_columna_proxima(ctx->modulus, px);
    c1     = modulus_columna_proxima(ctx->modulus,
        px + x->fines.latitudo * s->scala);
    l0     = modulus_linea_proxima(ctx->modulus, py);
    l1     = modulus_linea_proxima(ctx->modulus,
        py + x->fines.altitudo * s->scala);
    per (l = l0; l < l1; l++)
    {
        per (c = c0; c < c1; c++)
        {
            si (x->impletum)
            {
                cellula = _cellula(ctx, s, c, l);
                si (cellula != NIHIL)
                {
                    _tollere(ctx, c, l);
                    cellula->color_fundi     = color;
                    cellula->color_litterae  = color;
                }
                perge;
            }
            /* margo: lineae primae/ultimae dextra-sinistra, columnae
             * primae/ultimae sursum-deorsum */
            bita = ZEPHYRUM;
            si (l == l0 || l == l1 - I)
            {
                si (c > c0)
                { bita |= TESSELLATIO_JUNCTURA_SINISTRA;
                }
                si (c < c1 - I)
                { bita |= TESSELLATIO_JUNCTURA_DEXTRA;
                }
            }
            si (c == c0 || c == c1 - I)
            {
                si (l > l0)
                { bita |= TESSELLATIO_JUNCTURA_SURSUM;
                }
                si (l < l1 - I)
                { bita |= TESSELLATIO_JUNCTURA_DEORSUM;
                }
            }
            _juncturas_ponere(ctx, s, c, l, bita, color);
        }
    }
}

/* Linea axialis tantum (obliqua: via pixelorum, T4). Positio =
 * pavimentum; cellula una sola = linea plena axis sui. */
interior vacuum
_lineam_ponere (
    constans Contextus* ctx,
       constans Scaena* s,
     constans Mandatum* x)
{
    s32 initium_x;
    s32 initium_y;
    s32 finis_x;
    s32 finis_y;
    s32 a;
    s32 b;
    s32 k;
    s32 fixa;
    i32 bita;
    i32 color;

    si (x->numerus_punctorum < II)
    {
        redde;
    }
    color      = _color(x->color);
    initium_x  = s->origo_x + x->puncta[ZEPHYRUM].x * s->scala;
    initium_y  = s->origo_y + x->puncta[ZEPHYRUM].y * s->scala;
    finis_x    = s->origo_x + x->puncta[I].x * s->scala;
    finis_y    = s->origo_y + x->puncta[I].y * s->scala;
    si (initium_y == finis_y)
    {
        fixa = modulus_linea(ctx->modulus, initium_y);
        a = modulus_columna(ctx->modulus, initium_x
            < finis_x ? initium_x : finis_x);
        b = modulus_columna(ctx->modulus, initium_x
            < finis_x ? finis_x : initium_x);
        per (k = a; k <= b; k++)
        {
            bita = ZEPHYRUM;
            bita |= (k > a
                || a == b) ? TESSELLATIO_JUNCTURA_SINISTRA : 0;
            bita |= (k < b || a == b) ? TESSELLATIO_JUNCTURA_DEXTRA : 0;
            _juncturas_ponere(ctx, s, k, fixa, bita, color);
        }
    }
    alioquin si (initium_x == finis_x)
    {
        fixa = modulus_columna(ctx->modulus, initium_x);
        a = modulus_linea(ctx->modulus, initium_y
            < finis_y ? initium_y : finis_y);
        b = modulus_linea(ctx->modulus, initium_y
            < finis_y ? finis_y : initium_y);
        per (k = a; k <= b; k++)
        {
            bita = ZEPHYRUM;
            bita |= (k > a || a == b) ? TESSELLATIO_JUNCTURA_SURSUM : 0;
            bita |= (k < b
                || a == b) ? TESSELLATIO_JUNCTURA_DEORSUM : 0;
            _juncturas_ponere(ctx, s, fixa, k, bita, color);
        }
    }
}

/* Scaena nova ex coetu: transformatio nativa; sectio in cellulis per
 * margines proximos, ∩ parentis. */
interior Scaena
_scaenam_impellere (
     constans Contextus* ctx,
        constans Scaena* parens,
      constans Mandatum* coetus)
{
    Scaena s;
       s32 x0;
       s32 y0;
       s32 c0;
       s32 c1;
       s32 l0;
       s32 l1;

    s          = *parens;
    s.origo_x  = parens->origo_x + coetus->fines.x * parens->scala
               + coetus->translatio.x * parens->scala;
    s.origo_y  = parens->origo_y + coetus->fines.y * parens->scala
               + coetus->translatio.y * parens->scala;
    s.scala    = parens->scala * (coetus->scala > ZEPHYRUM
                                  ? (s32)coetus->scala : I);
    s.reliqua  = (s32)coetus->magnitudo_arboris - I;
    si (coetus->sectio)
    {
        x0 = parens->origo_x + coetus->fines.x * parens->scala;
        y0 = parens->origo_y + coetus->fines.y * parens->scala;
        c0 = modulus_columna_proxima(ctx->modulus, x0);
        c1  = modulus_columna_proxima(ctx->modulus,
            x0 + coetus->fines.latitudo * parens->scala);
        l0  = modulus_linea_proxima(ctx->modulus, y0);
        l1  = modulus_linea_proxima(ctx->modulus,
            y0 + coetus->fines.altitudo * parens->scala);
        s.c0 = c0 > parens->c0 ? c0 : parens->c0;
        s.c1 = c1 < parens->c1 ? c1 : parens->c1;
        s.l0 = l0 > parens->l0 ? l0 : parens->l0;
        s.l1 = l1 < parens->l1 ? l1 : parens->l1;
    }
    redde s;
}

vacuum
tessellatio_computare (
        constans Mandata* m,
        constans Modulus* modulus,
           RunaePolitica  politica,
                     i32  fundus,
      TessellatioCellula* exitus)
{
             Contextus  ctx;
                Scaena  scaenae[SCAENAE_MAXIMAE];
                   i32  altitudo;
                   i32  n;
                   i32  i;
                   i32  k;
                   s32  px;
                   s32  py;
     constans Mandatum* x;
                Scaena* s;

    ctx.cellulae  = exitus;
    ctx.modulus   = modulus;
    ctx.politica  = politica;
    modulus_extensio_cellularum(modulus, &ctx.columnae, &ctx.lineae);
    per (k = ZEPHYRUM; k < (i32)(ctx.columnae * ctx.lineae); k++)
    {
        _vacare(&exitus[k]);
        exitus[k].color_litterae  = fundus;
        exitus[k].color_fundi     = fundus;
    }
    si (m == NIHIL)
    {
        redde;
    }
    scaenae[ZEPHYRUM].origo_x  = ZEPHYRUM;
    scaenae[ZEPHYRUM].origo_y  = ZEPHYRUM;
    scaenae[ZEPHYRUM].scala    = I;
    scaenae[ZEPHYRUM].reliqua  = -I;   /* radix numquam cadit */
    scaenae[ZEPHYRUM].c0       = ZEPHYRUM;
    scaenae[ZEPHYRUM].c1       = ctx.columnae;
    scaenae[ZEPHYRUM].l0       = ZEPHYRUM;
    scaenae[ZEPHYRUM].l1       = ctx.lineae;
    altitudo                   = ZEPHYRUM;
    n                          = mandata_numerus(m);
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = mandata_obtinere(m, i);
        /* ambulatio = rasterizatoris nativi (magnitudo_arboris) */
        per (k = I; k <= altitudo; k++)
        {
            scaenae[k].reliqua--;
        }
        si (x->genus == MANDATUM_COETUS)
        {
            si (altitudo + I < SCAENAE_MAXIMAE)
            {
                scaenae[altitudo + I] = _scaenam_impellere(&ctx,
                    &scaenae[altitudo], x);
                altitudo++;
            }
        }
        alioquin
        {
            s   = &scaenae[altitudo];
            px  = s->origo_x + x->fines.x * s->scala;
            py  = s->origo_y + x->fines.y * s->scala;
            commutatio (x->genus)
            {
                casus MANDATUM_TEXTUS:
                    _textum_ponere(&ctx, s, x, px, py);
                    frange;
                casus MANDATUM_RECTANGULUM:
                    _rectangulum_ponere(&ctx, s, x, px, py);
                    frange;
                casus MANDATUM_LINEA:
                    _lineam_ponere(&ctx, s, x);
                    frange;
                ordinarius:
                    frange;   /* imago, polygonum: via pixelorum (T4) */
            }
        }
        dum (   altitudo > ZEPHYRUM
             && scaenae[altitudo].reliqua <= ZEPHYRUM)
        {
            altitudo--;
        }
    }
}
