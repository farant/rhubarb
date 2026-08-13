/*
 * specimen.h - Regressio visualis: captura contra EXEMPLAR servatum
 *
 * Specimen est id quo res cognoscitur - imago servata cui capturae
 * posterae comparantur.
 *
 * ==========================================================
 * QUOD SCIENDUM EST ANTE USUM
 * ==========================================================
 *
 * Probatio visualis omnium probationum a MACHINA maxime pendet:
 * versio systematis, factor scalae schirmatis (machina Retina
 * imagines DUPLAS parit), litterarum formae, versio rasterizatoris.
 * MENSURATUM 2026-08-13: textus motu 0.3 puncti solo CCXI pixela
 * diversa post spretionem lenimenti relinquit.
 *
 * Ergo periculum verum non est "aedificari non potest" sed
 * "probatio quae semper clamat, quam omnes mox neglegunt" - quae
 * peior est quam nulla, quia fiduciam simulat.
 *
 * Duo consilia id minuunt, et AMBO hic infixa sunt:
 *   I.  ELEMENTUM, non fenestra tota. Area minor = strepitus minor,
 *       et mutatio aliena in pagina probationem tuam non frangit.
 *   II. EXEMPLAR ABSENS FRANGIT. Probatio quae exemplar suum in
 *       primo cursu tacite creat deinde IN PERPETUUM praeterit sine
 *       ulla collatione facta - id est machina viriditatis mendacis.
 *
 * ==========================================================
 * RITUS ACCEPTIONIS (nullum instrumentum, nulla vexilla)
 * ==========================================================
 *
 *   exemplar:     <scrinium>/<titulus>.png            (in git)
 *   candidatus:   <scrinium>/<titulus>.novum.png      (in fractura)
 *   differentia:  <scrinium>/<titulus>.differentia.png
 *
 * Fractura candidatum ET picturam differentiae scribit. Tu eas
 * OCULIS inspicis. Si mutatio recta est, candidatum super exemplar
 * MOVES:
 *
 *     mv scrinium/salve.novum.png scrinium/salve.png
 *
 * Nihil aliud. Acceptio motus plagulae est - actus deliberatus quem
 * git videt, non vexillum quod omnia simul benedicit. Plagulae
 * .novum/.differentia CONSULTO non ignorantur: dum manent, `git
 * status` tibi indicem fracturarum inspiciendarum tenet.
 *
 * SCRINIUM IPSUM NON CREATUR. Si abest, specimen recusat et causam
 * nominat - scrinium absens fere semper configurationem falsam
 * significat, et ubi exemplaria habitent DELIBERANDUM est.
 */

#ifndef SPECIMEN_H
#define SPECIMEN_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "imago_typus.h"
#include "imago_collatio.h"
#include "manus.h"

/* ============================================================
 * Typi
 * ============================================================ */

/*
 * SpecimenSententia - Quid acciderit
 *
 * MENSURAE_DISCREPANT sua sententia est CONSULTO: fere semper
 * significat machinam aut scalam schirmatis mutatam esse, NON
 * regressionem. Sub uno nomine cum DISCREPAT posita, horas
 * quaerendi mutationem quae nulla est costaret.
 */
nomen enumeratio {
    SPECIMEN_CONGRUIT = 0,          /* captura exemplari respondet */
    SPECIMEN_DISCREPAT,             /* mutatio vera (aut strepitus) */
    SPECIMEN_EXEMPLAR_ABEST,        /* nullum exemplar - FRACTURA */
    SPECIMEN_MENSURAE_DISCREPANT,   /* alia machina/scala, non regressio */
    SPECIMEN_ELEMENTUM_ABEST,       /* selector nihil visibile invenit */
    SPECIMEN_CAPTURA_FRACTA,        /* imago capi non potuit */
    SPECIMEN_SCRINIUM_INACCESSUM,   /* scrinium abest AUT scribi nequit
                                     * (sine POSIX ea discerni non
                                     * possunt - ergo nomen utrumque
                                     * fatetur potius quam alterum
                                     * praesumat) */
    SPECIMEN_TITULUS_INVALIDUS,     /* titulus semitam continet */
    SPECIMEN_ARGUMENTA_INVALIDA
} SpecimenSententia;

/*
 * SpecimenRegula
 *
 * @scrinium: directorium exemplarium (NON creatur si abest)
 * @collatio: quid differentiam constituat - vide imago_collatio.h.
 *            pixela_ferenda HIC ponitur, quia quantum strepitus
 *            ferendum sit ex probatione ipsa pendet.
 */
nomen structura {
    constans character* scrinium;
    CollatioRegula      collatio;
} SpecimenRegula;

/*
 * SpecimenFructus
 *
 * @collatio: fructus collationis, si collatio facta est. Mensura
 *            INTACTA manet etiam cum sententia mitis est.
 * @viae:     quae plagulae scriptae sint (vacuae si non scriptae)
 * @causa:    nuntius humanus - numquam vacuus cum sententia mala
 */
nomen structura {
    SpecimenSententia sententia;
    CollatioFructus   collatio;
    b32               collatio_facta;
    chorda            via_exemplaris;
    chorda            via_candidati;
    chorda            via_differentiae;
    chorda            causa;
} SpecimenFructus;

/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * specimen_regula_solita - Regula usitata
 *
 * @scrinium: ubi exemplaria habitent (necessarium)
 *
 * collatio = collatio_regula_solita() cum pictura PETITA (pictura
 * enim tota causa est cur fractura utilis sit).
 */
SpecimenRegula
specimen_regula_solita (constans character* scrinium);

/*
 * specimen_iudicare - Capturam contra exemplar servatum iudicare
 *
 * PURA quantum fieri potest: applicationem non poscit, fenestram
 * non aperit. Imago intrat, sententia exit (et plagulae in
 * fractura). Ideo probari potest imaginibus fictis solis.
 *
 * @captura: imago iam capta
 * @titulus: nomen exemplaris (SINE semita, SINE suffixu)
 */
SpecimenFructus
specimen_iudicare (constans Imago*     captura,
                   constans character* titulus,
                   SpecimenRegula      regula,
                   Piscina*            piscina);

/*
 * specimen_manus - Capere, secare, iudicare
 *
 * @selector: elementum secandum. NIHIL = fenestra tota.
 *
 * SCALA: capturae in machinis Retina pixela DUPLA habent dum
 * getBoundingClientRect pixela CSS reddit. Scala ex latitudine
 * imaginis contra innerWidth DERIVATUR, non ex devicePixelRatio
 * praesumpta - ita casus mixti recte cadunt.
 *
 * Manus fracta manet fracta: si haec vocatio fallit, sequentes
 * tacent donec manus_reficere vocetur (vide manus.h).
 */
SpecimenFructus
specimen_manus (Manus*              manus,
                constans character* titulus,
                constans character* selector,
                SpecimenRegula      regula,
                Piscina*            piscina);

/*
 * specimen_sententia_nomen - Nomen sententiae pro nuntiis
 */
constans character*
specimen_sententia_nomen (SpecimenSententia sententia);

/* ============================================================
 * Credo
 * ============================================================ */

/*
 * CREDO_SPECIMEN - Elementum exemplari suo respondere
 *
 * Fractura viam picturae differentiae NOMINAT, ut eam statim
 * aperire possis.
 */
#define CREDO_SPECIMEN(m, titulus, selector, regula, piscina) \
    do { \
        SpecimenFructus _sf = specimen_manus((m), (titulus), \
                                  (selector), (regula), (piscina)); \
        _credo_notare("credo_specimen", (titulus), \
            specimen_sententia_nomen(_sf.sententia), \
            "SPECIMEN_CONGRUIT", \
            __FILE__, __LINE__, \
            _sf.sententia == SPECIMEN_CONGRUIT); \
    } dum (0)

#endif /* SPECIMEN_H */
