/* crustula.h - crustula HTTP (RFC 6265): caput Cookie legere,
 * Set-Cookie scribere
 *
 * LEGERE: "a=1; b=2" - paria per ';' (spatia circa ignorata), valor
 * citatus ("...") sine virgulis redditur; par sine '=' aut sine
 * titulo omittitur. Primum par congruens vincit.
 *
 * SCRIBERE: titulus = token RFC 2616 (sine CTL, spatio, separatoribus
 * ()<>@,;:\"/[]?={}), valor = cookie-octet (sine CTL, spatio, '"',
 * ',', ';', '\') - aliter RECUSATUR (chorda vacua), numquam mittitur
 * evasum aut truncatum. Ordo: titulus=valor; Max-Age; Path; Domain;
 * Secure; HttpOnly; SameSite. SameSite=None sine Secure recusatur
 * (navigatores id abiciunt); praefixa "__Secure-" (Secure postulat)
 * et "__Host-" (Secure, Path=/, sine Domain) custodiuntur. */
#ifndef CRUSTULA_H
#define CRUSTULA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"

nomen enumeratio {
    CRUSTULUM_EADEM_LAXA = 0,    /* SameSite=Lax (defalta) */
    CRUSTULUM_EADEM_STRICTA,     /* SameSite=Strict */
    CRUSTULUM_EADEM_NULLA        /* SameSite=None (Secure necesse) */
} CrustulumEadem;

nomen structura {
                    s64  max_aetas_s;   /* <0 sine Max-Age; 0 delere */
     constans character* via;           /* NIHIL = "/" */
     constans character* dominium;      /* NIHIL = sine Domain */
                    b32  http_solum;    /* HttpOnly */
                    b32  securum;       /* Secure */
         CrustulumEadem  eadem;
} CrustulumOptiones;

/* Valor crustuli 'titulus' ex capite Cookie; datum NIHIL si absens
 * (valor vacuus praesens: datum non NIHIL, mensura 0). Chorda in
 * caput ipsum spectat. */
chorda
crustulum_valor (
                 chorda  caput_crustulorum,
     constans character* titulus);

/* Valor capitis Set-Cookie (sine "Set-Cookie: "); chorda vacua si
 * titulus, valor aut optiones recusantur. NIHIL optiones = sessionis,
 * Path=/, SameSite=Lax, sine HttpOnly/Secure. */
chorda
crustulum_scribere (
            constans character* titulus,
                        chorda  valor,
    constans CrustulumOptiones* optiones,
                       Piscina* piscina);

#endif /* CRUSTULA_H */
