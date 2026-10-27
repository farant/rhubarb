/* navigator_petitio.h - articulum petere et servare (navigator N2.5).
 * URL quaevis -> situs albus (v1: Vicipaedia, vicipaedia_sedem_legere)
 * -> URL fontis -> http_vocare in reactore vocantis (User-Agent,
 * Accept Parsoid, redirectiones V) -> titulus ex URL ULTIMA ->
 * URL canonica (redirectio "Aquinas" -> "Thomas_Aquinas" articulum
 * EUNDEM dat) -> navigator_versionem_offerre. Nihil obstat; finis
 * SEMEL, numquam intra navigator_petere. Petitio piscinam propriam
 * habet (corpus HTML magnum): dimittere post finem, numquam intra. */

#ifndef NAVIGATOR_PETITIO_H
#define NAVIGATOR_PETITIO_H

/* <aedilis corpus="lib/navigator_petitio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "http_vocatio.h"
#include "navigator_bibliotheca.h"

#define NAVIGATOR_AGENS_ORDINARIUS \
    "rhubarb-navigator/0.1 (personal reader)"

/* User-Agent: linea prima fili (spatia finalia abscissa); filum
 * absens aut vacuum -> NAVIGATOR_AGENS_ORDINARIUS. via NIHIL =
 * ~/.rhubarb/navigator/agens */
chorda
navigator_agens_legere (
    constans character* via,
               Piscina* piscina);

nomen enumeratio {
    NAVIGATOR_PETITIO_SERVATA = ZEPHYRUM, /* exitus bibliothecae
                                           * datus */
    NAVIGATOR_PETITIO_IGNOTA,      /* URL extra situs albos */
    NAVIGATOR_PETITIO_NON_INVENTA, /* 404 */
    NAVIGATOR_PETITIO_RETE,        /* rete, HTTP, status alius,
                                    * extractio */
    NAVIGATOR_PETITIO_ABRUPTA
} NavigatorPetitioStatus;

nomen structura {
          NavigatorPetitioStatus  status;
                 NavigatorExitus  exitus;     /* si SERVATA */
     constans NavigatorArticulus* articulus;  /* si SERVATA */
        constans NavigatorVersio* versio;     /* nova, aut ultima si
                                               * IDEM */
                          chorda descriptio; /* causa legibilis */
} NavigatorPetitioResultus;

nomen structura NavigatorPetitio NavigatorPetitio;

nomen vacuum (*NavigatorPetitioFinis)(vacuum* datum,
    constans NavigatorPetitioResultus* resultus);

/* NIHIL aut memset ZEPHYRUM = ordinariae */
nomen structura {
                          chorda  agens;  /* vacua = ordinarius */
    constans HttpVocatioOptiones* http;  /* NIHIL = ordinariae;
                                          * redirectiones V nisi
                                          * datae */
    /* PROBATIONES: radix URL fontis loco "https://<hospes>" (servus
     * scriptus, e.g. "http://127.0.0.1:4711"); NIHIL = verum */
            constans character* radix_fontis;
} NavigatorPetitioOptiones;

/* NIHIL solum si argumenta NIHIL aut memoria deficit */
NavigatorPetitio*
navigator_petere (
                              Reactor* reactor,
                 NavigatorBibliotheca* bibliotheca,
                               chorda  url,
                NavigatorPetitioFinis  finis,
                               vacuum* datum,
    constans NavigatorPetitioOptiones* optiones);

/* idempotens, synchronus: finis (ABRUPTA) ANTE reditum */
vacuum
navigator_petitionem_abrumpere (
    NavigatorPetitio* petitio);

/* post finem (pulsu proximo): piscina petitionis destruitur */
vacuum
navigator_petitionem_dimittere (
    NavigatorPetitio* petitio);

#endif /* NAVIGATOR_PETITIO_H */
