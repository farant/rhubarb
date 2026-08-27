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
    STML_EXPANSIO_LOCULUS_IGNOTUS       = VI,
    /* argumentum SUBARBOREUM positionem CHORDAE implens
     * (attributum, valor transclusionis, textus interpolatus) -
     * numquam chordificatio tacita (par. 6.1, vitium septimum) */
    STML_EXPANSIO_ARGUMENTUM_ARBOREUM   = VII,
    /* nomen argumenti bis datum (inscriptum + blocum, aut bloca
     * bina) - 'ultimus vincit' numquam fit */
    STML_EXPANSIO_ARGUMENTUM_GEMINUM    = VIII,
    /* COMMUTATIO forma mala (in COLLECTIONE iudicata - planum
     * statice iudicabile manet): de non totus-ref '&@x;'; est cum
     * referentia (LINEA: casus litterae, numquam praedicata
     * computata); liberum non-CASUS/ORDINARIUS; ORDINARIUS non
     * ultimus; attributum casus ignotum (par. 6.2) */
    STML_EXPANSIO_COMMUTATIO_MALFORMATA = IX,
    /* impletione: nullum bracchium congruit nec ORDINARIUS adest -
     * numquam vacuum tacitum (par. 6.2) */
    STML_EXPANSIO_CASUS_NULLUS          = X,
    /* loculus OPTIONALIS ('p="@p?"') extra bracchium praesentiam
     * statuens relatus (bracchia est/non-nihil statuunt; nihil et
     * ORDINARIUS non - ORDINARIUS et praesente et absente
     * exsequitur). In COLLECTIONE iudicatum: angustatio fluxus,
     * optionalitas honesta - numquam vacuum tacitum (par. 6.2) */
    STML_EXPANSIO_LOCULUS_NON_ANGUSTATUS = XI
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


/* ==================================================
 * DISTRIBUTIO (ronda IV colloquii exemplarium, rata 2026-08-26) -
 * proiectio contenti SECUNDA: legere -> expandere -> distribuere ->
 * consumens. NON intra stml_expandere: contractus machinae =
 * spatium templi '@' solum; distributio spatium contenti innotatum
 * est - limes purus manet. Arbor originalis saccharum authoris
 * octetim servat; consumentes in cursum sponte inscribuntur.
 *
 *   <nota t="a">          duo fratres <nota t="a"> fiunt -
 *     <> emere lac </>    involucrum typum DICTAT, deinde
 *     <> ianuam </>       dissolvitur
 *   </>
 *
 * Involucrum = elementum NOMINATUM cuius liberi (post praefixum
 * elementorum attributorum) fragmenta anonyma OMNES sunt (item).
 * Leges:
 *   - attributa involucri descendunt in item singula; collisio
 *     ITEM-VINCIT (attributum item eodem nomine - inscriptum aut
 *     elementum praefixi - involucrum supprimit); sepulcrum
 *     '<@x=/>' in praefixo item attributum hereditatum DELET
 *     (verbum delendi, par. 6.3) et in exitu manet (absentia
 *     explicita)
 *   - mixtura anonymi/nominati sub involucro = VITIUM clarum
 *     (homogeneitas; commenta transeunt positione servata)
 *   - item vacuum = elementum vacuum; item silvae sunt
 *   - nidificatio recursiva: involucra intra item distribuuntur;
 *     listae anonymae nidificatae typo eodem PLANANTUR (rithmus
 *     PER degeneratae - involucrum = templum uni-elementi)
 *   - fragmenta anonyma sine parente nominato (gradus documenti,
 *     intra fragmenta nominata) TRANSEUNT intacta
 * ================================================== */

/* Vitia distributionis - primum inventum vincit */
nomen enumeratio {
    STML_DISTRIBUTIO_BENE    = ZEPHYRUM,
    /* item et liberi nominati mixti sub involucro eodem */
    STML_DISTRIBUTIO_MIXTA   = I,
    /* memoria defecit */
    STML_DISTRIBUTIO_MEMORIA = II
} StmlDistributioVitium;

nomen structura {
                      b32  successus;
                StmlNodus* radix_distributa; /* arbor NOVA; originalis
                                             * intacta */
    StmlDistributioVitium vitium;
                      i32 linea;           /* liberi peccantis
                                             * (aut 0) */
                   chorda titulus;         /* involucri peccantis
                                             * (aut vacua) */
} StmlDistributioResultus;

/* Distribuere documentum: arbor nova in piscina vocantis;
 * involucra dissolvuntur, cetera clonantur. Super arborem EXPANSAM
 * plerumque vocatur (cursus plenus); in arbore quavis totalis. */
StmlDistributioResultus
stml_distribuere (
              StmlNodus* radix,
                Piscina* piscina,
    InternamentumChorda* intern);

#endif /* STML_MACROS_H */
