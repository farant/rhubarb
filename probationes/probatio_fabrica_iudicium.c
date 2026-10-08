/* probatio_fabrica_iudicium.c - probationes fabricae (fabrica-6 H3),
 * pars iudicium: regeneratio, relatio, vestigia lectionum, memoria
 * verificationum, praevisio, celer, porta ut actio, transitus, signum
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

interior b32
_meminisse (
                vacuum* datum,
    constans character* titulus,
     constans Sigillum* ingressus,
     constans Sigillum* artificium)
{
    DiscusFictus* discus;
             i32  i;

    discus = (DiscusFictus*)datum;
    per (i = ZEPHYRUM; i < xar_numerus(discus->verificationes); i++)
    {
        VerificatioFicta* v;

        v = (VerificatioFicta*)xar_obtinere(discus->verificationes, i);
        si (   chorda_aequalis_literis(v->titulus, titulus)
            && memcmp(v->ingressus.octeti, ingressus->octeti,
                   SIGILLUM_OCTETI) == 0
            && memcmp(v->artificium.octeti, artificium->octeti,
                   SIGILLUM_OCTETI) == 0)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_inscribere (
                vacuum* datum,
    constans character* titulus,
     constans Sigillum* ingressus,
     constans Sigillum* artificium)
{
        DiscusFictus* discus;
    VerificatioFicta* v;

    discus = (DiscusFictus*)datum;
    discus->inscriptiones++;
    v = (VerificatioFicta*)xar_addere(discus->verificationes);
    si (v == NIHIL)
    {
        redde;
    }
    v->titulus     = chorda_ex_literis(titulus, discus->piscina);
    v->ingressus   = *ingressus;
    v->artificium  = *artificium;
}

/* particulae transitus fictae (plan-5 T1): ut tabula vera, ultimae per
 * (titulus, exitus) */
interior vacuum
_particulas_scribere (
                vacuum* datum,
    constans character* titulus,
    constans character* exitus,
          constans Xar* particulae)
{
        DiscusFictus* discus;
    ParticulaeFictae* p = NIHIL;
                 i32  i;

    discus = (DiscusFictus*)datum;
    per (i = ZEPHYRUM; i
        < xar_numerus(discus->particulae_servatae); i++)
    {
        ParticulaeFictae* q = (ParticulaeFictae*)xar_obtinere(
            discus->particulae_servatae, i);

        si (   chorda_aequalis_literis(q->titulus, titulus)
            && chorda_aequalis_literis(q->exitus, exitus))
        {
            p = q;
        }
    }
    si (p == NIHIL)
    {
        p = (ParticulaeFictae*)xar_addere(discus->particulae_servatae);
        si (p == NIHIL)
        {
            redde;
        }
        p->titulus  = chorda_ex_literis(titulus, discus->piscina);
        p->exitus   = chorda_ex_literis(exitus, discus->piscina);
    }
    p->particulae = xar_creare(discus->piscina,
        (i32)magnitudo(FabricaParticula));
    per (i = ZEPHYRUM; p->particulae != NIHIL
        && i < xar_numerus(particulae); i++)
    {
        *(FabricaParticula*)xar_addere(p->particulae) =
            *(constans FabricaParticula*)xar_obtinere(particulae, i);
    }
}

interior b32
_particulas_legere (
                vacuum*  datum,
    constans character*  titulus,
    constans character*  exitus,
               Piscina*  piscina,
                   Xar** particulae_out)
{
    DiscusFictus* discus;
             i32  i;

    (vacuum)piscina;
    discus = (DiscusFictus*)datum;
    per (i = ZEPHYRUM; i
        < xar_numerus(discus->particulae_servatae); i++)
    {
        ParticulaeFictae* q = (ParticulaeFictae*)xar_obtinere(
            discus->particulae_servatae, i);

        si (   chorda_aequalis_literis(q->titulus, titulus)
            && chorda_aequalis_literis(q->exitus, exitus))
        {
            *particulae_out = q->particulae;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* vestigium ultimum non vacuum tituli (numerus lectionum; -I nullum) */
interior s32
_vestigium_numerare (
          DiscusFictus* discus,
    constans character* titulus)
{
    i32 i;

    per (i = xar_numerus(discus->vestigia_lectionum); i > ZEPHYRUM; i--)
    {
        VestigiumLectionumFictum* v;

        v = (VestigiumLectionumFictum*)xar_obtinere(
            discus->vestigia_lectionum, i - I);
        si (chorda_aequalis_literis(v->titulus, titulus))
        {
            redde (s32)xar_numerus(v->lectiones);
        }
    }
    redde -I;
}


s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum(
        "probatio_fabrica_iudicium", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ================================================== */

    /* PROBARE: regeneratio (Review Focus 4)               */


    /* ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* a;
               FabricaExitus* exitus;
             FabricaIudicium  iudicium;

        imprimere("\n--- Probans regenerationem ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "data/fons.txt", "datum\n");
        mundi_ponere(&discus, "gen/exitus.c", "linea I\nlinea II\n");
        a = mundi_actio(piscina, "gen", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "fasciculus",
            "data/fons.txt", piscina);
        exitus = mundi_exitum_addere(a, "gen/exitus.c",
            "regeneratio", piscina);

        /* regeneratio == commissum -> RECENS */
        discus.generatio = "linea I\nlinea II\n";
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_CHORDA_AEQUALIS_LITERIS(iudicium.artificium,
            "gen/exitus.c");
        CREDO_AEQUALIS_I32(discus.cursus, I);

        /* regeneratio differt -> STALUM, lineae numeratae */
        discus.generatio = "linea I\nlinea mutata\n";
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_VERUM(mundi_continet(iudicium.causa,
            "lineae differentes: 1)",
            piscina));

        /* generator fractus -> IGNOTUM, mandatum et causa nominata */
        discus.fractus = VERUM;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa,
            "tools/generator_fictus.sh", piscina));
        CREDO_VERUM(mundi_continet(iudicium.causa, "error fictus",
            piscina));
        discus.fractus = FALSUM;

        /* generator mutus (successus sine scriptura) -> IGNOTUM, etsi
         * reliquiae cursus prioris ibi stabant (sutura eas vacuat) */
        discus.generatio = "linea I\nlinea II\n";
        (vacuum)fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        discus.generatio = NIHIL;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "nihil scripsit",
            piscina));

        /* celer: numquam regenerat */
        discus.cursus = ZEPHYRUM;
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status,
            (i32)FABRICA_NON_IUDICATUM);
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);

        /* ingressus absens -> IGNOTUM, nulla regeneratio */
        mundi_ingressum_addere(a, "fasciculus",
            "data/absens.txt", piscina);
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "data/absens.txt",
            piscina));
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);
    }


    /* ================================================== */

    /* PROBARE: relatio '-provenientia'                    */


    /* ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* a;
               FabricaExitus* exitus;
             FabricaIudicium  iudicium;
                    Sigillum  s;
                   character  hex[SIGILLUM_HEX_MENSURA];
                   character  relatio[256];

        imprimere("\n--- Probans relationem ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "lib/manus.c", "int manus;\n");
        a = mundi_actio(piscina, "manus", FABRICA_ACTIO_INSTITUTIO);
        mundi_ingressum_addere(a, "fasciculus",
            "lib/manus.c", piscina);
        exitus = mundi_exitum_addere(a, "bin/manus",
            "relatio", piscina);
        CREDO_VERUM(mundi_sigillum(&sutura, a, piscina, &s));
        sigillum_hex(&s, hex);

        sprintf(relatio, "provenientia 1\nartificium bin/manus\n"
            "ingressus %s\ncommissum abc1234\n", hex);
        discus.relatio = relatio;
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* fons mutatus post institutionem -> STALUM */
        mundi_ponere(&discus, "lib/manus.c", "int manus_nova;\n");
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);

        /* binarium sine relatione -> IGNOTUM, numquam RECENS */
        discus.relatio = NIHIL;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "sine provenientia",
            piscina));
    }


    /* ==================================================
     * PROBARE: vestigia lectionum (plan 2 T2) - actio
     * lectiones="verum" clavatur libro suo: RECENS sine regeneratione
     * si omnia aequalia; contentum (L), absentia (A), nomina (D)
     * mutata -> regeneratio; fractus -> nullum vestigium
     * ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* a;
               FabricaExitus* exitus;
             FabricaIudicium  iudicium;
                         Xar* vestigium;
          constans character* radix_nomina[I];
          constans character* radix_plus[II];

        imprimere("\n--- Probans vestigia lectionum ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        sutura.lectiones_legere    = mundi_lectiones_legere;
        sutura.lectiones_scribere  = mundi_lectiones_scribere;
        mundi_ponere(&discus, "data/scriptum.sh", "echo\n");
        mundi_ponere(&discus, "data/fons.txt", "datum\n");
        mundi_ponere(&discus, "gen/exitus.c", "linea I\n");
        mundi_ponere(&discus, "build/aedilis/obiecta/lib__x.o",
            "obiectum\n");
        radix_nomina[0] = "a.h";
        mundi_directorium_ponere(&discus, "radix", radix_nomina, I);
        a = mundi_actio(piscina, "gen",
            FABRICA_ACTIO_GENERATOR);
        a->lectiones = VERUM;
        mundi_ingressum_addere(a, "fasciculus", "data/scriptum.sh",
            piscina);
        exitus = mundi_exitum_addere(a, "gen/exitus.c", "regeneratio",
            piscina);
        discus.generatio = "linea I\n";
        /* L fons, A umbra, D radix; scriptura et S praetermittuntur */
        discus.lectiones_ficti =
            "L\tdata/fons.txt\n"
            "A\tradix/umbra.h\n"
            "D\tradix\n"
            "L\tbuild/fabrica/scriptura/gen/temporaria.txt\n"
            "L\tbuild/aedilis/obiecta/actiones/ab/cdef\n"
            "L\tbuild/aedilis/obiecta/blobi/12/3456\n"
            "L\tbuild/aedilis/obiecta/lib__x.o\n"
            "S\tgen/exitus.c\n";

        /* I. sine vestigio: regeneratio, RECENS -> vestigium scriptum,
         * quattuor lectiones: scriptura, S et THESAURUS (blobi,
         * actiones - cache per sigilla ingressuum iam lectorum) non;
         * obiectum .o in radice eadem SIC (ingressus verus nexus) */
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_AEQUALIS_I32(discus.cursus, I);
        CREDO_AEQUALIS_I32(discus.vestigia_scripta, I);
        vestigium = ((VestigiumLectionumFictum*)xar_obtinere(
            discus.vestigia_lectionum, ZEPHYRUM))->lectiones;
        CREDO_AEQUALIS_I32(xar_numerus(vestigium), IV);

        /* II. nihil mutatum: RECENS per vestigium, sine currere -
         * etiam celer */
        discus.cursus = ZEPHYRUM;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_VERUM(mundi_continet(iudicium.causa, "lectiones",
            piscina));
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* III. contentum lectum mutatum (L) -> regeneratio */
        mundi_ponere(&discus, "data/fons.txt", "datum mutatum\n");
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32(discus.cursus, I);

        /* IV. quaesitum absens nunc exstat (A, obumbratio) */
        discus.cursus = ZEPHYRUM;
        mundi_ponere(&discus, "radix/umbra.h", "/* umbra */\n");
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32(discus.cursus, I);
        mundi_auferre(&discus, "radix/umbra.h");
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);

        /* V. directorium enumeratum nomen novum habet (D) */
        discus.cursus = ZEPHYRUM;
        radix_plus[0] = "a.h";
        radix_plus[1] = "b.h";
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        mundi_directorium_ponere(&discus, "radix", radix_plus, II);
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32(discus.cursus, I);

        /* VI. ingressus declaratus mutatus: clavis alia, vestigium
         * vetus non valet -> regeneratio */
        discus.cursus = ZEPHYRUM;
        mundi_ponere(&discus, "data/scriptum.sh", "echo mutatum\n");
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32(discus.cursus, I);

        /* VII. cursus fractus: nullum vestigium novum */
        mundi_ponere(&discus, "data/fons.txt", "datum tertium\n");
        discus.fractus = VERUM;
        {
            i32 ante;

            ante = discus.vestigia_scripta;
            iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM,
                piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_IGNOTUM);
            CREDO_AEQUALIS_I32(discus.vestigia_scripta, ante);
        }
        discus.fractus = FALSUM;

        /* IX. actio exituum DUORUM: vestigium cuiusque exitus manet
         * (fragmenta_silva XXII exitus: solum ultimus servabatur) */
        {
                    FabricaActio* m;
                   FabricaExitus* e_a;
                   FabricaExitus* e_b;

            mundi_ponere(&discus, "gen/a.c", "a I\n");
            mundi_ponere(&discus, "gen/b.c", "b I\n");
            m = mundi_actio(piscina, "multi", FABRICA_ACTIO_GENERATOR);
            m->lectiones = VERUM;
            mundi_ingressum_addere(m, "fasciculus", "data/scriptum.sh",
                piscina);
            e_a = mundi_exitum_addere(m, "gen/a.c", "regeneratio",
                piscina);
            e_b = mundi_exitum_addere(m, "gen/b.c", "regeneratio",
                piscina);
            e_a->scriptura           = e_a->via;
            e_b->scriptura           = e_b->via;
            discus.scriptura_rel     = "gen/a.c";
            discus.generatio         = "a I\n";
            discus.scriptura_altera  = "gen/b.c";
            discus.generatio_altera  = "b I\n";
            discus.lectiones_ficti   = "L\tdata/fons.txt\n";
            (vacuum)fabrica_iudicare(&sutura, m, e_a, VERUM, piscina);
            (vacuum)fabrica_iudicare(&sutura, m, e_b, VERUM, piscina);
            discus.cursus = ZEPHYRUM;
            iudicium = fabrica_iudicare(&sutura, m, e_a, VERUM,
                piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_RECENS);
            iudicium = fabrica_iudicare(&sutura, m, e_b, VERUM,
                piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_RECENS);
            CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);
            discus.scriptura_rel     = "gen/exitus.c";
            discus.generatio         = "linea I\n";
            discus.scriptura_altera  = NIHIL;
            discus.generatio_altera  = NIHIL;
        }

        /* X. AUDITUS MEMORIAE (Review Focus 5): vestigium cui lectio
         * UNA deest (ingressus quem liber non vidit) + plagula illa
         * mutata -> iudex ordinarius RECENS dicit (foramen); auditus
         * regenerat et AUDITUM DISCORS nominat */
        {
                         FabricaActio* g;
                        FabricaExitus* e_g;
             VestigiumLectionumFictum* v;
                                  i32  k;

            mundi_ponere(&discus, "data/occultum.txt", "occultum I\n");
            mundi_ponere(&discus, "gen/occ.c",
                "praefixum: occultum I\n");
            mundi_scriptum_addere(&discus, "gen_occ", "gen/occ.c",
                "data/occultum.txt", "praefixum: ", 0, FALSUM);
            g = mundi_actio_scripta(piscina, "occ", "gen_occ",
                "data/scriptum.sh", "gen/occ.c", "regeneratio");
            g->lectiones = VERUM;
            e_g = (FabricaExitus*)xar_obtinere(g->exitus, ZEPHYRUM);
            discus.lectiones_ficti =
                "L\tdata/occultum.txt\nL\tdata/fons.txt\n";
            iudicium = fabrica_iudicare(&sutura, g, e_g, VERUM,
                piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_RECENS);
            /* lectionem 'occultum' ex vestigio manu delere */
            per (k = ZEPHYRUM; k < xar_numerus(
                     discus.vestigia_lectionum); k++)
            {
                v = (VestigiumLectionumFictum*)xar_obtinere(
                    discus.vestigia_lectionum, k);
                si (chorda_aequalis_literis(v->titulus, "occ"))
                {
                    Xar* reliquae;
                    i32  j;

                    reliquae = xar_creare(piscina,
                        (i32)magnitudo(FabricaLectio));
                    per (j = ZEPHYRUM; j < xar_numerus(v->lectiones);
                         j++)
                    {
                        FabricaLectio* l;

                        l = (FabricaLectio*)xar_obtinere(v->lectiones,
                            j);
                        si (!chorda_aequalis_literis(l->via,
                                "data/occultum.txt"))
                        {
                            *(FabricaLectio*)xar_addere(reliquae) = *l;
                        }
                    }
                    v->lectiones = reliquae;
                }
            }
            mundi_ponere(&discus, "data/occultum.txt",
                "occultum MUTATUM\n");
            /* foramen: iudex ordinarius vestigio credit */
            iudicium = fabrica_iudicare(&sutura, g, e_g, VERUM,
                piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_RECENS);
            /* auditus omnium: regeneratio differt -> nominatur */
            sutura.auditus = I;
            iudicium = fabrica_iudicare(&sutura, g, e_g, VERUM,
                piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_STALUM);
            CREDO_VERUM(mundi_continet(iudicium.causa,
                "AUDITUM DISCORS",
                piscina));
            sutura.auditus = ZEPHYRUM;
        }

        /* VIII. actio sine lectiones: liber numquam petitur nec
         * vestigium scribitur */
        {
            i32 ante;

            a->lectiones   = FALSUM;
            ante           = discus.vestigia_scripta;
            discus.cursus  = ZEPHYRUM;
            iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM,
                piscina);
            CREDO_AEQUALIS_I32(discus.cursus, I);
            CREDO_AEQUALIS_I32(discus.vestigia_scripta, ante);
        }
    }


    /* ==================================================
     * PROBARE: memoria verificationum (T6, Review Focus 1)
     * ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* a;
               FabricaExitus* exitus;
             FabricaIudicium  iudicium;

        imprimere("\n--- Probans memoriam verificationum ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        sutura.meminisse   = _meminisse;
        sutura.inscribere  = _inscribere;
        mundi_ponere(&discus, "data/fons.txt", "datum\n");
        mundi_ponere(&discus, "gen/exitus.c", "linea I\n");
        a = mundi_actio(piscina, "gen", FABRICA_ACTIO_GENERATOR);
        a->memorabilis = VERUM;
        mundi_ingressum_addere(a, "fasciculus",
            "data/fons.txt", piscina);
        exitus = mundi_exitum_addere(a, "gen/exitus.c",
            "regeneratio", piscina);
        discus.generatio = "linea I\n";

        /* sine verificatione: regeneratio, RECENS -> inscribitur */
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_AEQUALIS_I32(discus.cursus, I);
        CREDO_AEQUALIS_I32(discus.inscriptiones, I);

        /* verificatio congruit -> RECENS sine currere, etiam plenus */
        discus.cursus = ZEPHYRUM;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_VERUM(mundi_continet(iudicium.causa, "memoria", piscina));
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);
        /* hit non iterum inscribitur */
        CREDO_AEQUALIS_I32(discus.inscriptiones, I);

        /* et celer: verificatio iudicat ubi regeneratio omittitur */
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* artificium MANU mutatum, ingressus idem -> memoria non
         * congruit: regeneratio, STALUM (Review Focus 1) */
        mundi_ponere(&discus, "gen/exitus.c", "linea I manu mutata\n");
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_AEQUALIS_I32(discus.cursus, I);
        CREDO_AEQUALIS_I32(discus.inscriptiones, I);

        /* actio NON memorabilis: verificatio numquam consulitur nec
         * scribitur (ingressus non provabiliter pleni) */
        mundi_ponere(&discus, "gen/exitus.c", "linea I\n");
        a->memorabilis = FALSUM;
        discus.cursus = ZEPHYRUM;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_AEQUALIS_I32(discus.cursus, I);
        CREDO_AEQUALIS_I32(discus.inscriptiones, I);
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status,
            (i32)FABRICA_NON_IUDICATUM);
    }


    /* ==================================================
     * PROBARE: mandatum in clave memoriae (T6)
     * ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* a;
               FabricaExitus* exitus;
             FabricaIudicium  iudicium;
                      chorda* verbum;

        imprimere("\n--- Probans mandatum in memoria ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        sutura.meminisse   = _meminisse;
        sutura.inscribere  = _inscribere;
        mundi_ponere(&discus, "data/fons.txt", "datum\n");
        mundi_ponere(&discus, "gen/exitus.c", "linea I\n");
        a = mundi_actio(piscina, "gen", FABRICA_ACTIO_GENERATOR);
        a->memorabilis = VERUM;
        mundi_ingressum_addere(a, "fasciculus",
            "data/fons.txt", piscina);
        exitus = mundi_exitum_addere(a, "gen/exitus.c",
            "regeneratio", piscina);
        discus.generatio = "linea I\n";
        (vacuum)fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32(discus.inscriptiones, I);

        /* radix nova in mandato (declaratio mutata), ingressus idem:
         * verificatio vetus NON congruit - regeneratio */
        verbum = (chorda*)xar_addere(a->mandatum);
        *verbum = chorda_ex_literis("tools/radix_nova.c", piscina);
        discus.cursus = ZEPHYRUM;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_AEQUALIS_I32(discus.cursus, I);
    }


    /* ==================================================
     * PROBARE: regeneratio semel per actionem et cursum (T6)
     * ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* a;
               FabricaExitus* primus;
               FabricaExitus* secundus;
             FabricaIudicium  iudicium;

        imprimere("\n--- Probans regenerationem per actionem ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        sutura.regenerationes = tabula_dispersa_creare_chorda(piscina,
            16);
        mundi_ponere(&discus, "data/fons.txt", "datum\n");
        mundi_ponere(&discus, "gen/a.c", "idem\n");
        mundi_ponere(&discus, "gen/b.c", "idem\n");
        a = mundi_actio(piscina, "multi", FABRICA_ACTIO_GENERATOR);
        mundi_ingressum_addere(a, "fasciculus",
            "data/fons.txt", piscina);
        primus = mundi_exitum_addere(a, "gen/a.c",
            "regeneratio", piscina);
        secundus = mundi_exitum_addere(a, "gen/b.c",
            "regeneratio", piscina);
        discus.generatio = "idem\n";

        /* generator unus, exitus duo: cursus UNUS (silva: XXII
         * fragmenta ex generatore uno ~CCXXXVIII s) */
        iudicium = fabrica_iudicare(&sutura, a, primus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        iudicium = fabrica_iudicare(&sutura, a, secundus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_AEQUALIS_I32(discus.cursus, I);

        /* sigillum actionis semel per cursum (celer: XXII exitus
         * silvae ingressus eosdem vicies bis explicabant, IX s) -
         * exitus secundus solum artificium suum legit */
        {
            i32 lecturae;

            sutura.digesta = tabula_dispersa_creare_chorda(piscina,
                16);
            discus.lecturae = ZEPHYRUM;
            (vacuum)fabrica_iudicare(&sutura, a, primus, FALSUM,
                piscina);
            lecturae = discus.lecturae;
            (vacuum)fabrica_iudicare(&sutura, a, secundus, FALSUM,
                piscina);
            CREDO_AEQUALIS_I32(discus.lecturae, lecturae + I);
        }

        /* cursus fractus quoque memoratur: exitus secundus IGNOTUM
         * eadem causa, sine cursu altero */
        sutura.regenerationes = tabula_dispersa_creare_chorda(piscina,
            16);
        discus.cursus = ZEPHYRUM;
        discus.fractus = VERUM;
        iudicium = fabrica_iudicare(&sutura, a, primus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        iudicium = fabrica_iudicare(&sutura, a, secundus, VERUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "error fictus",
            piscina));
        CREDO_AEQUALIS_I32(discus.cursus, I);
    }


    /* ==================================================
     * PROBARE: PRAEVISIO regenerationum (T6b): iudicium simul
     * ================================================== */

    {
          DiscusFictus  discus;
         FabricaSutura  sutura;
          FabricaActio* actiones[III];
                   Xar* ordo;
       FabricaIudicium  iudicium;
                   i32  i;
                   i32  stala;

        imprimere("\n--- Probans praevisionem regenerationum ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.currere_simul = mundi_currere_simul;
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "a2", "a2\n");
        mundi_ponere(&discus, "an", "an\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_ponere(&discus, "X2", "vetus\n");
        mundi_ponere(&discus, "XN", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_2", "X2", NIHIL, "n2\n", 0,
            FALSUM);
        mundi_scriptum_addere(&discus, "gen_n", "XN", NIHIL, "nn\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "L2", "gen_2", "a2",
            "X2",
            "regeneratio");
        actiones[1] = mundi_actio_scripta(piscina, "N", "gen_n", "an",
            "XN",
            "regeneratio");
        actiones[2] = mundi_actio_scripta(piscina, "L1", "gen_1", "a1",
            "X1",
            "regeneratio");
        actiones[0]->lectiones = VERUM;
        actiones[2]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, III);

        /* I. praevisio: tutae (lectiones) simul, ordine tituli; N non */
        fabrica_regenerationes_praevidere(&sutura, ordo, VERUM,
            piscina);
        CREDO_AEQUALIS_I32(xar_numerus(discus.undae_currendi), I);
        si (xar_numerus(discus.undae_currendi) == I)
        {
            CREDO_VERUM(chorda_aequalis_literis(*(chorda*)xar_obtinere(
                discus.undae_currendi, ZEPHYRUM), "L1 L2"));
        }
        CREDO_AEQUALIS_I32(discus.cursus, II);

        /* II. iudicium sequens memoriam invenit: N SOLA regeneratur,
         * verdicta ut sine praevisione (omnia STALUM, octeti differunt) */
        stala = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
        {
            FabricaActio* actio;

            actio = *(FabricaActio**)xar_obtinere(ordo, i);
            iudicium = fabrica_iudicare(&sutura, actio,
                (FabricaExitus*)xar_obtinere(actio->exitus, ZEPHYRUM),
                VERUM, piscina);
            si (iudicium.status == FABRICA_STALUM)
            {
                stala++;
            }
            CREDO_FALSUM(mundi_continet(iudicium.causa, "praevisio",
                piscina));
        }
        CREDO_AEQUALIS_I32(stala, III);
        CREDO_AEQUALIS_I32(discus.cursus, III);

        /* III. sine currere_simul: praevisio nihil agit (via vetus) */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a1", "a1\n");
        mundi_ponere(&discus, "X1", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_1", "X1", NIHIL, "n1\n", 0,
            FALSUM);
        actiones[0] = mundi_actio_scripta(piscina, "L1", "gen_1", "a1",
            "X1",
            "regeneratio");
        actiones[0]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones, I);
        fabrica_regenerationes_praevidere(&sutura, ordo, VERUM,
            piscina);
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);
        CREDO_AEQUALIS_I32(xar_numerus(discus.undae_currendi),
            ZEPHYRUM);
    }


    /* ==================================================
     * PROBARE: celer="verum" - regeneratio vilis sub iudicio celeri
     * (plan 1b T5)
     * ================================================== */

    {
          DiscusFictus  discus;
         FabricaSutura  sutura;
          FabricaActio* a;
       FabricaIudicium  iudicium;

        imprimere("\n--- Probans celer ---\n");
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        a = mundi_actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        /* sine celer: iudicium celere regenerationem omittit */
        iudicium = fabrica_iudicare(&sutura, a,
            (FabricaExitus*)xar_obtinere(a->exitus, ZEPHYRUM), FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status,
            (i32)FABRICA_NON_IUDICATUM);
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);
        /* celer: regeneratio etiam sub celeri -> STALUM verum */
        a->celer = VERUM;
        iudicium = fabrica_iudicare(&sutura, a,
            (FabricaExitus*)xar_obtinere(a->exitus, ZEPHYRUM), FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_AEQUALIS_I32(discus.cursus, I);
    }


    /* ==================================================
     * PROBARE: iudicium (fabrica spec 3 T5a) - porta ut actio:
     * declaratio iudicium sine lectiones et verdictum extra iudicium
     * recusat; strategia verdictum NUMQUAM currit (verdictum absens
     * STALUM, sine vestigio IGNOTUM); sanare sine argumentis iudicium
     * omittit, nominatum agit
     * ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* p;
               FabricaExitus* exitus;
             FabricaIudicium  iudicium;
                      chorda  contentum;
                      chorda  causa;
                         Xar* lectae;
                FabricaActio* actiones_iudicii[II];
                         Xar* ordo;
                         Xar* sanationes;
                         Xar* electa;
         InternamentumChorda* intern_iudicii;

        imprimere("\n--- Probans iudicium (porta ut actio) ---\n");
        causa.datum     = NIHIL;
        causa.mensura   = ZEPHYRUM;
        intern_iudicii  = internamentum_creare(piscina);

        /* I. declaratio bona: genus iudicium */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"porta_x\" genus=\"iudicium\""
            " lectiones=\"verum\">\n"
            "    <mandatum>\n"
            "      <verbum! (>./porta.sh\n"
            "    </mandatum>\n"
            "    <ingressus genus=\"fasciculus\" via=\"porta.sh\"/>\n"
            "    <exitus via=\"build/fabrica/verdicta/x.txt\""
            " provenientia=\"verdictum\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        lectae = fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern_iudicii, &causa);
        CREDO_NON_NIHIL(lectae);
        si (lectae != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)((FabricaActio*)xar_obtinere(lectae,
                ZEPHYRUM))->genus, (i32)FABRICA_ACTIO_IUDICIUM);
        }

        /* II. recusationes: sine lectiones; verdictum in generatore;
         * iudicium cum regeneratione */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"porta_x\" genus=\"iudicium\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"porta.sh\"/>\n"
            "    <exitus via=\"v.txt\" provenientia=\"verdictum\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern_iudicii, &causa));
        CREDO_VERUM(mundi_continet(causa, "iudicium sine lectiones",
            piscina));
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"porta.sh\"/>\n"
            "    <exitus via=\"v.txt\" provenientia=\"verdictum\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern_iudicii, &causa));
        CREDO_VERUM(mundi_continet(causa,
            "verdictum extra actionem iudicium",
            piscina));
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"porta_x\" genus=\"iudicium\""
            " lectiones=\"verum\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"porta.sh\"/>\n"
            "    <exitus via=\"v.txt\" provenientia=\"regeneratio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern_iudicii, &causa));
        CREDO_VERUM(mundi_continet(causa,
            "iudicium sine exitu verdicti",
            piscina));

        /* III. strategia verdictum: numquam currit */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.lectiones_legere    = mundi_lectiones_legere;
        sutura.lectiones_scribere  = mundi_lectiones_scribere;
        mundi_ponere(&discus, "porta.sh", "echo porta\n");
        p = mundi_actio(piscina, "porta_x",
            FABRICA_ACTIO_IUDICIUM);
        p->lectiones = VERUM;
        mundi_ingressum_addere(p, "fasciculus", "porta.sh", piscina);
        exitus = mundi_exitum_addere(p, "build/fabrica/verdicta/x.txt",
            "verdictum", piscina);
        iudicium = fabrica_iudicare(&sutura, p, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "verdictum absens",
            piscina));
        mundi_ponere(&discus, "build/fabrica/verdicta/x.txt",
            "x: transiit\n");
        iudicium = fabrica_iudicare(&sutura, p, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "nullum vestigium",
            piscina));
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);  /* numquam currit */

        /* IV. sanare sine argumentis iudicium omittit; nominatum agit */
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        mundi_ponere(&discus, "a", "a\n");
        mundi_ponere(&discus, "X", "vetus\n");
        mundi_ponere(&discus, "porta.sh", "echo porta\n");
        mundi_scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n",
            0,
            FALSUM);
        mundi_scriptum_addere(&discus, "porta_sh",
            "build/fabrica/verdicta/x.txt",
            NIHIL, "x: transiit\n", 0, FALSUM);
        actiones_iudicii[0] = mundi_actio_scripta(piscina, "A", "gen_a",
            "a",
            "X", "regeneratio");
        actiones_iudicii[1] = mundi_actio_scripta(piscina, "porta_x",
            "porta_sh", "porta.sh", "build/fabrica/verdicta/x.txt",
            "verdictum");
        actiones_iudicii[1]->genus = FABRICA_ACTIO_IUDICIUM;
        actiones_iudicii[1]->lectiones = VERUM;
        ordo = mundi_ordinare_fictas(piscina, actiones_iudicii, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_NIHIL(mundi_sanatio_invenire(sanationes, "porta_x"));
        CREDO_NON_NIHIL(mundi_sanatio_invenire(sanationes, "A"));
        CREDO_AEQUALIS_I32(discus.acta, I);
        electa = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis(
            "build/fabrica/verdicta/x.txt", piscina);
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_NON_NIHIL(mundi_sanatio_invenire(sanationes, "porta_x"));
        CREDO_AEQUALIS_I32(discus.acta, II);
    }


    /* ==================================================
     * PROBARE: transitus (fabrica spec 3 T5b) - sanare iudicium in
     * loco vestigium libri sui servat; leges: S = exitus (omittitur),
     * build/ cum domino declarato, radices systematis omissae, viae
     * absolutae ceterae sigillatae, E externa clavata / interna
     * omissa; iudicium post: RECENS sine cursu; lectio, ambitus,
     * fontatio, identitas mutata -> STALUM aut IGNOTUM; build/ sine
     * domino et FIFO -> transitus non servatus, vestigium vetus deletum
     * ================================================== */

    {
                DiscusFictus  discus;
               FabricaSutura  sutura;
                FabricaActio* actiones_transitus[I];
               FabricaExitus* exitus;
             FabricaIudicium  iudicium;
              FabricaSanatio* sanatio;
                         Xar* ordo;
                         Xar* sanationes;
                         Xar* electa;
                      chorda  causa;
         constans character* VERDICTUM = "build/fabrica/verdicta/x.txt";
         constans character* LIBER =
             "build/fabrica/lectiones/porta_x.tsv";
         constans character* LIBER_BONUS =
             "L\tdata/fons.txt\n"
             "L\tbuild/corpus.lst\n"
             "L\tx/build/obj.o\n"
             "S\tx/build/obj.o\n"
             "L\t/usr/include/stdio.h\n"
             "L\t/home/u/silva.toml\n"
             "E\tHOME\t/home/u\n"
             "E\tRADIX_FICTA\t/repo\n"
             "E\tOMNIA\n";

        imprimere("\n--- Probans transitum (vestigium portae) ---\n");
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        mundi_discum_parare(&discus, &sutura, piscina);
        /* sine memoriis per cursum: iudicia hic SEQUUNTUR mutationes
         * ingressuum in eodem processu (instrumentum verum processum
         * novum per iudicium habet) */
        sutura.lectiones_legere = mundi_lectiones_legere;
        sutura.lectiones_scribere = mundi_lectiones_scribere;
        sutura.particulas_scribere = _particulas_scribere;
        sutura.particulas_legere = _particulas_legere;
        sutura.ambitus = mundi_ambitus_fictum;
        sutura.species = mundi_species_ficta;
        sutura.identitas = mundi_identitatem_fictam_dare;
        sutura.effectus = mundi_effectus_ficti;
        sutura.exitus_noti = tabula_dispersa_creare_chorda(piscina, 16);
        (vacuum)tabula_dispersa_inserere(sutura.exitus_noti,
            chorda_ex_literis("build/corpus.lst", piscina), NIHIL);
        mundi_ambitus_ficti[0] = "HOME";
        mundi_ambitus_ficti[1] = "/home/u";
        mundi_ambitus_ficti[2] = NIHIL;
        mundi_ponere(&discus, "porta.sh", "echo porta\n");
        mundi_ponere(&discus, "data/fons.txt", "fons I\n");
        mundi_ponere(&discus, "build/corpus.lst", "a.toml\n");
        mundi_ponere(&discus, "build/sine_domino.txt", "x\n");
        mundi_ponere(&discus, "/home/u/silva.toml", "x = 1\n");
        mundi_ponere(&discus, "tools/v.sh", "v I\n");
        mundi_effectus_effusio = "octeti\ttools/v.sh\n";
        mundi_scriptum_addere(&discus, "porta_sh", VERDICTUM, NIHIL,
            "x: transiit\n", 0, FALSUM);
        actiones_transitus[0] = mundi_actio_scripta(piscina, "porta_x",
            "porta_sh", "porta.sh", VERDICTUM, "verdictum");
        actiones_transitus[0]->genus      = FABRICA_ACTIO_IUDICIUM;
        actiones_transitus[0]->lectiones  = VERUM;
        mundi_ingressum_addere(actiones_transitus[0], "effectus",
            "porta.sh",
            piscina);
        mundi_ingressum_addere(actiones_transitus[0], "identitas_clang",
            "clang",
            piscina);
        exitus =
            (FabricaExitus*)xar_obtinere(actiones_transitus[0]->exitus,
            ZEPHYRUM);
        ordo    = mundi_ordinare_fictas(piscina, actiones_transitus, I);
        electa  = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis(VERDICTUM,
            piscina);
        mundi_ponere(&discus, LIBER, LIBER_BONUS);

        /* I. transitus servatus: V lectiones (S, /usr, E interna omissae) */
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_x");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_SANATUM);
            CREDO_AEQUALIS_I32((i32)sanatio->causa.mensura, ZEPHYRUM);
        }
        CREDO_AEQUALIS_I32((i32)_vestigium_numerare(&discus, "porta_x"),
            V);
        discus.acta = ZEPHYRUM;
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);
        CREDO_AEQUALIS_I32(discus.acta, ZEPHYRUM);
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);

        /* II. lectio mutata -> STALUM nominata */
        mundi_ponere(&discus, "data/fons.txt", "fons II\n");
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "data/fons.txt",
            piscina));
        mundi_ponere(&discus, "data/fons.txt", "fons I\n");
        /* via absoluta extra systema sigillata */
        mundi_ponere(&discus, "/home/u/silva.toml", "x = 2\n");
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        mundi_ponere(&discus, "/home/u/silva.toml", "x = 1\n");

        /* III. ambitus externus: OMNIA absens clavatum; positum -> STALUM */
        mundi_ambitus_ficti[2] = "OMNIA";
        mundi_ambitus_ficti[3] = "1";
        mundi_ambitus_ficti[4] = NIHIL;
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "OMNIA", piscina));
        mundi_ambitus_ficti[2] = NIHIL;
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* IV. fontatio (scriptum fontatum) mutata: clavis alia -> IGNOTUM */
        mundi_ponere(&discus, "tools/v.sh", "v II\n");
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        /* QUIS mutatus (plan-5 T1): ingressus nominatur */
        CREDO_VERUM(mundi_continet(iudicium.causa, "tools/v.sh",
            piscina));
        mundi_ponere(&discus, "tools/v.sh", "v I\n");
        /* V. identitas clang mutata: clavis alia */
        mundi_identitas_ficta = "clang II";
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "clang", piscina));
        mundi_identitas_ficta = "clang I";
        /* VI. situs irresolutus (effectus) -> IGNOTUM nominatum */
        mundi_effectus_effusio =
            "ignotum\tporta.sh:4:1-4:5\tvalor ignotus\n";
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(mundi_continet(iudicium.causa, "effectus ignotus",
            piscina));
        mundi_effectus_effusio = "octeti\ttools/v.sh\n";
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* VII. build/ sine domino: transitus non servatus, vetus deletum */
        mundi_ponere(&discus, LIBER,
            "L\tdata/fons.txt\nL\tbuild/sine_domino.txt\n");
        mundi_auferre(&discus, VERDICTUM);
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_x");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_SANATUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "sine domino",
                piscina));
        }
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);

        /* VIIb (plan-5 T5b). lectio in vestigio PROPRIO actionis: opus
         * suum, non ingressus - filii (sqlite, redirectio) S non notant,
         * ergo 'sine domino' aliter. Vestigium COMMUNE non excusat. */
        {
            FabricaLocus* locus;

            si (actiones_transitus[0]->vestigia == NIHIL)
            {
                actiones_transitus[0]->vestigia = xar_creare(piscina,
                    (i32)magnitudo(FabricaLocus));
            }
            locus = (FabricaLocus*)xar_addere(
                actiones_transitus[0]->vestigia);
            CREDO_NON_NIHIL(locus);
            si (locus != NIHIL)
            {
                locus->forma    = FABRICA_LOCUS_ARBOR;
                locus->via      = chorda_ex_literis("build/proprium",
                    piscina);
                locus->suffixa  = chorda_ex_literis("", piscina);
            }
            mundi_ponere(&discus, LIBER,
                "L\tdata/fons.txt\nL\tbuild/proprium/portus.txt\n");
            mundi_auferre(&discus, VERDICTUM);
            sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
                piscina, &causa);
            sanatio = mundi_sanatio_invenire(sanationes, "porta_x");
            CREDO_NON_NIHIL(sanatio);
            si (sanatio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                    (i32)FABRICA_SANATUM);
                CREDO_AEQUALIS_I32((i32)sanatio->causa.mensura,
                    ZEPHYRUM);
            }
            iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
                exitus,
                VERUM, piscina);
            CREDO_AEQUALIS_I32((i32)iudicium.status,
                (i32)FABRICA_RECENS);
        }

        /* VIIc (plan-5 T5b). radices TEMPORARIAE (/tmp, /private/tmp,
         * /var/folders): status ambientis, non ingressus sigillabilis
         * (ut effectus classis temporaria) - /tmp ipsum quoque */
        mundi_ponere(&discus, "/tmp/probatio_x/a.txt", "a I\n");
        mundi_ponere(&discus, LIBER,
            "L\tdata/fons.txt\nD\t/tmp\nL\t/tmp/probatio_x/a.txt\n"
            "X\t/private/var/folders/ab/T/f\n");
        mundi_auferre(&discus, VERDICTUM);
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_x");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->causa.mensura, ZEPHYRUM);
        }
        mundi_ponere(&discus, "/tmp/probatio_x/a.txt", "a II\n");
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* VIII. FIFO (species ALIA): non sigillabilis */
        mundi_via_alia_ficta = "data/fifo";
        mundi_ponere(&discus, LIBER,
            "L\tdata/fons.txt\nL\tdata/fifo\n");
        mundi_auferre(&discus, VERDICTUM);
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_x");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_VERUM(mundi_continet(sanatio->causa,
                "non sigillabilis",
                piscina));
        }
        mundi_via_alia_ficta = NIHIL;

        /* IX. liber bonus iterum: servatus, RECENS */
        mundi_ponere(&discus, LIBER, LIBER_BONUS);
        mundi_auferre(&discus, VERDICTUM);
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        iudicium = fabrica_iudicare(&sutura, actiones_transitus[0],
            exitus,
            VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* X. AUDITUS (spec 3 XIII): RECENS tamen currit; transit */
        sutura.auditus  = I;
        sutura.audita   = tabula_dispersa_creare_chorda(piscina, 8);
        discus.acta     = ZEPHYRUM;
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_x");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_SANATUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "auditus",
                piscina));
        }
        CREDO_AEQUALIS_I32(discus.acta, I);

        /* XI. AUDITUM DISCORS: porta nunc fracta, liber lectionem novam
         * fert quam vestigium servatum non habet - nominatur */
        {
            i32 k;

            per (k = ZEPHYRUM; k < xar_numerus(discus.scripta); k++)
            {
                ScriptumFictum* sf = (ScriptumFictum*)xar_obtinere(
                    discus.scripta, k);

                si (strcmp(sf->verbum, "porta_sh") == ZEPHYRUM)
                {
                    sf->codex = I;
                }
            }
        }
        mundi_ponere(&discus, "data/novum.txt", "novum\n");
        mundi_ponere(&discus, LIBER,
            "L\tdata/fons.txt\nL\tdata/novum.txt\n");
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_x");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_AUDITUM_DISCORS);
            CREDO_VERUM(mundi_continet(sanatio->causa, "data/novum.txt",
                piscina));
        }
        sutura.auditus          = ZEPHYRUM;
        sutura.audita           = NIHIL;
        mundi_ambitus_ficti[0]  = NIHIL;
        mundi_effectus_effusio  = "";   /* ordinarium sequentibus */
    }


    /* ==================================================
     * PROBARE: SIGNUM (fabrica plan 5 T3) - bin/fabrica cursorem portae
     * IPSE currit et verdictum IPSE scribit ('<nomen>: <signum>
     * <verbum>'): signum absens aut FRACT in compendio = FRACTUM;
     * verdictum vetus
     * ANTE cursum deletum - silva.py extra omnem clavem
     * ================================================== */

    {
         DiscusFictus  discus;
        FabricaSutura  sutura;
         FabricaActio* actiones_signi[I];
       ScriptumFictum* scriptum;
                  Xar* ordo;
                  Xar* electa;
                  Xar* sanationes;
       FabricaSanatio* sanatio;
               chorda  causa;

        imprimere("\n--- Probans signum (verdictum in C) ---\n");
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        mundi_discum_parare(&discus, &sutura, piscina);
        mundi_memorias_parare(&sutura, piscina);
        sutura.verdictum_ponere = mundi_verdictum_ponere;
        mundi_ponere(&discus, "y.sh", "echo y\n");
        /* mutus: cursor ipse verdictum NON scribit */
        mundi_scriptum_addere(&discus, "run_y", "verdicta/y.txt", NIHIL,
            "ignoratum\n", 0, VERUM);
        scriptum = (ScriptumFictum*)xar_obtinere(discus.scripta,
            xar_numerus(discus.scripta) - I);
        scriptum->effusio = "initium\nY PROBATIONES: 3/3 praeteritae\n";
        actiones_signi[0] = mundi_actio_scripta(piscina, "porta_y",
            "run_y",
            "y.sh", "verdicta/y.txt", "verdictum");
        actiones_signi[0]->genus   = FABRICA_ACTIO_IUDICIUM;
        /* signum sine spatio finali: spatia sequentia transiliuntur */
        actiones_signi[0]->signum  = chorda_ex_literis("Y PROBATIONES:",
            piscina);
        ordo    = mundi_ordinare_fictas(piscina, actiones_signi, I);
        electa  = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) =
            chorda_ex_literis("verdicta/y.txt",
            piscina);

        /* I. transitus: verdictum a fabrica scriptum */
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_y");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_SANATUM);
        }
        CREDO_VERUM(mundi_contentum_est(&discus, "verdicta/y.txt",
            "y: Y PROBATIONES: 3/3\n"));

        /* II. signum absens: FRACTUM, verdictum vetus deletum */
        scriptum->effusio = "nihil hic\n";
        mundi_ponere(&discus, "y.sh", "echo y II\n");
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_y");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
            CREDO_VERUM(mundi_continet(sanatio->causa, "signum absens",
                piscina));
        }
        CREDO_NIHIL(mundi_fasciculum_invenire(&discus,
            "verdicta/y.txt"));

        /* III. FRACT in compendio: FRACTUM */
        scriptum->effusio = "Y PROBATIONES: FRACTA\n";
        mundi_ponere(&discus, "y.sh", "echo y III\n");
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        sanatio = mundi_sanatio_invenire(sanationes, "porta_y");
        CREDO_NON_NIHIL(sanatio);
        si (sanatio != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)sanatio->eventus,
                (i32)FABRICA_FRACTUM);
        }
        CREDO_NIHIL(mundi_fasciculum_invenire(&discus,
            "verdicta/y.txt"));
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
