/* capsula.c - File Embedding Library Implementation
 *
 * Runtime pro legere embedded files.
 */

#include "capsula.h"
#include "flatura.h"
#include "via.h"
#include "filum.h"
#include "iter_directoria.h"

#include <string.h>  /* memcmp */


/* ========================================================================
 * MODUS DISCI
 * ========================================================================
 *
 * Capsula duos fontes habet, ansam unam. radix.mensura discernit.
 * Vide capsula.h pro consilio; hic sola mechanica est.
 */

interior b32
_e_disco (constans Capsula* capsula);

interior b32
_e_disco (
    constans Capsula* capsula)
{
    redde capsula != NIHIL && capsula->radix.mensura > ZEPHYRUM;
}


/* Viam relativam cum radice iungere, normalizare, TRAVERSALEM REICERE.
 *
 * Redde chordam mensurae ZEPHYRUM si via extra radicem evadit. Custodia
 * eadem est quam hospitium in _filum_servire adhibet: praefixum
 * byteorum CUM limite segmenti ("/tmp/x" praefixum "/tmp/xy" non est).
 */
interior chorda
_via_tuta (constans Capsula* capsula, chorda relativa, Piscina* piscina);

interior chorda
_via_tuta (
    constans Capsula* capsula,
              chorda  relativa,
             Piscina* piscina)
{
    chorda partes[II];
    chorda iuncta;
    chorda normalizata;
    chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (relativa.mensura == ZEPHYRUM)
    {
        redde vacua;
    }

    partes[0]    = capsula->radix;
    partes[I]    = relativa;
    iuncta       = via_iungere(partes, (i32)II, piscina);
    normalizata  = via_normalizare(iuncta, piscina);

    si (   !chorda_incipit(normalizata, capsula->radix)
        || (normalizata.mensura > capsula->radix.mensura
            && normalizata.datum[capsula->radix.mensura] != '/'))
    {
        redde vacua;
    }

    redde normalizata;
}


interior CapsulaFructus
_e_disco_legere (constans Capsula* capsula, chorda relativa,
    Piscina* piscina);

interior CapsulaFructus
_e_disco_legere (
    constans Capsula* capsula,
              chorda  relativa,
             Piscina* piscina)
{
        CapsulaFructus  fructus;
                chorda  via_plena;
    constans character* cstr;

    fructus.status         = CAPSULA_NON_INVENTUM;
    fructus.datum.datum    = NIHIL;
    fructus.datum.mensura  = ZEPHYRUM;

    via_plena = _via_tuta(capsula, relativa, piscina);
    si (via_plena.mensura == ZEPHYRUM)
    {
        redde fructus;   /* traversalis reiecta, aut via vacua */
    }

    cstr = chorda_ut_cstr(via_plena, piscina);
    si (cstr == NIHIL)
    {
        fructus.status = CAPSULA_FRACTA_ALLOCATIO;
        redde fructus;
    }

    /* EXISTENTIA PRIMUM: filum_legere_totum mensuram ZEPHYRUM pro
     * UTROQUE reddit - filo vacuo et filo absenti. Sine hac custodia
     * filum vacuum ut CAPSULA_NON_INVENTUM appareret, quod mendacium
     * est quod nemo in facie videret. */
    si (!filum_existit(cstr))
    {
        redde fructus;
    }

    fructus.datum   = filum_legere_totum(cstr, piscina);
    fructus.status  = CAPSULA_OK;
    redde fructus;
}


/* Contextus ambulationis: cursus I numerat, cursus II implet. */
nomen structura {
    Capsula* capsula;
        i32  numerus;
        b32  implere;
    Piscina* piscina;
} ContextusAmbulandi;


interior s32
_introitum_tractare (chorda via_plena,
    constans DirectoriumIntroitus* introitus, vacuum* contextus);

interior s32
_introitum_tractare (
                           chorda  via_plena,
    constans DirectoriumIntroitus* introitus,
                           vacuum* contextus)
{
    ContextusAmbulandi* ctx    = (ContextusAmbulandi*)contextus;
                chorda  radix  = ctx->capsula->radix;
                   i32  initium;

    si (introitus == NIHIL || introitus->genus != INTROITUS_FILUM)
    {
        redde ZEPHYRUM;
    }
    /* Radix praefixum sit; aliter viam relativam computare non possumus */
    si (   !chorda_incipit(via_plena, radix)
        || via_plena.mensura <= radix.mensura)
    {
        redde ZEPHYRUM;
    }

    initium = radix.mensura;
    si (via_plena.datum[initium] == '/')
    {
        initium = initium + (i32)I;
    }

    si (ctx->implere)
    {
         i8* copia;
        i32  longitudo = via_plena.mensura - initium;
        i32  j;

        copia = (i8*)piscina_allocare(ctx->piscina,
            (memoriae_index)longitudo);
        si (copia == NIHIL)
        {
            redde (s32)I;   /* ambulationem sistere */
        }
        per (j = ZEPHYRUM; j < longitudo; j++)
        {
            copia[j] = via_plena.datum[initium + j];
        }

        ctx->capsula->index[ctx->numerus].via.datum = copia;
        ctx->capsula->index[ctx->numerus].via.mensura = longitudo;
        ctx->capsula->index[ctx->numerus].data_offset = ZEPHYRUM;
        ctx->capsula->index[ctx->numerus].mensura_compressa = ZEPHYRUM;
        ctx->capsula->index[ctx->numerus].mensura_cruda =
            (i32)introitus->mensura;
        ctx->capsula->index[ctx->numerus].compressa = FALSUM;
    }

    ctx->numerus = ctx->numerus + (i32)I;
    redde ZEPHYRUM;
}

Capsula*
capsula_aperire_e_disco (
    constans character* radix,
               Piscina* piscina)
{
               Capsula* capsula;
                chorda  normalizata;
    constans character* radix_cstr;
    DirectoriumFiltrum  filtrum;
        IntroitusGenus  genera[I];
    ContextusAmbulandi  ctx;

    si (radix == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }

    /* Absoluta ET normalizata: custodia traversalis praefixo byteorum
     * nititur, ergo radix ipsa formam canonicam habeat necesse est. */
    normalizata = via_normalizare(
        via_absoluta(chorda_ex_literis(radix, piscina), piscina),
        piscina);
    si (normalizata.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    radix_cstr = chorda_ut_cstr(normalizata, piscina);
    si (radix_cstr == NIHIL || !filum_directorium_existit(radix_cstr))
    {
        redde NIHIL;
    }

    capsula = (Capsula*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Capsula));
    si (capsula == NIHIL)
    {
        redde NIHIL;
    }
    capsula->datum            = NIHIL;
    capsula->mensura          = ZEPHYRUM;
    capsula->numerus_filorum  = ZEPHYRUM;
    capsula->index            = NIHIL;
    capsula->piscina          = piscina;
    capsula->radix            = normalizata;

    genera[0]                   = INTROITUS_FILUM;
    filtrum                     = directorium_filtrum_omnia();
    filtrum.genera_accepta      = genera;
    filtrum.genera_numerus      = (i32)I;
    filtrum.includere_occultos  = FALSUM;

    /* CURSUS I: numerare */
    ctx.capsula = capsula;
    ctx.numerus = ZEPHYRUM;
    ctx.implere = FALSUM;
    ctx.piscina = piscina;
    si (directorium_ambulare(radix_cstr, &filtrum, _introitum_tractare,
            &ctx, piscina) != ZEPHYRUM)
    {
        redde NIHIL;
    }

    si (ctx.numerus > ZEPHYRUM)
    {
        capsula->index = (CapsulaIndexum*)piscina_allocare(piscina,
            (memoriae_index)ctx.numerus
                * (memoriae_index)magnitudo(CapsulaIndexum));
        si (capsula->index == NIHIL)
        {
            redde NIHIL;
        }

        /* CURSUS II: implere */
        ctx.numerus = ZEPHYRUM;
        ctx.implere = VERUM;
        si (directorium_ambulare(radix_cstr, &filtrum,
                _introitum_tractare, &ctx, piscina) != ZEPHYRUM)
        {
            redde NIHIL;
        }
    }

    capsula->numerus_filorum = ctx.numerus;
    redde capsula;
}


/* ========================================================================
 * HELPER FUNCTIONES
 * ======================================================================== */

/* Legere i32 little-endian ex byte pointer */
interior i32
_capsula_legere_i32 (
    constans i8* p)
{
    i32 result;

    result = ((i32)(i8)p[0] & 0xFF)
        | (((i32)(i8)p[1] & 0xFF) << VIII)
        | (((i32)(i8)p[2] & 0xFF) << XVI)
        | (((i32)(i8)p[3] & 0xFF) << XXIV);

    redde result;
}

/* Comparare chorda cum C string */
interior b32
_capsula_via_aequalis (
                chorda  a,
    constans character* b)
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
capsula_aperire (
    constans CapsulaEmbed* embed,
                  Piscina* piscina)
{
        Capsula* capsula;
    constans i8* p;
            i32  magic;
            i32  versio;
            i32  numerus;
            i32  toc_offset;
            i32  i;

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
    magic    = _capsula_legere_i32(p);
    versio   = _capsula_legere_i32(p + IV);
    numerus  = _capsula_legere_i32(p + VIII);

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

    capsula->datum            = embed->datum;
    capsula->mensura          = embed->mensura;
    capsula->numerus_filorum  = numerus;
    capsula->piscina          = piscina;
    capsula->radix.datum      = NIHIL;      /* infixa, non e disco */
    capsula->radix.mensura    = ZEPHYRUM;

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
        i32  path_offset;
        i32  path_len;
        i32  data_offset;
        i32  comp_size;
        i32  raw_size;
         i8* path_copy;
        i32  j;

        constans i8* entry_p;

        entry_p = p + toc_offset + (i * XX);

        path_offset  = _capsula_legere_i32(entry_p);
        path_len     = _capsula_legere_i32(entry_p + IV);
        data_offset  = _capsula_legere_i32(entry_p + VIII);
        comp_size    = _capsula_legere_i32(entry_p + XII);
        raw_size     = _capsula_legere_i32(entry_p + XVI);

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

        capsula->index[i].via.datum          = path_copy;
        capsula->index[i].via.mensura        = (i32)path_len;
        capsula->index[i].data_offset        = data_offset;
        capsula->index[i].mensura_compressa  = comp_size;
        capsula->index[i].mensura_cruda      = raw_size;
        capsula->index[i].compressa          = (comp_size != raw_size);
    }

    redde capsula;
}


/* ========================================================================
 * LEGERE
 * ======================================================================== */

CapsulaFructus
capsula_legere (
               Capsula* capsula,
    constans character* via,
               Piscina* piscina)
{
    CapsulaFructus  fructus;
    CapsulaIndexum* entry;
       constans i8* data_ptr;

    fructus.status         = CAPSULA_OK;
    fructus.datum.datum    = NIHIL;
    fructus.datum.mensura  = 0;

    si (capsula == NIHIL || via == NIHIL || piscina == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    si (_e_disco(capsula))
    {
        redde _e_disco_legere(capsula,
            chorda_ex_literis(via, piscina), piscina);
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

        fructus.datum.datum    = inflated.datum;
        fructus.datum.mensura  = inflated.mensura;
    }
    alioquin
    {
        /* Uncompressed - copy data to piscina (embedded data is const) */
         i8* copy;
        i32  k;

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

        fructus.datum.datum    = copy;
        fructus.datum.mensura  = entry->mensura_cruda;
    }

    redde fructus;
}

CapsulaFructus
capsula_legere_chorda (
    Capsula* capsula,
     chorda  via,
    Piscina* piscina)
{
    CapsulaFructus  fructus;
    CapsulaIndexum* entry;
       constans i8* data_ptr;
               i32  i;

    fructus.status         = CAPSULA_OK;
    fructus.datum.datum    = NIHIL;
    fructus.datum.mensura  = 0;

    si (capsula == NIHIL || piscina == NIHIL)
    {
        fructus.status = CAPSULA_NON_INVENTUM;
        redde fructus;
    }

    si (_e_disco(capsula))
    {
        redde _e_disco_legere(capsula, via, piscina);
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

        fructus.datum.datum    = inflated.datum;
        fructus.datum.mensura  = inflated.mensura;
    }
    alioquin
    {
        /* Uncompressed - copy data to piscina (embedded data is const) */
         i8* copy;
        i32  k;

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

        fructus.datum.datum    = copy;
        fructus.datum.mensura  = entry->mensura_cruda;
    }

    redde fructus;
}


/* ========================================================================
 * QUAERERE
 * ======================================================================== */

b32
capsula_habet (
               Capsula* capsula,
    constans character* via)
{
    /* Modo disci DISCUM interrogat, non indicem photographatum: aliter
     * habet() et legere() dissentirent de plagula post aperturam addita,
     * et dissensus ille vitium esset quod nemo quaereret. */
    si (_e_disco(capsula) && via != NIHIL)
    {
                    chorda  via_plena;
        constans character* cstr;

        via_plena = _via_tuta(capsula,
            chorda_ex_literis(via, capsula->piscina), capsula->piscina);
        si (via_plena.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        cstr = chorda_ut_cstr(via_plena, capsula->piscina);
        redde cstr != NIHIL && filum_existit(cstr);
    }

    redde capsula_invenire(capsula, via) != NIHIL;
}

CapsulaIndexum*
capsula_invenire (
               Capsula* capsula,
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
capsula_numerus (
    Capsula* capsula)
{
    si (capsula == NIHIL)
    {
        redde 0;
    }

    redde capsula->numerus_filorum;
}

CapsulaIndexum*
capsula_indexum (
    Capsula* capsula,
        i32  index)
{
    si (capsula == NIHIL || index < 0 || index >= capsula->numerus_filorum)
    {
        redde NIHIL;
    }

    redde &capsula->index[index];
}

CapsulaIter
capsula_iter (
    Capsula* capsula)
{
    CapsulaIter iter;

    iter.capsula = capsula;
    iter.positus = 0;

    redde iter;
}

b32
capsula_iter_proximus (
    CapsulaIter* iter)
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
capsula_status_nuntium (
    CapsulaStatus status)
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
