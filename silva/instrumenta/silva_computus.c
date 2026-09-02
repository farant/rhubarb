/* silva_computus.c - imago memoriae et temporis parseris (vide .h) */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_parsare.h"
#include "silva_expandere.h"
#include "silva_c89_oraculum.h"
#include "silva_computus.h"
#include <string.h>
#include <time.h>

interior duplex
_ms (
    clock_t a,
    clock_t b)
{
    redde (duplex)(b - a) * 1000.0 / (duplex)CLOCKS_PER_SEC;
}

b32
silva_computus_metiri (
             constans character* via,
             constans character* fons,
                            i32  mensura,
    constans SilvaComputusClausura* clausura,
                            i32  numerus_clausurae,
                  SilvaComputus* exitus)
{
         Piscina* piscina;
             Xar* lexemata;
    SilvaParsura* parsura;
         clock_t  t0;
         clock_t  t1;
             i32  i;

    si (exitus == NIHIL) redde FALSUM;
    memset(exitus, 0, magnitudo(*exitus));
    si (fons == NIHIL) redde FALSUM;
    exitus->octeti_fontis = mensura;

    /* I. lexatio sola - piscina propria */
    piscina = piscina_generare_dynamicum("computus_lexare",
        SILVA_COMPUTUS_ALVEUS_INITIUM);
    si (piscina == NIHIL) redde FALSUM;
    t0 = clock();
    lexemata = silva_lexare(piscina, fons, mensura, ZEPHYRUM);
    t1 = clock();
    exitus->ms_lexandi = _ms(t0, t1);
    exitus->lexemata_cruda       = lexemata ? xar_numerus(lexemata)
                                            : ZEPHYRUM;
    exitus->usus_lexandi = piscina_summa_usus(piscina);
    exitus->allocationes_lexandi =
        piscina_numerus_allocationum(piscina);
    piscina_destruere(piscina);

    /* II. parsura plena - piscina propria, expansio intra */
    piscina = piscina_generare_dynamicum("computus_parsare",
        SILVA_COMPUTUS_ALVEUS_INITIUM);
    si (piscina == NIHIL) redde FALSUM;
    t0 = clock();
    si (clausura != NIHIL && numerus_clausurae > ZEPHYRUM)
    {
        SilvaExpansio* expansio;

        expansio = silva_expansio_creare(piscina);
        per (i = ZEPHYRUM; i < numerus_clausurae; i++)
        {
            si (clausura[i].textus == NIHIL) perge;
            silva_includendum_praebere(expansio, clausura[i].via,
                clausura[i].textus, clausura[i].mensura);
        }
        parsura = silva_parsare_cum_expansione(piscina, expansio, via,
            fons, mensura, &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    }
    alioquin
    {
        parsura = silva_parsare(piscina, via, fons, mensura,
            &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    }
    t1                   = clock();
    exitus->ms_parsandi  = _ms(t0, t1);
    si (parsura != NIHIL)
    {
        exitus->successus = parsura->successus;
        exitus->lexemata  = parsura->lexemata
                          ? xar_numerus(parsura->lexemata) : ZEPHYRUM;
        exitus->errores   = parsura->numerus_errorum;
    }
    exitus->usus          = piscina_summa_usus(piscina);
    exitus->otiosa        = piscina_summa_inutilis_allocatus(piscina);
    exitus->commissa      = exitus->usus + exitus->otiosa;
    exitus->apex          = piscina_summa_apex_usus(piscina);
    exitus->alvei         = piscina_numerus_alveorum(piscina);
    exitus->allocationes  = piscina_numerus_allocationum(piscina);
    piscina_destruere(piscina);

    redde parsura != NIHIL && parsura->successus;
}
