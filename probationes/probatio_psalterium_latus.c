/* probatio_psalterium_latus.c - latus psalterii (psalterium.h;
 * psalterium planum T5-T6)
 *
 * T5: auxilia pura (incipit, circulus signorum, argumentum spec
 * III.3) et visus indicis: CL ordines paginati, electus et paginae
 * per claves et sagittas, cifrae typatae (Review Focus III), latus
 * minimum (ordo unus in pagina), ordo ictu aperitur.
 *
 * T6: visus psalmi: aptatio (titulus, tonus), t/T circulus, ]/[
 * psalmi vicini, pagina, Esc, caput ictu; sonus sistitur (Review
 * Focus IV), status malus (I), memoria (V), restitutio et URI.
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
          GabcVisusPorta  porta;
    /* porta ficta: horologium a probatione positum */
                     b32 sonat;
                     s64 pulsus;
                     i32 incepta;
                     i32 sistita;
                     b32 recusare;  /* porta recusat (sonus mutus) */
} Mundus;

interior b32
_incipere (
    vacuum* ctx,
    chorda  carmen,
       s64  pulsus)
{
    Mundus* w = (Mundus*)ctx;

    (vacuum)carmen;
    w->incepta++;
    si (w->recusare)
    {
        redde FALSUM;
    }
    w->sonat   = VERUM;
    w->pulsus  = pulsus;
    redde VERUM;
}

interior vacuum
_quaerere (
    vacuum* ctx,
       s64  pulsus)
{
    ((Mundus*)ctx)->pulsus = pulsus;
}

interior vacuum
_sistere (
    vacuum* ctx)
{
    Mundus* w = (Mundus*)ctx;

    w->sistita++;
    w->sonat = FALSUM;
}

interior s64
_pulsus (
    vacuum* ctx)
{
    Mundus* w = (Mundus*)ctx;

    redde w->sonat ? w->pulsus : -I;
}

/* montatio in piscina data (memoria mensurabilis) super repositorium
 * datum (NIHIL = novum; idem = status servatus, ut vicus restituit) */
interior Mundus*
_mundum_plenum (
          CredoContextus* c,
                 Piscina* piscina,
      InsulaRepositorium* repo,
      constans character* argumentum,
                     i32  latitudo,
                     i32  altitudo)
{
    Mundus* w;
     Motus* motus;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    w->porta.incipere  = _incipere;
    w->porta.quaerere  = _quaerere;
    w->porta.sistere   = _sistere;
    w->porta.pulsus    = _pulsus;
    w->porta.ctx       = w;
    w->intern          = internamentum_creare(c->piscina);
    w->repo            = repo ? repo : insula_repositorium_creare(
        c->piscina, w->intern, "<hospes/>");
    si (   !w->repo
        || !psalterium_montare(&w->m, piscina, w->intern, w->repo, "p1",
               argumentum, latitudo, altitudo, &w->porta, NIHIL))
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
_mundum_mensura (
         CredoContextus* c,
     constans character* argumentum,
                    i32  latitudo,
                    i32  altitudo)
{
    redde _mundum_plenum(c, c->piscina, NIHIL, argumentum, latitudo,
        altitudo);
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

/* textus pictus aequalis litteris datis (elementum unum) */
interior b32
_textus_aequalis (
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

nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

interior vacuum
_campum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParCampi* par = (ParCampi*)ctx;

    si (par->valor[ZEPHYRUM] == '\0')
    {
        (vacuum)insula_attributum_tollere(radix, par->titulus);
        redde;
    }
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

/* status ab extra scriptus (ut status servatus aut malus): scriptore
 * psalterii; "" tollit */
interior b32
_campum_scribere (
                Mundus* w,
    constans character* titulus,
    constans character* valor,
               Piscina* p)
{
    ParCampi par;
      chorda prior;
         b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    prior        = w->repo->scriptor;
    insula_scriptorem_ponere(w->repo, chorda_ex_literis(
        "psalterium.agere", p));
    ok = mutare_ramum(&w->m.ramus, _campum_ponere, &par);
    insula_scriptorem_ponere(w->repo, prior);
    redde ok;
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


/* ==================================================
 * Psalmus (T6)
 * ================================================== */

/* titulus lineae imae: "Psalmus N · signum" (nomen ex gabc aptato) */
interior b32
_pes_est (
                Mundus* w,
               Piscina* p,
    constans character* expectatus)
{
    redde _textus_continet(_reddere(w, p), expectatus);
}

interior vacuum
_psalmus_probare (
    CredoContextus* c)
{
     PsalmodiaTabula* t = _tabula(c);
              Mundus* w = _mundum(c, NIHIL);
             Mandata* md;
                 s32  octavus;
                 i32  numerus;
              chorda  proximum;
              chorda  ultimum;
           character  pes[LXIV];
                 s32  pagina;

    CREDO_NECESSE (w != NIHIL && psalmodia_successus(t));
    numerus   = psalterium_signa_numerus(t);
    octavus   = psalterium_signi_index(t, chorda_ex_literis("8 G",
        c->piscina));
    proximum  = psalterium_signum(t, (i32)((octavus + I)
        % (s32)numerus),
        c->piscina);
    ultimum   = psalterium_signum(t, numerus - I, c->piscina);

    /* Enter in 109: visus psalmi, gabc "Psalmus 109" in 8 G */
    _littera(w, '1');
    _littera(w, '0');
    _littera(w, '9');
    _clavis(w, CLAVIS_REDITUS, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"),
        "psalmus");
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), CIX);
    CREDO_CHORDA_AEQUALIS_LITERIS (psalterium_titulus(&w->m,
        c->piscina),
        "Psalmus 109");
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Psalmus 109 \xC2\xB7 8 G"));
    /* annotatio cantus (gabc "annotation: 8 G;") supra initium */
    CREDO_VERUM (_textus_aequalis(md, "8 G"));
    /* caput: ad indicem, prior, proximus, tonus */
    CREDO_VERUM (_textus_aequalis(md, "\xE2\x89\xA1"));

    /* t: terminatio proxima ordine tabulae; T retro */
    _littera(w, 't');
    CREDO_CHORDA_AEQUALIS (_campus_textus(w, "signum"), proximum);
    sprintf(pes, "Psalmus 109 \xC2\xB7 %.*s", (integer)proximum.mensura,
        (constans character*)proximum.datum);
    CREDO_VERUM (_pes_est(w, c->piscina, pes));
    _littera(w, 'T');
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "signum"), "8 G");
    /* circulus in finibus redit */
    sprintf(pes, "109,%.*s", (integer)ultimum.mensura,
        (constans character*)ultimum.datum);
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m, pes));
    CREDO_CHORDA_AEQUALIS (_campus_textus(w, "signum"), ultimum);
    _littera(w, 't');
    CREDO_CHORDA_AEQUALIS (_campus_textus(w, "signum"),
        psalterium_signum(
        t, ZEPHYRUM, c->piscina));
    _littera(w, 'T');
    CREDO_CHORDA_AEQUALIS (_campus_textus(w, "signum"), ultimum);

    /* ] in CL et [ in I nihil */
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m, "150, 8 G"));
    _littera(w, ']');
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), CL);
    _littera(w, '[');
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), CXLIX);
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m, "1"));
    _littera(w, '[');
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), I);
    _littera(w, ']');
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), II);
    CREDO_VERUM (_pes_est(w, c->piscina, "Psalmus 2 \xC2\xB7 8 G"));

    /* tonus paginam servat (coercitam), psalmus novus paginam 0 */
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m, "118"));
    _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    _littera(w, 'j');
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), II);
    _littera(w, 't');
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), II);
    CREDO_NECESSE (_campum_scribere(w, "pagina", "999", c->piscina));
    _littera(w, 'T');
    pagina = _campus(w, "pagina", ZEPHYRUM);
    CREDO_VERUM (pagina > II && pagina < CMXCIX);
    _littera(w, ']');
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), CXIX);
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", -I), ZEPHYRUM);

    /* Esc: ad indicem, psalmus electus */
    _clavis(w, CLAVIS_EFFUGIUM, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"), "index");
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), CXIX);
    CREDO_AEQUALIS_I32 (_campus_textus(w, "psalmus").mensura, ZEPHYRUM);
    CREDO_VERUM (_pes_est(w, c->piscina, "119  "));

    /* ictus capitis: quod claves faciunt */
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m, "109, 8 G"));
    dispensator_recomponere(w->d);   /* vicus post argumentum */
    CREDO_VERUM (_ictus(w, "caput_proximus", c->piscina));
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), CX);
    CREDO_VERUM (_ictus(w, "caput_prior", c->piscina));
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), CIX);
    CREDO_VERUM (_ictus(w, "caput_signum", c->piscina));
    CREDO_CHORDA_AEQUALIS (_campus_textus(w, "signum"), proximum);
    CREDO_VERUM (_ictus(w, "caput_index", c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"), "index");
    CREDO_AEQUALIS_S32 (_campus(w, "electus", ZEPHYRUM), CIX);
}

/* Review Focus IV: tonus, psalmus, Esc sonum sistunt */
interior vacuum
_sonus_probare (
    CredoContextus* c)
{
     Mundus* w = _mundum(c, "109");
        i32  ante;

    CREDO_NECESSE (w != NIHIL);
    CREDO_AEQUALIS_S32 ((s32)psalterium_mora(&w->m), -I);
    _littera(w, ' ');
    CREDO_AEQUALIS_I32 (w->incepta, I);
    CREDO_VERUM (w->sonat);
    CREDO_AEQUALIS_S32 ((s32)psalterium_mora(&w->m), XXXIII);
    w->pulsus = CCC;
    (vacuum)psalterium_pulsare(&w->m);

    ante = w->sistita;
    _littera(w, 't');
    CREDO_FALSUM (w->sonat);
    CREDO_AEQUALIS_I32 (w->sistita, ante + I);
    CREDO_AEQUALIS_S32 ((s32)psalterium_mora(&w->m), -I);
    /* Spatium ab initio melodiae novae */
    _littera(w, ' ');
    CREDO_VERUM (w->sonat);
    CREDO_AEQUALIS_I32 (w->incepta, II);

    ante = w->sistita;
    _littera(w, ']');
    CREDO_FALSUM (w->sonat);
    CREDO_AEQUALIS_I32 (w->sistita, ante + I);

    _littera(w, ' ');
    CREDO_VERUM (w->sonat);
    ante = w->sistita;
    _clavis(w, CLAVIS_EFFUGIUM, ZEPHYRUM);
    CREDO_FALSUM (w->sonat);
    CREDO_AEQUALIS_I32 (w->sistita, ante + I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"), "index");
    CREDO_AEQUALIS_S32 ((s32)psalterium_mora(&w->m), -I);
}

/* Review Focus I: status servatus malus -> index aut 8 G, sine
 * vexillo, sine ruina */
interior vacuum
_status_malus_probare (
    CredoContextus* c)
{
     constans character* mali[III];
                 Mundus* w = _mundum(c, NIHIL);
                Mandata* md;
                    i32  k;

    CREDO_NECESSE (w != NIHIL);
    mali[ZEPHYRUM]  = "0";
    mali[I]         = "200";
    mali[II]        = "";
    per (k = ZEPHYRUM; k < III; k++)
    {
        CREDO_NECESSE (_campum_scribere(w, "visus", "psalmus",
            c->piscina));
        CREDO_NECESSE (_campum_scribere(w, "psalmus", mali[k],
            c->piscina));
        CREDO_VERUM (psalterium_semitam_secuta(&w->m));
        md = _reddere(w, c->piscina);
        CREDO_VERUM (_textus_continet(md, "  1  Be\xC3\xA1tus vir"));
        CREDO_CHORDA_AEQUALIS_LITERIS (psalterium_titulus(&w->m,
            c->piscina), "Psalterium");
        _littera(w, 't');
        _littera(w, ']');
        CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"),
            "psalmus");
    }
    /* signum ignotum: 8 G tacite */
    CREDO_NECESSE (_campum_scribere(w, "psalmus", "109", c->piscina));
    CREDO_NECESSE (_campum_scribere(w, "signum", "9 Q", c->piscina));
    CREDO_VERUM (psalterium_semitam_secuta(&w->m));
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "Psalmus 109 \xC2\xB7 8 G"));
    CREDO_FALSUM (_textus_continet(md, "9 Q"));
    CREDO_FALSUM (_textus_continet(md, "tonus"));
    /* t ab 8 G pergit */
    _littera(w, 't');
    CREDO_FALSUM (chorda_aequalis_literis(_campus_textus(w, "signum"),
        "9 Q"));
    CREDO_FALSUM (chorda_aequalis_literis(_campus_textus(w, "signum"),
        "8 G"));
}

/* Review Focus V: CC mutationes toni et psalmi - piscina montationis
 * non crescit */
interior vacuum
_memoria_probare (
    CredoContextus* c)
{
           Piscina* p = piscina_generare_dynamicum("psalterium_memoria",
                         (memoriae_index)(CCLVI * MXXIV));
            Mundus* w;
    memoriae_index  usus;
               i32  k;

    CREDO_NECESSE (p != NIHIL);
    w = _mundum_plenum(c, p, NIHIL, "109", LATITUDO, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    _littera(w, 't');
    _littera(w, ']');
    _littera(w, '[');
    (vacuum)_reddere(w, c->piscina);
    usus = piscina_summa_usus(p);
    per (k = ZEPHYRUM; k < CC; k++)
    {
        _littera(w, 't');
    }
    CREDO_AEQUALIS_I64 ((i64)piscina_summa_usus(p), (i64)usus);
    per (k = ZEPHYRUM; k < CC; k++)
    {
        _littera(w, (k % II) == ZEPHYRUM ? ']' : '[');
    }
    (vacuum)_reddere(w, c->piscina);
    CREDO_AEQUALIS_I64 ((i64)piscina_summa_usus(p), (i64)usus);
    piscina_destruere(p);
}

/* status servatus (Ps 109 in 2 A) post montationem novam: idem psalmus,
 * idem tonus; URI generis directe */
interior vacuum
_semita_probare (
    CredoContextus* c)
{
    Mundus* w = _mundum(c, "109, 2 A");
    Mundus* iterum;

    CREDO_NECESSE (w != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "signum"), "2 A");
    /* vicus: status in repositorio manet, montatio nova, semita
     * restituta, deinde semitam_secuta */
    iterum = _mundum_plenum(c, c->piscina, w->repo, NIHIL, LATITUDO,
        ALTITUDO);
    CREDO_NECESSE (iterum != NIHIL);
    CREDO_VERUM (psalterium_semitam_secuta(&iterum->m));
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(iterum, "visus"),
        "psalmus");
    CREDO_VERUM (_pes_est(iterum, c->piscina,
        "Psalmus 109 \xC2\xB7 2 A"));

    /* URI */
    w = _mundum(c, "psalterium/psalmus/109");
    CREDO_NECESSE (w != NIHIL);
    CREDO_AEQUALIS_S32 (_campus(w, "psalmus", ZEPHYRUM), CIX);
    CREDO_VERUM (_pes_est(w, c->piscina, "Psalmus 109 \xC2\xB7 8 G"));
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m,
        "psalterium/index"));
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"), "index");
    /* argumentum vacuum ($psalterium): index */
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m, "109"));
    CREDO_VERUM (psalterium_argumentum_ponere(&w->m, ""));
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"), "index");
    CREDO_FALSUM (psalterium_argumentum_ponere(&w->m,
        "psalterium/psalmus/151"));
    CREDO_FALSUM (psalterium_argumentum_ponere(&w->m, "151"));
    CREDO_CHORDA_AEQUALIS_LITERIS (_campus_textus(w, "visus"), "index");
    CREDO_VERUM (_mundum(c, "psalterium/psalmus/0") == NIHIL);
}

/* y rectanguli vexilli (COLOR_ERROR); -1 si nullum */
interior s32
_vexillum_y (
    Mandata* md)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus       == MANDATUM_RECTANGULUM
            && x->color.genus == COLOR_MANDATI_THEMA
            && x->color.valor == (i32)COLOR_ERROR)
        {
            redde x->fines.y;
        }
    }
    redde -I;
}

/* recensio finalis: vexillum (sonus mutus) caput non tegit; pagina 0
 * psalmo novo per historiam et post Esc; visus psalmi angustus */
interior vacuum
_recensio_probare (
    CredoContextus* c)
{
    Mundus* w = _mundum(c, "109");
   Mandata* md;

    CREDO_NECESSE (w != NIHIL);
    /* porta recusat: vexillum "can't play", caput manet */
    w->recusare = VERUM;
    _littera(w, ' ');
    md = _reddere(w, c->piscina);
    CREDO_VERUM (_textus_continet(md, "can't play"));
    CREDO_VERUM (_textus_aequalis(md, "\xE2\x89\xA1"));
    /* vexillum sub capite: limbus VIII + margo capitis XVI */
    CREDO_AEQUALIS_S32 (_vexillum_y(md), XXIV);
    w->recusare = FALSUM;

    /* historia: semita ad psalmum alium restituta -> pagina 0 */
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m, "118"));
    _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), I);
    CREDO_NECESSE (_campum_scribere(w, "psalmus", "119", c->piscina));
    CREDO_VERUM (psalterium_semitam_secuta(&w->m));
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), ZEPHYRUM);

    /* Esc deinde $psalterium(eundem): pagina 0 (ut Enter) */
    _clavis(w, CLAVIS_PAGINA_DEORSUM, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), I);
    _clavis(w, CLAVIS_EFFUGIUM, ZEPHYRUM);
    CREDO_NECESSE (psalterium_argumentum_ponere(&w->m, "119"));
    CREDO_AEQUALIS_S32 (_campus(w, "pagina", ZEPHYRUM), ZEPHYRUM);

    /* Review Focus II: visus psalmi angustus - nuntius "pane too
     * narrow", sine ruina */
    w = _mundum_mensura(c, "109", XL, ALTITUDO);
    CREDO_NECESSE (w != NIHIL);
    CREDO_VERUM (_textus_continet(_reddere(w, c->piscina),
        "pane too narrow"));
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
    { "psalmus: tonus, psalmi vicini, pagina, Esc, caput",
        _psalmus_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "psalmus: sonus sistitur", _sonus_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "status malus", _status_malus_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "memoria: CC mutationes", _memoria_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "semita: restitutio et URI", _semita_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "recensio: vexillum, pagina, angustus", _recensio_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("psalterium_latus", SECTIONES, NIHIL,
        NIHIL);
}
