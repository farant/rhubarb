/* materia_registrum.h - Registrum generum coctum (vocabularium nodorum)
 *
 * Portatum ex silva_registrum.h, quod M4 ex silva_tabulae.h scidit.
 * Illa scissio limitem strati VISIBILEM fecit in grapho inclusionum;
 * hoc caput eundem contractum in materia servat.
 *
 * NIHIL INCLUDIT PRAETER latina.h. Paupertas contractus est: qui
 * vocabularium nodorum legere vult motorem parsandi trahere NON debet.
 */

#ifndef MATERIA_REGISTRUM_H
#define MATERIA_REGISTRUM_H

#include "latina.h"

nomen structura {
    constans character* titulus;     /* nomen loci */
                   s32  species;     /* MateriaLocusSpecies */
} MateriaTabLocus;

nomen structura {
    constans character* titulus;     /* nomen generis */
                   i32  loci_offset; /* in seriem planam locorum */
                   i32  loci_numerus;
} MateriaTabGenus;

nomen structura {
    constans MateriaTabGenus* genera;
                         i32  numerus_generum;
    constans MateriaTabLocus* loci;
                         i32  numerus_locorum;
} MateriaRegistrumCoctum;


/* ==================================================
 * Diagnostica declarata (materia-sedes B1)
 *
 * Tabula SEPARATA, non campi novi in MateriaTabGenus/Locus: tabulae
 * registri manu scriptae (briar, shim C89, probationes) sub
 * -Wmissing-field-initializers frangerentur. Sigillum registri
 * nomina, offset et species solum hashit - diagnostica nullum
 * sigillum movent. Generatur solum si declaratio aliquid declarat
 * (<PRAEFIXUM>_DIAGNOSTICA); ceteri clientes octetim idem.
 * ================================================== */

nomen enumeratio {
    MATERIA_DIAGNOSTICUM_GENUS = 0,   /* nodus generis ipse */
    MATERIA_DIAGNOSTICUM_ABSENTIA,    /* locus non scriptus */
    MATERIA_DIAGNOSTICUM_VACUA        /* lista sine elemento pleno */
} MateriaDiagnosticumSpecies;

nomen enumeratio {
    MATERIA_GRAVITAS_ERRATUM = 0,
    MATERIA_GRAVITAS_MONITUM
} MateriaGravitas;

nomen structura {
                   s32  genus;     /* index generis */
                   s32  locus;     /* intra genus; -I pro GENUS */
                   s32  species;   /* MateriaDiagnosticumSpecies */
                   s32  gravitas;  /* MateriaGravitas */
    constans character* codex;     /* "grex/tok_clausura" */
    constans character* causa;
} MateriaTabDiagnosticum;

nomen structura {
    constans MateriaTabDiagnosticum* tabula;
                                i32  numerus;
    /* genera 'inanis': nodi eorum listam 'vacua' non implent
     * (separator, heredoc) */
                       constans s32* inania;
                                i32  numerus_inanium;
} MateriaDiagnosticaCocta;

#endif /* MATERIA_REGISTRUM_H */
