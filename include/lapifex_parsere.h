/* lapifex_parsere.h - Motor Parsationis LR(1)
 *
 * Consumit flumen signorum usans tabulam ACTION/GOTO,
 * producit valorem semanticum per reductio callbacks.
 *
 * Usus:
 *   LapifexParsaturaFructus f = lapifex_parsare(
 *       tabula, signa, numerus, reductio, contextus);
 *   si (f.successus) { ... f.valor ... }
 */

#ifndef LAPIFEX_PARSERE_H
#define LAPIFEX_PARSERE_H

#include "latina.h"
#include "lapifex_generare.h"

/* Maximum symbolorum in dextro productionis */
#define LAPIFEX_MAXIMUS_DEXTRUM XXXII

/* ================================================
 * Signum - Token in flumine
 * ================================================ */

nomen structura {
    s32  terminalis;    /* Index terminalis (-1 = $ finis) */
    s64  valor;         /* Valor semanticus opacus */
} LapifexSignum;

/* ================================================
 * Error Parsaturae
 * ================================================ */

nomen structura {
    s32  status;        /* Status ubi error accidit */
    s32  terminalis;    /* Terminalis qui causavit errorem */
    s32  positio;       /* Index signi in flumine */
    b32  est_error;     /* VERUM si error */
} LapifexParsaturaError;

/* ================================================
 * Fructus Parsaturae
 * ================================================ */

nomen structura {
    s64                    valor;      /* Valor finalis */
    b32                    successus;
    LapifexParsaturaError  error;
} LapifexParsaturaFructus;

/* ================================================
 * Callback Reductionis
 * ================================================
 * Parametri:
 *   productio_index  - Index productionis reductae
 *   valori           - Tabula valorum ex acervo (dextrum)
 *   numerus_dextrum  - Numerus symbolorum in dextro
 *   contextus        - Contextus opacus ab usore
 * Redde: s64 valor semanticus novus
 */

nomen s64 (*LapifexReductioCallback)(
    s32, constans s64*, s32, vacuum*);

/* ================================================
 * Parsare - Motor Principalis
 * ================================================ */

/* Parsare flumen signorum usans tabulam LR(1)
 *
 * tabula    - Tabula ACTION/GOTO constructa
 * signa     - Flumen signorum (tokens)
 * numerus   - Numerus signorum in flumine
 * reductio  - Callback pro reductione (vel NIHIL)
 * contextus - Contextus opacus pro callback
 *
 * Redde: LapifexParsaturaFructus
 */
LapifexParsaturaFructus
lapifex_parsare(
    LapifexTabula*           tabula,
    constans LapifexSignum*  signa,
    s32                      numerus,
    LapifexReductioCallback  reductio,
    vacuum*                  contextus);

#endif /* LAPIFEX_PARSERE_H */
