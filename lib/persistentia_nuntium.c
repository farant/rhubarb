#include "persistentia.h"
#include "nuntium.h"
#include "filum.h"
#include "uuid.h"
#include <string.h>

/* ==================================================
 * PERSISTENTIA NUNTIUM - File-Based Event Storage
 *
 * Serializes events using protobuf format (nuntium.h).
 * Writes to file with length-prefixed events.
 *
 * ================================================== */


/* ==================================================
 * Structura Interna
 * ================================================== */

nomen structura {
    Piscina*       piscina;
    FilumScriptor* scriptor;
    chorda         via_log;
    i32            numerus_eventuum;
} PersistentiaNuntiumData;


/* ==================================================
 * Serialization Helpers
 * ================================================== */

/* Serialize eventum ad NuntiumScriptor */
interior b32
_serialize_eventum(
    NuntiumScriptor*  scriptor,
    Piscina*          piscina,
    constans Eventum* eventum)
{
    chorda eventum_id;

    /* Generare event ID (UUIDv7) */
    eventum_id = uuidv7_creare(piscina);
    si (!eventum_id.datum)
    {
        redde FALSUM;
    }

    /* Scribere header */
    si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_ID, eventum_id))
    {
        redde FALSUM;
    }

    si (!nuntium_scribere_varint(scriptor, EVENTUM_TAG_GENUS, (i64)eventum->genus))
    {
        redde FALSUM;
    }

    si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_ENTITAS_ID, *eventum->entitas_id))
    {
        redde FALSUM;
    }

    /* Scribere payload secundum genus */
    commutatio (eventum->genus)
    {
        casus EVENTUS_CREARE_ENTITAS:
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_ENTITAS_GENUS,
                                         *eventum->datum.creare.entitas_genus))
            {
                redde FALSUM;
            }
            frange;

        casus EVENTUS_DELERE_ENTITAS:
            /* Nihil payload */
            frange;

        casus EVENTUS_PONERE_PROPRIETAS:
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_CLAVIS,
                                         *eventum->datum.proprietas.clavis))
            {
                redde FALSUM;
            }
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_VALOR,
                                         *eventum->datum.proprietas.valor))
            {
                redde FALSUM;
            }
            frange;

        casus EVENTUS_DELERE_PROPRIETAS:
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_CLAVIS,
                                         *eventum->datum.proprietas.clavis))
            {
                redde FALSUM;
            }
            frange;

        casus EVENTUS_ADDERE_RELATIO:
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_RELATIO_ID,
                                         *eventum->datum.relatio.relatio_id))
            {
                redde FALSUM;
            }
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_RELATIO_GENUS,
                                         *eventum->datum.relatio.relatio_genus))
            {
                redde FALSUM;
            }
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_DESTINATIO_ID,
                                         *eventum->datum.relatio.destinatio_id))
            {
                redde FALSUM;
            }
            frange;

        casus EVENTUS_DELERE_RELATIO:
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_RELATIO_ID,
                                         *eventum->datum.relatio.relatio_id))
            {
                redde FALSUM;
            }
            frange;

        casus EVENTUS_ADDERE_NOTA:
        casus EVENTUS_DELERE_NOTA:
            si (!nuntium_scribere_chorda(scriptor, EVENTUM_TAG_NOTA,
                                         *eventum->datum.nota.nota))
            {
                redde FALSUM;
            }
            frange;

        ordinarius:
            redde FALSUM;
    }

    redde VERUM;
}

/* Scribere length-prefixed eventum ad file */
interior b32
_scribere_eventum_ad_filum(
    PersistentiaNuntiumData* data,
    NuntiumScriptor*         scriptor)
{
    chorda eventum_datum;

    eventum_datum = nuntium_scriptor_datum(scriptor);
    si (!eventum_datum.datum || eventum_datum.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Scribere length prefix (varint) */
    {
        i8  length_buffer[X];  /* Max 10 bytes pro varint */
        i32 length_bytes = ZEPHYRUM;
        i64 mensura = (i64)eventum_datum.mensura;
        chorda length_chorda;

        /* Encode varint manually */
        fac
        {
            i8 byte = (i8)(mensura & 0x7F);
            mensura >>= VII;
            si (mensura != 0)
            {
                byte |= (i8)0x80;
            }
            length_buffer[length_bytes++] = byte;
        }
        dum (mensura != 0);

        length_chorda.datum = length_buffer;
        length_chorda.mensura = length_bytes;

        si (!filum_scriptor_scribere(data->scriptor, length_chorda))
        {
            redde FALSUM;
        }
    }

    /* Scribere eventum datum */
    si (!filum_scriptor_scribere(data->scriptor, eventum_datum))
    {
        redde FALSUM;
    }

    /* Sync ad discum */
    si (!filum_scriptor_sync(data->scriptor))
    {
        redde FALSUM;
    }

    data->numerus_eventuum++;

    redde VERUM;
}


/* ==================================================
 * Deserialization Helpers
 * ================================================== */

/* Legere varint ex buffer */
interior b32
_legere_varint_ex_buffer(
    i8*  datum,
    i32  mensura,
    i32* positio,
    i64* valor_out)
{
    i64 valor = 0;
    i32 shift = 0;
    i8  byte;

    dum (*positio < mensura)
    {
        byte = datum[*positio];
        (*positio)++;

        valor |= ((i64)(byte & 0x7F)) << shift;
        shift += VII;

        si (!(byte & 0x80))
        {
            *valor_out = valor;
            redde VERUM;
        }

        si (shift >= LXIV)
        {
            redde FALSUM;  /* Overflow */
        }
    }

    redde FALSUM;  /* Truncated */
}

/* Deserialize unum eventum ex NuntiumLector */
interior Eventum*
_deserialize_eventum(
    NuntiumLector* lector,
    Piscina*       piscina)
{
    Eventum* e;
    i32      tag;
    i32      wire_type;
    i32      genus = ZEPHYRUM;
    chorda   entitas_id = {ZEPHYRUM, NIHIL};
    chorda   entitas_genus = {ZEPHYRUM, NIHIL};
    chorda   clavis = {ZEPHYRUM, NIHIL};
    chorda   valor = {ZEPHYRUM, NIHIL};
    chorda   relatio_id = {ZEPHYRUM, NIHIL};
    chorda   relatio_genus = {ZEPHYRUM, NIHIL};
    chorda   destinatio_id = {ZEPHYRUM, NIHIL};
    chorda   nota = {ZEPHYRUM, NIHIL};

    /* Parse all fields */
    dum (nuntium_legere_field(lector, &tag, &wire_type))
    {
        commutatio (tag)
        {
            casus EVENTUM_TAG_ID:
                /* Skip event ID during replay */
                nuntium_legere_chorda(lector);
                frange;

            casus EVENTUM_TAG_GENUS:
                genus = (i32)nuntium_legere_varint(lector);
                frange;

            casus EVENTUM_TAG_ENTITAS_ID:
                entitas_id = nuntium_legere_chorda(lector);
                frange;

            casus EVENTUM_TAG_ENTITAS_GENUS:
                /* Also EVENTUM_TAG_CLAVIS, EVENTUM_TAG_RELATIO_ID, EVENTUM_TAG_NOTA */
                si (genus == EVENTUS_CREARE_ENTITAS)
                {
                    entitas_genus = nuntium_legere_chorda(lector);
                }
                alioquin si (genus == EVENTUS_PONERE_PROPRIETAS ||
                             genus == EVENTUS_DELERE_PROPRIETAS)
                {
                    clavis = nuntium_legere_chorda(lector);
                }
                alioquin si (genus == EVENTUS_ADDERE_RELATIO ||
                             genus == EVENTUS_DELERE_RELATIO)
                {
                    relatio_id = nuntium_legere_chorda(lector);
                }
                alioquin si (genus == EVENTUS_ADDERE_NOTA ||
                             genus == EVENTUS_DELERE_NOTA)
                {
                    nota = nuntium_legere_chorda(lector);
                }
                alioquin
                {
                    nuntium_saltare_field(lector, wire_type);
                }
                frange;

            casus EVENTUM_TAG_VALOR:
                /* Also EVENTUM_TAG_RELATIO_GENUS */
                si (genus == EVENTUS_PONERE_PROPRIETAS)
                {
                    valor = nuntium_legere_chorda(lector);
                }
                alioquin si (genus == EVENTUS_ADDERE_RELATIO)
                {
                    relatio_genus = nuntium_legere_chorda(lector);
                }
                alioquin
                {
                    nuntium_saltare_field(lector, wire_type);
                }
                frange;

            casus EVENTUM_TAG_DESTINATIO_ID:
                destinatio_id = nuntium_legere_chorda(lector);
                frange;

            ordinarius:
                nuntium_saltare_field(lector, wire_type);
                frange;
        }
    }

    si (nuntium_lector_error(lector))
    {
        redde NIHIL;
    }

    /* Allocare et populare Eventum */
    e = (Eventum*)piscina_allocare(piscina, magnitudo(Eventum));
    si (!e)
    {
        redde NIHIL;
    }

    e->genus = (EventusGenus)genus;

    /* Copiare entitas_id ad piscina */
    si (entitas_id.datum)
    {
        e->entitas_id = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
        si (e->entitas_id)
        {
            e->entitas_id->datum = (i8*)piscina_allocare(piscina,
                (memoriae_index)entitas_id.mensura);
            si (e->entitas_id->datum)
            {
                memcpy(e->entitas_id->datum, entitas_id.datum,
                       (memoriae_index)entitas_id.mensura);
                e->entitas_id->mensura = entitas_id.mensura;
            }
        }
    }

    /* Populare payload secundum genus */
    commutatio (genus)
    {
        casus EVENTUS_CREARE_ENTITAS:
            si (entitas_genus.datum)
            {
                e->datum.creare.entitas_genus = (chorda*)piscina_allocare(
                    piscina, magnitudo(chorda));
                si (e->datum.creare.entitas_genus)
                {
                    e->datum.creare.entitas_genus->datum = (i8*)piscina_allocare(
                        piscina, (memoriae_index)entitas_genus.mensura);
                    si (e->datum.creare.entitas_genus->datum)
                    {
                        memcpy(e->datum.creare.entitas_genus->datum,
                               entitas_genus.datum,
                               (memoriae_index)entitas_genus.mensura);
                        e->datum.creare.entitas_genus->mensura = entitas_genus.mensura;
                    }
                }
            }
            frange;

        casus EVENTUS_PONERE_PROPRIETAS:
        casus EVENTUS_DELERE_PROPRIETAS:
            si (clavis.datum)
            {
                e->datum.proprietas.clavis = (chorda*)piscina_allocare(
                    piscina, magnitudo(chorda));
                si (e->datum.proprietas.clavis)
                {
                    e->datum.proprietas.clavis->datum = (i8*)piscina_allocare(
                        piscina, (memoriae_index)clavis.mensura);
                    si (e->datum.proprietas.clavis->datum)
                    {
                        memcpy(e->datum.proprietas.clavis->datum,
                               clavis.datum, (memoriae_index)clavis.mensura);
                        e->datum.proprietas.clavis->mensura = clavis.mensura;
                    }
                }
            }
            si (valor.datum)
            {
                e->datum.proprietas.valor = (chorda*)piscina_allocare(
                    piscina, magnitudo(chorda));
                si (e->datum.proprietas.valor)
                {
                    e->datum.proprietas.valor->datum = (i8*)piscina_allocare(
                        piscina, (memoriae_index)valor.mensura);
                    si (e->datum.proprietas.valor->datum)
                    {
                        memcpy(e->datum.proprietas.valor->datum,
                               valor.datum, (memoriae_index)valor.mensura);
                        e->datum.proprietas.valor->mensura = valor.mensura;
                    }
                }
            }
            frange;

        casus EVENTUS_ADDERE_RELATIO:
        casus EVENTUS_DELERE_RELATIO:
            si (relatio_id.datum)
            {
                e->datum.relatio.relatio_id = (chorda*)piscina_allocare(
                    piscina, magnitudo(chorda));
                si (e->datum.relatio.relatio_id)
                {
                    e->datum.relatio.relatio_id->datum = (i8*)piscina_allocare(
                        piscina, (memoriae_index)relatio_id.mensura);
                    si (e->datum.relatio.relatio_id->datum)
                    {
                        memcpy(e->datum.relatio.relatio_id->datum,
                               relatio_id.datum, (memoriae_index)relatio_id.mensura);
                        e->datum.relatio.relatio_id->mensura = relatio_id.mensura;
                    }
                }
            }
            si (relatio_genus.datum)
            {
                e->datum.relatio.relatio_genus = (chorda*)piscina_allocare(
                    piscina, magnitudo(chorda));
                si (e->datum.relatio.relatio_genus)
                {
                    e->datum.relatio.relatio_genus->datum = (i8*)piscina_allocare(
                        piscina, (memoriae_index)relatio_genus.mensura);
                    si (e->datum.relatio.relatio_genus->datum)
                    {
                        memcpy(e->datum.relatio.relatio_genus->datum,
                               relatio_genus.datum, (memoriae_index)relatio_genus.mensura);
                        e->datum.relatio.relatio_genus->mensura = relatio_genus.mensura;
                    }
                }
            }
            si (destinatio_id.datum)
            {
                e->datum.relatio.destinatio_id = (chorda*)piscina_allocare(
                    piscina, magnitudo(chorda));
                si (e->datum.relatio.destinatio_id)
                {
                    e->datum.relatio.destinatio_id->datum = (i8*)piscina_allocare(
                        piscina, (memoriae_index)destinatio_id.mensura);
                    si (e->datum.relatio.destinatio_id->datum)
                    {
                        memcpy(e->datum.relatio.destinatio_id->datum,
                               destinatio_id.datum, (memoriae_index)destinatio_id.mensura);
                        e->datum.relatio.destinatio_id->mensura = destinatio_id.mensura;
                    }
                }
            }
            frange;

        casus EVENTUS_ADDERE_NOTA:
        casus EVENTUS_DELERE_NOTA:
            si (nota.datum)
            {
                e->datum.nota.nota = (chorda*)piscina_allocare(
                    piscina, magnitudo(chorda));
                si (e->datum.nota.nota)
                {
                    e->datum.nota.nota->datum = (i8*)piscina_allocare(
                        piscina, (memoriae_index)nota.mensura);
                    si (e->datum.nota.nota->datum)
                    {
                        memcpy(e->datum.nota.nota->datum,
                               nota.datum, (memoriae_index)nota.mensura);
                        e->datum.nota.nota->mensura = nota.mensura;
                    }
                }
            }
            frange;

        ordinarius:
            frange;
    }

    redde e;
}


/* ==================================================
 * Interface Implementation
 * ================================================== */

interior b32
_nuntium_scribere_eventum(
    vacuum*           datum,
    constans Eventum* eventum)
{
    PersistentiaNuntiumData* data;
    NuntiumScriptor*         scriptor;

    data = (PersistentiaNuntiumData*)datum;

    si (!data || !eventum)
    {
        redde FALSUM;
    }

    /* Creare scriptor */
    scriptor = nuntium_scriptor_creare(data->piscina, CCLVI);
    si (!scriptor)
    {
        redde FALSUM;
    }

    /* Serialize eventum */
    si (!_serialize_eventum(scriptor, data->piscina, eventum))
    {
        redde FALSUM;
    }

    /* Scribere ad filum */
    redde _scribere_eventum_ad_filum(data, scriptor);
}

interior Xar*
_nuntium_legere_eventus(
    vacuum*  datum,
    Piscina* piscina)
{
    PersistentiaNuntiumData* data;
    Xar*                     resultus;
    chorda                   log_datum;
    i32                      positio;
    character*               via_cstr;

    data = (PersistentiaNuntiumData*)datum;

    si (!data)
    {
        redde NIHIL;
    }

    /* Creare Xar pro eventus */
    resultus = xar_creare(piscina, magnitudo(Eventum*));
    si (!resultus)
    {
        redde NIHIL;
    }

    /* Legere totum filum */
    via_cstr = chorda_ut_cstr(data->via_log, piscina);
    si (!via_cstr)
    {
        redde resultus;  /* Empty */
    }

    si (!filum_existit(via_cstr))
    {
        redde resultus;  /* Empty - no file yet */
    }

    log_datum = filum_legere_totum(via_cstr, piscina);
    si (!log_datum.datum || log_datum.mensura == ZEPHYRUM)
    {
        redde resultus;  /* Empty */
    }

    /* Parse eventus */
    positio = ZEPHYRUM;
    dum (positio < log_datum.mensura)
    {
        i64            length;
        chorda         eventum_datum;
        NuntiumLector* lector;
        Eventum*       eventum;
        Eventum**      slot;

        /* Legere length prefix */
        si (!_legere_varint_ex_buffer(log_datum.datum, log_datum.mensura,
                                      &positio, &length))
        {
            frange;  /* Truncated */
        }

        /* Validate */
        si (positio + (i32)length > log_datum.mensura)
        {
            frange;  /* Truncated event */
        }

        /* Creare lector */
        eventum_datum.datum = log_datum.datum + positio;
        eventum_datum.mensura = (i32)length;

        lector = nuntium_lector_creare(piscina, eventum_datum);
        si (!lector)
        {
            frange;
        }

        /* Deserialize */
        eventum = _deserialize_eventum(lector, piscina);
        si (eventum)
        {
            slot = (Eventum**)xar_addere(resultus);
            si (slot)
            {
                *slot = eventum;
            }
        }

        positio += (i32)length;
    }

    redde resultus;
}

interior b32
_nuntium_sync(
    vacuum* datum)
{
    PersistentiaNuntiumData* data;

    data = (PersistentiaNuntiumData*)datum;

    si (!data || !data->scriptor)
    {
        redde FALSUM;
    }

    redde filum_scriptor_sync(data->scriptor);
}

interior vacuum
_nuntium_claudere(
    vacuum* datum)
{
    PersistentiaNuntiumData* data;

    data = (PersistentiaNuntiumData*)datum;

    si (!data)
    {
        redde;
    }

    si (data->scriptor)
    {
        filum_scriptor_sync(data->scriptor);
        filum_scriptor_claudere(data->scriptor);
        data->scriptor = NIHIL;
    }
}


/* ==================================================
 * Factory Functions
 * ================================================== */

Persistentia*
persistentia_nuntium_creare(
    Piscina*            piscina,
    constans character* via)
{
    Persistentia*            pers;
    PersistentiaNuntiumData* data;
    memoriae_index           via_len;
    character*               via_copy;

    si (!piscina || !via)
    {
        redde NIHIL;
    }

    /* Allocare structuras */
    pers = (Persistentia*)piscina_allocare(piscina, magnitudo(Persistentia));
    si (!pers)
    {
        redde NIHIL;
    }

    data = (PersistentiaNuntiumData*)piscina_allocare(
        piscina, magnitudo(PersistentiaNuntiumData));
    si (!data)
    {
        redde NIHIL;
    }

    data->piscina = piscina;
    data->numerus_eventuum = ZEPHYRUM;

    /* Copiare via */
    via_len = strlen(via);
    via_copy = (character*)piscina_allocare(piscina, via_len + I);
    si (!via_copy)
    {
        redde NIHIL;
    }
    memcpy(via_copy, via, via_len + I);
    data->via_log.datum = (i8*)via_copy;
    data->via_log.mensura = (i32)via_len;

    /* Aperire filum pro scriptione */
    data->scriptor = filum_scriptor_aperire(via, FILUM_MODUS_APPENDERE, piscina);
    si (!data->scriptor)
    {
        /* Try creating new file */
        data->scriptor = filum_scriptor_aperire(via, FILUM_MODUS_CREARE, piscina);
        si (!data->scriptor)
        {
            redde NIHIL;
        }
    }

    /* Ponere interface */
    pers->datum = data;
    pers->scribere_eventum = _nuntium_scribere_eventum;
    pers->legere_eventus = _nuntium_legere_eventus;
    pers->sync = _nuntium_sync;
    pers->claudere = _nuntium_claudere;

    redde pers;
}

Persistentia*
persistentia_nuntium_aperire(
    Piscina*            piscina,
    constans character* via)
{
    /* Verificare si filum existit */
    si (!filum_existit(via))
    {
        redde NIHIL;
    }

    redde persistentia_nuntium_creare(piscina, via);
}
