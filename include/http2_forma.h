/* http2_forma.h - formae HTTP/2 (RFC 9113 4, 6; http2-spec III.2)
 *
 * PURA: codificatio et decodificatio formarum (caput IX octetorum +
 * onus) et validatio 6. Codex iudicat, non agit: connexio (http2.h)
 * de RST_STREAM aut GOAWAY decernit. */
#ifndef HTTP2_FORMA_H
#define HTTP2_FORMA_H

#include "latina.h"

#define HTTP2_CAPUT_FORMAE       IX
#define HTTP2_PRAEFATIO          "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"
#define HTTP2_PRAEFATIO_MENSURA  XXIV
#define HTTP2_FORMA_MINIMA       (MMXLVIII * VIII)    /* 16384 */
#define HTTP2_FORMA_SUPREMA      0xffffff
#define HTTP2_FENESTRA_SUPREMA   0x7fffffff

/* genera (6.1-6.10) */
#define HTTP2_DATA          ZEPHYRUM
#define HTTP2_CAPITA        I          /* HEADERS */
#define HTTP2_PRIORITAS     II         /* PRIORITY (neglecta) */
#define HTTP2_ABROGATIO     III        /* RST_STREAM */
#define HTTP2_POSITIONES    IV         /* SETTINGS */
#define HTTP2_PROMISSIO     V          /* PUSH_PROMISE */
#define HTTP2_PULSUS        VI         /* PING */
#define HTTP2_DISCESSUS     VII        /* GOAWAY */
#define HTTP2_FENESTRA      VIII       /* WINDOW_UPDATE */
#define HTTP2_CONTINUATIO   IX         /* CONTINUATION */

/* vexilla */
#define HTTP2_FINIS_RIVI      0x01     /* END_STREAM */
#define HTTP2_RECEPTUM        0x01     /* ACK (SETTINGS, PING) */
#define HTTP2_FINIS_CAPITUM   0x04     /* END_HEADERS */
#define HTTP2_IMPLETUM        0x08     /* PADDED */
#define HTTP2_CUM_PRIORITATE  0x20     /* PRIORITY (in HEADERS) */

/* positiones (6.5.2) */
#define HTTP2_POSITIO_TABULA     I     /* HEADER_TABLE_SIZE */
#define HTTP2_POSITIO_IMPULSUS   II    /* ENABLE_PUSH */
#define HTTP2_POSITIO_RIVI       III   /* MAX_CONCURRENT_STREAMS */
#define HTTP2_POSITIO_FENESTRA   IV    /* INITIAL_WINDOW_SIZE */
#define HTTP2_POSITIO_FORMA      V     /* MAX_FRAME_SIZE */
#define HTTP2_POSITIO_CAPITA     VI    /* MAX_HEADER_LIST_SIZE */

/* codices errorum (7), ordine RFC */
nomen enumeratio {
    HTTP2_ERROR_NULLUS = ZEPHYRUM,   /* NO_ERROR */
    HTTP2_ERROR_PROTOCOLLI,          /* PROTOCOL_ERROR */
    HTTP2_ERROR_INTERNUS,            /* INTERNAL_ERROR */
    HTTP2_ERROR_FLUXUS,              /* FLOW_CONTROL_ERROR */
    HTTP2_ERROR_TEMPUS,              /* SETTINGS_TIMEOUT */
    HTTP2_ERROR_RIVUS_CLAUSUS,       /* STREAM_CLOSED */
    HTTP2_ERROR_MAGNITUDO,           /* FRAME_SIZE_ERROR */
    HTTP2_ERROR_RECUSATUM,           /* REFUSED_STREAM */
    HTTP2_ERROR_ABROGATUM,           /* CANCEL */
    HTTP2_ERROR_COMPRESSIO,          /* COMPRESSION_ERROR */
    HTTP2_ERROR_CONNEXIO,            /* CONNECT_ERROR */
    HTTP2_ERROR_MODERATIO,           /* ENHANCE_YOUR_CALM */
    HTTP2_ERROR_SECURITAS,           /* INADEQUATE_SECURITY */
    HTTP2_ERROR_HTTP_1_1             /* HTTP_1_1_REQUIRED */
} Http2Error;

/* forma lecta: contentum = onus sine impletione et sine campis
 * prioritatis (HEADERS); visum in datum vocantis */
nomen structura {
             i32  genus;
             i32  vexilla;
             i32  rivus;
     constans i8* contentum;
             i32  contentum_n;
} Http2Forma;

/* bona FALSUM: codex (Http2Error); connexionis VERUM = connexio tota
 * (GOAWAY), FALSUM = rivus solus (RST_STREAM in forma.rivus) */
nomen structura {
    b32 bona;
    i32 codex;
    b32 connexionis;
} Http2Iudicium;

/* forma una ex datum. Reddit octetos consumptos; 0 = nondum tota.
 * Forma supra 'forma_maxima' (SETTINGS_MAX_FRAME_SIZE nostrum):
 * HTTP2_CAPUT_FORMAE consumpti, FRAME_SIZE_ERROR connexionis (connexio
 * moritur, onus non exspectatur). Aliter forma mala longitudinem suam
 * reddit cum iudicium.bona FALSUM. Genera ignota: bona, praetereunda. */
i32
http2_formam_legere (
      constans i8* datum,
              i32  n,
              i32  forma_maxima,
       Http2Forma* forma,
    Http2Iudicium* iudicium);

/* caput + contentum in exitus (sine impletione); bitum reservatum
 * purgatum. Reddit IX + n, 0 si capacitas deficit aut n supra
 * 0xffffff. */
i32
http2_formam_scribere (
             i32  genus,
             i32  vexilla,
             i32  rivus,
     constans i8* contentum,
             i32  n,
              i8* exitus,
             i32  capacitas);

/* integrum XXXII bitorum ordine retis */
i32
http2_u32_legere (
    constans i8* datum);

vacuum
http2_u32_scribere (
    i32  valor,
     i8* exitus);

/* positio una (VI octeti: id XVI bitorum, valor XXXII) */
i32
http2_positionem_scribere (
    i32  id,
    i32  valor,
     i8* exitus);

/* positio k-ta formae SETTINGS; FALSUM extra */
b32
http2_positionem_legere (
    constans Http2Forma* forma,
                    i32  k,
                    i32* id,
                    i32* valor);

#endif /* HTTP2_FORMA_H */
