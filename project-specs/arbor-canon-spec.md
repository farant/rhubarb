# Arbor M2 §3 — the canon projection (spec)

Status: **design approved 2026-08-22** (Fran, by fork). Not yet built.
Ledger: parcum 01M0DMSTEJ (arbor arc); defect 01M0MY50CP (mute gate).

Sibling documents this one does not duplicate:
`arbor-parsura-spec.md` (the document form being judged),
`arbor-stml-visio.md` §4 (the doctrine boundary restated in §1.2 below),
`canon-referentia-spec.md` (sigils and reference semantics).

Read order on re-entry: this file → `arbor-parsura-spec.md` §2 (document
shape) → `lib/canon.worklog.md` → `canon.canon` header (it names two of
the pathologies this spec closes).

---

## 0. What measurement decided

Five parallel researchers plus direct probes, 2026-08-22. Everything in
this section changed a decision or corrected a written claim. Raw
findings are in the session scratchpad; the load-bearing numbers are
reproduced here because a number that is not written down becomes an
assumption within one session.

### 0.1 The genus-per-locus relation is derivable — totally

The design forked on one question: can we say *which genera may fill
locus L*, or only *"some node goes here"*?

- **Not in the baked registry.** `SilvaRegistrumCoctum` has exactly four
  fields (`silva/fontes/silva_tabulae.h:100-105`); a locus row is
  `{titulus, species}` and carries no genus set.
- **Not recoverable at runtime.** `SilvaTabProductio` is
  `{sinistrum, longitudo, id}` (`silva_tabulae.h:67-71`) — **the RHS
  symbol vector is not baked, only its length.** The generated
  `silva_c89_construere` switch encodes position→locus+species but never
  which nonterminal produced each value. It is a positional shuffle, not
  a type map.
- **Recoverable at GENERATION time, completely.**
  `silva/grammatica/c89.stml` annotates every RHS atom as
  `symbolum@locus`, and the generator already holds the pairing
  materialised: `SilvaGenProductio.loci` is an `Xar` **parallel to**
  `dextrum` (`silva/instrumenta/silva_generare.h:52-62`).

Derivation implemented and run during research: **165/165 (genus, locus)
pairs resolved across 53 genera, zero residue.** That matches the
registry exactly — 58 genera − 5 `<genera-extra>` = 53; 176 locus
entries − 11 extra-genus loci = 165.

Worked example, the one that decided it:

```
definitio-functionis / declarator
    → {declarator-aciei, declarator-functionis, declarator-monstrator,
       declarator-titulus, parenthesis}
```

`declarator-abstractus` is **excluded** there (it appears only in
`parametrum/declarator` and `species-typi/declarator`) — a real
distinction species-only typing cannot make.

Token loci are the larger win: **92 of 176 locus entries are TOKEN
species**, and the derivation pins most to 1–2 of the 95 `lex-*` tags
where a species-only schema would admit all 95 everywhere.

### 0.2 The seal does not cover the derivation inputs

`silva_arbor_sigillum` (`silva_arbor.c:149-235`) FNV-1a's **only** the
genus and locus tables. It does **not** cover the grammar productions,
`NOMINA_GENERUM`, or `ORTHOGRAPHIAE`.

Consequence, and this is the reason §4.3 exists: editing
`si/consequens` to admit a new statement genus leaves
`registrum-sigillum` **unchanged** while every projected content model
goes silently wrong. A projected canon cannot borrow that seal. It needs
its own, over its own inputs.

This is the same failure shape as the mute gate: a generated artifact
whose staleness is invisible.

### 0.3 The document vocabulary, measured

Census over 89 files (73 `probationes/fixa/roundtrip/` + 16 `lib/`),
25 MB of documents, 492,180 tag occurrences; opens = closes = 396,777,
delta 0.

| layer | names |
|---|---:|
| document root (`arbor`, `parsura`) | 2 |
| envelope sections | 8 |
| origin (nested **inside** token elements) | 6 |
| trivia (`ante`, `post`, `scissura`) | 3 |
| token `lex-*` | 95 |
| genus | 58 |
| locus (distinct **names**) | 62 |

**231 distinct tags** emittable; 220 observed. Two (`api`, `arbor`)
cannot be produced by any `.c` file at all and were found by reading,
not measurement.

Three corrections this forced on the working brief:

1. **62 locus NAMES, not 176.** 176 is the *entry* count; entries are
   (genus, slot) pairs and names repeat. The vocabulary is 3× smaller
   than assumed.
2. **There is an `origin` layer** (`expansio`, `pasta`,
   `stringificatio`, `api`, `extentum`, `invocatio-vacua`) that nests
   inside token elements. It was not in the four-layer model.
3. **`sumptus` is not an attribute.** It is the genus `ramus-sumptus`.

Other facts that constrain the schema directly:

- **Mixed content is mandatory.** `lex-identificator` carries text *and*
  `<ante>`/`<post>`/`<expansio>` siblings 26,425 times. Only 9 of 220
  observed tags carry text at all.
- **Exactly three bare attributes exist**: `linea-initium` (4,259),
  `crlf` (3), plus emittable-but-unobserved `lexicon`/`syntheticus`.
  STML interns these as `"true"`.

  > **CORRECTION, T5 (2026-08-22): the second half of this bullet was
  > wrong and is retracted.** It read "so `genus="veritas"` types
  > them." It does not. STML interns a bare attribute as the string
  > `"true"`, but canon's `veritas` accepts only `"verum"` or
  > `"falsum"` (`lib/canon.c:719-721`) — the two vocabularies do not
  > meet. Typing them `veritas` makes **every** arbor document fail:
  > `linea-initium` alone occurs 1,955 times in the roundtrip tier.
  >
  > Measured, not reasoned: a three-line probe canon with the same
  > bare attribute typed three ways gave `veritas` → VITIUM,
  > `electio` + `<optio>true</optio>` → clean, `textus` → clean.
  >
  > **The projection uses `electio` with a single `<optio>true</optio>`.**
  > It is the only typing that both passes and still constrains —
  > `textus` would admit any value. Do not "fix" this by widening
  > canon's `veritas`: `verum`/`falsum` is the house vocabulary, and
  > `"true"` is an STML representation detail that has no business
  > leaking into the schema language.
- **Absent loci are legitimate** — `definitio-ansi` fills loci 0,1,3 and
  leaves 2 NIHIL; the writer skips NIHIL loci
  (`silva_arbor.c:1656-1660`). Therefore **every locus is
  `minimum="0"`** in the projection. No exceptions.
- **Two cross-layer name collisions**: `corpus` is genus ∧ locus (and
  nests inside itself, 530×); `directiva` is envelope ∧ locus. Both are
  precisely what canon's `intra=` exists for.

### 0.4 Canon is blind to transclusion, and it is a silent blindness

Zero occurrences of `fragmentum` or `TRANSCLUSIO` in all 2,791 lines of
`lib/canon.c`. Measured behaviour:

- a transclusion node fails the `genus != STML_NODUS_ELEMENTUM` test in
  the content-model loop (`lib/canon.c:1774-1777`) and is **silently
  skipped** — the content it stands for is never judged;
- a fragment definition has `titulus == "#"` (`lib/stml.c:2217`), so it
  raises **false** `LIBERUM_ILLICITUM` vitia *and* its interior is never
  judged (early return).

No general transclusion resolver exists anywhere in the repo;
`stml-visio.md` marks resolution RESERVATUM. Silva has one, but it
resolves *out of* STML into `SilvaToken*`
(`silva_arbor.c:2543-2569`).

Arbor documents use this heavily and structurally: **9,966 fragment
definitions** in the measured corpus, each holding exactly one `lex-*`
child, ids of the form `lex<N>`, occurring iff `<ambiguus>` occurs.
Transclusions appear under 15 different parent tags.

### 0.5 The `augmentum` precedent — do not invent a second mechanism

`_augmentum_iudicare` (`lib/canon.c:1693-1842`) already judges content
whose definition lives elsewhere. Its shape is exactly what §3.2 needs:

1. augmentation nodes are **excluded from the ordinary descent**
   (`:1867-1870`) — a parent's content model never declares `%`;
2. a **collection pass runs first** (`:1624-1686`) because a definition
   may legally appear *after* its use in document order;
3. resolution tries home mint, then external keys, then fails with
   `CITATIO_IRRITA` (`:1743-1749`) — **never silently**;
4. resolved content is judged by re-entering ordinary judgment with
   `parens_vi`, the **semantic-parent override** (`:1838-1839`), so that
   `intra=`-scoped definitions still resolve even though the tree parent
   is the wrapper node.

Step 4 is the load-bearing detail. The worklog records it as a real bug
found on first corpus contact that all eight unit fixtures missed
(`canon.worklog.md:349-368`). §3.2 inherits it or repeats the bug.

### 0.6 The mute gate collides with nothing (verified)

Fran's condition for fixing it standalone was that it not break existing
files. Prototyped the rule, planted a fault before trusting it:

| probe | result |
|---|---|
| `attributum="a-b"` naming nothing | **VITIUM, exit 1** |
| control: `attributum="a"` which exists | VITIA 0, exit 0 |

Both reported "regulae 1", so the rule was examined rather than skipped
in each case.

**Corpus: 41 canons · 2,046 rules (unicitas + citatio) · VITIA 0.**

The distribution is what makes that meaningful: 1,646 of the 2,046 rules
live in `natura/cocta/individua.canon`, and `natura/cocta/*` are the
generated canons that judge `.census` files — exactly the collision
asked about. All clean.

**But the first version of the rule was WRONG, and the corpus said so.**
Two corrections, both found by extending the probe rather than by
reasoning:

1. **`<citatio>` has TWO attribute sides.** `attributum=` is the citing
   attribute; `ad="elementa/attributum"` names the *key* attribute. The
   first probe checked only the citing side. The rule must check both.
2. **The star citation is a wildcard, not a name.** With the `ad` side
   added, the corpus produced **470 vitia — every one of them
   `ad="*/nomen"`, every one in `natura/cocta/individua.canon`.** `*`
   means "any element" (librarium star citations); treating it as a
   literal element title makes the *rule* false, not the corpus.
   `ad="*/x"` and an absent `super=` say the same thing and must be
   handled the same way.

With both corrections, the corpus is clean again: **41 canons · 2,046
rules · both citation sides · star-aware · VITIA 0**, planted fault
still clamouring, control still silent.

This is the shape Fran's condition was designed to catch, and it caught
it. Had the rule shipped on the first measurement, it would have refused
to load the single largest generated canon in the repo — 470 rules — on
its first contact with production.

Documents cannot be affected in any case: the rule judges a **canon at
load time**, so even a violating canon would refuse to load rather than
change any document's verdict.

### 0.7 A correction to `arbor-parsura-spec.md:523`

That row says canon projection is *"blocked on canon-side adstricta
uniqueness"*. **That is wrong and this spec supersedes it.**

`intra=` is fully implemented — on `<elementum>`, `<unicitas>`, and
`<citatio>` (`lib/canon.c:1196`, `:1424`, `:1542`, scope evaluation at
`:2189`). What is missing is narrower: `<unicitas attributum=>` accepts
exactly one attribute, so uniqueness over a *tuple* cannot be stated.

And that does not block this project either: `canon.canon` and
`aedilis.canon` carry **zero** uniqueness rules today, and the only
uniqueness the projection needs (`<fons index>`) is single-attribute.
Composite keys remain a genuine want with two askers on file — they are
**§6, deferred**, not a dependency.

---

## 1. What this project is

Project silva's C89 grammar into a generated `.canon` so that serialized
syntax-tree documents become **judgeable**, and build the three canon
capabilities that judging them honestly requires.

### 1.1 Why, in one line each

- **A second oracle of a different KIND.** The round trip is a *lenient*
  oracle — it proves bytes survive, not that the document is well-formed
  as a document. The poisoned-CRC episode stayed 54/54 green. Canon asks
  a question no fidelity oracle can ask.
- **Anything not written by the writer has no gate at all today** —
  hand-written golden fixtures, explorer output, future pattern-engine
  emissions.
- **One vocabulary, a fourth consumer.** The registry already drives
  genus names, quaestio selectors, and STML tags.

### 1.2 Doctrine paragraph (required restatement)

`arbor-stml-visio.md` §4 requires this be named in every downstream
spec: **arbor documents are code documents, and code documents may be
languages.** A compact authored form whose expansion is the reader's job
is lawful here in a way it is not in a census. The line that stays
uncrossed is *computation*: substitution yes, evaluation never.

This spec touches that border in exactly one place — §3.2 makes canon
resolve transclusions before judging. That is substitution, performed by
the judge, over a code document. It is lawful. **It must not be
generalised to data dialects**, and §3.2 is therefore canon-gated.

### 1.3 Decisions taken (Fran, 2026-08-22)

| fork | decision |
|---|---|
| schema reach | **full grammar conformance** — derived genus-per-locus content models, not species-only |
| transclusion | **supported, transparently** — see §3.2 |
| arbor's reader | **stays hand-written** — do not generate it |
| mute gate | **fixed standalone, first** — verified non-colliding (§0.6) |

On the reader: the precedent is `silva/quaestiones.canon`, which has
both — the hand-written one is production because it *compiles
selectors*, which generation cannot do; the generated one exists only as
a test-only birth differential. Arbor's reader does reconstruction work
of the same kind.

---

## 2. Staging

Four stages. Each ships independently and each carries its own gate.

| stage | deliverable | depends on |
|---|---|---|
| **S1** | mute-gate fix in canon | nothing |
| **S2** | transparent transclusion in canon | nothing |
| **S3** | the projector: grammar → `.canon` | S2 |
| **S4** | freshness gate + corpus judgment | S3 |

S1 and S2 are independently useful to the four existing dialects and do
not mention arbor. S3 is the only part that knows what silva is.

---

## 3. The canon capabilities

### 3.1 S1 — a `<unicitas>`/`<citatio>` must name a real attribute

**Defect.** `attributum=` is used as a lookup key. If it names nothing,
every lookup returns nothing, no values are collected, no duplicates are
possible, and the rule reports success. `canon.canon`'s own header lists
this as pathology IV — *"a gate that guards nothing but appears to"* —
having closed the case where `attributum=` is **absent** and left the
case where it is **wrong**.

`attributum` is typed `compositum`, which forbids spaces, so
`attributum="a b"` is caught *if the canon is itself judged*.
`attributum="a-b"` is legal `compositum` and is caught by nothing.

**Rule.** At canon load time, **three** attribute references must each
name an attribute that some element in their scope declares:

| rule | reference | scope |
|---|---|---|
| `<unicitas>` | `attributum=` | elements in `super=` |
| `<citatio>` | `attributum=` (citing side) | elements in `super=`, or **any** if absent |
| `<citatio>` | the attribute half of `ad=` (key side) | elements in the title half of `ad=` |

**Wildcard.** A scope of `*` means *any element* and is handled exactly
like an absent `super=`. This is not a nicety: measured, 470 rules in
`natura/cocta/individua.canon` use `ad="*/nomen"`, and a rule that reads
`*` as a literal element title refuses every one of them (§0.6).

This is a judgment of the **canon**, not the document. It belongs with
the other load-time refusals in `canon_ex_nodo`, which report by
returning NIHIL and setting `*causa` (`lib/canon.c:1314-1344` is the
idiom).

**Scope note.** In the loaded structures the scope lists are already
split — `CanonUnicitas.super` and `CanonCitatio.super`/`.ad_elementa`
are `Xar` of `chorda*` (`lib/canon.c:68-94`), so no string splitting is
needed; the `parens/titulus` form's element name is the part after the
solidus. `intra=`-scoped definitions count — an attribute declared on
`<elementum nomen="regula" intra="nexus">` satisfies a rule naming
`regula`. The check is deliberately permissive about *which* definition
supplies the attribute; it refuses only when **no** definition does.

**Gate.** A planted fault, per house law. The prototype's fault and
control (§0.6) port directly: a canon whose rule names a nonexistent
attribute must produce a vitium; the same canon with a real attribute
must not. Both must confirm the rule was examined, not skipped.

**Regression bar.** All 41 canons must still load clean: **2,046 rules,
zero vitia, with all three references checked and `*` treated as a
wildcard.** Pinned so a change must be announced.

**Second planted fault, specific to the star.** A citation with
`ad="*/attributum-quod-non-est"` must still clamour. Otherwise the
wildcard fix becomes a blanket exemption that silently disables the key
side — turning the fix for one dead gate into another.

### 3.2 S2 — transclusion, judged transparently

**Semantics, as decided.** A transclusion is not a thing the schema
knows about. At the point where `<<#id>>` appears, canon judges
**whatever the fragment resolves to, as if it had been written inline at
that position**. Document-level sharing is a serialization concern; the
schema sees only the tree it stands for.

Three consequences, all normative:

1. **No dialect declares a transclusion tag.** Like `%`, the wrapper is
   excluded from the ordinary descent and no parent's content model
   mentions it (`lib/canon.c:2100-2103` is the precedent comment).
2. **An unresolvable transclusion is a loud vitium**, never a skip.
   Otherwise the dead gate is merely relocated one level up. Reuse
   `CITATIO_IRRITA` or mint a sibling; the requirement is that it is
   never silent.
3. **The fragment definition site is not judged in place.** It sits
   wherever the writer parked it, which is not a legal position for its
   content, so judging it there would produce false vitia — the exact
   defect measured in §0.4. It is judged **through its uses**. A
   fragment that is never transcluded is therefore never judged; whether
   that is itself a vitium is §7.

**Mechanism.** Follow `augmentum` (§0.5), do not invent a second one:

- a **collection pass** builds the fragment table before judgment,
  because define-before-use is emergent from writer walk order and
  **never asserted** — it broke once already on
  `lib/arbor2_glr_tabula.c` (`silva_arbor.worklog.md:1283-1296`);
- resolution is by `fragmentum_id`, and failure is loud;
- resolved content re-enters ordinary judgment carrying **`parens_vi` =
  the transclusion's semantic parent**, so that `intra=`-scoped
  definitions resolve against the position of *use*. This is the
  augmentum bug that eight fixtures missed; inheriting it here is the
  single most likely way to get S2 wrong.

**Canon-gated.** Declared per dialect, refused in data dialects (§1.2).
`canon.canon` gains the declaration; the four data dialects do not.

**Bounds to respect.** Fragments nest at depth and may contain
transclusions. `_fragmentum_aperire` unwraps only **one** level
(`lib/stml.c:2846-2847`). Cycle detection is required — a fragment
transcluding itself must refuse, not recur.

**Gate.** Planted fault: a document whose transclusion resolves to
content that is *illegal at the point of use* must produce a vitium, and
the identical content written inline must produce the same vitium. That
equivalence **is** the definition of transparent, so it is the test.
Plus: an orphan `<<#nothing>>` must clamour.

### 3.3 What is deliberately NOT built here

- **Composite-key uniqueness** — §6.
- **Element groups / substitution classes.** With 62 locus names the
  enumeration is tractable; a group mechanism would touch codegen, fork
  cardinality semantics, and contradict "canon never learns
  inheritance". §6.
- **Co-occurrence constraints.** Decreed above canon twice already
  (`canon.canon` header; `grammatica.canon:41-44`). Arbor being a third
  asker is not a reason to reverse a decision made twice — and §0.1's
  union-over-productions loses the correlation anyway.
- **Document-declared schema version.** Arbor stamps
  `registrum-sigillum` and checks it itself (`silva_arbor.c:3525`).
  Canon captures `versio=` and never compares it. Not a gap arbor needs
  closed.

---

## 4. The projection

### 4.1 Where it lives

**In the generator, not in a new tool that re-parses the grammar.**
`silva/instrumenta/silva_coquere.c:691-731` already emits `%s_LOCI[]`,
`%s_GENERA[]`, `%s_REGISTRUM` from `SilvaGenGrammatica`. The `.canon`
emitter is a sibling function over the same structure, written out by
`silva/generare.sh` as a committed generated artifact alongside the
tables.

Follow the natura precedent's split (`res-praecedens` finding): the
**model** decides what may be said, the **emitter** only writes what the
model already says. Do not let the emitter make decisions.

### 4.2 What it emits

- **58 genus elements**, tags verbatim from `SilvaTabGenus.titulus`
  (`silva_arbor.c:1641`).
- **Locus elements**, tags verbatim from `SilvaTabLocus.titulus`
  (`silva_arbor.c:1671`), declared `intra=` their genus — mandatory,
  since 62 names cover 176 entries and a locus tag means something only
  relative to its parent genus.
- **Content models from the §0.1 derivation**, with these rules:
  - every locus is `minimum="0"` (§0.3);
  - `maximum="1"` for NODUS/TOKEN/INDEX species; unbounded for LISTA_*;
  - **`ambiguus` is admitted in every node-bearing locus and list** —
    the GLR fabrica may wrap any reduction value
    (`silva_glr.c:205`, `:268`, `:1327`);
  - INDEX loci carry numeric text.
- **95 `lex-*` token elements**, from the dense
  `SilvaLexemaGenus` enum via the same mangling the writer uses
  (`silva_arbor.c:389-411`), so the two cannot diverge. Text content only
  for the variable-spelling genera (`ORTHOGRAPHIAE` decides).
- **The envelope**, hand-authored: `parsura`, `arbor`, `fontes`, `fons`,
  `cauda`, `directiva`, `regio-*`, `invocatio-vacua`, trivia, and the
  six origin elements.
- **`<citatio>`**: `def-f`, `f`, `regio-fons`, `fons-princeps` all
  reference `fons/index`; `index` is the definition. This is the first
  heavy consumer of citatio outside natura.
- **`<unicitas attributum="index" super="fons">`** — single-attribute,
  works today.

**Hand-authored policy, not derived** — the five `<genera-extra>`
(`error`, `ambiguus`, `conditionalis`, `ramus-sumptus`,
`ramus-omissus`) carry `slots=` as an uninterpreted string
(`silva_generare.h:69-72`), so their 11 loci have no derived
constraints. §0.1 caveat 2 (ERROR/CONDITIONALIS root-level only) is
INFERRED from two construction sites: **encode it as policy and let the
corpus gate falsify it**, rather than trusting the inference.

### 4.3 Its own seal

Because `registrum-sigillum` does not cover the derivation inputs
(§0.2), the projected canon carries its own seal over **the grammar
productions, `NOMINA_GENERUM`, and `ORTHOGRAPHIAE`** in addition to the
genus and locus tables.

The gate must be a **planted fault at birth**: change a production, and
the freshness check must fail. A seal nobody has watched fail is a seal
nobody knows works — that is precisely how §0.2 arose.

### 4.4 Freshness

Copy `natura_canones.sh`, including the exit-code contract that research
corrected (`res-praecedens`, and now recorded in `canon-project.md`):

- `-probare`: 0 fresh / 1 stale / 2 nothing judged. **Never 3.**
- bare (regenerates): 0 whole chain fresh / 2 defect / **3 = canon
  written but downstream now stale**, and 3 also when the downstream
  gate could not run — *unknown is never reported as 0*.

The reasoning generalises and is why this is not optional: hooks fire on
**edits**, generators write via **Bash**, so no hook can observe the
staleness a regeneration creates. The tool that causes it is the only
thing positioned to report it — and it reports, never acts.

---

## 5. Gates

Per house law, every new refusal gets a planted fault **at birth**, not
after the fact.

| stage | gate |
|---|---|
| S1 | planted fault + control (§3.1); all 41 canons clean, 2,046 rules pinned |
| S2 | inline/transcluded equivalence (§3.2); orphan transclusion clamours; cycle refuses |
| S3 | generated canon is itself judged by `canon.canon` |
| S4 | **corpus judgment**, below |

**The S4 corpus gate is the real one.** Generate arbor documents from
both tiers — 73 plain-C `probationes/fixa/roundtrip/` files and the
latinized `lib/` tier — and judge every one. Expected: zero vitia.

Two properties this gate must have, both learned the hard way:

1. **It must be able to fail.** Before trusting a green sweep, corrupt a
   document (move a node to an illegal locus) and confirm a vitium. A
   sweep from an unfalsified gate measures nothing — this is why §0.6
   planted a fault before running 41 files.
2. **Judge by the cause tally, not the total.** A headline count can
   hold steady while the reasons underneath change completely.

**Apparatus refusal (inherited from `arbor-parsura-spec.md` §6.5).**
The latinized tier requires the include closure; without it nothing
expands and every number is confidently wrong. The harness must refuse
loudly when a file transitively including `latina.h` yields zero
EXPANSIO tokens. This has already failed twice.

---

## 6. Reserved, with landing spots (no silent caps)

| reserved | landing |
|---|---|
| composite-key uniqueness | real want, two askers on file (`canon.canon:20-27`); **not a dependency** (§0.7). Build when a dialect cannot ship without it |
| element groups / substitution classes | if the enumerated form proves unreadable in practice. Touches codegen — measure the pain first |
| co-occurrence | decreed above canon twice; would need that decree reversed, not merely a third asker |
| generated arbor reader | quaestiones-style differential oracle, if the hand-written reader ever drifts from the schema |
| judging `arbor` subtree documents | the projection covers the tag; whether subtree documents get their own envelope is untouched |

---

## 7. Open

- **Is an untranscluded fragment a vitium?** §3.2 judges fragments
  through their uses, so a defined-but-unused fragment is never judged.
  The writer's two-pass use count means it should not emit one — which
  makes this a cheap assertion, if we want it.
- **Does `<arbor>` (subtree) share the `<parsura>` canon or need its
  own?** Same question `arbor-parsura-spec.md` §8 leaves open for
  included files.
- **Where does the derivation's cycle-cutting show?** §0.1 cuts cycles
  in left-recursive expression ladders. That is sound for reachability
  but the resulting sets should be spot-checked against the corpus gate
  rather than trusted.
- **`ORTHOGRAPHIAE` as a third seal input** is asserted in §4.3 but the
  exact set of tables the derivation reads should be enumerated by the
  implementer and the seal built from *that* list, not from this one.
