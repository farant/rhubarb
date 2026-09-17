/* crusta_oraculum.h - Casus oraculi bash (P11b): enumeratio,
 * involucrum, aurum
 *
 * Fons unus casuum pro instrumento (crusta/instrumenta/oraculum.c) et
 * portis oraculum et differentia: pathologiae.sh per lectorem
 * exemplorum (clavis 'pathologiae:titulus') et plagulae FreeBSD totae
 * (clavis 'freebsd:plagula'), ordine. Involucrum 'f() {\n' + datum
 * (+ '\n' si deest) + '}\n' - bash post id 'declare -f f' legit; nos
 * idem textum parsamus et coquimus.
 *
 * AURA (probationes/fixa/crusta/oraculum/): expectata.txt (sectio
 * EXSPECTATUM plurium linearum = effusio 'declare -f f') et sanitas.txt
 * (sectiones 'status' et 'ERRATUM' unius lineae ex 'bash -n -c datum').
 * Forma lectoris exemplorum; praefatio (lineae ante '####' primum)
 * versionem bash et ambitum nominat.
 */

#ifndef CRUSTA_ORACULUM_H
#define CRUSTA_ORACULUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

#define CRUSTA_ORACULUM_EXPECTATA \
    "probationes/fixa/crusta/oraculum/expectata.txt"
#define CRUSTA_ORACULUM_SANITAS \
    "probationes/fixa/crusta/oraculum/sanitas.txt"

nomen structura {
    /* 'pathologiae:titulus' aut 'freebsd:plagula' */
    chorda clavis;
    /* casus crudus, '\0' post mensuram */
    chorda datum;
} CrustaCasusOraculi;

/* Plagulam radici relativam legere; '\0' post mensuram. NIHIL =
 * absens aut memoria. */
character*
crusta_oraculum_legere (
               Piscina* piscina,
    constans character* radix,
    constans character* via,
                   i32* mensura);

/* Xar de CrustaCasusOraculi. NIHIL = plagula absens aut memoria;
 * *causa nominat (NIHIL licet). */
Xar*
crusta_oraculum_casus (
                Piscina*  piscina,
     constans character*  radix,
     constans character** causa);

/* 'f() {\n' + datum + ['\n'] + '}\n', '\0' post mensuram. datum NIHIL
 * = memoria. */
chorda
crusta_oraculum_involvere (
    Piscina* piscina,
     chorda  datum);

/* Textus sectionis 'sectio' casus 'clavis' in auro lecto (Xar de
 * CrustaExemplum); datum NIHIL si casus aut sectio absens. */
chorda
crusta_oraculum_aurum (
          constans Xar* aurum,
                chorda  clavis,
    constans character* sectio);

#endif /* CRUSTA_ORACULUM_H */
