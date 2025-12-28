/* capsula.c - File Embedding Library Implementation
 *
 * Runtime pro legere embedded files.
 */

#include "capsula.h"
#include "flatura.h"

#include <string.h>  /* memcmp */


/* ========================================================================
 * HELPER FUNCTIONES
 * ======================================================================== */

/* Legere i32 little-endian ex byte pointer */
interior i32
_capsula_legere_i32(constans i8* p)
{
    i32 result;

    result = ((i32)(i8)p[0] & 0xFF) |
             (((i32)(i8)p[1] & 0xFF) << VIII) |
             (((i32)(i8)p[2] & 0xFF) << XVI) |
             (((i32)(i8)p[3] & 0xFF) << XXIV);

    redde result;
}


/* Comparare chorda cum C string */
interior b32
_capsula_via_aequalis(chorda a, constans character* b)
{
    i32 len_b;
    i32 i;

    len_b = 0;
    dum (b[len_b] != '\0')
    {
        len_b++;
    }

    si ((i32)a.mensura != len_b)
    {
        redde FALSUM;
    }

    per (i = 0; i < len_b; i++)
    {
        si (a.datum[i] != (i8)b[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}


/* ========================================================================
 * APERIRE
 * ======================================================================== */

Capsula*
capsula_aperire(
    constans CapsulaEmbed* embed,
    Piscina*               piscina)
{
    Capsula*    capsula;
    constans i8* p;
    i32         magic;
    i32         versio;
    i32         numerus;
    i32         toc_offset;
    i32         i;

    si (embed == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }

    si (embed->datum == NIHIL || embed->mensura < XII)
    {
        redde NIHIL;
    }

    p = embed->datum;

    /* Legere header */
    magic = _capsula_legere_i32(p);
    versio = _capsula_legere_i32(p + IV);
    numerus = _capsula_legere_i32(p + VIII);

    /* Validate magic */
    si (magic != (i32)CAPSULA_MAGICA)
    {
        redde NIHIL;
    }

    /* Validate version */
    si (versio != (i32)CAPSULA_VERSIO)
    {
        redde NIHIL;
    }

    /* Allocate capsula struct */
    capsula = (Capsula*)piscina_allocare(piscina, (memoriae_index)magnitudo(Capsula));
    si (capsula == NIHIL)
    {
        redde NIHIL;
    }

    capsula->datum = embed->datum;
    capsula->mensura = embed->mensura;
    capsula->numerus_filorum = numerus;
    capsula->piscina = piscina;

    /* Allocate index array */
    si (numerus > 0)
    {
        capsula->index = (CapsulaIndexum*)piscina_allocare(
            piscina,
            (memoriae_index)numerus * (memoriae_index)magnitudo(CapsulaIndexum));

        si (capsula->index == NIHIL)
        {
            redde NIHIL;
        }
    }
    alioquin
    {
        capsula->index = NIHIL;
    }

    /* Parse TOC entries
     * Format per entry (20 bytes):
     *   path_offset (4), path_len (4), data_offset (4),
     *   compressed_size (4), raw_size (4)
     */
    toc_offset = XII;  /* After header */

    per (i = 0; i < numerus; i++)
    {
        i32 path_offset;
        i32 path_len;
        i32 data_offset;
        i32 comp_size;
        i32 raw_size;
        i8* path_copy;
        i32 j;

        constans i8* entry_p;

        entry_p = p + toc_offset + (i * XX);

        path_offset = _capsula_legere_i32(entry_p);
        path_len = _capsula_legere_i32(entry_p + IV);
        data_offset = _capsula_legere_i32(entry_p + VIII);
        comp_size = _capsula_legere_i32(entry_p + XII);
        raw_size = _capsula_legere_i32(entry_p + XVI);

        /* Copy path string into piscina (embedded data is const) */
        path_copy = (i8*)piscina_allocare(piscina, (memoriae_index)path_len);
        si (path_copy == NIHIL)
        {
            redde NIHIL;
        }
        per (j = 0; j < path_len; j++)
        {
            path_copy[j] = p[path_offset + j];
        }

        capsula->index[i].via.datum = path_copy;
        capsula->index[i].via.mensura = (i32)path_len;
        capsula->index[i].data_offset = data_offset;
        capsula->index[i].mensura_compressa = comp_size;
        capsula->index[i].mensura_cruda = raw_size;
        capsula->index[i].compressa = (comp_size != raw_size);
    }

    redde capsula;
}


/* ========================================================================
 * LEGERE
 * ======================================================================== */

CapsulaFructus
capsula_legere(
    Capsula*            capsula,
    constans character* via,
    Piscina*            piscina)
{
    CapsulaFructus   fructus;
    CapsulaIndexum*  entry;
    constans i8*     data_ptr;

    fructus.status = CAPSULA_OK;
    fructus.datum.datum = NIHIL;
    fructus.datum.mensura = 0;

    si (capsula == NIHIL || via == NIHIL || piscina == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    /* Find entry */
    entry = capsula_invenire(capsula, via);
    si (entry == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    /* Get pointer to data */
    data_ptr = capsula->datum + entry->data_offset;

    si (entry->compressa)
    {
        /* Decompress with flatura */
        FlaturaFructus inflated;

        inflated = flatura_gzip_inflare(data_ptr, entry->mensura_compressa, piscina);

        si (inflated.status != FLATURA_STATUS_OK)
        {
            fructus.status = CAPSULA_FRACTA_INFLATIO;
            redde fructus;
        }

        fructus.datum.datum = inflated.datum;
        fructus.datum.mensura = inflated.mensura;
    }
    alioquin
    {
        /* Uncompressed - copy data to piscina (embedded data is const) */
        i8* copy;
        i32 k;

        copy = (i8*)piscina_allocare(piscina, (memoriae_index)entry->mensura_cruda);
        si (copy == NIHIL)
        {
            fructus.status = CAPSULA_FRACTA_ALLOCATIO;
            redde fructus;
        }

        per (k = 0; k < entry->mensura_cruda; k++)
        {
            copy[k] = data_ptr[k];
        }

        fructus.datum.datum = copy;
        fructus.datum.mensura = entry->mensura_cruda;
    }

    redde fructus;
}


CapsulaFructus
capsula_legere_chorda(
    Capsula* capsula,
    chorda   via,
    Piscina* piscina)
{
    CapsulaFructus  fructus;
    CapsulaIndexum* entry;
    constans i8*    data_ptr;
    i32             i;

    fructus.status = CAPSULA_OK;
    fructus.datum.datum = NIHIL;
    fructus.datum.mensura = 0;

    si (capsula == NIHIL || piscina == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    /* Find entry by chorda comparison */
    entry = NIHIL;
    per (i = 0; i < capsula->numerus_filorum; i++)
    {
        si (capsula->index[i].via.mensura == via.mensura)
        {
            b32 match;
            i32 j;

            match = VERUM;
            per (j = 0; j < (i32)via.mensura; j++)
            {
                si (capsula->index[i].via.datum[j] != via.datum[j])
                {
                    match = FALSUM;
                    frange;
                }
            }

            si (match)
            {
                entry = &capsula->index[i];
                frange;
            }
        }
    }

    si (entry == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    /* Get pointer to data */
    data_ptr = capsula->datum + entry->data_offset;

    si (entry->compressa)
    {
        FlaturaFructus inflated;

        inflated = flatura_gzip_inflare(data_ptr, entry->mensura_compressa, piscina);

        si (inflated.status != FLATURA_STATUS_OK)
        {
            fructus.status = CAPSULA_FRACTA_INFLATIO;
            redde fructus;
        }

        fructus.datum.datum = inflated.datum;
        fructus.datum.mensura = inflated.mensura;
    }
    alioquin
    {
        /* Uncompressed - copy data to piscina (embedded data is const) */
        i8* copy;
        i32 k;

        copy = (i8*)piscina_allocare(piscina, (memoriae_index)entry->mensura_cruda);
        si (copy == NIHIL)
        {
            fructus.status = CAPSULA_FRACTA_ALLOCATIO;
            redde fructus;
        }

        per (k = 0; k < entry->mensura_cruda; k++)
        {
            copy[k] = data_ptr[k];
        }

        fructus.datum.datum = copy;
        fructus.datum.mensura = entry->mensura_cruda;
    }

    redde fructus;
}


/* ========================================================================
 * QUAERERE
 * ======================================================================== */

b32
capsula_habet(
    Capsula*            capsula,
    constans character* via)
{
    redde capsula_invenire(capsula, via) != NIHIL;
}


CapsulaIndexum*
capsula_invenire(
    Capsula*            capsula,
    constans character* via)
{
    i32 i;

    si (capsula == NIHIL || via == NIHIL)
    {
        redde NIHIL;
    }

    per (i = 0; i < capsula->numerus_filorum; i++)
    {
        si (_capsula_via_aequalis(capsula->index[i].via, via))
        {
            redde &capsula->index[i];
        }
    }

    redde NIHIL;
}


/* ========================================================================
 * ITERATIO
 * ======================================================================== */

i32
capsula_numerus(Capsula* capsula)
{
    si (capsula == NIHIL)
    {
        redde 0;
    }

    redde capsula->numerus_filorum;
}


CapsulaIndexum*
capsula_indexum(Capsula* capsula, i32 index)
{
    si (capsula == NIHIL || index < 0 || index >= capsula->numerus_filorum)
    {
        redde NIHIL;
    }

    redde &capsula->index[index];
}


CapsulaIter
capsula_iter(Capsula* capsula)
{
    CapsulaIter iter;

    iter.capsula = capsula;
    iter.positus = 0;

    redde iter;
}


b32
capsula_iter_proximus(CapsulaIter* iter)
{
    si (iter == NIHIL || iter->capsula == NIHIL)
    {
        redde FALSUM;
    }

    si (iter->positus >= iter->capsula->numerus_filorum)
    {
        redde FALSUM;
    }

    iter->positus++;
    redde VERUM;
}


/* ========================================================================
 * UTILITATES
 * ======================================================================== */

constans character*
capsula_status_nuntium(CapsulaStatus status)
{
    commutatio (status)
    {
        casus CAPSULA_OK:
            redde "OK";
        casus CAPSULA_FRACTA_MAGICA:
            redde "Magica invalida";
        casus CAPSULA_FRACTA_VERSIO:
            redde "Versio non supportata";
        casus CAPSULA_FRACTA_ALLOCATIO:
            redde "Allocatio fallita";
        casus CAPSULA_NON_INVENTUM:
            redde "Filum non inventum";
        casus CAPSULA_FRACTA_INFLATIO:
            redde "Decompressio fallita";
        casus CAPSULA_FRACTA_DATUM:
            redde "Datum invalidum";
        ordinarius:
            redde "Status ignotus";
    }
}
