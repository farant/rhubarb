/* probatio_psalterium_latus.c - latus psalterii (psalterium.h;
 * psalterium planum T5-T6)
 *
 * T5: auxilia pura (incipit, circulus signorum, argumentum spec
 * III.3) et visus indicis: CL ordines paginati, electus et paginae
 * per claves et sagittas, cifrae typatae (Review Focus III), latus
 * minimum (ordo unus in pagina), ordo ictu aperitur.
 *
 * Latus solum (ut probatio_antiphonale_latus): repositorium,
 * montatio, dispensator, eventa ficta. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "sors.h"
#include "psalterium.h"
#include "psalterium_infixum.h"
#include "psalmodia_tabula.h"
#include "toni_psalmorum_infixum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO  CDLXXX
#define ALTITUDO  CCCXX


/* ==================================================
 * Auxilia
 * ================================================== */

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
      PsalteriumMontatio  m;
             Dispensator* d;
} Mundus;

interior Mundus*
_mundum_mensura (
         CredoContextus* c,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo)
{
    Mundus* w;
     Motus* motus;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->intern  = internamentum_creare(c->piscina);
    w->repo    = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !psalterium_montare(&w->m, c->piscina, w->intern, w->repo,
               "p1", argumentum, latitudo, altitudo, NIHIL, NIHIL))
    {
        redde NIHIL;
    }
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, psalterium_componere, &w->m, CCC);
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
_mundum (
         CredoContextus* c,
     constans character* argumentum)
{
    redde _mundum_mensura(c, argumentum, LATITUDO, ALTITUDO);
}

interior Mandata*
_reddere (
     Mundus* w,
    Piscina* p)
{
    Mandata* md = mandata_creare(p, w->intern);

    dispensator_recomponere(w->d);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    redde md;
}

/* textus pictus qui litteras datas continet */
interior b32
_textus_continet (
               Mandata* md,
    constans character* pars)
{
    constans Mandatum* x;
                  i32  i;
                  i32  k;
                  i32  n;

    n = (i32)strlen(pars);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (x->genus != MANDATUM_TEXTUS)
        {
            perge;
        }
        per (k = ZEPHYRUM; k + n <= x->textus.mensura; k++)
        {
            si (memcmp(x->textus.datum + k, pars, (size_t)n)
                == ZEPHYRUM)
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior vacuum
_clavis (
       Mundus* w,
     clavis_t  clavis,
          s32  producta)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = clavis;
    e.datum.clavis.producta  = producta;
    e.datum.clavis.runa      = producta;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
_littera (
       Mundus* w,
    character  c)
{
    _clavis(w, (clavis_t)c, (s32)c);
}

/* ictus in medio componentis nominati */
interior b32
_ictus (
                Mundus* w,
    constans character* id,
               Piscina* p)
{
     Componens* c;
       Eventus  e;

    c = componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
    si (!c)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    e.genus = EVENTUS_MUS_LIBERATUS;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
    redde VERUM;
}

interior s32
_campus (
                Mundus* w,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a = insula_ramus_attributum(&w->m.ramus, titulus);
       s32  v;

    si (!a || !chorda_ut_s32(*a, &v))
    {
        redde praestitutum;
    }
    redde v;
}

/* campus textus; "" si abest */
interior chorda
_campus_textus (
                Mundus* w,
    constans character* titulus)
{
     chorda* a = insula_ramus_attributum(&w->m.ramus, titulus);
     chorda  vacua;

    si (a)
    {
        redde *a;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

/* ordines visi: numerus nodorum "r<n>", *primus et *ultimus n */
interior i32
_ordines (
    Mundus* w,
       s32* primus,
       s32* ultimus)
{
    Componens* radix = dispensator_arbor(w->d);
          i32  i;
          i32  numerus = ZEPHYRUM;

    *primus   = -I;
    *ultimus  = -I;
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
        i++)
    {
         Componens* f = componens_liberum(radix, i);
               s32  n;
            chorda  id = f->id;

        si (id.mensura < II || id.datum[ZEPHYRUM] != 'r')
        {
            perge;
        }
        id.datum++;
        id.mensura--;
        si (!chorda_ut_s32(id, &n))
        {
            perge;
        }
        si (*primus < ZEPHYRUM)
        {
            *primus = n;
        }
        *ultimus = n;
        numerus++;
    }
    redde numerus;
}

interior PsalmodiaTabula*
_tabula (
    CredoContextus* c)
{
    redde psalmodia_tabulam_legere(toni_psalmorum_infixum_textus(
        c->piscina), c->piscina);
}


/* ==================================================
 * Auxilia pura
 * ================================================== */

interior vacuum
_incipit_probare (
    CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS (psalterium_incipit(
        psalterium_infixum_psalmus(CIX, c->piscina), c->piscina),
        "Dixit Dóminus Dómino meo");
    /* Ps 1: ante '†' praeciditur (caput plagulae solum ante '*') */
    CREDO_CHORDA_AEQUALIS_LITERIS (psalterium_incipit(
        psalterium_infixum_psalmus(I, c->piscina), c->piscina),
        "Beátus vir, qui non ábiit in consílio impiórum");
    /* Ps 118: linea '% Aleph' praeteritur */
    CREDO_CHORDA_AEQUALIS_LITERIS (psalterium_incipit(
        psalterium_infixum_psalmus(CXVIII, c->piscina), c->piscina),
        "Beáti immaculáti in via");
    CREDO_CHORDA_AEQUALIS_LITERIS (psalterium_incipit(chorda_ex_literis(
        "% caput\r\n\n1. Lauda, Jerúsalem; * Dóminum.\n", c->piscina),
        c->piscina), "Lauda, Jerúsalem");
    CREDO_AEQUALIS_I32 (psalterium_incipit(chorda_ex_literis(
        "% solum\n\n", c->piscina), c->piscina).mensura, ZEPHYRUM);
}

interior vacuum
_signa_probare (
    CredoContextus* c)
{
     PsalmodiaTabula* t = _tabula(c);
                 s32  octavus;
                 i32  k;
                 i32  inventa = ZEPHYRUM;
                 i32  reditus = ZEPHYRUM;

    CREDO_NECESSE (psalmodia_successus(t));
    CREDO_AEQUALIS_I32 (psalterium_signa_numerus(t), XXXIII);
    octavus = psalterium_signi_index(t, chorda_ex_literis("8 G",
        c->piscina));
    CREDO_VERUM (octavus >= ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (psalterium_signum(t, (i32)octavus,
        c->piscina), "8 G");
    /* formae aliae eiusdem signi (psalmodia_signum_invenire) */
    CREDO_AEQUALIS_S32 (psalterium_signi_index(t, chorda_ex_literis(
        "VIII g", c->piscina)), octavus);
    CREDO_AEQUALIS_S32 (psalterium_signi_index(t, chorda_ex_literis(
        "8 Z", c->piscina)), -I);
    CREDO_AEQUALIS_S32 (psalterium_signi_index(t, chorda_ex_literis("",
        c->piscina)), -I);
    CREDO_AEQUALIS_I32 (psalterium_signum(t, XXXIII,
        c->piscina).mensura,
        ZEPHYRUM);
    per (k = ZEPHYRUM; k < psalterium_signa_numerus(t); k++)
    {
         constans PsalmodiaTonus* tonus;
                          chorda  s = psalterium_signum(t, k,
                                       c->piscina);

        si (psalmodia_signum_invenire(t, s, &tonus, NIHIL) != NIHIL)
        {
            inventa++;
        }
        si (psalterium_signi_index(t, s) == (s32)k)
        {
            reditus++;
        }
        alioquin
        {
            imprimere("signum %u \"%.*s\" non redit\n", (unsigned)k,
                (integer)s.mensura, (constans character*)s.datum);
        }
    }
    CREDO_AEQUALIS_I32 (inventa, XXXIII);
    CREDO_AEQUALIS_I32 (reditus, XXXIII);
}

/* argumentum -> VERUM/FALSUM, psalmus, signum, causa */
interior b32
_legere (
         CredoContextus* c,
     constans character* argumentum,
                    s32* psalmus,
                 chorda* signum,
                 chorda* causa)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, LXIV);
                  b32  ok;

    *psalmus  = -VII;
    ok        = psalterium_argumentum_legere(chorda_ex_literis(
        argumentum, c->piscina), c->piscina, psalmus, signum, a);
    *causa    = chorda_aedificator_finire(a);
    redde ok;
}

interior vacuum
_argumentum_probare (
    CredoContextus* c)
{
    ChordaAedificator* expectata;
      PsalmodiaTabula* t = _tabula(c);
                  s32  psalmus;
               chorda  signum;
               chorda  causa;

    CREDO_VERUM (_legere(c, "", &psalmus, &signum, &causa));
    CREDO_AEQUALIS_S32 (psalmus, -I);
    CREDO_AEQUALIS_I32 (signum.mensura, ZEPHYRUM);

    CREDO_VERUM (_legere(c, "109", &psalmus, &signum, &causa));
    CREDO_AEQUALIS_S32 (psalmus, CIX);
    CREDO_AEQUALIS_I32 (signum.mensura, ZEPHYRUM);

    CREDO_VERUM (_legere(c, "109,8 G", &psalmus, &signum, &causa));
    CREDO_AEQUALIS_S32 (psalmus, CIX);
    CREDO_CHORDA_AEQUALIS_LITERIS (signum, "8 G");

    /* spatia circa partes; signum in formam circuli redditur */
    CREDO_VERUM (_legere(c, " 109 , viii g ", &psalmus, &signum,
        &causa));
    CREDO_AEQUALIS_S32 (psalmus, CIX);
    CREDO_CHORDA_AEQUALIS_LITERIS (signum, "8 G");

    CREDO_FALSUM (_legere(c, "151", &psalmus, &signum, &causa));
    CREDO_CHORDA_AEQUALIS_LITERIS (causa,
        "psalterium: psalm number 1-150");
    CREDO_FALSUM (_legere(c, "0", &psalmus, &signum, &causa));
    CREDO_CHORDA_AEQUALIS_LITERIS (causa,
        "psalterium: psalm number 1-150");
    CREDO_FALSUM (_legere(c, "abc", &psalmus, &signum, &causa));
    CREDO_CHORDA_AEQUALIS_LITERIS (causa,
        "psalterium: psalm number 1-150");
    CREDO_FALSUM (_legere(c, "10x", &psalmus, &signum, &causa));
    CREDO_CHORDA_AEQUALIS_LITERIS (causa,
        "psalterium: psalm number 1-150");

    /* tonus ignotus: causa psalmodia_signum_invenire (differentias
     * toni enumerat) */
    expectata = chorda_aedificator_creare(c->piscina, LXIV);
    chorda_aedificator_appendere_literis(expectata, "psalterium: ");
    CREDO_NECESSE (psalmodia_signum_invenire(t, chorda_ex_literis("8 Z",
        c->piscina), NIHIL, expectata) == NIHIL);
    CREDO_FALSUM (_legere(c, "109, 8 Z", &psalmus, &signum, &causa));
    CREDO_CHORDA_AEQUALIS (causa, chorda_aedificator_finire(expectata));
    CREDO_VERUM (chorda_continet(causa, chorda_ex_literis("G",
        c->piscina)));
}


/* ==================================================
 * Index
 * ================================================== */

interior vacuum
_index_probare (
    CredoContextus* c)
{
     Mundus* w = _mundum(c, NIHIL);
    Mandata* md;
        s32  primus;
        s32  ultimus;
        s32  capacitas;
        s32  visi[CLI];
        s32  n;
        i32  k;
        i32  gradus;
        b32  omnes;

    CREDO_NECESSE (w != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"), "index");
    capacitas = (s32)_ordines(w, &primus, &ultimus);
    CREDO_VERUM (capacitas > I);
    CREDO_AEQUALIS_S32 (primus, I);
    CREDO_AEQUALIS_S32 (ultimus, capacitas);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "  1  Beátus vir"));
    CREDO_VERUM (_textus_continet(md, "Divinum Officium · MIT"));

    /* PageDown: electus ad ordinem primum paginae proximae */
    _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), capacitas + I);
    (vacuum)_ordines(w, &primus, &ultimus);
    CREDO_AEQUALIS_S32 (primus, capacitas + I);
    _clavis(w, CLAVIS_PAGINA_SURSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);
    /* PageUp in pagina prima: nihil */
    _clavis(w, CLAVIS_PAGINA_SURSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);

    /* Down / j / Up / k; Up in I nihil */
    _clavis(w, CLAVIS_DEORSUM, ZEPHYRUM);
    _littera(w, 'j');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), III);
    _clavis(w, CLAVIS_SURSUM, ZEPHYRUM);
    _littera(w, 'k');
    _littera(w, 'k');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);

    /* sagittae lineae imae */
    CREDO_VERUM (_ictus(w, "sagitta_proxima", c->piscina));
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), capacitas + I);
    CREDO_VERUM (_ictus(w, "sagitta_prior", c->piscina));
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);

    /* CL ordines per paginas omnes, quisque semel */
    memset(visi, ZEPHYRUM, magnitudo(visi));
    per (gradus = ZEPHYRUM; gradus < CL; gradus++)
    {
        (vacuum)_ordines(w, &primus, &ultimus);
        per (n = primus; n >= I && n <= ultimus && n <= CL; n++)
        {
            visi[n]++;
        }
        si (ultimus >= CL)
        {
            frange;
        }
        _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    }
    omnes = VERUM;
    per (k = I; k <= CL; k++)
    {
        omnes = omnes && visi[k] == I;
    }
    CREDO_VERUM (omnes);
    /* PageDown in pagina ultima: nihil; Down in CL nihil */
    n = _campus(w, "electus", ZEPHYRUM);
    _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), n);

    /* ordo 109 pictus; Enter aperit */
    _littera(w, '1');
    _littera(w, '0');
    _littera(w, '9');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), CIX);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md,
        "109  Dixit Dóminus Dómino meo"));
    _clavis(w, CLAVIS_REDITUS, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"),
        "psalmus");
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), CIX);

    /* ordo ictu aperitur */
    w = _mundum(c, "");
    CREDO_NECESSE (w != NIHIL);
    CREDO_VERUM (_ictus(w, "r3", c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"),
        "psalmus");
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), III);

    /* latus minimum: ordo unus in pagina, paginae tamen vertuntur */
    w = _mundum_mensura(c, NIHIL, LATITUDO, XX);
    CREDO_NECESSE (w != NIHIL);
    CREDO_AEQUALIS_I32 (_ordines(w, &primus, &ultimus), I);
    CREDO_AEQUALIS_S32 (primus, I);
    _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), II);
    (vacuum)_ordines(w, &primus, &ultimus);
    CREDO_AEQUALIS_S32 (primus, II);
    md = _reddere(w, c->piscina);
    CREDO_VERUM (md != NIHIL);
}

/* Review Focus III: cifrae typatae numquam extra I-CL eligunt */
interior vacuum
_numeri_probare (
    CredoContextus* c)
{
     Mundus* w = _mundum(c, NIHIL);
       Sors  sors;
        i32  k;
        b32  intra = VERUM;

    CREDO_NECESSE (w != NIHIL);
    _littera(w, '1');
    _littera(w, '0');
    _littera(w, '9');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), CIX);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "cifrae"), "109");
    CREDO_VERUM (_textus_continet(_reddere(w, c->piscina), "[109]"));

    /* motus cifras tollit */
    _clavis(w, CLAVIS_DEORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_campus_textus(w, "cifrae").mensura, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), CX);

    /* 151 > CL: a cifra nova incipit */
    _littera(w, '1');
    _littera(w, '5');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), XV);
    _littera(w, '1');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "cifrae"), "1");

    /* 0 solum: nihil eligit */
    _clavis(w, CLAVIS_SURSUM, ZEPHYRUM);
    _littera(w, '0');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);
    CREDO_AEQUALIS_I32 (_campus_textus(w, "cifrae").mensura, ZEPHYRUM);

    /* 200: 20, deinde 0 solum - cifrae vacuae, electus 20 */
    _littera(w, '2');
    _littera(w, '0');
    _littera(w, '0');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), XX);
    CREDO_AEQUALIS_I32 (_campus_textus(w, "cifrae").mensura, ZEPHYRUM);

    /* Backspace: cifra ultima tollitur */
    _littera(w, '1');
    _littera(w, '2');
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), XII);
    _clavis(w, CLAVIS_RETRORSUM, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "cifrae"), "1");
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);
    _clavis(w, CLAVIS_RETRORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_campus_textus(w, "cifrae").mensura, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);
    _clavis(w, CLAVIS_RETRORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), I);

    /* temere: cifrae et Backspace - electus semper intra I-CL */
    sors_seminare(&sors, (i64)CL, (i64)CIX);
    per (k = ZEPHYRUM; k < (i32)CD; k++)
    {
        s32 r = (s32)sors_intra(&sors, XII);
        s32 e;

        si (r < X)
        {
            _littera(w, (character)('0' + r));
        }
        alioquin si (r == X)
        {
            _clavis(w, CLAVIS_RETRORSUM, ZEPHYRUM);
        }
        alioquin
        {
            _clavis(w, CLAVIS_DEORSUM, ZEPHYRUM);
        }
        e = _campus(w, "electus", I);
        intra = intra && e >= I && e <= CL
            && _campus_textus(w, "cifrae").mensura <= III;
    }
    CREDO_VERUM (intra);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "incipit", _incipit_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "circulus signorum", _signa_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "argumentum (spec III.3)", _argumentum_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "index: ordines, paginae, claves, mus", _index_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "index: cifrae typatae", _numeri_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("psalterium_latus", SECTIONES, NIHIL,
        NIHIL);
}
