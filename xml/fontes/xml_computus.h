/* xml_computus.h - imago memoriae et temporis clientis xml
 * (gemellus toml_computus.h; planum xml-arbor P14)
 *
 * Una plagula, una mensura: parsura (xml_arbor) in piscina PROPRIA cum
 * alveo initiali fixo (numeri inter plagulas et commissiones
 * conferibiles); deinde in piscina altera emissio octetorum, scriptura
 * et lectio STML, comparator arborum; COCTIO (xml_coquere: DTD,
 * entitates, attributa, spatia, diagnostica) in piscina TERTIA
 * (usus_coquendi) - numeri parsurae puri manent. Tempus per clock()
 * (CPU), ms. Sumptus per octetum = (usus + usus_coquendi) / fons (P5:
 * arbor XIII-XXXVI x, douay-rheims LX x).
 *
 * Instrumentum: ./xml/computus.sh <plagula.xml> [-machina] [-iter N].
 * Porta: probatio_xml_computus (casus inlinei tres, aurum
 * xml/probationes/fixa/computus/basis.tsv - numeri deterministici
 * pinnati, tempora non; COMPUTUS_SCRIBERE=1 + causa nominata).
 */
#ifndef XML_COMPUTUS_H
#define XML_COMPUTUS_H

#include "latina.h"
#include "piscina.h"

#define XML_COMPUTUS_ALVEUS_INITIUM \
    ((memoriae_index)(IV * MXXIV * CCLVI))

nomen structura {
               i32 octeti_fontis;
               i32 nodi;                  /* radix inclusa */
               i32 lexemata;              /* loci + listae */
               i32 octeti_stml;           /* documentum scriptum */
               i32 diagnostica;           /* post coctionem */
               b32 successus;             /* octeti et arbor idem */
    memoriae_index usus;                  /* piscina parsurae: dati */
    memoriae_index commissa;              /* usus + otiosa */
    memoriae_index otiosa;
    memoriae_index apex;
    memoriae_index alvei;
    memoriae_index allocationes;
    memoriae_index allocationes_arboris;  /* piscina altera */
               f64 ms_parsandi;
               f64 ms_emittendi;          /* materia_scribere_nodum */
               f64 ms_arbor_scribendi;    /* arbor_scribere_nodum */
               f64 ms_arbor_legendi;      /* materia_arbor_legere */
               f64 ms_comparandi;         /* materia_arbor_aequalis */
               f64 ms_coquendi;           /* xml_coquere */
    memoriae_index usus_coquendi;         /* piscina coctionis: dati */
} XmlComputus;

/* VERUM si parsura successit ET octeti idem ET arbor relecta aequalis
 * (plagula invalida quoque: lex octetorum omnem fontem tenet); numeri
 * semper impleti (usque ad gradum quo perventum est). */
b32
xml_computus_metiri (
       constans character* fons,
                      i32  mensura,
              XmlComputus* exitus);

#endif /* XML_COMPUTUS_H */
