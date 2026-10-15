/* http.h - Bibliotheca HTTP Client
 *
 * HTTP/1.1 client pro petitionibus HTTPS.
 * Pure C89, utitur tls.h pro connexionibus securis.
 *
 * USUS:
 *   HttpPetitio* pet = http_petitio_creare(piscina, HTTP_GET, "https://api.example.com/data");
 *   http_petitio_caput_addere(pet, "Authorization", "Bearer token123");
 *   HttpResultus res = http_exsequi(pet, piscina);
 *   si (res.successus) {
 *       printf("Status: %d\n", res.responsum->status);
 *       printf("Corpus: %.*s\n", res.responsum->corpus.mensura, res.responsum->corpus.datum);
 *   }
 */

#ifndef HTTP_H
#define HTTP_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fasti.h"   /* MoraAngusta: duratio ms (stirps Morae) */


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* HTTP methodi */
nomen enumeratio {
    HTTP_GET = 0,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_METHODUS_IGNOTUS
} HttpMethodus;

/* HTTP error codes */
nomen enumeratio {
    HTTP_OK = 0,
    HTTP_ERROR_URL,          /* URL invalida */
    HTTP_ERROR_CONNEXIO,     /* Connexio fallita */
    HTTP_ERROR_TIMEOUT,      /* Timeout */
    HTTP_ERROR_PARSE,        /* Parse error */
    HTTP_ERROR_IO,           /* I/O error */
    HTTP_ERROR_REDIRECTIO    /* Nimis redirectiones */
} HttpError;

/* Caput (header) */
nomen structura {
    chorda titulus;
    chorda valor;
} HttpCaput;

/* Petitio (request) - opaque */
nomen structura HttpPetitio HttpPetitio;

/* Responsum (response) */
nomen structura {
       i32 status;               /* 200, 404, etc. */
    chorda status_descriptio;    /* "OK", "Not Found", etc. */
    chorda corpus;               /* Response body */

    /* Capita (headers) - pro accessu interno */
    HttpCaput* capita;
          i32  capita_numerus;
} HttpResponsum;

/* Resultus */
nomen structura {
              b32  successus;
    HttpResponsum* responsum;
        HttpError  error;
           chorda  error_descriptio;
} HttpResultus;


/* ========================================================================
 * LECTOR RESPONSI - incrementalis (quaestio-rivus-spec II.1)
 * ========================================================================
 * Octeti intrant, eventa exeunt; nullum I/O. Corpus decodificatum
 * (Content-Length, chunked, aut usque ad clausuram) per rivum exit SOLUM
 * si status 2xx; corpus TOTUM semper colligitur (responsum->corpus,
 * limes HTTP_LECTOR_CORPUS_MAXIMUM). Fines octetorum ubicumque cadere
 * possunt. Octeti post FINITUM neglecti. */

nomen vacuum (*HttpRivus)(vacuum* datum, constans i8* octeti, i32 n);

nomen enumeratio {
    HTTP_LECTOR_CAPITA = 0,   /* capita nondum integra */
    HTTP_LECTOR_CORPUS,       /* status notus, corpus currit */
    HTTP_LECTOR_FINITUS,
    HTTP_LECTOR_ERROR
} HttpLectorStatus;

nomen structura HttpLector HttpLector;   /* opacus */

#define HTTP_LECTOR_CAPITA_MAXIMA   (LXIV * M)        /* octeti capitum */
#define HTTP_LECTOR_CORPUS_MAXIMUM  (XVI * M * M)     /* octeti corporis */

/* methodus HTTP_HEAD: nullum corpus. rivus NIHIL = colligere solum */
HttpLector*
http_lector_creare (
         Piscina* piscina,
    HttpMethodus  methodus,
       HttpRivus  rivus,
          vacuum* rivi_datum);

HttpLectorStatus
http_lector_adicere (
      HttpLector* lector,
     constans i8* octeti,
             i32  n);

/* EOF connexionis: corpus usque ad clausuram FINITUM; ceterum nondum
 * finitum -> ERROR "truncatum" */
HttpLectorStatus
http_lector_finire (
    HttpLector* lector);

HttpLectorStatus
http_lector_status (
    constans HttpLector* lector);

/* NIHIL ante HTTP_LECTOR_CORPUS; corpus = collectum hactenus */
HttpResponsum*
http_lector_responsum (
    HttpLector* lector);

/* causa si ERROR, aliter vacua */
chorda
http_lector_error (
    constans HttpLector* lector);


/* ========================================================================
 * TYPI - SERVER (REQUEST PARSING)
 * ======================================================================== */

/* Status parsing */
nomen enumeratio {
    HTTP_PARSE_LINEA_PETITIONIS = 0,  /* Parsing request line */
    HTTP_PARSE_CAPITA,                 /* Parsing headers */
    HTTP_PARSE_CORPUS,                 /* Parsing body */
    HTTP_PARSE_COMPLETA,               /* Parsing complete */
    HTTP_PARSE_ERROR                   /* Parse error */
} HttpParseStatus;

/* Petitio serveri (parsed incoming request) */
nomen structura {
    HttpMethodus methodus;
          chorda uri;              /* Full URI "/path?query" */
          chorda via;              /* Path only "/path" */
          chorda quaestio;         /* Query string (after ?) */
             i32 versio;           /* 10 = HTTP/1.0, 11 = HTTP/1.1 */

    HttpCaput* capita;           /* Array of headers */
          i32  capita_numerus;

    chorda corpus;           /* Request body */
       i32 corpus_longitudo; /* Body length */

    b32 keep_alive;       /* Connection: keep-alive */
    b32 chunked;          /* Transfer-Encoding: chunked */
    i32 content_length;   /* Content-Length header value */
} HttpPetitioServeri;

/* Parser incrementalis (opaque) */
nomen structura HttpParser HttpParser;

/* Limites parseris (0 in creare_cum_limitibus = defalta) */
#define HTTP_PETITIO_MAXIMA_DEFALTA  (M * M)      /* 1MB petitio tota */
#define HTTP_URI_MAXIMA_DEFALTA      (VIII * M)   /* 8KB URI */

/* Resultus parse */
nomen structura {
                   b32  successus;
                   b32  completa;      /* FALSUM si necesse plus data */
    HttpPetitioServeri* petitio;
             HttpError  error;
                chorda  error_descriptio;
                   i32  status_suggestus;  /* Status HTTP mittendus (0 = nullus) */
} HttpParseResultus;


/* ========================================================================
 * FUNCTIONES - PETITIO
 * ======================================================================== */

/* Creare novam petitio
 *
 * methodus: HTTP_GET, HTTP_POST, etc.
 * url:      URL completa (e.g., "https://api.example.com/path")
 *
 * Redde: HttpPetitio* vel NIHIL si error
 */
HttpPetitio*
http_petitio_creare (
               Piscina* piscina,
          HttpMethodus  methodus,
    constans character* url);

/* Addere caput (header) ad petitio */
vacuum
http_petitio_caput_addere (
           HttpPetitio* petitio,
    constans character* titulus,
    constans character* valor);

/* Ponere tempus receptionis/connexionis (millisecunda; 0 = defaltum
 * XXX secundorum). Valet in semita plana ET https (vates-plan-1 T2:
 * olim https ordinarium tls XXX s semper habebat). Tempore icto
 * http_exsequi HTTP_ERROR_TIMEOUT reddit, corpore partiali consulto
 * abiecto. */
vacuum
http_petitio_tempus_ponere (
    HttpPetitio* petitio,
    MoraAngusta  tempus_ms);

/* Ponere corpus (body) petitionis */
vacuum
http_petitio_corpus_ponere (
           HttpPetitio* petitio,
    constans character* corpus,
                   i32  mensura);

/* Ponere corpus ex chorda */
vacuum
http_petitio_corpus_ponere_chorda (
    HttpPetitio* petitio,
         chorda  corpus);

/* --- destinatio et octeti petitionis (http-vocatio II.4: vectura extra
 * http.c, structura opaca) --- */

/* hospes NUL-terminatus in piscina petitionis */
constans character*
http_petitio_hospes (
    HttpPetitio* petitio);

i32
http_petitio_portus (
    constans HttpPetitio* petitio);

b32
http_petitio_est_https (
    constans HttpPetitio* petitio);

MoraAngusta
http_petitio_tempus (
    constans HttpPetitio* petitio);

HttpMethodus
http_petitio_methodus (
    constans HttpPetitio* petitio);

/* petitio tota serializata (linea, capita, corpus) */
chorda
http_petitio_octeti (
    HttpPetitio* petitio);


/* ========================================================================
 * FUNCTIONES - EXSEQUI
 * ======================================================================== */

/* Exsequi petitio et recipere responsum
 *
 * petitio: Petitio praeparata
 * piscina: Arena pro responsum
 *
 * Redde: Resultus cum responsum vel error
 */
HttpResultus
http_exsequi (
    HttpPetitio* petitio,
        Piscina* piscina);

/* Exsequi petitio cum sequendo redirectiones (301, 302, 307, 308)
 *
 * petitio:           Petitio praeparata
 * piscina:           Arena pro responsum
 * max_redirectiones: Maximum redirectiones sequendae (typice V vel X)
 *
 * Redde: Resultus cum responsum finale vel error
 */
HttpResultus
http_exsequi_cum_redirectionibus (
    HttpPetitio* petitio,
        Piscina* piscina,
            i32  max_redirectiones);


/* ========================================================================
 * VECTURA - sutura transportus (herbarium-spec par. V)
 *
 * Functio quae petitionem exsequitur + datum eius. Ordinaria =
 * http_exsequi. Involucra (herbarium) vecturam interiorem in datum
 * tenent; probationes vecturam scriptam inserunt.
 * ======================================================================== */

nomen structura {
    HttpResultus (*exsequi)(HttpPetitio* petitio, Piscina* piscina,
                            vacuum* datum);
    vacuum* datum;
} HttpVectura;

/* Exsequi cum rivo: ut http_exsequi, sed corpus 2xx per rivum exit dum
 * advenit (quaestio-rivus-spec II.2). responsum->corpus TOTUM quoque
 * redditur (R6). Lectio finitur cum responsum finitum est (connexio
 * non exspectatur claudi). */
HttpResultus
http_exsequi_rivus (
    HttpPetitio* petitio,
      HttpRivus  rivus,
         vacuum* rivi_datum,
        Piscina* piscina);

nomen structura {
    HttpResultus (*exsequi)(HttpPetitio* petitio, HttpRivus rivus,
                            vacuum* rivi_datum, Piscina* piscina,
                            vacuum* datum);
    vacuum* datum;
} HttpVecturaRivi;

/* Vectura rivi ordinaria: http_exsequi_rivus, datum NIHIL. */
HttpVecturaRivi
http_vectura_rivi_ordinaria (vacuum);

/* exsequi NIHIL -> HTTP_ERROR_CONNEXIO nominatus, numquam ruina */
HttpResultus
http_vectura_rivi_exsequi (
    HttpVecturaRivi  vectura,
        HttpPetitio* petitio,
          HttpRivus  rivus,
             vacuum* rivi_datum,
            Piscina* piscina);

/* Vectura ordinaria: http_exsequi, datum NIHIL. */
HttpVectura
http_vectura_ordinaria (vacuum);

/* Exsequi per vecturam. exsequi NIHIL -> HTTP_ERROR_CONNEXIO
 * nominatus, numquam ruina. */
HttpResultus
http_vectura_exsequi (
    HttpVectura  vectura,
    HttpPetitio* petitio,
        Piscina* piscina);

/* Visus lectionis petitionis - petitio opaca manet. Campi ut dati
 * sunt; capita in petitione ipsa iacent (non mutanda). NIHIL ->
 * visus vacuus (capita NIHIL, numeri 0). */
nomen structura {
          HttpMethodus  methodus;
                chorda  schema;
                chorda  hospes;
                   i32  portus;
                chorda  via;
                chorda  corpus;
    constans HttpCaput* capita;
                   i32  capita_numerus;
           MoraAngusta  tempus_ms;
} HttpPetitioVisus;

HttpPetitioVisus
http_petitio_visus (
    constans HttpPetitio* petitio);


/* ========================================================================
 * FUNCTIONES - RESPONSUM
 * ======================================================================== */

/* Obtinere caput (header) ex responsum
 *
 * Redde: Valor capitis vel chorda vacua si non invenitur
 */
chorda
http_responsum_caput (
         HttpResponsum* responsum,
    constans character* titulus);

/* Obtinere descriptio status (e.g., 200 -> "OK")
 *
 * status: HTTP status code (CC, CDIV, etc.)
 *
 * Redde: String descriptio status
 */
constans character*
http_status_descriptio (
    i32 status);

/* Serialize responsum ad HTTP response string
 *
 * responsum: Responsum cum status, capita, corpus
 * piscina:   Arena pro allocationibus
 *
 * Redde: Chorda cum "HTTP/1.1 200 OK\r\n..."
 */
chorda
http_responsum_serialize (
    HttpResponsum* responsum,
          Piscina* piscina);

/* Serialize cum capite Connection explicito (pro servo)
 *
 * Content-Length SEMPER emittitur (etiam 0) nisi status corpus
 * prohibet (1xx/204/304); capita Content-Length/Connection a vocatore
 * data saltantur - serializator framing possidet.
 *
 * keep_alive: VERUM = "Connection: keep-alive", FALSUM = "close"
 */
chorda
http_responsum_serialize_cum_conexione (
    HttpResponsum* responsum,
              b32  keep_alive,
          Piscina* piscina);


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

/* Obtinere nomen methodi */
constans character*
http_methodus_nomen (
    HttpMethodus methodus);

/* Obtinere error descriptio */
constans character*
http_error_descriptio (
    HttpError error);


/* ========================================================================
 * FUNCTIONES - SERVER (REQUEST PARSING)
 * ======================================================================== */

/* Parse petitio uno ictu (si habes totam petitionem)
 *
 * datum:     Data petitionis
 * longitudo: Longitudo datorum
 * piscina:   Arena pro allocationibus
 *
 * Redde: Resultus cum petitio vel error
 */
HttpParseResultus
http_petitio_parse (
    constans character* datum,
                   i32  longitudo,
               Piscina* piscina);

/* Creare parser incrementalis
 *
 * piscina: Arena pro allocationibus
 *
 * Redde: Parser vel NIHIL si error
 */
HttpParser*
http_parser_creare (
    Piscina* piscina);

/* Creare parser cum limitibus (0 = defalta)
 *
 * petitio_maxima: Limes totalis petitionis in bytes (413 si excessum)
 * uri_maxima:     Limes URI in bytes (414 si excessum)
 */
HttpParser*
http_parser_creare_cum_limitibus (
    Piscina* piscina,
        i32  petitio_maxima,
        i32  uri_maxima);

/* Bytes in buffer post finem petitionis completae (pipelining detectio)
 *
 * Redde: 0 nisi COMPLETA; aliter numerus byteorum reliquorum
 */
i32
http_parser_reliquiae (
    HttpParser* parser);

/* Adicere data ad parser
 *
 * parser:    Parser
 * datum:     Nova data
 * longitudo: Longitudo datorum
 *
 * Redde: Resultus (successus = parse valida, completa = petitio completa)
 */
HttpParseResultus
http_parser_adicere (
            HttpParser* parser,
    constans character* datum,
                   i32  longitudo);

/* Verificare si parsing completa */
b32
http_parser_est_completa (
    HttpParser* parser);

/* Obtinere petitio parsata (post completa) */
HttpPetitioServeri*
http_parser_obtinere_petitio (
    HttpParser* parser);

/* Reset parser pro nova petitio */
vacuum
http_parser_reset (
    HttpParser* parser);

/* Convertere methodus string ad enum */
HttpMethodus
http_methodus_ex_literis (
    constans character* methodus);


#endif /* HTTP_H */
