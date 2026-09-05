/* pictor.c - pictor: editor rasterum (P3: tabula + penicillus)
 *
 * Catena: volumen -> documentum; canones + domini e disco; insulae;
 * registra (actiones, figurae); dispensator (componere pictoris);
 * fenestra; ludus_fenestra_currere. -fumus: volumen temporarium,
 * XXX quadra, exitus.
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "thema.h"
#include "volumen.h"
#include "fenestra.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_actiones.h"
#include "pictor_figurae.h"
#include <stdio.h>
#include <string.h>

#define PICTOR_LATITUDO   (DC + XL)
#define PICTOR_ALTITUDO   CDLXXX
#define STATUS_ALTITUDO   XII
#define DOC_LATITUDO      CCCXX
#define DOC_ALTITUDO      CC
#define QUADRA_FUMI       XXX

interior Canon*
canonem_legere (
                Piscina* p,
    InternamentumChorda* in,
     constans character* via)
{
    chorda  fons;
    chorda  causa;
     Canon* c;

    fons = filum_legere_totum(via, p);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "pictor: canon abest: %s\n", via);
        redde NIHIL;
    }
    c = canon_legere(fons, p, in, &causa);
    si (!c)
    {
        fprintf(stderr, "pictor: canon malus %s: %.*s\n", via,
                (int)causa.mensura, causa.datum);
    }
    redde c;
}

s32
principale (
      integer   argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorDocumentum* doc;
     InsulaRepositorium* repo;
         ActioRegistrum* actiones;
         PictorActiones  actiones_ctx;
        FiguraRegistrum* figurae;
          PictorFigurae  figurae_ctx;
       PictorCompositio  compositio;
            Dispensator* d;
               Fenestra* fenestra;
        TabulaPixelorum* tabula;
          LudusFenestra* lf;
   FenestraConfiguratio  cfg;
                 chorda  domini;
           StmlResultus  res;
                    b32  fumus;
     constans character* via_voluminis;
                    s32  i;
                    s32  exitus;
                 chorda  ephemera_initialis;
                 chorda  durabilis_initialis;

    fumus          = FALSUM;
    via_voluminis  = NIHIL;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-fumus") == ZEPHYRUM)
        {
            fumus = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-volumen") == ZEPHYRUM
                     && i + I < argc)
        {
            i++;
            via_voluminis = argv[i];
        }
    }

    piscina = piscina_generare_dynamicum("pictor", IV * M * M);
    si (!piscina)
    {
        redde I;
    }
    intern = internamentum_creare(piscina);
    thema_initiare();

    /* documentum */
    si (fumus || !via_voluminis)
    {
        vol = fumus ? volumen_temporarium(piscina, "pictor_fumus")
                    : volumen_aperire_aut_creare(piscina,
                                                 "pictor.volumen");
    }
    alioquin
    {
        vol = volumen_aperire_aut_creare(piscina, via_voluminis);
    }
    si (!vol)
    {
        fprintf(stderr, "pictor: volumen aperiri non potuit\n");
        redde I;
    }
    doc = pictor_documentum_aperire(piscina, intern, vol);
    si (!doc)
    {
        doc = pictor_documentum_creare(piscina, intern, vol,
            DOC_LATITUDO,
                                       DOC_ALTITUDO, LXIV);
    }
    si (!doc)
    {
        fprintf(stderr, "pictor: documentum\n");
        redde I;
    }

    /* insulae + canones + domini */
    durabilis_initialis = chorda_ex_literis("<documentum latitudo=\"",
        piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_s32((s32)doc->latitudo, piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_s32((s32)doc->altitudo, piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_literis("\"/>", piscina), piscina);
    ephemera_initialis = chorda_ex_literis(
        "<ephemera instrumentum=\"penicillus\" color_primus=\"0\""
        " color_secundus=\"5\" magnitudo=\"1\" zoom=\"1\""
        " focus=\"tabula\"/>", piscina);
    repo = insula_repositorium_creare(piscina, intern,
        chorda_ut_cstr(durabilis_initialis, piscina),
        chorda_ut_cstr(ephemera_initialis, piscina));
    si (!repo)
    {
        redde I;
    }
    insula_ponere_canonem(repo, INSULA_DURABILIS,
        canonem_legere(piscina, intern,
        "apps/pictor/canones/durabilis.canon"));
    insula_ponere_canonem(repo, INSULA_EPHEMERA,
        canonem_legere(piscina, intern,
        "apps/pictor/canones/ephemera.canon"));
    domini = filum_legere_totum("apps/pictor/canones/domini.stml",
        piscina);
    res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina),
        piscina,
                                 intern);
    si (res.successus)
    {
        insula_dominos_legere(repo, INSULA_EPHEMERA,
            res.elementum_radix);
        insula_dominos_legere(repo, INSULA_DURABILIS,
            res.elementum_radix);
    }

    /* registra + dispensator */
    actiones          = actio_registrum_creare(piscina, intern);
    actiones_ctx.doc  = doc;
    pictor_actiones_registrare(actiones, &actiones_ctx);
    figurae          = figura_registrum_creare(piscina);
    figurae_ctx.doc  = doc;
    pictor_figurae_registrare(figurae, ZEPHYRUM, &figurae_ctx);
    compositio.fenestra_latitudo  = PICTOR_LATITUDO;
    compositio.fenestra_altitudo  = PICTOR_ALTITUDO;
    compositio.status_altitudo    = STATUS_ALTITUDO;
    d = dispensator_creare(piscina, intern, repo, actiones,
        pictor_componere,
                           &compositio, CCC);
    si (!d)
    {
        redde I;
    }

    /* fenestra */
    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.titulus   = "pictor";
    cfg.x         = C;
    cfg.y         = C;
    cfg.latitudo  = PICTOR_LATITUDO;
    cfg.altitudo  = PICTOR_ALTITUDO;
    cfg.vexilla   = FENESTRA_ORDINARIA;
    fenestra      = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "pictor: fenestra\n");
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               PICTOR_ALTITUDO);
    lf = ludus_fenestra_creare(piscina, d, figurae, ZEPHYRUM,
                               pictor_imago_fons, &figurae_ctx, tabula);
    si (!tabula || !lf)
    {
        redde I;
    }
    exitus = ludus_fenestra_currere(lf, fenestra,
                                    fumus ? QUADRA_FUMI : ZEPHYRUM);
    fenestra_destruere(fenestra);
    volumen_claudere(vol);
    piscina_destruere(piscina);
    redde exitus;
}
