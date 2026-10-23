/* emendatio.h - Carmen per locum emendatum: (pars, mensura, tempus)
 * -> nota, super librum STML
 *
 * TEXTUS MANET VERITAS: frusta MML ut auctor scripsit (l8, octavae,
 * lineae '|'). Emendatio CHIRURGICA est - signum notae solum in textu
 * frusti mutatur (nodus textus in arbore STML; scriptor non-pulcher
 * reliqua octetim reddit, sonda 10-23) - deinde liber TOTUS per
 * carmina_legere iudicatur: numquam liber semi-validus.
 *
 * LOCUS "M:T" aut "M:T+P": mensura (1 = prima), tempus (1 = primum;
 * unitas = 1/denominator, 6/8 9/8 12/8: tres octavae - ut linea
 * harmoniae), P pulsus additi intra tempus (0 ordinarium).
 *
 * NOTA NOVA "G2", "Bb3", "F#4" (partitura_notam_legere - grammatica
 * partiturae inversa; C4 = MIDI LX). Octava aliena: signum '<' / '>'
 * circumdatur ("d2." -> "<g2.>") - notae sequentes octavam suam
 * servant.
 *
 * NONDUM: pausa -> nota (E2: mensuram substituere), longitudo mutata,
 * notae motae in tempore (scriptor exemplar -> MML; pagina DAW).
 */
/* <aedilis corpus="lib/emendatio.c"/> */
#ifndef EMENDATIO_H
#define EMENDATIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "carmen.h"

nomen structura {
    i32 mensura;   /* 1.. */
    i32 tempus;    /* 1.. */
    s64 pulsus;    /* additi intra tempus */
} EmendatioLocus;

/* quod mutatum est (ad monstrandum): textus frusti ante et post */
nomen structura {
       s32 frustum;   /* index in parte */
       i32 offset;    /* in textu frusti VETERE */
    chorda vetus;     /* "d2." */
    chorda novum;     /* "<g2.>" */
} EmendatioMutatio;

/* "3:2", "3:2+480". FALSUM: forma mala */
b32
emendatio_locum_legere (
            chorda  textus,
    EmendatioLocus* locus);

/* nota quae ad locum in parte INCIPIT altitudinem novam accipit,
 * longitudo manet. FALSUM (vitium impletum, *novus intactus): carmen /
 * pars ignota, locus extra metrum, nulla nota ibi incipit (causa
 * nominat notam quae ibi sonat, aut pausam), sonitus '@', nota nova
 * mala aut extra MIDI (MML: octava 0..8, alteratio una), textus non
 * crudus ('<textus!>' - MML '<' fert), liber novus iudicio cadit */
b32
emendatio_notam_ponere (
                     chorda  liber,
                     chorda  carmen,
                     chorda  pars,
    constans EmendatioLocus* locus,
                     chorda  nota,
                    Piscina* piscina,
                     chorda* novus,
           EmendatioMutatio* mutatio,
             CarminisVitium* vitium);

#endif /* EMENDATIO_H */
