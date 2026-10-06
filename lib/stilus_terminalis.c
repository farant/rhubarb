/* stilus_terminalis.c - Vide stilus_terminalis.h */

#include "stilus_terminalis.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
_colores_aequales (
    StilusColor a,
    StilusColor b)
{
    si (a.genus != b.genus)
    {
        redde FALSUM;
    }
    /* NATIVUS: valor nihil significat */
    redde (b32)(a.genus == STILUS_COLOR_NATIVUS || a.valor == b.valor);
}

/* Gregem parametrorum scribere (gregis parametra numquam dividuntur).
 * Plus quam SERIES_PARAMETRA_MAXIMA (XXIV, ut Ghostty) in una serie =
 * series tota a terminali abicitur: tunc 'm' et series nova (sine
 * 0). */
interior vacuum
_gregem (
      ChordaAedificator* a,
                    i32* numerus,
     constans character* textus,
                    i32  parametra)
{
    si (*numerus + parametra > STILUS_PARAMETRA_MAXIMA)
    {
        chorda_aedificator_appendere_literis(a, "m\033[");
        *numerus = ZEPHYRUM;
    }
    si (*numerus > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, ';');
    }
    chorda_aedificator_appendere_literis(a, textus);
    *numerus += parametra;
}

/* Color SGR: praefixum (38, 48, 58) + "5;n" aut "2;r;g;b" ut grex;
 * NATIVUS nihil (reditio nuda defaltam dat) */
interior vacuum
_colorem (
      ChordaAedificator* a,
                    i32* numerus,
                    i32  praefixum,
            StilusColor  c,
                    i32  codificatio)
{
    character textus[XXXII];

    si (c.genus == STILUS_COLOR_NATIVUS)
    {
        redde;
    }
    si (   c.genus     == STILUS_COLOR_TABULA
        || codificatio == STILUS_CODIFICATIO_CCLVI)
    {
        i32 index = (c.genus == STILUS_COLOR_TABULA)
            ? (c.valor & 0xFF) : stilus_quantizare(c.valor);

        sprintf(textus, "%u;5;%u", (unsigned)praefixum,
            (unsigned)index);
        _gregem(a, numerus, textus, III);
        redde;
    }
    sprintf(textus, "%u;2;%u;%u;%u", (unsigned)praefixum,
        (unsigned)((c.valor >> XVI) & 0xFF),
        (unsigned)((c.valor >> VIII) & 0xFF),
        (unsigned)(c.valor & 0xFF));
    _gregem(a, numerus, textus, V);
}


/* ==================================================
 * Publica
 * ================================================== */

vacuum
stilus_nativus (
    StilusTerminalis* st)
{
    memset(st, ZEPHYRUM, magnitudo(StilusTerminalis));
}

b32
stilus_aequalis (
    constans StilusTerminalis* a,
    constans StilusTerminalis* b)
{
    redde (b32)(   a->ornamenta == b->ornamenta
                && a->sublinea == b->sublinea
                && _colores_aequales(a->color_litterae,
                b->color_litterae)
                && _colores_aequales(a->color_fundi, b->color_fundi)
                && _colores_aequales(a->color_sublineae,
                       b->color_sublineae));
}

vacuum
stilus_codificare (
            ChordaAedificator* a,
    constans StilusTerminalis* prior,
    constans StilusTerminalis* st,
                          i32  codificatio)
{
    i32 o        = st->ornamenta;
    i32 numerus  = ZEPHYRUM;

    si (prior && stilus_aequalis(prior, st))
    {
        redde;      /* v1: differentia nulla = nihil; aliter plena */
    }
    chorda_aedificator_appendere_literis(a, "\033[");
    _gregem(a, &numerus, "0", I);
    si (o & STILUS_CRASSUM)
    { _gregem(a, &numerus, "1", I);
    }
    si (o & STILUS_OBSCURUM)
    { _gregem(a, &numerus, "2", I);
    }
    si (o & STILUS_CURSIVUM)
    { _gregem(a, &numerus, "3", I);
    }
    si (st->sublinea == STILUS_SUBLINEA_SIMPLEX)
    {
        _gregem(a, &numerus, "4", I);
    }
    alioquin si (st->sublinea != STILUS_SUBLINEA_NULLA)
    {
        character textus[VIII];

        sprintf(textus, "4:%u", (unsigned)st->sublinea);
        /* sub-parametrum ut parametrum numeratur */
        _gregem(a, &numerus, textus, II);
    }
    si (o & STILUS_NICTANS)
    { _gregem(a, &numerus, "5", I);
    }
    si (o & STILUS_INVERSUM)
    { _gregem(a, &numerus, "7", I);
    }
    si (o & STILUS_INVISIBILE)
    { _gregem(a, &numerus, "8", I);
    }
    si (o & STILUS_TRANSFIXUM)
    { _gregem(a, &numerus, "9", I);
    }
    si (o & STILUS_SUPERLINEA)
    { _gregem(a, &numerus, "53", I);
    }
    _colorem(a, &numerus, XXXVIII, st->color_litterae, codificatio);
    _colorem(a, &numerus, XLVIII, st->color_fundi, codificatio);
    _colorem(a, &numerus, LVIII, st->color_sublineae, codificatio);
    chorda_aedificator_appendere_character(a, 'm');
}

i32
stilus_tabulae_color (
    s32 index)
{
    /* XVI PRIMAE: valores xterm defalti */
    hic_manens constans i32 PRIMAE[XVI] = {
        0x000000, 0xCD0000, 0x00CD00, 0xCDCD00,
        0x0000EE, 0xCD00CD, 0x00CDCD, 0xE5E5E5,
        0x7F7F7F, 0xFF0000, 0x00FF00, 0xFFFF00,
        0x5C5CFF, 0xFF00FF, 0x00FFFF, 0xFFFFFF
    };
    hic_manens constans i32 GRADUS[VI] = { 0, 95, 135, 175, 215, 255 };

    si (index < ZEPHYRUM || index > CCLV)
    {
        redde ZEPHYRUM;
    }
    si (index < XVI)
    {
        redde PRIMAE[index];
    }
    si (index < CCXXXII)
    {
        s32 k = index - XVI;

        redde (GRADUS[k / XXXVI] << XVI) | (GRADUS[(k / VI) % VI]
            << VIII)
            | GRADUS[k % VI];
    }
    {
        i32 v = (i32)(VIII + X * (index - CCXXXII));

        redde (v << XVI) | (v << VIII) | v;
    }
}

i32
stilus_quantizare (
    i32 rgb)
{
    /* tessera _cclvi (quadrans Q4) translatus: cubus (in aequalitate
     * gradus inferior) aut griseus (8 + 10k, proximus mediae RGB);
     * aequalitas distantiarum: cubus */
    hic_manens constans i32 GRADUS[VI] = { 0, 95, 135, 175, 215, 255 };
                        i32 index[III];
                        i32 d_cubi    = ZEPHYRUM;
                        i32 d_grisei  = ZEPHYRUM;
                        i32 summa     = ZEPHYRUM;
                        i32 griseus;
                        i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 c = (s32)((rgb >> (XVI - VIII * k)) & 0xFF);
        s32 d;
        i32 j;

        index[k] = ZEPHYRUM;
        per (j = I; j < VI; j++)
        {
            s32 prior  = c - (s32)GRADUS[index[k]];
            s32 hic    = c - (s32)GRADUS[j];

            si (hic * hic < prior * prior)
            {
                index[k] = j;
            }
        }
        d       = c - (s32)GRADUS[index[k]];
        d_cubi  += (i32)(d * d);
        summa   += (i32)c;
    }
    griseus = (summa / III < VIII) ? ZEPHYRUM
                                   : (summa / III - VIII + V) / X;
    si (griseus > XXIII)
    {
        griseus = XXIII;
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 d = (s32)((rgb >> (XVI - VIII * k)) & 0xFF)
            - (s32)(VIII + X * griseus);

        d_grisei += (i32)(d * d);
    }
    si (d_grisei < d_cubi)
    {
        redde CCXXXII + griseus;
    }
    redde XVI + XXXVI * index[0] + VI * index[1] + index[2];
}


/* ==================================================
 * Decodificatio (T2): Ghostty sgr.zig Parser.next translatus, attributa
 * in stilum applicata
 * ================================================== */

/* ':' POST parametrum i? (series_terminalis: bitus i) */
interior b32
_colon (
    constans SeriesLexema* l,
                      i32  i)
{
    redde (b32)(   i < (i32)(magnitudo(l->separatores) * VIII)
                && (l->separatores & ((i32)I << i)) != ZEPHYRUM);
}

/* ':' consecutivi ab i (ultimum parametrum separatorem non habet) */
interior i32
_colon_numerare (
    constans SeriesLexema* l,
                      i32  i)
{
    i32 n = ZEPHYRUM;

    dum (i < l->numerus_parametrorum - I && _colon(l, i))
    {
        n++;
        i++;
    }
    redde n;
}

interior StilusColor
_color_ponere (
    StilusColorGenus genus,
                 i32 valor)
{
    StilusColor c;

    c.genus = genus;
    c.valor = valor;
    redde c;
}

/* 38/48/58 (lexema[i] = 38 aut 48 aut 58). Redde parametros consumptos;
 * 0 = forma mala (vocans ignotum numerat; truncata reliqua consumit) */
interior i32
_colorem_legere (
    constans SeriesLexema* l,
                      i32  i,
                      b32  colon,
              StilusColor* c)
{
     constans s32* p        = l->parametra + i;
              i32  reliqua  = l->numerus_parametrorum - i;

    si (reliqua < II)
    {
        redde ZEPHYRUM;
    }
    si (p[1] == V)
    {
        si (reliqua < III)
        {
            redde ZEPHYRUM;
        }
        *c = _color_ponere(STILUS_COLOR_TABULA, (i32)p[2] & 0xFF);
        redde III;
    }
    si (p[1] != II || reliqua < V)
    {
        redde ZEPHYRUM;
    }
    si (!colon)
    {
        *c = _color_ponere(STILUS_COLOR_RGB, (((i32)p[2] & 0xFF) << XVI)
            | (((i32)p[3] & 0xFF) << VIII) | ((i32)p[4] & 0xFF));
        redde V;
    }
    /* ':' : 3 colones = r:g:b; 4 = spatium:r:g:b; aliter mala */
    commutatio (_colon_numerare(l, i + I))
    {
        casus III:
            *c = _color_ponere(STILUS_COLOR_RGB, (((i32)p[2] & 0xFF)
                << XVI)
                | (((i32)p[3] & 0xFF) << VIII) | ((i32)p[4] & 0xFF));
            redde V;
        casus IV:
            si (reliqua < VI)
            {
                redde ZEPHYRUM;
            }
            *c = _color_ponere(STILUS_COLOR_RGB, (((i32)p[3] & 0xFF)
                << XVI)
                | (((i32)p[4] & 0xFF) << VIII) | ((i32)p[5] & 0xFF));
            redde VI;
        ordinarius:
            redde ZEPHYRUM;
    }
}

/* Ornamenta per numerum SGR: bits ponendi et tollendi (22 crassum ET
 * obscurum tollit; 6 = 5, nictans). VERUM si numerus ornamenti. */
interior b32
_ornamentum (
                 s32  numerus,
    StilusTerminalis* st)
{
    hic_manens constans structura {
        s32 numerus;
        i32 ponere;
        i32 tollere;
    } TABULA[] = {
        { I,      STILUS_CRASSUM,    ZEPHYRUM },
        { II,     STILUS_OBSCURUM,   ZEPHYRUM },
        { III,    STILUS_CURSIVUM,   ZEPHYRUM },
        { V,      STILUS_NICTANS,    ZEPHYRUM },
        { VI,     STILUS_NICTANS,    ZEPHYRUM },
        { VII,    STILUS_INVERSUM,   ZEPHYRUM },
        { VIII,   STILUS_INVISIBILE, ZEPHYRUM },
        { IX,     STILUS_TRANSFIXUM, ZEPHYRUM },
        { XXII,   ZEPHYRUM, STILUS_CRASSUM | STILUS_OBSCURUM },
        { XXIII,  ZEPHYRUM, STILUS_CURSIVUM },
        { XXV,    ZEPHYRUM, STILUS_NICTANS },
        { XXVII,  ZEPHYRUM, STILUS_INVERSUM },
        { XXVIII, ZEPHYRUM, STILUS_INVISIBILE },
        { XXIX,   ZEPHYRUM, STILUS_TRANSFIXUM },
        { LIII,   STILUS_SUPERLINEA, ZEPHYRUM },
        { LV,     ZEPHYRUM, STILUS_SUPERLINEA }
    };
    i32 k;

    per (k = ZEPHYRUM; k < (i32)(magnitudo(TABULA)
        / magnitudo(TABULA[0]));
         k++)
    {
        si (TABULA[k].numerus == numerus)
        {
            st->ornamenta = (st->ornamenta | TABULA[k].ponere)
                & ~TABULA[k].tollere;
            redde VERUM;
        }
    }
    redde FALSUM;
}

i32
stilus_applicare (
    constans SeriesLexema* l,
         StilusTerminalis* st)
{
    i32 i       = ZEPHYRUM;
    i32 ignota  = ZEPHYRUM;

    /* SGR solum: CSI 'm' sine privato aut intermediis ('>4;2m' =
     * modifyOtherKeys xterm, non stilus) */
    si (   l->genus    != SERIES_CSI || l->finale != 'm'
        || l->privatum != 0
        || l->numerus_intermediorum > ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (l->numerus_parametrorum == ZEPHYRUM)
    {
        stilus_nativus(st);
        redde ZEPHYRUM;
    }
    dum (i < l->numerus_parametrorum)
    {
                 s32 p      = l->parametra[i];
                 b32 colon  = _colon(l, i);
                 i32 n;
         StilusColor c;

        /* ':' solum post 4, 38, 48, 58: aliter cursus ignotus */
        si (   colon && p != IV && p != XXXVIII && p != XLVIII
            && p != LVIII)
        {
            i += _colon_numerare(l, i) + I;
            ignota++;
            perge;
        }
        si (_ornamentum(p, st))
        {
            i++;
            perge;
        }
        commutatio (p)
        {
            casus ZEPHYRUM: stilus_nativus(st);                 frange;
            casus XXI: st->sublinea = STILUS_SUBLINEA_DUPLEX;    frange;
            casus XXIV: st->sublinea = STILUS_SUBLINEA_NULLA;    frange;
            casus XXXIX:
                st->color_litterae = _color_ponere(STILUS_COLOR_NATIVUS,
                    ZEPHYRUM);
                frange;
            casus XLIX:
                st->color_fundi = _color_ponere(STILUS_COLOR_NATIVUS,
                    ZEPHYRUM);
                frange;
            casus LIX:
                st->color_sublineae =
                    _color_ponere(STILUS_COLOR_NATIVUS,
                    ZEPHYRUM);
                frange;
            casus IV:
                si (!colon)
                {
                    st->sublinea = STILUS_SUBLINEA_SIMPLEX;
                    frange;
                }
                /* 4:n - unus sub-parametrus; plures = ignotum */
                si (   i + I >= l->numerus_parametrorum
                    || _colon(l, i + I))
                {
                    n = _colon_numerare(l, i);
                    i += n;      /* +I infra */
                    ignota++;
                    frange;
                }
                i++;
                st->sublinea = (l->parametra[i] >= ZEPHYRUM
                                && l->parametra[i] <= V)
                    ? (StilusSublinea)l->parametra[i]
                    : STILUS_SUBLINEA_SIMPLEX;
                frange;
            casus XXXVIII:
            casus XLVIII:
            casus LVIII:
                n = _colorem_legere(l, i, colon, &c);
                si (n == ZEPHYRUM)
                {
                    /* forma mala: ':' cursus aut reliqua consumuntur
                     * (Ghostty '5' reliquum ut nictans legeret) */
                    i = colon ? i + _colon_numerare(l,
                        i) : l->numerus_parametrorum;
                    ignota++;
                    frange;
                }
                si (p == XXXVIII)
                {
                    st->color_litterae = c;
                }
                alioquin si (p == XLVIII)
                {
                    st->color_fundi = c;
                }
                alioquin
                {
                    st->color_sublineae = c;
                }
                i += n - I;
                frange;
            ordinarius:
                si (p >= XXX && p <= XXXVII)
                {
                    st->color_litterae =
                        _color_ponere(STILUS_COLOR_TABULA,
                        (i32)(p - XXX));
                }
                alioquin si (p >= XL && p <= XLVII)
                {
                    st->color_fundi = _color_ponere(STILUS_COLOR_TABULA,
                        (i32)(p - XL));
                }
                alioquin si (p >= XC && p <= XCVII)
                {
                    st->color_litterae =
                        _color_ponere(STILUS_COLOR_TABULA,
                        (i32)(p - XC) + VIII);
                }
                alioquin si (p >= C && p <= CVII)
                {
                    st->color_fundi = _color_ponere(STILUS_COLOR_TABULA,
                        (i32)(p - C) + VIII);
                }
                alioquin
                {
                    ignota++;
                }
                frange;
        }
        i++;
    }
    redde ignota;
}
