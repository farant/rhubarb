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


/* ==================================================
 * T2: tabula dynamica (4)
 * ================================================== */

/* 4.1: magnitudo entrii = titulus + valor + XXXII */
#define ENTRIUM_SUPERFLUUM  XXXII

nomen structura {
    i32 initium;      /* in octeti */
    i32 titulus_n;
    i32 valor_n;
} Entrium;

/* entria vetustissimum ad 'primum'; octeti entriorum contigui, ordine
 * eodem, in [primus_octetus, finis_octetus) - compactio cum finis
 * capacitatem tangit. Umbrae: copiae pro restitutione codificatoris
 * cum capacitas exitus deficit. */
structura HpackTabula {
             i8* octeti;
             i8* umbra_octetorum;
        Entrium* entria;
        Entrium* umbra_entriorum;
            i32  capacitas;
            i32  entria_capacitas;
            i32  primus_octetus;
            i32  finis_octetus;
            i32  primum;
            i32  numerus;
            i32  usa;            /* summa magnitudinum entriorum */
            i32  maxima;         /* magnitudo maxima currens */
            i32  limes;          /* decodator: renovatio maxima */
            i32  petita;         /* codificator: renovatio finalis */
            i32  petita_minima;  /* ... et minima intercedens */
            b32  renovatio;
   HpackHuffman  modus;
};

HpackTabula*
hpack_tabulam_creare (
    Piscina* piscina,
        i32  mensura)
{
    HpackTabula* t;

    si (!piscina)
    {
        redde NIHIL;
    }
    t = (HpackTabula*)piscina_allocare(piscina, magnitudo(HpackTabula));
    si (!t)
    {
        redde NIHIL;
    }
    memset(t, ZEPHYRUM, magnitudo(HpackTabula));
    t->capacitas         = mensura;
    t->entria_capacitas  = mensura / ENTRIUM_SUPERFLUUM + I;
    t->octeti            = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + I));
    t->umbra_octetorum   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + I));
    t->entria            = (Entrium*)piscina_allocare(piscina,
        (memoriae_index)t->entria_capacitas * magnitudo(Entrium));
    t->umbra_entriorum   = (Entrium*)piscina_allocare(piscina,
        (memoriae_index)t->entria_capacitas * magnitudo(Entrium));
    si (   !t->octeti || !t->umbra_octetorum || !t->entria
        || !t->umbra_entriorum)
    {
        redde NIHIL;
    }
    t->maxima  = mensura;
    t->limes   = mensura;
    t->modus   = HPACK_HUFFMAN_SI_BREVIOR;
    redde t;
}

interior i32
_pondus (
    constans Entrium* e)
{
    redde e->titulus_n + e->valor_n + ENTRIUM_SUPERFLUUM;
}

interior vacuum
_evincere (
    HpackTabula* t)
{
    t->usa -= _pondus(&t->entria[t->primum]);
    t->primum++;
    t->numerus--;
    si (t->numerus == ZEPHYRUM)
    {
        t->primum          = ZEPHYRUM;
        t->primus_octetus  = ZEPHYRUM;
        t->finis_octetus   = ZEPHYRUM;
    }
    alioquin
    {
        t->primus_octetus = t->entria[t->primum].initium;
    }
}

interior vacuum
_magnitudinem (
    HpackTabula* t,
            i32  maxima)
{
    t->maxima = maxima;
    dum (t->numerus > ZEPHYRUM && t->usa > t->maxima)
    {
        _evincere(t);
    }
}

/* 4.4: entrium maius maxima tabulam vacuat nec additur */
interior vacuum
_addere (
    HpackTabula* t,
         chorda  titulus,
         chorda  valor)
{
    i32 m;
    i32 octeti;
    i32 i;

    m = titulus.mensura + valor.mensura + ENTRIUM_SUPERFLUUM;
    dum (t->numerus > ZEPHYRUM && t->usa + m > t->maxima)
    {
        _evincere(t);
    }
    si (m > t->maxima)
    {
        redde;
    }
    octeti = titulus.mensura + valor.mensura;
    si (t->finis_octetus + octeti > t->capacitas)
    {
        memmove(t->octeti, t->octeti + t->primus_octetus,
            (size_t)(t->finis_octetus - t->primus_octetus));
        per (i = ZEPHYRUM; i < t->numerus; i++)
        {
            t->entria[t->primum + i].initium -= t->primus_octetus;
        }
        t->finis_octetus   -= t->primus_octetus;
        t->primus_octetus  = ZEPHYRUM;
    }
    si (t->primum + t->numerus >= t->entria_capacitas)
    {
        memmove(t->entria, t->entria + t->primum,
            (size_t)t->numerus * magnitudo(Entrium));
        t->primum = ZEPHYRUM;
    }
    memcpy(t->octeti + t->finis_octetus, titulus.datum,
        (size_t)titulus.mensura);
    memcpy(t->octeti + t->finis_octetus + titulus.mensura, valor.datum,
        (size_t)valor.mensura);
    t->entria[t->primum + t->numerus].initium    = t->finis_octetus;
    t->entria[t->primum + t->numerus].titulus_n  = titulus.mensura;
    t->entria[t->primum + t->numerus].valor_n    = valor.mensura;
    t->finis_octetus                             += octeti;
    t->numerus++;
    t->usa += m;
}

i32
hpack_tabula_numerus (
    constans HpackTabula* t)
{
    redde t ? t->numerus : ZEPHYRUM;
}

i32
hpack_tabula_magnitudo (
    constans HpackTabula* t)
{
    redde t ? t->usa : ZEPHYRUM;
}

b32
hpack_tabula_obtinere (
    constans HpackTabula* t,
                     i32  i,
                  chorda* titulus,
                  chorda* valor)
{
    constans Entrium* e;

    si (!t || !titulus || !valor || i < I || i > t->numerus)
    {
        redde FALSUM;
    }
    e                 = &t->entria[t->primum + t->numerus - i];
    titulus->datum    = t->octeti + e->initium;
    titulus->mensura  = e->titulus_n;
    valor->datum      = t->octeti + e->initium + e->titulus_n;
    valor->mensura    = e->valor_n;
    redde VERUM;
}

/* spatium indicum commune (2.3.3): I..LXI statica, deinde dynamica */
interior b32
_indicatum (
    constans HpackTabula* t,
                     i32  index,
                  chorda* titulus,
                  chorda* valor)
{
    si (index <= HPACK_STATICA_NUMERUS)
    {
        redde hpack_staticum(index, titulus, valor);
    }
    redde hpack_tabula_obtinere(t, index - HPACK_STATICA_NUMERUS,
        titulus, valor);
}

vacuum
hpack_magnitudinem_ponere (
    HpackTabula* t,
            i32  mensura)
{
    i32 m;

    si (!t)
    {
        redde;
    }
    m         = mensura < t->capacitas ? mensura : t->capacitas;
    t->limes  = m;
    si (!t->renovatio || m < t->petita_minima)
    {
        t->petita_minima = m;
    }
    t->petita = m;
    si (m != t->maxima || t->petita_minima != t->maxima)
    {
        t->renovatio = VERUM;
    }
}

vacuum
hpack_huffman_ponere (
     HpackTabula* t,
    HpackHuffman  modus)
{
    si (t)
    {
        t->modus = modus;
    }
}


/* ==================================================
 * T2: decodator (6)
 * ================================================== */

interior chorda
_copia (
      chorda  s,
     Piscina* piscina)
{
    chorda r;

    r.mensura  = s.mensura;
    r.datum    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(s.mensura + I));
    si (r.datum && s.mensura > ZEPHYRUM)
    {
        memcpy(r.datum, s.datum, (size_t)s.mensura);
    }
    redde r;
}

/* integrum ad pos: truncatum (nullus octetus terminans) an nimium */
interior HpackError
_integrum (
    constans i8* datum,
            i32  n,
            i32  pos,
            i32  prefixum,
            i32* valor,
            i32* consumpti)
{
    i32 j;

    si (pos >= n)
    {
        redde HPACK_ERROR_TRUNCATUM;
    }
    si (hpack_integrum_legere(datum + pos, n - pos, prefixum, valor,
            consumpti))
    {
        redde HPACK_BENE;
    }
    per (j = pos + I; j < n; j++)
    {
        si (!(datum[j] & 0x80))
        {
            redde HPACK_ERROR_INTEGRUM;
        }
    }
    redde HPACK_ERROR_TRUNCATUM;
}

/* chorda literalis (5.2) ad *pos, in piscina */
interior HpackError
_chordam_legere (
    constans i8* datum,
            i32  n,
            i32* pos,
        Piscina* piscina,
         chorda* exitus)
{
    HpackError e;
           b32 huffman;
           i32 longitudo;
           i32 k;

    si (*pos >= n)
    {
        redde HPACK_ERROR_TRUNCATUM;
    }
    huffman  = (datum[*pos] & 0x80) != ZEPHYRUM;
    e        = _integrum(datum, n, *pos, VII, &longitudo, &k);
    si (e != HPACK_BENE)
    {
        redde e;
    }
    *pos += k;
    si (longitudo > n - *pos)
    {
        redde HPACK_ERROR_TRUNCATUM;
    }
    si (huffman)
    {
        si (!hpack_huffman_legere(datum + *pos, longitudo, piscina,
                exitus))
        {
            redde HPACK_ERROR_HUFFMAN;
        }
    }
    alioquin
    {
        exitus->mensura  = longitudo;
        exitus->datum    = (i8*)piscina_allocare(piscina,
            (memoriae_index)(longitudo + I));
        si (!exitus->datum)
        {
            redde HPACK_ERROR_NIMIA;
        }
        memcpy(exitus->datum, datum + *pos, (size_t)longitudo);
    }
    *pos += longitudo;
    redde HPACK_BENE;
}

HpackError
hpack_decodere (
    HpackTabula* t,
    constans i8* datum,
            i32  n,
            i32  capita_maxima,
        Piscina* piscina,
            Xar* capita)
{
    HpackError  e;
           i32  pos;
           i32  k;
           i32  index;
           i32  prefixum;
           b32  initium;
           b32  indicare;
           i64  summa;
    HpackCaput  h;
    HpackCaput* locus;

    si (!t || (n > ZEPHYRUM && !datum) || !piscina || !capita)
    {
        redde HPACK_ERROR_TRUNCATUM;
    }
    pos      = ZEPHYRUM;
    initium  = VERUM;
    summa    = ZEPHYRUM;
    dum (pos < n)
    {
        i32 b = datum[pos];

        h.numquam  = FALSUM;
        indicare   = FALSUM;
        si (b & 0x80)
        {
            /* 6.1 indicatum */
            e = _integrum(datum, n, pos, VII, &index, &k);
            si (e != HPACK_BENE)
            {
                redde e;
            }
            pos += k;
            si (   index == ZEPHYRUM
                || !_indicatum(t, index, &h.titulus, &h.valor))
            {
                redde HPACK_ERROR_INDEX;
            }
            h.titulus  = _copia(h.titulus, piscina);
            h.valor    = _copia(h.valor, piscina);
        }
        alioquin si ((b & 0xe0) == 0x20)
        {
            /* 6.3 renovatio magnitudinis: solum initio, intra limitem */
            e = _integrum(datum, n, pos, V, &index, &k);
            si (e != HPACK_BENE)
            {
                redde e;
            }
            pos += k;
            si (!initium || index > t->limes)
            {
                redde HPACK_ERROR_MAGNITUDO;
            }
            _magnitudinem(t, index);
            perge;
        }
        alioquin
        {
            /* 6.2 literalia: cum indice (01), sine (0000), numquam
             * (0001) */
            si ((b & 0xc0) == 0x40)
            {
                prefixum = VI;
                indicare = VERUM;
            }
            alioquin
            {
                prefixum   = IV;
                h.numquam  = (b & 0x10) != ZEPHYRUM;
            }
            e = _integrum(datum, n, pos, prefixum, &index, &k);
            si (e != HPACK_BENE)
            {
                redde e;
            }
            pos += k;
            si (index == ZEPHYRUM)
            {
                e = _chordam_legere(datum, n, &pos, piscina,
                    &h.titulus);
                si (e != HPACK_BENE)
                {
                    redde e;
                }
            }
            alioquin
            {
                chorda ignotum;

                si (!_indicatum(t, index, &h.titulus, &ignotum))
                {
                    redde HPACK_ERROR_INDEX;
                }
                h.titulus = _copia(h.titulus, piscina);
            }
            e = _chordam_legere(datum, n, &pos, piscina, &h.valor);
            si (e != HPACK_BENE)
            {
                redde e;
            }
        }
        initium  = FALSUM;
        summa   += (i64)h.titulus.mensura + (i64)h.valor.mensura
                 + (i64)ENTRIUM_SUPERFLUUM;
        si (capita_maxima > ZEPHYRUM && summa > (i64)capita_maxima)
        {
            redde HPACK_ERROR_NIMIA;
        }
        locus = (HpackCaput*)xar_addere(capita);
        si (!locus)
        {
            redde HPACK_ERROR_NIMIA;
        }
        *locus = h;
        si (indicare)
        {
            _addere(t, h.titulus, h.valor);
        }
    }
    redde HPACK_BENE;
}


/* ==================================================
 * T2: codificator (http2-plan C2)
 * ================================================== */

interior b32
_secretum (
    constans HpackCaput* h)
{
    redde h->numquam
        || chorda_aequalis_literis(h->titulus, "authorization")
        || chorda_aequalis_literis(h->titulus, "proxy-authorization")
        || (   chorda_aequalis_literis(h->titulus, "cookie")
            && h->valor.mensura < XX);
}

/* index paris exacti (statica, deinde dynamica recentissima primum),
 * aliter tituli; 0 nullum */
interior i32
_quaerere (
    constans HpackTabula* t,
                  chorda  titulus,
                  chorda  valor,
                     b32* exactum)
{
    chorda n;
    chorda v;
       i32 i;
       i32 gradus;

    *exactum = FALSUM;
    per (gradus = ZEPHYRUM; gradus < II; gradus++)
    {
        per (i = I; i <= HPACK_STATICA_NUMERUS + t->numerus; i++)
        {
            (vacuum)_indicatum(t, i, &n, &v);
            si (!chorda_aequalis(n, titulus))
            {
                perge;
            }
            si (gradus == I || chorda_aequalis(v, valor))
            {
                *exactum = gradus == ZEPHYRUM;
                redde i;
            }
        }
    }
    redde ZEPHYRUM;
}

interior b32
_integrum_scribere (
    i32  valor,
    i32  prefixum,
    i32  primus,
     i8* exitus,
    i32  capacitas,
    i32* pos)
{
    i32 k;

    k = hpack_integrum_scribere(valor, prefixum, primus, exitus + *pos,
        capacitas - *pos);
    *pos += k;
    redde k > ZEPHYRUM;
}

interior b32
_chordam_scribere (
    constans HpackTabula* t,
                  chorda  s,
                      i8* exitus,
                     i32  capacitas,
                     i32* pos)
{
    i32 longitudo_h;
    b32 huffman;

    longitudo_h  = hpack_huffman_longitudo(s);
    huffman      = t->modus == HPACK_HUFFMAN_SEMPER
                || (t->modus == HPACK_HUFFMAN_SI_BREVIOR
                    && longitudo_h < s.mensura);
    si (!_integrum_scribere(huffman ? longitudo_h : s.mensura, VII,
            huffman ? 0x80 : ZEPHYRUM, exitus, capacitas, pos))
    {
        redde FALSUM;
    }
    si (huffman)
    {
        si (longitudo_h > capacitas - *pos)
        {
            redde FALSUM;
        }
        (vacuum)hpack_huffman_scribere(s, exitus + *pos,
            capacitas - *pos);
        *pos += longitudo_h;
        redde VERUM;
    }
    si (s.mensura > capacitas - *pos)
    {
        redde FALSUM;
    }
    memcpy(exitus + *pos, s.datum, (size_t)s.mensura);
    *pos += s.mensura;
    redde VERUM;
}

/* literale: praefixum/primus repraesentationis, titulus indicatus aut
 * literalis, valor */
interior b32
_formam_scribere (
    constans HpackTabula* t,
     constans HpackCaput* h,
                     i32  index,
                     i32  prefixum,
                     i32  primus,
                      i8* exitus,
                     i32  capacitas,
                     i32* pos)
{
    redde _integrum_scribere(index, prefixum, primus, exitus,
               capacitas, pos)
        && (index != ZEPHYRUM
            || _chordam_scribere(t, h->titulus, exitus, capacitas, pos))
        && _chordam_scribere(t, h->valor, exitus, capacitas, pos);
}

i32
hpack_codificare (
            HpackTabula* t,
    constans HpackCaput* capita,
                    i32  numerus,
                     i8* exitus,
                    i32  capacitas)
{
    HpackTabula salva;
            i32 pos;
            i32 i;
            i32 index;
            b32 exactum;
            b32 bene;

    si (!t || (numerus > ZEPHYRUM && !capita) || !exitus)
    {
        redde ZEPHYRUM;
    }
    salva = *t;
    memcpy(t->umbra_octetorum, t->octeti, (size_t)t->capacitas);
    memcpy(t->umbra_entriorum, t->entria,
        (size_t)t->entria_capacitas * magnitudo(Entrium));
    pos   = ZEPHYRUM;
    bene  = VERUM;
    si (t->renovatio)
    {
        /* 4.2: minima intercedens prius, deinde finalis */
        si (t->petita_minima < t->petita)
        {
            bene = _integrum_scribere(t->petita_minima, V, 0x20, exitus,
                capacitas, &pos);
            _magnitudinem(t, t->petita_minima);
        }
        bene = bene && _integrum_scribere(t->petita, V, 0x20, exitus,
            capacitas, &pos);
        _magnitudinem(t, t->petita);
        t->renovatio = FALSUM;
    }
    per (i = ZEPHYRUM; bene && i < numerus; i++)
    {
        constans HpackCaput* h = &capita[i];

        index = _quaerere(t, h->titulus, h->valor, &exactum);
        si (_secretum(h))
        {
            bene = _formam_scribere(t, h, index, IV, 0x10, exitus,
                capacitas,
                &pos);
        }
        alioquin si (exactum)
        {
            bene = _integrum_scribere(index, VII, 0x80, exitus,
                capacitas, &pos);
        }
        alioquin si (h->titulus.mensura + h->valor.mensura
                     + ENTRIUM_SUPERFLUUM > t->maxima)
        {
            bene = _formam_scribere(t, h, index, IV, ZEPHYRUM, exitus,
                capacitas, &pos);
        }
        alioquin
        {
            bene = _formam_scribere(t, h, index, VI, 0x40, exitus,
                capacitas,
                &pos);
            si (bene)
            {
                _addere(t, h->titulus, h->valor);
            }
        }
    }
    si (!bene)
    {
        *t = salva;
        memcpy(t->octeti, t->umbra_octetorum, (size_t)t->capacitas);
        memcpy(t->entria, t->umbra_entriorum,
            (size_t)t->entria_capacitas * magnitudo(Entrium));
        redde ZEPHYRUM;
    }
    redde pos;
}
