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
#include "internamentum.h"
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
                   i32  lecturae;      /* vocationes legere (memoria) */
                   Xar* verificationes; /* VerificatioFicta (T6) */
                   i32  inscriptiones; /* vocationes inscribere */
} DiscusFictus;

/* memoria verificationum ficta (T6): tabula sqlite in memoria */
nomen structura {
      chorda titulus;
    Sigillum ingressus;
    Sigillum artificium;
} VerificatioFicta;

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
    ((DiscusFictus*)datum)->lecturae++;
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
    discus->lecturae       = ZEPHYRUM;
    discus->verificationes = xar_creare(piscina,
        (i32)magnitudo(VerificatioFicta));
    discus->inscriptiones  = ZEPHYRUM;

    fabrica_suturam_parare(sutura);

    sutura->datum      = discus;
    sutura->legere     = _legere;
    sutura->enumerare  = _enumerare;
    sutura->currere    = _currere;
    sutura->rogare     = _rogare;
    sutura->meminisse  = NIHIL;
    sutura->sigilla    = NIHIL;
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
    actio->sedes        = chorda_ex_literis("fixa:1", piscina);
    actio->memorabilis  = FALSUM;
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

    ingressus = (FabricaIngressus*)xar_addere(actio->ingressus);
    ingressus->genus = genus;
    ingressus->via = chorda_ex_literis(via, piscina);
    ingressus->suffixa = chorda_ex_literis("", piscina);
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
        /* scopus I + obiecta II + capita II + vendor I; systemata NON.
         * SCOPUS (fons principalis) inter viae: manifestum eum in
         * attributo solo nominat - sine eo mutatio fontis principalis
         * binarium stalum non faceret (T7: canon_examen et
         * canon_coquere digestum IDEM ferebant) */
        CREDO_AEQUALIS_I32(xar_numerus(viae), VI);
        {
            i32 k;
            b32 scopus_inventus;

            scopus_inventus = FALSUM;
            per (k = ZEPHYRUM; k < xar_numerus(viae); k++)
            {
                si (chorda_aequalis_literis(
                        *(chorda*)xar_obtinere(viae, k),
                        "tools/parvum.c"))
                {
                    scopus_inventus = VERUM;
                }
            }
            CREDO_VERUM(scopus_inventus);
        }
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
        _ponere(&discus, "tools/parvum.c", "int principale;\n");
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


    /* ==================================================
     * PROBARE: declarationes (T2)
     * ================================================== */

    {
         InternamentumChorda* intern;
                      chorda  contentum;
                      chorda  causa;
                         Xar* actiones;
                         Xar* subsystemata;
                FabricaActio* actio;
               FabricaExitus* exitus;
            FabricaIngressus* ingressus;

        imprimere("\n--- Probans declarationes ---\n");
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
        CREDO_AEQUALIS_I32((i32)ingressus->genus,
            (i32)FABRICA_INGRESSUS_MANIFESTUM);
        ingressus = (FabricaIngressus*)xar_obtinere(actio->ingressus,
            III);
        CREDO_AEQUALIS_I32((i32)ingressus->genus,
            (i32)FABRICA_INGRESSUS_DIRECTORIUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(ingressus->via, "include");
        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(exitus->via,
            "silva/amalgama/silva.c");
        CREDO_AEQUALIS_I32((i32)exitus->provenientia,
            (i32)FABRICA_PROVENIENTIA_REGENERATIO);
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
        CREDO_AEQUALIS_I32((i32)exitus->provenientia,
            (i32)FABRICA_PROVENIENTIA_RELATIO);
        /* scriptura absens = via ipsa */
        CREDO_CHORDA_AEQUALIS_LITERIS(exitus->scriptura, "bin/manus");

        /* recusationes: linea nominata */
        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/decl_genus_ignotum.stml",
            piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(_continet(causa, "d.stml:6", piscina));
        CREDO_VERUM(_continet(causa, "compilatio", piscina));

        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/decl_sine_exitu.stml", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(_continet(causa, "d.stml:2", piscina));
        CREDO_VERUM(_continet(causa, "sine exitu", piscina));

        /* titulus duplex: directoria scripturae colliderent */
        contentum = filum_legere_totum(
            "probationes/fixa/fabrica/decl_titulus_duplex.stml",
            piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(_continet(causa, "d.stml:6", piscina));
        CREDO_VERUM(_continet(causa, "gemina", piscina));

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
        CREDO_VERUM(_continet(causa, "d.stml:2", piscina));
        CREDO_VERUM(_continet(causa, "memorabilis", piscina));

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


    /* ==================================================
     * PROBARE: plagula provenientiae exclusa (T7, Review Focus 3)
     * ================================================== */

    {
              DiscusFictus  discus;
             FabricaSutura  sutura;
              FabricaActio* cum;
              FabricaActio* sine;
                  Sigillum  s1;
                  Sigillum  s2;
                    chorda  causa;

        imprimere("\n--- Probans provenientiam exclusam ---\n");
        _discum_parare(&discus, &sutura, piscina);
        causa = chorda_ex_literis("", piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(
            fabrica_provenientia_via(chorda_ex_literis("manus",
            piscina),
                piscina),
            "build/fabrica/provenientia/manus.c");

        _ponere(&discus, "lib/manus.c", "int manus;\n");
        _ponere(&discus, "build/fabrica/provenientia/manus.c", "H1\n");
        cum = _actio(piscina, "manus", FABRICA_ACTIO_INSTITUTIO);
        _ingressum_addere(cum, FABRICA_INGRESSUS_FASCICULUS,
            "lib/manus.c", piscina);
        _ingressum_addere(cum, FABRICA_INGRESSUS_FASCICULUS,
            "build/fabrica/provenientia/manus.c", piscina);
        sine = _actio(piscina, "manus", FABRICA_ACTIO_INSTITUTIO);
        _ingressum_addere(sine, FABRICA_INGRESSUS_FASCICULUS,
            "lib/manus.c", piscina);

        /* plagula provenientiae nihil ad sigillum confert */
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, cum, piscina,
            &s1, &causa));
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, sine, piscina,
            &s2, &causa));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);

        /* digestum novum in ea scriptum -> sigillum idem (aliter
         * binarium statim post institutionem stalum esset) */
        _ponere(&discus, "build/fabrica/provenientia/manus.c", "H2\n");
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, cum, piscina,
            &s2, &causa));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);

        /* fons verus mutatus -> sigillum aliud */
        _ponere(&discus, "lib/manus.c", "int manus_nova;\n");
        CREDO_VERUM(fabrica_actionem_sigillare(&sutura, cum, piscina,
            &s2, &causa));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
    }


    /* ==================================================
     * PROBARE: memoria sigillorum per cursum (celer < II s)
     * ================================================== */

    {
         DiscusFictus  discus;
        FabricaSutura  sutura;
         FabricaActio* a;
             Sigillum  s1;
             Sigillum  s2;
                  i32  lecturae;

        imprimere("\n--- Probans memoriam sigillorum ---\n");
        _discum_parare(&discus, &sutura, piscina);
        _ponere(&discus, "lib/x.c", "int x;\n");
        _ponere(&discus, "lib/y.c", "int y;\n");
        a = _actio(piscina, "m", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS, "lib/x.c",
            piscina);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS, "lib/y.c",
            piscina);
        sutura.sigilla = tabula_dispersa_creare_chorda(piscina, 16);

        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s1));
        lecturae = discus.lecturae;
        CREDO_AEQUALIS_I32(lecturae, II);
        /* iterum: nulla lectio, sigillum idem */
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_AEQUALIS_I32(discus.lecturae, lecturae);
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);
        /* memoria PER CURSUM: mutatio intra cursum non videtur (id
         * consulto - iudicium unum, arbor una) */
        _ponere(&discus, "lib/x.c", "int z;\n");
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);
    }


    /* ==================================================
     * PROBARE: ingressus PLAGULAE (corpus infixum)
     * ================================================== */

    {
            DiscusFictus  discus;
           FabricaSutura  sutura;
            FabricaActio* a;
        FabricaIngressus* ingressus;
                Sigillum  s1;
                Sigillum  s2;
      constans character* nomina[IV];
      constans character* nomina_plus[V];

        imprimere("\n--- Probans plagulas directorii ---\n");
        _discum_parare(&discus, &sutura, piscina);
        nomina[0] = "a.c";
        nomina[1] = "b.h";
        nomina[2] = "notae.md";
        nomina[3] = "sub.c";
        _directorium_ponere(&discus, "src", nomina, IV);
        /* sub.c DIRECTORIUM est, suffixo congruens: praetermittitur */
        _directorium_ponere(&discus, "src/sub.c", nomina, ZEPHYRUM);
        _ponere(&discus, "src/a.c", "int a;\n");
        _ponere(&discus, "src/b.h", "int b;\n");
        _ponere(&discus, "src/notae.md", "notae\n");
        a = _actio(piscina, "p", FABRICA_ACTIO_INSTITUTIO);
        _ingressum_addere(a, FABRICA_INGRESSUS_PLAGULAE, "src",
            piscina);
        ingressus = (FabricaIngressus*)xar_obtinere(a->ingressus,
            ZEPHYRUM);
        ingressus->suffixa = chorda_ex_literis(".c .h", piscina);

        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s1));
        /* plagula suffixo non congruens: nihil confert */
        _ponere(&discus, "src/notae.md", "notae mutatae\n");
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);
        /* contentum congruentis mutatum -> aliud */
        _ponere(&discus, "src/b.h", "int b2;\n");
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
        /* plagula nova congruens -> aliud (rebake post lib/) */
        _ponere(&discus, "src/b.h", "int b;\n");
        nomina_plus[0] = "a.c";
        nomina_plus[1] = "b.h";
        nomina_plus[2] = "d.c";
        nomina_plus[3] = "notae.md";
        nomina_plus[4] = "sub.c";
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        _directorium_ponere(&discus, "src", nomina_plus, V);
        _directorium_ponere(&discus, "src/sub.c", nomina, ZEPHYRUM);
        _ponere(&discus, "src/d.c", "int d;\n");
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
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
        _discum_parare(&discus, &sutura, piscina);
        sutura.meminisse   = _meminisse;
        sutura.inscribere  = _inscribere;
        _ponere(&discus, "data/fons.txt", "datum\n");
        _ponere(&discus, "gen/exitus.c", "linea I\n");
        a = _actio(piscina, "gen", FABRICA_ACTIO_GENERATOR);
        a->memorabilis = VERUM;
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS,
            "data/fons.txt", piscina);
        exitus = _exitum_addere(a, "gen/exitus.c",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);
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
        CREDO_VERUM(_continet(iudicium.causa, "memoria", piscina));
        CREDO_AEQUALIS_I32(discus.cursus, ZEPHYRUM);
        /* hit non iterum inscribitur */
        CREDO_AEQUALIS_I32(discus.inscriptiones, I);

        /* et celer: verificatio iudicat ubi regeneratio omittitur */
        iudicium = fabrica_iudicare(&sutura, a, exitus, FALSUM,
            piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_RECENS);

        /* artificium MANU mutatum, ingressus idem -> memoria non
         * congruit: regeneratio, STALUM (Review Focus 1) */
        _ponere(&discus, "gen/exitus.c", "linea I manu mutata\n");
        iudicium = fabrica_iudicare(&sutura, a, exitus, VERUM, piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_AEQUALIS_I32(discus.cursus, I);
        CREDO_AEQUALIS_I32(discus.inscriptiones, I);

        /* actio NON memorabilis: verificatio numquam consulitur nec
         * scribitur (ingressus non provabiliter pleni) */
        _ponere(&discus, "gen/exitus.c", "linea I\n");
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
        _discum_parare(&discus, &sutura, piscina);
        sutura.meminisse   = _meminisse;
        sutura.inscribere  = _inscribere;
        _ponere(&discus, "data/fons.txt", "datum\n");
        _ponere(&discus, "gen/exitus.c", "linea I\n");
        a = _actio(piscina, "gen", FABRICA_ACTIO_GENERATOR);
        a->memorabilis = VERUM;
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS,
            "data/fons.txt", piscina);
        exitus = _exitum_addere(a, "gen/exitus.c",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);
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
     * PROBARE: ingressus MANIFESTA (directorium manifestorum)
     * ================================================== */

    {
            DiscusFictus  discus;
           FabricaSutura  sutura;
            FabricaActio* a;
                Sigillum  s1;
                Sigillum  s2;
                  chorda  causa;
      constans character* nomina[III];
      constans character* nomina_minus[II];

        imprimere("\n--- Probans manifesta ---\n");
        _discum_parare(&discus, &sutura, piscina);
        nomina[0] = "a.stml";
        nomina[1] = "b.stml";
        nomina[2] = "notae.txt";
        _directorium_ponere(&discus, "build/cl", nomina, III);
        _ponere(&discus, "build/cl/a.stml",
            "<aedilis-manifestum scopus=\"tools/a.c\">\n"
            "  <obiecta><obiectum via=\"lib/x.c\"/></obiecta>\n"
            "</aedilis-manifestum>\n");
        _ponere(&discus, "build/cl/b.stml",
            "<aedilis-manifestum scopus=\"tools/b.c\">\n"
            "  <capita><caput via=\"include/y.h\"/></capita>\n"
            "</aedilis-manifestum>\n");
        _ponere(&discus, "build/cl/notae.txt", "non manifestum\n");
        _ponere(&discus, "tools/a.c", "int a;\n");
        _ponere(&discus, "tools/b.c", "int b;\n");
        _ponere(&discus, "lib/x.c", "int x;\n");
        _ponere(&discus, "include/y.h", "int y;\n");
        a = _actio(piscina, "m", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_MANIFESTA, "build/cl",
            piscina);
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s1));

        /* plagula in clausura manifesti SECUNDI mutata -> aliud */
        _ponere(&discus, "include/y.h", "int y2;\n");
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);
        _ponere(&discus, "include/y.h", "int y;\n");

        /* plagula non .stml nihil confert */
        _ponere(&discus, "build/cl/notae.txt", "mutatae\n");
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_VERUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI) == 0);

        /* manifestum ablatum (radix deleta) -> aliud */
        nomina_minus[0] = "a.stml";
        nomina_minus[1] = "notae.txt";
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        _directorium_ponere(&discus, "build/cl", nomina_minus, II);
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);

        /* directorium absens (clonus recens) -> FALSUM nominatum */
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, a, NIHIL,
            piscina, &s2, &causa));
        CREDO_VERUM(_continet(causa, "build/cl", piscina));
    }


    /* ==================================================
     * PROBARE: ingressus RADICES (radices inclusionum configurationis)
     * ================================================== */

    {
            DiscusFictus  discus;
           FabricaSutura  sutura;
            FabricaActio* a;
                Sigillum  s1;
                Sigillum  s2;
                  chorda  causa;
      constans character* include_nomina[I];
      constans character* src_nomina[I];
      constans character* src_plus[II];

        imprimere("\n--- Probans radices ---\n");
        _discum_parare(&discus, &sutura, piscina);
        _ponere(&discus, "aedilis.stml",
            "<aedilis>\n"
            "  <inclusa>\n"
            "    <via (>include\n"
            "    <via (>src\n"
            "  </inclusa>\n"
            "</aedilis>\n");
        include_nomina[0]  = "a.h";
        src_nomina[0]      = "b.h";
        _directorium_ponere(&discus, "include", include_nomina, I);
        _directorium_ponere(&discus, "src", src_nomina, I);
        a = _actio(piscina, "r", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_RADICES, "aedilis.stml",
            piscina);
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s1));

        /* caput novum in radice SECUNDA -> aliud (Review Focus 2) */
        src_plus[0] = "b.h";
        src_plus[1] = "c.h";
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        _directorium_ponere(&discus, "include", include_nomina, I);
        _directorium_ponere(&discus, "src", src_plus, II);
        CREDO_VERUM(_sigillum(&sutura, a, piscina, &s2));
        CREDO_FALSUM(memcmp(s1.octeti, s2.octeti, SIGILLUM_OCTETI)
            == 0);

        /* radix nominata sed absens -> FALSUM nominatum */
        discus.directoria = xar_creare(piscina,
            (i32)magnitudo(DirectoriumFictum));
        _directorium_ponere(&discus, "include", include_nomina, I);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, a, NIHIL,
            piscina, &s2, &causa));
        CREDO_VERUM(_continet(causa, "src", piscina));

        /* configuratio sine sectione inclusarum -> FALSUM (radices
         * nullae = tegmen nullum, numquam tacitum) */
        _directorium_ponere(&discus, "src", src_nomina, I);
        _ponere(&discus, "aedilis.stml", "<aedilis>\n</aedilis>\n");
        CREDO_FALSUM(fabrica_ingressus_sigillare(&sutura, a, NIHIL,
            piscina, &s2, &causa));
        CREDO_VERUM(_continet(causa, "inclusa", piscina));
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
        _discum_parare(&discus, &sutura, piscina);
        sutura.regenerationes = tabula_dispersa_creare_chorda(piscina,
            16);
        _ponere(&discus, "data/fons.txt", "datum\n");
        _ponere(&discus, "gen/a.c", "idem\n");
        _ponere(&discus, "gen/b.c", "idem\n");
        a = _actio(piscina, "multi", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, FABRICA_INGRESSUS_FASCICULUS,
            "data/fons.txt", piscina);
        primus = _exitum_addere(a, "gen/a.c",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);
        secundus = _exitum_addere(a, "gen/b.c",
            FABRICA_PROVENIENTIA_REGENERATIO, piscina);
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
        CREDO_VERUM(_continet(iudicium.causa, "error fictus", piscina));
        CREDO_AEQUALIS_I32(discus.cursus, I);
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
