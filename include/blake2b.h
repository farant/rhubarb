/* blake2b.h - BLAKE2b (RFC 7693): digestum 1..64 octetorum, cum clave
 * (MAC, 0..64 octeti) aut sine.
 *
 *   Blake2b b;
 *   blake2b_incipere(&b, XXXII, NIHIL, 0);
 *   blake2b_addere(&b, octeti, n);   ... iterum ...
 *   blake2b_finire(&b, digestum);    (exitus_n octeti)
 *
 * Incrementale = uno ictu pro omni scissione (probatio). Usus domus:
 * argon2id (H0, H'), et ubi hash velox cum longitudine variabili
 * postulatur. Non pro tesseris ipsis - pro iis argon2id. */
#ifndef BLAKE2B_H
#define BLAKE2B_H

#include "latina.h"

#define BLAKE2B_EXITUS_MAXIMUS  LXIV
#define BLAKE2B_CLAVIS_MAXIMA   LXIV
#define BLAKE2B_MASSA           CXXVIII

nomen structura {
    i64 h[VIII];               /* status catenae */
    i64 t[II];                 /* numerus octetorum (128 bits) */
     i8 b[BLAKE2B_MASSA];      /* massa currens */
    i32 c;                     /* octeti in b */
    i32 exitus_n;
} Blake2b;

/* FALSUM si exitus_n extra 1..64 aut clavis_n > 64 (aut clavis NIHIL
 * cum clavis_n > 0) */
b32
blake2b_incipere (
         Blake2b* status,
             i32  exitus_n,
     constans i8* clavis,
             i32  clavis_n);

vacuum
blake2b_addere (
         Blake2b* status,
     constans i8* octeti,
             i32  n);

/* exitus_n octeti in exitus */
vacuum
blake2b_finire (
    Blake2b* status,
         i8* exitus);

/* Uno ictu; nihil scribit si parametri mali */
vacuum
blake2b (
              i8* exitus,
             i32  exitus_n,
     constans i8* octeti,
             i32  n,
     constans i8* clavis,
             i32  clavis_n);

#endif /* BLAKE2B_H */
