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

> **ADDENDUM 2026-09-15 — §11 "As built on materia" governs.** This
> spec was written against silva, the same day the materia fork was
> decided; three clients (css, md, oratio) have since fixed the client
> shape and the substrate has changed under every §2 finding. §11
> retargets the body before a line of `html/` is written. Where §11
> and §§0–10 disagree, §11 wins. Plan: `project-specs/html-arbor-plan.md`.

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

---

## 11. As built on materia — addendum before construction (2026-09-15)

*Decided with Fran 2026-09-15 (decree 01M2KC03NG: oratio paused,
consumers resume HTML → JS → silva). Nothing in `html/` exists yet;
this section is the retarget the body needs, written from the three
clients that do exist (`css/`, `md/`, `oratio/`) and from materia's
source as read today, not from the spec's predictions.*

### 11.1 What changed under this spec

| body says | as built |
|---|---|
| D2 build `SilvaNodus` | `MateriaNodus` — materia is the substrate; silva is frozen |
| D4 consume silva through its amalgam | consume **materia directly**: `html/compile_probationes.sh` compiles `materia/fontes/*.c` into `html/build/` exactly as `css/` and `md/` do; html CONSUMES materia, never contains it |
| D5 reuse ladder S2+S3+S4 | materia's writer, reader, comparator (`materia_arbor_scribere_nodum` / `_legere` / `_aequalis`) and the byte emitter `materia_scribere_nodum` |
| D6 defer the S6 extraction | S6 **is** materia; parcum `01M12BD0` closes when this client's gates run (its evidence is §2 plus this addendum) |
| §2.2 `genus_identificator` is C-shaped | **CLOSED** — `MateriaLexiconCoctum` has no such field (HG1) |
| §2.3 five structural genera required | **CLOSED** — materia's registry requires none; css declares ten genera, none structural (HG2) |
| §2.4 foster parenting needs the reinserenda extension | still the one named extension (HG3): materia's reinserenda are reachable from the subtree path (MG1), but tree content emitted out of place is not what they carry |
| §3 `html/grammatica/html.stml` + G4's vestigial production | **DEAD** — the registry is GENERATED from a declaration (`html/grammatica/html.registrum.stml`, `./materia/coquere.sh`), not from a grammar; the canon is HAND-WRITTEN and drift-guarded (css B7 precedent, Fran's decree 2026-09-01) |
| §7 `silva_arbor_scribere_nodum` yields `<arbor grammatica="html" …>` | same envelope, materia's writer; the seal is `materia_arbor_sigillum` over the generated registry, pinned in `html.canon` and moved by hand with a cause |

H1–H6 stand unchanged. H4 gains a materia fact the css plan measured:
the writer omits an absent slot entirely (`materia_arbor.c`, the
`VALOR_NIHIL` path), so byte-exactness through absent slots is by
construction, as §4.2 claims.

### 11.2 Architecture as built

```
bytes
  ├─ html_lexare                      lib/html_lexema.c   EXISTS   Xar<HtmlLexema>
  ├─ html_adaptare                    html/fontes/        new      Xar<MateriaToken*>, ONE FOR ONE
  ├─ HtmlLigator                      html/fontes/        new      trivia bind INSIDE TAGS ONLY
  ├─ html_arbor_parsare               html/fontes/        new      MateriaNodus* (documentum), iterative
  ├─ materia_scribere_nodum           materia/            —        bytes: the SEPARATING oracle
  ├─ materia_arbor_scribere/legere    materia/            —        STML projection, two cycles
  ├─ materia_arbor_aequalis           materia/            —        the tree oracle
  └─ html.canon                       html/grammatica/    new      hand-written, drift-guarded
```

| module | owns |
|---|---|
| `html/grammatica/html.registrum.stml` | the declaration: ten genera, their loci in BYTE ORDER |
| `html/fontes/html_registrum_coctum.{h,c}` | GENERATED (never hand-edited; `probatio_html_registrum` compares bytes) |
| `html/fontes/html_registrum.h` | includes the generated header; hand-written slot enums per genus |
| `html/fontes/html_lexicon.{h,c}` | the `MateriaLexiconCoctum` for the 22 lexer genera (the enum's 23 members less the sentinel) |
| `html/fontes/html_adaptare.{h,c}` | `HtmlLexema` → `MateriaToken*` one for one; `HtmlLigator` (cumulare/solvere) |
| `html/fontes/html_arbor.{h,c}` | the simple builder; `VOID_ELEMENTA` and `CLAUSURAE_IMPLICITAE` as tables (M7) |
| `html/fontes/html_computus.{h,c}` + `html/instrumenta/computus.c` + `html/computus.sh` | the bench twin (css precedent) |
| `html/grammatica/html.canon` | hand-written; NOT in `canones.registrum` (the `<arbor>` root collision) |
| `html/compile_probationes.sh` | runner in the css shape; also compiles `md/fontes/*.c` for the consumer gate (§11.8) |

**No `html_selector`, no synthetic tokens, no lexer changes.** The
lexer already owns raw text and RCDATA (§2.1); the builder never
tells it anything.

### 11.3 The declaration (replaces §4.1's `<genera-extra>`)

Ten genera, appended in this order — the order is the enumeration and
the seal. Locus order within a genus is the emission order:
`materia_scribere_nodum` walks loci in registry order, so `attributa`
MUST precede `tok_finis` and `liberi` MUST precede `tok_clausura`.

```xml
<registrum grammatica="html" praefixum="HTML" typus="HtmlGenus"
  sedes="html/fontes">
  <genus titulus="documentum" nota="Structura documenti">
    <locus titulus="liberi" species="lista-nodus"/>
    <locus titulus="cauda"  species="token"/>
  </genus>
  <genus titulus="doctype">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="elementum" nota="Sex loci lexematum OMNES optionales (H4)">
    <locus titulus="tok_apertura"       species="token"/>
    <locus titulus="attributa"          species="lista-nodus"/>
    <locus titulus="tok_finis"          species="token"/>
    <locus titulus="liberi"             species="lista-nodus"/>
    <locus titulus="tok_clausura"       species="token"/>
    <locus titulus="tok_clausura_finis" species="token"/>
  </genus>
  <genus titulus="attributum">
    <locus titulus="tok_nomen"   species="token"/>
    <locus titulus="tok_aequale" species="token"/>
    <locus titulus="tok_valor"   species="token"/>
  </genus>
  <genus titulus="textus" nota="Contentum: nodi, non trivia (H6)">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="referentia">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="textus-crudus">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="commentarium">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="cdata">
    <locus titulus="tok" species="token"/>
  </genus>
  <genus titulus="elementum-malum" nota="Robustitas: tag clausurae sine pari">
    <locus titulus="tokens" species="lista-token"/>
  </genus>
</registrum>
```

**One deviation from §4.1, named.** `documentum` was
`prologus:lista-nodus radix:nodus cauda:token`. That shape assumes
exactly one root element, which HTML source does not guarantee: a
fragment has several top-level elements or none, and the parser that
guarantees one (`<html>`) does it by SYNTHESIS, which H4 forbids. A
total builder with no synthetic tokens must accept any sequence of
top-level nodes, so `documentum` is `liberi:lista-nodus cauda:token`
and the doctype is simply the first child when present. Nothing is
lost: "the document element" is a derived view (H3), not a slot.

The five structural genera of §4.1 do not exist. The `elementum-malum`
genus covers exactly one case in v1: an end tag whose name is open
nowhere on the stack (including `</br>`, which HTML5 rewrites to
`<br>` — v1 keeps it malum, total and honest, named deviation).
Everything else the lexer emits maps to a content genus by lexer
genus: `TEXTUS` → `textus`; `REFERENTIA` → `referentia`;
`TEXTUS_CRUDUS` and `_IMPERFECTUS` → `textus-crudus`; `COMMENTARIUM`,
`_IMPERFECTUM` and `_PRAVUM` → `commentarium`; `DOCTYPE` and
`_IMPERFECTUM` → `doctype`; `CDATA` and `_IMPERFECTA` → `cdata`. The
imperfect and pravum genera keep their own LEXEME genus in the
projection (`lex-commentarium-pravum`), so the tree says what the
lexer saw; the node genus says what it is.

### 11.4 The lexicon

22 rows in `HtmlLexemaGenus` order (asserted by title, css precedent).
The two decisions that are not mechanical:

| lexer genus | species | munus | why |
|---|---|---|---|
| `TEXTUS`, `REFERENTIA`, `TEXTUS_CRUDUS*`, `COMMENTARIUM*`, `DOCTYPE*`, `CDATA*`, `TAG_APERTURA`, `TAG_CLAUSURA`, `ATTRIBUTUM_NOMEN`, `ATTRIBUTUM_VALOR*` | VERBATIM | SUBSTANTIVUM | content. **Comments are SUBSTANTIVUM, not COMMENTUM**: H6 makes them nodes, and a COMMENTUM munus would let the ligator swallow them as trivia |
| `TAG_FINIS` `>`, `TAG_FINIS_SOLUS` `/>`, `AEQUALE` `=` | FIXUM | SUBSTANTIVUM | orthography lives in the genus, as css punctuation |
| `SPATIA` | VERBATIM | SPATIUM | absorbs newlines inside a tag; **no `munus LINEA`**, the same capability refusal css asserts positively |
| `DELIM` (a stray `/` inside a tag) | VERBATIM | SPATIUM | HTML5 names it "unexpected solidus in tag" and IGNORES it — trivia by the language's own definition; binding it as trivia keeps `elementum` free of a junk slot |
| `FINIS` | FIXUM `""` | FINIS | |

`genus_spatii = -I` (VERBATIM cannot feed template compression; css
precedent, guarded in materia).

### 11.5 Trivia: only inside tags, and that is the whole ligator

§5 stands and gets sharper. The lexer emits `SPATIA`/`DELIM` only
between `<name` and `>`; between tags every byte is `TEXTUS`. So the
`HtmlLigator` binds pending trivia to the next significant token
INSIDE a tag (attribute name, `=`, value, `>`) with css's divisio rule
(the first whitespace token containing a newline and everything after
it go to the NEXT token as `ante`), and is never consulted between
tags. `documentum.cauda` receives the pending trivia of a tag cut off
by EOF (`<div cl` + EOF) — the only way trivia reach the end.

Consequence for the substrate: a `textus` node whose token value is
whitespace-only (`\n    ` between `<ul>` and `<li>`) is a VERBATIM
lexeme with NO trivia, the sole child of its `<lex-textus>` — exactly
the case the T9 fix covers and the T11 constraint does not reach. md
already projects this shape. `\r\n` inside text takes materia's `cr`
attribute (B6); the corpus carries a CRLF fixture to prove it.

### 11.6 Reservations re-scoped (H5 under materia)

Two facts measured in `materia_arbor.c` today:

1. The reader REFUSES an unknown element: a child element whose title
   is not a locus of the genus → `"locus generi ignotus"` (line 3170);
   an element whose title is not a genus → `"genus registro ignotum"`
   (line 3090).
2. The reader IGNORES an unknown ATTRIBUTE on a node element:
   `_nodum_legere` reads `id` and nothing else; positions, `cr`, `n`,
   `f` are read on lexeme elements; there is no attribute census.

So §6.3 as written cannot be built: a reserved attribute (`ancora`,
`spatium`, `clonatum` on `elementum`) would be silently accepted by
the reader, which is precisely what H5 forbids, and the only thing
that refuses it is the canon at judgment time.

And the premise of H2 has weakened. In materia the format IS the
declaration plus the seal. Appending a locus later is a declaration
append and a seal move — the house law for every client, exercised
twice on css (B8) — and every projection is regenerated from source
(a projection is never a file of record; the seal refuses a stale
one). There is no format migration to fear.

**Decision (Fran to veto): v1 declares NOTHING reserved.** The four
extension points are recorded as `nota` text on the declaration, not
as loci. Refusal of a foreign extension is automatic and stays gated:

| a document carrying | is refused by |
|---|---|
| `<reinserendum>`, `<ancora>`, `<spatium>`, `<clonatum>` as a child of `<elementum>` | the reader: `locus generi ignotus` |
| `<reinserendum>` at any level as an element | the reader: `genus registro ignotum` |
| `ancora=`, `spatium=`, `clonatum=` as attributes on `<elementum>` | the canon: unknown attribute at judgment (every corpus document is judged) |

Gate 7 (§8) survives in that form: one probatio per row above, with
the named cause asserted, born red by a planted fault. The clone
question (`clonatum`: four NIHIL slots indistinguishable from damage)
is moot in v1, which never writes a clone; it is appended as an
`index` locus with the seal moved on the day mechanisms 2–3 arrive.
H2's INTENT is preserved where it matters: §4.2's genera are shaped so
parity needs no new genus.

### 11.7 Builder rules as tables (M7)

- **Void elements** (WHATWG list, 13): `area base br col embed hr img
  input link meta source track wbr`. A void element never opens; its
  `tok_clausura*` slots are NIHIL by construction.
- **Implied closes** (v1 "basic", one table `CLAUSURAE_IMPLICITAE`:
  open element → the start tags that close it): `p` ← every block
  start (`address article aside blockquote details div dl fieldset
  figcaption figure footer form h1…h6 header hgroup hr main menu nav
  ol p pre section table ul`); `li` ← `li`; `dt`/`dd` ← `dt dd`;
  `option` ← `option optgroup`; `optgroup` ← `optgroup`; `tr` ← `tr
  tbody thead tfoot`; `td`/`th` ← `td th tr tbody thead tfoot`;
  `thead`/`tbody`/`tfoot` ← `tbody thead tfoot`.
  **Refilled from the html5lib oracle (O2b-1, 2026-09-15):** `p`'s
  list completed to WHATWG "in body" (`+ center dialog dir search
  summary listing plaintext xmp li dd dt`); table sections, rows and
  cells also closed by `caption col colgroup`; `caption` ← `caption
  col colgroup tbody td tfoot th thead tr`; ruby `rb`/`rt`/`rp` ← `rb
  rt rtc rp`, `rtc` ← `rb rtc`; `h1…h6` ← `h1…h6`; `a nobr button
  select` ← themselves (row with `claudentia` NIHIL); `head` and
  `colgroup` ← EVERY start tag except their permitted set (`nisi`
  rows: `base basefont bgsound link meta title noscript noframes style
  script template head`; `col template`). Oracle 887 → 930.
  **Scope (O2b-5, 2026-09-15 — mechanism 1 of §6.1, the H1
  top-of-stack law relaxed):** a p-closer closes a `p` anywhere in
  BUTTON SCOPE (hidden by `applet caption html table td th marquee
  object template`, the MathML/SVG integration points, and `button`);
  `li` closes the nearest open `li`, `dd`/`dt` the nearest `dd`/`dt`,
  unless a "special" element other than `address div p` lies between;
  `button` closes a `button` in scope. Everything above the closed
  element closes with absent `tok_clausura*`. Implemented as
  VISIBILITY INDICES per stack frame (`ScopiGradus`: nearest visible
  `p`/`li`/`dd|dt`/`button` computed at push from the frame below),
  O(1) per token — a plain stack walk per start tag was quadratic in
  depth and killed the 100,000-deep totality pins on its first run
  (489 s). The other rows stay top-of-stack (h1–h6, table parts,
  options, self-closers: their HTML5 rules are top-of-stack or belong
  to the table machinery). Oracle 1,047 → 1,054.
- **Foreign content (O2b-6, 2026-09-15):** `html_alienum` derives each
  element's own namespace from its parent (`svg`/`math` open; in a
  MathML text integration point `mglyph`/`malignmark` stay MathML)
  and its children's from itself (HTML integration points
  `foreignObject desc title` / `mi mo mn ms mtext` / `annotation-xml`
  with an HTML encoding). The builder stores both per stack frame
  (`ScopiGradus`); a self-closing start tag whose element is foreign
  closes it (WHATWG "acknowledge the self-closing flag" — in HTML `/>`
  never closes, §11.3), and a start tag from the breakout list (`b big
  blockquote body br center code dd div dl dt em embed h1–h6 head hr i
  img li listing menu meta nobr ol p pre ruby s small span strong
  strike sub sup table tt u ul var`) pops foreign elements up to the
  nearest one whose children are HTML. Attributes at push are unseen,
  so `font` never breaks out and `annotation-xml` children are MathML
  in the builder. Oracle 1,054 → 1,086.
- **End-tag rule**: an end tag pops the stack to the nearest open
  element of the same name (ASCII case-insensitive, H1), implicitly
  closing everything above it (`<div><p>x</div>` closes `p` with
  absent `tok_clausura*`); no open element of that name → the end tag
  and its `>` become one `elementum-malum`.
- **Raw text**: nothing — the lexer emits `TEXTUS_CRUDUS` after
  `script style title textarea`; the builder appends `textus-crudus`.
  **O4 (2026-09-15):** the lexer's set is WHATWG's whole RAWTEXT list
  (`+ xmp iframe noembed noframes`) and `plaintext` swallows the rest
  of the file as ONE raw token that is perfect, not truncated (no
  closing tag exists to be missing). Oracle 1,086 → 1,112.
- **EOF**: every open element closes with absent `tok_clausura*`; a
  tag cut off mid-attributes keeps its `elementum` with `tok_finis`
  absent (the model expresses truncation; §4.2 last column).

The builder keeps an explicit open-element stack (an `Xar`), never
recursion: the parse of a 100 000-deep `<div>` chain must not touch
the C stack. The projection and the emitter are materia's, and
recursive; the totalitas gate measures where the first one dies and
pins it against `01M1FAD8`, which is the depth question this client
was chosen to force.

### 11.8 Gates as built (§8 mapped to probationes)

| # | probatio | asserts | planted fault at birth |
|---|---|---|---|
| — | `probatio_html_registrum` | `materia_registrum_recens` (generated tables byte-equal), lexicon order by title, loci contiguity, `munus LINEA` absent and the line-sensitive capability REFUSED, one minimal tree through writer→reader→writer | two lexicon rows swapped |
| 1 | `probatio_html_lexema` (exists, root suite) | lexer round trip | — |
| 2+7 | `probatio_html_adaptare` | one-for-one adaptation, BYTE COVERAGE (trivia + values == source) over the fixtures, segmented-Xar contract (allocate between accumulations) | a trivium dropped in `solvere` |
| 2 | `probatio_html_arbor` | parse → `materia_scribere_nodum` → `memcmp` on every inline case; §4.2's absent-slot table row by row; implied-close, void, raw-text, EOF cases | `tok_clausura_finis` never assigned |
| 5+6 | `probatio_html_corpus` | every corpus file byte-exact; self-measure (files read, bytes) | `cauda` skipped for one file |
| 3 | `probatio_html_stml` | write → read → write, TWO cycles byte-equal; `materia_arbor_aequalis` STRUCTURALIS (FIDELITAS unrunnable: no LINEA munus, as css); compression census 0; over the whole corpus | the re-read tree mutated before comparison |
| 4 | `probatio_html_canon` | drift guard both ways (every genus/locus/lexeme/envelope has one rule; every rule names one of them), seal pin vs live `materia_arbor_sigillum`, judgment of every corpus document + inline documents | one genus rule deleted → guard AND judgment red |
| 7 | `probatio_html_reservatio` | the three rows of §11.6, each with its named cause | the refusal message misnamed |
| — | `probatio_html_totalitas` | random bytes, mutated and truncated corpus, nesting, under `CREDO_NON_RUIT`; the parse alone at 100k depth survives; the first recursive walker's limit measured and pinned | the stack replaced by recursion for one construct |
| — | `probatio_html_computus` | golden `html/probationes/fixa/computus/basis.tsv` (deterministic columns only) | a golden count edited |
| — | `probatio_html_md` | **consumer of a consumer**: md's own fixtures rendered by `md_html_reddere` parse, emit byte-identical, project, pass `html.canon`, and contain ZERO `elementum-malum` | a malformed rendering asserted well-formed |

Every probatio self-measures its coverage ("N/N clean" of nothing is
the lying-green class). The C89 shim (`./materia/shim_probare.sh`)
runs at the end of the client as the phase-boundary audit and its
count goes in the phase-log.

### 11.9 Corpus

Snapshots in `probationes/fixa/html/` (the three that exist plus
`adversarius_2.html`: CRLF throughout, unclosed everything, `</` at
EOF, misnested formatting, `<table>` with stray content, `<script>`
with `</script` inside a string, mixed-case tags, duplicate
attributes, `<p>` chains, valueless and unquoted attributes, a bare
`&`, `]]>` in text, nesting to 200, and the literal strings
`</lex-textus>` and `</lex-textus-crudus>` inside text — the raw-form
refusal case md found); NUL rides inline in the probatio. Live house
pages by path, as css references `lib/speculum_assets/speculum.css`:
`briar/facies/facies.html`, `briar/probationes/fixa/facies/{salve,
circulus,fragmenta}.html`, `apps/villa/assets/index.html`,
`apps/mensor/assets/index.html`, `tools/silex_assets/index.html`,
`knotapel/atlas/web/index.html`, `lib/speculum_assets/probatio_visus.html`,
`reference/aquinas-dithering-tool.html` — fourteen files, ~110 KB,
every one with `<script>` and most with `<style>` (raw text is not a
corner case here, it is every page).

### 11.10 What this client tests in the substrate

Named before building so the answer is an answer either way:

- absent slots as MEANING at scale — every void and every implied
  close is an absent typed slot the emitter must skip and the reader
  must accept;
- a content-dominant tree with almost no trivia — the ligator is
  consulted only inside tags, so `spatia_ante/post` are empty on
  nearly every token; whitespace-only VERBATIM text values at every
  level of the tree (T9's fix, exercised thousands of times);
- raw-text regions owned by the lexer, with `</`-shaped bytes inside
  token values, through the STML raw form and its refusal;
- an ITERATIVE builder under materia's RECURSIVE walkers — the depth
  crash `01M1FAD8` measured on the substrate's side alone;
- a consumer of a consumer: the md client's output parsed by the html
  client, both on materia.

§10's recommendation for JS stands, and JS is next: regex-vs-division
and template interpolation are where the one-way pipe will finally be
tested.

### 11.11 As built (2026-09-15, H1–H11; plan `html-arbor-plan.md`)

Built in eleven commits the same day, one per task, every gate born
red. §11 held as written with these as-built facts:

- Lexicon 22 rows (not 23), loci 18 (not 17) — counted at H1.
- The reader IGNORES unknown attributes and REFUSES unknown elements,
  as §11.6 predicted; both pinned in `probatio_html_reservatio`.
- `/>` never closes an HTML element (HTML5); `</br>` is malum.
- Attribute tokens after an end tag (`</h3 x=y>`) close the pending
  clausura and form one malum — found by the totality gate at birth,
  the only builder bug in the arc.
- **Substrate limit found** (01M2KJ3PK2): a raw-text or comment token
  containing its own `</lex-…>` closing tag is refused by materia's
  writer; pinned in `probatio_html_stml`, Fran's decision.
- **Depth measured** (01M1FAD8): parse alone survives 100,000 levels
  (iterative); `materia_scribere_nodum` dies at 50,000; the STML
  projection is quadratic in depth. Pinned in `probatio_html_totalitas`.
- STML projection = 15× the source over the corpus; the round trip is
  ~19× the parse (css: 30×). Golden in `fixa/computus/basis.tsv`.
- md's rendered HTML parses through the whole chain; sabaw's notation
  yields one malum, pinned (md polish 01M1NBEVM0).
- Substrate changes forced: NONE (css 4, oratio 5). The generality
  test of §10's phase 4 passed in the sense the spec meant: nothing
  HTML needed was HTML-shaped.

Named after: the html5lib oracle with a rising pin, wild fixtures under
a sources file, the lexer's raw-text set (xmp, iframe, noembed,
noframes, plaintext). Then JS, per §10's recommendation.
