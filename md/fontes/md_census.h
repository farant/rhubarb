/* md_census.h - Census corporis markdown (C2)
 *
 * Numeri quos arbor parsata dat, non heuristica grep: nodi per genus,
 * capitula per gradum, listae per genus et laxitatem, officia, saepta
 * per formam et linguam (lexema derivatum lingua), nexus per formam,
 * lexemata totalia et derivata, lineae, octeti. Consumptor: instrumentum
 * md/instrumenta/census.c (md/census.sh) quod md/CENSUS.md scribit -
 * mensura COMMISSA, regenerabilis, numquam asserta (corpus mutat).
 * Modus citatorum: verbatim (code spans) quaeque cum via et linea, pro
 * probatione citationum (pythonica citata).
 */

#ifndef MD_CENSUS_H
#define MD_CENSUS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "md_registrum.h"

#define MD_CENSUS_LINGUA_MENSURA 32

nomen structura {
    character lingua[MD_CENSUS_LINGUA_MENSURA];
          i32 numerus;
} MdCensusLingua;

nomen structura {
    i32  plagulae;
    i32  fractae;               /* parsura NIHIL reddens */
    s64  octeti;
    i32  lineae;                /* lexemata terminatoris md-linea */
    i32  lexemata;
    i32  derivata;              /* fons I */
    i32  genera[MD_GENUS_NUMERUS];
    i32  capitula_gradus[7];    /* [1..6] */
    i32  listae_punctatae;
    i32  listae_numeratae;
    i32  listae_laxae;
    i32  officia_aperta;
    i32  officia_perfecta;
    i32  saepta_saepta;
    i32  saepta_indentata;
    i32  nexus_inlinei;
    i32  nexus_referentes;
    i32  nexus_auto;
    Xar* linguae;              /* MdCensusLingua, ordine inventionis */
} MdCensus;

/* citatum: verbatim (code span) unum */
nomen structura {
    chorda contentum;   /* valor derivatus si adest, aliter cruda iuncta */
       i32 linea;
} MdCitatum;

vacuum
md_census_nudum (
    MdCensus* c,
     Piscina* piscina);

/* parsare et numerare; FALSUM = parsura fracta (fractae++) */
b32
md_census_addere (
              MdCensus* c,
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

/* citata plagulae unius: Xar de MdCitatum (NIHIL = parsura fracta) */
Xar*
md_census_citata (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

#endif /* MD_CENSUS_H */
