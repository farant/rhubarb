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
