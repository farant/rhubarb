/* probatio_hpack.c - hpack primitiva (http2-spec III.1, http2-plan T1):
 * integra cum praefixo N bitorum (RFC 7541 5.1, exempla C.1), chordae
 * Huffman (Appendix B, exempla C.4), tabula statica (Appendix A). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "hpack.h"
#include "credo.h"
#include <string.h>

#define AMPLUM (CCLVI * XVI)

hic_manens Piscina* piscina;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

/* "1f 9a 0a" -> octeti; reddit numerum */
interior i32
hex_legere (
    constans character* hex,
                    i8* exitus,
                   i32  capacitas)
{
    i32 n;
    i32 alt;
    i32 v;

    n    = ZEPHYRUM;
    alt  = ZEPHYRUM;
    v    = ZEPHYRUM;
    dum (*hex != '\0' && n < capacitas)
    {
        character k = *hex++;

        si (k == ' ')
        {
            perge;
        }
        v = v * XVI + (i32)((k >= 'a') ? (k - 'a' + X) : (k - '0'));
        si (++alt == II)
        {
            exitus[n++]  = (i8)v;
            alt          = ZEPHYRUM;
            v            = ZEPHYRUM;
        }
    }
    redde n;
}

interior b32
aequales_hex (
           constans i8* a,
                   i32  n,
    constans character* hex)
{
     i8 b[CXXVIII];
    i32 m;

    m = hex_legere(hex, b, CXXVIII);
    redde m == n && memcmp(a, b, (size_t)n) == ZEPHYRUM;
}

interior vacuum
_sectio_integra (
    CredoContextus* c)
{
     i8 b[XVI];
    i32 n;
    i32 v;
    i32 k;
    i32 p;
    i32 i;

    (vacuum)c;
    CREDO_GRADUS("C.1: 10/5, 1337/5, 42/8");
    n = hpack_integrum_scribere(X, V, ZEPHYRUM, b, XVI);
    CREDO_VERUM(aequales_hex(b, n, "0a"));
    n = hpack_integrum_scribere(MCCCXXXVII, V, ZEPHYRUM, b, XVI);
    CREDO_VERUM(aequales_hex(b, n, "1f 9a 0a"));
    n = hpack_integrum_scribere(XLII, VIII, ZEPHYRUM, b, XVI);
    CREDO_VERUM(aequales_hex(b, n, "2a"));
    CREDO_VERUM(hpack_integrum_legere(b, n, VIII, &v, &k));
    CREDO_AEQUALIS_I32(v, XLII);
    CREDO_AEQUALIS_I32(k, I);

    CREDO_GRADUS("primus: bita alta servantur");
    n = hpack_integrum_scribere(X, V, 0x80, b, XVI);
    CREDO_VERUM(aequales_hex(b, n, "8a"));
    CREDO_VERUM(hpack_integrum_legere(b, n, V, &v, &k));
    CREDO_AEQUALIS_I32(v, X);

    CREDO_GRADUS("rotundum: omnis praefixus, limites");
    per (p = I; p <= VIII; p++)
    {
        i32 valores[V];

        valores[ZEPHYRUM]  = ZEPHYRUM;
        valores[I]         = ((i32)I << p) - II;
        valores[II]        = ((i32)I << p) - I;
        valores[III]       = (i32)I << p;
        valores[IV]        = 0x7fffffff;
        per (i = ZEPHYRUM; i < V; i++)
        {
            n = hpack_integrum_scribere(valores[i], p, ZEPHYRUM, b,
                XVI);
            CREDO_VERUM(n > ZEPHYRUM);
            CREDO_VERUM(hpack_integrum_legere(b, n, p, &v, &k));
            CREDO_AEQUALIS_I32(v, valores[i]);
            CREDO_AEQUALIS_I32(k, n);
        }
    }

    CREDO_GRADUS("truncatum, nimium, capacitas");
    n = hex_legere("1f 9a", b, XVI);
    CREDO_FALSUM(hpack_integrum_legere(b, n, V, &v, &k));
    CREDO_FALSUM(hpack_integrum_legere(b, ZEPHYRUM, V, &v, &k));
    n = hex_legere("1f ff ff ff ff 0f", b, XVI);
    CREDO_FALSUM(hpack_integrum_legere(b, n, V, &v, &k));
    CREDO_AEQUALIS_I32(hpack_integrum_scribere(MCCCXXXVII, V, ZEPHYRUM,
        b, II), ZEPHYRUM);
}

interior b32
huffman_aequalis (
    constans character* textus,
    constans character* hex)
{
        i8 b[CXXVIII];
       i32 n;
    chorda s;
    chorda r;

    s = chorda_ex_literis(textus, piscina);
    n = hpack_huffman_scribere(s, b, CXXVIII);
    redde n == hpack_huffman_longitudo(s)
        && aequales_hex(b, n, hex)
        && hpack_huffman_legere(b, n, piscina, &r)
        && chorda_aequalis(r, s);
}

interior vacuum
_sectio_huffman (
    CredoContextus* c)
{
        i8 b[AMPLUM];
        i8 omnes[CCLVI];
       i32 n;
       i32 i;
    chorda s;
    chorda r;

    (vacuum)c;
    CREDO_GRADUS("C.4: www.example.com, no-cache, custom-key/value");
    CREDO_VERUM(huffman_aequalis("www.example.com",
        "f1e3 c2e5 f23a 6ba0 ab90 f4ff"));
    CREDO_VERUM(huffman_aequalis("no-cache", "a8eb 1064 9cbf"));
    CREDO_VERUM(huffman_aequalis("custom-key", "25a8 49e9 5ba9 7d7f"));
    CREDO_VERUM(huffman_aequalis("custom-value",
        "25a8 49e9 5bb8 e8b4 bf"));

    CREDO_GRADUS("omnes CCLVI octeti: singuli et coniuncti");
    per (i = ZEPHYRUM; i < CCLVI; i++)
    {
        omnes[i]   = (i8)i;
        s.datum    = omnes + i;
        s.mensura  = I;
        n          = hpack_huffman_scribere(s, b, AMPLUM);
        CREDO_VERUM(n > ZEPHYRUM);
        CREDO_VERUM(hpack_huffman_legere(b, n, piscina, &r));
        CREDO_VERUM(chorda_aequalis(r, s));
    }
    s.datum    = omnes;
    s.mensura  = CCLVI;
    n          = hpack_huffman_scribere(s, b, AMPLUM);
    CREDO_AEQUALIS_I32(n, hpack_huffman_longitudo(s));
    CREDO_VERUM(hpack_huffman_legere(b, n, piscina, &r));
    CREDO_VERUM(chorda_aequalis(r, s));

    CREDO_GRADUS("vacua; impletio et EOS mala");
    CREDO_VERUM(hpack_huffman_legere(b, ZEPHYRUM, piscina, &r));
    CREDO_AEQUALIS_I32(r.mensura, ZEPHYRUM);
    /* '0' = 00000 + 111 impletio = 07 */
    n = hex_legere("07", b, XVI);
    CREDO_VERUM(hpack_huffman_legere(b, n, piscina, &r));
    CREDO_CHORDA_AEQUALIS_LITERIS(r, "0");
    n = hex_legere("06", b, XVI);
    CREDO_FALSUM(hpack_huffman_legere(b, n, piscina, &r));
    n = hex_legere("ff", b, XVI);
    CREDO_FALSUM(hpack_huffman_legere(b, n, piscina, &r));
    n = hex_legere("ff ff ff ff", b, XVI);
    CREDO_FALSUM(hpack_huffman_legere(b, n, piscina, &r));
    CREDO_AEQUALIS_I32(hpack_huffman_scribere(chorda_ex_literis(
        "www.example.com", piscina), b, IV), ZEPHYRUM);
}

interior vacuum
_sectio_staticum (
    CredoContextus* c)
{
    chorda t;
    chorda v;

    (vacuum)c;
    CREDO_GRADUS("I, II, LXI; 0 et LXII nulla");
    CREDO_VERUM(hpack_staticum(I, &t, &v));
    CREDO_CHORDA_AEQUALIS_LITERIS(t, ":authority");
    CREDO_AEQUALIS_I32(v.mensura, ZEPHYRUM);
    CREDO_VERUM(hpack_staticum(II, &t, &v));
    CREDO_CHORDA_AEQUALIS_LITERIS(t, ":method");
    CREDO_CHORDA_AEQUALIS_LITERIS(v, "GET");
    CREDO_VERUM(hpack_staticum(VIII, &t, &v));
    CREDO_CHORDA_AEQUALIS_LITERIS(t, ":status");
    CREDO_CHORDA_AEQUALIS_LITERIS(v, "200");
    CREDO_VERUM(hpack_staticum(LXI, &t, &v));
    CREDO_CHORDA_AEQUALIS_LITERIS(t, "www-authenticate");
    CREDO_AEQUALIS_I32(v.mensura, ZEPHYRUM);
    CREDO_FALSUM(hpack_staticum(ZEPHYRUM, &t, &v));
    CREDO_FALSUM(hpack_staticum(LXII, &t, &v));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "integra (C.1)", _sectio_integra, _parare, _purgare, NIHIL,
        NIHIL },
    { "huffman (Appendix B, C.4)", _sectio_huffman, _parare, _purgare,
        NIHIL, NIHIL },
    { "tabula statica (Appendix A)", _sectio_staticum, _parare,
        _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("hpack", SECTIONES);
}
