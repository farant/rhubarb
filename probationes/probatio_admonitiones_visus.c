/* probatio_admonitiones_visus.c - centrum admonitionum et sequax
 * (colloquia-plan T4a). Sequax: index ex cauda libri fixi (ordo,
 * tituli ex transcriptis), eventus novi per pulsum (titulus ex
 * directorio, recentissima), linea dimidia exspectat, liber absens
 * postea creatus, lectae ad tempus. Centrum: ordines, titulus et caput,
 * j k, x X (campus radicis), Enter et ictus duplex (porta), color non
 * lectae, aetas, paginae [ ], electa stabilis dum novae adveniunt. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "filum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "praeconium.h"
#include "admonitiones.h"
#include "admonitiones_visus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_FIXI "probationes/fixa/colloquia/eventa.jsonl"


/* ==================================================
 * Sequax
 * ================================================== */

/* liber in area: copia fixi aut vacuus */
interior constans character*
librum_parare (
    CredoContextus* c,
               b32  copia)
{
     constans character* area;
              character* via;

    area = credo_area(c);
    si (!area)
    {
        redde NIHIL;
    }
    via = (character*)piscina_allocare(c->piscina, (memoriae_index)
        strlen(area) + XXXII);
    sprintf(via, "%s/eventa.jsonl", area);
    /* fixum linea mala sine '\n' finitur: liber verus semper lineis
     * integris (write unum) */
    si (   copia
        && (   !filum_scribere(via, filum_legere_totum(VIA_FIXI,
                   c->piscina))
            || !filum_appendere_firmiter(via, chorda_ex_literis("\n",
                   c->piscina))))
    {
        redde NIHIL;
    }
    redde via;
}

interior b32
appendere (
         CredoContextus* c,
     constans character* via,
     constans character* linea)
{
    redde filum_appendere_firmiter(via, chorda_ex_literis(linea,
        c->piscina));
}

/* initium: IV admonitiones ex fixo (permission 140, Stop 150, Stop
 * 205, StopFailure 220), recentissima primum, tituli ex transcriptis */
interior vacuum
_probare_initium (
    CredoContextus* c)
{
     constans character* via;
     AdmonitionumSequax  s;

    via = librum_parare(c, VERUM);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(admonitiones_sequacem_initiare(&s, c->piscina,
        admonitiones_creare(c->piscina, L), via, M * M));
    CREDO_AEQUALIS_I32(admonitiones_numerus(s.index), IV);
    CREDO_AEQUALIS_I32(admonitiones_non_lectae(s.index), IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(s.index, ZEPHYRUM)->id,
        "colloquia:s-b:220");
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(s.index, III)->id,
        "colloquia:s-a:140");
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(s.index, ZEPHYRUM)->titulus,
        "Log check session");
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(s.index, II)->titulus,
        "secunda");
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(s.index, II)->corpus,
        "Done: built.");
}

/* pulsus: nihil novi 0; nova (titulus ex directorio, transcriptum sine
 * titulo) et PreToolUse (non qualis) -> I, recentissima; linea
 * dimidia exspectat */
interior vacuum
_probare_sequi (
    CredoContextus* c)
{
     constans character* via;
     AdmonitionumSequax  s;
                 chorda  r;

    via = librum_parare(c, VERUM);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(admonitiones_sequacem_initiare(&s, c->piscina,
        admonitiones_creare(c->piscina, L), via, M * M));
    CREDO_VERUM(s.eventus == NIHIL);
    CREDO_AEQUALIS_I32(admonitiones_sequi(&s, &r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(r.mensura, ZEPHYRUM);
    /* eventus pulsus: nulli, sed tabula adest */
    CREDO_NECESSE(s.eventus != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(s.eventus), ZEPHYRUM);
    CREDO_NECESSE(appendere(c, via,
        "{\"tempus\":400,\"sessio\":\"s-c\","
        "\"eventus\":\"Notification\","
        "\"panellus\":\"%1\",\"servitor\":\"/tmp/tmux-501/default\","
        "\"via\":\"/w/gamma\",\"transcriptum\":"
        "\"probationes/fixa/colloquia/nullum.jsonl\",\"genus\":"
        "\"permission_prompt\",\"nuntius\":\"Claude needs your "
        "permission to use Edit\"}\n"
        "{\"tempus\":410,\"sessio\":\"s-c\",\"eventus\":\"PreToolUse\","
        "\"panellus\":\"%1\",\"servitor\":\"\",\"via\":\"/w/gamma\","
        "\"transcriptum\":\"\",\"genus\":\"Edit\","
        "\"nuntius\":\"x\"}\n"));
    CREDO_AEQUALIS_I32(admonitiones_sequi(&s, &r), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r, "colloquia:s-c:400");
    /* eventus OMNES lecti, etiam non qualis, ordine libri */
    CREDO_NECESSE(s.eventus != NIHIL
        && xar_numerus(s.eventus) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans PraeconiumEventus*)
        xar_obtinere(s.eventus, ZEPHYRUM))->eventus, "Notification");
    CREDO_CHORDA_AEQUALIS_LITERIS(((constans PraeconiumEventus*)
        xar_obtinere(s.eventus, I))->eventus, "PreToolUse");
    CREDO_AEQUALIS_I32(admonitiones_numerus(s.index), V);
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(s.index, ZEPHYRUM)->titulus,
        "gamma");
    CREDO_VERUM(admonitio(s.index, ZEPHYRUM)->gradus
        == ADMONITIO_ATTENTIO);
    /* linea dimidia */
    CREDO_NECESSE(appendere(c, via,
        "{\"tempus\":500,\"sessio\":\"s-b\",\"eventus\":\"Stop\","));
    CREDO_AEQUALIS_I32(admonitiones_sequi(&s, &r), ZEPHYRUM);
    CREDO_VERUM(s.eventus != NIHIL
        && xar_numerus(s.eventus) == ZEPHYRUM);
    CREDO_NECESSE(appendere(c, via,
        "\"panellus\":\"%2\",\"servitor\":\"\",\"via\":\"/w/beta\","
        "\"transcriptum\":\"\",\"genus\":\"\","
        "\"nuntius\":\"Merged.\"}\n"));
    CREDO_AEQUALIS_I32(admonitiones_sequi(&s, &r), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r, "colloquia:s-b:500");
    /* titulus s-b ex memoria (transcriptum huius eventus vacuum) */
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(s.index, ZEPHYRUM)->titulus,
        "Log check session");
}

/* liber absens: index vacuus; creatus postea legitur ab initio */
interior vacuum
_probare_absentem (
    CredoContextus* c)
{
     constans character* via;
     AdmonitionumSequax  s;
                 chorda  r;

    via = librum_parare(c, FALSUM);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(admonitiones_sequacem_initiare(&s, c->piscina,
        admonitiones_creare(c->piscina, L), via, M * M));
    CREDO_AEQUALIS_I32(admonitiones_numerus(s.index), ZEPHYRUM);
    CREDO_AEQUALIS_I32(admonitiones_sequi(&s, &r), ZEPHYRUM);
    CREDO_NECESSE(appendere(c, via,
        "{\"tempus\":700,\"sessio\":\"s-e\","
        "\"eventus\":\"StopFailure\","
        "\"panellus\":\"%4\",\"servitor\":\"\",\"via\":\"/w/epsilon\","
        "\"transcriptum\":\"\",\"genus\":\"\",\"nuntius\":\"API "
        "error\"}\n"));
    CREDO_NECESSE(admonitiones_sequi(&s, &r) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(admonitio(s.index, ZEPHYRUM)->titulus,
        "epsilon");
}

/* lectae ad tempus: 205 -> 205, 150, 140 (non 220) */
interior vacuum
_probare_lectas_ad (
    CredoContextus* c)
{
     constans character* via;
     AdmonitionumSequax  s;

    via = librum_parare(c, VERUM);
    CREDO_NECESSE(via != NIHIL);
    CREDO_NECESSE(admonitiones_sequacem_initiare(&s, c->piscina,
        admonitiones_creare(c->piscina, L), via, M * M));
    CREDO_AEQUALIS_I32(admonitiones_lectas_notare_ad(s.index, (s64)CCV),
        III);
    CREDO_AEQUALIS_I32(admonitiones_non_lectae(s.index), I);
    CREDO_FALSUM(admonitio(s.index, ZEPHYRUM)->lecta);
    CREDO_AEQUALIS_I32(admonitiones_lectas_notare_ad(s.index, (s64)CCV),
        ZEPHYRUM);
}


/* ==================================================
 * Centrum
 * ================================================== */

nomen structura {
      InternamentumChorda* intern;
       InsulaRepositorium* repo;
             Admonitiones* index;
     AdmonitionumMontatio  m;
              Dispensator* d;
                      i32  actae;
                character  acta[LXIV];
} Mundus;

interior b32
porta_ficta (
                vacuum* ctx,
    constans Admonitio* x)
{
    Mundus* w;

    w = (Mundus*)ctx;
    w->actae++;
    sprintf(w->acta, "%.*s", (integer)(x->id.mensura < LX
        ? x->id.mensura : LX), (constans character*)x->id.datum);
    redde VERUM;
}

/* nunc: 3000 ms + V minuta */
interior s64
nunc_fictum (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde (s64)MMM + (s64)V * (s64)LX * (s64)M;
}

interior vacuum
fictam_addere (
                Mundus* w,
    constans character* id,
                   s64  tempus,
    GradusAdmonitionis  gradus,
    constans character* titulus,
               Piscina* p)
{
    Admonitio x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    x.id       = chorda_ex_literis(id, p);
    x.fons     = chorda_ex_literis("probatio", p);
    x.titulus  = chorda_ex_literis(titulus, p);
    x.corpus   = chorda_ex_literis("corpus", p);
    x.tempus   = tempus;
    x.gradus   = gradus;
    (vacuum)admonitiones_addere(w->index, &x);
}

/* centrum CCLXXX x altitudo; n admonitiones fictae (i: tempus
 * (i+1)*1000, "t<i>"); prima ATTENTIO */
interior Mundus*
centrum (
    CredoContextus* c,
               i32  n,
               i32  altitudo)
{
       Mundus* w;
        Motus* motus;
          i32  i;
    character  id[XVI];
    character  titulus[XVI];

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->intern  = internamentum_creare(c->piscina);
    w->index   = admonitiones_creare(c->piscina, C);
    per (i = ZEPHYRUM; i < n; i++)
    {
        sprintf(id, "a%d", (integer)i);
        sprintf(titulus, "t%d", (integer)i);
        fictam_addere(w, id, (s64)(i + I) * (s64)M, i == ZEPHYRUM
            ? ADMONITIO_ATTENTIO : ADMONITIO_NUNTIUS, titulus,
            c->piscina);
    }
    w->repo = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !admonitiones_montare(&w->m, c->piscina, w->intern, w->repo,
               "c1", w->index, porta_ficta, w, CCLXXX, altitudo, NIHIL))
    {
        redde NIHIL;
    }
    w->m.nunc  = nunc_fictum;
    w->d       = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, admonitiones_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
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

interior Fines
fines_componentis (
                 Mundus* w,
     constans character* id,
                Piscina* p)
{
     Componens* c;
         Fines  f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    c = componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
    si (c)
    {
        f = c->fines;
    }
    redde f;
}

interior b32
ictus (
                 Mundus* w,
     constans character* id,
        eventus_genus_t  genus,
                Piscina* p)
{
      Fines f;
    Eventus e;

    f = fines_componentis(w, id, p);
    si (f.altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = genus;
    e.datum.mus.x       = f.x + f.latitudo / II;
    e.datum.mus.y       = f.y + f.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
    redde VERUM;
}

interior Mandata*
reddere (
     Mundus* w,
    Piscina* p)
{
    Mandata* md;

    md = mandata_creare(p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    redde md;
}

/* textus aequalis pictus; color < 0 = quivis */
interior b32
textus_pictus (
                 Mandata* md,
      constans character* textus,
                     s32  color)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_aequalis_literis(x->textus, textus)
            && (color < ZEPHYRUM
                || (   x->color.genus == COLOR_MANDATI_THEMA
                    && x->color.valor == (i32)color)))
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
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

/* ordines, titulus, caput, aetas, color non lectae */
interior vacuum
_probare_centrum (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;

    w = centrum(c, III, CCLVI);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(fines_componentis(w, "r1", c->piscina).altitudo
        > ZEPHYRUM);
    CREDO_VERUM(fines_componentis(w, "r3", c->piscina).altitudo
        > ZEPHYRUM);
    CREDO_VERUM(fines_componentis(w, "r4", c->piscina).altitudo
        == ZEPHYRUM);
    md = reddere(w, c->piscina);
    CREDO_VERUM(textus_pictus(md, "Notifications", -I));
    CREDO_VERUM(textus_pictus(md, "3 unread of 3", -I));
    /* recentissima primum: t2 (3000 ms), aetas V minuta */
    CREDO_VERUM(textus_pictus(md, "t2", (s32)COLOR_ACCENT_PRIMARY));
    CREDO_VERUM(textus_pictus(md, "5m", -I));
    CREDO_VERUM(textus_pictus(md, "corpus", -I));
    /* x: electa (prima) lecta - T4e: celatur, electa ad proximam */
    clavis(w, 'x');
    md = reddere(w, c->piscina);
    CREDO_FALSUM(textus_pictus(md, "t2", -I));
    CREDO_VERUM(textus_pictus(md, "t1", (s32)COLOR_ACCENT_PRIMARY));
    CREDO_VERUM(textus_pictus(md, "2 unread, 1 read (r)", -I));
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a1"));
    CREDO_VERUM(fines_componentis(w, "r1", c->piscina).altitudo
        == ZEPHYRUM);
    /* r: lectae ostenduntur (colore textus) */
    clavis(w, 'r');
    CREDO_VERUM(campus_est(&w->m.ramus, "lectae", "verum"));
    md = reddere(w, c->piscina);
    CREDO_VERUM(textus_pictus(md, "t2", (s32)COLOR_TEXT));
    CREDO_VERUM(textus_pictus(md, "2 unread of 3", -I));
    /* x in modo ostenso: lecta manet visibilis et electa */
    clavis(w, 'x');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a1"));
    clavis(w, 'r');
    md = reddere(w, c->piscina);
    CREDO_VERUM(textus_pictus(md, "1 unread, 2 read (r)", -I));
    /* visibilis sola a0 (r3); a1 a2 lectae celantur */
    CREDO_VERUM(fines_componentis(w, "r3", c->piscina).altitudo
        > ZEPHYRUM);
    CREDO_VERUM(fines_componentis(w, "r2", c->piscina).altitudo
        == ZEPHYRUM);
    /* electa servata (a1) celata est: Enter visibilem agit (a0) */
    reditus(w);
    CREDO_AEQUALIS_I32(w->actae, I);
    CREDO_VERUM(strcmp(w->acta, "a0") == ZEPHYRUM);
}

/* j k: electa per id; Enter porta et lecta; X omnes et campus radicis;
 * ictus duplex */
interior vacuum
_probare_claves (
    CredoContextus* c)
{
         Mundus* w;
    InsulaRamus  radix;

    w = centrum(c, III, CCLVI);
    CREDO_NECESSE(w != NIHIL);
    clavis(w, 'j');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a1"));
    clavis(w, 'j');
    clavis(w, 'j');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a0"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a1"));
    reditus(w);
    CREDO_AEQUALIS_I32(w->actae, I);
    CREDO_VERUM(strcmp(w->acta, "a1") == ZEPHYRUM);
    CREDO_AEQUALIS_I32(admonitiones_non_lectae(w->index), II);
    clavis(w, 'X');
    CREDO_AEQUALIS_I32(admonitiones_non_lectae(w->index), ZEPHYRUM);
    radix = insula_ramus_radix(w->repo);
    CREDO_VERUM(campus_est(&radix, "admonitiones_lectae_ad", "3000"));
    /* T4e: omnes celatae */
    CREDO_VERUM(fines_componentis(w, "r3", c->piscina).altitudo
        == ZEPHYRUM);
    CREDO_VERUM(textus_pictus(reddere(w, c->piscina),
        "No unread notifications", -I));
    clavis(w, 'r');
    /* ictus duplex in r3 (a0) */
    CREDO_VERUM(ictus(w, "r3", EVENTUS_MUS_DEPRESSUS, c->piscina));
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a0"));
    CREDO_VERUM(ictus(w, "r3", EVENTUS_MUS_DUPLEX, c->piscina));
    CREDO_AEQUALIS_I32(w->actae, II);
    CREDO_VERUM(strcmp(w->acta, "a0") == ZEPHYRUM);
}

/* paginae: altitudo CXX -> VIII cellulae = II ordines per paginam */
interior vacuum
_probare_paginas (
    CredoContextus* c)
{
     Mundus* w;
    Mandata* md;

    w = centrum(c, V, CXX);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(fines_componentis(w, "r2", c->piscina).altitudo
        > ZEPHYRUM);
    CREDO_VERUM(fines_componentis(w, "r3", c->piscina).altitudo
        == ZEPHYRUM);
    md = reddere(w, c->piscina);
    CREDO_VERUM(textus_pictus(md, "1/3", -I));
    clavis(w, ']');
    CREDO_VERUM(campus_est(&w->m.ramus, "pagina", "1"));
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a2"));
    CREDO_VERUM(fines_componentis(w, "r3", c->piscina).altitudo
        > ZEPHYRUM);
    CREDO_VERUM(fines_componentis(w, "r1", c->piscina).altitudo
        == ZEPHYRUM);
    clavis(w, '[');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a4"));
    /* j in fine paginae ad paginam proximam */
    clavis(w, 'j');
    clavis(w, 'j');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a2"));
    CREDO_VERUM(campus_est(&w->m.ramus, "pagina", "1"));
}

/* electa stabilis: nova recentior advenit, electa idem id manet */
interior vacuum
_probare_stabilitatem (
    CredoContextus* c)
{
    Mundus* w;

    w = centrum(c, III, CCLVI);
    CREDO_NECESSE(w != NIHIL);
    clavis(w, 'j');
    CREDO_VERUM(campus_est(&w->m.ramus, "electa", "a1"));
    fictam_addere(w, "a9", (s64)IX * (s64)M, ADMONITIO_NUNTIUS, "t9",
        c->piscina);
    dispensator_recomponere(w->d);
    reditus(w);
    CREDO_VERUM(strcmp(w->acta, "a1") == ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "initium", _probare_initium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sequi", _probare_sequi, NIHIL, NIHIL, NIHIL, NIHIL },
    { "absens", _probare_absentem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lectae_ad", _probare_lectas_ad, NIHIL, NIHIL, NIHIL, NIHIL },
    { "centrum", _probare_centrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _probare_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "paginae", _probare_paginas, NIHIL, NIHIL, NIHIL, NIHIL },
    { "stabilitas", _probare_stabilitatem, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("admonitiones_visus", SECTIONES);
}
