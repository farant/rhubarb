/* fasciculum.h - Fasciculi macOS (.app): struere, legere, conferre.
 *
 * Fasciculus est directorium formae fixae: Foo.app/Contents/ cum
 * Info.plist, exsecutabili sub MacOS/, et rebus optionalibus. Haec
 * bibliotheca eum ut VALOREM describit, valorem in indicem plagularum
 * PURE reddit, indicem scribit, fasciculum exsistentem relegit, duos
 * confert.
 *
 * Info.plist dictio PlistValor est quam plist_scribere serializat -
 * hic nihil de XML scribitur. Specificatio:
 * project-specs/fasciculum-spec.md v1.
 *
 * NON tractat: subscriptionem, notarizationem, Launch Services,
 * .framework / .bundle / .appex, generationem iconum (id icones erit).
 */

#ifndef FASCICULUM_H
#define FASCICULUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "plist.h"
#include "xar.h"

nomen enumeratio {
    FASCICULUM_SUCCESSUS = ZEPHYRUM,
    FASCICULUM_ERROR_DESUNT,
    FASCICULUM_ERROR_EXSECUTABILE,
    FASCICULUM_ERROR_ICON,
    FASCICULUM_ERROR_PLISTA,
    FASCICULUM_ERROR_NON_FASCICULUM,
    FASCICULUM_ERROR_DIRECTORIUM,
    FASCICULUM_ERROR_SCRIPTIO,
    FASCICULUM_ERROR_MODUS,
    FASCICULUM_ERROR_MEMORIA,
    FASCICULUM_ERROR_IDENTITAS  /* characteres extra A-Z a-z 0-9 . - */
} FasciculumStatus;

nomen enumeratio {
    FASCICULUM_GENITUM,
    FASCICULUM_COPIATUM
} FasciculumOrigo;

/* Pars plani: aut OCTETI geniti, aut VIA fontis copiandi (par. III
 * specificationis: ergo reddere purum manet et binarium magnum per
 * piscinam non transit). */
nomen structura {
    FasciculumOrigo  origo;
             chorda  semita;   /* relativa: "Contents/Info.plist" */
             chorda  octeti;   /* GENITUM */
 constans character* fons;     /* COPIATUM */
                b32  exsecutabile_fiat;
} FasciculumPars;

nomen structura {
             chorda  identitas;  /* CFBundleIdentifier, necessarium */
             chorda  titulus;    /* CFBundleName */
             chorda  versio;     /* CFBundleShortVersionString */
             chorda  versio_aedificationis;  /* CFBundleVersion */
 constans character* exsecutabile;  /* via, necessarium */
 constans character* icon;          /* via ad .icns, aut NIHIL */
                b32  sine_scandali; /* LSUIElement */
         PlistValor* plista_extra;  /* dictio fusa, aut NIHIL */
} Fasciculum;

/* Reddere planum: PURUM (nullum contentum de disco legit; vias fontium
 * solum EXSISTERE probat). partes = Xar de FasciculumPars.
 *
 * Redde FALSUM cum statu et sede vitii. */
b32
fasciculum_reddere (
    constans Fasciculum*  f,
                    Xar** partes,
       FasciculumStatus*  status,
                 chorda*  sedes_vitii,
                Piscina*  piscina);

/* CFBundleIdentifier: litterae, cifrae, '.' et '-' SOLAE (Apple).
 * Vacua VERUM reddit - absentia DESUNT est, non characteres pravi.
 * Publica ut briar eandem regulam UNO loco adhibeat. */
b32
fasciculum_identitas_valida (
    chorda identitas);

/* Scribere planum sub radice data: directoria, plagulae genitae,
 * copiae, modus +x. SINE REVERSIONE: quod scriptum est manet, et
 * status semitam deficientem nominat (par. V). */
b32
fasciculum_scribere (
           constans Xar* partes,
     constans character* via_radicis,
       FasciculumStatus* status,
                 chorda* sedes_vitii,
                Piscina* piscina);

/* Legere fasciculum exsistentem. RECUSAT primum vitium nominans
 * (par. IV, VI); claves ignotae in plista_extra manent. */
b32
fasciculum_legere (
     constans character* via_radicis,
             Fasciculum* f,
       FasciculumStatus* status,
                 chorda* sedes_vitii,
                Piscina* piscina,
    InternamentumChorda* intern);

/* Aequalitas VALORUM (super plist_aequalis): ut scriptor fasciculum
 * iam rectum non rescribat. */
b32
fasciculum_aequalis (
    constans Fasciculum* a,
    constans Fasciculum* b);

#endif /* FASCICULUM_H */
