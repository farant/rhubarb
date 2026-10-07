# fabrica plan 5 — the gate rollout

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit),
> INLINE on main, ONE TASK PER TURN with Fran's approval, no subagents.
> Tasks that OPEN with a reading show Fran any result outside their named
> branches before work continues. Written 2026-10-06 from
> `fabrica-spec-5.md` (interview Q1–Q6, AUDIENDA A1–A4 decided = every
> recommendation). One opus per task in the park; `silva.commissio(opus=ID)`
> closes each. Gates T5–T8 are also rows of one expeditio (spec §IV).
> Park …JQFH2 (region 'fabrica - makefile replacement'); opera T1 …65K2,
> T2 …1PG2, T3 …JSTT, T4 …QT7W, T5–T8 …9SBK, T9–T11 …47GE, T12 …SYS.

**Goal:** a migrated gate is reused whenever its own inputs did not
change - first by making the inputs every verdict shares fine and
observable (re-run causes, directory listings, silva.py out of the
verdict), then by migrating aedilis, fabrica, generata, oratio and
pythonica with an extended checklist.

**Architecture:** `tools/fabrica.c` / `lib/fabrica.c` (causes into
`cursus`; the verdict runner in C; signum on the action); `tools/aedilis.c`
(enumerations); `aedificatio.canon` (signum, mandatum-derived argv);
`crusta` (argv from the mandatum; census split); `pythonica/` (audit-hook
read ledger, porta() thin); per-gate aedificatio.stml declarations.

**Spec:** `project-specs/fabrica-spec-5.md`.

## Global constraints

- Everything in `fabrica-plan-3.md` and `effectus-plan-3.md` Global
  constraints holds: exact-string edits only; `./silva/nexus.sh` before
  `vocabula.sh -nova`; plants compile and fail AT the asserted place;
  every "today X happens" in a doc gets a file:line and a check;
  rebuild bin/fabrica after lib/fabrica.c edits; freeze the tree while
  gates run.
- **Timing only on an idle machine** (`pgrep` first; alternate old/new
  binaries) - T6 of effectus slice 3.
- A stale green is the one unacceptable failure (spec §VII.1): every
  change that makes reuse MORE likely ships with a plant proving the
  input class still invalidates.

## File structure

| file | responsibility |
|---|---|
| `tools/fabrica.c`, `lib/fabrica.c` | T1 causes; T3 verdict runner |
| `tools/aedilis.c` | T2 enumerations |
| `aedificatio.canon`, `toml/aedificatio.stml` | T3 signum, mandatum |
| `crusta/fontes/crusta_effectus.c`, `crusta/instrumenta/effectus.c` | T3 argv from mandatum; T5 census split |
| `tools/reusus_retro.sh` (NEW) | T4 retro reuse rate (tool, not a gate) |
| `pythonica/` | T3 porta()/cross-check; T9–T11 profile, ledger |
| per-subsystem `aedificatio.stml` | T5–T8, T11 declarations |

---

### Task T1: why a verdict re-runs

- [x] **Step 1: Failing test** in `probatio_fabrica`: a stale verdict
  healed by `sanare` leaves a `cursus` row whose causa names the changed
  input (`lectio transitus mutata: <via>` / `ingressus mutatus: ...`).
- [x] **Step 2: Implement:** the judge's stale reason carried into the
  sanatio record; stored for SANATUM (causa of the run itself, if any,
  appended).
- [x] **Step 3: Query:** `bin/fabrica causae [titulus]` (or a tools/
  script) - counts per cause for a verdict. **Plant:** reason dropped ->
  test red. **Commit.**

### Task T2: directory enumerations (opening reading)

**Opens with a reading:** for each D row in porta_toml's trace, which
aedilis invocation (mode, argv, caller script) produced it and what the
listing is used for. Branches (spec §II.2): genuine (stays, named);
house-wide check inside a subsystem gate (moves out or is scoped);
membership-only (per-name A/X). Anything else -> Fran.

- [ ] **Step 1:** Reading recorded in the aedilis worklog.
- [ ] **Step 2: Failing test** per branch taken (aedilis test or
  fabrica fumus): adding an unrelated header leaves toml's trace digest
  unchanged; adding a file the gate DOES depend on changes it.
- [ ] **Step 3: Implement**; **plant:** the genuine enumeration removed
  -> the "file it depends on" case reuses (red). **Commit.**

### Task T3: the verdict runner in C (A1–A3)

- [ ] **Step 1: Failing tests:** `probatio_fabrica` - an `iudicium`
  action whose mandatum is the runner itself and which declares a
  signum: exit 0 + signum -> verdict `<G>: <count line>`, written only
  on pass, deleted before the run; exit 0 without signum -> FRACTUM;
  `iudicium-fumus` case for the same in a temporary root.
- [ ] **Step 2: Implement** in bin/fabrica (signum = literal prefix,
  matched in C, no regex - A2); canon: `signum` on iudicium actions.
- [ ] **Step 3: toml moves:** porta_toml mandatum =
  `./toml/compile_probationes.sh`, signum `TOML PROBATIONES: `;
  `pythonica/silva.py` leaves its ingressus. Verdict text compared to
  the silva.py one (spec §VII.2).
- [ ] **Step 4: A3:** effectus ingressus argv derived from its own
  action's mandatum when the via matches (`<argumenta>` stays only for
  roots run by other actions); `silva.argumenta_catenarum` compares
  PORTAE to the mandatum. **Plants:** signum check off -> red; mandatum
  argv changed -> cross-check red. **Commit** (owed gates).

### Task T4: measure reuse

- [ ] **Step 1: Tool** `tools/reusus_retro.sh <verdict> [-n N]`: over
  the last N commits, would the recorded trace have been reused between
  consecutive commits (changed paths from git ∩ trace L/X/A paths, D
  directories with added/removed entries, declared ingressus)? Prints
  rate and the voiding causes. Born checked: on toml's trace from
  BEFORE T2/T3 (saved at T1) it must name `include` and `silva.py`
  among the causes.
- [ ] **Step 2:** toml before (saved trace) / after (current) over the
  same commits; live causes from T1 since then. Numbers in spec §XI.
  **Commit.** Result shown to Fran before T5 (if the rate is still low,
  the next voiders are named first).

### Tasks T5–T8: aedilis, fabrica, generata, oratio (expeditio rows)

Each row = the spec §III checklist, one commit with its plants and three
audited reuses; the census split (in-chain vs standalone unresolved)
lands with T5 (aedilis). Order may change after T5–T6 (spec §VI).

- [ ] **T5** aedilis (`tools/aedilis_porta.sh`)
- [ ] **T6** fabrica (`tools/fabrica_oraculum.sh`)
- [ ] **T7** generata (`tools/generata_probare.sh`)
- [ ] **T8** oratio (`oratio/compile_probationes.sh`)

### Task T9: pythonica - where the time goes

- [ ] **Step 1:** `cProfile` of `pythonica/probatio_silva.py`: time per
  silva.py function and per spawned tool; candidates for C named (each
  becomes a desideratum, not work in this slice). **Commit** (docs).

### Task T10: the Python read ledger (A4)

- [ ] **Step 1: Failing test** (pythonica): a harness run that opens,
  lists, stats a file and reads an environment variable writes L, D,
  X/A and E lines in the `lib/lectiones.c` format.
- [ ] **Step 2: Implement:** `sys.addaudithook` (open, os.listdir,
  os.scandir) + recording wrappers (`os.environ`, stat family).
- [ ] **Step 3: Plants**, one per channel (hook off for open; wrapper
  off for stat; wrapper off for environ) -> red. **Commit.**

### Task T11: pythonica migrated

- [ ] spec §III checklist for pythonica; three audited reuses. **Commit.**

### Task T12: close

- [ ] radix: retro reuse rate measured (T4 tool) -> migrate or record
  "never" with the number.
- [ ] Per-gate savings table and reuse rates in spec §XI; worklogs,
  MEMORY; ledger: park closed, J6HF fulfilled or re-filed, shadow passes
  (…2VP7) next. **Commit.**

## Not in this plan (stated)

- Moving pythonica's house logic into C (T9 names candidates only).
- Shadow passes (park …2VP7).
- Per-test verdicts (spec 3 Q1).
