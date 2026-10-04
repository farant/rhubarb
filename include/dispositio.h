/* dispositio.h - Dispositio pura in CELLULIS (modulus 013, semita b;
 * project-specs/dispositio-plan.md)
 *
 * Exemplar Clay (../clay @ e6cc369; oraculum
 * tools/dispositio_oraculum.sh):
 * nodus directionem habet (linea | columna); per axem mensuram (APTA
 * contento, CRESCENS spatio reliquo, FIXA, PARS centesimis parentis)
 * cum minimo et maximo; spatium interius; intervallum inter liberos;
 * allineationem per axem; praecisionem per axem. Nihil de componente
 * scit: componere eam vocat ut fines impleat (ut tessellatio tesseram
 * nescit).
 *
 * INTEGRA, IN CELLULIS: fines in cellulis; pixela = cellulae x Modulus
 * in fine - alineatio ad cellulas PER CONSTRUCTIONEM (B6b).
 *
 * RADIX IMPLICITA (ut Clay): linea FIXA magnitudine superficiei, sine
 * spatio; nodi cum parente -1 eius liberi sunt.
 *
 * DIVERGENTIAE A CLAY NOMINATAE (numeri integri): medium = pavimentum
 * dimidii (etiam negativi: -3/2 -> -2); PARS = pavimentum
 * (spatium * centesimae / 100); divisio crescentium et contractio
 * (D2): ORA QUAEQUE pavimentum orae exactae (residuum aequaliter
 * sparsum, non primis datum - ea regula oras usque ad k/4 cellulas
 * promoveret). Proprietas Clay SERVATA: nodus vacuus
 * spatium transversum non habet (linea vacua "1 2 3 4" -> 3 x 0).
 *
 * Gradus: D1 apta, fixa, pars, positiones, allineatio, praecisio; D2
 * crescens et contractio secundum axem (aequatio Clay: minimi primum
 * crescunt, maximi primum contrahuntur; parens praecidens non
 * comprimit); textus (mensor) D3 - ante D3 textus neglegitur.
 *
 * PURUS: nulla allocatio praeter piscinam datam; idem ingressus, idem
 * exitus.
 */

#ifndef DISPOSITIO_H
#define DISPOSITIO_H

/* <aedilis corpus="lib/dispositio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "mandatum.h"   /* Fines */

nomen enumeratio {
    DISPOSITIO_LINEA = ZEPHYRUM,   /* liberi sinistrorsum */
    DISPOSITIO_COLUMNA             /* liberi deorsum */
} DispositioDirectio;

nomen enumeratio {
    DISPOSITIO_APTA = ZEPHYRUM,    /* contento (fit) */
    DISPOSITIO_CRESCENS,           /* spatio reliquo (grow) */
    DISPOSITIO_FIXA,               /* valor cellularum */
    DISPOSITIO_PARS                /* valor centesimarum parentis */
} DispositioGenus;

nomen enumeratio {
    DISPOSITIO_INITIUM = ZEPHYRUM,
    DISPOSITIO_MEDIUM,
    DISPOSITIO_FINIS
} DispositioAllineatio;

nomen structura {
    DispositioGenus genus;
                s32 valor;      /* FIXA: cellulae; PARS: centesimae */
                s32 minimum;    /* APTA, CRESCENS */
                s32 maximum;    /* APTA, CRESCENS; 0 = sine fine */
} DispositioMensura;

nomen structura {
       DispositioMensura latitudo;
       DispositioMensura altitudo;
      DispositioDirectio directio;
                     s32 spatium_sinistrum;
                     s32 spatium_dextrum;
                     s32 spatium_superum;
                     s32 spatium_inferum;
                     s32 intervallum;
    DispositioAllineatio allineatio_x;
    DispositioAllineatio allineatio_y;
                     b32 praecidere_x;
                     b32 praecidere_y;
                  chorda textus;     /* D3: mensura per mensorem */
} DispositioForma;

/* Latitudo textus in cellulis (scopus mensurat: runa lata II in
 * terminali, glyphus I in fenestra). D3. */
nomen s32 (*DispositioMensor)(
    chorda  textus,
    vacuum* ctx);

nomen structura Dispositio Dispositio;

Dispositio*
dispositio_creare (
    Piscina* piscina);

/* Nodos omnes tollere (memoria piscinae servatur ad quadrum
 * proximum) */
vacuum
dispositio_vacare (
    Dispositio* d);

/* Forma ordinaria: apta x apta, linea, spatia et intervallum 0,
 * initium, sine praecisione, textus vacuus */
vacuum
dispositio_formam_initiare (
    DispositioForma* forma);

/* Nodum sub parente (-1 = radix implicita) addere; liberi ordine
 * additionis. Redde indicem (0, 1, ... ordine additionis) aut -1 si
 * parens ignotus aut argumentum NIHIL. */
s32
dispositio_addere (
                  Dispositio* d,
                         s32  parens,
    constans DispositioForma* forma);

/* Disponere in superficie latitudo x altitudo (cellulae). mensor NIHIL
 * licet ante D3. */
vacuum
dispositio_computare (
          Dispositio* d,
                 s32  latitudo,
                 s32  altitudo,
    DispositioMensor  mensor,
              vacuum* ctx);

/* Fines nodi (cellulae, coordinatae absolutae superficiei); index
 * ignotus: fines nulli */
Fines
dispositio_fines (
    constans Dispositio* d,
                    s32  index);

s32
dispositio_numerus (
    constans Dispositio* d);

#endif /* DISPOSITIO_H */
