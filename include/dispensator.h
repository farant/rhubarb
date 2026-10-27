/* dispensator.h - Ansa: derivare -> destinatio -> actio -> componere
 *
 * Possidet Motus, Derivator, arborem componentium ultimam (piscinae
 * ping-pong), focum (in insula, ramo activo: attributa 'focus' et
 * 'focus_acervus' - restitutio ea restituit), super (id sub mure).
 * REGULA STALENESS: componere post OMNEM eventum dispensatum currit;
 * pingere/rasterizare semel per quadrum (extra hanc bibliothecam).
 * Quadrum 0 componitur in creatione. Sedes quietis: ante eventum
 * quemque, si motus quiescit, effundit (<quies/> in motus.c).
 * Derivata hic: mus_intravit/exiit (super), focus_captus/amissus,
 * focus_petitus (id foci ex arbore nova absens -> radici missum).
 *
 * CATENA FOCI (status-plan S8c): organum organa montare potest (via
 * "pictor:1/ludibrium:liber"). Focus rami = id componentis in spatio
 * suo, AUT segmentum liberi ("ludibrium:liber"; ':' in id componentis
 * numquam): focus intra illum ramum est. Dispensator a Motus.ramus
 * per segmenta descendit usque ad ramum cuius focus id est (aut
 * vacuus) = Motus.ramus_intimus: focus et focus_acervus ibi leguntur
 * et scribuntur, Tab ibi circuit. Ictus in componentem focusabilem
 * spatii nidificati catenam scribit (quisque ramus a Motus.ramus
 * segmentum proximum, ramus componentis id eius); ictus in gradu
 * superiore catenam ibi praecidit - focus inferiorum manet, reditus
 * eum restituit. Ramo intimo mutato pan/zoom sordida in priorem
 * effunduntur. Gestus applicationis hospitis manet (organum
 * nidificatum cum gestu proprio: postea, A6).
 * Liberum cuius spatium in arbore composita non est (hospes eum non
 * monstrat - notae calendarii extra visum diei): catena ibi
 * praeciditur et focus gradus illius in radicem spatii sui scribitur,
 * ne claves in organum invisibile cadant.
 */

#ifndef DISPENSATOR_H
#define DISPENSATOR_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

/* <aedilis corpus="lib/dispensator.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "eventus.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"
#include "derivare.h"
#include "destinatio.h"
#include "actio.h"


/* ==================================================
 * Typi
 * ================================================== */

/* <componens/>: arborem logicam ex insula et motu componit, in
 * piscinam datam; nihil scribit. */
nomen Componens* (*Componere)(
     InsulaRepositorium* repo,
        constans Motus* motus,
               Piscina* piscina,
    InternamentumChorda* intern,
                vacuum* ctx);

/* S5c (status-plan): systema medians in via eventus. ante: ante
 * mittere - VERUM = eventus consumptus, actio nulla vocatur. post: post
 * actionem quae consumpsit (actor = componens eius). Par unum
 * (dimissio); acervus effugii postea fortasse (census XIII). */
nomen structura Dispensator Dispensator;

nomen b32 (*DispensatorAnte)(
                 vacuum* ctx,
            Dispensator* d,
    constans Destinatio* des,
       constans Eventus* e);

nomen vacuum (*DispensatorPost)(
                 vacuum* ctx,
            Dispensator* d,
     constans Componens* actor,
       constans Eventus* e);

structura Dispensator {
                Piscina* piscina;
    InternamentumChorda* intern;
                Piscina* piscinae_arboris[II];   /* ping-pong */
                    i32  arbor_activa;
                Piscina* scratch;                /* per eventum */
     InsulaRepositorium* repo;
                  Motus  motus;
              Derivator  derivator;
         ActioRegistrum* actiones;
    DestinatioStrategia  strategia;      /* NIHIL = geometrica */
                 vacuum* strategia_ctx;
              Componere  componere;
                 vacuum* componere_ctx;
              Componens* arbor;
                 chorda  super;
                 chorda  super_spatium;   /* S2a: spatium componentis
                                           * sub mure */
                    s64  quies_ms;
                    Xar* effusio;                /* Xar de Eventus */
                    Xar* differenda;             /* ad limen */
                    Xar* notata;   /* notarius: Xar de EventusNotatum;
                                    * NIHIL = non notatur (A6) */
                    i32  compositiones;
        DispensatorAnte  ante;     /* S5c: NIHIL = nullum */
        DispensatorPost  post;
                 vacuum* medium_ctx;
};


/* ==================================================
 * Vita
 * ================================================== */

Dispensator*
dispensator_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
         ActioRegistrum* actiones,
              Componere  componere,
                 vacuum* componere_ctx,
                    s64  quies_ms);

/* strategia propria cum contextu; NIHIL = ordinaria
 * (destinatio_geometrica) */
vacuum
dispensator_ponere_strategiam (
            Dispensator* d,
    DestinatioStrategia  strategia,
                 vacuum* ctx);


/* ==================================================
 * Ansa
 * ================================================== */

vacuum
dispensator_tractare (
         Dispensator* d,
    constans Eventus* ev);

/* finis applicationis (S1a): quidquid pendet effunditur sine
 * quiete - pan/zoom sordida et gestus sordidus. Glutina (fenestra,
 * terminalis) ante exitum vocant; aliter pendentia perduntur. */
vacuum
dispensator_finire (
    Dispensator* d);

/* S5c: hamos medios ponere (NIHIL = tollere) */
vacuum
dispensator_medium_ponere (
        Dispensator* d,
    DispensatorAnte  ante,
    DispensatorPost  post,
             vacuum* ctx);

/* EVENTUS_NIHIL cum tempore: sedes quietis + recompositio */
vacuum
dispensator_pulsare (
    Dispensator* d,
            s64  tempus);

/* vicus quies T5: ms usque ad effusionem quietis proximam (pan/zoom
 * aut gestus sordidus, quies_ms post mutationem ultimam); ZEPHYRUM si
 * iam debita; -1 si nihil pendet. Effusio recusata tempus renovat:
 * iterum post quies_ms, numquam statim (ansa quieta aliter sine mora
 * circumiret). */
s64
dispensator_mora_proxima (
    constans Dispensator* d,
                     s64  nunc);

vacuum
dispensator_recomponere (
    Dispensator* d);

/* NOTARIUS (eventus A6; spec D6): eventa CRUDA quae dispensator
 * accipit (pulsus quoque - quies ex eis pendet) in Xar de
 * EventusNotatum appenduntur, cum destinato: eventus muris ->
 * scopus = id componentis cui traditur (captum si captura), scopus_x/y
 * in spatio eius (destinatio_ad_locale); ceteri sine scopo. Id in
 * piscinam dispensatoris copiatur (arbores piscinas reddunt).
 * NIHIL = notarius tollitur. Plagula: eventus_notata_scribere_stml. */
vacuum
dispensator_notarium_ponere (
    Dispensator* d,
            Xar* notata);


/* Sedes 'put' (brainstorm XVI §1): eventus derivatus ad id
 * directus, traditus post eventum currentem et recompositionem,
 * contra arborem NOVAM. In traditione directus: limen proximum. */
vacuum
dispensator_addressare (
     Dispensator* d,
          chorda  id,
 eventus_genus_t  genus,
             s64  tempus);


/* ==================================================
 * Lectio et focus
 * ================================================== */

Componens*
dispensator_arbor (
    constans Dispensator* d);

/* in ramo intimo (S8c) */
chorda
dispensator_focus (
    Dispensator* d);

/* in ramo intimo (S8c) */
vacuum
dispensator_focus_ponere (
     Dispensator* d,
          chorda  id);

/* in ramo intimo (S8c) */
vacuum
dispensator_focus_impellere (
     Dispensator* d,
          chorda  id);

/* in ramo intimo (S8c) */
vacuum
dispensator_focus_extrahere (
    Dispensator* d);

chorda
dispensator_super (
    constans Dispensator* d);

Motus*
dispensator_motus (
    Dispensator* d);

i32
dispensator_numerus_compositionum (
    constans Dispensator* d);

#endif /* DISPENSATOR_H */
