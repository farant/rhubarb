#include "fasti.h"
#include <sys/time.h>
#include <time.h>


/* ==================================================
 * Tabulae Internae
 * ================================================== */

/* Dies in mense (non-bissextilis) */
interior constans s32 DIES_IN_MENSE[XII] = {
    XXXI,   /* Ianuarius */
    XXVIII, /* Februarius */
    XXXI,   /* Martius */
    XXX,    /* Aprilis */
    XXXI,   /* Maius */
    XXX,    /* Iunius */
    XXXI,   /* Iulius */
    XXXI,   /* Augustus */
    XXX,    /* September */
    XXXI,   /* October */
    XXX,    /* November */
    XXXI    /* December */
};

/* Nomina mensium Latina */
interior constans character* NOMINA_MENSIUM_LATINA[XII] = {
    "Ianuarius", "Februarius", "Martius", "Aprilis",
    "Maius", "Iunius", "Iulius", "Augustus",
    "September", "October", "November", "December"
};

interior constans character* NOMINA_MENSIUM_LATINA_BREVIA[XII] = {
    "Ian", "Feb", "Mar", "Apr", "Mai", "Iun",
    "Iul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* Nomina mensium Anglica */
interior constans character* NOMINA_MENSIUM_ANGLICA[XII] = {
    "January", "February", "March", "April",
    "May", "June", "July", "August",
    "September", "October", "November", "December"
};

interior constans character* NOMINA_MENSIUM_ANGLICA_BREVIA[XII] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* Nomina dierum hebdomadis Latina */
interior constans character* NOMINA_DIERUM_LATINA[VII] = {
    "Dominica", "Feria II", "Feria III", "Feria IV",
    "Feria V", "Feria VI", "Sabbatum"
};

interior constans character* NOMINA_DIERUM_LATINA_BREVIA[VII] = {
    "Dom", "Fer II", "Fer III", "Fer IV",
    "Fer V", "Fer VI", "Sab"
};

/* Nomina dierum hebdomadis Anglica */
interior constans character* NOMINA_DIERUM_ANGLICA[VII] = {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
};

interior constans character* NOMINA_DIERUM_ANGLICA_BREVIA[VII] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};


/* ==================================================
 * Constructores
 * ================================================== */

Dies
fasti_dies(
    s32 annus,
    s32 mensis,
    s32 dies)
{
    Dies d;
    d.annus = annus;
    d.mensis = mensis;
    d.dies = dies;
    redde d;
}

Hora
fasti_hora(
    s32 hora,
    s32 minutum,
    s32 secundum)
{
    Hora h;
    h.hora = hora;
    h.minutum = minutum;
    h.secundum = secundum;
    redde h;
}

Dies
fasti_dies_hodie(
    vacuum)
{
    structura timeval tv;
    structura tm* tm_info;
    time_t secunda;
    Dies d;

    gettimeofday(&tv, NIHIL);
    secunda = tv.tv_sec;
    tm_info = localtime(&secunda);

    d.annus = (s32)(tm_info->tm_year + MCM);
    d.mensis = (s32)(tm_info->tm_mon + I);
    d.dies = (s32)(tm_info->tm_mday);

    redde d;
}

DiesHora
fasti_nunc(
    vacuum)
{
    structura timeval tv;
    structura tm* tm_info;
    time_t secunda;
    DiesHora dh;

    gettimeofday(&tv, NIHIL);
    secunda = tv.tv_sec;
    tm_info = localtime(&secunda);

    dh.dies.annus = (s32)(tm_info->tm_year + MCM);
    dh.dies.mensis = (s32)(tm_info->tm_mon + I);
    dh.dies.dies = (s32)(tm_info->tm_mday);
    dh.hora.hora = (s32)(tm_info->tm_hour);
    dh.hora.minutum = (s32)(tm_info->tm_min);
    dh.hora.secundum = (s32)(tm_info->tm_sec);

    redde dh;
}


/* ==================================================
 * Quaestiones Calendarii
 * ================================================== */

b32
fasti_est_bissextilis(
    s32 annus)
{
    /* Regulae Gregorianae:
     * - Divisibilis per 4: bissextilis
     * - Sed divisibilis per 100: non bissextilis
     * - Sed divisibilis per 400: bissextilis
     */
    si ((annus % CD) == ZEPHYRUM) {
        redde VERUM;
    }
    si ((annus % C) == ZEPHYRUM) {
        redde FALSUM;
    }
    si ((annus % IV) == ZEPHYRUM) {
        redde VERUM;
    }
    redde FALSUM;
}

s32
fasti_dies_in_mense(
    s32 mensis,
    s32 annus)
{
    si (mensis < I || mensis > XII) {
        redde ZEPHYRUM;
    }

    si (mensis == FASTI_FEBRUARIUS && fasti_est_bissextilis(annus)) {
        redde XXIX;
    }

    redde DIES_IN_MENSE[mensis - I];
}


/* ==================================================
 * Validatio
 * ================================================== */

b32
fasti_dies_valida(
    Dies dies)
{
    s32 dies_max;

    si (dies.mensis < I || dies.mensis > XII) {
        redde FALSUM;
    }

    dies_max = fasti_dies_in_mense(dies.mensis, dies.annus);

    si (dies.dies < I || dies.dies > dies_max) {
        redde FALSUM;
    }

    redde VERUM;
}

b32
fasti_hora_valida(
    Hora hora)
{
    si (hora.hora < ZEPHYRUM || hora.hora > XXIII) {
        redde FALSUM;
    }
    si (hora.minutum < ZEPHYRUM || hora.minutum > LIX) {
        redde FALSUM;
    }
    si (hora.secundum < ZEPHYRUM || hora.secundum > LIX) {
        redde FALSUM;
    }
    redde VERUM;
}


/* ==================================================
 * Conversio Iuliana
 *
 * Algorithmus pro conversione inter dies Gregorianas
 * et numeros Iulianos. Haec est basis omnium
 * computationum arithmeticarum.
 * ================================================== */

s64
fasti_ad_iulianum(
    Dies dies)
{
    /* Algorithmus: formula Iuliana pro calendario Gregoriano
     *
     * Haec formula convertet diem Gregorianam ad numerum
     * diei Iulianae (JDN).
     */
    s64 a, y, m;
    s64 jdn;

    a = (s64)((XIV - dies.mensis) / XII);
    y = (s64)(dies.annus) + 4800LL - a;
    m = (s64)(dies.mensis) + XII * a - III;

    /* Formula Gregoriana */
    jdn = (s64)(dies.dies)
        + (153LL * m + II) / V
        + 365LL * y
        + y / IV
        - y / C
        + y / CD
        - 32045LL;

    redde jdn;
}

Dies
fasti_ex_iuliano(
    s64 iulianus)
{
    /* Algorithmus inversus: JDN ad diem Gregorianam */
    s64 a, b, c, d, e, m;
    Dies dies;

    a = iulianus + 32044LL;
    b = (IV * a + III) / 146097LL;
    c = a - (146097LL * b) / IV;
    d = (IV * c + III) / 1461LL;
    e = c - (1461LL * d) / IV;
    m = (V * e + II) / 153LL;

    dies.dies = (s32)(e - (153LL * m + II) / V + I);
    dies.mensis = (s32)(m + III - XII * (m / X));
    dies.annus = (s32)(C * b + d - 4800LL + m / X);

    redde dies;
}


/* ==================================================
 * Dies Hebdomadis
 * ================================================== */

s32
fasti_dies_hebdomadis(
    Dies dies)
{
    /* Usans numeros Iulianos.
     * JDN % 7 dat diem hebdomadis ubi 0 = Lunae (Monday).
     * Nos volumus 0 = Dominica, ergo adiustamus.
     */
    s64 jdn = fasti_ad_iulianum(dies);
    s32 dh = (s32)((jdn + I) % VII);
    redde dh;
}

s32
fasti_dies_anni(
    Dies dies)
{
    /* Dies anni: 1 Ian = 1, 31 Dec = 365 (vel 366) */
    Dies prima = fasti_dies(dies.annus, I, I);
    s64 jdn_dies = fasti_ad_iulianum(dies);
    s64 jdn_prima = fasti_ad_iulianum(prima);
    redde (s32)(jdn_dies - jdn_prima + I);
}

s32
fasti_hebdomada_anni(
    Dies dies)
{
    /* ISO 8601 hebdomada:
     * - Hebdomada incipit Feria II (Monday)
     * - Prima hebdomada continet primam Feriam V anni
     *
     * Algorithmus:
     * 1. Inveni primam Feriam V anni
     * 2. Computa quot hebdomadae inter illam et diem datam
     */
    Dies ian_iv;
    s32 dh_ian_iv;
    s64 jdn_dies, jdn_prima_feria_ii;
    s32 hebdomada;

    /* 4 Ianuarii semper est in prima hebdomada ISO */
    ian_iv = fasti_dies(dies.annus, I, IV);
    dh_ian_iv = fasti_dies_hebdomadis(ian_iv);

    /* Converte ad Monday=0 pro computatione ISO */
    /* Dominica (0) -> 6, Feria II (1) -> 0, etc. */
    si (dh_ian_iv == ZEPHYRUM) {
        dh_ian_iv = VI;
    } alioquin {
        dh_ian_iv = dh_ian_iv - I;
    }

    /* Prima Feria II primae hebdomadis */
    jdn_prima_feria_ii = fasti_ad_iulianum(ian_iv) - (s64)dh_ian_iv;
    jdn_dies = fasti_ad_iulianum(dies);

    si (jdn_dies < jdn_prima_feria_ii) {
        /* Dies est in ultima hebdomada anni praecedentis */
        redde fasti_hebdomada_anni(fasti_dies(dies.annus - I, XII, XXVIII));
    }

    hebdomada = (s32)((jdn_dies - jdn_prima_feria_ii) / VII) + I;

    si (hebdomada > LII) {
        /* Verifica si revera est hebdomada 53 vel 1 anni sequentis */
        Dies ian_iv_seq = fasti_dies(dies.annus + I, I, IV);
        s32 dh_seq = fasti_dies_hebdomadis(ian_iv_seq);
        s64 jdn_seq;

        si (dh_seq == ZEPHYRUM) {
            dh_seq = VI;
        } alioquin {
            dh_seq = dh_seq - I;
        }

        jdn_seq = fasti_ad_iulianum(ian_iv_seq) - (s64)dh_seq;

        si (jdn_dies >= jdn_seq) {
            redde I;
        }
    }

    redde hebdomada;
}

Dies
fasti_prima_mensis(
    s32 mensis,
    s32 annus)
{
    redde fasti_dies(annus, mensis, I);
}

Dies
fasti_ultima_mensis(
    s32 mensis,
    s32 annus)
{
    redde fasti_dies(annus, mensis, fasti_dies_in_mense(mensis, annus));
}

s32
fasti_primus_dies_hebdomadis_mensis(
    s32 mensis,
    s32 annus)
{
    Dies prima = fasti_dies(annus, mensis, I);
    redde fasti_dies_hebdomadis(prima);
}


/* ==================================================
 * Arithmetica Dierum
 * ================================================== */

Dies
fasti_addere_dies(
    Dies dies,
    s32  numerus)
{
    s64 jdn = fasti_ad_iulianum(dies);
    jdn = jdn + (s64)numerus;
    redde fasti_ex_iuliano(jdn);
}

Dies
fasti_addere_menses(
    Dies dies,
    s32  numerus)
{
    s32 menses_totales;
    s32 dies_max;
    Dies fructus;

    menses_totales = (dies.annus * XII) + (dies.mensis - I) + numerus;

    fructus.annus = menses_totales / XII;
    fructus.mensis = (menses_totales % XII) + I;

    /* Tracta menses negativos */
    si (fructus.mensis < I) {
        fructus.mensis = fructus.mensis + XII;
        fructus.annus = fructus.annus - I;
    }

    /* Cohibere diem si necessarium */
    dies_max = fasti_dies_in_mense(fructus.mensis, fructus.annus);
    si (dies.dies > dies_max) {
        fructus.dies = dies_max;
    } alioquin {
        fructus.dies = dies.dies;
    }

    redde fructus;
}

Dies
fasti_addere_annos(
    Dies dies,
    s32  numerus)
{
    Dies fructus;
    s32 dies_max;

    fructus.annus = dies.annus + numerus;
    fructus.mensis = dies.mensis;

    /* Cohibere diem (e.g., 29 Feb in non-bissextili) */
    dies_max = fasti_dies_in_mense(fructus.mensis, fructus.annus);
    si (dies.dies > dies_max) {
        fructus.dies = dies_max;
    } alioquin {
        fructus.dies = dies.dies;
    }

    redde fructus;
}

Intervallum
fasti_differentia(
    Dies prima,
    Dies secunda)
{
    Intervallum i;
    s64 jdn_prima = fasti_ad_iulianum(prima);
    s64 jdn_secunda = fasti_ad_iulianum(secunda);
    i.dies_totales = jdn_prima - jdn_secunda;
    redde i;
}


/* ==================================================
 * Comparatio
 * ================================================== */

s32
fasti_comparare(
    Dies prima,
    Dies secunda)
{
    s64 jdn_prima = fasti_ad_iulianum(prima);
    s64 jdn_secunda = fasti_ad_iulianum(secunda);

    si (jdn_prima < jdn_secunda) {
        redde -I;
    } alioquin si (jdn_prima > jdn_secunda) {
        redde I;
    }
    redde ZEPHYRUM;
}

b32
fasti_ante(
    Dies prima,
    Dies secunda)
{
    redde fasti_comparare(prima, secunda) < ZEPHYRUM;
}

b32
fasti_post(
    Dies prima,
    Dies secunda)
{
    redde fasti_comparare(prima, secunda) > ZEPHYRUM;
}

b32
fasti_aequalis(
    Dies prima,
    Dies secunda)
{
    redde fasti_comparare(prima, secunda) == ZEPHYRUM;
}

b32
fasti_inter(
    Dies dies,
    Dies ab,
    Dies ad)
{
    redde !fasti_ante(dies, ab) && !fasti_post(dies, ad);
}


/* ==================================================
 * Conversio Unix
 * ================================================== */

s64
fasti_ad_unix(
    DiesHora dh)
{
    /* Unix epoch: 1970-01-01 00:00:00 UTC */
    Dies epoch = fasti_dies(MCMX + LXX, I, I);
    s64 jdn_dies = fasti_ad_iulianum(dh.dies);
    s64 jdn_epoch = fasti_ad_iulianum(epoch);
    s64 dies_diff = jdn_dies - jdn_epoch;
    s64 secunda;

    secunda = dies_diff * 86400LL;
    secunda = secunda + (s64)(dh.hora.hora) * 3600LL;
    secunda = secunda + (s64)(dh.hora.minutum) * LX;
    secunda = secunda + (s64)(dh.hora.secundum);

    redde secunda;
}

DiesHora
fasti_ex_unix(
    s64 timestamp)
{
    Dies epoch = fasti_dies(MCMX + LXX, I, I);
    s64 jdn_epoch = fasti_ad_iulianum(epoch);
    s64 dies_diff;
    s64 secunda_diei;
    DiesHora dh;

    dies_diff = timestamp / 86400LL;
    secunda_diei = timestamp % 86400LL;

    /* Tracta timestamps negativos */
    si (secunda_diei < ZEPHYRUM) {
        secunda_diei = secunda_diei + 86400LL;
        dies_diff = dies_diff - I;
    }

    dh.dies = fasti_ex_iuliano(jdn_epoch + dies_diff);
    dh.hora.hora = (s32)(secunda_diei / 3600LL);
    dh.hora.minutum = (s32)((secunda_diei % 3600LL) / LX);
    dh.hora.secundum = (s32)(secunda_diei % LX);

    redde dh;
}


/* ==================================================
 * Formatatio
 * ================================================== */

constans character*
fasti_suffixum_ordinale(s32 numerus)
{
    s32 absolutus;

    /* Uti valore absoluto */
    absolutus = numerus < ZEPHYRUM ? -numerus : numerus;

    /* Casus speciales: 11, 12, 13 semper "th" */
    si (absolutus >= XI && absolutus <= XIII) {
        redde "th";
    }

    /* Secundum ultimam cifram */
    commutatio (absolutus % X) {
        casus I:   redde "st";
        casus II:  redde "nd";
        casus III: redde "rd";
        ordinarius: redde "th";
    }
}

chorda
fasti_nomen_mensis(
    s32      mensis,
    b32      abbreviatum,
    s32      lingua,
    Piscina* piscina)
{
    constans character* titulis;

    si (mensis < I || mensis > XII) {
        redde chorda_ex_literis("", piscina);
    }

    si (lingua == FASTI_LINGUA_ANGLICA) {
        si (abbreviatum) {
            titulis = NOMINA_MENSIUM_ANGLICA_BREVIA[mensis - I];
        } alioquin {
            titulis = NOMINA_MENSIUM_ANGLICA[mensis - I];
        }
    } alioquin {
        si (abbreviatum) {
            titulis = NOMINA_MENSIUM_LATINA_BREVIA[mensis - I];
        } alioquin {
            titulis = NOMINA_MENSIUM_LATINA[mensis - I];
        }
    }

    redde chorda_ex_literis(titulis, piscina);
}

chorda
fasti_nomen_diei_hebdomadis(
    s32      dies_hebdomadis,
    b32      abbreviatum,
    s32      lingua,
    Piscina* piscina)
{
    constans character* titulis;

    si (dies_hebdomadis < ZEPHYRUM || dies_hebdomadis > VI) {
        redde chorda_ex_literis("", piscina);
    }

    si (lingua == FASTI_LINGUA_ANGLICA) {
        si (abbreviatum) {
            titulis = NOMINA_DIERUM_ANGLICA_BREVIA[dies_hebdomadis];
        } alioquin {
            titulis = NOMINA_DIERUM_ANGLICA[dies_hebdomadis];
        }
    } alioquin {
        si (abbreviatum) {
            titulis = NOMINA_DIERUM_LATINA_BREVIA[dies_hebdomadis];
        } alioquin {
            titulis = NOMINA_DIERUM_LATINA[dies_hebdomadis];
        }
    }

    redde chorda_ex_literis(titulis, piscina);
}

vacuum
fasti_scribere_diem(
    ChordaAedificator* aedificator,
    Dies               dies,
    s32                forma)
{
    commutatio (forma) {
        casus FASTI_FORMA_ISO:
            /* YYYY-MM-DD */
            chorda_aedificator_appendere_s32(aedificator, dies.annus);
            chorda_aedificator_appendere_character(aedificator, '-');
            si (dies.mensis < X) {
                chorda_aedificator_appendere_character(aedificator, '0');
            }
            chorda_aedificator_appendere_s32(aedificator, dies.mensis);
            chorda_aedificator_appendere_character(aedificator, '-');
            si (dies.dies < X) {
                chorda_aedificator_appendere_character(aedificator, '0');
            }
            chorda_aedificator_appendere_s32(aedificator, dies.dies);
            frange;

        casus FASTI_FORMA_EUROPAEA:
            /* DD/MM/YYYY */
            si (dies.dies < X) {
                chorda_aedificator_appendere_character(aedificator, '0');
            }
            chorda_aedificator_appendere_s32(aedificator, dies.dies);
            chorda_aedificator_appendere_character(aedificator, '/');
            si (dies.mensis < X) {
                chorda_aedificator_appendere_character(aedificator, '0');
            }
            chorda_aedificator_appendere_s32(aedificator, dies.mensis);
            chorda_aedificator_appendere_character(aedificator, '/');
            chorda_aedificator_appendere_s32(aedificator, dies.annus);
            frange;

        casus FASTI_FORMA_AMERICANA:
            /* MM/DD/YYYY */
            si (dies.mensis < X) {
                chorda_aedificator_appendere_character(aedificator, '0');
            }
            chorda_aedificator_appendere_s32(aedificator, dies.mensis);
            chorda_aedificator_appendere_character(aedificator, '/');
            si (dies.dies < X) {
                chorda_aedificator_appendere_character(aedificator, '0');
            }
            chorda_aedificator_appendere_s32(aedificator, dies.dies);
            chorda_aedificator_appendere_character(aedificator, '/');
            chorda_aedificator_appendere_s32(aedificator, dies.annus);
            frange;

        casus FASTI_FORMA_LATINA_BREVIS:
            /* 15 Mar 2024 */
            chorda_aedificator_appendere_s32(aedificator, dies.dies);
            chorda_aedificator_appendere_character(aedificator, ' ');
            chorda_aedificator_appendere_literis(aedificator,
                NOMINA_MENSIUM_LATINA_BREVIA[dies.mensis - I]);
            chorda_aedificator_appendere_character(aedificator, ' ');
            chorda_aedificator_appendere_s32(aedificator, dies.annus);
            frange;

        casus FASTI_FORMA_LATINA_LONGA:
            /* XV Martii MMXXIV - simplificatum: 15 Martii 2024 */
            chorda_aedificator_appendere_s32(aedificator, dies.dies);
            chorda_aedificator_appendere_character(aedificator, ' ');
            chorda_aedificator_appendere_literis(aedificator,
                NOMINA_MENSIUM_LATINA[dies.mensis - I]);
            chorda_aedificator_appendere_character(aedificator, ' ');
            chorda_aedificator_appendere_s32(aedificator, dies.annus);
            frange;

        casus FASTI_FORMA_LITURGICA: {
            /* Dominica, 15 Martii 2024 */
            s32 dh = fasti_dies_hebdomadis(dies);
            chorda_aedificator_appendere_literis(aedificator,
                NOMINA_DIERUM_LATINA[dh]);
            chorda_aedificator_appendere_literis(aedificator, ", ");
            chorda_aedificator_appendere_s32(aedificator, dies.dies);
            chorda_aedificator_appendere_character(aedificator, ' ');
            chorda_aedificator_appendere_literis(aedificator,
                NOMINA_MENSIUM_LATINA[dies.mensis - I]);
            chorda_aedificator_appendere_character(aedificator, ' ');
            chorda_aedificator_appendere_s32(aedificator, dies.annus);
            frange;
        }

        casus FASTI_FORMA_ANGLICA_LONGA: {
            /* Wednesday - December 24th, 2025 AD */
            s32 dh = fasti_dies_hebdomadis(dies);
            chorda_aedificator_appendere_literis(aedificator,
                NOMINA_DIERUM_ANGLICA[dh]);
            chorda_aedificator_appendere_literis(aedificator, " - ");
            chorda_aedificator_appendere_literis(aedificator,
                NOMINA_MENSIUM_ANGLICA[dies.mensis - I]);
            chorda_aedificator_appendere_character(aedificator, ' ');
            chorda_aedificator_appendere_s32(aedificator, dies.dies);
            chorda_aedificator_appendere_literis(aedificator,
                fasti_suffixum_ordinale(dies.dies));
            chorda_aedificator_appendere_literis(aedificator, ", ");
            chorda_aedificator_appendere_s32(aedificator, dies.annus);
            chorda_aedificator_appendere_literis(aedificator, " AD");
            frange;
        }

        ordinarius:
            /* ISO fallback */
            chorda_aedificator_appendere_s32(aedificator, dies.annus);
            chorda_aedificator_appendere_character(aedificator, '-');
            si (dies.mensis < X) {
                chorda_aedificator_appendere_character(aedificator, '0');
            }
            chorda_aedificator_appendere_s32(aedificator, dies.mensis);
            chorda_aedificator_appendere_character(aedificator, '-');
            si (dies.dies < X) {
                chorda_aedificator_appendere_character(aedificator, '0');
            }
            chorda_aedificator_appendere_s32(aedificator, dies.dies);
            frange;
    }
}

vacuum
fasti_scribere_horam(
    ChordaAedificator* aedificator,
    Hora               hora,
    b32                cum_secundis,
    b32                xxiv_horae)
{
    s32 h = hora.hora;
    constans character* meridies = "";

    si (!xxiv_horae) {
        si (h == ZEPHYRUM) {
            h = XII;
            meridies = " AM";
        } alioquin si (h < XII) {
            meridies = " AM";
        } alioquin si (h == XII) {
            meridies = " PM";
        } alioquin {
            h = h - XII;
            meridies = " PM";
        }
    }

    si (xxiv_horae && h < X) {
        chorda_aedificator_appendere_character(aedificator, '0');
    }
    chorda_aedificator_appendere_s32(aedificator, h);
    chorda_aedificator_appendere_character(aedificator, ':');
    si (hora.minutum < X) {
        chorda_aedificator_appendere_character(aedificator, '0');
    }
    chorda_aedificator_appendere_s32(aedificator, hora.minutum);

    si (cum_secundis) {
        chorda_aedificator_appendere_character(aedificator, ':');
        si (hora.secundum < X) {
            chorda_aedificator_appendere_character(aedificator, '0');
        }
        chorda_aedificator_appendere_s32(aedificator, hora.secundum);
    }

    si (!xxiv_horae) {
        chorda_aedificator_appendere_literis(aedificator, meridies);
    }
}

chorda
fasti_formare_diem(
    Dies      dies,
    s32       forma,
    Piscina*  piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, XXXII);
    fasti_scribere_diem(aed, dies, forma);
    redde chorda_aedificator_finire(aed);
}

chorda
fasti_formare_horam(
    Hora     hora,
    b32      cum_secundis,
    b32      xxiv_horae,
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, XVI);
    fasti_scribere_horam(aed, hora, cum_secundis, xxiv_horae);
    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Parsing
 * ================================================== */

interior b32
_parsare_duo_digiti(
    chorda s,
    s32    index,
    s32*   fructus)
{
    s32 d1, d2;

    si (index + I >= (s32)s.mensura) {
        redde FALSUM;
    }

    d1 = s.datum[index] - '0';
    d2 = s.datum[index + I] - '0';

    si (d1 < ZEPHYRUM || d1 > IX || d2 < ZEPHYRUM || d2 > IX) {
        redde FALSUM;
    }

    *fructus = d1 * X + d2;
    redde VERUM;
}

b32
fasti_parsare_iso(
    chorda  s,
    Dies*   fructus)
{
    /* YYYY-MM-DD (10 characteres) */
    s32 annus = ZEPHYRUM;
    s32 mensis = ZEPHYRUM;
    s32 dies = ZEPHYRUM;
    s32 i;
    s32 signum = I;
    s32 index = ZEPHYRUM;

    si (s.mensura < X) {
        redde FALSUM;
    }

    /* Tractare annum negativum */
    si (s.datum[ZEPHYRUM] == '-') {
        signum = -I;
        index = I;
        si (s.mensura < XI) {
            redde FALSUM;
        }
    }

    /* Parsare annum (4 digiti) */
    per (i = ZEPHYRUM; i < IV; i = i + I) {
        s32 d = s.datum[index + i] - '0';
        si (d < ZEPHYRUM || d > IX) {
            redde FALSUM;
        }
        annus = annus * X + d;
    }
    annus = annus * signum;
    index = index + IV;

    /* Separatorem '-' */
    si (s.datum[index] != '-') {
        redde FALSUM;
    }
    index = index + I;

    /* Parsare mensem */
    si (!_parsare_duo_digiti(s, index, &mensis)) {
        redde FALSUM;
    }
    index = index + II;

    /* Separatorem '-' */
    si (s.datum[index] != '-') {
        redde FALSUM;
    }
    index = index + I;

    /* Parsare diem */
    si (!_parsare_duo_digiti(s, index, &dies)) {
        redde FALSUM;
    }

    fructus->annus = annus;
    fructus->mensis = mensis;
    fructus->dies = dies;

    redde fasti_dies_valida(*fructus);
}

b32
fasti_parsare_diem(
    chorda  s,
    Dies*   fructus)
{
    /* Tentat varias formas */

    /* Prima: ISO (YYYY-MM-DD) */
    si (fasti_parsare_iso(s, fructus)) {
        redde VERUM;
    }

    /* TODO: addere alias formas (DD/MM/YYYY, MM/DD/YYYY) */

    redde FALSUM;
}


/* ==================================================
 * Computus Paschalis
 *
 * Algorithmus anonymus Gregorianus pro computando
 * diem Paschatis. Pascha est prima Dominica post
 * primam lunam plenam vernali aequinoctio.
 * ================================================== */

Dies
fasti_computus(
    s32 annus)
{
    /* Algorithmus anonymus Gregorianus (Meeus/Jones/Butcher) */
    s32 a, b, c, d, e, f, g, h, i, k, l, m;
    s32 mensis, dies;

    a = annus % XIX;
    b = annus / C;
    c = annus % C;
    d = b / IV;
    e = b % IV;
    f = (b + VIII) / XXV;
    g = (b - f + I) / III;
    h = (XIX * a + b - d - g + XV) % XXX;
    i = c / IV;
    k = c % IV;
    l = (XXXII + II * e + II * i - h - k) % VII;
    m = (a + XI * h + XXII * l) / CDLI;

    mensis = (h + l - VII * m + CXIV) / XXXI;
    dies = ((h + l - VII * m + CXIV) % XXXI) + I;

    redde fasti_dies(annus, mensis, dies);
}


/* ==================================================
 * Festivitates Mobiles
 * ================================================== */

Dies
fasti_feria_vi(
    s32 annus)
{
    redde fasti_addere_dies(fasti_computus(annus), -II);
}

Dies
fasti_feria_iv_cinerum(
    s32 annus)
{
    redde fasti_addere_dies(fasti_computus(annus), -XLVI);
}

Dies
fasti_dominica_palmarum(
    s32 annus)
{
    redde fasti_addere_dies(fasti_computus(annus), -VII);
}

Dies
fasti_pentecoste(
    s32 annus)
{
    redde fasti_addere_dies(fasti_computus(annus), XLIX);
}

constans character*
fasti_nomen_festivitatis(
    Dies dies)
{
    Dies pascha;

    pascha = fasti_computus(dies.annus);

    /* Verificare festivitates in ordine praecedentiae */
    si (fasti_aequalis(dies, pascha))
    {
        redde "Easter";
    }

    si (fasti_aequalis(dies, fasti_pentecoste(dies.annus)))
    {
        redde "Pentecost";
    }

    si (fasti_aequalis(dies, fasti_feria_vi(dies.annus)))
    {
        redde "Good Friday";
    }

    si (fasti_aequalis(dies, fasti_feria_iv_cinerum(dies.annus)))
    {
        redde "Ash Wednesday";
    }

    si (fasti_aequalis(dies, fasti_dominica_palmarum(dies.annus)))
    {
        redde "Palm Sunday";
    }

    redde NIHIL;
}
