/* probatio_hpack_corpus.c - hpack contra corpus hpack-test-case
 * (http2-plan T3): historiae VIII ex nghttp2, go-hpack,
 * node-http2-hpack et nghttp2-change-table-size per decodatorem
 * nostrum (tabula per numerati servata, header_table_size honoratus);
 * raw-data per codificatorem nostrum et decodatorem recentem iterum.
 * Data in probationes/fixa/hpack/corpus (ORIGO.txt: fons, commit,
 * licentia MIT). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "filum.h"
#include "hpack.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define CORPUS          "probationes/fixa/hpack/corpus/"
#define HISTORIAE       VIII
#define TABULA_ORDINARIA (MMXLVIII * II)

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

interior i32
nibble (
    i32 k)
{
    redde (k >= 'a') ? (i32)(k - 'a' + X) : (i32)(k - '0');
}

/* chorda hex -> octeti in piscina */
interior i8*
hex_ad_octetos (
    chorda  hex,
       i32* n)
{
     i8* b;
    i32  i;

    *n  = hex.mensura / II;
    b   = (i8*)piscina_allocare(piscina, (memoriae_index)(*n + I));
    per (i = ZEPHYRUM; b && i < *n; i++)
    {
        b[i] = (i8)(nibble(hex.datum[II * i]) * XVI
                    + nibble(hex.datum[II * i + I]));
    }
    redde b;
}

interior JsonValor*
historiam_legere (
    constans character* fons,
                   i32  k)
{
       character via[CCLVI];
          chorda textus;
    JsonResultus r;

    sprintf(via, CORPUS "%s/story_%02u.json", fons,
        (insignatus integer)k);
    textus = filum_legere_totum(via, piscina);
    si (textus.mensura == ZEPHYRUM)
    {
        imprimere("  deest: %s\n", via);
        redde NIHIL;
    }
    r = json_legere(textus, piscina);
    redde r.successus ? json_objectum_capere(r.radix, "cases") : NIHIL;
}

/* headers JSON ([{titulus: valor}]) -> HpackCaput[] in piscina */
interior HpackCaput*
capita_ex_json (
    JsonValor* headers,
          i32* numerus)
{
    HpackCaput* capita;
           i32  i;

    *numerus = headers ? json_tabulatum_numerus(headers) : ZEPHYRUM;
    capita   = (HpackCaput*)piscina_allocare(piscina,
        (memoriae_index)(*numerus + I) * magnitudo(HpackCaput));
    per (i = ZEPHYRUM; capita && i < *numerus; i++)
    {
        JsonPar* par;

        par =
            json_objectum_par_obtinere(json_tabulatum_obtinere(headers,
            i), ZEPHYRUM);
        si (!par)
        {
            redde NIHIL;
        }
        capita[i].titulus  = *par->clavis;
        capita[i].valor    = json_ad_chorda(par->valor);
        capita[i].numquam  = FALSUM;
    }
    redde capita;
}

interior b32
congruunt (
                    Xar* decodata,
    constans HpackCaput* exspectata,
                    i32  numerus)
{
    i32 i;

    si (xar_numerus(decodata) != numerus)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        HpackCaput* h = (HpackCaput*)xar_obtinere(decodata, i);

        si (   !chorda_aequalis(h->titulus, exspectata[i].titulus)
            || !chorda_aequalis(h->valor, exspectata[i].valor))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* historia tota per decodatorem unum; series discrepans nominatur */
interior b32
historiam_decodere (
    constans character* fons,
                   i32  k,
                  i32* casus_numerati)
{
      JsonValor* series;
    HpackTabula* d;
            i32  i;

    series = historiam_legere(fons, k);
    si (!series)
    {
        redde FALSUM;
    }
    d = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(series); i++)
    {
         JsonValor* c = json_tabulatum_obtinere(series, i);
        HpackCaput* exspectata;
               Xar* decodata;
                i8* filum;
               i32  n;
               i32  numerus;
               s64  mensura;
        HpackError  e;

        mensura = json_capere_integer(c, "header_table_size", -I);
        si (mensura >= ZEPHYRUM)
        {
            hpack_magnitudinem_ponere(d, (i32)mensura);
        }
        filum = hex_ad_octetos(json_capere_chorda(c, "wire",
            chorda_ex_literis("", piscina)), &n);
        exspectata = capita_ex_json(json_objectum_capere(c, "headers"),
            &numerus);
        decodata = xar_creare(piscina, (i32)magnitudo(HpackCaput));
        e = hpack_decodere(d, filum, n, ZEPHYRUM, piscina, decodata);
        si (   e != HPACK_BENE || !exspectata
            || !congruunt(decodata, exspectata, numerus))
        {
            imprimere("  %s story_%02u casus %u: error %u\n", fons,
                (insignatus integer)k, (insignatus integer)i,
                (insignatus integer)e);
            redde FALSUM;
        }
        (*casus_numerati)++;
    }
    redde VERUM;
}

interior vacuum
_sectio_decodere (
    CredoContextus* c)
{
    hic_manens constans character* FONTES[] = {
        "nghttp2", "go-hpack", "node-http2-hpack",
        "nghttp2-change-table-size", NIHIL
    };
    i32 f;
    i32 k;
    i32 numerati;

    (vacuum)c;
    per (f = ZEPHYRUM; FONTES[f]; f++)
    {
        CREDO_GRADUS(FONTES[f]);
        numerati = ZEPHYRUM;
        per (k = ZEPHYRUM; k < HISTORIAE; k++)
        {
            CREDO_VERUM(historiam_decodere(FONTES[f], k, &numerati));
        }
        imprimere("  %s: %u casus\n", FONTES[f],
            (insignatus integer)numerati);
        CREDO_VERUM(numerati > ZEPHYRUM);
    }
}

/* forma literalis sine indice, titulo literali (6.2.2): limes
 * longitudinis quam codificator noster numquam excedit */
interior i32
longitudo_literalis (
    constans HpackCaput* capita,
                    i32  numerus)
{
     i8 b[XVI];
    i32 summa;
    i32 i;

    summa = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        summa += I
            + hpack_integrum_scribere(capita[i].titulus.mensura, VII,
                  ZEPHYRUM, b, XVI)
            + capita[i].titulus.mensura
            + hpack_integrum_scribere(capita[i].valor.mensura, VII,
                  ZEPHYRUM, b, XVI)
            + capita[i].valor.mensura;
    }
    redde summa;
}

interior b32
historiam_iterare (
     i32  k,
    i32* casus_numerati)
{
      JsonValor* series;
    HpackTabula* codificator;
    HpackTabula* lector;
            i32  i;

    series = historiam_legere("raw-data", k);
    si (!series)
    {
        redde FALSUM;
    }
    codificator  = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    lector       = hpack_tabulam_creare(piscina, TABULA_ORDINARIA);
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(series); i++)
    {
        HpackCaput* capita;
               Xar* decodata;
                i8* exitus;
               i32  numerus;
               i32  limes;
               i32  n;

        capita = capita_ex_json(json_objectum_capere(
            json_tabulatum_obtinere(series, i), "headers"), &numerus);
        si (!capita)
        {
            redde FALSUM;
        }
        limes   = longitudo_literalis(capita, numerus);
        exitus  = (i8*)piscina_allocare(piscina,
            (memoriae_index)(limes + XVI));
        n = hpack_codificare(codificator, capita, numerus, exitus,
            limes + XVI);
        decodata = xar_creare(piscina, (i32)magnitudo(HpackCaput));
        si (   (numerus > ZEPHYRUM && n == ZEPHYRUM) || n > limes
            || hpack_decodere(lector, exitus, n, ZEPHYRUM, piscina,
                   decodata) != HPACK_BENE
            || !congruunt(decodata, capita, numerus))
        {
            imprimere("  raw-data story_%02u casus %u: n %u limes %u\n",
                (insignatus integer)k, (insignatus integer)i,
                (insignatus integer)n, (insignatus integer)limes);
            redde FALSUM;
        }
        (*casus_numerati)++;
    }
    redde VERUM;
}

interior vacuum
_sectio_iterum (
    CredoContextus* c)
{
    i32 k;
    i32 numerati;

    (vacuum)c;
    CREDO_GRADUS("raw-data: codificare, decodere, non longius literali");
    numerati = ZEPHYRUM;
    per (k = ZEPHYRUM; k < HISTORIAE; k++)
    {
        CREDO_VERUM(historiam_iterare(k, &numerati));
    }
    imprimere("  raw-data: %u casus\n", (insignatus integer)numerati);
    CREDO_VERUM(numerati > ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "decodere historias", _sectio_decodere, _parare, _purgare, NIHIL,
        NIHIL },
    { "iterum per codificatorem", _sectio_iterum, _parare, _purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("hpack_corpus", SECTIONES);
}
