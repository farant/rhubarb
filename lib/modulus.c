/* modulus.c - Vide modulus.h */

#include "modulus.h"
#include "utf8.h"

/* Divisio PAVIMENTI (divisor > 0) sine divisione negativa: C89
 * directionem eius implementationi relinquit. */
interior s32
_pavimentum (
    s32 dividendus,
    s32 divisor)
{
    si (dividendus >= ZEPHYRUM)
    {
        redde dividendus / divisor;
    }
    redde -((-dividendus + divisor - I) / divisor);
}

/* Margo cellulae proximus, dimidium sursum: pavimentum((2x + c) /
 * 2c) - exactum etiam cellula impari. */
interior s32
_proximum (
    s32 x,
    s32 cellula)
{
    redde _pavimentum(II * x + cellula, II * cellula);
}

Modulus
modulus_creare (
    s32 cellula_latitudo,
    s32 cellula_altitudo,
    s32 extensio_latitudo,
    s32 extensio_altitudo)
{
    Modulus m;

    m.cellula_latitudo          = cellula_latitudo > ZEPHYRUM
                                  ? cellula_latitudo : I;
    m.cellula_altitudo          = cellula_altitudo > ZEPHYRUM
                                  ? cellula_altitudo : I;
    m.extensio_latitudo         = extensio_latitudo > ZEPHYRUM
                                  ? extensio_latitudo : ZEPHYRUM;
    m.extensio_altitudo         = extensio_altitudo > ZEPHYRUM
                                  ? extensio_altitudo : ZEPHYRUM;
    m.schirmus_x.numerator    = I;
    m.schirmus_x.denominator  = I;
    m.schirmus_y.numerator    = I;
    m.schirmus_y.denominator  = I;
    redde m;
}

s32
modulus_columna (
    constans Modulus* m,
                 s32  x)
{
    redde _pavimentum(x, m->cellula_latitudo);
}

s32
modulus_linea (
    constans Modulus* m,
                 s32  y)
{
    redde _pavimentum(y, m->cellula_altitudo);
}

s32
modulus_columna_proxima (
    constans Modulus* m,
                 s32  x)
{
    redde _proximum(x, m->cellula_latitudo);
}

s32
modulus_linea_proxima (
    constans Modulus* m,
                 s32  y)
{
    redde _proximum(y, m->cellula_altitudo);
}

vacuum
modulus_extensio_cellularum (
    constans Modulus* m,
                 s32* columnae,
                 s32* lineae)
{
    *columnae  = m->extensio_latitudo / m->cellula_latitudo;
    *lineae    = m->extensio_altitudo / m->cellula_altitudo;
}

/* Columnae lineae [initium, finis) - finis ante '\n' aut finem. */
interior s32
_columnae_lineae (
         ModulusMensor  mensor,
           constans i8* initium,
           constans i8* finis)
{
    constans i8* p;
            s32  columnae;

    si (mensor.genus == MODULUS_MENSOR_RUNARUM)
    {
        redde (s32)runae_latitudo_textus(initium, finis,
            mensor.politica);
    }
    /* FONTIS: fenestra_textus pingit glyphum unum per gradum
     * utf8_decodere (invalidus quoque: TOFU, octetus unus) - idem
     * decodificator, ergo eadem mensura per constructionem */
    columnae  = ZEPHYRUM;
    p         = initium;
    dum (p < finis)
    {
        (vacuum)utf8_decodere(&p, finis);
        columnae++;
    }
    redde columnae;
}

vacuum
modulus_textum_metiri (
    constans Modulus* m,
       ModulusMensor  mensor,
              chorda  textus,
                 s32* latitudo,
                 s32* altitudo)
{
    constans i8* initium;
    constans i8* finis;
    constans i8* p;
            s32  maxima;
            s32  columnae;
            s32  lineae;

    *latitudo = ZEPHYRUM;
    *altitudo = ZEPHYRUM;
    si (textus.datum == NIHIL || textus.mensura <= ZEPHYRUM)
    {
        redde;
    }
    finis    = textus.datum + textus.mensura;
    initium  = textus.datum;
    maxima   = ZEPHYRUM;
    lineae   = I;
    /* '\n' (0x0A) numquam intra runam multi-octetam iacet (octeti
     * eius bitum altum ferunt): octetos quaerere tutum est */
    per (p = initium; p <= finis; p++)
    {
        si (p == finis || *p == '\n')
        {
            columnae  = _columnae_lineae(mensor, initium, p);
            maxima    = columnae > maxima ? columnae : maxima;
            si (p < finis)
            {
                lineae++;
                initium = p + I;
            }
        }
    }
    *latitudo = maxima * m->cellula_latitudo;
    *altitudo = lineae * m->cellula_altitudo;
}
