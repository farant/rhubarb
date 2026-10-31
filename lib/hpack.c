/* hpack.c - compressio capitum HTTP/2 (RFC 7541). Vide include/hpack.h.
 *
 * T1: integra (5.1), Huffman (5.2 + Appendix B, tabula in
 * hpack_huffman.c), tabula statica (Appendix A). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "hpack.h"
#include "hpack_huffman.h"
#include <string.h>

/* longitudo maxima codicis Huffman (EOS) */
#define HUFFMAN_LONGITUDO_MAXIMA  XXX

/* Appendix A: nomina et valores concatenata (sine NUL); TABULA dat
 * (initium tituli, longitudo, initium valoris, longitudo) */
hic_manens character HPACK_STATICA_TEXTUS[] =
    ":authority:methodGET:methodPOST:path/:path/index.html:sc"
    "hemehttp:schemehttps:status200:status204:status206:statu"
    "s304:status400:status404:status500accept-charsetaccept-e"
    "ncodinggzip, deflateaccept-languageaccept-rangesacceptac"
    "cess-control-allow-originageallowauthorizationcache-cont"
    "rolcontent-dispositioncontent-encodingcontent-languageco"
    "ntent-lengthcontent-locationcontent-rangecontent-typecoo"
    "kiedateetagexpectexpiresfromhostif-matchif-modified-sinc"
    "eif-none-matchif-rangeif-unmodified-sincelast-modifiedli"
    "nklocationmax-forwardsproxy-authenticateproxy-authorizat"
    "ionrangerefererrefreshretry-afterserverset-cookiestrict-"
    "transport-securitytransfer-encodinguser-agentvaryviawww-"
    "authenticate";

hic_manens constans i32 HPACK_STATICA_TABULA[HPACK_STATICA_NUMERUS][IV] =
    {
    { 0x000, 0x00a, 0x00a, 0x000 },
    { 0x00a, 0x007, 0x011, 0x003 },
    { 0x014, 0x007, 0x01b, 0x004 },
    { 0x01f, 0x005, 0x024, 0x001 },
    { 0x025, 0x005, 0x02a, 0x00b },
    { 0x035, 0x007, 0x03c, 0x004 },
    { 0x040, 0x007, 0x047, 0x005 },
    { 0x04c, 0x007, 0x053, 0x003 },
    { 0x056, 0x007, 0x05d, 0x003 },
    { 0x060, 0x007, 0x067, 0x003 },
    { 0x06a, 0x007, 0x071, 0x003 },
    { 0x074, 0x007, 0x07b, 0x003 },
    { 0x07e, 0x007, 0x085, 0x003 },
    { 0x088, 0x007, 0x08f, 0x003 },
    { 0x092, 0x00e, 0x0a0, 0x000 },
    { 0x0a0, 0x00f, 0x0af, 0x00d },
    { 0x0bc, 0x00f, 0x0cb, 0x000 },
    { 0x0cb, 0x00d, 0x0d8, 0x000 },
    { 0x0d8, 0x006, 0x0de, 0x000 },
    { 0x0de, 0x01b, 0x0f9, 0x000 },
    { 0x0f9, 0x003, 0x0fc, 0x000 },
    { 0x0fc, 0x005, 0x101, 0x000 },
    { 0x101, 0x00d, 0x10e, 0x000 },
    { 0x10e, 0x00d, 0x11b, 0x000 },
    { 0x11b, 0x013, 0x12e, 0x000 },
    { 0x12e, 0x010, 0x13e, 0x000 },
    { 0x13e, 0x010, 0x14e, 0x000 },
    { 0x14e, 0x00e, 0x15c, 0x000 },
    { 0x15c, 0x010, 0x16c, 0x000 },
    { 0x16c, 0x00d, 0x179, 0x000 },
    { 0x179, 0x00c, 0x185, 0x000 },
    { 0x185, 0x006, 0x18b, 0x000 },
    { 0x18b, 0x004, 0x18f, 0x000 },
    { 0x18f, 0x004, 0x193, 0x000 },
    { 0x193, 0x006, 0x199, 0x000 },
    { 0x199, 0x007, 0x1a0, 0x000 },
    { 0x1a0, 0x004, 0x1a4, 0x000 },
    { 0x1a4, 0x004, 0x1a8, 0x000 },
    { 0x1a8, 0x008, 0x1b0, 0x000 },
    { 0x1b0, 0x011, 0x1c1, 0x000 },
    { 0x1c1, 0x00d, 0x1ce, 0x000 },
    { 0x1ce, 0x008, 0x1d6, 0x000 },
    { 0x1d6, 0x013, 0x1e9, 0x000 },
    { 0x1e9, 0x00d, 0x1f6, 0x000 },
    { 0x1f6, 0x004, 0x1fa, 0x000 },
    { 0x1fa, 0x008, 0x202, 0x000 },
    { 0x202, 0x00c, 0x20e, 0x000 },
    { 0x20e, 0x012, 0x220, 0x000 },
    { 0x220, 0x013, 0x233, 0x000 },
    { 0x233, 0x005, 0x238, 0x000 },
    { 0x238, 0x007, 0x23f, 0x000 },
    { 0x23f, 0x007, 0x246, 0x000 },
    { 0x246, 0x00b, 0x251, 0x000 },
    { 0x251, 0x006, 0x257, 0x000 },
    { 0x257, 0x00a, 0x261, 0x000 },
    { 0x261, 0x019, 0x27a, 0x000 },
    { 0x27a, 0x011, 0x28b, 0x000 },
    { 0x28b, 0x00a, 0x295, 0x000 },
    { 0x295, 0x004, 0x299, 0x000 },
    { 0x299, 0x003, 0x29c, 0x000 },
    { 0x29c, 0x010, 0x2ac, 0x000 }
};


/* ==================================================
 * Integra (5.1)
 * ================================================== */

i32
hpack_integrum_scribere (
    i32  valor,
    i32  prefixum,
    i32  primus,
     i8* exitus,
    i32  capacitas)
{
    i32 maximus;
    i32 n;

    si (   !exitus || prefixum < I || prefixum > VIII
        || capacitas == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    maximus  = ((i32)I << prefixum) - I;
    primus   &= 0xff ^ maximus;
    si (valor < maximus)
    {
        exitus[ZEPHYRUM] = (i8)(primus | valor);
        redde I;
    }
    exitus[ZEPHYRUM]  = (i8)(primus | maximus);
    n                 = I;
    valor             -= maximus;
    dum (valor >= 0x80)
    {
        si (n >= capacitas)
        {
            redde ZEPHYRUM;
        }
        exitus[n++]  = (i8)((valor & 0x7f) | 0x80);
        valor        >>= VII;
    }
    si (n >= capacitas)
    {
        redde ZEPHYRUM;
    }
    exitus[n++] = (i8)valor;
    redde n;
}

b32
hpack_integrum_legere (
    constans i8* datum,
            i32  n,
            i32  prefixum,
            i32* valor,
            i32* consumpti)
{
    i32 maximus;
    i64 summa;
    i32 gradus;
    i32 k;
    i32 octetus;

    si (   !datum || n == ZEPHYRUM || prefixum < I || prefixum > VIII
        || !valor || !consumpti)
    {
        redde FALSUM;
    }
    maximus  = ((i32)I << prefixum) - I;
    summa    = (i64)(datum[ZEPHYRUM] & maximus);
    k        = I;
    si (summa < (i64)maximus)
    {
        *valor      = (i32)summa;
        *consumpti  = I;
        redde VERUM;
    }
    gradus = ZEPHYRUM;
    dum (k < n)
    {
        octetus  = datum[k++];
        si (gradus > XXVIII)
        {
            redde FALSUM;
        }
        summa   += (i64)(octetus & 0x7f) << gradus;
        gradus  += VII;
        si (summa > (i64)0x7fffffff)
        {
            redde FALSUM;
        }
        si (!(octetus & 0x80))
        {
            *valor      = (i32)summa;
            *consumpti  = k;
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Huffman (5.2)
 * ================================================== */

i32
hpack_huffman_longitudo (
    chorda s)
{
    i64 bita;
    i32 i;

    bita = ZEPHYRUM;
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        bita += (i64)HPACK_HUFFMAN_LONGITUDINES[s.datum[i]];
    }
    redde (i32)((bita + VII) / VIII);
}

i32
hpack_huffman_scribere (
     chorda  s,
         i8* exitus,
        i32  capacitas)
{
    i64 acervus;    /* bita nondum scripta, inferiora */
    i32 numerus;    /* quot bita in acervo */
    i32 n;
    i32 i;

    si (!exitus || hpack_huffman_longitudo(s) > capacitas)
    {
        redde ZEPHYRUM;
    }
    acervus  = ZEPHYRUM;
    numerus  = ZEPHYRUM;
    n        = ZEPHYRUM;
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i32 longitudo = HPACK_HUFFMAN_LONGITUDINES[s.datum[i]];

        acervus  = (acervus << longitudo)
                 | (i64)HPACK_HUFFMAN_CODICES[s.datum[i]];
        numerus += longitudo;
        dum (numerus >= VIII)
        {
            numerus      -= VIII;
            exitus[n++]  = (i8)((acervus >> numerus) & 0xff);
        }
    }
    si (numerus > ZEPHYRUM)
    {
        /* impletio: bita superiora EOS (omnia unum) */
        exitus[n++] = (i8)(((acervus << (VIII - numerus))
                           | (((i64)I << (VIII - numerus)) - I))
                          & 0xff);
    }
    redde n;
}

/* symbola ordinata per (longitudo, codex); per longitudinem initium et
 * numerus in ORDO - semel computata (tabula constans, ergo idem
 * semper) */
hic_manens i32 ORDO[HPACK_HUFFMAN_SYMBOLA];
hic_manens i32 INITIUM[HUFFMAN_LONGITUDO_MAXIMA + I];
hic_manens i32 NUMERUS[HUFFMAN_LONGITUDO_MAXIMA + I];
hic_manens b32 ORDO_PARATUS = FALSUM;

interior vacuum
_ordinem_parare (vacuum)
{
    i32 l;
    i32 s;
    i32 k;

    k = ZEPHYRUM;
    per (l = ZEPHYRUM; l <= HUFFMAN_LONGITUDO_MAXIMA; l++)
    {
        INITIUM[l] = k;
        NUMERUS[l] = ZEPHYRUM;
        per (s = ZEPHYRUM; s < HPACK_HUFFMAN_SYMBOLA; s++)
        {
            si ((i32)HPACK_HUFFMAN_LONGITUDINES[s] == l)
            {
                ORDO[k++] = s;
                NUMERUS[l]++;
            }
        }
    }
    ORDO_PARATUS = VERUM;
}

/* symbolum codicis 'codex' longitudinis 'l', aut -1 (ut i32 maximus) */
interior i32
_symbolum (
    i32 codex,
    i32 l)
{
    i32 i;

    per (i = INITIUM[l]; i < INITIUM[l] + NUMERUS[l]; i++)
    {
        si (HPACK_HUFFMAN_CODICES[ORDO[i]] == codex)
        {
            redde ORDO[i];
        }
    }
    redde (i32)HPACK_HUFFMAN_SYMBOLA;
}

b32
hpack_huffman_legere (
    constans i8* datum,
            i32  n,
        Piscina* piscina,
         chorda* exitus)
{
     i8* scriptum;
    i32  m;
    i32  codex;
    i32  l;
    i32  i;
    i32  b;

    si (!exitus || !piscina || (n > ZEPHYRUM && !datum))
    {
        redde FALSUM;
    }
    si (!ORDO_PARATUS)
    {
        _ordinem_parare();
    }
    /* codex brevissimus V bitorum: plus VIII/V symbolis per octetum
     * numquam */
    scriptum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(n * VIII / V + I));
    si (!scriptum)
    {
        redde FALSUM;
    }
    m      = ZEPHYRUM;
    codex  = ZEPHYRUM;
    l      = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (b = VII; b != (i32)-I; b--)
        {
            i32 s;

            codex = (codex << I) | (((i32)datum[i] >> b) & I);
            l++;
            si (l > HUFFMAN_LONGITUDO_MAXIMA)
            {
                redde FALSUM;
            }
            s = _symbolum(codex, l);
            si (s == HPACK_HUFFMAN_EOS)
            {
                redde FALSUM;
            }
            si (s < HPACK_HUFFMAN_EOS)
            {
                scriptum[m++]  = (i8)s;
                codex          = ZEPHYRUM;
                l              = ZEPHYRUM;
            }
        }
    }
    /* impletio: plus VII bita aut non omnia unum -> error */
    si (l > VII || codex != (((i32)I << l) - I))
    {
        redde FALSUM;
    }
    exitus->datum    = scriptum;
    exitus->mensura  = m;
    redde VERUM;
}


/* ==================================================
 * Tabula statica (Appendix A)
 * ================================================== */

b32
hpack_staticum (
       i32  index,
    chorda* titulus,
    chorda* valor)
{
    constans i32* t;

    si (   index < I || index > HPACK_STATICA_NUMERUS || !titulus
        || !valor)
    {
        redde FALSUM;
    }
    t                 = HPACK_STATICA_TABULA[index - I];
    titulus->datum    = (i8*)(HPACK_STATICA_TEXTUS + t[ZEPHYRUM]);
    titulus->mensura  = t[I];
    valor->datum      = (i8*)(HPACK_STATICA_TEXTUS + t[II]);
    valor->mensura    = t[III];
    redde VERUM;
}
