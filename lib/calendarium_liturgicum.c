/* ==================================================
 * CALENDARIUM LITURGICUM
 * Bibliotheca Calendarii Liturgici Romani (Forma Ordinaria)
 *
 * Secundum Calendarium Romanum Generale (1969)
 * ================================================== */

#include "postulata_posix.h"
#include "calendarium_liturgicum.h"
#include "chorda_aedificator.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>


/* ==================================================
 * Structura Interna
 * ================================================== */

/* translatio sollemnitatis impeditae (L3) */
nomen structura {
    constans SanctoraleDatum* datum;
                        Dies  originis;
                        Dies  nova;
} Translatio;

#define TRANSLATIONES_MAXIMAE VIII

/* annus civilis computatus: anchorae et translationes (pigrae -
 * computatio dies anni legit, ergo anchoras in cache iam requirit) */
nomen structura {
    AnchoresMobiles anchorae;
         Translatio translationes[TRANSLATIONES_MAXIMAE];
                s32 numerus_translationum;
                b32 translationes_computatae;
} AnnusComputatus;

structura CalendariumLiturgicum {
            Piscina* piscina;
    AnnusComputatus* anni[X];            /* Cache annorum recentium */
                s32  cache_indices[X];   /* Anni in cache */
                s32  cache_numerus;
};


/* ==================================================
 * Creatio et Destructio
 * ================================================== */

CalendariumLiturgicum*
calendarium_creare (
    Piscina* piscina)
{
    CalendariumLiturgicum* cal;
                      s32  i;

    cal = (CalendariumLiturgicum*)piscina_allocare(
        piscina,
        magnitudo(CalendariumLiturgicum)
        );

    si (cal == NIHIL)
    {
        redde NIHIL;
    }

    cal->piscina        = piscina;
    cal->cache_numerus  = ZEPHYRUM;

    per (i = ZEPHYRUM; i < X; i++)
    {
        cal->anni[i]           = NIHIL;
        cal->cache_indices[i]  = ZEPHYRUM;
    }

    redde cal;
}

vacuum
calendarium_destruere (
    CalendariumLiturgicum* cal)
{
    /* Omnia in piscina allocata sunt - nihil agendum */
    (vacuum)cal;
}


/* ==================================================
 * Anchorae Mobiles - Computatio
 * ================================================== */

interior vacuum
computare_anchorae (
    AnchoresMobiles* anch,
                s32  annus)
{
    Dies pascha;
    Dies secunda;
    Dies nativitas;
     s32 dies_hebdomadae;

    anch->annus = annus;

    /* Pascha - fundamentum omnium */
    pascha        = fasti_computus(annus);
    anch->pascha  = pascha;

    /* Dies ex Paschate derivatae */
    anch->feria_iv_cinerum          = fasti_addere_dies(pascha, -XLVI);
    anch->dominica_i_quadragesimae  = fasti_addere_dies(pascha, -XLII);
    anch->dominica_palmarum         = fasti_addere_dies(pascha, -VII);
    anch->feria_v_in_cena_domini    = fasti_addere_dies(pascha, -III);
    anch->feria_vi_in_passione      = fasti_addere_dies(pascha, -II);
    anch->sabbatum_sanctum          = fasti_addere_dies(pascha, -I);
    anch->ascensio                  = fasti_addere_dies(pascha, XLII);
    anch->pentecoste                = fasti_addere_dies(pascha, XLIX);
    anch->maria_mater_ecclesiae     = fasti_addere_dies(pascha, L);
    anch->ss_trinitas               = fasti_addere_dies(pascha, LVI);
    anch->corpus_christi            = fasti_addere_dies(pascha, LXIII);
    anch->cor_iesu                  = fasti_addere_dies(pascha, LXVIII);
    anch->cor_immaculatum           = fasti_addere_dies(pascha, LXIX);

    /* Dominica I Adventus - 4 dominicae ante Nativitatem */
    /* Invenire dominicam in vel ante 24 Dec (Vigilia), haec est Dominica IV Adventus */
    /* Tunc subtrahere 21 dies pro Dominica I Adventus */
    {
        Dies vigilia = fasti_dies(annus, FASTI_DECEMBER, XXIV);
         s32 dies_ad_dominicam = fasti_dies_hebdomadis(vigilia);
        Dies dominica_iv = fasti_addere_dies(vigilia,
            -dies_ad_dominicam);
        anch->dominica_i_adventus = fasti_addere_dies(dominica_iv,
            -XXI);
    }

    /* Christus Rex - dominica ante Adventum I */
    anch->christus_rex = fasti_addere_dies(anch->dominica_i_adventus,
        -VII);

    /* Epiphania (USA): dominica inter 2 et 8 Ian. Baptisma Domini:
     * dominica post Epiphaniam, nisi Epiphania 7 aut 8 Ian - tunc
     * feria II sequens (F8: olim Epiphania 6 Ian fixa et, ea
     * dominica, Baptisma 7 Ian - etiam calendario generali falsum) */
    secunda          = fasti_dies(annus, FASTI_IANUARIUS, II);
    dies_hebdomadae  = fasti_dies_hebdomadis(secunda);
    anch->epiphania  = fasti_addere_dies(secunda, (VII
        - dies_hebdomadae)
        % VII);
    anch->baptisma_domini = fasti_addere_dies(anch->epiphania,
        anch->epiphania.dies >= VII ? I : VII);

    /* Sancta Familia: dominica intra octavam Nativitatis (26-31 Dec);
     * si Nativitas dominica est, nulla - 30 Dec */
    nativitas        = fasti_dies(annus, FASTI_DECEMBER, XXV);
    dies_hebdomadae  = fasti_dies_hebdomadis(nativitas);
    anch->sancta_familia = dies_hebdomadae == ZEPHYRUM
        ? fasti_dies(annus, FASTI_DECEMBER, XXX)
        : fasti_addere_dies(nativitas, VII - dies_hebdomadae);
}

interior AnnusComputatus*
annum_obtinere (
    CalendariumLiturgicum* cal,
                      s32  annus)
{
                s32  i;
                s32  index_minimus;
    AnnusComputatus* a;

    per (i = ZEPHYRUM; i < cal->cache_numerus; i++)
    {
        si (cal->cache_indices[i] == annus)
        {
            redde cal->anni[i];
        }
    }

    a = (AnnusComputatus*)piscina_allocare(cal->piscina,
        magnitudo(AnnusComputatus));
    si (a == NIHIL)
    {
        redde NIHIL;
    }
    computare_anchorae(&a->anchorae, annus);
    a->numerus_translationum     = ZEPHYRUM;
    a->translationes_computatae  = FALSUM;

    /* Addere ad cache (plenus: substituere minimum - annus eiectus in
     * piscina manet, indices priores validi) */
    si (cal->cache_numerus < X)
    {
        cal->anni[cal->cache_numerus]           = a;
        cal->cache_indices[cal->cache_numerus]  = annus;
        cal->cache_numerus++;
    } alioquin
    {
        index_minimus = ZEPHYRUM;
        per (i = I; i < X; i++)
        {
            si (cal->cache_indices[i]
                < cal->cache_indices[index_minimus])
            {
                index_minimus = i;
            }
        }
        cal->anni[index_minimus]           = a;
        cal->cache_indices[index_minimus]  = annus;
    }

    redde a;
}

AnchoresMobiles*
calendarium_anchorae (
    CalendariumLiturgicum* cal,
                      s32  annus)
{
    AnnusComputatus* a;

    a = annum_obtinere(cal, annus);
    redde a ? &a->anchorae : NIHIL;
}


/* ==================================================
 * Limites Temporum
 * ================================================== */

Dies
calendarium_initium_temporis (
    CalendariumLiturgicum* cal,
         TempusLiturgicum  tempus,
                      s32  annus)
{
    AnchoresMobiles* anch;

    commutatio (tempus)
    {
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
calendarium_finis_temporis (
    CalendariumLiturgicum* cal,
         TempusLiturgicum  tempus,
                      s32  annus)
{
    AnchoresMobiles* anch;

    commutatio (tempus)
    {
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
            /* annus liturgicus N finit pridie Adventus I anni civilis N
             * (F9: olim annus + I) */
            anch = calendarium_anchorae(cal, annus);
            redde fasti_addere_dies(anch->dominica_i_adventus, -I);

        ordinarius:
            redde fasti_dies(annus, FASTI_DECEMBER, XXXI);
    }
}


/* ==================================================
 * Quaestiones Temporis
 * ================================================== */

TempusLiturgicum
calendarium_tempus (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
                s32  annus = dies.annus;
    AnchoresMobiles* anch;

    /* Obtinere anchorae pro anno currente */
    anch = calendarium_anchorae(cal, annus);


    /* ================================================
     * NATIVITAS (Dec 25 - Baptisma Domini)
     * Prioritas maxima pro hac periodo
     * ================================================ */

    /* Dec 25-31: Tempus Nativitatis */
    si (dies.mensis == FASTI_DECEMBER && dies.dies >= XXV)
    {
        redde TEMPUS_NATIVITATIS;
    }

    /* Ian 1 - Baptisma Domini: Tempus Nativitatis */
    si (   dies.mensis == FASTI_IANUARIUS
        && fasti_comparare(dies, anch->baptisma_domini) <= ZEPHYRUM)
    {
        redde TEMPUS_NATIVITATIS;
    }


    /* ================================================
     * ADVENTUS (Dom I Adv - Dec 24)
     * ================================================ */

    /* Si in Adventu (post Dom I Adv et ante Dec 25) */
    si (   fasti_comparare(dies, anch->dominica_i_adventus) >= ZEPHYRUM
        && dies.mensis != FASTI_IANUARIUS)
    {
        redde TEMPUS_ADVENTUS;
    }


    /* ================================================
     * CYCLUS PASCHALIS
     * ================================================ */

    /* Si post Pentecosten */
    si (fasti_comparare(dies, anch->pentecoste) > ZEPHYRUM)
    {
        redde TEMPUS_PER_ANNUM_II;
    }

    /* Si Pentecoste */
    si (fasti_comparare(dies, anch->pentecoste) == ZEPHYRUM)
    {
        redde TEMPUS_PASCHALE;
    }

    /* Si post Pascha */
    si (fasti_comparare(dies, anch->pascha) >= ZEPHYRUM)
    {
        redde TEMPUS_PASCHALE;
    }

    /* Si in Triduo */
    si (fasti_comparare(dies, anch->feria_v_in_cena_domini) >= ZEPHYRUM)
    {
        redde TEMPUS_TRIDUUM_SACRUM;
    }

    /* Si post Feriam IV Cinerum */
    si (fasti_comparare(dies, anch->feria_iv_cinerum) >= ZEPHYRUM)
    {
        redde TEMPUS_QUADRAGESIMAE;
    }


    /* ================================================
     * TEMPUS PER ANNUM I (post Baptisma - ante Quadragesimam)
     * ================================================ */

    si (fasti_comparare(dies, anch->baptisma_domini) > ZEPHYRUM)
    {
        redde TEMPUS_PER_ANNUM_I;
    }


    /* ================================================
     * TEMPUS PER ANNUM II (Defectus)
     * ================================================ */

    redde TEMPUS_PER_ANNUM_II;
}

InformatioTemporis
calendarium_tempus_info (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    InformatioTemporis  info;
      TempusLiturgicum  tempus;
       AnchoresMobiles* anch;
                  Dies  initium;
                   s32  dies_ab_initio;

    tempus                  = calendarium_tempus(cal, dies);
    info.tempus             = tempus;
    info.dies_in_hebdomada  = fasti_dies_hebdomadis(dies);
    info.est_dominica = (info.dies_in_hebdomada
        == ZEPHYRUM) ? VERUM : FALSUM;

    anch = calendarium_anchorae(cal, dies.annus);

    commutatio (tempus)
    {
        casus TEMPUS_ADVENTUS:
            initium = anch->dominica_i_adventus;
            si (fasti_comparare(dies, initium) < ZEPHYRUM)
            {
                /* Adventus anni sequentis */
                initium = calendarium_anchorae(cal, dies.annus
                    + I)->dominica_i_adventus;
            }
            dies_ab_initio = (s32)fasti_differentia(dies,
                initium).dies_totales;
            info.hebdomada = (dies_ab_initio / VII) + I;
            frange;

        casus TEMPUS_NATIVITATIS:
            /* Octava Nativitatis */
            info.hebdomada = I;
            frange;

        casus TEMPUS_PER_ANNUM_I:
            /* a dominica in aut ante Baptisma: Baptisma feria II (USA,
             * Epiphania 7-8 I) hebdomadam I non mutat */
            initium = fasti_addere_dies(anch->baptisma_domini,
                -fasti_dies_hebdomadis(anch->baptisma_domini));
            dies_ab_initio = (s32)fasti_differentia(dies,
                initium).dies_totales;
            info.hebdomada = (dies_ab_initio / VII) + I;
            frange;

        casus TEMPUS_QUADRAGESIMAE:
            initium = anch->feria_iv_cinerum;
            dies_ab_initio = (s32)fasti_differentia(dies,
                initium).dies_totales;
            /* Feria IV Cinerum est ante Dominicam I */
            info.hebdomada = (dies_ab_initio + IV) / VII;
            si (info.hebdomada == ZEPHYRUM)
            {
                info.hebdomada = I;  /* Post Cineres, ante Dom. I */
            }
            frange;

        casus TEMPUS_TRIDUUM_SACRUM:
            info.hebdomada = I;
            frange;

        casus TEMPUS_PASCHALE:
            initium = anch->pascha;
            dies_ab_initio = (s32)fasti_differentia(dies,
                initium).dies_totales;
            info.hebdomada = (dies_ab_initio / VII) + I;
            frange;

        casus TEMPUS_PER_ANNUM_II:
            /* Numeratio RETRORSUM: hebdomada quae a Christo Rege
             * (dominica ante Adventum I) incipit est XXXIV, ergo
             * hebdomada = XXXV - (Adventus I - dominica diei) / VII.
             * Adventus EIUSDEM anni civilis: tempus per annum II
             * semper ante eum cadit. Dominica in vel ante diem
             * quaeritur, ut feria hebdomadam dominicae suae ferat.
             *
             * Nulla cohibitio ad I. Olim Adventus anni SEQUENTIS
             * legebatur, numerus negativus fiebat, et cohibitio eum
             * in "Hebdomada I" plausibilem vertebat - omnis dies post
             * Pentecosten (mensuratum 2026-09-12). */
            initium = fasti_addere_dies(dies, -info.dies_in_hebdomada);
            dies_ab_initio = (s32)fasti_differentia(
                anch->dominica_i_adventus, initium).dies_totales;
            info.hebdomada = XXXV - (dies_ab_initio / VII);
            frange;

        ordinarius:
            info.hebdomada = I;
    }

    redde info;
}

ColorLiturgicus
calendarium_color_temporis (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    TempusLiturgicum  tempus = calendarium_tempus(cal, dies);
     AnchoresMobiles* anch;

    commutatio (tempus)
    {
        casus TEMPUS_ADVENTUS:
            /* Rosaceus Dominica III (Gaudete) */
            anch = calendarium_anchorae(cal, dies.annus);
            si (fasti_comparare(dies, anch->dominica_i_adventus)
                < ZEPHYRUM)
            {
                anch = calendarium_anchorae(cal, dies.annus + I);
            }
            si (   fasti_differentia(dies,
                anch->dominica_i_adventus).dies_totales >= XIV
                && fasti_differentia(dies,
                anch->dominica_i_adventus).dies_totales < XXI
                && fasti_dies_hebdomadis(dies) == ZEPHYRUM)
            {
                redde COLLIT_ROSACEUS;
            }
            redde COLLIT_VIOLACEUS;

        casus TEMPUS_NATIVITATIS:
            redde COLLIT_ALBUS;

        casus TEMPUS_PER_ANNUM_I:
        casus TEMPUS_PER_ANNUM_II:
            redde COLLIT_VIRIDIS;

        casus TEMPUS_QUADRAGESIMAE:
            /* Rosaceus Dominica IV (Laetare) */
            anch = calendarium_anchorae(cal, dies.annus);
            si (   fasti_differentia(dies,
                anch->dominica_i_quadragesimae).dies_totales >= XXI
                && fasti_differentia(dies,
                anch->dominica_i_quadragesimae).dies_totales < XXVIII
                && fasti_dies_hebdomadis(dies) == ZEPHYRUM)
            {
                redde COLLIT_ROSACEUS;
            }
            redde COLLIT_VIOLACEUS;

        casus TEMPUS_TRIDUUM_SACRUM:
            redde COLLIT_RUBER;  /* Feria VI in Passione */

        casus TEMPUS_PASCHALE:
            /* Ruber in Pentecoste */
            anch = calendarium_anchorae(cal, dies.annus);
            si (fasti_comparare(dies, anch->pentecoste) == ZEPHYRUM)
            {
                redde COLLIT_RUBER;
            }
            redde COLLIT_ALBUS;

        ordinarius:
            redde COLLIT_VIRIDIS;
    }
}


/* ==================================================
 * Quaestiones Cycli Lectionum
 * ================================================== */

CyclusDominicalis
calendarium_cyclus_dominicalis (
    s32 annus)
{
    /* Cyclus A = annus % 3 == 1 (2023, 2026, ...) */
    /* Cyclus B = annus % 3 == 2 (2024, 2027, ...) */
    /* Cyclus C = annus % 3 == 0 (2022, 2025, ...) */
    s32 residuum = annus % III;

    commutatio (residuum)
    {
        casus I:
            redde CYCLUS_A;
        casus II:
            redde CYCLUS_B;
        ordinarius:
            redde CYCLUS_C;
    }
}

CyclusQuotidianus
calendarium_cyclus_quotidianus (
    s32 annus)
{
    /* Cyclus I = annus impar */
    /* Cyclus II = annus par */
    redde (annus % II == I) ? CYCLUS_I : CYCLUS_II;
}

s32
calendarium_hebdomada_psalterii (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    InformatioTemporis info = calendarium_tempus_info(cal, dies);

    /* Hebdomada psalterii 1-4 rotat */
    redde ((info.hebdomada - I) % IV) + I;
}


/* ==================================================
 * Dies Peculiares
 * ================================================== */

b32
calendarium_est_feria_cinerum (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    AnchoresMobiles* anch = calendarium_anchorae(cal, dies.annus);
    redde fasti_comparare(dies, anch->feria_iv_cinerum) == ZEPHYRUM;
}

b32
calendarium_est_hebdomada_sancta (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    AnchoresMobiles* anch = calendarium_anchorae(cal, dies.annus);
               Dies  dominica_palmarum = anch->dominica_palmarum;

    redde fasti_comparare(dies, dominica_palmarum) >= ZEPHYRUM
        && fasti_comparare(dies, anch->feria_v_in_cena_domini)
            < ZEPHYRUM;
}

b32
calendarium_est_triduum (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    redde calendarium_tempus(cal, dies) == TEMPUS_TRIDUUM_SACRUM;
}

b32
calendarium_est_octava_paschae (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    AnchoresMobiles* anch = calendarium_anchorae(cal, dies.annus);
                s64  diff = fasti_differentia(dies,
                    anch->pascha).dies_totales;

    redde diff >= ZEPHYRUM && diff < VIII;
}

b32
calendarium_est_octava_nativitatis (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    Dies nativitas;
     s64 diff;

    (vacuum)cal;

    si (dies.mensis == FASTI_DECEMBER && dies.dies >= XXV)
    {
        nativitas  = fasti_dies(dies.annus, FASTI_DECEMBER, XXV);
        diff       = fasti_differentia(dies, nativitas).dies_totales;
        redde diff >= ZEPHYRUM && diff < VIII;
    }

    si (dies.mensis == FASTI_IANUARIUS && dies.dies == I)
    {
        redde VERUM;
    }

    redde FALSUM;
}


/* ==================================================
 * Invenire
 * ================================================== */

Dies
calendarium_proxima_dominica (
    Dies dies)
{
    s32 dies_hebdomadae = fasti_dies_hebdomadis(dies);

    si (dies_hebdomadae == ZEPHYRUM)
    {
        redde dies;
    }

    redde fasti_addere_dies(dies, VII - dies_hebdomadae);
}

Dies
calendarium_invenire_festum (
    CalendariumLiturgicum* cal,
                   chorda  titulus,
                      s32  annus)
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
    "Dies peculiaris",
    "Commemoratio defunctorum"
};

interior constans character* NOMINA_GRADUUM_BREVIA[] = {
    "fer",
    "mem opt",
    "mem",
    "fest",
    "sol",
    "Dom",
    "dies",
    "def"
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
calendarium_nomen_temporis (
    TempusLiturgicum  tempus,
                 b32  abbreviatum,
             Piscina* piscina)
{
    constans character* titulus;

    si (tempus < ZEPHYRUM || tempus > TEMPUS_PER_ANNUM_II)
    {
        redde chorda_ex_literis("", piscina);
    }

    titulus = abbreviatum ?
        NOMINA_TEMPORUM_BREVIA[tempus] :
        NOMINA_TEMPORUM[tempus];

    redde chorda_ex_literis(titulus, piscina);
}

chorda
calendarium_nomen_gradus (
    GradusCelebrationis  gradus,
                    b32  abbreviatum,
                Piscina* piscina)
{
    constans character* titulus;

    si (gradus < ZEPHYRUM || gradus > GRADUS_DEFUNCTORUM)
    {
        redde chorda_ex_literis("", piscina);
    }

    titulus = abbreviatum ?
        NOMINA_GRADUUM_BREVIA[gradus] :
        NOMINA_GRADUUM[gradus];

    redde chorda_ex_literis(titulus, piscina);
}

chorda
calendarium_nomen_coloris (
    ColorLiturgicus  color,
            Piscina* piscina)
{
    si (color < ZEPHYRUM || color > COLLIT_NIGER)
    {
        redde chorda_ex_literis("", piscina);
    }

    redde chorda_ex_literis(NOMINA_COLORUM[color], piscina);
}

interior constans character* NUMERI_ROMANI[] = {
    "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX",
    "X", "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII",
        "XIX",
    "XX", "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII",
        "XXVIII", "XXIX",
    "XXX", "XXXI", "XXXII", "XXXIII", "XXXIV"
};

chorda
calendarium_formare_titulum (
    CalendariumLiturgicum* cal,
                     Dies  dies,
                  Piscina* piscina)
{
    InformatioTemporis  info = calendarium_tempus_info(cal, dies);
             character  buffer[C];
    constans character* tempus_nomen;
    constans character* numerus;
                   s32  longitudo;
                chorda  result;

    tempus_nomen = NOMINA_TEMPORUM[info.tempus];

    si (info.hebdomada > ZEPHYRUM && info.hebdomada <= XXXIV)
    {
        numerus = NUMERI_ROMANI[info.hebdomada];
    } alioquin
    {
        numerus = "";
    }

    si (info.est_dominica)
    {
        si (   info.tempus == TEMPUS_PER_ANNUM_I
            || info.tempus == TEMPUS_PER_ANNUM_II)
        {
            longitudo = snprintf(buffer, C, "Dominica %s per Annum",
                numerus);
        } alioquin si (info.tempus == TEMPUS_ADVENTUS)
        {
            longitudo = snprintf(buffer, C, "Dominica %s Adventus",
                numerus);
        } alioquin si (info.tempus == TEMPUS_QUADRAGESIMAE)
        {
            longitudo = snprintf(buffer, C, "Dominica %s Quadragesimae",
                numerus);
        } alioquin si (info.tempus == TEMPUS_PASCHALE)
        {
            longitudo = snprintf(buffer, C, "Dominica %s Paschae",
                numerus);
        } alioquin
        {
            longitudo = snprintf(buffer, C, "Dominica - %s",
                tempus_nomen);
        }
    } alioquin
    {
        /* Feria */
        constans character* dies_nomina[] = {
            "Dominica", "Feria II", "Feria III", "Feria IV",
            "Feria V", "Feria VI", "Sabbatum"
        };
        longitudo = snprintf(buffer, C, "%s, Hebdomada %s",
            dies_nomina[info.dies_in_hebdomada], numerus);
    }

    result.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(longitudo + I));
    memcpy(result.datum, buffer, (memoriae_index)(longitudo + I));
    result.mensura = (i32)longitudo;

    redde result;
}

chorda
calendarium_nomen_celebrationum (
       Dies  dies,
    Piscina* piscina)
{
           ChordaAedificator* aed;
          constans character* nomen_mobile;
    constans SanctoraleDatum* sancta;
                         s32  num_sanctorum;
                         s32  i;
                         b32  primum;
                      chorda  result;

    aed = chorda_aedificator_creare(piscina, CCLVI);
    si (aed == NIHIL)
    {
        result.datum    = NIHIL;
        result.mensura  = ZEPHYRUM;
        redde result;
    }

    primum = VERUM;

    /* Primo: festivitates mobiles (Pascha, Pentecoste, etc.) */
    nomen_mobile = fasti_nomen_festivitatis(dies);
    si (nomen_mobile != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, nomen_mobile);
        primum = FALSUM;
    }

    /* Secundo: sanctorale (fixa) */
    sancta = sanctorale_obtinere(dies.mensis, dies.dies,
        &num_sanctorum);
    si (sancta != NIHIL)
    {
        per (i = ZEPHYRUM; i < num_sanctorum; i++)
        {
            si (!primum)
            {
                chorda_aedificator_appendere_literis(aed, ", ");
            }
            chorda_aedificator_appendere_literis(aed,
                sancta[i].titulus);
            primum = FALSUM;
        }
    }

    redde chorda_aedificator_finire(aed);
}


/* ==================================================
 * Proprium de Tempore - celebrationes mobiles (liturgia-plan L2)
 *
 * Quaeque dies ex anchoris anni civilis (offsetof in
 * AnchoresMobiles). Non memoria: celebratio DIEI ipsa (temporale
 * substituit); memoria: inter memorias sanctorales. Tituli Anglice ut
 * sanctorale (Franus 10-15).
 * ================================================== */

nomen structura {
     memoriae_index locus;    /* offsetof(AnchoresMobiles, ...) */
    SanctoraleDatum datum;    /* mensis, dies: ZEPHYRUM (ex loco) */
} MobileDatum;

interior constans MobileDatum MOBILIA[] = {
    { offsetof(AnchoresMobiles, pascha),
      { ZEPHYRUM, ZEPHYRUM,
        "Easter Sunday of the Resurrection of the Lord",
            "Easter Sunday",
        GRADUS_SOLLEMNITAS, ORDO_TRIDUUM, COLLIT_ALBUS, FALSUM } },
    { offsetof(AnchoresMobiles, feria_vi_in_passione),
      { ZEPHYRUM, ZEPHYRUM,
        "Friday of the Passion of the Lord", "Good Friday",
        GRADUS_DIES_PECULIARIS, ORDO_TRIDUUM, COLLIT_RUBER, FALSUM } },
    { offsetof(AnchoresMobiles, sabbatum_sanctum),
      { ZEPHYRUM, ZEPHYRUM, "Holy Saturday", "Holy Saturday",
        GRADUS_DIES_PECULIARIS, ORDO_TRIDUUM, COLLIT_VIOLACEUS,
        FALSUM } },
    { offsetof(AnchoresMobiles, feria_v_in_cena_domini),
      { ZEPHYRUM, ZEPHYRUM, "Holy Thursday", "Holy Thursday",
        GRADUS_DIES_PECULIARIS, ORDO_PRIVILEGIATUS, COLLIT_ALBUS,
        FALSUM } },
    { offsetof(AnchoresMobiles, dominica_palmarum),
      { ZEPHYRUM, ZEPHYRUM,
        "Palm Sunday of the Passion of the Lord", "Palm Sunday",
        GRADUS_DOMINICA, ORDO_PRIVILEGIATUS, COLLIT_RUBER, FALSUM } },
    { offsetof(AnchoresMobiles, feria_iv_cinerum),
      { ZEPHYRUM, ZEPHYRUM, "Ash Wednesday", "Ash Wednesday",
        GRADUS_DIES_PECULIARIS, ORDO_PRIVILEGIATUS, COLLIT_VIOLACEUS,
        FALSUM } },
    { offsetof(AnchoresMobiles, ascensio),
      { ZEPHYRUM, ZEPHYRUM, "The Ascension of the Lord", "Ascension",
        GRADUS_SOLLEMNITAS, ORDO_PRIVILEGIATUS, COLLIT_ALBUS,
            FALSUM } },
    { offsetof(AnchoresMobiles, pentecoste),
      { ZEPHYRUM, ZEPHYRUM, "Pentecost Sunday", "Pentecost",
        GRADUS_SOLLEMNITAS, ORDO_PRIVILEGIATUS, COLLIT_RUBER,
            FALSUM } },
    { offsetof(AnchoresMobiles, epiphania),
      { ZEPHYRUM, ZEPHYRUM, "The Epiphany of the Lord", "Epiphany",
        GRADUS_SOLLEMNITAS, ORDO_PRIVILEGIATUS, COLLIT_ALBUS,
            FALSUM } },
    { offsetof(AnchoresMobiles, ss_trinitas),
      { ZEPHYRUM, ZEPHYRUM, "The Most Holy Trinity", "Holy Trinity",
        GRADUS_SOLLEMNITAS, ORDO_SOLLEMNITAS_GENERALIS, COLLIT_ALBUS,
        FALSUM } },
    { offsetof(AnchoresMobiles, corpus_christi),
      { ZEPHYRUM, ZEPHYRUM, "The Most Holy Body and Blood of Christ",
        "Corpus Christi", GRADUS_SOLLEMNITAS,
            ORDO_SOLLEMNITAS_GENERALIS,
        COLLIT_ALBUS, FALSUM } },
    { offsetof(AnchoresMobiles, cor_iesu),
      { ZEPHYRUM, ZEPHYRUM, "The Most Sacred Heart of Jesus",
        "Sacred Heart", GRADUS_SOLLEMNITAS, ORDO_SOLLEMNITAS_GENERALIS,
        COLLIT_ALBUS, FALSUM } },
    { offsetof(AnchoresMobiles, christus_rex),
      { ZEPHYRUM, ZEPHYRUM,
          "Our Lord Jesus Christ, King of the Universe",
        "Christ the King", GRADUS_SOLLEMNITAS,
        ORDO_SOLLEMNITAS_GENERALIS, COLLIT_ALBUS, FALSUM } },
    { offsetof(AnchoresMobiles, sancta_familia),
      { ZEPHYRUM, ZEPHYRUM, "The Holy Family of Jesus, Mary and Joseph",
        "Holy Family", GRADUS_FESTUM, ORDO_FESTUM_DOMINI, COLLIT_ALBUS,
        FALSUM } },
    { offsetof(AnchoresMobiles, baptisma_domini),
      { ZEPHYRUM, ZEPHYRUM, "The Baptism of the Lord",
        "Baptism of the Lord", GRADUS_FESTUM, ORDO_FESTUM_DOMINI,
        COLLIT_ALBUS, FALSUM } },
    { offsetof(AnchoresMobiles, maria_mater_ecclesiae),
      { ZEPHYRUM, ZEPHYRUM,
          "The Blessed Virgin Mary, Mother of the Church",
        "Mary, Mother of the Church", GRADUS_MEMORIA,
        ORDO_MEMORIA_GENERALIS, COLLIT_ALBUS, FALSUM } },
    { offsetof(AnchoresMobiles, cor_immaculatum),
      { ZEPHYRUM, ZEPHYRUM,
        "The Immaculate Heart of the Blessed Virgin Mary",
        "Immaculate Heart of Mary", GRADUS_MEMORIA,
        ORDO_MEMORIA_GENERALIS, COLLIT_ALBUS, FALSUM } }
};

#define MOBILIA_NUMERUS \
    ((s32)(magnitudo(MOBILIA) / magnitudo(MOBILIA[ZEPHYRUM])))

/* dies anchorae entis mobilis */
interior Dies
dies_mobilis (
     constans AnchoresMobiles* anch,
         constans MobileDatum* m)
{
    redde *(constans Dies*)((constans character*)anch + m->locus);
}


/* ==================================================
 * Praecedentia Liturgica (liturgia-plan L1)
 *
 * Normae Universales de Anno Liturgico 59: OrdoTabulae (header).
 * Dies unus: celebratio temporalis (semper) et sanctorales diei.
 *   - sollemnitas, festum, Defuncti: celebratur si ordo eius MINOR
 *     est ordine temporali (aequales: temporale vincit); victa
 *     omittitur (translatio: L3);
 *   - memoriae: solum si principalis temporalis manet - in feria
 *     privilegiata (ordo IX) cum commemoratio, in feria (XIII)
 *     plenae (ad libitum si gradus ad libitum); aliter omittuntur.
 * Principalis = ordo minimus inter non ad libitum nec commemoratas.
 * ================================================== */

/* ordo temporalis diei: Triduum (feria VI, Sabbatum Sanctum, Dominica
 * Paschae) I; feria V Sancta (dies - Triduum vespere incipit),
 * Cineres, Hebdomada Sancta, octava Paschae, dominicae Adventus,
 * Quadragesimae, Paschae II; ceterae dominicae VI; feriae 17-24 XII,
 * octavae Nativitatis, Quadragesimae IX; ceterae feriae XIII */
interior OrdoTabulae
ordo_temporalis (
          CalendariumLiturgicum* cal,
                           Dies  dies,
    constans InformatioTemporis* tempus)
{
    AnchoresMobiles* anch;

    anch = calendarium_anchorae(cal, dies.annus);
    si (fasti_comparare(dies, anch->pascha) == ZEPHYRUM)
    {
        redde ORDO_TRIDUUM;
    }
    si (tempus->tempus == TEMPUS_TRIDUUM_SACRUM)
    {
        redde fasti_comparare(dies, anch->feria_v_in_cena_domini)
            == ZEPHYRUM ? ORDO_PRIVILEGIATUS : ORDO_TRIDUUM;
    }
    si (   calendarium_est_octava_paschae(cal, dies)
        || calendarium_est_feria_cinerum(cal, dies)
        || calendarium_est_hebdomada_sancta(cal, dies))
    {
        redde ORDO_PRIVILEGIATUS;
    }
    si (tempus->est_dominica)
    {
        si (   tempus->tempus == TEMPUS_ADVENTUS
            || tempus->tempus == TEMPUS_QUADRAGESIMAE
            || tempus->tempus == TEMPUS_PASCHALE)
        {
            redde ORDO_PRIVILEGIATUS;
        }
        redde ORDO_DOMINICA_COMMUNIS;
    }
    si (   (   tempus->tempus == TEMPUS_ADVENTUS
            && dies.mensis == FASTI_DECEMBER
            && dies.dies >= XVII)
        || tempus->tempus == TEMPUS_QUADRAGESIMAE
        || calendarium_est_octava_nativitatis(cal, dies))
    {
        redde ORDO_FERIA_PRIVILEGIATA;
    }
    redde ORDO_FERIA;
}

/* celebratio temporalis in loco dato */
interior vacuum
celebrationem_temporalem_ponere (
          CalendariumLiturgicum* cal,
                           Dies  dies,
    constans InformatioTemporis* tempus,
                         chorda  titulus,
                     Celebratio* celeb)
{
    celeb->titulus         = titulus;
    celeb->titulus_brevis  = titulus;  /* Idem pro nunc */
    celeb->color           = calendarium_color_temporis(cal, dies);
    celeb->genus           = GENUS_TEMPORALE;
    celeb->ordo            = ordo_temporalis(cal, dies, tempus);
    celeb->ad_libitum      = FALSUM;
    celeb->commemoratio    = FALSUM;
    celeb->translata       = FALSUM;
    celeb->dies_originis   = dies;
    celeb->lectionarium    = -I;

    si (tempus->est_dominica)
    {
        celeb->gradus = GRADUS_DOMINICA;
    } alioquin si (   tempus->tempus == TEMPUS_TRIDUUM_SACRUM
                   || calendarium_est_feria_cinerum(cal, dies)
                   || calendarium_est_hebdomada_sancta(cal, dies)
                   || calendarium_est_octava_paschae(cal, dies))
    {
        celeb->gradus = GRADUS_DIES_PECULIARIS;
    } alioquin
    {
        celeb->gradus = GRADUS_FERIA;
    }
}

/* celebratio sanctoralis in loco dato */
interior vacuum
celebrationem_sanctoralem_ponere (
    constans SanctoraleDatum* datum,
                  Celebratio* celeb,
                     Piscina* piscina)
{
    celeb->titulus         = chorda_ex_literis(datum->titulus, piscina);
    celeb->titulus_brevis  = chorda_ex_literis(datum->titulus_brevis,
        piscina);
    celeb->gradus        = datum->gradus;
    celeb->ordo          = datum->ordo;
    celeb->color         = datum->color;
    celeb->genus         = GENUS_SANCTORALE;
    celeb->ad_libitum    = FALSUM;
    celeb->commemoratio  = FALSUM;
    celeb->translata     = FALSUM;
    celeb->lectionarium  = -I;   /* dies_originis: obtinere_diem */
}

interior b32
est_memoria (
    GradusCelebrationis gradus)
{
    redde gradus == GRADUS_MEMORIA
        || gradus == GRADUS_MEMORIA_AD_LIBITUM;
}

/* ordo diei proprius: temporalis, aut mobilis qui eum substituit
 * (sine sanctoralibus et translationibus) */
interior OrdoTabulae
ordo_diei_proprii (
    CalendariumLiturgicum* cal,
                     Dies  dies)
{
    InformatioTemporis  tempus;
           OrdoTabulae  ordo;
       AnchoresMobiles* anch;
                   s32  i;

    tempus  = calendarium_tempus_info(cal, dies);
    ordo    = ordo_temporalis(cal, dies, &tempus);
    anch    = calendarium_anchorae(cal, dies.annus);
    per (i = ZEPHYRUM; i < MOBILIA_NUMERUS; i++)
    {
        si (   !est_memoria(MOBILIA[i].datum.gradus)
            && fasti_comparare(dies_mobilis(anch, &MOBILIA[i]), dies)
            == ZEPHYRUM
            && MOBILIA[i].datum.ordo <= ordo)
        {
            ordo = MOBILIA[i].datum.ordo;
        }
    }
    redde ordo;
}

/* dies libera translationi (UNLY 60): dies non in tabula I-VIII -
 * ordo proprius IX et infra, nullum festum aut sollemnitas
 * sanctoralis, nulla translatio alia */
interior b32
dies_libera (
          CalendariumLiturgicum* cal,
       constans AnnusComputatus* a,
                           Dies  dies)
{
    constans SanctoraleDatum* sancta;
                         s32  numerus;
                         s32  i;

    si (ordo_diei_proprii(cal, dies) < ORDO_FERIA_PRIVILEGIATA)
    {
        redde FALSUM;
    }
    sancta = sanctorale_obtinere(dies.mensis, dies.dies, &numerus);
    per (i = ZEPHYRUM; sancta && i < numerus; i++)
    {
        si (sancta[i].ordo <= ORDO_FESTUM_PROPRIUM)
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < a->numerus_translationum; i++)
    {
        si (fasti_comparare(a->translationes[i].nova, dies) == ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* translationes anni: sollemnitas fixa impedita (ordo diei proprii
 * aequalis aut maior - aequales temporale vincit) ad diem liberum
 * proximum SEQUENTEM; Ioseph in Hebdomada Sancta (CDW 2008) et
 * Ioannes Baptista (CDW 2022: 23 VI) ad PRIOREM */
interior vacuum
translationes_computare (
    CalendariumLiturgicum* cal,
          AnnusComputatus* a)
{
    constans SanctoraleDatum* sancta;
                         s32  numerus;
                         s32  mensis;
                         s32  dies;
                         s32  i;
                         s32  passus;
                        Dies  originis;
                        Dies  nova;
                         b32  retro;

    per (mensis = FASTI_IANUARIUS; mensis <= FASTI_DECEMBER; mensis++)
    {
        per (dies = I; dies <= fasti_dies_in_mense(mensis,
            a->anchorae.annus); dies++)
        {
            sancta = sanctorale_obtinere(mensis, dies, &numerus);
            per (i = ZEPHYRUM; sancta && i < numerus; i++)
            {
                si (sancta[i].gradus != GRADUS_SOLLEMNITAS)
                {
                    perge;
                }
                originis = fasti_dies(a->anchorae.annus, mensis, dies);
                si (ordo_diei_proprii(cal, originis) > sancta[i].ordo)
                {
                    perge;
                }
                retro = (   mensis == CAL_IOSEPH_M
                    && dies == CAL_IOSEPH_D
                         && calendarium_est_hebdomada_sancta(cal,
                         originis))
                    || (   mensis == CAL_NATIVITAS_IOANNIS_M
                        && dies == CAL_NATIVITAS_IOANNIS_D);
                nova = originis;
                per (passus = ZEPHYRUM; passus < LX; passus++)
                {
                    nova = fasti_addere_dies(nova, retro ? -I : I);
                    si (dies_libera(cal, a, nova))
                    {
                        frange;
                    }
                }
                si (   passus < LX
                    && a->numerus_translationum < TRANSLATIONES_MAXIMAE)
                {
                    a->translationes[a->numerus_translationum].datum  =
                        &sancta[i];
                    a->translationes[a->numerus_translationum].originis
                        = originis;
                    a->translationes[a->numerus_translationum].nova  =
                        nova;
                    a->numerus_translationum++;
                }
            }
        }
    }
}

/* annus cum translationibus (computatae semel; nota ANTE computationem
 * - dies_libera et ordo_diei_proprii annum ipsum legunt) */
interior AnnusComputatus*
annum_translationum (
    CalendariumLiturgicum* cal,
                      s32  annus)
{
    AnnusComputatus* a;

    a = annum_obtinere(cal, annus);
    si (a && !a->translationes_computatae)
    {
        a->translationes_computatae = VERUM;
        translationes_computare(cal, a);
    }
    redde a;
}


/* ==================================================
 * Quaestio Principalis
 * ================================================== */

InformatioDiei*
calendarium_obtinere_diem (
    CalendariumLiturgicum* cal,
                     Dies  dies,
                  Piscina* piscina)
{
              InformatioDiei* info;
    constans SanctoraleDatum* sancta;
                         s32  num_sanctorum;
                         s32  i;
    constans SanctoraleDatum* victor;
                         b32  translata;
                        Dies  originis;
             AnnusComputatus* annus;
                  Celebratio* celebrationes;
                         s32  idx;
                         s32  annus_liturgicus;
                 OrdoTabulae  ordo_temp;
             AnchoresMobiles* anch;
                         s32  prima;
                         s32  obligatoriae;

    info = (InformatioDiei*)piscina_allocare(piscina,
        magnitudo(InformatioDiei));

    si (info == NIHIL)
    {
        redde NIHIL;
    }

    info->dies         = dies;
    info->tempus_info  = calendarium_tempus_info(cal, dies);
    info->titulus_diei = calendarium_formare_titulum(cal, dies,
        piscina);

    /* Cyclus lectionum ANNI LITURGICI, qui Dominica I Adventus
     * incipit et nomen anni civilis in quo finit fert: a Dominica
     * I Adventus ad XXXI Dec annus liturgicus est annus civilis
     * SEQUENS. Olim annus civilis legebatur, ergo per Adventum et
     * octavam Nativitatis cyclus anno uno retro erat (2026-09-12). */
    annus_liturgicus = dies.annus;
    si (fasti_comparare(dies, calendarium_anchorae(cal,
            dies.annus)->dominica_i_adventus) >= ZEPHYRUM)
    {
        annus_liturgicus = dies.annus + I;
    }
    info->cyclus.cyclus_dominicalis =
        calendarium_cyclus_dominicalis(annus_liturgicus);
    info->cyclus.cyclus_quotidianus =
        calendarium_cyclus_quotidianus(annus_liturgicus);
    info->cyclus.hebdomada_psalterii =
        calendarium_hebdomada_psalterii(cal, dies);

    sancta = sanctorale_obtinere(dies.mensis, dies.dies,
        &num_sanctorum);
    anch = calendarium_anchorae(cal, dies.annus);
    /* dies, sanctorales, memoriae mobiles (II summum) */
    celebrationes = (Celebratio*)piscina_allocare(piscina,
        magnitudo(Celebratio) * (memoriae_index)(III + num_sanctorum));
    si (celebrationes == NIHIL)
    {
        redde NIHIL;
    }

    /* temporale primum (semper) */
    celebrationem_temporalem_ponere(cal, dies, &info->tempus_info,
        info->titulus_diei, &celebrationes[ZEPHYRUM]);

    /* mobilis non memoria: dies IPSE (temporale substituit) */
    per (i = ZEPHYRUM; i < MOBILIA_NUMERUS; i++)
    {
        si (   !est_memoria(MOBILIA[i].datum.gradus)
            && fasti_comparare(dies_mobilis(anch, &MOBILIA[i]), dies)
            == ZEPHYRUM
            && MOBILIA[i].datum.ordo <= celebrationes[ZEPHYRUM].ordo)
        {
            celebrationem_sanctoralem_ponere(&MOBILIA[i].datum,
                &celebrationes[ZEPHYRUM], piscina);
            celebrationes[ZEPHYRUM].genus = GENUS_MOBILE;
        }
    }
    ordo_temp  = celebrationes[ZEPHYRUM].ordo;
    idx        = I;

    /* sollemnitas, festum, Defuncti (et translatae huc - L3): victor
     * unus, si temporale vincit */
    victor     = NIHIL;
    translata  = FALSUM;
    originis   = dies;
    per (i = ZEPHYRUM; i < num_sanctorum; i++)
    {
        si (   !est_memoria(sancta[i].gradus)
            && sancta[i].ordo < ordo_temp
            && (!victor || sancta[i].ordo < victor->ordo))
        {
            victor = &sancta[i];
        }
    }
    annus = annum_translationum(cal, dies.annus);
    per (i = ZEPHYRUM; annus && i < annus->numerus_translationum; i++)
    {
        si (   fasti_comparare(annus->translationes[i].nova, dies)
            == ZEPHYRUM
            && annus->translationes[i].datum->ordo < ordo_temp
            && (!victor || annus->translationes[i].datum->ordo
            < victor->ordo))
        {
            victor     = annus->translationes[i].datum;
            translata  = VERUM;
            originis   = annus->translationes[i].originis;
        }
    }
    si (victor)
    {
        celebrationem_sanctoralem_ponere(victor, &celebrationes[idx],
            piscina);
        celebrationes[idx].translata      = translata;
        celebrationes[idx].dies_originis  = originis;
        idx++;
    }

    /* memoriae (sanctorales et mobiles): solum sub principali
     * temporali - feria privilegiata: commemoratio; feria: plenae;
     * obligatoriae duae eodem die: ambae ad libitum (L2) */
    prima = idx;
    si (   !victor
        && (   ordo_temp == ORDO_FERIA_PRIVILEGIATA
            || ordo_temp == ORDO_FERIA))
    {
        per (i = ZEPHYRUM; i < num_sanctorum; i++)
        {
            si (est_memoria(sancta[i].gradus))
            {
                celebrationem_sanctoralem_ponere(&sancta[i],
                    &celebrationes[idx++], piscina);
            }
        }
        per (i = ZEPHYRUM; i < MOBILIA_NUMERUS; i++)
        {
            si (   est_memoria(MOBILIA[i].datum.gradus)
                && fasti_comparare(dies_mobilis(anch, &MOBILIA[i]),
                dies)
                == ZEPHYRUM)
            {
                celebrationem_sanctoralem_ponere(&MOBILIA[i].datum,
                    &celebrationes[idx], piscina);
                celebrationes[idx++].genus = GENUS_MOBILE;
            }
        }
    }
    obligatoriae = ZEPHYRUM;
    per (i = prima; i < idx; i++)
    {
        si (celebrationes[i].gradus == GRADUS_MEMORIA)
        {
            obligatoriae++;
        }
    }
    per (i = prima; i < idx; i++)
    {
        celebrationes[i].commemoratio  = ordo_temp
            == ORDO_FERIA_PRIVILEGIATA;
        celebrationes[i].ad_libitum    = !celebrationes[i].commemoratio
            && (   celebrationes[i].gradus == GRADUS_MEMORIA_AD_LIBITUM
                || obligatoriae > I);
    }

    per (i = ZEPHYRUM; i < idx; i++)
    {
        si (!celebrationes[i].translata)
        {
            celebrationes[i].dies_originis = dies;
        }
    }
    info->celebrationes          = celebrationes;
    info->numerus_celebrationum  = idx;

    /* principalis: ordo minimus inter non ad libitum nec commemoratas;
     * aequales: prior (temporale) */
    info->celebratio_principalis = &celebrationes[ZEPHYRUM];
    per (i = I; i < idx; i++)
    {
        si (   !celebrationes[i].ad_libitum
            && !celebrationes[i].commemoratio
            && celebrationes[i].ordo
            < info->celebratio_principalis->ordo)
        {
            info->celebratio_principalis = &celebrationes[i];
        }
    }
    info->color_diei = info->celebratio_principalis->color;

    /* principalis sanctoralis aut mobilis (festum et supra; Palmae
     * dominica, Cineres dies peculiaris): titulus diei eius */
    si (   info->celebratio_principalis->genus  != GENUS_TEMPORALE
        && info->celebratio_principalis->gradus >= GRADUS_FESTUM)
    {
        info->titulus_diei = info->celebratio_principalis->titulus;
    }

    redde info;
}
