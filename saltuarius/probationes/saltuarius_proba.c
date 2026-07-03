/* saltuarius_proba.c - Implementatio adiutorum aureorum */

#include "saltuarius_proba.h"
#include "chorda_aedificator.h"
#include "utf8.h"

chorda
saltuarius_proba_linea (Piscina* piscina, TesseraOpus* opus,
    s32 y, s32 x_initium, i32 mensura)
{
    ChordaAedificator* aed;
    chorda fructus;
    i32 k;

    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)mensura * IV + XVI);
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        TesseraCellula cella = tessera_cellulam_legere(opus,
            x_initium + (s32)k, y);

        si (cella.signum == ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(aed, ' ');
        }
        alioquin
        {
            i32 octeti = tessera_signum_mensura(cella.signum);
            i32 o;

            per (o = ZEPHYRUM; o < octeti; o++)
            {
                chorda_aedificator_appendere_character(aed,
                    (character)((cella.signum >> (VIII * o))
                        & 0xFF));
            }
        }
    }
    fructus = chorda_aedificator_spectare(aed);
    /* spatia caudae praecidere */
    dum (fructus.mensura > ZEPHYRUM
        && fructus.datum[fructus.mensura - I] == ' ')
    {
        fructus.mensura--;
    }
    redde fructus;
}

s32
saltuarius_proba_quaerere (TesseraOpus* opus, s32 y,
    constans character* textus)
{
    constans i8* initium = (constans i8*)textus;
    constans i8* finis;
    i32 latitudo = tessera_latitudo(opus);
    s32 x;

    finis = initium;
    dum (*finis != '\0')
    {
        finis++;
    }

    per (x = ZEPHYRUM; x < (s32)latitudo; x++)
    {
        constans i8* cursor = initium;
        s32 columna = x;
        b32 congruit = VERUM;

        dum (cursor < finis)
        {
            constans i8* proxima = utf8_proxima_runa(cursor, finis);
            i32 octeti = (i32)(memoriae_index)(proxima - cursor);
            i32 signum = tessera_signum_ex_octetis(cursor, octeti);
            TesseraCellula cella;

            si (columna >= (s32)latitudo)
            {
                congruit = FALSUM;
                frange;
            }
            cella = tessera_cellulam_legere(opus, columna, y);
            si (cella.signum != signum)
            {
                congruit = FALSUM;
                frange;
            }
            cursor = proxima;
            columna++;
        }
        si (congruit)
        {
            redde x;
        }
    }
    redde -I;
}
