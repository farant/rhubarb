/* vicus_applicatio.c - compositio communis hospitis (T4) */

#include "vicus_applicatio.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "terminale.h"
#include <stdio.h>
#include <string.h>

/* dispositio ordinaria (vicus-latera decisio IX, Franus 2026-10-08):
 * decem tabulae - I scriba | terminale, II scriba | pictor, III-X
 * scriba | scriba */
#define TABULA_ORDINARIA(id, dextrum)                               \
    "<tabula id=\"" id "\" focus=\"sinistrum\">"                    \
    "<latus genus=\"scriba\"/>"                                     \
    "<acervus><latus genus=\"" dextrum "\"/></acervus></tabula>"

#define INDEX_ORDINARIUS                                            \
    "<tabulae activa=\"1\">"                                        \
    TABULA_ORDINARIA("1", "terminale")                              \
    TABULA_ORDINARIA("2", "pictor")                                 \
    TABULA_ORDINARIA("3", "scriba")                                 \
    TABULA_ORDINARIA("4", "scriba")                                 \
    TABULA_ORDINARIA("5", "scriba")                                 \
    TABULA_ORDINARIA("6", "scriba")                                 \
    TABULA_ORDINARIA("7", "scriba")                                 \
    TABULA_ORDINARIA("8", "scriba")                                 \
    TABULA_ORDINARIA("9", "scriba")                                 \
    TABULA_ORDINARIA("10", "scriba")                                \
    "</tabulae>"


/* ==================================================
 * Genera (VicusMontator, VicusDescriptor)
 * ================================================== */

interior b32
scribam_montare (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    redde scriba_montare((ScribaMontatio*)sedes, piscina, intern,
        volumen, repo, id, radix, latitudo, altitudo);
}

interior vacuum
scribae_gestum (
     Motus* motus,
    vacuum* ctx)
{
    scriba_gestum_ponere(motus, (ScribaActiones*)ctx);
}

interior vacuum
scribam_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    ScribaMontatio* m;

    m                 = (ScribaMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = scriba_componere;
    f->componere_ctx  = &m->compositio;
    f->gestum_ponere  = scribae_gestum;
    f->gestum_ctx     = &m->actiones_ctx;
}

interior b32
pictorem_montare (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    redde pictor_montare((PictorMontatio*)sedes, piscina, intern,
        volumen, repo, id, radix, latitudo, altitudo);
}

interior vacuum
pictorem_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    PictorMontatio* m;

    m                 = (PictorMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = pictor_componere;
    f->componere_ctx  = &m->compositio;
    f->fons           = pictor_imago_fons;
    f->fons_ctx       = &m->figurae_ctx;
}

/* terminale (vicus-latera S1c): concha nova in omni apertura (decisio
 * aemulatoris: nihil durabile praeter ramum); volumen et radix
 * viarum non leguntur - canones infixi */
interior b32
terminale_montare_in_vico (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    (vacuum)volumen;
    (vacuum)radix;
    redde terminale_montare((TerminaleApplicatio*)sedes, piscina,
        intern, repo, id, latitudo, altitudo);
}

/* pulsus sine mora: ansa hospitis XVI ms ipsa exspectat */
interior VicusPulsus
terminale_pulsare_in_vico (
    vacuum* ctx)
{
    AemulatorHospesPulsus ph;
              VicusPulsus p;

    ph         = terminale_pulsare((TerminaleApplicatio*)ctx, ZEPHYRUM);
    p.mutatum  = ph.mutatum;
    p.finitus  = ph.finitus;
    redde p;
}

interior vacuum
terminale_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    TerminaleApplicatio* m;

    m                  = (TerminaleApplicatio*)montatio;
    f->actiones        = m->actiones;
    f->figurae         = m->figurae;
    f->componere       = terminale_componere;
    f->componere_ctx   = m;
    f->pulsare         = terminale_pulsare_in_vico;
    f->pulsare_ctx     = m;
    f->vivit_in_fundo  = VERUM;
}


/* ==================================================
 * Applicatio
 * ================================================== */

Volumen*
vicus_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus)
{
    constans character* via_voluminis = NIHIL;
                   s32  i;

    *fumus = FALSUM;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-fumus") == ZEPHYRUM)
        {
            *fumus = VERUM;
        }
        alioquin si (   strcmp(argv[i], "-volumen") == ZEPHYRUM
                     && i + I < argc)
        {
            i++;
            via_voluminis = argv[i];
        }
    }
    si (*fumus)
    {
        redde volumen_temporarium(piscina, "vicus_fumus");
    }
    redde volumen_aperire_aut_creare(piscina,
        via_voluminis ? via_voluminis : "vicus.volumen");
}

b32
vicus_applicatio_aedificare (
        VicusApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    chorda causa;

    si (!app || !piscina || !intern || !volumen)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(VicusApplicatio));
    app->piscina  = piscina;
    app->intern   = intern;
    app->volumen  = volumen;
    app->vicus    = vicus_creare(piscina, intern, volumen, radix,
        latitudo, altitudo);
    si (   !app->vicus
        || !vicus_genus_addere(app->vicus, "scriba",
               magnitudo(ScribaMontatio), scribam_montare,
               scribam_describere)
        || !vicus_genus_addere(app->vicus, "pictor",
               magnitudo(PictorMontatio), pictorem_montare,
               pictorem_describere)
        || !vicus_genus_addere(app->vicus, "terminale",
               magnitudo(TerminaleApplicatio),
               terminale_montare_in_vico, terminale_describere)
        || !vicus_aperire(app->vicus, INDEX_ORDINARIUS))
    {
        causa = app->vicus ? vicus_causa(app->vicus)
                           : chorda_ex_literis("vicus_creare", piscina);
        fprintf(stderr, "vicus: aperiri non potuit: %.*s\n",
            (int)causa.mensura, causa.datum);
        redde FALSUM;
    }
    /* tabula praeterita (genus ignotum, montatio deficiens): nominatur,
     * ceterae currunt */
    causa = vicus_causa(app->vicus);
    si (causa.mensura > ZEPHYRUM)
    {
        fprintf(stderr, "vicus: %.*s\n", (int)causa.mensura,
            causa.datum);
    }
    app->d = dispensator_creare(piscina, intern, app->vicus->repo,
        vicus_actiones(app->vicus), vicus_componere, app->vicus, CCC);
    si (!app->d)
    {
        redde FALSUM;
    }
    vicus_dispensatorem_ligare(app->vicus, app->d);
    redde VERUM;
}
