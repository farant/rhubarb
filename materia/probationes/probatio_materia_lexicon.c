/* probatio_materia_lexicon.c - Porta descriptoris lexicalis
 *
 * DOCTRINA: porta tacita = porta mortua. Ergo omne vitium quod
 * materia_lexicon_ratum_facere nominare potest hic PLANTATUR et
 * recusatio EXIGITUR. Lexicon sanum transiens nihil probat de
 * porta; solum recusatio probat eam vivere.
 */

#include "latina.h"
#include "credo.h"
#include "materia_lexicon.h"
#include "lexicon_c89.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Lexicon CSS-FORME: unum genus spatiorum quod lineas
 * novas ABSORBET (ut CSS_LEX_SPATIA verum), ergo nullum
 * genus munus LINEA fert. Vehiculum politicae degradationis.
 * ================================================== */

hic_manens constans MateriaLexGenus GENERA_CSSFORMIA[] = {
    { "FINIS",         "",    MATERIA_LEX_FIXUM,    MATERIA_MUNUS_FINIS },
    { "IDENTIFICATOR", NIHIL, MATERIA_LEX_VERBATIM, MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIA",        NIHIL, MATERIA_LEX_VERBATIM, MATERIA_MUNUS_SPATIUM },
    { "COMMENTARIUM",  NIHIL, MATERIA_LEX_VERBATIM, MATERIA_MUNUS_COMMENTUM },
    { "COLON",         ":",   MATERIA_LEX_FIXUM,    MATERIA_MUNUS_SUBSTANTIVUM }
};

hic_manens constans MateriaLexiconCoctum LEXICON_CSSFORME = {
    GENERA_CSSFORMIA,
    (i32)(magnitudo(GENERA_CSSFORMIA) / magnitudo(GENERA_CSSFORMIA[0])),
    "lex-",
    (s32)II   /* SPATIA */
};


/* Adiumentum: lexicon unius generis fabricare, ut vitium
 * plantatum SOLUM causa recusationis sit. */
hic_manens MateriaLexGenus GENUS_PRAVUM[1];
hic_manens MateriaLexiconCoctum LEXICON_PRAVUM;

hic_manens constans MateriaLexiconCoctum*
_pravum_fabricare (
    constans character* titulus,
    constans character* orthographia,
                   s32  species,
                   s32  munus,
                   s32  genus_spatii)
{
    GENUS_PRAVUM[0].titulus      = titulus;
    GENUS_PRAVUM[0].orthographia = orthographia;
    GENUS_PRAVUM[0].species      = species;
    GENUS_PRAVUM[0].munus        = munus;

    LEXICON_PRAVUM.genera          = GENUS_PRAVUM;
    LEXICON_PRAVUM.numerus_generum = I;
    LEXICON_PRAVUM.praefixum_tagi  = "lex-";
    LEXICON_PRAVUM.genus_spatii    = genus_spatii;

    redde &LEXICON_PRAVUM;
}


s32 principale (vacuum)
{
                 b32  praeteritus;
            Piscina*  piscina;
 MateriaLexiconRatum  ratum;
  MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_materia_lexicon", 4096);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: porta lexicon C89 ACCIPIT
     * ======================================================== */
    {
        imprimere("\n--- Probans portam: lexicon C89 sanum ---\n");

        iudicium.vitium = (s32)MATERIA_LEX_VITIUM_NIHIL;
        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON_C89,
                         &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);
        CREDO_VERUM (ratum.ratum);
        CREDO_AEQUALIS_I32 (ratum.lexicon->numerus_generum,
            (i32)LEXICON_C89_NUMERUS_GENERUM);
    }


    /* ========================================================
     * PROBARE: VITIA PLANTATA - porta singula RECUSAT
     * ======================================================== */
    {
        MateriaLexiconRatum r;
        MateriaLexIudicium  iud;

        imprimere("\n--- Probans vitia plantata (porta CLAMARE debet) ---\n");

        /* NIHIL */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r, NIHIL, &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_NIHIL);
        CREDO_FALSUM (r.ratum);

        /* numerus_generum ZEPHYRUM */
        {
            MateriaLexiconCoctum vacuum_lex;

            vacuum_lex.genera          = GENERA_CSSFORMIA;
            vacuum_lex.numerus_generum = ZEPHYRUM;
            vacuum_lex.praefixum_tagi  = "lex-";
            vacuum_lex.genus_spatii    = (s32)-I;
            CREDO_FALSUM (materia_lexicon_ratum_facere(&r, &vacuum_lex, &iud));
            CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_NUMERUS);
        }

        /* praefixum NIHIL */
        {
            MateriaLexiconCoctum sine_prae;

            sine_prae.genera          = GENERA_CSSFORMIA;
            sine_prae.numerus_generum = I;
            sine_prae.praefixum_tagi  = NIHIL;
            sine_prae.genus_spatii    = (s32)-I;
            CREDO_FALSUM (materia_lexicon_ratum_facere(&r, &sine_prae, &iud));
            CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_PRAEFIXUM);
        }

        /* titulus NIHIL */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare(NIHIL, NIHIL, (s32)MATERIA_LEX_VERBATIM,
                (s32)MATERIA_MUNUS_SUBSTANTIVUM, (s32)-I), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_TITULUS);

        /* titulus vacuus */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("", NIHIL, (s32)MATERIA_LEX_VERBATIM,
                (s32)MATERIA_MUNUS_SUBSTANTIVUM, (s32)-I), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_TITULUS);

        /* species extra fines */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("X", NIHIL,
                (s32)MATERIA_LEX_NUMERUS_SPECIERUM,
                (s32)MATERIA_MUNUS_SUBSTANTIVUM, (s32)-I), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_SPECIES);

        /* munus extra fines */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("X", NIHIL, (s32)MATERIA_LEX_VERBATIM,
                (s32)MATERIA_MUNUS_NUMERUS_MUNERUM, (s32)-I), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_MUNUS);

        /* INVARIANS, latus I: VERBATIM cum orthographia */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("X", "x", (s32)MATERIA_LEX_VERBATIM,
                (s32)MATERIA_MUNUS_SUBSTANTIVUM, (s32)-I), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_INVARIANS);

        /* INVARIANS, latus II: FIXUM sine orthographia */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("X", NIHIL, (s32)MATERIA_LEX_FIXUM,
                (s32)MATERIA_MUNUS_SUBSTANTIVUM, (s32)-I), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_INVARIANS);

        /* REPETITUM cuius unitas non character unus */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("X", "  ", (s32)MATERIA_LEX_REPETITUM,
                (s32)MATERIA_MUNUS_SPATIUM, (s32)-I), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_REPETITUM);

        /* TERMINATOR non in nova linea desinens */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("X", "\r", (s32)MATERIA_LEX_TERMINATOR,
                (s32)MATERIA_MUNUS_LINEA, (s32)-I), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_TERMINATOR);

        /* genus_spatii extra fines */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("X", NIHIL, (s32)MATERIA_LEX_VERBATIM,
                (s32)MATERIA_MUNUS_SUBSTANTIVUM, (s32)V), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_SPATIUM);

        /* genus_spatii ad genus cuius munus non SPATIUM est */
        CREDO_FALSUM (materia_lexicon_ratum_facere(&r,
            _pravum_fabricare("X", NIHIL, (s32)MATERIA_LEX_VERBATIM,
                (s32)MATERIA_MUNUS_SUBSTANTIVUM, ZEPHYRUM), &iud));
        CREDO_AEQUALIS_S32 (iud.vitium, (s32)MATERIA_LEX_VITIUM_SPATIUM);

        /* Nomen vitii numquam NIHIL, etiam extra fines */
        CREDO_NON_NIHIL (materia_lexicon_vitium_nomen(
            (MateriaLexVitium)999));
    }


    /* ========================================================
     * PROBARE: larva capacitatis (politica degradationis B)
     * ======================================================== */
    {
        MateriaLexiconRatum r_css;
        MateriaLexIudicium  iud;

        imprimere("\n--- Probans larvam capacitatis ---\n");

        /* C89 omnia sex munera fert */
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SUBSTANTIVUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LAMINA));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_COMMENTUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));

        /* Lexicon CSS-forme: spatia et commenta ADSUNT, LINEA DEEST */
        CREDO_VERUM (materia_lexicon_ratum_facere(&r_css,
            &LEXICON_CSSFORME, &iud));
        CREDO_VERUM (materia_lexicon_munus_habet(&r_css,
            MATERIA_MUNUS_SPATIUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&r_css,
            MATERIA_MUNUS_COMMENTUM));
        CREDO_FALSUM (materia_lexicon_munus_habet(&r_css,
            MATERIA_MUNUS_LINEA));
        CREDO_FALSUM (materia_lexicon_munus_habet(&r_css,
            MATERIA_MUNUS_LAMINA));

        /* Postulatum inventoris blocci commentorum: COMMENTUM + LINEA.
         * C89 praestat; CSS-forme NON - ergo recusatio ad tempus
         * ONERIS, non responsum falsum per vocationem. */
        {
            i32 postulata = MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_COMMENTUM)
                          | MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_LINEA);

            CREDO_VERUM  (materia_lexicon_munera_habet(&ratum, postulata));
            CREDO_FALSUM (materia_lexicon_munera_habet(&r_css, postulata));
        }

        /* Ratum non factum nihil praestat */
        {
            MateriaLexiconRatum r_nullum;

            r_nullum.lexicon           = NIHIL;
            r_nullum.munera_praesentia = ZEPHYRUM;
            r_nullum.ratum             = FALSUM;
            CREDO_FALSUM (materia_lexicon_munus_habet(&r_nullum,
                MATERIA_MUNUS_SPATIUM));
        }
    }


    /* ========================================================
     * PROBARE: accessus + distributio C89
     * ======================================================== */
    {
        i32 i;
        i32 numerus_verbatim = ZEPHYRUM;
        i32 numerus_fixum    = ZEPHYRUM;
        i32 numerus_repetitum= ZEPHYRUM;
        i32 numerus_terminator = ZEPHYRUM;

        imprimere("\n--- Probans accessus et distributionem C89 ---\n");

        /* Valores noti - erratum generationis grossum caperent */
        CREDO_VERUM (strcmp(materia_lexicon_titulus(&ratum,
            (i32)LEXICON_C89_GENUS_IDENTIFICATOR), "IDENTIFICATOR") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (i32)LEXICON_C89_GENUS_AUTO), "auto") == ZEPHYRUM);
        CREDO_NIHIL (materia_lexicon_orthographia(&ratum,
            (i32)LEXICON_C89_GENUS_IDENTIFICATOR));
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (i32)LEXICON_C89_GENUS_SPATIA), " ") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (i32)LEXICON_C89_GENUS_NOVA_LINEA), "\n") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (i32)LEXICON_C89_GENUS_CONTINUATIO), "\\\n") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (i32)LEXICON_C89_GENUS_EOF), "") == ZEPHYRUM);

        /* Munera propria */
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (i32)LEXICON_C89_GENUS_NOVA_LINEA), (s32)MATERIA_MUNUS_LINEA);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (i32)LEXICON_C89_GENUS_CONTINUATIO), (s32)MATERIA_MUNUS_LAMINA);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (i32)LEXICON_C89_GENUS_COMMENTUM_LINEA),
            (s32)MATERIA_MUNUS_COMMENTUM);

        /* textum_fert: VERBATIM sola. Commenta VERUM, spatia FALSUM -
         * id est ipsa divergentia a valor_portandus (quae utrisque
         * VERUM dabat) quam descriptor tollit. */
        CREDO_VERUM  (materia_lexicon_textum_fert(&ratum,
            (i32)LEXICON_C89_GENUS_COMMENTUM_CLAUSUM));
        CREDO_VERUM  (materia_lexicon_textum_fert(&ratum,
            (i32)LEXICON_C89_GENUS_IDENTIFICATOR));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (i32)LEXICON_C89_GENUS_SPATIA));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (i32)LEXICON_C89_GENUS_NOVA_LINEA));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (i32)LEXICON_C89_GENUS_AUTO));

        /* trivium_est: commenta ET spatia, non identificator */
        CREDO_VERUM  (materia_lexicon_trivium_est(&ratum,
            (i32)LEXICON_C89_GENUS_COMMENTUM_CLAUSUM));
        CREDO_VERUM  (materia_lexicon_trivium_est(&ratum,
            (i32)LEXICON_C89_GENUS_SPATIA));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (i32)LEXICON_C89_GENUS_IDENTIFICATOR));

        /* Genus extra fines: nihil, non ruina */
        CREDO_NIHIL (materia_lexicon_titulus(&ratum,
            (i32)LEXICON_C89_NUMERUS_GENERUM));
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (i32)LEXICON_C89_NUMERUS_GENERUM),
            (s32)MATERIA_LEX_NUMERUS_SPECIERUM);

        /* DISTRIBUTIO - mensurata 2026-08-27 ante furcam.
         * Numeri pinnati: generatio quae eos movet AUDIRI debet. */
        per (i = ZEPHYRUM; i < (i32)LEXICON_C89_NUMERUS_GENERUM; i++)
        {
            commutatio ((s32)materia_lexicon_species(&ratum, i))
            {
            casus (s32)MATERIA_LEX_VERBATIM:   numerus_verbatim++;   frange;
            casus (s32)MATERIA_LEX_FIXUM:      numerus_fixum++;      frange;
            casus (s32)MATERIA_LEX_REPETITUM:  numerus_repetitum++;  frange;
            casus (s32)MATERIA_LEX_TERMINATOR: numerus_terminator++; frange;
            ordinarius: frange;
            }
        }
        CREDO_AEQUALIS_I32 (numerus_verbatim,   (i32)10);
        CREDO_AEQUALIS_I32 (numerus_fixum,      (i32)81);
        CREDO_AEQUALIS_I32 (numerus_repetitum,  (i32)2);
        CREDO_AEQUALIS_I32 (numerus_terminator, (i32)2);
        CREDO_AEQUALIS_I32 (numerus_verbatim + numerus_fixum
                          + numerus_repetitum + numerus_terminator,
                            (i32)LEXICON_C89_NUMERUS_GENERUM);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
