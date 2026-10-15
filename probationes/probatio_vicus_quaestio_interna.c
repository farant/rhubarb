/* probatio_vicus_quaestio_interna.c - porta $question in processu
 * (http-vocatio II.6) contra servum scriptum: bonum (textus planatus
 * per pulsus, usus, clavis in capite), clavis absens, mors (clausura
 * post deltam), error (eventus overloaded), pravum, lente + abrumpere.
 * Clavis FICTA in $TMPDIR; numquam in textu aut errore. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "reactor.h"
#include "vicus_quaestio.h"
#include "vicus_quaestio_interna.h"
#include "servus_scriptus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define CLAVIS_FICTA "clavis-ficta-probationis"

#define CAPUT_RIVI \
    "HTTP/1.1 200 OK\r\nContent-Type: text/event-stream\r\n" \
    "Connection: close\r\n\r\n"

#define INITIUM_RIVI \
    "event: message_start\ndata: {\"type\":\"message_start\"," \
    "\"message\":{\"id\":\"msg_1\",\"type\":\"message\",\"role\":" \
    "\"assistant\",\"model\":\"m\",\"content\":[]," \
    "\"stop_reason\":null,\"stop_sequence\":null,\"usage\":" \
    "{\"input_tokens\":12,\"output_tokens\":1}}}\n\n" \
    "event: content_block_start\ndata: {\"type\":" \
    "\"content_block_start\",\"index\":0,\"content_block\":" \
    "{\"type\":\"text\",\"text\":\"\"}}\n\n"

#define DELTA(t) \
    "event: content_block_delta\ndata: {\"type\":" \
    "\"content_block_delta\",\"index\":0,\"delta\":{\"type\":" \
    "\"text_delta\",\"text\":\"" t "\"}}\n\n"

#define FINIS_RIVI \
    "event: content_block_stop\ndata: {\"type\":" \
    "\"content_block_stop\",\"index\":0}\n\n" \
    "event: message_delta\ndata: {\"type\":\"message_delta\"," \
    "\"delta\":{\"stop_reason\":\"end_turn\",\"stop_sequence\":null}," \
    "\"usage\":{\"output_tokens\":34}}\n\n" \
    "event: message_stop\ndata: {\"type\":\"message_stop\"}\n\n"

#define ERROR_RIVI \
    "event: error\ndata: {\"type\":\"error\",\"error\":{\"type\":" \
    "\"overloaded_error\",\"message\":\"Overloaded\"}}\n\n"

nomen structura {
                  b32 finitus;
               chorda textus;
               chorda error;
                  s64 input;
                  s64 output;
                  i32 mutationes;
} Summa;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

/* clavis ficta in $TMPDIR/vqi.<pid>.clavis */
interior constans character*
_clavem_scribere (
    Piscina* p)
{
    constans character* tmp = getenv("TMPDIR");
             character* via;
                FILE* f;

    via = (character*)piscina_allocare(p, D);
    sprintf(via, "%s/vqi.%d.clavis", tmp ? tmp : "/tmp",
        (integer)getpid());
    f = fopen(via, "wb");
    si (f)
    {
        (vacuum)fputs(CLAVIS_FICTA "\n", f);
        (vacuum)fclose(f);
    }
    redde via;
}

interior VicusPortaQuaestionis
_portam (
               Reactor* r,
                   i32  portus,
    constans character* via_clavis,
               Piscina* p)
{
    VicusQuaestioInternaOptiones  o;
                       character* url;

    url = (character*)piscina_allocare(p, CXXVIII);
    sprintf(url, "http://127.0.0.1:%u/v1/messages",
        (insignatus integer)portus);
    o.via_clavis        = via_clavis;
    o.url               = url;
    o.conatus_maximi    = I;
    o.mora_iterandi_ms  = X;
    redde vicus_quaestio_interna(r, &o, p);
}

/* hospes: reactor in omni pulsu (R1), deinde porta */
interior Summa
_currere (
                  Reactor* r,
    VicusPortaQuaestionis* porta,
                   vacuum* h,
                      s64  tectum_ms,
                      b32  usque_ad_textum,
                  Piscina* p)
{
    Summa s;
      s64 initium = _ms_nunc();

    memset(&s, ZEPHYRUM, magnitudo(s));
    dum (_ms_nunc() - initium < tectum_ms)
    {
        VicusPulsusQuaestionis q;

        (vacuum)reactor_poll(r, X);
        q = porta->pulsare(h, p);
        si (q.mutatum)
        {
            s.textus = q.textus;
            s.mutationes++;
        }
        si (q.error.mensura > ZEPHYRUM)
        {
            s.error = q.error;
        }
        si (q.input || q.output)
        {
            s.input   = q.input;
            s.output  = q.output;
        }
        si (q.finitus)
        {
            s.finitus = VERUM;
            frange;
        }
        si (usque_ad_textum && s.mutationes > ZEPHYRUM)
        {
            frange;
        }
    }
    redde s;
}

interior vacuum*
_incipere (
    VicusPortaQuaestionis* porta,
                  Piscina* p,
                   chorda* error)
{
    error->datum    = NIHIL;
    error->mensura  = ZEPHYRUM;
    redde porta->incipere(porta->ctx, chorda_ex_literis("Quid?", p), X,
        LVI, p, error);
}

interior b32
_sine_clave (
     chorda  x,
    Piscina* p)
{
    redde x.mensura == ZEPHYRUM || !chorda_continet(x,
        chorda_ex_literis(CLAVIS_FICTA, p));
}

interior vacuum
_sectio_bonum (
    CredoContextus* c)
{
                  Reactor* r = reactor_creare(c->piscina);
             ServusGradus  g[II];
           ServusScriptus* s;
    VicusPortaQuaestionis  porta;
                   vacuum* h;
                   chorda  error;
                    Summa  x;
       constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti = CAPUT_RIVI INITIUM_RIVI DELTA("Salve");
    g[ZEPHYRUM].mensura = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms = ZEPHYRUM;
    g[ZEPHYRUM].claudere = FALSUM;
    g[I].octeti = DELTA(" munde.") FINIS_RIVI;
    g[I].mensura = ZEPHYRUM;
    g[I].mora_ms = L;
    g[I].claudere = VERUM;
    s = servum_scriptum_creare(r, g, II, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta = _portam(r, s->portus, via, c->piscina);
    CREDO_NECESSE(porta.incipere != NIHIL);
    h = _incipere(&porta, c->piscina, &error);
    CREDO_NECESSE_NON_NIHIL(h);
    x = _currere(r, &porta, h, III * M, FALSUM, c->piscina);
    CREDO_VERUM(x.finitus);
    CREDO_AEQUALIS_I32(x.error.mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(x.textus, "Salve munde.");
    CREDO_VERUM(x.mutationes >= II);              /* per partes */
    CREDO_AEQUALIS_S64(x.input, XII);
    CREDO_AEQUALIS_S64(x.output, XXXIV);
    CREDO_CHORDA_CONTINET(chorda_aedificator_spectare(s->petitio),
        chorda_ex_literis("x-api-key: " CLAVIS_FICTA, c->piscina));
    CREDO_VERUM(porta.pulsare(h, c->piscina).finitus);
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
    (vacuum)unlink(via);
}

interior vacuum
_sectio_clavis_absens (
    CredoContextus* c)
{
                  Reactor* r = reactor_creare(c->piscina);
    VicusPortaQuaestionis  porta;
                   chorda  error;

    porta = _portam(r, LXXX, "/via/nulla/anthropic.clavis", c->piscina);
    CREDO_VERUM(_incipere(&porta, c->piscina, &error) == NIHIL);
    CREDO_CHORDA_CONTINET(error, chorda_ex_literis("key", c->piscina));
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

/* delta, deinde clausura sine message_stop: error post textum */
interior vacuum
_sectio_mors (
    CredoContextus* c)
{
                  Reactor* r = reactor_creare(c->piscina);
             ServusGradus  g[I];
           ServusScriptus* s;
    VicusPortaQuaestionis  porta;
                   vacuum* h;
                   chorda  error;
                    Summa  x;
       constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI INITIUM_RIVI DELTA("Pars");
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta  = _portam(r, s->portus, via, c->piscina);
    h      = _incipere(&porta, c->piscina, &error);
    CREDO_NECESSE_NON_NIHIL(h);
    x = _currere(r, &porta, h, III * M, FALSUM, c->piscina);
    CREDO_VERUM(x.finitus);
    CREDO_VERUM(x.error.mensura > ZEPHYRUM);
    CREDO_VERUM(_sine_clave(x.error, c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(x.textus, "Pars");
    servum_scriptum_claudere(s);
    (vacuum)unlink(via);
}

interior vacuum
_sectio_error (
    CredoContextus* c)
{
                  Reactor* r = reactor_creare(c->piscina);
             ServusGradus  g[I];
           ServusScriptus* s;
    VicusPortaQuaestionis  porta;
                   vacuum* h;
                   chorda  error;
                    Summa  x;
       constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI INITIUM_RIVI ERROR_RIVI;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta  = _portam(r, s->portus, via, c->piscina);
    h      = _incipere(&porta, c->piscina, &error);
    CREDO_NECESSE_NON_NIHIL(h);
    x = _currere(r, &porta, h, III * M, FALSUM, c->piscina);
    CREDO_VERUM(x.finitus);
    CREDO_CHORDA_CONTINET(x.error, chorda_ex_literis("Overloaded",
        c->piscina));
    CREDO_VERUM(_sine_clave(x.error, c->piscina));
    CREDO_AEQUALIS_I32(x.textus.mensura, ZEPHYRUM);
    servum_scriptum_claudere(s);
    (vacuum)unlink(via);
}

/* octeti non SSE post capita rivi: error nominatus */
interior vacuum
_sectio_pravum (
    CredoContextus* c)
{
                  Reactor* r = reactor_creare(c->piscina);
             ServusGradus  g[I];
           ServusScriptus* s;
    VicusPortaQuaestionis  porta;
                   vacuum* h;
                   chorda  error;
                    Summa  x;
       constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI "non est rivus\n\n";
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta  = _portam(r, s->portus, via, c->piscina);
    h      = _incipere(&porta, c->piscina, &error);
    CREDO_NECESSE_NON_NIHIL(h);
    x = _currere(r, &porta, h, III * M, FALSUM, c->piscina);
    CREDO_VERUM(x.finitus);
    CREDO_VERUM(x.error.mensura > ZEPHYRUM);
    servum_scriptum_claudere(s);
    (vacuum)unlink(via);
}

/* textus primus, deinde silentium; abrumpere finit statim */
interior vacuum
_sectio_lente (
    CredoContextus* c)
{
                  Reactor* r = reactor_creare(c->piscina);
             ServusGradus  g[I];
           ServusScriptus* s;
    VicusPortaQuaestionis  porta;
                   vacuum* h;
                   chorda  error;
                    Summa  x;
                      s64  initium;
       constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI INITIUM_RIVI DELTA("Prima");
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s                     = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta  = _portam(r, s->portus, via, c->piscina);
    h      = _incipere(&porta, c->piscina, &error);
    CREDO_NECESSE_NON_NIHIL(h);
    x = _currere(r, &porta, h, III * M, VERUM, c->piscina);
    CREDO_FALSUM(x.finitus);
    CREDO_CHORDA_AEQUALIS_LITERIS(x.textus, "Prima");
    initium = _ms_nunc();
    porta.abrumpere(h);
    CREDO_VERUM(porta.pulsare(h, c->piscina).finitus);
    CREDO_VERUM(_ms_nunc() - initium < C);
    servum_scriptum_claudere(s);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
    (vacuum)unlink(via);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "bonum", _sectio_bonum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clavis absens", _sectio_clavis_absens, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "mors", _sectio_mors, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error", _sectio_error, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pravum", _sectio_pravum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lente", _sectio_lente, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_quaestio_interna", SECTIONES);
}
