/* vicus_applicatio.c - compositio communis hospitis (T4) */

#include "vicus_applicatio.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "terminale.h"
#include "iussum.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

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

/* S2b/S3b: contextus generis scribae - liber paginarum et iussa,
 * communia omnibus visibus */
nomen structura {
        ScribaLiber* liber;
    IussumRegistrum* iussa;
} ContextusScribae;

/* S3b: '$dies' -> dies hodiernus "MM/DD/YYYY" (forma concha vetus;
 * consumens, ut prunifex). S3b-2: '$dies(N)' = N dies ab hodie
 * (signatus: -1 heri, 7 hebdomas post); mktime menses et annos
 * normat. Argumentum non numerus aut plura: error. */
interior b32
dies_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
          time_t nunc;
    structura tm* tm;
    structura tm  dies;
             s32 gradus;
       character textus[XXXII];

    (vacuum)ctx;
    gradus = ZEPHYRUM;
    si (iussum->numerus_argumentorum > I)
    {
        effectus->error = chorda_ex_literis("dies: unum argumentum",
            piscina);
        redde VERUM;
    }
    si (   iussum->numerus_argumentorum == I
        && !chorda_ut_s32(iussum->argumenta[ZEPHYRUM], &gradus))
    {
        effectus->error = chorda_concatenare(chorda_ex_literis(
            "dies: numerus dierum non intellegitur: ", piscina),
            iussum->argumenta[ZEPHYRUM], piscina);
        redde VERUM;
    }
    nunc  = time(NIHIL);
    tm    = localtime(&nunc);
    si (!tm)
    {
        redde FALSUM;
    }
    dies           = *tm;
    dies.tm_mday   += gradus;
    dies.tm_isdst  = -I;
    si (mktime(&dies) == (time_t)-I)
    {
        redde FALSUM;
    }
    sprintf(textus, "%02d/%02d/%04d", dies.tm_mon + I, dies.tm_mday,
        dies.tm_year + MCM);
    effectus->textus = chorda_ex_literis(textus, piscina);
    redde VERUM;
}

/* S3c: verba aperientia ('$terminale', '$scriba(nomen)',
 * '$pictor(nomen)') - latus in acervo tabulae activae (petitio,
 * pulsu proximo applicata); signum manet (non consumunt) */
nomen structura {
                  Vicus* vicus;
     constans character* genus;
} ContextusAperiendi;

interior b32
nomen_paginae_validum (
    chorda c)
{
          i32 i;
    character x;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        x = (character)c.datum[i];
        si (!(   (x >= 'a' && x <= 'z') || (x >= '0' && x <= '9')
              || x == '_' || x == '-'))
        {
            redde FALSUM;
        }
    }
    redde c.mensura > ZEPHYRUM;
}

interior b32
aperire_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
    constans ContextusAperiendi* ca;
                         chorda  arg;

    ca = (constans ContextusAperiendi*)ctx;
    si (iussum->numerus_argumentorum > I)
    {
        effectus->error = chorda_concatenare(chorda_ex_literis(
            ca->genus, piscina), chorda_ex_literis(": unum argumentum",
            piscina), piscina);
        redde VERUM;
    }
    arg.datum    = NIHIL;
    arg.mensura  = ZEPHYRUM;
    si (iussum->numerus_argumentorum == I)
    {
        arg = iussum->argumenta[ZEPHYRUM];
    }
    si (   arg.mensura > ZEPHYRUM
        && strcmp(ca->genus, "scriba") == ZEPHYRUM
        && !nomen_paginae_validum(arg))
    {
        effectus->error = chorda_concatenare(chorda_ex_literis(
            "scriba: nomen paginae invalidum: ", piscina), arg,
            piscina);
        redde VERUM;
    }
    si (!vicus_acervo_aperire(ca->vicus, ca->genus,
            arg.mensura > ZEPHYRUM ? chorda_ut_cstr(arg, piscina)
                                   : NIHIL))
    {
        effectus->error = chorda_concatenare(chorda_ex_literis(
            ca->genus, piscina), chorda_ex_literis(
            ": aperiri non potest", piscina), piscina);
    }
    redde VERUM;
}

interior b32
aperiens_registrare (
        IussumRegistrum* r,
                  Vicus* v,
     constans character* genus,
                Piscina* piscina)
{
    ContextusAperiendi* ca;

    ca = (ContextusAperiendi*)piscina_allocare(piscina,
        magnitudo(ContextusAperiendi));
    si (!ca)
    {
        redde FALSUM;
    }
    ca->vicus = v;
    ca->genus = genus;
    redde iussum_registrare(r, genus, FALSUM, aperire_iussum, ca);
}

interior b32
scribam_montare (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    ContextusScribae* cs;
      ScribaMontatio* m;

    /* S2b/S3b: ctx = contextus communis (aedificare) */
    cs  = (ContextusScribae*)ctx;
    m   = (ScribaMontatio*)sedes;
    /* S3c: '$scriba(nomen)' - pagina nominata (condita si deest) est
     * pagina PRIMA visus; visus iam servatus (reapertura, navigatio)
     * suam servat */
    si (argumentum && cs->liber)
    {
        chorda visus;
           b32 inventum;

        visus = chorda_concatenare(chorda_ex_literis("scriba/visus/",
            piscina), chorda_ex_literis(id, piscina), piscina);
        (vacuum)volumen_plagulam_promere(volumen, visus, piscina,
            &inventum);
        si (!inventum)
        {
            si (!scriba_liber_paginam_condere(cs->liber,
                    chorda_ex_literis(argumentum, piscina)))
            {
                redde FALSUM;
            }
            (vacuum)volumen_plagulam_condere(volumen, visus,
                chorda_ex_literis(argumentum, piscina), "scriba:visus");
        }
    }
    si (!scriba_montare(m, piscina, intern, volumen, repo, id, radix,
            latitudo, altitudo, cs->liber))
    {
        redde FALSUM;
    }
    m->actiones_ctx.iussa = cs->iussa;
    redde VERUM;
}

/* focus advenit (S2b): visus stalus PRIMUM reficitur - aliter clavis
 * prima super folium vetus scriberet et mutationem alterius visus
 * reverteret; deinde gestus */
interior vacuum
scribae_gestum (
     Motus* motus,
    vacuum* ctx)
{
    ScribaMontatio* m;

    m = (ScribaMontatio*)ctx;
    (vacuum)scriba_reficere(m);
    scriba_gestum_ponere(motus, &m->actiones_ctx);
}

/* pulsus (S2b): visus stalus reficitur (pagina ab alio visu mutata) */
interior VicusPulsus
scribam_pulsare (
    vacuum* ctx)
{
    VicusPulsus p;

    p.mutatum = scriba_reficere((ScribaMontatio*)ctx);
    p.finitus = FALSUM;
    redde p;
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
    f->gestum_ctx     = m;
    f->pulsare        = scribam_pulsare;
    f->pulsare_ctx    = m;
}

interior b32
pictorem_montare (
                 vacuum* sedes,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* argumentum,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)ctx;
    /* S3c: argumentum identitatem solam dat (documentum per id
     * lateris) - picturae nominatae communes postea */
    (vacuum)argumentum;
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
     constans character* argumentum,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)ctx;
    (vacuum)volumen;
    (vacuum)radix;
    (vacuum)argumentum;
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

/* folium paginae novae (S2b): lateris sinistri cellulae (dimidium
 * cellulis rotundatum) minus margines; altitudo minus linea tabularum,
 * status, margines - minima ut scriba (XX x X) */
interior i32
folii_columnae (
    i32 latitudo)
{
    s32 n;

    n = ((s32)latitudo / II) / VICUS_CELLULA_LATITUDO - II;
    redde (i32)(n < XX ? XX : n);
}

interior i32
folii_lineae (
    i32 altitudo)
{
    s32 n;

    n = ((s32)altitudo - VICUS_ALTITUDO_TABULARUM)
        / VICUS_CELLULA_ALTITUDO - III;
    redde (i32)(n < X ? X : n);
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
              chorda  causa;
    ContextusScribae* cs;

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
    /* S2b: liber paginarum UNUS pro omnibus visibus scribae; paginae
     * novae magnitudine lateris (dimidium cellulis rotundatum, minus
     * margines et status - ut folium S2c) */
    cs = (ContextusScribae*)piscina_allocare(piscina,
        magnitudo(ContextusScribae));
    cs->liber = scriba_liber_aperire(piscina, intern, volumen,
        folii_columnae(latitudo), folii_lineae(altitudo));
    /* S3b: iussa omnium visuum scribae */
    cs->iussa = iussum_registrum_creare(piscina);
    si (   !app->vicus || !cs->liber || !cs->iussa
        || !iussum_registrare(cs->iussa, "dies", VERUM, dies_iussum,
               NIHIL)
        || !aperiens_registrare(cs->iussa, app->vicus, "terminale",
               piscina)
        || !aperiens_registrare(cs->iussa, app->vicus, "scriba",
               piscina)
        || !aperiens_registrare(cs->iussa, app->vicus, "pictor",
               piscina)
        || !vicus_genus_addere(app->vicus, "scriba",
               magnitudo(ScribaMontatio), scribam_montare,
               scribam_describere, cs)
        || !vicus_genus_addere(app->vicus, "pictor",
               magnitudo(PictorMontatio), pictorem_montare,
               pictorem_describere, NIHIL)
        || !vicus_genus_addere(app->vicus, "terminale",
               magnitudo(TerminaleApplicatio),
               terminale_montare_in_vico, terminale_describere, NIHIL)
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
