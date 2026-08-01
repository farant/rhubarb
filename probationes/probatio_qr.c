/* probatio_qr.c - Probationes QR Code Generationis */
#include "latina.h"
#include "qr.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
         b32  praeteritus;
    Piscina* piscina;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("probatio_qr", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: qr_amplitudo_versionis
     * ======================================================== */

    {
        imprimere("\n--- Probans qr_amplitudo_versionis ---\n");

        /* Versio 1 = 21 moduli */
        CREDO_AEQUALIS_I32 (qr_amplitudo_versionis(I), XXI);

        /* Versio 2 = 25 moduli */
        CREDO_AEQUALIS_I32 (qr_amplitudo_versionis(II), XXV);

        /* Versio 3 = 29 moduli */
        CREDO_AEQUALIS_I32 (qr_amplitudo_versionis(III), XXIX);

        /* Versio 10 = 57 moduli */
        CREDO_AEQUALIS_I32 (qr_amplitudo_versionis(X), LVII);

        /* Extra fines = 0 */
        CREDO_AEQUALIS_I32 (qr_amplitudo_versionis(ZEPHYRUM), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (qr_amplitudo_versionis(XLI), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: qr_versio_minima
     * ======================================================== */

    {
        imprimere("\n--- Probans qr_versio_minima ---\n");

        /* Parvum datum -> versio 1 */
        CREDO_AEQUALIS_I32 (qr_versio_minima(V, QR_ECC_M), I);

        /* Medium datum -> versio 1 adhuc */
        CREDO_AEQUALIS_I32 (qr_versio_minima(XIV, QR_ECC_M), I);

        /* Maius datum -> versio 2 */
        CREDO_AEQUALIS_I32 (qr_versio_minima(XX, QR_ECC_M), II);

        /* ECC-H habet minus capacitatis */
        CREDO_AEQUALIS_I32 (qr_versio_minima(X, QR_ECC_H), II);
    }


    /* ========================================================
     * PROBARE: qr_generare - generatio basica
     * ======================================================== */

    {
        QR     qr;
        chorda test_data;

        imprimere("\n--- Probans qr_generare (basica) ---\n");

        test_data = chorda_ex_literis("HELLO", piscina);
        qr = qr_generare(test_data, QR_ECC_M, piscina);

        /* Debet succedere */
        CREDO_VERUM    (qr.successus);
        CREDO_NON_NIHIL (qr.moduli);

        /* Versio 1 pro parvum datum */
        CREDO_AEQUALIS_I32 (qr.versio, I);
        CREDO_AEQUALIS_I32 (qr.amplitudo, XXI);
    }


    /* ========================================================
     * PROBARE: qr_generare - input vacuus
     * ======================================================== */

    {
        QR     qr;
        chorda vacua;

        imprimere("\n--- Probans qr_generare (vacuus) ---\n");

        /* Chorda vacua: datum = NIHIL, mensura = 0 */
        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        qr = qr_generare(vacua, QR_ECC_M, piscina);

        /* Debet fallere */
        CREDO_FALSUM (qr.successus);
        CREDO_NIHIL  (qr.moduli);
    }


    /* ========================================================
     * PROBARE: qr_modulus - finder patterns
     * ======================================================== */

    {
        QR     qr;
        chorda test_data;

        imprimere("\n--- Probans finder patterns ---\n");

        test_data = chorda_ex_literis("TEST", piscina);
        qr = qr_generare(test_data, QR_ECC_M, piscina);

        CREDO_VERUM (qr.successus);

        /* Top-left finder pattern (7x7 in angulo 0,0)
         * Exterior nigrum, medium album, centrum nigrum */

        /* Anguli exteriores - nigrum */
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, ZEPHYRUM, ZEPHYRUM), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, VI, ZEPHYRUM), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, ZEPHYRUM, VI), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, VI, VI), I);

        /* Centrum 3x3 - nigrum */
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, III, III), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, II, III), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, IV, III), I);

        /* Top-right finder (angulus mag-7, 0) */
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XX, ZEPHYRUM), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XIV, ZEPHYRUM), I);

        /* Bottom-left finder (angulus 0, mag-7) */
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, ZEPHYRUM, XX), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, ZEPHYRUM, XIV), I);
    }


    /* ========================================================
     * PROBARE: qr_modulus - timing patterns
     * ======================================================== */

    {
        QR     qr;
        chorda test_data;
        s32    m8, m9, m10, m11, m12;

        imprimere("\n--- Probans timing patterns ---\n");

        test_data = chorda_ex_literis("TEST", piscina);
        qr = qr_generare(test_data, QR_ECC_M, piscina);

        CREDO_VERUM (qr.successus);

        /* Timing horizontalis (versus 6) - alternans ab columna 8 */
        m8  = qr_modulus(&qr, VIII, VI);
        m9  = qr_modulus(&qr, IX, VI);
        m10 = qr_modulus(&qr, X, VI);
        m11 = qr_modulus(&qr, XI, VI);
        m12 = qr_modulus(&qr, XII, VI);

        /* Debet alternare */
        CREDO_INAEQUALITAS_S32 (m8, m9);
        CREDO_AEQUALIS_S32 (m8, m10);
        CREDO_INAEQUALITAS_S32 (m9, m10);
        CREDO_AEQUALIS_S32 (m9, m11);
        CREDO_AEQUALIS_S32 (m10, m12);

        /* Timing verticalis (columna 6) - alternans ab versu 8 */
        m8  = qr_modulus(&qr, VI, VIII);
        m9  = qr_modulus(&qr, VI, IX);
        m10 = qr_modulus(&qr, VI, X);

        CREDO_INAEQUALITAS_S32 (m8, m9);
        CREDO_AEQUALIS_S32 (m8, m10);
    }


    /* ========================================================
     * PROBARE: alignment patterns (V2+)
     * ======================================================== */

    {
        QR     qr;
        chorda test_data;

        imprimere("\n--- Probans alignment patterns ---\n");

        /* Generare V2 QR (necessitas >= 15 bytes ad M) */
        test_data = chorda_ex_literis("12345678901234567890", piscina);
        qr = qr_generare(test_data, QR_ECC_M, piscina);

        CREDO_VERUM (qr.successus);
        CREDO_AEQUALIS_I32 (qr.versio, II);
        CREDO_AEQUALIS_I32 (qr.amplitudo, XXV);

        /* V2 habet alignment pattern ad (18,18)
         * Pattern: 5x5 cum centrum nigrum, anulus albus, exterior niger */

        /* Centrum - nigrum */
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XVIII, XVIII), I);

        /* Anulus internus - albus */
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XVII, XVIII), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XIX, XVIII), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XVIII, XVII), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XVIII, XIX), ZEPHYRUM);

        /* Anguli exteriores - nigrum */
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XVI, XVI), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XX, XVI), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XVI, XX), I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XX, XX), I);
    }


    /* ========================================================
     * PROBARE: qr_modulus - fines
     * ======================================================== */

    {
        QR     qr;
        chorda test_data;

        imprimere("\n--- Probans qr_modulus fines ---\n");

        test_data = chorda_ex_literis("TEST", piscina);
        qr = qr_generare(test_data, QR_ECC_M, piscina);

        CREDO_VERUM (qr.successus);

        /* Extra fines redde -1 */
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, -I, ZEPHYRUM), -I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, ZEPHYRUM, -I), -I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, XXI, ZEPHYRUM), -I);
        CREDO_AEQUALIS_S32 (qr_modulus(&qr, ZEPHYRUM, XXI), -I);

        /* NIHIL qr */
        CREDO_AEQUALIS_S32 (qr_modulus(NIHIL, ZEPHYRUM, ZEPHYRUM), -I);
    }


    /* ========================================================
     * PROBARE: qr_generare - URL typica
     * ======================================================== */

    {
        QR     qr;
        chorda url;

        imprimere("\n--- Probans URL typica ---\n");

        url = chorda_ex_literis("https://example.com/?token=abc123", piscina);
        qr = qr_generare(url, QR_ECC_M, piscina);

        /* Debet succedere */
        CREDO_VERUM (qr.successus);
        CREDO_NON_NIHIL (qr.moduli);

        /* URL 33 characteres -> versio 3 (V2=26 nimis parva, V3=42 satis) */
        CREDO_AEQUALIS_I32 (qr.versio, III);
        CREDO_AEQUALIS_I32 (qr.amplitudo, XXIX);
    }


    /* ========================================================
     * PROBARE: qr_generare - ECC levels
     * ======================================================== */

    {
        QR     qr_l, qr_m, qr_q, qr_h;
        chorda datum;

        imprimere("\n--- Probans ECC levels ---\n");

        /* 15 bytes datum */
        datum = chorda_ex_literis("123456789012345", piscina);

        qr_l = qr_generare(datum, QR_ECC_L, piscina);
        qr_m = qr_generare(datum, QR_ECC_M, piscina);
        qr_q = qr_generare(datum, QR_ECC_Q, piscina);
        qr_h = qr_generare(datum, QR_ECC_H, piscina);

        CREDO_VERUM (qr_l.successus);
        CREDO_VERUM (qr_m.successus);
        CREDO_VERUM (qr_q.successus);
        CREDO_VERUM (qr_h.successus);

        /* ECC-L habet maximum capacitatem, parvissima versio */
        /* ECC-H habet minimum capacitatem, maxima versio */
        /* Pro 15 bytes: L=V1 (17), M=V2 (26>14), Q=V2 (20>11), H=V3 (24>14) */
        CREDO_AEQUALIS_I32 (qr_l.versio, I);
        CREDO_AEQUALIS_I32 (qr_m.versio, II);
        CREDO_AEQUALIS_I32 (qr_q.versio, II);
        CREDO_AEQUALIS_I32 (qr_h.versio, III);
    }


    /* ========================================================
     * PROBARE: structura bloccorum (arithmetica tabulae)
     *
     * Oraculum gyri (probatio_qr_gyrus.sh) XXV compositiones
     * versionis/gradus probat; hic OMNES XL cheaply. Totales
     * INDEPENDENTER hic scripti sunt - probatio quae numeros ex
     * eadem tabula haurit nihil probat.
     * ======================================================== */

    {
        /* codewords totales per versionem (ISO 18004 tabula 1) */
        interior constans i32 totales[10] = {
            26, 44, 70, 100, 134, 172, 196, 242, 292, 346
        };
        i32 v, e;

        imprimere("\n--- Probans qr_structura (XL compositiones) ---\n");

        per (v = I; v <= X; v++)
        {
            per (e = ZEPHYRUM; e <= III; e++)
            {
                i32 bloci = ZEPHYRUM;
                i32 data_cw = ZEPHYRUM;
                i32 ecc_cw = ZEPHYRUM;

                CREDO_VERUM(qr_structura(v, (QREcc)e, &bloci,
                    &data_cw, &ecc_cw));
                /* bloci ECC omnes eiusdem longitudinis esse DEBENT */
                CREDO_AEQUALIS_I32(ecc_cw % bloci, ZEPHYRUM);
                /* summa == totalis versionis (fons independens) */
                CREDO_AEQUALIS_I32(data_cw + ecc_cw, totales[v - I]);
                /* omnis bloccus unum saltem codeword data ferat */
                CREDO_VERUM(data_cw / bloci >= I);
            }
        }

        /* extra ambitum = refutatio, exeuntia intacta */
        {
            i32 sentinella = 12345;   /* i32 INSIGNATUS: -1 non licet */

            CREDO_FALSUM(qr_structura(ZEPHYRUM, QR_ECC_M, &sentinella,
                NIHIL, NIHIL));
            CREDO_FALSUM(qr_structura(XI, QR_ECC_M, &sentinella,
                NIHIL, NIHIL));
            CREDO_AEQUALIS_I32(sentinella, 12345);
        }
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
