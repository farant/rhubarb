/* legatus.h - LEGATUS: servus LSP silvae (chunk B)
 *
 * Legatus = qui pro silva apud potestates externas loquitur:
 * servus LSP 3.17 super JSON-RPC/stdio, iudicium C89 silvae
 * (examen) editori/agenti praebens. MACHINA cum principali tenui
 * (exemplar sessionis): probationes machinam in-processu super
 * FILE* agunt (transcripta aurea), binarium = involucrum stdio.
 *
 * Ansa synchrona uni-fili: nuntius unus plene tractatur (etiam
 * publicationes eius) ante proximum - conformis (spec §V).
 * STDOUT PURUS EST PROTOCOLLO - omnes notae ad stderr.
 *
 * v0 (chunk B): initialize/shutdown/exit, didOpen/didChange/
 * didClose -> publishDiagnostics (filtrum trans-plagulare C3,
 * exclusiones Q13, gradus severitatis, extensiones ex longitudine).
 * Chunk C addet hover + documentSymbol; chunk D laminam .h +
 * errores syntaxis positos + involucrum plugin.
 */

#ifndef LEGATUS_H
#define LEGATUS_H

#include "latina.h"

#include <stdio.h>

nomen structura {
    constans character* radix;   /* NIHIL = ex rootUri initialize
                                  * (praevalet si datum - commoditas
                                  * probationum) */
    b32 cum_posix;               /* systema += POSIX; praevaletur ab
                                  * initializationOptions.posix */
    constans character* binarium_via;  /* argv[0] - vigilia sui
                                        * (excubitor chunk 3);
                                        * NIHIL = vigilia quieta
                                        * (probationes) */
    constans character* fabrica_via;   /* NIHIL = "fabrica.tsv"
                                        * (commoditas probationum) */
} LegatusConfiguratio;

/* Ansa principalis: legere epistulas ex intra, respondere/publicare
 * in extra, usque ad "exit" aut finem fluxus. Redde codicem exitus:
 * 0 = exit post shutdown; 1 = exit sine shutdown AUT fluxus finitus
 * (cliens mortuus) AUT fluxus corruptus. cfg NIHIL licet. */
s32
legatus_currere (
    FILE*                        intra,
    FILE*                        extra,
    constans LegatusConfiguratio* cfg);

#endif /* LEGATUS_H */
