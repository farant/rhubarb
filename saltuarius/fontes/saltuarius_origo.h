/* saltuarius_origo.h - Catena originis + tabella (Phase C)
 *
 * ICTUS PECUNIAE: directio-conscius (INTENTIO clavis #2).
 * - Stratum N>0: lexema sub cursore -> catena SilvaOrigo DEORSUM
 *   ambulata (corpus = situs definitionis, invocatio = situs usus,
 *   nomen_macro) usque ad FONS.
 * - Stratum 0: quaestio PRORSUM - primum lexema fluxus finalis
 *   cuius maiorum FONS offset cursoris tegit ("quid hoc fit").
 * Quisque gradus resolvibilis SALTABILIS est (via + linea).
 *
 * Arena propria refecta per aedificationem (tituli ordinum) -
 * memoria frenata, exemplar limitis.
 */

#ifndef SALTUARIUS_ORIGO_H
#define SALTUARIUS_ORIGO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva.h"
#include "saltuarius_liber.h"

#define SALT_GRADUS_MAXIMI 16

nomen enumeratio {
    SALT_GRADUS_CAPUT = 0,     /* lexema ipsum */
    SALT_GRADUS_FIT,           /* stratum 0: quid fit */
    SALT_GRADUS_EXPANSIO,      /* <- expansio MACRO */
    SALT_GRADUS_ALIUS,         /* pasta / chorda / api */
    SALT_GRADUS_SCRIPTUM       /* fons (radix) */
} SaltuariusGradusGenus;

nomen structura {
    SaltuariusGradusGenus genus;
    chorda                titulus;     /* textus ordinis */
    b32                   saltabile;
    chorda                via;         /* fons via (alias in exp) */
    s32                   fons_index;
    i32                   linea;       /* 1-basata */
} SaltuariusGradus;

nomen structura {
    Piscina*         arena;       /* refecta per aedificationem */
    PiscinaNotatio   nota;
    SaltuariusGradus gradus[SALT_GRADUS_MAXIMI];
    i32              numerus;
    s32              selectio;
    b32              apertum;
} SaltuariusOrigo;

SaltuariusOrigo*
saltuarius_origo_creare (Piscina* persistens);

/* Catenam aedificare pro cursore libri (stratum activum decernit
 * directionem). VERUM si catena aedificata (tabella aperienda);
 * FALSUM si nihil narrabile (sine parsura / nihil inventum). */
b32
saltuarius_origo_aedificare (SaltuariusOrigo* origo,
    SaltuariusLiber* liber, constans SaltuariusNexus* nexus);

vacuum
saltuarius_origo_claudere (SaltuariusOrigo* origo);

vacuum
saltuarius_origo_movere (SaltuariusOrigo* origo, s32 delta);

/* Gradus selectus si saltabilis; NIHIL alioquin */
constans SaltuariusGradus*
saltuarius_origo_saltus (constans SaltuariusOrigo* origo);

#endif /* SALTUARIUS_ORIGO_H */
