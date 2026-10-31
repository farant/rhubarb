/* http2.h - connexio HTTP/2 SINE IO (RFC 9113; http2-spec III.3)
 *
 * Octeti intrant (http2_recipere), octeti exeunt (http2_mittenda) et
 * eventus prodeunt (http2_eventum_capere). Nullus socket, nullum
 * horologium: glutinum (http2_nexus) pulsat.
 *
 * Munus (D1): connexio munus accipit ab initio; solum CLIENS hoc gradu
 * aedificatur - SERVUS NIHIL reddit.
 *
 * Memoria: connexio diu vivit, piscina numquam liberat - ergo
 * tamponi fixi, piscina parva per rivum (deleta cum rivus clauditur),
 * et scheda eventuum quae vacatur cum omnes eventus capti sunt. */
#ifndef HTTP2_H
#define HTTP2_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "hpack.h"

nomen enumeratio {
    HTTP2_CLIENS = ZEPHYRUM,
    HTTP2_SERVUS
} Http2Munus;

/* positiones nostrae (NIHIL aut campus 0 = ordinaria, http2-plan C4):
 * tabula_capitum 4096, fenestra_initialis 1 MiB, forma_maxima 16384,
 * capita_maxima 65536, continuationes_maximae VIII,
 * octeti_capitum_maximi 65536 */
nomen structura {
    i32 tabula_capitum;
    i32 fenestra_initialis;
    i32 forma_maxima;
    i32 capita_maxima;
    i32 continuationes_maximae;
    i32 octeti_capitum_maximi;
} Http2Positiones;

nomen enumeratio {
    HTTP2_EVENTUS_CAPITA = ZEPHYRUM,  /* capita responsi (aut 1xx,
                                       * aut appendices) */
    HTTP2_EVENTUS_DATA,
    HTTP2_EVENTUS_FINIS,              /* rivus a servo finitus */
    HTTP2_EVENTUS_ABROGATUS,          /* RST_STREAM (codex) */
    HTTP2_EVENTUS_DISCESSUS,          /* GOAWAY: rivus > ultimum
                                       * (retentabilis), aut rivus 0
                                       * (connexio: codex, ultimus) */
    HTTP2_EVENTUS_PULSUS,             /* PING noster receptus (data) */
    HTTP2_EVENTUS_ERROR               /* connexio mortua (codex) */
} Http2EventusGenus;

/* capita et data in piscina vocantis (http2_eventum_capere) */
nomen structura {
    Http2EventusGenus  genus;
                  i32  rivus;
                  Xar* capita;        /* HpackCaput */
                  b32  informativa;   /* 1xx */
               chorda  data;
                  i32  codex;
                  i32  rivus_ultimus;
} Http2Eventus;

nomen structura Http2Connexio Http2Connexio;

/* CLIENS: praefatio + SETTINGS + WINDOW_UPDATE (C4) in exitu parata.
 * SERVUS: NIHIL (hoc gradu). */
Http2Connexio*
http2_connexionem_creare (
                  Http2Munus  munus,
    constans Http2Positiones* positiones,
                     Piscina* piscina);

/* octeti a pari; FALSUM si connexio mortua (aut nunc moritur: GOAWAY
 * in exitu, eventus ERROR) */
b32
http2_recipere (
    Http2Connexio* c,
      constans i8* datum,
              i32  n);

/* octeti mittendi (visum; valet usque ad consumptionem) */
i32
http2_mittenda (
    Http2Connexio*  c,
      constans i8** datum);

vacuum
http2_mittenda_consumere (
    Http2Connexio* c,
              i32  n);

/* eventus proximus, capita et data in piscina copiata; FALSUM si
 * nullus (scheda interna tum vacatur) */
b32
http2_eventum_capere (
    Http2Connexio* c,
     Http2Eventus* eventus,
          Piscina* piscina);

/* petitio: capita (primum :method, :scheme, :authority, :path) et
 * corpus (vacuum licet). Reddit rivum (impar), 0 si recusata (connexio
 * mortua, post GOAWAY, pseudo-capita desunt). Supra
 * MAX_CONCURRENT_STREAMS paris petitio exspectat (C3). */
i32
http2_petere (
          Http2Connexio* c,
    constans HpackCaput* capita,
                    i32  numerus,
                 chorda  corpus);

/* RST_STREAM CANCEL; rivus oblivioni datur */
b32
http2_rivum_abrogare (
    Http2Connexio* c,
              i32  rivus);

/* PING cum VIII octetis; eventus PULSUS cum receptum */
b32
http2_pulsare (
    Http2Connexio* c,
      constans i8* octo);

/* GOAWAY cum codice; connexio non iam petitiones accipit */
vacuum
http2_discedere (
    Http2Connexio* c,
              i32  codex);

b32
http2_viva (
    constans Http2Connexio* c);

#endif /* HTTP2_H */
