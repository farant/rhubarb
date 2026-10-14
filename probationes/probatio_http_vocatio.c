/* probatio_http_vocatio.c - vocatio HTTP non-obstruens contra servum
 * scriptum in eodem reactore (http-vocatio-spec II.4, III; plan Review
 * focus I-V). Omnis finis semel; post finem nihil vocationis in
 * reactore (servo clauso: fossae et timers nulli). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "reactor.h"
#include "http.h"
#include "http_vocatio.h"
#include "servus_scriptus.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define RESPONSUM_TRUNCATUM \
    "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n" \
    "Transfer-Encoding: chunked\r\n\r\n" \
    "5\r\nSalve\r\n6\r\n munde\r\n0\r\n\r\n"

#define RESPONSUM_LONGITUDINIS \
    "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n" \
    "Connection: keep-alive\r\n\r\nSalve"

nomen structura {
                  i32  finis;
                  i32  rivi;
    ChordaAedificator* corpus;
          HttpVocatio* abrumpenda;    /* in rivo primo abrumpitur */
} Observatio;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_observationem_initiare (
    Observatio* o,
       Piscina* p)
{
    memset(o, ZEPHYRUM, magnitudo(*o));
    o->corpus = chorda_aedificator_creare(p, LXIV);
}

interior vacuum
_rivus (
         vacuum* datum,
    constans i8* octeti,
            i32  n)
{
    Observatio* o = (Observatio*)datum;
           i32  k;

    o->rivi++;
    per (k = ZEPHYRUM; k < n; k++)
    {
        chorda_aedificator_appendere_character(o->corpus,
            (character)octeti[k]);
    }
    si (o->abrumpenda)
    {
        HttpVocatio* v = o->abrumpenda;

        o->abrumpenda = NIHIL;
        http_vocationem_abrumpere(v);
    }
}

interior vacuum
_finis (
         vacuum* datum,
    HttpVocatio* vocatio)
{
    (vacuum)vocatio;
    ((Observatio*)datum)->finis++;
}

interior b32
_currere (
        Reactor* r,
    HttpVocatio* v,
            s64  tectum_ms)
{
    s64 initium = _ms_nunc();

    dum (!http_vocatio_finita(v) && _ms_nunc() - initium < tectum_ms)
    {
        (vacuum)reactor_poll(r, XX);
    }
    redde http_vocatio_finita(v);
}

interior HttpPetitio*
_petitio (
    constans character* schema,
    constans character* hospes,
                   i32  portus,
               Piscina* p)
{
    character url[CCLVI];

    sprintf(url, "%s://%s:%u/v1/proba", schema, hospes,
        (insignatus integer)portus);
    redde http_petitio_creare(p, HTTP_GET, url);
}

/* responsum cum omni divisione in duo gradus: idem exitus; servus
 * connexionem tenet - finis per lectorem, non per clausuram */
interior vacuum
_sectio_divisiones (
    CredoContextus* c)
{
    constans character* resp  = RESPONSUM_TRUNCATUM;
                   i32  n     = (i32)strlen(resp);
                   i32  k;
                   i32  mala = ZEPHYRUM;

    per (k = I; k < n; k++)
    {
             Reactor* r = reactor_creare(c->piscina);
        ServusGradus  g[II];
      ServusScriptus* s;
          Observatio  o;
         HttpVocatio* v;
        HttpResultus  res;

        g[ZEPHYRUM].octeti = resp;
        g[ZEPHYRUM].mensura = k;
        g[ZEPHYRUM].mora_ms = ZEPHYRUM;
        g[ZEPHYRUM].claudere = FALSUM;
        g[I].octeti = resp + k;
        g[I].mensura = n - k;
        g[I].mora_ms = I;
        g[I].claudere = FALSUM;
        s = servum_scriptum_creare(r, g, II, c->piscina);
        CREDO_NECESSE_NON_NIHIL(s);
        _observationem_initiare(&o, c->piscina);
        v = http_vocare(r, _petitio("http", "127.0.0.1", s->portus,
            c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
        CREDO_NECESSE_NON_NIHIL(v);
        CREDO_NECESSE(o.finis == ZEPHYRUM);  /* numquam intra vocare */
        (vacuum)_currere(r, v, II * M);
        res = http_vocatio_resultus(v);
        si (   o.finis                != I
            || http_vocatio_status(v) != HTTP_VOCATIO_FINITA
            || !res.successus || res.responsum->status != CC
            || !chorda_aequalis_literis(
                chorda_aedificator_finire(o.corpus), "Salve munde"))
        {
            mala++;
        }
        servum_scriptum_claudere(s);
        si (   reactor_numerus_fd(r)    != ZEPHYRUM
            || reactor_numerus_timer(r) != ZEPHYRUM)
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
}

/* Review focus I: Content-Length, servus non claudit -> finis statim */
interior vacuum
_sectio_manet_apertum (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
     HttpVocatio* v;
             s64  initium;

    g[ZEPHYRUM].octeti    = RESPONSUM_LONGITUDINIS;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    initium = _ms_nunc();
    v = http_vocare(r, _petitio("http", "127.0.0.1", s->portus,
        c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
    CREDO_NECESSE(_currere(r, v, II * M));
    CREDO_VERUM(_ms_nunc() - initium < D);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_FINITA);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(o.corpus),
        "Salve");
    servum_scriptum_claudere(s);
}

/* Review focus III: petitione accepta servus claudit sine verbo */
interior vacuum
_sectio_clausum_statim (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
     HttpVocatio* v;
    HttpResultus  res;

    g[ZEPHYRUM].octeti    = NIHIL;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    v = http_vocare(r, _petitio("http", "127.0.0.1", s->portus,
        c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
    CREDO_NECESSE(_currere(r, v, II * M));
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32(o.finis, I);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_IRRITA);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_IO);
    CREDO_VERUM(res.error_descriptio.mensura > ZEPHYRUM);
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
}

/* portus sine servo: CONNEXIO nominata */
interior vacuum
_sectio_recusata (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
  ServusScriptus* s = servum_scriptum_creare(r, NIHIL, ZEPHYRUM,
      c->piscina);
      Observatio  o;
     HttpVocatio* v;
    HttpResultus  res;
             i32  portus;

    CREDO_NECESSE_NON_NIHIL(s);
    portus = s->portus;
    servum_scriptum_claudere(s);
    _observationem_initiare(&o, c->piscina);
    v = http_vocare(r, _petitio("http", "127.0.0.1", portus,
        c->piscina),
        _rivus, &o, _finis, &o, NIHIL, c->piscina);
    CREDO_NECESSE(_currere(r, v, II * M));
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_CONNEXIO);
    CREDO_VERUM(res.error_descriptio.mensura > ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

/* Review focus V: localhost (::1 fortasse primum) contra servum in
 * 127.0.0.1 solum -> addressus proximus */
interior vacuum
_sectio_hospes_localis (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
     HttpVocatio* v;

    g[ZEPHYRUM].octeti    = RESPONSUM_LONGITUDINIS;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    v = http_vocare(r, _petitio("http", "localhost", s->portus,
        c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
    CREDO_NECESSE(_currere(r, v, III * M));
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_FINITA);
    servum_scriptum_claudere(s);
}

/* capita et pars, deinde silentium: tempus otii, pars in rivo */
interior vacuum
_sectio_otium (
    CredoContextus* c)
{
                Reactor* r = reactor_creare(c->piscina);
           ServusGradus  g[I];
         ServusScriptus* s;
             Observatio  o;
            HttpVocatio* v;
    HttpVocatioOptiones  opt;
           HttpResultus  res;

    g[ZEPHYRUM].octeti    = "HTTP/1.1 200 OK\r\nTransfer-Encoding: "
        "chunked\r\n\r\n5\r\nSalve\r\n";
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    opt.mora_connexionis_ms  = ZEPHYRUM;
    opt.mora_otii_ms         = CC;
    v = http_vocare(r, _petitio("http", "127.0.0.1", s->portus,
        c->piscina), _rivus, &o, _finis, &o, &opt, c->piscina);
    CREDO_NECESSE(_currere(r, v, II * M));
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_TIMEOUT);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(o.corpus),
        "Salve");
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

/* https contra servum tacitum: tempus connexionis in PACTIO */
interior vacuum
_sectio_pactio_tacita (
    CredoContextus* c)
{
                Reactor* r = reactor_creare(c->piscina);
         ServusScriptus* s = servum_scriptum_creare(r, NIHIL, ZEPHYRUM,
             c->piscina);
             Observatio  o;
            HttpVocatio* v;
    HttpVocatioOptiones  opt;
           HttpResultus  res;

    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    opt.mora_connexionis_ms  = CC;
    opt.mora_otii_ms         = ZEPHYRUM;
    v = http_vocare(r, _petitio("https", "127.0.0.1", s->portus,
        c->piscina), _rivus, &o, _finis, &o, &opt, c->piscina);
    CREDO_NECESSE(_currere(r, v, II * M));
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32((i32)res.error, (i32)HTTP_ERROR_TIMEOUT);
    CREDO_CHORDA_CONTINET(res.error_descriptio, chorda_ex_literis(
        "connexionis", c->piscina));
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
}

/* abrumpere in NOMINA, PACTIO, LEGENS: finis semel ANTE reditum */
interior vacuum
_sectio_abrumpere (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
     HttpVocatio* v;
             s64  initium;

    /* NOMINA */
    _observationem_initiare(&o, c->piscina);
    v = http_vocare(r, _petitio("http", "localhost", LXXX, c->piscina),
        _rivus, &o, _finis, &o, NIHIL, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    http_vocationem_abrumpere(v);
    CREDO_AEQUALIS_I32(o.finis, I);
    http_vocationem_abrumpere(v);
    CREDO_AEQUALIS_I32(o.finis, I);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_ABRUPTA);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);

    /* PACTIO: https contra servum tacitum */
    s = servum_scriptum_creare(r, NIHIL, ZEPHYRUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    v = http_vocare(r, _petitio("https", "127.0.0.1", s->portus,
        c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
    initium = _ms_nunc();
    dum (   http_vocatio_status(v) != HTTP_VOCATIO_PACTIO
         && _ms_nunc() - initium < II * M)
    {
        (vacuum)reactor_poll(r, XX);
    }
    CREDO_NECESSE(http_vocatio_status(v) == HTTP_VOCATIO_PACTIO);
    http_vocationem_abrumpere(v);
    CREDO_AEQUALIS_I32(o.finis, I);
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);

    /* LEGENS: capita, deinde silentium */
    g[ZEPHYRUM].octeti    = "HTTP/1.1 200 OK\r\nTransfer-Encoding: "
        "chunked\r\n\r\n";
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    v = http_vocare(r, _petitio("http", "127.0.0.1", s->portus,
        c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
    initium = _ms_nunc();
    dum (s->index == ZEPHYRUM && _ms_nunc() - initium < II * M)
    {
        (vacuum)reactor_poll(r, XX);
    }
    (vacuum)reactor_poll(r, XX);
    CREDO_NECESSE(http_vocatio_status(v) == HTTP_VOCATIO_LEGENS);
    http_vocationem_abrumpere(v);
    CREDO_AEQUALIS_I32(o.finis, I);
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

/* Review focus II: rivus in prima parte abrumpit; capsa eadem plura
 * tenet - nullus rivus post finem */
interior vacuum
_sectio_abrumpere_intra_rivum (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
     HttpVocatio* v;

    g[ZEPHYRUM].octeti    = "HTTP/1.1 200 OK\r\nTransfer-Encoding: "
        "chunked\r\n\r\n5\r\nSalve\r\n5\r\nMunde\r\n0\r\n\r\n";
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    v = http_vocare(r, _petitio("http", "127.0.0.1", s->portus,
        c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
    o.abrumpenda = v;
    CREDO_NECESSE(_currere(r, v, II * M));
    CREDO_AEQUALIS_I32(o.finis, I);
    CREDO_AEQUALIS_I32(o.rivi, I);
    CREDO_AEQUALIS_I32((i32)http_vocatio_status(v),
        (i32)HTTP_VOCATIO_ABRUPTA);
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
}

/* Review focus IV: duae vocationes in uno reactore */
interior vacuum
_sectio_duae_simul (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  ga[I];
    ServusGradus  gb[I];
  ServusScriptus* sa;
  ServusScriptus* sb;
      Observatio  oa;
      Observatio  ob;
     HttpVocatio* va;
     HttpVocatio* vb;
             s64  initium;

    ga[ZEPHYRUM].octeti    =
        "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\n"
        "Prima";
    ga[ZEPHYRUM].mensura   = ZEPHYRUM;
    ga[ZEPHYRUM].mora_ms   = XXX;
    ga[ZEPHYRUM].claudere  = FALSUM;
    gb[ZEPHYRUM].octeti    =
        "HTTP/1.1 200 OK\r\nContent-Length: 7\r\n\r\n"
        "Secunda";
    gb[ZEPHYRUM].mensura = ZEPHYRUM;
    gb[ZEPHYRUM].mora_ms = X;
    gb[ZEPHYRUM].claudere = FALSUM;
    sa = servum_scriptum_creare(r, ga, I, c->piscina);
    sb = servum_scriptum_creare(r, gb, I, c->piscina);
    CREDO_NECESSE(sa != NIHIL && sb != NIHIL);
    _observationem_initiare(&oa, c->piscina);
    _observationem_initiare(&ob, c->piscina);
    va = http_vocare(r, _petitio("http", "127.0.0.1", sa->portus,
        c->piscina), _rivus, &oa, _finis, &oa, NIHIL, c->piscina);
    vb = http_vocare(r, _petitio("http", "127.0.0.1", sb->portus,
        c->piscina), _rivus, &ob, _finis, &ob, NIHIL, c->piscina);
    initium = _ms_nunc();
    dum (   (!http_vocatio_finita(va) || !http_vocatio_finita(vb))
         && _ms_nunc() - initium < II * M)
    {
        (vacuum)reactor_poll(r, XX);
    }
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(oa.corpus),
        "Prima");
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(ob.corpus),
        "Secunda");
    CREDO_AEQUALIS_I32(oa.finis, I);
    CREDO_AEQUALIS_I32(ob.finis, I);
    servum_scriptum_claudere(sa);
    servum_scriptum_claudere(sb);
}

/* non-2xx: rivus tacet, corpus totum in responso */
interior vacuum
_sectio_non_ducenti (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
     HttpVocatio* v;
    HttpResultus  res;

    g[ZEPHYRUM].octeti    =
        "HTTP/1.1 404 Not Found\r\nContent-Length: 6"
        "\r\n\r\nabsens";
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    _observationem_initiare(&o, c->piscina);
    v = http_vocare(r, _petitio("http", "127.0.0.1", s->portus,
        c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
    CREDO_NECESSE(_currere(r, v, II * M));
    res = http_vocatio_resultus(v);
    CREDO_NECESSE(res.successus && res.responsum != NIHIL);
    CREDO_AEQUALIS_I32(res.responsum->status, CDIV);
    CREDO_AEQUALIS_I32(o.rivi, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(res.responsum->corpus, "absens");
    servum_scriptum_claudere(s);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "divisiones", _sectio_divisiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { "manet apertum", _sectio_manet_apertum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "clausum statim", _sectio_clausum_statim, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "recusata", _sectio_recusata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "localhost", _sectio_hospes_localis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "otium", _sectio_otium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pactio tacita", _sectio_pactio_tacita, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "abrumpere", _sectio_abrumpere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abrumpere intra rivum", _sectio_abrumpere_intra_rivum, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "duae simul", _sectio_duae_simul, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non ducenti", _sectio_non_ducenti, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("http_vocatio", SECTIONES);
}
