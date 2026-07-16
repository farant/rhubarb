/* vigilia.h - vigilia processus residentis: binarium currens
 * contra mundum externum
 *
 * Generalizata 2026-07-16 ex duabus formis manu-volutis (tabularium
 * K2.1 vigilia disci; legatus vigilia fontium) - semen
 * lib/mcp_servus futuri. Scala staleness, ordo tertius: fontes ->
 * obiecta -> binarium (excubitor/fabrica) + BINARIUM -> PROCESSUS
 * (haec bibliotheca).
 *
 * DUAE VIGILIAE, UNUM CONTRACTUM:
 *   - vigilia DISCI (post-reaedificationem): binarium in disco
 *     mutatum sub processu currente - mtime/magnitudo mota ->
 *     sigillum disci confirmat (reaedificatio identica renovat
 *     tacite, non monet). Postulat signum + via_binarii.
 *   - vigilia FONTIUM (prae-reaedificationem): fons quilibet
 *     manifesti recentior binario currente. Postulat via_binarii
 *     (momentum ortus) + via_manifesti.
 *
 * INVARIANS MANIFESTI: aedificator (launcher) manifestum scribit -
 * indicem VERUM eorum quae compilaret. Vigilia ergo monet exacte
 * cum reconnect binarium aliud daret; vigil et aedificator
 * dissentire non possunt (BFS graphi inclusionum in residente
 * supervacuus fit).
 *
 * Forma manifesti: via una per lineam; lineae vacuae et '#'
 * praetermittuntur. Viae relativae radici currendi (launcher ex
 * radice exsequitur).
 *
 * MTEMPORA NANOSECUNDIS (parcum 'Mtimes nanosecundorum' hic
 * solutum pro hac scala): fenestra eiusdem-secundi clausa - fons
 * post ortum scriptus etiam intra secundum detegitur (APFS).
 *
 * Status GLUTINOSUS: semel non-recens, numquam retro; FONTES ->
 * BINARIUM promotio licet (reaedificatio nuntium meliorem dat).
 * Configuratio manca = vigilia quieta (numquam impedit, numquam
 * fallit).
 *
 * SUTURA POSIX (stat/sys-stat.h) - macOS ex decreto. */

#ifndef VIGILIA_H
#define VIGILIA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen enumeratio {
    VIGILIA_RECENS = 0,
    VIGILIA_FONTES_SUPERANT,   /* reaedificatio + reconnect */
    VIGILIA_BINARIUM_NOVIUS    /* reconnect solus sufficit */
} VigiliaStatus;

nomen structura {
    constans character* signum;         /* sigillum hex binarii ad
                                         * ortum (launcher computat -
                                         * binarium se ipsum non
                                         * inspicit); NIHIL = sine
                                         * identitate, vigilia disci
                                         * quieta */
    constans character* via_binarii;    /* NIHIL = ambae quietae */
    constans character* via_manifesti;  /* NIHIL = vigilia fontium
                                         * quieta */
} VigiliaConfiguratio;

nomen structura Vigilia Vigilia;

/* NIHIL solum piscina deficiente; cfg NIHIL aut campi NIHIL =
 * obiectum quietum validum */
Vigilia*
vigilia_creare (
    Piscina*                      piscina,
    constans VigiliaConfiguratio* cfg);

/* per petitionem vocanda (stat unum per plagulam vigilatam -
 * vile); status novum reddit. effimera = piscina petitionis
 * (lectio binarii ad confirmationem sigilli) */
VigiliaStatus
vigilia_inspicere (
    Vigilia* vigilia,
    Piscina* effimera);

VigiliaStatus
vigilia_status (
    constans Vigilia* vigilia);

/* signum breve (XVI hex + NUL); "" si sine identitate */
constans character*
vigilia_signum_breve (
    constans Vigilia* vigilia);

/* textus cautionis paratus (appende responsis); NIHIL si recens */
constans character*
vigilia_cautio (
    constans Vigilia* vigilia);

/* via culpae (fons superans aut via binarii); "" si recens */
constans character*
vigilia_causa (
    constans Vigilia* vigilia);

/* via in manifesto? (congruentia suffixi cum limite '/' - viae
 * manifesti absolutae, quaestiones relativae) */
b32
vigilia_continet (
    constans Vigilia* vigilia,
    chorda            via);

#endif /* VIGILIA_H */
