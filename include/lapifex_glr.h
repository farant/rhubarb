/* lapifex_glr.h - Motor Parsationis GLR (Generalized LR)
 *
 * Consumit tabulam ACTION/GOTO a lapifex_generare generatam,
 * tractat conflictus per furcationem/fusionem in Acervo
 * Grapho-Structurato (GSS).
 *
 * Usus:
 *   LapifexGLR* glr = lapifex_glr_creare(piscina, tabula);
 *   LapifexGLRFructus f = lapifex_glr_parsare(
 *       glr, signa, numerus, reductio, contextus);
 *   si (f.successus) { ... f.valori[0] ... }
 */

#ifndef LAPIFEX_GLR_H
#define LAPIFEX_GLR_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "lapifex_generare.h"
#include "lapifex_parsere.h"

/* ================================================
 * Maxima GLR
 * ================================================ */

/* Numerus maximus viarum activarum simultanearum */
#define LAPIFEX_GLR_MAX_VIAE CCLVI

/* Numerus maximus fructuum (parsationes ambiguae) */
#define LAPIFEX_GLR_MAX_FRUCTUS XXXII

/* ================================================
 * GSS Nodus - Nodus in Acervo Grapho-Structurato
 * ================================================ */

nomen structura LapifexGSSNodus LapifexGSSNodus;

structura LapifexGSSNodus {
    s32               status;             /* Status LR in hoc nodo */
    s64               valor;              /* Valor semanticus */
    Xar*              praedecessores;     /* Xar de LapifexGSSNodus* */
    s32               profunditas;        /* Profunditas in GSS */
    b32               vivus;              /* VERUM si via adhuc activa */
};

/* ================================================
 * Via Activa - Una via parsationis in GSS
 * ================================================ */

nomen structura {
    LapifexGSSNodus*  culmen;             /* Nodus in culmine huius viae */
    b32               activa;             /* VERUM si via adhuc activa */
    s32               index;              /* Index huius viae */
} LapifexGLRVia;

/* ================================================
 * Fructus GLR
 * ================================================ */

nomen structura {
    s64*              valori;             /* Tabula valorum fructuum */
    s32               numerus_fructuum;   /* Numerus parsationum inventarum */
    b32               successus;          /* VERUM si saltem una parsatio inventa */
    b32               ambiguus;           /* VERUM si plures parsationes inventae */
    LapifexParsaturaError  error;         /* Informatio erroris si fractum */
} LapifexGLRFructus;

/* ================================================
 * GLR Motor
 * ================================================ */

nomen structura {
    LapifexTabula*    tabula;             /* Tabula ACTION/GOTO */
    Piscina*          piscina;            /* Arena allocatoris */
    Xar*              viae;               /* Xar de LapifexGLRVia */
    Xar*              nodi_omnes;         /* Xar de LapifexGSSNodus* (omnes creati) */
    s32               numerus_furcationum; /* Statisticae */
    s32               numerus_fusionum;
} LapifexGLR;

/* ================================================
 * API Publica
 * ================================================ */

/* Creare motorem GLR
 * piscina  - Arena allocatoris
 * tabula   - Tabula ACTION/GOTO (cum possibilibus conflictibus)
 * Redde: LapifexGLR* vel NIHIL si error
 */
LapifexGLR*
lapifex_glr_creare(
    Piscina*        piscina,
    LapifexTabula*  tabula);

/* Parsare flumen signorum usans GLR
 * glr       - Motor GLR creatus
 * signa     - Flumen signorum (tokens)
 * numerus   - Numerus signorum in flumine
 * reductio  - Callback pro reductione (vel NIHIL)
 * contextus - Contextus opacus pro callback
 * Redde: LapifexGLRFructus
 */
LapifexGLRFructus
lapifex_glr_parsare(
    LapifexGLR*              glr,
    constans LapifexSignum*  signa,
    s32                      numerus,
    LapifexReductioCallback  reductio,
    vacuum*                  contextus);

/* Reficere motorem GLR pro nova parsatione
 * (liberare GSS nodos, reficere vias)
 */
vacuum
lapifex_glr_reficere(
    LapifexGLR*  glr);

#endif /* LAPIFEX_GLR_H */
