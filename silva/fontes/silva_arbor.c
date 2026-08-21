/* silva_arbor.c - vocabularium dialecti 'arbor' (T2)
 *
 * Vide silva_arbor.h pro contractu et ratione praefixi 'lex-'.
 */

#include "silva_arbor.h"
#include "silva_commissio.h"
#include "chorda_aedificator.h"
#include "friatio.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "stml.h"
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


/* ==================================================
 * Scriptor: arbor -> STML canonicum (T3)
 *
 * Ambulatio grammaticae-IGNARA (exemplar silva_scribere): forma
 * ex registro cocto sumitur, nulla scientia c89 hic. Ambulatio
 * tamen silva_scribere PROPRIA est - illius recursio 'interior'
 * est sine iunctura visitatoris, et dimidium lexematum suum ad
 * reconstructionem octetorum coniunctum (revolutio originum,
 * congruentia extentorum macro, glutinatio directivarum) cuius
 * arbor nullum usum habet. Forma ambulationis communis,
 * implementa duo.
 * ================================================== */

/* Nota per lexema: quotiens visum (passus I), quis numerus
 * fragmenti ei datus sit, an definitio iam emissa (passus II) */
nomen structura {
    i32 usus;
    i32 numerus;
    b32 emissum;
} ArborLexematisNota;

nomen structura {
                           Piscina* piscina;
               InternamentumChorda* intern;
     constans SilvaRegistrumCoctum* tabularium;
            constans SilvaExpansio* expansio;
                    TabulaDispersa* lexemata;
                               i32  numerus_notarum;

    /* Ancora: primum lexema ordine AMBULATIONIS (non ordine
     * octetorum) - lector eundem ordinem replicat */
                               b32 ancora_nota;
                               s32 ancora_offset;
                               i32 ancora_linea;
                               i32 ancora_columna;
                               s32 ancora_fons;
                               b32 ancora_initium_lineae;

    /* Fractura */
               constans character* causa;
              constans SilvaNodus* sedes;
} ArborScriptor;


/* Clavis tabulae = OCTETI monstratoris lexematis. Tabula clavem
 * non copiat (chordam ut datam servat), ergo cella e piscina
 * venit, non e pila. */
interior chorda
_clavis_lexematis (
                Piscina* piscina,
    constans SilvaToken* lexema)
{
    vacuum* cella;
    chorda  clavis;

    cella = piscina_allocare(piscina, magnitudo(constans SilvaToken*));
    si (cella == NIHIL)
    {
        clavis.mensura  = ZEPHYRUM;
        clavis.datum    = NIHIL;
        redde clavis;
    }
    *(constans SilvaToken**)cella = lexema;

    clavis.mensura  = (i32)magnitudo(constans SilvaToken*);
    clavis.datum    = (i8*)cella;
    redde clavis;
}

interior ArborLexematisNota*
_nota_lexematis (
          ArborScriptor* scriptor,
    constans SilvaToken* lexema)
{
    vacuum* inventum;
    chorda  clavis;

    clavis = _clavis_lexematis(scriptor->piscina, lexema);
    si (clavis.datum == NIHIL)
    {
        redde NIHIL;
    }
    si (tabula_dispersa_invenire(scriptor->lexemata, clavis, &inventum))
    {
        redde (ArborLexematisNota*)inventum;
    }
    redde NIHIL;
}


/* ==================================================
 * Passus I - usus lexematum numerare + ancoram capere
 * ================================================== */

interior constans SilvaToken*
_parsura_lexema_emissionis (
    constans SilvaToken*);

interior vacuum
_numerare_valorem (
    ArborScriptor*,
    SilvaValor);

interior vacuum
_numerare_lexema (
          ArborScriptor* scriptor,
    constans SilvaToken* lexema)
{
    ArborLexematisNota* nota;

    si (lexema == NIHIL)
    {
        redde;
    }

    nota = _nota_lexematis(scriptor, lexema);
    si (nota != NIHIL)
    {
        nota->usus++;
        redde;
    }

    nota = (ArborLexematisNota*)piscina_allocare(scriptor->piscina,
        magnitudo(ArborLexematisNota));
    si (nota == NIHIL)
    {
        redde;
    }
    nota->usus     = I;
    nota->numerus  = ZEPHYRUM;
    nota->emissum  = FALSUM;
    tabula_dispersa_inserere(scriptor->lexemata,
        _clavis_lexematis(scriptor->piscina, lexema), nota);

    /* Ancora = primum lexema ordine ambulationis */
    /* ANCORA = sedes ubi EMISSIO incipit, non sedes lexematis.
     * Emissio TRIVIIS ducentibus incipit, ergo si lexema primum
     * spatia_ante fert, ancora ex TRIVIO PRIMO sumenda est.
     * Aliter lector cursorem ad lexema ponit, deinde trivia ante
     * id emittit, et lexema ipsum post trivia cadit - omnes sedes
     * longitudine indentationis labuntur.
     * MENSURATUM (T6): lexema solum adhibens CLXXVIII divergentias
     * 'lexema/offset' super corpus dedit. Probatio parva id NON
     * cepit quia 'int n = 0;' lexema primum ad offset 0 sine ullo
     * trivio ducente habet - casus in quo vitium evanescit. */
    si (!scriptor->ancora_nota)
    {
        constans SilvaToken* initium;

        /* Catenam originis sequi: emissio ab invocatione strati 0
         * incipit, non a lexemate expanso (cuius sedes DEF-SITE
         * est, in plagula alia). Vide _parsura_lexema_emissionis. */
        lexema   = _parsura_lexema_emissionis(lexema);
        initium  = lexema;

        si (   lexema->spatia_ante != NIHIL
            && xar_numerus(lexema->spatia_ante) > ZEPHYRUM)
        {
            constans SilvaToken* trivium = *(SilvaToken**)
                xar_obtinere(lexema->spatia_ante, ZEPHYRUM);

            si (trivium != NIHIL && trivium->byte_offset >= ZEPHYRUM)
            {
                initium = trivium;
            }
        }

        scriptor->ancora_nota     = VERUM;
        scriptor->ancora_offset   = initium->byte_offset;
        scriptor->ancora_linea    = initium->linea;
        scriptor->ancora_columna  = initium->columna;
        scriptor->ancora_fons     = lexema->fons_index;
        /* NON DERIVABILE ex subarbore: an lexema primum lineam
         * incipiat pendet ab eo quod ANTE subarborem in plagula
         * stat. Contextus est, sicut ipsa ancora - ergo portandum.
         * (T6: X divergentiae 'lexema/initium-lineae' hinc.) */
        scriptor->ancora_initium_lineae = lexema->initium_lineae;
    }

    /* LEXEMATA ORIGINIS quoque numeranda. Invocatio UNA plura
     * lexemata expansa gignere potest (macro functio-simile:
     * credo.h CR(x)), et si quodque suam copiam scriberet emissor
     * invocationem PLURIES redderet - identitas duplicata
     * mentiretur (lex duplex, spec v1 §6). Hic numerata, machina
     * fragmentorum eandem rem agit quam bracchiis ambiguis agit.
     *
     * Post creationem notae SOLUM: lexema bis visum originem
     * eandem habet, ergo bis numerare usus inflaret. */
    commutatio (lexema->origo.genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        _numerare_lexema(scriptor,
            lexema->origo.datum.expansio.invocatio);
        frange;
    casus SILVA_ORIGO_PASTA:
        _numerare_lexema(scriptor, lexema->origo.datum.pasta.sinister);
        _numerare_lexema(scriptor, lexema->origo.datum.pasta.dexter);
        frange;
    casus SILVA_ORIGO_CHORDA:
        _numerare_lexema(scriptor,
            lexema->origo.datum.stringificatio.primus);
        frange;
    ordinarius:
        frange;
    }
}

interior vacuum
_numerare_nodum (
           ArborScriptor* scriptor,
     constans SilvaNodus* nodus)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        _numerare_valorem(scriptor, nodus->loci[i]);
    }
}

interior vacuum
_numerare_valorem (
    ArborScriptor* scriptor,
       SilvaValor  valor)
{
    i32 i;
    i32 numerus;

    commutatio (valor.genus)
    {
        casus SILVA_VALOR_NODUS:
            _numerare_nodum(scriptor, valor.datum.nodus);
            frange;
        casus SILVA_VALOR_TOKEN:
            _numerare_lexema(scriptor, valor.datum.token);
            frange;
        casus SILVA_VALOR_LISTA:
            numerus = silva_valor_lista_numerus(valor);
            per (i = ZEPHYRUM; i < numerus; i++)
            {
                SilvaValor* elementum = silva_valor_lista_obtinere(valor, i);

                si (elementum != NIHIL)
                {
                    _numerare_valorem(scriptor, *elementum);
                }
            }
            frange;
        ordinarius:
            frange;
    }
}


/* ==================================================
 * Passus II - emissio
 * ================================================== */

/* Decimale sine stdio (snprintf C99 est; postulata_posix hic
 * pretium non meretur pro numeris parvis) */
interior i32
_numerus_ad_literas (
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

    si (_numerus_ad_literas(numerus, buffer, (i32)magnitudo(buffer))
        == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde stml_attributum_addere(elementum, scriptor->piscina,
        scriptor->intern, titulus, buffer);
}

/* An chorda TEXTUI tuta sit. Duo pericula, ambo TACITA:
 *  - spatium album SOLUM: scriptor pulcher nodos textus spatii
 *    albi solius PRAETERIT (fidelitas vs legibilitas, lib/stml.c) -
 *    ergo tales octeti perirent sine querela;
 *  - NUL: chorda longitudinem fert, sed textus emissus
 *    terminatore legetur.
 * Genera triviorum spatii albi lentes proprias habent (infra), ergo
 * hic non veniunt; quod huc venit et non tutum est REFUTATUR. */
interior b32
_textus_tutus (
    constans chorda* valor)
{
    i32 i;
    b32 album_solum;

    si (valor == NIHIL || valor->mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    album_solum = VERUM;
    per (i = ZEPHYRUM; i < valor->mensura; i++)
    {
        character c = (character)valor->datum[i];

        si (c == '\0')
        {
            redde FALSUM;
        }
        si (   c != ' ' && c != '\t' && c != '\n'
            && c != '\r' && c != '\f' && c != '\v')
        {
            album_solum = FALSUM;
        }
    }
    redde album_solum ? FALSUM : VERUM;
}

/* Trivium ut elementum. Lentes compactae pro triviis spatii albi
 * NON ornamentum sunt sed NECESSITAS: valor eorum spatium album
 * SOLUM est, ergo ut textus emissus a scriptore pulchro tacite
 * praeteriretur. Lens numerum fert; nihil perit (valor numero
 * plene determinatur). */
interior StmlNodus*
_trivium_scribere (
          ArborScriptor* scriptor,
    constans SilvaToken* trivium)
{
    character  tag[SILVA_ARBOR_TAG_CAPACITAS];
    StmlNodus* elementum;

    si (trivium == NIHIL)
    {
        scriptor->causa = "trivium nihil";
        redde NIHIL;
    }
    si (silva_arbor_lexema_tag(trivium->genus, tag,
            (i32)magnitudo(tag)) == ZEPHYRUM)
    {
        scriptor->causa = "genus trivii ignotum";
        redde NIHIL;
    }

    elementum = stml_elementum_creare(scriptor->piscina,
        scriptor->intern, tag);
    si (elementum == NIHIL)
    {
        scriptor->causa = "elementum trivii creari non potuit";
        redde NIHIL;
    }

    commutatio (trivium->genus)
    {
        casus SILVA_LEX_SPATIA:
        casus SILVA_LEX_TABULAE:
            si (!_attributum_numeri(scriptor, elementum, "n",
                     trivium->valor.mensura))
            {
                scriptor->causa = "numerus trivii scribi non potuit";
                redde NIHIL;
            }
            frange;

        casus SILVA_LEX_NOVA_LINEA:
        casus SILVA_LEX_CONTINUATIO:
            /* '\r\n' contra '\n': octetus '\r' adest an non */
            si (   trivium->valor.mensura >= II
                && trivium->valor.datum[trivium->valor.mensura - II]
                       == (i8)'\r')
            {
                stml_attributum_boolean_addere(elementum,
                    scriptor->piscina, scriptor->intern, "crlf");
            }
            frange;

        ordinarius:
            /* commenta et cetera: valor ut textus */
            si (!_textus_tutus(&trivium->valor))
            {
                scriptor->causa = "valor trivii textui non tutus";
                redde NIHIL;
            }
            si (trivium->valor.mensura > ZEPHYRUM)
            {
                StmlNodus* textus = stml_textum_creare_ex_chorda(
                    scriptor->piscina, scriptor->intern,
                    trivium->valor);

                si (   textus == NIHIL
                    || !stml_liberum_addere(elementum, textus))
                {
                    scriptor->causa = "textus trivii addi non potuit";
                    redde NIHIL;
                }
            }
            frange;
    }
    redde elementum;
}

/* Involucrum <ante>/<post>; NIHIL si series vacua (nihil emittendum)
 * aut in fractura - vocans causam inspicit ut discernat. */
interior StmlNodus*
_involucrum_triviorum (
     ArborScriptor* scriptor,
               Xar* trivia,
constans character* titulus)
{
    StmlNodus* involucrum;
          i32  numerus;
          i32  i;

    si (trivia == NIHIL)
    {
        redde NIHIL;
    }
    numerus = xar_numerus(trivia);
    si (numerus == ZEPHYRUM)
    {
        redde NIHIL;
    }

    involucrum = stml_elementum_creare(scriptor->piscina,
        scriptor->intern, titulus);
    si (involucrum == NIHIL)
    {
        scriptor->causa = "involucrum triviorum creari non potuit";
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        SilvaToken* trivium    = *(SilvaToken**)xar_obtinere(trivia, i);
         StmlNodus* elementum  = _trivium_scribere(scriptor, trivium);

        si (elementum == NIHIL)
        {
            redde NIHIL;
        }
        si (!stml_liberum_addere(involucrum, elementum))
        {
            scriptor->causa = "trivium addi non potuit";
            redde NIHIL;
        }
    }
    redde involucrum;
}

interior StmlNodus*
_scribere_lexema (
          ArborScriptor* scriptor,
    constans SilvaToken* lexema);

/* Originem lexematis in elementum eius NESTARE.
 *
 * FONS nihil fert - ~XC% lexematum nihil solvit.
 *
 * Liberum NESTATUM lexema PLENUM est, non nomen: invocatio trivia
 * FERT (silva_nodus.h), ergo spatia ducentia in ea vivunt.
 *
 * DEF-SITE per REFERENTIAM (def-f/def-l), numquam inlinatum: aliter
 * quaeque plagula latina.h utens lexemata latina.h COPIARET.
 * Honeste: referentia pendere potest si plagula definiens huic
 * documento non adest - limes nominatus, non celatus.
 *
 * CAECATIO EXCLUSA: hidesets recursionem expansionis prohibent et
 * refectio eos NUMQUAM consulit, ergo clausurae emissionis non
 * pertinent (spec §3). Reservata ut 'cauda="#c7"'. */
interior b32
_origo_scribere (
           ArborScriptor* scriptor,
               StmlNodus* elementum,
     constans SilvaOrigo* origo)
{
     constans character* tag;
        constans chorda* titulus_macro;
    constans SilvaToken* primus;
    constans SilvaToken* secundus;
    constans SilvaToken* definitio;
              StmlNodus* elem;
              StmlNodus* scriptum;

    primus     = NIHIL;
    secundus   = NIHIL;
    definitio  = NIHIL;

    commutatio (origo->genus)
    {
    casus SILVA_ORIGO_FONS:
        redde VERUM;
    casus SILVA_ORIGO_EXPANSIO:
        tag            = SILVA_ARBOR_TAG_EXPANSIO;
        titulus_macro  = origo->datum.expansio.nomen_macro;
        primus         = origo->datum.expansio.invocatio;
        definitio      = origo->datum.expansio.corpus;
        frange;
    casus SILVA_ORIGO_PASTA:
        tag            = SILVA_ARBOR_TAG_PASTA;
        titulus_macro  = origo->datum.pasta.nomen_macro;
        primus         = origo->datum.pasta.sinister;
        secundus       = origo->datum.pasta.dexter;
        frange;
    casus SILVA_ORIGO_CHORDA:
        tag            = SILVA_ARBOR_TAG_STRINGIFICATIO;
        titulus_macro  = origo->datum.stringificatio.nomen_macro;
        primus         = origo->datum.stringificatio.primus;
        frange;
    casus SILVA_ORIGO_API:
        tag            = SILVA_ARBOR_TAG_API;
        titulus_macro  = origo->datum.api.nomen_macro;
        frange;
    ordinarius:
        scriptor->causa = "genus originis ignotum";
        redde FALSUM;
    }

    elem = stml_elementum_creare(scriptor->piscina, scriptor->intern,
        tag);
    si (elem == NIHIL)
    {
        scriptor->causa = "elementum originis creari non potuit";
        redde FALSUM;
    }
    si (titulus_macro != NIHIL && titulus_macro->mensura > ZEPHYRUM)
    {
        stml_attributum_addere_chorda(elem, scriptor->piscina,
            scriptor->intern, "macro", *titulus_macro);
    }
    si (definitio != NIHIL)
    {
        _attributum_numeri(scriptor, elem, "def-f",
            (i32)definitio->fons_index);
        _attributum_numeri(scriptor, elem, "def-l", definitio->linea);
        _attributum_numeri(scriptor, elem, "def-c", definitio->columna);
    }
    si (primus != NIHIL)
    {
        scriptum = _scribere_lexema(scriptor, primus);
        si (scriptum == NIHIL)
        {
            redde FALSUM;
        }
        si (!stml_liberum_addere(elem, scriptum))
        {
            scriptor->causa = "invocatio in originem addi non potuit";
            redde FALSUM;
        }
    }
    si (secundus != NIHIL)
    {
        scriptum = _scribere_lexema(scriptor, secundus);
        si (scriptum == NIHIL)
        {
            redde FALSUM;
        }
        si (!stml_liberum_addere(elem, scriptum))
        {
            scriptor->causa = "dexter in originem addi non potuit";
            redde FALSUM;
        }
    }
    si (!stml_liberum_addere(elementum, elem))
    {
        scriptor->causa = "origo in lexema addi non potuit";
        redde FALSUM;
    }
    redde VERUM;
}

interior StmlNodus*
_scribere_lexema (
          ArborScriptor* scriptor,
    constans SilvaToken* lexema)
{
             character  tag[SILVA_ARBOR_TAG_CAPACITAS];
             character  nomen_fragmenti[XXXII];
    ArborLexematisNota* nota;
             StmlNodus* elementum;
             StmlNodus* involucrum;
                   i32  i;
                   i32  numerus_scissurarum;

    si (lexema == NIHIL)
    {
        scriptor->causa = "lexema nihil";
        redde NIHIL;
    }

    /* LIMES EXPANSIONIS TOLLITUR (T6): lexema non-FONS originem
     * NESTATAM fert, non fracturam. Vide _origo_scribere. */

    nota = _nota_lexematis(scriptor, lexema);

    /* Usus secundus et sequentes: transclusio. Identitas res est -
     * duplicatio mentiretur (bracchia ambigua lexemata EADEM ferunt) */
    si (nota != NIHIL && nota->usus > I && nota->emissum)
    {
        StmlNodus* transclusio;

        si (_numerus_ad_literas(nota->numerus, nomen_fragmenti + IV,
                (i32)magnitudo(nomen_fragmenti) - IV) == ZEPHYRUM)
        {
            scriptor->causa = "nomen fragmenti scribi non potuit";
            redde NIHIL;
        }
        nomen_fragmenti[0] = '#';
        nomen_fragmenti[1] = 'l';
        nomen_fragmenti[2] = 'e';
        nomen_fragmenti[3] = 'x';

        transclusio = stml_elementum_creare(scriptor->piscina,
            scriptor->intern, "transclusio");
        si (transclusio == NIHIL)
        {
            scriptor->causa = "transclusio creari non potuit";
            redde NIHIL;
        }
        transclusio->genus = STML_NODUS_TRANSCLUSIO;
        transclusio->valor = chorda_internare_ex_literis(
            scriptor->intern, nomen_fragmenti);
        si (transclusio->valor == NIHIL)
        {
            scriptor->causa = "valor transclusionis internari non potuit";
            redde NIHIL;
        }
        redde transclusio;
    }

    si (silva_arbor_lexema_tag(lexema->genus, tag, (i32)magnitudo(tag))
        == ZEPHYRUM)
    {
        scriptor->causa = "genus lexematis ignotum";
        redde NIHIL;
    }

    elementum = stml_elementum_creare(scriptor->piscina,
        scriptor->intern, tag);
    si (elementum == NIHIL)
    {
        scriptor->causa = "elementum lexematis creari non potuit";
        redde NIHIL;
    }

    /* 'standard' et 'f' SOLUM cum non-ordinaria */
    si (lexema->standard != (i8)SILVA_STANDARD_C89)
    {
        si (!_attributum_numeri(scriptor, elementum, "standard",
                 (i32)lexema->standard))
        {
            scriptor->causa = "standard scribi non potuit";
            redde NIHIL;
        }
    }
    si (   lexema->fons_index >= ZEPHYRUM
        && lexema->fons_index != scriptor->ancora_fons)
    {
        si (!_attributum_numeri(scriptor, elementum, "f",
                 (i32)lexema->fons_index))
        {
            scriptor->causa = "fons scribi non potuit";
            redde NIHIL;
        }
    }

    /* ante */
    involucrum = _involucrum_triviorum(scriptor, lexema->spatia_ante,
        SILVA_ARBOR_TAG_ANTE);
    si (involucrum == NIHIL && scriptor->causa != NIHIL)
    {
        redde NIHIL;
    }
    si (   involucrum != NIHIL
        && !stml_liberum_addere(elementum, involucrum))
    {
        scriptor->causa = "involucrum ante addi non potuit";
        redde NIHIL;
    }

    /* valor - SOLUM generibus orthographiae variae. Orthographia
     * fixa in genere IPSO vivit; eam scribere fontem veritatis
     * tertium faceret */
    si (silva_arbor_valor_portandus(lexema->genus))
    {
        si (!_textus_tutus(&lexema->valor))
        {
            scriptor->causa = "valor lexematis textui non tutus";
            redde NIHIL;
        }
        si (lexema->valor.mensura > ZEPHYRUM)
        {
            StmlNodus* textus = stml_textum_creare_ex_chorda(
                scriptor->piscina, scriptor->intern, lexema->valor);

            si (   textus == NIHIL
                || !stml_liberum_addere(elementum, textus))
            {
                scriptor->causa = "textus lexematis addi non potuit";
                redde NIHIL;
            }
        }
    }

    /* post */
    involucrum = _involucrum_triviorum(scriptor, lexema->spatia_post,
        SILVA_ARBOR_TAG_POST);
    si (involucrum == NIHIL && scriptor->causa != NIHIL)
    {
        redde NIHIL;
    }
    si (   involucrum != NIHIL
        && !stml_liberum_addere(elementum, involucrum))
    {
        scriptor->causa = "involucrum post addi non potuit";
        redde NIHIL;
    }

    /* scissurae (laminae \<nl> INTRA lexema) */
    numerus_scissurarum = lexema->scissurae
        ? xar_numerus(lexema->scissurae) : (i32)ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus_scissurarum; i++)
    {
        SilvaScissura* scissura =
            (SilvaScissura*)xar_obtinere(lexema->scissurae, i);
        StmlNodus* elementum_scissurae;

        si (scissura == NIHIL || scissura->offset < ZEPHYRUM)
        {
            scriptor->causa = "scissura corrupta";
            redde NIHIL;
        }
        elementum_scissurae = stml_elementum_creare(scriptor->piscina,
            scriptor->intern, SILVA_ARBOR_TAG_SCISSURA);
        si (   elementum_scissurae == NIHIL
            || !_attributum_numeri(scriptor, elementum_scissurae,
                    "offset", (i32)scissura->offset))
        {
            scriptor->causa = "scissura scribi non potuit";
            redde NIHIL;
        }
        si (scissura->crlf)
        {
            stml_attributum_boolean_addere(elementum_scissurae,
                scriptor->piscina, scriptor->intern, "crlf");
        }
        si (!stml_liberum_addere(elementum, elementum_scissurae))
        {
            scriptor->causa = "scissura addi non potuit";
            redde NIHIL;
        }
    }

    /* SEDES PORTATA pro lexemate NON-FONS, et SOLUM pro eo.
     *
     * Lexema expansum sedem DEF-SITE fert - id est, sedem in
     * plagula ALIA (latina.h, exempli gratia). Ex hoc fluxu
     * octetorum derivari NEQUIT, quia in hoc fluxu OMNINO NON EST:
     * octetos hic invocatio tenet, non expansio. Lex 'sedes
     * derivatae' integra manet ubi derivatio POSSIBILIS est; ubi
     * non est, portare honestius quam fingere. */
    si (lexema->origo.genus != SILVA_ORIGO_FONS)
    {
        si (lexema->byte_offset >= ZEPHYRUM)
        {
            _attributum_numeri(scriptor, elementum, "b",
                (i32)lexema->byte_offset);
        }
        _attributum_numeri(scriptor, elementum, "linea",
            lexema->linea);
        _attributum_numeri(scriptor, elementum, "columna",
            lexema->columna);
        si (lexema->initium_lineae)
        {
            stml_attributum_boolean_addere(elementum,
                scriptor->piscina, scriptor->intern,
                "linea-initium");
        }
    }

    /* ORIGO nestata - post trivia, ante involucrum fragmenti (ut
     * fragmentum lexema TOTUM cum origine sua ferat) */
    si (!_origo_scribere(scriptor, elementum, &lexema->origo))
    {
        redde NIHIL;
    }

    /* Usus primus lexematis COMMUNICATI: fragmentum nominatum.
     * Passus I numeravit, ergo hic iam scimus - quod est ipsa
     * ratio cur passus duo sunt */
    si (nota != NIHIL && nota->usus > I)
    {
        StmlNodus* fragmentum;

        scriptor->numerus_notarum++;
        nota->numerus = scriptor->numerus_notarum;
        nota->emissum = VERUM;

        si (_numerus_ad_literas(nota->numerus, nomen_fragmenti + III,
                (i32)magnitudo(nomen_fragmenti) - III) == ZEPHYRUM)
        {
            scriptor->causa = "nomen fragmenti scribi non potuit";
            redde NIHIL;
        }
        nomen_fragmenti[0] = 'l';
        nomen_fragmenti[1] = 'e';
        nomen_fragmenti[2] = 'x';

        fragmentum = stml_elementum_creare(scriptor->piscina,
            scriptor->intern, "fragmentum");
        si (fragmentum == NIHIL)
        {
            scriptor->causa = "fragmentum creari non potuit";
            redde NIHIL;
        }
        fragmentum->fragmentum    = VERUM;
        fragmentum->fragmentum_id = chorda_internare_ex_literis(
            scriptor->intern, nomen_fragmenti);
        si (   fragmentum->fragmentum_id == NIHIL
            || !stml_liberum_addere(fragmentum, elementum))
        {
            scriptor->causa = "lexema in fragmentum addi non potuit";
            redde NIHIL;
        }
        redde fragmentum;
    }

    redde elementum;
}


interior StmlNodus*
_scribere_nodum_internum (
    ArborScriptor*,
    constans SilvaNodus*);

/* Valorem in involucrum loci scribere, specie loci DUCTUS.
 * Species custodia est, non ornamentum: silva_nodus_appendere
 * speciem listae solam probat, NUMQUAM quid intus eat - ergo
 * elementa listae mixtae HIC custodienda sunt. */
interior b32
_scribere_valorem_in (
            ArborScriptor* scriptor,
                StmlNodus* parens,
               SilvaValor  valor,
        SilvaLocusSpecies  species,
      constans SilvaNodus* sedes)
{
    StmlNodus* liberum;
    character  buffer[XVI];
          i32  numerus;
          i32  i;

    commutatio (species)
    {
        casus SILVA_LOCUS_NODUS:
            si (valor.genus != SILVA_VALOR_NODUS)
            {
                scriptor->causa = "locus NODUS valorem alienum fert";
                scriptor->sedes = sedes;
                redde FALSUM;
            }
            liberum = _scribere_nodum_internum(scriptor,
                valor.datum.nodus);
            si (liberum == NIHIL)
            {
                redde FALSUM;
            }
            redde stml_liberum_addere(parens, liberum);

        casus SILVA_LOCUS_TOKEN:
            si (valor.genus != SILVA_VALOR_TOKEN)
            {
                scriptor->causa = "locus TOKEN valorem alienum fert";
                scriptor->sedes = sedes;
                redde FALSUM;
            }
            liberum = _scribere_lexema(scriptor, valor.datum.token);
            si (liberum == NIHIL)
            {
                scriptor->sedes = sedes;
                redde FALSUM;
            }
            redde stml_liberum_addere(parens, liberum);

        casus SILVA_LOCUS_INDEX:
            si (valor.genus != SILVA_VALOR_INDEX)
            {
                scriptor->causa = "locus INDEX valorem alienum fert";
                scriptor->sedes = sedes;
                redde FALSUM;
            }
            si (valor.datum.index < ZEPHYRUM)
            {
                scriptor->causa = "index negativus";
                scriptor->sedes = sedes;
                redde FALSUM;
            }
            si (_numerus_ad_literas((i32)valor.datum.index, buffer,
                    (i32)magnitudo(buffer)) == ZEPHYRUM)
            {
                scriptor->causa = "index scribi non potuit";
                scriptor->sedes = sedes;
                redde FALSUM;
            }
            redde stml_textum_addere(parens, scriptor->piscina,
                scriptor->intern, buffer);

        casus SILVA_LOCUS_LISTA_NODUS:
        casus SILVA_LOCUS_LISTA_TOKEN:
        casus SILVA_LOCUS_LISTA_MIXTA:
            si (valor.genus != SILVA_VALOR_LISTA)
            {
                scriptor->causa = "locus LISTA valorem alienum fert";
                scriptor->sedes = sedes;
                redde FALSUM;
            }
            /* Mensura PROSPECTUS, numquam xar_numerus repositorii */
            numerus = silva_valor_lista_numerus(valor);
            per (i = ZEPHYRUM; i < numerus; i++)
            {
                SilvaValor* elementum =
                    silva_valor_lista_obtinere(valor, i);

                si (elementum == NIHIL)
                {
                    scriptor->causa = "elementum listae nihil";
                    scriptor->sedes = sedes;
                    redde FALSUM;
                }
                si (elementum->genus == SILVA_VALOR_NODUS)
                {
                    si (species == SILVA_LOCUS_LISTA_TOKEN)
                    {
                        scriptor->causa =
                            "nodus in lista TOKEN";
                        scriptor->sedes = sedes;
                        redde FALSUM;
                    }
                    liberum = _scribere_nodum_internum(scriptor,
                        elementum->datum.nodus);
                }
                alioquin si (elementum->genus == SILVA_VALOR_TOKEN)
                {
                    si (species == SILVA_LOCUS_LISTA_NODUS)
                    {
                        scriptor->causa =
                            "lexema in lista NODUS";
                        scriptor->sedes = sedes;
                        redde FALSUM;
                    }
                    liberum = _scribere_lexema(scriptor,
                        elementum->datum.token);
                }
                alioquin
                {
                    scriptor->causa = "elementum listae generis alieni";
                    scriptor->sedes = sedes;
                    redde FALSUM;
                }

                si (liberum == NIHIL)
                {
                    si (scriptor->sedes == NIHIL)
                    {
                        scriptor->sedes = sedes;
                    }
                    redde FALSUM;
                }
                si (!stml_liberum_addere(parens, liberum))
                {
                    scriptor->causa = "elementum listae addi non potuit";
                    scriptor->sedes = sedes;
                    redde FALSUM;
                }
            }
            redde VERUM;

        ordinarius:
            scriptor->causa = "species loci ignota";
            scriptor->sedes = sedes;
            redde FALSUM;
    }
}

interior StmlNodus*
_scribere_nodum_internum (
           ArborScriptor* scriptor,
     constans SilvaNodus* nodus)
{
    constans SilvaTabGenus* genus;
                 StmlNodus* elementum;
                       i32  i;

    si (nodus == NIHIL)
    {
        scriptor->causa = "nodus nihil";
        redde NIHIL;
    }
    si (   nodus->genus < ZEPHYRUM
        || (i32)nodus->genus >= scriptor->tabularium->numerus_generum)
    {
        scriptor->causa = "genus registro ignotum";
        scriptor->sedes = nodus;
        redde NIHIL;
    }

    genus = &scriptor->tabularium->genera[nodus->genus];

    /* Forma nodi cum forma registri congruere DEBET - aliter loci
     * et species inter se labuntur et documentum tacite mentitur */
    si (nodus->numerus_locorum != genus->loci_numerus)
    {
        scriptor->causa = "forma nodi registro non congruit";
        scriptor->sedes = nodus;
        redde NIHIL;
    }

    elementum = stml_elementum_creare(scriptor->piscina,
        scriptor->intern, genus->titulus);
    si (elementum == NIHIL)
    {
        scriptor->causa = "elementum generis creari non potuit";
        scriptor->sedes = nodus;
        redde NIHIL;
    }

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaTabLocus* locus;
                     StmlNodus* involucrum;
                           i32  absolutus;

        /* Locus NIHIL = elementum OMISSUM (absentia canonica) */
        si (nodus->loci[i].genus == SILVA_VALOR_NIHIL)
        {
            perge;
        }

        absolutus = genus->loci_offset + i;
        si (absolutus >= scriptor->tabularium->numerus_locorum)
        {
            scriptor->causa = "locus extra tabulam";
            scriptor->sedes = nodus;
            redde NIHIL;
        }
        locus = &scriptor->tabularium->loci[absolutus];

        involucrum = stml_elementum_creare(scriptor->piscina,
            scriptor->intern, locus->titulus);
        si (involucrum == NIHIL)
        {
            scriptor->causa = "involucrum loci creari non potuit";
            scriptor->sedes = nodus;
            redde NIHIL;
        }

        si (!_scribere_valorem_in(scriptor, involucrum, nodus->loci[i],
                 (SilvaLocusSpecies)locus->species, nodus))
        {
            redde NIHIL;
        }
        si (!stml_liberum_addere(elementum, involucrum))
        {
            scriptor->causa = "involucrum loci addi non potuit";
            scriptor->sedes = nodus;
            redde NIHIL;
        }
    }
    redde elementum;
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
    SilvaArborScriptura  fructus;
          ArborScriptor  scriptor;
              StmlNodus* involucrum;
              StmlNodus* radix;
                 chorda  sigillum;

    fructus.successus       = FALSUM;
    fructus.textus.mensura  = ZEPHYRUM;
    fructus.textus.datum    = NIHIL;
    fructus.causa           = NIHIL;
    fructus.sedes           = NIHIL;

    si (piscina == NIHIL || nodus == NIHIL || tabularium == NIHIL)
    {
        fructus.causa = "argumenta nihil";
        redde fructus;
    }
    si (grammatica == NIHIL)
    {
        fructus.causa = "grammatica innominata";
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

    scriptor.piscina          = piscina;
    scriptor.intern           = intern;
    scriptor.tabularium       = tabularium;
    scriptor.expansio         = expansio;
    scriptor.numerus_notarum  = ZEPHYRUM;
    scriptor.ancora_nota      = FALSUM;
    scriptor.ancora_offset    = -I;
    scriptor.ancora_linea     = ZEPHYRUM;
    scriptor.ancora_columna   = ZEPHYRUM;
    scriptor.ancora_fons      = ZEPHYRUM;
    scriptor.causa            = NIHIL;
    scriptor.sedes            = NIHIL;
    scriptor.lexemata        = tabula_dispersa_creare_chorda(piscina,
        256);
    si (scriptor.lexemata == NIHIL)
    {
        fructus.causa = "tabula lexematum creari non potuit";
        redde fructus;
    }

    /* PASSUS I: usus numerare + ancoram capere. Sine hoc usus
     * primus scire non posset an fragmentum opus esset */
    _numerare_nodum(&scriptor, nodus);

    involucrum = stml_elementum_creare(piscina, intern,
        SILVA_ARBOR_TAG_ENVOLUCRI);
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

    /* ANCORA sola - positiones ceterae ambulatione derivantur.
     * Lex: documentum canonicum mentiri non possit */
    si (scriptor.ancora_nota && scriptor.ancora_offset >= ZEPHYRUM)
    {
        _attributum_numeri(&scriptor, involucrum, "fons",
            (i32)scriptor.ancora_fons);
        _attributum_numeri(&scriptor, involucrum, "b",
            (i32)scriptor.ancora_offset);
        _attributum_numeri(&scriptor, involucrum, "linea",
            scriptor.ancora_linea);
        _attributum_numeri(&scriptor, involucrum, "columna",
            scriptor.ancora_columna);
        si (scriptor.ancora_initium_lineae)
        {
            stml_attributum_boolean_addere(involucrum, piscina,
                intern, "linea-initium");
        }
    }

    /* PASSUS II */
    radix = _scribere_nodum_internum(&scriptor, nodus);
    si (radix == NIHIL)
    {
        fructus.causa = scriptor.causa
            ? scriptor.causa : "scriptura fracta";
        fructus.sedes = scriptor.sedes;
        redde fructus;
    }
    si (!stml_liberum_addere(involucrum, radix))
    {
        fructus.causa = "radix in involucrum addi non potuit";
        redde fructus;
    }

    fructus.textus     = stml_scribere(involucrum, piscina, VERUM);
    fructus.successus  = VERUM;
    redde fructus;
}


/* ==================================================
 * Lector: STML canonicum -> arbor (T5a)
 * ================================================== */

nomen structura {
                           Piscina* piscina;
               InternamentumChorda* intern;
     constans SilvaRegistrumCoctum* tabularium;
                  SilvaArborVitium* vitium;
                    TabulaDispersa* fragmenta;   /* id -> SilvaToken* */
                               s32  fons_ordinarius;
} ArborLector;

/* Cursor derivationis (T5b): sedes currens in ambulatione
 * EMISSIONIS. Documentum positiones non fert - involucrum ancoram
 * solam - ergo hic eas reficimus. Lex: documentum canonicum
 * mentiri non possit. */
/* LACUNA: intervallum octetorum quod ARBORI non pertinet sed in
 * fluxu octetorum INTER lexemata eius iacet (laminae directivarum,
 * laminae regionum degradatarum).
 *
 * CUR OPUS SIT: arbor octetos suos CONTIGUOS emittit, sed in
 * plagula vera lexemata eius interrumpi possunt - regio in
 * initiatore, exempli gratia. Sine lacunis derivatio lexemati post
 * interruptionem sedem nimis parvam daret, et
 * silva_scribere_fontem (quae reinserenda per offset intertexit)
 * ordinem falsum refecisset. MENSURATUM, non divinatum (T3b). */
nomen structura {
    s32 offset;                 /* initium lacunae */
    s32 finis;                  /* post ultimum octetum */
    i32 linea_finalis;
    i32 columna_finalis;
    b32 post_lineam_finalis;
} ParsuraLacuna;

nomen structura {
    s32 offset;
    i32 linea;
    i32 columna;
    b32 post_lineam;   /* nova linea VERA visa (logica, non physica) */
    b32 sedes_notae;   /* ancora adest: ordinatas scribere licet */

    /* Lacunae, ordine offset; NIHIL = nullae (casus subarboris) */
    Xar* lacunae;
    i32  lacuna_proxima;
} ArborCursor;

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
_spatium_solum (
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

/* Textus liberorum DIRECTORUM solum.
 *
 * NON stml_textus_internus: illud posteros OMNES concatenat, quod
 * super contentum MIXTUM tacite fallit - elementum lexematis
 * elementum lexematis quod commentum in involucro 'ante' fert per
 * textum internum COMMENTUM ipsum ante valorem redderet, id est
 * textum commenti in valorem lexematis absorptum. Contractus
 * 'VERBATIM, non normalizatum' rectus manet; hoc de AMBITU est,
 * non de transformatione. */
interior chorda
_textus_directus (
    ArborLector* lector,
      StmlNodus* elementum)
{
    ChordaAedificator* aedificator;
                  i32  numerus;
                  i32  i;

    aedificator = chorda_aedificator_creare(lector->piscina, 64);
    si (aedificator == NIHIL)
    {
        chorda vacua;

        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }

    numerus = stml_numerus_liberorum(elementum);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* liberum = stml_liberum_ad_indicem(elementum, i);

        /* Textus spatii albi SOLIUS praeteritur: scriptor pulcher
         * lineas novas et indentationem inter elementa iniicit, et
         * illae nodi textus DIRECTI sunt - sine hoc omne elementum
         * lexematis pulchre scriptum valorem ferre videretur.
         * TUTUM est quia scriptor valorem spatii-albi-solius
         * REFUTAT (_textus_tutus): textus talis valor esse NON
         * potest, ergo ambiguitas nulla. Refutatio scriptoris est
         * quae hanc lectionem univocam facit - contractus unus per
         * duas partes. */
        si (   liberum        != NIHIL
            && liberum->genus == STML_NODUS_TEXTUS
            && liberum->valor != NIHIL
            && !_spatium_solum(liberum->valor))
        {
            chorda_aedificator_appendere_chorda(aedificator,
                *liberum->valor);
        }
    }
    redde chorda_aedificator_finire(aedificator);
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

/* Chordam ex charactere repetito (lentes triviorum invertere) */
interior chorda
_chorda_repetita (
     ArborLector* lector,
       character  c,
             i32  numerus)
{
    chorda fructus;
       i32 i;

    fructus.mensura = numerus;
    fructus.datum   = (i8*)piscina_allocare(lector->piscina,
        (memoriae_index)(numerus > ZEPHYRUM ? numerus : I));
    si (fructus.datum == NIHIL)
    {
        fructus.mensura = ZEPHYRUM;
        redde fructus;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        fructus.datum[i] = (i8)c;
    }
    redde fructus;
}

/* Elementum liberum proximum (textum spatii albi solius praeteriens;
 * textus alius in sede structurali RECUSATUR). *cursor promovetur. */
interior StmlNodus*
_elementum_proximum (
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
            si (_spatium_solum(liberum->valor))
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

/* Trivium unum ex elemento; lentes invertit */
interior SilvaToken*
_trivium_legere (
    ArborLector* lector,
      StmlNodus* elementum)
{
    SilvaLexemaGenus  genus;
              chorda  valor;
              chorda* attributum;
                 i32  numerus;

    si (elementum->titulus == NIHIL)
    {
        _recusare(lector, "trivium sine titulo", elementum->linea);
        redde NIHIL;
    }
    genus = silva_arbor_lexema_ex_tag(
        (constans character*)elementum->titulus->datum,
        elementum->titulus->mensura);
    si (genus == SILVA_LEX_NUMERUS_GENERUM)
    {
        _recusare(lector, "genus trivii registro ignotum",
            elementum->linea);
        redde NIHIL;
    }

    valor.mensura  = ZEPHYRUM;
    valor.datum    = NIHIL;

    commutatio (genus)
    {
        casus SILVA_LEX_SPATIA:
        casus SILVA_LEX_TABULAE:
            attributum = stml_attributum_capere(elementum, "n");
            si (!_numerus_ex_chorda(attributum, &numerus))
            {
                _recusare(lector, "trivium sine numero 'n'",
                    elementum->linea);
                redde NIHIL;
            }
            valor = _chorda_repetita(lector,
                (genus == SILVA_LEX_SPATIA) ? ' ' : '\t', numerus);
            frange;

        casus SILVA_LEX_NOVA_LINEA:
            valor = stml_attributum_habet(elementum, "crlf")
                ? chorda_ex_literis("\r\n", lector->piscina)
                : chorda_ex_literis("\n", lector->piscina);
            frange;

        casus SILVA_LEX_CONTINUATIO:
            valor = stml_attributum_habet(elementum, "crlf")
                ? chorda_ex_literis("\\\r\n", lector->piscina)
                : chorda_ex_literis("\\\n", lector->piscina);
            frange;

        ordinarius:
            valor = _textus_directus(lector, elementum);
            frange;
    }

    redde silva_token_ex_fonte(lector->piscina, genus, valor,
        -I, ZEPHYRUM, ZEPHYRUM, lector->fons_ordinarius);
}

/* Involucrum <ante>/<post> -> Xar de SilvaToken* (NIHIL si vacuum) */
interior b32
_trivia_legere (
    ArborLector*  lector,
      StmlNodus*  involucrum,
            Xar** exitus)
{
    Xar* series;
    i32  cursor;

    series = xar_creare(lector->piscina, magnitudo(SilvaToken*));
    si (series == NIHIL)
    {
        redde _recusare(lector, "series triviorum creari non potuit",
            involucrum->linea);
    }

    cursor = ZEPHYRUM;
    per (;;)
    {
        StmlNodus* liberum = _elementum_proximum(lector, involucrum,
            &cursor);
        SilvaToken* trivium;

        si (liberum == NIHIL)
        {
            si (   lector->vitium        != NIHIL
                && lector->vitium->causa != NIHIL)
            {
                redde FALSUM;
            }
            frange;
        }
        trivium = _trivium_legere(lector, liberum);
        si (trivium == NIHIL)
        {
            redde FALSUM;
        }
        {
            SilvaToken** cella = (SilvaToken**)xar_addere(series);

            si (cella == NIHIL)
            {
                redde _recusare(lector, "trivium addi non potuit",
                    liberum->linea);
            }
            *cella = trivium;
        }
    }

    *exitus = (xar_numerus(series) > ZEPHYRUM) ? series : NIHIL;
    redde VERUM;
}

interior SilvaToken*
_lexema_legere (
     ArborLector* lector,
       StmlNodus* elementum,
          chorda* fragmenti_id);

/* Originem ex elemento nestato reficere.
 *
 * ASYMMETRIA HONESTA circa DEF-SITE: documentum eum per
 * REFERENTIAM fert (def-f/def-l), non per lexemata. Ergo 'corpus'
 * lexema SYNTHETICUM fit - fontem et lineam ferens, sedem VERAM
 * non ferens (byte_offset -I). Id est quod referentia praestare
 * potest; plus fingere mendacium esset. Emissio corpus numquam
 * legit (invocatio strati 0 emittitur), ergo octeti intacti. */
interior b32
_origo_legere (
    ArborLector* lector,
      StmlNodus* elementum,
     SilvaToken* lexema)
{
    SilvaOrigoGenus  genus;
         SilvaToken* primus;
         SilvaToken* secundus;
         SilvaToken* definitio;
          StmlNodus* liberum;
             chorda* attributum;
             chorda* titulus_macro;
                i32  cursor;
                i32  numerus;

    primus     = NIHIL;
    secundus   = NIHIL;
    definitio  = NIHIL;

    si (chorda_aequalis_literis(*elementum->titulus,
            SILVA_ARBOR_TAG_EXPANSIO))
    {
        genus = SILVA_ORIGO_EXPANSIO;
    }
    alioquin si (chorda_aequalis_literis(*elementum->titulus,
                     SILVA_ARBOR_TAG_PASTA))
    {
        genus = SILVA_ORIGO_PASTA;
    }
    alioquin si (chorda_aequalis_literis(*elementum->titulus,
                     SILVA_ARBOR_TAG_STRINGIFICATIO))
    {
        genus = SILVA_ORIGO_CHORDA;
    }
    alioquin si (chorda_aequalis_literis(*elementum->titulus,
                     SILVA_ARBOR_TAG_API))
    {
        genus = SILVA_ORIGO_API;
    }
    alioquin
    {
        _recusare(lector, "genus originis ignotum", elementum->linea);
        redde FALSUM;
    }

    titulus_macro = stml_attributum_capere(elementum, "macro");

    /* Lexemata nestata: invocatio (et dexter pro PASTA) */
    cursor   = ZEPHYRUM;
    numerus  = stml_numerus_liberorum(elementum);
    per (;;)
    {
        SilvaToken* lectum;

        si (cursor >= numerus)
        {
            frange;
        }
        liberum = stml_liberum_ad_indicem(elementum, cursor);
        cursor++;
        si (liberum == NIHIL)
        {
            perge;
        }
        si (   liberum->genus != STML_NODUS_ELEMENTUM
            && liberum->genus != STML_NODUS_TRANSCLUSIO)
        {
            perge;
        }
        lectum = _lexema_legere(lector, liberum, NIHIL);
        si (lectum == NIHIL)
        {
            redde FALSUM;
        }
        si (primus == NIHIL)
        {
            primus = lectum;
        }
        alioquin si (secundus == NIHIL)
        {
            secundus = lectum;
        }
    }

    /* DEF-SITE: lexema syntheticum ex referentia */
    attributum = stml_attributum_capere(elementum, "def-l");
    si (attributum != NIHIL)
    {
           i32 linea;
           i32 fons;
           i32 columna;
        chorda vacua;

        linea    = ZEPHYRUM;
        fons     = ZEPHYRUM;
        columna  = ZEPHYRUM;
        (vacuum)_numerus_ex_chorda(attributum, &linea);
        attributum = stml_attributum_capere(elementum, "def-f");
        si (attributum != NIHIL)
        {
            (vacuum)_numerus_ex_chorda(attributum, &fons);
        }
        attributum = stml_attributum_capere(elementum, "def-c");
        si (attributum != NIHIL)
        {
            (vacuum)_numerus_ex_chorda(attributum, &columna);
        }
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        definitio = silva_token_ex_fonte(lector->piscina,
            SILVA_LEX_IDENTIFICATOR, vacua, -I, linea, columna,
            (s32)fons);
    }

    lexema->origo.genus = genus;
    commutatio (genus)
    {
    casus SILVA_ORIGO_EXPANSIO:
        lexema->origo.datum.expansio.invocatio    = primus;
        lexema->origo.datum.expansio.corpus       = definitio;
        lexema->origo.datum.expansio.nomen_macro  = titulus_macro;
        /* CAECATIO consulto NIHIL - vide _origo_scribere */
        lexema->origo.datum.expansio.caecatio     = NIHIL;
        frange;
    casus SILVA_ORIGO_PASTA:
        lexema->origo.datum.pasta.sinister     = primus;
        lexema->origo.datum.pasta.dexter       = secundus;
        lexema->origo.datum.pasta.nomen_macro  = titulus_macro;
        lexema->origo.datum.pasta.caecatio     = NIHIL;
        frange;
    casus SILVA_ORIGO_CHORDA:
        lexema->origo.datum.stringificatio.primus       = primus;
        lexema->origo.datum.stringificatio.nomen_macro  =
            titulus_macro;
        frange;
    casus SILVA_ORIGO_API:
        lexema->origo.datum.api.nomen_macro = titulus_macro;
        frange;
    ordinarius:
        frange;
    }
    redde VERUM;
}

interior SilvaToken*
_lexema_legere (
     ArborLector* lector,
       StmlNodus* elementum,
          chorda* fragmenti_id)
{
    SilvaLexemaGenus  genus;
          SilvaToken* lexema;
              chorda  valor;
              chorda* attributum;
                 i32  cursor;
                 i32  numerus;
                 b32  valor_visus;

    /* TRANSCLUSIO: lexema IDEM reddere, non par. Identitas res est -
     * bracchia ambigua obiecta EADEM ferunt, et duplicatio de arbore
     * mentiretur (geometria_fida id agnoscere scripta est). */
    si (elementum->genus == STML_NODUS_TRANSCLUSIO)
    {
        chorda  clavis;
        vacuum* inventum;

        si (elementum->valor == NIHIL || elementum->valor->mensura < II)
        {
            _recusare(lector, "transclusio sine identitate",
                elementum->linea);
            redde NIHIL;
        }
        /* valor '#lexN'; clavis fragmenti 'lexN' */
        clavis.mensura  = elementum->valor->mensura - I;
        clavis.datum    = elementum->valor->datum + I;

        si (!tabula_dispersa_invenire(lector->fragmenta, clavis,
                 &inventum))
        {
            /* Fragmentum ANTE usum definiendum est. Scriptor id
             * semper praestat (usus primus definit); documentum
             * aliunde veniens hic clare cadit. */
            _recusare(lector, "transclusio ad fragmentum ignotum",
                elementum->linea);
            redde NIHIL;
        }
        redde (SilvaToken*)inventum;
    }
    si (elementum->titulus == NIHIL)
    {
        _recusare(lector, "lexema sine titulo", elementum->linea);
        redde NIHIL;
    }

    genus = silva_arbor_lexema_ex_tag(
        (constans character*)elementum->titulus->datum,
        elementum->titulus->mensura);
    si (genus == SILVA_LEX_NUMERUS_GENERUM)
    {
        _recusare(lector, "genus lexematis registro ignotum",
            elementum->linea);
        redde NIHIL;
    }

    /* VALOR: adesse debet si et solum si orthographia varia est */
    valor        = _textus_directus(lector, elementum);
    valor_visus  = (valor.mensura > ZEPHYRUM) ? VERUM : FALSUM;

    si (valor_visus && !silva_arbor_valor_portandus(genus))
    {
        _recusare(lector, "valor in genere orthographiae fixae",
            elementum->linea);
        redde NIHIL;
    }
    si (!silva_arbor_valor_portandus(genus))
    {
        constans character* orthographia =
            silva_arbor_orthographia(genus);

        si (orthographia != NIHIL)
        {
            valor = chorda_ex_literis(orthographia, lector->piscina);
        }
    }

    lexema = silva_token_ex_fonte(lector->piscina, genus, valor,
        -I, ZEPHYRUM, ZEPHYRUM, lector->fons_ordinarius);
    si (lexema == NIHIL)
    {
        _recusare(lector, "lexema creari non potuit",
            elementum->linea);
        redde NIHIL;
    }

    attributum = stml_attributum_capere(elementum, "standard");
    si (attributum != NIHIL)
    {
        i32 gradus;

        si (!_numerus_ex_chorda(attributum, &gradus))
        {
            _recusare(lector, "standard non numerus",
                elementum->linea);
            redde NIHIL;
        }
        lexema->standard = (i8)gradus;
    }
    attributum = stml_attributum_capere(elementum, "f");
    si (attributum != NIHIL)
    {
        i32 fons;

        si (!_numerus_ex_chorda(attributum, &fons))
        {
            _recusare(lector, "fons non numerus", elementum->linea);
            redde NIHIL;
        }
        lexema->fons_index = (s32)fons;
    }

    /* SEDES PORTATA (lexema non-FONS solum - vide scriptorem) */
    attributum = stml_attributum_capere(elementum, "b");
    si (attributum != NIHIL)
    {
        i32 sedes_portata;

        si (!_numerus_ex_chorda(attributum, &sedes_portata))
        {
            _recusare(lector, "b non numerus", elementum->linea);
            redde NIHIL;
        }
        lexema->byte_offset = (s32)sedes_portata;
    }
    attributum = stml_attributum_capere(elementum, "linea");
    si (attributum != NIHIL)
    {
        i32 numerus_lineae;

        si (!_numerus_ex_chorda(attributum, &numerus_lineae))
        {
            _recusare(lector, "linea non numerus", elementum->linea);
            redde NIHIL;
        }
        lexema->linea = numerus_lineae;
    }
    attributum = stml_attributum_capere(elementum, "columna");
    si (attributum != NIHIL)
    {
        i32 numerus_columnae;

        si (!_numerus_ex_chorda(attributum, &numerus_columnae))
        {
            _recusare(lector, "columna non numerus",
                elementum->linea);
            redde NIHIL;
        }
        lexema->columna = numerus_columnae;
    }
    si (stml_attributum_habet(elementum, "linea-initium"))
    {
        lexema->initium_lineae = VERUM;
    }

    /* liberi: <ante>, <post>, <scissura> */
    cursor   = ZEPHYRUM;
    numerus  = stml_numerus_liberorum(elementum);
    per (;;)
    {
        StmlNodus* liberum;

        si (cursor >= numerus)
        {
            frange;
        }
        liberum = stml_liberum_ad_indicem(elementum, cursor);
        cursor++;
        si (   liberum          == NIHIL || liberum->genus != STML_NODUS_ELEMENTUM
            || liberum->titulus == NIHIL)
        {
            perge;
        }

        si (chorda_aequalis_literis(*liberum->titulus,
                SILVA_ARBOR_TAG_ANTE))
        {
            si (!_trivia_legere(lector, liberum, &lexema->spatia_ante))
            {
                redde NIHIL;
            }
        }
        alioquin si (chorda_aequalis_literis(*liberum->titulus,
                         SILVA_ARBOR_TAG_POST))
        {
            si (!_trivia_legere(lector, liberum, &lexema->spatia_post))
            {
                redde NIHIL;
            }
        }
        alioquin si (   chorda_aequalis_literis(*liberum->titulus,
                            SILVA_ARBOR_TAG_EXPANSIO)
                     || chorda_aequalis_literis(*liberum->titulus,
                            SILVA_ARBOR_TAG_PASTA)
                     || chorda_aequalis_literis(*liberum->titulus,
                            SILVA_ARBOR_TAG_STRINGIFICATIO)
                     || chorda_aequalis_literis(*liberum->titulus,
                            SILVA_ARBOR_TAG_API))
        {
            si (!_origo_legere(lector, liberum, lexema))
            {
                redde NIHIL;
            }
        }
        alioquin si (chorda_aequalis_literis(*liberum->titulus,
                         SILVA_ARBOR_TAG_SCISSURA))
        {
            SilvaScissura scissura;
                      i32 offset;

            attributum = stml_attributum_capere(liberum, "offset");
            si (!_numerus_ex_chorda(attributum, &offset))
            {
                _recusare(lector, "scissura sine offset",
                    liberum->linea);
                redde NIHIL;
            }
            scissura.offset  = (s32)offset;
            scissura.crlf    = stml_attributum_habet(liberum, "crlf");

            si (lexema->scissurae == NIHIL)
            {
                lexema->scissurae = xar_creare(lector->piscina,
                    magnitudo(SilvaScissura));
            }
            si (lexema->scissurae == NIHIL)
            {
                _recusare(lector, "scissurae creari non potuerunt",
                    liberum->linea);
                redde NIHIL;
            }
            {
                SilvaScissura* cella = (SilvaScissura*)
                    xar_addere(lexema->scissurae);

                si (cella == NIHIL)
                {
                    _recusare(lector, "scissura addi non potuit",
                        liberum->linea);
                    redde NIHIL;
                }
                *cella = scissura;
            }
        }
        alioquin
        {
            _recusare(lector, "elementum in lexemate ignotum",
                liberum->linea);
            redde NIHIL;
        }
    }

    /* Fragmentum: lexema sub ID suo deponere, ut transclusiones
     * sequentes HOC OBIECTUM inveniant */
    si (fragmenti_id != NIHIL)
    {
        tabula_dispersa_inserere(lector->fragmenta, *fragmenti_id,
            lexema);
    }

    redde lexema;
}

/* An tag praefixum lexematis ferat */
interior b32
_est_tag_lexematis (
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

/* Fragmentum involucrum est: contentum eius reddere, et ID eius
 * vocanti tradere (T5b id in tabulam ponit ut transclusiones
 * OBIECTUM IDEM inveniant - identitas res est, duplicatio
 * mentiretur). */
interior StmlNodus*
_fragmentum_aperire (
    ArborLector*  lector,
      StmlNodus*  elementum,
         chorda** id_exitus)
{
    i32 cursor;

    si (id_exitus != NIHIL)
    {
        *id_exitus = NIHIL;
    }
    si (elementum == NIHIL || !elementum->fragmentum)
    {
        redde elementum;
    }
    si (id_exitus != NIHIL)
    {
        *id_exitus = elementum->fragmentum_id;
    }
    cursor = ZEPHYRUM;
    redde _elementum_proximum(lector, elementum, &cursor);
}

interior SilvaNodus*
_nodum_legere (
    ArborLector*,
    StmlNodus*);

interior b32
_valorem_loci_legere (
           ArborLector* lector,
             StmlNodus* involucrum,
     SilvaLocusSpecies  species,
            SilvaNodus* nodus,
                   i32  locus)
{
     StmlNodus* liberum;
    SilvaValor  valor;
        chorda* fragmenti_id;
           i32  cursor;
           i32  numerus;

    fragmenti_id  = NIHIL;
    cursor        = ZEPHYRUM;

    si (species == SILVA_LOCUS_INDEX)
    {
        chorda textus = _textus_directus(lector, involucrum);

        si (!_numerus_ex_chorda(&textus, &numerus))
        {
            redde _recusare(lector, "locus INDEX numerum non fert",
                involucrum->linea);
        }
        si (!silva_nodus_ponere(nodus, locus,
                 silva_valor_index((s32)numerus), species))
        {
            redde _recusare(lector, "index poni non potuit",
                involucrum->linea);
        }
        redde VERUM;
    }

    si (species == SILVA_LOCUS_NODUS || species == SILVA_LOCUS_TOKEN)
    {
        liberum = _elementum_proximum(lector, involucrum, &cursor);
        si (liberum == NIHIL)
        {
            redde _recusare(lector, "locus vacuus", involucrum->linea);
        }
        liberum = _fragmentum_aperire(lector, liberum, &fragmenti_id);
        si (liberum == NIHIL)
        {
            redde FALSUM;
        }

        si (species == SILVA_LOCUS_NODUS)
        {
            SilvaNodus* filius;

            si (_est_tag_lexematis(liberum->titulus))
            {
                redde _recusare(lector, "lexema in loco NODUS",
                    liberum->linea);
            }
            filius = _nodum_legere(lector, liberum);
            si (filius == NIHIL)
            {
                redde FALSUM;
            }
            valor = silva_valor_nodus(filius);
        }
        alioquin
        {
            SilvaToken* lexema;

            si (   liberum->genus != STML_NODUS_TRANSCLUSIO
                && !_est_tag_lexematis(liberum->titulus))
            {
                redde _recusare(lector, "nodus in loco TOKEN",
                    liberum->linea);
            }
            lexema = _lexema_legere(lector, liberum, fragmenti_id);
            si (lexema == NIHIL)
            {
                redde FALSUM;
            }
            valor = silva_valor_token(lexema);
        }

        /* Plus quam unum elementum in loco singulari = forma corrupta */
        si (_elementum_proximum(lector, involucrum, &cursor) != NIHIL)
        {
            redde _recusare(lector, "locus singularis plura fert",
                involucrum->linea);
        }
        si (lector->vitium != NIHIL && lector->vitium->causa != NIHIL)
        {
            redde FALSUM;
        }
        si (!silva_nodus_ponere(nodus, locus, valor, species))
        {
            redde _recusare(lector, "valor loci poni non potuit",
                involucrum->linea);
        }
        redde VERUM;
    }

    /* LISTAE - lector elementa IPSE custodire debet: appendere
     * speciem loci solam probat, numquam quid intus eat */
    valor = silva_valor_lista_nova(lector->piscina);
    per (;;)
    {
        b32 est_lexema;

        liberum = _elementum_proximum(lector, involucrum, &cursor);
        si (liberum == NIHIL)
        {
            si (   lector->vitium        != NIHIL
                && lector->vitium->causa != NIHIL)
            {
                redde FALSUM;
            }
            frange;
        }
        liberum = _fragmentum_aperire(lector, liberum, &fragmenti_id);
        si (liberum == NIHIL)
        {
            redde FALSUM;
        }

        est_lexema = (liberum->genus == STML_NODUS_TRANSCLUSIO
                      || _est_tag_lexematis(liberum->titulus))
            ? VERUM : FALSUM;

        si (est_lexema)
        {
            SilvaToken* lexema;

            si (species == SILVA_LOCUS_LISTA_NODUS)
            {
                redde _recusare(lector, "lexema in lista NODUS",
                    liberum->linea);
            }
            lexema = _lexema_legere(lector, liberum, fragmenti_id);
            si (lexema == NIHIL)
            {
                redde FALSUM;
            }
            valor = silva_valor_lista_appendere(lector->piscina, valor,
                silva_valor_token(lexema));
        }
        alioquin
        {
            SilvaNodus* filius;

            si (species == SILVA_LOCUS_LISTA_TOKEN)
            {
                redde _recusare(lector, "nodus in lista TOKEN",
                    liberum->linea);
            }
            filius = _nodum_legere(lector, liberum);
            si (filius == NIHIL)
            {
                redde FALSUM;
            }
            valor = silva_valor_lista_appendere(lector->piscina, valor,
                silva_valor_nodus(filius));
        }
    }

    si (!silva_nodus_ponere(nodus, locus, valor, species))
    {
        redde _recusare(lector, "lista poni non potuit",
            involucrum->linea);
    }
    redde VERUM;
}

interior SilvaNodus*
_nodum_legere (
    ArborLector* lector,
      StmlNodus* elementum)
{
    constans SilvaTabGenus* genus;
                SilvaNodus* nodus;
                       s32  genus_index;
                       i32  cursor;

    si (elementum == NIHIL || elementum->titulus == NIHIL)
    {
        _recusare(lector, "nodus sine titulo",
            elementum ? elementum->linea : ZEPHYRUM);
        redde NIHIL;
    }

    genus_index = silva_arbor_genus_index(lector->tabularium,
        (constans character*)elementum->titulus->datum,
        elementum->titulus->mensura);
    si (genus_index < ZEPHYRUM)
    {
        _recusare(lector, "genus registro ignotum", elementum->linea);
        redde NIHIL;
    }
    genus = &lector->tabularium->genera[genus_index];

    /* Semita CUSTODITA (S32): species probata, semel-tantum scribere */
    nodus = silva_nodus_creare(lector->piscina, genus_index,
        genus->loci_numerus);
    si (nodus == NIHIL)
    {
        _recusare(lector, "nodus creari non potuit", elementum->linea);
        redde NIHIL;
    }

    cursor = ZEPHYRUM;
    per (;;)
    {
        StmlNodus* involucrum;
              s32  absolutus;
              i32  relativus;

        involucrum = _elementum_proximum(lector, elementum, &cursor);
        si (involucrum == NIHIL)
        {
            si (   lector->vitium        != NIHIL
                && lector->vitium->causa != NIHIL)
            {
                redde NIHIL;
            }
            frange;
        }
        si (involucrum->titulus == NIHIL)
        {
            _recusare(lector, "involucrum loci sine titulo",
                involucrum->linea);
            redde NIHIL;
        }

        absolutus = silva_arbor_locus_index(lector->tabularium,
            genus_index,
            (constans character*)involucrum->titulus->datum,
            involucrum->titulus->mensura);
        si (absolutus < ZEPHYRUM)
        {
            _recusare(lector, "locus generi ignotus",
                involucrum->linea);
            redde NIHIL;
        }
        relativus = (i32)absolutus - genus->loci_offset;

        si (!_valorem_loci_legere(lector, involucrum,
                 (SilvaLocusSpecies)
                     lector->tabularium->loci[absolutus].species,
                 nodus, relativus))
        {
            redde NIHIL;
        }
    }
    redde nodus;
}


/* ==================================================
 * Fixurae (T5b): positiones ex ancora derivare
 *
 * Documentum positiones NON fert; involucrum ancoram solam. Hic
 * ambulatione EMISSIONIS (eadem quam scriptor sequitur: loci ordine,
 * in altum) sedes reficimus. Lex: documentum canonicum mentiri non
 * possit - positio portata cum contento dissentire potest, derivata
 * non potest.
 * ================================================== */

interior vacuum
_cursorem_promovere (
        ArborCursor* cursor,
    constans chorda* octeti)
{
    i32 i;

    si (octeti == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < octeti->mensura; i++)
    {
        si ((character)octeti->datum[i] == '\n')
        {
            cursor->linea++;
            cursor->columna = I;
        }
        alioquin
        {
            cursor->columna++;
        }
    }
    cursor->offset += (s32)octeti->mensura;
}

interior vacuum
_trivium_ponere (
    ArborCursor* cursor,
     SilvaToken* trivium)
{
    si (trivium == NIHIL)
    {
        redde;
    }
    si (cursor->sedes_notae)
    {
        trivium->byte_offset  = cursor->offset;
        trivium->linea        = cursor->linea;
        trivium->columna      = cursor->columna;
    }

    _cursorem_promovere(cursor, &trivium->valor);

    /* Linea LOGICA: NOVA_LINEA lineam novam incipit, CONTINUATIO
     * NON (lamina est - linea physica crescit, logica perstat).
     * initium_lineae lineas LOGICAS numerat (silva_lexema.h). */
    si (trivium->genus == SILVA_LEX_NOVA_LINEA)
    {
        cursor->post_lineam = VERUM;
    }
}

interior vacuum
_lexema_ponere (
    ArborCursor* cursor,
     SilvaToken* lexema)
{
    i32 i;
    i32 s;
    i32 numerus_scissurarum;

    /* initium_lineae et longitudo ex TRIVIIS et VALORE derivantur,
     * non ex ancora - ergo arbor AUCTORATA (sine textu fontis) eas
     * tamen recte accipit. Ancora ORDINATAS solas regit: 'sine
     * ancora, sine ordinatis' rectum est; 'sine ancora, nihil
     * derivatum' non esset. */
    lexema->initium_lineae  = cursor->post_lineam;
    lexema->longitudo       = lexema->valor.mensura;
    si (cursor->sedes_notae)
    {
        lexema->byte_offset  = cursor->offset;
        lexema->linea        = cursor->linea;
        lexema->columna      = cursor->columna;
    }

    numerus_scissurarum = lexema->scissurae
        ? xar_numerus(lexema->scissurae) : (i32)ZEPHYRUM;
    s = ZEPHYRUM;

    /* Valor MUNDUS est; laminae emissae reinseruntur, ergo sedes
     * eas numerare debent (aliter omnia post lexema lamina-ferens
     * labuntur) */
    per (i = ZEPHYRUM; i <= lexema->valor.mensura; i++)
    {
        dum (s < numerus_scissurarum)
        {
            SilvaScissura* scissura = (SilvaScissura*)
                xar_obtinere(lexema->scissurae, s);

            si (scissura == NIHIL || scissura->offset != (s32)i)
            {
                frange;
            }
            cursor->offset += scissura->crlf ? III : II;
            cursor->linea++;
            cursor->columna = I;
            s++;
        }
        si (i == lexema->valor.mensura)
        {
            frange;
        }
        si ((character)lexema->valor.datum[i] == '\n')
        {
            cursor->linea++;
            cursor->columna = I;
        }
        alioquin
        {
            cursor->columna++;
        }
        cursor->offset++;
    }
    cursor->post_lineam = FALSUM;
}

interior vacuum
_positiones_valoris (
    ArborCursor*,
    SilvaValor);

interior vacuum
_positiones_lexematis (
    ArborCursor* cursor,
     SilvaToken* lexema)
{
    i32 numerus;
    i32 i;

    si (lexema == NIHIL)
    {
        redde;
    }
    /* LEXEMA EXPANSUM OCTETOS NON TENET. In fluxu octetorum
     * invocatio strati 0 stat (silva_scribere.h:22: 'lexema cuius
     * origo non FONS est non se emittit - invocatio strati 0
     * emittitur'), ergo derivatio idem facere DEBET: invocationem
     * ponere, expansionem praeterire. Aliter cursor bis promovetur
     * - semel expansione, semel invocatione - et sedes omnes
     * post primam macro labuntur.
     *
     * Dedup 'semel per instantiam' custodia 'iam positum' supra
     * gratis venit: invocatio inter lexemata expansa COMMUNICATA
     * est, ergo secundo visu sedem iam habet. */
    si (lexema->origo.genus != SILVA_ORIGO_FONS)
    {
        SilvaToken* invocatio;

        invocatio = NIHIL;
        commutatio (lexema->origo.genus)
        {
        casus SILVA_ORIGO_EXPANSIO:
            invocatio = lexema->origo.datum.expansio.invocatio;
            frange;
        casus SILVA_ORIGO_PASTA:
            invocatio = lexema->origo.datum.pasta.sinister;
            frange;
        casus SILVA_ORIGO_CHORDA:
            invocatio = lexema->origo.datum.stringificatio.primus;
            frange;
        ordinarius:
            frange;
        }
        si (invocatio != NIHIL)
        {
            _positiones_lexematis(cursor, invocatio);
        }
        redde;
    }

    /* IAM POSITUM: lexema communicatum (bracchia ambigua obiecta
     * eadem ferunt) eosdem octetos fontis tegit, ergo cursor bis
     * promoveri NON debet - alioquin bracchium secundum omnia post
     * se labi faceret */
    si (cursor->sedes_notae && lexema->byte_offset >= ZEPHYRUM)
    {
        redde;
    }

    /* LACUNAS transilire ad quas cursor pervenit. Omnis derivatio
     * per hanc functionem fluit, ergo unum punctum mutationis. */
    dum (   cursor->lacunae != NIHIL
         && cursor->lacuna_proxima < xar_numerus(cursor->lacunae))
    {
        ParsuraLacuna* lacuna;

        lacuna = (ParsuraLacuna*)xar_obtinere(cursor->lacunae,
            cursor->lacuna_proxima);
        si (lacuna == NIHIL || lacuna->finis <= cursor->offset)
        {
            cursor->lacuna_proxima++;
            perge;
        }
        si (lacuna->offset > cursor->offset)
        {
            frange;
        }
        cursor->offset       = lacuna->finis;
        cursor->linea        = lacuna->linea_finalis;
        cursor->columna      = lacuna->columna_finalis;
        cursor->post_lineam  = lacuna->post_lineam_finalis;
        cursor->lacuna_proxima++;
    }

    numerus = lexema->spatia_ante
        ? xar_numerus(lexema->spatia_ante) : (i32)ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        _trivium_ponere(cursor,
            *(SilvaToken**)xar_obtinere(lexema->spatia_ante, i));
    }

    _lexema_ponere(cursor, lexema);

    numerus = lexema->spatia_post
        ? xar_numerus(lexema->spatia_post) : (i32)ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        _trivium_ponere(cursor,
            *(SilvaToken**)xar_obtinere(lexema->spatia_post, i));
    }
}

interior vacuum
_positiones_nodi (
    ArborCursor* cursor,
     SilvaNodus* nodus)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        _positiones_valoris(cursor, nodus->loci[i]);
    }
}

interior vacuum
_positiones_valoris (
     ArborCursor* cursor,
      SilvaValor  valor)
{
    i32 numerus;
    i32 i;

    commutatio (valor.genus)
    {
        casus SILVA_VALOR_NODUS:
            _positiones_nodi(cursor, valor.datum.nodus);
            frange;
        casus SILVA_VALOR_TOKEN:
            _positiones_lexematis(cursor, valor.datum.token);
            frange;
        casus SILVA_VALOR_LISTA:
            numerus = silva_valor_lista_numerus(valor);
            per (i = ZEPHYRUM; i < numerus; i++)
            {
                SilvaValor* elementum =
                    silva_valor_lista_obtinere(valor, i);

                si (elementum != NIHIL)
                {
                    _positiones_valoris(cursor, *elementum);
                }
            }
            frange;
        ordinarius:
            frange;
    }
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
     ArborLector  lector;
    StmlResultus  resultus;
       StmlNodus* involucrum;
       StmlNodus* radix;
      SilvaNodus* arbor;
     ArborCursor  sedes;
          chorda* attributum;
          chorda  sigillum;
             i32  cursor;
             i32  numerus;
             b32  ancora_adest;

    si (vitium != NIHIL)
    {
        vitium->causa = NIHIL;
        vitium->linea = ZEPHYRUM;
    }

    lector.piscina          = piscina;
    lector.intern           = intern;
    lector.tabularium       = tabularium;
    lector.vitium           = vitium;
    lector.fragmenta        = NIHIL;
    lector.fons_ordinarius  = ZEPHYRUM;

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

    involucrum = resultus.elementum_radix;
    si (   involucrum == NIHIL || involucrum->titulus == NIHIL
        || !chorda_aequalis_literis(*involucrum->titulus,
                SILVA_ARBOR_TAG_ENVOLUCRI))
    {
        _recusare(&lector, "involucrum <arbor> deest",
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

    /* SIGILLUM: arbor vocabulario FALSO iudicata mendacium est */
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

    /* ANCORA: sedes lexematis PRIMI ordine ambulationis. Absens =
     * arbor AUCTORATA (sine fonte) - positiones -I manent, quod
     * legitimum est et quod comparator per provenientiam custodit. */
    ancora_adest          = FALSUM;
    sedes.offset          = ZEPHYRUM;
    sedes.linea           = I;
    sedes.columna         = I;
    sedes.post_lineam     = VERUM;
    sedes.lacunae         = NIHIL;
    sedes.lacuna_proxima  = ZEPHYRUM;

    attributum = stml_attributum_capere(involucrum, "b");
    si (attributum != NIHIL && _numerus_ex_chorda(attributum, &numerus))
    {
        sedes.offset = (s32)numerus;
        ancora_adest = VERUM;
        sedes.post_lineam = stml_attributum_habet(involucrum,
            "linea-initium");
    }
    attributum = stml_attributum_capere(involucrum, "linea");
    si (attributum != NIHIL && _numerus_ex_chorda(attributum, &numerus))
    {
        sedes.linea = numerus;
    }
    attributum = stml_attributum_capere(involucrum, "columna");
    si (attributum != NIHIL && _numerus_ex_chorda(attributum, &numerus))
    {
        sedes.columna = numerus;
    }
    attributum = stml_attributum_capere(involucrum, "fons");
    si (attributum != NIHIL && _numerus_ex_chorda(attributum, &numerus))
    {
        lector.fons_ordinarius = (s32)numerus;
    }

    lector.fragmenta = tabula_dispersa_creare_chorda(piscina, 64);
    si (lector.fragmenta == NIHIL)
    {
        _recusare(&lector, "tabula fragmentorum creari non potuit",
            involucrum->linea);
        redde NIHIL;
    }

    cursor  = ZEPHYRUM;
    radix   = _elementum_proximum(&lector, involucrum, &cursor);
    si (radix == NIHIL)
    {
        _recusare(&lector, "involucrum arborem non fert",
            involucrum->linea);
        redde NIHIL;
    }
    radix = _fragmentum_aperire(&lector, radix, NIHIL);
    si (radix == NIHIL)
    {
        redde NIHIL;
    }

    arbor = _nodum_legere(&lector, radix);
    si (arbor == NIHIL)
    {
        redde NIHIL;
    }

    /* FIXURAE (T5b) */
    sedes.sedes_notae = ancora_adest;
    _positiones_nodi(&sedes, arbor);

    /* PATER: commissio eum figit (S27) - nulla ambulatio nostra.
     * Praeterea arbor lecta NON interrogari NEQUE re-canonicari
     * potest ante commissionem: 'ambigui' ab hac sola ambulatione
     * impletur. Forma nuda (sine oraculo, sine resolutore) expresse
     * sancta est - silva_commissio.h:163-165. */
    si (silva_committere(piscina, silva_valor_nodus(arbor), tabularium,
            NIHIL, NIHIL, NIHIL) == NIHIL)
    {
        _recusare(&lector, "arbor committi non potuit",
            involucrum->linea);
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
        commutatio (lexema->origo.genus)
        {
        casus SILVA_ORIGO_EXPANSIO:
            proximum = lexema->origo.datum.expansio.invocatio;
            frange;
        casus SILVA_ORIGO_PASTA:
            proximum = lexema->origo.datum.pasta.sinister;
            frange;
        casus SILVA_ORIGO_CHORDA:
            proximum = lexema->origo.datum.stringificatio.primus;
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
    si (   lexema->spatia_ante != NIHIL
        && xar_numerus(lexema->spatia_ante) > ZEPHYRUM)
    {
        constans SilvaToken* trivium;

        trivium = *(SilvaToken**)xar_obtinere(lexema->spatia_ante,
            ZEPHYRUM);
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
    si (lexema->initium_lineae)
    {
        stml_attributum_boolean_addere(elementum, scriptor->piscina,
            scriptor->intern, "linea-initium");
    }
}

/* Offset primi lexematis ordine AMBULATIONIS; -I si nullum lexema.
 * Ordo ambulationis (non minimum octetorum) eligitur ut idem sit
 * ordo quo scriptor emittit. */
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
        redde valor.datum.token->byte_offset;
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
    r->offset       = primum->byte_offset;
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
    si (r->genus != PARSURA_REINS_DIRECTIVA)
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
        scriptum = _scribere_lexema(scriptor, lexema);
        si (scriptum == NIHIL)
        {
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

    fructus.successus       = FALSUM;
    fructus.textus.datum    = NIHIL;
    fructus.textus.mensura  = ZEPHYRUM;
    fructus.causa           = NIHIL;
    fructus.sedes           = NIHIL;

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

    scriptor.piscina                = piscina;
    scriptor.intern                 = intern;
    scriptor.tabularium             = tabularium;
    scriptor.expansio               = parsura->expansio;
    scriptor.numerus_notarum        = ZEPHYRUM;
    scriptor.ancora_nota            = FALSUM;
    scriptor.ancora_offset          = -I;
    scriptor.ancora_linea           = ZEPHYRUM;
    scriptor.ancora_columna         = ZEPHYRUM;
    scriptor.ancora_fons            = ZEPHYRUM;
    scriptor.ancora_initium_lineae  = FALSUM;
    scriptor.causa                  = NIHIL;
    scriptor.sedes                  = NIHIL;
    scriptor.lexemata         = tabula_dispersa_creare_chorda(
        piscina, 256);
    si (scriptor.lexemata == NIHIL)
    {
        fructus.causa = "tabula lexematum creari non potuit";
        redde fructus;
    }

    radix = parsura->commissio->radix;

    /* PASSUS I - numeratio usuum, UNA per documentum TOTUM */
    _numerare_valorem(&scriptor, radix);

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
            scriptum = _scribere_nodum_internum(&scriptor,
                elementum->datum.nodus);
            si (scriptum == NIHIL)
            {
                fructus.causa = scriptor.causa ? scriptor.causa
                                               : "scriptura fracta";
                fructus.sedes = scriptor.sedes;
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

        scriptum = _scribere_nodum_internum(&scriptor,
            radix.datum.nodus);
        si (scriptum == NIHIL)
        {
            fructus.causa = scriptor.causa ? scriptor.causa
                                           : "scriptura fracta";
            fructus.sedes = scriptor.sedes;
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
        lexema = _scribere_lexema(&scriptor, parsura->lexema_finis);
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

    fructus.textus     = stml_scribere(involucrum, piscina, VERUM);
    fructus.successus  = VERUM;
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
    sectio = _elementum_proximum(lector, involucrum, &cursor);
    si (   sectio == NIHIL || sectio->titulus == NIHIL
        || !chorda_aequalis_literis(*sectio->titulus,
               SILVA_ARBOR_TAG_FONTES))
    {
        _recusare(lector, "sectio <fontes> deest",
            sectio ? sectio->linea : involucrum->linea);
        redde FALSUM;
    }

    intra = ZEPHYRUM;
    dum ((elem = _elementum_proximum(lector, sectio, &intra))
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

/* Lacunam notare: ab initio dato usque ad sedem quam cursor post
 * laminam lectam tenet. Extentum ergo ex DERIVATIONE IPSA venit -
 * nullus fons veritatis secundus. */
interior b32
_parsura_lacunam_notare (
                     Xar* lacunae,
                     s32  initium,
    constans ArborCursor* post)
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
    lacuna->linea_finalis        = post->linea;
    lacuna->columna_finalis      = post->columna;
    lacuna->post_lineam_finalis  = post->post_lineam;
    redde VERUM;
}

/* Ancoram elementi in cursorem. Absente ancora cursor ab initio
 * plagulae incipit (casus arboris auctoratae). */
interior vacuum
_parsura_ancoram_legere (
      StmlNodus* elementum,
    ArborCursor* cursor)
{
    chorda* attributum;
       i32  numerus;

    cursor->offset       = ZEPHYRUM;
    cursor->linea        = I;
    cursor->columna      = I;
    cursor->post_lineam  = VERUM;
    cursor->sedes_notae  = FALSUM;
    /* lacunae/lacuna_proxima INTACTAE - vocans eas possidet */

    attributum = stml_attributum_capere(elementum, "b");
    si (   attributum != NIHIL && _numerus_ex_chorda(attributum,
            &numerus))
    {
        cursor->offset       = (s32)numerus;
        cursor->sedes_notae  = VERUM;
        cursor->post_lineam  = stml_attributum_habet(elementum,
            "linea-initium");
    }
    attributum = stml_attributum_capere(elementum, "linea");
    si (   attributum != NIHIL && _numerus_ex_chorda(attributum,
            &numerus))
    {
        cursor->linea = numerus;
    }
    attributum = stml_attributum_capere(elementum, "columna");
    si (   attributum != NIHIL && _numerus_ex_chorda(attributum,
            &numerus))
    {
        cursor->columna = numerus;
    }
}

/* Lexemata liberorum elementi in laminam novam. Directivae et
 * laminae crudae eandem formam habent - series lexematum. */
interior Xar*
_parsura_laminam_legere (
     ArborLector* lector,
       StmlNodus* elementum,
     ArborCursor* sedes)
{
           Xar* lamina;
     StmlNodus* liber;
           i32  cursor;

    lamina = xar_creare(lector->piscina, magnitudo(SilvaToken*));
    si (lamina == NIHIL)
    {
        _recusare(lector, "lamina creari non potuit",
            elementum->linea);
        redde NIHIL;
    }
    cursor = ZEPHYRUM;
    dum ((liber = _elementum_proximum(lector, elementum, &cursor))
             != NIHIL)
    {
         SilvaToken*  lexema;
         SilvaToken** sedes_lexematis;

        lexema = _lexema_legere(lector, liber, NIHIL);
        si (lexema == NIHIL)
        {
            redde NIHIL;
        }
        /* Sedes ordine DOCUMENTI - laminae in fluxu octetorum inter
         * nodos iacent, ergo cursor eas videre DEBET */
        _positiones_lexematis(sedes, lexema);
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

SilvaParsura*
silva_arbor_legere_parsuram (
                           Piscina* piscina,
               InternamentumChorda* intern,
                            chorda  textus,
     constans SilvaRegistrumCoctum* tabularium,
                constans character* grammatica,
                  SilvaArborVitium* vitium)
{
      ArborLector  lector;
      ArborCursor  sedes;
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

    lector.piscina          = piscina;
    lector.intern           = intern;
    lector.tabularium       = tabularium;
    lector.vitium           = vitium;
    lector.fragmenta        = NIHIL;
    lector.fons_ordinarius  = ZEPHYRUM;

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

    involucrum = resultus.elementum_radix;
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

    lector.fragmenta = tabula_dispersa_creare_chorda(piscina, 64);
    si (lector.fragmenta == NIHIL)
    {
        _recusare(&lector, "tabula fragmentorum creari non potuit",
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
        lector.fons_ordinarius = (s32)numerus;
    }

    /* Liberi involucri ordine DOCUMENTI, qui ordo PLAGULAE est.
     * <fontes> iam consumpta; <cauda> caudam fert; cetera nodi. */
    /* SEDES ab INITIO PLAGULAE. Derivatio ordine DOCUMENTI fit,
     * non ordine arboris: directivae ex arbore RETRAHUNTUR sed in
     * FLUXU OCTETORUM inter nodos iacent, ergo cursor eas videre
     * DEBET - aliter offsets omnes post directivam primam labuntur
     * et silva_scribere_fontem (quae per offset intertexit)
     * ordinem falsum reficit. */
    sedes.offset          = ZEPHYRUM;
    sedes.linea           = I;
    sedes.columna         = I;
    sedes.post_lineam     = VERUM;
    sedes.lacunae         = NIHIL;
    sedes.lacuna_proxima  = ZEPHYRUM;
    sedes.sedes_notae     = VERUM;

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
        dum ((elem = _elementum_proximum(&lector, involucrum, &cursor))
                 != NIHIL)
        {
            SilvaNodus* nodus;
             StmlNodus* apertum;

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
                    _recusare(&lector, "regio sine indice", elem->linea);
                    redde NIHIL;
                }
                index_regionis = (s32)numerus_attributi;

                attributum = stml_attributum_capere(elem, "pater");
                si (   attributum != NIHIL
                    && _numerus_ex_chorda(attributum, &numerus_attributi))
                {
                    index_patris = (s32)numerus_attributi;
                }
                attributum = stml_attributum_capere(elem, "regio-fons");
                si (   attributum != NIHIL
                    && _numerus_ex_chorda(attributum, &numerus_attributi))
                {
                    regio_fons = (s32)numerus_attributi;
                }
                attributum = stml_attributum_capere(elem, "regio-linea");
                si (   attributum != NIHIL
                    && _numerus_ex_chorda(attributum, &numerus_attributi))
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

                _parsura_ancoram_legere(elem, &sedes);
                initium_lacunae = sedes.offset;
                lamina = _parsura_laminam_legere(&lector, elem, &sedes);
                si (lamina == NIHIL)
                {
                    redde NIHIL;
                }
                si (!_parsura_lacunam_notare(lacunae,
                         initium_lacunae, &sedes))
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
                    _recusare(&lector, "ramus sine indice", elem->linea);
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
                    && _numerus_ex_chorda(attributum, &numerus_attributi))
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

            /* RETRACTIO: directivae ex ordine documenti in campum
             * suum. Documentum eas ubi scriptae sunt fert (superficies
             * editionis); parsura ONERATA eas seorsum fert, ut arbor
             * PURA maneat - id est quod subarborem motam octetos suos
             * ubicumque emittere sinit. */
            si (   elem->titulus != NIHIL
                && chorda_aequalis_literis(*elem->titulus,
                       SILVA_ARBOR_TAG_DIRECTIVA))
            {
                StmlNodus*  lexema_elem;
                      Xar*  lamina;
                      Xar** sedes_laminae;
                      i32   intra;
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
                lamina = xar_creare(piscina, magnitudo(SilvaToken*));
                si (lamina == NIHIL)
                {
                    _recusare(&lector, "lamina creari non potuit",
                        elem->linea);
                    redde NIHIL;
                }
                _parsura_ancoram_legere(elem, &sedes);
                initium_lacunae  = sedes.offset;
                intra            = ZEPHYRUM;
                dum ((lexema_elem = _elementum_proximum(&lector, elem,
                          &intra)) != NIHIL)
                {
                    SilvaToken*  lexema;
                    SilvaToken** sedes_lexematis;

                    lexema = _lexema_legere(&lector, lexema_elem, NIHIL);
                    si (lexema == NIHIL)
                    {
                        redde NIHIL;
                    }
                    _positiones_lexematis(&sedes, lexema);
                    sedes_lexematis = (SilvaToken**)xar_addere(lamina);
                    si (sedes_lexematis == NIHIL)
                    {
                        _recusare(&lector,
                            "lexema in laminam addi non potuit",
                            elem->linea);
                        redde NIHIL;
                    }
                    *sedes_lexematis = lexema;
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
                         initium_lacunae, &sedes))
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

                intra     = ZEPHYRUM;
                interius  = _elementum_proximum(&lector, elem, &intra);
                si (interius == NIHIL)
                {
                    _recusare(&lector, "<cauda> lexema non fert",
                        elem->linea);
                    redde NIHIL;
                }
                _parsura_ancoram_legere(elem, &sedes);
                parsura->lexema_finis = _lexema_legere(&lector, interius,
                    NIHIL);
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
            apertum = _fragmentum_aperire(&lector, elem, NIHIL);
            si (apertum == NIHIL)
            {
                redde NIHIL;
            }
            nodus = _nodum_legere(&lector, apertum);
            si (nodus == NIHIL)
            {
                redde NIHIL;
            }
            _parsura_ancoram_legere(elem, &sedes);
            _positiones_nodi(&sedes, nodus);
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
        _positiones_lexematis(&sedes, parsura->lexema_finis);
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
