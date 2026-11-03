> Raw analyst report, batch I of budget B2 (2026-10-31), filed verbatim as evidence for ../budget-fusionis.md.

# Batch I — foundation libraries, second pass (piscina, chorda, aedificator, xar, tabula_dispersa, internamentum, stml, credo, filum/lectiones)

Analyst: I (Opus 5.5), 2026-10-30/31. Nothing in the repo was changed except this file. All
prototypes, profiles and benches are in `scratchpad/b2/I/` (session scratch: it will not survive
the session). Load average was 6–19 on 8 cores the whole time, with other agents running. Every
A/B below ran both variants back to back, and I report **instructions retired** (`/usr/bin/time -l`)
next to wall time wherever I could. Instruction counts barely moved between runs (±0.5%). Wall
time did not.

## How the run budget looks now (this changes where minutes are)

The last fusio (`mensor quaestio fusio`, quinta 20f603f1) took 23.7 min, and **21.0 min of it was
frigida**. Thanks to E1 receipts, the gates themselves were mostly 0.0 min. So a per-run saving
counts roughly once in frigida, plus once more in the merge commit's own gates when the suite is
touched. I use **1.5 runs per merge** below (2 for things that also run in every commit).

frigida children (same run):

| child | time |
|---|---|
| radix | 455 s |
| crusta | 432 s (outlier: usually 45–70 s) |
| silva | 213 s (canon_corpus 40, obsoleta 40, lint 30, inutilia 29, plagula 29) |
| briar | 178 s |
| oratio | 96 s |
| saltuarius | 56 s |
| md | 51 s (md_stml 28) |
| toml | 32 s (totalitas 12) |

## Method

- **Profiles:** `sample` at 1 ms over five heavy binaries from `build/fabrica/area`, run with
  `RHUBARB_RADIX` set and `FABRICA_LECTIONES` unset. That covers probatio_silva_canon_corpus,
  _exemplaria_obsoleta and _arbor_plagula, probatio_md_stml, and probatio_toml_totalitas. I summed
  self time per function over the "Sort by top of stack" section, leaving out idle frames
  (read/wait/semwait), and added a caller and child tree walker (`selfsum.py`, `callers.py`,
  `children.py`).
- **Fork/sleep counts:** a `DYLD_INSERT_LIBRARIES` interposer counted forks and nanosleep time
  (`interpose/cnt.dylib`).
- **Prototypes:** relinked copies, built by swapping one object in a test's `nexus.log` link line,
  or in `bin/aedilis`'s object list. Two exceptions, both described under idea 4: a full scratch
  rebuild of canon_corpus (all 50 TUs), and a scratch mirror of the tree for the amalgamators.

What the profiles say, by foundation function. Average % of busy samples over the 4 non-fork
profiles, then the range:

- xar_obtinere: 7.3%, 2.5–13.9
- tabula_dispersa_invenire: 6.2%
- friatio_fnv1a: 3.9%
- `_lineae_excedunt`: 3.0%, up to 5.7 in md_stml
- tabula_dispersa_inserere: 2.9%
- xar_addere: 2.8%
- xar_numerus: 2.1%, 5.0 in canon
- memmove: 2.0%
- piscina_allocare: 1.8%
- chorda_aequalis_literis: 1.4%
- invenire_literis: 1.3%
- piscina_allocare_ordinatum: 1.1%
- memset: 1.0%, of which 81–97% comes from `allocare_segmentum`'s zeroing in xar

toml_totalitas is another story entirely: 24.6% `__fork` and 26% xzone-malloc fork locks, and
under that half its wall time is **sleeping in credo** (idea 3).

**The biggest finds are not in the self-time table.** The profiles ran without
`FABRICA_LECTIONES`, and under fabrica every file probe also costs a `write()` to the book. That
cost only showed up when I read the books themselves (idea 1).

---

## 1. aedilis probes every include through ~37 directories again and again, and every probe writes a book line — memo the existence test — est. **0.4–0.7 min/merge**

- **Where:** `lib/aedilis.c:_existit_sub_radice` (L91). It is called by `_directivam_resolvere`
  (rogata, includer-relative, then each `-I` directory in order), `_obiectum_probare`, and L760,
  L836 and L1200. It ends in `via_existit` (`lib/via.c:482`): a `stat()` plus
  `lectiones_notare(EXSTAT/ABSENS)`.
- **Evidence:** the book sizes in `build/fabrica/lectiones/`:

  | book | lines | unique | size |
  |---|---|---|---|
  | `porta_aedilis.tsv` | 3,803,871 (3.11 M of them `A`) | 17,887 | 123 MB |
  | `fragmentum_compile_tests.tsv` | 1,657,994 | 12,740 | 45 MB |
  | canon_corpus | 730,248 | 10,507 | 20 MB |
  | lint | 730,252 | 10,507 | 20 MB |
  | plagula | 730,253 | 10,507 | 20 MB |

  The three silva books come from their `bin/aedilis --corpus lib` child, which inherits
  `FABRICA_LECTIONES`.

  A typical stretch of a book: `A ./string.h`, `A ./lib/string.h`, `A ./include/string.h`, …
  `A ./html/fontes/string.h`. Every system header is resolved against all `-I` roots, for every
  source that includes it. `visa_systematum` is only consulted after resolution, and in `--corpus`
  mode this repeats for each of the 156 scopes.

  Measured, `bin/aedilis --corpus lib --partes` with a warm thesaurus, 3 runs each:

  | setup | instructions | wall | sys |
  |---|---|---|---|
  | no book | 9.2–9.4 G | 3.0–3.2 s | 1.4 s |
  | with book | 28.0–28.3 G | 5.1–5.4 s | 3.4 s |

  So the book alone costs 2 s of sys and 19 G instructions per call (≈ 2.9 µs per line). The
  remaining sys time is the ~720k `stat`s themselves.
- **Mechanism:** a per-process cache, `via → exstat`, in front of `via_existit`: a
  `TabulaDispersa` over a private piscina, filled on the first probe of each path. The first
  probe still calls `stat` and still writes its book line, so fabrica sees exactly the same set
  in the same first-occurrence order. Later probes of the same path return the cached answer.
  - **Premise:** the source tree does not change during one aedilis process. The extractor memo
    already assumes exactly that ("Memoria per cursum", "arbor gelata"), and every probed path
    is a source or header path. Nothing aedilis writes (struere.sh, objects) is ever probed by
    this function.
- **Measured** (scratch `aed/aed_memo`, relinked from build/aedilis/*.o with only aedilis.o
  swapped). Warm scratch thesaurus, book on, same run, load 12–16:

  | variant | instructions | wall | book lines |
  |---|---|---|---|
  | base | 28.2–28.5 G | 6.0–7.6 s | 720,853 |
  | memo | 4.6–4.7 G (**−83%**) | 2.7–3.6 s | 15,977 |

  - stdout is byte-identical (`cmp`).
  - Normalising the pid in the oracle temp file, the books' first-occurrence sequences are
    **identical** (9,163 entries each). That is exactly the key fabrica's `_lectiones_colligere`
    dedupes on (`visae`, letter + via, first wins).
- **Estimate:**
  - Silva suites: 3 `--corpus` calls per silva run × ~3.5 s ≈ 10 s per run.
  - porta_aedilis: about 600 per-file invocations × 5 kinds; single-file books lose ~68% of their
    lines (below). That is ~2.6 M fewer lines × ~5 µs ≈ 13 s per run.
  - fragmentum_compile_tests: one `aedilis --enumerare --plures` process, 1.66 M lines down to
    ~13k ≈ 8 s per run, and it runs in commits as well as in frigida.
  - fabrica parses smaller books: a harness of the `_lectiones_colligere` per-line work
    (`fab/harn`) shows 0.51 s CPU and an **848 MB piscina** for the porta_aedilis book, and
    0.10 s / 169 MB for a silva book.
  - Total: 30–45 s per merge.
- **API:** none. One static table in aedilis.c. **Risk:** low (the premise above). **Effort:** S
  (~35 lines).
- **Verify:**
  - aedilis gate, silva corpus suites, fabrica suites.
  - `cmp` the `--partes`/`--aristae` outputs.
  - Diff the first-occurrence sets of a book.
- **Plant:** cache existence under the *un-joined* path (`via`, not radix+via). Two different
  includers then collide, and a `-I` candidate resolves wrongly, so aedilis differentia and
  porta clausura go red. Not run: I only designed this plant, so the plant is owed before
  landing.

## 2. lectiones writes the same line again and again — per-process dedupe in `lectiones_notare` — est. **0.1–0.3 min/merge** (on top of idea 1; it also catches every other prober)

- **Where:** `lib/lectiones.c:_lectionem_notare`. Every call does `getenv` + `strcmp` + one
  `write()` syscall.
- **Evidence:** the same books. In single-file aedilis (`probatio_xar.c`, `probatio_stml.c`,
  `probatio_cellae.c`), lines vs unique are 1348/724, 1765/814 and 2505/1000. Duplicates come
  from more than include probes (L lines of re-read files, X of `latina.h`, …). fabrica treats
  the book as a set.
- **Mechanism:** a direct-mapped table of 16,384 `character*` slots in lectiones.c, keyed by
  FNV-1a of the line (letter + tab + via). Same line already in its slot → skip the write.
  - Collisions only rewrite a line, which is harmless; a new line is never dropped.
  - The table is cleared whenever the descriptor is closed or reopened (env unset or changed).
    That matches the existing "absent = CLOSED" rule and the deleted-book trap from the
    2026-10-27 worklog.
  - After `fork`, the child inherits the table. That is correct: everything the parent marked
    was written to the shared `O_APPEND` descriptor.
  - Only libc is used (malloc/free/memcmp), so lectiones keeps zero house dependencies.
  - Memory is bounded at 16k slots plus the line copies.
- **Measured** (`lect/aed_dedup`, only lectiones.o swapped), `--corpus lib`:

  | variant | instructions | wall | book lines |
  |---|---|---|---|
  | base | 28.5–29.2 G | 6.0–7.5 s | 720,853 |
  | dedupe | 10.7 G | 3.5–3.95 s | 28,901 |
  | dedupe + memo (idea 1) | 4.5–4.6 G | 2.6–3.35 s | 9,309 |

  - stdout is identical.
  - The first-occurrence sequence is identical to base (pid-normalised).
  - On 48 single-file invocations, base 182,412 lines → 59,766 with dedupe, 57,091 with both.
- **Estimate:** after idea 1 this mostly removes the remaining L/X repeats in per-file runs and in
  any other probing tool (compilator, fabrica_sutura readers, …): ~0.1–0.3 min. If idea 1 is
  refused, it alone takes ~60–70% of idea 1's book cost (the stats stay).
- **API:** none. **Risk:** low. **Effort:** S (~55 lines).
- **Test change needed (flag):** `probatio_lectiones` section III ("filii duo paralleli: lineae
  integrae") has two children each write the *same* path 500 times, and it asserts 1000 lines.
  With dedupe it sees 2.
  - Fix: make the path vary per iteration (append `j`). The assertion's real purpose, untorn
    `O_APPEND` lines under concurrency, stays intact.
  - Also run the fabrica suites, in case some test counts lines of a synthetic book.
- **Plant:** compare only the first `n-1` bytes. "L x.c" and "L x.h"-style pairs collide, and a
  new line is dropped. Add a test: two different paths sharing one slot must both appear. The
  slot can be forced by planting `LECTIONES_MEMORIA_SLOTI` = 1.

## 3. credo polls its forked child with fixed 1 ms sleeps — exponential backoff from 16 µs — est. **0.15–0.25 min/merge**

- **Where:** `lib/credo.c:credo_processus_iudicare` (L1331–1388): `waitpid(WNOHANG)`, then
  `nanosleep(1 ms)`, in a loop. Every `CREDO_NON_RUIT` / `NON_PENDET` / `RUIT_CUM` therefore
  waits at least one full sleep, about 1.3 ms in practice, even when the child exits in 100 µs.
- **Evidence:** interposer counts per run, with frigida wall times for context:

  | suite | forks | time asleep | wall |
  |---|---|---|---|
  | toml_totalitas | 6,159 | **8.8 s** | 14.0 s |
  | crusta_totalitas | 2,953 | 4.8 s | 7.2 s |
  | html_totalitas | 455 | 1.0 s | 1.7 s |
  | oratio_totalitas | 192 | 0.46 s | |
  | css_totalitas | 243 | 0.32 s | |
  | md_totalitas | 190 | 0.25 s | |
  | briar_totalitas | 190 | 0.26 s | |
  | stml_incolumitas | 32 | 0.04 s | |

  In the toml profile, `nanosleep` under `credo_processus_iudicare` holds 4,983 of the 9,762
  samples in `_corpus_vexare`.
- **Mechanism:** `pausa_us` starts at 16 and doubles up to 1000. Elapsed time accumulates the
  *nominal* pauses in µs (`i64`), and the timeout fires when `elapsum_us >= mora_ms × 1000`. That
  is the same "sum of requested sleeps" semantics as today, ±1 ms. ~15 changed lines, C89, no
  API change.
- **Measured** (`credo/toml_tot_{base,nova}`, credo.o swapped), 3 alternating runs, load 6–8:

  | variant | wall | Totalis / Fracti |
  |---|---|---|
  | base | 12.2–12.9 s | 12,358 / 0 |
  | backoff | 7.8–8.4 s (**−35%**) | 12,358 / 0 |

  - After the change the parent sleeps 6.1 s over 28k short naps. That is the children's real
    lifetime (fork, work, exit), so there is nothing left to squeeze without changing the test
    design.
  - probatio_credo_processus (300 ms SIGKILL path included) and probatio_stml_incolumitas: green,
    same counts.
- **Estimate:** toml −4.4 s, crusta about −2.5 s, html −0.5 s, the others −0.5 s together ≈ 8 s per
  run × 1–2 runs.
- **API:** none. **Risk:** low. **Effort:** S.
- **Coverage gap (found while designing the plant):**
  - Nothing pins that a child which takes longer than the first pauses but less than `mora`
    survives. A units plant (`elapsum_us >= mora_ms`, i.e. a timeout 1000× too short) would
    probably pass every suite: the totalitas children finish in under 5 ms against `mora` 5000.
  - Add to probatio_credo_processus: `CREDO_NON_PENDET(child that sleeps 50 ms, 2000)` must
    pass, and the existing 300 ms SIGKILL case must take ≥ 250 ms.
  - Plant: the units bug above. It must go red at the new 50 ms case.

## 4. xar: header fast path for `xar_obtinere` / `xar_numerus` as GNU `extern __inline__` (`gnu_inline`) — the amalgam-safe alternative to C1 — est. **0.25–0.4 min/merge**

- **Why C1 broke, and why this does not:**
  - The silva amalgamator emits every non-include directive verbatim, renames prefixes, and
    injects `static` before vendored functions whose storage class it cannot see. That is S44
    in `silva/instrumenta/silva_amalgama.c` L479–491, and it applies to header plagulas too: the
    committed silva.c has `static i32 silva_xar_numerus(...)` prototypes.
  - C1's `_xar_*_inl` helpers plus same-name macros therefore ended up live inside the amalgam.
  - This variant has no helpers and no macros. The block is guarded so that it is **dead code in
    every amalgam**:
    `#if defined(__GNUC__) && !defined(XAR_SINE_INLINEA) && !defined(SILVA_H) && !defined(OFFICINA_H) && !defined(XAR_SINE_INTRINSECIS)`
    (silva.c defines `SILVA_H` at L37; officina.c includes silva.h before the xar.h text, and
    defines `OFFICINA_H` at L44).
  - The amalgamator still mangles the dead text into `static extern __inline__ … silva_xar_obtinere`,
    but the preprocessor drops it before the compiler ever sees it.
  - lib/xar.c gains `#define XAR_SINE_INLINEA` before its first include, so the real definitions
    never see an inline predecessor. Without it, clang errors with `-Wstatic-in-inline`, because
    it merges `inline` into the later definition.
- **Tested against the real amalgamation step** (scratch tree `radix/`: symlinks to the repo
  with include/xar.h and lib/xar.c replaced):
  - `silva/build/amalgamator <repo>` reproduces the committed `silva/amalgama/silva.c`
    byte-for-byte (control). On the scratch tree it succeeds, and the new silva.c compiles
    standalone under the full -Werror flag set.
  - `officina/build/amalgamator` likewise reproduces the committed officina.c. On the scratch
    tree it compiles too, with `-I silva/amalgama`.
  - The amalgam diff is the dead block, plus xar.c's leading comment, which is now kept because
    the `#define` precedes the first include. That is cosmetic. Amalgams need regenerating anyway.
- **Mechanism:** same names, same signatures, so no symbol is emitted (gnu_inline). `&xar_obtinere`
  and calls from non-GNU compilers or `-O0` still hit the real function in xar.c. The inline
  covers:
  - NULL and bounds checks;
  - the first segment;
  - power-of-two first sizes, via `__builtin_clz`;
  - the non-power-of-two closed form (`q = index / primum`), taken from `xar_locare`;
  - `k >= mensura_tabulae → NIHIL` (identical: segmenta_maxima ≥ mensura_tabulae, so both
    original refusals return NIHIL).
- **Trap found (documented in the comment):** my first draft fell back to
  `(xar_obtinere)(xar, index)` for non-power-of-two first sizes. Under gnu_inline a call to the
  function's own name binds to *this body*, and tail-call elimination turned it into an
  **infinite loop**. probatio_xar hung in "fines segmentorum" (first sizes 3 and 7). The final
  version has no self-call. Any future edit must keep it that way.
- **Measured:**
  - Microbench `tools/xar_mensura.c`, best of 5, same run:

    | case | base | inline |
    |---|---|---|
    | seriatim | 4.2 ns | 1.6 ns |
    | lexemata | 4.35 ns | 1.6 ns |
    | temere | 3.4 ns | 1.7 ns |

    That beats C1's 2.1 ns, because both obtinere and numerus fold into the loop.
  - **Whole suite:** canon_corpus rebuilt in scratch, all 50 TUs. Headers must come from a full
    include/ mirror: quote-includes inside include/*.h find the repo's xar.h first. My first,
    partial build missed that and showed only −1.3%.

    | variant | instructions | cycles | xar_obtinere / xar_numerus call sites |
    |---|---|---|---|
    | base | 378.2 / 377.6 G | 142 / 129 G | 624 / 823 |
    | inline | 341.6 / 342.3 G (**−9.5%**) | 106 / 120 G (−7…−25%, load-noisy) | 20 / 0 |

    Fracti 0 in both.
  - Microbench and canon numbers are at load 8–18.
- **Estimate:** canon −9.5% (xar self 18.9%). Scaling by each suite's xar share: obsoleta ~5.5%,
  lint/inutilia ~6%, plagula ~4.5%, md_stml ~2.7%. That is ≈ 12 s per frigida silva+md, plus a few
  seconds across radix ≈ 15–25 s per merge.
- **API:** none at source level. Two definitions of each name now exist (header-inline and
  xar.c).
- **Tooling to check:**
  - examen: `__inline__` / `__attribute__((__gnu_inline__))` / `__builtin_clz` in a header. I
    added a `<tolera codex="EXTENSIO_COMPILATORIS">` line like piscina's, but whether one tolera
    covers the block is unverified.
  - legati `symbolum`/`vocantes` will show two definition sites.
  - silva semantic analysis parses the header copy.
  - `canon_corpus`'s "no zero-token macro invocation" assertion is unaffected: there are no
    macros.
- **Effort:** S–M (~70 header lines + 1 line in xar.c + regenerated amalgams + tooling checks).
- **Verify:**
  - probatio_xar against the prototype: 1310/1310.
  - Differential harness (`xar/dbg2`): 3.46 M index checks, inline vs `(xar_obtinere)`, over first
    sizes 1, 2, 3, 4, 5, 7, 8, 16 and 64 × ordinary/magnum, plus out-of-range and 0xFFFFFFFF
    indices and NIHIL: **0 mismatches**.
- **Plants:**
  - `+ I` on the power-of-two offset: 199 FRACTA (probatio_xar.c:195).
  - `+ I` on the non-power-of-two offset: "fines segmentorum (forma clausa)" FRACTA 23/25.
- **Also run:** probatio_xar with `-DXAR_SINE_INLINEA`, so the old path stays tested.
- **Not done:** xar_addere. Its slow path needs allocare_segmentum (static), so an inline version
  would need a new public slow-path symbol, which is an API addition.

## 5. `_lineae_excedunt` counts newlines byte by byte — use memchr — est. **0.05–0.1 min/merge**

- **Where:** `lib/stml.c:_lineae_excedunt` (L5191, the B5 early-exit replacement). It still holds
  2.3–5.7% self (md_stml 5.7, plagula 4.3), because it runs per pretty-printed block element over
  its whole content: O(content × depth) for subtrees under 30 lines.
- **Mechanism:** `memchr(p, '\n', end - p)` per newline, with the same `> limes` early exit. Same
  result.
- **Measured** (`lin/lb`, synthetic pretty STML with 0–60-space indentation and short lines):

  | content per element | byte loop | memchr |
  |---|---|---|
  | 4000-byte spans | 0.147 s | 0.052 s (2.8x) |
  | 600-byte spans | 0.13 s | 0.056 s (2.3x) |

- **Estimate:** ~60% of 3% on the writer-heavy suites (md_stml, plagula, lint, obsoleta, canon,
  inutilia) ≈ 3–6 s per merge.
- **API:** none. **Risk:** very low. **Effort:** S.
- **Plant:** count the newline at `p` twice (no `p++`). The 30/31-line goldens in
  probatio_stml_pulchrum (added with B5) flip.

## 6. Smaller, cheap if someone is in the file (≤ 0.05 min each)

- **xar segment zeroing** (`allocare_segmentum` L171): 81–97% of all `memset` samples (≈1% of
  busy) come from zeroing 4–16-element segments through the libc call. A word-store loop for
  segments ≤ 64 bytes, falling back to memset above that, saves maybe half of that. Allocation
  counts are unchanged, so computus golds are safe. Plant: zero one word short; the "vexilla"
  section (ORDINARIUS zeroing) catches it.
- **`stml_liberum_ad_indicem`** (cross-TU accessor, 14% of xar_obtinere's callers in canon): idea
  4 already removes the inner call. Inlining the accessor itself would need the same header
  treatment for stml.h. Revisit after idea 4.

---

## Summary table

| # | idea | est. min/merge | effort | API / golds |
|---|---|---|---|---|
| 1 | aedilis existence memo | 0.4–0.7 | S | none / none |
| 2 | lectiones per-process dedupe | 0.1–0.3 (more if 1 refused) | S | none; probatio_lectiones III must vary paths |
| 3 | credo poll backoff | 0.15–0.25 | S | none; add 50 ms survival test |
| 4 | xar gnu_inline header path | 0.25–0.4 | S–M | none (two definition sites; amalgams regenerated) |
| 5 | `_lineae_excedunt` memchr | 0.05–0.1 | S | none |
| 6 | small (segment zeroing) | ≤ 0.05 | S | none |
| | **sum** | **≈ 1.0–1.8** | | |

## Checked, not worth it

- **credo per-assertion cost:**
  - Each passing assertion allocates a notatio, makes 6 `chorda_ex_literis` copies of string
    literals, and calls `printf(".")`.
  - All credo.tsv files together hold 1.25 M assertions per full run. The largest suite,
    contractio, does 200k assertions in 154 ms *including its work*, so the ceiling is about
    1 s per run.
  - Not worth an API-visible change (e.g. storing literal pointers instead of copies).
- **credo section overhead:** a gettimeofday, a TSV line, and memory probes per section, over a
  few thousand sections in total. Negligible.
- **`credo_numerus_praeteriti` walks the whole list per call:** it is only called at the summary,
  not per section. No O(n²).
- **Cheaper fork via malloc env vars** (`MallocNanoZone=0`, `MallocXzoneEnabled=0`, …): xzone fork
  locks are 26% of toml_totalitas samples, but the A/B landed in a load spike (12→19) and was
  inconclusive. It would also be a platform hack in the test runner. Dropped.
- **chorda_aequalis:** it already does length check, pointer-equality shortcut, then memcmp. Its
  6.4% in canon_corpus is cache misses on `*n->titulus` from `canon_iudicare`'s `ad_elementa`
  scan (canon.c ~4171/4209). If both sides were interned in one intern, canon could compare
  pointers. That is a canon question, not a foundation one.
- **`_elementum_quaerere_vi`** (canon.c 943/951): 46% of tabula_dispersa_invenire in canon_corpus.
  Canon-specific, not looked at further.
- **filum/lectiones per-call cost without a book:** one getenv (~100–200 ns). Only matters through
  the probe flood, which ideas 1 and 2 remove.
- **piscina:** after C2 the fast path is ~25 instructions. `piscina_vacare` zeroes whole alvei,
  but it never showed up in these profiles (its callers are GUI and app libraries).
- **Hashing (fnv1a, probe):** round 1 already covered it. A word-at-a-time hash changes
  iteration order (C "not worth it").

## Open questions

1. **Idea 1's premise:** is "the source tree is frozen during one aedilis process" acceptable as
   written policy? It already holds for the extractor memo; I would add one sentence to
   aedilis.worklog.
2. **C7 revisited?** `chorda_internare` is **13.6% inclusive in md_stml** (6.6% obsoleta, 2.7%
   canon). Its callers are text-node values (`stml_textum_creare_ex_chorda`), attribute values
   and titles, all interned. On every miss the insert re-hashes and re-probes (inserere ≈ 12%
   of internare's children in md_stml). A private find-or-insert (C7) is the lever. It was
   skipped only because it adds API. A non-public header shared by tabula_dispersa.c and
   internamentum.c would avoid the public surface, if Fran accepts that form.
3. **Idea 4's guard names the two amalgams that embed xar.h** (`SILVA_H`, `OFFICINA_H`). A future
   amalgam that embeds xar.h would fail *loudly*, in its own standalone compile. The generic
   alternative is a one-line `#define AMALGAMA` in each amalgamator's prelude: tooling, Fran's
   call.
4. **Should credo time out on wall clock?** Today `mora` is a sum of nominal sleeps, so under load
   a 5 s `mora` stretches to 6–7 s of wall. Idea 3 keeps that semantics on purpose.
