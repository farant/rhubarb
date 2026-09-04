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
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
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
    "interpunctio", "numerus"
};

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
