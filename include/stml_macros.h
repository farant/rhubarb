#ifndef STML_MACROS_H
#define STML_MACROS_H

/* stml_macros.h - Expansio fragmentorum parametrizatorum (v1)
 *
 * Macrones = fragmenta cum argumentis (spec:
 * project-specs/stml-macros-spec.md; colloquium:
 * stml-macros-interview.md). Plagula formam macroneam SERVAT -
 * expansio proiectio pura tree->tree tempore onerationis est
 * (legere -> expandere -> consumens); arbor originalis intacta
 * manet et interrogabilis; scriptor arborem expansam numquam
 * videt.
 *
 *   Definitio:  <#f p="@p"><a x="&@p;"/></#>
 *               (loculi DECLARATI in tago aperienti, numquam
 *               inferti; corpus per '&@nomen;' refert)
 *   Vocatio:    <<#f p="123">>
 *               (transclusio cum argumentis; vocatio sine
 *               argumentis fragmenti sine loculis = resolutio
 *               transclusionis simplex)
 *   Strata:     ordo documenti - vocatio solum ad definitiones
 *               PRIORES resolvit; referentia posterior = vitium;
 *               terminatio per constructionem.
 *
 * Omnis defectus referentiae CLARUS est (lex 6): sex vitia
 * nominata, primum vincit. '&@x;' EXTRA corpus definitionis
 * littera manet (regula entis ignoti).
 */

#include "stml.h"

/* Vitia expansionis - primum inventum vincit */
nomen enumeratio {
    STML_EXPANSIO_BENE                  = ZEPHYRUM,
    /* vocatio ad id nusquam definitum */
    STML_EXPANSIO_FRAGMENTUM_IGNOTUM    = I,
    /* vocatio ad id POSTERIUS definitum (violatio stratorum) */
    STML_EXPANSIO_FRAGMENTUM_POSTERIUS  = II,
    /* definitiones duae eodem id */
    STML_EXPANSIO_FRAGMENTUM_GEMINUM    = III,
    /* loculus declaratus quem vocatio non implevit */
    STML_EXPANSIO_LOCULUS_NON_IMPLETUS  = IV,
    /* argumentum vocationis loculum nullum declaratum nominans */
    STML_EXPANSIO_ARGUMENTUM_SUPERFLUUM = V,
    /* corpus loculum non declaratum refert (in COLLECTIONE
     * iudicatum - linea definitionis) */
    STML_EXPANSIO_LOCULUS_IGNOTUS       = VI
} StmlExpansioVitium;

/* Nota provenientiae - una per splicem, radix splicis (liberi
 * implicati; splices interiores notas proprias ferunt). Tabula
 * lateralis, exemplar sedium: StmlNodus intactus, quaestiones
 * tabulam iungunt. */
nomen structura {
     StmlNodus* nodus;          /* radix splicis in arbore expansa */
        chorda* fragmentum_id;  /* internatum */
     StmlNodus* vocatio;        /* nodus transclusionis ORIGINALIS */
           i32  stratum;        /* profunditas impletionis, I-basata */
} StmlExpansioNota;

nomen structura {
                   b32  successus;
             StmlNodus* radix_expansa;      /* arbor NOVA; originalis
                                             * intacta */
                  Xar* tabula_expansionum; /* StmlExpansioNota,
                                             * ordine splicis */
    StmlExpansioVitium vitium;
                   i32 linea;              /* nodi peccantis (aut 0) */
                chorda fragmentum;         /* id in quaestione
                                             * (aut vacua) */
                chorda loculus;            /* loculus in quaestione
                                             * (aut vacua) */
} StmlExpansioResultus;

/* Expandere documentum: arbor nova in piscina vocantis; definitiones
 * demissae (visio contenti); vocationes corporibus impletis
 * substitutae; transclusiones non-'#' transeunt ut nodi. */
StmlExpansioResultus
stml_expandere (
              StmlNodus* radix,
                Piscina* piscina,
    InternamentumChorda* intern);

#endif /* STML_MACROS_H */
