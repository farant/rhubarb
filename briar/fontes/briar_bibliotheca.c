/* briar_bibliotheca.c - vide briar_bibliotheca.h */
#include "briar_bibliotheca.h"
#include "chorda_aedificator.h"
#include <string.h>

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

#define TECTUM_TYPI  XXIV

/* positio nominis ut verbi TOTIUS ante '(' (spatiis licet): ante
 * id initium, ' ' aut '*'; -1 si non inventum */
interior s32
_nomen_quaerere (
    chorda linea,
    chorda titulus)
{
    i32 k;

    si (titulus.mensura == ZEPHYRUM || titulus.mensura > linea.mensura)
    {
        redde (s32)-I;
    }
    per (k = ZEPHYRUM; k + titulus.mensura <= linea.mensura; k++)
    {
        i32 post = k + titulus.mensura;

        si (memcmp(linea.datum + k, titulus.datum,
                (size_t)titulus.mensura) != ZEPHYRUM)
        {
            perge;
        }
        si (   k > ZEPHYRUM && linea.datum[k - I] != (i8)' '
            && linea.datum[k - I] != (i8)'*')
        {
            perge;
        }
        dum (post < linea.mensura && linea.datum[post] == (i8)' ')
        {
            post++;
        }
        si (post < linea.mensura && linea.datum[post] == (i8)'(')
        {
            redde (s32)k;
        }
    }
    redde (s32)-I;
}

/* longitudo typi (textus ante nomen, spatiis finalibus demptis) */
interior i32
_typi_longitudo (
    chorda linea,
       s32 positio)
{
    i32 n = (i32)positio;

    dum (n > ZEPHYRUM && linea.datum[n - I] == (i8)' ')
    {
        n--;
    }
    redde n;
}

chorda
briar_bibliotheca_functiones (
    constans chorda* lineae,
    constans chorda* tituli,
                i32  numerus,
            Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);
                   i32 latitudo = ZEPHYRUM;
                   i32 i;

    si (aed == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        s32 p = _nomen_quaerere(lineae[i], tituli[i]);
        i32 t;

        si (p < ZEPHYRUM)
        {
            perge;
        }
        t = _typi_longitudo(lineae[i], p);
        si (t > latitudo && t <= (i32)TECTUM_TYPI)
        {
            latitudo = t;
        }
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        s32 p = _nomen_quaerere(lineae[i], tituli[i]);

        si (p < ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(aed, lineae[i]);
        }
        alioquin
        {
            chorda typus;
            chorda reliquum;
               i32 k;

            typus.datum       = lineae[i].datum;
            typus.mensura     = _typi_longitudo(lineae[i], p);
            reliquum.datum    = lineae[i].datum + p;
            reliquum.mensura  = lineae[i].mensura - (i32)p;
            chorda_aedificator_appendere_chorda(aed, typus);
            per (k = typus.mensura; k < latitudo; k++)
            {
                chorda_aedificator_appendere_character(aed, ' ');
            }
            si (typus.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(aed, ' ');
            }
            chorda_aedificator_appendere_chorda(aed, reliquum);
        }
        chorda_aedificator_appendere_character(aed, '\n');
    }
    redde chorda_aedificator_finire(aed);
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
