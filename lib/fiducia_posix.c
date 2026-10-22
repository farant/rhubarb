/* fiducia_posix.c - fiducia certificatorum sub POSIX: NONDUM.
 *
 * Variantes 'posix' fiducia_macos.m (SecTrust). Linux repositorium
 * systematis (/etc/ssl/certs) et iudicium X.509 per BearSSL nondum
 * scripta sunt (hospitium-v3-spec IV: 'fiducia_linux' futurum).
 * Usque tunc omnis catena RECUSATUR cum causa nominata - numquam
 * 'fide digna' sine iudicio (aliter TLS sub Linux tacite insecurum
 * esset). Applicatio probans (exemplar) vocationes egressas non
 * facit, ergo nihil amittit. */
#include "fiducia.h"
#include "fiducia_diffisa.h"

b32
fiducia_iudicare (
    constans FiduciaCertificatum* catena,
                             i32  numerus,
              constans character* hospes,
              constans character* ancora_via,
                          chorda* causa,
                         Piscina* piscina)
{
    (vacuum)hospes;
    (vacuum)ancora_via;
    /* indicium diffisarum PRIMUM (fiducia-linux T2): causa propria */
    si (!fiducia_diffisas_iudicare(catena, numerus, NIHIL, ZEPHYRUM,
            causa, piscina))
    {
        redde FALSUM;
    }
    si (causa && piscina)
    {
        *causa =
            chorda_ex_literis("certificatum reiectum: fiducia POSIX "
            "nondum (fiducia_linux futurum)", piscina);
    }
    redde FALSUM;
}

b32
fiducia_folium_describere (
    constans FiduciaCertificatum* folium,
                   FiduciaFolium* exitus,
                         Piscina* piscina)
{
    (vacuum)folium;
    si (exitus)
    {
        exitus->valida     = FALSUM;
        exitus->non_ante   = 0;
        exitus->non_post   = 0;
        exitus->subiectum  = chorda_ex_literis("", piscina);
    }
    redde FALSUM;
}
