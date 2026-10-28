/* probatio_pictor_inscriptio.c - salutatio et inscriptio in pictore
 * (communio-plan-5 T7; communio-spec IX.1-2): salutatio semel, "Draw
 * without signing in" -> menu "Sign in…"; inscriptio per codicem et
 * per nexum, nuntii (codex falsus, rete, expirata), campus (RETRORSUM,
 * REDITUS, limes), Cancel, resumptio post applicationem necatam;
 * epistula numquam in volumine. Servus in processu, epistula ficta. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "internamentum.h"
#include "thema.h"
#include "cives.h"
#include "moneta.h"
#include "reactor.h"
#include "hospitium.h"
#include "http.h"
#include "http_vocatio.h"
#include "epistula.h"
#include "communio_servus.h"
#include "communio_cliens.h"
#include "veredarius_domesticus.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "lectiones.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_figurae.h"
#include "pictor_applicatio.h"
#include "pictor_communio.h"
#include "testa_cursus.h"
#include "filum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO_TELEPHONI  CCXV
#define ALTITUDO_TELEPHONI  CDXIX

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;
/* servitium claviaturae fictum: genus ultimum et numerus vocationum */
hic_manens i32                  claviatura_genus;
hic_manens i32                  claviatura_vocationes;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina                = c->piscina;
    intern                 = internamentum_creare(piscina);
    claviatura_genus       = (i32)TESTA_CLAVIATURA_NULLA;
    claviatura_vocationes  = ZEPHYRUM;
    thema_initiare();
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina                = NIHIL;
    intern                 = NIHIL;
    claviatura_genus       = ZEPHYRUM;
    claviatura_vocationes  = ZEPHYRUM;
}

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

interior vacuum
claviaturam_fictam_ponere (
    vacuum* ctx,
       i32  genus)
{
    (vacuum)ctx;
    claviatura_genus = genus;
    claviatura_vocationes++;
}

/* servus: Franciscus in familia, epistula permissa, provisor fictus */
nomen structura {
           Volumen* vs;
             Cives* cives;
    CommunioServus* servus;
           Reactor* reactor;
        Veredarius* veredarius;
         Hospitium* hospitium;
      Epistularius* epistularius;
               i32  portus;
         character  basis[LXIV];
         character  via[CXXVIII];
} Orbis;

interior b32
hospitium_aperire (
    Orbis* o)
{
    HospitiumConfiguratio cfg;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.reactor   = o->reactor;
    cfg.portus    = o->portus;
    o->hospitium  = hospitium_creare(piscina, &cfg);
    si (   !o->hospitium || !communio_servum_montare(o->servus,
            o->hospitium))
    {
        redde FALSUM;
    }
    o->portus = (i32)hospitium_portus(o->hospitium);
    sprintf(o->basis, "http://127.0.0.1:%d", (integer)o->portus);
    redde VERUM;
}

interior b32
orbem_parare (
    Orbis* o)
{
       chorda primus;
    character moneta[MONETA_ULID_MENSURA];

    memset(o, ZEPHYRUM, magnitudo(*o));
    o->vs = volumen_temporarium(piscina,
        "probatio_pictor_inscriptio_s");
    o->cives = o->vs ? cives_aperire(o->vs, NIHIL, piscina) : NIHIL;
    si (!o->cives || !volumen_transactionem_incipere(o->vs))
    {
        redde FALSUM;
    }
    (vacuum)cives_creare(o->cives, lit("franciscus"), lit("Franciscus"),
        &primus, piscina);
    (vacuum)volumen_transactionem_committere(o->vs);
    o->servus      = communio_servum_creare(piscina, o->vs, o->cives,
        NIHIL);
    o->reactor     = reactor_creare(piscina);
    o->veredarius  = o->reactor ? veredarium_domesticum_creare(piscina,
        o->reactor) : NIHIL;
    si (!o->servus || !o->veredarius || !hospitium_aperire(o))
    {
        redde FALSUM;
    }
    o->epistularius = epistularium_aperire(piscina, o->reactor,
        "fictus",
        lit(""), NIHIL);
    communio_servum_epistularium_ponere(o->servus, o->epistularius,
        "communio@example.org", o->basis);
    moneta_ulid(moneta);
    sprintf(o->via, "/tmp/probatio_pictor_inscriptio_%s", moneta);
    redde o->epistularius && communio_membrum_addere(o->servus,
        lit("familia"), primus) && communio_epistulam_permittere(
        o->servus, lit("fran@example.org"), primus, lit("familia"));
}

interior vacuum
orbem_purgare (
    Orbis* o)
{
    (vacuum)filum_delere(o->via);
    epistularium_claudere(o->epistularius);
    communio_servum_destruere(o->servus);
    si (o->hospitium)
    {
        hospitium_destruere(o->hospitium);
    }
    reactor_destruere(o->reactor);
    volumen_claudere(o->vs);
}

/* applicatio cum communione ligata (ut apps/pictor/pictor.c) */
nomen structura {
    PictorApplicatio  app;
      PictorCommunio* pc;
          ManusLudus* m;
} Pictor;

interior b32
pictorem_aedificare (
            Pictor* p,
             Orbis* o,
           Volumen* v,
constans character* basis)
{
    si (!pictor_applicatio_aedificare(&p->app, piscina, intern, v,
            LATITUDO_TELEPHONI, ALTITUDO_TELEPHONI))
    {
        redde FALSUM;
    }
    p->app.montatio.compositio.elenchus_praesto = VERUM;
    p->app.montatio.actiones_ctx.claviaturam_ponere =
        claviaturam_fictam_ponere;
    p->pc = pictor_communionem_aperire(piscina, o->reactor,
        o->veredarius,
        v, o->via, basis ? basis : o->basis, NIHIL, NIHIL);
    si (!p->pc)
    {
        redde FALSUM;
    }
    pictor_communio_moram_ponere(p->pc, ZEPHYRUM);
    pictor_communio_inscriptionis_ponere(p->pc, L, ZEPHYRUM);
    pictor_communionem_ligare(p->pc, &p->app.montatio, p->app.d, "Mac");
    (vacuum)pictor_communio_inscriptionem_resumere(p->pc);
    pictor_communio_incipere(p->pc);
    pictor_salutationem_ponere(&p->app.montatio);
    dispensator_recomponere(p->app.d);
    p->m = manus_ludus_creare(piscina, p->app.d);
    redde p->m != NIHIL;
}

interior vacuum
origo (
    constans Componens* n,
                   s32* x,
                   s32* y)
{
    *x = ZEPHYRUM;
    *y = ZEPHYRUM;
    dum (n != NIHIL)
    {
        *x  += n->fines.x;
        *y  += n->fines.y;
        n   = n->parens;
    }
}

interior Componens*
nodus_arboris (
            Pictor* p,
constans character* id)
{
    redde componens_invenire_per_id(dispensator_arbor(p->app.d),
        chorda_ex_literis(id, piscina));
}

interior chorda
titulus_nodi (
            Pictor* p,
constans character* id)
{
    Componens* n;

    dispensator_recomponere(p->app.d);
    n = nodus_arboris(p, id);
    redde n ? chorda_ex_literis(chorda_ut_cstr(n->titulus, piscina),
        piscina) : lit("");
}

interior b32
premere_nodum (
            Pictor* p,
constans character* id)
{
    Componens* n;
          s32  x;
          s32  y;

    dispensator_recomponere(p->app.d);
    n = nodus_arboris(p, id);
    si (!n)
    {
        imprimere("  nodus abest: %s\n", id);
        redde FALSUM;
    }
    origo(n, &x, &y);
    (vacuum)manus_ludus_premere_ad(p->m, x + n->fines.latitudo / II,
        y + n->fines.altitudo / II);
    dispensator_recomponere(p->app.d);
    redde VERUM;
}

interior chorda
ephemera (
            Pictor* p,
constans character* titulus)
{
    chorda* a;

    a = insula_attributum(p->app.repo, titulus);
    redde a ? chorda_ex_literis(chorda_ut_cstr(*a, piscina), piscina)
            : lit("");
}

interior b32
donec_gradus (
     Orbis* o,
    Pictor* p,
       s32  gradus,
       i32  tectum_ms)
{
    i32 gyri;

    per (gyri = ZEPHYRUM; pictor_communio_inscriptionis_gradus(p->pc)
         != gradus && gyri < tectum_ms / X; gyri++)
    {
        (vacuum)reactor_poll(o->reactor, X);
    }
    redde pictor_communio_inscriptionis_gradus(p->pc) == gradus;
}

interior b32
donec_titulus (
             Orbis* o,
            Pictor* p,
constans character* id,
constans character* titulus,
               i32  tectum_ms)
{
    i32 gyri;

    per (gyri = ZEPHYRUM; !chorda_aequalis_literis(titulus_nodi(p, id),
         titulus) && gyri < tectum_ms / X; gyri++)
    {
        (vacuum)reactor_poll(o->reactor, X);
    }
    redde chorda_aequalis_literis(titulus_nodi(p, id), titulus);
}

/* codex ex epistula ficta ultima ("Code: XXXX-XXXX") */
interior chorda
codex_epistulae (
    Orbis* o)
{
    EpistulaNuntius n;
                i32 gyri;
                i32 k;
                i32 i;

    per (gyri = ZEPHYRUM; epistularius_fictus_numerus(o->epistularius)
         == ZEPHYRUM && gyri < C; gyri++)
    {
        (vacuum)reactor_poll(o->reactor, X);
    }
    k = epistularius_fictus_numerus(o->epistularius);
    si (k == ZEPHYRUM || !epistularius_fictus_nuntius(o->epistularius,
            k - I, &n))
    {
        redde lit("");
    }
    per (i = ZEPHYRUM; i + VI <= n.textus.mensura; i++)
    {
        si (memcmp(n.textus.datum + i, "Code: ", VI) == ZEPHYRUM)
        {
            redde chorda_ex_literis(chorda_ut_cstr(chorda_sectio(n.textus,
                i + VI, i + VI + IX), piscina), piscina);
        }
    }
    redde lit("");
}

interior vacuum
_finis_nexus (
         vacuum* datum,
    HttpVocatio* vocatio)
{
    (vacuum)vocatio;
    *(b32*)datum = VERUM;
}

/* nexus epistulae ultimae, bulla paginae pressa (POST formae) */
interior b32
nexum_premere (
    Orbis* o)
{
    EpistulaNuntius  n;
             chorda  forma;
        HttpPetitio* pet;
        HttpVocatio* v;
                i32  k;
                i32  i;
                i32  q;
                i32  gyri;
                b32  finita;
          character  actio[CXXVIII];

    k = epistularius_fictus_numerus(o->epistularius);
    si (k == ZEPHYRUM || !epistularius_fictus_nuntius(o->epistularius,
            k - I, &n))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n.textus.mensura && n.textus.datum[i] != '?';
         i++)
    {
    }
    per (q = i; q < n.textus.mensura && n.textus.datum[q] != '\n'; q++)
    {
    }
    si (i >= n.textus.mensura)
    {
        redde FALSUM;
    }
    forma = chorda_sectio(n.textus, i + I, q);
    sprintf(actio, "%s/communio/confirmare", o->basis);
    pet = http_petitio_creare(piscina, HTTP_POST, actio);
    si (!pet)
    {
        redde FALSUM;
    }
    http_petitio_caput_addere(pet, "Content-Type",
        "application/x-www-form-urlencoded");
    http_petitio_corpus_ponere_chorda(pet, forma);
    finita = FALSUM;
    v = http_vocare(o->reactor, pet, NIHIL, NIHIL, _finis_nexus,
        &finita,
        NIHIL, piscina);
    per (gyri = ZEPHYRUM; v && !finita && gyri < CC; gyri++)
    {
        (vacuum)reactor_poll(o->reactor, X);
    }
    redde v != NIHIL && finita;
}

interior chorda
pendens_legere (
    Volumen* v)
{
    b32 inventum;

    redde volumen_praesens_legere(v, lit("communio/inscriptio"),
        piscina,
        &inventum);
}

interior vacuum
_sectio_salutatio (
    CredoContextus* c)
{
       Orbis  o;
     Volumen* v;
      Pictor  p;
      Pictor  q;
   Componens* e;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    v = volumen_temporarium(piscina, "probatio_pictor_inscriptio_a");
    CREDO_NECESSE(v && pictorem_aedificare(&p, &o, v, NIHIL));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera(&p, "palette"), "salutatio");
    CREDO_VERUM(nodus_arboris(&p, "salutatio.inscribere") != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&p, "salutatio.pingere"),
        "textus:Draw without signing in");

    CREDO_GRADUS("Draw without signing in -> menu Sign in…");
    CREDO_VERUM(premere_nodum(&p, "salutatio.pingere"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera(&p, "palette"), "");
    CREDO_VERUM(premere_nodum(&p, "quadratum.elenchus"));
    e = nodus_arboris(&p, "elenchus");
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32(e->liberi ? xar_numerus(e->liberi) : ZEPHYRUM,
        IV);
    CREDO_VERUM(premere_nodum(&p, "elenchus.3"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera(&p, "palette"),
        "inscriptio");

    CREDO_GRADUS("iterum: salutatio non redit");
    CREDO_NECESSE(pictorem_aedificare(&q, &o, v, NIHIL));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera(&q, "palette"), "");
    pictor_communionem_claudere(p.pc);
    pictor_communionem_claudere(q.pc);
    volumen_claudere(v);
    orbem_purgare(&o);
}

interior vacuum
_sectio_codice (
    CredoContextus* c)
{
     Orbis  o;
   Volumen* v;
    Pictor  p;
    chorda  codex;
 character  t[XVI];

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    v = volumen_temporarium(piscina, "probatio_pictor_inscriptio_b");
    CREDO_NECESSE(v && pictorem_aedificare(&p, &o, v, NIHIL));
    CREDO_VERUM(premere_nodum(&p, "salutatio.inscribere"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera(&p, "palette"),
        "inscriptio");
    CREDO_AEQUALIS_I32(claviatura_genus,
        (i32)TESTA_CLAVIATURA_EPISTULA);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&p,
        "inscriptio.titulus"),
        "pagina:Email address");
    CREDO_VERUM(manus_ludus_scribere(p.m, "fran@example.org"));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&p, "inscriptio.campus"),
        "campus:fran@example.org");
    CREDO_VERUM(premere_nodum(&p, "inscriptio.mittere"));
    CREDO_VERUM(donec_titulus(&o, &p, "inscriptio.titulus",
        "pagina:Check your email", M * V));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&p, "inscriptio.nota"),
        "pagina:Tap the link we sent,");
    CREDO_AEQUALIS_I32(claviatura_genus, (i32)TESTA_CLAVIATURA_CODEX);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&p, "inscriptio.campus"),
        "campus:");
    CREDO_VERUM(donec_gradus(&o, &p, (s32)INSCRIPTIO_EXSPECTAT, M * V));
    codex = codex_epistulae(&o);
    CREDO_NECESSE(codex.mensura == IX);
    memcpy(t, codex.datum, (size_t)IX);
    t[IX] = '\0';
    CREDO_VERUM(manus_ludus_scribere(p.m, t));
    CREDO_VERUM(manus_ludus_clavem(p.m, (character)CLAVIS_REDITUS,
        ZEPHYRUM));
    CREDO_VERUM(donec_titulus(&o, &p, "inscriptio.titulus",
        "pagina:Signed in as Franciscus", M * V));
    CREDO_AEQUALIS_I32(claviatura_genus, (i32)TESTA_CLAVIATURA_NULLA);
    CREDO_VERUM(pictor_communio_inscripta(p.pc));
    CREDO_VERUM(premere_nodum(&p, "inscriptio.confirmata"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera(&p, "palette"), "");
    CREDO_VERUM(premere_nodum(&p, "quadratum.elenchus"));
    CREDO_AEQUALIS_I32(xar_numerus(nodus_arboris(&p,
        "elenchus")->liberi),
        III);
    CREDO_FALSUM(chorda_continet(pendens_legere(v),
        lit("example.org")));
    pictor_communionem_claudere(p.pc);
    volumen_claudere(v);
    orbem_purgare(&o);
}

interior vacuum
_sectio_nexu (
    CredoContextus* c)
{
     Orbis  o;
   Volumen* v;
    Pictor  p;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    v = volumen_temporarium(piscina, "probatio_pictor_inscriptio_c");
    CREDO_NECESSE(v && pictorem_aedificare(&p, &o, v, NIHIL));
    CREDO_VERUM(premere_nodum(&p, "salutatio.inscribere"));
    CREDO_VERUM(manus_ludus_scribere(p.m, "fran@example.org"));
    CREDO_VERUM(manus_ludus_clavem(p.m, (character)CLAVIS_REDITUS,
        ZEPHYRUM));
    CREDO_VERUM(donec_gradus(&o, &p, (s32)INSCRIPTIO_EXSPECTAT, M * V));
    CREDO_NECESSE(codex_epistulae(&o).mensura == IX);
    CREDO_VERUM(nexum_premere(&o));
    CREDO_VERUM(donec_titulus(&o, &p, "inscriptio.titulus",
        "pagina:Signed in as Franciscus", M * V));
    pictor_communionem_claudere(p.pc);
    volumen_claudere(v);
    orbem_purgare(&o);
}

interior vacuum
_sectio_errores (
    CredoContextus* c)
{
     Orbis  o;
   Volumen* v;
    Pictor  p;
    Pictor  q;
 character  prolixum[CCCI];

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    v = volumen_temporarium(piscina, "probatio_pictor_inscriptio_d");
    CREDO_NECESSE(v && pictorem_aedificare(&p, &o, v, NIHIL));
    CREDO_VERUM(premere_nodum(&p, "salutatio.inscribere"));

    CREDO_GRADUS("campus: RETRORSUM, limes");
    CREDO_VERUM(manus_ludus_scribere(p.m, "abc"));
    CREDO_VERUM(manus_ludus_clavem(p.m, (character)CLAVIS_RETRORSUM,
        ZEPHYRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&p, "inscriptio.campus"),
        "campus:ab");
    CREDO_VERUM(manus_ludus_clavem(p.m, (character)CLAVIS_RETRORSUM,
        ZEPHYRUM));
    CREDO_VERUM(manus_ludus_clavem(p.m, (character)CLAVIS_RETRORSUM,
        ZEPHYRUM));
    memset(prolixum, 'a', CCC);
    prolixum[CCC] = '\0';
    CREDO_VERUM(manus_ludus_scribere(p.m, prolixum));
    CREDO_AEQUALIS_I32(ephemera(&p, "inscriptio_textus").mensura,
        CCLIV);
    per (; ephemera(&p, "inscriptio_textus").mensura > ZEPHYRUM;)
    {
        CREDO_NECESSE(manus_ludus_clavem(p.m,
            (character)CLAVIS_RETRORSUM, ZEPHYRUM));
    }

    CREDO_GRADUS("codex falsus; codex non ASCII");
    CREDO_VERUM(manus_ludus_scribere(p.m, "fran@example.org"));
    CREDO_VERUM(premere_nodum(&p, "inscriptio.mittere"));
    CREDO_VERUM(donec_gradus(&o, &p, (s32)INSCRIPTIO_EXSPECTAT, M * V));
    CREDO_VERUM(manus_ludus_scribere(p.m, "ZZZZ-ZZZZ"));
    CREDO_VERUM(premere_nodum(&p, "inscriptio.mittere"));
    CREDO_VERUM(donec_titulus(&o, &p, "inscriptio.error",
        "pagina:That code isn't right.", M * V));
    CREDO_VERUM(manus_ludus_scribere(p.m, "\xC3\xA9\xC3\xA9"));
    CREDO_VERUM(premere_nodum(&p, "inscriptio.mittere"));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&p, "inscriptio.error"),
        "pagina:That code isn't right.");

    CREDO_GRADUS("Cancel: tessera deleta, palette clausa");
    CREDO_VERUM(premere_nodum(&p, "inscriptio.abicere"));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera(&p, "palette"), "");
    CREDO_FALSUM(filum_existit(o.via));
    CREDO_AEQUALIS_S32(pictor_communio_inscriptionis_gradus(p.pc), -I);
    pictor_communionem_claudere(p.pc);

    CREDO_GRADUS("rete: Can't reach the server.");
    CREDO_NECESSE(pictorem_aedificare(&q, &o, v, "http://127.0.0.1:9"));
    CREDO_VERUM(premere_nodum(&q, "quadratum.elenchus"));
    CREDO_VERUM(premere_nodum(&q, "elenchus.3"));
    CREDO_VERUM(manus_ludus_scribere(q.m, "fran@example.org"));
    CREDO_VERUM(premere_nodum(&q, "inscriptio.mittere"));
    CREDO_VERUM(donec_titulus(&o, &q, "inscriptio.error",
        "pagina:Can't reach the server. Try again.", M * V));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&q,
        "inscriptio.titulus"),
        "pagina:Email address");
    pictor_communionem_claudere(q.pc);
    volumen_claudere(v);
    orbem_purgare(&o);
}

interior vacuum
_sectio_expirata (
    CredoContextus* c)
{
     Orbis  o;
   Volumen* v;
    Pictor  p;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    v = volumen_temporarium(piscina, "probatio_pictor_inscriptio_e");
    CREDO_NECESSE(v && pictorem_aedificare(&p, &o, v, NIHIL));
    pictor_communio_inscriptionis_ponere(p.pc, L, CCC);
    CREDO_VERUM(premere_nodum(&p, "salutatio.inscribere"));
    CREDO_VERUM(manus_ludus_scribere(p.m, "fran@example.org"));
    CREDO_VERUM(premere_nodum(&p, "inscriptio.mittere"));
    CREDO_VERUM(donec_titulus(&o, &p, "inscriptio.error",
        "pagina:This sign-in expired. Start again.", M * III));
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&p,
        "inscriptio.titulus"),
        "pagina:Email address");
    pictor_communionem_claudere(p.pc);
    volumen_claudere(v);
    orbem_purgare(&o);
}

/* applicatio necata medio: reaedificata eodem volumine resumit */
interior vacuum
_sectio_resumptio (
    CredoContextus* c)
{
     Orbis  o;
   Volumen* v;
    Pictor  p;
    Pictor  q;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    v = volumen_temporarium(piscina, "probatio_pictor_inscriptio_f");
    CREDO_NECESSE(v && pictorem_aedificare(&p, &o, v, NIHIL));
    CREDO_VERUM(premere_nodum(&p, "salutatio.inscribere"));
    CREDO_VERUM(manus_ludus_scribere(p.m, "fran@example.org"));
    CREDO_VERUM(premere_nodum(&p, "inscriptio.mittere"));
    CREDO_VERUM(donec_gradus(&o, &p, (s32)INSCRIPTIO_EXSPECTAT, M * V));
    CREDO_NECESSE(codex_epistulae(&o).mensura == IX);
    /* necata: claudere inscriptionem pendentem NON abicit */
    pictor_communionem_claudere(p.pc);
    CREDO_VERUM(filum_existit(o.via));
    CREDO_FALSUM(chorda_continet(pendens_legere(v),
        lit("example.org")));
    CREDO_NECESSE(pictorem_aedificare(&q, &o, v, NIHIL));
    CREDO_CHORDA_AEQUALIS_LITERIS(ephemera(&q, "palette"),
        "inscriptio");
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_nodi(&q,
        "inscriptio.titulus"),
        "pagina:Check your email");
    CREDO_VERUM(nexum_premere(&o));
    CREDO_VERUM(donec_titulus(&o, &q, "inscriptio.titulus",
        "pagina:Signed in as Franciscus", M * V));
    pictor_communionem_claudere(q.pc);
    volumen_claudere(v);
    orbem_purgare(&o);
}

/* salutatio, inscriptio (I, II) sine fenestra; PNG si
 * PINACOTHECA_IMAGINES */
interior vacuum
_sectio_imagines (
    CredoContextus* c)
{
                Orbis  o;
              Volumen* v;
               Pictor  p;
      TabulaPixelorum* tabula;
        LudusFenestra* lf;
   constans character* directorium;
            character  via[DXII];

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    v = volumen_temporarium(piscina, "probatio_pictor_inscriptio_g");
    CREDO_NECESSE(v && pictorem_aedificare(&p, &o, v, NIHIL));
    tabula = tabula_pixelorum_creare_nuda(piscina, LATITUDO_TELEPHONI,
        ALTITUDO_TELEPHONI);
    lf = ludus_fenestra_creare(piscina, p.app.d, p.app.figurae,
        ZEPHYRUM,
        pictor_imago_fons, &p.app.montatio.figurae_ctx, tabula);
    CREDO_NECESSE_NON_NIHIL(lf);
    directorium = lectiones_ambitus("PINACOTHECA_IMAGINES");
    ludus_quadrum(lf, ZEPHYRUM);
    ludus_quadrum(lf, C);
    si (directorium && directorium[ZEPHYRUM])
    {
        sprintf(via, "%s/salutatio.png", directorium);
        CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf, via));
    }
    CREDO_VERUM(premere_nodum(&p, "salutatio.inscribere"));
    CREDO_VERUM(manus_ludus_scribere(p.m, "fran@example.org"));
    ludus_quadrum(lf, CC);
    si (directorium && directorium[ZEPHYRUM])
    {
        sprintf(via, "%s/inscriptio_epistula.png", directorium);
        CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf, via));
    }
    CREDO_VERUM(premere_nodum(&p, "inscriptio.mittere"));
    CREDO_VERUM(donec_gradus(&o, &p, (s32)INSCRIPTIO_EXSPECTAT, M * V));
    dispensator_recomponere(p.app.d);
    ludus_quadrum(lf, CCC);
    si (directorium && directorium[ZEPHYRUM])
    {
        sprintf(via, "%s/inscriptio_codex.png", directorium);
        CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf, via));
        imprimere("  imagines: %s\n", directorium);
    }
    pictor_communionem_claudere(p.pc);
    volumen_claudere(v);
    orbem_purgare(&o);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "salutatio", _sectio_salutatio, _parare, _purgare, NIHIL, NIHIL },
    { "per codicem", _sectio_codice, _parare, _purgare, NIHIL, NIHIL },
    { "per nexum", _sectio_nexu, _parare, _purgare, NIHIL, NIHIL },
    { "errores, campus, Cancel, rete", _sectio_errores, _parare,
        _purgare, NIHIL, NIHIL },
    { "expirata", _sectio_expirata, _parare, _purgare, NIHIL, NIHIL },
    { "resumptio applicationis", _sectio_resumptio, _parare, _purgare,
        NIHIL, NIHIL },
    { "imagines", _sectio_imagines, _parare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_inscriptio", SECTIONES);
}
