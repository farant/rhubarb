/* tessera_modi.h - Effugia modorum terminalis: intrare et exire
 *
 * INTERNUM (non caput publicum): pons posix eas ut chordas STATICAS
 * adhibet - tractatores signorum (fatalis, TSTP, CONT) eas per
 * write(2) scribunt, async-signal-tute; ergo manent literae
 * praecompositae, non tempore cursus aedificatae.
 *
 * LEX PARIUM (probatio_tessera_modi.c): omnis modus privatus "?Nh" in
 * INTRANDI suum "?Nl" in EXEUNDI habet, et nullus "?Nl" sine pari -
 * modus intratus sed in ruina non relictus terminalem vexat post
 * exitum (glutinum 200~ in concha, quadra suspensa, mus mortuus).
 *
 * Tessera modos PONIT, numquam QUAERIT: terminal modum ignotum
 * tacite neglegit (thesis xterm-solum).
 */

#ifndef TESSERA_MODI_H
#define TESSERA_MODI_H

/* Scrinium alternum + mus (pressus/solutus) + mus SGR */
#define INTRANDI "\033[?1049h\033[?1000h\033[?1006h"

/* Ordine inverso relicti; deinde stilus nativus + cursor visibilis */
#define EXEUNDI  "\033[?1006l\033[?1000l\033[?1049l\033[0m\033[?25h"

#endif /* TESSERA_MODI_H */
