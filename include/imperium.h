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

/* CLAVIS - sutura (forma fenestra_claviarius par).
 *
 * NOMEN SOLUM transit ("Enter", "Cmd+Shift+ArrowLeft"): codices
 * virtuales et vexilla modificatorum res PLATFORMAE sunt, ergo
 * imperium ea nescit et nescire debet. Tabula in fenestra_macos.m
 * vivit, ubi sola valet.
 *
 * Redde FALSUM si nomen ignotum - unde via CD reddit, non CCII
 * mendax. */
nomen b32 (*ImperiumClaviarius)(
    vacuum*             datum,
    constans character* clavis);

/* ==========================================================
 * VIVARIUM - imperium PARATUM (baculi inclusi)
 * ==========================================================
 *
 * Caput hoc supra dicit imperium super "duas res quae IAM ADSUNT"
 * stare - hospitium et internuntium. Verum est pro applicatione
 * quae RPC verum agit (forum, villa). FALSUM pro spectatore qui
 * solum legit: mensor_ui neutrum habebat, ergo servitorem HTTP et
 * pontem RPC creare debuit UT SOLUM canalem aestimandi haberet.
 *
 * Vivarium eam differentiam absorbet. Applicatio quae sua habet ea
 * TRADIT; quae non habet ea INTUS creata accipit.
 *
 * CUR NON IN VITREA: vitrea.h finem PERPETUUM suum declarat -
 * "adhaesio texturae + origo assetorum + transportus chordarum, IN
 * AETERNUM". Servitor HTTP post vexillum vitreae eum limitem
 * frangeret, et OMNIS applicatio vitreae hospitium ligaret, etiam
 * quae nihil servit.
 *
 * CUR HIC ET NON ALIBI: imperium hospitium et internuntium IAM
 * includit, ergo nihil novi trahit. Suturae (aestimator, imaginator,
 * missor) omnes vitrea-liberae sunt - ergo vivarium vitream non
 * novit, sicut imperium eam numquam novit.
 *
 * USUS (applicatio vitreae typica):
 *   VivariumConfiguratio f;
 *   memset(&f, 0, magnitudo(f));
 *   f.portus     = ZEPHYRUM;              (auto)
 *   f.aestimator = vitrea_aestimator;
 *   f.imaginator = vitrea_imaginator;
 *   f.missor     = vitrea_missor;
 *   f.datum      = vitrea;
 *   f.fenestra   = fenestra_numerus_nativus(fenestra);
 *   Vivarium v = imperium_vivarium(piscina, &f);
 *   si (!v.successus) { ... v.causa ... }
 *   ... in gyro: vivarium_gressus(&v);
 *
 * MODUS EVOLUTIONIS SOLUS: JS arbitrarium exsequitur. Post vexillum
 * (-vivum) sedeat, 127.0.0.1 solum, in capsula tradita NUMQUAM.
 */

nomen structura {
    i32                portus;       /* ZEPHYRUM = auto-selectus */
    ImperiumAestimator aestimator;   /* OBLIGATORIUS */
    ImperiumImaginator imaginator;   /* NIHIL = sine /imperium/imago */
    ImperiumClaviarius claviarius;   /* NIHIL = sine /imperium/clavis */
    /* Claviarius FENESTRAM poscit, ceterae suturae VITREAM - ergo
     * datum proprium. Unum 'datum' pro utroque ruinam silentem
     * daret (typus alius, compilator tacet). */
    vacuum*            claviarius_datum;
    InternuntiusMissor missor;       /* opus nisi internuntius datur */
    vacuum*            datum;        /* idem omnibus suturis */
    i32                fenestra;     /* ID nativum (ZEPHYRUM = nullum) */
    Hospitium*         hospitium;    /* NIHIL = intus creare */
    /* NIHIL = intus creare. Datus esse potest, sed RECENS esse
     * debet: imperium methodum 'imperium.responsum' in ponte
     * registrat, et registratio gemina fallit - ergo pons UNUS
     * imperium UNUM fert. Pontem iam ligatum tradere vivarium
     * RECUSARE facit (probatum). */
    Internuntius*      internuntius;
} VivariumConfiguratio;

/*
 * @portus:  portus VERUS (auto-selectus resolutus) - nuntiandus
 * @propria: an hospitium intus creatum sit (an destruendum tibi)
 * @causa:   NOMINAT quid defecerit; numquam vacua cum !successus
 */
nomen structura {
    b32           successus;
    Hospitium*    hospitium;
    Internuntius* internuntius;
    Imperium*     imperium;
    i32           portus;
    b32           propria;
    chorda        causa;
} Vivarium;

/*
 * imperium_vivarium - Omnia uno vocamine ligare
 *
 * Ordo ligandi intus tenetur (praebere ANTE fenestram ponere), ne
 * quaeque applicatio eum ex documentis rursus deducat.
 */
Vivarium
imperium_vivarium (Piscina* piscina,
                   constans VivariumConfiguratio* figura);

/*
 * vivarium_gressus - Gradus unus in gyro applicationis
 *
 * Numquam obstat. In eodem gyro quo pons exhauritur vocandus -
 * ansa duplex aliter dimidia manet.
 */
vacuum
vivarium_gressus (constans Vivarium* vivarium);

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

/* Claviarium ponere -> viam 'POST /imperium/clavis' aperit.
 * Sine eo via CDIV reddit (recusatio APERTA).
 *
 *   imperium_claviarium_ponere(imp, fenestra_claviarius, fenestra);
 *
 * Corpus petitionis = NOMEN clavis ("Enter", "Cmd+ArrowLeft").
 * SYNCHRONA, sine tessera: eventus in caudam ponitur et vocatio
 * statim redit - nihil est quod exspectetur, ergo tessera
 * machinamentum sine causa esset. Nomen ignotum CD reddit. */
vacuum
imperium_claviarium_ponere (
    Imperium*          imperium,
    ImperiumClaviarius claviarius,
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
