/* probatio_negotia_visus.c - latus negotiorum (negotia-plan T3):
 * index ordine activitatis cum summario; ictus et Enter portam
 * negotium_aperire vocant; sine porta visus in loco (Esc ad indicem);
 * argumentum (id, URI, ignotum); facultates cum globulis; status
 * vacui; linea sub mure (super) et exitus eius; renovatio scripturae
 * alienae per pulsum, etiam lateribus duobus Negotia communem
 * pulsantibus; paginae. Ids per moneta_numerator: beta "1", alpha
 * "2", facultates alphae "3" (shipped) et "4" (planned). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "volumen.h"
#include "moneta.h"
#include "negotia.h"
#include "negotia_visus.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
                Volumen* volumen;
                Negotia* n;
                    i32  numerator;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
} Mundus;

nomen structura {
       NegotiaMontatio  m;
           Dispensator* d;
    NegotiaVisusPortae  portae;
                   i32  aperta;
             character  acta[LXIV];
} Latus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* w;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->volumen = volumen_temporarium(c->piscina,
        "probatio_negotia_visus");
    si (w->volumen)
    {
        w->n = negotia_aperire(w->volumen, moneta_numerator,
            &w->numerator, c->piscina);
    }
    w->intern = internamentum_creare(c->piscina);
    w->repo   = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    c->datum  = w;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* w = (Mundus*)c->datum;

    si (w && w->volumen)
    {
        volumen_claudere(w->volumen);
    }
}

interior chorda
lit (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

/* beta "1", alpha "2" (facultates "3" shipped, "4" planned): alpha
 * activior, ergo r1 */
interior vacuum
seminare (
     Mundus* w,
    Piscina* p)
{
    chorda alpha;

    (vacuum)negotia_negotium_creare(w->n, lit("beta", p), lit("", p));
    alpha = negotia_negotium_creare(w->n, lit("alpha", p), lit("", p));
    (vacuum)negotia_facultatem_creare(w->n, alpha, lit("bibliotheca",
        p), NEGOTIA_PERFECTA, lit("acta in volumine", p));
    (vacuum)negotia_facultatem_creare(w->n, alpha, lit("tabula", p),
        NEGOTIA_PROPOSITA, lit("", p));
}

interior b32
porta_ficta (
     vacuum* ctx,
     chorda  negotium)
{
    Latus* l = (Latus*)ctx;

    l->aperta++;
    sprintf(l->acta, "%.*s", (integer)(negotium.mensura < LX
        ? negotium.mensura : LX), (constans character*)negotium.datum);
    redde VERUM;
}

/* latus CCLXXX x altitudo in ramo id; argumentum NIHIL = nullum */
interior Latus*
latus_montare (
          CredoContextus* c,
                  Mundus* w,
      constans character* id,
      constans character* argumentum,
                     b32  cum_porta,
                     i32  altitudo)
{
    Latus* l;
    Motus* motus;

    l = (Latus*)piscina_allocare(c->piscina, magnitudo(Latus));
    memset(l, ZEPHYRUM, magnitudo(Latus));
    l->portae.ctx               = l;
    l->portae.negotium_aperire  = porta_ficta;
    si (   !negotia_visus_montare(&l->m, c->piscina, w->intern, w->repo,
               id, w->n, cum_porta ? &l->portae : NIHIL, CCLXXX,
               altitudo, NIHIL)
        || (   argumentum
            && !negotia_visus_argumentum_ponere(&l->m, argumentum)))
    {
        redde NIHIL;
    }
    l->d = dispensator_creare(c->piscina, w->intern, w->repo,
        l->m.actiones, negotia_visus_componere, &l->m, CCC);
    si (!l->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(l->d);
    motus->ramus  = l->m.ramus;
    dispensator_recomponere(l->d);
    redde l;
}

interior vacuum
clavis (
        Latus* l,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (s32)typus;
    dispensator_tractare(l->d, &e);
    dispensator_recomponere(l->d);
}

/* Enter (CLAVIS_REDITUS) aut Esc (CLAVIS_EFFUGIUM) */
interior vacuum
clavis_specialis (
    Latus* l,
      s32  k)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = (clavis_t)k;
    e.datum.clavis.producta  = k;
    dispensator_tractare(l->d, &e);
    dispensator_recomponere(l->d);
}

interior Fines
fines_componentis (
                  Latus* l,
     constans character* id,
                Piscina* p)
{
    Componens* x;
        Fines  f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    x = componens_invenire_per_id(dispensator_arbor(l->d),
        chorda_ex_literis(id, p));
    si (x)
    {
        f = x->fines;
    }
    redde f;
}

interior b32
adest (
                  Latus* l,
     constans character* id,
                Piscina* p)
{
    redde fines_componentis(l, id, p).altitudo > ZEPHYRUM;
}

interior b32
ictus (
                   Latus* l,
      constans character* id,
         eventus_genus_t  genus,
                 Piscina* p)
{
      Fines f;
    Eventus e;

    f = fines_componentis(l, id, p);
    si (f.altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = genus;
    e.datum.mus.x       = f.x + f.latitudo / II;
    e.datum.mus.y       = f.y + f.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(l->d, &e);
    dispensator_recomponere(l->d);
    redde VERUM;
}

interior Mandata*
reddere (
       Mundus* w,
        Latus* l,
      Piscina* p)
{
    Mandata* md;

    md = mandata_creare(p, w->intern);
    pingere(dispensator_arbor(l->d), l->m.figurae, ZEPHYRUM, md);
    redde md;
}

interior b32
textus_pictus (
                Mandata* md,
     constans character* textus)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_aequalis_literis(x->textus, textus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* y textus primi qui fragmentum continet; -1 si nullus */
interior s32
textus_y (
                Mandata* md,
     constans character* fragmentum)
{
    constans Mandatum* x;
                  i32  i;
               chorda  f;

    f = chorda_ex_literis(fragmentum, md->piscina);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus, f))
        {
            redde x->fines.y;
        }
    }
    redde -I;
}

/* omnis textus intra altitudinem lateris (linea VIII pixela) */
interior b32
textus_intra (
    Mandata* md,
        s32  altitudo)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && (x->fines.y < ZEPHYRUM || x->fines.y + VIII > altitudo))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* linea sub mure: rectangulum exemplaris colore selectionis (figura_
 * ordinem_super_pingere) */
interior b32
super_pictum (
    Mandata* md)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus       == MANDATUM_RECTANGULUM && x->impletum
            && x->exemplar    != ZEPHYRUM
            && x->color.genus == COLOR_MANDATI_THEMA
            && x->color.valor == (i32)COLOR_SELECTION)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
campus_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, titulus);
    redde valor ? (a && chorda_aequalis_literis(*a, valor))
                : (a == NIHIL || a->mensura == ZEPHYRUM);
}

interior b32
locus_est (
                  Latus* l,
     constans character* titulus,
                Piscina* p)
{
    redde chorda_aequalis_literis(negotia_visus_locus_titulus(&l->m, p),
        titulus);
}

interior vacuum
_probare_indicem (
    CredoContextus* c)
{
     Mundus* w = (Mundus*)c->datum;
      Latus* l;
    Mandata* md;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l = latus_montare(c, w, "c1", NIHIL, VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(adest(l, "r1", c->piscina));
    CREDO_VERUM(adest(l, "r2", c->piscina));
    CREDO_FALSUM(adest(l, "r3", c->piscina));
    md = reddere(w, l, c->piscina);
    CREDO_VERUM(textus_pictus(md, "Projects"));
    CREDO_VERUM(textus_pictus(md, "alpha"));
    CREDO_VERUM(textus_pictus(md, "beta"));
    CREDO_VERUM(textus_pictus(md, "1 shipped \302\267 1 planned"));
    CREDO_VERUM(locus_est(l, "Projects", c->piscina));
}

interior vacuum
_probare_indicem_vacuum (
    CredoContextus* c)
{
    Mundus* w = (Mundus*)c->datum;
     Latus* l;

    CREDO_NECESSE_NON_NIHIL(w->n);
    l = latus_montare(c, w, "c1", NIHIL, VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_FALSUM(adest(l, "r1", c->piscina));
    CREDO_VERUM(textus_pictus(reddere(w, l, c->piscina),
        "No projects yet - create one with `negotia creare`"));
}

/* ictus unus et Enter: porta cum id pleno; visus index manet */
interior vacuum
_probare_aperire (
    CredoContextus* c)
{
    Mundus* w = (Mundus*)c->datum;
     Latus* l;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l = latus_montare(c, w, "c1", NIHIL, VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(ictus(l, "r1", EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_AEQUALIS_I32(l->aperta, I);
    CREDO_LITERAE_AEQUALES(l->acta, "2");
    clavis(l, 'j');
    CREDO_VERUM(campus_est(&l->m.ramus, "electa", "1"));
    clavis_specialis(l, (s32)CLAVIS_REDITUS);
    CREDO_AEQUALIS_I32(l->aperta, II);
    CREDO_LITERAE_AEQUALES(l->acta, "1");
    CREDO_FALSUM(campus_est(&l->m.ramus, "visus", "negotium"));
}

/* sine porta: visus in loco, origo index; Esc ad indicem */
interior vacuum
_probare_sine_porta (
    CredoContextus* c)
{
    Mundus* w = (Mundus*)c->datum;
     Latus* l;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l = latus_montare(c, w, "c1", NIHIL, FALSUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(ictus(l, "r1", EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "negotium"));
    CREDO_VERUM(campus_est(&l->m.ramus, "negotium", "2"));
    CREDO_VERUM(campus_est(&l->m.ramus, "origo", "index"));
    CREDO_VERUM(locus_est(l, "alpha", c->piscina));
    CREDO_VERUM(textus_pictus(reddere(w, l, c->piscina),
        "bibliotheca"));
    clavis_specialis(l, (s32)CLAVIS_EFFUGIUM);
    CREDO_FALSUM(campus_est(&l->m.ramus, "visus", "negotium"));
    CREDO_VERUM(locus_est(l, "Projects", c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "electa", "2"));
}

/* argumentum: facultates cum globulis; Esc non consumitur (latus prius
 * per vicum); URI; ignotum refutatur */
interior vacuum
_probare_negotium (
    CredoContextus* c)
{
     Mundus* w = (Mundus*)c->datum;
      Latus* l;
    Mandata* md;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l = latus_montare(c, w, "c1", "2", VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "negotium"));
    CREDO_VERUM(campus_est(&l->m.ramus, "negotium", "2"));
    CREDO_VERUM(adest(l, "r1", c->piscina));
    CREDO_VERUM(adest(l, "r2", c->piscina));
    CREDO_FALSUM(adest(l, "r3", c->piscina));
    md = reddere(w, l, c->piscina);
    CREDO_VERUM(textus_pictus(md, "alpha"));
    CREDO_VERUM(textus_pictus(md, "bibliotheca"));
    CREDO_VERUM(textus_pictus(md, "tabula"));
    CREDO_VERUM(textus_pictus(md, "shipped"));
    CREDO_VERUM(textus_pictus(md, "planned"));
    CREDO_VERUM(locus_est(l, "alpha", c->piscina));

    CREDO_GRADUS("Esc ab argumento: non consumitur");
    clavis_specialis(l, (s32)CLAVIS_EFFUGIUM);
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "negotium"));

    CREDO_GRADUS("ignotum refutatur, nihil mutatum");
    CREDO_FALSUM(negotia_visus_argumentum_ponere(&l->m, "ZZ"));
    CREDO_VERUM(campus_est(&l->m.ramus, "negotium", "2"));

    CREDO_GRADUS("URI");
    CREDO_VERUM(negotia_visus_argumentum_ponere(&l->m,
        "negotia/negotium/1"));
    CREDO_VERUM(campus_est(&l->m.ramus, "negotium", "1"));
    CREDO_VERUM(negotia_visus_argumentum_ponere(&l->m, ""));
    CREDO_FALSUM(campus_est(&l->m.ramus, "visus", "negotium"));
}

interior vacuum
_probare_negotium_vacuum (
    CredoContextus* c)
{
    Mundus* w = (Mundus*)c->datum;
     Latus* l;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l = latus_montare(c, w, "c1", "1", VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_FALSUM(adest(l, "r1", c->piscina));
    CREDO_VERUM(textus_pictus(reddere(w, l, c->piscina),
        "No features yet - add one with `negotia facultas`"));
}

/* motus super r2: campus super et linea illuminata; motus alibi
 * (exitus) delet */
interior vacuum
_probare_super (
    CredoContextus* c)
{
    Mundus* w = (Mundus*)c->datum;
     Latus* l;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l = latus_montare(c, w, "c1", NIHIL, VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_FALSUM(super_pictum(reddere(w, l, c->piscina)));
    CREDO_VERUM(ictus(l, "r2", EVENTUS_MUS_MOTUS, c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "super", "r2"));
    CREDO_VERUM(super_pictum(reddere(w, l, c->piscina)));
    CREDO_VERUM(ictus(l, "titulus", EVENTUS_MUS_MOTUS, c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "super", NIHIL));
    CREDO_FALSUM(super_pictum(reddere(w, l, c->piscina)));
}

/* scriptor alius (manubrium secundum): pulsus mutatum reddit et
 * ordinem novum ostendit; latus alterum idem videt */
interior vacuum
_probare_renovatio (
    CredoContextus* c)
{
     Mundus* w = (Mundus*)c->datum;
      Latus* l;
      Latus* l2;
    Volumen* v2;
    Negotia* n2;
        i32  numerator_alter;
    Mandata* md;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l   = latus_montare(c, w, "c1", "2", VERUM, CCLVI);
    l2  = latus_montare(c, w, "c2", NIHIL, VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(l2);
    CREDO_FALSUM(negotia_visus_reficere(&l->m));
    CREDO_VERUM(negotia_visus_mora(&l->m) == (s64)M);

    v2 = volumen_aperire(c->piscina, volumen_via(w->volumen));
    CREDO_NECESSE_NON_NIHIL(v2);
    numerator_alter = C;
    n2 = negotia_aperire(v2, moneta_numerator, &numerator_alter,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(n2);
    CREDO_CHORDA_NON_VACUA(negotia_facultatem_creare(n2, lit("2",
        c->piscina), lit("nova", c->piscina), NEGOTIA_INCHOATA,
        lit("", c->piscina)));

    CREDO_VERUM(negotia_visus_reficere(&l->m));
    dispensator_recomponere(l->d);
    CREDO_VERUM(adest(l, "r3", c->piscina));
    md = reddere(w, l, c->piscina);
    CREDO_VERUM(textus_pictus(md, "nova"));
    CREDO_VERUM(textus_pictus(md, "partial"));
    CREDO_FALSUM(negotia_visus_reficere(&l->m));

    CREDO_GRADUS("latus alterum mutationem videt");
    CREDO_VERUM(negotia_visus_reficere(&l2->m));
    dispensator_recomponere(l2->d);
    CREDO_VERUM(textus_pictus(reddere(w, l2, c->piscina),
        "1 shipped \302\267 1 partial \302\267 1 planned"));
    volumen_claudere(v2);
}

/* altitudo CXX: II ordines indicis per paginam; ] et sagitta */
interior vacuum
_probare_paginas (
    CredoContextus* c)
{
       Mundus* w = (Mundus*)c->datum;
        Latus* l;
          i32  k;
    character  titulus[XVI];

    CREDO_NECESSE_NON_NIHIL(w->n);
    per (k = ZEPHYRUM; k < V; k++)
    {
        sprintf(titulus, "n%u", (insignatus integer)k);
        (vacuum)negotia_negotium_creare(w->n, lit(titulus, c->piscina),
            lit("", c->piscina));
    }
    l = latus_montare(c, w, "c1", NIHIL, VERUM, CXX);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(adest(l, "r2", c->piscina));
    CREDO_FALSUM(adest(l, "r3", c->piscina));
    CREDO_VERUM(textus_pictus(reddere(w, l, c->piscina), "1/3"));
    clavis(l, ']');
    CREDO_VERUM(campus_est(&l->m.ramus, "pagina", "1"));
    CREDO_VERUM(adest(l, "r3", c->piscina));
    CREDO_VERUM(ictus(l, "pagina-proxima", EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "pagina", "2"));
    CREDO_VERUM(adest(l, "r5", c->piscina));
}

/* ictus in facultatem: visus facultas, corpus et historia; Enter
 * etiam aperit */
interior vacuum
_probare_facultatem (
    CredoContextus* c)
{
     Mundus* w = (Mundus*)c->datum;
      Latus* l;
    Mandata* md;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l = latus_montare(c, w, "c1", "2", VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(ictus(l, "r1", EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "facultas"));
    CREDO_VERUM(campus_est(&l->m.ramus, "facultas", "3"));
    CREDO_VERUM(campus_est(&l->m.ramus, "negotium", "2"));
    md = reddere(w, l, c->piscina);
    CREDO_VERUM(textus_pictus(md, "bibliotheca"));
    CREDO_VERUM(textus_pictus(md, "shipped"));
    CREDO_VERUM(textus_pictus(md, "acta in volumine"));
    CREDO_VERUM(textus_pictus(md, "History"));
    CREDO_VERUM(locus_est(l, "alpha \302\267 bibliotheca", c->piscina));

    CREDO_GRADUS("Enter in facultate electa aperit");
    l = latus_montare(c, w, "c2", "2", VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    clavis(l, 'j');
    clavis_specialis(l, (s32)CLAVIS_REDITUS);
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "facultas"));
    CREDO_VERUM(campus_est(&l->m.ramus, "facultas", "4"));
    CREDO_VERUM(textus_pictus(reddere(w, l, c->piscina), "tabula"));
}

/* III revisiones: III lineae historiae, recentissima prima */
interior vacuum
_probare_historiam (
    CredoContextus* c)
{
     Mundus* w = (Mundus*)c->datum;
      Latus* l;
    Mandata* md;
        s32  y1;
        s32  y2;
        s32  y3;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    CREDO_VERUM(negotia_facultatem_mutare(w->n, lit("3", c->piscina),
        lit("bibliotheca", c->piscina), NEGOTIA_INCHOATA, lit("secunda",
        c->piscina)));
    CREDO_VERUM(negotia_facultatem_mutare(w->n, lit("3", c->piscina),
        lit("bibliotheca", c->piscina), NEGOTIA_PERFECTA, lit(
        "tertia\nlinea altera", c->piscina)));
    l = latus_montare(c, w, "c1", "negotia/facultas/2/3", VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "facultas"));
    md = reddere(w, l, c->piscina);
    y1 = textus_y(md, " \302\267 shipped \302\267 tertia");
    y2 = textus_y(md, " \302\267 partial \302\267 secunda");
    /* CCLXXX pixela: linea praecisa post "acta in" */
    y3 = textus_y(md, " \302\267 shipped \302\267 acta in");
    CREDO_VERUM(y1 >= ZEPHYRUM);
    CREDO_VERUM(y2 > y1);
    CREDO_VERUM(y3 > y2);
    /* linea prima corporis sola in historia */
    CREDO_VERUM(textus_y(md, "\302\267 shipped \302\267 linea altera")
        < ZEPHYRUM);
}

/* Esc: ad facultates, electa = facultas; Esc iterum non consumitur
 * (ab argumento) */
interior vacuum
_probare_redire (
    CredoContextus* c)
{
    Mundus* w = (Mundus*)c->datum;
     Latus* l;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    l = latus_montare(c, w, "c1", "2", VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(ictus(l, "r2", EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "facultas"));
    clavis_specialis(l, (s32)CLAVIS_EFFUGIUM);
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "negotium"));
    CREDO_VERUM(campus_est(&l->m.ramus, "facultas", NIHIL));
    CREDO_VERUM(campus_est(&l->m.ramus, "electa", "4"));
    CREDO_VERUM(locus_est(l, "alpha", c->piscina));
    clavis_specialis(l, (s32)CLAVIS_EFFUGIUM);
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "negotium"));
}

/* corpus CC linearum: pagina prima intra latus, sagitta, ] paginam
 * secundam */
interior vacuum
_probare_corpus_longum (
    CredoContextus* c)
{
               Mundus* w = (Mundus*)c->datum;
                Latus* l;
              Mandata* md;
    ChordaAedificator* a;
               chorda  f;
                  i32  k;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    a = chorda_aedificator_creare(c->piscina, MMXLVIII);
    per (k = ZEPHYRUM; k < CC; k++)
    {
        (vacuum)chorda_aedificator_appendere_literis(a, "linea ");
        (vacuum)chorda_aedificator_appendere_i32(a, k);
        (vacuum)chorda_aedificator_appendere_character(a, '\n');
    }
    f = negotia_facultatem_creare(w->n, lit("2", c->piscina), lit(
        "longa", c->piscina), NEGOTIA_PROPOSITA,
        chorda_aedificator_spectare(a));
    CREDO_CHORDA_NON_VACUA(f);
    l = latus_montare(c, w, "c1", "2", VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(ictus(l, "r3", EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "visus", "facultas"));
    md = reddere(w, l, c->piscina);
    CREDO_VERUM(textus_pictus(md, "linea 0"));
    CREDO_FALSUM(textus_pictus(md, "linea 199"));
    CREDO_VERUM(textus_intra(md, CCLVI));
    CREDO_VERUM(adest(l, "pagina-proxima", c->piscina));
    clavis(l, ']');
    CREDO_VERUM(campus_est(&l->m.ramus, "pagina_textus", "1"));
    md = reddere(w, l, c->piscina);
    CREDO_FALSUM(textus_pictus(md, "linea 0"));
    CREDO_VERUM(textus_intra(md, CCLVI));
    CREDO_VERUM(ictus(l, "pagina-proxima", EVENTUS_MUS_DEPRESSUS,
        c->piscina));
    CREDO_VERUM(campus_est(&l->m.ramus, "pagina_textus", "2"));
}

/* titulus UTF-8 longus in ordine: praecisus per characteres, numquam
 * character dimidius */
interior vacuum
_probare_praecisionem (
    CredoContextus* c)
{
               Mundus* w = (Mundus*)c->datum;
                Latus* l;
              Mandata* md;
    ChordaAedificator* a;
    constans Mandatum* x;
                  i32  k;
                  b32  inventum = FALSUM;

    CREDO_NECESSE_NON_NIHIL(w->n);
    seminare(w, c->piscina);
    a = chorda_aedificator_creare(c->piscina, CC);
    per (k = ZEPHYRUM; k < LX; k++)
    {
        (vacuum)chorda_aedificator_appendere_literis(a, "\303\251");
    }
    CREDO_CHORDA_NON_VACUA(negotia_facultatem_creare(w->n, lit("2",
        c->piscina), chorda_aedificator_spectare(a), NEGOTIA_PROPOSITA,
        lit("", c->piscina)));
    l = latus_montare(c, w, "c1", "2", VERUM, CCLVI);
    CREDO_NECESSE_NON_NIHIL(l);
    md = reddere(w, l, c->piscina);
    per (k = ZEPHYRUM; k < xar_numerus(md->elementa); k++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, k);
        si (   x->genus == MANDATUM_TEXTUS && x->textus.mensura >= II
            && x->textus.datum[ZEPHYRUM] == (i8)0xC3)
        {
            inventum = VERUM;
            CREDO_AEQUALIS_I32(x->textus.mensura % II, ZEPHYRUM);
            CREDO_VERUM(x->textus.mensura >= L);
            CREDO_VERUM(x->textus.mensura < CXX);
        }
    }
    CREDO_VERUM(inventum);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "index", _probare_indicem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "index vacuus", _probare_indicem_vacuum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "aperire", _probare_aperire, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "sine porta", _probare_sine_porta, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "negotium", _probare_negotium, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "negotium vacuum", _probare_negotium_vacuum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "super", _probare_super, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "renovatio", _probare_renovatio, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "paginae", _probare_paginas, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "facultas", _probare_facultatem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "historia", _probare_historiam, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "redire", _probare_redire, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "corpus longum", _probare_corpus_longum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "praecisio", _probare_praecisionem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("negotia_visus", SECTIONES);
}
