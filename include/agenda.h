/* agenda.h - Agenda: res agendae, dies, rubricae, notae, perfectio
 * (agenda-plan A1)
 *
 * Documentum UNUM (historia, ut pictor_documentum): veritas est ACTA
 * in volumine (genus "agenda"); res ex actis proiciuntur. Insulam non
 * novit - stratum actionum (A2+) pontem facit. Horologium numquam
 * legitur: dies "nunc" (perfectio, nota) a vocante dantur (porta
 * horologii) - replicatio semper eadem.
 *
 * Acta (dies ISO YYYY-MM-DD):
 *   <res actio="nova" id="17"><@titulus=>...</></res>
 *   <res actio="titulus" id="17"><@titulus=>...</></res>
 *   <res actio="dies" id="17" dies="2026-10-14"/>    dies absens = sine
 *                                                    die
 *   <res actio="rubricae" id="17" rubricae="domus opus"/>  copia tota
 *                                                    (absens = vacua)
 *   <res actio="nota" id="17" dies="2026-10-12">textus</res>
 *   <res actio="perfecta" id="17" dies="2026-10-13"/>
 *   <res actio="aperta" id="17"/>                    perfectio tollitur
 *   <res actio="deleta" id="17"/>
 * Id = maximum + 1, deletis numeratis (id numquam iteratur); res
 * deleta actum nullum accipit. Rubrica [a-z0-9-]+ (kebab); copia
 * ordinata sine iteratis. Titulus vacuus recusatur. Res recurrentes
 * nondum - forma eas non excludit. Titulus forma elementi attributi
 * semper scribitur ('"' in valore repraesentabilis); notae in
 * contento.
 *
 * Proiectio sine memoria fixa (historia L1): status canonicus = textus
 * <agenda proximum="18"><res .../>...</agenda> - sigillum et
 * checkpoint idem textus.
 */

#ifndef AGENDA_H
#define AGENDA_H

/* <aedilis corpus="lib/agenda.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "historia.h"
#include "fasti.h"

nomen structura {
      Dies dies;
    chorda textus;
} AgendaNota;

nomen structura {
       s32  id;
    chorda  titulus;
       b32  dies_posita;         /* FALSUM = sine die */
      Dies  dies;
       Xar* rubricae;            /* chorda, ordine canonico */
       Xar* notae;               /* AgendaNota, ordine actorum */
       b32  perfecta;
      Dies  dies_perfectionis;
} AgendaRes;

nomen structura AgendaDocumentum AgendaDocumentum;

/* documentum ex volumine (spatium "" = nudum; vacuum si nullum actum
 * - nihil scribitur donec actum primum). intervallum <= 0 -> LXIV. */
AgendaDocumentum*
agenda_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
                    i32  intervallum);

/* res nova; redde id (> 0) aut 0 si recusata */
s32
agenda_rem_creare (
    AgendaDocumentum* doc,
              chorda  titulus);

/* acta typata: redde seq (> 0) aut 0 si recusatum (res absens aut
 * deleta, titulus vacuus, rubrica mala) */
s64
agenda_titulum_ponere (
    AgendaDocumentum* doc,
                 s32  id,
              chorda  titulus);

/* dies NIHIL = sine die */
s64
agenda_diem_ponere (
    AgendaDocumentum* doc,
                 s32  id,
       constans Dies* dies);

/* rubricae spatiis separatae ("a b"); copia tota substituitur */
s64
agenda_rubricas_ponere (
    AgendaDocumentum* doc,
                 s32  id,
              chorda  rubricae);

s64
agenda_notam_addere (
    AgendaDocumentum* doc,
                 s32  id,
                Dies  dies,
              chorda  textus);

s64
agenda_perficere (
    AgendaDocumentum* doc,
                 s32  id,
                Dies  dies);

s64
agenda_perfectionem_tollere (
    AgendaDocumentum* doc,
                 s32  id);

s64
agenda_rem_delere (
    AgendaDocumentum* doc,
                 s32  id);

b32
agenda_revocare (
    AgendaDocumentum* doc);

b32
agenda_reficere (
    AgendaDocumentum* doc);

/* reproicere ex nihilo et sigilla conferre */
b32
agenda_verificare (
    AgendaDocumentum* doc);

chorda
agenda_sigillum_hex (
    constans AgendaDocumentum* doc,
                      Piscina* piscina);

/* observatores (S5b), relatio */
Historia*
agenda_historia (
    AgendaDocumentum* doc);

/* proiectio: res vivae (non deletae) ordine id. Indicia valent usque
 * ad mutationem proximam (actum, revocare, reficere). */
i32
agenda_numerus_rerum (
    constans AgendaDocumentum* doc);

constans AgendaRes*
agenda_res_index (
    constans AgendaDocumentum* doc,
                          i32  index);

/* NIHIL si absens aut deleta */
constans AgendaRes*
agenda_res (
    constans AgendaDocumentum* doc,
                          s32  id);

/* A1b: quaestiones vistarum (A2 mensis, A3 dies). Xar de
 * (constans AgendaRes*) in piscina vocantis, ordine id; indicia
 * valent usque ad mutationem proximam. */

/* res vivae diei assignatae (dies_posita et dies == dies) */
Xar*
agenda_res_diei (
    constans AgendaDocumentum* doc,
                         Dies  dies,
                      Piscina* piscina);

/* res vivae die perfectae (dies_perfectionis == dies), ubicumque
 * assignatae - "calendarium perfectionem ostendit" (decisio 4) */
Xar*
agenda_perfectae_die (
    constans AgendaDocumentum* doc,
                         Dies  dies,
                      Piscina* piscina);

/* signum diei in craticula mensis */
nomen structura {
    i32 assignatae;   /* res diei assignatae (perfectae incluse) */
    i32 apertae;      /* earum nondum perfectae */
    i32 perfectae;    /* res eo die perfectae */
} AgendaSignum;

/* signa mensis: signa[d - 1] pro die d; dies ultra mensem nulli.
 * Transitus unus super res. FALSUM si mensis malus. */
b32
agenda_signa_mensis (
    constans AgendaDocumentum* doc,
                          s32  annus,
                          s32  mensis,
                 AgendaSignum  signa[XXXI]);

#endif /* AGENDA_H */
