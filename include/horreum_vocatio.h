/* horreum_vocatio.h - stratum REACTORIS horrei (horreum-spec II.6,
 * plan T5): opus quodlibet (horreum.h) per http_vocare in reactore
 * vocantis. Semantica eadem ac horreum_exsequi (horreum_cliens.h):
 * quisque conatus de novo signatur; RequestTimeTooSkewed semel
 * corrigitur sine mora; 412 post iterationem in opere condicionali ->
 * HEAD (x-amz-meta-sha256) = iam_nostrum; NoSuchUpload post iterationem
 * Complete -> HEAD (ETag); ceterum politica horreum_iterandum, mora per
 * timer reactoris (nihil umquam obstat aut dormit).
 *
 * Contractus:
 * - finis SEMEL, numquam intra horreum_vocare (defectus immediatus -
 *   signatio - in pulsu proximo nuntiatur);
 * - post finem nihil vocationis in reactore (timer cancellatus,
 *   http_vocatio et HEAD filia finitae);
 * - opus et quae tenet (chordae, capita, quaestio) vivere debent usque
 *   ad finem; piscina omnes conatus tenet - finis eam NE destruat
 *   (codex post reditum finis adhuc currit: dimitte in pulsu proximo);
 * - abrumpere idempotens et synchronus: finis (abrupta) ANTE reditum.
 *
 *   HorreumOpus o;
 *   horreum_opus_ponere(h, situla, clavis, corpus, NIHIL, &o, &e, p);
 *   horreum_vocare(reactor, h, &o, mea_finis, meum, NIHIL, p);
 *   ... reactor_poll ... mea_finis(meum, v, r): r->successus ... */
#ifndef HORREUM_VOCATIO_H
#define HORREUM_VOCATIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "reactor.h"
#include "http.h"
#include "http_vocatio.h"
#include "horreum.h"

/* <aedilis corpus="lib/horreum_vocatio.c"/> */

nomen structura HorreumVocatio HorreumVocatio;

nomen structura {
                b32  successus;
                b32  abrupta;       /* horreum_vocationem_abrumpere */
      HttpResponsum* responsum;     /* conatus ultimi operis; NIHIL si
                                     * transportus defecit */
    HorreumObjectum obiectum;
       HorreumError error;
                i32 conatus;       /* operis, sine HEAD filia */
                b32 iam_nostrum;
                s64 deviatio_s;    /* deviatio horologii adhibita */
} HorreumVocatioResultus;

nomen vacuum (*HorreumVocatioFinis)(vacuum* datum, HorreumVocatio* v,
    constans HorreumVocatioResultus* r);

/* NIHIL aut memset ZEPHYRUM = ordinariae */
nomen structura {
                         i32 conatus_maximi;  /* 0 = III */
                         i32 mora_basis_ms;   /* 0 = CC */
                         i64 semen;           /* sors; 0 = ex tempore */
                         s64 deviatio_s;      /* initium (resultus
                                                * prioris) */
    constans HttpVocatioOptiones* http;        /* NIHIL = ordinariae;
                                                * copiatur */
               HorreumObservator  observator;  /* NIHIL = nemo */
                          vacuum* observatoris_datum;
} HorreumVocatioOptiones;

/* vocatio incipit; NIHIL solum si reactor, opus, finis aut piscina
 * NIHIL (aut tabula timerorum plena) - ceteri defectus per finem */
HorreumVocatio*
horreum_vocare (
                            Reactor* reactor,
                            Horreum* h,
               constans HorreumOpus* opus,
                HorreumVocatioFinis  finis,
                             vacuum* datum,
    constans HorreumVocatioOptiones* optiones,
                            Piscina* piscina);

/* idempotens, synchronus: finis (abrupta) ANTE reditum vocatur; post
 * finem nihil agit */
vacuum
horreum_vocationem_abrumpere (
    HorreumVocatio* v);

b32
horreum_vocatio_finita (
    constans HorreumVocatio* v);

/* conatus operis hactenus responsi (progressus) */
i32
horreum_vocatio_conatus (
    constans HorreumVocatio* v);

#endif /* HORREUM_VOCATIO_H */
