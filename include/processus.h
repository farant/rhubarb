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
 * DUAE SEMITAE, ANSA UNA: processus_exsequi colligit et EXSPECTAT
 * (simplex, pro instrumentis conchae et probationibus). Semita
 * INCREMENTALIS (incipere/pulsare/metere, infra) idem opus per
 * tictus spargit, pro faciebus quae obstare non possunt.
 *
 * AMBAE corpus ansae IDEM adhibent (_ansam_pulsare interna) -
 * consulto, ne duo exemplaria ansae select() subtilis per tempus
 * dissideant. Quod hic emendatur, utrique semitae emendatur.
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

/* Processus IPSE fit programma nominatum (execvp - eadem PID,
 * descriptores aperti manent). Pro residentibus qui se super
 * fistulam vivam renovant: responde, effunde (fflush), deinde fi.
 *
 * CAVETO: quod in buffris FILE* userspace iacet perit - octeti in
 * fistula NUCLEI supersunt, octeti iam in buffrum stdio lecti non.
 * Residens qui hoc vocat introitum sine bufferis legat (_IONBF).
 *
 * Redde: numquam si felix; FALSUM si exec fefellit (vocator
 * pergat - defectus non fatalis). */
b32
processus_transformare(
    constans character* constans* argumenta);


/* ========================================================================
 * SEMITA INCREMENTALIS - pro faciebus quae obstare non possunt
 * ========================================================================
 *
 * CUR EXSISTIT: domus filis CARET (nullum pthread in lib/; reactor
 * ansam suam possidet; actor/cursus concurrentia non sunt). Ergo
 * facies vitreae filum UNUM habet, et processus_exsequi cum
 * 'ConnectTimeout=5' fenestram per quinque secunda CONGELARET si in
 * tictu vocaretur. Movere vocationem ex tractatore internuntii in
 * tictum NIHIL sanat - tictus aeque obstruitur.
 *
 * USUS (in tictu applicationis):
 *   si (processus == NIHIL && opus_est)
 *   {
 *       processus = processus_incipere(argumenta, 5000, piscina);
 *   }
 *   si (processus != NIHIL
 *       && processus_pulsare(processus) == PROCESSUS_PARATUS)
 *   {
 *       ProcessusResultus r = processus_metere(processus);
 *       ... r tractare ...
 *       processus = NIHIL;
 *   }
 *
 * Pulsus UNUS select() vocationem cum mora ZEPHYRI facit: quod
 * paratum est haurit, quod non, relinquit. Numquam obstat.
 */

nomen structura Processus Processus;

nomen enumeratio {
    PROCESSUS_CURRIT = 0,   /* nondum perfectus - pulsa iterum */
    PROCESSUS_PARATUS       /* perfectus (aut fractus) - mete */
} ProcessusStatus;

/* Processum generare et STATIM redire (nulla exspectatio).
 *
 * Redde: manubrium, aut NIHIL si piscina NIHIL est aut manubrium
 * allocari non potuit. Errores ALII (argumenta prava, furca fracta,
 * fistulae fractae) manubrium VALIDUM reddunt quod statim PARATUS
 * est et cuius processus_metere resultum erroris rectum fert -
 * ergo vocator unam semitam tractat, non duas.
 *
 * mora_maxima_ms: idem quod in processus_exsequi - terminus DURUS
 * ab hoc momento. ZEPHYRUM = infinitus. Terminus in pulsare
 * custoditur, ergo excessus processum occidit etiam si vocator
 * tarde pulsat.
 */
Processus*
processus_incipere(
    constans character* constans* argumenta,
    i32                          mora_maxima_ms,
    Piscina*                     piscina);

/* Pulsus unus: quod paratum est haurire, terminum custodire, statum
 * referre. NUMQUAM OBSTAT (select cum mora zephyri).
 *
 * PARATUS redditur solum cum fistulae clausae sunt ET infans
 * messus est (waitpid WNOHANG). Infans qui fistulas claudit sed
 * currere pergit CURRIT manet - recte, quia nondum finivit.
 */
ProcessusStatus
processus_pulsare(Processus* processus);

/* Effusio HACTENUS capta, sine expectatione - quod pulsare iam
 * hausit. Non destruit: iterum vocari potest.
 *
 * PRO QUO: probatio integrans quae effusionem prolis CURRENTIS
 * inspicere debet (e.g. 'console.log paginae in stdout applicationis
 * apparuitne?'). Sine hoc effusio ante processus_metere - id est
 * ante EXITUM - inaccessibilis erat, et applicatio quae currere
 * debet numquam meteretur.
 *
 * CAVE: chorda in sacculum ipsum spectat, qui pulsatione proxima
 * RELOCARI potest. Tracta statim; noli recondere trans pulsationem.
 */
chorda
processus_effusio_hactenus (
    constans Processus* processus);

chorda
processus_erratum_hactenus (
    constans Processus* processus);

/* Resultus post PARATUM. Si vocatur dum adhuc currit, OBSTAT donec
 * perficiatur (terminus adhuc valet) - commoditas pro vocatore qui
 * pulsare desiit, non semita ordinaria.
 *
 * Idempotens: iterum vocata idem resultum reddit.
 */
ProcessusResultus
processus_metere(Processus* processus);

/* Occidere et metere (SIGKILL). Pro pyxide 'abrumpe' faciei.
 * Post hoc processus_metere resultum cum PROCESSUS_ERROR_TEMPUS
 * fert. Tuta si iam perfectus (nihil agit).
 */
vacuum
processus_abrumpere(Processus* processus);

#endif /* PROCESSUS_H */
