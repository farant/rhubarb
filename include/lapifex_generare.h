/* lapifex_generare.h - Generator Grammaticae GLR
 *
 * Legit grammaticam ex STML, computat FIRST/FOLLOW coniuncta,
 * construit collectio canonica LR(1).
 *
 * Usus:
 *   LapifexGrammatica* g = lapifex_grammaticam_legere(piscina, intern, stml);
 *   lapifex_first_computare(g);
 *   lapifex_follow_computare(g);
 *   LapifexCollectio* c = lapifex_collectio_construere(g);
 *   lapifex_collectio_imprimere(c);
 */

#ifndef LAPIFEX_GENERARE_H
#define LAPIFEX_GENERARE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"

/* ================================================
 * Symbolum - Terminale vel Non-Terminale
 * ================================================ */

nomen structura {
    chorda*  titulus;          /* Symbolum appellatio (internatum) */
    chorda*  genus;            /* Pro terminalibus: genus lexematis */
    s32      index;            /* Index in tabula symbolorum */
    b32      est_terminale;    /* VERUM si terminale */
    Xar*     first;            /* FIRST coniunctum: Xar de s32 (indices terminalium) */
    Xar*     follow;           /* FOLLOW coniunctum: Xar de s32 (indices terminalium) */
    b32      habet_epsilon;    /* VERUM si epsilon in FIRST */
} LapifexSymbolum;

/* ================================================
 * Productio - Regula A -> alpha
 * ================================================ */

nomen structura {
    s32      sinistrum;        /* Index symboli non-terminalis (LHS) */
    Xar*     dextrum;          /* Xar de s32: indices symbolorum (RHS) */
    s32      index;            /* Index productionis */
} LapifexProductio;

/* ================================================
 * Grammatica - Collectio plena
 * ================================================ */

nomen structura {
    Xar*                     symbola;           /* Xar de LapifexSymbolum */
    Xar*                     productiones;      /* Xar de LapifexProductio */
    s32                      initium_index;     /* Index symboli initialis */
    i32                      numerus_terminalium;
    i32                      numerus_non_terminalium;
    Piscina*                 piscina;
    InternamentumChorda*     intern;
} LapifexGrammatica;

/* ================================================
 * Legere Grammaticam ex STML
 * ================================================ */

/* Legere grammaticam ex literis STML
 * Redde: LapifexGrammatica* vel NIHIL si error
 */
LapifexGrammatica*
lapifex_grammaticam_legere(
    Piscina*                 piscina,
    InternamentumChorda*     intern,
    constans character*      stml_fons);

/* ================================================
 * FIRST Coniuncta
 * ================================================ */

/* Computare FIRST coniuncta pro omnibus symbolis
 * Redde: VERUM si successus
 */
b32
lapifex_first_computare(
    LapifexGrammatica*  grammatica);

/* Obtinere FIRST coniunctum pro symbolo
 * Redde: Xar* de i32 (indices terminalium)
 */
Xar*
lapifex_first_obtinere(
    LapifexGrammatica*  grammatica,
    i32                 symbolum_index);

/* ================================================
 * Depuratio - Imprimere
 * ================================================ */

/* Imprimere grammaticam ad stdout */
vacuum
lapifex_grammaticam_imprimere(
    LapifexGrammatica*  grammatica);

/* Imprimere FIRST coniuncta ad stdout */
vacuum
lapifex_first_imprimere(
    LapifexGrammatica*  grammatica);

/* ================================================
 * FOLLOW Coniuncta
 * ================================================ */

/* Computare FOLLOW coniuncta pro omnibus symbolis
 * Requirit: FIRST iam computata
 * Redde: VERUM si successus
 */
b32
lapifex_follow_computare(
    LapifexGrammatica*  grammatica);

/* Obtinere FOLLOW coniunctum pro symbolo
 * Redde: Xar* de s32 (indices terminalium), vel NIHIL
 */
Xar*
lapifex_follow_obtinere(
    LapifexGrammatica*  grammatica,
    i32                 symbolum_index);

/* Imprimere FOLLOW coniuncta ad stdout */
vacuum
lapifex_follow_imprimere(
    LapifexGrammatica*  grammatica);

/* ================================================
 * LR(1) Res (Item): [A -> alpha . beta, a]
 * ================================================ */

nomen structura {
    s32  productio;     /* Index productionis */
    s32  punctum;       /* Positio puncti (0..longitudo RHS) */
    s32  prospectus;    /* Lookahead: index terminalis (-1 = EOF/finis) */
} LapifexRes;

/* ================================================
 * LR(1) Status (State): coniunctum rerum
 * ================================================ */

nomen structura {
    Xar*  res;          /* Xar de LapifexRes */
    s32   index;        /* Status numerus */
} LapifexStatus;

/* ================================================
 * Transitio: status --symbolum--> status_novus
 * ================================================ */

nomen structura {
    s32  status;        /* Fons status */
    s32  symbolum;      /* Symbolum transitionis */
    s32  status_novus;  /* Destinatio status */
} LapifexTransitio;

/* ================================================
 * Collectio Canonica: omnes status + transitiones
 * ================================================ */

nomen structura {
    Xar*                status_omnes;     /* Xar de LapifexStatus */
    Xar*                transitiones;     /* Xar de LapifexTransitio */
    LapifexGrammatica*  grammatica;
} LapifexCollectio;

/* ================================================
 * Constructio Collectionis LR(1)
 * ================================================ */

/* Construere collectionem canonicam LR(1)
 * Requirit: FIRST iam computata
 * Redde: LapifexCollectio* vel NIHIL si error
 */
LapifexCollectio*
lapifex_collectio_construere(
    LapifexGrammatica*  grammatica);

/* Imprimere collectionem ad stdout */
vacuum
lapifex_collectio_imprimere(
    LapifexCollectio*   collectio);

/* ================================================
 * ACTION/GOTO Tabula LR(1)
 * ================================================ */

/* Genus actionis in tabula */
nomen enumeratio {
    LAPIFEX_ACTIO_ERRARE,       /* 0: Nulla actio valida */
    LAPIFEX_ACTIO_TRANSPONERE,  /* SHIFT: impellere statum, consumere token */
    LAPIFEX_ACTIO_REDUCERE,     /* REDUCE: applicare productionem */
    LAPIFEX_ACTIO_ACCIPERE      /* ACCEPT: parsura completa */
} LapifexActioGenus;

/* Introitus in tabula ACTION */
nomen structura {
    s32                terminalis;          /* Index symboli (-1 = $) */
    LapifexActioGenus  actio;              /* SHIFT/REDUCE/ACCEPT */
    s32                valor;              /* SHIFT: status destinationis; REDUCE: index productionis */
    b32                conflictus_intentus; /* VERUM si conflictus GLR */
} LapifexActioIntroitus;

/* Introitus in tabula GOTO */
nomen structura {
    s32  non_terminalis;  /* Index symboli */
    s32  status_novus;    /* Status destinationis */
} LapifexGotoIntroitus;

/* Tabula pro uno statu */
nomen structura {
    Xar*  actiones;         /* Xar de LapifexActioIntroitus */
    Xar*  goto_introitus;   /* Xar de LapifexGotoIntroitus */
    s32   index;
    b32   habet_conflictum;
} LapifexStatusTabula;

/* Tabula completa ACTION/GOTO */
nomen structura {
    Xar*                status_tabulae;   /* Xar de LapifexStatusTabula */
    LapifexGrammatica*  grammatica;
    LapifexCollectio*   collectio;
    s32                 numerus_conflictuum;
} LapifexTabula;

/* Construere tabulam ACTION/GOTO ex collectione canonica
 * Redde: LapifexTabula* vel NIHIL si error
 */
LapifexTabula*
lapifex_tabulam_construere(
    LapifexCollectio*  collectio);

/* Quaerere actiones pro (status, terminalis)
 * Redde: Xar* de LapifexActioIntroitus (possibilis plures si conflictus)
 */
Xar*
lapifex_actiones_quaerere(
    LapifexTabula*  tabula,
    s32             status,
    s32             terminalis);

/* Quaerere goto pro (status, non_terminalis)
 * Redde: index status novi, vel -1 si non inventum
 */
s32
lapifex_goto_quaerere(
    LapifexTabula*  tabula,
    s32             status,
    s32             non_terminalis);

/* Imprimere tabulam ad stdout */
vacuum
lapifex_tabulam_imprimere(
    LapifexTabula*  tabula);

/* Imprimere conflictus ad stdout */
vacuum
lapifex_conflictus_imprimere(
    LapifexTabula*  tabula);

#endif /* LAPIFEX_GENERARE_H */
