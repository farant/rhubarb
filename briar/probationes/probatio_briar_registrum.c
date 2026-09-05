/* probatio_briar_registrum.c - Registrum et lexicon thistle
 *
 * Tres custodiae tabularum manu scriptarum (exemplar
 * probatio_md_registrum): ordo lexici per TITULOS, offsets locorum
 * CONTIGUI, loci nominati contra titulos. Et circuitus minimus per
 * materiam: documentum vacuum (FINIS solum) scriptum -> lectum ->
 * scriptum, octetim idem.
 */

#include "latina.h"
#include "credo.h"
#include "briar_registrum.h"
#include "briar_lexicon.h"
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
    "FINIS", "INTERPRES", "TAG_APERTUM", "TAG_CLAUSUM", "CRUDUM",
    "PROSA", "STML", "DERIVATUM"
};

hic_manens constans character* GENERA_EXSPECTATA[] = {
    "documentum", "interpres", "prosa", "regio", "elementum"
};

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_briar_registrum",
        65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans lexicon thistle ---\n");
    {
        i32 i;

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum,
            &BRIAR_LEXICON,
            &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);
        CREDO_AEQUALIS_I32 (BRIAR_LEXICON.numerus_generum,
            (i32)BRIAR_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)BRIAR_LEX_NUMERUS_GENERUM);
        per (i = ZEPHYRUM; i < (i32)BRIAR_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(&ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
        }
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)BRIAR_LEX_FINIS), (s32)MATERIA_LEX_FIXUM);
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)BRIAR_LEX_PROSA));
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)BRIAR_LEX_CRUDUM));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (s32)BRIAR_LEX_FINIS));
        CREDO_VERUM (strcmp(BRIAR_LEXICON.praefixum_tagi, "briar-")
            == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (BRIAR_LEXICON.genus_spatii, (s32)-I);
        /* nullum trivium, nullum genus LINEA: lineae novae intra
         * valores vivunt */
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
    }

    imprimere("\n--- Probans registrum nodorum ---\n");
    {
        i32 i;
        i32 offset = ZEPHYRUM;

        CREDO_AEQUALIS_I32 (BRIAR_REGISTRUM.numerus_generum,
            (i32)BRIAR_GENUS_NUMERUS);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])),
            (i32)BRIAR_GENUS_NUMERUS);
        per (i = ZEPHYRUM; i < (i32)BRIAR_GENUS_NUMERUS; i++)
        {
            CREDO_VERUM (strcmp(BRIAR_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(
                &BRIAR_REGISTRUM, GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
            CREDO_AEQUALIS_I32 (BRIAR_REGISTRUM.genera[i].loci_offset,
                offset);
            CREDO_MAIOR_I32 (BRIAR_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            offset += BRIAR_REGISTRUM.genera[i].loci_numerus;
        }
        CREDO_AEQUALIS_I32 (offset, BRIAR_REGISTRUM.numerus_locorum);
        CREDO_AEQUALIS_I32 (BRIAR_REGISTRUM.numerus_locorum, (i32)14);

        /* loci nominati contra titulos tabulae */
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_DOCUMENTUM].loci_offset
            + (i32)BRIAR_DOCUMENTUM_PARTES].titulus, "partes")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_DOCUMENTUM].loci_offset
            + (i32)BRIAR_DOCUMENTUM_VITIUM].titulus, "vitium")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_REGIO].loci_offset
            + (i32)BRIAR_REGIO_CONTENTUM].titulus, "contentum")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_REGIO].loci_offset
            + (i32)BRIAR_REGIO_CLAUSUM].titulus, "clausum")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_ELEMENTUM].loci_offset
            + (i32)BRIAR_ELEMENTUM_TITULUS].titulus, "titulus")
            == ZEPHYRUM);
        CREDO_AEQUALIS_S32 (BRIAR_REGISTRUM.loci[
            BRIAR_REGISTRUM.genera[BRIAR_GENUS_REGIO].loci_offset
            + (i32)BRIAR_REGIO_VITIUM].species,
            (s32)MATERIA_LOCUS_INDEX);
    }

    imprimere("\n--- Probans circuitum minimum per materiam ---\n");
    {
        MateriaArborConsilium consilium;
        MateriaArborScriptura s1;
        MateriaArborScriptura s2;
           MateriaArborVitium vitium;
                MateriaNodus* doc;
                MateriaNodus* lecta;
                MateriaToken* finis;
                       chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        materia_arbor_consilium_nudum(&consilium, &BRIAR_REGISTRUM,
            &ratum, "briar");
        doc = materia_nodus_creare(piscina, (s32)BRIAR_GENUS_DOCUMENTUM,
            (i32)4);
        finis = materia_token_creare(piscina, &FORMA,
            (s32)BRIAR_LEX_FINIS,
            vacua, ZEPHYRUM, I, I, BRIAR_FONS_PLAGULAE);
        CREDO_NON_NIHIL (doc);
        CREDO_NON_NIHIL (finis);
        CREDO_VERUM (materia_nodus_ponere(doc,
            (i32)BRIAR_DOCUMENTUM_FINIS,
            materia_valor_token(finis), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(doc,
            (i32)BRIAR_DOCUMENTUM_VITIUM,
            materia_valor_index((s32)BRIAR_VITIUM_NULLUM),
            MATERIA_LOCUS_INDEX));
        s1 = materia_arbor_scribere_nodum(piscina, doc, &consilium);
        CREDO_VERUM (s1.successus);
        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus,
            &consilium,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        si (lecta != NIHIL)
        {
            s2 = materia_arbor_scribere_nodum(piscina, lecta,
                &consilium);
            CREDO_VERUM (s2.successus);
            CREDO_AEQUALIS_I32 (s1.textus.mensura, s2.textus.mensura);
            CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
                (size_t)s1.textus.mensura) == ZEPHYRUM);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
