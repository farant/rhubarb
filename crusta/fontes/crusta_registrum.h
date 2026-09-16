/* crusta_registrum.h - Registrum generum crustae: caput generatum,
 * enumerationes locorum manu scriptae, dialectus
 *
 * CrustaGenus, CRUSTA_GENUS_NUMERUS_GENERUM et CRUSTA_REGISTRUM ex
 * crusta_registrum_coctum.h veniunt (GENERATUM ex
 * crusta/grammatica/crusta.registrum.stml per ./materia/coquere.sh).
 * Hic: enumerationes locorum per genus (ordo = ordo declarationis;
 * probatio_crusta_registrum quamque per titulos asserit) et
 * CrustaDialectus - tabulae grammaticae bash 5.2.15 optionum
 * ordinariarum (spec crusta-arbor-spec.md par. V; M7: tabula, numquam
 * programma; dialectus altera = instantia altera, numquam ramus).
 *
 * Genera quae formam locorum communem habent enumerationem communem
 * habent (commentarium socios nominat); tabula locorum nominatorum
 * portae registri quodque genus seorsum enumerat.
 */

#ifndef CRUSTA_REGISTRUM_H
#define CRUSTA_REGISTRUM_H

#include "latina.h"
#include "crusta_registrum_coctum.h"


/* ==================================================
 * Loci per genus
 * ================================================== */

nomen enumeratio { CRUSTA_PROGRAMMA_LIBERI = 0, CRUSTA_PROGRAMMA_CAUDA }
    CrustaProgrammaLocus;
nomen enumeratio { CRUSTA_SEPARATOR_TOK = 0 } CrustaSeparatorLocus;
nomen enumeratio { CRUSTA_MALUM_TOKENS = 0 } CrustaMalumLocus;
nomen enumeratio {
    CRUSTA_HEREDOC_PARTES = 0, CRUSTA_HEREDOC_TOK_DELIMITATOR,
    CRUSTA_HEREDOC_TOK_FINIS
} CrustaHeredocLocus;
nomen enumeratio { CRUSTA_CATENA_LIBERI = 0 } CrustaCatenaLocus;
nomen enumeratio { CRUSTA_PIPA_PRAEFIXA = 0, CRUSTA_PIPA_LIBERI }
    CrustaPipaLocus;
nomen enumeratio { CRUSTA_OPERATOR_TOK = 0 } CrustaOperatorLocus;
nomen enumeratio { CRUSTA_IMPERIUM_LIBERI = 0 } CrustaImperiumLocus;
nomen enumeratio {
    CRUSTA_ASSIGNATIO_TOK_TITULUS = 0,
        CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM,
    CRUSTA_ASSIGNATIO_TOK_OPERATOR, CRUSTA_ASSIGNATIO_VALOR
} CrustaAssignatioLocus;
nomen enumeratio {
    CRUSTA_TABULATUM_TOK_APERTURA = 0, CRUSTA_TABULATUM_LIBERI,
    CRUSTA_TABULATUM_TOK_CLAUSURA
} CrustaTabulatumLocus;
nomen enumeratio {
    CRUSTA_REDIRECTIO_TOK_FD = 0, CRUSTA_REDIRECTIO_TOK_OPERATOR,
    CRUSTA_REDIRECTIO_SCOPUS, CRUSTA_REDIRECTIO_CORPUS
} CrustaRedirectioLocus;
nomen enumeratio {
    CRUSTA_FUNCTIO_TOK_VERBUM = 0, CRUSTA_FUNCTIO_TOK_TITULUS,
    CRUSTA_FUNCTIO_TOK_APERTURA, CRUSTA_FUNCTIO_TOK_CLAUSURA,
    CRUSTA_FUNCTIO_CORPUS
} CrustaFunctioLocus;
/* grex et crustula: forma eadem */
nomen enumeratio {
    CRUSTA_GREX_TOK_APERTURA = 0, CRUSTA_GREX_LIBERI,
    CRUSTA_GREX_TOK_CLAUSURA, CRUSTA_GREX_REDIRECTIONES
} CrustaGrexLocus;
nomen enumeratio {
    CRUSTA_CRUSTULA_TOK_APERTURA = 0, CRUSTA_CRUSTULA_LIBERI,
    CRUSTA_CRUSTULA_TOK_CLAUSURA, CRUSTA_CRUSTULA_REDIRECTIONES
} CrustaCrustulaLocus;
nomen enumeratio {
    CRUSTA_CONDITIO_TOK_APERTURA = 0, CRUSTA_CONDITIO_PROBATIO,
    CRUSTA_CONDITIO_TOK_DEINDE, CRUSTA_CONDITIO_LIBERI,
    CRUSTA_CONDITIO_RAMI, CRUSTA_CONDITIO_TOK_CLAUSURA,
    CRUSTA_CONDITIO_REDIRECTIONES
} CrustaConditioLocus;
nomen enumeratio {
    CRUSTA_RAMUS_TOK_APERTURA = 0, CRUSTA_RAMUS_PROBATIO,
    CRUSTA_RAMUS_TOK_DEINDE, CRUSTA_RAMUS_LIBERI
} CrustaRamusLocus;
/* separator = LISTA (nodus separator, deinde corpora heredoc quae
 * lineam novam sequuntur); interiecta = corpora heredoc post titulum
 * ante 'in' (P6, corpus FreeBSD) */
nomen enumeratio {
    CRUSTA_ITERATIO_TOK_APERTURA = 0, CRUSTA_ITERATIO_TOK_TITULUS,
    CRUSTA_ITERATIO_INTERIECTA, CRUSTA_ITERATIO_TOK_IN,
    CRUSTA_ITERATIO_VERBA, CRUSTA_ITERATIO_SEPARATOR,
    CRUSTA_ITERATIO_CURSUS, CRUSTA_ITERATIO_REDIRECTIONES
} CrustaIteratioLocus;
nomen enumeratio {
    CRUSTA_CYCLUS_TOK_APERTURA = 0, CRUSTA_CYCLUS_TOK_PARENTHESIS,
    CRUSTA_CYCLUS_LIBERI, CRUSTA_CYCLUS_TOK_PARENTHESIS_CLAUSURA,
    CRUSTA_CYCLUS_SEPARATOR, CRUSTA_CYCLUS_CURSUS,
    CRUSTA_CYCLUS_REDIRECTIONES
} CrustaCyclusLocus;
nomen enumeratio {
    CRUSTA_REPETITIO_TOK_APERTURA = 0, CRUSTA_REPETITIO_PROBATIO,
    CRUSTA_REPETITIO_CURSUS, CRUSTA_REPETITIO_REDIRECTIONES
} CrustaRepetitioLocus;
nomen enumeratio {
    CRUSTA_CURSUS_TOK_APERTURA = 0, CRUSTA_CURSUS_LIBERI,
    CRUSTA_CURSUS_TOK_CLAUSURA
} CrustaCursusLocus;
nomen enumeratio {
    CRUSTA_ELECTIO_TOK_APERTURA = 0, CRUSTA_ELECTIO_VERBUM,
    CRUSTA_ELECTIO_INTERIECTA, CRUSTA_ELECTIO_TOK_IN,
    CRUSTA_ELECTIO_LIBERI, CRUSTA_ELECTIO_TOK_CLAUSURA,
    CRUSTA_ELECTIO_REDIRECTIONES
} CrustaElectioLocus;
nomen enumeratio {
    CRUSTA_OPTIO_TOK_PARENTHESIS = 0, CRUSTA_OPTIO_EXEMPLARIA,
    CRUSTA_OPTIO_TOK_PARENTHESIS_CLAUSURA, CRUSTA_OPTIO_LIBERI,
    CRUSTA_OPTIO_TOK_TERMINATOR
} CrustaOptioLocus;
/* arithmetica et iudicium: forma eadem */
nomen enumeratio {
    CRUSTA_ARITHMETICA_TOK_APERTURA = 0, CRUSTA_ARITHMETICA_EXPRESSIO,
    CRUSTA_ARITHMETICA_TOK_CLAUSURA, CRUSTA_ARITHMETICA_REDIRECTIONES
} CrustaArithmeticaLocus;
nomen enumeratio {
    CRUSTA_IUDICIUM_TOK_APERTURA = 0, CRUSTA_IUDICIUM_EXPRESSIO,
    CRUSTA_IUDICIUM_TOK_CLAUSURA, CRUSTA_IUDICIUM_REDIRECTIONES
} CrustaIudiciumLocus;
nomen enumeratio {
    CRUSTA_SOCIUS_TOK_VERBUM = 0, CRUSTA_SOCIUS_TOK_TITULUS,
    CRUSTA_SOCIUS_IMPERIUM
} CrustaSociusLocus;
nomen enumeratio { CRUSTA_VERBUM_PARTES = 0 } CrustaVerbumLocus;
/* pars-litteralis, -effugium, -continuatio, -simplex, -effugia,
 * -domus: locus unus 'tok' */
nomen enumeratio { CRUSTA_PARS_TOK = 0 } CrustaParsLocus;
/* pars-gemina et pars-versa */
nomen enumeratio {
    CRUSTA_GEMINA_TOK_APERTURA = 0, CRUSTA_GEMINA_PARTES,
    CRUSTA_GEMINA_TOK_CLAUSURA
} CrustaGeminaLocus;
nomen enumeratio {
    CRUSTA_PARAMETRUM_TOK_SIGILLUM = 0, CRUSTA_PARAMETRUM_TOK_TITULUS
} CrustaParametrumLocus;
nomen enumeratio {
    CRUSTA_EXPANSIO_TOK_APERTURA = 0, CRUSTA_EXPANSIO_TOK_PRAEFIXUM,
    CRUSTA_EXPANSIO_TOK_TITULUS, CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM,
    CRUSTA_EXPANSIO_TOK_OPERATOR, CRUSTA_EXPANSIO_ARGUMENTA,
    CRUSTA_EXPANSIO_TOK_CLAUSURA
} CrustaExpansioLocus;
/* pars-substitutio et pars-processus */
nomen enumeratio {
    CRUSTA_SUBSTITUTIO_TOK_APERTURA = 0, CRUSTA_SUBSTITUTIO_LIBERI,
    CRUSTA_SUBSTITUTIO_TOK_CLAUSURA
} CrustaSubstitutioLocus;
nomen enumeratio {
    CRUSTA_PARS_ARITHMETICA_TOK_APERTURA = 0,
    CRUSTA_PARS_ARITHMETICA_EXPRESSIO,
    CRUSTA_PARS_ARITHMETICA_TOK_CLAUSURA
} CrustaParsArithmeticaLocus;
nomen enumeratio { CRUSTA_NUMERUS_TOK = 0 } CrustaNumerusLocus;
nomen enumeratio {
    CRUSTA_VARIABILIS_TOK_TITULUS = 0, CRUSTA_VARIABILIS_TOK_SUBSCRIPTUM
} CrustaVariabilisLocus;
/* praeposita et iudicium-praeposita */
nomen enumeratio {
    CRUSTA_PRAEPOSITA_TOK_OPERATOR = 0, CRUSTA_PRAEPOSITA_OPERANDUM
} CrustaPraepositaLocus;
nomen enumeratio {
    CRUSTA_POSTPOSITA_OPERANDUM = 0, CRUSTA_POSTPOSITA_TOK_OPERATOR
} CrustaPostpositaLocus;
/* binaria, iudicium-binaria, iudicium-coniuncta */
nomen enumeratio {
    CRUSTA_BINARIA_SINISTER = 0, CRUSTA_BINARIA_TOK_OPERATOR,
    CRUSTA_BINARIA_DEXTER
} CrustaBinariaLocus;
nomen enumeratio {
    CRUSTA_TERNARIA_PROBATIO = 0, CRUSTA_TERNARIA_TOK_QUAESTIO,
    CRUSTA_TERNARIA_SINISTER, CRUSTA_TERNARIA_TOK_COLON,
    CRUSTA_TERNARIA_DEXTER
} CrustaTernariaLocus;
/* inclusa et iudicium-inclusa */
nomen enumeratio {
    CRUSTA_INCLUSA_TOK_APERTURA = 0, CRUSTA_INCLUSA_EXPRESSIO,
    CRUSTA_INCLUSA_TOK_CLAUSURA
} CrustaInclusaLocus;


/* ==================================================
 * Dialectus (M7: tabulae, NIHIL terminatae)
 * ================================================== */

nomen enumeratio {
    CRUSTA_OPERATOR_BINARIUS = 0,
    CRUSTA_OPERATOR_PRAEPOSITUS,
    CRUSTA_OPERATOR_POSTPOSITUS,
    CRUSTA_OPERATOR_TERNARIUS
} CrustaOperatorSpecies;

/* Operator arithmeticus: praecedentia maior artius ligat; dextra =
 * associatio dextra. Tabula titulo NIHIL terminatur. */
nomen structura {
       constans character* titulus;
                      i32  praecedentia;
                      b32  dextra;
    CrustaOperatorSpecies  species;
} CrustaOperator;

nomen structura {
    constans character* constans* reservata;
    constans character* constans* aedificatores;
    constans character* constans* redirectiones;
    constans character* constans* optionum_termini;
    constans CrustaOperator* arithmetici;
    constans character* constans* iudicii_praeposita;
    constans character* constans* iudicii_binaria;
} CrustaDialectus;

/* bash 5.2.15 optionum ordinariarum (in crusta_lexicon.c) */
externus constans CrustaDialectus CRUSTA_BASH;

#endif /* CRUSTA_REGISTRUM_H */
