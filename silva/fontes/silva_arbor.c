/* silva_arbor.c - vocabularium dialecti 'arbor' (T2)
 *
 * Vide silva_arbor.h pro contractu et ratione praefixi 'lex-'.
 */

#include "silva_arbor.h"
#include "chorda_aedificator.h"
#include "friatio.h"
#include <string.h>


/* ==================================================
 * Tabula orthographiae - FONS VERITATIS SECUNDUS
 *
 * Ordo POSITIONALIS est (index = genus), ergo quaesitio O(I).
 * Campus 'genus' redundans NON otiosus est: solus exsistit ut
 * silva_arbor_orthographia congruentiam positionis probare possit
 * et, si quis enumerationem ordinet neque hanc tabulam, RECUSET
 * potius quam mentiatur. Probatio idem trans genera OMNIA affirmat.
 *
 * NIHIL = orthographia VARIA (valor in documento portandus).
 * Vide portam in probatio_silva_arbor.c: orthographia CUIUSQUE
 * introitus lexatur et genus redditum exspectatur - haec tabula
 * a lexatore divergere non potest sine porta rubra.
 * ================================================== */

hic_manens constans structura {
      SilvaLexemaGenus  genus;
    constans character* orthographia;
} ORTHOGRAPHIAE[SILVA_LEX_NUMERUS_GENERUM] = {
    { SILVA_LEX_EOF,                   NIHIL },

    /* Robustitas - valor verbatim */
    { SILVA_LEX_OCTETUS_IGNOTUS,       NIHIL },
    { SILVA_LEX_STRING_IMPERFECTUM,    NIHIL },
    { SILVA_LEX_CHARACTER_IMPERFECTUM, NIHIL },

    /* Identificator et litterae - valor verbatim */
    { SILVA_LEX_IDENTIFICATOR,         NIHIL },
    { SILVA_LEX_INTEGER,               NIHIL },
    { SILVA_LEX_FLOAT,                 NIHIL },
    { SILVA_LEX_CHARACTER_LIT,         NIHIL },
    { SILVA_LEX_STRING_LIT,            NIHIL },

    /* Verba clausa C89 - XXXII */
    { SILVA_LEX_AUTO,                  "auto" },
    { SILVA_LEX_BREAK,                 "break" },
    { SILVA_LEX_CASE,                  "case" },
    { SILVA_LEX_CHAR,                  "char" },
    { SILVA_LEX_CONST,                 "const" },
    { SILVA_LEX_CONTINUE,              "continue" },
    { SILVA_LEX_DEFAULT,               "default" },
    { SILVA_LEX_DO,                    "do" },
    { SILVA_LEX_DOUBLE,                "double" },
    { SILVA_LEX_ELSE,                  "else" },
    { SILVA_LEX_ENUM,                  "enum" },
    { SILVA_LEX_EXTERN,                "extern" },
    { SILVA_LEX_FLOAT_KW,              "float" },
    { SILVA_LEX_FOR,                   "for" },
    { SILVA_LEX_GOTO,                  "goto" },
    { SILVA_LEX_IF,                    "if" },
    { SILVA_LEX_INT,                   "int" },
    { SILVA_LEX_LONG,                  "long" },
    { SILVA_LEX_REGISTER,              "register" },
    { SILVA_LEX_RETURN,                "return" },
    { SILVA_LEX_SHORT,                 "short" },
    { SILVA_LEX_SIGNED,                "signed" },
    { SILVA_LEX_SIZEOF,                "sizeof" },
    { SILVA_LEX_STATIC,                "static" },
    { SILVA_LEX_STRUCT,                "struct" },
    { SILVA_LEX_SWITCH,                "switch" },
    { SILVA_LEX_TYPEDEF,               "typedef" },
    { SILVA_LEX_UNION,                 "union" },
    { SILVA_LEX_UNSIGNED,              "unsigned" },
    { SILVA_LEX_VOID,                  "void" },
    { SILVA_LEX_VOLATILE,              "volatile" },
    { SILVA_LEX_WHILE,                 "while" },

    /* Interpunctiones - XLVIII */
    { SILVA_LEX_QUADRA_APERTA,         "[" },
    { SILVA_LEX_QUADRA_CLAUSA,         "]" },
    { SILVA_LEX_PAREN_APERTA,          "(" },
    { SILVA_LEX_PAREN_CLAUSA,          ")" },
    { SILVA_LEX_BRACE_APERTA,          "{" },
    { SILVA_LEX_BRACE_CLAUSA,          "}" },
    { SILVA_LEX_PUNCTUM,               "." },
    { SILVA_LEX_SAGITTA,               "->" },
    { SILVA_LEX_INCREMENTUM,           "++" },
    { SILVA_LEX_DECREMENTUM,           "--" },
    { SILVA_LEX_AMPERSAND,             "&" },
    { SILVA_LEX_STAR,                  "*" },
    { SILVA_LEX_PLUS,                  "+" },
    { SILVA_LEX_MINUS,                 "-" },
    { SILVA_LEX_TILDE,                 "~" },
    { SILVA_LEX_EXCLAMATIO,            "!" },
    { SILVA_LEX_SOLIDUS,               "/" },
    { SILVA_LEX_PERCENTUM,             "%" },
    { SILVA_LEX_SINISTRORSUM,          "<<" },
    { SILVA_LEX_DEXTRORSUM,            ">>" },
    { SILVA_LEX_MINOR,                 "<" },
    { SILVA_LEX_MAIOR,                 ">" },
    { SILVA_LEX_MINOR_AEQUALIS,        "<=" },
    { SILVA_LEX_MAIOR_AEQUALIS,        ">=" },
    { SILVA_LEX_AEQUALIS_AEQUALIS,     "==" },
    { SILVA_LEX_NON_AEQUALIS,          "!=" },
    { SILVA_LEX_CARET,                 "^" },
    { SILVA_LEX_BARRA,                 "|" },
    { SILVA_LEX_ET_ET,                 "&&" },
    { SILVA_LEX_VEL_VEL,               "||" },
    { SILVA_LEX_QUAESTIO,              "?" },
    { SILVA_LEX_COLON,                 ":" },
    { SILVA_LEX_SEMICOLON,             ";" },
    { SILVA_LEX_ELLIPSIS,              "..." },
    { SILVA_LEX_ASSIGNATIO,            "=" },
    { SILVA_LEX_STAR_ASSIGNATIO,       "*=" },
    { SILVA_LEX_SOLIDUS_ASSIGNATIO,    "/=" },
    { SILVA_LEX_PERCENTUM_ASSIGNATIO,  "%=" },
    { SILVA_LEX_PLUS_ASSIGNATIO,       "+=" },
    { SILVA_LEX_MINUS_ASSIGNATIO,      "-=" },
    { SILVA_LEX_SINISTRORSUM_ASSIGNATIO, "<<=" },
    { SILVA_LEX_DEXTRORSUM_ASSIGNATIO, ">>=" },
    { SILVA_LEX_AMPERSAND_ASSIGNATIO,  "&=" },
    { SILVA_LEX_CARET_ASSIGNATIO,      "^=" },
    { SILVA_LEX_BARRA_ASSIGNATIO,      "|=" },
    { SILVA_LEX_COMMA,                 "," },
    { SILVA_LEX_CANCELLUM,             "#" },
    { SILVA_LEX_CANCELLUM_CANCELLUM,   "##" },

    /* Trivia - valor verbatim (numerus spatiorum, textus commenti) */
    { SILVA_LEX_SPATIA,                NIHIL },
    { SILVA_LEX_TABULAE,               NIHIL },
    { SILVA_LEX_NOVA_LINEA,            NIHIL },
    { SILVA_LEX_CONTINUATIO,           NIHIL },
    { SILVA_LEX_COMMENTUM_CLAUSUM,     NIHIL },
    { SILVA_LEX_COMMENTUM_LINEA,       NIHIL }
};

hic_manens constans character* HEX_CIFRAE = "0123456789abcdef";


/* ==================================================
 * Sigillum registri
 * ================================================== */

chorda
silva_arbor_sigillum (
                           Piscina* piscina,
     constans SilvaRegistrumCoctum* tabularium)
{
    ChordaAedificator* materia;
    ChordaAedificator* exitus;
               chorda  friandum;
               chorda  vacua;
                  i32  friatum;
                  i32  i;

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;

    si (piscina == NIHIL || tabularium == NIHIL)
    {
        redde vacua;
    }

    materia = chorda_aedificator_creare(piscina, 4096);
    si (materia == NIHIL)
    {
        redde vacua;
    }

    /* Separator post CAMPUM QUEMQUE: sine eo 'ab' + 'c' et 'a' +
     * 'bc' eandem materiam darent. */
    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans SilvaTabGenus* genus = &tabularium->genera[i];

        si (genus->titulus != NIHIL)
        {
            chorda_aedificator_appendere_literis(materia,
                genus->titulus);
        }
        chorda_aedificator_appendere_character(materia, '\n');
        chorda_aedificator_appendere_i32(materia, genus->loci_offset);
        chorda_aedificator_appendere_character(materia, '\n');
        chorda_aedificator_appendere_i32(materia, genus->loci_numerus);
        chorda_aedificator_appendere_character(materia, '\n');
    }

    per (i = ZEPHYRUM; i < tabularium->numerus_locorum; i++)
    {
        constans SilvaTabLocus* locus = &tabularium->loci[i];

        si (locus->titulus != NIHIL)
        {
            chorda_aedificator_appendere_literis(materia,
                locus->titulus);
        }
        chorda_aedificator_appendere_character(materia, '\n');
        chorda_aedificator_appendere_s32(materia, locus->species);
        chorda_aedificator_appendere_character(materia, '\n');
    }

    friandum = chorda_aedificator_spectare(materia);
    friatum  = friatio_fnv1a_literis(
        (constans character*)friandum.datum, friandum.mensura);
    chorda_aedificator_destruere(materia);

    /* Hexadecimale VIII characterum, ante-implitum. NON per
     * chorda_aedificator_appendere_hex_i32: illud '%x' adhibet, ergo
     * longitudinem VARIAM dat (sigillum 0x0000abcd 'abcd' fieret) -
     * sigilla oculo conferenda longitudinem fixam petunt. */
    exitus = chorda_aedificator_creare(piscina,
        SILVA_ARBOR_SIGILLI_LONGITUDO + I);
    si (exitus == NIHIL)
    {
        redde vacua;
    }

    per (i = ZEPHYRUM; i < SILVA_ARBOR_SIGILLI_LONGITUDO; i++)
    {
        i32 gradus;
        i32 nibble;

        gradus = (SILVA_ARBOR_SIGILLI_LONGITUDO - I - i) * IV;
        nibble = (friatum >> gradus) & (i32)0xF;
        chorda_aedificator_appendere_character(exitus,
            HEX_CIFRAE[nibble]);
    }

    redde chorda_aedificator_finire(exitus);
}


/* ==================================================
 * Quaesitiones nominum registri
 *
 * OSTIUM NOMINATUM: haec QUINTA descriptio manu-voluta quaesitionis
 * nominis-in-indicem est. Priores: silva_scribere.c:76,
 * silva_commissio.c:471, silva_parsare.c:10, silva_quaestio.c:289
 * (cuius formam scandendi haec sequitur). Regula tertiae vicis iam
 * longe transgressa est - adiutor communis in silva_tabulae.h
 * promovendus, sed id OPUS M1 NON est: promotio quinque plagulas
 * exsistentes tangit et probationes suas petit.
 * ================================================== */

s32
silva_arbor_genus_index (
     constans SilvaRegistrumCoctum* tabularium,
                constans character* titulus,
                               i32  mensura)
{
    i32 i;

    si (tabularium == NIHIL || titulus == NIHIL)
    {
        redde -I;
    }

    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans character* candidatus = tabularium->genera[i].titulus;

        si (   candidatus                                   != NIHIL
            && strlen(candidatus) == (size_t)mensura
            && memcmp(candidatus, titulus, (size_t)mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

s32
silva_arbor_locus_index (
     constans SilvaRegistrumCoctum* tabularium,
                               s32  genus_index,
                constans character* titulus,
                               i32  mensura)
{
    constans SilvaTabGenus* genus;
                       i32  i;

    si (tabularium == NIHIL || titulus == NIHIL)
    {
        redde -I;
    }
    si (   genus_index < ZEPHYRUM
        || (i32)genus_index >= tabularium->numerus_generum)
    {
        redde -I;
    }

    genus = &tabularium->genera[genus_index];

    per (i = ZEPHYRUM; i < genus->loci_numerus; i++)
    {
                       i32  absolutus = genus->loci_offset + i;
        constans character* candidatus;

        si (absolutus >= tabularium->numerus_locorum)
        {
            frange;
        }
        candidatus = tabularium->loci[absolutus].titulus;

        si (   candidatus                                   != NIHIL
            && strlen(candidatus) == (size_t)mensura
            && memcmp(candidatus, titulus, (size_t)mensura) == ZEPHYRUM)
        {
            redde (s32)absolutus;
        }
    }
    redde -I;
}


/* ==================================================
 * Orthographia lexematum
 * ================================================== */

constans character*
silva_arbor_orthographia (
    SilvaLexemaGenus genus)
{
    si ((i32)genus >= (i32)SILVA_LEX_NUMERUS_GENERUM)
    {
        redde NIHIL;
    }
    /* Custodia ordinis - vide caput tabulae. Congruentia fracta
     * RECUSAT; mentiri non licet. */
    si (ORTHOGRAPHIAE[genus].genus != genus)
    {
        redde NIHIL;
    }
    redde ORTHOGRAPHIAE[genus].orthographia;
}

b32
silva_arbor_valor_portandus (
    SilvaLexemaGenus genus)
{
    si ((i32)genus >= (i32)SILVA_LEX_NUMERUS_GENERUM)
    {
        redde FALSUM;
    }
    /* EOF orthographiam non habet NEQUE valorem - vacuus semper */
    si (genus == SILVA_LEX_EOF)
    {
        redde FALSUM;
    }
    redde (silva_arbor_orthographia(genus) == NIHIL) ? VERUM : FALSUM;
}


/* ==================================================
 * Mangulatio tagorum lexematum
 * ================================================== */

i32
silva_arbor_lexema_tag (
    SilvaLexemaGenus  genus,
           character* buffer,
                 i32  capacitas)
{
    constans character* titulus;
                   i32  praefixum;
                   i32  scripta;
                   i32  i;

    si (buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si ((i32)genus >= (i32)SILVA_LEX_NUMERUS_GENERUM)
    {
        redde ZEPHYRUM;
    }

    titulus = silva_lexema_genus_nomen(genus);
    si (titulus == NIHIL)
    {
        redde ZEPHYRUM;
    }

    praefixum  = (i32)strlen(SILVA_ARBOR_PRAEFIXUM);
    scripta    = praefixum + (i32)strlen(titulus);
    si (scripta + I > capacitas)
    {
        redde ZEPHYRUM;
    }

    memcpy(buffer, SILVA_ARBOR_PRAEFIXUM, (size_t)praefixum);
    per (i = ZEPHYRUM; titulus[i] != '\0'; i++)
    {
        character c = titulus[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c - 'A' + 'a');
        }
        alioquin si (c == '_')
        {
            c = '-';
        }
        buffer[praefixum + i] = c;
    }
    buffer[scripta] = '\0';
    redde scripta;
}

SilvaLexemaGenus
silva_arbor_lexema_ex_tag (
     constans character* tag,
                    i32  mensura)
{
    character buffer[SILVA_ARBOR_TAG_CAPACITAS];
          i32 i;

    si (tag == NIHIL || mensura == ZEPHYRUM)
    {
        redde SILVA_LEX_NUMERUS_GENERUM;
    }

    /* Per mangulationem ANTRORSAM - ergo directiones divergere
     * non possunt. Genera XCV sunt; haec quaesitio semel per
     * elementum lectionis fit, non in ansa arta. */
    per (i = ZEPHYRUM; i < (i32)SILVA_LEX_NUMERUS_GENERUM; i++)
    {
        i32 longitudo = silva_arbor_lexema_tag((SilvaLexemaGenus)i,
            buffer, SILVA_ARBOR_TAG_CAPACITAS);

        si (   longitudo                            == mensura
            && memcmp(buffer, tag, (size_t)mensura) == ZEPHYRUM)
        {
            redde (SilvaLexemaGenus)i;
        }
    }
    redde SILVA_LEX_NUMERUS_GENERUM;
}
