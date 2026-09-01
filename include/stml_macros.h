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
    STML_EXPANSIO_LOCULUS_NON_ANGUSTATUS = XI,
    /* EXEMPLARIA (spec stml-exemplaria-spec.md par. 5, gradus II).
     * Dura omnia praeter XVIII (custos finis, mollis): */
    /* EXEMPLAR sine output= aut output sine sigillo '$' */
    STML_EXPANSIO_EXEMPLAR_SINE_EXITU      = XII,
    /* EXEMPLAR/PER/TRANSPARENTIA malformata: modus ignotus,
     * adsignatio output gemina, de/congruentia sine '$', voca
     * malformatum, aut constructum intra corpus templi (porta
     * 'de=@arg' nondum aedificata - spec par. 8.5) */
    STML_EXPANSIO_EXEMPLAR_MALFORMATUM     = XIII,
    /* de= aut congruentia= relationem nullam priorem nominans */
    STML_EXPANSIO_SCOPUS_IGNOTUS           = XIV,
    /* corpus exemplaris non unum elementum (silva sub applicatione
     * fluitanti - involve in parentem verum aut ancorata adhibe) */
    STML_EXPANSIO_CORPUS_SILVESTRE         = XV,
    /* modus="unum" cum 0 aut >=2 congruentiis */
    STML_EXPANSIO_UNUM_VIOLATUM            = XVI,
    /* modus="optional" cum >=2 congruentiis */
    STML_EXPANSIO_OPTIONAL_MULTIPLEX       = XVII,
    /* relatio output= quam nihil inferius consumit (codex mortuus;
     * custos finis - ambulatio pergit, successus cadit) */
    STML_EXPANSIO_EXITUS_NON_CONSUMPTUS    = XVIII,
    /* TRANSPARENTIA sine tags= aut tags vacuis; attributa=
     * (caecitas attributorum - tituli formae littera congruentiae
     * non facti, decretum 2026-08-31) praesens sed vacuum */
    STML_EXPANSIO_TRANSPARENTIA_MALFORMATA = XIX,
    /* CATENA malformata: adsignatio output gemina, de= sine '$',
     * modus=/ancorata= in involucro (gradibus pertinent), catena
     * vacua, liber non-nexus (textus/elementum alienum/transclusio
     * contenti), nexus cum de=/output= (uncus RESERVATUS -
     * ratificationes III/IV 2026-08-31), aut CATENA nidificata cum
     * attributis (nexus catenatus NUDUS solum - plananda) */
    STML_EXPANSIO_CATENA_MALFORMATA        = XX,
    /* DIRIBITIO malformata (dispensatio plani catenae - decretum
     * 2026-08-31): liber non-bracchium; CASUS sine <@est=> aut
     * cum pluribus; corpus bracchii non unum mandatum purum;
     * radix conditionis cum output=/de=/modus (probatio boolea
     * nuda - ancorata licet); ORDINARIUS non ultimus (bracchia
     * posteriora obumbraret); diribitio sine bracchiis; mandatum
     * ignotum (nec EXEMPLAR nec CATENA nec DIRIBITIO); involucrum
     * cum modus=/ancorata=; adsignatio gemina; nexus catenae
     * DIRIBITIO cum attributis. */
    STML_EXPANSIO_DIRIBITIO_MALFORMATA     = XXI,
    /* Collisio capturarum trans gradus (lex extensionis, decretum
     * 2026-08-31): ordines tabulata sunt - gradus quisque capturas
     * ordinis influentis HEREDITAT et suas addit; titulus iteratus
     * trans gradus = collisio CLARA (correlatio ponti '&@n;'
     * pertinet, numquam iterationi fortuitae; iunctio-per-
     * nominationem = ianua nominata, non v1). */
    STML_EXPANSIO_CAPTURA_COLLISA          = XXII,
    /* SINE malformatum (antiiunctio - decretum 2026-08-31): nexus
     * catenae solus, filtrum purum (ordines cum ZEPHYRO
     * congruentiis interioribus servati, INTACTI). Vitia: attributa
     * machinae in SINE (de/output/modus/ancorata - nihil fert,
     * nihil vertit); corpus non exemplar unum; fons NIHIL (nexus
     * primus catenae documenti sine de= - absentia lata documenti
     * = ianua posterior nominata, CLARE recusata). */
    STML_EXPANSIO_SINE_MALFORMATUM         = XXIII,
    /* INDAGO malformatum (vestigatio resolutionis - decretum
     * 2026-08-31): elementum NUDUM solum ('<INDAGO/>') - attributum
     * quodvis aut liber non-commentum = vitium. Documento ferente,
     * mandata consumpta commenta fabulae suae in sedibus suis
     * relinquunt (bytes consulto differunt - vestigium in medio
     * ipso vivit, commentum = invisibile natura). */
    STML_EXPANSIO_INDAGO_MALFORMATUM       = XXIV
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


/* ==================================================
 * CONGRUENTIA STRICTA (spec exemplarium par. 4, gradus
 * aedificationis I) - matcher gradarius generalis, ex
 * silva_arbor.c promotus ('definitio retro currit' - templum
 * contra candidatum, permissivitate NULLA). Modus STRICTUS
 * machinae; modus laxus (EXEMPLAR) iuxta aedificabitur.
 *
 * Regulae (quinque, ex recognitione parametrorum probatae):
 *   - genus/titulus/fragmentum/crudus/attributa aequalia exacte
 *   - liberum definitionis UNICUM textus totus '&@x;' = captura
 *     silvae (liberi candidati OMNES, saltem unus)
 *   - loculus ITERATUS = silvae octetim aequales (regula V -
 *     capturae non-lineares aequalitas sunt)
 *   - textus/transclusio per valorem; cetera liberi gradatim
 * ================================================== */

/* Captura congruentiae: loculus + silva capta */
nomen structura {
    chorda* titulus;   /* nomen loculi, internatum */
       Xar* nodi;      /* StmlNodus* capti, ordine candidati */
} StmlCaptura;

/* Par nodorum (vetus = candidati, novus = definitionis) - pro
 * consumentibus qui sedes vel tabulas laterales repungunt */
nomen structura {
    StmlNodus* vetus;
    StmlNodus* novus;
} StmlCongruentiaPar;

/* Congruentia stricta gradaria: corpus templi contra candidatum.
 * capturae (Xar de StmlCaptura, non NIHIL): capturae appenduntur
 * ordine inventionis. paria (Xar de StmlCongruentiaPar, NIHIL
 * licet): paria appenduntur successu; VOCANS defectu truncat
 * (xar_truncare ad initium suum) - matcher numquam retro tollit. */
b32
stml_congruere_strictum (
                Piscina* piscina,
    InternamentumChorda* intern,
              StmlNodus* templum,
              StmlNodus* candidatus,
                    Xar* capturae,
                    Xar* paria);

#endif /* STML_MACROS_H */
