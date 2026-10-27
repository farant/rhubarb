/* probatio_navigator_petitio.c - articulum petere et servare (navigator
 * N2.5) contra servum scriptum in eodem reactore: radix_fontis URL
 * fontis ad servum vertit, articulus tamen Vicipaediae manet. Finis
 * semel, numquam intra navigator_petere; post finem reactor vacuus. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "reactor.h"
#include "volumen.h"
#include "moneta.h"
#include "filum.h"
#include "navigator_bibliotheca.h"
#include "navigator_petitio.h"
#include "servus_scriptus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define PAGINA(titulus, corpus) \
    "<html><head><title>" titulus "</title></head><body>" \
    "<section data-mw-section-id=\"0\">" corpus "</section>" \
    "</body></html>"

#define AGENS "proba-agens/1"

nomen structura {
                 Volumen* volumen;
    NavigatorBibliotheca* b;
                     i32  numerator;
                 Reactor* reactor;
} Mundus;

nomen structura {
                         i32 finis;
    NavigatorPetitioResultus resultus;
} Observatio;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    m->volumen = volumen_temporarium(c->piscina, "probatio_petitio");
    si (m->volumen)
    {
        m->b = navigator_bibliotheca_aperire(m->volumen,
            moneta_numerator, &m->numerator, c->piscina);
    }
    m->reactor  = reactor_creare(c->piscina);
    c->datum    = m;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* m = (Mundus*)c->datum;

    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
}

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_finis (
                               vacuum* datum,
    constans NavigatorPetitioResultus* r)
{
    Observatio* o = (Observatio*)datum;

    o->finis++;
    o->resultus = *r;
}

/* reactor poscitur usque ad finem (aut tectum) */
interior b32
_currere (
       Reactor* r,
    Observatio* o,
           s64  tectum_ms)
{
    s64 initium = _ms_nunc();

    dum (o->finis == ZEPHYRUM && _ms_nunc() - initium < tectum_ms)
    {
        (vacuum)reactor_poll(r, XX);
    }
    redde o->finis > ZEPHYRUM;
}

/* responsum 200 cum corpore HTML (in piscina: servus eum tenet) */
interior constans character*
_responsum_ducenti (
    constans character* corpus,
               Piscina* p)
{
    character* s;

    s = (character*)piscina_allocare(p, (memoriae_index)(strlen(corpus)
        + CXXVIII));
    sprintf(s, "HTTP/1.1 200 OK\r\nContent-Type: text/html; "
        "charset=utf-8\r\nContent-Length: %u\r\n\r\n%s",
        (insignatus integer)strlen(corpus), corpus);
    redde s;
}

interior ServusScriptus*
_servus (
                 Reactor* r,
      constans character* octeti,
                     b32  claudere,
                 Piscina* p)
{
    ServusGradus* g;

    g = (ServusGradus*)piscina_allocare(p, magnitudo(ServusGradus));
    g->octeti = octeti;
    g->mensura = ZEPHYRUM;
    g->mora_ms = ZEPHYRUM;
    g->claudere = claudere;
    redde servum_scriptum_creare(r, g, I, p);
}

/* optiones: radix ad servum, agens probationis */
interior NavigatorPetitioOptiones
_optiones (
    ServusScriptus* s,
           Piscina* p)
{
    NavigatorPetitioOptiones  o;
                   character* radix;

    memset(&o, ZEPHYRUM, magnitudo(o));
    radix = (character*)piscina_allocare(p, LXIV);
    sprintf(radix, "http://127.0.0.1:%u",
        (insignatus integer)s->portus);
    o.radix_fontis  = radix;
    o.agens         = chorda_ex_literis(AGENS, p);
    redde o;
}

interior b32
_continet (
                chorda  fenum,
    constans character* acus,
               Piscina* p)
{
    redde chorda_continet(fenum, chorda_ex_literis(acus, p));
}

interior b32
_reactor_vacuus (
    Reactor* r)
{
    redde reactor_numerus_fd(r) == ZEPHYRUM
        && reactor_numerus_timer(r) == ZEPHYRUM;
}

/* petere, currere, dimittere; finis numquam intra petere */
interior b32
_petere (
                      Mundus* m,
          constans character* url,
    NavigatorPetitioOptiones* opt,
                  Observatio* o,
                     Piscina* p)
{
    NavigatorPetitio* np;
                 b32  intra;

    memset(o, ZEPHYRUM, magnitudo(*o));
    np = navigator_petere(m->reactor, m->b, chorda_ex_literis(url, p),
        _finis, o, opt);
    si (!np)
    {
        redde FALSUM;
    }
    intra = (o->finis != ZEPHYRUM);
    (vacuum)_currere(m->reactor, o, III * M);
    navigator_petitionem_dimittere(np);
    redde !intra && o->finis == I;
}

interior vacuum
_probare_agens (
    CredoContextus* c)
{
     constans character* area = getenv("TMPDIR");
              character  via[CCLVI];

    CREDO_NECESSE_NON_NIHIL(area);

    CREDO_GRADUS("filum: linea prima, spatia finalia abscissa");
    sprintf(via, "%s/agens_plenum", area);
    CREDO_NECESSE(filum_scribere_literis(via,
        "rhubarb-navigator/0.1 (https://exemplum.org; proba)  \n"
        "linea secunda\n"));
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_agens_legere(via,
        c->piscina), "rhubarb-navigator/0.1 (https://exemplum.org; "
        "proba)");

    CREDO_GRADUS("filum vacuum -> ordinarius");
    sprintf(via, "%s/agens_vacuum", area);
    CREDO_NECESSE(filum_scribere_literis(via, " \n"));
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_agens_legere(via,
        c->piscina), NAVIGATOR_AGENS_ORDINARIUS);

    CREDO_GRADUS("filum absens -> ordinarius");
    sprintf(via, "%s/agens_nullum", area);
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_agens_legere(via,
        c->piscina), NAVIGATOR_AGENS_ORDINARIUS);
}

interior vacuum
_probare_servata (
    CredoContextus* c)
{
                      Mundus* m = (Mundus*)c->datum;
              ServusScriptus* s;
    NavigatorPetitioOptiones  opt;
                  Observatio  o;
                      chorda  petitio;

    CREDO_NECESSE(m->b && m->reactor);
    s = _servus(m->reactor, _responsum_ducenti(PAGINA("Rheum",
        "<p>Rheum est planta.</p>"), c->piscina), FALSUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    opt = _optiones(s, c->piscina);

    CREDO_GRADUS("prima: SERVATA, NOVUM; finis semel, non intra");
    CREDO_VERUM(_petere(m, "https://la.wikipedia.org/wiki/Rheum", &opt,
        &o, c->piscina));
    CREDO_AEQUALIS_I32((i32)o.resultus.status,
        (i32)NAVIGATOR_PETITIO_SERVATA);
    CREDO_AEQUALIS_I32((i32)o.resultus.exitus, (i32)NAVIGATOR_NOVUM);
    CREDO_NECESSE(o.resultus.articulus && o.resultus.versio);
    CREDO_CHORDA_AEQUALIS_LITERIS(o.resultus.articulus->url,
        "https://la.wikipedia.org/wiki/Rheum");
    CREDO_CHORDA_AEQUALIS_LITERIS(o.resultus.versio->titulus, "Rheum");

    CREDO_GRADUS("servus: via fontis, User-Agent, Accept Parsoid");
    petitio = chorda_aedificator_spectare(s->petitio);
    CREDO_VERUM(chorda_incipit(petitio, chorda_ex_literis(
        "GET /api/rest_v1/page/html/Rheum HTTP/1.1\r\n", c->piscina)));
    CREDO_VERUM(_continet(petitio, "User-Agent: " AGENS "\r\n",
        c->piscina));
    CREDO_VERUM(_continet(petitio, "profile=\"https://www.mediawiki.org"
        "/wiki/Specs/HTML/2.8.0\"", c->piscina));

    CREDO_GRADUS("iterum: SERVATA, IDEM");
    CREDO_VERUM(_petere(m, "https://la.wikipedia.org/wiki/Rheum", &opt,
        &o, c->piscina));
    CREDO_AEQUALIS_I32((i32)o.resultus.status,
        (i32)NAVIGATOR_PETITIO_SERVATA);
    CREDO_AEQUALIS_I32((i32)o.resultus.exitus, (i32)NAVIGATOR_IDEM);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(m->b), I);
    servum_scriptum_claudere(s);
    CREDO_VERUM(_reactor_vacuus(m->reactor));
}

/* "Aquinas" -> 302 -> ".../Thomas_Aquinas": articulus sub titulo
 * ultimo */
interior vacuum
_probare_redirectio (
    CredoContextus* c)
{
                      Mundus* m = (Mundus*)c->datum;
              ServusScriptus* a;
              ServusScriptus* b;
    NavigatorPetitioOptiones  opt;
                  Observatio  o;
                   character* locatio;

    CREDO_NECESSE(m->b && m->reactor);
    b = _servus(m->reactor, _responsum_ducenti(PAGINA("Thomas Aquinas",
        "<p>Thomas de Aquino.</p>"), c->piscina), FALSUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(b);
    locatio = (character*)piscina_allocare(c->piscina, CCLVI);
    /* Vicipaedia vera (2026-10-27): rest_v1 ad API MediaWiki novam
     * redirigit */
    sprintf(locatio, "HTTP/1.1 302 Found\r\nLocation: http://127.0.0.1:"
        "%u/w/rest.php/v1/page/Thomas_Aquinas/html?redirect=no\r\n"
        "Content-Length: 0\r\n\r\n", (insignatus integer)b->portus);
    a = _servus(m->reactor, locatio, FALSUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(a);
    opt = _optiones(a, c->piscina);

    CREDO_GRADUS("articulus sub URL canonica tituli ultimi");
    CREDO_VERUM(_petere(m, "https://en.wikipedia.org/wiki/Aquinas",
        &opt,
        &o, c->piscina));
    CREDO_AEQUALIS_I32((i32)o.resultus.status,
        (i32)NAVIGATOR_PETITIO_SERVATA);
    CREDO_NECESSE_NON_NIHIL(o.resultus.articulus);
    CREDO_CHORDA_AEQUALIS_LITERIS(o.resultus.articulus->url,
        "https://en.wikipedia.org/wiki/Thomas_Aquinas");
    CREDO_VERUM(navigator_articulum_invenire(m->b, chorda_ex_literis(
        "https://en.wikipedia.org/wiki/Aquinas", c->piscina)) == NIHIL);
    CREDO_AEQUALIS_I32(a->acceptae, I);
    CREDO_AEQUALIS_I32(b->acceptae, I);
    servum_scriptum_claudere(a);
    servum_scriptum_claudere(b);
    CREDO_VERUM(_reactor_vacuus(m->reactor));
}

interior vacuum
_probare_non_inventa (
    CredoContextus* c)
{
                      Mundus* m = (Mundus*)c->datum;
              ServusScriptus* s;
    NavigatorPetitioOptiones  opt;
                  Observatio  o;
                         s64  acta;

    CREDO_NECESSE(m->b && m->reactor);
    s = _servus(m->reactor,
        "HTTP/1.1 404 Not Found\r\nContent-Length: 4"
        "\r\n\r\nnihil", FALSUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    opt   = _optiones(s, c->piscina);
    acta  = volumen_summa_actorum(m->volumen);

    CREDO_GRADUS("404: NON_INVENTA, nihil scriptum");
    CREDO_VERUM(_petere(m, "https://la.wikipedia.org/wiki/Nusquam",
        &opt,
        &o, c->piscina));
    CREDO_AEQUALIS_I32((i32)o.resultus.status,
        (i32)NAVIGATOR_PETITIO_NON_INVENTA);
    CREDO_VERUM(o.resultus.articulus == NIHIL);
    CREDO_VERUM(volumen_summa_actorum(m->volumen) == acta);
    servum_scriptum_claudere(s);
    CREDO_VERUM(_reactor_vacuus(m->reactor));
}

interior vacuum
_probare_ignota (
    CredoContextus* c)
{
        Mundus* m = (Mundus*)c->datum;
    Observatio  o;

    CREDO_NECESSE(m->b && m->reactor);
    CREDO_GRADUS("situs non albus: IGNOTA, finis non intra");
    CREDO_VERUM(_petere(m, "https://example.com/wiki/Rheum", NIHIL, &o,
        c->piscina));
    CREDO_AEQUALIS_I32((i32)o.resultus.status,
        (i32)NAVIGATOR_PETITIO_IGNOTA);
    CREDO_VERUM(o.resultus.descriptio.mensura > ZEPHYRUM);
    CREDO_VERUM(_reactor_vacuus(m->reactor));
}

interior vacuum
_probare_rete (
    CredoContextus* c)
{
                      Mundus* m = (Mundus*)c->datum;
              ServusScriptus* s;
    NavigatorPetitioOptiones  opt;
                  Observatio  o;

    CREDO_NECESSE(m->b && m->reactor);
    s = _servus(m->reactor, NIHIL, VERUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    opt = _optiones(s, c->piscina);

    CREDO_GRADUS("servus claudit: RETE cum causa");
    CREDO_VERUM(_petere(m, "https://la.wikipedia.org/wiki/Rheum", &opt,
        &o, c->piscina));
    CREDO_AEQUALIS_I32((i32)o.resultus.status,
        (i32)NAVIGATOR_PETITIO_RETE);
    CREDO_VERUM(o.resultus.descriptio.mensura > ZEPHYRUM);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(m->b), ZEPHYRUM);
    servum_scriptum_claudere(s);
    CREDO_VERUM(_reactor_vacuus(m->reactor));
}

interior vacuum
_probare_abrumpere (
    CredoContextus* c)
{
                      Mundus* m = (Mundus*)c->datum;
              ServusScriptus* s;
    NavigatorPetitioOptiones  opt;
                  Observatio  o;
            NavigatorPetitio* np;
                         i32  k;

    CREDO_NECESSE(m->b && m->reactor);
    s = servum_scriptum_creare(m->reactor, NIHIL, ZEPHYRUM, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    opt = _optiones(s, c->piscina);
    memset(&o, ZEPHYRUM, magnitudo(o));
    np = navigator_petere(m->reactor, m->b, chorda_ex_literis(
        "https://la.wikipedia.org/wiki/Rheum", c->piscina), _finis, &o,
        &opt);
    CREDO_NECESSE_NON_NIHIL(np);
    per (k = ZEPHYRUM; k < V; k++)
    {
        (vacuum)reactor_poll(m->reactor, X);
    }

    CREDO_GRADUS("abrumpere: finis ABRUPTA ante reditum, semel");
    CREDO_AEQUALIS_I32(o.finis, ZEPHYRUM);
    navigator_petitionem_abrumpere(np);
    CREDO_AEQUALIS_I32(o.finis, I);
    CREDO_AEQUALIS_I32((i32)o.resultus.status,
        (i32)NAVIGATOR_PETITIO_ABRUPTA);
    navigator_petitionem_abrumpere(np);
    CREDO_AEQUALIS_I32(o.finis, I);
    navigator_petitionem_dimittere(np);
    servum_scriptum_claudere(s);
    CREDO_VERUM(_reactor_vacuus(m->reactor));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "agens", _probare_agens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "servata", _probare_servata, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "redirectio", _probare_redirectio, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "non inventa", _probare_non_inventa, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "ignota", _probare_ignota, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "rete", _probare_rete, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "abrumpere", _probare_abrumpere, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("navigator_petitio", SECTIONES);
}
