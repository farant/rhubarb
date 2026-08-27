# html_arbor — the HTML tree, and the substrate's second client

*2026-08-27. Designed with Fran. Third slice of
`project-specs/css-html-js-spec.md`; sibling of
`project-specs/css-arbor-spec.md`, whose decisions D1-D7 it inherits
except where stated.*

**This spec has two jobs.** It designs `html_arbor`, and it is the
**S6 evaluation instrument** — written before any CSS code exists,
precisely to find out whether the substrate seam designed for one
client survives a second one. Fran's proposal, and it has already paid
for itself: §2 records four findings, two of which correct the CSS
spec and its plan.

---

## 0. Decisions

Inherited from `css-arbor-spec.md` unchanged: D1 (hand-written
recursive descent), D2 (build `SilvaNodus`, reuse the projection),
D4 (sibling subsystem, consuming silva through its amalgam), D5
(reuse ladder S2+S3+S4), D6 (defer the S6 extraction).

New, or differing:

| # | decision | rationale |
|---|---|---|
| H1 | **v1 is the simple builder** — void elements, implied closes, case-insensitive matching, raw-text containment | The parent spec's line. §6 costs the full-parity path so the deferral is a schedule, not a wall. |
| H2 | **The format is designed for full parity even though v1 does not implement it** | Fran's call. A format migration later is far more expensive than reserved vocabulary now. §6.3. |
| H3 | **Parity means tree SHAPE, never value normalization** | The DOM has decoded entities and lowercased tags. Our contract is verbatim spans. If parity ever meant normalized values, byte-exactness would die instantly. Normalized views are DERIVED. |
| H4 | **No synthetic tokens in v1** | An implied close is recorded by an ABSENT `tok_clausura` slot, not a manufactured token. §4.2. |
| H5 | **Reserved vocabulary is REFUSED, not ignored** | A v1 that silently accepted an anchor it does not honour would lie. Same law as arbor's seal mismatch. |
| H6 | **Text is a node, whitespace outside tags is text** | Not a choice — `html_lexema` already decided it. §5. |

---

## 1. What exists

`html_lexema` shipped 2026-08-18: `include/html_lexema.h` (126 lines),
`lib/html_lexema.c` (825), `probationes/probatio_html_lexema.c` (533).
Fixtures: `probationes/fixa/html/{adversarius, laboratorium_index_2026-08-18, villa_index_2026-08-18}.html`.

Twenty-two genera plus the sentinel (23 enum members), total lexing, byte fidelity via
`html_lexemata_emittere`, positions in bytes, case recorded not
normalized, references undecoded.

---

## 2. THE SEAM PROBE — what this exercise found

Four findings. Each was reached by reading, not by building.

### 2.1 A pre-registered prediction, FALSIFIED

Before writing this spec I predicted HTML would force **parser→lexer
feedback**, because the tokenizer must know when it is inside raw text
(`script`, `style`) or RCDATA (`title`, `textarea`) — and that CSS's
one-way `lexare → adaptare → parsare` pipe would not survive it.

**Wrong.** `html_lexema` already solved it internally. Its header:

> *"Lexator modum trans lexemata servat"* — the lexer keeps mode
> across tokens.

The raw-text element set is **closed** and hardcoded at
`lib/html_lexema.c:192-195`. Feedback is only structurally required
when the trigger set is *open*. It isn't for HTML.

**The one-way pipe survives.** The prediction likely still holds for
JS, where regex-vs-division depends on arbitrary preceding grammar
rather than a word list — that is where the pipe will actually be
tested.

### 2.2 The lexicon seam has a C-shaped field in it — CORRECTS THE CSS PLAN

`css-arbor-plan.md` Task 4 defines `SilvaLexicon` with a
`genus_identificator` member. That field is wrong.

`SILVA_LEX_IDENTIFICATOR` appears at `silva_arbor.c:2851` in exactly
one context: synthesizing a **macro-definition-site token** while
reconstructing an origin chain from `<expansio def-f=… def-c=…>`.
Pure preprocessor machinery. Nothing but C ever reaches that code.

Left as designed, CSS and HTML would each supply a value that is never
read — the substrate making frontends answer C's questions, which is
the exact leak S6 exists to remove.

**Remedy:** drop `genus_identificator` from `SilvaLexicon`; the origin
reader keeps `SILVA_LEX_IDENTIFICATOR` as a local C constant, since it
only ever runs on origin genera that only C produces.

### 2.3 The coupling census is a floor, second confirmation

G4 already found that `silva_generare.c:1315` requires five
**structural node genera** by name — `ambiguus`, `error`,
`conditionalis`, `ramus-sumptus`, `ramus-omissus` — three of them
modelling the C preprocessor.

HTML hits this too. It must declare `conditionalis` and
`ramus-omissus` to be accepted, despite having no preprocessor. **Two
clients, same wart** — which is what promotes it from a CSS quirk to
an S6 work item.

### 2.4 Full parity needs ONE bounded substrate extension — located precisely

This is the finding Fran's "don't paint us into a corner" question was
actually about, and it took two passes to get right.

**First answer (wrong):** foster parenting is the same problem as C
directives interrupting the token stream, so silva's anchors and
gap-aware cursor already cover it.

**Corrected by reading `silva_scribere.c:760`:** `silva_scribere_fontem`
sorts `st.reinserenda` by offset and pours it into a structural tree
walk. But *reinserenda* are **non-tree** bytes. Foster parenting moves
**tree** content — a node whose byte position differs from its tree
position. The tree walk would emit it at its tree position, and the
round trip would fail.

**So the honest statement:** the machinery is the right *shape* but
does not cover this case. Full parity needs the reinserenda mechanism
taught to carry tree content emitted out of place — the node emits
nothing in situ and registers its bytes at their true offset.

That is a **named, bounded extension**, not a rewrite and not a corner.
It is also the single strongest argument for H2: the extension is
cheap, but a *format* that cannot express it is expensive.

### 2.5 Verdict on the seam

The seam **survives a second client**, with one interface correction
(§2.2) and one substrate work item deferred behind a feature nobody
has asked for yet (§2.4).

Confirmations, not assumptions: `LISTA_MIXTA` is genuinely exercised
by C89 (56 sites in `silva_tabulae_c89.c`), so HTML's mixed content
breaks no new ground; `SilvaNodus` slot absence is the canonical
default (`canon` cardinality: *"absentia loci canonica est"*), so H4
needs nothing built.

---

## 3. Architecture

Identical in shape to CSS, which is itself a result worth noting.

```
bytes
  ├─ html_lexare                 lib/html_lexema.c   EXISTS     Xar<HtmlLexema>
  ├─ html_adaptare               html/fontes/        new        Xar<SilvaToken*>
  ├─ html_arbor_parsare          html/fontes/        new, RD    SilvaNodus*
  ├─ silva_arbor_scribere_nodum  silva/              seam only  STML
  ├─ silva_scribere_nodum        silva/              UNCHANGED  bytes
  └─ html.canon                  html/grammatica/    generated  schema
```

| module | owns |
|---|---|
| `html_lexicon` | the `SilvaLexicon` instance for HTML |
| `html_adaptare` | `HtmlLexema` → `SilvaToken*` |
| `html_arbor` | the simple nesting builder |
| `html/grammatica/html.stml` | node vocabulary, `<genera-extra>` + the vestigial production (G4's shape) |

**No `html_selector` module.** CSS needed one because selector matching
is a shared future; HTML has no equivalent sub-language. Attributes are
part of the element genus.

---

## 4. Node vocabulary

### 4.1 Genera

```xml
<genera-extra>
  <!-- QUINQUE structuralia a generatore requisita (G4) -->
  <genus titulus="error"         slots="tokens:lista-token"/>
  <genus titulus="ambiguus"      slots="interpretationes:lista-nodus canonica:index"/>
  <genus titulus="conditionalis" slots="rami:lista-nodus finis:lista-token"/>
  <genus titulus="ramus-sumptus" slots="directiva:lista-token contentum:lista-nodus conditio_id:index"/>
  <genus titulus="ramus-omissus" slots="directiva:lista-token cruda:lista-token conditio_id:index"/>

  <!-- Structura documenti -->
  <genus titulus="documentum" slots="prologus:lista-nodus radix:nodus cauda:token"/>
  <genus titulus="doctype"    slots="tok:token"/>
  <genus titulus="elementum"  slots="tok_apertura:token attributa:lista-nodus
                                     tok_finis:token liberi:lista-nodus
                                     tok_clausura:token tok_clausura_finis:token"/>
  <genus titulus="attributum" slots="tok_nomen:token tok_aequale:token
                                     tok_valor:token"/>

  <!-- Contentum -->
  <genus titulus="textus"          slots="tok:token"/>
  <genus titulus="referentia"      slots="tok:token"/>
  <genus titulus="textus-crudus"   slots="tok:token"/>
  <genus titulus="commentarium"    slots="tok:token"/>
  <genus titulus="cdata"           slots="tok:token"/>

  <!-- Robustitas -->
  <genus titulus="elementum-malum" slots="tokens:lista-token"/>
</genera-extra>
```

### 4.2 The absent-slot invariant (H4)

**Every one of `elementum`'s six token slots is optional, and that is a
stated invariant, not an accident.** v1 exercises it immediately:

| source | which slots are NIHIL |
|---|---|
| `<br>` (void) | `tok_clausura`, `tok_clausura_finis` |
| `<p>a<p>b` (implied close) | first `p`'s `tok_clausura`, `tok_clausura_finis` |
| `<div>` at EOF (unclosed) | same |
| `<img/>` | `tok_clausura*`; `tok_finis` holds `/>` |
| *(future)* an adoption-agency clone | `tok_apertura`, `attributa`, `tok_finis` too |

The last row is the point: a clone is *"an element whose tags are
elsewhere"*, which the model already expresses. Full parity's
mechanisms 2 and 3 (§6.1) need **no new genus and no new slot** —
which is why H4 is worth writing down rather than discovering later.

An absent slot emits nothing, so byte-exactness is preserved by
construction rather than by care.

---

## 5. Text and trivia — settled by the lexer, not by us

CSS needed decision D7 (the hybrid rule) because CSS Syntax L3 and
silva disagreed about whitespace. **HTML needs no such decision**,
because `html_lexema` already made it: `HTML_LEX_SPATIA` is *trivia
intra tag* only. Whitespace **outside** tags is part of
`HTML_LEX_TEXTUS`.

Consequences:

- **HTML's trivia surface is tiny** — inside tags only, between the
  tag name, attributes, and the closing `>`.
- **Everything else is content**: text, references, raw text,
  comments, CDATA all become nodes in `liberi`.
- `liberi` is `lista-nodus`, not `lista-mixta` — text is wrapped in a
  `textus` node exactly as CSS wraps preserved tokens in
  `lexema-servatum`. Two languages, one shape; that consistency is
  itself mild evidence the substrate fits.

So D7's seam — *"inside a value versus everywhere else"*, the rule
that needed its own gate in CSS — **does not exist in HTML.** The
whitespace problem was CSS-specific, not substrate-general. Worth
recording, because it was the design point I was least sure of.

---

## 6. Full parity: the costed roadmap (H2)

### 6.1 The five mechanisms

| # | mechanism | substrate impact |
|---|---|---|
| 1 | **Insertion modes** (~23) | **None** — a parser state machine |
| 2 | **Adoption agency** — clones misnested formatting elements | **None** — absent slots, §4.2 |
| 3 | **Reconstruct active formatting** — more elements than tags | **None** — same |
| 4 | **Foster parenting** — moves tree content out of byte order | **ONE bounded extension**, §2.4 |
| 5 | **Foreign content** (SVG/MathML) — namespaces, real self-closing, CDATA validity | **None** — a genus or attribute question |

Plus fragment parsing (`innerHTML`), an entry point rather than a
mechanism.

Four of five are free. This is a far better position than "deferred by
name" suggested, and it is the answer to *"are we painting ourselves
into a corner"*: **no, and here is the one place that would need work.**

### 6.2 Foreign content, specifically

The lexer is **already** foreign-content-ready, and deliberately so.
Its header records recognising CDATA everywhere, against spec, because
*"lexator arborem nescit — genus nominatum clamans praestat
commentario pravo mutilanti."* That over-recognition, chosen for
loudness, is exactly what mechanism 5 later needs. The tree decides
validity; the lexer refuses to guess.

### 6.3 What v1 RESERVES (H2 + H5)

Declared in the document format, **refused if present**, so their
absence is checked rather than assumed:

| reserved | for |
|---|---|
| `ancora` attribute on `elementum` | mechanism 4 — an element whose bytes are not where its tree position implies |
| `<reinserendum>` element | mechanism 4 — tree content registered at a true offset |
| `spatium` attribute on `elementum` | mechanism 5 — the namespace (`html`, `svg`, `mathml`) |
| `clonatum` attribute on `elementum` | mechanisms 2 and 3 — marks an element whose tags live elsewhere, so a reader need not infer it from absent slots |

`clonatum` deserves a word. §4.2 shows the *model* already expresses a
clone. But a **reader** seeing four NIHIL slots cannot distinguish
"clone" from "badly truncated" — and the arbor arc's law is that a
document must not be able to lie. Reserving the marker now costs
nothing; discovering later that clones are indistinguishable from
damage costs a format version.

---

## 7. Document and canon

As CSS (`css-arbor-spec.md` §7): no new writer, no new reader.
`silva_arbor_scribere_nodum` on the root `documentum` node yields
`<arbor grammatica="html" registrum-sigillum="…" fons="…">`, seal
validated on read.

`documentum` carries `cauda:token` for the EOF token, so trailing
bytes are its `spatia_ante` — same resolution as CSS's `plagula`.

`html.canon` generated per G4's path, with the **same species-level
limitation** (G3): a vestigial production gives
`silva_gen_impletiones_computare` nothing to close over. Second
instance of that gap — which, like §2.3, promotes it from a quirk to
an S6 item.

**The canon is where §6.3's reservations are enforced.** Unknown
attributes REFUSE at load (`bffdd5d4`), so declaring `ancora`,
`spatium`, `clonatum`, and `<reinserendum>` as *known but forbidden in
v1* is exactly H5's refuse-don't-ignore, at no implementation cost.

---

## 8. Gates

As CSS §9, with HTML-specific corpus and two additions:

1. Lexer round trip — exists.
2. **Tree → bytes**, `memcmp` — the **separating oracle**, emitting
   directly from the parse beside the STML round trip.
3. STML round trip — write → read → compare, **two cycles**.
4. Canon gate on every document.
5. Corpus — the three existing fixtures plus real house HTML, plus
   adversarial: unclosed everything, `</` at EOF, misnested formatting
   (which v1 must *round-trip* even though it will not *reshape*),
   `<table>` with stray content, `<script>` containing `</script` in a
   string, mixed-case tags, duplicate attributes, `<p>` implied-close
   chains.
6. **The absent-slot gate** (new): every row of §4.2's table asserted
   explicitly, since H4 is the invariant full parity depends on. A
   `<br>` whose `tok_clausura` is accidentally filled would pass every
   byte gate and silently break mechanism 2 later.
7. **The reservation gate** (new): a document carrying `ancora`,
   `spatium`, `clonatum`, or `<reinserendum>` must be **REFUSED** by
   the v1 reader with a named cause. H5 is worthless unless tested —
   a reserved word that is silently ignored is not reserved.
8. A planted fault at each gate's birth; the fault must fail the
   *assertion*, not the build.

---

## 9. Named gaps

| # | item | exit |
|---|---|---|
| HG1 | `SilvaLexicon.genus_identificator` is C-shaped (§2.2) | Correct `css-arbor-plan.md` Task 4 **before executing it**. |
| HG2 | Five structural genera required, three preprocessor-shaped (§2.3) | S6 work item; two clients now. |
| HG3 | Foster parenting needs the reinserenda extension (§2.4) | Only when mechanism 4 is wanted. Format is ready (§6.3). |
| HG4 | Species-level canon, second instance (§7) | As CSS G3. |
| ~~HG5~~ | **CLOSED 2026-08-27** — §2.4 measured both directions | See `materia-spec.md` §4.1. Probe A: subtree emission is tree-order (`AAABBB` → `BBBAAA`), so the gap is real. Probe B: the reinserenda pour round-trips a consumed `#define` 27/27, so the machinery is live. The correction: it is **unreachable** from the subtree path (`silva_scribere.c:686` passes `fons_index = -I`, disabling the skip), so mechanism 4 costs three named edits rather than one widening. |

---

## 10. What this exercise proved

It was proposed as a way to evaluate S6 without paying for S6, and it
worked:

- **Two coupling sites found on paper** (§2.2, §2.3), one of which
  corrects a plan already written.
- **One prediction falsified** (§2.1) — the design would have carried
  machinery for a problem that does not exist.
- **One optimistic claim corrected** (§2.4) — and the corrected version
  is what justifies H2.
- **One CSS design point shown to be CSS-specific** (§5) — D7's
  whitespace seam is not a substrate concern.
- **The seam survives** (§2.5), with one interface fix.

The cost was reading. The alternative was discovering §2.2 while
debugging a lexicon that three languages already depended on.

**Recommendation for JS:** run the same exercise before implementing.
§2.1 predicts the one-way pipe will actually break there — regex-vs-
division and nested template interpolation are the two places a lexer
genuinely cannot decide alone. If the pipe survives JS too, the
substrate is proven for S6. If it breaks, better to know it on paper.
