/* probatio_fabrica_declarationes.c - probationes fabricae
 * (fabrica-6 H3), pars declarationes: lectio aedificatio.stml,
 * composita et praecondiciones
 *
 * Mundus fictus (discus in memoria, sutura ficta):
 * probationes/fabrica_mundus_fictus.h. Spec:
 * project-specs/fabrica-spec-v2.md et sequentes. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "sigillum.h"
#include "internamentum.h"
#include "fabrica.h"
#include "credo.h"
#include "fabrica_mundus_fictus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * PROBARE: declarationes (T2)
 * ================================================== */

interior vacuum
_probare_declarationes (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
         InternamentumChorda* intern;
                      chorda  contentum;
                      chorda  causa;
                         Xar* actiones;
                         Xar* subsystemata;
                FabricaActio* actio;
               FabricaExitus* exitus;
            FabricaIngressus* ingressus;

        intern         = internamentum_creare(piscina);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/decl_bona.stml", piscina);
        actiones = fabrica_declarationes_legere(contentum,
            "silva/aedificatio.stml", piscina, intern, &causa);
        CREDO_NON_NIHIL(actiones);
        CREDO_AEQUALIS_I32(xar_numerus(actiones), II);

        actio = (FabricaActio*)xar_obtinere(actiones, ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(actio->titulus,
            "amalgama_silva");
        CREDO_AEQUALIS_I32((i32)actio->genus,
            (i32)FABRICA_ACTIO_GENERATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS(actio->sedes,
            "silva/aedificatio.stml:2");
        CREDO_AEQUALIS_I32(xar_numerus(actio->mandatum), II);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(actio->mandatum, ZEPHYRUM),
            "./silva/amalgamare.sh");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(actio->mandatum, I), "-comparare");
        CREDO_AEQUALIS_I32(xar_numerus(actio->ingressus), V);
        ingressus = (FabricaIngressus*)xar_obtinere(actio->ingressus,
            ZEPHYRUM);
        CREDO_VERUM(ingressus->genus == mundi_genus("manifestum",
            piscina));
        ingressus = (FabricaIngressus*)xar_obtinere(actio->ingressus,
            III);
        CREDO_VERUM(ingressus->genus == mundi_genus("directorium",
            piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS(ingressus->via, "include");
        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(exitus->via,
            "silva/amalgama/silva.c");
        CREDO_VERUM(exitus->strategia == mundi_strategia("regeneratio",
            piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS(exitus->scriptura,
            "silva/amalgama/silva_nova.c");

        /* memorabilis="verum" (T6) */
        CREDO_VERUM(actio->memorabilis);

        actio = (FabricaActio*)xar_obtinere(actiones, I);
        /* memorabilis absens = FALSUM */
        CREDO_FALSUM(actio->memorabilis);
        CREDO_AEQUALIS_I32((i32)actio->genus,
            (i32)FABRICA_ACTIO_INSTITUTIO);
        CREDO_AEQUALIS_I32(xar_numerus(actio->mandatum), ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(actio->sedes,
            "silva/aedificatio.stml:15");
        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, ZEPHYRUM);
        CREDO_VERUM(exitus->strategia == mundi_strategia("relatio",
            piscina));
        /* scriptura absens = via ipsa */
        CREDO_CHORDA_AEQUALIS_LITERIS(exitus->scriptura, "bin/manus");

        /* recusationes: linea nominata */
        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/decl_genus_ignotum.stml",
            piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:6", piscina));
        CREDO_VERUM(mundi_continet(causa, "compilatio", piscina));

        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/decl_sine_exitu.stml", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:2", piscina));
        CREDO_VERUM(mundi_continet(causa, "sine exitu", piscina));

        /* titulus duplex: directoria scripturae colliderent */
        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/decl_titulus_duplex.stml",
            piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:6", piscina));
        CREDO_VERUM(mundi_continet(causa, "gemina", piscina));

        /* memorabilis nec verum nec falsum -> recusatur */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"generator\""
            " memorabilis=\"fortasse\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"b\" provenientia=\"regeneratio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:2", piscina));
        CREDO_VERUM(mundi_continet(causa, "memorabilis", piscina));

        /* radix: subsystemata */
        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/radix_bona.stml", piscina);
        subsystemata = fabrica_subsystemata_legere(contentum, piscina,
            intern, &causa);
        CREDO_NON_NIHIL(subsystemata);
        CREDO_AEQUALIS_I32(xar_numerus(subsystemata), II);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(subsystemata, I), "tessera");

        /* radix aliena (declaratio, non fabrica) -> NIHIL */
        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/decl_bona.stml", piscina);
        CREDO_NIHIL(fabrica_subsystemata_legere(contentum, piscina,
            intern, &causa));
    }
}


/* ==================================================
 * PROBARE: composita et praecondiciones (plan 1b T2)
 * ================================================== */

interior vacuum
_probare_composita_et_praecondiciones (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        InternamentumChorda* intern;
                     chorda  contentum;
                     chorda  causa;
                        Xar* actiones;
                        Xar* composita;
                        Xar* viae;
                        Xar* iudicia;
                        Xar* ordo;
            FabricaIudicium  iudicium;
            FabricaIudicium* locus;
                        i32  i;

        intern         = internamentum_creare(piscina);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* strategia ignota: numquam iudicatur */
        CREDO_NON_NIHIL(mundi_strategia("ignota", piscina));
        CREDO_FALSUM(mundi_strategia("ignota", piscina)->iudicatur);
        CREDO_VERUM(mundi_strategia("regeneratio", piscina)->iudicatur);
        CREDO_VERUM(mundi_strategia("relatio", piscina)->iudicatur);

        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"x\" provenientia=\"regeneratio\"/>\n"
            "    <exitus via=\"y\" provenientia=\"regeneratio\"/>\n"
            "  </actio>\n"
            "  <actio titulus=\"i\" genus=\"institutio\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"bin/i\" provenientia=\"relatio\"/>\n"
            "  </actio>\n"
            "  <actio titulus=\"o\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"build/o\" provenientia=\"ignota\"/>\n"
            "  </actio>\n"
            "  <compositum titulus=\"omnia\">\n"
            "    <pars actio=\"g\"/>\n"
            "    <pars compositum=\"binaria\"/>\n"
            "    <pars artificium=\"x\"/>\n"
            "  </compositum>\n"
            "  <compositum titulus=\"binaria\">\n"
            "    <pars artificium=\"bin/i\"/>\n"
            "  </compositum>\n"
            "  <compositum titulus=\"cyclus_a\">\n"
            "    <pars compositum=\"cyclus_b\"/>\n"
            "  </compositum>\n"
            "  <compositum titulus=\"cyclus_b\">\n"
            "    <pars compositum=\"cyclus_a\"/>\n"
            "  </compositum>\n"
            "  <compositum titulus=\"orbum\">\n"
            "    <pars artificium=\"nusquam\"/>\n"
            "  </compositum>\n"
            "  <compositum titulus=\"cum_praecondicione\">\n"
            "    <pars artificium=\"build/o\"/>\n"
            "  </compositum>\n"
            "</aedificatio>\n", piscina);
        actiones = fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa);
        CREDO_NON_NIHIL(actiones);
        composita = fabrica_composita_legere(contentum, "d.stml",
            piscina, intern, &causa);
        CREDO_NON_NIHIL(composita);
        CREDO_AEQUALIS_I32(xar_numerus(composita), VI);

        /* planum, sine duplicibus, ordine primae apparitionis -
         * nidificatum == planum (pessimum associativum) */
        viae = fabrica_compositum_explicare(composita, actiones,
            chorda_ex_literis("omnia", piscina), piscina, &causa);
        CREDO_NON_NIHIL(viae);
        CREDO_AEQUALIS_I32(xar_numerus(viae), III);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(viae,
            ZEPHYRUM), "x");
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(viae, I),
            "y");
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(viae, II),
            "bin/i");

        /* cyclus: tituli ambo nominati */
        CREDO_NIHIL(fabrica_compositum_explicare(composita, actiones,
            chorda_ex_literis("cyclus_a", piscina), piscina, &causa));
        CREDO_VERUM(mundi_continet(causa, "cyclus", piscina));
        CREDO_VERUM(mundi_continet(causa, "cyclus_a", piscina));
        CREDO_VERUM(mundi_continet(causa, "cyclus_b", piscina));

        /* pars ignota: sedes et nomen */
        CREDO_NIHIL(fabrica_compositum_explicare(composita, actiones,
            chorda_ex_literis("orbum", piscina), piscina, &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:30", piscina));
        CREDO_VERUM(mundi_continet(causa, "nusquam", piscina));

        /* exitus ignotus pars esse nequit (pessimum semper IGNOTUM) */
        CREDO_NIHIL(fabrica_compositum_explicare(composita, actiones,
            chorda_ex_literis("cum_praecondicione", piscina), piscina,
            &causa));
        CREDO_VERUM(mundi_continet(causa, "praecondicio", piscina));

        /* compositum ignotum */
        CREDO_NIHIL(fabrica_compositum_explicare(composita, actiones,
            chorda_ex_literis("nullum", piscina), piscina, &causa));
        CREDO_VERUM(mundi_continet(causa, "compositum ignotum",
            piscina));

        /* lector: pars nominibus duobus, compositum sine parte */
        CREDO_NIHIL(fabrica_composita_legere(chorda_ex_literis(
            "<aedificatio>\n"
            "  <compositum titulus=\"c\">\n"
            "    <pars actio=\"g\" artificium=\"x\"/>\n"
            "  </compositum>\n"
            "</aedificatio>\n", piscina), "d.stml", piscina, intern,
            &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:3", piscina));
        CREDO_NIHIL(fabrica_composita_legere(chorda_ex_literis(
            "<aedificatio>\n"
            "  <compositum titulus=\"c\">\n"
            "  </compositum>\n"
            "</aedificatio>\n", piscina), "d.stml", piscina, intern,
            &causa));
        CREDO_VERUM(mundi_continet(causa, "sine parte", piscina));

        /* pessimum partium */
        iudicia = xar_creare(piscina, (i32)magnitudo(FabricaIudicium));
        locus = (FabricaIudicium*)xar_addere(iudicia);
        locus->artificium = chorda_ex_literis("x", piscina);
        locus->status = FABRICA_RECENS;
        locus->causa = chorda_ex_literis("", piscina);
        iudicium = fabrica_iudicia_coniungere(iudicia,
            chorda_ex_literis("c", piscina), piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        locus              = (FabricaIudicium*)xar_addere(iudicia);
        locus->artificium  = chorda_ex_literis("z", piscina);
        locus->status      = FABRICA_IGNOTUM;
        locus->causa       = chorda_ex_literis("", piscina);
        iudicium = fabrica_iudicia_coniungere(iudicia,
            chorda_ex_literis("c", piscina), piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "z", piscina));
        locus              = (FabricaIudicium*)xar_addere(iudicia);
        locus->artificium  = chorda_ex_literis("y", piscina);
        locus->status      = FABRICA_STALUM;
        locus->causa       = chorda_ex_literis("", piscina);
        iudicium = fabrica_iudicia_coniungere(iudicia,
            chorda_ex_literis("c", piscina), piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(iudicium.artificium, "c");
        CREDO_VERUM(mundi_continet(iudicium.causa, "y", piscina));
        /* pars non pessima non nominatur */
        CREDO_FALSUM(mundi_continet(iudicium.causa, "z", piscina));
        /* plures quam III pessimae: "+N" */
        per (i = ZEPHYRUM; i < IV; i++)
        {
            locus              = (FabricaIudicium*)xar_addere(iudicia);
            locus->artificium  = chorda_ex_literis("w", piscina);
            locus->status      = FABRICA_STALUM;
            locus->causa       = chorda_ex_literis("", piscina);
        }
        iudicium = fabrica_iudicia_coniungere(iudicia,
            chorda_ex_literis("c", piscina), piscina);
        CREDO_VERUM(mundi_continet(iudicium.causa, "+II", piscina));

        /* praecondiciones: p praecondicionem o nominat, ante eam
         * declarata - ordo o ante p */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"p\" genus=\"institutio\">\n"
            "    <praecondicio actio=\"o\"/>\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"bin/p\" provenientia=\"relatio\"/>\n"
            "  </actio>\n"
            "  <actio titulus=\"o\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"build/o\" provenientia=\"ignota\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        actiones = fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa);
        CREDO_NON_NIHIL(actiones);
        CREDO_AEQUALIS_I32(xar_numerus(((FabricaActio*)xar_obtinere(
            actiones, ZEPHYRUM))->praecondiciones), I);
        CREDO_VERUM(fabrica_praecondiciones_probare(actiones, piscina,
            &causa));
        ordo = fabrica_ordinare(actiones, piscina, &causa);
        CREDO_NON_NIHIL(ordo);
        CREDO_CHORDA_AEQUALIS_LITERIS((*(FabricaActio**)xar_obtinere(
            ordo, ZEPHYRUM))->titulus, "o");

        /* praecondicio ignota */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"p\" genus=\"institutio\">\n"
            "    <praecondicio actio=\"nulla\"/>\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"bin/p\" provenientia=\"relatio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        actiones = fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa);
        CREDO_FALSUM(fabrica_praecondiciones_probare(actiones, piscina,
            &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:2", piscina));
        CREDO_VERUM(mundi_continet(causa, "nulla", piscina));

        /* exitus ignotus ut INGRESSUS: praecondicio sola licet */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"q\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"build/o\"/>\n"
            "    <exitus via=\"z\" provenientia=\"regeneratio\"/>\n"
            "  </actio>\n"
            "  <actio titulus=\"o\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"build/o\" provenientia=\"ignota\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        actiones = fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa);
        CREDO_FALSUM(fabrica_praecondiciones_probare(actiones, piscina,
            &causa));
        CREDO_VERUM(mundi_continet(causa, "praecondicio", piscina));
        CREDO_VERUM(mundi_continet(causa, "build/o", piscina));
    }
}


/* ==================================================
 * PROBARE: familiae (plan 2 T4; ex probatio_fabrica_genera)
 * ================================================== */

interior vacuum
_probare_familias (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
               DiscusFictus  discus;
              FabricaSutura  sutura;
        InternamentumChorda* intern;
                     chorda  contentum;
                     chorda  causa;
                        Xar* actiones;
         constans character* plagulae[II];

        intern         = internamentum_creare(piscina);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* FAMILIA (plan 2 T4): actio una per plagulam congruentem
         * directorii (enumerare suturae); templum STML (#@ ... &@x;)
         * per stml_expandere impletur - basis et fons argumenta
         * fabricae; titulus 'familia:basis', fons ingressus. */
        {
              constans character* familiae_nomina[V];
              constans character* familia_textus;
                    FabricaActio* instantia;
                FabricaIngressus* ultimus;

            familia_textus =
                "<aedificatio>\n"
                "  <actio titulus=\"a\" genus=\"generator\">\n"
                "    <ingressus genus=\"fasciculus\" via=\"x\"/>\n"
                "    <exitus via=\"y\" provenientia=\"regeneratio\"/>\n"
                "  </actio>\n"
                "  <familia titulus=\"pt\" via=\"t\" praefixum=\"probatio_\""
                " suffixum=\".c\">\n"
                "    <#@instantia basis=\"@basis\" fons=\"@fons\">\n"
                "      <actio genus=\"generator\" lectiones=\"verum\">\n"
                "        <mandatum>\n"
                "          <verbum! (>./struere.sh\n"
                "          <verbum! (>&@fons;\n"
                "        </mandatum>\n"
                "        <exitus via=\"build/t/&@basis;\""
                " provenientia=\"regeneratio\"/>\n"
                "      </actio>\n"
                "    </#>\n"
                "  </familia>\n"
                "</aedificatio>\n";
            mundi_discum_parare(&discus, &sutura, piscina);
            familiae_nomina[0] = "adiumentum.c";
            familiae_nomina[1] = "probatio_a.c";
            familiae_nomina[2] = "probatio_b.c";
            familiae_nomina[3] = "probatio_c.c";
            familiae_nomina[4] = "probatio_d.h";
            mundi_directorium_ponere(&discus, "t", familiae_nomina, V);
            contentum = chorda_ex_literis(familia_textus, piscina);
            actiones =
                fabrica_declarationes_legere_cum_sutura(contentum,
                "d.stml", &sutura, piscina, intern, &causa);
            CREDO_NON_NIHIL(actiones);
            si (actiones != NIHIL)
            {
                /* III congruentes (adiumentum et .h non) + actio a */
                CREDO_AEQUALIS_I32(xar_numerus(actiones), IV);
                instantia = (FabricaActio*)xar_obtinere(actiones, I);
                CREDO_VERUM(chorda_aequalis_literis(instantia->titulus,
                    "pt:probatio_a"));
                CREDO_VERUM(instantia->lectiones);
                CREDO_AEQUALIS_I32(xar_numerus(instantia->mandatum),
                    II);
                CREDO_VERUM(chorda_aequalis_literis(*(chorda*)
                    xar_obtinere(instantia->mandatum, I),
                    "t/probatio_a.c"));
                CREDO_VERUM(chorda_aequalis_literis(((FabricaExitus*)
                    xar_obtinere(instantia->exitus, ZEPHYRUM))->via,
                    "build/t/probatio_a"));
                ultimus = (FabricaIngressus*)xar_obtinere(
                    instantia->ingressus,
                    xar_numerus(instantia->ingressus) - I);
                CREDO_VERUM(chorda_aequalis_literis(ultimus->via,
                    "t/probatio_a.c"));
                CREDO_VERUM(ultimus->genus == mundi_genus("fasciculus",
                    piscina));
                instantia = (FabricaActio*)xar_obtinere(actiones, III);
                CREDO_VERUM(chorda_aequalis_literis(instantia->titulus,
                    "pt:probatio_c"));
            }

            /* plagula nova -> actio nova */
            mundi_discum_parare(&discus, &sutura, piscina);
            familiae_nomina[4] = "probatio_e.c";
            mundi_directorium_ponere(&discus, "t", familiae_nomina, V);
            actiones =
                fabrica_declarationes_legere_cum_sutura(contentum,
                "d.stml", &sutura, piscina, intern, &causa);
            CREDO_NON_NIHIL(actiones);
            si (actiones != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(actiones), V);
            }

            /* sine sutura (lector purus): familia recusatur */
            CREDO_NIHIL(fabrica_declarationes_legere(contentum,
                "d.stml",
                piscina, intern, &causa));
            CREDO_VERUM(mundi_continet(causa, "familia", piscina));

            /* directorium absens: recusatur, nominatum */
            mundi_discum_parare(&discus, &sutura, piscina);
            CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
                "d.stml", &sutura, piscina, intern, &causa));
            CREDO_VERUM(mundi_continet(causa, "directorium", piscina));

            /* titulus in templo: recusatur (fabrica titulum ponit) */
            mundi_directorium_ponere(&discus, "t", familiae_nomina, V);
            contentum = chorda_ex_literis(
                "<aedificatio>\n"
                "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
                "    <#@instantia basis=\"@basis\">\n"
                "      <actio titulus=\"x\" genus=\"generator\">\n"
                "        <exitus via=\"b/&@basis;\""
                " provenientia=\"regeneratio\"/>\n"
                "      </actio>\n"
                "    </#>\n"
                "  </familia>\n"
                "</aedificatio>\n", piscina);
            CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
                "d.stml", &sutura, piscina, intern, &causa));
            CREDO_VERUM(mundi_continet(causa, "titulus", piscina));

            /* loculus non declaratus in corpore: vitium expansionis */
            contentum = chorda_ex_literis(
                "<aedificatio>\n"
                "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
                "    <#@instantia basis=\"@basis\">\n"
                "      <actio genus=\"generator\">\n"
                "        <exitus via=\"b/&@nescio;\""
                " provenientia=\"regeneratio\"/>\n"
                "      </actio>\n"
                "    </#>\n"
                "  </familia>\n"
                "</aedificatio>\n", piscina);
            CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
                "d.stml", &sutura, piscina, intern, &causa));
            CREDO_VERUM(mundi_continet(causa, "expansio", piscina));
        }

        /* RECUSATIONES FAMILIAE (fabrica-6, ante _familiam_explicare
         * per gradus): omnis recusatio sedem et causam suam nominat */
        mundi_discum_parare(&discus, &sutura, piscina);
        plagulae[0] = "a.c";
        plagulae[1] = "b.c";
        mundi_directorium_ponere(&discus, "t", plagulae, II);

        /* familia sine via, sine titulo */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia titulus=\"pt\">\n"
            "    <#@instantia basis=\"@basis\">\n"
            "      <actio genus=\"generator\">\n"
            "        <exitus via=\"b/&@basis;\""
            " provenientia=\"regeneratio\"/>\n"
            "      </actio>\n"
            "    </#>\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "d.stml:2", piscina));
        CREDO_VERUM(mundi_continet(causa, "sine titulo aut via",
            piscina));
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia via=\"t\">\n"
            "    <#@instantia basis=\"@basis\">\n"
            "      <actio genus=\"generator\">\n"
            "        <exitus via=\"b/&@basis;\""
            " provenientia=\"regeneratio\"/>\n"
            "      </actio>\n"
            "    </#>\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "sine titulo aut via",
            piscina));

        /* templa duo; elementum quod templum non est */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
            "    <#@prima basis=\"@basis\">\n"
            "      <actio genus=\"generator\">\n"
            "        <exitus via=\"b/&@basis;\""
            " provenientia=\"regeneratio\"/>\n"
            "      </actio>\n"
            "    </#>\n"
            "    <#@altera basis=\"@basis\">\n"
            "      <actio genus=\"generator\">\n"
            "        <exitus via=\"c/&@basis;\""
            " provenientia=\"regeneratio\"/>\n"
            "      </actio>\n"
            "    </#>\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "templum UNUM", piscina));
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
            "    <actio genus=\"generator\">\n"
            "      <exitus via=\"b\" provenientia=\"regeneratio\"/>\n"
            "    </actio>\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "templum UNUM", piscina));

        /* familia sine templo */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "sine templo", piscina));

        /* corpus templi: actio nulla, actiones duae, elementum
         * alienum */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
            "    <#@instantia basis=\"@basis\">\n"
            "    </#>\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "actionem UNAM", piscina));
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
            "    <#@instantia basis=\"@basis\">\n"
            "      <actio genus=\"generator\">\n"
            "        <exitus via=\"b/&@basis;\""
            " provenientia=\"regeneratio\"/>\n"
            "      </actio>\n"
            "      <actio genus=\"generator\">\n"
            "        <exitus via=\"c/&@basis;\""
            " provenientia=\"regeneratio\"/>\n"
            "      </actio>\n"
            "    </#>\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "actionem UNAM", piscina));
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
            "    <#@instantia basis=\"@basis\">\n"
            "      <nota/>\n"
            "    </#>\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "actionem UNAM", piscina));

        /* nomen plagulae cum '&' aut '"': recusatur, nominatum */
        mundi_discum_parare(&discus, &sutura, piscina);
        plagulae[0] = "a&b.c";
        mundi_directorium_ponere(&discus, "t", plagulae, I);
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <familia titulus=\"pt\" via=\"t\" suffixum=\".c\">\n"
            "    <#@instantia basis=\"@basis\">\n"
            "      <actio genus=\"generator\">\n"
            "        <exitus via=\"b/&@basis;\""
            " provenientia=\"regeneratio\"/>\n"
            "      </actio>\n"
            "    </#>\n"
            "  </familia>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "nomen plagulae", piscina));
        CREDO_VERUM(mundi_continet(causa, "a&b.c", piscina));
        mundi_discum_parare(&discus, &sutura, piscina);
        plagulae[0] = "a\"b.c";
        mundi_directorium_ponere(&discus, "t", plagulae, I);
        CREDO_NIHIL(fabrica_declarationes_legere_cum_sutura(contentum,
            "d.stml", &sutura, piscina, intern, &causa));
        CREDO_VERUM(mundi_continet(causa, "nomen plagulae", piscina));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "declarationes",
      _probare_declarationes,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "composita et praecondiciones",
      _probare_composita_et_praecondiciones,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "familias",
      _probare_familias,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("fabrica_declarationes", SECTIONES);
}
