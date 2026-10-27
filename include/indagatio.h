/* indagatio.h - INDAGATIO: quaestio textus per omnia genera datorum
 * (project-specs/indagatio.md, genera-datorum D5)
 *
 * Genus quaerendum = declaratio conspectus cum <quaerendum genus=".."/>
 * (conspectus.h): computare eius IndagatioConspectus reddit (ordines
 * indicis clavis unius), enumerare claves omnes. Indagatio genera ex
 * registro ipsa invenit: auctor generis codicem indicis nullum
 * scribit.
 *
 *   documenta -> conspectus quaerendus (clavis) -> ordines -> FTS5
 *
 * LEGES:
 *   - Index DERIVATUS: in memoria, aedificatus quaestione prima,
 *     numquam in volumine.
 *   - Pigritia: auditor claves irritatas sordidas notat; ordines earum
 *     ante quaestionem proximam rescribuntur.
 *   - Verba plana: spatiis divisa, quodque citatum cum asterisco
 *     praefixi; operatores FTS5 numquam - nulla quaestio est error
 *     syntaxis. Verba nulla = fructus vacuus.
 *   - Fructus per greges (genera), ordine inventi optimi (bm25, titulus
 *     gravior) aut alphabetice.
 *   - Nulla fila: omnia in vocatione.
 */

#ifndef INDAGATIO_H
#define INDAGATIO_H

/* <aedilis corpus="lib/indagatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "conspectus.h"


/* ==================================================
 * Typi
 * ================================================== */

nomen structura Indagatio Indagatio;   /* opacum */

/* ordo indicis unus = inventum unum (res, pagina, persona) */
nomen structura {
    chorda titulus;     /* "19 Oct - pagina 2 de 3" */
    chorda contextus;   /* "perfecta 1 Oct" (non quaeritur) */
    chorda corpus;      /* textus quaerendus */
    chorda semita;      /* "calendarium/dies/2026-10-19?..." */
} IndagatioOrdo;

/* conspectus generis quaerendi: ordines clavis unius (vacuus licet:
 * res deleta); omnia in piscina conspectus */
nomen structura {
    Xar* ordines;       /* IndagatioOrdo */
} IndagatioConspectus;

nomen structura {
    chorda textus;
       b32 congruit;    /* VERUM: verbum quaesitum (signandum) */
} IndagatioPars;

nomen structura {
    chorda  titulus;
    chorda  contextus;
    chorda  semita;
       Xar* excerptum;   /* IndagatioPars */
} IndagatioInventum;

nomen structura {
    chorda  genus;       /* titulus gregis ("Agenda") */
    chorda  conspectus;  /* nomen declarationis - filtrum semitae */
       i32  numerus;     /* inventa omnia gregis */
       Xar* inventa;     /* IndagatioInventum: capacitas prima aut
                         * pagina */
} IndagatioGrex;

nomen enumeratio {
    INDAGATIO_ORDO_OPTIMUS = I,     /* grex inventi optimi primus */
    INDAGATIO_ORDO_ALPHABETICUS
} IndagatioOrdoGregum;

nomen structura {
                 chorda verba;      /* quod persona scripsit */
                 chorda conspectus; /* vacua = greges omnes */
                 chorda clavis;     /* comites P5b: vacua = claves
                                     * omnes; aliter ordines clavis
                                     * huius solae (liber comes unus) */
                    i32 pagina;     /* 0 = prima (grege uno) */
                    i32 capacitas;  /* inventa per gregem aut paginam */
    IndagatioOrdoGregum ordo;
} IndagatioPetitio;

nomen structura {
     Xar* greges;        /* IndagatioGrex; vacua sine verbis */
     s64  versio;        /* versio indicis quaesiti */
} IndagatioFructus;

nomen structura {
    i64 ordines;        /* ordines in indice */
    i64 rescripta;      /* ordines rescripti a creatione (pigritia) */
    s64 versio;         /* crescit cum rescriptione quavis */
    b32 aedificatus;
} IndagatioMensura;


/* ==================================================
 * Genera
 * ================================================== */

/* codex canonicus communis (opera.codificare generum quaerendorum) */
chorda
indagatio_conspectum_codificare (
             vacuum* ctx,
    constans vacuum* conspectus,
            Piscina* piscina);


/* ==================================================
 * Index
 * ================================================== */

/* super registrum PARATUM: genera quaerenda invenit, auditores ponit;
 * index nondum aedificatur. NIHIL si argumenta mala. */
Indagatio*
indagatio_creare (
                Piscina* piscina,
    ConspectusRegistrum* repertorium);

/* indicem claudit, auditores tollit, arenas reddit */
vacuum
indagatio_destruere (
    Indagatio* ix);

/* renovat, aedificat si nondum, sordidas rescribit, deinde quaerit.
 * NIHIL si index deficit (causa: indagatio_error). */
IndagatioFructus*
indagatio_quaerere (
                    Indagatio* ix,
    constans IndagatioPetitio* petitio,
                      Piscina* piscina);

/* sine quaestione (pulsus otiosus): ordines rescripti redduntur */
i32
indagatio_renovare (
    Indagatio* ix);

/* expressio MATCH ex verbis planis; vacua si verba nulla */
chorda
indagatio_verba_formare (
      chorda  verba,
     Piscina* piscina);

/* index ex nihilo in alterum aedificatur et ordo per ordinem
 * confertur; numerus discrepantium (inscriptiones in discrepantes,
 * NIHIL licet) */
i32
indagatio_verificare (
    Indagatio* ix,
          Xar* discrepantes);

IndagatioMensura
indagatio_mensura (
    constans Indagatio* ix);

/* causa erroris ultimi; NIHIL si nullus */
constans character*
indagatio_error (
    constans Indagatio* ix);

#endif /* INDAGATIO_H */
