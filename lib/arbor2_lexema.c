/* ==================================================
 * ARBOR2 LEXEMA - C89 Lexer Implementation (v2)
 *
 * Forked from arbor_lexema.c for arbor-v2.
 * Key differences:
 * - NOVA_LINEA token type for explicit newlines
 * - Line continuation tracking (est_continuatio)
 * - Phase 2.7: NOVA_LINEA collected as spatia for roundtrip
 *   preservation. Preprocessor checks spatia_post for
 *   directive boundary detection.
 * ================================================== */

#include "arbor2_lexema.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Lexator State (Internal)
 * ================================================== */

structura Arbor2Lexator {
    constans character*       fons;
    i32                       mensura;
    i32                       positus;
    i32                       linea;
    i32                       columna;
    Piscina*                  piscina;
    InternamentumChorda*      intern;
    Xar*                      spatia_pendentia;   /* Spatia collecta pro proximo lexema (Xar of Arbor2Lexema*) */
};

/* ==================================================
 * Verba Clausa (C89 Keywords)
 * ================================================== */

hic_manens structura {
    constans character*  verbum;
    Arbor2LexemaGenus    genus;
} VERBA_CLAUSA_C[] = {
    {"auto",      ARBOR2_LEXEMA_AUTO},
    {"break",     ARBOR2_LEXEMA_BREAK},
    {"case",      ARBOR2_LEXEMA_CASE},
    {"char",      ARBOR2_LEXEMA_CHAR},
    {"const",     ARBOR2_LEXEMA_CONST},
    {"continue",  ARBOR2_LEXEMA_CONTINUE},
    {"default",   ARBOR2_LEXEMA_DEFAULT},
    {"do",        ARBOR2_LEXEMA_DO},
    {"double",    ARBOR2_LEXEMA_DOUBLE},
    {"else",      ARBOR2_LEXEMA_ELSE},
    {"enum",      ARBOR2_LEXEMA_ENUM},
    {"extern",    ARBOR2_LEXEMA_EXTERN},
    {"float",     ARBOR2_LEXEMA_FLOAT},
    {"for",       ARBOR2_LEXEMA_FOR},
    {"goto",      ARBOR2_LEXEMA_GOTO},
    {"if",        ARBOR2_LEXEMA_IF},
    {"int",       ARBOR2_LEXEMA_INT},
    {"long",      ARBOR2_LEXEMA_LONG},
    {"register",  ARBOR2_LEXEMA_REGISTER},
    {"return",    ARBOR2_LEXEMA_RETURN},
    {"short",     ARBOR2_LEXEMA_SHORT},
    {"signed",    ARBOR2_LEXEMA_SIGNED},
    {"sizeof",    ARBOR2_LEXEMA_SIZEOF},
    {"static",    ARBOR2_LEXEMA_STATIC},
    {"struct",    ARBOR2_LEXEMA_STRUCT},
    {"switch",    ARBOR2_LEXEMA_SWITCH},
    {"typedef",   ARBOR2_LEXEMA_TYPEDEF},
    {"union",     ARBOR2_LEXEMA_UNION},
    {"unsigned",  ARBOR2_LEXEMA_UNSIGNED},
    {"void",      ARBOR2_LEXEMA_VOID},
    {"volatile",  ARBOR2_LEXEMA_VOLATILE},
    {"while",     ARBOR2_LEXEMA_WHILE},
    {NIHIL,       ARBOR2_LEXEMA_ERROR}
};

/* ==================================================
 * Nomina Generum (pro debugging)
 * ================================================== */

hic_manens constans character* NOMINA_GENERUM[] = {
    /* C89 Keywords (32) */
    "AUTO",
    "BREAK",
    "CASE",
    "CHAR",
    "CONST",
    "CONTINUE",
    "DEFAULT",
    "DO",
    "DOUBLE",
    "ELSE",
    "ENUM",
    "EXTERN",
    "FLOAT",
    "FOR",
    "GOTO",
    "IF",
    "INT",
    "LONG",
    "REGISTER",
    "RETURN",
    "SHORT",
    "SIGNED",
    "SIZEOF",
    "STATIC",
    "STRUCT",
    "SWITCH",
    "TYPEDEF",
    "UNION",
    "UNSIGNED",
    "VOID",
    "VOLATILE",
    "WHILE",

    /* Identifiers and Literals */
    "IDENTIFICATOR",
    "INTEGER",
    "FLOAT_LIT",
    "CHAR_LIT",
    "STRING_LIT",

    /* Arithmetic Operators */
    "PLUS",
    "MINUS",
    "ASTERISCUS",
    "SOLIDUS",
    "PERCENTUM",

    /* Bitwise Operators */
    "AMPERSAND",
    "PIPA",
    "CARET",
    "TILDE",

    /* Logical Operators */
    "EXCLAMATIO",
    "DUAMPERSAND",
    "DUPIPA",

    /* Comparison Operators */
    "AEQUALIS",
    "NON_AEQUALIS",
    "MINOR",
    "MAIOR",
    "MINOR_AEQ",
    "MAIOR_AEQ",

    /* Shift Operators */
    "SINISTRUM",
    "DEXTRUM",

    /* Assignment Operators */
    "ASSIGNATIO",
    "PLUS_ASSIGN",
    "MINUS_ASSIGN",
    "MULT_ASSIGN",
    "DIV_ASSIGN",
    "MOD_ASSIGN",
    "AND_ASSIGN",
    "OR_ASSIGN",
    "XOR_ASSIGN",
    "SHL_ASSIGN",
    "SHR_ASSIGN",

    /* Increment/Decrement */
    "DUPLUS",
    "DUMINUS",

    /* Member Access */
    "PUNCTUM",
    "SAGITTA",

    /* Punctuation */
    "COMMA",
    "COLON",
    "SEMICOLON",
    "QUAESTIO",

    /* Delimiters */
    "PAREN_APERTA",
    "PAREN_CLAUSA",
    "BRACKET_APERTA",
    "BRACKET_CLAUSA",
    "BRACE_APERTA",
    "BRACE_CLAUSA",

    /* Preprocessor */
    "HASH",
    "HASH_HASH",

    /* Ellipsis */
    "ELLIPSIS",

    /* NEW: Explicit newline token */
    "NOVA_LINEA",

    /* Whitespace tokens (Phase 2.6) */
    "SPATIA",
    "TABULAE",
    "CONTINUATIO",

    /* Comment tokens (Phase 2.6) */
    "COMMENTUM_CLAUSUM",
    "COMMENTUM_LINEA",

    /* Special */
    "EOF",
    "ERROR",

    /* Pseudo-type for keyword macro table */
    "TYPEDEF_MACRO"
};

/* ==================================================
 * Helper Functions - Const Cast for Interning
 *
 * chorda_internare() facit copiam, ergo castum const->non-const
 * est salvum. Usamus unionem ad evitandum monitum compilatoris.
 * ================================================== */

interior chorda
_chorda_ex_fonte(constans character* fons, i32 mensura)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = fons;
    c.datum = u.m;
    c.mensura = mensura;

    redde c;
}

/* ==================================================
 * Helper Functions - Character Classification
 * ================================================== */

interior b32
_est_cifra(character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_cifra_hex(character c)
{
    redde (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

interior b32
_est_cifra_octalis(character c)
{
    redde c >= '0' && c <= '7';
}

interior b32
_est_littera(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

interior b32
_est_identificator_initium(character c)
{
    redde _est_littera(c);
}

interior b32
_est_identificator_corpus(character c)
{
    redde _est_littera(c) || _est_cifra(c);
}

/* ==================================================
 * Helper Functions - Position Tracking
 * ================================================== */

interior b32
_finis(Arbor2Lexator* lex)
{
    redde lex->positus >= lex->mensura;
}

interior character
_aspicere(Arbor2Lexator* lex, i32 offset)
{
    i32 pos;

    pos = lex->positus + offset;
    si (pos < ZEPHYRUM || pos >= lex->mensura)
    {
        redde '\0';
    }
    redde lex->fons[pos];
}

interior vacuum
_progredi(Arbor2Lexator* lex, i32 n)
{
    i32 i;

    per (i = ZEPHYRUM; i < n && !_finis(lex); i++)
    {
        si (lex->fons[lex->positus] == '\n')
        {
            lex->linea++;
            lex->columna = I;
        }
        alioquin
        {
            lex->columna++;
        }
        lex->positus++;
    }
}

/* ==================================================
 * Helper Functions - Token Creation
 * ================================================== */

interior Arbor2Lexema*
_creare_lexema(Arbor2Lexator* lex, Arbor2LexemaGenus genus,
               i32 byte_offset, i32 longitudo, i32 linea, i32 columna)
{
    Arbor2Lexema* lexema;
    chorda temp;
    chorda* internata;

    lexema = piscina_allocare(lex->piscina, magnitudo(Arbor2Lexema));
    lexema->genus = genus;
    lexema->byte_offset = byte_offset;
    lexema->longitudo = longitudo;
    lexema->linea = linea;
    lexema->columna = columna;
    lexema->spatia_ante = NIHIL;
    lexema->spatia_post = NIHIL;
    lexema->standard = ARBOR2_STANDARD_C89;

    /* Internare valor (chorda_internare facit copiam) */
    si (longitudo > ZEPHYRUM)
    {
        temp = _chorda_ex_fonte(lex->fons + byte_offset, longitudo);
        internata = chorda_internare(lex->intern, temp);
        lexema->valor = *internata;
    }
    alioquin
    {
        lexema->valor.datum = NIHIL;
        lexema->valor.mensura = ZEPHYRUM;
    }

    redde lexema;
}

/* Create whitespace/comment lexema for spatia_ante/spatia_post.
 * Phase 2.6: All trivia are now typed lexema tokens. */
interior Arbor2Lexema*
_creare_spatium(Arbor2Lexator* lex, Arbor2LexemaGenus genus,
                i32 byte_offset, i32 longitudo, i32 linea, i32 columna)
{
    Arbor2Lexema* spatium;
    chorda temp;
    chorda* internata;

    spatium = piscina_allocare(lex->piscina, magnitudo(Arbor2Lexema));
    spatium->genus = genus;
    spatium->byte_offset = byte_offset;
    spatium->longitudo = longitudo;
    spatium->linea = linea;
    spatium->columna = columna;
    spatium->spatia_ante = NIHIL;
    spatium->spatia_post = NIHIL;
    spatium->standard = ARBOR2_STANDARD_C89;

    /* chorda_internare facit copiam */
    si (longitudo > ZEPHYRUM)
    {
        temp = _chorda_ex_fonte(lex->fons + byte_offset, longitudo);
        internata = chorda_internare(lex->intern, temp);
        spatium->valor = *internata;
    }
    alioquin
    {
        spatium->valor.datum = NIHIL;
        spatium->valor.mensura = ZEPHYRUM;
    }

    redde spatium;
}

/* ==================================================
 * Spatia Collection (Phase 2.6)
 *
 * Collects horizontal whitespace, comments, and line continuations
 * as typed Arbor2Lexema tokens instead of opaque Arbor2Trivia.
 * Newlines are NOT consumed - they become NOVA_LINEA tokens.
 * ================================================== */

interior vacuum
_colligere_spatia(Arbor2Lexator* lex)
{
    character c;
    character c2;
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    Arbor2Lexema* spatium;
    Arbor2Lexema** locus;
    i32 space_count;
    i32 tab_count;

    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);
        c2 = _aspicere(lex, I);

        /* Line continuation: backslash-newline */
        si (c == '\\' && (c2 == '\n' || (c2 == '\r' && _aspicere(lex, II) == '\n')))
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            /* Consume backslash */
            _progredi(lex, I);
            /* Consume newline (and possible CR) */
            si (c2 == '\r')
            {
                _progredi(lex, II);  /* \r\n */
            }
            alioquin
            {
                _progredi(lex, I);   /* \n */
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_CONTINUATIO,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(lex->spatia_pendentia);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Spaces - collect contiguous run */
        si (c == ' ')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;
            space_count = ZEPHYRUM;

            dum (!_finis(lex) && _aspicere(lex, ZEPHYRUM) == ' ')
            {
                _progredi(lex, I);
                space_count++;
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_SPATIA,
                initium,
                space_count,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(lex->spatia_pendentia);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Tabs - collect contiguous run */
        si (c == '\t')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;
            tab_count = ZEPHYRUM;

            dum (!_finis(lex) && _aspicere(lex, ZEPHYRUM) == '\t')
            {
                _progredi(lex, I);
                tab_count++;
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_TABULAE,
                initium,
                tab_count,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(lex->spatia_pendentia);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Block comment: slash star ... star slash */
        si (c == '/' && c2 == '*')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, II);  /* Transire slash star */

            dum (!_finis(lex))
            {
                si (_aspicere(lex, ZEPHYRUM) == '*' && _aspicere(lex, I) == '/')
                {
                    _progredi(lex, II);  /* Transire star slash */
                    frange;
                }
                _progredi(lex, I);
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_COMMENTUM_CLAUSUM,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(lex->spatia_pendentia);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Line comment: slash slash (C99) */
        si (c == '/' && c2 == '/')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, II);  /* Transire slash slash */

            /* Consume until newline (but NOT the newline itself) */
            dum (!_finis(lex) && _aspicere(lex, ZEPHYRUM) != '\n')
            {
                _progredi(lex, I);
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_COMMENTUM_LINEA,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(lex->spatia_pendentia);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Newline: \n or \r\n or bare \r - now collected as spatia (Phase 2.7) */
        si (c == '\n')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, I);

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_NOVA_LINEA,
                initium,
                I,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(lex->spatia_pendentia);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        si (c == '\r')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            si (c2 == '\n')
            {
                _progredi(lex, II);  /* \r\n */
            }
            alioquin
            {
                _progredi(lex, I);   /* bare \r */
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_NOVA_LINEA,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(lex->spatia_pendentia);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        frange;  /* Non est spatium */
    }
}

/* Collect trailing spatia (whitespace/comments after token on same line).
 * Phase 2.6: Uses typed Arbor2Lexema* tokens. */
interior vacuum
_colligere_spatia_trailing(Arbor2Lexator* lex, Arbor2Lexema* lexema)
{
    character c;
    character c2;
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    Arbor2Lexema* spatium;
    Arbor2Lexema** locus;
    Xar* trailing;
    i32 count;

    trailing = xar_creare(lex->piscina, magnitudo(Arbor2Lexema*));

    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);
        c2 = _aspicere(lex, I);

        /* Line continuation: backslash-newline */
        si (c == '\\' && (c2 == '\n' || (c2 == '\r' && _aspicere(lex, II) == '\n')))
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, I);  /* backslash */
            si (c2 == '\r')
            {
                _progredi(lex, II);  /* \r\n */
            }
            alioquin
            {
                _progredi(lex, I);   /* \n */
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_CONTINUATIO,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Spaces */
        si (c == ' ')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;
            count = ZEPHYRUM;

            dum (!_finis(lex) && _aspicere(lex, ZEPHYRUM) == ' ')
            {
                _progredi(lex, I);
                count++;
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_SPATIA,
                initium,
                count,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Tabs */
        si (c == '\t')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;
            count = ZEPHYRUM;

            dum (!_finis(lex) && _aspicere(lex, ZEPHYRUM) == '\t')
            {
                _progredi(lex, I);
                count++;
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_TABULAE,
                initium,
                count,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Block comment on same line */
        si (c == '/' && c2 == '*')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, II);

            dum (!_finis(lex))
            {
                si (_aspicere(lex, ZEPHYRUM) == '*' && _aspicere(lex, I) == '/')
                {
                    _progredi(lex, II);
                    frange;
                }
                _progredi(lex, I);
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_COMMENTUM_CLAUSUM,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Line comment on same line */
        si (c == '/' && c2 == '/')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, II);

            dum (!_finis(lex) && _aspicere(lex, ZEPHYRUM) != '\n')
            {
                _progredi(lex, I);
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_COMMENTUM_LINEA,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            perge;
        }

        /* Newline: collect into spatia_post and stop (Phase 2.7) */
        si (c == '\n')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, I);

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_NOVA_LINEA,
                initium,
                I,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            frange;  /* Stop after newline - next token gets rest as spatia_ante */
        }

        si (c == '\r')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            si (c2 == '\n')
            {
                _progredi(lex, II);  /* \r\n */
            }
            alioquin
            {
                _progredi(lex, I);   /* bare \r */
            }

            spatium = _creare_spatium(
                lex,
                ARBOR2_LEXEMA_NOVA_LINEA,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = spatium;
            }
            frange;  /* Stop after newline */
        }

        frange;  /* Non est spatium */
    }

    si (xar_numerus(trailing) > ZEPHYRUM)
    {
        lexema->spatia_post = trailing;
    }
}

/* Assign pending leading spatia to lexema.
 * Phase 2.6: Uses typed Arbor2Lexema* tokens. */
interior vacuum
_assignare_spatia_ante(Arbor2Lexator* lex, Arbor2Lexema* lexema)
{
    si (xar_numerus(lex->spatia_pendentia) > ZEPHYRUM)
    {
        lexema->spatia_ante = lex->spatia_pendentia;
        lex->spatia_pendentia = xar_creare(lex->piscina, magnitudo(Arbor2Lexema*));
    }
}

/* ==================================================
 * Keyword Lookup
 * ================================================== */

interior Arbor2LexemaGenus
_quaerere_verbum_clausum(constans character* verbum, i32 longitudo)
{
    i32 i;

    per (i = ZEPHYRUM; VERBA_CLAUSA_C[i].verbum != NIHIL; i++)
    {
        si ((i32)strlen(VERBA_CLAUSA_C[i].verbum) == longitudo &&
            memcmp(VERBA_CLAUSA_C[i].verbum, verbum, (size_t)longitudo) == ZEPHYRUM)
        {
            redde VERBA_CLAUSA_C[i].genus;
        }
    }

    redde ARBOR2_LEXEMA_IDENTIFICATOR;
}

/* ==================================================
 * Token Reading Functions
 * ================================================== */

interior Arbor2Lexema*
_legere_identificator(Arbor2Lexator* lex)
{
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    i32 longitudo;
    Arbor2LexemaGenus genus;
    Arbor2Lexema* lexema;

    initium = lex->positus;
    linea_initium = lex->linea;
    columna_initium = lex->columna;

    dum (!_finis(lex) && _est_identificator_corpus(_aspicere(lex, ZEPHYRUM)))
    {
        _progredi(lex, I);
    }

    longitudo = lex->positus - initium;
    genus = _quaerere_verbum_clausum(lex->fons + initium, longitudo);

    lexema = _creare_lexema(lex, genus, initium, longitudo,
                            linea_initium, columna_initium);

    redde lexema;
}

interior Arbor2Lexema*
_legere_numerum(Arbor2Lexator* lex)
{
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    character c;
    character c2;
    Arbor2LexemaGenus genus;
    Arbor2Lexema* lexema;
    b32 est_hex;
    b32 est_octalis;
    b32 est_fluitans;

    initium = lex->positus;
    linea_initium = lex->linea;
    columna_initium = lex->columna;
    genus = ARBOR2_LEXEMA_INTEGER;
    est_hex = FALSUM;
    est_octalis = FALSUM;
    est_fluitans = FALSUM;

    c = _aspicere(lex, ZEPHYRUM);

    /* Hex: 0x or 0X */
    si (c == '0')
    {
        c2 = _aspicere(lex, I);
        si (c2 == 'x' || c2 == 'X')
        {
            est_hex = VERUM;
            _progredi(lex, II);

            dum (!_finis(lex) && _est_cifra_hex(_aspicere(lex, ZEPHYRUM)))
            {
                _progredi(lex, I);
            }
        }
        alioquin si (_est_cifra_octalis(c2))
        {
            /* Octal: 0... */
            est_octalis = VERUM;
            _progredi(lex, I);

            dum (!_finis(lex) && _est_cifra_octalis(_aspicere(lex, ZEPHYRUM)))
            {
                _progredi(lex, I);
            }
        }
        alioquin
        {
            /* Solum 0 */
            _progredi(lex, I);
        }
    }
    alioquin
    {
        /* Decimal */
        dum (!_finis(lex) && _est_cifra(_aspicere(lex, ZEPHYRUM)))
        {
            _progredi(lex, I);
        }
    }

    /* Pars fractionalis */
    si (!est_hex && !est_octalis && _aspicere(lex, ZEPHYRUM) == '.')
    {
        c2 = _aspicere(lex, I);
        si (_est_cifra(c2))
        {
            est_fluitans = VERUM;
            _progredi(lex, I);  /* Transire . */

            dum (!_finis(lex) && _est_cifra(_aspicere(lex, ZEPHYRUM)))
            {
                _progredi(lex, I);
            }
        }
    }

    /* Exponens */
    c = _aspicere(lex, ZEPHYRUM);
    si (!est_hex && !est_octalis && (c == 'e' || c == 'E'))
    {
        est_fluitans = VERUM;
        _progredi(lex, I);

        c = _aspicere(lex, ZEPHYRUM);
        si (c == '+' || c == '-')
        {
            _progredi(lex, I);
        }

        dum (!_finis(lex) && _est_cifra(_aspicere(lex, ZEPHYRUM)))
        {
            _progredi(lex, I);
        }
    }

    /* Suffixes: L, U, UL, LU, LL, ULL, f, F, l, L (pro fluitans) */
    c = _aspicere(lex, ZEPHYRUM);
    si (est_fluitans)
    {
        si (c == 'f' || c == 'F' || c == 'l' || c == 'L')
        {
            _progredi(lex, I);
        }
        genus = ARBOR2_LEXEMA_FLOAT_LIT;
    }
    alioquin
    {
        /* Integer suffixes */
        si (c == 'u' || c == 'U')
        {
            _progredi(lex, I);
            c = _aspicere(lex, ZEPHYRUM);
            si (c == 'l' || c == 'L')
            {
                _progredi(lex, I);
                c = _aspicere(lex, ZEPHYRUM);
                si (c == 'l' || c == 'L')
                {
                    _progredi(lex, I);  /* ULL */
                }
            }
        }
        alioquin si (c == 'l' || c == 'L')
        {
            _progredi(lex, I);
            c = _aspicere(lex, ZEPHYRUM);
            si (c == 'l' || c == 'L')
            {
                _progredi(lex, I);  /* LL */
                c = _aspicere(lex, ZEPHYRUM);
            }
            si (c == 'u' || c == 'U')
            {
                _progredi(lex, I);  /* LU or LLU */
            }
        }
    }

    lexema = _creare_lexema(lex, genus, initium, lex->positus - initium,
                            linea_initium, columna_initium);

    redde lexema;
}

interior Arbor2Lexema*
_legere_chordam(Arbor2Lexator* lex)
{
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    character c;
    Arbor2Lexema* lexema;

    initium = lex->positus;
    linea_initium = lex->linea;
    columna_initium = lex->columna;

    _progredi(lex, I);  /* Transire " */

    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);

        si (c == '"')
        {
            _progredi(lex, I);
            frange;
        }
        alioquin si (c == '\\')
        {
            _progredi(lex, II);  /* Transire escape sequence */
        }
        alioquin si (c == '\n')
        {
            /* Error: unterminated string */
            frange;
        }
        alioquin
        {
            _progredi(lex, I);
        }
    }

    lexema = _creare_lexema(lex, ARBOR2_LEXEMA_STRING_LIT, initium,
                            lex->positus - initium, linea_initium, columna_initium);

    redde lexema;
}

interior Arbor2Lexema*
_legere_characterem(Arbor2Lexator* lex)
{
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    character c;
    Arbor2Lexema* lexema;

    initium = lex->positus;
    linea_initium = lex->linea;
    columna_initium = lex->columna;

    _progredi(lex, I);  /* Transire ' */

    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);

        si (c == '\'')
        {
            _progredi(lex, I);
            frange;
        }
        alioquin si (c == '\\')
        {
            _progredi(lex, II);  /* Transire escape sequence */
        }
        alioquin si (c == '\n')
        {
            /* Error: unterminated char */
            frange;
        }
        alioquin
        {
            _progredi(lex, I);
        }
    }

    lexema = _creare_lexema(lex, ARBOR2_LEXEMA_CHAR_LIT, initium,
                            lex->positus - initium, linea_initium, columna_initium);

    redde lexema;
}

/* ==================================================
 * Main Tokenization
 *
 * Phase 2.7: NOVA_LINEA now collected as spatia for roundtrip
 * preservation. Preprocessor checks spatia_post for directive
 * boundary detection.
 * ================================================== */

Arbor2Lexema*
arbor2_lexema_proximum(Arbor2Lexator* lex)
{
    character c;
    character c2;
    character c3;
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    Arbor2Lexema* lexema;
    Arbor2LexemaGenus genus;

    /* Colligere leading trivia (whitespace, comments, AND newlines) */
    _colligere_spatia(lex);

    /* EOF */
    si (_finis(lex))
    {
        lexema = _creare_lexema(lex, ARBOR2_LEXEMA_EOF, lex->positus, ZEPHYRUM,
                                lex->linea, lex->columna);
        _assignare_spatia_ante(lex, lexema);
        redde lexema;
    }

    initium = lex->positus;
    linea_initium = lex->linea;
    columna_initium = lex->columna;

    c = _aspicere(lex, ZEPHYRUM);
    c2 = _aspicere(lex, I);
    c3 = _aspicere(lex, II);

    /* Identificator vel verbum clausum */
    si (_est_identificator_initium(c))
    {
        lexema = _legere_identificator(lex);
        _assignare_spatia_ante(lex, lexema);
        _colligere_spatia_trailing(lex, lexema);
        redde lexema;
    }

    /* Numerus */
    si (_est_cifra(c))
    {
        lexema = _legere_numerum(lex);
        _assignare_spatia_ante(lex, lexema);
        _colligere_spatia_trailing(lex, lexema);
        redde lexema;
    }

    /* Chorda literalis */
    si (c == '"')
    {
        lexema = _legere_chordam(lex);
        _assignare_spatia_ante(lex, lexema);
        _colligere_spatia_trailing(lex, lexema);
        redde lexema;
    }

    /* Character literalis */
    si (c == '\'')
    {
        lexema = _legere_characterem(lex);
        _assignare_spatia_ante(lex, lexema);
        _colligere_spatia_trailing(lex, lexema);
        redde lexema;
    }

    /* Multi-character operators (3 chars) */
    si (c == '.' && c2 == '.' && c3 == '.')
    {
        _progredi(lex, III);
        lexema = _creare_lexema(lex, ARBOR2_LEXEMA_ELLIPSIS, initium, III,
                                linea_initium, columna_initium);
        _assignare_spatia_ante(lex, lexema);
        _colligere_spatia_trailing(lex, lexema);
        redde lexema;
    }

    si (c == '<' && c2 == '<' && c3 == '=')
    {
        _progredi(lex, III);
        lexema = _creare_lexema(lex, ARBOR2_LEXEMA_SHL_ASSIGN, initium, III,
                                linea_initium, columna_initium);
        _assignare_spatia_ante(lex, lexema);
        _colligere_spatia_trailing(lex, lexema);
        redde lexema;
    }

    si (c == '>' && c2 == '>' && c3 == '=')
    {
        _progredi(lex, III);
        lexema = _creare_lexema(lex, ARBOR2_LEXEMA_SHR_ASSIGN, initium, III,
                                linea_initium, columna_initium);
        _assignare_spatia_ante(lex, lexema);
        _colligere_spatia_trailing(lex, lexema);
        redde lexema;
    }

    /* Multi-character operators (2 chars) */
    si (c == '+' && c2 == '+')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_DUPLUS;
    }
    alioquin si (c == '-' && c2 == '-')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_DUMINUS;
    }
    alioquin si (c == '-' && c2 == '>')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_SAGITTA;
    }
    alioquin si (c == '&' && c2 == '&')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_DUAMPERSAND;
    }
    alioquin si (c == '|' && c2 == '|')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_DUPIPA;
    }
    alioquin si (c == '=' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_AEQUALIS;
    }
    alioquin si (c == '!' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_NON_AEQUALIS;
    }
    alioquin si (c == '<' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_MINOR_AEQ;
    }
    alioquin si (c == '>' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_MAIOR_AEQ;
    }
    alioquin si (c == '<' && c2 == '<')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_SINISTRUM;
    }
    alioquin si (c == '>' && c2 == '>')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_DEXTRUM;
    }
    alioquin si (c == '+' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_PLUS_ASSIGN;
    }
    alioquin si (c == '-' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_MINUS_ASSIGN;
    }
    alioquin si (c == '*' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_MULT_ASSIGN;
    }
    alioquin si (c == '/' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_DIV_ASSIGN;
    }
    alioquin si (c == '%' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_MOD_ASSIGN;
    }
    alioquin si (c == '&' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_AND_ASSIGN;
    }
    alioquin si (c == '|' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_OR_ASSIGN;
    }
    alioquin si (c == '^' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_XOR_ASSIGN;
    }
    alioquin si (c == '#' && c2 == '#')
    {
        _progredi(lex, II);
        genus = ARBOR2_LEXEMA_HASH_HASH;
    }
    /* Single character operators */
    alioquin si (c == '+')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_PLUS;
    }
    alioquin si (c == '-')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_MINUS;
    }
    alioquin si (c == '*')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_ASTERISCUS;
    }
    alioquin si (c == '/')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_SOLIDUS;
    }
    alioquin si (c == '%')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_PERCENTUM;
    }
    alioquin si (c == '&')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_AMPERSAND;
    }
    alioquin si (c == '|')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_PIPA;
    }
    alioquin si (c == '^')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_CARET;
    }
    alioquin si (c == '~')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_TILDE;
    }
    alioquin si (c == '!')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_EXCLAMATIO;
    }
    alioquin si (c == '<')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_MINOR;
    }
    alioquin si (c == '>')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_MAIOR;
    }
    alioquin si (c == '=')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_ASSIGNATIO;
    }
    alioquin si (c == '.')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_PUNCTUM;
    }
    alioquin si (c == ',')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_COMMA;
    }
    alioquin si (c == ':')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_COLON;
    }
    alioquin si (c == ';')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_SEMICOLON;
    }
    alioquin si (c == '?')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_QUAESTIO;
    }
    alioquin si (c == '(')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_PAREN_APERTA;
    }
    alioquin si (c == ')')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_PAREN_CLAUSA;
    }
    alioquin si (c == '[')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_BRACKET_APERTA;
    }
    alioquin si (c == ']')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_BRACKET_CLAUSA;
    }
    alioquin si (c == '{')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_BRACE_APERTA;
    }
    alioquin si (c == '}')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_BRACE_CLAUSA;
    }
    alioquin si (c == '#')
    {
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_HASH;
    }
    alioquin
    {
        /* Unknown character - error */
        _progredi(lex, I);
        genus = ARBOR2_LEXEMA_ERROR;
    }

    lexema = _creare_lexema(lex, genus, initium, lex->positus - initium,
                            linea_initium, columna_initium);
    _assignare_spatia_ante(lex, lexema);
    _colligere_spatia_trailing(lex, lexema);

    redde lexema;
}

/* ==================================================
 * Tokenize All
 * ================================================== */

Xar*
arbor2_lexema_omnia(Arbor2Lexator* lex)
{
    Xar* lexemata;
    Arbor2Lexema* lexema;
    Arbor2Lexema** locus;

    lexemata = xar_creare(lex->piscina, magnitudo(Arbor2Lexema*));

    fac
    {
        lexema = arbor2_lexema_proximum(lex);
        locus = xar_addere(lexemata);
        si (locus == NIHIL)
        {
            frange;
        }
        *locus = lexema;
    }
    dum (lexema->genus != ARBOR2_LEXEMA_EOF && lexema->genus != ARBOR2_LEXEMA_ERROR);

    redde lexemata;
}

/* ==================================================
 * API Functions
 * ================================================== */

Arbor2Lexator*
arbor2_lexator_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura)
{
    Arbor2Lexator* lex;

    lex = piscina_allocare(piscina, magnitudo(Arbor2Lexator));
    lex->fons = fons;
    lex->mensura = mensura;
    lex->positus = ZEPHYRUM;
    lex->linea = I;
    lex->columna = I;
    lex->piscina = piscina;
    lex->intern = intern;
    lex->spatia_pendentia = xar_creare(piscina, magnitudo(Arbor2Lexema*));

    redde lex;
}

constans character*
arbor2_lexema_genus_nomen(Arbor2LexemaGenus genus)
{
    si (genus >= ZEPHYRUM && genus <= ARBOR2_LEXEMA_TYPEDEF_MACRO)
    {
        redde NOMINA_GENERUM[genus];
    }
    redde "IGNOTUM";
}
