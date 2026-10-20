/* argon2id.h - Argon2id (RFC 9106, versio 0x13) pro tesseris
 *
 * GRADATIM: opus in reactore per gradus agitur - gradus unus
 * 'caudices_maximi' caudices (1 KiB singuli) implet et redit, ut ansa
 * una (hospitium) interim alias petitiones serviat. Lanae seriatim
 * computantur (p > 1 licet, RFC vector p = 4; productio p = 1).
 *
 *   Argon2idOpus* o = argon2id_incipere(&par, tessera, tn, sal, sn, p);
 *   dum (!argon2id_gradus(o, CXXVIII)) { ... ansa ... }
 *   argon2id_digestum(o, digestum);
 *
 * Memoria: m_cost_kib KiB in piscina data (allocatio UNA) - piscina
 * propria per opus, destructa post digestum (memoria redditur).
 * Productio (OWASP): m = 19456 (19 MiB), t = 2, p = 1, longitudo 32,
 * sal 16 octeti ex moneta.
 *
 * PHC: $argon2id$v=19$m=<m>,t=<t>,p=<p>$<sal>$<digestum> (base64 sine
 * '=', alphabetum normale). */
#ifndef ARGON2ID_H
#define ARGON2ID_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

#define ARGON2ID_VERSIO          0x13
#define ARGON2ID_SAL_MAXIMUS     LXIV
#define ARGON2ID_DIGESTUM_MAXIMUM LXIV

nomen structura {
                i32  t_cost;       /* transitus >= 1 */
                i32  m_cost_kib;   /* >= 8 * lanae */
                i32  lanae;        /* >= 1 */
                i32  longitudo;    /* digestum 4..64 octeti */
        constans i8* secretum;     /* K (NIHIL licet) */
                i32  secretum_n;
        constans i8* ad;           /* X, data associata (NIHIL licet) */
                i32  ad_n;
} Argon2idParametri;

nomen structura Argon2idOpus Argon2idOpus;

/* H0 et caudices primi; NIHIL si parametri mali aut memoria deest */
Argon2idOpus*
argon2id_incipere (
    constans Argon2idParametri* parametri,
                   constans i8* tessera,
                           i32  tn,
                   constans i8* sal,
                           i32  sn,
                       Piscina* piscina);

/* Implere ad caudices_maximi caudices (0 = omnes). VERUM = finitum */
b32
argon2id_gradus (
     Argon2idOpus* opus,
              i32  caudices_maximi);

/* Post finem: parametri->longitudo octeti in exitus */
vacuum
argon2id_digestum (
    Argon2idOpus* opus,
              i8* exitus);

/* Uno ictu; FALSUM si parametri mali */
b32
argon2id_computare (
    constans Argon2idParametri* parametri,
                   constans i8* tessera,
                           i32  tn,
                   constans i8* sal,
                           i32  sn,
                            i8* exitus,
                       Piscina* piscina);

/* Chorda PHC; vacua si parametri mali */
chorda
argon2id_phc_scribere (
    constans Argon2idParametri* parametri,
                   constans i8* sal,
                           i32  sn,
                   constans i8* digestum,
                       Piscina* piscina);

/* PHC legere: parametri (t, m, p, longitudo = dn), sal (<= 64),
 * digestum (<= 64). FALSUM si algorithmus alius, v != 19, campi
 * absentes aut mali, base64 mala. secretum/ad non tanguntur. */
b32
argon2id_phc_legere (
                chorda  phc,
     Argon2idParametri* parametri,
                    i8* sal,
                   i32* sn,
                    i8* digestum,
                   i32* dn);

#endif /* ARGON2ID_H */
