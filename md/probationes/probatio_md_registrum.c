/* probatio_md_registrum.c - Registrum et lexicon markdown
 *
 * TRES CUSTODIAE contra id quod tabulae MANU SCRIPTAE patiuntur
 * (exemplar probatio_css_registrum):
 *
 *  1. ORDO LEXICI enumerationem MdLexGenus sequi DEBET - materia
 *     genera ut INDICES tractat. Assertio per TITULOS: permutatio
 *     capitur, non absorbetur.
 *  2. OFFSETS LOCORUM CONTIGUI. Offset unus omissus omnia sequentia
 *     labi facit et nodus formam ALIENAM tacite legeret.
 *  3. Markdown munus LINEA FERT (contra CSS): lector materiae lineas
 *     ex cursore reficit quem solum munus LINEA movet - ergo
 *     comparator FIDELIS hic licet. Assertum POSITIVE.
 *
 * Et circuitus minimus per materiam: documentum cum linea vacua,
 * scriptum -> lectum -> scriptum, octetim idem.
 */

#include "latina.h"
#include "credo.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Fons veritatis SECUNDUS consulto: si md_lexicon.h permutetur, haec
 * lista et lexicon DISCREPABUNT et probatio cadet. */
hic_manens constans character* ORDO_EXSPECTATUS[] = {
    "FINIS", "LINEA", "LINEA_CR",
    "INDENTATIO", "MARCA_CITATIONIS", "MARCA_PUNCTI", "MARCA_NUMERI",
    "MARCA_OFFICII", "MARCA_ATX", "CLAUSURA_ATX", "SUBDUCTIO",
        "SAEPTUM",
    "INFO", "DIVISIO", "PIPA",
    "TEXTUS", "DELIMITATOR", "GRAVIS", "EFFUGIUM", "ENS",
        "NEXUS_APERTUM",
    "NEXUS_CAUDA", "AUTONEXUS", "HTML", "SPATIA_FINALIA",
    "DERIVATUM"
};

hic_manens constans character* GENERA_EXSPECTATA[] = {
    "documentum", "praefatio", "linea", "linea-vacua", "paragraphus",
    "capitulum", "saeptum", "limes", "citatio", "lista", "elementum",
    "divisio", "tabula", "ordo", "cella", "html", "definitio-nexus",
    "inlinea", "textus", "emphasis", "fortis", "deletio", "verbatim",
    "nexus", "imago", "fractura-mollis", "fractura-dura",
        "html-inlineum"
};

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_md_registrum",
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

        imprimere("\n--- Probans lexicon markdown ---\n");

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &MD_LEXICON,
            &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);

        CREDO_AEQUALIS_I32 (MD_LEXICON.numerus_generum,
            (i32)MD_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)MD_LEX_NUMERUS_GENERUM);

        per (i = ZEPHYRUM; i < (i32)MD_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(&ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
        }

        /* Sedes notae */
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)MD_LEX_LINEA), (s32)MATERIA_LEX_TERMINATOR);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)MD_LEX_LINEA), "\n") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)MD_LEX_FINIS), (s32)MATERIA_LEX_FIXUM);
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)MD_LEX_TEXTUS));
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)MD_LEX_INDENTATIO));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (s32)MD_LEX_FINIS));
        CREDO_VERUM (strcmp(MD_LEXICON.praefixum_tagi, "md-")
            == ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: modellum SINE triviis - et munus LINEA ADEST
     * ======================================================== */

    {
        i32 i;
        i32 trivia;

        imprimere("\n--- Probans absentiam triviorum, praesentiam LINEAE ---\n");

        /* Nullum genus praeter LINEA/LINEA_CR trivium est: indentatio
         * et spatia finalia CONTENTUM sunt (spec par. III). */
        trivia = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)MD_LEX_NUMERUS_GENERUM; i++)
        {
            si (materia_lexicon_trivium_est(&ratum, (s32)i))
            {
                trivia = trivia + I;
            }
        }
        CREDO_AEQUALIS_I32 (trivia, (i32)II);
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)MD_LEX_LINEA));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)MD_LEX_INDENTATIO));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)MD_LEX_SPATIA_FINALIA));
        CREDO_AEQUALIS_S32 (MD_LEXICON.genus_spatii, (s32)-I);

        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_COMMENTUM));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LAMINA));
    }


    /* ========================================================
     * PROBARE: registrum - ordo et CONTIGUITAS locorum
     * ======================================================== */

    {
        i32 i;
        i32 exspectatus_offset;

        imprimere("\n--- Probans registrum nodorum ---\n");

        CREDO_AEQUALIS_I32 (MD_REGISTRUM.numerus_generum,
            (i32)MD_GENUS_NUMERUS);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])), (i32)MD_GENUS_NUMERUS);

        per (i = ZEPHYRUM; i < (i32)MD_GENUS_NUMERUS; i++)
        {
            CREDO_NON_NIHIL (MD_REGISTRUM.genera[i].titulus);
            CREDO_VERUM (strcmp(MD_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(&MD_REGISTRUM,
                GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
        }

        exspectatus_offset = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)MD_GENUS_NUMERUS; i++)
        {
            CREDO_AEQUALIS_I32 (MD_REGISTRUM.genera[i].loci_offset,
                exspectatus_offset);
            CREDO_MAIOR_I32 (MD_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            exspectatus_offset += MD_REGISTRUM.genera[i].loci_numerus;
        }
        CREDO_AEQUALIS_I32 (exspectatus_offset,
            MD_REGISTRUM.numerus_locorum);

        per (i = ZEPHYRUM; i < MD_REGISTRUM.numerus_locorum; i++)
        {
            CREDO_NON_NIHIL (MD_REGISTRUM.loci[i].titulus);
            CREDO_MINOR_S32 (MD_REGISTRUM.loci[i].species,
                (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM);
        }

        /* Enumerationes locorum nominatae contra TITULOS tabulae -
         * ne tertia tabula manu scripta per se labatur. Sedes notae
         * ex quaque familia. */
        CREDO_VERUM (strcmp(MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_CAPITULUM].loci_offset
            + (i32)MD_CAPITULUM_GRADUS].titulus, "gradus") == ZEPHYRUM);
        CREDO_VERUM (strcmp(MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_CAPITULUM].loci_offset
            + (i32)MD_CAPITULUM_SUBDUCTIO].titulus, "subductio")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_PARAGRAPHUS].loci_offset
            + (i32)MD_PARAGRAPHUS_NUDUS].titulus, "nudus") == ZEPHYRUM);
        CREDO_VERUM (strcmp(MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_LISTA].loci_offset
            + (i32)MD_LISTA_LAXA].titulus, "laxa") == ZEPHYRUM);
        CREDO_VERUM (strcmp(MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_NEXUS].loci_offset
            + (i32)MD_NEXUS_DESCRIPTIO].titulus, "descriptio")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_IMAGO].loci_offset
            + (i32)MD_NEXUS_DESCRIPTIO].titulus, "descriptio")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_FRACTURA_DURA].loci_offset
            + (i32)MD_DURA_PRAEFIXA].titulus, "praefixa") == ZEPHYRUM);
        CREDO_VERUM (strcmp(MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_DEFINITIO_NEXUS].loci_offset
            + (i32)MD_DEFINITIO_DESCRIPTIO].titulus, "descriptio")
            == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_TEXTUS].loci_offset
            + (i32)MD_TEXTUS_CRUDUM].species,
            (s32)MATERIA_LOCUS_LISTA_TOKEN);
        CREDO_AEQUALIS_S32 (MD_REGISTRUM.loci[
            MD_REGISTRUM.genera[MD_GENUS_SAEPTUM].loci_offset
            + (i32)MD_SAEPTUM_FORMA].species, (s32)MATERIA_LOCUS_INDEX);
    }


    /* ========================================================
     * PROBARE: circuitus per materiam (documentum cum linea vacua)
     * ======================================================== */

    {
        MateriaArborConsilium c;
        MateriaNodus*         documentum;
        MateriaNodus*         vacua;
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
        MateriaNodus*         lecta;
        MateriaArborVitium    vitium;

        imprimere("\n--- Probans circuitum md per materiam ---\n");

        /* "\n" - linea vacua una, deinde FINIS */
        vacua = materia_nodus_creare(piscina, (s32)MD_GENUS_LINEA_VACUA,
            (i32)II);
        CREDO_VERUM (materia_nodus_ponere(vacua, (i32)MD_VACUA_FINIS,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)MD_LEX_LINEA, chorda_ex_literis("\n", piscina),
                ZEPHYRUM, (i32)I, (i32)I, MD_FONS_PLAGULAE)),
            MATERIA_LOCUS_TOKEN));

        documentum = materia_nodus_creare(piscina,
            (s32)MD_GENUS_DOCUMENTUM, (i32)III);
        CREDO_VERUM (materia_nodus_appendere(piscina, documentum,
            (i32)MD_DOCUMENTUM_BLOCI, materia_valor_nodus(vacua),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(documentum,
            (i32)MD_DOCUMENTUM_FINIS,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)MD_LEX_FINIS, chorda_ex_literis("", piscina),
                (s32)I, (i32)II, (i32)I, MD_FONS_PLAGULAE)),
            MATERIA_LOCUS_TOKEN));

        materia_arbor_consilium_nudum(&c, &MD_REGISTRUM, &ratum, "md");
        s1 = materia_arbor_scribere_nodum(piscina, documentum, &c);
        CREDO_VERUM (s1.successus);
        si (!s1.successus)
        {
            imprimere("    (scriptura: %s)\n",
                s1.causa ? s1.causa : "-");
        }
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum,
            "grammatica=\"md\""));
        si (strstr((character*)s1.textus.datum, "<linea-vacua")
            == NIHIL)
        {
            imprimere("    documentum:\n%.*s\n",
                (integer)s1.textus.mensura,
                (character*)s1.textus.datum);
        }
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum,
            "<linea-vacua"));

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        si (lecta == NIHIL)
        {
            imprimere("    (lectio: %s, linea %d)\n",
                vitium.causa ? vitium.causa : "-",
                (integer)vitium.linea);
        }
        alioquin
        {
            CREDO_AEQUALIS_S32 (lecta->genus, (s32)MD_GENUS_DOCUMENTUM);
            s2 = materia_arbor_scribere_nodum(piscina, lecta, &c);
            CREDO_VERUM (s2.successus);
            CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
            CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
                (size_t)s1.textus.mensura) == ZEPHYRUM);
        }
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
