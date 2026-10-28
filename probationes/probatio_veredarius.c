/* probatio_veredarius.c - veredarius (communio-plan-5 T1): petitio
 * HTTP una per interfaciem; veredarius domesticus super http_vocatio
 * in reactore; cliens communionis nullam aliam viam HTTP habet. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "hospitium.h"
#include "http.h"
#include "veredarius_domesticus.h"
#include "filum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina* piscina;

/* quod servus vidit */
hic_manens structura {
       i32 petitiones;
    chorda auctoritas;
    chorda mimen;
    chorda corpus;
} servus;

/* finis veredarii */
hic_manens structura {
       i32 vocationes;
       b32 successus;
       i32 status;
    chorda corpus;
    chorda causa;
} exitus;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
    memset(&servus, ZEPHYRUM, magnitudo(servus));
    memset(&exitus, ZEPHYRUM, magnitudo(exitus));
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

interior chorda
copia (
    chorda c)
{
    redde chorda_ex_literis(chorda_ut_cstr(c, piscina), piscina);
}

interior vacuum
_viam_salve (
    HospitiumColloquium* col)
{
    servus.petitiones++;
    servus.auctoritas  = copia(colloquium_caput(col, "Authorization"));
    servus.mimen       = copia(colloquium_caput(col, "Content-Type"));
    servus.corpus      = copia(colloquium_petitio(col)->corpus);
    colloquium_respondere(col, servus.corpus.mensura > ZEPHYRUM ? CCI
        : CC, "text/plain", lit("salve"));
}

/* numquam respondet */
interior vacuum
_viam_tacitam (
    HospitiumColloquium* col)
{
    servus.petitiones++;
    (vacuum)colloquium_differre(col);
}

interior vacuum
_finis (
                          vacuum* ctx,
    constans VeredariusResponsum* r)
{
    (vacuum)ctx;
    exitus.vocationes++;
    exitus.successus  = r->successus;
    exitus.status     = r->status;
    exitus.corpus     = copia(r->corpus);
    exitus.causa      = copia(r->causa);
}

interior vacuum
agere (
    Reactor* r,
        i32  ms)
{
    i32 gyri;

    per (gyri = ZEPHYRUM; exitus.vocationes == ZEPHYRUM && gyri < ms
         / X; gyri++)
    {
        (vacuum)reactor_poll(r, X);
    }
}

interior VeredariusPetitio
petitio (
    VeredariusMethodus  methodus,
    constans character* url,
                   i32  tempus_ms)
{
    VeredariusPetitio p;

    memset(&p, ZEPHYRUM, magnitudo(p));
    p.methodus   = methodus;
    p.url        = url;
    p.tempus_ms  = tempus_ms;
    redde p;
}

interior vacuum
_sectio_domesticus (
    CredoContextus* c)
{
                  Reactor* r;
                Hospitium* h;
    HospitiumConfiguratio  cfg;
               Veredarius* v;
        VeredariusPetitio  p;
                character  url[CXXVIII];
                      i32  signum;

    (vacuum)c;
    r = reactor_creare(piscina);
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.reactor  = r;
    h            = r ? hospitium_creare(piscina, &cfg) : NIHIL;
    CREDO_NECESSE(h
        && hospitium_praebere(h, HTTP_GET, "/salve", _viam_salve, NIHIL)
        && hospitium_praebere(h, HTTP_POST, "/salve", _viam_salve,
        NIHIL)
        && hospitium_praebere(h, HTTP_GET, "/tacita", _viam_tacitam,
               NIHIL));
    v = veredarium_domesticum_creare(piscina, r);
    CREDO_NECESSE_NON_NIHIL(v);
    sprintf(url, "http://127.0.0.1:%d/salve",
        (integer)hospitium_portus(h));
    p = petitio(VEREDARIUS_GET, url, M * V);
    CREDO_VERUM(v->petere(v->sui, &p, piscina, _finis, NIHIL, &signum));
    CREDO_AEQUALIS_I32(exitus.vocationes, ZEPHYRUM);
    agere(r, M * V);
    CREDO_AEQUALIS_I32(exitus.vocationes, I);
    CREDO_VERUM(exitus.successus);
    CREDO_AEQUALIS_I32(exitus.status, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(exitus.corpus, "salve");
    CREDO_AEQUALIS_I32(servus.auctoritas.mensura, ZEPHYRUM);

    CREDO_GRADUS("POST cum corpore et auctoritate");
    exitus.vocationes  = ZEPHYRUM;
    p                  = petitio(VEREDARIUS_POST, url, M * V);
    p.auctoritas       = "Bearer tessera-probationis";
    p.corpus           = lit("{\"a\":1}");
    p.mimen            = "application/json";
    CREDO_VERUM(v->petere(v->sui, &p, piscina, _finis, NIHIL, &signum));
    agere(r, M * V);
    CREDO_AEQUALIS_I32(exitus.vocationes, I);
    CREDO_AEQUALIS_I32(exitus.status, CCI);
    CREDO_CHORDA_AEQUALIS_LITERIS(servus.auctoritas,
        "Bearer tessera-probationis");
    CREDO_CHORDA_AEQUALIS_LITERIS(servus.mimen, "application/json");
    CREDO_CHORDA_AEQUALIS_LITERIS(servus.corpus, "{\"a\":1}");

    CREDO_GRADUS("abrumpere: finis numquam");
    exitus.vocationes  = ZEPHYRUM;
    sprintf(url, "http://127.0.0.1:%d/tacita?q=arcanum",
        (integer)hospitium_portus(h));
    p = petitio(VEREDARIUS_GET, url, M * V);
    CREDO_VERUM(v->petere(v->sui, &p, piscina, _finis, NIHIL, &signum));
    v->abrumpere(v->sui, signum);
    agere(r, CCC);
    CREDO_AEQUALIS_I32(exitus.vocationes, ZEPHYRUM);
    v->abrumpere(v->sui, signum);    /* iterum: innocuum */

    CREDO_GRADUS("tempus: successus nullus, causa sine URL");
    p = petitio(VEREDARIUS_GET, url, CCC);
    CREDO_VERUM(v->petere(v->sui, &p, piscina, _finis, NIHIL, &signum));
    agere(r, M * III);
    CREDO_AEQUALIS_I32(exitus.vocationes, I);
    CREDO_FALSUM(exitus.successus);
    CREDO_AEQUALIS_I32(exitus.status, ZEPHYRUM);
    CREDO_VERUM(exitus.causa.mensura > ZEPHYRUM);
    CREDO_FALSUM(chorda_continet(exitus.causa, lit("arcanum")));
    CREDO_FALSUM(chorda_continet(exitus.causa, lit("tacita")));
    hospitium_destruere(h);
    reactor_destruere(r);
}

/* C1/regula: omnis HTTP clientis per veredarium */
interior vacuum
_sectio_nullus_http (
    CredoContextus* c)
{
    chorda fons;

    (vacuum)c;
    fons = filum_legere_totum("lib/communio_cliens.c", piscina);
    CREDO_NECESSE(fons.mensura > ZEPHYRUM);
    CREDO_FALSUM(chorda_continet(fons, lit("http_vocare")));
    CREDO_FALSUM(chorda_continet(fons, lit("http_vocatio.h")));
    CREDO_VERUM(chorda_continet(fons, lit("veredarius.h")));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "domesticus", _sectio_domesticus, _parare, _purgare, NIHIL,
        NIHIL },
    { "nullus http_vocare in cliente", _sectio_nullus_http, _parare,
        _purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("veredarius", SECTIONES);
}
