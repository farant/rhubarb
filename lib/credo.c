/* credo.c */
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>

/* ======================================================
 * Globalis Status
 * ====================================================== */

universalis        Piscina* _credo_piscina  = NIHIL;
universalis   CredoNotatio* _credo_primus   = NIHIL;
universalis   CredoNotatio* _credo_nunc     = NIHIL;
universalis memoriae_index  _credo_numerus  = ZEPHYRUM;

/* ======================================================
 * Adiutores Interni
 * ====================================================== */

interior CredoNotatio*
_credo_allocare_notatio (
    vacuum)
{
    CredoNotatio* notatio;

    si (!_credo_piscina)
    {
        redde NIHIL;
    }

    notatio = (CredoNotatio*)piscina_allocare(_credo_piscina, magnitudo(CredoNotatio));
    si (!notatio)
    {
        redde NIHIL;
    }

    return notatio;
}

interior vacuum
_credo_concatenare (
    CredoNotatio* notatio)
{
    si (!notatio) redde;

    si (!_credo_primus)
    {
        _credo_primus   = notatio;
        _credo_nunc     = notatio;
    } 
    alioquin
    {
        _credo_nunc->sequens = notatio;
        _credo_nunc          = notatio;
    }

    _credo_numerus++;
}


/* ======================================================
 * Apparatus Globalis
 * ====================================================== */

vacuum
credo_aperire (
    Piscina* piscina)
{
    _credo_piscina  = piscina;
    _credo_primus   = NIHIL;
    _credo_nunc     = NIHIL;
    _credo_numerus  = ZEPHYRUM;
}

vacuum
credo_claudere (
    vacuum)
{
    /* Piscina manet; conditio solum reficitur */
    _credo_piscina  = NIHIL;
    _credo_primus   = NIHIL;
    _credo_nunc     = NIHIL;
    _credo_numerus  = ZEPHYRUM;
}

/* ======================================================
 * Interrogatio
 * ====================================================== */

memoriae_index
credo_numerus_totalis (
    vacuum)
{
    redde _credo_numerus;
}

memoriae_index
credo_numerus_praeteriti (
    vacuum)
{
    memoriae_index  summa   = ZEPHYRUM;
      CredoNotatio* notatio     = _credo_primus;

    dum (notatio)
    {
        si (notatio->praeteritus)
        {
            summa++;
        }
        notatio = notatio->sequens; /* Procedere in catena ad sequens */
    }

    redde summa;
}

memoriae_index 
credo_numerus_fracti (
    vacuum)
{
    redde _credo_numerus - credo_numerus_praeteriti();
}


b32 
credo_omnia_praeterierunt (
    vacuum)
{
    redde credo_numerus_fracti() == ZEPHYRUM;
}


vacuum
credo_imprimere_compendium (
    vacuum)
{
    memoriae_index totalis    = credo_numerus_totalis();
    memoriae_index praeteriti = credo_numerus_praeteriti();
    memoriae_index fracti     = credo_numerus_fracti();

    imprimere("\n");
    imprimere("=== CREDO COMPENDIUM ===\n");
    imprimere("Totalis:    %zu\n", totalis);
    imprimere("Praeteriti: %zu\n", praeteriti);
    imprimere("Fracti:     %zu\n", fracti);

    si (fracti > ZEPHYRUM)
    {
        imprimere("\nConditio: FRACTA\n");
    }
    alioquin
    {
        imprimere("\nConditio: OMNIA PRAETERIERUNT\n");
    }
    imprimere("=========================\n\n");
}


/* ======================================================
 * Recordatio Interni
 * ====================================================== */

vacuum
_credo_notare (
    constans character* genus,
    constans character* expressio,
    constans character* valor_primus,
    constans character* valor_secundus,
    constans character* filum,
                   s32  versus,
                   b32  praeteritus)
{
    CredoNotatio* notatio = _credo_allocare_notatio();

    si (!notatio) redde;

    notatio->genus          = chorda_ex_literis(genus,          _credo_piscina);   
    notatio->expressio      = chorda_ex_literis(expressio,      _credo_piscina);
    notatio->valor_primus   = chorda_ex_literis(valor_primus,   _credo_piscina);
    notatio->valor_secundus = chorda_ex_literis(valor_secundus, _credo_piscina);
    notatio->filum          = filum;
    notatio->versus         = versus;
    notatio->praeteritus    = praeteritus;
    notatio->ordo           = _credo_numerus;

    _credo_concatenare(notatio);

    /* Signum statim */
    si (praeteritus)
    {
        imprimere(".");
    }
    alioquin
    {
        imprimere("F");
        imprimere("\n FRACTA (%s): %s at %s:%d\n",
                  chorda_ut_cstr(notatio->genus, _credo_piscina),
                  chorda_ut_cstr(notatio->expressio, _credo_piscina),
                  filum, versus);
        imprimere("  Speratus: %s\n",
                  chorda_ut_cstr(notatio->valor_secundus, _credo_piscina));
        imprimere("  Receptus: %s\n",
                  chorda_ut_cstr(notatio->valor_primus, _credo_piscina));
    }
}

vacuum
_credo_notare_integrum (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus == valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus == valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_inaequalitas (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus != valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_inaequalitas_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus != valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_chorda (
    constans character* genus,
    constans character* expressio,
    constans character* valor_primus,
    constans character* valor_secundus,
    constans character* filum,
                   s32  versus)
{
    b32 praeteritus;
    chorda chorda_primus;
    chorda chorda_secundus;

    chorda_primus   = chorda_ex_literis(valor_primus, _credo_piscina);
    chorda_secundus = chorda_ex_literis(valor_secundus, _credo_piscina);

    praeteritus = chorda_aequalis(chorda_primus, chorda_secundus);

    _credo_notare(genus, expressio, valor_primus, 
                  valor_secundus, filum, versus, praeteritus);
}

