/* nomina_macos.m - resolutio nominum per DNSServiceGetAddrInfo (vide
 * caput). Capita systematis ANTE latina.h (ut tls_securetransport_macos.m). */
#include <dns_sd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#include "nomina.h"

structura NominaQuaestio {
             Reactor* reactor;
       DNSServiceRef  servitium;     /* NULL: litteralis */
             integer  fossa;         /* -1: nulla in reactore */
         NominaFinis  finis;
              vacuum* datum;
             Piscina* piscina;
                 i32  portus;
     NominaAddressus  addressus[NOMINA_ADDRESSUS_MAXIMI];
                 i32  numerus;
    /* familia respondit (positive aut NoSuchRecord): A, AAAA */
                 b32 quattuor_responsum;
                 b32 sex_responsum;
      ReactorTimerId mora;
                 b32 mora_posita;
                 b32 finita;
              chorda error;
};

interior vacuum
_moram_tollere (
    NominaQuaestio* q)
{
    si (q->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(q->reactor, q->mora);
        q->mora_posita = FALSUM;
    }
}

/* servitium et fossa dimittuntur (numquam intra callback servitii:
 * semper ex timer aut abrumpere) */
interior vacuum
_dimittere (
    NominaQuaestio* q)
{
    _moram_tollere(q);
    si (q->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(q->reactor, q->fossa);
        q->fossa = -I;
    }
    si (q->servitium != NULL)
    {
        DNSServiceRefDeallocate(q->servitium);
        q->servitium = NULL;
    }
}

interior vacuum
_finire (
    vacuum* datum)
{
    NominaQuaestio* q = (NominaQuaestio*)datum;

    q->mora_posita = FALSUM;   /* incensus */
    si (q->finita)
    {
        redde;
    }
    q->finita = VERUM;
    _dimittere(q);
    si (q->numerus == ZEPHYRUM && q->error.mensura == ZEPHYRUM)
    {
        q->error = chorda_ex_literis("nomen ignotum", q->piscina);
    }
    nomina_familias_alternare(q->addressus, q->numerus);
    q->finis(q->datum, q->addressus, q->error.mensura > ZEPHYRUM
        ? ZEPHYRUM : q->numerus, q->error);
}

interior vacuum
_finire_post (
    NominaQuaestio* q,
               i32  ms)
{
    _moram_tollere(q);
    q->mora         = reactor_timer_adicere(q->reactor, ms, FALSUM,
        _finire, q);
    q->mora_posita  = (q->mora != REACTOR_TIMER_ERROR);
}

interior vacuum
_addere (
                NominaQuaestio* q,
    constans structura sockaddr* sa)
{
    NominaAddressus* a;
             size_t  n;

    si (q->numerus >= NOMINA_ADDRESSUS_MAXIMI)
    {
        redde;
    }
    a = &q->addressus[q->numerus];
    memset(a, 0, sizeof(*a));
    si (sa->sa_family == AF_INET)
    {
        structura sockaddr_in quattuor;

        n = sizeof(quattuor);
        memcpy(&quattuor, sa, n);
        quattuor.sin_port = htons((uint16_t)q->portus);
        memcpy(a->octeti, &quattuor, n);
    }
    alioquin si (sa->sa_family == AF_INET6)
    {
        structura sockaddr_in6 sex;

        n = sizeof(sex);
        memcpy(&sex, sa, n);
        sex.sin6_port = htons((uint16_t)q->portus);
        memcpy(a->octeti, &sex, n);
    }
    alioquin
    {
        redde;
    }
    a->mensura = (i32)n;
    a->familia = (s32)sa->sa_family;
    q->numerus++;
}

interior vacuum DNSSD_API
_responsum (
             DNSServiceRef  servitium,
           DNSServiceFlags  vexilla,
                  uint32_t  interfacies,
       DNSServiceErrorType  erratum,
       constans character*  hospes,
    constans structura sockaddr* sa,
                  uint32_t  ttl,
                    vacuum* contextus)
{
    NominaQuaestio* q = (NominaQuaestio*)contextus;

    (vacuum)servitium;
    (vacuum)interfacies;
    (vacuum)hospes;
    (vacuum)ttl;
    si (q->finita)
    {
        redde;
    }
    si (   erratum == kDNSServiceErr_NoError && sa != NULL
        && (vexilla & kDNSServiceFlagsAdd))
    {
        _addere(q, sa);
        si (sa->sa_family == AF_INET)
        { q->quattuor_responsum = VERUM;
        }
        si (sa->sa_family == AF_INET6)
        { q->sex_responsum = VERUM;
        }
    }
    alioquin si (erratum == kDNSServiceErr_NoSuchRecord && sa != NULL)
    {
        si (sa->sa_family == AF_INET)
        { q->quattuor_responsum = VERUM;
        }
        si (sa->sa_family == AF_INET6)
        { q->sex_responsum = VERUM;
        }
    }
    alioquin si (erratum != kDNSServiceErr_NoError)
    {
        character nuntius[LXIV];

        sprintf(nuntius, "DNS erratum %d", (integer)erratum);
        q->error               = chorda_ex_literis(nuntius, q->piscina);
        q->quattuor_responsum  = VERUM;
        q->sex_responsum       = VERUM;
    }
    si (vexilla & kDNSServiceFlagsMoreComing)
    {
        redde;
    }
    si (q->quattuor_responsum && q->sex_responsum)
    {
        _finire_post(q, ZEPHYRUM);
    }
    alioquin si (q->numerus > ZEPHYRUM && !q->mora_posita)
    {
        _finire_post(q, NOMINA_MORA_RESOLUTIONIS_MS);
    }
}

interior vacuum
_legibilis (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
         NominaQuaestio* q = (NominaQuaestio*)datum;
    DNSServiceErrorType  erratum;

    (vacuum)fossa;
    (vacuum)eventus;
    erratum = DNSServiceProcessResult(q->servitium);
    si (erratum != kDNSServiceErr_NoError && !q->finita)
    {
        q->error = chorda_ex_literis("DNSServiceProcessResult fallita",
            q->piscina);
        _finire_post(q, ZEPHYRUM);
    }
}

NominaQuaestio*
nomina_quaerere_incipere (
        Reactor* reactor,
         chorda  hospes,
            i32  portus,
    NominaFinis  finis,
         vacuum* datum,
        Piscina* piscina,
         chorda* error)
{
     NominaQuaestio* q;
          character* hospes_c;
    structura in_addr  quattuor;
    structura in6_addr sex;
     DNSServiceErrorType erratum;

    si (!reactor || !finis || !piscina || hospes.mensura <= ZEPHYRUM)
    {
        si (error && piscina)
        {
            *error = chorda_ex_literis("hospes vacuus", piscina);
        }
        redde NIHIL;
    }
    q = (NominaQuaestio*)piscina_allocare(piscina,
        sizeof(NominaQuaestio));
    memset(q, 0, sizeof(NominaQuaestio));
    q->reactor  = reactor;
    q->fossa    = -I;
    q->finis    = finis;
    q->datum    = datum;
    q->piscina  = piscina;
    q->portus   = portus;
    hospes_c    = chorda_ut_cstr(hospes, piscina);

    /* IP litteralis: addressus unus, finis per timer 0 ms */
    si (inet_pton(AF_INET, hospes_c, &quattuor) == I)
    {
        structura sockaddr_in sa;

        memset(&sa, 0, sizeof(sa));
        sa.sin_len     = (uint8_t)sizeof(sa);
        sa.sin_family  = AF_INET;
        sa.sin_addr    = quattuor;
        _addere(q, (constans structura sockaddr*)(constans vacuum*)&sa);
        _finire_post(q, ZEPHYRUM);
        redde q;
    }
    si (inet_pton(AF_INET6, hospes_c, &sex) == I)
    {
        structura sockaddr_in6 sa;

        memset(&sa, 0, sizeof(sa));
        sa.sin6_len     = (uint8_t)sizeof(sa);
        sa.sin6_family  = AF_INET6;
        sa.sin6_addr    = sex;
        _addere(q, (constans structura sockaddr*)(constans vacuum*)&sa);
        _finire_post(q, ZEPHYRUM);
        redde q;
    }

    /* ReturnIntermediates: sine eo responsa NEGATIVA (NoSuchRecord)
     * numquam veniunt - nomen ignotum nullum callback daret, quaestio
     * usque ad tempus vocantis exspectaret, et "ambae familiae
     * responderunt" numquam fieret (inventum per rete in T5) */
    erratum = DNSServiceGetAddrInfo(&q->servitium,
        kDNSServiceFlagsReturnIntermediates, 0,
        kDNSServiceProtocol_IPv4 | kDNSServiceProtocol_IPv6, hospes_c,
        _responsum, q);
    si (erratum != kDNSServiceErr_NoError || q->servitium == NULL)
    {
        si (error)
        {
            *error = chorda_ex_literis("DNSServiceGetAddrInfo fallita",
                piscina);
        }
        redde NIHIL;
    }
    q->fossa = DNSServiceRefSockFD(q->servitium);
    si (   q->fossa < ZEPHYRUM || !reactor_adicere(reactor, q->fossa,
            REACTOR_LEGERE, _legibilis, q))
    {
        q->fossa = -I;
        DNSServiceRefDeallocate(q->servitium);
        si (error)
        {
            *error =
                chorda_ex_literis("fossa DNS in reactorem non intrat",
                piscina);
        }
        redde NIHIL;
    }
    redde q;
}

vacuum
nomina_quaerere_abrumpere (
    NominaQuaestio* q)
{
    si (!q || q->finita)
    {
        redde;
    }
    q->finita = VERUM;
    _dimittere(q);
}
