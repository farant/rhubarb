/* ==================================================
 * CALENDARIUM LITURGICUM
 * Bibliotheca Calendarii Liturgici Romani (Forma Ordinaria)
 *
 * Secundum Calendarium Romanum Generale (1969)
 * ================================================== */

#include "calendarium_liturgicum.h"
#include <string.h>
#include <stdio.h>


/* ==================================================
 * Structura Interna
 * ================================================== */

structura CalendariumLiturgicum {
    Piscina*         piscina;
    AnchoresMobiles* anchorae_cache[X];  /* Cache pro annis recentibus */
    s32              cache_indices[X];   /* Anni in cache */
    s32              cache_numerus;
};


/* ==================================================
 * Creatio et Destructio
 * ================================================== */

CalendariumLiturgicum*
calendarium_creare(Piscina* piscina)
{
    CalendariumLiturgicum* cal;
    s32 i;

    cal = (CalendariumLiturgicum*)piscina_allocare(
        piscina,
        magnitudo(CalendariumLiturgicum)
    );

    si (cal == NIHIL) {
        redde NIHIL;
    }

    cal->piscina = piscina;
    cal->cache_numerus = ZEPHYRUM;

    per (i = ZEPHYRUM; i < X; i++) {
        cal->anchorae_cache[i] = NIHIL;
        cal->cache_indices[i] = ZEPHYRUM;
    }

    redde cal;
}

vacuum
calendarium_destruere(CalendariumLiturgicum* cal)
{
    /* Omnia in piscina allocata sunt - nihil agendum */
    (vacuum)cal;
}


/* ==================================================
 * Anchorae Mobiles - Computatio
 * ================================================== */

interior AnchoresMobiles*
computare_anchorae(s32 annus, Piscina* piscina)
{
    AnchoresMobiles* anch;
    Dies pascha;
    Dies epiphania;
    s32 dies_hebdomadae;

    anch = (AnchoresMobiles*)piscina_allocare(
        piscina,
        magnitudo(AnchoresMobiles)
    );

    si (anch == NIHIL) {
        redde NIHIL;
    }

    anch->annus = annus;

    /* Pascha - fundamentum omnium */
    pascha = fasti_computus(annus);
    anch->pascha = pascha;

    /* Dies ex Paschate derivatae */
    anch->feria_iv_cinerum = fasti_addere_dies(pascha, -XLVI);
    anch->dominica_i_quadragesimae = fasti_addere_dies(pascha, -XLII);
    anch->dominica_palmarum = fasti_addere_dies(pascha, -VII);
    anch->feria_v_in_cena_domini = fasti_addere_dies(pascha, -III);
    anch->feria_vi_in_passione = fasti_addere_dies(pascha, -II);
    anch->sabbatum_sanctum = fasti_addere_dies(pascha, -I);
    anch->ascensio = fasti_addere_dies(pascha, XXXIX);
    anch->pentecoste = fasti_addere_dies(pascha, XLIX);
    anch->ss_trinitas = fasti_addere_dies(pascha, LVI);
    anch->corpus_christi = fasti_addere_dies(pascha, LX);
    anch->cor_iesu = fasti_addere_dies(pascha, LXVIII);

    /* Dominica I Adventus - 4 dominicae ante Nativitatem */
    /* Invenire dominicam in vel ante 24 Dec (Vigilia), haec est Dominica IV Adventus */
    /* Tunc subtrahere 21 dies pro Dominica I Adventus */
    {
        Dies vigilia = fasti_dies(annus, FASTI_DECEMBER, XXIV);
        s32 dies_ad_dominicam = fasti_dies_hebdomadis(vigilia);
        Dies dominica_iv = fasti_addere_dies(vigilia, -dies_ad_dominicam);
        anch->dominica_i_adventus = fasti_addere_dies(dominica_iv, -XXI);
    }

    /* Christus Rex - dominica ante Adventum I */
    anch->christus_rex = fasti_addere_dies(anch->dominica_i_adventus, -VII);

    /* Baptisma Domini - dominica post Epiphaniam */
    /* Si Epiphania (6 Ian) est dominica, Baptisma est die 7 vel 8 Ian */
    epiphania = fasti_dies(annus, FASTI_IANUARIUS, VI);
    dies_hebdomadae = fasti_dies_hebdomadis(epiphania);

    si (dies_hebdomadae == ZEPHYRUM) {
        /* Epiphania est dominica - Baptisma est feria II */
        anch->baptisma_domini = fasti_dies(annus, FASTI_IANUARIUS, VII);
    } alioquin {
        /* Baptisma est dominica proxima post Epiphaniam */
        anch->baptisma_domini = fasti_addere_dies(
            epiphania,
            VII - dies_hebdomadae
        );
    }

    redde anch;
}

AnchoresMobiles*
calendarium_anchorae(CalendariumLiturgicum* cal, s32 annus)
{
    s32 i;
    s32 index_minimus;
    AnchoresMobiles* anch;

    /* Quaerere in cache */
    per (i = ZEPHYRUM; i < cal->cache_numerus; i++) {
        si (cal->cache_indices[i] == annus) {
            redde cal->anchorae_cache[i];
        }
    }

    /* Non in cache - computare */
    anch = computare_anchorae(annus, cal->piscina);

    si (anch == NIHIL) {
        redde NIHIL;
    }

    /* Addere ad cache */
    si (cal->cache_numerus < X) {
        cal->anchorae_cache[cal->cache_numerus] = anch;
        cal->cache_indices[cal->cache_numerus] = annus;
        cal->cache_numerus++;
    } alioquin {
        /* Cache plenus - substituere minimum */
        index_minimus = ZEPHYRUM;
        per (i = I; i < X; i++) {
            si (cal->cache_indices[i] < cal->cache_indices[index_minimus]) {
                index_minimus = i;
            }
        }
        cal->anchorae_cache[index_minimus] = anch;
        cal->cache_indices[index_minimus] = annus;
    }

    redde anch;
}


/* ==================================================
 * Limites Temporum
 * ================================================== */

Dies
calendarium_initium_temporis(
    CalendariumLiturgicum* cal,
    TempusLiturgicum       tempus,
    s32                    annus)
{
    AnchoresMobiles* anch;

    commutatio (tempus) {
        casus TEMPUS_ADVENTUS:
            /* Annus liturgicus: Adventus anni N est in anno civili N-1 */
            anch = calendarium_anchorae(cal, annus - I);
            redde anch->dominica_i_adventus;

        casus TEMPUS_NATIVITATIS:
            redde fasti_dies(annus - I, FASTI_DECEMBER, XXV);

        casus TEMPUS_PER_ANNUM_I:
            anch = calendarium_anchorae(cal, annus);
            redde fasti_addere_dies(anch->baptisma_domini, I);

        casus TEMPUS_QUADRAGESIMAE:
            anch = calendarium_anchorae(cal, annus);
            redde anch->feria_iv_cinerum;

        casus TEMPUS_TRIDUUM_SACRUM:
            anch = calendarium_anchorae(cal, annus);
            redde anch->feria_v_in_cena_domini;

        casus TEMPUS_PASCHALE:
            anch = calendarium_anchorae(cal, annus);
            redde anch->pascha;

        casus TEMPUS_PER_ANNUM_II:
            anch = calendarium_anchorae(cal, annus);
            redde fasti_addere_dies(anch->pentecoste, I);

        ordinarius:
            redde fasti_dies(annus, FASTI_IANUARIUS, I);
    }
}

Dies
calendarium_finis_temporis(
    CalendariumLiturgicum* cal,
    TempusLiturgicum       tempus,
    s32                    annus)
{
    AnchoresMobiles* anch;

    commutatio (tempus) {
        casus TEMPUS_ADVENTUS:
            redde fasti_dies(annus - I, FASTI_DECEMBER, XXIV);

        casus TEMPUS_NATIVITATIS:
            anch = calendarium_anchorae(cal, annus);
            redde anch->baptisma_domini;

        casus TEMPUS_PER_ANNUM_I:
            anch = calendarium_anchorae(cal, annus);
            redde fasti_addere_dies(anch->feria_iv_cinerum, -I);

        casus TEMPUS_QUADRAGESIMAE:
            anch = calendarium_anchorae(cal, annus);
            redde fasti_addere_dies(anch->feria_v_in_cena_domini, -I);

        casus TEMPUS_TRIDUUM_SACRUM:
            anch = calendarium_anchorae(cal, annus);
            redde fasti_addere_dies(anch->pascha, -I);

        casus TEMPUS_PASCHALE:
            anch = calendarium_anchorae(cal, annus);
            redde anch->pentecoste;

        casus TEMPUS_PER_ANNUM_II:
            anch = calendarium_anchorae(cal, annus + I);
            redde fasti_addere_dies(anch->dominica_i_adventus, -I);

        ordinarius:
            redde fasti_dies(annus, FASTI_DECEMBER, XXXI);
    }
}


/* ==================================================
 * Quaestiones Temporis
 * ================================================== */

TempusLiturgicum
calendarium_tempus(CalendariumLiturgicum* cal, Dies dies)
{
    s32 annus = dies.annus;
    AnchoresMobiles* anch;

    /* Obtinere anchorae pro anno currente */
    anch = calendarium_anchorae(cal, annus);

    /* ================================================
     * NATIVITAS (Dec 25 - Baptisma Domini)
     * Prioritas maxima pro hac periodo
     * ================================================ */

    /* Dec 25-31: Tempus Nativitatis */
    si (dies.mensis == FASTI_DECEMBER && dies.dies >= XXV) {
        redde TEMPUS_NATIVITATIS;
    }

    /* Ian 1 - Baptisma Domini: Tempus Nativitatis */
    si (dies.mensis == FASTI_IANUARIUS &&
        fasti_comparare(dies, anch->baptisma_domini) <= ZEPHYRUM) {
        redde TEMPUS_NATIVITATIS;
    }

    /* ================================================
     * ADVENTUS (Dom I Adv - Dec 24)
     * ================================================ */

    /* Si in Adventu (post Dom I Adv et ante Dec 25) */
    si (fasti_comparare(dies, anch->dominica_i_adventus) >= ZEPHYRUM &&
        dies.mensis != FASTI_IANUARIUS) {
        redde TEMPUS_ADVENTUS;
    }

    /* ================================================
     * CYCLUS PASCHALIS
     * ================================================ */

    /* Si post Pentecosten */
    si (fasti_comparare(dies, anch->pentecoste) > ZEPHYRUM) {
        redde TEMPUS_PER_ANNUM_II;
    }

    /* Si Pentecoste */
    si (fasti_comparare(dies, anch->pentecoste) == ZEPHYRUM) {
        redde TEMPUS_PASCHALE;
    }

    /* Si post Pascha */
    si (fasti_comparare(dies, anch->pascha) >= ZEPHYRUM) {
        redde TEMPUS_PASCHALE;
    }

    /* Si in Triduo */
    si (fasti_comparare(dies, anch->feria_v_in_cena_domini) >= ZEPHYRUM) {
        redde TEMPUS_TRIDUUM_SACRUM;
    }

    /* Si post Feriam IV Cinerum */
    si (fasti_comparare(dies, anch->feria_iv_cinerum) >= ZEPHYRUM) {
        redde TEMPUS_QUADRAGESIMAE;
    }

    /* ================================================
     * TEMPUS PER ANNUM I (post Baptisma - ante Quadragesimam)
     * ================================================ */

    si (fasti_comparare(dies, anch->baptisma_domini) > ZEPHYRUM) {
        redde TEMPUS_PER_ANNUM_I;
    }

    /* ================================================
     * TEMPUS PER ANNUM II (Defectus)
     * ================================================ */

    redde TEMPUS_PER_ANNUM_II;
}

InformatioTemporis
calendarium_tempus_info(CalendariumLiturgicum* cal, Dies dies)
{
    InformatioTemporis info;
    TempusLiturgicum tempus;
    AnchoresMobiles* anch;
    Dies initium;
    s32 dies_ab_initio;

    tempus = calendarium_tempus(cal, dies);
    info.tempus = tempus;
    info.dies_in_hebdomada = fasti_dies_hebdomadis(dies);
    info.est_dominica = (info.dies_in_hebdomada == ZEPHYRUM) ? VERUM : FALSUM;

    anch = calendarium_anchorae(cal, dies.annus);

    commutatio (tempus) {
        casus TEMPUS_ADVENTUS:
            initium = anch->dominica_i_adventus;
            si (fasti_comparare(dies, initium) < ZEPHYRUM) {
                /* Adventus anni sequentis */
                initium = calendarium_anchorae(cal, dies.annus + I)->dominica_i_adventus;
            }
            dies_ab_initio = (s32)fasti_differentia(dies, initium).dies_totales;
            info.hebdomada = (dies_ab_initio / VII) + I;
            frange;

        casus TEMPUS_NATIVITATIS:
            /* Octava Nativitatis */
            info.hebdomada = I;
            frange;

        casus TEMPUS_PER_ANNUM_I:
            initium = anch->baptisma_domini;
            dies_ab_initio = (s32)fasti_differentia(dies, initium).dies_totales;
            info.hebdomada = (dies_ab_initio / VII) + I;
            frange;

        casus TEMPUS_QUADRAGESIMAE:
            initium = anch->feria_iv_cinerum;
            dies_ab_initio = (s32)fasti_differentia(dies, initium).dies_totales;
            /* Feria IV Cinerum est ante Dominicam I */
            info.hebdomada = (dies_ab_initio + IV) / VII;
            si (info.hebdomada == ZEPHYRUM) {
                info.hebdomada = I;  /* Post Cineres, ante Dom. I */
            }
            frange;

        casus TEMPUS_TRIDUUM_SACRUM:
            info.hebdomada = I;
            frange;

        casus TEMPUS_PASCHALE:
            initium = anch->pascha;
            dies_ab_initio = (s32)fasti_differentia(dies, initium).dies_totales;
            info.hebdomada = (dies_ab_initio / VII) + I;
            frange;

        casus TEMPUS_PER_ANNUM_II:
            /* Numeratio continua a Pentecoste */
            /* Hebdomada post Pentecosten est ~34 */
            initium = anch->pentecoste;
            dies_ab_initio = (s32)fasti_differentia(dies, initium).dies_totales;
            /* Calculus complexus - simplificamus */
            info.hebdomada = XXXIV -
                (s32)((fasti_differentia(
                    calendarium_anchorae(cal, dies.annus + I)->dominica_i_adventus,
                    dies
                ).dies_totales + VI) / VII);
            si (info.hebdomada < I) {
                info.hebdomada = I;
            }
            frange;

        ordinarius:
            info.hebdomada = I;
    }

    redde info;
}

ColorLiturgicus
calendarium_color_temporis(CalendariumLiturgicum* cal, Dies dies)
{
    TempusLiturgicum tempus = calendarium_tempus(cal, dies);
    AnchoresMobiles* anch;

    commutatio (tempus) {
        casus TEMPUS_ADVENTUS:
            /* Rosaceus Dominica III (Gaudete) */
            anch = calendarium_anchorae(cal, dies.annus);
            si (fasti_comparare(dies, anch->dominica_i_adventus) < ZEPHYRUM) {
                anch = calendarium_anchorae(cal, dies.annus + I);
            }
            si (fasti_differentia(dies, anch->dominica_i_adventus).dies_totales >= XIV &&
                fasti_differentia(dies, anch->dominica_i_adventus).dies_totales < XXI &&
                fasti_dies_hebdomadis(dies) == ZEPHYRUM) {
                redde COLOR_ROSACEUS;
            }
            redde COLOR_VIOLACEUS;

        casus TEMPUS_NATIVITATIS:
            redde COLOR_ALBUS;

        casus TEMPUS_PER_ANNUM_I:
        casus TEMPUS_PER_ANNUM_II:
            redde COLOR_VIRIDIS;

        casus TEMPUS_QUADRAGESIMAE:
            /* Rosaceus Dominica IV (Laetare) */
            anch = calendarium_anchorae(cal, dies.annus);
            si (fasti_differentia(dies, anch->dominica_i_quadragesimae).dies_totales >= XXI &&
                fasti_differentia(dies, anch->dominica_i_quadragesimae).dies_totales < XXVIII &&
                fasti_dies_hebdomadis(dies) == ZEPHYRUM) {
                redde COLOR_ROSACEUS;
            }
            redde COLOR_VIOLACEUS;

        casus TEMPUS_TRIDUUM_SACRUM:
            redde COLOR_RUBER;  /* Feria VI in Passione */

        casus TEMPUS_PASCHALE:
            /* Ruber in Pentecoste */
            anch = calendarium_anchorae(cal, dies.annus);
            si (fasti_comparare(dies, anch->pentecoste) == ZEPHYRUM) {
                redde COLOR_RUBER;
            }
            redde COLOR_ALBUS;

        ordinarius:
            redde COLOR_VIRIDIS;
    }
}


/* ==================================================
 * Quaestiones Cycli Lectionum
 * ================================================== */

CyclusDominicalis
calendarium_cyclus_dominicalis(s32 annus)
{
    /* Cyclus A = annus % 3 == 1 (2023, 2026, ...) */
    /* Cyclus B = annus % 3 == 2 (2024, 2027, ...) */
    /* Cyclus C = annus % 3 == 0 (2022, 2025, ...) */
    s32 residuum = annus % III;

    commutatio (residuum) {
        casus I:
            redde CYCLUS_A;
        casus II:
            redde CYCLUS_B;
        ordinarius:
            redde CYCLUS_C;
    }
}

CyclusQuotidianus
calendarium_cyclus_quotidianus(s32 annus)
{
    /* Cyclus I = annus impar */
    /* Cyclus II = annus par */
    redde (annus % II == I) ? CYCLUS_I : CYCLUS_II;
}

s32
calendarium_hebdomada_psalterii(CalendariumLiturgicum* cal, Dies dies)
{
    InformatioTemporis info = calendarium_tempus_info(cal, dies);

    /* Hebdomada psalterii 1-4 rotat */
    redde ((info.hebdomada - I) % IV) + I;
}


/* ==================================================
 * Dies Peculiares
 * ================================================== */

b32
calendarium_est_feria_cinerum(CalendariumLiturgicum* cal, Dies dies)
{
    AnchoresMobiles* anch = calendarium_anchorae(cal, dies.annus);
    redde fasti_comparare(dies, anch->feria_iv_cinerum) == ZEPHYRUM;
}

b32
calendarium_est_hebdomada_sancta(CalendariumLiturgicum* cal, Dies dies)
{
    AnchoresMobiles* anch = calendarium_anchorae(cal, dies.annus);
    Dies dominica_palmarum = anch->dominica_palmarum;

    redde fasti_comparare(dies, dominica_palmarum) >= ZEPHYRUM &&
           fasti_comparare(dies, anch->feria_v_in_cena_domini) < ZEPHYRUM;
}

b32
calendarium_est_triduum(CalendariumLiturgicum* cal, Dies dies)
{
    redde calendarium_tempus(cal, dies) == TEMPUS_TRIDUUM_SACRUM;
}

b32
calendarium_est_octava_paschae(CalendariumLiturgicum* cal, Dies dies)
{
    AnchoresMobiles* anch = calendarium_anchorae(cal, dies.annus);
    s64 diff = fasti_differentia(dies, anch->pascha).dies_totales;

    redde diff >= ZEPHYRUM && diff < VIII;
}

b32
calendarium_est_octava_nativitatis(CalendariumLiturgicum* cal, Dies dies)
{
    Dies nativitas;
    s64 diff;

    (vacuum)cal;

    si (dies.mensis == FASTI_DECEMBER && dies.dies >= XXV) {
        nativitas = fasti_dies(dies.annus, FASTI_DECEMBER, XXV);
        diff = fasti_differentia(dies, nativitas).dies_totales;
        redde diff >= ZEPHYRUM && diff < VIII;
    }

    si (dies.mensis == FASTI_IANUARIUS && dies.dies == I) {
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * Invenire
 * ================================================== */

Dies
calendarium_proxima_dominica(Dies dies)
{
    s32 dies_hebdomadae = fasti_dies_hebdomadis(dies);

    si (dies_hebdomadae == ZEPHYRUM) {
        redde dies;
    }

    redde fasti_addere_dies(dies, VII - dies_hebdomadae);
}

Dies
calendarium_invenire_festum(
    CalendariumLiturgicum* cal,
    chorda                 titulus,
    s32                    annus)
{
    /* TODO: Implementare post sanctorale */
    (vacuum)cal;
    (vacuum)titulus;
    (vacuum)annus;

    redde fasti_dies(ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
}


/* ==================================================
 * Formatatio
 * ================================================== */

interior constans character* NOMINA_TEMPORUM[] = {
    "Tempus Adventus",
    "Tempus Nativitatis",
    "Tempus per Annum",
    "Tempus Quadragesimae",
    "Triduum Sacrum",
    "Tempus Paschale",
    "Tempus per Annum"
};

interior constans character* NOMINA_TEMPORUM_BREVIA[] = {
    "Adv",
    "Nat",
    "Ord",
    "Quad",
    "Trid",
    "Pasch",
    "Ord"
};

interior constans character* NOMINA_GRADUUM[] = {
    "Feria",
    "Memoria ad libitum",
    "Memoria",
    "Festum",
    "Sollemnitas",
    "Dominica",
    "Dies peculiaris"
};

interior constans character* NOMINA_GRADUUM_BREVIA[] = {
    "fer",
    "mem opt",
    "mem",
    "fest",
    "sol",
    "Dom",
    "dies"
};

interior constans character* NOMINA_COLORUM[] = {
    "Albus",
    "Ruber",
    "Viridis",
    "Violaceus",
    "Rosaceus",
    "Niger"
};

chorda
calendarium_nomen_temporis(
    TempusLiturgicum tempus,
    b32              abbreviatum,
    Piscina*         piscina)
{
    constans character* titulus;

    si (tempus < ZEPHYRUM || tempus > TEMPUS_PER_ANNUM_II) {
        redde chorda_ex_literis("", piscina);
    }

    titulus = abbreviatum ?
        NOMINA_TEMPORUM_BREVIA[tempus] :
        NOMINA_TEMPORUM[tempus];

    redde chorda_ex_literis(titulus, piscina);
}

chorda
calendarium_nomen_gradus(
    GradusCelebrationis gradus,
    b32                 abbreviatum,
    Piscina*            piscina)
{
    constans character* titulus;

    si (gradus < ZEPHYRUM || gradus > GRADUS_DIES_PECULIARIS) {
        redde chorda_ex_literis("", piscina);
    }

    titulus = abbreviatum ?
        NOMINA_GRADUUM_BREVIA[gradus] :
        NOMINA_GRADUUM[gradus];

    redde chorda_ex_literis(titulus, piscina);
}

chorda
calendarium_nomen_coloris(ColorLiturgicus color, Piscina* piscina)
{
    si (color < ZEPHYRUM || color > COLOR_NIGER) {
        redde chorda_ex_literis("", piscina);
    }

    redde chorda_ex_literis(NOMINA_COLORUM[color], piscina);
}

interior constans character* NUMERI_ROMANI[] = {
    "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX",
    "X", "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX",
    "XX", "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX",
    "XXX", "XXXI", "XXXII", "XXXIII", "XXXIV"
};

chorda
calendarium_formare_titulum(
    CalendariumLiturgicum* cal,
    Dies                   dies,
    Piscina*               piscina)
{
    InformatioTemporis info = calendarium_tempus_info(cal, dies);
    character buffer[C];
    constans character* tempus_nomen;
    constans character* numerus;
    s32 longitudo;
    chorda result;

    tempus_nomen = NOMINA_TEMPORUM[info.tempus];

    si (info.hebdomada > ZEPHYRUM && info.hebdomada <= XXXIV) {
        numerus = NUMERI_ROMANI[info.hebdomada];
    } alioquin {
        numerus = "";
    }

    si (info.est_dominica) {
        si (info.tempus == TEMPUS_PER_ANNUM_I ||
            info.tempus == TEMPUS_PER_ANNUM_II) {
            longitudo = snprintf(buffer, C, "Dominica %s per Annum", numerus);
        } alioquin si (info.tempus == TEMPUS_ADVENTUS) {
            longitudo = snprintf(buffer, C, "Dominica %s Adventus", numerus);
        } alioquin si (info.tempus == TEMPUS_QUADRAGESIMAE) {
            longitudo = snprintf(buffer, C, "Dominica %s Quadragesimae", numerus);
        } alioquin si (info.tempus == TEMPUS_PASCHALE) {
            longitudo = snprintf(buffer, C, "Dominica %s Paschae", numerus);
        } alioquin {
            longitudo = snprintf(buffer, C, "Dominica - %s", tempus_nomen);
        }
    } alioquin {
        /* Feria */
        constans character* dies_nomina[] = {
            "Dominica", "Feria II", "Feria III", "Feria IV",
            "Feria V", "Feria VI", "Sabbatum"
        };
        longitudo = snprintf(buffer, C, "%s, Hebdomada %s",
            dies_nomina[info.dies_in_hebdomada], numerus);
    }

    result.datum = (i8*)piscina_allocare(piscina, (memoriae_index)(longitudo + I));
    memcpy(result.datum, buffer, (memoriae_index)(longitudo + I));
    result.mensura = (i32)longitudo;

    redde result;
}


/* ==================================================
 * Praecedentia Liturgica
 *
 * Secundum Tabulam Dierum Liturgicorum (GNLYC)
 *
 * Ordo praecedentiae:
 * 1. Triduum Paschale
 * 2. Nativitas, Epiphania, Ascensio, Pentecoste
 *    Dominicae Adventus, Quadragesimae, Paschae
 *    Feria IV Cinerum, Hebdomada Sancta, Octava Paschae
 * 3. Sollemnitates Domini, BVM, Sanctorum
 * 4. Commemoratio Omnium Fidelium Defunctorum
 * 5. Dominicae Nativitatis et per Annum
 * 6. Festa Domini et Sanctorum
 * 7. Feriae Adventus (17-24 Dec), Quadragesimae
 * 8. Memoriae obligatoriae
 * 9. Memoriae ad libitum, Feriae per Annum
 * ================================================== */

/*
 * Gradus praecedentiae superior = magis privilegiatus
 */
nomen enumeratio {
    PRAECED_FERIA              = ZEPHYRUM,
    PRAECED_MEMORIA_AD_LIBITUM = I,
    PRAECED_MEMORIA            = II,
    PRAECED_FERIA_PRIVILEGIATA = III,  /* Adventus 17-24, Quadragesima */
    PRAECED_FESTUM             = IV,
    PRAECED_DOMINICA           = V,
    PRAECED_DEFUNCTORUM        = VI,   /* 2 Nov */
    PRAECED_SOLLEMNITAS        = VII,
    PRAECED_DOMINICA_MAIOR     = VIII, /* Adventus, Quadragesima, Pascha */
    PRAECED_DIES_PECULIARIS    = IX,   /* Feria IV Cinerum, Hebd. Sancta, Oct. Pasch. */
    PRAECED_TRIDUUM            = X
} GradusPraecedentiae;


/*
 * obtinere_praeced_temporale
 *
 * Determinare gradum praecedentiae diei ex temporale.
 */
interior GradusPraecedentiae
obtinere_praeced_temporale(CalendariumLiturgicum* cal, Dies dies)
{
    TempusLiturgicum tempus = calendarium_tempus(cal, dies);
    s32 dies_hebd = fasti_dies_hebdomadis(dies);
    b32 est_dominica = (dies_hebd == ZEPHYRUM);

    /* Triduum Paschale - maxima praecedentia */
    si (tempus == TEMPUS_TRIDUUM_SACRUM) {
        redde PRAECED_TRIDUUM;
    }

    /* Octava Paschae */
    si (calendarium_est_octava_paschae(cal, dies)) {
        redde PRAECED_DIES_PECULIARIS;
    }

    /* Feria IV Cinerum */
    si (calendarium_est_feria_cinerum(cal, dies)) {
        redde PRAECED_DIES_PECULIARIS;
    }

    /* Hebdomada Sancta (Feria II - Feria IV) */
    si (calendarium_est_hebdomada_sancta(cal, dies) && !est_dominica) {
        redde PRAECED_DIES_PECULIARIS;
    }

    /* Dominicae maiores (Adventus, Quadragesimae, Paschae) */
    si (est_dominica) {
        si (tempus == TEMPUS_ADVENTUS ||
            tempus == TEMPUS_QUADRAGESIMAE ||
            tempus == TEMPUS_PASCHALE) {
            redde PRAECED_DOMINICA_MAIOR;
        }
        redde PRAECED_DOMINICA;
    }

    /* Feriae privilegiatae Adventus (17-24 Dec) */
    si (tempus == TEMPUS_ADVENTUS &&
        dies.mensis == FASTI_DECEMBER &&
        dies.dies >= XVII && dies.dies <= XXIV) {
        redde PRAECED_FERIA_PRIVILEGIATA;
    }

    /* Feriae Quadragesimae */
    si (tempus == TEMPUS_QUADRAGESIMAE) {
        redde PRAECED_FERIA_PRIVILEGIATA;
    }

    /* 2 Novembris - Commemoratio Defunctorum */
    si (dies.mensis == FASTI_NOVEMBER && dies.dies == II) {
        redde PRAECED_DEFUNCTORUM;
    }

    /* Feriae Adventus (ante 17 Dec) */
    si (tempus == TEMPUS_ADVENTUS) {
        redde PRAECED_FERIA_PRIVILEGIATA;
    }

    /* Feriae ordinariae */
    redde PRAECED_FERIA;
}


/*
 * obtinere_praeced_sanctorale
 *
 * Convertere GradusCelebrationis ad GradusPraecedentiae.
 */
interior GradusPraecedentiae
obtinere_praeced_sanctorale(GradusCelebrationis gradus)
{
    commutatio (gradus) {
        casus GRADUS_SOLLEMNITAS:
            redde PRAECED_SOLLEMNITAS;
        casus GRADUS_FESTUM:
            redde PRAECED_FESTUM;
        casus GRADUS_MEMORIA:
            redde PRAECED_MEMORIA;
        casus GRADUS_MEMORIA_AD_LIBITUM:
            redde PRAECED_MEMORIA_AD_LIBITUM;
        casus GRADUS_DIES_PECULIARIS:
            redde PRAECED_DEFUNCTORUM;  /* e.g. 2 Nov */
        ordinarius:
            redde PRAECED_FERIA;
    }
}


/*
 * comparare_praecedentiam
 *
 * Comparare praecedentiam temporalis et sanctoralis.
 * Redit: >0 si sanctorale vincit, <0 si temporale vincit, 0 si aequales.
 */
interior s32
comparare_praecedentiam(
    GradusPraecedentiae praeced_temp,
    GradusPraecedentiae praeced_sanct)
{
    /* Si temporale est Triduum vel Dies Peculiaris, temporale vincit */
    si (praeced_temp >= PRAECED_DIES_PECULIARIS) {
        redde -I;
    }

    /* Si sanctorale est sollemnitas */
    si (praeced_sanct == PRAECED_SOLLEMNITAS) {
        /* Sollemnitas vincit omnes praeter Dominicas maiores et supra */
        si (praeced_temp >= PRAECED_DOMINICA_MAIOR) {
            redde -I;  /* Dominica maior vincit */
        }
        redde I;  /* Sollemnitas vincit */
    }

    /* Dominicae (omnes) vincunt festa et memorias */
    si (praeced_temp >= PRAECED_DOMINICA) {
        redde -I;
    }

    /* Commemoratio Defunctorum (2 Nov) - specialis */
    si (praeced_temp == PRAECED_DEFUNCTORUM ||
        praeced_sanct == PRAECED_DEFUNCTORUM) {
        redde ZEPHYRUM;  /* Celebratur */
    }

    /* Festa */
    si (praeced_sanct == PRAECED_FESTUM) {
        si (praeced_temp <= PRAECED_FERIA_PRIVILEGIATA) {
            redde I;  /* Festum vincit feriam */
        }
        redde -I;
    }

    /* Memoriae in Quadragesima fiunt ad libitum */
    si (praeced_temp == PRAECED_FERIA_PRIVILEGIATA) {
        si (praeced_sanct == PRAECED_MEMORIA) {
            /* Memoria obligatoria fit ad libitum in Quadragesima */
            redde ZEPHYRUM;
        }
        si (praeced_sanct == PRAECED_MEMORIA_AD_LIBITUM) {
            redde ZEPHYRUM;  /* Etiam permissa */
        }
        redde -I;
    }

    /* Memoriae in feriis ordinariae */
    si (praeced_sanct >= PRAECED_MEMORIA_AD_LIBITUM) {
        redde I;
    }

    redde ZEPHYRUM;
}


/*
 * creare_celebrationem_temporale
 *
 * Creare Celebratio pro die temporali.
 */
interior Celebratio*
creare_celebrationem_temporale(
    CalendariumLiturgicum* cal,
    Dies                   dies,
    Piscina*               piscina)
{
    Celebratio* celeb;
    InformatioTemporis info;
    chorda titulus;

    celeb = (Celebratio*)piscina_allocare(piscina, magnitudo(Celebratio));
    si (celeb == NIHIL) {
        redde NIHIL;
    }

    info = calendarium_tempus_info(cal, dies);
    titulus = calendarium_formare_titulum(cal, dies, piscina);

    celeb->titulus = titulus;
    celeb->titulus_brevis = titulus;  /* Idem pro nunc */
    celeb->color = calendarium_color_temporis(cal, dies);
    celeb->genus = GENUS_TEMPORALE;
    celeb->lectionarium = -I;

    /* Determinare gradus */
    si (info.est_dominica) {
        celeb->gradus = GRADUS_DOMINICA;
    } alioquin si (calendarium_tempus(cal, dies) == TEMPUS_TRIDUUM_SACRUM ||
                  calendarium_est_feria_cinerum(cal, dies) ||
                  calendarium_est_hebdomada_sancta(cal, dies) ||
                  calendarium_est_octava_paschae(cal, dies)) {
        celeb->gradus = GRADUS_DIES_PECULIARIS;
    } alioquin {
        celeb->gradus = GRADUS_FERIA;
    }

    redde celeb;
}


/*
 * creare_celebrationem_sanctorale
 *
 * Creare Celebratio ex SanctoraleDatum.
 */
interior Celebratio*
creare_celebrationem_sanctorale(
    constans SanctoraleDatum* datum,
    Piscina*                  piscina)
{
    Celebratio* celeb;

    celeb = (Celebratio*)piscina_allocare(piscina, magnitudo(Celebratio));
    si (celeb == NIHIL) {
        redde NIHIL;
    }

    celeb->titulus = chorda_ex_literis(datum->titulus, piscina);
    celeb->titulus_brevis = chorda_ex_literis(datum->titulus_brevis, piscina);
    celeb->gradus = datum->gradus;
    celeb->color = datum->color;
    celeb->genus = GENUS_SANCTORALE;
    celeb->lectionarium = -I;

    redde celeb;
}


/* ==================================================
 * Quaestio Principalis
 * ================================================== */

InformatioDiei*
calendarium_obtinere_diem(
    CalendariumLiturgicum* cal,
    Dies                   dies,
    Piscina*               piscina)
{
    InformatioDiei* info;
    GradusPraecedentiae praeced_temp;
    constans SanctoraleDatum* sancta;
    s32 num_sanctorum;
    s32 i;
    Celebratio* celeb_temp;
    Celebratio* celebrationes_array;
    s32 num_celebrationum;
    s32 idx;

    info = (InformatioDiei*)piscina_allocare(piscina, magnitudo(InformatioDiei));

    si (info == NIHIL) {
        redde NIHIL;
    }

    info->dies = dies;
    info->tempus_info = calendarium_tempus_info(cal, dies);
    info->titulus_diei = calendarium_formare_titulum(cal, dies, piscina);

    /* Cyclus lectionum */
    info->cyclus.cyclus_dominicalis = calendarium_cyclus_dominicalis(dies.annus);
    info->cyclus.cyclus_quotidianus = calendarium_cyclus_quotidianus(dies.annus);
    info->cyclus.hebdomada_psalterii = calendarium_hebdomada_psalterii(cal, dies);

    /* Obtinere praecedentiam temporalem */
    praeced_temp = obtinere_praeced_temporale(cal, dies);

    /* Creare celebrationem temporalem */
    celeb_temp = creare_celebrationem_temporale(cal, dies, piscina);

    /* Obtinere celebrationes sanctorales */
    sancta = sanctorale_obtinere(dies.mensis, dies.dies, &num_sanctorum);

    /* Allocare array pro celebrationibus */
    num_celebrationum = I + num_sanctorum;  /* Temporale + sanctorale */
    celebrationes_array = (Celebratio*)piscina_allocare(
        piscina,
        magnitudo(Celebratio) * (memoriae_index)num_celebrationum
    );

    si (celebrationes_array == NIHIL) {
        redde NIHIL;
    }

    idx = ZEPHYRUM;

    /* Addere celebrationem temporalem */
    si (celeb_temp != NIHIL) {
        celebrationes_array[idx] = *celeb_temp;
        idx++;
    }

    /* Addere celebrationes sanctorales validas */
    per (i = ZEPHYRUM; i < num_sanctorum; i++) {
        GradusPraecedentiae praeced_sanct;
        s32 comp;
        Celebratio* celeb_sanct;

        praeced_sanct = obtinere_praeced_sanctorale(sancta[i].gradus);
        comp = comparare_praecedentiam(praeced_temp, praeced_sanct);

        /* Si sanctorale vincit vel est permissum */
        si (comp >= ZEPHYRUM) {
            celeb_sanct = creare_celebrationem_sanctorale(&sancta[i], piscina);
            si (celeb_sanct != NIHIL) {
                celebrationes_array[idx] = *celeb_sanct;
                idx++;
            }
        }
    }

    info->celebrationes = celebrationes_array;
    info->numerus_celebrationum = idx;

    /* Determinare celebrationem principalem et colorem */
    si (idx > ZEPHYRUM) {
        Celebratio* principalis = &celebrationes_array[ZEPHYRUM];
        s32 j;

        /* Invenire celebrationem cum maximo gradu */
        per (j = I; j < idx; j++) {
            si (celebrationes_array[j].gradus > principalis->gradus) {
                principalis = &celebrationes_array[j];
            }
        }

        info->celebratio_principalis = principalis;
        info->color_diei = principalis->color;

        /* Si principalis est sanctorale, titulus diei est titulus sancti */
        si (principalis->genus == GENUS_SANCTORALE &&
            principalis->gradus >= GRADUS_FESTUM) {
            info->titulus_diei = principalis->titulus;
        }
    } alioquin {
        info->celebratio_principalis = NIHIL;
        info->color_diei = calendarium_color_temporis(cal, dies);
    }

    redde info;
}
