/* probatio_toml_registrum.c - Registrum et lexicon toml
 *
 * QUATTUOR CUSTODIAE (exemplar probatio_crusta_registrum):
 *
 *  1. ORDO LEXICI enumerationem TomlLexGenus sequi DEBET, quia
 *     materia genera ut INDICES tractat. Assertio per TITULOS fit,
 *     non per numeros: permutatio ergo capitur, non absorbetur.
 *  2. RANCOR tabularum coctarum: toml_registrum_coctum.{h,c} ex
 *     toml.registrum.stml GENERATAE - redditio in memoria contra
 *     plagulas commissas OCTETIM (materia_registrum_recens).
 *  3. OFFSETS LOCORUM CONTIGUI, et LOCI NOMINATI (enumerationes
 *     toml_registrum.h) contra titulos tabulae - ne tabula tertia
 *     manu scripta per se labatur. Genera quae enumerationem communem
 *     habent hic SEORSUM enumerantur.
 *  4. LINEA NOVA BIS: LINEA_FINIS substantiva, LINEA trivium; munus
 *     LINEA ADEST - capacitas linea-sensitiva CONCEDITUR.
 * Et arbor minima 'a = 1' per scriptorem -> lectorem -> scriptorem.
 */

#include "latina.h"
#include "credo.h"
#include "toml_registrum.h"
#include "toml_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_coctor.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Quaesitio litterarum in chorda SINE fine NUL */
interior b32
_textus_continet (
                 chorda  textus,
     constans character* litterae)
{
    i32 mensura = (i32)strlen(litterae);
    i32 i;

    si (mensura == ZEPHYRUM || textus.mensura < mensura)
    {
        redde FALSUM;
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

interior MateriaToken*
_token (
               Piscina* piscina,
                   s32  genus,
    constans character* textus,
                   s32  offset)
{
    redde materia_token_creare(piscina, &FORMA, genus,
        chorda_ex_literis(textus, piscina), offset, (i32)I,
        (i32)(offset + I), ZEPHYRUM);
}

/* Nomina generum lexicalium, ordine TomlLexGenus. Fons veritatis
 * SECUNDUS consulto: si toml_lexicon.h permutetur, haec lista et
 * lexicon DISCREPABUNT et probatio cadet. */
hic_manens constans character* ORDO_EXSPECTATUS[] = {
    "FINIS",
    "CLAVIS_NUDA",
    "CLAVIS_GEMINA",
    "CLAVIS_SIMPLEX",
    "PUNCTUM",
    "SIGNUM",
    "TABULA_APERTURA",
    "TABULA_CLAUSURA",
    "SERIES_TABULARUM_APERTURA",
    "SERIES_TABULARUM_CLAUSURA",
    "CHORDA_GEMINA",
    "CHORDA_GEMINA_MULTA",
    "CHORDA_SIMPLEX",
    "CHORDA_SIMPLEX_MULTA",
    "NUMERUS",
    "TEMPUS",
    "VERUM",
    "FALSUM",
    "SERIES_APERTURA",
    "SERIES_CLAUSURA",
    "COMPACTA_APERTURA",
    "COMPACTA_CLAUSURA",
    "COMMA",
    "LINEA_FINIS",
    "SPATIUM",
    "COMMENTUM",
    "LINEA",
    "IGNOTUM"
};

/* Tituli generum nodorum, ordine declarationis */
hic_manens constans character* GENERA_EXSPECTATA[] = {
    "documentum",
    "caput-tabulae",
    "caput-seriei",
    "par",
    "clavis",
    "chorda",
    "numerus",
    "boolean",
    "tempus",
    "series",
    "tabula-compacta",
    "comma",
    "linea",
    "malum"
};

nomen structura {
                    s32  genus;
                    i32  locus;
     constans character* titulus;
} LocusNominatus;

/* Omnis locus omnis generis: enumeratio toml_registrum.h contra
 * titulum tabulae generatae */
hic_manens constans LocusNominatus LOCI_NOMINATI[] = {
    { (s32)TOML_GENUS_DOCUMENTUM,
        (i32)TOML_DOCUMENTUM_LIBERI, "liberi" },
    { (s32)TOML_GENUS_DOCUMENTUM,
        (i32)TOML_DOCUMENTUM_CAUDA, "cauda" },
    { (s32)TOML_GENUS_CAPUT_TABULAE,
        (i32)TOML_CAPUT_TOK_APERTURA, "tok_apertura" },
    { (s32)TOML_GENUS_CAPUT_TABULAE,
        (i32)TOML_CAPUT_CLAVIS, "clavis" },
    { (s32)TOML_GENUS_CAPUT_TABULAE,
        (i32)TOML_CAPUT_TOK_CLAUSURA, "tok_clausura" },
    { (s32)TOML_GENUS_CAPUT_SERIEI,
        (i32)TOML_CAPUT_TOK_APERTURA, "tok_apertura" },
    { (s32)TOML_GENUS_CAPUT_SERIEI,
        (i32)TOML_CAPUT_CLAVIS, "clavis" },
    { (s32)TOML_GENUS_CAPUT_SERIEI,
        (i32)TOML_CAPUT_TOK_CLAUSURA, "tok_clausura" },
    { (s32)TOML_GENUS_PAR,
        (i32)TOML_PAR_CLAVIS, "clavis" },
    { (s32)TOML_GENUS_PAR,
        (i32)TOML_PAR_TOK_SIGNUM, "tok_signum" },
    { (s32)TOML_GENUS_PAR,
        (i32)TOML_PAR_VALOR, "valor" },
    { (s32)TOML_GENUS_CLAVIS,
        (i32)TOML_CLAVIS_PARTES, "partes" },
    { (s32)TOML_GENUS_CHORDA,
        (i32)TOML_LEXEMA_TOK, "tok" },
    { (s32)TOML_GENUS_NUMERUS,
        (i32)TOML_LEXEMA_TOK, "tok" },
    { (s32)TOML_GENUS_BOOLEAN,
        (i32)TOML_LEXEMA_TOK, "tok" },
    { (s32)TOML_GENUS_TEMPUS,
        (i32)TOML_LEXEMA_TOK, "tok" },
    { (s32)TOML_GENUS_SERIES,
        (i32)TOML_INCLUSA_TOK_APERTURA, "tok_apertura" },
    { (s32)TOML_GENUS_SERIES,
        (i32)TOML_INCLUSA_LIBERI, "liberi" },
    { (s32)TOML_GENUS_SERIES,
        (i32)TOML_INCLUSA_TOK_CLAUSURA, "tok_clausura" },
    { (s32)TOML_GENUS_TABULA_COMPACTA,
        (i32)TOML_INCLUSA_TOK_APERTURA, "tok_apertura" },
    { (s32)TOML_GENUS_TABULA_COMPACTA,
        (i32)TOML_INCLUSA_LIBERI, "liberi" },
    { (s32)TOML_GENUS_TABULA_COMPACTA,
        (i32)TOML_INCLUSA_TOK_CLAUSURA, "tok_clausura" },
    { (s32)TOML_GENUS_COMMA,
        (i32)TOML_LEXEMA_TOK, "tok" },
    { (s32)TOML_GENUS_LINEA,
        (i32)TOML_LEXEMA_TOK, "tok" },
    { (s32)TOML_GENUS_MALUM,
        (i32)TOML_MALUM_TOKENS, "tokens" }
};

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
MateriaLexiconRatum  ratum;
 MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_toml_registrum",
        65536);
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

        imprimere("\n--- Probans lexicon toml ---\n");

        CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &TOML_LEXICON,
            &iudicium));
        CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);

        CREDO_AEQUALIS_I32 (TOML_LEXICON.numerus_generum,
            (i32)TOML_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)TOML_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)TOML_LEX_NUMERUS_GENERUM, (i32)XXVIII);

        /* ORDO per titulos - permutatio capitur */
        per (i = ZEPHYRUM; i < (i32)TOML_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(&ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
        }

        /* Sedes notae */
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)TOML_LEX_SERIES_TABULARUM_APERTURA),
            (s32)MATERIA_LEX_FIXUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)TOML_LEX_SERIES_TABULARUM_APERTURA), "[[")
                == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(&ratum,
            (s32)TOML_LEX_FALSUM), "false") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)TOML_LEX_LINEA_FINIS), (s32)MATERIA_LEX_TERMINATOR);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(&ratum,
            (s32)TOML_LEX_LINEA), (s32)MATERIA_LEX_TERMINATOR);
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)TOML_LEX_CLAVIS_NUDA));
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)TOML_LEX_NUMERUS));
        CREDO_VERUM (materia_lexicon_textum_fert(&ratum,
            (s32)TOML_LEX_IGNOTUM));
        CREDO_FALSUM (materia_lexicon_textum_fert(&ratum,
            (s32)TOML_LEX_COMMA));
    }


    /* ==================================================
     * PROBARE: linea nova bis, trivia, munus LINEA ADEST
     * ================================================== */

    {
        i32 postulata;

        imprimere("\n--- Probans trivia et lineam novam bis ---\n");

        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)TOML_LEX_SPATIUM));
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)TOML_LEX_COMMENTUM));
        CREDO_VERUM (materia_lexicon_trivium_est(&ratum,
            (s32)TOML_LEX_LINEA));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)TOML_LEX_LINEA_FINIS));
        CREDO_FALSUM (materia_lexicon_trivium_est(&ratum,
            (s32)TOML_LEX_IGNOTUM));
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (s32)TOML_LEX_LINEA), (s32)MATERIA_MUNUS_LINEA);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_munus(&ratum,
            (s32)TOML_LEX_LINEA_FINIS),
            (s32)MATERIA_MUNUS_SUBSTANTIVUM);

        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_SPATIUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_COMMENTUM));
        CREDO_VERUM (materia_lexicon_munus_habet(&ratum,
            MATERIA_MUNUS_FINIS));

        postulata = MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_SPATIUM)
                  | MATERIA_MUNUS_VEXILLUM(MATERIA_MUNUS_LINEA);
        CREDO_VERUM (materia_lexicon_munera_habet(&ratum, postulata));
    }


    /* ==================================================
     * PROBARE: porta rancoris tabularum coctarum
     * ================================================== */

    {
        constans character* radix = getenv("RHUBARB_RADIX");
             MateriaRancor  rancor;

        imprimere("\n--- Probans rancorem tabularum coctarum ---\n");
        si (!materia_registrum_recens(piscina, radix != NIHIL ? radix
            : ".", "toml/grammatica/toml.registrum.stml", &rancor))
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
                TOML_REGISTRUM.numerus_locorum);
            CREDO_AEQUALIS_I32 (rancor.coctio.numerus_generum,
                TOML_REGISTRUM.numerus_generum);
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

        CREDO_AEQUALIS_I32 (TOML_REGISTRUM.numerus_generum,
            (i32)TOML_GENUS_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)TOML_GENUS_NUMERUS_GENERUM, (i32)XIV);
        CREDO_AEQUALIS_I32 (TOML_REGISTRUM.numerus_locorum, (i32)XXV);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])),
            (i32)TOML_GENUS_NUMERUS_GENERUM);

        per (i = ZEPHYRUM; i < (i32)TOML_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_NON_NIHIL (TOML_REGISTRUM.genera[i].titulus);
            CREDO_VERUM (strcmp(TOML_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(
                &TOML_REGISTRUM, GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
        }

        exspectatus_offset = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)TOML_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_AEQUALIS_I32 (TOML_REGISTRUM.genera[i].loci_offset,
                exspectatus_offset);
            CREDO_MAIOR_I32 (TOML_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            exspectatus_offset += TOML_REGISTRUM.genera[i].loci_numerus;
        }
        CREDO_AEQUALIS_I32 (exspectatus_offset,
            TOML_REGISTRUM.numerus_locorum);

        numerus_nominatorum = (i32)(magnitudo(LOCI_NOMINATI)
            / magnitudo(LOCI_NOMINATI[0]));
        CREDO_AEQUALIS_I32 (numerus_nominatorum,
            TOML_REGISTRUM.numerus_locorum);
        per (i = ZEPHYRUM; i < numerus_nominatorum; i++)
        {
            constans LocusNominatus* n = &LOCI_NOMINATI[i];
                                i32  absolutus;

            CREDO_MINOR_I32 (n->locus,
                TOML_REGISTRUM.genera[n->genus].loci_numerus);
            absolutus = TOML_REGISTRUM.genera[n->genus].loci_offset
                      + n->locus;
            CREDO_VERUM (strcmp(TOML_REGISTRUM.loci[absolutus].titulus,
                n->titulus) == ZEPHYRUM);
        }
    }


    /* ==================================================
     * PROBARE: circuitus per materiam (arbor minima 'a = 1')
     * ================================================== */

    {
        MateriaArborConsilium c;
                 MateriaNodus* documentum;
                 MateriaNodus* par;
                 MateriaNodus* clavis;
                 MateriaNodus* numerus;
        MateriaArborScriptura  s1;
        MateriaArborScriptura  s2;
                 MateriaNodus* lecta;
           MateriaArborVitium  vitium;

        imprimere("\n--- Probans circuitum toml per materiam ---\n");

        clavis = materia_nodus_creare(piscina, (s32)TOML_GENUS_CLAVIS,
            TOML_REGISTRUM.genera[TOML_GENUS_CLAVIS].loci_numerus);
        CREDO_VERUM (materia_nodus_appendere(piscina, clavis,
            (i32)TOML_CLAVIS_PARTES, materia_valor_token(_token(piscina,
                (s32)TOML_LEX_CLAVIS_NUDA, "a", ZEPHYRUM)),
            MATERIA_LOCUS_LISTA_TOKEN));
        numerus = materia_nodus_creare(piscina, (s32)TOML_GENUS_NUMERUS,
            TOML_REGISTRUM.genera[TOML_GENUS_NUMERUS].loci_numerus);
        CREDO_VERUM (materia_nodus_ponere(numerus, (i32)TOML_LEXEMA_TOK,
            materia_valor_token(_token(piscina, (s32)TOML_LEX_NUMERUS,
                "1", (s32)IV)), MATERIA_LOCUS_TOKEN));
        par = materia_nodus_creare(piscina, (s32)TOML_GENUS_PAR,
            TOML_REGISTRUM.genera[TOML_GENUS_PAR].loci_numerus);
        CREDO_VERUM (materia_nodus_ponere(par, (i32)TOML_PAR_CLAVIS,
            materia_valor_nodus(clavis), MATERIA_LOCUS_NODUS));
        CREDO_VERUM (materia_nodus_ponere(par, (i32)TOML_PAR_TOK_SIGNUM,
            materia_valor_token(_token(piscina, (s32)TOML_LEX_SIGNUM,
                "=", (s32)II)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(par, (i32)TOML_PAR_VALOR,
            materia_valor_nodus(numerus), MATERIA_LOCUS_NODUS));
        documentum = materia_nodus_creare(piscina,
            (s32)TOML_GENUS_DOCUMENTUM,
            TOML_REGISTRUM.genera[TOML_GENUS_DOCUMENTUM].loci_numerus);
        CREDO_VERUM (materia_nodus_appendere(piscina, documentum,
            (i32)TOML_DOCUMENTUM_LIBERI, materia_valor_nodus(par),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(documentum,
            (i32)TOML_DOCUMENTUM_CAUDA, materia_valor_token(_token(
                piscina, (s32)TOML_LEX_FINIS, "", (s32)V)),
            MATERIA_LOCUS_TOKEN));

        materia_arbor_consilium_nudum(&c, &TOML_REGISTRUM, &ratum,
            "toml");
        s1 = materia_arbor_scribere_nodum(piscina, documentum, &c);
        CREDO_VERUM (s1.successus);
        CREDO_VERUM (_textus_continet(s1.textus,
            "grammatica=\"toml\""));
        CREDO_VERUM (_textus_continet(s1.textus, "<toml-clavis-nuda"));
        CREDO_VERUM (_textus_continet(s1.textus, "<par"));

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, &c,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        si (lecta != NIHIL)
        {
            CREDO_AEQUALIS_S32 (lecta->genus,
                (s32)TOML_GENUS_DOCUMENTUM);
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
