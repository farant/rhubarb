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
_credo_allocare_recordum (
    vacuum)
{
    CredoNotatio* rec;

    si (!_credo_piscina)
    {
        redde NIHIL;
    }

    rec = (CredoNotatio*)piscina_allocare(_credo_piscina, magnitudo(CredoNotatio));
    si (!rec)
    {
        redde NIHIL;
    }

    return rec;
}

interior vacuum
_credo_append (
    CredoNotatio* rec)
{
    si (!rec) redde;

    si (!_credo_primus)
    {
        _credo_primus   = rec;
        _credo_nunc     = rec;
    } 
    alioquin
    {
        _credo_nunc->sequens = rec;
        _credo_nunc          = rec;
    }

    _credo_numerus++;
}

interior chorda
_credo_stringify_integer (
        s32  valor,
    Piscina* piscina)
{
    character buffer[64];
       chorda fructus;

    snprintf(buffer, 64, "%d", valor);
    fructus = chorda_transcribere(chorda_ex_literis(buffer), piscina);

    redde fructus;
}


/* ======================================================
 * Apparatus Globalis
 * ====================================================== */

vacuum
credo_initialize (
    Piscina* piscina)
{
    _credo_piscina  = piscina;
    _credo_primus   = NIHIL;
    _credo_nunc     = NIHIL;
    _credo_numerus  = ZEPHYRUM;
}

vacuum
credo_finalize (
    vacuum)
{
    /* Don't destroy piscina, just reset state */
    _credo_piscina  = NIHIL;
    _credo_primus   = NIHIL;
    _credo_nunc     = NIHIL;
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
    memoriae_index  count   = ZEPHYRUM;
      CredoNotatio* rec     = _credo_primus;

    dum (rec)
    {
        si (rec->passed)
        {
            count++;
        }
        rec = rec->sequens; /* Move to next in linked list */
    }

    redde count;
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
        imprimere("\nStatus: FRACTA\n");
    }
    alioquin
    {
        imprimere("\nStatus: OMNIA PRAETERIERUNT\n");
    }
    imprimere("=========================\n\n");
}


/* ======================================================
 * Recordatio Interni
 * ====================================================== */

vacuum
_credo_record (
    character* type_label,
    character* expression_source,
       chorda  value_a,
       chorda  value_b,
    character* filum,
          s32  versus,
          b32  passed)
{
    CredoNotatio* rec = _credo_allocare_recordum();

    si (!rec) redde;

    rec->type_label         = chorda_transcribere(
                                    chorda_ex_literis(type_label),       
                                    _credo_piscina);
    rec->expression_source  = chorda_transcribere(
                                    chorda_ex_literis(expression_source),
                                    _credo_piscina);
    rec->value_a_chorda     = value_a;
    rec->value_b_chorda     = value_b;
    rec->filum              = filum;
    rec->versus             = versus;
    rec->passed             = passed;
    rec->ordo               = _credo_numerus;

    _credo_append(rec);

    /* Print immediate feedback */
    si (passed)
    {
        imprimere(".");
    }
    alioquin
    {
        imprimere("F");
        imprimere("\n FRACTA: %s at %s:%d\n",
                  chorda_ut_cstr(rec->expression_source, _credo_piscina),
                  filum, versus);
    }
}

vacuum
_credo_record_integralis (
    character* type_label,
    character* expression_source,
          s32  value_a,
          s32  value_b,
    character* filum,
          s32  versus)
{
    chorda a_str = _credo_stringify_integer(value_a, _credo_piscina);
    chorda b_str = _credo_stringify_integer(value_b, _credo_piscina);

    b32 passed = (value_a == value_b);

    _credo_record(type_label, expression_source, a_str, b_str, filum, versus, passed);
}

vacuum
_credo_record_chorda (
    character* type_label,
    character* expression_source,
       chorda  value_a,
       chorda  value_b,
    character* filum,
          s32  versus)
{
    b32 passed = chorda_aequalis(value_a, value_b);

    _credo_record(type_label, expression_source, value_a, 
                  value_b, filum, versus, passed);
}

