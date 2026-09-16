/* probatio_html_registrum.c - Registrum et lexicon HTML
 *
 * QUATTUOR CUSTODIAE (exemplar probatio_css_registrum):
 *
 *  1. ORDO LEXICI enumerationem HtmlLexemaGenus sequi DEBET, quia
 *     materia genera ut INDICES tractat. Assertio per TITULOS fit,
 *     non per numeros: permutatio ergo capitur, non absorbetur.
 *  2. RANCOR tabularum coctarum: html_registrum_coctum.{h,c} ex
 *     html.registrum.stml GENERATAE - redditio in memoria contra
 *     plagulas commissas OCTETIM (materia_registrum_recens).
 *  3. OFFSETS LOCORUM CONTIGUI, et LOCI NOMINATI (enumerationes
 *     html_registrum.h) contra titulos tabulae - ne tabula tertia
 *     manu scripta per se labatur.
 *  4. HTML munere LINEA CARET (SPATIA lineas novas intra tag
 *     absorbet), ergo capacitas linea-sensitiva RECUSATUR - assertum
 *     POSITIVE; et COMMENTARIUM SUBSTANTIVUM est, non trivium (H6).
 */

#include "latina.h"
#include "credo.h"
#include "html_registrum.h"
#include "html_lexicon.h"
#include "html_lexema.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_coctor.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Quaesitio litterarum in chorda SINE fine NUL (strstr super datum
 * chordae examini domesticum est) */
interior b32
_textus_continet (
                 chorda  textus,
     constans character* litterae)
{
    i32 mensura = (i32)strlen(litterae);
    i32 i;

    si (mensura == ZEPHYRUM || textus.mensura < mensura)
    { redde FALSUM;
    }
    per (i = ZEPHYRUM; i + mensura <= textus.mensura; i++)
    {
        si (memcmp(textus.datum + i, litterae, (size_t)mensura)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Nomina generum lexicalium HTML, ordine HtmlLexemaGenus. Fons
 * veritatis SECUNDUS consulto: si html_lexema.h permutetur, haec
 * lista et lexicon DISCREPABUNT et probatio cadet. */
hic_manens constans character* ORDO_EXSPECTATUS[] = {
    "FINIS", "TEXTUS", "REFERENTIA", "TEXTUS_CRUDUS",
    "TEXTUS_CRUDUS_IMPERFECTUS", "TAG_APERTURA", "TAG_CLAUSURA",
    "TAG_FINIS", "TAG_FINIS_SOLUS", "ATTRIBUTUM_NOMEN", "AEQUALE",
    "ATTRIBUTUM_VALOR", "ATTRIBUTUM_VALOR_IMPERFECTUS", "COMMENTARIUM",
    "COMMENTARIUM_IMPERFECTUM", "COMMENTARIUM_PRAVUM", "DOCTYPE",
    "DOCTYPE_IMPERFECTUM", "CDATA", "CDATA_IMPERFECTA", "SPATIA",
    "DELIM"
};

hic_manens constans character* GENERA_EXSPECTATA[] = {
    "documentum", "doctype", "elementum", "attributum", "textus",
    "referentia", "textus-crudus", "commentarium", "cdata",
    "elementum-malum"
};

/* Locus NOMINATUS: enumeratio html_registrum.h contra TITULOS
 * tabulae. Sine hac probatione enumerationes tertia tabula manu
 * scripta essent, quae per se labi posset - lapsus TACITUS. */
nomen structura {
                    s32  genus;
                    i32  locus;
     constans character* titulus;
} LocusNominatus;

hic_manens constans LocusNominatus LOCI_NOMINATI[] = {
    { (s32)HTML_GENUS_DOCUMENTUM, (i32)HTML_DOCUMENTUM_LIBERI,
        "liberi" },
    { (s32)HTML_GENUS_DOCUMENTUM, (i32)HTML_DOCUMENTUM_CAUDA,
        "cauda" },
    { (s32)HTML_GENUS_DOCTYPE,    (i32)HTML_DOCTYPE_TOK,       "tok" },
    { (s32)HTML_GENUS_ELEMENTUM,  (i32)HTML_ELEMENTUM_TOK_APERTURA,
        "tok_apertura" },
    { (s32)HTML_GENUS_ELEMENTUM,  (i32)HTML_ELEMENTUM_ATTRIBUTA,
        "attributa" },
    { (s32)HTML_GENUS_ELEMENTUM,  (i32)HTML_ELEMENTUM_TOK_FINIS,
        "tok_finis" },
    { (s32)HTML_GENUS_ELEMENTUM,  (i32)HTML_ELEMENTUM_LIBERI,
        "liberi" },
    { (s32)HTML_GENUS_ELEMENTUM,  (i32)HTML_ELEMENTUM_TOK_CLAUSURA,
        "tok_clausura" },
    { (s32)HTML_GENUS_ELEMENTUM,
        (i32)HTML_ELEMENTUM_TOK_CLAUSURA_FINIS,
        "tok_clausura_finis" },
    { (s32)HTML_GENUS_ELEMENTUM,  (i32)HTML_ELEMENTUM_SYNTHESIS,
        "synthesis" },
    { (s32)HTML_GENUS_ATTRIBUTUM, (i32)HTML_ATTRIBUTUM_TOK_NOMEN,
        "tok_nomen" },
    { (s32)HTML_GENUS_ATTRIBUTUM, (i32)HTML_ATTRIBUTUM_TOK_AEQUALE,
        "tok_aequale" },
    { (s32)HTML_GENUS_ATTRIBUTUM, (i32)HTML_ATTRIBUTUM_TOK_VALOR,
        "tok_valor" },
    { (s32)HTML_GENUS_TEXTUS,        (i32)HTML_TEXTUS_TOK,
        "tok" },
    { (s32)HTML_GENUS_REFERENTIA,    (i32)HTML_REFERENTIA_TOK,
        "tok" },
    { (s32)HTML_GENUS_TEXTUS_CRUDUS, (i32)HTML_CRUDUS_TOK,
        "tok" },
    { (s32)HTML_GENUS_COMMENTARIUM,  (i32)HTML_COMMENTARIUM_TOK,
        "tok" },
    { (s32)HTML_GENUS_CDATA,         (i32)HTML_CDATA_TOK,
        "tok" },
    { (s32)HTML_GENUS_ELEMENTUM_MALUM, (i32)HTML_MALUM_TOKENS,
        "tokens" }
};

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_html_registrum",
        32768);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: lexicon portam transit, et ORDINEM servat
     * ================================================== */

    {
        i32 i;

        imprimere("\n--- Probans lexicon HTML ---\n");

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &HTML_LEXICON,
            &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);

        /* Numerus generum lexico == numerus lexatoris (XXII) */
        CREDO_AEQUALIS_I32 (HTML_LEXICON.numerus_generum,
            (i32)HTML_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)HTML_LEX_NUMERUS_GENERUM);

        /* ORDO per titulos - permutatio capitur */
        per (i = ZEPHYRUM; i < (i32)HTML_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(&ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
        }

        /* Sedes notae - erratum transcriptionis grossum caperent */
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)HTML_LEX_TAG_FINIS), (s32)MATERIA_LEX_FIXUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)HTML_LEX_TAG_FINIS), ">") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)HTML_LEX_TAG_FINIS_SOLUS), "/>") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)HTML_LEX_AEQUALE), "=") == ZEPHYRUM);
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)HTML_LEX_TEXTUS));
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)HTML_LEX_CDATA));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (s32)HTML_LEX_TAG_FINIS));
    }


    /* ==================================================
     * PROBARE: trivia INTRA TAG sola; commentaria NODI (H6)
     * ================================================== */

    {
        i32 postulata;

        imprimere("\n--- Probans trivia et munus LINEA absens ---\n");

        /* SPATIA lineas novas intra tag absorbet: VERBATIM/SPATIUM */
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)HTML_LEX_SPATIA), (s32)MATERIA_LEX_VERBATIM);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (s32)HTML_LEX_SPATIA), (s32)MATERIA_MUNUS_SPATIUM);
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)HTML_LEX_SPATIA));
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)HTML_LEX_DELIM));

        /* COMMENTARIUM* et TEXTUS contentum sunt - NUMQUAM trivia */
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)HTML_LEX_COMMENTARIUM));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)HTML_LEX_COMMENTARIUM_PRAVUM));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)HTML_LEX_TEXTUS));
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (s32)HTML_LEX_COMMENTARIUM),
            (s32)MATERIA_MUNUS_SUBSTANTIVUM);

        /* Munera quae HTML FERT, et quae NON */
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_COMMENTUM));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_FALSUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LAMINA));

        /* Capacitas linea-sensitiva RECUSATUR (politica B) */
        postulata = MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_SPATIUM)
                  | MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_LINEA);
        CREDO_FALSUM (materia_lexicon_munera_habet(&ratum, postulata));
        CREDO_VERUM (materia_lexicon_munera_habet(&ratum,
            MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_SPATIUM)));
    }


    /* ==================================================
     * PROBARE: porta rancoris tabularum coctarum
     * html_registrum_coctum.{h,c} ex html.registrum.stml GENERATAE -
     * redditio in memoria contra plagulas commissas OCTETIM (output,
     * numquam tempora): declaratio mutata sine ./materia/coquere.sh
     * -scribere, aut plagula generata manu tacta, hic rubet cum linea
     * prima divergente.
     * ================================================== */

    {
        constans character* radix = getenv("RHUBARB_RADIX");
             MateriaRancor  rancor;

        imprimere("\n--- Probans rancorem tabularum coctarum ---\n");
        si (!materia_registrum_recens(piscina, radix
            != NIHIL ? radix : ".",
                "html/grammatica/html.registrum.stml", &rancor))
        {
            imprimere("    recusatio: %.*s\n",
                (integer)rancor.causa.mensura,
                (constans character*)rancor.causa.datum);
            CREDO_CULPA ("declaratio absens aut recusata");
        }
        alioquin
        {
            si (!rancor.recens)
            {
                imprimere("    RANCIDUM: %.*s:%u\n",
                    (integer)rancor.via.mensura,
                    (constans character*)rancor.via.datum,
                    rancor.linea);
            }
            CREDO_VERUM (rancor.recens);
            CREDO_AEQUALIS_I32 (rancor.coctio.numerus_locorum,
                HTML_REGISTRUM.numerus_locorum);
            CREDO_AEQUALIS_I32 (rancor.coctio.numerus_generum,
                HTML_REGISTRUM.numerus_generum);
        }
    }


    /* ==================================================
     * PROBARE: registrum - ordo, CONTIGUITAS locorum, loci nominati
     * ================================================== */

    {
        i32 i;
        i32 exspectatus_offset;
        i32 numerus_nominatorum;

        imprimere("\n--- Probans registrum nodorum ---\n");

        CREDO_AEQUALIS_I32 (HTML_REGISTRUM.numerus_generum,
            (i32)HTML_GENUS_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])),
            (i32)HTML_GENUS_NUMERUS_GENERUM);

        /* Enumeratio et tabula congruunt, per TITULOS */
        per (i = ZEPHYRUM; i < (i32)HTML_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_NON_NIHIL (HTML_REGISTRUM.genera[i].titulus);
            CREDO_VERUM (strcmp(HTML_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(
                &HTML_REGISTRUM, GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
        }

        /* CONTIGUITAS: offset cuiusque == summa numerorum priorum */
        exspectatus_offset = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)HTML_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_AEQUALIS_I32 (HTML_REGISTRUM.genera[i].loci_offset,
                exspectatus_offset);
            CREDO_MAIOR_I32 (HTML_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            exspectatus_offset += HTML_REGISTRUM.genera[i].loci_numerus;
        }
        CREDO_AEQUALIS_I32 (exspectatus_offset,
            HTML_REGISTRUM.numerus_locorum);

        /* Loci omnes titulum et speciem validam ferunt */
        per (i = ZEPHYRUM; i < HTML_REGISTRUM.numerus_locorum; i++)
        {
            CREDO_NON_NIHIL (HTML_REGISTRUM.loci[i].titulus);
            CREDO_MINOR_S32 (HTML_REGISTRUM.loci[i].species,
                (s32)MATERIA_LOCUS_NUMERUS_SPECIERUM);
        }

        /* LOCI NOMINATI: enumeratio cuiusque generis contra titulos,
         * et numerus nominatorum == numerus locorum (XIX: synthesis
         * O7a) - ne locus ullus innominatus maneat */
        numerus_nominatorum = (i32)(magnitudo(LOCI_NOMINATI)
            / magnitudo(LOCI_NOMINATI[0]));
        CREDO_AEQUALIS_I32 (numerus_nominatorum,
            HTML_REGISTRUM.numerus_locorum);
        per (i = ZEPHYRUM; i < numerus_nominatorum; i++)
        {
            constans LocusNominatus* n = &LOCI_NOMINATI[i];
                                i32  absolutus;

            CREDO_MINOR_I32 (n->locus,
                HTML_REGISTRUM.genera[n->genus].loci_numerus);
            absolutus = HTML_REGISTRUM.genera[n->genus].loci_offset
                      + n->locus;
            CREDO_VERUM (strcmp(HTML_REGISTRUM.loci[absolutus].titulus,
                n->titulus) == ZEPHYRUM);
        }
    }


    /* ==================================================
     * PROBARE: circuitus per materiam (arbor HTML minima '<br>')
     * ================================================== */

    {
        MateriaArborConsilium c;
                 MateriaNodus* documentum;
                 MateriaNodus* elementum;
        MateriaArborScriptura  s1;
        MateriaArborScriptura  s2;
                 MateriaNodus* lecta;
           MateriaArborVitium  vitium;

        imprimere("\n--- Probans circuitum HTML per materiam ---\n");

        /* '<br>' - elementum vacuum: apertura et finis soli, clausura
         * ABSENS (H4) */
        elementum = materia_nodus_creare(piscina,
            (s32)HTML_GENUS_ELEMENTUM, (i32)VII);
        CREDO_VERUM (materia_nodus_ponere(elementum,
            (i32)HTML_ELEMENTUM_TOK_APERTURA,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)HTML_LEX_TAG_APERTURA,
                chorda_ex_literis("<br", piscina), ZEPHYRUM, (i32)I,
                (i32)I, ZEPHYRUM)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(elementum,
            (i32)HTML_ELEMENTUM_TOK_FINIS,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)HTML_LEX_TAG_FINIS,
                chorda_ex_literis(">", piscina), (s32)III, (i32)I,
                (i32)IV, ZEPHYRUM)), MATERIA_LOCUS_TOKEN));

        documentum = materia_nodus_creare(piscina,
            (s32)HTML_GENUS_DOCUMENTUM, (i32)II);
        CREDO_VERUM (materia_nodus_appendere(piscina, documentum,
            (i32)HTML_DOCUMENTUM_LIBERI, materia_valor_nodus(elementum),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(documentum,
            (i32)HTML_DOCUMENTUM_CAUDA,
            materia_valor_token(materia_token_creare(piscina, &FORMA,
                (s32)HTML_LEX_FINIS,
                chorda_ex_literis("", piscina), (s32)IV, (i32)I,
                (i32)V, ZEPHYRUM)), MATERIA_LOCUS_TOKEN));

        materia_arbor_consilium_nudum(&c, &HTML_REGISTRUM, &ratum,
            "html");
        s1 = materia_arbor_scribere_nodum(piscina, documentum, &c);
        CREDO_VERUM (s1.successus);
        CREDO_VERUM (_textus_continet(s1.textus,
            "grammatica=\"html\""));
        CREDO_VERUM (_textus_continet(s1.textus, "<lex-tag-finis"));

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        CREDO_AEQUALIS_S32 (lecta->genus, (s32)HTML_GENUS_DOCUMENTUM);

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
