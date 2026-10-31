/* probatio_hpack.c - hpack primitiva (http2-spec III.1, http2-plan T1):
 * integra cum praefixo N bitorum (RFC 7541 5.1, exempla C.1), chordae
 * Huffman (Appendix B, exempla C.4), tabula statica (Appendix A). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "hpack.h"
#include "xar.h"
#include "credo.h"
#include "fixa/hpack/rfc7541_exempla.h"
#include <stdio.h>
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

/* ===== T2: tabula dynamica, decodator, codificator ===== */

#define TABULA_ORDINARIA  (MMXLVIII * II)

interior constans ExemplumHpack*
exemplum (
    constans character* id)
{
    i32 i;

    per (i = ZEPHYRUM; i < (i32)EXEMPLA_RFC7541_NUMERUS; i++)
    {
        si (strcmp(EXEMPLA_RFC7541[i].id, id) == ZEPHYRUM)
        {
            redde &EXEMPLA_RFC7541[i];
        }
    }
    redde NIHIL;
}

interior HpackCaput*
caput_i (
    Xar* capita,
    i32  i)
{
    redde (HpackCaput*)xar_obtinere(capita, i);
}

interior b32
capita_congruunt (
                       Xar* capita,
    constans ExemplumHpack* e)
{
    i32 i;

    si (xar_numerus(capita) != e->numerus_capitum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < e->numerus_capitum; i++)
    {
        si (   !chorda_aequalis_literis(caput_i(capita, i)->titulus,
                   e->capita[i].titulus)
            || !chorda_aequalis_literis(caput_i(capita, i)->valor,
                   e->capita[i].valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
tabula_congruit (
                  HpackTabula* t,
       constans ExemplumHpack* e)
{
    chorda n;
    chorda v;
       i32 i;

    si (hpack_tabula_numerus(t) != e->numerus_entrium)
    {
        redde FALSUM;
    }
    si (   e->mensura_tabulae        >= ZEPHYRUM
        && hpack_tabula_magnitudo(t) != (i32)e->mensura_tabulae)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < e->numerus_entrium; i++)
    {
        si (   !hpack_tabula_obtinere(t, i + I, &n, &v)
            || !chorda_aequalis_literis(n, e->entria[i].titulus)
            || !chorda_aequalis_literis(v, e->entria[i].valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* exemplum per decodatorem d: capita et tabula RFC */
interior b32
decodere_congruit (
                  HpackTabula* d,
       constans ExemplumHpack* e)
{
     i8  b[CCLVI];
    i32  n;
    Xar* capita;

    n       = hex_legere(e->hex, b, CCLVI);
    capita  = xar_creare(piscina, (i32)magnitudo(HpackCaput));
    redde hpack_decodere(d, b, n, ZEPHYRUM, piscina, capita)
            == HPACK_BENE
        && capita_congruunt(capita, e)
        && tabula_congruit(d, e);
}

/* capita exempli per codificatorem c (numquam ex 'numquam'): octeti
 * RFC */
interior b32
codificare_congruit (
                  HpackTabula* c,
       constans ExemplumHpack* e,
                          b32  numquam)
{
    HpackCaput capita[VI];
            i8 b[CCLVI];
           i32 n;
           i32 i;

    per (i = ZEPHYRUM; i < e->numerus_capitum; i++)
    {
        capita[i].titulus  = chorda_ex_literis(e->capita[i].titulus,
            piscina);
        capita[i].valor    = chorda_ex_literis(e->capita[i].valor,
            piscina);
        capita[i].numquam  = numquam;
    }
    n = hpack_codificare(c, capita, e->numerus_capitum, b, CCLVI);
    redde n > ZEPHYRUM && aequales_hex(b, n, e->hex);
}

interior vacuum
_sectio_c2 (
    CredoContextus* c)
{
    HpackTabula* d;
    HpackTabula* k;
            Xar* capita;
             i8  b[LXIV];
            i32  n;

    (vacuum)c;
    CREDO_GRADUS("C.2.1 literale cum indice");
    d = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    CREDO_VERUM(decodere_congruit(d, exemplum("C.2.1")));
    k = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    hpack_huffman_ponere(k, HPACK_HUFFMAN_NUMQUAM);
    CREDO_VERUM(codificare_congruit(k, exemplum("C.2.1"), FALSUM));

    CREDO_GRADUS("C.2.2 sine indice: tabula immutata");
    d = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    CREDO_VERUM(decodere_congruit(d, exemplum("C.2.2")));
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(d), ZEPHYRUM);

    CREDO_GRADUS("C.2.3 numquam: vexillum servatur");
    d       = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    n       = hex_legere(exemplum("C.2.3")->hex, b, LXIV);
    capita  = xar_creare(piscina, (i32)magnitudo(HpackCaput));
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_BENE);
    CREDO_VERUM(capita_congruunt(capita, exemplum("C.2.3")));
    CREDO_VERUM(caput_i(capita, ZEPHYRUM)->numquam);
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(d), ZEPHYRUM);
    k = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    hpack_huffman_ponere(k, HPACK_HUFFMAN_NUMQUAM);
    CREDO_VERUM(codificare_congruit(k, exemplum("C.2.3"), VERUM));
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(k), ZEPHYRUM);

    CREDO_GRADUS("C.2.4 indicatum");
    d = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    CREDO_VERUM(decodere_congruit(d, exemplum("C.2.4")));
    k = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    CREDO_VERUM(codificare_congruit(k, exemplum("C.2.4"), FALSUM));
}

/* series trium (C.3-C.6): decodator et codificator suas tabulas per
 * exempla servant */
interior b32
seriem_probare (
    constans character* series,
                   i32  magnitudo_tabulae,
          HpackHuffman  modus)
{
    HpackTabula* d;
    HpackTabula* k;
      character  id[VIII];
            i32  i;

    d = hpack_tabulam_creare(piscina, magnitudo_tabulae);
    k = hpack_tabulam_creare(piscina, magnitudo_tabulae);
    hpack_huffman_ponere(k, modus);
    per (i = I; i <= III; i++)
    {
        sprintf(id, "%s.%u", series, (insignatus integer)i);
        si (   !decodere_congruit(d, exemplum(id))
            || !codificare_congruit(k, exemplum(id), FALSUM)
            || !tabula_congruit(k, exemplum(id)))
        {
            imprimere("  series %s: gradus %u discrepat\n", series,
                (insignatus integer)i);
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_series (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_GRADUS("C.3 petitiones sine Huffman");
    CREDO_VERUM(seriem_probare("C.3", TABULA_ORDINARIA,
        HPACK_HUFFMAN_NUMQUAM));
    CREDO_GRADUS("C.4 petitiones cum Huffman");
    CREDO_VERUM(seriem_probare("C.4", TABULA_ORDINARIA,
        HPACK_HUFFMAN_SEMPER));
    CREDO_GRADUS("C.5 responsa sine Huffman, tabula CCLVI, evictio");
    CREDO_VERUM(seriem_probare("C.5", CCLVI, HPACK_HUFFMAN_NUMQUAM));
    CREDO_GRADUS("C.6 responsa cum Huffman, tabula CCLVI, evictio");
    CREDO_VERUM(seriem_probare("C.6", CCLVI, HPACK_HUFFMAN_SEMPER));
}

interior HpackCaput
caput (
    constans character* titulus,
    constans character* valor)
{
    HpackCaput h;

    h.titulus  = chorda_ex_literis(titulus, piscina);
    h.valor    = chorda_ex_literis(valor, piscina);
    h.numquam  = FALSUM;
    redde h;
}

interior vacuum
_sectio_secreta (
    CredoContextus* c)
{
    HpackTabula* k;
    HpackTabula* d;
     HpackCaput  h[III];
            Xar* capita;
             i8  b[CCLVI];
            i32  n;

    (vacuum)c;
    CREDO_GRADUS("authorization et cookie brevis: numquam indicata");
    k            = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    d            = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    h[ZEPHYRUM]  = caput("authorization", "bearer x");
    n            = hpack_codificare(k, h, I, b, CCLVI);
    CREDO_VERUM(n > ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)(b[ZEPHYRUM] & 0xf0), 0x10);
    h[ZEPHYRUM]  = caput("cookie", "a=b");
    n            = hpack_codificare(k, h, I, b, CCLVI);
    CREDO_AEQUALIS_I32((i32)(b[ZEPHYRUM] & 0xf0), 0x10);
    h[ZEPHYRUM]  = caput("proxy-authorization", "basic y");
    n            = hpack_codificare(k, h, I, b, CCLVI);
    CREDO_AEQUALIS_I32((i32)(b[ZEPHYRUM] & 0xf0), 0x10);
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(k), ZEPHYRUM);
    capita = xar_creare(piscina, (i32)magnitudo(HpackCaput));
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_BENE);
    CREDO_VERUM(caput_i(capita, ZEPHYRUM)->numquam);

    CREDO_GRADUS("cookie longus et caput signatum");
    h[ZEPHYRUM]  = caput("cookie", "sessio=0123456789abcdef");
    n            = hpack_codificare(k, h, I, b, CCLVI);
    CREDO_AEQUALIS_I32((i32)(b[ZEPHYRUM] & 0xc0), 0x40);
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(k), I);
    h[ZEPHYRUM]          = caput("x-secretum", "z");
    h[ZEPHYRUM].numquam  = VERUM;
    n                    = hpack_codificare(k, h, I, b, CCLVI);
    CREDO_AEQUALIS_I32((i32)(b[ZEPHYRUM] & 0xf0), 0x10);
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(k), I);

    CREDO_GRADUS("capacitas deficit: tabula immutata");
    h[ZEPHYRUM]  = caput(":authority", "www.example.com");
    h[I]         = caput("custom-key", "custom-value");
    n            = hpack_codificare(k, h, II, b, V);
    CREDO_AEQUALIS_I32(n, ZEPHYRUM);
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(k), I);
}

interior vacuum
_sectio_magnitudo (
    CredoContextus* c)
{
    HpackTabula* k;
    HpackTabula* d;
     HpackCaput  h[I];
            Xar* capita;
             i8  b[LXIV];
            i32  n;

    (vacuum)c;
    CREDO_GRADUS("codificator: renovatio ad initium");
    k = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    hpack_magnitudinem_ponere(k, ZEPHYRUM);
    h[ZEPHYRUM]  = caput("custom-key", "custom-value");
    n            = hpack_codificare(k, h, I, b, LXIV);
    CREDO_VERUM(n > I);
    CREDO_AEQUALIS_I32((i32)b[ZEPHYRUM], 0x20);
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(k), ZEPHYRUM);
    n = hpack_codificare(k, h, I, b, LXIV);
    CREDO_VERUM((b[ZEPHYRUM] & 0xe0) != 0x20);

    CREDO_GRADUS("decodator: supra limitem, post caput, evictio");
    d       = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    capita  = xar_creare(piscina, (i32)magnitudo(HpackCaput));
    n       = hex_legere("3f e1 3f", b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_ERROR_MAGNITUDO);
    d = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    n = hex_legere("82 20", b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_ERROR_MAGNITUDO);
    d = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    CREDO_VERUM(decodere_congruit(d, exemplum("C.2.1")));
    n = hex_legere("20 82", b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_BENE);
    CREDO_AEQUALIS_I32(hpack_tabula_numerus(d), ZEPHYRUM);
}

interior vacuum
_sectio_bombae (
    CredoContextus* c)
{
    HpackTabula* d;
            Xar* capita;
             i8  b[LXIV];
            i32  n;

    (vacuum)c;
    CREDO_GRADUS("index 0 et extra tabulam");
    capita  = xar_creare(piscina, (i32)magnitudo(HpackCaput));
    d       = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    n       = hex_legere("80", b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_ERROR_INDEX);
    n = hex_legere("ff 10", b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_ERROR_INDEX);

    CREDO_GRADUS("truncatum, integrum nimium, Huffman malum");
    n = hex_legere("40 0a 63 75 73", b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_ERROR_TRUNCATUM);
    n = hex_legere("ff ff ff ff ff 0f", b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_ERROR_INTEGRUM);
    n = hex_legere("40 81 06 01 61", b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, ZEPHYRUM, piscina,
        capita), (i32)HPACK_ERROR_HUFFMAN);

    CREDO_GRADUS("index capitum supra limitem (Review Focus 5)");
    d = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    n = hex_legere(exemplum("C.3.1")->hex, b, LXIV);
    CREDO_AEQUALIS_I32((i32)hpack_decodere(d, b, n, LXIV, piscina,
        capita), (i32)HPACK_ERROR_NIMIA);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "integra (C.1)", _sectio_integra, _parare, _purgare, NIHIL,
        NIHIL },
    { "huffman (Appendix B, C.4)", _sectio_huffman, _parare, _purgare,
        NIHIL, NIHIL },
    { "tabula statica (Appendix A)", _sectio_staticum, _parare,
        _purgare, NIHIL, NIHIL },
    { "C.2 repraesentationes", _sectio_c2, _parare, _purgare, NIHIL,
        NIHIL },
    { "C.3-C.6 series", _sectio_series, _parare, _purgare, NIHIL,
        NIHIL },
    { "secreta numquam indicata", _sectio_secreta, _parare, _purgare,
        NIHIL, NIHIL },
    { "magnitudo tabulae", _sectio_magnitudo, _parare, _purgare, NIHIL,
        NIHIL },
    { "bombae et errores", _sectio_bombae, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("hpack", SECTIONES);
}
