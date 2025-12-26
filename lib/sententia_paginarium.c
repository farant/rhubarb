/* sententia_paginarium.c - Implementatio Paginationis Sententiarum
 */

#include "sententia_paginarium.h"
#include "sententia_fissio.h"
#include "utf8.h"
#include <string.h>

/* ========================================================================
 * CONSTANTAE
 * ======================================================================== */

#define LIMINA_VERSUS_DEFECTUS     LXX   /* 70% */
#define LINEAE_VERSUS_PROBARE      V     /* 5 lineae */

/* ========================================================================
 * FUNCTIONES INTERNAE - Characteres
 * ======================================================================== */

interior b32
_est_spatium(character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* Verificare si range est solum whitespace */
interior b32
_est_solum_spatium(chorda textus, i32 initium, i32 finis)
{
    i32 i;

    si (finis <= initium)
    {
        redde VERUM;
    }

    per (i = initium; i < finis; i++)
    {
        si (!_est_spatium((character)textus.datum[i]))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* ========================================================================
 * FUNCTIONES INTERNAE - Paragraph et Versus Detectio
 * ======================================================================== */

/* Numerare newlines consecutivas ad positionem
 * Tractare \r\n ut unam newline
 */
interior i32
_numerare_newlines(chorda textus, i32 pos)
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

/* Detegere si paragraphus est versus
 * Heuristica: si limina_pct% linearum < limina_lat% latitudinis
 * UTF-8 aware
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
        constans i8* scan_ptr;
        constans i8* scan_finis = textus.datum + textus.mensura;

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

        /* Mensurare lineam usque ad '\n' vel '\r' (UTF-8 aware) */
        scan_ptr = textus.datum + pos;
        dum (scan_ptr < scan_finis)
        {
            s32 runa = utf8_decodere(&scan_ptr, scan_finis);
            si (runa == '\n' || runa == '\r')
            {
                frange;
            }
            longitudo_lineae++;
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
        pos = (i32)(scan_ptr - textus.datum);
    }

    /* Si nullae lineae, non est versus */
    si (lineae_probatae == 0)
    {
        redde FALSUM;
    }

    /* Minimus II lineae necessariae */
    si (lineae_probatae < II)
    {
        redde FALSUM;
    }

    /* limina_pct% threshold */
    redde (lineae_breves * C >= lineae_probatae * limina_pct);
}

/* Invenire finem paragraphi (2+ newlines vel finis textus)
 * Redde: positio post ultimum character ante paragraph break
 */
interior i32
_invenire_finem_paragraphi(chorda textus, i32 pos)
{
    dum (pos < textus.mensura)
    {
        /* Probare paragraph break */
        si (_numerare_newlines(textus, pos) >= II)
        {
            redde pos;
        }

        pos++;
    }

    redde textus.mensura;
}

/* ========================================================================
 * FUNCTIONES INTERNAE - Extractio Linearum Versus
 * ======================================================================== */

/* Extrahere lineas versus de paragrapho
 * Quaeque linea usque ad \n est una SententiaCopia
 */
interior vacuum
_extrahere_lineas_versus(
    chorda   textus,
    i32      paragraphus_init,
    i32      paragraphus_fin,
    Xar*     copiae)
{
    i32 pos = paragraphus_init;

    dum (pos < paragraphus_fin)
    {
        SententiaCopia* copia;
        i32 linea_init;
        i32 linea_fin;

        /* Saltare leading newlines/spaces */
        dum (pos < paragraphus_fin &&
             ((character)textus.datum[pos] == '\n' ||
              (character)textus.datum[pos] == '\r'))
        {
            pos++;
        }

        si (pos >= paragraphus_fin)
        {
            frange;
        }

        linea_init = pos;

        /* Invenire finem lineae */
        dum (pos < paragraphus_fin &&
             (character)textus.datum[pos] != '\n' &&
             (character)textus.datum[pos] != '\r')
        {
            pos++;
        }

        linea_fin = pos;

        /* Trim trailing spaces */
        dum (linea_fin > linea_init && (character)textus.datum[linea_fin - I] == ' ')
        {
            linea_fin--;
        }

        /* Saltare lineas vacuas vel solum spatium */
        si (linea_fin > linea_init && !_est_solum_spatium(textus, linea_init, linea_fin))
        {
            copia = (SententiaCopia*)xar_addere(copiae);
            copia->initium = linea_init;
            copia->finis = linea_fin;
            copia->est_versus = VERUM;
            copia->est_vacua = FALSUM;
        }
    }
}

/* ========================================================================
 * FUNCTIONES INTERNAE - Extractio Sententiarum Prosae
 * ======================================================================== */

/* Extrahere sententias de paragrapho prosa
 * Usat sententia_fissio_indices()
 */
interior vacuum
_extrahere_sententias_prosa(
    chorda   textus,
    i32      paragraphus_init,
    i32      paragraphus_fin,
    Xar*     copiae,
    Piscina* piscina)
{
    chorda sectio;
    SententiaIndicesFructus indices;
    i32 i;

    /* Creare sectio pro paragrapho */
    sectio.datum = textus.datum + paragraphus_init;
    sectio.mensura = paragraphus_fin - paragraphus_init;

    /* Findere in sententias */
    indices = sententia_fissio_indices(sectio, piscina);

    /* Convertere indices relativos ad absolutos et addere ad copiae */
    per (i = 0; i < indices.numerus; i++)
    {
        i32 abs_init = paragraphus_init + indices.indices[i].initium;
        i32 abs_fin = paragraphus_init + indices.indices[i].finis;

        /* Saltare sententias vacuas vel solum spatium */
        si (!_est_solum_spatium(textus, abs_init, abs_fin))
        {
            SententiaCopia* copia = (SententiaCopia*)xar_addere(copiae);
            copia->initium = abs_init;
            copia->finis = abs_fin;
            copia->est_versus = FALSUM;
            copia->est_vacua = FALSUM;
        }
    }
}

/* ========================================================================
 * FUNCTIONES INTERNAE - Word Wrapping
 * ======================================================================== */

/* Word-wrap una sententia in lineas
 * Similis ad logicam paginarium sed pro una sententia
 */
interior SententiaPagina*
_wrap_sententia(
    SententiaPaginariumResultus* resultus,
    SententiaCopia*              copia)
{
    SententiaPagina* pagina;
    i32 pos;
    i32 latitudo = resultus->cache_latitudo;

    pagina = (SententiaPagina*)xar_addere(resultus->paginae);
    pagina->lineae = xar_creare(resultus->piscina, magnitudo(PaginariumLinea));
    pagina->initium = copia->initium;
    pagina->finis = copia->finis;
    pagina->est_versus = copia->est_versus;

    si (copia->est_vacua)
    {
        /* Linea vacua */
        PaginariumLinea* linea = (PaginariumLinea*)xar_addere(pagina->lineae);
        linea->initium = copia->initium;
        linea->finis = copia->finis;
        linea->est_vacua = VERUM;
        linea->modus_versus = FALSUM;
        redde pagina;
    }

    pos = copia->initium;

    dum (pos < copia->finis)
    {
        PaginariumLinea* linea;
        i32 visual_len;
        i32 source_len;
        i32 last_break;
        b32 habet_break;
        constans i8* scan_ptr;
        constans i8* scan_finis;

        linea = (PaginariumLinea*)xar_addere(pagina->lineae);
        linea->initium = pos;
        linea->est_vacua = FALSUM;
        linea->modus_versus = copia->est_versus;

        si (copia->est_versus)
        {
            /* Versus: preservare structuram lineae fonte */
            i32 line_len = 0;
            i32 offset = 0;
            i32 last_space_offset = 0;
            b32 habet_spatium = FALSUM;
            b32 hit_newline = FALSUM;

            scan_ptr = resultus->textus.datum + pos;
            scan_finis = resultus->textus.datum + copia->finis;

            dum (scan_ptr < scan_finis && line_len < latitudo)
            {
                constans i8* runa_initium = scan_ptr;
                s32 runa = utf8_decodere(&scan_ptr, scan_finis);
                i32 runa_bytes = (i32)(scan_ptr - runa_initium);

                si (runa == '\n' || runa == '\r')
                {
                    hit_newline = VERUM;
                    frange;
                }
                si (runa == ' ')
                {
                    last_space_offset = offset;
                    habet_spatium = VERUM;
                }
                line_len++;
                offset += runa_bytes;
            }

            si (line_len >= latitudo && habet_spatium && !hit_newline)
            {
                linea->finis = pos + last_space_offset;
                pos = pos + last_space_offset + I;
            }
            alioquin
            {
                linea->finis = pos + offset;
                pos = pos + offset;
                /* Consumere newline */
                si (hit_newline && pos < copia->finis)
                {
                    si ((character)resultus->textus.datum[pos] == '\r')
                    {
                        pos++;
                    }
                    si (pos < copia->finis && (character)resultus->textus.datum[pos] == '\n')
                    {
                        pos++;
                    }
                }
            }
        }
        alioquin
        {
            /* Prosa: tractare \n ut spatium, word-wrap */
            visual_len = 0;
            source_len = 0;
            last_break = 0;
            habet_break = FALSUM;

            scan_ptr = resultus->textus.datum + pos;
            scan_finis = resultus->textus.datum + copia->finis;

            dum (visual_len < latitudo && scan_ptr < scan_finis)
            {
                constans i8* runa_initium = scan_ptr;
                s32 runa = utf8_decodere(&scan_ptr, scan_finis);
                i32 runa_bytes = (i32)(scan_ptr - runa_initium);

                /* Praeterire \r */
                si (runa == '\r')
                {
                    source_len += runa_bytes;
                    perge;
                }

                /* \n -> spatium */
                si (runa == '\n')
                {
                    source_len += runa_bytes;
                    last_break = source_len;
                    habet_break = VERUM;
                    visual_len++;
                    perge;
                }

                si (runa == ' ')
                {
                    source_len += runa_bytes;
                    last_break = source_len;
                    habet_break = VERUM;
                    visual_len++;
                    perge;
                }

                source_len += runa_bytes;
                visual_len++;
            }

            /* Applicare word wrap */
            si (visual_len >= latitudo && habet_break)
            {
                linea->finis = pos + last_break;
                pos = pos + last_break;
            }
            alioquin
            {
                linea->finis = pos + source_len;
                pos = pos + source_len;
            }
        }
    }

    redde pagina;
}

/* ========================================================================
 * FUNCTIONES PUBLICAE - CREATIO
 * ======================================================================== */

SententiaPaginariumResultus*
sententia_paginarium_creare(
    chorda   textus,
    i32      latitudo,
    i32      altitudo,
    Piscina* piscina)
{
    SententiaPaginariumResultus* resultus;
    i32 pos;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    resultus = piscina_allocare(piscina, magnitudo(SententiaPaginariumResultus));
    si (resultus == NIHIL)
    {
        redde NIHIL;
    }

    resultus->copiae = xar_creare(piscina, magnitudo(SententiaCopia));
    resultus->paginae = xar_creare(piscina, magnitudo(SententiaPagina));
    resultus->numerus_sententiarum = 0;
    resultus->textus = textus;
    resultus->piscina = piscina;
    resultus->cache_latitudo = latitudo;
    resultus->cache_altitudo = altitudo;

    si (textus.mensura <= 0)
    {
        redde resultus;
    }

    /* Iterare per paragraphos */
    pos = 0;

    dum (pos < textus.mensura)
    {
        i32 paragraphus_init;
        i32 paragraphus_fin;
        b32 est_versus;
        i32 newlines;

        /* Saltare leading whitespace */
        dum (pos < textus.mensura && _est_spatium((character)textus.datum[pos]))
        {
            pos++;
        }

        si (pos >= textus.mensura)
        {
            frange;
        }

        paragraphus_init = pos;

        /* Invenire finem paragraphi */
        paragraphus_fin = _invenire_finem_paragraphi(textus, pos);

        /* Detegere modus versus/prosa */
        est_versus = _est_versus(textus, paragraphus_init, latitudo,
                                 LIMINA_VERSUS_DEFECTUS, LINEAE_VERSUS_PROBARE);

        /* Extrahere unitates */
        si (est_versus)
        {
            _extrahere_lineas_versus(textus, paragraphus_init, paragraphus_fin, resultus->copiae);
        }
        alioquin
        {
            _extrahere_sententias_prosa(textus, paragraphus_init, paragraphus_fin, resultus->copiae, piscina);
        }

        /* Movere post paragraph break */
        pos = paragraphus_fin;
        newlines = _numerare_newlines(textus, pos);
        pos += newlines;
    }

    resultus->numerus_sententiarum = (i32)xar_numerus(resultus->copiae);

    redde resultus;
}

/* ========================================================================
 * FUNCTIONES PUBLICAE - ACCESSUS
 * ======================================================================== */

i32
sententia_paginarium_numerus(
    SententiaPaginariumResultus* resultus)
{
    si (resultus == NIHIL)
    {
        redde 0;
    }

    redde resultus->numerus_sententiarum;
}

SententiaCopia*
sententia_paginarium_copia_obtinere(
    SententiaPaginariumResultus* resultus,
    i32                          index)
{
    si (resultus == NIHIL || resultus->copiae == NIHIL)
    {
        redde NIHIL;
    }

    si (index < 0 || index >= resultus->numerus_sententiarum)
    {
        redde NIHIL;
    }

    redde (SententiaCopia*)xar_obtinere(resultus->copiae, index);
}

SententiaPagina*
sententia_paginarium_pagina_obtinere(
    SententiaPaginariumResultus* resultus,
    i32                          index)
{
    SententiaCopia* copia;
    i32 num_paginae;

    si (resultus == NIHIL)
    {
        redde NIHIL;
    }

    si (index < 0 || index >= resultus->numerus_sententiarum)
    {
        redde NIHIL;
    }

    /* Probare si iam in cache */
    num_paginae = (i32)xar_numerus(resultus->paginae);

    si (index < num_paginae)
    {
        redde (SententiaPagina*)xar_obtinere(resultus->paginae, index);
    }

    /* Lazy: creare paginas usque ad index */
    dum (num_paginae <= index)
    {
        copia = sententia_paginarium_copia_obtinere(resultus, num_paginae);
        si (copia == NIHIL)
        {
            redde NIHIL;
        }
        _wrap_sententia(resultus, copia);
        num_paginae++;
    }

    redde (SententiaPagina*)xar_obtinere(resultus->paginae, index);
}

/* ========================================================================
 * FUNCTIONES PUBLICAE - REDDITIO
 * ======================================================================== */

chorda
sententia_paginarium_linea_reddere(
    SententiaPaginariumResultus* resultus,
    PaginariumLinea*             linea)
{
    chorda res;
    i32 longitudo;
    i8* buffer;
    i32 i;
    i32 buf_pos;

    res.datum = NIHIL;
    res.mensura = 0;

    si (resultus == NIHIL || linea == NIHIL)
    {
        redde res;
    }

    si (linea->est_vacua)
    {
        redde res;
    }

    longitudo = linea->finis - linea->initium;
    si (longitudo <= 0)
    {
        redde res;
    }

    buffer = (i8*)piscina_allocare(resultus->piscina, (i64)(longitudo + I));
    si (buffer == NIHIL)
    {
        redde res;
    }

    buf_pos = 0;

    si (linea->modus_versus)
    {
        /* Versus: copiare ut est */
        per (i = linea->initium; i < linea->finis; i++)
        {
            character c = (character)resultus->textus.datum[i];
            si (c != '\n' && c != '\r')
            {
                buffer[buf_pos++] = resultus->textus.datum[i];
            }
        }
    }
    alioquin
    {
        /* Prosa: substituere \n cum spatio */
        per (i = linea->initium; i < linea->finis; i++)
        {
            character c = (character)resultus->textus.datum[i];
            si (c == '\n' || c == '\r')
            {
                si (buf_pos > 0 && buffer[buf_pos - 1] != ' ')
                {
                    buffer[buf_pos++] = ' ';
                }
            }
            alioquin
            {
                buffer[buf_pos++] = resultus->textus.datum[i];
            }
        }

        /* Removere trailing spatium */
        dum (buf_pos > 0 && buffer[buf_pos - 1] == ' ')
        {
            buf_pos--;
        }
    }

    res.datum = buffer;
    res.mensura = buf_pos;

    redde res;
}
