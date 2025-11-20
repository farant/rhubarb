#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Functiones Friationis
 * ================================================== */

i32
tabula_friare_fnv1a(chorda clavis)
{
    i32 friatum = 2166136261U;
    i32 i;

    per (i = ZEPHYRUM; i < clavis.mensura; i++)
    {
        friatum ^= (i8)clavis.datum[i];
        friatum *= 16777619U;
    }

    redde friatum;
}

/* Friare literis C directe (pro evitare const cast) */
interior i32
_friare_literis(constans character* cstr)
{
    i32 friatum = 2166136261U;
    constans character* p = cstr;

    dum (*p)
    {
        friatum ^= (i8)*p;
        friatum *= 16777619U;
        p++;
    }

    redde friatum;
}

/* Comparare literis C cum chorda */
interior b32
_aequalis_literis_chorda(constans character* cstr, chorda clavis)
{
    i32 mensura_cstr;
    i32 i;

    mensura_cstr = (i32)strlen(cstr);

    si (mensura_cstr != clavis.mensura)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < mensura_cstr; i++)
    {
        si (cstr[i] != (character)clavis.datum[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* ==================================================
 * Adiutor: Proxima Potentia Binaria
 * ================================================== */

interior i32
_proxima_potentia_2(i32 n)
{
    n--;
    n |= n >> I;
    n |= n >> II;
    n |= n >> IV;
    n |= n >> VIII;
    n |= n >> XVI;
    n++;
    redde n;
}

/* ==================================================
 * Functiones Friationis
 * ================================================== */

i32
tabula_friare_multiplicatio(chorda clavis)
{
    i32 friatum = 5381U;
    i32 i;

    per (i = ZEPHYRUM; i < clavis.mensura; i++)
    {
        friatum = ((friatum << V) + friatum) + (i8)clavis.datum[i];
    }

    redde friatum;
}



/* ==================================================
 * Adiutor: Invenire Slotum
 * Redde positus sloti, statuit vexillum *inventum
 * ================================================== */

interior i32
_invenire_slotum (
    TabulaDispersa* tabula,
            chorda  clavis,
               i32  friatum,
               b32* inventum)
{
    i32 positus;
    i32 distantia;
    i32 positus_primus_deletum;

    *inventum              = FALSUM;
    positus                = friatum & tabula->capacitas_mask;
    distantia              = ZEPHYRUM;
    positus_primus_deletum = (i32)-I;

    dum (tabula->sloti[positus].status != SLOT_VACUUM)
    {
        si (tabula->sloti[positus].status == SLOT_OCCUPATUM)
        {
            si (tabula->sloti[positus].hash_cachatum == friatum &&
                tabula->comparatio(
                    tabula->sloti[positus].clavis, clavis) == ZEPHYRUM)
            {
                *inventum = VERUM;
                redde positus;
            }
            tabula->collisiones_totales++;
        }
        alioquin si (tabula->sloti[positus].status == SLOT_DELETUM)
        {
            si (positus_primus_deletum == (i32)-I)
            {
                positus_primus_deletum = positus;
            }
        }

        positus = (positus + I) & tabula->capacitas_mask;
        distantia++;

        si (distantia > tabula->distantia_maxima)
        {
            tabula->distantia_maxima = distantia;
        }

        si (distantia > tabula->capacitas)
        {
            frange;
        }
    }

    si (positus_primus_deletum != (i32)-I)
    {
        redde positus_primus_deletum;
    }

    redde positus;
}

/* Invenire slotum pro literis C (evitare const cast) */
interior i32
_invenire_slotum_literis (
    TabulaDispersa*     tabula,
    constans character* cstr,
               i32      friatum,
               b32*     inventum)
{
    i32 positus;
    i32 distantia;
    i32 positus_primus_deletum;

    *inventum              = FALSUM;
    positus                = friatum & tabula->capacitas_mask;
    distantia              = ZEPHYRUM;
    positus_primus_deletum = (i32)-I;

    dum (tabula->sloti[positus].status != SLOT_VACUUM)
    {
        si (tabula->sloti[positus].status == SLOT_OCCUPATUM)
        {
            si (tabula->sloti[positus].hash_cachatum == friatum &&
                _aequalis_literis_chorda(cstr, tabula->sloti[positus].clavis))
            {
                *inventum = VERUM;
                redde positus;
            }
            tabula->collisiones_totales++;
        }
        alioquin si (tabula->sloti[positus].status == SLOT_DELETUM)
        {
            si (positus_primus_deletum == (i32)-I)
            {
                positus_primus_deletum = positus;
            }
        }

        positus = (positus + I) & tabula->capacitas_mask;
        distantia++;

        si (distantia > tabula->distantia_maxima)
        {
            tabula->distantia_maxima = distantia;
        }

        si (distantia > tabula->capacitas)
        {
            frange;
        }
    }

    si (positus_primus_deletum != (i32)-I)
    {
        redde positus_primus_deletum;
    }

    redde positus;
}


/* ==================================================
 * Crescentia
 * ================================================== */

interior b32
_tabula_dispersa_crescere(TabulaDispersa* tabula)
{
    Slotus* sloti_veteres;
       i32  capacitas_vetus;
       i32  i;

    sloti_veteres   = tabula->sloti;
    capacitas_vetus = tabula->capacitas;

    tabula->capacitas      = capacitas_vetus * II;
    tabula->capacitas_mask = tabula->capacitas - I;

    tabula->sloti = (Slotus*)piscina_allocare(
        tabula->piscina,
        tabula->capacitas * magnitudo(Slotus));

    si (!tabula->sloti)
    {
        tabula->sloti          = sloti_veteres;
        tabula->capacitas      = capacitas_vetus;
        tabula->capacitas_mask = tabula->capacitas - I;
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < tabula->capacitas; i++)
    {
        tabula->sloti[i].status = SLOT_VACUUM;
    }

    tabula->numerus = ZEPHYRUM;
    tabula->numerus_deletorum = ZEPHYRUM;
    tabula->collisiones_totales = ZEPHYRUM;
    tabula->distantia_maxima = ZEPHYRUM;

    per (i = ZEPHYRUM; i < capacitas_vetus; i++)
    {
        si (sloti_veteres[i].status == SLOT_OCCUPATUM)
        {
            tabula_dispersa_inserere(
                tabula,
                sloti_veteres[i].clavis,
                sloti_veteres[i].valor);
        }
    }

    redde VERUM;
}

/* ==================================================
 * Creatio
 * ================================================== */

TabulaDispersa*
tabula_dispersa_creare(
             Piscina* piscina,
                 i32  capacitas_initialis,
       TabulaFriatio  friatio,
    TabulaComparatio  comparatio)
{
    TabulaDispersa* tabula;
               i32  capacitas;
               i32  i;

    si (!piscina || !friatio || !comparatio)
    {
        redde NIHIL;
    }

    capacitas = _proxima_potentia_2(capacitas_initialis);
    si (capacitas < XVI) capacitas = XVI;

    tabula = (TabulaDispersa*)piscina_allocare(
                                piscina, 
                                magnitudo(TabulaDispersa));
    si (!tabula) redde NIHIL;

    tabula->sloti = (Slotus*)piscina_allocare(
                                piscina,
                                capacitas * magnitudo(Slotus));
    si (!tabula->sloti) redde NIHIL;

    per (i = ZEPHYRUM; i < capacitas; i++)
    {
        tabula->sloti[i].status         = SLOT_VACUUM;
        tabula->sloti[i].clavis.mensura = ZEPHYRUM;
        tabula->sloti[i].clavis.datum   = NIHIL;
        tabula->sloti[i].valor          = NIHIL;
        tabula->sloti[i].hash_cachatum  = ZEPHYRUM;
    }

    tabula->capacitas                = capacitas;
    tabula->capacitas_mask           = capacitas - I;
    tabula->numerus                  = ZEPHYRUM;
    tabula->numerus_deletorum        = ZEPHYRUM;
    tabula->piscina                  = piscina;
    tabula->comparatio               = comparatio;
    tabula->friatio                  = friatio;
    tabula->factor_maximus           = 0.75f;
    tabula->factor_deletorum_maximus = 0.25f;
    tabula->collisiones_totales      = ZEPHYRUM;
    tabula->distantia_maxima         = ZEPHYRUM;

    redde tabula;
}


TabulaDispersa*
tabula_dispersa_creare_chorda(
    Piscina* piscina,
        i32  capacitas_initialis)
{
    redde tabula_dispersa_creare(
        piscina,
        capacitas_initialis,
        tabula_friare_fnv1a,
        (TabulaComparatio)chorda_comparare);
}


/* ==================================================
 * Insertio
 * ================================================== */

b32
tabula_dispersa_inserere(
    TabulaDispersa* tabula,
            chorda  clavis,
            vacuum* valor)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Confer factores oneris */
    si (tabula_dispersa_factor_oneris(tabula) > tabula->factor_maximus ||
        tabula_dispersa_factor_deletorum(tabula) > tabula->factor_deletorum_maximus)
    {
        si (!_tabula_dispersa_crescere(tabula))
        {
            redde FALSUM;
        }
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        /* Renovatio existentis */
        tabula->sloti[positus].valor = valor;
        redde VERUM;
    }

    /* Insertio nova */
    si (tabula->sloti[positus].status == SLOT_DELETUM)
    {
        tabula->numerus_deletorum--;
    }

    tabula->sloti[positus].clavis          = clavis;
    tabula->sloti[positus].valor           = valor;
    tabula->sloti[positus].hash_cachatum   = friatum;
    tabula->sloti[positus].status          = SLOT_OCCUPATUM;
    tabula->numerus++;

    redde VERUM;
}


/* ==================================================
 * Quaestio
 * ================================================== */

b32
tabula_dispersa_invenire(
    TabulaDispersa*  tabula,
            chorda   clavis,
            vacuum** valor_out)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        si (valor_out)
        {
            *valor_out = tabula->sloti[positus].valor;
        }
        redde VERUM;
    }

    redde FALSUM;
}

b32
tabula_dispersa_continet(
    TabulaDispersa* tabula,
            chorda  clavis)
{
    redde tabula_dispersa_invenire(tabula, clavis, NIHIL);
}

b32
tabula_dispersa_invenire_literis(
    TabulaDispersa*        tabula,
    constans character*    cstr,
            vacuum**       valor_out)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || !cstr)
    {
        redde FALSUM;
    }

    /* Friare literis directe */
    friatum = _friare_literis(cstr);

    /* Invenire slotum (comparare literis directe) */
    positus = _invenire_slotum_literis(tabula, cstr, friatum, &inventum);

    si (inventum)
    {
        si (valor_out)
        {
            *valor_out = tabula->sloti[positus].valor;
        }
        redde VERUM;
    }

    redde FALSUM;
}

b32
tabula_dispersa_continet_literis(
    TabulaDispersa*     tabula,
    constans character* cstr)
{
    redde tabula_dispersa_invenire_literis(tabula, cstr, NIHIL);
}

/* ==================================================
 * Deletio
 * ================================================== */

b32
tabula_dispersa_delere(
    TabulaDispersa* tabula,
            chorda  clavis)
{
    i32 friatum;
    i32 positus;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    friatum = tabula->friatio(clavis);
    positus = _invenire_slotum(tabula, clavis, friatum, &inventum);

    si (inventum)
    {
        tabula->sloti[positus].status = SLOT_DELETUM;
        tabula->numerus--;
        tabula->numerus_deletorum++;
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * Cyclus Vitae
 * ================================================== */

vacuum
tabula_dispersa_vacare(TabulaDispersa* tabula)
{
    i32 i;

    si (!tabula) redde;

    per (i = ZEPHYRUM; i < tabula->capacitas; i++)
    {
        tabula->sloti[i].status         = SLOT_VACUUM;
        tabula->sloti[i].clavis.mensura = ZEPHYRUM;
        tabula->sloti[i].clavis.datum   = NIHIL;
        tabula->sloti[i].valor          = NIHIL;
    }

    tabula->numerus             = ZEPHYRUM;
    tabula->numerus_deletorum   = ZEPHYRUM;
    tabula->collisiones_totales = ZEPHYRUM;
    tabula->distantia_maxima    = ZEPHYRUM;
}

/* ==================================================
 * Iterator
 * ================================================== */

TabulaIterator
tabula_dispersa_iterator_initium(TabulaDispersa* tabula)
{
    TabulaIterator iter;
    iter.tabula        = tabula;
    iter.positus_currens = ZEPHYRUM;
    redde iter;
}

b32
tabula_dispersa_iterator_proximum(
    TabulaIterator*  iter,
            chorda*  clavis_out,
            vacuum** valor_out)
{
    si (!iter || !iter->tabula) redde FALSUM;

    dum (iter->positus_currens < iter->tabula->capacitas)
    {
        si (iter->tabula->sloti[iter->positus_currens].status == SLOT_OCCUPATUM)
        {
            si (clavis_out)
            {
                *clavis_out = iter->tabula->sloti[iter->positus_currens].clavis;
            }
            si (valor_out)
            {
                *valor_out  = iter->tabula->sloti[iter->positus_currens].valor;
            }
            iter->positus_currens++;
            redde VERUM;
        }
        iter->positus_currens++;
    }

    redde FALSUM;
}


/* ==================================================
 * Statisticae
 * ================================================== */

f32
tabula_dispersa_factor_oneris(TabulaDispersa* tabula)
{
    si (!tabula || tabula->capacitas == ZEPHYRUM)
    {
        redde 0.0f;
    }
    redde (f32)tabula->numerus / (f32)tabula->capacitas;
}

f32
tabula_dispersa_factor_deletorum(TabulaDispersa* tabula)
{
    si (!tabula || tabula->capacitas == ZEPHYRUM)
    {
        redde 0.0f;
    }
    redde (f32)tabula->numerus_deletorum / (f32)tabula->capacitas;
}

i32
tabula_dispersa_numerus(TabulaDispersa* tabula)
{
    redde tabula ? tabula->numerus : ZEPHYRUM;
}

vacuum
tabula_dispersa_status_imprimere(TabulaDispersa* tabula)
{
    si (!tabula)
    {
        imprimere("Tabula Dispersa: NIHIL\n");
        redde;
    }

    imprimere("Status Tabulae Dispersae:\n");
    imprimere("  Capacitas: %u\n", tabula->capacitas);
    imprimere("  Numerus: %u\n", tabula->numerus);
    imprimere("  Numerus deletorum: %u\n", tabula->numerus_deletorum);
    imprimere("  Factor oneris: %.2f%%\n", tabula_dispersa_factor_oneris(tabula) * C);
    imprimere("  Factor deletorum: %.2f%%\n",
                tabula_dispersa_factor_deletorum(tabula) * C);
    imprimere("  Collisiones totales: %u\n", tabula->collisiones_totales);
    imprimere("  Distantia maxima: %u\n", tabula->distantia_maxima);
}
