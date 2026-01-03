/*
 * imago.c - Image loading implementation
 *
 * Utitur stb_image.h pro caricare imagines.
 * stb_image est detailium implementationis, non exponitur in API.
 *
 * NOTA: stb_image.h DEBET esse ANTE latina.h propter conflictus macros
 * (fac, L, FILE, etc.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Configuratio stb_image */
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_THREAD_LOCALS

/* Disable warnings for stb_image */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcomment"
#pragma clang diagnostic ignored "-Wcast-qual"
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif

#include "../vendor/stb_image.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

/* ============================================================
 * Wrapper functions in pure C (ante latina.h)
 * ============================================================ */

static unsigned char*
_stbi_load_file(const char* filename, int* width, int* height)
{
    return stbi_load(filename, width, height, NULL, 4);
}

static unsigned char*
_stbi_load_mem(const unsigned char* buffer, int len, int* width, int* height)
{
    return stbi_load_from_memory(buffer, len, width, height, NULL, 4);
}

static void
_stbi_free(unsigned char* data)
{
    stbi_image_free(data);
}

static const char*
_stbi_error(void)
{
    return stbi_failure_reason();
}

/* ============================================================
 * Nunc includere imago.h (et latina.h)
 * ============================================================ */

#include "imago.h"

/* ============================================================
 * Functiones Publicae
 * ============================================================ */

ImagoFructus
imago_caricare_ex_file(constans character* via, Piscina* piscina)
{
    ImagoFructus fructus;
    insignatus character* stbi_data;
    integer lat, alt;
    i32 pixela_size;
    constans character* err;

    /* Initiare fructum */
    fructus.successus = FALSUM;
    fructus.imago.pixela = NIHIL;
    fructus.imago.latitudo = 0;
    fructus.imago.altitudo = 0;
    fructus.error.datum = NIHIL;
    fructus.error.mensura = 0;

    si (via == NIHIL || piscina == NIHIL)
    {
        fructus.error = chorda_ex_literis("Argumenta invalida", piscina);
        redde fructus;
    }

    /* Caricare cum stb_image */
    stbi_data = _stbi_load_file(via, &lat, &alt);
    si (stbi_data == NIHIL)
    {
        err = _stbi_error();
        si (err != NIHIL)
        {
            fructus.error = chorda_ex_literis(err, piscina);
        }
        alioquin
        {
            fructus.error = chorda_ex_literis("Non possum caricare imaginem", piscina);
        }
        redde fructus;
    }

    /* Copiare ad piscina */
    pixela_size = (i32)(lat * alt * 4);
    fructus.imago.pixela = (i8*)piscina_allocare(piscina, (memoriae_index)pixela_size);
    si (fructus.imago.pixela == NIHIL)
    {
        _stbi_free(stbi_data);
        fructus.error = chorda_ex_literis("Non possum allocare memoriam", piscina);
        redde fructus;
    }

    memcpy(fructus.imago.pixela, stbi_data, (size_t)pixela_size);
    _stbi_free(stbi_data);

    fructus.imago.latitudo = (i32)lat;
    fructus.imago.altitudo = (i32)alt;
    fructus.successus = VERUM;

    redde fructus;
}

ImagoFructus
imago_caricare_ex_memoria(constans i8* datum, i32 mensura, Piscina* piscina)
{
    ImagoFructus fructus;
    insignatus character* stbi_data;
    integer lat, alt;
    i32 pixela_size;
    constans character* err;

    /* Initiare fructum */
    fructus.successus = FALSUM;
    fructus.imago.pixela = NIHIL;
    fructus.imago.latitudo = 0;
    fructus.imago.altitudo = 0;
    fructus.error.datum = NIHIL;
    fructus.error.mensura = 0;

    si (datum == NIHIL || mensura <= 0 || piscina == NIHIL)
    {
        fructus.error = chorda_ex_literis("Argumenta invalida", piscina);
        redde fructus;
    }

    /* Caricare cum stb_image */
    stbi_data = _stbi_load_mem((constans insignatus character*)datum, (integer)mensura, &lat, &alt);
    si (stbi_data == NIHIL)
    {
        err = _stbi_error();
        si (err != NIHIL)
        {
            fructus.error = chorda_ex_literis(err, piscina);
        }
        alioquin
        {
            fructus.error = chorda_ex_literis("Non possum caricare imaginem", piscina);
        }
        redde fructus;
    }

    /* Copiare ad piscina */
    pixela_size = (i32)(lat * alt * 4);
    fructus.imago.pixela = (i8*)piscina_allocare(piscina, (memoriae_index)pixela_size);
    si (fructus.imago.pixela == NIHIL)
    {
        _stbi_free(stbi_data);
        fructus.error = chorda_ex_literis("Non possum allocare memoriam", piscina);
        redde fructus;
    }

    memcpy(fructus.imago.pixela, stbi_data, (size_t)pixela_size);
    _stbi_free(stbi_data);

    fructus.imago.latitudo = (i32)lat;
    fructus.imago.altitudo = (i32)alt;
    fructus.successus = VERUM;

    redde fructus;
}
