/*
 * mundus.h - mundus: corpora in solo, gradu temporis promota
 *
 * Status mundi parvi (project-specs/mundus-spec.md IV.4): locus unus
 * (tabula gressus, mundus_solum.h) et corpora. Corpus locum in solo,
 * faciem et INTENTUM habet: directionem in quam ambulare vult, aut
 * nullam. mundus_gradi omnia corpora spatio temporis promovet.
 *
 * Mundus nescit QUIS intentum ponat: latus ex clavibus (persona
 * lusoris), postea ratio vagandi (bestiae). Nec pixela nec claves nec
 * horologium novit - numeris solis probatur.
 *
 * REGULAE GRADUS
 *   - dt cogitur ad [0, MUNDUS_GRADUS_MAXIMUS]: mora longa corpus non
 *     transfert.
 *   - Intentum normalizatur: obliquum non celerius quam rectum.
 *   - Motus in particulas MUNDUS_PASSUS dividitur; particula tota
 *     temptatur, deinde secundum x solum, deinde secundum z solum
 *     (corpus secundum parietem labitur).
 *   - Facies ad intentum vertitur via breviore, MUNDUS_VERSURA gradus
 *     per secundam, etiam dum corpus obstructum est.
 *   - Corpora inter se non obstant (gradus hic).
 */

#ifndef MUNDUS_H
#define MUNDUS_H

/* <aedilis corpus="lib/mundus.c"/> */

#include "latina.h"
#include "mundus_solum.h"

#define MUNDUS_CELERITAS         4.0f    /* unitates per secundam */
#define MUNDUS_VERSURA           720.0f  /* gradus per secundam */
#define MUNDUS_GRADUS_MAXIMUS    0.1f    /* secundae per gradum */
#define MUNDUS_PASSUS            0.125f  /* unitates: motus in
                                          * particulas dividitur */
#define MUNDUS_CORPORIS_RADIUS   0.5f
#define MUNDUS_CORPORIS_ALTITUDO 2.0f

nomen structura {
    f32 x;              /* locus in solo */
    f32 z;
    f32 y;              /* altitudo soli sub corpore */
    f32 facies;         /* gradus in [0, 360); 0 = +z, crescit
                         * versus +x */
    f32 radius;
    b32 intendit;       /* VERUM: ambulare vult */
    f32 intentum_x;     /* directio in mundo; longitudo neglegitur
                         * (nulla = non ambulat) */
    f32 intentum_z;
    b32 movetur;        /* gradu ultimo locum mutavit */
    f32 tempus_motus;   /* secundae motus continui (tempus cycli
                         * ambulandi); 0 dum quiescit */
    f32 tempus_quietis; /* secundae quietis continuae (tempus cycli
                         * quietis); 0 dum movetur */
    s32 forma;          /* index vocantis (quae statua); mundus eum
                         * non legit */
} MundusCorpus;

nomen structura {
    constans MundusSolum* solum;
            MundusCorpus* corpora;
                     i32  corporum;
} Mundus;

/*
 * mundus_corpus_ponere - corpus in loco gressibili ponere
 *
 * x, z et y corporis scribuntur; motus, tempus_motus et
 * tempus_quietis purgantur;
 * radius, facies, intentum et forma non tanguntur.
 *
 * Redde: FALSUM si argumenta NIHIL aut corpus ibi stare non potest
 * (corpus tunc non mutatur)
 */
b32
mundus_corpus_ponere (
    constans Mundus* m,
       MundusCorpus* c,
                f32  x,
                f32  z);

/*
 * mundus_gradi - mundum dt secundis promovere
 *
 * dt negativum aut non numerus: 0 (facies et locus manent; movetur
 * FALSUM fit). Mundus sine solo: nihil. Corpus quod gradu non
 * movetur (non intendit aut impeditur) tempus_quietis dt auget;
 * quod movetur id purgat.
 */
vacuum
mundus_gradi (
    Mundus* m,
       f32  dt);

#endif /* MUNDUS_H */
