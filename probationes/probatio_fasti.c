/* probatio_fasti.c - Probationes Fasti (Calendarium) */
#include "latina.h"
#include "fasti.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


/* ==================================================
 * Probare fasti_dies constructor
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d;

        d = fasti_dies(MMXXIV, III, XV);
        CREDO_AEQUALIS_S32(d.annus, MMXXIV);
        CREDO_AEQUALIS_S32(d.mensis, III);
        CREDO_AEQUALIS_S32(d.dies, XV);
    }
}


/* ==================================================
 * Probare fasti_hora constructor
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Hora h;

        h = fasti_hora(XIV, XXX, XLV);
        CREDO_AEQUALIS_S32(h.hora, XIV);
        CREDO_AEQUALIS_S32(h.minutum, XXX);
        CREDO_AEQUALIS_S32(h.secundum, XLV);
    }
}


/* ==================================================
 * Probare fasti_est_bissextilis
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
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
}


/* ==================================================
 * Probare fasti_dies_in_mense
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
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
        CREDO_AEQUALIS_S32(fasti_dies_in_mense(ZEPHYRUM, MMXXIV),
            ZEPHYRUM);
    }
}


/* ==================================================
 * Probare fasti_dies_valida
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d_valida, d_invalida1, d_invalida2, d_invalida3;

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
}


/* ==================================================
 * Probare fasti_dies_hebdomadis
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d;
         s32 dh;

        /* 1 Ianuarii 2024 erat Feria II (Monday) = 1 */
        d   = fasti_dies(MMXXIV, I, I);
        dh  = fasti_dies_hebdomadis(d);
        CREDO_AEQUALIS_S32(dh, FASTI_FERIA_II);

        /* 25 Decembris 2024 est Feria IV (Wednesday) = 3 */
        d   = fasti_dies(MMXXIV, XII, XXV);
        dh  = fasti_dies_hebdomadis(d);
        CREDO_AEQUALIS_S32(dh, FASTI_FERIA_IV);

        /* 1 Ianuarii 2000 erat Sabbatum = 6 */
        d   = fasti_dies(MM, I, I);
        dh  = fasti_dies_hebdomadis(d);
        CREDO_AEQUALIS_S32(dh, FASTI_SABBATUM);

        /* 4 Iulii 1776 erat Feria V (Thursday) = 4 */
        d   = fasti_dies(MDCCLXXVI, VII, IV);
        dh  = fasti_dies_hebdomadis(d);
        CREDO_AEQUALIS_S32(dh, FASTI_FERIA_V);
    }
}


/* ==================================================
 * Probare fasti_ad_iulianum et fasti_ex_iuliano
 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d_orig, d_round;
        s64 jdn;

        /* Round-trip probatio */
        d_orig   = fasti_dies(MMXXIV, III, XV);
        jdn      = fasti_ad_iulianum(d_orig);
        d_round  = fasti_ex_iuliano(jdn);

        CREDO_AEQUALIS_S32(d_round.annus, d_orig.annus);
        CREDO_AEQUALIS_S32(d_round.mensis, d_orig.mensis);
        CREDO_AEQUALIS_S32(d_round.dies, d_orig.dies);

        /* Alia probatio */
        d_orig   = fasti_dies(MCMXCIX, XII, XXXI);
        jdn      = fasti_ad_iulianum(d_orig);
        d_round  = fasti_ex_iuliano(jdn);

        CREDO_AEQUALIS_S32(d_round.annus, d_orig.annus);
        CREDO_AEQUALIS_S32(d_round.mensis, d_orig.mensis);
        CREDO_AEQUALIS_S32(d_round.dies, d_orig.dies);

        /* 1 Ianuarii 2000 */
        d_orig   = fasti_dies(MM, I, I);
        jdn      = fasti_ad_iulianum(d_orig);
        d_round  = fasti_ex_iuliano(jdn);

        CREDO_AEQUALIS_S32(d_round.annus, MM);
        CREDO_AEQUALIS_S32(d_round.mensis, I);
        CREDO_AEQUALIS_S32(d_round.dies, I);
    }
}


/* ==================================================
 * Probare fasti_addere_dies
 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d, result;

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
        d       = fasti_dies(MMXXIV, III, I);
        result  = fasti_addere_dies(d, -I);
        CREDO_AEQUALIS_S32(result.annus, MMXXIV);
        CREDO_AEQUALIS_S32(result.mensis, II);
        CREDO_AEQUALIS_S32(result.dies, XXIX); /* 2024 est bissextilis */
    }
}


/* ==================================================
 * Probare fasti_addere_menses
 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d, result;

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
        d       = fasti_dies(MMXXIV, I, XXXI);
        result  = fasti_addere_menses(d, I);
        CREDO_AEQUALIS_S32(result.mensis, II);
        CREDO_AEQUALIS_S32(result.dies, XXIX); /* Cohibitus ad 29 */
    }
}


/* ==================================================
 * Probare fasti_addere_annos
 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d, result;

        d       = fasti_dies(MMXX, VI, XV);
        result  = fasti_addere_annos(d, IV);
        CREDO_AEQUALIS_S32(result.annus, MMXXIV);
        CREDO_AEQUALIS_S32(result.mensis, VI);
        CREDO_AEQUALIS_S32(result.dies, XV);

        /* 29 Feb 2024 + 1 annus = 28 Feb 2025 (cohibitus) */
        d       = fasti_dies(MMXXIV, II, XXIX);
        result  = fasti_addere_annos(d, I);
        CREDO_AEQUALIS_S32(result.annus, MMXXV);
        CREDO_AEQUALIS_S32(result.mensis, II);
        CREDO_AEQUALIS_S32(result.dies, XXVIII);
    }
}


/* ==================================================
 * Probare fasti_differentia
 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d1, d2;
        Intervallum i;

        d1  = fasti_dies(MMXXIV, I, I);
        d2  = fasti_dies(MMXXIV, I, XXXI);
        i   = fasti_differentia(d2, d1);
        CREDO_AEQUALIS_S64(i.dies_totales, XXX);

        /* Differentia inversa */
        i = fasti_differentia(d1, d2);
        CREDO_AEQUALIS_S64(i.dies_totales, -XXX);

        /* Dies aequales */
        i = fasti_differentia(d1, d1);
        CREDO_AEQUALIS_S64(i.dies_totales, ZEPHYRUM);
    }
}


/* ==================================================
 * Probare fasti_comparare et amici
 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d1, d2, d3;

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
}


/* ==================================================
 * Probare fasti_inter
 * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d, ab, ad;

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
}


/* ==================================================
 * Probare fasti_computus (Easter)
 * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies pascha;

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
        CREDO_AEQUALIS_S32(fasti_dies_hebdomadis(fasti_computus(MMXXIV)),
            FASTI_DOMINICA);
        CREDO_AEQUALIS_S32(fasti_dies_hebdomadis(fasti_computus(MMXXIII)),
            FASTI_DOMINICA);
        CREDO_AEQUALIS_S32(fasti_dies_hebdomadis(fasti_computus(MMXXV)),
            FASTI_DOMINICA);
    }
}


/* ==================================================
 * Probare fasti_nomen_mensis
 * ================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    chorda titulus;

    titulus = fasti_nomen_mensis(III, FALSUM, FASTI_LINGUA_LATINA,
        c->piscina);
    CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Martius",
        c->piscina));

    titulus = fasti_nomen_mensis(III, VERUM, FASTI_LINGUA_LATINA,
        c->piscina);
    CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Mar",
        c->piscina));

    titulus = fasti_nomen_mensis(III, FALSUM, FASTI_LINGUA_ANGLICA,
        c->piscina);
    CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("March",
        c->piscina));

    titulus = fasti_nomen_mensis(XII, VERUM, FASTI_LINGUA_ANGLICA,
        c->piscina);
    CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Dec",
        c->piscina));
}


/* ==================================================
 * Probare fasti_nomen_diei_hebdomadis
 * ================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    chorda titulus;

    titulus = fasti_nomen_diei_hebdomadis(FASTI_DOMINICA, FALSUM,
        FASTI_LINGUA_LATINA, c->piscina);
    CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Dominica",
        c->piscina));

    titulus = fasti_nomen_diei_hebdomadis(FASTI_FERIA_II, VERUM,
        FASTI_LINGUA_LATINA, c->piscina);
    CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Fer II",
        c->piscina));

    titulus = fasti_nomen_diei_hebdomadis(FASTI_DOMINICA, FALSUM,
        FASTI_LINGUA_ANGLICA, c->piscina);
    CREDO_CHORDA_AEQUALIS(titulus, chorda_ex_literis("Sunday",
        c->piscina));
}


/* ==================================================
 * Probare fasti_formare_diem
 * ================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
      Dies d;
    chorda s;

    d = fasti_dies(MMXXIV, III, XV);

    s = fasti_formare_diem(d, FASTI_FORMA_ISO, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("2024-03-15",
        c->piscina));

    s = fasti_formare_diem(d, FASTI_FORMA_EUROPAEA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("15/03/2024",
        c->piscina));

    s = fasti_formare_diem(d, FASTI_FORMA_AMERICANA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("03/15/2024",
        c->piscina));

    s = fasti_formare_diem(d, FASTI_FORMA_LATINA_BREVIS, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("15 Mar 2024",
        c->piscina));

    s = fasti_formare_diem(d, FASTI_FORMA_LATINA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("XV Martii MMXXIV",
        c->piscina));

    s = fasti_formare_diem(d, FASTI_FORMA_LITURGICA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis(
        "Feria VI, 15 Martii 2024", c->piscina));
}


/* ==================================================
 * Probare formas Latinas: mensis GENETIVO, numeri Romani
 *
 * "XV Martii" = dies quintus decimus Martii (mensis):
 * nomen mensis in dato genetivum est. Numeri Romani
 * I..MMMCMXCIX; extra limites cifrae scribuntur.
 * ================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    interior constans character* constans CALENDAE[XII] = {
        "I Ianuarii MMXXVI",   "I Februarii MMXXVI",
        "I Martii MMXXVI",     "I Aprilis MMXXVI",
        "I Maii MMXXVI",       "I Iunii MMXXVI",
        "I Iulii MMXXVI",      "I Augusti MMXXVI",
        "I Septembris MMXXVI", "I Octobris MMXXVI",
        "I Novembris MMXXVI",  "I Decembris MMXXVI"
    };
    chorda s;
       s32 m;

    per (m = I; m <= XII; m++)
    {
        s = fasti_formare_diem(fasti_dies(MM + XXVI, m, I),
            FASTI_FORMA_LATINA_LONGA, c->piscina);
        CREDO_CHORDA_AEQUALIS(s,
            chorda_ex_literis(CALENDAE[m - I], c->piscina));
    }

    /* formae subtractivae: CM, XC, IX, IV */
    s = fasti_formare_diem(fasti_dies(MCMXCIX, XII, XXXI),
        FASTI_FORMA_LATINA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("XXXI Decembris MCMXCIX", c->piscina));

    s = fasti_formare_diem(fasti_dies(IV * M - I, VIII, XXIV),
        FASTI_FORMA_LATINA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("XXIV Augusti MMMCMXCIX", c->piscina));

    /* longissimus sub IV milibus: 3888 */
    s = fasti_formare_diem(fasti_dies(IV * M - C - XII, VII, XIX),
        FASTI_FORMA_LATINA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("XIX Iulii MMMDCCCLXXXVIII", c->piscina));

    /* extra limites: cifrae, non numerus Romanus falsus */
    s = fasti_formare_diem(fasti_dies(IV * M, I, IX),
        FASTI_FORMA_LATINA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("IX Ianuarii 4000", c->piscina));

    s = fasti_formare_diem(fasti_dies(ZEPHYRUM, IV, XIV),
        FASTI_FORMA_LATINA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("XIV Aprilis 0", c->piscina));

    /* forma liturgica: dies et annus cifris, mensis genetivo */
    s = fasti_formare_diem(fasti_dies(MM + XXVI, IX, XIV),
        FASTI_FORMA_LITURGICA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("Feria II, 14 Septembris 2026", c->piscina));
}


/* ==================================================
 * Probare fasti_formare_horam
 * ================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
      Hora h;
    chorda s;

    h = fasti_hora(XIV, XXX, XLV);

    s = fasti_formare_horam(h, VERUM, VERUM, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("14:30:45",
        c->piscina));

    s = fasti_formare_horam(h, FALSUM, VERUM, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("14:30", c->piscina));

    s = fasti_formare_horam(h, FALSUM, FALSUM, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("2:30 PM", c->piscina));

    h = fasti_hora(ZEPHYRUM, V, ZEPHYRUM);
    s = fasti_formare_horam(h, FALSUM, FALSUM, c->piscina);
    CREDO_CHORDA_AEQUALIS(s, chorda_ex_literis("12:05 AM",
        c->piscina));
}


/* ==================================================
 * Probare fasti_parsare_iso
 * ================================================== */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    chorda s;
      Dies d;
       b32 successus;

    s          = chorda_ex_literis("2024-03-15", c->piscina);
    successus  = fasti_parsare_iso(s, &d);
    CREDO_VERUM(successus);
    CREDO_AEQUALIS_S32(d.annus, MMXXIV);
    CREDO_AEQUALIS_S32(d.mensis, III);
    CREDO_AEQUALIS_S32(d.dies, XV);

    /* Data invalida */
    s          = chorda_ex_literis("2024-02-31", c->piscina);
    successus  = fasti_parsare_iso(s, &d);
    CREDO_FALSUM(successus);

    /* Forma invalida */
    s          = chorda_ex_literis("not-a-date", c->piscina);
    successus  = fasti_parsare_iso(s, &d);
    CREDO_FALSUM(successus);
}


/* ==================================================
 * Probare fasti_dies_anni
 * ================================================== */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies d;

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
}


/* ==================================================
 * Probare fasti_primus_dies_hebdomadis_mensis
 * ================================================== */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        /* Ianuarius 2024 incipit Feria II */
        CREDO_AEQUALIS_S32(fasti_primus_dies_hebdomadis_mensis(I,
            MMXXIV), FASTI_FERIA_II);

        /* Martius 2024 incipit Feria VI */
        CREDO_AEQUALIS_S32(fasti_primus_dies_hebdomadis_mensis(III,
            MMXXIV), FASTI_FERIA_VI);
    }
}


/* ==================================================
 * Probare conversio Unix
 * ================================================== */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
        DiesHora dh, dh_round;
        s64 timestamp;

        /* Unix epoch */
        dh.dies    = fasti_dies(MCMX + LXX, I, I);
        dh.hora    = fasti_hora(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
        timestamp  = fasti_ad_unix(dh);
        CREDO_AEQUALIS_S64(timestamp, ZEPHYRUM);

        /* Round-trip */
        dh.dies    = fasti_dies(MMXXIV, III, XV);
        dh.hora    = fasti_hora(XII, XXX, XLV);
        timestamp  = fasti_ad_unix(dh);
        dh_round   = fasti_ex_unix(timestamp);

        CREDO_AEQUALIS_S32(dh_round.dies.annus, MMXXIV);
        CREDO_AEQUALIS_S32(dh_round.dies.mensis, III);
        CREDO_AEQUALIS_S32(dh_round.dies.dies, XV);
        CREDO_AEQUALIS_S32(dh_round.hora.hora, XII);
        CREDO_AEQUALIS_S32(dh_round.hora.minutum, XXX);
        CREDO_AEQUALIS_S32(dh_round.hora.secundum, XLV);
    }
}


/* ==================================================
 * Probare fasti_dies_hodie (functio systematis)
 * ================================================== */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Dies hodie;

        hodie = fasti_dies_hodie();

        /* Dies hodierna debet esse valida */
        CREDO_VERUM(fasti_dies_valida(hodie));

        /* Annus debet esse rationabilis (post 2020, ante 2100) */
        CREDO_VERUM(hodie.annus >= MMXX);
        CREDO_VERUM(hodie.annus < MMC);
    }
}


/* ==================================================
 * Probare fasti_suffixum_ordinale
 * ================================================== */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        constans character* suff;

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
}


/* ==================================================
 * Probare FASTI_FORMA_ANGLICA_LONGA
 * ================================================== */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
      Dies d;
    chorda s;

    /* Nativitas Domini - Thursday - December 25th, 2025 AD */
    d = fasti_dies(MMXXV, XII, XXV);
    s = fasti_formare_diem(d, FASTI_FORMA_ANGLICA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("Thursday - December 25th, 2025 AD",
        c->piscina));

    /* 1 Ianuarii 2024 = Monday - January 1st, 2024 AD */
    d = fasti_dies(MMXXIV, I, I);
    s = fasti_formare_diem(d, FASTI_FORMA_ANGLICA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("Monday - January 1st, 2024 AD",
        c->piscina));

    /* 22 Februarii = "22nd" */
    d = fasti_dies(MMXXIV, II, XXII);
    s = fasti_formare_diem(d, FASTI_FORMA_ANGLICA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("Thursday - February 22nd, 2024 AD",
        c->piscina));

    /* 13 Martii = "13th" (teen exception) */
    d = fasti_dies(MMXXIV, III, XIII);
    s = fasti_formare_diem(d, FASTI_FORMA_ANGLICA_LONGA, c->piscina);
    CREDO_CHORDA_AEQUALIS(s,
        chorda_ex_literis("Wednesday - March 13th, 2024 AD",
        c->piscina));
}


/* ==================================================
 * Probare fasti_ad_iso / fasti_ex_iso (instans plenum, UTC)
 * ================================================== */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
    DiesHora dh;
    DiesHora reditus;
      chorda s;

    dh.dies  = fasti_dies(2026, 9, 11);
    dh.hora  = fasti_hora(2, 41, 32);
    s        = fasti_ad_iso(dh, c->piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(s, "2026-09-11T02:41:32Z");

    CREDO_VERUM(fasti_ex_iso(s, &reditus));
    CREDO_AEQUALIS_S32(reditus.dies.annus, 2026);
    CREDO_AEQUALIS_S32(reditus.dies.mensis, 9);
    CREDO_AEQUALIS_S32(reditus.dies.dies, 11);
    CREDO_AEQUALIS_S32(reditus.hora.hora, 2);
    CREDO_AEQUALIS_S32(reditus.hora.minutum, 41);
    CREDO_AEQUALIS_S32(reditus.hora.secundum, 32);

    /* offsetum ad UTC vertitur */
    CREDO_VERUM(fasti_ex_iso(
        chorda_ex_literis("2026-09-11T04:41:32+02:00", c->piscina),
        &reditus));
    CREDO_AEQUALIS_S32(reditus.hora.hora, 2);
    CREDO_AEQUALIS_S32(reditus.dies.dies, 11);

    /* offsetum trans medium noctis diem quoque mutat */
    CREDO_VERUM(fasti_ex_iso(
        chorda_ex_literis("2026-09-11T00:30:00+02:00", c->piscina),
        &reditus));
    CREDO_AEQUALIS_S32(reditus.dies.dies, 10);
    CREDO_AEQUALIS_S32(reditus.hora.hora, 22);

    /* offsetum negativum in alteram partem */
    CREDO_VERUM(fasti_ex_iso(
        chorda_ex_literis("2026-09-11T23:30:00-02:00", c->piscina),
        &reditus));
    CREDO_AEQUALIS_S32(reditus.dies.dies, 12);
    CREDO_AEQUALIS_S32(reditus.hora.hora, 1);

    /* recusationes nominatae */
    CREDO_FALSUM(fasti_ex_iso(
        chorda_ex_literis("2026-09-11", c->piscina), &reditus));
    CREDO_FALSUM(fasti_ex_iso(
        chorda_ex_literis("2026-09-11 02:41:32Z", c->piscina),
        &reditus));
    CREDO_FALSUM(fasti_ex_iso(
        chorda_ex_literis("2026-09-11T02:41:32", c->piscina),
        &reditus));
    CREDO_FALSUM(fasti_ex_iso(
        chorda_ex_literis("2026-13-11T02:41:32Z", c->piscina),
        &reditus));
    CREDO_FALSUM(fasti_ex_iso(
        chorda_ex_literis("2026-09-11T25:41:32Z", c->piscina),
        &reditus));
    /* fractiones secundorum: v1 RECUSAT nominatim */
    CREDO_FALSUM(fasti_ex_iso(
        chorda_ex_literis("2026-09-11T02:41:32.5Z", c->piscina),
        &reditus));

    /* ad_iso diem invalidam RECUSAT (chorda vacua) */
    dh.dies = fasti_dies(2026, 13, 11);
    CREDO_CHORDA_VACUA(fasti_ad_iso(dh, c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fasti_dies", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fasti_hora", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fasti_est_bissextilis", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_dies_in_mense", _sectio_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_dies_valida", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fasti_dies_hebdomadis", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "conversio Iuliana", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_addere_dies", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fasti_addere_menses", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fasti_addere_annos", _sectio_decima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_differentia", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "comparatio", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_inter", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_computus", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_nomen_mensis", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_nomen_diei_hebdomadis", _sectio_decima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "fasti_formare_diem", _sectio_decima_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "formas Latinas", _sectio_decima_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_formare_horam", _sectio_decima_nona, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_parsare_iso", _sectio_vicesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_dies_anni", _sectio_vicesima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_primus_dies_hebdomadis_mensis", _sectio_vicesima_secunda,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "conversio Unix", _sectio_vicesima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_dies_hodie", _sectio_vicesima_quarta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "fasti_suffixum_ordinale", _sectio_vicesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "FASTI_FORMA_ANGLICA_LONGA", _sectio_vicesima_sexta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "fasti_ad_iso / fasti_ex_iso", _sectio_vicesima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("fasti", SECTIONES);
}
