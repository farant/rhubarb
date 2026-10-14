/* rivus_eventuum.c - Server-Sent Events, lector incrementalis (vide
 * caput; quaestio-rivus T3) */
#include "rivus_eventuum.h"
#include "chorda_aedificator.h"
#include <string.h>

structura RivusEventuum {
                 Piscina* piscina;
    RivusEventuumFunctio  functio;
                  vacuum* datum;
       ChordaAedificator* linea;
       ChordaAedificator* data;
       ChordaAedificator* eventus;
                     b32  data_visa;
                     b32  cr_pendens;    /* CR ultimus: LF proximus
                                          * neglegitur */
                     i32 nota_visa;      /* octeti BOM initiales visi */
                     b32 nota_finita;   /* initium iudicatum */
                     i32 numerus;
};

hic_manens constans i8 RIVUS_NOTA_ORDINIS[III] = { (i8)0xEF, (i8)0xBB,
    (i8)0xBF };

RivusEventuum*
rivus_eventuum_creare (
                 Piscina* piscina,
    RivusEventuumFunctio  functio,
                  vacuum* datum)
{
    RivusEventuum* r;

    si (!piscina)
    {
        redde NIHIL;
    }
    r = (RivusEventuum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(RivusEventuum));
    memset(r, ZEPHYRUM, magnitudo(RivusEventuum));
    r->piscina  = piscina;
    r->functio  = functio;
    r->datum    = datum;
    r->linea    = chorda_aedificator_creare(piscina, CXXVIII);
    r->data     = chorda_aedificator_creare(piscina, CXXVIII);
    r->eventus  = chorda_aedificator_creare(piscina, XXXII);
    redde r;
}

/* linea vacua: eventus (si data visa) functioni datur, deinde omnia
 * purgantur */
interior vacuum
_eventum_emittere (
    RivusEventuum* r)
{
    chorda data;
    chorda eventus;

    si (r->data_visa)
    {
        data = chorda_aedificator_spectare(r->data);
        si (   data.mensura > ZEPHYRUM
            && data.datum[data.mensura - I] == (i8)'\n')
        {
            data.mensura--;
        }
        eventus = chorda_aedificator_spectare(r->eventus);
        si (eventus.mensura == ZEPHYRUM)
        {
            eventus = chorda_ex_literis("message", r->piscina);
        }
        r->numerus++;
        si (r->functio)
        {
            r->functio(r->datum, eventus, data);
        }
    }
    chorda_aedificator_reset(r->data);
    chorda_aedificator_reset(r->eventus);
    r->data_visa = FALSUM;
}

/* linea integra (sine fine): campus et valor */
interior vacuum
_lineam_tractare (
    RivusEventuum* r)
{
    chorda linea;
    chorda campus;
    chorda valor;
       i32 k;

    linea = chorda_aedificator_spectare(r->linea);
    si (linea.mensura == ZEPHYRUM)
    {
        _eventum_emittere(r);
        redde;
    }
    si (linea.datum[ZEPHYRUM] == (i8)':')
    {
        chorda_aedificator_reset(r->linea);
        redde;
    }
    k = ZEPHYRUM;
    dum (k < linea.mensura && linea.datum[k] != (i8)':')
    {
        k++;
    }
    campus.datum    = linea.datum;
    campus.mensura  = k;
    si (k < linea.mensura)
    {
        valor.datum    = linea.datum + k + I;
        valor.mensura  = linea.mensura - k - I;
    }
    alioquin
    {
        valor.datum    = linea.datum + k;
        valor.mensura  = ZEPHYRUM;
    }
    si (valor.mensura > ZEPHYRUM && valor.datum[ZEPHYRUM] == (i8)' ')
    {
        valor.datum++;
        valor.mensura--;
    }
    si (chorda_aequalis_literis(campus, "data"))
    {
        chorda_aedificator_appendere_chorda(r->data, valor);
        chorda_aedificator_appendere_character(r->data, '\n');
        r->data_visa = VERUM;
    }
    alioquin si (chorda_aequalis_literis(campus, "event"))
    {
        chorda_aedificator_reset(r->eventus);
        chorda_aedificator_appendere_chorda(r->eventus, valor);
    }
    /* id, retry, ignota: neglecta */
    chorda_aedificator_reset(r->linea);
}

/* octetus unus post iudicium BOM */
interior vacuum
_octetum_tractare (
    RivusEventuum* r,
               i8  c)
{
    si (r->cr_pendens)
    {
        r->cr_pendens = FALSUM;
        si (c == (i8)'\n')
        {
            redde;
        }
    }
    si (c == (i8)'\r')
    {
        _lineam_tractare(r);
        r->cr_pendens = VERUM;
    }
    alioquin si (c == (i8)'\n')
    {
        _lineam_tractare(r);
    }
    alioquin
    {
        chorda_aedificator_appendere_character(r->linea, (character)c);
    }
}

vacuum
rivus_eventuum_adicere (
     RivusEventuum* r,
       constans i8* octeti,
               i32  n)
{
    i32 i;
    i32 k;

    si (!r || !octeti)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!r->nota_finita)
        {
            si (octeti[i] == RIVUS_NOTA_ORDINIS[r->nota_visa])
            {
                r->nota_visa++;
                si (r->nota_visa == III)
                {
                    r->nota_finita = VERUM;
                }
                perge;
            }
            /* praefixum BOM sine fine: octeti eius textus sunt */
            r->nota_finita = VERUM;
            per (k = ZEPHYRUM; k < r->nota_visa; k++)
            {
                _octetum_tractare(r, RIVUS_NOTA_ORDINIS[k]);
            }
        }
        _octetum_tractare(r, octeti[i]);
    }
}

vacuum
rivus_eventuum_finire (
    RivusEventuum* r)
{
    si (!r)
    {
        redde;
    }
    /* linea imperfecta et eventus sine linea vacua: abiciuntur */
    chorda_aedificator_reset(r->linea);
    chorda_aedificator_reset(r->data);
    chorda_aedificator_reset(r->eventus);
    r->data_visa   = FALSUM;
    r->cr_pendens  = FALSUM;
}

i32
rivus_eventuum_numerus (
    constans RivusEventuum* r)
{
    redde r ? r->numerus : ZEPHYRUM;
}
