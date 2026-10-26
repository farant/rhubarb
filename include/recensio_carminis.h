/* recensio_carminis.h - Carmen recensitum: summarium, censura, sonus
 * uno transitu
 *
 * Agens componens quattuor instrumenta vocabat (carmina, partitura,
 * censura, auscultatio - 'vesperae', 2026-10-23); recensio ea in
 * structuram UNAM colligit, et duo scriptores tenues eam reddunt:
 * textus (terminale, agens - pauca signa) et JSON (MCP:
 * structuredContent per mcp_respondere_json; claves Latinae, Franus
 * 10-23). Inventum
 * quodque in JSON 'textus' eundem fert ac linea censurae - formae duae
 * non divergunt.
 *
 * MONITA = inventa censurae + mensurae tacitae + partes tacitae (una
 * quaeque: pars notas habens quae sola reddita aliquibus mensuris
 * tacet, si mensuram habet quam tacitae mixturae totius non iam
 * nominant - idem vitium non bis; linea partis 'TACET 1-16',
 * '(tota: instrumentum?)' si sola tota tacet) + (tonsa ? 1 : 0) +
 * (limitator summae ultra -3 dB ? 1 : 0 - mixtura vere nimis
 * calida); auditio -recensio exitum 3 reddit si monita. Limitatio
 * levior (apices fortuiti) nominatur in linea soni, non monetur.
 *
 * Partitura non ordinaria (moles maxima): forma.partitura (et
 * .harmonia) eam addunt.
 *
 * USUS:
 *   RecensioForma     f;
 *   RecensioCarminis  r;
 *   recensio_carminis_ordinaria(&f);
 *   si (recensio_carminis_facere(carmen, &f, NIHIL, piscina, &r))
 *       mcp_respondere_json(vocatio,
 *           recensio_carminis_json(&r, piscina));
 */
/* <aedilis corpus="lib/recensio_carminis.c"/> */
#ifndef RECENSIO_CARMINIS_H
#define RECENSIO_CARMINIS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "json.h"
#include "carmen.h"
#include "cantor.h"
#include "censura.h"
#include "auscultatio.h"

nomen structura {
    CensuraForma censura;     /* regulae */
             b32 partitura;   /* partituram includere */
             b32 harmonia;    /* cum linea harmoniae (si partitura) */
} RecensioForma;

/* mensurae (1 = prima) ubi pars sola tacet quamvis notas habeat */
nomen structura {
    i32* mensurae;
    i32  numerus;
    b32  tota;        /* pars sola tota tacet (rms < TACITUM) */
} RecensioTacitae;

nomen structura {
           constans Carmen* carmen;
             RecensioForma  forma;
                       i32  mensurae;
                       f64  secundae;     /* carminis (sine cauda) */
           CensuraInventum* inventa;
                       i32  numerus_inventorum;
      AuscultatioSummarium  sonus;        /* mixtura tota (post
                                           * limitatorem) */
             MixturaCensus mixtura;      /* census redditionis: apex
                                           * ante limitatorem,
                                           * limitata, reductio */
      AuscultatioSummarium* partes;       /* pars sola quaeque */
                       i32* tacitae;      /* mensurae (1 = prima) */
                       i32  numerus_tacitarum;
           RecensioTacitae* tacitae_partium; /* per partem: mensurae
                                              * ubi sola tacet */
                    chorda partitura;    /* vacua nisi rogata */
} RecensioCarminis;

/* censura ordinaria, sine partitura */
vacuum
recensio_carminis_ordinaria (
    RecensioForma* forma);

/* omnia uno transitu (redditio offline + pars quaeque sola); forma
 * NIHIL = ordinaria, resolutor NIHIL = ordinarius cantoris. Omnia e
 * piscina. FALSUM: memoria deest */
b32
recensio_carminis_facere (
              constans Carmen* carmen,
       constans RecensioForma* forma,
     constans CantorResolutor* resolutor,
                      Piscina* piscina,
             RecensioCarminis* recensio);

/* inventa + tacitae + (tonsa ? 1 : 0) + (reductio < -3 dB ? 1 : 0) */
i32
recensio_carminis_monita (
    constans RecensioCarminis* recensio);

/* textus terminalis (linea prima: 'recensio T: clavis, metrum, tempus,
 * N mensurae, S s; monita M'). FALSUM: memoria deest */
b32
recensio_carminis_textus (
    constans RecensioCarminis* recensio,
            ChordaAedificator* textus);

/* JSON (claves Latinae; numeri ad millesimas, dBFS ad decimas); NIHIL
 * si memoria deest */
JsonValor*
recensio_carminis_json (
    constans RecensioCarminis* recensio,
                      Piscina* piscina);

#endif /* RECENSIO_CARMINIS_H */
