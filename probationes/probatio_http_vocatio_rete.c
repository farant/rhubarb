/* probatio_http_vocatio_rete.c - vocatio vera per rete (http-vocatio
 * III): HTTPS ad api.anthropic.com SINE clave -> 401
 * authentication_error (DNS + TLS gradatim + HTTP sine secreto); nomen
 * .invalid -> error nominatus. Manu: ./compile_tests.sh
 * probatio_http_vocatio_rete */
/* <aedilis facultas="rete"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "reactor.h"
#include "http.h"
#include "http_vocatio.h"
#include "nomina.h"
#include <string.h>
#include <sys/time.h>

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_rivus (
         vacuum* datum,
    constans i8* octeti,
            i32  n)
{
    (vacuum)datum;
    (vacuum)octeti;
    (vacuum)n;
}

interior vacuum
_finis (
         vacuum* datum,
    HttpVocatio* vocatio)
{
    (vacuum)vocatio;
    (*(i32*)datum)++;
}

interior vacuum
_sectio_sine_clave (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
     HttpPetitio* hp = http_petitio_creare(c->piscina, HTTP_POST,
         "https://api.anthropic.com/v1/messages");
     HttpVocatio* v;
    HttpResultus  res;
             i32  fines = ZEPHYRUM;
             s64  initium;

    http_petitio_caput_addere(hp, "content-type", "application/json");
    http_petitio_caput_addere(hp, "anthropic-version", "2023-06-01");
    http_petitio_corpus_ponere_chorda(hp, chorda_ex_literis("{}",
        c->piscina));
    v = http_vocare(r, hp, _rivus, NIHIL, _finis, &fines, NIHIL,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    initium = _ms_nunc();
    dum (!http_vocatio_finita(v) && _ms_nunc() - initium < XX * M)
    {
        (vacuum)reactor_poll(r, C);
    }
    res = http_vocatio_resultus(v);
    CREDO_AEQUALIS_I32(fines, I);
    CREDO_NECESSE(res.successus && res.responsum != NIHIL);
    CREDO_AEQUALIS_I32(res.responsum->status, CDI);
    CREDO_CHORDA_CONTINET(res.responsum->corpus, chorda_ex_literis(
        "authentication_error", c->piscina));
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
}

nomen structura {
       i32 vocatus;
    chorda error;
} Nominatio;

interior vacuum
_nomina_finis (
                      vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    Nominatio* x = (Nominatio*)datum;

    (vacuum)addressus;
    (vacuum)numerus;
    x->vocatus++;
    x->error = error;
}

interior vacuum
_sectio_nomen_ignotum (
    CredoContextus* c)
{
      Reactor* r = reactor_creare(c->piscina);
    Nominatio  x;
       chorda  error;
          s64  initium;

    memset(&x, ZEPHYRUM, magnitudo(x));
    CREDO_NECESSE_NON_NIHIL(nomina_quaerere_incipere(r,
        chorda_ex_literis("nomen-nullum.invalid", c->piscina), LXXX,
        _nomina_finis, &x, c->piscina, &error));
    initium = _ms_nunc();
    dum (x.vocatus == ZEPHYRUM && _ms_nunc() - initium < X * M)
    {
        (vacuum)reactor_poll(r, C);
    }
    CREDO_AEQUALIS_I32(x.vocatus, I);
    CREDO_VERUM(x.error.mensura > ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "sine clave", _sectio_sine_clave, NIHIL, NIHIL, "rete", NIHIL },
    { "nomen ignotum", _sectio_nomen_ignotum, NIHIL, NIHIL, "rete",
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("http_vocatio_rete", SECTIONES);
}
