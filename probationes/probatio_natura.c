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
    "<.genus nomen=\"vivens\">\n"
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
    "  <.genus nomen=\"planta\">\n"
    "    <proprietates>\n"
    "      <proprietas nomen=\"altitudo\" genus=\"quantitas\"\n"
    "        modulus=\"metrum_probatum\"/>\n"
    "    </proprietates>\n"
    "    <.species nomen=\"rosa\">\n"
    "      <definitio>flos notissimus</definitio>\n"
    "      <valor nomen=\"color\" certitudo=\"scientia\"\n"
    "        fons=\"corpus\">ruber</valor>\n"
    "      <valor nomen=\"vita\">vigens</valor>\n"
    "      <relatum nomen=\"edit\" ad=\"rosa\"/>\n"
    "      <historia>\n"
    "        <eventum quando=\"1800-05\" actio=\"nasci\">descripta</eventum>\n"
    "      </historia>\n"
    /* NIDIFICATIO: cultivar intra speciem - profunditas quam
     * plagulae verae ferunt (planta: malus_domestica >
     * granny_smith; identificatio: isbn > isbn_10) */
    "      <.cultivar nomen=\"rosa_alba\">\n"
    "        <definitio>rosa colore albo</definitio>\n"
    "        <valor nomen=\"color\">albus</valor>\n"
    "      </.cultivar>\n"
    "    </.species>\n"
    "  </.genus>\n"
    "</.genus>\n"
    "<.genus nomen=\"artificium\">\n"
    "  <actiones>\n"
    "    <actio nomen=\"fieri\"/>\n"
    "  </actiones>\n"
    "  <relationes>\n"
    "    <relatio nomen=\"factum_a\" ad=\"vivens\"/>\n"
    "  </relationes>\n"
    "  <proprietates>\n"
    "    <proprietas nomen=\"pretium\" genus=\"numerus\"/>\n"
    "  </proprietates>\n"
    "  <.species nomen=\"corona\" etiam=\"planta\">\n"
    "    <valor nomen=\"pretium\">10</valor>\n"
    "    <valor nomen=\"color\">albus</valor>\n"
    "    <relatum nomen=\"edit\" ad=\"rosa\"/>\n"
    "    <historia>\n"
    "      <eventum quando=\"1900\" actio=\"nasci\">texta</eventum>\n"
    "    </historia>\n"
    "  </.species>\n"
    "</.genus>\n"
    "<.genus nomen=\"geminus\"/>\n"
    "</natura>\n";

interior constans character* FIXTURA_METRUM =
    "<natura modulus=\"metrum_probatum\" versio=\"1\" lingua=\"latina\">\n"
    "<.genus nomen=\"quantitas\">\n"
    "  <definitio>quantum mensuratum</definitio>\n"
    "</.genus>\n"
    "<.genus nomen=\"alienum\" sub=\"vivens\" modulus=\"probatum\">\n"
    "  <definitio>peregrinus sub vivente</definitio>\n"
    "</.genus>\n"
    "<.genus nomen=\"geminus\"/>\n"
    "</natura>\n";

interior constans character* FIXTURA_IUDICIUM =
    "<natura modulus=\"iudicium\" versio=\"1\" lingua=\"latina\">\n"
    "<.genus nomen=\"gradus_assensus\">\n"
    "  <definitio>quam firmiter tenetur</definitio>\n"
    "  <.species nomen=\"scientia\"/>\n"
    "  <.species nomen=\"opinio\"/>\n"
    "</.genus>\n"
    "<.genus nomen=\"corpus_institutionis\">\n"
    "  <definitio>memoria exercitationis</definitio>\n"
    "</.genus>\n"
    "</natura>\n";

/* regulam quamque semel violat; lecta cum stirpe \"aliud\" (XIV) */
interior constans character* FIXTURA_VITIOSA =
    "<natura modulus=\"vitiosum\" versio=\"1\" lingua=\"latina\">\n"
    "<.genus nomen=\"basis\">\n"
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
    "  <.species nomen=\"exemplum\">\n"
    "    <valor nomen=\"inexistens\">x</valor>\n"
    "    <valor nomen=\"modus\" fons=\"ignotus\">duo</valor>\n"
    "    <valor nomen=\"cursus\" valens_a=\"2020\"\n"
    "      valens_ad=\"1999\">initium</valor>\n"
    "    <valor nomen=\"modus\" certitudo=\"fortasse\">unus</valor>\n"
    "    <relatum nomen=\"nescitur\" ad=\"basis\"/>\n"
    "    <historia>\n"
    "      <eventum quando=\"2000\" actio=\"volare\">volavit</eventum>\n"
    "    </historia>\n"
    "  </.species>\n"
    "  <.species nomen=\"exemplum\"/>\n"
    "</.genus>\n"
    "<.genus nomen=\"filius\" sub=\"absens\"/>\n"
    "</natura>\n";

/* declarationes necessitudinum: registratio, ligatio (nomen /
 * conversum / scriptura / citatio explicita), compages (termini),
 * regulae XX-XXIII et XXV */
interior constans character* FIXTURA_NECESSITUDINES =
    "<natura modulus=\"nexus_probandus\" versio=\"1\">\n"
    "<necessitudo nomen=\"continet\" conversum=\"continetur_in\"\n"
    "  a=\"*\" ad=\"*\">\n"
    "  <definitio>Totum partem intra se habet.</definitio>\n"
    "  <scriptio>contenta_in</scriptio>\n"
    "</necessitudo>\n"
    "<necessitudo nomen=\"pars_de\" sub=\"continet\" a=\"*\" ad=\"*\"/>\n"
    "<.genus nomen=\"actus_probandus\">\n"
    "  <definitio>actus fictus probationis</definitio>\n"
    "  <termini>\n"
    "    <terminus munus=\"possessor_primus\" necessitudo=\"continet\"\n"
    "      ad=\"actus_probandus\"/>\n"
    "    <terminus munus=\"possessor_alter\" necessitudo=\"continet\"\n"
    "      ad=\"actus_probandus\"/>\n"
    "  </termini>\n"
    "  <relationes>\n"
    "    <relatio nomen=\"pars_de\" ad=\"actus_probandus\"/>\n"
    "    <relatio nomen=\"contenta_in\" ad=\"actus_probandus\"/>\n"
    "    <relatio nomen=\"ignotum_x\" ad=\"actus_probandus\"/>\n"
    "    <relatio nomen=\"y\" necessitudo=\"non_exstat\"\n"
    "      ad=\"actus_probandus\"/>\n"
    "  </relationes>\n"
    "  <.species nomen=\"exemplum_actus\">\n"
    "    <relatum nomen=\"possessor_primus\" ad=\"actus_probandus\"/>\n"
    "  </.species>\n"
    "</.genus>\n"
    "<.genus nomen=\"subactus_probandus\" sub=\"actus_probandus\"/>\n"
    "</natura>\n";

/* fines et angustatio: fixtura communis duobus scaenis */
interior constans character* FIXTURA_FINES =
    "<natura modulus=\"fines_probandum\" versio=\"1\">\n"
    "<.genus nomen=\"animal_p\"/>\n"
    "<.genus nomen=\"canis_p\" sub=\"animal_p\"/>\n"
    "<.genus nomen=\"lapis_p\"/>\n"
    "<necessitudo nomen=\"mordet\" a=\"*\" ad=\"animal_p\"/>\n"
    "<necessitudo nomen=\"mordet_canem\" sub=\"mordet\"\n"
    "  ad=\"canis_p\"/>\n"
    "<necessitudo nomen=\"mordet_omnia\" sub=\"mordet\"\n"
    "  ad=\"*\"/>\n"
    "<.genus nomen=\"pulex_p\">\n"
    "  <relationes>\n"
    "    <relatio nomen=\"mordet\" ad=\"lapis_p\"/>\n"
    "    <relatio nomen=\"mordet\" ad=\"canis_p\"/>\n"
    "  </relationes>\n"
    "</.genus>\n"
    "</natura>\n";


/* ==================================================
 * Auxilia
 * ================================================== */

interior i32
vulnera_regulae (
    NaturaBibliotheca* bib,
                  i32  regula);
interior i32
monita_regulae (
    NaturaBibliotheca* bib,
                  i32  regula);

interior i32
vulnera_regulae (
    NaturaBibliotheca* bib,
                  i32  regula)
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

interior i32
monita_regulae (
    NaturaBibliotheca* bib,
                  i32  regula)
{
    i32 i;
    i32 numerus;

    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(bib->diagnostica); i++)
    {
        NaturaDiagnosticum* d;

        d = (NaturaDiagnosticum*)xar_obtinere(bib->diagnostica, i);
        si (   d->gradus == NATURA_GRADUS_MONITUM
            && d->regula == regula)
        {
            numerus++;
        }
    }

    redde numerus;
}

s32 principale (vacuum)
{
             b32  praeteritus;
         Piscina* piscina;

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
                      i32  vulnera;

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

        /* mensurae bibliothecae - arbor porphyriana (2026-08-10):
         * species/cultivares GENERA sunt (IX + rosa + rosa_alba +
         * corona + scientia + opinio = XIV); res = individua sola
         * (fixturae nulla habent) */
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->exemplaria), III);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->genera_omnia),
                            XIV);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->res_omnes),
                            ZEPHYRUM);


        /* ====================================================
         * PROBARE: quaestiones nominum
         * ==================================================== */

        {
            NaturaGenus* planta;
            NaturaGenus* vivens;
              NaturaEns* rosa_ens;

            imprimere("\n--- Probans quaestiones ---\n");

            planta = natura_genus(bib, "planta");
            vivens = natura_genus(bib, "vivens");
            CREDO_NON_NIHIL (planta);
            CREDO_NON_NIHIL (vivens);
            CREDO_AEQUALIS_PTR (planta->parens, vivens);

            /* species genus EST (arbor porphyriana): rosa per
             * natura_genus invenitur, parens e nidificatione */
            CREDO_NON_NIHIL (natura_genus(bib, "rosa"));
            rosa_ens = natura_ens_in(bib, "probatum", "rosa");
            CREDO_NON_NIHIL (rosa_ens);
            CREDO_AEQUALIS_I32 ((i32)rosa_ens->discrimen,
                                (i32)NATURA_ENS_GENUS);
            CREDO_AEQUALIS_PTR (
                ((NaturaGenus*)rosa_ens->corpus)->parens, planta);

            /* homonyma: nudum NIHIL, addressatum utrumque */
            CREDO_NIHIL (natura_genus(bib, "geminus"));
            CREDO_NON_NIHIL (natura_genus_in(bib, "probatum",
                                             "geminus"));
            CREDO_NON_NIHIL (natura_genus_in(bib, "metrum_probatum",
                                             "geminus"));
            CREDO_INAEQUALITAS_PTR (
                natura_genus_in(bib, "probatum", "geminus"),
                natura_genus_in(bib, "metrum_probatum", "geminus"));

            /* arbor porphyriana: res_suae individua sola tenet
             * (vacuum hic); subgenera per liberi - planta rosam
             * fert, rosa rosam_albam (profunditas = parentela) */
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(planta->res_suae),
                                ZEPHYRUM);
            {
                NaturaGenus* rosa_g;
                        i32  j;
                        b32  rosa_inventa;

                rosa_g        = natura_genus(bib, "rosa");
                rosa_inventa  = FALSUM;
                per (j = ZEPHYRUM;
                     j < xar_numerus(planta->liberi); j++)
                {
                    si (*(NaturaGenus**)xar_obtinere(
                            planta->liberi, j) == rosa_g)
                    {
                        rosa_inventa = VERUM;
                    }
                }
                CREDO_VERUM (rosa_inventa);
            }
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
            NaturaGenus* corona;

            imprimere("\n--- Probans etiam ---\n");

            corona_ens = natura_ens_in(bib, "probatum", "corona");
            CREDO_NON_NIHIL (corona_ens);
            /* arbor porphyriana: corona GENUS, catena secunda per
             * parens_etiam (doctrina a rebus portata) */
            CREDO_AEQUALIS_I32 ((i32)corona_ens->discrimen,
                                (i32)NATURA_ENS_GENUS);

            corona = (NaturaGenus*)corona_ens->corpus;
            CREDO_AEQUALIS_PTR (corona->parens,
                                natura_genus(bib, "artificium"));
            CREDO_AEQUALIS_PTR (corona->parens_etiam,
                                natura_genus(bib, "planta"));
        }


        /* ====================================================
         * TAXINOMIA vs APPARATUS: catenae duae distinctae
         *
         * Plagulae profunditatem Porphyrianam SEMPER tulerunt
         * (<.species isbn> speciem isbn_10 continet; <.species
         * malus_domestica> cultivar granny_smith) sed graphus
         * eam non ferebat: res omnes ad genus ambiens
         * planabantur. Nunc AMBAE dicuntur - et haec probatio
         * custodit ne altera alteram corrumpat.
         * ==================================================== */

        {
              NaturaEns* granny_ens;
              NaturaEns* malus_d_ens;
            NaturaGenus* granny;
            NaturaGenus* malus_d;

            imprimere("\n--- Taxinomia nidificata ---\n");

            granny_ens  = natura_ens_in(bib, "probatum",
                                        "rosa_alba");
            malus_d_ens = natura_ens_in(bib, "probatum",
                                        "rosa");
            CREDO_NON_NIHIL (granny_ens);
            CREDO_NON_NIHIL (malus_d_ens);

            /* CUSTODIA INVERSA CONSULTO (2026-08-10): custodia
             * pristina hic genus_suum planum servabat - arbor
             * porphyriana eam ipsam mutat. Nunc species/cultivar
             * GENERA sunt: profunditas taxinomica = parentela
             * generum, catena UNA pro universalibus. */
            CREDO_AEQUALIS_I32 ((i32)granny_ens->discrimen,
                                (i32)NATURA_ENS_GENUS);
            CREDO_AEQUALIS_I32 ((i32)malus_d_ens->discrimen,
                                (i32)NATURA_ENS_GENUS);

            granny   = (NaturaGenus*)granny_ens->corpus;
            malus_d  = (NaturaGenus*)malus_d_ens->corpus;

            /* parentela: rosa_alba sub rosa, rosa sub planta */
            CREDO_AEQUALIS_PTR (granny->parens, malus_d);
            CREDO_AEQUALIS_PTR (malus_d->parens,
                                natura_genus(bib, "planta"));

            /* et deorsum quoque ambulabilis (liberi) */
            {
                i32 j;
                b32 granny_inventa;

                granny_inventa = FALSUM;
                per (j = ZEPHYRUM;
                     j < xar_numerus(malus_d->liberi); j++)
                {
                    si (*(NaturaGenus**)xar_obtinere(
                            malus_d->liberi, j) == granny)
                    {
                        granny_inventa = VERUM;
                    }
                }
                CREDO_VERUM (granny_inventa);
            }

            /* hereditas apparatus per parentelam fluit: maiores
             * granny = rosa -> planta -> vivens */
            {
                Xar* maiores;

                maiores = natura_maiores(granny, piscina);
                CREDO_NON_NIHIL (maiores);
                CREDO_AEQUALIS_I32 ((i32)xar_numerus(maiores), III);
            }
        }


        /* ====================================================
         * PROBARE: maiores (etiam trans-exemplar)
         * ==================================================== */

        {
            NaturaGenus* planta;
            NaturaGenus* vivens;
            NaturaGenus* alienum;
                    Xar* maiores;

            imprimere("\n--- Probans maiores ---\n");

            planta   = natura_genus(bib, "planta");
            vivens   = natura_genus(bib, "vivens");
            alienum  = natura_genus(bib, "alienum");
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
                       NaturaGenus* planta;
                       NaturaGenus* vivens;
                               Xar* apparatus;
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


        /* ====================================================
         * PROBARE: determinationes in apparatu (arbor
         * porphyriana - valor in genere proprietatem hereditatam
         * CLAUDIT et apparatus id ostendere debet)
         * ==================================================== */

        {
            NaturaGenus* rosa;
                    Xar* apparatus;
                    i32  j;
                    i32  valores;
                    i32  relata;
            NaturaGenus* auctor_valoris;

            imprimere("\n--- Probans determinationes ---\n");

            rosa = natura_genus(bib, "rosa");
            CREDO_NON_NIHIL (rosa);

            apparatus = natura_apparatus(bib, rosa, piscina);
            CREDO_NON_NIHIL (apparatus);

            /* rosa: valor color + valor vita + relatum edit (III)
             * planta: altitudo (I); vivens: VI - summa X */
            CREDO_AEQUALIS_I32 ((i32)xar_numerus(apparatus), X);

            valores         = ZEPHYRUM;
            relata          = ZEPHYRUM;
            auctor_valoris  = NIHIL;
            per (j = ZEPHYRUM; j < xar_numerus(apparatus); j++)
            {
                NaturaApparatusMembrum* m;

                m = (NaturaApparatusMembrum*)xar_obtinere(
                    apparatus, j);
                si (chorda_aequalis_literis(*m->nodus->titulus,
                                            "valor"))
                {
                    valores++;
                    auctor_valoris = m->auctor;
                }
                alioquin si (chorda_aequalis_literis(
                                 *m->nodus->titulus, "relatum"))
                {
                    relata++;
                }
            }
            CREDO_AEQUALIS_I32 (valores, II);
            CREDO_AEQUALIS_I32 (relata, I);
            /* auctor = genus quod determinavit (rosa ipsa) */
            CREDO_AEQUALIS_PTR (auctor_valoris, rosa);
        }
    }


    /* ========================================================
     * PROBARE: necessitudines - registratio et regulae novae
     * ======================================================== */

    {
        NaturaBibliotheca* bib;
                      i32  vulnera;

        imprimere("\n--- Probans necessitudines (registratio) ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(FIXTURA_NECESSITUDINES, piscina),
            "nexus_probandus"));

        vulnera = natura_nectere(bib);
        /* vulnus unum: citatio 'non_exstat' (regula XXI) */
        CREDO_AEQUALIS_I32 (vulnera, I);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXI), I);
        CREDO_AEQUALIS_I32 (
            (i32)xar_numerus(bib->necessitudines_omnes), II);

        /* nomen nudum resolvit; genus eodem nomine NIHIL */
        CREDO_NON_NIHIL (natura_necessitudo(bib, "continet"));
        CREDO_NIHIL (natura_genus(bib, "continet"));

        /* scriptura et conversum IDENTITATEM UNAM resolvunt */
        CREDO_AEQUALIS_PTR (natura_necessitudo(bib, "contenta_in"),
                            natura_necessitudo(bib, "continet"));
        CREDO_AEQUALIS_PTR (natura_necessitudo(bib, "continetur_in"),
                            natura_necessitudo(bib, "continet"));

        /* sub= necessitudinis resolutum */
        CREDO_AEQUALIS_PTR (
            natura_necessitudo(bib, "pars_de")->parens,
            natura_necessitudo(bib, "continet"));

        /* monitum XX aggregatum: nomen unum insolutum (ignotum_x)
         * - 'y' non numeratur (citatio eius XXI iam clamavit) */
        CREDO_AEQUALIS_I32 (monita_regulae(bib, XX), I);

        /* relatum munus compagis invenit (regula XI tacet) */
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XI), ZEPHYRUM);

        /* modulus cum necessitudine sola regulam XXV non violat */
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXV), ZEPHYRUM);

        /* TERMINI IN APPARATU: compages actus membra apparatus
         * sunt, et sub-actus compagem HEREDITAT (catena maiorum
         * eadem quae proprietatibus servit) */
        {
            NaturaGenus* actus_g;
            NaturaGenus* subactus_g;
                    Xar* apparatus;
                    i32  k;
                    i32  termini_numerus;

            actus_g     = natura_genus(bib, "actus_probandus");
            subactus_g  = natura_genus(bib, "subactus_probandus");
            CREDO_NON_NIHIL (actus_g);
            CREDO_NON_NIHIL (subactus_g);

            apparatus = natura_apparatus(bib, actus_g, piscina);
            CREDO_NON_NIHIL (apparatus);
            termini_numerus = ZEPHYRUM;
            per (k = ZEPHYRUM; k < xar_numerus(apparatus); k++)
            {
                NaturaApparatusMembrum* m;

                m = (NaturaApparatusMembrum*)xar_obtinere(
                    apparatus, k);
                si (chorda_aequalis_literis(*m->nodus->titulus,
                                            "terminus"))
                {
                    termini_numerus++;
                }
            }
            CREDO_AEQUALIS_I32 (termini_numerus, II);

            apparatus = natura_apparatus(bib, subactus_g, piscina);
            CREDO_NON_NIHIL (apparatus);
            termini_numerus = ZEPHYRUM;
            per (k = ZEPHYRUM; k < xar_numerus(apparatus); k++)
            {
                NaturaApparatusMembrum* m;

                m = (NaturaApparatusMembrum*)xar_obtinere(
                    apparatus, k);
                si (chorda_aequalis_literis(*m->nodus->titulus,
                                            "terminus"))
                {
                    termini_numerus++;
                }
            }
            CREDO_AEQUALIS_I32 (termini_numerus, II);
        }
    }

    {
        NaturaBibliotheca* bib;

        imprimere("\n--- Probans fines (regula XXII) ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(FIXTURA_FINES, piscina),
            "fines_probandum"));

        (vacuum)natura_nectere(bib);
        /* dilatatio (mordet_omnia ad='*') + sedes extra finem
         * (lapis_p extra animal_p); mordet->canis_p INTRA finem */
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXII), II);

        /* tabula sedium ligatarum: mordet II sedes fert, ambae
         * recta directione - canis_p INTRA, lapis_p EXCEDIT */
        {
            NaturaNecessitudo* mordet_n;
                          i32  k;
                          i32  numerus_s;
                          i32  intra_n;
                          i32  excedit_n;

            mordet_n = natura_necessitudo(bib, "mordet");
            CREDO_NON_NIHIL (mordet_n);
            numerus_s  = ZEPHYRUM;
            intra_n    = ZEPHYRUM;
            excedit_n  = ZEPHYRUM;
            per (k = ZEPHYRUM;
                 k < xar_numerus(bib->sedes_ligatae); k++)
            {
                NaturaSedesLigata* s;

                s = (NaturaSedesLigata*)xar_obtinere(
                    bib->sedes_ligatae, k);
                si (s->ligata != mordet_n)
                {
                    perge;
                }
                numerus_s++;
                CREDO_VERUM (!s->conversa);
                si (s->verdictum == NATURA_SEDES_INTRA)
                {
                    intra_n++;
                }
                si (s->verdictum == NATURA_SEDES_EXCEDIT)
                {
                    excedit_n++;
                }
            }
            CREDO_AEQUALIS_I32 (numerus_s, II);
            CREDO_AEQUALIS_I32 (intra_n, I);
            CREDO_AEQUALIS_I32 (excedit_n, I);
        }

        /* regula XXIV: tres familiae sine converso fines impares
         * habent (mordet a=* ad=animal_p; mordet_canem ad=canis_p;
         * mordet_omnia ad=animal_p hereditatum) - directio
         * innominata, monitum in declaratione */
        CREDO_AEQUALIS_I32 (monita_regulae(bib, XXIV), III);
    }

    {
        NaturaBibliotheca* bib;

        imprimere("\n--- Probans directionem citationis"
                  " (regula XXII) ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(
                "<natura modulus=\"directio_probandum\" versio=\"1\">\n"
                "<.genus nomen=\"basis_p\"/>\n"
                "<.genus nomen=\"aedes_p\"/>\n"
                "<necessitudo nomen=\"fundat_p\"\n"
                "  conversum=\"fundatur_in_p\"\n"
                "  a=\"basis_p\" ad=\"aedes_p\"/>\n"
                "<.genus nomen=\"turris_p\" sub=\"aedes_p\">\n"
                "  <relationes>\n"
                "    <relatio nomen=\"stat_super\"\n"
                "      necessitudo=\"fundatur_in_p\" ad=\"basis_p\"/>\n"
                "    <relatio nomen=\"nititur_in\"\n"
                "      necessitudo=\"fundatur_in_p\" ad=\"basis_p\"/>\n"
                "    <relatio nomen=\"stat_male\"\n"
                "      necessitudo=\"fundatur_in_p\" ad=\"aedes_p\"/>\n"
                "    <relatio nomen=\"fundatur_in_p\" ad=\"basis_p\"/>\n"
                "  </relationes>\n"
                "</.genus>\n"
                "</natura>\n", piscina),
            "directio_probandum"));

        (vacuum)natura_nectere(bib);
        /* citatio DIRECTIONEM nominat (decretum 2026-08-11):
         * stat_super et nititur_in conversam citant, ad=basis_p
         * intra finem a= - LEGALES (ante decretum contra ad=
         * iudicatae essent). stat_male conversam citat sed
         * ad=aedes_p extra a=basis_p - VULNUS UNUM. sedes
         * verbo-ligata conversa (fundatur_in_p) legalis manet. */
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXII), I);
        /* fundat_p conversum habet - regula XXIV tacet */
        CREDO_AEQUALIS_I32 (monita_regulae(bib, XXIV), ZEPHYRUM);

        /* tabula: IV sedes fundat_p, omnes conversae (citatio aut
         * verbum ipsum conversum); stat_male sola EXCEDIT */
        {
            NaturaNecessitudo* fundat_n;
                          i32  k;
                          i32  numerus_s;
                          i32  intra_n;
                          i32  excedit_n;

            fundat_n = natura_necessitudo(bib, "fundat_p");
            CREDO_NON_NIHIL (fundat_n);
            numerus_s  = ZEPHYRUM;
            intra_n    = ZEPHYRUM;
            excedit_n  = ZEPHYRUM;
            per (k = ZEPHYRUM;
                 k < xar_numerus(bib->sedes_ligatae); k++)
            {
                NaturaSedesLigata* s;

                s = (NaturaSedesLigata*)xar_obtinere(
                    bib->sedes_ligatae, k);
                si (s->ligata != fundat_n)
                {
                    perge;
                }
                numerus_s++;
                CREDO_VERUM (s->conversa);
                si (s->verdictum == NATURA_SEDES_INTRA)
                {
                    intra_n++;
                }
                si (s->verdictum == NATURA_SEDES_EXCEDIT)
                {
                    excedit_n++;
                }
            }
            CREDO_AEQUALIS_I32 (numerus_s, IV);
            CREDO_AEQUALIS_I32 (intra_n, III);
            CREDO_AEQUALIS_I32 (excedit_n, I);
        }
    }

    {
        NaturaBibliotheca* bib;

        imprimere("\n--- Probans superpositionem finis"
                  " (angustatio) ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(FIXTURA_FINES, piscina),
            "fines_probandum"));

        /* familia ignota recusatur */
        CREDO_VERUM (!natura_finem_superponere(bib, "non_est",
                                               "ad", "lapis_p"));
        /* superpositio ante nexuram: mordet ad -> lapis_p */
        CREDO_VERUM (natura_finem_superponere(bib, "mordet",
                                              "ad", "lapis_p"));

        (vacuum)natura_nectere(bib);

        /* verdicta VERSA sub hypothesi: lapis_p INTRA fit,
         * canis_p EXCEDIT; insuper mordet_canem (ad=canis_p)
         * finem parentis novum excedit (nexus subfamiliarum quem
         * ambulatio manualis non videt); dilatatio mordet_omnia
         * manet - III vulnera XXII */
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXII), III);
        /* regula XXIV sub hypothesi quoque currit */
        CREDO_AEQUALIS_I32 (monita_regulae(bib, XXIV), III);

        /* post nexuram superpositio recusatur */
        CREDO_VERUM (!natura_finem_superponere(bib, "mordet",
                                               "ad", "animal_p"));
    }

    {
        NaturaBibliotheca* bib;

        imprimere("\n--- Probans munus geminum (regula XXIII) ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(
                "<natura modulus=\"munus_probandum\" versio=\"1\">\n"
                "<necessitudo nomen=\"participatio_p\" a=\"*\" ad=\"*\"/>\n"
                "<.genus nomen=\"pactum_p\">\n"
                "  <termini>\n"
                "    <terminus munus=\"dator\"\n"
                "      necessitudo=\"participatio_p\" ad=\"*\"/>\n"
                "    <terminus munus=\"dator\"\n"
                "      necessitudo=\"participatio_p\" ad=\"*\"/>\n"
                "  </termini>\n"
                "</.genus>\n"
                "</natura>\n", piscina),
            "munus_probandum"));

        (vacuum)natura_nectere(bib);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXIII), I);
        /* termini citati ligantur - nihil insolutum */
        CREDO_AEQUALIS_I32 (monita_regulae(bib, XX), ZEPHYRUM);
        /* participatio_p sine converso sed fines pares (*,*) -
         * regula XXIV tacet */
        CREDO_AEQUALIS_I32 (monita_regulae(bib, XXIV), ZEPHYRUM);
    }

    {
        NaturaBibliotheca* bib;

        imprimere("\n--- Probans regulam XXV (modulus vacuus) ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(
                "<natura modulus=\"vacuus\" versio=\"1\">\n"
                "<nota>nihil declaratur</nota>\n"
                "</natura>\n", piscina),
            "vacuus"));

        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXV), I);
    }

    {
        NaturaBibliotheca* bib;

        imprimere("\n--- Probans foramen II (ad= necessitudinem) ---\n");

        bib = natura_bibliotheca_creare(piscina);
        CREDO_NON_NIHIL (bib);

        CREDO_VERUM (natura_legere(bib,
            chorda_ex_literis(
                "<natura modulus=\"discrimen_probandum\" versio=\"1\">\n"
                "<necessitudo nomen=\"tangere\" a=\"*\" ad=\"*\"/>\n"
                "<.genus nomen=\"res_probata\">\n"
                "  <relationes>\n"
                "    <relatio nomen=\"x\" ad=\"tangere\"/>\n"
                "  </relationes>\n"
                "</.genus>\n"
                "</natura>\n", piscina),
            "discrimen_probandum"));

        (vacuum)natura_nectere(bib);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, II), I);
    }


    /* ========================================================
     * PROBARE: fixtura vitiosa - regulae singulae
     * ======================================================== */

    {
        NaturaBibliotheca* bib;
                      i32  vulnera;

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
        CREDO_AEQUALIS_I32 (vulnera, XI);

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
        /* regula XV migravit (unicitas 'entia' canonis); VII
         * dimidium formae quoque - ORDO solus oneratori manet
         * (fixtura intervallo inverso eum probat) */
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XV),   ZEPHYRUM);
        CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XVI),  I);

        /* proprietas specie typata MUNDA est post arborem
         * porphyrianam: species genus est, ergo typus ad GENUS
         * solvit - monitum pristinum ('ad rem solvit') evanuit
         * CONSULTO (id ipsum quod migratio emendabat) */
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
                si (   d->gradus == NATURA_GRADUS_MONITUM
                    && d->regula == III)
                {
                    monita++;
                }
            }
            CREDO_AEQUALIS_I32 (monita, ZEPHYRUM);
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
                "<.genus nomen=\"unum\" nomen=\"duo\">\n"
                "  <definitio>attributum duplicatum</definitio>\n"
                "</.genus>\n"
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
            chorda_ex_literis("<natura modulus=\"x\"><.genus",
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
