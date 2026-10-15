/* probatio_reactor_mutatio.c - reactor sub mutatione intra dispatchum
 * (http-vocatio-spec II.0): callback fossam ALIAM removet aut addit,
 * locus fossae in eodem transitu reusus, timer se ipsum reponit, timer
 * natus in transitu proximum exspectat, poll sine activis non dormit.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "reactor.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

nomen structura {
     Reactor* reactor;
         i32  vocatus;
     integer  removenda;          /* -1 = nulla */
     integer  addenda;            /* -1 = nulla */
      vacuum* datum_addendae;
} Nota;

nomen structura {
           Reactor* reactor;
    ReactorTimerId  id;
               i32  vocatus;
               i32  secundi;
} Horologium;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_notam_initiare (
       Nota* n,
    Reactor* r)
{
    n->reactor         = r;
    n->vocatus         = ZEPHYRUM;
    n->removenda       = -I;
    n->addenda         = -I;
    n->datum_addendae  = NIHIL;
}

/* octetum unum legit (ne fossa iterum legibilis sit), numerat, deinde
 * fossam alienam removet et/aut addit */
interior vacuum
_notare (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
         Nota* n = (Nota*)datum;
    character  octetus;

    (vacuum)eventus;
    (vacuum)read(fossa, &octetus, I);
    n->vocatus++;
    si (n->removenda >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(n->reactor, n->removenda);
        n->removenda = -I;
    }
    si (n->addenda >= ZEPHYRUM)
    {
        (vacuum)reactor_adicere(n->reactor, n->addenda, REACTOR_LEGERE,
            _notare, n->datum_addendae);
        n->addenda = -I;
    }
}

/* pipa NON blocans: eventus perperam dispatchatus (fossa sine datis)
 * read() EAGAIN reddit et numeratur - numquam suitam suspendit (planta
 * generationis olim suitam X minuta suspendebat) */
interior b32
_pipam (
    integer* pipa)
{
    redde pipe(pipa) == ZEPHYRUM
        && fcntl(pipa[ZEPHYRUM], F_SETFL, O_NONBLOCK) == ZEPHYRUM;
}

interior vacuum
_scribere (
    integer* pipa)
{
    (vacuum)write(pipa[I], "x", I);
}

interior vacuum
_claudere (
    integer* pipa)
{
    (vacuum)close(pipa[ZEPHYRUM]);
    (vacuum)close(pipa[I]);
}

/* A removet B (posteriorem): B non vocatur, D tamen vocatur (olim
 * reactor_removere poll_fds rebuilebat et revents D purgabat) */
interior vacuum
_sectio_aliena_remota (
    CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
     integer  a[II];
     integer  b[II];
     integer  d[II];
        Nota  na;
        Nota  nb;
        Nota  nd;

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(_pipam(a) && _pipam(b)
        && _pipam(d));
    _notam_initiare(&na, r);
    _notam_initiare(&nb, r);
    _notam_initiare(&nd, r);
    na.removenda = b[ZEPHYRUM];
    CREDO_NECESSE(reactor_adicere(r, a[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &na));
    CREDO_NECESSE(reactor_adicere(r, b[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &nb));
    CREDO_NECESSE(reactor_adicere(r, d[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &nd));
    _scribere(a);
    _scribere(b);
    _scribere(d);
    (vacuum)reactor_poll(r, C);
    CREDO_AEQUALIS_I32(na.vocatus, I);
    CREDO_AEQUALIS_I32(nb.vocatus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(nd.vocatus, I);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), II);
    _claudere(a);
    _claudere(b);
    _claudere(d);
}

/* B addit E in locum A liberum (inferiorem): D in hoc transitu vocatur,
 * E solum in proximo */
interior vacuum
_sectio_addita_inferior (
    CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
     integer  a[II];
     integer  b[II];
     integer  d[II];
     integer  e[II];
        Nota  na;
        Nota  nb;
        Nota  nd;
        Nota  ne;

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(_pipam(a) && _pipam(b)
        && _pipam(d) && _pipam(e));
    _notam_initiare(&na, r);
    _notam_initiare(&nb, r);
    _notam_initiare(&nd, r);
    _notam_initiare(&ne, r);
    CREDO_NECESSE(reactor_adicere(r, a[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &na));
    CREDO_NECESSE(reactor_adicere(r, b[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &nb));
    CREDO_NECESSE(reactor_adicere(r, d[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &nd));
    CREDO_NECESSE(reactor_removere(r, a[ZEPHYRUM]));
    nb.addenda         = e[ZEPHYRUM];
    nb.datum_addendae  = &ne;
    _scribere(b);
    _scribere(d);
    _scribere(e);
    (vacuum)reactor_poll(r, C);
    CREDO_AEQUALIS_I32(nb.vocatus, I);
    CREDO_AEQUALIS_I32(nd.vocatus, I);
    CREDO_AEQUALIS_I32(ne.vocatus, ZEPHYRUM);
    (vacuum)reactor_poll(r, C);
    CREDO_AEQUALIS_I32(ne.vocatus, I);
    _claudere(a);
    _claudere(b);
    _claudere(d);
    _claudere(e);
}

/* A removet D et addit E, quae locum D accipit: E revents D numquam
 * accipit (custodia generationis - transit etiam in codice vetere,
 * quia rebuild revents purgabat; planta generationis eam rubram facit)
 */
interior vacuum
_sectio_locus_iterum (
    CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
     integer  a[II];
     integer  b[II];
     integer  d[II];
     integer  e[II];
        Nota  na;
        Nota  nb;
        Nota  nd;
        Nota  ne;

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(_pipam(a) && _pipam(b)
        && _pipam(d) && _pipam(e));
    _notam_initiare(&na, r);
    _notam_initiare(&nb, r);
    _notam_initiare(&nd, r);
    _notam_initiare(&ne, r);
    na.removenda       = d[ZEPHYRUM];
    na.addenda         = e[ZEPHYRUM];
    na.datum_addendae  = &ne;
    CREDO_NECESSE(reactor_adicere(r, a[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &na));
    CREDO_NECESSE(reactor_adicere(r, b[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &nb));
    CREDO_NECESSE(reactor_adicere(r, d[ZEPHYRUM], REACTOR_LEGERE,
        _notare, &nd));
    _scribere(a);
    _scribere(d);
    (vacuum)reactor_poll(r, C);
    CREDO_AEQUALIS_I32(na.vocatus, I);
    CREDO_AEQUALIS_I32(nd.vocatus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(ne.vocatus, ZEPHYRUM);
    _scribere(e);
    (vacuum)reactor_poll(r, C);
    CREDO_AEQUALIS_I32(ne.vocatus, I);
    _claudere(a);
    _claudere(b);
    _claudere(d);
    _claudere(e);
}

interior vacuum
_secundum (
    vacuum* datum)
{
    ((Horologium*)datum)->secundi++;
}

interior vacuum
_reponere (
    vacuum* datum)
{
    Horologium* h = (Horologium*)datum;

    h->vocatus++;
    (vacuum)reactor_timer_cancellare(h->reactor, h->id);
    h->id = reactor_timer_adicere(h->reactor, X, FALSUM, _secundum, h);
}

/* timer se ipsum cancellat et novum addit: novus (eundem locum fortasse
 * accipiens) VERE incendit */
interior vacuum
_sectio_timer_reponit (
    CredoContextus* c)
{
       Reactor* r = reactor_creare(c->piscina);
    Horologium  h;
           s64  initium;

    CREDO_NECESSE_NON_NIHIL(r);
    h.reactor  = r;
    h.vocatus  = ZEPHYRUM;
    h.secundi  = ZEPHYRUM;
    h.id       = reactor_timer_adicere(r, X, FALSUM, _reponere, &h);
    initium    = _ms_nunc();
    dum (h.secundi == ZEPHYRUM && _ms_nunc() - initium < CCC)
    {
        (vacuum)reactor_poll(r, XX);
    }
    CREDO_AEQUALIS_I32(h.vocatus, I);
    CREDO_AEQUALIS_I32(h.secundi, I);
}

interior vacuum
_gignere (
    vacuum* datum)
{
    Horologium* h = (Horologium*)datum;

    h->vocatus++;
    (vacuum)reactor_timer_adicere(h->reactor, ZEPHYRUM, FALSUM,
        _secundum, h);
}

/* timer in transitu natus (0 ms) in proximo transitu incendit, non in
 * eodem (aliter callback qui semper addit ansam numquam finiret).
 * Dispositio: parens REPETENS in loco 0 (locum tenet), FORAMEN in
 * loco 1 (timer cancellatus), timer longus in loco 2 (summa aquae III).
 * Filius foramen accipit - supra parentem, infra summam quam ansa
 * visitat - solum limes eum servat. (Parens singularis locum suum ante
 * callback liberat et filius eum reusat; filius ultra summam aquae
 * numquam visitatur: neutrum limitem probat.) */
interior vacuum
_sectio_timer_natus (
    CredoContextus* c)
{
           Reactor* r = reactor_creare(c->piscina);
        Horologium  h;
    ReactorTimerId  foramen;

    CREDO_NECESSE_NON_NIHIL(r);
    h.reactor = r;
    h.vocatus = ZEPHYRUM;
    h.secundi = ZEPHYRUM;
    h.id = reactor_timer_adicere(r, ZEPHYRUM, VERUM, _gignere,
        &h);
    foramen = reactor_timer_adicere(r, X * M, FALSUM, _secundum, &h);
    (vacuum)reactor_timer_adicere(r, X * M, FALSUM, _secundum, &h);
    CREDO_NECESSE(reactor_timer_cancellare(r, foramen));
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_AEQUALIS_I32(h.vocatus, I);
    CREDO_AEQUALIS_I32(h.secundi, ZEPHYRUM);
    CREDO_NECESSE(reactor_timer_cancellare(r, h.id));
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_AEQUALIS_I32(h.secundi, I);
}

/* post timer incensum nihil activum: reactor_poll(r, D) statim redit
 * (olim timer_numerus, summa aquae, poll() sine fossis D ms dormiebat)
 */
interior vacuum
_sectio_otiosus (
    CredoContextus* c)
{
       Reactor* r = reactor_creare(c->piscina);
    Horologium  h;
           s64  initium;

    CREDO_NECESSE_NON_NIHIL(r);
    h.reactor = r;
    h.vocatus = ZEPHYRUM;
    h.secundi = ZEPHYRUM;
    (vacuum)reactor_timer_adicere(r, I, FALSUM, _secundum, &h);
    initium = _ms_nunc();
    dum (h.secundi == ZEPHYRUM && _ms_nunc() - initium < CC)
    {
        (vacuum)reactor_poll(r, X);
    }
    CREDO_NECESSE(h.secundi == I);
    initium = _ms_nunc();
    (vacuum)reactor_poll(r, D);
    CREDO_VERUM(_ms_nunc() - initium < C);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "aliena remota", _sectio_aliena_remota, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "addita inferior", _sectio_addita_inferior, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "locus reusus", _sectio_locus_iterum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "timer reponit", _sectio_timer_reponit, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "timer natus", _sectio_timer_natus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "otiosus", _sectio_otiosus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("reactor_mutatio", SECTIONES);
}
