#ifndef MANUS_H
#define MANUS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "fasti.h"

/* manus.h - manus quae in applicationem vivam porrigitur
 *
 * PROBLEMA: imperium canalem dedit - iussum intrat, valor redit.
 * Sed canalis nudus probationem non facit. Probatio scripta super
 * canalem nudum tria repetit in omni linea: JS componere, tesseram
 * petere, tesseram pulsare donec perficiatur. Quod ter repetitur,
 * ter falli potest.
 *
 * SOLUTIO: manus. Premit, scribit, EXSPECTAT, iudicat. Quae in
 * imperio tria itinera erant, hic unum vocamen est.
 *
 * ==================================================
 * CARDO: MORA, NON ASSERTIO
 * ==================================================
 *
 * Asserta pars facilis sunt. Quod suitas interfaciei semper necat
 * ASYNCHRONIA est: ictum immittis, DOM se renovat - QUANDO? Omne
 * vitium fragile in historia probationum interfacierum vitium
 * TEMPORIS est.
 *
 * Et quisquis 'somnum CC ms' scribit suitam MENTIRI docet: transit
 * quia CC millisecunda hodie in hac machina satis fuerunt. Cras,
 * in machina onerata, non erunt - et suita quae heri vera erat
 * falsum dicet de codice qui non mutatus est.
 *
 * ERGO: nulla functio huius bibliothecae somnum praebet. Non quia
 * oblitus sum: quia si somnus commode scribi potest, aliquis eo
 * utetur. Consilium est, non omissio.
 *
 * ==================================================
 * QUOMODO EXSPECTATIO FIT (et cur non aliter)
 * ==================================================
 *
 * Mora IN PAGINA fit, non hic. JS missum promissum reddit quod se
 * solvit cum condicio impletur aut terminus transit; imperium
 * promissa IAM exspectat (vide imperium.h 'PROMISSA TRACTANTUR'),
 * ergo unum iter sufficit.
 *
 * CUR NON HINC PULSARE: pulsatio ab extra condiciones TRANSEUNTES
 * amittit - quod inter duas petitiones apparuit et abiit numquam
 * visum est. Intus, ubi mutationes fiunt, nihil elabitur. Et
 * exspectatio duorum secundorum quadraginta connexiones costaret
 * ubi una sufficit.
 *
 * TERMINUS DUPLEX tamen: pagina suum terminum servat, et manus
 * suum (paulo longiorem). Nam si pagina ipsa haeret - gyrus
 * infinitus, applicatio mortua - promissum numquam solvitur et
 * tessera in aeternum pendet. Terminus exterior custos est contra
 * id quod interior custodire non potest: se ipsum.
 *
 * ==================================================
 * ASSERTA EXSPECTANT
 * ==================================================
 *
 * CREDO_MANUS_* non semel interrogant: exspectant donec verum sit
 * aut terminus transeat. Hoc est quod somnum SUPERVACUUM facit -
 * non solum vetitum. Nam si assertum ipsum exspectat, causa somni
 * scribendi omnino perit.
 *
 *   CREDO_MANUS_TEXTUS(m, "#titulus", "Laboratorium 0001");
 *     -> donec textus par sit, aut II s transeant
 *
 * Fractum nuntiat QUID exspectaverit et QUID viderit - non solum
 * 'falsum'. Assertum quod 'falsum' dicit et tacet horam furatur.
 *
 * ==================================================
 * OMNIA 'VISIBILE' SIGNIFICANT, NON 'PRAESENS'
 * ==================================================
 *
 * Selector quisque huius bibliothecae elementa VISIBILIA solum
 * videt: neque 'display:none', neque 'visibility:hidden', et area
 * non vacua. Attributum 'hidden' ergo elementum AUFERT quantum ad
 * manum attinet.
 *
 * CUR: elementum occultum in DOM MANET. Assertum quod praesentiam
 * quaerit ergo VIRIDE fit de re quam usor videre non potest -
 * idemque de scaena quae numquam abiit. Mensuratum: probatio
 * experimenti 0001 'forma abiit' asseruit dum forma tantum occulta
 * esset, et sine hac lege transisset. Transitus scaenarum - id
 * ipsum quod probandum erat - probari non poterat.
 *
 * Idem valet de ACTIONIBUS: premere quod usor videre non potest
 * numquam intenditur, ergo manus id recusat.
 *
 * PRO DOM CRUDO: manus_aestimare, ubi JS tuum quidquid vis
 * interrogat.
 *
 * ==================================================
 * MANUS FRACTA TACET
 * ==================================================
 *
 * Post primum fractum manus FRACTA fit: vocamina sequentia nihil
 * agunt, nihil notant, statim redeunt. Una causa vera in compendio
 * apparet, non decem sequelae eius.
 *
 * Nam applicatio quae elementum primum non ostendit reliqua omnia
 * fallet, et decem lineae rubrae unum vitium celant. Praeterea
 * manus fracta applicationem iam falsam non pulsat.
 *
 * USUS:
 *   Manus* m = manus_aperire(piscina, "127.0.0.1", portus);
 *
 *   manus_premere(m, "#nav-experimenta");
 *   manus_scribere(m, "#quaestio", "utrum nexus adiuvent?");
 *
 *   CREDO_MANUS_EXISTIT(m, ".tessera");
 *   CREDO_MANUS_TEXTUS(m, "#titulus", "Laboratorium 0001");
 *   CREDO_MANUS_NUMERUS(m, ".tessera", III);
 *
 *   manus_imaginem(m, "/tmp/0001.png");
 *   manus_claudere(m);
 *
 * PERICULUM - MODUS DEV SOLUS: manus per imperium loquitur, et
 * imperium exsecutionem codicis arbitrarii praebet. Applicatio
 * probanda sub vexillo -vivum currat, numquam vecta.
 */

nomen structura Manus Manus;

/* ==================================================
 * Morae (fasti.h Mora - ms, SIGNATA)
 * ================================================== */

/* Terminus ordinarius assertorum. Duo secunda: satis ut reditio
 * atque unum iter retis transeant, parum ut suita fracta cito
 * respondeat. */
#define MANUS_MORA_ORDINARIA  ((Mora)MM)

/* Pro re quae iam adesse DEBET - eius mora vitium est, non
 * tarditas. */
#define MANUS_MORA_BREVIS     ((Mora)CC)

/* Pro re quae vere tarda est: aedificatio, reditus retis veri. */
#define MANUS_MORA_LONGA      ((Mora)(X * M))

/* ==================================================
 * Cyclus vitae
 * ================================================== */

/* Ad applicationem IAM CURRENTEM se applicare. Semita ordinaria
 * dum laboras: applicatio in altera fenestra cum -vivum currit,
 * probatio ei se applicat.
 *
 * NIHIL si portus non respondet - nam manus quae nihil tangit
 * silentio omnia asserta praeterire faceret. */
Manus*
manus_aperire (
    Piscina*            piscina,
    constans character* hospes,
    i32                 portus);

/* Applicationem ipsam generare, portum exspectare, manum reddere.
 * Semita hermetica (integratio continua, machina aliena).
 *
 * argumenta: vector NIHIL-terminatus, ut processus.h postulat;
 *            argumenta[0] = via ad binarium. Vexillum -vivum ET
 *            portum ipse pone - manus portum non coniectat.
 * portus:    idem quem applicationi dedisti.
 * mora:      quam diu portum exspectet antequam desperet.
 *
 * CAVE FISTULAS: processus_incipere effusionem per fistulas capit,
 * et fistula plena (LXIV KB) scriptorem OBSTRUIT. Applicatio quae
 * multa in stdout fundit ergo haereret si nemo hauriret. Manus
 * pulsat in omni gyro exspectationis suae - id est cur haec via
 * gyrum habere DEBET etiam cum nihil exspectat. */
Manus*
manus_incipere (
    Piscina*                    piscina,
    constans character* constans* argumenta,
    i32                         portus,
    Mora                        mora);

/* Connexionem claudere; si manus applicationem genuit, eam quoque
 * occidere (processus_abrumpere). Tuta si iam clausa. */
vacuum
manus_claudere (
    Manus* manus);

/* ==================================================
 * Errores paginae
 * ==================================================
 *
 * Manus collectorem in paginam ponit cum aperitur: exceptiones non
 * captae ('error'), promissa reiecta sine captura
 * ('unhandledrejection'), et console.error involutum.
 *
 * CUR HOC NECESSARIUM SIT: asserta tua omnia transire possunt dum
 * pagina exceptiones vomit. Facies quae in via silenter cecidit
 * saepe eundem textum ostendit - ergo suita viridis manet et vitium
 * ad usorem it. Assertum ultimum eorum quae probatio de se ipsa
 * scire debet.
 *
 * QUOD NON CAPITUR: quidquid ANTE manus_aperire acciderit - onus
 * paginae ipsum inprimis. Collector enim tunc nondum aderat. Pro
 * illis vitrea ipsa manus tendere debet (semita non aedificata:
 * involucrum in shim internuntii, quod OMNI applicationi prodesset,
 * non probationibus solis). */

/* Quot errores collecti; 'primus' (si non NIHIL) primum nuntium
 * accipit - primus enim fere semper causa est, ceteri sequelae. */
i32
manus_errores (
    Manus*  manus,
    chorda* primus);

/* Collecta DELERE - 'his rationem reddidi; a novo incipe'.
 *
 * CUR NECESSARIUM: acervus solum CRESCIT. Sine hoc, primus error
 * CREDO_MANUS_SINE_ERRORIBUS in reliquum cursum INUTILEM facit -
 * omne vocamen sequens eundem errorem VETEREM refert, et error
 * NOVUS ad assertum LX a vetere ad assertum V discerni non potest
 * (manus_errores 'primum' reddit, non 'ultimum').
 *
 * Ergo probatio quae semitam culpae CONSULTO exercet - validatio
 * fallens, iteratio, CDIV exspectatum - omnes custodes post se
 * necaret, et scriptor aut assertum deleret aut discat id
 * neglegere. Utrumque custodem occidit.
 *
 * Tutum si collector abest (nihil agit): 'nullus collector' non est
 * error sed condicio. */
vacuum
manus_errores_purgare (
    Manus* manus);

/* ==================================================
 * Status
 * ================================================== */

/* VERUM post primum fractum. Vocamina sequentia tacent. */
b32
manus_fracta (
    constans Manus* manus);

/* Cur fracta - vacua si integra. Pro nuntio finali suitae. */
chorda
manus_causa (
    constans Manus* manus);

/* ==================================================
 * Actiones (nihil exspectant - actio ipsa synchrona est)
 * ================================================== */

/* JS crudum aestimare, valorem JSON reddere ut CHORDAM (numerus
 * '2', chorda '"salve"' cum virgulis, obiectum '{...}').
 *
 * Fundamentum omnium infra. Publica quia experimentum quod formam
 * novam poscit eam hic invenire debet, non bibliothecam mutare. */
chorda
manus_aestimare (
    Manus*              manus,
    constans character* js,
    Mora                mora);

/* Elementum primum selectoris premere. FALSUM (et manus fracta) si
 * nullum congruit - clic in vacuum vitium est, non nihil. */
b32
manus_premere (
    Manus*              manus,
    constans character* selector);

/* Textum in campum ponere ET eventus 'input'/'change' immittere -
 * sine iis facies reactiva nihil sentit. */
b32
manus_scribere (
    Manus*              manus,
    constans character* selector,
    constans character* textus);

/* Premere id quod TEXTUM hunc fert - non id quod selector nominat.
 *
 *   manus_premere_textum(m, "condere");
 *
 * CUR: 'preme pyxidem quae CONDERE dicit' est quod intendis; '#mittere'
 * est quomodo hodie scriptum est. Probatio in textu fundata mutationem
 * marcationis superest, et legitur ut intentio.
 *
 * Elementum IMUM eligitur quod textum continet: <body> enim omnia
 * continet, et sine hac lege quaeque petitio corpus premeret. */
b32
manus_premere_textum (
    Manus*              manus,
    constans character* textus);

/* Imaginem paginae in plagulam scribere (per imperium/imago).
 * FALSUM si applicatio imaginatorem non praebuit. */
b32
manus_imaginem (
    Manus*              manus,
    constans character* via);

/* Imago AUTOMATICA in hanc plagulam cum manus PRIMUM frangitur;
 * NIHIL eam rursus tacet (tacita ordinarie).
 *
 * CUR: nuntius fracturae dicit quid exspectatum et quid visum sit -
 * non quid SCAENA tunc esset. Probatio quae sine spectatore currit
 * (suita nocturna, machina aliena) illam scaenam aliter perdit in
 * aeternum. Imago in ipso momento capitur, ANTE quam quicquam
 * aliud paginam moveat.
 *
 * Idea a Cypress sumpta. Consulto NON ordinaria: bibliotheca quae
 * plagulas sine rogatu scribit importuna est. */
vacuum
manus_imaginem_culpae_ponere (
    Manus*              manus,
    constans character* via);

/* ==================================================
 * Exploratio (TACITA - nihil notat, nihil exspectat)
 * ==================================================
 *
 * Pro rebus condicionalibus: 'si pyxis aperta est, claude eam'.
 * NON asserta: fractum non pariunt.
 */

b32
manus_existit (
    Manus*              manus,
    constans character* selector);

i32
manus_numerus (
    Manus*              manus,
    constans character* selector);

/* Textus elementi primi: campi valorem, cetera contentum. Vacua si
 * nullum congruit. */
chorda
manus_textus (
    Manus*              manus,
    constans character* selector);

/* ==================================================
 * Asserta (EXSPECTANT, notant, frangunt)
 * ==================================================
 *
 * Forma nuda terminum ordinarium sumit; forma _MORA eum nominat.
 * Cum moram nominas, causam in commentario nomina - mora longa
 * sine causa somnus est alio nomine.
 */

#define CREDO_MANUS_EXISTIT(m, sel) \
	_manus_credo_existere((m), (sel), VERUM, \
			MANUS_MORA_ORDINARIA, __FILE__, __LINE__)

#define CREDO_MANUS_EXISTIT_MORA(m, sel, mora) \
	_manus_credo_existere((m), (sel), VERUM, \
			(mora), __FILE__, __LINE__)

/* Absentiam exspectare - pro re quae ABIRE debet (pyxis clausa,
 * rota volvens finita). */
#define CREDO_MANUS_ABEST(m, sel) \
	_manus_credo_existere((m), (sel), FALSUM, \
			MANUS_MORA_ORDINARIA, __FILE__, __LINE__)

#define CREDO_MANUS_ABEST_MORA(m, sel, mora) \
	_manus_credo_existere((m), (sel), FALSUM, \
			(mora), __FILE__, __LINE__)

#define CREDO_MANUS_TEXTUS(m, sel, exspectatum) \
	_manus_credo_textum((m), (sel), (exspectatum), FALSUM, \
			MANUS_MORA_ORDINARIA, __FILE__, __LINE__)

#define CREDO_MANUS_TEXTUS_MORA(m, sel, exspectatum, mora) \
	_manus_credo_textum((m), (sel), (exspectatum), FALSUM, \
			(mora), __FILE__, __LINE__)

/* Pars sufficit - pro textu qui numerum mutabilem aut tempus
 * continet. */
#define CREDO_MANUS_TEXTUS_CONTINET(m, sel, pars) \
	_manus_credo_textum((m), (sel), (pars), VERUM, \
			MANUS_MORA_ORDINARIA, __FILE__, __LINE__)

#define CREDO_MANUS_TEXTUS_CONTINET_MORA(m, sel, pars, mora) \
	_manus_credo_textum((m), (sel), (pars), VERUM, \
			(mora), __FILE__, __LINE__)

#define CREDO_MANUS_NUMERUS(m, sel, quot) \
	_manus_credo_numerum((m), (sel), (quot), \
			MANUS_MORA_ORDINARIA, __FILE__, __LINE__)

#define CREDO_MANUS_NUMERUS_MORA(m, sel, quot, mora) \
	_manus_credo_numerum((m), (sel), (quot), \
			(mora), __FILE__, __LINE__)

/* Textus USQUAM in pagina visibilis - sine selectore.
 *
 *   CREDO_MANUS_TEXTUM(m, "experimentum conditum est");
 *
 * Quod usor legit, non ubi marcatio id ponit. */
#define CREDO_MANUS_TEXTUM(m, textus) \
	_manus_credo_textum_paginae((m), (textus), VERUM, \
			MANUS_MORA_ORDINARIA, __FILE__, __LINE__)

#define CREDO_MANUS_TEXTUM_MORA(m, textus, mora) \
	_manus_credo_textum_paginae((m), (textus), VERUM, \
			(mora), __FILE__, __LINE__)

#define CREDO_MANUS_TEXTUM_ABEST(m, textus) \
	_manus_credo_textum_paginae((m), (textus), FALSUM, \
			MANUS_MORA_ORDINARIA, __FILE__, __LINE__)

#define CREDO_MANUS_TEXTUM_ABEST_MORA(m, textus, mora) \
	_manus_credo_textum_paginae((m), (textus), FALSUM, \
			(mora), __FILE__, __LINE__)

/* ABEST vs ABEST_OMNINO - distinctio quam Cypress inter
 * 'not.be.visible' et 'not.exist' servat.
 *
 * ABEST         = usor non videt (occultum SUFFICIT).
 * ABEST_OMNINO  = in DOM omnino non est.
 *
 * Utrumque opus est: prius quaerit num scaena abierit; posterius
 * num applicatio nodos occultos in aeternum congerat - quod primum
 * numquam videret. */
#define CREDO_MANUS_ABEST_OMNINO(m, sel) \
	_manus_credo_omnino((m), (sel), \
			MANUS_MORA_ORDINARIA, __FILE__, __LINE__)

#define CREDO_MANUS_ABEST_OMNINO_MORA(m, sel, mora) \
	_manus_credo_omnino((m), (sel), \
			(mora), __FILE__, __LINE__)

/* Nullus error in pagina ex quo manus aperta est. Vide 'Errores
 * paginae' supra: assertum ultimum eorum quae probatio de se ipsa
 * scire debet. */
#define CREDO_MANUS_SINE_ERRORIBUS(m) \
	_manus_credo_sine_erroribus((m), __FILE__, __LINE__)

/* ==================================================
 * Interiora macronum (ne voces recta - __FILE__ perderes)
 * ================================================== */

b32
_manus_credo_existere (
    Manus*              manus,
    constans character* selector,
    b32                 adesse,
    Mora                mora,
    constans character* filum,
    s32                 versus);

b32
_manus_credo_textum (
    Manus*              manus,
    constans character* selector,
    constans character* exspectatum,
    b32                 pars_sufficit,
    Mora                mora,
    constans character* filum,
    s32                 versus);

b32
_manus_credo_numerum (
    Manus*              manus,
    constans character* selector,
    i32                 quot,
    Mora                mora,
    constans character* filum,
    s32                 versus);

b32
_manus_credo_textum_paginae (
    Manus*              manus,
    constans character* textus,
    b32                 adesse,
    Mora                mora,
    constans character* filum,
    s32                 versus);

b32
_manus_credo_omnino (
    Manus*              manus,
    constans character* selector,
    Mora                mora,
    constans character* filum,
    s32                 versus);

b32
_manus_credo_sine_erroribus (
    Manus*              manus,
    constans character* filum,
    s32                 versus);

#endif /* MANUS_H */
