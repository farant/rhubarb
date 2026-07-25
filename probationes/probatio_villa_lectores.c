/* probatio_villa_lectores.c - Porta lectorum villae.
 *
 * OFFLINE integre: nulla rete, nullus servus, nullus daemon. Omnis
 * probatio aut functio pura est aut contra FIXA VERA curritur
 * (probationes/fixa/villa/, ex droplet Debian capta, bis, octetim
 * comparata, manu expurgata).
 *
 * INSIDIA FIXORUM: filum_legere_totum chordam VACUAM reddit cum
 * plagula abest. Parsator chordae vacuae 'nullas res inventas'
 * feliciter reddit - ergo via mendosa probationem VIRIDEM pareret
 * quae nihil probat. Ideo omnis lectio fixi per _fixum() transit
 * quod mensuram > 0 ADSERIT antequam quicquam aliud fiat.
 *
 * Fixa tria (discus, memoria, tempus) VARIANTIA sunt: numeri inter
 * capturas mutant. Ergo hic FORMA figitur et numerus NUMQUAM -
 * nulla adsertio numerum specificum ex eis petit.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "filum.h"
#include "xar.h"
#include "villa_lectores.h"
#include <stdio.h>
#include <string.h>

#define FX_RADIX "probationes/fixa/villa/"

/* fixum legere - VACUUM = defectus, non 'nihil inventum' */
interior chorda
_fixum (Piscina* piscina, constans character* titulus)
{
	character via[CCLVI];
	chorda    contentum;

	sprintf(via, "%s%s", FX_RADIX, titulus);
	contentum = filum_legere_totum(via, piscina);

	/* PORTA: hic ruit statim si via mendosa est, potius quam
	 * parsatorem nihil invenire et probationem transire */
	si (contentum.mensura == ZEPHYRUM)
	{
		imprimere("  FIXUM ABEST AUT VACUUM: %s\n", via);
	}
	CREDO_VERUM (contentum.mensura > ZEPHYRUM);
	redde contentum;
}

/* fixum errati ssh (probationes/fixa/villa/ssh/<nomen>.err) */
interior chorda
_fixum_ssh (Piscina* piscina, constans character* titulus)
{
	character via[CCLVI];
	chorda    contentum;

	sprintf(via, "%sssh/%s.err", FX_RADIX, titulus);
	contentum = filum_legere_totum(via, piscina);
	/* fixum 'successus' VACUUM est de iure (ssh felix nihil ad
	 * erratum scribit) - ergo hic mensura non adseritur, aliter
	 * ac in _fixum. Exsistentiam solam probamus. */
	CREDO_VERUM (filum_existit(via));
	redde contentum;
}

interior chorda
_vacua_ch (vacuum)
{
	chorda c;

	c.datum = NIHIL;
	c.mensura = ZEPHYRUM;
	redde c;
}

/* argumentum n-tum cum litteris comparare */
interior b32
_arg_est (constans MandatumSsh* m, i32 index,
	constans character* litterae)
{
	si (!m->successus || index >= m->numerus)
	{
		redde FALSUM;
	}
	redde (b32)(strcmp(m->argumenta[index], litterae) == ZEPHYRUM);
}

/* an aliquod argumentum praefixum habeat */
interior b32
_arg_praefixum_adest (constans MandatumSsh* m,
	constans character* praefixum)
{
	i32            i;
	memoriae_index n = strlen(praefixum);

	si (!m->successus)
	{
		redde FALSUM;
	}
	per (i = ZEPHYRUM; i < m->numerus; i++)
	{
		si (strncmp(m->argumenta[i], praefixum, n) == ZEPHYRUM)
		{
			redde VERUM;
		}
	}
	redde FALSUM;
}

/* clausulam/situm nomine invenire */
interior constans SitusNginx*
_situs_nomine (constans Xar* situs, constans character* hospes)
{
	i32 i;

	per (i = ZEPHYRUM; i < xar_numerus(situs); i++)
	{
		constans SitusNginx* s = (constans SitusNginx*)
			xar_obtinere(situs, i);

		si (s != NIHIL
			&& chorda_aequalis_literis(s->hospes, hospes))
		{
			redde s;
		}
	}
	redde NIHIL;
}

interior constans StatusServitii*
_servitium_nomine (constans Xar* servitia, constans character* id)
{
	i32 i;

	per (i = ZEPHYRUM; i < xar_numerus(servitia); i++)
	{
		constans StatusServitii* s = (constans StatusServitii*)
			xar_obtinere(servitia, i);

		si (s != NIHIL && chorda_aequalis_literis(s->id, id))
		{
			redde s;
		}
	}
	redde NIHIL;
}

/* chordam n characterum fabricare (pro porta viae moderandi) */
interior chorda
_via_longitudinis (Piscina* piscina, i32 mensura)
{
	chorda c;
	i32    i;

	c.datum = (i8*)piscina_allocare(piscina,
		(memoriae_index)mensura);
	c.mensura = mensura;
	per (i = ZEPHYRUM; i < mensura; i++)
	{
		c.datum[i] = (i == ZEPHYRUM) ? (i8)'/' : (i8)'x';
	}
	redde c;
}

s32 principale (vacuum)
{
	Piscina* piscina;
	b32      praeteritus;

	piscina = piscina_generare_dynamicum("probatio_villa_lectores",
		8388608);
	si (!piscina)
	{
		imprimere("FRACTA: piscina\n");
		redde I;
	}
	credo_aperire(piscina);

	/* ==============================================================
	 * I. AEDIFICATOR MANDATI SSH - forma plena
	 * ============================================================== */
	{
		ConfiguratioSsh cfg;
		MandatumSsh     m;

		imprimere("\n--- I. mandatum ssh: forma plena ---\n");
		memset(&cfg, 0, magnitudo(ConfiguratioSsh));
		cfg.alias         = chorda_ex_literis("smaragda", piscina);
		cfg.via_moderandi = chorda_ex_literis(
			"/Users/exemplum/.rhubarb/cm", piscina);

		m = villa_mandatum_ssh(&cfg,
			chorda_ex_literis("uptime", piscina), piscina);

		CREDO_VERUM (m.successus);
		CREDO_VERUM (m.error == VILLA_SSH_OK);
		CREDO_VERUM (_arg_est(&m, ZEPHYRUM, "ssh"));

		/* BatchMode NON NEGOTIABILE - sine eo rogatio interactiva
		 * faciem congelaret */
		CREDO_VERUM (_arg_praefixum_adest(&m, "BatchMode=yes"));
		CREDO_VERUM (_arg_praefixum_adest(&m, "ConnectTimeout="));
		CREDO_VERUM (_arg_praefixum_adest(&m, "ControlMaster=auto"));
		CREDO_VERUM (_arg_praefixum_adest(&m, "ControlPath="));
		CREDO_VERUM (_arg_praefixum_adest(&m, "ControlPersist="));

		/* alias ut destinatio; imperium ULTIMUM */
		CREDO_VERUM (_arg_est(&m, m.numerus - II, "smaragda"));
		CREDO_VERUM (_arg_est(&m, m.numerus - I, "uptime"));
		/* vector NIHIL-terminatus pro processus_exsequi */
		CREDO_NIHIL (m.argumenta[m.numerus]);
	}

	/* II. sutura binarii + portus + clavis + usor@hospes */
	{
		ConfiguratioSsh cfg;
		MandatumSsh     m;

		imprimere("\n--- II. sutura, portus, clavis, usor ---\n");
		memset(&cfg, 0, magnitudo(ConfiguratioSsh));
		cfg.via_ssh = "/stipes/ssh";
		cfg.hospes  = chorda_ex_literis("198.51.100.7", piscina);
		cfg.usor    = chorda_ex_literis("fran", piscina);
		cfg.portus  = 2222;
		cfg.clavis  = chorda_ex_literis("/clavis/id_ed25519",
			piscina);

		m = villa_mandatum_ssh(&cfg,
			chorda_ex_literis("hostname", piscina), piscina);

		CREDO_VERUM (m.successus);
		CREDO_VERUM (_arg_est(&m, ZEPHYRUM, "/stipes/ssh"));
		CREDO_VERUM (_arg_praefixum_adest(&m, "-p"));
		CREDO_VERUM (_arg_praefixum_adest(&m, "2222"));
		CREDO_VERUM (_arg_praefixum_adest(&m, "-i"));
		CREDO_VERUM (_arg_praefixum_adest(&m, "/clavis/id_ed25519"));
		CREDO_VERUM (_arg_est(&m, m.numerus - II,
			"fran@198.51.100.7"));

		/* via_moderandi vacua = NULLA multiplicatio (semita
		 * probationis id facit; stipes socketum non ligat) */
		CREDO_FALSUM (_arg_praefixum_adest(&m, "ControlPath="));
		CREDO_FALSUM (_arg_praefixum_adest(&m, "ControlMaster=auto"));
	}

	/* III. alias PRAEVALET super hospes/usor (identitas prima) */
	{
		ConfiguratioSsh cfg;
		MandatumSsh     m;

		imprimere("\n--- III. alias praevalet ---\n");
		memset(&cfg, 0, magnitudo(ConfiguratioSsh));
		cfg.alias  = chorda_ex_literis("smaragda", piscina);
		cfg.hospes = chorda_ex_literis("198.51.100.7", piscina);
		cfg.usor   = chorda_ex_literis("radix", piscina);

		m = villa_mandatum_ssh(&cfg,
			chorda_ex_literis("uptime", piscina), piscina);

		CREDO_VERUM (m.successus);
		CREDO_VERUM (_arg_est(&m, m.numerus - II, "smaragda"));
		CREDO_FALSUM (_arg_praefixum_adest(&m, "radix@198.51.100.7"));
	}

	/* IV. PORTA VIAE MODERANDI - terminus C octetorum.
	 * Sine hac porta ssh socketum tacite deserit et omne imperium
	 * ad manum plenam degenerat: defectus se ut MORA INEXPLICATA
	 * ostendit, non ut error, quia ssh eum nonnisi sub '-v' nuntiat. */
	{
		ConfiguratioSsh cfg;
		MandatumSsh     m;

		imprimere("\n--- IV. porta viae moderandi ---\n");

		/* LIX + '/' + XL = C exacte: ACCIPITUR (terminus ipse) */
		memset(&cfg, 0, magnitudo(ConfiguratioSsh));
		cfg.alias         = chorda_ex_literis("smaragda", piscina);
		cfg.via_moderandi = _via_longitudinis(piscina, (i32)LIX);
		m = villa_mandatum_ssh(&cfg,
			chorda_ex_literis("uptime", piscina), piscina);
		CREDO_VERUM (m.successus);

		/* LX + '/' + XL = CI: RECUSATUR, causa NOMINATA */
		cfg.via_moderandi = _via_longitudinis(piscina, (i32)LX);
		m = villa_mandatum_ssh(&cfg,
			chorda_ex_literis("uptime", piscina), piscina);
		CREDO_FALSUM (m.successus);
		CREDO_VERUM (m.error == VILLA_SSH_VIA_LONGA);
		CREDO_CHORDA_CONTINET (m.causa,
			chorda_ex_literis("101", piscina));
		CREDO_NIHIL (m.argumenta);
	}

	/* V. recusationes nominatae + NULLA CONCHA */
	{
		ConfiguratioSsh cfg;
		MandatumSsh     m;

		imprimere("\n--- V. recusationes et concha absens ---\n");

		memset(&cfg, 0, magnitudo(ConfiguratioSsh));
		m = villa_mandatum_ssh(&cfg,
			chorda_ex_literis("uptime", piscina), piscina);
		CREDO_FALSUM (m.successus);
		CREDO_VERUM (m.error == VILLA_SSH_SINE_DESTINATIONE);

		cfg.alias = chorda_ex_literis("smaragda", piscina);
		m = villa_mandatum_ssh(&cfg, chorda_ex_literis("", piscina),
			piscina);
		CREDO_FALSUM (m.successus);
		CREDO_VERUM (m.error == VILLA_SSH_SINE_IMPERIO);

		/* imperium ELEMENTUM UNUM manet quamvis spatia, virgulas,
		 * lineas novas ferat - nemo eum findit, ergo classis
		 * defectuum citationis conchae omnino ABEST */
		m = villa_mandatum_ssh(&cfg,
			chorda_ex_literis(
				"systemctl restart nginx; echo 'captus' && rm -rf /",
				piscina),
			piscina);
		CREDO_VERUM (m.successus);
		CREDO_VERUM (_arg_est(&m, m.numerus - I,
			"systemctl restart nginx; echo 'captus' && rm -rf /"));
	}

	/* ==============================================================
	 * V-bis. DISCRETIO EXITUS SSH
	 *
	 * Fixa VERA, capta 2026-07-24 currendo ssh contra hospitem
	 * insolubilem, portum clausum, clavem pravam, known_hosts
	 * vacuum, imperium remotum absens. Nihil fictum: nuntii ssh
	 * per versiones et systemata mutant, et forma ficta hic
	 * praecise ea falleret quae discernere debemus.
	 * ============================================================== */
	{
		CausaExitus c;

		imprimere("\n--- V-bis. discretio exitus ssh ---\n");

		/* successus: nihil discernendum */
		c = villa_exitum_discernere(ZEPHYRUM,
			_fixum_ssh(piscina, "successus"), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_SUCCESSUS);
		CREDO_FALSUM (c.ssh_ipse);

		/* CCLV = ssh de se ipso queritur; erratum causam nominat */
		c = villa_exitum_discernere((i32)CCLV,
			_fixum_ssh(piscina, "hospes_ignotus"), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_HOSPES_IGNOTUS);
		CREDO_VERUM (c.ssh_ipse);
		CREDO_CHORDA_CONTINET (c.causa,
			chorda_ex_literis("Could not resolve", piscina));

		c = villa_exitum_discernere((i32)CCLV,
			_fixum_ssh(piscina, "recusata"), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_CONEXIO_RECUSATA);

		c = villa_exitum_discernere((i32)CCLV,
			_fixum_ssh(piscina, "tempus"), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_TEMPUS);

		c = villa_exitum_discernere((i32)CCLV,
			_fixum_ssh(piscina, "clavis_hospitis"), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_CLAVIS_HOSPITIS);

		/* INSIDIA CENTRALIS: hoc fixum lineam FALLACEM PRIMAM fert
		 * ('Load key "/dev/null": invalid format') et decisivam
		 * SECUNDAM. Parsator lineae primae 'causa ignota' diceret.
		 * Ergo adserimus et genus RECTUM et causam ex linea
		 * SECUNDA sumptam. */
		c = villa_exitum_discernere((i32)CCLV,
			_fixum_ssh(piscina, "clavis_negata"), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_PERMISSIO);
		CREDO_CHORDA_CONTINET (c.causa,
			chorda_ex_literis("Permission denied", piscina));
		CREDO_FALSUM (chorda_continet(c.causa,
			chorda_ex_literis("invalid format", piscina)));

		/* CXXVII = imperium REMOTUM abest. Distinctio quam villa
		 * postulat: ssh cucurrit, capsa respondit, imperium
		 * defuit - quod prorsus aliud est quam 'ssh ipsum abest'
		 * (PROCESSUS_ERROR_EXEC, stratum prius). */
		c = villa_exitum_discernere((i32)CXXVII,
			_fixum_ssh(piscina, "imperium_absens"), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_IMPERIUM_ABSENS);
		CREDO_FALSUM (c.ssh_ipse);
		CREDO_CHORDA_CONTINET (c.causa,
			chorda_ex_literis("command not found", piscina));

		/* INDEPENDENTIA A CONCHA REMOTA. Fixum supra formam BASH
		 * fert ('bash: line 1: X: command not found') quia concha
		 * ingressus droplet bash est. Sed /bin/sh ibi DASH est, et
		 * dash aliter loquitur: 'sh: 1: X: not found' - sine verbo
		 * 'command'. Si villa umquam imperium per 'sh -c' miserit,
		 * forma mutabitur.
		 *
		 * Discretor id superstat quia CODICE CXXVII ducitur, quem
		 * POSIX figit, non prosa conchae. Haec adsertio id PROBAT
		 * potius quam sperat - alioquin fragilitas latens maneret
		 * quam nemo inveniret nisi die quo concha mutaretur.
		 * (Forma dash ex droplet vero sumpta 2026-07-24.) */
		c = villa_exitum_discernere((i32)CXXVII,
			chorda_ex_literis(
				"sh: 1: nulla-tale-imperium: not found\n", piscina),
			piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_IMPERIUM_ABSENS);
		CREDO_FALSUM (c.ssh_ipse);
		CREDO_CHORDA_CONTINET (c.causa,
			chorda_ex_literis("not found", piscina));

		/* codex alius = imperium cucurrit et fefellit */
		c = villa_exitum_discernere((i32)III,
			chorda_ex_literis("nginx: configuration file test"
				" failed\n", piscina), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_IMPERIUM_FRACTUM);
		CREDO_FALSUM (c.ssh_ipse);
		CREDO_AEQUALIS_I32 (c.codex, (i32)III);

		/* CCLV cum errato ignoto: ssh_ipse manet VERUM (codex id
		 * dicit) sed genus ALIUS - ne causam fingamus */
		c = villa_exitum_discernere((i32)CCLV,
			chorda_ex_literis("ssh: aliquid novum et ignotum\n",
				piscina), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_SSH_ALIUS);
		CREDO_VERUM (c.ssh_ipse);
		CREDO_CHORDA_CONTINET (c.causa,
			chorda_ex_literis("aliquid novum", piscina));

		/* erratum VACUUM: genus ex codice solo, nulla ruina */
		c = villa_exitum_discernere((i32)CCLV, _vacua_ch(), piscina);
		CREDO_VERUM (c.genus == VILLA_EXITUS_SSH_ALIUS);
		CREDO_CHORDA_VACUA (c.causa);

		/* nomina omnia praesto (facies ea monstrat) */
		CREDO_VERUM (strlen(villa_exitus_nomen(
			VILLA_EXITUS_CLAVIS_HOSPITIS)) > ZEPHYRUM);
		CREDO_VERUM (strcmp(villa_exitus_nomen(
			VILLA_EXITUS_IMPERIUM_ABSENS),
			villa_exitus_nomen(VILLA_EXITUS_SSH_ALIUS)) != ZEPHYRUM);
	}

	/* ==============================================================
	 * VI. SYSTEMCTL SHOW - unitates activae
	 * ============================================================== */
	{
		chorda fx = _fixum(piscina, "systemctl_show_activae.txt");
		Xar*   servitia = villa_systemctl_legere(fx, piscina);
		constans StatusServitii* nginx;

		imprimere("\n--- VI. systemctl show: activae ---\n");
		CREDO_NON_NIHIL (servitia);
		CREDO_AEQUALIS_I32 (xar_numerus(servitia), (i32)III);

		/* CLAVE ducimur, non positione: systemd proprietates
		 * ORDINE SUO reddit (MainPID primum, Id tertium) quamvis
		 * alium ordinem petiverimus */
		nginx = _servitium_nomine(servitia, "nginx.service");
		CREDO_NON_NIHIL (nginx);
		si (nginx != NIHIL)
		{
			CREDO_CHORDA_AEQUALIS_LITERIS (nginx->status_vitae,
				"active");
			CREDO_CHORDA_AEQUALIS_LITERIS (nginx->sub_status,
				"running");
			CREDO_CHORDA_AEQUALIS_LITERIS (nginx->status_plagulae,
				"enabled");
			CREDO_AEQUALIS_I32 (nginx->pid, 936);
			CREDO_AEQUALIS_I32 (nginx->restitutiones, ZEPHYRUM);
			CREDO_VERUM (nginx->inventa);
			CREDO_VERUM (nginx->currit);

			/* Description SPATIA fert - findere ad '=' primum
			 * solum, non ad verba */
			CREDO_CHORDA_AEQUALIS_LITERIS (nginx->descriptio,
				"A high performance web server and a reverse"
				" proxy server");

			/* tempus chorda HUMANA est, VERBATIM servata - eam
			 * parsare temptare error esset (pete
			 * ActiveEnterTimestampMonotonic si numerum vis) */
			CREDO_CHORDA_AEQUALIS_LITERIS (nginx->tempus_initii,
				"Fri 2025-09-12 19:42:15 UTC");
		}
	}

	/* VII. systemctl show - unitas IGNOTA (valores vacui NORMALES) */
	{
		chorda fx = _fixum(piscina, "systemctl_show_ignota.txt");
		Xar*   servitia = villa_systemctl_legere(fx, piscina);
		constans StatusServitii* s;

		imprimere("\n--- VII. systemctl show: ignota ---\n");
		CREDO_NON_NIHIL (servitia);
		CREDO_AEQUALIS_I32 (xar_numerus(servitia), (i32)I);

		s = (constans StatusServitii*)xar_obtinere(servitia,
			ZEPHYRUM);
		CREDO_NON_NIHIL (s);
		si (s != NIHIL)
		{
			CREDO_CHORDA_AEQUALIS_LITERIS (s->status_oneris,
				"not-found");
			CREDO_FALSUM (s->inventa);
			CREDO_FALSUM (s->currit);
			CREDO_AEQUALIS_I32 (s->pid, ZEPHYRUM);
			/* 'UnitFileState=' et 'ActiveEnterTimestamp=' NUDAE
			 * redduntur - vacuum est valor legitimus, non defectus */
			CREDO_CHORDA_VACUA (s->status_plagulae);
			CREDO_CHORDA_VACUA (s->tempus_initii);
			/* Description nomen petitum REPETIT cum unitas abest */
			CREDO_CHORDA_AEQUALIS_LITERIS (s->descriptio,
				"nulla-tale-servitium.service");
		}
	}

	/* VIII. systemctl show - unitas EXSISTENS sed non currens */
	{
		chorda fx = _fixum(piscina, "systemctl_show_inactiva.txt");
		Xar*   servitia = villa_systemctl_legere(fx, piscina);
		constans StatusServitii* s;

		imprimere("\n--- VIII. systemctl show: inactiva ---\n");
		CREDO_AEQUALIS_I32 (xar_numerus(servitia), (i32)I);
		s = (constans StatusServitii*)xar_obtinere(servitia,
			ZEPHYRUM);
		CREDO_NON_NIHIL (s);
		si (s != NIHIL)
		{
			/* onerata SED non currens - duo iudicia DIVERSA, quod
			 * unum vexillum 'salva' confunderet */
			CREDO_VERUM (s->inventa);
			CREDO_FALSUM (s->currit);
			CREDO_CHORDA_AEQUALIS_LITERIS (s->status_plagulae,
				"disabled");
			CREDO_CHORDA_VACUA (s->tempus_initii);
		}
	}

	/* ==============================================================
	 * IX. LIST-UNITS - legenda, columnae vagae, descriptio spatiosa
	 * ============================================================== */
	{
		chorda fx = _fixum(piscina, "list_units.txt");
		Xar*   unitates = villa_unitates_legere(fx, piscina);
		i32    i;
		b32    nginx_inventa = FALSUM;
		b32    prosa_inventa = FALSUM;

		imprimere("\n--- IX. list-units ---\n");
		CREDO_NON_NIHIL (unitates);

		/* effusio XXV unitates enumerat; LEGENDA in calce (post
		 * lineam vacuam) prosam fert quae, si legeretur, ut
		 * unitates appareret */
		CREDO_AEQUALIS_I32 (xar_numerus(unitates), (i32)XXV);

		per (i = ZEPHYRUM; i < xar_numerus(unitates); i++)
		{
			constans UnitasCursoria* u = (constans UnitasCursoria*)
				xar_obtinere(unitates, i);

			si (u == NIHIL)
			{
				perge;
			}
			si (chorda_aequalis_literis(u->unitas, "nginx.service"))
			{
				nginx_inventa = VERUM;
				CREDO_CHORDA_AEQUALIS_LITERIS (u->onus, "loaded");
				CREDO_CHORDA_AEQUALIS_LITERIS (u->vita, "active");
				CREDO_CHORDA_AEQUALIS_LITERIS (u->sub, "running");
				/* DESCRIPTIO ultima columna est et spatia fert -
				 * quattuor verba prima sola finduntur */
				CREDO_CHORDA_AEQUALIS_LITERIS (u->descriptio,
					"A high performance web server and a reverse"
					" proxy server");
			}
			si (chorda_aequalis_literis(u->unitas, "Legend:"))
			{
				prosa_inventa = VERUM;
			}
		}
		CREDO_VERUM (nginx_inventa);
		CREDO_FALSUM (prosa_inventa);

		/* COLUMNAE VAGAE: expurgatio fixi ordinationem in tribus
		 * lineis fregit (spatium unum pro columnis ordinatis).
		 * Parsator per CURSUS spatii albi findit, ergo eas recte
		 * legit - quod parsator columnarum fixarum non faceret. */
		{
			b32 libri_inventa = FALSUM;

			per (i = ZEPHYRUM; i < xar_numerus(unitates); i++)
			{
				constans UnitasCursoria* u =
					(constans UnitasCursoria*)
						xar_obtinere(unitates, i);

				si (u != NIHIL
					&& chorda_aequalis_literis(u->unitas,
						"libri.service"))
				{
					libri_inventa = VERUM;
					CREDO_CHORDA_AEQUALIS_LITERIS (u->onus,
						"loaded");
					CREDO_CHORDA_AEQUALIS_LITERIS (u->descriptio,
						"Libri Desiderati");
				}
			}
			CREDO_VERUM (libri_inventa);
		}
	}

	/* ==============================================================
	 * X. NGINX -T - clausulae crudae
	 * ============================================================== */
	{
		chorda fx = _fixum(piscina, "nginx_T.txt");
		Xar*   clausulae = villa_nginx_clausulas_legere(fx, piscina);
		constans ClausulaNginx* prima;

		imprimere("\n--- X. nginx -T: clausulae crudae ---\n");
		CREDO_NON_NIHIL (clausulae);

		/* SEPTEM bloca 'server' vera. Fixum TRIA amplius in
		 * COMMENTARIIS fert (duo in exemplari #mail, unum in
		 * hospite virtuali exemplari sites-enabled/default) -
		 * scrutator qui commentaria non tollit X numeraret. */
		CREDO_AEQUALIS_I32 (xar_numerus(clausulae), (i32)VII);

		prima = (constans ClausulaNginx*)xar_obtinere(clausulae,
			ZEPHYRUM);
		CREDO_NON_NIHIL (prima);
		si (prima != NIHIL)
		{
			CREDO_CHORDA_AEQUALIS_LITERIS (prima->hospes,
				"libri.exemplum.com");
			CREDO_CHORDA_AEQUALIS_LITERIS (prima->hospites,
				"libri.exemplum.com www.libri.exemplum.com");

			/* 'proxy_pass' INTRA 'location' vivit - uno unco
			 * altius quam 'server_name'. Scrutator planus eum
			 * omnino non videret. */
			CREDO_CHORDA_AEQUALIS_LITERIS (prima->destinatio,
				"http://localhost:4003");
			/* UNUM proxy_pass in hoc bloco - numerus terminum
			 * 'primum-vincit' VISIBILEM facit, ne facies unum ex
			 * pluribus tacite ut solum ostendat */
			CREDO_AEQUALIS_I32 (prima->destinationes, (i32)I);

			/* 'ssl_certificate', NON 'ssl_certificate_key' quod
			 * lineam proximam occupat */
			CREDO_CHORDA_AEQUALIS_LITERIS (prima->certificatum,
				"/etc/letsencrypt/live/libri.exemplum.com/"
				"fullchain.pem");
			CREDO_VERUM (prima->ssl);
			CREDO_FALSUM (prima->redirigit);

			/* marca '# configuration file X:' COMMENTARIUM est -
			 * ergo ante ablationem commentariorum legenda */
			CREDO_CHORDA_AEQUALIS_LITERIS (prima->plagula,
				"/etc/nginx/sites-enabled/libri");
		}

		/* clausula secunda = geminum Certbot: SOLUM redirigit */
		{
			constans ClausulaNginx* socia = (constans ClausulaNginx*)
				xar_obtinere(clausulae, I);

			CREDO_NON_NIHIL (socia);
			si (socia != NIHIL)
			{
				/* 'return 301' INTRA 'if (...) {' iacet - ergo
				 * profunditas uncorum vera numeranda est */
				CREDO_VERUM (socia->redirigit);
				CREDO_CHORDA_VACUA (socia->destinatio);
				CREDO_CHORDA_VACUA (socia->certificatum);
				CREDO_FALSUM (socia->ssl);
				CREDO_CHORDA_AEQUALIS_LITERIS (socia->hospes,
					"libri.exemplum.com");
			}
		}

		/* clausula ordinaria: nulla destinatio, sed RADIX -
		 * 'non omne blocum upstream habet' */
		{
			i32 i;
			b32 communis_inventa = FALSUM;

			per (i = ZEPHYRUM; i < xar_numerus(clausulae); i++)
			{
				constans ClausulaNginx* c =
					(constans ClausulaNginx*)
						xar_obtinere(clausulae, i);

				si (c != NIHIL
					&& chorda_aequalis_literis(c->hospes, "_"))
				{
					communis_inventa = VERUM;
					CREDO_CHORDA_VACUA (c->destinatio);
					CREDO_CHORDA_VACUA (c->certificatum);
					CREDO_CHORDA_AEQUALIS_LITERIS (c->radix,
						"/var/www/html");
					CREDO_FALSUM (c->ssl);
					CREDO_CHORDA_AEQUALIS_LITERIS (c->plagula,
						"/etc/nginx/sites-enabled/default");
				}
			}
			CREDO_VERUM (communis_inventa);
		}

		/* 'server_tokens' NON est 'server' - comparatio verbi TOTA.
		 * Si praefixo compararetur, clausula phantasma ex nginx.conf
		 * nasceretur cum hospite vacuo. */
		{
			i32 i;
			b32 phantasma = FALSUM;

			per (i = ZEPHYRUM; i < xar_numerus(clausulae); i++)
			{
				constans ClausulaNginx* c =
					(constans ClausulaNginx*)
						xar_obtinere(clausulae, i);

				si (c != NIHIL && chorda_vacua(c->hospes))
				{
					phantasma = VERUM;
				}
			}
			CREDO_FALSUM (phantasma);
		}
	}

	/* XI. NGINX -T - situs plicati (gemina Certbot conflata) */
	{
		chorda fx = _fixum(piscina, "nginx_T.txt");
		Xar*   situs = villa_nginx_legere(fx, piscina);
		constans SitusNginx* libri;
		constans SitusNginx* communis;

		imprimere("\n--- XI. nginx -T: situs plicati ---\n");
		CREDO_NON_NIHIL (situs);

		/* VII clausulae -> IV situs. Certbot bloca GEMINA parit
		 * (HTTPS cum proxy_pass + HTTP quod solum redirigit);
		 * qui blocum unum per situm praesumit DUPLICITER numerat. */
		CREDO_AEQUALIS_I32 (xar_numerus(situs), (i32)IV);

		libri = _situs_nomine(situs, "libri.exemplum.com");
		CREDO_NON_NIHIL (libri);
		si (libri != NIHIL)
		{
			CREDO_AEQUALIS_I32 (libri->clausulae, (i32)II);
			CREDO_VERUM (libri->habet_redirectionem);
			CREDO_VERUM (libri->ssl);
			CREDO_CHORDA_AEQUALIS_LITERIS (libri->destinatio,
				"http://localhost:4003");
			CREDO_CHORDA_AEQUALIS_LITERIS (libri->certificatum,
				"/etc/letsencrypt/live/libri.exemplum.com/"
				"fullchain.pem");
		}

		/* situs communis ('server_name _'): unum blocum, nulla
		 * redirectio, nullum upstream - sed situs verus manet */
		communis = _situs_nomine(situs, "_");
		CREDO_NON_NIHIL (communis);
		si (communis != NIHIL)
		{
			CREDO_AEQUALIS_I32 (communis->clausulae, (i32)I);
			CREDO_FALSUM (communis->habet_redirectionem);
			CREDO_CHORDA_VACUA (communis->destinatio);
			CREDO_CHORDA_AEQUALIS_LITERIS (communis->radix,
				"/var/www/html");
		}

		/* ceteri duo situs procurati adsunt cum destinationibus
		 * suis (portus interni DIVERSI - ergo plicatio situs non
		 * confudit) */
		{
			constans SitusNginx* tempora =
				_situs_nomine(situs, "tempora.exemplum.com");
			constans SitusNginx* gemma =
				_situs_nomine(situs, "gemma.exemplum.org");

			CREDO_NON_NIHIL (tempora);
			CREDO_NON_NIHIL (gemma);
			si (tempora != NIHIL)
			{
				CREDO_CHORDA_AEQUALIS_LITERIS (tempora->destinatio,
					"http://localhost:4001");
				CREDO_VERUM (tempora->habet_redirectionem);
				CREDO_AEQUALIS_I32 (tempora->clausulae, (i32)II);
			}
			si (gemma != NIHIL)
			{
				CREDO_CHORDA_AEQUALIS_LITERIS (gemma->destinatio,
					"http://localhost:4002");
				CREDO_VERUM (gemma->habet_redirectionem);
				/* gemma nomen UNUM habet (nullum 'www') - ergo
				 * hospites == hospes */
				CREDO_CHORDA_AEQUALIS_LITERIS (gemma->hospites,
					"gemma.exemplum.org");
			}
		}
	}

	/* ==============================================================
	 * XII. LECTORES PARVI - FORMA figitur, numerus NUMQUAM
	 * ============================================================== */
	{
		chorda      fx = _fixum(piscina, "df.txt");
		StatusDisci discus;

		imprimere("\n--- XII. df -P ---\n");
		CREDO_VERUM (villa_discum_legere(fx, &discus, piscina));
		CREDO_CHORDA_AEQUALIS_LITERIS (discus.systema, "/dev/vda1");
		CREDO_CHORDA_AEQUALIS_LITERIS (discus.punctum, "/");

		/* fixum VARIANS est: numeri inter capturas mutant. Ergo
		 * SOLA relatio figitur, numquam valor.
		 *
		 * CAUTIO (correctio 2026-07-24): adsertio prior
		 * 'praesto > ZEPHYRUM' erat. Ea in disco VERE PLENO
		 * FALSA fit - id est, probatio rumperetur in ipso casu
		 * cuius causa exsistit. Adsertio quae casum suum non
		 * superstat nulla adsertio est. Ergo TERMINI figuntur,
		 * non positivitas. */
		CREDO_VERUM (discus.frusta > ZEPHYRUM);
		CREDO_VERUM (discus.usa <= discus.frusta);
		CREDO_VERUM (discus.praesto <= discus.frusta);
		CREDO_VERUM (discus.usa + discus.praesto <= discus.frusta);
		CREDO_VERUM (discus.capacitas <= (i32)C);
	}

	/* XIII. meminfo */
	{
		chorda         fx = _fixum(piscina, "meminfo.txt");
		StatusMemoriae memoria;

		imprimere("\n--- XIII. /proc/meminfo ---\n");
		CREDO_VERUM (villa_memoriam_legere(fx, &memoria));

		CREDO_VERUM (memoria.summa_kb > ZEPHYRUM);
		CREDO_VERUM (memoria.libera_kb > ZEPHYRUM);
		CREDO_VERUM (memoria.praesto_kb > ZEPHYRUM);
		CREDO_VERUM (memoria.condita_kb > ZEPHYRUM);
		/* relationes quae per omnem capturam verae manent */
		CREDO_VERUM (memoria.libera_kb <= memoria.summa_kb);
		CREDO_VERUM (memoria.praesto_kb <= memoria.summa_kb);
		CREDO_VERUM (memoria.libera_kb <= memoria.praesto_kb);
	}

	/* XIV. /proc/uptime - pars integra sola, nullum punctum
	 * fluitans in villa */
	{
		chorda fx = _fixum(piscina, "uptime.txt");
		i64    secunda = ZEPHYRUM;

		imprimere("\n--- XIV. /proc/uptime ---\n");
		CREDO_VERUM (villa_tempus_activum_legere(fx, &secunda));
		CREDO_VERUM (secunda > ZEPHYRUM);
		/* '27223431.74 25953501.86' - campus PRIMUS solus, et
		 * fractio abscissa (numerus integer, non 27223431.74) */
		CREDO_VERUM (secunda < (i64)27223432);
		CREDO_VERUM (secunda > (i64)27223430);
	}

	/* XV. intrantia degenerata: parsatores taceant, non ruant */
	{
		chorda      vacua;
		StatusDisci discus;
		i64         secunda = ZEPHYRUM;

		imprimere("\n--- XV. intrantia degenerata ---\n");
		vacua.datum   = NIHIL;
		vacua.mensura = ZEPHYRUM;

		CREDO_AEQUALIS_I32 (
			xar_numerus(villa_systemctl_legere(vacua, piscina)),
			ZEPHYRUM);
		CREDO_AEQUALIS_I32 (
			xar_numerus(villa_unitates_legere(vacua, piscina)),
			ZEPHYRUM);
		CREDO_AEQUALIS_I32 (
			xar_numerus(villa_nginx_legere(vacua, piscina)),
			ZEPHYRUM);
		CREDO_FALSUM (villa_discum_legere(vacua, &discus, piscina));
		CREDO_FALSUM (villa_tempus_activum_legere(vacua, &secunda));

		/* prosa quae nihil promittit: nulla clausula, nulla ruina */
		{
			chorda prosa = chorda_ex_literis(
				"nulla est hic configuratio\nsed lineae sunt\n",
				piscina);

			CREDO_AEQUALIS_I32 (
				xar_numerus(
					villa_nginx_clausulas_legere(prosa, piscina)),
				ZEPHYRUM);
		}

		/* unci INAEQUALES: '}' plus quam '{' - profunditas infra
		 * zephyrum non labatur (numerus insignatus circumiret et
		 * clausulas phantasmas pareret) */
		{
			chorda pravum = chorda_ex_literis(
				"}\n}\nserver {\n  server_name a.exemplum.com;\n}\n",
				piscina);
			Xar* c = villa_nginx_clausulas_legere(pravum, piscina);

			CREDO_AEQUALIS_I32 (xar_numerus(c), (i32)I);
		}
	}

	/* ==============================================================
	 * XVI-XIX. FORMAE DEFECTUS
	 *
	 * Fixa capta 2026-07-24 (post V3), unitate systemd temporaria
	 * in droplet CONSULTO fracta - vide captare_fracta.sh. Ante
	 * haec parsator contra SANITATEM SOLAM probatus erat, quod in
	 * instrumento cuius tota causa est defectum ostendere absurdum
	 * est.
	 * ============================================================== */

	/* XVI. unitas FRACTA: causa et codex adsunt */
	{
		chorda fx = _fixum(piscina, "systemctl_show_fracta.txt");
		Xar*   servitia = villa_systemctl_legere(fx, piscina);
		constans StatusServitii* s;

		imprimere("\n--- XVI. systemctl show: FRACTA ---\n");
		CREDO_AEQUALIS_I32 (xar_numerus(servitia), (i32)I);
		s = (constans StatusServitii*)xar_obtinere(servitia,
			ZEPHYRUM);
		CREDO_NON_NIHIL (s);
		si (s != NIHIL)
		{
			CREDO_CHORDA_AEQUALIS_LITERIS (s->status_vitae,
				"failed");
			CREDO_CHORDA_AEQUALIS_LITERIS (s->sub_status, "failed");
			CREDO_VERUM (s->fracta);
			CREDO_VERUM (s->inventa);   /* onerata SED fracta */
			CREDO_FALSUM (s->currit);

			/* haec duo sunt tota ratio propter quam fixum fractum
			 * capiendum fuit: in nullo alio fixo apparent */
			CREDO_CHORDA_AEQUALIS_LITERIS (s->causa_finis,
				"exit-code");
			CREDO_AEQUALIS_I32 (s->codex_exitus, (i32)I);

			CREDO_CHORDA_AEQUALIS_LITERIS (s->status_plagulae,
				"static");
		}
	}

	/* XVII. DUAE INSIDIAE quas fixum fractum revelavit.
	 * Hae adsertiones sunt forma DURABILIS monitionis: commentarius
	 * solus senesceret, probatio clamat. */
	{
		Xar* fracta = villa_systemctl_legere(
			_fixum(piscina, "systemctl_show_fracta.txt"), piscina);
		Xar* ignota = villa_systemctl_legere(
			_fixum(piscina, "systemctl_show_ignota.txt"), piscina);
		Xar* inactiva = villa_systemctl_legere(
			_fixum(piscina, "systemctl_show_inactiva.txt"), piscina);
		constans StatusServitii* f;
		constans StatusServitii* g;
		constans StatusServitii* h;

		imprimere("\n--- XVII. insidiae duae ---\n");
		f = (constans StatusServitii*)xar_obtinere(fracta, ZEPHYRUM);
		g = (constans StatusServitii*)xar_obtinere(ignota, ZEPHYRUM);
		h = (constans StatusServitii*)xar_obtinere(inactiva,
			ZEPHYRUM);
		CREDO_NON_NIHIL (f);
		CREDO_NON_NIHIL (g);
		CREDO_NON_NIHIL (h);
		si (f == NIHIL || g == NIHIL || h == NIHIL)
		{
			imprimere("  (structurae absunt - reliqua omissa)\n");
		}
		alioquin
		{
			/* INSIDIA (a): 'ActiveEnterTimestamp=' VACUUM est in
			 * FRACTA, IGNOTA, et INACTIVA pariter. Ergo campus
			 * vacuus de causa NIHIL dicit - eo ut indicio uti
			 * ('vacuum ergo non inventa') falsum esset. */
			CREDO_CHORDA_VACUA (f->tempus_initii);
			CREDO_CHORDA_VACUA (g->tempus_initii);
			CREDO_CHORDA_VACUA (h->tempus_initii);

			/* INSIDIA (b): systemd 'Result=success' etiam IGNOTAE
			 * et MORTUAE reddit. Ergo causa_finis SOLA monstrata
			 * mendacium ostenderet ('Result: success' iuxta
			 * servitium quod non currit). Vexillum 'fracta'
			 * iudicium portat quod chorda non portat. */
			CREDO_CHORDA_AEQUALIS_LITERIS (g->causa_finis,
				"success");
			CREDO_CHORDA_AEQUALIS_LITERIS (h->causa_finis,
				"success");
			CREDO_FALSUM (g->fracta);
			CREDO_FALSUM (h->fracta);
			CREDO_VERUM (f->fracta);

			/* tria genera, tria iudicia DISTINCTA - quod unum
			 * vexillum 'salva' confunderet */
			CREDO_FALSUM (g->inventa);   /* ignota */
			CREDO_VERUM (h->inventa);    /* inventa, mortua */
			CREDO_VERUM (f->inventa);    /* inventa, fracta */
		}
	}

	/* XVIII. unitas ignota IN MEDIO responsi multiplicis - recordum
	 * non-inventum vicinos suos non corrumpat */
	{
		chorda fx = _fixum(piscina,
			"systemctl_show_medio_ignota.txt");
		Xar*   servitia = villa_systemctl_legere(fx, piscina);
		constans StatusServitii* medium;

		imprimere("\n--- XVIII. ignota in medio ---\n");
		CREDO_AEQUALIS_I32 (xar_numerus(servitia), (i32)III);

		medium = (constans StatusServitii*)xar_obtinere(servitia, I);
		CREDO_NON_NIHIL (medium);
		si (medium != NIHIL)
		{
			CREDO_FALSUM (medium->inventa);
		}
		/* vicini INTACTI: primus et ultimus adhuc currunt */
		{
			constans StatusServitii* primus = (constans
				StatusServitii*)xar_obtinere(servitia, ZEPHYRUM);
			constans StatusServitii* ultimus = (constans
				StatusServitii*)xar_obtinere(servitia, II);

			CREDO_NON_NIHIL (primus);
			CREDO_NON_NIHIL (ultimus);
			si (primus != NIHIL && ultimus != NIHIL)
			{
				CREDO_VERUM (primus->currit);
				CREDO_VERUM (ultimus->currit);
				CREDO_VERUM (primus->pid > ZEPHYRUM);
				CREDO_VERUM (ultimus->pid > ZEPHYRUM);
			}
		}
	}

	/* XIX. list-units cum unitate fracta / vacuum / diaria vacua */
	{
		Xar* fractae = villa_unitates_legere(
			_fixum(piscina, "list_units_fractae.txt"), piscina);
		Xar* vacuae = villa_unitates_legere(
			_fixum(piscina, "list_units_vacuae.txt"), piscina);
		constans UnitasCursoria* u;

		imprimere("\n--- XIX. list-units: fractae et vacuae ---\n");
		CREDO_AEQUALIS_I32 (xar_numerus(fractae), (i32)I);
		u = (constans UnitasCursoria*)xar_obtinere(fractae, ZEPHYRUM);
		CREDO_NON_NIHIL (u);
		si (u != NIHIL)
		{
			CREDO_CHORDA_AEQUALIS_LITERIS (u->vita, "failed");
			CREDO_CHORDA_AEQUALIS_LITERIS (u->sub, "failed");
		}

		/* FORMA VACUA propria est, non absentia formae: caput +
		 * linea vacua + 'N loaded units listed.'. Parsator
		 * ZEPHYRUM reddat - id quod probat parsatorem lineam
		 * vacuam ante prosam invenire, non prosam legere. */
		CREDO_AEQUALIS_I32 (xar_numerus(vacuae), ZEPHYRUM);

		/* diaria vacua: journalctl praefationem 'Hint:'
		 * MULTILINEAM ante '-- No entries --' emittit. Villa v1
		 * diaria non parsat sed MONSTRAT - ergo hic solum figimus
		 * quod praefatio adest, ut quisquis eam tollere volet
		 * sciat quid tollendum sit. */
		{
			chorda diaria = _fixum(piscina, "journalctl_vacua.txt");

			CREDO_CHORDA_INCIPIT (diaria,
				chorda_ex_literis("Hint:", piscina));
			CREDO_CHORDA_CONTINET (diaria,
				chorda_ex_literis("-- No entries --", piscina));
		}
	}

	imprimere("\n");
	credo_imprimere_compendium();

	praeteritus = credo_omnia_praeterierunt();
	piscina_destruere(piscina);

	si (praeteritus)
	{
		redde ZEPHYRUM;
	}
	redde I;
}
