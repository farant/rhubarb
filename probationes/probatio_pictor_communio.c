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
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina;
hic_manens i32      mutationes;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina     = c->piscina;
    mutationes  = ZEPHYRUM;
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
    o->notans.v.petere     = _notans_petere;
    o->notans.v.abrumpere  = _notans_abrumpere;
    o->notans.v.sui        = &o->notans;
    o->notans.fundus       = o->domesticus;
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

hic_manens constans CredoSectio SECTIONES[] = {
    { "advenit, auctor, index servatus", _sectio_advenit, _parare,
        _purgare, NIHIL, NIHIL },
    { "sine tessera, sine veredario", _sectio_sine_tessera, _parare,
        _purgare, NIHIL, NIHIL },
    { "resumptio", _sectio_resumptio, _parare, _purgare, NIHIL, NIHIL },
    { "revocatus", _sectio_revocatus, _parare, _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_communio", SECTIONES);
}
