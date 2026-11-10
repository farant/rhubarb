/* probatio_membrum__delectus.c - delectus (membrum__delectus.h):
 * status (clausus, aperire, claudere, duo delectus - unus apertus),
 * claves (j / k, sagittae, Reditus eligit, Esc claudit, aliae
 * deglutiuntur), mus (motus illuminat, ictus in optione eligit,
 * obscura deglutitur, DIALOGUS manet, extra claudit), locus menu
 * (sub bottone; supra si area deest), botton (nodus et latitudo). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "mandatum.h"
#include "elenchus.h"
#include "membrum__delectus.h"
#include "credo.h"
#include <string.h>

hic_manens constans character scriptor[] = "probatio.agere";

/* tres optiones; media obscura in OPTIONES_OBSCURAE */
hic_manens constans ElenchusRes OPTIONES[III] = {
    { "walk", "walk", NIHIL, VERUM, FALSUM },
    { "throw", "boulder_throw", NIHIL, VERUM, FALSUM },
    { "idle", "idle", NIHIL, VERUM, FALSUM }
};

hic_manens constans ElenchusRes OPTIONES_OBSCURAE[III] = {
    { "walk", "walk", NIHIL, VERUM, FALSUM },
    { "throw", "boulder_throw", NIHIL, FALSUM, FALSUM },
    { "idle", "idle", NIHIL, VERUM, FALSUM }
};

nomen structura {
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
            InsulaRamus  ramus;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    x->intern  = internamentum_creare(c->piscina);
    x->repo    = insula_repositorium_creare(c->piscina, x->intern,
        "<hospes><latus id=\"t\"/></hospes>");
    si (x->repo)
    {
        x->ramus = insula_ramus(x->repo, "latus", "t");
    }
    c->datum = x;
}

/* optiones cum electa ex statu */
interior Elenchus
_optiones (
                    Mundus* x,
      constans ElenchusRes* res)
{
    Elenchus e;

    e.res      = res;
    e.numerus  = III;
    e.electa   = delectus_electa(&x->ramus, III);
    redde e;
}

interior Fines
_fines (
    s32 x,
    s32 y,
    s32 latitudo,
    s32 altitudo)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    redde f;
}

/* nodus optionis k (aut DIALOGUS si k < 0) */
interior Componens*
_nodus (
               Mundus* x,
    constans Elenchus* e,
                  s32  k,
              Piscina* p)
{
    Componens* r = delectus_componere(e, _fines(X, X, LX, XII),
        _fines(ZEPHYRUM, ZEPHYRUM, CCC, CCC), "a", p, x->intern);

    si (!r || k < ZEPHYRUM)
    {
        redde r;
    }
    redde componens_liberum(r, (i32)k);
}

interior DelectusExitus
_agere (
                  Mundus* x,
    constans ElenchusRes* res,
      constans Componens* nodus,
         eventus_genus_t  genus,
                clavis_t  codex,
                     s32  producta,
                     s32* electum,
                 Piscina* p)
{
     Eventus ev;
    Elenchus e = _optiones(x, res);

    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus             = genus;
    ev.datum.mus.botton  = MUS_SINISTER;
    si (genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        memset(&ev.datum, ZEPHYRUM, magnitudo(ev.datum));
        ev.datum.clavis.clavis    = codex;
        ev.datum.clavis.producta  = producta;
    }
    redde delectus_eventum_agere(&x->ramus, scriptor, &e, nodus, &ev,
        electum, p);
}

interior vacuum
_probare_statum (
    CredoContextus* c)
{
    Mundus* x = (Mundus*)c->datum;
    chorda* a;
       s32  electum = -VII;

    /* clausus: nihil apertum, eventus hospitis est */
    CREDO_FALSUM (delectus_apertus(&x->ramus, NIHIL));
    CREDO_FALSUM (delectus_apertus(&x->ramus, "cyclus"));
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), -I);
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, (clavis_t)'j', 'j', &electum,
        c->piscina), (s32)DELECTUS_NIHIL);
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_MUS_DEPRESSUS, ZEPHYRUM, ZEPHYRUM, &electum,
        c->piscina), (s32)DELECTUS_NIHIL);
    CREDO_AEQUALIS_S32 (electum, -VII);
    CREDO_FALSUM (delectus_apertus(&x->ramus, NIHIL));
    /* aperire cum valore currente illuminato */
    CREDO_NECESSE (delectus_aperire(&x->ramus, scriptor, "cyclus", I,
        c->piscina));
    CREDO_VERUM (delectus_apertus(&x->ramus, NIHIL));
    CREDO_VERUM (delectus_apertus(&x->ramus, "cyclus"));
    CREDO_FALSUM (delectus_apertus(&x->ramus, "modus"));
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), I);
    a = insula_ramus_attributum(&x->ramus, "delectus");
    CREDO_VERUM (a && chorda_aequalis_literis(*a, "cyclus"));
    /* electa extra numerum optionum: nulla */
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, I), -I);
    /* alter aperitur: primus clausus, electa nova */
    CREDO_NECESSE (delectus_aperire(&x->ramus, scriptor, "modus", -I,
        c->piscina));
    CREDO_FALSUM (delectus_apertus(&x->ramus, "cyclus"));
    CREDO_VERUM (delectus_apertus(&x->ramus, "modus"));
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), -I);
    /* claudere: campi ambo absunt */
    CREDO_VERUM (delectus_claudere(&x->ramus, scriptor, c->piscina));
    CREDO_FALSUM (delectus_apertus(&x->ramus, NIHIL));
    CREDO_VERUM (insula_ramus_attributum(&x->ramus, "delectus")
        == NIHIL);
    CREDO_VERUM (insula_ramus_attributum(&x->ramus, "delectus_electa")
        == NIHIL);
    /* argumenta mala */
    CREDO_FALSUM (delectus_aperire(&x->ramus, scriptor, "", I,
        c->piscina));
    CREDO_FALSUM (delectus_aperire(&x->ramus, scriptor, NIHIL, I,
        c->piscina));
    CREDO_FALSUM (delectus_aperire(NIHIL, scriptor, "cyclus", I,
        c->piscina));
    CREDO_FALSUM (delectus_claudere(NIHIL, scriptor, c->piscina));
}

interior vacuum
_probare_claves (
    CredoContextus* c)
{
    Mundus* x        = (Mundus*)c->datum;
       s32  electum  = -VII;

    CREDO_NECESSE (delectus_aperire(&x->ramus, scriptor, "cyclus", I,
        c->piscina));
    /* j deorsum, k sursum (deglutita, status mutatus) */
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, (clavis_t)'j', 'j', &electum,
        c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), II);
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, (clavis_t)'k', 'k', &electum,
        c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), I);
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, CLAVIS_SURSUM, ZEPHYRUM, &electum,
        c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), ZEPHYRUM);
    /* clavis aliena: deglutitur, apertus manet */
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, (clavis_t)'q', 'q', &electum,
        c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_VERUM (delectus_apertus(&x->ramus, "cyclus"));
    CREDO_AEQUALIS_S32 (electum, -VII);
    /* Reditus: electum, clausus */
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, CLAVIS_DEORSUM, ZEPHYRUM, &electum,
        c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, CLAVIS_REDITUS, XIII, &electum,
        c->piscina), (s32)DELECTUS_ELECTUM);
    CREDO_AEQUALIS_S32 (electum, I);
    CREDO_FALSUM (delectus_apertus(&x->ramus, NIHIL));
    /* Esc: clausus sine electione */
    electum = -VII;
    CREDO_NECESSE (delectus_aperire(&x->ramus, scriptor, "cyclus", II,
        c->piscina));
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, CLAVIS_EFFUGIUM, XXVII, &electum,
        c->piscina), (s32)DELECTUS_CLAUSUM);
    CREDO_AEQUALIS_S32 (electum, -VII);
    CREDO_FALSUM (delectus_apertus(&x->ramus, NIHIL));
    /* obscura saltatur: ab 0 deorsum ad II */
    CREDO_NECESSE (delectus_aperire(&x->ramus, scriptor, "cyclus",
        ZEPHYRUM, c->piscina));
    (vacuum)_agere(x, OPTIONES_OBSCURAE, NIHIL,
        EVENTUS_CLAVIS_DEPRESSUS, (clavis_t)'j', 'j', &electum,
        c->piscina);
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), II);
    /* eventus nec muris nec clavis: hospitis (apertus manet) */
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_FOCUS_CAPTUS, ZEPHYRUM, ZEPHYRUM, &electum,
        c->piscina), (s32)DELECTUS_NIHIL);
    CREDO_VERUM (delectus_apertus(&x->ramus, "cyclus"));
}

interior vacuum
_probare_murem (
    CredoContextus* c)
{
      Mundus* x = (Mundus*)c->datum;
    Elenchus  e;
         s32  electum = -VII;

    CREDO_NECESSE (delectus_aperire(&x->ramus, scriptor, "cyclus",
        ZEPHYRUM, c->piscina));
    e = _optiones(x, OPTIONES_OBSCURAE);
    /* motus illuminat habilem, non obscuram */
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES_OBSCURAE,
        _nodus(x, &e, II, c->piscina), EVENTUS_MUS_MOTUS, ZEPHYRUM,
        ZEPHYRUM, &electum, c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), II);
    (vacuum)_agere(x, OPTIONES_OBSCURAE, _nodus(x, &e, I, c->piscina),
        EVENTUS_MUS_MOTUS, ZEPHYRUM, ZEPHYRUM, &electum, c->piscina);
    CREDO_AEQUALIS_S32 (delectus_electa(&x->ramus, III), II);
    /* ictus in obscura: deglutitur, apertus */
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES_OBSCURAE,
        _nodus(x, &e, I, c->piscina), EVENTUS_MUS_DEPRESSUS, ZEPHYRUM,
        ZEPHYRUM, &electum, c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_VERUM (delectus_apertus(&x->ramus, "cyclus"));
    /* ictus in margine menu (DIALOGUS), duplex, liberatio: manet */
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES_OBSCURAE,
        _nodus(x, &e, -I, c->piscina), EVENTUS_MUS_DEPRESSUS, ZEPHYRUM,
        ZEPHYRUM, &electum, c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES_OBSCURAE, NIHIL,
        EVENTUS_MUS_DUPLEX, ZEPHYRUM, ZEPHYRUM, &electum, c->piscina),
        (s32)DELECTUS_DEGLUTITUM);
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES_OBSCURAE, NIHIL,
        EVENTUS_MUS_LIBERATUS, ZEPHYRUM, ZEPHYRUM, &electum,
        c->piscina), (s32)DELECTUS_DEGLUTITUM);
    CREDO_VERUM (delectus_apertus(&x->ramus, "cyclus"));
    CREDO_AEQUALIS_S32 (electum, -VII);
    /* ictus in optione habili: electum, clausus */
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES_OBSCURAE,
        _nodus(x, &e, II, c->piscina), EVENTUS_MUS_DEPRESSUS, ZEPHYRUM,
        ZEPHYRUM, &electum, c->piscina), (s32)DELECTUS_ELECTUM);
    CREDO_AEQUALIS_S32 (electum, II);
    CREDO_FALSUM (delectus_apertus(&x->ramus, NIHIL));
    /* ictus extra (nodus alius aut nullus): clausus */
    electum = -VII;
    CREDO_NECESSE (delectus_aperire(&x->ramus, scriptor, "cyclus",
        ZEPHYRUM, c->piscina));
    CREDO_AEQUALIS_S32 ((s32)_agere(x, OPTIONES, NIHIL,
        EVENTUS_MUS_DEPRESSUS, ZEPHYRUM, ZEPHYRUM, &electum,
        c->piscina), (s32)DELECTUS_CLAUSUM);
    CREDO_FALSUM (delectus_apertus(&x->ramus, NIHIL));
    CREDO_AEQUALIS_S32 (electum, -VII);
}

interior vacuum
_probare_locum (
    CredoContextus* c)
{
       Mundus* x = (Mundus*)c->datum;
     Elenchus  e = _optiones(x, OPTIONES);
     Elenchus  vacuus;
    Componens* menu;
    Componens* botton;
        Fines  b;
        Fines  area = _fines(ZEPHYRUM, ZEPHYRUM, CC, CC);

    /* sub bottone, margine sinistro alineatum, tres optiones */
    b     = _fines(XX, X, LX, XII);
    menu  = delectus_componere(&e, b, area, "a", c->piscina, x->intern);
    CREDO_NECESSE (menu != NIHIL);
    CREDO_AEQUALIS_S32 (menu->fines.x, XX);
    CREDO_VERUM (menu->fines.y >= b.y + b.altitudo);
    CREDO_VERUM (menu->fines.y <= b.y + b.altitudo + II);
    CREDO_VERUM (menu->partes == PARTES_DIALOGUS);
    CREDO_VERUM (componens_liberum(menu, II) != NIHIL);
    CREDO_AEQUALIS_S32 (elenchus_index(componens_liberum(menu, II)),
        II);
    /* botton in ima area: menu SUPRA bottonem, non super eum */
    b     = _fines(XX, CLXXX, LX, XII);
    menu  = delectus_componere(&e, b, area, "a", c->piscina, x->intern);
    CREDO_NECESSE (menu != NIHIL);
    CREDO_VERUM (menu->fines.y + menu->fines.altitudo <= b.y);
    CREDO_VERUM (menu->fines.y >= ZEPHYRUM);
    /* botton ad marginem dextrum: menu intra aream manet */
    b     = _fines(CXC, X, X, XII);
    menu  = delectus_componere(&e, b, area, "a", c->piscina, x->intern);
    CREDO_NECESSE (menu != NIHIL);
    CREDO_VERUM (menu->fines.x + menu->fines.latitudo <= CC);
    /* optiones vacuae: nullum menu */
    vacuus.res      = OPTIONES;
    vacuus.numerus  = ZEPHYRUM;
    vacuus.electa   = -I;
    CREDO_VERUM (delectus_componere(&vacuus, b, area, "a", c->piscina,
        x->intern) == NIHIL);
    CREDO_VERUM (delectus_componere(NIHIL, b, area, "a", c->piscina,
        x->intern) == NIHIL);
    /* botton: nodus, fines, latitudo cum textu crescit */
    b       = _fines(V, VI, L, XII);
    botton  = delectus_bottonem_componere("cyclus", b, "a", c->piscina,
        x->intern);
    CREDO_NECESSE (botton != NIHIL);
    CREDO_VERUM (botton->partes == PARTES_BOTTONE);
    CREDO_VERUM (chorda_aequalis_literis(botton->id, "cyclus"));
    CREDO_AEQUALIS_S32 (botton->fines.x, V);
    CREDO_AEQUALIS_S32 (botton->fines.latitudo, L);
    CREDO_VERUM (delectus_bottonis_latitudo(chorda_ex_literis(
        "boulder_throw", c->piscina))
        > delectus_bottonis_latitudo(chorda_ex_literis("walk",
            c->piscina)));
    CREDO_AEQUALIS_I32 (delectus_bottonis_latitudo(chorda_ex_literis(
        "walk", c->piscina)) - delectus_bottonis_latitudo(
        chorda_ex_literis("wa", c->piscina)), XII);
}

interior vacuum
_pingere_omnia (
    Mundus* x,
   Piscina* p)
{
    Mandata* md  = mandata_creare(p, x->intern);
     chorda  t   = chorda_ex_literis("boulder_throw", p);

    delectus_bottonem_pingere(md, _fines(ZEPHYRUM, ZEPHYRUM, C, XII), t,
        FALSUM, VERUM);
    delectus_bottonem_pingere(md, _fines(ZEPHYRUM, ZEPHYRUM, C, XII), t,
        VERUM, VERUM);
    delectus_bottonem_pingere(md, _fines(ZEPHYRUM, ZEPHYRUM, C, XII), t,
        FALSUM, FALSUM);
    delectus_bottonem_pingere(md, _fines(ZEPHYRUM, ZEPHYRUM, I, I), t,
        FALSUM, VERUM);
    delectus_bottonem_pingere(NIHIL, _fines(ZEPHYRUM, ZEPHYRUM, C,
        XII), t, FALSUM, VERUM);
}

interior vacuum
_probare_picturam (
    CredoContextus* c)
{
     Mundus* x   = (Mundus*)c->datum;
    Mandata* md  = mandata_creare(c->piscina, x->intern);
        i32  ante;

    CREDO_NECESSE (md != NIHIL);
    ante = mandata_numerus(md);
    delectus_bottonem_pingere(md, _fines(ZEPHYRUM, ZEPHYRUM, C, XII),
        chorda_ex_literis("walk", c->piscina), FALSUM, VERUM);
    CREDO_VERUM (mandata_numerus(md) > ante);
    CREDO_NON_RUIT (_pingere_omnia(x, c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "status", _probare_statum, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "claves", _probare_claves, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "mus", _probare_murem, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "locus", _probare_locum, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "pictura", _probare_picturam, mundum_parare, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("membrum__delectus", SECTIONES);
}
