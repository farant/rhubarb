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

#endif /* NEXUS_ORDINES_H */
