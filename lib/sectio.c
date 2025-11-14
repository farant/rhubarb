#include "sectio.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Structurae Internae - Mach-O Segment/Section Headers
 * ================================================== */

/* Segment command 64-bit */
nomen structura {
	i32 cmd;
	i32 cmdsize;
	character segname[XVI];
	memoriae_index vmaddr;
	memoriae_index vmsize;
	memoriae_index fileoff;
	memoriae_index filesize;
	i32 maxprot;
	i32 initprot;
	i32 nsects;
	i32 flags;
} _SegmentCommand64;

/* Section 64-bit */
nomen structura {
	character sectname[XVI];
	character segname[XVI];
	memoriae_index addr;
	memoriae_index size;
	i32 offset;
	i32 align;
	i32 reloff;
	i32 nreloc;
	i32 flags;
	i32 reserved1;
	i32 reserved2;
	i32 reserved3;
} _Section64;


/* ==================================================
 * Structurae Opacae - Implementatio
 * ================================================== */

structura Sectio {
	constans MachO* macho;           /* Back-reference */
	character nomen_segmenti[XVI];   /* Segment name */
	character nomen_sectionis[XVI];  /* Section name */
	memoriae_index offset;           /* File offset */
	memoriae_index mensura;          /* Size */
	memoriae_index addressa;         /* VM address */
	i32 ordinatio;                   /* Alignment */
	i32 vexilla;                     /* Flags */
};


/* ==================================================
 * Error Tracking
 * ================================================== */

hic_manens character _sectio_error_buffer[CCLVI];
hic_manens       b32 _sectio_error_est = FALSUM;

interior vacuum
_sectio_error_ponere(
	constans character* message)
{
	strncpy(_sectio_error_buffer, message, CCLV);
	_sectio_error_buffer[CCLV] = '\0';
	_sectio_error_est          = VERUM;
}

interior vacuum
_sectio_error_purgare(
	vacuum)
{
	_sectio_error_est              = FALSUM;
	_sectio_error_buffer[ZEPHYRUM] = '\0';
}


/* ==================================================
 * Adiutores Interni
 * ================================================== */

/* Copiare nomen ex char[16] ad chordam
 * Nomen potest non terminari cum \0, ergo cavendum */
interior chorda
_sectio_nomen_ex_buffer(
	constans character* buffer,
	       Piscina* piscina)
{
	i32 length;
	character* copy;

	/* Invenire longitudinem (maximum 16) */
	length = ZEPHYRUM;
	dum (length < XVI && buffer[length] != '\0')
	{
		length++;
	}

	/* Allocare et copiare */
	copy = (character*)piscina_allocare(piscina, (memoriae_index)length + I);
	si (!copy)
	{
		chorda vacuum_str;
		vacuum_str.datum   = NIHIL;
		vacuum_str.mensura = ZEPHYRUM;
		redde vacuum_str;
	}

	memcpy(copy, buffer, (size_t)length);
	copy[length] = '\0';

	redde chorda_ex_literis(copy, piscina);
}

/* Comparare nomen cum char[16] buffer */
interior b32
_sectio_nomen_aequalis(
	constans character* buffer,
	constans character* name)
{
	i32 i;

	per (i = ZEPHYRUM; i < XVI; i++)
	{
		si (buffer[i] != name[i])
		{
			redde FALSUM;
		}

		/* Ambo terminati */
		si (buffer[i] == '\0' && name[i] == '\0')
		{
			redde VERUM;
		}

		/* Unus terminatus, alter non */
		si (buffer[i] == '\0' || name[i] == '\0')
		{
			redde FALSUM;
		}
	}

	/* Si hic pervenit, primi 16 characteres aequales */
	/* Verificare si name habet characteres ultra */
	redde name[XVI] == '\0';
}

/* Creare Sectio ex _Section64 */
interior Sectio*
_sectio_creare_ex_section64(
	constans MachO* macho,
	constans _Section64* section,
	       Piscina* piscina)
{
	Sectio* sectio;

	sectio = (Sectio*)piscina_allocare(piscina, magnitudo(Sectio));
	si (!sectio)
	{
		_sectio_error_ponere("piscina_allocare fracta");
		redde NIHIL;
	}

	sectio->macho = macho;

	/* Copiare nomina */
	memcpy(sectio->nomen_segmenti, section->segname, XVI);
	memcpy(sectio->nomen_sectionis, section->sectname, XVI);

	/* Copiare attributa */
	sectio->offset    = section->offset;
	sectio->mensura   = section->size;
	sectio->addressa  = section->addr;
	sectio->ordinatio = section->align;
	sectio->vexilla   = section->flags;

	redde sectio;
}


/* ==================================================
 * Iteratio - Omnes Sectiones
 * ================================================== */

SectioIterator
sectio_iterator_initium(
	constans MachO* macho)
{
	SectioIterator iter;

	iter.macho            = macho;
	iter.mandatum_index   = ZEPHYRUM;
	iter.sectio_index     = ZEPHYRUM;
	iter.sectio_numerus   = ZEPHYRUM;
	iter.offset_currens   = ZEPHYRUM;

	redde iter;
}

Sectio*
sectio_iterator_proximum(
	SectioIterator* iter)
{
	MachoIteratorMandatum mandatum_iter;
	MandatumOnustum* mandatum;
	constans _SegmentCommand64* segment;
	constans _Section64* sections;
	constans _Section64* section;
	i32 mandatum_index_currens;

	si (!iter || !iter->macho)
	{
		redde NIHIL;
	}

	/* Initializare iterator mandatorum */
	mandatum_iter = macho_iterator_mandatorum_initium(iter->macho);
	mandatum_index_currens = ZEPHYRUM;

	/* Si in medio segmenti, reddere sectionem proximam ex segmento currenti */
	si (iter->sectio_index > ZEPHYRUM && iter->sectio_index < iter->sectio_numerus)
	{
		/* Saltare ad mandatum currentem */
		dum (mandatum_index_currens < iter->mandatum_index)
		{
			mandatum = macho_iterator_mandatorum_proximum(&mandatum_iter);
			si (!mandatum)
			{
				redde NIHIL;
			}
			mandatum_index_currens++;
		}

		/* Obtinere mandatum currens (nunc sumus ad index correctus) */
		mandatum = macho_iterator_mandatorum_proximum(&mandatum_iter);
		si (!mandatum || mandatum_genus(mandatum) != MACHO_LC_SEGMENT_64)
		{
			redde NIHIL;
		}

		segment = (constans _SegmentCommand64*)mandatum_datum(mandatum);
		sections = (constans _Section64*)((constans i8*)segment +
		           magnitudo(_SegmentCommand64));
		section = &sections[iter->sectio_index];
		iter->sectio_index++;

		redde _sectio_creare_ex_section64(
			iter->macho,
			section,
			macho_piscina(iter->macho));
	}

	/* Saltare ad mandatum post ultimum processum */
	dum (mandatum_index_currens <= iter->mandatum_index)
	{
		mandatum = macho_iterator_mandatorum_proximum(&mandatum_iter);
		si (!mandatum)
		{
			redde NIHIL;
		}
		mandatum_index_currens++;
	}

	/* Iterare usque inveniamus LC_SEGMENT_64 cum sectionibus */
	dum ((mandatum = macho_iterator_mandatorum_proximum(&mandatum_iter)) != NIHIL)
	{
		si (mandatum_genus(mandatum) == MACHO_LC_SEGMENT_64)
		{
			segment = (constans _SegmentCommand64*)mandatum_datum(mandatum);

			si (segment->nsects > ZEPHYRUM)
			{
				/* Invenit segmentum cum sectionibus */
				iter->mandatum_index = mandatum_index_currens;
				iter->sectio_index   = I;  /* Proxima sectio ad legere */
				iter->sectio_numerus = segment->nsects;

				/* Reddere primam sectionem */
				sections = (constans _Section64*)((constans i8*)segment +
				           magnitudo(_SegmentCommand64));
				section = &sections[ZEPHYRUM];

				redde _sectio_creare_ex_section64(
					iter->macho,
					section,
					macho_piscina(iter->macho));
			}
		}
		mandatum_index_currens++;
	}

	/* Nullum segmentum cum sectionibus inventum */
	redde NIHIL;
}

b32
sectio_iterator_finis(
	SectioIterator* iter)
{
	si (!iter || !iter->macho)
	{
		redde VERUM;
	}

	/* Verificare si plures sectiones in segmento currenti */
	si (iter->sectio_index < iter->sectio_numerus)
	{
		redde FALSUM;
	}

	/* Verificare si plura segmenta */
	/* TODO: Implementare verificatio */
	redde FALSUM;
}


/* ==================================================
 * Quaestio - Invenire Sectiones
 * ================================================== */

Sectio*
sectio_invenire(
	constans MachO* macho,
	constans character* nomen_segmenti,
	constans character* nomen_sectionis,
	       Piscina* piscina)
{
	MachoIteratorMandatum mandatum_iter;
	MandatumOnustum* mandatum;
	constans _SegmentCommand64* segment;
	constans _Section64* sections;
	constans _Section64* section;
	i32 i;

	si (!macho || !nomen_segmenti || !nomen_sectionis || !piscina)
	{
		_sectio_error_ponere("Parametri NIHIL");
		redde NIHIL;
	}

	_sectio_error_purgare();

	/* Iterare omnia mandata oneris */
	mandatum_iter = macho_iterator_mandatorum_initium(macho);

	dum ((mandatum = macho_iterator_mandatorum_proximum(&mandatum_iter)) != NIHIL)
	{
		si (mandatum_genus(mandatum) == MACHO_LC_SEGMENT_64)
		{
			segment = (constans _SegmentCommand64*)mandatum_datum(mandatum);

			/* Verificare nomen segmenti */
			si (!_sectio_nomen_aequalis(segment->segname, nomen_segmenti))
			{
				perge;
			}

			/* Segmentum inventum, quaerere sectionem */
			sections = (constans _Section64*)((constans i8*)segment +
			           magnitudo(_SegmentCommand64));

			per (i = ZEPHYRUM; i < segment->nsects; i++)
			{
				section = &sections[i];

				si (_sectio_nomen_aequalis(section->sectname, nomen_sectionis))
				{
					/* Sectio inventa! */
					redde _sectio_creare_ex_section64(macho, section, piscina);
				}
			}
		}
	}

	/* Non inventa */
	redde NIHIL;
}


/* ==================================================
 * Interrogatio - Attributa Sectionis
 * ================================================== */

chorda
sectio_nomen(
	constans Sectio* sectio,
	       Piscina* piscina)
{
	si (!sectio || !piscina)
	{
		chorda vacuum_str;
		vacuum_str.datum   = NIHIL;
		vacuum_str.mensura = ZEPHYRUM;
		redde vacuum_str;
	}

	redde _sectio_nomen_ex_buffer(sectio->nomen_sectionis, piscina);
}

chorda
sectio_nomen_segmenti(
	constans Sectio* sectio,
	       Piscina* piscina)
{
	si (!sectio || !piscina)
	{
		chorda vacuum_str;
		vacuum_str.datum   = NIHIL;
		vacuum_str.mensura = ZEPHYRUM;
		redde vacuum_str;
	}

	redde _sectio_nomen_ex_buffer(sectio->nomen_segmenti, piscina);
}

chorda
sectio_datum(
	constans Sectio* sectio)
{
	constans i8* datum;
	unio {
		constans i8* constans_ptr;
		i8* mutable_ptr;
	} cast_helper;

	si (!sectio || !sectio->macho)
	{
		chorda vacuum_str;
		vacuum_str.datum   = NIHIL;
		vacuum_str.mensura = ZEPHYRUM;
		redde vacuum_str;
	}

	datum = macho_datum(sectio->macho);
	si (!datum)
	{
		chorda vacuum_str;
		vacuum_str.datum   = NIHIL;
		vacuum_str.mensura = ZEPHYRUM;
		redde vacuum_str;
	}

	/* Cast away const for zero-copy chorda view
	 * Data is read-only but chorda.datum type is i8* (not const)
	 * Using union to cast without triggering -Wcast-qual */
	cast_helper.constans_ptr = datum + sectio->offset;
	redde chorda_ex_buffer(cast_helper.mutable_ptr, (i32)sectio->mensura);
}

memoriae_index
sectio_mensura(
	constans Sectio* sectio)
{
	si (!sectio) redde ZEPHYRUM;
	redde sectio->mensura;
}

memoriae_index
sectio_addressa(
	constans Sectio* sectio)
{
	si (!sectio) redde ZEPHYRUM;
	redde sectio->addressa;
}

i32
sectio_ordinatio(
	constans Sectio* sectio)
{
	si (!sectio) redde ZEPHYRUM;
	redde sectio->ordinatio;
}

i32
sectio_vexilla(
	constans Sectio* sectio)
{
	si (!sectio) redde ZEPHYRUM;
	redde sectio->vexilla;
}


/* ==================================================
 * Utilitas - Genus Sectionis
 * ================================================== */

i32
sectio_extrahere_genus(
	i32 vexilla)
{
	redde vexilla & SECTIO_GENUS_MASK;
}

b32
sectio_est_instructiones(
	constans Sectio* sectio)
{
	i32 vexilla;

	si (!sectio) redde FALSUM;

	vexilla = sectio->vexilla;

	redde (vexilla & SECTIO_ATTR_PURE_INSTRUCTIONS) != ZEPHYRUM ||
	       (vexilla & SECTIO_ATTR_SOME_INSTRUCTIONS) != ZEPHYRUM;
}

b32
sectio_est_debug(
	constans Sectio* sectio)
{
	si (!sectio) redde FALSUM;

	redde (sectio->vexilla & SECTIO_ATTR_DEBUG) != ZEPHYRUM;
}


/* ==================================================
 * Errores
 * ================================================== */

constans character*
sectio_error_recens(
	vacuum)
{
	si (!_sectio_error_est) redde NIHIL;
	redde _sectio_error_buffer;
}
