/* ==================================================
 * ARBOR LEXEMA - C89 Lexer Implementation
 *
 * Tokenizat fontem C in fluxum lexematum.
 * Conservat trivia (spatia, commenta) pro roundtrip.
 * ================================================== */

#include "arbor_lexema.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Lexator State (Internal)
 * ================================================== */

structura ArborLexator {
    constans character*       fons;
    i32                       mensura;
    i32                       positus;
    i32                       linea;
    i32                       columna;
    Piscina*                  piscina;
    InternamentumChorda*      intern;
    Xar*                      trivia_pendente;   /* Trivia collecta pro proximo lexema */
};

/* ==================================================
 * Verba Clausa (C89 Keywords)
 * ================================================== */

hic_manens structura {
    constans character*  verbum;
    ArborLexemaGenus     genus;
} VERBA_CLAUSA_C[] = {
    {"auto",      ARBOR_LEXEMA_AUTO},
    {"break",     ARBOR_LEXEMA_BREAK},
    {"case",      ARBOR_LEXEMA_CASE},
    {"char",      ARBOR_LEXEMA_CHAR},
    {"const",     ARBOR_LEXEMA_CONST},
    {"continue",  ARBOR_LEXEMA_CONTINUE},
    {"default",   ARBOR_LEXEMA_DEFAULT},
    {"do",        ARBOR_LEXEMA_DO},
    {"double",    ARBOR_LEXEMA_DOUBLE},
    {"else",      ARBOR_LEXEMA_ELSE},
    {"enum",      ARBOR_LEXEMA_ENUM},
    {"extern",    ARBOR_LEXEMA_EXTERN},
    {"float",     ARBOR_LEXEMA_FLOAT},
    {"for",       ARBOR_LEXEMA_FOR},
    {"goto",      ARBOR_LEXEMA_GOTO},
    {"if",        ARBOR_LEXEMA_IF},
    {"int",       ARBOR_LEXEMA_INT},
    {"long",      ARBOR_LEXEMA_LONG},
    {"register",  ARBOR_LEXEMA_REGISTER},
    {"return",    ARBOR_LEXEMA_RETURN},
    {"short",     ARBOR_LEXEMA_SHORT},
    {"signed",    ARBOR_LEXEMA_SIGNED},
    {"sizeof",    ARBOR_LEXEMA_SIZEOF},
    {"static",    ARBOR_LEXEMA_STATIC},
    {"struct",    ARBOR_LEXEMA_STRUCT},
    {"switch",    ARBOR_LEXEMA_SWITCH},
    {"typedef",   ARBOR_LEXEMA_TYPEDEF},
    {"union",     ARBOR_LEXEMA_UNION},
    {"unsigned",  ARBOR_LEXEMA_UNSIGNED},
    {"void",      ARBOR_LEXEMA_VOID},
    {"volatile",  ARBOR_LEXEMA_VOLATILE},
    {"while",     ARBOR_LEXEMA_WHILE},
    {NIHIL,       ARBOR_LEXEMA_ERROR}
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

    /* Special */
    "EOF",
    "ERROR"
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
_est_spatium(character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

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
_finis(ArborLexator* lex)
{
    redde lex->positus >= lex->mensura;
}

interior character
_aspicere(ArborLexator* lex, i32 offset)
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
_progredi(ArborLexator* lex, i32 n)
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

interior ArborLexema*
_creare_lexema(ArborLexator* lex, ArborLexemaGenus genus,
               i32 byte_offset, i32 longitudo, i32 linea, i32 columna)
{
    ArborLexema* lexema;
    chorda temp;
    chorda* internata;

    lexema = piscina_allocare(lex->piscina, magnitudo(ArborLexema));
    lexema->genus = genus;
    lexema->byte_offset = byte_offset;
    lexema->longitudo = longitudo;
    lexema->linea = linea;
    lexema->columna = columna;
    lexema->trivia_ante = NIHIL;
    lexema->trivia_post = NIHIL;
    lexema->standard = ARBOR_STANDARD_C89;

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

interior ArborTrivia*
_creare_trivia(ArborLexator* lex, i32 byte_offset, i32 longitudo,
               i32 linea, i32 columna, b32 est_commentum, b32 est_c99)
{
    ArborTrivia* trivia;
    chorda temp;
    chorda* internata;

    trivia = piscina_allocare(lex->piscina, magnitudo(ArborTrivia));
    trivia->linea = linea;
    trivia->columna = columna;
    trivia->est_commentum = est_commentum;
    trivia->est_c99 = est_c99;

    /* chorda_internare facit copiam */
    si (longitudo > ZEPHYRUM)
    {
        temp = _chorda_ex_fonte(lex->fons + byte_offset, longitudo);
        internata = chorda_internare(lex->intern, temp);
        trivia->valor = *internata;
    }
    alioquin
    {
        trivia->valor.datum = NIHIL;
        trivia->valor.mensura = ZEPHYRUM;
    }

    redde trivia;
}

/* ==================================================
 * Trivia Collection
 * ================================================== */

interior vacuum
_colligere_trivia(ArborLexator* lex)
{
    character c;
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    ArborTrivia* trivia;
    ArborTrivia** locus;

    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);

        /* Spatium (whitespace) */
        si (_est_spatium(c))
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            dum (!_finis(lex) && _est_spatium(_aspicere(lex, ZEPHYRUM)))
            {
                _progredi(lex, I);
            }

            trivia = _creare_trivia(
                lex,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium,
                FALSUM,  /* non est commentum */
                FALSUM   /* non est c99 */
            );
            locus = xar_addere(lex->trivia_pendente);
            si (locus != NIHIL)
            {
                *locus = trivia;
            }
        }
        /* Block comment: slash star ... star slash */
        alioquin si (c == '/' && _aspicere(lex, I) == '*')
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

            trivia = _creare_trivia(
                lex,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium,
                VERUM,   /* est commentum */
                FALSUM   /* non est c99 */
            );
            locus = xar_addere(lex->trivia_pendente);
            si (locus != NIHIL)
            {
                *locus = trivia;
            }
        }
        /* Line comment: slash slash (C99) */
        alioquin si (c == '/' && _aspicere(lex, I) == '/')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, II);  /* Transire slash slash */

            dum (!_finis(lex) && _aspicere(lex, ZEPHYRUM) != '\n')
            {
                _progredi(lex, I);
            }

            trivia = _creare_trivia(
                lex,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium,
                VERUM,   /* est commentum */
                VERUM    /* EST c99 */
            );
            locus = xar_addere(lex->trivia_pendente);
            si (locus != NIHIL)
            {
                *locus = trivia;
            }
        }
        alioquin
        {
            frange;  /* Non est trivia */
        }
    }
}

interior vacuum
_colligere_trivia_trailing(ArborLexator* lex, ArborLexema* lexema)
{
    character c;
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    ArborTrivia* trivia;
    ArborTrivia** locus;
    Xar* trailing;

    trailing = xar_creare(lex->piscina, magnitudo(ArborTrivia*));

    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);

        /* Solum spatia horizontales (non newline) */
        si (c == ' ' || c == '\t')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            dum (!_finis(lex))
            {
                c = _aspicere(lex, ZEPHYRUM);
                si (c == ' ' || c == '\t')
                {
                    _progredi(lex, I);
                }
                alioquin
                {
                    frange;
                }
            }

            trivia = _creare_trivia(
                lex,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium,
                FALSUM,
                FALSUM
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = trivia;
            }
        }
        /* Block comment on same line */
        alioquin si (c == '/' && _aspicere(lex, I) == '*')
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

            trivia = _creare_trivia(
                lex,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium,
                VERUM,
                FALSUM
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = trivia;
            }
        }
        /* Line comment on same line */
        alioquin si (c == '/' && _aspicere(lex, I) == '/')
        {
            initium = lex->positus;
            linea_initium = lex->linea;
            columna_initium = lex->columna;

            _progredi(lex, II);

            dum (!_finis(lex) && _aspicere(lex, ZEPHYRUM) != '\n')
            {
                _progredi(lex, I);
            }

            trivia = _creare_trivia(
                lex,
                initium,
                lex->positus - initium,
                linea_initium,
                columna_initium,
                VERUM,
                VERUM
            );
            locus = xar_addere(trailing);
            si (locus != NIHIL)
            {
                *locus = trivia;
            }
        }
        alioquin
        {
            frange;  /* Finire trailing trivia */
        }
    }

    si (xar_numerus(trailing) > ZEPHYRUM)
    {
        lexema->trivia_post = trailing;
    }
}

interior vacuum
_assignare_trivia_ante(ArborLexator* lex, ArborLexema* lexema)
{
    si (xar_numerus(lex->trivia_pendente) > ZEPHYRUM)
    {
        lexema->trivia_ante = lex->trivia_pendente;
        lex->trivia_pendente = xar_creare(lex->piscina, magnitudo(ArborTrivia*));
    }
}

/* ==================================================
 * Keyword Lookup
 * ================================================== */

interior ArborLexemaGenus
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

    redde ARBOR_LEXEMA_IDENTIFICATOR;
}

/* ==================================================
 * Token Reading Functions
 * ================================================== */

interior ArborLexema*
_legere_identificator(ArborLexator* lex)
{
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    i32 longitudo;
    ArborLexemaGenus genus;
    ArborLexema* lexema;

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

interior ArborLexema*
_legere_numerum(ArborLexator* lex)
{
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    character c;
    character c2;
    ArborLexemaGenus genus;
    ArborLexema* lexema;
    b32 est_hex;
    b32 est_octalis;
    b32 est_fluitans;

    initium = lex->positus;
    linea_initium = lex->linea;
    columna_initium = lex->columna;
    genus = ARBOR_LEXEMA_INTEGER;
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
        genus = ARBOR_LEXEMA_FLOAT_LIT;
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

interior ArborLexema*
_legere_chordam(ArborLexator* lex)
{
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    character c;
    ArborLexema* lexema;

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

    lexema = _creare_lexema(lex, ARBOR_LEXEMA_STRING_LIT, initium,
                            lex->positus - initium, linea_initium, columna_initium);

    redde lexema;
}

interior ArborLexema*
_legere_characterem(ArborLexator* lex)
{
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    character c;
    ArborLexema* lexema;

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

    lexema = _creare_lexema(lex, ARBOR_LEXEMA_CHAR_LIT, initium,
                            lex->positus - initium, linea_initium, columna_initium);

    redde lexema;
}

/* ==================================================
 * Main Tokenization
 * ================================================== */

ArborLexema*
arbor_lexema_proximum(ArborLexator* lex)
{
    character c;
    character c2;
    character c3;
    i32 initium;
    i32 linea_initium;
    i32 columna_initium;
    ArborLexema* lexema;
    ArborLexemaGenus genus;

    /* Colligere leading trivia */
    _colligere_trivia(lex);

    /* EOF */
    si (_finis(lex))
    {
        lexema = _creare_lexema(lex, ARBOR_LEXEMA_EOF, lex->positus, ZEPHYRUM,
                                lex->linea, lex->columna);
        _assignare_trivia_ante(lex, lexema);
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
        _assignare_trivia_ante(lex, lexema);
        _colligere_trivia_trailing(lex, lexema);
        redde lexema;
    }

    /* Numerus */
    si (_est_cifra(c))
    {
        lexema = _legere_numerum(lex);
        _assignare_trivia_ante(lex, lexema);
        _colligere_trivia_trailing(lex, lexema);
        redde lexema;
    }

    /* Chorda literalis */
    si (c == '"')
    {
        lexema = _legere_chordam(lex);
        _assignare_trivia_ante(lex, lexema);
        _colligere_trivia_trailing(lex, lexema);
        redde lexema;
    }

    /* Character literalis */
    si (c == '\'')
    {
        lexema = _legere_characterem(lex);
        _assignare_trivia_ante(lex, lexema);
        _colligere_trivia_trailing(lex, lexema);
        redde lexema;
    }

    /* Multi-character operators (3 chars) */
    si (c == '.' && c2 == '.' && c3 == '.')
    {
        _progredi(lex, III);
        lexema = _creare_lexema(lex, ARBOR_LEXEMA_ELLIPSIS, initium, III,
                                linea_initium, columna_initium);
        _assignare_trivia_ante(lex, lexema);
        _colligere_trivia_trailing(lex, lexema);
        redde lexema;
    }

    si (c == '<' && c2 == '<' && c3 == '=')
    {
        _progredi(lex, III);
        lexema = _creare_lexema(lex, ARBOR_LEXEMA_SHL_ASSIGN, initium, III,
                                linea_initium, columna_initium);
        _assignare_trivia_ante(lex, lexema);
        _colligere_trivia_trailing(lex, lexema);
        redde lexema;
    }

    si (c == '>' && c2 == '>' && c3 == '=')
    {
        _progredi(lex, III);
        lexema = _creare_lexema(lex, ARBOR_LEXEMA_SHR_ASSIGN, initium, III,
                                linea_initium, columna_initium);
        _assignare_trivia_ante(lex, lexema);
        _colligere_trivia_trailing(lex, lexema);
        redde lexema;
    }

    /* Multi-character operators (2 chars) */
    si (c == '+' && c2 == '+')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_DUPLUS;
    }
    alioquin si (c == '-' && c2 == '-')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_DUMINUS;
    }
    alioquin si (c == '-' && c2 == '>')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_SAGITTA;
    }
    alioquin si (c == '&' && c2 == '&')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_DUAMPERSAND;
    }
    alioquin si (c == '|' && c2 == '|')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_DUPIPA;
    }
    alioquin si (c == '=' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_AEQUALIS;
    }
    alioquin si (c == '!' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_NON_AEQUALIS;
    }
    alioquin si (c == '<' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_MINOR_AEQ;
    }
    alioquin si (c == '>' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_MAIOR_AEQ;
    }
    alioquin si (c == '<' && c2 == '<')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_SINISTRUM;
    }
    alioquin si (c == '>' && c2 == '>')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_DEXTRUM;
    }
    alioquin si (c == '+' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_PLUS_ASSIGN;
    }
    alioquin si (c == '-' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_MINUS_ASSIGN;
    }
    alioquin si (c == '*' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_MULT_ASSIGN;
    }
    alioquin si (c == '/' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_DIV_ASSIGN;
    }
    alioquin si (c == '%' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_MOD_ASSIGN;
    }
    alioquin si (c == '&' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_AND_ASSIGN;
    }
    alioquin si (c == '|' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_OR_ASSIGN;
    }
    alioquin si (c == '^' && c2 == '=')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_XOR_ASSIGN;
    }
    alioquin si (c == '#' && c2 == '#')
    {
        _progredi(lex, II);
        genus = ARBOR_LEXEMA_HASH_HASH;
    }
    /* Single character operators */
    alioquin si (c == '+')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_PLUS;
    }
    alioquin si (c == '-')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_MINUS;
    }
    alioquin si (c == '*')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_ASTERISCUS;
    }
    alioquin si (c == '/')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_SOLIDUS;
    }
    alioquin si (c == '%')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_PERCENTUM;
    }
    alioquin si (c == '&')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_AMPERSAND;
    }
    alioquin si (c == '|')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_PIPA;
    }
    alioquin si (c == '^')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_CARET;
    }
    alioquin si (c == '~')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_TILDE;
    }
    alioquin si (c == '!')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_EXCLAMATIO;
    }
    alioquin si (c == '<')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_MINOR;
    }
    alioquin si (c == '>')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_MAIOR;
    }
    alioquin si (c == '=')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_ASSIGNATIO;
    }
    alioquin si (c == '.')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_PUNCTUM;
    }
    alioquin si (c == ',')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_COMMA;
    }
    alioquin si (c == ':')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_COLON;
    }
    alioquin si (c == ';')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_SEMICOLON;
    }
    alioquin si (c == '?')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_QUAESTIO;
    }
    alioquin si (c == '(')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_PAREN_APERTA;
    }
    alioquin si (c == ')')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_PAREN_CLAUSA;
    }
    alioquin si (c == '[')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_BRACKET_APERTA;
    }
    alioquin si (c == ']')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_BRACKET_CLAUSA;
    }
    alioquin si (c == '{')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_BRACE_APERTA;
    }
    alioquin si (c == '}')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_BRACE_CLAUSA;
    }
    alioquin si (c == '#')
    {
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_HASH;
    }
    alioquin
    {
        /* Unknown character - error */
        _progredi(lex, I);
        genus = ARBOR_LEXEMA_ERROR;
    }

    lexema = _creare_lexema(lex, genus, initium, lex->positus - initium,
                            linea_initium, columna_initium);
    _assignare_trivia_ante(lex, lexema);
    _colligere_trivia_trailing(lex, lexema);

    redde lexema;
}

/* ==================================================
 * Peek Function
 * ================================================== */

ArborLexema*
arbor_lexema_prospicere(ArborLexator* lex, i32 ahead)
{
    /* TODO: Implementare lookahead buffer */
    (vacuum)lex;
    (vacuum)ahead;
    redde NIHIL;
}

/* ==================================================
 * Tokenize All
 * ================================================== */

Xar*
arbor_lexema_omnia(ArborLexator* lex)
{
    Xar* lexemata;
    ArborLexema* lexema;
    ArborLexema** locus;

    lexemata = xar_creare(lex->piscina, magnitudo(ArborLexema*));

    fac
    {
        lexema = arbor_lexema_proximum(lex);
        locus = xar_addere(lexemata);
        si (locus == NIHIL)
        {
            frange;
        }
        *locus = lexema;
    }
    dum (lexema->genus != ARBOR_LEXEMA_EOF && lexema->genus != ARBOR_LEXEMA_ERROR);

    redde lexemata;
}

/* ==================================================
 * API Functions
 * ================================================== */

ArborLexator*
arbor_lexator_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura)
{
    ArborLexator* lex;

    lex = piscina_allocare(piscina, magnitudo(ArborLexator));
    lex->fons = fons;
    lex->mensura = mensura;
    lex->positus = ZEPHYRUM;
    lex->linea = I;
    lex->columna = I;
    lex->piscina = piscina;
    lex->intern = intern;
    lex->trivia_pendente = xar_creare(piscina, magnitudo(ArborTrivia*));

    redde lex;
}

constans character*
arbor_lexema_genus_nomen(ArborLexemaGenus genus)
{
    si (genus >= ZEPHYRUM && genus <= ARBOR_LEXEMA_ERROR)
    {
        redde NOMINA_GENERUM[genus];
    }
    redde "IGNOTUM";
}
