/* probatio_catena_foci.c - catena foci (status-plan S8c-2): organum
 * ludicrum (probationes/fixa/organum/ludibrium.stml) tres gradus
 * montatum sub dispensatore vero. Quisque gradus componentem
 * focusabilem 'agere' (actio ludibrium.agere, ctx = ramus gradus)
 * fert; parens registrum liberi in via liberi miscet, hospes gradum
 * primum in via sua. Motus.ramus = gradus primus (ut vicus latus
 * focatum). Ictus in gradu III catenam scribit; claves eo eunt; ictus
 * in gradu I catenam praecidit (focus inferiorum manet); pan/zoom
 * sordida in ramum intimum relictum effunduntur. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "insula.h"
#include "componens.h"
#include "actio.h"
#include "motus.h"
#include "dispensator.h"
#include "organum.h"
#include "organum_montatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_LUDIBRII "probationes/fixa/organum/ludibrium.stml"

/* tres gradus ludibrii (id "liber" ubique) sub hospite */
nomen structura {
    InsulaRepositorium* repo;
           InsulaRamus  g[III];
        ActioRegistrum* reg[III];
        ActioRegistrum* hospes;
           Dispensator* d;
} Mundus;

interior vacuum
incrementare (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    chorda* a;
       s32  v;

    (vacuum)ctx;
    a = stml_attributum_capere(radix, "valor");
    v = ZEPHYRUM;
    si (a)
    {
        (vacuum)chorda_ut_s32(*a, &v);
    }
    insula_attributum_ponere(radix, p, in, "valor",
        chorda_ut_cstr(chorda_ex_s32(v + I, p), p));
}

/* actio 'ludibrium.agere': ictus et claves valorem gradus sui augent
 * (ctx = ramus gradus) */
interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    (vacuum)nodus;
    si (   ev->genus != EVENTUS_MUS_DEPRESSUS
        && ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    redde mutare_ramum((InsulaRamus*)ctx, incrementare, NIHIL);
}

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                    s32  x,
                    s32  latitudo)
{
    Componens* c;
        Fines  f;

    c           = componens_creare(p, in, id, PARTES_NULLUM);
    f.x         = x;
    f.y         = ZEPHYRUM;
    f.latitudo  = latitudo;
    f.altitudo  = C;
    componens_ponere_fines(c, f);
    redde c;
}

/* gradus i: radix in via sua (x relativum C nisi primus), 'agere'
 * ad (X, X) relativum, gradus proximus intra */
interior Componens*
gradum_componere (
                 Mundus* m,
                    i32  i,
                Piscina* p,
    InternamentumChorda* in)
{
    Componens* r;
    Componens* a;

    r = nodus(p, in, "radix", i == ZEPHYRUM ? ZEPHYRUM : C,
        CCC - (s32)i * C);
    componens_ponere_spatium(r, chorda_ut_cstr(m->g[i].via, p));
    a                  = nodus(p, in, "agere", X, XX);
    a->fines.y         = X;
    a->fines.altitudo  = XX;
    componens_ponere_actio(a, "ludibrium.agere");
    componens_ponere_focusabilis(a, VERUM);
    componens_addere_liberum(r, a);
    si (i + I < III)
    {
        componens_addere_liberum(r, gradum_componere(m, i + I, p, in));
    }
    redde r;
}

/* hospes (CD x C): gradus I ad x 0, II ad C, III ad CC; 'agere'
 * gradus k ad x k*C + X..XXX */
interior Componens*
componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    Componens* radix;

    (vacuum)repo;
    (vacuum)motus;
    radix = nodus(p, in, "hospes", ZEPHYRUM, CD);
    componens_addere_liberum(radix, gradum_componere((Mundus*)ctx,
        ZEPHYRUM, p, in));
    redde radix;
}

/* mundus: organa = per organum (canon, domini) aut rami nudi (sine
 * canone - pan/zoom libere scribuntur) */
interior b32
mundum_creare (
                 Mundus* m,
                    b32  organa,
                Piscina* p,
    InternamentumChorda* in)
{
        Organum* o;
    InsulaRamus  radix;
          Motus* motus;
            i32  i;

    memset(m, ZEPHYRUM, magnitudo(*m));
    m->repo = insula_repositorium_creare(p, in, "<hospes/>");
    o = organum_legere_viam(p, in, NIHIL, VIA_LUDIBRII,
        "probatio");
    si (!m->repo || !o)
    {
        redde FALSUM;
    }
    radix = insula_ramus_radix(m->repo);
    per (i = ZEPHYRUM; i < III; i++)
    {
        m->g[i] = insula_ramus_liberi(i == ZEPHYRUM ? &radix
            : &m->g[i - I], "ludibrium", "liber");
        si (organa ? !organum_ramum_parare(o, &m->g[i], NIHIL, NIHIL,
                chorda_ut_cstr(m->g[i].via, p), p, in)
                : !insula_ramum_initiare(&m->g[i],
                "<ludibrium id=\"liber\"/>"))
        {
            redde FALSUM;
        }
        m->reg[i] = actio_registrum_creare(p, in);
        (vacuum)actio_registrare(m->reg[i], "ludibrium.agere", agere,
            &m->g[i]);
    }
    /* compositio vera: parens liberum in via liberi, hospes primum */
    per (i = II; i > ZEPHYRUM; i--)
    {
        si (!actio_registrum_miscere_in_spatio(m->reg[i - I], m->reg[i],
                m->g[i].via))
        {
            redde FALSUM;
        }
    }
    m->hospes = actio_registrum_creare(p, in);
    si (!actio_registrum_miscere_in_spatio(m->hospes, m->reg[ZEPHYRUM],
            m->g[ZEPHYRUM].via))
    {
        redde FALSUM;
    }
    m->d = dispensator_creare(p, in, m->repo, m->hospes, componere, m,
        CCC);
    si (!m->d)
    {
        redde FALSUM;
    }
    /* hospes caput catenae ponit (ut vicus motum_aptare) */
    motus           = dispensator_motus(m->d);
    motus->ramus    = m->g[ZEPHYRUM];
    motus->spatium  = m->g[ZEPHYRUM].via;
    redde VERUM;
}

interior vacuum
ictus (
    Mundus* m,
       s64  t,
       s32  x)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.tempus       = t;
    e.datum.mus.x  = x;
    e.datum.mus.y  = XV;
    dispensator_tractare(m->d, &e);
}

interior vacuum
clavis (
    Mundus* m,
       s64  t)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                 = t;
    e.datum.clavis.producta  = 'x';
    e.datum.clavis.clavis    = (clavis_t)'x';
    dispensator_tractare(m->d, &e);
}

interior b32
valor_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

/* gradus k (0..II): valores "a b c" */
interior b32
valores_sunt (
                Mundus* m,
    constans character* a,
    constans character* b,
    constans character* c)
{
    redde valor_est(&m->g[ZEPHYRUM], "valor", a)
        && valor_est(&m->g[I], "valor", b)
        && valor_est(&m->g[II], "valor", c);
}

/* ictus in gradu III: actio gradus III; catena a capite scripta;
 * ramus intimus = gradus III */
interior vacuum
_probare_catenam (
    CredoContextus* c)
{
    InternamentumChorda* in;
                 Mundus  m;
                  Motus* motus;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, VERUM, c->piscina, in));
    motus = dispensator_motus(m.d);
    ictus(&m, M, CCXV);
    CREDO_VERUM(valores_sunt(&m, "0", "0", "1"));
    CREDO_VERUM(valor_est(&m.g[ZEPHYRUM], "focus", "ludibrium:liber"));
    CREDO_VERUM(valor_est(&m.g[I], "focus", "ludibrium:liber"));
    CREDO_VERUM(valor_est(&m.g[II], "focus", "agere"));
    CREDO_VERUM(motus->ramus_intimus.via.datum == m.g[II].via.datum);
    CREDO_CHORDA_AEQUALIS(motus->spatium_intimum, m.g[II].via);
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(m.d), "agere");
    /* caput catenae ab hospite intactum */
    CREDO_CHORDA_AEQUALIS(motus->spatium, m.g[ZEPHYRUM].via);
}

/* claves ad focum gradus intimi */
interior vacuum
_probare_claves (
    CredoContextus* c)
{
    InternamentumChorda* in;
                 Mundus  m;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, VERUM, c->piscina, in));
    ictus(&m, M, CCXV);
    clavis(&m, M + X);
    clavis(&m, M + XX);
    CREDO_VERUM(valores_sunt(&m, "0", "0", "3"));
    /* gradus II: catena eadem, intimus mutatur */
    ictus(&m, M + XXX, CXV);
    CREDO_VERUM(valor_est(&m.g[I], "focus", "agere"));
    clavis(&m, M + XL);
    CREDO_VERUM(valores_sunt(&m, "0", "2", "3"));
}

/* ictus in gradu I catenam praecidit; focus inferiorum manet et
 * reditus eum restituit */
interior vacuum
_probare_praecisionem (
    CredoContextus* c)
{
    InternamentumChorda* in;
                 Mundus  m;
                  Motus* motus;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, VERUM, c->piscina, in));
    motus = dispensator_motus(m.d);
    ictus(&m, M, CCXV);
    ictus(&m, M + X, XV);
    CREDO_VERUM(valor_est(&m.g[ZEPHYRUM], "focus", "agere"));
    CREDO_VERUM(valor_est(&m.g[I], "focus", "ludibrium:liber"));
    CREDO_VERUM(valor_est(&m.g[II], "focus", "agere"));
    CREDO_CHORDA_AEQUALIS(motus->spatium_intimum, m.g[ZEPHYRUM].via);
    clavis(&m, M + XX);
    CREDO_VERUM(valores_sunt(&m, "2", "0", "1"));
}

/* pan/zoom sordida in intimum RELICTUM effunduntur (rami nudi: sine
 * canone, pan_x libere) */
interior vacuum
_probare_effusionem (
    CredoContextus* c)
{
    InternamentumChorda* in;
                 Mundus  m;
                  Motus* motus;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, FALSUM, c->piscina, in));
    motus = dispensator_motus(m.d);
    ictus(&m, M, CCXV);
    motus->pan.x                      = VII;
    motus->sordida                    = VERUM;
    motus->tempus_ultimae_mutationis  = M + X;
    ictus(&m, M + X, XV);
    CREDO_VERUM(valor_est(&m.g[II], "pan_x", "7"));
    CREDO_VERUM(valor_est(&m.g[ZEPHYRUM], "pan_x", NIHIL));
    CREDO_FALSUM(motus->sordida);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "catena",       _probare_catenam,       NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "claves",       _probare_claves,        NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "praecisio",    _probare_praecisionem,  NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "effusio",      _probare_effusionem,    NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("catena_foci", SECTIONES);
}
