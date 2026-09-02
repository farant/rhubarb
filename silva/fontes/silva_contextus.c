/* silva_contextus.c - Implementatio contextus hospitis (Phase 7) */

#include "silva_contextus.h"
#include "silva_expandere.h"   /* defaltae finium */
#include "silva_glr.h"         /* defaltae frontis/passus */
#include "silva_latina_datum.h"
#include <string.h>

/* Copia cstring in piscinam (contextus vocatorem supervivit -
 * lectio vitae fons->via) */
interior constans character*
_literis_figere (
               Piscina* piscina,
    constans character* fons)
{
    memoriae_index  mensura;
         character* novum;

    si (fons == NIHIL)
    {
        redde NIHIL;
    }
    mensura  = strlen(fons) + I;
    novum    = (character*)piscina_allocare(piscina, mensura);
    si (novum == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(novum, fons, mensura);
    redde novum;
}

interior b32
_plagulam_addere (
        SilvaContextus* contextus,
                   Xar* quo,
    constans character* via,
    constans character* textus,
                   i32  mensura)
{
    SilvaContextusPlagula* locus;
       constans character* via_fixa;
                character* textus_fixus;

    si (   contextus == NIHIL || quo == NIHIL || via == NIHIL
        || textus    == NIHIL)
    {
        redde FALSUM;
    }
    via_fixa = _literis_figere(contextus->piscina, via);
    si (via_fixa == NIHIL)
    {
        redde FALSUM;
    }
    textus_fixus = (character*)piscina_allocare(contextus->piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (textus_fixus == NIHIL)
    {
        redde FALSUM;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(textus_fixus, textus, (memoriae_index)mensura);
    }

    locus = (SilvaContextusPlagula*)xar_addere(quo);
    si (locus == NIHIL)
    {
        redde FALSUM;
    }
    locus->via      = via_fixa;
    locus->textus   = textus_fixus;
    locus->mensura  = mensura;
    redde VERUM;
}

SilvaContextus*
silva_contextus_creare (
    Piscina* piscina)
{
    SilvaContextus* contextus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    contextus = (SilvaContextus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaContextus));
    si (contextus == NIHIL)
    {
        redde NIHIL;
    }
    contextus->piscina             = piscina;
    contextus->fines.lexemata      = SILVA_LIMEN_LEXEMATUM_DEFALTUM;
    contextus->fines.generationes  = SILVA_LIMEN_GENERATIONUM_DEFALTUM;
    contextus->fines.profunditas_includendi =
        SILVA_LIMEN_INCLUDENDI_DEFALTUM;
    contextus->fines.profunditas_regionum =
        SILVA_LIMEN_REGIONUM_DEFALTUM;
    contextus->fines.frons      = SILVA_GLR_LIMEN_FRONTIS_DEFALTUM;
    contextus->pergere          = NIHIL;
    contextus->pergere_datum    = NIHIL;
    contextus->passus_pergendi  = SILVA_GLR_PASSUS_PERGENDI_DEFALTUM;
    contextus->lexica = xar_creare(piscina,
        magnitudo(SilvaContextusPlagula));
    contextus->praebenda = xar_creare(piscina,
        magnitudo(SilvaContextusPlagula));
    si (contextus->lexica == NIHIL || contextus->praebenda == NIHIL)
    {
        redde NIHIL;
    }
    redde contextus;
}

b32
silva_contextus_lexicon_addere (
        SilvaContextus* contextus,
    constans character* via,
    constans character* textus,
                   i32  mensura)
{
    si (contextus == NIHIL)
    {
        redde FALSUM;
    }
    redde _plagulam_addere(contextus, contextus->lexica, via, textus,
        mensura);
}

b32
silva_contextus_latinam_addere (
    SilvaContextus* contextus)
{
    redde silva_contextus_lexicon_addere(contextus, "latina.h",
        silva_latina_textus, silva_latina_mensura);
}

b32
silva_contextus_praebere (
        SilvaContextus* contextus,
    constans character* via,
    constans character* textus,
                   i32  mensura)
{
    si (contextus == NIHIL)
    {
        redde FALSUM;
    }
    redde _plagulam_addere(contextus, contextus->praebenda, via,
        textus, mensura);
}

vacuum
silva_contextus_pergere_ponere (
         SilvaContextus* contextus,
    SilvaPergereFunctio  pergere,
                 vacuum* datum,
                    i32  passus)
{
    si (contextus == NIHIL)
    {
        redde;
    }
    contextus->pergere          = pergere;
    contextus->pergere_datum    = datum;
    contextus->passus_pergendi  = passus;
}
