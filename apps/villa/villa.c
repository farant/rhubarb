/* apps/villa/villa.c - villa: fenestra vitreae in servos remotos.
 *
 * SCELETUM V4a. Catena tota vivit (fenestra -> capsula -> vitrea ->
 * internuntius -> agens), sed facies ADHUC PLACEHOLDER est: V4b eam
 * aedificat. Quod hic probatur non est quid monstretur sed quod
 * strata coniuncta sint.
 *
 * OMNE OPUS VERUM IN lib/villa_agens.c HABITAT. Haec plagula
 * nectit, non agit - ideo probationes radicis agentem custodire
 * possunt quamvis apps/ a nullo cursore verratur.
 *
 * TICTUS NUMQUAM OBSTAT: villa_agens_pulsare gradum unum omnium
 * probationum currentium agit et statim redit. Vide processus.h de
 * causa (domus filis CARET).
 *
 * Tractatores IS:
 *   villa_status {}          -> configuratio + probationes + summa
 *   villa_probare {alias?}   -> probationem incipere (non obstat)
 *   transmittere {...}       -> mundus (allowlist clientis)
 *
 * Vexillum -fumus: sine oculis - configuratio, semina, probatio
 * una per stipitem, numeri impressi, exitus.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "fenestra.h"
#include "capsula.h"
#include "vitrea.h"
#include "internuntius.h"
#include "speculum.h"
#include "cliens_tabularii.h"
#include "villa_agens.h"
#include "villa_lectores.h"
#include "capsula_villa.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* capsula fontium speculi - externus directus (mos consumptoris:
 * caput generatum non includitur, silva ".." non resolvit) */
/* <aedilis obiectum="build/speculum/villa/capsula_speculi_villa.c"/> */
externus constans CapsulaEmbed capsula_speculi_villa;

#define VILLA_PORTUS_ORDINARIUS 8753
#define VILLA_TICTUS_MS         CC
/* probatio automatica: XXX secunda inter cyclos. Probationes
 * eventus NON scribunt (telemetria non est historia), ergo pretium
 * solum itinera ssh sunt. */
#define VILLA_INTERVALLUM_MS    30000

nomen structura {
	ClientTabularii   cliens;
	VillaConfiguratio configuratio;
	VillaAgens*       agens;
	Xar*              unitates;    /* chorda - unitates petendae */
	chorda            alias;       /* servus unicus (V4b plures) */
	chorda            clavis;
	i64               tempus_ultimum_ms;
	i32               cycli;
} VillaStatus;

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

/* ========================================================================
 * TRACTATORES INTERNUNTII
 * ======================================================================== */

interior JsonValor*
_villa_status (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
	chorda* culpa)
{
	VillaStatus* v = (VillaStatus*)datum;
	JsonValor*   r = json_objectum_creare(piscina);
	constans ProbatioServi* p;

	(vacuum)argumenta;
	(vacuum)culpa;

	json_objectum_ponere(r, "alias",
		json_chorda_creare(piscina, v->alias));
	json_objectum_ponere(r, "via_ssh",
		json_chorda_creare_literis(piscina,
			(v->configuratio.via_ssh != NIHIL)
				? v->configuratio.via_ssh : "ssh"));
	json_objectum_ponere(r, "via_moderandi",
		json_chorda_creare(piscina,
			v->configuratio.via_moderandi));
	json_objectum_ponere(r, "currentes",
		json_integer_creare(piscina,
			(s64)villa_probationes_currentes(v->agens)));
	json_objectum_ponere(r, "cycli",
		json_integer_creare(piscina,
			(s64)v->cycli));

	p = villa_probatio_ultima(v->agens, v->clavis);
	si (p == NIHIL)
	{
		json_objectum_ponere(r, "probatio",
			json_nullum_creare(piscina));
		redde r;
	}
	{
		JsonValor* o = json_objectum_creare(piscina);

		json_objectum_ponere(o, "felix",
			json_boolean_creare(piscina, p->felix));
		json_objectum_ponere(o, "mora_ms",
			json_integer_creare(piscina,
			(s64)p->mora_ms));
		json_objectum_ponere(o, "causa",
			json_chorda_creare_literis(piscina,
				villa_exitus_nomen(p->causa.genus)));
		json_objectum_ponere(o, "querelae",
			json_chorda_creare(piscina, p->querelae));
		json_objectum_ponere(o, "unitates",
			json_integer_creare(piscina,
			(s64)((p->unitates != NIHIL)
					? xar_numerus(p->unitates) : ZEPHYRUM)));
		json_objectum_ponere(o, "servitia",
			json_integer_creare(piscina,
			(s64)((p->servitia != NIHIL)
					? xar_numerus(p->servitia) : ZEPHYRUM)));
		json_objectum_ponere(o, "situs",
			json_integer_creare(piscina,
			(s64)((p->situs != NIHIL)
					? xar_numerus(p->situs) : ZEPHYRUM)));
		json_objectum_ponere(o, "discus",
			json_chorda_creare(piscina, p->discus.systema));
		json_objectum_ponere(o, "capacitas",
			json_integer_creare(piscina,
			(s64)p->discus.capacitas));
		json_objectum_ponere(o, "memoria_kb",
			json_integer_creare(piscina,
			(s64)p->memoria.summa_kb));
		json_objectum_ponere(o, "tempus_activum",
			json_integer_creare(piscina,
			(s64)p->tempus_activum));
		json_objectum_ponere(r, "probatio", o);
	}
	redde r;
}

interior JsonValor*
_villa_probare (JsonValor* argumenta, Piscina* piscina,
	vacuum* datum, chorda* culpa)
{
	VillaStatus* v = (VillaStatus*)datum;
	JsonValor*   r = json_objectum_creare(piscina);
	chorda       causa = _ch("");
	b32          bene;

	(vacuum)argumenta;
	(vacuum)culpa;

	bene = villa_probationem_incipere(v->agens, v->clavis, v->alias,
		v->unitates, &causa);
	json_objectum_ponere(r, "incepta",
		json_boolean_creare(piscina, bene));
	json_objectum_ponere(r, "causa",
		json_chorda_creare(piscina, causa));
	redde r;
}

/* ========================================================================
 * PRINCIPALE
 * ======================================================================== */

s32 principale (integer argc, character** argv)
{
	Piscina*    piscina = piscina_generare_dynamicum("villa",
		16777216);
	Piscina*    piscina_vocationis = piscina_generare_dynamicum(
		"villa_vocationes", 8388608);
	VillaStatus villa;
	chorda      causa;
	b32         fumus = FALSUM;
	integer     k;
	FenestraConfiguratio figura_fenestrae;
	VitreaConfiguratio   figura_vitreae;
	Fenestra*     fenestra;
	Capsula*      capsula;
	Vitrea*       vitrea;
	Internuntius* inx;

	si (piscina == NIHIL || piscina_vocationis == NIHIL)
	{
		redde I;
	}
	memset(&villa, 0, magnitudo(VillaStatus));
	cliens_tabularii_incipere(&villa.cliens,
		(i32)VILLA_PORTUS_ORDINARIUS);
	villa.cliens.praefixum = "[villa]";
	villa.alias  = _ch("smaragda");
	villa.clavis = _ch("primus");
	causa = _ch("");

	per (k = I; k < argc; k++)
	{
		si (strcmp(argv[k], "-portus") == ZEPHYRUM && k + I < argc)
		{
			villa.cliens.portus = (i32)atoi(argv[k + I]);
			k++;
		}
		alioquin si (strcmp(argv[k], "-alias") == ZEPHYRUM
			&& k + I < argc)
		{
			villa.alias = _ch(argv[k + I]);
			k++;
		}
		alioquin si (strcmp(argv[k], "-fumus") == ZEPHYRUM)
		{
			fumus = VERUM;
		}
	}

	/* configuratio: VILLA_SSH semel, directorium moderandi 0700 */
	si (!villa_configurationem_parare(&villa.configuratio, piscina,
		&causa))
	{
		imprimere("[villa] configuratio FRACTA: %.*s\n",
			(int)causa.mensura, (constans character*)causa.datum);
		redde I;
	}
	villa.agens = villa_agens_creare(&villa.configuratio,
		&villa.cliens, piscina);
	si (villa.agens == NIHIL)
	{
		imprimere("[villa] agens FRACTUS\n");
		redde I;
	}
	villa.unitates = xar_creare(piscina, (i32)magnitudo(chorda));
	{
		chorda* e = (chorda*)xar_addere(villa.unitates);

		si (e != NIHIL)
		{
			*e = _ch("nginx.service");
		}
	}

	si (fumus)
	{
		/* PORTA SINE OCULIS: catena tota sine fenestra. Semina
		 * omittuntur si daemon abest - fumus stratum agentis
		 * probat, non daemonem. */
		constans ProbatioServi* p;
		i32 gressus = ZEPHYRUM;

		imprimere("[villa] fumus: ssh=%s\n",
			(villa.configuratio.via_ssh != NIHIL)
				? villa.configuratio.via_ssh : "ssh");
		si (!villa_probationem_incipere(villa.agens, villa.clavis,
			villa.alias, villa.unitates, &causa))
		{
			imprimere("[villa] fumus FRACTUS (initium): %.*s\n",
				(int)causa.mensura,
				(constans character*)causa.datum);
			redde I;
		}
		/* TERMINUS VERUS EST MORA PROBATIONIS (processus eam
		 * custodit), non hic numerus: numerus gradum aeternum
		 * solum arcet. Prius II miliones erant, quod conexionem
		 * ssh FRIGIDAM (salutatio TCP + auctoritas + magister
		 * novus) non capiebat dum calidam capiebat - id est,
		 * terminus TEMPORIS unitate falsa expressus, qui cursum
		 * primum solum frangebat. */
		dum (villa_probationes_currentes(villa.agens) > ZEPHYRUM
			&& gressus < 200000000)
		{
			(vacuum)villa_agens_pulsare(villa.agens);
			gressus++;
		}
		p = villa_probatio_ultima(villa.agens, villa.clavis);
		si (p == NIHIL)
		{
			imprimere("[villa] fumus FRACTUS: nulla probatio\n");
			redde I;
		}
		imprimere("[villa] fumus: felix=%d causa=%s pulsus=%d\n",
			(int)p->felix, villa_exitus_nomen(p->causa.genus),
			(int)gressus);
		imprimere("[villa] fumus: unitates=%d servitia=%d situs=%d"
			" querelae='%.*s'\n",
			(int)((p->unitates != NIHIL)
				? xar_numerus(p->unitates) : ZEPHYRUM),
			(int)((p->servitia != NIHIL)
				? xar_numerus(p->servitia) : ZEPHYRUM),
			(int)((p->situs != NIHIL)
				? xar_numerus(p->situs) : ZEPHYRUM),
			(int)p->querelae.mensura,
			(constans character*)p->querelae.datum);
		piscina_destruere(piscina_vocationis);
		piscina_destruere(piscina);
		redde p->felix ? ZEPHYRUM : I;
	}

	/* semina generum: idempotens, custodia collisionis intus */
	si (!villa_genera_seminare(villa.agens, &causa))
	{
		/* NON fatale: daemon abesse potest, aut clavis ab alia app
		 * occupata esse. Nominamus et pergimus - facies sine
		 * generibus adhuc probare potest. */
		imprimere("[villa] semina omissa: %.*s\n",
			(int)causa.mensura, (constans character*)causa.datum);
	}

	figura_fenestrae.titulus = "villa";
	figura_fenestrae.x = CC;
	figura_fenestrae.y = CC;
	figura_fenestrae.latitudo = 1000;
	figura_fenestrae.altitudo = 900;
	figura_fenestrae.vexilla = FENESTRA_CLAUDIBILIS
		| FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
	fenestra = fenestra_creare(piscina, &figura_fenestrae);
	si (fenestra == NIHIL)
	{
		imprimere("FRACTA: fenestra\n");
		redde I;
	}
	capsula = capsula_aperire(&capsula_villa, piscina);
	si (capsula == NIHIL)
	{
		imprimere("FRACTA: capsula\n");
		redde I;
	}
	figura_vitreae.origo = VITREA_ORIGO_CAPSULA;
	figura_vitreae.capsula = capsula;
	figura_vitreae.via_initialis = "index.html";
	figura_vitreae.url = NIHIL;
	figura_vitreae.inspectabilis = VERUM;
	vitrea = vitrea_creare(piscina, fenestra, &figura_vitreae);
	si (vitrea == NIHIL)
	{
		imprimere("FRACTA: vitrea\n");
		redde I;
	}
	inx = internuntius_creare(piscina, vitrea_missor, vitrea);
	si (inx == NIHIL)
	{
		imprimere("FRACTA: internuntius\n");
		redde I;
	}
	(vacuum)internuntius_praebere(inx, "villa_status",
		_villa_status, &villa);
	(vacuum)internuntius_praebere(inx, "villa_probare",
		_villa_probare, &villa);
	(vacuum)internuntius_praebere(inx, "transmittere",
		cliens_tabularii_transmittere, &villa.cliens);

	{
		Speculum* speculum = speculum_creare(piscina,
			&capsula_speculi_villa, inx, vitrea_aestimator, vitrea);
		i64 tictus = ZEPHYRUM;

		si (speculum == NIHIL)
		{
			imprimere("FRACTA: speculum\n");
			redde I;
		}
		imprimere("[villa] fenestra aperta (alias %.*s, daemon"
			" portus %d)\n", (int)villa.alias.mensura,
			(constans character*)villa.alias.datum,
			(int)villa.cliens.portus);
		fflush(stdout);

		dum (!fenestra_debet_claudere(fenestra))
		{
			Eventus eventus;
			chorda  nuntium;
			VitreaNuntiusGenus genus;
			PiscinaNotatio nota;

			fenestra_expectare_eventus(fenestra, VILLA_TICTUS_MS);
			dum (fenestra_obtinere_eventus(fenestra, &eventus))
			{
				(vacuum)speculum_tangere(speculum, &eventus);
			}
			nota = piscina_notare(piscina_vocationis);
			dum (vitrea_obtinere_nuntium(vitrea, &nuntium, &genus))
			{
				si (genus == VITREA_NUNTIUS_PONS)
				{
					internuntius_tractare(inx, nuntium,
						piscina_vocationis);
				}
				alioquin
				{
					vitrea_recargare(vitrea);
				}
			}

			/* SLOT PROBATIONIS: gradus unus, numquam obstans.
			 * Photographiae in arenis agentis vivunt, NON in
			 * piscina_vocationis quae mox reficitur. */
			si (villa_agens_pulsare(villa.agens) > ZEPHYRUM)
			{
				villa.cycli++;
			}
			/* horologium tictuum: fenestra_expectare_eventus
			 * VILLA_TICTUS_MS morari SOLET sed non promittit,
			 * ergo numerus tictuum aestimatio est, non mensura.
			 * V4b horologium verum adhibeat si praecisio importat. */
			tictus++;
			si (villa_probationes_currentes(villa.agens) == ZEPHYRUM
				&& tictus * (i64)VILLA_TICTUS_MS
					>= (i64)VILLA_INTERVALLUM_MS)
			{
				tictus = ZEPHYRUM;
				(vacuum)villa_probationem_incipere(villa.agens,
					villa.clavis, villa.alias, villa.unitates,
					&causa);
			}
			piscina_reficere(piscina_vocationis, nota);
		}
	}
	vitrea_destruere(vitrea);
	fenestra_destruere(fenestra);
	piscina_destruere(piscina_vocationis);
	piscina_destruere(piscina);
	redde ZEPHYRUM;
}
