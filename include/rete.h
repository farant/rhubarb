/* rete.h - inspectio interfacierum reticuli localis
 *
 * Implementatio: rete_posix.c (macOS, Linux - getifaddrs).
 *
 * QUAESTIO QUAM SOLVIT: "quo addressu me telephonum inveniat?"
 * Servus vitreae in 0.0.0.0 ligari potest, sed URL quod in QR
 * ponimus addressum CONCRETUM poscit.
 *
 * ADDRESSUM LOCALE SINGULARE NON EST - haec est tota difficultas.
 * Laptop simul habere potest:
 *   en0        wifi domestica          192.168.1.x
 *   en0        hotspot telephoni       172.20.10.x
 *   utun0-3    cuniculi VPN            10.x / fd00::
 *   bridge100  cum laptop ipse communicat
 *   lo0        loopback                127.0.0.1
 * Praeterea nexus-locales (169.254.x.x) apparent cum DHCP defecit
 * - addressum quod EXSISTIT sed nihil attingit.
 *
 * Ergo haec bibliotheca NON unum addressum divinat: candidatos
 * omnes reddit cum titulis interfacierum, et vocans (aut homo
 * oculis) eligit. Divinatio hic peior esset quam electio: cum
 * telephonum hotspot praebet, interfacies recta NON est ea quae
 * domi recta erat.
 */

#ifndef RETE_H
#define RETE_H

#include "latina.h"

#define RETE_TITULUS_MENSURA    XXXII
#define RETE_ADDRESSUM_MENSURA  XLVI   /* spatium IPv6 futurum */

nomen structura {
    /* 'titulus', NON 'nomen': nomen macro latinae est (typedef) */
    character titulus[RETE_TITULUS_MENSURA];     /* "en0" */
    character addressum[RETE_ADDRESSUM_MENSURA]; /* "172.20.10.4" */
} ReteInterfacies;

/* Addressus IPv4 reticuli localis: interfacies VIVAE, non
 * loopback, non nexus-locales (169.254.x.x - DHCP defecit).
 * Ordo = ordo systematis (en0 ante utun plerumque).
 *
 * effusio:   tabulatum scribendum
 * capacitas: quot ordines capere possit
 *
 * Redde: numerus scriptus (<= capacitas), ZEPHYRUM si nullus
 *        aut si enumeratio defecit. ZEPHYRUM NON est error
 *        distinguendus: utroque casu addressum monstrandum non est.
 */
i32
rete_addressus_locales (
    ReteInterfacies* effusio,
    i32              capacitas);

#endif /* RETE_H */
