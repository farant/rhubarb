# Arbor M2 §3 — canon projection: Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: use
> superpowers:executing-plans (inline). Steps use checkbox (`- [ ]`)
> syntax for tracking.

**Goal:** silva's C89 grammar projects to a generated `.canon`, and
every arbor document in both corpus tiers is judged clean by it — with
canon first taught the two things it needs to judge such a document
honestly (a uniqueness rule that cannot be inert, and transclusion
judged transparently).

**Architecture:** Three independent pieces. T1 and T2–T3 are changes to
`lib/canon.c` that mention nothing about silva and benefit the four
existing dialects. T4–T5 add a `.canon` emitter to silva's *generator*
(not a new tool that re-parses the grammar) because the generator
already holds the symbol↔locus pairing materialised. T6–T7 are the
gates.

**Tech stack:** C89 under the house flags; `credo` for tests; the
canon engine's existing load-time refusal idiom.

**Spec:** `project-specs/arbor-canon-spec.md`. Read §0.6 and §3.1
before T1, §0.4/§0.5 and §3.2 before T2, §0.1 and §4 before T4, §0.2
and §4.3 before T6.

---

## Global Constraints

- C89 only. House flags: `-std=c89 -pedantic -Wall -Wextra -Werror
  -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes
  -Wmissing-prototypes -Wwrite-strings -Wno-long-long
  -Wno-overlength-strings`. The real list is in `compile_tests.sh`,
  not root CLAUDE.md.
- Latin identifiers, comments, messages. **Every word `#define`d in
  `latina.h` is a forbidden identifier** — `nomen`, `registrum`,
  `magnitudo`, `structura`, `casus`, `exire`, `brevis`. Use `titulus`.
  Run `./silva/censor.sh <file>` on every new file.
- `i32`/`i64` are **UNSIGNED**; use `s32`/`s64` for anything signed.
  `chorda` is **not** null-terminated.
- **Never write `*/` inside a block comment.** It terminates the
  comment and the error surfaces lines later as nonsense. Cost one
  build during spec research; write "star" in prose instead.
- **`#` at the start of a word inside a shell argument list terminates
  the command.** Frameworks and files after it vanish silently.
- Refusals are loud. A canon-level defect returns NIHIL and sets
  `*causa`; a document-level defect calls `vitium_addere`. Never a
  silent skip.
- **Every new refusal gets a PLANTED FAULT in the same commit.** A gate
  nobody has watched fail is a gate nobody knows works. This is the
  house law that this whole project exists to apply to arbor documents.
- Build/test: `./compile_tests.sh <filter>`. **Exit 2 = nothing ran.**
  **After editing anything in `include/`, the first run rebuilds the
  nexus graph (~135s) and exits 0 having run NOTHING — run it twice.**
  Tell: exit 0 with no `CREDO COMPENDIUM` line.
- **Never gate on a piped exit status.** `cmd | head` reports head's
  status. Redirect to a file, capture `$?`, then read the file. A
  stale binary reporting old results is the failure this prevents —
  it happened twice during spec research.

---

## File Structure

| file | responsibility |
|---|---|
| `lib/canon.c` | **modify** — T1 load-time reference check; T2 fragment collection; T3 transparent judgment |
| `include/canon.h` | **modify** — T3 may add a vitium genus for an unresolvable transclusion |
| `canon.canon` | **modify, T3** — declare transclusion support for the canon dialect (canon-gated per spec §1.2) |
| `probationes/probatio_canon.c` | **modify** — planted faults + controls for T1 and T3 |
| `silva/instrumenta/silva_generare.h` | **modify, T4** — the derived genus-set structure |
| `silva/instrumenta/silva_coquere.c` | **modify, T4/T5** — derivation + `.canon` emitter, sibling to the registry emitter at `:691-731` |
| `silva/generare.sh` | **modify, T5** — write the generated canon as a committed artifact |
| `silva/c89.canon` | **new, generated, committed** — the projection output |
| `tools/silva_canon.sh` | **new, T6** — freshness gate, `natura_canones.sh` contract |
| `silva/probationes/probatio_silva_canon.c` | **new, T7** — corpus judgment, both tiers |

**Note on where the derivation lives.** Not a new tool re-parsing
`silva/grammatica/c89.stml`. `SilvaGenProductio.loci` is an `Xar`
**parallel to** `dextrum` (`silva/instrumenta/silva_generare.h:52-62`) —
the symbol↔locus pairing is already in memory inside the generator. The
registry emitter (`silva_coquere.c:691-731`) is the sibling to copy.

---

## T1 — A uniqueness rule that cannot be inert

Spec §3.1. Closes defect 01M0MY50CP.

- [ ] **Step 1: Read the seams.** `lib/canon.c:10-95` (the internal
  structs — note `super` and `ad_elementa` are already `Xar` of
  `chorda*`, so no string splitting is needed), `:1314-1344` (the
  load-time refusal idiom: set `*causa`, return NIHIL), and
  `canon.canon:8-20` (the four pathologies, of which this is IV).

- [ ] **Step 2: Write the failing tests first — three faults, three
  controls.** In `probationes/probatio_canon.c`. Each fault must refuse
  AND each control must load, or the test proves nothing:

  | fault | must |
  |---|---|
  | `<unicitas attributum="non-tale" super="item">` | refuse |
  | `<citatio attributum="non-tale" super="item" ad="item/a">` | refuse |
  | `<citatio attributum="a" super="item" ad="item/non-tale">` | refuse |
  | **`<citatio attributum="a" ad="*/non-tale">`** | **refuse** — the star must not become a blanket exemption |
  | same four with real attribute names | **load clean** |

- [ ] **Step 3: Run them, confirm they fail** the way you expect — the
  refusals not yet refusing. Read the first failure specifically; it may
  be failing for a different reason than the one under test.

- [ ] **Step 4: Implement.** A pass at the end of `canon_ex_nodo`, after
  elements and rules are both loaded (it must be after both — a rule may
  precede its elements in document order). For each of the three
  references in spec §3.1's table, ask: does some element in scope
  declare this attribute?

  Scope resolution, exactly:
  - empty scope list → any element in the canon;
  - a scope entry of `*` → **any element in the canon** (spec §3.1);
  - otherwise → elements whose title matches an entry, taking the part
    after a solidus for the `parens/titulus` form;
  - `intra=`-scoped definitions count.

  On failure set `*causa` naming the rule and the attribute, return
  NIHIL.

- [ ] **Step 5: Run the tests, confirm all six pass.**

- [ ] **Step 6: The regression bar — judge the whole corpus.**
  Every `.canon` in the repo must still load.

  ```sh
  find . -name "*.canon" -not -path "*/build/*" -not -path "./gesta/*" \
    | sort > /tmp/canones.lst
  # judge each; expected: all load, zero refusals
  ```

  **Expected: 41 canons, 2,046 rules, zero refusals.** Measured
  2026-08-22 with all three references checked and `*` star-aware. If
  the count of canons or rules has moved, say so rather than adjusting
  the number silently.

- [ ] **Step 7: Commit.**

```bash
git add lib/canon.c probationes/probatio_canon.c
git commit -m "canon: unicitas/citatio attributum ignotum est vitium (porta muta clausa)"
```

**Why the star matters, in one line:** the first version of this rule
produced 470 refusals in `natura/cocta/individua.canon`, all
`ad="*/nomen"`, because it read `*` as an element title. The corpus was
right and the rule was wrong.

---

## T2 — Fragment collection *(SHIPPED — scope corrected during execution)*

Spec §3.2, first half. Follows `augmentum`'s two-pass structure.

> **PLAN CORRECTION, made while executing (2026-08-22).** As written,
> T2 was *collection only* — and a collection pass whose sole consumer
> arrives in T3 **cannot carry its own gate**, which violates this
> plan's own rule that a task ends in an independently testable
> deliverable. Rather than write a hollow test, T2 absorbed the three
> things that are observable without any judgment machinery:
>
> - **exclude fragments/transclusions from the ordinary descent**
>   (moved up from T3 step 4.1) — kills the false vitia;
> - **orphan transclusion → loud vitium** (moved up from T3);
> - **duplicate fragment id → loud vitium**.
>
> That leaves T3 as purely *judge the resolved content transparently*
> (`parens_vi` + inline equivalence), which is a cleaner boundary:
> **T2 makes canon honest about fragments, T3 makes it thorough.**
> Orphan detection landing sooner is a straight gain.
>
> **Measured before/after** (`bin/canon_examen`, toy dialect):
>
> | case | before | after |
> |---|---|---|
> | valid fragment + transclusion | **2 false vitia** | 0 |
> | orphan `<<#nusquam>>` | **0, exit 0 (silent)** | 1, exit 1 |
> | duplicate fragment id | 4 false, 0 about the duplicate | 1 correct |
> | use *before* definition | — | 0 (two passes work) |
>
> New vitium genera: `CANON_TRANSCLUSIO_IRRITA`,
> `CANON_FRAGMENTUM_GEMINUM`. `probatio_canon` 243/243; full suite
> 136/137 (`planta_lectio` pre-existing, reproduced at HEAD); 41 canons
> load; **no judged document in the repo uses fragments**, so this is a
> no-op on the existing corpus.

- [ ] **Step 1: Read the precedent.**
  `_augmenta_cusasque_colligere` (`lib/canon.c:1624-1686`) — one walk
  building both the node list and the mint table, descending into
  augmentations so nested ones are collected in their own right. Note
  why two passes exist: **a definition may legally appear after its
  use in document order.**

- [ ] **Step 2: Write the failing test.** A document with a
  transclusion whose fragment is defined *later* in the document must
  resolve. If collection and judgment were one pass, this fails.

- [ ] **Step 3: Run it, confirm it fails.**

- [ ] **Step 4: Implement the collection pass.** Walk the document once,
  building a table from `fragmentum_id` to the defining node. A fragment
  node is `genus == STML_NODUS_ELEMENTUM` with `titulus == "#"` and
  `fragmentum_id` set (`lib/stml.c:2216-2224`). Descend into fragments —
  they nest and may contain transclusions.

  **Duplicate ids must refuse**, not last-wins: two definitions of one id
  make every use ambiguous.

- [ ] **Step 5: Run, confirm pass. Commit.**

```bash
git add lib/canon.c probationes/probatio_canon.c
git commit -m "canon: fragmenta ante iudicium colliguntur (mos augmenti)"
```

---

## T3 — Transparent judgment

Spec §3.2, second half. **The load-bearing tranche.**

- [ ] **Step 1: Read the precedent's trap.**
  `_augmentum_iudicare` (`lib/canon.c:1693-1842`), specifically the
  `parens_vi` argument at `:1838-1839` and `_elementum_quaerere_vi`
  (`:849-888`). The worklog (`canon.worklog.md:349-368`) records that
  omitting the semantic-parent override was a real bug found on first
  corpus contact that **all eight unit fixtures missed.** This tranche
  repeats that bug unless the override is carried.

- [ ] **Step 2: Write the failing tests. The first one IS the
  definition of transparent:**

  | test | expectation |
  |---|---|
  | content X transcluded into position P | **same verdict** as X written inline at P |
  | ...where X is *illegal* at P | both produce the same vitium |
  | orphan `<<#nothing>>` | loud vitium, never silent |
  | fragment transcluding itself | refuses, does not recur |
  | transcluded content whose definition is `intra=`-scoped | resolves against the position of USE |
  | a fragment definition sitting where its content would be illegal | **no** vitium — judged through uses, not in place |

  The last two are the ones that will actually break. Write them first.

- [ ] **Step 3: Run, confirm they fail.**

- [ ] **Step 4: Implement.** Three changes, mirroring augmentum:
  1. **Exclude fragments and transclusions from the ordinary descent** —
     a parent's content model never declares them
     (`lib/canon.c:2100-2107` is the precedent). This alone removes the
     false `LIBERUM_ILLICITUM` on `titulus == "#"`.
  2. **At a transclusion node**, resolve via T2's table; on failure emit
     a loud vitium (`CANON_CITATIO_IRRITA` or a sibling).
  3. **Judge the resolved content by re-entering `nodum_iudicare` with
     `parens_vi` = the transclusion's semantic parent.**

- [ ] **Step 5: Run all tests, confirm pass.**

- [ ] **Step 6: Canon-gate it.** Declare transclusion support for the
  canon dialect in `canon.canon`; the four data dialects do not get it
  (spec §1.2 — code documents may be languages, censuses may not).

- [ ] **Step 7: Regression — all 41 canons still load, corpus still
  clean.**

- [ ] **Step 8: Commit.**

```bash
git add lib/canon.c include/canon.h canon.canon probationes/probatio_canon.c
git commit -m "canon: transclusio pellucida - contentum iudicatur, involucrum non"
```

---

## T4 — The derivation: grammar to genus sets

Spec §0.1 and §4.2.

- [ ] **Step 1: Read the structures.**
  `silva/instrumenta/silva_generare.h:43-46` (`SilvaGenLocusMappa`),
  `:52-62` (`SilvaGenProductio` — `loci` is parallel to `dextrum`),
  `:69-72` (`SilvaGenGenusExtra`, carried as an uninterpreted string),
  `:85-88`, `:106-111` (`SilvaGenGrammatica`).

- [ ] **Step 2: Write the failing test — pin the numbers.** The
  derivation must resolve **165 (genus, locus) pairs across 53 genera
  with zero residue** (58 genera − 5 extra; 176 locus entries − 11
  extra-genus). Assert the totals AND this specific set, which was the
  case that decided the design:

  ```
  definitio-functionis / declarator
    → {declarator-aciei, declarator-functionis, declarator-monstrator,
       declarator-titulus, parenthesis}
  ```

  `declarator-abstractus` must be **absent** — it belongs only to
  `parametrum/declarator` and `species-typi/declarator`.

- [ ] **Step 3: Run, confirm fail.**

- [ ] **Step 4: Implement the closure.** For a symbol `N`:
  terminal → `{TOKEN N}`; production with `genus="G"` → `{NODUS G}`;
  production with `modus="lista-*"` or empty RHS → `{LISTA N}`;
  single-atom production without `genus=` → **recurse (pass-through)**;
  cut cycles (left-recursive expression ladders).

  Then for each production carrying `genus="G"`, for each atom `S@L`:
  `tabula[G][L] |= genera(S)`. A second pass unrolls list rules: atoms
  bound to `@lista`/`@lista+` recurse into the list nonterminal, others
  contribute element genera.

- [ ] **Step 5: Run, confirm 165/165 and the `declarator` set.**

- [ ] **Step 6: Commit.**

```bash
git add silva/instrumenta/silva_generare.h silva/instrumenta/silva_coquere.c
git commit -m "silva: clausura generum per locum ex grammatica (CLXV/CLXV)"
```

---

## T5 — The emitter

Spec §4.2.

- [ ] **Step 1: Read the sibling.** `silva_coquere.c:691-731`, which
  emits `%s_LOCI[]`, `%s_GENERA[]`, `%s_REGISTRUM`. Copy its shape.
  Follow natura's split: **the model decides what may be said, the
  emitter only writes what the model already says.**

- [ ] **Step 2: Write the failing test.** The generated canon must be
  judged clean by `canon.canon` — the schema of schemas judging the
  generated schema. That is the cheapest real gate available here.

- [ ] **Step 3: Run, confirm fail** (no file yet).

- [ ] **Step 4: Implement.** Emit, per spec §4.2:
  - 58 genus elements, tags verbatim from `SilvaTabGenus.titulus`;
  - locus elements declared **`intra=` their genus** — mandatory, since
    62 names cover 176 entries and a locus tag means something only
    relative to its parent genus;
  - content models from T4, with **every locus `minimum="0"`** (absent
    loci are legitimate: `definitio-ansi` leaves locus 2 NIHIL) and
    `maximum="1"` for NODUS/TOKEN/INDEX, unbounded for LISTA_*;
  - **`ambiguus` admitted in every node-bearing locus and list** — the
    GLR fabrica may wrap any reduction value;
  - 95 `lex-*` elements via the same mangling the writer uses
    (`silva_arbor.c:389-411`), so the two cannot diverge; text content
    only where `ORTHOGRAPHIAE` says so;
  - the hand-authored envelope: `parsura`, `arbor`, `fontes`, `fons`,
    `cauda`, `directiva`, `regio-*`, `invocatio-vacua`, the three trivia
    tags, and the six origin tags;
  - `<citatio>` for `def-f`, `f`, `regio-fons`, `fons-princeps` → `fons/index`;
  - `<unicitas attributum="index" super="fons">`.

  **Hand-authored policy, not derived:** the five `<genera-extra>`
  (`error`, `ambiguus`, `conditionalis`, `ramus-sumptus`,
  `ramus-omissus`) have no derived constraints for their 11 loci. Spec
  §0.1 caveat 2 (ERROR/CONDITIONALIS root-level only) is **INFERRED** —
  encode it as policy and let T7 falsify it.

  Emit a `GENERATUM — NE MANU EDITES` header.

- [ ] **Step 5: Wire into `silva/generare.sh`; commit the artifact.**

- [ ] **Step 6: Run, confirm `canon.canon` judges it clean.**

- [ ] **Step 7: Commit.**

```bash
git add silva/instrumenta/silva_coquere.c silva/generare.sh silva/c89.canon
git commit -m "silva: canon c89 generatus ex grammatica"
```

---

## T6 — Its own seal, and freshness

Spec §0.2 and §4.3. **Do not skip on the grounds that `registrum-sigillum` exists.**

- [ ] **Step 1: Enumerate the real derivation inputs.** Spec §7 leaves
  this deliberately to the implementer: build the seal from the tables
  the derivation *actually reads*, not from the spec's guess. At minimum
  the grammar productions, `NOMINA_GENERUM`, and `ORTHOGRAPHIAE` —
  `silva_arbor_sigillum` (`silva_arbor.c:149-235`) covers **none** of
  these, which is exactly the hole.

- [ ] **Step 2: Write the failing test — the planted fault at birth.**
  Change a production; the freshness check must fail. This test is the
  entire point of the tranche: §0.2 exists because a seal was trusted
  without anyone watching it fail.

- [ ] **Step 3: Run, confirm fail.**

- [ ] **Step 4: Implement the seal and `tools/silva_canon.sh`**, copying
  `natura_canones.sh`'s contract exactly:
  - `-probare`: 0 fresh / 1 stale / 2 nothing judged. **Never 3.**
  - bare (regenerates): 0 whole chain fresh / 2 defect / **3 = canon
    written but downstream now stale**; 3 also when the downstream gate
    could not run — *unknown is never reported as 0.*

- [ ] **Step 5: Run; confirm the planted fault fails and a fresh tree
  passes.**

- [ ] **Step 6: Commit.**

```bash
git add silva/instrumenta/silva_coquere.c tools/silva_canon.sh
git commit -m "silva: sigillum canonis proprium + porta recentiae (exitus III)"
```

---

## T7 — The corpus gate

Spec §5. **The real gate.**

- [ ] **Step 1: Write the falsification FIRST.** Before any green sweep
  is trusted: take a valid arbor document, move a node into an illegal
  locus, and confirm a vitium. A sweep from an unfalsified gate measures
  nothing — this is why the T1 corpus run planted a fault before running
  41 files.

- [ ] **Step 2: Write the sweep.** Generate arbor documents from both
  tiers and judge every one: 73 plain-C files in
  `probationes/fixa/roundtrip/`, then the latinized `lib/` tier.

- [ ] **Step 3: The apparatus refusal** (inherited from
  `arbor-parsura-spec.md` §6.5). The latinized tier needs the include
  closure; without it nothing expands and every number is confidently
  wrong. **Refuse loudly when a file transitively including `latina.h`
  yields zero EXPANSIO tokens.** This has already failed twice — it is a
  gate, not a habit.

- [ ] **Step 4: Run. Expected: zero vitia, both tiers.**

  If there are vitia, **judge by the cause tally, not the total** — a
  headline count can hold steady while the reasons underneath change
  completely. Most likely causes, in order: the `<genera-extra>`
  hand-authored policy (T5), the INFERRED root-level-only claim for
  ERROR/CONDITIONALIS, and T4's cycle-cutting.

- [ ] **Step 5: Pin the numbers** so a future change must be announced,
  and commit.

```bash
git add silva/probationes/probatio_silva_canon.c
git commit -m "silva: porta canonis super corpus utrumque (LXXIII + latinizata)"
```

---

## Deferred, with landing spots (no silent caps)

| deferred | landing |
|---|---|
| composite-key uniqueness | two askers on file (`canon.canon:20-27`); **not a dependency** (spec §0.7). Build when a dialect cannot ship without it |
| element groups / substitution classes | if the enumerated form proves unreadable in practice; touches codegen, so measure the pain first |
| co-occurrence constraints | decreed above canon twice; needs that decree reversed, not a third asker |
| generated arbor reader | quaestiones-style differential oracle, if the hand-written reader drifts from the schema |
| judging `<arbor>` subtree documents | T5 emits the tag; whether subtree documents need their own envelope is untouched |
| the other three registries | `sceletum` and `imparilis` (15 genera / 29 loci each). `generare.sh` already loops, so this is near-free once T5 works — but it is not gated here |
