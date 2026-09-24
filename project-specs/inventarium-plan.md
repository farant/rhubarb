# Inventarium v1 — plan

> Inline on main, one task per turn with Fran's approval, no subagents.
> Spec: `project-specs/inventarium-spec.md`. Ledger: park "Inventarium v1"
> in region "inventory management for batch processes"; one `opus` per
> task below, closed by `silva.commissio(opus=ID)`.

**Global constraints.** C89 in Latin via latina.h, house flags; `i32`/`i64`
UNSIGNED; chorda not NUL-terminated; new C files via `scribe`; tests with
credo, born red by a planted fault; worklog first, gates last; commit via
`silva.commissio` with explicit paths; formator `-scribere` then `-vitia`;
new words checked with `./oratio/quaere.sh`. Gates for every task:
**gesta AND radix** (probatio_cliens_tabularii lives in the root suite —
lesson of e0988825).

> **STATUS 2026-09-23 — planned.** Nothing built.

---

### Task 1: `gesta_inventarium` — the pure fold and validator

**Files:** create `gesta/fontes/gesta_inventarium.h`, `.c`,
`gesta/fontes/gesta_inventarium.worklog.md`,
`gesta/probationes/probatio_gesta_inventarium.c`; register the probatio
in `gesta/compile_probationes.sh` (and the generated source list if the
runner derives it).

**Interfaces (produces):**

    /* VERUM = eventus inventarii (ordo-additus, ordo-remotus,
     * lens-addita, cella-posita) */
    b32 gesta_inventarium_eventus_est (chorda genus_eventus);

    /* statum (obiectum JSON rei) per eventum mutare - PURUM.
     * actor/creatum = provenientia cellarum */
    b32 gesta_inventarium_applicare (JsonValor* status, chorda genus_eventus,
            JsonValor* datum, chorda actor, chorda creatum, Piscina* pn);

    /* NIHIL = licet; aliter causae OMNES (lineae) contra statum currentem */
    constans character* gesta_inventarium_validare (JsonValor* status,
            chorda genus_eventus, JsonValor* datum, Piscina* pn);

**Tests:** fold each kind on an empty state; order kept; a cell after its
row's removal is gone; provenance copied from actor/creatum; every
refusal in spec §3 named, and two bad cells in one event give two causes.
**Done when** the probatio is green and a planted fault (e.g. fold drops
`fons`) turns it red.

### Task 2: wire into gesta — fold, write-time validation, seeded genus

**Files:** `gesta/fontes/gesta.c` (`_statum_transformare` delegates the
four kinds; the preparation/validation step calls
`gesta_inventarium_validare` against the entity's current state);
`gesta/fontes/tabularium.c` (seed genus `inventarium`, no state machine);
`gesta/probationes/probatio_gesta.c`.

**Done when** events written through `gesta_scribere` fold into the
entity (read back via the state), a bad cell event is REFUSED with its
causes and nothing written, and replay (rebuild from annales) reproduces
the same table byte for byte.

### Task 3: MCP tool `inventarium` (write actions)

**Files:** `gesta/fontes/tabularium.c` (tool table entry — the count is
derived, ARGUMENTORUM_NUMERUS; handler `_tab_inventarium`; dispatch);
`gesta/probationes/probatio_tabularium.c`.

Actions `ordines` (present keys reported and skipped; new keys in one
event), `lens`, `cellae` (`fons` default `manu`), `removere` (causa
required). Resolution of `res` by id/fragment/title as elsewhere; branch
writes refused.

**Done when** each action works through tools/call, refusals name all
causes, and re-pushing the same rows is idempotent (reports, writes
nothing).

### Task 4: `tabula` view and the frontier

**Files:** `gesta/fontes/tabularium.c`, `probatio_tabularium.c`.

Render rows × lenses (`ita` → `+`, `non` → `-`, unknown → `·`, text
truncated to fit), then a footer: per lens, unknown count and the oldest
and newest cell dates. Deterministic output (no clock), so the tabula.md
projection may include it later.

**Done when** a fixture inventory renders exactly (asserted), and the
frontier counts match.

### Task 5: first inventory — test suites (acceptance)

**Files:** a small derivation outside the MCP server (e.g.
`tools/inventarium_suitarum.py`: runner-shaped scripts from `git ls-files`
+ `PORTAE` membership from `pythonica/silva.py`), printing JSON to push;
no C.

Create the inventory in the region, push rows (`ordines`), add lenses
`in PORTAE` (derived), `GUI aut manu` and `tegit` (by hand), fill what is
known. **Done when** `tabula` shows the 7 unregistered scripts with a
reason each and the frontier lists what remains — and Fran has looked at
it.
