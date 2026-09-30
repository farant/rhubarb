/* probatio_fabrica.c - Probationes nuclei fabricae (iudex, T1)
 *
 * Machina super suturam in memoria agitur: tabula plagularum,
 * tabula directoriorum, generator scriptus (scribit, frangitur, aut
 * mutus manet - directorium scripturae ante cursum VACUAT, ut sutura
 * vera debet), relatio '-provenientia' scripta. Nihil in disco nisi
 * fixurae manifestorum (probationes/fixa/fabrica/).
 *
 * Spec: project-specs/fabrica-spec-v2.md; planum fabrica-plan-1a.md
 * T1 (sectiones et Review Focus 2, 4).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "sigillum.h"
#include "fabrica.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Discus in memoria (sutura ficta)
 * ================================================== */

nomen structura {
    chorda via;
    chorda contentum;
       b32 deletum;
} FasciculusFictus;

nomen structura {
    chorda  via;
       Xar* nomina;   /* chorda */
} DirectoriumFictum;

nomen structura {
               Piscina* piscina;
                   Xar* fasciculi;     /* FasciculusFictus */
                   Xar* directoria;    /* DirectoriumFictum */
    constans character* scriptura_rel; /* ubi generator scribit */
    constans character* generatio;     /* NIHIL = generator mutus */
                   b32  fractus;
                   i32  cursus;
    constans character* relatio;       /* NIHIL = sine provenientia */
} DiscusFictus;

interior FasciculusFictus*
_fasciculum_invenire (
                DiscusFictus* discus,
          constans character* via)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(discus->fasciculi);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        FasciculusFictus* f;

        f = (FasciculusFictus*)xar_obtinere(discus->fasciculi, i);
        si (!f->deletum && chorda_aequalis_literis(f->via, via))
        {
            redde f;
        }
    }
    redde NIHIL;
}

interior vacuum
_ponere (
                DiscusFictus* discus,
          constans character* via,
          constans character* contentum)
{
    FasciculusFictus* f;

    f = _fasciculum_invenire(discus, via);
    si (f == NIHIL)
    {
        f = (FasciculusFictus*)xar_addere(discus->fasciculi);
        si (f == NIHIL)
        {
            redde;
        }
        f->via      = chorda_ex_literis(via, discus->piscina);
        f->deletum  = FALSUM;
    }
    f->contentum = chorda_ex_literis(contentum, discus->piscina);
}

interior vacuum
_directorium_ponere (
                DiscusFictus* discus,
          constans character* via,
          constans character* nomina[],
                         i32  numerus)
{
    DirectoriumFictum* d;
                  i32  i;

    d = (DirectoriumFictum*)xar_addere(discus->directoria);
    si (d == NIHIL)
    {
        redde;
    }
    d->via     = chorda_ex_literis(via, discus->piscina);
    d->nomina  = xar_creare(discus->piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda* locus;

        locus = (chorda*)xar_addere(d->nomina);
        si (locus != NIHIL)
        {
            *locus = chorda_ex_literis(nomina[i], discus->piscina);
        }
    }
}

interior b32
_legere (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* contentum_out)
{
    FasciculusFictus* f;

    (vacuum)piscina;
    f = _fasciculum_invenire((DiscusFictus*)datum, via);
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    *contentum_out = f->contentum;
    redde VERUM;
}

interior b32
_enumerare (
                vacuum*  datum,
    constans character*  via,
               Piscina*  piscina,
                   Xar** nomina_out)
{
    DiscusFictus* discus;
             i32  i;
             i32  numerus;

    (vacuum)piscina;
    discus   = (DiscusFictus*)datum;
    numerus  = xar_numerus(discus->directoria);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        DirectoriumFictum* d;

        d = (DirectoriumFictum*)xar_obtinere(discus->directoria, i);
        si (chorda_aequalis_literis(d->via, via))
        {
            *nomina_out = d->nomina;
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_currere (
                vacuum* datum,
          constans Xar* mandatum,
    constans character* scriptura_dir,
               Piscina* piscina,
                chorda* causa_out)
{
    DiscusFictus* discus;
          chorda  praefixum;
             i32  i;
             i32  numerus;

    (vacuum)mandatum;
    discus = (DiscusFictus*)datum;
    discus->cursus++;
    /* contractus suturae: directorium scripturae VACUUM */
    praefixum  = chorda_ex_literis(scriptura_dir, piscina);
    numerus    = xar_numerus(discus->fasciculi);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        FasciculusFictus* f;

        f = (FasciculusFictus*)xar_obtinere(discus->fasciculi, i);
        si (chorda_incipit(f->via, praefixum))
        {
            f->deletum = VERUM;
        }
    }
    si (discus->fractus)
    {
        *causa_out = chorda_ex_literis("exitus I: error fictus",
            piscina);
        redde FALSUM;
    }
    si (discus->generatio != NIHIL)
    {
        character via[256];

        sprintf(via, "%s/%s", scriptura_dir, discus->scriptura_rel);
        _ponere(discus, via, discus->generatio);
    }
    redde VERUM;
}

interior b32
_rogare (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* relatio_out)
{
    DiscusFictus* discus;

    (vacuum)via;
    discus = (DiscusFictus*)datum;
    si (discus->relatio == NIHIL)
    {
        redde FALSUM;
    }
    *relatio_out = chorda_ex_literis(discus->relatio, piscina);
    redde VERUM;
}

interior vacuum
_discum_parare (
          DiscusFictus* discus,
         FabricaSutura* sutura,
               Piscina* piscina)
{
    discus->piscina = piscina;
    discus->fasciculi = xar_creare(piscina,
        (i32)magnitudo(FasciculusFictus));
    discus->directoria = xar_creare(piscina,
        (i32)magnitudo(DirectoriumFictum));
    discus->scriptura_rel  = "gen/exitus.c";
    discus->generatio      = NIHIL;
    discus->fractus        = FALSUM;
    discus->cursus         = ZEPHYRUM;
    discus->relatio        = NIHIL;

    sutura->datum      = discus;
    sutura->legere     = _legere;
    sutura->enumerare  = _enumerare;
    sutura->currere    = _currere;
    sutura->rogare     = _rogare;
    sutura->meminisse  = NIHIL;
}


/* ==================================================
 * Actiones fictae
 * ================================================== */

interior FabricaActio*
_actio (
                 Piscina* piscina,
      constans character* titulus,
    FabricaGenusActionis  genus)
{
    FabricaActio* actio;
          chorda* verbum;

    actio = (FabricaActio*)piscina_allocare(piscina,
        magnitudo(FabricaActio));
    actio->titulus = chorda_ex_literis(titulus, piscina);
    actio->genus = genus;
    actio->mandatum = xar_creare(piscina, (i32)magnitudo(chorda));
    verbum = (chorda*)xar_addere(actio->mandatum);
    *verbum = chorda_ex_literis("tools/generator_fictus.sh", piscina);
    actio->ingressus = xar_creare(piscina,
        (i32)magnitudo(FabricaIngressus));
    actio->exitus = xar_creare(piscina,
        (i32)magnitudo(FabricaExitus));
    actio->sedes = chorda_ex_literis("fixa:1", piscina);
    redde actio;
}

interior vacuum
_ingressum_addere (
             FabricaActio* actio,
    FabricaGenusIngressus  genus,
       constans character* via,
                  Piscina* piscina)
{
    FabricaIngressus* ingressus;

    ingressus         = (FabricaIngressus*)xar_addere(actio->ingressus);
    ingressus->genus  = genus;
    ingressus->via    = chorda_ex_literis(via, piscina);
}

interior FabricaExitus*
_exitum_addere (
                 FabricaActio* actio,
           constans character* via,
          FabricaProvenientia  provenientia,
                      Piscina* piscina)
{
    FabricaExitus* exitus;

    exitus                = (FabricaExitus*)xar_addere(actio->exitus);
    exitus->via           = chorda_ex_literis(via, piscina);
    exitus->scriptura     = chorda_ex_literis("gen/exitus.c", piscina);
    exitus->provenientia  = provenientia;
    redde exitus;
}

interior b32
_sigillum (
    constans FabricaSutura* sutura,
     constans FabricaActio* actio,
                   Piscina* piscina,
                  Sigillum* sigillum)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    redde fabrica_ingressus_sigillare(sutura, actio, NIHIL, piscina,
        sigillum, &causa);
}

interior b32
_continet (
                chorda  fenum,
    constans character* acus,
               Piscina* piscina)
{
    redde chorda_continet(fenum, chorda_ex_literis(acus, piscina));
}


s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_fabrica", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ================================================== */

    /* PROBARE: sigillum copiae - ordo et contentum        */


    /* ================================================== */

    {
              DiscusFictus  discus;
             FabricaSutura  sutura;
              FabricaActio* a;
              FabricaActio* b;
                  Sigillum  sa;
                  Sigillum  sb;

        imprimere("\n--- Probans sigillum copiae ---\n");
        _discum_parare(&discus, &sutura, piscina);
        _ponere(&discus, "lib/x.c", "int x;\n");
        _ponere(&discus, "lib/y.c", "int y;\n");

        a = _actio(piscina, "a", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS, "lib/x.c",
            piscina);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS, "lib/y.c",
            piscina);
        b = _actio(piscina, "b", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(b, FABRICA_INGRESSUS_FASCICULUS, "lib/y.c",
            piscina);
        _ingressum_addere(b, FABRICA_INGRESSUS_FASCICULUS, "lib/x.c",
            piscina);

        CREDO_VERUM(_sigillum(&sutura, a, piscina, &sa));
        CREDO_VERUM(_sigillum(&sutura, b, piscina, &sb));
        /* ordo declarationis nihil refert */
        CREDO_VERUM(memcmp(sa.octeti, sb.octeti, SIGILLUM_OCTETI)
            == 0);

        /* octetus unus mutatus -> sigillum aliud */
        _ponere(&discus, "lib/x.c", "int z;\n");
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &sb));
        CREDO_FALSUM(memcmp(sa.octeti, sb.octeti, SIGILLUM_OCTETI)
            == 0);

        /* contenta permutata inter vias -> sigillum aliud (via in
         * sigillo est, non solum contenta) */
        _ponere(&discus, "lib/x.c", "int y;\n");
        _ponere(&discus, "lib/y.c", "int x;\n");
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &sb));
        CREDO_FALSUM(memcmp(sa.octeti, sb.octeti, SIGILLUM_OCTETI)
            == 0);

        /* via renominata, contentum idem -> sigillum aliud (via IPSA
         * in sigillo est; permutatio supra id non probat - ordo
         * sigillorum contentorum solus eam mutat) */
        {
             FabricaActio* r1;
             FabricaActio* r2;
                 Sigillum  s1;
                 Sigillum  s2;

            _ponere(&discus, "lib/prima.c", "idem\n");
            _ponere(&discus, "lib/secunda.c", "idem\n");
            r1 = _actio(piscina, "r1", FABRICA_ACTIO_GENERATOR);
            _ingressum_addere(r1, FABRICA_INGRESSUS_FASCICULUS,
                "lib/prima.c", piscina);
            r2 = _actio(piscina, "r2", FABRICA_ACTIO_GENERATOR);
            _ingressum_addere(r2, FABRICA_INGRESSUS_FASCICULUS,
                "lib/secunda.c", piscina);
            CREDO_VERUM(_sigillum(&sutura, r1, piscina, &s1));
            CREDO_VERUM(_sigillum(&sutura, r2, piscina, &s2));
            CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
                == 0);
        }

        /* ingressus absens -> FALSUM, via nominata */
        {
            FabricaActio* c;
                  chorda  causa;

            c = _actio(piscina, "c", FABRICA_ACTIO_GENERATOR);
            _ingressum_addere(c, FABRICA_INGRESSUS_FASCICULUS,
                "lib/nusquam.c", piscina);
            causa.datum    = NIHIL;
            causa.mensura  = ZEPHYRUM;
            CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, c, NIHIL,
                piscina, &sb, &causa));
            CREDO_VERUM(_continet(causa, "lib/nusquam.c", piscina));
        }
    }


    /* ================================================== */

    /* PROBARE: manifestum aedilis                         */


    /* ================================================== */

    {
              DiscusFictus  discus;
             FabricaSutura  sutura;
                    chorda  parvum;
                    chorda  inresolutum;
                    chorda  causa;
                       Xar* viae;
                       Xar* inresolutae;
              FabricaActio* a;
                  Sigillum  s;

        imprimere("\n--- Probans manifestum ---\n");
        _discum_parare(&discus, &sutura, piscina);
        parvum = filum_legere_totum(
            "probationes/fixa/fabrica/manifestum_parvum.stml",
            piscina);
        inresolutum = filum_legere_totum(
            "probationes/fixa/fabrica/manifestum_inresolutum.stml",
            piscina);
        CREDO_VERUM(parvum.mensura > 0);
        CREDO_VERUM(inresolutum.mensura > 0);

        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_VERUM(fabrica_manifestum_legere(parvum, piscina, &viae,
            &inresolutae, &causa));
        /* obiecta II + capita II + vendor I; systemata NON */
        CREDO_AEQUALIS_I32(xar_numerus(viae), V);
        CREDO_AEQUALIS_I32(xar_numerus(inresolutae), ZEPHYRUM);
        {
            i32 i;
            b32 systema_inventum;

            systema_inventum = FALSUM;
            per (i = ZEPHYRUM; i < xar_numerus(viae); i++)
            {
                si (chorda_aequalis_literis(
                        *(chorda*)xar_obtinere(viae, i), "stdio.h"))
                {
                    systema_inventum = VERUM;
                }
            }
            CREDO_FALSUM(systema_inventum);
        }

        CREDO_VERUM(fabrica_manifestum_legere(inresolutum, piscina,
            &viae, &inresolutae, &causa));
        CREDO_AEQUALIS_I32(xar_numerus(inresolutae), I);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            *(chorda*)xar_obtinere(inresolutae, ZEPHYRUM), "nusquam.h");

        /* ingressus MANIFESTUM: viae eius sigillantur */
        _ponere(&discus, "build/m.stml", chorda_ut_cstr(parvum,
            piscina));
        _ponere(&discus, "lib/alpha.c", "a\n");
        _ponere(&discus, "lib/beta.c", "b\n");
        _ponere(&discus, "include/alpha.h", "ah\n");
        _ponere(&discus, "include/beta.h", "bh\n");
        _ponere(&discus, "vendor/gamma.c", "g\n");
        a = _actio(piscina, "m", FABRICA_ACTIO_INSTITUTIO);
        _ingressum_addere(a, FABRICA_INGRESSUS_MANIFESTUM,
            "build/m.stml", piscina);
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s));

        /* manifestum incompletum -> FALSUM, inresoluta nominata */
        _ponere(&discus, "build/m.stml", chorda_ut_cstr(inresolutum,
            piscina));
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, a, NIHIL,
            piscina, &s, &causa));
        CREDO_VERUM(_continet(causa, "nusquam.h", piscina));
    }


    /* ================================================== */

    /* PROBARE: directorium - nomen novum (Review Focus 2) */


    /* ================================================== */

    {
                    DiscusFictus  discus;
                   FabricaSutura  sutura;
                    FabricaActio* a;
                        Sigillum  s1;
                        Sigillum  s2;
              constans character* bina[II];
              constans character* terna[III];

        imprimere("\n--- Probans directorium ---\n");
        _discum_parare(&discus, &sutura, piscina);
        bina[0]   = "a.h";
        bina[1]   = "b.h";
        terna[0]  = "a.h";
        terna[1]  = "b.h";
        terna[2]  = "c.h";
        _directorium_ponere(&discus, "include", bina, II);
        a = _actio(piscina, "d", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_DIRECTORIUM, "include",
            piscina);
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s1));

        /* caput novum in radice inclusa: resolutionem priorem
         * obumbrare potest -> sigillum aliud */
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        _directorium_ponere(&discus, "include", terna, III);
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
    }


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
        _discum_parare(&discus, &sutura, piscina);
        _ponere(&discus, "data/fons.txt", "datum\n");
        _ponere(&discus, "gen/exitus.c", "linea I\nlinea II\n");
        a = _actio(piscina, "gen", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS,
            "data/fons.txt", piscina);
        exitus = _exitum_addere(a, "gen/exitus.c",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);

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
        CREDO_VERUM(_continet(iudicium.causa, "lineae differentes: 1)",
            piscina));

        /* generator fractus -> IGNOTUM, mandatum et causa nominata */
        discus.fractus = VERUM;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(_continet(iudicium.causa,
            "tools/generator_fictus.sh", piscina));
        CREDO_VERUM(_continet(iudicium.causa, "error fictus",
            piscina));
        discus.fractus = FALSUM;

        /* generator mutus (successus sine scriptura) -> IGNOTUM, etsi
         * reliquiae cursus prioris ibi stabant (sutura eas vacuat) */
        discus.generatio = "linea I\nlinea II\n";
        (vacuum)fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        discus.generatio = NIHIL;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(_continet(iudicium.causa, "nihil scripsit",
            piscina));

        /* celer: numquam regenerat */
        discus.cursus = ZEPHYRUM;
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status,
            (i32)FABRICA_NON_IUDICATUM);
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);

        /* ingressus absens -> IGNOTUM, nulla regeneratio */
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS,
            "data/absens.txt", piscina);
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(_continet(iudicium.causa, "data/absens.txt",
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
        _discum_parare(&discus, &sutura, piscina);
        _ponere(&discus, "lib/manus.c", "int manus;\n");
        a = _actio(piscina, "manus", FABRICA_ACTIO_INSTITUTIO);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS,
            "lib/manus.c", piscina);
        exitus = _exitum_addere(a, "bin/manus",
            FABRICA_PROVENIENTIA_RELATIO, piscina);
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s));
        sigillum_hex(&s, hex);

        sprintf(relatio, "provenientia 1\nartificium bin/manus\n"
            "ingressus %s\ncommissum abc1234\n", hex);
        discus.relatio = relatio;
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* fons mutatus post institutionem -> STALUM */
        _ponere(&discus, "lib/manus.c", "int manus_nova;\n");
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);

        /* binarium sine relatione -> IGNOTUM, numquam RECENS */
        discus.relatio = NIHIL;
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_IGNOTUM);
        CREDO_VERUM(_continet(iudicium.causa, "sine provenientia",
            piscina));
    }


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
        b = _actio(piscina, "B", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(b, FABRICA_INGRESSUS_FASCICULUS, "gen/a.h",
            piscina);
        (vacuum)_exitum_addere(b, "gen/b.c",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);
        a = _actio(piscina, "A", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS, "data/a",
            piscina);
        (vacuum)_exitum_addere(a, "gen/a.h",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);
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
        a         = _actio(piscina, "C", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS, "gen/d",
            piscina);
        (vacuum)_exitum_addere(a, "gen/c",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);
        b = _actio(piscina, "D", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(b, FABRICA_INGRESSUS_FASCICULUS, "gen/c",
            piscina);
        (vacuum)_exitum_addere(b, "gen/d",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);
        *(FabricaActio*)xar_addere(actiones) = *a;
        *(FabricaActio*)xar_addere(actiones) = *b;
        ordo = fabrica_ordinare(actiones, piscina, &causa);
        CREDO_NIHIL(ordo);
        CREDO_VERUM(_continet(causa, "C", piscina));
        CREDO_VERUM(_continet(causa, "D", piscina));
        CREDO_VERUM(_continet(causa, "cyclus", piscina));
    }


    /* ================================================== */

    /* Compendium                                          */


    /* ================================================== */

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
