/* conspectus.h - CONSPECTUS: exemplaria visus (view models) ex actis
 * voluminis derivata (project-specs/proiectiones.md, R5/D4a)
 *
 * Conspectus = quod visus unus pingit, DENORMALIZATUM consulto, sub
 * inscriptione 'nomen/clavis' ("dies/2026-10-19"). FUNCTIO PURA
 * fontium (computare): cache, reaedificatio et verificatio eandem
 * vocant - via secunda nulla. Genus datorum: derivata (numquam in
 * acta, numquam servatur ut veritas).
 *
 *   acta -> documenta (historia) -> conspectus -> visus
 *
 * DECLARATIO (STML, iuxta C functionis: lib/conspectus_<n>.stml):
 *
 *   <conspectus nomen="dies" versio="1">
 *     <clavis typus="dies"/>
 *     <fons documentum="agenda" irritat="per-rem"/>
 *     <fons documentum="personae" irritat="omnia"/>
 *     <fons spatium="dies/{clavis}"/>
 *     <fons pura="liturgia"/>
 *   </conspectus>
 *
 * Fontes - quae acta quas claves irritant:
 *   documentum + omnia    actum sub spatio documenti ("agenda/...",
 *                         notae revocandi incluse): claves omnes
 *   documentum + per-rem  claves rerum mutatarum, veteres et novae
 *                         (per differentiam: post acta vestigium et
 *                         claves cuiusque rei iterum computantur et
 *                         cum memoratis conferuntur - ConspectusPerRem;
 *                         vestigii friatio sola servatur)
 *   spatium               actum cuius genus (aut via plagulae) sub
 *                         spatio "dies/<clavis>/" cadit: clavis ex
 *                         spatio ipso
 *   pura                  numquam (functio clavis solius)
 * Documentum = SPATIUM suum (agenda_aperire(..., "agenda", ...)):
 * acta eius omnia, revocare et reficere quoque, sub "agenda/" cadunt.
 * Clavis per organum_typum_iudicare typo suo congruere debet.
 *
 * CURSOR (proiectiones V, emendatum D4a): registrum seq voluminis
 * tenet; ante responsum acta post cursorem legit et per genus fontibus
 * tribuit - scriptores OMNES videt (latera omnia, historiae quaevis,
 * processus alius), revocare et reficere ut acta cursoris. Irritatio
 * signum tantum ponit; computatio in lectione proxima (crebra acta,
 * computatio una).
 *
 * LEGES:
 *   - Conspectus COPIAT quod ostendit in piscinam suam (una per
 *     inscriptionem, liberata cum recomputatur aut eicitur);
 *     indicem in memoriam documenti numquam tenet.
 *   - Nec horologium nec electio nec dispositio in conspectu: hodie,
 *     dies electa, textus praecisus ad latitudinem in pictura manent.
 *   - Conspectus alium legens (intra computare) dependentiam
 *     registrat: irritatio per catenam ascendit; circulus recusatur.
 *   - Tectum in octetis (piscinae inscriptionum): ultra, minime nuper
 *     lecta eiciuntur.
 *   - Declaratio sine operibus C aut opera sine declaratione: parare
 *     recusat, nominatim.
 *   - Nulla fila (domus): computare in lectione currit.
 */

#ifndef CONSPECTUS_H
#define CONSPECTUS_H

/* <aedilis corpus="lib/conspectus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"


/* ==================================================
 * Typi
 * ================================================== */

nomen structura ConspectusRegistrum ConspectusRegistrum;   /* opacum */

/* opera C conspectus unius (nomine declarationis) */
nomen structura {
    /* conspectum clavis in piscina data computare (omnia copiata);
     * NIHIL = clavis sine conspectu (e.g. dies inexistens) */
    vacuum* (*computare)(vacuum* ctx, ConspectusRegistrum* repertorium,
                         chorda clavis, Piscina* piscina);
    /* codex canonicus (STML): verificatio confert, conservatio
     * postea servabit */
    chorda  (*codificare)(vacuum* ctx, constans vacuum* conspectus,
                          Piscina* piscina);
    vacuum* ctx;
} ConspectusOpera;

/* fons per-rem: res documenti enumerat, claves cuiusque reddit */
nomen structura {
    i32     (*numerus)(constans vacuum* documentum);
    /* identitas rei (stabilis trans acta) */
    chorda  (*id)(constans vacuum* documentum, i32 index,
                  Piscina* piscina);
    /* vestigium rei: textus qui mutatur quotiens res mutatur (e.g.
     * codex canonicus rei); vestigium mutatum = claves rei, veteres
     * et novae, irritantur */
    chorda  (*vestigium)(constans vacuum* documentum, i32 index,
                         Piscina* piscina);
    /* claves quas res tangit in claves (Xar de chorda) addere */
    vacuum  (*claves)(vacuum* ctx, constans vacuum* documentum,
                      i32 index, Xar* claves, Piscina* piscina);
    vacuum* ctx;
} ConspectusPerRem;

nomen enumeratio {
    CONSPECTUS_ABSENS = I,    /* numquam lectum aut eiectum */
    CONSPECTUS_RECENS,
    CONSPECTUS_STALUM
} ConspectusStatus;


/* ==================================================
 * Constructio
 * ================================================== */

/* registrum unum per volumen, lateribus omnibus commune. tectum
 * octetorum (0 = ordinarium); cursor = finis voluminis (cache
 * vacuum: nihil stalum). NIHIL si argumenta mala. */
ConspectusRegistrum*
conspectus_registrum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
         memoriae_index  tectum);

/* declarationem legere (<conspectus> unum; plagula altera = vocatio
 * altera) et iudicare:
 * canon conspectus + leges (fons documentum irritat poscit; spatium
 * '{clavis}' semel; nomen bis recusatur). *vitia SEMPER impletum. */
b32
conspectus_declarare (
    ConspectusRegistrum*  repertorium,
                 chorda   fons,
                    Xar** vitia);

b32
conspectus_opera_ponere (
       ConspectusRegistrum* repertorium,
        constans character* titulus,
           ConspectusOpera  opera);

/* fons per-rem conspectus 'titulus' super documentum spatii dati */
b32
conspectus_per_rem_ponere (
       ConspectusRegistrum* repertorium,
        constans character* titulus,
        constans character* spatium,
          ConspectusPerRem  per_rem);

/* documentum in memoria spatio suo ("agenda") - fontibus per-rem
 * necessarium; vocator possidet */
b32
conspectus_documentum_ligare (
    ConspectusRegistrum* repertorium,
     constans character* spatium,
                 vacuum* documentum);

/* omnia iudicare ante usum: declarationes cum operibus, per-rem cum
 * functionibus et documento ligato. FALSUM et *vitia nominata si
 * deest quid; post VERUM lectio licet. */
b32
conspectus_parare (
    ConspectusRegistrum*  repertorium,
                    Xar** vitia);


/* ==================================================
 * Lectio
 * ================================================== */

/* conspectus inscriptionis: cursorem promovet, deinde recens ex
 * cache aut computat. Valet usque ad lectionem proximam EIUSDEM
 * inscriptionis quae eum irritatum invenit (piscina tum demum
 * liberatur), aut eiectionem per tectum. NIHIL si nomen ignotum,
 * clavis typo non congruit, aut computare NIHIL reddit. */
constans vacuum*
conspectus_legere (
    ConspectusRegistrum* repertorium,
     constans character* titulus,
                 chorda  clavis);

/* status inscriptionis post cursorem promotum (nihil computat) */
ConspectusStatus
conspectus_status (
    ConspectusRegistrum* repertorium,
     constans character* titulus,
                 chorda  clavis);

/* cursorem promovere sine lectione; seq novum redditur */
s64
conspectus_renovare (
    ConspectusRegistrum* repertorium);


/* ==================================================
 * Verificatio et mensura
 * ================================================== */

/* inscriptiones RECENTES omnes ex nihilo recomputat et codices
 * confert. Numerus discrepantium redditur; inscriptiones eorum in
 * discrepantes (Xar de chorda 'nomen/clavis', NIHIL licet). */
i32
conspectus_verificare (
    ConspectusRegistrum* repertorium,
                    Xar* discrepantes);

/* octeti piscinarum inscriptionum (tectum his comparatur) */
memoriae_index
conspectus_summa_usus (
    constans ConspectusRegistrum* repertorium);

#endif /* CONSPECTUS_H */
