/* probatio_horreum_vocatio.c - stratum reactoris horrei (horreum-plan
 * T5) contra S3 fictum per rete (horreum_simulacrum_rete.h, 127.0.0.1)
 * in eodem reactore: iter rotundum, iterationes per timers, 'iam
 * nostrum' cum HEAD filia, horologium correctum, abrumpere in mora,
 * finis numquam intra horreum_vocare, ianuae duae (vectura et rete)
 * idem respondent. */
#include "postulata_posix.h"
#include "latina.h"
#include "horreum.h"
#include "horreum_cliens.h"
#include "horreum_vocatio.h"
#include "horreum_simulacrum_rete.h"
#include "reactor.h"
#include "chorda.h"
#include "piscina.h"
#include "credo.h"
#include <string.h>
#include <time.h>

#define SECRETUM   "SECRETUM-SIMULACRI-0123456789"
#define CLAVIS_ID  "AKIDSIMULACRI"
#define ETAG_NULLA "00000000000000000000000000000000"

nomen structura {
                       i32  vocata;
                       i32  vocata_intra;    /* dum horreum_vocare */
                       b32  intra;
    HorreumVocatioResultus  resultus;
            HorreumVocatio* vocatio;
} Testis;

nomen structura {
                   Reactor* reactor;
                Simulacrum* simulacrum;
            SimulacrumRete* rete;
                   Horreum* horreum;
    HorreumVocatioOptiones  optiones;
                    chorda  situla;
} Mundus;

interior chorda
_l (
    constans character* l,
               Piscina* p)
{
    redde chorda_ex_literis(l, p);
}

interior vacuum
_finis (
                             vacuum* datum,
                     HorreumVocatio* v,
    constans HorreumVocatioResultus* r)
{
    Testis* t = (Testis*)datum;

    (vacuum)v;
    t->vocata++;
    si (t->intra)
    {
        t->vocata_intra++;
    }
    t->resultus = *r;
}

interior HorreumVocatio*
_vocare (
                           Mundus* m,
                          Horreum* h,
             constans HorreumOpus* opus,
                           Testis* t,
                          Piscina* p)
{
    memset(t, ZEPHYRUM, magnitudo(*t));
    t->intra    = VERUM;
    t->vocatio  = horreum_vocare(m->reactor, h, opus, _finis, t,
        &m->optiones, p);
    t->intra    = FALSUM;
    redde t->vocatio;
}

/* reactor pulsatur donec finis (limes ~X s) */
interior b32
_pulsare (
    Mundus* m,
    Testis* t)
{
    i32 i;

    per (i = ZEPHYRUM; i < M && t->vocata == ZEPHYRUM; i++)
    {
        (vacuum)reactor_poll(m->reactor, X);
    }
    redde t->vocata > ZEPHYRUM;
}

interior Horreum*
_horreum_ad (
    constans character* terminus,
               Piscina* p)
{
    HorreumConfiguratio c;
           HorreumError e;

    memset(&c, ZEPHYRUM, magnitudo(c));
    c.provisor   = "generic";
    c.terminus   = _l(terminus, p);
    c.regio      = _l("us-east-1", p);
    c.situla     = _l(SIMULACRUM_SITULA, p);
    c.clavis_id  = _l(CLAVIS_ID, p);
    c.secretum   = _l(SECRETUM, p);
    redde horreum_creare(&c, &e, p);
}

interior chorda
_terminus (
        i32  portus,
    Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, LXIV);

    chorda_aedificator_appendere_literis(a, "http://127.0.0.1:");
    chorda_aedificator_appendere_i32(a, portus);
    redde chorda_aedificator_finire(a);
}

interior b32
_mundus (
     Mundus* m,
    Piscina* p)
{
    memset(m, ZEPHYRUM, magnitudo(*m));
    m->reactor     = reactor_creare(p);
    m->simulacrum  = simulacrum_creare(_l(CLAVIS_ID, p), _l(SECRETUM,
        p),
        _l("us-east-1", p), p);
    m->rete = simulacrum_rete_aperire(m->simulacrum, m->reactor, p);
    si (!m->reactor || !m->rete)
    {
        redde FALSUM;
    }
    m->horreum = _horreum_ad(chorda_ut_cstr(_terminus(m->rete->portus,
        p), p), p);
    m->optiones.mora_basis_ms  = I;
    m->optiones.semen          = VII;
    m->situla                  = _l(SIMULACRUM_SITULA, p);
    redde m->horreum != NIHIL;
}

interior vacuum
_mundum_claudere (
    Mundus* m)
{
    simulacrum_rete_claudere(m->rete);
    simulacrum_claudere(m->simulacrum);
    horreum_claudere(m->horreum);
    reactor_destruere(m->reactor);
}

interior SimulacrumVitium
_vitium (
    SimulacrumVitiumGenus  genus,
             HttpMethodus  methodus,
       constans character* clavis,
                      i32  status,
       constans character* codex,
                      i32  numerus)
{
    SimulacrumVitium v;

    v.genus       = genus;
    v.methodus    = methodus;
    v.clavis      = clavis;
    v.parametrum  = NIHIL;
    v.status      = status;
    v.codex       = codex;
    v.secundae    = ZEPHYRUM;
    v.numerus     = numerus;
    redde v;
}

/* corpus MXXIV octetorum, forma nota */
interior chorda
_corpus_mille (
    Piscina* p)
{
     i8* d = (i8*)piscina_allocare(p, (memoriae_index)MXXIV);
    i32  i;

    per (i = ZEPHYRUM; i < MXXIV; i++)
    {
        d[i] = (i8)('a' + (i % XXVI));
    }
    redde chorda_ex_buffer(d, MXXIV);
}

/* ===== iter ===== */
interior vacuum
_sectio_iter (
    CredoContextus* c)
{
        Piscina* p = c->piscina;
         Mundus  m;
         Testis  t;
    HorreumOpus  o;
   HorreumError  e;
         chorda  clavis = _l("via/obiectum.bin", p);
         chorda  corpus = _corpus_mille(p);
         chorda  etag;
            i32  i;

    CREDO_NECESSE(_mundus(&m, p));
    CREDO_VERUM(horreum_opus_ponere(m.horreum, m.situla, clavis, corpus,
        NIHIL, &o, &e, p));
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_AEQUALIS_I32(t.vocata, ZEPHYRUM);
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_VERUM(t.resultus.successus);
    CREDO_AEQUALIS_I32(t.resultus.conatus, I);
    CREDO_AEQUALIS_I32(t.resultus.obiectum.etag.mensura, XXXII);
    etag = t.resultus.obiectum.etag;
    per (i = ZEPHYRUM; i < V; i++)
    {
        (vacuum)reactor_poll(m.reactor, ZEPHYRUM);
    }
    CREDO_AEQUALIS_I32(t.vocata, I);
    CREDO_VERUM(horreum_vocatio_finita(t.vocatio));
    CREDO_VERUM(chorda_aequalis(simulacrum_obiectum_corpus(m.simulacrum,
        clavis), corpus));
    CREDO_AEQUALIS_I32(simulacrum_corpus_maximum(m.simulacrum), MXXIV);

    CREDO_VERUM(horreum_opus_capere(m.horreum, m.situla, clavis, NIHIL,
        &o, &e, p));
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_VERUM(t.resultus.successus);
    CREDO_NECESSE_NON_NIHIL(t.resultus.responsum);
    CREDO_VERUM(chorda_aequalis(t.resultus.responsum->corpus, corpus));
    CREDO_VERUM(chorda_aequalis(t.resultus.obiectum.etag, etag));
    CREDO_AEQUALIS_I32(simulacrum_petitiones(m.simulacrum), II);
    _mundum_claudere(&m);
}

/* ===== transitorium: 503 bis, deinde bene ===== */
interior vacuum
_sectio_transitorium (
    CredoContextus* c)
{
        Piscina* p = c->piscina;
         Mundus  m;
         Testis  t;
    HorreumOpus  o;
   HorreumError  e;

    CREDO_NECESSE(_mundus(&m, p));
    simulacrum_vitium_addere(m.simulacrum, _vitium(
        SIMULACRUM_VITIUM_STATUS, HTTP_PUT, NIHIL, DIII,
        "ServiceUnavailable", II));
    CREDO_VERUM(horreum_opus_ponere(m.horreum, m.situla, _l("t.txt", p),
        _l("salve", p), NIHIL, &o, &e, p));
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_VERUM(t.resultus.successus);
    CREDO_AEQUALIS_I32(t.resultus.conatus, III);
    CREDO_AEQUALIS_I32(simulacrum_petitiones(m.simulacrum), III);
    _mundum_claudere(&m);
}

/* ===== iam_nostrum: PUT condicionalis perficitur, responsum perit ===== */
interior vacuum
_sectio_iam_nostrum (
    CredoContextus* c)
{
           Piscina* p = c->piscina;
            Mundus  m;
            Testis  t;
       HorreumOpus  o;
      HorreumError  e;
    HorreumPonendi  ponendi;

    CREDO_NECESSE(_mundus(&m, p));
    simulacrum_vitium_addere(m.simulacrum, _vitium(
        SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE, HTTP_PUT, NIHIL, ZEPHYRUM,
        NIHIL, I));
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_abest      = VERUM;
    ponendi.periculum_accipere  = VERUM;   /* generic: nihil probatum */
    CREDO_VERUM(horreum_opus_ponere(m.horreum, m.situla, _l("n.txt", p),
        _l("nostrum", p), &ponendi, &o, &e, p));
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_VERUM(t.resultus.successus);
    CREDO_VERUM(t.resultus.iam_nostrum);
    CREDO_VERUM(t.resultus.obiectum.iam_nostrum);
    CREDO_AEQUALIS_I32(t.resultus.conatus, II);
    CREDO_AEQUALIS_I32(t.resultus.obiectum.etag.mensura, XXXII);
    /* PUT (abruptum), PUT (412), HEAD */
    CREDO_AEQUALIS_I32(simulacrum_petitiones(m.simulacrum), III);
    CREDO_AEQUALIS_I32(t.vocata, I);
    _mundum_claudere(&m);
}

/* ===== horologium: servus MCC s ante nos ===== */
interior vacuum
_sectio_horologium (
    CredoContextus* c)
{
        Piscina* p = c->piscina;
         Mundus  m;
         Testis  t;
    HorreumOpus  o;
   HorreumError  e;

    CREDO_NECESSE(_mundus(&m, p));
    simulacrum_horam_ponere(m.simulacrum, (MomentumSec)time(NIHIL)
        + (MoraSec)MCC);
    CREDO_VERUM(horreum_opus_ponere(m.horreum, m.situla, _l("h.txt", p),
        _l("hora", p), NIHIL, &o, &e, p));
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_VERUM(t.resultus.successus);
    CREDO_AEQUALIS_I32(t.resultus.conatus, II);
    CREDO_MAIOR_S64(t.resultus.deviatio_s, (s64)MCXC);
    CREDO_MINOR_S64(t.resultus.deviatio_s, (s64)MCCX);
    _mundum_claudere(&m);
}

/* ===== abrumpere in mora prima ===== */
interior vacuum
_sectio_abrumpere (
    CredoContextus* c)
{
           Piscina* p = c->piscina;
            Mundus  m;
            Testis  t;
       HorreumOpus  o;
      HorreumError  e;
            chorda  clavis = _l("a.txt", p);
               i32  timer_ante;
               i32  i;

    CREDO_NECESSE(_mundus(&m, p));
    m.optiones.mora_basis_ms = X * M;          /* mora numquam expirat */
    simulacrum_obiectum_ponere(m.simulacrum, clavis, _l("salve", p));
    simulacrum_vitium_addere(m.simulacrum, _vitium(
        SIMULACRUM_VITIUM_STATUS, HTTP_GET, NIHIL, DIII,
        "ServiceUnavailable", I));
    CREDO_VERUM(horreum_opus_capere(m.horreum, m.situla, clavis, NIHIL,
        &o, &e, p));
    timer_ante = reactor_numerus_timer(m.reactor);
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    per (i = ZEPHYRUM; i < M && horreum_vocatio_conatus(t.vocatio)
        == ZEPHYRUM; i++)
    {
        (vacuum)reactor_poll(m.reactor, X);
    }
    CREDO_AEQUALIS_I32(horreum_vocatio_conatus(t.vocatio), I);
    CREDO_AEQUALIS_I32(t.vocata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(m.reactor), timer_ante
        + I);

    horreum_vocationem_abrumpere(t.vocatio);
    CREDO_AEQUALIS_I32(t.vocata, I);
    CREDO_VERUM(t.resultus.abrupta);
    CREDO_FALSUM(t.resultus.successus);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(m.reactor), timer_ante);
    horreum_vocationem_abrumpere(t.vocatio);
    per (i = ZEPHYRUM; i < X; i++)
    {
        (vacuum)reactor_poll(m.reactor, X);
    }
    CREDO_AEQUALIS_I32(t.vocata, I);
    CREDO_AEQUALIS_I32(simulacrum_petitiones(m.simulacrum), I);
    _mundum_claudere(&m);
}

/* ===== finis_non_intra: defectus immediatus et portus clausus ===== */
interior vacuum
_sectio_finis_non_intra (
    CredoContextus* c)
{
           Piscina* p = c->piscina;
            Mundus  m;
            Testis  t;
       HorreumOpus  o;
      HorreumError  e;
    SimulacrumRete* clausum;
           Horreum* h_clausum;

    CREDO_NECESSE(_mundus(&m, p));
    CREDO_VERUM(horreum_opus_ponere(m.horreum, m.situla, _l("f.txt", p),
        _l("finis", p), NIHIL, &o, &e, p));

    /* signatio sine horreo deficit intra horreum_vocare */
    CREDO_NON_NIHIL(_vocare(&m, NIHIL, &o, &t, p));
    CREDO_AEQUALIS_I32(t.vocata, ZEPHYRUM);
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_AEQUALIS_I32(t.vocata_intra, ZEPHYRUM);
    CREDO_FALSUM(t.resultus.successus);
    CREDO_AEQUALIS_I32((i32)t.resultus.error.genus, (i32)HORREUM_MALUM);
    CREDO_AEQUALIS_I32(t.resultus.conatus, ZEPHYRUM);

    /* portus clausus: conexio recusatur, iteratur, deinde finis */
    clausum = simulacrum_rete_aperire(m.simulacrum, m.reactor, p);
    CREDO_NECESSE_NON_NIHIL(clausum);
    h_clausum = _horreum_ad(chorda_ut_cstr(_terminus(clausum->portus,
        p),
        p), p);
    simulacrum_rete_claudere(clausum);
    CREDO_NECESSE_NON_NIHIL(h_clausum);
    CREDO_VERUM(horreum_opus_ponere(h_clausum, m.situla, _l("f.txt", p),
        _l("finis", p), NIHIL, &o, &e, p));
    CREDO_NON_NIHIL(_vocare(&m, h_clausum, &o, &t, p));
    CREDO_AEQUALIS_I32(t.vocata, ZEPHYRUM);
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_AEQUALIS_I32(t.vocata_intra, ZEPHYRUM);
    CREDO_FALSUM(t.resultus.successus);
    CREDO_AEQUALIS_I32((i32)t.resultus.error.genus,
        (i32)HORREUM_TRANSITORIUM);
    CREDO_AEQUALIS_I32(t.resultus.conatus, III);
    CREDO_AEQUALIS_I32(simulacrum_petitiones(m.simulacrum), ZEPHYRUM);
    horreum_claudere(h_clausum);
    _mundum_claudere(&m);
}

/* ===== duae_ianuae: vectura et rete idem respondent ===== */
interior vacuum
_sectio_duae_ianuae (
    CredoContextus* c)
{
                    Piscina* p = c->piscina;
                     Mundus  m;
                     Testis  t;
                HorreumOpus  o;
               HorreumError  e;
            HorreumCapiendi  ca;
            HorreumObjectum  ob;
    HorreumClientisOptiones  co;
                    Horreum* h_vectura;
                     chorda  clavis = _l("par.txt", p);
                     chorda  corpus;
                        i32  visa;

    CREDO_NECESSE(_mundus(&m, p));
    simulacrum_obiectum_ponere(m.simulacrum, clavis, _l("salve", p));
    h_vectura = _horreum_ad("http://simulacrum.invalid", p);
    CREDO_NECESSE_NON_NIHIL(h_vectura);
    memset(&co, ZEPHYRUM, magnitudo(co));
    co.vectura = simulacrum_vectura(m.simulacrum);
    horreum_cliens_parare(h_vectura, &co, p);
    CREDO_VERUM(horreum_capere(h_vectura, m.situla, clavis, NIHIL,
        &corpus, &ob, &e, p));

    CREDO_VERUM(horreum_opus_capere(m.horreum, m.situla, clavis, NIHIL,
        &o, &e, p));
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_VERUM(t.resultus.successus);
    CREDO_NECESSE_NON_NIHIL(t.resultus.responsum);
    CREDO_VERUM(chorda_aequalis(t.resultus.responsum->corpus, corpus));
    CREDO_VERUM(chorda_aequalis(t.resultus.obiectum.etag, ob.etag));
    CREDO_AEQUALIS_I64(t.resultus.obiectum.mensura, ob.mensura);

    /* If-Match per rete: visum a servo */
    visa = simulacrum_si_etag_visa(m.simulacrum);
    memset(&ca, ZEPHYRUM, magnitudo(ca));
    ca.si_etag = ob.etag;
    CREDO_VERUM(horreum_opus_capere(m.horreum, m.situla, clavis, &ca,
        &o,
        &e, p));
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_VERUM(t.resultus.successus);
    CREDO_AEQUALIS_I32(simulacrum_si_etag_visa(m.simulacrum), visa + I);

    /* ETag mutatum: 412 per rete, non iteratur */
    simulacrum_etag_ponere(m.simulacrum, clavis, ETAG_NULLA);
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_FALSUM(t.resultus.successus);
    CREDO_AEQUALIS_I32((i32)t.resultus.error.genus,
        (i32)HORREUM_CONDITIO);
    CREDO_AEQUALIS_I32(t.resultus.error.status, CDXII);
    CREDO_AEQUALIS_I32(t.resultus.conatus, I);
    horreum_claudere(h_vectura);
    _mundum_claudere(&m);
}

/* ===== observator: omnis conatus, filia HEAD comprehensa ===== */
nomen structura {
                 i32 numerus;
    HorreumOpusGenus genera[VIII];
                 i32 status[VIII];        /* 0 = transportus */
                 i32 conatus[VIII];
} Observata;

interior vacuum
_observare (
                        vacuum* datum,
    constans HorreumObservatio* o)
{
    Observata* ob = (Observata*)datum;

    si (ob->numerus < VIII)
    {
        ob->genera[ob->numerus]   = o->opus->genus;
        ob->status[ob->numerus]   = o->responsum ? o->responsum->status
                                                 : ZEPHYRUM;
        ob->conatus[ob->numerus]  = o->conatus;
    }
    ob->numerus++;
}

interior vacuum
_sectio_observator (
    CredoContextus* c)
{
           Piscina* p = c->piscina;
            Mundus  m;
            Testis  t;
         Observata  ob;
       HorreumOpus  o;
      HorreumError  e;
    HorreumPonendi  ponendi;

    CREDO_NECESSE(_mundus(&m, p));
    memset(&ob, ZEPHYRUM, magnitudo(ob));
    m.optiones.observator          = _observare;
    m.optiones.observatoris_datum  = &ob;
    simulacrum_vitium_addere(m.simulacrum, _vitium(
        SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE, HTTP_PUT, NIHIL, ZEPHYRUM,
        NIHIL, I));
    memset(&ponendi, ZEPHYRUM, magnitudo(ponendi));
    ponendi.solum_si_abest      = VERUM;
    ponendi.periculum_accipere  = VERUM;   /* generic: nihil probatum */
    CREDO_VERUM(horreum_opus_ponere(m.horreum, m.situla, _l("v.txt", p),
        _l("visum", p), &ponendi, &o, &e, p));
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    CREDO_VERUM(t.resultus.iam_nostrum);
    CREDO_AEQUALIS_I32(ob.numerus, III);
    CREDO_AEQUALIS_I32((i32)ob.genera[ZEPHYRUM],
        (i32)HORREUM_OPUS_PONERE);
    CREDO_AEQUALIS_I32(ob.status[ZEPHYRUM], ZEPHYRUM);
    CREDO_AEQUALIS_I32(ob.conatus[ZEPHYRUM], I);
    CREDO_AEQUALIS_I32(ob.status[I], CDXII);
    CREDO_AEQUALIS_I32(ob.conatus[I], II);
    CREDO_AEQUALIS_I32((i32)ob.genera[II],
        (i32)HORREUM_OPUS_INSPICERE);
    CREDO_AEQUALIS_I32(ob.status[II], CC);
    _mundum_claudere(&m);
}

/* ===== observator_abrumpens: abrumpere INTRA observatorem - post
 * finem (abrupta) nulla petitio ulterior, nullum timer (recensio
 * I5) ===== */
interior vacuum
_observare_abrumpens (
                        vacuum* datum,
    constans HorreumObservatio* o)
{
    Testis* t = (Testis*)datum;

    (vacuum)o;
    horreum_vocationem_abrumpere(t->vocatio);
}

interior vacuum
_sectio_observator_abrumpens (
    CredoContextus* c)
{
        Piscina* p = c->piscina;
         Mundus  m;
         Testis  t;
    HorreumOpus  o;
   HorreumError  e;
            i32  timer_ante;
            i32  i;

    CREDO_NECESSE(_mundus(&m, p));
    m.optiones.observator          = _observare_abrumpens;
    m.optiones.observatoris_datum  = &t;
    simulacrum_vitium_addere(m.simulacrum, _vitium(
        SIMULACRUM_VITIUM_STATUS, HTTP_PUT, NIHIL, DIII,
        "ServiceUnavailable", II));
    CREDO_VERUM(horreum_opus_ponere(m.horreum, m.situla, _l("ab.txt",
        p),
        _l("salve", p), NIHIL, &o, &e, p));
    timer_ante = reactor_numerus_timer(m.reactor);
    CREDO_NON_NIHIL(_vocare(&m, m.horreum, &o, &t, p));
    CREDO_VERUM(_pulsare(&m, &t));
    per (i = ZEPHYRUM; i < L; i++)
    {
        (vacuum)reactor_poll(m.reactor, X);
    }
    CREDO_AEQUALIS_I32(t.vocata, I);
    CREDO_VERUM(t.resultus.abrupta);
    CREDO_FALSUM(t.resultus.successus);
    CREDO_AEQUALIS_I32(simulacrum_petitiones(m.simulacrum), I);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(m.reactor), timer_ante);
    _mundum_claudere(&m);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iter", _sectio_iter, NIHIL, NIHIL, NIHIL, NIHIL },
    { "transitorium", _sectio_transitorium, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "iam_nostrum", _sectio_iam_nostrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "horologium", _sectio_horologium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abrumpere", _sectio_abrumpere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "finis_non_intra", _sectio_finis_non_intra, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "duae_ianuae", _sectio_duae_ianuae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "observator", _sectio_observator, NIHIL, NIHIL, NIHIL, NIHIL },
    { "observator_abrumpens", _sectio_observator_abrumpens, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("horreum_vocatio", SECTIONES);
}
