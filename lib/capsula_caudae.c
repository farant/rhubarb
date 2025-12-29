/* capsula_caudae.c - Tail-Appended Asset Embedding Implementation
 *
 * Runtime pro legere assets ex cauda binarii.
 */

#include "capsula_caudae.h"
#include "flatura.h"

#include <string.h>  /* memcmp */


/* ========================================================================
 * HELPER FUNCTIONES
 * ======================================================================== */

/* Legere i32 little-endian ex byte pointer */
interior i32
_caudae_legere_i32(constans i8* p)
{
    i32 result;

    result = ((i32)(i8)p[0] & 0xFF) |
             (((i32)(i8)p[1] & 0xFF) << VIII) |
             (((i32)(i8)p[2] & 0xFF) << XVI) |
             (((i32)(i8)p[3] & 0xFF) << XXIV);

    redde result;
}


/* Legere i64 little-endian ex byte pointer */
interior i64
_caudae_legere_i64(constans i8* p)
{
    i64 result;
    i64 low;
    i64 high;

    low = (i64)((i32)(i8)p[0] & 0xFF) |
          ((i64)((i32)(i8)p[1] & 0xFF) << VIII) |
          ((i64)((i32)(i8)p[2] & 0xFF) << XVI) |
          ((i64)((i32)(i8)p[3] & 0xFF) << XXIV);

    high = (i64)((i32)(i8)p[4] & 0xFF) |
           ((i64)((i32)(i8)p[5] & 0xFF) << VIII) |
           ((i64)((i32)(i8)p[6] & 0xFF) << XVI) |
           ((i64)((i32)(i8)p[7] & 0xFF) << XXIV);

    result = low | (high << 32);

    redde result;
}


/* Comparare chorda cum C string */
interior b32
_caudae_via_aequalis(chorda a, constans character* b)
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
 * APERIRE / CLAUDERE
 * ======================================================================== */

CapsulaCaudae*
capsula_caudae_aperire(
    constans character* via_binarii,
    Piscina*            piscina)
{
    CapsulaCaudae*  capsula;
    FILUM*          filum;
    i8              footer[XVI];
    i32             magic0;
    i32             magic1;
    i64             asset_offset;
    i8              header[XII];
    i32             header_magic;
    i32             versio;
    i32             numerus;
    i32             toc_size;
    i8*             toc_data;
    i32             i;
    si (via_binarii == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }

    /* Aperire filum binarii */
    filum = fopen(via_binarii, "rb");
    si (filum == NIHIL)
    {
        redde NIHIL;
    }

    /* Seek ad finem - 16 bytes pro footer */
    si (fseek(filum, -CAPSULA_CAUDAE_FOOTER_MENSURA, SEEK_END) != 0)
    {
        fclose(filum);
        redde NIHIL;
    }

    /* Legere footer */
    si (fread(footer, I, (size_t)CAPSULA_CAUDAE_FOOTER_MENSURA, filum) !=
        (size_t)CAPSULA_CAUDAE_FOOTER_MENSURA)
    {
        fclose(filum);
        redde NIHIL;
    }

    /* Verificare magic "CAPSCLDA" */
    magic0 = _caudae_legere_i32(footer + VIII);
    magic1 = _caudae_legere_i32(footer + XII);

    si (magic0 != (i32)CAPSULA_CAUDAE_MAGICA_0 ||
        magic1 != (i32)CAPSULA_CAUDAE_MAGICA_1)
    {
        fclose(filum);
        redde NIHIL;
    }

    /* Legere asset offset */
    asset_offset = _caudae_legere_i64(footer);

    /* Seek ad asset data */
    si (fseek(filum, (longus)asset_offset, SEEK_SET) != 0)
    {
        fclose(filum);
        redde NIHIL;
    }

    /* Legere header (12 bytes) */
    si (fread(header, I, XII, filum) != XII)
    {
        fclose(filum);
        redde NIHIL;
    }

    header_magic = _caudae_legere_i32(header);
    versio = _caudae_legere_i32(header + IV);
    numerus = _caudae_legere_i32(header + VIII);

    /* Validate header magic (same as capsula) */
    si (header_magic != (i32)CAPSULA_MAGICA)
    {
        fclose(filum);
        redde NIHIL;
    }

    /* Validate version */
    si (versio != (i32)CAPSULA_VERSIO)
    {
        fclose(filum);
        redde NIHIL;
    }

    /* Allocare capsula struct */
    capsula = (CapsulaCaudae*)piscina_allocare(piscina,
                                                (memoriae_index)magnitudo(CapsulaCaudae));
    si (capsula == NIHIL)
    {
        fclose(filum);
        redde NIHIL;
    }

    capsula->filum = filum;
    capsula->asset_offset = asset_offset;
    capsula->numerus_filorum = numerus;
    capsula->piscina = piscina;

    /* Allocare index array */
    si (numerus > 0)
    {
        capsula->index = (CapsulaIndexum*)piscina_allocare(
            piscina,
            (memoriae_index)numerus * (memoriae_index)magnitudo(CapsulaIndexum));

        si (capsula->index == NIHIL)
        {
            fclose(filum);
            redde NIHIL;
        }
    }
    alioquin
    {
        capsula->index = NIHIL;
        redde capsula;
    }

    /* Legere TOC data (20 bytes per entry) */
    toc_size = numerus * XX;
    toc_data = (i8*)piscina_allocare(piscina, (memoriae_index)toc_size);
    si (toc_data == NIHIL)
    {
        fclose(filum);
        redde NIHIL;
    }

    si (fread(toc_data, I, (size_t)toc_size, filum) != (size_t)toc_size)
    {
        fclose(filum);
        redde NIHIL;
    }

    /* Parse TOC entries et legere path strings */
    per (i = 0; i < numerus; i++)
    {
        i32 path_offset;
        i32 path_len;
        i32 data_offset;
        i32 comp_size;
        i32 raw_size;
        i8* path_copy;

        constans i8* entry_p;

        entry_p = toc_data + (i * XX);

        path_offset = _caudae_legere_i32(entry_p);
        path_len = _caudae_legere_i32(entry_p + IV);
        data_offset = _caudae_legere_i32(entry_p + VIII);
        comp_size = _caudae_legere_i32(entry_p + XII);
        raw_size = _caudae_legere_i32(entry_p + XVI);

        /* Allocare et legere path string */
        path_copy = (i8*)piscina_allocare(piscina, (memoriae_index)path_len);
        si (path_copy == NIHIL)
        {
            fclose(filum);
            redde NIHIL;
        }

        /* Seek to path in string table et legere
         * path_offset is relative to asset blob, add asset_offset */
        si (fseek(filum, (longus)(asset_offset + (i64)path_offset), SEEK_SET) != 0)
        {
            fclose(filum);
            redde NIHIL;
        }

        si (fread(path_copy, I, (size_t)path_len, filum) != (size_t)path_len)
        {
            fclose(filum);
            redde NIHIL;
        }

        capsula->index[i].via.datum = path_copy;
        capsula->index[i].via.mensura = path_len;
        capsula->index[i].data_offset = data_offset;
        capsula->index[i].mensura_compressa = comp_size;
        capsula->index[i].mensura_cruda = raw_size;
        capsula->index[i].compressa = (comp_size != raw_size);
    }

    redde capsula;
}


vacuum
capsula_caudae_claudere(CapsulaCaudae* capsula)
{
    si (capsula != NIHIL && capsula->filum != NIHIL)
    {
        fclose(capsula->filum);
        capsula->filum = NIHIL;
    }
}


/* ========================================================================
 * LEGERE
 * ======================================================================== */

CapsulaFructus
capsula_caudae_legere(
    CapsulaCaudae*      capsula,
    constans character* via,
    Piscina*            piscina)
{
    CapsulaFructus   fructus;
    CapsulaIndexum*  entry;
    i8*              compressed_data;
    i64              file_data_offset;

    fructus.status = CAPSULA_OK;
    fructus.datum.datum = NIHIL;
    fructus.datum.mensura = 0;

    si (capsula == NIHIL || via == NIHIL || piscina == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    si (capsula->filum == NIHIL)
    {
        fructus.status = CAPSULA_FRACTA_DATUM;
        redde fructus;
    }

    /* Invenire entry */
    entry = capsula_caudae_invenire(capsula, via);
    si (entry == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    /* Seek ad data in file
     * data_offset is relative to asset blob, add asset_offset
     */
    file_data_offset = capsula->asset_offset + (i64)entry->data_offset;

    si (fseek(capsula->filum, (longus)file_data_offset, SEEK_SET) != 0)
    {
        fructus.status = CAPSULA_FRACTA_DATUM;
        redde fructus;
    }

    si (entry->compressa)
    {
        FlaturaFructus inflated;

        /* Legere compressed data */
        compressed_data = (i8*)piscina_allocare(piscina,
                                                 (memoriae_index)entry->mensura_compressa);
        si (compressed_data == NIHIL)
        {
            fructus.status = CAPSULA_FRACTA_ALLOCATIO;
            redde fructus;
        }

        si (fread(compressed_data, I, (size_t)entry->mensura_compressa, capsula->filum) !=
            (size_t)entry->mensura_compressa)
        {
            fructus.status = CAPSULA_FRACTA_DATUM;
            redde fructus;
        }

        /* Decompress */
        inflated = flatura_gzip_inflare(compressed_data,
                                        entry->mensura_compressa,
                                        piscina);

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
        /* Uncompressed - legere directe */
        i8* data;

        data = (i8*)piscina_allocare(piscina, (memoriae_index)entry->mensura_cruda);
        si (data == NIHIL)
        {
            fructus.status = CAPSULA_FRACTA_ALLOCATIO;
            redde fructus;
        }

        si (fread(data, I, (size_t)entry->mensura_cruda, capsula->filum) !=
            (size_t)entry->mensura_cruda)
        {
            fructus.status = CAPSULA_FRACTA_DATUM;
            redde fructus;
        }

        fructus.datum.datum = data;
        fructus.datum.mensura = entry->mensura_cruda;
    }

    redde fructus;
}


CapsulaFructus
capsula_caudae_legere_chorda(
    CapsulaCaudae* capsula,
    chorda         via,
    Piscina*       piscina)
{
    CapsulaFructus  fructus;
    CapsulaIndexum* entry;
    i8*             compressed_data;
    i64             file_data_offset;
    i32             i;

    fructus.status = CAPSULA_OK;
    fructus.datum.datum = NIHIL;
    fructus.datum.mensura = 0;

    si (capsula == NIHIL || piscina == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    si (capsula->filum == NIHIL)
    {
        fructus.status = CAPSULA_FRACTA_DATUM;
        redde fructus;
    }

    /* Invenire entry per chorda comparison */
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

    /* Seek ad data in file - add asset_offset */
    file_data_offset = capsula->asset_offset + (i64)entry->data_offset;

    si (fseek(capsula->filum, (longus)file_data_offset, SEEK_SET) != 0)
    {
        fructus.status = CAPSULA_FRACTA_DATUM;
        redde fructus;
    }

    si (entry->compressa)
    {
        FlaturaFructus inflated;

        compressed_data = (i8*)piscina_allocare(piscina,
                                                 (memoriae_index)entry->mensura_compressa);
        si (compressed_data == NIHIL)
        {
            fructus.status = CAPSULA_FRACTA_ALLOCATIO;
            redde fructus;
        }

        si (fread(compressed_data, I, (size_t)entry->mensura_compressa, capsula->filum) !=
            (size_t)entry->mensura_compressa)
        {
            fructus.status = CAPSULA_FRACTA_DATUM;
            redde fructus;
        }

        inflated = flatura_gzip_inflare(compressed_data,
                                        entry->mensura_compressa,
                                        piscina);

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
        i8* data;

        data = (i8*)piscina_allocare(piscina, (memoriae_index)entry->mensura_cruda);
        si (data == NIHIL)
        {
            fructus.status = CAPSULA_FRACTA_ALLOCATIO;
            redde fructus;
        }

        si (fread(data, I, (size_t)entry->mensura_cruda, capsula->filum) !=
            (size_t)entry->mensura_cruda)
        {
            fructus.status = CAPSULA_FRACTA_DATUM;
            redde fructus;
        }

        fructus.datum.datum = data;
        fructus.datum.mensura = entry->mensura_cruda;
    }

    redde fructus;
}


/* ========================================================================
 * QUAERERE
 * ======================================================================== */

b32
capsula_caudae_habet(
    CapsulaCaudae*      capsula,
    constans character* via)
{
    redde capsula_caudae_invenire(capsula, via) != NIHIL;
}


CapsulaIndexum*
capsula_caudae_invenire(
    CapsulaCaudae*      capsula,
    constans character* via)
{
    i32 i;

    si (capsula == NIHIL || via == NIHIL)
    {
        redde NIHIL;
    }

    per (i = 0; i < capsula->numerus_filorum; i++)
    {
        si (_caudae_via_aequalis(capsula->index[i].via, via))
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
capsula_caudae_numerus(CapsulaCaudae* capsula)
{
    si (capsula == NIHIL)
    {
        redde 0;
    }

    redde capsula->numerus_filorum;
}


CapsulaIndexum*
capsula_caudae_indexum(CapsulaCaudae* capsula, i32 index)
{
    si (capsula == NIHIL || index < 0 || index >= capsula->numerus_filorum)
    {
        redde NIHIL;
    }

    redde &capsula->index[index];
}


CapsulaCaudaeIter
capsula_caudae_iter(CapsulaCaudae* capsula)
{
    CapsulaCaudaeIter iter;

    iter.capsula = capsula;
    iter.positus = 0;

    redde iter;
}


b32
capsula_caudae_iter_proximus(CapsulaCaudaeIter* iter)
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

b32
capsula_caudae_habet_assets(constans character* via_binarii)
{
    FILUM* filum;
    i8    footer[XVI];
    i32   magic0;
    i32   magic1;

    si (via_binarii == NIHIL)
    {
        redde FALSUM;
    }

    filum = fopen(via_binarii, "rb");
    si (filum == NIHIL)
    {
        redde FALSUM;
    }

    /* Seek ad finem - 16 bytes */
    si (fseek(filum, -CAPSULA_CAUDAE_FOOTER_MENSURA, SEEK_END) != 0)
    {
        fclose(filum);
        redde FALSUM;
    }

    /* Legere footer */
    si (fread(footer, I, (size_t)CAPSULA_CAUDAE_FOOTER_MENSURA, filum) !=
        (size_t)CAPSULA_CAUDAE_FOOTER_MENSURA)
    {
        fclose(filum);
        redde FALSUM;
    }

    fclose(filum);

    /* Verificare magic */
    magic0 = _caudae_legere_i32(footer + VIII);
    magic1 = _caudae_legere_i32(footer + XII);

    redde (magic0 == (i32)CAPSULA_CAUDAE_MAGICA_0 &&
           magic1 == (i32)CAPSULA_CAUDAE_MAGICA_1);
}
