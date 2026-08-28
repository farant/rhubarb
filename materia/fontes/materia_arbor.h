/* materia_arbor.h - Arbor <-> STML canonicum
 *
 * Ambulatio grammaticae-IGNARA: forma ex registro cocto sumitur,
 * nulla scientia linguae hic. Quod frons privatum habet - campi
 * lexematis proprii, catenae originis - per UNCUM venit, numquam
 * per ramum in hoc modulo.
 *
 * ============================================================
 * DUAE SUTURAE, ET CUR DUAE
 * ============================================================
 *
 * MateriaOrigoUncus (materia_nodus.h) respondet quaestionibus de
 * SEDE et de RADICE emissionis - quae materia ipsa poscit ut
 * ancoram ponat et lexemata derivata agnoscat.
 *
 * MateriaArborFrons (hic) respondet quaestioni ALIA: quid frons in
 * DOCUMENTUM scribat quod materia nescit. C89 tria habet - campum
 * 'standard', scissuras, catenam originis nestatam - quorum nullum
 * materiae quicquam significat. Uncus dicit UBI lexema sit; frons
 * dicit QUID DE EO SCRIBENDUM SIT.
 *
 * Distinctio non aesthetica est: sedes materiae opus est (ancora
 * sine ea mentitur), ornamentum non est. Si ambo unus uncus essent,
 * lingua sine derivatione uncum implere deberet ut ancoram haberet.
 *
 * ============================================================
 * LEX ANCORAE (portata ex silva_arbor.c:715-760, MENSURATA)
 * ============================================================
 *
 * ANCORA EST UBI EMISSIO INCIPIT, NON UBI LEXEMA EST. Emissio
 * TRIVIIS DUCENTIBUS incipit, ergo si lexema primum spatia_ante
 * fert, ancora ex TRIVIO PRIMO sumenda est. Aliter lector cursorem
 * ad lexema ponit, deinde trivia ante id emittit, et sedes omnes
 * longitudine indentationis labuntur.
 *
 * Silva id mensuravit: lexemate solo adhibito, CLXXVIII
 * divergentiae 'lexema/offset' super corpus. Probatio parva id NON
 * cepit, quia 'int n = 0;' lexema primum ad offset 0 sine ullo
 * trivio ducente habet - casus in quo vitium EVANESCIT. Portatur
 * hic cum eadem cautione: probatio quae ancoram sine triviis
 * ducentibus solum probat NIHIL de hac lege dicit.
 */

#ifndef MATERIA_ARBOR_H
#define MATERIA_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "stml.h"
#include "internamentum.h"
#include "materia_token.h"
#include "materia_nodus.h"
#include "materia_registrum.h"
#include "materia_lexicon.h"

#define MATERIA_ARBOR_TAG_CAPACITAS   64
#define MATERIA_ARBOR_TAG_ANTE        "ante"
#define MATERIA_ARBOR_TAG_POST        "post"
#define MATERIA_ARBOR_TAG_ENVOLUCRI   "arbor"
#define MATERIA_ARBOR_SIGILLI_LONGITUDO 8


/* ==================================================
 * Fructus
 * ================================================== */

nomen structura {
    constans vacuum* clavis;      /* MateriaNodus* aut MateriaToken* */
                b32  est_lexema;
                i32  initium;     /* offset octetorum, INCLUSIVUS */
                i32  finis;       /* offset octetorum, EXCLUSIVUS */
} MateriaArborSedes;

/* Census compressionis - ambulatio tegumentum SUUM metiatur.
 * Familia templorum mortua exitum VALIDUM incompressum relinquit:
 * portae octetorum virides manent, sola magnitudo crescit. Ergo
 * PRAESENTIA asserenda est, numquam numeri pincti. */
nomen structura {
    i32 spatia_vocationes;
} MateriaArborCensus;

nomen structura {
                       b32  successus;
                    chorda  textus;   /* octeti STML; vacua in fractura */
        constans character* causa;
    constans MateriaNodus*  sedes;
                       Xar* sedes_valorum;   /* MateriaArborSedes */
        MateriaArborCensus  census;
} MateriaArborScriptura;


/* ==================================================
 * Scriptor - opacus; frons per accessus agit
 * ================================================== */

nomen structura MateriaArborScriptor MateriaArborScriptor;

Piscina*
materia_arbor_scriptor_piscina (MateriaArborScriptor* scriptor);

InternamentumChorda*
materia_arbor_scriptor_intern (MateriaArborScriptor* scriptor);

/* Causam fracturae ponere (prima vincit, ut in silva). */
vacuum
materia_arbor_scriptor_recusare (
    MateriaArborScriptor* scriptor,
      constans character* causa);

/* Attributum numeri - frons id saepissime poscit; sine eo omnis
 * frons decimale suum sine stdio rescriberet. */
b32
materia_arbor_attributum_numeri (
    MateriaArborScriptor* scriptor,
               StmlNodus* elementum,
      constans character* titulus,
                     i32  numerus);

/* Tag lexematis ex genere mangulatum (praefixum lexici + titulus
 * minusculus, '_' -> '-'). ZEPHYRUM = genus ignotum. */
i32
materia_arbor_lexema_tag (
    constans MateriaLexiconRatum* ratum,
                             s32  genus,
                      character*  buffer,
                             i32  capacitas);

/* Inversum, per mangulationem ANTRORSAM omnium generum - ergo
 * directiones divergere NON possunt. Reddit numerus_generum si
 * ignotum. */
s32
materia_arbor_lexema_ex_tag (
    constans MateriaLexiconRatum* ratum,
             constans character*  tag,
                             i32  mensura);


/* ==================================================
 * Frons - quod lingua de lexemate scribit quod materia nescit
 * ================================================== */

nomen structura {
    vacuum* datum;

    /* Passus I: lexemata quae ORIGO huius nominat quoque numeranda
     * sunt (invocatio una plura lexemata expansa gignere potest;
     * si quodque suam copiam scriberet, identitas duplicata
     * mentiretur). Frons ea per 'numerare' reddit. NIHIL = nulla. */
    vacuum (*origo_numerare)(
        vacuum* datum,
        constans MateriaToken* lexema,
        vacuum (*numerare)(vacuum* ctx, constans MateriaToken*),
        vacuum* ctx);

    /* Passus II, DUO uncus quia ORDO OCTETIM VISIBILIS EST.
     *
     * Attributa et liberi listae SEPARATAE sunt, ergo unus uncus
     * post omnia non sufficeret: silva 'standard' ANTE 'f' scribit
     * et scissuras/originem POST <post>. Attributa igitur PRIMA
     * vocantur, liberi ULTIMI, et ordo intra utramque listam
     * silvae congruit.
     *
     * Ordo attributorum: [frons] standard · [materia] f, b, linea,
     *   columna, linea-initium
     * Ordo liberorum:    [materia] ante, textus, post ·
     *   [frons] scissurae, origo
     *
     * Uterque FALSUM in fractura; causam per
     * materia_arbor_scriptor_recusare ponat. NIHIL = nihil addendum. */
    b32 (*attributa_ornare)(
        vacuum* datum,
        MateriaArborScriptor* scriptor,
        StmlNodus* elementum,
        constans MateriaToken* lexema);

    b32 (*liberos_ornare)(
        vacuum* datum,
        MateriaArborScriptor* scriptor,
        StmlNodus* elementum,
        constans MateriaToken* lexema);
} MateriaArborFrons;


/* ==================================================
 * Sigillum registri
 * ================================================== */

chorda
materia_arbor_sigillum (
                          Piscina* piscina,
    constans MateriaRegistrumCoctum* tabularium);

s32
materia_arbor_genus_index (
    constans MateriaRegistrumCoctum* tabularium,
                 constans character* titulus,
                                i32  mensura);


/* ==================================================
 * Consilium + ingressus
 * ================================================== */

nomen structura {
    constans MateriaRegistrumCoctum* tabularium;
       constans MateriaLexiconRatum* lexicon;
                 constans character* grammatica;  /* nomen in involucro */
          constans MateriaOrigoUncus* origo;      /* NIHIL licet */
           constans MateriaArborFrons* frons;     /* NIHIL licet */
                InternamentumChorda*  intern;     /* NIHIL = pigre */
                                 b32  templa_activa;
                                 b32  sedes_colligere;
} MateriaArborConsilium;

vacuum
materia_arbor_consilium_nudum (
        MateriaArborConsilium* consilium,
    constans MateriaRegistrumCoctum* tabularium,
       constans MateriaLexiconRatum* lexicon,
                 constans character* grammatica);

MateriaArborScriptura
materia_arbor_scribere_nodum (
                        Piscina* piscina,
          constans MateriaNodus* nodus,
    constans MateriaArborConsilium* consilium);

#endif /* MATERIA_ARBOR_H */
