/* dispensator.h - Ansa: derivare -> destinatio -> actio -> componere
 *
 * Possidet Motus, Derivator, arborem componentium ultimam (piscinae
 * ping-pong), focum (in insula ephemerarum: attributa 'focus' et
 * 'focus_acervus' - restitutio ea restituit), super (id sub mure).
 * REGULA STALENESS: componere post OMNEM eventum dispensatum currit;
 * pingere/rasterizare semel per quadrum (extra hanc bibliothecam).
 * Quadrum 0 componitur in creatione. Sedes quietis: ante eventum
 * quemque, si motus quiescit, effundit (<quies/> in motus.c).
 * Derivata hic: mus_intravit/exiit (super), focus_captus/amissus,
 * focus_petitus (id foci ex arbore nova absens -> radici missum).
 */

#ifndef DISPENSATOR_H
#define DISPENSATOR_H

/* <aedilis corpus="lib/dispensator.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"
#include "derivare.h"
#include "destinatio.h"
#include "actio.h"


/* ==================================================
 * Typi
 * ================================================== */

/* <componens/>: arborem logicam ex insulis et motu componit, in
 * piscinam datam; nihil scribit. */
nomen Componens* (*Componere)(
     InsulaRepositorium* repo,
        constans Motus* motus,
               Piscina* piscina,
    InternamentumChorda* intern,
                vacuum* ctx);

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Piscina* piscinae_arboris[II];   /* ping-pong */
                    i32  arbor_activa;
                Piscina* scratch;                /* per eventum */
     InsulaRepositorium* repo;
                  Motus  motus;
              Derivator  derivator;
         ActioRegistrum* actiones;
    DestinatioStrategia  strategia;
              Componere  componere;
                 vacuum* componere_ctx;
              Componens* arbor;
                 chorda  super;
                    s64  quies_ms;
                    Xar* effusio;                /* Xar de Eventus */
                    i32  compositiones;
} Dispensator;


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

vacuum
dispensator_ponere_strategiam (
            Dispensator* d,
    DestinatioStrategia  strategia);


/* ==================================================
 * Ansa
 * ================================================== */

vacuum
dispensator_tractare (
         Dispensator* d,
    constans Eventus* ev);

/* EVENTUS_NIHIL cum tempore: sedes quietis + recompositio */
vacuum
dispensator_pulsare (
    Dispensator* d,
            s64  tempus);

vacuum
dispensator_recomponere (
    Dispensator* d);


/* ==================================================
 * Lectio et focus
 * ================================================== */

Componens*
dispensator_arbor (
    constans Dispensator* d);

chorda
dispensator_focus (
    Dispensator* d);

vacuum
dispensator_focus_ponere (
     Dispensator* d,
          chorda  id);

vacuum
dispensator_focus_impellere (
     Dispensator* d,
          chorda  id);

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
