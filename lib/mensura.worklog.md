# mensura worklog

## 2026-08-13 — birth (library half)

Fran's design: a CLI that records any metric — benchmark a command, or
pipe a value in with a label — with push/pop nesting scoped to a run,
stored in a volumen. This commit is the library the CLI will sit on.

### Deliberately NOT Metrum

`Metrum` (parked, 01KYAMZVFN) is RRD ring-buffers: continuous samples,
**lossy by design**, fixed size forever, resolution degrading with age.
Right for "memory over a month".

Mensura is the opposite: discrete events, **lossless**, bound to a run,
**nested**. "Which commit made compilation slower" is a question RRD
structurally cannot answer, because by the time you ask, that sample has
been averaged into a bucket. Sisters, not the same thing — hence
separate names, decided before either could squat on the other's.

### Journal first, volumen second

Writing to sqlite means open + transaction, maybe 10ms. Across 150
files that adds a second to *the very build being measured*. An
instrument that changes what it measures is worse than useless — it
gets switched off.

So the hot path is a **plain file append**: one line, no sqlite.
`mensura_condere` folds the whole batch into the volumen in one
transaction after the run.

**The journal is truncated after a successful fold.** Without that,
every fold re-imports everything already imported and the numbers
inflate silently — a graph that looks like real growth. Poisoned it
(skipped truncation): probatio line 226 went red. That assertion is the
whole defence against double-counting.

### Format decisions

TSV, seven fields, one line per measurement. `mensura_ad_lineam` /
`mensura_ex_linea` are a pure round-trip pair, so the format is fully
testable with no file and no volumen.

**A field containing a TAB or newline is refused, not escaped.** A
format that permits breaking itself corrupts data *silently* — the
reader counts fields, finds a different number, and produces a
plausible wrong record. Refusing at write time is the only place the
error is still attributable.

`unitas` is mandatory. Without it you cannot label an axis or safely
aggregate, and adding it after data exists means backfilling.

Value is `duplex`, formatted `%.6f` (microsecond resolution). Locale
stays "C" because nothing calls `setlocale`, so the decimal point is
stable across machines — worth knowing before someone adds `setlocale`
somewhere and quietly breaks every stored number.

`momentum` is **s64** milliseconds. gettimeofday is not monotonic (NTP
can step it), so a delta can be negative; signed makes that visible
rather than wrapping to an enormous positive and producing a branch
that silently never runs (codex 80's whole point).

### Tooling gaps closed on the way

silva's POSIX shim lacked `getpid` — added, same as `sys/utsname.h` and
`sysconf` earlier today. The shim only describes what our analyser
knows; real compilation uses real headers, so a missing entry shows up
as REICE on code clang accepts happily.

### Note to self

Twice today I wrote a plausible house API name from surrounding
convention instead of reading the header —
`chorda_aedificator_addere_literas` (real:
`..._appendere_literis`) and `CREDO_MAIOR_S64` (does not exist; there is
`CREDO_AEQUALIS_S64` but no S64 ordering macro). The post-edit hook
caught both before compilation. Naming here is regular enough that
wrong guesses look right.

### Not built yet

The `mensor` CLI: `sessio`, `mensurare -- <cmd>`, `addere`, `condere`.
Nesting plan — `MENSOR_SESSIO` / `MENSOR_PARENS` in the environment so
child processes nest for free, plus an explicit `-parens` for
same-process shell loops where inheritance gives nothing.
