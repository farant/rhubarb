/* probatio_vicus_stratum.c - stratum supra latera (colloquia-plan
 * T4b): genus fictum 'tegmen' (scopus "s1" XL x XX ad C, C) ut stratum
 * montatur; genus ignotum et stratum iteratum recusantur; superficies
 * tota in ramo; ictus in scopo actionem strati vocat et focum non
 * mutat; ictus extra scopum ad latera transit (focus mutatur); in
 * omni tabula componitur; omni pulsu pulsatur; mutatio magnitudinis
 * superficiem strati scribit. Latera: genus fictum 'quietus'. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "canon.h"
#include "componens.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "vicus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Genera ficta
 * ================================================== */

interior b32
quietum_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol,
     InsulaRepositorium* r,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)sedes;
    (vacuum)p;
    (vacuum)in;
    (vacuum)vol;
    (vacuum)r;
    (vacuum)id;
    (vacuum)argumentum;
    (vacuum)latitudo;
    (vacuum)altitudo;
    (vacuum)ctx;
    redde VERUM;
}

interior vacuum
quietum_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    (vacuum)montatio;
    (vacuum)f;
}

hic_manens constans character CANON_TEGMINIS[] =
    "<canon dialectus=\"tegmen\" versio=\"1\">"
    "<elementum nomen=\"tegmen\" radix=\"verum\">"
    "<attributum nomen=\"id\" genus=\"nomen\"/>"
    "<attributum nomen=\"superficies_latitudo\" genus=\"numerus\"/>"
    "<attributum nomen=\"superficies_altitudo\" genus=\"numerus\"/>"
    "</elementum></canon>";

/* tegmen: ictus et pulsus numerantur */
nomen structura {
      ActioRegistrum* actiones;
                 i32  ictus;
                 i32  pulsus;
                 b32  mutat;
                 i32  latitudo;    /* montationis */
} Tegmen;

interior b32
tegmen_agere (
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
    si (   ev->genus == EVENTUS_MUS_DEPRESSUS && nodus
        && chorda_aequalis_literis(nodus->id, "s1"))
    {
        ((Tegmen*)ctx)->ictus++;
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
tegmen_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol,
     InsulaRepositorium* r,
     constans character* id,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
         Tegmen* t;
    InsulaRamus  ramus;
          Canon* canon;
         chorda  causa;
      character  elementum[CXXVIII];

    (vacuum)vol;
    (vacuum)argumentum;
    (vacuum)altitudo;
    (vacuum)ctx;
    t            = (Tegmen*)sedes;
    t->latitudo  = latitudo;
    t->actiones  = actio_registrum_creare(p, in);
    /* ramus cum canone suo (genera vera: organum_ramum_parare) */
    ramus  = insula_ramus(r, "tegmen", id);
    canon  = canon_legere(chorda_ex_literis(CANON_TEGMINIS, p), p, in,
        &causa);
    si (!canon)
    {
        redde FALSUM;
    }
    insula_ramus_canonem_ponere(&ramus, canon);
    sprintf(elementum, "<tegmen id=\"%s\"/>", id);
    redde t->actiones && insula_ramum_initiare(&ramus, elementum)
        && actio_registrare(t->actiones, "tegmen.agere", tegmen_agere,
        t);
}

/* radix 0 x 0, scopus "s1" XL x XX ad (C, C) */
interior Componens*
tegmen_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
     Componens* radix;
     Componens* s;
         Fines  f;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)ctx;
    radix  = componens_creare(piscina, intern, "tegmen", PARTES_NULLUM);
    s      = componens_creare(piscina, intern, "s1", PARTES_BOTTONE);
    si (!radix || !s)
    {
        redde radix;
    }
    memset(&f, ZEPHYRUM, magnitudo(f));
    componens_ponere_fines(radix, f);
    f.x         = C;
    f.y         = C;
    f.latitudo  = XL;
    f.altitudo  = XX;
    componens_ponere_fines(s, f);
    componens_ponere_actio(s, "tegmen.agere");
    componens_addere_liberum(radix, s);
    redde radix;
}

interior VicusPulsus
tegmen_pulsare (
    vacuum* ctx)
{
         Tegmen* t;
    VicusPulsus  p;

    t = (Tegmen*)ctx;
    t->pulsus++;
    p.mutatum = t->mutat;
    p.finitus = FALSUM;
    redde p;
}

interior vacuum
tegmen_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    f->actiones       = ((Tegmen*)montatio)->actiones;
    f->componere      = tegmen_componere;
    f->componere_ctx  = montatio;
    f->pulsare        = tegmen_pulsare;
    f->pulsare_ctx    = montatio;
}


/* ==================================================
 * Mundus
 * ================================================== */

nomen structura {
        Volumen* volumen;
          Vicus* v;
    Dispensator* d;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    thema_initiare();
    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_stratum");
    si (x->volumen)
    {
        x->v = vicus_creare(c->piscina,
            internamentum_creare(c->piscina),
            x->volumen, CDLXXX, CDLXXX);
    }
    si (   x->v
        && (   !vicus_genus_addere(x->v, "quietus", I, quietum_montare,
                   quietum_describere, NIHIL)
            || !vicus_genus_addere(x->v, "tegmen", magnitudo(Tegmen),
                   tegmen_montare, tegmen_describere, NIHIL)
            || !vicus_aperire(x->v,
                   "<tabulae activa=\"A\"><tabula id=\"A\">"
                   "<latus genus=\"quietus\"/><acervus>"
                   "<latus genus=\"quietus\"/></acervus></tabula>"
                   "<tabula id=\"B\"><latus genus=\"quietus\"/>"
                   "<acervus><latus genus=\"quietus\"/></acervus>"
                   "</tabula></tabulae>")))
    {
        x->v = NIHIL;
    }
    si (x->v)
    {
        x->d = dispensator_creare(c->piscina, x->v->intern, x->v->repo,
            vicus_actiones(x->v), vicus_componere, x->v, CCC);
        si (x->d)
        {
            vicus_dispensatorem_ligare(x->v, x->d);
        }
    }
    c->datum = x;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

interior Tegmen*
tegmen (
    Mundus* x)
{
    redde (x->v && x->v->stratum && x->v->stratum->montata)
        ? (Tegmen*)x->v->stratum->montatio : NIHIL;
}

interior vacuum
ictus (
    Mundus* x,
       s32  px,
       s32  py)
{
    Eventus e;

    dispensator_recomponere(x->d);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.tempus            = M;
    e.datum.mus.x       = px;
    e.datum.mus.y       = py;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(x->d, &e);
    e.genus = EVENTUS_MUS_LIBERATUS;
    dispensator_tractare(x->d, &e);
}


/* ==================================================
 * Sectiones
 * ================================================== */

/* additio: ignotum et iteratum recusantur; superficies tota */
interior vacuum
_probare_additionem (
    CredoContextus* c)
{
         Mundus* x;
    InsulaRamus  ramus;
         chorda* a;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL && x->d != NIHIL);
    CREDO_FALSUM(vicus_stratum_addere(x->v, "ignotum"));
    CREDO_VERUM(vicus_stratum_addere(x->v, "tegmen"));
    CREDO_FALSUM(vicus_stratum_addere(x->v, "tegmen"));
    CREDO_NECESSE(tegmen(x) != NIHIL);
    CREDO_AEQUALIS_I32(tegmen(x)->latitudo, CDLXXX);
    ramus  = insula_ramus(x->v->repo, "tegmen", "stratum_tegmen");
    a      = insula_ramus_attributum(&ramus, "superficies_latitudo");
    CREDO_NECESSE(a != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "480");
}

/* ictus: in scopo stratum agit, focus manet; extra ad latera */
interior vacuum
_probare_ictum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL && x->d != NIHIL);
    CREDO_NECESSE(vicus_stratum_addere(x->v, "tegmen"));
    CREDO_NECESSE(vicus_focum_ponere(x->v, VICUS_SINISTRUM));
    /* scopus (C..CXL, C..CXX) in latere sinistro - focato */
    ictus(x, CXX, CX);
    CREDO_AEQUALIS_I32(tegmen(x)->ictus, I);
    /* focus ad dextrum, deinde ictus in scopo: focus manet dexter */
    CREDO_NECESSE(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    ictus(x, CXX, CX);
    CREDO_AEQUALIS_I32(tegmen(x)->ictus, II);
    CREDO_AEQUALIS_I32(vicus_activa(x->v)->focus, VICUS_DEXTRUM);
    /* extra scopum in sinistro: latus focatur, stratum nihil */
    ictus(x, L, CC);
    CREDO_AEQUALIS_I32(tegmen(x)->ictus, II);
    CREDO_AEQUALIS_I32(vicus_activa(x->v)->focus, VICUS_SINISTRUM);
}

/* in omni tabula: scopus componitur et agit */
interior vacuum
_probare_tabulas (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL && x->d != NIHIL);
    CREDO_NECESSE(vicus_stratum_addere(x->v, "tegmen"));
    CREDO_NECESSE(vicus_activam_ponere(x->v, "B"));
    dispensator_recomponere(x->d);
    CREDO_NON_NIHIL(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("s1", c->piscina)));
    ictus(x, CXX, CX);
    CREDO_AEQUALIS_I32(tegmen(x)->ictus, I);
}

/* pulsus: semper; mutatum -> VERUM */
interior vacuum
_probare_pulsum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL);
    CREDO_NECESSE(vicus_stratum_addere(x->v, "tegmen"));
    (vacuum)vicus_pulsare(x->v);
    tegmen(x)->pulsus = ZEPHYRUM;
    CREDO_FALSUM(vicus_pulsare(x->v));
    CREDO_AEQUALIS_I32(tegmen(x)->pulsus, I);
    tegmen(x)->mutat = VERUM;
    CREDO_VERUM(vicus_pulsare(x->v));
    CREDO_AEQUALIS_I32(tegmen(x)->pulsus, II);
}

/* mutatio magnitudinis: superficies strati tota */
interior vacuum
_probare_magnitudinem (
    CredoContextus* c)
{
         Mundus* x;
        Eventus  e;
    InsulaRamus  ramus;
         chorda* a;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->v != NIHIL && x->d != NIHIL);
    CREDO_NECESSE(vicus_stratum_addere(x->v, "tegmen"));
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                               = EVENTUS_MUTARE_MAGNITUDINEM;
    e.datum.mutare_magnitudinem.latitudo  = DC;
    e.datum.mutare_magnitudinem.altitudo  = CD;
    dispensator_tractare(x->d, &e);
    ramus  = insula_ramus(x->v->repo, "tegmen", "stratum_tegmen");
    a      = insula_ramus_attributum(&ramus, "superficies_latitudo");
    CREDO_NECESSE(a != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "600");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "additio", _probare_additionem, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { "ictus", _probare_ictum, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "tabulae", _probare_tabulas, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "pulsus", _probare_pulsum, mundum_parare, mundum_purgare, NIHIL,
      NIHIL },
    { "magnitudo", _probare_magnitudinem, mundum_parare, mundum_purgare,
      NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("vicus_stratum", SECTIONES);
}
