/* probatio_dimissio.c - hami medii dispensatoris et systema
 * dimissionis (status-plan S5c) supra organum ludicrum 'pop' bis
 * montatum (spatia w1 ad x 0, w2 ad x 200):
 *   aperire  (10,10,20,20)   pop.aperire - palette commutat (dominus)
 *   pictura  (50,10,140,80)  pop.ictus   - ictus numerat
 *   palette  (60,20,80,60)   DIALOGUS, si aperta:
 *     optio.a    (5,5,20,10)   pop.eligere  claudit
 *     fila       (30,5,20,10)  pop.agere
 *     optio.mala (5,25,20,10)  pop.recusare claudit (non consumit)
 * radix (0,0,400,100) hospes.clavis - claves non consumptas numerat. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "stml.h"
#include "insula.h"
#include "actio.h"
#include "componens.h"
#include "dispensator.h"
#include "organum.h"
#include "dimissio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character manifestum[] =
    "<organum genus=\"pop\" versio=\"1\">"
    "<actiones><actio nomen=\"pop.aperire\"/></actiones><status>"
    "<campus nomen=\"id\" typus=\"nomen\" vita=\"persistens\"/>"
    "<campus nomen=\"palette\" typus=\"nomen\" vita=\"transiens\""
    " aspectus=\"popover\" scriptor=\"pop.aperire\"/>"
    "<campus nomen=\"ictus\" typus=\"numerus\" vita=\"persistens\"/>"
    "<campus nomen=\"agere\" typus=\"numerus\" vita=\"persistens\"/>"
    "<campus nomen=\"electum\" typus=\"nomen\" vita=\"persistens\"/>"
    "</status></organum>";

/* constans (status inter sectiones non manet, credo examen 92); ctx
 * mutatoris (vacuum*) copiam localem accipit */
hic_manens constans character attr_palette[] = "palette";
hic_manens constans character attr_ictus[]   = "ictus";
hic_manens constans character attr_agere[]   = "agere";
hic_manens constans character attr_claves[]  = "claves";


/* ==================================================
 * Mutatores et actiones
 * ================================================== */

interior vacuum
incrementare (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    chorda* a;
       s32  v;

    a = stml_attributum_capere(radix, (constans character*)ctx);
    v = ZEPHYRUM;
    si (a)
    {
        (vacuum)chorda_ut_s32(*a, &v);
    }
    insula_attributum_ponere(radix, p, in, (constans character*)ctx,
        chorda_ut_cstr(chorda_ex_s32(v + I, p), p));
}

interior vacuum
commutare_palettam (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    si (stml_attributum_capere(radix, attr_palette))
    {
        (vacuum)insula_attributum_tollere(radix, attr_palette);
        redde;
    }
    insula_attributum_ponere(radix, p, in, attr_palette, "colores");
}

interior vacuum
eligere_a (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "electum", "a");
}

interior InsulaRamus
ramus_nodi (
    InsulaRepositorium* repo,
             Componens* nodus)
{
    chorda sp;

    /* spatium = via rami "pop:<id>" (status-plan S8c) */
    sp = componens_spatium(nodus);
    redde insula_ramus(repo, "pop", chorda_ut_cstr(chorda_sectio(sp, IV,
        sp.mensura), repo->piscina));
}

interior b32
actio_aperire (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    InsulaRamus r;

    (vacuum)motus; (vacuum)des; (vacuum)ctx;
    si (ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    r = ramus_nodi(repo, nodus);
    redde mutare_ramum(&r, commutare_palettam, NIHIL);
}

interior b32
actio_ictus (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    InsulaRamus r;
      character titulus[VIII];

    (vacuum)motus; (vacuum)des; (vacuum)ctx;
    si (ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    r = ramus_nodi(repo, nodus);
    strcpy(titulus, attr_ictus);
    redde mutare_ramum(&r, incrementare, titulus);
}

interior b32
actio_eligere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    InsulaRamus r;

    (vacuum)motus; (vacuum)des; (vacuum)ctx;
    si (ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    r = ramus_nodi(repo, nodus);
    redde mutare_ramum(&r, eligere_a, NIHIL);
}

interior b32
actio_agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    InsulaRamus r;
      character titulus[VIII];

    (vacuum)motus; (vacuum)des; (vacuum)ctx;
    si (ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    r = ramus_nodi(repo, nodus);
    strcpy(titulus, attr_agere);
    redde mutare_ramum(&r, incrementare, titulus);
}

interior b32
actio_recusare (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    (vacuum)repo; (vacuum)motus; (vacuum)des; (vacuum)nodus;
    (vacuum)ev; (vacuum)ctx;
    redde FALSUM;
}

/* hospes: claves non consumptas numerat (in radice ephemerae) */
interior b32
actio_clavis (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    character titulus[VIII];

    (vacuum)motus; (vacuum)des; (vacuum)nodus; (vacuum)ctx;
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    strcpy(titulus, attr_claves);
    redde insula_mutare(repo, incrementare, titulus);
}


/* ==================================================
 * Compositio
 * ================================================== */

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  lat,
                    s32  alt,
     constans character* actio)
{
    Componens* c;
        Fines  f;

    c    = componens_creare(p, in, id, partes);
    f.x  = x; f.y = y; f.latitudo = lat; f.altitudo = alt;
    componens_ponere_fines(c, f);
    si (actio)
    {
        componens_ponere_actio(c, actio);
    }
    redde c;
}

interior Componens*
organum_ludicrum (
     InsulaRepositorium* repo,
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                    s32  x)
{
     Componens* w;
     Componens* pal;
     Componens* o;
   InsulaRamus  r;

    w = nodus(p, in, "w", PARTES_NULLUM, x, ZEPHYRUM, CC, C, NIHIL);
    r = insula_ramus(repo, "pop", id);
    componens_ponere_spatium(w, chorda_ut_cstr(r.via, p));
    componens_addere_liberum(w, nodus(p, in, "aperire", PARTES_BOTTONE,
        X, X, XX, XX, "pop.aperire"));
    componens_addere_liberum(w, nodus(p, in, "pictura", PARTES_TABULA,
        L, X, CXL, LXXX, "pop.ictus"));
    si (insula_ramus_attributum(&r, attr_palette))
    {
        pal = nodus(p, in, "palette", PARTES_DIALOGUS, LX, XX, LXXX, LX,
            NIHIL);
        o = nodus(p, in, "optio.a", PARTES_BOTTONE, V, V, XX, X,
            "pop.eligere");
        o->claudit = VERUM;
        componens_addere_liberum(pal, o);
        componens_addere_liberum(pal, nodus(p, in, "fila",
            PARTES_BOTTONE,
            XXX, V, XX, X, "pop.agere"));
        o = nodus(p, in, "optio.mala", PARTES_BOTTONE, V, XXV, XX, X,
            "pop.recusare");
        o->claudit = VERUM;
        componens_addere_liberum(pal, o);
        componens_addere_liberum(w, pal);
    }
    redde w;
}

interior Componens*
componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    Componens* radix;

    (vacuum)motus;
    (vacuum)ctx;
    radix = nodus(p, in, "radix", PARTES_NULLUM, ZEPHYRUM, ZEPHYRUM, CD,
        C, "hospes.clavis");
    componens_addere_liberum(radix, organum_ludicrum(repo, p, in, "w1",
        ZEPHYRUM));
    componens_addere_liberum(radix, organum_ludicrum(repo, p, in, "w2",
        CC));
    redde radix;
}


/* ==================================================
 * Mundus
 * ================================================== */

nomen structura {
      InsulaRepositorium* repo;
             Dispensator* d;
                Dimissio* dimissio;
             InsulaRamus  w1;
             InsulaRamus  w2;
} Mundus;

interior ActioRegistrum*
registrum_organi (
                Piscina* p,
    InternamentumChorda* in)
{
    ActioRegistrum* r;

    r = actio_registrum_creare(p, in);
    (vacuum)actio_registrare(r, "pop.aperire", actio_aperire, NIHIL);
    (vacuum)actio_registrare(r, "pop.ictus", actio_ictus, NIHIL);
    (vacuum)actio_registrare(r, "pop.eligere", actio_eligere, NIHIL);
    (vacuum)actio_registrare(r, "pop.agere", actio_agere, NIHIL);
    (vacuum)actio_registrare(r, "pop.recusare", actio_recusare, NIHIL);
    redde r;
}

/* cum_dimissione FALSUM: dispensator nudus (hami nulli) */
interior b32
mundum_creare (
                 Mundus* m,
                Piscina* p,
    InternamentumChorda* in,
                    b32  cum_dimissione)
{
     ActioRegistrum* reg;
            Organum* o;
                Xar* vitia;
          StmlNodus* domini;

    memset(m, ZEPHYRUM, magnitudo(*m));
    m->repo = insula_repositorium_creare(p, in,
        "<hospes><pop id=\"w1\"/><pop id=\"w2\"/></hospes>");
    o = organum_legere(chorda_ex_literis(manifestum, p), p, in, &vitia);
    si (!m->repo || !o)
    {
        redde FALSUM;
    }
    m->w1   = insula_ramus(m->repo, "pop", "w1");
    m->w2   = insula_ramus(m->repo, "pop", "w2");
    domini  = organum_dominos_generare(o, p, in);
    (vacuum)insula_ramus_dominos_legere(&m->w1,
        domini);
    (vacuum)insula_ramus_dominos_legere(&m->w2,
        domini);
    reg = actio_registrum_creare(p, in);
    (vacuum)actio_registrare(reg, "hospes.clavis", actio_clavis, NIHIL);
    (vacuum)actio_registrum_miscere_in_spatio(reg, registrum_organi(p,
        in), m->w1.via);
    (vacuum)actio_registrum_miscere_in_spatio(reg, registrum_organi(p,
        in), m->w2.via);
    m->d = dispensator_creare(p, in, m->repo, reg, componere, NIHIL,
        CCC);
    si (!m->d)
    {
        redde FALSUM;
    }
    si (cum_dimissione)
    {
        m->dimissio = dimissio_creare(p, in, m->repo);
        si (   !m->dimissio
            || !dimissio_ramum_addere(m->dimissio, &m->w1, o)
            || !dimissio_ramum_addere(m->dimissio, &m->w2, o))
        {
            redde FALSUM;
        }
        dimissio_dispensatori_ligare(m->dimissio, m->d);
    }
    redde VERUM;
}

interior vacuum
ictus (
    Mundus* m,
       s32  x,
       s32  y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x  = x;
    e.datum.mus.y  = y;
    dispensator_tractare(m->d, &e);
    e.genus = EVENTUS_MUS_LIBERATUS;
    dispensator_tractare(m->d, &e);
}

interior vacuum
effugium (
    Mundus* m)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = CLAVIS_EFFUGIUM;
    e.datum.clavis.producta  = XXVII;
    dispensator_tractare(m->d, &e);
}

interior b32
valor_est (
    constans InsulaRamus* r,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(r, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

interior b32
claves_sunt (
                Mundus* m,
    constans character* valor)
{
    chorda* a;

    a = insula_attributum(m->repo, attr_claves);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}


/* ==================================================
 * I. hami medii dispensatoris (sine dimissione)
 * ================================================== */

nomen structura {
          b32 consumere;
          i32 post_vocationes;
    character actor[XXXII];
} Medium;

interior b32
ante_ludicrum (
                 vacuum* ctx,
            Dispensator* d,
    constans Destinatio* des,
       constans Eventus* e)
{
    (vacuum)d; (vacuum)des;
    redde ((Medium*)ctx)->consumere
        && e->genus == EVENTUS_MUS_DEPRESSUS;
}

interior vacuum
post_ludicrum (
                 vacuum* ctx,
            Dispensator* d,
     constans Componens* actor,
       constans Eventus* e)
{
    Medium* m;

    (vacuum)d; (vacuum)e;
    m = (Medium*)ctx;
    m->post_vocationes++;
    sprintf(m->actor, "%.*s", (int)actor->id.mensura,
        (constans character*)actor->id.datum);
}

interior vacuum
_probare_medium (
    CredoContextus* c)
{
    InternamentumChorda* in;
                 Mundus  m;
                 Medium  md;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, FALSUM));
    memset(&md, ZEPHYRUM, magnitudo(md));
    dispensator_medium_ponere(m.d, ante_ludicrum, post_ludicrum, &md);

    /* post: actor consumens */
    ictus(&m, C, LXXX);
    CREDO_VERUM(valor_est(&m.w1, attr_ictus, "1"));
    CREDO_AEQUALIS_I32(md.post_vocationes, I);
    CREDO_VERUM(strcmp(md.actor, "pictura") == ZEPHYRUM);
    /* ante consumit: actio nulla, post nullum */
    md.consumere = VERUM;
    ictus(&m, C, LXXX);
    CREDO_VERUM(valor_est(&m.w1, attr_ictus, "1"));
    CREDO_AEQUALIS_I32(md.post_vocationes, I);
    /* hami sublati: ut olim */
    dispensator_medium_ponere(m.d, NIHIL, NIHIL, NIHIL);
    ictus(&m, C, LXXX);
    CREDO_VERUM(valor_est(&m.w1, attr_ictus, "2"));
}


/* ==================================================
 * II. dimissio: Esc
 * ================================================== */

interior vacuum
_probare_effugium (
    CredoContextus* c)
{
    InternamentumChorda* in;
                 Mundus  m;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, VERUM));
    m.d->motus.spatium = m.w1.via;
    /* nihil apertum: Esc transit ad hospitem */
    effugium(&m);
    CREDO_VERUM(claves_sunt(&m, "1"));
    ictus(&m, XV, XV);                         /* w1 aperire */
    CREDO_VERUM(valor_est(&m.w1, attr_palette, "colores"));
    effugium(&m);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, NIHIL));
    CREDO_VERUM(claves_sunt(&m, "1"));         /* consumptus */
    /* gestus in cursu (captura) interior est: Esc transit, popover
     * manet; captura soluta Esc eum claudit */
    ictus(&m, XV, XV);
    m.d->motus.captura = chorda_ex_literis("tractus", c->piscina);
    effugium(&m);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, "colores"));
    CREDO_VERUM(claves_sunt(&m, "2"));
    m.d->motus.captura.mensura = ZEPHYRUM;
    effugium(&m);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, NIHIL));
}


/* ==================================================
 * III. ictus: extra (consumitur), intus, aperiens, alius organi
 * ================================================== */

interior vacuum
_probare_ictus (
    CredoContextus* c)
{
    InternamentumChorda* in;
                 Mundus  m;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, VERUM));

    /* extra in eodem organo: claudit, consumitur (pictura intacta) */
    ictus(&m, XV, XV);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, "colores"));
    ictus(&m, CLXX, LXXXV);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, NIHIL));
    CREDO_VERUM(valor_est(&m.w1, attr_ictus, NIHIL));
    /* clausa: ictus pingit */
    ictus(&m, CLXX, LXXXV);
    CREDO_VERUM(valor_est(&m.w1, attr_ictus, "1"));

    /* intus (fila): manet aperta, actio currit */
    ictus(&m, XV, XV);
    ictus(&m, XCV, XXX);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, "colores"));
    CREDO_VERUM(valor_est(&m.w1, attr_agere, "1"));

    /* aperiens: commutat (dominus ipse) */
    ictus(&m, XV, XV);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, NIHIL));
    ictus(&m, XV, XV);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, "colores"));

    /* organum alterum: w1 clauditur, ictus transit ad w2 */
    ictus(&m, CC + CLXX, LXXXV);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, NIHIL));
    CREDO_VERUM(valor_est(&m.w2, attr_ictus, "1"));
    /* aperire in w2 dum w1 aperta: w1 clauditur, w2 aperitur */
    ictus(&m, XV, XV);
    ictus(&m, CC + XV, XV);
    CREDO_VERUM(valor_est(&m.w1, attr_palette, NIHIL));
    CREDO_VERUM(valor_est(&m.w2, attr_palette, "colores"));
}


/* ==================================================
 * IV. claudit: post actionem successam
 * ================================================== */

interior vacuum
_probare_claudit (
    CredoContextus* c)
{
    InternamentumChorda* in;
                 Mundus  m;

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, VERUM));
    ictus(&m, XV, XV);
    ictus(&m, LXX, XXX);                       /* optio.a */
    CREDO_VERUM(valor_est(&m.w1, "electum", "a"));
    CREDO_VERUM(valor_est(&m.w1, attr_palette, NIHIL));
    /* claudit sed actio non consumit: manet aperta */
    ictus(&m, XV, XV);
    ictus(&m, LXX, L);                         /* optio.mala */
    CREDO_VERUM(valor_est(&m.w1, attr_palette, "colores"));
}


/* ==================================================
 * V. domini: palette solius aperientis et dimissionis
 * ================================================== */

interior vacuum
_probare_dominos (
    CredoContextus* c)
{
       InternamentumChorda* in;
                    Mundus  m;
                 character  par[] = "x";

    in = internamentum_creare(c->piscina);
    CREDO_NECESSE(mundum_creare(&m, c->piscina, in, VERUM));
    insula_scriptorem_ponere(m.repo, chorda_ex_literis("pop.ictus",
        c->piscina));
    CREDO_FALSUM(mutare_ramum(&m.w1,
        commutare_palettam,
        par));
    insula_scriptorem_ponere(m.repo, chorda_ex_literis("dimissio",
        c->piscina));
    CREDO_VERUM(mutare_ramum(&m.w1, commutare_palettam,
        par));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "medium",   _probare_medium,   NIHIL, NIHIL, NIHIL, NIHIL },
    { "effugium", _probare_effugium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ictus",    _probare_ictus,    NIHIL, NIHIL, NIHIL, NIHIL },
    { "claudit",  _probare_claudit,  NIHIL, NIHIL, NIHIL, NIHIL },
    { "domini",   _probare_dominos,  NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("dimissio", SECTIONES);
}
