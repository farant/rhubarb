/* probatio_css_arbor.c - Spina plagulae (T9)
 *
 * TRES ORDINES PROBATIONIS, consulto distincti (spec par. IX.4):
 *
 *  I. STRUCTURA   - nodus radicis rectus, loci recti
 * II. OCTETI      - emissio DIRECTA ex parsura (materia_scribere).
 *                   ORACULUM SEPARANS: si porta STML postea cadat,
 *                   haec dicet utrum analyzator an proiectio
 *                   culpanda sit. Sine ea vitium parsatoris et
 *                   vitium proiectionis numerus unus indistinctus
 *                   sunt.
 *III. STML        - circuitus bis (scribere->legere->scribere),
 *                   quia vitium quod COMPOUNDIT circuitum unum
 *                   saepe superat.
 *
 * INCOMPLETUM NOMINATUM: spina regulas NONDUM parsat (T10). Id
 * ASSERITUR, non tacetur - vide '_tegumentum NONDUM' infra. Porta
 * quae rubra esse DEBET et viridis est peior quam nulla porta.
 */

#include "latina.h"
#include "credo.h"
#include "css_arbor.h"
#include "css_registrum.h"
#include "css_lexicon.h"
#include "css_lexema.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Locus NOMINATUS: enumeratio css_registrum.h contra TITULOS
 * tabulae. Sine hac probatione enumerationes tertia tabula manu
 * scripta essent, quae per se labi posset - et lapsus TACITUS
 * (valor in locum alienum poneretur). */
nomen structura {
                    s32 genus;
                    i32 locus;
    constans character* titulus;
} LocusNominatus;

hic_manens constans LocusNominatus LOCI_NOMINATI[] = {
    { (s32)CSS_GENUS_PLAGULA,            (i32)CSS_PLAGULA_REGULAE,        "regulae" },
    { (s32)CSS_GENUS_PLAGULA,            (i32)CSS_PLAGULA_CAUDA,          "cauda" },
    { (s32)CSS_GENUS_REGULA_QUALIFICATA, (i32)CSS_REGULA_PRAELUDIUM,      "praeludium" },
    { (s32)CSS_GENUS_REGULA_QUALIFICATA, (i32)CSS_REGULA_CORPUS,          "corpus" },
    { (s32)CSS_GENUS_REGULA_APUD,        (i32)CSS_APUD_TOK_NOMEN,         "tok_nomen" },
    { (s32)CSS_GENUS_REGULA_APUD,        (i32)CSS_APUD_PRAELUDIUM,        "praeludium" },
    { (s32)CSS_GENUS_REGULA_APUD,        (i32)CSS_APUD_CORPUS,            "corpus" },
    { (s32)CSS_GENUS_REGULA_APUD,        (i32)CSS_APUD_TOK_TERMINATOR,    "tok_terminator" },
    { (s32)CSS_GENUS_DECLARATIO,         (i32)CSS_DECL_TOK_NOMEN,         "tok_nomen" },
    { (s32)CSS_GENUS_DECLARATIO,         (i32)CSS_DECL_TOK_COLON,         "tok_colon" },
    { (s32)CSS_GENUS_DECLARATIO,         (i32)CSS_DECL_VALOR,             "valor" },
    { (s32)CSS_GENUS_DECLARATIO,         (i32)CSS_DECL_PRAEVALENTIA,      "praevalentia" },
    { (s32)CSS_GENUS_DECLARATIO,         (i32)CSS_DECL_TOK_TERMINATOR,    "tok_terminator" },
    { (s32)CSS_GENUS_PRAEVALENTIA,       (i32)CSS_PRAEVALENTIA_TOK_CLAMOR,"tok_clamor" },
    { (s32)CSS_GENUS_PRAEVALENTIA,       (i32)CSS_PRAEVALENTIA_TOK_VERBUM,"tok_verbum" },
    { (s32)CSS_GENUS_LEXEMA_SERVATUM,    (i32)CSS_SERVATUM_TOK,           "tok" },
    { (s32)CSS_GENUS_FUNCTIO,            (i32)CSS_FUNCTIO_TOK_NOMEN,      "tok_nomen" },
    { (s32)CSS_GENUS_FUNCTIO,            (i32)CSS_FUNCTIO_ARGUMENTA,      "argumenta" },
    { (s32)CSS_GENUS_FUNCTIO,            (i32)CSS_FUNCTIO_TOK_CLAUSUM,    "tok_clausum" },
    { (s32)CSS_GENUS_SAEPTUM,            (i32)CSS_SAEPTUM_TOK_APERTUM,    "tok_apertum" },
    { (s32)CSS_GENUS_SAEPTUM,            (i32)CSS_SAEPTUM_CONTENTUM,      "contentum" },
    { (s32)CSS_GENUS_SAEPTUM,            (i32)CSS_SAEPTUM_TOK_CLAUSUM,    "tok_clausum" },
    { (s32)CSS_GENUS_REGULA_MALA,        (i32)CSS_MALUM_TOKENS,           "tokens" },
    { (s32)CSS_GENUS_DECLARATIO_MALA,    (i32)CSS_MALUM_TOKENS,           "tokens" }
};


/* ORACULUM SEPARANS: emissio DIRECTA ex parsura, sine STML. */
hic_manens b32
_octetos_probare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    MateriaNodus* radix;
    MateriaScriptura emissa;
    MateriaScripturaConsilium consilium;

    radix = css_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL) { redde FALSUM; }

    materia_scriptura_consilium_nudum(&consilium, &CSS_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    si (!emissa.successus) { redde FALSUM; }
    si (emissa.textus.mensura != mensura) { redde FALSUM; }
    si (mensura == ZEPHYRUM) { redde VERUM; }
    redde (b32)(memcmp(emissa.textus.datum, fons,
                       (size_t)mensura) == ZEPHYRUM);
}


integer
principale (vacuum)
{
    Piscina* piscina;
    MateriaLexiconRatum ratum;
    MateriaLexIudicium iudicium;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_css_arbor", 65536);
    credo_aperire(piscina);

    si (!materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON, &iudicium))
    {
        imprimere("LEXICON CSS RECUSATUM\n");
        redde I;
    }


    /* ========================================================
     * CUSTODIA: loci nominati contra TITULOS tabulae
     * ======================================================== */
    {
        i32 numerus = (i32)(magnitudo(LOCI_NOMINATI)
                          / magnitudo(LOCI_NOMINATI[0]));
        i32 i;

        imprimere("\n--- Probans locos nominatos ---\n");

        /* OMNIS locus tabulae nomen habet - aliter parsator numerum
         * nudum pro loco innominato scriberet et custodia eum non
         * videret. */
        CREDO_AEQUALIS_I32 (numerus, CSS_REGISTRUM.numerus_locorum);

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            constans MateriaTabGenus* genus;
            constans character* titulus;

            genus = &CSS_REGISTRUM.genera[LOCI_NOMINATI[i].genus];
            CREDO_MINOR_I32 (LOCI_NOMINATI[i].locus,
                genus->loci_numerus);
            titulus = CSS_REGISTRUM.loci[genus->loci_offset
                + LOCI_NOMINATI[i].locus].titulus;
            CREDO_VERUM (strcmp(titulus,
                LOCI_NOMINATI[i].titulus) == ZEPHYRUM);
        }
    }


    /* ========================================================
     * STRUCTURA: casus degeneres
     * ======================================================== */
    {
        MateriaNodus* radix;

        imprimere("\n--- Probans plagulam vacuam ---\n");
        {
            radix = css_arbor_parsare(piscina, "", ZEPHYRUM);
            CREDO_NON_NIHIL (radix);
            CREDO_AEQUALIS_S32 (radix->genus, (s32)CSS_GENUS_PLAGULA);
            CREDO_AEQUALIS_I32 (radix->numerus_locorum, II);

            /* Regulae: lista VACUA, non locus vacuus. Distinctio
             * consulta - vide css_arbor.c. */
            CREDO_AEQUALIS_S32 (
                (s32)radix->loci[CSS_PLAGULA_REGULAE].genus,
                (s32)MATERIA_VALOR_LISTA);
            CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
                radix->loci[CSS_PLAGULA_REGULAE]), ZEPHYRUM);

            /* Cauda ADEST etiam in plagula vacua - lexema FINIS
             * semper exstat (spec par. VII). */
            CREDO_AEQUALIS_S32 (
                (s32)radix->loci[CSS_PLAGULA_CAUDA].genus,
                (s32)MATERIA_VALOR_TOKEN);
            CREDO_AEQUALIS_S32 (
                radix->loci[CSS_PLAGULA_CAUDA].datum.token->genus,
                (s32)CSS_LEX_FINIS);
        }

        imprimere("\n--- Probans plagulam spatiorum solorum ---\n");
        {
            constans character* fons = "\n\n  ";
            MateriaToken* cauda;

            radix = css_arbor_parsare(piscina, fons,
                (i32)strlen(fons));
            CREDO_NON_NIHIL (radix);
            CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
                radix->loci[CSS_PLAGULA_REGULAE]), ZEPHYRUM);

            /* Spatia in caudam eunt ut 'ante' lexematis FINIS -
             * prior NIHIL est, ergo divisio ZEPHYRUM. Dominus
             * unus, nullus octetus perditus. */
            cauda = radix->loci[CSS_PLAGULA_CAUDA].datum.token;
            CREDO_AEQUALIS_I32 (cauda->numerus_ante, I);
            CREDO_AEQUALIS_I32 (cauda->numerus_post, ZEPHYRUM);
            CREDO_AEQUALIS_S32 (cauda->spatia_ante[ZEPHYRUM]->genus,
                (s32)CSS_LEX_SPATIA);
        }

        imprimere("\n--- Probans plagulam commentarii solius ---\n");
        {
            constans character* fons = "/* solum */\n";
            MateriaToken* cauda;

            radix = css_arbor_parsare(piscina, fons,
                (i32)strlen(fons));
            CREDO_NON_NIHIL (radix);
            cauda = radix->loci[CSS_PLAGULA_CAUDA].datum.token;
            /* COMMENTARIUM + SPATIA("\n") = II ante */
            CREDO_AEQUALIS_I32 (cauda->numerus_ante, II);
            CREDO_AEQUALIS_S32 (cauda->spatia_ante[ZEPHYRUM]->genus,
                (s32)CSS_LEX_COMMENTARIUM);
        }
    }


    /* ========================================================
     * OCTETI: oraculum separans (par. IX.4)
     * ======================================================== */
    {
        hic_manens constans character* DEGENERES[] = {
            "",
            " ",
            "\n\n  ",
            "\t",
            "/* solum */",
            "/* solum */\n",
            "/*",
            "\r\n"
        };
        i32 numerus = (i32)(magnitudo(DEGENERES)
                          / magnitudo(DEGENERES[0]));
        i32 i;
        i32 probata = ZEPHYRUM;

        imprimere("\n--- ORACULUM: octeti ex parsura DIRECTE ---\n");

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            CREDO_VERUM (_octetos_probare(piscina, DEGENERES[i],
                (i32)strlen(DEGENERES[i])));
            probata = probata + I;
        }
        CREDO_MAIOR_S32 ((s32)probata, (s32)V);
    }


    /* ========================================================
     * REGULAE QUALIFICATAE (B2) - pinnae T9 VERSAE
     * ======================================================== */
    {
        imprimere("\n--- Probans circuitum octetorum, regulae ---\n");

        /* Hae duae CREDO_FALSUM natae sunt (T9): porta rubra donec
         * regulae parsarentur. B2 eas vertit - id ipsum quod pinna
         * rubra promittebat. */
        CREDO_VERUM (_octetos_probare(piscina, "a{}", III));
        CREDO_VERUM (_octetos_probare(piscina, "a{color:red}",
            XII));
        CREDO_VERUM (_octetos_probare(piscina,
            "a , b > c { color : red ; }", XXVII));
        CREDO_VERUM (_octetos_probare(piscina, "\n/* c */\na{}\n",
            XIII));
        CREDO_VERUM (_octetos_probare(piscina, "a b{}", V));

        imprimere("\n--- Probans praeludium CONTENTUM (D7) ---\n");
        {
            /* 'a b{}' - praeludium III elementa: a, SPATIUM, b.
             * Spatium NODUS est (lexema-servatum), non trivium -
             * intra praeludia trivia NON exsistunt (D7 auctum, vide
             * css_arbor.c). */
            MateriaNodus* plagula = css_arbor_parsare(piscina,
                "a b{}", V);
            MateriaNodus* regula;
            MateriaNodus* servatum;
            MateriaValor  regulae;
            MateriaValor  praeludium;

            CREDO_NON_NIHIL (plagula);
            regulae = plagula->loci[CSS_PLAGULA_REGULAE];
            CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(regulae),
                I);
            regula = materia_valor_lista_obtinere(regulae,
                ZEPHYRUM)->datum.nodus;
            CREDO_AEQUALIS_S32 (regula->genus,
                (s32)CSS_GENUS_REGULA_QUALIFICATA);
            praeludium = regula->loci[CSS_REGULA_PRAELUDIUM];
            CREDO_AEQUALIS_I32 (
                materia_valor_lista_numerus(praeludium), III);

            /* Elementum medium: servatum SPATII, SINE triviis */
            servatum = materia_valor_lista_obtinere(praeludium,
                I)->datum.nodus;
            CREDO_AEQUALIS_S32 (servatum->genus,
                (s32)CSS_GENUS_LEXEMA_SERVATUM);
            CREDO_AEQUALIS_S32 (
                servatum->loci[CSS_SERVATUM_TOK].datum.token->genus,
                (s32)CSS_LEX_SPATIA);
            CREDO_AEQUALIS_I32 (servatum->loci[CSS_SERVATUM_TOK]
                .datum.token->numerus_ante, ZEPHYRUM);

            /* Corpus: saeptum */
            CREDO_AEQUALIS_S32 (
                regula->loci[CSS_REGULA_CORPUS].datum.nodus->genus,
                (s32)CSS_GENUS_SAEPTUM);
        }
    }


    /* ========================================================
     * STML: circuitus BIS
     * ======================================================== */
    {
        hic_manens constans character* DEGENERES[] = {
            "",
            "\n\n  ",
            "/* solum */\n"
        };
        i32 numerus = (i32)(magnitudo(DEGENERES)
                          / magnitudo(DEGENERES[0]));
        i32 i;

        imprimere("\n--- Probans circuitum STML (bis) ---\n");

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            MateriaNodus* radix;
            MateriaNodus* lecta;
            MateriaNodus* relecta;
            MateriaArborConsilium consilium;
            MateriaArborVitium vitium;
            MateriaArborScriptura s1;
            MateriaArborScriptura s2;
            MateriaArborScriptura s3;
            i32 mensura = (i32)strlen(DEGENERES[i]);

            radix = css_arbor_parsare(piscina, DEGENERES[i], mensura);
            CREDO_NON_NIHIL (radix);

            materia_arbor_consilium_nudum(&consilium, &CSS_REGISTRUM,
                &ratum, "css");
            s1 = materia_arbor_scribere_nodum(piscina, radix,
                &consilium);
            CREDO_VERUM (s1.successus);

            lecta = materia_arbor_legere(piscina, NIHIL, s1.textus,
                &consilium, &vitium);
            CREDO_NON_NIHIL (lecta);
            CREDO_NIHIL (vitium.causa);

            s2 = materia_arbor_scribere_nodum(piscina, lecta,
                &consilium);
            CREDO_VERUM (s2.successus);
            CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
            CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
                (size_t)s1.textus.mensura) == ZEPHYRUM);

            /* CIRCUITUS SECUNDUS: vitium quod COMPOUNDIT (positio
             * uno errans, trivium omissum) circuitum unum saepe
             * superat - primus formam documenti reddit, secundus
             * formam formae. */
            relecta = materia_arbor_legere(piscina, NIHIL, s2.textus,
                &consilium, &vitium);
            CREDO_NON_NIHIL (relecta);
            s3 = materia_arbor_scribere_nodum(piscina, relecta,
                &consilium);
            CREDO_VERUM (s3.successus);
            CREDO_AEQUALIS_I32 (s3.textus.mensura, s1.textus.mensura);
            CREDO_VERUM (memcmp(s1.textus.datum, s3.textus.datum,
                (size_t)s1.textus.mensura) == ZEPHYRUM);

            /* ARBOR quoque servatur, non octeti soli. Locus
             * 'regulae' LISTA manere DEBET - si scriptor listam
             * vacuam omittat et lector NIHIL reddat, octeti IDEM
             * essent et arbor MUTATA. Discrimen quod comparator
             * documentorum videre NON potest. */
            CREDO_AEQUALIS_S32 (
                (s32)lecta->loci[CSS_PLAGULA_REGULAE].genus,
                (s32)radix->loci[CSS_PLAGULA_REGULAE].genus);
            CREDO_AEQUALIS_S32 (
                (s32)lecta->loci[CSS_PLAGULA_CAUDA].genus,
                (s32)MATERIA_VALOR_TOKEN);

            /* Octeti ex arbore LECTA quoque fontem reddunt */
            {
                MateriaScriptura emissa;
                MateriaScripturaConsilium cs;

                materia_scriptura_consilium_nudum(&cs, &CSS_REGISTRUM);
                emissa = materia_scribere_nodum(piscina, lecta, &cs);
                CREDO_VERUM (emissa.successus);
                CREDO_AEQUALIS_I32 (emissa.textus.mensura, mensura);
            }
        }
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
