/* fabrica_mundus_fictus.c - MUNDUS FICTUS probationum fabricae
 * (fabrica-6 H3): vide fabrica_mundus_fictus.h. Machina super
 * suturam in memoria agitur: tabula plagularum, tabula
 * directoriorum, generator scriptus (scribit, frangitur, aut mutus
 * manet - directorium scripturae ante cursum VACUAT, ut sutura vera
 * debet), relatio '-provenientia' scripta. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "sigillum.h"
#include "internamentum.h"
#include "fabrica.h"
#include "fabrica_mundus_fictus.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Discus in memoria (sutura ficta)
 * ================================================== */

FasciculusFictus*
mundi_fasciculum_invenire (
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

vacuum
mundi_ponere (
                DiscusFictus* discus,
          constans character* via,
          constans character* contentum)
{
    FasciculusFictus* f;

    f = mundi_fasciculum_invenire(discus, via);
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
    f->contentum  = chorda_ex_literis(contentum, discus->piscina);
    f->tempus     = ++discus->horologium;
}

vacuum
mundi_auferre (
          DiscusFictus* discus,
    constans character* via)
{
    FasciculusFictus* f;

    f = mundi_fasciculum_invenire(discus, via);
    si (f != NIHIL)
    {
        f->deletum = VERUM;
    }
}

vacuum
mundi_directorium_ponere (
                DiscusFictus* discus,
          constans character* via,
          constans character* nomina[],
                         i32  numerus)
{
    DirectoriumFictum* d = NIHIL;
                  i32  i;

    /* via eadem iterum = enumeratio NOVA (ut directorium verum), non
     * recordum alterum quod quaestio prima numquam invenit (chassis
     * fabrica-6 T1: directorium auctum aliter invisibile) */
    per (i = ZEPHYRUM; i < xar_numerus(discus->directoria); i++)
    {
        DirectoriumFictum* e = (DirectoriumFictum*)xar_obtinere(
            discus->directoria, i);

        si (chorda_aequalis_literis(e->via, via))
        {
            d = e;
        }
    }
    si (d == NIHIL)
    {
        d = (DirectoriumFictum*)xar_addere(discus->directoria);
    }
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

b32
mundi_legere (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* contentum_out)
{
    FasciculusFictus* f;

    (vacuum)piscina;
    ((DiscusFictus*)datum)->lecturae++;
    f = mundi_fasciculum_invenire((DiscusFictus*)datum, via);
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    *contentum_out = f->contentum;
    redde VERUM;
}

b32
mundi_enumerare (
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
ScriptumFictum*
mundi_scriptum_invenire (
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
constans character*
mundi_generare (
      DiscusFictus* discus,
    ScriptumFictum* scriptum)
{
     FasciculusFictus* fons;
            character* exitus;
       memoriae_index  mensura;

    fons = (scriptum->fons != NIHIL)
        ? mundi_fasciculum_invenire(discus, scriptum->fons) : NIHIL;
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

b32
mundi_currere (
                vacuum* datum,
          constans Xar* mandatum,
    constans character* scriptura_dir,
    constans character* liber_via,
               Piscina* piscina,
                chorda* causa_out,
                   i32* duratio_ms_out)
{
    DiscusFictus* discus;
          chorda  praefixum;
             i32  i;
             i32  numerus;

    discus = (DiscusFictus*)datum;
    discus->cursus++;
    /* liber lectionum: generator fictus 'legit' quod scriptum est */
    si (liber_via != NIHIL && discus->lectiones_ficti != NIHIL)
    {
        mundi_ponere(discus, liber_via, discus->lectiones_ficti);
    }
    *duratio_ms_out = VII;   /* tempus fictum regenerationis */
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

        scriptum = mundi_scriptum_invenire(discus, mandatum);
        si (scriptum != NIHIL)
        {
            character via[256];

            sprintf(via, "%s/%s", scriptura_dir, scriptum->via);
            mundi_ponere(discus, via, mundi_generare(discus, scriptum));
            redde VERUM;
        }
    }
    si (discus->generatio != NIHIL)
    {
        character via[256];

        sprintf(via, "%s/%s", scriptura_dir, discus->scriptura_rel);
        mundi_ponere(discus, via, discus->generatio);
    }
    si (discus->generatio_altera != NIHIL)
    {
        character via[256];

        sprintf(via, "%s/%s", scriptura_dir, discus->scriptura_altera);
        mundi_ponere(discus, via, discus->generatio_altera);
    }
    redde VERUM;
}

b32
mundi_agere (
                   vacuum* datum,
    constans FabricaActio* actio,
       constans character* acta_via,
                  Piscina* piscina,
             FabricaActum* actum_out)
{
      DiscusFictus* discus;
    ScriptumFictum* scriptum;

    discus = (DiscusFictus*)datum;
    discus->acta++;
    actum_out->duratio_ms = I;
    actum_out->cauda = chorda_ex_literis("", piscina);
    scriptum = mundi_scriptum_invenire(discus, actio->mandatum);
    si (scriptum == NIHIL)
    {
        actum_out->codex = -I;
        actum_out->cauda = chorda_ex_literis("scriptum nullum",
            piscina);
        redde FALSUM;
    }
    si (!scriptum->mutus)
    {
        mundi_ponere(discus, scriptum->via, mundi_generare(discus,
            scriptum));
    }
    si (scriptum->alia != NIHIL)
    {
        mundi_ponere(discus, scriptum->alia, "alia\n");
    }
    si (scriptum->effusio != NIHIL)
    {
        mundi_ponere(discus, acta_via, scriptum->effusio);
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

/* verdictum ponere fictum (plan 5 T3): contentum NIHIL = deletum */
b32
mundi_verdictum_ponere (
                vacuum* datum,
    constans character* via,
       constans chorda* contentum)
{
    DiscusFictus* discus = (DiscusFictus*)datum;

    si (contentum == NIHIL)
    {
        mundi_auferre(discus, via);
        redde VERUM;
    }
    mundi_ponere(discus, via, chorda_ut_cstr(*contentum,
        discus->piscina));
    redde VERUM;
}

vacuum
mundi_cursum_inscribere (
                     vacuum* datum,
    constans FabricaSanatio* sanatio)
{
    DiscusFictus* discus;
    CursusFictus* cursus;

    discus = (DiscusFictus*)datum;
    cursus = (CursusFictus*)xar_addere(discus->cursus_ficti);
    si (cursus != NIHIL)
    {
        cursus->titulus     = sanatio->actio->titulus;
        cursus->duratio_ms  = sanatio->duratio_ms;
        cursus->eventus     = sanatio->eventus;
        cursus->stalum      = sanatio->stalum;
    }
}

/* duratio cursus ULTIMI SANATI aut PRAEPARATI tituli (ut SQL verum) */
b32
mundi_cursum_legere (
                vacuum* datum,
    constans character* titulus,
                   i32* duratio_ms_out)
{
    DiscusFictus* discus;
             b32  inventum;
             i32  i;

    discus    = (DiscusFictus*)datum;
    inventum  = FALSUM;
    per (i = ZEPHYRUM; i < xar_numerus(discus->cursus_ficti); i++)
    {
        CursusFictus* cursus;

        cursus = (CursusFictus*)xar_obtinere(discus->cursus_ficti, i);
        si (   chorda_aequalis_literis(cursus->titulus, titulus)
            && (   cursus->eventus == FABRICA_SANATUM
                || cursus->eventus == FABRICA_PRAEPARATUM))
        {
            *duratio_ms_out  = cursus->duratio_ms;
            inventum         = VERUM;
        }
    }
    redde inventum;
}

s32
mundi_vestigia_comparare_via (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(((constans FabricaVestigium*)a)->via,
        ((constans FabricaVestigium*)b)->via);
}

/* currere SIMUL fictum (T6b, praevisio): mundi_currere fictum per
 * membrum, unda nominata (tituli spatiis) in undae_actae */
vacuum
mundi_currere_simul (
                            vacuum*  datum,
    constans FabricaActio* constans* actiones,
                               i32  numerus,
       constans character* constans* scripturae,
       constans character* constans* libri,
                           Piscina*  piscina,
                               b32*  felices_out,
                            chorda*  causae_out,
                               i32*  durationes_out)
{
     DiscusFictus* discus;
           chorda  unda;
              i32  k;

    discus  = (DiscusFictus*)datum;
    unda    = chorda_ex_literis("", piscina);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        causae_out[k] = chorda_ex_literis("", piscina);
        felices_out[k] = mundi_currere(datum, actiones[k]->mandatum,
            scripturae[k], libri[k], piscina, &causae_out[k],
            &durationes_out[k]);
        unda = chorda_concatenare(unda, (k > ZEPHYRUM)
            ? chorda_ex_literis(" ", piscina)
            : chorda_ex_literis("", piscina), piscina);
        unda = chorda_concatenare(unda, actiones[k]->titulus, piscina);
    }
    *(chorda*)xar_addere(discus->undae_currendi) = unda;
}

/* agere SIMUL fictum (plan 2 T6): membra ordine dato 'incipiunt';
 * post fracturam membra ab indice fila_ficta non incipiunt. Unda
 * acta notatur (tituli spatiis). scriptum_s: plagula scripta et 'S' in
 * libro membri. */
vacuum
mundi_agere_simul (
                            vacuum*  datum,
    constans FabricaActio* constans* actiones,
                               i32  numerus,
       constans character* constans* acta_viae,
       constans character* constans* libri,
                           Piscina*  piscina,
                      FabricaActum*  acta_out,
                               b32*  incepta_out)
{
     DiscusFictus* discus;
           chorda  unda;
              i32  k;
              b32  fractum;

    discus   = (DiscusFictus*)datum;
    unda     = chorda_ex_literis("", piscina);
    fractum  = FALSUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        ScriptumFictum* scriptum;

        si (fractum && k >= discus->fila_ficta)
        {
            incepta_out[k]          = FALSUM;
            acta_out[k].codex       = -I;
            acta_out[k].duratio_ms  = ZEPHYRUM;
            acta_out[k].cauda       = chorda_ex_literis("", piscina);
            perge;
        }
        incepta_out[k] = mundi_agere(datum, actiones[k], acta_viae[k],
            piscina,
            &acta_out[k]);
        discus->acta_simul++;
        scriptum = mundi_scriptum_invenire(discus,
            actiones[k]->mandatum);
        si (scriptum != NIHIL && scriptum->scriptum_s != NIHIL)
        {
            character linea[CCLVI];

            mundi_ponere(discus, scriptum->scriptum_s, "s\n");
            sprintf(linea, "S\t%s\n", scriptum->scriptum_s);
            mundi_ponere(discus, libri[k], linea);
        }
        si (acta_out[k].codex != ZEPHYRUM)
        {
            fractum = VERUM;
        }
        unda = chorda_concatenare(unda, (k > ZEPHYRUM)
            ? chorda_ex_literis(" ", piscina)
            : chorda_ex_literis("", piscina), piscina);
        unda = chorda_concatenare(unda, actiones[k]->titulus, piscina);
    }
    *(chorda*)xar_addere(discus->undae_actae) = unda;
}

b32
mundi_vestigium_capere (
     vacuum*  datum,
    Piscina*  piscina,
        Xar** vestigia_out)
{
    DiscusFictus* discus;
             Xar* vestigia;
             i32  i;

    discus    = (DiscusFictus*)datum;
    vestigia  = xar_creare(piscina, (i32)magnitudo(FabricaVestigium));
    per (i = ZEPHYRUM; i < xar_numerus(discus->fasciculi); i++)
    {
        FasciculusFictus* f;
        FabricaVestigium* v;

        f = (FasciculusFictus*)xar_obtinere(discus->fasciculi, i);
        si (f->deletum)
        {
            perge;
        }
        v             = (FabricaVestigium*)xar_addere(vestigia);
        v->via        = f->via;
        v->tempus_ns  = f->tempus;
        v->mensura    = (s64)f->contentum.mensura;
    }
    xar_ordinare(vestigia, mundi_vestigia_comparare_via);
    *vestigia_out = vestigia;
    redde VERUM;
}

b32
mundi_rogare (
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

b32
mundi_lectiones_legere (
                vacuum*  datum,
    constans character*  titulus,
    constans character*  exitus,
     constans Sigillum*  ingressus,
     constans Sigillum*  artificium,
               Piscina*  piscina,
                   Xar** lectiones_out)
{
    DiscusFictus* discus;
             i32  i;

    (vacuum)piscina;
    discus = (DiscusFictus*)datum;
    per (i = xar_numerus(discus->vestigia_lectionum); i > ZEPHYRUM; i--)
    {
        VestigiumLectionumFictum* v;

        v = (VestigiumLectionumFictum*)xar_obtinere(
            discus->vestigia_lectionum, i - I);
        si (   chorda_aequalis_literis(v->titulus, titulus)
            && chorda_aequalis_literis(v->exitus, exitus)
            && memcmp(v->ingressus.octeti, ingressus->octeti,
                   SIGILLUM_OCTETI) == 0
            && memcmp(v->artificium.octeti, artificium->octeti,
                   SIGILLUM_OCTETI) == 0)
        {
            /* ut tabula vera: ordines nulli = vestigium nullum (scriptura
             * vacua = deletio, spec 3 T5b) */
            si (xar_numerus(v->lectiones) == 0)
            {
                redde FALSUM;
            }
            *lectiones_out = v->lectiones;
            redde VERUM;
        }
    }
    redde FALSUM;
}

vacuum
mundi_lectiones_scribere (
                vacuum* datum,
    constans character* titulus,
    constans character* exitus,
     constans Sigillum* ingressus,
     constans Sigillum* artificium,
          constans Xar* lectiones)
{
                DiscusFictus* discus;
    VestigiumLectionumFictum* v;
                         i32  i;

    discus = (DiscusFictus*)datum;
    discus->vestigia_scripta++;
    /* ut tabula vera: vestigium prius (titulus, exitus) deletum */
    per (i = ZEPHYRUM; i < xar_numerus(discus->vestigia_lectionum); i++)
    {
        VestigiumLectionumFictum* vetus;

        vetus = (VestigiumLectionumFictum*)xar_obtinere(
            discus->vestigia_lectionum, i);
        si (   chorda_aequalis_literis(vetus->titulus, titulus)
            && chorda_aequalis_literis(vetus->exitus, exitus))
        {
            vetus->titulus = chorda_ex_literis("", discus->piscina);
        }
    }
    v =
        (VestigiumLectionumFictum*)xar_addere(discus->vestigia_lectionum);
    si (v == NIHIL)
    {
        redde;
    }
    v->titulus     = chorda_ex_literis(titulus, discus->piscina);
    v->exitus      = chorda_ex_literis(exitus, discus->piscina);
    v->ingressus   = *ingressus;
    v->artificium  = *artificium;
    v->lectiones   = xar_creare(discus->piscina,
        (i32)magnitudo(FabricaLectio));
    per (i = ZEPHYRUM; i < xar_numerus(lectiones); i++)
    {
        *(FabricaLectio*)xar_addere(v->lectiones) =
            *(constans FabricaLectio*)xar_obtinere(lectiones, i);
    }
}

vacuum
mundi_discum_parare (
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
    discus->acta        = ZEPHYRUM;
    discus->horologium  = ZEPHYRUM;
    discus->cursus_ficti = xar_creare(piscina,
        (i32)magnitudo(CursusFictus));
    discus->lectiones_ficti = NIHIL;
    discus->undae_actae = xar_creare(piscina, (i32)magnitudo(chorda));
    discus->particulae_servatae = xar_creare(piscina,
        (i32)magnitudo(ParticulaeFictae));
    discus->acta_simul = ZEPHYRUM;
    discus->undae_currendi = xar_creare(piscina,
        (i32)magnitudo(chorda));
    discus->fila_ficta        = C;
    discus->scriptura_altera  = NIHIL;
    discus->generatio_altera  = NIHIL;
    discus->vestigia_lectionum  = xar_creare(piscina,
        (i32)magnitudo(VestigiumLectionumFictum));
    discus->vestigia_scripta    = ZEPHYRUM;

    fabrica_suturam_parare(sutura);

    sutura->datum              = discus;
    sutura->legere             = mundi_legere;
    sutura->enumerare          = mundi_enumerare;
    sutura->currere            = mundi_currere;
    sutura->rogare             = mundi_rogare;
    sutura->agere              = mundi_agere;
    sutura->vestigium_capere   = mundi_vestigium_capere;
    sutura->cursum_inscribere  = mundi_cursum_inscribere;
    sutura->cursum_legere      = mundi_cursum_legere;
    sutura->meminisse          = NIHIL;
    sutura->sigilla            = NIHIL;
}


/* ==================================================
 * Actiones fictae
 * ================================================== */

FabricaActio*
mundi_actio (
                 Piscina* piscina,
      constans character* titulus,
    FabricaGenusActionis  genus)
{
    FabricaActio* actio;
          chorda* verbum;

    actio = (FabricaActio*)piscina_allocare(piscina,
        magnitudo(FabricaActio));
    memset(actio, ZEPHYRUM, magnitudo(FabricaActio));
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
    actio->lectiones    = FALSUM;
    actio->celer        = FALSUM;
    actio->praecondiciones = xar_creare(piscina,
        (i32)magnitudo(chorda));
    actio->vestigia = xar_creare(piscina,
        (i32)magnitudo(FabricaLocus));
    actio->communia = xar_creare(piscina,
        (i32)magnitudo(FabricaLocus));
    actio->dependentiae = NIHIL;
    redde actio;
}

constans FabricaGenus*
mundi_genus (
    constans character* titulus,
               Piscina* piscina)
{
    redde fabrica_genus_invenire(chorda_ex_literis(titulus, piscina));
}

constans FabricaStrategia*
mundi_strategia (
    constans character* titulus,
               Piscina* piscina)
{
    redde fabrica_strategia_invenire(chorda_ex_literis(titulus,
        piscina));
}

vacuum
mundi_ingressum_addere (
           FabricaActio* actio,
     constans character* genus,
     constans character* via,
                Piscina* piscina)
{
    FabricaIngressus* ingressus;

    ingressus = (FabricaIngressus*)xar_addere(actio->ingressus);
    ingressus->genus = mundi_genus(genus, piscina);
    ingressus->via = chorda_ex_literis(via, piscina);
    ingressus->suffixa = chorda_ex_literis("", piscina);
}

/* genus exitus = genus ordinarium strategiae (ut lector facit) */
FabricaExitus*
mundi_exitum_addere (
           FabricaActio* actio,
     constans character* via,
     constans character* strategia,
                Piscina* piscina)
{
    FabricaExitus* exitus;

    exitus             = (FabricaExitus*)xar_addere(actio->exitus);
    exitus->via        = chorda_ex_literis(via, piscina);
    exitus->scriptura  = chorda_ex_literis("gen/exitus.c", piscina);
    exitus->strategia  = mundi_strategia(strategia, piscina);
    exitus->genus      = (exitus->strategia != NIHIL)
        ? mundi_genus(exitus->strategia->genus_ordinarium,
        piscina) : NIHIL;
    redde exitus;
}

b32
mundi_sigillum (
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

b32
mundi_continet (
                chorda  fenum,
    constans character* acus,
               Piscina* piscina)
{
    redde chorda_continet(fenum, chorda_ex_literis(acus, piscina));
}


/* ==================================================
 * Sanare: auxilia (plan 1b T3)
 * ================================================== */

vacuum
mundi_scriptum_addere (
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
    scriptum->verbum      = verbum;
    scriptum->via         = via;
    scriptum->fons        = fons;
    scriptum->praefixum   = praefixum;
    scriptum->codex       = codex;
    scriptum->mutus       = mutus;
    scriptum->relatio     = NIHIL;
    scriptum->alia        = NIHIL;
    scriptum->scriptum_s  = NIHIL;
    scriptum->effusio     = NIHIL;
}

/* actio cum generatore scripto, ingressu uno, exitu uno (scriptura =
 * via: regeneratio in scriptura/VIA cadit) */
FabricaActio*
mundi_actio_scripta (
               Piscina* piscina,
    constans character* titulus,
    constans character* verbum,
    constans character* ingressus,
    constans character* exitus,
    constans character* strategia)
{
     FabricaActio* actio;
    FabricaExitus* e;

    actio = mundi_actio(piscina, titulus, FABRICA_ACTIO_GENERATOR);
    *(chorda*)xar_obtinere(actio->mandatum, ZEPHYRUM) =
        chorda_ex_literis(verbum, piscina);
    mundi_ingressum_addere(actio, "fasciculus", ingressus, piscina);
    e = mundi_exitum_addere(actio, exitus, strategia,
        piscina);
    e->scriptura = e->via;
    redde actio;
}

Xar*
mundi_ordinare_fictas (
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

FabricaSanatio*
mundi_sanatio_invenire (
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
/* sutura iudicii ficta (fabrica spec 3 T5b): ambitus portae (paria
 * titulus, valor; NIHIL finit), via ALIA (FIFO), identitas clang,
 * effectus cursoris */
constans character* mundi_ambitus_ficti[VIII] = {
    NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL
};

constans character* mundi_via_alia_ficta       = NIHIL;

constans character* mundi_identitas_ficta      = "clang I";

constans character* mundi_effectus_effusio     = "";

constans character* mundi_via_absens_ficta     = NIHIL;

b32
mundi_ambitus_fictum (
                vacuum* datum,
    constans character* titulus,
               Piscina* piscina,
                chorda* valor_out)
{
    i32 k;

    (vacuum)datum;
    per (k = ZEPHYRUM; k + I < VIII
        && mundi_ambitus_ficti[k] != NIHIL; k += II)
    {
        si (strcmp(mundi_ambitus_ficti[k], titulus) == ZEPHYRUM)
        {
            *valor_out = chorda_ex_literis(mundi_ambitus_ficti[k + I],
                piscina);
            redde VERUM;
        }
    }
    redde FALSUM;
}

i32
mundi_species_ficta (
                vacuum* datum,
    constans character* via)
{
    (vacuum)datum;
    si (   mundi_via_alia_ficta              != NIHIL
        && strcmp(via, mundi_via_alia_ficta) == ZEPHYRUM)
    {
        redde (i32)FABRICA_SPECIES_ALIA;
    }
    si (   mundi_via_absens_ficta              != NIHIL
        && strcmp(via, mundi_via_absens_ficta) == ZEPHYRUM)
    {
        redde (i32)FABRICA_SPECIES_ABSENS;
    }
    redde (i32)FABRICA_SPECIES_PLAGULA;
}

b32
mundi_identitatem_fictam_dare (
     vacuum* datum,
    Piscina* piscina,
   Sigillum* identitas_out,
     chorda* causa_out)
{
    (vacuum)datum;
    (vacuum)piscina;
    (vacuum)causa_out;
    *identitas_out = sigillum_computare(mundi_identitas_ficta,
        strlen(mundi_identitas_ficta));
    redde VERUM;
}

/* effectus scripti ficti (effectus-plan T7): lineae '-clavis' */
b32
mundi_effectus_ficti (
                vacuum* datum,
    constans character* via,
               Piscina* piscina,
                chorda* effusio_out,
                   i32* codex_out)
{
    (vacuum)datum;
    (vacuum)via;
    *effusio_out  = chorda_ex_literis(mundi_effectus_effusio, piscina);
    *codex_out    = ZEPHYRUM;
    redde VERUM;
}

vacuum
mundi_memorias_parare (
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

b32
mundi_contentum_est (
          DiscusFictus* discus,
    constans character* via,
    constans character* contentum)
{
    FasciculusFictus* f;

    f = mundi_fasciculum_invenire(discus, via);
    redde f != NIHIL
        && chorda_aequalis_literis(f->contentum, contentum);
}
