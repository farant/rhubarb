/* probatio_oratio_registrum.c - Registrum et lexicon orationis (T1)
 *
 * TRES CUSTODIAE contra id quod tabulae MANU SCRIPTAE patiuntur
 * (exemplar probatio_md_registrum):
 *  1. ORDO LEXICI enumerationem OratioLexGenus sequi DEBET (per
 *     TITULOS: permutatio capitur).
 *  2. OFFSETS LOCORUM CONTIGUI (offset omissus formam alienam legeret).
 *  3. Modellum SINE triviis cum munere LINEA (comparator FIDELIS licet):
 *     spatium CONTENTUM est, non trivium.
 * Et circuitus minimus per materiam: "a\n" - vocabulum cum cauda,
 * scriptum -> lectum -> scriptum, octetim idem. Culpa plantata
 * nativitatis: numerus locorum generis unius mutatus (contiguitas).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_registrum.h"
#include "oratio_lexicon.h"
#include "oratio_lexema.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

hic_manens constans character* ORDO_EXSPECTATUS[] = {
    "FINIS", "LINEA", "LINEA_CR", "SPATIUM",
    "LITTERAE", "DIGITI", "HYPHEN", "APOSTROPHUS", "PUNCTUM",
    "INTERPUNCTIO", "SIGNUM",
    "DERIVATUM"
};

hic_manens constans character* GENERA_EXSPECTATA[] = {
    "documentum", "paragraphus", "sententia", "vocabulum",
    "interpunctio", "numerus",
    /* gradus III (T11): unum per classem universalem, ordine UD */
    "analysis-substantivi", "analysis-nominis-proprii",
        "analysis-verbi",
    "analysis-auxiliaris", "analysis-adiectivi", "analysis-adverbii",
    "analysis-pronominis", "analysis-determinantis",
    "analysis-adpositionis", "analysis-numeralis",
    "analysis-coniunctionis-coordinantis",
    "analysis-coniunctionis-subordinantis", "analysis-particulae",
    "analysis-interiectionis", "analysis-symboli",
    "analysis-interpunctionis", "analysis-ignoti"
};

/* plagulam legere (canon glossarii: custos classium) */
interior b32
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                chorda* exitus)
{
          FILE* f = fopen(via, "rb");
        longus  longitudo;
            i8* memoria;

    exitus->datum    = NIHIL;
    exitus->mensura  = ZEPHYRUM;
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde FALSUM;
    }
    memoria = (i8*)piscina_allocare(piscina, (memoriae_index)longitudo
        + I);
    si (   memoria                                 == NIHIL
        || fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    memoria[longitudo]  = ZEPHYRUM;
    exitus->datum       = memoria;
    exitus->mensura     = (i32)longitudo;
    redde VERUM;
}

/* titulus loci j-ti generis g */
interior constans character*
_locus (
    OratioGenus g,
            i32 j)
{
    redde ORATIO_REGISTRUM.loci[ORATIO_REGISTRUM.genera[g].loci_offset
        + j].titulus;
}

interior s32
_species (
    OratioGenus g,
            i32 j)
{
    redde ORATIO_REGISTRUM.loci[ORATIO_REGISTRUM.genera[g].loci_offset
        + j].species;
}

interior MateriaToken*
_lexema (
               Piscina* piscina,
                   s32  genus,
    constans character* textus,
                   s32  offset,
                   i32  linea,
                   i32  columna)
{
    redde materia_token_creare(piscina, &FORMA, genus,
        chorda_ex_literis(textus, piscina), offset, linea, columna,
        ORATIO_FONS_PLAGULAE);
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_oratio_registrum",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    {
        i32 i;
        i32 trivia = ZEPHYRUM;

        imprimere("\n--- Probans lexicon orationis ---\n");
        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum,
            &ORATIO_LEXICON,
            &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);
        CREDO_AEQUALIS_I32 (ORATIO_LEXICON.numerus_generum,
            (i32)ORATIO_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)ORATIO_LEX_NUMERUS_GENERUM);
        per (i = ZEPHYRUM; i < (i32)ORATIO_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(&ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
            si (materia_lexicon_trivium_est(&ratum, (s32)i))
            {
                trivia = trivia + I;
            }
        }
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)ORATIO_LEX_LINEA), (s32)MATERIA_LEX_TERMINATOR);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)ORATIO_LEX_FINIS), (s32)MATERIA_LEX_FIXUM);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)ORATIO_LEX_PUNCTUM), (s32)MATERIA_LEX_FIXUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)ORATIO_LEX_HYPHEN), "-") == ZEPHYRUM);
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)ORATIO_LEX_LITTERAE));
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)ORATIO_LEX_SPATIUM));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (s32)ORATIO_LEX_FINIS));
        CREDO_VERUM (strcmp(ORATIO_LEXICON.praefixum_tagi, "or-")
            == ZEPHYRUM);
        /* trivia: LINEA et LINEA_CR solae (munus LINEA); SPATIUM contentum */
        CREDO_AEQUALIS_I32 (trivia, (i32)II);
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)ORATIO_LEX_SPATIUM));
        CREDO_AEQUALIS_S32 (ORATIO_LEXICON.genus_spatii, (s32)-I);
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_COMMENTUM));
    }

    {
        i32 i;
        i32 exspectatus_offset = ZEPHYRUM;

        imprimere("\n--- Probans registrum nodorum ---\n");
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.numerus_generum,
            (i32)ORATIO_GENUS_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])),
            (i32)ORATIO_GENUS_NUMERUS_GENERUM);
        per (i = ZEPHYRUM; i < (i32)ORATIO_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_NON_NIHIL (ORATIO_REGISTRUM.genera[i].titulus);
            CREDO_VERUM (strcmp(ORATIO_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(
                &ORATIO_REGISTRUM, GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
            CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[i].loci_offset,
                exspectatus_offset);
            CREDO_MAIOR_I32 (ORATIO_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            exspectatus_offset += ORATIO_REGISTRUM.genera[i].loci_numerus;
        }
        CREDO_AEQUALIS_I32 (exspectatus_offset,
            ORATIO_REGISTRUM.numerus_locorum);
        per (i = ZEPHYRUM; i < ORATIO_REGISTRUM.numerus_locorum; i++)
        {
            CREDO_NON_NIHIL (ORATIO_REGISTRUM.loci[i].titulus);
            CREDO_MINOR_S32 (ORATIO_REGISTRUM.loci[i].species,
                (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM);
        }
        /* enumerationes locorum contra titulos tabulae */
        CREDO_VERUM (strcmp(ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_VOCABULUM].loci_offset
            + (i32)ORATIO_VOCABULUM_CAUDA].titulus, "cauda")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_VOCABULUM].loci_offset
            + (i32)ORATIO_VOCABULUM_LINGUAE].titulus, "linguae")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_PARAGRAPHUS].loci_offset
            + (i32)ORATIO_PARAGRAPHUS_CAUDA].titulus, "cauda")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_NUMERUS].loci_offset
            + (i32)ORATIO_NUMERUS_CRUDUM].titulus, "crudum")
            == ZEPHYRUM);
        /* T6b: forma APPENSA post caudam, INDEX (lex deprimendi) */
        CREDO_VERUM (strcmp(ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_PARAGRAPHUS].loci_offset
            + (i32)ORATIO_PARAGRAPHUS_FORMA].titulus, "forma")
            == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_PARAGRAPHUS].loci_offset
            + (i32)ORATIO_PARAGRAPHUS_FORMA].species,
            (s32)MATERIA_LOCUS_INDEX);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_PARAGRAPHUS].loci_numerus, (i32)IV);
        CREDO_MINOR_S32 ((s32)ORATIO_FORMA_INDEX,
            (s32)ORATIO_FORMA_NUMERUS_FORMARUM);
        CREDO_AEQUALIS_S32 (ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_VOCABULUM].loci_offset
            + (i32)ORATIO_VOCABULUM_PARTES].species,
            (s32)MATERIA_LOCUS_LISTA_TOKEN);
        CREDO_AEQUALIS_S32 (ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_VOCABULUM].loci_offset
            + (i32)ORATIO_VOCABULUM_ANALYSES].species,
            (s32)MATERIA_LOCUS_LISTA_NODUS);
        CREDO_AEQUALIS_S32 (ORATIO_REGISTRUM.loci[
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_INTERPUNCTIO].loci_offset
            + (i32)ORATIO_INTERPUNCTIO_SIGNUM].species,
            (s32)MATERIA_LOCUS_TOKEN);
    }

        {
        i32 i;
        i32 j;

        imprimere("\n--- Probans genera analysis-* (T11) ---\n");
        CREDO_AEQUALIS_I32 ((i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM,
            (i32)XVII);
        CREDO_AEQUALIS_I32 ((i32)ORATIO_GENUS_NUMERUS_GENERUM
            - (i32)ORATIO_GENUS_ANALYSIS_PRIMUM,
            (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM);
        CREDO_AEQUALIS_I32 ((i32)ORATIO_GENUS_ANALYSIS_PRIMUM,
            (i32)ORATIO_GENUS_NUMERUS + I);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.numerus_locorum, (i32)146);
        per (i = ZEPHYRUM; i
            < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM; i++)
        {
                 OratioClassis  cl  = (OratioClassis)i;
                   OratioGenus  g   = oratio_classis_genus(cl);
            constans character* t   = oratio_classis_titulus(cl);

            CREDO_AEQUALIS_I32 ((i32)oratio_genus_classis(g), i);
            CREDO_NON_NIHIL (t);
            CREDO_AEQUALIS_I32 ((i32)oratio_classis_ex_titulo(t,
                (i32)strlen(t)), i);
            CREDO_VERUM (strncmp(ORATIO_REGISTRUM.genera[g].titulus,
                "analysis-", (size_t)IX) == ZEPHYRUM);
            /* loci communes V primi, ordine, speciebus suis */
            CREDO_VERUM (ORATIO_REGISTRUM.genera[g].loci_numerus
                >= (i32)ORATIO_ANALYSIS_COMMUNIA_NUMERUS);
            CREDO_VERUM (strcmp(_locus(g, (i32)ORATIO_ANALYSIS_LEMMA),
                "lemma") == ZEPHYRUM);
            CREDO_VERUM (strcmp(_locus(g, (i32)ORATIO_ANALYSIS_LINGUA),
                "lingua") == ZEPHYRUM);
            CREDO_VERUM (strcmp(_locus(g, (i32)ORATIO_ANALYSIS_FONS),
                "fons") == ZEPHYRUM);
            CREDO_VERUM (strcmp(_locus(g, (i32)ORATIO_ANALYSIS_NATIVUM),
                "nativum") == ZEPHYRUM);
            CREDO_VERUM (strcmp(_locus(g, (i32)ORATIO_ANALYSIS_SENSUS),
                "sensus") == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (_species(g, (i32)ORATIO_ANALYSIS_LEMMA),
                (s32)MATERIA_LOCUS_TOKEN);
            CREDO_AEQUALIS_S32 (_species(g,
                (i32)ORATIO_ANALYSIS_LINGUA),
                (s32)MATERIA_LOCUS_INDEX);
            CREDO_AEQUALIS_S32 (_species(g, (i32)ORATIO_ANALYSIS_FONS),
                (s32)MATERIA_LOCUS_INDEX);
            CREDO_AEQUALIS_S32 (_species(g,
                (i32)ORATIO_ANALYSIS_NATIVUM),
                (s32)MATERIA_LOCUS_TOKEN);
            CREDO_AEQUALIS_S32 (_species(g,
                (i32)ORATIO_ANALYSIS_SENSUS),
                (s32)MATERIA_LOCUS_TOKEN);
            /* accidentia: INDEX omnia */
            per (j = (i32)ORATIO_ANALYSIS_COMMUNIA_NUMERUS;
                 j < ORATIO_REGISTRUM.genera[g].loci_numerus; j++)
            {
                CREDO_AEQUALIS_S32 (_species(g, j),
                    (s32)MATERIA_LOCUS_INDEX);
            }
        }
        CREDO_AEQUALIS_I32 ((i32)oratio_classis_ex_titulo("xyzzy",
            (i32)V),
            (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM);
        CREDO_AEQUALIS_I32 ((i32)oratio_genus_classis(
            ORATIO_GENUS_VOCABULUM),
            (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM);
        CREDO_NIHIL (oratio_classis_titulus(
            ORATIO_CLASSIS_NUMERUS_CLASSIUM));
        /* accidentia per classem (decisio 2026-09-04) */
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_SUBSTANTIVI].loci_numerus, (i32)IX);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_SUBSTANTIVI,
            (i32)ORATIO_ANALYSIS_SUBSTANTIVI_CASUS), "casus")
                == ZEPHYRUM);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_SUBSTANTIVI,
            (i32)ORATIO_ANALYSIS_SUBSTANTIVI_DECLINATIO), "declinatio")
            == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_NOMINIS_PROPRII].loci_numerus,
            (i32)IX);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_VERBI].loci_numerus, (i32)XIV);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_VERBI,
            (i32)ORATIO_ANALYSIS_VERBI_PERSONA), "persona")
                == ZEPHYRUM);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_VERBI,
            (i32)ORATIO_ANALYSIS_VERBI_FORMA_VERBI), "forma-verbi")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_VERBI,
            (i32)ORATIO_ANALYSIS_VERBI_GENUS), "genus") == ZEPHYRUM);
        /* auxiliare = forma verbi (loci iidem) */
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_AUXILIARIS].loci_numerus, (i32)XIV);
        per (j = ZEPHYRUM; j < (i32)XIV; j++)
        {
            CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_VERBI, j),
                _locus(ORATIO_GENUS_ANALYSIS_AUXILIARIS, j))
                    == ZEPHYRUM);
        }
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_ADIECTIVI].loci_numerus, (i32)X);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_ADIECTIVI,
            (i32)ORATIO_ANALYSIS_ADIECTIVI_GRADUS), "gradus")
                == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_ADVERBII].loci_numerus, (i32)VI);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_PRONOMINIS].loci_numerus, (i32)IX);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_PRONOMINIS,
            (i32)ORATIO_ANALYSIS_PRONOMINIS_PERSONA), "persona")
            == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_DETERMINANTIS].loci_numerus,
            (i32)VIII);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_ADPOSITIONIS].loci_numerus, (i32)VI);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_ADPOSITIONIS,
            (i32)ORATIO_ANALYSIS_ADPOSITIONIS_CASUS), "casus")
                == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_NUMERALIS].loci_numerus, (i32)IX);
        CREDO_VERUM (strcmp(_locus(ORATIO_GENUS_ANALYSIS_NUMERALIS,
            (i32)ORATIO_ANALYSIS_NUMERALIS_SPECIES), "species")
                == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_CONIUNCTIONIS_COORDINANTIS].loci_numerus,
            (i32)V);
        CREDO_AEQUALIS_I32 (ORATIO_REGISTRUM.genera[
            ORATIO_GENUS_ANALYSIS_IGNOTI].loci_numerus, (i32)V);
        /* enumerationes: primus et ultimus titulus */
        CREDO_VERUM (strcmp(ORATIO_TITULI_CASUUM[ORATIO_CASUS_NOMINATIVUS],
            "nominativus") == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_CASUUM[ORATIO_CASUS_NUMERUS
            - I],
            "locativus") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)ORATIO_CASUS_NUMERUS, (i32)VII);
        CREDO_VERUM (strcmp(ORATIO_TITULI_NUMERORUM[
            ORATIO_NUMERUS_GRAMMATICUS_NUMERUS - I], "dualis")
                == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_GENERUM_GRAMMATICORUM[
            ORATIO_GENUS_GRAMMATICUM_NUMERUS - I], "commune")
                == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_PERSONARUM[
            ORATIO_PERSONA_TERTIA], "tertia") == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_TEMPORUM[
            ORATIO_TEMPUS_NUMERUS - I], "praeteritum") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)ORATIO_TEMPUS_NUMERUS, (i32)VII);
        CREDO_VERUM (strcmp(ORATIO_TITULI_MODORUM[
            ORATIO_MODUS_NUMERUS - I], "infinitivus") == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_VOCUM[ORATIO_VOX_DEPONENS],
            "deponens") == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_FORMARUM_VERBI[
            ORATIO_FORMA_VERBI_NUMERUS - I], "supinum") == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_GRADUUM[
            ORATIO_GRADUS_SUPERLATIVUS], "superlativus") == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_SPECIERUM_NUMERALIS[
            ORATIO_SPECIES_NUMERALIS_NUMERUS - I], "adverbiale")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_LINGUARUM[ORATIO_LINGUA_ANGLICA],
            "anglica") == ZEPHYRUM);
        CREDO_VERUM (strcmp(ORATIO_TITULI_FONTIUM_ANALYSIS[
            ORATIO_FONS_ANALYSIS_GLOSSARIUM], "glossarium")
                == ZEPHYRUM);
        /* CUSTOS: tituli classium == optiones 'classis' canonis glossarii
         * (oratio/grammatica/glossarium.canon) - una fons vocabularii */
        {
            constans character* radix = getenv("RHUBARB_RADIX");
                     character  via[1024];
                        chorda  canon;

            sprintf(via, "%s/oratio/grammatica/glossarium.canon",
                radix != NIHIL ? radix : ".");
            si (!_plagulam_legere(piscina, via, &canon))
            {
                CREDO_CULPA ("glossarium.canon absens");
            }
            alioquin
            {
                per (i = ZEPHYRUM; i
                    < (i32)ORATIO_CLASSIS_NUMERUS_CLASSIUM;
                     i++)
                {
                    character optio[96];

                    sprintf(optio, "<optio>%s</optio>",
                        oratio_classis_titulus((OratioClassis)i));
                    si (strstr((character*)canon.datum, optio) == NIHIL)
                    {
                        imprimere("    classis '%s' in canone glossarii DEEST\n",
                            oratio_classis_titulus((OratioClassis)i));
                    }
                    CREDO_NON_NIHIL (strstr((character*)canon.datum,
                        optio));
                }
            }
        }
    }

    {
        MateriaArborConsilium c;
        MateriaNodus*         documentum;
        MateriaNodus*         paragraphus;
        MateriaNodus*         sententia;
        MateriaNodus*         vocabulum;
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
        MateriaNodus*         lecta;
        MateriaArborVitium    vitium;

        imprimere("\n--- Probans circuitum orationis per materiam ---\n");
        /* "a\n": vocabulum 'a' cum cauda LINEA */
        vocabulum = materia_nodus_creare(piscina,
            (s32)ORATIO_GENUS_VOCABULUM,
            (i32)V);
        CREDO_VERUM (materia_nodus_appendere(piscina, vocabulum,
            (i32)ORATIO_VOCABULUM_PARTES,
            materia_valor_token(_lexema(piscina,
            (s32)ORATIO_LEX_LITTERAE,
                "a", ZEPHYRUM, (i32)I, (i32)I)),
                MATERIA_LOCUS_LISTA_TOKEN));
        CREDO_VERUM (materia_nodus_appendere(piscina, vocabulum,
            (i32)ORATIO_VOCABULUM_CAUDA,
            materia_valor_token(_lexema(piscina, (s32)ORATIO_LEX_LINEA,
                "\n", (s32)I, (i32)I, (i32)II)),
                MATERIA_LOCUS_LISTA_TOKEN));
                /* T11: analysis-substantivi in vocabulo - lemma derivatum, lingua
         * et casus INDEX; circuitus totus eam ferre debet */
        {
            MateriaNodus* analysis = materia_nodus_creare(piscina,
                (s32)ORATIO_GENUS_ANALYSIS_SUBSTANTIVI,
                ORATIO_REGISTRUM.genera[
                    ORATIO_GENUS_ANALYSIS_SUBSTANTIVI].loci_numerus);
            MateriaToken* origo = materia_valor_lista_obtinere(
                vocabulum->loci[ORATIO_VOCABULUM_PARTES], ZEPHYRUM)
                ->datum.token;

            CREDO_NON_NIHIL (analysis);
            CREDO_VERUM (materia_nodus_ponere(analysis,
                (i32)ORATIO_ANALYSIS_LEMMA,
                materia_valor_token(oratio_lexema_derivatum(piscina,
                    (s32)ORATIO_LEX_DERIVATUM,
                    chorda_ex_literis("a", piscina), origo)),
                MATERIA_LOCUS_TOKEN));
            CREDO_VERUM (materia_nodus_ponere(analysis,
                (i32)ORATIO_ANALYSIS_LINGUA,
                materia_valor_index((s32)ORATIO_LINGUA_LATINA),
                MATERIA_LOCUS_INDEX));
            CREDO_VERUM (materia_nodus_ponere(analysis,
                (i32)ORATIO_ANALYSIS_SUBSTANTIVI_CASUS,
                materia_valor_index((s32)ORATIO_CASUS_ABLATIVUS),
                MATERIA_LOCUS_INDEX));
            CREDO_VERUM (materia_nodus_appendere(piscina, vocabulum,
                (i32)ORATIO_VOCABULUM_ANALYSES,
                materia_valor_nodus(analysis),
                MATERIA_LOCUS_LISTA_NODUS));
        }
        sententia = materia_nodus_creare(piscina,
            (s32)ORATIO_GENUS_SENTENTIA,
            (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, sententia,
            (i32)ORATIO_SENTENTIA_ELEMENTA,
            materia_valor_nodus(vocabulum),
            MATERIA_LOCUS_LISTA_NODUS));
        paragraphus = materia_nodus_creare(piscina,
            (s32)ORATIO_GENUS_PARAGRAPHUS,
            ORATIO_REGISTRUM.genera[ORATIO_GENUS_PARAGRAPHUS].loci_numerus);
        CREDO_VERUM (materia_nodus_appendere(piscina, paragraphus,
            (i32)ORATIO_PARAGRAPHUS_SENTENTIAE,
            materia_valor_nodus(sententia),
            MATERIA_LOCUS_LISTA_NODUS));
        documentum = materia_nodus_creare(piscina,
            (s32)ORATIO_GENUS_DOCUMENTUM, (i32)III);
        CREDO_VERUM (materia_nodus_appendere(piscina, documentum,
            (i32)ORATIO_DOCUMENTUM_PARAGRAPHI,
            materia_valor_nodus(paragraphus),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(documentum,
            (i32)ORATIO_DOCUMENTUM_FINIS,
            materia_valor_token(_lexema(piscina, (s32)ORATIO_LEX_FINIS,
            "",
                (s32)II, (i32)II, (i32)I)), MATERIA_LOCUS_TOKEN));

        materia_arbor_consilium_nudum(&c, &ORATIO_REGISTRUM, &ratum,
            "oratio");
        s1 = materia_arbor_scribere_nodum(piscina, documentum, &c);
        CREDO_VERUM (s1.successus);
        si (!s1.successus)
        {
            imprimere("    (scriptura: %s)\n",
                s1.causa ? s1.causa : "-");
        }
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum,
            "grammatica=\"oratio\""));
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum,
            "<vocabulum"));
                CREDO_NON_NIHIL (strstr((character*)s1.textus.datum,
                    "<or-litterae"));
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum,
            "<analysis-substantivi"));
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum, "<casus"));
        CREDO_NON_NIHIL (strstr((character*)s1.textus.datum, "<lemma"));
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
            CREDO_AEQUALIS_S32 (lecta->genus,
                (s32)ORATIO_GENUS_DOCUMENTUM);
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
