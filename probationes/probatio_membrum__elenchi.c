/* probatio_membrum__elenchi.c - menu hospitis (nomenclator-plan U4):
 * res ex tabula (omissae extra visum, divisor transit, obscurae),
 * aperire / legere / claudere, eventa (motus, ictus, obscura, extra,
 * DIALOGUS, Esc, claves, Reditus, duplex) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "elenchus.h"
#include "membrum__praecepta.h"
#include "membrum__elenchi.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VISUS_A    I
#define VISUS_B    II
#define CTX_A      I
#define CTX_B      II
#define REQ_ELECTA I

hic_manens constans character scriptor[] = "probatio.agere";

/* hospes fictus: vocationes per praeceptum (a b c d) */
nomen structura {
    i32 vocata[IV];
    b32 electa;
} Hospes;

interior b32
fac_a (
    vacuum* c)
{ ((Hospes*)c)->vocata[ZEPHYRUM]++; redde VERUM;
}

interior b32
fac_b (
    vacuum* c)
{ ((Hospes*)c)->vocata[I]++; redde VERUM;
}

interior b32
fac_c (
    vacuum* c)
{ ((Hospes*)c)->vocata[II]++; redde VERUM;
}

interior b32
fac_d (
    vacuum* c)
{ ((Hospes*)c)->vocata[III]++; redde VERUM;
}

interior b32
requisita (
       i32  r,
    vacuum* ctx)
{
    redde !(r & REQ_ELECTA) || ((Hospes*)ctx)->electa;
}

hic_manens constans Praeceptum PRAECEPTA[] = {
    { "t.a", "Alpha", "a", VISUS_A, ZEPHYRUM, fac_a },
    { "t.b", "Beta", "b", VISUS_A, REQ_ELECTA, fac_b },
    { "t.c", "Gamma", "c", VISUS_B, ZEPHYRUM, fac_c },
    { "t.d", "Delta", NIHIL, VISUS_A | VISUS_B, ZEPHYRUM, fac_d },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* CTX_A in VISUS_A: Alpha, (Gamma omissa: divisor transit ad) Beta,
 * Delta (divisor) */
hic_manens constans ElenchiIntroitus INTROITUS[] = {
    { CTX_A, "t.a", FALSUM },
    { CTX_A, "t.c", VERUM },
    { CTX_A, "t.b", FALSUM },
    { CTX_A, "t.d", VERUM },
    { CTX_B, "t.d", FALSUM },
    { ZEPHYRUM, NIHIL, FALSUM }
};

hic_manens constans ElenchiIntroitus MALI[] = {
    { CTX_A, "t.z", FALSUM },
    { ZEPHYRUM, NIHIL, FALSUM }
};

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
             InsulaRamus  ramus;
                  Hospes  h;
           ElenchiHospes  hospes;
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
    x->hospes.introitus  = INTROITUS;
    x->hospes.praecepta  = PRAECEPTA;
    x->hospes.visus      = VISUS_A;
    x->hospes.requisita  = requisita;
    x->hospes.ctx        = &x->h;
    c->datum             = x;
}

interior b32
aperire (
     Mundus* x,
    Piscina* p)
{
    Punctum a;

    a.x = X;
    a.y = XX;
    redde elenchi_aperire(&x->ramus, scriptor, CTX_A, a, p);
}

interior b32
legere (
      Mundus* x,
     Piscina* p,
    Elenchus* e)
{
    Punctum a;

    redde elenchi_legere(&x->ramus, &x->hospes, p, e, &a);
}

/* nodus rei k menu (aut DIALOGUS si k < 0) */
interior Componens*
nodus_rei (
               Mundus* x,
    constans Elenchus* e,
                  s32  k,
              Piscina* p)
{
     Componens* r;
         Fines  f;

    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = C;
    f.altitudo  = C;
    r           = elenchus_componere(e, f, "a", p, x->intern);
    si (!r || k < ZEPHYRUM)
    {
        redde r;
    }
    redde componens_liberum(r, (i32)k);
}

interior b32
agere (
                   Mundus* x,
        constans Elenchus* e,
       constans Componens* nodus,
          eventus_genus_t  genus,
                 clavis_t  codex,
                      s32  producta,
                  Piscina* p)
{
    Eventus ev;

    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus             = genus;
    ev.datum.mus.botton  = MUS_SINISTER;
    si (genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        memset(&ev.datum, ZEPHYRUM, magnitudo(ev.datum));
        ev.datum.clavis.clavis    = codex;
        ev.datum.clavis.producta  = producta;
    }
    redde elenchi_agere(&x->ramus, scriptor, &x->hospes, e, nodus, &ev,
        p);
}

interior b32
electa_est (
                 Mundus* x,
     constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&x->ramus, "elenchus_electa");
    redde valor ? (a && chorda_aequalis_literis(*a, valor)) : !a;
}

/* legere: res, divisores, obscurae, ancora, verificatio */
interior vacuum
_probare_legere (
    CredoContextus* c)
{
      Mundus* x;
    Elenchus  e;
     Punctum  a;
      chorda* s;

    x = (Mundus*)c->datum;
    CREDO_VERUM(elenchi_verificare(INTROITUS, PRAECEPTA));
    CREDO_FALSUM(elenchi_verificare(MALI, PRAECEPTA));
    CREDO_FALSUM(legere(x, c->piscina, &e));
    CREDO_NECESSE(aperire(x, c->piscina));
    s = insula_ramus_attributum(&x->ramus, "elenchus");
    CREDO_VERUM(s && chorda_aequalis_literis(*s, "1 10 20"));
    CREDO_NECESSE(elenchi_legere(&x->ramus, &x->hospes, c->piscina, &e,
        &a));
    CREDO_AEQUALIS_I32(e.numerus, III);
    CREDO_VERUM(strcmp(e.res[ZEPHYRUM].textus, "Alpha") == ZEPHYRUM);
    CREDO_VERUM(strcmp(e.res[I].textus, "Beta") == ZEPHYRUM);
    CREDO_VERUM(strcmp(e.res[II].textus, "Delta") == ZEPHYRUM);
    CREDO_FALSUM(e.res[ZEPHYRUM].divisor);
    CREDO_VERUM(e.res[I].divisor);
    CREDO_VERUM(e.res[II].divisor);
    CREDO_VERUM(e.res[ZEPHYRUM].habilis);
    CREDO_FALSUM(e.res[I].habilis);
    CREDO_VERUM(strcmp(e.res[ZEPHYRUM].clavis, "a") == ZEPHYRUM);
    CREDO_AEQUALIS_S32(e.electa, -I);
    CREDO_AEQUALIS_S32(a.x, X);
    CREDO_AEQUALIS_S32(a.y, XX);
    x->h.electa = VERUM;
    CREDO_NECESSE(legere(x, c->piscina, &e));
    CREDO_VERUM(e.res[I].habilis);
    CREDO_VERUM(elenchi_claudere(&x->ramus, scriptor, c->piscina));
    CREDO_FALSUM(legere(x, c->piscina, &e));
}

/* mus: motus illuminat (obscura non); ictus habilis claudit et agit;
 * obscura deglutitur; DIALOGUS manet; extra claudit; duplex
 * deglutitur */
interior vacuum
_probare_murem (
    CredoContextus* c)
{
      Mundus* x;
    Elenchus  e;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aperire(x, c->piscina));
    CREDO_NECESSE(legere(x, c->piscina, &e));
    CREDO_VERUM(agere(x, &e, nodus_rei(x, &e, II, c->piscina),
        EVENTUS_MUS_MOTUS, ZEPHYRUM, ZEPHYRUM, c->piscina));
    CREDO_VERUM(electa_est(x, "2"));
    (vacuum)agere(x, &e, nodus_rei(x, &e, I, c->piscina),
        EVENTUS_MUS_MOTUS, ZEPHYRUM, ZEPHYRUM, c->piscina);
    CREDO_VERUM(electa_est(x, "2"));
    /* obscura: deglutitur, apertum, nulla vocatio */
    CREDO_VERUM(agere(x, &e, nodus_rei(x, &e, I, c->piscina),
        EVENTUS_MUS_DEPRESSUS, ZEPHYRUM, ZEPHYRUM, c->piscina));
    CREDO_VERUM(legere(x, c->piscina, &e));
    CREDO_AEQUALIS_I32(x->h.vocata[I], ZEPHYRUM);
    /* DIALOGUS (margo): manet; duplex deglutitur */
    CREDO_VERUM(agere(x, &e, nodus_rei(x, &e, -I, c->piscina),
        EVENTUS_MUS_DEPRESSUS, ZEPHYRUM, ZEPHYRUM, c->piscina));
    CREDO_VERUM(agere(x, &e, NIHIL, EVENTUS_MUS_DUPLEX, ZEPHYRUM,
        ZEPHYRUM, c->piscina));
    CREDO_VERUM(legere(x, c->piscina, &e));
    /* habilis: clausum, Alpha vocata */
    CREDO_VERUM(agere(x, &e, nodus_rei(x, &e, ZEPHYRUM, c->piscina),
        EVENTUS_MUS_DEPRESSUS, ZEPHYRUM, ZEPHYRUM, c->piscina));
    CREDO_FALSUM(legere(x, c->piscina, &e));
    CREDO_AEQUALIS_I32(x->h.vocata[ZEPHYRUM], I);
    /* extra (nodus alius): clausum */
    CREDO_NECESSE(aperire(x, c->piscina));
    CREDO_NECESSE(legere(x, c->piscina, &e));
    CREDO_VERUM(agere(x, &e, NIHIL, EVENTUS_MUS_DEPRESSUS, ZEPHYRUM,
        ZEPHYRUM, c->piscina));
    CREDO_FALSUM(legere(x, c->piscina, &e));
}

/* claves: Deorsum illuminat, Reditus agit et claudit; clavis alia
 * deglutitur; Esc claudit; eventus alius non tractatur */
interior vacuum
_probare_claves (
    CredoContextus* c)
{
      Mundus* x;
    Elenchus  e;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(aperire(x, c->piscina));
    CREDO_NECESSE(legere(x, c->piscina, &e));
    CREDO_VERUM(agere(x, &e, NIHIL, EVENTUS_CLAVIS_DEPRESSUS,
        CLAVIS_DEORSUM, ZEPHYRUM, c->piscina));
    CREDO_VERUM(electa_est(x, "0"));
    CREDO_VERUM(agere(x, &e, NIHIL, EVENTUS_CLAVIS_DEPRESSUS,
        (clavis_t)'q', 'q', c->piscina));
    CREDO_NECESSE(legere(x, c->piscina, &e));
    CREDO_VERUM(agere(x, &e, NIHIL, EVENTUS_CLAVIS_DEPRESSUS,
        CLAVIS_REDITUS, XIII, c->piscina));
    CREDO_FALSUM(legere(x, c->piscina, &e));
    CREDO_AEQUALIS_I32(x->h.vocata[ZEPHYRUM], I);
    CREDO_NECESSE(aperire(x, c->piscina));
    CREDO_NECESSE(legere(x, c->piscina, &e));
    CREDO_FALSUM(agere(x, &e, NIHIL, EVENTUS_FOCUS_CAPTUS, ZEPHYRUM,
        ZEPHYRUM, c->piscina));
    CREDO_VERUM(agere(x, &e, NIHIL, EVENTUS_CLAVIS_DEPRESSUS,
        CLAVIS_EFFUGIUM, XXVII, c->piscina));
    CREDO_FALSUM(legere(x, c->piscina, &e));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "legere", _probare_legere, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "mus", _probare_murem, mundum_parare, NIHIL, NIHIL, NIHIL },
    { "claves", _probare_claves, mundum_parare, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("membrum__elenchi", SECTIONES);
}
