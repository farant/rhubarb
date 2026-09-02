/* probatio_css_stml.c - Circuitus proiectionis STML (B6, spec IX.3+8)
 *
 * Scribere -> legere -> scribere, BIS: vitium quod se componit
 * (positio uno errans, trivium omissum) circuitum unum saepe
 * superat - primus formam documenti reddit, secundus formam formae.
 *
 * DUO ORACULA, classes DISIUNCTAE:
 *   octeti      (materia_scribere contra fontem) - quod perditur
 *   comparator  (B1, STRUCTURALIS) - quod MIGRAT sine iactura:
 *               commentarium ex 'post' prioris in 'ante' sequentis
 *               fluxum octetorum EXACTE servat. Sectio 'discrimen
 *               oraculorum' infra id MENSURAT, non asserit.
 *
 * FIDELITAS hic currere NON potest, et causa capacitas est, non
 * vitium: lector lineas ex cursore reficit quem solum munus LINEA
 * movet, et CSS munus LINEA non habet (spatia unum genus). Positio
 * ex octetis derivabilis manet; documentum offset stalum numquam
 * fert (lex positionum). STRUCTURALIS asseritur.
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
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* CORPUS[] = {
    "probationes/fixa/css/componentia_2026-08-18.css",
    "probationes/fixa/css/thema_2026-08-18.css",
    "probationes/fixa/css/adversarius.css",
    "probationes/fixa/css/adversarius_2.css",
    "lib/speculum_assets/speculum.css",
    "knotapel/atlas/web/style.css"
};

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }

    *mensura = (i32)longitudo;
    redde memoria;
}

/* Circuitus plenus super fontem unum. VERUM = omnia sana; causa
 * imprimitur ante FALSUM ut rubor nominatus sit. */
interior b32
_circuitum_probare (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura)
{
              MateriaNodus* radix;
              MateriaNodus* lecta;
              MateriaNodus* relecta;
      MateriaArborScriptura s1;
      MateriaArborScriptura s2;
      MateriaArborScriptura s3;
         MateriaArborVitium vitium;
    MateriaArborDifferentia d;

    radix = css_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        imprimere("    (parsura NIHIL)\n");
        redde FALSUM;
    }

    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        imprimere("    (scriptura I: %s)\n", s1.causa ? s1.causa : "-");
        redde FALSUM;
    }

    /* Compressio MORTUA pro CSS (spec par. III): praesentia
     * asseritur, numquam pinnae. */
    si (s1.census.spatia_vocationes != ZEPHYRUM)
    {
        imprimere("    (compressio viva?!)\n");
        redde FALSUM;
    }

    lecta = materia_arbor_legere(piscina, NIHIL, s1.textus,
        consilium, &vitium);
    si (lecta == NIHIL)
    {
        imprimere("    (lectio I: %s, linea %d)\n",
            vitium.causa ? vitium.causa : "-", (integer)vitium.linea);
        redde FALSUM;
    }

    s2 = materia_arbor_scribere_nodum(piscina, lecta, consilium);
    si (!s2.successus)
    {
        imprimere("    (scriptura II: %s)\n",
            s2.causa ? s2.causa : "-");
        redde FALSUM;
    }
    si (   s1.textus.mensura != s2.textus.mensura
        || memcmp(s1.textus.datum, s2.textus.datum,
               (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (documenta I/II dispari: %d contra %d)\n",
            (integer)s1.textus.mensura, (integer)s2.textus.mensura);
        redde FALSUM;
    }

    /* CIRCUITUS SECUNDUS */
    relecta = materia_arbor_legere(piscina, NIHIL, s2.textus,
        consilium, &vitium);
    si (relecta == NIHIL)
    {
        imprimere("    (lectio II: %s)\n",
            vitium.causa ? vitium.causa : "-");
        redde FALSUM;
    }
    s3 = materia_arbor_scribere_nodum(piscina, relecta, consilium);
    si (   !s3.successus
        || s1.textus.mensura != s3.textus.mensura
        || memcmp(s1.textus.datum, s3.textus.datum,
               (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (circuitus II dispar)\n");
        redde FALSUM;
    }

    /* ORACULUM ARBORIS (B1): dislocatio dominii octetim invisibilis */
    si (!materia_arbor_aequalis(radix, lecta,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d))
    {
        imprimere("    (arbor dispar: %s @ %s)\n",
            d.campus ? d.campus : "-", d.via);
        redde FALSUM;
    }

    /* Octeti ex arbore LECTA quoque fontem reddunt (lectio->emissio) */
    {
                 MateriaScriptura emissa;
        MateriaScripturaConsilium cs;

        materia_scriptura_consilium_nudum(&cs, &CSS_REGISTRUM);
        emissa = materia_scribere_nodum(piscina, lecta, &cs);
        si (   !emissa.successus
            || emissa.textus.mensura != mensura
            || (mensura > ZEPHYRUM
                && memcmp(emissa.textus.datum, fons,
                       (size_t)mensura) != ZEPHYRUM))
        {
            imprimere("    (lecta->octeti dispari)\n");
            redde FALSUM;
        }
    }
    redde VERUM;
}

integer
principale (vacuum)
{
                    Piscina* piscina;
         constans character* radix_viae;
        MateriaLexiconRatum  ratum;
         MateriaLexIudicium  iudicium;
      MateriaArborConsilium  consilium;
                        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_css_stml",
        4194304);
    credo_aperire(piscina);

    si (!materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON, &iudicium))
    {
        imprimere("LEXICON CSS RECUSATUM\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium, &CSS_REGISTRUM, &ratum,
        "css");


    /* ========================================================
     * CASUS INLINEI: quaeque forma nodi per proiectionem
     * ======================================================== */

    {
        hic_manens constans character* CASUS[] = {
            "a{color:red}",
            "a , b > c { color : red ; }",
            "a{margin:1px /* c */ 2px}",
            "a{color:red ! important ;}",
            "@media screen{a{color:red}}",
            "@import url(a.css);",
            "@page{@top-left{}}",
            "}a{}",
            "<!--a{}-->",
            "a{color red}",
            "a{b:\"unterminated"
        };
        i32 numerus = (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));
        i32 i;

        imprimere("\n--- Probans circuitum STML inlineum ---\n");

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            CREDO_VERUM (_circuitum_probare(piscina, &consilium,
                CASUS[i], (i32)strlen(CASUS[i])));
        }
    }


    /* ========================================================
     * DISCRIMEN ORACULORUM: migratio dominii MENSURATA
     * ======================================================== */

    {
        /* Trivium ex 'post' prioris in 'ante' sequentis motum:
         * octeti IDEM manent (emissio ordinem fontis servat),
         * arbor MUTATA est. Haec est classis tota ob quam
         * comparator B1 portatus est - hic mensuratur, non
         * asseritur. */
        MateriaNodus* a;
        MateriaNodus* b;
        MateriaToken* cauda_b;
        MateriaToken* trivium;
        MateriaScriptura ea;
        MateriaScriptura eb;
        MateriaScripturaConsilium cs;
        MateriaArborDifferentia d;

        imprimere("\n--- Probans discrimen oraculorum ---\n");

        a = css_arbor_parsare(piscina, "a{} ", IV);
        b = css_arbor_parsare(piscina, "a{} ", IV);
        CREDO_NON_NIHIL (a);
        CREDO_NON_NIHIL (b);

        /* In 'a{} ' spatium caudale ad '}' ut POST ligatur (divisio
         * sine linea nova omnia PRIORI dat - MENSURATUM: versio
         * prima huius probationis contrarium credidit et rubuit).
         * Migratio: ex '}'.post in cauda.ante. */
        cauda_b = b->loci[CSS_PLAGULA_CAUDA].datum.token;
        CREDO_AEQUALIS_I32 (cauda_b->numerus_ante, ZEPHYRUM);
        {
            MateriaNodus* regula = materia_valor_lista_obtinere(
                b->loci[CSS_PLAGULA_REGULAE], ZEPHYRUM)->datum.nodus;
            MateriaNodus* corpus =
                regula->loci[CSS_REGULA_CORPUS].datum.nodus;
            MateriaToken* clausum = corpus
                ->loci[CSS_SAEPTUM_TOK_CLAUSUM].datum.token;

            CREDO_AEQUALIS_I32 (clausum->numerus_post, I);
            trivium                = clausum->spatia_post[ZEPHYRUM];
            clausum->numerus_post  = ZEPHYRUM;
            CREDO_VERUM (materia_token_trivia_ante_ponere(cauda_b,
                piscina, &trivium, (i32)I));
        }

        /* Octeti: IDEM - migratio ordinem fontis servat */
        materia_scriptura_consilium_nudum(&cs, &CSS_REGISTRUM);
        ea = materia_scribere_nodum(piscina, a, &cs);
        eb = materia_scribere_nodum(piscina, b, &cs);
        CREDO_VERUM (ea.successus);
        CREDO_VERUM (eb.successus);
        CREDO_AEQUALIS_I32 (ea.textus.mensura, eb.textus.mensura);
        CREDO_VERUM (memcmp(ea.textus.datum, eb.textus.datum,
            (size_t)ea.textus.mensura) == ZEPHYRUM);

        /* Arbor: DISPAR - solus comparator id videt */
        CREDO_FALSUM (materia_arbor_aequalis(a, b,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
        CREDO_NON_NIHIL (d.campus);
    }


    /* ========================================================
     * CORPUS TOTUM per circuitum, se metiens
     * ======================================================== */

    {
        i32 numerus = (i32)(magnitudo(CORPUS) / magnitudo(CORPUS[0]));
        i32 i;
        i32 circuitus_probati = ZEPHYRUM;

        imprimere("\n--- Probans circuitum STML super corpus ---\n");

        radix_viae = getenv("RHUBARB_RADIX");
        si (radix_viae == NIHIL)
        {
            radix_viae = ".";
        }

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            character  via[DXII];
            character* textus;
                  i32  mensura = ZEPHYRUM;

            sprintf(via, "%s/%s", radix_viae, CORPUS[i]);
            textus = _plagulam_legere(piscina, via, &mensura);
            si (textus == NIHIL)
            {
                imprimere("  ABEST: %s\n", CORPUS[i]);
                perge;
            }
            imprimere("  %s (%d octeti)\n", CORPUS[i],
                (integer)mensura);
            CREDO_VERUM (_circuitum_probare(piscina, &consilium,
                textus, mensura));
            circuitus_probati = circuitus_probati + I;
        }

        /* Porta tegumentum SUUM metitur */
        CREDO_AEQUALIS_I32 (circuitus_probati, VI);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
