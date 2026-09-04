/* oratio_registrum.h - Vocabularium nodorum orationis (oratio-spec par. III)
 *
 * MANU SCRIPTUM ut md_registrum.h: enumeratio typum VERUM habet et
 * comitem ORATIO_GENUS_NUMERUS; probatio congruentiam cum tabula per
 * TITULOS et CONTIGUITATEM offsetuum asserit.
 *
 * MODELLUM (spec par. III, ut aedificatum): documentum -> paragraphi
 * -> sententiae -> elementa (vocabulum | interpunctio | numerus).
 * Omnis octetus lexema in loco UNO est. Spatia et lineae novae
 * CONTENTUM sunt, numquam trivia: lex possessoris (STML par. I.2):
 * spatium post elementum usque ad lineam novam primam INCLUSIVE in
 * CAUDA elementi iacet; residuum (lineae vacuae inter paragraphos) in
 * CAUDA paragraphi; spatium ante paragraphum primum in PRAEFIXIS
 * documenti, indentatio paragraphi in PRAEFIXIS paragraphi.
 * Sententia loca spatii non habet - elementum ultimum spatium suum
 * fert.
 *
 * VOCABULUM = verbum superficiei cum PARTIBUS (litterae, hyphen,
 * apostrophus, punctum intra): 'well-known', 'don't', 'virumque' sunt
 * vocabulum UNUM; arbor linguistica non decernit. ANALYSES (gradus
 * III) sunt nodi generum analysis-* (appensa), ordine fontis, prima
 * = primaria; CLASSES et LINGUAE lexemata DERIVATA (fons I) sunt -
 * compendium classium universalium ordine, ut selectio
 * '[classes~=verbum]' congruat. Lista absens = vacua (materia locum
 * semel scribit).
 *
 * Genera APPENDUNTUR, numquam interponuntur.
 */

#ifndef ORATIO_REGISTRUM_H
#define ORATIO_REGISTRUM_H

#include "latina.h"
#include "materia_registrum.h"

nomen enumeratio {
    ORATIO_GENUS_DOCUMENTUM = 0,
    ORATIO_GENUS_PARAGRAPHUS,
    ORATIO_GENUS_SENTENTIA,
    ORATIO_GENUS_VOCABULUM,
    ORATIO_GENUS_INTERPUNCTIO,
    ORATIO_GENUS_NUMERUS,

    ORATIO_GENUS_NUMERUS_GENERUM
} OratioGenus;


/* ==================================================
 * Indices LOCORUM intra nodum (fenestra generis)
 * ================================================== */

nomen enumeratio {
    ORATIO_DOCUMENTUM_PRAEFIXA = 0,   /* LISTA_TOKEN: spatium ante paragraphum primum */
    ORATIO_DOCUMENTUM_PARAGRAPHI,     /* LISTA_NODUS paragraphus */
    ORATIO_DOCUMENTUM_FINIS           /* TOKEN FINIS */
} OratioLocusDocumenti;

nomen enumeratio {
    ORATIO_PARAGRAPHUS_PRAEFIXA = 0,  /* LISTA_TOKEN: indentatio */
    ORATIO_PARAGRAPHUS_SENTENTIAE,    /* LISTA_NODUS sententia */
    ORATIO_PARAGRAPHUS_CAUDA          /* LISTA_TOKEN: lineae vacuae post */
} OratioLocusParagraphi;

nomen enumeratio {
    ORATIO_SENTENTIA_ELEMENTA = 0     /* LISTA_NODUS vocabulum | interpunctio | numerus */
} OratioLocusSententiae;

nomen enumeratio {
    ORATIO_VOCABULUM_PARTES = 0,      /* LISTA_TOKEN: litterae, hyphen, apostrophus, punctum */
    ORATIO_VOCABULUM_CAUDA,           /* LISTA_TOKEN: spatium post (lex possessoris) */
    ORATIO_VOCABULUM_ANALYSES,        /* LISTA_NODUS analysis-* (gradus III) */
    ORATIO_VOCABULUM_CLASSES,         /* TOKEN? DERIVATUM: classes ordine */
    ORATIO_VOCABULUM_LINGUAE          /* TOKEN? DERIVATUM: linguae ordine */
} OratioLocusVocabuli;

nomen enumeratio {
    ORATIO_INTERPUNCTIO_SIGNUM = 0,   /* TOKEN: punctum | interpunctio | signum */
    ORATIO_INTERPUNCTIO_CAUDA         /* LISTA_TOKEN */
} OratioLocusInterpunctionis;

nomen enumeratio {
    ORATIO_NUMERUS_CRUDUM = 0,        /* LISTA_TOKEN: digiti et separatores */
    ORATIO_NUMERUS_CAUDA              /* LISTA_TOKEN */
} OratioLocusNumeri;

externus constans MateriaRegistrumCoctum ORATIO_REGISTRUM;

#endif /* ORATIO_REGISTRUM_H */
