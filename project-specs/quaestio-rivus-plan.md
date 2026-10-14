# quaestio-rivus plan - streaming $question answers (route A)

> **For agentic workers:** execution mode is FIXED by Fran's standing
> rule (memory `plan-execution-inline-checkpoints`): inline, one task per
> turn, Fran approves each before the next; no subagents. GUI steps end
> with Fran's own look before the commit. Steps use checkbox (`- [ ]`)
> syntax for tracking.

**Goal:** `$question` answers grow on the scriba page as they arrive,
scriba never blocks, Esc cancels, and the finished page equals what the
non-streamed path produces today.

**Architecture:** a transport-free core of incremental parsers
(`http_lector` in `http.c`, `rivus_eventuum` for SSE, the Anthropic
stream decoder in `vates.c`) that route B will reuse unchanged; a
streaming transport seam `HttpVecturaRivi` beside `HttpVectura`
(ordinary, herbarium capture/replay, fictus); `vates_mittere_rivum`
returning the same assembled `VatesResponsum` as `vates_mittere`; a
helper `bin/quaestio` writing length-prefixed frames; vicus pumping it
per frame through `processus`, and scriba running any long command
through a new `IussumOpus` (lock, snapshot, re-render, Esc, one
commit).

**Tech stack:** C89 + latina.h; house `http`, `tls`, `tcp`, `json`,
`chorda_aedificator`, `xar`, `herbarium`, `vates`, `processus`,
`iussum`, `scriba_*`, `vicus_applicatio`; credo v2.

**Spec:** `project-specs/quaestio-rivus-spec.md` (quarta 75ca3ed4).
The spec is the authority; deviations are ledgered as rulings below.

## Global constraints

- Flags `tools/vexilla.sh` (C89, -Werror, -Wconversion,
  -Wsign-conversion, -Wcast-qual, -Wstrict-prototypes,
  -Wmissing-prototypes, -Wwrite-strings).
- Latin identifiers; NEVER a latina.h macro word (`nomen per casus
  duplex magnitudo registrum interior ordinarius sponte ...`) nor a
  single Roman numeral letter (`I V X L C D M`) as an identifier;
  compile-check every new header in a TU.
- `chorda` not NUL-terminated; `i32` UNSIGNED, `s32`/`s64` signed.
- POSIX `.c`: `postulata_posix.h` FIRST.
- The API key: read only by `bin/quaestio` from
  `$HOME/.rhubarb/anthropic.clavis`; never argv, never stdout, never a
  frame, never an error text; buffer zeroed after `vates_aperire`.
  Claude never reads or prints the key file.
- New suites: credo v2 (`CredoSectio` table, `credo_suitam_currere`).
- New `lib/*.c` -> regenerate `compile_tests_fontes_generata.sh`
  (`./tools/compile_tests_fontes_generare.sh`) in the same commit; a lib
  that vicus links gaining a NEW lib dependency -> also ludus_tessera's
  hand list (run `./ludus_tessera/compile_probationes.sh`).
- Every behaviour born red; plants via `silva.planta()`; restore with
  plain cp + touch.
- Worktree commits: absolute-path guard (`os.chdir(Q);
  sys.path.insert(0, Q+'/pythonica'); assert
  silva.__file__.startswith(Q+'/')`), commissio adds owed gates itself,
  freeze the tree while gates run, never `~/.bin` installs.
- Latin lint before commit:
  `VOCABULA_VIAE_ADDITAE="<files>" ./oratio/vocabula.sh -nova`.

## Rulings made while planning (spec deviations, ledgered here)

- **R1 `http_lector` lives in `http.h` / `http.c`**, not a new
  `http_lector.h`: it reuses `http.c`'s interior header parsers
  (`_parse_status_line`, `_parse_header`, `_hex_ad_valor`,
  `_http_corpus_prohibitum`) and aedilis maps `include/http.h ->
  lib/http.c`. Cost if wrong: a later file split.
- **R2 lector oracle = literal expectations** (status, headers, body)
  at every split, not "same as http_exsequi's parse": that parse is
  inline in `http_exsequi` (`lib/http.c:900-1080`) and not callable.
  The same fixture bytes also go through `http_exsequi` over the
  existing raw-TCP fixture in T2, which closes the gap.
- **R3 the SSE library is `rivus_eventuum`** (no `sse_` prefix: not a
  Latin word, the lint would refuse it).
- **R4 vicus is not installed** (it runs from the tree via
  `apps/vicus/vicus.sh`; `aedificatio.stml` has no `institutio_vicus`).
  The spec's "~/.bin once installed" line does not apply: `vicus.sh`
  builds `bin/quaestio` next to `bin/vicus`, and vicus resolves the
  helper as `dirname(argv[0]) + "/quaestio"` (no platform API; falls
  back to `quaestio` on PATH via `execvp`).
- **R5 herbarium judges a stream AFTER it ends** (herbarium-spec-2 says
  "before the consumer sees a byte", which a stream cannot honour).
  `_iudex_anthropic` returns "" for `Content-Type: text/event-stream`
  in v1 (norma for streams = follow-up, spec V); non-2xx stream
  responses are plain JSON and are judged as today.
- **R6 the streaming transport ALWAYS returns the whole decoded body**
  too (`responsum->corpus`), and calls the sink only for 2xx. Then
  errors, retries and herbarium reuse the existing code unchanged.
  Bodies are capped at `HTTP_LECTOR_CORPUS_MAXIMUM` (16 MB).
- **R7 `IussumOpusStatus` has a third value `IUSSUM_OPUS_IRRITUM`**
  ("ended with nothing to insert": error before any text, spec III.5)
  - scriba restores the snapshot and commits nothing.
- **R8 prompt line count = free lines - 1** (`responsum_disponere`
  puts `<answer>` alone on the first line), text width = page width
  minus `2 * QUAESTIO_MARGO` (or the page width when that is < 20,
  matching `responsum_disponere`'s own rule).

## Review focus

1. **A chunk boundary, CRLF or SSE line end split across two reads.**
   Every byte position. T1 and T3 tests feed whole, byte by byte, and
   at every 2-way split; T5 frames likewise.
2. **A stream that dies mid-answer** (network drop, helper killed, no
   `message_stop` / no `F` frame). Expected: what arrived stays,
   `[error: ...]` line, status says why, never a hang. T4
   `rivus truncatus`, T5 frame `PLURA` at EOF, T8 `helper sine F`.
3. **Overloaded / 5xx before the first token** must retry exactly as
   the blocking path does; after the first token it must NOT retry
   (text already on screen). T4 sections `iteratio ante deltam` and
   `nulla iteratio post deltam`.
4. **Esc while the helper is mid-write** - the frame reader must cope
   with a half-written frame and `processus_abrumpere`. T8 `esc`.
5. **The finished page differs from the non-streamed page** for the
   same text (the whole point of re-planning). T8 `aequale` compares
   the streamed result with `responsum_aptare(quaestio_textum_planare
   (text))` on the same page.

---

### Task 1: `http_lector` - incremental HTTP response parser

**Files:** Modify `include/http.h` (new section after `HttpResultus`),
`lib/http.c` (new section after `_decodificare_chunked`); Create
`probationes/probatio_http_lector.c`; append `lib/http.worklog.md`.

**Interfaces:** Produces (in `include/http.h`):

```c
/* ========================================================================
 * LECTOR RESPONSI - incrementalis (quaestio-rivus-spec II.1)
 * ========================================================================
 * Octeti intrant, eventa exeunt; nullum I/O. Corpus decodificatum
 * (Content-Length, chunked, aut usque ad clausuram) per rivum exit SOLUM
 * si status 2xx; corpus TOTUM semper colligitur (responsum->corpus,
 * limes HTTP_LECTOR_CORPUS_MAXIMUM). Fines octetorum ubicumque cadere
 * possunt. Octeti post FINITUM neglecti. */

nomen vacuum (*HttpRivus)(vacuum* datum, constans i8* octeti, i32 n);

nomen enumeratio {
    HTTP_LECTOR_CAPITA = 0,   /* capita nondum integra */
    HTTP_LECTOR_CORPUS,       /* status notus, corpus currit */
    HTTP_LECTOR_FINITUS,
    HTTP_LECTOR_ERROR
} HttpLectorStatus;

nomen structura HttpLector HttpLector;   /* opacus */

#define HTTP_LECTOR_CAPITA_MAXIMA   (LXIV * M)        /* octeti capitum */
#define HTTP_LECTOR_CORPUS_MAXIMUM  (XVI * M * M)     /* octeti corporis */

/* methodus HTTP_HEAD: nullum corpus. rivus NIHIL = colligere solum */
HttpLector*
http_lector_creare (
        Piscina* piscina,
    HttpMethodus methodus,
       HttpRivus rivus,
         vacuum* rivi_datum);

HttpLectorStatus
http_lector_adicere (
      HttpLector* lector,
    constans i8* octeti,
             i32  n);

/* EOF connexionis: corpus usque ad clausuram FINITUM; ceterum nondum
 * finitum -> ERROR "truncatum" */
HttpLectorStatus
http_lector_finire (
    HttpLector* lector);

HttpLectorStatus
http_lector_status (
    constans HttpLector* lector);

/* NIHIL ante HTTP_LECTOR_CORPUS; corpus = collectum hactenus */
HttpResponsum*
http_lector_responsum (
    HttpLector* lector);

/* causa si ERROR, aliter vacua */
chorda
http_lector_error (
    constans HttpLector* lector);
```

- [ ] **Step 1.1: failing test** - create
  `probationes/probatio_http_lector.c`:

```c
/* probatio_http_lector.c - lector responsi HTTP incrementalis
 * (quaestio-rivus-spec II.1)
 *
 * Casus quisque TOTUS, OCTETO SINGULO, et ad omnem divisionem binam
 * adicitur: status, caput, corpus et quod rivus accepit eadem esse
 * debent. Longitudo, chunked (extensio, postrema), clausura, non-2xx
 * (rivus tacet), HEAD/204 (nullum corpus), prava (hex, truncatum,
 * capita nimia), octeti post finem neglecti.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "http.h"
#include <string.h>

nomen structura {
    ChordaAedificator* aed;
                  i32  vocationes;
} Collector;

interior vacuum
_colligere (
          vacuum* datum,
    constans i8* octeti,
             i32  n)
{
    Collector* c = (Collector*)datum;
    i32 k;

    c->vocationes++;
    per (k = ZEPHYRUM; k < n; k++)
    {
        chorda_aedificator_appendere_character(c->aed,
            (character)octeti[k]);
    }
}

/* modus: 0 totus, 1 octetus singulus, k > 1 = divisio ad k */
interior HttpLector*
_currere (
    constans character* crudum,
                   s32  modus,
          HttpMethodus  methodus,
                   b32  finire,
             Collector* c,
              Piscina* p)
{
    HttpLector* l;
           i32  n = (i32)strlen(crudum);
           i32  k;

    c->aed         = chorda_aedificator_creare(p, LXIV);
    c->vocationes  = ZEPHYRUM;
    l = http_lector_creare(p, methodus, _colligere, c);
    si (modus == ZEPHYRUM)
    {
        (vacuum)http_lector_adicere(l, (constans i8*)crudum, n);
    }
    alioquin si (modus == I)
    {
        per (k = ZEPHYRUM; k < n; k++)
        {
            (vacuum)http_lector_adicere(l, (constans i8*)crudum + k, I);
        }
    }
    alioquin
    {
        (vacuum)http_lector_adicere(l, (constans i8*)crudum,
            (i32)modus);
        (vacuum)http_lector_adicere(l, (constans i8*)crudum + modus,
            n - (i32)modus);
    }
    si (finire)
    {
        (vacuum)http_lector_finire(l);
    }
    redde l;
}

/* omnes divisiones: status finalis, status HTTP, corpus, rivus */
interior vacuum
_omnes (
     CredoContextus* c,
 constans character* crudum,
        HttpMethodus methodus,
                 b32 finire,
    HttpLectorStatus status_exspectatus,
                 i32 status_http,
 constans character* corpus,
                 b32 rivus_loquitur)
{
    s32 modus;
    s32 n = (s32)strlen(crudum);
    i32 mala = ZEPHYRUM;

    per (modus = ZEPHYRUM; modus < n; modus++)
    {
        Collector col;
        HttpLector* l = _currere(crudum, modus, methodus, finire, &col,
            c->piscina);
        HttpResponsum* r = http_lector_responsum(l);
        chorda rivus = chorda_aedificator_finire(col.aed);

        si (http_lector_status(l) != status_exspectatus)
        {
            mala++;
            perge;
        }
        si (status_exspectatus == HTTP_LECTOR_ERROR)
        {
            perge;
        }
        si (   !r || r->status != status_http
            || !chorda_aequalis_literis(r->corpus, corpus)
            || (rivus_loquitur
                ? !chorda_aequalis_literis(rivus, corpus)
                : (col.vocationes != ZEPHYRUM)))
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
}

interior vacuum
_sectio_longitudo (
    CredoContextus* c)
{
    Collector   col;
    HttpLector* l;

    _omnes(c, "HTTP/1.1 200 OK\r\nContent-Length: 11\r\nX-Nota: b\r\n"
        "\r\nsalve munde", HTTP_POST, FALSUM, HTTP_LECTOR_FINITUS, CC,
        "salve munde", VERUM);
    /* caput legitur; octeti post finem neglecti */
    l = _currere("HTTP/1.1 200 OK\r\nContent-Length: 2\r\nX-Nota: b\r\n"
        "\r\nokXYZ", ZEPHYRUM, HTTP_GET, FALSUM, &col, c->piscina);
    CREDO_AEQUALIS_I32((i32)http_lector_status(l),
        (i32)HTTP_LECTOR_FINITUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(http_responsum_caput(
        http_lector_responsum(l), "X-Nota"), "b");
    CREDO_CHORDA_AEQUALIS_LITERIS(http_lector_responsum(l)->corpus,
        "ok");
}

interior vacuum
_sectio_chunked (
    CredoContextus* c)
{
    _omnes(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "5;nomen=v\r\nsalve\r\n6\r\n munde\r\n0\r\nX-Postrema: q\r\n"
        "\r\n", HTTP_POST, FALSUM, HTTP_LECTOR_FINITUS, CC,
        "salve munde", VERUM);
    /* magnitudo hex maiuscula et minuscula */
    _omnes(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "A\r\n0123456789\r\nb\r\nabcdefghijk\r\n0\r\n\r\n", HTTP_GET,
        FALSUM, HTTP_LECTOR_FINITUS, CC, "0123456789abcdefghijk",
        VERUM);
}

interior vacuum
_sectio_clausura (
    CredoContextus* c)
{
    /* sine longitudine: corpus usque ad EOF */
    _omnes(c, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n"
        "usque ad finem", HTTP_GET, VERUM, HTTP_LECTOR_FINITUS, CC,
        "usque ad finem", VERUM);
}

interior vacuum
_sectio_non_2xx (
    CredoContextus* c)
{
    /* corpus colligitur, rivus tacet */
    _omnes(c, "HTTP/1.1 529 Overloaded\r\nContent-Length: 15\r\n\r\n"
        "{\"error\":\"o\"}xx", HTTP_POST, FALSUM, HTTP_LECTOR_FINITUS,
        DXXIX, "{\"error\":\"o\"}xx", FALSUM);
}

interior vacuum
_sectio_sine_corpore (
    CredoContextus* c)
{
    _omnes(c, "HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\n", HTTP_HEAD,
        FALSUM, HTTP_LECTOR_FINITUS, CC, "", FALSUM);
    _omnes(c, "HTTP/1.1 204 No Content\r\n\r\n", HTTP_GET, FALSUM,
        HTTP_LECTOR_FINITUS, CCIV, "", FALSUM);
}

interior vacuum
_sectio_prava (
    CredoContextus* c)
{
    Collector   col;
    HttpLector* l;
    character*  ingens;
    i32         k;

    /* hex pravum */
    _omnes(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "zz\r\nsalve\r\n", HTTP_GET, FALSUM, HTTP_LECTOR_ERROR, CC, "",
        FALSUM);
    /* chunked truncatum + EOF */
    _omnes(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n"
        "5\r\nsal", HTTP_GET, VERUM, HTTP_LECTOR_ERROR, CC, "", FALSUM);
    /* longitudo non impleta + EOF */
    _omnes(c, "HTTP/1.1 200 OK\r\nContent-Length: 9\r\n\r\nsalve",
        HTTP_GET, VERUM, HTTP_LECTOR_ERROR, CC, "", FALSUM);
    /* linea status prava */
    l = _currere("garbage\r\n\r\n", ZEPHYRUM, HTTP_GET, FALSUM, &col,
        c->piscina);
    CREDO_AEQUALIS_I32((i32)http_lector_status(l),
        (i32)HTTP_LECTOR_ERROR);
    CREDO_VERUM(http_lector_error(l).mensura > ZEPHYRUM);
    /* capita nimia */
    ingens = (character*)piscina_allocare(c->piscina,
        (memoriae_index)(HTTP_LECTOR_CAPITA_MAXIMA + C));
    memcpy(ingens, "HTTP/1.1 200 OK\r\nX: ", XX);
    per (k = XX; k < HTTP_LECTOR_CAPITA_MAXIMA + L; k++)
    {
        ingens[k] = 'a';
    }
    ingens[k] = '\0';
    l = _currere(ingens, ZEPHYRUM, HTTP_GET, FALSUM, &col, c->piscina);
    CREDO_AEQUALIS_I32((i32)http_lector_status(l),
        (i32)HTTP_LECTOR_ERROR);
    /* EOF ante capita: vacuum */
    l = http_lector_creare(c->piscina, HTTP_GET, NIHIL, NIHIL);
    CREDO_AEQUALIS_I32((i32)http_lector_finire(l),
        (i32)HTTP_LECTOR_ERROR);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "longitudo", _sectio_longitudo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chunked", _sectio_chunked, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausura", _sectio_clausura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non 2xx", _sectio_non_2xx, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine corpore", _sectio_sine_corpore, NIHIL, NIHIL, NIHIL, NIHIL },
    { "prava", _sectio_prava, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("http_lector", SECTIONES);
}
```

- [ ] **Step 1.2: run, see it fail**
  Run: `./compile_tests.sh http_lector`
  Expected: compile error `implicit declaration of function
  'http_lector_creare'` (the header section is not there yet).

- [ ] **Step 1.3: header** - add the Interfaces block above to
  `include/http.h` right after the `HttpResultus` typedef (the request
  parser types follow it).

- [ ] **Step 1.4: implementation** - add to `lib/http.c` after
  `_decodificare_chunked` (it uses `_parse_status_line`,
  `_parse_header`, `_hex_ad_valor`, `_chorda_ad_i32`,
  `_http_corpus_prohibitum`, `_chorda_aequalis_literis_ignora_casus`
  and `http_responsum_caput`; `_http_corpus_prohibitum` is defined
  later in the file at `lib/http.c:718` - add a prototype
  `interior b32 _http_corpus_prohibitum (i32 status);` above the new
  section, matching its definition):

```c
/* ========================================================================
 * LECTOR RESPONSI INCREMENTALIS (quaestio-rivus-spec II.1)
 * ======================================================================== */

nomen enumeratio {
    LECTOR_CAPITA = 0,
    LECTOR_LONGITUDO,        /* Content-Length: reliqua */
    LECTOR_CLAUSURA,         /* usque ad EOF */
    LECTOR_FRUSTI_MENSURA,   /* chunked: linea magnitudinis */
    LECTOR_FRUSTUM,          /* chunked: data */
    LECTOR_FRUSTI_FINIS,     /* chunked: CRLF post data */
    LECTOR_POSTREMA,         /* chunked: capita postrema usque vacuam */
    LECTOR_FINITUS,
    LECTOR_ERROR
} LectorGradus;

structura HttpLector {
         Piscina* piscina;
     HttpMethodus methodus;
        HttpRivus rivus;
           vacuum* rivi_datum;
     LectorGradus gradus;
              i8* capita;
             i32  capita_n;
   HttpResponsum* responsum;
              i8* corpus;
             i32  corpus_n;
             i32  corpus_capacitas;
             i32  reliqua;
             i32  frusti_mensura;
             b32  hex_visum;
             b32  in_extensione;
             b32  cr_visum;
             i32  linea_n;
             b32  rivus_activus;
          chorda  error;
};

interior HttpLectorStatus
_lector_status_publicus (
    constans HttpLector* l)
{
    commutatio (l->gradus)
    {
        casus LECTOR_CAPITA:   redde HTTP_LECTOR_CAPITA;
        casus LECTOR_FINITUS:  redde HTTP_LECTOR_FINITUS;
        casus LECTOR_ERROR:    redde HTTP_LECTOR_ERROR;
        ordinarius:            redde HTTP_LECTOR_CORPUS;
    }
}

interior vacuum
_lector_errare (
             HttpLector* l,
    constans character* causa)
{
    l->gradus  = LECTOR_ERROR;
    l->error   = chorda_ex_literis(causa, l->piscina);
}

interior vacuum
_lector_corpus_addere (
      HttpLector* l,
    constans i8* octeti,
             i32  n)
{
    si (n == ZEPHYRUM)
    {
        redde;
    }
    si (l->corpus_n + n > HTTP_LECTOR_CORPUS_MAXIMUM)
    {
        _lector_errare(l, "corpus nimium");
        redde;
    }
    si (l->corpus_n + n > l->corpus_capacitas)
    {
        i32 capacitas = l->corpus_capacitas > ZEPHYRUM
            ? l->corpus_capacitas : M;
        i8* novum;

        dum (capacitas < l->corpus_n + n)
        {
            capacitas *= II;
        }
        novum = (i8*)piscina_allocare(l->piscina, (i64)capacitas);
        si (l->corpus_n > ZEPHYRUM)
        {
            memcpy(novum, l->corpus, (size_t)l->corpus_n);
        }
        l->corpus            = novum;
        l->corpus_capacitas  = capacitas;
    }
    memcpy(l->corpus + l->corpus_n, octeti, (size_t)n);
    l->corpus_n += n;
    si (l->rivus_activus && l->rivus)
    {
        l->rivus(l->rivi_datum, octeti, n);
    }
}

/* capita[0..capita_n) desinunt in \r\n\r\n: status, capita, modus
 * corporis */
interior vacuum
_lector_capita_parare (
    HttpLector* l)
{
    constans character* t      = (constans character*)l->capita;
                   i32  finis  = l->capita_n - II;  /* ante \r\n ultimum */
                   i32  a      = ZEPHYRUM;
                   i32  b;
                chorda  te;
                chorda  cl;

    l->responsum = (HttpResponsum*)piscina_allocare(l->piscina,
        (i64)magnitudo(HttpResponsum));
    memset(l->responsum, ZEPHYRUM, magnitudo(HttpResponsum));
    l->responsum->capita = (HttpCaput*)piscina_allocare(l->piscina,
        (i64)(HTTP_CAPITA_MAXIMA * (i32)magnitudo(HttpCaput)));
    b = a;
    dum (b + I < finis && !(t[b] == '\r' && t[b + I] == '\n'))
    {
        b++;
    }
    si (!_parse_status_line(t + a, b - a, l->responsum, l->piscina))
    {
        _lector_errare(l, "linea status prava");
        redde;
    }
    a = b + II;
    dum (a < finis)
    {
        b = a;
        dum (b + I < finis + II && !(t[b] == '\r' && t[b + I] == '\n'))
        {
            b++;
        }
        si (b > a && l->responsum->capita_numerus < HTTP_CAPITA_MAXIMA)
        {
            _parse_header(t + a, b - a,
                &l->responsum->capita[l->responsum->capita_numerus],
                l->piscina);
            l->responsum->capita_numerus++;
        }
        a = b + II;
    }
    l->rivus_activus = l->responsum->status >= CC
        && l->responsum->status < CCC;
    te = http_responsum_caput(l->responsum, "Transfer-Encoding");
    cl = http_responsum_caput(l->responsum, "Content-Length");
    si (   l->methodus == HTTP_HEAD
        || _http_corpus_prohibitum(l->responsum->status))
    {
        l->gradus = LECTOR_FINITUS;
    }
    alioquin si (   te.mensura > ZEPHYRUM
                 && _chorda_aequalis_literis_ignora_casus(te, "chunked"))
    {
        l->gradus          = LECTOR_FRUSTI_MENSURA;
        l->frusti_mensura  = ZEPHYRUM;
        l->hex_visum       = FALSUM;
        l->in_extensione   = FALSUM;
    }
    alioquin si (cl.mensura > ZEPHYRUM)
    {
        l->reliqua  = _chorda_ad_i32(cl);
        l->gradus   = l->reliqua == ZEPHYRUM ? LECTOR_FINITUS
                                             : LECTOR_LONGITUDO;
    }
    alioquin
    {
        l->gradus = LECTOR_CLAUSURA;
    }
}

HttpLector*
http_lector_creare (
        Piscina* piscina,
    HttpMethodus methodus,
       HttpRivus rivus,
         vacuum* rivi_datum)
{
    HttpLector* l;

    si (!piscina)
    {
        redde NIHIL;
    }
    l = (HttpLector*)piscina_allocare(piscina,
        (i64)magnitudo(HttpLector));
    memset(l, ZEPHYRUM, magnitudo(HttpLector));
    l->piscina     = piscina;
    l->methodus    = methodus;
    l->rivus       = rivus;
    l->rivi_datum  = rivi_datum;
    l->gradus      = LECTOR_CAPITA;
    l->capita      = (i8*)piscina_allocare(piscina,
        (i64)HTTP_LECTOR_CAPITA_MAXIMA);
    redde l;
}

HttpLectorStatus
http_lector_adicere (
      HttpLector* l,
    constans i8* octeti,
             i32  n)
{
    i32 i = ZEPHYRUM;

    si (!l)
    {
        redde HTTP_LECTOR_ERROR;
    }
    dum (   i < n && l->gradus != LECTOR_FINITUS
         && l->gradus != LECTOR_ERROR)
    {
        i8  c = octeti[i];
        i32 k;

        commutatio (l->gradus)
        {
            casus LECTOR_CAPITA:
                si (l->capita_n >= HTTP_LECTOR_CAPITA_MAXIMA)
                {
                    _lector_errare(l, "capita nimia");
                    frange;
                }
                l->capita[l->capita_n++] = c;
                i++;
                si (   l->capita_n >= IV
                    && l->capita[l->capita_n - IV]  == '\r'
                    && l->capita[l->capita_n - III] == '\n'
                    && l->capita[l->capita_n - II]  == '\r'
                    && l->capita[l->capita_n - I]   == '\n')
                {
                    _lector_capita_parare(l);
                }
                frange;
            casus LECTOR_LONGITUDO:
            casus LECTOR_FRUSTUM:
                k = n - i < l->reliqua ? n - i : l->reliqua;
                _lector_corpus_addere(l, octeti + i, k);
                i          += k;
                l->reliqua -= k;
                si (l->gradus != LECTOR_ERROR && l->reliqua == ZEPHYRUM)
                {
                    si (l->gradus == LECTOR_LONGITUDO)
                    {
                        l->gradus = LECTOR_FINITUS;
                    }
                    alioquin
                    {
                        l->gradus    = LECTOR_FRUSTI_FINIS;
                        l->cr_visum  = FALSUM;
                    }
                }
                frange;
            casus LECTOR_CLAUSURA:
                _lector_corpus_addere(l, octeti + i, n - i);
                i = n;
                frange;
            casus LECTOR_FRUSTI_MENSURA:
                i++;
                si (c == '\n')
                {
                    si (!l->hex_visum)
                    {
                        _lector_errare(l, "magnitudo frusti absens");
                    }
                    alioquin si (l->frusti_mensura == ZEPHYRUM)
                    {
                        l->gradus   = LECTOR_POSTREMA;
                        l->linea_n  = ZEPHYRUM;
                    }
                    alioquin
                    {
                        l->gradus   = LECTOR_FRUSTUM;
                        l->reliqua  = l->frusti_mensura;
                    }
                    l->frusti_mensura  = ZEPHYRUM;
                    l->hex_visum       = FALSUM;
                    l->in_extensione   = FALSUM;
                }
                alioquin si (c == '\r' || l->in_extensione)
                {
                    /* finis lineae aut extensio: neglegitur */
                }
                alioquin si (c == ';' || c == ' ' || c == '\t')
                {
                    l->in_extensione = l->hex_visum;
                    si (!l->hex_visum)
                    {
                        _lector_errare(l, "magnitudo frusti prava");
                    }
                }
                alioquin
                {
                    integer v = _hex_ad_valor(c);

                    si (v < ZEPHYRUM)
                    {
                        _lector_errare(l, "magnitudo frusti prava");
                    }
                    alioquin si (l->frusti_mensura
                        > HTTP_LECTOR_CORPUS_MAXIMUM / XVI)
                    {
                        _lector_errare(l, "frustum nimium");
                    }
                    alioquin
                    {
                        l->frusti_mensura = l->frusti_mensura * XVI
                            + (i32)v;
                        l->hex_visum = VERUM;
                    }
                }
                frange;
            casus LECTOR_FRUSTI_FINIS:
                i++;
                si (c == '\r' && !l->cr_visum)
                {
                    l->cr_visum = VERUM;
                }
                alioquin si (c == '\n')
                {
                    l->gradus = LECTOR_FRUSTI_MENSURA;
                }
                alioquin
                {
                    _lector_errare(l, "finis frusti pravus");
                }
                frange;
            casus LECTOR_POSTREMA:
                i++;
                si (c == '\n')
                {
                    si (l->linea_n == ZEPHYRUM)
                    {
                        l->gradus = LECTOR_FINITUS;
                    }
                    l->linea_n = ZEPHYRUM;
                }
                alioquin si (c != '\r')
                {
                    l->linea_n++;
                }
                frange;
            ordinarius:
                i = n;
                frange;
        }
    }
    redde _lector_status_publicus(l);
}

HttpLectorStatus
http_lector_finire (
    HttpLector* l)
{
    si (!l)
    {
        redde HTTP_LECTOR_ERROR;
    }
    si (l->gradus == LECTOR_CLAUSURA)
    {
        l->gradus = LECTOR_FINITUS;
    }
    alioquin si (l->gradus == LECTOR_CAPITA)
    {
        _lector_errare(l, l->capita_n == ZEPHYRUM ? "responsum vacuum"
                                                  : "capita truncata");
    }
    alioquin si (l->gradus != LECTOR_FINITUS && l->gradus != LECTOR_ERROR)
    {
        _lector_errare(l, "corpus truncatum");
    }
    redde _lector_status_publicus(l);
}

HttpLectorStatus
http_lector_status (
    constans HttpLector* l)
{
    redde l ? _lector_status_publicus(l) : HTTP_LECTOR_ERROR;
}

HttpResponsum*
http_lector_responsum (
    HttpLector* l)
{
    si (!l || !l->responsum)
    {
        redde NIHIL;
    }
    l->responsum->corpus.datum    = l->corpus;
    l->responsum->corpus.mensura  = l->corpus_n;
    redde l->responsum;
}

chorda
http_lector_error (
    constans HttpLector* l)
{
    chorda v;

    si (l && l->gradus == LECTOR_ERROR)
    {
        redde l->error;
    }
    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}
```

  Signatures verified while planning: `_hex_ad_valor (i8 c)`,
  `_parse_status_line (constans character* data, i32 len,
  HttpResponsum*, Piscina*)`, `_parse_header (constans character*
  line, i32 len, HttpCaput*, Piscina*)` (`lib/http.c:166,497,555`).

- [ ] **Step 1.5: run, see it pass**
  Run: `./compile_tests.sh http_lector`
  Expected: `=== SECTIONES: 6/6 transierunt ===` in
  `build/test_logs/singulae/probatio_http_lector.log`.
  Also `./compile_tests.sh http` (probatio_http, probatio_http_parser
  still green: the old paths are untouched).

- [ ] **Step 1.6: plants** (each red, then green):
  `l->gradus = LECTOR_FRUSTI_MENSURA;` in `LECTOR_FRUSTI_FINIS` ->
  `l->gradus = LECTOR_FRUSTUM;` (chunk boundary) ; in
  `_lector_capita_parare` `l->rivus_activus = l->responsum->status >=
  CC` -> `l->rivus_activus = VERUM || l->responsum->status >= CC`
  (non-2xx must stay silent).

- [ ] **Step 1.7: worklog + commit** - `lib/http.worklog.md`: dated
  entry (lector design, R1/R2, the 2-split oracle). Lint, then
  `silva.commissio('http_lector: lector responsi incrementalis ...',
  ['include/http.h','lib/http.c','probationes/probatio_http_lector.c',
  'lib/http.worklog.md'])`.

---

### Task 2: streaming transport - `http_exsequi_rivus`, `HttpVecturaRivi`, herbarium

**Files:** Modify `include/http.h`, `lib/http.c` (connect+send helper
shared by `http_exsequi`), `include/herbarium.h`, `lib/herbarium.c`,
`probationes/probatio_http.c` (new fixture mode + section),
`probationes/probatio_herbarium.c`; worklogs.

**Interfaces:** Consumes T1 (`HttpLector`, `HttpRivus`). Produces:

```c
/* http.h - after HttpVectura */

/* Exsequi cum rivo: ut http_exsequi, sed corpus 2xx per rivum exit dum
 * advenit (quaestio-rivus-spec II.2). responsum->corpus TOTUM quoque
 * redditur (R6). Lectio finitur cum responsum finitum est (connexio
 * non exspectatur claudi). */
HttpResultus
http_exsequi_rivus (
    HttpPetitio* petitio,
      HttpRivus  rivus,
         vacuum* rivi_datum,
        Piscina* piscina);

nomen structura {
    HttpResultus (*exsequi)(HttpPetitio* petitio, HttpRivus rivus,
                            vacuum* rivi_datum, Piscina* piscina,
                            vacuum* datum);
    vacuum* datum;
} HttpVecturaRivi;

/* Vectura rivi ordinaria: http_exsequi_rivus, datum NIHIL. */
HttpVecturaRivi
http_vectura_rivi_ordinaria (vacuum);

/* exsequi NIHIL -> HTTP_ERROR_CONNEXIO nominatus, numquam ruina */
HttpResultus
http_vectura_rivi_exsequi (
    HttpVecturaRivi  vectura,
       HttpPetitio*  petitio,
         HttpRivus   rivus,
            vacuum*  rivi_datum,
           Piscina*  piscina);

/* herbarium.h */

/* Vectura capiens rivi: ut herbarium_vectura - interiorem exsequitur,
 * rivum TRANSMITTIT, deinde responsum totum iudicat et premit (post
 * finem rivi: R5). */
HttpVecturaRivi
herbarium_vectura_rivi (
          Herbarium* herbarium,
    HttpVecturaRivi  involuta);

/* Vectura reddens rivi: corpus speciminis 2xx per rivum in frustis
 * VII octetorum (fines ubicumque), deinde responsum. */
HttpVecturaRivi
herbarium_reddens_rivi (
    Piscina* piscina,
        Xar* specimina);
```

- [ ] **Step 2.1: failing tests.**
  `probationes/probatio_http.c`: add `FIXTURA_RIVUS` to the fixture
  mode enum and a branch in the raw-server child (after the existing
  chunked branches, same file `probationes/probatio_http.c:288-320`)
  that sends the response in THREE writes with a pause, then keeps the
  connection OPEN for 2 s (the client must stop on `FINITUS`, not
  wait for close):

```c
        alioquin si (modus == FIXTURA_RIVUS)
        {
            constans character* a = "HTTP/1.1 200 OK\r\n"
                "Transfer-Encoding: chunked\r\n\r\n5\r\nsalve\r\n";
            constans character* b = "6\r\n munde\r\n";
            constans character* fin = "0\r\n\r\n";

            (vacuum)tcp_mittere_omnia(conn.connexio, (constans i8*)a,
                (i32)strlen(a));
            usleep(CCC * M);   /* 300 ms: rivus primum ante finem */
            (vacuum)tcp_mittere_omnia(conn.connexio, (constans i8*)b,
                (i32)strlen(b));
            (vacuum)tcp_mittere_omnia(conn.connexio,
                (constans i8*)fin, (i32)strlen(fin));
            sleep(II);         /* connexio aperta manet */
        }
```

  and a test function (old-style suite: follow the file's existing
  `probatio_chunked_*` pattern and call it from `principale`):

```c
/* rivus: frustum primum ANTE finem advenit; corpus totum redditur;
 * lectio finitur sine clausura (servus II s apertus manet) */
nomen structura {
    ChordaAedificator* aed;
                  s64  primum_ms;
} RivusProbationis;

interior s64
_ms_nunc_probationis (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_rivus_probationis (
          vacuum* datum,
    constans i8* octeti,
             i32  n)
{
    RivusProbationis* r = (RivusProbationis*)datum;
    i32 k;

    si (r->primum_ms == ZEPHYRUM)
    {
        r->primum_ms = _ms_nunc_probationis();
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        chorda_aedificator_appendere_character(r->aed,
            (character)octeti[k]);
    }
}

interior vacuum
probatio_rivus (
    Piscina* piscina)
{
         pid_t  pid;
           i32  portus;
     character  url[CXXVIII];
   HttpPetitio* p;
  HttpResultus  res;
RivusProbationis r;
           s64  initium;
           s64  finis;

    imprimere("\n--- Probans http_exsequi_rivus ---\n");
    portus = _fixtura_prava_incipere(&pid, FIXTURA_RIVUS);
    CREDO_VERUM(portus > ZEPHYRUM);
    sprintf(url, "http://127.0.0.1:%u/rivus", portus);
    p = http_petitio_creare(piscina, HTTP_GET, url);
    r.aed        = chorda_aedificator_creare(piscina, LXIV);
    r.primum_ms  = ZEPHYRUM;
    initium      = _ms_nunc_probationis();
    res          = http_exsequi_rivus(p, _rivus_probationis, &r, piscina);
    finis        = _ms_nunc_probationis();
    CREDO_VERUM(res.successus);
    CREDO_CHORDA_AEQUALIS_LITERIS(chorda_aedificator_finire(r.aed),
        "salve munde");
    CREDO_CHORDA_AEQUALIS_LITERIS(res.responsum->corpus, "salve munde");
    /* frustum primum ante pausam; finis ante clausuram servi (II s) */
    CREDO_VERUM(r.primum_ms - initium < CC);
    CREDO_VERUM(finis - initium < M + D);
    _fixtura_prava_finire(pid);
}
```

  (Use the file's actual fixture start/stop helper names - read
  `probationes/probatio_http.c:190-200` and the end of the fixture;
  rename the calls above to match. `sys/time.h` include needed.)

  `probationes/probatio_herbarium.c`: a section that wraps a scripted
  inner `HttpVecturaRivi` (returns 529 JSON once, then 200 SSE with
  `Content-Type: text/event-stream`) in `herbarium_vectura_rivi`:
  the 529 is pressed (one specimen file), the stream is passed
  through the sink unchanged and NOT pressed when the iudex returns
  "" ; then `herbarium_reddens_rivi` over the pressed specimen feeds
  its body to a collecting sink (equal to the stored body, sink called
  more than once for a body longer than VII bytes). Follow the file's
  existing capture test for directory setup and cleanup.

- [ ] **Step 2.2: run, see it fail**
  `./compile_tests.sh probatio_http\b` and `./compile_tests.sh
  herbarium` - expected: implicit declaration of
  `http_exsequi_rivus` / `herbarium_vectura_rivi`.

- [ ] **Step 2.3: implementation, http.**
  1. Extract from `http_exsequi` (`lib/http.c:753-870`) the
     connect + send part into
     `interior b32 _connexionem_aperire (HttpPetitio* petitio,
     Piscina* piscina, TlsConnexio** tls, TcpConnexio** tcp,
     HttpResultus* error)`: builds `_construere_petitio`, connects
     (https or tcp, with `petitio->tempus_ms`), sends; on failure
     fills `*error` exactly as today and returns `FALSUM`.
     `http_exsequi` calls it; its read loop and parse stay as they are
     (byte-for-byte behaviour unchanged; probatio_http stays green).
  2. `http_exsequi_rivus`:

```c
HttpResultus
http_exsequi_rivus (
    HttpPetitio* petitio,
      HttpRivus  rivus,
         vacuum* rivi_datum,
        Piscina* piscina)
{
    HttpResultus      res;
     TlsConnexio*     tls_conn = NIHIL;
     TcpConnexio*     tcp_conn = NIHIL;
      HttpLector*     lector;
              i8*     buffer;
             s32      n;
    HttpLectorStatus  status;

    si (!petitio || !piscina)
    {
        redde _creare_error(HTTP_ERROR_URL, "Argumenta invalida",
            piscina);
    }
    si (!_connexionem_aperire(petitio, piscina, &tls_conn, &tcp_conn,
            &res))
    {
        redde res;
    }
    lector  = http_lector_creare(piscina, petitio->methodus, rivus,
        rivi_datum);
    buffer  = (i8*)piscina_allocare(piscina, (i64)HTTP_BUFFER_MAGNITUDO);
    status  = HTTP_LECTOR_CAPITA;
    fac
    {
        n = tls_conn ? tls_recipere(tls_conn, buffer,
                           HTTP_BUFFER_MAGNITUDO)
                     : tcp_recipere(tcp_conn, buffer,
                           HTTP_BUFFER_MAGNITUDO);
        si (n > ZEPHYRUM)
        {
            status = http_lector_adicere(lector, buffer, (i32)n);
        }
    } dum (   n > ZEPHYRUM && status != HTTP_LECTOR_FINITUS
           && status != HTTP_LECTOR_ERROR);
    si (tls_conn)
    {
        tls_claudere(tls_conn);
    }
    alioquin
    {
        tcp_claudere(tcp_conn);
    }
    si (status == HTTP_LECTOR_ERROR)
    {
        redde _creare_error(HTTP_ERROR_PARSE,
            chorda_ut_cstr(http_lector_error(lector), piscina), piscina);
    }
    si (status != HTTP_LECTOR_FINITUS)
    {
        si (n == TCP_ITERUM)
        {
            redde _creare_error(HTTP_ERROR_TIMEOUT,
                "Tempus receptionis excessum (rivus)", piscina);
        }
        si (n < ZEPHYRUM)
        {
            redde _creare_error(HTTP_ERROR_IO,
                "Error retis inter receptionem (rivus)", piscina);
        }
        status = http_lector_finire(lector);
        si (status != HTTP_LECTOR_FINITUS)
        {
            redde _creare_error(HTTP_ERROR_IO,
                chorda_ut_cstr(http_lector_error(lector), piscina),
                piscina);
        }
    }
    res.successus                 = VERUM;
    res.responsum                 = http_lector_responsum(lector);
    res.error                     = HTTP_OK;
    res.error_descriptio.datum    = NIHIL;
    res.error_descriptio.mensura  = ZEPHYRUM;
    redde res;
}
```

     (check `_creare_error`'s second parameter type - it takes
     `constans character*`; `chorda_ut_cstr` gives that.)
  3. `http_vectura_rivi_ordinaria` / `http_vectura_rivi_exsequi`
     next to `_vectura_ordinaria_exsequi` (`lib/http.c:2365`), same
     shape: an interior adapter calling `http_exsequi_rivus`, and the
     exsequi-NIHIL guard returning `HTTP_ERROR_CONNEXIO` "vectura
     rivi nulla".

- [ ] **Step 2.4: implementation, herbarium.**
  1. Split `_exsequi_capiens` (`lib/herbarium.c:515-547`): its body
     after the inner call becomes `interior vacuum _iudicare_et_premere
     (Herbarium* h, HttpPetitio* petitio, HttpResultus res, Piscina*
     piscina)`; `_exsequi_capiens` = inner call + that.
  2. `HerbariumInvolucrum` gains `HttpVecturaRivi involuta_rivi;`.
     `_exsequi_capiens_rivi(petitio, rivus, rivi_datum, piscina,
     datum)` = `http_vectura_rivi_exsequi(inv->involuta_rivi, ...)` +
     `_iudicare_et_premere`. `herbarium_vectura_rivi` mirrors
     `herbarium_vectura` (`!herbarium` -> return `involuta`).
  3. `_exsequi_reddens_rivi`: same as `_exsequi_reddens`
     (`lib/herbarium.c:873-907`), then, if `200 <= status < 300` and
     `rivus`, feed `s->corpus` in pieces of `VII` bytes. Shares the
     `HerbariumRedditor` struct; `herbarium_reddens_rivi` mirrors
     `herbarium_reddens`.

- [ ] **Step 2.5: run, see it pass** - `./compile_tests.sh http`,
  `./compile_tests.sh herbarium`, `./compile_tests.sh vates` (vates
  sits on the refactored `http_exsequi`; must stay green).

- [ ] **Step 2.6: plants** - in `http_exsequi_rivus` drop the
  `status != HTTP_LECTOR_FINITUS` stop condition from the `dum`
  (client waits for close -> the `< M + D` assert goes red); in
  `_exsequi_reddens_rivi` feed only the first piece (sink total
  shorter -> red).

- [ ] **Step 2.7: worklogs + commit** - `lib/http.worklog.md`,
  `lib/herbarium.worklog.md` (R5, R6). Files: `include/http.h`,
  `lib/http.c`, `include/herbarium.h`, `lib/herbarium.c`,
  `probationes/probatio_http.c`, `probationes/probatio_herbarium.c`,
  worklogs.

---

### Task 3: `rivus_eventuum` - SSE parser

**Files:** Create `include/rivus_eventuum.h`, `lib/rivus_eventuum.c`,
`probationes/probatio_rivus_eventuum.c`, `lib/rivus_eventuum.worklog.md`;
regenerate `compile_tests_fontes_generata.sh`.

**Interfaces:** Produces:

```c
/* rivus_eventuum.h - Server-Sent Events, lector incrementalis
 * (quaestio-rivus-spec II.3; WHATWG "event stream interpretation")
 *
 * Octeti intrant; eventa (nomen, data) per functionem exeunt. Lineae
 * CRLF, LF aut CR (CR ad finem adiectionis + LF in proxima = unus
 * finis); 'event:' nomen, 'data:' (plures lineae '\n' iunctae),
 * 'id:' et 'retry:' neglecta, ':' commentarium, linea vacua dispatchat
 * (data vacua: nihil). Spatium unum post ':' tollitur. BOM initialis
 * neglegitur. In fine (finire) eventus imperfectus abicitur. chordae
 * functioni datae post reditum invalidae sunt: copia si servanda. */

#ifndef RIVUS_EVENTUUM_H
#define RIVUS_EVENTUUM_H

/* <aedilis corpus="lib/rivus_eventuum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen vacuum (*RivusEventuumFunctio)(vacuum* datum, chorda eventus,
                                     chorda data);

nomen structura RivusEventuum RivusEventuum;   /* opacus */

RivusEventuum*
rivus_eventuum_creare (
                Piscina* piscina,
    RivusEventuumFunctio functio,
                  vacuum* datum);

vacuum
rivus_eventuum_adicere (
     RivusEventuum* rivus,
     constans i8* octeti,
              i32  n);

vacuum
rivus_eventuum_finire (
    RivusEventuum* rivus);

/* eventa dispatchata hactenus */
i32
rivus_eventuum_numerus (
    constans RivusEventuum* rivus);

#endif /* RIVUS_EVENTUUM_H */
```

- [ ] **Step 3.1: failing test** -
  `probationes/probatio_rivus_eventuum.c`, credo v2, with a collector
  that records `"<event>=<data>|"` per dispatch, and an `_omnes`
  helper feeding whole / byte by byte / every 2-split (same shape as
  T1's) and asserting the record string. Sections:

```c
/* nomen ordinarium "message"; data multilinea; commentarium */
_omnes(c, ": commentarium\ndata: a\ndata: b\n\n", "message=a\nb|");
/* event nominatum, CRLF et CR */
_omnes(c, "event: x\r\ndata: 1\r\n\r\nevent: y\rdata: 2\r\r",
    "x=1|y=2|");
/* spatium unum solum tollitur; sine ':' campus totus */
_omnes(c, "data:  duo\ndata\n\n", "message= duo\n|");
/* id, retry, campi ignoti neglecti; data vacua nihil dispatchat */
_omnes(c, "id: 7\nretry: 10\nignotum: z\n\nevent: q\n\ndata: v\n\n",
    "message=v|");
/* BOM initialis */
_omnes(c, "\xEF\xBB\xBF" "data: b\n\n", "message=b|");
/* finire: eventus imperfectus abicitur */
_omnes_finire(c, "data: a\n\ndata: b\n", "message=a|");
/* Anthropic forma vera (ex documentis API) */
_omnes(c, "event: content_block_delta\n"
    "data: {\"type\":\"content_block_delta\",\"index\":0,"
    "\"delta\":{\"type\":\"text_delta\",\"text\":\"Hi\"}}\n\n",
    "content_block_delta={\"type\":\"content_block_delta\",\"index\":0,"
    "\"delta\":{\"type\":\"text_delta\",\"text\":\"Hi\"}}|");
```

  (each line above is one CREDO assertion inside a named section:
  `commune`, `fines linearum`, `spatia`, `neglecta`, `bom`, `finis`,
  `anthropic`; `_omnes_finire` = `_omnes` + `rivus_eventuum_finire`
  before reading the record.)

- [ ] **Step 3.2: run, see it fail** - `./compile_tests.sh
  rivus_eventuum` -> missing header.

- [ ] **Step 3.3: implementation** - `lib/rivus_eventuum.c`:

```c
/* rivus_eventuum.c - Server-Sent Events, lector incrementalis
 * (vide caput) */
#include "rivus_eventuum.h"
#include "chorda_aedificator.h"
#include <string.h>

structura RivusEventuum {
                 Piscina* piscina;
    RivusEventuumFunctio  functio;
                  vacuum* datum;
       ChordaAedificator* linea;
       ChordaAedificator* data;
       ChordaAedificator* eventus;
                     b32  data_visa;
                     b32  cr_pendens;   /* CR ultimus: LF proximus neglegitur */
                     i32  bom;          /* octeti BOM initiales visi */
                     b32  initium;      /* nondum octetus ullus */
                     i32  numerus;
};

RivusEventuum*
rivus_eventuum_creare (
                Piscina* piscina,
    RivusEventuumFunctio functio,
                  vacuum* datum)
{
    RivusEventuum* r;

    si (!piscina)
    {
        redde NIHIL;
    }
    r = (RivusEventuum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(RivusEventuum));
    memset(r, ZEPHYRUM, magnitudo(RivusEventuum));
    r->piscina  = piscina;
    r->functio  = functio;
    r->datum    = datum;
    r->linea    = chorda_aedificator_creare(piscina, CXXVIII);
    r->data     = chorda_aedificator_creare(piscina, CXXVIII);
    r->eventus  = chorda_aedificator_creare(piscina, XXXII);
    r->initium  = VERUM;
    redde r;
}

interior vacuum
_dispatchare (
    RivusEventuum* r)
{
    chorda data;
    chorda eventus;

    si (r->data_visa)
    {
        data = chorda_aedificator_spectare(r->data);
        si (data.mensura > ZEPHYRUM && data.datum[data.mensura - I] == '\n')
        {
            data.mensura--;
        }
        eventus = chorda_aedificator_spectare(r->eventus);
        si (eventus.mensura == ZEPHYRUM)
        {
            eventus = chorda_ex_literis("message", r->piscina);
        }
        r->numerus++;
        si (r->functio)
        {
            r->functio(r->datum, eventus, data);
        }
    }
    chorda_aedificator_reset(r->data);
    chorda_aedificator_reset(r->eventus);
    r->data_visa = FALSUM;
}

interior vacuum
_lineam_tractare (
    RivusEventuum* r)
{
    chorda linea = chorda_aedificator_spectare(r->linea);
    i32    k;
    chorda campus;
    chorda valor;

    si (linea.mensura == ZEPHYRUM)
    {
        _dispatchare(r);
        redde;
    }
    si (linea.datum[ZEPHYRUM] == ':')
    {
        chorda_aedificator_reset(r->linea);
        redde;
    }
    k = ZEPHYRUM;
    dum (k < linea.mensura && linea.datum[k] != ':')
    {
        k++;
    }
    campus.datum    = linea.datum;
    campus.mensura  = k;
    valor.datum     = linea.datum + (k < linea.mensura ? k + I : k);
    valor.mensura   = k < linea.mensura ? linea.mensura - k - I : ZEPHYRUM;
    si (valor.mensura > ZEPHYRUM && valor.datum[ZEPHYRUM] == ' ')
    {
        valor.datum++;
        valor.mensura--;
    }
    si (chorda_aequalis_literis(campus, "data"))
    {
        chorda_aedificator_appendere_chorda(r->data, valor);
        chorda_aedificator_appendere_character(r->data, '\n');
        r->data_visa = VERUM;
    }
    alioquin si (chorda_aequalis_literis(campus, "event"))
    {
        chorda_aedificator_reset(r->eventus);
        chorda_aedificator_appendere_chorda(r->eventus, valor);
    }
    /* id, retry, ignota: neglecta */
    chorda_aedificator_reset(r->linea);
}

vacuum
rivus_eventuum_adicere (
     RivusEventuum* r,
     constans i8* octeti,
              i32  n)
{
    hic_manens constans i8 BOM[III] = { (i8)0xEF, (i8)0xBB, (i8)0xBF };
    i32 i;

    si (!r)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        i8 c = octeti[i];

        si (r->initium || r->bom > ZEPHYRUM)
        {
            si (r->bom < III && c == BOM[r->bom])
            {
                r->bom++;
                r->initium = FALSUM;
                si (r->bom == III)
                {
                    r->bom = -I;   /* BOM totus: finitum */
                }
                perge;
            }
            /* BOM partialis non est BOM: octeti eius ut textus */
            si (r->bom > ZEPHYRUM)
            {
                i32 k;
                per (k = ZEPHYRUM; k < r->bom; k++)
                {
                    chorda_aedificator_appendere_character(r->linea,
                        (character)BOM[k]);
                }
            }
            r->bom      = -I;
            r->initium  = FALSUM;
        }
        si (r->cr_pendens)
        {
            r->cr_pendens = FALSUM;
            si (c == '\n')
            {
                perge;
            }
        }
        si (c == '\r')
        {
            _lineam_tractare(r);
            r->cr_pendens = VERUM;
        }
        alioquin si (c == '\n')
        {
            _lineam_tractare(r);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(r->linea,
                (character)c);
        }
    }
}

vacuum
rivus_eventuum_finire (
    RivusEventuum* r)
{
    si (!r)
    {
        redde;
    }
    /* linea imperfecta et eventus sine linea vacua: abiciuntur */
    chorda_aedificator_reset(r->linea);
    chorda_aedificator_reset(r->data);
    chorda_aedificator_reset(r->eventus);
    r->data_visa = FALSUM;
}

i32
rivus_eventuum_numerus (
    constans RivusEventuum* r)
{
    redde r ? r->numerus : ZEPHYRUM;
}
```

  `r->bom` is `s32` (it takes -1): declare it `s32 bom;` in the
  struct. (`chorda_aedificator_spectare` / `chorda_aedificator_reset`:
  `include/chorda_aedificator.h:206,226` - verified while planning.)
  Note the BOM state machine above must also handle `initium` being
  cleared on the first non-BOM byte - review it against the `bom`
  section's split cases before moving on.

- [ ] **Step 3.4: run, see it pass** - `./compile_tests.sh
  rivus_eventuum` 7/7; regenerate the source list
  (`./tools/compile_tests_fontes_generare.sh`).

- [ ] **Step 3.5: plants** - `_dispatchare(r)` on empty line ->
  removed (no dispatch -> red); `r->cr_pendens = VERUM;` -> `FALSUM`
  (CRLF counted as two line ends -> extra dispatch -> red at the CRLF
  split cases).

- [ ] **Step 3.6: worklog + commit** (with the regenerated
  `compile_tests_fontes_generata.sh`).

---

### Task 4: vates streaming - `vates_mittere_rivum`

**Files:** Modify `include/vates.h`, `lib/vates.c`; Create
`probationes/probatio_vates_rivus.c`; `lib/vates.worklog.md`.

**Interfaces:** Consumes T2 (`HttpVecturaRivi`,
`herbarium_vectura_rivi`), T3 (`RivusEventuum`). Produces:

```c
/* vates.h - VatesOptiones gains (after 'vectura'): */
        HttpVecturaRivi vectura_rivi;           /* exsequi NIHIL = ordinaria */

/* vates.h - after vates_mittere */

/* delta textus (cogitatio FALSUM) aut cogitationis (VERUM) dum advenit;
 * chorda post reditum invalida */
nomen vacuum (*VatesRivus)(vacuum* datum, chorda delta, b32 cogitatio);

/* Ut vates_mittere, sed "stream": true; deltae per rivum dum adveniunt.
 * Redditur responsum IDEM ac vates_mittere pro eodem nuntio (bloci,
 * causa finis, usus) - congregatum ex eventis. Iteratio SOLUM ante
 * deltam primam (post eam defectus nuntiatur, non iteratur). Rivus
 * truncatus (message_stop deest) = VATES_ERROR_RETE; eventus 'error'
 * = VATES_ERROR_STATUS cum genere et nuntio provisoris. */
VatesResponsum*
vates_mittere_rivum (
            Vates* vates,
     VatesPetitio* petitio,
           chorda  propositum,
       VatesRivus  rivus,
           vacuum* datum,
          Piscina* piscina);

/* PURA: corpus SSE Anthropic totum -> responsum neutrum; deltae per
 * rivum (NIHIL licet). */
VatesResponsum*
vates_anthropic_rivum_legere (
         chorda  corpus,
     VatesRivus  rivus,
         vacuum* datum,
        Piscina* piscina);

/* FICTUS: responsum textus per rivum (SSE verum: message_start, ping,
 * deltae partium, message_delta, message_stop) */
vacuum
vates_fictus_rivum (
             Vates* vates,
    constans chorda* partes,
                i32  numerus,
          VatesUsus  usus);

/* FICTUS: rivus qui post partes eventu 'error' finitur (numerus 0 =
 * error ante deltam) */
vacuum
vates_fictus_rivum_fractum (
                  Vates* vates,
         constans chorda* partes,
                     i32  numerus,
     constans character* error_genus,
     constans character* error_nuntius);
```

- [ ] **Step 4.1: failing test** - `probationes/probatio_vates_rivus.c`
  (credo v2; `_fictus` helper as in `probationes/probatio_quaestio.c:26-37`,
  `sine_herbario`, `conatus_maximi = III`, `mora_iterandi_ms = I`).
  A collector records deltas joined with `|`. Sections:
  - `deltae`: `vates_fictus_rivum(v, {"Sal","ve mu","nde."}, III,
    usus{12,34})`; `vates_mittere_rivum` -> deltas `"Sal|ve mu|nde.|"`,
    `r->successus`, one TEXTUS block `"Salve munde."`,
    `causa_finis == VATES_FINIS`, `usus.input == 12`,
    `usus.output == 34`; the request body
    (`vates_fictus_petitio(v, 0)`) contains `"stream":true`.
  - `aequale`: the same text through `vates_fictus_textum` +
    `vates_mittere` gives the same block text, causa finis and usage
    (field by field), and its request body does NOT contain
    `"stream"`.
  - `error eventus`: `vates_fictus_rivum_fractum(v, {"a"}, I,
    "overloaded_error", "Overloaded")` -> delta `"a|"`,
    `!successus`, `error == VATES_ERROR_STATUS`, genus
    `"overloaded_error"`, and `vates_fictus_petitiones_numerus == 1`
    (no retry after a delta).
  - `iteratio ante deltam`: `vates_fictus_crudum(v, 529, "{...overloaded...}")`
    then `vates_fictus_rivum(v, {"ok"}, I, usus)` ->
    `successus`, two requests sent, delta `"ok|"` exactly once.
  - `fractum ante deltam iteratur`: `vates_fictus_rivum_fractum(v,
    NIHIL, 0, "overloaded_error", "o")` then a good stream -> success,
    two requests.
  - `truncatus`: `vates_anthropic_rivum_legere` over an SSE body
    without `message_stop` -> `!successus`, `VATES_ERROR_RETE`,
    nuntius contains `"truncatus"`.
  - `instrumentum`: an SSE body with a `tool_use` block streamed as
    two `input_json_delta` pieces (`{"q":` + `"x"}`) ->
    `VATES_INSTRUMENTUM_PETITUM` block, `input` object with `q == "x"`,
    causa `VATES_FINIS_INSTRUMENTUM`.
  - `cogitatio`: a `thinking` block with `thinking_delta` +
    `signature_delta`, then text -> collector sees the thinking delta
    with flag VERUM, the response keeps the thinking block as OPACUM
    whose crudum has `"thinking"` = joined text and `"signature"` set.

- [ ] **Step 4.2: run, see it fail** - `./compile_tests.sh vates_rivus`
  -> implicit declarations.

- [ ] **Step 4.3: implementation** (in `lib/vates.c`):
  1. `VatesPetitio` gains `b32 rivus;`; `_corpus_anthropic`
     (`lib/vates.c:398`) after `max_tokens`:
     `si (pe->rivus) { json_objectum_ponere(radix, "stream",
     json_boolean_creare(piscina, VERUM)); }`.
  2. `Vates` gains `HttpVecturaRivi vectura_rivi;`;
     `_anthropic_struere` sets it from options or
     `http_vectura_rivi_ordinaria()`; `_herbarium_adiungere` also does
     `v->vectura_rivi = herbarium_vectura_rivi(v->herbarium,
     v->vectura_rivi);`. `_iudex_anthropic` (`lib/vates.c:1010`)
     returns "" when `http_responsum_caput(responsum, "Content-Type")`
     starts with `text/event-stream` (R5).
  3. The stream collector:

```c
/* rivus Anthropic: eventa -> nuntius congregatus (forma responsi non
 * rivi) -> _legere; quaestio-rivus-spec II.4 */
nomen structura {
            JsonValor* blocus;      /* content_block ex start */
    ChordaAedificator* textus;      /* text / thinking */
    ChordaAedificator* json;        /* input_json_delta */
               chorda  signatura;
                  b32  signata;
} VatesBlocusRivi;

nomen structura {
       Piscina* piscina;
    VatesRivus  rivus;
        vacuum* datum;
    JsonValor* nuntius;            /* message ex message_start */
           Xar* bloci;              /* VatesBlocusRivi */
        chorda  causa_finis;
           b32  causa_visa;
           s64  output;
           b32  output_visum;
           b32  finitus;            /* message_stop */
           b32  erratum;
        chorda  error_genus;
        chorda  error_nuntius;
           i32  deltae;
} VatesCollector;

interior VatesBlocusRivi*
_blocus_rivi (
    VatesCollector* c,
               s64  index)
{
    si (index < ZEPHYRUM || index >= (s64)xar_numerus(c->bloci))
    {
        redde NIHIL;
    }
    redde (VatesBlocusRivi*)xar_obtinere(c->bloci, (i32)index);
}

interior vacuum
_eventum_anthropic (
    vacuum* datum,
    chorda  eventus,
    chorda  data)
{
    VatesCollector* c = (VatesCollector*)datum;
      JsonResultus  j = json_legere(data, c->piscina);
            chorda  vacua = chorda_ex_literis("", c->piscina);
            chorda  typus;

    (vacuum)eventus;   /* typus in data ipsa (Anthropic) */
    si (!j.successus || !json_est_objectum(j.radix))
    {
        redde;
    }
    typus = json_capere_chorda(j.radix, "type", vacua);
    si (chorda_aequalis_literis(typus, "message_start"))
    {
        c->nuntius = json_objectum_capere(j.radix, "message");
    }
    alioquin si (chorda_aequalis_literis(typus, "content_block_start"))
    {
        VatesBlocusRivi* b = (VatesBlocusRivi*)xar_addere(c->bloci);

        memset(b, ZEPHYRUM, magnitudo(*b));
        b->blocus  = json_objectum_capere(j.radix, "content_block");
        b->textus  = chorda_aedificator_creare(c->piscina, CXXVIII);
        b->json    = chorda_aedificator_creare(c->piscina, XXXII);
    }
    alioquin si (chorda_aequalis_literis(typus, "content_block_delta"))
    {
        VatesBlocusRivi* b = _blocus_rivi(c,
            json_capere_integer(j.radix, "index", -I));
        JsonValor* d = json_objectum_capere(j.radix, "delta");
        chorda dt = json_capere_chorda(d, "type", vacua);

        si (!b)
        {
            redde;
        }
        si (chorda_aequalis_literis(dt, "text_delta"))
        {
            chorda t = json_capere_chorda(d, "text", vacua);

            chorda_aedificator_appendere_chorda(b->textus, t);
            c->deltae++;
            si (c->rivus)
            {
                c->rivus(c->datum, t, FALSUM);
            }
        }
        alioquin si (chorda_aequalis_literis(dt, "thinking_delta"))
        {
            chorda t = json_capere_chorda(d, "thinking", vacua);

            chorda_aedificator_appendere_chorda(b->textus, t);
            c->deltae++;
            si (c->rivus)
            {
                c->rivus(c->datum, t, VERUM);
            }
        }
        alioquin si (chorda_aequalis_literis(dt, "signature_delta"))
        {
            b->signatura  = json_capere_chorda(d, "signature", vacua);
            b->signata    = VERUM;
        }
        alioquin si (chorda_aequalis_literis(dt, "input_json_delta"))
        {
            chorda_aedificator_appendere_chorda(b->json,
                json_capere_chorda(d, "partial_json", vacua));
        }
    }
    alioquin si (chorda_aequalis_literis(typus, "message_delta"))
    {
        JsonValor* d = json_objectum_capere(j.radix, "delta");
        JsonValor* u = json_objectum_capere(j.radix, "usage");

        si (d && json_objectum_capere(d, "stop_reason")
            && json_est_chorda(json_objectum_capere(d, "stop_reason")))
        {
            c->causa_finis  = json_capere_chorda(d, "stop_reason", vacua);
            c->causa_visa   = VERUM;
        }
        si (u && json_objectum_capere(u, "output_tokens"))
        {
            c->output        = json_capere_integer(u, "output_tokens", 0);
            c->output_visum  = VERUM;
        }
    }
    alioquin si (chorda_aequalis_literis(typus, "message_stop"))
    {
        c->finitus = VERUM;
    }
    alioquin si (chorda_aequalis_literis(typus, "error"))
    {
        JsonValor* e = json_objectum_capere(j.radix, "error");

        c->erratum        = VERUM;
        c->error_genus    = json_capere_chorda(e, "type", vacua);
        c->error_nuntius  = json_capere_chorda(e, "message", vacua);
    }
    /* ping, content_block_stop, ignota: nihil */
}

/* nuntius congregatus (forma non-rivi) -> _legere */
interior VatesResponsum*
_collectorem_finire (
    VatesCollector* c)
{
    VatesResponsum* r;
         JsonValor* content;
              chorda novitas;
                 i32 k;

    si (c->erratum)
    {
        r = _responsum_vacuum(c->piscina);
        r->status_http    = CC;
        r->conatus        = I;
        r->error          = VATES_ERROR_STATUS;
        r->error_genus    = c->error_genus;
        r->error_nuntius  = c->error_nuntius;
        redde r;
    }
    si (!c->finitus || !c->nuntius)
    {
        r = _responsum_vacuum(c->piscina);
        r->status_http  = CC;
        r->conatus      = I;
        r->error        = VATES_ERROR_RETE;
        r->error_nuntius = chorda_ex_literis(
            "rivus truncatus (message_stop deest)", c->piscina);
        redde r;
    }
    content = json_tabulatum_creare(c->piscina);
    per (k = ZEPHYRUM; k < xar_numerus(c->bloci); k++)
    {
        VatesBlocusRivi* b = (VatesBlocusRivi*)xar_obtinere(c->bloci, k);
        chorda typus;

        si (!b->blocus)
        {
            perge;
        }
        typus = json_capere_chorda(b->blocus, "type",
            chorda_ex_literis("", c->piscina));
        si (chorda_aequalis_literis(typus, "text"))
        {
            json_objectum_ponere(b->blocus, "text", json_chorda_creare(
                c->piscina, chorda_aedificator_finire(b->textus)));
        }
        alioquin si (chorda_aequalis_literis(typus, "thinking"))
        {
            json_objectum_ponere(b->blocus, "thinking",
                json_chorda_creare(c->piscina,
                chorda_aedificator_finire(b->textus)));
            si (b->signata)
            {
                json_objectum_ponere(b->blocus, "signature",
                    json_chorda_creare(c->piscina, b->signatura));
            }
        }
        alioquin si (chorda_aequalis_literis(typus, "tool_use"))
        {
            chorda js = chorda_aedificator_finire(b->json);
            JsonResultus ji = json_legere(js.mensura > ZEPHYRUM ? js
                : chorda_ex_literis("{}", c->piscina), c->piscina);

            json_objectum_ponere(b->blocus, "input", ji.successus
                ? ji.radix : json_objectum_creare(c->piscina));
        }
        json_tabulatum_addere(content, b->blocus);
    }
    json_objectum_ponere(c->nuntius, "content", content);
    si (c->causa_visa)
    {
        json_objectum_ponere(c->nuntius, "stop_reason",
            json_chorda_creare(c->piscina, c->causa_finis));
    }
    si (c->output_visum && json_objectum_capere(c->nuntius, "usage"))
    {
        json_objectum_ponere(json_objectum_capere(c->nuntius, "usage"),
            "output_tokens", json_integer_creare(c->piscina, c->output));
    }
    redde _legere(CC, json_scribere(c->nuntius, c->piscina), c->piscina,
        &novitas);
}
```

  4. `vates_anthropic_rivum_legere`: collector + `RivusEventuum` over
     the whole body, `rivus_eventuum_finire`, `_collectorem_finire`.
  5. `vates_mittere_rivum`: copy `vates_mittere`'s loop
     (`lib/vates.c:1643-1726`) with these differences: set
     `petitio->rivus = VERUM` before rendering the body (reset to
     `FALSUM` before returning); per attempt a fresh collector + SSE
     parser, and a body sink `_rivus_http(datum, octeti, n)` =
     `rivus_eventuum_adicere`; transport `http_vectura_rivi_exsequi
     (vates->vectura_rivi, hp, _rivus_http, &sse, piscina)`; on
     transport failure the same error mapping, `iterandum` only when
     `collector.deltae == 0`; status != 200 -> `_legere(status,
     corpus)` exactly as today (no deltas were sent: R6); status 200
     -> `rivus_eventuum_finire` + `_collectorem_finire`, `iterandum =
     collector.deltae == 0 && collector.erratum &&
     (genus is "overloaded_error" or "api_error")`. Accounting,
     ledger, pretium as in `vates_mittere`.
  6. Fictus: `VatesFictumResponsum` gains `b32 rivus;`;
     `_fictus_exsequi_rivi(petitio, rivus, rivi_datum, piscina, datum)`
     = `_fictus_exsequi` + for `f->rivus` a `Content-Type:
     text/event-stream` header and, for 2xx, the body fed to `rivus`
     in pieces of `V` bytes. `vates_fictus_aperire` sets
     `v->vectura_rivi = { _fictus_exsequi_rivi, v }`.
     `vates_fictus_rivum` / `_fractum` build the SSE body with a
     builder: for each event, `"event: <typus>\ndata: " +
     json_scribere(obj) + "\n\n"`, objects built with the json API
     (message_start with id `msg_fictus_N`, model `fictus`, usage
     input/cache from `usus` and `output_tokens: 1`; one
     `content_block_start` text block; a `ping`; one
     `content_block_delta` per part; `content_block_stop`;
     `message_delta` with `stop_reason: end_turn` and
     `output_tokens: usus.output`; `message_stop`). The `_fractum`
     variant stops after the parts and emits
     `event: error\ndata: {"type":"error","error":{"type":...,
     "message":...}}`.

- [ ] **Step 4.4: run, see it pass** - `./compile_tests.sh vates`
  (probatio_vates, probatio_vates_rivus, probatio_herbarium,
  probatio_quaestio all green).

- [ ] **Step 4.5: plants** - `iterandum` ignoring `deltae`
  (`collector.deltae == 0 &&` removed) -> `error eventus` red (two
  requests); `_collectorem_finire` without the `output_tokens`
  overwrite -> `deltae` usage assert red.

- [ ] **Step 4.6: worklog + commit** (`include/vates.h`,
  `lib/vates.c`, `probationes/probatio_vates_rivus.c`,
  `lib/vates.worklog.md`; ludus_tessera hand list if vates is linked
  by vicus - it is, via quaestio: add `rivus_eventuum` there and run
  `./ludus_tessera/compile_probationes.sh`).

---

### Task 5: `quaestio` - planning pipeline, line budget, stream, frames

**Files:** Modify `include/quaestio.h`, `lib/quaestio.c`,
`probationes/probatio_quaestio.c`, the vicus port caller
(`apps/vicus/vicus.c:60`, signature only - T8 replaces it);
`lib/quaestio.worklog.md`.

**Interfaces:** Consumes T4 (`VatesRivus`, `vates_mittere_rivum`).
Produces (in `include/quaestio.h`; `characteres` becomes `lineae`):

```c
/* ASCII + Markdown -> lineae logicae (vide caput: forma) */
chorda
quaestio_textum_planare (
      chorda  crudum,
    Piscina* piscina);

/* lineae: lineae paginae pro textu; latitudo: columnae textus (post
 * margines). Systema formam paginae dicit; max_tokens = lineae *
 * latitudo / III + LXIV */
QuaestioResponsum
quaestio_rogare (
        Vates* vates,
       chorda  exemplar,
       chorda  quaestio,
          s32  lineae,
          s32  latitudo,
      Piscina* piscina);

/* idem per rivum: deltae CRUDAE textus (non cogitationis) per rivum;
 * responsum finale ut quaestio_rogare (textus planatus) */
QuaestioResponsum
quaestio_rogare_rivum (
        Vates* vates,
       chorda  exemplar,
       chorda  quaestio,
          s32  lineae,
          s32  latitudo,
   VatesRivus  rivus,
       vacuum* datum,
      Piscina* piscina);

/* ---- tabulae adiutoris (bin/quaestio -> vicus), II.6 ----
 * "T <n>\n<n octeti>" textus, "E <n>\n<n octeti>" error,
 * "U <input> <output>\n" usus, "F <causa>\n" finis */
nomen enumeratio {
    QUAESTIO_TABULA_TEXTUS = 0,
    QUAESTIO_TABULA_ERROR,
    QUAESTIO_TABULA_USUS,
    QUAESTIO_TABULA_FINIS
} QuaestioTabulaGenus;

nomen structura {
    QuaestioTabulaGenus  genus;
                 chorda  textus;   /* TEXTUS, ERROR; FINIS: causa */
                    s64  input;
                    s64  output;
} QuaestioTabula;

nomen enumeratio {
    QUAESTIO_LECTIO_TABULA = 0,   /* *tabula impleta, *positio promota */
    QUAESTIO_LECTIO_PLURA,        /* tabula imperfecta: exspecta */
    QUAESTIO_LECTIO_PRAVA         /* forma prava */
} QuaestioLectio;

chorda
quaestio_tabulam_scribere (
    constans QuaestioTabula* tabula,
                   Piscina* piscina);

QuaestioLectio
quaestio_tabulam_legere (
            chorda  fluxus,
              i32* positio,
    QuaestioTabula* tabula,
          Piscina* piscina);

/* adiutor: rogare_rivum, tabulae per scriptorem (stdout in bin/quaestio):
 * T per deltam STATIM; successus: U + "F <causa_finis_cruda>"; error
 * (ante aut post textum): E + "F error". Semper F ultima. */
nomen vacuum (*QuaestioScriptor)(vacuum* datum, chorda octeti);

vacuum
quaestio_adiutor_currere (
              Vates* vates,
             chorda  exemplar,
             chorda  quaestio,
                s32  lineae,
                s32  latitudo,
    QuaestioScriptor scriptor,
             vacuum* datum,
            Piscina* piscina);
```

- [ ] **Step 5.1: failing tests** - `probationes/probatio_quaestio.c`:
  - `petitio` section: call with `lineae = X`, `latitudo = LVI`;
    assert the system text contains `"The page has 10 lines of 56
    characters"`, `"a blank line between paragraphs uses a whole
    line"`, `"Light Markdown is fine"`, and `"\"max_tokens\":250"`
    (`10 * 56 / 3 + 64`); and does NOT contain `"no Markdown"`.
  - all other existing sections: replace the `D, LX` arguments by
    `X, LVI` (behaviour unchanged; `markdown` section's exact
    expectation stays).
  - new `planare` section: `quaestio_textum_planare` on the
    `markdown` section's input equals that section's expected text.
  - new `rivus` section: fictus stream `{"# T", "itle\n\nSal", "ve"}`
    -> deltas `"# T|itle\n\nSal|ve|"`, final `r.textus` = `"# Title\n\nSalve"`.
  - new `tabulae` section: write T("ab\ncd"), U(12, 34), E("x"),
    F("end_turn"); concatenate; read back at every 2-split of the
    stream (feeding prefix then whole) -> same four frames;
    a prefix ending mid-frame -> `PLURA`; `"Q 3\n"` -> `PRAVA`;
    `"T zz\n"` -> `PRAVA`.
  - new `adiutor` section: fictus stream `{"a", "b"}` + usage ->
    collected frames are `T a`, `T b`, `U 12 34`, `F end_turn` in
    order; fictus 529 -> `E` (contains "Overloaded") then
    `F error`; a `vates_fictus_rivum_fractum` after one part ->
    `T`, `E`, `F error`.

- [ ] **Step 5.2: run, see it fail** - `./compile_tests.sh quaestio`
  -> implicit declarations / prompt asserts.

- [ ] **Step 5.3: implementation** (`lib/quaestio.c`):
  1. `quaestio_textum_planare(crudum) = markdown_planare(
     ad_ascii(crudum))`; `quaestio_rogare`'s last step calls it.
  2. One interior `petitionem_struere(exemplar, quaestio, lineae,
     latitudo, piscina)` for both paths, with
     `vates_petitio_maxima_ponere(petitio, lineae * latitudo / III +
     LXIV)` and the system text:

```c
    sprintf(systema, "Answer in plain ASCII. Light Markdown is fine "
        "(headings, lists, code blocks); it will be reflowed to the "
        "page. The page has %d lines of %d characters. Every paragraph "
        "starts on a new line, a blank line between paragraphs uses a "
        "whole line, and a short last line still uses the whole line, "
        "so prefer fewer, fuller paragraphs. Be direct.",
        (integer)lineae, (integer)latitudo);
```

     (`systema` buffer: grow to `DCCLXVIII`.)
  3. `quaestio_rogare_rivum`: same as `quaestio_rogare` but
     `vates_mittere_rivum` with an adapter that forwards only
     `cogitatio == FALSUM` deltas to the caller's rivus.
  4. Frames: writer with `sprintf` headers; reader: find `'\n'` from
     `*positio` (none -> PLURA), parse the letter and numbers (strict:
     digits only; else PRAVA), for T/E check `n` bytes available
     (else PLURA), fill `tabula` (textus copied into `piscina`),
     advance `*positio`.
  5. `quaestio_adiutor_currere`: rivus adapter writes a T frame per
     delta via `scriptor`; at the end U + F, or E + `F error`.

- [ ] **Step 5.4: run, see it pass** - `./compile_tests.sh quaestio`;
  `./compile_tests.sh vicus` still compiles (update the one call in
  `apps/vicus/vicus.c` to the new parameters only, passing the line
  count and text width the port receives - T8 replaces this port).

- [ ] **Step 5.5: plants** - reader accepting a T frame before all
  `n` bytes arrived (PLURA check removed) -> `tabulae` split cases
  red; adiutor without the final F -> `adiutor` red.

- [ ] **Step 5.6: worklog + commit.**

---

### Task 6: helper `bin/quaestio`

**Files:** Create `apps/quaestio/quaestio.c`; Modify
`apps/vicus/vicus.sh`; `lib/quaestio.worklog.md`.

**Interfaces:** Consumes T5 (`quaestio_adiutor_currere`). Produces the
binary `bin/quaestio` with arguments `-exemplar X -lineae L -latitudo
W -quaestio <text>` writing frames to stdout.

- [ ] **Step 6.1: the helper** - `apps/quaestio/quaestio.c` (no unit
  test: all logic is in `quaestio_adiutor_currere`, tested in T5; the
  binary is checked by hand in 6.3 and live in T8):

```c
/* quaestio.c - adiutor $question (quaestio-rivus-spec II.6): rogatio
 * per rivum, tabulae in stdout. Clavis ex $HOME/.rhubarb/anthropic.clavis
 * SOLUM hic legitur (numquam argv, numquam stdout); post
 * vates_aperire nullatur. Exitus 0 post F; 2 si argumenta prava (E + F
 * tamen scribuntur). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "vates.h"
#include "quaestio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior vacuum
_scribere (
    vacuum* datum,
    chorda  octeti)
{
    (vacuum)datum;
    (vacuum)fwrite(octeti.datum, I, (size_t)octeti.mensura, stdout);
    (vacuum)fflush(stdout);
}

interior vacuum
_errorem_scribere (
    constans character* causa,
              Piscina* piscina)
{
    QuaestioTabula t;

    memset(&t, ZEPHYRUM, magnitudo(t));
    t.genus   = QUAESTIO_TABULA_ERROR;
    t.textus  = chorda_ex_literis(causa, piscina);
    _scribere(NIHIL, quaestio_tabulam_scribere(&t, piscina));
    t.genus   = QUAESTIO_TABULA_FINIS;
    t.textus  = chorda_ex_literis("error", piscina);
    _scribere(NIHIL, quaestio_tabulam_scribere(&t, piscina));
}

s32
principale (
          s32  argc,
    character** argv)
{
          Piscina* piscina;
    constans character* exemplar = "";
    constans character* quaestio = NIHIL;
              s32  lineae   = ZEPHYRUM;
              s32  latitudo = ZEPHYRUM;
              s32  k;
        character  via[DXII];
        character  clavis[DXII];
             FILE* f;
           size_t  n;
            Vates* vates;
    VatesOptiones  optiones;
    constans character* domus;

    piscina = piscina_generare_dynamicum("quaestio", CXXVIII * M);
    si (!piscina)
    {
        redde I;
    }
    per (k = I; k + I < argc; k += II)
    {
        si (strcmp(argv[k], "-exemplar") == ZEPHYRUM)
        {
            exemplar = argv[k + I];
        }
        alioquin si (strcmp(argv[k], "-lineae") == ZEPHYRUM)
        {
            lineae = (s32)atoi(argv[k + I]);
        }
        alioquin si (strcmp(argv[k], "-latitudo") == ZEPHYRUM)
        {
            latitudo = (s32)atoi(argv[k + I]);
        }
        alioquin si (strcmp(argv[k], "-quaestio") == ZEPHYRUM)
        {
            quaestio = argv[k + I];
        }
    }
    si (!quaestio || lineae <= ZEPHYRUM || latitudo <= ZEPHYRUM)
    {
        _errorem_scribere("usus: quaestio -exemplar X -lineae L "
            "-latitudo W -quaestio <textus>", piscina);
        redde II;
    }
    domus = getenv("HOME");
    si (!domus || strlen(domus) > CD)
    {
        _errorem_scribere("HOME deest", piscina);
        redde ZEPHYRUM;
    }
    sprintf(via, "%s/.rhubarb/anthropic.clavis", domus);
    f = fopen(via, "rb");
    si (!f)
    {
        _errorem_scribere("no API key (~/.rhubarb/anthropic.clavis)",
            piscina);
        redde ZEPHYRUM;
    }
    n = fread(clavis, I, magnitudo(clavis) - I, f);
    (vacuum)fclose(f);
    dum (n > ZEPHYRUM && (clavis[n - I] == '\n' || clavis[n - I] == '\r'
        || clavis[n - I] == ' '))
    {
        n--;
    }
    optiones = vates_optiones_ordinariae();
    vates = vates_aperire(piscina, chorda_ex_literis("anthropic",
        piscina), chorda_ex_buffer((i8*)clavis, (i32)n), &optiones);
    memset(clavis, ZEPHYRUM, magnitudo(clavis));
    si (!vates)
    {
        _errorem_scribere("vates aperiri non potuit", piscina);
        redde ZEPHYRUM;
    }
    quaestio_adiutor_currere(vates, chorda_ex_literis(exemplar, piscina),
        chorda_ex_literis(quaestio, piscina), lineae, latitudo,
        _scribere, NIHIL, piscina);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
```

  `chorda_ex_buffer` (`include/chorda.h:29`) points into `clavis`
  without copying; `vates_aperire` keeps its one NUL-terminated copy,
  so zeroing `clavis` right after is safe. Compare with today's
  in-process read at `apps/vicus/vicus.c:60` (deleted in T8).

- [ ] **Step 6.2: build wiring** - `apps/vicus/vicus.sh`, before
  building vicus:

```bash
./bin/aedilis apps/quaestio/quaestio.c >&2 || exit 1
bash build/aedilis/quaestio/struere.sh >&2 || exit 1
mkdir -p bin && rm -f bin/quaestio \
    && cp build/aedilis/quaestio/quaestio bin/quaestio
```

- [ ] **Step 6.3: by-hand check (no key needed)** -
  `AEDIFICARE_SOLUM=1 ./apps/vicus/vicus.sh` then
  `bin/quaestio -lineae 5 | od -c | head` -> `E <n>\n usus: ...` and
  `F error\n`, exit code 2; `HOME=/nonexistent bin/quaestio -lineae 5
  -latitudo 40 -quaestio q` -> `E ... no API key ...`, `F error`,
  exit 0. (Claude runs these; they never touch the real key file.)

- [ ] **Step 6.4: lint + commit** (`apps/quaestio/quaestio.c`,
  `apps/vicus/vicus.sh`, worklog).

---

### Task 7: `IussumOpus` + scriba running state

**Files:** Modify `include/iussum.h`, `include/scriba_actiones.h`,
`lib/scriba_actiones.c`, `lib/scriba_applicatio.c`,
`lib/scriba_figurae.c`, `probationes/probatio_vicus_iussa.c`;
worklogs `lib/iussum.worklog.md`, `lib/scriba_actiones.worklog.md`.

**Interfaces:** Produces (in `include/iussum.h`):

```c
/* OPUS CURRENS (quaestio-rivus-spec II.7): iussum longum. Hospes
 * (scriba) paginam claudit, folium ante opus servat, per pulsum
 * pulsare vocat et folium = basis + textus hactenus reficit; FINITUM:
 * caput + textus, commissio UNA; IRRITUM: basis restituitur, nihil
 * committitur (nuntius solum); Esc: abrumpere, deinde ut FINITUM. */
nomen enumeratio {
    IUSSUM_OPUS_CURRIT = 0,
    IUSSUM_OPUS_FINITUM,
    IUSSUM_OPUS_IRRITUM
} IussumOpusStatus;

nomen structura IussumOpus IussumOpus;

structura IussumOpus {
    vacuum* datum;
    /* textus TOTUS hactenus (ut effectus->textus) in piscina data;
     * *mutatum VERUM si textus mutatus; *nuntius vacuus = nihil novi */
    IussumOpusStatus (*pulsare)(IussumOpus* opus, Piscina* piscina,
                                chorda* textus, chorda* nuntius,
                                b32* mutatum);
    /* Esc: opus sistitur; textus finalis, nuntius */
    vacuum (*abrumpere)(IussumOpus* opus, Piscina* piscina,
                        chorda* textus, chorda* nuntius);
};

/* IussumEffectus gains (after 'differre'): */
    IussumOpus* opus;   /* non NIHIL: effectus currit (textus
                         * per pulsus); caput in fine applicatur */
```

`include/scriba_actiones.h` - `ScribaActiones` gains:

```c
    /* opus currens (quaestio-rivus III): NIHIL nullum; pagina clausa */
           IussumOpus* opus;
                Iussum iussum_operis;
                chorda caput_operis;
                   b32 consumit_operis;
                   i8* basis_cellulae;    /* folium ante opus */
                  s32* basis_indentatio;
              Piscina* piscina_operis;    /* pulsus: vacatur */
```

and a new function `b32 scriba_opus_pulsare (ScribaActiones* sa);`
(called from `scriba_reficere`).

- [ ] **Step 7.1: failing test** - `probationes/probatio_vicus_iussa.c`
  new section `opus` (same harness as `_probare_dilatum`):

```c
/* opus: verbum 'lente' differt, deinde opus fictum reddit quod per
 * pulsus "a" -> "a b" -> "a b c" crescit et finit */
nomen structura {
    IussumOpus  opus;
           s32  pulsus;
           b32  abruptum;
} OpusFictum;

interior IussumOpusStatus
_opus_pulsare (
    IussumOpus* opus,
       Piscina* p,
        chorda* textus,
        chorda* nuntius,
           b32* mutatum)
{
    OpusFictum* o = (OpusFictum*)opus->datum;
    hic_manens constans character* GRADUS[III] = { "\na", "\na b",
        "\na b c" };

    o->pulsus++;
    *textus   = chorda_ex_literis(GRADUS[(o->pulsus - I) % III], p);
    *nuntius  = chorda_ex_literis(o->pulsus >= III ? "lente: facta"
                                                   : "lente: currit", p);
    *mutatum  = VERUM;
    redde o->pulsus >= III ? IUSSUM_OPUS_FINITUM : IUSSUM_OPUS_CURRIT;
}

interior vacuum
_opus_abrumpere (
    IussumOpus* opus,
       Piscina* p,
        chorda* textus,
        chorda* nuntius)
{
    OpusFictum* o = (OpusFictum*)opus->datum;

    o->abruptum  = VERUM;
    *textus      = chorda_ex_literis("\na\n[cancelled]", p);
    *nuntius     = chorda_ex_literis("lente: cancelled", p);
}

interior b32
lente_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* p,
     IussumEffectus* effectus)
{
    OpusFictum* o = (OpusFictum*)ctx;

    si (!iussum->iterum)
    {
        effectus->differre  = VERUM;
        effectus->nuntius   = chorda_ex_literis("lente: incipit", p);
        redde VERUM;
    }
    o->opus.datum      = o;
    o->opus.pulsare    = _opus_pulsare;
    o->opus.abrumpere  = _opus_abrumpere;
    effectus->opus     = &o->opus;
    effectus->caput    = chorda_ex_literis("<lente facta>", p);
    effectus->nuntius  = chorda_ex_literis("lente: currit (Esc)", p);
    redde VERUM;
}

interior vacuum
_probare_opus (
    CredoContextus* c)
{
    MundusApplicationis* ms   = (MundusApplicationis*)c->datum;
        VicusApplicatio* app  = &ms->app;
                  Vicus* v    = ms->v;
             ManusLudus* m    = ms->m;
         ScribaMontatio* sin  = ms->sin;
             OpusFictum  o;
                    s64  cursor;

    memset(&o, ZEPHYRUM, magnitudo(o));
    CREDO_NECESSE(iussum_registrare(sin->actiones_ctx.iussa, "lente",
        FALSUM, lente_iussum, &o));
    ictus(m, XII, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$lente<>q</>"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    cursor = scriba_documentum_cursor(sin->actiones_ctx.doc);
    ictus(m, XII, I);
    (vacuum)vicus_pulsare(v);   /* gradus I: pictura */
    (vacuum)vicus_pulsare(v);   /* gradus II: opus incipit */
    dispensator_recomponere(app->d);
    CREDO_VERUM(sin->actiones_ctx.opus != NIHIL);
    CREDO_VERUM(nuntius_est(v, sin, "lente: currit"));
    /* pulsus: crescit, nihil commissum */
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(linea_est(sin, XIII, "a"));
    CREDO_VERUM(scriba_documentum_cursor(sin->actiones_ctx.doc)
        == cursor);
    /* clausa: claves negleguntur */
    CREDO_VERUM(manus_ludus_scribere(m, "dd"));
    CREDO_VERUM(linea_est(sin, XII, "$lente<>q</>"));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(linea_est(sin, XIII, "a b"));
    /* finis: caput, textus, commissio UNA */
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app->d);
    CREDO_VERUM(sin->actiones_ctx.opus == NIHIL);
    CREDO_VERUM(linea_est(sin, XII, "<lente facta>q</>"));
    CREDO_VERUM(linea_est(sin, XIII, "a b c"));
    CREDO_VERUM(nuntius_est(v, sin, "lente: facta"));
    CREDO_VERUM(scriba_documentum_cursor(sin->actiones_ctx.doc)
        == cursor + I);
    /* 'u': totum restituitur */
    ictus(m, XX, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "u"));
    CREDO_VERUM(linea_est(sin, XII, "$lente<>q</>"));
}

/* Esc: abrumpere, textus finalis committitur */
interior vacuum
_probare_opus_esc (
    CredoContextus* c)
{
    MundusApplicationis* ms   = (MundusApplicationis*)c->datum;
                  Vicus* v    = ms->v;
             ManusLudus* m    = ms->m;
         ScribaMontatio* sin  = ms->sin;
             OpusFictum  o;

    memset(&o, ZEPHYRUM, magnitudo(o));
    CREDO_NECESSE(iussum_registrare(sin->actiones_ctx.iussa, "lente",
        FALSUM, lente_iussum, &o));
    ictus(m, XII, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$lente<>q</>"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    ictus(m, XII, I);
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(o.abruptum);
    CREDO_VERUM(sin->actiones_ctx.opus == NIHIL);
    CREDO_VERUM(linea_est(sin, XII, "<lente facta>q</>"));
    CREDO_VERUM(linea_est(sin, XIII, "a"));
    CREDO_VERUM(linea_est(sin, XIV, "[cancelled]"));
}
```

  Plus a `color` check in `_probare_opus` while running: render
  (`nodus_status`-style helper: build the tree, `pingere`,
  `delineare_mandata` into a `TabulaPixelorum` as the file's
  `pixela_coloris` callers do) and assert a free cell of line XX is
  `color_ad_pixelum(thema_color(COLOR_SUPERFICIES))` while running
  and `COLOR_BACKGROUND` after the finish. Register both new sections
  in `SECTIONES`.

- [ ] **Step 7.2: run, see it fail** - `./compile_tests.sh vicus_iussa`
  -> `no member named 'opus'`.

- [ ] **Step 7.3: implementation.**
  1. `include/iussum.h`: the block above; `IussumEffectus.opus`.
     Every place that zeroes an `IussumEffectus` (memset) keeps it
     `NIHIL` - check `lib/iussum.c` for designated field copies.
  2. `lib/scriba_actiones.c`:
     - in `scriba_iussum_dilatum_currere`, in the success branch
       (`lib/scriba_actiones.c:1324`), before `effectum_applicare`:

```c
        alioquin si (eff.opus)
        {
            const s32 n = (s32)(sa->laboris.latitudo
                * sa->laboris.altitudo);
            /* opus currens: basis servatur, pagina clauditur */
            sa->opus             = eff.opus;
            sa->iussum_operis    = iussum;
            sa->caput_operis     = eff.caput;
            sa->consumit_operis  = iussum_consumit(sa->iussa,
                iussum.verbum);
            sa->basis_cellulae   = (i8*)piscina_allocare(
                sa->doc->piscina, (memoriae_index)n);
            sa->basis_indentatio = (s32*)piscina_allocare(
                sa->doc->piscina,
                (memoriae_index)sa->laboris.altitudo * magnitudo(s32));
            memcpy(sa->basis_cellulae, sa->laboris.cellulae,
                (memoriae_index)n);
            memcpy(sa->basis_indentatio, sa->laboris.indentatio,
                (memoriae_index)sa->laboris.altitudo * magnitudo(s32));
            sa->piscina_operis = piscina_generare_dynamicum(
                "scriba_opus", LXIV * M);
            si (ramus.repo)
            {
                nuntium_ponere(&ramus, eff.nuntius);
            }
        }
```

       (C89: no `const` local initialised from an expression after
       statements - declare `n` at the top of the block as `s32 n;`
       and assign; `const` is `constans` anyway.)
     - `scriba_opus_pulsare(sa)`: if `!sa->opus` return FALSUM;
       `piscina_vacare(sa->piscina_operis)`; call `pulsare`; on
       `mutatum` or a final status: restore the basis (memcpy back),
       then `effectum_applicare(&sa->laboris, &sa->iussum_operis,
       sa->consumit_operis, final ? sa->caput_operis : vacua, textus,
       &l, &col)`; on FINITUM: `scriba_documentum_committere`,
       `cursor_laboris` update, nuntius, `_opus_finire(sa)` (destroy
       `piscina_operis`, `opus = NIHIL`); on IRRITUM: restore basis
       only, nuntius, `_opus_finire`. Nuntius writes go under scriptor
       `"pagina.clavis"` exactly as `scriba_iussum_dilatum_currere`
       does (`lib/scriba_actiones.c:1279-1286` - copy that block).
     - `scriba_pagina_clavis`: first thing, `si (sa->opus)`: a key
       event with `CLAVIS_EFFUGIUM` -> `abrumpere` then the FINITUM
       path (shared helper `_opus_claudere(sa, textus, nuntius)`);
       return VERUM for EVERY event while running (keys and mouse:
       "ignored", spec III.4).
     - `scriba_iussum_ad_punctum`: return FALSUM while `sa->opus`.
  3. `lib/scriba_applicatio.c:258`: after the dilatum call,
     `mutatum = scriba_opus_pulsare(sa) || mutatum;`.
  4. `lib/scriba_figurae.c:182`: page fill
     `color_thematis(sf->sa->opus ? COLOR_SUPERFICIES :
     COLOR_BACKGROUND)`.

- [ ] **Step 7.4: run, see it pass** - `./compile_tests.sh vicus_iussa`
  (5/5 sections), `./compile_tests.sh scriba`, `./compile_tests.sh
  iussum`.

- [ ] **Step 7.5: plants** - the lock (`return VERUM` for every event
  while running) removed -> `dd` deletes line XII -> red; FINITUM
  without restoring the basis first -> `a b c` drawn over `a b`
  remnants or line XIV wrong -> red; figure fill unconditional ->
  color assert red.

- [ ] **Step 7.6: worklogs + commit.**

---

### Task 8: vicus `$question` on the stream + live check

**Files:** Modify `include/vicus_applicatio.h`,
`lib/vicus_applicatio.c`, `apps/vicus/vicus.c`,
`probationes/probatio_vicus_quaestio.c`; worklogs
`lib/vicus_applicatio.worklog.md`, `lib/quaestio.worklog.md`.

**Interfaces:** Consumes T5 (`quaestio_textum_planare`, frames), T6
(binary), T7 (`IussumOpus`). Produces (replacing the blocking
`VicusPortaQuaestionis`):

```c
/* $question per rivum (quaestio-rivus-spec II.8): porta incipit,
 * pulsat, abrumpit - numquam obstat. NIHIL = nulla (gemellus
 * terminalis, probationes sine porta). */
nomen structura {
     b32  finitus;      /* F visa (aut adiutor mortuus) */
  chorda  textus_novus; /* crudum NOVUM post pulsum priorem */
  chorda  error;        /* non vacuum: defectus */
     s64  input;
     s64  output;
} VicusPulsusQuaestionis;

nomen structura {
    /* opus incipere; NIHIL + *error si nequit */
    vacuum* (*incipere)(vacuum* ctx, chorda quaestio, s32 lineae,
                        s32 latitudo, Piscina* piscina, chorda* error);
    VicusPulsusQuaestionis (*pulsare)(vacuum* manubrium,
                                      Piscina* piscina);
    vacuum (*abrumpere)(vacuum* manubrium);
    vacuum* ctx;
} VicusPortaQuaestionis;

vacuum
vicus_applicatio_quaestionis_portam_ponere (
          VicusApplicatio* app,
    constans VicusPortaQuaestionis* porta);   /* NIHIL = nulla */
```

- [ ] **Step 8.1: failing tests** - rewrite
  `probationes/probatio_vicus_quaestio.c`'s fake port as a scripted
  async port: `incipere` records `(quaestio, lineae, latitudo)` and
  returns a handle over a script (array of
  `VicusPulsusQuaestionis`, one per pulse); `abrumpere` sets a flag.
  Keep the existing non-port sections (`sine porta`, `forma`, `sine
  lineis`, `scriptura prava`) adapted to the new setter. New/changed
  sections:
  - `crescit`: script `"Salve "`, `"munde. "`, `"Iterum."` + finitus,
    usage 12/34 -> after stage I/II pulses the page shows `<answer>`
    and `Salve` on the line below the command, then `Salve munde.`,
    then the final text; nothing committed until the finish; then
    head `<question answered>`, status `question: 12 in, 34 out`,
    one undo restores the page.
  - `aequale`: the streamed final lines equal `"\n" +
    responsum_aptare(quaestio_textum_planare(<joined raw>), w, l)`
    for the same page (call through a test-visible function: expose
    `vicus_quaestio_disponere(chorda crudum, s32 w, s32 l, Piscina*)`
    in `include/vicus_applicatio.h` as the one layout entry point the
    verb uses - the test then compares the page lines with it).
  - `rogatum`: `incipere` got `lineae == free lines - 1` and
    `latitudo == page width - 2 * QUAESTIO_MARGO` (R8).
  - `plenum`: a script that keeps sending text past the free lines
    -> `abrumpere` called, the page ends at a sentence end within the
    free lines, status `question: page full`.
  - `esc`: Esc after one piece -> `abrumpere` called, lines =
    first piece + `[cancelled]`, head answered, status
    `question: cancelled`.
  - `error medio`: script piece then `error = "Overloaded"` ->
    piece + `[error: Overloaded]`, status `question: Overloaded`.
  - `error ante textum`: first pulse error -> page unchanged, head
    unchanged (still clickable), status `question: ...`.
  - `incipere nequit`: `incipere` returns NIHIL + error -> status
    shows it, nothing changes, no opus.

- [ ] **Step 8.2: run, see it fail** - `./compile_tests.sh
  vicus_quaestio`.

- [ ] **Step 8.3: implementation, verb** (`lib/vicus_applicatio.c`):
  - `ContextusQuaestionis` holds a copy of the port struct (`b32
    porta_posita`).
  - `vicus_quaestio_disponere(crudum, w, l, piscina)` = `"\n" +
    responsum_aptare(quaestio_textum_planare(crudum, piscina), w, l,
    piscina)` - one function, used for every pulse and the finish.
  - `OpusQuaestionis` (first member `IussumOpus opus`): port, handle,
    own dynamic piscina (`piscina_generare_dynamicum("quaestio_opus",
    LXIV * M)`), a `ChordaAedificator* crudum` in it, `w`, `l`,
    `b32 textus_visus`.
  - `pulsare`: `p = porta.pulsare(handle, own piscina)`; append
    `p.textus_novus`; if text grew: `planatum =
    quaestio_textum_planare(raw, piscina)`; if `responsum_disponere
    (planatum, w, FALSUM, NIHIL, &n) > l` -> `porta.abrumpere`,
    textus = `vicus_quaestio_disponere(raw, w, l)`, nuntius
    `question: page full`, FINITUM; else textus = layout, mutatum.
    `p.error` non-empty: no text yet -> nuntius `question: <error>`,
    IRRITUM; else textus = `vicus_quaestio_disponere(raw +
    "\n\n[error: <error>]", w, l)`, nuntius `question: <error>`,
    FINITUM. `p.finitus` -> textus = final layout, nuntius
    `question: <in> in, <out> out` (same format as today's
    `apps/vicus/vicus.c` port), FINITUM. On FINITUM/IRRITUM destroy
    the own piscina AFTER copying the textus into the pulse piscina.
  - `abrumpere`: `porta.abrumpere`; textus = `vicus_quaestio_disponere
    (raw + "\n\n[cancelled]", w, l)`; nuntius `question: cancelled`.
  - `quaestionem_iussum` stage II: `l_prompt = l - I` (min `I`),
    `w_textus = w - II * QUAESTIO_MARGO >= XX ? w - II *
    QUAESTIO_MARGO : w` (R8); `handle = porta.incipere(...)`;
    NIHIL -> `effectus->error = "question: " + error`; else
    `effectus->opus`, `effectus->caput = "<question answered>"`,
    `effectus->nuntius = "question: streaming... (Esc cancels)"`.
    The character budget code (`lib/vicus_applicatio.c:468-469`) is
    deleted.
- [ ] **Step 8.4: implementation, app** (`apps/vicus/vicus.c`): the
  port over `processus`: `incipere` builds argv `{helper, "-exemplar",
  QUAESTIO_EXEMPLAR, "-lineae", L, "-latitudo", W, "-quaestio", q,
  NIHIL}` (numbers via `sprintf` into handle buffers) and calls
  `processus_incipere(argv, CLXXX * M, piscina)`; `pulsare` calls
  `processus_pulsare`, then reads frames from
  `processus_effusio_hactenus` from the handle's offset with
  `quaestio_tabulam_legere` until `PLURA` (T -> append to
  `textus_novus` copied into the pulse piscina; E -> error; U ->
  usage; F -> finitus); if `PARATUS` and no F -> error `helper exited
  without answer` + the first CC bytes of
  `processus_erratum_hactenus`; `PRAVA` -> error `helper: bad frame`.
  `abrumpere` = `processus_abrumpere`. The helper path is computed in
  `principale` from `argv[0]` (R4): directory part + `/quaestio`, or
  `quaestio` when `argv[0]` has no `/`. The old in-process key read
  and `quaestio_rogare` call are deleted from vicus (the key is read
  only by the helper now).

- [ ] **Step 8.5: run, see it pass** - `./compile_tests.sh vicus`
  (all vicus suites), `./compile_tests.sh quaestio`,
  `./compile_tests.sh scriba`; `AEDIFICARE_SOLUM=1
  ./apps/vicus/vicus.sh` builds both binaries;
  `./ludus_tessera/compile_probationes.sh`.

- [ ] **Step 8.6: plants** - overflow check removed (`plenum` red);
  `[cancelled]` line not appended (`esc` red); `l_prompt = l`
  (`rogatum` red).

- [ ] **Step 8.7: live check by Fran** (key needed - Fran runs it):

```
! cd ../rhubarb-quarta && ./apps/vicus/vicus.sh
```

  1. Write `$question<>Explain the C89 rules for declarations in two
     short paragraphs.</>` with ~15 free lines below, click it.
  2. Within ~1-2 s text starts appearing under the command and grows;
     the page turns the desk gray; typing does nothing.
  3. When done: page back to normal color, head `<question answered>`,
     status shows usage; `u` removes the whole answer in one step.
  4. Ask a long question with only ~4 free lines: it stops itself at
     a sentence end (`question: page full`).
  5. Ask again and press Esc mid-answer: `[cancelled]` line, status
     `question: cancelled`.
  6. (Measures AUDIENDA 1 and 3) Note whether the text arrives in
     small steady pieces or big jumps - recorded in the worklog.

- [ ] **Step 8.8: worklogs (incl. Fran's observations) + commit**;
  then frigida on the branch head and, with Fran's go, ff main (the
  usual ritual: merge main in first if it moved; `bin/fabrica sanare
  installata` in main).

---

## Self-review (done while writing)

- **Spec coverage:** II.1 T1; II.2 T2; II.3 T3; II.4 T4 (incl. fictus,
  herbarium capture/replay T2, norma skip R5); II.5 T5; II.6 T5
  (frames) + T6 (binary); II.7 T7; II.8 T8; III T7 (lock, snapshot,
  color, Esc, one commit) + T8 (status texts, [cancelled]/[error],
  error before text, page full, 180 s limit = `CLXXX * M` in T8.4);
  IV tests per task; V out of scope; AUDIENDA 1/3 measured in T8.7,
  2 by the first live capture, 4 accepted.
- **Type consistency:** `HttpRivus` (T1) used by T2/T4;
  `HttpVecturaRivi` (T2) used by T4; `VatesRivus` (T4) used by T5;
  `QuaestioTabula*` (T5) used by T6/T8; `IussumOpus` (T7) used by T8;
  `VicusPortaQuaestionis` replaced in T8 (T5 only adapts the old one's
  call).
- **Known soft spots to check during execution** (not placeholders,
  but exact names to confirm against the code before compiling):
  `_parse_status_line` / `_parse_header` / `_hex_ad_valor` signatures
  (T1), the raw-fixture helper names in `probatio_http.c` (T2),
  builder `spectare`/`reset` (T3) and `chorda_ex_buffer` (T6) -
  verified while planning; `iussum_consumit` is public
  (`include/iussum.h:153`), `nuntium_ponere` interior in the same file
  as the T7 code (`lib/scriba_actiones.c:719`).
