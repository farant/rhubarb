/* probatio_css_registrum.c - Registrum et lexicon CSS
 *
 * TRES CUSTODIAE, omnes contra id quod tabulae MANU SCRIPTAE
 * revera patiuntur:
 *
 *  1. ORDO LEXICI enumerationem CssLexemaGenus sequi DEBET, quia
 *     materia genera ut INDICES tractat. Assertio per TITULOS fit,
 *     non per numeros: permutatio ergo capitur, non absorbetur.
 *  2. OFFSETS LOCORUM CONTIGUI sunt. Tabula manu scripta hoc ipso
 *     loco errat - offset unus omissus omnia sequentia labi facit,
 *     et nodus formam ALIENAM tacite legeret.
 *  3. CSS munere LINEA CARET, et capacitas linea-sensitiva ergo
 *     RECUSATUR. Assertum POSITIVE, ne 'nemo id petivit' pro
 *     'recte agit' habeatur.
 */

#include "latina.h"
#include "credo.h"
#include "css_registrum.h"
#include "css_lexicon.h"
#include "css_lexema.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Nomina generum lexicalium CSS, ordine CssLexemaGenus. Fons
 * veritatis SECUNDUS consulto: si css_lexema.h permutetur, haec
 * lista et lexicon DISCREPABUNT et probatio cadet - quod est
 * officium eius. */
hic_manens constans character* ORDO_EXSPECTATUS[] = {
    "FINIS", "IDENTIFICATOR", "FUNCTIO", "APUD_VERBUM", "CANCELLUM",
    "CHORDA", "CHORDA_IMPERFECTA", "URL", "URL_IMPERFECTA", "NUMERUS",
    "CENTESIMA", "DIMENSIO", "SPATIA", "COMMENTARIUM",
    "COMMENTARIUM_IMPERFECTUM", "CDO", "CDC", "COLON", "SEMICOLON",
    "COMMA", "QUADRA_APERTA", "QUADRA_CLAUSA", "PAREN_APERTA",
    "PAREN_CLAUSA", "BRACE_APERTA", "BRACE_CLAUSA", "DELIM"
};

hic_manens constans character* GENERA_EXSPECTATA[] = {
    "plagula", "regula-qualificata", "regula-apud", "declaratio",
    "praevalentia", "lexema-servatum", "functio", "saeptum",
    "regula-mala", "declaratio-mala",
    /* B8: selectores (spec par. V) - APPENSA, numquam interposita */
    "selector-lista", "selector-complexus", "selector-compositus",
    "selector-typi", "selector-universalis", "selector-classis",
    "selector-identitatis", "selector-attributi", "pseudo-classis",
    "pseudo-elementum", "selector-malus"
};

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_css_registrum",
        32768);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: lexicon portam transit, et ORDINEM servat
     * ======================================================== */

    {
        i32 i;

        imprimere("\n--- Probans lexicon CSS ---\n");

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON,
            &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);

        /* Numerus generum lexico == numerus lexatoris */
        CREDO_AEQUALIS_I32 (CSS_LEXICON.numerus_generum,
            (i32)CSS_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)CSS_LEX_NUMERUS_GENERUM);

        /* ORDO per titulos - permutatio capitur */
        per (i = ZEPHYRUM; i < (i32)CSS_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(&ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
        }

        /* Sedes notae - erratum transcriptionis grossum caperent */
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)CSS_LEX_BRACE_APERTA), (s32)MATERIA_LEX_FIXUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)CSS_LEX_BRACE_APERTA), "{") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)CSS_LEX_CDO), "<!--") == ZEPHYRUM);
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)CSS_LEX_IDENTIFICATOR));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (s32)CSS_LEX_COLON));
    }


    /* ========================================================
     * PROBARE: CSS munere LINEA CARET - et id RECTUM est
     * ======================================================== */

    {
        i32 postulata;

        imprimere("\n--- Probans absentiam muneris LINEA ---\n");

        /* SPATIA CSS spatia+tabulas+lineas novas absorbet, ergo
         * VERBATIM/SPATIUM est, non REPETITUM/SPATIUM ut C89. */
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)CSS_LEX_SPATIA), (s32)MATERIA_LEX_VERBATIM);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (s32)CSS_LEX_SPATIA), (s32)MATERIA_MUNUS_SPATIUM);
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)CSS_LEX_SPATIA));

        /* Munera quae CSS FERT */
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_COMMENTUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));

        /* Et quae NON */
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LAMINA));

        /* Capacitas linea-sensitiva (inventor blocci commentorum
         * COMMENTUM + LINEA poscit) RECUSATUR. Politica B agit. */
        postulata = MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_COMMENTUM)
                  | MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_LINEA);
        CREDO_FALSUM (materia_lexicon_munera_habet(&ratum, postulata));

        /* Sed capacitas quae COMMENTUM solum poscit LICET */
        CREDO_VERUM (materia_lexicon_munera_habet(&ratum,
            MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_COMMENTUM)));
    }


    /* ========================================================
     * PROBARE: registrum - ordo et CONTIGUITAS locorum
     * ======================================================== */

    {
        i32 i;
        i32 exspectatus_offset;

        imprimere("\n--- Probans registrum nodorum ---\n");

        CREDO_AEQUALIS_I32 (CSS_REGISTRUM.numerus_generum,
            (i32)CSS_GENUS_NUMERUS);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])), (i32)CSS_GENUS_NUMERUS);

        /* Enumeratio et tabula congruunt, per TITULOS */
        per (i = ZEPHYRUM; i < (i32)CSS_GENUS_NUMERUS; i++)
        {
            CREDO_NON_NIHIL (CSS_REGISTRUM.genera[i].titulus);
            CREDO_VERUM (strcmp(CSS_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            /* et quaesitio inversa idem indicem reddit */
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(&CSS_REGISTRUM,
                GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
        }

        /* CONTIGUITAS: offset cuiusque == summa numerorum priorum.
         * Hic tabula manu scripta errat, et error TACITUS est -
         * nodus formam alienam legeret sine querela. */
        exspectatus_offset = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)CSS_GENUS_NUMERUS; i++)
        {
            CREDO_AEQUALIS_I32 (CSS_REGISTRUM.genera[i].loci_offset,
                exspectatus_offset);
            CREDO_MAIOR_I32 (CSS_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            exspectatus_offset += CSS_REGISTRUM.genera[i].loci_numerus;
        }
        /* et summa tabulam locorum EXACTE implet */
        CREDO_AEQUALIS_I32 (exspectatus_offset,
            CSS_REGISTRUM.numerus_locorum);

        /* Loci omnes titulum et speciem validam ferunt */
        per (i = ZEPHYRUM; i < CSS_REGISTRUM.numerus_locorum; i++)
        {
            CREDO_NON_NIHIL (CSS_REGISTRUM.loci[i].titulus);
            CREDO_MINOR_S32 (CSS_REGISTRUM.loci[i].species,
                (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM);
        }
    }


    /* ========================================================
     * PROBARE: circuitus per materiam (arbor CSS minima)
     * ======================================================== */

    {
        MateriaArborConsilium c;
        MateriaNodus*         plagula;
        MateriaNodus*         regula;
        MateriaNodus*         saeptum;
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
        MateriaNodus*         lecta;
        MateriaArborVitium    vitium;

        imprimere("\n--- Probans circuitum CSS per materiam ---\n");

        /* 'a{}' - arbor minima quae genera tria exercet */
        saeptum = materia_nodus_creare(piscina, (s32)CSS_GENUS_SAEPTUM,
            (i32)3);
        CREDO_VERUM (materia_nodus_ponere(saeptum, ZEPHYRUM,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)CSS_LEX_BRACE_APERTA,
                chorda_ex_literis("{", piscina), (s32)I, (i32)I,
                (i32)II,
                ZEPHYRUM)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(saeptum, (i32)II,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)CSS_LEX_BRACE_CLAUSA,
                chorda_ex_literis("}", piscina), (s32)II, (i32)I,
                (i32)III,
                ZEPHYRUM)), MATERIA_LOCUS_TOKEN));

        regula = materia_nodus_creare(piscina,
            (s32)CSS_GENUS_REGULA_QUALIFICATA, (i32)II);
        CREDO_VERUM (materia_nodus_appendere(piscina, regula, ZEPHYRUM,
            materia_valor_nodus(materia_nodus_creare(piscina,
                (s32)CSS_GENUS_LEXEMA_SERVATUM, (i32)I)),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(regula, (i32)I,
            materia_valor_nodus(saeptum), MATERIA_LOCUS_NODUS));

        /* praeludium: lexema 'a' in lexema-servatum */
        {
            MateriaValor* e = materia_valor_lista_obtinere(
                regula->loci[0], ZEPHYRUM);

            CREDO_NON_NIHIL (e);
            CREDO_VERUM (materia_nodus_ponere(e->datum.nodus, ZEPHYRUM,
                materia_valor_token(materia_token_creare(piscina,
                &FORMA,
                    (s32)CSS_LEX_IDENTIFICATOR,
                    chorda_ex_literis("a", piscina), ZEPHYRUM, (i32)I,
                    (i32)I, ZEPHYRUM)), MATERIA_LOCUS_TOKEN));
        }

        plagula = materia_nodus_creare(piscina, (s32)CSS_GENUS_PLAGULA,
            (i32)II);
        CREDO_VERUM (materia_nodus_appendere(piscina, plagula, ZEPHYRUM,
            materia_valor_nodus(regula), MATERIA_LOCUS_LISTA_NODUS));

        materia_arbor_consilium_nudum(&c, &CSS_REGISTRUM, &ratum,
            "css");
        s1 = materia_arbor_scribere_nodum(piscina, plagula, &c);
        CREDO_VERUM (s1.successus);
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum,
            "grammatica=\"css\""));

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        CREDO_AEQUALIS_S32 (lecta->genus, (s32)CSS_GENUS_PLAGULA);

        s2 = materia_arbor_scribere_nodum(piscina, lecta, &c);
        CREDO_VERUM (s2.successus);
        CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
        CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
            (size_t)s1.textus.mensura) == ZEPHYRUM);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
