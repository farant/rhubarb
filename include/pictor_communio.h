/* pictor_communio.h - communio intra pictorem (communio-plan-5 T5;
 * communio-spec IX)
 *
 * Cliens communionis pro applicatione pictoris: tessera ex filo (via
 * voluminis + ".tessera", 0600), servus ex hospite (veredarius), acta
 * advecta -> mutata; index apparatuum spatii (GET /communio/apparatus)
 * in praesentia "communio/apparatus" servatur ut attributiones ("by
 * Amanda · iPhone") etiam sine rete respondeant. Tessera recusata
 * (401: revocatus) -> desinit, filum tesserae delet, mutata - "Sign
 * in..." redit. Sine veredario (Android) nulla communio. */
#ifndef PICTOR_COMMUNIO_H
#define PICTOR_COMMUNIO_H
/* <aedilis corpus="lib/pictor_communio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "volumen.h"
#include "veredarius.h"
#include "pictor_applicatio.h"
#include "testa_cursus.h"

/* picturae advectae, index apparatuum, status inscriptionis mutatus */
nomen vacuum (*PictorCommunioMutata)(vacuum* ctx);

nomen structura PictorCommunio PictorCommunio;

/* NIHIL si veredarius (aut reactor, volumen) NIHIL. Nihil petit donec
 * incipere. */
PictorCommunio*
pictor_communionem_aperire (
               Piscina* piscina,
               Reactor* reactor,
            Veredarius* veredarius,
               Volumen* volumen,
    constans character* via_tesserae,
    constans character* basis,
  PictorCommunioMutata  mutata,
                vacuum* ctx);

/* tessera valida in filo et a servo non recusata */
b32
pictor_communio_inscripta (
    constans PictorCommunio* pc);

/* cliens incipit (si tessera; aliter nihil) et index apparatuum
 * petitur; post inscriptionem novam iterum vocatur */
vacuum
pictor_communio_incipere (
    PictorCommunio* pc);

/* resumptio (applicatio redit, rete redit): inscriptio pendens statim
 * inspicit; petitiones clientis in itinere (fortasse post somnum
 * mortuae) abrumpuntur et semitae statim iterum incipiunt */
vacuum
pictor_communio_excitare (
    PictorCommunio* pc);

/* "Persona · Titulus" apparatus replicae (ex indice servato); "" si
 * ignota */
chorda
pictor_communio_auctor (
    constans PictorCommunio* pc,
                     chorda  replica,
                    Piscina* piscina);

/* persona huius apparatus ("Amanda"); "" si ignota */
chorda
pictor_communio_persona (
    constans PictorCommunio* pc,
                    Piscina* piscina);

/* mora tractionis in secundis (probationes: 0) */
vacuum
pictor_communio_moram_ponere (
    PictorCommunio* pc,
               i32  secundae);

/* petitiones abrumpit, tesseram e memoria delet; inscriptio pendens
 * RELINQUITUR (filum et entrium manent: applicatio proxima resumit) */
vacuum
pictor_communionem_claudere (
    PictorCommunio* pc);


/* ==================================================
 * plan-5 T7: inscriptio per pictorem (CommunioInscriptio)
 * ================================================== */

/* incipit (prior non inscripta abicitur); FALSUM si tessera exstat aut
 * argumenta mala. Inscripta -> cliens incipit, mutata. */
b32
pictor_communio_inscriptionem_incipere (
        PictorCommunio* pc,
                chorda  epistula,
    constans character* titulus);

/* FALSUM (sine petitione) si codex non VIII Crockford aut nulla
 * inscriptio exspectans */
b32
pictor_communio_codicem_dare (
     PictorCommunio* pc,
             chorda  codex);

/* -1 nulla; aliter InscriptioGradus */
s32
pictor_communio_inscriptionis_gradus (
    constans PictorCommunio* pc);

/* Franus 2026-10-30: epistula ad quam codex missus est ("Sent to
 * ..."), dum inscriptio exspectat - in memoria sola; "" post
 * resumptionem (numquam servata), abiectionem, inscriptionem */
chorda
pictor_communio_epistula_missa (
    constans PictorCommunio* pc,
                    Piscina* piscina);

/* Cancel (aut post INSCRIPTA: dimittere) */
vacuum
pictor_communio_inscriptionem_abicere (
    PictorCommunio* pc);

/* pendens in volumine resumitur; VERUM si resumpta */
b32
pictor_communio_inscriptionem_resumere (
    PictorCommunio* pc);

/* probationes: mora inspectionis et vita (ms; 0 = ordinaria) */
vacuum
pictor_communio_inscriptionis_ponere (
    PictorCommunio* pc,
               i32  mora_ms,
               i32  vita_ms);

/* montationi pictoris ligare (apps/pictor/pictor.c et probationes):
 * compositio (communio_praesto, inscripta, auctor, persona, gradus
 * inscriptionis), actiones (inscriptio), mutata -> inscripta
 * renovata, Shared pictures renovatae, claviatura celata cum
 * inscripta, recompositio. machina = titulus apparatus. */
vacuum
pictor_communionem_ligare (
         PictorCommunio* pc,
         PictorMontatio* montatio,
            Dispensator* d,
     constans character* machina);


/* ==================================================
 * pulsus-plan T9: notificationes (pulsus-spec III.4)
 *
 * Acta propria a servo accepta (CommunioMissa) -> eventus "pictor.
 * picturae", res "<replica>:<spatium>" per picturam tactam, mora CCC,
 * formae "{persona} shared a picture" / "{persona} shared {numerus}
 * pictures" - SEMPER cum inscripta, etiam sine permissione in hoc
 * apparatu (notificatio aliis est).
 * ================================================== */

/* testa pulsum praebet (iOS): permissio petitur nunc si inscripta, aut
 * statim post inscriptionem - numquam ante; semel per processum.
 * Tessera -> communio_cliens_pulsum_inscribere(.., ambitus testae,
 * thema "earth.golden.pictor"), et iterum cum cliens novus (post
 * revocationem). a->pulsum_petere NIHIL (macOS, Android): nihil. */
vacuum
pictor_communio_pulsum_parare (
           PictorCommunio* pc,
    constans TestaAmbitus* a);

/* "Notify family now": eventus statim cum pictura propria 'spatium'
 * (NIHIL = sine pictura: servus fasciculum pendentem statim mittit,
 * aut nihil). FALSUM si non inscripta aut cliens nondum. */
b32
pictor_communio_pulsum_statim (
         PictorCommunio* pc,
     constans character* spatium);

/* pulsus-plan T10: notificatio pressa (tactus testae, post
 * pulsum_parare): picturae omnes notae -> spectator in grege
 * (pictor_gregem_aperire_in); aliter rete "Loading...", communio
 * excitatur, grex iterum post omne advenerunt; mora transacta (ordinarie
 * XV s) -> notae aperiuntur aut rete. plures -> rete. Probationes:
 * mora in ms. */
vacuum
pictor_communio_tactus_moram_ponere (
    PictorCommunio* pc,
               i32  ms);

#endif /* PICTOR_COMMUNIO_H */
