#include "chorda.h"
#include "piscina.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#define CHORDA_FRIATUM_OFFSET  2166136261U
#define CHORDA_FRIATUM_PRIMUS    16777619U


/* ==================================================
 * Constructores
 * ================================================== */

chorda
chorda_ex_literis (
    constans character* cstr,
    Piscina* piscina)
{
    chorda  fructus;
       i32  mensura;
        i8* allocatus;

    si (!cstr || !piscina)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

      mensura = (i32)strlen(cstr);
    allocatus = (i8*)piscina_allocare(piscina, mensura);

    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, cstr, mensura);
    fructus.mensura = mensura;
    fructus.datum   = allocatus;
    redde fructus;
}


chorda
chorda_ex_buffer (
     i8* buffer,
    i32  mensura)
{
    chorda fructus;

    si (!buffer)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    fructus.mensura = mensura;
    fructus.datum   = buffer;

    redde fructus;
}


chorda
chorda_sectio (
    chorda s,
       i32 initium, 
       i32 finis)
{
    chorda fructus;

    si (!s.datum || initium > finis || finis > s.mensura)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    fructus.mensura = finis - initium;
    fructus.datum   = s.datum + initium;
    
    redde fructus;
}

chorda
chorda_transcribere (
     chorda  s,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    memcpy(allocatus, s.datum, s.mensura);

    fructus.mensura = s.mensura;
    fructus.datum   = allocatus;

    redde fructus;
}

chorda
chorda_concatenare (
	chorda  a,
	chorda  b,
	Piscina* piscina)
{
	chorda   fructus;
	    i8*  allocatus;
	   i32   mensura_totalis;

	si (!piscina || !a.datum || !b.datum)
	{
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	mensura_totalis = a.mensura + b.mensura;
	allocatus = (i8*)piscina_allocare(piscina, mensura_totalis);
	si (!allocatus)
	{
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	memcpy(allocatus,                 a.datum, a.mensura);
	memcpy(allocatus + a.mensura, b.datum, b.mensura);

	fructus.mensura = mensura_totalis;
	fructus.datum   = allocatus;

	redde fructus;
}

chorda
chorda_praecidi_laterale (
	 chorda  s,
	Piscina* piscina)
{
	chorda  fructus;
	   i32  initium;
	   i32  finis;
	   i32  i;

	si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
	{
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	initium = ZEPHYRUM;
	finis   = s.mensura;

	per (i = ZEPHYRUM; i < s.mensura; i++)
	{
		si (!isspace((i8)s.datum[i]))
		{
			initium = i;
			frange;
		}
	}

	per (i = s.mensura - I; i >= ZEPHYRUM; i--)
	{
		si (!isspace((i8)s.datum[i]))
		{
			finis = i + I;
			frange;
		}
	}

	si (initium >= finis)
	{
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	redde chorda_sectio(s, initium, finis);
}

/* ==================================================
 * Divisio
 * ================================================== */

chorda_fissio_fructus
chorda_fissio (
		chorda  s,
		character delim,
		Piscina* piscina)
{
	chorda_fissio_fructus fructus;
	                chorda* elementa;
	                   i32  capacitas;
	                   i32  numerus;
	                   i32  initium;
	                   i32  i;

	si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
	{
		fructus.elementa = NIHIL;
		fructus.numerus  = ZEPHYRUM;
		redde fructus;
	}

	capacitas = XVI;
	elementa = (chorda*)piscina_allocare(piscina, capacitas * magnitudo(chorda));
	si (!elementa)
	{
		fructus.elementa = NIHIL;
		fructus.numerus  = ZEPHYRUM;
		redde fructus;
	}

	numerus = ZEPHYRUM;
	initium = ZEPHYRUM;

	per (i = ZEPHYRUM; i <= s.mensura; i++)
	{
		b32 est_delim = (i < s.mensura && s.datum[i] == delim);
		b32 est_finis = (i == s.mensura);

		si (est_delim || est_finis)
		{
			si (numerus >= capacitas)
			{
				capacitas *= II;
				elementa = (chorda*)piscina_allocare(piscina, capacitas * magnitudo(chorda));
				si (!elementa)
				{
					fructus.elementa = NIHIL;
					fructus.numerus  = ZEPHYRUM;
					redde fructus;
				}
			}

			elementa[numerus] = chorda_sectio(s, initium, i);
			numerus++;
			initium = i + I;
		}
	}

	fructus.elementa = elementa;
	fructus.numerus  = numerus;
	redde fructus;
}

/* ==================================================
 * Comparatio
 * ================================================== */


b32 
chorda_aequalis (
    chorda a,
    chorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }

    si (a.datum == b.datum)
    {
        redde VERUM;
    }

    redde memcmp(a.datum, b.datum, a.mensura) == ZEPHYRUM;
}
 
b32
chorda_aequalis_case_insensitivus (
    chorda a,
    chorda b)
{
    memoriae_index i;

    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < a.mensura; i++)
    {
        si (tolower((character)a.datum[i]) != tolower((character)b.datum[i]))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

s32
chorda_comparare (
    chorda a,
    chorda b)
{
    memoriae_index minima_mensura; 
               s32 cmp_result;

    minima_mensura  = a.mensura < b.mensura ? a.mensura : b.mensura;
    cmp_result      = memcmp(a.datum, b.datum, minima_mensura);

    si (cmp_result != ZEPHYRUM)
    {
        redde cmp_result;
    }

    si (a.mensura < b.mensura) redde -I;
    si (a.mensura > b.mensura) redde I;

    redde ZEPHYRUM;
}

/* ==================================================
 * Quaestio
 * ================================================== */

b32
chorda_continet (
    chorda fenum,
    chorda acus)
{
    memoriae_index i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde FALSUM;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum, acus.mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}


b32
chorda_incipit (
    chorda s,
    chorda prefixum)
{
    si (!s.datum || !prefixum.datum || prefixum.mensura > s.mensura)
    {
        redde FALSUM;
    }

    si (prefixum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    redde memcmp(s.datum, prefixum.datum, prefixum.mensura) == ZEPHYRUM;
}



b32
chorda_terminatur (
    chorda s,
    chorda suffixum)
{
    memoriae_index offset;

    si (!s.datum || !suffixum.datum || suffixum.mensura > s.mensura)
    {
        redde FALSUM;
    }

    si (suffixum.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }

    offset = s.mensura - suffixum.mensura;
    redde memcmp(s.datum + offset, suffixum.datum, suffixum.mensura) == ZEPHYRUM;
}
   

chorda
chorda_invenire (
    chorda fenum,
    chorda acus)
{
    memoriae_index i;
            chorda fructus;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;

        redde fructus;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde fenum;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum, acus.mensura) == ZEPHYRUM)
        {
            fructus.mensura = acus.mensura;
            fructus.datum = fenum.datum + i;
            redde fructus;
        }
    }

    fructus.mensura = ZEPHYRUM;
    fructus.datum   = NIHIL;

    redde fructus;
}


i32
chorda_numerare_occurrentia (
    chorda fenum,
    chorda acus)
{
    i32 count;
    i32 positus;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura || acus.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    count   = ZEPHYRUM;
    positus = ZEPHYRUM;

    dum (positus <= fenum.mensura - acus.mensura)
    {
        si (memcmp(fenum.datum + positus, acus.datum, acus.mensura) == ZEPHYRUM)
        {
            count++;
            positus += acus.mensura;
        }
        alioquin
        {
            positus++;
        }
    }

    redde count;
}

/* ==================================================
 * Manipulatio
 * ================================================== */


chorda
chorda_praecidere (
    chorda s)
{
    i32 initium;
    i32 finis;
    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    initium = ZEPHYRUM;
    finis   = s.mensura;

    /* Praecidere initium */
    dum (initium < s.mensura && isspace((character)s.datum[initium]))
    {
        initium++;
    }

    /* Praecidere finis */
    dum (finis > initium && isspace((character)s.datum[finis - I]))
    {
        finis--;
    }

    redde chorda_sectio(s, initium, finis);
}


chorda 
chorda_minuscula (
     chorda  s,
    Piscina* piscina)
{
            chorda  fructus;
         character* allocatus;
    memoriae_index  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;

        redde fructus;
    }

    allocatus = (character*)piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;

        redde fructus;
    }

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        allocatus[i] = (character)tolower((character)s.datum[i]);
    }

    fructus.mensura = s.mensura;
    fructus.datum   = (i8*)(void*)allocatus;

    redde fructus;
}


chorda
chorda_maiuscula (
     chorda  s,
    Piscina* piscina)
{
            chorda  fructus;
         character* allocatus;
    memoriae_index  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;

        redde fructus;
    }

    allocatus = (character*)piscina_allocare(piscina, s.mensura);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;

        redde fructus;
    }

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        allocatus[i] = (character)toupper((character)s.datum[i]);
    }

    fructus.mensura = s.mensura;
    fructus.datum   = (i8*)(void*)allocatus;

    redde fructus;
}

/* ==================================================
 * Convenientia
 * ================================================== */


character*
chorda_ut_cstr (
     chorda  s,
    Piscina* piscina)
{
    character* allocatus;

    si (!s.datum)
    {
        redde NIHIL;
    }

    allocatus = (character*)piscina_allocare(piscina, s.mensura + I);
    si (!allocatus)
    {
        redde NIHIL;
    }

    memcpy(allocatus, s.datum, s.mensura);
    allocatus[s.mensura] = '\0';

    redde allocatus;
}

b32
chorda_ut_s32 (
    chorda  s,
       s32* fructus)
{
          character* cstr_temporalis;
          character* terminus;
    signatus longus  longus_valor;

    si (!s.datum || !fructus || s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    cstr_temporalis = (character*)memoriae_allocare(s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    longus_valor = strtol(cstr_temporalis, &terminus, X);

    si (terminus != cstr_temporalis && *terminus == '\0')
    {
        *fructus = (s32)longus_valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}


b32
chorda_ut_i32 (
    chorda  s,
       i32* fructus)
{
            character* cstr_temporalis;
            character* terminus;
    insignatus longus  valor;

    si (!s.datum || !fructus || s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    cstr_temporalis = (character*)memoriae_allocare(s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    valor = strtoul(cstr_temporalis, &terminus, X);


    /* Confer si tota chorda parata est */
    si (terminus != cstr_temporalis && *terminus == '\0')
    {
        *fructus = (i32)valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}


/* ==================================================
 * FRIATIO
 * ================================================== */

i32 
chorda_friare (
    chorda s)
{
    /* Algoritmus FNV-1a friationis */
    i32 friatum;
    i32 i;

    friatum = CHORDA_FRIATUM_OFFSET;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        friatum ^= (i8)s.datum[i];
        friatum *= CHORDA_FRIATUM_PRIMUS;
    }

    redde friatum;
}
