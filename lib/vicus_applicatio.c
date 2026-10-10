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

/* S3e: contextus generis pictoris - memoria picturarum COMMUNIS omnium
 * laterum (documentum unum per spatium: latus quod picturam relinquit
 * et postea redit documentum alterius lateris recens videt, non
 * copiam veterem) */
nomen structura {
    TabulaDispersa* documenta;
} ContextusPictoris;

/* S3e: $pictor-next / $pictor-prev */
nomen structura {
                  Vicus* vicus;
                    s32  directio;     /* I next, -I prev */
     constans character* verbum;
} ContextusCycli;

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

/* ictus primus (Franus): ictus qui latus scribae focat iussum sub se
 * statim currit - ut si latus iam focatum esset. Pagina in spatio
 * lateris quaeritur (arbor composita); ictus extra iussum focat
 * solum */
interior b32
scribae_ictus_primus (
                 vacuum* ctx,
     InsulaRepositorium* repo,
                  Motus* motus,
              Componens* arbor,
       constans Eventus* ev)
{
    ScribaMontatio* m;
         Componens* pagina;
           Punctum  p;
            chorda  prior;
               b32  actum;

    m = (ScribaMontatio*)ctx;
    si (   !m || !arbor || ev->genus != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    pagina = componens_invenire_in_spatio(arbor, m->ramus.id,
        chorda_ex_literis("pagina", m->actiones_ctx.doc->piscina));
    p.x = ev->datum.mus.x;
    p.y = ev->datum.mus.y;
    si (   !pagina
        || !scriba_iussum_ad_punctum(&m->actiones_ctx, pagina, p))
    {
        redde FALSUM;
    }
    /* scriptor = actio paginae: domini status scribae (cursor, modus,
     * nuntius) 'pagina.clavis' solum admittunt - intra tractatorem
     * radicis vici scriptor alius est et scripturae tacite
     * recusarentur */
    prior = repo->scriptor;
    insula_scriptorem_ponere(repo, chorda_ex_literis("pagina.clavis",
        m->actiones_ctx.doc->piscina));
    actum = scriba_pagina_clavis(repo, motus, NIHIL, pagina, ev,
        &m->actiones_ctx);
    insula_scriptorem_ponere(repo, prior);
    redde actum;
}

interior vacuum
scribam_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    ScribaMontatio* m;

    m                    = (ScribaMontatio*)montatio;
    f->actiones          = m->actiones;
    f->figurae           = m->figurae;
    f->componere         = scriba_componere;
    f->componere_ctx     = &m->compositio;
    f->gestum_ponere     = scribae_gestum;
    f->gestum_ctx        = m;
    f->pulsare           = scribam_pulsare;
    f->pulsare_ctx       = m;
    f->ictus_primus      = scribae_ictus_primus;
    f->ictus_primus_ctx  = m;
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
     PictorMontatio* m;
  ContextusPictoris* cp;
             vacuum* valor;
             chorda  clavis;

    m   = (PictorMontatio*)sedes;
    cp  = (ContextusPictoris*)ctx;
    si (!pictor_montare(m, piscina, intern, volumen, repo, id, radix,
            latitudo, altitudo))
    {
        redde FALSUM;
    }
    /* S3e: memoria communis - documentum spatii huius, si iam
     * apertum, idem fit */
    si (cp && cp->documenta && id && id[ZEPHYRUM])
    {
        clavis  = chorda_ex_literis(id, piscina);
        valor   = NIHIL;
        si (tabula_dispersa_invenire(cp->documenta, clavis, &valor))
        {
            m->documenta = cp->documenta;
            (vacuum)pictor_picturam_ponere(m, id);
        }
        alioquin
        {
            (vacuum)tabula_dispersa_inserere(cp->documenta, clavis,
                m->doc);
            m->documenta = cp->documenta;
        }
    }
    /* S3e: argumentum picturam bibliothecae nominans ostenditur;
     * aliter (S3c, identitas sola) pictura lateris ipsius */
    si (argumentum && argumentum[ZEPHYRUM])
    {
        (vacuum)pictor_picturam_ponere(m, argumentum);
    }
    redde VERUM;
}

/* S3e: facies.argumentum_ponere pictoris */
interior b32
pictoris_argumentum_ponere (
                 vacuum* ctx,
     constans character* argumentum)
{
    /* S3f: pictura ignota ($pictor-new) conditur */
    redde pictor_picturam_ponere((PictorMontatio*)ctx, argumentum)
        || pictor_picturam_condere((PictorMontatio*)ctx, argumentum);
}

interior vacuum
pictorem_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    PictorMontatio* m;

    m                         = (PictorMontatio*)montatio;
    f->actiones               = m->actiones;
    f->figurae                = m->figurae;
    f->componere              = pictor_componere;
    f->componere_ctx          = &m->compositio;
    f->fons                   = pictor_imago_fons;
    f->fons_ctx               = &m->figurae_ctx;
    f->argumentum_ponere      = pictoris_argumentum_ponere;
    f->argumentum_ponere_ctx  = m;
}

/* S3e: pictura lateris pictoris: argumentum si picturam bibliothecae
 * nominat, aliter id lateris */
interior chorda
pictura_lateris (
       constans VicusLatus* l,
              constans Xar* bibliotheca)
{
    i32 i;

    per (i = ZEPHYRUM; !chorda_vacua(l->argumentum)
                       && i < xar_numerus(bibliotheca); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(bibliotheca, i),
            l->argumentum))
        {
            redde l->argumentum;
        }
    }
    redde l->id;
}

/* S3e: $pictor-next / $pictor-prev (Franus): frons pictoris tabulae
 * activae picturam proximam bibliothecae (ordine viae, circulo)
 * ostendit - etiam in latere alio ostensam (documentum unum, memoria
 * communis: latera plura tuta; Franus regulam praeteritionis
 * removit). Nullus pictor in acervo: latus novum. PETITIO. */
interior b32
cyclus_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
    constans ContextusCycli* cc;
                VicusTabula* t;
                 VicusLatus* l;
                 VicusLatus* frons;
                        Xar* bibliotheca;
                     chorda  currens;
                     chorda  pictura;
                        s32  n;
                        s32  initium;
                        s32  k;
                        s32  index;
                        i32  i;

    cc = (constans ContextusCycli*)ctx;
    si (iussum->numerus_argumentorum > ZEPHYRUM)
    {
        effectus->error =
            chorda_concatenare(chorda_ex_literis(cc->verbum,
            piscina), chorda_ex_literis(": nulla argumenta", piscina),
            piscina);
        redde VERUM;
    }
    bibliotheca  = pictor_documenta_enumerare(cc->vicus->volumen,
        piscina);
    t      = vicus_activa(cc->vicus);
    frons  = NIHIL;
    per (i = ZEPHYRUM; t && i < xar_numerus(t->acervus); i++)
    {
        l = (VicusLatus*)xar_obtinere(t->acervus, i);
        si (chorda_aequalis_literis(l->genus, "pictor"))
        {
            frons = l;
        }
    }
    currens.datum    = NIHIL;
    currens.mensura  = ZEPHYRUM;
    si (frons && bibliotheca)
    {
        currens = pictura_lateris(frons, bibliotheca);
    }
    n        = bibliotheca ? (s32)xar_numerus(bibliotheca) : ZEPHYRUM;
    initium  = -I;
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(bibliotheca, (i32)k),
            currens))
        {
            initium = k;
        }
    }
    per (k = I; k <= n; k++)
    {
        index = initium < ZEPHYRUM
            ? (cc->directio > ZEPHYRUM ? k - I : n - k)
            : ((initium + cc->directio * k) % n + n) % n;
        pictura = *(chorda*)xar_obtinere(bibliotheca, (i32)index);
        si (chorda_vacua(pictura) || chorda_aequalis(pictura, currens))
        {
            perge;
        }
        si (!vicus_acervo_mutare(cc->vicus, "pictor",
                chorda_ut_cstr(pictura, piscina)))
        {
            effectus->error = chorda_concatenare(chorda_ex_literis(
                cc->verbum, piscina), chorda_ex_literis(
                ": mutari non potest", piscina), piscina);
        }
        redde VERUM;
    }
    effectus->error = chorda_concatenare(chorda_ex_literis(cc->verbum,
        piscina), chorda_ex_literis(": nulla alia pictura", piscina),
        piscina);
    redde VERUM;
}

/* S3f: $pictor-new (Franus): pictura nova vacua "pictura_<n>" (n
 * primum liberum ab I) in fronte pictoris tabulae activae; nullus
 * pictor: latus novum. PETITIO. */
interior b32
novum_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* piscina,
     IussumEffectus* effectus)
{
    constans ContextusCycli* cc;
                        Xar* bibliotheca;
                  character  titulus[XXXII];
                        s32  n;
                        i32  i;
                        b32  liberum;

    cc = (constans ContextusCycli*)ctx;
    si (iussum->numerus_argumentorum > ZEPHYRUM)
    {
        effectus->error =
            chorda_concatenare(chorda_ex_literis(cc->verbum,
            piscina), chorda_ex_literis(": nulla argumenta", piscina),
            piscina);
        redde VERUM;
    }
    bibliotheca = pictor_documenta_enumerare(cc->vicus->volumen,
        piscina);
    per (n = I; n < M; n++)
    {
        sprintf(titulus, "pictura_%d", (integer)n);
        liberum = VERUM;
        per (i = ZEPHYRUM; bibliotheca && i < xar_numerus(bibliotheca);
             i++)
        {
            si (chorda_aequalis_literis(*(chorda*)xar_obtinere(
                bibliotheca, i), titulus))
            {
                liberum = FALSUM;
            }
        }
        si (liberum)
        {
            frange;
        }
    }
    si (!vicus_acervo_mutare(cc->vicus, "pictor", titulus))
    {
        effectus->error =
            chorda_concatenare(chorda_ex_literis(cc->verbum,
            piscina), chorda_ex_literis(": condi non potest", piscina),
            piscina);
    }
    redde VERUM;
}

interior b32
cyclum_registrare (
        IussumRegistrum* r,
                  Vicus* v,
     constans character* verbum,
                    s32  directio,
                Piscina* piscina)
{
    ContextusCycli* cc;

    cc = (ContextusCycli*)piscina_allocare(piscina,
        magnitudo(ContextusCycli));
    si (!cc)
    {
        redde FALSUM;
    }
    cc->vicus     = v;
    cc->directio  = directio;
    cc->verbum    = verbum;
    redde iussum_registrare(r, verbum, FALSUM, directio == ZEPHYRUM
        ? novum_iussum : cyclus_iussum, cc);
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
    /* Franus: 'tmux a' statim post aperturam ad concham, non scribam */
    f->focus_in_apertura = VERUM;
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
   ContextusPictoris* cp;

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
    /* S3e: memoria picturarum communis */
    cp = (ContextusPictoris*)piscina_allocare(piscina,
        magnitudo(ContextusPictoris));
    si (cp)
    {
        cp->documenta = tabula_dispersa_creare_chorda(piscina, XVI);
    }
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
        || !cyclum_registrare(cs->iussa, app->vicus, "pictor-next", I,
               piscina)
        || !cyclum_registrare(cs->iussa, app->vicus, "pictor-prev", -I,
               piscina)
        || !cyclum_registrare(cs->iussa, app->vicus, "pictor-new",
               ZEPHYRUM, piscina)
        || !vicus_genus_addere(app->vicus, "scriba",
               magnitudo(ScribaMontatio), scribam_montare,
               scribam_describere, cs)
        || !vicus_genus_addere(app->vicus, "pictor",
               magnitudo(PictorMontatio), pictorem_montare,
               pictorem_describere, cp)
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
