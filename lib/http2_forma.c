/* http2_forma.c - formae HTTP/2 (RFC 9113 4, 6). Vide
 * include/http2_forma.h. */
#include "latina.h"
#include "http2_forma.h"
#include <string.h>

#define LONGITUDO_PULSUS      VIII
#define LONGITUDO_FENESTRAE   IV
#define LONGITUDO_ABROGATIONIS IV
#define LONGITUDO_PRIORITATIS V
#define LONGITUDO_POSITIONIS  VI
#define LONGITUDO_DISCESSUS   VIII

i32
http2_u32_legere (
    constans i8* datum)
{
    redde ((i32)datum[ZEPHYRUM] << XXIV) | ((i32)datum[I] << XVI)
        | ((i32)datum[II] << VIII) | (i32)datum[III];
}

vacuum
http2_u32_scribere (
    i32  valor,
     i8* exitus)
{
    exitus[ZEPHYRUM]  = (i8)((valor >> XXIV) & 0xff);
    exitus[I]         = (i8)((valor >> XVI) & 0xff);
    exitus[II]        = (i8)((valor >> VIII) & 0xff);
    exitus[III]       = (i8)(valor & 0xff);
}

i32
http2_positionem_scribere (
    i32  id,
    i32  valor,
     i8* exitus)
{
    exitus[ZEPHYRUM]  = (i8)((id >> VIII) & 0xff);
    exitus[I]         = (i8)(id & 0xff);
    http2_u32_scribere(valor, exitus + II);
    redde LONGITUDO_POSITIONIS;
}

b32
http2_positionem_legere (
    constans Http2Forma* forma,
                    i32  k,
                    i32* id,
                    i32* valor)
{
    constans i8* p;

    si (   !forma || !id || !valor || forma->genus != HTTP2_POSITIONES
        || (k + I) * LONGITUDO_POSITIONIS > forma->contentum_n)
    {
        redde FALSUM;
    }
    p       = forma->contentum + k * LONGITUDO_POSITIONIS;
    *id     = ((i32)p[ZEPHYRUM] << VIII) | (i32)p[I];
    *valor  = http2_u32_legere(p + II);
    redde VERUM;
}

i32
http2_formam_scribere (
             i32  genus,
             i32  vexilla,
             i32  rivus,
     constans i8* contentum,
             i32  n,
              i8* exitus,
             i32  capacitas)
{
    si (   !exitus || n > HTTP2_FORMA_SUPREMA
        || capacitas < HTTP2_CAPUT_FORMAE + n
        || (n > ZEPHYRUM && !contentum))
    {
        redde ZEPHYRUM;
    }
    exitus[ZEPHYRUM]  = (i8)((n >> XVI) & 0xff);
    exitus[I]         = (i8)((n >> VIII) & 0xff);
    exitus[II]        = (i8)(n & 0xff);
    exitus[III]       = (i8)genus;
    exitus[IV]        = (i8)vexilla;
    http2_u32_scribere(rivus & HTTP2_FENESTRA_SUPREMA, exitus + V);
    si (n > ZEPHYRUM)
    {
        memcpy(exitus + HTTP2_CAPUT_FORMAE, contentum, (size_t)n);
    }
    redde HTTP2_CAPUT_FORMAE + n;
}

interior vacuum
_malum (
    Http2Iudicium* j,
              i32  codex,
              b32  connexionis)
{
    j->bona         = FALSUM;
    j->codex        = codex;
    j->connexionis  = connexionis;
}

/* 6.5.2: valores positionum */
interior vacuum
_positiones_iudicare (
    constans Http2Forma* f,
         Http2Iudicium* j)
{
    i32 k;
    i32 id;
    i32 valor;

    per (k = ZEPHYRUM; http2_positionem_legere(f, k, &id, &valor); k++)
    {
        si (id == HTTP2_POSITIO_IMPULSUS && valor > I)
        {
            _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
            redde;
        }
        si (   id == HTTP2_POSITIO_FENESTRA
            && valor > HTTP2_FENESTRA_SUPREMA)
        {
            _malum(j, HTTP2_ERROR_FLUXUS, VERUM);
            redde;
        }
        si (   id == HTTP2_POSITIO_FORMA
            && (valor < HTTP2_FORMA_MINIMA
            || valor > HTTP2_FORMA_SUPREMA))
        {
            _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
            redde;
        }
    }
}

/* impletio (DATA, HEADERS, PUSH_PROMISE): octetus primus = longitudo
 * impletionis, quae onus reliquum non aequare potest */
interior b32
_impletum_demere (
       Http2Forma* f,
    Http2Iudicium* j)
{
    i32 impletio;

    si (!(f->vexilla & HTTP2_IMPLETUM))
    {
        redde VERUM;
    }
    si (f->contentum_n < I)
    {
        _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
        redde FALSUM;
    }
    impletio = f->contentum[ZEPHYRUM];
    f->contentum++;
    f->contentum_n--;
    /* impletio >= onus totum (= contentum_n + I) */
    si (impletio > f->contentum_n)
    {
        _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
        redde FALSUM;
    }
    f->contentum_n -= impletio;
    redde VERUM;
}

i32
http2_formam_legere (
      constans i8* datum,
              i32  n,
              i32  forma_maxima,
       Http2Forma* f,
    Http2Iudicium* j)
{
    i32 longitudo;
    b32 rivus_nullus;

    si (!datum || !f || !j || n < HTTP2_CAPUT_FORMAE)
    {
        redde ZEPHYRUM;
    }
    j->bona         = VERUM;
    j->codex        = HTTP2_ERROR_NULLUS;
    j->connexionis  = FALSUM;
    longitudo = ((i32)datum[ZEPHYRUM] << XVI) | ((i32)datum[I] << VIII)
              | (i32)datum[II];
    f->genus    = datum[III];
    f->vexilla  = datum[IV];
    f->rivus = http2_u32_legere(datum
        + V) & HTTP2_FENESTRA_SUPREMA;
    f->contentum    = datum + HTTP2_CAPUT_FORMAE;
    f->contentum_n  = longitudo;
    f->longitudo    = longitudo;
    si (longitudo > forma_maxima)
    {
        _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
        redde HTTP2_CAPUT_FORMAE;
    }
    si (n < HTTP2_CAPUT_FORMAE + longitudo)
    {
        redde ZEPHYRUM;
    }
    rivus_nullus = f->rivus == ZEPHYRUM;
    commutatio (f->genus)
    {
        casus HTTP2_DATA:
            si (rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
                frange;
            }
            (vacuum)_impletum_demere(f, j);
            frange;
        casus HTTP2_CAPITA:
            si (rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
                frange;
            }
            si (!_impletum_demere(f, j))
            {
                frange;
            }
            si (f->vexilla & HTTP2_CUM_PRIORITATE)
            {
                si (f->contentum_n < LONGITUDO_PRIORITATIS)
                {
                    _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
                    frange;
                }
                f->contentum    += LONGITUDO_PRIORITATIS;
                f->contentum_n  -= LONGITUDO_PRIORITATIS;
            }
            frange;
        casus HTTP2_PRIORITAS:
            si (rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
            }
            alioquin si (longitudo != LONGITUDO_PRIORITATIS)
            {
                _malum(j, HTTP2_ERROR_MAGNITUDO, FALSUM);
            }
            frange;
        casus HTTP2_ABROGATIO:
            si (rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
            }
            alioquin si (longitudo != LONGITUDO_ABROGATIONIS)
            {
                _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
            }
            frange;
        casus HTTP2_POSITIONES:
            si (!rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
            }
            alioquin si (f->vexilla & HTTP2_RECEPTUM)
            {
                si (longitudo != ZEPHYRUM)
                {
                    _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
                }
            }
            alioquin si (longitudo % LONGITUDO_POSITIONIS != ZEPHYRUM)
            {
                _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
            }
            alioquin
            {
                _positiones_iudicare(f, j);
            }
            frange;
        casus HTTP2_PROMISSIO:
            si (rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
                frange;
            }
            (vacuum)_impletum_demere(f, j);
            frange;
        casus HTTP2_PULSUS:
            si (!rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
            }
            alioquin si (longitudo != LONGITUDO_PULSUS)
            {
                _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
            }
            frange;
        casus HTTP2_DISCESSUS:
            si (!rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
            }
            alioquin si (longitudo < LONGITUDO_DISCESSUS)
            {
                _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
            }
            frange;
        casus HTTP2_FENESTRA:
            si (longitudo != LONGITUDO_FENESTRAE)
            {
                _malum(j, HTTP2_ERROR_MAGNITUDO, VERUM);
            }
            alioquin si ((http2_u32_legere(f->contentum)
                          & HTTP2_FENESTRA_SUPREMA) == ZEPHYRUM)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, rivus_nullus);
            }
            frange;
        casus HTTP2_CONTINUATIO:
            si (rivus_nullus)
            {
                _malum(j, HTTP2_ERROR_PROTOCOLLI, VERUM);
            }
            frange;
        ordinarius:
            /* 4.1: genera ignota praetereuntur */
            frange;
    }
    redde HTTP2_CAPUT_FORMAE + longitudo;
}
