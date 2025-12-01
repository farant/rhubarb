#include "chorda.h"
#include "chorda_aedificator.h"
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
	chorda  fructus;
	    i8* allocatus;
	   i32  mensura_totalis;

	si (!piscina)
	{
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	/* Si a vacuus est, redde transcriptionem b */
	si (a.mensura == ZEPHYRUM || !a.datum)
	{
		redde chorda_transcribere(b, piscina);
	}

	/* Si b vacuus est, redde transcriptionem a */
	si (b.mensura == ZEPHYRUM || !b.datum)
	{
		redde chorda_transcribere(a, piscina);
	}

	mensura_totalis = a.mensura + b.mensura;
	allocatus = (i8*)piscina_allocare(piscina, mensura_totalis);
	si (!allocatus)
	{
		fructus.mensura = ZEPHYRUM;
		fructus.datum   = NIHIL;
		redde fructus;
	}

	memcpy(allocatus,             a.datum, (memoriae_index)a.mensura);
	memcpy(allocatus + a.mensura, b.datum, (memoriae_index)b.mensura);

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
	character  delim,
	  Piscina* piscina)
{
	chorda_fissio_fructus  fructus;
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
				chorda* elementa_nova;
				    i32 j;

				capacitas *= II;
				elementa_nova = (chorda*)piscina_allocare(piscina, capacitas * magnitudo(chorda));
				si (!elementa_nova)
				{
					fructus.elementa = NIHIL;
					fructus.numerus  = ZEPHYRUM;
					redde fructus;
				}

				/* Transcribere elementa veteres ad array novum */
				per (j = ZEPHYRUM; j < numerus; j++)
				{
					elementa_nova[j] = elementa[j];
				}

				elementa = elementa_nova;
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
chorda_aequalis_literis (
    chorda              s,
    constans character* cstr)
{
    i32 len;
    i32 i;

    si (!cstr)
    {
        redde FALSUM;
    }

    /* Mensura literarum */
    len = ZEPHYRUM;
    dum (cstr[len] != '\0')
    {
        len++;
    }

    si (s.mensura != len)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < len; i++)
    {
        si ((character)s.datum[i] != cstr[i])
        {
            redde FALSUM;
        }
    }

    redde VERUM;
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


s32
chorda_invenire_index (
    chorda fenum,
    chorda acus)
{
    i32 i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde -I;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    per (i = ZEPHYRUM; i <= fenum.mensura - acus.mensura; i++)
    {
        si (memcmp(fenum.datum + i, acus.datum, (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }

    redde -I;
}


chorda
chorda_invenire_ultimum (
    chorda fenum,
    chorda acus)
{
    chorda fructus;
       s32 i;
       s32 max_i;
       s32 ultima_positio;

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

    ultima_positio = -I;
    max_i = (s32)(fenum.mensura - acus.mensura);

    per (i = max_i; i >= ZEPHYRUM; i--)
    {
        si (memcmp(fenum.datum + i, acus.datum, (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            ultima_positio = i;
            frange;
        }
    }

    si (ultima_positio < ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    fructus.mensura = acus.mensura;
    fructus.datum   = fenum.datum + ultima_positio;
    redde fructus;
}


s32
chorda_invenire_ultimum_index (
    chorda fenum,
    chorda acus)
{
    s32 i;
    s32 max_i;

    si (!fenum.datum || !acus.datum || acus.mensura > fenum.mensura)
    {
        redde -I;
    }

    si (acus.mensura == ZEPHYRUM)
    {
        redde (s32)fenum.mensura;
    }

    max_i = (s32)(fenum.mensura - acus.mensura);

    per (i = max_i; i >= ZEPHYRUM; i--)
    {
        si (memcmp(fenum.datum + i, acus.datum, (memoriae_index)acus.mensura) == ZEPHYRUM)
        {
            redde i;
        }
    }

    redde -I;
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
        si (memcmp(fenum.datum + positus, acus.datum, (memoriae_index)acus.mensura) == ZEPHYRUM)
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

    allocatus = (character*)piscina_allocare(piscina, (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;

        redde fructus;
    }

    per (i = ZEPHYRUM; i < (memoriae_index)s.mensura; i++)
    {
        allocatus[i] = (character)toupper((character)s.datum[i]);
    }

    fructus.mensura = s.mensura;
    fructus.datum   = (i8*)(void*)allocatus;

    redde fructus;
}


chorda
chorda_praecidere_sinistram (
    chorda s)
{
    i32 initium;

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    initium = ZEPHYRUM;

    dum (initium < s.mensura && isspace((character)s.datum[initium]))
    {
        initium++;
    }

    redde chorda_sectio(s, initium, s.mensura);
}


chorda
chorda_praecidere_dextram (
    chorda s)
{
    i32 finis;

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        redde s;
    }

    finis = s.mensura;

    dum (finis > ZEPHYRUM && isspace((character)s.datum[finis - I]))
    {
        finis--;
    }

    redde chorda_sectio(s, ZEPHYRUM, finis);
}


chorda
chorda_substituere (
      chorda  s,
      chorda  antiquum,
      chorda  novum,
    Piscina* piscina)
{
    chorda  fructus;
       i32  numerus_occurrentia;
       i32  mensura_nova;
        i8* allocatus;
       i32  positus_lecti;
       i32  positus_scripti;
       i32  i;

    si (!piscina)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    /* Si antiquum vacuus, redde copiam originalem */
    si (!antiquum.datum || antiquum.mensura == ZEPHYRUM)
    {
        redde chorda_transcribere(s, piscina);
    }

    /* Si s vacuus, redde vacuus */
    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    /* Numerare occurrentias */
    numerus_occurrentia = chorda_numerare_occurrentia(s, antiquum);

    si (numerus_occurrentia == ZEPHYRUM)
    {
        redde chorda_transcribere(s, piscina);
    }

    /* Calculare mensuram novam */
    mensura_nova = s.mensura + (numerus_occurrentia * (novum.mensura - antiquum.mensura));

    si (mensura_nova <= ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    positus_lecti   = ZEPHYRUM;
    positus_scripti = ZEPHYRUM;

    dum (positus_lecti < s.mensura)
    {
        /* Verificare si hic est antiquum */
        si (positus_lecti <= s.mensura - antiquum.mensura &&
            memcmp(s.datum + positus_lecti, antiquum.datum, (memoriae_index)antiquum.mensura) == ZEPHYRUM)
        {
            /* Scribere novum */
            per (i = ZEPHYRUM; i < novum.mensura; i++)
            {
                allocatus[positus_scripti++] = novum.datum[i];
            }
            positus_lecti += antiquum.mensura;
        }
        alioquin
        {
            allocatus[positus_scripti++] = s.datum[positus_lecti++];
        }
    }

    fructus.mensura = mensura_nova;
    fructus.datum   = allocatus;
    redde fructus;
}


chorda
chorda_invertere (
     chorda  s,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;
       i32  i;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        allocatus[i] = s.datum[s.mensura - I - i];
    }

    fructus.mensura = s.mensura;
    fructus.datum   = allocatus;
    redde fructus;
}


chorda
chorda_duplicare (
     chorda  s,
        i32  numerus,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;
       i32  mensura_nova;
       i32  i;

    si (!piscina || numerus <= ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    si (!s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    mensura_nova = s.mensura * numerus;

    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        memcpy(allocatus + (i * s.mensura), s.datum, (memoriae_index)s.mensura);
    }

    fructus.mensura = mensura_nova;
    fructus.datum   = allocatus;
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


/* ==================================================
 * Conversio Casus
 * ================================================== */

/* Interior: Extrahere verba ex chorda
 * Regulae:
 * - Characteres non-alphanumerici sunt delimitatores (omittuntur)
 * - Transitio minuscula→maiuscula incipit verbum novum
 * - Littera post digitum incipit verbum novum
 * - Maiusculae consecutivae ante minusculam: scinde ante ultimam maiusculam
 */
interior chorda_fissio_fructus
_extrahere_verba (
       chorda  s,
      Piscina* piscina)
{
    chorda_fissio_fructus  fructus;
                   chorda* verba;
                      i32  capacitas;
                      i32  numerus;
                      i32  i;
                      s32  initium_verbi;
                      b32  in_verbo;
                character  c_currens;
                character  c_praecedans;

    fructus.elementa = NIHIL;
    fructus.numerus  = ZEPHYRUM;

    si (!s.datum || s.mensura == ZEPHYRUM || !piscina)
    {
        redde fructus;
    }

    capacitas = XVI;
    verba = (chorda*)piscina_allocare(piscina, (memoriae_index)capacitas * magnitudo(chorda));
    si (!verba)
    {
        redde fructus;
    }

    numerus       = ZEPHYRUM;
    initium_verbi = -I;
    in_verbo      = FALSUM;
    c_praecedans  = '\0';

    per (i = ZEPHYRUM; i <= s.mensura; i++)
    {
        b32 est_finis = (i == s.mensura);
        b32 est_alpha = FALSUM;
        b32 est_digitus = FALSUM;
        b32 est_maiuscula = FALSUM;
        b32 praec_minuscula = FALSUM;
        b32 praec_digitus = FALSUM;
        b32 praec_maiuscula = FALSUM;
        b32 debet_scindere = FALSUM;

        si (!est_finis)
        {
            c_currens = (character)s.datum[i];
            est_alpha = isalpha((integer)c_currens) != ZEPHYRUM;
            est_digitus = isdigit((integer)c_currens) != ZEPHYRUM;
            est_maiuscula = isupper((integer)c_currens) != ZEPHYRUM;
        }

        si (c_praecedans != '\0')
        {
            praec_minuscula = islower((integer)c_praecedans) != ZEPHYRUM;
            praec_digitus = isdigit((integer)c_praecedans) != ZEPHYRUM;
            praec_maiuscula = isupper((integer)c_praecedans) != ZEPHYRUM;
        }

        /* Determinare si debemus scindere */
        si (in_verbo && !est_finis && (est_alpha || est_digitus))
        {
            /* Minuscula ad maiusculam: scinde */
            si (praec_minuscula && est_maiuscula)
            {
                debet_scindere = VERUM;
            }
            /* Digitus ad litteram: scinde */
            alioquin si (praec_digitus && est_alpha)
            {
                debet_scindere = VERUM;
            }
            /* Maiuscula ante maiusculam, sed proximus est minuscula: scinde ante currens */
            /* Exemplum: "XMLParser" - quando ad 'P' venimus post 'L', scinde */
            alioquin si (praec_maiuscula && est_maiuscula && i + I < s.mensura)
            {
                character c_proximus = (character)s.datum[i + I];
                si (islower((integer)c_proximus))
                {
                    debet_scindere = VERUM;
                }
            }
        }

        /* Finis verbi */
        si (in_verbo && (est_finis || (!est_alpha && !est_digitus) || debet_scindere))
        {
            /* Verificare capacitatem */
            si (numerus >= capacitas)
            {
                chorda* verba_nova;
                    i32 j;

                capacitas *= II;
                verba_nova = (chorda*)piscina_allocare(piscina, (memoriae_index)capacitas * magnitudo(chorda));
                si (!verba_nova)
                {
                    fructus.elementa = NIHIL;
                    fructus.numerus  = ZEPHYRUM;
                    redde fructus;
                }

                per (j = ZEPHYRUM; j < numerus; j++)
                {
                    verba_nova[j] = verba[j];
                }
                verba = verba_nova;
            }

            verba[numerus] = chorda_sectio(s, (i32)initium_verbi, i);
            numerus++;

            si (debet_scindere)
            {
                /* Initium novi verbi est currens character */
                initium_verbi = (s32)i;
                in_verbo = VERUM;
            }
            alioquin
            {
                in_verbo = FALSUM;
                initium_verbi = -I;
            }
        }

        /* Initium verbi novi */
        si (!in_verbo && !est_finis && (est_alpha || est_digitus))
        {
            initium_verbi = (s32)i;
            in_verbo = VERUM;
        }

        si (!est_finis)
        {
            c_praecedans = c_currens;
        }
    }

    fructus.elementa = verba;
    fructus.numerus  = numerus;
    redde fructus;
}


chorda
chorda_pascalis (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                      i8*  allocatus;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    /* Allocare maximum possibile (originalis mensura) */
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (j == ZEPHYRUM)
            {
                /* Prima littera maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura = positus;
    fructus.datum   = allocatus;
    redde fructus;
}


chorda
chorda_camelus (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                      i8*  allocatus;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)s.mensura);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (i == ZEPHYRUM)
            {
                /* Primum verbum totum minusculum */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
            alioquin si (j == ZEPHYRUM)
            {
                /* Prima littera verborum subsequentium maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura = positus;
    fructus.datum   = allocatus;
    redde fructus;
}


chorda
chorda_serpens (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                      i8*  allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '_' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '_';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            allocatus[positus++] = (i8)tolower((integer)(character)verbum.datum[j]);
        }
    }

    fructus.mensura = positus;
    fructus.datum   = allocatus;
    redde fructus;
}


chorda
chorda_kebab (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                      i8*  allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '-' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '-';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            allocatus[positus++] = (i8)tolower((integer)(character)verbum.datum[j]);
        }
    }

    fructus.mensura = positus;
    fructus.datum   = allocatus;
    redde fructus;
}


chorda
chorda_pascalis_serpens (
     chorda  s,
    Piscina* piscina)
{
    chorda_fissio_fructus  verba;
                   chorda  fructus;
                      i8*  allocatus;
                      i32  mensura_nova;
                      i32  positus;
                      i32  i;
                      i32  j;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    verba = _extrahere_verba(s, piscina);

    si (verba.numerus == ZEPHYRUM)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    /* Calculare mensuram: verba + separatores */
    mensura_nova = ZEPHYRUM;
    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        mensura_nova += verba.elementa[i].mensura;
        si (i > ZEPHYRUM)
        {
            mensura_nova += I; /* Pro '_' */
        }
    }

    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura_nova);
    si (!allocatus)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    positus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < verba.numerus; i++)
    {
        chorda verbum = verba.elementa[i];

        si (i > ZEPHYRUM)
        {
            allocatus[positus++] = '_';
        }

        per (j = ZEPHYRUM; j < verbum.mensura; j++)
        {
            character c = (character)verbum.datum[j];

            si (j == ZEPHYRUM)
            {
                /* Prima littera maiuscula */
                allocatus[positus++] = (i8)toupper((integer)c);
            }
            alioquin
            {
                /* Reliquae minusculae */
                allocatus[positus++] = (i8)tolower((integer)c);
            }
        }
    }

    fructus.mensura = positus;
    fructus.datum   = allocatus;
    redde fructus;
}


/* ==================================================
 * Novae Functiones Utilitatis
 * ================================================== */

b32
chorda_ut_f64 (
    chorda  s,
       f64* fructus)
{
    character* cstr_temporalis;
    character* terminus;
          f64  valor;

    si (!s.datum || !fructus || s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }

    cstr_temporalis = (character*)memoriae_allocare((memoriae_index)s.mensura + I);
    si (!cstr_temporalis)
    {
        redde FALSUM;
    }

    memcpy(cstr_temporalis, s.datum, (memoriae_index)s.mensura);
    cstr_temporalis[s.mensura] = '\0';

    valor = strtod(cstr_temporalis, &terminus);

    si (terminus != cstr_temporalis && *terminus == '\0')
    {
        *fructus = valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }

    liberare(cstr_temporalis);
    redde FALSUM;
}


chorda
chorda_ex_s32 (
       s32  numerus,
    Piscina* piscina)
{
         chorda  fructus;
      character  buffer[CXXXII];
            s32  mensura_signed;
            i32  mensura;
             i8* allocatus;

    fructus.mensura = ZEPHYRUM;
    fructus.datum   = NIHIL;

    si (!piscina)
    {
        redde fructus;
    }

    mensura_signed = snprintf(buffer, magnitudo(buffer), "%d", numerus);
    si (mensura_signed < ZEPHYRUM)
    {
        redde fructus;
    }

    mensura = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }

    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.mensura = mensura;
    fructus.datum   = allocatus;

    redde fructus;
}


chorda
chorda_ex_f64 (
       f64  numerus,
       i32  praecisio,
    Piscina* piscina)
{
         chorda  fructus;
      character  buffer[CXXXII];
      character  formatalis[XVI];
            s32  mensura_signed;
            i32  mensura;
             i8* allocatus;

    fructus.mensura = ZEPHYRUM;
    fructus.datum   = NIHIL;

    si (!piscina || praecisio < ZEPHYRUM || praecisio > L)
    {
        redde fructus;
    }

    snprintf(formatalis, magnitudo(formatalis), "%%.%df", praecisio);
    mensura_signed = snprintf(buffer, magnitudo(buffer), formatalis, numerus);
    si (mensura_signed < ZEPHYRUM)
    {
        redde fructus;
    }

    mensura = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }

    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.mensura = mensura;
    fructus.datum   = allocatus;

    redde fructus;
}


chorda
chorda_character_ad (
    chorda   s,
       i32   index,
    Piscina* piscina)
{
    chorda  fructus;
        i8* allocatus;

    fructus.mensura = ZEPHYRUM;
    fructus.datum   = NIHIL;

    si (!piscina || !s.datum || index < ZEPHYRUM || index >= s.mensura)
    {
        redde fructus;
    }

    allocatus = (i8*)piscina_allocare(piscina, I);
    si (!allocatus)
    {
        redde fructus;
    }

    allocatus[ZEPHYRUM] = s.datum[index];
    fructus.mensura = I;
    fructus.datum   = allocatus;

    redde fructus;
}


b32
chorda_vacua (
    chorda s)
{
    redde s.datum == NIHIL || s.mensura == ZEPHYRUM;
}


chorda_fissio_fructus
chorda_fissio_chorda (
    chorda   s,
    chorda   delim,
    Piscina* piscina)
{
    chorda_fissio_fructus  fructus;
                   chorda* elementa;
                      i32  capacitas;
                      i32  numerus;
                      i32  initium;
                      i32  i;

    fructus.elementa = NIHIL;
    fructus.numerus  = ZEPHYRUM;

    si (!piscina || !s.datum || s.mensura == ZEPHYRUM)
    {
        redde fructus;
    }

    /* Si delimitator vacuus, redde totam chordam ut unum elementum */
    si (!delim.datum || delim.mensura == ZEPHYRUM)
    {
        elementa = (chorda*)piscina_allocare(piscina, magnitudo(chorda));
        si (!elementa)
        {
            redde fructus;
        }
        elementa[ZEPHYRUM] = s;
        fructus.elementa = elementa;
        fructus.numerus  = I;
        redde fructus;
    }

    capacitas = XVI;
    elementa = (chorda*)piscina_allocare(piscina, (memoriae_index)capacitas * magnitudo(chorda));
    si (!elementa)
    {
        redde fructus;
    }

    numerus = ZEPHYRUM;
    initium = ZEPHYRUM;
    i = ZEPHYRUM;

    dum (i <= s.mensura - delim.mensura)
    {
        /* Verificare si hic est delimitator */
        si (memcmp(s.datum + i, delim.datum, (memoriae_index)delim.mensura) == ZEPHYRUM)
        {
            /* Verificare capacitatem */
            si (numerus >= capacitas)
            {
                chorda* elementa_nova;
                    i32 j;

                capacitas *= II;
                elementa_nova = (chorda*)piscina_allocare(piscina, (memoriae_index)capacitas * magnitudo(chorda));
                si (!elementa_nova)
                {
                    fructus.elementa = NIHIL;
                    fructus.numerus  = ZEPHYRUM;
                    redde fructus;
                }

                per (j = ZEPHYRUM; j < numerus; j++)
                {
                    elementa_nova[j] = elementa[j];
                }
                elementa = elementa_nova;
            }

            elementa[numerus] = chorda_sectio(s, initium, i);
            numerus++;
            initium = i + delim.mensura;
            i = initium;
        }
        alioquin
        {
            i++;
        }
    }

    /* Addere ultimum segmentum */
    si (numerus >= capacitas)
    {
        chorda* elementa_nova;
            i32 j;

        capacitas *= II;
        elementa_nova = (chorda*)piscina_allocare(piscina, (memoriae_index)capacitas * magnitudo(chorda));
        si (!elementa_nova)
        {
            fructus.elementa = NIHIL;
            fructus.numerus  = ZEPHYRUM;
            redde fructus;
        }

        per (j = ZEPHYRUM; j < numerus; j++)
        {
            elementa_nova[j] = elementa[j];
        }
        elementa = elementa_nova;
    }

    elementa[numerus] = chorda_sectio(s, initium, s.mensura);
    numerus++;

    fructus.elementa = elementa;
    fructus.numerus  = numerus;
    redde fructus;
}


chorda
chorda_iungere (
    chorda*  elementa,
        i32  numerus,
     chorda  separator,
    Piscina* piscina)
{
    ChordaAedificator* aed;
                   i32 i;
                chorda fructus;

    fructus.datum   = NIHIL;
    fructus.mensura = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }

    si (numerus == ZEPHYRUM || elementa == NIHIL)
    {
        redde fructus;
    }

    aed = chorda_aedificator_creare(piscina, CXXVIII);
    si (aed == NIHIL)
    {
        redde fructus;
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (i > ZEPHYRUM && separator.datum != NIHIL && separator.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(aed, separator);
        }
        si (elementa[i].datum != NIHIL)
        {
            chorda_aedificator_appendere_chorda(aed, elementa[i]);
        }
    }

    redde chorda_aedificator_finire(aed);
}
