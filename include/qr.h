/* qr.h - QR Code Generatio
 *
 * Bibliotheca pro QR code generation (ISO/IEC 18004).
 * Utilis pro mobile pairing, URLs, data encoding.
 *
 * AMBITUS: versiones I-X, modus OCTETUS solus. Tabulae ultra V10
 * non extenduntur; qr_versio_minima ZEPHYRUM reddit si datum
 * capacitatem V10 excedit (refutatio nominata, non codex fractus).
 *
 * VERIFICATIO: probationes/probatio_qr_gyrus.sh gyrum CLAUDIT -
 * CoreImage (decodator alienus) legit quod scripsimus. Probationes
 * structurales solae codicem ILLEGIBILEM transire sinunt: id
 * mensuratum est 2026-08-01, cum LXVI casus viderent formas rectas
 * dum V4+ propter intertextionem bloccorum absentem frangebantur.
 *
 * USUS:
 *   QR qr = qr_generare(datum, QR_ECC_M, piscina);
 *   si (qr.successus) {
 *       // qr.moduli continet amplitudo × amplitudo bytes
 *       // 0 = modulus albus, 1 = modulus niger
 *   }
 */

#ifndef QR_H
#define QR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"


/* ========================================================================
 * TYPI
 * ======================================================================== */

/* Gradus correctionis errorum
 * Maior gradus = plus redundantiae = minor capacitas datorum
 */
nomen enumeratio {
    QR_ECC_L = 0,   /* ~7% recuperatio - maxima capacitas */
    QR_ECC_M = 1,   /* ~15% recuperatio - bona aequilibrium */
    QR_ECC_Q = 2,   /* ~25% recuperatio */
    QR_ECC_H = 3    /* ~30% recuperatio - maxima durabilitas */
} QREcc;

/* Modus codificationis datorum */
nomen enumeratio {
    QR_MODUS_NUMERICUS      = 1,  /* 0-9 tantum (3 digiti → 10 bits) */
    QR_MODUS_ALPHANUMERICUS = 2,  /* 0-9, A-Z, space, $%*+-./: (2 char → 11 bits) */
    QR_MODUS_OCTETUS        = 4   /* Quilibet byte (1 byte → 8 bits) */
} QRModus;

/* Fructus generationis QR */
nomen structura {
    i32  amplitudo;    /* Latitudo et altitudo in modulis (QR semper quadratum) */
    i8*  moduli;       /* Array amplitudo × amplitudo: 0=albus, 1=niger */
    i32  versio;       /* Versio QR (I-X hic), determinat amplitudinem */
    b32  successus;    /* VERUM si generatio successit */
} QR;


/* ========================================================================
 * FUNCTIONES - GENERATIO
 * ======================================================================== */

/* Generare QR code ex chorda
 *
 * datum:    Chorda ad codificandum
 * ecc:      Gradus correctionis errorum
 * piscina:  Arena pro allocatio
 *
 * Redde: QR cum modulis (successus=FALSUM si error vel data nimis magna)
 */
QR
qr_generare(
    chorda   datum,
    QREcc    ecc,
    Piscina* piscina);

/* Generare QR code ex data binaria
 *
 * datum:    Pointer ad data binaria
 * mensura:  Longitudo datorum in bytes
 * ecc:      Gradus correctionis errorum
 * piscina:  Arena pro allocatio
 *
 * Redde: QR cum modulis
 */
QR
qr_generare_ex_octets(
    constans i8* datum,
           i32  mensura,
         QREcc  ecc,
      Piscina*  piscina);


/* ========================================================================
 * FUNCTIONES - ACCESSUS
 * ======================================================================== */

/* Obtinere valorem moduli ad positionem (x, y)
 *
 * qr: QR code
 * x:  Columna (0 = sinistra)
 * y:  Versus (0 = superior)
 *
 * Redde: 0=albus, 1=niger, -1=extra fines
 */
s32
qr_modulus(
    constans QR* qr,
           s32  x,
           s32  y);


/* ========================================================================
 * FUNCTIONES - UTILITAS
 * ======================================================================== */

/* Calculare versio minima necessaria pro data
 *
 * mensura_datorum: Longitudo datorum in bytes
 * ecc:             Gradus correctionis errorum
 *
 * Redde: Versio (I-X), vel 0 si data nimis magna (ultra V10)
 */
i32
qr_versio_minima(
    i32   mensura_datorum,
    QREcc ecc);

/* Structura bloccorum Reed-Solomon pro versione/gradu
 *
 * Introspectio: quot bloccis codewords dividantur, et quot
 * codewords data/ECC versio ferat. Invariantia quam observatores
 * probare possunt: ecc_codewords % bloci == 0, et
 * data_codewords + ecc_codewords == codewords totales versionis.
 *
 * versio: 1-10 (ambitus tabularum praesens)
 * Redde: FALSUM si versio/gradus extra ambitum (exeuntia intacta)
 */
b32
qr_structura(
    i32    versio,
    QREcc  ecc,
    i32*   bloci,
    i32*   data_codewords,
    i32*   ecc_codewords);

/* Obtinere amplitudinem (latitudo/altitudo) pro versione
 *
 * versio: Versio QR (I-X supportatae)
 *
 * Redde: Amplitudo in modulis (21 pro v1, 25 pro v2, etc.)
 *        Formula: 17 + versio * 4
 */
i32
qr_amplitudo_versionis(
    i32 versio);


#endif /* QR_H */
