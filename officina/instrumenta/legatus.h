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
                                        * (lib/vigilia); NIHIL =
                                        * vigilia quieta
                                        * (probationes) */
    constans character* via_manifesti; /* manifestum clausurae a
                                        * launchero scriptum
                                        * (lib/vigilia - invarians:
                                        * aedificator indicem
                                        * scribit); NIHIL = vigilia
                                        * fontium quieta */
    b32 modus_mcp;               /* VERUM = servus MCP (LEGATI):
                                  * framing lineis-delimitatum,
                                  * methodi tools; radix ex cfg
                                  * (initialize MCP rootUri caret) */
    constans character* signum;  /* sigillum hex binarii (launcher
                                  * computat, K2.1 exemplar
                                  * tabularii) -> serverInfo.version;
                                  * NIHIL = "0" */
    b32 renatus;                 /* MCP: exec sui perfectus (renovare)
                                  * - initium initialize-aequivalens
                                  * SPONTE agitur, quia cliens
                                  * initialize non remittit (servum
                                  * mutatum nescit) */
    constans character* via_renovatoris; /* scriptum quod renovare
                                  * ut exploratorem praevium agit et
                                  * deinde exsequitur; NIHIL =
                                  * <radix>/officina/legatus.sh
                                  * (probationes stipulam dant) */
} LegatusConfiguratio;

/* Ansa principalis: legere epistulas ex intra, respondere/publicare
 * in extra, usque ad "exit" aut finem fluxus. Redde codicem exitus:
 * 0 = exit post shutdown; 1 = exit sine shutdown AUT fluxus finitus
 * (cliens mortuus) AUT fluxus corruptus. cfg NIHIL licet. */
s32
legatus_currere (
                            FILE* intra,
                            FILE* extra,
    constans LegatusConfiguratio* cfg);

#endif /* LEGATUS_H */
