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

/* Genus valoris turni expressionis (impressio hospitis-lateris;
 * chunk C). NULLUS = nihil imprimendum (vacuum/aggregatum/…). */
enum {
    SESSIO_VALOR_NULLUS = 0,
    SESSIO_VALOR_SIGNATUS,
    SESSIO_VALOR_INSIGNATUS,
    SESSIO_VALOR_FLUITANS_32,
    SESSIO_VALOR_FLUITANS_64,
    SESSIO_VALOR_MONSTRATOR,
    SESSIO_VALOR_MONSTRATOR_CHARACTERUM,
    SESSIO_VALOR_AGGREGATUM   /* typus notus, impressio parcata */
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
    /* exsecutio (chunk B: replicatio documenti totius) */
    b32    exsecutum;           /* replicatio facta */
    s32    halitus_genus;       /* MachinulaExitusGenus; -1 nisi
                                 * halitus (BENE non relatum nisi
                                 * codex exit) */
    s64    halitus_codex;
    chorda halitus_nuntius;
    s32    halitus_turnus;      /* quis turnus fregit; -1 nisi */
    chorda effusio;             /* stdout turni novi (captum) */
    chorda effusio_erroris;     /* stderr turni novi */
    /* historia mutata: redefinitio effusionem turni veteris mutavit */
    const s32* turni_mutati;
    s32    turni_mutati_numerus;
    /* valor turni expressionis (chunk C: impressio) */
    b32    divisum;             /* scissio automatica: declaratio +
                                 * assignatio (initiator non
                                 * constans) - textus = assignatio */
    chorda typus_textus;        /* typus redditus latine; vacua si
                                 * irreddibilis */
    s32    valor_genus;         /* SESSIO_VALOR_* */
    b32    valor_validus;       /* exsecutio BENE, valor in codex */
    s64    valor;               /* codex crudus (figurae f32/f64) */
} SessioRelatum;

typedef struct {
    const char* radix;          /* radix repositorii (systema +
                                 * capita); NIHIL = sine systemate */
    b32         cum_posix;      /* systema POSIX concatenatum */
    b32         sine_capitibus; /* ambulatio capitum omissa (probae
                                 * velociores; #include tunc deest) */
    /* bibliothecae demissae SEMEL in mundum (viae radici relativae,
     * e.g. "lib/piscina.c") */
    const char* const* plagulae;
    s32         plagulae_numerus;
    b32         sine_recusationibus;  /* modus scripti: scripturae/
                                       * tempus/stdin LICENT */
} SessioConfiguratio;

/* Praeparat contextum (systema+latina lexicon + capita). NIHIL si
 * infrastructura deest. NOTA: UNA sessio EXSECUTANS uno tempore per
 * processum (regio basi fixa - "una regio uno tempore"); sessio
 * altera creari potest, sed turni eius reicientur APPARATUS dum
 * mundus prioris vivit - destrue priorem primum. */
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

/* effusio capta novissima turni (acta; vacua si numquam involutus
 * aut nondum exsecutus) */
chorda sessio_turnus_effusio(const Sessio* sessio, i32 index);

/* Formator transcripti (chunk C): relatum -> textus ostensionis
 * (echo reparationis, effusio capta, echo declarationis,
 * valor : typus [+ praevisus char*], notitiae historiae, diagnostica,
 * halitus). Superficies (colloquium) eum imprimit; probationes eum
 * byte-comparant. */
chorda sessio_relatum_formare(Sessio* sessio,
    const SessioRelatum* relatum, Piscina* piscina);

/* Textus ostensus integer (materializatus in piscinam datam) */
chorda sessio_documentum(const Sessio* sessio, Piscina* piscina);

/* ==================================================
 * Chunk D: via #! + emissores
 * ================================================== */

/* Scriptum in turnos findere et seriatim offerre (cursor #! +
 * :aperi). Linea #! prima spatiis obliteratur (\n servato -
 * positiones exactae). Primus turnus REIECTUS sistit. Redditur
 * numerus turnorum RECEPTORUM; -1 = apparatus. relatum_out (licet
 * NIHIL) = relatum ultimum (reiecti aut ultimi recepti).
 * NOTA v0: commenta INTER turnos in findendo pereunt. */
s32 sessio_scriptum_offerre(Sessio* sessio, chorda textus,
    SessioRelatum* relatum_out);

/* Functionem nominatam in mundo currenti currit (C12: principale
 * post scriptum; tractio vindex-eval futura). Flumina directa -
 * SINE captura (scriptum = programma verum). VERUM = halitus BENE;
 * codex_out (licet NIHIL) = valor redditus / codex exitus. */
b32 sessio_functionem_currere(Sessio* sessio,
    const char* titulus, s64* codex_out);

/* Exportatio C89 stricta: textus involutus + `#include "latina.h"`
 * praepositum + principale syntheticum turnos ordine vocans (NISI
 * usor principale definivit). clang vexillis domus compilat. */
chorda sessio_documentum_strictum(const Sessio* sessio,
    Piscina* piscina);

#endif /* SESSIO_H */
