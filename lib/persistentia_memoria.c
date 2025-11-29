#include "persistentia.h"

/* ==================================================
 * PERSISTENTIA MEMORIA - In-Memory Event Storage
 *
 * Simplest implementation: stores Eventum* in Xar.
 * No serialization. Fast for tests.
 *
 * ================================================== */


/* ==================================================
 * Structura Interna
 * ================================================== */

nomen structura {
    Piscina* piscina;
    Xar*     eventus;  /* Xar of Eventum* */
} PersistentiaMemoriaData;


/* ==================================================
 * Interface Implementation
 * ================================================== */

interior b32
_memoria_scribere_eventum(
    vacuum*           datum,
    constans Eventum* eventum)
{
    PersistentiaMemoriaData* data;
    Eventum**                slot;
    Eventum*                 copia;

    data = (PersistentiaMemoriaData*)datum;

    si (!data || !eventum)
    {
        redde FALSUM;
    }

    /* Copiare eventum ad piscinam (eventum potest esse temporarium) */
    copia = (Eventum*)piscina_allocare(data->piscina, magnitudo(Eventum));
    si (!copia)
    {
        redde FALSUM;
    }

    *copia = *eventum;

    /* Addere ad Xar */
    slot = (Eventum**)xar_addere(data->eventus);
    si (!slot)
    {
        redde FALSUM;
    }

    *slot = copia;

    redde VERUM;
}

interior Xar*
_memoria_legere_eventus(
    vacuum*  datum,
    Piscina* piscina)
{
    PersistentiaMemoriaData* data;
    Xar*                     resultus;
    Eventum**                slot;
    Eventum*                 eventum;
    i32                      i;
    i32                      numerus;

    (vacuum)piscina;  /* Non usatum - eventus iam in memoria */

    data = (PersistentiaMemoriaData*)datum;

    si (!data)
    {
        redde NIHIL;
    }

    /* Creare novum Xar cum copiis (caller's piscina) */
    resultus = xar_creare(piscina, magnitudo(Eventum*));
    si (!resultus)
    {
        redde NIHIL;
    }

    numerus = xar_numerus(data->eventus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        eventum = *(Eventum**)xar_obtinere(data->eventus, i);
        slot = (Eventum**)xar_addere(resultus);
        si (slot)
        {
            *slot = eventum;  /* Eventus manent in memoria originali */
        }
    }

    redde resultus;
}

interior b32
_memoria_sync(
    vacuum* datum)
{
    (vacuum)datum;
    /* No-op pro memoria */
    redde VERUM;
}

interior vacuum
_memoria_claudere(
    vacuum* datum)
{
    (vacuum)datum;
    /* No-op pro memoria - piscina liberat omnia */
}


/* ==================================================
 * Factory Function
 * ================================================== */

Persistentia*
persistentia_memoria_creare(
    Piscina* piscina)
{
    Persistentia*            pers;
    PersistentiaMemoriaData* data;

    si (!piscina)
    {
        redde NIHIL;
    }

    /* Allocare structuram */
    pers = (Persistentia*)piscina_allocare(piscina, magnitudo(Persistentia));
    si (!pers)
    {
        redde NIHIL;
    }

    /* Allocare datum internum */
    data = (PersistentiaMemoriaData*)piscina_allocare(
        piscina, magnitudo(PersistentiaMemoriaData));
    si (!data)
    {
        redde NIHIL;
    }

    data->piscina = piscina;
    data->eventus = xar_creare(piscina, magnitudo(Eventum*));
    si (!data->eventus)
    {
        redde NIHIL;
    }

    /* Ponere interface */
    pers->datum = data;
    pers->scribere_eventum = _memoria_scribere_eventum;
    pers->legere_eventus = _memoria_legere_eventus;
    pers->sync = _memoria_sync;
    pers->claudere = _memoria_claudere;

    redde pers;
}


/* ==================================================
 * Helper Functions - Eventum Creators
 * ================================================== */

Eventum*
eventum_creare_entitas(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus)
{
    Eventum* e;

    si (!piscina || !entitas_id || !entitas_genus)
    {
        redde NIHIL;
    }

    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = EVENTUS_CREARE_ENTITAS;
    e->entitas_id = entitas_id;
    e->entitas_genus = entitas_genus;

    redde e;
}

Eventum*
eventum_delere_entitas(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus)
{
    Eventum* e;

    si (!piscina || !entitas_id || !entitas_genus)
    {
        redde NIHIL;
    }

    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = EVENTUS_DELERE_ENTITAS;
    e->entitas_id = entitas_id;
    e->entitas_genus = entitas_genus;

    redde e;
}

Eventum*
eventum_ponere_proprietas(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus,
    chorda*  clavis,
    chorda*  valor)
{
    Eventum* e;

    si (!piscina || !entitas_id || !entitas_genus || !clavis || !valor)
    {
        redde NIHIL;
    }

    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = EVENTUS_PONERE_PROPRIETAS;
    e->entitas_id = entitas_id;
    e->entitas_genus = entitas_genus;
    e->datum.proprietas.clavis = clavis;
    e->datum.proprietas.valor = valor;

    redde e;
}

Eventum*
eventum_delere_proprietas(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus,
    chorda*  clavis)
{
    Eventum* e;

    si (!piscina || !entitas_id || !entitas_genus || !clavis)
    {
        redde NIHIL;
    }

    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = EVENTUS_DELERE_PROPRIETAS;
    e->entitas_id = entitas_id;
    e->entitas_genus = entitas_genus;
    e->datum.proprietas.clavis = clavis;
    e->datum.proprietas.valor = NIHIL;

    redde e;
}

Eventum*
eventum_addere_relatio(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus,
    chorda*  relatio_id,
    chorda*  relatio_genus,
    chorda*  destinatio_id)
{
    Eventum* e;

    si (!piscina || !entitas_id || !entitas_genus || !relatio_id || !relatio_genus || !destinatio_id)
    {
        redde NIHIL;
    }

    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = EVENTUS_ADDERE_RELATIO;
    e->entitas_id = entitas_id;
    e->entitas_genus = entitas_genus;
    e->datum.relatio.relatio_id = relatio_id;
    e->datum.relatio.relatio_genus = relatio_genus;
    e->datum.relatio.destinatio_id = destinatio_id;

    redde e;
}

Eventum*
eventum_delere_relatio(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus,
    chorda*  relatio_id)
{
    Eventum* e;

    si (!piscina || !entitas_id || !entitas_genus || !relatio_id)
    {
        redde NIHIL;
    }

    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = EVENTUS_DELERE_RELATIO;
    e->entitas_id = entitas_id;
    e->entitas_genus = entitas_genus;
    e->datum.relatio.relatio_id = relatio_id;
    e->datum.relatio.relatio_genus = NIHIL;
    e->datum.relatio.destinatio_id = NIHIL;

    redde e;
}

Eventum*
eventum_addere_nota(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus,
    chorda*  nota)
{
    Eventum* e;

    si (!piscina || !entitas_id || !entitas_genus || !nota)
    {
        redde NIHIL;
    }

    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = EVENTUS_ADDERE_NOTA;
    e->entitas_id = entitas_id;
    e->entitas_genus = entitas_genus;
    e->datum.nota.nota = nota;

    redde e;
}

Eventum*
eventum_delere_nota(
    Piscina* piscina,
    chorda*  entitas_id,
    chorda*  entitas_genus,
    chorda*  nota)
{
    Eventum* e;

    si (!piscina || !entitas_id || !entitas_genus || !nota)
    {
        redde NIHIL;
    }

    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = EVENTUS_DELERE_NOTA;
    e->entitas_id = entitas_id;
    e->entitas_genus = entitas_genus;
    e->datum.nota.nota = nota;

    redde e;
}
