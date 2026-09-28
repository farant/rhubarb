# tessera_eventum.worklog.md

## 2026-09-28: input vectors, T1 (harness + seed); H1 measured

`probationes/probatio_tessera_vectores.c` + `vectores_initus.h` (plan:
`project-specs/tessera-vectores-plan.md`). A test-local pons
(`PonsFrustorum`) delivers scripted chunks, and an empty chunk is a
timeout. Every vector runs four shapes: INTEGRA, BIPARTITA (every split),
SINGULA (byte at a time), SEQUENS (+ 'a'). The memoria pons is untouched
(it's in the hand-written amalgam header).

**H1 confirmed by measurement.** The first run of tessera's own 34 seed
cases, all marked VALET: 21 failures, ALL in shape SINGULA, ALL on
sequences of 3+ bytes. INTEGRA, BIPARTITA and SEQUENS all green;
two-byte sequences (`é`, alt+b, alt+Enter) green in every shape. Cause:
`tessera_eventum_expectare` reads once, and on INCOMPLETUM reads exactly
ONCE more (the ~25 ms `TESSERA_MORA_FUGAE_MS` read, line 597) before
treating the sequence as timed out. Two faces:

- an ESC sequence in 3+ reads → FUGA, then the rest as phantom keys
  (`\033[A` → fuga, `[`, `A`)
- a 3/4-byte UTF-8 rune in 3+ reads → **silently lost** (lead byte
  dropped by the timeout path, continuation bytes dropped as invalid).
  Worse than the ESC face, because nothing shows.

Real-world: ssh and slow links split writes. The fix belongs to its own
task (loop reads while bytes keep arriving within the timeout, and treat
only a 0-byte read as the timeout). The 21 owed shapes are its red tests
already.

**Design refinement (plan D5):** debt is per SHAPE (`formae_debitae`
bitmask), not per vector. Vector-level debt would have hidden the
INTEGRA/BIPARTITA/SEQUENS assertions of these 21 vectors, and those are
real, passing tests.

Planted faults, both directions: a flipped expectation (sursum →
deorsum) failed by name in exactly integra, bipartita 1 and sequens (the
singula shape stayed "debitum manet", correctly); a passing shape falsely
marked owed failed with `DEBITUM SOLUTUM - promove: "a" [integra]`.

**Tool incident, not tessera:** a python transform of `vectores_initus.h`
piped into `./silva/scribe.sh` writing the SAME file truncated it (the
classic `cat f | … > f` race). scribe then wrote 0 bytes and examen said
ACCIPE. Recovered from the conversation (the file was untracked). Rule:
transform to a scratch file, then scribe from it. Filed as a question in
the tabularium (scribe should refuse empty stdin).

## 2026-09-28: T1b, the H1 fix (reader loops while bytes arrive)

`tessera_eventum_expectare`: after INCOMPLETUM the reader now reads
**while** bytes keep arriving within `TESSERA_MORA_FUGAE_MS`. Only an
EMPTY read is the timeout (then the old path: lone ESC → FUGA,
otherwise drop one byte). The loop is bounded: each iteration adds bytes,
and the 64-byte buffer is finite (a full buffer is handled as a timeout).
The three copies of "parse, skipping consumed noise" became one helper,
`_parsare_plene`.

Proof, in order:
1. The 21 owed SINGULA shapes all went to `DEBITUM SOLUTUM - promove`,
   and nothing else moved: the red-before-green was T1's debt list.
2. Promoted to VALET (186 assertions, 0 debts). The existing eventum
   suite (68) is unchanged and green: behavior-preserving elsewhere.
3. Amalgam regenerated: VERIFICATUM (standalone full severity, hospes
   7/7, nm 0, censura), idempotent (md5 identical on a second run);
   only `tessera.c` changed. saltuarius 13/13 against it.
4. Planted fault that COMPILES: a `frange` at the end of the new loop's
   body (= the old one-extra-read behavior) → exactly 21 failures, all
   SINGULA. My FIRST plant (`dum` → `si`) did NOT compile (`frange`
   outside a loop), and the pipeline hid the build error behind a later
   green run. That's the house rule "gates born red by a plant THAT
   COMPILES", earned again: capture each run's log and check rc before
   reading a verdict.

Unchanged and still named for later: an overlong sequence that fills the
64-byte buffer is still flushed as a timeout (FUGA + the rest as phantom
keys). That's the tokenizer's job (bounded CSI with `csi_ignore`,
terminal-planning modules/002).
