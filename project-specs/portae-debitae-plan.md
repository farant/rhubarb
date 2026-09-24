# Portae debitae v1 — plan

> Inline on main, one task per turn with Fran's approval, no subagents.
> Spec: `project-specs/portae-debitae-spec.md`. Ledger: park "Portae
> debitae v1" in region "automatic calculation of relevant tests"; one
> `opus` per task below, closed by `silva.commissio(opus=ID)`.

> **STATUS 2026-09-24 — ALL FIVE TASKS DONE.** T1 `3f6dfa7d` (machine
> form + `frigida -inventarium`), T2 `ce87f9b5` (`silva.inventarium`,
> `portae_debitae`; dcd516c7 replay both ways), T3 `5f2d383e` (lenses
> filled, Fran looked; live replay also found `frigida` owed), T4
> `aaf1c843` (`tools/portae_debitae.sh`), T5 `deecd624` (commissio adds
> and runs owed gates — committed with NO gates requested; it found
> `pythonica` itself). Costs: aedilis covers lib/include (186 s);
> inventory read 3.3 s (desideratum …JK3Y); pythonica suite 3:13 → 4:39.

**Global constraints.** C89 in Latin via latina.h, house flags; `i32`/`i64`
UNSIGNED; chorda not NUL-terminated; tests with credo (C) or the
pythonica `credo` (Python), every new assertion born red; worklog first,
gates last; commit via `silva.commissio` with explicit paths; formator
`-scribere` then `-vitia`/`-delta HEAD`; new words checked with
`./oratio/quaere.sh`, then `./oratio/vocabula.sh -nova`. Verify every
scripted edit with `git diff` (lesson of cacf090f). Gates: C tasks owe
**gesta AND radix**; Python tasks owe **pythonica**. After a gesta
change: `renovare` the resident before using the live tool.

---

### Task 1: machine form of the table + `frigida -inventarium`

**Files:** `gesta/fontes/tabularium.c` (`_tab_inventarium`: `forma`
argument; the tool-table entry), `gesta/fontes/frigida.c` (form
`-inventarium` → instrumentum `inventarium`, a read), `gesta/frigida.sh`
(usage line), `gesta/probationes/probatio_tabularium.c`,
`gesta/probationes/probatio_frigida.c`, `gesta/gesta.worklog.md`.

**Produces:**

    inventarium {res, actus: "tabula", forma: "machina"}
        -> one line per PRESENT cell, rows in table order then lenses in
           table order:  ordo \t lens \t genus \t valor \n
           (in ordo and valor: '\' -> "\\", tab -> "\t", newline -> "\n")
    ./gesta/frigida.sh -inventarium <res>     (same text; writes nothing)

**Tests:** exact machine output for the existing fixture (`Iv suitae`),
including a `non-applicabile` cell and a value holding a tab; `forma`
unknown refused with the legal forms named; frigida: verb known, output
contains the rows, annals unchanged (read writes nothing), unknown res =
refused exit, superfluous operand = usage exit. **Done when** gesta and
radix are green and the live `./gesta/frigida.sh -inventarium 'suitae
probationum'` prints the 35 rows.

### Task 2: `silva.inventarium` + `silva.portae_debitae` (Python core)

**Files:** `pythonica/silva.py`, `pythonica/probatio_silva.py`,
`pythonica/pythonica.worklog.md`.

**Produces:**

    inventarium(res) -> Inventarium(ordines: [clavis], lentes: [nomen],
                                    cellae: {clavis: {lens: (genus, valor)}})
        # via _frigida_imperium(['-inventarium', res]); unescapes \\ \t \n;
        # SilvaError naming the frigida exit when the read fails
    Debitum = namedtuple('Debitum', 'porta causa manu')
    portae_debitae(viae, inventarium_res='suitae probationum')
        -> (debita: [Debitum] in PORTAE order, intecta: [via])

Coverage per spec §2: (a) for each row whose `porta` names a gate in
`SUITAE`, the tests `<dir>/probatio_*.c` and their `bin/aedilis <t>
--partes` closures (O/C/V lines), run 4 at a time with subprocess (no
threads needed: Popen + wait); (b) `currit binaria` — `.c` entries by
closure, other paths literally; (c) `tegit viae` — `fnmatch` with `**`.
A row with `manu tantum` = `ita` yields `manu=True`; its gate is the
row's script itself when `porta` is n.a. `causa` names the chain (file →
test or entry → gate). Paths normalised relative to RADIX.

**Tests (born red):** a fake inventory (substitute `FRIGIDA_IMPERIUM`,
as `opus_praeiudicare`'s tests do) holding the root row with and without
`currit binaria = gesta/instrumenta/tabulariumd_principale.c`; the
**dcd516c7 replay** (spec §5) — without the cell radix is absent, with
it radix is present and its causa names `tabulariumd_principale.c`;
`gesta` owed by compile closure in both; a `tegit viae` glob hit; a
`manu` row; an uncovered path in `intecta`; unescaping round trip.

### Task 3: the two lenses, filled (data, reviewed by Fran)

**Files:** `tools/inventarium_suitarum.py` (mode `currit`: propose
runtime edges by scanning the tests' `system(`/`processus_*` call
arguments with `./silva/selecta.sh` for the call sites, keeping only
arguments that name a script under the tree, a `bin/` program or a
`*/build/` binary, and mapping a binary to its entry `.c` where aedilis
or the builder names one; fixture strings dropped).

Ledger: `lens` `currit binaria` and `tegit viae` (textus); push the
proposed `currit binaria` cells with `fons: derivatum`; write `tegit
viae` by hand for all 31 gates plus the GUI rows (each fumus script
covers at least itself and the tool it drives); n.a. where a gate truly
has no such input. **Done when** both lenses' frontiers read 0/35, the
dcd516c7 replay passes LIVE through the real inventory, and Fran has
looked at the two columns (`tabula`).

### Task 4: `tools/portae_debitae.sh` (the query)

**Files:** `tools/portae_debitae.sh` (thin: `python3` into
`silva.portae_debitae`; no args = `git diff --name-only HEAD` + staged
+ untracked-but-not-ignored), `pythonica/probatio_silva.py` (runs the
script), `pythonica/README.md` (usage).

Output: one block per owed gate (`porta` + indented causae), then
`manu:` gates, then `intecta:` paths. Exit 0 judged, 2 nothing judged
(inventory unreadable). **Tests:** live-tree invariant — the query for
`gesta/fontes/tabularium.c` names gesta and radix; a `.md` path lands in
`intecta`; a bad inventory title exits 2.

### Task 5: `commissio` and `commissio_umbra` add and run owed gates

**Files:** `pythonica/silva.py`, `pythonica/probatio_silva.py`,
`pythonica/README.md`, `pythonica/pythonica.worklog.md`; memory bench
line after the commit.

Before the gates: `debita = portae_debitae(viae)`; effective gates =
requested ∪ owed (PORTAE order, requested filters kept); one line per
ADDED gate `porta debita addita: <porta> — <causa prima>`; `manu` gates
printed as `manu debita: ...`, never run; `sine_debitis='<cause>'`
skips the addition and prints the cause (empty cause = SilvaError). An
unreadable inventory prints a warning and proceeds with the requested
gates (warn-only v1: never block a commit on this machinery).
**Tests:** substitute `portae_debitae` in-process and the gates via
`PYTHONICA_PORTAE_FICTAE` — an owed gate absent from `portae` is run and
named; `sine_debitis` skips it and prints the cause; a manual gate is
printed, not run; broken inventory → warning, commit proceeds.
**Done when** the pythonica gate is green and a real commit through
`commissio` shows the added-gate line.
