#include "via.h"
#include "chorda_aedificator.h"
#include <string.h>
#include <unistd.h>

/* ===========================================================
 * CONSTANTAE
 * =========================================================== */

#define VIA_SEPARATOR           '/'
#define VIA_SEPARATOR_CHORDA    "/"
#define VIA_DIRECTORIUM_CURRENS "."
#define VIA_DIRECTORIUM_PARENS  ".."


/* ===========================================================
 * ADIUTORES INTERNI
 * =========================================================== */

/* Verificare si character separator est */
interior b32
_est_separator (
	character c)
{
	redde c == VIA_SEPARATOR;
}

/* Invenire ultimum separatorem in chorda */
interior s32
_invenire_ultimum_separatorem (
	chorda s)
{
	s32 i;

	per (i = (s32)(s.mensura - I); i >= ZEPHYRUM; i--)
	{
		si (_est_separator((character)s.datum[i]))
		{
			redde i;
		}
	}

	redde -I;
}

/* Invenire ultimum punctum in chorda (pro extensione) */
interior s32
_invenire_ultimum_punctum (
	chorda s)
{
	s32 i;

	per (i = (s32)(s.mensura - I); i >= ZEPHYRUM; i--)
	{
		si (s.datum[i] == '.')
		{
			redde i;
		}
	}

	redde -I;
}


/* ===========================================================
 * IUNCTIO
 * =========================================================== */

chorda
via_iungere (
	constans chorda* partes,
	            i32  numerus,
	        Piscina* piscina)
{
	ChordaAedificator* aedificator;
	              i32  i;
	           chorda  pars;
	           chorda  fructus;

	si (!partes || numerus == ZEPHYRUM || !piscina)
	{
		chorda vacua;
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	aedificator = chorda_aedificator_creare(piscina, CCLVI);
	si (!aedificator)
	{
		chorda vacua;
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	per (i = ZEPHYRUM; i < numerus; i++)
	{
		pars = partes[i];

		/* Omittere partes vacuas */
		si (pars.mensura == ZEPHYRUM) perge;

		/* Si non prima pars et aedificator non vacuum et
		 * aedificator non terminat cum separator et
		 * pars non incipit cum separator,
		 * addere separator */
		si (i > ZEPHYRUM && chorda_aedificator_longitudo(aedificator) > ZEPHYRUM)
		{
			chorda spectatio  = chorda_aedificator_spectare(aedificator);
			character ultimus = (character)spectatio.datum[spectatio.mensura - I];

			si (!_est_separator(ultimus) && 
                !_est_separator((character)pars.datum[ZEPHYRUM]))
			{
				chorda_aedificator_appendere_character(aedificator, VIA_SEPARATOR);
			}
		}

		chorda_aedificator_appendere_chorda(aedificator, pars);
	}

	fructus = chorda_aedificator_finire(aedificator);
	redde fructus;
}


/* ===========================================================
 * NORMALIZATIO
 * =========================================================== */

chorda
via_normalizare (
     chorda  via,
    Piscina* piscina)
{
	 chorda_fissio_fructus  fissio;
	                   b32  est_absoluta;
	                   i32  i;
	                chorda  pars;
	                chorda* stack;
	                   i32  stack_numerus;
	                chorda  fructus;

	si (!piscina || via.mensura == ZEPHYRUM)
	{
		redde via;
	}

	est_absoluta = via_est_absoluta(via);

	/* Findere per separatorem */
	fissio = chorda_fissio(via, VIA_SEPARATOR, piscina);

	/* Allocare stack pro partes (maximus magnitudo = numerus partium) */
	stack = (chorda*)piscina_allocare(piscina, (memoriae_index)fissio.numerus * magnitudo(chorda));
	si (!stack)
	{
		chorda vacua;
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	stack_numerus = ZEPHYRUM;

	per (i = ZEPHYRUM; i < fissio.numerus; i++)
	{
		pars = fissio.elementa[i];

		/* Omittere partes vacuas et "." */
		si (pars.mensura == ZEPHYRUM) perge;
		si (chorda_aequalis(pars, chorda_ex_literis(".", piscina))) perge;

		/* Tractare ".." */
		si (chorda_aequalis(pars, chorda_ex_literis("..", piscina)))
		{
			si (stack_numerus > ZEPHYRUM)
			{
				/* Pop */
				stack_numerus--;
			}
			alioquin si (!est_absoluta)
			{
				/* Via relativa, servare ".." */
				stack[stack_numerus] = pars;
				stack_numerus++;
			}
			/* Si absoluta et stack vacuum, omittere (non possumus ire supra radicem) */
		}
		alioquin
		{
			/* Pars normalis - push */
			stack[stack_numerus] = pars;
			stack_numerus++;
		}
	}

	/* Si nullae partes, redde appropriatum */
	si (stack_numerus == ZEPHYRUM)
	{
		si (est_absoluta)
		{
			redde chorda_ex_literis(VIA_SEPARATOR_CHORDA, piscina);
		}
		alioquin
		{
			redde chorda_ex_literis(VIA_DIRECTORIUM_CURRENS, piscina);
		}
	}

	/* Iungere partes */
	fructus = via_iungere(stack, stack_numerus, piscina);

	/* Si absoluta, assecurare incipit cum "/" */
	si (est_absoluta && (fructus.mensura == ZEPHYRUM || 
        !_est_separator((character)fructus.datum[ZEPHYRUM])))
	{
		chorda partes_cum_radice[II];
		partes_cum_radice[ZEPHYRUM] = chorda_ex_literis(VIA_SEPARATOR_CHORDA, piscina);
		partes_cum_radice[I]        = fructus;
		fructus                     = via_iungere(partes_cum_radice, II, piscina);
	}

	redde fructus;
}

chorda
via_absoluta (
     chorda  via,
    Piscina* piscina)
{
	chorda cwd;
	chorda partes[II];
	chorda iuncta;

	si (!piscina)
	{
		chorda vacua;
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	/* Si iam absoluta, normalizare solum */
	si (via_est_absoluta(via))
	{
		redde via_normalizare(via, piscina);
	}

	/* Obtinere CWD */
	cwd = via_directorium_currens(piscina);
	si (cwd.mensura == ZEPHYRUM)
	{
		/* Errore obtinendo CWD */
		redde via;
	}

	/* Iungere CWD cum via */
	partes[ZEPHYRUM] = cwd;
	partes[I]        = via;
	iuncta           = via_iungere(partes, II, piscina);

	/* Normalizare */
	redde via_normalizare(iuncta, piscina);
}


/* ===========================================================
 * EXTRACTIO
 * =========================================================== */

chorda
via_directorium (
     chorda  via,
    Piscina* piscina)
{
	s32 positus;

	si (!piscina || via.mensura == ZEPHYRUM)
	{
		redde chorda_ex_literis(VIA_DIRECTORIUM_CURRENS, piscina);
	}

	/* Invenire ultimum separatorem */
	positus = _invenire_ultimum_separatorem(via);

	/* Si nullus separator, redde "." */
	si (positus < ZEPHYRUM)
	{
		redde chorda_ex_literis(VIA_DIRECTORIUM_CURRENS, piscina);
	}

	/* Si separator ad initium (radix), redde "/" */
	si (positus == ZEPHYRUM)
	{
		redde chorda_ex_literis(VIA_SEPARATOR_CHORDA, piscina);
	}

	/* Redde partem ante separatorem */
	redde chorda_sectio(via, ZEPHYRUM, (i32)positus);
}

chorda
via_nomen (
     chorda  via,
    Piscina* piscina)
{
	   s32 positus;
	chorda vacua;

	(vacuum)piscina; /* Non usatus in hac versione */

	si (via.mensura == ZEPHYRUM)
	{
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	/* Invenire ultimum separatorem */
	positus = _invenire_ultimum_separatorem(via);

	/* Si nullus separator, tota via est nomen */
	si (positus < ZEPHYRUM)
	{
		redde via;
	}

	/* Redde partem post separatorem */
	redde chorda_sectio(via, (i32)(positus + I), via.mensura);
}

chorda
via_nomen_radix (
     chorda  via,
    Piscina* piscina)
{
	chorda titulus;
	   s32 positus_puncti;

	/* Obtinere nomen fili */
	titulus = via_nomen(via, piscina);

	si (titulus.mensura == ZEPHYRUM)
	{
		redde titulus;
	}

	/* Si incipit cum puncto (dotfile), redde totum */
	si ((character)titulus.datum[ZEPHYRUM] == '.')
	{
		redde titulus;
	}

	/* Invenire ultimum punctum */
	positus_puncti = _invenire_ultimum_punctum(titulus);

	/* Si nullus punctum, redde totum nomen */
	si (positus_puncti < ZEPHYRUM)
	{
		redde titulus;
	}

	/* Redde partem ante punctum */
	redde chorda_sectio(titulus, ZEPHYRUM, (i32)positus_puncti);
}

chorda
via_extensio (
      chorda  via,
    Piscina* piscina)
{
	chorda titulus;
	   s32 positus_puncti;
	chorda vacua;

	/* Obtinere nomen fili */
	titulus = via_nomen(via, piscina);

	si (titulus.mensura == ZEPHYRUM)
	{
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	/* Si incipit cum puncto (dotfile), nullus extensio */
	si ((character)titulus.datum[ZEPHYRUM] == '.')
	{
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	/* Invenire ultimum punctum */
	positus_puncti = _invenire_ultimum_punctum(titulus);

	/* Si nullus punctum, nullus extensio */
	si (positus_puncti < ZEPHYRUM)
	{
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	/* Redde partem a puncto ad finem (includens punctum) */
	redde chorda_sectio(titulus, (i32)positus_puncti, titulus.mensura);
}


/* ===========================================================
 * INTERROGATIO
 * =========================================================== */

b32
via_est_absoluta (
	chorda via)
{
	si (via.mensura == ZEPHYRUM) redde FALSUM;
	redde _est_separator((character)via.datum[ZEPHYRUM]);
}

b32
via_est_relativa (
	chorda via)
{
	redde !via_est_absoluta(via);
}


/* ===========================================================
 * UTILITAS
 * =========================================================== */

chorda
via_directorium_currens (
	Piscina* piscina)
{
	character  buffer[MMMMXCVI]; /* 4096 bytes pro via */
	character* fructus;

	si (!piscina)
	{
		chorda vacua;
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	fructus = getcwd(buffer, MMMMXCVI);
	si (!fructus)
	{
		/* Errore */
		chorda vacua;
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	redde chorda_ex_literis(buffer, piscina);
}
