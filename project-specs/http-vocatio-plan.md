# http-vocatio plan - non-blocking HTTP(S) client on the reactor (route B of $question)

> **For agentic workers:** execution mode is FIXED by Fran's standing
> rule (memory `plan-execution-inline-checkpoints`): inline, one task per
> turn, Fran approves each before the next; no subagents. GUI steps end
> with Fran's own look before the commit. Steps use checkbox (`- [ ]`)
> syntax for tracking.

**Goal:** `$question` in vicus streams through an in-process,
non-blocking HTTPS client driven by the host's reactor - no child
process, the window never stalls - while behaving exactly as route A
does today.

**Architecture:** five non-blocking layers, each using only the one
below: `reactor` (made safe under mutation) <- `nomina` (DNS via
`DNSServiceGetAddrInfo`) <- `tcp_incipere` (non-blocking connect) <-
stepped TLS (`tls_incipere` / `tls_gradus`) <- `http_vocatio` (the call
state machine, feeding the existing `http_lector`). On top:
`vates_vocare_rivum` (same contract as `vates_mittere_rivum`, retry on a
reactor timer) and the vicus port `vicus_quaestio_interna`. The host
(vicus) owns the one reactor and polls it every pulse.

**Tech stack:** C89 + latina.h; house `reactor`, `tcp`, `tls`
(SecureTransport), `http` (`http_lector`), `rivus_eventuum`, `vates`,
`quaestio`, `piscina`, `chorda_aedificator`; `<dns_sd.h>` (libSystem);
credo v2.

**Spec:** `project-specs/http-vocatio-spec.md` (quarta 3e28f6e7). The
spec is the authority; deviations are ledgered as rulings below.

## Global constraints

- Flags `tools/vexilla.sh` (C89, -Werror, -Wconversion,
  -Wsign-conversion, -Wcast-qual, -Wstrict-prototypes,
  -Wmissing-prototypes, -Wwrite-strings).
- Latin identifiers; NEVER a latina.h macro word (`nomen per casus
  duplex magnitudo registrum interior ordinarius sponte fac dum ...`) nor
  a single Roman numeral letter (`I V X L C D M`) as an identifier;
  every Roman numeral 0-3999 is a macro.
- `chorda` not NUL-terminated; `i32` UNSIGNED, `s32`/`s64` signed.
- POSIX `.c`: `postulata_posix.h` FIRST. Platform `.m` files: system
  headers BEFORE latina.h (`character` collides with CoreFoundation).
- The API key (route B, ledger decision ...AAQVQ3): read by the vicus
  port from `$HOME/.rhubarb/anthropic.clavis` (or a test path) into the
  CALL's own piscina; every copy (file buffer, `Vates->clavis`, request
  headers, serialized request) lives in that piscina, which is
  `piscina_vacare`d (zeroes every block, `lib/piscina.c:126-133`) and
  destroyed when the call ends. Never in a message, status, error text,
  annals or insula. Claude never reads or prints the real key file;
  tests use a fake key in `$TMPDIR`.
- `finis` callbacks never run inside the function that started the
  call (`nomina_quaerere_incipere`, `http_vocare`, `vates_vocare_rivum`)
  - early outcomes go through a 0 ms reactor timer. A `finis` callback
  must NOT destroy the call's piscina (code still runs after it
  returns); release on the next pulse.
- New suites: credo v2 (`CredoSectio` table, `credo_suitam_currere`).
- New `lib/*.c` / `lib/*.m` -> regenerate
  `compile_tests_fontes_generata.sh`
  (`./tools/compile_tests_fontes_generare.sh`) in the same commit.
- New POSIX names in examined `.c` files -> `silva/fontes/systema_posix.h`
  (Darwin values) + `officina/auspex_posix.sh` `IUNGO_ASSERTA` + plant +
  glossary tolerations (`oratio/glossarium.stml`, classis
  `ignotum-permissum`) + `silva/fontes/systema_posix.worklog.md`, the
  analytica T2 procedure (622f8951).
- Every behaviour born red; plants via `silva.planta()`; restore with
  plain cp + touch.
- Commit procedure (every task): `./silva/formator.sh -scribere <files>`
  then `-vitia` parity against `git show HEAD:` copies (fix only NEW
  findings); Latin lint `VOCABULA_VIAE_ADDITAE="<files>"
  ./oratio/vocabula.sh -nova`; then `silva.commissio` from a detached
  script (python double-fork + `os.setsid`, log in the scratchpad,
  watch the log for `RESULTUM` / `Traceback` with Monitor - never an
  rc file written before `wait()`), with the worktree guard (`os.chdir(Q);
  sys.path.insert(0, Q+'/pythonica'); assert
  silva.__file__.startswith(Q+'/')`). Freeze the tree while gates run;
  never `~/.bin` installs from quarta.
- Network tests live in separate files marked
  `/* <aedilis facultas="rete"/> */` (gates link them only,
  `lib/fabrica_probationes_c.c:949`); run them by hand.

## Rulings made while planning (spec deviations, ledgered here)

- **R1 the host polls, the port does not.** Spec II.6 has the port's
  `pulsare` call `reactor_poll(r, 0)`. With a shared reactor, the host
  pulse polls once (`apps/vicus/vicus.c` `vicum_pulsare`) and the port
  only reads its handle; tests poll in their own loop. Cost if wrong:
  one extra call in the port.
- **R2 the key is zeroed by vacating the call's piscina**, not buffer by
  buffer: `Vates` keeps its own copy (`lib/vates.c:1067`) and every
  retry rebuilds the header from it, so the earliest safe moment is the
  end of the call. The file buffer is still zeroed right after
  `vates_anthropic_aperire`, and `http_vocatio` zeroes its serialized
  request after sending. Cost if wrong: none known.
- **R3 nomina returns addresses with the port already set** (the `.m`
  file fills `sin_port`/`sin6_port`), so `tcp_posix.c` never needs
  `sockaddr_in6` (absent from the examen lexicon). `tcp_incipere` takes
  raw sockaddr bytes.
- **R4 resolution delay 50 ms (RFC 8305 §3).** `DNSServiceGetAddrInfo`
  answers each family separately; nomina finishes when both families
  have answered (positive or `NoSuchRecord`), or 50 ms after the first
  address arrives, whichever is first.
- **R5 two leaks fixed in place:** `tcp_claudere` skips `close()` after
  a peer close (`lib/tcp_posix.c:411`, `clausa` set by `tcp_recipere`
  at `:388-391`), and `tls_claudere` skips `SSLClose`/`CFRelease` in the
  same case (`lib/tls_macos.m:623`). Both in the tasks that touch those
  files, with tests. Cost if wrong: none - closing an fd we own is
  always right.
- **R6 extra API the spec did not name:** `tcp_ex_fossa` (wrap an
  existing fd - TLS tests over a socketpair); `HttpVocatioOptiones`
  (connect / idle limits, NIHIL = defaults) as an `http_vocare`
  argument; `VatesOptiones.url` (tests point vates at a local server);
  `quaestio_petitionem_parare` + `quaestio_responsum_ex_vate` (today's
  interior `petitionem_struere` / `responsum_reddere`, made public so
  the port shares them).
- **R7 on error `HttpResultus.responsum` is NIHIL**, as
  `http_exsequi_rivus` does today; streamed bytes already reached the
  sink. (Spec II.4 said "body so far kept"; the sink is where it is
  kept.)
- **R8 the connect phase has one deadline** (NOMINA + CONNECTIO +
  PACTIO together, not re-armed inside the phase); the idle limit is
  re-armed on every byte moved.
- **R9 vates abort also calls `finis` once** (responsum error
  `VATES_ERROR_RETE`, nuntius "abruptum"), matching `http_vocatio`'s
  "every path ends in exactly one finis".
- **R10 route B bypasses `HttpVecturaRivi`**, so herbarium capture and
  the fictus transport do not see route-B traffic; the port sets
  `sine_herbario`. Tests use the scripted local server instead.
- **R11 route A stays selectable as `-quaestio adiutor`**; route B is
  the default.

## Review focus

The failure modes most likely to bite that no spec line names; each has
its test in the owning task.
1. A server that answers fully and keeps the connection open
   (Content-Length or chunked, keep-alive) must finish at once - never
   wait for a close that never comes. (Task 5, section `manet apertum`.)
2. `abrumpere` called from INSIDE the body sink (Esc arrives while a
   delta is being handled) must end the call once, cleanly, with no
   further sink calls. (Task 5, `abrumpere intra rivum`.)
3. A server that accepts and closes before sending anything must give a
   named error, not a hang. (Task 5, `clausum statim`.)
4. Two calls on one reactor must not see each other's events. (Task 5,
   `duae simul`.)
5. `localhost` resolving to `::1` first while the server listens on
   127.0.0.1 only must fall back to the next address. (Task 5,
   `localhost`.)

---

### Task 1: reactor - dispatch safe under mutation (spec II.0)

**Files:**
- Modify: `lib/reactor.c` (structs `:27-63`, `_rebuilere_poll_fds`
  `:151-168`, `_processare_timers` `:261-297`, `reactor_adicere`
  `:355-415`, `reactor_modificare` `:417-440`, `reactor_removere`
  `:442-464`, `reactor_poll` `:553-626`)
- Create: `probationes/probatio_reactor_mutatio.c`
- Modify: `lib/reactor.worklog.md` (create if absent)

**Interfaces:**
- Consumes: nothing new.
- Produces: no API change. Guarantees later tasks rely on: a callback
  may add/remove ANY fd and add/cancel ANY timer (its own included)
  during dispatch; an fd added during a pass is first dispatched on the
  next pass; a timer added during a pass first fires on the next pass;
  `reactor_poll(r, ms)` with no active fd and no active timer returns
  at once.

- [ ] **Step 1: Write the failing test**

`probationes/probatio_reactor_mutatio.c`:

```c
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
    ReactorTimerId id;
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
     integer a[II];
     integer b[II];
     integer d[II];
        Nota na;
        Nota nb;
        Nota nd;

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(pipe(a) == ZEPHYRUM && pipe(b) == ZEPHYRUM
        && pipe(d) == ZEPHYRUM);
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
     integer a[II];
     integer b[II];
     integer d[II];
     integer e[II];
        Nota na;
        Nota nb;
        Nota nd;
        Nota ne;

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(pipe(a) == ZEPHYRUM && pipe(b) == ZEPHYRUM
        && pipe(d) == ZEPHYRUM && pipe(e) == ZEPHYRUM);
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
_sectio_locus_reusus (
    CredoContextus* c)
{
    Reactor* r = reactor_creare(c->piscina);
     integer a[II];
     integer b[II];
     integer d[II];
     integer e[II];
        Nota na;
        Nota nb;
        Nota nd;
        Nota ne;

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(pipe(a) == ZEPHYRUM && pipe(b) == ZEPHYRUM
        && pipe(d) == ZEPHYRUM && pipe(e) == ZEPHYRUM);
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
    Horologium h;
          s64 initium;

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
 * eodem (aliter callback qui semper addit ansam numquam finiret) */
interior vacuum
_sectio_timer_natus (
    CredoContextus* c)
{
     Reactor* r = reactor_creare(c->piscina);
    Horologium h;

    CREDO_NECESSE_NON_NIHIL(r);
    h.reactor  = r;
    h.vocatus  = ZEPHYRUM;
    h.secundi  = ZEPHYRUM;
    h.id       = reactor_timer_adicere(r, ZEPHYRUM, FALSUM, _gignere, &h);
    (vacuum)reactor_poll(r, ZEPHYRUM);
    CREDO_AEQUALIS_I32(h.vocatus, I);
    CREDO_AEQUALIS_I32(h.secundi, ZEPHYRUM);
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
    Horologium h;
          s64 initium;

    CREDO_NECESSE_NON_NIHIL(r);
    h.reactor  = r;
    h.vocatus  = ZEPHYRUM;
    h.secundi  = ZEPHYRUM;
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
    { "locus reusus", _sectio_locus_reusus, NIHIL, NIHIL, NIHIL, NIHIL },
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
```

(The CredoSectio initializer has six fields - titulus, probare, parare,
purgare, signa, exspectata_fracta - `include/credo.h:649-670`; check the
count against `probationes/probatio_vicus_quaestio_porta.c:229-237`.)

- [ ] **Step 2: Run it to verify it fails**

Run: `./compile_tests.sh probatio_reactor_mutatio > $S/t1_red.log 2>&1; grep -E 'FRACTA|Totalis|Fracti' $S/t1_red.log`
(`S` = the scratchpad.)
Expected: FRACTA in `aliena remota` (nd.vocatus 0), `addita inferior`
(nd.vocatus 0), `timer reponit` (secundi 0), `otiosus` (elapsed >= C);
`timer natus` most likely FRACTA (secundi 1 in pass one - same-ms
dependent); `locus reusus` TRANSIIT (see its comment).

- [ ] **Step 3: Implement**

In `lib/reactor.c`:

Entry and reactor fields:

```c
/* Entry pro FD in reactor */
nomen structura {
            integer  fd;
                i32  eventus;
    ReactorCallback  callback;
             vacuum* data;
                b32  activus;
                i32  generatio;   /* auget in omni inscriptione et
                                   * remotione: dispatchus registrationem
                                   * eandem agnoscit (http-vocatio II.0) */
} ReactorFdEntry;
```

and in `structura Reactor`, after `poll_fds`:

```c
    /* imago transitus: poll_fds[k] pertinet ad fd_entries[poll_index[k]]
     * registrationis poll_generatio[k] - callbacks tabulam mutare
     * possunt sine dispatcho turbato */
                 i32 poll_index[REACTOR_FD_MAXIMA];
                 i32 poll_generatio[REACTOR_FD_MAXIMA];
                 i32 poll_numerus;
```

`_rebuilere_poll_fds` records the snapshot:

```c
interior vacuum
_rebuilere_poll_fds (
    Reactor* reactor)
{
    i32 i;
    i32 poll_idx = 0;

    per (i = 0; i < reactor->fd_numerus && poll_idx < REACTOR_FD_MAXIMA; i++)
    {
        si (reactor->fd_entries[i].activus)
        {
            reactor->poll_fds[poll_idx].fd = (integer)reactor->fd_entries[i].fd;
            reactor->poll_fds[poll_idx].events = _eventus_ad_poll(reactor->fd_entries[i].eventus);
            reactor->poll_fds[poll_idx].revents = 0;
            reactor->poll_index[poll_idx]      = i;
            reactor->poll_generatio[poll_idx]  = reactor->fd_entries[i].generatio;
            poll_idx++;
        }
    }
    reactor->poll_numerus = poll_idx;
}
```

`reactor_adicere`: new registrations bump the generation; DELETE the two
`_rebuilere_poll_fds(reactor);` calls (`:379`, `:412`) - `reactor_poll`
rebuilds at the start of every pass, and a rebuild during dispatch
zeroed every later `revents`:

```c
    reactor->fd_entries[index].fd        = fd;
    reactor->fd_entries[index].eventus   = eventus;
    reactor->fd_entries[index].callback  = callback;
    reactor->fd_entries[index].data      = data;
    reactor->fd_entries[index].activus   = VERUM;
    reactor->fd_entries[index].generatio++;

    redde VERUM;
```

`reactor_modificare`: delete its `_rebuilere_poll_fds(reactor);` (`:437`).
`reactor_removere`:

```c
    reactor->fd_entries[index].activus = FALSUM;
    reactor->fd_entries[index].generatio++;

    redde VERUM;
```

`_processare_timers`: one-shot off BEFORE the callback, repeating
deadline set before it, timers born in this pass skipped:

```c
interior i32
_processare_timers (
    Reactor* reactor)
{
                 i64 nunc;
                 i32 i;
                 i32 numerus;
                 i32 processati = 0;
      ReactorTimerId limes;

    nunc     = _obtinere_tempus_ms();
    /* timers in hoc transitu nati (id >= limes) proximum exspectant */
    limes    = reactor->proximus_timer_id;
    numerus  = reactor->timer_numerus;

    per (i = 0; i < numerus; i++)
    {
        ReactorTimerEntry*   t = &reactor->timers[i];
        ReactorTimerCallback functio;
                     vacuum* datum;

        si (   !t->activus || t->proximus_ms > nunc || t->id >= limes)
        {
            perge;
        }
        functio  = t->callback;
        datum    = t->data;
        /* status ANTE callback: callback se ipsum cancellare et
         * timerem novum (fortasse in hoc loco) addere potest */
        si (t->repetere)
        {
            t->proximus_ms = nunc + (i64)t->interval_ms;
        }
        alioquin
        {
            t->activus = FALSUM;
        }
        si (functio)
        {
            functio(datum);
        }
        processati++;
    }

    redde processati;
}
```

`reactor_poll`: early return counts ACTIVE timers; dispatch walks the
snapshot and skips registrations that changed:

```c
        si (active_count == 0 && reactor_numerus_timer(reactor) == 0)
        {
            redde 0;
        }

        /* Vocare poll() */
        poll_result = poll(reactor->poll_fds, (nfds_t)reactor->poll_numerus, (integer)timeout);
    }

    /* Processare FD events (per imaginem; vide _rebuilere_poll_fds) */
    si (poll_result > 0)
    {
        i32 numerus = reactor->poll_numerus;

        per (poll_idx = 0; poll_idx < numerus; poll_idx++)
        {
            ReactorFdEntry* e;

            si (reactor->poll_fds[poll_idx].revents == 0)
            {
                perge;
            }
            e = &reactor->fd_entries[reactor->poll_index[poll_idx]];
            /* remota aut locus reusus intra hunc transitum */
            si (   !e->activus
                || e->generatio != reactor->poll_generatio[poll_idx])
            {
                perge;
            }
            si (e->callback)
            {
                e->callback(e->fd, _poll_ad_eventus(
                    reactor->poll_fds[poll_idx].revents), e->data);
            }
            events_processati++;
        }
    }
```

(`i` stays declared - the active-count loop above still uses it.)

- [ ] **Step 4: Run it to verify it passes, then the old suite**

Run: `./compile_tests.sh probatio_reactor_mutatio > $S/t1_green.log 2>&1; grep -E 'FRACTA|Totalis|Fracti' $S/t1_green.log; ./compile_tests.sh probatio_reactor > $S/t1_old.log 2>&1; echo rc=$?; ./compile_tests.sh hospitium > $S/t1_hosp.log 2>&1; echo rc=$?`
Expected: 0 fracti; old reactor suite rc=0 (102 assertions incl.
`callback_reentrans`); hospitium rc=0.

- [ ] **Step 5: Plants** (`silva.planta('lib/reactor.c', vetus, novus, 'radix', filtrum='probatio_reactor_mutatio', tolerans='spatia')`)
  - `|| e->generatio != reactor->poll_generatio[poll_idx])` -> `)` -
    expect `locus reusus` red.
  - `            t->activus = FALSUM;\n        }\n        si (functio)`
    -> move: replace `t->activus = FALSUM;` (inside the `alioquin`)
    with `;` and the call `functio(datum);` with
    `functio(datum); si (!t->repetere) t->activus = FALSUM;` -
    expect `timer reponit` red.
  - `|| t->id >= limes)` -> `)` - expect `timer natus` red.
  - `reactor_numerus_timer(reactor) == 0` -> `reactor->timer_numerus == 0`
    - expect `otiosus` red.

- [ ] **Step 6: Worklog + commit**

Append to `lib/reactor.worklog.md` (dated 2026-10-1x): the three hazards
(rebuild during dispatch zeroed later revents; one-shot deactivated
after its callback; high-water early return), the snapshot+generation
fix, timers born in a pass wait one pass, and "a callback may now
mutate the table freely". Regenerate nothing (no new lib file). Commit
`lib/reactor.c lib/reactor.worklog.md probationes/probatio_reactor_mutatio.c`
per the commit procedure; message: `reactor: dispatchus sub mutatione
tutus (http-vocatio II.0) - ...`.

---

### Task 2: `nomina` - non-blocking name resolution (spec II.1)

**Files:**
- Create: `include/nomina.h`, `lib/nomina_macos.m`,
  `lib/nomina_macos.worklog.md`
- Create: `probationes/probatio_nomina.c`
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: Task 1's guarantee (callbacks mutate the reactor freely).
- Produces:

```c
#define NOMINA_ADDRESSUS_MAXIMI  XVI
#define NOMINA_ADDRESSUS_OCTETI  CXXVIII   /* sockaddr_storage */
#define NOMINA_MORA_RESOLUTIONIS_MS  L     /* RFC 8305 §3 (R4) */

nomen structura {
     i8 octeti[NOMINA_ADDRESSUS_OCTETI];   /* sockaddr crudum, PORTU
                                            * POSITO (R3) */
    i32 mensura;                           /* sizeof sockaddr_in(6) */
    s32 familia;                           /* AF_INET aut AF_INET6 */
} NominaAddressus;

nomen structura NominaQuaestio NominaQuaestio;

/* semel; numquam intra nomina_quaerere_incipere; error vacua =
 * successus (numerus >= I), aliter numerus ZEPHYRUM */
nomen vacuum (*NominaFinis)(vacuum* datum,
    constans NominaAddressus* addressus, i32 numerus, chorda error);

NominaQuaestio* nomina_quaerere_incipere(Reactor* reactor,
    chorda hospes, i32 portus, NominaFinis finis, vacuum* datum,
    Piscina* piscina, chorda* error);   /* NIHIL + *error si nequit */
vacuum nomina_quaerere_abrumpere(NominaQuaestio* quaestio);
    /* finis numquam postea; nihil in reactore relinquitur; post finem
     * aut iterum: nihil agit */
```

- [ ] **Step 1: Write the failing test**

`probationes/probatio_nomina.c`:

```c
/* probatio_nomina.c - resolutio nominum non-obstruens (http-vocatio
 * II.1): localhost sine rete, IP litteralis sine quaestione, finis
 * numquam intra incipere, abrumpere tacet, nihil in reactore post
 * finem. Portus in addressu ipso (R3): octeti II-III big-endian in
 * sockaddr_in et sockaddr_in6 (BSD: sa_len, sa_family, portus). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "nomina.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>

#define PORTUS_PROBATIONIS (VIII * M + LXXX)   /* 8080 */

nomen structura {
                i32 vocatus;
                i32 numerus;
             chorda error;
    NominaAddressus primus;
                b32 quattuor;       /* AF_INET inter responsa */
                i32 portus_primi;
} Exitus;

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
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    Exitus* x = (Exitus*)datum;
       i32  k;

    x->vocatus++;
    x->numerus  = numerus;
    x->error    = error;
    si (numerus > ZEPHYRUM)
    {
        x->primus        = addressus[ZEPHYRUM];
        x->portus_primi  = (i32)((i32)(insignatus character)
            addressus[ZEPHYRUM].octeti[II] * CCLVI
            + (i32)(insignatus character)addressus[ZEPHYRUM].octeti[III]);
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (addressus[k].familia == AF_INET)
        {
            x->quattuor = VERUM;
        }
    }
}

interior vacuum
_exspectare (
    Reactor* r,
     Exitus* x,
        s64  tectum_ms)
{
    s64 initium = _ms_nunc();

    dum (x->vocatus == ZEPHYRUM && _ms_nunc() - initium < tectum_ms)
    {
        (vacuum)reactor_poll(r, L);
    }
}

interior vacuum
_sectio_localhost (
    CredoContextus* c)
{
           Reactor* r = reactor_creare(c->piscina);
    NominaQuaestio* q;
             chorda error;
             Exitus x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    q = nomina_quaerere_incipere(r, chorda_ex_literis("localhost",
        c->piscina), PORTUS_PROBATIONIS, _finis, &x, c->piscina, &error);
    CREDO_NECESSE_NON_NIHIL(q);
    CREDO_AEQUALIS_I32(x.vocatus, ZEPHYRUM);    /* numquam intra incipere */
    _exspectare(r, &x, III * M);
    CREDO_AEQUALIS_I32(x.vocatus, I);
    CREDO_AEQUALIS_I32(x.error.mensura, ZEPHYRUM);
    CREDO_VERUM(x.numerus >= I);
    CREDO_VERUM(x.quattuor);
    CREDO_AEQUALIS_I32(x.portus_primi, PORTUS_PROBATIONIS);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}
```

The remaining sections:

```c
/* IP litteralis: sine DNS, finis post transitum primum, non ante */
interior vacuum
_sectio_litteralis (
    CredoContextus* c)
{
           Reactor* r = reactor_creare(c->piscina);
    NominaQuaestio* q;
             chorda error;
             Exitus x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    q = nomina_quaerere_incipere(r, chorda_ex_literis("127.0.0.1",
        c->piscina), PORTUS_PROBATIONIS, _finis, &x, c->piscina,
        &error);
    CREDO_NECESSE_NON_NIHIL(q);
    CREDO_AEQUALIS_I32(x.vocatus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);  /* nulla DNS */
    (vacuum)reactor_poll(r, C);
    CREDO_AEQUALIS_I32(x.vocatus, I);
    CREDO_AEQUALIS_I32(x.numerus, I);
    CREDO_AEQUALIS_I32((i32)x.primus.familia, (i32)AF_INET);
    CREDO_AEQUALIS_I32(x.portus_primi, PORTUS_PROBATIONIS);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
}

/* abrumpere statim: finis numquam, nihil relictum; iterum: nihil */
interior vacuum
_sectio_abruptum (
    CredoContextus* c)
{
           Reactor* r = reactor_creare(c->piscina);
    NominaQuaestio* q;
             chorda error;
             Exitus x;
                s64 initium;

    memset(&x, ZEPHYRUM, magnitudo(x));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    q = nomina_quaerere_incipere(r, chorda_ex_literis("localhost",
        c->piscina), PORTUS_PROBATIONIS, _finis, &x, c->piscina,
        &error);
    CREDO_NECESSE_NON_NIHIL(q);
    nomina_quaerere_abrumpere(q);
    nomina_quaerere_abrumpere(q);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_timer(r), ZEPHYRUM);
    initium = _ms_nunc();
    dum (_ms_nunc() - initium < CC)
    {
        (vacuum)reactor_poll(r, L);
    }
    CREDO_AEQUALIS_I32(x.vocatus, ZEPHYRUM);
}

/* hospes vacuus: NIHIL + error nominatus, nihil in reactore */
interior vacuum
_sectio_vacuus (
    CredoContextus* c)
{
           Reactor* r = reactor_creare(c->piscina);
             chorda error;
             Exitus x;

    memset(&x, ZEPHYRUM, magnitudo(x));
    error.datum    = NIHIL;
    error.mensura  = ZEPHYRUM;
    CREDO_VERUM(nomina_quaerere_incipere(r, chorda_ex_literis("",
        c->piscina), PORTUS_PROBATIONIS, _finis, &x, c->piscina,
        &error) == NIHIL);
    CREDO_VERUM(error.mensura > ZEPHYRUM);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "localhost", _sectio_localhost, NIHIL, NIHIL, NIHIL, NIHIL },
    { "litteralis", _sectio_litteralis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abruptum", _sectio_abruptum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vacuus", _sectio_vacuus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("nomina", SECTIONES);
}
```

(The port is read from the `octeti` bytes without `ntohs`, which keeps
the test free of lexicon additions.)

- [ ] **Step 2: Run it to verify it fails**

Run: `./compile_tests.sh probatio_nomina > $S/t2_red.log 2>&1; tail -5 $S/t2_red.log`
Expected: build failure - `nomina.h` not found.

- [ ] **Step 3: Implement**

`include/nomina.h` - the Interfaces block above, wrapped:

```c
/* nomina.h - resolutio nominum NON-OBSTRUENS super reactorem
 * (http-vocatio-spec II.1, decretum ...CYT9A8)
 *
 * Implementatio: nomina_macos.m (DNSServiceGetAddrInfo ex <dns_sd.h>:
 * per resolutorem systematis - /etc/hosts, DNS per interfaciem, VPN,
 * /etc/resolver/*, mDNS). Fossa servitii in reactorem vocantis intrat;
 * responsa per familiam (IPv4, IPv6) colliguntur; finis cum ambae
 * responderunt aut L ms post addressum primum (RFC 8305 §3). IP
 * litteralis sine quaestione. finis SEMEL, numquam intra incipere
 * (timer 0 ms). */
#ifndef NOMINA_H
#define NOMINA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"

/* ... defines, NominaAddressus, NominaQuaestio, NominaFinis and the two
 * prototypes exactly as in the Interfaces block, each prototype in the
 * house layout (return type on its own line, aligned parameters) ... */

#endif /* NOMINA_H */
```

`lib/nomina_macos.m`:

```c
/* nomina_macos.m - resolutio nominum per DNSServiceGetAddrInfo (vide
 * caput). Capita systematis ANTE latina.h (ut tls_macos.m). */
#include <dns_sd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "nomina.h"

structura NominaQuaestio {
            Reactor* reactor;
       DNSServiceRef  servitium;     /* NULL: litteralis */
             integer  fossa;         /* -1: nulla in reactore */
         NominaFinis  finis;
              vacuum* datum;
             Piscina* piscina;
                 i32  portus;
     NominaAddressus  addressus[NOMINA_ADDRESSUS_MAXIMI];
                 i32  numerus;
                 b32  quattuor_responsum;  /* A: positivum aut NoSuchRecord */
                 b32  sex_responsum;       /* AAAA */
      ReactorTimerId  mora;
                 b32  mora_posita;
                 b32  finita;
              chorda  error;
};

interior vacuum
_moram_tollere (
    NominaQuaestio* q)
{
    si (q->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(q->reactor, q->mora);
        q->mora_posita = FALSUM;
    }
}

/* servitium et fossa dimittuntur (numquam intra callback servitii:
 * semper ex timer aut abrumpere) */
interior vacuum
_dimittere (
    NominaQuaestio* q)
{
    _moram_tollere(q);
    si (q->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(q->reactor, q->fossa);
        q->fossa = -I;
    }
    si (q->servitium != NULL)
    {
        DNSServiceRefDeallocate(q->servitium);
        q->servitium = NULL;
    }
}

interior vacuum
_finire (
    vacuum* datum)
{
    NominaQuaestio* q = (NominaQuaestio*)datum;

    q->mora_posita = FALSUM;   /* incensus */
    si (q->finita)
    {
        redde;
    }
    q->finita = VERUM;
    _dimittere(q);
    si (q->numerus == ZEPHYRUM && q->error.mensura == ZEPHYRUM)
    {
        q->error = chorda_ex_literis("nomen ignotum", q->piscina);
    }
    q->finis(q->datum, q->addressus, q->error.mensura > ZEPHYRUM
        ? ZEPHYRUM : q->numerus, q->error);
}

interior vacuum
_finire_post (
    NominaQuaestio* q,
               i32  ms)
{
    _moram_tollere(q);
    q->mora         = reactor_timer_adicere(q->reactor, ms, FALSUM,
        _finire, q);
    q->mora_posita  = (q->mora != REACTOR_TIMER_ERROR);
}

interior vacuum
_addere (
                NominaQuaestio* q,
    constans structura sockaddr* sa)
{
    NominaAddressus* a;
             size_t  n;

    si (q->numerus >= NOMINA_ADDRESSUS_MAXIMI)
    {
        redde;
    }
    a = &q->addressus[q->numerus];
    memset(a, 0, sizeof(*a));
    si (sa->sa_family == AF_INET)
    {
        structura sockaddr_in quattuor;

        n = sizeof(quattuor);
        memcpy(&quattuor, sa, n);
        quattuor.sin_port = htons((uint16_t)q->portus);
        memcpy(a->octeti, &quattuor, n);
    }
    alioquin si (sa->sa_family == AF_INET6)
    {
        structura sockaddr_in6 sex;

        n = sizeof(sex);
        memcpy(&sex, sa, n);
        sex.sin6_port = htons((uint16_t)q->portus);
        memcpy(a->octeti, &sex, n);
    }
    alioquin
    {
        redde;
    }
    a->mensura  = (i32)n;
    a->familia  = (s32)sa->sa_family;
    q->numerus++;
}

interior vacuum DNSSD_API
_responsum (
             DNSServiceRef  servitium,
           DNSServiceFlags  vexilla,
                  uint32_t  interfacies,
       DNSServiceErrorType  erratum,
       constans character*  hospes,
    constans structura sockaddr* sa,
                  uint32_t  ttl,
                    vacuum* contextus)
{
    NominaQuaestio* q = (NominaQuaestio*)contextus;

    (vacuum)servitium;
    (vacuum)interfacies;
    (vacuum)hospes;
    (vacuum)ttl;
    si (q->finita)
    {
        redde;
    }
    si (erratum == kDNSServiceErr_NoError && sa != NULL
        && (vexilla & kDNSServiceFlagsAdd))
    {
        _addere(q, sa);
        si (sa->sa_family == AF_INET)  { q->quattuor_responsum = VERUM; }
        si (sa->sa_family == AF_INET6) { q->sex_responsum = VERUM; }
    }
    alioquin si (erratum == kDNSServiceErr_NoSuchRecord && sa != NULL)
    {
        si (sa->sa_family == AF_INET)  { q->quattuor_responsum = VERUM; }
        si (sa->sa_family == AF_INET6) { q->sex_responsum = VERUM; }
    }
    alioquin si (erratum != kDNSServiceErr_NoError)
    {
        character nuntius[LXIV];

        sprintf(nuntius, "DNS erratum %d", (integer)erratum);
        q->error               = chorda_ex_literis(nuntius, q->piscina);
        q->quattuor_responsum  = VERUM;
        q->sex_responsum       = VERUM;
    }
    si (vexilla & kDNSServiceFlagsMoreComing)
    {
        redde;
    }
    si (q->quattuor_responsum && q->sex_responsum)
    {
        _finire_post(q, ZEPHYRUM);
    }
    alioquin si (q->numerus > ZEPHYRUM && !q->mora_posita)
    {
        _finire_post(q, NOMINA_MORA_RESOLUTIONIS_MS);
    }
}

interior vacuum
_legibilis (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    NominaQuaestio* q = (NominaQuaestio*)datum;
    DNSServiceErrorType erratum;

    (vacuum)fossa;
    (vacuum)eventus;
    erratum = DNSServiceProcessResult(q->servitium);
    si (erratum != kDNSServiceErr_NoError && !q->finita)
    {
        q->error = chorda_ex_literis("DNSServiceProcessResult fallita",
            q->piscina);
        _finire_post(q, ZEPHYRUM);
    }
}

NominaQuaestio*
nomina_quaerere_incipere (
        Reactor* reactor,
         chorda  hospes,
            i32  portus,
    NominaFinis  finis,
         vacuum* datum,
        Piscina* piscina,
         chorda* error)
{
     NominaQuaestio* q;
         character* hospes_c;
    structura in_addr  quattuor;
    structura in6_addr sex;
     DNSServiceErrorType erratum;

    si (!reactor || !finis || !piscina || hospes.mensura <= ZEPHYRUM)
    {
        si (error && piscina)
        {
            *error = chorda_ex_literis("hospes vacuus", piscina);
        }
        redde NIHIL;
    }
    q = (NominaQuaestio*)piscina_allocare(piscina, sizeof(NominaQuaestio));
    memset(q, 0, sizeof(NominaQuaestio));
    q->reactor  = reactor;
    q->fossa    = -I;
    q->finis    = finis;
    q->datum    = datum;
    q->piscina  = piscina;
    q->portus   = portus;
    hospes_c    = chorda_ut_cstr(hospes, piscina);

    /* IP litteralis: addressus unus, finis per timer 0 ms */
    si (inet_pton(AF_INET, hospes_c, &quattuor) == I)
    {
        structura sockaddr_in sa;

        memset(&sa, 0, sizeof(sa));
        sa.sin_len     = (uint8_t)sizeof(sa);
        sa.sin_family  = AF_INET;
        sa.sin_addr    = quattuor;
        _addere(q, (constans structura sockaddr*)(constans vacuum*)&sa);
        _finire_post(q, ZEPHYRUM);
        redde q;
    }
    si (inet_pton(AF_INET6, hospes_c, &sex) == I)
    {
        structura sockaddr_in6 sa;

        memset(&sa, 0, sizeof(sa));
        sa.sin6_len     = (uint8_t)sizeof(sa);
        sa.sin6_family  = AF_INET6;
        sa.sin6_addr    = sex;
        _addere(q, (constans structura sockaddr*)(constans vacuum*)&sa);
        _finire_post(q, ZEPHYRUM);
        redde q;
    }

    erratum = DNSServiceGetAddrInfo(&q->servitium, 0, 0,
        kDNSServiceProtocol_IPv4 | kDNSServiceProtocol_IPv6, hospes_c,
        _responsum, q);
    si (erratum != kDNSServiceErr_NoError || q->servitium == NULL)
    {
        si (error)
        {
            *error = chorda_ex_literis("DNSServiceGetAddrInfo fallita",
                piscina);
        }
        redde NIHIL;
    }
    q->fossa = DNSServiceRefSockFD(q->servitium);
    si (q->fossa < ZEPHYRUM || !reactor_adicere(reactor, q->fossa,
            REACTOR_LEGERE, _legibilis, q))
    {
        q->fossa = -I;
        DNSServiceRefDeallocate(q->servitium);
        si (error)
        {
            *error = chorda_ex_literis("fossa DNS in reactorem non intrat",
                piscina);
        }
        redde NIHIL;
    }
    redde q;
}

vacuum
nomina_quaerere_abrumpere (
    NominaQuaestio* q)
{
    si (!q || q->finita)
    {
        redde;
    }
    q->finita = VERUM;
    _dimittere(q);
}
```

(Platform `.m` files may use `sizeof`, `NULL`, `uint16_t` from the
system headers - `tls_macos.m` does. If the formatter or lint objects
to `sizeof`, use `magnitudo`, which latina.h defines identically.
`sprintf` needs `<stdio.h>` - C89 has no `snprintf`; the buffer of LXIV
holds any `%d`.)

Then: `./tools/compile_tests_fontes_generare.sh` (adds
`lib/nomina_macos.m` to `OBJC_SOURCES`).

- [ ] **Step 4: Run it to verify it passes**

Run: `./compile_tests.sh probatio_nomina > $S/t2_green.log 2>&1; grep -E 'FRACTA|Totalis|Fracti|error' $S/t2_green.log`
Expected: 0 fracti. If `localhost` times out with the network off,
that is AUDIENDA item 2 of the spec turning false - stop and report.

- [ ] **Step 5: Plants** (`filtrum='probatio_nomina'`, via
  `lib/nomina_macos.m`)
  - in the literal branch `_finire_post(q, ZEPHYRUM);\n        redde q;`
    (first occurrence) -> `_finire(q);\n        redde q;` - expect
    `litteralis` red (finis inside incipere).
  - `quattuor.sin_port = htons((uint16_t)q->portus);` -> `;` - expect
    `localhost` or `litteralis` red (port 0).
  - in `nomina_quaerere_abrumpere`, `_dimittere(q);` -> `;` - expect
    `abruptum` red (fd left in reactor).

- [ ] **Step 6: Worklog + commit**

`lib/nomina_macos.worklog.md`: the per-family answer rule and the 50 ms
delay, "never finish inside the DNS-SD callback" (deallocation inside
its own reply callback is avoided by finishing from a timer), literal
IPs, port set here (R3). Commit `include/nomina.h lib/nomina_macos.m
lib/nomina_macos.worklog.md probationes/probatio_nomina.c
compile_tests_fontes_generata.sh`.

---

### Task 3: TCP - non-blocking connect, `tcp_ex_fossa`, fd leak (spec II.2, R5)

**Files:**
- Modify: `include/tcp.h` (after `tcp_connectere_cum_optionibus`),
  `lib/tcp_posix.c` (new functions after `tcp_connectere_cum_optionibus`
  `:151-280`; `tcp_claudere` `:403-417`)
- Modify: `silva/fontes/systema_posix.h` (errno and socket sections,
  `:276-298`), `officina/auspex_posix.sh` (near `:98` and `:188`),
  `oratio/glossarium.stml`, `silva/fontes/systema_posix.worklog.md`
- Create: `probationes/probatio_tcp_incipere.c`
- Modify: `lib/tcp_posix.worklog.md` (create if absent)

**Interfaces:**
- Consumes: `nomina_quaerere_incipere` + `NominaAddressus` (Task 2) in
  the test only.
- Produces:

```c
/* http-vocatio II.2: socket O_NONBLOCK + connect ad addressum crudum
 * (sockaddr, PORTU POSITO - nomina, R3). successus: connexio PENDENS;
 * vocans fossam (tcp_obtinere_fd) ad SCRIBENDUM exspectat, deinde
 * tcp_incipere_perficere. Nullum SO_RCVTIMEO/SO_SNDTIMEO: tempora
 * vocantis (timers reactoris) sunt. */
TcpResultus tcp_incipere(constans i8* addressus, i32 mensura,
    Piscina* piscina);
/* post scribibilitatem SOLUM: TCP_OK = connexa; aliter
 * TCP_ERROR_CONNEXIO et *descriptio (strerror SO_ERROR) */
TcpError tcp_incipere_perficere(TcpConnexio* connexio,
    chorda* descriptio, Piscina* piscina);
/* fossam exsistentem involvit (probationes, fossae acceptae aliunde);
 * fossa a connexione possidetur - tcp_claudere eam claudit */
TcpConnexio* tcp_ex_fossa(integer fossa, Piscina* piscina);
```

and the behaviour change: `tcp_claudere` closes the fd even after the
peer closed (`clausa` from `tcp_recipere`), and is idempotent.

- [ ] **Step 1: Write the failing test**

`probationes/probatio_tcp_incipere.c`:

```c
/* probatio_tcp_incipere.c - connexio TCP non-blocans (http-vocatio
 * II.2): servus in 127.0.0.1 (portus ephemerus), addressus per nomina,
 * connexio pendens -> scribibilis -> perficere; portus clausus ->
 * error nominatus; fossa post clausuram paris VERE clauditur (R5:
 * olim tcp_claudere close() omittebat); tcp_ex_fossa. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "nomina.h"
#include "tcp.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

nomen structura {
                i32 vocatus;
                i32 numerus;
    NominaAddressus addressus;
} Responsum;

nomen structura {
    i32 scribibilis;
} Signum;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_nomina_finis (
                     vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    Responsum* r = (Responsum*)datum;

    (vacuum)error;
    r->vocatus++;
    r->numerus = numerus;
    si (numerus > ZEPHYRUM)
    {
        r->addressus = addressus[ZEPHYRUM];
    }
}

/* 127.0.0.1:portus per nomina (IP litteralis: sine DNS) */
interior b32
_addressum (
    Reactor* r,
        i32  portus,
    NominaAddressus* exitus,
    Piscina* p)
{
    Responsum x;
       chorda error;
          s64 initium;

    memset(&x, ZEPHYRUM, magnitudo(x));
    si (!nomina_quaerere_incipere(r, chorda_ex_literis("127.0.0.1", p),
            portus, _nomina_finis, &x, p, &error))
    {
        redde FALSUM;
    }
    initium = _ms_nunc();
    dum (x.vocatus == ZEPHYRUM && _ms_nunc() - initium < M)
    {
        (vacuum)reactor_poll(r, L);
    }
    *exitus = x.addressus;
    redde x.numerus == I;
}

interior vacuum
_scribibilis (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    (vacuum)fossa;
    (vacuum)eventus;
    ((Signum*)datum)->scribibilis++;
}

interior b32
_exspectare_scribibilem (
        Reactor* r,
    TcpConnexio* conn)
{
    Signum s;
       s64 initium = _ms_nunc();

    s.scribibilis = ZEPHYRUM;
    (vacuum)reactor_adicere(r, (integer)tcp_obtinere_fd(conn),
        REACTOR_SCRIBERE, _scribibilis, &s);
    dum (s.scribibilis == ZEPHYRUM && _ms_nunc() - initium < II * M)
    {
        (vacuum)reactor_poll(r, L);
    }
    (vacuum)reactor_removere(r, (integer)tcp_obtinere_fd(conn));
    redde s.scribibilis > ZEPHYRUM;
}

interior TcpServus*
_servum_creare (
    Piscina* p)
{
    TcpServusOptiones o = tcp_servus_optiones_default();
    TcpServusResultus sr = tcp_servus_creare_cum_optionibus("127.0.0.1",
        ZEPHYRUM, &o, p);

    si (!sr.successus || tcp_servus_auscultare(sr.servus, VIII)
            != TCP_OK)
    {
        redde NIHIL;
    }
    redde sr.servus;
}

interior vacuum
_sectio_connexa (
    CredoContextus* c)
{
            Reactor* r = reactor_creare(c->piscina);
          TcpServus* servus = _servum_creare(c->piscina);
    NominaAddressus  a;
        TcpResultus  res;
        TcpResultus  acc;
             chorda  descr;
                s64  initium;

    CREDO_NECESSE_NON_NIHIL(servus);
    CREDO_NECESSE(_addressum(r, tcp_servus_obtinere_portum(servus), &a,
        c->piscina));
    initium = _ms_nunc();
    res = tcp_incipere(a.octeti, a.mensura, c->piscina);
    CREDO_VERUM(_ms_nunc() - initium < C);     /* non obstat */
    CREDO_NECESSE(res.successus);
    CREDO_NECESSE(_exspectare_scribibilem(r, res.connexio));
    descr.datum    = NIHIL;
    descr.mensura  = ZEPHYRUM;
    CREDO_AEQUALIS_I32((i32)tcp_incipere_perficere(res.connexio, &descr,
        c->piscina), (i32)TCP_OK);
    acc = tcp_servus_accipere(servus, c->piscina);
    CREDO_VERUM(acc.successus);
    tcp_claudere(acc.connexio);
    tcp_claudere(res.connexio);
    tcp_servus_claudere(servus);
    CREDO_AEQUALIS_I32(reactor_numerus_fd(r), ZEPHYRUM);
}

/* portus sine auscultatore: perficere errorem nominat */
interior vacuum
_sectio_recusata (
    CredoContextus* c)
{
            Reactor* r = reactor_creare(c->piscina);
          TcpServus* servus = _servum_creare(c->piscina);
    NominaAddressus  a;
        TcpResultus  res;
             chorda  descr;
                i32  portus;

    CREDO_NECESSE_NON_NIHIL(servus);
    portus = tcp_servus_obtinere_portum(servus);
    tcp_servus_claudere(servus);
    CREDO_NECESSE(_addressum(r, portus, &a, c->piscina));
    res = tcp_incipere(a.octeti, a.mensura, c->piscina);
    si (!res.successus)
    {
        /* recusatio statim (loopback) - etiam licet, nominata */
        CREDO_AEQUALIS_I32((i32)res.error, (i32)TCP_ERROR_CONNEXIO);
        CREDO_VERUM(res.error_descriptio.mensura > ZEPHYRUM);
        redde;
    }
    CREDO_NECESSE(_exspectare_scribibilem(r, res.connexio));
    descr.datum    = NIHIL;
    descr.mensura  = ZEPHYRUM;
    CREDO_AEQUALIS_I32((i32)tcp_incipere_perficere(res.connexio, &descr,
        c->piscina), (i32)TCP_ERROR_CONNEXIO);
    CREDO_VERUM(descr.mensura > ZEPHYRUM);
    tcp_claudere(res.connexio);
}

/* par claudit, tcp_recipere 0 reddit (clausa), tcp_claudere fossam
 * TAMEN claudit; iterum: nihil */
interior vacuum
_sectio_clausura_paris (
    CredoContextus* c)
{
            Reactor* r = reactor_creare(c->piscina);
          TcpServus* servus = _servum_creare(c->piscina);
    NominaAddressus  a;
        TcpResultus  res;
        TcpResultus  acc;
                 i8  capsa[XVI];
                s32  fossa;
                s32  n = -I;
                s64  initium;

    CREDO_NECESSE_NON_NIHIL(servus);
    CREDO_NECESSE(_addressum(r, tcp_servus_obtinere_portum(servus), &a,
        c->piscina));
    res = tcp_incipere(a.octeti, a.mensura, c->piscina);
    CREDO_NECESSE(res.successus);
    CREDO_NECESSE(_exspectare_scribibilem(r, res.connexio));
    acc = tcp_servus_accipere(servus, c->piscina);
    CREDO_NECESSE(acc.successus);
    tcp_claudere(acc.connexio);
    initium = _ms_nunc();
    dum (n != ZEPHYRUM && _ms_nunc() - initium < M)
    {
        n = tcp_recipere(res.connexio, capsa, XVI);
    }
    CREDO_AEQUALIS_S64((s64)n, (s64)ZEPHYRUM);
    fossa = tcp_obtinere_fd(res.connexio);
    CREDO_NECESSE(fossa >= ZEPHYRUM);
    tcp_claudere(res.connexio);
    CREDO_VERUM(fcntl((integer)fossa, F_GETFD) == -I);
    tcp_claudere(res.connexio);
    tcp_servus_claudere(servus);
}

interior vacuum
_sectio_ex_fossa (
    CredoContextus* c)
{
         integer pipa[II];
    TcpConnexio* conn;

    CREDO_NECESSE(pipe(pipa) == ZEPHYRUM);
    conn = tcp_ex_fossa(pipa[ZEPHYRUM], c->piscina);
    CREDO_NECESSE_NON_NIHIL(conn);
    CREDO_AEQUALIS_I32((i32)tcp_obtinere_fd(conn), (i32)pipa[ZEPHYRUM]);
    tcp_claudere(conn);
    CREDO_VERUM(fcntl(pipa[ZEPHYRUM], F_GETFD) == -I);
    CREDO_VERUM(tcp_ex_fossa(-I, c->piscina) == NIHIL);
    (vacuum)close(pipa[I]);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "connexa", _sectio_connexa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusata", _sectio_recusata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausura paris", _sectio_clausura_paris, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "ex fossa", _sectio_ex_fossa, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tcp_incipere", SECTIONES);
}
```

- [ ] **Step 2: Run it to verify it fails**

Run: `./compile_tests.sh probatio_tcp_incipere > $S/t3_red.log 2>&1; tail -5 $S/t3_red.log`
Expected: build failure - `tcp_incipere`, `tcp_incipere_perficere`,
`tcp_ex_fossa` undeclared.

- [ ] **Step 3: Implement**

`include/tcp.h`: the three prototypes of the Interfaces block (house
layout), and on `tcp_claudere`'s comment add "fossa SEMPER clauditur
(etiam post clausuram paris); iterum vocare tutum".

`lib/tcp_posix.c`, after `tcp_connectere_cum_optionibus`:

```c
/* ========================================================================
 * FUNCTIONES PUBLICAE - CONNEXIO NON-BLOCANS (http-vocatio II.2)
 * ======================================================================== */

interior TcpConnexio*
_connexionem_creare (
    integer  fd,
    Piscina* piscina)
{
    TcpConnexio* conn;

    conn = (TcpConnexio*)piscina_allocare(piscina, (i64)magnitudo(TcpConnexio));
    si (!conn)
    {
        redde NIHIL;
    }
    conn->piscina  = piscina;
    conn->fd       = fd;
    conn->clausa   = FALSUM;
    redde conn;
}

TcpResultus
tcp_incipere (
    constans i8* addressus,
            i32  mensura,
        Piscina* piscina)
{
    constans structura sockaddr* sa;
                  TcpResultus  res;
                  TcpOptiones  opt;
                  TcpConnexio* conn;
                      integer  fd;
                      integer  r;
                      integer  erratum;

    si (!addressus || mensura == 0 || !piscina)
    {
        redde _creare_error(TCP_ERROR_CONNEXIO, "Argumenta invalida", piscina);
    }
    sa = (constans structura sockaddr*)(constans vacuum*)addressus;
    fd = socket(sa->sa_family, SOCK_STREAM, IPPROTO_TCP);
    si (fd == -1)
    {
        redde _creare_error(TCP_ERROR_CONNEXIO, strerror(errno), piscina);
    }
    si (_ponere_non_blocans_fd(fd, VERUM) != TCP_OK)
    {
        close(fd);
        redde _creare_error(TCP_ERROR_CONNEXIO, "O_NONBLOCK fallitum", piscina);
    }
    _ponere_nosigpipe(fd);
    opt.timeout_ms  = 0;          /* tempora vocantis, non socket */
    opt.nodelay     = VERUM;
    opt.keepalive   = FALSUM;
    _applicare_optiones(fd, &opt);

    /* EINPROGRESS = in fundo pergit; EINTR in socket non-blocans idem
     * (POSIX: connexio asynchrone perficitur) */
    r = connect(fd, sa, (socklen_t)mensura);
    si (r == -1 && errno != EINPROGRESS && errno != EINTR)
    {
        erratum = errno;
        close(fd);
        redde _creare_error(TCP_ERROR_CONNEXIO, strerror(erratum), piscina);
    }

    conn = _connexionem_creare(fd, piscina);
    si (!conn)
    {
        close(fd);
        redde _creare_error(TCP_ERROR_CONNEXIO, "Allocatio fallita", piscina);
    }
    res.successus                 = VERUM;
    res.connexio                  = conn;
    res.error                     = TCP_OK;
    res.error_descriptio.datum    = NIHIL;
    res.error_descriptio.mensura  = 0;
    redde res;
}

TcpError
tcp_incipere_perficere (
    TcpConnexio* connexio,
         chorda* descriptio,
        Piscina* piscina)
{
      integer erratum = 0;
    socklen_t n       = (socklen_t)magnitudo(erratum);

    si (!connexio || connexio->fd < 0)
    {
        redde TCP_ERROR_IO;
    }
    si (getsockopt(connexio->fd, SOL_SOCKET, SO_ERROR, &erratum, &n) != 0)
    {
        erratum = errno;
    }
    si (erratum == 0)
    {
        redde TCP_OK;
    }
    si (descriptio && piscina)
    {
        *descriptio = chorda_ex_literis(strerror(erratum), piscina);
    }
    redde TCP_ERROR_CONNEXIO;
}

TcpConnexio*
tcp_ex_fossa (
    integer  fossa,
    Piscina* piscina)
{
    si (fossa < 0 || !piscina)
    {
        redde NIHIL;
    }
    redde _connexionem_creare(fossa, piscina);
}
```

(`_ponere_non_blocans_fd` is defined further down the file -
`tcp_ponere_non_blocans` `:867` calls it; if it is below the new code,
add a forward declaration `interior TcpError _ponere_non_blocans_fd
(integer fd, b32 non_blocans);` above.)

`tcp_claudere` (R5):

```c
vacuum
tcp_claudere (
    TcpConnexio* connexio)
{
    si (!connexio)
    {
        redde;
    }

    /* SEMPER claudere: clausa a tcp_recipere poni potest (par clausit)
     * dum fossa nostra adhuc aperta est - olim close() omittebatur et
     * fossa omnis connexionis ab hospite clausae peribat (R5) */
    si (connexio->fd >= 0)
    {
        close(connexio->fd);
        connexio->fd = -1;
    }
    connexio->clausa = VERUM;
}
```

Lexicon (examen judges `lib/tcp_posix.c` at commit):
`silva/fontes/systema_posix.h` errno section gains
`#define EINPROGRESS 36`; the socket section gains
`#define SO_ERROR     0x1007`. `officina/auspex_posix.sh`: next to the
`SO_KEEPALIVE` assert add
`echo "IUNGO_ASSERTA(SO_ERROR == 0x1007);"`, next to `ENOENT` add
`echo "IUNGO_ASSERTA(EINPROGRESS == 36);"`. Run
`./officina/auspex_posix.sh` -> CONSENTIT. Plant `EINPROGRESS 37` in
`systema_posix.h` -> DISSENTIT, restore -> CONSENTIT. Run the lint; for
each NOVA word it names from these names (expect `inprogress`), add a
glossary line in the 622f8951 form:

```xml
  <vocabulum lemma="inprogress" lingua="anglica" classis="ignotum-permissum" contextus="latinus" nota="EINPROGRESS errno.h (connexio non-blocans pergit); systema_posix.h 2026-10-1x pro lib/tcp_posix.c (http-vocatio T3)"/>
```

Worklog entry in `silva/fontes/systema_posix.worklog.md`.

- [ ] **Step 4: Run it to verify it passes, then neighbours**

Run: `./compile_tests.sh probatio_tcp_incipere > $S/t3_green.log 2>&1; grep -E 'FRACTA|Totalis|Fracti' $S/t3_green.log; for t in probatio_tcp probatio_http probatio_hospitium; do ./compile_tests.sh $t > $S/t3_$t.log 2>&1; echo "$t rc=$?"; done`
Expected: 0 fracti; neighbours rc=0 (`probatio_tcp` has network
sections - if it is link-only in gates it may need the network; a red
section there that also fails on HEAD is not ours - check with
`git stash`-free comparison: build HEAD's copy in a scratch worktree
only if it fails).

- [ ] **Step 5: Plants** (`filtrum='probatio_tcp_incipere'`, `lib/tcp_posix.c`)
  - `si (connexio->fd >= 0)\n    {\n        close(connexio->fd);` (in
    `tcp_claudere`) -> `si (connexio->fd >= 0 && !connexio->clausa)\n    {\n        close(connexio->fd);`
    - expect `clausura paris` red.
  - `si (r == -1 && errno != EINPROGRESS && errno != EINTR)` ->
    `si (r == -1)` - expect `connexa` red.
  - in `tcp_incipere_perficere`, `si (erratum == 0)` -> `si (VERUM)` -
    expect `recusata` red (when the refusal is not immediate).

- [ ] **Step 6: Worklog + commit**

`lib/tcp_posix.worklog.md`: the fd leak (every server-closed
connection, i.e. every `http_exsequi`, leaked its fd since the
`clausa` guard was added), non-blocking connect contract, no socket
timeouts on these sockets. Commit `include/tcp.h lib/tcp_posix.c
lib/tcp_posix.worklog.md probationes/probatio_tcp_incipere.c
silva/fontes/systema_posix.h silva/fontes/systema_posix.worklog.md
officina/auspex_posix.sh oratio/glossarium.stml`.

---

### Task 4: TLS - stepped handshake, non-blocking I/O, context leak (spec II.3, R5)

**Files:**
- Modify: `include/tls.h`, `lib/tls_macos.m` (struct `:22-28`, read
  callback `:36-87`, write callback `:89-124`,
  `tls_connectere_cum_optionibus` `:292-476`, `tls_mittere` `:483-518`,
  `tls_recipere` `:554-613`, `tls_claudere` `:615-640`)
- Create: `probationes/probatio_tls_gradus.c`
- Modify: `lib/tls_macos.worklog.md`

**Interfaces:**
- Consumes: `tcp_incipere`, `tcp_incipere_perficere` (Task 3);
  `nomina` (Task 2) in the test.
- Produces:

```c
#define TLS_ITERUM (-2)   /* gradatim: nihil paratum nunc (= TCP_ITERUM) */

nomen enumeratio {
    TLS_GRADUS_PERFECTUS = 0,
    TLS_GRADUS_LEGERE,        /* exspecta fossam legibilem, iterum */
    TLS_GRADUS_SCRIBERE,      /* exspecta scribibilem, iterum */
    TLS_GRADUS_ERROR          /* tls_gradus_error causam dat */
} TlsGradus;

/* super TCP IAM connexam et non-blocantem: contextus paratur (SNI,
 * TLS 1.2+, cifrae admissae, verificatio), nullus octetus mittitur.
 * Connexio GRADATIM: tls_recipere/tls_mittere TLS_ITERUM reddunt,
 * numquam exspectant; tempus excessum numquam ponitur. tcp a TLS
 * possidetur: tls_claudere (et defectus hic) eam claudit. */
TlsResultus tls_incipere(TcpConnexio* tcp, constans character* hospes,
    constans TlsOptiones* optiones, Piscina* piscina);
/* gradus unus pactionis (SSLHandshake); quid exspectandum reddit */
TlsGradus tls_gradus(TlsConnexio* connexio);
/* post TLS_GRADUS_ERROR: descriptio (certificatum, pactio, clausa) */
chorda tls_gradus_error(TlsConnexio* connexio);
```

Gradatim I/O contract (documented on `tls_recipere` / `tls_mittere`):
`tls_recipere` returns > 0 bytes, 0 = peer closed (clean), `TLS_ITERUM`
= nothing now, < 0 other error - the caller drains it UNTIL
`TLS_ITERUM` on every readiness, because SecureTransport may hold
decrypted bytes the socket no longer signals. `tls_mittere` after
returning `TLS_ITERUM` must be called again with the SAME remaining
bytes; it returns how many to advance (libcurl's SecureTransport
quirk: after `errSSLWouldBlock` the data is buffered inside and is
flushed by `SSLWrite(NULL, 0)`).

- [ ] **Step 1: Write the failing test**

`probationes/probatio_tls_gradus.c` (no network: a local TCP peer that
is not a TLS server):

```c
/* probatio_tls_gradus.c - pactio TLS gradatim (http-vocatio II.3) sine
 * rete: par TCP localis qui servus TLS NON est. Salutatio clientis
 * (recordum 0x16 0x03) mittitur et gradus LEGERE reddit sine mora;
 * responsum pravum -> ERROR nominatus intra tectum, numquam obstat;
 * par clausus -> ERROR; tls_claudere fossam claudit (R5). Pactio vera:
 * probatio_http_vocatio_rete.c (facultas rete). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "reactor.h"
#include "nomina.h"
#include "tcp.h"
#include "tls.h"
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/time.h>

nomen structura {
        TcpServus* servus;
      TcpConnexio* par;       /* servi latus (acceptum) */
      TlsConnexio* tls;
              s32  fossa;     /* clientis */
} Iunctura;

nomen structura {
                i32 vocatus;
    NominaAddressus addressus;
} Responsum;

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_nomina_finis (
                     vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    Responsum* r = (Responsum*)datum;

    (vacuum)error;
    r->vocatus++;
    si (numerus > ZEPHYRUM)
    {
        r->addressus = addressus[ZEPHYRUM];
    }
}

/* fossa parata ad eventus (poll() simplex, tectum ms) */
interior b32
_exspectare (
    s32 fossa,
    b32 scribere,
    i32 tectum_ms)
{
    structura pollfd pf;

    pf.fd       = (integer)fossa;
    pf.events   = scribere ? POLLOUT : POLLIN;
    pf.revents  = 0;
    redde poll(&pf, I, (integer)tectum_ms) == I;
}

/* servus 127.0.0.1, connexio non-blocans, acceptio, tls_incipere */
interior b32
_iungere (
     Iunctura* j,
      Piscina* p)
{
          Reactor* r = reactor_creare(p);
    TcpServusOptiones o = tcp_servus_optiones_default();
    TcpServusResultus sr;
         Responsum x;
            chorda error;
       TcpResultus res;
       TcpResultus acc;
       TlsResultus tr;
               s64 initium;

    sr = tcp_servus_creare_cum_optionibus("127.0.0.1", ZEPHYRUM, &o, p);
    si (!sr.successus || tcp_servus_auscultare(sr.servus, IV) != TCP_OK)
    {
        redde FALSUM;
    }
    j->servus = sr.servus;
    memset(&x, ZEPHYRUM, magnitudo(x));
    si (!nomina_quaerere_incipere(r, chorda_ex_literis("127.0.0.1", p),
            tcp_servus_obtinere_portum(sr.servus), _nomina_finis, &x, p,
            &error))
    {
        redde FALSUM;
    }
    initium = _ms_nunc();
    dum (x.vocatus == ZEPHYRUM && _ms_nunc() - initium < M)
    {
        (vacuum)reactor_poll(r, L);
    }
    res = tcp_incipere(x.addressus.octeti, x.addressus.mensura, p);
    si (!res.successus)
    {
        redde FALSUM;
    }
    j->fossa = tcp_obtinere_fd(res.connexio);
    si (!_exspectare(j->fossa, VERUM, M)
        || tcp_incipere_perficere(res.connexio, &error, p) != TCP_OK)
    {
        redde FALSUM;
    }
    acc = tcp_servus_accipere(sr.servus, p);
    si (!acc.successus)
    {
        redde FALSUM;
    }
    j->par  = acc.connexio;
    tr      = tls_incipere(res.connexio, "localhost", NIHIL, p);
    si (!tr.successus)
    {
        redde FALSUM;
    }
    j->tls = tr.connexio;
    redde VERUM;
}

/* salutatio clientis mittitur, gradus LEGERE sine mora */
interior vacuum
_sectio_salutatio (
    CredoContextus* c)
{
    Iunctura j;
          i8 capsa[DXII];
         s32 n;
         s64 initium;

    memset(&j, ZEPHYRUM, magnitudo(j));
    CREDO_NECESSE(_iungere(&j, c->piscina));
    initium = _ms_nunc();
    CREDO_AEQUALIS_I32((i32)tls_gradus(j.tls), (i32)TLS_GRADUS_LEGERE);
    CREDO_VERUM(_ms_nunc() - initium < C);
    /* iterum sine responso: LEGERE, numquam 'tempus excessum' */
    CREDO_AEQUALIS_I32((i32)tls_gradus(j.tls), (i32)TLS_GRADUS_LEGERE);
    CREDO_NECESSE(_exspectare(tcp_obtinere_fd(j.par), FALSUM, M));
    n = tcp_recipere(j.par, capsa, DXII);
    CREDO_VERUM(n > V);
    CREDO_AEQUALIS_I32((i32)(insignatus character)capsa[ZEPHYRUM], XXII);
    CREDO_AEQUALIS_I32((i32)(insignatus character)capsa[I], III);
    tls_claudere(j.tls);
    tcp_claudere(j.par);
    tcp_servus_claudere(j.servus);
}

/* responsum non TLS: ERROR nominatus intra tectum */
interior vacuum
_sectio_pravum (
    CredoContextus* c)
{
     Iunctura j;
    TlsGradus g;
          s64 initium;
       chorda causa;

    memset(&j, ZEPHYRUM, magnitudo(j));
    CREDO_NECESSE(_iungere(&j, c->piscina));
    g = tls_gradus(j.tls);
    CREDO_NECESSE(g == TLS_GRADUS_LEGERE);
    CREDO_NECESSE(tcp_mittere_omnia(j.par,
        (constans i8*)"HTTP/1.1 400 Bad Request\r\n\r\n", XXVIII));
    initium = _ms_nunc();
    dum (g == TLS_GRADUS_LEGERE && _ms_nunc() - initium < II * M)
    {
        (vacuum)_exspectare(j.fossa, FALSUM, C);
        g = tls_gradus(j.tls);
    }
    CREDO_AEQUALIS_I32((i32)g, (i32)TLS_GRADUS_ERROR);
    causa = tls_gradus_error(j.tls);
    CREDO_VERUM(causa.mensura > ZEPHYRUM);
    tls_claudere(j.tls);
    tcp_claudere(j.par);
    tcp_servus_claudere(j.servus);
}

/* par claudit: ERROR; tls_claudere fossam clientis VERE claudit */
interior vacuum
_sectio_clausus (
    CredoContextus* c)
{
     Iunctura j;
    TlsGradus g;
          s64 initium;

    memset(&j, ZEPHYRUM, magnitudo(j));
    CREDO_NECESSE(_iungere(&j, c->piscina));
    g = tls_gradus(j.tls);
    CREDO_NECESSE(g == TLS_GRADUS_LEGERE);
    tcp_claudere(j.par);
    initium = _ms_nunc();
    dum (g == TLS_GRADUS_LEGERE && _ms_nunc() - initium < II * M)
    {
        (vacuum)_exspectare(j.fossa, FALSUM, C);
        g = tls_gradus(j.tls);
    }
    CREDO_AEQUALIS_I32((i32)g, (i32)TLS_GRADUS_ERROR);
    tls_claudere(j.tls);
    CREDO_VERUM(fcntl((integer)j.fossa, F_GETFD) == -I);
    tls_claudere(j.tls);
    tcp_servus_claudere(j.servus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "salutatio", _sectio_salutatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pravum", _sectio_pravum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausus", _sectio_clausus, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tls_gradus", SECTIONES);
}
```

(`poll`/`POLLIN`/`POLLOUT`/`struct pollfd` are in the lexicon -
`lib/reactor.c:13-16`. `tls_incipere(..., NIHIL, ...)` means default
options with verification on.)

- [ ] **Step 2: Run it to verify it fails**

Run: `./compile_tests.sh probatio_tls_gradus > $S/t4_red.log 2>&1; tail -5 $S/t4_red.log`
Expected: build failure - `tls_incipere`, `tls_gradus`,
`TLS_GRADUS_LEGERE` undeclared.

- [ ] **Step 3: Implement**

`include/tls.h`: `TLS_ITERUM`, `TlsGradus`, the three prototypes, and
the gradatim contract text on `tls_recipere` / `tls_mittere` (Interfaces
block above).

`lib/tls_macos.m`:

Struct:

```c
structura TlsConnexio {
    Piscina*       piscina;
    TcpConnexio*   tcp;           /* Connexio TCP subiacens */
    SSLContextRef  ssl_context;   /* Security.framework context */
    b32            clausa;
    b32            tempus_excessum; /* callback lectionis TCP_ITERUM vidit
                                     * (solum modo obstruente) */
    b32            gradatim;      /* http-vocatio II.3: non-blocans */
    b32            scribere_pendet; /* callback scriptionis EAGAIN vidit */
    size_t         scriptum_pendens; /* SSLWrite WouldBlock: octeti intus
                                     * servati, SSLWrite(NULL,0) eos
                                     * effundit (mos libcurl) */
    chorda         erratum;       /* post TLS_GRADUS_ERROR */
};
```

Read callback, the `TCP_ITERUM` branch:

```c
    si (n == TCP_ITERUM)
    {
        /* obstruens: SO_RCVTIMEO incensum = tempus excessum;
         * gradatim: solum 'nihil nunc' */
        si (!conn->gradatim)
        {
            conn->tempus_excessum = VERUM;
        }
        *dataLength = 0;
        redde errSSLWouldBlock;
    }
```

Write callback, the `n == 0` branch:

```c
    si (n == 0)
    {
        conn->scribere_pendet = VERUM;
        *dataLength = 0;
        redde errSSLWouldBlock;
    }
```

Factor the context setup out of `tls_connectere_cum_optionibus`
(`:343-415`, unchanged behaviour):

```c
/* contextus SSL super conn->tcp: callbacks, connexio, SNI, durare,
 * verificatio. NIHIL si bene; aliter causa (contextus dimissus,
 * conn->ssl_context NIHIL; tcp NON clausa - vocans claudit) */
interior constans character*
_contextum_parare(TlsConnexio* conn, constans character* hospes,
    constans TlsOptiones* optiones)
{
    SSLContextRef ssl_context;
    OSStatus status;
    constans character* causa;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    ssl_context = SSLCreateContext(NIHIL, kSSLClientSide, kSSLStreamType);
#pragma clang diagnostic pop
    si (!ssl_context)
    {
        redde "SSLCreateContext fallita";
    }
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    status = SSLSetIOFuncs(ssl_context, _tls_read_callback, _tls_write_callback);
    si (status == noErr)
    {
        status = SSLSetConnection(ssl_context, conn);
    }
    si (status == noErr)
    {
        status = SSLSetPeerDomainName(ssl_context, hospes, strlen(hospes));
    }
#pragma clang diagnostic pop
    si (status != noErr)
    {
        CFRelease(ssl_context);
        redde "SSLSetIOFuncs/SSLSetConnection/SSLSetPeerDomainName fallita";
    }
    causa = _durare(ssl_context, conn->piscina);
    si (causa)
    {
        CFRelease(ssl_context);
        redde causa;
    }
    si (optiones && !optiones->verificare_certificatum)
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        SSLSetSessionOption(ssl_context, kSSLSessionOptionBreakOnServerAuth, true);
#pragma clang diagnostic pop
    }
    conn->ssl_context = ssl_context;
    redde NIHIL;
}
```

In `tls_connectere_cum_optionibus` replace `:343-415` with:

```c
    {
        constans character* causa = _contextum_parare(conn, hospes, optiones);

        si (causa)
        {
            tcp_claudere(conn->tcp);
            redde _creare_error(TLS_ERROR_HANDSHAKE, causa, piscina);
        }
        ssl_context = conn->ssl_context;
    }
```

(the three old messages "SSLSetIOFuncs fallita" etc. become one - grep
the tests for those strings first: `grep -rn 'SSLSet' probationes/`;
if one is asserted, keep separate messages.) Initialize the new fields
in both constructors (`gradatim`, `scribere_pendet` FALSUM,
`scriptum_pendens` 0, `erratum` empty).

New functions after `tls_connectere_cum_optionibus`:

```c
TlsResultus
tls_incipere(
    TcpConnexio*          tcp,
    constans character*   hospes,
    constans TlsOptiones* optiones,
    Piscina*              piscina)
{
    TlsResultus res;
    TlsConnexio* conn;
    constans character* causa;

    si (!tcp || !hospes || !piscina)
    {
        si (tcp)
        {
            tcp_claudere(tcp);
        }
        redde _creare_error(TLS_ERROR_TCP, "Argumenta invalida", piscina);
    }
    conn = (TlsConnexio*)piscina_allocare(piscina, (i64)magnitudo(TlsConnexio));
    si (!conn)
    {
        tcp_claudere(tcp);
        redde _creare_error(TLS_ERROR_TCP, "Allocatio fallita", piscina);
    }
    memset(conn, 0, magnitudo(TlsConnexio));
    conn->piscina   = piscina;
    conn->tcp       = tcp;
    conn->gradatim  = VERUM;
    causa = _contextum_parare(conn, hospes, optiones);
    si (causa)
    {
        tcp_claudere(tcp);
        redde _creare_error(TLS_ERROR_HANDSHAKE, causa, piscina);
    }
    res.successus = VERUM;
    res.connexio = conn;
    res.error = TLS_OK;
    res.error_descriptio.datum = NIHIL;
    res.error_descriptio.mensura = 0;
    redde res;
}

TlsGradus
tls_gradus(TlsConnexio* connexio)
{
    OSStatus status;

    si (!connexio || !connexio->ssl_context || connexio->clausa)
    {
        redde TLS_GRADUS_ERROR;
    }
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    fac
    {
        connexio->scribere_pendet = FALSUM;
        status = SSLHandshake(connexio->ssl_context);
    } dum (status == errSSLServerAuthCompleted);
#pragma clang diagnostic pop
    si (status == noErr)
    {
        redde TLS_GRADUS_PERFECTUS;
    }
    si (status == errSSLWouldBlock)
    {
        redde connexio->scribere_pendet ? TLS_GRADUS_SCRIBERE
                                        : TLS_GRADUS_LEGERE;
    }
    connexio->erratum = chorda_ex_literis(_osstatus_descriptio(status),
        connexio->piscina);
    redde TLS_GRADUS_ERROR;
}

chorda
tls_gradus_error(TlsConnexio* connexio)
{
    chorda vacua;

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    si (!connexio)
    {
        redde vacua;
    }
    redde connexio->erratum;
}
```

`tls_mittere` - at the top, after the argument checks:

```c
    si (connexio->gradatim)
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        si (connexio->scriptum_pendens > 0)
        {
            status = SSLWrite(connexio->ssl_context, NIHIL, 0, &processed);
            si (status == errSSLWouldBlock)
            {
                redde TLS_ITERUM;
            }
            si (status != noErr)
            {
                redde -1;
            }
            processed = connexio->scriptum_pendens;
            connexio->scriptum_pendens = 0;
            redde (s32)processed;
        }
        status = SSLWrite(connexio->ssl_context, data, (size_t)mensura, &processed);
#pragma clang diagnostic pop
        si (status == errSSLWouldBlock)
        {
            si (processed > 0)
            {
                redde (s32)processed;
            }
            connexio->scriptum_pendens = (size_t)mensura;
            redde TLS_ITERUM;
        }
        redde status == noErr ? (s32)processed : -1;
    }
```

`tls_recipere` - after the argument checks, before the blocking loop:

```c
    si (connexio->gradatim)
    {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        status = SSLRead(connexio->ssl_context, buffer, (size_t)capacitas, &processed);
#pragma clang diagnostic pop
        si (processed > 0)
        {
            redde (s32)processed;
        }
        si (status == errSSLWouldBlock)
        {
            redde TLS_ITERUM;
        }
        si (status == errSSLClosedGraceful || status == errSSLClosedNoNotify)
        {
            connexio->clausa = VERUM;
            redde 0;
        }
        redde -1;
    }
```

`tls_claudere` (R5):

```c
vacuum
tls_claudere(TlsConnexio* connexio)
{
    si (!connexio)
    {
        redde;
    }

    /* contextus SEMPER dimittitur - clausa a tls_recipere poni potest
     * (par clausit) dum contextus adhuc vivit (R5) */
    si (connexio->ssl_context)
    {
        si (!connexio->clausa)
        {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
            SSLClose(connexio->ssl_context);
#pragma clang diagnostic pop
        }
        CFRelease(connexio->ssl_context);
        connexio->ssl_context = NIHIL;
    }

    si (connexio->tcp)
    {
        tcp_claudere(connexio->tcp);
        connexio->tcp = NIHIL;
    }

    connexio->clausa = VERUM;
}
```

(`SSLClose` on a non-blocking socket may return `errSSLWouldBlock`
without sending close_notify; that is acceptable - the socket closes
right after.)

- [ ] **Step 4: Run it to verify it passes, then neighbours**

Run: `./compile_tests.sh probatio_tls_gradus > $S/t4_green.log 2>&1; grep -E 'FRACTA|Totalis|Fracti' $S/t4_green.log; for t in probatio_http probatio_vates probatio_herbarium; do ./compile_tests.sh $t > $S/t4_$t.log 2>&1; echo "$t rc=$?"; done`
Expected: 0 fracti; neighbours rc=0 (the blocking TLS path is
unchanged).

- [ ] **Step 5: Plants** (`filtrum='probatio_tls_gradus'`, `lib/tls_macos.m`)
  - in `tls_gradus`, `redde connexio->scribere_pendet ? TLS_GRADUS_SCRIBERE\n                                        : TLS_GRADUS_LEGERE;`
    -> `redde TLS_GRADUS_ERROR;` - expect `salutatio` red.
  - in `tls_gradus`, `                                        : TLS_GRADUS_LEGERE;`
    -> `                                        : TLS_GRADUS_SCRIBERE;` -
    expect `salutatio` red (wrong readiness: a caller would wait for
    writability forever while the server's reply sits unread). Never
    plant the `dum` condition: `errSSLWouldBlock` there spins forever
    and hangs the suite instead of failing it.
  - NOT plantable here (no TLS server offline): the context release in
    `tls_claudere`, the gradatim `TLS_ITERUM` of `tls_recipere`, and the
    `SSLWrite(NULL, 0)` flush. Task 5's rete file exercises the read
    path end to end; record the other two as unverified in the
    worklog.

- [ ] **Step 6: Worklog + commit**

`lib/tls_macos.worklog.md`: stepped mode, "would block is not timeout"
(`:58-63`), the curl write quirk, drain-until-ITERUM rule, the context
leak (R5), which behaviours are only exercised by the rete file.
Commit `include/tls.h lib/tls_macos.m lib/tls_macos.worklog.md
probationes/probatio_tls_gradus.c`.

---

### Task 5: `http_vocatio` - the call state machine + scripted server (spec II.4, III)

**Files:**
- Modify: `include/http.h` (accessors, after `http_petitio_corpus_ponere_chorda`), `lib/http.c` (after `:699-716`)
- Create: `include/http_vocatio.h`, `lib/http_vocatio.c`,
  `lib/http_vocatio.worklog.md`
- Create: `probationes/servus_scriptus.h` (test helper)
- Create: `probationes/probatio_http_vocatio.c`,
  `probationes/probatio_http_vocatio_rete.c` (facultas rete)
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: Task 1 guarantees; `nomina_quaerere_incipere` /
  `_abrumpere` / `NominaAddressus` (Task 2); `tcp_incipere`,
  `tcp_incipere_perficere`, `tcp_recipere`, `tcp_mittere`,
  `tcp_claudere`, `TCP_ITERUM` (Task 3 + existing); `tls_incipere`,
  `tls_gradus`, `tls_gradus_error`, `tls_recipere`, `tls_mittere`,
  `tls_claudere`, `TLS_ITERUM` (Task 4); `http_lector_*` (quaestio-rivus
  T1, `include/http.h:106-135`).
- Produces (`include/http.h`, implemented in `lib/http.c` where the
  opaque struct lives):

```c
/* http-vocatio II.4: destinatio et octeti petitionis (pro vectura
 * extra http.c). hospes NUL-terminatus in piscina petitionis. */
constans character* http_petitio_hospes(HttpPetitio* petitio);
i32 http_petitio_portus(constans HttpPetitio* petitio);
b32 http_petitio_est_https(constans HttpPetitio* petitio);
MoraAngusta http_petitio_tempus(constans HttpPetitio* petitio);
HttpMethodus http_petitio_methodus(constans HttpPetitio* petitio);
chorda http_petitio_octeti(HttpPetitio* petitio);  /* tota, serializata */
```

and `include/http_vocatio.h`:

```c
nomen enumeratio {
    HTTP_VOCATIO_NOMINA = 0,
    HTTP_VOCATIO_CONNECTIO,
    HTTP_VOCATIO_PACTIO,        /* solum https */
    HTTP_VOCATIO_MITTENS,
    HTTP_VOCATIO_LEGENS,
    HTTP_VOCATIO_FINITA,
    HTTP_VOCATIO_IRRITA,
    HTTP_VOCATIO_ABRUPTA
} HttpVocatioStatus;

nomen structura HttpVocatio HttpVocatio;

/* SEMEL per vocationem; numquam intra http_vocare; piscinam
 * vocationis NE destruat (codex post reditum adhuc currit) */
nomen vacuum (*HttpVocatioFinis)(vacuum* datum, HttpVocatio* vocatio);

nomen structura {
    i32 mora_connexionis_ms;  /* 0 = tempus petitionis, aliter XXX s:
                               * NOMINA+CONNECTIO+PACTIO simul (R8) */
    i32 mora_otii_ms;         /* 0 = LX s sine octeto in MITTENS/LEGENS */
} HttpVocatioOptiones;

HttpVocatio* http_vocare(Reactor* reactor, HttpPetitio* petitio,
    HttpRivus rivus, vacuum* rivi_datum, HttpVocatioFinis finis,
    vacuum* finis_datum, constans HttpVocatioOptiones* optiones,
    Piscina* piscina);          /* NIHIL solum si argumenta NIHIL */
HttpVocatioStatus http_vocatio_status(constans HttpVocatio* vocatio);
b32 http_vocatio_finita(constans HttpVocatio* vocatio);
/* post finem: FINITA -> successus + responsum (status, capita, corpus
 * totum); aliter error + descriptio, responsum NIHIL (R7) */
HttpResultus http_vocatio_resultus(constans HttpVocatio* vocatio);
/* idempotens, synchronus: finis (ABRUPTA) ANTE reditum vocatur */
vacuum http_vocationem_abrumpere(HttpVocatio* vocatio);
```

Behaviour contract (header text): body bytes reach `rivus` only for
2xx and never after `finis`; the call ends when `http_lector` reports
finished (no wait for close), at a clean close if the body is
close-delimited, or on error/timeout/abort; after `finis` nothing of
the call is in the reactor; the serialized request is zeroed after it
is sent and again at `finis` (R2).

- [ ] **Step 1: Write the scripted server helper**

`probationes/servus_scriptus.h`:

```c
/* servus_scriptus.h - servus HTTP SCRIPTUS pro probationibus clientium
 * non-obstruentium (http-vocatio-spec III)
 *
 * tcp_servus in 127.0.0.1 (portus ephemerus) in EODEM reactore ac
 * cliens probatus: processus unus, ansa una, ordo determinatus.
 * Connexiones SINGULAE: dum una aperta est, ceterae clauduntur; post
 * clausuram connexio proxima scriptum AB INITIO recipit (iteratio
 * clientis idem scriptum videt - acceptae numerat). Petitio colligitur
 * (capita + corpus Content-Length), deinde gradus scripti: post mora_ms
 * octeti mittuntur; claudere: connexio post gradum clauditur. Gradus
 * nullus = silentium (cliens tempus suum experitur).
 *
 * Functiones INTERIORES (more ludus_toy.h): includens AMBOS aditus
 * vocare debet - servum_scriptum_creare et servum_scriptum_claudere -
 * qui ceteras omnes trahunt. */
#ifndef SERVUS_SCRIPTUS_H
#define SERVUS_SCRIPTUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "reactor.h"
#include "tcp.h"
#include <string.h>

#define SERVUS_CAPSA DXII

nomen structura {
    constans character* octeti;     /* NIHIL = nihil mittitur */
                   i32  mensura;    /* 0 = strlen(octeti) */
                   i32  mora_ms;    /* post gradum priorem / petitionem */
                   b32  claudere;   /* post hunc gradum */
} ServusGradus;

nomen structura {
               Reactor* reactor;
               Piscina* piscina;
             TcpServus* servus;
               integer  fossa_auscultans;
           TcpConnexio* cliens;
               integer  fossa_clientis;
 constans ServusGradus* gradus;
                   i32  numerus;
                   i32  index;
     ChordaAedificator* petitio;     /* octeti petitionis recepti */
                   b32  petitio_plena;
                   i32  portus;
                   i32  acceptae;
        ReactorTimerId  mora;
                   b32  mora_posita;
} ServusScriptus;

interior vacuum _servi_gradus (vacuum* datum);

interior vacuum
_servi_clientem_claudere (
    ServusScriptus* s)
{
    si (s->fossa_clientis >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(s->reactor, s->fossa_clientis);
        s->fossa_clientis = -I;
    }
    si (s->cliens)
    {
        tcp_claudere(s->cliens);
        s->cliens = NIHIL;
    }
}

interior vacuum
_servi_proximum (
    ServusScriptus* s)
{
    si (s->index >= s->numerus)
    {
        redde;
    }
    s->mora = reactor_timer_adicere(s->reactor,
        s->gradus[s->index].mora_ms, FALSUM, _servi_gradus, s);
    s->mora_posita = VERUM;
}

interior vacuum
_servi_gradus (
    vacuum* datum)
{
          ServusScriptus* s = (ServusScriptus*)datum;
    constans ServusGradus* g;
                     i32  n;

    s->mora_posita = FALSUM;
    si (s->index >= s->numerus || !s->cliens)
    {
        redde;
    }
    g = &s->gradus[s->index];
    s->index++;
    si (g->octeti)
    {
        n = g->mensura > ZEPHYRUM ? g->mensura : (i32)strlen(g->octeti);
        si (n > ZEPHYRUM)
        {
            (vacuum)tcp_mittere_omnia(s->cliens, (constans i8*)g->octeti,
                n);
        }
    }
    si (g->claudere)
    {
        _servi_clientem_claudere(s);
        redde;
    }
    _servi_proximum(s);
}

/* littera parva ASCII sine ctype */
interior character
_servi_parva (
    character x)
{
    redde (x >= 'A' && x <= 'Z') ? (character)(x + ('a' - 'A')) : x;
}

/* capita finita et corpus Content-Length totum receptum? */
interior b32
_servi_petitio_plena (
    ServusScriptus* s)
{
    chorda p = chorda_aedificator_spectare(s->petitio);
       s32 finis_capitum = -I;
       s32 k;
       s32 j;
       s32 longitudo = ZEPHYRUM;
    constans character* clavis = "content-length:";

    per (k = ZEPHYRUM; k + III < (s32)p.mensura; k++)
    {
        si (p.datum[k] == '\r' && p.datum[k + I] == '\n'
            && p.datum[k + II] == '\r' && p.datum[k + III] == '\n')
        {
            finis_capitum = k;
            frange;
        }
    }
    si (finis_capitum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k + XV <= finis_capitum; k++)
    {
        per (j = ZEPHYRUM; j < XV
            && _servi_parva((character)p.datum[k + j]) == clavis[j]; j++)
        {
        }
        si (j == XV)
        {
            per (j = k + XV; j < finis_capitum
                && (p.datum[j] == ' ' || (p.datum[j] >= '0'
                && p.datum[j] <= '9')); j++)
            {
                si (p.datum[j] != ' ')
                {
                    longitudo = longitudo * X + (p.datum[j] - '0');
                }
            }
            frange;
        }
    }
    redde (s32)p.mensura >= finis_capitum + IV + longitudo;
}

interior vacuum
_servi_lectio (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    ServusScriptus* s = (ServusScriptus*)datum;
                 i8 capsa[SERVUS_CAPSA];
                s32 n;
                s32 k;

    (vacuum)fossa;
    (vacuum)eventus;
    si (!s->cliens)
    {
        redde;
    }
    n = tcp_recipere(s->cliens, capsa, SERVUS_CAPSA);
    si (n > ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            chorda_aedificator_appendere_character(s->petitio,
                (character)capsa[k]);
        }
        si (!s->petitio_plena && _servi_petitio_plena(s))
        {
            s->petitio_plena = VERUM;
            _servi_proximum(s);
        }
        redde;
    }
    si (n == ZEPHYRUM)
    {
        _servi_clientem_claudere(s);     /* cliens clausit */
    }
}

interior vacuum
_servi_acceptio (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    ServusScriptus* s = (ServusScriptus*)datum;
        TcpResultus acc;

    (vacuum)fossa;
    (vacuum)eventus;
    acc = tcp_servus_accipere(s->servus, s->piscina);
    si (!acc.successus)
    {
        redde;
    }
    s->acceptae++;
    si (s->cliens)
    {
        tcp_claudere(acc.connexio);      /* una simul */
        redde;
    }
    /* connexio nova: scriptum ab initio */
    si (s->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(s->reactor, s->mora);
        s->mora_posita = FALSUM;
    }
    s->index           = ZEPHYRUM;
    s->petitio_plena   = FALSUM;
    s->petitio         = chorda_aedificator_creare(s->piscina, D);
    s->cliens          = acc.connexio;
    s->fossa_clientis  = (integer)tcp_obtinere_fd(acc.connexio);
    (vacuum)reactor_adicere(s->reactor, s->fossa_clientis, REACTOR_LEGERE,
        _servi_lectio, s);
}

interior ServusScriptus*
servum_scriptum_creare (
                 Reactor* reactor,
    constans ServusGradus* gradus,
                     i32  numerus,
                 Piscina* piscina)
{
       ServusScriptus* s;
    TcpServusOptiones  o = tcp_servus_optiones_default();
    TcpServusResultus  sr;

    s = (ServusScriptus*)piscina_allocare(piscina,
        magnitudo(ServusScriptus));
    memset(s, ZEPHYRUM, magnitudo(ServusScriptus));
    s->reactor         = reactor;
    s->piscina         = piscina;
    s->gradus          = gradus;
    s->numerus         = numerus;
    s->fossa_clientis  = -I;
    s->petitio         = chorda_aedificator_creare(piscina, D);
    sr = tcp_servus_creare_cum_optionibus("127.0.0.1", ZEPHYRUM, &o,
        piscina);
    si (!sr.successus || tcp_servus_auscultare(sr.servus, VIII) != TCP_OK)
    {
        redde NIHIL;
    }
    s->servus            = sr.servus;
    s->portus            = tcp_servus_obtinere_portum(sr.servus);
    s->fossa_auscultans  = tcp_servus_obtinere_fd(sr.servus);
    (vacuum)reactor_adicere(reactor, s->fossa_auscultans, REACTOR_LEGERE,
        _servi_acceptio, s);
    redde s;
}

interior vacuum
servum_scriptum_claudere (
    ServusScriptus* s)
{
    si (!s)
    {
        redde;
    }
    si (s->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(s->reactor, s->mora);
        s->mora_posita = FALSUM;
    }
    _servi_clientem_claudere(s);
    si (s->servus)
    {
        (vacuum)reactor_removere(s->reactor, s->fossa_auscultans);
        tcp_servus_claudere(s->servus);
        s->servus = NIHIL;
    }
}

#endif /* SERVUS_SCRIPTUS_H */
```

(Check `chorda.datum`'s element type in `include/chorda.h:15` (`i8*`)
- the `'0'` arithmetic above compiles under `-Wconversion` only with
the casts as written; adjust to `(s32)(p.datum[j] - '0')` if clang
asks.)

- [ ] **Step 2: Write the failing test**

`probationes/probatio_http_vocatio.c`:

```c
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
                  i32 finis;
                  i32 rivi;
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
    constans character* resp = RESPONSUM_TRUNCATUM;
                   i32  n    = (i32)strlen(resp);
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

        g[ZEPHYRUM].octeti    = resp;
        g[ZEPHYRUM].mensura   = k;
        g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
        g[ZEPHYRUM].claudere  = FALSUM;
        g[I].octeti           = resp + k;
        g[I].mensura          = n - k;
        g[I].mora_ms          = I;
        g[I].claudere         = FALSUM;
        s = servum_scriptum_creare(r, g, II, c->piscina);
        CREDO_NECESSE_NON_NIHIL(s);
        _observationem_initiare(&o, c->piscina);
        v = http_vocare(r, _petitio("http", "127.0.0.1", s->portus,
            c->piscina), _rivus, &o, _finis, &o, NIHIL, c->piscina);
        CREDO_NECESSE_NON_NIHIL(v);
        CREDO_NECESSE(o.finis == ZEPHYRUM);  /* numquam intra vocare */
        (vacuum)_currere(r, v, II * M);
        res = http_vocatio_resultus(v);
        si (   o.finis != I || http_vocatio_status(v) != HTTP_VOCATIO_FINITA
            || !res.successus || res.responsum->status != CC
            || !chorda_aequalis_literis(chorda_aedificator_finire(o.corpus),
                "Salve munde"))
        {
            mala++;
        }
        servum_scriptum_claudere(s);
        si (reactor_numerus_fd(r) != ZEPHYRUM
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
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
    v = http_vocare(r, _petitio("http", "127.0.0.1", portus, c->piscina),
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
_sectio_localhost (
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
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
    dum (http_vocatio_status(v) != HTTP_VOCATIO_PACTIO
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
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

    ga[ZEPHYRUM].octeti    = "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\n"
        "Prima";
    ga[ZEPHYRUM].mensura   = ZEPHYRUM;
    ga[ZEPHYRUM].mora_ms   = XXX;
    ga[ZEPHYRUM].claudere  = FALSUM;
    gb[ZEPHYRUM].octeti    = "HTTP/1.1 200 OK\r\nContent-Length: 7\r\n\r\n"
        "Secunda";
    gb[ZEPHYRUM].mensura   = ZEPHYRUM;
    gb[ZEPHYRUM].mora_ms   = X;
    gb[ZEPHYRUM].claudere  = FALSUM;
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
    dum ((!http_vocatio_finita(va) || !http_vocatio_finita(vb))
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

    g[ZEPHYRUM].octeti    = "HTTP/1.1 404 Not Found\r\nContent-Length: 6"
        "\r\n\r\nabsens";
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s = servum_scriptum_creare(r, g, I, c->piscina);
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
    { "localhost", _sectio_localhost, NIHIL, NIHIL, NIHIL, NIHIL },
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
```

`probationes/probatio_http_vocatio_rete.c` (run by hand; gates link
it only):

```c
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
    Nominatio x;
       chorda error;
          s64 initium;

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
```

(`CDI` = 401, `CDIV` = 404. The `anthropic-version` literal is the
value `VATES_VERSIO_ANTHROPIC` holds - check `lib/vates.c:1055-1057`
and use the same string.)

- [ ] **Step 3: Run it to verify it fails**

Run: `./compile_tests.sh probatio_http_vocatio > $S/t5_red.log 2>&1; tail -5 $S/t5_red.log`
Expected: build failure - `http_vocatio.h` not found.

- [ ] **Step 4: Implement the http.c accessors**

`include/http.h` - the six prototypes of the Interfaces block.
`lib/http.c`, after `http_petitio_corpus_ponere_chorda`:

```c
/* ========================================================================
 * DESTINATIO PETITIONIS (http-vocatio II.4)
 * ======================================================================== */

constans character*
http_petitio_hospes (
    HttpPetitio* petitio)
{
    si (!petitio)
    {
        redde NIHIL;
    }
    redde chorda_ut_cstr(petitio->hospes, petitio->piscina);
}

i32
http_petitio_portus (
    constans HttpPetitio* petitio)
{
    redde petitio ? petitio->portus : ZEPHYRUM;
}

b32
http_petitio_est_https (
    constans HttpPetitio* petitio)
{
    redde petitio && chorda_aequalis_literis(petitio->schema, "https");
}

MoraAngusta
http_petitio_tempus (
    constans HttpPetitio* petitio)
{
    redde petitio ? petitio->tempus_ms : ZEPHYRUM;
}

HttpMethodus
http_petitio_methodus (
    constans HttpPetitio* petitio)
{
    redde petitio ? petitio->methodus : HTTP_GET;
}

chorda
http_petitio_octeti (
    HttpPetitio* petitio)
{
    chorda vacua;

    si (!petitio)
    {
        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde vacua;
    }
    redde _construere_petitio(petitio);
}
```

(`_construere_petitio` is defined at `:396`, above - no forward
declaration needed.)

- [ ] **Step 5: Implement `http_vocatio`**

`include/http_vocatio.h` - the Interfaces block with the header comment
(contract text above), includes `latina.h piscina.h chorda.h reactor.h
http.h`.

`lib/http_vocatio.c`:

```c
/* http_vocatio.c - vocatio HTTP(S) non-obstruens super reactorem
 * (http-vocatio-spec II.4; vide caput) */
#include "http_vocatio.h"
#include "nomina.h"
#include "tcp.h"
#include "tls.h"
#include <string.h>

#define HTTP_VOCATIO_CAPSA             (XVI * MXXIV)
#define HTTP_VOCATIO_MORA_CONNEXIONIS  (XXX * M)
#define HTTP_VOCATIO_MORA_OTII         (LX * M)

structura HttpVocatio {
              Reactor* reactor;
              Piscina* piscina;
          HttpPetitio* petitio;
            HttpRivus  rivus;
               vacuum* rivi_datum;
     HttpVocatioFinis  finis;
               vacuum* finis_datum;
    HttpVocatioStatus  status;
       NominaQuaestio* quaestio;
      NominaAddressus  addressus[NOMINA_ADDRESSUS_MAXIMI];
                  i32  addressus_numerus;
                  i32  addressus_index;
               chorda  error_ultimus;     /* connexionis prioris */
          TcpConnexio* tcp;
          TlsConnexio* tls;               /* tcp possidet */
              integer  fossa;             /* in reactore; -1 nulla */
                  b32  est_https;
               chorda  octeti;            /* petitio (clavis intus: R2) */
                  i32  missa;
           HttpLector* lector;
                   i8* capsa;
       ReactorTimerId  mora;
                  b32  mora_posita;
                  i32  mora_connexionis_ms;
                  i32  mora_otii_ms;
            HttpError  error_pendens;     /* != HTTP_OK: per timer 0 */
               chorda  descriptio_pendens;
         HttpResultus  resultus;
                  b32  finita;
};

interior vacuum _mora_expiravit (vacuum* datum);
interior vacuum _fossa_parata (integer fossa, i32 eventus, vacuum* datum);

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

interior vacuum
_moram_ponere (
    HttpVocatio* v,
            i32  ms)
{
    si (v->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(v->reactor, v->mora);
    }
    v->mora         = reactor_timer_adicere(v->reactor, ms, FALSUM,
        _mora_expiravit, v);
    v->mora_posita  = (v->mora != REACTOR_TIMER_ERROR);
}

interior vacuum
_fossam_ponere (
    HttpVocatio* v,
            i32  eventus)
{
    integer fd = (integer)tcp_obtinere_fd(v->tcp);

    si (v->fossa == fd)
    {
        (vacuum)reactor_modificare(v->reactor, fd, eventus);
        redde;
    }
    si (v->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(v->reactor, v->fossa);
    }
    v->fossa = fd;
    (vacuum)reactor_adicere(v->reactor, fd, eventus, _fossa_parata, v);
}

/* fossa e reactore PRIUS, deinde clausura (numerus fossae reusus) */
interior vacuum
_connexionem_dimittere (
    HttpVocatio* v)
{
    si (v->fossa >= ZEPHYRUM)
    {
        (vacuum)reactor_removere(v->reactor, v->fossa);
        v->fossa = -I;
    }
    si (v->tls)
    {
        tls_claudere(v->tls);
        v->tls = NIHIL;
        v->tcp = NIHIL;
    }
    alioquin si (v->tcp)
    {
        tcp_claudere(v->tcp);
        v->tcp = NIHIL;
    }
}

interior vacuum
_octetos_obliterare (
    HttpVocatio* v)
{
    si (v->octeti.datum && v->octeti.mensura > ZEPHYRUM)
    {
        memset(v->octeti.datum, ZEPHYRUM, (size_t)v->octeti.mensura);
    }
}

interior vacuum
_finire (
          HttpVocatio* v,
    HttpVocatioStatus  status,
            HttpError  error,
               chorda  descriptio)
{
    si (v->finita)
    {
        redde;
    }
    v->finita = VERUM;
    si (v->quaestio)
    {
        nomina_quaerere_abrumpere(v->quaestio);
        v->quaestio = NIHIL;
    }
    si (v->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(v->reactor, v->mora);
        v->mora_posita = FALSUM;
    }
    _connexionem_dimittere(v);
    _octetos_obliterare(v);
    v->status                     = status;
    v->resultus.successus         = (status == HTTP_VOCATIO_FINITA);
    v->resultus.responsum         = v->resultus.successus
        ? http_lector_responsum(v->lector) : NIHIL;
    v->resultus.error             = v->resultus.successus ? HTTP_OK : error;
    v->resultus.error_descriptio  = descriptio;
    v->finis(v->finis_datum, v);
}

interior vacuum
_errorem_differre (
    HttpVocatio* v,
      HttpError  error,
         chorda  descriptio)
{
    v->error_pendens       = error;
    v->descriptio_pendens  = descriptio;
    _moram_ponere(v, ZEPHYRUM);
}

interior vacuum
_mora_expiravit (
    vacuum* datum)
{
    HttpVocatio* v = (HttpVocatio*)datum;

    v->mora_posita = FALSUM;
    si (v->finita)
    {
        redde;
    }
    si (v->error_pendens != HTTP_OK)
    {
        _finire(v, HTTP_VOCATIO_IRRITA, v->error_pendens,
            v->descriptio_pendens);
        redde;
    }
    _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_TIMEOUT, chorda_ex_literis(
        v->status <= HTTP_VOCATIO_PACTIO ? "tempus connexionis excessum"
                                         : "tempus otii excessum",
        v->piscina));
}

/* rivus vocantis custoditus: post finem (abruptum intra rivum) nihil */
interior vacuum
_rivus_custos (
         vacuum* datum,
    constans i8* octeti,
            i32  n)
{
    HttpVocatio* v = (HttpVocatio*)datum;

    si (!v->finita && v->rivus)
    {
        v->rivus(v->rivi_datum, octeti, n);
    }
}

interior vacuum
_connectere_proximum (
    HttpVocatio* v)
{
    dum (v->addressus_index < v->addressus_numerus)
    {
        NominaAddressus* a = &v->addressus[v->addressus_index];
             TcpResultus res;

        v->addressus_index++;
        res = tcp_incipere(a->octeti, a->mensura, v->piscina);
        si (res.successus)
        {
            v->tcp     = res.connexio;
            v->status  = HTTP_VOCATIO_CONNECTIO;
            _fossam_ponere(v, REACTOR_SCRIBERE);
            redde;
        }
        v->error_ultimus = res.error_descriptio;
    }
    _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO,
        v->error_ultimus.mensura > ZEPHYRUM ? v->error_ultimus
            : chorda_ex_literis("connexio recusata", v->piscina));
}

interior vacuum
_nomina_finis (
                     vacuum* datum,
    constans NominaAddressus* addressus,
                         i32  numerus,
                      chorda  error)
{
    HttpVocatio* v = (HttpVocatio*)datum;
            i32  k;

    v->quaestio = NIHIL;
    si (v->finita)
    {
        redde;
    }
    si (numerus == ZEPHYRUM)
    {
        _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO,
            chorda_concatenare(chorda_ex_literis("nomen: ", v->piscina),
            error, v->piscina));
        redde;
    }
    per (k = ZEPHYRUM; k < numerus && k < NOMINA_ADDRESSUS_MAXIMI; k++)
    {
        v->addressus[k] = addressus[k];
    }
    v->addressus_numerus  = k;
    v->addressus_index    = ZEPHYRUM;
    _connectere_proximum(v);
}

interior vacuum
_mittere (
    HttpVocatio* v)
{
    dum (v->missa < v->octeti.mensura)
    {
        constans i8* initium = (constans i8*)v->octeti.datum + v->missa;
                i32  restans = v->octeti.mensura - v->missa;
                s32  n       = v->tls
            ? tls_mittere(v->tls, initium, restans)
            : tcp_mittere(v->tcp, initium, restans);

        si (n > ZEPHYRUM)
        {
            v->missa += (i32)n;
            _moram_ponere(v, v->mora_otii_ms);
            perge;
        }
        si (n == TLS_ITERUM || n == ZEPHYRUM)
        {
            _fossam_ponere(v, REACTOR_SCRIBERE);
            redde;
        }
        _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_IO, chorda_ex_literis(
            "mittere fallita", v->piscina));
        redde;
    }
    _octetos_obliterare(v);
    v->status = HTTP_VOCATIO_LEGENS;
    _fossam_ponere(v, REACTOR_LEGERE);
}

/* legere USQUE AD ITERUM: SecureTransport octetos decryptos tenere
 * potest quos fossa iam non nuntiat */
interior vacuum
_legere (
    HttpVocatio* v)
{
    dum (!v->finita)
    {
        s32 n = v->tls
            ? tls_recipere(v->tls, v->capsa, HTTP_VOCATIO_CAPSA)
            : tcp_recipere(v->tcp, v->capsa, HTTP_VOCATIO_CAPSA);
        HttpLectorStatus st;

        si (n > ZEPHYRUM)
        {
            _moram_ponere(v, v->mora_otii_ms);
            st = http_lector_adicere(v->lector, v->capsa, (i32)n);
            si (v->finita)
            {
                redde;                   /* abruptum intra rivum */
            }
            si (st == HTTP_LECTOR_FINITUS)
            {
                _finire(v, HTTP_VOCATIO_FINITA, HTTP_OK, _vacua());
                redde;
            }
            si (st == HTTP_LECTOR_ERROR)
            {
                _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_PARSE,
                    http_lector_error(v->lector));
                redde;
            }
            perge;
        }
        si (n == TCP_ITERUM)
        {
            redde;                       /* = TLS_ITERUM: exspecta */
        }
        si (n == ZEPHYRUM)
        {
            st = http_lector_finire(v->lector);
            si (st == HTTP_LECTOR_FINITUS)
            {
                _finire(v, HTTP_VOCATIO_FINITA, HTTP_OK, _vacua());
            }
            alioquin
            {
                _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_IO,
                    http_lector_error(v->lector).mensura > ZEPHYRUM
                    ? http_lector_error(v->lector)
                    : chorda_ex_literis("connexio clausa ante responsum",
                        v->piscina));
            }
            redde;
        }
        _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_IO, chorda_ex_literis(
            "error retis inter receptionem", v->piscina));
        redde;
    }
}

interior vacuum
_pactionem_promovere (
    HttpVocatio* v)
{
    commutatio (tls_gradus(v->tls))
    {
        casus TLS_GRADUS_PERFECTUS:
            v->status = HTTP_VOCATIO_MITTENS;
            _moram_ponere(v, v->mora_otii_ms);
            _mittere(v);
            frange;
        casus TLS_GRADUS_LEGERE:
            _fossam_ponere(v, REACTOR_LEGERE);
            frange;
        casus TLS_GRADUS_SCRIBERE:
            _fossam_ponere(v, REACTOR_SCRIBERE);
            frange;
        ordinarius:
            _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO,
                chorda_concatenare(chorda_ex_literis("TLS: ", v->piscina),
                tls_gradus_error(v->tls), v->piscina));
            frange;
    }
}

interior vacuum
_connexionem_perficere (
    HttpVocatio* v)
{
         chorda descr = _vacua();
    TlsResultus tr;

    si (tcp_incipere_perficere(v->tcp, &descr, v->piscina) != TCP_OK)
    {
        v->error_ultimus = descr;
        _connexionem_dimittere(v);
        _connectere_proximum(v);
        redde;
    }
    si (v->est_https)
    {
        tr = tls_incipere(v->tcp, http_petitio_hospes(v->petitio), NIHIL,
            v->piscina);
        si (!tr.successus)
        {
            v->tcp = NIHIL;              /* tls_incipere clausit */
            _finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO,
                tr.error_descriptio);
            redde;
        }
        v->tls     = tr.connexio;
        v->status  = HTTP_VOCATIO_PACTIO;
        _pactionem_promovere(v);
        redde;
    }
    v->status = HTTP_VOCATIO_MITTENS;
    _moram_ponere(v, v->mora_otii_ms);
    _mittere(v);
}

interior vacuum
_fossa_parata (
    integer  fossa,
        i32  eventus,
     vacuum* datum)
{
    HttpVocatio* v = (HttpVocatio*)datum;

    (vacuum)fossa;
    (vacuum)eventus;
    si (v->finita)
    {
        redde;
    }
    commutatio (v->status)
    {
        casus HTTP_VOCATIO_CONNECTIO:
            _connexionem_perficere(v);
            frange;
        casus HTTP_VOCATIO_PACTIO:
            _pactionem_promovere(v);
            frange;
        casus HTTP_VOCATIO_MITTENS:
            _mittere(v);
            frange;
        casus HTTP_VOCATIO_LEGENS:
            _legere(v);
            frange;
        ordinarius:
            frange;
    }
}

HttpVocatio*
http_vocare (
                    Reactor* reactor,
                HttpPetitio* petitio,
                  HttpRivus  rivus,
                     vacuum* rivi_datum,
           HttpVocatioFinis  finis,
                     vacuum* finis_datum,
    constans HttpVocatioOptiones* optiones,
                    Piscina* piscina)
{
    HttpVocatio* v;
         chorda  error = _vacua();
    MoraAngusta  tempus;

    si (!reactor || !petitio || !finis || !piscina)
    {
        redde NIHIL;
    }
    v = (HttpVocatio*)piscina_allocare(piscina, magnitudo(HttpVocatio));
    memset(v, ZEPHYRUM, magnitudo(HttpVocatio));
    v->reactor        = reactor;
    v->piscina        = piscina;
    v->petitio        = petitio;
    v->rivus          = rivus;
    v->rivi_datum     = rivi_datum;
    v->finis          = finis;
    v->finis_datum    = finis_datum;
    v->status         = HTTP_VOCATIO_NOMINA;
    v->fossa          = -I;
    v->error_pendens  = HTTP_OK;
    v->est_https      = http_petitio_est_https(petitio);
    tempus            = http_petitio_tempus(petitio);
    v->mora_connexionis_ms = (optiones && optiones->mora_connexionis_ms
        > ZEPHYRUM) ? optiones->mora_connexionis_ms
        : (tempus > ZEPHYRUM ? (i32)tempus : HTTP_VOCATIO_MORA_CONNEXIONIS);
    v->mora_otii_ms   = (optiones && optiones->mora_otii_ms > ZEPHYRUM)
        ? optiones->mora_otii_ms : HTTP_VOCATIO_MORA_OTII;
    v->octeti         = http_petitio_octeti(petitio);
    v->lector         = http_lector_creare(piscina,
        http_petitio_methodus(petitio), _rivus_custos, v);
    v->capsa          = (i8*)piscina_allocare(piscina,
        (i64)HTTP_VOCATIO_CAPSA);
    _moram_ponere(v, v->mora_connexionis_ms);
    v->quaestio = nomina_quaerere_incipere(reactor, chorda_ex_literis(
        http_petitio_hospes(petitio), piscina), http_petitio_portus(petitio),
        _nomina_finis, v, piscina, &error);
    si (!v->quaestio)
    {
        _errorem_differre(v, HTTP_ERROR_CONNEXIO, error);
    }
    redde v;
}

HttpVocatioStatus
http_vocatio_status (
    constans HttpVocatio* v)
{
    redde v ? v->status : HTTP_VOCATIO_IRRITA;
}

b32
http_vocatio_finita (
    constans HttpVocatio* v)
{
    redde !v || v->finita;
}

HttpResultus
http_vocatio_resultus (
    constans HttpVocatio* v)
{
    HttpResultus r;

    si (!v || !v->finita)
    {
        memset(&r, ZEPHYRUM, magnitudo(r));
        r.error = HTTP_ERROR_IO;
        redde r;
    }
    redde v->resultus;
}

vacuum
http_vocationem_abrumpere (
    HttpVocatio* v)
{
    si (!v || v->finita)
    {
        redde;
    }
    _finire(v, HTTP_VOCATIO_ABRUPTA, HTTP_ERROR_IO, chorda_ex_literis(
        "abruptum", v->piscina));
}
```

(Type checks for the executor: `chorda.mensura`'s exact type in
`include/chorda.h` decides the casts in `_mittere`; `HTTP_OK` is the
zero of `HttpError` - `include/http.h:43`; enum comparisons
`v->status <= HTTP_VOCATIO_PACTIO` rely on the declared order.)

Then `./tools/compile_tests_fontes_generare.sh`.

- [ ] **Step 6: Run it to verify it passes, then the rete file and neighbours**

Run: `./compile_tests.sh probatio_http_vocatio > $S/t5_green.log 2>&1; grep -E 'FRACTA|Totalis|Fracti' $S/t5_green.log; ./compile_tests.sh probatio_http_vocatio_rete > $S/t5_rete.log 2>&1; grep -E 'FRACTA|OMISSA|Totalis|Fracti' $S/t5_rete.log; for t in probatio_http probatio_http_lector probatio_vates probatio_herbarium; do ./compile_tests.sh $t > $S/t5_$t.log 2>&1; echo "$t rc=$?"; done`
Expected: 0 fracti; rete file green with the network up (this is the
first real stepped TLS handshake + gradatim read - if it fails, debug
Task 4 here); neighbours rc=0.

- [ ] **Step 7: Plants** (`filtrum='probatio_http_vocatio'`, `lib/http_vocatio.c`)
  - in `_rivus_custos`, `si (!v->finita && v->rivus)` -> `si (v->rivus)`
    - expect `abrumpere intra rivum` red (rivi 2).
  - in `_legere`, the first `si (st == HTTP_LECTOR_FINITUS)` block's
    `_finire(v, HTTP_VOCATIO_FINITA, HTTP_OK, _vacua());\n                redde;`
    -> `perge;` - expect `manet apertum` red (waits for a close).
  - in `_connexionem_perficere`, `_connectere_proximum(v);\n        redde;`
    -> `_finire(v, HTTP_VOCATIO_IRRITA, HTTP_ERROR_CONNEXIO, descr);\n        redde;`
    - expect `localhost` red when `::1` is tried first (if it stays
    green, `localhost` resolved 127.0.0.1 first on this machine:
    record that, and plant `v->addressus_index = ZEPHYRUM;` ->
    `v->addressus_index = I;` with a two-address literal instead).
  - in `_finire`, delete `_connexionem_dimittere(v);` - expect
    `recusata`/`clausum statim` red (fd left in reactor).
  - in `_mora_expiravit`, `v->status <= HTTP_VOCATIO_PACTIO` ->
    `v->status < HTTP_VOCATIO_PACTIO` - expect `pactio tacita` red.

- [ ] **Step 8: Worklog + commit**

`lib/http_vocatio.worklog.md`: the state machine, why the sink is
guarded, drain-until-ITERUM, remove-before-close (fd numbers are
reused), the "finis must not free the piscina" rule, R7/R8, what the
rete file proved. Commit `include/http.h lib/http.c include/http_vocatio.h
lib/http_vocatio.c lib/http_vocatio.worklog.md probationes/servus_scriptus.h
probationes/probatio_http_vocatio.c probationes/probatio_http_vocatio_rete.c
compile_tests_fontes_generata.sh`.

---

### Task 6: vates - `vates_vocare_rivum`; quaestio shares its request (spec II.5, R6, R9, R10)

**Files:**
- Modify: `include/vates.h` (`VatesOptiones` `:239-258`; new block after
  `vates_mittere_rivum` `:313-319`), `lib/vates.c` (`_petitionem_http`
  `:1994-2024`, `vates_mittere_rivum` `:2230-2333`, new code after it)
- Modify: `include/quaestio.h`, `lib/quaestio.c` (`petitionem_struere`
  `:640-668`, `responsum_reddere` `:672-720`)
- Create: `probationes/probatio_vates_vocatio.c`
- Modify: `lib/vates.worklog.md`, `lib/quaestio.worklog.md`

**Interfaces:**
- Consumes: `http_vocare`, `http_vocatio_resultus`,
  `http_vocationem_abrumpere`, `HttpVocatioFinis` (Task 5); the
  scripted server (Task 5).
- Produces (`include/vates.h`, which now includes `reactor.h`):

```c
/* in VatesOptiones, ultimum campum: */
    constans character*  url;   /* NIHIL = VATES_URL_ANTHROPIC
                                 * (probationes: servus localis, R6) */

nomen structura VatesVocatio VatesVocatio;   /* opacum */
/* SEMEL (etiam post abrumpere, R9); numquam intra vates_vocare_rivum;
 * piscinam vocationis NE destruat */
nomen vacuum (*VatesVocatioFinis)(vacuum* datum, VatesVocatio* vocatio);

/* ut vates_mittere_rivum, NON obstruens: super reactorem vocantis,
 * iteratio solum ante deltam primam, mora iterandi per timer (numquam
 * dormit); responsum IDEM ac via obstruens pro eisdem eventis. Non per
 * vecturam rivi (herbarium, fictus: R10). NIHIL solum si argumenta
 * NIHIL. */
VatesVocatio* vates_vocare_rivum(Vates* vates, VatesPetitio* petitio,
    chorda propositum, Reactor* reactor, VatesRivus rivus, vacuum* datum,
    VatesVocatioFinis finis, vacuum* finis_datum, Piscina* piscina);
VatesResponsum* vates_vocatio_responsum(constans VatesVocatio* vocatio);
    /* NIHIL ante finem */
b32 vates_vocatio_finita(constans VatesVocatio* vocatio);
vacuum vates_vocationem_abrumpere(VatesVocatio* vocatio);
    /* synchronus: finis ante reditum, responsum error RETE "abruptum" */
```

and `include/quaestio.h`:

```c
/* petitio $question communis (rogare, rivus, porta interna): systema
 * formam paginae dicit; exemplar vacua = QUAESTIO_EXEMPLAR */
VatesPetitio* quaestio_petitionem_parare(chorda exemplar,
    chorda quaestio, s32 lineae, s32 latitudo, Piscina* piscina);
/* responsum vatis -> QuaestioResponsum (textus blocorum planatus) */
QuaestioResponsum quaestio_responsum_ex_vate(
    constans VatesResponsum* responsum, Piscina* piscina);
```

- [ ] **Step 1: Write the failing test**

`probationes/probatio_vates_vocatio.c`:

```c
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

#define RESPONSUM_PLENUS \
    "HTTP/1.1 529 Overloaded\r\nContent-Type: application/json\r\n" \
    "Content-Length: 75\r\nConnection: close\r\n\r\n" \
    "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\"," \
    "\"message\":\"Overloaded\"}}"

nomen structura {
                  i32 fines;
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
    i32  mora_iterandi_ms,
    Piscina* p)
{
    VatesOptiones o = vates_optiones_ordinariae();
        character url[CXXVIII];
        character* copia;

    sprintf(url, "http://127.0.0.1:%u/v1/messages",
        (insignatus integer)portus);
    copia = chorda_ut_cstr(chorda_ex_literis(url, p), p);
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
    VatesPetitio* pe = vates_petitio_creare(p, chorda_ex_literis("m", p));
    VatesNuntius* n  = vates_nuntium_addere(pe, VATES_USOR);

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

    g[ZEPHYRUM].octeti    = CAPUT_RIVI EVENTA_PRIMA;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    g[I].octeti           = EVENTA_ULTIMA;
    g[I].mensura          = ZEPHYRUM;
    g[I].mora_ms          = XXX;
    g[I].claudere         = VERUM;
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
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

/* delta, deinde clausura sine message_stop: nulla iteratio */
interior vacuum
_sectio_post_deltam (
    CredoContextus* c)
{
         Reactor* r = reactor_creare(c->piscina);
    ServusGradus  g[I];
  ServusScriptus* s;
      Observatio  o;
    VatesVocatio* vv;
  VatesResponsum* rv;
             s64  pm;

    g[ZEPHYRUM].octeti    = CAPUT_RIVI EVENTA_PRIMA;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    o.fines   = ZEPHYRUM;
    o.deltae  = chorda_aedificator_creare(c->piscina, LXIV);
    vv = vates_vocare_rivum(_vatem(s->portus, ZEPHYRUM, c->piscina),
        _petitio(c->piscina), chorda_ex_literis("proba", c->piscina), r,
        _delta, &o, _finis, &o, c->piscina);
    rv = _currere(r, vv, III * M, &pm);
    CREDO_NECESSE_NON_NIHIL(rv);
    CREDO_AEQUALIS_I32((i32)rv->error, (i32)VATES_ERROR_RETE);
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
    s = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    o.fines   = ZEPHYRUM;
    o.deltae  = chorda_aedificator_creare(c->piscina, LXIV);
    vv = vates_vocare_rivum(_vatem(s->portus, ZEPHYRUM, c->piscina),
        _petitio(c->piscina), chorda_ex_literis("proba", c->piscina), r,
        _delta, &o, _finis, &o, c->piscina);
    initium = _ms_nunc();
    dum (chorda_aedificator_spectare(o.deltae).mensura == ZEPHYRUM
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
```

(The 529 body is 75 bytes - recount it when typing and fix the
Content-Length; `VatesBlocus`, `bloci`, `causa_finis_cruda`, `conatus`
are the `VatesResponsum` fields `lib/quaestio.c:672-720` already uses;
`chorda_aequalis` is in `include/chorda.h`.)

- [ ] **Step 2: Run it to verify it fails**

Run: `./compile_tests.sh probatio_vates_vocatio > $S/t6_red.log 2>&1; tail -5 $S/t6_red.log`
Expected: build failure - `vates_vocare_rivum`, `VatesVocatio`,
`VatesOptiones.url` unknown.

- [ ] **Step 3: Implement - vates**

`include/vates.h`: `#include "reactor.h"`; the `url` field (LAST in
`VatesOptiones`, so `vates_optiones_ordinariae`'s memset defaults it to
NIHIL); the `VatesVocatio` block from Interfaces after
`vates_mittere_rivum`.

`lib/vates.c` `_petitionem_http`:

```c
    HttpPetitio* hp = http_petitio_creare(piscina, HTTP_POST,
        v->optiones.url ? v->optiones.url : VATES_URL_ANTHROPIC);
```

Factor the attempt judgement and bookkeeping out of
`vates_mittere_rivum` (body unchanged, moved):

```c
/* conatus unus rivi iudicatur: responsum et iterandum (communis viae
 * obstruentis et vates_vocare_rivum - ne divergant, http-vocatio II.5) */
interior VatesResponsum*
_rivum_iudicare (
     VatesCollector* c,
      RivusEventuum* lector_eventuum,
       HttpResultus  resultus_http,
                i32  conatus,
            Piscina* piscina,
                b32* iterandum)
{
    VatesResponsum* r;

    si (!resultus_http.successus || !resultus_http.responsum)
    {
        r = _responsum_vacuum(piscina);
        r->error = (resultus_http.error == HTTP_ERROR_TIMEOUT)
            ? VATES_ERROR_TEMPUS : VATES_ERROR_RETE;
        r->error_nuntius = resultus_http.error_descriptio;
        /* post deltam primam numquam iteratur: textus iam visus */
        *iterandum = c->deltae == 0
            && (r->error == VATES_ERROR_RETE || conatus == I);
    }
    alioquin si (resultus_http.responsum->status != CC)
    {
        chorda novitas;

        /* non-2xx: corpus JSON totum, rivus tacuit (R6) */
        r = _legere(resultus_http.responsum->status,
            resultus_http.responsum->corpus, piscina, &novitas);
        *iterandum = _status_iterandus(r->status_http);
    }
    alioquin
    {
        rivus_eventuum_finire(lector_eventuum);
        r = _collectorem_finire(c);
        *iterandum = c->deltae == 0 && c->erratum
            && (chorda_aequalis_literis(c->error_genus,
                "overloaded_error")
                || chorda_aequalis_literis(c->error_genus,
                "api_error"));
    }
    redde r;
}

interior vacuum
_conatum_notare (
              Vates* vates,
       VatesPetitio* petitio,
             chorda  propositum,
                i32  conatus,
     VatesResponsum* r,
                s64  initium,
          VatesUsus* summa,
                b32* pretium_notum,
            Piscina* piscina)
{
    r->usus.mora_ms = _ms_nunc() - initium;
    r->usus.pretium = _pretium(vates,
        r->exemplar.mensura > 0 ? r->exemplar : petitio->exemplar,
        &r->usus);
    _rationarium_scribere(vates, petitio, propositum, conatus, r,
        piscina);
    _usus_addere(summa, &r->usus, pretium_notum);
}

interior vacuum
_responsum_claudere (
    VatesResponsum* r,
          VatesUsus summa,
                b32 pretium_notum,
                i32 conatus)
{
    r->usus = summa;
    si (!pretium_notum)
    {
        r->usus.pretium = -I;
    }
    r->conatus = conatus;
}
```

`vates_mittere_rivum`'s loop body becomes:

```c
        resultus_http = http_vectura_rivi_exsequi(vates->vectura_rivi,
            hp, _rivus_http, lector_eventuum, piscina);
        r = _rivum_iudicare(&c, lector_eventuum, resultus_http, conatus,
            piscina, &iterandum);
        _conatum_notare(vates, petitio, propositum, conatus, r, initium,
            &summa, &pretium_notum, piscina);
        si (!iterandum || conatus >= vates->optiones.conatus_maximi)
        {
            frange;
        }
        _dormire(_mora_iterandi(vates, resultus_http));
    }
    _responsum_claudere(r, summa, pretium_notum, conatus);
    redde r;
```

New code after `vates_mittere_rivum`:

```c
/* ========================================================================
 * VOCATIO PER RIVUM NON OBSTRUENS (http-vocatio II.5)
 * ======================================================================== */

structura VatesVocatio {
                Vates* vates;
         VatesPetitio* petitio;
                chorda propositum;
                chorda corpus;
              Reactor* reactor;
            VatesRivus rivus;
                vacuum* datum;
     VatesVocatioFinis finis;
                vacuum* finis_datum;
              Piscina* piscina;
                   i32 conatus;
                   s64 initium;
             VatesUsus summa;
                   b32 pretium_notum;
        VatesCollector collector;
         RivusEventuum* lector_eventuum;
           HttpVocatio* http;
        ReactorTimerId mora;
                   b32 mora_posita;
        VatesResponsum* responsum;
        VatesResponsum* responsum_pendens;   /* error ante initium */
                   b32 abrumpens;
                   b32 finita;
};

interior vacuum
_vocationem_finire (
     VatesVocatio* vv,
    VatesResponsum* r)
{
    si (vv->finita)
    {
        redde;
    }
    vv->finita = VERUM;
    si (vv->mora_posita)
    {
        (vacuum)reactor_timer_cancellare(vv->reactor, vv->mora);
        vv->mora_posita = FALSUM;
    }
    _responsum_claudere(r, vv->summa, vv->pretium_notum, vv->conatus);
    vv->responsum = r;
    vv->finis(vv->finis_datum, vv);
}

interior vacuum _conatum_incipere (VatesVocatio* vv);

interior vacuum
_iterum (
    vacuum* datum)
{
    VatesVocatio* vv = (VatesVocatio*)datum;

    vv->mora_posita = FALSUM;
    si (vv->finita)
    {
        redde;
    }
    si (vv->responsum_pendens)
    {
        _vocationem_finire(vv, vv->responsum_pendens);
        redde;
    }
    vv->conatus++;
    _conatum_incipere(vv);
}

interior vacuum
_vocationis_http_finis (
         vacuum* datum,
    HttpVocatio* hv)
{
    VatesVocatio* vv = (VatesVocatio*)datum;
    HttpResultus  res;
  VatesResponsum* r;
             b32  iterandum;

    si (vv->finita || vv->abrumpens)
    {
        redde;
    }
    res = http_vocatio_resultus(hv);
    r = _rivum_iudicare(&vv->collector, vv->lector_eventuum, res,
        vv->conatus, vv->piscina, &iterandum);
    _conatum_notare(vv->vates, vv->petitio, vv->propositum, vv->conatus,
        r, vv->initium, &vv->summa, &vv->pretium_notum, vv->piscina);
    si (!iterandum || vv->conatus >= vv->vates->optiones.conatus_maximi)
    {
        _vocationem_finire(vv, r);
        redde;
    }
    /* mora per timer - numquam dormire in reactore */
    vv->mora = reactor_timer_adicere(vv->reactor,
        (i32)_mora_iterandi(vv->vates, res), FALSUM, _iterum, vv);
    vv->mora_posita = (vv->mora != REACTOR_TIMER_ERROR);
}

interior vacuum
_conatum_incipere (
    VatesVocatio* vv)
{
    HttpPetitio* hp = _petitionem_http(vv->vates, vv->petitio,
        vv->corpus, vv->piscina);

    vv->initium = _ms_nunc();
    _collectorem_initiare(&vv->collector, vv->rivus, vv->datum,
        vv->piscina);
    vv->lector_eventuum = rivus_eventuum_creare(vv->piscina,
        _eventum_anthropic, &vv->collector);
    vv->http = http_vocare(vv->reactor, hp, _rivus_http,
        vv->lector_eventuum, _vocationis_http_finis, vv, NIHIL,
        vv->piscina);
}

VatesVocatio*
vates_vocare_rivum (
                Vates* vates,
         VatesPetitio* petitio,
                chorda  propositum,
              Reactor* reactor,
            VatesRivus  rivus,
                vacuum* datum,
     VatesVocatioFinis  finis,
                vacuum* finis_datum,
              Piscina* piscina)
{
    VatesVocatio* vv;

    si (!vates || !petitio || !reactor || !finis || !piscina)
    {
        redde NIHIL;
    }
    vv = (VatesVocatio*)piscina_allocare(piscina, magnitudo(VatesVocatio));
    memset(vv, 0, magnitudo(VatesVocatio));
    vv->vates        = vates;
    vv->petitio      = petitio;
    vv->propositum   = propositum;
    vv->reactor      = reactor;
    vv->rivus        = rivus;
    vv->datum        = datum;
    vv->finis        = finis;
    vv->finis_datum  = finis_datum;
    vv->piscina      = piscina;
    vv->conatus      = I;
    si (_signa_numerare(petitio) > VATES_PUNCTA_THESAURI_MAXIMA)
    {
        VatesResponsum* r = _responsum_vacuum(piscina);

        r->error          = VATES_ERROR_LIMES;
        r->error_nuntius  = chorda_ex_literis(
            "plus quam IV puncta cache (API: IV maxime)", piscina);
        vv->responsum_pendens = r;
        vv->mora = reactor_timer_adicere(reactor, ZEPHYRUM, FALSUM,
            _iterum, vv);
        vv->mora_posita = VERUM;
        redde vv;
    }
    petitio->rivus  = VERUM;
    vv->corpus      = vates_anthropic_corpus(petitio, piscina);
    petitio->rivus  = FALSUM;
    _conatum_incipere(vv);
    redde vv;
}

VatesResponsum*
vates_vocatio_responsum (
    constans VatesVocatio* vv)
{
    redde vv ? vv->responsum : NIHIL;
}

b32
vates_vocatio_finita (
    constans VatesVocatio* vv)
{
    redde !vv || vv->finita;
}

vacuum
vates_vocationem_abrumpere (
    VatesVocatio* vv)
{
    VatesResponsum* r;

    si (!vv || vv->finita)
    {
        redde;
    }
    vv->abrumpens = VERUM;
    si (vv->http)
    {
        http_vocationem_abrumpere(vv->http);
    }
    r                 = _responsum_vacuum(vv->piscina);
    r->error          = VATES_ERROR_RETE;
    r->error_nuntius  = chorda_ex_literis("abruptum", vv->piscina);
    _vocationem_finire(vv, r);
}
```

(`lib/vates.c` gains `#include "http_vocatio.h"`; `_mora_iterandi` is
above `vates_mittere` - it takes an `HttpResultus` by value, as here.)

- [ ] **Step 4: Implement - quaestio**

In `lib/quaestio.c`: rename with
`./silva/renominare.sh petitionem_struere quaestio_petitionem_parare -via lib/quaestio.c -scribere`
and `./silva/renominare.sh responsum_reddere quaestio_responsum_ex_vate -via lib/quaestio.c -scribere`;
drop `interior` from both definitions; add the two prototypes to
`include/quaestio.h` (Interfaces block). No behaviour change.

- [ ] **Step 5: Run it to verify it passes, then neighbours**

Run: `./compile_tests.sh probatio_vates_vocatio > $S/t6_green.log 2>&1; grep -E 'FRACTA|Totalis|Fracti' $S/t6_green.log; for t in probatio_vates probatio_vates_rivus probatio_quaestio probatio_herbarium probatio_norma; do ./compile_tests.sh $t > $S/t6_$t.log 2>&1; echo "$t rc=$?"; done`
Expected: 0 fracti; every neighbour rc=0 (the factoring kept
`vates_mittere_rivum` byte-identical in behaviour - `probatio_vates_rivus`
is its guard).

- [ ] **Step 6: Plants** (`filtrum='probatio_vates_vocatio'`, `lib/vates.c`)
  - in `_vocationis_http_finis`, `(i32)_mora_iterandi(vv->vates, res)` ->
    `(i32)(_mora_iterandi(vv->vates, res) * X)` - expect `iteratio` red
    (no second accept within the 3 s guard).
  - in `_rivum_iudicare`, `*iterandum = c->deltae == 0\n            && (r->error == VATES_ERROR_RETE || conatus == I);`
    -> `*iterandum = (r->error == VATES_ERROR_RETE || conatus == I);`
    - expect `post deltam` red (conatus II).
  - in `vates_vocationem_abrumpere`, `_vocationem_finire(vv, r);` ->
    `(vacuum)r;` - expect `abrumpere` red (fines 0).
  - in `_petitionem_http`, `v->optiones.url ? v->optiones.url : VATES_URL_ANTHROPIC`
    -> `VATES_URL_ANTHROPIC` - expect `par` red (no local server hit).

- [ ] **Step 7: Worklogs + commit**

`lib/vates.worklog.md`: the shared judgement (`_rivum_iudicare`), retry
on a timer, R9, R10 (route B is not captured by herbarium - route A's
`bin/quaestio` did capture, since it never set `sine_herbario`).
`lib/quaestio.worklog.md`: the two public functions. Commit
`include/vates.h lib/vates.c include/quaestio.h lib/quaestio.c
lib/vates.worklog.md lib/quaestio.worklog.md
probationes/probatio_vates_vocatio.c`.

---

### Task 7: vicus port `vicus_quaestio_interna` + wiring + live check (spec II.6, R1, R2, R11)

**Files:**
- Create: `include/vicus_quaestio_interna.h`,
  `lib/vicus_quaestio_interna.c`, `lib/vicus_quaestio_interna.worklog.md`
- Modify: `apps/vicus/vicus.c` (`vicum_pulsare` `:37-43`, locals
  `:80-99`, port wiring `:165-172`, `ludus_fenestra_pulsum_ponere`
  `:201`)
- Create: `probationes/probatio_vicus_quaestio_interna.c`
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: `vates_aperire`, `vates_vocare_rivum`,
  `vates_vocatio_responsum`, `vates_vocationem_abrumpere` (Task 6);
  `quaestio_petitionem_parare`, `quaestio_responsum_ex_vate`,
  `quaestio_textum_planare`; `VicusPortaQuaestionis`,
  `VicusPulsusQuaestionis` (`include/vicus_quaestio.h`).
- Produces:

```c
nomen structura {
    constans character* via_clavis;    /* NIHIL = $HOME/.rhubarb/
                                        * anthropic.clavis */
    constans character* url;           /* NIHIL = API Anthropic */
                   i32  conatus_maximi;    /* 0 = vatis ordinarium */
                   i32  mora_iterandi_ms;  /* 0 = vatis ordinarium */
} VicusQuaestioInternaOptiones;

/* porta $question IN PROCESSU super reactorem hospitis (via B): hospes
 * reactorem in omni pulsu poscit (R1), porta solum manubrium legit.
 * Clavis per petitionem legitur in piscinam vocationis, quae in fine
 * vacatur et destruitur (R2). optiones NIHIL = ordinaria. */
VicusPortaQuaestionis vicus_quaestio_interna(Reactor* reactor,
    constans VicusQuaestioInternaOptiones* optiones, Piscina* piscina);
```

- [ ] **Step 1: Write the failing test**

`probationes/probatio_vicus_quaestio_interna.c` - the route A port
scenarios replayed against the scripted server, with a FAKE key file in
`$TMPDIR`:

```c
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
    VicusQuaestioInternaOptiones o;
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
    chorda x,
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
    VicusPortaQuaestionis porta;
                  vacuum* h;
                  chorda  error;
                   Summa  x;
      constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI INITIUM_RIVI DELTA("Salve");
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    g[I].octeti           = DELTA(" munde.") FINIS_RIVI;
    g[I].mensura          = ZEPHYRUM;
    g[I].mora_ms          = L;
    g[I].claudere         = VERUM;
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
    VicusPortaQuaestionis porta;
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
    VicusPortaQuaestionis porta;
                  vacuum* h;
                  chorda  error;
                   Summa  x;
      constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI INITIUM_RIVI DELTA("Pars");
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta = _portam(r, s->portus, via, c->piscina);
    h = _incipere(&porta, c->piscina, &error);
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
    VicusPortaQuaestionis porta;
                  vacuum* h;
                  chorda  error;
                   Summa  x;
      constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI INITIUM_RIVI ERROR_RIVI;
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta = _portam(r, s->portus, via, c->piscina);
    h = _incipere(&porta, c->piscina, &error);
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
    VicusPortaQuaestionis porta;
                  vacuum* h;
                  chorda  error;
                   Summa  x;
      constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI "non est rivus\n\n";
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = VERUM;
    s = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta = _portam(r, s->portus, via, c->piscina);
    h = _incipere(&porta, c->piscina, &error);
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
    VicusPortaQuaestionis porta;
                  vacuum* h;
                  chorda  error;
                   Summa  x;
                     s64  initium;
      constans character* via = _clavem_scribere(c->piscina);

    g[ZEPHYRUM].octeti    = CAPUT_RIVI INITIUM_RIVI DELTA("Prima");
    g[ZEPHYRUM].mensura   = ZEPHYRUM;
    g[ZEPHYRUM].mora_ms   = ZEPHYRUM;
    g[ZEPHYRUM].claudere  = FALSUM;
    s = servum_scriptum_creare(r, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    porta = _portam(r, s->portus, via, c->piscina);
    h = _incipere(&porta, c->piscina, &error);
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
```

(`chorda_continet` - check its exact name in `include/chorda.h`; the
macro `CREDO_CHORDA_CONTINET` uses it. A test writing to `$TMPDIR/<name>.<pid>`
needs no `<debitum_scripturae>` - memory `fabrica-project`.)

- [ ] **Step 2: Run it to verify it fails**

Run: `./compile_tests.sh probatio_vicus_quaestio_interna > $S/t7_red.log 2>&1; tail -5 $S/t7_red.log`
Expected: build failure - `vicus_quaestio_interna.h` not found.

- [ ] **Step 3: Implement the port**

`include/vicus_quaestio_interna.h`: header comment (Interfaces text),
includes `latina.h piscina.h chorda.h reactor.h vicus_quaestio.h`, the
options struct and the prototype.

`lib/vicus_quaestio_interna.c`:

```c
/* vicus_quaestio_interna.c - porta $question in processu (via B; vide
 * caput, http-vocatio-spec II.6) */
#include "postulata_posix.h"
#include "vicus_quaestio_interna.h"
#include "vates.h"
#include "quaestio.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
                         Reactor* reactor;
    VicusQuaestioInternaOptiones  optiones;   /* chordae copiatae */
} PortaInterna;

nomen structura {
               Piscina* piscina;            /* vici: manubrium, textus */
               Piscina* piscina_vocationis; /* clavis intus: vacatur et
                                             * destruitur (R2) */
          VatesVocatio* vocatio;
     ChordaAedificator* crudum;
                   b32  mutatum;
                   b32  vocatio_finita;     /* finis vatis visus */
                   b32  finitus;            /* vico nuntiatum */
                chorda  error;
                   s64  input;
                   s64  output;
} ManubriumInternum;

interior constans character*
_copia (
    constans character* s,
               Piscina* p)
{
    character* c;

    si (!s)
    {
        redde NIHIL;
    }
    c = (character*)piscina_allocare(p, (memoriae_index)strlen(s) + I);
    strcpy(c, s);
    redde c;
}

/* piscina vocationis: VACARE (omnia blocka zephyrantur - clavis in
 * Vates, capitibus, octetis) deinde destruere. Numquam intra finem
 * vatis (codex post reditum currit) - solum ex pulsare/abrumpere. */
interior vacuum
_vocationem_dimittere (
    ManubriumInternum* h)
{
    si (h->piscina_vocationis)
    {
        piscina_vacare(h->piscina_vocationis);
        piscina_destruere(h->piscina_vocationis);
        h->piscina_vocationis = NIHIL;
    }
    h->vocatio = NIHIL;
}

/* clavis ex via (NIHIL: $HOME/.rhubarb/anthropic.clavis), spatia
 * utrimque tolluntur (ut apps/quaestio/quaestio.c); capsa localis
 * zephyratur; chorda vacua + *error si nequit */
interior chorda
_clavem_legere (
    constans character* via,
               Piscina* p_clavis,
               Piscina* p_erroris,
                chorda* error)
{
    constans character* domus;
             character  semita[DXII];
             character  capsa[DXII];
                  FILE* f;
                size_t  n;
                size_t  a;
                chorda  clavis;

    clavis.datum    = NIHIL;
    clavis.mensura  = ZEPHYRUM;
    si (!via)
    {
        domus = getenv("HOME");
        si (!domus || strlen(domus) > CD)
        {
            *error = chorda_ex_literis("HOME absent", p_erroris);
            redde clavis;
        }
        sprintf(semita, "%s/.rhubarb/anthropic.clavis", domus);
        via = semita;
    }
    f = fopen(via, "rb");
    si (!f)
    {
        *error = chorda_ex_literis(
            "no API key (~/.rhubarb/anthropic.clavis)", p_erroris);
        redde clavis;
    }
    n = fread(capsa, I, magnitudo(capsa) - I, f);
    (vacuum)fclose(f);
    a = ZEPHYRUM;
    dum (a < n && (capsa[a] == ' ' || capsa[a] == '\n'
        || capsa[a] == '\r' || capsa[a] == '\t'))
    {
        a++;
    }
    dum (n > a && (capsa[n - I] == ' ' || capsa[n - I] == '\n'
        || capsa[n - I] == '\r' || capsa[n - I] == '\t'))
    {
        n--;
    }
    si (n > a)
    {
        clavis.datum = (i8*)piscina_allocare(p_clavis,
            (memoriae_index)(n - a));
        memcpy(clavis.datum, capsa + a, n - a);
        clavis.mensura = (i32)(n - a);
    }
    alioquin
    {
        *error = chorda_ex_literis("API key file empty", p_erroris);
    }
    memset(capsa, ZEPHYRUM, magnitudo(capsa));
    redde clavis;
}

interior vacuum
_delta (
    vacuum* datum,
    chorda  delta,
       b32  cogitatio)
{
    ManubriumInternum* h = (ManubriumInternum*)datum;

    si (!cogitatio)
    {
        chorda_aedificator_appendere_chorda(h->crudum, delta);
        h->mutatum = VERUM;
    }
}

/* finis vatis: copiae in piscinam VICI (vocationis mox destruitur) */
interior vacuum
_finis (
          vacuum* datum,
    VatesVocatio* vocatio)
{
    ManubriumInternum* h = (ManubriumInternum*)datum;
    QuaestioResponsum  q = quaestio_responsum_ex_vate(
        vates_vocatio_responsum(vocatio), h->piscina);

    h->vocatio_finita = VERUM;
    si (q.successus)
    {
        h->input   = q.usus.input;
        h->output  = q.usus.output;
    }
    alioquin
    {
        h->error = q.error;
    }
}

interior vacuum*
_incipere (
     vacuum* ctx,
     chorda  quaestio,
        s32  lineae,
        s32  latitudo,
    Piscina* piscina,
     chorda* error)
{
         PortaInterna* pi = (PortaInterna*)ctx;
    ManubriumInternum* h;
                chorda clavis;
                Vates* vates;
         VatesOptiones optiones;
                chorda vacua;

    h = (ManubriumInternum*)piscina_allocare(piscina,
        magnitudo(ManubriumInternum));
    memset(h, ZEPHYRUM, magnitudo(ManubriumInternum));
    h->piscina             = piscina;
    h->crudum              = chorda_aedificator_creare(piscina, D);
    h->piscina_vocationis  = piscina_generare_dynamicum("quaestio-interna",
        LXIV * M);
    si (!h->piscina_vocationis)
    {
        *error = chorda_ex_literis("out of memory", piscina);
        redde NIHIL;
    }
    clavis = _clavem_legere(pi->optiones.via_clavis,
        h->piscina_vocationis, piscina, error);
    si (clavis.mensura == ZEPHYRUM)
    {
        _vocationem_dimittere(h);
        redde NIHIL;
    }
    optiones                   = vates_optiones_ordinariae();
    optiones.url               = pi->optiones.url;
    optiones.sine_herbario     = VERUM;    /* R10 */
    optiones.conatus_maximi    = pi->optiones.conatus_maximi;
    optiones.mora_iterandi_ms  = pi->optiones.mora_iterandi_ms;
    vates = vates_aperire(h->piscina_vocationis, chorda_ex_literis(
        "anthropic", h->piscina_vocationis), clavis, &optiones);
    /* vates copiam suam unam tenet; nostra zephyratur (R2) */
    memset(clavis.datum, ZEPHYRUM, (size_t)clavis.mensura);
    si (!vates)
    {
        _vocationem_dimittere(h);
        *error = chorda_ex_literis("vates anthropic not opened", piscina);
        redde NIHIL;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    h->vocatio = vates_vocare_rivum(vates, quaestio_petitionem_parare(
        vacua, quaestio, lineae, latitudo, h->piscina_vocationis),
        chorda_ex_literis("quaestio", h->piscina_vocationis), pi->reactor,
        _delta, h, _finis, h, h->piscina_vocationis);
    si (!h->vocatio)
    {
        _vocationem_dimittere(h);
        *error = chorda_ex_literis("question not started", piscina);
        redde NIHIL;
    }
    redde h;
}

/* R1: hospes reactorem iam poscit; hic manubrium solum legitur */
interior VicusPulsusQuaestionis
_pulsare (
     vacuum* manubrium,
    Piscina* piscina)
{
         ManubriumInternum* h = (ManubriumInternum*)manubrium;
    VicusPulsusQuaestionis  r;

    memset(&r, ZEPHYRUM, magnitudo(r));
    si (h->finitus)
    {
        r.finitus = VERUM;
        redde r;
    }
    si (h->mutatum)
    {
        h->mutatum  = FALSUM;
        r.mutatum   = VERUM;
        r.textus    = quaestio_textum_planare(chorda_aedificator_spectare(
            h->crudum), piscina);
    }
    si (h->vocatio_finita)
    {
        r.finitus   = VERUM;
        r.error     = h->error;
        r.input     = h->input;
        r.output    = h->output;
        h->finitus  = VERUM;
        _vocationem_dimittere(h);
    }
    redde r;
}

interior vacuum
_abrumpere (
    vacuum* manubrium)
{
    ManubriumInternum* h = (ManubriumInternum*)manubrium;

    si (h->finitus)
    {
        redde;
    }
    si (h->vocatio && !h->vocatio_finita)
    {
        vates_vocationem_abrumpere(h->vocatio);   /* finis synchronus */
    }
    h->finitus = VERUM;
    _vocationem_dimittere(h);
}

VicusPortaQuaestionis
vicus_quaestio_interna (
                          Reactor* reactor,
    constans VicusQuaestioInternaOptiones* optiones,
                          Piscina* piscina)
{
    VicusPortaQuaestionis porta;
            PortaInterna* pi;

    memset(&porta, ZEPHYRUM, magnitudo(porta));
    pi = (reactor && piscina) ? (PortaInterna*)piscina_allocare(piscina,
        magnitudo(PortaInterna)) : NIHIL;
    si (!pi)
    {
        redde porta;            /* incipere NIHIL: porta nulla */
    }
    memset(pi, ZEPHYRUM, magnitudo(PortaInterna));
    pi->reactor = reactor;
    si (optiones)
    {
        pi->optiones                   = *optiones;
        pi->optiones.via_clavis        = _copia(optiones->via_clavis,
            piscina);
        pi->optiones.url               = _copia(optiones->url, piscina);
    }
    porta.incipere   = _incipere;
    porta.pulsare    = _pulsare;
    porta.abrumpere  = _abrumpere;
    porta.ctx        = pi;
    redde porta;
}
```

(The error a user sees on abort is produced by vicus itself
(`[cancelled]`, quaestio-rivus T8) - the "abruptum" responsum never
reaches the page because `finitus` is set before the next pulse.)

Then `./tools/compile_tests_fontes_generare.sh`.

- [ ] **Step 4: Run it to verify it passes**

Run: `./compile_tests.sh probatio_vicus_quaestio_interna > $S/t7_green.log 2>&1; grep -E 'FRACTA|Totalis|Fracti' $S/t7_green.log`
Expected: 0 fracti.

- [ ] **Step 5: Wire vicus**

`apps/vicus/vicus.c`:

```c
#include "reactor.h"
#include "vicus_quaestio_interna.h"

/* hospes reactorem SOLUS possidet et in omni pulsu poscit (http-vocatio
 * R1); nihil inscriptum = nihil agit */
nomen structura {
      Vicus* vicus;
    Reactor* reactor;
} PulsusVici;

interior b32
vicum_pulsare (
    vacuum* ctx)
{
    PulsusVici* pv = (PulsusVici*)ctx;

    (vacuum)reactor_poll(pv->reactor, ZEPHYRUM);
    redde vicus_pulsare(pv->vicus);
}
```

locals: `PulsusVici pulsus;` `Reactor* reactor;` `b32 per_adiutorem;`.
After the piscina is created: `reactor = reactor_creare(piscina);`. The
port wiring (`:165-172`) becomes:

```c
    /* $question: in processu (via B, http-vocatio) nisi -quaestio
     * adiutor (via A: bin/quaestio per processum, R11) */
    per_adiutorem = FALSUM;
    per (k = I; k + I < (s32)argc; k++)
    {
        si (   strcmp(argv[k], "-quaestio") == ZEPHYRUM
            && strcmp(argv[k + I], "adiutor") == ZEPHYRUM)
        {
            per_adiutorem = VERUM;
        }
    }
    porta_quaestionis = per_adiutorem
        ? vicus_quaestio_porta(vicus_quaestio_adiutoris_via(
              argc > ZEPHYRUM ? argv[ZEPHYRUM] : NIHIL, piscina),
              ZEPHYRUM, piscina)
        : vicus_quaestio_interna(reactor, NIHIL, piscina);
    vicus_applicatio_quaestionis_portam_ponere(&app,
        &porta_quaestionis);
```

and the pulse registration (`:201`):

```c
    pulsus.vicus    = app.vicus;
    pulsus.reactor  = reactor;
    ludus_fenestra_pulsum_ponere(lf, vicum_pulsare, &pulsus);
```

Update the file's top comment: `-quaestio adiutor`. Build without
launching: `AEDIFICARE_SOLUM=1 ./apps/vicus/vicus.sh > $S/t7_vicus.log 2>&1; echo rc=$?`
(NEVER without `AEDIFICARE_SOLUM` - the script ends in `exec bin/vicus`.)

- [ ] **Step 6: Neighbours**

Run: `for t in probatio_vicus_quaestio probatio_vicus_quaestio_porta probatio_vicus_iussa probatio_vicus probatio_quaestio; do ./compile_tests.sh $t > $S/t7_$t.log 2>&1; echo "$t rc=$?"; done; ./ludus_tessera/compile_probationes.sh > $S/t7_lt.log 2>&1; echo lt rc=$?`
Expected: all rc=0. If ludus_tessera fails at link with a missing
symbol from a new lib file, add the file to its hand list (memory
`worktree-secunda`) and rerun.

- [ ] **Step 7: Plants** (`filtrum='probatio_vicus_quaestio_interna'`,
  `lib/vicus_quaestio_interna.c`)
  - in `_finis`, `h->error = q.error;` -> `;` - expect `error`/`mors` red.
  - in `_abrumpere`, `vates_vocationem_abrumpere(h->vocatio);` -> `;`
    - expect `lente` red (fd left in reactor).
  - in `_delta`, `si (!cogitatio)` -> `si (cogitatio)` - expect
    `bonum` red (no text reaches the page).

- [ ] **Step 8: Fran's live check (GUI - ask first)**

Ask Fran to run `bin/vicus` from `../rhubarb-quarta` (route B) and ask
a `$question`: steady pieces, page locked, Esc cancels, page-full stop,
one undo step, no stall while connecting. Then `bin/vicus -quaestio
adiutor` once (route A still works). Claude never runs it with the
real key.

- [ ] **Step 9: Worklog, memory, ledger, commit**

`lib/vicus_quaestio_interna.worklog.md`: R1, R2 (vacate + destroy),
"never release inside finis", the route A/B difference in herbarium
capture (R10). Commit `include/vicus_quaestio_interna.h
lib/vicus_quaestio_interna.c lib/vicus_quaestio_interna.worklog.md
apps/vicus/vicus.c probationes/probatio_vicus_quaestio_interna.c
compile_tests_fontes_generata.sh`. Ledger: wish ...7EHS -> impletum with
a note naming the commits; memory `quaestio-rivus.md` gets the route B
line. Then the merge ritual (merge main, frigida, ff, heal in main -
ask Fran first if another tree is merging).

---

## Self-review (done while writing)

**Spec coverage.** II.0 -> Task 1; II.1 -> Task 2; II.2 -> Task 3; II.3
-> Task 4; II.4 + III scripted server + the opt-in network check ->
Task 5; II.5 -> Task 6; II.6 + wiring + live check -> Task 7. §IV
non-goals: none implemented. AUDIENDA: item 1 (reactor hazards) becomes
Task 1's red tests; item 2 (`localhost` offline) is checked by Task 2
`localhost`; item 3 (examen + `dns_sd.h`) avoided by the `.m` file, and
the TCP names get lexicon entries in Task 3; item 4 (SecureTransport
mid-record) is exercised only by the rete file (Task 5) and the live
check; items 5-6 (ping interval, idle poll cost) are not tested -
recorded in the worklogs.

**Placeholder scan.** No TBD/TODO. Three places say "check the exact
name/type" (CredoSectio field count, `chorda.mensura` type,
`chorda_continet`): each names the file to check and the fallback.

**Type consistency.** `NominaAddressus.octeti/mensura/familia` (T2) ==
`tcp_incipere(constans i8*, i32, Piscina*)` arguments (T3, T5).
`TLS_ITERUM == TCP_ITERUM == -2` (T4) is what `_legere` compares (T5).
`HttpVocatioFinis(vacuum*, HttpVocatio*)` (T5) == vates'
`_vocationis_http_finis` (T6). `VatesVocatioFinis(vacuum*, VatesVocatio*)`
(T6) == the port's `_finis` (T7). `quaestio_petitionem_parare(exemplar,
quaestio, lineae, latitudo, piscina)` (T6) == the port's call (T7).
`ServusScriptus.portus/petitio/acceptae/index` used in T5-T7 are the
struct fields in T5 Step 1.

**Review focus.** Five lines, each with a named section in Task 5.

**Known weak spots.** `timer natus` (T1) is red on old code only when
the new timer lands in the same millisecond (likely, not certain).
`locus reusus` (T1) passes on old code by accident and is guarded by a
plant instead. Task 4 cannot plant three behaviours without a TLS
server; the rete file and the live check carry them.
