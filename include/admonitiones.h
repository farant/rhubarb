/* admonitiones.h - admonitiones (notificationes) generales
 * (colloquia-plan T4a): fons quisque admonitiones addit; lectae per id
 * notantur; actio = genus + argumentum (hospes interpretatur:
 * "panellus" -> T4c). Memoria in piscina; vicus statum in ramo radicis
 * servat.
 *
 * Chordae in addere COPIANTUR in piscinam indicis (fontes ex piscinis
 * transitoriis legunt). Index ordinatus: recentissima primum.
 */

#ifndef ADMONITIONES_H
#define ADMONITIONES_H

/* <aedilis corpus="lib/admonitiones.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "praeconium.h"

nomen enumeratio {
    ADMONITIO_NUNTIUS = ZEPHYRUM,   /* perfecit, informatio */
    ADMONITIO_ATTENTIO              /* te eget (licentia, error) */
} GradusAdmonitionis;

nomen structura {
                chorda id;          /* stabilis: "<fons>:<clavis>" */
                chorda fons;        /* "colloquia" */
                chorda titulus;     /* "secunda" */
                chorda corpus;      /* "Needs permission: Bash" */
                chorda genus;       /* occasio: "permissio" | "finis" |
                                     * "vitium" (genera cantualis -
                                     * tintinnabulum); vacua = nulla */
                   s64 tempus;
    GradusAdmonitionis gradus;
                   b32 lecta;
                chorda actio;       /* "panellus"; vacua = nulla */
                chorda argumentum;  /* "%17" (+ socket tmux) */
} Admonitio;

nomen structura Admonitiones Admonitiones;

Admonitiones*
admonitiones_creare (
     Piscina* piscina,
         i32  maximum);

/* addere (id iteratum: ignoratur, FALSUM); vetustissimae ultra maximum
 * eiciuntur */
b32
admonitiones_addere (
               Admonitiones* a,
         constans Admonitio* x);

/* lecta notare; FALSUM si id ignotum */
b32
admonitionem_legere_notare (
    Admonitiones* a,
          chorda  id);

i32
admonitiones_numerus (
    constans Admonitiones* a);

i32
admonitiones_non_lectae (
    constans Admonitiones* a);

/* recentissima primum; NIHIL extra fines */
constans Admonitio*
admonitio (
    constans Admonitiones* a,
                      i32  index);

/* colloquia -> admonitiones: eventus qualis (EXSPECTAT, PERFECIT,
 * StopFailure) -> admonitio cum actione "panellus" (argumentum
 * "<panellus> <socket>") et genere (licentia/input/elicitatio ->
 * permissio, Stop -> finis, StopFailure -> vitium); FALSUM si eventus
 * non qualis */
b32
admonitio_ex_eventu (
    constans PraeconiumEventus* e,
                        chorda  titulus,
                     Admonitio* x,
                       Piscina* piscina);

#endif /* ADMONITIONES_H */
