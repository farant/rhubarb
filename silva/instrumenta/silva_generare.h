/* silva_generare.h - Generator Grammaticae GLR
 *
 * Legit grammaticam ex STML, computat FIRST/FOLLOW coniuncta,
 * construit collectio canonica LR(1).
 *
 * Usus:
 *   SilvaGenGrammatica* g = silva_gen_grammaticam_legere(piscina, intern, stml);
 *   silva_gen_first_computare(g);
 *   silva_gen_follow_computare(g);
 *   SilvaGenCollectio* c = silva_gen_collectio_construere(g);
 *   silva_gen_collectio_imprimere(c);
 */

#ifndef SILVA_GENERARE_H
#define SILVA_GENERARE_H

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
    chorda*  exemplum;         /* Pro lexere: exemplum congruendi (vel NIHIL) */
    s32      index;            /* Index in tabula symbolorum */
    b32      est_terminale;    /* VERUM si terminale */
    Xar*     first;            /* FIRST coniunctum: Xar de s32 (indices terminalium) */
    Xar*     follow;           /* FOLLOW coniunctum: Xar de s32 (indices terminalium) */
    b32      habet_epsilon;    /* VERUM si epsilon in FIRST */
} SilvaGenSymbolum;

/* ================================================
 * Mappa loci - annotatio symbolum@locus (forma v0, spec-v2 §9.3)
 * Parallela dextro productionis: loci[k] pertinet ad dextrum[k].
 * ================================================ */

nomen structura {
    chorda*  titulus;          /* nomen loci (slot); NIHIL = sine loco */
    b32      appendere;        /* @locus+ (lista accumulans) */
} SilvaGenLocusMappa;

/* ================================================
 * Productio - Regula A -> alpha (cum annotationibus)
 * ================================================ */

nomen structura {
    s32      sinistrum;        /* Index symboli non-terminalis (LHS) */
    Xar*     dextrum;          /* Xar de s32: indices symbolorum (RHS) */
    s32      index;            /* Index productionis */
    /* Annotationes (forma v0): */
    chorda*  genus;            /* genus nodi; NIHIL = pass-through */
    chorda*  id;               /* id stabile (mandatum si genus) */
    chorda*  modus;            /* lista-initium|lista-appendere|NIHIL */
    chorda*  manu;             /* functio constructionis manualis|NIHIL */
    Xar*     loci;             /* Xar de SilvaGenLocusMappa - parallela dextro */
} SilvaGenProductio;

/* ================================================
 * Genus extra - genera structuralia (<genera-extra>):
 * CONDITIONALIS, ERROR, AMBIGUUS ... unum registrum (S30)
 * ================================================ */

nomen structura {
    chorda*  titulus;          /* nomen generis */
    chorda*  loci_descriptio;  /* "tokens:lista-token ..." (cruda; layouts Chunk D) */
} SilvaGenGenusExtra;

/* ================================================
 * Registrum generum (S20) - genera nodorum cum locis
 * unificatis trans productiones + genera-extra.
 * species = SilvaLocusSpecies (silva_nodus.h) ut s32.
 * ================================================ */

nomen structura {
    chorda*  titulus;          /* nomen loci */
    s32      species;          /* SilvaLocusSpecies */
} SilvaGenLocusDef;

nomen structura {
    chorda*  titulus;          /* nomen generis */
    Xar*     loci;             /* Xar de SilvaGenLocusDef (per valorem) */
    b32      ex_extra;         /* ex <genera-extra> */
} SilvaGenGenusDef;

/* ================================================
 * Praelatio (<praelatio terminalis="ELSE"
 * actio="transponere"/>) - resolutio declarata cellae
 * conflictus in tabulis (M2c; genera-c89.md decisiones 12)
 * ================================================ */

nomen structura {
    s32  terminalis;   /* index symboli terminalis */
    s32  actio;        /* SilvaGenActioGenus retinendum (s32) */
} SilvaGenPraelatio;

/* ================================================
 * Grammatica - Collectio plena
 * ================================================ */

nomen structura {
    Xar*                     symbola;           /* Xar de SilvaGenSymbolum */
    Xar*                     productiones;      /* Xar de SilvaGenProductio */
    Xar*                     genera_extra;      /* Xar de SilvaGenGenusExtra */
    Xar*                     praelationes;      /* Xar de SilvaGenPraelatio */
    s32                      initium_index;     /* Index symboli initialis */
    i32                      numerus_terminalium;
    i32                      numerus_non_terminalium;
    Piscina*                 piscina;
    InternamentumChorda*     intern;
} SilvaGenGrammatica;

/* ================================================
 * Legere Grammaticam ex STML
 * ================================================ */

/* Legere grammaticam ex literis STML
 * Redde: SilvaGenGrammatica* vel NIHIL si error
 */
SilvaGenGrammatica*
silva_gen_grammaticam_legere(
    Piscina*                 piscina,
    InternamentumChorda*     intern,
    constans character*      stml_fons);

/* Computare registrum generum (S20): unificatio locorum trans
 * productiones eiusdem generis (conflictus specierum = error) +
 * genera-extra (collisio generum = error). Species inferuntur:
 * terminale@locus -> TOKEN; non-terminale@locus -> NODUS (vel
 * LISTA_MIXTA si non-terminale lista-valens est); @locus+ -> LISTA_*.
 * Reddit Xar de SilvaGenGenusDef vel NIHIL in errore. */
Xar*
silva_gen_registrum_computare(
    SilvaGenGrammatica*  grammatica);

/* ================================================
 * FIRST Coniuncta
 * ================================================ */

/* Computare FIRST coniuncta pro omnibus symbolis
 * Redde: VERUM si successus
 */
b32
silva_gen_first_computare(
    SilvaGenGrammatica*  grammatica);

/* Obtinere FIRST coniunctum pro symbolo
 * Redde: Xar* de i32 (indices terminalium)
 */
Xar*
silva_gen_first_obtinere(
    SilvaGenGrammatica*  grammatica,
    i32                 symbolum_index);

/* ================================================
 * Depuratio - Imprimere
 * ================================================ */

/* Imprimere grammaticam ad stdout */
vacuum
silva_gen_grammaticam_imprimere(
    SilvaGenGrammatica*  grammatica);

/* Imprimere FIRST coniuncta ad stdout */
vacuum
silva_gen_first_imprimere(
    SilvaGenGrammatica*  grammatica);

/* ================================================
 * FOLLOW Coniuncta
 * ================================================ */

/* Computare FOLLOW coniuncta pro omnibus symbolis
 * Requirit: FIRST iam computata
 * Redde: VERUM si successus
 */
b32
silva_gen_follow_computare(
    SilvaGenGrammatica*  grammatica);

/* Obtinere FOLLOW coniunctum pro symbolo
 * Redde: Xar* de s32 (indices terminalium), vel NIHIL
 */
Xar*
silva_gen_follow_obtinere(
    SilvaGenGrammatica*  grammatica,
    i32                 symbolum_index);

/* Imprimere FOLLOW coniuncta ad stdout */
vacuum
silva_gen_follow_imprimere(
    SilvaGenGrammatica*  grammatica);

/* ================================================
 * LR(1) Res (Item): [A -> alpha . beta, a]
 * ================================================ */

nomen structura {
    s32  productio;     /* Index productionis */
    s32  punctum;       /* Positio puncti (0..longitudo RHS) */
    s32  prospectus;    /* Lookahead: index terminalis (-1 = EOF/finis) */
} SilvaGenRes;

/* ================================================
 * LR(1) Status (State): coniunctum rerum
 * ================================================ */

nomen structura {
    Xar*  res;          /* Xar de SilvaGenRes */
    s32   index;        /* Status numerus */
} SilvaGenStatus;

/* ================================================
 * Transitio: status --symbolum--> status_novus
 * ================================================ */

nomen structura {
    s32  status;        /* Fons status */
    s32  symbolum;      /* Symbolum transitionis */
    s32  status_novus;  /* Destinatio status */
} SilvaGenTransitio;

/* ================================================
 * Collectio Canonica: omnes status + transitiones
 * ================================================ */

nomen structura {
    Xar*                status_omnes;     /* Xar de SilvaGenStatus */
    Xar*                transitiones;     /* Xar de SilvaGenTransitio */
    SilvaGenGrammatica*  grammatica;
} SilvaGenCollectio;

/* ================================================
 * Constructio Collectionis LR(1)
 * ================================================ */

/* Construere collectionem canonicam LR(1)
 * Requirit: FIRST iam computata
 * Redde: SilvaGenCollectio* vel NIHIL si error
 */
SilvaGenCollectio*
silva_gen_collectio_construere(
    SilvaGenGrammatica*  grammatica);

/* Construere collectionem LALR(1) (multo velocior quam canonica LR(1))
 * Construit LR(0) nucleos, deinde propagat prospectus.
 * Requirit: FIRST iam computata
 * Redde: SilvaGenCollectio* vel NIHIL si error
 */
SilvaGenCollectio*
silva_gen_collectio_lalr_construere(
    SilvaGenGrammatica*  grammatica);

/* Imprimere collectionem ad stdout */
vacuum
silva_gen_collectio_imprimere(
    SilvaGenCollectio*   collectio);

/* ================================================
 * ACTION/GOTO Tabula LR(1)
 * ================================================ */

/* Genus actionis in tabula */
nomen enumeratio {
    SILVA_GEN_ACTIO_ERRARE,       /* 0: Nulla actio valida */
    SILVA_GEN_ACTIO_TRANSPONERE,  /* SHIFT: impellere statum, consumere token */
    SILVA_GEN_ACTIO_REDUCERE,     /* REDUCE: applicare productionem */
    SILVA_GEN_ACTIO_ACCIPERE      /* ACCEPT: parsura completa */
} SilvaGenActioGenus;

/* Introitus in tabula ACTION */
nomen structura {
    s32                terminalis;          /* Index symboli (-1 = $) */
    SilvaGenActioGenus  actio;              /* SHIFT/REDUCE/ACCEPT */
    s32                valor;              /* SHIFT: status destinationis; REDUCE: index productionis */
    b32                conflictus_intentus; /* VERUM si conflictus GLR */
} SilvaGenActioIntroitus;

/* Introitus in tabula GOTO */
nomen structura {
    s32  non_terminalis;  /* Index symboli */
    s32  status_novus;    /* Status destinationis */
} SilvaGenGotoIntroitus;

/* Tabula pro uno statu */
nomen structura {
    Xar*  actiones;         /* Xar de SilvaGenActioIntroitus */
    Xar*  goto_introitus;   /* Xar de SilvaGenGotoIntroitus */
    s32   index;
    b32   habet_conflictum;
} SilvaGenStatusTabula;

/* Cella praelata: conflictus declarate resolutus (memoria pro
 * emissione + censu) */
nomen structura {
    s32  status;
    s32  terminalis;
    s32  actio_retenta;      /* SilvaGenActioGenus (s32) */
    s32  productio_remota;   /* index productionis remotae */
} SilvaGenCellaPraelata;

/* Tabula completa ACTION/GOTO */
nomen structura {
    Xar*                status_tabulae;   /* Xar de SilvaGenStatusTabula */
    SilvaGenGrammatica*  grammatica;
    SilvaGenCollectio*   collectio;
    s32                 numerus_conflictuum;
    Xar*                cellae_praelatae; /* Xar de SilvaGenCellaPraelata */
    i32                 numerus_praelatarum;
} SilvaGenTabula;

/* Construere tabulam ACTION/GOTO ex collectione canonica
 * Redde: SilvaGenTabula* vel NIHIL si error
 */
SilvaGenTabula*
silva_gen_tabulam_construere(
    SilvaGenCollectio*  collectio);

/* Quaerere actiones pro (status, terminalis)
 * Redde: Xar* de SilvaGenActioIntroitus (possibilis plures si conflictus)
 */
Xar*
silva_gen_actiones_quaerere(
    SilvaGenTabula*  tabula,
    s32             status,
    s32             terminalis);

/* Quaerere goto pro (status, non_terminalis)
 * Redde: index status novi, vel -1 si non inventum
 */
s32
silva_gen_goto_quaerere(
    SilvaGenTabula*  tabula,
    s32             status,
    s32             non_terminalis);

/* Imprimere tabulam ad stdout */
vacuum
silva_gen_tabulam_imprimere(
    SilvaGenTabula*  tabula);

/* Imprimere conflictus ad stdout */
vacuum
silva_gen_conflictus_imprimere(
    SilvaGenTabula*  tabula);

#endif /* SILVA_GENERARE_H */
