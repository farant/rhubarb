/* ==================================================
 * PROBATIO CALENDARIUM LITURGICUM
 * Probationes pro bibliotheca calendarii liturgici
 * ================================================== */

#include "latina.h"
#include "calendarium_liturgicum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


s32 principale(vacuum)
{
    Piscina* piscina;
    CalendariumLiturgicum* cal;
    AnchoresMobiles* anch;
    InformatioDiei* info;
    Dies dies;
    chorda titulus;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_calendarium", M * M);
    si (!piscina) {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    cal = calendarium_creare(piscina);


    /* ==================================================
     * Probare Anchorae Mobiles 2024
     * ================================================== */
    {
        imprimere("\n--- Probans Anchorae 2024 ---\n");

        anch = calendarium_anchorae(cal, MMXXIV);

        /* Pascha 2024 = 31 Martii */
        CREDO_AEQUALIS_S32(anch->pascha.annus, MMXXIV);
        CREDO_AEQUALIS_S32(anch->pascha.mensis, FASTI_MARTIUS);
        CREDO_AEQUALIS_S32(anch->pascha.dies, XXXI);

        /* Feria IV Cinerum 2024 = 14 Februarii */
        CREDO_AEQUALIS_S32(anch->feria_iv_cinerum.mensis, FASTI_FEBRUARIUS);
        CREDO_AEQUALIS_S32(anch->feria_iv_cinerum.dies, XIV);

        /* Pentecoste 2024 = 19 Maii */
        CREDO_AEQUALIS_S32(anch->pentecoste.mensis, FASTI_MAIUS);
        CREDO_AEQUALIS_S32(anch->pentecoste.dies, XIX);

        /* Dominica I Adventus 2024 = 1 Decembris */
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.mensis, FASTI_DECEMBER);
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.dies, I);
    }


    /* ==================================================
     * Probare Anchorae Mobiles 2025
     * ================================================== */
    {
        imprimere("\n--- Probans Anchorae 2025 ---\n");

        anch = calendarium_anchorae(cal, MMXXV);

        /* Pascha 2025 = 20 Aprilis */
        CREDO_AEQUALIS_S32(anch->pascha.annus, MMXXV);
        CREDO_AEQUALIS_S32(anch->pascha.mensis, FASTI_APRILIS);
        CREDO_AEQUALIS_S32(anch->pascha.dies, XX);

        /* Feria IV Cinerum 2025 = 5 Martii */
        CREDO_AEQUALIS_S32(anch->feria_iv_cinerum.mensis, FASTI_MARTIUS);
        CREDO_AEQUALIS_S32(anch->feria_iv_cinerum.dies, V);

        /* Pentecoste 2025 = 8 Iunii */
        CREDO_AEQUALIS_S32(anch->pentecoste.mensis, FASTI_IUNIUS);
        CREDO_AEQUALIS_S32(anch->pentecoste.dies, VIII);

        /* Dominica I Adventus 2025 = 30 Novembris */
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.mensis, FASTI_NOVEMBER);
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.dies, XXX);
    }


    /* ==================================================
     * Probare Tempus Adventus
     * ================================================== */
    {
        imprimere("\n--- Probans Tempus Adventus ---\n");

        /* 1 Dec 2024 = Dominica I Adventus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, I);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_ADVENTUS);

        /* 24 Dec 2024 = Feria IV Adventus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, XXIV);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_ADVENTUS);
    }


    /* ==================================================
     * Probare Tempus Nativitatis
     * ================================================== */
    {
        imprimere("\n--- Probans Tempus Nativitatis ---\n");

        /* 25 Dec 2024 = Nativitas Domini */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, XXV);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_NATIVITATIS);

        /* 1 Ian 2025 = Octava Nativitatis (Maria Mater Dei) */
        dies = fasti_dies(MMXXV, FASTI_IANUARIUS, I);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_NATIVITATIS);
    }


    /* ==================================================
     * Probare Tempus Quadragesimae
     * ================================================== */
    {
        imprimere("\n--- Probans Tempus Quadragesimae ---\n");

        /* 5 Mar 2025 = Feria IV Cinerum */
        dies = fasti_dies(MMXXV, FASTI_MARTIUS, V);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_QUADRAGESIMAE);
        CREDO_VERUM(calendarium_est_feria_cinerum(cal, dies));

        /* 13 Apr 2025 = Dominica Palmarum (Hebdomada Sancta) */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XIII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_QUADRAGESIMAE);
        CREDO_VERUM(calendarium_est_hebdomada_sancta(cal, dies));
    }


    /* ==================================================
     * Probare Triduum Sacrum
     * ================================================== */
    {
        imprimere("\n--- Probans Triduum Sacrum ---\n");

        /* 17 Apr 2025 = Feria V in Cena Domini */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XVII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_TRIDUUM_SACRUM);
        CREDO_VERUM(calendarium_est_triduum(cal, dies));

        /* 18 Apr 2025 = Feria VI in Passione */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XVIII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_TRIDUUM_SACRUM);

        /* 19 Apr 2025 = Sabbatum Sanctum */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XIX);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_TRIDUUM_SACRUM);
    }


    /* ==================================================
     * Probare Tempus Paschale
     * ================================================== */
    {
        imprimere("\n--- Probans Tempus Paschale ---\n");

        /* 20 Apr 2025 = Dominica Paschae */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XX);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_PASCHALE);
        CREDO_VERUM(calendarium_est_octava_paschae(cal, dies));

        /* 27 Apr 2025 = Dominica in Albis */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XXVII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_PASCHALE);
        CREDO_VERUM(calendarium_est_octava_paschae(cal, dies));

        /* 8 Iun 2025 = Pentecoste */
        dies = fasti_dies(MMXXV, FASTI_IUNIUS, VIII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_PASCHALE);
    }


    /* ==================================================
     * Probare Tempus per Annum
     * ================================================== */
    {
        imprimere("\n--- Probans Tempus per Annum ---\n");

        /* 15 Ian 2025 = Tempus per Annum I (post Baptisma Domini) */
        dies = fasti_dies(MMXXV, FASTI_IANUARIUS, XV);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_PER_ANNUM_I);

        /* 15 Iun 2025 = Tempus per Annum II (post Pentecosten) */
        dies = fasti_dies(MMXXV, FASTI_IUNIUS, XV);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies), (s32)TEMPUS_PER_ANNUM_II);
    }


    /* ==================================================
     * Probare Colores Temporum
     * ================================================== */
    {
        imprimere("\n--- Probans Colores ---\n");

        /* Adventus = Violaceus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, I);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies), (s32)COLOR_VIOLACEUS);

        /* Nativitas = Albus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, XXV);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies), (s32)COLOR_ALBUS);

        /* Tempus per Annum = Viridis */
        dies = fasti_dies(MMXXV, FASTI_IUNIUS, XV);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies), (s32)COLOR_VIRIDIS);

        /* Quadragesima = Violaceus */
        dies = fasti_dies(MMXXV, FASTI_MARTIUS, X);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies), (s32)COLOR_VIOLACEUS);

        /* Pascha = Albus */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XX);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies), (s32)COLOR_ALBUS);

        /* Pentecoste = Ruber */
        dies = fasti_dies(MMXXV, FASTI_IUNIUS, VIII);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies), (s32)COLOR_RUBER);
    }


    /* ==================================================
     * Probare Cyclus Lectionum
     * ================================================== */
    {
        imprimere("\n--- Probans Cyclus ---\n");

        /* 2024 = B, 2025 = C, 2023 = A */
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXIV), (s32)CYCLUS_B);
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXV), (s32)CYCLUS_C);
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXIV - I), (s32)CYCLUS_A);  /* 2023 */

        /* 2024 = II (par), 2025 = I (impar) */
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_quotidianus(MMXXIV), (s32)CYCLUS_II);
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_quotidianus(MMXXV), (s32)CYCLUS_I);
    }


    /* ==================================================
     * Probare Octavae
     * ================================================== */
    {
        imprimere("\n--- Probans Octavae ---\n");

        /* Octava Paschae */
        CREDO_VERUM(calendarium_est_octava_paschae(cal,
            fasti_dies(MMXXV, FASTI_APRILIS, XX)));
        CREDO_VERUM(calendarium_est_octava_paschae(cal,
            fasti_dies(MMXXV, FASTI_APRILIS, XXVII)));
        CREDO_FALSUM(calendarium_est_octava_paschae(cal,
            fasti_dies(MMXXV, FASTI_APRILIS, XXVIII)));

        /* Octava Nativitatis */
        CREDO_VERUM(calendarium_est_octava_nativitatis(cal,
            fasti_dies(MMXXIV, FASTI_DECEMBER, XXV)));
        CREDO_VERUM(calendarium_est_octava_nativitatis(cal,
            fasti_dies(MMXXV, FASTI_IANUARIUS, I)));
        CREDO_FALSUM(calendarium_est_octava_nativitatis(cal,
            fasti_dies(MMXXV, FASTI_IANUARIUS, II)));
    }


    /* ==================================================
     * Probare Formatatio
     * ================================================== */
    {
        imprimere("\n--- Probans Formatatio ---\n");

        titulus = calendarium_nomen_temporis(TEMPUS_ADVENTUS, FALSUM, piscina);
        CREDO_AEQUALIS_S32((s32)titulus.mensura, XV);  /* "Tempus Adventus" = 15 chars */

        titulus = calendarium_nomen_temporis(TEMPUS_ADVENTUS, VERUM, piscina);
        CREDO_AEQUALIS_S32((s32)titulus.mensura, III);  /* "Adv" */

        titulus = calendarium_nomen_coloris(COLOR_ALBUS, piscina);
        CREDO_AEQUALIS_S32((s32)titulus.mensura, V);  /* "Albus" */

        /* Formare titulum pro Dominica I Adventus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, I);
        titulus = calendarium_formare_titulum(cal, dies, piscina);
        CREDO_VERUM(titulus.mensura > ZEPHYRUM);
    }


    /* ==================================================
     * Probare InformatioDiei
     * ================================================== */
    {
        imprimere("\n--- Probans InformatioDiei ---\n");

        /* Dominica I Adventus 2024 */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, I);
        info = calendarium_obtinere_diem(cal, dies, piscina);

        CREDO_VERUM(info != NIHIL);
        CREDO_AEQUALIS_S32(info->dies.annus, MMXXIV);
        CREDO_AEQUALIS_S32(info->dies.mensis, FASTI_DECEMBER);
        CREDO_AEQUALIS_S32(info->dies.dies, I);
        CREDO_AEQUALIS_S32((s32)info->tempus_info.tempus, (s32)TEMPUS_ADVENTUS);
        CREDO_VERUM(info->tempus_info.est_dominica);
        CREDO_AEQUALIS_S32((s32)info->color_diei, (s32)COLOR_VIOLACEUS);
        CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_dominicalis, (s32)CYCLUS_B);
    }


    /* Compendium */
    imprimere("\n");
    credo_imprimere_compendium();

    calendarium_destruere(cal);
    credo_claudere();
    piscina_destruere(piscina);

    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
