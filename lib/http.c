/* http.c - Implementatio HTTP Client
 *
 * Pure C89 HTTP/1.1 client.
 */

#include "http.h"
#include "tls.h"
#include "tcp.h"
#include "chorda_aedificator.h"
#include "via.h"

#include <string.h>


/* ========================================================================
 * CONSTANTES
 * ======================================================================== */

#define HTTP_CAPITA_MAXIMA      LXIV        /* 64 headers maxime */
#define HTTP_BUFFER_MAGNITUDO   MMMMXCVI    /* 4096 bytes */
#define HTTP_CORPUS_INITIALE    MMMMXCVI    /* 4096 bytes initiale */


/* ========================================================================
 * STRUCTURA INTERNA
 * ======================================================================== */

structura HttpPetitio {
    Piscina*     piscina;
    HttpMethodus methodus;

    /* URL partes */
    chorda schema;       /* "http" vel "https" */
    chorda hospes;       /* "api.example.com" */
    i32    portus;       /* 443, 80, etc. */
    chorda via;          /* "/path/to/resource" */

    /* Corpus */
    chorda corpus;

    /* Capita */
    HttpCaput capita[HTTP_CAPITA_MAXIMA];
    i32       capita_numerus;
};


/* ========================================================================
 * FUNCTIONES INTERNAE - CHORDA UTILITAS
 * ======================================================================== */

/* Creare chorda ex partibus (similis chorda_ex_literis sed cum longitudine) */
interior chorda
_chorda_ex_partibus(constans character* datum, i32 mensura, Piscina* piscina)
{
    chorda res;
    i8* buffer;

    si (!datum || mensura <= 0 || !piscina)
    {
        res.datum = NIHIL;
        res.mensura = 0;
        redde res;
    }

    buffer = (i8*)piscina_allocare(piscina, (i64)(mensura + I));
    memcpy(buffer, datum, (size_t)mensura);
    buffer[mensura] = '\0';

    res.datum = buffer;
    res.mensura = mensura;
    redde res;
}

/* Comparare chorda cum literis case-insensitive */
interior b32
_chorda_aequalis_literis_ignora_casus(chorda s, constans character* cstr)
{
    i32 len;
    i32 i;

    si (!cstr)
    {
        redde s.mensura == 0;
    }

    len = (i32)strlen(cstr);
    si (s.mensura != len)
    {
        redde FALSUM;
    }

    per (i = 0; i < len; i++)
    {
        character a = (character)s.datum[i];
        character b = cstr[i];

        /* Convertere ad minusculas */
        si (a >= 'A' && a <= 'Z')
        {
            a = (character)(a + ('a' - 'A'));
        }
        si (b >= 'A' && b <= 'Z')
        {
            b = (character)(b + ('a' - 'A'));
        }

        si (a != b)
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - NUMERI
 * ======================================================================== */

/* Convertere chorda ad i32 */
interior i32
_chorda_ad_i32(chorda s)
{
    i32 resultatus = 0;
    i32 i;

    per (i = 0; i < s.mensura; i++)
    {
        si (s.datum[i] >= '0' && s.datum[i] <= '9')
        {
            resultatus = resultatus * X + (i32)(s.datum[i] - '0');
        }
        alioquin
        {
            frange;
        }
    }

    redde resultatus;
}

/* Convertere hex character ad valor
 * Redde: valor (0-15) vel -1 si invalidum
 * Nota: Utitur integer (signed) pro -1 redditio
 */
interior integer
_hex_ad_valor(i8 c)
{
    si (c >= '0' && c <= '9') redde c - '0';
    si (c >= 'A' && c <= 'F') redde c - 'A' + X;
    si (c >= 'a' && c <= 'f') redde c - 'a' + X;
    redde -1;
}

/* Decodificare chunked transfer encoding
 *
 * Format:
 *   <hex-size>\r\n
 *   <data>\r\n
 *   <hex-size>\r\n
 *   <data>\r\n
 *   0\r\n
 *   \r\n
 */
interior chorda
_decodificare_chunked(constans i8* data, i32 len, Piscina* piscina)
{
    chorda resultatus;
    i8*    output;
    i32    output_pos;
    i32    i;
    i32    chunk_size;
    integer hex_val;

    /* Allocare output buffer (maxime = input longitudo) */
    output = piscina_allocare(piscina, len);
    output_pos = 0;
    i = 0;

    dum (i < len)
    {
        /* Legere chunk size (hexadecimal) */
        chunk_size = 0;
        dum (i < len)
        {
            hex_val = _hex_ad_valor(data[i]);
            si (hex_val >= 0)
            {
                chunk_size = chunk_size * XVI + (i32)hex_val;
                i++;
            }
            alioquin
            {
                frange;
            }
        }

        /* Saltare \r\n post chunk size */
        si (i < len && data[i] == '\r') i++;
        si (i < len && data[i] == '\n') i++;

        /* Si chunk_size == 0, finis */
        si (chunk_size == 0)
        {
            frange;
        }

        /* Copiare chunk data */
        si (i + chunk_size <= len)
        {
            memcpy(output + output_pos, data + i, (size_t)chunk_size);
            output_pos += chunk_size;
            i += chunk_size;
        }
        alioquin
        {
            /* Truncatus - copiare quod possumus */
            i32 available = len - i;
            memcpy(output + output_pos, data + i, (size_t)available);
            output_pos += available;
            frange;
        }

        /* Saltare \r\n post chunk data */
        si (i < len && data[i] == '\r') i++;
        si (i < len && data[i] == '\n') i++;
    }

    resultatus.datum = output;
    resultatus.mensura = output_pos;

    redde resultatus;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - URL PARSING
 * ======================================================================== */

interior b32
_parse_url(HttpPetitio* pet, constans character* url)
{
    constans character* p = url;
    constans character* schema_end;
    constans character* hospes_start;
    constans character* hospes_end;
    constans character* portus_start;
    constans character* via_start;
    i32 len;

    /* Schema: http:// vel https:// */
    schema_end = strstr(p, "://");
    si (!schema_end)
    {
        redde FALSUM;
    }

    len = (i32)(schema_end - p);
    pet->schema = _chorda_ex_partibus(p, len, pet->piscina);

    p = schema_end + III;  /* Saltare "://" */
    hospes_start = p;

    /* Default portus */
    si (chorda_aequalis_literis(pet->schema, "https"))
    {
        pet->portus = CDXL + III;  /* 443 */
    }
    alioquin
    {
        pet->portus = LXXX;  /* 80 */
    }

    /* Invenire finem hospitis */
    hospes_end = NIHIL;
    portus_start = NIHIL;
    via_start = NIHIL;

    dum (*p != '\0')
    {
        si (*p == ':')
        {
            hospes_end = p;
            portus_start = p + I;
            p++;
            perge;
        }
        si (*p == '/')
        {
            si (!hospes_end)
            {
                hospes_end = p;
            }
            via_start = p;
            frange;
        }
        p++;
    }

    si (!hospes_end)
    {
        hospes_end = p;  /* Finis stringae */
    }

    /* Hospes */
    len = (i32)(hospes_end - hospes_start);
    pet->hospes = _chorda_ex_partibus(hospes_start, len, pet->piscina);

    /* Portus (si specificatum) */
    si (portus_start && via_start)
    {
        i32 portus_val = 0;
        constans character* pp = portus_start;
        dum (pp < via_start && *pp >= '0' && *pp <= '9')
        {
            portus_val = portus_val * X + (i32)(*pp - '0');
            pp++;
        }
        si (portus_val > 0)
        {
            pet->portus = portus_val;
        }
    }
    alioquin si (portus_start && !via_start)
    {
        i32 portus_val = 0;
        constans character* pp = portus_start;
        dum (*pp >= '0' && *pp <= '9')
        {
            portus_val = portus_val * X + (i32)(*pp - '0');
            pp++;
        }
        si (portus_val > 0)
        {
            pet->portus = portus_val;
        }
    }

    /* Via */
    si (via_start)
    {
        pet->via = chorda_ex_literis(via_start, pet->piscina);
    }
    alioquin
    {
        pet->via = chorda_ex_literis("/", pet->piscina);
    }

    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES INTERNAE - REQUEST BUILDING
 * ======================================================================== */

interior chorda
_construere_petitio(HttpPetitio* pet)
{
    ChordaAedificator* aed;
    i32 i;
    b32 habet_host = FALSUM;
    b32 habet_content_length = FALSUM;

    aed = chorda_aedificator_creare(pet->piscina, DXII);

    /* Request line: METHOD /path HTTP/1.1\r\n */
    chorda_aedificator_appendere_literis(aed, http_methodus_nomen(pet->methodus));
    chorda_aedificator_appendere_character(aed, ' ');
    chorda_aedificator_appendere_chorda(aed, pet->via);
    chorda_aedificator_appendere_literis(aed, " HTTP/1.1\r\n");

    /* Verificare si Host iam in capita */
    per (i = 0; i < pet->capita_numerus; i++)
    {
        si (_chorda_aequalis_literis_ignora_casus(pet->capita[i].titulus, "Host"))
        {
            habet_host = VERUM;
        }
        si (_chorda_aequalis_literis_ignora_casus(pet->capita[i].titulus, "Content-Length"))
        {
            habet_content_length = VERUM;
        }
    }

    /* Host header (si non iam addita) */
    si (!habet_host)
    {
        chorda_aedificator_appendere_literis(aed, "Host: ");
        chorda_aedificator_appendere_chorda(aed, pet->hospes);
        chorda_aedificator_appendere_literis(aed, "\r\n");
    }

    /* User capita */
    per (i = 0; i < pet->capita_numerus; i++)
    {
        chorda_aedificator_appendere_chorda(aed, pet->capita[i].titulus);
        chorda_aedificator_appendere_literis(aed, ": ");
        chorda_aedificator_appendere_chorda(aed, pet->capita[i].valor);
        chorda_aedificator_appendere_literis(aed, "\r\n");
    }

    /* Content-Length si corpus */
    si (pet->corpus.mensura > 0 && !habet_content_length)
    {
        chorda_aedificator_appendere_literis(aed, "Content-Length: ");
        chorda_aedificator_appendere_i32(aed, pet->corpus.mensura);
        chorda_aedificator_appendere_literis(aed, "\r\n");
    }

    /* Connection: close */
    chorda_aedificator_appendere_literis(aed, "Connection: close\r\n");

    /* Finis capitum */
    chorda_aedificator_appendere_literis(aed, "\r\n");

    /* Corpus si praesens */
    si (pet->corpus.mensura > 0)
    {
        chorda_aedificator_appendere_chorda(aed, pet->corpus);
    }

    redde chorda_aedificator_finire(aed);
}


/* ========================================================================
 * FUNCTIONES INTERNAE - RESPONSE PARSING
 * ======================================================================== */

interior HttpResultus
_creare_error(HttpError error, constans character* msg, Piscina* piscina)
{
    HttpResultus res;
    res.successus = FALSUM;
    res.responsum = NIHIL;
    res.error = error;
    si (msg && piscina)
    {
        res.error_descriptio = chorda_ex_literis(msg, piscina);
    }
    alioquin
    {
        res.error_descriptio.datum = NIHIL;
        res.error_descriptio.mensura = 0;
    }
    redde res;
}

interior b32
_parse_status_line(constans character* data, i32 len, HttpResponsum* resp, Piscina* piscina)
{
    constans character* p = data;
    constans character* end = data + len;
    constans character* desc_start;
    i32 status_val = 0;

    /* HTTP/1.1 200 OK\r\n */

    /* Saltare "HTTP/1.x " */
    dum (p < end && *p != ' ')
    {
        p++;
    }
    si (p >= end)
    {
        redde FALSUM;
    }
    p++;  /* Saltare spatium */

    /* Parse status code */
    dum (p < end && *p >= '0' && *p <= '9')
    {
        status_val = status_val * X + (i32)(*p - '0');
        p++;
    }

    si (p >= end || status_val == 0)
    {
        redde FALSUM;
    }

    resp->status = status_val;

    /* Saltare spatium */
    si (*p == ' ')
    {
        p++;
    }

    /* Status descriptio */
    desc_start = p;
    dum (p < end && *p != '\r' && *p != '\n')
    {
        p++;
    }

    resp->status_descriptio = _chorda_ex_partibus(desc_start, (i32)(p - desc_start), piscina);

    redde VERUM;
}

interior b32
_parse_header(constans character* line, i32 len, HttpCaput* caput, Piscina* piscina)
{
    constans character* p = line;
    constans character* end = line + len;
    constans character* colon;
    constans character* valor_start;

    /* Invenire colon */
    colon = NIHIL;
    dum (p < end)
    {
        si (*p == ':')
        {
            colon = p;
            frange;
        }
        p++;
    }

    si (!colon)
    {
        redde FALSUM;
    }

    /* Titulus */
    caput->titulus = _chorda_ex_partibus(line, (i32)(colon - line), piscina);

    /* Valor (saltare spatia) */
    valor_start = colon + I;
    dum (valor_start < end && (*valor_start == ' ' || *valor_start == '\t'))
    {
        valor_start++;
    }

    /* Removere \r\n in fine */
    p = end;
    dum (p > valor_start && (*(p - I) == '\r' || *(p - I) == '\n'))
    {
        p--;
    }

    caput->valor = _chorda_ex_partibus(valor_start, (i32)(p - valor_start), piscina);

    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - PETITIO
 * ======================================================================== */

HttpPetitio*
http_petitio_creare(
    Piscina*            piscina,
    HttpMethodus        methodus,
    constans character* url)
{
    HttpPetitio* pet;

    si (!piscina || !url)
    {
        redde NIHIL;
    }

    pet = (HttpPetitio*)piscina_allocare(piscina, (i64)magnitudo(HttpPetitio));
    si (!pet)
    {
        redde NIHIL;
    }

    pet->piscina = piscina;
    pet->methodus = methodus;
    pet->capita_numerus = 0;
    pet->corpus.datum = NIHIL;
    pet->corpus.mensura = 0;

    si (!_parse_url(pet, url))
    {
        redde NIHIL;
    }

    redde pet;
}

vacuum
http_petitio_caput_addere(
    HttpPetitio*        petitio,
    constans character* titulus,
    constans character* valor)
{
    si (!petitio || !titulus || !valor)
    {
        redde;
    }

    si (petitio->capita_numerus >= HTTP_CAPITA_MAXIMA)
    {
        redde;
    }

    petitio->capita[petitio->capita_numerus].titulus =
        chorda_ex_literis(titulus, petitio->piscina);
    petitio->capita[petitio->capita_numerus].valor =
        chorda_ex_literis(valor, petitio->piscina);
    petitio->capita_numerus++;
}

vacuum
http_petitio_corpus_ponere(
    HttpPetitio*        petitio,
    constans character* corpus,
    i32                 mensura)
{
    si (!petitio || !corpus || mensura <= 0)
    {
        redde;
    }

    petitio->corpus = _chorda_ex_partibus(corpus, mensura, petitio->piscina);
}

vacuum
http_petitio_corpus_ponere_chorda(
    HttpPetitio* petitio,
    chorda       corpus)
{
    si (!petitio)
    {
        redde;
    }

    petitio->corpus = corpus;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - EXSEQUI
 * ======================================================================== */

HttpResultus
http_exsequi(
    HttpPetitio* petitio,
    Piscina*     piscina)
{
    HttpResultus res;
    HttpResponsum* resp;
    chorda petitio_str;
    TlsConnexio* tls_conn = NIHIL;
    TcpConnexio* tcp_conn = NIHIL;
    b32 est_https;
    i8* buffer;
    i8* total_data;
    i32 total_size;
    i32 total_capacity;
    i32 n;
    constans character* headers_end;
    constans character* line_start;
    constans character* line_end;

    si (!petitio || !piscina)
    {
        redde _creare_error(HTTP_ERROR_URL, "Argumenta invalida", piscina);
    }

    /* Allocare responsum */
    resp = (HttpResponsum*)piscina_allocare(piscina, (i64)magnitudo(HttpResponsum));
    si (!resp)
    {
        redde _creare_error(HTTP_ERROR_CONNEXIO, "Allocatio fallita", piscina);
    }

    resp->capita = (HttpCaput*)piscina_allocare(piscina,
                                                 (i64)(HTTP_CAPITA_MAXIMA * (i32)magnitudo(HttpCaput)));
    resp->capita_numerus = 0;

    /* Construere petitio string */
    petitio_str = _construere_petitio(petitio);

    /* Determinare si HTTPS */
    est_https = chorda_aequalis_literis(petitio->schema, "https");

    /* Connectere */
    si (est_https)
    {
        TlsResultus tls_res = tls_connectere((constans character*)petitio->hospes.datum,
                                              petitio->portus,
                                              piscina);
        si (!tls_res.successus)
        {
            res.successus = FALSUM;
            res.responsum = NIHIL;
            res.error = HTTP_ERROR_CONNEXIO;
            res.error_descriptio = tls_res.error_descriptio;
            redde res;
        }
        tls_conn = tls_res.connexio;
    }
    alioquin
    {
        TcpResultus tcp_res = tcp_connectere((constans character*)petitio->hospes.datum,
                                              petitio->portus,
                                              piscina);
        si (!tcp_res.successus)
        {
            res.successus = FALSUM;
            res.responsum = NIHIL;
            res.error = HTTP_ERROR_CONNEXIO;
            res.error_descriptio = tcp_res.error_descriptio;
            redde res;
        }
        tcp_conn = tcp_res.connexio;
    }

    /* Mittere petitio */
    si (est_https)
    {
        si (!tls_mittere_omnia(tls_conn, (constans i8*)petitio_str.datum, petitio_str.mensura))
        {
            tls_claudere(tls_conn);
            redde _creare_error(HTTP_ERROR_IO, "Mittere fallita", piscina);
        }
    }
    alioquin
    {
        si (!tcp_mittere_omnia(tcp_conn, (constans i8*)petitio_str.datum, petitio_str.mensura))
        {
            tcp_claudere(tcp_conn);
            redde _creare_error(HTTP_ERROR_IO, "Mittere fallita", piscina);
        }
    }

    /* Recipere responsum */
    buffer = (i8*)piscina_allocare(piscina, (i64)HTTP_BUFFER_MAGNITUDO);
    total_capacity = HTTP_CORPUS_INITIALE * IV;
    total_data = (i8*)piscina_allocare(piscina, (i64)total_capacity);
    total_size = 0;

    fac
    {
        si (est_https)
        {
            n = tls_recipere(tls_conn, buffer, HTTP_BUFFER_MAGNITUDO);
        }
        alioquin
        {
            n = tcp_recipere(tcp_conn, buffer, HTTP_BUFFER_MAGNITUDO);
        }

        si (n > 0)
        {
            /* Expandere si necesse */
            si (total_size + n > total_capacity)
            {
                i8* nova_data;
                i32 nova_capacitas = total_capacity * II;
                dum (nova_capacitas < total_size + n)
                {
                    nova_capacitas *= II;
                }
                nova_data = (i8*)piscina_allocare(piscina, (i64)nova_capacitas);
                memcpy(nova_data, total_data, (size_t)total_size);
                total_data = nova_data;
                total_capacity = nova_capacitas;
            }

            memcpy(total_data + total_size, buffer, (size_t)n);
            total_size += n;
        }
    } dum (n > 0);

    /* Claudere connexio */
    si (est_https)
    {
        tls_claudere(tls_conn);
    }
    alioquin
    {
        tcp_claudere(tcp_conn);
    }

    si (total_size == 0)
    {
        redde _creare_error(HTTP_ERROR_IO, "Responsum vacuum", piscina);
    }

    /* Parse responsum */
    /* Invenire finem capitum (\r\n\r\n) */
    headers_end = NIHIL;
    {
        i32 i;
        per (i = 0; i < total_size - III; i++)
        {
            si (total_data[i] == '\r' && total_data[i + I] == '\n' &&
                total_data[i + II] == '\r' && total_data[i + III] == '\n')
            {
                headers_end = (constans character*)(total_data + i);
                frange;
            }
        }
    }

    si (!headers_end)
    {
        redde _creare_error(HTTP_ERROR_PARSE, "Capita non inventa", piscina);
    }

    /* Parse status line */
    line_start = (constans character*)total_data;
    line_end = strstr(line_start, "\r\n");
    si (!line_end || line_end > headers_end)
    {
        redde _creare_error(HTTP_ERROR_PARSE, "Status line invalida", piscina);
    }

    si (!_parse_status_line(line_start, (i32)(line_end - line_start), resp, piscina))
    {
        redde _creare_error(HTTP_ERROR_PARSE, "Status line parse fallita", piscina);
    }

    /* Parse headers */
    line_start = line_end + II;  /* Post \r\n */
    dum (line_start < headers_end)
    {
        line_end = strstr(line_start, "\r\n");
        si (!line_end || line_end > headers_end)
        {
            frange;
        }

        si (line_end > line_start && resp->capita_numerus < HTTP_CAPITA_MAXIMA)
        {
            _parse_header(line_start, (i32)(line_end - line_start),
                         &resp->capita[resp->capita_numerus], piscina);
            resp->capita_numerus++;
        }

        line_start = line_end + II;
    }

    /* Corpus - cum Transfer-Encoding et Content-Length supporto */
    {
        constans character* body_start = headers_end + IV;  /* Post \r\n\r\n */
        i32 body_len = total_size - (i32)(body_start - (constans character*)total_data);
        chorda transfer_encoding;
        chorda content_length_hdr;

        si (body_len > 0)
        {
            /* Verificare Transfer-Encoding: chunked */
            transfer_encoding = http_responsum_caput(resp, "Transfer-Encoding");
            si (transfer_encoding.mensura > 0 &&
                _chorda_aequalis_literis_ignora_casus(transfer_encoding, "chunked"))
            {
                /* Decodificare chunked encoding */
                resp->corpus = _decodificare_chunked((constans i8*)body_start,
                                                      body_len, piscina);
            }
            alioquin
            {
                /* Verificare Content-Length */
                content_length_hdr = http_responsum_caput(resp, "Content-Length");
                si (content_length_hdr.mensura > 0)
                {
                    i32 expected_len = _chorda_ad_i32(content_length_hdr);
                    si (expected_len > 0 && expected_len <= body_len)
                    {
                        /* Usare exacte Content-Length bytes */
                        resp->corpus = _chorda_ex_partibus(body_start,
                                                           expected_len, piscina);
                    }
                    alioquin
                    {
                        /* Content-Length maior quam data disponibilia */
                        resp->corpus = _chorda_ex_partibus(body_start,
                                                           body_len, piscina);
                    }
                }
                alioquin
                {
                    /* Nullum Content-Length - usare omnia */
                    resp->corpus = _chorda_ex_partibus(body_start, body_len, piscina);
                }
            }
        }
        alioquin
        {
            resp->corpus.datum = NIHIL;
            resp->corpus.mensura = 0;
        }
    }

    /* Successus */
    res.successus = VERUM;
    res.responsum = resp;
    res.error = HTTP_OK;
    res.error_descriptio.datum = NIHIL;
    res.error_descriptio.mensura = 0;

    redde res;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - RESPONSUM
 * ======================================================================== */

chorda
http_responsum_caput(
    HttpResponsum*      responsum,
    constans character* titulus)
{
    chorda vacua;
    i32 i;

    vacua.datum = NIHIL;
    vacua.mensura = 0;

    si (!responsum || !titulus)
    {
        redde vacua;
    }

    per (i = 0; i < responsum->capita_numerus; i++)
    {
        si (_chorda_aequalis_literis_ignora_casus(responsum->capita[i].titulus, titulus))
        {
            redde responsum->capita[i].valor;
        }
    }

    redde vacua;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - UTILITAS
 * ======================================================================== */

constans character*
http_methodus_nomen(HttpMethodus methodus)
{
    commutatio (methodus)
    {
        casus HTTP_GET:     redde "GET";
        casus HTTP_POST:    redde "POST";
        casus HTTP_PUT:     redde "PUT";
        casus HTTP_DELETE:  redde "DELETE";
        casus HTTP_PATCH:   redde "PATCH";
        casus HTTP_HEAD:    redde "HEAD";
        casus HTTP_OPTIONS: redde "OPTIONS";
        ordinarius:         redde "GET";
    }
}

constans character*
http_error_descriptio(HttpError error)
{
    commutatio (error)
    {
        casus HTTP_OK:              redde "OK";
        casus HTTP_ERROR_URL:       redde "URL invalida";
        casus HTTP_ERROR_CONNEXIO:  redde "Connexio fallita";
        casus HTTP_ERROR_TIMEOUT:   redde "Timeout";
        casus HTTP_ERROR_PARSE:     redde "Parse error";
        casus HTTP_ERROR_IO:        redde "I/O error";
        casus HTTP_ERROR_REDIRECTIO: redde "Nimis redirectiones";
        ordinarius:                 redde "Error ignotus";
    }
}


/* ========================================================================
 * FUNCTIONES INTERNAE - URL RELATIVUM
 * ======================================================================== */

/* Resolvere URL relativum contra petitio base
 *
 * Casus:
 *   "https://other.com/path"  -> usare ut est (absolutum)
 *   "//other.com/path"        -> schema originale + "//other.com/path"
 *   "/new-path"               -> schema://hospes:portus + "/new-path"
 *   "new-path"                -> schema://hospes:portus + via_directory + "new-path"
 */
interior constans character*
_resolvere_url_relativum(
    HttpPetitio* base,
    chorda       location,
    Piscina*     piscina)
{
    ChordaAedificator* aed;
    chorda resultatus;
    i32 i;
    i32 ultima_slash;

    /* Casus 1: URL absolutum (continet "://") */
    per (i = 0; i < location.mensura - II; i++)
    {
        si (location.datum[i] == ':' &&
            location.datum[i + I] == '/' &&
            location.datum[i + II] == '/')
        {
            /* Est URL absolutum - copiare ut est */
            character* buffer = (character*)piscina_allocare(piscina,
                                                              (i64)(location.mensura + I));
            per (i = 0; i < location.mensura; i++)
            {
                buffer[i] = (character)location.datum[i];
            }
            buffer[location.mensura] = '\0';
            redde buffer;
        }
    }

    aed = chorda_aedificator_creare(piscina, CCLVI);

    /* Casus 2: Protocol-relative (incipit cum "//") */
    si (location.mensura >= II &&
        location.datum[0] == '/' &&
        location.datum[I] == '/')
    {
        chorda_aedificator_appendere_chorda(aed, base->schema);
        chorda_aedificator_appendere_character(aed, ':');
        per (i = 0; i < location.mensura; i++)
        {
            chorda_aedificator_appendere_character(aed, (character)location.datum[i]);
        }
        resultatus = chorda_aedificator_finire(aed);
        redde (constans character*)resultatus.datum;
    }

    /* Casus 3 et 4: Via absolutum vel relativum - necessitamus base */
    chorda_aedificator_appendere_chorda(aed, base->schema);
    chorda_aedificator_appendere_literis(aed, "://");
    chorda_aedificator_appendere_chorda(aed, base->hospes);

    /* Addere portus si non standard */
    si ((chorda_aequalis_literis(base->schema, "https") && base->portus != 443) ||
        (chorda_aequalis_literis(base->schema, "http") && base->portus != 80))
    {
        chorda_aedificator_appendere_character(aed, ':');
        chorda_aedificator_appendere_i32(aed, base->portus);
    }

    /* Casus 3: Via absolutum (incipit cum "/") */
    si (location.mensura > 0 && location.datum[0] == '/')
    {
        chorda via_normalizata;
        chorda location_chorda;

        /* Creare chorda ex location */
        location_chorda.datum = location.datum;
        location_chorda.mensura = location.mensura;

        /* Normalizare viam (resolvere . et ..) */
        via_normalizata = via_normalizare(location_chorda, piscina);

        chorda_aedificator_appendere_chorda(aed, via_normalizata);
        resultatus = chorda_aedificator_finire(aed);
        redde (constans character*)resultatus.datum;
    }

    /* Casus 4: Via relativum - resolvere contra directory current */
    {
        ChordaAedificator* via_aed;
        chorda via_combinata;
        chorda via_normalizata;

        via_aed = chorda_aedificator_creare(piscina, CXXVIII);

        /* Invenire ultimam "/" in via base */
        ultima_slash = 0;
        per (i = 0; i < base->via.mensura; i++)
        {
            si (base->via.datum[i] == '/')
            {
                ultima_slash = i;
            }
        }

        /* Copiare via usque ad ultimam "/" (inclusive) */
        per (i = 0; i <= ultima_slash; i++)
        {
            chorda_aedificator_appendere_character(via_aed,
                                                    (character)base->via.datum[i]);
        }

        /* Appendere location relativum */
        per (i = 0; i < location.mensura; i++)
        {
            chorda_aedificator_appendere_character(via_aed,
                                                    (character)location.datum[i]);
        }

        via_combinata = chorda_aedificator_finire(via_aed);

        /* Normalizare viam (resolvere . et ..) */
        via_normalizata = via_normalizare(via_combinata, piscina);

        chorda_aedificator_appendere_chorda(aed, via_normalizata);
    }

    resultatus = chorda_aedificator_finire(aed);
    redde (constans character*)resultatus.datum;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - REDIRECTIONES
 * ======================================================================== */

HttpResultus
http_exsequi_cum_redirectionibus(
    HttpPetitio* petitio,
    Piscina*     piscina,
    i32          max_redirectiones)
{
    HttpResultus res;
    chorda location;
    i32 redirectiones = 0;
    constans character* nova_url;
    HttpPetitio* nova_petitio;
    HttpPetitio* current_petitio;
    i32 i;

    si (!petitio || !piscina || max_redirectiones < 0)
    {
        redde _creare_error(HTTP_ERROR_URL, "Argumenta invalida", piscina);
    }

    /* Prima petitio */
    current_petitio = petitio;
    res = http_exsequi(current_petitio, piscina);

    /* Sequere redirectiones */
    dum (res.successus && redirectiones < max_redirectiones)
    {
        /* Verificare si est redirectio */
        si (res.responsum->status != 301 &&    /* Moved Permanently */
            res.responsum->status != 302 &&    /* Found */
            res.responsum->status != 307 &&    /* Temporary Redirect */
            res.responsum->status != 308)      /* Permanent Redirect */
        {
            /* Non est redirectio, redde resultatum */
            frange;
        }

        /* Obtinere Location header */
        location = http_responsum_caput(res.responsum, "Location");
        si (location.mensura == 0)
        {
            /* Nullum Location header, redde quod habemus */
            frange;
        }

        /* Resolvere URL (absolutum vel relativum) */
        nova_url = _resolvere_url_relativum(current_petitio, location, piscina);
        si (!nova_url)
        {
            redde _creare_error(HTTP_ERROR_URL, "Redirectio URL invalida", piscina);
        }

        /* Creare nova petitio */
        nova_petitio = http_petitio_creare(piscina, petitio->methodus, nova_url);
        si (!nova_petitio)
        {
            redde _creare_error(HTTP_ERROR_URL, "Redirectio URL invalida", piscina);
        }

        /* Copiare capita originales (excepto Host) */
        per (i = 0; i < petitio->capita_numerus; i++)
        {
            si (!_chorda_aequalis_literis_ignora_casus(petitio->capita[i].titulus, "Host"))
            {
                nova_petitio->capita[nova_petitio->capita_numerus] = petitio->capita[i];
                nova_petitio->capita_numerus++;
            }
        }

        /* Pro 307/308, preservare corpus */
        si (res.responsum->status == 307 || res.responsum->status == 308)
        {
            nova_petitio->corpus = petitio->corpus;
        }

        /* Actualizare current_petitio pro proxima redirectio */
        current_petitio = nova_petitio;

        /* Exsequi nova petitio */
        res = http_exsequi(nova_petitio, piscina);
        redirectiones++;
    }

    /* Verificare si nimis redirectiones */
    si (res.successus && redirectiones >= max_redirectiones)
    {
        si (res.responsum->status == 301 ||
            res.responsum->status == 302 ||
            res.responsum->status == 307 ||
            res.responsum->status == 308)
        {
            redde _creare_error(HTTP_ERROR_REDIRECTIO, "Nimis redirectiones", piscina);
        }
    }

    redde res;
}


/* ========================================================================
 * STRUCTURA INTERNA - SERVER PARSER
 * ======================================================================== */

#define HTTP_PARSER_BUFFER_INITIALE  MMMMXCVI    /* 4096 bytes */

structura HttpParser {
    Piscina*            piscina;
    HttpParseStatus     status;
    HttpPetitioServeri* petitio;

    /* Buffer pro data accumulata */
    i8*    buffer;
    i32    buffer_mensura;
    i32    buffer_capacitas;

    /* Parsing state */
    i32 cursor;           /* Current position in buffer */
    i32 linea_initium;    /* Start of current line */
};


/* ========================================================================
 * FUNCTIONES INTERNAE - SERVER PARSER
 * ======================================================================== */

/* Expandere buffer si necesse */
interior b32
_parser_expandere_buffer(HttpParser* parser, i32 additio)
{
    i32 nova_capacitas;
    i8* nova_buffer;

    si (parser->buffer_mensura + additio <= parser->buffer_capacitas)
    {
        redde VERUM;
    }

    nova_capacitas = parser->buffer_capacitas * II;
    dum (nova_capacitas < parser->buffer_mensura + additio)
    {
        nova_capacitas *= II;
    }

    nova_buffer = (i8*)piscina_allocare(parser->piscina, (i64)nova_capacitas);
    si (!nova_buffer)
    {
        redde FALSUM;
    }

    si (parser->buffer_mensura > 0)
    {
        memcpy(nova_buffer, parser->buffer, (size_t)parser->buffer_mensura);
    }

    parser->buffer = nova_buffer;
    parser->buffer_capacitas = nova_capacitas;

    redde VERUM;
}

/* Invenire CRLF in buffer ex positione
 * Redde: Positio CRLF vel I32_MAX si non invenit
 */
#define PARSER_NON_INVENIT  0xFFFFFFFFu

interior i32
_parser_invenire_crlf(HttpParser* parser, i32 initium)
{
    i32 i;

    si (parser->buffer_mensura < II)
    {
        redde PARSER_NON_INVENIT;
    }

    per (i = initium; i < parser->buffer_mensura - I; i++)
    {
        si (parser->buffer[i] == '\r' && parser->buffer[i + I] == '\n')
        {
            redde i;
        }
    }

    redde PARSER_NON_INVENIT;
}

/* Parse request line: METHOD URI HTTP/1.x */
interior b32
_parser_parse_linea_petitionis(HttpParser* parser, i32 linea_finis)
{
    i32 i = parser->linea_initium;
    i32 methodus_initium;
    i32 methodus_finis;
    i32 uri_initium;
    i32 uri_finis;
    i32 versio_initium;
    character methodus_str[XVI];
    i32 methodus_len;
    i32 j;

    /* Saltare leading whitespace */
    dum (i < linea_finis && (parser->buffer[i] == ' ' || parser->buffer[i] == '\t'))
    {
        i++;
    }

    /* Parse methodus */
    methodus_initium = i;
    dum (i < linea_finis && parser->buffer[i] != ' ')
    {
        i++;
    }
    methodus_finis = i;

    /* Copiare methodus ad string temporarium */
    methodus_len = methodus_finis - methodus_initium;
    si (methodus_len <= 0 || methodus_len >= XVI)
    {
        redde FALSUM;
    }

    per (j = 0; j < methodus_len; j++)
    {
        methodus_str[j] = (character)parser->buffer[methodus_initium + j];
    }
    methodus_str[methodus_len] = '\0';

    parser->petitio->methodus = http_methodus_ex_literis(methodus_str);

    /* Saltare spatia */
    dum (i < linea_finis && parser->buffer[i] == ' ')
    {
        i++;
    }

    /* Parse URI */
    uri_initium = i;
    dum (i < linea_finis && parser->buffer[i] != ' ')
    {
        i++;
    }
    uri_finis = i;

    si (uri_finis <= uri_initium)
    {
        redde FALSUM;
    }

    /* Creare URI chorda */
    parser->petitio->uri = _chorda_ex_partibus(
        (constans character*)&parser->buffer[uri_initium],
        uri_finis - uri_initium,
        parser->piscina);

    /* Separare via et quaestio */
    {
        i32 q;
        i32 uri_len = uri_finis - uri_initium;

        /* Invenire ? */
        per (q = 0; q < uri_len; q++)
        {
            si (parser->buffer[uri_initium + q] == '?')
            {
                frange;
            }
        }

        si (q < uri_len)
        {
            /* Habet quaestio */
            parser->petitio->via = _chorda_ex_partibus(
                (constans character*)&parser->buffer[uri_initium],
                q, parser->piscina);
            parser->petitio->quaestio = _chorda_ex_partibus(
                (constans character*)&parser->buffer[uri_initium + q + I],
                uri_len - q - I, parser->piscina);
        }
        alioquin
        {
            /* Nullum quaestio */
            parser->petitio->via = parser->petitio->uri;
            parser->petitio->quaestio.datum = NIHIL;
            parser->petitio->quaestio.mensura = 0;
        }
    }

    /* Saltare spatia */
    dum (i < linea_finis && parser->buffer[i] == ' ')
    {
        i++;
    }

    /* Parse HTTP version */
    versio_initium = i;

    /* Verificare "HTTP/1." */
    si (linea_finis - versio_initium >= VII &&
        parser->buffer[versio_initium] == 'H' &&
        parser->buffer[versio_initium + I] == 'T' &&
        parser->buffer[versio_initium + II] == 'T' &&
        parser->buffer[versio_initium + III] == 'P' &&
        parser->buffer[versio_initium + IV] == '/' &&
        parser->buffer[versio_initium + V] == '1' &&
        parser->buffer[versio_initium + VI] == '.')
    {
        si (linea_finis - versio_initium >= VIII &&
            parser->buffer[versio_initium + VII] == '1')
        {
            parser->petitio->versio = XI;  /* HTTP/1.1 */
        }
        alioquin
        {
            parser->petitio->versio = X;   /* HTTP/1.0 */
        }
    }
    alioquin
    {
        parser->petitio->versio = XI;  /* Default HTTP/1.1 */
    }

    /* Default keep-alive pro HTTP/1.1 */
    parser->petitio->keep_alive = (parser->petitio->versio == XI);

    redde VERUM;
}

/* Parse single header line */
interior b32
_parser_parse_caput(HttpParser* parser, i32 linea_finis)
{
    i32 i = parser->linea_initium;
    i32 colon_pos = PARSER_NON_INVENIT;
    i32 valor_initium;
    HttpCaput* caput;

    /* Invenire colon */
    dum (i < linea_finis)
    {
        si (parser->buffer[i] == ':')
        {
            colon_pos = i;
            frange;
        }
        i++;
    }

    si (colon_pos == PARSER_NON_INVENIT)
    {
        redde FALSUM;
    }

    /* Verificare capacitas */
    si (parser->petitio->capita_numerus >= HTTP_CAPITA_MAXIMA)
    {
        redde VERUM;  /* Ignorare extra headers */
    }

    caput = &parser->petitio->capita[parser->petitio->capita_numerus];

    /* Titulus */
    caput->titulus = _chorda_ex_partibus(
        (constans character*)&parser->buffer[parser->linea_initium],
        colon_pos - parser->linea_initium,
        parser->piscina);

    /* Valor (saltare spatia) */
    valor_initium = colon_pos + I;
    dum (valor_initium < linea_finis &&
         (parser->buffer[valor_initium] == ' ' ||
          parser->buffer[valor_initium] == '\t'))
    {
        valor_initium++;
    }

    caput->valor = _chorda_ex_partibus(
        (constans character*)&parser->buffer[valor_initium],
        linea_finis - valor_initium,
        parser->piscina);

    parser->petitio->capita_numerus++;

    /* Verificare special headers */
    si (_chorda_aequalis_literis_ignora_casus(caput->titulus, "Content-Length"))
    {
        parser->petitio->content_length = _chorda_ad_i32(caput->valor);
        parser->petitio->corpus_longitudo = parser->petitio->content_length;
    }
    alioquin si (_chorda_aequalis_literis_ignora_casus(caput->titulus, "Transfer-Encoding"))
    {
        si (_chorda_aequalis_literis_ignora_casus(caput->valor, "chunked"))
        {
            parser->petitio->chunked = VERUM;
        }
    }
    alioquin si (_chorda_aequalis_literis_ignora_casus(caput->titulus, "Connection"))
    {
        si (_chorda_aequalis_literis_ignora_casus(caput->valor, "close"))
        {
            parser->petitio->keep_alive = FALSUM;
        }
        alioquin si (_chorda_aequalis_literis_ignora_casus(caput->valor, "keep-alive"))
        {
            parser->petitio->keep_alive = VERUM;
        }
    }

    redde VERUM;
}


/* ========================================================================
 * FUNCTIONES PUBLICAE - SERVER PARSER
 * ======================================================================== */

HttpMethodus
http_methodus_ex_literis(constans character* methodus)
{
    si (!methodus)
    {
        redde HTTP_METHODUS_IGNOTUS;
    }

    si (strcmp(methodus, "GET") == 0)     redde HTTP_GET;
    si (strcmp(methodus, "POST") == 0)    redde HTTP_POST;
    si (strcmp(methodus, "PUT") == 0)     redde HTTP_PUT;
    si (strcmp(methodus, "DELETE") == 0)  redde HTTP_DELETE;
    si (strcmp(methodus, "PATCH") == 0)   redde HTTP_PATCH;
    si (strcmp(methodus, "HEAD") == 0)    redde HTTP_HEAD;
    si (strcmp(methodus, "OPTIONS") == 0) redde HTTP_OPTIONS;

    redde HTTP_METHODUS_IGNOTUS;
}

HttpParser*
http_parser_creare(Piscina* piscina)
{
    HttpParser* parser;
    HttpPetitioServeri* petitio;

    si (!piscina)
    {
        redde NIHIL;
    }

    parser = (HttpParser*)piscina_allocare(piscina, (i64)magnitudo(HttpParser));
    si (!parser)
    {
        redde NIHIL;
    }

    petitio = (HttpPetitioServeri*)piscina_allocare(piscina,
                                                     (i64)magnitudo(HttpPetitioServeri));
    si (!petitio)
    {
        redde NIHIL;
    }

    /* Allocare capita array */
    petitio->capita = (HttpCaput*)piscina_allocare(piscina,
                                                    (i64)(HTTP_CAPITA_MAXIMA * (i32)magnitudo(HttpCaput)));
    si (!petitio->capita)
    {
        redde NIHIL;
    }

    /* Initiare parser */
    parser->piscina = piscina;
    parser->status = HTTP_PARSE_LINEA_PETITIONIS;
    parser->petitio = petitio;

    /* Allocare buffer initiale */
    parser->buffer = (i8*)piscina_allocare(piscina, (i64)HTTP_PARSER_BUFFER_INITIALE);
    si (!parser->buffer)
    {
        redde NIHIL;
    }
    parser->buffer_mensura = 0;
    parser->buffer_capacitas = HTTP_PARSER_BUFFER_INITIALE;
    parser->cursor = 0;
    parser->linea_initium = 0;

    /* Initiare petitio */
    petitio->methodus = HTTP_GET;
    petitio->uri.datum = NIHIL;
    petitio->uri.mensura = 0;
    petitio->via.datum = NIHIL;
    petitio->via.mensura = 0;
    petitio->quaestio.datum = NIHIL;
    petitio->quaestio.mensura = 0;
    petitio->versio = XI;
    petitio->capita_numerus = 0;
    petitio->corpus.datum = NIHIL;
    petitio->corpus.mensura = 0;
    petitio->corpus_longitudo = 0;
    petitio->keep_alive = VERUM;
    petitio->chunked = FALSUM;
    petitio->content_length = 0;

    redde parser;
}

HttpParseResultus
http_parser_adicere(
    HttpParser*         parser,
    constans character* datum,
    i32                 longitudo)
{
    HttpParseResultus res;
    i32 crlf_pos;

    /* Default resultatus */
    res.successus = VERUM;
    res.completa = FALSUM;
    res.petitio = NIHIL;
    res.error = HTTP_OK;
    res.error_descriptio.datum = NIHIL;
    res.error_descriptio.mensura = 0;

    si (!parser || !datum || longitudo <= 0)
    {
        res.successus = FALSUM;
        res.error = HTTP_ERROR_PARSE;
        redde res;
    }

    si (parser->status == HTTP_PARSE_ERROR ||
        parser->status == HTTP_PARSE_COMPLETA)
    {
        res.petitio = parser->petitio;
        res.completa = (parser->status == HTTP_PARSE_COMPLETA);
        redde res;
    }

    /* Expandere buffer et adicere data */
    si (!_parser_expandere_buffer(parser, longitudo))
    {
        parser->status = HTTP_PARSE_ERROR;
        res.successus = FALSUM;
        res.error = HTTP_ERROR_PARSE;
        res.error_descriptio = chorda_ex_literis("Buffer overflow", parser->piscina);
        redde res;
    }

    memcpy(parser->buffer + parser->buffer_mensura, datum, (size_t)longitudo);
    parser->buffer_mensura += longitudo;

    /* State machine parsing */
    dum (parser->status != HTTP_PARSE_COMPLETA &&
         parser->status != HTTP_PARSE_ERROR)
    {
        commutatio (parser->status)
        {
            casus HTTP_PARSE_LINEA_PETITIONIS:
                crlf_pos = _parser_invenire_crlf(parser, parser->cursor);
                si (crlf_pos == PARSER_NON_INVENIT)
                {
                    /* Necesse plus data */
                    res.petitio = parser->petitio;
                    redde res;
                }

                si (!_parser_parse_linea_petitionis(parser, crlf_pos))
                {
                    parser->status = HTTP_PARSE_ERROR;
                    res.successus = FALSUM;
                    res.error = HTTP_ERROR_PARSE;
                    res.error_descriptio = chorda_ex_literis("Request line invalida",
                                                              parser->piscina);
                    redde res;
                }

                parser->cursor = crlf_pos + II;  /* Post \r\n */
                parser->linea_initium = parser->cursor;
                parser->status = HTTP_PARSE_CAPITA;
                frange;

            casus HTTP_PARSE_CAPITA:
                crlf_pos = _parser_invenire_crlf(parser, parser->cursor);
                si (crlf_pos == PARSER_NON_INVENIT)
                {
                    /* Necesse plus data */
                    res.petitio = parser->petitio;
                    redde res;
                }

                /* Linea vacua = finis capitum */
                si (crlf_pos == parser->linea_initium)
                {
                    parser->cursor = crlf_pos + II;

                    /* Verificare si expectamus corpus */
                    si (parser->petitio->content_length > 0)
                    {
                        parser->status = HTTP_PARSE_CORPUS;
                    }
                    alioquin si (parser->petitio->chunked)
                    {
                        /* TODO: Chunked not yet supported pro server */
                        parser->status = HTTP_PARSE_COMPLETA;
                    }
                    alioquin
                    {
                        parser->status = HTTP_PARSE_COMPLETA;
                    }
                    frange;
                }

                /* Parse header */
                si (!_parser_parse_caput(parser, crlf_pos))
                {
                    parser->status = HTTP_PARSE_ERROR;
                    res.successus = FALSUM;
                    res.error = HTTP_ERROR_PARSE;
                    res.error_descriptio = chorda_ex_literis("Header invalida",
                                                              parser->piscina);
                    redde res;
                }

                parser->cursor = crlf_pos + II;
                parser->linea_initium = parser->cursor;
                frange;

            casus HTTP_PARSE_CORPUS:
            {
                i32 corpus_disponibile;

                corpus_disponibile = parser->buffer_mensura - parser->cursor;

                si (corpus_disponibile >= parser->petitio->content_length)
                {
                    /* Habemus totum corpus */
                    parser->petitio->corpus = _chorda_ex_partibus(
                        (constans character*)&parser->buffer[parser->cursor],
                        parser->petitio->content_length,
                        parser->piscina);
                    parser->petitio->corpus_longitudo = parser->petitio->content_length;
                    parser->status = HTTP_PARSE_COMPLETA;
                }
                alioquin
                {
                    /* Necesse plus data */
                    res.petitio = parser->petitio;
                    redde res;
                }
                frange;
            }

            ordinarius:
                parser->status = HTTP_PARSE_COMPLETA;
                frange;
        }
    }

    /* Parsing completa */
    res.petitio = parser->petitio;
    res.completa = (parser->status == HTTP_PARSE_COMPLETA);

    redde res;
}

b32
http_parser_est_completa(HttpParser* parser)
{
    si (!parser)
    {
        redde FALSUM;
    }

    redde (parser->status == HTTP_PARSE_COMPLETA);
}

HttpPetitioServeri*
http_parser_obtinere_petitio(HttpParser* parser)
{
    si (!parser)
    {
        redde NIHIL;
    }

    redde parser->petitio;
}

vacuum
http_parser_reset(HttpParser* parser)
{
    si (!parser)
    {
        redde;
    }

    parser->status = HTTP_PARSE_LINEA_PETITIONIS;
    parser->buffer_mensura = 0;
    parser->cursor = 0;
    parser->linea_initium = 0;

    /* Reset petitio */
    parser->petitio->methodus = HTTP_GET;
    parser->petitio->uri.datum = NIHIL;
    parser->petitio->uri.mensura = 0;
    parser->petitio->via.datum = NIHIL;
    parser->petitio->via.mensura = 0;
    parser->petitio->quaestio.datum = NIHIL;
    parser->petitio->quaestio.mensura = 0;
    parser->petitio->versio = XI;
    parser->petitio->capita_numerus = 0;
    parser->petitio->corpus.datum = NIHIL;
    parser->petitio->corpus.mensura = 0;
    parser->petitio->corpus_longitudo = 0;
    parser->petitio->keep_alive = VERUM;
    parser->petitio->chunked = FALSUM;
    parser->petitio->content_length = 0;
}

HttpParseResultus
http_petitio_parse(
    constans character* datum,
    i32                 longitudo,
    Piscina*            piscina)
{
    HttpParser* parser;
    HttpParseResultus res;

    si (!datum || longitudo <= 0 || !piscina)
    {
        res.successus = FALSUM;
        res.completa = FALSUM;
        res.petitio = NIHIL;
        res.error = HTTP_ERROR_PARSE;
        res.error_descriptio.datum = NIHIL;
        res.error_descriptio.mensura = 0;
        redde res;
    }

    parser = http_parser_creare(piscina);
    si (!parser)
    {
        res.successus = FALSUM;
        res.completa = FALSUM;
        res.petitio = NIHIL;
        res.error = HTTP_ERROR_PARSE;
        res.error_descriptio = chorda_ex_literis("Parser creatio fallita", piscina);
        redde res;
    }

    redde http_parser_adicere(parser, datum, longitudo);
}
