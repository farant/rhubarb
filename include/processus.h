/* processus.h - Bibliotheca generationis processuum (captura + terminus)
 *
 * API platform-agnostica pro processibus filiatis: exsecutio,
 * captura effusionis et errati SEPARATIM, codex exitus, terminus
 * temporis durus.
 *
 * NULLA CONCHA. Argumenta ut vector traduntur (execvp), non ut
 * chorda quam /bin/sh interpretetur - ergo citatio conchae non
 * exsistit et classis defectuum citationis omnino DELETA est.
 * Hoc momenti est ubicumque usor textum liberum scribit quod
 * imperium fit (exempli gratia manus libera villae).
 *
 * Implementatio: processus_posix.c (macOS, Linux)
 *
 * USUS:
 *   constans character* argumenta[] = {
 *       "ssh", "-o", "BatchMode=yes", "usor@hospes", "uptime", NIHIL };
 *   ProcessusResultus r = processus_exsequi(argumenta, 5000, piscina);
 *   si (r.successus && r.codex_exitus == 0)
 *   {
 *       ... r.effusio ...            (r.erratum = diagnostica)
 *   }
 *
 * NOTA DE FLUENTE: haec functio colligit et EXSPECTAT. Lectio
 * incrementalis (diaria sequentia) non adest consulto - ansa
 * interna iam est ansa fluentis cum revocatione omissa, ergo
 * additio postera erit ingressus novus super eadem interna, non
 * rescriptio.
 */

#ifndef PROCESSUS_H
#define PROCESSUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

/* ========================================================================
 * TYPI
 * ======================================================================== */

nomen enumeratio {
    PROCESSUS_OK = 0,
    PROCESSUS_ERROR_ARGUMENTA,   /* vector vacuus aut NIHIL */
    PROCESSUS_ERROR_GENERARE,    /* pipe/fork fallita */
    PROCESSUS_ERROR_EXEC,        /* binarium non inventum aut non
                                  * exsecutabile - DISTINCTUM a codice
                                  * exitus 127 processus veri (ssh 127
                                  * = imperium REMOTUM deest) */
    PROCESSUS_ERROR_TEMPUS,      /* mora excessa - processus occisus */
    PROCESSUS_ERROR_IO           /* lectio/exspectatio fracta */
} ProcessusError;

nomen structura {
    b32            successus;      /* generatus ET exspectatus.
                                    * NOTA: codex_exitus non-zerus
                                    * successus MANET - processus
                                    * cucurrit et respondit. */
    i32            codex_exitus;   /* WEXITSTATUS (0 nisi signo) */
    i32            signum;         /* WTERMSIG, aut 0 */
    chorda         effusio;        /* stdout captus */
    chorda         erratum;        /* stderr captus - SEPARATIM */
    i32            mora_ms;        /* tempus elapsum */
    ProcessusError error;
    chorda         error_descriptio;
} ProcessusResultus;

/* ========================================================================
 * FUNCTIONES
 * ======================================================================== */

/* Processum generare, exspectare, effusionem capere.
 *
 * argumenta:       vector NIHIL-terminatus; argumenta[0] = via ad
 *                  binarium (semita quaesita ut execvp facit).
 *                  NULLA concha adhibetur - spatia, virgulae,
 *                  lineae novae in elementis TUTA sunt.
 * mora_maxima_ms:  terminus durus; 0 = infinitus. Excessa =
 *                  SIGKILL + PROCESSUS_ERROR_TEMPUS (effusio
 *                  partialis usque ad id punctum servatur).
 * piscina:         arena pro effusione/errato/descriptione.
 *
 * Redde: resultus. successus = FALSUM significat processum non
 * cucurrisse aut non recte exspectatum esse - NON eum fefellisse
 * (id est codex_exitus).
 */
ProcessusResultus
processus_exsequi(
    constans character* constans* argumenta,
    i32                          mora_maxima_ms,
    Piscina*                     piscina);

/* Descriptio erroris ut litterae staticae (pro nuntiis). */
constans character*
processus_error_nomen(ProcessusError error);

#endif /* PROCESSUS_H */
