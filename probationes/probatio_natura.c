/* probatio_natura.c - Probationes oneratoris bibliothecae semanticae
 *
 * Fixturae tres sanae (probatum / metrum_probatum / iudicium)
 * mechanismos omnes exercent: sub= trans-exemplar, proprietas typo
 * bibliothecae, electio, machina statuum, umbra (externum),
 * homonyma. Fixtura vitiosa regulam quamque semel violat.
 */
#include "latina.h"
#include "natura.h"
#include "credo.h"
#include <stdio.h>

/* ==================================================
 * Fixturae
 * ================================================== */

interior constans character* FIXTURA_PROBATUM =
    "<natura modulus=\"probatum\" versio=\"1\" lingua=\"latina\">\n"
    "<fontes>\n"
    "  <fons clavis=\"corpus\" genus=\"corpus_institutionis\"\n"
    "    modulus=\"iudicium\">memoria scribae</fons>\n"
    "</fontes>\n"
    "<genus nomen=\"vivens\">\n"
    "  <definitio>quod vivit</definitio>\n"
    "  <proprietates>\n"
    "    <proprietas nomen=\"aetas\" genus=\"numerus\"/>\n"
    "    <proprietas nomen=\"color\" genus=\"electio\">\n"
    "      <optio>ruber</optio>\n"
    "      <optio>albus</optio>\n"
    "    </proprietas>\n"
    "  </proprietates>\n"
    "  <machina_statuum nomen=\"vita\">\n"
    "    <status nomen=\"vigens\"/>\n"
    "    <status nomen=\"mortuum\"/>\n"
    "    <transitus a=\"vigens\" ad=\"mortuum\" per=\"mors\"/>\n"
    "  </machina_statuum>\n"
    "  <actiones>\n"
    "    <actio nomen=\"nasci\"/>\n"
    "  </actiones>\n"
    "  <relationes>\n"
    "    <relatio nomen=\"edit\" ad=\"vivens\"/>\n"
    "    <relatio nomen=\"videt\" ad=\"umbra_ignota\" externum=\"verum\"/>\n"
    "  </relationes>\n"
    "  <genus nomen=\"planta\">\n"
    "    <proprietates>\n"
    "      <proprietas nomen=\"altitudo\" genus=\"quantitas\"\n"
    "        modulus=\"metrum_probatum\"/>\n"
    "    </proprietates>\n"
    "    <species nomen=\"rosa\">\n"
    "      <definitio>flos notissimus</definitio>\n"
    "      <valor nomen=\"color\" certitudo=\"scientia\"\n"
    "        fons=\"corpus\">ruber</valor>\n"
    "      <valor nomen=\"vita\">vigens</valor>\n"
    "      <relatum nomen=\"edit\" ad=\"rosa\"/>\n"
    "      <historia>\n"
    "        <eventum quando=\"1800-05\" actio=\"nasci\">descripta</eventum>\n"
    "      </historia>\n"
    "    </species>\n"
    "  </genus>\n"
    "</genus>\n"
    "<genus nomen=\"artificium\">\n"
    "  <actiones>\n"
    "    <actio nomen=\"fieri\"/>\n"
    "  </actiones>\n"
    "  <relationes>\n"
    "    <relatio nomen=\"factum_a\" ad=\"vivens\"/>\n"
    "  </relationes>\n"
    "  <proprietates>\n"
    "    <proprietas nomen=\"pretium\" genus=\"numerus\"/>\n"
    "  </proprietates>\n"
    "  <species nomen=\"corona\" etiam=\"planta\">\n"
    "    <valor nomen=\"pretium\">10</valor>\n"
    "    <valor nomen=\"color\">albus</valor>\n"
    "    <relatum nomen=\"edit\" ad=\"rosa\"/>\n"
    "    <historia>\n"
    "      <eventum quando=\"1900\" actio=\"nasci\">texta</eventum>\n"
    "    </historia>\n"
    "  </species>\n"
    "</genus>\n"
    "<genus nomen=\"geminus\"/>\n"
    "</natura>\n";

interior constans character* FIXTURA_METRUM =
    "<natura modulus=\"metrum_probatum\" versio=\"1\" lingua=\"latina\">\n"
    "<genus nomen=\"quantitas\">\n"
    "  <definitio>quantum mensuratum</definitio>\n"
    "</genus>\n"
    "<genus nomen=\"alienum\" sub=\"vivens\" modulus=\"probatum\">\n"
    "  <definitio>peregrinus sub vivente</definitio>\n"
    "</genus>\n"
    "<genus nomen=\"geminus\"/>\n"
    "</natura>\n";

interior constans character* FIXTURA_IUDICIUM =
    "<natura modulus=\"iudicium\" versio=\"1\" lingua=\"latina\">\n"
    "<genus nomen=\"gradus_assensus\">\n"
    "  <definitio>quam firmiter tenetur</definitio>\n"
    "  <species nomen=\"scientia\"/>\n"
    "  <species nomen=\"opinio\"/>\n"
    "</genus>\n"
    "<genus nomen=\"corpus_institutionis\">\n"
    "  <definitio>memoria exercitationis</definitio>\n"
    "</genus>\n"
    "</natura>\n";

/* regulam quamque semel violat; lecta cum stirpe \"aliud\" (XIV) */
interior constans character* FIXTURA_VITIOSA =
    "<natura modulus=\"vitiosum\" versio=\"1\" lingua=\"latina\">\n"
    "<genus nomen=\"basis\">\n"
    "  <proprietates>\n"
    "    <proprietas nomen=\"modus\" genus=\"electio\">\n"
    "      <optio>unus</optio>\n"
    "    </proprietas>\n"
    "    <proprietas nomen=\"pondus\" genus=\"quantitas\"\n"
    "      modulus=\"absens\"/>\n"
    "    <proprietas nomen=\"gradus\" genus=\"scientia\"\n"
    "      modulus=\"iudicium\"/>\n"
    "  </proprietates>\n"
    "  <machina_statuum nomen=\"cursus\">\n"
    "    <status nomen=\"initium\"/>\n"
    "    <transitus a=\"initium\" ad=\"nusquam\" per=\"saltus\"/>\n"
    "  </machina_statuum>\n"
    "  <relationes>\n"
    "    <relatio nomen=\"tangit\" ad=\"phantasma\"/>\n"
    "    <relatio nomen=\"videt\" ad=\"basis\" externum=\"verum\"/>\n"
    "  </relationes>\n"
    "  <mysterium/>\n"
    "  <species nomen=\"exemplum\">\n"
    "    <valor nomen=\"inexistens\">x</valor>\n"
    "    <valor nomen=\"modus\" fons=\"ignotus\">duo</valor>\n"
    "    <valor nomen=\"cursus\" valens_a=\"1999-13\">initium</valor>\n"
    "    <valor nomen=\"modus\" certitudo=\"fortasse\">unus</valor>\n"
    "    <relatum nomen=\"nescitur\" ad=\"basis\"/>\n"
    "    <historia>\n"
    "      <eventum quando=\"2000\" actio=\"volare\">volavit</eventum>\n"
    "    </historia>\n"
    "  </species>\n"
    "  <species nomen=\"exemplum\"/>\n"
    "</genus>\n"
    "<genus nomen=\"filius\" sub=\"absens\"/>\n"
    "</natura>\n";

/* ==================================================
 * Auxilia
 * ================================================== */

interior i32 vulnera_regulae(NaturaBibliotheca* bib, i32 regula);

interior i32
vulnera_regulae(
    NaturaBibliotheca* bib,
    i32                regula)
{
    i32 i;
    i32 numerus;

    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(bib->diagnostica); i++)
    {
        NaturaDiagnosticum* d;

        d = (NaturaDiagnosticum*)xar_obtinere(bib->diagnostica, i);
        si (d->gradus == NATURA_GRADUS_VULNUS && d->regula == regula)
        {
            numerus++;
        }
    }

    redde numerus;
}

s32 principale (vacuum)
{
             b32  praeteritus;
        Piscina*  piscina;

    piscina = piscina_generare_dynamicum("probatio_natura", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: bibliotheca sana - legere + nectere
     * ======================================================== */

    {
        NaturaBibliotheca* bib;
        i32                vulnera;

        imprimere("\n--- Probans lectionem sanam ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(FIXTURA_PROBATUM, piscina),
            "probatum"));
        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(FIXTURA_METRUM, piscina),
            "metrum_probatum"));
        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(FIXTURA_IUDICIUM, piscina),
            "iudicium"));

        vulnera = natura_nectere(bib);
        CREDO_AEQUALIS_I32 (vulnera, ZEPHYRUM);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->diagnostica),
                            ZEPHYRUM);

        /* mensurae bibliothecae */
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->exemplaria), III);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->genera_omnia),
                            IX);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->res_omnes), IV);


        /* ====================================================
         * PROBARE: quaestiones nominum
         * ==================================================== */

        {
            NaturaGenus* planta;
            NaturaGenus* vivens;
            NaturaEns*   rosa_ens;

            imprimere("\n--- Probans quaestiones ---\n");

            planta = natura_genus(bib, "planta");
            vivens = natura_genus(bib, "vivens");
            CREDO_NON_NIHIL (planta);
            CREDO_NON_NIHIL (vivens);
            CREDO_AEQUALIS_PTR (planta->parens, vivens);

            /* res non genus */
            CREDO_NIHIL (natura_genus(bib, "rosa"));
            rosa_ens = natura_ens_in(bib, "probatum", "rosa");
            CREDO_NON_NIHIL (rosa_ens);
            CREDO_AEQUALIS_I32 ((i32)rosa_ens->discrimen,
                                (i32)NATURA_ENS_RES);

            /* homonyma: nudum NIHIL, addressatum utrumque */
            CREDO_NIHIL (natura_genus(bib, "geminus"));
            CREDO_NON_NIHIL (natura_genus_in(bib, "probatum",
                                             "geminus"));
            CREDO_NON_NIHIL (natura_genus_in(bib, "metrum_probatum",
                                             "geminus"));
            CREDO_INAEQUALITAS_PTR (
                natura_genus_in(bib, "probatum", "geminus"),
                natura_genus_in(bib, "metrum_probatum", "geminus"));

            /* res_suae */
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(planta->res_suae),
                                I);
        }


        /* ====================================================
         * PROBARE: etiam - membrum essentiale duplex
         *
         * corona species artificii est, etiam planta. Regulae
         * IX/X/XI/XIII per catenam SECUNDAM transeunt: color et
         * edit et nasci in vivente sola declarantur, quo corona
         * per plantam pervenit.
         * ==================================================== */

        {
            NaturaEns* corona_ens;
            NaturaRes* corona;

            imprimere("\n--- Probans etiam ---\n");

            corona_ens = natura_ens_in(bib, "probatum", "corona");
            CREDO_NON_NIHIL (corona_ens);
            CREDO_AEQUALIS_I32 ((i32)corona_ens->discrimen,
                                (i32)NATURA_ENS_RES);

            corona = (NaturaRes*)corona_ens->corpus;
            CREDO_AEQUALIS_PTR (corona->genus_suum,
                                natura_genus(bib, "artificium"));
            CREDO_AEQUALIS_PTR (corona->genus_etiam,
                                natura_genus(bib, "planta"));
        }


        /* ====================================================
         * PROBARE: maiores (etiam trans-exemplar)
         * ==================================================== */

        {
            NaturaGenus* planta;
            NaturaGenus* vivens;
            NaturaGenus* alienum;
            Xar*         maiores;

            imprimere("\n--- Probans maiores ---\n");

            planta  = natura_genus(bib, "planta");
            vivens  = natura_genus(bib, "vivens");
            alienum = natura_genus(bib, "alienum");
            CREDO_NON_NIHIL (alienum);

            maiores = natura_maiores(planta, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(maiores), I);
            CREDO_AEQUALIS_PTR (
                *(NaturaGenus**)xar_obtinere(maiores, ZEPHYRUM),
                vivens);

            /* sub= trans-exemplar */
            maiores = natura_maiores(alienum, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(maiores), I);
            CREDO_AEQUALIS_PTR (
                *(NaturaGenus**)xar_obtinere(maiores, ZEPHYRUM),
                vivens);

            /* index reversus */
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(vivens->liberi),
                                II);

            maiores = natura_maiores(vivens, piscina);
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(maiores),
                                ZEPHYRUM);
        }


        /* ====================================================
         * PROBARE: apparatus cum hereditate
         * ==================================================== */

        {
            NaturaGenus*            planta;
            NaturaGenus*            vivens;
            Xar*                    apparatus;
            NaturaApparatusMembrum* membrum;

            imprimere("\n--- Probans apparatum ---\n");

            planta = natura_genus(bib, "planta");
            vivens = natura_genus(bib, "vivens");

            apparatus = natura_apparatus(bib, planta, piscina);
            CREDO_NON_NIHIL (apparatus);

            /* planta: altitudo (I)
             * vivens: aetas color (II) + vita (I) + nasci (I)
             *         + edit videt (II) = VI
             * summa VII, generis ipsius primum */
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(apparatus), VII);

            membrum = (NaturaApparatusMembrum*)xar_obtinere(
                apparatus, ZEPHYRUM);
            CREDO_AEQUALIS_PTR (membrum->auctor, planta);
            CREDO_VERUM (chorda_aequalis_literis(
                *membrum->nodus->titulus, "proprietas"));

            membrum = (NaturaApparatusMembrum*)xar_obtinere(
                apparatus, I);
            CREDO_AEQUALIS_PTR (membrum->auctor, vivens);

            membrum = (NaturaApparatusMembrum*)xar_obtinere(
                apparatus, III);
            CREDO_VERUM (chorda_aequalis_literis(
                *membrum->nodus->titulus, "machina_statuum"));
        }
    }


    /* ========================================================
     * PROBARE: fixtura vitiosa - regulae singulae
     * ======================================================== */

    {
        NaturaBibliotheca* bib;
        i32                vulnera;

        imprimere("\n--- Probans regulas contractus ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        /* stirps consulto falsa (regula XIV) */
        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(FIXTURA_VITIOSA, piscina), "aliud"));
        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(FIXTURA_IUDICIUM, piscina),
            "iudicium"));

        vulnera = natura_nectere(bib);
        CREDO_AEQUALIS_I32 (vulnera, XII);

        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, II),   I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, III),  I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, IV),   I);
        /* regulae V, VIII, XII MIGRAVERUNT ad canonem (2026-08-06):
         * onerator fons= ignotum, <mysterium/>, transitus alienum
         * tacite fert; natura.canon ea clamat (citationes 'fontium'
         * et 'transitus-*'; probatur in probatio_canon et per
         * differentiam plantatam in migratione) */
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, V),    ZEPHYRUM);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, VI),   I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, VII),  I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, VIII), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, IX),   I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, X),    I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XI),   I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XII),  ZEPHYRUM);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XIII), I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XIV),  I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XV),   I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XVI),  I);

        /* proprietas re dictionarii typata SOLVIT - monitum, non
         * vulnus (regula II parem gradum iam accipit) */
        {
            i32 monita;
            i32 j;

            monita = ZEPHYRUM;
            per (j = ZEPHYRUM;
                 j < xar_numerus(bib->diagnostica); j++)
            {
                NaturaDiagnosticum* d;

                d = (NaturaDiagnosticum*)xar_obtinere(
                    bib->diagnostica, j);
                si (d->gradus == NATURA_GRADUS_MONITUM &&
                    d->regula == III)
                {
                    monita++;
                }
            }
            CREDO_AEQUALIS_I32 (monita, I);
        }
    }


    /* ========================================================
     * PROBARE: strictum -> regula I
     *
     * Plagula PARSATUR sed attributum bis fert: parsatio successum
     * reddit, vitium tamen regulae I fit. Plagula 'nomen=duo'
     * dicit dum omnis lector 'unum' adhibet - discrimen quod
     * tacere non licet.
     * ======================================================== */

    {
        NaturaBibliotheca* bib;

        imprimere("\n--- Probans strictum in regula I ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(
                "<natura modulus=\"duplex\" versio=\"1\" lingua=\"latina\">\n"
                "<genus nomen=\"unum\" nomen=\"duo\">\n"
                "  <definitio>attributum duplicatum</definitio>\n"
                "</genus>\n"
                "</natura>\n", piscina),
            "duplex"));

        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, I), I);
    }


    /* ========================================================
     * PROBARE: parsatio fracta (regula I)
     * ======================================================== */

    {
        NaturaBibliotheca* bib;

        imprimere("\n--- Probans parsationem fractam ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_FALSUM (natura_legere(bib,
            chorda_ex_literis("<natura modulus=\"x\"><genus",
                              piscina),
            "x"));
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, I), I);

        /* radix aliena */
        CREDO_FALSUM (natura_legere(bib,
            chorda_ex_literis("<alia/>", piscina), "alia"));
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, I), II);
    }


    /* ========================================================
     * Compendium
     * ======================================================== */

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
