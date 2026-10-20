/* probatio_colloquia_visus.c - visus sessionum (colloquia-plan T5).
 * Fons: cauda libri fixi (IV sessiones), tituli (transcriptum aut
 * directorium), pulsus cum eventu novo. Visus: greges ("Needs you",
 * "Idle"; "Closed" solum cum 'c'), caput, j k inter greges, Enter et
 * ictus duplex (porta), electa stabilis dum ordo mutatur, pictura. */
#include "postulata_posix.h"
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
#include "processus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

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
 * (latitudo x CCCLX; ordinarie CCLXXX) */
interior Mundus*
mundus_latum (
     CredoContextus* c,
                i32  latitudo)
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
               w->repo, "v1", &w->fons, porta_ficta, w, latitudo, CCCLX,
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

interior Mundus*
mundus (
    CredoContextus* c)
{
    redde mundus_latum(c, CCLXXX);
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

/* T10b: mensor fictus - scriptum quod lineae.tsv (iuxta se) reddit,
 * post moram secundarum datam; codex exitus datus */
interior constans character*
mensor_fictus (
     CredoContextus* c,
                i32  mora,
                i32  codex)
{
    constans character* argumenta[IV];
             character* via;
             character  corpus[CCLVI];

    via = (character*)piscina_allocare(c->piscina, (memoriae_index)
        strlen(credo_area(c)) + XXXII);
    sprintf(via, "%s/mensor_%d_%d", credo_area(c), (integer)mora,
        (integer)codex);
    sprintf(corpus, "#!/bin/sh\n[ \"$1 $2 $3\" = \"quaestio currentes "
        "-machina\" ] || exit 9\nsleep %d\ncat \"$(dirname \"$0\")/"
        "lineae.tsv\"\nexit %d\n", (integer)mora, (integer)codex);
    si (!filum_scribere(via, chorda_ex_literis(corpus, c->piscina)))
    {
        redde NIHIL;
    }
    argumenta[0]    = "/bin/chmod";
    argumenta[I]    = "+x";
    argumenta[II]   = via;
    argumenta[III]  = NIHIL;
    (vacuum)processus_exsequi(argumenta, V * M, c->piscina);
    redde via;
}

interior b32
lineas_ponere (
         CredoContextus* c,
     constans character* lineae)
{
    character via[DXII];

    sprintf(via, "%s/lineae.tsv", credo_area(c));
    redde filum_scribere(via, chorda_ex_literis(lineae, c->piscina));
}

/* pulsare (nunc fixo) donec quaestio messa; VERUM si pulsus ullus
 * mutatum reddidit */
interior b32
messem_exspectare (
    ColloquiorumFons* f,
                 s64  nunc)
{
    structura timespec  mora;
                   b32 mutatum = FALSUM;
                   i32 k;

    mora.tv_sec   = ZEPHYRUM;
    mora.tv_nsec  = X * M * M;
    per (k = ZEPHYRUM; k < D; k++)
    {
        si (colloquia_visus_fontem_pulsare(f, nunc))
        {
            mutatum = VERUM;
        }
        si (f->quaestio == NIHIL)
        {
            frange;
        }
        (vacuum)nanosleep(&mora, NIHIL);
    }
    redde mutatum;
}

interior constans CursusCurrensLinea*
cursus_k (
    constans ColloquiorumFons* f,
                          i32  k)
{
    constans Xar* l = colloquia_visus_cursus(f);

    redde (l && k < xar_numerus(l))
        ? (constans CursusCurrensLinea*)xar_obtinere(l, k) : NIHIL;
}

#define LINEAE_I \
    "CURRENS\t0\tr1\t\tcommissio\t/w/secunda\trhubarb-secunda\t1000\t" \
    "4242\tvivus\t1\t0\t5\t0\t0\t7000\t595000\tsess-a\t\n" \
    "CURRENS\t1\tp1\tr1\tporta:radix\t/w/secunda\trhubarb-secunda\t" \
    "1100\t4242\tvivus\t0\t0\t0\t120\t351\t74000\t96000\tsess-a\t\n" \
    "CURRENS\t0\tr2\t\tcommissio\t/w/quinta\tquinta\t900\t77\t" \
    "mortuus\t0\t0\t0\t0\t0\t0\t0\t\t\n" \
    "CURRENS\t1\tp2\tr2\tporta:aedilis\t/w/quinta\tquinta\t950\t77\t" \
    "mortuus\t0\t0\t0\t0\t0\t0\t0\t\t\n" \
    "CURRENS\t0\tr3\t\tcommissio\t/w/tertia\ttertia\t800\t0\t" \
    "ignotus\t0\t0\t0\t0\t0\t0\t0\t\t\n" \
    "linea aliena\n"

/* CURSUS (colloquia T10b): mensor nullus -> NIHIL; quaestio in processu
 * filio, pulsus non obstat; radices vivae solae cum filiis; V s inter
 * quaestiones; mutatio -> VERUM; mensor fractus -> instantanea vetus */
interior vacuum
_probare_cursus (
    CredoContextus* c)
{
                 Mundus* w = mundus(c);
     constans character* lentus;
     constans character* fractus;
                    s64  nunc = (s64)C * (s64)M;
                clock_t  ante;
                    b32  mutatum;

    CREDO_NECESSE(w != NIHIL);
    /* I. sine mensore: nihil */
    (vacuum)colloquia_visus_fontem_pulsare(&w->fons, nunc);
    CREDO_VERUM(colloquia_visus_cursus(&w->fons) == NIHIL);
    CREDO_VERUM(w->fons.quaestio == NIHIL);

    /* II. quaestio prima: incipitur, messa, vivae solae */
    CREDO_NECESSE(lineas_ponere(c, LINEAE_I));
    colloquia_visus_fontem_mensorem_ponere(&w->fons, mensor_fictus(c,
        ZEPHYRUM, ZEPHYRUM));
    mutatum = messem_exspectare(&w->fons, nunc);
    CREDO_VERUM(mutatum);
    CREDO_NECESSE(colloquia_visus_cursus(&w->fons) != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(colloquia_visus_cursus(&w->fons)),
        II);
    CREDO_CHORDA_AEQUALIS_LITERIS(cursus_k(&w->fons, ZEPHYRUM)->id,
        "r1");
    CREDO_CHORDA_AEQUALIS_LITERIS(cursus_k(&w->fons,
        ZEPHYRUM)->colloquium,
        "sess-a");
    CREDO_CHORDA_AEQUALIS_LITERIS(cursus_k(&w->fons, I)->titulus,
        "porta:radix");
    CREDO_AEQUALIS_I32(cursus_k(&w->fons, I)->iudicata, CXX);

    /* III. intra V s: nulla quaestio nova, lineae mutatae non visae */
    CREDO_NECESSE(lineas_ponere(c,
        "CURRENS\t0\tr9\t\tcommissio\t/w/s\ts"
        "\t1\t1\tvivus\t0\t0\t0\t0\t0\t0\t0\tsess-b\t\n"));
    CREDO_FALSUM(colloquia_visus_fontem_pulsare(&w->fons,
        nunc + (s64)IV * (s64)M));
    CREDO_VERUM(w->fons.quaestio == NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(cursus_k(&w->fons, ZEPHYRUM)->id,
        "r1");

    /* IV. post V s: quaestio nova, mutatio VERUM */
    mutatum = messem_exspectare(&w->fons, nunc + (s64)V * (s64)M);
    CREDO_VERUM(mutatum);
    CREDO_AEQUALIS_I32(xar_numerus(colloquia_visus_cursus(&w->fons)),
        I);
    CREDO_CHORDA_AEQUALIS_LITERIS(cursus_k(&w->fons, ZEPHYRUM)->id,
        "r9");
    /* eadem iterum: non mutatum */
    mutatum = messem_exspectare(&w->fons, nunc + (s64)X * (s64)M);
    CREDO_FALSUM(mutatum);

    /* V. mensor lentus (II s): pulsus non obstat */
    lentus = mensor_fictus(c, II, ZEPHYRUM);
    colloquia_visus_fontem_mensorem_ponere(&w->fons, lentus);
    ante = clock();
    (vacuum)colloquia_visus_fontem_pulsare(&w->fons,
        nunc + (s64)XX * (s64)M);
    (vacuum)colloquia_visus_fontem_pulsare(&w->fons,
        nunc + (s64)XX * (s64)M);
    CREDO_VERUM(w->fons.quaestio != NIHIL);
    CREDO_VERUM((clock() - ante) < (clock_t)(CLOCKS_PER_SEC / II));
    (vacuum)messem_exspectare(&w->fons, nunc + (s64)XX * (s64)M);
    CREDO_VERUM(w->fons.quaestio == NIHIL);

    /* VI. mensor fractus (exitus I): instantanea vetus servatur */
    fractus = mensor_fictus(c, ZEPHYRUM, I);
    colloquia_visus_fontem_mensorem_ponere(&w->fons, fractus);
    CREDO_NECESSE(lineas_ponere(c,
        "CURRENS\t0\tr7\t\tcommissio\t/w/s\ts"
        "\t1\t1\tvivus\t0\t0\t0\t0\t0\t0\t0\t\t\n"));
    (vacuum)messem_exspectare(&w->fons, nunc + (s64)XXX * (s64)M);
    CREDO_NECESSE(colloquia_visus_cursus(&w->fons) != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(cursus_k(&w->fons, ZEPHYRUM)->id,
        "r9");
}

/* T10c: cursus ficti (nunc_fictum = CCCXX ms + V min): r1 sub s-a cum
 * porta aperta; r2 sine sessione cum fracto; r3 sessionis ignotae
 * (s-zz); r4 sub s-b, arbor III graduum cum filio fracto */
#define LINEAE_T10C \
    "CURRENS\t0\tr1\t\tcommissio\t/w/rhubarb-secunda\trhubarb-secunda" \
    "\t200320\t4242\tvivus\t2\t0\t7\t0\t0\t7000\t595000\ts-a\t\n" \
    "CURRENS\t1\tp1\tr1\tporta:radix\t/w/rhubarb-secunda\t" \
    "rhubarb-secunda\t230320\t4242\tvivus\t3\t0\t0\t120\t351\t74000" \
    "\t96000\ts-a\t\n" \
    "CURRENS\t0\tr2\t\tcommissio\t/w/rhubarb-quinta\trhubarb-quinta" \
    "\t100320\t77\tvivus\t4\t1\t9\t0\t0\t0\t0\t\t" \
    "probationes_radicis/probatio_git\n" \
    "CURRENS\t0\tr3\t\tporta:aedilis\t/w/rhubarb-tertia\trhubarb-tertia" \
    "\t290320\t88\tvivus\t0\t0\t0\t0\t0\t5000\t251000\ts-zz\t\n" \
    "CURRENS\t0\tr4\t\tcommissio\t/w/rhubarb-secunda\trhubarb-secunda" \
    "\t280320\t4242\tvivus\t0\t0\t0\t0\t0\t0\t0\ts-b\t\n" \
    "CURRENS\t1\tp4\tr4\tporta:pythonica\t/w/rhubarb-secunda\t" \
    "rhubarb-secunda\t290320\t4242\tvivus\t1\t1\t0\t0\t0\t0\t0\ts-b\t" \
    "pythonica#x\n" \
    "CURRENS\t2\tq4\tp4\tpythonica#umbra\t/w/rhubarb-secunda\t" \
    "rhubarb-secunda\t295320\t4242\tvivus\t0\t0\t0\t0\t0\t0\t0\ts-b" \
    "\t\n"

/* Xar de CursusCurrensLinea ex lineis (ut fons legit) */
interior Xar*
cursus_ficti (
    Piscina* piscina)
{
                    Xar* l;
              character* lineae;
                 chorda  linea;
                    i32  initium = ZEPHYRUM;
                    i32  k;
                    i32  n = (i32)strlen(LINEAE_T10C);

    /* copia mutabilis: chordae lineae in eam spectant */
    lineae = (character*)piscina_allocare(piscina, (memoriae_index)n
        + I);
    si (!lineae)
    {
        redde NIHIL;
    }
    strcpy(lineae, LINEAE_T10C);
    l = xar_creare(piscina, (i32)magnitudo(CursusCurrensLinea));
    per (k = ZEPHYRUM; l && k < n; k++)
    {
        si (lineae[k] != '\n')
        {
            perge;
        }
        linea          = chorda_ex_literis("", piscina);
        linea.datum    = (i8*)(lineae + initium);
        linea.mensura  = k - initium;
        initium        = k + I;
        si (!cursus_currentem_ex_linea(linea,
            (CursusCurrensLinea*)xar_addere(l)))
        {
            redde NIHIL;
        }
    }
    redde l;
}

#define NUNC_T10C ((s64)CCCXX + (s64)V * (s64)LX * (s64)M)

/* linea cursus (T10c): radix + filius profundissimus apertus, portae,
 * progressus membrorum, historia, fracta totius arboris; filius et
 * index extra -> vacua */
interior vacuum
_probare_cursus_textum (
    CredoContextus* c)
{
    Xar* l = cursus_ficti(c->piscina);

    CREDO_NECESSE(l != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(l), VII);
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquia_visus_cursus_textus(l,
        ZEPHYRUM, NUNC_T10C, c->piscina), "commit rhubarb-secunda 1m40s"
        " | gates 2/7 | radix 1m10s 120/351 (1m14s-1m36s)");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquia_visus_cursus_textus(l, II,
        NUNC_T10C, c->piscina),
        "commit rhubarb-quinta 3m20s | gates 4/9"
        " | failed: probationes_radicis/probatio_git");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquia_visus_cursus_textus(l, III,
        NUNC_T10C, c->piscina),
        "aedilis rhubarb-tertia 10s (5s-4m11s)");
    CREDO_CHORDA_AEQUALIS_LITERIS(colloquia_visus_cursus_textus(l, IV,
        NUNC_T10C, c->piscina), "commit rhubarb-secunda 20s | "
        "pythonica#umbra 5s | failed: pythonica#x");
    CREDO_AEQUALIS_I32(colloquia_visus_cursus_textus(l, I, NUNC_T10C,
        c->piscina).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(colloquia_visus_cursus_textus(l, VII, NUNC_T10C,
        c->piscina).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(colloquia_visus_cursus_textus(NIHIL, ZEPHYRUM,
        NUNC_T10C, c->piscina).mensura, ZEPHYRUM);
}

/* altitudo ordinis id dati in arbore (-1 si absens) */
interior s32
altitudo_ordinis (
                 Mundus* w,
     constans character* id)
{
     Componens* radix;
     Componens* f;
           i32  i;

    dispensator_recomponere(w->d);
    radix = dispensator_arbor(w->d);
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
         i++)
    {
        f = componens_liberum(radix, i);
        si (chorda_aequalis_literis(f->id, id))
        {
            redde f->fines.altitudo;
        }
    }
    redde -I;
}

/* partes ordinis id dati in arbore (-1 si absens) */
interior s32
partes_ordinis (
                 Mundus* w,
     constans character* id)
{
     Componens* radix;
     Componens* f;
           i32  i;

    dispensator_recomponere(w->d);
    radix = dispensator_arbor(w->d);
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
         i++)
    {
        f = componens_liberum(radix, i);
        si (chorda_aequalis_literis(f->id, id))
        {
            redde (s32)f->partes;
        }
    }
    redde -I;
}

/* id ordinis "r<n>" sessionis datae */
interior constans character*
ordo_sessionis (
                Mundus* w,
    constans character* sessio,
               Piscina* piscina)
{
          Xar* l   = colloquia_visus_fontis(&w->fons, piscina);
    character* id  = (character*)piscina_allocare(piscina, XVI);
          i32  i;

    per (i = ZEPHYRUM; l && id && i < xar_numerus(l); i++)
    {
        si (chorda_aequalis_literis(((constans Colloquium*)xar_obtinere(l,
            i))->sessio, sessio))
        {
            sprintf(id, "r%d", (integer)(i + I));
            redde id;
        }
    }
    redde "";
}

/* ordines cum cursibus (T10c): grex "Other runs" (g5) cum radicibus
 * sine sessione nota (c<n>); ordo sessionis cum cursu altior; pictura
 * latae superficiei: linea cursus et "failed:" separatim; j numquam
 * in c<n> */
interior vacuum
_probare_cursus_ordines (
    CredoContextus* c)
{
                Mundus* w = mundus_latum(c, DCCC);
               Mandata* md;
    constans character* sb;
    constans character* sc;
                   i32  k;

    CREDO_NECESSE(w != NIHIL);
    w->fons.cursus = cursus_ficti(c->piscina);
    CREDO_NECESSE(w->fons.cursus != NIHIL);
    /* s-a clausa (Closed celatur): cursus eius in "Other runs" (c1) */
    CREDO_VERUM(ordines_sunt(w, "g0 r3 g3 r1 r4 g5 c1 c3 c4"));
    sb = ordo_sessionis(w, "s-b", c->piscina);
    sc = ordo_sessionis(w, "s-c", c->piscina);
    /* radix una (r4): linea una (II unitates = XVI px) altior - filii
     * (p4, q4) non numerantur */
    CREDO_AEQUALIS_S32(altitudo_ordinis(w, sb) - altitudo_ordinis(w,
        sc),
        XVI);
    md = reddere(w, c->piscina);
    CREDO_VERUM(textus_pictus(md, "Other runs"));
    CREDO_VERUM(textus_pictus(md,
        "commit rhubarb-secunda 1m40s | gates "
        "2/7 | radix 1m10s 120/351 (1m14s-1m36s)"));
    CREDO_VERUM(textus_pictus(md,
        "commit rhubarb-quinta 3m20s | gates 4/9"
        " | "));
    CREDO_VERUM(textus_pictus(md,
        "failed: probationes_radicis/probatio_git"));
    /* sub s-b: linea cursus et fracta separatim */
    CREDO_VERUM(textus_pictus(md, "commit rhubarb-secunda 20s | "
        "pythonica#umbra 5s | "));
    CREDO_VERUM(textus_pictus(md, "failed: pythonica#x"));
    /* clausae ostensae: s-a (r2) cursum suum sub se habet, non in g5 */
    clavis(w, 'c');
    CREDO_VERUM(ordines_sunt(w, "g0 r3 g3 r1 r4 g4 r2 g5 c3 c4"));
    CREDO_AEQUALIS_S32(altitudo_ordinis(w, "r2") - altitudo_ordinis(w,
        sc), XVI);
    clavis(w, 'c');
    /* fasciae cursuum eligi nequeunt (non PARTES_ITEM) */
    CREDO_VERUM(partes_ordinis(w, "c3") >= ZEPHYRUM
        && partes_ordinis(w, "c3") != (s32)PARTES_ITEM);
    CREDO_VERUM(partes_ordinis(w, "c4") != (s32)PARTES_ITEM);
    /* j ultra ultimam sessionem: electa in r4 (l[III]) manet, numquam
     * in c<n> */
    per (k = ZEPHYRUM; k < X; k++)
    {
        clavis(w, 'j');
    }
    {
        constans Colloquium* ultima =
            (constans Colloquium*)xar_obtinere(
            colloquia_visus_fontis(&w->fons, c->piscina), III);
                  character sessio[LXIV];

        sprintf(sessio, "%.*s", (integer)ultima->sessio.mensura,
            (constans character*)ultima->sessio.datum);
        CREDO_VERUM(campus_est(&w->m.ramus, "electa", sessio));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "fons", _probare_fontem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "greges", _probare_greges, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _probare_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stabilitas", _probare_stabilitatem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pictura", _probare_picturam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cursus", _probare_cursus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cursus textus", _probare_cursus_textum, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "cursus ordines", _probare_cursus_ordines, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("colloquia_visus", SECTIONES);
}
