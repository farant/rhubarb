/* fiducia_der.h - partes certificati X.509 ex DER (fiducia-linux T1)
 *
 * Pura et sine allocatione: sectiones in octetos datos monstrant.
 * Solum quae fiducia poscit, quae decodificator BearSSL non praebet:
 * emittens, series, subiectum, SubjectPublicKeyInfo (indicium
 * diffisarum - OneCRL emittens+series aut subiectum+SHA-256 clavis) et
 * nomina alterna iPAddress (BearSSL ea non extrahit,
 * vendor/bearssl_amalgama.h:9449). Signaturas NON iudicat - hoc
 * BearSSL facit; hic solum forma.
 *
 * Longitudines DER solum definitae (forma brevis aut longa, <= IV
 * octeti); quaeque intra fines reliquos probatur - DER malum aut
 * truncatum FALSUM cum causa nominata reddit, numquam extra legit. */
#ifndef FIDUCIA_DER_H
#define FIDUCIA_DER_H

#include "latina.h"

/* <aedilis corpus="lib/fiducia_der.c"/> */

nomen structura {
    constans i8* octeti;
            i32  mensura;
} FiduciaSectio;

nomen structura {
    FiduciaSectio emittens;        /* Name DER (tag et longitudo
                                     * comprehensa) */
    FiduciaSectio series;          /* INTEGER contentum (0x00 initiale
                                     * servatum) */
    FiduciaSectio subiectum;       /* Name DER */
    FiduciaSectio clavis_publica;  /* SubjectPublicKeyInfo DER */
    FiduciaSectio extensiones;     /* Extensions SEQUENCE contentum;
                                     * mensura 0 si nullae */
              b32 habet_alterna;       /* extensio 2.5.29.17 adest */
} FiduciaPartes;

/* VERUM si certificatum bene formatum; FALSUM + *causa (literae
 * staticae) aliter. partes semper impletae (zephyro si FALSUM) */
b32
fiducia_partes_legere (
           constans i8*  der,
                   i32   mensura,
         FiduciaPartes*  partes,
    constans character** causa);

/* VERUM si iPAddress (IV aut XVI octeti) inter nomina alterna est.
 * Sine SAN aut SAN malo: FALSUM */
b32
fiducia_alterna_ip_continet (
    constans FiduciaPartes* partes,
               constans i8* ip,
                       i32  mensura);

/* nomen commune (OID 2.5.4.3) subiecti: VERUM + titulus (contentum
 * chordae, sine tag) in *titulus si adest */
b32
fiducia_nomen_commune (
    constans FiduciaPartes* partes,
             FiduciaSectio* titulus);

#endif /* FIDUCIA_DER_H */
