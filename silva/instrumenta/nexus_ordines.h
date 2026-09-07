/* nexus_ordines.h - ordines nexus ex analysi UNA (unitas communis;
 * LEGATUS v0.1b: geminus sepultus)
 *
 * Logica ordinum sedes/usus ex nexus_percursus levata: analysi data
 * (parsura + semantica), omnem sedem declarationis (symbola TU,
 * systemate excluso) et omnem sedem usus (folia identificatorum
 * resoluta, ambiguis per canonicam sequtis) receptori fundit.
 * Consumptores: nexus_percursus (sweep -> tsv, dedup + fprintf in
 * receptore SUO) et legatus (superpositio indicis vivi). QUIA unitas
 * una est, superpositio a sweep divergere non potest - lectio
 * praeparatoris ante vitium applicata.
 *
 * Ordo emissionis = ordo tabulae: symbola indice, deinde usus
 * AMBULATIONE LATA (series, non recursio) - paritas octetim cum
 * tsv historico pendet ab hoc ordine, noli in recursionem
 * "purgare".
 *
 * Mundus amalgamatis: silva.h solum (fontes numquam); machinula
 * hospitis (Piscina/Xar bibliothecae) pro serie effimera.
 *
 * C89 vanilla in capite - hospites amalgamata consumunt. */

#ifndef NEXUS_ORDINES_H
#define NEXUS_ORDINES_H

#include "piscina.h"
#include "silva.h"

/* receptor ordinis unius (species = "sedes" | "usus"; via CRUDA ut
 * a silva_fons_via - praefixum "./" ambulationis NON detractum,
 * consumptor tractat; linea/columna 1-basatae ex radice originis;
 * profunditas = scopus symboli, 0 = plagula) */
typedef void (*NexusOrdinesReceptor)(
    void*              datum,
    const SilvaChorda* titulus,
    const char*        species,
    const char*        genus,
    const SilvaChorda* via,
    unsigned int       linea,
    unsigned int       columna,
    unsigned int       profunditas);

/* Sedes + usus TU unius fundere. effimera = piscina vocantis pro
 * serie ambulationis (vita = haec vocatio). Systema exclusum;
 * impliciti exclusi; lexemata synthetica (byte_offset < 0) omissa;
 * viae sine '/' (copiae basename capitum praebitorum) omissae -
 * ambulatio directa .h ordinem canonicum via plena praebet.
 * Stadium tertium (LEGATUS v0.2): macra - sedes ex vista actorum
 * (genus "macro", columna I) + usus per radicem originis; usus
 * macrorum in latina.h definitorum exclusi (sedes manent). */
void
nexus_ordines_fundere (
      const SilvaParsura* parsura,
    const SilvaSemantica* sem,
                 Piscina* effimera,
    NexusOrdinesReceptor  receptor,
                    void* datum);

/* genus symboli -> titulus tabulae (variabile/functio/typedef/
 * constans/parametrum/?) */
const char*
nexus_ordines_genus_titulus (
    int genus);

/* titulus macronis EXTIMI (in fonte scripti) lexematis expansi:
 * catenam originis ascendit; NULL si lexema FONS purum aut catena
 * corrupta. Communis emissioni (usus macrorum) et legato
 * (resolutio positionis - hover/definitio in invocatione). */
const SilvaChorda*
nexus_ordines_titulus_macronis (
    SilvaToken* tok);

/* GRADUS PRAEBITIONIS CAPITUM (2026-09-07, quaestio 01M1TD1FMFT3):
 * capita eiusdem basename in expansore PRIMUS-VINCIT sedent, ergo
 * ordo praebitionis lex est, non fortuna. Gradus: 0 = include/
 * (capita publica domus), 1 = cetera, 2 = fixturae (membrum viae
 * 'fixa' integrum - corpora synthetica, saepe DECURTATA consulto).
 * Intra gradum ordo canonicus (strcmp). Olim strcmp solus:
 * 'include/' ante 'probationes/' fortuna litterarum, donec fixtura
 * briar 'briar/probationes/fixa/amalgama/fabrica/include/latina.h'
 * ante 'include/' cecidit et latina.h decurtatum (sine commutatio,
 * casus, NIHIL, numeris) omni TU praebuit: silva_formator.c XIX
 * errores, sedes definitionum VI perditae, tabula 1.10M -> 0.96M
 * ordinum. Communis sweep (nexus_percursus) et legato
 * (praeparator) - divergere non possunt. via radici-relativa
 * ("./" tolerata); NULL = gradus 1. */
unsigned int
nexus_ordines_capitis_gradus (
    const char* via);

/* comparator pro xar_ordinare super elementis 'const char*' (viae
 * radici-relativae): gradus, deinde strcmp */
int
nexus_ordines_capita_comparare (
    const void* a,
    const void* b);

#endif /* NEXUS_ORDINES_H */
