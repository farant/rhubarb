#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Hash functions
 * ================================================== */

i32
tabula_friare_fnv1a(chorda clavis)
{
    i32 hash = 2166136261U;
    i32 i;

    per (i = ZEPHYRUM; i < clavis.mensura; i++) 
    {
        hash ^= (i8)clavis.datum[i];
        hash *= 16777619U;
    }

    redde hash;
}

/* ==================================================
 * Helper: Find Slot
 * Returns slot index, sets *inventum flag
 * ================================================== */

interior i32
_invenire_slotum (
    TabulaDispersa* tabula,
            chorda  clavis
               i32  hash
               b32* inventum)
{
    i32 index;
    i32 distantia;
    i32 index_primus_deletum;

    *inventum            = FALSUM;
    index                = hash & tabula->capacitas_mask;
    distantia            = ZEPHYRUM;
    index_primus_deletum = (i32)-I;

    dum (tabula->slot[index].status != SLOT_VACUUM)
    {
        si (tabula->sloti[index].status == SLOT_OCCUPATUM)
        {
            si (tabula->sloti[index].hash_cachatum == hash &&
                tabula->comparatio(
                    tabula->sloti[index].clavis, clavis) == ZEPHYRUM)
            {
                *inventum = VERUM;
                redde index;
            }
            tabula->collisiones_totales++;
        }
        alioquin si (tabula->sloti[index].status == SLOT_DELETUM)
        {
            si (index_primus_deletum == (i32)-I)
            {
                index_primus_deletum = index;
            }
        }

        index = (index + I) & tabula->capacitas_mask;
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

    si (index_primus_deletum != (i32)-I)
    {
        redde index_primus_deletum;
    }

    redde index;
}


/* ==================================================
 * Resizing
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
        tabula->capacitqas * magnitudo(Slotus));

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
 * Creation
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
        _comparatio_chorda);
}


/* ==================================================
 * Insertion
 * ================================================== */

b32
tabula_dispersa_inserere(
    TabulaDispersa* tabula,
            chorda  clavis,
            vacuum* valor)
{
    i32 hash;
    i32 index;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Check load factors */
    si (tabula_dispersa_factor_oneris(tabula) > tabula->factor_maximus ||
        tabula_dispersa_factor_deletorum(tabula) > tabula->factor_deletorum_maximus)
    {
        si (!_tabula_dispersa_crescere(tabula))
        {
            redde FALSUM;
        }
    }

    hash = tabula->friatio(clavis);
    index = _invenire_slotum(tabula, clavis, hash, &inventum);

    si (inventum)
    {
        /* Update existing */
        tabula->sloti[index].valor = valor;
        redde VERUM;
    }

    /* Insert new */
    si (tabula->sloti[index].status == SLOT_DELETUM)
    {
        tabula->numerus_deletorum--;
    }

    tabula->sloti[index].clavis          = clavis;
    tabula->sloti[index].valor           = valor;
    tabula->sloti[index].hash_cachatum   = hash;
    tabula->sloti[index].status          = SLOT_OCCUPATUM;
    tabula->numerus++;

    redde VERUM;
}


/* ==================================================
 * Lookup
 * ================================================== */

b32
tabula_dispersa_invenire(
    TabulaDispersa*  tabula,
            chorda   clavis,
            vacuum** valor_out)
{
    i32 hash;
    i32 index;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    hash  = tabula->friatio(clavis);
    index = _invenire_slotum(tabula, clavis, hash, &inventum);

    si (inventum)
    {
        si (valor_out)
        {
            *valor_out = tabula->sloti[index].valor;
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

/* ==================================================
 * Deletion
 * ================================================== */

b32
tabula_dispersa_delere(
    TabulaDispersa* tabula,
            chorda  clavis)
{
    i32 hash;
    i32 index;
    b32 inventum;

    si (!tabula || clavis.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    hash  = tabula->friatio(clavis);
    index = _invenire_slotum(tabula, clavis, hash, &inventum);

    si (inventum)
    {
        tabula->sloti[index].status = SLOT_DELETUM;
        tabula->numerus--;
        tabula->numerus_deletorum++;
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * Lifecycle
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
    iter.index_currens = ZEPHYRUM;
    redde iter;
}

b32
tabula_dispersa_iterator_proximum(
    TabulaIterator*  iter,
            chorda*  clavis_out,
            vacuum** valor_out)
{
    si (!iter || !iter->tabula) redde FALSUM;

    dum (iter->index_currens < iter->tabula->capacitas)
    {
        si (iter->tabula->sloti[iter->index_currens].status == SLOT_OCCUPATUM)
        {
            si (clavis_out)
            {
                *clavis_out = iter->tabula->sloti[iter->index_currens].clavis;
            }
            si (valor_out)
            {
                *valor_out  = iter->tabula->sloti[iter->index_currens].valor;
            }
            iter->index_currens++;
            redde VERUM;
        }
        iter->index_currens++;
    }

    redde FALSUM;
}

/* ==================================================
 * Statistics
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

    imprimere("Tabula Dispersa Status:\n");
    imprimere("  Capacitas: %zu\n", tabula->capacitas);
    imprimere("  Numerus: %zu\n", tabula->numerus);
    imprimere("  Numerus deletorum: %zu\n", tabula->numerus_deletorum);
    imprimere("  Factor oneris: %.2f%%\n", tabula_dispersa_factor_oneris(tabula) * C);
    imprimere("  Factor deletorum: %.2f%%\n", 
                tabula_dispersa_factor_deletorum(tabula) * C);
    imprimere("  Collisiones totales: %zu\n", tabula->collisiones_totales);
    imprimere("  Distantia maxima: %zu\n", tabula->distantia_maxima);
}
