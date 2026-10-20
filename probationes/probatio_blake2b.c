/* probatio_blake2b.c - BLAKE2b (hospitium v3 T5). Oraculum: textus
 * RFC 7693 IMMUTATUS (probationes/fixa/blake2/rfc7693.txt, sha256
 * infra fixum): Appendix A ("abc", BLAKE2b-512) et Appendix E (modulus
 * probationis sui RFC hic re-expressus: longitudines et digestum
 * magnum EX TEXTU leguntur, nulla hex manu copiata). Rivus = uno ictu
 * pro omni scissione. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "sigillum.h"
#include "blake2b.h"
#include <string.h>
#include <stdlib.h>

#define VIA_RFC "probationes/fixa/blake2/rfc7693.txt"
#define SIGILLUM_RFC \
    "c943754888364fe29bbd0bb3c71c6658ef392371497eaa4a9d9dde015b0721e5"

interior chorda
_textus (
    Piscina* p)
{
    redde filum_legere_totum(VIA_RFC, p);
}

/* positio post 'quaesitum' in textu, aut -I */
interior s32
_post (
                chorda  t,
    constans character* quaesitum)
{
    i32 n = (i32)strlen(quaesitum);
    i32 i;

    per (i = ZEPHYRUM; i + n <= t.mensura; i++)
    {
        si (memcmp(t.datum + i, quaesitum, (size_t)n) == ZEPHYRUM)
        {
            redde (s32)(i + n);
        }
    }
    redde -I;
}

interior s32
_hex (
    i8 c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    si (c >= 'A' && c <= 'F')
    {
        redde (s32)(c - 'A') + X;
    }
    redde -I;
}

/* n octeti hex ab a: signa "HH" (spatiis/lineis separata, praefixum
 * '0x' et ',' licent); FALSUM si ante n non inventi */
interior b32
_octeti (
     chorda  t,
        s32  a,
        i32  n,
         i8* exitus)
{
    i32 k = ZEPHYRUM;
    i32 i;

    si (a < ZEPHYRUM)
    {
        redde FALSUM;
    }
    i = (i32)a;
    dum (k < n && i + I < t.mensura)
    {
        si (t.datum[i] == '0' && (t.datum[i + I] == 'x'))
        {
            i += II;
            perge;
        }
        si (   _hex(t.datum[i]) >= ZEPHYRUM && _hex(t.datum[i + I])
            >= ZEPHYRUM)
        {
            exitus[k++] = (i8)(_hex(t.datum[i]) * XVI
                + _hex(t.datum[i + I]));
            i += II;
            perge;
        }
        si (   t.datum[i] == ' ' || t.datum[i] == '\n'
            || t.datum[i] == ',' || t.datum[i] == '\r')
        {
            i++;
            perge;
        }
        redde FALSUM;
    }
    redde k == n;
}

/* n numeri decimales ab a ("{ 20, 32, 48, 64 }") */
interior b32
_numeri (
     chorda  t,
        s32  a,
        i32  n,
        i32* exitus)
{
    i32 k = ZEPHYRUM;
    i32 i;

    si (a < ZEPHYRUM)
    {
        redde FALSUM;
    }
    i = (i32)a;
    dum (k < n && i < t.mensura && t.datum[i] != '}')
    {
        si (t.datum[i] >= '0' && t.datum[i] <= '9')
        {
            i32 v = ZEPHYRUM;

            dum (   i < t.mensura && t.datum[i] >= '0'
                 && t.datum[i] <= '9')
            {
                v = v * X + (i32)(t.datum[i] - '0');
                i++;
            }
            exitus[k++] = v;
            perge;
        }
        i++;
    }
    redde k == n;
}

interior vacuum
_sectio_fixum (CredoContextus* c)
{
       chorda t = _textus(c->piscina);
     Sigillum s;
    character hex[SIGILLUM_HEX_MENSURA];

    CREDO_NECESSE(t.mensura > ZEPHYRUM);
    s = sigillum_computare(t.datum, (memoriae_index)t.mensura);
    sigillum_hex(&s, hex);
    CREDO_VERUM(strcmp(hex, SIGILLUM_RFC) == ZEPHYRUM);
}

interior vacuum
_sectio_appendix_a (CredoContextus* c)
{
     chorda t = _textus(c->piscina);
         i8 expectatum[LXIV];
         i8 digestum[LXIV];
    Blake2b b;

    CREDO_NECESSE(_octeti(t, _post(t, "BLAKE2b-512(\"abc\") = "),
        LXIV, expectatum));
    blake2b(digestum, LXIV, (constans i8*)"abc", III, NIHIL, ZEPHYRUM);
    CREDO_VERUM(memcmp(digestum, expectatum, LXIV) == ZEPHYRUM);

    /* incrementaliter, octetim */
    CREDO_VERUM(blake2b_incipere(&b, LXIV, NIHIL, ZEPHYRUM));
    blake2b_addere(&b, (constans i8*)"a", I);
    blake2b_addere(&b, (constans i8*)"b", I);
    blake2b_addere(&b, (constans i8*)"c", I);
    blake2b_finire(&b, digestum);
    CREDO_VERUM(memcmp(digestum, expectatum, LXIV) == ZEPHYRUM);
}

/* RFC 7693 Appendix E: selftest_seq */
interior vacuum
_sequentia (
     i8* exitus,
    i32  n,
    i32  semen)
{
    i32 a = 0xDEAD4BADU * semen;
    i32 b = I;
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        i32 t = a + b;

        a          = b;
        b          = t;
        exitus[i]  = (i8)((t >> XXIV) & 0xFF);
    }
}

interior vacuum
_sectio_probatio_sui (CredoContextus* c)
{
     chorda t = _textus(c->piscina);
         i8 magnum[XXXII];
        i32 md_len[IV];
        i32 in_len[VI];
         i8 in[MXXIV];
         i8 md[LXIV];
         i8 clavis[LXIV];
    Blake2b ctx;
        i32 i;
        i32 j;

    CREDO_NECESSE(_octeti(t, _post(t,
        "const uint8_t blake2b_res[32] = {"), XXXII, magnum));
    CREDO_NECESSE(_numeri(t, _post(t, "b2b_md_len[4] = {"), IV,
        md_len));
    CREDO_NECESSE(_numeri(t, _post(t, "b2b_in_len[6] = {"), VI,
        in_len));
    CREDO_AEQUALIS_I32(in_len[V], MXXIV);

    CREDO_VERUM(blake2b_incipere(&ctx, XXXII, NIHIL, ZEPHYRUM));
    per (i = ZEPHYRUM; i < IV; i++)
    {
        per (j = ZEPHYRUM; j < VI; j++)
        {
            _sequentia(in, in_len[j], in_len[j]);
            blake2b(md, md_len[i], in, in_len[j], NIHIL, ZEPHYRUM);
            blake2b_addere(&ctx, md, md_len[i]);

            _sequentia(clavis, md_len[i], md_len[i]);
            blake2b(md, md_len[i], in, in_len[j], clavis, md_len[i]);
            blake2b_addere(&ctx, md, md_len[i]);
        }
    }
    blake2b_finire(&ctx, md);
    CREDO_VERUM(memcmp(md, magnum, XXXII) == ZEPHYRUM);
}

/* rivus = uno ictu, pro omni scissione 300 octetorum */
interior vacuum
_sectio_rivus (CredoContextus* c)
{
     i8 data[CCC];
     i8 clavis[XXXII];
     i8 unum[LXIV];
     i8 rivus[LXIV];
    i32 k;
    i32 malae = ZEPHYRUM;
    i32 exitus_n[III];
    i32 e;

    (vacuum)c;
    exitus_n[ZEPHYRUM]  = XX;
    exitus_n[I]         = XXXII;
    exitus_n[II]        = LXIV;
    _sequentia(data, CCC, VII);
    _sequentia(clavis, XXXII, XI);
    per (e = ZEPHYRUM; e < III; e++)
    {
        per (k = ZEPHYRUM; k <= CCC; k++)
        {
            Blake2b b;
                i32 cl = (k % II) ? XXXII : ZEPHYRUM;

            blake2b(unum, exitus_n[e], data, CCC,
                cl ? clavis : NIHIL, cl);
            (vacuum)blake2b_incipere(&b, exitus_n[e],
                cl ? clavis : NIHIL,
                cl);
            blake2b_addere(&b, data, k);
            blake2b_addere(&b, data + k, CCC - k);
            blake2b_finire(&b, rivus);
            si (memcmp(unum, rivus, (size_t)exitus_n[e]) != ZEPHYRUM)
            {
                malae++;
            }
        }
    }
    CREDO_AEQUALIS_I32(malae, ZEPHYRUM);
}

interior vacuum
_sectio_limites (CredoContextus* c)
{
    Blake2b b;
         i8 clavis[LXV];

    (vacuum)c;
    memset(clavis, ZEPHYRUM, magnitudo(clavis));
    CREDO_FALSUM(blake2b_incipere(&b, ZEPHYRUM, NIHIL, ZEPHYRUM));
    CREDO_FALSUM(blake2b_incipere(&b, LXV, NIHIL, ZEPHYRUM));
    CREDO_FALSUM(blake2b_incipere(&b, XXXII, clavis, LXV));
    CREDO_FALSUM(blake2b_incipere(&b, XXXII, NIHIL, I));
    CREDO_VERUM(blake2b_incipere(&b, I, clavis, LXIV));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fixum", _sectio_fixum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abc", _sectio_appendix_a, NIHIL, NIHIL, NIHIL, NIHIL },
    { "probatio_sui", _sectio_probatio_sui, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "rivus", _sectio_rivus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limites", _sectio_limites, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("blake2b", SECTIONES);
}
