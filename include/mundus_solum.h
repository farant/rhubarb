/*
 * mundus_solum.h - tabula gressus loci: ubi corpus ambulare potest
 *
 * Locus est statua cuius nodi signati sunt (statua.h: gressibile,
 * solidum, aqua). Hinc tabula desuper struitur: craticula super
 * vestigium loci in plano x z, per cellam GENUS superficiei summae et
 * ALTITUDO eius (project-specs/mundus-spec.md IV.3). Quod summum est
 * decidit: saxum super solum cellam solidam facit, aqua super solum
 * aquam, tectum casae casam totam solidam.
 *
 * Nihil de pixelis, clavibus aut tempore scit. Purum: tabula semel
 * structa non mutatur.
 *
 * REGULAE
 *   - Nodi in positura quietis. Nodus sine signo praeteritur (ornatus).
 *   - Nodus gressibilis GRESSIBILE dat etiam si solidus quoque est;
 *     aqua sine gressibili AQUAM; solidum solum SOLIDUM.
 *   - Per cellam triangulum summum ad centrum cellae vincit; altitudine
 *     pari superficies non gressibilis vincit.
 *   - Triangula verticalia (parietes rupium) nihil tegunt: rupes est
 *     saltus altitudinis inter cellas vicinas.
 *   - Gradus: corpus ab altitudine ad altitudinem transit si
 *     differentia non excedit ASCENSUS_MAXIMUS + CLIVUS_MAXIMUS *
 *     distantia. Scalae (gradus dimidiae unitatis) transeunt, rupes
 *     (IV unitates) non.
 */

#ifndef MUNDUS_SOLUM_H
#define MUNDUS_SOLUM_H

/* <aedilis corpus="lib/mundus_solum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "statua.h"

#define MUNDUS_CELLAE_PER_UNITATEM  IV
/* ascensus per unitatem itineris */
#define MUNDUS_CLIVUS_MAXIMUS       1.5f
/* gradus unus (scalae) quem corpus sine itinere superat */
#define MUNDUS_ASCENSUS_MAXIMUS     0.75f
/* latus tabulae maximum (cellae); locus maior recusatur */
#define MUNDUS_SOLI_LATUS_MAXIMUM   4096

nomen enumeratio {
    MUNDUS_SOLUM_NIHIL = ZEPHYRUM,   /* extra locum aut vacuum */
    MUNDUS_SOLUM_GRESSIBILE,
    MUNDUS_SOLUM_AQUA,
    MUNDUS_SOLUM_SOLIDUM
} MundusSoliGenus;

nomen structura {
    f32  origo_x;       /* angulus cellae (0,0) in mundo */
    f32  origo_z;
    i32  latitudo;      /* cellae secundum x */
    i32  altitudo;      /* cellae secundum z */
     i8* genera;        /* MundusSoliGenus per cellam, ordo z primus */
    f32* altitudines;   /* y superficiei summae per cellam */
} MundusSolum;

/*
 * mundus_solum_struere - tabulam ex loco struere
 *
 * @locus: statua finita (statua_finire)
 *
 * Statua sine nodis signatis: tabula vacua (latitudo et altitudo 0),
 * omnia NIHIL.
 *
 * Redde: NIHIL si argumenta NIHIL, locus nimis magnus aut piscina
 * deficit
 */
MundusSolum*
mundus_solum_struere (
    constans Statua* locus,
            Piscina* piscina);

/* genus cellae in qua (x, z) iacet; extra tabulam NIHIL */
MundusSoliGenus
mundus_solum_genus (
    constans MundusSolum* s,
                     f32  x,
                     f32  z);

/*
 * mundus_solum_altitudo - altitudo soli sub (x, z)
 *
 * Inter centra cellarum interpolatur, sed solum per cellas quas
 * gradus unus iungit: scalae clivus fiunt, margo rupis non descendit.
 *
 * Redde: FALSUM si (x, z) non gressibile (*y non scribitur)
 */
b32
mundus_solum_altitudo (
    constans MundusSolum* s,
                     f32  x,
                     f32  z,
                     f32* y);

/*
 * mundus_solum_stare - corpus radii dati in (x, z) stare potest?
 *
 * Cella centri et omnes cellae quarum centrum intra radium a centro
 * CELLAE illius iacet gressibiles sunt et gradu a centro attinguntur
 * (corpus nec in parietem rupis intrat nec super marginem eius
 * pendet). Vestigium a centro cellae metitur, non a puncto ipso:
 * iudicium intra cellam idem est, ergo corpus secundum parietem
 * rectum sine haesitatione labitur.
 */
b32
mundus_solum_stare (
    constans MundusSolum* s,
                     f32  x,
                     f32  z,
                     f32  radius);

/* ab (x0, z0) ad (x1, z1): in fine stare licet et altitudo gradu
 * attingitur. Initium non gressibile: FALSUM. */
b32
mundus_solum_licet (
    constans MundusSolum* s,
                     f32  x0,
                     f32  z0,
                     f32  x1,
                     f32  z1,
                     f32  radius);

/*
 * mundus_solum_initium - locus initii: centrum cellae medio tabulae
 * proximae ubi corpus stare potest
 *
 * Redde: FALSUM si nulla talis cella
 */
b32
mundus_solum_initium (
    constans MundusSolum* s,
                     f32  radius,
                     f32* x,
                     f32* z);

/*
 * mundus_solum_attingere - cellae quas corpus ab (x, z) ambulando
 * attingit
 *
 * Per cellas vicinas (IV directiones) it, quaque transitione inter
 * centra cellarum per mundus_solum_licet probata. visa[cella] = I
 * ubi attingitur, 0 aliter (ordo idem ac genera).
 *
 * @visa:    latitudo * altitudo octeti, a vocante praebiti
 * @piscina: memoria laboris (acervus cellarum)
 *
 * Redde: numerus cellarum attactarum; 0 si corpus in (x, z) stare
 * non potest, argumenta NIHIL aut memoria deficit (visa tunc tota 0,
 * nisi ipsa NIHIL)
 */
i32
mundus_solum_attingere (
    constans MundusSolum* s,
                     f32  x,
                     f32  z,
                     f32  radius,
                      i8* visa,
                 Piscina* piscina);

#endif /* MUNDUS_SOLUM_H */
