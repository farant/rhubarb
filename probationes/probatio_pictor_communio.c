/* probatio_pictor_communio.c - communio intra pictorem (communio-plan-5
 * T5): PictorCommunio super clientem communionis - acta advecta
 * mutationem nuntiant, index apparatuum servatur (attributiones "by
 * Amanda · iPhone"), sine tessera nulla petitio, apparatus revocatus
 * semel 401 accipit et desinit (tessera deleta). Servus in processu,
 * veredarius domesticus, duo volumina. */
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
#include "communio_servus.h"
#include "communio_cliens.h"
#include "communio.h"
#include "veredarius_domesticus.h"
#include "pictor_documentum.h"
#include "pictor_communio.h"
#include "filum.h"
#include "pulsus_forma.h"
#include "testa_cursus.h"
#include "veredarius_pulsus.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina;
hic_manens i32      mutationes;
/* pulsus-plan T9: vocationes pulsum_petere fictae */
hic_manens i32 pulsus_petitiones;
/* pulsus-plan T10: tactus quem pictor testae dedit (tangere) */
hic_manens TestaPulsusTactus tactus_captus;
hic_manens vacuum*           tactus_ctx;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina            = c->piscina;
    mutationes         = ZEPHYRUM;
    pulsus_petitiones  = ZEPHYRUM;
    tactus_captus      = NIHIL;
    tactus_ctx         = NIHIL;
    thema_initiare();
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

/* veredarius notans: petitiones numerat, ceterum domesticus */
nomen structura {
    Veredarius  v;
    Veredarius* fundus;
           i32  petitiones;
           i32  abrumptiones;
} VeredariusNotans;

interior b32
_notans_petere (
                        vacuum* sui,
    constans VeredariusPetitio* p,
                       Piscina* pi,
               VeredariusFinis  finis,
                        vacuum* ctx,
                           i32* signum)
{
    VeredariusNotans* n = (VeredariusNotans*)sui;

    n->petitiones++;
    redde n->fundus->petere(n->fundus->sui, p, pi, finis, ctx, signum);
}

interior vacuum
_notans_abrumpere (
    vacuum* sui,
       i32  signum)
{
    VeredariusNotans* n = (VeredariusNotans*)sui;

    n->abrumptiones++;
    n->fundus->abrumpere(n->fundus->sui, signum);
}

/* servus in processu: Franciscus "Mac", Amanda "iPhone" */
nomen structura {
             Volumen* vs;
               Cives* cives;
      CommunioServus* servus;
             Reactor* reactor;
          Veredarius* domesticus;
    VeredariusNotans  notans;
    VeredariusPulsus  pulsus;     /* pulsus-plan T9 */
           Hospitium* hospitium;
                 i32  portus;
           character  basis[LXIV];
           character  via_a[CXXVIII];
           character  via_b[CXXVIII];
           character  id_a[MONETA_ULID_MENSURA];
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

/* tesseram in filo cudere, digestum apud servum inscribere */
interior b32
apparatum_dare (
                 Orbis* o,
                chorda  civis,
    constans character* titulus,
             character* via,
             character* id)
{
    character moneta[MONETA_ULID_MENSURA];
    character digestum[LXV];

    moneta_ulid(moneta);
    sprintf(via, "/tmp/probatio_pictor_communio_%s", moneta);
    redde communio_membrum_addere(o->servus, lit("familia"), civis)
        && communio_tesseram_scribere(via, digestum)
        && communio_apparatum_addere(o->servus, civis, lit("familia"),
               lit(titulus), lit(digestum), id);
}

interior b32
orbem_parare (
    Orbis* o)
{
       chorda primus;
       chorda amanda;
    character id_b[MONETA_ULID_MENSURA];

    memset(o, ZEPHYRUM, magnitudo(*o));
    o->vs = volumen_temporarium(piscina, "probatio_pictor_communio_s");
    o->cives = o->vs ? cives_aperire(o->vs, NIHIL, piscina) : NIHIL;
    si (!o->cives || !volumen_transactionem_incipere(o->vs))
    {
        redde FALSUM;
    }
    (vacuum)cives_creare(o->cives, lit("franciscus"), lit("Franciscus"),
        &primus, piscina);
    (vacuum)cives_creare(o->cives, lit("amanda"), lit("Amanda"),
        &amanda,
        piscina);
    (vacuum)volumen_transactionem_committere(o->vs);
    o->servus = communio_servum_creare(piscina, o->vs, o->cives,
        NIHIL);
    o->reactor = reactor_creare(piscina);
    o->domesticus = o->reactor ? veredarium_domesticum_creare(piscina,
        o->reactor) : NIHIL;
    veredarium_pulsus_parare(&o->pulsus, o->domesticus, o->reactor);
    o->notans.v.petere     = _notans_petere;
    o->notans.v.abrumpere  = _notans_abrumpere;
    o->notans.v.sui        = &o->notans;
    o->notans.fundus       = &o->pulsus.v;
    redde o->servus && o->domesticus && hospitium_aperire(o)
        && apparatum_dare(o, primus, "Mac", o->via_a, o->id_a)
        && apparatum_dare(o, amanda, "iPhone", o->via_b, id_b);
}

interior vacuum
orbem_purgare (
    Orbis* o)
{
    (vacuum)filum_delere(o->via_a);
    (vacuum)filum_delere(o->via_b);
    communio_servum_destruere(o->servus);
    si (o->hospitium)
    {
        hospitium_destruere(o->hospitium);
    }
    reactor_destruere(o->reactor);
    volumen_claudere(o->vs);
}

interior vacuum
agere (
    Orbis* o,
      i32  ms)
{
    i32 gyri;

    per (gyri = ZEPHYRUM; gyri < ms / X; gyri++)
    {
        (vacuum)reactor_poll(o->reactor, X);
    }
}

interior vacuum
_mutata (
    vacuum* ctx)
{
    (vacuum)ctx;
    mutationes++;
}

interior PictorCommunio*
communionem (
             Orbis* o,
           Volumen* v,
constans character* via)
{
    PictorCommunio* pc;

    pc = pictor_communionem_aperire(piscina, o->reactor, &o->notans.v,
        v,
        via, o->basis, _mutata, NIHIL);
    si (pc)
    {
        pictor_communio_moram_ponere(pc, ZEPHYRUM);
    }
    redde pc;
}

interior s64
ictus (
    PictorDocumentum* doc,
                 i32  x,
                 i32  color)
{
    character t[CXXVIII];

    sprintf(t, "<ictus instrumentum=\"penicillus\" color=\"%lu\""
        " magnitudo=\"3\"><punctum x=\"%lu\" y=\"10\"/></ictus>",
        (insignatus longus)color, (insignatus longus)x);
    redde pictor_documentum_actum(doc, lit(t));
}

/* agere donec A picturas alienas n habet et auctor B notus */
interior b32
donec_advenit (
         Orbis* o,
       Volumen* va,
PictorCommunio* pa,
        chorda  replica_b,
           i32  n)
{
    Xar* l;
    i32  gyri;

    per (gyri = ZEPHYRUM; gyri < D; gyri++)
    {
        (vacuum)reactor_poll(o->reactor, X);
        l = pictor_aliena_recentia(va, piscina);
        si (   l && xar_numerus(l) >= n
            && pictor_communio_auctor(pa, replica_b, piscina).mensura
               > ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior i32
acta_replicae (
    Volumen* v,
     chorda  replica)
{
    Xar* l;
    i32  i;
    i32  n;

    l = volumen_acta_legere(v, ZEPHYRUM, piscina);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (chorda_aequalis(((VolumenActum*)xar_obtinere(l, i))
                ->identitas.replica, replica))
        {
            n++;
        }
    }
    redde n;
}

/* acta propria non derivata (quae communicantur) */
interior i32
acta_communicanda (
    Volumen* v)
{
    Xar* l;
    i32  i;
    i32  n;

    l = volumen_acta_propria_legere(v, ZEPHYRUM, piscina);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (!communio_actum_derivatum((VolumenActum*)xar_obtinere(l, i),
                piscina))
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
_sectio_advenit (
    CredoContextus* c)
{
                  Orbis  o;
                Volumen* va;
                Volumen* vb;
    InternamentumChorda* intern;
         PictorCommunio* pa;
         PictorCommunio* pb;
       PictorDocumentum* p;

    (vacuum)c;
    mutationes = ZEPHYRUM;
    CREDO_NECESSE(orbem_parare(&o));
    intern = internamentum_creare(piscina);
    va = volumen_temporarium(piscina,
        "probatio_pictor_communio_a");
    vb = volumen_temporarium(piscina,
        "probatio_pictor_communio_b");
    CREDO_NECESSE(intern && va && vb);
    pa = communionem(&o, va, o.via_a);
    pb = communionem(&o, vb, o.via_b);
    CREDO_NECESSE(pa && pb);
    CREDO_VERUM(pictor_communio_inscripta(pa));
    pictor_communio_incipere(pa);
    pictor_communio_incipere(pb);
    p = pictor_documentum_creare(piscina, intern, vb, "p", C, LX, II);
    CREDO_NECESSE(p && ictus(p, X, II) > ZEPHYRUM);
    pictor_communio_excitare(pb);
    CREDO_VERUM(donec_advenit(&o, va, pa, volumen_replica(vb), I));
    CREDO_VERUM(mutationes > ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(pictor_communio_auctor(pa,
        volumen_replica(vb), piscina), "Amanda \xC2\xB7 iPhone");
    CREDO_CHORDA_AEQUALIS_LITERIS(pictor_communio_persona(pa, piscina),
        "Franciscus");
    CREDO_AEQUALIS_I32(pictor_communio_auctor(pa, lit("ignota"),
        piscina).mensura, ZEPHYRUM);

    CREDO_GRADUS("index servatus: servo absente post reaperturam");
    pictor_communionem_claudere(pa);
    hospitium_destruere(o.hospitium);
    o.hospitium  = NIHIL;
    pa           = communionem(&o, va, o.via_a);
    CREDO_NECESSE_NON_NIHIL(pa);
    CREDO_CHORDA_AEQUALIS_LITERIS(pictor_communio_auctor(pa,
        volumen_replica(vb), piscina), "Amanda \xC2\xB7 iPhone");
    pictor_communionem_claudere(pa);
    pictor_communionem_claudere(pb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

interior vacuum
_sectio_sine_tessera (
    CredoContextus* c)
{
              Orbis  o;
            Volumen* va;
     PictorCommunio* pa;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    va = volumen_temporarium(piscina, "probatio_pictor_communio_c");
    CREDO_NECESSE_NON_NIHIL(va);
    pa = communionem(&o, va, "/tmp/probatio_pictor_communio_nulla");
    CREDO_NECESSE_NON_NIHIL(pa);
    CREDO_FALSUM(pictor_communio_inscripta(pa));
    pictor_communio_incipere(pa);
    pictor_communio_excitare(pa);
    agere(&o, CCC);
    CREDO_AEQUALIS_I32(o.notans.petitiones, ZEPHYRUM);

    CREDO_GRADUS("sine veredario: NIHIL");
    CREDO_NIHIL(pictor_communionem_aperire(piscina, o.reactor, NIHIL,
        va,
        o.via_a, o.basis, _mutata, NIHIL));
    pictor_communionem_claudere(pa);
    volumen_claudere(va);
    orbem_purgare(&o);
}

/* servus cadit medio, redit: excitare - acta nova semel */
interior vacuum
_sectio_resumptio (
    CredoContextus* c)
{
                  Orbis  o;
                Volumen* va;
                Volumen* vb;
    InternamentumChorda* intern;
         PictorCommunio* pa;
         PictorCommunio* pb;
       PictorDocumentum* p;
                    i32  k;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    intern = internamentum_creare(piscina);
    va = volumen_temporarium(piscina,
        "probatio_pictor_communio_d");
    vb = volumen_temporarium(piscina,
        "probatio_pictor_communio_e");
    CREDO_NECESSE(intern && va && vb);
    pa = communionem(&o, va, o.via_a);
    pb = communionem(&o, vb, o.via_b);
    CREDO_NECESSE(pa && pb);
    pictor_communio_incipere(pa);
    pictor_communio_incipere(pb);
    p = pictor_documentum_creare(piscina, intern, vb, "p", C, LX, II);
    CREDO_NECESSE(p && ictus(p, X, II) > ZEPHYRUM);
    pictor_communio_excitare(pb);
    CREDO_NECESSE(donec_advenit(&o, va, pa, volumen_replica(vb), I));

    CREDO_GRADUS("servus cadit; B pingit");
    hospitium_destruere(o.hospitium);
    o.hospitium = NIHIL;
    per (k = ZEPHYRUM; k < III; k++)
    {
        CREDO_NECESSE(ictus(p, XX + k * X, III) > ZEPHYRUM);
    }
    pictor_communio_excitare(pb);
    agere(&o, CCC);

    CREDO_GRADUS("servus redit: excitare, acta semel");
    CREDO_NECESSE(hospitium_aperire(&o));
    pictor_communio_excitare(pb);
    pictor_communio_excitare(pa);
    per (k = ZEPHYRUM; k < D && acta_replicae(va, volumen_replica(vb))
         < acta_communicanda(vb); k++)
    {
        (vacuum)reactor_poll(o.reactor, X);
        si (k % L == ZEPHYRUM)
        {
            pictor_communio_excitare(pa);
            pictor_communio_excitare(pb);
        }
    }
    agere(&o, CC);
    imprimere("  resumptio: A habet %d, B communicat %d\n",
        (int)acta_replicae(va, volumen_replica(vb)),
        (int)acta_communicanda(vb));
    CREDO_AEQUALIS_I32(acta_replicae(va, volumen_replica(vb)),
        acta_communicanda(vb));
    pictor_communionem_claudere(pa);
    pictor_communionem_claudere(pb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

/* review finalis II: post somnum (iOS) tractio longa in itinere fortasse
 * mortua est - excitare eam abrumpit et statim novam petit */
interior vacuum
_sectio_somnus (
    CredoContextus* c)
{
              Orbis  o;
            Volumen* va;
     PictorCommunio* pa;
                i32  abruptae;
                i32  petitiones;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    va = volumen_temporarium(piscina, "probatio_pictor_communio_g");
    CREDO_NECESSE_NON_NIHIL(va);
    pa = communionem(&o, va, o.via_a);
    CREDO_NECESSE_NON_NIHIL(pa);
    pictor_communio_moram_ponere(pa, XXV);
    pictor_communio_incipere(pa);
    agere(&o, CCC);
    abruptae    = o.notans.abrumptiones;
    petitiones  = o.notans.petitiones;
    pictor_communio_excitare(pa);
    CREDO_VERUM(o.notans.abrumptiones > abruptae);
    CREDO_VERUM(o.notans.petitiones > petitiones);
    agere(&o, CC);
    CREDO_VERUM(pictor_communio_inscripta(pa));
    pictor_communionem_claudere(pa);
    volumen_claudere(va);
    orbem_purgare(&o);
}

/* revocatus: 401 semel, desinit, tessera deleta */
interior vacuum
_sectio_revocatus (
    CredoContextus* c)
{
              Orbis  o;
            Volumen* va;
     PictorCommunio* pa;
                i32  k;
                i32  petitiones;

    (vacuum)c;
    mutationes = ZEPHYRUM;
    CREDO_NECESSE(orbem_parare(&o));
    va = volumen_temporarium(piscina, "probatio_pictor_communio_f");
    CREDO_NECESSE_NON_NIHIL(va);
    pa = communionem(&o, va, o.via_a);
    CREDO_NECESSE_NON_NIHIL(pa);
    pictor_communio_incipere(pa);
    agere(&o, CCC);
    CREDO_NECESSE(communio_apparatum_revocare(o.servus, lit(o.id_a)));
    mutationes = ZEPHYRUM;
    pictor_communio_excitare(pa);
    per (k = ZEPHYRUM; k < CC && pictor_communio_inscripta(pa); k++)
    {
        (vacuum)reactor_poll(o.reactor, X);
    }
    CREDO_FALSUM(pictor_communio_inscripta(pa));
    CREDO_VERUM(mutationes > ZEPHYRUM);
    CREDO_FALSUM(filum_existit(o.via_a));
    petitiones = o.notans.petitiones;
    pictor_communio_excitare(pa);
    agere(&o, M);
    CREDO_AEQUALIS_I32(o.notans.petitiones, petitiones);
    pictor_communionem_claudere(pa);
    volumen_claudere(va);
    orbem_purgare(&o);
}


/* ==================================================
 * pulsus-plan T9: pulsus in pictore
 * ================================================== */

#define TESSERA_FICTA \
    "a0b1c2d3e4f5a6b7c8d9e0f1a2b3c4d5e6f7a8b9c0d1e2f3a4b5c6d7e8f9a0b1"


/* testa ficta: pulsum_petere numerat et tesseram statim dat */
interior b32
_pulsum_petere_fictum (
               vacuum* sui,
    TestaPulsusSignum  signum,
    TestaPulsusTactus  tactus,
               vacuum* ctx)
{
    (vacuum)sui;
    tactus_captus  = tactus;
    tactus_ctx     = ctx;
    pulsus_petitiones++;
    signum(ctx, lit(TESSERA_FICTA), lit(""));
    redde VERUM;
}

interior vacuum
ambitum_fictum (
    TestaAmbitus* a)
{
    memset(a, ZEPHYRUM, magnitudo(*a));
    a->pulsum_petere   = _pulsum_petere_fictum;
    a->pulsus_ambitus  = "sandbox";
}

/* agere donec n eventus missi (aut M ms), deinde paulum ultra */
interior b32
donec_eventus (
    Orbis* o,
      i32  n)
{
    i32 gyri;

    per (gyri = ZEPHYRUM; !veredarius_pulsus_corpus(&o->pulsus, 'P',
             "/communio/pulsus/eventus", n - I) && gyri < C; gyri++)
    {
        agere(o, X);
    }
    agere(o, CC);
    redde veredarius_pulsus_corpus(&o->pulsus, 'P',
        "/communio/pulsus/eventus", n - I) != NIHIL;
}

/* eventus k-tus missus, lectus */
interior b32
eventum_legere (
            Orbis* o,
              i32  k,
    PulsusEventus* ev)
{
     constans character* corpus;
                 chorda  causa;

    corpus = veredarius_pulsus_corpus(&o->pulsus, 'P',
        "/communio/pulsus/eventus", k);
    redde corpus && pulsus_eventum_legere(lit(corpus), piscina, ev,
        &causa);
}

interior chorda
id_picturae (
               Volumen* v,
    constans character* spatium)
{
    redde chorda_concatenare(chorda_concatenare(volumen_replica(v),
        lit(":"), piscina), lit(spatium), piscina);
}

interior vacuum
_sectio_pulsus_eventus (
    CredoContextus* c)
{
                  Orbis  o;
                Volumen* va;
                Volumen* vb;
    InternamentumChorda* intern;
         PictorCommunio* pa;
         PictorCommunio* pb;
       PictorDocumentum* p;
       PictorDocumentum* q;
           TestaAmbitus  a;
          PulsusEventus  ev;
                 chorda  tessera;
                 chorda  ambitus;
                 chorda  thema;
                 chorda  causa;
     constans character* corpus;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    intern = internamentum_creare(piscina);
    va = volumen_temporarium(piscina, "probatio_pictor_communio_h");
    CREDO_NECESSE(intern && va);
    pa = communionem(&o, va, o.via_a);
    CREDO_NECESSE_NON_NIHIL(pa);
    ambitum_fictum(&a);
    pulsus_petitiones = ZEPHYRUM;
    pictor_communio_pulsum_parare(pa, &a);
    CREDO_AEQUALIS_I32(pulsus_petitiones, I);
    /* picturae duae ante initium: fasciculus unus, missa una */
    p = pictor_documentum_creare(piscina, intern, va, "p", C, LX, II);
    q = pictor_documentum_creare(piscina, intern, va, "q", C, LX, II);
    CREDO_NECESSE(   p && q && ictus(p, X, II) > ZEPHYRUM
                  && ictus(q, XX, III) > ZEPHYRUM);
    pictor_communio_incipere(pa);
    CREDO_VERUM(donec_eventus(&o, I));

    CREDO_GRADUS("inscriptio: tessera, sandbox, thema");
    corpus = veredarius_pulsus_corpus(&o.pulsus, 'U',
        "/communio/pulsus/inscriptio", ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(corpus);
    CREDO_VERUM(pulsus_inscriptionem_legere(lit(corpus), piscina,
        &tessera, &ambitus, &thema, &causa));
    CREDO_CHORDA_AEQUALIS_LITERIS(tessera, TESSERA_FICTA);
    CREDO_CHORDA_AEQUALIS_LITERIS(ambitus, "sandbox");
    CREDO_CHORDA_AEQUALIS_LITERIS(thema, "earth.golden.pictor");

    CREDO_GRADUS("eventus unus: res duae <replica>:<spatium>");
    CREDO_NIHIL(veredarius_pulsus_corpus(&o.pulsus, 'P',
        "/communio/pulsus/eventus", I));
    CREDO_NECESSE(eventum_legere(&o, ZEPHYRUM, &ev));
    CREDO_CHORDA_AEQUALIS_LITERIS(ev.clavis, "pictor.picturae");
    CREDO_CHORDA_AEQUALIS_LITERIS(ev.thema, "earth.golden.pictor");
    CREDO_AEQUALIS_I32(ev.mora_s, CCC);
    CREDO_FALSUM(ev.statim);
    CREDO_CHORDA_AEQUALIS_LITERIS(ev.unus,
        "{persona} shared a picture");
    CREDO_CHORDA_AEQUALIS_LITERIS(ev.plures,
        "{persona} shared {numerus} pictures");
    CREDO_NECESSE(xar_numerus(ev.res) == II);
    CREDO_VERUM(chorda_aequalis(*(chorda*)xar_obtinere(ev.res,
        ZEPHYRUM),
        id_picturae(va, "p")));
    CREDO_VERUM(chorda_aequalis(*(chorda*)xar_obtinere(ev.res, I),
        id_picturae(va, "q")));

    CREDO_GRADUS("statim: pictura aperta, deinde nulla");
    CREDO_VERUM(pictor_communio_pulsum_statim(pa, "p"));
    CREDO_VERUM(donec_eventus(&o, II));
    CREDO_NECESSE(eventum_legere(&o, I, &ev));
    CREDO_VERUM(ev.statim);
    CREDO_CHORDA_AEQUALIS_LITERIS(ev.clavis, "pictor.picturae");
    CREDO_NECESSE(xar_numerus(ev.res) == I);
    CREDO_VERUM(chorda_aequalis(*(chorda*)xar_obtinere(ev.res,
        ZEPHYRUM),
        id_picturae(va, "p")));
    CREDO_VERUM(pictor_communio_pulsum_statim(pa, NIHIL));
    CREDO_VERUM(donec_eventus(&o, III));
    CREDO_NECESSE(eventum_legere(&o, II, &ev));
    CREDO_VERUM(ev.statim);
    CREDO_AEQUALIS_I32(xar_numerus(ev.res), ZEPHYRUM);

    CREDO_GRADUS("non inscriptus: pulsum_petere numquam, statim nihil");
    vb = volumen_temporarium(piscina, "probatio_pictor_communio_i");
    CREDO_NECESSE_NON_NIHIL(vb);
    pb = communionem(&o, vb, "/tmp/probatio_pictor_communio_nulla");
    CREDO_NECESSE_NON_NIHIL(pb);
    pulsus_petitiones = ZEPHYRUM;
    pictor_communio_pulsum_parare(pb, &a);
    pictor_communio_incipere(pb);
    CREDO_AEQUALIS_I32(pulsus_petitiones, ZEPHYRUM);
    CREDO_FALSUM(pictor_communio_pulsum_statim(pb, "p"));
    pictor_communionem_claudere(pa);
    pictor_communionem_claudere(pb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

/* pulsus-plan T10: notificatio pressa - corpus ut APNs tradit */
interior vacuum
tangere (
    constans character* id,
                   b32  plures)
{
    character corpus[DXII];

    sprintf(corpus, "{\"aps\":{\"alert\":{\"title\":\"pictor\","
        "\"body\":\"Amanda shared a picture\"}},\"pulsus\":{"
        "\"clavis\":\"pictor.picturae\",\"res\":[%s%s%s],\"a\":"
        "\"civis\"%s}}", id ? "\"" : "", id ? id : "", id ? "\"" : "",
        plures ? ",\"plures\":true" : "");
    tactus_captus(tactus_ctx, lit(corpus));
}

interior chorda
attributum_app (
    PictorApplicatio* app,
  constans character* titulus)
{
    chorda* a;

    a = insula_attributum(app->repo, titulus);
    redde a ? chorda_ex_literis(chorda_ut_cstr(*a, piscina), piscina)
            : lit("");
}

interior chorda
titulus_app (
    PictorApplicatio* app,
  constans character* id)
{
    Componens* n;

    dispensator_recomponere(app->d);
    n = componens_invenire_per_id(dispensator_arbor(app->d), lit(id));
    redde n ? chorda_ex_literis(chorda_ut_cstr(n->titulus, piscina),
        piscina) : lit("");
}

interior vacuum
_sectio_grex_tactus (
    CredoContextus* c)
{
                  Orbis  o;
                Volumen* va;
                Volumen* vb;
    InternamentumChorda* intern;
       PictorApplicatio  app;
         PictorCommunio* pa;
         PictorCommunio* pb;
       PictorDocumentum* p;
           TestaAmbitus  a;
              character  id[CXXVIII];
              character  ignota[CXXVIII];
                    i32  gyri;

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    intern = internamentum_creare(piscina);
    va = volumen_temporarium(piscina, "probatio_pictor_communio_j");
    vb = volumen_temporarium(piscina, "probatio_pictor_communio_k");
    CREDO_NECESSE(intern && va && vb);
    CREDO_NECESSE(pictor_applicatio_aedificare(&app, piscina, intern,
        va,
        CCXV, CDXIX));
    app.montatio.compositio.elenchus_praesto = VERUM;
    pa = communionem(&o, va, o.via_a);
    pb = communionem(&o, vb, o.via_b);
    CREDO_NECESSE(pa && pb);
    pictor_communionem_ligare(pa, &app.montatio, app.d, "Mac");
    ambitum_fictum(&a);
    pictor_communio_pulsum_parare(pa, &a);
    CREDO_NECESSE(tactus_captus != NIHIL);
    pictor_communio_incipere(pa);
    pictor_communio_incipere(pb);
    agere(&o, CC);
    sprintf(id, "%s:p", chorda_ut_cstr(volumen_replica(vb), piscina));
    sprintf(ignota, "%s:nulla", chorda_ut_cstr(volumen_replica(vb),
        piscina));

    CREDO_GRADUS("pictura nondum advecta: rete cum Loading");
    tangere(id, FALSUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(attributum_app(&app, "palette"),
        "communia");
    CREDO_VERUM(app.montatio.communia.onerans);
    CREDO_CHORDA_AEQUALIS_LITERIS(titulus_app(&app, "communia.vacua"),
        "pagina:Loading new pictures\xE2\x80\xA6");

    CREDO_GRADUS("B pingit: post advenerunt spectator in p");
    p = pictor_documentum_creare(piscina, intern, vb, "p", C, LX, II);
    CREDO_NECESSE(p && ictus(p, X, II) > ZEPHYRUM);
    pictor_communio_excitare(pb);
    per (gyri = ZEPHYRUM; gyri < D && !chorda_aequalis_literis(
             attributum_app(&app, "spectata"), id); gyri++)
    {
        (vacuum)reactor_poll(o.reactor, X);
    }
    CREDO_CHORDA_AEQUALIS_LITERIS(attributum_app(&app, "spectata"), id);
    CREDO_CHORDA_AEQUALIS_LITERIS(attributum_app(&app, "palette"),
        "spectator");
    CREDO_FALSUM(app.montatio.communia.onerans);

    CREDO_GRADUS("pictura nota: spectator statim");
    tangere(id, FALSUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(attributum_app(&app, "palette"),
        "spectator");
    CREDO_FALSUM(app.montatio.communia.onerans);

    CREDO_GRADUS("plures: rete sine Loading");
    tangere(id, VERUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(attributum_app(&app, "palette"),
        "communia");
    CREDO_FALSUM(app.montatio.communia.onerans);

    CREDO_GRADUS("ignota: post moram rete, Loading tollitur");
    pictor_communio_tactus_moram_ponere(pa, CC);
    tangere(ignota, FALSUM);
    CREDO_VERUM(app.montatio.communia.onerans);
    agere(&o, CD);
    CREDO_FALSUM(app.montatio.communia.onerans);
    CREDO_CHORDA_AEQUALIS_LITERIS(attributum_app(&app, "palette"),
        "communia");
    pictor_communionem_claudere(pa);
    pictor_communionem_claudere(pb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "advenit, auctor, index servatus", _sectio_advenit, _parare,
        _purgare, NIHIL, NIHIL },
    { "sine tessera, sine veredario", _sectio_sine_tessera, _parare,
        _purgare, NIHIL, NIHIL },
    { "resumptio", _sectio_resumptio, _parare, _purgare, NIHIL, NIHIL },
    { "revocatus", _sectio_revocatus, _parare, _purgare, NIHIL, NIHIL },
    { "somnus", _sectio_somnus, _parare, _purgare, NIHIL, NIHIL },
    { "pulsus eventus", _sectio_pulsus_eventus, _parare, _purgare,
        NIHIL, NIHIL },
    { "grex: tactus", _sectio_grex_tactus, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_communio", SECTIONES);
}
