/* briar_registrum.h - Vocabularium nodorum thistle (cliens materiae)
 *
 * Quinque genera, XIV loci, series plana ut css/md. Regio = tag
 * crudus in columna 0 usque ad clausuram in columna 0; elementum =
 * tag STML in columna 0 usque ad clausuram in columna 0 aut linea
 * una se-claudens; prosa = cetera (markdown); interpres = linea '#!'
 * prima. Loci INDEX 'vitium' valores BriarVitium ferunt.
 */

#ifndef BRIAR_REGISTRUM_H
#define BRIAR_REGISTRUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    BRIAR_GENUS_DOCUMENTUM = 0,
    BRIAR_GENUS_INTERPRES,
    BRIAR_GENUS_PROSA,
    BRIAR_GENUS_REGIO,
    BRIAR_GENUS_ELEMENTUM,

    BRIAR_GENUS_NUMERUS
} BriarGenus;

nomen enumeratio {
    BRIAR_DOCUMENTUM_INTERPRES = 0,   /* NODUS? interpres */
    BRIAR_DOCUMENTUM_PARTES,          /* LISTA_NODUS prosa|regio|elementum */
    BRIAR_DOCUMENTUM_FINIS,           /* TOKEN FINIS */
    BRIAR_DOCUMENTUM_VITIUM           /* INDEX BriarVitium */
} BriarLocusDocumenti;

nomen enumeratio {
    BRIAR_INTERPRES_TOK = 0           /* TOKEN INTERPRES (linea tota) */
} BriarLocusInterpretis;

nomen enumeratio {
    BRIAR_PROSA_TOK = 0               /* TOKEN PROSA (cursus totus) */
} BriarLocusProsae;

nomen enumeratio {
    BRIAR_REGIO_APERTUM = 0,          /* TOKEN TAG_APERTUM (linea tota) */
    BRIAR_REGIO_TITULUS,              /* TOKEN DERIVATUM (nomen) */
    BRIAR_REGIO_CONTENTUM,            /* TOKEN? CRUDUM (absens = vacuum) */
    BRIAR_REGIO_CLAUSUM,              /* TOKEN? TAG_CLAUSUM (absens = non clausa) */
    BRIAR_REGIO_VITIUM                /* INDEX BriarVitium */
} BriarLocusRegionis;

nomen enumeratio {
    BRIAR_ELEMENTUM_TOK = 0,          /* TOKEN STML (extensio tota) */
    BRIAR_ELEMENTUM_TITULUS,          /* TOKEN DERIVATUM (nomen) */
    BRIAR_ELEMENTUM_VITIUM            /* INDEX BriarVitium */
} BriarLocusElementi;

nomen enumeratio {
    BRIAR_VITIUM_NULLUM = 0,
    BRIAR_VITIUM_REGIO_NON_CLAUSA,
    BRIAR_VITIUM_ELEMENTUM_NON_CLAUSUM,
    BRIAR_VITIUM_SAEPTUM_NON_CLAUSUM
} BriarVitium;

externus constans MateriaRegistrumCoctum BRIAR_REGISTRUM;

#endif /* BRIAR_REGISTRUM_H */
