# tabularium in arboribus — plan (restore on open, closed trees, seq guard)

*2026-09-30. Written in `../rhubarb-secunda` after runae U6a, for the
main-tree session to execute (Fran: it is refactoring the build system
next to this code). Prompted by ledger quaestio …VSY50E (a READ wrote
26 genesis events into the committed annals of a worktree) and the
older …MKMD2 (the ledger is not worktree-safe). Fran's idea: a
gitignored marker file that turns ledger writes off. Names marked
(unsealed) are working names; Fran names.*

## 1. Goal

The house doctrine (CLAUDE.md, TABULARIUM) says: *truth is
`gesta/annales/tabularium.jsonl` (committed); the `.db` is a
rebuildable projection of it.* The code does not honour that on open,
and one cold READ in a fresh tree proved it:

| today | after this plan |
|---|---|
| a tree without `tabularium.db` opens an EMPTY ledger | the db is rebuilt from the annals first; reads see the true ledger |
| the first open of an empty db seeds 26 genesis events THROUGH the append path, into the committed annals, seq restarting at 1 | seeding finds its genera (restored) and writes nothing |
| a linked worktree (or clone) can write, forking an append-only log that cannot be merged (…MKMD2: seq is a dense primary key) | writes refuse by name in a CLOSED tree; a marker file opens or closes any tree |
| an append whose seq is ≤ the annals' last seq is written silently | refused loudly — the log never restarts numbering |

## 2. What exists (read 2026-09-30, commit 10a2a6c4)

The chain behind …VSY50E, measured 2026-09-29 03:51Z in rhubarb-secunda:

1. `tools/portae_debitae.sh` → `silva.portae_debitae_relatio` →
   `silva.inventarium` (`pythonica/silva.py:1914`) → cold path
   `./gesta/frigida.sh -inventarium 'suitae probationum'` — a LECTIO.
2. `gesta/frigida.sh` first runs `gesta/tabularium.sh </dev/null` "to
   build objects" (its own comment claims the store stays untouched),
   then `nota_frigida` → `frigida_currere`.
3. `frigida_currere` opens the ledger for READ forms too:
   `tabularium_creare` + `tabularium_se_initiare`
   (`gesta/fontes/frigida.c:559`, `:678`).
4. `tabularium_se_initiare` (`gesta/fontes/tabularium.c:14938`) runs
   `_initialize_tractare` (`:14460`): `gesta_aperire` on
   `tabularium.db` (gitignored, `.gitignore:23`; absent in a fresh
   tree → created empty), `gesta_plicare`, then **`_seminare`**
   (`:14235`, called `:14485`) — "idempotent", but against the DB,
   which is empty, so every genus + the tag vocabulary is written.
5. Every write goes through `gesta_scribere` (`gesta/fontes/gesta.c:3091`)
   and the annals append `_annalem_appendere` (`gesta.c:1913`,
   `fopen(... "ab")` at `:1949`) → 26 lines, seq 1–26, appended to 1,497
   lines of real history.
6. `gesta_ex_annalibus_restituere(piscina, via_annalium,
   via_scrinii_novi)` (`gesta.c:4582`, declared `gesta.h:139`) exists and
   is tested (`gesta/probationes/probatio_gesta.c:843`, `:3782`) but NO
   production path calls it.

Only `silva.commissio`'s VETITAE refusal stopped the lines reaching a
commit; Fran reverted them by hand.

## 3. Decisions (proposed; Fran decides)

**D1 — the switch.** One choke point: the annals append
(`_annalem_appendere`, or the top of `gesta_scribere` — every writer,
MCP daemon, cold path, pythonica, ends there). A tree is **open** or
**closed**:
- default: the main tree (git dir == common dir) is OPEN; a linked
  worktree (git dir ≠ common dir) is CLOSED — the failure was a tree
  nobody configured, so the safe default must not depend on anyone
  remembering a file;
- a gitignored marker at the repo root overrides either way:
  `.tabularium_clausum` closes (e.g. the main tree during ledger
  maintenance), `.tabularium_apertum` opens (a scratch tree on purpose);
  both present = closed. Names (unsealed).
- a closed tree refuses writes with a named message: *"tabularium
  clausum in hac arbore (<causa: arbor coniuncta | .tabularium_clausum>)
  - scribe ex arbore principali"*; exit 1 on the cold path, an error
  result on MCP.

Open question inside D1: are the frigida_probare worktrees and the
pythonica umbra clones (`~/.rhubarb/umbrae`, full clones, not
worktrees) closed? Clones have git dir == common dir, so by the rule
above they would be OPEN. Proposal: the umbra/frigida creators drop
`.tabularium_clausum` into every tree they make.

**D2 — restore on open.** In `gesta_aperire` (or one wrapper every
opener uses): if the db holds 0 events and the annals hold > 0 lines,
replay the annals into the db before anything else
(`gesta_ex_annalibus_restituere` does this into a NEW db today — reuse
its body on the empty one). Restoring writes only the gitignored db,
never the annals, so it runs in CLOSED trees too — reads there must be
right. A db whose last seq is BEHIND the annals (stale projection) is
the same case at larger scale; proposal: replay the missing tail, and
refuse (never guess) if the db is AHEAD of the annals.

**D3 — seeding.** After D2 `_seminare` finds its genera and writes
nothing in any tree with history. In a CLOSED tree it must also never
try (skip silently — reads work without it). Only a truly empty ledger
(no annals) in an OPEN tree is seeded: a brand-new repository.

**D4 — seq guard.** The append path refuses an event whose seq is ≤
the last seq already in the annals file (read the tail once per open,
keep it in `GestaMundus`). This is independent of D1–D3: it turns ANY
future second-genesis into a loud refusal instead of silent
corruption.

**D5 — frigida.sh builds without running the server.** Replace the
`gesta/tabularium.sh </dev/null` step with a build-only path (a
`-struere` flag on the launcher, or aedilis directly). A read must
never start a writer. (The main tree's build-system refactor may
already change this line — coordinate there.)

## 4. Tasks (inline, one per turn; red first; plants)

**L1 — the failing test.** New probatio (gesta suite): a temp dir with
a COPY of the committed annals and no db; run the cold read
(`frigida_currere` with `-inventarium` and `-res`) and a cold write.
Assert: (a) the annals are byte-identical after the read; (b) the read
sees a known res from the annals (e.g. the 'suitae probationum'
inventory); (c) in a tree marked closed, the write is refused by name
and the annals are still byte-identical. Today (a) and (b) fail.
Plus a linked-worktree case (`git worktree add` into build/, as
`probatio_git` does for 059fe759) for the D1 default.

**L2 — restore on open (D2).** Plants: restore skipped → (a)(b) red;
tail-replay skipped → a stale-db case red.

**L3 — the switch (D1) + closed-tree seeding (D3).** Plants: worktree
detection inverted; marker ignored; seeding in a closed tree.

**L4 — seq guard (D4).** Its own case: a hand-made event with seq ≤
tail is refused and the file is unchanged. Plant: guard off.

**L5 — frigida.sh build-only (D5)** — after the build-system refactor
settles, so it lands once.

**L6 — docs and ledger.** CLAUDE.md TABULARIUM paragraph (closed trees,
the markers, "restore on open"); gesta worklog; `worktree-secunda`
memory note (it currently says "ledger MCP DISABLED there; never run
portae_debitae there" — the second half becomes unnecessary); close
…VSY50E and …MKMD2 (the merge question in …MKMD2 — two trees' annals
cannot be unioned — stays open unless D1's "worktrees never write"
is accepted as its answer).

## 5. Names to seal (Fran)

`.tabularium_clausum` / `.tabularium_apertum` (or one file with a
word inside); the refusal message; the launcher's build-only flag.

## 6. AUDIENDA (not verified)

- Whether `gesta/tabularium.sh </dev/null` (the build step) ALSO seeds
  in a fresh tree — the read path (§2.3) is enough to explain the 26
  lines and is what L1 reproduces; the server path was not isolated.
- The cost of restore-on-open for the real ledger (~1,500 events today)
  was not measured; `probatio_gesta` restores smaller fixtures.
- How the MCP daemon (`tabulariumd`) behaves when its tree is closed —
  it is only ever run from the main tree today (the session's MCP
  config), so D1 should not affect it, but no test says so.
- Whether other writers bypass `gesta_scribere` (e.g. `forum.jsonl`,
  the forum ledger, has its own annals — same pattern? not read).
