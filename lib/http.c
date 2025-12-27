/* http.c - Implementatio HTTP Client
 *
 * Pure C89 HTTP/1.1 client.
 */

#include "http.h"
#include "tls.h"
#include "tcp.h"
#include "chorda_aedificator.h"

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

    /* Corpus */
    {
        constans character* body_start = headers_end + IV;  /* Post \r\n\r\n */
        i32 body_len = total_size - (i32)(body_start - (constans character*)total_data);

        si (body_len > 0)
        {
            resp->corpus = _chorda_ex_partibus(body_start, body_len, piscina);
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
        ordinarius:                 redde "Error ignotus";
    }
}
