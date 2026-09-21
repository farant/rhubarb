# STML: fuga sequentiae claudentis propriae — plan

*A raw literal `<T!>…</T>` cannot today carry the text `</T>`. This
plan makes it carry it, transparently, so that a value is preserved
and presented as written. Born 2026-09-19, from Fran's question about
bash comments.*

**Spec:** the RESERVATIO note on ledger decree **01M2KPJ0HW**
(2026-09-18) carries the syntax, the ladder rule and the rejected
alternatives. That note governs; this plan is how it gets built.

**Status:** Step 0 SHIPPED (`a3f7c6e5`). **Task 1 SHIPPED
2026-09-19** — the predicate reports the ladder depth. **Task 2
SHIPPED 2026-09-21** — reader decodes and writer encodes, merged
because neither half is safe alone. **Task 3 SHIPPED 2026-09-21** —
materia's refusal deleted; **the trigger that started this plan now
lints clean.** **Task 4 SHIPPED 2026-09-21** — the symptom itself
gated, docs corrected. **PLAN CLOSED.**

**§9's open question was answered by building it.** The honest case
was never defect pressure — population in real corpora is still ZERO.
It was that the format should be able to describe itself, and Fran hit
the limit by doing exactly that. What the work actually bought, beyond
the escape: three wrong beliefs in these very documents were corrected
against the source, and a test that had been scheduled to break since
the day it was written was rebuilt on a contract instead of a limit.

---

## 1. The trigger, measured

A bash script whose comment contains its own closing tag:

```
#!/bin/bash
# see </crusta-commentum> here
echo hi
```

```
$ ./tools/diagnostica.sh c2.sh
c2.sh:3:1: [erratum] materia:scriptura
  valor sequentiam claudentem fert
exit: 1
```

**A valid bash script fails the commit lint because of what is inside
a comment.** Tier-2 rules cannot run on it at all: they need the STML
projection, and the projection is refused.

The class generalises to every client that serialises text as a
literal with its own closing tag. Already live in three:

| client | element | reachable by |
|---|---|---|
| crusta | `<crusta-commentum!>` | a comment mentioning the tag |
| html | `<lex-textus-crudus!>` | `<script>` containing the tag |
| html | `<lex-commentarium!>` | a comment containing the tag |
| md | (split by hand today) | — |

Population in real corpora is ZERO (`circuitus` reports
`sequentia 0` over all 1,708 html5lib cases). Every known case is
adversarial or self-referential — which is exactly how Fran hit it:
writing *about* the format inside the format.

---

## 2. What Step 0 already did (`a3f7c6e5`)

The reader and the writer's guard disagreed about what terminates a
raw region:

- reader stopped at `</T` + (`>` | `!` | whitespace) — three forms
- writer refused only `</T>` — one form

So `</T ` and `</T!` passed the guard, were written raw, and produced
a document the reader could not parse. Now `stml_crudi_terminator_est`
/ `_terminatorem_fert` are exported from `lib/stml.c` and BOTH sides
call them.

**This plan depends on that.** An escape covering only `</T>` would
inherit exactly the hole Step 0 closed.

---

## 3. Global constraints

- C89 in Latin via `latina.h`; house flags from `tools/vexilla.sh`.
- `i32`/`i64` are UNSIGNED. `chorda` is NOT NUL-terminated.
- latina macros are forbidden identifiers (`nomen`, `casus`,
  `magnitudo`, `registrum`, …). Check new words with
  `./oratio/quaere.sh` BEFORE staging.
- `./silva/formator.sh <viae> -scribere` then `-vitia`.
- **Every gate born red by a plant**, and a plant that breaks the
  BUILD proves nothing.
- Docs first, gates last, then commit.
- **After ANY `lib/stml.c` edit:** `./tools/natura_struere.sh` plus
  `briar_struere.sh`, `briar_spectator_struere.sh`,
  `silex_struere.sh`, or the root suite goes red on staleness. This
  bit twice during Step 0.
- Every materia commit runs `./materia/shim_probare.sh` plus all six
  client suites plus briar plus the root suite.

---

## 4. The rule (from the reservation, verbatim in effect)

Inside the raw form of element `T`, ONLY the sequence that would
terminate it is escapable. `<tag!><\/foo>` is untouched (`foo ≠ tag`).

```
WRITE:  '<' + '\'×N + '/T' + delim   (N ≥ 0)  →  '<' + '\'×(N+1) + '/T' + delim
READ:   '<' + '\'×N + '/T' + delim   (N ≥ 1)  →  '<' + '\'×(N−1) + '/T' + delim
TERMINATOR is only N = 0.
```

`delim` is `>` | `!` | whitespace — Step 0's three forms, via the
shared predicate. **The reservation wrote the rule against `>` only;
this plan widens it, because Step 0 proved the other two are real.**

**The ladder is mandatory, not ornament.** Without it a value already
containing `<\/T>` gets no escape (it holds no `</T>`), and the reader
resolves it *to* `</T>`. Value silently changed. That is the failure
this plan must plant against first.

---

## 5. Where the code goes

**Corrected 2026-09-19 (Task 1), against the source.** This section
and the reservation both claimed `StmlTokenContext` has no piscina,
and on that basis routed decoding through the parser. It has one
(`Piscina* piscina` + `InternamentumChorda* intern`), and the
tokenizer already allocates from it (`xar_creare(ctx->piscina, …)`
while reading attributes). **There is no parser-layer change.**

| site | file | change |
|---|---|---|
| predicate | `lib/stml.c` | ✅ Task 1: `stml_crudi_sequentia_est` reports the backslash-run depth; `stml_crudi_terminator_est` IS that function at depth 0. ONE definition — Step 0's whole point. |
| terminator scan | `lib/stml.c` `_tok_legere_contentus_crudus` | stop only at depth 0. Local, NO allocation. |
| **decode** | `lib/stml.c` `_tok_legere_contentus_crudus` | SAME function that counts the run. Allocate a decoded copy from `ctx->piscina` only when depth > 0 is seen; otherwise the zero-copy slice is untouched. |
| escape on write | `materia/fontes/materia_arbor.c` `_valorem_crudum_notare` | escape instead of refusing; allocate from `st->piscina`. |

---

## 6. Tasks

### Task 1 — the predicate learns the ladder ✅ SHIPPED 2026-09-19

**Files:** `lib/stml.c`, `include/stml.h`, `probationes/probatio_stml.c`

`stml_crudi_sequentia_est(textus, positus, titulus, longitudo, &fuga)`
recognises the whole form and reports the depth; `fuga` may be NIHIL.
`stml_crudi_terminator_est` is now *defined as* that call with
`fuga == ZEPHYRUM`, so the two questions cannot drift apart.

**Behaviour is unchanged by this task** — `<\/T>` was already not a
terminator, and still isn't. The step adds a question the code can
answer; nothing acts on the answer until Task 2.

Gate: 12 ladder cases (N = 0..3 × three delimiters), five non-ladder
shapes, and Fran's actual bash trigger against the writer's guard with
a multi-byte tag. Born red by returning a constant depth — 19
assertions fired.

### Task 2 — the reader decodes AND the writer encodes ✅ SHIPPED 2026-09-21

**Files:** `lib/stml.c`, `probationes/probatio_stml.c`

**The plan's split was wrong and was merged on measurement** (Fran
approved). Neither half is safe alone:

| | reader decodes | writer encodes | `<T!><\/T></T>` round trip |
|---|---|---|---|
| before | no | no | stable |
| reader only | yes | no | `<T!></T></T>` — unparseable |
| writer only | no | yes | value silently changed |
| both | yes | yes | stable |

Each broken case is a **silently changed or unparseable value** (risk
1), and no gate would catch it — corpus population is zero, so every
suite stays green through the window.

Built: `_crudum_solvere` (peel one rung, only when the region carries
an escape — the no-escape path keeps its zero-copy slice) and
`_crudum_fugatum_scribere` (add one rung, at all FOUR raw emission
sites: normal form and capture form, each flat and multiline; those
two blocks are verbatim copies, so fixing one alone would recreate the
two-halves defect inside one function).

**`stml_textus_internus` does NOT escape**, on measurement: its header
claims it returns source bytes, but `<a>x&amp;y</a>` yields `x&y` — it
already resolves entities, so its exactness is trivia and layout, not
character encoding. Raw escaping follows the entity precedent. This is
what kept the change out of the golden pins.

**Escape before line splitting**: a delimiter may be any whitespace,
newline included, so a sequence can straddle a line end.

Gates, each half planted separately: reader plant (strip every
backslash) fires the VALUE assertion at depths 2 and 3; writer plant
(escape only bare sequences) fires the BYTE assertion. **Depth 1 alone
would not have caught either** — stripping all and stripping one agree
there.

Cost: no-escape path allocates nothing and now makes one call FEWER
per byte (direct `sequentia_est` instead of the `_terminator_est`
wrapper). html's computus golden did not move.

### Task 3 — materia stops refusing ✅ SHIPPED 2026-09-21

**Files:** `materia/fontes/materia_arbor.c`,
`materia/probationes/probatio_materia_diagnostica.c`,
`crusta/probationes/probatio_crusta_{stml,canon}.c`,
`html/probationes/probatio_html_stml.c`

`_valorem_crudum_notare` no longer refuses; it just marks the element
raw. materia builds an `StmlNodus` tree that `stml_scribere` emits, so
the escape follows automatically — **no escaping code belongs in
materia at all.** The task was a deletion.

**Fran's original trigger lints clean**: `# vide </crusta-commentum>
hic` → `0 diagnostica`, exit 0.

Promoted, each gate having gone red on the first run after the
deletion, exactly as its own note predicted (`RUBET CUM MATERIA
RECIDAT`): html's five cases and crusta's two, from REFUSAL to
byte-identical round trip; crusta stml `IDEM` 51 → 52 and
`RECUSATA_SEQUENTIA` 1 → 0; crusta canon `iudicata` 51 → 52.

**Two call sites, and only one is where you would look.** The raw mark
is set from the lexeme path AND the trivia path. Planting the old
guard back on the lexeme path alone reddened `probatio_crusta_stml`
but left `probatio_crusta_canon` green — crusta's case is a *comment*,
which travels the trivia path. Both were planted; both reddened.

**`probatio_materia_diagnostica` lost its refusal vehicle for the
second time.** It asserts the *tractus* of a refusal, so it needs one
to ride on; NUL was the first vehicle (until the `nul` attribute),
this was the second. Both were limits the house *intended* to remove,
so the test was scheduled to break from the day it was written. Third
vehicle: an unknown token genus — a violated contract, not a missing
capability. **Rule: choose a refusal vehicle from the contract, never
from a limit.** The cause is now pinned too; both earlier vehicles
changed silently under an assertion that checked only *that* it
refused, never *why*.

### Task 4 — the corpora and the clients ✅ SHIPPED 2026-09-21

**Files:** `crusta/probationes/probatio_crusta_facies.c`,
`crusta/fontes/crusta_arbor.worklog.md`, `materia/CLAUDE.md`,
`crusta/CLAUDE.md`, `html/CLAUDE.md`

**`circuitus` needed nothing, and that was worth checking rather than
asserting.** It already pins `RECUSATA_SEQUENTIA` at 0 and
`IDEM = TOTALIS − RECUSATA_NUL − RECUSATA_SEQUENTIA`, and it is green
at 1,708/1,708. The counter was always 0 because the html5lib corpus
contains no such case — population is zero, which is the whole reason
this class needed finding by hand.

**The real gap was that no gate held Fran's symptom.** The round-trip
gates prove bytes; none proved the file LINTS CLEAN.
`probatio_crusta_facies` VII now runs `crusta_diagnostica_omnia` —
the path `./tools/diagnostica.sh` walks — over his exact script.

It asserts THREE things, because "zero findings" does not differ from
"nothing ran" (case VI in the same file taught that with a missing
lintrum): the Xar present, `causa` absent, count zero. And the plant
corrected the comment: the failure arrives as a **diagnostic**, not a
`causa` — `causa` stays NIHIL and `materia:scriptura` sits inside the
Xar. The gate now prints each finding's code, so a future red NAMES
it.

Docs: the limit was still described as open in three orientation
files; all three now record it as solved, with the reflexes.

---

## 7. Risks, named

1. **The ladder.** Its failure mode is a silently changed value, not a
   refusal. Every plant must assert VALUE EQUALITY, never "it
   refused".
2. **`lib/stml.c` is THE one parser** (law VIII.3). Everything
   depends on it; the blast radius is every client plus briar, silex
   and natura.
3. **Zero-copy becomes conditional-copy** — smaller than it looked
   (measured 2026-09-19). The TREE path already copies: the parser
   interns the raw text value (`chorda_internare`), so no node ever
   held the slice. Only `stml_lexemata_colligere` hands out the slice
   itself, and its sole consumer is `probatio_stml`. And value-vs-
   extent divergence is already the documented contract —
   `include/stml.h` on `StmlLexema` says the value is SEMANTIC
   (entities resolved) and a byte consumer must use the extent, which
   is exactly the shape a decoded value has. Still verify the
   no-escape path allocates nothing; the rest needs no new contract.
4. **Documents already on disk.** A value containing a literal `<\/T>`
   written BEFORE this change decodes differently after it. Zero known
   instances (corpora report `sequentia 0`), but it is a real
   migration question, and the answer should be written down rather
   than assumed.

---

## 8. What this does NOT solve

**NUL.** Settled separately on 2026-09-19 (`e84c33e7`): the `nul`
attribute carries offsets out of band, because an unrepresentable
BYTE and a value COLLIDING with its delimiter are different causes
deserving different remedies. The reservation proposed exactly that
division; this plan is its second half.

---

## 9. Open question for Fran

Population in real corpora is ZERO, and the ladder is the most
delicate thing in the shared parser. The honest case FOR building it
is not defect pressure — it is that the format should be able to
describe itself, and Fran hit the limit by doing exactly that.

Worth deciding deliberately rather than by momentum.
