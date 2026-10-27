/* nomenclator.h - NOMENCLATOR (contactus; nomenclator-plan P2): index
 * personarum in latere vici. Manifestum apps/nomenclator/organum.stml
 * (genus "nomenclator"); documentum personarum per ligamen (a
 * parente, commune omnibus lateribus); status visus in ramo insulae
 * (persona_electa, pagina). Index per album (membrum__album),
 * praecepta per membrum__praecepta. */
#ifndef NOMENCLATOR_H
#define NOMENCLATOR_H

/* <aedilis corpus="lib/nomenclator.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "volumen.h"
#include "personae.h"
#include "organum_montatio.h"

/* porta hodie (vicus: horologium; probationes: dies fixa) */
nomen b32 (*NomenclatorHodie)(
    vacuum* ctx,
      Dies* hodie);

/* P6c: porta "diem in calendario aperire" (vicus: latus calendarii in
 * die dato; probationes: dies notatur); NIHIL = globulus et clavis
 * absunt */
nomen b32 (*NomenclatorDiemAperire)(
    vacuum* ctx,
       Dies dies);

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
      PersonaeDocumentum* documentum;   /* ligamen (a parente) */
        NomenclatorHodie  hodie;        /* porta (P5: aetas) */
                  vacuum* hodie_ctx;
  NomenclatorDiemAperire  dies_aperire;      /* P6c: post montationem */
                  vacuum* dies_aperire_ctx;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx;  /* privatum */
} NomenclatorMontatio;

/* montare in ramo <nomenclator id="..."> repositorii: organum ex
 * manifesto infixo, actio nomenclator.agere, figurae; superficies
 * latitudo x altitudo (0 = ordinaria); systemata NIHIL licet. P5b:
 * volumen = notae personae (membrum__charta, liber "persona/<id>" in
 * dimidio infero visus personae) */
b32
nomenclator_montare (
     NomenclatorMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
      PersonaeDocumentum* documentum,
                 Volumen* volumen,      /* P5b: NIHIL = sine notis */
        NomenclatorHodie  hodie,
                  vacuum* hodie_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = NomenclatorMontatio* */
Componens*
nomenclator_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* P5b: pulsus vici - notae refectae (visus stalus); VERUM si mutatum */
b32
nomenclator_reficere (
    NomenclatorMontatio* m);

/* vicus quies T6: VERUM dum opus aut iussum dilatum in notis pendet
 * (pulsus crebri necessarii) */
b32
nomenclator_occupatus (
    constans NomenclatorMontatio* m);

/* P5b: gestus scribae notarum Motui addicitur (focus lateris) -
 * ctx = NomenclatorMontatio* */
vacuum
nomenclator_gestum_ponere (
     Motus* motus,
    vacuum* ctx);

/* P6c: portam diei ponere (post montationem; vicus in montatione) -
 * globulus ad natalem et clavis 'c' in visu personae */
vacuum
nomenclator_portam_diei_ponere (
       NomenclatorMontatio* m,
    NomenclatorDiemAperire  porta,
                    vacuum* ctx);

/* argumentum lateris (vicus_acervo_mutare / montatio; semitae R4b):
 * semita nomenclatoris - URI ("nomenclator/7", "nomenclator/01JC9X...",
 * "nomenclator?quaestio=anna") aut cognomen: "<id>" ->
 * nomenclator/<id>, "" aut NIHIL -> index (persona_aperta sublata).
 * FALSUM si semita mala aut persona ignota (status non mutatur) */
b32
nomenclator_argumentum_ponere (
     NomenclatorMontatio* m,
      constans character* argumentum);

/* comites P5 (facies.advocatum; ctx = NomenclatorMontatio*): prompta
 * "quaestio" (quaestione commissa praeimpleta) - ut praeceptum eius */
b32
nomenclator_quaestionem_aperire (
    vacuum* ctx);

/* comites P1 (facies.locus_titulus; ctx = NomenclatorMontatio*):
 * nomen personae apertae ("Anna Rossi"), index "Contacts" */
chorda
nomenclator_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);

/* semitae R4b (facies.semitam_secuta): post semitam a vico applicatam
 * (historia) liber notarum personam apertam sequitur; D5 S3c: pagina
 * notarum semitae (notae.pagina) praevalet paginae visae ultimae.
 * FALSUM si latus nullum */
b32
nomenclator_semitam_secuta (
    vacuum* ctx);

/* persona electa: campus persona_electa si viva, aliter prima indicis;
 * chorda vacua si nulla (id in intern documenti) */
chorda
nomenclator_persona_electa (
    constans NomenclatorMontatio* m);

#endif /* NOMENCLATOR_H */
