/* probatio_fabrica_ordo.c - probationes fabricae (fabrica-6 H3),
 * pars ordo: ordo dependentiae, vestigia scripturae et undae,
 * dependentiae per locos
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

/* vestigia ULTIMA tituli (omnes exitus) - ordo ex lectionibus */
interior b32
_lectiones_ultimae (
                vacuum*  datum,
    constans character*  titulus,
               Piscina*  piscina,
                   Xar** lectiones_out)
{
    DiscusFictus* discus;
             Xar* omnes;
             i32  i;
             i32  j;

    discus  = (DiscusFictus*)datum;
    omnes   = xar_creare(piscina, (i32)magnitudo(FabricaLectio));
    per (i = ZEPHYRUM; i < xar_numerus(discus->vestigia_lectionum); i++)
    {
        VestigiumLectionumFictum* v;

        v = (VestigiumLectionumFictum*)xar_obtinere(
            discus->vestigia_lectionum, i);
        si (!chorda_aequalis_literis(v->titulus, titulus))
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(v->lectiones); j++)
        {
            *(FabricaLectio*)xar_addere(omnes) =
                *(FabricaLectio*)xar_obtinere(v->lectiones, j);
        }
    }
    si (xar_numerus(omnes) == 0)
    {
        redde FALSUM;
    }
    *lectiones_out = omnes;
    redde VERUM;
}

interior vacuum
_locum_addere_test (
                   Xar* loci,
      FabricaFormaLoci  forma,
    constans character* via,
    constans character* suffixa,
               Piscina* piscina)
{
    FabricaLocus* locus;

    locus           = (FabricaLocus*)xar_addere(loci);
    locus->forma    = forma;
    locus->via      = chorda_ex_literis(via, piscina);
    locus->suffixa  = chorda_ex_literis(suffixa, piscina);
}

/* photographia ficta: plagulae non deletae, tempus = horologium */
interior Xar*
_photographia (
    FabricaSutura* sutura,
          Piscina* piscina)
{
    Xar* vestigia;

    vestigia = NIHIL;
    (vacuum)sutura->vestigium_capere(sutura->datum, piscina, &vestigia);
    redde vestigia;
}


s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum(
        "probatio_fabrica_ordo", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ================================================== */

    /* PROBARE: ordo dependentiae                          */


    /* ================================================== */

    {
                 Xar* actiones;
                 Xar* ordo;
              chorda  causa;
        FabricaActio* a;
        FabricaActio* b;

        imprimere("\n--- Probans ordinem ---\n");
        actiones = xar_creare(piscina, (i32)magnitudo(FabricaActio));

        /* B legit exitum A; datae ordine B, A -> A ante B */
        b = mundi_actio(piscina, "B", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(b, "fasciculus", "gen/a.h",
            piscina);
        (vacuum)mundi_exitum_addere(b, "gen/b.c",
            "regeneratio", piscina);
        a = mundi_actio(piscina, "A", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "fasciculus", "data/a",
            piscina);
        (vacuum)mundi_exitum_addere(a, "gen/a.h",
            "regeneratio", piscina);
        *(FabricaActio*)xar_addere(actiones) = *b;
        *(FabricaActio*)xar_addere(actiones) = *a;

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        ordo           = fabrica_ordinare(actiones, piscina, &causa);
        CREDO_NON_NIHIL(ordo);
        CREDO_AEQUALIS_I32(xar_numerus(ordo), II);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            (*(FabricaActio**)xar_obtinere(ordo, 0))->titulus, "A");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            (*(FabricaActio**)xar_obtinere(ordo, 1))->titulus, "B");

        /* cyclus C <-> D -> NIHIL, ambo nominati */
        actiones  = xar_creare(piscina, (i32)magnitudo(FabricaActio));
        a         = mundi_actio(piscina, "C", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "fasciculus", "gen/d",
            piscina);
        (vacuum)mundi_exitum_addere(a, "gen/c",
            "regeneratio", piscina);
        b = mundi_actio(piscina, "D", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(b, "fasciculus", "gen/c",
            piscina);
        (vacuum)mundi_exitum_addere(b, "gen/d",
            "regeneratio", piscina);
        *(FabricaActio*)xar_addere(actiones) = *a;
        *(FabricaActio*)xar_addere(actiones) = *b;
        ordo = fabrica_ordinare(actiones, piscina, &causa);
        CREDO_NIHIL(ordo);
        CREDO_VERUM(mundi_continet(causa, "C", piscina));
        CREDO_VERUM(mundi_continet(causa, "D", piscina));
        CREDO_VERUM(mundi_continet(causa, "cyclus", piscina));
    }


    /* ==================================================
     * PROBARE: vestigia et undae (plan 1b T4)
     * ================================================== */

    {
          DiscusFictus  discus;
         FabricaSutura  sutura;
          FabricaActio* a;
          FabricaActio* actiones[III];
                   Xar* ante;
                   Xar* extra;
                   Xar* ordo;
                   Xar* undae;
                   Xar* sanationes;
        FabricaSanatio* sanatio;
                chorda  causa;

        imprimere("\n--- Probans vestigia et undas ---\n");
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_ponere(&discus, "vetus.txt", "v\n");
        mundi_ponere(&discus, "fixum.c", "int f;\n");
        mundi_ponere(&discus, "build/x.o", "o\n");
        a = mundi_actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        _locum_addere_test(a->vestigia, FABRICA_LOCUS_ARBOR, "build/a",
            "", piscina);
        _locum_addere_test(a->vestigia, FABRICA_LOCUS_PLAGULAE, "build",
            ".o", piscina);
        _locum_addere_test(a->communia, FABRICA_LOCUS_ARBOR,
            "build/obiecta", "", piscina);

        /* intra vestigium: exitus, opera, communia, involucrum */
        ante = _photographia(&sutura, piscina);
        mundi_ponere(&discus, "X", "novum\n");
        mundi_ponere(&discus, "build/a/sub/w.o", "w\n");
        mundi_ponere(&discus, "build/obiecta/z.o", "z\n");
        mundi_ponere(&discus, "build/x.o", "o2\n");
        mundi_ponere(&discus, "build/fabrica/acta/A.log", "acta\n");
        mundi_ponere(&discus, "build/fabrica/provenientia/A.c", "p\n");
        mundi_ponere(&discus, "build/fabrica/provenientia/A.o", "p\n");
        mundi_ponere(&discus, "build/fabrica/scriptura/A/X", "s\n");
        /* status executoris (T4: mensor_ui -shm tetigit) */
        mundi_ponere(&discus, "build/fabrica.db-shm", "shm\n");
        mundi_ponere(&discus, "build/fabrica/sera", "");
        extra = fabrica_vestigia_comparare(a, ante,
            _photographia(&sutura, piscina), piscina);
        CREDO_NON_NIHIL(extra);
        CREDO_AEQUALIS_I32(xar_numerus(extra), ZEPHYRUM);

        /* extra: plagula nova, deleta, rescripta octetis eisdem,
         * plagulae suffixo non congruens aut gradu altiore,
         * provenientia ACTIONIS ALTERIUS */
        ante = _photographia(&sutura, piscina);
        mundi_ponere(&discus, "alia.txt", "alia\n");
        mundi_auferre(&discus, "vetus.txt");
        mundi_ponere(&discus, "fixum.c", "int f;\n");
        mundi_ponere(&discus, "build/x.c", "c\n");
        mundi_ponere(&discus, "build/sub/y.o", "y\n");
        mundi_ponere(&discus, "build/fabrica/provenientia/B.c", "p\n");
        extra = fabrica_vestigia_comparare(a, ante,
            _photographia(&sutura, piscina), piscina);
        CREDO_AEQUALIS_I32(xar_numerus(extra), VI);
        CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(extra,
            ZEPHYRUM), "alia.txt");

        /* sanare: scriptura extra vestigium -> FRACTUM nominans eam */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        ((ScriptumFictum*)xar_obtinere(discus.scripta,
            ZEPHYRUM))->alia =
            "apps/commissum.c";
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        ordo = mundi_ordinare_fictas(piscina, actiones, I);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "A");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_FRACTUM
            && mundi_continet(sanatio->causa, "extra vestigium",
            piscina)
            && mundi_continet(sanatio->causa, "apps/commissum.c",
            piscina));

        /* undae: A et D independentes, disiunctae -> unda una */
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "D", "gen_d", "a",
            "Z",
            "regeneratio");
        ordo   = mundi_ordinare_fictas(piscina, actiones, II);
        undae  = fabrica_undas_formare(ordo, piscina);
        CREDO_NON_NIHIL(undae);
        CREDO_AEQUALIS_I32(xar_numerus(undae), I);
        CREDO_AEQUALIS_I32(xar_numerus(*(Xar**)xar_obtinere(undae,
            ZEPHYRUM)), II);

        /* dependentia (C ex X) -> undae duae */
        actiones[1] = mundi_actio_scripta(piscina, "C", "gen_c", "X",
            "Y",
            "regeneratio");
        ordo   = mundi_ordinare_fictas(piscina, actiones, II);
        undae  = fabrica_undas_formare(ordo, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(undae), II);

        /* communia (etiam disiuncta alioquin) -> undae duae */
        actiones[1] = mundi_actio_scripta(piscina, "D", "gen_d", "a",
            "Z",
            "regeneratio");
        _locum_addere_test(actiones[0]->communia, FABRICA_LOCUS_ARBOR,
            "build/obiecta", "", piscina);
        ordo   = mundi_ordinare_fictas(piscina, actiones, II);
        undae  = fabrica_undas_formare(ordo, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(undae), II);

        /* lector: vestigium (arbor ordinaria), plagulae cum suffixis,
         * communis -> communia; forma ignota recusatur */
        {
             InternamentumChorda* intern;
                             Xar* lectae;
                    FabricaActio* lecta;

            intern = internamentum_creare(piscina);
            lectae = fabrica_declarationes_legere(chorda_ex_literis(
                "<aedificatio>\n"
                "  <actio titulus=\"g\" genus=\"generator\">\n"
                "    <vestigium via=\"build/g\"/>\n"
                "    <vestigium via=\"build\" forma=\"plagulae\""
                " suffixa=\".o\"/>\n"
                "    <vestigium via=\"build/aedilis/obiecta\""
                " communis=\"verum\"/>\n"
                "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
                "    <exitus via=\"b\" provenientia=\"regeneratio\"/>\n"
                "  </actio>\n"
                "</aedificatio>\n", piscina), "d.stml", piscina, intern,
                &causa);
            CREDO_NON_NIHIL(lectae);
            lecta = (FabricaActio*)xar_obtinere(lectae, ZEPHYRUM);
            CREDO_AEQUALIS_I32(xar_numerus(lecta->vestigia), II);
            CREDO_AEQUALIS_I32(xar_numerus(lecta->communia), I);
            CREDO_AEQUALIS_I32((i32)((FabricaLocus*)xar_obtinere(
                lecta->vestigia, ZEPHYRUM))->forma,
                (i32)FABRICA_LOCUS_ARBOR);
            CREDO_CHORDA_AEQUALIS_LITERIS(((FabricaLocus*)xar_obtinere(
                lecta->vestigia, I))->suffixa, ".o");
            CREDO_NIHIL(fabrica_declarationes_legere(chorda_ex_literis(
                "<aedificatio>\n"
                "  <actio titulus=\"g\" genus=\"generator\">\n"
                "    <vestigium via=\"x\" forma=\"nebula\"/>\n"
                "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
                "    <exitus via=\"b\" provenientia=\"regeneratio\"/>\n"
                "  </actio>\n"
                "</aedificatio>\n", piscina), "d.stml", piscina, intern,
                &causa));
            CREDO_VERUM(mundi_continet(causa, "d.stml:3", piscina));
        }

        /* vestigia concurrentia (eadem arbor) -> undae duae */
        actiones[0] = mundi_actio_scripta(piscina, "A", "gen_a", "a",
            "X",
            "regeneratio");
        _locum_addere_test(actiones[0]->vestigia, FABRICA_LOCUS_ARBOR,
            "build/opera", "", piscina);
        _locum_addere_test(actiones[1]->vestigia, FABRICA_LOCUS_ARBOR,
            "build/opera/sub", "", piscina);
        ordo   = mundi_ordinare_fictas(piscina, actiones, II);
        undae  = fabrica_undas_formare(ordo, piscina);
        CREDO_AEQUALIS_I32(xar_numerus(undae), II);
    }


    /* ==================================================
     * PROBARE: dependentiae per locos (plan 1b T5)
     * ================================================== */

    {
          DiscusFictus  discus;
         FabricaSutura  sutura;
          FabricaActio* g;
          FabricaActio* b;
          FabricaActio* t;
          FabricaActio* u;
                   Xar* actiones;
                   Xar* ordo;
                   Xar* sanationes;
        FabricaSanatio* sanatio;
                chorda  causa;

        imprimere("\n--- Probans dependentias per locos ---\n");
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* I. arcus per CLAUSURAM manifesti: B (declarata ante G) legit
         * gen/x.h solum per manifestum suum - G eum producit. Sine
         * computatione: nullus arcus, ordo declarationis (B primum) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "tools/b.c", "int b;\n");
        mundi_ponere(&discus, "gen/x.h", "vetus\n");
        mundi_ponere(&discus, "bin/b", "b\n");
        mundi_ponere(&discus, "build/b.stml",
            "<aedilis-manifestum scopus=\"tools/b.c\">\n"
            "  <capita><caput via=\"gen/x.h\"/></capita>\n"
            "</aedilis-manifestum>\n");
        mundi_scriptum_addere(&discus, "gen_g", "gen/x.h", NIHIL,
            "novum\n",
            0, FALSUM);
        /* B nunc recens: generator eius "b" scribit, ut bin/b */
        mundi_scriptum_addere(&discus, "gen_b", "bin/b", NIHIL, "b\n",
            0,
            FALSUM);
        b = mundi_actio_scripta(piscina, "B", "gen_b", "a", "bin/b",
            "regeneratio");
        mundi_ingressum_addere(b, "manifestum", "build/b.stml",
            piscina);
        g = mundi_actio_scripta(piscina, "G", "gen_g", "a", "gen/x.h",
            "regeneratio");
        actiones = xar_creare(piscina, (i32)magnitudo(FabricaActio));
        *(FabricaActio*)xar_addere(actiones) = *b;
        *(FabricaActio*)xar_addere(actiones) = *g;
        ordo = fabrica_ordinare(actiones, piscina, &causa);
        CREDO_CHORDA_AEQUALIS_LITERIS((*(FabricaActio**)xar_obtinere(
            ordo, ZEPHYRUM))->titulus, "B");
        CREDO_VERUM(fabrica_dependentias_computare(&sutura, actiones,
            piscina, &causa));
        ordo = fabrica_ordinare(actiones, piscina, &causa);
        CREDO_NON_NIHIL(ordo);
        CREDO_CHORDA_AEQUALIS_LITERIS((*(FabricaActio**)xar_obtinere(
            ordo, ZEPHYRUM))->titulus, "G");
        /* sanare -siccum sequitur: G agendum, B post eam fortasse */
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, VERUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "B");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_FORTASSE);

        /* II. instrumentum nihil enumerat: U instrumentum bin/t
         * adhibet,
         * T (producens bin/t) clausuram cum exitu U legit - cyclus
         * bootstrap nullus */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "tools/t.c", "int t;\n");
        mundi_ponere(&discus, "gen/u.c", "u\n");
        mundi_ponere(&discus, "bin/t", "t\n");
        mundi_ponere(&discus, "build/t.stml",
            "<aedilis-manifestum scopus=\"tools/t.c\">\n"
            "  <obiecta><obiectum via=\"gen/u.c\"/></obiecta>\n"
            "</aedilis-manifestum>\n");
        t = mundi_actio(piscina, "T", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(t, "manifestum", "build/t.stml",
            piscina);
        (vacuum)mundi_exitum_addere(t, "bin/t", "relatio", piscina);
        u = mundi_actio(piscina, "U", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(u, "instrumentum", "bin/t", piscina);
        mundi_ingressum_addere(u, "fasciculus", "a", piscina);
        (vacuum)mundi_exitum_addere(u, "gen/u.c", "regeneratio",
            piscina);
        actiones = xar_creare(piscina, (i32)magnitudo(FabricaActio));
        *(FabricaActio*)xar_addere(actiones) = *t;
        *(FabricaActio*)xar_addere(actiones) = *u;
        CREDO_VERUM(fabrica_dependentias_computare(&sutura, actiones,
            piscina, &causa));
        ordo = fabrica_ordinare(actiones, piscina, &causa);
        CREDO_NON_NIHIL(ordo);
        CREDO_CHORDA_AEQUALIS_LITERIS((*(FabricaActio**)xar_obtinere(
            ordo, ZEPHYRUM))->titulus, "U");
        CREDO_AEQUALIS_I32(xar_numerus(((FabricaActio*)xar_obtinere(
            actiones, I))->dependentiae), ZEPHYRUM);

        /* III. clonus recens: ingressus absens (gen/y nondum scriptum)
         * -> via declarata arcum dat; IV. praelatio: actio exitum suum
         * legens sibi non pendet */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        g = mundi_actio_scripta(piscina, "G", "gen_g", "a", "gen/y",
            "regeneratio");
        mundi_ingressum_addere(g, "fasciculus", "gen/y", piscina);
        b = mundi_actio_scripta(piscina, "B", "gen_b", "gen/y", "gen/z",
            "regeneratio");
        actiones = xar_creare(piscina, (i32)magnitudo(FabricaActio));
        *(FabricaActio*)xar_addere(actiones) = *b;
        *(FabricaActio*)xar_addere(actiones) = *g;
        CREDO_VERUM(fabrica_dependentias_computare(&sutura, actiones,
            piscina, &causa));
        ordo = fabrica_ordinare(actiones, piscina, &causa);
        CREDO_NON_NIHIL(ordo);
        CREDO_CHORDA_AEQUALIS_LITERIS((*(FabricaActio**)xar_obtinere(
            ordo, ZEPHYRUM))->titulus, "G");
        CREDO_AEQUALIS_I32(xar_numerus(((FabricaActio*)xar_obtinere(
            actiones, I))->dependentiae), ZEPHYRUM);

        /* VI. ORDO EX VESTIGIO (plan 2 T2): S lectiones="verum"
         * nihil de gen/x.h declarat; vestigium eius id legit, G id
         * producit -> S post G. Sine vestigio nullus arcus (clonus
         * recens: ut manifesta absentia olim) */
        {
            FabricaActio* s_act;

            mundi_discum_parare(&discus, &sutura, piscina);
            sutura.lectiones_legere    = mundi_lectiones_legere;
            sutura.lectiones_scribere  = mundi_lectiones_scribere;
            sutura.lectiones_ultimae   = _lectiones_ultimae;
            mundi_ponere(&discus, "a", "a\n");
            mundi_ponere(&discus, "gen/x.h", "x\n");
            mundi_ponere(&discus, "gen/s.c", "s\n");
            s_act = mundi_actio_scripta(piscina, "S", "gen_s", "a",
                "gen/s.c",
                "regeneratio");
            s_act->lectiones = VERUM;
            g = mundi_actio_scripta(piscina, "G", "gen_g", "a",
                "gen/x.h",
                "regeneratio");
            actiones = xar_creare(piscina,
                (i32)magnitudo(FabricaActio));
            *(FabricaActio*)xar_addere(actiones) = *s_act;
            *(FabricaActio*)xar_addere(actiones) = *g;
            CREDO_VERUM(fabrica_dependentias_computare(&sutura,
                actiones, piscina, &causa));
            CREDO_AEQUALIS_I32(xar_numerus(((FabricaActio*)xar_obtinere(
                actiones, ZEPHYRUM))->dependentiae), ZEPHYRUM);
            {
                VestigiumLectionumFictum* v;
                           FabricaLectio* l;

                v = (VestigiumLectionumFictum*)xar_addere(
                    discus.vestigia_lectionum);
                v->titulus  = chorda_ex_literis("S", piscina);
                v->exitus   = chorda_ex_literis("gen/s.c", piscina);
                v->lectiones  = xar_creare(piscina,
                    (i32)magnitudo(FabricaLectio));
                l         = (FabricaLectio*)xar_addere(v->lectiones);
                l->genus  = LECTIO_LEGIT;
                l->via    = chorda_ex_literis("gen/x.h", piscina);
            }
            CREDO_VERUM(fabrica_dependentias_computare(&sutura,
                actiones, piscina, &causa));
            CREDO_AEQUALIS_I32(xar_numerus(((FabricaActio*)xar_obtinere(
                actiones, ZEPHYRUM))->dependentiae), I);
        }

        /* V. radices nihil enumerant (parcum …AR15): R radices
         * aedilis.stml sigillat (radix 'gen'), P in 'gen' scribit et
         * exitum R legit. Radices = sigillum obumbrationis, ordo ex
         * manifestis - aliter cyclus (catena amalgamatoris: fontes_X
         * radicem silva/amalgama sigillat, amalgama_X ibi scribit) */
        {
            constans character* gen_nomina[I];

            mundi_discum_parare(&discus, &sutura, piscina);
            mundi_ponere(&discus, "a", "a\n");
            mundi_ponere(&discus, "aedilis.stml",
                "<aedilis>\n"
                "  <inclusa>\n"
                "    <via (>gen\n"
                "  </inclusa>\n"
                "</aedilis>\n");
            gen_nomina[0] = "p.h";
            mundi_directorium_ponere(&discus, "gen", gen_nomina, I);
            b = mundi_actio_scripta(piscina, "P", "gen_p", "out/r",
                "gen/p.h", "regeneratio");
            g = mundi_actio_scripta(piscina, "R", "gen_r", "a", "out/r",
                "regeneratio");
            mundi_ingressum_addere(g, "radices", "aedilis.stml",
                piscina);
            actiones = xar_creare(piscina,
                (i32)magnitudo(FabricaActio));
            *(FabricaActio*)xar_addere(actiones) = *b;
            *(FabricaActio*)xar_addere(actiones) = *g;
            CREDO_VERUM(fabrica_dependentias_computare(&sutura,
                actiones, piscina, &causa));
            ordo = fabrica_ordinare(actiones, piscina, &causa);
            CREDO_NON_NIHIL(ordo);
            si (ordo != NIHIL)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS((*(FabricaActio**)
                    xar_obtinere(ordo, ZEPHYRUM))->titulus, "R");
            }
            CREDO_AEQUALIS_I32(xar_numerus(((FabricaActio*)
                xar_obtinere(actiones, I))->dependentiae), ZEPHYRUM);
        }
    }

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
