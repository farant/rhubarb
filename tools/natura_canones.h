#ifndef NATURA_CANONES_H
#define NATURA_CANONES_H

/* tools/natura_canones.h - exemplar communis inter onerationem
 * et emissionem.
 *
 * natura_canones.c corpus onerat et exemplar elementi PLICAT;
 * natura_canones_emissio.c id in litteras vertit (TSV inspectionis,
 * canonem generatum). Sceletum hic manet quia utraque pars eo
 * eget et neutra eius domina est.
 */

#include "latina.h"
#include "natura.h"
#include "stml.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>

nomen structura {
         chorda*  titulus;   /* nomen naturae, snake_case */
         chorda*  modulus;
    NaturaGenus*  genus;     /* genus ipsum, aut genus rei continentis */
      StmlNodus*  nodus;     /* nodus entis (genus aut res) */
             b32  est_res;   /* VERUM = species/individuum/cultivar */
} NcEns;

/* ==================================================
 * Exemplar elementi - apparatus PLICATUS
 *
 * natura_apparatus hereditatem iam solvit (catenam sub= trans
 * exemplaria ascendit); hic dispositio sola restat: quid
 * attributum fiat, quid liberum, quid electio.
 * ================================================== */

nomen enumeratio {
    NC_MEMBRUM_ATTRIBUTUM = I,   /* -> <attributum> */
    NC_MEMBRUM_LIBERUM    = II   /* -> <liberum> + <elementum intra=> */
} NcMembrumDiscrimen;

nomen structura {
     NcMembrumDiscrimen  discrimen;
                chorda*  titulus;       /* nomen naturae, snake */
    constans character*  praefixum;     /* "status_" vel NIHIL */
    constans character*  genus_valoris; /* "textus"/"nomen"/... */
                   Xar*  optiones;      /* Xar de chorda* - electio */
                chorda*  praestitutum;  /* ordinarius=, vel NIHIL */
} NcMembrum;

nomen structura {
      NcEns*  ens;
        Xar*  membra;     /* Xar de NcMembrum */
        Xar*  actiones;   /* Xar de chorda* - pro <eventum actio=> */
} NcElementum;

/* ==================================================
 * Emissio (natura_canones_emissio.c)
 * ================================================== */

/* nomen naturae (snake) -> nomen canonis (kebab) */
vacuum
_kebab_scribere(
    FILE*             f,
    constans chorda*  t);

/* exemplar unius elementi in TSV - modus -inspicere */
vacuum
_elementum_inspicere(
    FILE*         f,
    NcElementum*  el);

#endif /* NATURA_CANONES_H */
