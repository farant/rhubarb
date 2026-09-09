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

    /* gradus III (T11): genera analysis-* APPENSA, unum per classem
     * universalem ordine UD (spec par. V); genus = PRIMUM + classis */
    ORATIO_GENUS_ANALYSIS_SUBSTANTIVI,
    ORATIO_GENUS_ANALYSIS_NOMINIS_PROPRII,
    ORATIO_GENUS_ANALYSIS_VERBI,
    ORATIO_GENUS_ANALYSIS_AUXILIARIS,
    ORATIO_GENUS_ANALYSIS_ADIECTIVI,
    ORATIO_GENUS_ANALYSIS_ADVERBII,
    ORATIO_GENUS_ANALYSIS_PRONOMINIS,
    ORATIO_GENUS_ANALYSIS_DETERMINANTIS,
    ORATIO_GENUS_ANALYSIS_ADPOSITIONIS,
    ORATIO_GENUS_ANALYSIS_NUMERALIS,
    ORATIO_GENUS_ANALYSIS_CONIUNCTIONIS_COORDINANTIS,
    ORATIO_GENUS_ANALYSIS_CONIUNCTIONIS_SUBORDINANTIS,
    ORATIO_GENUS_ANALYSIS_PARTICULAE,
    ORATIO_GENUS_ANALYSIS_INTERIECTIONIS,
    ORATIO_GENUS_ANALYSIS_SYMBOLI,
    ORATIO_GENUS_ANALYSIS_INTERPUNCTIONIS,
    ORATIO_GENUS_ANALYSIS_IGNOTI,
    /* T19d (2026-09-07): UMBRA - dependens exspectatus lectionis
     * (relatio, condiciones, impletio); APPENSA post analyses, ergo
     * analyses [PRIMUM, ULTIMUM) contiguae manent */
        ORATIO_GENUS_UMBRA,
    /* T20a (2026-09-08, decisiones XLIII-XLIV): CLAUSULA - capsa
     * sudoku inter vicinitatem et sententiam; nodus in lista
     * 'clausulae' sententiae, umbras e schemate ferens (verbum
     * finitum, subiectum); elementa plana per INDEX eum monstrant.
     * APPENSA post umbram (extra [PRIMUM, ULTIMUM) analysium) */
    ORATIO_GENUS_CLAUSULA,

    ORATIO_GENUS_NUMERUS_GENERUM,
    ORATIO_GENUS_ANALYSIS_PRIMUM = ORATIO_GENUS_ANALYSIS_SUBSTANTIVI,
    ORATIO_GENUS_ANALYSIS_ULTIMUM = ORATIO_GENUS_UMBRA   /* exclusivum */
} OratioGenus;

/* Classes universales (UD XVII, spec par. V) ordine generum analysis-*;
 * tituli = optiones 'classis' canonis glossarii (custos in porta). */
nomen enumeratio {
    ORATIO_CLASSIS_SUBSTANTIVUM,
    ORATIO_CLASSIS_NOMEN_PROPRIUM,
    ORATIO_CLASSIS_VERBUM,
    ORATIO_CLASSIS_AUXILIARE,
    ORATIO_CLASSIS_ADIECTIVUM,
    ORATIO_CLASSIS_ADVERBIUM,
    ORATIO_CLASSIS_PRONOMEN,
    ORATIO_CLASSIS_DETERMINANS,
    ORATIO_CLASSIS_ADPOSITIO,
    ORATIO_CLASSIS_NUMERALE,
    ORATIO_CLASSIS_CONIUNCTIO_COORDINANS,
    ORATIO_CLASSIS_CONIUNCTIO_SUBORDINANS,
    ORATIO_CLASSIS_PARTICULA,
    ORATIO_CLASSIS_INTERIECTIO,
    ORATIO_CLASSIS_SYMBOLUM,
    ORATIO_CLASSIS_INTERPUNCTIO,
    ORATIO_CLASSIS_IGNOTUM,

    ORATIO_CLASSIS_NUMERUS_CLASSIUM
} OratioClassis;


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
    ORATIO_PARAGRAPHUS_CAUDA,         /* LISTA_TOKEN: lineae vacuae post */
    ORATIO_PARAGRAPHUS_FORMA          /* INDEX: OratioForma (T6b, lex deprimendi) */
} OratioLocusParagraphi;

/* FORMA textus paragraphi (spec decisio XXIII): a classificatore ex
 * indiciis arboris ante lectorem sententiarum decisa et ut INDEX in
 * paragrapho deposita; lector eam consulit (versus/tabula/index:
 * linea unitas; titulus: unitas una; prosa: regula sententiarum).
 * Ambiguitas ad PROSAM vergit (decisio XXIV: iunctio, non scissio).
 * APPENDUNTUR, numquam permutantur - valor in proiectione numerus est. */
nomen enumeratio {
    ORATIO_FORMA_PROSA = 0,
    ORATIO_FORMA_VERSUS,
    ORATIO_FORMA_TITULUS,
    ORATIO_FORMA_TABULA,
    ORATIO_FORMA_INDEX,

    ORATIO_FORMA_NUMERUS_FORMARUM
} OratioForma;

nomen enumeratio {
    ORATIO_SENTENTIA_ELEMENTA = 0,    /* LISTA_NODUS vocabulum | interpunctio | numerus */
    ORATIO_SENTENTIA_CLAUSULAE        /* LISTA_NODUS clausula (T20a; absens = non stampata) */
} OratioLocusSententiae;

nomen enumeratio {
    ORATIO_VOCABULUM_PARTES = 0,      /* LISTA_TOKEN: litterae, hyphen, apostrophus, punctum */
    ORATIO_VOCABULUM_CAUDA,           /* LISTA_TOKEN: spatium post (lex possessoris) */
    ORATIO_VOCABULUM_ANALYSES,        /* LISTA_NODUS analysis-* (gradus III) */
    ORATIO_VOCABULUM_CLASSES,         /* TOKEN? DERIVATUM: classes ordine */
    ORATIO_VOCABULUM_LINGUAE,         /* TOKEN? DERIVATUM: linguae ordine */
        ORATIO_VOCABULUM_DECISIO,         /* INDEX? OratioDecisio (T19g): non scripta = nemo decidit */
    ORATIO_VOCABULUM_AUCTOR,          /* TOKEN? DERIVATUM (T19g): titulus regulae decidentis */
    ORATIO_VOCABULUM_CLAUSULA,        /* INDEX? (T20a): clausula intra clausulas sententiae; non scripta = APERTA */
    ORATIO_VOCABULUM_CLAUSULA_CAUSA   /* INDEX? OratioClausulaCausa (T20a): stratum quod posuit */
} OratioLocusVocabuli;

nomen enumeratio {
        ORATIO_INTERPUNCTIO_SIGNUM = 0,   /* TOKEN: punctum | interpunctio | signum */
    ORATIO_INTERPUNCTIO_CAUDA,        /* LISTA_TOKEN */
    ORATIO_INTERPUNCTIO_CLAUSULA,     /* INDEX? (T20a) */
    ORATIO_INTERPUNCTIO_CLAUSULA_CAUSA /* INDEX? OratioClausulaCausa (T20a) */
} OratioLocusInterpunctionis;

nomen enumeratio {
        ORATIO_NUMERUS_CRUDUM = 0,        /* LISTA_TOKEN: digiti et separatores */
    ORATIO_NUMERUS_CAUDA,             /* LISTA_TOKEN */
    ORATIO_NUMERUS_CLAUSULA,          /* INDEX? (T20a) */
    ORATIO_NUMERUS_CLAUSULA_CAUSA     /* INDEX? OratioClausulaCausa (T20a) */
} OratioLocusNumeri;


/* ==================================================
 * Gradus III (T11): analyses - loci et accidentia
 * ==================================================
 *
 * Genus analysis-<classis> locos habet: V COMMUNES (lemma, lingua,
 * fons, nativum, sensus) deinde ACCIDENTIA classis suae ut INDEX in
 * enumerationes parvas (valor in proiectione numerus est, ut forma
 * paragraphi). Locus absens = accidens non datum (WORDS 'X'). Lex
 * appendendi: enumerationes APPENDUNTUR, numquam permutantur.
 *
 * Accidentia per classem (decisio 2026-09-04, electio Claudi Frano
 * probante): substantivum et nomen proprium casus numerus genus
 * declinatio; verbum et auxiliare persona numerus tempus modus vox
 * forma-verbi coniugatio casus genus (participia congruunt);
 * adiectivum casus numerus genus gradus declinatio; adverbium gradus;
 * pronomen casus numerus genus persona; determinans casus numerus
 * genus; adpositio casus (rectus); numerale casus numerus genus
 * species; cetera (coniunctiones, particula, interiectio, symbolum,
 * interpunctio, ignotum) communia sola.
 */

/* casus */
nomen enumeratio {
    ORATIO_CASUS_NOMINATIVUS = 0,
    ORATIO_CASUS_GENITIVUS,
    ORATIO_CASUS_DATIVUS,
    ORATIO_CASUS_ACCUSATIVUS,
    ORATIO_CASUS_ABLATIVUS,
    ORATIO_CASUS_LOCATIVUS,
    ORATIO_CASUS_VOCATIVUS,

    ORATIO_CASUS_NUMERUS
} OratioCasus;

/* numerus */
nomen enumeratio {
    ORATIO_NUMERUS_GRAMMATICUS_SINGULARIS = 0,
    ORATIO_NUMERUS_GRAMMATICUS_PLURALIS,
    ORATIO_NUMERUS_GRAMMATICUS_DUALIS,

    ORATIO_NUMERUS_GRAMMATICUS_NUMERUS
} OratioNumerusGrammaticus;

/* genus */
nomen enumeratio {
    ORATIO_GENUS_GRAMMATICUM_MASCULINUM = 0,
    ORATIO_GENUS_GRAMMATICUM_FEMININUM,
    ORATIO_GENUS_GRAMMATICUM_NEUTRUM,
    ORATIO_GENUS_GRAMMATICUM_COMMUNE,

    ORATIO_GENUS_GRAMMATICUM_NUMERUS
} OratioGenusGrammaticum;

/* persona */
nomen enumeratio {
    ORATIO_PERSONA_PRIMA = 0,
    ORATIO_PERSONA_SECUNDA,
    ORATIO_PERSONA_TERTIA,

    ORATIO_PERSONA_NUMERUS
} OratioPersona;

/* tempus */
nomen enumeratio {
    ORATIO_TEMPUS_PRAESENS = 0,
    ORATIO_TEMPUS_IMPERFECTUM,
    ORATIO_TEMPUS_FUTURUM,
    ORATIO_TEMPUS_PERFECTUM,
    ORATIO_TEMPUS_PLUSQUAMPERFECTUM,
    ORATIO_TEMPUS_FUTURUM_EXACTUM,
    ORATIO_TEMPUS_PRAETERITUM,

    ORATIO_TEMPUS_NUMERUS
} OratioTempus;

/* modus */
nomen enumeratio {
    ORATIO_MODUS_INDICATIVUS = 0,
    ORATIO_MODUS_SUBIUNCTIVUS,
    ORATIO_MODUS_IMPERATIVUS,
    ORATIO_MODUS_INFINITIVUS,

    ORATIO_MODUS_NUMERUS
} OratioModus;

/* vox */
nomen enumeratio {
    ORATIO_VOX_ACTIVA = 0,
    ORATIO_VOX_PASSIVA,
    ORATIO_VOX_DEPONENS,

    ORATIO_VOX_NUMERUS
} OratioVox;

/* forma-verbi */
nomen enumeratio {
    ORATIO_FORMA_VERBI_FINITUM = 0,
    ORATIO_FORMA_VERBI_INFINITIVUM,
    ORATIO_FORMA_VERBI_PARTICIPIUM,
    ORATIO_FORMA_VERBI_GERUNDIUM,
    ORATIO_FORMA_VERBI_GERUNDIVUM,
    ORATIO_FORMA_VERBI_SUPINUM,

    ORATIO_FORMA_VERBI_NUMERUS
} OratioFormaVerbi;

/* gradus */
nomen enumeratio {
    ORATIO_GRADUS_POSITIVUS = 0,
    ORATIO_GRADUS_COMPARATIVUS,
    ORATIO_GRADUS_SUPERLATIVUS,

    ORATIO_GRADUS_NUMERUS
} OratioGradus;

/* species */
nomen enumeratio {
    ORATIO_SPECIES_NUMERALIS_CARDINALE = 0,
    ORATIO_SPECIES_NUMERALIS_ORDINALE,
    ORATIO_SPECIES_NUMERALIS_DISTRIBUTIVUM,
    ORATIO_SPECIES_NUMERALIS_ADVERBIALE,

    ORATIO_SPECIES_NUMERALIS_NUMERUS
} OratioSpeciesNumeralis;

/* lingua */
nomen enumeratio {
    ORATIO_LINGUA_LATINA = 0,
    ORATIO_LINGUA_ANGLICA,

    ORATIO_LINGUA_NUMERUS
} OratioLingua;

/* DIALECTUS documenti (T22 b / T30 c, 2026-09-09): classicus | medius -
 * decisio ex censu formarum per orthographiam mediam (contractus IV)
 * recuperatarum inter vocabula documenti (praescansio); medius si
 * >= V millesimae (chartae XXIX, Dante XII, classici <= I). Ignotus =
 * nondum censum (annotator ipse censet). Medius: formae NOTAE quoque
 * variant (sancte = sanctae). Ut lingua documenti (T18), non locus
 * arboris sed decisio censu documenti toti. */
nomen enumeratio {
    ORATIO_DIALECTUS_IGNOTUS = 0,
    ORATIO_DIALECTUS_CLASSICUS,
    ORATIO_DIALECTUS_MEDIUS,

    ORATIO_DIALECTUS_NUMERUS
} OratioDialectus;

/* fons */
nomen enumeratio {
    ORATIO_FONS_ANALYSIS_VOCABULARIUM_LA = 0,
    ORATIO_FONS_ANALYSIS_VOCABULARIUM_EN,
        ORATIO_FONS_ANALYSIS_GLOSSARIUM,
        ORATIO_FONS_ANALYSIS_REGULA,     /* T13: regula sine fonte (vocabulum capitale ignotum = nomen proprium) */
    ORATIO_FONS_ANALYSIS_ORTHOGRAPHIA,   /* T22: forma varians orthographiae mediae (e > ae, h addita ...) in vocabulario Latino inventa */

    ORATIO_FONS_ANALYSIS_NUMERUS
} OratioFonsAnalysis;

/* RELATIO umbrae (T19d): quid lectio a dependente exspectet.
 * APPENSA solum. obiectum = rectum (adpositio casum, verbum
 * transitivum accusativum); caput = quod concordat (adiectivum,
 * determinans ad substantivum casu numero genere). */
nomen enumeratio {
    ORATIO_RELATIO_OBIECTUM = 0,
    ORATIO_RELATIO_CAPUT,
    /* T20a: relationes CLAUSULAE (umbrae e schemate generis):
     * verbum-finitum = verbum quod clausulam regit (forma finita),
     * subiectum = nominativus cum verbo concordans */
    ORATIO_RELATIO_VERBUM_FINITUM,
    ORATIO_RELATIO_SUBIECTUM,
    /* T31 b: obiectum VERBI (accusativum verbi transitivi) - relatio
     * propria ne regulae obiecti adpositionum (gradus I, relatio 0) eam
     * impleant; directio UD: verbum caput obiecti (obj) */
    ORATIO_RELATIO_OBIECTUM_VERBI,

    ORATIO_RELATIO_NUMERUS
} OratioRelatio;

/* DECISIO vocabuli (T19g 2026-09-08, decretum SUDOKU decisio XL):
 * quo genere decisionis lectio prima vocabuli facta sit - ab
 * exsecutore post gradus omnes in locum 'decisio' scripta cum
 * auctore (titulus regulae). Non scripta = NEMO decidit: cellula
 * APERTA si lectiones plures (ordo fontis manet), SOLUTA si una.
 * praelatio = ordo <praelatio> (prior classis, exceptio, regula
 * linguae: decisio MOLLIS, ordinat solum); impletio = ordo
 * <impletio> (lectio ligata praelata, carrier aut implens:
 * TESTIMONIUM); umbra = lex umbrarum (lectio umbris a vicino
 * impletis prima: TESTIMONIUM). APPENSA solum. */
nomen enumeratio {
    ORATIO_DECISIO_PRAELATIO = 0,
    ORATIO_DECISIO_IMPLETIO,
    ORATIO_DECISIO_UMBRA,

    ORATIO_DECISIO_NUMERUS
} OratioDecisio;

/* loci generis UMBRA (T19d): condiciones INDEX (non scriptae =
 * liberae), impletio INDEX ordinalibus sententiae (non scripta =
 * INVENTUM: dependens non repertus). Vide oratio-spec par. VII. */
nomen enumeratio {
    ORATIO_UMBRA_RELATIO = 0,        /* INDEX: OratioRelatio */
    ORATIO_UMBRA_CLASSIS,            /* INDEX: OratioClassis exspectata (absens = quaelibet) */
    ORATIO_UMBRA_CASUS,              /* INDEX: OratioCasus */
    ORATIO_UMBRA_NUMERUS,            /* INDEX: OratioNumerusGrammaticus */
    ORATIO_UMBRA_GENUS,              /* INDEX: OratioGenusGrammaticum */
        ORATIO_UMBRA_IMPLETIO_VOCABULUM, /* INDEX: ordinalis vocabuli implentis intra elementa */
    ORATIO_UMBRA_IMPLETIO_ANALYSIS,  /* INDEX: ordinalis analysis implentis intra analyses */
    /* T20a: condicio FORMAE VERBI (finitum | infinitivum | ...) -
     * valor unus finitatem dicit (modus tres valores poscerret);
     * APPENSA post impletionem, non scripta = quaelibet */
    ORATIO_UMBRA_FORMA_VERBI,        /* INDEX: OratioFormaVerbi exspectata */

    ORATIO_UMBRA_NUMERUS_LOCORUM
} OratioUmbraLocus;

/* loci generis CLAUSULA (T20a, 2026-09-08, decisiones XLIII-XLIV):
 * semen = ordinalis elementi quod clausulam aperuit (non scriptum =
 * principalis sine semine), species, pater = clausula continens
 * intra listam sententiae (non scriptus = summa; coordinatae
 * sorores, numquam nidificatae), umbrae = nodi umbra e SCHEMATE
 * generis nati, non regula: verbum-finitum (forma finita) et
 * subiectum (nominativus); impletio per legem exsecutoris unam
 * (T20c), vacua = inventum. */
nomen enumeratio {
    ORATIO_CLAUSULA_SEMEN = 0,       /* INDEX?: ordinalis elementi seminis */
    ORATIO_CLAUSULA_SPECIES,         /* INDEX: OratioSpeciesClausulae */
    ORATIO_CLAUSULA_PATER,           /* INDEX?: clausula continens */
    ORATIO_CLAUSULA_UMBRAE,          /* LISTA_NODUS umbra */

    ORATIO_CLAUSULA_NUMERUS_LOCORUM
} OratioClausulaLocus;

/* species clausulae (T20a): APPENSAE solum */
nomen enumeratio {
    ORATIO_SPECIES_CLAUSULAE_PRINCIPALIS = 0,
    ORATIO_SPECIES_CLAUSULAE_SUBORDINATA,
    ORATIO_SPECIES_CLAUSULAE_RELATIVA,
    ORATIO_SPECIES_CLAUSULAE_COORDINATA,
    ORATIO_SPECIES_CLAUSULAE_PARENTHETICA,

    ORATIO_SPECIES_CLAUSULAE_NUMERUS
} OratioSpeciesClausulae;

/* CAUSA positionis elementi in clausula (T20a, decisio XLV): stratum
 * quod elementum posuit - semen (elementum ipsum clausulam aperuit),
 * extentum (inter semina, acervo), clausura (post verbum finitum
 * certum clausulae subordinatae ad parentem redditum - prior verbi
 * finalis), catena (per umbram ligatam, T20b), unica (sententia sine
 * semine ullo: clausula una). Non scripta = APERTA. Oraculum stratum
 * quodque seorsum metitur. APPENSAE solum. */
nomen enumeratio {
    ORATIO_CLAUSULA_CAUSA_SEMEN = 0,
    ORATIO_CLAUSULA_CAUSA_EXTENTUM,
    ORATIO_CLAUSULA_CAUSA_CLAUSURA,
        ORATIO_CLAUSULA_CAUSA_CATENA,
    ORATIO_CLAUSULA_CAUSA_UNICA,
    /* T20a bis: SCISSIO verbi - verbum finitum certum alterum in
     * clausula quae suum iam habet clausulam coordinatam novam aperit
     * (asyndeton = semen absens, inventum); verbum ipsum et verba
     * sequentia usque ad semen proximum hanc causam ferunt */
    ORATIO_CLAUSULA_CAUSA_VERBUM,

    ORATIO_CLAUSULA_CAUSA_NUMERUS
} OratioClausulaCausa;

/* loci COMMUNES omnis generis analysis-* (primi V, ordine hoc) */
nomen enumeratio {
    ORATIO_ANALYSIS_LEMMA = 0,        /* TOKEN DERIVATUM: lemma fontis */
    ORATIO_ANALYSIS_LINGUA,           /* INDEX: OratioLingua */
    ORATIO_ANALYSIS_FONS,             /* INDEX: OratioFonsAnalysis */
    ORATIO_ANALYSIS_NATIVUM,          /* TOKEN DERIVATUM: codex fontis verbatim (lexema UNUM, decisio Frani) */
    ORATIO_ANALYSIS_SENSUS,           /* TOKEN? DERIVATUM: sensus fontis */

    ORATIO_ANALYSIS_COMMUNIA_NUMERUS
} OratioLocusAnalysis;

/* accidentia analysis-substantivi (et analysis-nominis-proprii) (INDEX omnia) */
nomen enumeratio {
    ORATIO_ANALYSIS_SUBSTANTIVI_CASUS =
        ORATIO_ANALYSIS_COMMUNIA_NUMERUS,
    ORATIO_ANALYSIS_SUBSTANTIVI_NUMERUS,
    ORATIO_ANALYSIS_SUBSTANTIVI_GENUS,
    ORATIO_ANALYSIS_SUBSTANTIVI_DECLINATIO,

        ORATIO_ANALYSIS_SUBSTANTIVI_UMBRAE,   /* LISTA_NODUS: umbrae (T19d) */

        ORATIO_ANALYSIS_SUBSTANTIVI_NUMERUS_LOCORUM
} OratioLocusAnalysisSubstantivi;

/* accidentia analysis-verbi (et analysis-auxiliaris) (INDEX omnia) */
nomen enumeratio {
    ORATIO_ANALYSIS_VERBI_PERSONA = ORATIO_ANALYSIS_COMMUNIA_NUMERUS,
    ORATIO_ANALYSIS_VERBI_NUMERUS,
    ORATIO_ANALYSIS_VERBI_TEMPUS,
    ORATIO_ANALYSIS_VERBI_MODUS,
    ORATIO_ANALYSIS_VERBI_VOX,
    ORATIO_ANALYSIS_VERBI_FORMA_VERBI,
    ORATIO_ANALYSIS_VERBI_CONIUGATIO,
    ORATIO_ANALYSIS_VERBI_CASUS,
    ORATIO_ANALYSIS_VERBI_GENUS,

        ORATIO_ANALYSIS_VERBI_UMBRAE,   /* LISTA_NODUS: umbrae (T19d) */

        ORATIO_ANALYSIS_VERBI_NUMERUS_LOCORUM
} OratioLocusAnalysisVerbi;

/* accidentia analysis-adiectivi (INDEX omnia) */
nomen enumeratio {
    ORATIO_ANALYSIS_ADIECTIVI_CASUS = ORATIO_ANALYSIS_COMMUNIA_NUMERUS,
    ORATIO_ANALYSIS_ADIECTIVI_NUMERUS,
    ORATIO_ANALYSIS_ADIECTIVI_GENUS,
    ORATIO_ANALYSIS_ADIECTIVI_GRADUS,
    ORATIO_ANALYSIS_ADIECTIVI_DECLINATIO,

        ORATIO_ANALYSIS_ADIECTIVI_UMBRAE,   /* LISTA_NODUS: umbrae (T19d) */

        ORATIO_ANALYSIS_ADIECTIVI_NUMERUS_LOCORUM
} OratioLocusAnalysisAdiectivi;

/* accidentia analysis-adverbii (INDEX omnia) */
nomen enumeratio {
    ORATIO_ANALYSIS_ADVERBII_GRADUS = ORATIO_ANALYSIS_COMMUNIA_NUMERUS,

        ORATIO_ANALYSIS_ADVERBII_UMBRAE,   /* LISTA_NODUS: umbrae (T19d) */

        ORATIO_ANALYSIS_ADVERBII_NUMERUS_LOCORUM
} OratioLocusAnalysisAdverbii;

/* accidentia analysis-pronominis (INDEX omnia) */
nomen enumeratio {
    ORATIO_ANALYSIS_PRONOMINIS_CASUS = ORATIO_ANALYSIS_COMMUNIA_NUMERUS,
    ORATIO_ANALYSIS_PRONOMINIS_NUMERUS,
    ORATIO_ANALYSIS_PRONOMINIS_GENUS,
    ORATIO_ANALYSIS_PRONOMINIS_PERSONA,

        ORATIO_ANALYSIS_PRONOMINIS_UMBRAE,   /* LISTA_NODUS: umbrae (T19d) */

        ORATIO_ANALYSIS_PRONOMINIS_NUMERUS_LOCORUM
} OratioLocusAnalysisPronominis;

/* accidentia analysis-determinantis (INDEX omnia) */
nomen enumeratio {
    ORATIO_ANALYSIS_DETERMINANTIS_CASUS =
        ORATIO_ANALYSIS_COMMUNIA_NUMERUS,
    ORATIO_ANALYSIS_DETERMINANTIS_NUMERUS,
    ORATIO_ANALYSIS_DETERMINANTIS_GENUS,

        ORATIO_ANALYSIS_DETERMINANTIS_UMBRAE,   /* LISTA_NODUS: umbrae (T19d) */

        ORATIO_ANALYSIS_DETERMINANTIS_NUMERUS_LOCORUM
} OratioLocusAnalysisDeterminantis;

/* accidentia analysis-adpositionis (INDEX omnia) */
nomen enumeratio {
    ORATIO_ANALYSIS_ADPOSITIONIS_CASUS =
        ORATIO_ANALYSIS_COMMUNIA_NUMERUS,

        ORATIO_ANALYSIS_ADPOSITIONIS_UMBRAE,   /* LISTA_NODUS: umbrae (T19d) */

        ORATIO_ANALYSIS_ADPOSITIONIS_NUMERUS_LOCORUM
} OratioLocusAnalysisAdpositionis;

/* accidentia analysis-numeralis (INDEX omnia) */
nomen enumeratio {
    ORATIO_ANALYSIS_NUMERALIS_CASUS = ORATIO_ANALYSIS_COMMUNIA_NUMERUS,
    ORATIO_ANALYSIS_NUMERALIS_NUMERUS,
    ORATIO_ANALYSIS_NUMERALIS_GENUS,
    ORATIO_ANALYSIS_NUMERALIS_SPECIES,

        ORATIO_ANALYSIS_NUMERALIS_UMBRAE,   /* LISTA_NODUS: umbrae (T19d) */

        ORATIO_ANALYSIS_NUMERALIS_NUMERUS_LOCORUM
} OratioLocusAnalysisNumeralis;

/* Tituli enumerationum (index = valor); mensurae == *_NUMERUS (porta). */
externus constans character* constans ORATIO_TITULI_CLASSIUM[];
externus constans character* constans ORATIO_TITULI_CASUUM[];
externus constans character* constans ORATIO_TITULI_NUMERORUM[];
externus constans character* constans ORATIO_TITULI_GENERUM_GRAMMATICORUM[];
externus constans character* constans ORATIO_TITULI_PERSONARUM[];
externus constans character* constans ORATIO_TITULI_TEMPORUM[];
externus constans character* constans ORATIO_TITULI_MODORUM[];
externus constans character* constans ORATIO_TITULI_VOCUM[];
externus constans character* constans ORATIO_TITULI_FORMARUM_VERBI[];
externus constans character* constans ORATIO_TITULI_GRADUUM[];
externus constans character* constans ORATIO_TITULI_SPECIERUM_NUMERALIS[];
externus constans character* constans ORATIO_TITULI_LINGUARUM[];
externus constans character* constans ORATIO_TITULI_DIALECTORUM[];   /* T22 b */
externus constans character* constans ORATIO_TITULI_RELATIONUM[];   /* T19d */
externus constans character* constans ORATIO_TITULI_DECISIONUM[];   /* T19g */
externus constans character* constans ORATIO_TITULI_SPECIERUM_CLAUSULAE[];   /* T20a */
externus constans character* constans ORATIO_TITULI_CAUSARUM_CLAUSULAE[];    /* T20a */
externus constans character* constans ORATIO_TITULI_FONTIUM_ANALYSIS[];

/* T20a: locus 'clausula' (causa FALSUM) aut 'clausula-causa' (causa
 * VERUM) generis elementi (vocabulum | interpunctio | numerus); -I si
 * genus non elementum. Una sedes pro stampa, proiectione, oraculo. */
s32
oratio_locus_clausulae (
    OratioGenus genus,
            b32 causa);

/* Titulus classis ("substantivum" ...); NIHIL si extra. */
constans character*
oratio_classis_titulus (
    OratioClassis classis);

/* Classis ex titulo; ORATIO_CLASSIS_NUMERUS_CLASSIUM si ignotus. */
OratioClassis
oratio_classis_ex_titulo (
    constans character* titulus,
                   i32  mensura);

/* Genus analysis-* classis (PRIMUM + classis). */
OratioGenus
oratio_classis_genus (
    OratioClassis classis);

/* Classis generis analysis-*; ORATIO_CLASSIS_NUMERUS_CLASSIUM si genus
 * analysis non est. */
OratioClassis
oratio_genus_classis (
    OratioGenus genus);

externus constans MateriaRegistrumCoctum ORATIO_REGISTRUM;

#endif /* ORATIO_REGISTRUM_H */
