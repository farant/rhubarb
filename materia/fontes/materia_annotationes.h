/* materia_annotationes.h - Collector annotationum STML in commentariis
 * (project-specs/excusatio-declarata-spec.md par. III; desideratum
 * 01M2SD93PV)
 *
 * Soror materiae eius quod silva_annotationes.h pro fronte C89 agit.
 * Commentarium cuius contentum primum non-spatium '<' est ANNOTATIO
 * est: corpus totum (praefixo decorationis exuto) ad stml_legere it.
 * Cetera PROSA sunt et omittuntur. Commentarium quod '<' fert sed
 * parsari nequit RETINETUR (parsata FALSUM cum sede et statu) -
 * numquam tacite prosa reclassificatum: consumptor errare nequit.
 *
 * ==================================================
 * AMBITUS: CUR SEDES NODI, NON SEDES COMMENTARII
 * ==================================================
 *
 * Trivia EXTRA sedes nodorum iacent. Mensuratum 2026-09-18 super
 * plagula plantata:
 *
 *   <conditio sedes="3:1-5:3" octeti="96-149">
 *     <crusta-reservatum octeti="96-98"><ante>
 *       <crusta-commentum! octeti="12-95">
 *
 * Commentarium octetos 12-95 tenet; nodus cui adnexum est ab octeto
 * 96 incipit. Etiam radix ('programma') ab 96 incipit - trivia
 * principalis nodo NULLI in sede est. Ergo CONTINENTIA possessorem
 * invenire NON potest, et materia_tractus_nodi id ipsum dicit:
 * "trivia loci non sunt, ergo commentum ducens numquam includitur".
 *
 * Sed descensus possessorem CONSTRUCTIONE novit: lexema in loco nodi
 * sedet, et trivia in lexemate. Ergo nulla ars ancestralis, nulla
 * conventio nominum ('tok_*' contra 'crusta-*') portans.
 *
 * Hinc 'scopus' = sedes NODI POSSIDENTIS, et ambitus excusationis
 * granularitatem GRAMMATICAE habet: commentarium ante 'if' totam
 * conditionem tegit, commentarium intra imperium illud solum.
 * EXCLUSIO PER PLAGULAM FIERI NON POTEST - proprietas, non lacuna.
 *
 * ==================================================
 * QUOD NON DECLARATUR
 * ==================================================
 *
 * Quod genus commentarium sit IAM declaratum est: munus
 * MATERIA_MUNUS_COMMENTUM in lexico (materia_lexicon.h), quod
 * clientes omnes sex iam ferunt. Sola DECORATIO deest, et chorda una
 * est, ergo argumentum est, non tabula cocta (divergentia B plani).
 * Signum nominatum ad tabulam aedificandam: cliens cuius commentaria
 * BLOCI sunt (clausura + exuvia continuationis per lineam).
 */

#ifndef MATERIA_ANNOTATIONES_H
#define MATERIA_ANNOTATIONES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"


/* ==================================================
 * Annotatio collecta
 * ================================================== */

nomen structura {
    /* fons commentarii */
    chorda textus;   /* corpus purgatum (praefixo et spatiis exutis) */
    chorda crudum;   /* valor lexematis integer */

    /* sedes COMMENTARII ipsius - hic diagnosticum excusationis
     * mortuae ponitur */
    MateriaTractus commentarium;

    /* possessor et AMBITUS */
    constans MateriaNodus* nodus;
           MateriaTractus  scopus;

    /* fructus stml_legere super textu purgato */
           b32  parsata;
     StmlNodus* arbor;    /* elementum primum; NIHIL si fractum */
    StmlStatus  status;
           i32  linea_erroris;     /* a linea PRIMA commentarii, I-basata */
           i32  columna_erroris;

    /* appensa (silva-migratio T11, 2026-09-25) */
     StmlNodus* documentum;   /* radix documenti (liberi = elementa
                               * omnia - commentarium unum plura ferre
                               * potest) */
        chorda error;        /* nuntius stml_legere; vacuus si parsata */
} MateriaAnnotatio;


/* ==================================================
 * Decoratio commentarii (silva-migratio T11, 2026-09-25)
 *
 * Quod circa corpus annotationis stat, per formam commentarii:
 * delimitator apertus (crusta '#', C89 bini aperientes), claudens
 * (C89 bini claudentes; NIHIL = linearis) et nota continuationis
 * quae post quamque lineam novam, spatiis ducentibus, exuitur cum
 * spatio uno sequente (C89 '*'). Spec excusatio par. IV hoc ut
 * 'signum nominatum' differebat - cliens cuius commentaria BLOCI
 * sunt; silva is cliens est.
 * ================================================== */

nomen structura {
     constans character* aperitio;     /* NIHIL = nullus */
     constans character* clausura;     /* NIHIL = linearis */
              character  continuatio;  /* '\0' = nulla */
} MateriaDecoratio;

/* Commentarium UNUM ad annotationem. ANCORA: primum non-spatium
 * (spatia, tabulae, lineae novae, nota continuationis exutae) est
 * '<' cum LITTERA sequente - prosa ut '< 5' numquam annotatio est
 * (lex silvae, 2026-09-25 in materiam lata; in domo nullum
 * commentarium aliter iudicatur).
 *
 * FALSUM = PROSA: 'annotatio' non tangitur. VERUM = annotatio
 * (parsata aut non): textus (ab ancora, delimitatoribus et
 * continuationibus exutis), crudum, parsata, arbor, documentum,
 * status, linea_erroris/columna_erroris, error impleta; sedes et
 * possessor VOCANTIS sunt. Parsatio super corpus purgatum TOTUM fit
 * (spatiis et lineis ducentibus servatis), ergo linea fontis erroris
 * = linea commentarii + linea_erroris - I; columnae sub decoratione
 * exuta moventur. Memoria deficiente annotatio RETINETUR non parsata
 * (numquam tacite prosa). 'intern' NIHIL licet (creatur). */
b32
materia_annotationem_legere (
                      Piscina* piscina,
                       chorda  valor,
    constans MateriaDecoratio* decoratio,
          InternamentumChorda* intern,
             MateriaAnnotatio* annotatio);


/* ==================================================
 * API
 * ================================================== */

/* Annotationes arboris totius. Descensus locorum (REFERENTIA numquam
 * descensa - arbor circuitum haberet); in lexemate quoque trivia ANTE
 * et POST inspiciuntur; commentaria per MATERIA_MUNUS_COMMENTUM
 * inventa.
 *
 * 'praefixum' delimitator apertus exuendus ("#" crustae); NIHIL aut
 * vacuum = nullus. Purgatio et ancora: materia_annotationem_legere.
 *
 * 'origo' uncus sedium (NIHIL licet); 'intern' NIHIL licet (collector
 * suum creat).
 *
 * Xar de MateriaAnnotatio (per valorem), ordine descensus; NIHIL si
 * argumentum necessarium abest aut memoria defecit. Xar VACUUM sanum
 * est (plagula sine annotationibus). */
Xar*
materia_annotationes_colligere (
                       Piscina* piscina,
         constans MateriaNodus* radix,
  constans MateriaLexiconRatum* lexicon,
            constans character* praefixum,
    constans MateriaOrigoUncus* origo,
           InternamentumChorda* intern);

/* Idem cum decoratione PLENA (commentaria bloci: css, C89).
 * materia_annotationes_colligere = haec cum decoratione
 * { praefixum, NIHIL, '\0' }. decoratio NIHIL = nulla. */
Xar*
materia_annotationes_decoratione_colligere (
                       Piscina* piscina,
         constans MateriaNodus* radix,
  constans MateriaLexiconRatum* lexicon,
     constans MateriaDecoratio* decoratio,
    constans MateriaOrigoUncus* origo,
           InternamentumChorda* intern);

#endif /* MATERIA_ANNOTATIONES_H */
