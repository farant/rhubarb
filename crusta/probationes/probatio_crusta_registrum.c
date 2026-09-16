/* probatio_crusta_registrum.c - Registrum, lexicon et dialectus crustae
 *
 * QUINQUE CUSTODIAE (exemplar probatio_html_registrum):
 *
 *  1. ORDO LEXICI enumerationem CrustaLexGenus sequi DEBET, quia
 *     materia genera ut INDICES tractat. Assertio per TITULOS fit,
 *     non per numeros: permutatio ergo capitur, non absorbetur.
 *  2. RANCOR tabularum coctarum: crusta_registrum_coctum.{h,c} ex
 *     crusta.registrum.stml GENERATAE - redditio in memoria contra
 *     plagulas commissas OCTETIM (materia_registrum_recens).
 *  3. OFFSETS LOCORUM CONTIGUI, et LOCI NOMINATI (enumerationes
 *     crusta_registrum.h) contra titulos tabulae - ne tabula tertia
 *     manu scripta per se labatur. Genera quae enumerationem communem
 *     habent hic SEORSUM enumerantur (tabula ex declaratione
 *     generata: scratchpad gen_registrum.py in P1; eodem modo
 *     regenera).
 *  4. LINEA NOVA BIS (C6): SEPARATOR_LINEAE substantiva, LINEA
 *     trivium; munus LINEA ADEST (responsum md), capacitas
 *     linea-sensitiva ergo CONCEDITUR - assertum POSITIVE.
 *  5. DIALECTUS: tabulae NIHIL terminatae, sine duplicatis, verba
 *     nota praesentia; arithmetici VACUI usque ad P4.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_registrum.h"
#include "crusta_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_coctor.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Quaesitio litterarum in chorda SINE fine NUL */
interior b32
_textus_continet (
                 chorda  textus,
     constans character* litterae)
{
    i32 mensura = (i32)strlen(litterae);
    i32 i;

    si (mensura == ZEPHYRUM || textus.mensura < mensura)
    { redde FALSUM;
    }
    per (i = ZEPHYRUM; i + mensura <= textus.mensura; i++)
    {
        si (memcmp(textus.datum + i, litterae, (size_t)mensura)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Tabula dialecti: NIHIL terminata, sine duplicatis; numerum reddit,
 * -I si duplicatum */
interior s32
_tabula_sana (
    constans character* constans* tabula)
{
    s32 n = ZEPHYRUM;
    s32 i;
    s32 j;

    dum (tabula[n] != NIHIL)
    {
        n++;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        per (j = i + I; j < n; j++)
        {
            si (strcmp(tabula[i], tabula[j]) == ZEPHYRUM)
            {
                redde (s32)-I;
            }
        }
    }
    redde n;
}

interior b32
_tabula_habet (
    constans character* constans* tabula,
    constans character*           verbum)
{
    s32 i;

    per (i = ZEPHYRUM; tabula[i] != NIHIL; i++)
    {
        si (strcmp(tabula[i], verbum) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Nomina generum lexicalium, ordine CrustaLexGenus. Fons veritatis
 * SECUNDUS consulto: si crusta_lexicon.h permutetur, haec lista et
 * lexicon DISCREPABUNT et probatio cadet. */
hic_manens constans character* ORDO_EXSPECTATUS[] = {
    "FINIS",
    "RESERVATUM",
    "ASSIGNATIO_TITULUS",
    "SUBSCRIPTUM",
    "ASSIGNATIO_OPERATOR",
    "REDIRECTIO_FD",
    "REDIRECTIO_OPERATOR",
    "PIPA",
    "PIPA_ERRORIS",
    "ET",
    "AUT",
    "SEPARATOR",
    "SEPARATOR_LINEAE",
    "TERMINATOR_OPTIONIS",
    "PARENTHESIS",
    "PARENTHESIS_CLAUSURA",
    "ARITHMETICA_APERTURA",
    "ARITHMETICA_CLAUSURA",
    "IUDICIUM_APERTURA",
    "IUDICIUM_CLAUSURA",
    "LITTERALIS",
    "EFFUGIUM",
    "CONTINUATIO",
    "SIMPLEX",
    "GEMINA_APERTURA",
    "GEMINA_CLAUSURA",
    "EFFUGIA",
    "VERSA_APERTURA",
    "PARAMETRUM_SIGILLUM",
    "PARAMETRUM_TITULUS",
    "EXPANSIO_APERTURA",
    "EXPANSIO_PRAEFIXUM",
    "EXPANSIO_OPERATOR",
    "EXPANSIO_CLAUSURA",
    "SUBSTITUTIO_APERTURA",
    "GRAVIS",
    "ARITHMETICA_PARTIS_APERTURA",
    "PROCESSUS_APERTURA",
    "DOMUS",
    "HEREDOC_DELIMITATOR",
    "NUMERUS",
    "VARIABILIS",
    "ARITHMETICA_OPERATOR",
    "ARITHMETICA_SEPARATOR",
    "IUDICIUM_OPERATOR",
    "REGULA",
    "SPATIUM",
    "COMMENTUM",
    "LINEA",
    "LAMINA"
};

hic_manens constans character* GENERA_EXSPECTATA[] = {
    "programma",
    "separator",
    "malum",
    "heredoc",
    "catena",
    "pipa",
    "operator",
    "imperium",
    "assignatio",
    "tabulatum",
    "redirectio",
    "functio",
    "grex",
    "crustula",
    "conditio",
    "ramus",
    "iteratio",
    "cyclus",
    "repetitio",
    "cursus",
    "electio",
    "optio",
    "arithmetica",
    "iudicium",
    "socius",
    "verbum",
    "pars-litteralis",
    "pars-effugium",
    "pars-continuatio",
    "pars-simplex",
    "pars-gemina",
    "pars-effugia",
    "pars-versa",
    "pars-parametrum",
    "pars-expansio",
    "pars-substitutio",
    "pars-arithmetica",
    "pars-processus",
    "pars-domus",
    "numerus",
    "variabilis",
    "praeposita",
    "postposita",
    "binaria",
    "ternaria",
    "inclusa",
    "iudicium-praeposita",
    "iudicium-binaria",
    "iudicium-coniuncta",
    "iudicium-inclusa"
};

/* Locus NOMINATUS: enumeratio crusta_registrum.h contra TITULOS
 * tabulae (CLII ordines, ex declaratione generati; P6 duo
 * addidit: iteratio.interiecta, electio.interiecta, et separator
 * iterationis/cycli lista facta). */
nomen structura {
                    s32  genus;
                    i32  locus;
     constans character* titulus;
} LocusNominatus;

hic_manens constans LocusNominatus LOCI_NOMINATI[] = {
    { (s32)CRUSTA_GENUS_PROGRAMMA,
        (i32)CRUSTA_PROGRAMMA_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_PROGRAMMA,
        (i32)CRUSTA_PROGRAMMA_CAUDA, "cauda" },
    { (s32)CRUSTA_GENUS_SEPARATOR,
        (i32)CRUSTA_SEPARATOR_TOK, "tok" },
    { (s32)CRUSTA_GENUS_MALUM,
        (i32)CRUSTA_MALUM_TOKENS, "tokens" },
    { (s32)CRUSTA_GENUS_HEREDOC,
        (i32)CRUSTA_HEREDOC_PARTES, "partes" },
    { (s32)CRUSTA_GENUS_HEREDOC,
        (i32)CRUSTA_HEREDOC_TOK_DELIMITATOR, "tok_delimitator" },
    { (s32)CRUSTA_GENUS_HEREDOC,
        (i32)CRUSTA_HEREDOC_TOK_FINIS, "tok_finis" },
    { (s32)CRUSTA_GENUS_CATENA,
        (i32)CRUSTA_CATENA_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_PIPA,
        (i32)CRUSTA_PIPA_PRAEFIXA, "praefixa" },
    { (s32)CRUSTA_GENUS_PIPA,
        (i32)CRUSTA_PIPA_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_OPERATOR,
        (i32)CRUSTA_OPERATOR_TOK, "tok" },
    { (s32)CRUSTA_GENUS_IMPERIUM,
        (i32)CRUSTA_IMPERIUM_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_ASSIGNATIO,
        (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS, "tok_titulus" },
    { (s32)CRUSTA_GENUS_ASSIGNATIO,
        (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM, "tok_subscriptum" },
    { (s32)CRUSTA_GENUS_ASSIGNATIO,
        (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_ASSIGNATIO,
        (i32)CRUSTA_ASSIGNATIO_VALOR, "valor" },
    { (s32)CRUSTA_GENUS_TABULATUM,
        (i32)CRUSTA_TABULATUM_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_TABULATUM,
        (i32)CRUSTA_TABULATUM_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_TABULATUM,
        (i32)CRUSTA_TABULATUM_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_REDIRECTIO,
        (i32)CRUSTA_REDIRECTIO_TOK_FD, "tok_fd" },
    { (s32)CRUSTA_GENUS_REDIRECTIO,
        (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_REDIRECTIO,
        (i32)CRUSTA_REDIRECTIO_SCOPUS, "scopus" },
    { (s32)CRUSTA_GENUS_REDIRECTIO,
        (i32)CRUSTA_REDIRECTIO_CORPUS, "corpus" },
    { (s32)CRUSTA_GENUS_FUNCTIO,
        (i32)CRUSTA_FUNCTIO_TOK_VERBUM, "tok_verbum" },
    { (s32)CRUSTA_GENUS_FUNCTIO,
        (i32)CRUSTA_FUNCTIO_TOK_TITULUS, "tok_titulus" },
    { (s32)CRUSTA_GENUS_FUNCTIO,
        (i32)CRUSTA_FUNCTIO_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_FUNCTIO,
        (i32)CRUSTA_FUNCTIO_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_FUNCTIO,
        (i32)CRUSTA_FUNCTIO_CORPUS, "corpus" },
    { (s32)CRUSTA_GENUS_GREX,
        (i32)CRUSTA_GREX_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_GREX,
        (i32)CRUSTA_GREX_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_GREX,
        (i32)CRUSTA_GREX_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_GREX,
        (i32)CRUSTA_GREX_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_CRUSTULA,
        (i32)CRUSTA_CRUSTULA_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_CRUSTULA,
        (i32)CRUSTA_CRUSTULA_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_CRUSTULA,
        (i32)CRUSTA_CRUSTULA_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_CRUSTULA,
        (i32)CRUSTA_CRUSTULA_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_CONDITIO,
        (i32)CRUSTA_CONDITIO_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_CONDITIO,
        (i32)CRUSTA_CONDITIO_PROBATIO, "probatio" },
    { (s32)CRUSTA_GENUS_CONDITIO,
        (i32)CRUSTA_CONDITIO_TOK_DEINDE, "tok_deinde" },
    { (s32)CRUSTA_GENUS_CONDITIO,
        (i32)CRUSTA_CONDITIO_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_CONDITIO,
        (i32)CRUSTA_CONDITIO_RAMI, "rami" },
    { (s32)CRUSTA_GENUS_CONDITIO,
        (i32)CRUSTA_CONDITIO_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_CONDITIO,
        (i32)CRUSTA_CONDITIO_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_RAMUS,
        (i32)CRUSTA_RAMUS_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_RAMUS,
        (i32)CRUSTA_RAMUS_PROBATIO, "probatio" },
    { (s32)CRUSTA_GENUS_RAMUS,
        (i32)CRUSTA_RAMUS_TOK_DEINDE, "tok_deinde" },
    { (s32)CRUSTA_GENUS_RAMUS,
        (i32)CRUSTA_RAMUS_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_ITERATIO,
        (i32)CRUSTA_ITERATIO_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_ITERATIO,
        (i32)CRUSTA_ITERATIO_TOK_TITULUS, "tok_titulus" },
    { (s32)CRUSTA_GENUS_ITERATIO,
        (i32)CRUSTA_ITERATIO_INTERIECTA, "interiecta" },
    { (s32)CRUSTA_GENUS_ITERATIO,
        (i32)CRUSTA_ITERATIO_TOK_IN, "tok_in" },
    { (s32)CRUSTA_GENUS_ITERATIO,
        (i32)CRUSTA_ITERATIO_VERBA, "verba" },
    { (s32)CRUSTA_GENUS_ITERATIO,
        (i32)CRUSTA_ITERATIO_SEPARATOR, "separator" },
    { (s32)CRUSTA_GENUS_ITERATIO,
        (i32)CRUSTA_ITERATIO_CURSUS, "cursus" },
    { (s32)CRUSTA_GENUS_ITERATIO,
        (i32)CRUSTA_ITERATIO_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_CYCLUS,
        (i32)CRUSTA_CYCLUS_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_CYCLUS,
        (i32)CRUSTA_CYCLUS_TOK_PARENTHESIS, "tok_parenthesis" },
    { (s32)CRUSTA_GENUS_CYCLUS,
        (i32)CRUSTA_CYCLUS_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_CYCLUS,
        (i32)CRUSTA_CYCLUS_TOK_PARENTHESIS_CLAUSURA,
            "tok_parenthesis_clausura" },
    { (s32)CRUSTA_GENUS_CYCLUS,
        (i32)CRUSTA_CYCLUS_SEPARATOR, "separator" },
    { (s32)CRUSTA_GENUS_CYCLUS,
        (i32)CRUSTA_CYCLUS_CURSUS, "cursus" },
    { (s32)CRUSTA_GENUS_CYCLUS,
        (i32)CRUSTA_CYCLUS_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_REPETITIO,
        (i32)CRUSTA_REPETITIO_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_REPETITIO,
        (i32)CRUSTA_REPETITIO_PROBATIO, "probatio" },
    { (s32)CRUSTA_GENUS_REPETITIO,
        (i32)CRUSTA_REPETITIO_CURSUS, "cursus" },
    { (s32)CRUSTA_GENUS_REPETITIO,
        (i32)CRUSTA_REPETITIO_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_CURSUS,
        (i32)CRUSTA_CURSUS_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_CURSUS,
        (i32)CRUSTA_CURSUS_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_CURSUS,
        (i32)CRUSTA_CURSUS_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_ELECTIO,
        (i32)CRUSTA_ELECTIO_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_ELECTIO,
        (i32)CRUSTA_ELECTIO_VERBUM, "verbum" },
    { (s32)CRUSTA_GENUS_ELECTIO,
        (i32)CRUSTA_ELECTIO_INTERIECTA, "interiecta" },
    { (s32)CRUSTA_GENUS_ELECTIO,
        (i32)CRUSTA_ELECTIO_TOK_IN, "tok_in" },
    { (s32)CRUSTA_GENUS_ELECTIO,
        (i32)CRUSTA_ELECTIO_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_ELECTIO,
        (i32)CRUSTA_ELECTIO_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_ELECTIO,
        (i32)CRUSTA_ELECTIO_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_OPTIO,
        (i32)CRUSTA_OPTIO_TOK_PARENTHESIS, "tok_parenthesis" },
    { (s32)CRUSTA_GENUS_OPTIO,
        (i32)CRUSTA_OPTIO_EXEMPLARIA, "exemplaria" },
    { (s32)CRUSTA_GENUS_OPTIO,
        (i32)CRUSTA_OPTIO_TOK_PARENTHESIS_CLAUSURA,
            "tok_parenthesis_clausura" },
    { (s32)CRUSTA_GENUS_OPTIO,
        (i32)CRUSTA_OPTIO_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_OPTIO,
        (i32)CRUSTA_OPTIO_TOK_TERMINATOR, "tok_terminator" },
    { (s32)CRUSTA_GENUS_ARITHMETICA,
        (i32)CRUSTA_ARITHMETICA_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_ARITHMETICA,
        (i32)CRUSTA_ARITHMETICA_EXPRESSIO, "expressio" },
    { (s32)CRUSTA_GENUS_ARITHMETICA,
        (i32)CRUSTA_ARITHMETICA_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_ARITHMETICA,
        (i32)CRUSTA_ARITHMETICA_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_IUDICIUM,
        (i32)CRUSTA_IUDICIUM_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_IUDICIUM,
        (i32)CRUSTA_IUDICIUM_EXPRESSIO, "expressio" },
    { (s32)CRUSTA_GENUS_IUDICIUM,
        (i32)CRUSTA_IUDICIUM_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_IUDICIUM,
        (i32)CRUSTA_IUDICIUM_REDIRECTIONES, "redirectiones" },
    { (s32)CRUSTA_GENUS_SOCIUS,
        (i32)CRUSTA_SOCIUS_TOK_VERBUM, "tok_verbum" },
    { (s32)CRUSTA_GENUS_SOCIUS,
        (i32)CRUSTA_SOCIUS_TOK_TITULUS, "tok_titulus" },
    { (s32)CRUSTA_GENUS_SOCIUS,
        (i32)CRUSTA_SOCIUS_IMPERIUM, "imperium" },
    { (s32)CRUSTA_GENUS_VERBUM,
        (i32)CRUSTA_VERBUM_PARTES, "partes" },
    { (s32)CRUSTA_GENUS_PARS_LITTERALIS,
        (i32)CRUSTA_PARS_TOK, "tok" },
    { (s32)CRUSTA_GENUS_PARS_EFFUGIUM,
        (i32)CRUSTA_PARS_TOK, "tok" },
    { (s32)CRUSTA_GENUS_PARS_CONTINUATIO,
        (i32)CRUSTA_PARS_TOK, "tok" },
    { (s32)CRUSTA_GENUS_PARS_SIMPLEX,
        (i32)CRUSTA_PARS_TOK, "tok" },
    { (s32)CRUSTA_GENUS_PARS_GEMINA,
        (i32)CRUSTA_GEMINA_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_PARS_GEMINA,
        (i32)CRUSTA_GEMINA_PARTES, "partes" },
    { (s32)CRUSTA_GENUS_PARS_GEMINA,
        (i32)CRUSTA_GEMINA_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_PARS_EFFUGIA,
        (i32)CRUSTA_PARS_TOK, "tok" },
    { (s32)CRUSTA_GENUS_PARS_VERSA,
        (i32)CRUSTA_GEMINA_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_PARS_VERSA,
        (i32)CRUSTA_GEMINA_PARTES, "partes" },
    { (s32)CRUSTA_GENUS_PARS_VERSA,
        (i32)CRUSTA_GEMINA_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_PARS_PARAMETRUM,
        (i32)CRUSTA_PARAMETRUM_TOK_SIGILLUM, "tok_sigillum" },
    { (s32)CRUSTA_GENUS_PARS_PARAMETRUM,
        (i32)CRUSTA_PARAMETRUM_TOK_TITULUS, "tok_titulus" },
    { (s32)CRUSTA_GENUS_PARS_EXPANSIO,
        (i32)CRUSTA_EXPANSIO_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_PARS_EXPANSIO,
        (i32)CRUSTA_EXPANSIO_TOK_PRAEFIXUM, "tok_praefixum" },
    { (s32)CRUSTA_GENUS_PARS_EXPANSIO,
        (i32)CRUSTA_EXPANSIO_TOK_TITULUS, "tok_titulus" },
    { (s32)CRUSTA_GENUS_PARS_EXPANSIO,
        (i32)CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM, "tok_subscriptum" },
    { (s32)CRUSTA_GENUS_PARS_EXPANSIO,
        (i32)CRUSTA_EXPANSIO_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_PARS_EXPANSIO,
        (i32)CRUSTA_EXPANSIO_ARGUMENTA, "argumenta" },
    { (s32)CRUSTA_GENUS_PARS_EXPANSIO,
        (i32)CRUSTA_EXPANSIO_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO,
        (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO,
        (i32)CRUSTA_SUBSTITUTIO_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO,
        (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_PARS_ARITHMETICA,
        (i32)CRUSTA_PARS_ARITHMETICA_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_PARS_ARITHMETICA,
        (i32)CRUSTA_PARS_ARITHMETICA_EXPRESSIO, "expressio" },
    { (s32)CRUSTA_GENUS_PARS_ARITHMETICA,
        (i32)CRUSTA_PARS_ARITHMETICA_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_PARS_PROCESSUS,
        (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_PARS_PROCESSUS,
        (i32)CRUSTA_SUBSTITUTIO_LIBERI, "liberi" },
    { (s32)CRUSTA_GENUS_PARS_PROCESSUS,
        (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_PARS_DOMUS,
        (i32)CRUSTA_PARS_TOK, "tok" },
    { (s32)CRUSTA_GENUS_NUMERUS,
        (i32)CRUSTA_NUMERUS_TOK, "tok" },
    { (s32)CRUSTA_GENUS_VARIABILIS,
        (i32)CRUSTA_VARIABILIS_TOK_TITULUS, "tok_titulus" },
    { (s32)CRUSTA_GENUS_VARIABILIS,
        (i32)CRUSTA_VARIABILIS_TOK_SUBSCRIPTUM, "tok_subscriptum" },
    { (s32)CRUSTA_GENUS_PRAEPOSITA,
        (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_PRAEPOSITA,
        (i32)CRUSTA_PRAEPOSITA_OPERANDUM, "operandum" },
    { (s32)CRUSTA_GENUS_POSTPOSITA,
        (i32)CRUSTA_POSTPOSITA_OPERANDUM, "operandum" },
    { (s32)CRUSTA_GENUS_POSTPOSITA,
        (i32)CRUSTA_POSTPOSITA_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_BINARIA,
        (i32)CRUSTA_BINARIA_SINISTER, "sinister" },
    { (s32)CRUSTA_GENUS_BINARIA,
        (i32)CRUSTA_BINARIA_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_BINARIA,
        (i32)CRUSTA_BINARIA_DEXTER, "dexter" },
    { (s32)CRUSTA_GENUS_TERNARIA,
        (i32)CRUSTA_TERNARIA_PROBATIO, "probatio" },
    { (s32)CRUSTA_GENUS_TERNARIA,
        (i32)CRUSTA_TERNARIA_TOK_QUAESTIO, "tok_quaestio" },
    { (s32)CRUSTA_GENUS_TERNARIA,
        (i32)CRUSTA_TERNARIA_SINISTER, "sinister" },
    { (s32)CRUSTA_GENUS_TERNARIA,
        (i32)CRUSTA_TERNARIA_TOK_COLON, "tok_colon" },
    { (s32)CRUSTA_GENUS_TERNARIA,
        (i32)CRUSTA_TERNARIA_DEXTER, "dexter" },
    { (s32)CRUSTA_GENUS_INCLUSA,
        (i32)CRUSTA_INCLUSA_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_INCLUSA,
        (i32)CRUSTA_INCLUSA_EXPRESSIO, "expressio" },
    { (s32)CRUSTA_GENUS_INCLUSA,
        (i32)CRUSTA_INCLUSA_TOK_CLAUSURA, "tok_clausura" },
    { (s32)CRUSTA_GENUS_IUDICIUM_PRAEPOSITA,
        (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_IUDICIUM_PRAEPOSITA,
        (i32)CRUSTA_PRAEPOSITA_OPERANDUM, "operandum" },
    { (s32)CRUSTA_GENUS_IUDICIUM_BINARIA,
        (i32)CRUSTA_BINARIA_SINISTER, "sinister" },
    { (s32)CRUSTA_GENUS_IUDICIUM_BINARIA,
        (i32)CRUSTA_BINARIA_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_IUDICIUM_BINARIA,
        (i32)CRUSTA_BINARIA_DEXTER, "dexter" },
    { (s32)CRUSTA_GENUS_IUDICIUM_CONIUNCTA,
        (i32)CRUSTA_BINARIA_SINISTER, "sinister" },
    { (s32)CRUSTA_GENUS_IUDICIUM_CONIUNCTA,
        (i32)CRUSTA_BINARIA_TOK_OPERATOR, "tok_operator" },
    { (s32)CRUSTA_GENUS_IUDICIUM_CONIUNCTA,
        (i32)CRUSTA_BINARIA_DEXTER, "dexter" },
    { (s32)CRUSTA_GENUS_IUDICIUM_INCLUSA,
        (i32)CRUSTA_INCLUSA_TOK_APERTURA, "tok_apertura" },
    { (s32)CRUSTA_GENUS_IUDICIUM_INCLUSA,
        (i32)CRUSTA_INCLUSA_EXPRESSIO, "expressio" },
    { (s32)CRUSTA_GENUS_IUDICIUM_INCLUSA,
        (i32)CRUSTA_INCLUSA_TOK_CLAUSURA, "tok_clausura" }
};

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_crusta_registrum",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: lexicon portam transit, et ORDINEM servat
     * ================================================== */

    {
        i32 i;

        imprimere("\n--- Probans lexicon crustae ---\n");

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum,
            &CRUSTA_LEXICON,
            &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);

        CREDO_AEQUALIS_I32 (CRUSTA_LEXICON.numerus_generum,
            (i32)CRUSTA_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)CRUSTA_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)CRUSTA_LEX_NUMERUS_GENERUM, (i32)L);

        /* ORDO per titulos - permutatio capitur */
        per (i = ZEPHYRUM; i < (i32)CRUSTA_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(&ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
        }

        /* Sedes notae */
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)CRUSTA_LEX_PIPA), (s32)MATERIA_LEX_FIXUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)CRUSTA_LEX_PIPA), "|") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)CRUSTA_LEX_ARITHMETICA_PARTIS_APERTURA), "$((")
            == ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)CRUSTA_LEX_SEPARATOR_LINEAE),
            (s32)MATERIA_LEX_TERMINATOR);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)CRUSTA_LEX_LINEA), (s32)MATERIA_LEX_TERMINATOR);
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)CRUSTA_LEX_LITTERALIS));
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)CRUSTA_LEX_HEREDOC_DELIMITATOR));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (s32)CRUSTA_LEX_PIPA));
    }


    /* ==================================================
     * PROBARE: linea nova bis (C6), trivia, munus LINEA ADEST
     * ================================================== */

    {
        i32 postulata;

        imprimere("\n--- Probans trivia et lineam novam bis ---\n");

        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)CRUSTA_LEX_SPATIUM));
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)CRUSTA_LEX_COMMENTUM));
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)CRUSTA_LEX_LINEA));
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)CRUSTA_LEX_LAMINA));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)CRUSTA_LEX_SEPARATOR_LINEAE));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)CRUSTA_LEX_SEPARATOR));
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (s32)CRUSTA_LEX_LINEA), (s32)MATERIA_MUNUS_LINEA);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (s32)CRUSTA_LEX_LAMINA), (s32)MATERIA_MUNUS_LAMINA);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (s32)CRUSTA_LEX_SEPARATOR_LINEAE),
            (s32)MATERIA_MUNUS_SUBSTANTIVUM);

        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LAMINA));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_COMMENTUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));

        /* Capacitas linea-sensitiva CONCEDITUR (contra html) */
        postulata = MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_SPATIUM)
                  | MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_LINEA);
        CREDO_VERUM (materia_lexicon_munera_habet(&ratum, postulata));
    }


    /* ==================================================
     * PROBARE: dialectus CRUSTA_BASH - tabulae sanae
     * ================================================== */

    {
        imprimere("\n--- Probans dialectum bash ---\n");

        CREDO_AEQUALIS_S32 (_tabula_sana(CRUSTA_BASH.reservata),
            (s32)XXII);
        CREDO_AEQUALIS_S32 (_tabula_sana(CRUSTA_BASH.aedificatores),
            (s32)VI);
        CREDO_AEQUALIS_S32 (_tabula_sana(CRUSTA_BASH.redirectiones),
            (s32)XII);
        CREDO_AEQUALIS_S32 (_tabula_sana(CRUSTA_BASH.optionum_termini),
            (s32)III);
        CREDO_AEQUALIS_S32 (
            _tabula_sana(CRUSTA_BASH.iudicii_praeposita), (s32)XXVI);
        CREDO_AEQUALIS_S32 (_tabula_sana(CRUSTA_BASH.iudicii_binaria),
            (s32)XV);
        CREDO_VERUM (_tabula_habet(CRUSTA_BASH.reservata, "if"));
        CREDO_VERUM (_tabula_habet(CRUSTA_BASH.reservata, "]]"));
        CREDO_VERUM (_tabula_habet(CRUSTA_BASH.reservata, "coproc"));
        CREDO_FALSUM (_tabula_habet(CRUSTA_BASH.reservata, "echo"));
        CREDO_VERUM (_tabula_habet(CRUSTA_BASH.redirectiones, "<<-"));
        CREDO_VERUM (_tabula_habet(CRUSTA_BASH.redirectiones, "&>>"));
        CREDO_VERUM (_tabula_habet(CRUSTA_BASH.aedificatores, "local"));
        CREDO_VERUM (_tabula_habet(CRUSTA_BASH.iudicii_binaria, "=~"));
        /* arithmetici: XLI ordines (P4), praecedentiae in [I, XVI] */
        {
            s32 n = ZEPHYRUM;

            dum (CRUSTA_BASH.arithmetici[n].titulus != NIHIL)
            {
                CREDO_VERUM (CRUSTA_BASH.arithmetici[n].praecedentia
                    >= (i32)I);
                CREDO_VERUM (CRUSTA_BASH.arithmetici[n].praecedentia
                    <= (i32)XVI);
                n++;
            }
            CREDO_AEQUALIS_S32 (n, (s32)XLI);
        }
    }


    /* ==================================================
     * PROBARE: porta rancoris tabularum coctarum
     * ================================================== */

    {
        constans character* radix = getenv("RHUBARB_RADIX");
             MateriaRancor  rancor;

        imprimere("\n--- Probans rancorem tabularum coctarum ---\n");
        si (!materia_registrum_recens(piscina, radix
            != NIHIL ? radix : ".",
                "crusta/grammatica/crusta.registrum.stml", &rancor))
        {
            imprimere("    recusatio: %.*s\n",
                (integer)rancor.causa.mensura,
                (constans character*)rancor.causa.datum);
            CREDO_CULPA ("declaratio absens aut recusata");
        }
        alioquin
        {
            si (!rancor.recens)
            {
                imprimere("    RANCIDUM: %.*s:%u\n",
                    (integer)rancor.via.mensura,
                    (constans character*)rancor.via.datum,
                    rancor.linea);
            }
            CREDO_VERUM (rancor.recens);
            CREDO_AEQUALIS_I32 (rancor.coctio.numerus_locorum,
                CRUSTA_REGISTRUM.numerus_locorum);
            CREDO_AEQUALIS_I32 (rancor.coctio.numerus_generum,
                CRUSTA_REGISTRUM.numerus_generum);
        }
    }


    /* ==================================================
     * PROBARE: registrum - ordo, CONTIGUITAS locorum, loci nominati
     * ================================================== */

    {
        i32 i;
        i32 exspectatus_offset;
        i32 numerus_nominatorum;

        imprimere("\n--- Probans registrum nodorum ---\n");

        CREDO_AEQUALIS_I32 (CRUSTA_REGISTRUM.numerus_generum,
            (i32)CRUSTA_GENUS_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)CRUSTA_GENUS_NUMERUS_GENERUM, (i32)L);
        CREDO_AEQUALIS_I32 (CRUSTA_REGISTRUM.numerus_locorum,
            (i32)CLII);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])),
            (i32)CRUSTA_GENUS_NUMERUS_GENERUM);

        per (i = ZEPHYRUM; i < (i32)CRUSTA_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_NON_NIHIL (CRUSTA_REGISTRUM.genera[i].titulus);
            CREDO_VERUM (strcmp(CRUSTA_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(
                &CRUSTA_REGISTRUM, GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
        }

        exspectatus_offset = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)CRUSTA_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_AEQUALIS_I32 (CRUSTA_REGISTRUM.genera[i].loci_offset,
                exspectatus_offset);
            CREDO_MAIOR_I32 (CRUSTA_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            exspectatus_offset
                += CRUSTA_REGISTRUM.genera[i].loci_numerus;
        }
        CREDO_AEQUALIS_I32 (exspectatus_offset,
            CRUSTA_REGISTRUM.numerus_locorum);

        per (i = ZEPHYRUM; i < CRUSTA_REGISTRUM.numerus_locorum; i++)
        {
            CREDO_NON_NIHIL (CRUSTA_REGISTRUM.loci[i].titulus);
            CREDO_MINOR_S32 (CRUSTA_REGISTRUM.loci[i].species,
                (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM);
        }

        /* referentia una: redirectio.corpus */
        CREDO_AEQUALIS_S32 (CRUSTA_REGISTRUM.loci[
            CRUSTA_REGISTRUM.genera[CRUSTA_GENUS_REDIRECTIO].loci_offset
            + (i32)CRUSTA_REDIRECTIO_CORPUS].species,
            (s32)MATERIA_LOCUS_REFERENTIA);

        numerus_nominatorum = (i32)(magnitudo(LOCI_NOMINATI)
            / magnitudo(LOCI_NOMINATI[0]));
        CREDO_AEQUALIS_I32 (numerus_nominatorum,
            CRUSTA_REGISTRUM.numerus_locorum);
        per (i = ZEPHYRUM; i < numerus_nominatorum; i++)
        {
            constans LocusNominatus* n = &LOCI_NOMINATI[i];
                                i32  absolutus;

            CREDO_MINOR_I32 (n->locus,
                CRUSTA_REGISTRUM.genera[n->genus].loci_numerus);
            absolutus = CRUSTA_REGISTRUM.genera[n->genus].loci_offset
                      + n->locus;
            CREDO_VERUM (strcmp(
                CRUSTA_REGISTRUM.loci[absolutus].titulus, n->titulus)
                == ZEPHYRUM);
        }
    }


    /* ==================================================
     * PROBARE: circuitus per materiam (arbor minima 'echo')
     * ================================================== */

    {
        MateriaArborConsilium c;
                 MateriaNodus* programma;
                 MateriaNodus* imperium;
                 MateriaNodus* verbum;
                 MateriaNodus* pars;
        MateriaArborScriptura  s1;
        MateriaArborScriptura  s2;
                 MateriaNodus* lecta;
           MateriaArborVitium  vitium;

        imprimere("\n--- Probans circuitum crustae per materiam ---\n");

        pars = materia_nodus_creare(piscina,
            (s32)CRUSTA_GENUS_PARS_LITTERALIS,
            CRUSTA_REGISTRUM.genera[CRUSTA_GENUS_PARS_LITTERALIS]
                .loci_numerus);
        CREDO_VERUM (materia_nodus_ponere(pars, (i32)CRUSTA_PARS_TOK,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)CRUSTA_LEX_LITTERALIS,
                chorda_ex_literis("echo", piscina), ZEPHYRUM, (i32)I,
                (i32)I, ZEPHYRUM)), MATERIA_LOCUS_TOKEN));
        verbum = materia_nodus_creare(piscina, (s32)CRUSTA_GENUS_VERBUM,
            CRUSTA_REGISTRUM.genera[CRUSTA_GENUS_VERBUM].loci_numerus);
        CREDO_VERUM (materia_nodus_appendere(piscina, verbum,
            (i32)CRUSTA_VERBUM_PARTES, materia_valor_nodus(pars),
            MATERIA_LOCUS_LISTA_NODUS));
        imperium = materia_nodus_creare(piscina,
            (s32)CRUSTA_GENUS_IMPERIUM,
            CRUSTA_REGISTRUM.genera[CRUSTA_GENUS_IMPERIUM]
                .loci_numerus);
        CREDO_VERUM (materia_nodus_appendere(piscina, imperium,
            (i32)CRUSTA_IMPERIUM_LIBERI, materia_valor_nodus(verbum),
            MATERIA_LOCUS_LISTA_NODUS));
        programma = materia_nodus_creare(piscina,
            (s32)CRUSTA_GENUS_PROGRAMMA,
            CRUSTA_REGISTRUM.genera[CRUSTA_GENUS_PROGRAMMA]
                .loci_numerus);
        CREDO_VERUM (materia_nodus_appendere(piscina, programma,
            (i32)CRUSTA_PROGRAMMA_LIBERI, materia_valor_nodus(imperium),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(programma,
            (i32)CRUSTA_PROGRAMMA_CAUDA,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)CRUSTA_LEX_FINIS,
                chorda_ex_literis("", piscina), (s32)IV, (i32)I,
                (i32)V, ZEPHYRUM)), MATERIA_LOCUS_TOKEN));

        materia_arbor_consilium_nudum(&c, &CRUSTA_REGISTRUM, &ratum,
            "crusta");
        s1 = materia_arbor_scribere_nodum(piscina, programma, &c);
        CREDO_VERUM (s1.successus);
        CREDO_VERUM (_textus_continet(s1.textus,
            "grammatica=\"crusta\""));
        CREDO_VERUM (_textus_continet(s1.textus, "<crusta-litteralis"));
        CREDO_VERUM (_textus_continet(s1.textus, "<pars-litteralis"));

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        CREDO_AEQUALIS_S32 (lecta->genus, (s32)CRUSTA_GENUS_PROGRAMMA);

        s2 = materia_arbor_scribere_nodum(piscina, lecta, &c);
        CREDO_VERUM (s2.successus);
        CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
