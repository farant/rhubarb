/* silva_arbor.c - vocabularium dialecti 'arbor' (T2)
 *
 * Vide silva_arbor.h pro contractu et ratione praefixi 'lex-'.
 */

#include "silva_arbor.h"
#include "silva_frons.h"
#include "silva_commissio.h"
#include "chorda_aedificator.h"
#include "friatio.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "stml.h"
#include "stml_macros.h"
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

        si (   candidatus != NIHIL
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

        si (   candidatus != NIHIL
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


/* ==================================================
 * Scriptor parsurae - status C89 (silva-migratio T10c, 2026-09-25)
 *
 * Scriptor IPSE materiae est (sessio T10a, materia_arbor.h): passus
 * I/II, lexemata, trivia, fragmenta, templa, sedes valorum. Hic solum
 * quod documentum <parsura> C89 addit: fontes, reinserenda, ancorae
 * per liberum supremum, compressio (folia macronum et familiae
 * parametrorum - post-passus C89, decretum ...MQF). Ambulatio vetus
 * (MDXXX lineae) deleta.
 * ================================================== */

nomen structura {
                          Piscina* piscina;
              InternamentumChorda* intern;
             MateriaArborScriptor* sessio;
    SilvaArborCensusCompressionis  census;
               constans character* causa;
} ArborScriptor;

/* Decimale sine stdio (snprintf C99 est; postulata_posix hic
 * pretium non meretur pro numeris parvis) */
interior i32
_silvae_numerus_ad_literas (
          i32  numerus,
    character* buffer,
          i32  capacitas)
{
    character inversa[XVI];
          i32 longitudo;
          i32 i;

    si (buffer == NIHIL || capacitas < II)
    {
        redde ZEPHYRUM;
    }
    si (numerus == ZEPHYRUM)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        redde I;
    }

    longitudo = ZEPHYRUM;
    dum (numerus > ZEPHYRUM && longitudo < (i32)magnitudo(inversa))
    {
        inversa[longitudo]  = (character)('0' + (numerus % X));
        numerus             /= X;
        longitudo++;
    }
    si (longitudo + I > capacitas)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        buffer[i] = inversa[longitudo - I - i];
    }
    buffer[longitudo] = '\0';
    redde longitudo;
}

interior b32
_attributum_numeri (
     ArborScriptor* scriptor,
         StmlNodus* elementum,
constans character* titulus,
               i32  numerus)
{
    character buffer[XVI];

    si (_silvae_numerus_ad_literas(numerus, buffer,
        (i32)magnitudo(buffer))
        == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde stml_attributum_addere(elementum, scriptor->piscina,
        scriptor->intern, titulus, buffer);
}

SilvaArborScriptura
silva_arbor_scribere_nodum (
                          Piscina* piscina,
              constans SilvaNodus* nodus,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
           constans SilvaExpansio* expansio,
              InternamentumChorda* intern)
{
    /* SUPER MATERIAM (silva-migratio T10b, 2026-09-25): scriptor
     * materiae cum unci C89 super caudam silvae (silva_frons_arborem_
     * silvae_parare). Paritas ante switch probata: octeti idem super
     * XI,DCCXI nodos supremos corporis shim (omnes, non primi soli),
     * circuitus idem, comparator silvae aequalis. Ambulatio vetus
     * (_scribere_nodum_internum) parsurae solae servit usque ad T10c. */
     SilvaArborScriptura  fructus;
              SilvaFrons* frons;
   MateriaArborConsilium  consilium;
   MateriaArborScriptura  ms;

    fructus.successus                   = FALSUM;
    fructus.textus.mensura              = ZEPHYRUM;
    fructus.textus.datum                = NIHIL;
    fructus.causa                       = NIHIL;
    fructus.sedes                       = NIHIL;
    fructus.sedes_valorum               = NIHIL;
    fructus.census.spatia_vocationes    = ZEPHYRUM;
    fructus.census.folia_formae         = ZEPHYRUM;
    fructus.census.folia_vocationes     = ZEPHYRUM;
    fructus.census.parametra_visa       = ZEPHYRUM;
    fructus.census.parametra_compressa  = ZEPHYRUM;

    si (piscina == NIHIL || nodus == NIHIL || tabularium == NIHIL)
    {
        fructus.causa = "argumenta nihil";
        redde fructus;
    }
    frons = silva_frons_creare(piscina, expansio);
    si (   frons == NIHIL
        || !silva_frons_arborem_silvae_parare(frons, tabularium,
               grammatica, intern, &consilium))
    {
        fructus.causa = (frons != NIHIL && silva_frons_causa(frons))
            ? silva_frons_causa(frons) : "frons C89 parari non potuit";
        redde fructus;
    }
    ms = materia_arbor_scribere_nodum(piscina, nodus, &consilium);
    fructus.successus = ms.successus;
    fructus.textus = ms.textus;
    fructus.causa = ms.causa;
    fructus.sedes = ms.sedes;
    fructus.census.spatia_vocationes = ms.census.spatia_vocationes;
    redde fructus;
}


/* ==================================================
 * Lector parsurae - status C89 (silva-migratio T10c)
 *
 * Lector IPSE materiae est (sessio lectionis + fixura, T10a); hic
 * involucrum <parsura> (fontes, regiones, directivae, cauda) et
 * lacunae. Ambulatio vetus deleta.
 * ================================================== */

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
       SilvaArborVitium* vitium;
} ArborLector;

/* Lacuna = MateriaLacuna (campi idem, fons inclusus) */
nomen MateriaLacuna ParsuraLacuna;

/* Recusare: causam et lineam figere. Semper FALSUM reddit ut
 * vocantes 'redde _recusare(...)' scribere possint. Prima causa
 * vincit - profundissima est et propissima vero vitio. */
interior b32
_recusare (
            ArborLector* lector,
     constans character* causa,
                    i32  linea)
{
    si (lector->vitium != NIHIL && lector->vitium->causa == NIHIL)
    {
        lector->vitium->causa = causa;
        lector->vitium->linea = linea;
    }
    redde FALSUM;
}

interior b32
_silvae_spatium_solum (
    constans chorda* valor)
{
    i32 i;

    si (valor == NIHIL)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        character c = (character)valor->datum[i];

        si (   c != ' ' && c != '\t' && c != '\n'
            && c != '\r' && c != '\f' && c != '\v')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Numerum decimalem ex chorda; FALSUM si non totus numerus */
interior b32
_numerus_ex_chorda (
    constans chorda* valor,
                i32* exitus)
{
    i32 fructus;
    i32 i;

    si (valor == NIHIL || valor->mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    fructus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        character c = (character)valor->datum[i];

        si (c < '0' || c > '9')
        {
            redde FALSUM;
        }
        fructus = (fructus * X) + (i32)(c - '0');
    }
    *exitus = fructus;
    redde VERUM;
}

/* Elementum liberum proximum (textum spatii albi solius praeteriens;
 * textus alius in sede structurali RECUSATUR). *cursor promovetur. */
interior StmlNodus*
_silvae_elementum_proximum (
    ArborLector* lector,
      StmlNodus* parens,
            i32* cursor)
{
    i32 numerus = stml_numerus_liberorum(parens);

    dum (*cursor < numerus)
    {
        StmlNodus* liberum = stml_liberum_ad_indicem(parens, *cursor);

        (*cursor)++;
        si (liberum == NIHIL)
        {
            perge;
        }
        si (liberum->genus == STML_NODUS_TEXTUS)
        {
            si (_silvae_spatium_solum(liberum->valor))
            {
                perge;
            }
            _recusare(lector, "textus in sede structurali",
                liberum->linea);
            redde NIHIL;
        }
        si (liberum->genus == STML_NODUS_COMMENTUM)
        {
            perge;
        }
        redde liberum;
    }
    redde NIHIL;
}

/* An tag praefixum lexematis ferat */
interior b32
_silvae_est_tag_lexematis (
    constans chorda* titulus)
{
    i32 longitudo = (i32)strlen(SILVA_ARBOR_PRAEFIXUM);

    si (titulus == NIHIL || titulus->mensura < longitudo)
    {
        redde FALSUM;
    }
    redde (memcmp(titulus->datum, SILVA_ARBOR_PRAEFIXUM,
               (size_t)longitudo) == ZEPHYRUM) ? VERUM : FALSUM;
}

SilvaNodus*
silva_arbor_legere (
                           Piscina* piscina,
               InternamentumChorda* intern,
                            chorda  textus,
     constans SilvaRegistrumCoctum* tabularium,
                constans character* grammatica,
                  SilvaArborVitium* vitium)
{
    /* SUPER MATERIAM (T10b): lector materiae cum unci C89; extenta
     * lecta in frons vivunt, ergo subarbor cum <extentum> IAM legitur
     * (lector vetus 'extentum sine expansione' recusabat - CI nodi
     * corporis). Patres: politica materiae (omnes; decretum …MQF). */
            SilvaFrons* frons;
 MateriaArborConsilium  consilium;
    MateriaArborVitium  mv;
          MateriaNodus* arbor;

    si (vitium != NIHIL)
    {
        vitium->causa = NIHIL;
        vitium->linea = ZEPHYRUM;
    }
    si (piscina == NIHIL || tabularium == NIHIL || grammatica == NIHIL)
    {
        si (vitium != NIHIL)
        { vitium->causa = "argumenta nihil";
        }
        redde NIHIL;
    }
    frons = silva_frons_creare(piscina, NIHIL);
    si (   frons == NIHIL
        || !silva_frons_arborem_silvae_parare(frons, tabularium,
               grammatica, intern, &consilium))
    {
        si (vitium != NIHIL)
        {
            vitium->causa = (frons != NIHIL && silva_frons_causa(frons))
                ? silva_frons_causa(frons) : "frons C89 parari non potuit";
        }
        redde NIHIL;
    }
    mv.causa = NIHIL;
    mv.linea = ZEPHYRUM;
    arbor = materia_arbor_legere(piscina, intern, textus, &consilium,
        &mv);
    si (arbor == NIHIL)
    {
        si (vitium != NIHIL)
        {
            vitium->causa = mv.causa;
            vitium->linea = mv.linea;
        }
        redde NIHIL;
    }

    /* Commissio ut antea: 'ambigui' ab hac sola ambulatione impletur,
     * et arbor lecta NON re-canonicari potest ante eam
     * (silva_commissio.h:163-165). */
    si (silva_committere(piscina, silva_valor_nodus(arbor), tabularium,
            NIHIL, NIHIL, NIHIL) == NIHIL)
    {
        si (vitium != NIHIL)
        { vitium->causa = "arbor committi non potuit";
        }
        redde NIHIL;
    }
    redde arbor;
}


/* ==================================================
 * Parsura: plagula INTEGRA <-> STML canonicum (M2 §2)
 *
 * Documentum <parsura> CLAUSURAM EMISSIONIS fert - id est prorsus
 * quod silva_scribere_fontem legit - et nihil aliud. Strata, acta,
 * numeratores et vexilla salutis EXCLUDUNTUR: strata EXITUS
 * expansionis sunt, ergo stratum mutare incohaerens est, et
 * documentum quod plagulam EDITAM iuxta stratum NON EDITUM ferre
 * posset MENTIRI posset.
 *
 * UNUS scriptor totum documentum scribit (et UNUS lector totum
 * relegit) CONSULTO: numeratio fragmentorum documento-scopata est.
 * silva_arbor_scribere_nodum per nodum vocatum numerus_notarum ad
 * zephyrum omni vocamine reponeret, ergo duo nodi supremi ambo
 * '<#lex1>' emitterent et documentum identitates GEMINAS ferret.
 *
 * Statica huius sectionis praefixum _parsura_ ferunt: in amalgamate
 * uno statica omnia spatium nominum unum communicant.
 *
 * Consilium: project-specs/arbor-parsura-spec.md.
 * ================================================== */

/* Chorda nullo NON terminatur; APIs quae 'constans character*'
 * petunt bufferum nullatum egent. */
interior character*
_parsura_via_nullata (
             Piscina* piscina,
     constans chorda* via)
{
    character* buffer;

    si (via == NIHIL || via->datum == NIHIL)
    {
        redde NIHIL;
    }
    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)via->mensura + I);
    si (buffer == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(buffer, via->datum, (memoriae_index)via->mensura);
    buffer[via->mensura] = '\0';
    redde buffer;
}

/* Sectio fontium: tabula index -> via. Lector eam REPETIT quia
 * scriptor expansionem non-NIHIL postulat ut fons_index solvat.
 * NB tag in initio commentarii scanner annotationum evocat - ergo
 * hic nominatim, non per tagum. */
interior b32
_parsura_fontes_scribere (
             ArborScriptor* scriptor,
                 StmlNodus* involucrum,
    constans SilvaExpansio* expansio)
{
    StmlNodus* sectio;
          i32  i;

    sectio = stml_elementum_creare(scriptor->piscina,
        scriptor->intern, SILVA_ARBOR_TAG_FONTES);
    si (sectio == NIHIL)
    {
        redde FALSUM;
    }
    si (expansio != NIHIL && expansio->fontes != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(expansio->fontes); i++)
        {
            SilvaFons* fons;
            StmlNodus* elem;

            fons = (SilvaFons*)xar_obtinere(expansio->fontes, i);
            si (fons == NIHIL)
            {
                perge;
            }
            elem = stml_elementum_creare(scriptor->piscina,
                scriptor->intern, SILVA_ARBOR_TAG_FONS);
            si (elem == NIHIL)
            {
                redde FALSUM;
            }
            _attributum_numeri(scriptor, elem, "index", i);
            si (fons->via != NIHIL)
            {
                stml_attributum_addere_chorda(elem, scriptor->piscina,
                    scriptor->intern, "via", *fons->via);
            }
            si (fons->est_lexicon)
            {
                stml_attributum_boolean_addere(elem,
                    scriptor->piscina, scriptor->intern, "lexicon");
            }
            si (fons->est_syntheticus)
            {
                stml_attributum_boolean_addere(elem,
                    scriptor->piscina, scriptor->intern,
                    "syntheticus");
            }
            si (!stml_liberum_addere(sectio, elem))
            {
                redde FALSUM;
            }
        }
    }
    redde stml_liberum_addere(involucrum, sectio);
}

/* Primum lexema ordine AMBULATIONIS; NIHIL si nullum. */
interior SilvaToken*
_parsura_primum_lexema (
    SilvaValor valor)
{
    i32 numerus;
    i32 i;

    si (valor.genus == SILVA_VALOR_TOKEN)
    {
        redde valor.datum.token;
    }
    si (valor.genus == SILVA_VALOR_NODUS && valor.datum.nodus != NIHIL)
    {
        per (i = ZEPHYRUM; i < valor.datum.nodus->numerus_locorum;
             i++)
        {
            SilvaToken* lexema;

            lexema = _parsura_primum_lexema(
                valor.datum.nodus->loci[i]);
            si (lexema != NIHIL)
            {
                redde lexema;
            }
        }
        redde NIHIL;
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        numerus = silva_valor_lista_numerus(valor);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaValor* elementum;
            SilvaToken* lexema;

            elementum = silva_valor_lista_obtinere(valor, i);
            si (elementum == NIHIL)
            {
                perge;
            }
            lexema = _parsura_primum_lexema(*elementum);
            si (lexema != NIHIL)
            {
                redde lexema;
            }
        }
    }
    redde NIHIL;
}

/* Lexema quod EMISSIONEM incipit: catenam originis ad radicem
 * strati 0 sequi.
 *
 * Lexema expansum sedem DEF-SITE fert (silva_token_ex_expansione
 * campos lexicales a corpore copiat), sed in fluxu octetorum
 * INVOCATIO stat. Ancora ergo ex expansione sumpta sedem plagulae
 * ALTERIUS daret - et nodus totus inde laberetur.
 *
 * MENSURATUM: latina.h 'nomen insignatus brevis i16;' - lexema
 * primum expansum 'typedef' est, cuius sedes est linea XXXIX
 * ('#define nomen typedef'), non linea CCCXLIX ubi nodus vere
 * stat. */
interior constans SilvaToken*
_parsura_lexema_emissionis (
    constans SilvaToken* lexema)
{
    i32 custodia;

    custodia = ZEPHYRUM;
    dum (lexema != NIHIL && custodia < 64)
    {
        constans SilvaToken* proximum;

        proximum = NIHIL;
        commutatio (silva_token_origo(lexema)->genus)
        {
        casus SILVA_ORIGO_EXPANSIO:
            proximum =
                silva_token_origo(lexema)->datum.expansio.invocatio;
            frange;
        casus SILVA_ORIGO_PASTA:
            proximum = silva_token_origo(lexema)->datum.pasta.invocatio
                     ? silva_token_origo(lexema)->datum.pasta.invocatio
                     : silva_token_origo(lexema)->datum.pasta.sinister;
            frange;
        casus SILVA_ORIGO_CHORDA:
            proximum =
                silva_token_origo(lexema)->datum.stringificatio.primus;
            frange;
        ordinarius:
            frange;
        }
        si (proximum == NIHIL)
        {
            frange;
        }
        lexema = proximum;
        custodia++;
    }
    redde lexema;
}

/* ANCORA per liberum SUPREMUM documenti.
 *
 * CUR non cursor unus continuus: contentum NON-arboreum (laminae
 * regionum degradatarum) INTRA spatium octetorum nodi iacere
 * potest - regio in initiatore, exempli gratia. Ordo documenti
 * ergo ordinem octetorum exprimere NEQUIT, et cursor linearis
 * sedes post nodum totum poneret. Ancora per liberum id solvit,
 * et est prorsus exemplar M1 (documentum <arbor> ancoram fert)
 * granulositate minore. */
interior vacuum
_parsura_ancoram_scribere (
           ArborScriptor* scriptor,
               StmlNodus* elementum,
     constans SilvaToken* lexema)
{
    constans SilvaToken* initium;

    si (lexema == NIHIL || lexema->byte_offset < ZEPHYRUM)
    {
        redde;
    }

    /* ANCORA = sedes ubi EMISSIO INCIPIT, non sedes LEXEMATIS.
     *
     * Emissio triviis DUCENTIBUS incipit (commentarium, spatia),
     * ergo ancora ex lexemate sumpta lectorem cursorem ad lexema
     * ponere faceret, trivia ANTE id emittere, et sedes omnes
     * longitudine triviorum labi. Idem vitium M1 T6 cepit (CLXXVIII
     * divergentiae); hic RECURRIT quia ancoram novam scripsi potius
     * quam rationem M1 adhibui.
     *
     * Probationes unitatis id capere NON potuerunt: fontes earum
     * commentaria ducentia non habent. 'Fixturae praesumptiones
     * tuas communicant; corpus non.' */
    /* Catenam originis PRIMO sequi: emissio ab invocatione strati
     * 0 incipit, non a lexemate expanso */
    lexema   = _parsura_lexema_emissionis(lexema);
    initium  = lexema;
    si (   silva_token_ante_numerus(lexema) > ZEPHYRUM
        && silva_token_ante_numerus(lexema) > ZEPHYRUM)
    {
        constans SilvaToken* trivium;

        trivium = silva_token_ante(lexema, ZEPHYRUM);
        si (trivium != NIHIL && trivium->byte_offset >= ZEPHYRUM)
        {
            initium = trivium;
        }
    }

    _attributum_numeri(scriptor, elementum, "b",
        (i32)initium->byte_offset);
    _attributum_numeri(scriptor, elementum, "linea", initium->linea);
    _attributum_numeri(scriptor, elementum, "columna",
        initium->columna);
    /* initium_lineae LEXEMATIS, non trivii - proprietas lexematis
     * est (M1 idem facit) */
    si (silva_token_initium_lineae(lexema))
    {
        stml_attributum_boolean_addere(elementum, scriptor->piscina,
            scriptor->intern, "linea-initium");
    }
}

/* Offset primi lexematis ordine AMBULATIONIS; -I si nullum lexema.
 * Ordo ambulationis (non minimum octetorum) eligitur ut idem sit
 * ordo quo scriptor emittit. */
/* Offset ubi EMISSIO lexematis INCIPIT: catena originis ad stratum
 * 0 secuta, deinde trivium DUCENS si adest.
 *
 * LEX ANCORAE, FACIES QUARTA. Ordinatio supremorum offset LEXEMATIS
 * CRUDI utebatur dum ancora scripta triviis inclusis ordinaretur -
 * DUAE notiones 'ubi hoc incipit'. Directiva commento magno praeeunte
 * ergo post nodum ordinabatur quem PRAECEDERE debebat, et ordo
 * documenti (qui ordo PLAGULAE esse debet) frangebatur.
 * MENSURATUM (T7): lib/arbor2_glr_tabula.c - directiva ad octetum
 * MCIII CCLXI post declarationem ad MCIII DCCCXLVII scripta est,
 * unde transclusio ad fragmentum in arbore definitum ante suam
 * definitionem lecta est (laminae passu ZERO leguntur).
 *
 * EADEM SANATIO QUA ANCORA TER: functio UNA ambabus servit, ratio
 * COPIATUR non re-cogitatur. */
interior s32
_parsura_offset_emissionis (
    constans SilvaToken* lexema)
{
    constans SilvaToken* initium;

    si (lexema == NIHIL)
    {
        redde -I;
    }
    initium = _parsura_lexema_emissionis(lexema);
    si (initium == NIHIL)
    {
        redde -I;
    }
    si (   silva_token_ante_numerus(initium) > ZEPHYRUM
        && silva_token_ante_numerus(initium) > ZEPHYRUM)
    {
        constans SilvaToken* trivium = silva_token_ante(initium,
            ZEPHYRUM);

        si (trivium != NIHIL && trivium->byte_offset >= ZEPHYRUM)
        {
            redde trivium->byte_offset;
        }
    }
    redde initium->byte_offset;
}

interior s32
_parsura_primum_offset (
    SilvaValor valor)
{
    i32 numerus;
    i32 i;

    si (valor.genus == SILVA_VALOR_TOKEN)
    {
        si (valor.datum.token == NIHIL)
        {
            redde -I;
        }
        redde _parsura_offset_emissionis(valor.datum.token);
    }
    si (valor.genus == SILVA_VALOR_NODUS && valor.datum.nodus != NIHIL)
    {
        per (i = ZEPHYRUM; i < valor.datum.nodus->numerus_locorum;
             i++)
        {
            s32 offset;

            offset = _parsura_primum_offset(valor.datum.nodus->loci[i]);
            si (offset >= ZEPHYRUM)
            {
                redde offset;
            }
        }
        redde -I;
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        numerus = silva_valor_lista_numerus(valor);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaValor* elementum;
                   s32  offset;

            elementum = silva_valor_lista_obtinere(valor, i);
            si (elementum == NIHIL)
            {
                perge;
            }
            offset = _parsura_primum_offset(*elementum);
            si (offset >= ZEPHYRUM)
            {
                redde offset;
            }
        }
    }
    redde -I;
}


/* ==================================================
 * Reinserenda: quod ARBORI non pertinet sed PLAGULAE
 *
 * Duae classes, una lista: lineae directivarum consumptae, et
 * laminae regionum DEGRADATARUM. Regio TEXTA nihil confert -
 * lineas suas ex ARBORE emittit (dominus unus, silva_expandere.h
 * :130), quod _regiones_colligere in silva_scribere.c per
 * 'si (!regio->est_texta)' dicit.
 *
 * Cur lista PLANA et non elementum <conditionalis> continuum:
 * in regione degradata ramus SUMPTUS corpus suum in fluxum
 * lexematum (ergo in ARBOREM) mittit, dum rami non sumpti laminas
 * crudas retinent. Ergo contentum arboris INTRA spatium octetorum
 * regionis intertexitur, et involucrum continuum mentiretur.
 * Lista plana ordine octetorum id honeste reddit.
 * ================================================== */

#define PARSURA_REINS_DIRECTIVA        0
#define PARSURA_REINS_REGIO_DIRECTIVA  1
#define PARSURA_REINS_REGIO_CRUDA      2
#define PARSURA_REINS_REGIO_FINIS      3
#define PARSURA_REINS_INVOCATIO_VACUA  4

nomen structura {
                s32  offset;
                i32  genus;
                Xar* lamina;
                s32  regio_index;
                s32  pater_index;
                s32  ramus_index;
    SilvaRamusGenus  ramus_genus;
                i32  conditio_id;
                s32  regio_fons;
                i32  regio_linea;
} ParsuraReinserendum;

interior s32
_parsura_reinserenda_conferre (
    constans vacuum* a,
    constans vacuum* b)
{
    constans ParsuraReinserendum* ra;
    constans ParsuraReinserendum* rb;

    ra = (constans ParsuraReinserendum*)a;
    rb = (constans ParsuraReinserendum*)b;
    si (ra->offset < rb->offset)
    {
        redde -I;
    }
    si (ra->offset > rb->offset)
    {
        redde I;
    }
    redde ZEPHYRUM;
}

/* Laminam addere si fontem nostrum spectat (custodia eadem ac
 * silva_scribere.c _reinserendum_addere) */
interior b32
_parsura_reinserendum_addere (
                     Xar* acervus,
                     Xar* lamina,
                     i32  genus,
                     s32  regio_index,
                     s32  pater_index,
                     s32  ramus_index,
         SilvaRamusGenus  ramus_genus,
                     i32  conditio_id,
                     s32  regio_fons,
                     i32  regio_linea,
                     s32  fons_index)
{
             SilvaToken* primum;
    ParsuraReinserendum* r;

    si (lamina == NIHIL || xar_numerus(lamina) == ZEPHYRUM)
    {
        redde VERUM;
    }
    primum = *(SilvaToken**)xar_obtinere(lamina, ZEPHYRUM);
    si (primum == NIHIL)
    {
        redde VERUM;
    }
    si (fons_index >= ZEPHYRUM && primum->fons_index != fons_index)
    {
        redde VERUM;
    }
    r = (ParsuraReinserendum*)xar_addere(acervus);
    si (r == NIHIL)
    {
        redde FALSUM;
    }
    r->offset       = _parsura_offset_emissionis(primum);
    r->genus        = genus;
    r->lamina       = lamina;
    r->regio_index  = regio_index;
    r->pater_index  = pater_index;
    r->ramus_index  = ramus_index;
    r->ramus_genus  = ramus_genus;
    r->conditio_id  = conditio_id;
    r->regio_fons   = regio_fons;
    r->regio_linea  = regio_linea;
    redde VERUM;
}

/* Regiones ambulare ordine PRAEFIXO, indices assignans. *proximus
 * index proximum liberum tenet, ut filiae indices unicos capiant. */
interior b32
_parsura_regiones_colligere (
     Xar* acervus,
     Xar* regiones,
     s32  pater_index,
     s32* proximus,
     s32  fons_index)
{
    i32 i;

    si (regiones == NIHIL)
    {
        redde VERUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(regiones); i++)
    {
        SilvaRegio* regio;
               s32  index_huius;
               i32  j;

        regio = *(SilvaRegio**)xar_obtinere(regiones, i);
        si (regio == NIHIL)
        {
            perge;
        }
        index_huius = *proximus;
        (*proximus)++;

        si (!regio->est_texta && regio->rami != NIHIL)
        {
            per (j = ZEPHYRUM; j < xar_numerus(regio->rami); j++)
            {
                SilvaRamus* ramus;

                ramus = *(SilvaRamus**)xar_obtinere(regio->rami, j);
                si (ramus == NIHIL)
                {
                    perge;
                }
                si (!_parsura_reinserendum_addere(acervus,
                         ramus->directiva,
                         PARSURA_REINS_REGIO_DIRECTIVA, index_huius,
                         pater_index, (s32)j, ramus->genus,
                         ramus->conditio_id, regio->fons_index,
                         regio->linea, fons_index))
                {
                    redde FALSUM;
                }
                si (!_parsura_reinserendum_addere(acervus,
                         ramus->lexemata_cruda,
                         PARSURA_REINS_REGIO_CRUDA, index_huius,
                         pater_index, (s32)j, ramus->genus,
                         ramus->conditio_id, regio->fons_index,
                         regio->linea, fons_index))
                {
                    redde FALSUM;
                }
            }
        }
        si (!regio->est_texta && regio->directiva_finis != NIHIL)
        {
            si (!_parsura_reinserendum_addere(acervus,
                     regio->directiva_finis,
                     PARSURA_REINS_REGIO_FINIS, index_huius,
                     pater_index, -I, SILVA_RAMUS_IF, ZEPHYRUM,
                     regio->fons_index, regio->linea, fons_index))
            {
                redde FALSUM;
            }
        }
        si (!_parsura_regiones_colligere(acervus, regio->filiae,
                 index_huius, proximus, fons_index))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Nomen generis rami (verba ipsa C sunt - directiva ea fert, sed
 * documentum genus NOMINAT ne lector lineam re-parsare debeat) */
interior constans character*
_parsura_ramus_genus_nomen (
    SilvaRamusGenus genus)
{
    commutatio (genus)
    {
    casus SILVA_RAMUS_IF:      redde "if";
    casus SILVA_RAMUS_IFDEF:   redde "ifdef";
    casus SILVA_RAMUS_IFNDEF:  redde "ifndef";
    casus SILVA_RAMUS_ELIF:    redde "elif";
    casus SILVA_RAMUS_ELSE:    redde "else";
    ordinarius:                frange;
    }
    redde "if";
}

interior SilvaRamusGenus
_parsura_ramus_genus_ex_nomine (
    constans chorda* titulus)
{
    si (titulus == NIHIL)
    {
        redde SILVA_RAMUS_IF;
    }
    si (chorda_aequalis_literis(*titulus, "ifdef"))
    {
        redde SILVA_RAMUS_IFDEF;
    }
    si (chorda_aequalis_literis(*titulus, "ifndef"))
    {
        redde SILVA_RAMUS_IFNDEF;
    }
    si (chorda_aequalis_literis(*titulus, "elif"))
    {
        redde SILVA_RAMUS_ELIF;
    }
    si (chorda_aequalis_literis(*titulus, "else"))
    {
        redde SILVA_RAMUS_ELSE;
    }
    redde SILVA_RAMUS_IF;
}

/* Reinserendum unum in elementum documenti. Genus elementi
 * classem NOMINAT ut lector eam sine re-parsatione lineae
 * cognoscat. */
interior StmlNodus*
_parsura_reinserendum_scribere (
           ArborScriptor* scriptor,
     ParsuraReinserendum* r)
{
     constans character* tag;
              StmlNodus* elementum;
                    i32  i;

    commutatio (r->genus)
    {
    casus PARSURA_REINS_REGIO_DIRECTIVA:
        tag = SILVA_ARBOR_TAG_REGIO_DIRECTIVA;
        frange;
    casus PARSURA_REINS_REGIO_CRUDA:
        tag = SILVA_ARBOR_TAG_REGIO_CRUDA;
        frange;
    casus PARSURA_REINS_REGIO_FINIS:
        tag = SILVA_ARBOR_TAG_REGIO_FINIS;
        frange;
    casus PARSURA_REINS_INVOCATIO_VACUA:
        tag = SILVA_ARBOR_TAG_INVOCATIO_VACUA;
        frange;
    ordinarius:
        tag = SILVA_ARBOR_TAG_DIRECTIVA;
        frange;
    }
    elementum = stml_elementum_creare(scriptor->piscina,
        scriptor->intern, tag);
    si (elementum == NIHIL)
    {
        scriptor->causa = "reinserendum creari non potuit";
        redde NIHIL;
    }
    si (   r->genus != PARSURA_REINS_DIRECTIVA
        && r->genus != PARSURA_REINS_INVOCATIO_VACUA)
    {
        _attributum_numeri(scriptor, elementum, "regio",
            (i32)r->regio_index);
        si (r->pater_index >= ZEPHYRUM)
        {
            _attributum_numeri(scriptor, elementum, "pater",
                (i32)r->pater_index);
        }
        _attributum_numeri(scriptor, elementum, "regio-fons",
            (i32)r->regio_fons);
        _attributum_numeri(scriptor, elementum, "regio-linea",
            r->regio_linea);
    }
    si (   r->genus == PARSURA_REINS_REGIO_DIRECTIVA
        || r->genus == PARSURA_REINS_REGIO_CRUDA)
    {
        _attributum_numeri(scriptor, elementum, "ramus",
            (i32)r->ramus_index);
        stml_attributum_addere(elementum, scriptor->piscina,
            scriptor->intern, "genus",
            _parsura_ramus_genus_nomen(r->ramus_genus));
        _attributum_numeri(scriptor, elementum, "conditio",
            r->conditio_id);
    }
    _parsura_ancoram_scribere(scriptor, elementum,
        *(SilvaToken**)xar_obtinere(r->lamina, ZEPHYRUM));

    per (i = ZEPHYRUM; i < xar_numerus(r->lamina); i++)
    {
        SilvaToken* lexema;
         StmlNodus* scriptum;

        lexema = *(SilvaToken**)xar_obtinere(r->lamina, i);
        si (lexema == NIHIL)
        {
            perge;
        }
        scriptum = materia_arbor_lexema_scribere(scriptor->sessio,
            lexema);
        si (scriptum == NIHIL)
        {
            scriptor->causa = materia_arbor_scriptor_causa(
                scriptor->sessio);
            redde NIHIL;
        }
        si (!stml_liberum_addere(elementum, scriptum))
        {
            scriptor->causa =
                "lexema in reinserendum addi non potuit";
            redde NIHIL;
        }
    }
    redde elementum;
}


/* ==================================================
 * Folia macronum - compressio formarum repetitarum (macros v1)
 *
 * Folium = elementum lexematis originem <expansio> ferens. Lexemata
 * trans-fontes sedes DEFINITIONIS ferunt (non usus), ergo folia
 * eiusdem expansionis octetim IDENTICA sunt (mensuratum 2026-08-26:
 * NIHIL 189B x118 in sex plagulis lib). Forma quae bis pluriesve
 * apparet in definitionem capitis '<#@m-<macro>>' levatur (occursus
 * PRIMUS in definitionem MOVETUR - sedes definitionis, exemplar
 * lexN), sedes omnes vocationes '<<#@m-<macro>>>' fiunt. Ex
 * contento derivatum, ergo independens invocationis.
 * ================================================== */

nomen structura {
         chorda* clavis;   /* signum: scriptio compacta internata */
      StmlNodus* nodus;
      StmlNodus* parens;
            i32  index;    /* in parens->liberi */
} FoliumOccursus;

nomen structura {
        chorda* clavis;
        chorda* id;       /* '@m-...' internatum */
     StmlNodus* primus;   /* corpus definitionis (nodus MOTUS) */
} FoliumForma;

/* Par substitutionis - clavis tabulae in campum 'vetus' monstrat
 * (cellae Xaris stabiles; tabula chordam SINE copia servat) */
nomen structura {
     StmlNodus* vetus;
     StmlNodus* novus;
} FoliumPar;

interior b32
_folium_candidatum (
    StmlNodus* n)
{
    i32 i;
    i32 num;

    si (   n->genus != STML_NODUS_ELEMENTUM
        || n->fragmentum
        || !_silvae_est_tag_lexematis(n->titulus))
    {
        redde FALSUM;
    }
    num = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(n, i);

        si (   l          != NIHIL
            && l->genus   == STML_NODUS_ELEMENTUM
            && l->titulus != NIHIL
            && chorda_aequalis_literis(*l->titulus,
                   SILVA_ARBOR_TAG_EXPANSIO))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_folia_colligere (
    ArborScriptor* scriptor,
        StmlNodus* n,
              Xar* occursus)
{
    i32 i;
    i32 num;

    si (n == NIHIL || n->genus != STML_NODUS_ELEMENTUM)
    {
        redde;
    }
    num = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(n, i);

        si (l == NIHIL)
        {
            perge;
        }
        si (_folium_candidatum(l))
        {
            FoliumOccursus* o;
                    chorda  scriptum;

            scriptum  = stml_scribere(l, scriptor->piscina, FALSUM);
            o         = (FoliumOccursus*)xar_addere(occursus);
            si (o == NIHIL)
            {
                redde;
            }
            o->clavis  = chorda_internare(scriptor->intern, scriptum);
            o->nodus   = l;
            o->parens  = n;
            o->index   = i;
            perge;  /* in candidatum non descendere (extimus vincit) */
        }
        _folia_colligere(scriptor, l, occursus);
    }
}

/* Subarbores parallelas (isomorphas - signa octetim aequalia id
 * spondent) in tabulam substitutionis mappare: elementum vetus ->
 * elementum novum, ut paria sedium repungantur. */
interior vacuum
_folia_paria_mappare (
         StmlNodus* vetus,
         StmlNodus* novus,
               Xar* mappae,
    TabulaDispersa* substituti)
{
    FoliumPar* par;
       chorda  clavis;
          i32  i;
          i32  num;

    par = (FoliumPar*)xar_addere(mappae);
    si (par == NIHIL)
    {
        redde;
    }
    par->vetus      = vetus;
    par->novus      = novus;
    clavis.datum    = (i8*)&par->vetus;
    clavis.mensura  = (i32)magnitudo(par->vetus);
    tabula_dispersa_inserere(substituti, clavis, novus);

    num = stml_numerus_liberorum(vetus);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* lv = stml_liberum_ad_indicem(vetus, i);
        StmlNodus* ln = stml_liberum_ad_indicem(novus, i);

        si (   lv        != NIHIL && ln != NIHIL
            && lv->genus == STML_NODUS_ELEMENTUM
            && ln->genus == STML_NODUS_ELEMENTUM)
        {
            _folia_paria_mappare(lv, ln, mappae, substituti);
        }
    }
}

interior b32
_folia_macronum_comprimere (
    ArborScriptor* scriptor,
        StmlNodus* involucrum)
{
               Xar* occursus;      /* FoliumOccursus */
               Xar* formae;        /* FoliumForma */
               Xar* mappae;        /* FoliumPar */
    TabulaDispersa* numeri;        /* clavis -> numerus occursuum */
    TabulaDispersa* electae;       /* clavis -> FoliumForma* */
    TabulaDispersa* substituti;    /* &vetus -> novus */
    TabulaDispersa* nomina;        /* id basis -> numerus formarum
                                    * eodem titulo (suffixum '-K') */
               i32 i;

    occursus = xar_creare(scriptor->piscina,
        magnitudo(FoliumOccursus));
    formae      = xar_creare(scriptor->piscina, magnitudo(FoliumForma));
    mappae      = xar_creare(scriptor->piscina, magnitudo(FoliumPar));
    numeri      = tabula_dispersa_creare_chorda(scriptor->piscina, 256);
    electae     = tabula_dispersa_creare_chorda(scriptor->piscina, 64);
    substituti  = tabula_dispersa_creare_chorda(scriptor->piscina, 256);
    nomina      = tabula_dispersa_creare_chorda(scriptor->piscina, 64);
    si (   occursus   == NIHIL || formae == NIHIL || mappae == NIHIL
        || numeri     == NIHIL || electae == NIHIL
        || substituti == NIHIL || nomina == NIHIL)
    {
        scriptor->causa = "compressio foliorum parari non potuit";
        redde FALSUM;
    }

    _folia_colligere(scriptor, involucrum, occursus);

    /* numeratio per formam */
    per (i = ZEPHYRUM; i < xar_numerus(occursus); i++)
    {
        FoliumOccursus* o =
            (FoliumOccursus*)xar_obtinere(occursus, i);
                vacuum* valor;
        memoriae_index  n;

        n = tabula_dispersa_invenire(numeri, *o->clavis, &valor)
                ? (memoriae_index)valor : ZEPHYRUM;
        tabula_dispersa_inserere(numeri, *o->clavis,
            (vacuum*)(n + I));
    }

    /* electio + definitio (ordine occursus primi - determinatum) */
    per (i = ZEPHYRUM; i < xar_numerus(occursus); i++)
    {
        FoliumOccursus* o =
            (FoliumOccursus*)xar_obtinere(occursus, i);
             vacuum* valor;
             chorda* macro;
          StmlNodus* nodus_expansionis;
          StmlNodus* definitio;
        FoliumForma* forma;
          character  id_litterae[96];
             chorda  id;
                i32  j;
                i32  num;
                i32  basis;

        si (   !tabula_dispersa_invenire(numeri, *o->clavis, &valor)
            || (memoriae_index)valor < II
            || tabula_dispersa_continet(electae, *o->clavis))
        {
            perge;
        }

        /* titulus macronis ex libero <expansio> primo */
        nodus_expansionis  = NIHIL;
        num                = stml_numerus_liberorum(o->nodus);
        per (j = ZEPHYRUM; j < num; j++)
        {
            StmlNodus* l = stml_liberum_ad_indicem(o->nodus, j);

            si (   l          != NIHIL
                && l->genus   == STML_NODUS_ELEMENTUM
                && l->titulus != NIHIL
                && chorda_aequalis_literis(*l->titulus,
                       SILVA_ARBOR_TAG_EXPANSIO))
            {
                nodus_expansionis = l;
                frange;
            }
        }
        macro = nodus_expansionis != NIHIL
            ? stml_attributum_capere(nodus_expansionis, "macro")
            : NIHIL;
        si (   macro == NIHIL || macro->mensura == ZEPHYRUM
            || macro->mensura > 64)
        {
            perge;  /* sine titulo tuto: formam praeterire */
        }

        /* id '@m-<macro>'; formae distinctae eiusdem macronis
         * suffixum '-K' capiunt per NUMERATOREM tituli - tabulae
         * grammaticae macronem unum centies argumentis diversis
         * vocant, ergo scansio limitata (tectum '-99') GEMINUM
         * peperit (mensuratum: lapifex_c89_grammatica). Titulus
         * macronis '-' ferre nequit (identificator C), ergo
         * suffixum collidi non potest. */
        memcpy(id_litterae, "@m-", 3);
        memcpy(id_litterae + III, macro->datum,
            (memoriae_index)macro->mensura);
        basis       = III + macro->mensura;
        id.datum    = (i8*)id_litterae;
        id.mensura  = basis;
        {
                    chorda* basis_internata;
                    vacuum* prior;
            memoriae_index  n;

            basis_internata = chorda_internare(scriptor->intern, id);
            si (basis_internata == NIHIL)
            {
                redde FALSUM;
            }
            n = tabula_dispersa_invenire(nomina, *basis_internata,
                    &prior)
                    ? (memoriae_index)prior : ZEPHYRUM;
            tabula_dispersa_inserere(nomina, *basis_internata,
                (vacuum*)(n + I));
            si (n > ZEPHYRUM)
            {
                character num_lit[XVI];
                      i32 ln;

                si (_silvae_numerus_ad_literas((i32)(n + I), num_lit,
                        (i32)magnitudo(num_lit)) == ZEPHYRUM)
                {
                    redde FALSUM;
                }
                id_litterae[basis]  = '-';
                ln                  = (i32)strlen(num_lit);
                memcpy(id_litterae + basis + I, num_lit,
                    (memoriae_index)ln);
                id.mensura = basis + I + ln;
            }
        }

        definitio = stml_elementum_creare(scriptor->piscina,
            scriptor->intern, "fragmentum");
        si (definitio == NIHIL)
        {
            scriptor->causa = "definitio folii creari non potuit";
            redde FALSUM;
        }
        definitio->fragmentum    = VERUM;
        definitio->fragmentum_id = chorda_internare(scriptor->intern,
            id);
        /* occursus primus in definitionem MOVETUR (sedes
         * definitionis); cella parentis vocationem infra accipit */
        si (   definitio->fragmentum_id == NIHIL
            || !stml_liberum_addere(definitio, o->nodus))
        {
            scriptor->causa = "definitio folii construi non potuit";
            redde FALSUM;
        }

        forma = (FoliumForma*)xar_addere(formae);
        si (forma == NIHIL)
        {
            redde FALSUM;
        }
        forma->clavis  = o->clavis;
        forma->id      = definitio->fragmentum_id;
        forma->primus  = o->nodus;
        tabula_dispersa_inserere(electae, *o->clavis, forma);
    }

    /* substitutio: occursus quisque formae electae vocatio fit */
    per (i = ZEPHYRUM; i < xar_numerus(occursus); i++)
    {
        FoliumOccursus* o =
            (FoliumOccursus*)xar_obtinere(occursus, i);
             vacuum* valor;
        FoliumForma* forma;
          StmlNodus* vocatio;
          character  valor_litterae[100];
             chorda  interior_vocationis;

        si (!tabula_dispersa_invenire(electae, *o->clavis, &valor))
        {
            perge;
        }
        forma = (FoliumForma*)valor;

        valor_litterae[0] = '#';
        memcpy(valor_litterae + I, forma->id->datum,
            (memoriae_index)forma->id->mensura);
        interior_vocationis.datum    = (i8*)valor_litterae;
        interior_vocationis.mensura  = I + forma->id->mensura;
        vocatio = stml_transclusionem_creare(scriptor->piscina,
            scriptor->intern, interior_vocationis);
        si (vocatio == NIHIL)
        {
            scriptor->causa = "vocatio folii creari non potuit";
            redde FALSUM;
        }
        *(StmlNodus**)xar_obtinere(o->parens->liberi, o->index) =
            vocatio;
        vocatio->parens = o->parens;
        scriptor->census.folia_vocationes++;

        si (o->nodus != forma->primus)
        {
            _folia_paria_mappare(o->nodus, forma->primus, mappae,
                substituti);
        }
    }

    /* paria sedium repungere: elementa substituta ad corpus
     * definitionis monstrant (sedes definitionis, exemplar lexN) */
    /* repunctio parium per sessionem materiae (T10c) */
    materia_arbor_scriptor_repungere(scriptor->sessio, substituti);

    scriptor->census.folia_formae += xar_numerus(formae);

    /* definitiones in caput inserere: [fontes, post, ante] + novae
     * + reliqua (liberi novi - insertio media Xari non est) */
    si (xar_numerus(formae) > ZEPHYRUM)
    {
        Xar* liberi_novi;
        i32  caput_finis = III;  /* fontes + def-post + def-ante */

        liberi_novi = xar_creare(scriptor->piscina,
            magnitudo(StmlNodus*));
        si (liberi_novi == NIHIL)
        {
            scriptor->causa = "liberi novi creari non potuerunt";
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < xar_numerus(involucrum->liberi); i++)
        {
            StmlNodus** cella;

            si (i == caput_finis)
            {
                i32 k;

                per (k = ZEPHYRUM; k < xar_numerus(formae); k++)
                {
                    FoliumForma* f =
                        (FoliumForma*)xar_obtinere(formae, k);
                    StmlNodus* definitio = f->primus->parens;

                    cella = (StmlNodus**)xar_addere(liberi_novi);
                    si (cella == NIHIL)
                    {
                        redde FALSUM;
                    }
                    *cella             = definitio;
                    definitio->parens  = involucrum;
                }
            }
            cella = (StmlNodus**)xar_addere(liberi_novi);
            si (cella == NIHIL)
            {
                redde FALSUM;
            }
            *cella = *(StmlNodus**)xar_obtinere(involucrum->liberi,
                i);
        }
        involucrum->liberi = liberi_novi;
    }
    redde VERUM;
}


/* ==================================================
 * Familia parametrorum - compressio formarum summarum (spec
 * macronum par. 6.1; mensuratum 2026-08-26: sceleta 52/189 per
 * gradus, formae summae = 81-87% parametrorum ambobus gradibus).
 *
 * Involucra structurae (specificatores/declarator/tok_*) in
 * definitiones capitis SEMEL; contenta tok in argumenta
 * SUBARBOREA bloci ('<@typus=>...</>'). Axis participationis
 * lexN (vocatio communis / lexema inscriptum / definitio
 * primi-usus) in argumentum solvitur - templum unum formas
 * plures tegit (productum -> summa).
 *
 * CONGRUENTIA = definitio retro currens (sceleti ambulantis v2
 * embryo, structura congruentia -> tabula argumentorum, machina
 * generali substituibilis): ambulatio gradaria corporis
 * definitionis contra candidatum - elementa litteralia octetim
 * (titulus/attributa/genus), textus '&@x;' TOTUS liberum unicum
 * definitionis = captura SILVAE liberorum candidati (loculus
 * iteratus silvas aequales poscit). Capturae MOVENTUR (numquam
 * clonantur) in elementa argumentorum - definitiones '<#lexN>'
 * primi-usus identitatem unam servant (probatum 2026-08-26:
 * expansio def semel transfert; canon def intra argumentum
 * invenit et '<<#lexN>>' posteriora resolvit).
 * ================================================== */

interior constans character* _par_familia_fons =
    "<familia>"
    "<#@par-nomina typus=\"@typus\" nomen=\"@nomen\">"
    "<parametrum><specificatores>"
    "<typus-nominatus><tok_titulus>&@typus;</tok_titulus>"
    "</typus-nominatus>"
    "<typus-nominatus><tok_titulus>&@nomen;</tok_titulus>"
    "</typus-nominatus>"
    "</specificatores></parametrum></#>"
    "<#@par-declaratum typus=\"@typus\" nomen=\"@nomen\">"
    "<parametrum><specificatores>"
    "<typus-nominatus><tok_titulus>&@typus;</tok_titulus>"
    "</typus-nominatus>"
    "</specificatores><declarator>"
    "<declarator-titulus><tok_titulus>&@nomen;</tok_titulus>"
    "</declarator-titulus>"
    "</declarator></parametrum></#>"
    "<#@par-monstratum typus=\"@typus\" stella=\"@stella\""
    " nomen=\"@nomen\">"
    "<parametrum><specificatores>"
    "<typus-nominatus><tok_titulus>&@typus;</tok_titulus>"
    "</typus-nominatus>"
    "</specificatores><declarator><declarator-monstrator>"
    "<tok_stella>&@stella;</tok_stella>"
    "<qualificatores/>"
    "<internum><declarator-titulus>"
    "<tok_titulus>&@nomen;</tok_titulus>"
    "</declarator-titulus></internum>"
    "</declarator-monstrator></declarator></parametrum></#>"
    "</familia>";

nomen structura {
     StmlNodus* definitio;  /* <#@par-...> */
     StmlNodus* corpus;     /* liberum primum (parametrum) */
        chorda* id;
           b32  adhibita;
} ParTemplum;

nomen structura {
     StmlNodus* nodus;      /* parametrum congruens */
     StmlNodus* parens;     /* cella stabilis - clavis tabulae */
    ParTemplum* templum;
           Xar* capturae;   /* StmlCaptura */
} ParCongruentia;

/* Matcher gradarius in MACHINAM promotus (stml_congruere_strictum,
 * lib/stml_macros.c - spec exemplarium par. 4, gradus I): logica
 * verbatim mota, recognitio hic VOCANS tenuis facta. */

/* Candidatum contra familiam temptare - primum templum congruens
 * vincit (structurae disiunctae, ordo indifferens). Defectus
 * memoriae = candidatum praeterire (compressio optionalis est -
 * exitus incompressus validus manet). */
interior ParCongruentia*
_par_temptare (
    ArborScriptor* scriptor,
        StmlNodus* cand,
              Xar* templa,
              Xar* congruentiae,
              Xar* paria)
{
    i32 t;

    per (t = ZEPHYRUM; t < xar_numerus(templa); t++)
    {
        ParTemplum* templum =
            (ParTemplum*)xar_obtinere(templa, t);
               Xar* capturae;
               i32  paria_initium;

        si (templum == NIHIL)
        {
            perge;
        }
        capturae = xar_creare(scriptor->piscina,
            magnitudo(StmlCaptura));
        si (capturae == NIHIL)
        {
            redde NIHIL;
        }
        paria_initium = xar_numerus(paria);
        si (stml_congruere_strictum(scriptor->piscina,
                scriptor->intern, templum->corpus, cand,
                capturae, paria))
        {
            ParCongruentia* con =
                (ParCongruentia*)xar_addere(congruentiae);

            si (con == NIHIL)
            {
                xar_truncare(paria, paria_initium);
                redde NIHIL;
            }
            con->nodus         = cand;
            con->parens        = cand->parens;
            con->templum       = templum;
            con->capturae      = capturae;
            templum->adhibita  = VERUM;
            redde con;
        }
        xar_truncare(paria, paria_initium);
    }
    redde NIHIL;
}

interior vacuum
_parametra_colligere (
    ArborScriptor* scriptor,
        StmlNodus* n,
              Xar* templa,
              Xar* congruentiae,
              Xar* paria)
{
    i32 i;
    i32 num;

    si (n == NIHIL || n->genus != STML_NODUS_ELEMENTUM)
    {
        redde;
    }
    num = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(n, i);

        si (l == NIHIL)
        {
            perge;
        }
        si (   l->genus   == STML_NODUS_ELEMENTUM
            && l->titulus != NIHIL
            && chorda_aequalis_literis(*l->titulus, "parametrum"))
        {
            scriptor->census.parametra_visa++;
            si (_par_temptare(scriptor, l, templa, congruentiae,
                    paria) != NIHIL)
            {
                perge;  /* congruens - non descendere */
            }
        }
        _parametra_colligere(scriptor, l, templa, congruentiae,
            paria);
    }
}

interior b32
_parametra_comprimere (
    ArborScriptor* scriptor,
        StmlNodus* involucrum)
{
      StmlResultus  familia_res;
               Xar* templa;        /* ParTemplum */
               Xar* congruentiae;  /* ParCongruentia */
               Xar* paria;         /* StmlCongruentiaPar */
    TabulaDispersa* congruentium;  /* &con->nodus -> con */
    TabulaDispersa* substituti;    /* &par->vetus -> novus */
    TabulaDispersa* refecti;       /* &con->parens -> VERUM */
               i32  i;
               i32  num;

    familia_res = stml_legere(
        chorda_ex_literis(_par_familia_fons, scriptor->piscina),
        scriptor->piscina, scriptor->intern);
    si (   !familia_res.successus
        || familia_res.elementum_radix == NIHIL)
    {
        scriptor->causa = "familia parametrorum parsari non potuit";
        redde FALSUM;
    }
    templa = xar_creare(scriptor->piscina, magnitudo(ParTemplum));
    congruentiae = xar_creare(scriptor->piscina,
        magnitudo(ParCongruentia));
    paria = xar_creare(scriptor->piscina,
        magnitudo(StmlCongruentiaPar));
    si (templa == NIHIL || congruentiae == NIHIL || paria == NIHIL)
    {
        scriptor->causa = "familia parametrorum parari non potuit";
        redde FALSUM;
    }
    num = stml_numerus_liberorum(familia_res.elementum_radix);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(
            familia_res.elementum_radix, i);
        ParTemplum* templum;

        si (   l                == NIHIL || !l->fragmentum
            || l->fragmentum_id == NIHIL)
        {
            perge;
        }
        templum = (ParTemplum*)xar_addere(templa);
        si (templum == NIHIL)
        {
            redde FALSUM;
        }
        templum->definitio  = l;
        templum->corpus     = stml_liberum_ad_indicem(l, ZEPHYRUM);
        templum->id         = l->fragmentum_id;
        templum->adhibita   = FALSUM;
        si (templum->corpus == NIHIL)
        {
            scriptor->causa = "templum parametri sine corpore";
            redde FALSUM;
        }
    }

    _parametra_colligere(scriptor, involucrum, templa,
        congruentiae, paria);
    scriptor->census.parametra_compressa +=
        xar_numerus(congruentiae);
    si (xar_numerus(congruentiae) == ZEPHYRUM)
    {
        redde VERUM;
    }

    congruentium = tabula_dispersa_creare_chorda(scriptor->piscina,
        256);
    substituti = tabula_dispersa_creare_chorda(scriptor->piscina,
        256);
    refecti = tabula_dispersa_creare_chorda(scriptor->piscina, 64);
    si (   congruentium == NIHIL || substituti == NIHIL
        || refecti      == NIHIL)
    {
        scriptor->causa = "tabulae parametrorum parari non potuerunt";
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(congruentiae); i++)
    {
        ParCongruentia* con =
            (ParCongruentia*)xar_obtinere(congruentiae, i);
        chorda clavis;

        clavis.datum    = (i8*)&con->nodus;
        clavis.mensura  = (i32)magnitudo(con->nodus);
        tabula_dispersa_inserere(congruentium, clavis, con);
    }

    /* parentes reficere: liberum congruens -> vocatio + argumenta
     * bloci (insertio media Xari non est - liberi novi) */
    per (i = ZEPHYRUM; i < xar_numerus(congruentiae); i++)
    {
        ParCongruentia* con =
            (ParCongruentia*)xar_obtinere(congruentiae, i);
                chorda  clavis;
                   Xar* liberi_novi;
                   i32  j;

        clavis.datum    = (i8*)&con->parens;
        clavis.mensura  = (i32)magnitudo(con->parens);
        si (tabula_dispersa_continet(refecti, clavis))
        {
            perge;
        }
        tabula_dispersa_inserere(refecti, clavis, (vacuum*)VERUM);

        liberi_novi = xar_creare(scriptor->piscina,
            magnitudo(StmlNodus*));
        si (liberi_novi == NIHIL)
        {
            scriptor->causa =
                "liberi parametrorum parari non potuerunt";
            redde FALSUM;
        }
        per (j = ZEPHYRUM;
             j < xar_numerus(con->parens->liberi); j++)
        {
            StmlNodus* liberum = *(StmlNodus**)xar_obtinere(
                con->parens->liberi, j);
            StmlNodus** cella;
               vacuum*  valor;
               chorda   clavis_liberi;

            clavis_liberi.datum    = (i8*)&liberum;
            clavis_liberi.mensura  = (i32)magnitudo(liberum);
            si (   liberum != NIHIL
                && tabula_dispersa_invenire(congruentium,
                       clavis_liberi, &valor))
            {
                ParCongruentia* cx = (ParCongruentia*)valor;
                     StmlNodus* vocatio;
                     character  valor_litterae[64];
                        chorda  interior_vocationis;
                           i32  k;

                valor_litterae[ZEPHYRUM] = '#';
                memcpy(valor_litterae + I, cx->templum->id->datum,
                    (memoriae_index)cx->templum->id->mensura);
                interior_vocationis.datum    = (i8*)valor_litterae;
                interior_vocationis.mensura  =
                    I + cx->templum->id->mensura;
                vocatio = stml_transclusionem_creare(
                    scriptor->piscina, scriptor->intern,
                    interior_vocationis);
                si (vocatio == NIHIL)
                {
                    scriptor->causa =
                        "vocatio parametri creari non potuit";
                    redde FALSUM;
                }
                cella = (StmlNodus**)xar_addere(liberi_novi);
                si (cella == NIHIL)
                {
                    redde FALSUM;
                }
                *cella           = vocatio;
                vocatio->parens  = con->parens;
                per (k = ZEPHYRUM;
                     k < xar_numerus(cx->capturae); k++)
                {
                    StmlCaptura* captura =
                        (StmlCaptura*)xar_obtinere(cx->capturae, k);
                     StmlNodus* argumentum;
                           i32  m;

                    argumentum = stml_elementum_creare(
                        scriptor->piscina, scriptor->intern, "@");
                    si (argumentum == NIHIL)
                    {
                        scriptor->causa =
                            "argumentum parametri creari non potuit";
                        redde FALSUM;
                    }
                    argumentum->attributum_titulus =
                        captura->titulus;
                    per (m = ZEPHYRUM;
                         m < xar_numerus(captura->nodi); m++)
                    {
                        StmlNodus* motus = *(StmlNodus**)
                            xar_obtinere(captura->nodi, m);

                        si (   motus == NIHIL
                            || !stml_liberum_addere(argumentum,
                                   motus))
                        {
                            scriptor->causa =
                                "captura parametri moveri non potuit";
                            redde FALSUM;
                        }
                    }
                    cella = (StmlNodus**)xar_addere(liberi_novi);
                    si (cella == NIHIL)
                    {
                        redde FALSUM;
                    }
                    *cella              = argumentum;
                    argumentum->parens  = con->parens;
                }
                perge;
            }
            cella = (StmlNodus**)xar_addere(liberi_novi);
            si (cella == NIHIL)
            {
                redde FALSUM;
            }
            *cella = liberum;
        }
        con->parens->liberi = liberi_novi;
    }

    /* paria sedium repungere: involucra dissoluta ad corpus
     * definitionis (sedes definitionis, exemplar foliorum) */
    per (i = ZEPHYRUM; i < xar_numerus(paria); i++)
    {
        StmlCongruentiaPar* par =
            (StmlCongruentiaPar*)xar_obtinere(paria, i);
           chorda clavis;

        clavis.datum    = (i8*)&par->vetus;
        clavis.mensura  = (i32)magnitudo(par->vetus);
        tabula_dispersa_inserere(substituti, clavis, par->novus);
    }
    /* repunctio parium per sessionem materiae (T10c) */
    materia_arbor_scriptor_repungere(scriptor->sessio, substituti);

    /* definitiones adhibitas in caput inserere (ordine familiae -
     * determinatum, independens invocationis), post [fontes,
     * def-post, def-ante] */
    {
        Xar* liberi_novi;
        i32  caput_finis  = III;
        b32  ulla         = FALSUM;

        per (i = ZEPHYRUM; i < xar_numerus(templa); i++)
        {
            ParTemplum* t = (ParTemplum*)xar_obtinere(templa, i);

            si (t != NIHIL && t->adhibita)
            {
                ulla = VERUM;
            }
        }
        si (!ulla)
        {
            redde VERUM;
        }
        liberi_novi = xar_creare(scriptor->piscina,
            magnitudo(StmlNodus*));
        si (liberi_novi == NIHIL)
        {
            scriptor->causa = "caput parametrorum parari non potuit";
            redde FALSUM;
        }
        per (i = ZEPHYRUM;
             i < xar_numerus(involucrum->liberi); i++)
        {
            StmlNodus** cella;

            si (i == caput_finis)
            {
                i32 k;

                per (k = ZEPHYRUM; k < xar_numerus(templa); k++)
                {
                    ParTemplum* t =
                        (ParTemplum*)xar_obtinere(templa, k);

                    si (t == NIHIL || !t->adhibita)
                    {
                        perge;
                    }
                    cella = (StmlNodus**)xar_addere(liberi_novi);
                    si (cella == NIHIL)
                    {
                        redde FALSUM;
                    }
                    *cella                = t->definitio;
                    t->definitio->parens  = involucrum;
                }
            }
            cella = (StmlNodus**)xar_addere(liberi_novi);
            si (cella == NIHIL)
            {
                redde FALSUM;
            }
            *cella = *(StmlNodus**)xar_obtinere(
                involucrum->liberi, i);
        }
        involucrum->liberi = liberi_novi;
    }
    redde VERUM;
}

SilvaArborScriptura
silva_arbor_scribere_parsuram (
                          Piscina* piscina,
            constans SilvaParsura* parsura,
    constans SilvaRegistrumCoctum* tabularium,
               constans character* grammatica,
                              s32  fons_index,
              InternamentumChorda* intern)
{
          ArborScriptor  scriptor;
    SilvaArborScriptura  fructus;
              StmlNodus* involucrum;
                 chorda  sigillum;
             SilvaValor  radix;
                    i32  numerus;
                    i32  i;
             SilvaFrons* frons;
  MateriaArborConsilium  consilium;
  MateriaArborScriptura  ms;
     constans character* causa_sessionis = NIHIL;

    fructus.successus                   = FALSUM;
    fructus.textus.datum                = NIHIL;
    fructus.textus.mensura              = ZEPHYRUM;
    fructus.causa                       = NIHIL;
    fructus.sedes                       = NIHIL;
    fructus.sedes_valorum               = NIHIL;
    fructus.census.spatia_vocationes    = ZEPHYRUM;
    fructus.census.folia_formae         = ZEPHYRUM;
    fructus.census.folia_vocationes     = ZEPHYRUM;
    fructus.census.parametra_visa       = ZEPHYRUM;
    fructus.census.parametra_compressa  = ZEPHYRUM;

    si (   piscina            == NIHIL || parsura == NIHIL
        || tabularium         == NIHIL || grammatica == NIHIL
        || parsura->commissio == NIHIL)
    {
        fructus.causa = "argumenta nihil";
        redde fructus;
    }
    si (intern == NIHIL)
    {
        intern = internamentum_creare(piscina);
        si (intern == NIHIL)
        {
            fructus.causa = "internamentum creari non potuit";
            redde fructus;
        }
    }

    /* SESSIO MATERIAE (T10c): unci C89 super caudam silvae, templa
     * activa (parsura sola caput definitionum fert), sedes valorum
     * collectae. */
    frons = silva_frons_creare(piscina, parsura->expansio);
    si (   frons == NIHIL
        || !silva_frons_arborem_silvae_parare(frons, tabularium,
               grammatica, intern, &consilium))
    {
        fructus.causa = "frons C89 parari non potuit";
        redde fructus;
    }
    consilium.templa_activa    = VERUM;
    consilium.sedes_colligere  = VERUM;

    scriptor.piscina  = piscina;
    scriptor.intern   = intern;
    scriptor.causa    = NIHIL;
    memset(&scriptor.census, 0, magnitudo(scriptor.census));
    scriptor.sessio   = materia_arbor_scriptor_creare(piscina,
        &consilium,
        &causa_sessionis);
    si (scriptor.sessio == NIHIL)
    {
        fructus.causa = causa_sessionis ? causa_sessionis
                                        : "sessio creari non potuit";
        redde fructus;
    }

    radix = parsura->commissio->radix;

    /* PASSUS I - numeratio usuum, UNA per documentum TOTUM */
    materia_arbor_scriptor_numerare(scriptor.sessio, radix);

    /* POST passum I, quia _numerare_lexema ancoram ex lexemate PRIMO
     * ambulationis ponit - quod in plagula latinizata ex CAPITE venit
     * (latina.h), non ex principe. Statutio ante passum I silenter
     * deleretur. Vide notam ad scriptor.ancora_fons supra. */
    materia_arbor_scriptor_fontem_ponere(scriptor.sessio,
        parsura->fons_princeps);

    involucrum = stml_elementum_creare(piscina, intern,
        SILVA_ARBOR_TAG_PARSURA);
    si (involucrum == NIHIL)
    {
        fructus.causa = "involucrum creari non potuit";
        redde fructus;
    }
    stml_attributum_addere(involucrum, piscina, intern, "grammatica",
        grammatica);

    sigillum = silva_arbor_sigillum(piscina, tabularium);
    si (sigillum.mensura == ZEPHYRUM)
    {
        fructus.causa = "sigillum computari non potuit";
        redde fructus;
    }
    stml_attributum_addere_chorda(involucrum, piscina, intern,
        "registrum-sigillum", sigillum);

    /* NULLA ANCORA: plagula ipsa initium est (spec §1) - sedes
     * omnes ab offset ZEPHYRUM, linea I derivantur. */
    si (fons_index >= ZEPHYRUM)
    {
        _attributum_numeri(&scriptor, involucrum, "fons-princeps",
            (i32)fons_index);
    }

    si (!_parsura_fontes_scribere(&scriptor, involucrum,
             parsura->expansio))
    {
        fructus.causa = "sectio fontium scribi non potuit";
        redde fructus;
    }

    /* TEMPLA (macros v1): definitiones post <fontes>, ante
     * vocationes omnes - strata ordine documenti. Ordo gravis:
     * folia macronum (infra, post arborem constructam) vocationes
     * spatiorum in corporibus ferre possunt - definitiones
     * spatiorum PRIORES esse debent. */
    si (!materia_arbor_templa_spatiorum_scribere(scriptor.sessio,
            involucrum))
    {
        fructus.causa = materia_arbor_scriptor_causa(scriptor.sessio)
            ? materia_arbor_scriptor_causa(scriptor.sessio)
            : "definitio templi scribi non potuit";
        redde fructus;
    }

    /* PASSUS II - arbor. Nodi supremi LIBERI DIRECTI involucri
     * sunt (non in involucro listae) ut ordo documenti ordo
     * plagulae sit. */
    si (radix.genus == SILVA_VALOR_LISTA)
    {
        Xar* acervus;
        s32  proximus_regionis;
        i32  reinserendorum;
        i32  d;
        i32  k;

        /* Omnia quae ARBORI non pertinent in unam listam, deinde
         * ordine octetorum. Ordinatio per offset HIC licita est et
         * HIC SOLUM - res strati fluxus, non arboris
         * (silva_scribere.h:19). */
        acervus = xar_creare(piscina, magnitudo(ParsuraReinserendum));
        si (acervus == NIHIL)
        {
            fructus.causa = "acervus creari non potuit";
            redde fructus;
        }
        si (parsura->directivae != NIHIL)
        {
            per (k = ZEPHYRUM; k < xar_numerus(parsura->directivae);
                 k++)
            {
                si (!_parsura_reinserendum_addere(acervus,
                         *(Xar**)xar_obtinere(parsura->directivae, k),
                         PARSURA_REINS_DIRECTIVA, -I, -I, -I,
                         SILVA_RAMUS_IF, ZEPHYRUM, -I, ZEPHYRUM,
                         fons_index))
                {
                    fructus.causa = "directiva colligi non potuit";
                    redde fructus;
                }
            }
        }
        /* INVOCATIONES VACUAE - eodem iure quo directivae:
         * octetos tegunt quos arbor non fert. */
        si (   parsura->expansio          != NIHIL
            && parsura->expansio->extenta != NIHIL)
        {
            per (k = ZEPHYRUM;
                 k < xar_numerus(parsura->expansio->extenta); k++)
            {
                SilvaExtentumInvocationis* ext =
                    (SilvaExtentumInvocationis*)xar_obtinere(
                        parsura->expansio->extenta, k);

                si (ext == NIHIL || !ext->vacua)
                {
                    perge;
                }
                si (!_parsura_reinserendum_addere(acervus, ext->lamina,
                         PARSURA_REINS_INVOCATIO_VACUA, -I, -I, -I,
                         SILVA_RAMUS_IF, ZEPHYRUM, -I, ZEPHYRUM,
                         fons_index))
                {
                    fructus.causa =
                        "invocatio vacua colligi non potuit";
                    redde fructus;
                }
            }
        }
        proximus_regionis = ZEPHYRUM;
        si (parsura->expansio != NIHIL)
        {
            si (!_parsura_regiones_colligere(acervus,
                     parsura->expansio->regiones, -I,
                     &proximus_regionis, fons_index))
            {
                fructus.causa = "regiones colligi non potuerunt";
                redde fructus;
            }
        }
        xar_ordinare(acervus, _parsura_reinserenda_conferre);

        numerus         = silva_valor_lista_numerus(radix);
        reinserendorum  = xar_numerus(acervus);
        d               = ZEPHYRUM;
        i               = ZEPHYRUM;

        per (;;)
        {
             ParsuraReinserendum* r;
                      SilvaValor* elementum;
                       StmlNodus* scriptum;
                             s32  offset_nodi;

            r = d < reinserendorum
                ? (ParsuraReinserendum*)xar_obtinere(acervus, d)
                : NIHIL;
            elementum    = NIHIL;
            offset_nodi  = -I;

            si (i < numerus)
            {
                elementum = silva_valor_lista_obtinere(radix, i);
                si (elementum != NIHIL)
                {
                    offset_nodi = _parsura_primum_offset(*elementum);
                }
            }
            si (r == NIHIL && elementum == NIHIL)
            {
                frange;
            }
            si (   r != NIHIL
                && (elementum == NIHIL || r->offset <= offset_nodi))
            {
                scriptum = _parsura_reinserendum_scribere(&scriptor,
                    r);
                si (scriptum == NIHIL)
                {
                    fructus.causa = scriptor.causa
                                  ? scriptor.causa
                                  : "reinserendum fractum";
                    redde fructus;
                }
                si (!stml_liberum_addere(involucrum, scriptum))
                {
                    fructus.causa =
                        "reinserendum in involucrum addi non potuit";
                    redde fructus;
                }
                d++;
                perge;
            }
            si (   elementum->genus       != SILVA_VALOR_NODUS
                || elementum->datum.nodus == NIHIL)
            {
                fructus.causa = "radix elementum non-nodale fert";
                redde fructus;
            }
            scriptum = materia_arbor_nodum_scribere(scriptor.sessio,
                elementum->datum.nodus);
            si (scriptum == NIHIL)
            {
                ms = materia_arbor_scriptor_finire(scriptor.sessio,
                    involucrum, FALSUM);
                fructus.causa =
                    ms.causa ? ms.causa : "scriptura fracta";
                fructus.sedes = ms.sedes;
                redde fructus;
            }
            _parsura_ancoram_scribere(&scriptor, scriptum,
                _parsura_primum_lexema(*elementum));
            si (!stml_liberum_addere(involucrum, scriptum))
            {
                fructus.causa = "nodus in involucrum addi non potuit";
                redde fructus;
            }
            i++;
        }
    }
    alioquin si (   radix.genus       == SILVA_VALOR_NODUS
                 && radix.datum.nodus != NIHIL)
    {
        StmlNodus* scriptum;

        scriptum = materia_arbor_nodum_scribere(scriptor.sessio,
            radix.datum.nodus);
        si (scriptum == NIHIL)
        {
            ms = materia_arbor_scriptor_finire(scriptor.sessio,
                involucrum, FALSUM);
            fructus.causa = ms.causa ? ms.causa : "scriptura fracta";
            fructus.sedes = ms.sedes;
            redde fructus;
        }
        si (!stml_liberum_addere(involucrum, scriptum))
        {
            fructus.causa = "nodus in involucrum addi non potuit";
            redde fructus;
        }
    }
    alioquin
    {
        fructus.causa = "radix nec lista nec nodus";
        redde fructus;
    }

    /* CAUDA: trivia plagulae post ultimum lexema. Custodia eadem
     * ac silva_scribere_fontem: lexema EOF plagulae HUIUS solum. */
    si (   parsura->lexema_finis             != NIHIL
        && fons_index                        >= ZEPHYRUM
        && parsura->lexema_finis->fons_index != fons_index)
    {
        /* Plagula INCLUSA: EOF suum in includendis manet, non in
         * parsura->lexema_finis (silva_scribere.c:727-750 illam
         * semitam sequitur). Documentum pro plagula inclusa nondum
         * SPECIFICATUM est (spec §8, apertum), ergo RECUSAMUS
         * NOMINATIM potius quam caudam tacite omittimus - omissio
         * tacita documentum mendax faceret. */
        fructus.causa = "cauda plagulae inclusae nondum lata";
        redde fructus;
    }
    si (   parsura->lexema_finis != NIHIL
        && (   fons_index < ZEPHYRUM
            || parsura->lexema_finis->fons_index == fons_index))
    {
        StmlNodus* cauda;
        StmlNodus* lexema;

        cauda = stml_elementum_creare(piscina, intern,
            SILVA_ARBOR_TAG_CAUDA);
        si (cauda == NIHIL)
        {
            fructus.causa = "cauda creari non potuit";
            redde fructus;
        }
        _parsura_ancoram_scribere(&scriptor, cauda,
            parsura->lexema_finis);
        lexema = materia_arbor_lexema_scribere(scriptor.sessio,
            parsura->lexema_finis);
        si (lexema == NIHIL)
        {
            fructus.causa = scriptor.causa ? scriptor.causa
                                           : "cauda scribi non potuit";
            redde fructus;
        }
        si (   !stml_liberum_addere(cauda, lexema)
            || !stml_liberum_addere(involucrum, cauda))
        {
            fructus.causa = "cauda in involucrum addi non potuit";
            redde fructus;
        }
    }

    /* FOLIA MACRONUM: post arborem INTEGRAM constructam (formae ex
     * contento finali numerantur), ante scriptionem. */
    si (!_folia_macronum_comprimere(&scriptor, involucrum))
    {
        fructus.causa = scriptor.causa != NIHIL
            ? scriptor.causa
            : "folia macronum comprimi non potuerunt";
        redde fructus;
    }

    /* FAMILIA PARAMETRORUM: post folia (contenta tok iam
     * compressa in argumenta cadunt), ante scriptionem. */
    si (!_parametra_comprimere(&scriptor, involucrum))
    {
        fructus.causa = scriptor.causa != NIHIL
            ? scriptor.causa
            : "parametra comprimi non potuerunt";
        redde fructus;
    }

    /* Textus + sedes valorum per sessionem (paria compressione iam
     * repuncta) */
    ms = materia_arbor_scriptor_finire(scriptor.sessio, involucrum,
        VERUM);
    si (!ms.successus)
    {
        fructus.causa = ms.causa ? ms.causa : "scriptura fracta";
        fructus.sedes = ms.sedes;
        redde fructus;
    }
    fructus.textus                    = ms.textus;
    fructus.sedes_valorum             = ms.sedes_valorum;
    fructus.census                    = scriptor.census;
    fructus.census.spatia_vocationes  = ms.census.spatia_vocationes;
    fructus.successus                 = VERUM;
    redde fructus;
}

/* Sectionem fontium relegere in expansionem novam. Indices
 * documenti contra indices redditos PROBANTUR: tabula fontium ordinata est, et
 * divergentia significaret fons_index cuiusque lexematis alium
 * fontem nominare - mendacium tacitum. */
interior b32
_parsura_fontes_legere (
      ArborLector* lector,
        StmlNodus* involucrum,
    SilvaExpansio* expansio)
{
    StmlNodus* sectio;
    StmlNodus* elem;
          i32  cursor;
          i32  intra;

    cursor = ZEPHYRUM;
    sectio = _silvae_elementum_proximum(lector, involucrum, &cursor);
    si (   sectio == NIHIL || sectio->titulus == NIHIL
        || !chorda_aequalis_literis(*sectio->titulus,
               SILVA_ARBOR_TAG_FONTES))
    {
        _recusare(lector, "sectio <fontes> deest",
            sectio ? sectio->linea : involucrum->linea);
        redde FALSUM;
    }

    intra = ZEPHYRUM;
    dum ((elem = _silvae_elementum_proximum(lector, sectio, &intra))
             != NIHIL)
    {
           chorda* via;
           chorda* attributum;
        character* nullata;
        SilvaFons* fons;
              i32  index_documenti;
              s32  index_redditus;

        si (   elem->titulus == NIHIL
            || !chorda_aequalis_literis(*elem->titulus,
                   SILVA_ARBOR_TAG_FONS))
        {
            _recusare(lector, "elementum non-<fons> in <fontes>",
                elem->linea);
            redde FALSUM;
        }
        via = stml_attributum_capere(elem, "via");
        si (via == NIHIL)
        {
            _recusare(lector, "<fons> sine via", elem->linea);
            redde FALSUM;
        }
        nullata = _parsura_via_nullata(lector->piscina, via);
        si (nullata == NIHIL)
        {
            _recusare(lector, "via transcribi non potuit",
                elem->linea);
            redde FALSUM;
        }
        index_redditus = silva_fons_addere(expansio, nullata,
            stml_attributum_habet(elem, "syntheticus"));
        si (index_redditus < ZEPHYRUM)
        {
            _recusare(lector, "fons addi non potuit", elem->linea);
            redde FALSUM;
        }
        attributum = stml_attributum_capere(elem, "index");
        si (   attributum           != NIHIL
            && _numerus_ex_chorda(attributum, &index_documenti)
            && (s32)index_documenti != index_redditus)
        {
            _recusare(lector, "index fontis non congruit",
                elem->linea);
            redde FALSUM;
        }
        fons = (SilvaFons*)xar_obtinere(expansio->fontes,
            (i32)index_redditus);
        si (fons != NIHIL)
        {
            fons->est_lexicon = stml_attributum_habet(elem,
                "lexicon");
        }
    }
    redde VERUM;
}

/* Lacunas ordine offset */
interior s32
_parsura_lacunas_conferre (
    constans vacuum* a,
    constans vacuum* b)
{
    constans ParsuraLacuna* la;
    constans ParsuraLacuna* lb;

    la = (constans ParsuraLacuna*)a;
    lb = (constans ParsuraLacuna*)b;
    si (la->offset < lb->offset)
    {
        redde -I;
    }
    si (la->offset > lb->offset)
    {
        redde I;
    }
    redde ZEPHYRUM;
}

/* Fons laminae = fons lexematis primi (lamina una plagula constat -
 * directiva aut ramus regionis in una plagula iacet). -I si vacua. */
interior s32
_laminae_fons (
    constans Xar* lamina)
{
    constans SilvaToken** sedes;

    si (lamina == NIHIL || xar_numerus(lamina) == ZEPHYRUM)
    {
        redde -I;
    }
    sedes = (constans SilvaToken**)xar_obtinere(lamina, ZEPHYRUM);
    si (sedes == NIHIL || *sedes == NIHIL)
    {
        redde -I;
    }
    redde (*sedes)->fons_index;
}

/* Lacunam notare: ab initio dato usque ad sedem quam cursor post
 * laminam lectam tenet. Extentum ergo ex DERIVATIONE IPSA venit -
 * nullus fons veritatis secundus. */
interior b32
_parsura_lacunam_notare (
                            Xar* lacunae,
                            s32  initium,
    constans MateriaArborCursor* post,
                            s32  fons)
{
    ParsuraLacuna* lacuna;

    si (lacunae == NIHIL || post->offset <= initium)
    {
        redde VERUM;
    }
    lacuna = (ParsuraLacuna*)xar_addere(lacunae);
    si (lacuna == NIHIL)
    {
        redde FALSUM;
    }
    lacuna->offset               = initium;
    lacuna->finis                = post->offset;
    lacuna->fons                 = fons;
    lacuna->linea_finalis        = post->linea;
    lacuna->columna_finalis      = post->columna;
    lacuna->post_lineam_finalis  = post->post_lineam;
    redde VERUM;
}

/* Ancora elementi: materia_arbor_fixura_ancoram_legere (T10a) -
 * portata ex hac sede cum re-quaesitione indicis lacunarum. */

/* Lexemata liberorum elementi in laminam novam. Directivae et
 * laminae crudae eandem formam habent - series lexematum. Lexema per
 * sessionem materiae (fragmentum ipsa aperit - lex quae TER hic
 * fefellit, nunc in sutura), sedes ordine DOCUMENTI per fixuram:
 * laminae in fluxu octetorum inter nodos iacent. */
interior Xar*
_parsura_laminam_legere (
            ArborLector* lector,
     MateriaArborLector* sessio,
     MateriaArborFixura* fixura,
              StmlNodus* elementum)
{
           Xar* lamina;
     StmlNodus* liber;
           i32  cursor;

    lamina = xar_creare(lector->piscina, magnitudo(SilvaToken*));
    si (lamina == NIHIL)
    {
        _recusare(lector, "lamina creari non potuit", elementum->linea);
        redde NIHIL;
    }
    cursor = ZEPHYRUM;
    dum ((liber = _silvae_elementum_proximum(lector, elementum,
        &cursor))
             != NIHIL)
    {
         SilvaToken*  lexema;
         SilvaToken** sedes_lexematis;

        lexema = materia_arbor_lexema_legere(sessio, liber, NIHIL);
        si (lexema == NIHIL)
        {
            redde NIHIL;
        }
        materia_arbor_positiones_lexematis(fixura, lexema);
        sedes_lexematis = (SilvaToken**)xar_addere(lamina);
        si (sedes_lexematis == NIHIL)
        {
            _recusare(lector, "lexema in laminam addi non potuit",
                elementum->linea);
            redde NIHIL;
        }
        *sedes_lexematis = lexema;
    }
    redde lamina;
}

/* Regionem indicis dati obtinere aut creare. Tabula per indicem
 * documenti crescit; lacunae NIHIL manent (ordo documenti ordinem
 * indicum non promittit). */
interior SilvaRegio*
_parsura_regionem_obtinere (
     ArborLector* lector,
             Xar* tabula,
             Xar* patres,
             s32  index,
             s32  pater_index,
             s32  fons,
             i32  linea)
{
     SilvaRegio*  regio;
     SilvaRegio** sedes;
            s32*  sedes_patris;

    si (index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    dum (xar_numerus(tabula) <= (i32)index)
    {
        sedes = (SilvaRegio**)xar_addere(tabula);
        si (sedes == NIHIL)
        {
            redde NIHIL;
        }
        *sedes        = NIHIL;
        sedes_patris  = (s32*)xar_addere(patres);
        si (sedes_patris == NIHIL)
        {
            redde NIHIL;
        }
        *sedes_patris = -I;
    }
    sedes = (SilvaRegio**)xar_obtinere(tabula, (i32)index);
    si (sedes == NIHIL)
    {
        redde NIHIL;
    }
    si (*sedes == NIHIL)
    {
        regio = (SilvaRegio*)piscina_allocare(lector->piscina,
            magnitudo(SilvaRegio));
        si (regio == NIHIL)
        {
            redde NIHIL;
        }
        memset(regio, ZEPHYRUM, magnitudo(SilvaRegio));
        regio->fons_index  = fons;
        regio->linea       = linea;
        regio->rami        = xar_creare(lector->piscina,
            magnitudo(SilvaRamus*));
        regio->filiae      = xar_creare(lector->piscina,
            magnitudo(SilvaRegio*));
        /* DEGRADATA per constructionem: regiones TEXTAE in ARBORE
         * vivunt, non hic - ergo quidquid in documento apparet
         * lineas suas POSSIDET (silva_expandere.h:130). */
        regio->est_texta  = FALSUM;
        *sedes            = regio;
    }
    sedes_patris = (s32*)xar_obtinere(patres, (i32)index);
    si (sedes_patris != NIHIL && *sedes_patris < ZEPHYRUM)
    {
        *sedes_patris = pater_index;
    }
    redde *sedes;
}

/* Ramum indicis dati intra regionem obtinere aut creare. */
interior SilvaRamus*
_parsura_ramum_obtinere (
     ArborLector* lector,
      SilvaRegio* regio,
             s32  index)
{
    SilvaRamus*  ramus;
    SilvaRamus** sedes;

    si (regio == NIHIL || index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    dum (xar_numerus(regio->rami) <= (i32)index)
    {
        sedes = (SilvaRamus**)xar_addere(regio->rami);
        si (sedes == NIHIL)
        {
            redde NIHIL;
        }
        *sedes = NIHIL;
    }
    sedes = (SilvaRamus**)xar_obtinere(regio->rami, (i32)index);
    si (sedes == NIHIL)
    {
        redde NIHIL;
    }
    si (*sedes == NIHIL)
    {
        ramus = (SilvaRamus*)piscina_allocare(lector->piscina,
            magnitudo(SilvaRamus));
        si (ramus == NIHIL)
        {
            redde NIHIL;
        }
        memset(ramus, ZEPHYRUM, magnitudo(SilvaRamus));
        ramus->regio           = regio;
        ramus->corpus_initium  = -I;
        ramus->corpus_finis    = -I;
        *sedes                 = ramus;
    }
    redde *sedes;
}

/* Elementum radix arboris EXPANSAE invenire. Expansio arborem NOVAM
 * reddit (documentum clonatum), ergo commoditas 'elementum_radix'
 * resultatus lectionis ad arborem VETEREM monstrat - hic primus
 * liberorum ELEMENTUM quaeritur. */
interior StmlNodus*
_expansae_elementum_radix (
    StmlNodus* radix)
{
    i32 i;
    i32 num;

    si (radix == NIHIL || radix->liberi == NIHIL)
    {
        redde NIHIL;
    }
    num = xar_numerus(radix->liberi);
    per (i = ZEPHYRUM; i < num; i++)
    {
        StmlNodus* liberum;

        liberum = *(StmlNodus**)xar_obtinere(radix->liberi, i);
        si (liberum != NIHIL && liberum->genus == STML_NODUS_ELEMENTUM)
        {
            redde liberum;
        }
    }
    redde NIHIL;
}

/* Longitudines laminae (T10c) - vide silva_frons_longitudines_
 * figere */
interior vacuum
_parsura_laminam_longitudine (
    SilvaFrons* frons,
           Xar* lamina)
{
    i32 k;

    per (k = ZEPHYRUM; lamina != NIHIL && k < xar_numerus(lamina); k++)
    {
        silva_frons_longitudines_figere(frons,
            silva_valor_token(*(SilvaToken**)xar_obtinere(lamina, k)));
    }
}

interior vacuum
_parsura_laminas_longitudine (
    SilvaFrons* frons,
           Xar* laminae)
{
    i32 k;

    per (k = ZEPHYRUM; laminae != NIHIL
        && k < xar_numerus(laminae); k++)
    {
        _parsura_laminam_longitudine(frons,
            *(Xar**)xar_obtinere(laminae, k));
    }
}

SilvaParsura*
silva_arbor_legere_parsuram (
                           Piscina* piscina,
               InternamentumChorda* intern,
                            chorda  textus,
     constans SilvaRegistrumCoctum* tabularium,
                constans character* grammatica,
                  SilvaArborVitium* vitium)
{
         ArborLector lector;
  MateriaArborFixura sedes;
MateriaArborLector* sessio;
        SilvaFrons* frons;
MateriaArborConsilium consilium;
     StmlResultus  resultus;
        StmlNodus* involucrum;
        StmlNodus* elem;
     SilvaParsura* parsura;
    SilvaExpansio* expansio;
              Xar* regiones_tabula;
              Xar* regiones_patres;
              Xar* lacunae;
              i32  passus;
       SilvaValor  radix;
           chorda* attributum;
           chorda  sigillum;
              i32  cursor;
              i32  numerus;

    si (vitium != NIHIL)
    {
        vitium->causa = NIHIL;
        vitium->linea = ZEPHYRUM;
    }

    lector.piscina  = piscina;
    lector.intern   = intern;
    lector.vitium   = vitium;

    si (piscina == NIHIL || tabularium == NIHIL || grammatica == NIHIL)
    {
        _recusare(&lector, "argumenta nihil", ZEPHYRUM);
        redde NIHIL;
    }
    si (intern == NIHIL)
    {
        intern = internamentum_creare(piscina);
        si (intern == NIHIL)
        {
            _recusare(&lector, "internamentum creari non potuit",
                ZEPHYRUM);
            redde NIHIL;
        }
        lector.intern = intern;
    }

    resultus = stml_legere(textus, piscina, intern);
    si (!resultus.successus)
    {
        _recusare(&lector, "STML parsari non potuit",
            resultus.linea_erroris);
        redde NIHIL;
    }

    /* EXPANSIO TEMPLORUM (macros v1): documentum formam macroneam
     * servat, lector visionem CONTENTI legit. Documentum sine
     * templis = transitio pura (fragmenta contenti <#lexN> et
     * transclusiones eorum INTACTA - spatium templi '#@' solum
     * tangitur, vide stml_macros.h). */
    {
        StmlExpansioResultus expansio;

        expansio = stml_expandere(resultus.radix, piscina, intern);
        si (!expansio.successus)
        {
            _recusare(&lector, "expansio templorum fracta",
                expansio.linea);
            redde NIHIL;
        }
        involucrum = _expansae_elementum_radix(expansio.radix_expansa);
    }
    si (   involucrum == NIHIL || involucrum->titulus == NIHIL
        || !chorda_aequalis_literis(*involucrum->titulus,
               SILVA_ARBOR_TAG_PARSURA))
    {
        _recusare(&lector, "involucrum <parsura> deest",
            involucrum ? involucrum->linea : ZEPHYRUM);
        redde NIHIL;
    }

    attributum = stml_attributum_capere(involucrum, "grammatica");
    si (   attributum == NIHIL
        || !chorda_aequalis_literis(*attributum, grammatica))
    {
        _recusare(&lector, "grammatica non congruit",
            involucrum->linea);
        redde NIHIL;
    }

    /* SIGILLUM: parsura vocabulario FALSO iudicata mendacium est */
    sigillum   = silva_arbor_sigillum(piscina, tabularium);
    attributum = stml_attributum_capere(involucrum,
        "registrum-sigillum");
    si (   attributum == NIHIL || sigillum.mensura == ZEPHYRUM
        || !chorda_aequalis(*attributum, sigillum))
    {
        _recusare(&lector, "sigillum registri non congruit",
            involucrum->linea);
        redde NIHIL;
    }

    expansio = silva_expansio_creare(piscina);
    si (expansio == NIHIL)
    {
        _recusare(&lector, "expansio creari non potuit",
            involucrum->linea);
        redde NIHIL;
    }
    si (!_parsura_fontes_legere(&lector, involucrum, expansio))
    {
        redde NIHIL;
    }

    /* SESSIO LECTIONIS MATERIAE (T10c): unci C89 super caudam silvae;
     * extenta lecta in expansionem NOVAM (silva_scribere_fontem ea
     * super parsuram lectam invenire debet). Tabula fragmentorum
     * DOCUMENTO-scopata in sessione vivit. */
    frons = silva_frons_creare(piscina, expansio);
    si (   frons == NIHIL
        || !silva_frons_arborem_silvae_parare(frons, tabularium,
               grammatica, intern, &consilium))
    {
        _recusare(&lector, "frons C89 parari non potuit",
            involucrum->linea);
        redde NIHIL;
    }
    silva_frons_extenta_lecta_ponere(frons, expansio->extenta);
    sessio = materia_arbor_lector_creare(piscina, intern, &consilium,
        vitium);
    si (sessio == NIHIL)
    {
        _recusare(&lector, "sessio lectionis creari non potuit",
            involucrum->linea);
        redde NIHIL;
    }

    parsura = (SilvaParsura*)piscina_allocare(piscina,
        magnitudo(SilvaParsura));
    si (parsura == NIHIL)
    {
        _recusare(&lector, "parsura allocari non potuit",
            involucrum->linea);
        redde NIHIL;
    }
    /* CAMPI EXCLUSI ZEPHYRO EXPLICITE - telemetria parsurae in
     * documento NON vivit (spec §1), ergo hic nasci debet, non
     * ex memoria non-initializata fluere. */
    memset(parsura, ZEPHYRUM, magnitudo(SilvaParsura));
    parsura->expansio       = expansio;
    parsura->fons_princeps  = -I;

    attributum = stml_attributum_capere(involucrum, "fons-princeps");
    si (   attributum != NIHIL && _numerus_ex_chorda(attributum,
            &numerus))
    {
        parsura->fons_princeps = (s32)numerus;
        materia_arbor_lector_fontem_ponere(sessio, (s32)numerus);
    }

    /* Liberi involucri ordine DOCUMENTI, qui ordo PLAGULAE est.
     * <fontes> iam consumpta; <cauda> caudam fert; cetera nodi. */
    /* SEDES ab INITIO PLAGULAE. Derivatio ordine DOCUMENTI fit,
     * non ordine arboris: directivae ex arbore RETRAHUNTUR sed in
     * FLUXU OCTETORUM inter nodos iacent, ergo cursor eas videre
     * DEBET - aliter offsets omnes post directivam primam labuntur
     * et silva_scribere_fontem (quae per offset intertexit)
     * ordinem falsum reficit. */
    materia_arbor_fixura_initiare(&sedes, &consilium, NIHIL);

    /* Cursor nunc PER LIBERUM ex ancora ponitur (vide
     * _parsura_ancoram_legere); haec initializatio defaltum solum
     * est pro liberis sine ancora. */
    regiones_tabula = xar_creare(piscina, magnitudo(SilvaRegio*));
    regiones_patres = xar_creare(piscina, magnitudo(s32));
    si (regiones_tabula == NIHIL || regiones_patres == NIHIL)
    {
        _recusare(&lector, "tabula regionum creari non potuit",
            involucrum->linea);
        redde NIHIL;
    }

    lacunae = xar_creare(piscina, magnitudo(ParsuraLacuna));
    si (lacunae == NIHIL)
    {
        _recusare(&lector, "lacunae creari non potuerunt",
            involucrum->linea);
        redde NIHIL;
    }

    radix = silva_valor_lista_nova(piscina);

    /* DUO PASSUS, et ordo eorum EST consilium: laminae (directivae,
     * regiones degradatae) LACUNAS in fluxu octetorum arboris
     * faciunt, ergo NOTAE esse debent antequam arbor derivetur.
     * Passus I laminas legit et lacunas notat; passus II arborem
     * cursore LACUNARUM CONSCIO derivat. */
    per (passus = ZEPHYRUM; passus < II; passus++)
    {
        si (passus == I)
        {
            xar_ordinare(lacunae, _parsura_lacunas_conferre);
            sedes.lacunae         = lacunae;
            sedes.lacuna_proxima  = ZEPHYRUM;
        }
        cursor = ZEPHYRUM;
        dum ((elem = _silvae_elementum_proximum(&lector, involucrum,
            &cursor))
                 != NIHIL)
        {
            SilvaNodus* nodus;

            si (   elem->titulus != NIHIL
                && chorda_aequalis_literis(*elem->titulus,
                       SILVA_ARBOR_TAG_FONTES))
            {
                perge;
            }
            /* REGIONES DEGRADATAE: laminae quae lineas suas POSSIDENT.
             * Arbor regionum hic REFICITUR ut _regiones_colligere
             * (silva_scribere.c) eadem reinserenda ad easdem sedes
             * inveniat. Regiones TEXTAE hic NUMQUAM apparent - illae in
             * ARBORE vivunt (dominus unus). */
            si (   elem->titulus != NIHIL
                && (   chorda_aequalis_literis(*elem->titulus,
                           SILVA_ARBOR_TAG_REGIO_DIRECTIVA)
                    || chorda_aequalis_literis(*elem->titulus,
                           SILVA_ARBOR_TAG_REGIO_CRUDA)
                    || chorda_aequalis_literis(*elem->titulus,
                           SILVA_ARBOR_TAG_REGIO_FINIS)))
            {
                SilvaRegio* regio;
                SilvaRamus* ramus;
                       Xar* lamina;
                       s32  initium_lacunae;
                       s32  index_regionis;
                       s32  index_patris;
                       s32  regio_fons;
                       i32  regio_linea;
                       i32  numerus_attributi;

                si (passus != ZEPHYRUM)
                {
                    perge;
                }

                index_patris  = -I;
                regio_fons    = ZEPHYRUM;
                regio_linea   = ZEPHYRUM;

                attributum = stml_attributum_capere(elem, "regio");
                si (   attributum == NIHIL
                    || !_numerus_ex_chorda(attributum,
                            &numerus_attributi))
                {
                    _recusare(&lector, "regio sine indice",
                        elem->linea);
                    redde NIHIL;
                }
                index_regionis = (s32)numerus_attributi;

                attributum = stml_attributum_capere(elem, "pater");
                si (   attributum != NIHIL
                    && _numerus_ex_chorda(attributum,
                    &numerus_attributi))
                {
                    index_patris = (s32)numerus_attributi;
                }
                attributum = stml_attributum_capere(elem, "regio-fons");
                si (   attributum != NIHIL
                    && _numerus_ex_chorda(attributum,
                    &numerus_attributi))
                {
                    regio_fons = (s32)numerus_attributi;
                }
                attributum = stml_attributum_capere(elem,
                    "regio-linea");
                si (   attributum != NIHIL
                    && _numerus_ex_chorda(attributum,
                    &numerus_attributi))
                {
                    regio_linea = numerus_attributi;
                }

                regio = _parsura_regionem_obtinere(&lector,
                    regiones_tabula, regiones_patres, index_regionis,
                    index_patris, regio_fons, regio_linea);
                si (regio == NIHIL)
                {
                    _recusare(&lector, "regio creari non potuit",
                        elem->linea);
                    redde NIHIL;
                }

                materia_arbor_fixura_ancoram_legere(&sedes, elem);
                initium_lacunae = sedes.cursor.offset;
                lamina = _parsura_laminam_legere(&lector, sessio,
                    &sedes, elem);
                si (lamina == NIHIL)
                {
                    redde NIHIL;
                }
                si (!_parsura_lacunam_notare(lacunae,
                         initium_lacunae, &sedes.cursor,
                         _laminae_fons(lamina)))
                {
                    _recusare(&lector, "lacuna notari non potuit",
                        elem->linea);
                    redde NIHIL;
                }

                si (chorda_aequalis_literis(*elem->titulus,
                        SILVA_ARBOR_TAG_REGIO_FINIS))
                {
                    regio->directiva_finis = lamina;
                    perge;
                }

                attributum = stml_attributum_capere(elem, "ramus");
                si (   attributum == NIHIL
                    || !_numerus_ex_chorda(attributum,
                            &numerus_attributi))
                {
                    _recusare(&lector, "ramus sine indice",
                        elem->linea);
                    redde NIHIL;
                }
                ramus = _parsura_ramum_obtinere(&lector, regio,
                    (s32)numerus_attributi);
                si (ramus == NIHIL)
                {
                    _recusare(&lector, "ramus creari non potuit",
                        elem->linea);
                    redde NIHIL;
                }
                ramus->genus = _parsura_ramus_genus_ex_nomine(
                    stml_attributum_capere(elem, "genus"));
                attributum = stml_attributum_capere(elem, "conditio");
                si (   attributum != NIHIL
                    && _numerus_ex_chorda(attributum,
                    &numerus_attributi))
                {
                    ramus->conditio_id = numerus_attributi;
                }

                si (chorda_aequalis_literis(*elem->titulus,
                        SILVA_ARBOR_TAG_REGIO_CRUDA))
                {
                    ramus->lexemata_cruda = lamina;
                }
                alioquin
                {
                    ramus->directiva = lamina;
                }
                /* est_sumptum STRUCTURALE est: ramus sine lamina cruda
                 * sumptus est. Documentum configurationem NON portat
                 * (spec §2) - forma eam iam dicit. */
                ramus->est_sumptum = ramus->lexemata_cruda == NIHIL;
                perge;
            }

            /* INVOCATIO VACUA: lamina invocationis quae ZERO
             * lexemata peperit. Ordine documenti stat et lacunam
             * facit, prorsus ut directiva consumpta - ergo passu
             * ZERO legenda (laminae ante arborem, ut lacunae
             * exsistant cum arbor derivatur). */
            si (   elem->titulus != NIHIL
                && chorda_aequalis_literis(*elem->titulus,
                       SILVA_ARBOR_TAG_INVOCATIO_VACUA))
            {
                                      Xar* lamina;
                                      s32  initium_lacunae;
                SilvaExtentumInvocationis* ext;

                si (passus != ZEPHYRUM)
                {
                    perge;
                }
                materia_arbor_fixura_ancoram_legere(&sedes, elem);
                initium_lacunae = sedes.cursor.offset;
                lamina = _parsura_laminam_legere(&lector, sessio,
                    &sedes, elem);
                si (lamina == NIHIL)
                {
                    redde NIHIL;
                }
                si (!_parsura_lacunam_notare(lacunae, initium_lacunae,
                         &sedes.cursor, _laminae_fons(lamina)))
                {
                    _recusare(&lector, "lacuna notari non potuit",
                        elem->linea);
                    redde NIHIL;
                }
                si (expansio->extenta == NIHIL)
                {
                    _recusare(&lector, "extenta absentia", elem->linea);
                    redde NIHIL;
                }
                ext = (SilvaExtentumInvocationis*)
                    xar_addere(expansio->extenta);
                si (ext == NIHIL)
                {
                    _recusare(&lector,
                        "invocatio vacua addi non potuit",
                        elem->linea);
                    redde NIHIL;
                }
                ext->vacua   = VERUM;
                ext->lamina  = lamina;
                /* Invocatio = lexema primum laminae (nomen macri).
                 * Emissor eam non consulit pro vacuis - lamina sola
                 * reinserenda est - sed identitas honesta manet. */
                ext->invocatio  = (xar_numerus(lamina) > ZEPHYRUM)
                    ? *(SilvaToken**)xar_obtinere(lamina, ZEPHYRUM)
                    : NIHIL;
                perge;
            }

            /* RETRACTIO: directivae ex ordine documenti in campum
             * suum. Documentum eas ubi scriptae sunt fert (superficies
             * editionis); parsura ONERATA eas seorsum fert, ut arbor
             * PURA maneat - id est quod subarborem motam octetos suos
             * ubicumque emittere sinit. */
            si (   elem->titulus != NIHIL
                && chorda_aequalis_literis(*elem->titulus,
                       SILVA_ARBOR_TAG_DIRECTIVA))
            {
                      Xar*  lamina;
                      Xar** sedes_laminae;
                      s32   initium_lacunae;

                si (passus != ZEPHYRUM)
                {
                    perge;
                }

                si (parsura->directivae == NIHIL)
                {
                    parsura->directivae = xar_creare(piscina,
                        magnitudo(Xar*));
                    si (parsura->directivae == NIHIL)
                    {
                        _recusare(&lector,
                            "xar directivarum creari non potuit",
                            elem->linea);
                        redde NIHIL;
                    }
                }
                /* LAMINA PER FUNCTIONEM COMMUNEM.
                 *
                 * Hic ansa PROPRIA stabat quae _parsura_laminam_legere
                 * verbatim repetebat - et cum illa fragmenta aperire
                 * disceret, haec NON didicit. Quarta superficies
                 * eiusdem conceptus, quartum vitium eiusdem generis.
                 * Sanatio ergo non est hanc quoque emendare sed
                 * DELERE: unus conceptus, una functio. */
                materia_arbor_fixura_ancoram_legere(&sedes, elem);
                initium_lacunae = sedes.cursor.offset;
                lamina = _parsura_laminam_legere(&lector, sessio,
                    &sedes, elem);
                si (lamina == NIHIL)
                {
                    redde NIHIL;
                }
                sedes_laminae = (Xar**)xar_addere(parsura->directivae);
                si (sedes_laminae == NIHIL)
                {
                    _recusare(&lector,
                        "lamina in directivas addi non potuit",
                        elem->linea);
                    redde NIHIL;
                }
                *sedes_laminae = lamina;
                si (!_parsura_lacunam_notare(lacunae,
                         initium_lacunae, &sedes.cursor,
                         _laminae_fons(lamina)))
                {
                    _recusare(&lector, "lacuna notari non potuit",
                        elem->linea);
                    redde NIHIL;
                }
                perge;
            }
            si (   elem->titulus != NIHIL
                && chorda_aequalis_literis(*elem->titulus,
                       SILVA_ARBOR_TAG_CAUDA))
            {
                StmlNodus* interius;
                      i32  intra;

                si (passus != I)
                {
                    perge;
                }

                intra = ZEPHYRUM;
                interius = _silvae_elementum_proximum(&lector, elem,
                    &intra);
                si (interius == NIHIL)
                {
                    _recusare(&lector, "<cauda> lexema non fert",
                        elem->linea);
                    redde NIHIL;
                }
                materia_arbor_fixura_ancoram_legere(&sedes, elem);
                parsura->lexema_finis =
                    materia_arbor_lexema_legere(sessio,
                    interius, NIHIL);
                si (parsura->lexema_finis == NIHIL)
                {
                    redde NIHIL;
                }
                perge;
            }
            si (passus != I)
            {
                perge;
            }
            nodus = materia_arbor_nodum_legere(sessio, elem);
            si (nodus == NIHIL)
            {
                redde NIHIL;
            }
            materia_arbor_fixura_ancoram_legere(&sedes, elem);
            materia_arbor_positiones_nodi(&sedes, nodus);
            /* PROSPECTUM NOVUM reddit (mensura + I) - lista semantica
             * VALORIS est, ergo reassignandum, non 'successus' */
            radix = silva_valor_lista_appendere(piscina, radix,
                silva_valor_nodus(nodus));
            si (radix.genus != SILVA_VALOR_LISTA)
            {
                _recusare(&lector, "nodus in radicem addi non potuit",
                    elem->linea);
                redde NIHIL;
            }
        }
    }

    /* Arborem regionum texere: radices in expansionem, ceterae in
     * filias patris. _regiones_colligere in filias RECURRIT, ergo
     * nexus rectus laminas omnes attingibiles reddit. */
    {
        i32 ri;

        per (ri = ZEPHYRUM; ri < xar_numerus(regiones_tabula); ri++)
        {
             SilvaRegio*  regio;
             SilvaRegio** sedes_regionis;
             SilvaRegio** locus;
                    s32*  sedes_patris;

            sedes_regionis = (SilvaRegio**)xar_obtinere(
                regiones_tabula, ri);
            sedes_patris = (s32*)xar_obtinere(regiones_patres, ri);
            si (sedes_regionis == NIHIL || *sedes_regionis == NIHIL)
            {
                perge;
            }
            regio = *sedes_regionis;
            locus = NIHIL;
            si (   sedes_patris  != NIHIL
                && *sedes_patris >= ZEPHYRUM
                && *sedes_patris < (s32)xar_numerus(regiones_tabula))
            {
                SilvaRegio** sedes_parentis;

                sedes_parentis = (SilvaRegio**)xar_obtinere(
                    regiones_tabula, (i32)*sedes_patris);
                si (   sedes_parentis  != NIHIL
                    && *sedes_parentis != NIHIL)
                {
                    regio->pater = *sedes_parentis;
                    locus = (SilvaRegio**)xar_addere(
                        (*sedes_parentis)->filiae);
                }
            }
            si (locus == NIHIL)
            {
                locus = (SilvaRegio**)xar_addere(expansio->regiones);
            }
            si (locus == NIHIL)
            {
                _recusare(&lector, "regio nectari non potuit",
                    involucrum->linea);
                redde NIHIL;
            }
            *locus = regio;
        }
    }

    /* Cauda ultima - cursor iam per documentum totum ambulavit */
    si (parsura->lexema_finis != NIHIL)
    {
        materia_arbor_positiones_lexematis(&sedes,
            parsura->lexema_finis);
    }

    /* Referentiae (nullae in C89) et LONGITUDINES: lector materiae
     * trivia sine unco creat; lector silvae vetus omne lexema cum
     * longitudine = valor. Arbor, cauda, laminae (directivae,
     * regiones), extenta (lecta et vacua - in expansione eadem). */
    si (!materia_arbor_lector_finire(sessio))
    {
        _recusare(&lector, "referentiae solvi non potuerunt",
            involucrum->linea);
        redde NIHIL;
    }
    silva_frons_longitudines_figere(frons, radix);
    si (parsura->lexema_finis != NIHIL)
    {
        silva_frons_longitudines_figere(frons,
            silva_valor_token(parsura->lexema_finis));
    }
    _parsura_laminas_longitudine(frons, parsura->directivae);
    {
        i32 ri;

        per (ri = ZEPHYRUM; ri < xar_numerus(regiones_tabula); ri++)
        {
            SilvaRegio** sr =
                (SilvaRegio**)xar_obtinere(regiones_tabula,
                ri);
                   i32 rj;

            si (sr == NIHIL || *sr == NIHIL)
            { perge;
            }
            per (rj = ZEPHYRUM; (*sr)->rami != NIHIL
                && rj < xar_numerus((*sr)->rami); rj++)
            {
                SilvaRamus* ramus = *(SilvaRamus**)xar_obtinere(
                    (*sr)->rami, rj);

                si (ramus != NIHIL)
                {
                    _parsura_laminam_longitudine(frons,
                        ramus->directiva);
                    _parsura_laminam_longitudine(frons,
                        ramus->lexemata_cruda);
                }
            }
            _parsura_laminam_longitudine(frons, (*sr)->directiva_finis);
        }
    }

    parsura->commissio = silva_committere(piscina, radix, tabularium,
        NIHIL, NIHIL, NIHIL);
    si (parsura->commissio == NIHIL)
    {
        _recusare(&lector, "parsura committi non potuit",
            involucrum->linea);
        redde NIHIL;
    }
    parsura->successus = VERUM;
    redde parsura;
}
