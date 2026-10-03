# fabrica — interview raw, slice 3 (2026-10-03)

Feature: slice 3 of the house build layer. Earlier specs defer to it:
"tests as fabrica actions" (spec 2 §I out of scope), "per-test verdict
actions and a store shared across worktrees (slice 3+)" (spec 2 §XIII),
"Verdicts can be nondeterministic — a flaky test cached by its input key
hides the flake; slice 3 needs a policy (at least: never cache a
failure)" (spec 1b), and spec v2 §I: "Slice 3's mapping: a gate becomes
an action whose inputs are its suite's closure; a receipt becomes a
record keyed to THAT digest — editing a doc no longer invalidates the
silva gate."

Context carried into the interview (session 2026-10-03):
- Slice 2 complete (read ledger + verifying traces, content store,
  bin/compilator, familia, parallel heal + judging; spec 2 §XIII).
- Excubitor migration middle path done: tools/cursor_communis.sh;
  toml, css, crusta (+ facies/oraculum launchers), root compile_tests.sh
  compile through bin/compilator (35a1589d). W0ZBW 5/20 rows.
  tools/cursoris_oraculum.sh compares old vs new runner (root mode:
  verdict lines, since root tests print ports/pids/addresses/timings).
- Today's receipts (pythonica/silva.py): a live receipt
  `build/portae/<nomen>[.<filtrum>].viva.json` is keyed to
  sigillum_arboris() = HEAD + whole `git diff` + every untracked file
  (VETITAE excluded). Any edit anywhere makes every receipt stale, and
  commissio DELETES all live receipts after each commit (HEAD moved) -
  so a passing gate is never reused across commits. Shadow receipts
  (porta_umbra) are keyed to a photograph tree + the committed paths.
- portae_debitae maps paths -> owed gates via aedilis closures +
  inventory lenses ('tegit viae', 'tegit fontes', 'currit binaria').
  Found today: tools/cursor_communis.sh was owed by no suite (only
  vexilla) - declared lenses have gaps.
- What the read ledger sees: house C code reading through filum
  (bin/compilator hashes headers through filum, tests read fixtures
  through filum). What it does not see: bash `source`/`cat`, clang's
  own reads (covered by compilator's depfile keys), executed binaries.

## Round 1 — shape

**Q1. How wide should slice 3 be?**
Options: gate receipts first / per-test verdicts directly / wide (+ a
store shared across worktrees).
**A: Gate receipts first.** A gate becomes an action whose inputs are
its suite's closure; its receipt is keyed to that digest instead of the
whole-tree seal. Per-test verdicts on top later if it holds.

**Q2. What is cached when a gate passes or fails?**
Options: cache passes only / cache both, re-run failures / no reuse,
only skip within a session.
**A: Cache passes only.** A pass is recorded under its input digest and
reused; a failure is never cached and always re-runs.

**Q3. First client suite?**
Options: toml / radix / silva.
**A: toml.** Small (45 objects), on cursor_communis with aedilis
closures per test; easy to oracle.

## Round 2 — mechanics

**Q4. How is a gate's input digest determined?**
Options: traced + declared scripts / declared only (inventory lenses +
aedilis closures) / declared, audited by trace.
**A: Traced + declared scripts.** The gate runs with the read ledger on:
compilator's header hashes and the tests' fixture reads come from the
trace; the shell scripts the runner sources or executes (bash reads,
outside filum) are declared. Digest = union. Anything unreadable =
IGNOTUM, no reuse.

**Q5. Where does a gate's record live and get judged?**
Options: fabrica action / pythonica receipts with a new key.
**A: fabrica action.** The gate is declared in aedificatio.stml as an
action of a new kind (verdict); bin/fabrica judges it from its trace
and stores the pass; commissio asks fabrica before running it. One
judge, one ledger (spec v2 §I mapping).

**Q6. Who may reuse a recorded pass?**
Options: commissio + porta() / commissio only.
**A: commissio + porta().** commissio skips an owed or named gate whose
digest matches a pass (line 'non iterum cursa: digestum idem, <commit>');
porta() by hand reports it too unless forced; shadow (umbra) runs record
passes as well.

## Round 3 — honesty

**Q7. A gate reads something outside the repo (SDK, $HOME, /tmp, network)?**
Options: allowlist, else IGNOTUM / hash them too / ignore outside reads.
**A: Allowlist, else IGNOTUM.** System roots the clang identity already
pins (SDK, /usr/include, /usr/lib) are allowed and not hashed; any other
read outside the repo (~/.rhubarb, /tmp fixtures, sockets) makes the run
IGNOTUM - its pass is not reused. For toml this should be nothing.

**Q8. Does a recorded pass expire on its own?**
Options: no expiry / expire after N days.
**A: No expiry.** Inputs determine the verdict (the claim under test); a
pass is reused while its digest matches. Store purge (keep 5
generations) is the only eviction.

**Q9. The slice's oracle - how do we know reuse is honest?**
Options: plants + sampling audit / plants only / plants + fs_usage audit.
**A: Plants + sampling audit.** Plants that compile: edit a traced
input, a declared script, an undeclared read -> must re-run / go
IGNOTUM. Plus an audit mode: a reused pass is re-run anyway (e.g.
FABRICA_AUDIT=1 or every Nth) and must still pass; disagreement names
the missed input.

## Round 4 — scripts and the rest

**Q10. Where do the gate's declared shell scripts come from?**
Options: derived by crusta / declared in aedificatio.stml / runner
reports them at run time.
**A: Derived by crusta.** Parse the runner with the house bash parser
and follow `source`/`.` and direct executions of repo scripts
transitively (cursor_communis.sh -> vexilla.sh ...). A non-literal path
(source "$X") = IGNOTUM unless declared. No hand list to rot.

**Q11. Gates that are not yet fabrica actions?**
Options: keep tree-seal receipts / migrate all in slice 3.
**A: Keep tree-seal receipts.** Unmigrated gates keep today's
sigillum_arboris receipts unchanged; commissio asks fabrica first for
migrated ones. Migrating the rest = a batch job (expeditio) after toml
proves out, like W0ZBW.
