#include "macho.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================================================
 * Structurae Internae - Mach-O Headers
 * ================================================== */

/* Header Mach-O 64-bit */
nomen structura {
	i32 magica;
	i32 genus_processoris;
	i32 subgenus_processoris;
	i32 genus_filum;
	i32 numerus_mandatorum;
	i32 mensura_mandatorum;
	i32 vexilla;
	i32 reservatus;
} _MachoHeader64;

/* Header Mach-O 32-bit */
nomen structura {
	i32 magica;
	i32 genus_processoris;
	i32 subgenus_processoris;
	i32 genus_filum;
	i32 numerus_mandatorum;
	i32 mensura_mandatorum;
	i32 vexilla;
} _MachoHeader32;

/* Mandatum oneris genericum */
nomen structura {
	i32 genus;
	i32 mensura;
} _MandatumOnustumHeader;

/* Header crassus (fat binary) */
nomen structura {
	i32 magica;
	i32 numerus_architecturae;
} _MachoHeaderCrassus;

/* Architectura in fat binary */
nomen structura {
	i32 genus_processoris;
	i32 subgenus_processoris;
	i32 offset;
	i32 mensura;
	i32 ordinatio;
} _MachoArchitecturaCrassa;


/* ==================================================
 * Structurae Opacae - Implementatio
 * ================================================== */

/* MachoFilum - container */
structura MachoFilum {
	   constans i8*  datum;              /* Datum crudum fili */
	memoriae_index   mensura;            /* Mensura totalis */
	           b32   est_crassus;       /* Fat binary? */
	           i32   numerus_imago;     /* Numerus imaginum */
	         MachO** imagines;          /* Tabula imaginum */
	       Piscina*  piscina;
};

/* MachO - imago singula */
structura MachO {
	   constans i8* datum;              /* Datum crudum imaginis */
	memoriae_index  mensura;            /* Mensura imaginis */
	           b32  est_64bit;         /* 64-bit? */
	           i32  genus_processoris;
	           i32  subgenus_processoris;
	           i32  genus_filum;
	           i32  numerus_mandatorum;
	           i32  mensura_mandatorum;
	memoriae_index  offset_mandatorum;  /* Offset ad prima mandatum */
	       Piscina* piscina;
};

/* MandatumOnustum - mandatum oneris */
structura MandatumOnustum {
	   constans i8* datum;  /* Index ad datum mandati */
	           i32  genus;
	memoriae_index  mensura;
};


/* ==================================================
 * Error Tracking
 * ================================================== */

hic_manens character _macho_error_buffer[CCLVI];
hic_manens       b32 _macho_error_est = FALSUM;

interior vacuum
_macho_error_ponere(
	constans character* message)
{
	strncpy(_macho_error_buffer, message, CCLV);
	_macho_error_buffer[CCLV] = '\0';
	_macho_error_est          = VERUM;
}

interior vacuum
_macho_error_purgare(
	vacuum)
{
	_macho_error_est              = FALSUM;
	_macho_error_buffer[ZEPHYRUM] = '\0';
}


/* ==================================================
 * Adiutores Interni - Validatio
 * ================================================== */

interior b32
_macho_verificare_magicam(
	i32 magica)
{
	/* Verificare big-endian (non sustentum) */
	si (magica == MACHO_MAGICA_32_BYTESWAP ||
	    magica == MACHO_MAGICA_64_BYTESWAP ||
	    magica == MACHO_MAGICA_FAT_BYTESWAP)
	{
		imprimere("ERROR: Big-endian Mach-O non sustentum\n");
		imprimere("       Magica: 0x%x\n", magica);
		exire(I);
	}

	/* Verificare magica valida */
	si (magica == MACHO_MAGICA_32 ||
	    magica == MACHO_MAGICA_64 ||
	    magica == MACHO_MAGICA_FAT ||
	    magica == MACHO_MAGICA_FAT_64)
	{
		redde VERUM;
	}

	_macho_error_ponere("Magica Mach-O invalida");
	redde FALSUM;
}


/* ==================================================
 * Adiutores Interni - Parsitio
 * ================================================== */

interior MachO*
_macho_parsere_imaginem(
	   constans i8* datum,
	memoriae_index  mensura,
	       Piscina* piscina)
{
	                  MachO* macho;
	                    i32  magica;
	constans _MachoHeader64* header64;
	constans _MachoHeader32* header32;

	si (!datum || !piscina)
	{
		_macho_error_ponere("datum vel piscina NIHIL");
		redde NIHIL;
	}

	si (mensura < magnitudo(_MachoHeader32))
	{
		_macho_error_ponere("Filum nimis parvum pro header");
		redde NIHIL;
	}

	/* Legere magicam */
	magica = *(constans i32*)datum;

	si (!_macho_verificare_magicam(magica))
	{
		redde NIHIL;
	}

	/* Verificare non crassus */
	si (magica == MACHO_MAGICA_FAT || magica == MACHO_MAGICA_FAT_64)
	{
		_macho_error_ponere("Speratus imaginem singulam, invenit crassam");
		redde NIHIL;
	}

	/* Allocare structuram MachO */
	macho = (MachO*)piscina_allocare(piscina, magnitudo(MachO));
	si (!macho)
	{
		_macho_error_ponere("piscina_allocare fracta");
		redde NIHIL;
	}

	macho->datum   = datum;
	macho->mensura = mensura;
	macho->piscina = piscina;

	/* Parsere header 64-bit vel 32-bit */
	si (magica == MACHO_MAGICA_64)
	{
		si (mensura < magnitudo(_MachoHeader64))
		{
			_macho_error_ponere("Filum nimis parvum pro header 64-bit");
			redde NIHIL;
		}

		header64 = (constans _MachoHeader64*)datum;

		macho->est_64bit            = VERUM;
		macho->genus_processoris    = header64->genus_processoris;
		macho->subgenus_processoris = header64->subgenus_processoris;
		macho->genus_filum          = header64->genus_filum;
		macho->numerus_mandatorum   = header64->numerus_mandatorum;
		macho->mensura_mandatorum   = header64->mensura_mandatorum;
		macho->offset_mandatorum    = magnitudo(_MachoHeader64);
	}
	alioquin si (magica == MACHO_MAGICA_32)
	{
		header32 = (constans _MachoHeader32*)datum;

		macho->est_64bit            = FALSUM;
		macho->genus_processoris    = header32->genus_processoris;
		macho->subgenus_processoris = header32->subgenus_processoris;
		macho->genus_filum          = header32->genus_filum;
		macho->numerus_mandatorum   = header32->numerus_mandatorum;
		macho->mensura_mandatorum   = header32->mensura_mandatorum;
		macho->offset_mandatorum    = magnitudo(_MachoHeader32);
	}
	alioquin
	{
		_macho_error_ponere("Magica ignota");
		redde NIHIL;
	}

	/* Verificare mensuram mandatorum rationabilem */
	si (macho->offset_mandatorum + macho->mensura_mandatorum > mensura)
	{
		_macho_error_ponere("Mandata oneris excedunt mensuram fili");
		redde NIHIL;
	}

	redde macho;
}


/* ==================================================
 * Creatio - Filum/Container
 * ================================================== */

MachoFilum*
macho_filum_ex_memoria(
	   constans i8* datum,
	memoriae_index  mensura,
	       Piscina* piscina)
{
	                       MachoFilum* filum;
	                              i32  magica;
	     constans _MachoHeaderCrassus* header_crassus;
	constans _MachoArchitecturaCrassa* arch;
	                              i32  i;
	                      constans i8* imago_datum;
	                   memoriae_index  imago_mensura;

	si (!datum || !piscina)
	{
		_macho_error_ponere("datum vel piscina NIHIL");
		redde NIHIL;
	}

	_macho_error_purgare();

	si (mensura < magnitudo(i32))
	{
		_macho_error_ponere("Filum nimis parvum");
		redde NIHIL;
	}

	/* Allocare structuram filum */
	filum = (MachoFilum*)piscina_allocare(piscina, magnitudo(MachoFilum));
	si (!filum)
	{
		_macho_error_ponere("piscina_allocare fracta");
		redde NIHIL;
	}

	filum->datum   = datum;
	filum->mensura = mensura;
	filum->piscina = piscina;

	/* Legere magicam */
	magica = *(constans i32*)datum;

	si (!_macho_verificare_magicam(magica))
	{
		redde NIHIL;
	}

	/* Verificare si crassus (fat) vel singulus */
	si (magica == MACHO_MAGICA_FAT || magica == MACHO_MAGICA_FAT_64)
	{
		/* Fat binary */
		si (mensura < magnitudo(_MachoHeaderCrassus))
		{
			_macho_error_ponere("Filum nimis parvum pro header crassus");
			redde NIHIL;
		}

		header_crassus = (constans _MachoHeaderCrassus*)datum;

		filum->est_crassus    = VERUM;
		filum->numerus_imago  = header_crassus->numerus_architecturae;

		/* LIMITATIO TEMPORARIA: solum una architectura */
		si (filum->numerus_imago > I)
		{
			imprimere("ERROR: Fat binary cum %d architecturis non sustentum nunc\n",
			          filum->numerus_imago);
			imprimere("       Implementatio futura addetur\n");
			exire(I);
		}

		/* Allocare tabulam imaginum */
		filum->imagines = (MachO**)piscina_allocare(
			piscina,
			(memoriae_index)filum->numerus_imago * magnitudo(MachO*));
		si (!filum->imagines)
		{
			_macho_error_ponere("piscina_allocare fracta pro imagines");
			redde NIHIL;
		}

		/* Parsere omnes architecturas */
		arch = (constans _MachoArchitecturaCrassa*)(datum + magnitudo(_MachoHeaderCrassus));

		per (i = ZEPHYRUM; i < filum->numerus_imago; i++)
		{
			si ((memoriae_index)(arch[i].offset + arch[i].mensura) > mensura)
			{
				_macho_error_ponere("Offset architecturae invalidus");
				redde NIHIL;
			}

			imago_datum   = datum + arch[i].offset;
			imago_mensura = arch[i].mensura;

			filum->imagines[i] = _macho_parsere_imaginem(
				imago_datum,
				imago_mensura,
				piscina);

			si (!filum->imagines[i])
			{
				redde NIHIL;
			}
		}
	}
	alioquin
	{
		/* Singulus Mach-O */
		filum->est_crassus   = FALSUM;
		filum->numerus_imago = I;

		/* Allocare tabulam cum una imagine */
		filum->imagines = (MachO**)piscina_allocare(piscina, magnitudo(MachO*));
		si (!filum->imagines)
		{
			_macho_error_ponere("piscina_allocare fracta pro imagines");
			redde NIHIL;
		}

		/* Parsere imaginem singulam */
		filum->imagines[ZEPHYRUM] = _macho_parsere_imaginem(
			datum,
			mensura,
			piscina);

		si (!filum->imagines[ZEPHYRUM])
		{
			redde NIHIL;
		}
	}

	redde filum;
}

MachoFilum*
macho_filum_aperire(
	constans character* via,
	           Piscina* piscina)
{
	    chorda  contentum;
	MachoFilum* filum;

	si (!via || !piscina)
	{
		_macho_error_ponere("via vel piscina NIHIL");
		redde NIHIL;
	}

	_macho_error_purgare();

	/* Legere totum filum */
	contentum = filum_legere_totum(via, piscina);
	si (contentum.mensura == ZEPHYRUM || !contentum.datum)
	{
		constans character* error = filum_error_recens();
		si (error)
		{
			_macho_error_ponere(error);
		}
		alioquin
		{
			_macho_error_ponere("filum_legere_totum fracta");
		}
		redde NIHIL;
	}

	/* Parsere ex memoria */
	filum = macho_filum_ex_memoria(contentum.datum, (memoriae_index)contentum.mensura, piscina);

	redde filum;
}


/* ==================================================
 * Interrogatio - Filum/Container
 * ================================================== */

i32
macho_filum_numerus_imago(
	constans MachoFilum* filum)
{
	si (!filum) redde ZEPHYRUM;
	redde filum->numerus_imago;
}

MachO*
macho_filum_imago(
	constans MachoFilum* filum,
	                i32  index)
{
	si (!filum) redde NIHIL;
	si (index < ZEPHYRUM || index >= filum->numerus_imago) redde NIHIL;
	redde filum->imagines[index];
}

i32
macho_filum_genus_processoris(
	constans MachoFilum* filum,
	                i32  index)
{
	MachO* imago;

	si (!filum) redde ZEPHYRUM;
	si (index < ZEPHYRUM || index >= filum->numerus_imago) redde ZEPHYRUM;

	imago = filum->imagines[index];
	si (!imago) redde ZEPHYRUM;

	redde imago->genus_processoris;
}

i32
macho_filum_subgenus_processoris(
	constans MachoFilum* filum,
	                i32  index)
{
	MachO* imago;

	si (!filum) redde ZEPHYRUM;
	si (index < ZEPHYRUM || index >= filum->numerus_imago) redde ZEPHYRUM;

	imago = filum->imagines[index];
	si (!imago) redde ZEPHYRUM;

	redde imago->subgenus_processoris;
}


/* ==================================================
 * Creatio - Imago Singula (Convenientia)
 * ================================================== */

MachO*
macho_ex_memoria(
	   constans i8* datum,
	memoriae_index  mensura,
	       Piscina* piscina)
{
	MachoFilum* filum;
	     MachO* macho;

	filum = macho_filum_ex_memoria(datum, mensura, piscina);
	si (!filum) redde NIHIL;

	si (filum->numerus_imago != I)
	{
		imprimere("ERROR: Speratus filum singulum, invenit %d imagines\n",
		          filum->numerus_imago);
		exire(I);
	}

	macho = filum->imagines[ZEPHYRUM];
	redde macho;
}

MachO*
macho_aperire(
	constans character* via,
	           Piscina* piscina)
{
	MachoFilum* filum;
	     MachO* macho;

	filum = macho_filum_aperire(via, piscina);
	si (!filum) redde NIHIL;

	si (filum->numerus_imago != I)
	{
		imprimere("ERROR: Speratus filum singulum, invenit %d imagines\n",
		          filum->numerus_imago);
		exire(I);
	}

	macho = filum->imagines[ZEPHYRUM];
	redde macho;
}


/* ==================================================
 * Interrogatio - Header Mach-O
 * ================================================== */

i32
macho_genus_processoris(
	constans MachO* macho)
{
	si (!macho) redde ZEPHYRUM;
	redde macho->genus_processoris;
}

i32
macho_subgenus_processoris(
	constans MachO* macho)
{
	si (!macho) redde ZEPHYRUM;
	redde macho->subgenus_processoris;
}

i32
macho_genus_filum(
	constans MachO* macho)
{
	si (!macho) redde ZEPHYRUM;
	redde macho->genus_filum;
}

i32
macho_numerus_mandatorum(
	constans MachO* macho)
{
	si (!macho) redde ZEPHYRUM;
	redde macho->numerus_mandatorum;
}

b32
macho_est_64bit(
	constans MachO* macho)
{
	si (!macho) redde FALSUM;
	redde macho->est_64bit;
}


/* ==================================================
 * Iteratio - Mandata Oneris
 * ================================================== */

MachoIteratorMandatum
macho_iterator_mandatorum_initium(
	constans MachO* macho)
{
	MachoIteratorMandatum iter;

	iter.macho          = macho;
	iter.index_currens  = ZEPHYRUM;
	iter.offset_currens = ZEPHYRUM;

	redde iter;
}

MandatumOnustum*
macho_iterator_mandatorum_proximum(
	MachoIteratorMandatum* iter)
{
	                MandatumOnustum* mandatum;
	constans _MandatumOnustumHeader* header;
	                    constans i8* mandatum_datum;
	                 memoriae_index  offset_absolutus;

	si (!iter || !iter->macho) redde NIHIL;

	/* Verificare si finis */
	si (iter->index_currens >= iter->macho->numerus_mandatorum)
	{
		redde NIHIL;
	}

	/* Calculare offset absolutum */
	offset_absolutus = iter->macho->offset_mandatorum + (memoriae_index)iter->offset_currens;

	/* Verificare limites */
	si (offset_absolutus + magnitudo(_MandatumOnustumHeader) >
	    iter->macho->mensura)
	{
		_macho_error_ponere("Mandatum excedit mensuram fili");
		redde NIHIL;
	}

	mandatum_datum = iter->macho->datum + offset_absolutus;
	header = (constans _MandatumOnustumHeader*)mandatum_datum;

	/* Verificare mensuram mandati */
	si (header->mensura < magnitudo(_MandatumOnustumHeader))
	{
		_macho_error_ponere("Mensura mandati invalida");
		redde NIHIL;
	}

	si (offset_absolutus + (memoriae_index)header->mensura > iter->macho->mensura)
	{
		_macho_error_ponere("Mandatum excedit mensuram fili");
		redde NIHIL;
	}

	/* Allocare structuram mandati */
	mandatum = (MandatumOnustum*)piscina_allocare(
		iter->macho->piscina,
		magnitudo(MandatumOnustum));
	si (!mandatum)
	{
		_macho_error_ponere("piscina_allocare fracta");
		redde NIHIL;
	}

	mandatum->datum   = mandatum_datum;
	mandatum->genus   = header->genus;
	mandatum->mensura = header->mensura;

	/* Avancere iteratorem */
	iter->index_currens++;
	iter->offset_currens += header->mensura;

	redde mandatum;
}

b32
macho_iterator_mandatorum_finis(
	MachoIteratorMandatum* iter)
{
	si (!iter || !iter->macho) redde VERUM;
	redde iter->index_currens >= iter->macho->numerus_mandatorum;
}


/* ==================================================
 * Interrogatio - Mandata Oneris
 * ================================================== */

i32
mandatum_genus(
	constans MandatumOnustum* mandatum)
{
	si (!mandatum) redde ZEPHYRUM;
	redde mandatum->genus;
}

memoriae_index
mandatum_magnitudo(
	constans MandatumOnustum* mandatum)
{
	si (!mandatum) redde ZEPHYRUM;
	redde mandatum->mensura;
}

constans vacuum*
mandatum_datum(
	constans MandatumOnustum* mandatum)
{
	si (!mandatum) redde NIHIL;
	redde mandatum->datum;
}


/* ==================================================
 * Errores
 * ================================================== */

constans character*
macho_error_recens(
	vacuum)
{
	si (!_macho_error_est) redde NIHIL;
	redde _macho_error_buffer;
}
