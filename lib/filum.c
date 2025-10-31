#include "filum.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Platform-specific headers */
#ifdef _WIN32
	#include <sys/stat.h>
	#include <io.h>
#else
	#include <sys/stat.h>
	#include <unistd.h>
#endif

/* ==================================================
 * Structura FilumLector - Internal
 * ================================================== */

structura FilumLector {
         FILUM* descriptum;
           i32  numerus_versus;
           b32  est_finis;
       Piscina* piscina;
     character  buffer[MMMMXCVI]; /* 4096 byte buffer pro legere */
};


/* ==================================================
 * Error Tracking - Thread-unsafe but simple
 * ================================================== */

hic_manens character _filum_error_buffer[CCLVI]; /* 256 bytes */
hic_manens       b32 _filum_error_est = FALSUM;

interior vacuum
_filum_error_ponere(
    constans character* message)
{
	strncpy(_filum_error_buffer, message, CCLV);
	_filum_error_buffer[CCLV] = '\0';
	_filum_error_est = VERUM;
}

interior vacuum
_filum_error_purgare(
    vacuum)
{
	_filum_error_est = FALSUM;
	_filum_error_buffer[ZEPHYRUM] = '\0';
}


/* ==================================================
 * Lectio Simplex
 * ================================================== */

chorda
filum_legere_totum(
	constans character* via,
	           Piscina* piscina)
{
	         FILUM* f;
	memoriae_index  mensura;
	            i8* buffer;
	memoriae_index  legere_summa;
	        chorda  fructus;

	si (!via || !piscina)
	{
		_filum_error_ponere("via vel piscina est NIHIL");
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	_filum_error_purgare();

	/* Aperire filum */
	f = fopen(via, "rb");
	si (!f)
	{
		_filum_error_ponere("fopen fracta");
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	/* Invenire mensuram */
	si (fseek(f, ZEPHYRUM, SEEK_END) != ZEPHYRUM)
	{
		fclose(f);
		_filum_error_ponere("fseek fracta");
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	{
		longus mensura_signed = ftell(f);
		si (mensura_signed < ZEPHYRUM)
		{
			fclose(f);
			_filum_error_ponere("ftell fracta");
			fructus.mensura = ZEPHYRUM;
			fructus.datum   = NIHIL;
			redde fructus;
		}
		mensura = (memoriae_index)mensura_signed;
	}

	si (fseek(f, ZEPHYRUM, SEEK_SET) != ZEPHYRUM)
	{
		fclose(f);
		_filum_error_ponere("fseek ad initium fracta");
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	/* Allocare buffer */
	buffer = (i8*)piscina_allocare(piscina, mensura);
	si (!buffer)
	{
		fclose(f);
		_filum_error_ponere("piscina_allocare fracta");
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	/* Legere contentum */
	legere_summa = fread(buffer, I, mensura, f);
	fclose(f);

	si (legere_summa != mensura)
	{
		_filum_error_ponere("fread fracta - non omnia legere");
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	fructus.mensura = (i32)mensura;
	fructus.datum   = buffer;
	redde fructus;
}

b32
filum_existit(
    constans character* via)
{
	FILUM* f;

	si (!via) redde FALSUM;

	f = fopen(via, "r");
	si (!f) redde FALSUM;

	fclose(f);
	redde VERUM;
}

memoriae_index
filum_mensura(
    constans character* via)
{
	 FILUM* f;
	longus  mensura_signed;

	si (!via) redde ZEPHYRUM;

	f = fopen(via, "rb");
	si (!f) redde ZEPHYRUM;

	si (fseek(f, ZEPHYRUM, SEEK_END) != ZEPHYRUM)
	{
		fclose(f);
		redde ZEPHYRUM;
	}

	mensura_signed = ftell(f);
	fclose(f);

	si (mensura_signed < ZEPHYRUM) redde ZEPHYRUM;

	redde (memoriae_index)mensura_signed;
}


/* ==================================================
 * Lectio per Lineas
 * ================================================== */

FilumLector*
filum_lector_aperire(
	constans character* via,
	           Piscina* piscina)
{
	FilumLector* lector;

	si (!via || !piscina)
	{
		_filum_error_ponere("via vel piscina est NIHIL");
		redde NIHIL;
	}

	_filum_error_purgare();

	lector = (FilumLector*)piscina_allocare(piscina, magnitudo(FilumLector));
	si (!lector)
	{
		_filum_error_ponere("piscina_allocare fracta");
		redde NIHIL;
	}

	lector->descriptum = fopen(via, "r");
	si (!lector->descriptum)
	{
		_filum_error_ponere("fopen fracta");
		redde NIHIL;
	}

	lector->numerus_versus = ZEPHYRUM;
	lector->est_finis      = FALSUM;
	lector->piscina        = piscina;

	redde lector;
}

b32
filum_lector_lineam_proximam(
    FilumLector* lector,
         chorda* linea_out)
{
	             character* result;
	        memoriae_index  longitudo;
	                    i8* buffer_allocatus;
	                   b32  habet_newline;
	    ChordaAedificator* aedificator;
	        memoriae_index  i;

	si (!lector || !linea_out)
	{
		redde FALSUM;
	}

	si (lector->est_finis)
	{
		linea_out->mensura = ZEPHYRUM;
		linea_out->datum   = NIHIL;
		redde FALSUM;
	}

	/* Prima lectio */
	result = fgets(lector->buffer, MMMMXCVI, lector->descriptum);
	si (!result)
	{
		/* EOF vel error */
		lector->est_finis  = VERUM;
		linea_out->mensura = ZEPHYRUM;
		linea_out->datum   = NIHIL;
		redde FALSUM;
	}

	lector->numerus_versus++;

	longitudo = strlen(lector->buffer);

	/* Verificare si habet newline ad finem */
	habet_newline = FALSUM;
	si (longitudo > ZEPHYRUM &&
	    (lector->buffer[longitudo - I] == '\n' ||
	     lector->buffer[longitudo - I] == '\r'))
	{
		habet_newline = VERUM;
	}

	/* Si linea completa (habet newline VEL non implevit buffer),
	 * processare directe */
	si (habet_newline || longitudo < (MMMMXCVI - I))
	{
		/* Removere newline characteres */
		dum (longitudo > ZEPHYRUM &&
		     (lector->buffer[longitudo - I] == '\n' ||
		      lector->buffer[longitudo - I] == '\r'))
		{
			lector->buffer[longitudo - I] = '\0';
			longitudo--;
		}

		/* Allocare ex piscina */
		buffer_allocatus = (i8*)piscina_allocare(lector->piscina, longitudo);
		si (!buffer_allocatus)
		{
			_filum_error_ponere("piscina_allocare fracta");
			linea_out->mensura = ZEPHYRUM;
			linea_out->datum   = NIHIL;
			redde FALSUM;
		}

		memcpy(buffer_allocatus, lector->buffer, longitudo);
		linea_out->mensura = (i32)longitudo;
		linea_out->datum   = buffer_allocatus;

		redde VERUM;
	}

	/* Linea continuat - usare ChordaAedificator pro crescentia dynamica */
	aedificator = chorda_aedificator_creare(lector->piscina, MMMMXCVI * II);
	si (!aedificator)
	{
		_filum_error_ponere("chorda_aedificator_creare fracta");
		linea_out->mensura = ZEPHYRUM;
		linea_out->datum   = NIHIL;
		redde FALSUM;
	}

	/* Appendere primum fragmentum */
	per (i = ZEPHYRUM; i < longitudo; i++)
	{
		si (!chorda_aedificator_appendere_character(aedificator, lector->buffer[i]))
		{
			_filum_error_ponere("chorda_aedificator_appendere fracta");
			linea_out->mensura = ZEPHYRUM;
			linea_out->datum   = NIHIL;
			redde FALSUM;
		}
	}

	/* Legere fragmenta reliqua usque ad newline vel EOF */
	dum (VERUM)
	{
		result = fgets(lector->buffer, MMMMXCVI, lector->descriptum);
		si (!result)
		{
			/* EOF - finire lineam */
			lector->est_finis = VERUM;
			*linea_out = chorda_aedificator_finire(aedificator);
			redde VERUM;
		}

		longitudo = strlen(lector->buffer);

		/* Verificare newline */
		habet_newline = FALSUM;
		si (longitudo > ZEPHYRUM &&
		    (lector->buffer[longitudo - I] == '\n' ||
		     lector->buffer[longitudo - I] == '\r'))
		{
			habet_newline = VERUM;
		}

		/* Removere newline si existit */
		dum (longitudo > ZEPHYRUM &&
		     (lector->buffer[longitudo - I] == '\n' ||
		      lector->buffer[longitudo - I] == '\r'))
		{
			lector->buffer[longitudo - I] = '\0';
			longitudo--;
		}

		/* Appendere fragmentum */
		per (i = ZEPHYRUM; i < longitudo; i++)
		{
			si (!chorda_aedificator_appendere_character(aedificator, lector->buffer[i]))
			{
				_filum_error_ponere("chorda_aedificator_appendere fracta");
				linea_out->mensura = ZEPHYRUM;
				linea_out->datum   = NIHIL;
				redde FALSUM;
			}
		}

		si (habet_newline || longitudo < (MMMMXCVI - I))
		{
			/* Linea completa */
			*linea_out = chorda_aedificator_finire(aedificator);
			redde VERUM;
		}
	}
}

i32
filum_lector_numerus_versus(
    FilumLector* lector)
{
	redde lector ? lector->numerus_versus : ZEPHYRUM;
}

b32
filum_lector_finis(
    FilumLector* lector)
{
	redde lector ? lector->est_finis : VERUM;
}

vacuum
filum_lector_claudere(
    FilumLector* lector)
{
	si (!lector) redde;

	si (lector->descriptum)
	{
		fclose(lector->descriptum);
		lector->descriptum = NIHIL;
	}

	/* Nota: FilumLector ipsa allocata ex piscina,
	 * ergo non liberatur manualiter */
}


/* ==================================================
 * Scriptio
 * ================================================== */

b32
filum_scribere(
	constans character* via,
	            chorda  contentum)
{
	         FILUM* f;
	memoriae_index  scriptus;

	si (!via || !contentum.datum)
	{
		_filum_error_ponere("via vel contentum est NIHIL");
		redde FALSUM;
	}

	_filum_error_purgare();

	f = fopen(via, "wb");
	si (!f)
	{
		_filum_error_ponere("fopen pro scribere fracta");
		redde FALSUM;
	}

	scriptus = fwrite(contentum.datum, I, (memoriae_index)contentum.mensura, f);
	fclose(f);

	si (scriptus != (memoriae_index)contentum.mensura)
	{
		_filum_error_ponere("fwrite fracta - non omnia scripta");
		redde FALSUM;
	}

	redde VERUM;
}

b32
filum_scribere_literis(
	constans character* via,
	constans character* contentum)
{
	         FILUM* f;
	memoriae_index  longitudo;
	memoriae_index  scriptus;

	si (!via || !contentum)
	{
		_filum_error_ponere("via vel contentum est NIHIL");
		redde FALSUM;
	}

	_filum_error_purgare();

	f = fopen(via, "w");
	si (!f)
	{
		_filum_error_ponere("fopen pro scribere fracta");
		redde FALSUM;
	}

	longitudo = strlen(contentum);
	scriptus  = fwrite(contentum, I, longitudo, f);
	fclose(f);

	si (scriptus != longitudo)
	{
		_filum_error_ponere("fwrite fracta");
		redde FALSUM;
	}

	redde VERUM;
}

b32
filum_appendere(
	constans character* via,
	            chorda  contentum)
{
	         FILUM* f;
	memoriae_index  scriptus;

	si (!via || !contentum.datum)
	{
		_filum_error_ponere("via vel contentum est NIHIL");
		redde FALSUM;
	}

	_filum_error_purgare();

	f = fopen(via, "ab");
	si (!f)
	{
		_filum_error_ponere("fopen pro appendere fracta");
		redde FALSUM;
	}

	scriptus = fwrite(contentum.datum, I, (memoriae_index)contentum.mensura, f);
	fclose(f);

	si (scriptus != (memoriae_index)contentum.mensura)
	{
		_filum_error_ponere("fwrite fracta");
		redde FALSUM;
	}

	redde VERUM;
}

b32
filum_appendere_literis(
	constans character* via,
	constans character* contentum)
{
	         FILUM* f;
	memoriae_index  longitudo;
	memoriae_index  scriptus;

	si (!via || !contentum)
	{
		_filum_error_ponere("via vel contentum est NIHIL");
		redde FALSUM;
	}

	_filum_error_purgare();

	f = fopen(via, "a");
	si (!f)
	{
		_filum_error_ponere("fopen pro appendere fracta");
		redde FALSUM;
	}

	longitudo = strlen(contentum);
	scriptus  = fwrite(contentum, I, longitudo, f);
	fclose(f);

	si (scriptus != longitudo)
	{
		_filum_error_ponere("fwrite fracta");
		redde FALSUM;
	}

	redde VERUM;
}


/* ==================================================
 * Manipulatio Filorum
 * ================================================== */

b32
filum_delere(
    constans character* via)
{
	si (!via)
	{
		_filum_error_ponere("via est NIHIL");
		redde FALSUM;
	}

	_filum_error_purgare();

	si (remove(via) != ZEPHYRUM)
	{
		_filum_error_ponere("remove fracta");
		redde FALSUM;
	}

	redde VERUM;
}

b32
filum_movere(
	constans character* via_vetus,
	constans character* via_nova)
{
	si (!via_vetus || !via_nova)
	{
		_filum_error_ponere("via est NIHIL");
		redde FALSUM;
	}

	_filum_error_purgare();

	si (rename(via_vetus, via_nova) != ZEPHYRUM)
	{
		_filum_error_ponere("rename fracta");
		redde FALSUM;
	}

	redde VERUM;
}

b32
filum_copiare(
	constans character* via_fons,
	constans character* via_destinatio)
{
	         FILUM* fons;
	         FILUM* dest;
	     character  buffer[MMMMXCVI];
	memoriae_index  legere_bytes;
	memoriae_index  scriptus_bytes;

	si (!via_fons || !via_destinatio)
	{
		_filum_error_ponere("via est NIHIL");
		redde FALSUM;
	}

	_filum_error_purgare();

	fons = fopen(via_fons, "rb");
	si (!fons)
	{
		_filum_error_ponere("aperire fons fracta");
		redde FALSUM;
	}

	dest = fopen(via_destinatio, "wb");
	si (!dest)
	{
		fclose(fons);
		_filum_error_ponere("aperire destinatio fracta");
		redde FALSUM;
	}

	/* Copiare per buffer */
	dum ((legere_bytes = fread(buffer, I, MMMMXCVI, fons)) > ZEPHYRUM)
	{
		scriptus_bytes = fwrite(buffer, I, legere_bytes, dest);
		si (scriptus_bytes != legere_bytes)
		{
			fclose(fons);
			fclose(dest);
			_filum_error_ponere("fwrite in copiare fracta");
			redde FALSUM;
		}
	}

	fclose(fons);
	fclose(dest);

	redde VERUM;
}


/* ==================================================
 * Interrogatio Status
 * ================================================== */

b32
filum_status(
	constans character* via,
	       FilumStatus* status_out)
{
	structura stat st;

	si (!via || !status_out)
	{
		_filum_error_ponere("via vel status_out est NIHIL");
		redde FALSUM;
	}

	_filum_error_purgare();

	si (stat(via, &st) != ZEPHYRUM)
	{
		_filum_error_ponere("stat fracta");
		redde FALSUM;
	}

	status_out->mensura = (memoriae_index)st.st_size;

#ifdef _WIN32
	status_out->est_directorium = (st.st_mode & _S_IFDIR) != ZEPHYRUM;
	status_out->est_filum       = (st.st_mode & _S_IFREG) != ZEPHYRUM;
	status_out->potest_legere   = (st.st_mode & _S_IREAD) != ZEPHYRUM;
	status_out->potest_scribere = (st.st_mode & _S_IWRITE) != ZEPHYRUM;
#else
	status_out->est_directorium = S_ISDIR(st.st_mode);
	status_out->est_filum       = S_ISREG(st.st_mode);
	status_out->potest_legere   = (st.st_mode & S_IRUSR) != ZEPHYRUM;
	status_out->potest_scribere = (st.st_mode & S_IWUSR) != ZEPHYRUM;
#endif

	redde VERUM;
}


/* ==================================================
 * Errores
 * ================================================== */

constans character*
filum_error_recens(
    vacuum)
{
	si (!_filum_error_est) redde NIHIL;
	redde _filum_error_buffer;
}
