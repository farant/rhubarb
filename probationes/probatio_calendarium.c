/* ==================================================
 * PROBATIO CALENDARIUM LITURGICUM
 * Probationes pro bibliotheca calendarii liturgici
 * ================================================== */

#include "latina.h"
#include "calendarium_liturgicum.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


/* hebdomada et hebdomada psalterii diei - brevitatis causa */
interior s32
_hebdomada_diei (
    CalendariumLiturgicum* cal,
                      s32  annus,
                      s32  mensis,
                      s32  dies)
{
    redde calendarium_tempus_info(cal,
        fasti_dies(annus, mensis, dies)).hebdomada;
}

interior s32
_psalterium_diei (
    CalendariumLiturgicum* cal,
                      s32  annus,
                      s32  mensis,
                      s32  dies)
{
    redde calendarium_hebdomada_psalterii(cal,
        fasti_dies(annus, mensis, dies));
}


s32 principale(vacuum)
{
                  Piscina* piscina;
    CalendariumLiturgicum* cal;
          AnchoresMobiles* anch;
           InformatioDiei* info;
                     Dies  dies;
                   chorda  titulus;
                      b32  praeteritus;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_calendarium", M * M);
    si (!piscina)
    {
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
        CREDO_AEQUALIS_S32(anch->feria_iv_cinerum.mensis,
            FASTI_FEBRUARIUS);
        CREDO_AEQUALIS_S32(anch->feria_iv_cinerum.dies, XIV);

        /* Pentecoste 2024 = 19 Maii */
        CREDO_AEQUALIS_S32(anch->pentecoste.mensis, FASTI_MAIUS);
        CREDO_AEQUALIS_S32(anch->pentecoste.dies, XIX);

        /* Dominica I Adventus 2024 = 1 Decembris */
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.mensis,
            FASTI_DECEMBER);
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
        CREDO_AEQUALIS_S32(anch->feria_iv_cinerum.mensis,
            FASTI_MARTIUS);
        CREDO_AEQUALIS_S32(anch->feria_iv_cinerum.dies, V);

        /* Pentecoste 2025 = 8 Iunii */
        CREDO_AEQUALIS_S32(anch->pentecoste.mensis, FASTI_IUNIUS);
        CREDO_AEQUALIS_S32(anch->pentecoste.dies, VIII);

        /* Dominica I Adventus 2025 = 30 Novembris */
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.mensis,
            FASTI_NOVEMBER);
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.dies, XXX);
    }


    /* ==================================================
     * Probare Anchorae Mobiles 2026 et Adventum 2023
     * ================================================== */

    {
        imprimere("\n--- Probans Anchorae 2026 (et Adventus 2023)"
            " ---\n");

        anch = calendarium_anchorae(cal, MM + XXVI);

        /* Pascha 2026 = 5 Aprilis; Ascensio (feria V) = 14 Maii */
        CREDO_AEQUALIS_S32(anch->pascha.mensis, FASTI_APRILIS);
        CREDO_AEQUALIS_S32(anch->pascha.dies, V);
        CREDO_AEQUALIS_S32(anch->ascensio.mensis, FASTI_MAIUS);
        CREDO_AEQUALIS_S32(anch->ascensio.dies, XIV);

        /* Sacratissimum Cor Iesu 2026 = 12 Iunii (feria VI) */
        CREDO_AEQUALIS_S32(anch->cor_iesu.mensis, FASTI_IUNIUS);
        CREDO_AEQUALIS_S32(anch->cor_iesu.dies, XII);

        /* Baptisma Domini 2026 = 11 Ianuarii (Epiphania feria III) */
        CREDO_AEQUALIS_S32(anch->baptisma_domini.mensis,
            FASTI_IANUARIUS);
        CREDO_AEQUALIS_S32(anch->baptisma_domini.dies, XI);

        /* Christus Rex 2026 = 22 Novembris; Adventus I = 29 */
        CREDO_AEQUALIS_S32(anch->christus_rex.mensis, FASTI_NOVEMBER);
        CREDO_AEQUALIS_S32(anch->christus_rex.dies, XXII);
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.mensis,
            FASTI_NOVEMBER);
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.dies, XXIX);

        /* Adventus 2023: vigilia Nativitatis IPSA dominica est, ergo
         * Dominica IV = 24 Dec et Adventus I = 3 Decembris */
        anch = calendarium_anchorae(cal, MMXXIII);
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.mensis,
            FASTI_DECEMBER);
        CREDO_AEQUALIS_S32(anch->dominica_i_adventus.dies, III);
    }


    /* ==================================================
     * Probare Tempus Adventus
     * ================================================== */

    {
        imprimere("\n--- Probans Tempus Adventus ---\n");

        /* 1 Dec 2024 = Dominica I Adventus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, I);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_ADVENTUS);

        /* 24 Dec 2024 = Feria IV Adventus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, XXIV);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_ADVENTUS);
    }


    /* ==================================================
     * Probare Tempus Nativitatis
     * ================================================== */

    {
        imprimere("\n--- Probans Tempus Nativitatis ---\n");

        /* 25 Dec 2024 = Nativitas Domini */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, XXV);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_NATIVITATIS);

        /* 1 Ian 2025 = Octava Nativitatis (Maria Mater Dei) */
        dies = fasti_dies(MMXXV, FASTI_IANUARIUS, I);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_NATIVITATIS);
    }


    /* ==================================================
     * Probare Tempus Quadragesimae
     * ================================================== */

    {
        imprimere("\n--- Probans Tempus Quadragesimae ---\n");

        /* 5 Mar 2025 = Feria IV Cinerum */
        dies = fasti_dies(MMXXV, FASTI_MARTIUS, V);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_QUADRAGESIMAE);
        CREDO_VERUM(calendarium_est_feria_cinerum(cal, dies));

        /* 13 Apr 2025 = Dominica Palmarum (Hebdomada Sancta) */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XIII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_QUADRAGESIMAE);
        CREDO_VERUM(calendarium_est_hebdomada_sancta(cal, dies));
    }


    /* ==================================================
     * Probare Triduum Sacrum
     * ================================================== */

    {
        imprimere("\n--- Probans Triduum Sacrum ---\n");

        /* 17 Apr 2025 = Feria V in Cena Domini */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XVII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_TRIDUUM_SACRUM);
        CREDO_VERUM(calendarium_est_triduum(cal, dies));

        /* 18 Apr 2025 = Feria VI in Passione */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XVIII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_TRIDUUM_SACRUM);

        /* 19 Apr 2025 = Sabbatum Sanctum */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XIX);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_TRIDUUM_SACRUM);
    }


    /* ==================================================
     * Probare Tempus Paschale
     * ================================================== */

    {
        imprimere("\n--- Probans Tempus Paschale ---\n");

        /* 20 Apr 2025 = Dominica Paschae */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XX);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_PASCHALE);
        CREDO_VERUM(calendarium_est_octava_paschae(cal, dies));

        /* 27 Apr 2025 = Dominica in Albis */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XXVII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_PASCHALE);
        CREDO_VERUM(calendarium_est_octava_paschae(cal, dies));

        /* 8 Iun 2025 = Pentecoste */
        dies = fasti_dies(MMXXV, FASTI_IUNIUS, VIII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_PASCHALE);
    }


    /* ==================================================
     * Probare Tempus per Annum
     * ================================================== */

    {
        imprimere("\n--- Probans Tempus per Annum ---\n");

        /* 15 Ian 2025 = Tempus per Annum I (post Baptisma Domini) */
        dies = fasti_dies(MMXXV, FASTI_IANUARIUS, XV);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_PER_ANNUM_I);

        /* 15 Iun 2025 = Tempus per Annum II (post Pentecosten) */
        dies = fasti_dies(MMXXV, FASTI_IUNIUS, XV);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_PER_ANNUM_II);
    }


    /* ==================================================
     * Probare Hebdomadas (numerus hebdomadae et psalterii)
     *
     * Dies noti sine hac bibliotheca. Post Pentecosten
     * numeratio RETRORSUM fit: hebdomada Christi Regis est
     * XXXIV, et hebdomada psalterii ex hebdomada rotat.
     * ================================================== */

    {
        imprimere("\n--- Probans Hebdomadas ---\n");

        /* Tempus per annum I: 12 Ian 2026 = feria II hebdomadae I;
         * 18 Ian = Dominica II; 17 Feb (ante Cineres) = VI */
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_IANUARIUS, XII), I);
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_IANUARIUS, XVIII), II);
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_FEBRUARIUS, XVII), VI);
        CREDO_AEQUALIS_S32(_psalterium_diei(cal,
            MM + XXVI, FASTI_FEBRUARIUS, XVII), II);

        /* Quadragesima et Pascha 2026 */
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_FEBRUARIUS, XXII), I);
        titulus = calendarium_formare_titulum(cal,
            fasti_dies(MM + XXVI, FASTI_FEBRUARIUS, XXII), piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(titulus,
            "Dominica I Quadragesimae");
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_APRILIS, XII), II);
        titulus = calendarium_formare_titulum(cal,
            fasti_dies(MM + XXVI, FASTI_APRILIS, XII), piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Dominica II Paschae");

        /* Adventus: 21 Dec 2025 = Dominica IV; 29 Nov 2026 = I */
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MMXXV, FASTI_DECEMBER, XXI), IV);
        dies = fasti_dies(MM + XXVI, FASTI_NOVEMBER, XXIX);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_ADVENTUS);
        CREDO_AEQUALIS_S32(calendarium_tempus_info(cal, dies).hebdomada,
            I);
        titulus = calendarium_formare_titulum(cal, dies, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(titulus, "Dominica I Adventus");

        /* Tempus per annum II. Ultima dies ante Adventum 2026 */
        dies = fasti_dies(MM + XXVI, FASTI_NOVEMBER, XXVIII);
        CREDO_AEQUALIS_S32((s32)calendarium_tempus(cal, dies),
            (s32)TEMPUS_PER_ANNUM_II);
        CREDO_AEQUALIS_S32(calendarium_tempus_info(cal, dies).hebdomada,
            XXXIV);

        /* Feria II post Pentecosten: 2024 = VII, 2025 = X,
         * 2026 = VIII (psalterium IV) */
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MMXXIV, FASTI_MAIUS, XX), VII);
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MMXXV, FASTI_IUNIUS, IX), X);
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_MAIUS, XXV), VIII);
        CREDO_AEQUALIS_S32(_psalterium_diei(cal,
            MM + XXVI, FASTI_MAIUS, XXV), IV);

        /* Septembris 2026: sabbatum 12 = hebdomada XXIII, dominica
         * 13 = XXIV (psalterium IV) */
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_SEPTEMBER, XII), XXIII);
        titulus = calendarium_formare_titulum(cal,
            fasti_dies(MM + XXVI, FASTI_SEPTEMBER, XII), piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(titulus,
            "Sabbatum, Hebdomada XXIII");
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_SEPTEMBER, XIII), XXIV);
        CREDO_AEQUALIS_S32(_psalterium_diei(cal,
            MM + XXVI, FASTI_SEPTEMBER, XIII), IV);
        titulus = calendarium_formare_titulum(cal,
            fasti_dies(MM + XXVI, FASTI_SEPTEMBER, XIII), piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(titulus,
            "Dominica XXIV per Annum");

        /* Christus Rex = Dominica XXXIV (psalterium II):
         * 23 Nov 2025 et 22 Nov 2026 */
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MMXXV, FASTI_NOVEMBER, XXIII), XXXIV);
        CREDO_AEQUALIS_S32(_hebdomada_diei(cal,
            MM + XXVI, FASTI_NOVEMBER, XXII), XXXIV);
        CREDO_AEQUALIS_S32(_psalterium_diei(cal,
            MM + XXVI, FASTI_NOVEMBER, XXII), II);
    }


    /* ==================================================
     * Probare Colores Temporum
     * ================================================== */

    {
        imprimere("\n--- Probans Colores ---\n");

        /* Adventus = Violaceus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, I);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies),
            (s32)COLLIT_VIOLACEUS);

        /* Nativitas = Albus */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, XXV);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies),
            (s32)COLLIT_ALBUS);

        /* Tempus per Annum = Viridis */
        dies = fasti_dies(MMXXV, FASTI_IUNIUS, XV);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies),
            (s32)COLLIT_VIRIDIS);

        /* Quadragesima = Violaceus */
        dies = fasti_dies(MMXXV, FASTI_MARTIUS, X);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies),
            (s32)COLLIT_VIOLACEUS);

        /* Pascha = Albus */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XX);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies),
            (s32)COLLIT_ALBUS);

        /* Pentecoste = Ruber */
        dies = fasti_dies(MMXXV, FASTI_IUNIUS, VIII);
        CREDO_AEQUALIS_S32((s32)calendarium_color_temporis(cal, dies),
            (s32)COLLIT_RUBER);
    }


    /* ==================================================
     * Probare Cyclus Lectionum
     * ================================================== */

    {
        imprimere("\n--- Probans Cyclus ---\n");

        /* 2024 = B, 2025 = C, 2023 = A */
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXIV),
            (s32)CYCLUS_B);
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXV),
            (s32)CYCLUS_C);
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_dominicalis(MMXXIV
            - I), (s32)CYCLUS_A);  /* 2023 */

        /* 2024 = II (par), 2025 = I (impar) */
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_quotidianus(MMXXIV),
            (s32)CYCLUS_II);
        CREDO_AEQUALIS_S32((s32)calendarium_cyclus_quotidianus(MMXXV),
            (s32)CYCLUS_I);
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

        titulus = calendarium_nomen_temporis(TEMPUS_ADVENTUS, FALSUM,
            piscina);
        CREDO_AEQUALIS_S32((s32)titulus.mensura, XV);  /* "Tempus Adventus" = 15 chars */

        titulus = calendarium_nomen_temporis(TEMPUS_ADVENTUS, VERUM,
            piscina);
        CREDO_AEQUALIS_S32((s32)titulus.mensura, III);  /* "Adv" */

        titulus = calendarium_nomen_coloris(COLLIT_ALBUS, piscina);
        CREDO_AEQUALIS_S32((s32)titulus.mensura, V);  /* "Albus" */

        /* Formare titulum pro Dominica I Adventus */
        dies     = fasti_dies(MMXXIV, FASTI_DECEMBER, I);
        titulus  = calendarium_formare_titulum(cal, dies, piscina);
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
        CREDO_AEQUALIS_S32((s32)info->tempus_info.tempus,
            (s32)TEMPUS_ADVENTUS);
        CREDO_VERUM(info->tempus_info.est_dominica);
        CREDO_AEQUALIS_S32((s32)info->color_diei,
            (s32)COLLIT_VIOLACEUS);
        CREDO_AEQUALIS_S32((s32)info->cyclus.cyclus_dominicalis,
            (s32)CYCLUS_B);
    }


    /* ==================================================
     * Probare Sanctorale
     * ================================================== */

    {
        constans SanctoraleDatum* sanct;
                             s32  numerus;

        imprimere("\n--- Probans Sanctorale ---\n");

        /* Numerus totalis */
        CREDO_VERUM(sanctorale_numerus() > C);  /* Plus quam 100 */

        /* 1 Ian - Maria Mater Dei (Sollemnitas, dies obligationis) */
        sanct = sanctorale_obtinere(FASTI_IANUARIUS, I, &numerus);
        CREDO_VERUM(sanct != NIHIL);
        CREDO_AEQUALIS_S32(numerus, I);
        CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
        CREDO_AEQUALIS_S32((s32)sanct->color, (s32)COLLIT_ALBUS);
        CREDO_VERUM(sanct->dies_obligationis);

        /* 25 Dec - Nativitas (Sollemnitas, dies obligationis) */
        sanct = sanctorale_obtinere(FASTI_DECEMBER, XXV, &numerus);
        CREDO_VERUM(sanct != NIHIL);
        CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
        CREDO_VERUM(sanct->dies_obligationis);

        /* 15 Aug - Assumptio (Sollemnitas, dies obligationis) */
        sanct = sanctorale_obtinere(FASTI_AUGUSTUS, XV, &numerus);
        CREDO_VERUM(sanct != NIHIL);
        CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
        CREDO_VERUM(sanct->dies_obligationis);

        /* 1 Nov - Omnes Sancti (Sollemnitas, dies obligationis) */
        sanct = sanctorale_obtinere(FASTI_NOVEMBER, I, &numerus);
        CREDO_VERUM(sanct != NIHIL);
        CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
        CREDO_VERUM(sanct->dies_obligationis);

        /* 8 Dec - Immaculata Conceptio (Sollemnitas, dies obligationis) */
        sanct = sanctorale_obtinere(FASTI_DECEMBER, VIII, &numerus);
        CREDO_VERUM(sanct != NIHIL);
        CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
        CREDO_VERUM(sanct->dies_obligationis);

        /* 29 Jun - Petrus et Paulus (Sollemnitas, non dies obligationis in US) */
        sanct = sanctorale_obtinere(FASTI_IUNIUS, XXIX, &numerus);
        CREDO_VERUM(sanct != NIHIL);
        CREDO_AEQUALIS_S32((s32)sanct->gradus, (s32)GRADUS_SOLLEMNITAS);
        CREDO_AEQUALIS_S32((s32)sanct->color, (s32)COLLIT_RUBER);
        CREDO_FALSUM(sanct->dies_obligationis);

        /* 20 Jan - Duo sancti (Fabianus et Sebastianus) */
        sanct = sanctorale_obtinere(FASTI_IANUARIUS, XX, &numerus);
        CREDO_VERUM(sanct != NIHIL);
        CREDO_AEQUALIS_S32(numerus, II);  /* Duo celebrationes */

        /* 10 Ian - Nullus sanctus */
        sanct = sanctorale_obtinere(FASTI_IANUARIUS, X, &numerus);
        CREDO_VERUM(sanct == NIHIL);
        CREDO_AEQUALIS_S32(numerus, ZEPHYRUM);
    }


    /* ==================================================
     * Probare Praecedentiam
     * ================================================== */

    {
        imprimere("\n--- Probans Praecedentiam ---\n");

        /* 28 Jan 2025 - St. Thomas Aquinas (Memoria) in Ordinary Time */
        /* Memoria celebratur */
        dies = fasti_dies(MMXXV, FASTI_IANUARIUS, XXVIII);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->numerus_celebrationum >= II);  /* Temporale + Sanctorale */
        CREDO_VERUM(info->celebratio_principalis != NIHIL);
        CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
            (s32)GRADUS_MEMORIA);

        /* 25 Dec 2024 - Nativitas Domini (Sollemnitas) */
        /* Sollemnitas vincit */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, XXV);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->celebratio_principalis != NIHIL);
        CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
            (s32)GRADUS_SOLLEMNITAS);
        CREDO_AEQUALIS_S32((s32)info->color_diei, (s32)COLLIT_ALBUS);

        /* 19 Mar 2025 - St. Joseph (Sollemnitas) */
        /* In Lent, but Solemnity takes precedence */
        dies = fasti_dies(MMXXV, FASTI_MARTIUS, XIX);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->celebratio_principalis != NIHIL);
        CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
            (s32)GRADUS_SOLLEMNITAS);
        CREDO_AEQUALIS_S32((s32)info->color_diei, (s32)COLLIT_ALBUS);

        /* 5 Mar 2025 - Ash Wednesday */
        /* Dies Peculiaris - no saint takes precedence */
        dies = fasti_dies(MMXXV, FASTI_MARTIUS, V);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->celebratio_principalis != NIHIL);
        CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
            (s32)GRADUS_DIES_PECULIARIS);
        CREDO_AEQUALIS_S32((s32)info->color_diei,
            (s32)COLLIT_VIOLACEUS);

        /* 1 Dec 2024 - Dominica I Adventus */
        /* Sunday takes precedence over any memoria */
        dies = fasti_dies(MMXXIV, FASTI_DECEMBER, I);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->celebratio_principalis != NIHIL);
        CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
            (s32)GRADUS_DOMINICA);

        /* 2 Nov 2025 - All Souls Day */
        /* Dies peculiaris - special case */
        dies = fasti_dies(MMXXV, FASTI_NOVEMBER, II);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->numerus_celebrationum >= I);

        /* 25 Apr 2025 - St. Mark (Festum) */
        /* NB: Pascha 2025 = 20 Apr, ergo 25 Apr est in Octava Paschae! */
        /* Octava Paschae vincit festum - St. Mark transferred */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XXV);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->celebratio_principalis != NIHIL);
        CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
            (s32)GRADUS_DIES_PECULIARIS);
        CREDO_AEQUALIS_S32((s32)info->color_diei, (s32)COLLIT_ALBUS);

        /* 25 Apr 2024 - St. Mark (Festum) */
        /* In 2024, Pascha = 31 Mar, ergo 25 Apr est post Octavam */
        /* Festum celebratur */
        dies = fasti_dies(MMXXIV, FASTI_APRILIS, XXV);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->celebratio_principalis != NIHIL);
        CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
            (s32)GRADUS_FESTUM);
        CREDO_AEQUALIS_S32((s32)info->color_diei, (s32)COLLIT_RUBER);

        /* 17 Apr 2025 - Holy Thursday (Triduum) */
        /* Triduum takes precedence over everything */
        dies = fasti_dies(MMXXV, FASTI_APRILIS, XVII);
        info = calendarium_obtinere_diem(cal, dies, piscina);
        CREDO_VERUM(info != NIHIL);
        CREDO_VERUM(info->celebratio_principalis != NIHIL);
        CREDO_AEQUALIS_S32((s32)info->celebratio_principalis->gradus,
            (s32)GRADUS_DIES_PECULIARIS);
    }


    /* Compendium */
    imprimere("\n");
    credo_imprimere_compendium();

    /* verdictum ANTE clausuram: sessio clausa = totalis ZEPHYRUM =
     * NIHIL PROBATUM */
    praeteritus = credo_omnia_praeterierunt();
    calendarium_destruere(cal);
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
