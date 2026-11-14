/* officina_dorsum.h - dorsum nativum aarch64 (M5 T7: functiones
 * integrae; T8-T9 memoriam, vocationes ad hospitem et fluitantia
 * addunt)
 *
 * Functio medullae (index conexionis) in codicem A64 vertitur et in
 * aream exsecutabilem scribitur, ab OFFICINA_DORSUM_INITIUM sursum.
 * Machinula eam per officina_effugium vocat (machinula_nativum_ponere
 * aut machinula_nativum_compilare).
 *
 * Quod dorsum nondum emittit RECUSATUR nominatim (causa_out), numquam
 * tacite male vertitur; functio recusata interpretata manet.
 *
 * Halitus ex codice nativo (sistere, divisio per zephyrum, divisio
 * superfluens) per stipitem ad officina_effugium_salire eunt cum
 * codice OfficinaDorsumSaltus; positio per officina_dorsum_positio
 * ex pc invenitur.
 *
 * Dorsum UNUM vivum per aream: creare cursorem ad INITIUM reponit -
 * codex dorsi prioris superscribitur.
 */

#ifndef OFFICINA_DORSUM_H
#define OFFICINA_DORSUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "memoria_exsecutabilis.h"
#include "officina_conexio.h"

/* codex dorsi ab hoc offset areae (infra: trampolinum effugii et
 * codex manu scriptus probationum) */
#define OFFICINA_DORSUM_INITIUM 0x10000UL

/* codex quem stipes ad officina_effugium_salire fert:
 * (genus << 32) | index (causa sistendi; aliter zephyrus) */
nomen enumeratio {
    DORSUM_SALTUS_SISTERE = 1,
    DORSUM_SALTUS_DIVISIO_ZEPHYRUM,
    DORSUM_SALTUS_DIVISIO_SUPERFLUXUS,
    DORSUM_SALTUS_PONS,     /* aedificatum per pontem halitum posuit */
    DORSUM_SALTUS_INDIRECTA /* vocatio per descriptorem non nativum */
} OfficinaDorsumSaltus;

nomen structura OfficinaDorsum OfficinaDorsum;

/* NIHIL si area aut conexio deest */
OfficinaDorsum*
officina_dorsum_creare (
                 Piscina* piscina,
        constans Conexio* conexio,
    MemoriaExsecutabilis* area);

/* functionem (index conexionis) et clausuram vocatorum compilat;
 * FALSUM + causa nominata (nihil in area mutatum quod vocari
 * possit). Iam compilata: VERUM, nihil agit. */
b32
officina_dorsum_compilare (
    OfficinaDorsum* d,
               s32  functio_index,
            chorda* causa_out);

/* introitus functionis compilatae; NIHIL si non compilata */
vacuum*
officina_dorsum_introitus (
    constans OfficinaDorsum* d,
                        s32  functio_index);

/* pc -> (functio, index planus instructionis medullae cuius codex
 * pc continet; prologus = instructio prima); FALSUM extra codicem
 * dorsi */
b32
officina_dorsum_positio (
    constans OfficinaDorsum* d,
             memoriae_index  pc,
                        s32* functio_index_out,
                        i32* instructio_out);

/* VERUM si pc in duabus instructionibus primis functionis iacet (stp
 * x29,x30; mov x29,sp): tabulatum functionis nondum positum - x29
 * adhuc vocantis est et vocans ipse in x30 stat (vitium stivae
 * exhaustae in 'stp' ipso) */
b32
officina_dorsum_ante_tabulatum (
    constans OfficinaDorsum* d,
             memoriae_index  pc);

#endif /* OFFICINA_DORSUM_H */
