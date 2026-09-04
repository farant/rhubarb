# NOG TRANSLATIO — quid ex nog transferendum sit

*Born 2026-09-04 (Fran's question: "are there any targets in nog
worth porting to rhubarb?"). ONE place answering: what does the nog
corpus contain, what of it does rhubarb lack, and what does a port
actually cost? nog lives at ../nog (siblings nog_0, nog_1, nog_2 are
older snapshots). It is not referenced anywhere in this repo — grep
for "nog" over rhubarb's .md/.txt/.census returns nothing — so this
is a first survey, not an update. One of three surveys taken the
same day: solarium-comparatio.md (the other external predecessor,
a very different verdict) and knotapel-promotio.md (promotion from
inside this repo). piscina-adoptio.md measures the memory-model gap
that all three name as a port cost.*

*Discipline note: this is a map of AVAILABLE material, not a work
queue. House rule stands (c89-lacunae.md): rows graduate on PULL — a
consumer needs it — never checklist-completionism. Everything below
is dormant until something in rhubarb wants it.*

Status vocabulary: **PORTANDUM** (real gap; rhubarb has no
equivalent) · **PENDENS** (defensible, no consumer yet) ·
**SUPERVACUUM** (rhubarb already has it) · **MORTUUM** (obsoleted by
something strictly better here, or conflicts with house style).

## I. Quid sit nog

A C library collection, dated Sep 2025 — the generation before this
one, before Latin, and a wholly separate codebase. Flat `src/` +
`include/`, one module per concern, English identifiers, plain
`char*`, `Arena*` where rhubarb would pass `Piscina*`.

- 56 sources in `src/`, 61 headers, README claims "27 C89-compliant
  libraries"
- **36,240 lines of tests** across ~100 files in `tests/`
- Build: one Makefile, `CFLAGS = -std=c89 -Wall -Wextra -pedantic`

Nine module names appear in both trees — base64, color, http, json,
reactor, router, uri, uuid, xar — but the code did not carry over:
nog's `base64.c` is 621 lines against rhubarb's 255; `color.c` 1102
against 181. Concepts survived, source did not.

## II. Duo obstacula

**1. The dialect.** rhubarb is `latina.h` C: Latin keywords, Roman
numeral literals, `chorda` (length-carrying, NOT null-terminated)
in place of `char*`, `Piscina*` as explicit trailing parameter,
Latin identifiers and comments, `<aedilis corpus=...>` directive in
the header, a paired `.worklog.md`. Nothing in nog is written this
way. Every line changes.

**2. nog is not actually C89.** The README's claim and the Makefile's
`-std=c89` were never exercised. Measured 2026-09-04:

> **34 of 56 sources in `src/` fail to compile at all** under
> `-std=c89 -pedantic` (`cc -fsyntax-only`). Only 22 pass.

Dominant root cause is a single line — `nog_common.h:188`:

```c
#define NOG_INLINE static inline __attribute__((always_inline))
```

Defined unconditionally on non-MSVC, with no `__STDC_VERSION__`
guard. `string_utils.h` uses it for ten `char_is_*` predicates, so
every file including it dies under `-std=c89` with "unknown type
name 'inline'". One-line bug in nog — but it means no other quality
claim in that README has been under load either. Treat all of them
as AUDIENDUM.

## III. PORTANDUM — real gaps

Checked by grepping all 159 rhubarb headers for each concept. These
have no equivalent here. Corroborated by `extra-libraries.md`, whose
open wishlist still names "binary tree" and "linked list?" — the
ordered-structure hole is already known.

| Module | src | tests | Fit + pull |
|---|---|---|---|
| `skiplist` | 857 | **1,426** | Ordered map. rhubarb has `tabula_dispersa` (hash) and `xar` (vector), nothing ordered. Largest single test file in nog. Pull: first ordered-iteration need. |
| `roaring` | 1,235 | 825 | Compressed bitmaps, arena-native already. Set ops for `arbor_index` cross-references, `natura`, `quaerere` candidate filtering. **Caveat in §VI.** |
| `bitset` | 1,046 | 881 | Plain bitsets; companion to roaring. Has malloc constructors to drop (§VII). |
| `adjacency_list` | 1,047 | **1,750** (8 files) | Graph storage. `arbor_index` references, `entitas` relations, and the include-closure BFS `aedilis` already hand-rolls. Cleanest warning profile of the group (15). |
| `ring_buffer` | 577 | 332 | Lock-free SPSC. Sits under `reactor`, `tcp_posix`, `vigilia`. Cleanest code in nog: 8 warnings, 0 errors under house flags. |

## IV. PENDENS — defensible, no consumer yet

| Module | src | tests | Note |
|---|---|---|---|
| `csv` | 670 | 714 | TSV/CSV parsing currently lives in *shell* — `excubitor.sh` reads `fabrica.tsv`; also `amalgama_*.sh`, `forma_evolutio.sh`, `natura_metamodulus_generare.sh`, `silex_semen_fumus.sh`. A C reader moves manifest parsing into the type-checked half. |
| `tree_viz` | 511 | 195 | ASCII tree rendering with a B-tree-aware path. Debug view for `arbor2` ASTs. Small; may be cheaper to write fresh than to read. |
| `arena_debug` | 465 | 261 | **Do not port the code.** `piscina` already exposes `summa_apex_usus`, `numerus_allocationum`, `summa_inutilis_allocatus`, `numerus_alveorum`. What transfers is the ideas layered on top: guard bytes, fill patterns, allocation heat map, per-site file/line tracking, pattern detection. Steal the list, not the file. |
| `mkparse` + `mkbuilder` | 1,741 | 1,261 | Competent Makefile parse/emit. Design mismatch — rhubarb went to STML manifests + `aedilis` + shell deliberately. Only plausible pull: `silex` emitting a Makefile for a scaffolded project leaving this ecosystem. |

## V. SUPERVACUUM et MORTUUM

Already covered here, one-to-one:

`arena`→`piscina` · `hash_table`/`u32_hash_table`→`tabula_dispersa` ·
`intern`→`internamentum` · `string_builder`→`chorda_aedificator` ·
`string_utils`/`hm_string`→`chorda` · `hash`→`friatio` ·
`crc`→already inside `friatio`/`flatura`/`imago_png` ·
`json`,`json_builder`→`json` · `argparse`→`argumenta` ·
`uri`→`uri`/`url` · `ini`→`toml` · `time`→`tempus` ·
`dir_iter`→`iter_directoria` · `draw`→`delineare` ·
`window*`→`fenestra`/`vitrea` · `nog_ui_*`→`widget`/`elementa`/`schirmata` ·
`font_6x8`→`fons_6x8` · `profiler`→`mensura` · `jsonrpc`→`internuntius` ·
`test_runner`→`credo` · `http`,`tcp`,`reactor`,`router`,`uuid`,`base64`,`xar`
→ same names, already present.

Three MORTUUM worth naming explicitly, because size could otherwise
be mistaken for value:

| Module | Volume | Why dead |
|---|---|---|
| `btree` (+`persist`, `file_backend`, `internal`, serializers) | **4,972 src / 5,081 tests** | Largest asset in nog and the most thoroughly obsoleted. `scrinium` vendors SQLite with FTS5; `volumen` gives document-as-one-sqlite-file with append-only event log + SHA-256 CAS. A hand-rolled B-tree would re-solve a solved problem with weaker durability. |
| `cparse` + `cbuilder` | 2,162 src / 1,690 tests | Superseded by silva / `arbor2_*` / `lapifex`, which round-trip C byte-for-byte through the AST and face macros honestly. nog's cparse is a toy beside it. |
| `error` (+`logger_c89`) | 365 + 164 | Thread-local global error store. **Conflicts with house style** — rhubarb uses per-library status enums (`ARBOR_ERROR_*`, `ARBOR_PP_ERROR_*`, …) and `aedilis`'s posture: *RECUSARE CLAMOSE — nullus fructus, causa nominata*. Importing a central error object would fight that. |

`config`/`env` (672 + 658) have no rhubarb twin, but the territory is
covered by `toml` + `canon` + STML, and `getenv` appears at only two
call sites in the whole tree (`silex.c`, `villa_agens.c`). No pull.

## VI. Mensura — measured friction

Every candidate compiled 2026-09-04 under the house flag set
(`-std=c89 -pedantic -Wall -Wextra -Wconversion -Wsign-conversion
-Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings`):

```
skiplist          857 loc     85 warn   12 err
roaring          1235 loc    102 warn    0 err
bitset           1046 loc    118 warn    0 err
adjacency_list   1047 loc     15 warn    0 err
ring_buffer       577 loc      8 warn    0 err
csv               670 loc     44 warn    8 err
tree_viz          511 loc     47 warn    9 err
```

Aggregate warning causes across all candidates:

```
174  -Wc99-extensions
142  -Wdeclaration-after-statement
134  -Wsign-conversion
100  -Wgcc-compat            (for (int i = ...))
 85  -Wlong-long
 24  -Wcast-qual
```

Most of this is mechanical. The 134 `-Wsign-conversion` are not:
each one is an index/size type decision nog made loosely and rhubarb
would have to make on purpose.

**Roaring caveat (found on audit, worth the record).** Run containers
— the run-length compression that is the "R" in Roaring — are
switched on throughout (`src/roaring.c` lines 340, 524, 557, 619,
917, 994, 1069, 1136, 1173) but **never created**: `create_run_container`
at `src/roaring.c:65` has no call site, which is why it trips
`-Wunused-function`. `tests/test_roaring.c` mentions RUN **zero**
times. So the highest-value candidate ships array and bitmap
containers only, with the compression path dead and untested. Port
it as a two-container bitmap and add runs deliberately, or not at
all — do not assume the 825 test lines cover what the name implies.

## VII. Ordo — if and when something pulls

By value ÷ friction, not as a queue:

1. **`ring_buffer`** — 577 lines, 8 warnings, arena-only dep. The
   smallest real thing; good calibration for what a nog→rhubarb port
   actually costs before committing to a larger one.
2. **`roaring`** — highest payoff, subject to the §VI caveat.
3. **`bitset`** — natural companion; some of it may collapse into
   roaring's container layer rather than standing alone.
4. **`adjacency_list`** — most tests per line, and `aedilis`'s
   include-closure BFS is a waiting first customer.
5. **`skiplist`** — only with an ordered-iteration need in hand.
   1,426 test lines is a lot to translate on spec.

**Method.** Treat nog as a spec repository, not a source repository.
For each: read the implementation to recover the design, write fresh
in house dialect against the full flag set, port the test file FIRST
as the acceptance criteria. The tests are the durable asset; the C
is a first draft that never met a strict compiler.

**One interface decision up front** (Eskil's rule — the API is what
you cannot refactor later): nog gives every structure a dual
`x_create()` / `x_create_arena()` pair, hedging malloc against arena.
rhubarb does not hedge — everything takes `Piscina*`. Collapse to the
arena form on the way in. Porting the split and regretting it later
is the expensive order.

## VIII. AUDIENDA

Unverified; audit on first touch, do not promote to a row above.

- **nog's tests have never been run here.** Their existence and size
  are measured; their pass rate is not. Given §II the suite may not
  build at all under `-std=c89`.
- **Implementations were not read line by line** — only headers, APIs,
  dependency graphs, and compiler diagnostics. Algorithmic correctness
  is assumed from test volume, which the roaring finding shows is not
  a safe assumption.
- **nog_0 / nog_1 / nog_2 assumed to be older snapshots** (161 / 128 /
  166 sources vs 189, dated Sep 19–24 2025) — never diffed against
  `nog`. If a port begins, check whether the newest tree is actually
  the best version of the module in question.
