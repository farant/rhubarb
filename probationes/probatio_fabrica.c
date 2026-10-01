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
                   Xar* scripta;       /* ScriptumFictum (1b T3) */
                   i32  acta;          /* vocationes agere */
} DiscusFictus;

/* generator scriptus (plan 1b T3): verbum = mandatum[0] actionis.
 * Contentum = praefixum + contentum(fons) - fons DISCI HODIERNI
 * legitur,
 * ergo dependentia vera modulatur. Regeneratio (currere) semper
 * scribit in scripturam; agere in loco, nisi mutus; codex agere
 * solum afficit; relatio (non NIHIL) post agere ponitur. */
nomen structura {
    constans character* verbum;
    constans character* via;
    constans character* fons;
    constans character* praefixum;
                   s32  codex;
                   b32  mutus;
    constans character* relatio;
} ScriptumFictum;

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

/* scriptum actionis per mandatum[0]; NIHIL si nullum */
interior ScriptumFictum*
_scriptum_invenire (
     DiscusFictus* discus,
     constans Xar* mandatum)
{
    chorda verbum;
       i32 i;

    si (discus->scripta == NIHIL || xar_numerus(mandatum) == 0)
    {
        redde NIHIL;
    }
    verbum = *(chorda*)xar_obtinere(mandatum, ZEPHYRUM);
    per (i = ZEPHYRUM; i < xar_numerus(discus->scripta); i++)
    {
        ScriptumFictum* scriptum;

        scriptum = (ScriptumFictum*)xar_obtinere(discus->scripta, i);
        si (chorda_aequalis_literis(verbum, scriptum->verbum))
        {
            redde scriptum;
        }
    }
    redde NIHIL;
}

/* contentum generatum: praefixum + contentum fontis hodiernum */
interior constans character*
_generare (
      DiscusFictus* discus,
    ScriptumFictum* scriptum)
{
     FasciculusFictus* fons;
            character* exitus;
       memoriae_index  mensura;

    fons = (scriptum->fons != NIHIL)
        ? _fasciculum_invenire(discus, scriptum->fons) : NIHIL;
    mensura = strlen(scriptum->praefixum)
        + (fons != NIHIL ? (memoriae_index)fons->contentum.mensura : 0)
        + 1;
    exitus = (character*)piscina_allocare(discus->piscina, mensura);
    strcpy(exitus, scriptum->praefixum);
    si (fons != NIHIL)
    {
        memcpy(exitus + strlen(scriptum->praefixum),
            fons->contentum.datum,
            (memoriae_index)fons->contentum.mensura);
        exitus[mensura - 1] = '\0';
    }
    redde exitus;
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
    {
        ScriptumFictum* scriptum;

        scriptum = _scriptum_invenire(discus, mandatum);
        si (scriptum != NIHIL)
        {
            character via[256];

            sprintf(via, "%s/%s", scriptura_dir, scriptum->via);
            _ponere(discus, via, _generare(discus, scriptum));
            redde VERUM;
        }
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
_agere (
                   vacuum* datum,
    constans FabricaActio* actio,
       constans character* acta_via,
                  Piscina* piscina,
             FabricaActum* actum_out)
{
      DiscusFictus* discus;
    ScriptumFictum* scriptum;

    (vacuum)acta_via;
    discus = (DiscusFictus*)datum;
    discus->acta++;
    actum_out->duratio_ms = I;
    actum_out->cauda = chorda_ex_literis("", piscina);
    scriptum = _scriptum_invenire(discus, actio->mandatum);
    si (scriptum == NIHIL)
    {
        actum_out->codex = -I;
        actum_out->cauda = chorda_ex_literis("scriptum nullum",
            piscina);
        redde FALSUM;
    }
    si (!scriptum->mutus)
    {
        _ponere(discus, scriptum->via, _generare(discus, scriptum));
    }
    si (scriptum->relatio != NIHIL)
    {
        discus->relatio = scriptum->relatio;
    }
    actum_out->codex = scriptum->codex;
    si (scriptum->codex != 0)
    {
        actum_out->cauda = chorda_ex_literis("error ficti acti",
            piscina);
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
    discus->scripta        = xar_creare(piscina,
        (i32)magnitudo(ScriptumFictum));
    discus->acta           = ZEPHYRUM;

    fabrica_suturam_parare(sutura);

    sutura->datum      = discus;
    sutura->legere     = _legere;
    sutura->enumerare  = _enumerare;
    sutura->currere    = _currere;
    sutura->rogare     = _rogare;
    sutura->agere      = _agere;
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
    actio->praecondiciones = xar_creare(piscina,
        (i32)magnitudo(chorda));
    redde actio;
}

interior constans FabricaGenus*
_genus (
    constans character* titulus,
               Piscina* piscina)
{
    redde fabrica_genus_invenire(chorda_ex_literis(titulus, piscina));
}

interior constans FabricaStrategia*
_strategia (
    constans character* titulus,
               Piscina* piscina)
{
    redde fabrica_strategia_invenire(chorda_ex_literis(titulus,
        piscina));
}

interior vacuum
_ingressum_addere (
           FabricaActio* actio,
     constans character* genus,
     constans character* via,
                Piscina* piscina)
{
    FabricaIngressus* ingressus;

    ingressus = (FabricaIngressus*)xar_addere(actio->ingressus);
    ingressus->genus = _genus(genus, piscina);
    ingressus->via = chorda_ex_literis(via, piscina);
    ingressus->suffixa = chorda_ex_literis("", piscina);
}

/* genus exitus = genus ordinarium strategiae (ut lector facit) */
interior FabricaExitus*
_exitum_addere (
           FabricaActio* actio,
     constans character* via,
     constans character* strategia,
                Piscina* piscina)
{
    FabricaExitus* exitus;

    exitus             = (FabricaExitus*)xar_addere(actio->exitus);
    exitus->via        = chorda_ex_literis(via, piscina);
    exitus->scriptura  = chorda_ex_literis("gen/exitus.c", piscina);
    exitus->strategia  = _strategia(strategia, piscina);
    exitus->genus      = (exitus->strategia != NIHIL)
        ? _genus(exitus->strategia->genus_ordinarium, piscina) : NIHIL;
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


/* ==================================================
 * Sanare: auxilia (plan 1b T3)
 * ================================================== */

interior vacuum
_scriptum_addere (
          DiscusFictus* discus,
    constans character* verbum,
    constans character* via,
    constans character* fons,
    constans character* praefixum,
                   s32  codex,
                   b32  mutus)
{
    ScriptumFictum* scriptum;

    scriptum = (ScriptumFictum*)xar_addere(discus->scripta);
    si (scriptum == NIHIL)
    {
        redde;
    }
    scriptum->verbum     = verbum;
    scriptum->via        = via;
    scriptum->fons       = fons;
    scriptum->praefixum  = praefixum;
    scriptum->codex      = codex;
    scriptum->mutus      = mutus;
    scriptum->relatio    = NIHIL;
}

/* actio cum generatore scripto, ingressu uno, exitu uno (scriptura =
 * via: regeneratio in scriptura/VIA cadit) */
interior FabricaActio*
_actio_scripta (
               Piscina* piscina,
    constans character* titulus,
    constans character* verbum,
    constans character* ingressus,
    constans character* exitus,
    constans character* strategia)
{
     FabricaActio* actio;
    FabricaExitus* e;

    actio = _actio(piscina, titulus, FABRICA_ACTIO_GENERATOR);
    *(chorda*)xar_obtinere(actio->mandatum, ZEPHYRUM) =
        chorda_ex_literis(verbum, piscina);
    _ingressum_addere(actio, "fasciculus", ingressus, piscina);
    e             = _exitum_addere(actio, exitus, strategia, piscina);
    e->scriptura  = e->via;
    redde actio;
}

interior Xar*
_ordinare_fictas (
          Piscina* piscina,
     FabricaActio* actiones[],
              i32  numerus)
{
       Xar* valores;
    chorda  causa;
       i32  i;

    valores = xar_creare(piscina, (i32)magnitudo(FabricaActio));
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        *(FabricaActio*)xar_addere(valores) = *actiones[i];
    }
    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    redde fabrica_ordinare(valores, piscina, &causa);
}

interior FabricaSanatio*
_sanatio_invenire (
                   Xar* sanationes,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(sanationes); i++)
    {
        FabricaSanatio* sanatio;

        sanatio = (FabricaSanatio*)xar_obtinere(sanationes, i);
        si (chorda_aequalis_literis(sanatio->actio->titulus, titulus))
        {
            redde sanatio;
        }
    }
    redde NIHIL;
}

/* memoriae per cursum, ut sutura vera (bin/fabrica) eas habet */
interior vacuum
_memorias_parare (
    FabricaSutura* sutura,
          Piscina* piscina)
{
    sutura->sigilla = tabula_dispersa_creare_chorda(piscina,
        16);
    sutura->regenerationes = tabula_dispersa_creare_chorda(piscina,
        16);
    sutura->digesta = tabula_dispersa_creare_chorda(piscina,
        16);
}

interior vacuum
_chordam_addere_test (
                   Xar* xar,
    constans character* valor,
               Piscina* piscina)
{
    *(chorda*)xar_addere(xar) = chorda_ex_literis(valor, piscina);
}

interior b32
_contentum_est (
          DiscusFictus* discus,
    constans character* via,
    constans character* contentum)
{
    FasciculusFictus* f;

    f = _fasciculum_invenire(discus, via);
    redde f != NIHIL
        && chorda_aequalis_literis(f->contentum, contentum);
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
        _ingressum_addere(a, "fasciculus", "lib/x.c",
            piscina);
        _ingressum_addere(a, "fasciculus", "lib/y.c",
            piscina);
        b = _actio(piscina, "b", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(b, "fasciculus", "lib/y.c",
            piscina);
        _ingressum_addere(b, "fasciculus", "lib/x.c",
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
            _ingressum_addere(r1, "fasciculus",
                "lib/prima.c", piscina);
            r2 = _actio(piscina, "r2", FABRICA_ACTIO_GENERATOR);
            _ingressum_addere(r2, "fasciculus",
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
            _ingressum_addere(c, "fasciculus",
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
        _ingressum_addere(a, "manifestum",
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
        _ingressum_addere(a, "directorium", "include",
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
        _ingressum_addere(a, "fasciculus",
            "data/fons.txt", piscina);
        exitus = _exitum_addere(a, "gen/exitus.c",
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
        _ingressum_addere(a, "fasciculus",
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
        _ingressum_addere(a, "fasciculus",
            "lib/manus.c", piscina);
        exitus = _exitum_addere(a, "bin/manus",
            "relatio", piscina);
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
        _ingressum_addere(b, "fasciculus", "gen/a.h",
            piscina);
        (vacuum)_exitum_addere(b, "gen/b.c",
            "regeneratio", piscina);
        a = _actio(piscina, "A", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, "fasciculus", "data/a",
            piscina);
        (vacuum)_exitum_addere(a, "gen/a.h",
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
        a         = _actio(piscina, "C", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, "fasciculus", "gen/d",
            piscina);
        (vacuum)_exitum_addere(a, "gen/c",
            "regeneratio", piscina);
        b = _actio(piscina, "D", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(b, "fasciculus", "gen/c",
            piscina);
        (vacuum)_exitum_addere(b, "gen/d",
            "regeneratio", piscina);
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
        CREDO_VERUM(ingressus->genus == _genus("manifestum", piscina));
        ingressus = (FabricaIngressus*)xar_obtinere(actio->ingressus,
            III);
        CREDO_VERUM(ingressus->genus == _genus("directorium", piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS(ingressus->via, "include");
        exitus = (FabricaExitus*)xar_obtinere(actio->exitus, ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(exitus->via,
            "silva/amalgama/silva.c");
        CREDO_VERUM(exitus->strategia == _strategia("regeneratio",
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
        CREDO_VERUM(exitus->strategia == _strategia("relatio",
            piscina));
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
        _ingressum_addere(cum, "fasciculus",
            "lib/manus.c", piscina);
        _ingressum_addere(cum, "fasciculus",
            "build/fabrica/provenientia/manus.c", piscina);
        sine = _actio(piscina, "manus", FABRICA_ACTIO_INSTITUTIO);
        _ingressum_addere(sine, "fasciculus",
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
        _ingressum_addere(a, "fasciculus", "lib/x.c",
            piscina);
        _ingressum_addere(a, "fasciculus", "lib/y.c",
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
        _ingressum_addere(a, "plagulae", "src",
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
        _ingressum_addere(a, "fasciculus",
            "data/fons.txt", piscina);
        exitus = _exitum_addere(a, "gen/exitus.c",
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
        _ingressum_addere(a, "fasciculus",
            "data/fons.txt", piscina);
        exitus = _exitum_addere(a, "gen/exitus.c",
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
        _ingressum_addere(a, "manifesta", "build/cl",
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
        _ingressum_addere(a, "radices", "aedilis.stml",
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
     * PROBARE: actio tacta a viis commissis (T8 gradus III)
     * ================================================== */

    {
            DiscusFictus  discus;
           FabricaSutura  sutura;
            FabricaActio* a;
            FabricaActio* m;
            FabricaActio* p;
        FabricaIngressus* ingressus;
                     Xar* viae;
      constans character* nomina[II];
      constans character* src_nomina[I];

        imprimere("\n--- Probans actionem tactam ---\n");
        _discum_parare(&discus, &sutura, piscina);
        _ponere(&discus, "lib/x.c", "int x;\n");
        _ponere(&discus, "gen/exitus.c", "g\n");
        nomina[0] = "a.h";
        nomina[1] = "b.h";
        _directorium_ponere(&discus, "include", nomina, II);
        a = _actio(piscina, "a", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(a, "fasciculus", "lib/x.c",
            piscina);
        _ingressum_addere(a, "directorium", "include",
            piscina);
        (vacuum)_exitum_addere(a, "gen/exitus.c",
            "regeneratio", piscina);

        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("lib/y.c",
            piscina);
        /* via aliena -> non tacta */
        CREDO_FALSUM(fabrica_actio_tacta(&sutura, a, viae, piscina));
        /* ingressus ipse -> tacta */
        *(chorda*)xar_addere(viae) = chorda_ex_literis("lib/x.c",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, a, viae, piscina));
        /* plagula nova in directorio enumerato (include/nova.h) */
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("include/nova.h",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, a, viae, piscina));
        /* exitus ipse commissus -> tacta */
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("gen/exitus.c",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, a, viae, piscina));

        /* manifestum: plagula in clausura eius */
        _ponere(&discus, "build/m.stml",
            "<aedilis-manifestum scopus=\"tools/m.c\">\n"
            "  <capita><caput via=\"include/a.h\"/></capita>\n"
            "</aedilis-manifestum>\n");
        _ponere(&discus, "tools/m.c", "int m;\n");
        _ponere(&discus, "include/a.h", "int a;\n");
        m = _actio(piscina, "m", FABRICA_ACTIO_GENERATOR);
        _ingressum_addere(m, "manifestum",
            "build/m.stml",
            piscina);
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("include/a.h",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, m, viae, piscina));

        /* plagulae: plagula nova suffixo congruens in directorio */
        src_nomina[0] = "a.c";
        _directorium_ponere(&discus, "src", src_nomina, I);
        _ponere(&discus, "src/a.c", "int a;\n");
        p = _actio(piscina, "p", FABRICA_ACTIO_INSTITUTIO);
        _ingressum_addere(p, "plagulae", "src",
            piscina);
        ingressus = (FabricaIngressus*)xar_obtinere(p->ingressus,
            ZEPHYRUM);
        ingressus->suffixa = chorda_ex_literis(".c", piscina);
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("src/nova.c",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, p, viae, piscina));
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("src/notae.md",
            piscina);
        CREDO_FALSUM(fabrica_actio_tacta(&sutura, p, viae, piscina));

        /* ingressus absens -> tacta (conservativum: iudex IGNOTUM
         * nominabit, numquam tacite praetermittitur) */
        _ingressum_addere(a, "fasciculus",
            "lib/abest.c",
            piscina);
        viae = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(viae) = chorda_ex_literis("doc/nihil.md",
            piscina);
        CREDO_VERUM(fabrica_actio_tacta(&sutura, a, viae, piscina));
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
        _ingressum_addere(a, "fasciculus",
            "data/fons.txt", piscina);
        primus = _exitum_addere(a, "gen/a.c",
            "regeneratio", piscina);
        secundus = _exitum_addere(a, "gen/b.c",
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
        CREDO_VERUM(_continet(iudicium.causa, "error fictus", piscina));
        CREDO_AEQUALIS_I32(discus.cursus, I);
    }


    /* ==================================================
     * PROBARE: genera et strategiae (plan 1b T1)
     * ================================================== */

    {
               DiscusFictus  discus;
              FabricaSutura  sutura;
               FabricaActio* p;
           FabricaIngressus* ingressus;
              FabricaExitus  exitus;
               FabricaLocus* locus;
        InternamentumChorda* intern;
                     chorda  contentum;
                     chorda  causa;
                        Xar* loci;
                        Xar* actiones;
                        i32  i;
                        b32  plagulae_inventae;
                        b32  plagula_inventa;
         constans character* genera[IX];
         constans character* src_nomina[II];

        imprimere("\n--- Probans genera et strategias ---\n");
        intern         = internamentum_creare(piscina);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* registrum generum: omnia nomina declarationum, sigillare
         * omnibus */
        genera[0] = "fasciculus";
        genera[1] = "configuratio";
        genera[2] = "instrumentum";
        genera[3] = "directorium";
        genera[4] = "manifestum";
        genera[5] = "plagulae";
        genera[6] = "manifesta";
        genera[7] = "radices";
        genera[8] = "binarium";
        per (i = ZEPHYRUM; i < IX; i++)
        {
            CREDO_NON_NIHIL(_genus(genera[i], piscina));
            CREDO_VERUM(_genus(genera[i], piscina)->sigillare != NIHIL);
        }
        CREDO_NIHIL(_genus("compilatio", piscina));

        /* exitus esse possunt: fasciculus, binarium; ceteri non */
        CREDO_VERUM(_genus("fasciculus", piscina)->locare != NIHIL);
        CREDO_VERUM(_genus("binarium", piscina)->locare != NIHIL);
        CREDO_VERUM(_genus("manifestum", piscina)->locare == NIHIL);
        CREDO_VERUM(_genus("fasciculus", piscina)->reproducibile);
        CREDO_FALSUM(_genus("binarium", piscina)->reproducibile);

        /* locare: locus unus, PLAGULA, via exitus */
        exitus.via = chorda_ex_literis("bin/manus", piscina);
        exitus.scriptura = exitus.via;
        exitus.genus = _genus("binarium", piscina);
        exitus.strategia = _strategia("relatio", piscina);
        loci = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
        CREDO_VERUM(exitus.genus->locare(&exitus, piscina, loci));
        CREDO_AEQUALIS_I32(xar_numerus(loci), I);
        locus = (FabricaLocus*)xar_obtinere(loci, ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)locus->forma,
            (i32)FABRICA_LOCUS_PLAGULA);
        CREDO_CHORDA_AEQUALIS_LITERIS(locus->via, "bin/manus");

        /* registrum strategiarum */
        CREDO_NON_NIHIL(_strategia("regeneratio", piscina));
        CREDO_NON_NIHIL(_strategia("relatio", piscina));
        CREDO_NIHIL(_strategia("compilatio", piscina));
        CREDO_VERUM(_strategia("regeneratio",
            piscina)->octetis_comparat);
        CREDO_FALSUM(_strategia("relatio", piscina)->octetis_comparat);
        CREDO_VERUM(strcmp(_strategia("relatio",
            piscina)->genus_ordinarium, "binarium") == 0);
        CREDO_VERUM(strcmp(_strategia("regeneratio",
            piscina)->genus_ordinarium, "fasciculus") == 0);

        /* lector: genus exitus absens -> genus ordinarium */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"i\" genus=\"institutio\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"bin/i\" provenientia=\"relatio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        actiones = fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa);
        CREDO_NON_NIHIL(actiones);
        CREDO_VERUM(((FabricaExitus*)xar_obtinere(((FabricaActio*)
            xar_obtinere(actiones, ZEPHYRUM))->exitus, ZEPHYRUM))->genus
            == _genus("binarium", piscina));

        /* lector: regeneratio generi non reproducibili recusatur */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"b\" genus=\"binarium\""
            " provenientia=\"regeneratio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(_continet(causa, "d.stml:4", piscina));
        CREDO_VERUM(_continet(causa,
            "strategia regeneratio generi binarium non licet",
            piscina));

        /* lector: genus ingressus solum exitus esse nequit */
        contentum = chorda_ex_literis(
            "<aedificatio>\n"
            "  <actio titulus=\"g\" genus=\"generator\">\n"
            "    <ingressus genus=\"fasciculus\" via=\"a\"/>\n"
            "    <exitus via=\"b\" genus=\"manifestum\""
            " provenientia=\"regeneratio\"/>\n"
            "  </actio>\n"
            "</aedificatio>\n", piscina);
        CREDO_NIHIL(fabrica_declarationes_legere(contentum, "d.stml",
            piscina, intern, &causa));
        CREDO_VERUM(_continet(causa, "exitus esse nequit", piscina));

        /* enumerare: plagulae -> locus PLAGULAE cum suffixis, et
         * PLAGULA pro quaque plagula congruente */
        _discum_parare(&discus, &sutura, piscina);
        src_nomina[0] = "a.c";
        src_nomina[1] = "notae.md";
        _directorium_ponere(&discus, "src", src_nomina, II);
        _ponere(&discus, "src/a.c", "int a;\n");
        _ponere(&discus, "src/notae.md", "notae\n");
        p = _actio(piscina, "p", FABRICA_ACTIO_INSTITUTIO);
        _ingressum_addere(p, "plagulae", "src", piscina);
        ingressus = (FabricaIngressus*)xar_obtinere(p->ingressus,
            ZEPHYRUM);
        ingressus->suffixa = chorda_ex_literis(".c", piscina);
        loci = xar_creare(piscina, (i32)magnitudo(FabricaLocus));
        CREDO_VERUM(fabrica_actionem_enumerare(&sutura, p, piscina,
            loci, &causa));
        plagulae_inventae  = FALSUM;
        plagula_inventa    = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(loci); i++)
        {
            locus = (FabricaLocus*)xar_obtinere(loci, i);
            si (   locus->forma == FABRICA_LOCUS_PLAGULAE
                && chorda_aequalis_literis(locus->via, "src")
                && chorda_aequalis_literis(locus->suffixa, ".c"))
            {
                plagulae_inventae = VERUM;
            }
            si (   locus->forma == FABRICA_LOCUS_PLAGULA
                && chorda_aequalis_literis(locus->via, "src/a.c"))
            {
                plagula_inventa = VERUM;
            }
            /* plagula suffixo non congruens nusquam */
            CREDO_FALSUM(chorda_aequalis_literis(locus->via,
                "src/notae.md"));
        }
        CREDO_VERUM(plagulae_inventae);
        CREDO_VERUM(plagula_inventa);
    }


    /* ==================================================
     * PROBARE: composita et praecondiciones (plan 1b T2)
     * ================================================== */

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

        imprimere("\n--- Probans composita et praecondiciones ---\n");
        intern         = internamentum_creare(piscina);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* strategia ignota: numquam iudicatur */
        CREDO_NON_NIHIL(_strategia("ignota", piscina));
        CREDO_FALSUM(_strategia("ignota", piscina)->iudicatur);
        CREDO_VERUM(_strategia("regeneratio", piscina)->iudicatur);
        CREDO_VERUM(_strategia("relatio", piscina)->iudicatur);

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
        CREDO_VERUM(_continet(causa, "cyclus", piscina));
        CREDO_VERUM(_continet(causa, "cyclus_a", piscina));
        CREDO_VERUM(_continet(causa, "cyclus_b", piscina));

        /* pars ignota: sedes et nomen */
        CREDO_NIHIL(fabrica_compositum_explicare(composita, actiones,
            chorda_ex_literis("orbum", piscina), piscina, &causa));
        CREDO_VERUM(_continet(causa, "d.stml:30", piscina));
        CREDO_VERUM(_continet(causa, "nusquam", piscina));

        /* exitus ignotus pars esse nequit (pessimum semper IGNOTUM) */
        CREDO_NIHIL(fabrica_compositum_explicare(composita, actiones,
            chorda_ex_literis("cum_praecondicione", piscina), piscina,
            &causa));
        CREDO_VERUM(_continet(causa, "praecondicio", piscina));

        /* compositum ignotum */
        CREDO_NIHIL(fabrica_compositum_explicare(composita, actiones,
            chorda_ex_literis("nullum", piscina), piscina, &causa));
        CREDO_VERUM(_continet(causa, "compositum ignotum", piscina));

        /* lector: pars nominibus duobus, compositum sine parte */
        CREDO_NIHIL(fabrica_composita_legere(chorda_ex_literis(
            "<aedificatio>\n"
            "  <compositum titulus=\"c\">\n"
            "    <pars actio=\"g\" artificium=\"x\"/>\n"
            "  </compositum>\n"
            "</aedificatio>\n", piscina), "d.stml", piscina, intern,
            &causa));
        CREDO_VERUM(_continet(causa, "d.stml:3", piscina));
        CREDO_NIHIL(fabrica_composita_legere(chorda_ex_literis(
            "<aedificatio>\n"
            "  <compositum titulus=\"c\">\n"
            "  </compositum>\n"
            "</aedificatio>\n", piscina), "d.stml", piscina, intern,
            &causa));
        CREDO_VERUM(_continet(causa, "sine parte", piscina));

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
        CREDO_VERUM(_continet(iudicium.causa, "z", piscina));
        locus              = (FabricaIudicium*)xar_addere(iudicia);
        locus->artificium  = chorda_ex_literis("y", piscina);
        locus->status      = FABRICA_STALUM;
        locus->causa       = chorda_ex_literis("", piscina);
        iudicium = fabrica_iudicia_coniungere(iudicia,
            chorda_ex_literis("c", piscina), piscina);
        CREDO_AEQUALIS_I32((i32)iudicium.status, (i32)FABRICA_STALUM);
        CREDO_CHORDA_AEQUALIS_LITERIS(iudicium.artificium, "c");
        CREDO_VERUM(_continet(iudicium.causa, "y", piscina));
        /* pars non pessima non nominatur */
        CREDO_FALSUM(_continet(iudicium.causa, "z", piscina));
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
        CREDO_VERUM(_continet(iudicium.causa, "+II", piscina));

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
        CREDO_VERUM(_continet(causa, "d.stml:2", piscina));
        CREDO_VERUM(_continet(causa, "nulla", piscina));

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
        CREDO_VERUM(_continet(causa, "praecondicio", piscina));
        CREDO_VERUM(_continet(causa, "build/o", piscina));
    }


    /* ==================================================
     * PROBARE: sanare (plan 1b T3, Review Focus 2-4)
     * ================================================== */

    {
          DiscusFictus  discus;
         FabricaSutura  sutura;
         FabricaSutura  nuda;
          FabricaActio* actiones[III];
          FabricaActio* b;
                   Xar* ordo;
                   Xar* sanationes;
                   Xar* electa;
        FabricaSanatio* sanatio;
                chorda  causa;
              Sigillum  sigillum;
             character  hex[SIGILLUM_HEX_MENSURA];
             character* relatio_vetus;
             character* relatio_nova;
                   i32  i;
                   i32  praeparata;

        imprimere("\n--- Probans sanare ---\n");
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;

        /* I. sanatum: X stalum, agere in loco scribit, iudicium post
         * RECENS - et VERE iterum iudicatum (regeneratio iterum
         * currit: memoria regenerationum vacata) */
        _discum_parare(&discus, &sutura, piscina);
        _memorias_parare(&sutura, piscina);
        _ponere(&discus, "a", "a\n");
        _ponere(&discus, "X", "vetus\n");
        _scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n", 0,
            FALSUM);
        actiones[0] = _actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        ordo = _ordinare_fictas(piscina, actiones, I);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(xar_numerus(sanationes), I);
        sanatio = _sanatio_invenire(sanationes, "A");
        CREDO_NON_NIHIL(sanatio);
        CREDO_AEQUALIS_I32((i32)sanatio->eventus, (i32)FABRICA_SANATUM);
        CREDO_VERUM(_contentum_est(&discus, "X", "novum\n"));
        CREDO_AEQUALIS_I32(discus.acta, I);
        CREDO_AEQUALIS_I32(discus.cursus, II);

        /* II. memoria per cursum PURGATA (Review Focus 2): A ingressum
         * suum X legit (praelatio) - sigillum X vetus memoratur; B
         * (binarium, relatio) X legit. Sine purgatione B sigillo VETERE
         * iudicaretur, relationi veteri congrueret, numquam
         * ageretur. */
        _discum_parare(&discus, &sutura, piscina);
        _ponere(&discus, "a", "a\n");
        _ponere(&discus, "X", "vetus\n");
        _scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n", 0,
            FALSUM);
        _scriptum_addere(&discus, "gen_b", "bin/b", NIHIL, "binarium\n",
            0, FALSUM);
        actiones[0] = _actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        _ingressum_addere(actiones[0], "fasciculus", "X", piscina);
        b = _actio_scripta(piscina, "B", "gen_b", "X", "bin/b",
            "relatio");
        actiones[1]  = b;
        nuda         = sutura;   /* sine memoriis: sigilla vera */
        CREDO_VERUM(fabrica_actionem_sigillare(&nuda, b, piscina,
            &sigillum, &causa));
        sigillum_hex(&sigillum, hex);
        relatio_vetus = (character*)piscina_allocare(piscina, 256);
        sprintf(relatio_vetus, "provenientia 1\ningressus %s\n", hex);
        _ponere(&discus, "X", "novum\n");
        CREDO_VERUM(fabrica_actionem_sigillare(&nuda, b, piscina,
            &sigillum, &causa));
        sigillum_hex(&sigillum, hex);
        relatio_nova = (character*)piscina_allocare(piscina, 256);
        sprintf(relatio_nova, "provenientia 1\ningressus %s\n", hex);
        _ponere(&discus, "X", "vetus\n");
        discus.relatio = relatio_vetus;
        ((ScriptumFictum*)xar_obtinere(discus.scripta, I))->relatio =
            relatio_nova;
        _memorias_parare(&sutura, piscina);
        ordo = _ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        sanatio = _sanatio_invenire(sanationes, "B");
        CREDO_NON_NIHIL(sanatio);
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_SANATUM);
        CREDO_AEQUALIS_I32(discus.acta, II);

        /* III. post-condicio (Review Focus 3): exitus 0, nihil
         * scriptum -> FRACTUM, numquam SANATUM */
        _discum_parare(&discus, &sutura, piscina);
        _memorias_parare(&sutura, piscina);
        _ponere(&discus, "a", "a\n");
        _ponere(&discus, "X", "vetus\n");
        _scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n", 0,
            VERUM);
        actiones[0] = _actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        ordo = _ordinare_fictas(piscina, actiones, I);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        sanatio = _sanatio_invenire(sanationes, "A");
        CREDO_NON_NIHIL(sanatio);
        CREDO_AEQUALIS_I32((i32)sanatio->eventus, (i32)FABRICA_FRACTUM);
        CREDO_VERUM(_continet(sanatio->causa, "exitus 0 sed non RECENS",
            piscina));

        /* IV. dependentia fracta (Review Focus 4): A frangitur, C (ex
         * X) OMISSUM nominans A, D independens SANATUM */
        _discum_parare(&discus, &sutura, piscina);
        _memorias_parare(&sutura, piscina);
        _ponere(&discus, "a", "a\n");
        _ponere(&discus, "X", "vetus\n");
        _ponere(&discus, "Y", "C:alienum\n");
        _ponere(&discus, "Z", "vetus\n");
        _scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n", I,
            VERUM);
        _scriptum_addere(&discus, "gen_c", "Y", "X", "C:", 0, FALSUM);
        _scriptum_addere(&discus, "gen_d", "Z", NIHIL, "novum\n", 0,
            FALSUM);
        actiones[0] = _actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        actiones[1] = _actio_scripta(piscina, "C", "gen_c", "X", "Y",
            "regeneratio");
        actiones[2] = _actio_scripta(piscina, "D", "gen_d", "a", "Z",
            "regeneratio");
        ordo = _ordinare_fictas(piscina, actiones, III);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        sanatio = _sanatio_invenire(sanationes, "A");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_FRACTUM
            && _continet(sanatio->causa, "error ficti acti", piscina));
        sanatio = _sanatio_invenire(sanationes, "C");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_OMISSUM
            && _continet(sanatio->causa, "A", piscina));
        sanatio = _sanatio_invenire(sanationes, "D");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_SANATUM);
        CREDO_AEQUALIS_I32(discus.acta, II);
        CREDO_VERUM(_contentum_est(&discus, "Y", "C:alienum\n"));

        /* V. praecondicio ignota SEMEL ante dependentes duas; ordo
         * declarationis (P1, P2, O) - ordinare O primam ponit */
        _discum_parare(&discus, &sutura, piscina);
        _memorias_parare(&sutura, piscina);
        _ponere(&discus, "a", "a\n");
        _ponere(&discus, "p1", "vetus\n");
        _ponere(&discus, "p2", "vetus\n");
        _scriptum_addere(&discus, "gen_o", "build/o", NIHIL,
            "obiecta\n",
            0, FALSUM);
        _scriptum_addere(&discus, "gen_p1", "p1", NIHIL, "novum\n", 0,
            FALSUM);
        _scriptum_addere(&discus, "gen_p2", "p2", NIHIL, "novum\n", 0,
            FALSUM);
        actiones[0] = _actio_scripta(piscina, "P1", "gen_p1", "a", "p1",
            "regeneratio");
        _chordam_addere_test(actiones[0]->praecondiciones, "O",
            piscina);
        actiones[1] = _actio_scripta(piscina, "P2", "gen_p2", "a", "p2",
            "regeneratio");
        _chordam_addere_test(actiones[1]->praecondiciones, "O",
            piscina);
        actiones[2] = _actio_scripta(piscina, "O", "gen_o", "a",
            "build/o", "ignota");
        ordo = _ordinare_fictas(piscina, actiones, III);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        praeparata = ZEPHYRUM;
        per (i = ZEPHYRUM; sanationes != NIHIL
             && i < xar_numerus(sanationes); i++)
        {
            si (((FabricaSanatio*)xar_obtinere(sanationes,
                    i))->eventus == FABRICA_PRAEPARATUM)
            {
                praeparata++;
            }
        }
        CREDO_AEQUALIS_I32(praeparata, I);
        CREDO_VERUM(_contentum_est(&discus, "build/o", "obiecta\n"));
        CREDO_AEQUALIS_I32(discus.acta, III);
        sanatio = _sanatio_invenire(sanationes, "P2");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_SANATUM);

        /* VI. electa: solum Z et quae supra eam - A (X) intacta */
        _discum_parare(&discus, &sutura, piscina);
        _memorias_parare(&sutura, piscina);
        _ponere(&discus, "a", "a\n");
        _ponere(&discus, "X", "vetus\n");
        _ponere(&discus, "Z", "vetus\n");
        _scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n", 0,
            FALSUM);
        _scriptum_addere(&discus, "gen_d", "Z", NIHIL, "novum\n", 0,
            FALSUM);
        actiones[0] = _actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        actiones[1] = _actio_scripta(piscina, "D", "gen_d", "a", "Z",
            "regeneratio");
        ordo = _ordinare_fictas(piscina, actiones, II);
        electa = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis("Z", piscina);
        sanationes = fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(xar_numerus(sanationes), I);
        CREDO_NIHIL(_sanatio_invenire(sanationes, "A"));
        CREDO_VERUM(_contentum_est(&discus, "X", "vetus\n"));
        CREDO_AEQUALIS_I32(discus.acta, I);

        /* VII. siccum: A AGENDUM, C (ex X, nunc recens) FORTASSE;
         * nihil agitur */
        _discum_parare(&discus, &sutura, piscina);
        _memorias_parare(&sutura, piscina);
        _ponere(&discus, "a", "a\n");
        _ponere(&discus, "X", "vetus\n");
        _ponere(&discus, "Y", "C:vetus\n");
        _scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n", 0,
            FALSUM);
        _scriptum_addere(&discus, "gen_c", "Y", "X", "C:", 0, FALSUM);
        actiones[0] = _actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        actiones[1] = _actio_scripta(piscina, "C", "gen_c", "X", "Y",
            "regeneratio");
        ordo = _ordinare_fictas(piscina, actiones, II);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, VERUM,
            piscina, &causa);
        sanatio = _sanatio_invenire(sanationes, "A");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_AGENDUM);
        sanatio = _sanatio_invenire(sanationes, "C");
        CREDO_VERUM(sanatio != NIHIL
            && sanatio->eventus == FABRICA_FORTASSE);
        CREDO_AEQUALIS_I32(discus.acta, ZEPHYRUM);
        CREDO_VERUM(_contentum_est(&discus, "X", "vetus\n"));

        /* VIII. nihil stalum: Xar vacua (non NIHIL), nihil actum */
        _discum_parare(&discus, &sutura, piscina);
        _memorias_parare(&sutura, piscina);
        _ponere(&discus, "a", "a\n");
        _ponere(&discus, "X", "novum\n");
        _scriptum_addere(&discus, "gen_a", "X", NIHIL, "novum\n", 0,
            FALSUM);
        actiones[0] = _actio_scripta(piscina, "A", "gen_a", "a", "X",
            "regeneratio");
        ordo = _ordinare_fictas(piscina, actiones, I);
        sanationes = fabrica_sanare(&sutura, ordo, NIHIL, FALSUM,
            piscina, &causa);
        CREDO_NON_NIHIL(sanationes);
        CREDO_AEQUALIS_I32(xar_numerus(sanationes), ZEPHYRUM);
        CREDO_AEQUALIS_I32(discus.acta, ZEPHYRUM);

        /* IX. electa a nulla actione producta -> NIHIL nominatum */
        electa = xar_creare(piscina, (i32)magnitudo(chorda));
        *(chorda*)xar_addere(electa) = chorda_ex_literis("nusquam",
            piscina);
        CREDO_NIHIL(fabrica_sanare(&sutura, ordo, electa, FALSUM,
            piscina, &causa));
        CREDO_VERUM(_continet(causa, "nusquam", piscina));
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
