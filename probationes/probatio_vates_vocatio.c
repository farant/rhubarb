/* probatio_vates_vocatio.c - vates per rivum NON obstruens contra
 * servum scriptum (http-vocatio II.5): par cum vates_anthropic_rivum_
 * legere in eisdem eventis; petitio rivi (stream, clavis, via);
 * iteratio ante deltam (529 bis, acceptae II); post deltam nulla
 * iteratio; abrumpere synchronus (R9); mora iterandi non dormit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "reactor.h"
#include "vates.h"
#include "servus_scriptus.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define CAPUT_RIVI \
    "HTTP/1.1 200 OK\r\nContent-Type: text/event-stream\r\n" \
    "Connection: close\r\n\r\n"

#define EVENTA_PRIMA \
    "event: message_start\ndata: {\"type\":\"message_start\"," \
    "\"message\":{\"id\":\"msg_1\",\"type\":\"message\",\"role\":" \
    "\"assistant\",\"model\":\"m\",\"content\":[]," \
    "\"stop_reason\":null,\"stop_sequence\":null,\"usage\":" \
    "{\"input_tokens\":12,\"output_tokens\":1}}}\n\n" \
    "event: content_block_start\ndata: {\"type\":" \
    "\"content_block_start\",\"index\":0,\"content_block\":" \
    "{\"type\":\"text\",\"text\":\"\"}}\n\n" \
    "event: content_block_delta\ndata: {\"type\":" \
    "\"content_block_delta\",\"index\":0,\"delta\":{\"type\":" \
    "\"text_delta\",\"text\":\"Salve\"}}\n\n"

#define EVENTA_ULTIMA \
    "event: content_block_delta\ndata: {\"type\":" \
    "\"content_block_delta\",\"index\":0,\"delta\":{\"type\":" \
    "\"text_delta\",\"text\":\" munde.\"}}\n\n" \
    "event: content_block_stop\ndata: {\"type\":" \
    "\"content_block_stop\",\"index\":0}\n\n" \
    "event: message_delta\ndata: {\"type\":\"message_delta\"," \
    "\"delta\":{\"stop_reason\":\"end_turn\",\"stop_sequence\":null}," \
    "\"usage\":{\"output_tokens\":34}}\n\n" \
    "event: message_stop\ndata: {\"type\":\"message_stop\"}\n\n"

#define ERROR_RIVI \
    "event: error\ndata: {\"type\":\"error\",\"error\":{\"type\":" \
    "\"overloaded_error\",\"message\":\"Overloaded\"}}\n\n"

#define RESPONSUM_PLENUS \
    "HTTP/1.1 529 Overloaded\r\nContent-Type: application/json\r\n" \
    "Content-Length: 75\r\nConnection: close\r\n\r\n" \
    "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\"," \
    "\"message\":\"Overloaded\"}}"

nomen structura {
                  i32  fines;
    ChordaAedificator* deltae;
} Observatio;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_delta (
    vacuum* datum,
    chorda  delta,
       b32  cogitatio)
{
    Observatio* o = (Observatio*)datum;

    si (!cogitatio)
    {
        chorda_aedificator_appendere_chorda(o->deltae, delta);
        chorda_aedificator_appendere_character(o->deltae, '|');
    }
}

interior vacuum
_finis (
          vacuum* datum,
    VatesVocatio* vocatio)
{
    (vacuum)vocatio;
    ((Observatio*)datum)->fines++;
}

interior Vates*
_vatem (
            i32  portus,
    MoraAngusta  mora_iterandi_ms,
        Piscina* p)
{
    VatesOptiones  o = vates_optiones_ordinariae();
        character  url[CXXVIII];
        character* copia;

    sprintf(url, "http://127.0.0.1:%u/v1/messages",
        (insignatus integer)portus);
    copia               = chorda_ut_cstr(chorda_ex_literis(url, p), p);
    o.url               = copia;
    o.sine_herbario     = VERUM;
    o.conatus_maximi    = II;
    o.mora_iterandi_ms  = mora_iterandi_ms;
    redde vates_aperire(p, chorda_ex_literis("anthropic", p),
        chorda_ex_literis("clavis-ficta-probationis", p), &o);
}

interior VatesPetitio*
_petitio (
    Piscina* p)
{
    VatesPetitio* pe = vates_petitio_creare(p, chorda_ex_literis("m",
        p));
    VatesNuntius* n = vates_nuntium_addere(pe, VATES_USOR);

    vates_petitio_maxima_ponere(pe, C);
    (vacuum)vates_textum_addere(n, chorda_ex_literis("salve", p));
    redde pe;
}

interior VatesResponsum*
_currere (
         Reactor* r,
    VatesVocatio* vv,
             s64  tectum_ms,
             s64* poll_maximus)
{
    s64 initium = _ms_nunc();

    *poll_maximus = ZEPHYRUM;
    dum (!vates_vocatio_finita(vv) && _ms_nunc() - initium < tectum_ms)
    {
        s64 a = _ms_nunc();

        (vacuum)reactor_poll(r, X);
        si (_ms_nunc() - a > *poll_maximus)
        {
            *poll_maximus = _ms_nunc() - a;
        }
    }
    redde vates_vocatio_responsum(vv);
}

/* textus per partes, finis; par cum lectione obstruente eorundem */
interior vacuum
_sectio_par (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[II];
  ServusScriptus* s;
      Observatio  o;
    VatesVocatio* vv;
  VatesResponsum* rv;
  VatesResponsum* rl;
             s64  pm;

    g[ZEPHYRUM].octeti = CAPUT_RIVI EVENTA_PRIMA;
    g[ZEPHYRUM].mensura = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms = ZEPHYRUM;
    g[ZEPHYRUM].claudere = FALSUM;
    g[I].octeti = EVENTA_ULTIMA;
    g[I].mensura = ZEPHYRUM;
    g[I].mora_ms = XXX;
    g[I].claudere = VERUM;
    s = servum_scriptum_creare(r, g, II, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    o.fines   = ZEPHYRUM;
    o.deltae  = chorda_aedificator_creare(c->piscina, LXIV);
    vv = vates_vocare_rivum(_vatem(s->portus, ZEPHYRUM, c->piscina),
        _petitio(c->piscina), chorda_ex_literis("proba", c->piscina), r,
        _delta, &o, _finis, &o, c->piscina);
    CREDO_NECESSE_NON_NIHIL(vv);
    CREDO_NECESSE(o.fines == ZEPHYRUM);
    rv = _currere(r, vv, III * M, &pm);
    CREDO_NECESSE_NON_NIHIL(rv);
    CREDO_AEQUALIS_I32(o.fines, I);
    CREDO_VERUM(rv->successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(o.deltae),
        "Salve| munde.|");
    rl = vates_anthropic_rivum_legere(chorda_ex_literis(EVENTA_PRIMA
        EVENTA_ULTIMA, c->piscina), NIHIL, NIHIL, c->piscina);
    CREDO_NECESSE_NON_NIHIL(rl);
    CREDO_AEQUALIS_I32((i32)rv->successus, (i32)rl->successus);
    CREDO_VERUM(chorda_aequalis(rv->causa_finis_cruda,
        rl->causa_finis_cruda));
    CREDO_AEQUALIS_S64(rv->usus.input, rl->usus.input);
    CREDO_AEQUALIS_S64(rv->usus.output, rl->usus.output);
    CREDO_AEQUALIS_I32((i32)xar_numerus(rv->bloci),
        (i32)xar_numerus(rl->bloci));
    CREDO_VERUM(chorda_aequalis(((VatesBlocus*)xar_obtinere(rv->bloci,
        ZEPHYRUM))->textus, ((VatesBlocus*)xar_obtinere(rl->bloci,
        ZEPHYRUM))->textus));
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

/* petitio: POST via, stream, clavis in capite */
interior vacuum
_sectio_petitio (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
    VatesVocatio* vv;
          chorda  visa;
             s64  pm;

    g[ZEPHYRUM].octeti    = CAPUT_RIVI EVENTA_PRIMA EVENTA_ULTIMA;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    o.fines   = ZEPHYRUM;
    o.deltae  = chorda_aedificator_creare(c->piscina, LXIV);
    vv = vates_vocare_rivum(_vatem(s->portus, ZEPHYRUM, c->piscina),
        _petitio(c->piscina), chorda_ex_literis("proba", c->piscina), r,
        _delta, &o, _finis, &o, c->piscina);
    (vacuum)_currere(r, vv, III * M, &pm);
    visa = chorda_aedificator_spectare(s->petitio);
    CREDO_CHORDA_CONTINET(visa, chorda_ex_literis("POST /v1/messages ",
        c->piscina));
    CREDO_CHORDA_CONTINET(visa, chorda_ex_literis("\"stream\":true",
        c->piscina));
    CREDO_CHORDA_CONTINET(visa, chorda_ex_literis(
        "clavis-ficta-probationis", c->piscina));
    servum_scriptum_claudere(s);
}

/* 529 ante deltam: iteratur (conatus II, acceptae II); mora iterandi
 * CC ms per timer - nullus reactor_poll(X) diutius L ms */
interior vacuum
_sectio_iteratio (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
    VatesVocatio* vv;
  VatesResponsum* rv;
             s64  pm;

    g[ZEPHYRUM].octeti    = RESPONSUM_PLENUS;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    o.fines   = ZEPHYRUM;
    o.deltae  = chorda_aedificator_creare(c->piscina, LXIV);
    vv = vates_vocare_rivum(_vatem(s->portus, CC, c->piscina),
        _petitio(c->piscina), chorda_ex_literis("proba", c->piscina), r,
        _delta, &o, _finis, &o, c->piscina);
    rv = _currere(r, vv, III * M, &pm);
    CREDO_NECESSE_NON_NIHIL(rv);
    CREDO_FALSUM(rv->successus);
    CREDO_AEQUALIS_I32((i32)rv->error, (i32)VATES_ERROR_STATUS);
    CREDO_AEQUALIS_I32(rv->conatus, II);
    CREDO_AEQUALIS_I32(s->acceptae, II);
    CREDO_VERUM(pm < L);
    servum_scriptum_claudere(s);
}

/* post deltam numquam iteratur, per utrumque ramum iudicii: (a) rivus
 * in medio corpore abruptus (chunked truncatum -> vocatio HTTP fracta,
 * ramus vecturae); (b) eventus overloaded_error post deltam (genus
 * iterabile, ramus collectoris). Utroque: conatus I, acceptio una,
 * textus visus servatus. (Corpus clausura terminatum sine
 * message_stop HTTP-sanum est et ab collectore iudicatur - neutrum
 * ramum custodiae probaret.) */
interior VatesResponsum*
_post_deltam (
    constans character*  octeti,
               Reactor*  r,
        ServusScriptus** servus,
            Observatio*  o,
               Piscina*  p)
{
    ServusGradus* g = (ServusGradus*)piscina_allocare(p,
        magnitudo(ServusGradus));
           s64 pm;

    g->octeti    = octeti;
    g->mensura   = ZEPHYRUM;
    g->mora_ms   = ZEPHYRUM;
    g->claudere  = VERUM;
    *servus      = servum_scriptum_creare(r, g, I, p);
    si (!*servus)
    {
        redde NIHIL;
    }
    o->fines   = ZEPHYRUM;
    o->deltae  = chorda_aedificator_creare(p, LXIV);
    redde _currere(r, vates_vocare_rivum(_vatem((*servus)->portus,
        X, p), _petitio(p), chorda_ex_literis("proba", p), r, _delta, o,
        _finis, o, p), III * M, &pm);
}

interior vacuum
_sectio_post_deltam (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
  ServusScriptus* s;
      Observatio  o;
  VatesResponsum* rv;
       character* truncatum;

    /* (a) chunked: capita, frustum unum (EVENTA_PRIMA), clausura sine
     * frusto ultimo */
    truncatum = (character*)piscina_allocare(c->piscina,
        (memoriae_index)strlen(EVENTA_PRIMA) + CC);
    sprintf(truncatum, "HTTP/1.1 200 OK\r\nContent-Type: "
        "text/event-stream\r\nTransfer-Encoding: chunked\r\n\r\n"
        "%x\r\n%s\r\n", (insignatus integer)strlen(EVENTA_PRIMA),
        EVENTA_PRIMA);
    rv = _post_deltam(truncatum, r, &s, &o, c->piscina);
    CREDO_NECESSE_NON_NIHIL(rv);
    CREDO_AEQUALIS_I32((i32)rv->error, (i32)VATES_ERROR_RETE);
    CREDO_AEQUALIS_I32(rv->conatus, I);
    CREDO_AEQUALIS_I32(s->acceptae, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(o.deltae),
        "Salve|");
    servum_scriptum_claudere(s);

    /* (b) overloaded_error post deltam */
    rv = _post_deltam(CAPUT_RIVI EVENTA_PRIMA ERROR_RIVI, r, &s, &o,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(rv);
    CREDO_AEQUALIS_I32((i32)rv->error, (i32)VATES_ERROR_STATUS);
    CREDO_AEQUALIS_I32(rv->conatus, I);
    CREDO_AEQUALIS_I32(s->acceptae, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(o.deltae),
        "Salve|");
    servum_scriptum_claudere(s);
}

/* abrumpere post deltam: finis semel ante reditum, "abruptum" */
interior vacuum
_sectio_abrumpere (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
    VatesVocatio* vv;
             s64  initium;

    g[ZEPHYRUM].octeti    = CAPUT_RIVI EVENTA_PRIMA;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    o.fines   = ZEPHYRUM;
    o.deltae  = chorda_aedificator_creare(c->piscina, LXIV);
    vv = vates_vocare_rivum(_vatem(s->portus, ZEPHYRUM, c->piscina),
        _petitio(c->piscina), chorda_ex_literis("proba", c->piscina), r,
        _delta, &o, _finis, &o, c->piscina);
    initium = _ms_nunc();
    dum (   chorda_aedificator_spectare(o.deltae).mensura == ZEPHYRUM
         && _ms_nunc() - initium < II * M)
    {
        (vacuum)reactor_poll(r, X);
    }
    CREDO_NECESSE(o.fines == ZEPHYRUM);
    vates_vocationem_abrumpere(vv);
    CREDO_AEQUALIS_I32(o.fines, I);
    vates_vocationem_abrumpere(vv);
    CREDO_AEQUALIS_I32(o.fines, I);
    CREDO_NECESSE_NON_NIHIL(vates_vocatio_responsum(vv));
    CREDO_CHORDA_CONTINET(vates_vocatio_responsum(vv)->error_nuntius,
        chorda_ex_literis("abruptum", c->piscina));
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "par", _sectio_par, NIHIL, NIHIL, NIHIL, NIHIL },
    { "petitio", _sectio_petitio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iteratio", _sectio_iteratio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "post deltam", _sectio_post_deltam, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abrumpere", _sectio_abrumpere, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vates_vocatio", SECTIONES);
}
