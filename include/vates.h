/* vates.h - provisor neuter exemplarium linguae (LLM)
 *
 * project-specs/vates-spec.md. Petitio neutra -> provisor (Anthropic
 * primus) -> responsum neutrum; usus et pretium in loco UNO pro omni
 * vocante. Principia: (1) nihil abicitur - blocus ignotus OPACUM fit
 * et verbatim redditur; (2) octeti petitionis deterministici (cache =
 * praefixum); (3) clavis A VOCANTE datur, numquam ex ambitu legitur;
 * (4) CONFIGURATIO linguam provisoris loquitur, CODEX nullam: provisor,
 * exemplar, conatus et optiones provisoris propriae ex configuratione
 * veniunt; codex vocans solum Vates* et typos neutros tangit, ergo
 * mutatio provisoris = mutatio configurationis, non codicis.
 *
 * STATUS: PROBATUM a Frano 2026-10-07 (vates-plan-1 T3); implementatio
 * in vates-plan-2.
 *
 * USUS:
 *   VatesOptiones o = vates_optiones_ordinariae();
 *   Vates* v = vates_aperire(piscina, provisor, clavis, &o);  -- ex configuratione
 *   VatesPetitio* p = vates_petitio_creare(piscina, exemplar);
 *   vates_systema_addere(p, systema, VERUM);
 *   n = vates_nuntium_addere(p, VATES_USOR);
 *   vates_textum_addere(n, textus);
 *   r = vates_mittere(v, p, chorda_ex_literis("magister", piscina), piscina);
 *   si (r->successus && r->causa_finis == VATES_FINIS_INSTRUMENTUM) ...
 *   vates_responsum_addere(p, r);    -- bloci responsi in petitionem
 */

#ifndef VATES_H
#define VATES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "http.h"


/* ========================================================================
 * BLOCI ET NUNTII
 * ======================================================================== */

nomen enumeratio {
    VATES_USOR = 0,
    VATES_ASSISTENS
} VatesMunus;

nomen enumeratio {
    VATES_TEXTUS = 0,
    VATES_INSTRUMENTUM_PETITUM,   /* tool_use */
    VATES_INSTRUMENTI_EFFECTUS,   /* tool_result */
    VATES_OPACUM                  /* cogitatio, signatura, ignota: verbatim */
} VatesBlociGenus;

nomen structura {
    VatesBlociGenus  genus;
             chorda  textus;     /* TEXTUS; INSTRUMENTI_EFFECTUS: contentum */
             chorda  id;         /* PETITUM: id; EFFECTUS: id petiti */
             chorda  titulus;    /* PETITUM: nomen instrumenti */
          JsonValor* input;      /* PETITUM */
                b32  erratum;    /* EFFECTUS: is_error */
          JsonValor* crudum;     /* OPACUM: blocus provisoris verbatim */
             chorda  provisor;   /* OPACUM: provisor originis ("anthropic");
                                  * provisor ALIUS hunc blocum omittit, ut
                                  * API ipsa cogitationem alienam omittit */
                b32 signum_thesauri; /* punctum cache post hunc blocum */
} VatesBlocus;

nomen structura {
    VatesMunus  munus;
           Xar* bloci;          /* VatesBlocus* */
} VatesNuntius;

nomen structura {
        chorda  titulus;
        chorda  descriptio;
     JsonValor* schema;          /* JSON Schema input */
} VatesInstrumentum;


/* ========================================================================
 * PETITIO
 * ======================================================================== */

nomen structura VatesPetitio VatesPetitio;   /* opaca */

VatesPetitio*
vates_petitio_creare (
     Piscina* piscina,
      chorda  exemplar);

/* max_tokens (0 = XVI M). */
vacuum
vates_petitio_maxima_ponere (
    VatesPetitio* petitio,
             s32  signa_maxima);

/* effort, verbis provisoris ("low".."max"); vacua = ordinarium. */
vacuum
vates_petitio_conatum_ponere (
    VatesPetitio* petitio,
          chorda  conatus);

/* Summarium cogitationis reddi (display "summarized"); FALSUM =
 * ordinarium provisoris. Cogitatio OPACA manet utroque modo. */
vacuum
vates_petitio_cogitationem_monstrare (
    VatesPetitio* petitio,
             b32  monstrare);

/* Punctum cache automaticum in cauda petitionis (cache_control summum). */
vacuum
vates_petitio_caudam_signare (
    VatesPetitio* petitio,
             b32  signare);

vacuum
vates_systema_addere (
    VatesPetitio* petitio,
          chorda  textus,
             b32  signum_thesauri);

vacuum
vates_instrumentum_addere (
                  VatesPetitio* petitio,
    constans VatesInstrumentum* instrumentum);

/* Nuntius novus in fine; bloci per functiones sequentes. */
VatesNuntius*
vates_nuntium_addere (
     VatesPetitio* petitio,
       VatesMunus  munus);

/* Bloci: reddunt blocum ut vocans signum_thesauri ponere possit. */
VatesBlocus*
vates_textum_addere (
     VatesNuntius* nuntius,
           chorda  textus);

VatesBlocus*
vates_effectum_addere (
     VatesNuntius* nuntius,
           chorda  id,
           chorda  contentum,
              b32  erratum);

/* Effugia pro provisoribus: campi corporis (ultimi immisti) et capita
 * (e.g. anthropic-beta). Ita res nondum modellatae (fallbacks, betae)
 * sine mutatione vatis adhibentur. SIGNATA provisore: provisor solum
 * sua adhibet, aliena praeterit - codex qui fallbacks Anthropic ponit
 * sub provisore alio immutatus currit (sine eis). */
vacuum
vates_extra_ponere (
           VatesPetitio* petitio,
     constans character* provisor,
     constans character* clavis,
              JsonValor* valor);

vacuum
vates_caput_addere (
           VatesPetitio* petitio,
     constans character* provisor,
     constans character* titulus,
     constans character* valor);


/* ========================================================================
 * RESPONSUM
 * ======================================================================== */

nomen enumeratio {
    VATES_FINIS = 0,                /* end_turn, stop_sequence */
    VATES_FINIS_INSTRUMENTUM,       /* tool_use */
    VATES_FINIS_MAXIMUM,            /* max_tokens */
    VATES_FINIS_RECUSATIO,          /* refusal */
    VATES_FINIS_PAUSA,              /* pause_turn */
    VATES_FINIS_ALIA                /* ignota: vide causa_finis_cruda */
} VatesCausaFinis;

nomen enumeratio {
    VATES_OK = 0,
    VATES_ERROR_RETE,               /* connexio, TLS */
    VATES_ERROR_TEMPUS,
    VATES_ERROR_STATUS,             /* HTTP non CC: genus + nuntius provisoris */
    VATES_ERROR_PARSE,
    VATES_ERROR_LIMES               /* recusatio localis ante missionem */
} VatesError;

nomen structura {
    s64 input;
    s64 cache_lectum;
    s64 cache_scriptum_5m;
    s64 cache_scriptum_1h;
    s64 output;
    s64 mora_ms;
    s64 pretium;          /* micro-dollaria; -I = pretium ignotum */
} VatesUsus;

nomen structura {
               b32  successus;
        VatesError  error;
               i32  status_http;
            chorda  error_genus;          /* e.g. "overloaded_error" */
            chorda  error_nuntius;
            chorda  id;
            chorda  exemplar;
               Xar* bloci;                /* VatesBlocus* */
   VatesCausaFinis  causa_finis;
            chorda  causa_finis_cruda;
            chorda  recusatio_categoria;  /* stop_details.category */
         VatesUsus  usus;                 /* summa conatuum omnium */
               i32  conatus;              /* conatus facti */
} VatesResponsum;

/* Bloci responsi ut nuntius ASSISTENS in fine petitionis (OPACA
 * verbatim) - vicis proximae praeparatio. */
vacuum
vates_responsum_addere (
               VatesPetitio* petitio,
    constans VatesResponsum* responsum);


/* ========================================================================
 * PROVISOR
 * ======================================================================== */

/* Pretia in micro-dollariis per MTok - a VOCANTE data, numquam in
 * codice (pretia mutantur). */
nomen structura {
    constans character* exemplar;
                   s64  input;
                   s64  output;
                   s64  cache_lectum;
                   s64  cache_5m;
                   s64  cache_1h;
} VatesPretium;

nomen structura {
            MoraAngusta  tempus_ms;              /* 0 = X minuta */
                    i32  conatus_maximi;         /* 0 = III */
            MoraAngusta  mora_iterandi_ms;       /* 0 = II s */
            MoraAngusta  mora_iterandi_maxima_ms; /* limes retry-after; 0 = LX s */
    constans VatesPretium* pretia;
                    i32  pretia_numerus;
    constans character*  rationarium_via;        /* NIHIL = sine rationario */
    constans character*  herbarium_via;          /* NIHIL = sedes ordinaria
                                                  * (herbarium_sedes_ordinaria,
                                                  * hospes provisoris) */
                    b32 sine_herbario;          /* VERUM = sine captura
                                                  * (expresse; herbarium-spec-2) */
            HttpVectura vectura;                /* exsequi NIHIL = ordinaria */
} VatesOptiones;

nomen structura Vates Vates;   /* opacum */

VatesOptiones
vates_optiones_ordinariae (vacuum);

/* Officina per NOMEN provisoris (registrum compilatum, more
 * FabricaGenus): "anthropic", "fictus" (clavis neglecta); posterius
 * "openai". Nomen ignotum -> NIHIL et nuntius in stderr nomina nota
 * enumerans (error configurationis, in initio). Via ordinaria pro
 * applicationibus: provisor ex configuratione legitur.
 * clavis: octeti clavis API, a vocante lecti. Numquam scribitur in
 * rationarium, herbarium, nuntios errorum. */
Vates*
vates_aperire (
                   Piscina* piscina,
                    chorda  provisor,
                    chorda  clavis,
    constans VatesOptiones* optiones);

/* Nomen provisoris huius Vatis ("anthropic"). */
chorda
vates_provisor (
    Vates* vates);

/* Constructores proprii (publici; vates_aperire eos vocat). Optiones
 * provisori propriae, si quando necessariae (url basis, versio API),
 * campi nominati in VatesOptiones fiunt - initio, numquam in codice
 * vocantis. */
Vates*
vates_anthropic_aperire (
                   Piscina* piscina,
                    chorda  clavis,
    constans VatesOptiones* optiones);

/* Vocatio una, obstruens, cum iterationibus. propositum = signum
 * rationarii ("magister", "compactor"), numquam provisori missum.
 * Numquam NIHIL: defectus in responsum->error. */
VatesResponsum*
vates_mittere (
            Vates* vates,
     VatesPetitio* petitio,
           chorda  propositum,
          Piscina* piscina);

constans character*
vates_error_descriptio (
    VatesError error);


/* ========================================================================
 * PURA (sine rete) - pro probationibus et instrumentis
 * ======================================================================== */

/* Corpus JSON Anthropic petitionis, octeti EXACTI (deterministici). */
chorda
vates_anthropic_corpus (
    constans VatesPetitio* petitio,
                  Piscina* piscina);

/* Responsum Anthropic (status + corpus) -> responsum neutrum. */
VatesResponsum*
vates_anthropic_legere (
         i32  status_http,
      chorda  corpus,
     Piscina* piscina);


/* ========================================================================
 * FICTUS - provisor Anthropic super vecturam scriptam (probationes
 * consumptorum: sine rete, gratis). Responsa neutra in caudam ponuntur
 * et ut JSON Anthropic VERUM redduntur; petitiones missae servantur.
 * ======================================================================== */

Vates*
vates_fictus_aperire (
                   Piscina* piscina,
    constans VatesOptiones* optiones);

/* Responsum textus cum usu. */
vacuum
vates_fictus_textum (
            Vates* vates,
           chorda  textus,
        VatesUsus  usus);

/* Responsum petitionis instrumenti (stop_reason tool_use). */
vacuum
vates_fictus_instrumentum (
                   Vates* vates,
                  chorda  id,
                  chorda  titulus,
      constans character* input_json);

/* Responsum crudum: status + corpus JSON quodvis (errores, novitates). */
vacuum
vates_fictus_crudum (
                   Vates* vates,
                     i32  status_http,
      constans character* corpus_json);

i32
vates_fictus_petitiones_numerus (
    Vates* vates);

/* Corpus JSON exactum petitionis index-ae missae. */
chorda
vates_fictus_petitio (
    Vates* vates,
      i32  index);

#endif /* VATES_H */
