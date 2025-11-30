/* sputnik_lexema.c - Lexer pro Sputnik
 *
 * Tokenizator pro lingua Sputnik.
 */

#include "sputnik_lexema.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Verba Clausa (Keywords)
 * ================================================== */

hic_manens structura {
    constans character*  verbum;
    SputnikLexemaGenus   genus;
} VERBA_CLAUSA[] = {
    {"sit",      SPUTNIK_LEXEMA_LET},
    {"constans", SPUTNIK_LEXEMA_CONST},
    {"si",       SPUTNIK_LEXEMA_SI},
    {"alioquin", SPUTNIK_LEXEMA_ALIOQUIN},
    {"dum",      SPUTNIK_LEXEMA_DUM},
    {"per",      SPUTNIK_LEXEMA_PER},
    {"redde",    SPUTNIK_LEXEMA_REDDE},
    {"functio",  SPUTNIK_LEXEMA_FUNCTIO},
    {"verum",    SPUTNIK_LEXEMA_VERUM},
    {"falsum",   SPUTNIK_LEXEMA_FALSUM},
    {"nihil",    SPUTNIK_LEXEMA_NIHIL},
    {NIHIL,      ZEPHYRUM}
};


/* ==================================================
 * Adiutores Interni - Character Classification
 * ================================================== */

interior b32
_est_littera(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

interior b32
_est_cifra(character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_spatium(character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior b32
_est_identificator_initium(character c)
{
    redde _est_littera(c) || c == '_';
}

interior b32
_est_identificator_character(character c)
{
    redde _est_littera(c) || _est_cifra(c) || c == '_';
}


/* ==================================================
 * Adiutores Interni - Lexator Navigation
 * ================================================== */

interior b32
_finis(SputnikLexator* lex)
{
    redde lex->positus >= lex->fons.mensura;
}

interior character
_aspicere(SputnikLexator* lex, i32 offset)
{
    i32 pos;
    pos = lex->positus + offset;
    si (pos < ZEPHYRUM || pos >= lex->fons.mensura)
    {
        redde '\0';
    }
    redde (character)lex->fons.datum[pos];
}

interior vacuum
_progredi(SputnikLexator* lex, i32 numerus)
{
    i32 i;
    per (i = ZEPHYRUM; i < numerus && lex->positus < lex->fons.mensura; i++)
    {
        si ((character)lex->fons.datum[lex->positus] == '\n')
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

interior vacuum
_praeterire_spatium(SputnikLexator* lex)
{
    dum (!_finis(lex) && _est_spatium(_aspicere(lex, ZEPHYRUM)))
    {
        _progredi(lex, I);
    }
}


/* ==================================================
 * Adiutores Interni - Keyword Lookup
 * ================================================== */

interior SputnikLexemaGenus
_quaerere_verbum_clausum(chorda valor)
{
    i32 i;
    i32 len;

    per (i = ZEPHYRUM; VERBA_CLAUSA[i].verbum != NIHIL; i++)
    {
        len = (i32)strlen(VERBA_CLAUSA[i].verbum);
        si (valor.mensura == len)
        {
            si (memcmp(valor.datum, VERBA_CLAUSA[i].verbum, (size_t)len) == ZEPHYRUM)
            {
                redde VERBA_CLAUSA[i].genus;
            }
        }
    }

    redde ZEPHYRUM;  /* Non inventum */
}


/* ==================================================
 * Adiutores Interni - Token Builders
 * ================================================== */

interior SputnikLexema
_creare_lexema(SputnikLexator* lex, SputnikLexemaGenus genus,
               i32 initium, i32 finis, i32 linea, i32 columna)
{
    SputnikLexema lexema;

    lexema.genus = genus;
    lexema.valor.datum = lex->fons.datum + initium;
    lexema.valor.mensura = finis - initium;
    lexema.linea = linea;
    lexema.columna = columna;
    lexema.offset_initium = initium;
    lexema.offset_finis = finis;

    redde lexema;
}

interior SputnikLexema
_creare_lexema_simplex(SputnikLexator* lex, SputnikLexemaGenus genus, i32 mensura)
{
    SputnikLexema lexema;
    i32 initium;
    i32 linea;
    i32 columna;

    initium = lex->positus;
    linea = lex->linea;
    columna = lex->columna;

    _progredi(lex, mensura);

    lexema = _creare_lexema(lex, genus, initium, lex->positus, linea, columna);

    redde lexema;
}


/* ==================================================
 * Legere Identificatorem
 * ================================================== */

interior SputnikLexema
_legere_identificator(SputnikLexator* lex)
{
    SputnikLexema lexema;
    SputnikLexemaGenus genus;
    i32 initium;
    i32 linea;
    i32 columna;
    character c;
    character post;

    initium = lex->positus;
    linea = lex->linea;
    columna = lex->columna;

    /* Primus character iam validatus */
    _progredi(lex, I);

    /* Legere reliquos characteres */
    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);

        si (_est_identificator_character(c))
        {
            _progredi(lex, I);
        }
        alioquin si (c == '-')
        {
            /* Hyphen: solum consume si sequitur identifier character */
            post = _aspicere(lex, I);
            si (_est_identificator_character(post))
            {
                _progredi(lex, I);
            }
            alioquin
            {
                frange;
            }
        }
        alioquin
        {
            frange;
        }
    }

    /* Creare lexema */
    lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_IDENTIFICATOR,
                            initium, lex->positus, linea, columna);

    /* Verificare si verbum clausum */
    genus = _quaerere_verbum_clausum(lexema.valor);
    si (genus != ZEPHYRUM)
    {
        lexema.genus = genus;
    }

    redde lexema;
}


/* ==================================================
 * Legere Numerum
 * ================================================== */

interior SputnikLexema
_legere_numerum(SputnikLexator* lex)
{
    SputnikLexema lexema;
    i32 initium;
    i32 linea;
    i32 columna;
    character c;
    character post;

    initium = lex->positus;
    linea = lex->linea;
    columna = lex->columna;

    /* Legere digitos cum underscore */
    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);

        si (_est_cifra(c))
        {
            _progredi(lex, I);
        }
        alioquin si (c == '_')
        {
            /* Underscore inter digitos permissus */
            post = _aspicere(lex, I);
            si (_est_cifra(post))
            {
                _progredi(lex, I);
            }
            alioquin
            {
                frange;
            }
        }
        alioquin si (c == '.')
        {
            /* Punctum decimale */
            post = _aspicere(lex, I);
            si (_est_cifra(post))
            {
                _progredi(lex, I);  /* Consume . */
                /* Legere digitos post punctum */
                dum (!_finis(lex))
                {
                    c = _aspicere(lex, ZEPHYRUM);
                    si (_est_cifra(c))
                    {
                        _progredi(lex, I);
                    }
                    alioquin si (c == '_')
                    {
                        post = _aspicere(lex, I);
                        si (_est_cifra(post))
                        {
                            _progredi(lex, I);
                        }
                        alioquin
                        {
                            frange;
                        }
                    }
                    alioquin
                    {
                        frange;
                    }
                }
                frange;  /* Exit outer loop */
            }
            alioquin
            {
                frange;  /* Not decimal, stop */
            }
        }
        alioquin
        {
            frange;
        }
    }

    lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_NUMERUS,
                            initium, lex->positus, linea, columna);

    redde lexema;
}


/* ==================================================
 * Legere Chordam (String)
 * ================================================== */

interior SputnikLexema
_legere_chordam(SputnikLexator* lex)
{
    SputnikLexema lexema;
    i32 initium;
    i32 linea;
    i32 columna;
    character quota;
    character c;

    initium = lex->positus;
    linea = lex->linea;
    columna = lex->columna;

    /* Memorare quota character */
    quota = _aspicere(lex, ZEPHYRUM);
    _progredi(lex, I);

    /* Legere usque ad quota claudens */
    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);

        si (c == quota)
        {
            _progredi(lex, I);
            lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_CHORDA,
                                    initium, lex->positus, linea, columna);
            redde lexema;
        }
        alioquin si (c == '\\')
        {
            /* Skip escape sequence (simplex) */
            _progredi(lex, II);
        }
        alioquin si (c == '\n')
        {
            /* Unterminated string at newline */
            frange;
        }
        alioquin
        {
            _progredi(lex, I);
        }
    }

    /* Error: chorda non clausa */
    lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_ERROR,
                            initium, lex->positus, linea, columna);
    redde lexema;
}


/* ==================================================
 * Legere Signum (Tag)
 * ================================================== */

interior SputnikLexema
_legere_signum(SputnikLexator* lex)
{
    SputnikLexema lexema;
    i32 initium;
    i32 linea;
    i32 columna;
    character c;
    character post;

    initium = lex->positus;
    linea = lex->linea;
    columna = lex->columna;

    /* Skip # */
    _progredi(lex, I);

    /* Verificare initium validum */
    c = _aspicere(lex, ZEPHYRUM);
    si (!_est_identificator_initium(c))
    {
        /* Error: signum malum */
        lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_ERROR,
                                initium, lex->positus, linea, columna);
        redde lexema;
    }

    /* Legere corpus signi */
    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);

        si (_est_identificator_character(c))
        {
            _progredi(lex, I);
        }
        alioquin si (c == '-')
        {
            /* Hyphen permissa in signis */
            post = _aspicere(lex, I);
            si (_est_identificator_character(post))
            {
                _progredi(lex, I);
            }
            alioquin
            {
                frange;
            }
        }
        alioquin si (c == ':' && _aspicere(lex, I) == ':')
        {
            /* Namespace separator :: */
            _progredi(lex, II);
        }
        alioquin
        {
            frange;
        }
    }

    lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_SIGNUM,
                            initium, lex->positus, linea, columna);
    redde lexema;
}


/* ==================================================
 * Legere Commentum
 * ================================================== */

interior SputnikLexema
_legere_commentum_linea(SputnikLexator* lex)
{
    SputnikLexema lexema;
    i32 initium;
    i32 linea;
    i32 columna;
    character c;

    initium = lex->positus;
    linea = lex->linea;
    columna = lex->columna;

    /* Skip // */
    _progredi(lex, II);

    /* Legere usque ad finem lineae */
    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);
        si (c == '\n')
        {
            frange;
        }
        _progredi(lex, I);
    }

    lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_COMMENTUM,
                            initium, lex->positus, linea, columna);
    redde lexema;
}

interior SputnikLexema
_legere_commentum_bloc(SputnikLexator* lex)
{
    SputnikLexema lexema;
    i32 initium;
    i32 linea;
    i32 columna;
    character c;

    initium = lex->positus;
    linea = lex->linea;
    columna = lex->columna;

    /* Skip opening block comment */
    _progredi(lex, II);

    /* Legere usque ad */
    dum (!_finis(lex))
    {
        c = _aspicere(lex, ZEPHYRUM);
        si (c == '*' && _aspicere(lex, I) == '/')
        {
            _progredi(lex, II);
            lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_COMMENTUM,
                                    initium, lex->positus, linea, columna);
            redde lexema;
        }
        _progredi(lex, I);
    }

    /* Error: commentum non clausum (sed redde ut commentum) */
    lexema = _creare_lexema(lex, SPUTNIK_LEXEMA_COMMENTUM,
                            initium, lex->positus, linea, columna);
    redde lexema;
}


/* ==================================================
 * Creatio
 * ================================================== */

SputnikLexator*
sputnik_lexator_creare(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SputnikLexator* lex;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    lex = piscina_allocare(piscina, magnitudo(SputnikLexator));
    si (lex == NIHIL)
    {
        redde NIHIL;
    }

    lex->fons = fons;
    lex->positus = ZEPHYRUM;
    lex->linea = I;
    lex->columna = I;
    lex->piscina = piscina;
    lex->intern = intern;

    redde lex;
}

SputnikLexator*
sputnik_lexator_creare_ex_literis(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    chorda ch;

    ch = chorda_ex_literis(fons, piscina);
    redde sputnik_lexator_creare(ch, piscina, intern);
}


/* ==================================================
 * Lexicatio Singularis
 * ================================================== */

SputnikLexema
sputnik_lexator_legere(SputnikLexator* lex)
{
    SputnikLexema lexema;
    character c;
    character c2;
    character c3;

    /* Skip spatium */
    _praeterire_spatium(lex);

    /* Verificare finem */
    si (_finis(lex))
    {
        lexema.genus = SPUTNIK_LEXEMA_FINIS;
        lexema.valor.datum = NIHIL;
        lexema.valor.mensura = ZEPHYRUM;
        lexema.linea = lex->linea;
        lexema.columna = lex->columna;
        lexema.offset_initium = lex->positus;
        lexema.offset_finis = lex->positus;
        redde lexema;
    }

    c = _aspicere(lex, ZEPHYRUM);
    c2 = _aspicere(lex, I);
    c3 = _aspicere(lex, II);

    /* Identificator */
    si (_est_identificator_initium(c))
    {
        redde _legere_identificator(lex);
    }

    /* Numerus */
    si (_est_cifra(c))
    {
        redde _legere_numerum(lex);
    }

    /* Chorda */
    si (c == '"' || c == '\'')
    {
        redde _legere_chordam(lex);
    }

    /* Signum (Tag) */
    si (c == '#')
    {
        redde _legere_signum(lex);
    }

    /* Commentum */
    si (c == '/' && c2 == '/')
    {
        redde _legere_commentum_linea(lex);
    }
    si (c == '/' && c2 == '*')
    {
        redde _legere_commentum_bloc(lex);
    }

    /* Operatores multi-character (maximal munch) */

    /* === */
    si (c == '=' && c2 == '=' && c3 == '=')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_AEQUALIS_TRIA, III);
    }

    /* !== */
    si (c == '!' && c2 == '=' && c3 == '=')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_STRICTE_NON_AEQ, III);
    }

    /* ... */
    si (c == '.' && c2 == '.' && c3 == '.')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_PUNCTUM_TRIPLEX, III);
    }

    /* == */
    si (c == '=' && c2 == '=')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_AEQUALIS_DUO, II);
    }

    /* != */
    si (c == '!' && c2 == '=')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_NON_AEQUALIS, II);
    }

    /* <= */
    si (c == '<' && c2 == '=')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_MINOR_AUT, II);
    }

    /* >= */
    si (c == '>' && c2 == '=')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_MAIOR_AUT, II);
    }

    /* && */
    si (c == '&' && c2 == '&')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_ET, II);
    }

    /* || */
    si (c == '|' && c2 == '|')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_VEL, II);
    }

    /* => */
    si (c == '=' && c2 == '>')
    {
        redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_SAGITTA, II);
    }

    /* Single character tokens */
    commutatio (c)
    {
        casus '+':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_PLUS, I);
        casus '-':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_MINUS, I);
        casus '*':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_ASTERISCUS, I);
        casus '/':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_DIVISIO, I);
        casus '%':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_MODULUS, I);
        casus '=':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_AEQUALIS, I);
        casus '<':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_MINOR, I);
        casus '>':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_MAIOR, I);
        casus '!':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_NON, I);
        casus '(':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_PARENTHESIS_A, I);
        casus ')':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_PARENTHESIS_C, I);
        casus '{':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_BRACCHIUM_A, I);
        casus '}':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_BRACCHIUM_C, I);
        casus '[':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_QUADRATUM_A, I);
        casus ']':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_QUADRATUM_C, I);
        casus ';':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_SEMICOLON, I);
        casus ',':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_COMMA, I);
        casus '.':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_PUNCTUM, I);
        casus ':':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_COLON, I);
        casus '?':
            redde _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_INTERROGATIO, I);
        ordinarius:
            frange;
    }

    /* Character ignotus */
    lexema = _creare_lexema_simplex(lex, SPUTNIK_LEXEMA_ERROR, I);
    redde lexema;
}

SputnikLexema
sputnik_lexator_aspicere(SputnikLexator* lex)
{
    SputnikLexema lexema;
    i32 positus_salvatus;
    i32 linea_salvata;
    i32 columna_salvata;

    /* Salvare statum */
    positus_salvatus = lex->positus;
    linea_salvata = lex->linea;
    columna_salvata = lex->columna;

    /* Legere */
    lexema = sputnik_lexator_legere(lex);

    /* Restaurare statum */
    lex->positus = positus_salvatus;
    lex->linea = linea_salvata;
    lex->columna = columna_salvata;

    redde lexema;
}

b32
sputnik_lexator_finis(SputnikLexator* lex)
{
    SputnikLexema lexema;
    lexema = sputnik_lexator_aspicere(lex);
    redde lexema.genus == SPUTNIK_LEXEMA_FINIS;
}


/* ==================================================
 * Lexicatio Totalis
 * ================================================== */

SputnikLexatorResultus
sputnik_lexicare(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SputnikLexatorResultus resultus;
    SputnikLexator* lex;
    SputnikLexema lexema;
    SputnikLexema* ptr;

    resultus.successus = FALSUM;
    resultus.lexemata = NIHIL;
    resultus.error_nuntius.datum = NIHIL;
    resultus.error_nuntius.mensura = ZEPHYRUM;
    resultus.error_linea = ZEPHYRUM;
    resultus.error_columna = ZEPHYRUM;

    lex = sputnik_lexator_creare(fons, piscina, intern);
    si (lex == NIHIL)
    {
        redde resultus;
    }

    resultus.lexemata = xar_creare(piscina, magnitudo(SputnikLexema));
    si (resultus.lexemata == NIHIL)
    {
        redde resultus;
    }

    dum (VERUM)
    {
        lexema = sputnik_lexator_legere(lex);

        si (lexema.genus == SPUTNIK_LEXEMA_ERROR)
        {
            resultus.error_linea = lexema.linea;
            resultus.error_columna = lexema.columna;
            redde resultus;
        }

        ptr = xar_addere(resultus.lexemata);
        *ptr = lexema;

        si (lexema.genus == SPUTNIK_LEXEMA_FINIS)
        {
            frange;
        }
    }

    resultus.successus = VERUM;
    redde resultus;
}

SputnikLexatorResultus
sputnik_lexicare_ex_literis(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    chorda ch;
    ch = chorda_ex_literis(fons, piscina);
    redde sputnik_lexicare(ch, piscina, intern);
}


/* ==================================================
 * Utilitas
 * ================================================== */

constans character*
sputnik_lexema_genus_nomen(SputnikLexemaGenus genus)
{
    commutatio (genus)
    {
        casus SPUTNIK_LEXEMA_FINIS:           redde "FINIS";
        casus SPUTNIK_LEXEMA_ERROR:           redde "ERROR";
        casus SPUTNIK_LEXEMA_NUMERUS:         redde "NUMERUS";
        casus SPUTNIK_LEXEMA_CHORDA:          redde "CHORDA";
        casus SPUTNIK_LEXEMA_VERUM:           redde "VERUM";
        casus SPUTNIK_LEXEMA_FALSUM:          redde "FALSUM";
        casus SPUTNIK_LEXEMA_NIHIL:           redde "NIHIL";
        casus SPUTNIK_LEXEMA_IDENTIFICATOR:   redde "IDENTIFICATOR";
        casus SPUTNIK_LEXEMA_SIGNUM:          redde "SIGNUM";
        casus SPUTNIK_LEXEMA_LET:             redde "LET";
        casus SPUTNIK_LEXEMA_CONST:           redde "CONST";
        casus SPUTNIK_LEXEMA_SI:              redde "SI";
        casus SPUTNIK_LEXEMA_ALIOQUIN:        redde "ALIOQUIN";
        casus SPUTNIK_LEXEMA_DUM:             redde "DUM";
        casus SPUTNIK_LEXEMA_PER:             redde "PER";
        casus SPUTNIK_LEXEMA_REDDE:           redde "REDDE";
        casus SPUTNIK_LEXEMA_FUNCTIO:         redde "FUNCTIO";
        casus SPUTNIK_LEXEMA_PLUS:            redde "PLUS";
        casus SPUTNIK_LEXEMA_MINUS:           redde "MINUS";
        casus SPUTNIK_LEXEMA_ASTERISCUS:      redde "ASTERISCUS";
        casus SPUTNIK_LEXEMA_DIVISIO:         redde "DIVISIO";
        casus SPUTNIK_LEXEMA_MODULUS:         redde "MODULUS";
        casus SPUTNIK_LEXEMA_AEQUALIS:        redde "AEQUALIS";
        casus SPUTNIK_LEXEMA_AEQUALIS_DUO:    redde "AEQUALIS_DUO";
        casus SPUTNIK_LEXEMA_AEQUALIS_TRIA:   redde "AEQUALIS_TRIA";
        casus SPUTNIK_LEXEMA_NON_AEQUALIS:    redde "NON_AEQUALIS";
        casus SPUTNIK_LEXEMA_STRICTE_NON_AEQ: redde "STRICTE_NON_AEQ";
        casus SPUTNIK_LEXEMA_MINOR:           redde "MINOR";
        casus SPUTNIK_LEXEMA_MAIOR:           redde "MAIOR";
        casus SPUTNIK_LEXEMA_MINOR_AUT:       redde "MINOR_AUT";
        casus SPUTNIK_LEXEMA_MAIOR_AUT:       redde "MAIOR_AUT";
        casus SPUTNIK_LEXEMA_ET:              redde "ET";
        casus SPUTNIK_LEXEMA_VEL:             redde "VEL";
        casus SPUTNIK_LEXEMA_NON:             redde "NON";
        casus SPUTNIK_LEXEMA_SAGITTA:         redde "SAGITTA";
        casus SPUTNIK_LEXEMA_PUNCTUM_TRIPLEX: redde "PUNCTUM_TRIPLEX";
        casus SPUTNIK_LEXEMA_PARENTHESIS_A:   redde "PARENTHESIS_A";
        casus SPUTNIK_LEXEMA_PARENTHESIS_C:   redde "PARENTHESIS_C";
        casus SPUTNIK_LEXEMA_BRACCHIUM_A:     redde "BRACCHIUM_A";
        casus SPUTNIK_LEXEMA_BRACCHIUM_C:     redde "BRACCHIUM_C";
        casus SPUTNIK_LEXEMA_QUADRATUM_A:     redde "QUADRATUM_A";
        casus SPUTNIK_LEXEMA_QUADRATUM_C:     redde "QUADRATUM_C";
        casus SPUTNIK_LEXEMA_SEMICOLON:       redde "SEMICOLON";
        casus SPUTNIK_LEXEMA_COMMA:           redde "COMMA";
        casus SPUTNIK_LEXEMA_PUNCTUM:         redde "PUNCTUM";
        casus SPUTNIK_LEXEMA_COLON:           redde "COLON";
        casus SPUTNIK_LEXEMA_INTERROGATIO:   redde "INTERROGATIO";
        casus SPUTNIK_LEXEMA_COMMENTUM:       redde "COMMENTUM";
        ordinarius:                           redde "IGNOTUS";
    }
}

vacuum
sputnik_lexema_imprimere(SputnikLexema* lexema)
{
    imprimere("[%s", sputnik_lexema_genus_nomen(lexema->genus));
    si (lexema->valor.mensura > ZEPHYRUM)
    {
        imprimere(" \"%.*s\"", lexema->valor.mensura, lexema->valor.datum);
    }
    imprimere(" @%d:%d]\n", lexema->linea, lexema->columna);
}

vacuum
sputnik_lexemata_imprimere(Xar* lexemata)
{
    i32 i;
    i32 numerus;
    SputnikLexema* lexema;

    si (lexemata == NIHIL)
    {
        imprimere("(nihil)\n");
        redde;
    }

    numerus = xar_numerus(lexemata);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        lexema = xar_obtinere(lexemata, i);
        sputnik_lexema_imprimere(lexema);
    }
}
