# STML: fuga sequentiae claudentis propriae — plan

*A raw literal `<T!>…</T>` cannot today carry the text `</T>`. This
plan makes it carry it, transparently, so that a value is preserved
and presented as written. Born 2026-09-19, from Fran's question about
bash comments.*

**Spec:** the RESERVATIO note on ledger decree **01M2KPJ0HW**
(2026-09-18) carries the syntax, the ladder rule and the rejected
alternatives. That note governs; this plan is how it gets built.

**Status:** Step 0 SHIPPED (`a3f7c6e5`). Steps 1–4 unbuilt.

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

Measured during the design pass, and the reservation UNDER-COUNTED
this: it says the reader change is "local to one scanner". True for
FINDING the terminator; false for DECODING.

| site | file | change |
|---|---|---|
| terminator scan | `lib/stml.c` `_tok_legere_contentus_crudus` | count the `\` run before `/`; run > 0 = content. Local, NO allocation. |
| **decode** | `lib/stml.c`, parser layer | the raw token value is a ZERO-COPY slice (`token.valor.datum = ctx->input.datum + initium`) and `StmlTokenContext` has NO piscina by design. Decoding needs a copy. Its home is the PARSER, which has `ctx->piscina` and already copies conditionally for CRLF (`_crlf_canonicalizare`). |
| escape on write | `materia/fontes/materia_arbor.c` `_valorem_crudum_notare` | escape instead of refusing; allocate from `st->piscina`. |
| predicate | `lib/stml.c` | extend `stml_crudi_terminator_est` with the backslash-run count, or add a sibling. ONE definition — Step 0's whole point. |

---

## 6. Tasks

### Task 1 — the predicate learns the ladder

**Files:** `lib/stml.c`, `include/stml.h`, `probationes/probatio_stml.c`

Teach the shared predicate to report the backslash-run length before
`/`, so callers can distinguish terminator (0) from escaped content
(> 0). Keep ONE definition.

Gate: unit cases for N = 0, 1, 2, 3 against each of the three
delimiters. Born red by returning a constant run length.

### Task 2 — the reader decodes

**Files:** `lib/stml.c`, `probationes/probatio_stml.c`

Scanner: stop only at run = 0. Parser: when the raw slice contains any
escaped sequence, allocate a decoded copy from `ctx->piscina`;
otherwise keep the zero-copy slice untouched.

Gate: a document containing `<\/T>` reads back as `</T>`; a document
containing `<\\/T>` reads back as `<\/T>`. **Plant the ladder
specifically**: make the decoder strip unconditionally and assert the
second case goes wrong.

Measure and record: the projection cost delta when no escape is
present must be ZERO (the zero-copy path is untouched).

### Task 3 — the writer escapes

**Files:** `materia/fontes/materia_arbor.c`,
`html/probationes/probatio_html_stml.c`

Replace the refusal with the escape. The five pinned cases in
`probatio_html_stml` promote from REFUSAL to round-trip.

Gate: all five round-trip byte-identically. **Plant the silent
corruption**: escape only `</T>` (not the ladder) and assert that a
value already containing `<\/T>` comes back changed — the gate must
catch a CHANGED VALUE, not a refusal.

### Task 4 — the corpora and the clients

**Files:** `html/probationes/probatio_html_circuitus.c`,
`crusta/probationes/*`, worklogs, `materia/CLAUDE.md`

- `circuitus`: `RECUSATA_SEQUENTIA` stays 0 and the total stays 1,708.
- crusta: a gate case for Fran's actual trigger — a bash comment
  containing `</crusta-commentum>` round-trips and the file lints
  clean.
- The `materia:scriptura` erratum for that file disappears.

---

## 7. Risks, named

1. **The ladder.** Its failure mode is a silently changed value, not a
   refusal. Every plant must assert VALUE EQUALITY, never "it
   refused".
2. **`lib/stml.c` is THE one parser** (law VIII.3). Everything
   depends on it; the blast radius is every client plus briar, silex
   and natura.
3. **Zero-copy becomes conditional-copy.** Verify the no-escape path
   allocates nothing — a regression here is invisible to correctness
   gates and shows only in `computus`.
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
