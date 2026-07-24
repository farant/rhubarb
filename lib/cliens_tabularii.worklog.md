# cliens_tabularii — worklog

## 2026-07-24 — extraction from forum.c (villa V2)

~340 lines lifted out of `apps/forum/forum.c` into `include/` +
`lib/`. forum converted in the same commit, so there is never a
period with two copies.

### The real reason wasn't DRY — it was coverage

`compile_tests.sh` globs root `probationes/`. **No runner anywhere
sees `apps/`** — `apps/forum/fumus.sh` is referenced by no script, no
Makefile, no hook. So this client, including the non-obvious
fast-fail fix below, had *zero* automated coverage for its entire
life. It has 21 asserts now.

### The subtle law the extraction had to preserve

`_conectere_aut_gignere` spawns the launcher **once**, then fails
fast forever after. That looks like an optimization and isn't: the
webview polls every 2s, and a blocking handler that waits 5s for a
dead daemon to appear freezes the UI. Section VII of the gate pins
both halves — first call pays the spawn window, second returns
immediately.

While moving it, `genitus` went from `hic_manens` (a hidden
per-process global) into the struct. Two clients on different ports
in one process would previously have had the second one's spawn
suppressed by the first. Nobody had two clients yet; villa plus a
future second world would have found it the hard way.

### Other changes made deliberately during the move

- `actor` is a struct field, not the literal `"fran"` baked into
  `transmittere`. A hardcoded identity inside a shared library is a
  landmine for the second consumer.
- `launcher` and `praefixum` likewise configurable — villa's stderr
  says `[villa]`, not `[forum]`.
- `quantum` became a parameter of `cliens_tabularii_legere` instead
  of a hardcoded `CC`. It was policy wearing the costume of a
  constant.

The allowlist stayed fixed at `addere|gerere|legere|quaerere`
on purpose: that set *is* the contract "an app may write to the
world," and widening it should be a deliberate edit, not a config
knob.

### Method note

The body was extracted with `sed` line ranges and transformed
mechanically rather than retyped. For a ~340-line move that is worth
doing — it makes the diff a genuine move, and it removes any chance
of a transcription slip in code that already works. Only the
signatures and the three semantic changes above were hand-edited.

Gates: `probationes/probatio_cliens_tabularii.c` 21/21 (ephemeral
daemon on `-portus 0`, fully offline); forum FUMUS PLENUS unchanged;
root suite 104/104; gesta 4/4.
