# STML Exemplaria — spec v1 (the pattern layer)

**Status:** DESIGNATUM, nondum aedificatum.
**Fons:** ASSEMBLED from `project-specs/stml-exemplaria-interview.md`
(rounds I–V, the baubellarium simulation, the Q3 measurement, and the
seven decisions sealed at the formal interview 2026-08-27). Nothing in
this document is new design; where a section states a rule, the
interview file carries the reasoning and the alternatives that were
rejected. When this spec and the interview file disagree, the
interview file wins and this spec has a bug.
**Parcum:** 01M0ZR9AXN. **Extends:** `stml-macros-spec.md` (the engine
is the same engine; the CAPS builtin grammar is §6.2's).
**Vision:** `stml-visio.md` §9 (control plane), §10 (strangler
trajectory).

---

## §0 Identity

Macros let a document abbreviate itself; EXEMPLAR lets a document
observe itself; PER turns observations into content.

**THE INVARIANT: EXEMPLAR never appears in resolved STML.** A match
set is not emittable. Engine-space material is consumed, not emitted.
Resolution's contract is unchanged — resolved documents remain plain,
sigil-free, canon-judgeable content. What widens is where
instantiation ARGUMENTS come from: macro calls take them from the
call site; EXEMPLAR takes them from matches.

### §0.1 Standing law this spec sits on (all pre-decreed)

- **Pattern-cardinality thesis**: control flow comes from match
  cardinality. There is no loop construct and no reduce; `modus=`
  governs cardinality at the root; PER is per-row instantiation,
  not iteration.
- **The definition IS the pattern**: templates are patterns with
  zero permissiveness. One lockstep matcher serves template
  recognition (strict) and queries (loose); strict is the degenerate
  case, not a second engine.
- **The no-expression-language law**: everything is enumerable —
  literals, presence, structure. Never predicates, never arithmetic,
  never a string language. This is WHY canon can judge a pattern
  document: the language is total (document-order strata forbid
  recursion → termination guaranteed; single assignment →
  referential transparency).
- **The sigil triangle**: `$` binds (match side), `&@x;` fills
  (template side), `%` acts on what is bound (mutation side —
  future). Match-side and fill-side are visually distinct on the
  page, always.
- **Alias, never copy**: patterns BIND existing nodes. Whether a
  consumer copies (PER splice) or works in place (future verbs) is
  the consumer's business, never the pattern's.
- **Loose-by-default for queries**: in an EXEMPLAR body, written =
  required, omitted = unconstrained. (Templates remain
  strict-by-default; polarity is a property of the construct, at the
  root, never per-node.)
- **Least surprise** (V.5): every surface choice below was judged by
  the authoring party's expectation, human or LLM, with the syntax
  on the page before ratification.

---

## §1 The result type

A pattern application yields a SET OF MATCHES. Each match is a
BINDING ENVIRONMENT: named captures → values. Every match implicitly
binds its ROOT (the node where the pattern applied). All uses are
projections of this one type — nodes are root bindings, narrowing is
matching within bindings, booleans are set-non-emptiness, values are
captures. No per-use syntax exists.

### §1.1 The type ladder

Three rungs, every mismatch loud:

1. **scalar** — a captured VALUE (attribute value, text);
2. **forest** — a captured SUBTREE or sibling run;
3. **list-of-bindings** — the match set itself.

Lists are consumable only by PER (per-row instantiation) or
wholesale; a scalar position fed a list REFUSES with a named vitium
(the seventh-vitium pattern).

### §1.2 The C representation (sealed 2a)

In-memory only. PER is the SOLE v1 consumer.

```c
/* Ligamen unum: titulus internatus + unus e tribus gradibus */
nomen structura {
        chorda*  titulus;   /* internatum - aequalitas monstratoris */
     StmlNodus*  nodus;     /* gradus I: nodus; NIHIL si non */
         chorda  valor;     /* gradus II: valor; mensura 0 si non */
            Xar* silva;     /* gradus III: Xar de StmlNodus*; NIHIL */
} ExemplarLigamen;

/* Congruentia una: radix + ligamina eius */
nomen structura {
     StmlNodus*  radix;      /* semper ligatum */
            Xar* ligamina;   /* Xar de ExemplarLigamen */
} ExemplarCongruentia;

/* Copia congruentiarum = Xar de ExemplarCongruentia (ordo documenti) */
```

No serialized STML form of the relation in v1 — the lint REPORT is
already a document PER produces. The relation SCHEMA (provenance
columns, canon-judged rows, serialized form) is designed at the
relational increment (§8.6), not here.

### §1.3 Provenance (sealed 2b, measurement-grounded)

v1 matches carry the matched NODES plus `linea` — which SURVIVES
expansion cloning (measured; `_duplicare_recursivum` copies linea and
zeroes positus). Lint reports cite file + line.

**THE DECREED OBLIGATION — the extent provenance rule.** The Q3
measurement found the hole at 100%: `stml_expandere` superficially
duplicates every node it walks and deliberately zeroes
`positus_initium/finis` on each clone, so the expanded content view
carries ZERO byte extents; and the writer-side `sedes_valorum` joins
only uncompressed values (31% of parametrum sites). Therefore:

> Before any transform or mutation verb rides EXEMPLAR matches, the
> engine MUST gain an extent provenance rule: either positus carried
> through expansion for untouched/moved content, or an
> expanded↔document side table (the `tabula_expansionum` precedent).
> Decreed now; BUILT when the first transform consumer pulls.

This obligation is spec text precisely so it cannot be forgotten: a
v1 consumer needing only reports rides linea; nothing that WRITES may
ride matches until extents exist.

---

## §2 Surface — the EXEMPLAR element

```stml
<TRANSPARENTIA tags="expansio pasta stringificatio api extentum
                     ante post"/>

<EXEMPLAR de="$scope" modus="omnia" output="$m" [ancorata]>
  ...corpus exemplaris (unum elementum)...
</EXEMPLAR>

<PER congruentia="$m">
  ...templum - capturae ut argumenta adveniunt...
</PER>
```

EXEMPLAR is a CAPS builtin under §6.2's builtin-verb grammar. The
engine acts only on `@`-space and CAPS builtins; unmarked content is
never touched (the boundary law, unchanged by interleaving).

### §2.1 `de=` — the scope (sealed 1b)

| form | meaning |
|---|---|
| absent | the document (the enclosing content tree) |
| `de="$x"` | a prior EXEMPLAR's `output=` relation — piping is naming the previous output; application is per-row, extending the input rows |

`de="@arg"` (a macro argument becoming a scope — the argument→scope
conversion) is a NAMED DOOR for the composition/dispatch increment
(§8.5). An unknown `de=` name is a loud vitium.

### §2.2 `output=` — required, sigil-typed

`output="$name"` binds the match relation. REQUIRED: a match set is
not emittable, so a pattern without `output=` is dead code — loud
vitium, likewise an `output=` no later content consumes.
Document-order stratification governs the name: single assignment,
referenceable only BELOW the binding. Cycles are impossible by
construction. (`output="@forest"` — a deferred forest from
PER/COMMUTATIO — already exists on the template side and is
unchanged.)

### §2.3 `modus=` — match cardinality (sealed 4a/4c)

| modus | meaning | edge |
|---|---|---|
| absent | = `omnia` | — |
| `omnia` | all matches, document order | zero matches = empty relation, lawful |
| `unum` | exactly one | 0 or ≥2 = loud vitium |
| `primum` | first in document order | zero = empty, lawful |
| `optional` | zero or one | ≥2 = loud vitium |

The quantifier vocabulary is ONE vocabulary — the same four words the
mutation layer will read transactionally (unum = exactly-one-or-
refuse; fan-out failure = abort-all). Decreed here so the two sides
can never drift.

### §2.4 Application: floating by default, `ancorata` flag (sealed 3a/3b)

- **Absent** (floating): the engine tries the pattern at EVERY
  subtree root within the scope. This is the query case, and what
  every measured Q3 walk was. Floating pattern bodies must be
  SINGLE-ROOTED; a forest body under floating application is a loud
  vitium naming the alternatives (wrap the siblings in their real
  parent element, or use `ancorata`). Sibling-run matching is a
  named future increment.
- **`ancorata`** (bare flag): the scope root itself must match — the
  dispatch case.

Rationale, recorded: anchored-by-default would hand the bare query
zero matches against a `<parsura>` root — the exact
least-surprise failure V.5 exists to prevent.

### §2.5 Captures

- `$x` as a BOOLEAN attribute binds the NODE: `<parametrum $par>`.
- `$x` in a VALUE position binds the VALUE: `n="$n"`.
- A repeated `$x` is an EQUALITY constraint (non-linear pattern):
  bound forests/values must be equal — the parametrum matcher's
  forests-equal rule, which is also the mutation layer's
  token-series equality. One definition, both sides.
- Every match implicitly binds its root; explicit `$` names are for
  everything else.

### §2.6 Loose matching semantics (sealed 4b + the loose law)

Within the pattern body (always loose in v1):

- **Element**: tag must equal (interned comparison); written
  attributes are required (literal value must equal; `$x` binds);
  omitted attributes unconstrained.
- **Children = ORDERED SUBSEQUENCE**: written children must each
  match some candidate child, in the same relative order, gaps
  allowed. Omitted children unconstrained. (AST child order is
  meaning; a lint that says "A before C" can say so. Unordered
  existential and exact-run were weighed and rejected — interview
  file, decision 4b.)
- **`<*/>`** matches any ONE element (one lockstep rule; quaestio's
  UNIVERSALIS vocabulary).
- **Text** nodes match by equality; a `$x` whole-text child binds
  the text (scalar) — mirroring the argument-map classification.

NOT in v1 (all reserved, §8): `<**>` descendant wildcards; `? * +`
occurrence suffixes; strict-mode surface (`strictum` flag); the
per-element strictness escape hatch.

### §2.7 TRANSPARENTIA (sealed 6b — the measurement-earned decree)

A document-level CAPS declaration, strata-scoped like template
definitions:

```stml
<TRANSPARENTIA tags="expansio pasta stringificatio api extentum
                     ante post"/>
```

Loose matching DOES NOT SEE into the subtrees of declared-transparent
tags — they are metadata, not content. `@`-space (template
definitions, argument-elements) is engine-inherent transparency, not
declared. The engine stays dialect-ignorant: it reads the
declaration, hardcodes no tag names; dialect knowledge lives in
dialect documents (arbor lint documents declare arbor's origin
wrappers and trivia, shareable as a fragment across lint docs).

Why this is load-bearing and not hygiene, measured: without these
skips a loose query for identifier `NULL` also matches the `NIHIL`
spelling inside the token's own `<expansio>` wrapper — wrong content,
wrong sites, silently. (Canon-declared transparency was rejected for
the circular runtime coupling; the per-EXEMPLAR attribute was
rejected because forgetting it is a silent wrong-content bug.)

Scope note: transparency governs MATCHING only. It does not affect
emission, distribution, or canon.

---

## §3 The bridge — PER

**THE BRIDGE IS THE SCOPING MECHANISM.** Inside a per-match consumer,
each match's captures arrive as that instantiation's ARGUMENT MAP —
the same mechanism a macro call uses. `$n` in the pattern, `&@n;` in
the template, the builtin is the bridge. There are NO new
variable-scoping rules anywhere in this spec.

```stml
<EXEMPLAR modus="omnia" output="$sectiones">
  <sectio titulus="$t"/>
</EXEMPLAR>
<index>
  <PER congruentia="$sectiones"><res>&@t;</res></PER>
</index>
```

- `<PER congruentia="$m">` instantiates its body once per match, in
  match (document) order; captures fill `&@x;` references through the
  existing argument map — node/forest captures splice as subtree
  arguments (MOVED-vs-cloned follows the existing engine rules for
  argument material), scalar captures fill as strings.
- Delegation `voca="#@f"` is free: captures-as-arguments IS a call.
- Zero matches → PER contributes nothing (empty forest), lawful.
- Aggregations do not exist; when they arrive they are decreed
  projections (§8.8), never expressions.

Strata safety (the L2 door): EXEMPLAR observes only content ABOVE
itself in document order, and a spliced result can never be
re-matched — backward-looking by construction, cycles impossible.
This is what makes in-document derivation lawful by the macro
precedent rather than the computed-views violation.

---

## §4 Engine — the interleaved model (sealed 1a/6a)

- **Interleaved**: `stml_expandere` resolves EXEMPLAR where it finds
  it during the one expansion walk. A lint over a corpus document is
  ONE pure call: `expandere(lint-doc + corpus-doc) → report-doc`.
  There is no separate pattern engine, no second evaluation time.
- **Home**: `lib/stml_macros.c`. No new object file (the
  canon.o→stml_macros.o hand-link-list lesson). Split later only if
  taste demands, behind the same header.
- **The matcher**: `_par_congruere` (silva_arbor.c) PROMOTED into the
  engine as the general lockstep matcher — that is the strict mode,
  already corpus-hardened. Loose mode (§2.6) is built beside it in
  the same walk shape. Parametrum recognition is rebuilt ON the
  promoted matcher. Definition of done: the plagula gate (78 + 155
  byte-exact + compression presence) green through the extracted
  matcher.
- **Chain position**: legere → expandere (EXEMPLAR resolves here) →
  distribuere → consumer. The instrumentum-spec §3.1 chain laws bind
  this engine verbatim: ONE internamentum per chain (ids meet at a
  raw pointer compare); gate on `successus`, never on the root
  pointer (soft vitia); feed the DOCUMENTUM root; the pre-expansion
  element root is stale afterwards.
- **Tag comparison is interned-pointer equality** — the tree's own
  intern. Measured: naive byte-compares cost a further 1.3–1.5× on
  top of the walk. A pattern tag absent from the intern simply never
  matches; the differential gate (§7) guards the optimization.
- **Identity rules during matching** (probe-proven): content
  fragments (`<#lexN>`) are counted at their DEFINITION;
  transclusions are skipped (the same token, not a second site);
  sites dedup by node identity — which also makes matching correct
  across ambiguus arms sharing subtrees.
- **Determinism**: expansion is a pure function of (document + scope
  snapshot). With the v1 `de=` set, every scope is in-document, so
  v1 expansion remains a pure function of the document alone.
- **ADNEXIO PRAECOX** (build finding, 2026-08-31): the walk
  attaches each shallow duplicate to its parent BEFORE filling its
  children, so the spine above the current position is always
  linked and the partial expanded tree IS "content above" —
  without this the scope law holds only in prose (measured: every
  floating match saw an empty tree). Final trees are unchanged;
  only mid-walk visibility differs. Details + the two lexer
  blessings (`*` tag, `$` attribute name) and the edge laws set at
  build time: lib/stml_macros.worklog.md 2026-08-31.

### §4.1 Performance envelope (measured, Q3 2026-08-27)

Recorded as the engine's expectations, not budgets: warm loose
matching over the projected content view runs 2–11× a native silva
walk (interned tags; the projected tree carries 2.7× the nodes);
projection costs 4.4× parse, once per file change; warm-vs-cold is
45×, so RESIDENT/CACHED expanded documents are a named obligation of
the one-engine verdict; document size (32× source, pretty-dominated)
has the pulchrum lever when it matters. Full numbers: interview file,
"Q3 mensura peracta".

---

## §5 Vitia

All new vitia append to the existing stml_macros vitium enum;
numbering assigned at build time; every one carries a line. Names
provisional until code:

| vitium | fires when |
|---|---|
| `EXEMPLAR_SINE_EXITU` | EXEMPLAR without `output=` |
| `EXITUS_NON_CONSUMPTUS` | an `output=` nothing below consumes |
| `SCOPUS_IGNOTUS` | `de=` names no prior output |
| `CORPUS_SILVESTRE` | forest body under floating application |
| `UNUM_VIOLATUM` | `modus="unum"` with 0 or ≥2 matches |
| `OPTIONAL_MULTIPLEX` | `modus="optional"` with ≥2 matches |
| `LISTA_IN_SCALARI` | a list fed to a scalar position (ladder breach) |
| `TRANSPARENTIA_MALFORMATA` | malformed declaration (empty tags, duplicate declaration conflicts) |

Soft-vitium discipline follows the engine's existing END-GUARD
pattern: the walk continues, `successus` falls at the end, consumers
gate on `successus`.

---

## §6 Canon

Pattern documents are STML documents; canon judges them — this is a
one-engine dividend, not new machinery. EXEMPLAR/PER/TRANSPARENTIA
enter the CAPS builtin vocabulary the way COMMUTATIO did: attributes
enumerable, arms/body statically declared, nothing requires
evaluating anything to judge the plan. The `$`-capture forms are
lexical (attribute names / values), judgeable as spelling. Canon
work lands with the build, not before.

---

## §7 Consumers and gates (sealed 7a/7b)

Each level inherits its oracle from the previous one: identity,
then differential, then derivation.

### §7.1 L0 — parametrum recognition (changes nothing, on purpose)

Recognition rebuilt on the promoted matcher; resolved output
byte-identical; the corpus is the oracle (plagula gate 78 + 155 +
compression presence). Authorship migrates from a C recognizer
toward declared patterns; bytes prove nothing moved.

### §7.2 L1 — the first lint (a probe query promoted)

One of the measured queries (si-vs-NIHIL, or
piscina_allocare-with-magnitudo) becomes the first lint DOCUMENT:
EXEMPLAR + TRANSPARENTIA + PER producing a report document. Its
differential oracle ALREADY EXISTS and is green: the Q3 probe's
silva-side walk (233/233 files, identical site multisets). The gate
is site-set equality against that walk, plus report-document shape.

The SECOND lint re-expresses an examen codex, chosen at build time by
fit (v1 subset; has a C oracle to gate against differentially).

**LINT II BUILT + GREEN 2026-08-31** (ffcc6837;
probatio_silva_exemplaria_obsoleta.c; fixture
fixa/exemplaria/obsoletum_usleep.stml): **codex 87
OBSOLETUM_ADHIBITUM** (usleep — the one-entry `<gradus>` table) as
one TRANSPARENTIA row (L1 wrappers + regio-cruda) + one EXEMPLAR +
one relatum. The oracle is the SHIPPING ANALYZER — the examen
recipe verbatim in the probatio (the apparatus path composes no
systema, so the codex never fires on it = false-green; the recipe
copy is load-bearing). ONE parse feeds BOTH judges. 155 lib + 6
named probationes files, 27/27 sites, ZERO divergence on the
first full run; count-equality only (positions wait on §1.3).
Survey residue: codices 69/70 are ANTIJOIN-shaped (§8 relational
door's confirmed pull); codex 83 wants prefix matching (a
pattern-vocabulary door). Selection + scouting record: interview
file "Lint II candidate chosen".

**BUILT + GREEN 2026-08-31** (probatio_silva_exemplaria_lint.c;
fixture fixa/exemplaria/conditio_nihilum.stml): 233/233 files,
3,639 sites both ways, apparatus gate clean, invariant asserted.
The gate's first catch was a SEMANTICS clarification, now law:
**relation counting is PER-ROW — an occurrence inside nested
conditio scopes (ternarius-in-condition; one site in the corpus)
is observed once per enclosing row**, and an oracle must mirror
that, not count distinct tokens. Details:
lib/stml_macros.worklog.md 2026-08-31 (L1 entry).

### §7.3 L2 — the door (not v1 build)

In-document derivation (self-maintaining indexes, summary tables;
eventually arbor `<fontes>`, speculum listings). Lawful by §3's
strata argument; needs floating application (have it). Follows its
own pull.

---

## §8 Reserved doors (all sealed 7c — none in v1)

Each door is NAMED with its pull condition so deferral stays a
decision:

1. **`strictum` flag** — strict-mode EXEMPLAR surface. Pull: a
   document consumer needing exact-match semantics (today strict
   lives where it lives — templates run backwards).
2. **The loose escape hatch** ("exactly these children, no others").
   Reserved UNSPELLED — per-node polarity is what the root-polarity
   law warns against; the spelling gets its own care at pull time.
3. **`<**>` + `? * +` suffixes** — descendant wildcards and
   occurrence quantifiers. Pull: a pattern needing a descendant
   constraint INSIDE its body (not at its root — floating covers
   that), or counted children. Ranges will be attributes
   (`minime=`/`maxime=`), never `{n,m}`.
4. **Sibling-run floating** — forest bodies under floating. Pull: a
   statement-sequence lint.
5. **`de="@arg"` argument→scope** — the missing bridge direction.
   Pull: EXEMPLAR inside reusable macro definitions; the dialect
   dispatch scenario (with §8.7). Includes declared capture
   signatures (three independent pulls recorded).
6. **The relational layer** — serialized relation schema (provenance
   columns, canon-judged rows), ANTIJOIN/difference, joins
   (`$abc.n`). Antijoin comes FIRST when this opens (inequality =
   equality + set difference — the no-expression law's survival
   route). Pull: the control plane, or the first cross-document
   lint.
7. **CASUS pattern-cases** — `<CASUS><@est=><EXEMPLAR>…` block-form
   on builtins; spelling decreed 2026-08-26; PROMOTED load-bearing
   (dialect dispatch = typeclass dispatch on root constructors).
   Pull: the first multi-dialect macro.
8. **Aggregates** — decreed projections
   (`numerus/summa/minimum/maximum`), the closed-verb pattern's
   fourth appearance. Pull: the first business-data consumer — and
   the EXEMPLAR-vs-store query fork gets its own SEPARATE
   measurement first (interview item 13).
9. **Mutation verbs** — `%` = the in-place consumer family of
   matches; positional form = sugar over the bridge (one IR,
   unratified synthesis); transactional quantifier reading; BLOCKED
   on the §1.3 extent provenance rule by decree.
10. **PARSE ingress** — both forms; singleton-relation reading;
    static ingress enumerability (fabrica rhyme); the inline form =
    the quasi-quotation/Coccinelle door (per-dialect hole-blessing).
    Pull: the control plane / first out-of-document lint run.
11. **DISTRIBUTIO-identity decree** (interview item 12) and the
    **user-raisable vitium** (`<VITIUM nuntius=>`) — each waits for
    its consumer.
12. **CATENA — BUILT 2026-08-31** (decree: interview file "CATENA
    sketched" + addenda + build-time ratifications; commits
    88fe15d7→5359c5e2) — pipe sugar over named chaining: stages
    thread output→scope implicitly; **`output=` lives on the
    CATENA element**, wrapper also takes `de=` under EXEMPLAR's
    law; links carry NO de=/output= (a link output= is the
    RESERVED tee, vitium-loud) but MAY carry modus=; engine reads
    the wrapper form (COMMUTATIO precedent). The minted-names
    desugaring is the MENTAL MODEL — the code threads the relation
    pointer stage-to-stage ("consumed by construction" literally
    true; only the tail registers, so XVIII concentrates there).
    Nested chains FLATTEN; links may be macros expanding to
    EXEMPLAR/CATENA (forest splices); literal links used RAW,
    macro-produced links get instantiation semantics — the
    conveyance rule (ctx->applicatio, vitium-XIII refinement).
    `<(>` = permitted decoration (fragments dissolve; the sketch
    spelling works verbatim). **THE PURITY LAW (ratified): every
    link is a PURE relation→relation function — conservative link
    vocabulary, mutation verbs excluded; effects consume the TAIL
    relation at one point outside the chain** (verb-as-consumer,
    never verb-as-stage). Vitium XX = CATENA_MALFORMATA; canon
    CAPS row judges the static form (links in LINK form, bodies
    quoted). GATE GREEN: conditio_nihilum_catena.stml (the `<(>`
    spelling) yields a byte-identical relatum vs the named form
    over 233/233 plagulae, 3691 sites. Open sub-questions: the tee
    (reserved); capture threading across links (second pull on
    V.4).
13. **DIRIBITIO — BUILT 2026-08-31, same night** (decree +
    ratifications: interview file "pure-command CLASS" section;
    commits 0c4d159b→ae55eb75) — the pure-plane switch closing the
    command class: set-level GUARDED dispatch (condition in an
    `<EST>` CAPS element — the pinned `<@est=>` spelling CANNOT
    parse outside invocation position, §6.3; revised + backported
    to the fill door), first-congruent-arm-wins, ORDINARIUS
    optional-always-LAST, arm receives the ORIGINAL relation,
    no-arm = empty relation out, condition roots bare (ancorata
    allowed), arm = one pure command. Works at document level
    (CATENA's wrapper law), as a CATENA link, and recursively in
    est slots; conditions and arms may be EXEMPLAR/CATENA/
    DIRIBITIO or templates expanding to those. Vitium XXI; canon
    CAPS row (`_mandatum_forma_iudicare` = the class's static
    link-form judge). GATE GREEN: lint II as a self-guarded chain,
    byte-identical relatum, 161/161. Doors that stay named:
    per-row routing, alternation (first-non-empty), capture flow
    condition→arm, guard-narrows-arm, COMMUTATIO est= → `<EST>`
    migration.
14. **SINE — BUILT 2026-08-31, third night** (decree: interview
    "SINE seed" + relational-door rounds; commits 6ea2eccb,
    8f0bbbda, 14ea9405, + gate) — the antijoin as a pure chain
    FILTER: body = one bare pattern, `&@n;` bridge-filled per row
    (THE BRIDGE IS THE JOIN — fully correlated NOT-EXISTS, no key
    machinery), filled pattern petitioned floating within the
    ROW'S SUBTREE, zero matches = row kept UNCHANGED. Vitium
    XXIII (engine attrs / malformed body / fons NIHIL — document-
    wide absence stays a named door, loud); canon CAPS link row.
    THE GATE DELIVERED: codex 69 (variabilis inutilis) as a
    three-chain pattern document vs the examen recipe in-process —
    first run 865 false sites, all one insight (the bridge
    promotes captured DOCUMENT material into the pattern plane,
    carrying representation baggage hand-written patterns never
    had), killed by three matcher refinements: symmetric blindness
    (forma children filtered like candidate children),
    `TRANSPARENTIA attributa=` (declared attribute blindness for
    provenance b/linea/columna/f), and content-alias resolution
    (`<<#lexN>>` resolved in effective children; sharing =
    representation). Final: EXACT agreement, 206 files, 0
    divergence, L1 + lint II byte gates intact. Named doors:
    document-scope SINE, uncorrelated named-relation subtract,
    pure-command SINE bodies, anchored SINE, petere-through-alias
    enumeration, second-same-shape-declaration rows (multi-binding
    enumeration).

---

## §9 Build order

1. **Matcher extraction** — promote `_par_congruere` into
   lib/stml_macros.c (strict mode, general names); parametrum
   recognition in silva_arbor.c becomes a CALLER of the lib-side
   matcher. Gate: plagula gate green through it, byte-exact.
2. **Loose mode + EXEMPLAR/PER + TRANSPARENTIA** — the §2/§3/§4
   surface in the engine, vitia of §5, probationes: unit fixtures
   per rule (subsequence, `<*>`, equality-constraint, modus edges,
   every vitium) + the L1 differential gate wiring.
3. **L0** — recognition rebuilt on the matcher (identity gate).
4. **L1** — the promoted probe-query lint end-to-end (differential
   gate + report shape). Canon vocabulary lands here.

Post-v1, in pull order: lint II (examen codex), then §8 doors as
their consumers arrive.

### §9.1 The materia interaction (recorded 2026-08-31)

Silva is frozen while `materia` (the general parse substrate) forks
out of it; silva_arbor migrates into materia at that project's phase
5. Ruling (Fran): the fork is ENVIRONMENT for this spec, not a
dependency — proceed, review materia separately later. Specifics:

- The engine home (lib/stml_macros.c) is lib-side: untouched by the
  freeze.
- Step 1 is SUBTRACTIVE on silva_arbor.c (~150 lines leave): it
  shrinks the very file the fork must migrate.
- Step 3 modifies frozen silva_arbor.c; per the freeze protocol that
  work replays into materia's arbor when the migration lands — made
  small by the matcher being lib-side (the recognizer is a thin
  caller either way).
- The plagula gate remains the gate wherever arbor lives; if the
  migration lands first, step 3 targets materia's arbor and the gate
  moves with it.

---

## §10 What done looks like

v1 is DONE when: the plagula gate is green through the extracted
matcher (L0); one lint document produces its report through one pure
expandere call with site-set equality against the C oracle (L1);
every §5 vitium has a firing fixture; canon judges the two lint
documents; and no EXEMPLAR, PER, or TRANSPARENTIA element survives
into any resolved document (§0's invariant, asserted in the gates).

**v1 COMPLETE 2026-08-31.** L0: plagula 78+155 byte-exact through
the engine matcher (step 1). Surface + vitia: 51/51 fixtures
(step 2; step 3 absorbed). L1: 233/233 files, 3,639 sites both
ways, invariant asserted (step 4). Canon: the CAPS builtins enter
canon ENGINE-SIDE (the template-space precedent) —
`CANON_MACHINAE_MALFORMATUM` (XXIII) judges their form statically
at load; well-formed CAPS are INVISIBLE to every dialect's
vocabulary (the fragment/augmentation precedent); pattern bodies
are QUOTED, never judged against the calling canon (per-dialect
patterns doctrine); gated in probatio_canon.c (354/354) with the
lint-document shape judged clean and five malformed constructs
each raising exactly one XXIII. **The EXPANDED SENSE is judged
too, and pinned**: `canon_iudicare_expansum` EXECUTES the commands
(interleaving put pattern resolution inside stml_expandere) and
judges the result — including a vitium in content that exists
ONLY after execution (a PER splicing an illegal element), and
failed execution surfacing as CANON_EXPANSIO_FRACTA with the
engine's vitium number. Pinning this exposed and fixed a
coverage hole: canon_iudicare judged only the FIRST root of a
document node — lint documents are multi-root, so all top-level
elements are now judged (fragments collected across roots;
radix-check/infixum/key-scopes stay first-root, their consumers
being single-root dialects — a named narrowing). Lint II
(post-v1, §7.2) will be judged by this same vocabulary when it
lands.
