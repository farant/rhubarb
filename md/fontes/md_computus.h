/* md_computus.h - imago memoriae et temporis parsatoris markdown
 * (gemellus css_computus.h; RP par. 11) - semita MATERIAE per clientem
 * tertium, columnae EAEDEM ac css ut phasis V trans clientes legatur.
 *
 * Una plagula, una mensura: parsura (bloci + inlinea) in piscina PROPRIA
 * cum alveo initiali fixo, deinde emissio octetorum (fons ZEPHYRUM -
 * derivata omissa), scriptura et lectio STML (consilium md cum unco
 * originis), comparator arborum in piscina altera. Tempus per clock().
 *
 * Instrumentum: ./md/computus.sh <plagula.md> [-machina] [-iter N].
 * Porta: probatio_md_computus (fixa/md, aurum
 * md/probationes/fixa/computus/basis.tsv - numeri deterministici
 * pinnati, tempora non; COMPUTUS_SCRIBERE=1 + causa nominata).
 */

#ifndef MD_COMPUTUS_H
#define MD_COMPUTUS_H

#include "latina.h"
#include "piscina.h"

#define MD_COMPUTUS_ALVEUS_INITIUM ((memoriae_index)(MMMMXCVI * CCLVI))

nomen structura {
               i32 octeti_fontis;
               i32 nodi;
               i32 lexemata;
               i32 octeti_stml;
               b32 successus;
    memoriae_index usus;
    memoriae_index commissa;
    memoriae_index otiosa;
    memoriae_index apex;
    memoriae_index alvei;
    memoriae_index allocationes;
    memoriae_index allocationes_arboris;
            duplex ms_parsandi;
            duplex ms_emittendi;
            duplex ms_arbor_scribendi;
            duplex ms_arbor_legendi;
            duplex ms_comparandi;
} MdComputus;

b32
md_computus_metiri (
    constans character* fons,
                   i32  mensura,
            MdComputus* exitus);

#endif /* MD_COMPUTUS_H */
