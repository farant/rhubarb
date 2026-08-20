# Arbor M1 — Implementation Plan

Source spec: `project-specs/arbor-stml-spec-v2.md` (codebase-grounded).
Lineage: visio → exhibita → colloquium → spec v1 → four explorer
sweeps + seven follow-ups → spec v2 → **this**.
Ledger: parcum 01M0DMSTEJ; blocking quaestio 01M0EDQ735BV.

**Goal:** silva parse trees project to canonical STML documents and
load back, gated by two round-trip oracles over the roundtrip corpus.

**Architecture:** one generic reader + one generic writer in
`silva/fontes/silva_arbor.{h,c}`, driven at runtime by
`SilvaRegistrumCoctum` (no code generation). A tree-equality
comparator ships as fontes public API, not test-local. Two
prerequisite tranches land first: a shared-library fix in lib/stml.c
and a freshness gate on silva's amalgam manifests.

**Tech:** C89, latina.h macros, Latin identifiers throughout.
Vendored libs called by their PLAIN names inside `silva/fontes/`
(`stml_*`, `friatio_*`) — the amalgamator applies `silva_`
prefixing automatically.

## Global Constraints

- Flags per `compile_tests.sh`: `-std=c89 -pedantic -Wall -Wextra
  -Werror -Wconversion -Wsign-conversion -Wcast-qual
  -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings
  -Wno-long-long -Wno-overlength-strings`. `-Wcast-qual` forbids
  casting away `constans`.
- **Every latina.h word is a FORBIDDEN identifier** (`nomen`,
  `registrum`, `magnitudo`, `structura`, `casus`, `brevis`,
  `exire`…). Use `titulus`/`tabularium`. `./silva/censor.sh` names
  the culprit on cryptic parse cascades.
- `i32`/`i64` are **UNSIGNED**; `s32`/`s64` signed. `chorda` is NOT
  null-terminated.
- Tests: `./silva/compile_probationes.sh [filter]` — globs
  `probatio_*.c`, zero registration. **Exit 2 = NOTHING RAN.**
- Root suite: `./compile_tests.sh` for lib/ changes.
- zsh traps: `$?` after a pipe is the tail's status (use
  `${pipestatus[1]}`); bare `===` expands.
- Commits end with `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`.
- **NEVER commit** `FAQ.md`, `gesta/annales/forum.jsonl`,
  `gesta/annales/tabula.md`, `gesta/annales/tabularium.jsonl`.
  Stage explicitly by path; never `git add -A`.

## File Structure

| File | Responsibility |
|---|---|
| `lib/stml.c` (modify) | pretty-writer boundary fix (T0) |
| `probationes/probatio_stml.c` (modify) | 6 failing shapes + 2 regression guards + 2-cycle stability (T0) |
| `tools/amalgama_fontes_generare.sh` (modify) | `-probare` mode + `LC_ALL=C` (T1) |
| `tools/amalgama_excludenda_generare.sh` (modify) | `-probare` mode (T1) |
| `silva/amalgamare.sh` (modify) | gate 0: manifest freshness (T1) |
| `silva/fontes/silva_arbor.h` (new) | public surface: writer, reader, comparator, seal |
| `silva/fontes/silva_arbor.c` (new) | vocabulary layer, writer, reader |
| `silva/fontes/silva_arbor_aequalitas.c` (new) | tree comparator (own file: consumers beyond arbor) |
| `silva/probationes/probatio_silva_arbor.c` (new) | unit tests: vocabulary, seal, spelling table, comparator |
| `silva/probationes/probatio_silva_arbor_circuitus.c` (new) | the M1 gate: two oracles over the corpus |
| `silva/amalgama/silva.h` (modify) | public prototypes (hand-written surface) |

---

## T0 — lib/stml.c pretty-writer fix

Blocks arbor's pretty output. Quaestio 01M0EDQ735BV. **Blast radius
measured CLEAN**: all five `pulchrum=VERUM` call sites
(`tools/gutenberg_index.c:487`, `tools/librarium_merge.c:948`,
`lib/aedilis.c:1462`, `probationes/probatio_aedilis.c:391`) build
text nodes only via the single-text-child pattern or attribute-only
trees, so the three committed artifacts (`gutenberg_index.stml`,
`librarium.stml`, `aedilis.stml`) do not move. `apps/forum/forum.c`
passes FALSUM.

**The contract.** In pulchrum mode, whitespace-only TEXTUS children
are TRANSPARENT (skipped; boundaries computed against real
neighbors). For the remaining effective children, a boundary emits
`\n` UNLESS either side of it is a non-whitespace-only TEXTUS child
— this covers the open-tag boundary, each between-children
boundary, and the close-tag boundary. When a boundary collapses,
the element neighbor must ALSO not self-indent; achieve that by
invoking it with `pulchrum=FALSUM` (established precedent:
lib/stml.c:4359 already does exactly this).

**Named consequence, accepted:** forcing FALSUM flattens that
child's entire subtree formatting. Harmless for arbor (the
adjacent elements are small `<ante>`/`<post>` trivia wrappers) and
it follows the existing pattern. The alternative — moving
indentation from each child's case body into the loop so the
boundary decision is atomic — is cleaner but touches four case
bodies; **named door**, take only if FALSUM-forcing proves
insufficient.

Also in this patch (same function, decided in scope):
- whitespace-only text children currently leave a **stray blank
  line** (the loop appends its trailing `\n` for a child whose
  content was suppressed). The transparency rule above fixes it.
- `STML_NODUS_TRANSCLUSIO` **never self-indents** (lib/stml.c:
  4460-4467 omits `_scribere_indentatio`, unlike ELEMENTUM/
  COMMENTUM/PROCESSIO/DOCTYPE). One-line addition.

- [ ] **Step 1 — write the failing tests.** Add to
  `probationes/probatio_stml.c`. For each shape: parse the compact
  form, write with `pulchrum=VERUM`, assert byte-equality against
  the DESIRED output below (these FAIL today, by design).

  | # | input | desired pretty output |
  |---|---|---|
  | 1 | `<t><ante>x</ante>n</t>` | `<t>\n  <ante>x</ante>n</t>` |
  | 2 | `<t>n<post>y</post></t>` | `<t>n<post>y</post>\n</t>` |
  | 3 | `<t>a<b/>c</t>` | `<t>a<b/>c</t>` |
  | 6 | `<t><a/>  <b/></t>` | `<t>\n  <a/>\n  <b/>\n</t>` |
  | 7 | `<t>a<b/>c<d/>e</t>` | `<t>a<b/>c<d/>e</t>` |
  | 8 | `<root><t><ante>x</ante>n</t></root>` | `<root>\n  <t>\n    <ante>x</ante>n</t>\n</root>` |

  Shape 3 states the forced interpretation: an element sandwiched
  between text children loses its own indentation entirely — the
  only self-consistent reading of "no whitespace beside text".
  Shape 8 proves `indentatio + I` composition.

- [ ] **Step 2 — add the two-cycle stability assertion** to each
  shape: take the pretty output, `stml_legere` it, `stml_scribere`
  again with pulchrum, assert the second output is byte-identical
  to the first. **One cycle is not sufficient evidence** — the bug
  compounds (3 bytes → 5 bytes on a second pass). Make this
  standard for every pretty-mode test from now on.

- [ ] **Step 3 — add the regression guards** (must pass BEFORE and
  after): shape 4 `<t><a/><b/></t>` → `<t>\n  <a/>\n  <b/>\n</t>`;
  shape 5 `<t>only text</t>` unchanged (hits the pre-existing
  single-text-child special case, untouched by this work).

- [ ] **Step 4 — run; confirm the right ones fail.** `./compile_tests.sh stml`
  Expected: shapes 1,2,3,6,7,8 FAIL; guards 4,5 PASS. If a guard
  fails, the test itself is wrong — fix the test, not the library.

- [ ] **Step 5 — implement.** In `stml_scribere_ad_aedificator`'s
  mixed-children branch (lib/stml.c:4364-4388): build the effective
  child list (skip whitespace-only TEXTUS when pulchrum), then
  emit per-boundary as contracted, passing `FALSUM` to any element
  child whose left boundary collapsed. Add `_scribere_indentatio`
  to the TRANSCLUSIO case. `_spatium_album_solum` is `interior` and
  already exists (lib/stml.c) — reuse it, do not write a second one.

- [ ] **Step 6 — verify.** `./compile_tests.sh stml` → all green.
  Then `./compile_tests.sh` (full root suite) → green, and confirm
  `git status` shows NO diff in `gutenberg_index.stml`,
  `librarium.stml`, `aedilis.stml`. A diff there means the blast
  radius analysis was wrong — stop and investigate.

- [ ] **Step 7 — commit**, close quaestio 01M0EDQ735BV with a nota
  recording the measured before/after bytes.

**Risk to watch:** a naive `si (!est_textus)` around the loop's `\n`
without suppressing the neighbor's self-indent glues indent SPACES
onto text (`n  <post>`) — the same corruption class, and it looks
plausible in review. The shape tests above catch it.

Untouched and confirmed orthogonal: `crudus`, all three capture
forms, `clausura_anonyma`, the 2026-08-06 newline-dedup fix
(4259-4287). Separately noted, NOT in this patch: fragment
interiors are hardcoded non-pretty (lines 4070, 4096) — file as its
own item if arbor's fragments ever wrap large subtrees.

---

## T1 — Manifest freshness gate

`amalgamare.sh` does not regenerate its manifests and nothing
detects staleness: add a fontes module, skip the generators, and
the amalgam is silently WRONG while passing its own six gates.

**Must be unconditional regenerate-and-diff, never triggered by
"a new file appeared"** — `excludenda_generata.h` also drifts when
an already-vendored function flips unused→used because new code
called it, which no file-existence heuristic sees.

Pattern to clone: `tools/natura_canones.sh -probare` (:38-186) —
PID-scoped temp dir with `trap` cleanup, redirect the generator's
output there, `cmp -s` against the committed file. **Keep its exit
distinction**: 1 = artifact stale, 2 = the checker itself stale.
(`probatio_silva_tabulae`'s in-process shape does NOT transfer —
these generators are shell + `bin/aedilis` subprocesses with no
linkable C entry point.)

- [ ] **Step 1 — plant the fault first.** Create a throwaway
  `silva/fontes/silva_probatio_vetustatis.c`, run `amalgamare.sh`
  WITHOUT regenerating, and confirm today's behavior: it succeeds
  and the module is silently absent from the amalgam. Record the
  evidence — this is the fault the gate must catch. **A gate whose
  failure mode was never observed is a dead gate.**
- [ ] **Step 2 — `-probare` for the fontes generator.** CLI flag
  parse (~8 lines, copy natura_canones.sh:44-64); branch before the
  existing `mv` at :197 to `cmp -s` the scratch file
  (`$STATIO/fontes_generata.h.novum`, already produced) against
  `$EXITUS`. Also pin `LC_ALL=C` on the sorts at :52, :62-65,
  :98-105 — the excludenda generator already does this at :143;
  without it byte-identity isn't guaranteed across locales.
- [ ] **Step 3 — `-probare` for the excludenda generator.** Same
  flag parse; redirect `_caput_emittere`'s output block (:143) to a
  temp path, `cmp -s`, clean up.
- [ ] **Step 4 — wire as gate 0** in `silva/amalgamare.sh`, before
  any dependency objects are built — failing fast beats discarding
  a completed build. Failure message names the exact regeneration
  command, mirroring the censor gate's "regenera: …" style.
- [ ] **Step 5 — verify against the planted fault**: with the
  throwaway file present and manifests unregenerated, `amalgamare.sh`
  must now REFUSE with exit 1 and name the command. Measure the exit
  code directly, NOT through a pipe. Then regenerate, confirm it
  passes, delete the throwaway, regenerate again, confirm clean.
- [ ] **Step 6 — commit.**

Note: the excludenda probe runs a real clang harvest pass, so it is
not instant — same order as compiles `amalgamare.sh` already pays.
The fontes probe is cheap and could later ride a hook.

---

## T2 — Vocabulary layer (`silva_arbor.{h,c}`)

Foundation for both directions. Nothing here touches trees yet.

- [ ] **Step 1 — the seal.** `silva_registrum_sigillum(piscina,
  tabularium) → chorda` (lowercase hex). Walk `genera[0..
  numerus_generum)` appending titulus + `loci_offset` +
  `loci_numerus`, then `loci[0..numerus_locorum)` appending titulus
  + species, into a `ChordaAedificator`; one
  `friatio_fnv1a_literis` pass. **Do not vendor `sigillum`** — the
  FNV-1a pair is already in the amalgam, same translation unit,
  zero manifest edits. 32 bits is correctly sized: one comparison,
  so a false accept is 2⁻³².
- [ ] **Step 2 — test the seal**: same registry twice → identical;
  c89 vs sceletum → different; a hand-mutated copy of one genus
  titulus → different.
- [ ] **Step 3 — registry name lookups.** Local static helpers
  (genus titulus → index; locus titulus → index + species, scoped
  to a genus), following `silva_quaestio.c:289-309`'s scan shape.
  Note in a comment that this is the FIFTH such duplication
  (scribere:76, commissio:471, parsare:10, quaestio:289) — promoting
  a shared helper to `silva_tabulae.h` is a named door, not M1 work.
- [ ] **Step 4 — the spelling table + its gate.** A genus→spelling
  table for all fixed-spelling lexemes (32 keywords + punctuation).
  **No reverse mapping exists in silva today** — keywords have only
  a forward table (`VERBA_CLAUSA`, `hic_manens` at
  silva_lexema.c:52) and punctuation only an if-else recognizer
  (`_legere_interpunctionem`, :717). This is a second source of
  truth, so gate it: for every entry, lex the spelling and assert
  the genus comes back. Silva implements no trigraphs, so spellings
  are 1:1 today; if that changes, valor must be carried — record
  the dependency in the header comment.
- [ ] **Step 5 — tag mangling.** Node genus → tag verbatim. Lexeme
  genus → lowercase, `_`→`-`, prefix `lex-`. Test the measured
  collision explicitly: node genus `assignatio` → `assignatio`,
  lexeme `SILVA_LEX_ASSIGNATIO` → `lex-assignatio`; and that
  `corpus` as a genus and as a locus both round-trip through their
  own paths.
- [ ] **Step 6 — commit.**

---

## T3 — Writer (tree → STML)

- [ ] **Step 1 — signature.** `silva_arbor_scribere_nodum(Piscina*,
  constans SilvaNodus*, constans SilvaRegistrumCoctum*, constans
  SilvaExpansio*, InternamentumChorda*) → SilvaArborScriptura`
  (successus / textus / causa / sedes, mirroring `SilvaScriptura`).
  **`SilvaExpansio*` is REQUIRED** — the fons table lives at
  `SilvaExpansio.fontes` (silva_expandere.h:176) and a bare node
  cannot resolve `fons_index`. `InternamentumChorda*` may be NIHIL
  → lazily create via `internamentum_creare`, following
  `silva_annotationes.c:388-391` (`SilvaContextus` carries none).
- [ ] **Step 2 — build an StmlNodus tree, then `stml_scribere`.**
  Do NOT reuse `silva_scribere`'s walk: its recursion is `interior`
  with no visitor seam and its token half is coupled to byte
  reconstruction arbor has no use for. Shared walk SHAPE,
  independent implementation.
- [ ] **Step 3 — emission rules** (spec §2-§3): genus element →
  locus wrapper elements in layout order, NIHIL omitted; tokens as
  `lex-` elements with valor as text for variable-spelling genera
  only; `<ante>`/`<post>` with compact lenses (`<spatia n="4"/>`,
  `<tabulae n="2"/>`, `<nova-linea/>` +`crlf`, `<continuatio/>`),
  comments as entity-escaped text; `<scissura offset="N"/>`;
  `standard`/`f` attributes only when non-default; INDEX loci as
  arabic text content. **No positions are written** — the envelope
  carries the anchor only.
- [ ] **Step 4 — AMBIGUUS diverges from scribere deliberately.**
  Scribere emits only the canonical arm (silva_scribere.c:367-403);
  arbor emits ALL interpretations. First use of a shared token
  writes a named fragment (`fragmentum = VERUM`, `fragmentum_id`),
  later uses write `STML_NODUS_TRANSCLUSIO` with the id.
  `StmlNodus` is a transparent struct — set the fields directly
  (precedent: probatio_stml.c:2815-2848 proves the
  hand-constructed-node path round-trips).
- [ ] **Step 5 — refuse loud** on a non-FONS token (fractura clara
  at the expansion boundary, mirroring scribere), unknown genus,
  and corrupt layout. Never a silent omission.
- [ ] **Step 6 — tests** in `probatio_silva_arbor.c`: the three
  exhibits from `arbor-stml-exhibita.md` (pure declaration; a
  latina-expanded statement expected to REFUSE; an AMBIGUUS pair
  with shared tokens producing exactly one fragment + one
  transclusion). Assert the refusals by their named causa.
- [ ] **Step 7 — commit.**

---

## T4 — Comparator (`silva_arbor_aequalitas.c`)

Public fontes API, not a test helper: `silva/probationes/` has zero
shared helper files (each probatio its own TU), and the next
consumer is already visible — mutatio's gates ask exactly "did this
transform preserve the tree".

- [ ] **Step 1 — signature.** `silva_arbor_aequalis(constans
  SilvaNodus* a, constans SilvaNodus* b, SilvaArborDifferentia*
  causa) → b32`, where the out-struct names the first divergence
  (path, field, both values) — a boolean alone is useless in a gate.
- [ ] **Step 2 — compare**: genus; `numerus_locorum`; per locus the
  valor genus and species; tokens on genus, valor bytes, `standard`,
  `fons_index`, `initium_lineae`, trivia series (genus + valor, in
  order, both lists), scissurae; lists element-wise on the
  PROSPECTUS length (never `xar_numerus` of the repositorium).
- [ ] **Step 3 — pater.** Compare it, but document why it is
  meaningful: committere assigns from the LAST visit in walk order,
  so pater on genuinely shared subtrees is walk-order-defined
  (parked twice: phase-log:1356-1359, :5288-5295). Both trees are
  walked identically, so the comparison stays deterministic; the
  semantic question is inherited, not created here.
- [ ] **Step 4 — tests**: identical trees equal; each compared
  field mutated in turn produces inequality AND names the right
  field. **A comparator that cannot fail is the dead-gate class** —
  every field gets a planted-mutation test.
- [ ] **Step 5 — commit.**

---

## T5 — Reader (STML → tree)

- [ ] **Step 1 — signature.** `silva_arbor_legere(Piscina*,
  InternamentumChorda*, chorda textus, constans
  SilvaRegistrumCoctum*, SilvaArborVitium* causa) → SilvaNodus*`.
  NIHIL + a named causa with a LINE NUMBER on any refusal
  (`StmlNodus.linea` exists for exactly this).
- [ ] **Step 2 — validate before building**: envelope present;
  `grammatica` matches; **seal matches or REFUSE** (a tree judged by
  the wrong vocabulary is a lie); every genus/locus tag known;
  locus species matches its content; valor present iff the genus is
  variable-spelling.
- [ ] **Step 3 — build through the checked path.**
  `silva_nodus_creare` + `silva_nodus_ponere` (S32: species-checked,
  write-once, silva_nodus.c:212-242) — construction checking for
  free. **The loader must police `LISTA_MIXTA` element kinds
  itself**: `silva_nodus_appendere` validates only that the species
  is a list kind, never what goes inside.
- [ ] **Step 4 — tokens.** `silva_token_ex_fonte` for base fields,
  then assign `initium_lineae`, `spatia_ante`, `spatia_post`,
  `scissurae` directly — the established house pattern
  (silva_lexema.c:888, :976, :988, :350), not a workaround.
  Restore fixed spellings from T2's gated table.
- [ ] **Step 5 — whitespace.** Skip whitespace-only text nodes at
  structural positions (reuse the same predicate shape as
  `_spatium_album_solum`; it is `interior` to lib/stml.c, so arbor
  writes its own trivial version). Valor text is taken VERBATIM via
  `stml_textus_internus` — **never `stml_textus_normalizatus`**,
  which transforms.
- [ ] **Step 6 — fixups**: recompute pater, `initium_lineae`,
  `longitudo`, and stratum-0 positions by an emission walk from the
  envelope anchor. Then resolve fragments/transclusions so shared
  tokens are the SAME object (identity is the point).
- [ ] **Step 7 — commit** the loaded tree:
  `silva_committere(piscina, silva_valor_nodus(radix), tabularium,
  NIHIL, NIHIL, NIHIL)`. A loaded tree is not queryable or
  re-canonicalizable until committed; the bare no-resolution form is
  explicitly sanctioned (silva_commissio.h:163-165).
- [ ] **Step 8 — refusal tests, each with a planted fault**: bad
  seal, unknown genus, unknown locus, species mismatch, valor on a
  fixed-spelling genus, dangling transclusion id. Each must refuse
  with its own named causa and a line number.
- [ ] **Step 9 — commit.**

---

## T6 — The M1 gate (`probatio_silva_arbor_circuitus.c`)

- [ ] **Step 1 — corpus enumeration.** Repo-root
  `probationes/fixa/roundtrip/` (78 fixtures: 73 `.c` + 5 `.h`),
  path via `RHUBARB_RADIX` else `".."`
  (probatio_silva_fidelitas.c:194-198). No reusable enumerator
  exists — fidelitas inlines its `opendir` loop (:207-246); copy it.
- [ ] **Step 2 — parse + select subtrees.**
  `silva_c89_parsare(piscina, via, fons, mensura, NIHIL)`, then pick
  subtrees with a compiled selector —
  `silva_quaestio_compilare(piscina, &SILVA_C89_REGISTRUM, genus,
  &causa)` + `silva_quaestio_exsequi(q, parsura->commissio->radix,
  piscina)` (precedent probatio_silva_quaestio.c:44-58, :260). This
  makes the gate DOGFOOD the shared vocabulary: the same registry
  titulus is the selector tag and the STML tag.
- [ ] **Step 3 — oracle A, tree equality.** serialize → load →
  `silva_arbor_aequalis` against the original, recomputed fixup
  fields included.
- [ ] **Step 4 — oracle B, byte round-trip.** load →
  `silva_scribere_nodum` → compare against the original subtree's
  emitted bytes. **NOT redundant with oracle A.** Trivia
  single-ownership is enforced NOWHERE at runtime (lex-time
  construction is disjoint only because one linear pass splits each
  run, silva_lexema.c:930-1010; the generator's check is static over
  grammar SOURCE, silva_generare.c:614-651; `ponere` never inspects
  trivia). A structural comparator is **blind to double-ownership**
  — both owners look locally correct — and only emission reveals it,
  as duplicated bytes. Orphaned trivia shows as missing bytes.
- [ ] **Step 5 — per-file arena.** Create and destroy a per-fixture
  `opus` piscina (`piscina_generare_dynamicum("arbor-opus",
  67108864)`), destroying on EVERY exit path. The long-lived arena
  holds context only. This is the documented fix for a real silent
  death: parsing 230+ files into one long-lived pool exhausted it
  and the process died mid-run with no signal (formator.c:265-270).
- [ ] **Step 6 — the representational audit.** Log by NAME every
  field that could not survive the round trip. This is the deliverable
  that justifies the phase (visio §2.1) — write it into
  `silva/fontes/silva_arbor.worklog.md`, not just the terminal.
- [ ] **Step 7 — plant a fault and confirm the gate fails.** Mutate
  one serialized token's valor and confirm oracle A fails; duplicate
  one trivia reference across two owners and confirm oracle B fails
  while A passes. **That second check is the whole reason both
  oracles exist** — if A also fails, the trivia model is different
  from what was measured; stop and investigate.
- [ ] **Step 8 — commit.**

---

## T7 — Amalgam integration

Run in this exact order; **skipping the generators produces a
silently wrong amalgam** (T1's gate now catches it, which is the
point of doing T1 first):

- [ ] **Step 1** — `./tools/amalgama_fontes_generare.sh silva`
- [ ] **Step 2** — `./tools/amalgama_excludenda_generare.sh silva`
      (this is what pulls `stml_scribere` and the mutation APIs back
      out of `EXCLUDENDA_STML` — they are currently pruned as dead
      code because silva has no runtime STML *writer* today. Arbor
      is the first.)
- [ ] **Step 3** — hand-extend `silva/amalgama/silva.h` with the
      public prototypes (writer, reader, comparator, seal) in the
      vanilla-C89 style: `int`/`const`, `SilvaPiscina*`, no latina.
      No `fontes_politica.sh` or `amalgamator.c` edits are needed —
      stml and friatio are already vendored.
- [ ] **Step 4** — `./silva/amalgamare.sh`; all gates green
      (standalone compile, hospes host-pollution, nm-intersection
      zero, censor) plus T1's new gate 0.
- [ ] **Step 5** — `./silva/compile_probationes.sh` full, then
      `./compile_tests.sh` full.
- [ ] **Step 6 — commit**, then file the RELATIO debrief on parcum
      01M0DMSTEJ with the instrument debrief (adhibita / fructus /
      asperitates / desiderata).

---

## Deferred, with landing spots (no silent caps)

- Mined pure subtrees via `est_fons_purus` → M1-hardening, if the
  78 fixtures prove too tame.
- Hand-authored golden trees → M3, where authoring becomes a surface.
- origo chains, caecatio, conditional regions, strata → M2
  `<parsura>`.
- Canon projection from the registry → M2.
- Registry name→index helper promotion → door (fifth duplication).
- stml fragment interiors never pretty-printed → own item.
- Transclusion write-path coverage is thinner than fragments' (no
  nested case, no stability assertion) and arbor is its heaviest
  user → arbor's own gates close this.
- Selector engine cannot descend into a named locus, so path-shaped
  2D patterns have no 1D equivalent → record in mutatio-visio §4.
