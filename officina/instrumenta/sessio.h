/* sessio.h - machina sessionis M4b (nucleus REPL, sine UI)
 *
 * Documentum = veritas; mundus = cache replicationis (chunk B).
 * Chunk A: documentum + turni + iudicium ante demissionem — SINE
 * machinula. Fluxus turni: parsura classificationis (grammatica
 * liberalis, reparatio `;`) → transformatio involucri (sententiae →
 * `void turnus_N(void){…}`) → iudicium textus involuti (receptum
 * examinis, bis-analysis) → verdictum documenti TOTIUS (violatio
 * non-provisionalis ubivis reicit — invariabile viride) → appensio
 * aut substitutio (religatio in loco).
 *
 * Consumit amalgama silvae (silva.h) + cives lib (piscina/chorda/
 * xar). Superficies quattuor consumptorum: colloquium (linearis),
 * cursor #! (batch), subsellium (tessera, tractio), vindex-eval
 * (tractio). Spec: project-specs/officina-m4b-spec.md.
 */

#ifndef SESSIO_H
#define SESSIO_H

#include "piscina.h"
#include "chorda.h"

typedef struct Sessio Sessio;

/* Genus turni (classificatio per elementum grammaticae) */
enum {
    SESSIO_TURNUS_DECLARATIO = 0,
    SESSIO_TURNUS_SENTENTIA  = 1,
    SESSIO_TURNUS_EXPRESSIO  = 2,   /* sententia-expressionis */
    SESSIO_TURNUS_DEFINITIO  = 3,
    SESSIO_TURNUS_DIRECTIVA  = 4    /* initus incipiens '#' */
};

/* Verdictum oblationis */
enum {
    SESSIO_ACCEPTUM  = 0,   /* in documentum receptum */
    SESSIO_REIECTUM  = 1,   /* iudicium REICE - documentum immotum */
    SESSIO_APPARATUS = 2    /* infrastructura fracta */
};

/* Diagnosticum ostensum: positiones in DOCUMENTO OSTENSO (lineae
 * quas usor typavit), non in textu involuto iudicato. */
typedef struct {
    chorda via;            /* "<sessio>" aut caput praebitum */
    i32    linea;          /* 1-basata; 0 si ignota */
    i32    columna;
    s32    severitas;      /* ExamenSeveritas speculum (0=violatio,
                            * 1=suspectum, 2=domesticum, 3=infra) */
    s32    codex;          /* ExamenCodex; -1 = regula sessionis */
    b32    provisionale;
    b32    extra_turnum;   /* extra turnum novum flagravit (C2:
                            * redefinitio turnum veterem aegrotat) */
    s32    turnus_vetus;   /* index turni veteris; -1 nisi */
    chorda nuntius;
} SessioDiagnosticum;

/* Relatum oblationis. Chordae + ordines in piscina sessionis
 * allocantur; vivunt usque ad sessio_destruere. */
typedef struct {
    s32    verdictum;           /* SESSIO_* */
    s32    genus;               /* SESSIO_TURNUS_*; -1 si ignotum */
    b32    reparatum;           /* `;` appensum (echo debetur) */
    b32    substitutus;         /* religatio: turnus vetus mutatus */
    s32    turnus_substitutus;  /* index substituti; -1 nisi */
    s32    turnus_index;        /* index turni accepti; -1 si reiectum */
    chorda textus;              /* textus receptus (post reparationem) */
    chorda nomen_declaratum;    /* vacua nisi declaratio/definitio */
    const SessioDiagnosticum* diagnostica;
    s32    diagnostica_numerus;
} SessioRelatum;

typedef struct {
    const char* radix;          /* radix repositorii (systema +
                                 * capita); NIHIL = sine systemate */
    b32         cum_posix;      /* systema POSIX concatenatum */
    b32         sine_capitibus; /* ambulatio capitum omissa (probae
                                 * velociores; #include tunc deest) */
} SessioConfiguratio;

/* Praeparat contextum (latina compilata + systema + capita).
 * NIHIL si infrastructura deest. */
Sessio* sessio_creare(Piscina* piscina,
    const SessioConfiguratio* configuratio);
void sessio_destruere(Sessio* sessio);

/* Offert turnum. Initus = textus turni (una elementum aut directiva
 * una; multi-elementa reiciuntur — corpus compositum { } fuga).
 * Acceptum ⇒ documentum crevit aut turnus religatus in loco. */
SessioRelatum sessio_turnum_offerre(Sessio* sessio, chorda initus);

/* Probatio integritatis initus (librae/parentheses/commenta
 * aperta) — VERUM = initus clausus, offerre licet; FALSUM = pergere
 * legere (superficies lineam continuationis dat). */
b32 sessio_initus_completus(Sessio* sessio, chorda initus);

/* Inspectio documenti */
i32    sessio_turni_numerus(const Sessio* sessio);
chorda sessio_turnus_textus(const Sessio* sessio, i32 index);
s32    sessio_turnus_genus(const Sessio* sessio, i32 index);
chorda sessio_turnus_nomen(const Sessio* sessio, i32 index);

/* Textus ostensus integer (materializatus in piscinam datam) */
chorda sessio_documentum(const Sessio* sessio, Piscina* piscina);

#endif /* SESSIO_H */
