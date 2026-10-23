/* fiducia.h - iudicium fiduciae certificatorum per PLATFORMAM
 * (tls-bearssl-spec II.4)
 *
 * Catena (DER, folium primum) contra repositorium systematis aut
 * ancoram SOLAM, politia SSL cum nomine hospitis (catena, tempus,
 * nomen, revocatio - quidquid systema honorat). Implementationes:
 * fiducia_macos.m (SecTrustEvaluateWithError) et fiducia_posix.c
 * (fiducia-linux T3: BearSSL br_x509_minimal + fasciculus CA
 * distributionis per vias notas, SAN solum, revocatio nulla); postea
 * fiducia_windows.c. Ambae indicium diffisarum domus PRIMUM consulunt
 * (fiducia_diffisa.h).
 *
 * NOTA: iudicium OBSTRUERE potest (intermedia per AIA, revocatio) - ut
 * SecureTransport intus faciebat. FiduciaFolium = TlsCertificatum sine
 * tls.h (tls-bearssl R6). */
#ifndef FIDUCIA_H
#define FIDUCIA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen structura {
    constans i8* octeti;    /* DER */
            i32  mensura;
} FiduciaCertificatum;

/* summarium folii (tls_certificatum_obtinere) */
nomen structura {
       b32 valida;      /* legibile et non_post > 0 */
       s64 non_ante;    /* epocha unix */
       s64 non_post;
    chorda subiectum;   /* summarium subiecti (CN); vacuum si abest */
} FiduciaFolium;

/* VERUM si catena fide digna pro hospite (nomen DNS aut IP). ancora_via
 * NIHIL = repositorium systematis; aliter certificatum DER quod SOLUM
 * fide dignum est. FALSUM: *causa nominata in piscina - 'certificatum
 * reiectum: <ratio systematis>', 'certificatum catenae non est DER',
 * 'ancora legi non potest', 'ancora non est certificatum DER' */
b32
fiducia_iudicare (
    constans FiduciaCertificatum* catena,
                             i32  numerus,
              constans character* hospes,
              constans character* ancora_via,
                          chorda* causa,
                         Piscina* piscina);

/* folium DER -> tempora (epocha unix) et subiectum. FALSUM si non DER
 * (exitus valida FALSUM) */
b32
fiducia_folium_describere (
    constans FiduciaCertificatum* folium,
                   FiduciaFolium* exitus,
                         Piscina* piscina);

/* fasciculus certificatorum (POSIX): via NIHIL = systematis (prima via
 * nota exstans; *via_usa eam nominat), aliter fasciculus PEM datus in
 * repositorium temporarium (probationes). *ancorae = ancorae legitimae,
 * *omissae = obiecta praeterita (non CERTIFICATE, malum, aut diffisa).
 * FALSUM + *causa si nullus fasciculus aut legi nequit. macOS: FALSUM
 * "solum POSIX" (SecTrust fasciculum non habet) */
b32
fiducia_fasciculum_describere (
    constans character* via,
                   i32* ancorae,
                   i32* omissae,
                chorda* via_usa,
                chorda* causa,
               Piscina* piscina);

#endif /* FIDUCIA_H */
