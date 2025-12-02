#include "coloratio.h"
#include "registrum_commandi.h"
#include <string.h>


/* ==================================================
 * Helper Functions
 * ================================================== */

/* Verificare si character est pars verbi (identifier) */
hic_manens b32
_est_character_verbi(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_');
}

/* Verificare si character potest initiare nomen tag/attributum */
hic_manens b32
_est_nomen_initium(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_') || (c == ':');
}

/* Verificare si character potest esse in nomine tag/attributum */
hic_manens b32
_est_nomen_character(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_') || (c == ':') ||
           (c == '-') || (c == '.');
}

/* Verificare si character potest esse in valore nudo (bare value) */
hic_manens b32
_est_valor_nudus_character(character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           (c == '_') || (c == '-');
}

/* Verificare si character est spatium */
hic_manens b32
_est_spatium(character c)
{
    redde (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}


/* ==================================================
 * Creation
 * ================================================== */

Coloratio*
coloratio_creare(
    Piscina* piscina,
    i32 latitudo,
    i32 altitudo)
{
    Coloratio* coloratio;
    memoriae_index indices_size;
    memoriae_index status_size;

    si (!piscina || latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM)
    {
        redde NIHIL;
    }

    /* Allocare structuram */
    coloratio = (Coloratio*)piscina_allocare(piscina, magnitudo(Coloratio));
    si (!coloratio)
    {
        redde NIHIL;
    }

    coloratio->piscina = piscina;
    coloratio->latitudo = latitudo;
    coloratio->altitudo = altitudo;
    coloratio->regulae = ZEPHYRUM;
    coloratio->reg_commandi = NIHIL;

    /* Allocare indices array */
    indices_size = (memoriae_index)(latitudo * altitudo) * magnitudo(i8);
    coloratio->indices = (i8*)piscina_allocare(piscina, indices_size);
    si (!coloratio->indices)
    {
        redde NIHIL;
    }

    /* Allocare status_lineae array */
    status_size = (memoriae_index)altitudo * magnitudo(i8);
    coloratio->status_lineae = (i8*)piscina_allocare(piscina, status_size);
    si (!coloratio->status_lineae)
    {
        redde NIHIL;
    }

    /* Initiare ad zero */
    memset(coloratio->indices, COLORATIO_DEFALTA, (size_t)indices_size);
    memset(coloratio->status_lineae, STATUS_LINEA_NORMALIS, (size_t)status_size);

    redde coloratio;
}

Coloratio*
coloratio_creare_ex_tabula(
    Piscina* piscina,
    constans TabulaCharacterum* tabula)
{
    si (!piscina || !tabula)
    {
        redde NIHIL;
    }

    redde coloratio_creare(piscina, tabula->latitudo, tabula->altitudo);
}


/* ==================================================
 * Configuration
 * ================================================== */

vacuum
coloratio_ponere_regulas(
    Coloratio* coloratio,
    i32 regulae)
{
    si (coloratio)
    {
        coloratio->regulae = regulae;
    }
}

vacuum
coloratio_addere_regulam(
    Coloratio* coloratio,
    i32 regula)
{
    si (coloratio)
    {
        coloratio->regulae |= regula;
    }
}

vacuum
coloratio_removere_regulam(
    Coloratio* coloratio,
    i32 regula)
{
    si (coloratio)
    {
        coloratio->regulae &= ~regula;
    }
}

vacuum
coloratio_ponere_registrum(
    Coloratio* coloratio,
    structura RegistrumCommandi* reg)
{
    si (coloratio)
    {
        coloratio->reg_commandi = reg;
    }
}


/* ==================================================
 * Sputnik Keywords
 * ================================================== */

/* Keywords - mapped to their color categories */
hic_manens constans character* clavis_sputnik_declarationes[] = {
    "sit", "constans", "functio", "entitas", NIHIL
};

hic_manens constans character* clavis_sputnik_control[] = {
    "si", "alioquin", "dum", "per", "redde", "frange", "perge", NIHIL
};

hic_manens constans character* clavis_sputnik_valores[] = {
    "verum", "falsum", "nihil", NIHIL
};

hic_manens constans character* clavis_sputnik_builtin[] = {
    "print", "len", "typeof", "REPO", NIHIL
};

/* Verificare si verbum est in lista */
hic_manens b32
_est_in_lista(constans character* verbum, i32 longitudo, constans character** lista)
{
    i32 i;
    constans character* clavis;

    per (i = ZEPHYRUM; lista[i] != NIHIL; i++)
    {
        clavis = lista[i];
        /* Comparare longitudinem et contentum */
        si ((i32)strlen(clavis) == longitudo &&
            memcmp(verbum, clavis, (size_t)longitudo) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Verificare si character est digitus */
hic_manens b32
_est_digitus(character c)
{
    redde (c >= '0' && c <= '9');
}


/* ==================================================
 * Internal: Line Coloring Functions
 * ================================================== */

/* Colorare sputnik syntax in linea */
hic_manens vacuum
_colorare_sputnik(
    Coloratio* coloratio,
    constans TabulaCharacterum* tabula,
    i32 linea)
{
    i32 col;
    i32 initium;
    i32 finis;
    i32 i;
    character c;
    character verbum[LXIV];
    i32 verbum_long;

    col = ZEPHYRUM;

    dum (col < tabula->latitudo)
    {
        c = tabula_cellula(tabula, linea, col);

        /* Skip spaces */
        si (c == ' ' || c == '\0')
        {
            col++;
            perge;
        }

        /* Comments: // */
        si (c == '/' && col + I < tabula->latitudo &&
            tabula_cellula(tabula, linea, col + I) == '/')
        {
            /* Color rest of line as comment */
            per (i = col; i < tabula->latitudo; i++)
            {
                si (tabula_cellula(tabula, linea, i) == '\0') frange;
                coloratio_index(coloratio, linea, i) = COLORATIO_COMMENTUM;
            }
            redde;
        }

        /* Strings: " or ' */
        si (c == '"' || c == '\'')
        {
            character delimitator = c;
            initium = col;
            col++;

            dum (col < tabula->latitudo)
            {
                c = tabula_cellula(tabula, linea, col);
                si (c == '\0') frange;
                si (c == '\\' && col + I < tabula->latitudo)
                {
                    col += II;  /* Skip escape sequence */
                    perge;
                }
                si (c == delimitator)
                {
                    col++;
                    frange;
                }
                col++;
            }

            finis = col;
            per (i = initium; i < finis; i++)
            {
                coloratio_index(coloratio, linea, i) = COLORATIO_CHORDA;
            }
            perge;
        }

        /* Template strings: ` */
        si (c == '`')
        {
            /* Colorare backtick initium */
            coloratio_index(coloratio, linea, col) = COLORATIO_CHORDA;
            col++;

            dum (col < tabula->latitudo)
            {
                c = tabula_cellula(tabula, linea, col);
                si (c == '\0') frange;

                /* Escape sequences */
                si (c == '\\' && col + I < tabula->latitudo)
                {
                    coloratio_index(coloratio, linea, col) = COLORATIO_CHORDA;
                    col++;
                    coloratio_index(coloratio, linea, col) = COLORATIO_CHORDA;
                    col++;
                    perge;
                }

                /* String interpolation: ${ ... } */
                si (c == '$' && col + I < tabula->latitudo &&
                    tabula_cellula(tabula, linea, col + I) == '{')
                {
                    i32 brace_depth;

                    /* Colorare ${ ut operans */
                    coloratio_index(coloratio, linea, col) = COLORATIO_OPERANS;
                    col++;
                    coloratio_index(coloratio, linea, col) = COLORATIO_OPERANS;
                    col++;

                    /* Scandere expressionem, colorare ut defalta */
                    brace_depth = I;
                    dum (col < tabula->latitudo && brace_depth > ZEPHYRUM)
                    {
                        c = tabula_cellula(tabula, linea, col);
                        si (c == '\0') frange;

                        si (c == '{')
                        {
                            brace_depth++;
                            coloratio_index(coloratio, linea, col) = COLORATIO_OPERANS;
                            col++;
                        }
                        alioquin si (c == '}')
                        {
                            brace_depth--;
                            si (brace_depth == ZEPHYRUM)
                            {
                                /* Closing } of interpolation */
                                coloratio_index(coloratio, linea, col) = COLORATIO_OPERANS;
                                col++;
                                frange;
                            }
                            coloratio_index(coloratio, linea, col) = COLORATIO_OPERANS;
                            col++;
                        }
                        alioquin
                        {
                            /* Default color for expression content */
                            coloratio_index(coloratio, linea, col) = COLORATIO_DEFALTA;
                            col++;
                        }
                    }
                    perge;
                }

                /* Closing backtick */
                si (c == '`')
                {
                    coloratio_index(coloratio, linea, col) = COLORATIO_CHORDA;
                    col++;
                    frange;
                }

                /* Normal string content */
                coloratio_index(coloratio, linea, col) = COLORATIO_CHORDA;
                col++;
            }
            perge;
        }

        /* Numbers: digits, optionally with . and $ suffix */
        si (_est_digitus(c) || (c == '.' && col + I < tabula->latitudo &&
            _est_digitus(tabula_cellula(tabula, linea, col + I))))
        {
            initium = col;

            /* Integer part */
            dum (col < tabula->latitudo && _est_digitus(tabula_cellula(tabula, linea, col)))
            {
                col++;
            }

            /* Decimal part */
            si (col < tabula->latitudo && tabula_cellula(tabula, linea, col) == '.')
            {
                col++;
                dum (col < tabula->latitudo && _est_digitus(tabula_cellula(tabula, linea, col)))
                {
                    col++;
                }
            }

            /* Currency suffix $ */
            si (col < tabula->latitudo && tabula_cellula(tabula, linea, col) == '$')
            {
                col++;
            }

            finis = col;
            per (i = initium; i < finis; i++)
            {
                coloratio_index(coloratio, linea, i) = COLORATIO_NUMERUS;
            }
            perge;
        }

        /* Tags: #word */
        si (c == '#' && col + I < tabula->latitudo &&
            _est_character_verbi(tabula_cellula(tabula, linea, col + I)))
        {
            initium = col;
            col++;
            dum (col < tabula->latitudo && _est_character_verbi(tabula_cellula(tabula, linea, col)))
            {
                col++;
            }
            finis = col;
            per (i = initium; i < finis; i++)
            {
                coloratio_index(coloratio, linea, i) = COLORATIO_PROPRIETAS;
            }
            perge;
        }

        /* Identifiers and keywords */
        si (_est_character_verbi(c) && !_est_digitus(c))
        {
            initium = col;
            verbum_long = ZEPHYRUM;

            dum (col < tabula->latitudo && _est_character_verbi(tabula_cellula(tabula, linea, col)))
            {
                si (verbum_long < LXIII)
                {
                    verbum[verbum_long] = tabula_cellula(tabula, linea, col);
                    verbum_long++;
                }
                col++;
            }
            verbum[verbum_long] = '\0';

            finis = col;

            /* Check keyword categories */
            si (_est_in_lista(verbum, verbum_long, clavis_sputnik_declarationes) ||
                _est_in_lista(verbum, verbum_long, clavis_sputnik_control))
            {
                per (i = initium; i < finis; i++)
                {
                    coloratio_index(coloratio, linea, i) = COLORATIO_CLAVIS;
                }
            }
            alioquin si (_est_in_lista(verbum, verbum_long, clavis_sputnik_valores))
            {
                per (i = initium; i < finis; i++)
                {
                    coloratio_index(coloratio, linea, i) = COLORATIO_NUMERUS;  /* Same as numbers */
                }
            }
            alioquin si (_est_in_lista(verbum, verbum_long, clavis_sputnik_builtin))
            {
                per (i = initium; i < finis; i++)
                {
                    coloratio_index(coloratio, linea, i) = COLORATIO_COMMANDUM;  /* Builtins like commands */
                }
            }
            /* Else: leave as default */

            perge;
        }

        /* Operators */
        si (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
            c == '=' || c == '<' || c == '>' || c == '!' || c == '&' || c == '|')
        {
            coloratio_index(coloratio, linea, col) = COLORATIO_OPERANS;
            col++;
            /* Handle multi-char operators */
            si (col < tabula->latitudo)
            {
                character c2 = tabula_cellula(tabula, linea, col);
                si ((c == '=' && c2 == '=') ||
                    (c == '!' && c2 == '=') ||
                    (c == '<' && c2 == '=') ||
                    (c == '>' && c2 == '=') ||
                    (c == '&' && c2 == '&') ||
                    (c == '|' && c2 == '|') ||
                    (c == '=' && c2 == '>') ||
                    (c == '+' && c2 == '+') ||
                    (c == '-' && c2 == '-') ||
                    (c == '+' && c2 == '=') ||
                    (c == '-' && c2 == '=') ||
                    (c == '*' && c2 == '=') ||
                    (c == '/' && c2 == '='))
                {
                    coloratio_index(coloratio, linea, col) = COLORATIO_OPERANS;
                    col++;
                }
            }
            perge;
        }

        /* Default: skip character */
        col++;
    }
}


/* Colorare #link patterns in linea (outside sputnik blocks) */
hic_manens vacuum
_colorare_links(
    Coloratio* coloratio,
    constans TabulaCharacterum* tabula,
    i32 linea)
{
    i32 col;
    i32 initium;
    i32 i;
    character c;

    per (col = ZEPHYRUM; col < tabula->latitudo; col++)
    {
        c = tabula_cellula(tabula, linea, col);

        si (c == '#')
        {
            /* Verificare si proximus character est valida initium verbi */
            si (col + I < tabula->latitudo &&
                _est_character_verbi(tabula_cellula(tabula, linea, col + I)))
            {
                initium = col;
                col++;

                /* Scandere verbum (permittere - pro nomina paginarum) */
                dum (col < tabula->latitudo)
                {
                    c = tabula_cellula(tabula, linea, col);
                    si (!_est_character_verbi(c) && c != '-')
                    {
                        frange;
                    }
                    col++;
                }

                /* Colorare totum #word */
                per (i = initium; i < col; i++)
                {
                    coloratio_index(coloratio, linea, i) = COLORATIO_LINK;
                }

                col--;  /* Adjust for loop increment */
            }
        }
    }
}

/* Colorare $command patterns in linea */
hic_manens vacuum
_colorare_commanda(
    Coloratio* coloratio,
    constans TabulaCharacterum* tabula,
    i32 linea)
{
    i32 col;
    i32 initium;
    i32 i;
    i32 longitudo;
    character c;
    character nomen_commandi[XXXII];
    b32 debet_colorare;

    per (col = ZEPHYRUM; col < tabula->latitudo; col++)
    {
        c = tabula_cellula(tabula, linea, col);

        si (c == '$')
        {
            /* Found command start - scan forward for word chars */
            initium = col;
            col++;

            dum (col < tabula->latitudo)
            {
                c = tabula_cellula(tabula, linea, col);
                si (!_est_character_verbi(c))
                {
                    frange;
                }
                col++;
            }

            /* Si habemus characteres post $, verificare et colorare */
            si (col > initium + I)
            {
                /* Extrahere nomen commandi (sine $) */
                longitudo = col - (initium + I);
                si (longitudo >= XXXII)
                {
                    longitudo = XXXII - I;
                }

                per (i = ZEPHYRUM; i < longitudo; i++)
                {
                    nomen_commandi[i] = tabula_cellula(tabula, linea, initium + I + i);
                }
                nomen_commandi[longitudo] = '\0';

                /* Determinare si debet colorare */
                si (coloratio->reg_commandi)
                {
                    /* Solum colorare si commandum est registratum */
                    debet_colorare = registrum_commandi_habet(
                        coloratio->reg_commandi, nomen_commandi);
                }
                alioquin
                {
                    /* Sine registrum, colorare omnes $word patterns */
                    debet_colorare = VERUM;
                }

                /* Colorare si validum */
                si (debet_colorare)
                {
                    per (i = initium; i < col; i++)
                    {
                        coloratio_index(coloratio, linea, i) = COLORATIO_COMMANDUM;
                    }
                }
            }

            col--;  /* Adjust for loop increment */
        }
    }
}

/* Computare status lineae (for multi-line block tracking) */
hic_manens i8
_computare_status_lineae(
    constans TabulaCharacterum* tabula,
    i32 linea,
    i8 status_initium)
{
    i32 col;
    character c;
    i8 status;
    b32 in_string;
    character string_delim;

    status = status_initium;
    in_string = FALSUM;
    string_delim = '\0';

    per (col = ZEPHYRUM; col < tabula->latitudo; col++)
    {
        c = tabula_cellula(tabula, linea, col);

        si (c == '\0')
        {
            frange;  /* End of content in line */
        }

        /* Track string state to avoid false tag matches */
        si (!in_string)
        {
            si (c == '"' || c == '\'' || c == '`')
            {
                in_string = VERUM;
                string_delim = c;
                perge;
            }
        }
        alioquin
        {
            si (c == string_delim)
            {
                /* Check for escape */
                si (col > ZEPHYRUM && tabula_cellula(tabula, linea, col - I) == '\\')
                {
                    perge;  /* Escaped quote */
                }
                in_string = FALSUM;
                string_delim = '\0';
            }
            perge;  /* Don't check for tags inside strings */
        }

        /* Check for <sputnik> open tag */
        si (c == '<' && status == STATUS_LINEA_NORMALIS)
        {
            /* Check for "<sputnik>" - need at least 9 chars */
            si (col + VIII < tabula->latitudo)
            {
                si (tabula_cellula(tabula, linea, col + I) == 's' &&
                    tabula_cellula(tabula, linea, col + II) == 'p' &&
                    tabula_cellula(tabula, linea, col + III) == 'u' &&
                    tabula_cellula(tabula, linea, col + IV) == 't' &&
                    tabula_cellula(tabula, linea, col + V) == 'n' &&
                    tabula_cellula(tabula, linea, col + VI) == 'i' &&
                    tabula_cellula(tabula, linea, col + VII) == 'k' &&
                    tabula_cellula(tabula, linea, col + VIII) == '>')
                {
                    status = STATUS_LINEA_IN_SPUTNIK;
                    col += VIII;  /* Skip past tag */
                    perge;
                }
            }
        }

        /* Check for </sputnik> close tag */
        si (c == '<' && status == STATUS_LINEA_IN_SPUTNIK)
        {
            /* Check for "</sputnik>" - need at least 10 chars */
            si (col + IX < tabula->latitudo)
            {
                si (tabula_cellula(tabula, linea, col + I) == '/' &&
                    tabula_cellula(tabula, linea, col + II) == 's' &&
                    tabula_cellula(tabula, linea, col + III) == 'p' &&
                    tabula_cellula(tabula, linea, col + IV) == 'u' &&
                    tabula_cellula(tabula, linea, col + V) == 't' &&
                    tabula_cellula(tabula, linea, col + VI) == 'n' &&
                    tabula_cellula(tabula, linea, col + VII) == 'i' &&
                    tabula_cellula(tabula, linea, col + VIII) == 'k' &&
                    tabula_cellula(tabula, linea, col + IX) == '>')
                {
                    status = STATUS_LINEA_NORMALIS;
                    col += IX;  /* Skip past tag */
                    perge;
                }
            }
        }
    }

    redde status;
}

/* Colorare STML/HTML tags in linea */
hic_manens vacuum
_colorare_tags(
    Coloratio* coloratio,
    constans TabulaCharacterum* tabula,
    i32 linea)
{
    i32 col;
    i32 tag_initium;
    character c;
    character quote_char;

    col = ZEPHYRUM;
    dum (col < tabula->latitudo)
    {
        c = tabula_cellula(tabula, linea, col);

        si (c == '\0')
        {
            frange;  /* Finis lineae */
        }

        si (c == '<')
        {
            tag_initium = col;

            /* Colorare '<' ut TAGUM */
            coloratio_index(coloratio, linea, col) = COLORATIO_TAGUM;
            col++;

            si (col >= tabula->latitudo)
            {
                frange;
            }

            c = tabula_cellula(tabula, linea, col);

            /* Tractare closing tag </name> */
            si (c == '/')
            {
                coloratio_index(coloratio, linea, col) = COLORATIO_TAGUM;
                col++;
            }

            /* Scandere nomen tag */
            si (col < tabula->latitudo)
            {
                c = tabula_cellula(tabula, linea, col);
                si (_est_nomen_initium(c))
                {
                    dum (col < tabula->latitudo)
                    {
                        c = tabula_cellula(tabula, linea, col);
                        si (!_est_nomen_character(c))
                        {
                            frange;
                        }
                        coloratio_index(coloratio, linea, col) = COLORATIO_TAGUM;
                        col++;
                    }
                }
            }

            /* Tractare '!' pro raw content tags (STML extension) */
            si (col < tabula->latitudo)
            {
                c = tabula_cellula(tabula, linea, col);
                si (c == '!')
                {
                    coloratio_index(coloratio, linea, col) = COLORATIO_TAGUM;
                    col++;
                }
            }

            /* Scandere attributa */
            dum (col < tabula->latitudo)
            {
                c = tabula_cellula(tabula, linea, col);

                si (c == '\0')
                {
                    frange;
                }

                /* Saltare spatia */
                si (_est_spatium(c))
                {
                    col++;
                    perge;
                }

                /* Tag finis: '>' */
                si (c == '>')
                {
                    coloratio_index(coloratio, linea, col) = COLORATIO_TAGUM;
                    col++;
                    frange;  /* Tag completum */
                }

                /* Self-closing: '/>' */
                si (c == '/')
                {
                    coloratio_index(coloratio, linea, col) = COLORATIO_TAGUM;
                    col++;
                    si (col < tabula->latitudo)
                    {
                        c = tabula_cellula(tabula, linea, col);
                        si (c == '>')
                        {
                            coloratio_index(coloratio, linea, col) = COLORATIO_TAGUM;
                            col++;
                        }
                    }
                    frange;  /* Tag completum */
                }

                /* Nomen attributi */
                si (_est_nomen_initium(c))
                {
                    /* Colorare nomen attributi */
                    dum (col < tabula->latitudo)
                    {
                        c = tabula_cellula(tabula, linea, col);
                        si (!_est_nomen_character(c))
                        {
                            frange;
                        }
                        coloratio_index(coloratio, linea, col) = COLORATIO_ATTRIBUTUM;
                        col++;
                    }

                    /* Verificare pro '=' */
                    si (col < tabula->latitudo)
                    {
                        c = tabula_cellula(tabula, linea, col);
                        si (c == '=')
                        {
                            /* Colorare '=' ut OPERANS */
                            coloratio_index(coloratio, linea, col) = COLORATIO_OPERANS;
                            col++;

                            /* Legere valor */
                            si (col < tabula->latitudo)
                            {
                                c = tabula_cellula(tabula, linea, col);

                                /* Valor in quotis */
                                si (c == '"' || c == '\'')
                                {
                                    quote_char = c;
                                    coloratio_index(coloratio, linea, col) = COLORATIO_VALOR_ATTR;
                                    col++;

                                    /* Scandere usque ad closing quote */
                                    dum (col < tabula->latitudo)
                                    {
                                        c = tabula_cellula(tabula, linea, col);
                                        si (c == '\0')
                                        {
                                            frange;
                                        }
                                        coloratio_index(coloratio, linea, col) = COLORATIO_VALOR_ATTR;
                                        si (c == quote_char)
                                        {
                                            col++;
                                            frange;
                                        }
                                        col++;
                                    }
                                }
                                alioquin si (_est_valor_nudus_character(c))
                                {
                                    /* Valor nudus (bare value) */
                                    dum (col < tabula->latitudo)
                                    {
                                        c = tabula_cellula(tabula, linea, col);
                                        si (!_est_valor_nudus_character(c))
                                        {
                                            frange;
                                        }
                                        coloratio_index(coloratio, linea, col) = COLORATIO_VALOR_ATTR;
                                        col++;
                                    }
                                }
                            }
                        }
                        /* Si non '=', est attributum boolean - iam coloratum */
                    }
                    perge;
                }

                /* Character ignotus in tag - saltare */
                col++;
            }

            (vacuum)tag_initium;  /* Supprimere monitum de variabile non usato */
            perge;
        }

        col++;
    }
}

/* Colorare singulam lineam cum contextu */
hic_manens vacuum
_colorare_lineam(
    Coloratio* coloratio,
    constans TabulaCharacterum* tabula,
    i32 linea,
    i8 status_initium)
{
    /* Apply STML tag highlighting si regula activa */
    si (coloratio->regulae & COLORATIO_REGULA_STML)
    {
        _colorare_tags(coloratio, tabula, linea);
    }

    /* Apply sputnik syntax highlighting si in sputnik block */
    si (status_initium == STATUS_LINEA_IN_SPUTNIK &&
        (coloratio->regulae & COLORATIO_REGULA_SPUTNIK))
    {
        _colorare_sputnik(coloratio, tabula, linea);
    }
    alioquin
    {
        /* Apply #link highlighting in normal text (non sputnik) */
        _colorare_links(coloratio, tabula, linea);
    }

    /* Apply $command highlighting si regula activa */
    si (coloratio->regulae & COLORATIO_REGULA_COMMANDA)
    {
        _colorare_commanda(coloratio, tabula, linea);
    }
}


/* ==================================================
 * Processing
 * ================================================== */

vacuum
coloratio_vacare(
    Coloratio* coloratio)
{
    memoriae_index indices_size;
    memoriae_index status_size;

    si (!coloratio)
    {
        redde;
    }

    indices_size = (memoriae_index)(coloratio->latitudo * coloratio->altitudo) * magnitudo(i8);
    status_size = (memoriae_index)coloratio->altitudo * magnitudo(i8);

    memset(coloratio->indices, COLORATIO_DEFALTA, (size_t)indices_size);
    memset(coloratio->status_lineae, STATUS_LINEA_NORMALIS, (size_t)status_size);
}

vacuum
coloratio_computare(
    Coloratio* coloratio,
    constans TabulaCharacterum* tabula)
{
    i32 linea;
    i8 status_currens;
    i8 status_initium;

    si (!coloratio || !tabula)
    {
        redde;
    }

    /* Vacare indices ante computare */
    coloratio_vacare(coloratio);

    /* Pass 1: Compute line-ending states for block tracking */
    status_currens = STATUS_LINEA_NORMALIS;
    per (linea = ZEPHYRUM; linea < tabula->altitudo; linea++)
    {
        status_currens = _computare_status_lineae(tabula, linea, status_currens);
        coloratio->status_lineae[linea] = status_currens;
    }

    /* Pass 2: Color each line knowing its context */
    per (linea = ZEPHYRUM; linea < tabula->altitudo; linea++)
    {
        status_initium = (linea > ZEPHYRUM) ?
            coloratio->status_lineae[linea - I] :
            STATUS_LINEA_NORMALIS;

        _colorare_lineam(coloratio, tabula, linea, status_initium);
    }
}


/* ==================================================
 * Query
 * ================================================== */

i8
coloratio_obtinere(
    constans Coloratio* coloratio,
    i32 linea,
    i32 columna)
{
    si (!coloratio || !coloratio->indices)
    {
        redde COLORATIO_DEFALTA;
    }

    si (linea < ZEPHYRUM || linea >= coloratio->altitudo ||
        columna < ZEPHYRUM || columna >= coloratio->latitudo)
    {
        redde COLORATIO_DEFALTA;
    }

    redde coloratio->indices[linea * coloratio->latitudo + columna];
}

i8
coloratio_obtinere_status_lineae(
    constans Coloratio* coloratio,
    i32 linea)
{
    si (!coloratio || !coloratio->status_lineae)
    {
        redde STATUS_LINEA_NORMALIS;
    }

    si (linea < ZEPHYRUM || linea >= coloratio->altitudo)
    {
        redde STATUS_LINEA_NORMALIS;
    }

    redde coloratio->status_lineae[linea];
}
