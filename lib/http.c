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
        casus HTTP_GET:    redde "GET";
        casus HTTP_POST:   redde "POST";
        casus HTTP_PUT:    redde "PUT";
        casus HTTP_DELETE: redde "DELETE";
        casus HTTP_PATCH:  redde "PATCH";
        casus HTTP_HEAD:   redde "HEAD";
        ordinarius:        redde "GET";
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
