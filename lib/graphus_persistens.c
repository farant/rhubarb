#include "graphus_persistens.h"
#include "uuid.h"
#include <string.h>

/* ==================================================
 * Structura Interna
 * ================================================== */

structura GraphusPersistens {
	Piscina*             piscina;
	GraphusEntitatum*    graphus;
	EntitasRepositorium* repositorium;
	FilumScriptor*       scriptor;
	chorda               via_log;
	i32                  numerus_eventuum;
};

/* ==================================================
 * Functiones Auxiliares - Event Serialization
 * ================================================== */

/* Scribere header commune */
interior b32
_scribere_header(
	NuntiumScriptor* scriptor,
	chorda           eventus_id,
	i32              genus,
	chorda           entitas_id)
{
	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_ID, eventus_id))
	{
		redde FALSUM;
	}
	si (!nuntium_scribere_varint(scriptor, EVENTUS_TAG_GENUS, (i64)genus))
	{
		redde FALSUM;
	}
	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_ENTITAS_ID, entitas_id))
	{
		redde FALSUM;
	}
	redde VERUM;
}

/* Generare eventus_id (UUIDv7) */
interior chorda
_generare_eventus_id(
	Piscina* piscina)
{
	redde uuidv7_creare(piscina);
}

/* Scribere eventum ad log file */
interior b32
_scribere_eventum_ad_log(
	GraphusPersistens* gp,
	NuntiumScriptor*   scriptor)
{
	chorda eventum_datum;

	eventum_datum = nuntium_scriptor_datum(scriptor);
	si (!eventum_datum.datum || eventum_datum.mensura == ZEPHYRUM)
	{
		redde FALSUM;
	}

	/* Scribere length prefix */
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

		si (!filum_scriptor_scribere(gp->scriptor, length_chorda))
		{
			redde FALSUM;
		}
	}

	/* Scribere eventum datum */
	si (!filum_scriptor_scribere(gp->scriptor, eventum_datum))
	{
		redde FALSUM;
	}

	/* Sync ad discum */
	si (!filum_scriptor_sync(gp->scriptor))
	{
		redde FALSUM;
	}

	gp->numerus_eventuum++;

	redde VERUM;
}

/* ==================================================
 * Event Writers - One per event type
 * ================================================== */

interior b32
_scribere_eventum_creare_entitas(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             entitas_genus)
{
	NuntiumScriptor* scriptor;
	chorda           eventus_id;

	scriptor = nuntium_scriptor_creare(gp->piscina, CCLVI);
	si (!scriptor)
	{
		redde FALSUM;
	}

	eventus_id = _generare_eventus_id(gp->piscina);
	si (!eventus_id.datum)
	{
		redde FALSUM;
	}

	/* Header */
	si (!_scribere_header(scriptor, eventus_id, EVENTUS_CREARE_ENTITAS, entitas_id))
	{
		redde FALSUM;
	}

	/* Payload */
	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_ENTITAS_GENUS, entitas_genus))
	{
		redde FALSUM;
	}

	redde _scribere_eventum_ad_log(gp, scriptor);
}

interior b32
_scribere_eventum_delere_entitas(
	GraphusPersistens* gp,
	chorda             entitas_id)
{
	NuntiumScriptor* scriptor;
	chorda           eventus_id;

	scriptor = nuntium_scriptor_creare(gp->piscina, CCLVI);
	si (!scriptor)
	{
		redde FALSUM;
	}

	eventus_id = _generare_eventus_id(gp->piscina);
	si (!eventus_id.datum)
	{
		redde FALSUM;
	}

	/* Header only, no payload */
	si (!_scribere_header(scriptor, eventus_id, EVENTUS_DELERE_ENTITAS, entitas_id))
	{
		redde FALSUM;
	}

	redde _scribere_eventum_ad_log(gp, scriptor);
}

interior b32
_scribere_eventum_ponere_proprietas(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             clavis,
	chorda             valor)
{
	NuntiumScriptor* scriptor;
	chorda           eventus_id;

	scriptor = nuntium_scriptor_creare(gp->piscina, CCLVI);
	si (!scriptor)
	{
		redde FALSUM;
	}

	eventus_id = _generare_eventus_id(gp->piscina);
	si (!eventus_id.datum)
	{
		redde FALSUM;
	}

	si (!_scribere_header(scriptor, eventus_id, EVENTUS_PONERE_PROPRIETAS, entitas_id))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_CLAVIS, clavis))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_VALOR, valor))
	{
		redde FALSUM;
	}

	redde _scribere_eventum_ad_log(gp, scriptor);
}

interior b32
_scribere_eventum_delere_proprietas(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             clavis)
{
	NuntiumScriptor* scriptor;
	chorda           eventus_id;

	scriptor = nuntium_scriptor_creare(gp->piscina, CCLVI);
	si (!scriptor)
	{
		redde FALSUM;
	}

	eventus_id = _generare_eventus_id(gp->piscina);
	si (!eventus_id.datum)
	{
		redde FALSUM;
	}

	si (!_scribere_header(scriptor, eventus_id, EVENTUS_DELERE_PROPRIETAS, entitas_id))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_CLAVIS, clavis))
	{
		redde FALSUM;
	}

	redde _scribere_eventum_ad_log(gp, scriptor);
}

interior b32
_scribere_eventum_addere_relatio(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             relatio_id,
	chorda             relatio_genus,
	chorda             destinatio_id)
{
	NuntiumScriptor* scriptor;
	chorda           eventus_id;

	scriptor = nuntium_scriptor_creare(gp->piscina, CCLVI);
	si (!scriptor)
	{
		redde FALSUM;
	}

	eventus_id = _generare_eventus_id(gp->piscina);
	si (!eventus_id.datum)
	{
		redde FALSUM;
	}

	si (!_scribere_header(scriptor, eventus_id, EVENTUS_ADDERE_RELATIO, entitas_id))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_RELATIO_ID, relatio_id))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_RELATIO_GENUS, relatio_genus))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_DESTINATIO_ID, destinatio_id))
	{
		redde FALSUM;
	}

	redde _scribere_eventum_ad_log(gp, scriptor);
}

interior b32
_scribere_eventum_delere_relatio(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             relatio_id)
{
	NuntiumScriptor* scriptor;
	chorda           eventus_id;

	scriptor = nuntium_scriptor_creare(gp->piscina, CCLVI);
	si (!scriptor)
	{
		redde FALSUM;
	}

	eventus_id = _generare_eventus_id(gp->piscina);
	si (!eventus_id.datum)
	{
		redde FALSUM;
	}

	si (!_scribere_header(scriptor, eventus_id, EVENTUS_DELERE_RELATIO, entitas_id))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_RELATIO_ID, relatio_id))
	{
		redde FALSUM;
	}

	redde _scribere_eventum_ad_log(gp, scriptor);
}

interior b32
_scribere_eventum_addere_nota(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             nota)
{
	NuntiumScriptor* scriptor;
	chorda           eventus_id;

	scriptor = nuntium_scriptor_creare(gp->piscina, CCLVI);
	si (!scriptor)
	{
		redde FALSUM;
	}

	eventus_id = _generare_eventus_id(gp->piscina);
	si (!eventus_id.datum)
	{
		redde FALSUM;
	}

	si (!_scribere_header(scriptor, eventus_id, EVENTUS_ADDERE_NOTA, entitas_id))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_NOTA, nota))
	{
		redde FALSUM;
	}

	redde _scribere_eventum_ad_log(gp, scriptor);
}

interior b32
_scribere_eventum_delere_nota(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             nota)
{
	NuntiumScriptor* scriptor;
	chorda           eventus_id;

	scriptor = nuntium_scriptor_creare(gp->piscina, CCLVI);
	si (!scriptor)
	{
		redde FALSUM;
	}

	eventus_id = _generare_eventus_id(gp->piscina);
	si (!eventus_id.datum)
	{
		redde FALSUM;
	}

	si (!_scribere_header(scriptor, eventus_id, EVENTUS_DELERE_NOTA, entitas_id))
	{
		redde FALSUM;
	}

	si (!nuntium_scribere_chorda(scriptor, EVENTUS_TAG_NOTA, nota))
	{
		redde FALSUM;
	}

	redde _scribere_eventum_ad_log(gp, scriptor);
}


/* ==================================================
 * Event Replay - Apply event to in-memory graph
 * ================================================== */

interior b32
_applicare_eventum_creare_entitas(
	GraphusPersistens*   gp,
	chorda               entitas_id,
	chorda               entitas_genus)
{
	InternamentumChorda* intern;
	chorda*              id_internatum;
	chorda*              genus_internatum;
	Entitas*             entitas;

	intern = graphus_entitatum_internamentum(gp->graphus);

	id_internatum = chorda_internare(intern, entitas_id);
	si (!id_internatum)
	{
		redde FALSUM;
	}

	genus_internatum = chorda_internare(intern, entitas_genus);
	si (!genus_internatum)
	{
		redde FALSUM;
	}

	entitas = entitas_creare(gp->piscina, id_internatum, genus_internatum);
	si (!entitas)
	{
		redde FALSUM;
	}

	redde graphus_entitatum_addere_entitatem(gp->graphus, entitas);
}

interior b32
_applicare_eventum_delere_entitas(
	GraphusPersistens* gp,
	chorda             entitas_id)
{
	InternamentumChorda* intern;
	chorda*              id_internatum;

	intern = graphus_entitatum_internamentum(gp->graphus);

	id_internatum = chorda_internare(intern, entitas_id);
	si (!id_internatum)
	{
		redde FALSUM;
	}

	/* Use repositorium from underlying graphus to delete */
	redde gp->repositorium->entitas_delere(gp->graphus, id_internatum);
}

interior b32
_applicare_eventum_ponere_proprietas(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             clavis,
	chorda             valor)
{
	InternamentumChorda* intern;
	chorda*              id_internatum;
	chorda*              clavis_internata;
	chorda*              valor_internatus;
	Entitas*             entitas;

	intern = graphus_entitatum_internamentum(gp->graphus);

	id_internatum = chorda_internare(intern, entitas_id);
	si (!id_internatum)
	{
		redde FALSUM;
	}

	entitas = gp->repositorium->capere_entitatem(gp->graphus, id_internatum);
	si (!entitas)
	{
		redde FALSUM;
	}

	clavis_internata = chorda_internare(intern, clavis);
	si (!clavis_internata)
	{
		redde FALSUM;
	}

	valor_internatus = chorda_internare(intern, valor);
	si (!valor_internatus)
	{
		redde FALSUM;
	}

	redde entitas_proprietas_addere(entitas, clavis_internata, valor_internatus);
}

interior b32
_applicare_eventum_delere_proprietas(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             clavis)
{
	InternamentumChorda* intern;
	chorda*              id_internatum;
	chorda*              clavis_internata;
	Entitas*             entitas;

	intern = graphus_entitatum_internamentum(gp->graphus);

	id_internatum = chorda_internare(intern, entitas_id);
	si (!id_internatum)
	{
		redde FALSUM;
	}

	entitas = gp->repositorium->capere_entitatem(gp->graphus, id_internatum);
	si (!entitas)
	{
		redde FALSUM;
	}

	clavis_internata = chorda_internare(intern, clavis);
	si (!clavis_internata)
	{
		redde FALSUM;
	}

	redde entitas_proprietas_delere(entitas, clavis_internata);
}

interior b32
_applicare_eventum_addere_relatio(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             relatio_id,
	chorda             relatio_genus,
	chorda             destinatio_id)
{
	InternamentumChorda* intern;
	chorda*              id_internatum;
	chorda*              relatio_id_internatum;
	chorda*              genus_internatum;
	chorda*              dest_id_internatum;
	Entitas*             entitas;
	Relatio*             relatio;

	intern = graphus_entitatum_internamentum(gp->graphus);

	id_internatum = chorda_internare(intern, entitas_id);
	si (!id_internatum)
	{
		redde FALSUM;
	}

	entitas = gp->repositorium->capere_entitatem(gp->graphus, id_internatum);
	si (!entitas)
	{
		redde FALSUM;
	}

	relatio_id_internatum = chorda_internare(intern, relatio_id);
	si (!relatio_id_internatum)
	{
		redde FALSUM;
	}

	genus_internatum = chorda_internare(intern, relatio_genus);
	si (!genus_internatum)
	{
		redde FALSUM;
	}

	dest_id_internatum = chorda_internare(intern, destinatio_id);
	si (!dest_id_internatum)
	{
		redde FALSUM;
	}

	/* Use entitas_relatio_addere_cum_id to preserve the relation ID from log */
	relatio = entitas_relatio_addere_cum_id(
		entitas,
		gp->piscina,
		intern,
		relatio_id_internatum,
		genus_internatum,
		dest_id_internatum);

	si (!relatio)
	{
		redde FALSUM;
	}

	/* Register in graph index */
	redde graphus_entitatum_registrare_relatio(gp->graphus, relatio);
}

interior b32
_applicare_eventum_delere_relatio(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             relatio_id)
{
	InternamentumChorda* intern;
	chorda*              relatio_id_internatum;

	(vacuum)entitas_id;  /* Non usatum directe */

	intern = graphus_entitatum_internamentum(gp->graphus);

	relatio_id_internatum = chorda_internare(intern, relatio_id);
	si (!relatio_id_internatum)
	{
		redde FALSUM;
	}

	/* Use the graphus repositorium to delete */
	redde gp->repositorium->relatio_delere(gp->graphus, relatio_id_internatum);
}

interior b32
_applicare_eventum_addere_nota(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             nota)
{
	InternamentumChorda* intern;
	chorda*              id_internatum;
	chorda*              nota_internata;
	Entitas*             entitas;

	intern = graphus_entitatum_internamentum(gp->graphus);

	id_internatum = chorda_internare(intern, entitas_id);
	si (!id_internatum)
	{
		redde FALSUM;
	}

	entitas = gp->repositorium->capere_entitatem(gp->graphus, id_internatum);
	si (!entitas)
	{
		redde FALSUM;
	}

	nota_internata = chorda_internare(intern, nota);
	si (!nota_internata)
	{
		redde FALSUM;
	}

	redde entitas_nota_addere(entitas, nota_internata);
}

interior b32
_applicare_eventum_delere_nota(
	GraphusPersistens* gp,
	chorda             entitas_id,
	chorda             nota)
{
	InternamentumChorda* intern;
	chorda*              id_internatum;
	chorda*              nota_internata;
	Entitas*             entitas;

	intern = graphus_entitatum_internamentum(gp->graphus);

	id_internatum = chorda_internare(intern, entitas_id);
	si (!id_internatum)
	{
		redde FALSUM;
	}

	entitas = gp->repositorium->capere_entitatem(gp->graphus, id_internatum);
	si (!entitas)
	{
		redde FALSUM;
	}

	nota_internata = chorda_internare(intern, nota);
	si (!nota_internata)
	{
		redde FALSUM;
	}

	redde entitas_nota_delere(entitas, nota_internata);
}


/* ==================================================
 * Event Parsing and Replay
 * ================================================== */

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

interior b32
_applicare_eventum(
	GraphusPersistens* gp,
	NuntiumLector*     lector)
{
	i32    tag;
	i32    wire_type;
	i32    genus = ZEPHYRUM;
	chorda entitas_id = {ZEPHYRUM, NIHIL};
	chorda entitas_genus = {ZEPHYRUM, NIHIL};
	chorda clavis = {ZEPHYRUM, NIHIL};
	chorda valor = {ZEPHYRUM, NIHIL};
	chorda relatio_id = {ZEPHYRUM, NIHIL};
	chorda relatio_genus = {ZEPHYRUM, NIHIL};
	chorda destinatio_id = {ZEPHYRUM, NIHIL};
	chorda nota = {ZEPHYRUM, NIHIL};

	/* Parse all fields */
	dum (nuntium_legere_field(lector, &tag, &wire_type))
	{
		commutatio (tag)
		{
			casus EVENTUS_TAG_ID:
				/* Skip event ID during replay */
				nuntium_legere_chorda(lector);
				frange;

			casus EVENTUS_TAG_GENUS:
				genus = (i32)nuntium_legere_varint(lector);
				frange;

			casus EVENTUS_TAG_ENTITAS_ID:
				entitas_id = nuntium_legere_chorda(lector);
				frange;

			casus EVENTUS_TAG_ENTITAS_GENUS:
				/* Also EVENTUS_TAG_CLAVIS, EVENTUS_TAG_RELATIO_ID, EVENTUS_TAG_NOTA */
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

			casus EVENTUS_TAG_VALOR:
				/* Also EVENTUS_TAG_RELATIO_GENUS */
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

			casus EVENTUS_TAG_DESTINATIO_ID:
				destinatio_id = nuntium_legere_chorda(lector);
				frange;

			ordinarius:
				nuntium_saltare_field(lector, wire_type);
				frange;
		}
	}

	si (nuntium_lector_error(lector))
	{
		redde FALSUM;
	}

	/* Apply based on genus */
	commutatio (genus)
	{
		casus EVENTUS_CREARE_ENTITAS:
			redde _applicare_eventum_creare_entitas(gp, entitas_id, entitas_genus);

		casus EVENTUS_DELERE_ENTITAS:
			redde _applicare_eventum_delere_entitas(gp, entitas_id);

		casus EVENTUS_PONERE_PROPRIETAS:
			redde _applicare_eventum_ponere_proprietas(gp, entitas_id, clavis, valor);

		casus EVENTUS_DELERE_PROPRIETAS:
			redde _applicare_eventum_delere_proprietas(gp, entitas_id, clavis);

		casus EVENTUS_ADDERE_RELATIO:
			redde _applicare_eventum_addere_relatio(gp, entitas_id, relatio_id,
			                                        relatio_genus, destinatio_id);

		casus EVENTUS_DELERE_RELATIO:
			redde _applicare_eventum_delere_relatio(gp, entitas_id, relatio_id);

		casus EVENTUS_ADDERE_NOTA:
			redde _applicare_eventum_addere_nota(gp, entitas_id, nota);

		casus EVENTUS_DELERE_NOTA:
			redde _applicare_eventum_delere_nota(gp, entitas_id, nota);

		ordinarius:
			redde FALSUM;  /* Unknown event type */
	}
}

interior b32
_replay_log(
	GraphusPersistens* gp,
	chorda             log_datum)
{
	i32 positio = ZEPHYRUM;

	dum (positio < log_datum.mensura)
	{
		i64            length;
		chorda         eventum_datum;
		NuntiumLector* lector;

		/* Read length prefix */
		si (!_legere_varint_ex_buffer(log_datum.datum, log_datum.mensura,
		                              &positio, &length))
		{
			redde FALSUM;
		}

		/* Validate we have enough data */
		si (positio + (i32)length > log_datum.mensura)
		{
			redde FALSUM;  /* Truncated event */
		}

		/* Create reader for this event */
		eventum_datum.datum = log_datum.datum + positio;
		eventum_datum.mensura = (i32)length;

		lector = nuntium_lector_creare(gp->piscina, eventum_datum);
		si (!lector)
		{
			redde FALSUM;
		}

		/* Apply event */
		si (!_applicare_eventum(gp, lector))
		{
			redde FALSUM;
		}

		gp->numerus_eventuum++;
		positio += (i32)length;
	}

	redde VERUM;
}


/* ==================================================
 * Repositorium Interface Implementation
 * ================================================== */

/* Forward declarations */
interior Entitas* _gp_capere_entitatem(vacuum* datum, chorda* id);
interior Xar* _gp_capere_entitates_relatae(vacuum* datum, Entitas* entitas, chorda* genus);
interior Xar* _gp_quaerere_cum_nota(vacuum* datum, chorda* nota);
interior Xar* _gp_quaerere_cum_praefixo_notae(vacuum* datum, constans character* praefixum);
interior Xar* _gp_quaerere_textum(vacuum* datum, constans character* query);
interior Xar* _gp_capere_radices(vacuum* datum);
interior Relatio* _gp_capere_relatio(vacuum* datum, chorda* relatio_id);
interior Xar* _gp_capere_relationes_ad(vacuum* datum, chorda* entitas_id);
interior Entitas* _gp_entitas_creare(vacuum* datum, constans character* genus);
interior b32 _gp_entitas_delere(vacuum* datum, chorda* id);
interior b32 _gp_proprietas_ponere(vacuum* datum, Entitas* entitas, constans character* clavis, constans character* valor);
interior b32 _gp_proprietas_delere(vacuum* datum, Entitas* entitas, constans character* clavis);
interior Relatio* _gp_relatio_addere(vacuum* datum, Entitas* entitas, constans character* genus, chorda* destinatio_id);
interior b32 _gp_relatio_delere(vacuum* datum, chorda* relatio_id);
interior b32 _gp_nota_addere(vacuum* datum, Entitas* entitas, constans character* nota);
interior b32 _gp_nota_delere(vacuum* datum, Entitas* entitas, constans character* nota);

/* Read operations - delegate to underlying graphus */

interior Entitas*
_gp_capere_entitatem(
	vacuum* datum,
	chorda* id)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	redde gp->repositorium->capere_entitatem(gp->graphus, id);
}

interior Xar*
_gp_capere_entitates_relatae(
	vacuum*  datum,
	Entitas* entitas,
	chorda*  genus)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	redde gp->repositorium->capere_entitates_relatae(gp->graphus, entitas, genus);
}

interior Xar*
_gp_quaerere_cum_nota(
	vacuum* datum,
	chorda* nota)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	redde gp->repositorium->quaerere_cum_nota(gp->graphus, nota);
}

interior Xar*
_gp_quaerere_cum_praefixo_notae(
	vacuum*             datum,
	constans character* praefixum)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	redde gp->repositorium->quaerere_cum_praefixo_notae(gp->graphus, praefixum);
}

interior Xar*
_gp_quaerere_textum(
	vacuum*             datum,
	constans character* query)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	redde gp->repositorium->quaerere_textum(gp->graphus, query);
}

interior Xar*
_gp_capere_radices(
	vacuum* datum)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	redde gp->repositorium->capere_radices(gp->graphus);
}

interior Relatio*
_gp_capere_relatio(
	vacuum* datum,
	chorda* relatio_id)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	redde gp->repositorium->capere_relatio(gp->graphus, relatio_id);
}

interior Xar*
_gp_capere_relationes_ad(
	vacuum* datum,
	chorda* entitas_id)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	redde gp->repositorium->capere_relationes_ad(gp->graphus, entitas_id);
}

/* Write operations - log event then apply to graphus */

interior Entitas*
_gp_entitas_creare(
	vacuum*             datum,
	constans character* genus)
{
	GraphusPersistens*   gp = (GraphusPersistens*)datum;
	InternamentumChorda* intern;
	chorda               entitas_id;
	chorda*              id_internatum;
	chorda*              genus_internatum;
	chorda               genus_chorda;
	Entitas*             entitas;

	intern = graphus_entitatum_internamentum(gp->graphus);

	/* Generate new entity ID */
	entitas_id = uuidv7_creare(gp->piscina);
	si (!entitas_id.datum)
	{
		redde NIHIL;
	}

	/* Create genus chorda */
	genus_chorda = chorda_ex_literis(genus, gp->piscina);
	si (!genus_chorda.datum)
	{
		redde NIHIL;
	}

	/* Log the event first */
	si (!_scribere_eventum_creare_entitas(gp, entitas_id, genus_chorda))
	{
		redde NIHIL;
	}

	/* Now apply to in-memory graph */
	id_internatum = chorda_internare(intern, entitas_id);
	si (!id_internatum)
	{
		redde NIHIL;
	}

	genus_internatum = chorda_internare_ex_literis(intern, genus);
	si (!genus_internatum)
	{
		redde NIHIL;
	}

	entitas = entitas_creare(gp->piscina, id_internatum, genus_internatum);
	si (!entitas)
	{
		redde NIHIL;
	}

	si (!graphus_entitatum_addere_entitatem(gp->graphus, entitas))
	{
		redde NIHIL;
	}

	redde entitas;
}

interior b32
_gp_entitas_delere(
	vacuum* datum,
	chorda* id)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;

	si (!id)
	{
		redde FALSUM;
	}

	/* Log event */
	si (!_scribere_eventum_delere_entitas(gp, *id))
	{
		redde FALSUM;
	}

	/* Apply to graph */
	redde gp->repositorium->entitas_delere(gp->graphus, id);
}

interior b32
_gp_proprietas_ponere(
	vacuum*             datum,
	Entitas*            entitas,
	constans character* clavis,
	constans character* valor)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	chorda             clavis_chorda;
	chorda             valor_chorda;

	si (!entitas || !clavis || !valor)
	{
		redde FALSUM;
	}

	clavis_chorda = chorda_ex_literis(clavis, gp->piscina);
	valor_chorda = chorda_ex_literis(valor, gp->piscina);

	si (!clavis_chorda.datum || !valor_chorda.datum)
	{
		redde FALSUM;
	}

	/* Log event */
	si (!_scribere_eventum_ponere_proprietas(gp, *entitas->id, clavis_chorda, valor_chorda))
	{
		redde FALSUM;
	}

	/* Apply to graph */
	redde gp->repositorium->proprietas_ponere(gp->graphus, entitas, clavis, valor);
}

interior b32
_gp_proprietas_delere(
	vacuum*             datum,
	Entitas*            entitas,
	constans character* clavis)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	chorda             clavis_chorda;

	si (!entitas || !clavis)
	{
		redde FALSUM;
	}

	clavis_chorda = chorda_ex_literis(clavis, gp->piscina);
	si (!clavis_chorda.datum)
	{
		redde FALSUM;
	}

	/* Log event */
	si (!_scribere_eventum_delere_proprietas(gp, *entitas->id, clavis_chorda))
	{
		redde FALSUM;
	}

	/* Apply to graph */
	redde gp->repositorium->proprietas_delere(gp->graphus, entitas, clavis);
}

interior Relatio*
_gp_relatio_addere(
	vacuum*             datum,
	Entitas*            entitas,
	constans character* genus,
	chorda*             destinatio_id)
{
	GraphusPersistens*   gp = (GraphusPersistens*)datum;
	InternamentumChorda* intern;
	chorda               relatio_id;
	chorda               genus_chorda;
	chorda*              relatio_id_internatum;
	chorda*              genus_internatum;
	Relatio*             relatio;

	si (!entitas || !genus || !destinatio_id)
	{
		redde NIHIL;
	}

	intern = graphus_entitatum_internamentum(gp->graphus);

	/* Generate relation ID */
	relatio_id = uuidv7_creare(gp->piscina);
	si (!relatio_id.datum)
	{
		redde NIHIL;
	}

	genus_chorda = chorda_ex_literis(genus, gp->piscina);
	si (!genus_chorda.datum)
	{
		redde NIHIL;
	}

	/* Log event */
	si (!_scribere_eventum_addere_relatio(gp, *entitas->id, relatio_id,
	                                       genus_chorda, *destinatio_id))
	{
		redde NIHIL;
	}

	/* Apply to graph */
	relatio_id_internatum = chorda_internare(intern, relatio_id);
	si (!relatio_id_internatum)
	{
		redde NIHIL;
	}

	genus_internatum = chorda_internare_ex_literis(intern, genus);
	si (!genus_internatum)
	{
		redde NIHIL;
	}

	relatio = entitas_relatio_addere_cum_id(
		entitas,
		gp->piscina,
		intern,
		relatio_id_internatum,
		genus_internatum,
		destinatio_id);

	si (!relatio)
	{
		redde NIHIL;
	}

	graphus_entitatum_registrare_relatio(gp->graphus, relatio);

	redde relatio;
}

interior b32
_gp_relatio_delere(
	vacuum* datum,
	chorda* relatio_id)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	Relatio*           relatio;
	chorda             entitas_id_dummy;

	si (!relatio_id)
	{
		redde FALSUM;
	}

	/* Need to find source entity for logging */
	relatio = gp->repositorium->capere_relatio(gp->graphus, relatio_id);
	si (!relatio)
	{
		redde FALSUM;
	}

	/* We don't have source entity ID easily, use empty for now */
	entitas_id_dummy.datum = NIHIL;
	entitas_id_dummy.mensura = ZEPHYRUM;

	/* Log event */
	si (!_scribere_eventum_delere_relatio(gp, entitas_id_dummy, *relatio_id))
	{
		redde FALSUM;
	}

	/* Apply to graph */
	redde gp->repositorium->relatio_delere(gp->graphus, relatio_id);
}

interior b32
_gp_nota_addere(
	vacuum*             datum,
	Entitas*            entitas,
	constans character* nota)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	chorda             nota_chorda;

	si (!entitas || !nota)
	{
		redde FALSUM;
	}

	nota_chorda = chorda_ex_literis(nota, gp->piscina);
	si (!nota_chorda.datum)
	{
		redde FALSUM;
	}

	/* Log event */
	si (!_scribere_eventum_addere_nota(gp, *entitas->id, nota_chorda))
	{
		redde FALSUM;
	}

	/* Apply to graph */
	redde gp->repositorium->nota_addere(gp->graphus, entitas, nota);
}

interior b32
_gp_nota_delere(
	vacuum*             datum,
	Entitas*            entitas,
	constans character* nota)
{
	GraphusPersistens* gp = (GraphusPersistens*)datum;
	chorda             nota_chorda;

	si (!entitas || !nota)
	{
		redde FALSUM;
	}

	nota_chorda = chorda_ex_literis(nota, gp->piscina);
	si (!nota_chorda.datum)
	{
		redde FALSUM;
	}

	/* Log event */
	si (!_scribere_eventum_delere_nota(gp, *entitas->id, nota_chorda))
	{
		redde FALSUM;
	}

	/* Apply to graph */
	redde gp->repositorium->nota_delere(gp->graphus, entitas, nota);
}


/* ==================================================
 * Creare Repositorium pro GraphusPersistens
 * ================================================== */

interior EntitasRepositorium*
_creare_gp_repositorium(
	GraphusPersistens* gp)
{
	EntitasRepositorium* repo;

	repo = (EntitasRepositorium*)piscina_allocare(gp->piscina,
	                                               magnitudo(EntitasRepositorium));
	si (!repo)
	{
		redde NIHIL;
	}

	repo->datum = gp;

	/* Read operations */
	repo->capere_entitatem            = _gp_capere_entitatem;
	repo->capere_entitates_relatae    = _gp_capere_entitates_relatae;
	repo->quaerere_cum_nota           = _gp_quaerere_cum_nota;
	repo->quaerere_cum_praefixo_notae = _gp_quaerere_cum_praefixo_notae;
	repo->quaerere_textum             = _gp_quaerere_textum;
	repo->capere_radices              = _gp_capere_radices;
	repo->capere_relatio              = _gp_capere_relatio;
	repo->capere_relationes_ad        = _gp_capere_relationes_ad;

	/* Write operations */
	repo->entitas_creare              = _gp_entitas_creare;
	repo->entitas_delere              = _gp_entitas_delere;
	repo->proprietas_ponere           = _gp_proprietas_ponere;
	repo->proprietas_delere           = _gp_proprietas_delere;
	repo->relatio_addere              = _gp_relatio_addere;
	repo->relatio_delere              = _gp_relatio_delere;
	repo->nota_addere                 = _gp_nota_addere;
	repo->nota_delere                 = _gp_nota_delere;

	redde repo;
}


/* ==================================================
 * Public API
 * ================================================== */

GraphusPersistens*
graphus_persistens_creare(
	Piscina*            piscina,
	constans character* via_log)
{
	GraphusPersistens* gp;
	b32                log_existit;
	chorda             log_datum;
	memoriae_index     via_len;
	character*         via_copy;

	si (!piscina || !via_log)
	{
		redde NIHIL;
	}

	/* Allocate structure */
	gp = (GraphusPersistens*)piscina_allocare(piscina, magnitudo(GraphusPersistens));
	si (!gp)
	{
		redde NIHIL;
	}

	gp->piscina = piscina;
	gp->numerus_eventuum = ZEPHYRUM;

	/* Copy path */
	via_len = strlen(via_log);
	via_copy = (character*)piscina_allocare(piscina, via_len + I);
	si (!via_copy)
	{
		redde NIHIL;
	}
	memcpy(via_copy, via_log, via_len + I);
	gp->via_log.datum = (i8*)via_copy;
	gp->via_log.mensura = (i32)via_len;

	/* Create in-memory graph */
	gp->graphus = graphus_entitatum_creare(piscina);
	si (!gp->graphus)
	{
		redde NIHIL;
	}

	/* Get repositorium from graphus (for internal use during replay) */
	gp->repositorium = graphus_entitatum_repositorium_creare(gp->graphus);
	si (!gp->repositorium)
	{
		redde NIHIL;
	}

	/* Check if log exists */
	log_existit = filum_existit(via_log);

	/* If log exists, replay it */
	si (log_existit)
	{
		log_datum = filum_legere_totum(via_log, piscina);
		si (log_datum.datum && log_datum.mensura > ZEPHYRUM)
		{
			si (!_replay_log(gp, log_datum))
			{
				redde NIHIL;  /* Replay failed */
			}
		}
	}

	/* Open log for appending */
	gp->scriptor = filum_scriptor_aperire(via_log, FILUM_MODUS_APPENDERE, piscina);
	si (!gp->scriptor)
	{
		redde NIHIL;
	}

	redde gp;
}

GraphusPersistens*
graphus_persistens_aperire(
	Piscina*            piscina,
	constans character* via_log)
{
	/* Verify file exists first */
	si (!filum_existit(via_log))
	{
		redde NIHIL;
	}

	redde graphus_persistens_creare(piscina, via_log);
}

vacuum
graphus_persistens_claudere(
	GraphusPersistens* gp)
{
	si (!gp)
	{
		redde;
	}

	si (gp->scriptor)
	{
		filum_scriptor_sync(gp->scriptor);
		filum_scriptor_claudere(gp->scriptor);
		gp->scriptor = NIHIL;
	}
}

EntitasRepositorium*
graphus_persistens_repositorium(
	GraphusPersistens* gp)
{
	si (!gp)
	{
		redde NIHIL;
	}

	/* Create wrapper repositorium that logs writes */
	redde _creare_gp_repositorium(gp);
}

GraphusEntitatum*
graphus_persistens_graphus(
	GraphusPersistens* gp)
{
	si (!gp)
	{
		redde NIHIL;
	}

	redde gp->graphus;
}

b32
graphus_persistens_sync(
	GraphusPersistens* gp)
{
	si (!gp || !gp->scriptor)
	{
		redde FALSUM;
	}

	redde filum_scriptor_sync(gp->scriptor);
}

i32
graphus_persistens_numerus_eventuum(
	GraphusPersistens* gp)
{
	si (!gp)
	{
		redde ZEPHYRUM;
	}

	redde gp->numerus_eventuum;
}

memoriae_index
graphus_persistens_mensura_log(
	GraphusPersistens* gp)
{
	character* via_cstr;

	si (!gp)
	{
		redde ZEPHYRUM;
	}

	via_cstr = chorda_ut_cstr(gp->via_log, gp->piscina);
	si (!via_cstr)
	{
		redde ZEPHYRUM;
	}

	redde filum_mensura(via_cstr);
}
