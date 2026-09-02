/* silva_glr.h - Motor GLR super tabulas coctas (Phase 4)
 *
 * Compositio (spec-v2 par. 3.1): sceletum lapifex_glr (GSS + enumeratio
 * viarum + worklist) + refinamenta arbor2 (duo passus per lexema,
 * exhaustio reducendorum, fusio in fronte translationis) + SilvaValor
 * signatus + constructio generata per callback.
 *
 * Motor tabulae-ignarus est: tabulam coctam (SilvaTabulaCocta) et
 * constructorem (subscriptio silva_*_construere generata) accipit -
 * grammaticam numquam novit. Dispatch unus ex ambobus passibus (par 9.4).
 *
 * REDUCTIONES: nodus quisque reductione creatus RECENS est (nulla fusio
 * intra passum reducendi - vide phase-log Phase 4 INTENTIO: fusio
 * intra-passum lapifex cum dedup (nodus, productio) vias per marginem
 * novum amittere potest). Exhaustio FIFO = undae latitudinis-primae;
 * fusio semel per lexema in fronte translationis (Chunk B).
 *
 * MEMORIA (duae piscinae, par 3.1): nodi GSS et apparatus in piscina
 * motoris (effimera per parsuram); arbores per constructorem in
 * piscina_arborum (a vocatore possessa) allocantur.
 *
 * PURITAS (S26/S27): constructor purus; pater numquam in constructione;
 * passus post-acceptum (Chunk C) eum figit.
 */

#ifndef SILVA_GLR_H
#define SILVA_GLR_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_tabulae.h"

/* Longitudo maxima dextri productionis (in validatione imposita) */
#define SILVA_GLR_DEXTRUM_MAXIMUM 32

/* Terminalia sentinela prospectus */
#define SILVA_GLR_PROSPECTUS_FINIS   (-1)  /* $ - ultra fluxum */
#define SILVA_GLR_TERMINALE_IGNOTUM  (-2)  /* genus sine terminale in tabula */


/* ==================================================
 * Constructor (subscriptio generata, par 9.4)
 * ================================================== */

nomen SilvaValor (*SilvaGLRConstructor) (
    Piscina*             piscina,
    s32                  productio,
    constans SilvaValor* valores);

/* Fabrica ambigui (generata - silva_PREFIX_ambiguum_fabricare):
 * involucrum AMBIGUUS ex lista interpretationum + indice canonico.
 * REQUISITA (spec-v2 par 12.2) - genera structuralia generator
 * imponit, ergo fabrica semper existit. */
nomen SilvaValor (*SilvaGLRFabricaAmbigui) (
    Piscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica);


/* ==================================================
 * Nodus GSS (acervus grapho-structuratus)
 * ================================================== */

nomen structura SilvaGSSNodus SilvaGSSNodus;

structura SilvaGSSNodus {
           s32  status;
    SilvaValor  valor;
           Xar* praedecessores;   /* SilvaGSSNodus* (dedup in additione) */
    SilvaToken* lexema;           /* lexema quo creatus (diagnostica/fusio) */
};


/* ==================================================
 * Motor
 * ================================================== */

nomen structura {
    constans SilvaTabulaCocta* tabula;
          SilvaGLRConstructor  constructor;
       SilvaGLRFabricaAmbigui  fabrica;
                      Piscina* piscina;    /* GSS + apparatus */

    /* Effimera per-reductionem REUSA (census 2026-07-04: xar recens
     * per reductionem = LXXXIV centesimae apicis arenae; una viva
     * simul - contenta per COPIAM VALORIS in constructiones fluunt,
     * numquam retenta). Vacatur in _vias_enumerare, segmenta manent. */
    Xar* viae_effimerae;

    /* Fines (Phase 7 Chunk A - par 8.2). 0 = infinitum. Frons ultra
     * limen = fractura munda segmenti (gubernator nodum ERROR facit -
     * totalitas tenet). Intermissio: pergere FALSUM = desiste;
     * passus_pergendi = intervallum lexematum inter interrogationes. */
    i32 limen_frontis;
    b32     (*pergere)(vacuum* datum);   /* NIHIL = numquam rogare */
    vacuum* pergere_datum;
       i32  passus_pergendi;

    /* Statisticae parsurae novissimae (etiam in fructum copiatae -
     * numeratores sunt PRODUCTUM, non depuratio: spec-v2 par 12.2) */
    i32 frons_maxima;
    i32 nodi_creati;
    i32 reductiones_factae;
    i32 fusiones;                /* compactiones (aequales + involutae) */
    i32 transmutationes;         /* religationes identitatis post-exhaustum */
    i32 transmutationes_negatae; /* exhaustum non-NODUS - brachium separatum */
    i32 eventa_marginis_novi;    /* idem status, basis alia (classis Farshi) */
} SilvaGLR;

/* Defaltae finium motoris (tree-sitter habitus - semper activae) */
#define SILVA_GLR_LIMEN_FRONTIS_DEFALTUM  4096
#define SILVA_GLR_PASSUS_PERGENDI_DEFALTUM 1024


/* ==================================================
 * Fructus parsurae
 * ================================================== */

nomen structura {
           b32  successus;
    SilvaValor  valor;              /* radix arboris */
    SilvaToken* lexema_finis;       /* lexema EOF translatum (trivia!) */
           i32  numerus_acceptorum;

    /* Numeratores compactionis (productum queribile) */
    i32 frons_maxima;
    i32 fusiones;
    i32 transmutationes;
    i32 transmutationes_negatae;
    i32 eventa_marginis_novi;

    /* Error (successus == FALSUM) */
    b32 est_error;
    s32 positio;            /* index lexematis ubi fractum */
    s32 terminalis;         /* prospectus (aut IGNOTUM) */
    s32 status;             /* status primus frontis tunc */
    b32 est_ultra_limen;    /* frons limen excessit (Phase 7) */
    b32 est_intermissus;    /* pergere FALSUM reddidit */
} SilvaGLRFructus;


/* ==================================================
 * API
 * ================================================== */

/* Validare tabulam coctam: fines omnium introituum, ordinatio laminarum,
 * longitudines productionum, goto pro quoque LHS reducibili (goto absens
 * = circuitus infinitus, non defectus mundus - arbor2 worklog), et
 * UNICITAS SYMBOLI ACCESSUS (quisque status uno solo symbolo intratur -
 * proprietas LR quam index reducendorum praesumit; hic assertio, non
 * assumptio - spec-v2 par 12.2). Piscina pro spatio effimero.
 * Errores in stderr; reddit FALSUM in tabula invalida. */
b32
silva_glr_tabulam_validare (
                      Piscina* piscina,
    constans SilvaTabulaCocta* tabula);

/* Creare motorem. Tabula validatur - NIHIL in invalida.
 * Fabrica REQUISITA (NIHIL = error): genera structuralia generator
 * imponit, ergo quaevis tabula generata fabricam habet. */
SilvaGLR*
silva_glr_creare (
                      Piscina* piscina,
    constans SilvaTabulaCocta* tabula,
          SilvaGLRConstructor  constructor,
       SilvaGLRFabricaAmbigui  fabrica);

/* Parsare fluxum lexematum (Xar de SilvaToken*, EOF ultimo - forma
 * silva_lexare). Arbores in piscina_arborum allocantur. Reentrans:
 * status frontis per parsuram localis est. */
SilvaGLRFructus
silva_glr_parsare (
        SilvaGLR* glr,
    constans Xar* lexemata,
         Piscina* piscina_arborum);

/* Terminale coctum pro genere lexematis (SILVA_GLR_TERMINALE_IGNOTUM
 * si genus in tabula non est). Publicum pro probationibus/adaptatoribus. */
s32
silva_glr_terminale_ex_genere (
    constans SilvaTabulaCocta* tabula,
             SilvaLexemaGenus  genus);

#endif /* SILVA_GLR_H */
