/* paginarium.c - Implementatio Paginarii
 */

#include "paginarium.h"
#include <string.h>

/* ========================================================================
 * FUNCTIONES INTERNAE
 * ======================================================================== */

/* Numerare newlines consecutivas ad positionem
 * Tractare \r\n ut unam newline (CRLF compatibility)
 */
interior i32
_numerare_newlines(
    chorda textus,
    i32    pos)
{
    i32 numerus = 0;

    dum (pos < textus.mensura)
    {
        character c = (character)textus.datum[pos];

        /* Praeterire \r */
        si (c == '\r')
        {
            pos++;
            perge;
        }

        /* Numerare \n */
        si (c == '\n')
        {
            numerus++;
            pos++;
            perge;
        }

        /* Non-whitespace - terminare */
        frange;
    }

    redde numerus;
}

/* Detegere si paragraphus est versus (poesis, index, etc.)
 * Heuristica: si limina_pct% primarum lineae_probare linearum sunt < limina_lat% latitudinis
 */
interior b32
_est_versus(
    chorda textus,
    i32    pos,
    i32    latitudo,
    i32    limina_pct,
    i32    lineae_probare)
{
    i32 lineae_breves = 0;
    i32 lineae_probatae = 0;
    i32 limina_lat = (latitudo * limina_pct) / C;

    /* Probare usque ad lineae_probare lineas */
    dum (lineae_probatae < lineae_probare && pos < textus.mensura)
    {
        i32 longitudo_lineae = 0;
        i32 offset = 0;

        /* Praeterire \r\n ante lineam */
        dum (pos < textus.mensura)
        {
            character c = (character)textus.datum[pos];
            si (c == '\r' || c == '\n')
            {
                pos++;
            }
            alioquin
            {
                frange;
            }
        }

        /* Mensurare lineam usque ad '\n' vel '\r' */
        dum (pos + offset < textus.mensura)
        {
            character c = (character)textus.datum[pos + offset];
            si (c == '\n' || c == '\r')
            {
                frange;
            }
            longitudo_lineae++;
            offset++;
        }

        /* Praeterire lineas vacuas */
        si (longitudo_lineae == 0)
        {
            perge;
        }

        si (longitudo_lineae < limina_lat)
        {
            lineae_breves++;
        }

        lineae_probatae++;
        pos += offset;
    }

    /* Si limina_pct% linearum sunt breves, est versus */
    si (lineae_probatae == 0)
    {
        redde FALSUM;
    }

    /* Minimus II lineae necessariae pro detectione versus */
    /* Una linea brevis non sufficit - potest esse finis paragraphi */
    si (lineae_probatae < II)
    {
        redde FALSUM;
    }

    /* 80% threshold: lineae_breves * 100 >= lineae_probatae * limina_pct */
    redde (lineae_breves * C >= lineae_probatae * limina_pct);
}

/* ========================================================================
 * FUNCTIONES PUBLICAE - CONFIGURATIO
 * ======================================================================== */

PaginariumConfig
paginarium_config_defectus(vacuum)
{
    PaginariumConfig config;

    config.latitudo = PAGINARIUM_LATITUDO_DEFECTUS;
    config.altitudo = PAGINARIUM_ALTITUDO_DEFECTUS;
    config.limina_versus = PAGINARIUM_LIMINA_VERSUS_DEFECTUS;
    config.lineae_versus_probare = PAGINARIUM_LINEAE_PROBARE_DEFECTUS;

    redde config;
}

/* ========================================================================
 * FUNCTIONES PUBLICAE - PAGINATIO
 * ======================================================================== */

PaginariumResultus
paginarium_paginare(
    chorda           textus,
    PaginariumConfig config,
    Piscina*         piscina)
{
    PaginariumResultus resultus;
    i32 pos;
    b32 modus_versus;

    resultus.paginae = xar_creare(piscina, magnitudo(PaginariumPagina));
    resultus.numerus_paginarum = 0;

    si (textus.mensura == 0)
    {
        redde resultus;
    }

    pos = 0;

    /* Detegere modus initialis */
    modus_versus = _est_versus(textus, 0, config.latitudo,
                               config.limina_versus, config.lineae_versus_probare);

    dum (pos < textus.mensura)
    {
        PaginariumPagina* pagina;
        i32 lineae_usae = 0;

        /* Creare novam paginam */
        pagina = (PaginariumPagina*)xar_addere(resultus.paginae);
        pagina->lineae = xar_creare(piscina, magnitudo(PaginariumLinea));
        pagina->initium = pos;

        dum (pos < textus.mensura && lineae_usae < config.altitudo)
        {
            PaginariumLinea* linea;
            i32 newlines;

            /* Verificare paragraph break (2+ newlines) */
            newlines = _numerare_newlines(textus, pos);
            si (newlines >= II)
            {
                /* Emittere lineam vacuam */
                linea = (PaginariumLinea*)xar_addere(pagina->lineae);
                linea->initium = pos;
                linea->finis = pos + newlines;
                linea->est_vacua = VERUM;
                linea->modus_versus = modus_versus;

                pos += newlines;
                lineae_usae++;

                /* Re-detegere modus pro novo paragrapho */
                modus_versus = _est_versus(textus, pos, config.latitudo,
                                           config.limina_versus, config.lineae_versus_probare);
                perge;
            }

            /* Creare lineam */
            linea = (PaginariumLinea*)xar_addere(pagina->lineae);
            linea->est_vacua = FALSUM;
            linea->modus_versus = modus_versus;
            linea->initium = pos;

            si (modus_versus)
            {
                /* Versus: preservare structuram, word-wrap si necessarium */
                i32 line_len = 0;
                i32 offset = 0;
                s32 last_space = -1;
                b32 hit_newline = FALSUM;

                dum (pos + offset < textus.mensura && line_len < config.latitudo)
                {
                    character c = (character)textus.datum[pos + offset];
                    si (c == '\n' || c == '\r')
                    {
                        hit_newline = VERUM;
                        frange;
                    }
                    si (c == ' ')
                    {
                        last_space = (s32)offset;
                    }
                    line_len++;
                    offset++;
                }

                /* Si linea excedit latitudinem et habemus spatium, wrap ibi */
                si (line_len >= config.latitudo && last_space > 0 && !hit_newline)
                {
                    linea->finis = pos + (i32)last_space;
                    pos += (i32)last_space + I;
                }
                alioquin
                {
                    linea->finis = pos + offset;
                    pos += offset;
                    /* Consumere \r\n si praesens, NISI est paragraph break */
                    si (hit_newline && pos < textus.mensura)
                    {
                        /* Praeterire \r */
                        dum (pos < textus.mensura &&
                             (character)textus.datum[pos] == '\r')
                        {
                            pos++;
                        }
                        /* Non consumere \n si est paragraph break (2+ newlines) */
                        si (pos < textus.mensura &&
                            (character)textus.datum[pos] == '\n' &&
                            _numerare_newlines(textus, pos) < II)
                        {
                            pos++;
                        }
                    }
                }
            }
            alioquin
            {
                /* Prosa: tractare singulum newline ut spatium */
                i32 visual_len = 0;
                i32 source_len = 0;
                s32 last_break_source = -1;

                dum (visual_len < config.latitudo && pos + source_len < textus.mensura)
                {
                    character c = (character)textus.datum[pos + source_len];

                    /* Praeterire \r (non computare ut visual) */
                    si (c == '\r')
                    {
                        source_len++;
                        perge;
                    }

                    /* Verificare paragraph break */
                    si (c == '\n')
                    {
                        si (_numerare_newlines(textus, pos + source_len) >= II)
                        {
                            frange;  /* Terminare ante paragraph break */
                        }
                        /* Singulum newline = spatium */
                        last_break_source = (s32)(source_len + I);
                        source_len++;
                        visual_len++;
                        perge;
                    }

                    si (c == ' ')
                    {
                        last_break_source = (s32)(source_len + I);
                    }

                    source_len++;
                    visual_len++;
                }

                /* Applicare word wrap */
                si (visual_len >= config.latitudo && last_break_source > 0)
                {
                    linea->finis = pos + (i32)last_break_source;
                    pos += (i32)last_break_source;
                }
                alioquin
                {
                    linea->finis = pos + source_len;
                    pos += source_len;
                }
            }

            lineae_usae++;
        }

        pagina->finis = pos;
        resultus.numerus_paginarum++;
    }

    redde resultus;
}

/* ========================================================================
 * FUNCTIONES PUBLICAE - ACCESSUS
 * ======================================================================== */

PaginariumPagina*
paginarium_pagina_obtinere(
    PaginariumResultus* resultus,
    i32                 index)
{
    si (!resultus || !resultus->paginae)
    {
        redde NIHIL;
    }

    si (index < 0 || index >= resultus->numerus_paginarum)
    {
        redde NIHIL;
    }

    redde (PaginariumPagina*)xar_obtinere(resultus->paginae, index);
}

PaginariumLinea*
paginarium_linea_obtinere(
    PaginariumPagina* pagina,
    i32               index)
{
    si (!pagina || !pagina->lineae)
    {
        redde NIHIL;
    }

    si (index < 0 || index >= (i32)xar_numerus(pagina->lineae))
    {
        redde NIHIL;
    }

    redde (PaginariumLinea*)xar_obtinere(pagina->lineae, index);
}

i32
paginarium_pagina_numerus_linearum(
    PaginariumPagina* pagina)
{
    si (!pagina || !pagina->lineae)
    {
        redde 0;
    }

    redde (i32)xar_numerus(pagina->lineae);
}

/* ========================================================================
 * FUNCTIONES PUBLICAE - REDDITIO
 * ======================================================================== */

chorda
paginarium_linea_reddere(
    chorda           textus,
    PaginariumLinea* linea,
    Piscina*         piscina)
{
    chorda resultus;
    i32 longitudo;
    i8* buffer;
    i32 i;
    i32 buf_pos;

    resultus.datum = NIHIL;
    resultus.mensura = 0;

    si (!linea || !piscina)
    {
        redde resultus;
    }

    /* Linea vacua */
    si (linea->est_vacua)
    {
        redde resultus;
    }

    longitudo = linea->finis - linea->initium;
    si (longitudo <= 0)
    {
        redde resultus;
    }

    /* Allocare buffer (potentialiter maior si non versus) */
    buffer = (i8*)piscina_allocare(piscina, (i32)longitudo + I);
    si (!buffer)
    {
        redde resultus;
    }

    buf_pos = 0;

    si (linea->modus_versus)
    {
        /* Versus: copiare ut est (praeter trailing newline) */
        per (i = linea->initium; i < linea->finis; i++)
        {
            character c = (character)textus.datum[i];
            si (c != '\n' && c != '\r')
            {
                buffer[buf_pos++] = textus.datum[i];
            }
        }
    }
    alioquin
    {
        /* Prosa: substituere singulum \n cum spatio */
        per (i = linea->initium; i < linea->finis; i++)
        {
            character c = (character)textus.datum[i];
            si (c == '\n' || c == '\r')
            {
                /* Non addere spatium si iam spatium */
                si (buf_pos > 0 && buffer[buf_pos - 1] != ' ')
                {
                    buffer[buf_pos++] = ' ';
                }
            }
            alioquin
            {
                buffer[buf_pos++] = textus.datum[i];
            }
        }

        /* Removere trailing spatium */
        dum (buf_pos > 0 && buffer[buf_pos - 1] == ' ')
        {
            buf_pos--;
        }
    }

    resultus.datum = buffer;
    resultus.mensura = buf_pos;

    redde resultus;
}
