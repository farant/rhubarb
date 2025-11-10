#include "argumenta.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===========================================================
 * CONSTANTAE
 * =========================================================== */

#define ARGUMENTA_MAXIMUS_DEFINITIONES CXXVIII  /* 128 max definitions */
#define ARGUMENTA_MAXIMUS_EXEMPLA      XVI      /* 16 max examples */
#define ARGUMENTA_MAXIMUS_VALORES      CXXVIII  /* 128 max values */


/* ===========================================================
 * TYPI INTERNI
 * =========================================================== */

nomen enumeratio {
	ARGUMENTA_GENUS_VEXILLUM,
	ARGUMENTA_GENUS_OPTIO,
	ARGUMENTA_GENUS_POSITIONALIS
} ArgumentaGenus;

/* Definitio singularis (vexillum, optio, vel positionalis) */
nomen structura {
  ArgumentaGenus  genus;
	   character* nomen_brevis;
	   character* nomen_longus;
	   character* titulus;        /* Pro positionalia */
	   character* descriptio;
	         b32  necessarius;    /* Pro positionalia */
} ArgumentaDefinitio;

/* Parser cum omnibus definitionibus */
structura ArgumentaParser {
	           Piscina* piscina;
	ArgumentaDefinitio  definitiones[ARGUMENTA_MAXIMUS_DEFINITIONES];
	               i32  numerus_definitionum;
	         character* descriptio_programmi;
	         character* exempla[ARGUMENTA_MAXIMUS_EXEMPLA];
	               i32  numerus_exemplorum;
	         character* nuntius_erroris;
};

/* Valor datus (vexillum vel optio) */
nomen structura {
	character* titulus;
	character* valor;  /* NIHIL pro vexilla */
} ArgumentaValor;

/* Fructus parsitionis */
structura ArgumentaFructus {
           Piscina* piscina;
    ArgumentaValor  valores[ARGUMENTA_MAXIMUS_VALORES];
               i32  numerus_valorum;
         character* positionalia[ARGUMENTA_MAXIMUS_VALORES];
               i32  numerus_positionalium;
};


/* ===========================================================
 * ADIUTORES INTERNI - CHORDAE
 * =========================================================== */

/* Duplicare chordam C ad piscinam */
interior character*
_duplicare_cstr (
	constans character* str,
	           Piscina* piscina)
{
	memoriae_index  mensura;
	     character* novum;

	si (!str) redde NIHIL;

	mensura = strlen(str);
	novum   = (character*)piscina_allocare(piscina, mensura + I);
	si (!novum) redde NIHIL;

	strcpy(novum, str);
	redde novum;
}

/* Verificare si chorda incipit cum praefixo */
interior b32
_incipit_cum (
	constans character* str,
	constans character* praefixum)
{
	memoriae_index mensura_praefixum;

	si (!str || !praefixum) redde FALSUM;

	mensura_praefixum = strlen(praefixum);
	redde strncmp(str, praefixum, mensura_praefixum) == ZEPHYRUM;
}

/* Verificare si duae chordae aequales sunt */
interior b32
_aequalis_cstr (
	constans character* a,
	constans character* b)
{
	si (a == NIHIL && b == NIHIL) redde VERUM;
	si (a == NIHIL || b == NIHIL) redde FALSUM;
	redde strcmp(a, b) == ZEPHYRUM;
}


/* ===========================================================
 * ADIUTORES INTERNI - QUAESTIO
 * =========================================================== */

/* Invenire definitionem per nomen (breve vel longum) */
interior ArgumentaDefinitio*
_invenire_definitionem (
	   ArgumentaParser* parser,
	constans character* titulus)
{
	i32 i;

	per (i = ZEPHYRUM; i < parser->numerus_definitionum; i++)
	{
		ArgumentaDefinitio* def = &parser->definitiones[i];

		si (_aequalis_cstr(def->nomen_brevis, titulus)) redde def;
		si (_aequalis_cstr(def->nomen_longus, titulus)) redde def;
	}

	redde NIHIL;
}

/* Invenire valorem in fructu per nomen */
interior constans ArgumentaValor*
_invenire_valorem (
	constans ArgumentaFructus* fructus,
	       constans character* titulus)
{
	i32 i;

	per (i = ZEPHYRUM; i < fructus->numerus_valorum; i++)
	{
		si (_aequalis_cstr(fructus->valores[i].titulus, titulus))
		{
			redde &fructus->valores[i];
		}
	}

	redde NIHIL;
}


/* ===========================================================
 * CREATIO
 * =========================================================== */

ArgumentaParser*
argumenta_creare (
	Piscina* piscina)
{
	ArgumentaParser* parser;

	si (!piscina) redde NIHIL;

	parser = (ArgumentaParser*)piscina_allocare(piscina, magnitudo(ArgumentaParser));
	si (!parser) redde NIHIL;

	memset(parser, ZEPHYRUM, magnitudo(ArgumentaParser));
	parser->piscina = piscina;

	redde parser;
}


/* ===========================================================
 * DEFINITIO - VEXILLA
 * =========================================================== */

vacuum
argumenta_addere_vexillum (
	   ArgumentaParser* parser,
	constans character* nomen_brevis,
	constans character* nomen_longus,
	constans character* descriptio)
{
	ArgumentaDefinitio* def;

	si (!parser) redde;
	si (!nomen_brevis && !nomen_longus) redde;

	si (parser->numerus_definitionum >= ARGUMENTA_MAXIMUS_DEFINITIONES)
	{
		fprintf(stderr, "FATALIS: Nimis multae definitiones (max: %d)\n",
		        ARGUMENTA_MAXIMUS_DEFINITIONES);
		fprintf(stderr, "Considera augere ARGUMENTA_MAXIMUS_DEFINITIONES vel usa xar\n");
		exit(I);
	}

	def = &parser->definitiones[parser->numerus_definitionum];
	parser->numerus_definitionum++;

	def->genus        = ARGUMENTA_GENUS_VEXILLUM;
	def->nomen_brevis = _duplicare_cstr(nomen_brevis, parser->piscina);
	def->nomen_longus = _duplicare_cstr(nomen_longus, parser->piscina);
	def->descriptio   = _duplicare_cstr(descriptio, parser->piscina);
	def->necessarius  = FALSUM;
}


/* ===========================================================
 * DEFINITIO - OPTIONES
 * =========================================================== */

vacuum
argumenta_addere_optionem (
	   ArgumentaParser* parser,
	constans character* nomen_brevis,
	constans character* nomen_longus,
	constans character* descriptio)
{
	ArgumentaDefinitio* def;

	si (!parser) redde;
	si (!nomen_brevis && !nomen_longus) redde;

	si (parser->numerus_definitionum >= ARGUMENTA_MAXIMUS_DEFINITIONES)
	{
		fprintf(stderr, "FATALIS: Nimis multae definitiones (max: %d)\n",
		        ARGUMENTA_MAXIMUS_DEFINITIONES);
		fprintf(stderr, "Considera augere ARGUMENTA_MAXIMUS_DEFINITIONES vel usa xar\n");
		exit(I);
	}

	def = &parser->definitiones[parser->numerus_definitionum];
	parser->numerus_definitionum++;

	def->genus        = ARGUMENTA_GENUS_OPTIO;
	def->nomen_brevis = _duplicare_cstr(nomen_brevis, parser->piscina);
	def->nomen_longus = _duplicare_cstr(nomen_longus, parser->piscina);
	def->descriptio   = _duplicare_cstr(descriptio, parser->piscina);
	def->necessarius  = FALSUM;
}


/* ===========================================================
 * DEFINITIO - POSITIONALIA
 * =========================================================== */

vacuum
argumenta_addere_positionalem (
	   ArgumentaParser* parser,
	constans character* titulus,
	constans character* descriptio,
	               b32  necessarius)
{
	ArgumentaDefinitio* def;

	si (!parser) redde;
	si (!titulus) redde;

	si (parser->numerus_definitionum >= ARGUMENTA_MAXIMUS_DEFINITIONES)
	{
		fprintf(stderr, "FATALIS: Nimis multae definitiones (max: %d)\n",
		        ARGUMENTA_MAXIMUS_DEFINITIONES);
		fprintf(stderr, "Considera augere ARGUMENTA_MAXIMUS_DEFINITIONES vel usa xar\n");
		exit(I);
	}

	def = &parser->definitiones[parser->numerus_definitionum];
	parser->numerus_definitionum++;

	def->genus       = ARGUMENTA_GENUS_POSITIONALIS;
	def->titulus     = _duplicare_cstr(titulus, parser->piscina);
	def->descriptio  = _duplicare_cstr(descriptio, parser->piscina);
	def->necessarius = necessarius;
}


/* ===========================================================
 * DEFINITIO - AUXILIUM MANUALE
 * =========================================================== */

vacuum
argumenta_ponere_descriptionem (
	   ArgumentaParser* parser,
	constans character* descriptio)
{
	si (!parser) redde;

	parser->descriptio_programmi = _duplicare_cstr(descriptio, parser->piscina);
}

vacuum
argumenta_addere_exemplum (
	   ArgumentaParser* parser,
	constans character* exemplum)
{
	si (!parser) redde;

	si (parser->numerus_exemplorum >= ARGUMENTA_MAXIMUS_EXEMPLA)
	{
		fprintf(stderr, "FATALIS: Nimis multa exempla (max: %d)\n",
		        ARGUMENTA_MAXIMUS_EXEMPLA);
		fprintf(stderr, "Considera augere ARGUMENTA_MAXIMUS_EXEMPLA\n");
		exit(I);
	}

	parser->exempla[parser->numerus_exemplorum] =
		_duplicare_cstr(exemplum, parser->piscina);
	parser->numerus_exemplorum++;
}


/* ===========================================================
 * PARSITIO - ADIUTORES
 * =========================================================== */

/* Addere valorem ad fructum */
interior vacuum
_addere_valorem (
	  ArgumentaFructus* fructus,
	constans character* titulus,
	constans character* valor)
{
	ArgumentaValor* v;

	si (fructus->numerus_valorum >= ARGUMENTA_MAXIMUS_VALORES)
	{
		fprintf(stderr, "FATALIS: Nimis multi valores (max: %d)\n",
		        ARGUMENTA_MAXIMUS_VALORES);
		fprintf(stderr, "Considera augere ARGUMENTA_MAXIMUS_VALORES vel usa xar\n");
		exit(I);
	}

	v = &fructus->valores[fructus->numerus_valorum];
	fructus->numerus_valorum++;

	v->titulus = _duplicare_cstr(titulus, fructus->piscina);
	v->valor   = _duplicare_cstr(valor, fructus->piscina);
}

/* Addere positionale ad fructum */
interior vacuum
_addere_positionale (
	  ArgumentaFructus* fructus,
	constans character* valor)
{
	si (fructus->numerus_positionalium >= ARGUMENTA_MAXIMUS_VALORES)
	{
		fprintf(stderr, "FATALIS: Nimis multa positionalia (max: %d)\n",
		        ARGUMENTA_MAXIMUS_VALORES);
		fprintf(stderr, "Considera augere ARGUMENTA_MAXIMUS_VALORES vel usa xar\n");
		exit(I);
	}

	fructus->positionalia[fructus->numerus_positionalium] =
		_duplicare_cstr(valor, fructus->piscina);
	fructus->numerus_positionalium++;
}

/* Parsere argumentum cum "--" (longum) */
interior b32
_parsere_longum (
	   ArgumentaParser* parser,
	  ArgumentaFructus* fructus,
	constans character* arg,
	constans character* arg_sequens,
	               i32* salti)
{
	         character  buffer[CCLVI];
	         character  nomen_completum[CCLVI];
	constans character* titulus;
	constans character* valor;
	         character* equals;
	ArgumentaDefinitio* def;

	/* Copiare ad buffer pro manipulatione */
	strncpy(buffer, arg + II, CCLVI - I);  /* Omittere "--" */
	buffer[CCLVI - I] = '\0';

	/* Quaerere '=' */
	equals = strchr(buffer, '=');
	si (equals)
	{
		/* --titulus=valor */
		*equals = '\0';
		titulus = buffer;
		valor   = equals + I;
	}
	alioquin
	{
		titulus = buffer;
		valor   = NIHIL;
	}

	/* Construere nomen completum cum "--" */
	snprintf(nomen_completum, CCLVI, "--%s", titulus);

	/* Invenire definitionem */
	def = _invenire_definitionem(parser, nomen_completum);
	si (!def)
	{
		snprintf(buffer, CCLVI, "Optio ignota: %s", nomen_completum);
		parser->nuntius_erroris = _duplicare_cstr(buffer, parser->piscina);
		redde FALSUM;
	}

	/* Si vexillum, addere sine valore */
	si (def->genus == ARGUMENTA_GENUS_VEXILLUM)
	{
		si (valor)
		{
			snprintf(buffer, CCLVI, "Vexillum %s non accipit valorem", nomen_completum);
			parser->nuntius_erroris = _duplicare_cstr(buffer, parser->piscina);
			redde FALSUM;
		}
		/* Addere sub forma brevis */
		si (def->nomen_brevis)
		{
			_addere_valorem(fructus, def->nomen_brevis, NIHIL);
		}
		/* Addere sub forma longus etiam */
		si (def->nomen_longus)
		{
			_addere_valorem(fructus, def->nomen_longus, NIHIL);
		}
		redde VERUM;
	}

	/* Si optio, accipere valorem */
	si (!valor)
	{
		/* Valor debet esse argumentum sequens */
		si (!arg_sequens)
		{
			snprintf(buffer, CCLVI, "Optio %s requirit valorem", nomen_completum);
			parser->nuntius_erroris = _duplicare_cstr(buffer, parser->piscina);
			redde FALSUM;
		}
		valor  = arg_sequens;
		*salti = I;
	}

	/* Addere sub forma brevis */
	si (def->nomen_brevis)
	{
		_addere_valorem(fructus, def->nomen_brevis, valor);
	}
	/* Addere sub forma longus etiam */
	si (def->nomen_longus)
	{
		_addere_valorem(fructus, def->nomen_longus, valor);
	}
	redde VERUM;
}

/* Parsere argumentum cum "-" (breve) */
interior b32
_parsere_breve (
	   ArgumentaParser* parser,
	  ArgumentaFructus* fructus,
	constans character* arg,
	constans character* arg_sequens,
	               i32* salti)
{
	         character  buffer[CCLVI];
	         character  nomen_completum[III];
	constans character* valor;
	ArgumentaDefinitio* def;

	/* Nomen est duo charactera: "-X" */
	si (strlen(arg) < II || arg[I] == '\0')
	{
		snprintf(buffer, CCLVI, "Vexillum breve invalidum: %s", arg);
		parser->nuntius_erroris = _duplicare_cstr(buffer, parser->piscina);
		redde FALSUM;
	}

	/* Construere nomen "-X" */
	nomen_completum[ZEPHYRUM] = '-';
	nomen_completum[I]        = arg[I];
	nomen_completum[II]       = '\0';

	/* Invenire definitionem */
	def = _invenire_definitionem(parser, nomen_completum);
	si (!def)
	{
		snprintf(buffer, CCLVI, "Optio ignota: %s", nomen_completum);
		parser->nuntius_erroris = _duplicare_cstr(buffer, parser->piscina);
		redde FALSUM;
	}

	/* Si vexillum, addere sine valore */
	si (def->genus == ARGUMENTA_GENUS_VEXILLUM)
	{
		/* Addere sub forma brevis */
		si (def->nomen_brevis)
		{
			_addere_valorem(fructus, def->nomen_brevis, NIHIL);
		}
		/* Addere sub forma longus etiam */
		si (def->nomen_longus)
		{
			_addere_valorem(fructus, def->nomen_longus, NIHIL);
		}
		redde VERUM;
	}

	/* Si optio, accipere valorem ex sequente */
	si (!arg_sequens)
	{
		snprintf(buffer, CCLVI, "Optio %s requirit valorem", nomen_completum);
		parser->nuntius_erroris = _duplicare_cstr(buffer, parser->piscina);
		redde FALSUM;
	}

	valor  = arg_sequens;
	*salti = I;

	/* Addere sub forma brevis */
	si (def->nomen_brevis)
	{
		_addere_valorem(fructus, def->nomen_brevis, valor);
	}
	/* Addere sub forma longus etiam */
	si (def->nomen_longus)
	{
		_addere_valorem(fructus, def->nomen_longus, valor);
	}
	redde VERUM;
}


/* ===========================================================
 * PARSITIO
 * =========================================================== */

ArgumentaFructus*
argumenta_conari_parsere (
	             ArgumentaParser* parser,
	                         i32  argc,
	constans character* constans* argv)
{
	ArgumentaFructus* fructus;
	             i32  i;
	             i32  numerus_positionalium_necessariorum;

	si (!parser || !argv) redde NIHIL;

	/* Creare fructum */
	fructus = (ArgumentaFructus*)piscina_allocare(
		parser->piscina,
		magnitudo(ArgumentaFructus));
	si (!fructus) redde NIHIL;

	memset(fructus, ZEPHYRUM, magnitudo(ArgumentaFructus));
	fructus->piscina = parser->piscina;

	/* Parsere argumenta (omittere argv[0] = nomen programmi) */
	per (i = I; i < argc; i++)
	{
		constans character* arg = argv[i];
		constans character* arg_sequens = (i + I < argc) ? argv[i + I] : NIHIL;
		               i32  salti = ZEPHYRUM;

		si (_incipit_cum(arg, "--"))
		{
			/* Argumentum longum */
			si (!_parsere_longum(parser, fructus, arg, arg_sequens, &salti))
			{
				redde NIHIL;
			}
			i += salti;
		}
		alioquin si (_incipit_cum(arg, "-") && strlen(arg) > I)
		{
			/* Argumentum breve */
			si (!_parsere_breve(parser, fructus, arg, arg_sequens, &salti))
			{
				redde NIHIL;
			}
			i += salti;
		}
		alioquin
		{
			/* Argumentum positionale */
			_addere_positionale(fructus, arg);
		}
	}

	/* Verificare positionalia necessaria */
	numerus_positionalium_necessariorum = ZEPHYRUM;
	per (i = ZEPHYRUM; i < parser->numerus_definitionum; i++)
	{
		si (parser->definitiones[i].genus == ARGUMENTA_GENUS_POSITIONALIS &&
		    parser->definitiones[i].necessarius)
		{
			numerus_positionalium_necessariorum++;
		}
	}

	si (fructus->numerus_positionalium < numerus_positionalium_necessariorum)
	{
		character buffer[CCLVI];
		snprintf(buffer, CCLVI, "Argumenta positionalia necessaria desunt");
		parser->nuntius_erroris = _duplicare_cstr(buffer, parser->piscina);
		redde NIHIL;
	}

	redde fructus;
}

ArgumentaFructus*
argumenta_parsere (
	             ArgumentaParser* parser,
	                         i32  argc,
	constans character* constans* argv)
{
	ArgumentaFructus* fructus;

	fructus = argumenta_conari_parsere(parser, argc, argv);
	si (!fructus)
	{
		argumenta_imprimere_errorem(parser);
		exire(I);
	}

	redde fructus;
}


/* ===========================================================
 * INTERROGATIO - VEXILLA
 * =========================================================== */

b32
argumenta_habet_vexillum (
	constans ArgumentaFructus* fructus,
	       constans character* titulus)
{
	constans ArgumentaValor* v;

	si (!fructus || !titulus) redde FALSUM;

	v = _invenire_valorem(fructus, titulus);
	redde v != NIHIL;
}


/* ===========================================================
 * INTERROGATIO - OPTIONES
 * =========================================================== */

chorda
argumenta_obtinere_optionem (
	constans ArgumentaFructus* fructus,
	       constans character* titulus,
	                  Piscina* piscina)
{
	constans ArgumentaValor* v;
	                 chorda  vacua;

	si (!fructus || !titulus || !piscina)
	{
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	v = _invenire_valorem(fructus, titulus);
	si (!v || !v->valor)
	{
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	redde chorda_ex_literis(v->valor, piscina);
}


/* ===========================================================
 * INTERROGATIO - POSITIONALIA
 * =========================================================== */

chorda
argumenta_obtinere_positionalem (
	constans ArgumentaFructus* fructus,
	                      i32  index,
	                  Piscina* piscina)
{
	chorda vacua;

	si (!fructus || !piscina ||
	    index < ZEPHYRUM ||
	    index >= fructus->numerus_positionalium)
	{
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;
		redde vacua;
	}

	redde chorda_ex_literis(fructus->positionalia[index], piscina);
}

i32
argumenta_numerus_positionalium (
	constans ArgumentaFructus* fructus)
{
	si (!fructus) redde ZEPHYRUM;
	redde fructus->numerus_positionalium;
}


/* ===========================================================
 * AUXILIUM ET ERRORES
 * =========================================================== */

vacuum
argumenta_imprimere_auxilium (
	constans ArgumentaParser* parser)
{
	i32 i;
	i32 numerus_positionalium;

	si (!parser) redde;

	/* Descriptio programmi */
	si (parser->descriptio_programmi)
	{
		imprimere("%s\n\n", parser->descriptio_programmi);
	}

	/* Usus */
	imprimere("USUS:\n    ");

	/* Vexilla et optiones */
	si (parser->numerus_definitionum > ZEPHYRUM)
	{
		imprimere("[OPTIONES] ");
	}

	/* Positionalia */
	numerus_positionalium = ZEPHYRUM;
	per (i = ZEPHYRUM; i < parser->numerus_definitionum; i++)
	{
		si (parser->definitiones[i].genus == ARGUMENTA_GENUS_POSITIONALIS)
		{
			si (parser->definitiones[i].necessarius)
			{
				imprimere("<%s> ", parser->definitiones[i].titulus);
			}
			alioquin
			{
				imprimere("[%s] ", parser->definitiones[i].titulus);
			}
			numerus_positionalium++;
		}
	}
	imprimere("\n\n");

	/* Optiones et vexilla */
	imprimere("OPTIONES:\n");
	per (i = ZEPHYRUM; i < parser->numerus_definitionum; i++)
	{
		constans ArgumentaDefinitio* def = &parser->definitiones[i];

		si (def->genus == ARGUMENTA_GENUS_POSITIONALIS) perge;

		imprimere("    ");

		/* Nomina */
		si (def->nomen_brevis && def->nomen_longus)
		{
			imprimere("%s, %s", def->nomen_brevis, def->nomen_longus);
		}
		alioquin si (def->nomen_brevis)
		{
			imprimere("%s", def->nomen_brevis);
		}
		alioquin
		{
			imprimere("%s", def->nomen_longus);
		}

		/* Valor placeholder pro optiones */
		si (def->genus == ARGUMENTA_GENUS_OPTIO)
		{
			imprimere(" <valor>");
		}

		imprimere("\n");

		/* Descriptio */
		si (def->descriptio)
		{
			imprimere("        %s\n", def->descriptio);
		}

		imprimere("\n");
	}

	/* Argumenta positionalia */
	si (numerus_positionalium > ZEPHYRUM)
	{
		imprimere("ARGUMENTA:\n");
		per (i = ZEPHYRUM; i < parser->numerus_definitionum; i++)
		{
			constans ArgumentaDefinitio* def = &parser->definitiones[i];

			si (def->genus != ARGUMENTA_GENUS_POSITIONALIS) perge;

			imprimere("    <%s>", def->titulus);
			si (!def->necessarius)
			{
				imprimere(" (optionalis)");
			}
			imprimere("\n");

			si (def->descriptio)
			{
				imprimere("        %s\n", def->descriptio);
			}
			imprimere("\n");
		}
	}

	/* Exempla */
	si (parser->numerus_exemplorum > ZEPHYRUM)
	{
		imprimere("EXEMPLA:\n");
		per (i = ZEPHYRUM; i < parser->numerus_exemplorum; i++)
		{
			imprimere("    %s\n", parser->exempla[i]);
		}
		imprimere("\n");
	}
}

vacuum
argumenta_imprimere_errorem (
	constans ArgumentaParser* parser)
{
	si (!parser) redde;

	si (parser->nuntius_erroris)
	{
		fprintf(stderr, "Errore: %s\n", parser->nuntius_erroris);
	}
	alioquin
	{
		fprintf(stderr, "Errore parsendo argumenta\n");
	}

	fprintf(stderr, "Usa --help pro auxilio\n");
}
