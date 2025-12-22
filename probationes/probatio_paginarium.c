/* probatio_paginarium.c - Probationes Paginarii */
#include "latina.h"
#include "paginarium.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("test_paginarium", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * Probare paginarium_config_defectus
     * ================================================== */

    {
        PaginariumConfig config;

        imprimere("\n--- Probans paginarium_config_defectus ---\n");

        config = paginarium_config_defectus();

        CREDO_AEQUALIS_I32(config.latitudo, LXXX);
        CREDO_AEQUALIS_I32(config.altitudo, XXV);
        CREDO_AEQUALIS_I32(config.limina_versus, LXXX);
        CREDO_AEQUALIS_I32(config.lineae_versus_probare, V);
    }


    /* ==================================================
     * Probare textus vacuus
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        chorda             textus;

        imprimere("\n--- Probans textus_vacuus ---\n");

        config = paginarium_config_defectus();
        config.latitudo = XL;
        config.altitudo = X;

        textus.datum = NIHIL;
        textus.mensura = ZEPHYRUM;

        resultus = paginarium_paginare(textus, config, piscina);

        CREDO_AEQUALIS_I32(resultus.numerus_paginarum, ZEPHYRUM);
    }


    /* ==================================================
     * Probare prosa simplex - una linea
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;

        imprimere("\n--- Probans prosa_simplex ---\n");

        config = paginarium_config_defectus();
        config.latitudo = L;
        config.altitudo = X;

        textus = chorda_ex_literis("Haec est linea simplex.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        CREDO_AEQUALIS_I32(resultus.numerus_paginarum, I);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);
        CREDO_AEQUALIS_I32(paginarium_pagina_numerus_linearum(pagina), I);

        linea = paginarium_linea_obtinere(pagina, ZEPHYRUM);
        CREDO_NON_NIHIL(linea);
        CREDO_FALSUM(linea->est_vacua);
        CREDO_FALSUM(linea->modus_versus);

        reddita = paginarium_linea_reddere(textus, linea, piscina);
        CREDO_CHORDA_AEQUALIS(reddita, textus);
    }


    /* ==================================================
     * Probare prosa cum involutione (word wrap)
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;

        imprimere("\n--- Probans prosa_involutio ---\n");

        config = paginarium_config_defectus();
        config.latitudo = XX;
        config.altitudo = X;

        /* Textus longior quam latitudo - debet involvi */
        textus = chorda_ex_literis("Haec est linea longa quae debet involvi.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        CREDO_AEQUALIS_I32(resultus.numerus_paginarum, I);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Debet habere plures lineas */
        CREDO_VERUM(paginarium_pagina_numerus_linearum(pagina) > I);

        /* Prima linea */
        linea = paginarium_linea_obtinere(pagina, ZEPHYRUM);
        CREDO_NON_NIHIL(linea);
        CREDO_FALSUM(linea->est_vacua);

        reddita = paginarium_linea_reddere(textus, linea, piscina);
        /* Verificare quod linea non excedit latitudinem */
        CREDO_VERUM(reddita.mensura <= config.latitudo);
    }


    /* ==================================================
     * Probare prosa cum \n singulum -> spatium
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;

        imprimere("\n--- Probans prosa_newline_singulum ---\n");

        config = paginarium_config_defectus();
        config.latitudo = L;
        config.altitudo = X;

        /* Textus cum newline singulum - debet convertere ad spatium */
        /* Lineae longae ut non detegatur ut versus */
        textus = chorda_ex_literis(
            "Haec est linea prima quae est satis longa ut prosa sit.\n"
            "Et haec est linea secunda quae etiam est satis longa.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* In modo prosa, newline singulum = spatium, ergo una linea */
        linea = paginarium_linea_obtinere(pagina, ZEPHYRUM);
        CREDO_NON_NIHIL(linea);
        CREDO_FALSUM(linea->modus_versus);

        reddita = paginarium_linea_reddere(textus, linea, piscina);
        /* Newline debet esse spatium */
        CREDO_VERUM(reddita.mensura > ZEPHYRUM);
    }


    /* ==================================================
     * Probare versus purus (lineae breves)
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;

        imprimere("\n--- Probans versus_purus ---\n");

        config = paginarium_config_defectus();
        config.latitudo = L;
        config.altitudo = XX;

        /* Versus: lineae breves (< 80% de latitudine) */
        textus = chorda_ex_literis(
            "Rosa\n"
            "Alba\n"
            "In horto\n"
            "Floret\n"
            "Pulchra", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        CREDO_AEQUALIS_I32(resultus.numerus_paginarum, I);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Debet esse in modo versus */
        linea = paginarium_linea_obtinere(pagina, ZEPHYRUM);
        CREDO_NON_NIHIL(linea);
        CREDO_VERUM(linea->modus_versus);

        /* Debet habere V lineas (una per versum) */
        CREDO_AEQUALIS_I32(paginarium_pagina_numerus_linearum(pagina), V);
    }


    /* ==================================================
     * Probare paragraph break (2+ newlines)
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        i32                i;
        i32                numerus_vacuarum;

        imprimere("\n--- Probans paragraph_break ---\n");

        config = paginarium_config_defectus();
        config.latitudo = LX;
        config.altitudo = XX;

        /* Duo paragraphi separati per duas newlines */
        /* Lineae longae ut prosa detegatur */
        textus = chorda_ex_literis(
            "Haec est linea prima paragraphi primi quae est satis longa.\n"
            "Et secunda linea paragraphi primi etiam est satis longa.\n\n"
            "Nunc incipit paragraphus secundus qui etiam habet lineas longas.\n"
            "Et ultima linea paragraphi secundi est satis longa.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Debet habere lineam vacuam inter paragraphos */
        numerus_vacuarum = ZEPHYRUM;
        per (i = ZEPHYRUM; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && linea->est_vacua)
            {
                numerus_vacuarum++;
            }
        }
        CREDO_VERUM(numerus_vacuarum >= I);
    }


    /* ==================================================
     * Probare mixed prosa/versus per paragraphum
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        i32                i;
        b32                invenit_prosa;
        b32                invenit_versus;

        imprimere("\n--- Probans mixed_prosa_versus ---\n");

        config = paginarium_config_defectus();
        config.latitudo = L;
        config.altitudo = L;

        /* Prosa, deinde versus, deinde prosa */
        /* Prosa lineae debent esse >= 80% latitudinis (40 chars) */
        textus = chorda_ex_literis(
            "Haec est prima linea paragraphi quae est satis longa.\n"
            "Et secunda linea paragraphi quae etiam est satis longa.\n\n"
            "Brevis\n"
            "Versus\n"
            "Hic\n"
            "Est\n"
            "Poesis\n\n"
            "Haec est linea ultima paragraphi quae est satis longa.\n"
            "Et alia linea ultima paragraphi quae etiam est longa.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Verificare quod habemus et prosa et versus */
        invenit_prosa = FALSUM;
        invenit_versus = FALSUM;

        per (i = ZEPHYRUM; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && !linea->est_vacua)
            {
                si (linea->modus_versus)
                {
                    invenit_versus = VERUM;
                }
                alioquin
                {
                    invenit_prosa = VERUM;
                }
            }
        }

        CREDO_VERUM(invenit_prosa);
        CREDO_VERUM(invenit_versus);
    }


    /* ==================================================
     * Probare multiplex paginas
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        chorda             textus;

        imprimere("\n--- Probans multiplex_paginas ---\n");

        config = paginarium_config_defectus();
        config.latitudo = XX;
        config.altitudo = III;  /* Tres lineae per paginam */

        /* Textus quod excedit unam paginam */
        textus = chorda_ex_literis(
            "Linea una.\n\n"
            "Linea duo.\n\n"
            "Linea tres.\n\n"
            "Linea quattuor.\n\n"
            "Linea quinque.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        /* Debet habere plures paginas */
        CREDO_VERUM(resultus.numerus_paginarum > I);

        /* Verificare primam paginam */
        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);
        CREDO_VERUM(paginarium_pagina_numerus_linearum(pagina) <= config.altitudo);

        /* Verificare secundam paginam */
        pagina = paginarium_pagina_obtinere(&resultus, I);
        CREDO_NON_NIHIL(pagina);
        CREDO_VERUM(paginarium_pagina_numerus_linearum(pagina) <= config.altitudo);
    }


    /* ==================================================
     * Probare linea reddere - versus preservat newlines
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;
        chorda             speratus;

        imprimere("\n--- Probans linea_reddere_versus ---\n");

        config = paginarium_config_defectus();
        config.latitudo = L;
        config.altitudo = X;

        /* Versus simplex */
        textus = chorda_ex_literis(
            "Rosa\n"
            "Alba\n"
            "Pulchra", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Prima linea debet esse "Rosa" */
        linea = paginarium_linea_obtinere(pagina, ZEPHYRUM);
        CREDO_NON_NIHIL(linea);

        reddita = paginarium_linea_reddere(textus, linea, piscina);
        speratus = chorda_ex_literis("Rosa", piscina);
        CREDO_CHORDA_AEQUALIS(reddita, speratus);
    }


    /* ==================================================
     * Probare linea vacua reddere
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;
        i32                i;

        imprimere("\n--- Probans linea_vacua_reddere ---\n");

        config = paginarium_config_defectus();
        config.latitudo = LX;
        config.altitudo = X;

        /* Lineae longae ut prosa detegatur */
        textus = chorda_ex_literis(
            "Haec est prima linea quae est satis longa pro prosa.\n"
            "Et secunda linea quae etiam est satis longa.\n\n"
            "Haec est tertia linea quae incipit novum paragraphum.\n"
            "Et quarta linea quae etiam est satis longa.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Invenire lineam vacuam */
        per (i = ZEPHYRUM; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && linea->est_vacua)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                /* Linea vacua debet reddere chordam vacuam */
                CREDO_AEQUALIS_I32(reddita.mensura, ZEPHYRUM);
                frange;
            }
        }
    }


    /* ==================================================
     * Probare obtinere index invalidus
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;

        imprimere("\n--- Probans index_invalidus ---\n");

        config = paginarium_config_defectus();
        textus = chorda_ex_literis("Textus.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        /* Index extra limites */
        pagina = paginarium_pagina_obtinere(&resultus, C);
        CREDO_NIHIL(pagina);

        /* Linea index invalidus */
        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        linea = paginarium_linea_obtinere(pagina, C);
        CREDO_NIHIL(linea);
    }


    /* ==================================================
     * Probare word wrap in versus modo
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;
        i32                i;

        imprimere("\n--- Probans versus_word_wrap ---\n");

        config = paginarium_config_defectus();
        config.latitudo = XV;  /* Latitudo parva */
        config.altitudo = XX;

        /* Versus cum linea longa quae excedit latitudinem */
        textus = chorda_ex_literis(
            "Brevis\n"
            "Haec linea est nimis longa pro versus\n"
            "Alia", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Verificare quod omnes lineae non excedunt latitudinem */
        per (i = ZEPHYRUM; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && !linea->est_vacua)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                CREDO_VERUM(reddita.mensura <= config.latitudo);
            }
        }
    }


    /* ==================================================
     * Probare Pindar (Gutenberg 10717) - textus realis
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;
        i32                i;

        imprimere("\n--- Probans pindar_textus ---\n");

        config = paginarium_config_defectus();
        config.latitudo = LX;   /* 60 characteres - realisticus pro widget */
        config.altitudo = XXX;

        /* Exemplum ex Pindar: titulus + introductio prosa
         * Titulus: lineae breves (versus modus)
         * Introductio: lineae longae hard-wrapped (prosa modus)
         */
        textus = chorda_ex_literis(
            "*** START OF THE PROJECT GUTENBERG EBOOK 10717 ***\n"
            "\n"
            "THE EXTANT\n"
            "\n"
            "ODES OF PINDAR\n"
            "\n"
            "\n"
            "INTRODUCTION.\n"
            "\n"
            "\n"
            "Probably no poet of importance equal or approaching to that of Pindar\n"
            "finds so few and so infrequent readers. The causes are not far to\n"
            "seek: in the first and most obvious place comes the great difficulty\n"
            "of his language.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        CREDO_VERUM(resultus.numerus_paginarum >= I);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Imprimere lineas pro depuratione */
        imprimere("  Numerus linearum: %d\n", paginarium_pagina_numerus_linearum(pagina));

        per (i = 0; i < paginarium_pagina_numerus_linearum(pagina) && i < XX; i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                si (linea->est_vacua)
                {
                    imprimere("  [%d] <vacua> versus=%d\n", i, linea->modus_versus);
                }
                alioquin
                {
                    imprimere("  [%d] len=%d versus=%d: \"%.40s%s\"\n",
                        i, reddita.mensura, linea->modus_versus,
                        reddita.datum,
                        reddita.mensura > XL ? "..." : "");
                }
            }
        }

        /* Verificare quod prosa lineae non excedunt latitudinem */
        per (i = 0; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && !linea->est_vacua)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                CREDO_VERUM(reddita.mensura <= config.latitudo);
            }
        }
    }


    /* ==================================================
     * Probare tres newlines -> una linea vacua
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        i32                i;
        i32                numerus_vacuarum;

        imprimere("\n--- Probans tres_newlines ---\n");

        config = paginarium_config_defectus();
        config.latitudo = LX;
        config.altitudo = XX;

        /* Tres newlines debent collapsare ad unam lineam vacuam */
        /* Lineae longae ut prosa detegatur */
        textus = chorda_ex_literis(
            "Haec est prima linea quae est satis longa pro prosa.\n"
            "Et secunda linea quae etiam est satis longa.\n\n\n"
            "Haec est tertia linea post tres newlines.\n"
            "Et quarta linea quae etiam est satis longa.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Numerare lineas vacuas - debet esse una */
        numerus_vacuarum = ZEPHYRUM;
        per (i = ZEPHYRUM; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && linea->est_vacua)
            {
                numerus_vacuarum++;
            }
        }
        CREDO_AEQUALIS_I32(numerus_vacuarum, I);
    }


    /* ==================================================
     * Probare pindar plenum - cum poesia ante prosa
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;
        i32                i;
        i32                num_lineae;

        imprimere("\n--- Probans pindar_plenum_67 ---\n");

        config = paginarium_config_defectus();
        config.latitudo = LXVII;   /* 67 characteres */
        config.altitudo = LX;      /* 60 lineae - omnia in una pagina */

        /* Structura vera: tituli + poesia + INTRODUCTION + prosa */
        textus = chorda_ex_literis(
            "*** START OF THE PROJECT GUTENBERG EBOOK 10717 ***\n"
            "\n"
            "THE EXTANT\n"
            "\n"
            "ODES OF PINDAR\n"
            "\n"
            "_First edition printed 1874._\n"
            "\n"
            "  SON OF THE LIGHTNING, FAIR AND FIERY STAR,\n"
            "  STRONG-WINGED IMPERIAL PINDAR, VOICE DIVINE,\n"
            "  LET THESE DEEP DRAUGHTS OF THY ENCHANTED WINE\n"
            "  LIFT ME WITH THEE IN SOARINGS HIGH AND FAR\n"
            "  PROUDER THAN PEGASEAN, OR THE CAR\n"
            "  WHEREIN APOLLO RAPT THE HUNTRESS MAID.\n"
            "  SO LET ME RANGE MINE HOUR, TOO SOON TO FADE\n"
            "  INTO STRANGE PRESENCE OF THE THINGS THAT ARE.\n"
            "\n"
            "\n"
            "\n"
            "\n"
            "INTRODUCTION.\n"
            "\n"
            "\n"
            "Probably no poet of importance equal or approaching to that of Pindar\n"
            "finds so few and so infrequent readers. The causes are not far to\n"
            "seek: in the first and most obvious place comes the great difficulty\n"
            "of his language, in the second the frequent obscurity of his thought,\n"
            "resulting mainly from his exceeding allusiveness and his abrupt\n"
            "transitions.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        imprimere("  Config: latitudo=%d, 80%% limina=%d\n",
            config.latitudo, (config.latitudo * LXXX) / C);

        num_lineae = paginarium_pagina_numerus_linearum(pagina);
        imprimere("  Numerus linearum: %d\n", num_lineae);

        per (i = 0; i < num_lineae; i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                si (linea->est_vacua)
                {
                    imprimere("  [%d] <vacua>\n", i);
                }
                alioquin
                {
                    imprimere("  [%d] len=%d v=%d: \"%.*s\"\n",
                        i, reddita.mensura, linea->modus_versus,
                        reddita.mensura > XLV ? XLV : reddita.mensura,
                        reddita.datum);
                }
            }
        }

        /* Verificare: post INTRODUCTION prosa debet esse versus=0 */
        /* Quaerere "Probably" */
        per (i = 0; i < num_lineae; i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && !linea->est_vacua)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                si (reddita.mensura >= VIII)
                {
                    /* Comparare primos VIII characteres */
                    si (reddita.datum[0] == 'P' &&
                        reddita.datum[1] == 'r' &&
                        reddita.datum[2] == 'o' &&
                        reddita.datum[3] == 'b')
                    {
                        imprimere("  -> Prosa incipit [%d] versus=%d\n",
                            i, linea->modus_versus);
                        CREDO_VERUM(linea->modus_versus == FALSUM);
                    }
                }
            }
        }
    }


    /* ==================================================
     * Probare prosa gutenberg - lat 67, isolata
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;
        i32                i;

        imprimere("\n--- Probans prosa_gutenberg_67 ---\n");

        config = paginarium_config_defectus();
        config.latitudo = LXVII;   /* 67 characteres - exacte ut widget */
        config.altitudo = XXX;

        /* Prosa pura sine titulis - debet esse prosa modus */
        /* Lineae Gutenberg sunt ~70 characteres, > 80% de 67 = 53.6 */
        textus = chorda_ex_literis(
            "Probably no poet of importance equal or approaching to that of Pindar\n"
            "finds so few and so infrequent readers. The causes are not far to\n"
            "seek: in the first and most obvious place comes the great difficulty\n"
            "of his language, in the second the frequent obscurity of his thought,\n"
            "resulting mainly from his exceeding allusiveness and his abrupt\n"
            "transitions.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        CREDO_VERUM(resultus.numerus_paginarum >= I);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        /* Imprimere lineas */
        imprimere("  Config: latitudo=%d, limina_versus=%d%%\n",
            config.latitudo, config.limina_versus);
        imprimere("  Numerus linearum: %d\n", paginarium_pagina_numerus_linearum(pagina));

        per (i = 0; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                imprimere("  [%d] len=%d versus=%d: \"%.*s\"\n",
                    i, reddita.mensura, linea->modus_versus,
                    reddita.mensura, reddita.datum);
            }
        }

        /* Verificare: prima linea debet esse prosa (versus=0) */
        linea = paginarium_linea_obtinere(pagina, ZEPHYRUM);
        CREDO_NON_NIHIL(linea);
        CREDO_VERUM(linea->modus_versus == FALSUM);  /* Prosa modus */

        /* Verificare: nulla linea habet "Pindar" solum */
        per (i = 0; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && !linea->est_vacua)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                /* Si linea est "Pindar" solum, est problema */
                si (reddita.mensura == VI)
                {
                    /* Potentialiter orphanum - imprimere */
                    imprimere("  MONITUM: linea brevis [%d]: \"%.*s\"\n",
                        i, reddita.mensura, reddita.datum);
                }
            }
        }
    }


    /* ==================================================
     * Probare CRLF line endings (Windows)
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        chorda             reddita;
        i32                i;

        imprimere("\n--- Probans crlf_line_endings ---\n");

        config = paginarium_config_defectus();
        config.latitudo = LXVII;
        config.altitudo = XXX;

        /* CRLF line endings - prosa cum \r\n */
        textus = chorda_ex_literis(
            "Probably no poet of importance equal or approaching to that of Pindar\r\n"
            "finds so few and so infrequent readers. The causes are not far to\r\n"
            "seek: in the first and most obvious place comes the great difficulty\r\n"
            "of his language.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        imprimere("  Numerus linearum: %d\n", paginarium_pagina_numerus_linearum(pagina));

        per (i = 0; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea && !linea->est_vacua)
            {
                reddita = paginarium_linea_reddere(textus, linea, piscina);
                imprimere("  [%d] len=%d v=%d: \"%.*s\"\n",
                    i, reddita.mensura, linea->modus_versus,
                    reddita.mensura > XLV ? XLV : reddita.mensura,
                    reddita.datum);
            }
        }

        /* Verificare: debet esse prosa (versus=0) */
        linea = paginarium_linea_obtinere(pagina, ZEPHYRUM);
        CREDO_NON_NIHIL(linea);
        CREDO_VERUM(linea->modus_versus == FALSUM);
    }


    /* ==================================================
     * Probare CRLF paragraph breaks
     * ================================================== */

    {
        PaginariumConfig   config;
        PaginariumResultus resultus;
        PaginariumPagina*  pagina;
        PaginariumLinea*   linea;
        chorda             textus;
        i32                numerus_vacuarum;
        i32                i;

        imprimere("\n--- Probans crlf_paragraph_break ---\n");

        config = paginarium_config_defectus();
        config.latitudo = LXVII;
        config.altitudo = XXX;

        /* CRLF paragraph break: \r\n\r\n */
        textus = chorda_ex_literis(
            "First paragraph with some long text that should flow nicely here.\r\n"
            "\r\n"
            "Second paragraph after the break.", piscina);

        resultus = paginarium_paginare(textus, config, piscina);

        pagina = paginarium_pagina_obtinere(&resultus, ZEPHYRUM);
        CREDO_NON_NIHIL(pagina);

        imprimere("  Numerus linearum: %d\n", paginarium_pagina_numerus_linearum(pagina));

        /* Numerare lineas vacuas - debet esse una */
        numerus_vacuarum = ZEPHYRUM;
        per (i = ZEPHYRUM; i < paginarium_pagina_numerus_linearum(pagina); i++)
        {
            linea = paginarium_linea_obtinere(pagina, i);
            si (linea)
            {
                si (linea->est_vacua)
                {
                    imprimere("  [%d] <vacua>\n", i);
                    numerus_vacuarum++;
                }
                alioquin
                {
                    chorda reddita = paginarium_linea_reddere(textus, linea, piscina);
                    imprimere("  [%d] len=%d: \"%.*s\"\n",
                        i, reddita.mensura, reddita.mensura, reddita.datum);
                }
            }
        }

        CREDO_VERUM(numerus_vacuarum >= I);
    }


    /* =================================================
     * Compendium
     * ================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    /* Destructio */
    credo_claudere();
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
