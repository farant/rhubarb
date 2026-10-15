/* nomina.h - resolutio nominum NON-OBSTRUENS super reactorem
 * (http-vocatio-spec II.1, decretum ...CYT9A8)
 *
 * Implementatio: nomina_macos.m (DNSServiceGetAddrInfo ex <dns_sd.h>:
 * per resolutorem systematis - /etc/hosts, DNS per interfaciem, VPN,
 * /etc/resolver, mDNS - ergo responsa eadem ac systema). Fossa
 * servitii in reactorem VOCANTIS intrat; responsa per familiam (IPv4,
 * IPv6) colliguntur; finis cum ambae responderunt (positive aut
 * NoSuchRecord) aut NOMINA_MORA_RESOLUTIONIS_MS post addressum primum
 * (RFC 8305 §3, "resolution delay"). IP litteralis sine quaestione.
 *
 * finis SEMEL, numquam intra nomina_quaerere_incipere (timer 0 ms).
 * Addressus PORTU POSITO redduntur (sockaddr crudum, tcp_incipere
 * directe accipit). */
#ifndef NOMINA_H
#define NOMINA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"

#define NOMINA_ADDRESSUS_MAXIMI      XVI
#define NOMINA_ADDRESSUS_OCTETI      CXXVIII   /* sockaddr_storage */
#define NOMINA_MORA_RESOLUTIONIS_MS  L         /* RFC 8305 §3 */

nomen structura {
     i8 octeti[NOMINA_ADDRESSUS_OCTETI];   /* sockaddr crudum, portu
                                            * posito */
    i32 mensura;                           /* sizeof sockaddr_in(6) */
    s32 familia;                           /* AF_INET aut AF_INET6 */
} NominaAddressus;

nomen structura NominaQuaestio NominaQuaestio;

/* semel; numquam intra nomina_quaerere_incipere. error vacua =
 * successus (numerus >= I); aliter numerus ZEPHYRUM et error
 * nominatus ("nomen ignotum", erratum DNS) */
nomen vacuum (*NominaFinis)(vacuum* datum,
    constans NominaAddressus* addressus, i32 numerus, chorda error);

/* quaestio incipit; NIHIL + *error si nequit (hospes vacuus, servitium
 * DNS non aperitur) - tunc nihil in reactore */
NominaQuaestio*
nomina_quaerere_incipere (
        Reactor* reactor,
         chorda  hospes,
            i32  portus,
    NominaFinis  finis,
         vacuum* datum,
        Piscina* piscina,
         chorda* error);

/* finis numquam postea; fossa et timers e reactore; post finem aut
 * iterum vocata: nihil agit */
vacuum
nomina_quaerere_abrumpere (
    NominaQuaestio* quaestio);

#endif /* NOMINA_H */
