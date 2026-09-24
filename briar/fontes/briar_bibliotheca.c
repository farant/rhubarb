/* briar_bibliotheca.c - vide briar_bibliotheca.h */
#include "briar_bibliotheca.h"

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

chorda
briar_bibliotheca_descriptio (
    chorda textus,
    chorda titulus)
{
    i32 k = ZEPHYRUM;
    i32 finis;
    i32 j;

    /* '/' '*' ' ' */
    si (   textus.mensura < III || textus.datum[0] != '/'
        || textus.datum[I] != '*' || textus.datum[II] != ' ')
    {
        redde _vacua();
    }
    k = III;
    /* titulus capitis */
    si (   textus.mensura < k + titulus.mensura
        || !chorda_aequalis(chorda_sectio(textus, k, k
               + titulus.mensura), titulus))
    {
        redde _vacua();
    }
    k = k + titulus.mensura;
    dum (k < textus.mensura && textus.datum[k] == ' ')
    {
        k++;
    }
    /* separator: '-', ':' aut '—' (UTF-8 e2 80 94) */
    si (   k < textus.mensura
        && (textus.datum[k] == '-' || textus.datum[k] == ':'))
    {
        k++;
    }
    alioquin si (   k + II < textus.mensura
                 && (i32)(textus.datum[k] & 0xFF)      == 0xE2
                 && (i32)(textus.datum[k + I] & 0xFF)  == 0x80
                 && (i32)(textus.datum[k + II] & 0xFF) == 0x94)
    {
        k = k + III;
    }
    alioquin
    {
        redde _vacua();
    }
    dum (k < textus.mensura && textus.datum[k] == ' ')
    {
        k++;
    }
    finis = k;
    dum (   finis < textus.mensura && textus.datum[finis] != '\n'
         && textus.datum[finis] != '\r')
    {
        finis++;
    }
    /* clausura commentarii in eadem linea */
    per (j = k; j + I < finis; j++)
    {
        si (textus.datum[j] == '*' && textus.datum[j + I] == '/')
        {
            finis = j;
            frange;
        }
    }
    dum (finis > k && textus.datum[finis - I] == ' ')
    {
        finis--;
    }
    si (finis <= k)
    {
        redde _vacua();
    }
    redde chorda_sectio(textus, k, finis);
}
