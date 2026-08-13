#ifndef IMPERIUM_H
#define IMPERIUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "hospitium.h"
#include "internuntius.h"

/* imperium.h - vitream ab EXTRA regere (modus-dev SOLUS)
 *
 * PROBLEMA: vitrea_aestimare JS in fenestram vivam mittit sed NIHIL
 * REDDIT. Ergo agens applicationem agitare potest, interrogare non
 * potest - et probatio quae interrogare nequit probatio non est.
 *
 * SOLUTIO: ansa duplex super duas res quae IAM ADSUNT et in EODEM
 * gyro pulsantur - hospitium (foris intus) et internuntius (intus
 * foris). Iussum per HTTP intrat, per aestimatorem in paginam it,
 * per pontem redit.
 *
 *   POST /imperium          corpus = JS crudum -> {"tessera": N}
 *   GET  /imperium/N        -> {"status":"pendens"}
 *                              {"status":"perfectum","valor":...}
 *                              {"status":"culpa","nuntius":...}
 *   GET  /imperium/species  -> {"fenestra": ID}  (pro screencapture)
 *
 * CUR TESSERA ET NON RESPONSUM UNUM: hospitium filo UNO currit et
 * tractator cito redire DEBET; petitionem apertam tenere gyrum
 * ipsum necaret qui responsum ferre debet. Ergo duo itinera -
 * agenti nihil constat, applicationi vita.
 *
 * PROMISSA TRACTANTUR: si JS promissum reddit, imperium id
 * exspectat et valorem solutum reddit. Sine hoc quaeque operatio
 * interfaciei (quae fere semper async est) 'null' redderet -
 * defectus qui probationes VIRIDES et MENDACES pareret.
 *
 * PERICULUM - MODUS DEV SOLUS: haec ansa EXSECUTIONEM CODICIS
 * ARBITRARII in applicatione praebet. Numquam in capsula vecta
 * aperienda; hospitium ipsum 127.0.0.1 ligat, et applicatio
 * imperium sub vexillo -vivum solo praebeat.
 *
 * USUS (tres res, unum vocamen):
 *   imp = imperium_creare(piscina, vitrea_aestimator, vitrea);
 *   imperium_fenestram_ponere(imp, id_fenestrae_nativae);
 *   imperium_praebere(imp, hospitium, inx);
 */

nomen structura Imperium Imperium;

/* seam-formis: eadem forma qua vitrea_aestimator (JS crudum) */
nomen vacuum (*ImperiumAestimator)(
    vacuum* datum,
    chorda  js);

/* IMAGO - suturae geminae (formae vitrea_imaginator pares).
 *
 * Imperium typum Vitrea IGNORAT consulto: suturae solae eum
 * ligant, unde probatio sine fenestra fieri potest (probatio
 * huius bibliothecae id facit). */
nomen vacuum (*ImperiumImagoFacta)(
    vacuum* datum,
    b32     successus);

nomen b32 (*ImperiumImaginator)(
    vacuum*             datum,
    constans character* via,
    ImperiumImagoFacta  facta,
    vacuum*             facta_datum);

/* ==================================================
 * Cyclus vitae
 * ================================================== */

Imperium*
imperium_creare (
    Piscina*           piscina,
    ImperiumAestimator aestimator,
    vacuum*            datum);

/* Vias HTTP et methodum pontis 'imperium.responsum' registrare.
 * FALSUM si registratio fallit. */
b32
imperium_praebere (
    Imperium*     imperium,
    Hospitium*    hospitium,
    Internuntius* internuntius);

/* Id fenestrae nativae (CGWindowID in macOS) ut instrumentum
 * exterius SOLAM hanc fenestram capere possit, non scrinium.
 * Cadens est: imaginator (infra) melior via est. */
vacuum
imperium_fenestram_ponere (
    Imperium* imperium,
    i32       id_fenestrae);

/* Imaginatorem ponere -> viam 'POST /imperium/imago' aperit.
 * Sine eo via CDIV reddit (recusatio APERTA, non imago vacua).
 *
 *   imperium_imaginatorem_ponere(imp, vitrea_imaginator, vitrea);
 *
 * Petitio tesseram reddit ut iussa cetera; imago scripta tesseram
 * cum VIA plagulae perficit. Asynchrona enim est, et tesserae iam
 * aderant - nihil novi machinamenti opus fuit. */
vacuum
imperium_imaginatorem_ponere (
    Imperium*          imperium,
    ImperiumImaginator imaginator,
    vacuum*            datum);

/* ==================================================
 * Numeratores (mensura est productum)
 * ================================================== */

nomen structura {
    i64 iussa_missa;
    i64 responsa_recepta;
    i64 culpae;
    i64 pendentia;
} ImperiumFructus;

ImperiumFructus
imperium_fructus (
    constans Imperium* imperium);

#endif /* IMPERIUM_H */
