#include "symbola.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Structurae Internae - Mach-O Symbol Table Format
 * ================================================== */

/* LC_SYMTAB command structure */
nomen structura {
	i32 cmd;
	i32 cmdsize;
	i32 symoff;     /* Offset ad tabula symbolorum */
	i32 nsyms;      /* Numerus symbolorum */
	i32 stroff;     /* Offset ad string table */
	i32 strsize;    /* Mensura string table */
} _SymtabCommand;

/* nlist_64 - Symbol table entry (64-bit) */
nomen structura {
	i32 n_strx;             /* String table offset */
	 i8 n_type;             /* Type flag */
	 i8 n_sect;             /* Section number (1-based) */
	i16 n_desc;             /* Description flags */
	memoriae_index n_value; /* Value (address) */
} _Nlist64;

/* nlist - Symbol table entry (32-bit) */
nomen structura {
	i32 n_strx;
	 i8 n_type;
	 i8 n_sect;
	i16 n_desc;
	i32 n_value;
} _Nlist32;


/* ==================================================
 * Structurae Opacae - Implementatio
 * ================================================== */

structura TabulaSymbolorum {
	constans MachO* macho;              /* Back-reference */
	   constans i8* symbola_datum;      /* Pointer to symbol array */
	   constans i8* chordae_datum;      /* Pointer to string table */
	           i32  numerus_symbolarum; /* Number of symbols */
	           i32  mensura_chordarum;  /* String table size */
	           b32  est_64bit;          /* 64-bit symbols? */
	       Piscina* piscina;
};

structura Symbolum {
	constans TabulaSymbolorum* tabula; /* Back-reference */
	                      i32  index;  /* Index in symbol table */
	                      i32  n_strx;
	                       i8  n_type;
	                       i8  n_sect;
	                      i16  n_desc;
	           memoriae_index  n_value;
};


/* ==================================================
 * Error Tracking
 * ================================================== */

hic_manens character _symbola_error_buffer[CCLVI];
hic_manens       b32 _symbola_error_est = FALSUM;

interior vacuum
_symbola_error_ponere(
	constans character* message)
{
	strncpy(_symbola_error_buffer, message, CCLV);
	_symbola_error_buffer[CCLV] = '\0';
	_symbola_error_est           = VERUM;
}

interior vacuum
_symbola_error_purgare(
	vacuum)
{
	_symbola_error_est               = FALSUM;
	_symbola_error_buffer[ZEPHYRUM]  = '\0';
}


/* ==================================================
 * Adiutores Interni
 * ================================================== */

/* Creare Symbolum ex indice in tabula */
interior Symbolum*
_symbolum_creare_ex_indice(
	constans TabulaSymbolorum* tabula,
	                      i32  index)
{
	Symbolum* symbolum;
	constans _Nlist64* nlist64;
	constans _Nlist32* nlist32;

	si (index < ZEPHYRUM || index >= tabula->numerus_symbolarum)
	{
		redde NIHIL;
	}

	symbolum = (Symbolum*)piscina_allocare(tabula->piscina, magnitudo(Symbolum));
	si (!symbolum)
	{
		_symbola_error_ponere("piscina_allocare fracta");
		redde NIHIL;
	}

	symbolum->tabula = tabula;
	symbolum->index  = index;

	/* Legere nlist entry ex tabula */
	si (tabula->est_64bit)
	{
		nlist64 = ((constans _Nlist64*)tabula->symbola_datum) + index;
		symbolum->n_strx  = nlist64->n_strx;
		symbolum->n_type  = nlist64->n_type;
		symbolum->n_sect  = nlist64->n_sect;
		symbolum->n_desc  = nlist64->n_desc;
		symbolum->n_value = nlist64->n_value;
	}
	alioquin
	{
		nlist32 = ((constans _Nlist32*)tabula->symbola_datum) + index;
		symbolum->n_strx  = nlist32->n_strx;
		symbolum->n_type  = nlist32->n_type;
		symbolum->n_sect  = nlist32->n_sect;
		symbolum->n_desc  = nlist32->n_desc;
		symbolum->n_value = nlist32->n_value;
	}

	redde symbolum;
}

/* Obtinere nomen ex string table per offset */
interior constans character*
_symbola_obtinere_nomen_cstr(
	constans TabulaSymbolorum* tabula,
	                      i32  strx)
{
	si (strx < ZEPHYRUM || strx >= tabula->mensura_chordarum)
	{
		redde "";
	}

	redde (constans character*)(tabula->chordae_datum + strx);
}


/* ==================================================
 * Creatio - Tabula Symbolorum
 * ================================================== */

TabulaSymbolorum*
tabula_symbolorum_ex_macho(
	constans MachO* macho,
	       Piscina* piscina)
{
	       TabulaSymbolorum* tabula;
	  MachoIteratorMandatum  iter;
	        MandatumOnustum* mandatum;
	constans _SymtabCommand* symtab_cmd;
	            constans i8* datum_macho;
	         memoriae_index  mensura_macho;

	si (!macho || !piscina)
	{
		_symbola_error_ponere("macho vel piscina NIHIL");
		redde NIHIL;
	}

	_symbola_error_purgare();

	/* Quaerere LC_SYMTAB mandatum */
	iter = macho_iterator_mandatorum_initium(macho);
	symtab_cmd = NIHIL;

	dum ((mandatum = macho_iterator_mandatorum_proximum(&iter)) != NIHIL)
	{
		si (mandatum_genus(mandatum) == MACHO_LC_SYMTAB)
		{
			symtab_cmd = (constans _SymtabCommand*)mandatum_datum(mandatum);
			frange;
		}
	}

	si (!symtab_cmd)
	{
		_symbola_error_ponere("LC_SYMTAB non inventum");
		redde NIHIL;
	}

	/* Validare offsets */
	datum_macho   = macho_datum(macho);
	mensura_macho = macho_mensura(macho);

	si (!datum_macho)
	{
		_symbola_error_ponere("macho datum NIHIL");
		redde NIHIL;
	}

	/* Verificare limites symbol table */
	si (macho_est_64bit(macho))
	{
		si ((memoriae_index)symtab_cmd->symoff +
		    (memoriae_index)symtab_cmd->nsyms * magnitudo(_Nlist64) > mensura_macho)
		{
			_symbola_error_ponere("Symbol table excedit mensuram fili");
			redde NIHIL;
		}
	}
	alioquin
	{
		si ((memoriae_index)symtab_cmd->symoff +
		    (memoriae_index)symtab_cmd->nsyms * magnitudo(_Nlist32) > mensura_macho)
		{
			_symbola_error_ponere("Symbol table excedit mensuram fili");
			redde NIHIL;
		}
	}

	/* Verificare limites string table */
	si ((memoriae_index)symtab_cmd->stroff +
	    (memoriae_index)symtab_cmd->strsize > mensura_macho)
	{
		_symbola_error_ponere("String table excedit mensuram fili");
		redde NIHIL;
	}

	/* Creare tabulam */
	tabula = (TabulaSymbolorum*)piscina_allocare(piscina, magnitudo(TabulaSymbolorum));
	si (!tabula)
	{
		_symbola_error_ponere("piscina_allocare fracta");
		redde NIHIL;
	}

	tabula->macho              = macho;
	tabula->symbola_datum      = datum_macho + symtab_cmd->symoff;
	tabula->chordae_datum      = datum_macho + symtab_cmd->stroff;
	tabula->numerus_symbolarum = symtab_cmd->nsyms;
	tabula->mensura_chordarum  = symtab_cmd->strsize;
	tabula->est_64bit          = macho_est_64bit(macho);
	tabula->piscina            = piscina;

	redde tabula;
}


/* ==================================================
 * Interrogatio - Tabula
 * ================================================== */

i32
tabula_symbolorum_numerus(
	constans TabulaSymbolorum* tabula)
{
	si (!tabula) redde ZEPHYRUM;
	redde tabula->numerus_symbolarum;
}

Symbolum*
tabula_symbolorum_symbolum(
	constans TabulaSymbolorum* tabula,
	                      i32  index)
{
	si (!tabula) redde NIHIL;
	redde _symbolum_creare_ex_indice(tabula, index);
}


/* ==================================================
 * Iteratio - Omnia Symbola
 * ================================================== */

SymbolorumIterator
symbolorum_iterator_initium(
	constans TabulaSymbolorum* tabula)
{
	SymbolorumIterator iter;

	iter.tabula         = tabula;
	iter.index_currens  = ZEPHYRUM;

	redde iter;
}

Symbolum*
symbolorum_iterator_proximum(
	SymbolorumIterator* iter)
{
	si (!iter || !iter->tabula) redde NIHIL;

	si (iter->index_currens >= iter->tabula->numerus_symbolarum)
	{
		redde NIHIL;
	}

	redde _symbolum_creare_ex_indice(iter->tabula, iter->index_currens++);
}

b32
symbolorum_iterator_finis(
	SymbolorumIterator* iter)
{
	si (!iter || !iter->tabula) redde VERUM;
	redde iter->index_currens >= iter->tabula->numerus_symbolarum;
}


/* ==================================================
 * Quaestio - Invenire Symbola
 * ================================================== */

Symbolum*
symbolum_invenire(
	constans TabulaSymbolorum* tabula,
	constans character* appellatio,
	       Piscina* piscina)
{
	i32 i;
	constans character* sym_nomen;
	Symbolum* symbolum;

	si (!tabula || !appellatio || !piscina)
	{
		redde NIHIL;
	}

	/* Quaestio linearis */
	per (i = ZEPHYRUM; i < tabula->numerus_symbolarum; i++)
	{
		symbolum = _symbolum_creare_ex_indice(tabula, i);
		si (!symbolum) perge;

		sym_nomen = _symbola_obtinere_nomen_cstr(tabula, symbolum->n_strx);

		si (strcmp(sym_nomen, appellatio) == ZEPHYRUM)
		{
			redde symbolum;
		}
	}

	redde NIHIL;
}

Symbolum*
symbolum_invenire_per_addressa(
	constans TabulaSymbolorum* tabula,
	           memoriae_index  addressa,
	                  Piscina* piscina)
{
	i32 i;
	Symbolum* symbolum;

	si (!tabula || !piscina)
	{
		redde NIHIL;
	}

	/* Quaestio linearis */
	per (i = ZEPHYRUM; i < tabula->numerus_symbolarum; i++)
	{
		symbolum = _symbolum_creare_ex_indice(tabula, i);
		si (!symbolum) perge;

		si (symbolum->n_value == addressa)
		{
			redde symbolum;
		}
	}

	redde NIHIL;
}


/* ==================================================
 * Interrogatio - Attributa Symboli
 * ================================================== */

chorda
symbolum_nomen(
	constans Symbolum* symbolum,
	          Piscina* piscina)
{
	constans character* cstr;
	unio {
		constans i8* constans_ptr;
		i8* mutable_ptr;
	} cast_helper;

	si (!symbolum || !piscina)
	{
		chorda vacuum_str;
		vacuum_str.datum   = NIHIL;
		vacuum_str.mensura = ZEPHYRUM;
		redde vacuum_str;
	}

	cstr = _symbola_obtinere_nomen_cstr(symbolum->tabula, symbolum->n_strx);

	/* Cast away const for zero-copy chorda view */
	cast_helper.constans_ptr = (constans i8*)cstr;
	redde chorda_ex_literis((character*)cast_helper.mutable_ptr, piscina);
}

memoriae_index
symbolum_addressa(
	constans Symbolum* symbolum)
{
	si (!symbolum) redde ZEPHYRUM;
	redde symbolum->n_value;
}

i32
symbolum_genus(
	constans Symbolum* symbolum)
{
	si (!symbolum) redde ZEPHYRUM;
	redde (i32)symbolum->n_type;
}

i32
symbolum_numerus_sectionis(
	constans Symbolum* symbolum)
{
	si (!symbolum) redde ZEPHYRUM;
	redde (i32)symbolum->n_sect;
}

i32
symbolum_descriptor(
	constans Symbolum* symbolum)
{
	si (!symbolum) redde ZEPHYRUM;
	redde (i32)symbolum->n_desc;
}


/* ==================================================
 * Utilitas - Interrogationes Booleanas
 * ================================================== */

b32
symbolum_est_functio(
	constans Symbolum* symbolum)
{
	i32 type;

	si (!symbolum) redde FALSUM;

	/* Debug symbols non sunt functiones */
	si (symbolum_est_debug(symbolum)) redde FALSUM;

	/* Undefined symbols non sunt functiones */
	si (symbolum_est_indefinitum(symbolum)) redde FALSUM;

	type = symbolum->n_type & SYMBOLUM_TYPE_MASK;

	/* Symbolum debet esse in sectione */
	si (type != SYMBOLUM_TYPE_SECT) redde FALSUM;

	/* Heuristica: si in sectione (typice 1 = __TEXT,__text), probabiliter functio */
	/* Sectio I typice est __TEXT,__text pro executabilibus */
	si (symbolum->n_sect == I) redde VERUM;

	redde FALSUM;
}

b32
symbolum_est_externum(
	constans Symbolum* symbolum)
{
	si (!symbolum) redde FALSUM;
	redde (symbolum->n_type & SYMBOLUM_EXT) != ZEPHYRUM;
}

b32
symbolum_est_privatum_externum(
	constans Symbolum* symbolum)
{
	si (!symbolum) redde FALSUM;
	redde (symbolum->n_type & SYMBOLUM_PEXT) != ZEPHYRUM;
}

b32
symbolum_est_indefinitum(
	constans Symbolum* symbolum)
{
	i32 type;

	si (!symbolum) redde FALSUM;

	type = symbolum->n_type & SYMBOLUM_TYPE_MASK;
	redde type == SYMBOLUM_TYPE_UNDEF;
}

b32
symbolum_est_debug(
	constans Symbolum* symbolum)
{
	si (!symbolum) redde FALSUM;
	redde (symbolum->n_type & SYMBOLUM_STAB) != ZEPHYRUM;
}

b32
symbolum_est_debile(
	constans Symbolum* symbolum)
{
	si (!symbolum) redde FALSUM;
	redde (symbolum->n_desc & SYMBOLUM_WEAK_DEF) != ZEPHYRUM ||
	       (symbolum->n_desc & SYMBOLUM_WEAK_REF) != ZEPHYRUM;
}


/* ==================================================
 * Convenientia - Filtrationes
 * ================================================== */

Symbolum**
symbola_obtinere_functiones(
	constans TabulaSymbolorum* tabula,
	                      i32* numerus,
	                  Piscina* piscina)
{
	i32 i;
	i32 numerus_functionum;
	Symbolum** functiones;
	Symbolum* sym;
	i32 index;

	si (!tabula || !numerus || !piscina)
	{
		si (numerus) *numerus = ZEPHYRUM;
		redde NIHIL;
	}

	/* Prima ambulatio: numerare functiones */
	numerus_functionum = ZEPHYRUM;
	per (i = ZEPHYRUM; i < tabula->numerus_symbolarum; i++)
	{
		sym = _symbolum_creare_ex_indice(tabula, i);
		si (!sym) perge;

		si (symbolum_est_functio(sym))
		{
			numerus_functionum++;
		}
	}

	si (numerus_functionum == ZEPHYRUM)
	{
		*numerus = ZEPHYRUM;
		redde NIHIL;
	}

	/* Allocare tabulam */
	functiones = (Symbolum**)piscina_allocare(
		piscina,
		(memoriae_index)numerus_functionum * magnitudo(Symbolum*));
	si (!functiones)
	{
		*numerus = ZEPHYRUM;
		redde NIHIL;
	}

	/* Secunda ambulatio: colligere functiones */
	index = ZEPHYRUM;
	per (i = ZEPHYRUM; i < tabula->numerus_symbolarum; i++)
	{
		sym = _symbolum_creare_ex_indice(tabula, i);
		si (!sym) perge;

		si (symbolum_est_functio(sym))
		{
			functiones[index++] = sym;
		}
	}

	*numerus = numerus_functionum;
	redde functiones;
}

Symbolum**
symbola_obtinere_externa_indefinita(
	constans TabulaSymbolorum* tabula,
	                      i32* numerus,
	                  Piscina* piscina)
{
	i32 i;
	i32 numerus_externa;
	Symbolum** externa;
	Symbolum* sym;
	i32 index;

	si (!tabula || !numerus || !piscina)
	{
		si (numerus) *numerus = ZEPHYRUM;
		redde NIHIL;
	}

	/* Prima ambulatio: numerare externa indefinita */
	numerus_externa = ZEPHYRUM;
	per (i = ZEPHYRUM; i < tabula->numerus_symbolarum; i++)
	{
		sym = _symbolum_creare_ex_indice(tabula, i);
		si (!sym) perge;

		si (symbolum_est_indefinitum(sym) && symbolum_est_externum(sym))
		{
			numerus_externa++;
		}
	}

	si (numerus_externa == ZEPHYRUM)
	{
		*numerus = ZEPHYRUM;
		redde NIHIL;
	}

	/* Allocare tabulam */
	externa = (Symbolum**)piscina_allocare(
		piscina,
		(memoriae_index)numerus_externa * magnitudo(Symbolum*));
	si (!externa)
	{
		*numerus = ZEPHYRUM;
		redde NIHIL;
	}

	/* Secunda ambulatio: colligere externa */
	index = ZEPHYRUM;
	per (i = ZEPHYRUM; i < tabula->numerus_symbolarum; i++)
	{
		sym = _symbolum_creare_ex_indice(tabula, i);
		si (!sym) perge;

		si (symbolum_est_indefinitum(sym) && symbolum_est_externum(sym))
		{
			externa[index++] = sym;
		}
	}

	*numerus = numerus_externa;
	redde externa;
}


/* ==================================================
 * Errores
 * ================================================== */

constans character*
symbola_error_recens(
	vacuum)
{
	si (!_symbola_error_est) redde NIHIL;
	redde _symbola_error_buffer;
}
