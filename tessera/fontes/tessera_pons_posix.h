/* tessera_pons_posix.h - Pons POSIX (Phase B)
 *
 * Machina vera: termios crudus + scrinium alternum + mus SGR +
 * TIOCGWINSZ + signa (WINCH interrumpit, TSTP/CONT intermittunt et
 * resumunt, fatalia restituunt et remoriuntur) + atexit. CAPITA
 * SYSTEMATIS IN .c SOLO (mos tcp.h; probatum sub vexillis plenis
 * sine macris capacitatum - spec-v2 par 1.6).
 *
 * NIHIL redditur si stdin non terminal est (sine capite = pons
 * memoriae, expressus).
 *
 * STATICUM UNUM SANCTUM (CLAUDE.md pin): termios servatus + chorda
 * restitutionis praefixa pro tractatoribus signorum fatalium
 * (write + tcsetattr - ambo async-signal-tuta).
 */

#ifndef TESSERA_PONS_POSIX_H
#define TESSERA_PONS_POSIX_H

#include "latina.h"
#include "piscina.h"
#include "tessera_pons.h"

TesseraPons*
tessera_pons_posix_creare (
    Piscina* piscina);

#endif /* TESSERA_PONS_POSIX_H */
