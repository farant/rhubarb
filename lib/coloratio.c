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
 * Internal: Line Coloring Functions
 * ================================================== */

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

/* Colorare singulam lineam cum contextu */
hic_manens vacuum
_colorare_lineam(
    Coloratio* coloratio,
    constans TabulaCharacterum* tabula,
    i32 linea,
    i8 status_initium)
{
    (vacuum)status_initium;  /* TODO: use for sputnik syntax coloring */

    /* Apply $command highlighting si regula activa */
    si (coloratio->regulae & COLORATIO_REGULA_COMMANDA)
    {
        _colorare_commanda(coloratio, tabula, linea);
    }

    /* TODO: STML tag highlighting */
    /* TODO: Sputnik syntax highlighting when status_initium == IN_SPUTNIK */
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
