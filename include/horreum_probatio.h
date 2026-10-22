/* horreum_probatio.h - PROBATIO VIVA provisoris S3 (horreum-plan T6):
 * quid situla revera facit, non quid documenta dicunt.
 *
 * Sub praefixum + id + "/" (ordinarium "horreum-probatio/") probat: iter
 * PUT/GET/HEAD/DELETE in clave difficili (spatium, +, %, ?, #, &,
 * unicode, //, / finalis) et obiecto vacuo; If-None-Match: * (412 si
 * adest, 200 si abest); If-Match falsum (412); x-amz-acl public-read;
 * summae CRC32 et SHA-256 (vera accepta, FALSA RECUSATA); DeleteObjects
 * cum Content-MD5; enumeratio cum delimitatore; multipars III x V MiB
 * (et abortus); URL praesignatae GET et PUT sine Authorization. Omnia
 * quae fecit delet (obiecta et onera) et numerat quae relicta sunt.
 *
 * Horreum in modum omnia_permittere ponitur (capacitates provisoris non
 * impediunt) et horreum_cliens_parare cum vectura data vocatur - horreum
 * post probationem ad usum ordinarium non redit. Responsa (si directorium
 * datum) purgata servantur: textus elementorum ID, DisplayName et
 * Location (R2: rationarium in hospite) -> SCRUBBED.
 *
 * Numquam secretum aut signaturam scribit. */
#ifndef HORREUM_PROBATIO_H
#define HORREUM_PROBATIO_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "http.h"
#include "horreum.h"

/* <aedilis corpus="lib/horreum_probatio.c"/> */

#define HORREUM_PROBATIO_LINEAE XVI

nomen enumeratio {
    HORREUM_PROBATUM = 0,
    HORREUM_REFUTATUM,
    HORREUM_NON_PROBATUM
} HorreumVerdictum;

nomen structura {
    constans character* titulus;      /* "solum_si_abest", "iter", ... */
                   i32  capacitas;    /* HORREUM_CAP_* aut 0 */
      HorreumVerdictum  verdictum;
                chorda  causa;        /* quid visum (sine secreto) */
} HorreumProbatioLinea;

nomen structura {
    HorreumProbatioLinea lineae[HORREUM_PROBATIO_LINEAE];
                     i32 numerus;
                  chorda praefixum;          /* praefixum + id + "/" */
                  chorda caput_petitionis;   /* nomen capitis id
                                               * petitionis; vacuum =
                                               * nullum visum */
                  chorda clavis_difficilis;
                     s32 relicta;            /* obiecta + onera post
                                               * purgationem; -I =
                                               * ignotum */
} HorreumProbatio;

/* memset ZEPHYRUM = ordinariae */
nomen structura {
            HttpVectura  vectura;     /* exsequi NIHIL = http_exsequi */
                 chorda  praefixum;   /* vacuum = "horreum-probatio/" */
                 chorda  id;          /* vacuum = ex tempore */
     constans character* responsa;    /* directorium; NIHIL = nulla */
} HorreumProbandi;

/* FALSUM solum si probatio incipere nequit (situla nulla, directorium
 * responsorum); verdicta in relatione */
b32
horreum_probare (
                     Horreum* h,
    constans HorreumProbandi* optiones,
             HorreumProbatio* relatio,
                HorreumError* error,
                     Piscina* piscina);

/* corpus XML: textus elementorum ID, DisplayName, Location -> SCRUBBED */
chorda
horreum_responsum_purgare (
     chorda  corpus,
    Piscina* piscina);

constans character*
horreum_verdictum_nomen (
    HorreumVerdictum v);

#endif /* HORREUM_PROBATIO_H */
