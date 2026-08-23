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

## T3 — Transparent judgment *(SHIPPED)*

Spec §3.2, second half. **The load-bearing tranche.**

> **SHIPPED 2026-08-22.** Implemented as an **effective child list**:
> `_liberos_effectivos` builds, per node, the child list with every
> transclusion already replaced by its fragment's content. All three
> walks (cardinality, licentia, recursion) then read that one list, so
> they cannot diverge — the alternative was teaching each walk about
> transclusion separately and hoping three answers agreed.
>
> **Both warned-of traps fired, and both were caught by measurement:**
>
> 1. **The equivalence test failed first time.** Transcluded gave 0
>    vitia where inline gave 2 — because I converted the *cardinality*
>    loop to the effective list and left the *licentia* loop walking
>    raw children while using the effective count. An index mismatch on
>    top of a miss. The test that *defines* transparent is what caught
>    it; a weaker test would have passed.
> 2. **`parens_vi` is load-bearing, measured.** With the override
>    reverted to `NIHIL`, the `intra=` case yields **VITIA 1**; with it,
>    0. The augmentum bug reproduced deliberately rather than assumed.
>
> **A bug my own restructure introduced:** moving fragment collection
> ahead of judgment left the T2 collection call still in place, so the
> table was built **twice** and every duplicate id would have been
> reported twice. Caught by re-reading the call site, not by a test —
> worth a test if this area is touched again.
>
> Also added: `CANON_TRANSCLUSIO_CIRCULARIS`. A fragment transcluding
> itself clamours once and terminates instead of expanding forever.
>
> `probatio_canon` 259/259; full suite 136/137 (`planta_lectio`
> pre-existing, same line/values); 41 canons load.

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

## T5 — The emitter *(SHIPPED 2026-08-22)*

Spec §4.2.

> **EXECUTION NOTES — five things that changed T5's shape. Read
> before T6/T7.**
>
> **(a) The emitter lives in a NEW file, not `silva_coquere.c`.**
> `silva/instrumenta/silva_canon_coquere.{c,h}`. `silva_coquere.c` was
> already ~900 lines and the canon projection is a different output
> format over the same model, not a variation on table baking.
> `generare.sh` no longer globs `instrumenta/*.c` (see (b)), so the new
> file is named explicitly in the generator's closure list.
>
> **(b) `./silva/generare.sh` was ALREADY BROKEN at HEAD — 83
> undefined symbols, exit 1.** Not caused by T5; confirmed by stashing
> every T5 change and re-running. The script linked *every*
> `instrumenta/*.c` into the generator with a hand-maintained **deny**
> list (`silva_amalgama`, `nexus_ordines`). Three dev tools have since
> joined instrumenta and reference runtime (`fontes/`) symbols —
> `silva_differre`, `silva_formator`, `silva_iudicium` — and nobody
> added them to the deny list. Its own comment already called the
> `nexus_ordines` case *"fractura latens"*; this is the same wound,
> three times over.
>
> **Fixed by inverting to an ALLOW list** (`GENERATOR_FONTES`): the
> generator's real closure is three files. A deny list rots every time
> someone adds a tool; an allow list cannot, because a new tool has to
> be named to enter the link. Missing source now exits 1 by name.
>
> **(c) The root genus set is NOT derivable from the impletiones
> table** — a genuine gap T4 could not have seen. Impletiones are
> minted per production carrying `genus=`, and the start rule
> (`elementa`, `<initium>elementa</initium>`) is **pass-through**: it
> mints no genus, therefore no `(genus, locus)` pair, therefore no row.
> But `<parsura>` carries top-level nodes as **direct children** (the
> writer deliberately does not wrap them in a list element, so document
> order equals file order), so that vocabulary is real and had to be
> computed separately: `silva_gen_genera_radicis_computare`, reusing
> T4's `_genera_symboli` closure on `initium_index`. It **refuses
> loudly** if the set comes back empty — an empty root would generate a
> canon that rejects every document, with the cause invisible.
>
> **(d) Two restatements, two asserted equalities — deliberately NOT
> shared code.** The emitter cannot call `silva_arbor_lexema_tag` or
> `silva_arbor_valor_portandus`: `silva_arbor.c` pulls in
> `silva_commissio`/`stml`/`friatio`/`tabula_dispersa`, and linking it
> would make the **generator depend on the engine it generates tables
> for** — a direction reversal, not a cost. So the emitter restates the
> `lex-` mangling and the text-bearing set, and
> `probatio_silva_canon` asserts both against the arbor side across all
> 95 genera. **Calibrated**: adding `SILVA_LEX_VOID` to the emitter's
> text-bearing set fails the suite with `TEXTUS DISCREPAT: lex-void
> (arbor 0, canon 1)`. A named failure beats silent sharing.
>
> `silva/fontes/silva_token.c` IS linked into the generator — it is a
> leaf (latina/piscina/chorda/xar, all already present), so the genus
> **names** cost nothing. Only the engine was refused.
>
> **(e) Spec §0.3's bare-attribute typing was wrong** — `genus="veritas"`
> rejects `"true"`. See the correction block in the spec. The
> projection uses `electio` + `<optio>true</optio>`.

**Shipped shape** — `silva/c89.canon`, 6,178 lines, 346 elements:

| layer | count | source |
|---|---:|---|
| `lex-*` | 95 | `SilvaLexemaGenus` enum |
| locus elements (`intra=` their genus) | 176 | registry + T4 vocabulary |
| genus elements | 58 | registry |
| envelope | 17 | hand-authored policy |

`<citatio>` × 4 (`f`, `def-f`, `regio-fons`, `fons-princeps` → `fons/index`),
`<unicitas attributum="index" super="fons">` × 1.
Registered in `canones.registrum` by **root element** `<parsura>`;
`<arbor>` (subtree documents) deliberately absent, per spec §6.

**Gates run, each falsified before being believed:**

| gate | result | falsification |
|---|---|---|
| `canon.canon` judges the projection | 0 vitia | 159 planted `genus="nmuerus"` → 159 named vitia |
| derived model actually constrains | — | `declarator-abstractus` in `definitio-functionis/declarator` → 3 vitia; `declarator-titulus` → clean |
| roundtrip corpus (73 docs) | 0 vitia | same probe, via the registry route |
| `probatio_silva_canon` | 115/115 | planted emitter divergence → fails by tag name |
| silva suite | 48/48 | — |
| `probatio_canon` / `probatio_natura_canones` | 259/259, 207/207 | — |

Generated tables (`fontes/silva_tabulae_*`, `amalgama/silva.h`,
`hospes.c`) came back **byte-identical** to committed — the generator
stayed deterministic across the change.

> **RECONNAISSANCE done 2026-08-22, before implementation. Two findings
> that change T5's shape — read both before writing the emitter.**
>
> **(a) Token tags ARE derivable in the generator.** Terminals declare
> their lexeme genus in the grammar:
> `<terminalis titulus="IDENTIFICATOR" genus="SILVA_LEX_IDENTIFICATOR"/>`
> (`silva/grammatica/c89.stml:41-50`). The arbor tag is
> `"lex-" + NOMINA_GENERUM[genus]` lowercased with `_`→`-`
> (`silva_arbor.c:389-411`), and `NOMINA_GENERUM[SILVA_LEX_X] == "X"`,
> so stripping the `SILVA_LEX_` prefix from `genus=` yields the same
> string **by construction**. Verified against real output:
> `SILVA_LEX_INT` → `lex-int`, `SILVA_LEX_PAREN_APERTA` →
> `lex-paren-aperta`.
>
> *But that equality is an invariant nobody asserts.* If `genus=` and
> `NOMINA_GENERUM` ever drift, the projected canon emits wrong tags
> silently. **`NOMINA_GENERUM` is therefore a T6 seal input** — already
> named in T6 step 1, and this is the concrete reason.
>
> **(b) Trivia tags are NOT in the grammar — a real gap.** `SPATIA`,
> `NOVA_LINEA`, `COMMENTUM_CLAUSUM`, `COMMENTUM_LINEA`, `CONTINUATIO`
> are lexeme genera that never appear as grammar terminals; they ride
> as trivia inside `<ante>`/`<post>`. The grammar declares only the
> terminals it parses, so a purely grammar-driven emitter cannot reach
> them.
>
> Two ways out, decide at the seam: give the generator
> `silva/fontes/silva_token.h` and enumerate the dense
> `SilvaLexemaGenus` range directly (what `silva_arbor.c:431-441`
> already does for the reverse lookup), or hand-author the trivia tags
> with the rest of the envelope. The first is preferable — 95 tags from
> one enum, and it removes a hand-list that could drift.
>
> **RESOLVED — take the enum route.** The dependency I was about to
> price already exists: `silva_generare.c:7` includes `silva_nodus.h`
> from `fontes/`, `generare.sh` already compiles with
> `-I$SILVA_DIR/fontes`, and `silva_differre.c`, `silva_amalgama.c` and
> `silva_iudicium.c` all include `silva_token.h` today. Adding it costs
> nothing and introduces no new direction. (Instance of silva's own
> rule — *price narrowings at the seam, not from memory*: the seam had
> already dissolved the deferral.)
>
> Consequence for §4.2's "95 `lex-*` elements": the grammar yields only
> the parsed subset. The count must come from the enum, not from the
> terminal list.

- [x] **Step 1: Read the sibling.** `silva_coquere.c:691-731`, which
  emits `%s_LOCI[]`, `%s_GENERA[]`, `%s_REGISTRUM`. Copy its shape.
  Follow natura's split: **the model decides what may be said, the
  emitter only writes what the model already says.**

- [x] **Step 2: Write the failing test.** The generated canon must be
  judged clean by `canon.canon` — the schema of schemas judging the
  generated schema. That is the cheapest real gate available here.

- [x] **Step 3: Run, confirm fail** (no file yet).

- [x] **Step 4: Implement.** Emit, per spec §4.2:
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

- [x] **Step 5: Wire into `silva/generare.sh`; commit the artifact.**

- [x] **Step 6: Run, confirm `canon.canon` judges it clean.**

- [x] **Step 7: Commit.**

```bash
git add silva/instrumenta/silva_coquere.c silva/generare.sh silva/c89.canon
git commit -m "silva: canon c89 generatus ex grammatica"
```

---

## T6 — Its own seal, and freshness *(SHIPPED 2026-08-22)*

Spec §0.2 and §4.3. **Do not skip on the grounds that `registrum-sigillum` exists.**

> **EXECUTION NOTES.**
>
> **(a) Step 1 was right to distrust the spec's list.** §4.3 named
> `ORTHOGRAPHIAE` as a seal input. **The emitter does not read it** — it
> *restates* it (T5 note (d)), and `probatio_silva_canon` guards the
> restatement. Sealing a source the emitter never reads would make the
> gate fire on a change that cannot alter the output — the exact
> "*porta quae falso clamat mox neglegitur*" failure `natura_canones.sh`
> documents. Sealed instead: **productions with annotations,
> genera-extra, the genus/locus registry, and `NOMINA_GENERUM`**.
>
> Division of labour, and neither half covers the other:
> - **seal + freshness gate** → *data* drift (grammar, tables, names)
>   leaving a stale artifact
> - **`probatio_silva_canon`** → *code* drift (emitter diverging from
>   the arbor writer)
>
> **(b) The §0.2 hole, demonstrated rather than asserted.** Changed
> `sententia@consequens` → `declaratio@consequens` in `si` (both
> nonterminals, so both project to species NODUS):
>
> | | before | after |
> |---|---|---|
> | genus/locus tables (all `registrum-sigillum` covers) | — | **byte-identical** |
> | `si/consequens` content model | 14 statement genera | 1 |
> | canon seal | `8b92ecb4` | `2ea300ec` |
>
> `registrum-sigillum` would not have moved. This is now a suite
> assertion (§VIII of the probatio), mutating the grammar **in memory**
> so the file on disk is never touched.
>
> **(c) A committed artifact must be invocation-independent — found the
> hard way.** The header embedded the output path and the grammar path,
> both of which vary by caller (`generare.sh` runs from `silva/`, the
> gate from the repo root). Two *correct* regenerations therefore
> disagreed, which would make an output-comparing gate cry wolf on the
> first run. Fixed by emitting **basenames only**. Any generated file
> compared byte-wise needs this property; check for it before building
> the comparison.
>
> **(d) A bug in the gate's own guard list, found by trying to make it
> fire.** I had put the grammar in the "is the binary stale?" guard.
> The grammar is not compiled into the binary, so a grammar edit
> reported **exit 2 (tool stale)** instead of **exit 1 (canon stale)** —
> the gate masking the single case it exists to catch. The guard list
> answers "is the *binary* stale", and only compiled sources belong in
> it.
>
> **(e) Exit 3's honest mapping here.** Nothing is cooked *from*
> `c89.canon` (unlike natura, which has generated readers), so 3 cannot
> mean "downstream now stale". It keeps the other half of the contract —
> *unknown is never reported as 0* — and means **canon written, validity
> unexplored** (`bin/canon_examen` or `canon.canon` absent, or the
> written canon rejected).
>
> **(f) `-canon` mode on the generator** writes only the canon and skips
> LALR construction entirely (the projection reads the grammar, not the
> baked tables). 0.02s, so the gate is cheap enough to run often.

**Gates, each falsified — and note the sequencing trap below:**

| planted fault | expected | got |
|---|---|---|
| grammar edited | 1 + seal **changed** | `8b92ecb4 → a623c699`, "fontes derivationis" |
| canon hand-edited, grammar intact | 1 + seal **identical** | "EMISSOR mutatus, non grammatica" |
| guard source newer than binary | 2, names the builder | ✓ |
| guard source **absent** | 2 (the silent hole natura warned of) | ✓ |
| `bin/canon_examen` absent | **3**, never 0 | ✓ |
| unknown argument | 2, never a silent write | ✓ |

**Sequencing trap, recorded because it nearly passed unnoticed:** the
`touch` from the stale-binary fault was not undone, so the next two
faults hit the stale-binary gate *first*. One of them still returned the
expected exit code — **right answer, wrong reason**. Rebuild between
planted faults, and check the *message*, not just the code.

- [x] **Step 1: Enumerate the real derivation inputs.** Spec §7 leaves
  this deliberately to the implementer: build the seal from the tables
  the derivation *actually reads*, not from the spec's guess. At minimum
  the grammar productions, `NOMINA_GENERUM`, and `ORTHOGRAPHIAE` —
  `silva_arbor_sigillum` (`silva_arbor.c:149-235`) covers **none** of
  these, which is exactly the hole.

- [x] **Step 2: Write the failing test — the planted fault at birth.**
  Change a production; the freshness check must fail. This test is the
  entire point of the tranche: §0.2 exists because a seal was trusted
  without anyone watching it fail.

- [x] **Step 3: Run, confirm fail.**

- [x] **Step 4: Implement the seal and `tools/silva_canon.sh`**, copying
  `natura_canones.sh`'s contract exactly:
  - `-probare`: 0 fresh / 1 stale / 2 nothing judged. **Never 3.**
  - bare (regenerates): 0 whole chain fresh / 2 defect / **3 = canon
    written but downstream now stale**; 3 also when the downstream gate
    could not run — *unknown is never reported as 0.*

- [x] **Step 5: Run; confirm the planted fault fails and a fresh tree
  passes.**

- [x] **Step 6: Commit.**

```bash
git add silva/instrumenta/silva_coquere.c tools/silva_canon.sh
git commit -m "silva: sigillum canonis proprium + porta recentiae (exitus III)"
```

---

## T6.5 — Transclusion in canon files themselves *(SHIPPED 2026-08-22)*

> **EXECUTION NOTES.**
>
> **(a) The loader's silent drop was at `canon.c:1323`** — `si (!n ||
> n->genus != STML_NODUS_ELEMENTUM) perge;`. A TRANSCLUSIO node is not
> an ELEMENTUM, so it fell out there, exactly as the judging path did
> before T3.
>
> **(b) All three T3 helpers were reused unchanged** —
> `_fragmenta_colligere`, `_transclusiones_resolvere`,
> `_liberos_effectivos`. The loader adds one wrapper,
> `_liberos_canonis`, whose only extra job is **skipping fragment
> definitions** (a `<#vocab>` at `<canon>` level is a declaration, not a
> child; without the skip it fell through as an unknown element). No new
> vitium genus was needed.
>
> **(c) FOUR child-walks needed converting, not three.** The plan named
> `<elementum>`, `<attributum>`/`<optio>`, and the `<canon>` top level;
> `claves-externae` → `<clavis>` is a fourth. Leaving it raw would have
> recreated the very defect in miniature — a place where a fragment is
> silently ignored.
>
> **(d) Cycles are caught AFTER the walks, not before.** Only expansion
> can see a cycle (the `in_cursu` stack lives inside
> `_liberos_effectivos`), so collection and resolution cannot. The
> vitia list therefore needs a **second** check at the end of
> `canon_ex_nodo`. Without it, a self-transcluding canon loaded happily —
> this was the one test still red after the first implementation.
>
> **(e) `canon.canon` needed NO change.** T3 already skips fragment
> definitions when judging, and a transclusion resolves at its *use*
> site, where `<liberum>` is legal. So a fragment-bearing canon was
> already judged clean — which was precisely what made the defect
> dangerous, and is why no declaration is required now.
>
> **(f) Factoring is by TEXT comparison, not set comparison.** The
> emitter renders what it *would* emit for a locus and compares bytes
> against the rendered fragment. A false substitution is therefore
> impossible: one byte different and it emits inline. If the grammar
> changes so a locus no longer matches, the result is *less factoring,
> never wrongness*.
>
> **(g) Fragment membership is derived, names come from the grammar.**
> `silva_gen_genera_symboli_computare(g, "expressio")` runs T4's closure
> on a named nonterminal, rendered through the *same* function the loci
> use — so the comparison is exact by construction.

**Result:** `silva/c89.canon` **6,178 → 4,647 lines (−25%)**, 34
transclusions: `lexemata` ×15, `expressio` ×13, `sententia` ×6. More
sites than the census predicted (11/12/6), because exact-render matching
found cases the approximate set census missed.

The readability payoff, and the erased category name restored:

```xml
<elementum nomen="conditio" intra="si">
  <<#expressio>>
</elementum>
```

**Gates:** `canon.canon` clean · corpus 73/73 · `probatio_canon`
**279/279** (was 259; +20) · `probatio_silva_canon` 124/124 with counts
unmoved (95/176/75 — factoring changes content, not element counts) ·
silva 48/48 · natura_canones 207/207 · all 42 canons load ·
`silva_canon.sh -probare` 0 · `canon_coquere.sh -probare` 0.

**Calibrated, not assumed:** a clean corpus would also result from a
canon that admits everything, so the discriminating probes were re-run
against the *factored* canon — `declarator-abstractus` in
`definitio-functionis/declarator` still rejected, and `declaratio` in an
expression slot rejected **through** `<<#expressio>>`, proving the
fragment resolves to a real constraining vocabulary.

**The seal did not move** (`8b92ecb4`), correctly: T6 seals *derivation
inputs*, and this changed only the emitter's output form. That is the
division of labour in (a) of T6 working as designed.

---

**Added 2026-08-22 (Fran, by fork), after T5 measured the repetition.**
Defect `01M0NWBAQMFE5N6X3EZP0AK6RA`. Ordered after T6 deliberately: T6's
seal covers *derivation inputs* (productions, `NOMINA_GENERUM`,
`ORTHOGRAPHIAE`), not the canon's text, so changing the output's shape
here does not invalidate it.

### What is already true (measured, not assumed)

A `.canon` file is STML, so fragments already **parse**. Syntax is
`<#name>…</#>` to define and `<<#name>>` to use — *not* `&name;` (that is
the SIGNA form for attribute values, a different mechanism; getting this
wrong produces a parse failure that looks like "fragments are
unsupported").

| path | transclusion | status |
|---|---|---|
| judging (`canon_iudicare`) | resolved transparently | **T3, shipped** |
| loading (`canon_ex_nodo`) | **silently skipped** | this tranche |

**The failure mode is the worst possible pairing.** `canon.canon` judges a
fragment-using canon **clean (0 vitia)** — correctly, because T3 sees
through the fragment — and then the rules built from it carry an *empty*
content model and reject every document. Green light, wrong behaviour.
This is spec §0.4's mute gate one level up: T3 closed it for documents and
left it open for canon files.

Proof of the diagnosis, reproduce before fixing: one element carrying an
inline `<liberum>` **and** a transcluded one. Inline → clean; transcluded
→ `liberum hic non licet`. That isolates transclusion from every other
cause.

### Why this and not element groups

Transclusion is **textual factoring** — the canon still says "one of these
19", written once. Element groups / substitution classes would be **schema
abstraction**, a new concept in the canon language; spec §6 reserves them
and they touch codegen. Transclusion gets most of the benefit at a
fraction of the cost, and needs no new vitium genera: XVIII–XX (orphan,
duplicate, cyclic) already exist from T3, so the loading path **reuses**
T3's collection rather than inventing a second mechanism — the same
`augmentum` lesson that shaped T2/T3.

### The repetition it addresses (measured on the shipped `c89.canon`)

335 elements carry children; **123 distinct content models**; 63%
duplication. The three biggest:

| block | × | what it is |
|---|---:|---|
| 19 genera | 12 | the grammar's `expressio` nonterminal |
| 14 genera | 6 | the grammar's `sententia` nonterminal |
| all 94 lexemes | 11 | "unparsed token run" (directives, regions, extenta) |

The first two are **names the grammar has and the closure erased**.
`<<#expressio>>` at each use site restores the name — which is the point,
beyond the ~1,300 lines saved. The remaining duplication (89 identical
token elements, ~41 one-line loci) is emitter uniformity and should be
left alone; factoring it buys nothing.

### Steps

- [x] **Step 1: Read the T3 seam.** `_fragmenta_colligere`,
  `_transclusionis_petitum`, `_liberos_effectivos` in `lib/canon.c` — the
  loading path wants the same collect-then-resolve shape, not a new one.

- [x] **Step 2: Write the failing test first**, in
  `probationes/probatio_canon.c`. The planted fault and its control are
  the inline/transcluded pair above — a transcluded `<liberum>` must
  behave **identically** to an inline one. Equivalence *is* the
  definition, exactly as in T3; do not test a proxy.
  Also assert the three T3 refusals still fire from the loading path:
  orphan `<<#nusquam>>`, duplicate `<#x>` twice, cyclic.

- [x] **Step 3: Run, confirm the transcluded case fails** with
  `liberum hic non licet` (not some other error).

- [x] **Step 4: Implement** in `canon_ex_nodo`: collect fragments first
  (definitions may follow uses), then walk the effective child list.
  `<attributum>`, `<liberum>` and `<optio>` should all resolve — a
  fragment holding attributes is as useful as one holding children.

- [x] **Step 5: Declare it in `canon.canon`** — a fragment child of
  `<canon>`, and the dialect must still judge itself clean.

- [x] **Step 6: Regression bar** — all 41 canons still load, 2,046 rules.
  A change to the loading path touches every dialect.

- [x] **Step 7: Emitter factoring.** `<#expressio>`, `<#sententia>`,
  `<#lexemata>` emitted once; use sites transclude. Derive the fragment
  membership from the model (do not hand-list), and **name the fragments
  after the grammar nonterminals they came from**.

- [x] **Step 8: Re-run the T5 gates.** `canon.canon` clean; roundtrip
  corpus 73/73; `probatio_silva_canon` — its pinned counts (95/176/75)
  will move, and moving them is the announcement, not a nuisance.

```bash
git add lib/canon.c canon.canon probationes/probatio_canon.c \
        silva/instrumenta/silva_canon_coquere.c silva/c89.canon
git commit -m "canon: transclusio in plagulis canonis (porta muta gradus superioris)"
```

---

## T7 — The corpus gate *(SHIPPED 2026-08-22)*

Spec §5. **The real gate.**

> **EXECUTION NOTES.**
>
> **(a) Shared apparatus, by Fran's call.** The closure + apparatus
> machinery moved to `silva/probationes/apparatus.{c,h}`, used by both
> `probatio_silva_arbor_plagula` and the new
> `probatio_silva_canon_corpus`. A gate that has already fired twice
> must not get a second independent implementation. `compile_probationes.sh`
> gained a step compiling non-`probatio_*` sources in `probationes/`.
> **Regression bar for the refactor: plagula still 154/154, 0 apparatus
> broken.**
>
> **(b) I dropped `radix` while moving the header loop** — the closure
> paths are repo-root-relative, so every header would have failed to
> open. Caught by re-reading the moved code, and note the apparatus gate
> would have caught it loudly anyway: no headers → no expansion → refuse.
> The gate protecting its own extraction is the design working.
>
> **(c) `lib/` + `*` inside a comment is `/*`.** Bit me a third time,
> in a new disguise — a directory glob in prose, not syntax being
> described. The rule generalises past its original form.
>
> **(d) BOTH TIERS CLEAN ON THE FIRST RUN** — 73/73 and 154/154, zero
> vitia. I had predicted defects. That prediction being wrong is not
> the finding; **the finding is what the coverage census showed.**

**Coverage of the hand-authored envelope — measured, because "154/154
clean" could equally mean "those elements never appeared":**

| element | plain tier | latinized tier |
|---|---:|---:|
| `expansio` | 28 | **219,013** |
| `pasta` | 0 | **11,322** |
| `extentum` | 0 | **2,827** |
| `stringificatio` | 0 | **4** |
| `conditionalis` | 3 | 144 |
| `error` | **41** | 0 |
| `invocatio-vacua` | 0 | **0** |
| `scissura` | 0 | **0** |

Six of eight are genuinely exercised, and the T5 policy claims they
encode are therefore validated — including §0.1's INFERRED
"ERROR root-level only", which the plain tier's 41 `<error>` nodes
exercise and confirm.

**Two are not covered, and this is asserted rather than left implicit:**
`invocatio-vacua` (a macro yielding zero tokens) and `scissura` (a
line-continuation inside a token) appear nowhere in either corpus. The
test pins them at **0** so the hole stays visible; adding a fixture will
break the assertion, which is the correct announcement. Landing spot:
purpose-built fixtures in `probationes/fixa/`.

Presence is asserted with `> 0` rather than pinned counts — `lib/` grows,
and a pinned 219,013 would cry wolf on every unrelated edit. Presence is
the property the gate actually needs.

**Falsification ran first**, per Step 1: a valid document with
`<declaratores>` corrupted to `<declaratoreel>` produces 2 vitia
(`liberum hic non licet`, `elementum extra canonem: an declaratores?`)
while the uncorrupted document produces 0.

- [x] **Step 1: Write the falsification FIRST.** Before any green sweep
  is trusted: take a valid arbor document, move a node into an illegal
  locus, and confirm a vitium. A sweep from an unfalsified gate measures
  nothing — this is why the T1 corpus run planted a fault before running
  41 files.

- [x] **Step 2: Write the sweep.** Generate arbor documents from both
  tiers and judge every one: 73 plain-C files in
  `probationes/fixa/roundtrip/`, then the latinized `lib/` tier.

- [x] **Step 3: The apparatus refusal** (inherited from
  `arbor-parsura-spec.md` §6.5). The latinized tier needs the include
  closure; without it nothing expands and every number is confidently
  wrong. **Refuse loudly when a file transitively including `latina.h`
  yields zero EXPANSIO tokens.** This has already failed twice — it is a
  gate, not a habit.

- [x] **Step 4: Run. Expected: zero vitia, both tiers.**

  If there are vitia, **judge by the cause tally, not the total** — a
  headline count can hold steady while the reasons underneath change
  completely. Most likely causes, in order: the `<genera-extra>`
  hand-authored policy (T5), the INFERRED root-level-only claim for
  ERROR/CONDITIONALIS, and T4's cycle-cutting.

- [x] **Step 5: Pin the numbers** so a future change must be announced,
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
