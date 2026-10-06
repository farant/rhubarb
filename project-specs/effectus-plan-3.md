# effectus plan 3 — which definition REACHES a use (effectus slice 3)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit),
> INLINE on main, ONE TASK PER TURN with Fran's approval, no subagents.
> Several tasks OPEN with a reading or a measurement; a result outside
> the named branches is shown to Fran before work continues. Written
> 2026-10-06 from `effectus-spec-3.md` (interview Q1–Q7 in
> `effectus-3-interview.md`; A1–A5 decided = every recommendation). On
> approval: one opus per task in a park; `silva.commissio(opus=ID)`
> closes each.
> Park …JZEM (region 'flow analysis'); opera T1 …BRF, T2 …RAW, T3 …2B0,
> T4 …0H6, T5 …7H0, T6 …SMR.

**Goal:** a variable's value at a use is the join of the definitions
that may REACH it - not of every definition in the process - so the
531 `discordia` sites (sequential reassignment) and the script/function
argument sites (507 `argumentum`) resolve, while every site's new value
stays a subset of its slice-2 value (checked over the whole house).

**Architecture:** inside `crusta_effectus.c`: `_definitiones_attingentes`
(backward walk over crusta's AST, spec §II) replaces "all definitions
in scope" in `_variabilem_intus` and its siblings
(`_variabilis_definita`, `_elementa_colligere`); FALLBACK = today's
union. `Arcus` gains the exec site's argument words (spec §III). The
CLI gains `-subsumere`. A1 adds one optional attribute to the effectus
ingressus (`aedificatio.canon`).

**Spec:** `project-specs/effectus-spec-3.md` (§0 data, §I principles,
§II the walk, §III arguments, §VIII measuring, §IX review focus, §X
done, §XII decided).

## Global constraints

- Everything in `effectus-plan.md` and `effectus-plan-2.md` Global
  constraints holds - and slice 2's lessons: exact-string edits only
  (no whitespace-insensitive helper); `./silva/nexus.sh <sym>` BEFORE
  `vocabula.sh -nova`; plants must compile and fail AT the asserted
  place; every "today X happens" in a doc gets a file:line and a check.
- **Soundness (spec §I.1):** after every task, `-subsumere` over all
  tracked `.sh` against the FROZEN slice-2 binary passes. A failure is
  a defect, whatever the fixtures say.
- **Frozen reference:** before T2's first edit, copy the slice-2 build
  of `crusta/build/effectus` (ccc2f3a3) to the scratchpad as
  `effectus.slice2`; never rebuild it.

## Review Focus (spec §IX -> where tested)

1. Subshell leak `( X=a ); cat "$X"` -> T3 CONTRARY.
2. `cmd | while read x; do Y=...; done; cat "$Y"` -> T3 CONTRARY.
3. Functions called from a sourcing plagula -> T4 (cursor_communis
   functions, called by runners).
4. `[ -n "$X" ] || X=d` is a MAY -> T2 CONTRARY.
5. Recursion through call sites -> T4 `CREDO_NON_PENDET`.
6. Cost: census and judge time within ~10% -> measured T2, T4, T5, T6.

## File structure

| file | responsibility |
|---|---|
| `crusta/instrumenta/effectus.c` | T1 `-subsumere`; T5 chain-root argv |
| `crusta/fontes/crusta_effectus.c` | T2 walk; T3 boundaries; T4 functions; T5 sourcing + Arcus argv |
| `crusta/effectus_subsumptio.sh` (NEW) | T1 house-wide subset run (tool, C14: gates spawn nothing) |
| `crusta/probationes/probatio_crusta_effectus.c` | T1 predicate section; T2–T5 one section each |
| `aedificatio.canon`, `toml/aedificatio.stml` | T5 `argumenta` on the effectus ingressus (A1) |
| `crusta/fontes/crusta_effectus.worklog.md`, spec §XI, MEMORY, ledger | per task; T6 closes |

---

### Task T1: the subset check

**Opens with:** copy `crusta/build/effectus` (built at ccc2f3a3) to the
scratchpad as `effectus.slice2`; record its md5 in the worklog.

- [ ] **Step 1: Failing section** `XVIII. subsumptio` in
  `probatio_crusta_effectus`: two in-memory summaries - NOVUM site
  covered (equal / glob / prefix / old `nulla`) passes; NOVUM site with
  a member outside the old value fails, named.
- [ ] **Step 2: Implement** `crusta_effectus_subsumpta` (or a CLI-only
  function in `effectus.c`): per site of NOVUM, the VETUS site at the
  same (plagula, sedes, elementum) covers it. CLI: `effectus
  -subsumere VETUS NOVUM` (exit 1 + the uncovered sites).
- [ ] **Step 3: Tool** `crusta/effectus_subsumptio.sh <binarium_vetus>`:
  every tracked `.sh` summarized by both binaries, `-subsumere` each;
  prints `N/M subsumpta`. Born green: old binary vs itself -> M/M.
- [ ] **Step 4: Plant:** NOVUM summary with one path changed -> named
  red (section and tool). Restore.
- [ ] **Step 5: Commit** (gates `crusta`, owed).

### Task T2: the walk - sequence, branches, loops, fallback

- [ ] **Step 1: Failing section** `XIX. ordo`: `X=a; X=b; cat "$X"` ->
  `b` only; `if c; then X=b; fi; cat "$X"` (with `X=a` before) ->
  {a, b}; `case` with two branches; `for f in a b; do cat "$f"; f=z;
  done` -> inside {a, b}, after {a, b, z}; back-edge: `X=a; while c; do
  cat "$X"; X=b; done` -> {a, b}; A2 CONTRARY `X=a; [ -n "$Y" ] ||
  X=b; cat "$X"` -> {a, b} (never b alone); A3: `X=a; eval "$E"; cat
  "$X"` -> FALLBACK (slice-2 answer).
- [ ] **Step 2: Implement** `_definitiones_attingentes` (spec §II.1-2,
  4) and route `_variabilem_intus` through it; FALLBACK = today's
  loop. `_ansa_ligans` becomes the for-binding case (keep the function
  only if still called).
- [ ] **Step 3: House:** `-subsumere` over all `.sh` vs
  `effectus.slice2` passes; census by causa delta (worklog); judge time.
- [ ] **Step 4: Plant:** the kill rule applied to a CATENA member
  (A2 contrary red). Restore.
- [ ] **Step 5: Commit** (gates `crusta`, `iudicium-fumus`, owed).

### Task T3: process boundaries (Q7)

- [ ] **Step 1: Failing section** `XX. subprocessus`: `( X=b ); cat
  "$X"` (X=a before) -> a only (RF 1); `ls | while read x; do Y=b;
  done; cat "$Y"` (Y=a before) -> a only (RF 2); `X="$(Y=c; echo
  "$Y")"`: Y=c does not reach after; inside `( )`, outer definitions DO
  reach.
- [ ] **Step 2: Implement** the boundary rule (§II.2, CRUSTULA, PIPA
  segment, SUBSTITUTIO).
- [ ] **Step 3: House** subset check; census delta. **Plant:** boundary
  ignored -> RF 1 red. **Commit.**

### Task T4: functions (call sites, arguments)

**Opens with a reading:** how many house functions are called from a
plagula other than their own (cursor_communis, mensor_suitae), and how
`_ambitum_derivare` orders plagulae - recorded in the worklog.

- [ ] **Step 1: Failing section** `XXI. functiones`: body reads a global
  assigned differently before each of two calls -> union of the two;
  `f a.txt; f b.txt` with `cat "$1"` -> {a.txt, b.txt} (A4); function
  defined in a SOURCED plagula, called in the sourcing one (RF 3);
  uncalled function -> FALLBACK; mutual recursion `f(){ g; }; g(){ f;
  }` -> bounded, `CREDO_NON_PENDET` (RF 5); `shift` in body -> `$N`
  stays `argumentum`.
- [ ] **Step 2: Implement** the FUNCTIO rule and positional binding.
- [ ] **Step 3: House** subset check; census delta; judge time.
  **Plant:** call sites ignored -> union again (red). **Commit.**

### Task T5: sourced plagulae, script arguments (Arcus, A1)

**Opens with a reading:** every effectus ingressus in the house
(`./crusta/effectus.sh -catenae`) and the argv silva.py runs it with;
branch: a mismatch is shown to Fran before the attribute is written.

- [ ] **Step 1: Failing section** `XXII. argumenta`: parent `./a/c.sh
  x.txt` + child `cat "$1"` -> x.txt; two callers -> union; one caller
  with `"$@"` -> `argumentum`; walk continues at the `source` site
  (definition in the sourcing plagula before `source` reaches the
  sourced plagula's top level).
- [ ] **Step 2: Implement** `Arcus` argument words; child `$N`; the
  `source`-site continuation.
- [ ] **Step 3: A1:** `aedificatio.canon` ingressus attribute
  `argumenta` (nota); `toml/aedificatio.stml` `argumenta=""`; `-clavis`
  passes it to the root; the cross-check against silva.py's table
  (where it lives decided by the opening reading); fabrica-fumus XXXII
  green.
- [ ] **Step 4: House** subset check; toml key diff (FILTER resolves to
  ""?), oracle, judge time. **Plants:** argv dropped -> section red;
  mismatched declared argv -> cross-check red. **Commit** (owed gates).

### Task T6: close

- [ ] **Step 1:** Census by causa before (slice-2 T7) / after in spec
  §XI; `discordia`, `argumentum` deltas; subset check final.
- [ ] **Step 2:** Toml: 0 errata, key, oracle non tecta 0, judge time.
- [ ] **Step 3:** Docs (worklog, crusta/CLAUDE.md, MEMORY); ledger:
  park closed; next (must-write-before-read key shrink, Q5; recursio
  fixpoint, A5) filed.
- [ ] **Step 4: Commit.**

## Not in this plan (stated)

- Dropping key lines for "written earlier in this run" (Q5).
- `recursio` fixpoint (A5); `shift` modelling; `eval` contents.
- Inter-script variable passing through the environment (`export X;
  ./child`) beyond today's `ambitus`.
