# vates plan 2 - herbarium + vates implemented, proven live, merged

> **For agentic workers:** execution mode is FIXED by Fran's standing
> rule (memory `plan-execution-inline-checkpoints`): inline, one task per
> turn, Fran approves each before the next; no subagents. Steps use
> checkbox (`- [ ]`) syntax for tracking.

**Goal:** implement the two approved headers (`include/herbarium.h`,
`include/vates.h`, 7b5ca52b), prove vates against the real API
(one call, cache read on the second identical prefix, thinking
round-trip), and merge `rhubarb-quarta` into main.

**Architecture:** one durable-append primitive lands in its home
library (`filum`) first, because both new libraries and optchat need
it. herbarium is generic over `HttpVectura` (capture wrapper + replay
transport). vates is one file (`lib/vates.c`, aedilis convention
`include/X.h -> lib/X.c`): neutral model, Anthropic render + parse
(pure, golden-tested), a by-name provider table, `vates_mittere` with
retries and accounting, and fictus = the Anthropic backend over a
scripted vectura. Every response block keeps its provider JSON
(`crudum`) and is re-emitted verbatim to the same provider: nothing is
dropped, unknown fields survive.

**Tech stack:** C89 + latina.h; house `http`, `json`, `filum`, `xar`,
`sigillum`, `fasti`, `iter_directoria`, `chorda_aedificator`; credo.

**Specs:** `project-specs/vates-spec.md` (incl. §I.4 provider
principle), `project-specs/herbarium-spec.md`. Headers are the
contract: a deviation from them needs Fran, not a ruling.
Plan 1 record: `build/vates-plan-1.progress.md` (quarta).

## Global constraints

- Flags `tools/vexilla.sh` (C89, -Werror, -Wconversion,
  -Wsign-conversion, -Wcast-qual, -Wstrict-prototypes,
  -Wmissing-prototypes, -Wwrite-strings).
- Latin identifiers; NEVER a latina.h macro word as an identifier
  (`nomen per casus duplex magnitudo registrum interior ordinarius
  ...`; T3 of plan 1 tripped on `interior`) - compile-check every new
  header in a TU, examen alone misses it (quaestio …2Y6MT). No
  `_Capital` identifiers. Uppercase Roman numerals are macros.
- `chorda` not NUL-terminated; `i32` UNSIGNED, `s32`/`s64` signed.
- POSIX `.c`: `postulata_posix.h` FIRST.
- The API key: copied ONCE into the `Vates` (NUL-terminated, for the
  header), never into the ledger, herbarium, error text or stdout.
- Worktree commits: absolute-path guard
  (`os.chdir(Q); sys.path.insert(0, Q+'/pythonica'); assert
  silva.__file__.startswith(Q+'/')`), gates named by hand,
  `sine_debitis='<cause>'`, never `~/.bin` installs, never a parallel
  Bash call that cds elsewhere while a commit runs.
- New `lib/*.c` -> regenerate `compile_tests_fontes_generata.sh`
  (`./tools/compile_tests_fontes_generare.sh`) in the same commit.
- Every behaviour born red by a plant that COMPILES; restore with plain
  `cp`/edit; `cmp` against the saved good copy.

## Review focus

1. **The key leaking into an artefact.** Ledger lines, herbarium
   specimens and error text must never contain it. T5 test
   `probatio_clavis_non_effunditur` uses a distinctive key through a
   scripted vectura and greps every written file.
2. **Non-ASCII text through render and parse.** Latin with macrons and
   a non-BMP character must round-trip byte-exact (JSON escapes are
   the classic place to lose it). T3 golden test includes
   `"Ave, cārissime ✝ 𝔙"`.
3. **A hostile `retry-after`** (e.g. 3600) must be capped by
   `mora_iterandi_maxima_ms`, not obeyed. T5 test with a 5 ms cap
   asserts the whole call returns quickly.
4. **Capture failure must not break the request.** Directory made
   unwritable mid-run: the response still returns intact, stderr once.
   T2 test `probatio_captura_defectus`.
5. **An assistant message whose only blocks are another provider's
   OPACUM** renders to an empty content array, which the API rejects.
   The message is omitted; JSON stays valid. T3 test.

---

### Task 1: `filum_appendere_firmiter` - durable append (home library)

**Why here:** `filum_scriptor_sync` is `fflush` only (`include/filum.h:108`
says "Sync ad discum (fflush)"); the vates ledger, herbarium's sighting
index and optchat's log (Taelin §2) need write + fsync.

**Files:** Modify `include/filum.h` (after `filum_scribere_literis`),
`lib/filum.c` (end), `probationes/probatio_filum.c`, `lib/filum.worklog.md`
(create if absent).

**Interfaces:** Produces
`b32 filum_appendere_firmiter (constans character* via, chorda contentum);`

- [ ] **Step 1.1: failing test** - add to `probationes/probatio_filum.c`
  before `principale`, call it from `principale` (read the file first;
  follow its existing call pattern and temp-path convention if it has
  one, else use the `/tmp/... getpid()` form below and unlink at end):

```c
interior vacuum
probatio_appendere_firmiter(Piscina* piscina)
{
    character via[CCLVI];
    chorda    lectum;

    imprimere("\n--- Probans filum_appendere_firmiter ---\n");
    sprintf(via, "/tmp/probatio_filum_firmiter_%ld.txt", (longus)getpid());
    (vacuum)unlink(via);

    /* creat si abest, deinde appendit - ordo servatur */
    CREDO_VERUM(filum_appendere_firmiter(via,
        chorda_ex_literis("prima\n", piscina)));
    CREDO_VERUM(filum_appendere_firmiter(via,
        chorda_ex_literis("secunda\n", piscina)));
    lectum = filum_legere_totum(via, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "prima\nsecunda\n");

    /* contentum vacuum: filum manet, nihil additur */
    CREDO_VERUM(filum_appendere_firmiter(via, chorda_ex_literis("", piscina)));
    lectum = filum_legere_totum(via, piscina);
    CREDO_AEQUALIS_I32(lectum.mensura, XV);

    /* directorium absens -> FALSUM, non ruina */
    CREDO_FALSUM(filum_appendere_firmiter(
        "/tmp/probatio_filum_absens_directorium/x.txt",
        chorda_ex_literis("x", piscina)));

    (vacuum)unlink(via);
}
```

- [ ] **Step 1.2: run, expect compile failure**
  Run `./compile_tests.sh probatio_filum` - Expected: implicit
  declaration of `filum_appendere_firmiter`.

- [ ] **Step 1.3: declare** in `include/filum.h`:

```c
/* Appendere contentum ad filum (creatur si abest, modus 0644) per
 * write() UNUM deinde fsync() - in disco ante reditum, aut FALSUM.
 * Pro annalibus 'append-only' (rationarium vatis, index herbarii,
 * acta optchat). O_APPEND: lineae < PIPE_BUF (4096) a processibus
 * pluribus scriptae non intermiscentur. Contentum vacuum: filum
 * creatur, nihil scribitur, fsync tamen. */
b32
filum_appendere_firmiter (
    constans character* via,
                chorda  contentum);
```

- [ ] **Step 1.4: implement** at the end of `lib/filum.c` (add
  `#include <fcntl.h>` / `<unistd.h>` if absent; `postulata_posix.h`
  must already be first - check):

```c
b32
filum_appendere_firmiter (
    constans character* via,
                chorda  contentum)
{
    integer descriptor;
    ssize_t scripta = 0;
        b32 bene;

    si (!via || (contentum.mensura > 0 && !contentum.datum))
    {
        redde FALSUM;
    }
    descriptor = open(via, O_WRONLY | O_APPEND | O_CREAT, 0644);
    si (descriptor < 0)
    {
        redde FALSUM;
    }
    si (contentum.mensura > 0)
    {
        scripta = write(descriptor, contentum.datum,
                        (size_t)contentum.mensura);
    }
    bene = (scripta == (ssize_t)contentum.mensura)
        && (fsync(descriptor) == 0);
    si (close(descriptor) != 0)
    {
        bene = FALSUM;
    }
    redde bene;
}
```

- [ ] **Step 1.5: green** - `./compile_tests.sh probatio_filum` PASS.
- [ ] **Step 1.6: plants** - (a) `O_APPEND` -> `O_TRUNC`: red on
  "prima\nsecunda\n"; (b) return `VERUM` when `open` fails: red on the
  absent-directory assertion. Restore, `cmp`.
- [ ] **Step 1.7: worklog + commit.** Note in `lib/filum.worklog.md`
  (why: sync = fflush only; who needs it). Gates: `radix`, `generata`
  (filum.h is widely included - also run the grep from plan 1 T2
  Step 2.8 for subsystem suites including `filum.h` and name them).
  Use `commissio_umbra`.

---

### Task 2: herbarium

**Files:** Create `lib/herbarium.c`, `probationes/probatio_herbarium.c`,
`lib/herbarium.worklog.md`; regenerate `compile_tests_fontes_generata.sh`.

**Interfaces:** Consumes `HttpVectura`, `http_vectura_exsequi`,
`http_petitio_visus`, `http_responsum_caput`, `http_status_descriptio`,
`http_methodus_nomen` (http), `filum_appendere_firmiter` (T1),
`filum_scribere`, `filum_legere_totum`, `filum_existit`,
`filum_directorium_creare_cum_parentibus`, `sigillum_computare`,
`sigillum_hex`, `fasti_nunc`, `fasti_ad_iso`,
`directorium_iterator_*`, `chorda_minuscula`, json builder/reader.
Produces everything in `include/herbarium.h` (unchanged).

Design decisions inside the header's freedom (record in worklog):
- variants differ by MASKED body: digit runs -> `#`, runs of
  `[A-Za-z0-9_-]` of length >= 12 containing a digit -> `@`;
- default header allowlist `content-type`, `retry-after`,
  `request-id`, `x-request-id` (compared lowercased), plus the caller's;
- each press uses its own temporary piscina (sightings must not grow
  the herbarium's arena forever);
- specimen files are pretty-printed JSON (they are read by people at
  promotion time); the sighting index is compact JSONL via
  `filum_appendere_firmiter`;
- `herbarium_vectura(NIHIL, x)` returns `x` (no capture, no failure).

- [ ] **Step 2.1: failing tests** - create
  `probationes/probatio_herbarium.c` (`./silva/scribe.sh`):

```c
/* probatio_herbarium.c - herbarium: captura, variantes, indices
 * admissi, secreta petitionis, defectus capturae, redditio
 * (vates-plan-2 T2; herbarium-spec par. VI). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "http.h"
#include "json.h"
#include "filum.h"
#include "herbarium.h"
#include "iter_directoria.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/* ---- vectura scripta: responsa ordine reddit ---- */

nomen structura {
             i32  status;
    constans character* corpus;
    constans character* caput_titulus;   /* NIHIL = sine capite */
    constans character* caput_valor;
} ResponsumScriptum;

nomen structura {
    constans ResponsumScriptum* responsa;
                         i32  numerus;
                         i32  index;
} Scriptor;

interior HttpResultus
_scripta_exsequi (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
    Scriptor*      s = (Scriptor*)datum;
    HttpResultus   res;
    HttpResponsum* resp;
    constans ResponsumScriptum* r;

    (vacuum)petitio;
    memset(&res, 0, magnitudo(res));
    si (s->index >= s->numerus)
    {
        res.error = HTTP_ERROR_CONNEXIO;
        redde res;
    }
    r = &s->responsa[s->index];
    s->index++;
    resp = (HttpResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status = r->status;
    resp->corpus = chorda_ex_literis(r->corpus, piscina);
    resp->capita = (HttpCaput*)piscina_allocare(piscina,
        (i64)(III * magnitudo(HttpCaput)));
    resp->capita[0].titulus = chorda_ex_literis("Content-Type", piscina);
    resp->capita[0].valor = chorda_ex_literis("application/json", piscina);
    resp->capita[I].titulus = chorda_ex_literis("Set-Cookie", piscina);
    resp->capita[I].valor = chorda_ex_literis("sessio=SECRETUM_COOKIE", piscina);
    resp->capita_numerus = II;
    si (r->caput_titulus)
    {
        resp->capita[II].titulus = chorda_ex_literis(r->caput_titulus, piscina);
        resp->capita[II].valor = chorda_ex_literis(r->caput_valor, piscina);
        resp->capita_numerus = III;
    }
    res.successus = VERUM;
    res.responsum = resp;
    redde res;
}

interior HttpVectura
_scripta (Scriptor* s)
{
    HttpVectura v;

    v.exsequi = _scripta_exsequi;
    v.datum   = s;
    redde v;
}

interior HttpPetitio*
_petitio (Piscina* piscina)
{
    HttpPetitio* p = http_petitio_creare(piscina, HTTP_POST,
        "https://api.example.com/v1/messages");

    http_petitio_caput_addere(p, "x-api-key", "SECRETUM_CLAVIS_PROBATIONIS");
    http_petitio_corpus_ponere_chorda(p, chorda_ex_literis(
        "{\"model\":\"exemplar-x\",\"messages\":\"CORPUS_SECRETUM\"}", piscina));
    redde p;
}

/* numerus filorum in <dir>/specimina et linearum in <dir>/index.jsonl */
interior i32
_specimina_numerare (constans character* dir, Piscina* piscina)
{
    character             via[DXII];
    DirectoriumIterator*  it;
    DirectoriumIntroitus* introitus;
    i32                   numerus = 0;

    sprintf(via, "%s/specimina", dir);
    it = directorium_iterator_aperire(via, piscina);
    si (!it)
    {
        redde 0;
    }
    dum ((introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        si (introitus->genus == INTROITUS_FILUM)
        {
            numerus++;
        }
    }
    directorium_iterator_claudere(it);
    redde numerus;
}

interior i32
_lineae_indicis (constans character* dir, Piscina* piscina)
{
    character via[DXII];
    chorda    textus;
    i32       i;
    i32       lineae = 0;

    sprintf(via, "%s/index.jsonl", dir);
    textus = filum_legere_totum(via, piscina);
    per (i = 0; i < textus.mensura; i++)
    {
        si (textus.datum[i] == '\n')
        {
            lineae++;
        }
    }
    redde lineae;
}

/* omnia fila directorii concatenata (pro inquisitione secretorum) */
interior chorda
_omnia_legere (constans character* dir, Piscina* piscina)
{
    character             via[DXII];
    character             plena[DXII];
    DirectoriumIterator*  it;
    DirectoriumIntroitus* introitus;
    chorda                summa = chorda_ex_literis("", piscina);

    sprintf(via, "%s/specimina", dir);
    it = directorium_iterator_aperire(via, piscina);
    dum (it && (introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        chorda pars;
        chorda iuncta;

        sprintf(plena, "%s/%.*s", via, (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        pars = filum_legere_totum(plena, piscina);
        iuncta.mensura = summa.mensura + pars.mensura;
        iuncta.datum = (i8*)piscina_allocare(piscina, (i64)iuncta.mensura + I);
        memcpy(iuncta.datum, summa.datum, (size_t)summa.mensura);
        memcpy(iuncta.datum + summa.mensura, pars.datum, (size_t)pars.mensura);
        summa = iuncta;
    }
    si (it)
    {
        directorium_iterator_claudere(it);
    }
    redde summa;
}

interior vacuum
_purgare (constans character* dir, Piscina* piscina)
{
    character             via[DXII];
    character             plena[DXII];
    DirectoriumIterator*  it;
    DirectoriumIntroitus* introitus;

    sprintf(via, "%s/specimina", dir);
    it = directorium_iterator_aperire(via, piscina);
    dum (it && (introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        sprintf(plena, "%s/%.*s", via, (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        (vacuum)unlink(plena);
    }
    si (it)
    {
        directorium_iterator_claudere(it);
    }
    (vacuum)rmdir(via);
    sprintf(via, "%s/index.jsonl", dir);
    (vacuum)unlink(via);
    (vacuum)rmdir(dir);
}

interior vacuum
_directorium (character* fructus, constans character* titulus)
{
    sprintf(fructus, "/tmp/probatio_herbarium_%s_%ld", titulus,
            (longus)getpid());
}

/* ---- probationes ---- */

interior vacuum
probatio_sceleton(Piscina* piscina)
{
    HttpResponsum a;
    HttpResponsum b;
    HttpResponsum c;
    HttpResponsum d;

    imprimere("\n--- Probans herbarium_clavis_sceleti ---\n");
    memset(&a, 0, magnitudo(a));
    memset(&b, 0, magnitudo(b));
    memset(&c, 0, magnitudo(c));
    memset(&d, 0, magnitudo(d));
    a.status = CDXXIX;
    a.corpus = chorda_ex_literis(
        "{\"type\":\"error\",\"error\":{\"type\":\"x\",\"message\":\"m1\"}}", piscina);
    b = a;
    b.corpus = chorda_ex_literis(
        "{\"type\":\"error\",\"error\":{\"type\":\"y\",\"message\":\"aliud\"}}", piscina);
    c = a;
    c.corpus = chorda_ex_literis(
        "{\"type\":\"error\",\"error\":{\"type\":\"x\",\"message\":\"m\"},\"novum\":1}", piscina);
    d.status = DII;
    d.corpus = chorda_ex_literis("<html>bad gateway</html>", piscina);

    /* valores diversi, forma eadem -> clavis eadem */
    CREDO_CHORDA_AEQUALIS(herbarium_clavis_sceleti(NIHIL, &a, piscina, NIHIL),
                          herbarium_clavis_sceleti(NIHIL, &b, piscina, NIHIL));
    /* campus novus -> clavis alia */
    CREDO_FALSUM(chorda_aequalis(herbarium_clavis_sceleti(NIHIL, &a, piscina, NIHIL),
                                 herbarium_clavis_sceleti(NIHIL, &c, piscina, NIHIL)));
    /* non JSON: status + classis longitudinis */
    CREDO_CHORDA_INCIPIT(herbarium_clavis_sceleti(NIHIL, &d, piscina, NIHIL),
                         chorda_ex_literis("502:crudum:", piscina));
}

interior vacuum
probatio_captura(Piscina* piscina)
{
    character dir[CCLVI];
    ResponsumScriptum responsa[VI];
    Scriptor  s;
    HerbariumOptiones o;
    Herbarium* h;
    HttpVectura v;
    HttpResultus r;
    chorda omnia;
    constans character* constans campi[II] = { "model", NIHIL };
    i32 i;

    imprimere("\n--- Probans herbarium captura ---\n");
    _directorium(dir, "captura");
    _purgare(dir, piscina);

    /* 0,1: idem genus, numeri soli diversi -> una variantis
     * 2: verba alia -> variantis II
     * 3: verba tertia, sed variantes_maximae II -> non servatur
     * 4: status CC sub limine -> nihil
     * 5: genus aliud (529) -> specimen novum */
    responsa[0].status = CDXXIX;
    responsa[0].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"limit 50 reached\"}}";
    responsa[0].caput_titulus = "retry-after";
    responsa[0].caput_valor = "2";
    responsa[I] = responsa[0];
    responsa[I].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"limit 99 reached\"}}";
    responsa[II] = responsa[0];
    responsa[II].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"slow down please\"}}";
    responsa[III] = responsa[0];
    responsa[III].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"tertia verba\"}}";
    responsa[IV].status = CC;
    responsa[IV].corpus = "{\"ok\":true}";
    responsa[IV].caput_titulus = NIHIL;
    responsa[IV].caput_valor = NIHIL;
    responsa[V].status = DXXIX;
    responsa[V].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"}}";
    responsa[V].caput_titulus = NIHIL;
    responsa[V].caput_valor = NIHIL;
    s.responsa = responsa;
    s.numerus = VI;
    s.index = 0;

    o = herbarium_optiones_ordinariae();
    o.directorium = dir;
    o.variantes_maximae = II;
    o.campi_petitionis = campi;
    h = herbarium_aperire(piscina, &o);
    CREDO_NON_NIHIL(h);
    v = herbarium_vectura(h, _scripta(&s));

    per (i = 0; i < VI; i++)
    {
        r = http_vectura_exsequi(v, _petitio(piscina), piscina);
        /* responsum TRANSMITTITUR immutatum */
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32(r.responsum->status, responsa[i].status);
        CREDO_CHORDA_AEQUALIS_LITERIS(r.responsum->corpus, responsa[i].corpus);
    }

    /* 429: variantes II; 529: I; 200: nulla -> III fila */
    CREDO_AEQUALIS_I32(_specimina_numerare(dir, piscina), III);
    /* visiones: V (omnes supra limen, etiam non servatae) */
    CREDO_AEQUALIS_I32(_lineae_indicis(dir, piscina), V);

    omnia = _omnia_legere(dir, piscina);
    /* indices admissi: content-type et retry-after, NON set-cookie */
    CREDO_CHORDA_CONTINET(omnia, chorda_ex_literis("retry-after", piscina));
    CREDO_FALSUM(chorda_continet(omnia, chorda_ex_literis("SECRETUM_COOKIE", piscina)));
    /* capita et corpus petitionis numquam */
    CREDO_FALSUM(chorda_continet(omnia, chorda_ex_literis("SECRETUM_CLAVIS", piscina)));
    CREDO_FALSUM(chorda_continet(omnia, chorda_ex_literis("CORPUS_SECRETUM", piscina)));
    /* summarium: campus 'model' petitionis */
    CREDO_CHORDA_CONTINET(omnia, chorda_ex_literis("exemplar-x", piscina));

    _purgare(dir, piscina);
}

interior vacuum
probatio_premere_explicite(Piscina* piscina)
{
    character dir[CCLVI];
    HerbariumOptiones o;
    Herbarium* h;
    HttpResponsum resp;
    Xar* s;
    HerbariumSpecimen* sp;

    imprimere("\n--- Probans herbarium_premere (novitas) ---\n");
    _directorium(dir, "premere");
    _purgare(dir, piscina);
    o = herbarium_optiones_ordinariae();
    o.directorium = dir;
    h = herbarium_aperire(piscina, &o);
    memset(&resp, 0, magnitudo(resp));
    resp.status = CC;
    resp.corpus = chorda_ex_literis("{\"content\":[{\"type\":\"novum_genus\"}]}", piscina);
    herbarium_premere(h, _petitio(piscina), &resp,
                      chorda_ex_literis("blocus ignotus: novum_genus", piscina));
    s = herbarium_enumerare(piscina, dir);
    CREDO_AEQUALIS_I32(xar_numerus(s), I);
    sp = (HerbariumSpecimen*)xar_obtinere(s, 0);
    CREDO_CHORDA_AEQUALIS_LITERIS(sp->causa, "blocus ignotus: novum_genus");
    CREDO_AEQUALIS_I32(sp->status, CC);
    /* herbarium NIHIL: nihil fit, nulla ruina */
    herbarium_premere(NIHIL, _petitio(piscina), &resp, sp->causa);
    _purgare(dir, piscina);
}

interior vacuum
probatio_captura_defectus(Piscina* piscina)
{
    character dir[CCLVI];
    character spec[CCLVI];
    ResponsumScriptum responsa[I];
    Scriptor  s;
    HerbariumOptiones o;
    Herbarium* h;
    HttpResultus r;

    imprimere("\n--- Probans defectus capturae (responsum intactum) ---\n");
    /* aperire in loco impossibili -> NIHIL; vectura NIHIL transmittit */
    o = herbarium_optiones_ordinariae();
    o.directorium = "/dev/null/herbarium";
    CREDO_NIHIL(herbarium_aperire(piscina, &o));

    _directorium(dir, "defectus");
    _purgare(dir, piscina);
    responsa[0].status = D;
    responsa[0].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"api_error\",\"message\":\"x\"}}";
    responsa[0].caput_titulus = NIHIL;
    responsa[0].caput_valor = NIHIL;
    s.responsa = responsa;
    s.numerus = I;
    s.index = 0;
    o.directorium = dir;
    h = herbarium_aperire(piscina, &o);
    CREDO_NON_NIHIL(h);
    /* directorium post aperitionem non scribendum */
    sprintf(spec, "%s/specimina", dir);
    (vacuum)chmod(spec, 0500);
    (vacuum)chmod(dir, 0500);
    r = http_vectura_exsequi(herbarium_vectura(h, _scripta(&s)),
                             _petitio(piscina), piscina);
    CREDO_VERUM(r.successus);
    CREDO_AEQUALIS_I32(r.responsum->status, D);
    CREDO_CHORDA_CONTINET(r.responsum->corpus, chorda_ex_literis("api_error", piscina));
    (vacuum)chmod(dir, 0755);
    (vacuum)chmod(spec, 0755);

    /* herbarium NIHIL in vectura: interior ipsa redditur */
    s.index = 0;
    r = http_vectura_exsequi(herbarium_vectura(NIHIL, _scripta(&s)),
                             _petitio(piscina), piscina);
    CREDO_VERUM(r.successus);
    _purgare(dir, piscina);
}

interior vacuum
probatio_redditio(Piscina* piscina)
{
    character dir[CCLVI];
    ResponsumScriptum responsa[II];
    Scriptor  s;
    HerbariumOptiones o;
    Herbarium* h;
    HttpVectura reddens;
    HttpResultus r;
    Xar* specimina;
    i32 i;

    imprimere("\n--- Probans herbarium_enumerare + herbarium_reddens ---\n");
    _directorium(dir, "redditio");
    _purgare(dir, piscina);
    responsa[0].status = CDXXIX;
    responsa[0].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"a\"}}";
    responsa[0].caput_titulus = "retry-after";
    responsa[0].caput_valor = "7";
    responsa[I].status = DXXIX;
    responsa[I].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"}}";
    responsa[I].caput_titulus = NIHIL;
    responsa[I].caput_valor = NIHIL;
    s.responsa = responsa;
    s.numerus = II;
    s.index = 0;
    o = herbarium_optiones_ordinariae();
    o.directorium = dir;
    h = herbarium_aperire(piscina, &o);
    per (i = 0; i < II; i++)
    {
        (vacuum)http_vectura_exsequi(herbarium_vectura(h, _scripta(&s)),
                                     _petitio(piscina), piscina);
    }

    specimina = herbarium_enumerare(piscina, dir);
    CREDO_AEQUALIS_I32(xar_numerus(specimina), II);
    reddens = herbarium_reddens(piscina, specimina);
    per (i = 0; i < II; i++)
    {
        HerbariumSpecimen* sp = (HerbariumSpecimen*)xar_obtinere(specimina, i);

        r = http_vectura_exsequi(reddens, _petitio(piscina), piscina);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32(r.responsum->status, sp->status);
        CREDO_CHORDAE_AEQUALES(r.responsum->corpus, sp->corpus);
    }
    /* retry-after servatum et redditum */
    {
        b32 inventum = FALSUM;

        s.index = 0;
        reddens = herbarium_reddens(piscina, specimina);
        per (i = 0; i < II; i++)
        {
            r = http_vectura_exsequi(reddens, _petitio(piscina), piscina);
            si (http_responsum_caput(r.responsum, "retry-after").mensura > 0)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS(
                    http_responsum_caput(r.responsum, "retry-after"), "7");
                inventum = VERUM;
            }
        }
        CREDO_VERUM(inventum);
    }
    /* exhaustum -> error nominatus */
    r = http_vectura_exsequi(reddens, _petitio(piscina), piscina);
    CREDO_FALSUM(r.successus);
    CREDO_VERUM(r.error == HTTP_ERROR_CONNEXIO);
    /* directorium absens -> Xar vacuum */
    CREDO_AEQUALIS_I32(xar_numerus(herbarium_enumerare(piscina,
        "/tmp/probatio_herbarium_numquam_creatum")), 0);
    _purgare(dir, piscina);
}

s32
principale (vacuum)
{
    Piscina* piscina;
         b32 successus;

    piscina = piscina_generare_dynamicum("probatio_herbarium", M * M);
    credo_aperire(piscina);

    probatio_sceleton(piscina);
    probatio_captura(piscina);
    probatio_premere_explicite(piscina);
    probatio_captura_defectus(piscina);
    probatio_redditio(piscina);

    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? 0 : I;
}
```

- [ ] **Step 2.2: run, expect failure** - `./compile_tests.sh probatio_herbarium`
  Expected: link failure (undefined `herbarium_*`) or the generated
  source list not yet containing `lib/herbarium.c`.

- [ ] **Step 2.3: implement `lib/herbarium.c`** (`./silva/scribe.sh`):

```c
/* herbarium.c - specimina pressa responsorum API: captura et redditio
 * (herbarium-spec; vates-plan-2 T2). Decisiones intra caput: vide
 * lib/herbarium.worklog.md. */
#include "postulata_posix.h"
#include "herbarium.h"
#include "json.h"
#include "filum.h"
#include "fasti.h"
#include "sigillum.h"
#include "iter_directoria.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HERBARIUM_VIA_MAXIMA  MXXIV

structura Herbarium {
               Piscina* piscina;
     HerbariumOptiones  optiones;
             character  specimina_dir[HERBARIUM_VIA_MAXIMA];
             character  index_via[HERBARIUM_VIA_MAXIMA];
                   b32  defectus_nuntiatus;
};

nomen structura {
     Herbarium* herbarium;
    HttpVectura involuta;
} HerbariumInvolucrum;

nomen structura {
    Xar* specimina;
    i32  index;
} HerbariumRedditor;

hic_manens constans character* _capita_ordinaria[] = {
    "content-type", "retry-after", "request-id", "x-request-id", NIHIL
};


/* ======================================================================
 * AUXILIA
 * ====================================================================== */

interior vacuum
_defectum_nuntiare (
              Herbarium* h,
    constans character* nuntius)
{
    si (!h->defectus_nuntiatus)
    {
        fprintf(stderr, "herbarium: %s (captura pergit sine nuntiis "
                        "ulterioribus; petitiones intactae)\n", nuntius);
        h->defectus_nuntiatus = VERUM;
    }
}

interior b32
_est_cifra (i8 c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_vocis (i8 c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || _est_cifra(c) || c == '_' || c == '-';
}

/* Larvare: series [A-Za-z0-9_-] >= XII cum cifra -> '@'; ceterae
 * cifrarum series -> '#'. Variantes per textum larvatum different. */
interior chorda
_larvare (
      chorda s,
    Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)s.mensura + I);
    i32 i = 0;

    dum (i < s.mensura)
    {
        si (_est_vocis(s.datum[i]))
        {
            i32 initium = i;
            b32 cifra = FALSUM;

            dum (i < s.mensura && _est_vocis(s.datum[i]))
            {
                si (_est_cifra(s.datum[i]))
                {
                    cifra = VERUM;
                }
                i++;
            }
            si (cifra && i - initium >= XII)
            {
                chorda_aedificator_appendere_character(aed, '@');
            }
            alioquin
            {
                i32 k;
                b32 in_cifris = FALSUM;

                per (k = initium; k < i; k++)
                {
                    si (_est_cifra(s.datum[k]))
                    {
                        si (!in_cifris)
                        {
                            chorda_aedificator_appendere_character(aed, '#');
                        }
                        in_cifris = VERUM;
                    }
                    alioquin
                    {
                        chorda_aedificator_appendere_character(aed,
                            (character)s.datum[k]);
                        in_cifris = FALSUM;
                    }
                }
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed, (character)s.datum[i]);
            i++;
        }
    }
    redde chorda_aedificator_finire(aed);
}

interior vacuum
_sceleton (
    ChordaAedificator* aed,
            JsonValor* v)
{
    commutatio (json_genus(v))
    {
        casus JSON_OBJECTUM:
        {
            JsonObjectumIterator it = json_objectum_iterator(v);
            chorda     clavis;
            JsonValor* filius;
            b32        primus = VERUM;

            chorda_aedificator_appendere_character(aed, '{');
            dum (json_objectum_iterator_proxima(&it, &clavis, &filius))
            {
                si (!primus)
                {
                    chorda_aedificator_appendere_character(aed, ',');
                }
                primus = FALSUM;
                chorda_aedificator_appendere_chorda(aed, clavis);
                chorda_aedificator_appendere_character(aed, ':');
                _sceleton(aed, filius);
            }
            chorda_aedificator_appendere_character(aed, '}');
            frange;
        }
        casus JSON_TABULATUM:
            chorda_aedificator_appendere_character(aed, '[');
            si (json_tabulatum_numerus(v) > 0)
            {
                _sceleton(aed, json_tabulatum_obtinere(v, 0));
            }
            chorda_aedificator_appendere_character(aed, ']');
            frange;
        casus JSON_CHORDA:
            chorda_aedificator_appendere_character(aed, 's');
            frange;
        casus JSON_INTEGER:
        casus JSON_FLUITANS:
            chorda_aedificator_appendere_character(aed, 'n');
            frange;
        casus JSON_BOOLEAN:
            chorda_aedificator_appendere_character(aed, 'b');
            frange;
        ordinarius:
            chorda_aedificator_appendere_character(aed, 'z');
            frange;
    }
}

interior b32
_caput_admissum (
    constans Herbarium* h,
               chorda  titulus,
             Piscina*  piscina)
{
    chorda minuscula = chorda_minuscula(titulus, piscina);
    i32    i;

    per (i = 0; _capita_ordinaria[i]; i++)
    {
        si (chorda_aequalis_literis(minuscula, _capita_ordinaria[i]))
        {
            redde VERUM;
        }
    }
    si (h->optiones.capita_admissa)
    {
        per (i = 0; h->optiones.capita_admissa[i]; i++)
        {
            si (chorda_aequalis_literis(minuscula,
                    h->optiones.capita_admissa[i]))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

interior JsonValor*
_specimen_json (
             Herbarium* h,
           HttpPetitio* petitio,
         HttpResponsum* resp,
                chorda  clavis,
                chorda  causa,
              Piscina*  piscina)
{
    JsonValor*       obj    = json_objectum_creare(piscina);
    JsonValor*       capita = json_tabulatum_creare(piscina);
    JsonValor*       summ   = json_objectum_creare(piscina);
    HttpPetitioVisus visus  = http_petitio_visus(petitio);
    i32              i;

    json_objectum_ponere(obj, "clavis", json_chorda_creare(piscina, clavis));
    json_objectum_ponere(obj, "causa", json_chorda_creare(piscina, causa));
    json_objectum_ponere(obj, "status",
        json_integer_creare(piscina, (s64)resp->status));
    per (i = 0; i < resp->capita_numerus; i++)
    {
        si (_caput_admissum(h, resp->capita[i].titulus, piscina))
        {
            JsonValor* c = json_objectum_creare(piscina);

            json_objectum_ponere(c, "titulus",
                json_chorda_creare(piscina, resp->capita[i].titulus));
            json_objectum_ponere(c, "valor",
                json_chorda_creare(piscina, resp->capita[i].valor));
            json_tabulatum_addere(capita, c);
        }
    }
    json_objectum_ponere(obj, "capita", capita);
    json_objectum_ponere(obj, "corpus", json_chorda_creare(piscina, resp->corpus));
    json_objectum_ponere(obj, "primum_visum",
        json_chorda_creare(piscina, fasti_ad_iso(fasti_nunc(), piscina)));
    json_objectum_ponere(obj, "methodus", json_chorda_creare_literis(piscina,
        petitio ? http_methodus_nomen(visus.methodus) : ""));
    json_objectum_ponere(obj, "hospes", json_chorda_creare(piscina, visus.hospes));
    json_objectum_ponere(obj, "via", json_chorda_creare(piscina, visus.via));
    si (h->optiones.campi_petitionis && visus.corpus.mensura > 0)
    {
        JsonResultus j = json_legere(visus.corpus, piscina);

        si (j.successus && json_est_objectum(j.radix))
        {
            per (i = 0; h->optiones.campi_petitionis[i]; i++)
            {
                JsonValor* v = json_objectum_capere(j.radix,
                    h->optiones.campi_petitionis[i]);

                si (v)
                {
                    json_objectum_ponere(summ, h->optiones.campi_petitionis[i], v);
                }
            }
        }
    }
    json_objectum_ponere(obj, "summarium", summ);
    redde obj;
}

interior vacuum
_premere (
       Herbarium* h,
     HttpPetitio* petitio,
    HttpResponsum* resp,
           chorda  causa)
{
    Piscina*  p = piscina_generare_dynamicum("herbarium_premere", LXIV * M);
    chorda    clavis;
    chorda    larvatum;
    Sigillum  sig;
    character hex[SIGILLUM_HEX_MENSURA];
    character via[HERBARIUM_VIA_MAXIMA + C];
    s32       novus = -I;
    i32       n;

    si (!p)
    {
        _defectum_nuntiare(h, "piscina creari non potuit");
        redde;
    }
    clavis = h->optiones.clavis
        ? h->optiones.clavis(petitio, resp, p, h->optiones.clavis_datum)
        : herbarium_clavis_sceleti(petitio, resp, p, NIHIL);
    sig = sigillum_computare(clavis.datum, (memoriae_index)clavis.mensura);
    sigillum_hex(&sig, hex);
    larvatum = _larvare(resp->corpus, p);

    per (n = I; n <= h->optiones.variantes_maximae; n++)
    {
        sprintf(via, "%s/%s-%u.json", h->specimina_dir, hex, n);
        si (!filum_existit(via))
        {
            novus = (s32)n;
            frange;
        }
        {
            JsonResultus j = json_legere(filum_legere_totum(via, p), p);
            chorda       prius = j.successus
                ? json_capere_chorda(j.radix, "corpus", chorda_ex_literis("", p))
                : chorda_ex_literis("", p);

            si (chorda_aequalis(_larvare(prius, p), larvatum))
            {
                novus = 0;
                frange;
            }
        }
    }
    si (novus > 0)
    {
        JsonValor* spec = _specimen_json(h, petitio, resp, clavis, causa, p);

        si (!filum_scribere(via, json_scribere_pulchrum(spec, p)))
        {
            _defectum_nuntiare(h, "specimen scribi non potuit");
        }
    }
    {
        JsonValor* linea = json_objectum_creare(p);
        chorda     textus;
        chorda     cum_fine;

        json_objectum_ponere(linea, "tempus",
            json_chorda_creare(p, fasti_ad_iso(fasti_nunc(), p)));
        json_objectum_ponere(linea, "sigillum", json_chorda_creare_literis(p, hex));
        json_objectum_ponere(linea, "status", json_integer_creare(p, (s64)resp->status));
        json_objectum_ponere(linea, "causa", json_chorda_creare(p, causa));
        json_objectum_ponere(linea, "variantes_index",
            json_integer_creare(p, novus > 0 ? (s64)novus : 0));
        textus = json_scribere(linea, p);
        cum_fine.mensura = textus.mensura + I;
        cum_fine.datum = (i8*)piscina_allocare(p, (i64)cum_fine.mensura);
        memcpy(cum_fine.datum, textus.datum, (size_t)textus.mensura);
        cum_fine.datum[textus.mensura] = '\n';
        si (!filum_appendere_firmiter(h->index_via, cum_fine))
        {
            _defectum_nuntiare(h, "index.jsonl scribi non potuit");
        }
    }
    piscina_destruere(p);
}


/* ======================================================================
 * CAPTURA
 * ====================================================================== */

HerbariumOptiones
herbarium_optiones_ordinariae (vacuum)
{
    HerbariumOptiones o;

    memset(&o, 0, magnitudo(o));
    redde o;
}

Herbarium*
herbarium_aperire (
                       Piscina* piscina,
    constans HerbariumOptiones* optiones)
{
    Herbarium* h;

    si (!piscina || !optiones || !optiones->directorium
        || strlen(optiones->directorium) + XXXII >= HERBARIUM_VIA_MAXIMA)
    {
        redde NIHIL;
    }
    h = (Herbarium*)piscina_allocare(piscina, (i64)magnitudo(Herbarium));
    si (!h)
    {
        redde NIHIL;
    }
    memset(h, 0, magnitudo(*h));
    h->piscina  = piscina;
    h->optiones = *optiones;
    si (h->optiones.status_minimus == 0)
    {
        h->optiones.status_minimus = CD;
    }
    si (h->optiones.variantes_maximae == 0)
    {
        h->optiones.variantes_maximae = III;
    }
    sprintf(h->specimina_dir, "%s/specimina", optiones->directorium);
    sprintf(h->index_via, "%s/index.jsonl", optiones->directorium);
    si (!filum_directorium_creare_cum_parentibus(h->specimina_dir))
    {
        fprintf(stderr, "herbarium: directorium creari non potest: %s"
                        " (sine captura pergitur)\n", h->specimina_dir);
        redde NIHIL;
    }
    redde h;
}

interior HttpResultus
_exsequi_capiens (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
    HerbariumInvolucrum* inv = (HerbariumInvolucrum*)datum;
    HttpResultus         res;

    res = http_vectura_exsequi(inv->involuta, petitio, piscina);
    si (res.successus && res.responsum
        && res.responsum->status >= inv->herbarium->optiones.status_minimus)
    {
        _premere(inv->herbarium, petitio, res.responsum,
                 chorda_ex_literis("status", piscina));
    }
    redde res;
}

HttpVectura
herbarium_vectura (
      Herbarium* herbarium,
    HttpVectura  involuta)
{
    HerbariumInvolucrum* inv;
    HttpVectura          v;

    si (!herbarium)
    {
        redde involuta;
    }
    inv = (HerbariumInvolucrum*)piscina_allocare(herbarium->piscina,
        (i64)magnitudo(HerbariumInvolucrum));
    inv->herbarium = herbarium;
    inv->involuta  = involuta;
    v.exsequi = _exsequi_capiens;
    v.datum   = inv;
    redde v;
}

vacuum
herbarium_premere (
        Herbarium* herbarium,
      HttpPetitio* petitio,
    HttpResponsum* responsum,
           chorda  causa)
{
    si (!herbarium || !responsum)
    {
        redde;
    }
    _premere(herbarium, petitio, responsum, causa);
}

chorda
herbarium_clavis_sceleti (
      HttpPetitio* petitio,
    HttpResponsum* responsum,
          Piscina* piscina,
           vacuum* datum)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina, CCLVI);
    character          numerus[XXXII];
    JsonResultus       j;

    (vacuum)petitio;
    (vacuum)datum;
    sprintf(numerus, "%u:", responsum ? responsum->status : 0);
    chorda_aedificator_appendere_literis(aed, numerus);
    si (!responsum)
    {
        redde chorda_aedificator_finire(aed);
    }
    j = json_legere(responsum->corpus, piscina);
    si (j.successus)
    {
        _sceleton(aed, j.radix);
    }
    alioquin
    {
        i32 m = responsum->corpus.mensura;

        chorda_aedificator_appendere_literis(aed, "crudum:");
        chorda_aedificator_appendere_chorda(aed,
            http_responsum_caput(responsum, "content-type"));
        chorda_aedificator_appendere_literis(aed,
            m == 0 ? ":0" : m < M ? ":<1K" : m < LXIV * M ? ":<64K" : ":>=64K");
    }
    redde chorda_aedificator_finire(aed);
}


/* ======================================================================
 * REDDITIO
 * ====================================================================== */

interior b32
_specimen_legere (
               chorda  nomen_fili,
               chorda  textus,
    HerbariumSpecimen* s,
              Piscina* piscina)
{
    JsonResultus j = json_legere(textus, piscina);
    JsonValor*   capita;
    chorda       vacua = chorda_ex_literis("", piscina);
    s32          linea = -I;
    i32          i;

    si (!j.successus || !json_est_objectum(j.radix))
    {
        redde FALSUM;
    }
    memset(s, 0, magnitudo(*s));
    s->clavis       = json_capere_chorda(j.radix, "clavis", vacua);
    s->causa        = json_capere_chorda(j.radix, "causa", vacua);
    s->status       = (i32)json_capere_integer(j.radix, "status", 0);
    s->corpus       = json_capere_chorda(j.radix, "corpus", vacua);
    s->primum_visum = json_capere_chorda(j.radix, "primum_visum", vacua);
    s->methodus     = json_capere_chorda(j.radix, "methodus", vacua);
    s->hospes       = json_capere_chorda(j.radix, "hospes", vacua);
    s->via          = json_capere_chorda(j.radix, "via", vacua);
    s->summarium    = json_objectum_capere(j.radix, "summarium")
        ? json_scribere(json_objectum_capere(j.radix, "summarium"), piscina)
        : vacua;
    capita = json_objectum_capere(j.radix, "capita");
    s->capita_numerus = capita ? json_tabulatum_numerus(capita) : 0;
    s->capita = (HttpCaput*)piscina_allocare(piscina,
        (i64)((s->capita_numerus + I) * magnitudo(HttpCaput)));
    per (i = 0; i < s->capita_numerus; i++)
    {
        JsonValor* c = json_tabulatum_obtinere(capita, i);

        s->capita[i].titulus = json_capere_chorda(c, "titulus", vacua);
        s->capita[i].valor   = json_capere_chorda(c, "valor", vacua);
    }
    /* nomen: <sigillum>-<n>.json */
    per (i = 0; i < nomen_fili.mensura; i++)
    {
        si (nomen_fili.datum[i] == '-')
        {
            linea = (s32)i;
        }
    }
    s->sigillum.datum   = nomen_fili.datum;
    s->sigillum.mensura = linea > 0 ? (i32)linea : nomen_fili.mensura;
    s->variantes_index  = 0;
    si (linea > 0)
    {
        per (i = (i32)linea + I; i < nomen_fili.mensura
             && _est_cifra(nomen_fili.datum[i]); i++)
        {
            s->variantes_index = s->variantes_index * X
                + (i32)(nomen_fili.datum[i] - '0');
        }
    }
    redde VERUM;
}

interior integer
_specimina_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans HerbariumSpecimen* x = *(constans HerbariumSpecimen* constans*)a;
    constans HerbariumSpecimen* y = *(constans HerbariumSpecimen* constans*)b;
    i32     brevior = x->sigillum.mensura < y->sigillum.mensura
                    ? x->sigillum.mensura : y->sigillum.mensura;
    integer c = memcmp(x->sigillum.datum, y->sigillum.datum, (size_t)brevior);

    si (c != 0)
    {
        redde c;
    }
    si (x->sigillum.mensura != y->sigillum.mensura)
    {
        redde x->sigillum.mensura < y->sigillum.mensura ? -I : I;
    }
    si (x->variantes_index != y->variantes_index)
    {
        redde x->variantes_index < y->variantes_index ? -I : I;
    }
    redde 0;
}

Xar*
herbarium_enumerare (
               Piscina* piscina,
    constans character* directorium)
{
    Xar*                  fructus = xar_creare(piscina,
                                        (i32)magnitudo(HerbariumSpecimen));
    Xar*                  lecta   = xar_creare(piscina,
                                        (i32)magnitudo(HerbariumSpecimen));
    character             dir[HERBARIUM_VIA_MAXIMA];
    character             via[HERBARIUM_VIA_MAXIMA + CCLVI];
    DirectoriumIterator*  it;
    DirectoriumIntroitus* introitus;
    HerbariumSpecimen**   ordo;
    i32                   numerus;
    i32                   i;

    si (!directorium || strlen(directorium) + XVI >= HERBARIUM_VIA_MAXIMA)
    {
        redde fructus;
    }
    sprintf(dir, "%s/specimina", directorium);
    it = directorium_iterator_aperire(dir, piscina);
    si (!it)
    {
        redde fructus;
    }
    dum ((introitus = directorium_iterator_proximum(it)) != NIHIL)
    {
        HerbariumSpecimen* s;
        chorda             titulus;

        si (introitus->genus != INTROITUS_FILUM
            || introitus->titulus.mensura <= V
            || memcmp(introitus->titulus.datum + introitus->titulus.mensura - V,
                      ".json", V) != 0)
        {
            perge;
        }
        sprintf(via, "%s/%.*s", dir, (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        titulus = chorda_ex_literis(via + strlen(dir) + I, piscina);
        s = (HerbariumSpecimen*)xar_addere(lecta);
        si (!_specimen_legere(titulus, filum_legere_totum(via, piscina),
                              s, piscina))
        {
            xar_truncare(lecta, xar_numerus(lecta) - I);
        }
    }
    directorium_iterator_claudere(it);

    numerus = xar_numerus(lecta);
    si (numerus == 0)
    {
        redde fructus;
    }
    ordo = (HerbariumSpecimen**)piscina_allocare(piscina,
        (i64)(numerus * magnitudo(HerbariumSpecimen*)));
    per (i = 0; i < numerus; i++)
    {
        ordo[i] = (HerbariumSpecimen*)xar_obtinere(lecta, i);
    }
    qsort(ordo, (size_t)numerus, magnitudo(HerbariumSpecimen*),
          _specimina_comparare);
    per (i = 0; i < numerus; i++)
    {
        *(HerbariumSpecimen*)xar_addere(fructus) = *ordo[i];
    }
    redde fructus;
}

interior HttpResultus
_exsequi_reddens (
    HttpPetitio* petitio,
        Piscina* piscina,
         vacuum* datum)
{
    HerbariumRedditor* r = (HerbariumRedditor*)datum;
    HttpResultus       res;
    HttpResponsum*     resp;
    HerbariumSpecimen* s;

    (vacuum)petitio;
    memset(&res, 0, magnitudo(res));
    si (r->index >= xar_numerus(r->specimina))
    {
        res.error = HTTP_ERROR_CONNEXIO;
        res.error_descriptio = chorda_ex_literis("herbarium exhaustum", piscina);
        redde res;
    }
    s = (HerbariumSpecimen*)xar_obtinere(r->specimina, r->index);
    r->index++;
    resp = (HttpResponsum*)piscina_allocare(piscina, (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status             = s->status;
    resp->status_descriptio  = chorda_ex_literis(http_status_descriptio(s->status),
                                                 piscina);
    resp->corpus             = s->corpus;
    resp->capita             = s->capita;
    resp->capita_numerus     = s->capita_numerus;
    res.successus = VERUM;
    res.responsum = resp;
    redde res;
}

HttpVectura
herbarium_reddens (
    Piscina* piscina,
        Xar* specimina)
{
    HerbariumRedditor* r = (HerbariumRedditor*)piscina_allocare(piscina,
        (i64)magnitudo(HerbariumRedditor));
    HttpVectura        v;

    r->specimina = specimina;
    r->index     = 0;
    v.exsequi = _exsequi_reddens;
    v.datum   = r;
    redde v;
}
```

  Exact signatures to confirm on first compile (rule on any mismatch):
  `chorda_minuscula(chorda, Piscina*)`, `chorda_aedificator_appendere_chorda(aed, chorda)`,
  `xar_truncare(xar, i32 numerus)`, `filum_directorium_creare_cum_parentibus`
  succeeding on an existing directory, `json_capere_integer` on an
  integer stored by `json_integer_creare`.

- [ ] **Step 2.4: register + green.** `./tools/compile_tests_fontes_generare.sh`
  then `./compile_tests.sh probatio_herbarium` -> PASS. Lint:
  `VOCABULA_VIAE_ADDITAE=lib/herbarium.c ./oratio/vocabula.sh -nova`
  (after `./silva/nexus.sh -renovare`), glossary entries as needed.
- [ ] **Step 2.5: plants** (each red, then restore + `cmp`):
  1. `_caput_admissum` returns `VERUM` always -> "SECRETUM_COOKIE" red.
  2. variant compare uses raw body (`prius` vs `resp->corpus`) instead
     of masked -> 429 count III instead of II -> red.
  3. `_exsequi_capiens` presses even below `status_minimus` -> 200
     pressed -> count red.
  4. `_premere` skips the index append -> sightings red.
  5. `_exsequi_reddens` returns `s->status + I` -> replay red.
- [ ] **Step 2.6: worklog + commit.** `lib/herbarium.worklog.md`: the
  design decisions listed above + plant results. Gates: `radix`,
  `generata` (new lib in the generated source list). `commissio_umbra`.

---

### Task 3: vates - neutral model and the Anthropic request body (pure)

**Files:** Create `lib/vates.c`, `probationes/probatio_vates.c`;
regenerate `compile_tests_fontes_generata.sh`.

**Interfaces:** Produces (header) `vates_petitio_creare`,
`_maxima_ponere`, `_conatum_ponere`, `_cogitationem_monstrare`,
`_caudam_signare`, `vates_systema_addere`, `vates_instrumentum_addere`,
`vates_nuntium_addere`, `vates_textum_addere`, `vates_effectum_addere`,
`vates_extra_ponere`, `vates_caput_addere`, `vates_responsum_addere`,
`vates_anthropic_corpus`. Internal for later tasks:
`interior s32 _signa_numerare (constans VatesPetitio*)`,
`interior VatesResponsum* _responsum_vacuum (Piscina*)`,
`structura VatesPetitio` fields below, `VatesEffugium`.

Render rules (decided here, recorded in the worklog):
- key order of the body is fixed: `model`, `max_tokens`, `thinking`
  (only when showing summaries: `{"type":"adaptive","display":"summarized"}`),
  `output_config` (only with effort), `cache_control` (tail), `system`,
  `tools`, `messages`, then tagged extras for `"anthropic"` (last).
- a block that carries `crudum` from provider `"anthropic"` is emitted
  VERBATIM (unknown fields survive: citations, future keys); with
  `signum_thesauri` a shallow copy + `cache_control` is emitted for
  TEXTUS / PETITUM / EFFECTUS; OPACUM never gets `cache_control`.
- a block whose `crudum` comes from another provider is rebuilt from
  its neutral fields if TEXTUS / PETITUM / EFFECTUS, omitted if OPACUM.
- a message left with no blocks is omitted (Review focus 5).

- [ ] **Step 3.1: failing tests** - create `probationes/probatio_vates.c`:

```c
/* probatio_vates.c - vates: corpus Anthropic (octeti exacti),
 * lectio responsorum, mittere (iterationes, rationarium, herbarium),
 * fictus, specimina commissa (vates-plan-2 T3-T6). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "http.h"
#include "filum.h"
#include "herbarium.h"
#include "vates.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

interior chorda
_c (constans character* s, Piscina* piscina)
{
    redde chorda_ex_literis(s, piscina);
}

interior vacuum
probatio_corpus_minimum(Piscina* piscina)
{
    VatesPetitio* p;
    VatesNuntius* n;

    imprimere("\n--- Probans corpus minimum (octeti exacti) ---\n");
    p = vates_petitio_creare(piscina, _c("claude-opus-5-5", piscina));
    vates_systema_addere(p, _c("Sis brevis.", piscina), VERUM);
    n = vates_nuntium_addere(p, VATES_USOR);
    vates_textum_addere(n, _c("Salve", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(vates_anthropic_corpus(p, piscina),
        "{\"model\":\"claude-opus-5-5\",\"max_tokens\":16000,"
        "\"system\":[{\"type\":\"text\",\"text\":\"Sis brevis.\","
        "\"cache_control\":{\"type\":\"ephemeral\"}}],"
        "\"messages\":[{\"role\":\"user\",\"content\":"
        "[{\"type\":\"text\",\"text\":\"Salve\"}]}]}");
}

interior vacuum
probatio_corpus_plenum(Piscina* piscina)
{
    VatesPetitio*     p;
    VatesNuntius*     n;
    VatesBlocus*      b;
    VatesInstrumentum t;
    chorda            primum;

    imprimere("\n--- Probans corpus plenum + determinismus + unicode ---\n");
    p = vates_petitio_creare(piscina, _c("claude-sonnet-5-5", piscina));
    vates_petitio_maxima_ponere(p, MM);
    vates_petitio_conatum_ponere(p, _c("high", piscina));
    vates_petitio_cogitationem_monstrare(p, VERUM);
    vates_petitio_caudam_signare(p, VERUM);
    t.titulus = _c("zoom", piscina);
    t.descriptio = _c("Aperit lineam.", piscina);
    t.schema = json_legere_literis("{\"type\":\"object\"}", piscina).radix;
    vates_instrumentum_addere(p, &t);
    n = vates_nuntium_addere(p, VATES_USOR);
    b = vates_textum_addere(n, _c("Ave, c\xc4\x81rissime \xe2\x9c\x9d \xf0\x9d\x94\x99", piscina));
    b->signum_thesauri = VERUM;
    n = vates_nuntium_addere(p, VATES_USOR);
    vates_effectum_addere(n, _c("toolu_1", piscina), _c("nihil", piscina), VERUM);
    vates_extra_ponere(p, "anthropic", "fallbacks",
                       json_chorda_creare_literis(piscina, "default"));
    vates_extra_ponere(p, "openai", "store",
                       json_boolean_creare(piscina, FALSUM));
    primum = vates_anthropic_corpus(p, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(primum,
        "{\"model\":\"claude-sonnet-5-5\",\"max_tokens\":2000,"
        "\"thinking\":{\"type\":\"adaptive\",\"display\":\"summarized\"},"
        "\"output_config\":{\"effort\":\"high\"},"
        "\"cache_control\":{\"type\":\"ephemeral\"},"
        "\"tools\":[{\"name\":\"zoom\",\"description\":\"Aperit lineam.\","
        "\"input_schema\":{\"type\":\"object\"}}],"
        "\"messages\":[{\"role\":\"user\",\"content\":[{\"type\":\"text\","
        "\"text\":\"Ave, c\xc4\x81rissime \xe2\x9c\x9d \xf0\x9d\x94\x99\","
        "\"cache_control\":{\"type\":\"ephemeral\"}}]},"
        "{\"role\":\"user\",\"content\":[{\"type\":\"tool_result\","
        "\"tool_use_id\":\"toolu_1\",\"content\":\"nihil\",\"is_error\":true}]}],"
        "\"fallbacks\":\"default\"}");
    /* idem corpus bis -> octeti idem */
    CREDO_CHORDAE_AEQUALES(primum, vates_anthropic_corpus(p, piscina));
}

interior vacuum
probatio_opaca(Piscina* piscina)
{
    VatesPetitio*  p;
    VatesNuntius*  n;
    VatesBlocus*   b;
    chorda         corpus;

    imprimere("\n--- Probans OPACUM: verbatim, alienum omissum ---\n");
    p = vates_petitio_creare(piscina, _c("claude-opus-5-5", piscina));
    n = vates_nuntium_addere(p, VATES_ASSISTENS);
    b = vates_textum_addere(n, _c("", piscina));
    b->genus = VATES_OPACUM;
    b->crudum = json_legere_literis(
        "{\"type\":\"thinking\",\"thinking\":\"\",\"signature\":\"sig123\"}",
        piscina).radix;
    b->provisor = _c("anthropic", piscina);
    b->signum_thesauri = VERUM;              /* in OPACO neglegitur */
    /* nuntius cuius soli bloci aliena OPACA sunt -> omittitur */
    n = vates_nuntium_addere(p, VATES_ASSISTENS);
    b = vates_textum_addere(n, _c("", piscina));
    b->genus = VATES_OPACUM;
    b->crudum = json_legere_literis("{\"type\":\"reasoning\",\"encrypted\":\"x\"}",
                                    piscina).radix;
    b->provisor = _c("openai", piscina);
    n = vates_nuntium_addere(p, VATES_USOR);
    vates_textum_addere(n, _c("Et nunc?", piscina));
    corpus = vates_anthropic_corpus(p, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(corpus,
        "{\"model\":\"claude-opus-5-5\",\"max_tokens\":16000,"
        "\"messages\":[{\"role\":\"assistant\",\"content\":[{\"type\":\"thinking\","
        "\"thinking\":\"\",\"signature\":\"sig123\"}]},"
        "{\"role\":\"user\",\"content\":[{\"type\":\"text\",\"text\":\"Et nunc?\"}]}]}");
}

s32
principale (vacuum)
{
    Piscina* piscina;
         b32 successus;

    piscina = piscina_generare_dynamicum("probatio_vates", M * M);
    credo_aperire(piscina);

    /* T3 */
    probatio_corpus_minimum(piscina);
    probatio_corpus_plenum(piscina);
    probatio_opaca(piscina);
    /* T4, T5, T6 addunt hic vocationes suas */

    credo_imprimere_compendium();
    successus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde successus ? 0 : I;
}
```

- [ ] **Step 3.2: run, expect failure** - `./compile_tests.sh probatio_vates`
  Expected: undefined `vates_*` (no `lib/vates.c` yet).

- [ ] **Step 3.3: implement part A of `lib/vates.c`** (`./silva/scribe.sh`):

```c
/* vates.c - provisor neuter exemplarium linguae (vates-spec;
 * vates-plan-2). Partes: A exemplar neutrum + corpus Anthropic (T3),
 * B lectio (T4), C provisores + mittere + rationarium + fictus (T5). */
#include "postulata_posix.h"
#include "vates.h"
#include "herbarium.h"
#include "filum.h"
#include "fasti.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define VATES_SIGNA_ORDINARIA        (XVI * M)   /* max_tokens */
#define VATES_PUNCTA_THESAURI_MAXIMA IV

nomen structura {
        chorda  provisor;
        chorda  clavis;      /* campus corporis aut titulus capitis */
    JsonValor*  valor;       /* extra */
        chorda  textus;      /* valor capitis */
} VatesEffugium;

structura VatesPetitio {
    Piscina* piscina;
     chorda  exemplar;
        s32  signa_maxima;
     chorda  conatus;
        b32  cogitatio_monstranda;
        b32  cauda_signata;
        Xar* systema;       /* VatesBlocus */
        Xar* instrumenta;   /* VatesInstrumentum */
        Xar* nuntii;        /* VatesNuntius */
        Xar* extra;         /* VatesEffugium */
        Xar* capita;        /* VatesEffugium */
};


/* ======================================================================
 * A. EXEMPLAR NEUTRUM
 * ====================================================================== */

interior chorda
_vacua (vacuum)
{
    chorda v;

    v.datum   = NIHIL;
    v.mensura = 0;
    redde v;
}

VatesPetitio*
vates_petitio_creare (
    Piscina* piscina,
      chorda exemplar)
{
    VatesPetitio* p;

    si (!piscina)
    {
        redde NIHIL;
    }
    p = (VatesPetitio*)piscina_allocare(piscina, (i64)magnitudo(VatesPetitio));
    memset(p, 0, magnitudo(*p));
    p->piscina     = piscina;
    p->exemplar    = exemplar;
    p->systema     = xar_creare(piscina, (i32)magnitudo(VatesBlocus));
    p->instrumenta = xar_creare(piscina, (i32)magnitudo(VatesInstrumentum));
    p->nuntii      = xar_creare(piscina, (i32)magnitudo(VatesNuntius));
    p->extra       = xar_creare(piscina, (i32)magnitudo(VatesEffugium));
    p->capita      = xar_creare(piscina, (i32)magnitudo(VatesEffugium));
    redde p;
}

vacuum
vates_petitio_maxima_ponere (VatesPetitio* petitio, s32 signa_maxima)
{
    si (petitio)
    {
        petitio->signa_maxima = signa_maxima;
    }
}

vacuum
vates_petitio_conatum_ponere (VatesPetitio* petitio, chorda conatus)
{
    si (petitio)
    {
        petitio->conatus = conatus;
    }
}

vacuum
vates_petitio_cogitationem_monstrare (VatesPetitio* petitio, b32 monstrare)
{
    si (petitio)
    {
        petitio->cogitatio_monstranda = monstrare;
    }
}

vacuum
vates_petitio_caudam_signare (VatesPetitio* petitio, b32 signare)
{
    si (petitio)
    {
        petitio->cauda_signata = signare;
    }
}

vacuum
vates_systema_addere (
    VatesPetitio* petitio,
          chorda  textus,
             b32  signum_thesauri)
{
    VatesBlocus* b;

    si (!petitio)
    {
        redde;
    }
    b = (VatesBlocus*)xar_addere(petitio->systema);
    memset(b, 0, magnitudo(*b));
    b->genus           = VATES_TEXTUS;
    b->textus          = textus;
    b->signum_thesauri = signum_thesauri;
}

vacuum
vates_instrumentum_addere (
                  VatesPetitio* petitio,
    constans VatesInstrumentum* instrumentum)
{
    si (petitio && instrumentum)
    {
        *(VatesInstrumentum*)xar_addere(petitio->instrumenta) = *instrumentum;
    }
}

VatesNuntius*
vates_nuntium_addere (
    VatesPetitio* petitio,
       VatesMunus munus)
{
    VatesNuntius* n;

    si (!petitio)
    {
        redde NIHIL;
    }
    n = (VatesNuntius*)xar_addere(petitio->nuntii);
    n->munus = munus;
    n->bloci = xar_creare(petitio->piscina, (i32)magnitudo(VatesBlocus));
    redde n;
}

VatesBlocus*
vates_textum_addere (
    VatesNuntius* nuntius,
           chorda textus)
{
    VatesBlocus* b;

    si (!nuntius)
    {
        redde NIHIL;
    }
    b = (VatesBlocus*)xar_addere(nuntius->bloci);
    memset(b, 0, magnitudo(*b));
    b->genus  = VATES_TEXTUS;
    b->textus = textus;
    redde b;
}

VatesBlocus*
vates_effectum_addere (
    VatesNuntius* nuntius,
           chorda id,
           chorda contentum,
              b32 erratum)
{
    VatesBlocus* b = vates_textum_addere(nuntius, contentum);

    si (b)
    {
        b->genus   = VATES_INSTRUMENTI_EFFECTUS;
        b->id      = id;
        b->erratum = erratum;
    }
    redde b;
}

vacuum
vates_extra_ponere (
          VatesPetitio* petitio,
    constans character* provisor,
    constans character* clavis,
            JsonValor* valor)
{
    VatesEffugium* e;

    si (!petitio || !provisor || !clavis)
    {
        redde;
    }
    e = (VatesEffugium*)xar_addere(petitio->extra);
    memset(e, 0, magnitudo(*e));
    e->provisor = chorda_ex_literis(provisor, petitio->piscina);
    e->clavis   = chorda_ex_literis(clavis, petitio->piscina);
    e->valor    = valor;
}

vacuum
vates_caput_addere (
          VatesPetitio* petitio,
    constans character* provisor,
    constans character* titulus,
    constans character* valor)
{
    VatesEffugium* e;

    si (!petitio || !provisor || !titulus || !valor)
    {
        redde;
    }
    e = (VatesEffugium*)xar_addere(petitio->capita);
    memset(e, 0, magnitudo(*e));
    e->provisor = chorda_ex_literis(provisor, petitio->piscina);
    e->clavis   = chorda_ex_literis(titulus, petitio->piscina);
    e->textus   = chorda_ex_literis(valor, petitio->piscina);
}

vacuum
vates_responsum_addere (
               VatesPetitio* petitio,
    constans VatesResponsum* responsum)
{
    VatesNuntius* n;
    i32           i;

    si (!petitio || !responsum || !responsum->bloci
        || xar_numerus(responsum->bloci) == 0)
    {
        redde;
    }
    n = vates_nuntium_addere(petitio, VATES_ASSISTENS);
    per (i = 0; i < xar_numerus(responsum->bloci); i++)
    {
        *(VatesBlocus*)xar_addere(n->bloci) =
            *(VatesBlocus*)xar_obtinere(responsum->bloci, i);
    }
}

interior VatesResponsum*
_responsum_vacuum (Piscina* piscina)
{
    VatesResponsum* r = (VatesResponsum*)piscina_allocare(piscina,
        (i64)magnitudo(VatesResponsum));

    memset(r, 0, magnitudo(*r));
    r->bloci        = xar_creare(piscina, (i32)magnitudo(VatesBlocus));
    r->usus.pretium = -I;
    redde r;
}

interior s32
_signa_numerare (constans VatesPetitio* p)
{
    s32 numerus = p->cauda_signata ? I : 0;
    i32 i;
    i32 k;

    per (i = 0; i < xar_numerus(p->systema); i++)
    {
        si (((VatesBlocus*)xar_obtinere(p->systema, i))->signum_thesauri)
        {
            numerus++;
        }
    }
    per (i = 0; i < xar_numerus(p->nuntii); i++)
    {
        VatesNuntius* n = (VatesNuntius*)xar_obtinere(p->nuntii, i);

        per (k = 0; k < xar_numerus(n->bloci); k++)
        {
            VatesBlocus* b = (VatesBlocus*)xar_obtinere(n->bloci, k);

            si (b->signum_thesauri && b->genus != VATES_OPACUM)
            {
                numerus++;
            }
        }
    }
    redde numerus;
}


/* ======================================================================
 * A. CORPUS ANTHROPIC
 * ====================================================================== */

interior JsonValor*
_signum_cache (Piscina* piscina)
{
    JsonValor* cc = json_objectum_creare(piscina);

    json_objectum_ponere(cc, "type", json_chorda_creare_literis(piscina, "ephemeral"));
    redde cc;
}

interior b32
_ab_anthropic (constans VatesBlocus* b)
{
    redde b->crudum != NIHIL && chorda_aequalis_literis(b->provisor, "anthropic");
}

/* copia levis objecti + cache_control (crudum ipsum numquam mutatur) */
interior JsonValor*
_copia_signata (JsonValor* crudum, Piscina* piscina)
{
    JsonValor*           o  = json_objectum_creare(piscina);
    JsonObjectumIterator it = json_objectum_iterator(crudum);
    chorda               clavis;
    JsonValor*           valor;

    dum (json_objectum_iterator_proxima(&it, &clavis, &valor))
    {
        json_objectum_ponere_chorda(o, clavis, valor);
    }
    json_objectum_ponere(o, "cache_control", _signum_cache(piscina));
    redde o;
}

interior JsonValor*
_blocum_anthropic (constans VatesBlocus* b, Piscina* piscina)
{
    JsonValor* o = NIHIL;

    si (b->genus == VATES_OPACUM)
    {
        redde _ab_anthropic(b) ? b->crudum : NIHIL;
    }
    si (_ab_anthropic(b))
    {
        redde b->signum_thesauri ? _copia_signata(b->crudum, piscina) : b->crudum;
    }
    o = json_objectum_creare(piscina);
    commutatio (b->genus)
    {
        casus VATES_TEXTUS:
            json_objectum_ponere(o, "type", json_chorda_creare_literis(piscina, "text"));
            json_objectum_ponere(o, "text", json_chorda_creare(piscina, b->textus));
            frange;
        casus VATES_INSTRUMENTUM_PETITUM:
            json_objectum_ponere(o, "type", json_chorda_creare_literis(piscina, "tool_use"));
            json_objectum_ponere(o, "id", json_chorda_creare(piscina, b->id));
            json_objectum_ponere(o, "name", json_chorda_creare(piscina, b->titulus));
            json_objectum_ponere(o, "input",
                b->input ? b->input : json_objectum_creare(piscina));
            frange;
        casus VATES_INSTRUMENTI_EFFECTUS:
            json_objectum_ponere(o, "type", json_chorda_creare_literis(piscina, "tool_result"));
            json_objectum_ponere(o, "tool_use_id", json_chorda_creare(piscina, b->id));
            json_objectum_ponere(o, "content", json_chorda_creare(piscina, b->textus));
            si (b->erratum)
            {
                json_objectum_ponere(o, "is_error", json_boolean_creare(piscina, VERUM));
            }
            frange;
        ordinarius:
            redde NIHIL;
    }
    si (b->signum_thesauri)
    {
        json_objectum_ponere(o, "cache_control", _signum_cache(piscina));
    }
    redde o;
}

interior JsonValor*
_blocos_anthropic (Xar* bloci, Piscina* piscina)
{
    JsonValor* arr = json_tabulatum_creare(piscina);
    i32        i;

    per (i = 0; i < xar_numerus(bloci); i++)
    {
        JsonValor* o = _blocum_anthropic((VatesBlocus*)xar_obtinere(bloci, i),
                                         piscina);

        si (o)
        {
            json_tabulatum_addere(arr, o);
        }
    }
    redde arr;
}

interior JsonValor*
_corpus_anthropic (constans VatesPetitio* pe, Piscina* piscina)
{
    JsonValor* radix = json_objectum_creare(piscina);
    JsonValor* nuntii;
    i32        i;

    json_objectum_ponere(radix, "model", json_chorda_creare(piscina, pe->exemplar));
    json_objectum_ponere(radix, "max_tokens", json_integer_creare(piscina,
        pe->signa_maxima > 0 ? (s64)pe->signa_maxima : (s64)VATES_SIGNA_ORDINARIA));
    si (pe->cogitatio_monstranda)
    {
        JsonValor* c = json_objectum_creare(piscina);

        json_objectum_ponere(c, "type", json_chorda_creare_literis(piscina, "adaptive"));
        json_objectum_ponere(c, "display", json_chorda_creare_literis(piscina, "summarized"));
        json_objectum_ponere(radix, "thinking", c);
    }
    si (pe->conatus.mensura > 0)
    {
        JsonValor* c = json_objectum_creare(piscina);

        json_objectum_ponere(c, "effort", json_chorda_creare(piscina, pe->conatus));
        json_objectum_ponere(radix, "output_config", c);
    }
    si (pe->cauda_signata)
    {
        json_objectum_ponere(radix, "cache_control", _signum_cache(piscina));
    }
    si (xar_numerus(pe->systema) > 0)
    {
        json_objectum_ponere(radix, "system", _blocos_anthropic(pe->systema, piscina));
    }
    si (xar_numerus(pe->instrumenta) > 0)
    {
        JsonValor* arr = json_tabulatum_creare(piscina);

        per (i = 0; i < xar_numerus(pe->instrumenta); i++)
        {
            VatesInstrumentum* t = (VatesInstrumentum*)xar_obtinere(pe->instrumenta, i);
            JsonValor*         o = json_objectum_creare(piscina);

            json_objectum_ponere(o, "name", json_chorda_creare(piscina, t->titulus));
            json_objectum_ponere(o, "description", json_chorda_creare(piscina, t->descriptio));
            json_objectum_ponere(o, "input_schema",
                t->schema ? t->schema : json_objectum_creare(piscina));
            json_tabulatum_addere(arr, o);
        }
        json_objectum_ponere(radix, "tools", arr);
    }
    nuntii = json_tabulatum_creare(piscina);
    per (i = 0; i < xar_numerus(pe->nuntii); i++)
    {
        VatesNuntius* n       = (VatesNuntius*)xar_obtinere(pe->nuntii, i);
        JsonValor*    content = _blocos_anthropic(n->bloci, piscina);
        JsonValor*    o;

        si (json_tabulatum_numerus(content) == 0)
        {
            perge;   /* nuntius vacuus omittitur (Review focus 5) */
        }
        o = json_objectum_creare(piscina);
        json_objectum_ponere(o, "role", json_chorda_creare_literis(piscina,
            n->munus == VATES_USOR ? "user" : "assistant"));
        json_objectum_ponere(o, "content", content);
        json_tabulatum_addere(nuntii, o);
    }
    json_objectum_ponere(radix, "messages", nuntii);
    per (i = 0; i < xar_numerus(pe->extra); i++)
    {
        VatesEffugium* e = (VatesEffugium*)xar_obtinere(pe->extra, i);

        si (chorda_aequalis_literis(e->provisor, "anthropic"))
        {
            json_objectum_ponere_chorda(radix, e->clavis, e->valor);
        }
    }
    redde radix;
}

chorda
vates_anthropic_corpus (
    constans VatesPetitio* petitio,
                 Piscina* piscina)
{
    si (!petitio || !piscina)
    {
        redde _vacua();
    }
    redde json_scribere(_corpus_anthropic(petitio, piscina), piscina);
}
```

- [ ] **Step 3.4: register + green.** `./tools/compile_tests_fontes_generare.sh`;
  `./compile_tests.sh probatio_vates` PASS. If the golden JSON differs
  ONLY by the json writer's escaping of non-ASCII, the writer is the
  truth (it is what goes on the wire): compare against the API's
  needs, not taste, and ledger a ruling with the exact bytes.
- [ ] **Step 3.5: plants** - (1) `_blocum_anthropic` ignores
  `signum_thesauri` -> minimum golden red; (2) foreign OPACUM emitted
  -> opaca golden red; (3) empty-message skip removed -> opaca golden
  red (empty content array); (4) extras emitted regardless of provider
  -> plenum red (`"store":false`). Restore + `cmp`.
- [ ] **Step 3.6: lint + worklog + commit.** `lib/vates.worklog.md`
  (render rules above). Gates `radix`, `generata`. `commissio_umbra`.

---

### Task 4: vates - reading Anthropic responses (pure)

**Files:** Modify `lib/vates.c` (part B, appended), `probationes/probatio_vates.c`.

**Interfaces:** Produces `vates_anthropic_legere`; internal
`interior VatesResponsum* _legere (i32 status, chorda corpus, Piscina*, chorda* novitas)`
used by T5 (novitas = what was not modelled, for herbarium).

Parse rules (worklog):
- every content block keeps `crudum` + `provisor = "anthropic"`; genus:
  `text` -> TEXTUS (`textus` filled), `tool_use` -> PETITUM (id, name,
  input), everything else -> OPACUM (`thinking`, `redacted_thinking`
  expected; any other type = NOVITAS "blocus ignotus: <type>");
- known keys: text {type,text,citations}, tool_use {type,id,name,input};
  an unknown key on a known block = NOVITAS "campus ignotus in <type>:
  <key>" (block still usable; crudum preserves it);
- top-level known keys {id,type,role,model,content,stop_reason,
  stop_sequence,stop_details,usage}; unknown = NOVITAS;
- `stop_reason` map: end_turn/stop_sequence -> FINIS, tool_use ->
  INSTRUMENTUM, max_tokens -> MAXIMUM, refusal -> RECUSATIO (category
  from `stop_details.category`), pause_turn -> PAUSA, other -> ALIA +
  NOVITAS;
- usage: `cache_creation.ephemeral_{5m,1h}_input_tokens` when present,
  else all of `cache_creation_input_tokens` counts as 5m;
- status != 200 -> STATUS with `error.type` / `error.message` (or the
  first CCLVI bytes of a non-JSON body); 200 + unparseable -> PARSE.
- only the first NOVITAS is reported (one press per response).

- [ ] **Step 4.1: failing tests** - add to `probatio_vates.c` (before
  `principale`) and call `probatio_legere(piscina);` under `/* T4 */`:

```c
interior VatesBlocus*
_blocus (VatesResponsum* r, i32 i)
{
    redde (VatesBlocus*)xar_obtinere(r->bloci, i);
}

interior vacuum
probatio_legere(Piscina* piscina)
{
    VatesResponsum* r;
    VatesPetitio*   p;
    VatesNuntius*   n;

    imprimere("\n--- Probans vates_anthropic_legere ---\n");

    /* textus + usus cum cache_creation */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"msg_1\",\"type\":\"message\",\"role\":\"assistant\","
        "\"model\":\"claude-opus-5-5\",\"content\":[{\"type\":\"text\",\"text\":\"Salve!\"}],"
        "\"stop_reason\":\"end_turn\",\"stop_sequence\":null,\"usage\":{\"input_tokens\":12,"
        "\"cache_read_input_tokens\":3000,\"cache_creation_input_tokens\":400,"
        "\"cache_creation\":{\"ephemeral_5m_input_tokens\":300,\"ephemeral_1h_input_tokens\":100},"
        "\"output_tokens\":5}}", piscina), piscina);
    CREDO_VERUM(r->successus);
    CREDO_VERUM(r->causa_finis == VATES_FINIS);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->id, "msg_1");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->exemplar, "claude-opus-5-5");
    CREDO_AEQUALIS_I32(xar_numerus(r->bloci), I);
    CREDO_VERUM(_blocus(r, 0)->genus == VATES_TEXTUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(_blocus(r, 0)->textus, "Salve!");
    CREDO_AEQUALIS_S64(r->usus.input, 12);
    CREDO_AEQUALIS_S64(r->usus.cache_lectum, 3000);
    CREDO_AEQUALIS_S64(r->usus.cache_scriptum_5m, 300);
    CREDO_AEQUALIS_S64(r->usus.cache_scriptum_1h, 100);
    CREDO_AEQUALIS_S64(r->usus.output, 5);

    /* usus sine cache_creation: totum in 5m */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[],\"stop_reason\":\"end_turn\",\"stop_sequence\":null,"
        "\"usage\":{\"input_tokens\":1,\"cache_creation_input_tokens\":77,\"output_tokens\":1}}",
        piscina), piscina);
    CREDO_AEQUALIS_S64(r->usus.cache_scriptum_5m, 77);

    /* cogitatio + tool_use: OPACUM et PETITUM; reditus verbatim */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m2\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[{\"type\":\"thinking\",\"thinking\":\"\",\"signature\":\"SIG\"},"
        "{\"type\":\"tool_use\",\"id\":\"toolu_9\",\"name\":\"zoom\",\"input\":{\"id\":0,\"n\":8}}],"
        "\"stop_reason\":\"tool_use\",\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}",
        piscina), piscina);
    CREDO_VERUM(r->causa_finis == VATES_FINIS_INSTRUMENTUM);
    CREDO_VERUM(_blocus(r, 0)->genus == VATES_OPACUM);
    CREDO_VERUM(_blocus(r, I)->genus == VATES_INSTRUMENTUM_PETITUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_blocus(r, I)->id, "toolu_9");
    CREDO_CHORDA_AEQUALIS_LITERIS(_blocus(r, I)->titulus, "zoom");
    CREDO_AEQUALIS_S64(json_capere_integer(_blocus(r, I)->input, "n", 0), 8);
    p = vates_petitio_creare(piscina, _c("x", piscina));
    vates_responsum_addere(p, r);
    n = vates_nuntium_addere(p, VATES_USOR);
    vates_effectum_addere(n, _c("toolu_9", piscina), _c("linea", piscina), FALSUM);
    CREDO_CHORDA_CONTINET(vates_anthropic_corpus(p, piscina), _c(
        "{\"role\":\"assistant\",\"content\":[{\"type\":\"thinking\",\"thinking\":\"\","
        "\"signature\":\"SIG\"},{\"type\":\"tool_use\",\"id\":\"toolu_9\",\"name\":\"zoom\","
        "\"input\":{\"id\":0,\"n\":8}}]}", piscina));

    /* citationes: TEXTUS cum crudo - redditur cum citationibus */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m3\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[{\"type\":\"text\",\"text\":\"t\",\"citations\":[{\"x\":1}]}],"
        "\"stop_reason\":\"end_turn\",\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}",
        piscina), piscina);
    CREDO_VERUM(_blocus(r, 0)->genus == VATES_TEXTUS);
    p = vates_petitio_creare(piscina, _c("x", piscina));
    vates_responsum_addere(p, r);
    CREDO_CHORDA_CONTINET(vates_anthropic_corpus(p, piscina),
                          _c("\"citations\":[{\"x\":1}]", piscina));

    /* recusatio cum categoria */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m4\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[],\"stop_reason\":\"refusal\",\"stop_sequence\":null,"
        "\"stop_details\":{\"type\":\"refusal\",\"category\":\"cyber\",\"explanation\":\"e\"},"
        "\"usage\":{\"input_tokens\":1,\"output_tokens\":0}}", piscina), piscina);
    CREDO_VERUM(r->successus);
    CREDO_VERUM(r->causa_finis == VATES_FINIS_RECUSATIO);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->recusatio_categoria, "cyber");

    /* max_tokens */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m5\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[{\"type\":\"text\",\"text\":\"trunc\"}],\"stop_reason\":\"max_tokens\","
        "\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":9}}",
        piscina), piscina);
    CREDO_VERUM(r->causa_finis == VATES_FINIS_MAXIMUM);

    /* errores provisoris */
    r = vates_anthropic_legere(DXXIX, _c(
        "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"},"
        "\"request_id\":\"req_1\"}", piscina), piscina);
    CREDO_FALSUM(r->successus);
    CREDO_VERUM(r->error == VATES_ERROR_STATUS);
    CREDO_AEQUALIS_I32(r->status_http, DXXIX);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->error_genus, "overloaded_error");
    CREDO_CHORDA_AEQUALIS_LITERIS(r->error_nuntius, "Overloaded");
    r = vates_anthropic_legere(DII, _c("<html>bad gateway</html>", piscina), piscina);
    CREDO_VERUM(r->error == VATES_ERROR_STATUS);
    CREDO_CHORDA_CONTINET(r->error_nuntius, _c("bad gateway", piscina));

    /* CC sed non legibile */
    r = vates_anthropic_legere(CC, _c("{\"id\":", piscina), piscina);
    CREDO_FALSUM(r->successus);
    CREDO_VERUM(r->error == VATES_ERROR_PARSE);

    /* blocus ignotus -> OPACUM, non abicitur */
    r = vates_anthropic_legere(CC, _c(
        "{\"id\":\"m6\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"x\","
        "\"content\":[{\"type\":\"server_tool_use\",\"id\":\"s1\"},{\"type\":\"text\",\"text\":\"ok\"}],"
        "\"stop_reason\":\"end_turn\",\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}",
        piscina), piscina);
    CREDO_AEQUALIS_I32(xar_numerus(r->bloci), II);
    CREDO_VERUM(_blocus(r, 0)->genus == VATES_OPACUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(_blocus(r, 0)->provisor, "anthropic");
}
```

- [ ] **Step 4.2: run, expect failure** (`vates_anthropic_legere` undefined).

- [ ] **Step 4.3: implement part B** (append to `lib/vates.c`):

```c
/* ======================================================================
 * B. LECTIO ANTHROPIC
 * ====================================================================== */

hic_manens constans character* _claves_summae[] = {
    "id", "type", "role", "model", "content", "stop_reason",
    "stop_sequence", "stop_details", "usage", NIHIL
};
hic_manens constans character* _claves_textus[] = {
    "type", "text", "citations", NIHIL
};
hic_manens constans character* _claves_petiti[] = {
    "type", "id", "name", "input", NIHIL
};

interior b32
_in_indice (chorda clavis, constans character* constans* index)
{
    i32 i;

    per (i = 0; index[i]; i++)
    {
        si (chorda_aequalis_literis(clavis, index[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* prima novitas sola servatur */
interior vacuum
_novitas (
               chorda* novitas,
    constans character* praefixum,
               chorda  quid,
             Piscina*  piscina)
{
    character buffer[CCLVI];

    si (novitas->mensura > 0)
    {
        redde;
    }
    sprintf(buffer, "%s%.*s", praefixum,
            (integer)(quid.mensura > CC ? CC : quid.mensura),
            (constans character*)quid.datum);
    *novitas = chorda_ex_literis(buffer, piscina);
}

interior vacuum
_claves_probare (
            JsonValor* obj,
    constans character* constans* nota,
    constans character* praefixum,
               chorda* novitas,
             Piscina*  piscina)
{
    JsonObjectumIterator it = json_objectum_iterator(obj);
    chorda               clavis;
    JsonValor*           valor;

    dum (json_objectum_iterator_proxima(&it, &clavis, &valor))
    {
        si (!_in_indice(clavis, nota))
        {
            _novitas(novitas, praefixum, clavis, piscina);
        }
    }
}

interior VatesCausaFinis
_causa_finis (chorda s, chorda* novitas, Piscina* piscina)
{
    si (chorda_aequalis_literis(s, "end_turn")
        || chorda_aequalis_literis(s, "stop_sequence"))
    {
        redde VATES_FINIS;
    }
    si (chorda_aequalis_literis(s, "tool_use"))
    {
        redde VATES_FINIS_INSTRUMENTUM;
    }
    si (chorda_aequalis_literis(s, "max_tokens"))
    {
        redde VATES_FINIS_MAXIMUM;
    }
    si (chorda_aequalis_literis(s, "refusal"))
    {
        redde VATES_FINIS_RECUSATIO;
    }
    si (chorda_aequalis_literis(s, "pause_turn"))
    {
        redde VATES_FINIS_PAUSA;
    }
    _novitas(novitas, "stop_reason ignota: ", s, piscina);
    redde VATES_FINIS_ALIA;
}

interior VatesResponsum*
_legere (
       i32  status,
    chorda  corpus,
  Piscina*  piscina,
   chorda*  novitas)
{
    VatesResponsum* r = _responsum_vacuum(piscina);
    JsonResultus    j = json_legere(corpus, piscina);
    chorda          vacua = chorda_ex_literis("", piscina);
    JsonValor*      content;
    JsonValor*      usus;
    i32             i;

    *novitas      = _vacua();
    r->status_http = status;
    r->conatus     = I;
    si (status != CC)
    {
        r->error = VATES_ERROR_STATUS;
        si (j.successus)
        {
            JsonValor* e = json_objectum_capere(j.radix, "error");

            r->error_genus   = json_capere_chorda(e, "type", vacua);
            r->error_nuntius = json_capere_chorda(e, "message", vacua);
        }
        alioquin
        {
            r->error_nuntius.datum   = corpus.datum;
            r->error_nuntius.mensura = corpus.mensura > CCLVI ? CCLVI : corpus.mensura;
        }
        redde r;
    }
    si (!j.successus || !json_est_objectum(j.radix))
    {
        r->error         = VATES_ERROR_PARSE;
        r->error_nuntius = j.successus ? chorda_ex_literis("radix non objectum", piscina)
                                       : j.error;
        redde r;
    }
    _claves_probare(j.radix, _claves_summae, "campus ignotus: ", novitas, piscina);
    r->id                 = json_capere_chorda(j.radix, "id", vacua);
    r->exemplar           = json_capere_chorda(j.radix, "model", vacua);
    r->causa_finis_cruda  = json_capere_chorda(j.radix, "stop_reason", vacua);
    r->causa_finis        = _causa_finis(r->causa_finis_cruda, novitas, piscina);
    r->recusatio_categoria = json_capere_chorda(
        json_objectum_capere(j.radix, "stop_details"), "category", vacua);

    content = json_objectum_capere(j.radix, "content");
    per (i = 0; content && i < json_tabulatum_numerus(content); i++)
    {
        JsonValor*   e     = json_tabulatum_obtinere(content, i);
        chorda       typus = json_capere_chorda(e, "type", vacua);
        VatesBlocus* b     = (VatesBlocus*)xar_addere(r->bloci);

        memset(b, 0, magnitudo(*b));
        b->crudum   = e;
        b->provisor = chorda_ex_literis("anthropic", piscina);
        si (chorda_aequalis_literis(typus, "text"))
        {
            b->genus  = VATES_TEXTUS;
            b->textus = json_capere_chorda(e, "text", vacua);
            _claves_probare(e, _claves_textus, "campus ignotus in text: ",
                            novitas, piscina);
        }
        alioquin si (chorda_aequalis_literis(typus, "tool_use"))
        {
            b->genus   = VATES_INSTRUMENTUM_PETITUM;
            b->id      = json_capere_chorda(e, "id", vacua);
            b->titulus = json_capere_chorda(e, "name", vacua);
            b->input   = json_objectum_capere(e, "input");
            _claves_probare(e, _claves_petiti, "campus ignotus in tool_use: ",
                            novitas, piscina);
        }
        alioquin
        {
            b->genus = VATES_OPACUM;
            si (!chorda_aequalis_literis(typus, "thinking")
                && !chorda_aequalis_literis(typus, "redacted_thinking"))
            {
                _novitas(novitas, "blocus ignotus: ", typus, piscina);
            }
        }
    }

    usus = json_objectum_capere(j.radix, "usage");
    r->usus.input        = json_capere_integer(usus, "input_tokens", 0);
    r->usus.output       = json_capere_integer(usus, "output_tokens", 0);
    r->usus.cache_lectum = json_capere_integer(usus, "cache_read_input_tokens", 0);
    si (json_objectum_capere(usus, "cache_creation"))
    {
        JsonValor* cc = json_objectum_capere(usus, "cache_creation");

        r->usus.cache_scriptum_5m = json_capere_integer(cc, "ephemeral_5m_input_tokens", 0);
        r->usus.cache_scriptum_1h = json_capere_integer(cc, "ephemeral_1h_input_tokens", 0);
    }
    alioquin
    {
        r->usus.cache_scriptum_5m = json_capere_integer(usus,
            "cache_creation_input_tokens", 0);
    }
    r->successus = VERUM;
    r->error     = VATES_OK;
    redde r;
}

VatesResponsum*
vates_anthropic_legere (
       i32  status_http,
    chorda  corpus,
  Piscina*  piscina)
{
    chorda novitas;

    redde _legere(status_http, corpus, piscina, &novitas);
}
```

- [ ] **Step 4.4: green** - `./compile_tests.sh probatio_vates` PASS.
- [ ] **Step 4.5: plants** - (1) unknown block types dropped instead
  of OPACUM -> count II red; (2) `cache_creation` object ignored ->
  5m/1h red; (3) STATUS path reads `error.message` into `error_genus`
  -> red; (4) render ignores `crudum` for TEXTUS -> citations red.
- [ ] **Step 4.6: worklog + commit** (parse rules above). Gates
  `radix`, `generata`.

---

### Task 5: vates - providers by name, `vates_mittere`, accounting, fictus

**Files:** Modify `lib/vates.c` (part C, appended), `probationes/probatio_vates.c`.

**Interfaces:** Produces (header) `vates_optiones_ordinariae`,
`vates_aperire`, `vates_provisor`, `vates_anthropic_aperire`,
`vates_mittere`, `vates_error_descriptio`, `vates_fictus_aperire`,
`vates_fictus_textum`, `vates_fictus_instrumentum`,
`vates_fictus_crudum`, `vates_fictus_petitiones_numerus`,
`vates_fictus_petitio`. Consumes T1 (`filum_appendere_firmiter`), T2
(herbarium), T3/T4 internals.

Decisions (worklog):
- the provider table is compiled in (`{"anthropic", ...}, {"fictus", ...}`);
  unknown name -> NIHIL + stderr listing the known names;
- the key is copied ONCE into the `Vates` (NUL-terminated for the
  header). `http_petitio_caput_addere` necessarily copies it into the
  call's piscina too (http's API); nothing else holds it;
- retry: RETE always, TEMPUS only on the first attempt, HTTP 408, 409,
  429, >= 500; wait = `retry-after` seconds if present else
  `mora_iterandi_ms`, always capped by `mora_iterandi_maxima_ms`;
- usage on the returned response = sum over attempts; each attempt is
  its own ledger line; price -1 when the model is not in the table;
- the Anthropic herbarium key = status + `error.type` + masked
  `error.message`, else herbarium's skeleton key. The masking function
  is a 30-line copy of herbarium's (not exported: the approved header
  stays as approved) - Ruling recorded;
- `piscina == NIHIL` -> NIHIL (nothing to allocate in), the one
  exception to "never NIHIL" - documented in the worklog.

- [ ] **Step 5.1: failing tests** - add to `probatio_vates.c` and call
  each under `/* T5 */` in `principale`:

```c
/* ---- vectura scripta pro provisore Anthropic: capita capit ---- */
nomen structura {
             i32  status;          /* 0 = HTTP_ERROR_TIMEOUT */
    constans character* corpus;
    constans character* retry_after; /* NIHIL = sine */
} Scriptum;

nomen structura {
    constans Scriptum* scripta;
                 i32  numerus;
                 i32  index;
           character  capita_visa[MMMCMXCIX];
} ScriptorVatis;

interior HttpResultus
_scriptor_exsequi (HttpPetitio* petitio, Piscina* piscina, vacuum* datum)
{
    ScriptorVatis*   s = (ScriptorVatis*)datum;
    HttpPetitioVisus visus = http_petitio_visus(petitio);
    HttpResultus     res;
    HttpResponsum*   resp;
    constans Scriptum* sc;
    i32              i;
    size_t           longitudo = 0;

    s->capita_visa[0] = '\0';
    per (i = 0; i < visus.capita_numerus; i++)
    {
        longitudo = strlen(s->capita_visa);
        sprintf(s->capita_visa + longitudo, "%.*s: %.*s\n",
                (integer)visus.capita[i].titulus.mensura,
                (constans character*)visus.capita[i].titulus.datum,
                (integer)visus.capita[i].valor.mensura,
                (constans character*)visus.capita[i].valor.datum);
    }
    memset(&res, 0, magnitudo(res));
    si (s->index >= s->numerus)
    {
        res.error = HTTP_ERROR_CONNEXIO;
        redde res;
    }
    sc = &s->scripta[s->index];
    s->index++;
    si (sc->status == 0)
    {
        res.error = HTTP_ERROR_TIMEOUT;
        res.error_descriptio = chorda_ex_literis("tempus scriptum", piscina);
        redde res;
    }
    resp = (HttpResponsum*)piscina_allocare(piscina, (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status = sc->status;
    resp->corpus = chorda_ex_literis(sc->corpus, piscina);
    resp->capita = (HttpCaput*)piscina_allocare(piscina, (i64)magnitudo(HttpCaput));
    si (sc->retry_after)
    {
        resp->capita[0].titulus = chorda_ex_literis("retry-after", piscina);
        resp->capita[0].valor = chorda_ex_literis(sc->retry_after, piscina);
        resp->capita_numerus = I;
    }
    res.successus = VERUM;
    res.responsum = resp;
    redde res;
}

hic_manens constans character* _TEXTUS_BONUS =
    "{\"id\":\"msg_b\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"m\","
    "\"content\":[{\"type\":\"text\",\"text\":\"bene\"}],\"stop_reason\":\"end_turn\","
    "\"stop_sequence\":null,\"usage\":{\"input_tokens\":10,\"output_tokens\":2}}";
hic_manens constans character* _ONUSTUS =
    "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"}}";

interior VatesPetitio*
_petitio_simplex (Piscina* piscina)
{
    VatesPetitio* p = vates_petitio_creare(piscina, _c("m", piscina));

    vates_textum_addere(vates_nuntium_addere(p, VATES_USOR), _c("salve", piscina));
    redde p;
}

interior i32
_lineae (constans character* via, Piscina* piscina)
{
    chorda t = filum_legere_totum(via, piscina);
    i32    i;
    i32    n = 0;

    per (i = 0; i < t.mensura; i++)
    {
        si (t.datum[i] == '\n')
        {
            n++;
        }
    }
    redde n;
}

interior vacuum
probatio_provisores(Piscina* piscina)
{
    VatesOptiones o = vates_optiones_ordinariae();
    Vates*        v;

    imprimere("\n--- Probans vates_aperire per nomen ---\n");
    v = vates_aperire(piscina, _c("anthropic", piscina), _c("k", piscina), &o);
    CREDO_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(vates_provisor(v), "anthropic");
    v = vates_aperire(piscina, _c("fictus", piscina), _c("", piscina), &o);
    CREDO_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(vates_provisor(v), "fictus");
    CREDO_NIHIL(vates_aperire(piscina, _c("openai", piscina), _c("k", piscina), &o));
}

interior vacuum
probatio_fictus_et_rationarium(Piscina* piscina)
{
    VatesOptiones   o = vates_optiones_ordinariae();
    VatesPretium    pretium;
    VatesUsus       u;
    Vates*          v;
    VatesPetitio*   p;
    VatesResponsum* r;
    character       via[CCLVI];
    chorda          linea;

    imprimere("\n--- Probans fictus + pretium + rationarium ---\n");
    sprintf(via, "/tmp/probatio_vates_rationarium_%ld.jsonl", (longus)getpid());
    (vacuum)unlink(via);
    pretium.exemplar     = "fictus";
    pretium.input        = IV * M * M;      /* $4 / MTok */
    pretium.output       = XX * M * M;
    pretium.cache_lectum = CC * M;
    pretium.cache_5m     = V * M * M;
    pretium.cache_1h     = VIII * M * M;
    o.pretia = &pretium;
    o.pretia_numerus = I;
    o.rationarium_via = via;
    v = vates_fictus_aperire(piscina, &o);
    memset(&u, 0, magnitudo(u));
    u.input = M;
    u.cache_lectum = MM;
    u.cache_scriptum_5m = CCC;
    u.output = L;
    vates_fictus_textum(v, _c("responsum fictum", piscina), u);
    p = _petitio_simplex(piscina);
    r = vates_mittere(v, p, _c("probatio", piscina), piscina);
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32(r->conatus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(((VatesBlocus*)xar_obtinere(r->bloci, 0))->textus,
                                  "responsum fictum");
    /* (1000*4e6 + 2000*2e5 + 300*5e6 + 50*2e7) / 1e6 = 6900 */
    CREDO_AEQUALIS_S64(r->usus.pretium, 6900);
    CREDO_AEQUALIS_I32(_lineae(via, piscina), I);
    linea = filum_legere_totum(via, piscina);
    CREDO_CHORDA_CONTINET(linea, _c("\"propositum\":\"probatio\"", piscina));
    CREDO_CHORDA_CONTINET(linea, _c("\"pretium\":6900", piscina));
    /* fictus: corpus missum = corpus purum */
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), I);
    CREDO_CHORDAE_AEQUALES(vates_fictus_petitio(v, 0), vates_anthropic_corpus(p, piscina));
    (vacuum)unlink(via);
}

interior vacuum
probatio_iterationes(Piscina* piscina)
{
    VatesOptiones   o = vates_optiones_ordinariae();
    VatesUsus       u;
    Vates*          v;
    VatesResponsum* r;
    character       via[CCLVI];

    imprimere("\n--- Probans iterationes ---\n");
    sprintf(via, "/tmp/probatio_vates_iter_%ld.jsonl", (longus)getpid());
    (vacuum)unlink(via);
    o.mora_iterandi_ms = I;
    o.rationarium_via = via;
    memset(&u, 0, magnitudo(u));
    u.input = X;
    u.output = II;

    /* 429 deinde bene: conatus II, lineae II, usus summatus */
    v = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, CDXXIX,
        "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"x\"}}");
    vates_fictus_textum(v, _c("bene", piscina), u);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina), piscina);
    CREDO_VERUM(r->successus);
    CREDO_AEQUALIS_I32(r->conatus, II);
    CREDO_AEQUALIS_I32(_lineae(via, piscina), II);
    CREDO_AEQUALIS_S64(r->usus.input, X);

    /* 400: non iteratur */
    v = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, CD,
        "{\"type\":\"error\",\"error\":{\"type\":\"invalid_request_error\",\"message\":\"y\"}}");
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina), piscina);
    CREDO_FALSUM(r->successus);
    CREDO_AEQUALIS_I32(r->conatus, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->error_genus, "invalid_request_error");

    /* 529 ter: conatus III, STATUS */
    v = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, DXXIX, _ONUSTUS);
    vates_fictus_crudum(v, DXXIX, _ONUSTUS);
    vates_fictus_crudum(v, DXXIX, _ONUSTUS);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina), piscina);
    CREDO_VERUM(r->error == VATES_ERROR_STATUS);
    CREDO_AEQUALIS_I32(r->conatus, III);

    /* fictus exhaustus: RETE, iteratur usque ad III */
    v = vates_fictus_aperire(piscina, &o);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina), piscina);
    CREDO_VERUM(r->error == VATES_ERROR_RETE);
    CREDO_AEQUALIS_I32(r->conatus, III);
    (vacuum)unlink(via);
}

interior vacuum
probatio_limes(Piscina* piscina)
{
    VatesOptiones   o = vates_optiones_ordinariae();
    Vates*          v = vates_fictus_aperire(piscina, &o);
    VatesPetitio*   p = vates_petitio_creare(piscina, _c("m", piscina));
    VatesNuntius*   n = vates_nuntium_addere(p, VATES_USOR);
    VatesResponsum* r;
    i32             i;

    imprimere("\n--- Probans limes punctorum cache ---\n");
    vates_petitio_caudam_signare(p, VERUM);
    per (i = 0; i < IV; i++)
    {
        vates_textum_addere(n, _c("x", piscina))->signum_thesauri = VERUM;
    }
    r = vates_mittere(v, p, _c("t", piscina), piscina);
    CREDO_VERUM(r->error == VATES_ERROR_LIMES);
    CREDO_AEQUALIS_I32(vates_fictus_petitiones_numerus(v), 0);
}

interior vacuum
probatio_tempus_et_retry_after(Piscina* piscina)
{
    VatesOptiones   o = vates_optiones_ordinariae();
    ScriptorVatis   s;
    Scriptum        scripta[II];
    Vates*          v;
    VatesResponsum* r;
    time_t          initium;

    imprimere("\n--- Probans retry-after hostile + TEMPUS semel ---\n");
    scripta[0].status = CDXXIX;
    scripta[0].corpus = "{\"type\":\"error\",\"error\":{\"type\":\"rate_limit_error\",\"message\":\"z\"}}";
    scripta[0].retry_after = "3600";
    scripta[I].status = CC;
    scripta[I].corpus = _TEXTUS_BONUS;
    scripta[I].retry_after = NIHIL;
    memset(&s, 0, magnitudo(s));
    s.scripta = scripta;
    s.numerus = II;
    o.mora_iterandi_maxima_ms = V;
    o.vectura.exsequi = _scriptor_exsequi;
    o.vectura.datum = &s;
    v = vates_anthropic_aperire(piscina, _c("k", piscina), &o);
    initium = time(NIHIL);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina), piscina);
    CREDO_VERUM(r->successus);
    CREDO_VERUM(time(NIHIL) - initium < II);

    /* TEMPUS bis: iteratur semel tantum */
    scripta[0].status = 0;
    scripta[I].status = 0;
    s.index = 0;
    v = vates_anthropic_aperire(piscina, _c("k", piscina), &o);
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina), piscina);
    CREDO_VERUM(r->error == VATES_ERROR_TEMPUS);
    CREDO_AEQUALIS_I32(r->conatus, II);
}

interior vacuum
probatio_clavis_non_effunditur(Piscina* piscina)
{
    VatesOptiones   o = vates_optiones_ordinariae();
    ScriptorVatis   s;
    Scriptum        scripta[II];
    Vates*          v;
    VatesPetitio*   p;
    VatesResponsum* r;
    character       rat[CCLVI];
    character       herb[CCLVI];
    character       via[DXII];
    chorda          clavis = _c("CLAVIS_SECRETA_PROBATIONIS", piscina);

    imprimere("\n--- Probans clavis numquam effunditur ---\n");
    sprintf(rat, "/tmp/probatio_vates_clavis_%ld.jsonl", (longus)getpid());
    sprintf(herb, "/tmp/probatio_vates_herb_%ld", (longus)getpid());
    (vacuum)unlink(rat);
    scripta[0].status = DXXIX;
    scripta[0].corpus = _ONUSTUS;
    scripta[0].retry_after = NIHIL;
    scripta[I].status = CC;
    scripta[I].corpus = _TEXTUS_BONUS;
    scripta[I].retry_after = NIHIL;
    memset(&s, 0, magnitudo(s));
    s.scripta = scripta;
    s.numerus = II;
    o.mora_iterandi_ms = I;
    o.rationarium_via = rat;
    o.herbarium_via = herb;
    o.vectura.exsequi = _scriptor_exsequi;
    o.vectura.datum = &s;
    v = vates_anthropic_aperire(piscina, clavis, &o);
    p = _petitio_simplex(piscina);
    vates_caput_addere(p, "anthropic", "anthropic-beta", "beta-probationis");
    vates_caput_addere(p, "openai", "openai-beta", "alienum");
    r = vates_mittere(v, p, _c("t", piscina), piscina);
    CREDO_VERUM(r->successus);
    /* clavis in capite petitionis - et solum ibi */
    CREDO_VERUM(strstr(s.capita_visa, "x-api-key: CLAVIS_SECRETA_PROBATIONIS") != NIHIL);
    CREDO_VERUM(strstr(s.capita_visa, "anthropic-beta: beta-probationis") != NIHIL);
    CREDO_VERUM(strstr(s.capita_visa, "openai-beta") == NIHIL);
    CREDO_FALSUM(chorda_continet(filum_legere_totum(rat, piscina), clavis));
    sprintf(via, "%s/index.jsonl", herb);
    CREDO_FALSUM(chorda_continet(filum_legere_totum(via, piscina), clavis));
    {
        Xar* sp = herbarium_enumerare(piscina, herb);
        i32  i;

        CREDO_AEQUALIS_I32(xar_numerus(sp), I);
        per (i = 0; i < xar_numerus(sp); i++)
        {
            HerbariumSpecimen* h = (HerbariumSpecimen*)xar_obtinere(sp, i);

            CREDO_FALSUM(chorda_continet(h->corpus, clavis));
            CREDO_CHORDA_INCIPIT(h->clavis, _c("529:overloaded_error:", piscina));
            sprintf(via, "%s/specimina/%.*s-%u.json", herb,
                    (integer)h->sigillum.mensura,
                    (constans character*)h->sigillum.datum, h->variantes_index);
            CREDO_FALSUM(chorda_continet(filum_legere_totum(via, piscina), clavis));
            (vacuum)unlink(via);
        }
    }
    sprintf(via, "%s/index.jsonl", herb);
    (vacuum)unlink(via);
    sprintf(via, "%s/specimina", herb);
    (vacuum)rmdir(via);
    (vacuum)rmdir(herb);
    (vacuum)unlink(rat);
}

interior vacuum
probatio_novitas_pressa(Piscina* piscina)
{
    VatesOptiones   o = vates_optiones_ordinariae();
    Vates*          v;
    VatesResponsum* r;
    character       herb[CCLVI];
    character       via[DXII];
    Xar*            sp;
    i32             i;
    b32             inventa = FALSUM;

    imprimere("\n--- Probans novitas in herbarium premitur ---\n");
    sprintf(herb, "/tmp/probatio_vates_novitas_%ld", (longus)getpid());
    o.herbarium_via = herb;
    v = vates_fictus_aperire(piscina, &o);
    vates_fictus_crudum(v, CC,
        "{\"id\":\"m\",\"type\":\"message\",\"role\":\"assistant\",\"model\":\"m\","
        "\"content\":[{\"type\":\"server_tool_use\",\"id\":\"s\"}],\"stop_reason\":\"end_turn\","
        "\"stop_sequence\":null,\"usage\":{\"input_tokens\":1,\"output_tokens\":1}}");
    r = vates_mittere(v, _petitio_simplex(piscina), _c("t", piscina), piscina);
    CREDO_VERUM(r->successus);
    sp = herbarium_enumerare(piscina, herb);
    per (i = 0; i < xar_numerus(sp); i++)
    {
        HerbariumSpecimen* h = (HerbariumSpecimen*)xar_obtinere(sp, i);

        si (chorda_aequalis_literis(h->causa, "blocus ignotus: server_tool_use"))
        {
            inventa = VERUM;
        }
        sprintf(via, "%s/specimina/%.*s-%u.json", herb, (integer)h->sigillum.mensura,
                (constans character*)h->sigillum.datum, h->variantes_index);
        (vacuum)unlink(via);
    }
    CREDO_VERUM(inventa);
    sprintf(via, "%s/index.jsonl", herb);
    (vacuum)unlink(via);
    sprintf(via, "%s/specimina", herb);
    (vacuum)rmdir(via);
    (vacuum)rmdir(herb);
}
```

  Also add `#include <time.h>` to the test's includes.

- [ ] **Step 5.2: run, expect failure** (undefined `vates_aperire` etc.).

- [ ] **Step 5.3: implement part C** (append to `lib/vates.c`):

```c
/* ======================================================================
 * C. PROVISORES, MITTERE, RATIONARIUM, FICTUS
 * ====================================================================== */

#define VATES_URL_ANTHROPIC     "https://api.anthropic.com/v1/messages"
#define VATES_VERSIO_ANTHROPIC  "2023-06-01"

nomen structura {
       i32  status;
    chorda  corpus;
} VatesFictumResponsum;

structura Vates {
           Piscina* piscina;
            chorda  provisor;
         character* clavis;               /* NUL-terminata, UNA copia */
     VatesOptiones  optiones;
       HttpVectura  vectura;
         Herbarium* herbarium;
               b32  rationarium_defectus;
               Xar* fictus_responsa;      /* VatesFictumResponsum */
               i32  fictus_index;
               Xar* fictus_petitiones;    /* chorda */
               i32  fictus_numerus;
};

nomen Vates* (*VatesConstructor)(Piscina* piscina, chorda clavis,
                                 constans VatesOptiones* optiones);

nomen structura {
    constans character* titulus;
       VatesConstructor constructor;
} VatesProvisorInscriptus;

hic_manens constans character* constans _campi_anthropic[] = { "model", NIHIL };

interior chorda
_chorda_copia (chorda s, Piscina* piscina)
{
    chorda c;

    c.mensura = s.mensura;
    c.datum   = (i8*)piscina_allocare(piscina, (i64)s.mensura + I);
    si (s.mensura > 0)
    {
        memcpy(c.datum, s.datum, (size_t)s.mensura);
    }
    redde c;
}

/* larvare: copia herbarii (caput probatum non exportat) - Ruling T5 */
interior b32
_cifra (i8 c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_vocis (i8 c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || _cifra(c) || c == '_' || c == '-';
}

interior chorda
_larvare (chorda s, Piscina* piscina)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)s.mensura + I);
    i32 i = 0;

    dum (i < s.mensura)
    {
        si (_vocis(s.datum[i]))
        {
            i32 initium = i;
            b32 cifra   = FALSUM;

            dum (i < s.mensura && _vocis(s.datum[i]))
            {
                si (_cifra(s.datum[i]))
                {
                    cifra = VERUM;
                }
                i++;
            }
            si (cifra && i - initium >= XII)
            {
                chorda_aedificator_appendere_character(aed, '@');
            }
            alioquin
            {
                i32 k;
                b32 in_cifris = FALSUM;

                per (k = initium; k < i; k++)
                {
                    si (_cifra(s.datum[k]))
                    {
                        si (!in_cifris)
                        {
                            chorda_aedificator_appendere_character(aed, '#');
                        }
                        in_cifris = VERUM;
                    }
                    alioquin
                    {
                        chorda_aedificator_appendere_character(aed,
                            (character)s.datum[k]);
                        in_cifris = FALSUM;
                    }
                }
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_character(aed, (character)s.datum[i]);
            i++;
        }
    }
    redde chorda_aedificator_finire(aed);
}

interior chorda
_clavis_anthropic (
      HttpPetitio* petitio,
    HttpResponsum* responsum,
          Piscina* piscina,
           vacuum* datum)
{
    JsonResultus       j = json_legere(responsum->corpus, piscina);
    JsonValor*         e = j.successus ? json_objectum_capere(j.radix, "error") : NIHIL;
    ChordaAedificator* aed;
    character          numerus[XXXII];
    chorda             vacua = chorda_ex_literis("", piscina);

    si (!e)
    {
        redde herbarium_clavis_sceleti(petitio, responsum, piscina, datum);
    }
    aed = chorda_aedificator_creare(piscina, CXXVIII);
    sprintf(numerus, "%u:", responsum->status);
    chorda_aedificator_appendere_literis(aed, numerus);
    chorda_aedificator_appendere_chorda(aed, json_capere_chorda(e, "type", vacua));
    chorda_aedificator_appendere_character(aed, ':');
    chorda_aedificator_appendere_chorda(aed,
        _larvare(json_capere_chorda(e, "message", vacua), piscina));
    redde chorda_aedificator_finire(aed);
}

VatesOptiones
vates_optiones_ordinariae (vacuum)
{
    VatesOptiones o;

    memset(&o, 0, magnitudo(o));
    redde o;
}

interior Vates*
_anthropic_struere (
                   Piscina* piscina,
                    chorda  clavis,
    constans VatesOptiones* optiones,
       constans character* provisor)
{
    Vates* v;

    si (!piscina)
    {
        redde NIHIL;
    }
    v = (Vates*)piscina_allocare(piscina, (i64)magnitudo(Vates));
    memset(v, 0, magnitudo(*v));
    v->piscina  = piscina;
    v->provisor = chorda_ex_literis(provisor, piscina);
    v->clavis   = (character*)piscina_allocare(piscina, (i64)clavis.mensura + I);
    si (clavis.mensura > 0)
    {
        memcpy(v->clavis, clavis.datum, (size_t)clavis.mensura);
    }
    v->clavis[clavis.mensura] = '\0';
    v->optiones = optiones ? *optiones : vates_optiones_ordinariae();
    si (v->optiones.tempus_ms <= 0)
    {
        v->optiones.tempus_ms = DC * M;           /* X minuta */
    }
    si (v->optiones.conatus_maximi == 0)
    {
        v->optiones.conatus_maximi = III;
    }
    si (v->optiones.mora_iterandi_ms <= 0)
    {
        v->optiones.mora_iterandi_ms = II * M;
    }
    si (v->optiones.mora_iterandi_maxima_ms <= 0)
    {
        v->optiones.mora_iterandi_maxima_ms = LX * M;
    }
    v->vectura = v->optiones.vectura.exsequi ? v->optiones.vectura
                                             : http_vectura_ordinaria();
    redde v;
}

interior vacuum
_herbarium_adiungere (Vates* v)
{
    HerbariumOptiones ho;

    si (!v->optiones.herbarium_via)
    {
        redde;
    }
    ho = herbarium_optiones_ordinariae();
    ho.directorium      = v->optiones.herbarium_via;
    ho.clavis           = _clavis_anthropic;
    ho.campi_petitionis = _campi_anthropic;
    v->herbarium = herbarium_aperire(v->piscina, &ho);
    v->vectura   = herbarium_vectura(v->herbarium, v->vectura);
}

Vates*
vates_anthropic_aperire (
                   Piscina* piscina,
                    chorda  clavis,
    constans VatesOptiones* optiones)
{
    Vates* v = _anthropic_struere(piscina, clavis, optiones, "anthropic");

    si (v)
    {
        _herbarium_adiungere(v);
    }
    redde v;
}

interior HttpResultus
_fictus_exsequi (HttpPetitio* petitio, Piscina* piscina, vacuum* datum)
{
    Vates*                v = (Vates*)datum;
    HttpResultus          res;
    HttpResponsum*        resp;
    VatesFictumResponsum* f;

    *(chorda*)xar_addere(v->fictus_petitiones) =
        _chorda_copia(http_petitio_visus(petitio).corpus, v->piscina);
    memset(&res, 0, magnitudo(res));
    si (v->fictus_index >= xar_numerus(v->fictus_responsa))
    {
        res.error = HTTP_ERROR_CONNEXIO;
        res.error_descriptio = chorda_ex_literis("fictus exhaustum", piscina);
        redde res;
    }
    f = (VatesFictumResponsum*)xar_obtinere(v->fictus_responsa, v->fictus_index);
    v->fictus_index++;
    resp = (HttpResponsum*)piscina_allocare(piscina, (i64)magnitudo(HttpResponsum));
    memset(resp, 0, magnitudo(*resp));
    resp->status = f->status;
    resp->corpus = f->corpus;
    res.successus = VERUM;
    res.responsum = resp;
    redde res;
}

Vates*
vates_fictus_aperire (
                   Piscina* piscina,
    constans VatesOptiones* optiones)
{
    Vates* v = _anthropic_struere(piscina, chorda_ex_literis("fictus", piscina),
                                  optiones, "fictus");

    si (!v)
    {
        redde NIHIL;
    }
    v->fictus_responsa   = xar_creare(piscina, (i32)magnitudo(VatesFictumResponsum));
    v->fictus_petitiones = xar_creare(piscina, (i32)magnitudo(chorda));
    v->vectura.exsequi   = _fictus_exsequi;
    v->vectura.datum     = v;
    _herbarium_adiungere(v);
    redde v;
}

interior Vates*
_fictus_constructor (Piscina* piscina, chorda clavis, constans VatesOptiones* o)
{
    (vacuum)clavis;
    redde vates_fictus_aperire(piscina, o);
}

hic_manens constans VatesProvisorInscriptus _provisores[] = {
    { "anthropic", vates_anthropic_aperire },
    { "fictus",    _fictus_constructor },
    { NIHIL,       NIHIL }
};

Vates*
vates_aperire (
                   Piscina* piscina,
                    chorda  provisor,
                    chorda  clavis,
    constans VatesOptiones* optiones)
{
    i32 i;

    per (i = 0; _provisores[i].titulus; i++)
    {
        si (chorda_aequalis_literis(provisor, _provisores[i].titulus))
        {
            redde _provisores[i].constructor(piscina, clavis, optiones);
        }
    }
    fprintf(stderr, "vates: provisor ignotus '%.*s' - noti:",
            (integer)provisor.mensura, (constans character*)provisor.datum);
    per (i = 0; _provisores[i].titulus; i++)
    {
        fprintf(stderr, " %s", _provisores[i].titulus);
    }
    fprintf(stderr, "\n");
    redde NIHIL;
}

chorda
vates_provisor (Vates* vates)
{
    redde vates ? vates->provisor : _vacua();
}

constans character*
vates_error_descriptio (VatesError error)
{
    commutatio (error)
    {
        casus VATES_OK:            redde "OK";
        casus VATES_ERROR_RETE:    redde "rete (connexio aut TLS)";
        casus VATES_ERROR_TEMPUS:  redde "tempus excessum";
        casus VATES_ERROR_STATUS:  redde "status HTTP provisoris";
        casus VATES_ERROR_PARSE:   redde "responsum non legibile";
        casus VATES_ERROR_LIMES:   redde "limes localis (ante missionem)";
        ordinarius:                redde "error ignotus";
    }
}

/* ---- fictus: responsa in cauda ---- */

interior vacuum
_fictus_ponere (Vates* v, i32 status, chorda corpus)
{
    VatesFictumResponsum* f;

    si (!v || !v->fictus_responsa)
    {
        redde;
    }
    f = (VatesFictumResponsum*)xar_addere(v->fictus_responsa);
    f->status = status;
    f->corpus = corpus;
}

interior JsonValor*
_fictum_nuntium (Vates* v, constans character* causa_finis, JsonValor* content,
                 VatesUsus usus)
{
    Piscina*   p = v->piscina;
    JsonValor* r = json_objectum_creare(p);
    JsonValor* u = json_objectum_creare(p);
    JsonValor* cc = json_objectum_creare(p);
    character  id[LXIV];

    v->fictus_numerus++;
    sprintf(id, "msg_fictus_%u", v->fictus_numerus);
    json_objectum_ponere(r, "id", json_chorda_creare_literis(p, id));
    json_objectum_ponere(r, "type", json_chorda_creare_literis(p, "message"));
    json_objectum_ponere(r, "role", json_chorda_creare_literis(p, "assistant"));
    json_objectum_ponere(r, "model", json_chorda_creare_literis(p, "fictus"));
    json_objectum_ponere(r, "content", content);
    json_objectum_ponere(r, "stop_reason", json_chorda_creare_literis(p, causa_finis));
    json_objectum_ponere(r, "stop_sequence", json_nullum_creare(p));
    json_objectum_ponere(u, "input_tokens", json_integer_creare(p, usus.input));
    json_objectum_ponere(u, "cache_read_input_tokens",
                         json_integer_creare(p, usus.cache_lectum));
    json_objectum_ponere(u, "cache_creation_input_tokens", json_integer_creare(p,
        usus.cache_scriptum_5m + usus.cache_scriptum_1h));
    json_objectum_ponere(cc, "ephemeral_5m_input_tokens",
                         json_integer_creare(p, usus.cache_scriptum_5m));
    json_objectum_ponere(cc, "ephemeral_1h_input_tokens",
                         json_integer_creare(p, usus.cache_scriptum_1h));
    json_objectum_ponere(u, "cache_creation", cc);
    json_objectum_ponere(u, "output_tokens", json_integer_creare(p, usus.output));
    json_objectum_ponere(r, "usage", u);
    redde r;
}

vacuum
vates_fictus_textum (Vates* vates, chorda textus, VatesUsus usus)
{
    JsonValor* content;
    JsonValor* b;

    si (!vates || !vates->fictus_responsa)
    {
        redde;
    }
    content = json_tabulatum_creare(vates->piscina);
    b = json_objectum_creare(vates->piscina);
    json_objectum_ponere(b, "type", json_chorda_creare_literis(vates->piscina, "text"));
    json_objectum_ponere(b, "text", json_chorda_creare(vates->piscina, textus));
    json_tabulatum_addere(content, b);
    _fictus_ponere(vates, CC, json_scribere(
        _fictum_nuntium(vates, "end_turn", content, usus), vates->piscina));
}

vacuum
vates_fictus_instrumentum (
                  Vates* vates,
                  chorda id,
                  chorda titulus,
    constans character* input_json)
{
    JsonValor*   content;
    JsonValor*   b;
    JsonResultus input;
    VatesUsus    nullus;

    si (!vates || !vates->fictus_responsa)
    {
        redde;
    }
    memset(&nullus, 0, magnitudo(nullus));
    input = json_legere_literis(input_json ? input_json : "{}", vates->piscina);
    content = json_tabulatum_creare(vates->piscina);
    b = json_objectum_creare(vates->piscina);
    json_objectum_ponere(b, "type", json_chorda_creare_literis(vates->piscina, "tool_use"));
    json_objectum_ponere(b, "id", json_chorda_creare(vates->piscina, id));
    json_objectum_ponere(b, "name", json_chorda_creare(vates->piscina, titulus));
    json_objectum_ponere(b, "input", input.successus ? input.radix
                                     : json_objectum_creare(vates->piscina));
    json_tabulatum_addere(content, b);
    _fictus_ponere(vates, CC, json_scribere(
        _fictum_nuntium(vates, "tool_use", content, nullus), vates->piscina));
}

vacuum
vates_fictus_crudum (Vates* vates, i32 status_http, constans character* corpus_json)
{
    si (!vates || !corpus_json)
    {
        redde;
    }
    _fictus_ponere(vates, status_http, chorda_ex_literis(corpus_json, vates->piscina));
}

i32
vates_fictus_petitiones_numerus (Vates* vates)
{
    redde (vates && vates->fictus_petitiones) ? xar_numerus(vates->fictus_petitiones) : 0;
}

chorda
vates_fictus_petitio (Vates* vates, i32 index)
{
    si (!vates || !vates->fictus_petitiones
        || index >= xar_numerus(vates->fictus_petitiones))
    {
        redde _vacua();
    }
    redde *(chorda*)xar_obtinere(vates->fictus_petitiones, index);
}

/* ---- mittere ---- */

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

interior vacuum
_dormire (s64 ms)
{
    structura timespec pausa;

    si (ms <= 0)
    {
        redde;
    }
    pausa.tv_sec  = (time_t)(ms / M);
    pausa.tv_nsec = (longus)((ms % M) * M * M);
    (vacuum)nanosleep(&pausa, NIHIL);
}

interior b32
_status_iterandus (i32 status)
{
    redde status == CDVIII || status == CDIX || status == CDXXIX || status >= D;
}

interior s64
_mora_iterandi (constans Vates* v, HttpResultus hr)
{
    s64 mora = (s64)v->optiones.mora_iterandi_ms;

    si (hr.successus && hr.responsum)
    {
        chorda ra = http_responsum_caput(hr.responsum, "retry-after");
        s64    secunda;

        si (ra.mensura > 0 && chorda_ut_s64(ra, &secunda) && secunda >= 0)
        {
            mora = secunda * M;
        }
    }
    si (mora > (s64)v->optiones.mora_iterandi_maxima_ms)
    {
        mora = (s64)v->optiones.mora_iterandi_maxima_ms;
    }
    redde mora;
}

interior s64
_pretium (constans Vates* v, chorda exemplar, constans VatesUsus* u)
{
    i32 i;

    per (i = 0; v->optiones.pretia && i < v->optiones.pretia_numerus; i++)
    {
        constans VatesPretium* p = &v->optiones.pretia[i];

        si (chorda_aequalis_literis(exemplar, p->exemplar))
        {
            redde (u->input * p->input + u->output * p->output
                   + u->cache_lectum * p->cache_lectum
                   + u->cache_scriptum_5m * p->cache_5m
                   + u->cache_scriptum_1h * p->cache_1h) / ((s64)M * M);
        }
    }
    redde -I;
}

interior vacuum
_usus_addere (VatesUsus* summa, constans VatesUsus* u, b32* pretium_notum)
{
    summa->input             += u->input;
    summa->cache_lectum      += u->cache_lectum;
    summa->cache_scriptum_5m += u->cache_scriptum_5m;
    summa->cache_scriptum_1h += u->cache_scriptum_1h;
    summa->output            += u->output;
    summa->mora_ms           += u->mora_ms;
    si (u->pretium >= 0)
    {
        summa->pretium += u->pretium;
        *pretium_notum = VERUM;
    }
}

interior HttpPetitio*
_petitionem_http (constans Vates* v, constans VatesPetitio* pe, chorda corpus,
                  Piscina* piscina)
{
    HttpPetitio* hp = http_petitio_creare(piscina, HTTP_POST, VATES_URL_ANTHROPIC);
    i32          i;

    http_petitio_caput_addere(hp, "content-type", "application/json");
    http_petitio_caput_addere(hp, "x-api-key", v->clavis);
    http_petitio_caput_addere(hp, "anthropic-version", VATES_VERSIO_ANTHROPIC);
    per (i = 0; i < xar_numerus(pe->capita); i++)
    {
        VatesEffugium* e = (VatesEffugium*)xar_obtinere(pe->capita, i);

        si (chorda_aequalis_literis(e->provisor, "anthropic"))
        {
            http_petitio_caput_addere(hp, chorda_ut_cstr(e->clavis, piscina),
                                      chorda_ut_cstr(e->textus, piscina));
        }
    }
    http_petitio_corpus_ponere_chorda(hp, corpus);
    http_petitio_tempus_ponere(hp, v->optiones.tempus_ms);
    redde hp;
}

interior vacuum
_rationarium_scribere (
                  Vates* v,
    constans VatesPetitio* pe,
                 chorda  propositum,
                    i32  conatus,
    constans VatesResponsum* r,
               Piscina*  piscina)
{
    JsonValor* o;
    JsonValor* u;
    chorda     textus;
    chorda     linea;

    si (!v->optiones.rationarium_via)
    {
        redde;
    }
    o = json_objectum_creare(piscina);
    u = json_objectum_creare(piscina);
    json_objectum_ponere(o, "tempus",
        json_chorda_creare(piscina, fasti_ad_iso(fasti_nunc(), piscina)));
    json_objectum_ponere(o, "provisor", json_chorda_creare(piscina, v->provisor));
    json_objectum_ponere(o, "exemplar", json_chorda_creare(piscina, pe->exemplar));
    json_objectum_ponere(o, "propositum", json_chorda_creare(piscina, propositum));
    json_objectum_ponere(o, "conatus", json_integer_creare(piscina, (s64)conatus));
    json_objectum_ponere(o, "status", json_integer_creare(piscina, (s64)r->status_http));
    json_objectum_ponere(o, "causa_finis", json_chorda_creare(piscina, r->causa_finis_cruda));
    si (r->error != VATES_OK)
    {
        json_objectum_ponere(o, "error", json_chorda_creare_literis(piscina,
            vates_error_descriptio(r->error)));
        json_objectum_ponere(o, "error_genus", json_chorda_creare(piscina, r->error_genus));
    }
    json_objectum_ponere(u, "input", json_integer_creare(piscina, r->usus.input));
    json_objectum_ponere(u, "cache_lectum", json_integer_creare(piscina, r->usus.cache_lectum));
    json_objectum_ponere(u, "cache_5m", json_integer_creare(piscina, r->usus.cache_scriptum_5m));
    json_objectum_ponere(u, "cache_1h", json_integer_creare(piscina, r->usus.cache_scriptum_1h));
    json_objectum_ponere(u, "output", json_integer_creare(piscina, r->usus.output));
    json_objectum_ponere(o, "usus", u);
    json_objectum_ponere(o, "pretium", json_integer_creare(piscina, r->usus.pretium));
    json_objectum_ponere(o, "mora_ms", json_integer_creare(piscina, r->usus.mora_ms));
    json_objectum_ponere(o, "id", json_chorda_creare(piscina, r->id));
    textus = json_scribere(o, piscina);
    linea.mensura = textus.mensura + I;
    linea.datum = (i8*)piscina_allocare(piscina, (i64)linea.mensura);
    memcpy(linea.datum, textus.datum, (size_t)textus.mensura);
    linea.datum[textus.mensura] = '\n';
    si (!filum_appendere_firmiter(v->optiones.rationarium_via, linea)
        && !v->rationarium_defectus)
    {
        fprintf(stderr, "vates: rationarium scribi non potest: %s\n",
                v->optiones.rationarium_via);
        v->rationarium_defectus = VERUM;
    }
}

VatesResponsum*
vates_mittere (
            Vates* vates,
     VatesPetitio* petitio,
           chorda  propositum,
          Piscina* piscina)
{
    VatesResponsum* r = NIHIL;
    VatesUsus       summa;
    chorda          corpus;
    i32             conatus;
    b32             pretium_notum = FALSUM;

    si (!piscina)
    {
        redde NIHIL;
    }
    si (!vates || !petitio)
    {
        r = _responsum_vacuum(piscina);
        r->error = VATES_ERROR_LIMES;
        r->error_nuntius = chorda_ex_literis("vates aut petitio NIHIL", piscina);
        redde r;
    }
    si (_signa_numerare(petitio) > VATES_PUNCTA_THESAURI_MAXIMA)
    {
        r = _responsum_vacuum(piscina);
        r->error = VATES_ERROR_LIMES;
        r->error_nuntius = chorda_ex_literis(
            "plus quam IV puncta cache (API: IV maxime)", piscina);
        redde r;
    }
    memset(&summa, 0, magnitudo(summa));
    corpus = vates_anthropic_corpus(petitio, piscina);
    per (conatus = I; ; conatus++)
    {
        HttpPetitio* hp = _petitionem_http(vates, petitio, corpus, piscina);
        s64          initium = _ms_nunc();
        HttpResultus hr = http_vectura_exsequi(vates->vectura, hp, piscina);
        b32          iterandum;

        si (!hr.successus || !hr.responsum)
        {
            r = _responsum_vacuum(piscina);
            r->error = (hr.error == HTTP_ERROR_TIMEOUT) ? VATES_ERROR_TEMPUS
                                                        : VATES_ERROR_RETE;
            r->error_nuntius = hr.error_descriptio;
            iterandum = (r->error == VATES_ERROR_RETE) || (conatus == I);
        }
        alioquin
        {
            chorda novitas;

            r = _legere(hr.responsum->status, hr.responsum->corpus, piscina, &novitas);
            si (novitas.mensura > 0 && vates->herbarium)
            {
                herbarium_premere(vates->herbarium, hp, hr.responsum, novitas);
            }
            iterandum = _status_iterandus(r->status_http);
        }
        r->usus.mora_ms = _ms_nunc() - initium;
        r->usus.pretium = _pretium(vates,
            r->exemplar.mensura > 0 ? r->exemplar : petitio->exemplar, &r->usus);
        _rationarium_scribere(vates, petitio, propositum, conatus, r, piscina);
        _usus_addere(&summa, &r->usus, &pretium_notum);
        si (!iterandum || conatus >= vates->optiones.conatus_maximi)
        {
            frange;
        }
        _dormire(_mora_iterandi(vates, hr));
    }
    r->usus = summa;
    si (!pretium_notum)
    {
        r->usus.pretium = -I;
    }
    r->conatus = conatus;
    redde r;
}
```

- [ ] **Step 5.4: green** - `./compile_tests.sh probatio_vates` PASS.
- [ ] **Step 5.5: plants** - (1) `_rationarium_scribere` writes `v->clavis`
  as a field -> clavis test red; (2) `retry-after` uncapped -> retry
  test red (or slow: the assertion `< II` s); (3) TEMPUS retried every
  attempt -> conatus III red; (4) NO plant for usage summation: error attempts
  carry no token usage, so "sum" and "last only" differ only in
  `mora_ms` (timing-dependent) - ledger this as a Ruling instead of a
  flaky plant; (5) foreign-provider header sent -> `openai-beta`
  red; (6) novelty not pressed -> herbarium causa red.
- [ ] **Step 5.6: worklog + commit.** Gates `radix`, `generata`.

---

### Task 6: committed specimens + the sweep

**Files:** Create `probationes/fixa/vates/herbarium/specimina/*.json`
(seeds), modify `probationes/probatio_vates.c`.

- [ ] **Step 6.1: seeds** (hand-made in herbarium's exact format; one
  file per kind; names `semen_<kind>-1.json`): overloaded 529,
  rate_limit 429 (with `retry-after`), invalid_request 400,
  authentication 401, api_error 500, refusal 200, max_tokens 200,
  unknown block 200. Example (`semen_overloaded-1.json`):

```json
{
  "clavis": "529:overloaded_error:Overloaded",
  "causa": "status",
  "status": 529,
  "capita": [{"titulus": "content-type", "valor": "application/json"}],
  "corpus": "{\"type\":\"error\",\"error\":{\"type\":\"overloaded_error\",\"message\":\"Overloaded\"},\"request_id\":\"req_011CQ\"}",
  "primum_visum": "2026-10-08T00:00:00Z",
  "methodus": "POST",
  "hospes": "api.anthropic.com",
  "via": "/v1/messages",
  "summarium": {"model": "claude-sonnet-5-5"}
}
```

  The other seven follow the same shape; their `corpus` values are the
  bodies used in T4's tests (error bodies with the matching
  `error.type`; the 200 bodies from the refusal, max_tokens and
  server_tool_use cases).

- [ ] **Step 6.2: failing sweep test**, called under `/* T6 */`:

```c
interior vacuum
probatio_specimina_commissa(Piscina* piscina)
{
    Xar*        s = herbarium_enumerare(piscina, "probationes/fixa/vates/herbarium");
    HttpVectura reddens;
    i32         i;

    imprimere("\n--- Probans specimina commissa (exitus NOMINATUS) ---\n");
    /* praesentia > 0, non numerus fixus: semina crescunt */
    CREDO_MAIOR_I32(xar_numerus(s), 0);
    reddens = herbarium_reddens(piscina, s);
    per (i = 0; i < xar_numerus(s); i++)
    {
        HerbariumSpecimen* sp = (HerbariumSpecimen*)xar_obtinere(s, i);
        HttpResultus       hr = http_vectura_exsequi(reddens, NIHIL, piscina);
        VatesResponsum*    r = vates_anthropic_legere(hr.responsum->status,
                                                      hr.responsum->corpus, piscina);

        imprimere("  %.*s\n", (integer)sp->sigillum.mensura,
                  (constans character*)sp->sigillum.datum);
        si (sp->status != CC)
        {
            CREDO_VERUM(r->error == VATES_ERROR_STATUS);
            CREDO_CHORDA_NON_VACUA(r->error_genus);
        }
        alioquin
        {
            CREDO_VERUM(r->successus);
            CREDO_VERUM(r->causa_finis != VATES_FINIS_ALIA);
        }
    }
}
```

- [ ] **Step 6.3: green; plant** - a seed whose 200 body has
  `"stop_reason":"novum_finis"` -> red (ALIA = unnamed); remove it.
  This is the payoff contract: a real specimen the parser cannot name
  turns the suite red.
- [ ] **Step 6.4: commit** (seeds + test). Gates `radix`.

---

### Task 7: live smoke - one call, cache read, thinking round-trip

**Files:** Create `tools/vates_fumus.c`, `tools/vates_fumus.sh`.

`vates_fumus` (hand-run, never a gate, costs cents) reads
`~/.rhubarb/anthropic.clavis` exactly as `rete_fumus` does, then:

1. `vates_aperire("anthropic")` with `rationarium_via =
   build/vates_fumus.rationarium.jsonl`, `herbarium_via =
   build/vates_fumus_herbarium`, prices for `claude-sonnet-5-5`
   (input 2,000,000; output 10,000,000; cache_lectum 200,000; cache_5m
   2,500,000; cache_1h 4,000,000 micro-dollars per MTok - the skill's
   2026-10-06 table; Sonnet 5.5 cache-write rates assumed 1.25x / 2x
   of input).
2. Call A: system prompt of >= 600 tokens (a fixed Latin passage,
   repeated), `signum_thesauri` on it, user "Responde uno verbo: salve".
3. Call B: the identical request. Expect `B.usus.cache_lectum > 0`
   (BENE) - the cache proof.
4. Call C: `cogitationem_monstrare`, effort `high`, a small reasoning
   question; if the response holds an OPACUM thinking block, append it
   (`vates_responsum_addere`) + a user turn and call D; D must succeed
   (the signature survived re-emission verbatim). No thinking block ->
   NOTA (not FRACT): "nulla cogitatio reddita - reditus non probatus".
5. Print a table: per call status, causa_finis, usus, pretium; ledger
   line count == calls made.

Model default `claude-sonnet-5-5` (the compactor; cheap enough),
override `-exemplar <id>`.

- [ ] Write it in the style of `tools/rete_fumus.c` (same `_nuntiare`
  helper; key buffer zeroed after `vates_aperire`; the `Vates` holds
  the only copy). Build/run like `rete_fumus.sh`.
- [ ] Run; Fran runs the key-leak grep over stdout, the ledger and the
  herbarium directory (a `build/clavis_probare_vates.sh` like plan 1).
- [ ] Record the numbers (tokens, cache read, cost per call) in
  `lib/vates.worklog.md`; commit the tool. Gates: none owed (tool not
  in a suite) besides `oratio` if glossary entries are added.

---

### Task 7b: teach vates the live response shape (added 2026-10-08 from T7, Fran's ruling)

T7's live run pressed specimens on every 200: the API returns top-level
`container` and `diagnostics` (not in `_claves_summae`) and usage extras
`output_tokens_details`, `service_tier`, `inference_geo` (usage keys are
not novelty-checked).

- [ ] Failing test: a T4-style parse of a body carrying `container`
  and `diagnostics` must report NO novelty - expose via the herbarium
  path: fictus + `herbarium_via`, a 200 body with both keys -> zero
  specimens pressed (today: one).
- [ ] Add `"container"`, `"diagnostics"` to `_claves_summae`.
- [ ] Promote one captured live specimen (Fran reads it first: it holds a
  real response text + thinking signature, no request content) into
  `probationes/fixa/vates/herbarium/specimina/` under a descriptive
  name; the sweep test then covers the REAL shape.
- [ ] Worklog: the usage extras are known-but-unmodelled (output_tokens
  already includes thinking; service_tier/inference_geo are reporting).
- [ ] Plant: remove "diagnostics" from the list -> the new test red.
- [ ] Commit; gates `radix`, `generata`.

### Task 8: integrate and merge (STOP for Fran before the merge)

- [ ] Merge main into `rhubarb-quarta` (no-ff "Fusio main in
  rhubarb-quarta ..."); rebuild fabrica -> aedilis -> compilator;
  `./tools/compile_tests_fontes_generare.sh -probare`.
- [ ] `./tools/frigida_probare.sh -ref HEAD` (fresh worktree, all
  suites, ~13 min); any red suite compared against
  `-ref main -suitae X` (pre-existing red = proceed, named).
- [ ] **STOP: ask Fran** to approve the fast-forward of main (an
  outward side effect on the shared branch).
- [ ] `git -C ../rhubarb merge --ff-only rhubarb-quarta` (refuses if
  main moved: then merge main in again and repeat the frigida run).
- [ ] Ledger: parcum …SQM6 note (what merged, commits); memory
  `optchat-project` progress line; next = optchat plan (O0-O5).
