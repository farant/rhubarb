/* probatio_colloquia_visus.c - visus sessionum (colloquia-plan T5).
 * Fons: cauda libri fixi (IV sessiones), tituli (transcriptum aut
 * directorium), pulsus cum eventu novo. Visus: greges ("Needs you",
 * "Idle"; "Closed" solum cum 'c'), caput, j k inter greges, Enter et
 * ictus duplex (porta), electa stabilis dum ordo mutatur, pictura. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "filum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "colloquia.h"
#include "colloquia_visus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_FIXI "probationes/fixa/colloquia/eventa.jsonl"

nomen structura {
      InternamentumChorda* intern;
       InsulaRepositorium* repo;
         ColloquiorumFons  fons;
     ColloquiorumMontatio  m;
              Dispensator* d;
       constans character* via;
                      i32  actae;
                character  acta[LXIV];
} Mundus;

interior b32
porta_ficta (
                 vacuum* ctx,
    constans Colloquium* c)
{
    Mundus* w;

    w = (Mundus*)ctx;
    w->actae++;
    sprintf(w->acta, "%.*s", (integer)(c->sessio.mensura < LX
        ? c->sessio.mensura : LX),
        (constans character*)c->sessio.datum);
    redde VERUM;
}

/* nunc: 320 ms + V minuta (s-c "5m") */
interior s64
nunc_fictum (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde (s64)CCCXX + (s64)V * (s64)LX * (s64)M;
}

/* liber in area (copia fixi + '\n'); fons sine tmux; visus montatus
 * (CCLXXX x CCCLX) */
interior Mundus*
mundus (
    CredoContextus* c)
{
                 Mundus* w;
     constans character* area;
              character* via;
                  Motus* motus;

    thema_initiare();
    area  = credo_area(c);
    w     = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    si (!area || !w)
    {
        redde NIHIL;
    }
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    via = (character*)piscina_allocare(c->piscina, (memoriae_index)
        strlen(area) + XXXII);
    sprintf(via, "%s/eventa.jsonl", area);
    w->via = via;
    si (   !filum_scribere(via, filum_legere_totum(VIA_FIXI,
        c->piscina))
        || !filum_appendere_firmiter(via, chorda_ex_literis("\n",
               c->piscina))
        || !colloquia_visus_fontem_initiare(&w->fons, c->piscina, via,
               M * M, NIHIL))
    {
        redde NIHIL;
    }
    w->intern  = internamentum_creare(c->piscina);
    w->repo    = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !colloquia_visus_montare(&w->m, c->piscina, w->intern,
               w->repo, "v1", &w->fons, porta_ficta, w, CCLXXX, CCCLX,
               NIHIL))
    {
        redde NIHIL;
    }
    w->m.nunc  = nunc_fictum;
    w->d       = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, colloquia_visus_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
}

interior constans Colloquium*
colloquium (
                   Xar* l,
    constans character* sessio)
{
    constans Colloquium* k;
                    i32  i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        k = (constans Colloquium*)xar_obtinere(l, i);
        si (chorda_aequalis_literis(k->sessio, sessio))
        {
            redde k;
        }
    }
    redde NIHIL;
}

/* ordines (tituli gregum et res) ordine arboris */
interior b32
ordines_sunt (
                 Mundus* w,
     constans character* exspectati)
{
     Componens* radix;
     Componens* l;
     character  visi[CCLVI];
           i32  i;

    dispensator_recomponere(w->d);
    radix           = dispensator_arbor(w->d);
    visi[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
         i++)
    {
        l = componens_liberum(radix, i);
        si (   (l->partes != PARTES_INDEX && l->partes != PARTES_ITEM)
            || chorda_aequalis_literis(l->id, "caput")
            || chorda_aequalis_literis(l->id, "index_paginae"))
        {
            perge;
        }
        si (visi[ZEPHYRUM])
        {
            strcat(visi, " ");
        }
        strncat(visi, (constans character*)l->id.datum,
            (size_t)l->id.mensura);
    }
    si (strcmp(visi, exspectati) != ZEPHYRUM)
    {
        imprimere("  ordines: '%s'\n", visi);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
clavis (
       Mundus* w,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (s32)typus;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
reditus (
    Mundus* w)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = CLAVIS_REDITUS;
    e.datum.clavis.producta  = XIII;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior b32
ictus (
                 Mundus* w,
     constans character* id,
        eventus_genus_t  genus,
                Piscina* p)
{
     Componens* c;
       Eventus  e;

    dispensator_recomponere(w->d);
    c = componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
    si (!c)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = genus;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
    redde VERUM;
}

interior b32
campus_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

interior Mandata*
reddere (
     Mundus* w,
    Piscina* p)
{
    Mandata* md;

    dispensator_recomponere(w->d);
    md = mandata_creare(p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
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

/* polygonum plenum coloris dati? */
interior b32
signum_coloris (
       Mandata* md,
    ColorThema  color)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus       == MANDATUM_POLYGONUM && x->impletum
            && x->color.genus == COLOR_MANDATI_THEMA
            && x->color.valor == (i32)color)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* fons: cauda, tituli, pulsus cum eventu novo */
interior vacuum
_probare_fontem (
    CredoContextus* c)
{
                 Mundus* w;
                    Xar* l;
    constans Colloquium* k;

    w = mundus(c);
    CREDO_NECESSE(w != NIHIL);
    l = colloquia_visus_fontis(&w->fons, c->piscina);
    CREDO_NECESSE(l != NIHIL && xar_numerus(l) == IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquium(l, "s-a")->titulus,
        "secunda");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquium(l, "s-b")->titulus,
        "Log check session");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquium(l, "s-c")->titulus,
        "gamma");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquium(l, "s-d")->titulus,
        "delta");
    CREDO_FALSUM(colloquia_visus_fontem_pulsare(&w->fons, ZEPHYRUM));
    CREDO_NECESSE(filum_appendere_firmiter(w->via, chorda_ex_literis(
        "{\"tempus\":400,\"sessio\":\"s-d\",\"eventus\":"
        "\"UserPromptSubmit\",\"panellus\":\"%9\",\"servitor\":\"\","
        "\"via\":\"/w/delta\",\"transcriptum\":\"\",\"genus\":\"\","
        "\"nuntius\":\"new work\"}\n", c->piscina)));
    CREDO_VERUM(colloquia_visus_fontem_pulsare(&w->fons, ZEPHYRUM));
    l = colloquia_visus_fontis(&w->fons, c->piscina);
    k = colloquium(l, "s-d");
    CREDO_NECESSE(k != NIHIL);
    CREDO_AEQUALIS_S32((s32)k->status, (s32)COLLOQUIUM_LABORAT);
    CREDO_CHORDA_AEQUALIS_LITERIS(k->linea, "new work");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans Colloquium*)xar_obtinere(l,
        ZEPHYRUM))->sessio, "s-d");
}

/* greges: s-b te eget (r3), s-c s-d otiosae (r1 r4), s-a clausa (r2,
 * solum cum 'c') */
interior vacuum
_probare_greges (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;

    w = mundus(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(ordines_sunt(w, "g0 r3 g3 r1 r4"));
    md = reddere(w, c->piscina);
    CREDO_VERUM(textus_pictus(md, "Sessions"));
    CREDO_VERUM(textus_pictus(md, "3 active, 1 closed (c)"));
    clavis(w, 'c');
    CREDO_VERUM(campus_est(&w->m.ramus, "clausa", "verum"));
    CREDO_VERUM(ordines_sunt(w, "g0 r3 g3 r1 r4 g4 r2"));
    md = reddere(w, c->piscina);
    CREDO_VERUM(textus_pictus(md, "3 active"));
    clavis(w, 'c');
    CREDO_VERUM(ordines_sunt(w, "g0 r3 g3 r1 r4"));
}

/* j k inter greges (tituli praetereuntur); Enter et ictus duplex */
interior vacuum
_probare_claves (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c);
    CREDO_NECESSE(w != NIHIL);
    clavis(w, 'j');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "s-c"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "s-d"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "s-d"));
    clavis(w, 'k');
    clavis(w, 'k');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "s-b"));
    reditus(w);
    CREDO_AEQUALIS_I32(w->actae, I);
    CREDO_VERUM(strcmp(w->acta, "s-b") == ZEPHYRUM);
    CREDO_VERUM(ictus(w, "r4", EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "s-d"));
    CREDO_VERUM(ictus(w, "r4", EVENTUS_MUS_DUPLEX, c->piscina));
    CREDO_AEQUALIS_I32(w->actae, II);
    CREDO_VERUM(strcmp(w->acta, "s-d") == ZEPHYRUM);
}

/* electa stabilis: s-d electa, deinde laborat et prima fit - electa
 * manet s-d */
interior vacuum
_probare_stabilitatem (
    CredoContextus* c)
{
    Mundus* w;

    w = mundus(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(ictus(w, "r4", EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "s-d"));
    CREDO_NECESSE(filum_appendere_firmiter(w->via, chorda_ex_literis(
        "{\"tempus\":400,\"sessio\":\"s-d\",\"eventus\":"
        "\"UserPromptSubmit\",\"panellus\":\"%9\",\"servitor\":\"\","
        "\"via\":\"/w/delta\",\"transcriptum\":\"\",\"genus\":\"\","
        "\"nuntius\":\"new work\"}\n", c->piscina)));
    CREDO_VERUM(colloquia_visus_reficere(&w->m));
    /* s-d nunc r1 in grege Working */
    CREDO_VERUM(ordines_sunt(w, "g0 r4 g1 r1 g3 r2"));
    reditus(w);
    CREDO_VERUM(strcmp(w->acta, "s-d") == ZEPHYRUM);
}

/* pictura: titulus gregis, signum coloris (te eget), aetas */
interior vacuum
_probare_picturam (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;

    w = mundus(c);
    CREDO_NECESSE(w != NIHIL);
    md = reddere(w, c->piscina);
    CREDO_VERUM(textus_pictus(md, "Needs you"));
    CREDO_VERUM(textus_pictus(md, "Idle"));
    CREDO_FALSUM(textus_pictus(md, "Closed"));
    CREDO_VERUM(textus_pictus(md, "Log check session"));
    CREDO_VERUM(textus_pictus(md, "API error: overloaded"));
    CREDO_VERUM(textus_pictus(md, "testing C"));
    CREDO_VERUM(textus_pictus(md, "5m"));
    CREDO_VERUM(signum_coloris(md, COLOR_WARNING));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fons", _probare_fontem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "greges", _probare_greges, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _probare_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stabilitas", _probare_stabilitatem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pictura", _probare_picturam, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("colloquia_visus", SECTIONES);
}
