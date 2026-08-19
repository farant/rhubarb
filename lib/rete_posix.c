/* rete_posix.c - enumeratio interfacierum per getifaddrs
 * (macOS, Linux). Vide rete.h pro consilio.
 */

#include "postulata_posix.h"
#include "rete.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>

/* chorda C in campum fixum copiare, semper terminata */
interior vacuum
_copiare_tuto (
             character* scopus,
                   i32  mensura,
    constans character* fons)
{
    i32 n;

    si (mensura <= ZEPHYRUM)
    {
        redde;
    }
    n = (i32)strlen(fons);
    si (n > mensura - I)
    {
        n = mensura - I;
    }
    memcpy(scopus, fons, (size_t)n);
    scopus[n] = '\0';
}

i32
rete_addressus_locales (
    ReteInterfacies* effusio,
                i32  capacitas)
{
    structura ifaddrs* omnia = NIHIL;
    structura ifaddrs* it;
                  i32 numerus = ZEPHYRUM;

    si (effusio == NIHIL || capacitas <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (getifaddrs(&omnia) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    per (it = omnia; it != NIHIL && numerus < capacitas;
         it = it->ifa_next)
    {
        structura sockaddr_in* quattuor;
                    character textus[RETE_ADDRESSUM_MENSURA];

        si (it->ifa_addr == NIHIL)
        {
            perge;
        }
        si (it->ifa_addr->sa_family != AF_INET)
        {
            perge;   /* IPv6 postea, cum tractum */
        }
        si (!(it->ifa_flags & (insignatus integer)IFF_UP))
        {
            perge;   /* interfacies iacens */
        }
        si (it->ifa_flags & (insignatus integer)IFF_LOOPBACK)
        {
            perge;   /* 127.0.0.1 telephonum non attingit */
        }

        quattuor = (structura sockaddr_in*)(vacuum*)it->ifa_addr;
        si (inet_ntop(AF_INET, &quattuor->sin_addr, textus,
                (socklen_t)magnitudo(textus)) == NIHIL)
        {
            perge;
        }
        /* nexus-localis: addressum quod exsistit sed nihil attingit
         * (DHCP defecit) - monstrare eum peius est quam tacere */
        si (strncmp(textus, "169.254.", VIII) == ZEPHYRUM)
        {
            perge;
        }

        _copiare_tuto(effusio[numerus].titulus,
            RETE_TITULUS_MENSURA,
            it->ifa_name ? it->ifa_name : "?");
        _copiare_tuto(effusio[numerus].addressum,
            RETE_ADDRESSUM_MENSURA, textus);
        numerus++;
    }

    freeifaddrs(omnia);
    redde numerus;
}
