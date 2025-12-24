/* probatio_fasti.c - Probationes Fasti (Calendarium) */
#include "latina.h"
#include "fasti.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
    b32  praeteritus;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_fasti", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Probare fasti_dies constructor
     * ================================================== */

    {
        Dies d;

        imprimere("\n--- Probans fasti_dies ---\n");

        d = fasti_dies(MMXXIV, III, XV);
        CREDO_AEQUALIS_S32(d.annus, MMXXIV);
        CREDO_AEQUALIS_S32(d.mensis, III);
        CREDO_AEQUALIS_S32(d.dies, XV);
    }


    /* ==================================================
     * Probare fasti_hora constructor
     * ================================================== */

    {
        Hora h;

        imprimere("\n--- Probans fasti_hora ---\n");

        h = fasti_hora(XIV, XXX, XLV);
        CREDO_AEQUALIS_S32(h.hora, XIV);
        CREDO_AEQUALIS_S32(h.minutum, XXX);
        CREDO_AEQUALIS_S32(h.secundum, XLV);
    }


    /* ==================================================
     * Probare fasti_est_bissextilis
     * ================================================== */

    {
        imprimere("\n--- Probans fasti_est_bissextilis ---\n");

        /* 2000 est bissextilis (divisibilis per 400) */
        CREDO_VERUM(fasti_est_bissextilis(MM));

        /* 2004 est bissextilis (divisibilis per 4) */
        CREDO_VERUM(fasti_est_bissextilis(MMIV));

        /* 1900 non est bissextilis (divisibilis per 100 sed non 400) */
        CREDO_FALSUM(fasti_est_bissextilis(MCM));

        /* 2023 non est bissextilis */
        CREDO_FALSUM(fasti_est_bissextilis(MMXXIII));

        /* 2024 est bissextilis */
        CREDO_VERUM(fasti_est_bissextilis(MMXXIV));
    }


    /* ==================================================
     * Probare fasti_dies_in_mense
     * ================================================== */

    {
        imprimere("\n--- Probans fasti_dies_in_mense ---\n");

        /* Ianuarius habet 31 dies */
        CREDO_AEQUALIS_S32(fasti_dies_in_mense(I, MMXXIV), XXXI);

        /* Februarius in anno bissextili habet 29 dies */
        CREDO_AEQUALIS_S32(fasti_dies_in_mense(II, MMXXIV), XXIX);

        /* Februarius in anno non-bissextili habet 28 dies */
        CREDO_AEQUALIS_S32(fasti_dies_in_mense(II, MMXXIII), XXVIII);

        /* Aprilis habet 30 dies */
        CREDO_AEQUALIS_S32(fasti_dies_in_mense(IV, MMXXIV), XXX);

        /* December habet 31 dies */
        CREDO_AEQUALIS_S32(fasti_dies_in_mense(XII, MMXXIV), XXXI);

        /* Mensis invalidus reddit 0 */
        CREDO_AEQUALIS_S32(fasti_dies_in_mense(XIII, MMXXIV), ZEPHYRUM);
        CREDO_AEQUALIS_S32(fasti_dies_in_mense(ZEPHYRUM, MMXXIV), ZEPHYRUM);
    }


    /* ==================================================
     * Probare fasti_dies_valida
     * ================================================== */

    {
        Dies d_valida, d_invalida1, d_invalida2, d_invalida3;

        imprimere("\n--- Probans fasti_dies_valida ---\n");

        d_valida = fasti_dies(MMXXIV, III, XV);
        CREDO_VERUM(fasti_dies_valida(d_valida));

        /* 31 Februarii non existit */
        d_invalida1 = fasti_dies(MMXXIV, II, XXXI);
        CREDO_FALSUM(fasti_dies_valida(d_invalida1));

        /* Mensis 13 non existit */
        d_invalida2 = fasti_dies(MMXXIV, XIII, I);
        CREDO_FALSUM(fasti_dies_valida(d_invalida2));

        /* Dies 0 non existit */
        d_invalida3 = fasti_dies(MMXXIV, I, ZEPHYRUM);
        CREDO_FALSUM(fasti_dies_valida(d_invalida3));

        /* 29 Feb 2024 (bissextilis) est valida */
        CREDO_VERUM(fasti_dies_valida(fasti_dies(MMXXIV, II, XXIX)));

        /* 29 Feb 2023 (non-bissextilis) non est valida */
        CREDO_FALSUM(fasti_dies_valida(fasti_dies(MMXXIII, II, XXIX)));
    }


    /* ==================================================
     * Probare fasti_dies_hebdomadis
     * ================================================== */

    {
        Dies d;
        s32 dh;

        imprimere("\n--- Probans fasti_dies_hebdomadis ---\n");

        /* 1 Ianuarii 2024 erat Feria II (Monday) = 1 */
        d = fasti_dies(MMXXIV, I, I);
        dh = fasti_dies_hebdomadis(d);
        CREDO_AEQUALIS_S32(dh, FASTI_FERIA_II);

        /* 25 Decembris 2024 est Feria IV (Wednesday) = 3 */
        d = fasti_dies(MMXXIV, XII, XXV);
        dh = fasti_dies_hebdomadis(d);
        CREDO_AEQUALIS_S32(dh, FASTI_FERIA_IV);

        /* 1 Ianuarii 2000 erat Sabbatum = 6 */
        d = fasti_dies(MM, I, I);
        dh = fasti_dies_hebdomadis(d);
        CREDO_AEQUALIS_S32(dh, FASTI_SABBATUM);

        /* 4 Iulii 1776 erat Feria V (Thursday) = 4 */
        d = fasti_dies(MDCCLXXVI, VII, IV);
        dh = fasti_dies_hebdomadis(d);
        CREDO_AEQUALIS_S32(dh, FASTI_FERIA_V);
    }


    /* ==================================================
     * Probare fasti_ad_iulianum et fasti_ex_iuliano
     * ================================================== */

    {
        Dies d_orig, d_round;
        s64 jdn;

        imprimere("\n--- Probans conversio Iuliana ---\n");

        /* Round-trip probatio */
        d_orig = fasti_dies(MMXXIV, III, XV);
        jdn = fasti_ad_iulianum(d_orig);
        d_round = fasti_ex_iuliano(jdn);

        CREDO_AEQUALIS_S32(d_round.annus, d_orig.annus);
        CREDO_AEQUALIS_S32(d_round.mensis, d_orig.mensis);
        CREDO_AEQUALIS_S32(d_round.dies, d_orig.dies);

        /* Alia probatio */
        d_orig = fasti_dies(MCMXCIX, XII, XXXI);
        jdn = fasti_ad_iulianum(d_orig);
        d_round = fasti_ex_iuliano(jdn);

        CREDO_AEQUALIS_S32(d_round.annus, d_orig.annus);
        CREDO_AEQUALIS_S32(d_round.mensis, d_orig.mensis);
        CREDO_AEQUALIS_S32(d_round.dies, d_orig.dies);

        /* 1 Ianuarii 2000 */
        d_orig = fasti_dies(MM, I, I);
        jdn = fasti_ad_iulianum(d_orig);
        d_round = fasti_ex_iuliano(jdn);

        CREDO_AEQUALIS_S32(d_round.annus, MM);
        CREDO_AEQUALIS_S32(d_round.mensis, I);
        CREDO_AEQUALIS_S32(d_round.dies, I);
    }


    /* ==================================================
     * Probare fasti_addere_dies
     * ================================================== */

    {
        Dies d, result;

        imprimere("\n--- Probans fasti_addere_dies ---\n");

        d = fasti_dies(MMXXIV, I, I);

        /* Addere 30 dies */
        result = fasti_addere_dies(d, XXX);
        CREDO_AEQUALIS_S32(result.annus, MMXXIV);
        CREDO_AEQUALIS_S32(result.mensis, I);
        CREDO_AEQUALIS_S32(result.dies, XXXI);

        /* Addere 31 dies (transire mensem) */
        result = fasti_addere_dies(d, XXXI);
        CREDO_AEQUALIS_S32(result.annus, MMXXIV);
        CREDO_AEQUALIS_S32(result.mensis, II);
        CREDO_AEQUALIS_S32(result.dies, I);

        /* Subtrahere dies (negativus) */
        d = fasti_dies(MMXXIV, III, I);
        result = fasti_addere_dies(d, -I);
        CREDO_AEQUALIS_S32(result.annus, MMXXIV);
        CREDO_AEQUALIS_S32(result.mensis, II);
        CREDO_AEQUALIS_S32(result.dies, XXIX); /* 2024 est bissextilis */
    }


    /* ==================================================
     * Probare fasti_addere_menses
     * ================================================== */

    {
        Dies d, result;

        imprimere("\n--- Probans fasti_addere_menses ---\n");

        d = fasti_dies(MMXXIV, I, XV);

        /* Addere 2 menses */
        result = fasti_addere_menses(d, II);
        CREDO_AEQUALIS_S32(result.annus, MMXXIV);
        CREDO_AEQUALIS_S32(result.mensis, III);
        CREDO_AEQUALIS_S32(result.dies, XV);

        /* Addere 12 menses (transire annum) */
        result = fasti_addere_menses(d, XII);
        CREDO_AEQUALIS_S32(result.annus, MMXXV);
        CREDO_AEQUALIS_S32(result.mensis, I);
        CREDO_AEQUALIS_S32(result.dies, XV);

        /* 31 Ianuarii + 1 mensis = 29 Februarii (cohibitus) */
        d = fasti_dies(MMXXIV, I, XXXI);
        result = fasti_addere_menses(d, I);
        CREDO_AEQUALIS_S32(result.mensis, II);
        CREDO_AEQUALIS_S32(result.dies, XXIX); /* Cohibitus ad 29 */
    }


    /* ==================================================
     * Probare fasti_addere_annos
     * ================================================== */

    {
        Dies d, result;

        imprimere("\n--- Probans fasti_addere_annos ---\n");

        d = fasti_dies(MMXX, VI, XV);
        result = fasti_addere_annos(d, IV);
        CREDO_AEQUALIS_S32(result.annus, MMXXIV);
        CREDO_AEQUALIS_S32(result.mensis, VI);
        CREDO_AEQUALIS_S32(result.dies, XV);

        /* 29 Feb 2024 + 1 annus = 28 Feb 2025 (cohibitus) */
        d = fasti_dies(MMXXIV, II, XXIX);
        result = fasti_addere_annos(d, I);
        CREDO_AEQUALIS_S32(result.annus, MMXXV);
        CREDO_AEQUALIS_S32(result.mensis, II);
        CREDO_AEQUALIS_S32(result.dies, XXVIII);
    }


    /* ==================================================
     * Probare fasti_differentia
     * ================================================== */

    {
        Dies d1, d2;
        Intervallum i;

        imprimere("\n--- Probans fasti_differentia ---\n");

        d1 = fasti_dies(MMXXIV, I, I);
        d2 = fasti_dies(MMXXIV, I, XXXI);
        i = fasti_differentia(d2, d1);
        CREDO_AEQUALIS_S64(i.dies_totales, XXX);

        /* Differentia inversa */
        i = fasti_differentia(d1, d2);
        CREDO_AEQUALIS_S64(i.dies_totales, -XXX);

        /* Dies aequales */
        i = fasti_differentia(d1, d1);
        CREDO_AEQUALIS_S64(i.dies_totales, ZEPHYRUM);
    }


    /* ==================================================
     * Probare fasti_comparare et amici
     * ================================================== */

    {
        Dies d1, d2, d3;

        imprimere("\n--- Probans comparatio ---\n");

        d1 = fasti_dies(MMXXIV, I, I);
        d2 = fasti_dies(MMXXIV, VI, XV);
        d3 = fasti_dies(MMXXIV, I, I);

        CREDO_AEQUALIS_S32(fasti_comparare(d1, d2), -I);
        CREDO_AEQUALIS_S32(fasti_comparare(d2, d1), I);
        CREDO_AEQUALIS_S32(fasti_comparare(d1, d3), ZEPHYRUM);

        CREDO_VERUM(fasti_ante(d1, d2));
        CREDO_FALSUM(fasti_ante(d2, d1));

        CREDO_VERUM(fasti_post(d2, d1));
        CREDO_FALSUM(fasti_post(d1, d2));

        CREDO_VERUM(fasti_aequalis(d1, d3));
        CREDO_FALSUM(fasti_aequalis(d1, d2));
    }


    /* ==================================================
     * Probare fasti_inter
     * ================================================== */

    {
        Dies d, ab, ad;

        imprimere("\n--- Probans fasti_inter ---\n");

        ab = fasti_dies(MMXXIV, I, I);
        ad = fasti_dies(MMXXIV, XII, XXXI);

        d = fasti_dies(MMXXIV, VI, XV);
        CREDO_VERUM(fasti_inter(d, ab, ad));

        d = fasti_dies(MMXXIII, VI, XV);
        CREDO_FALSUM(fasti_inter(d, ab, ad));

        /* Limites sunt inclusivi */
        CREDO_VERUM(fasti_inter(ab, ab, ad));
        CREDO_VERUM(fasti_inter(ad, ab, ad));
    }


    /* ==================================================
     * Probare fasti_computus (Easter)
     * ================================================== */

    {
        Dies pascha;

        imprimere("\n--- Probans fasti_computus ---\n");

        /* Pascha 2024: 31 Martii */
        pascha = fasti_computus(MMXXIV);
        CREDO_AEQUALIS_S32(pascha.annus, MMXXIV);
        CREDO_AEQUALIS_S32(pascha.mensis, III);
        CREDO_AEQUALIS_S32(pascha.dies, XXXI);

        /* Pascha 2023: 9 Aprilis */
        pascha = fasti_computus(MMXXIII);
        CREDO_AEQUALIS_S32(pascha.annus, MMXXIII);
        CREDO_AEQUALIS_S32(pascha.mensis, IV);
        CREDO_AEQUALIS_S32(pascha.dies, IX);

        /* Pascha 2025: 20 Aprilis */
        pascha = fasti_computus(MMXXV);
        CREDO_AEQUALIS_S32(pascha.annus, MMXXV);
        CREDO_AEQUALIS_S32(pascha.mensis, IV);
        CREDO_AEQUALIS_S32(pascha.dies, XX);

        /* Pascha 2000: 23 Aprilis */
        pascha = fasti_computus(MM);
        CREDO_AEQUALIS_S32(pascha.annus, MM);
        CREDO_AEQUALIS_S32(pascha.mensis, IV);
        CREDO_AEQUALIS_S32(pascha.dies, XXIII);

        /* Verificare quod Pascha semper est Dominica */
        CREDO_AEQUALIS_S32(fasti_dies_hebdomadis(fasti_computus(MMXXIV)), FASTI_DOMINICA);
        CREDO_AEQUALIS_S32(fasti_dies_hebdomadis(fasti_computus(MMXXIII)), FASTI_DOMINICA);
        CREDO_AEQUALIS_S32(fasti_dies_hebdomadis(fasti_computus(MMXXV)), FASTI_DOMINICA);
    }


    /* ==================================================
     * Probare fasti_nomen_mensis
     * ================================================== */

    {
        chorda titulus;

        imprimere("\n--- Probans fasti_nomen_mensis ---\n");

        titulus = fasti_nomen_mensis(III, FALSUM, FASTI_LINGUA_LATINA, piscina);
        CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Martius", piscina));

        titulus = fasti_nomen_mensis(III, VERUM, FASTI_LINGUA_LATINA, piscina);
        CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Mar", piscina));

        titulus = fasti_nomen_mensis(III, FALSUM, FASTI_LINGUA_ANGLICA, piscina);
        CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("March", piscina));

        titulus = fasti_nomen_mensis(XII, VERUM, FASTI_LINGUA_ANGLICA, piscina);
        CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Dec", piscina));
    }


    /* ==================================================
     * Probare fasti_nomen_diei_hebdomadis
     * ================================================== */

    {
        chorda titulus;

        imprimere("\n--- Probans fasti_nomen_diei_hebdomadis ---\n");

        titulus = fasti_nomen_diei_hebdomadis(FASTI_DOMINICA, FALSUM, FASTI_LINGUA_LATINA, piscina);
        CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Dominica", piscina));

        titulus = fasti_nomen_diei_hebdomadis(FASTI_FERIA_II, VERUM, FASTI_LINGUA_LATINA, piscina);
        CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Fer II", piscina));

        titulus = fasti_nomen_diei_hebdomadis(FASTI_DOMINICA, FALSUM, FASTI_LINGUA_ANGLICA, piscina);
        CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Sunday", piscina));
    }


    /* ==================================================
     * Probare fasti_formare_diem
     * ================================================== */

    {
        Dies d;
        chorda s;

        imprimere("\n--- Probans fasti_formare_diem ---\n");

        d = fasti_dies(MMXXIV, III, XV);

        s = fasti_formare_diem(d, FASTI_FORMA_ISO, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("2024-03-15", piscina));

        s = fasti_formare_diem(d, FASTI_FORMA_EUROPAEA, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("15/03/2024", piscina));

        s = fasti_formare_diem(d, FASTI_FORMA_AMERICANA, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("03/15/2024", piscina));

        s = fasti_formare_diem(d, FASTI_FORMA_LATINA_BREVIS, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("15 Mar 2024", piscina));
    }


    /* ==================================================
     * Probare fasti_formare_horam
     * ================================================== */

    {
        Hora h;
        chorda s;

        imprimere("\n--- Probans fasti_formare_horam ---\n");

        h = fasti_hora(XIV, XXX, XLV);

        s = fasti_formare_horam(h, VERUM, VERUM, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("14:30:45", piscina));

        s = fasti_formare_horam(h, FALSUM, VERUM, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("14:30", piscina));

        s = fasti_formare_horam(h, FALSUM, FALSUM, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("2:30 PM", piscina));

        h = fasti_hora(ZEPHYRUM, V, ZEPHYRUM);
        s = fasti_formare_horam(h, FALSUM, FALSUM, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("12:05 AM", piscina));
    }


    /* ==================================================
     * Probare fasti_parsare_iso
     * ================================================== */

    {
        chorda s;
        Dies d;
        b32 successus;

        imprimere("\n--- Probans fasti_parsare_iso ---\n");

        s = chorda_ex_literis("2024-03-15", piscina);
        successus = fasti_parsare_iso(s, &d);
        CREDO_VERUM(successus);
        CREDO_AEQUALIS_S32(d.annus, MMXXIV);
        CREDO_AEQUALIS_S32(d.mensis, III);
        CREDO_AEQUALIS_S32(d.dies, XV);

        /* Data invalida */
        s = chorda_ex_literis("2024-02-31", piscina);
        successus = fasti_parsare_iso(s, &d);
        CREDO_FALSUM(successus);

        /* Forma invalida */
        s = chorda_ex_literis("not-a-date", piscina);
        successus = fasti_parsare_iso(s, &d);
        CREDO_FALSUM(successus);
    }


    /* ==================================================
     * Probare fasti_dies_anni
     * ================================================== */

    {
        Dies d;

        imprimere("\n--- Probans fasti_dies_anni ---\n");

        /* 1 Ianuarii = dies 1 */
        d = fasti_dies(MMXXIV, I, I);
        CREDO_AEQUALIS_S32(fasti_dies_anni(d), I);

        /* 31 Decembris 2024 (bissextilis) = dies 366 */
        d = fasti_dies(MMXXIV, XII, XXXI);
        CREDO_AEQUALIS_S32(fasti_dies_anni(d), CCCLXVI);

        /* 31 Decembris 2023 (non-bissextilis) = dies 365 */
        d = fasti_dies(MMXXIII, XII, XXXI);
        CREDO_AEQUALIS_S32(fasti_dies_anni(d), CCCLXV);
    }


    /* ==================================================
     * Probare fasti_primus_dies_hebdomadis_mensis
     * ================================================== */

    {
        imprimere("\n--- Probans fasti_primus_dies_hebdomadis_mensis ---\n");

        /* Ianuarius 2024 incipit Feria II */
        CREDO_AEQUALIS_S32(fasti_primus_dies_hebdomadis_mensis(I, MMXXIV), FASTI_FERIA_II);

        /* Martius 2024 incipit Feria VI */
        CREDO_AEQUALIS_S32(fasti_primus_dies_hebdomadis_mensis(III, MMXXIV), FASTI_FERIA_VI);
    }


    /* ==================================================
     * Probare conversio Unix
     * ================================================== */

    {
        DiesHora dh, dh_round;
        s64 timestamp;

        imprimere("\n--- Probans conversio Unix ---\n");

        /* Unix epoch */
        dh.dies = fasti_dies(MCMX + LXX, I, I);
        dh.hora = fasti_hora(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        timestamp = fasti_ad_unix(dh);
        CREDO_AEQUALIS_S64(timestamp, ZEPHYRUM);

        /* Round-trip */
        dh.dies = fasti_dies(MMXXIV, III, XV);
        dh.hora = fasti_hora(XII, XXX, XLV);
        timestamp = fasti_ad_unix(dh);
        dh_round = fasti_ex_unix(timestamp);

        CREDO_AEQUALIS_S32(dh_round.dies.annus, MMXXIV);
        CREDO_AEQUALIS_S32(dh_round.dies.mensis, III);
        CREDO_AEQUALIS_S32(dh_round.dies.dies, XV);
        CREDO_AEQUALIS_S32(dh_round.hora.hora, XII);
        CREDO_AEQUALIS_S32(dh_round.hora.minutum, XXX);
        CREDO_AEQUALIS_S32(dh_round.hora.secundum, XLV);
    }


    /* ==================================================
     * Probare fasti_dies_hodie (functio systematis)
     * ================================================== */

    {
        Dies hodie;

        imprimere("\n--- Probans fasti_dies_hodie ---\n");

        hodie = fasti_dies_hodie();

        /* Dies hodierna debet esse valida */
        CREDO_VERUM(fasti_dies_valida(hodie));

        /* Annus debet esse rationabilis (post 2020, ante 2100) */
        CREDO_VERUM(hodie.annus >= MMXX);
        CREDO_VERUM(hodie.annus < MMC);
    }


    /* ==================================================
     * Probare fasti_suffixum_ordinale
     * ================================================== */

    {
        constans character* suff;

        imprimere("\n--- Probans fasti_suffixum_ordinale ---\n");

        /* 1st, 2nd, 3rd */
        suff = fasti_suffixum_ordinale(I);
        CREDO_AEQUALIS_S32(suff[0], 's');
        CREDO_AEQUALIS_S32(suff[1], 't');

        suff = fasti_suffixum_ordinale(II);
        CREDO_AEQUALIS_S32(suff[0], 'n');
        CREDO_AEQUALIS_S32(suff[1], 'd');

        suff = fasti_suffixum_ordinale(III);
        CREDO_AEQUALIS_S32(suff[0], 'r');
        CREDO_AEQUALIS_S32(suff[1], 'd');

        /* 4th - 10th are all "th" */
        suff = fasti_suffixum_ordinale(IV);
        CREDO_AEQUALIS_S32(suff[0], 't');
        CREDO_AEQUALIS_S32(suff[1], 'h');

        suff = fasti_suffixum_ordinale(X);
        CREDO_AEQUALIS_S32(suff[0], 't');
        CREDO_AEQUALIS_S32(suff[1], 'h');

        /* 11th, 12th, 13th are exceptions (all "th") */
        suff = fasti_suffixum_ordinale(XI);
        CREDO_AEQUALIS_S32(suff[0], 't');
        CREDO_AEQUALIS_S32(suff[1], 'h');

        suff = fasti_suffixum_ordinale(XII);
        CREDO_AEQUALIS_S32(suff[0], 't');
        CREDO_AEQUALIS_S32(suff[1], 'h');

        suff = fasti_suffixum_ordinale(XIII);
        CREDO_AEQUALIS_S32(suff[0], 't');
        CREDO_AEQUALIS_S32(suff[1], 'h');

        /* 21st, 22nd, 23rd follow normal pattern */
        suff = fasti_suffixum_ordinale(XXI);
        CREDO_AEQUALIS_S32(suff[0], 's');
        CREDO_AEQUALIS_S32(suff[1], 't');

        suff = fasti_suffixum_ordinale(XXII);
        CREDO_AEQUALIS_S32(suff[0], 'n');
        CREDO_AEQUALIS_S32(suff[1], 'd');

        suff = fasti_suffixum_ordinale(XXIII);
        CREDO_AEQUALIS_S32(suff[0], 'r');
        CREDO_AEQUALIS_S32(suff[1], 'd');

        /* 31st */
        suff = fasti_suffixum_ordinale(XXXI);
        CREDO_AEQUALIS_S32(suff[0], 's');
        CREDO_AEQUALIS_S32(suff[1], 't');
    }


    /* ==================================================
     * Probare FASTI_FORMA_ANGLICA_LONGA
     * ================================================== */

    {
        Dies d;
        chorda s;

        imprimere("\n--- Probans FASTI_FORMA_ANGLICA_LONGA ---\n");

        /* Nativitas Domini - Thursday, December 25th 2025 */
        d = fasti_dies(MMXXV, XII, XXV);
        s = fasti_formare_diem(d, FASTI_FORMA_ANGLICA_LONGA, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("Thursday, December 25th 2025", piscina));

        /* 1 Ianuarii 2024 = Monday, January 1st 2024 */
        d = fasti_dies(MMXXIV, I, I);
        s = fasti_formare_diem(d, FASTI_FORMA_ANGLICA_LONGA, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("Monday, January 1st 2024", piscina));

        /* 22 Februarii = "22nd" */
        d = fasti_dies(MMXXIV, II, XXII);
        s = fasti_formare_diem(d, FASTI_FORMA_ANGLICA_LONGA, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("Thursday, February 22nd 2024", piscina));

        /* 13 Martii = "13th" (teen exception) */
        d = fasti_dies(MMXXIV, III, XIII);
        s = fasti_formare_diem(d, FASTI_FORMA_ANGLICA_LONGA, piscina);
        CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("Wednesday, March 13th 2024", piscina));
    }


    /* ==================================================
     * Conclusio
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde (praeteritus) ? ZEPHYRUM : I;
}
