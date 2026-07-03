/* saltuarius_liber.c - Implementatio fili aperti */

#include "saltuarius_liber.h"
#include "utf8.h"

SaltuariusLiber*
saltuarius_liber_aperire (Piscina* persistens,
    constans SaltuariusNexus* nexus, chorda via, chorda textus)
{
    SaltuariusLiber* liber;
    memoriae_index initium_arenae;

    liber = (SaltuariusLiber*)piscina_allocare_ordinatum(persistens,
        (memoriae_index)magnitudo(SaltuariusLiber), IV);
    si (liber == NIHIL)
    {
        redde NIHIL;
    }
    initium_arenae = (memoriae_index)textus.mensura * III + 262144;
    liber->piscina = piscina_generare_dynamicum("salt_liber",
        initium_arenae);
    si (liber->piscina == NIHIL)
    {
        redde NIHIL;
    }
    liber->arena_silvae = NIHIL;
    liber->via = chorda_transcribere(via, liber->piscina);
    liber->textus = chorda_transcribere(textus, liber->piscina);
    liber->classis = NIHIL;
    liber->est_colorata = FALSUM;
    liber->cursor_linea = ZEPHYRUM;
    liber->cursor_columna = ZEPHYRUM;
    liber->volumen_y = ZEPHYRUM;
    liber->volumen_x = ZEPHYRUM;

    /* lineae: unus ambulatus */
    {
        i32 numerus = I;
        i32 k;

        per (k = ZEPHYRUM; k < liber->textus.mensura; k++)
        {
            si (liber->textus.datum[k] == '\n')
            {
                numerus++;
            }
        }
        /* plagula vacua = 1 linea vacua; cauda sine \n = ultima */
        liber->lineae = (SaltuariusLinea*)piscina_allocare_ordinatum(
            liber->piscina, (memoriae_index)numerus
                * (memoriae_index)magnitudo(SaltuariusLinea), IV);
        si (liber->lineae == NIHIL)
        {
            piscina_destruere(liber->piscina);
            redde NIHIL;
        }
        liber->numerus_linearum = numerus;
        {
            i32 linea = ZEPHYRUM;
            i32 initium = ZEPHYRUM;

            per (k = ZEPHYRUM; k <= liber->textus.mensura; k++)
            {
                si (k == liber->textus.mensura
                    || liber->textus.datum[k] == '\n')
                {
                    i32 mensura = k - initium;

                    /* \r caudae (CRLF) tondere */
                    si (mensura > ZEPHYRUM
                        && liber->textus.datum[initium + mensura - I]
                            == '\r')
                    {
                        mensura--;
                    }
                    liber->lineae[linea].offset = (i32)initium;
                    liber->lineae[linea].mensura = mensura;
                    linea++;
                    initium = k + I;
                    si (k == liber->textus.mensura)
                    {
                        frange;
                    }
                }
            }
        }
    }

    /* coloratio: .c/.h lexantur */
    si (saltuarius_nexus_est_fons_c(liber->via)
        && liber->textus.mensura > ZEPHYRUM)
    {
        liber->arena_silvae = silva_piscina_generare_dynamicum(
            "salt_liber_silva",
            (memoriae_index)textus.mensura * IV + 1048576);
        si (liber->arena_silvae != NIHIL)
        {
            liber->classis = (i8*)piscina_allocare(liber->piscina,
                (memoriae_index)liber->textus.mensura);
            si (liber->classis != NIHIL)
            {
                saltuarius_nexus_classificare(nexus,
                    liber->arena_silvae, liber->textus,
                    liber->classis);
                liber->est_colorata = VERUM;
            }
        }
    }
    redde liber;
}

vacuum
saltuarius_liber_destruere (SaltuariusLiber* liber)
{
    SilvaPiscina* arena_silvae = liber->arena_silvae;
    Piscina* piscina = liber->piscina;

    /* campos ANTE lectos: structura arenam vocatoris habitat sed
     * contenta in arenis morituris vivunt */
    liber->arena_silvae = NIHIL;
    liber->piscina = NIHIL;
    liber->classis = NIHIL;
    liber->lineae = NIHIL;
    liber->numerus_linearum = ZEPHYRUM;
    si (arena_silvae != NIHIL)
    {
        silva_piscina_destruere(arena_silvae);
    }
    si (piscina != NIHIL)
    {
        piscina_destruere(piscina);
    }
}

chorda
saltuarius_liber_linea (constans SaltuariusLiber* liber, s32 index)
{
    chorda vacua;

    si (index < ZEPHYRUM || index >= (s32)liber->numerus_linearum)
    {
        vacua.mensura = ZEPHYRUM;
        vacua.datum = NIHIL;
        redde vacua;
    }
    redde chorda_sectio(liber->textus,
        (i32)liber->lineae[index].offset,
        (i32)liber->lineae[index].offset
            + liber->lineae[index].mensura);
}

i32
saltuarius_liber_linea_runae (constans SaltuariusLiber* liber,
    s32 index)
{
    chorda linea = saltuarius_liber_linea(liber, index);

    si (linea.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (i32)utf8_numerare_runas(linea.datum,
        (s32)linea.mensura);
}

vacuum
saltuarius_liber_movere (SaltuariusLiber* liber, s32 delta_linea)
{
    s32 nova = liber->cursor_linea + delta_linea;
    i32 runae;

    si (nova < ZEPHYRUM)
    {
        nova = ZEPHYRUM;
    }
    si (nova >= (s32)liber->numerus_linearum)
    {
        nova = (s32)liber->numerus_linearum - I;
    }
    liber->cursor_linea = nova;
    runae = saltuarius_liber_linea_runae(liber, nova);
    si (liber->cursor_columna > (s32)runae)
    {
        liber->cursor_columna = (s32)runae;
    }
}

vacuum
saltuarius_liber_movere_col (SaltuariusLiber* liber, s32 delta)
{
    s32 nova = liber->cursor_columna + delta;
    i32 runae = saltuarius_liber_linea_runae(liber,
        liber->cursor_linea);

    si (nova < ZEPHYRUM)
    {
        nova = ZEPHYRUM;
    }
    si (nova > (s32)runae)
    {
        nova = (s32)runae;
    }
    liber->cursor_columna = nova;
}

vacuum
saltuarius_liber_primum (SaltuariusLiber* liber)
{
    liber->cursor_linea = ZEPHYRUM;
    liber->cursor_columna = ZEPHYRUM;
}

vacuum
saltuarius_liber_ultimum (SaltuariusLiber* liber)
{
    si (liber->numerus_linearum > ZEPHYRUM)
    {
        liber->cursor_linea = (s32)liber->numerus_linearum - I;
        liber->cursor_columna = ZEPHYRUM;
    }
}

vacuum
saltuarius_liber_aptare (SaltuariusLiber* liber,
    i32 altitudo_fenestrae, i32 latitudo_fenestrae)
{
    si (altitudo_fenestrae > ZEPHYRUM)
    {
        si (liber->cursor_linea < liber->volumen_y)
        {
            liber->volumen_y = liber->cursor_linea;
        }
        si (liber->cursor_linea
            >= liber->volumen_y + (s32)altitudo_fenestrae)
        {
            liber->volumen_y = liber->cursor_linea
                - (s32)altitudo_fenestrae + I;
        }
        si (liber->volumen_y < ZEPHYRUM)
        {
            liber->volumen_y = ZEPHYRUM;
        }
    }
    si (latitudo_fenestrae > ZEPHYRUM)
    {
        si (liber->cursor_columna < liber->volumen_x)
        {
            liber->volumen_x = liber->cursor_columna;
        }
        si (liber->cursor_columna
            >= liber->volumen_x + (s32)latitudo_fenestrae)
        {
            liber->volumen_x = liber->cursor_columna
                - (s32)latitudo_fenestrae + I;
        }
        si (liber->volumen_x < ZEPHYRUM)
        {
            liber->volumen_x = ZEPHYRUM;
        }
    }
}
