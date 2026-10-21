/* canalis.h - canalis ad servum currentem, identitate systematis
 * operandi probatus (canalis-spec II.1)
 *
 * Linea mandatorum NON scriptor alter fit (lex 'processus unus
 * scribit', include/scrinium.h) sed cliens: petitionem per canalem ad
 * servum currentem mittit. Canalis localis est per definitionem -
 * numquam per rete.
 *
 * Nomen CONCEPTUS, non mechanismi. Implementatio: canalis_posix.c
 * (AF_UNIX: directorium 0700, socket 0600, uid paris per getpeereid /
 * SO_PEERCRED). Connexiones TcpConnexio ordinariae redduntur
 * (tcp_ex_fossa), ergo tcp_recipere / tcp_mittere et reactor
 * immutati.
 *
 * AUCTORITAS: uid idem SOLUM (canalis_par_admissus). Nullus grex,
 * nulla exceptio radicis (radix 'sudo -u' habet).
 *
 * Windows (nondum): AF_UNIX per Winsock (Win10 1803+, WSAPoll) sine
 * credentialibus paris - admissio per ACL directorii; aut named pipes
 * (ACL + GetNamedPipeClientProcessId; IOCP, non poll, non
 * TcpConnexio). Cum portatione decernendum.
 */
#ifndef CANALIS_H
#define CANALIS_H

/* <aedilis corpus="lib/canalis_posix.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tcp.h"

nomen structura CanalisAuditor CanalisAuditor;

nomen structura {
               b32  successus;
    CanalisAuditor* auditor;
            chorda  error_descriptio;
} CanalisResultus;

/* Auscultare in via (socket localis).
 *
 * PRAECONDITIO: vocans seram scriptoris unius tenet - socket vetus in
 * via tunc nullius servi vivi est, ergo deleri tuto potest.
 *
 * Directorium parens absens -> creatur 0700; praesens cum ullo bit
 * gregis aut aliorum -> recusatur ("directorium canalis apertum"),
 * numquam tacite artatur. In via: socket -> deletur; aliud (filum,
 * directorium) -> recusatur et intactum manet. Via longior sun_path
 * (CIV octeti macOS, CVIII Linux, NUL numerato) -> recusatur, limes
 * nominatur, numquam truncatur. Socket 0600 (sine umask), non-blocans. */
CanalisResultus
canalis_audire (
    constans character* via,
               Piscina* piscina);

/* Accipere connexionem (non-blocans): nihil pendens -> successus
 * FALSUM, error TCP_ERROR_ITERUM. Connexio accepta non-blocans. */
TcpResultus
canalis_accipere (
    CanalisAuditor* auditor,
           Piscina* piscina);

/* fossa auscultans (pro reactore); -I si auditor NIHIL */
integer
canalis_fossa (
    CanalisAuditor* auditor);

/* claudit et viam delet; NIHIL tutum */
vacuum
canalis_claudere (
    CanalisAuditor* auditor);

/* Connectere (obstruens): quaelibet defectio -> successus FALSUM cum
 * descriptione (absens et recusatum non distinguuntur - sera vocantis
 * arbitratur). */
TcpResultus
canalis_connectere (
    constans character* via,
               Piscina* piscina);

/* uid paris: VERUM si lectus; FALSUM si legi nequit */
b32
canalis_par_uid (
    TcpConnexio* connexio,
            i32* uid);

/* uid paris lectus ET == geteuid(); aliter FALSUM (clausum deficit) */
b32
canalis_par_admissus (
    TcpConnexio* connexio);

#endif /* CANALIS_H */
