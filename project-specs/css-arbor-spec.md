# css_arbor — the CSS tree, and silva as a language substrate

*2026-08-27. Designed with Fran. Successor slice to
`project-specs/css-html-js-spec.md`, which shipped `css_lexema` and
named this rung by title: "Later (css_arbor, when a consumer wants
it): component values, rules, declarations, with the spec's recovery
algorithm." Closes the tree half of desideratum `01KY0P4ZC45Y8VYJZJBXH27H43`.*

**Framing decided up front:** the goal is not a parser good enough for
a particular application. CSS is a stable, long-horizon target; the
goal is a *good* CSS parser — byte-exact round trip, selectors
understood. Linting and the theming audit are the first modest
consumer, in the git-library tradition: the first consumer is modest,
the design outlives it.

---

## 0. Decisions taken (do not re-litigate)

| # | decision | rationale |
|---|---|---|
| D1 | Hand-written recursive descent, **not** silva's GLR | CSS Syntax L3 is *defined* algorithmically — consume-a-rule / consume-a-block / consume-a-component-value, recursive descent over balanced delimiters that never fails. Totality is a property of that algorithm. GLR exists for C's genuine ambiguity (typedef-name, macro expansion); CSS has none. |
| D2 | Build **`SilvaNodus`** against a CSS registry; reuse silva's projection | The node model is engine-agnostic (`genus` is an `s32` registry index). Reusing it inherits the STML writer, reader, comparator, emitter, and query engine. A new surface re-implementing an old concept re-earns the old concept's bugs. |
| D3 | Selectors live in their **own module**, `css_selector` | `css_arbor` stops at Syntax L3, leaving a qualified rule's prelude an unanalysed component-value list. Serves the "one selector engine, two domains" convergence, and isolates the whitespace-as-combinator problem in one place. |
| D4 | New sibling subsystem **`css/`**, consuming silva through its amalgam | `lib/` and `include/` never include silva — the root tier is silva-free foundation. Precedent for the sibling shape: `officina/`, `tessera/`. |
| D5 | Reuse ladder **S2+S3+S4**: projection, emitter, queries | One lexicon seam unlocks all three; the emitter and query engine are already neutral. Skip S5 (parse engine) per D1. |
| D6 | **Defer S6** (extracting the neutral substrate) until after HTML | Silva has one client today, so any seam drawn now is a guess. Two clients reveal a seam; three prove it. Recorded as named debt, §11. |
| D7 | Whitespace is **hybrid**: trivia structurally, content inside values | §6. |

---

## 1. What we are taking from silva, measured

Silva turns out to be two libraries wearing one name. Nobody planned
this; it fell out of the registry-driven design. Counting C89-lexeme
references per module:

**Language-neutral substrate that already exists:**

| module | lines | C89 refs |
|---|---|---|
| `silva_generare` (grammar compiler) | 3915 | 2 |
| `silva_quaestio` (selector engine over trees) | 1833 | **0** |
| `silva_glr` (LR/GLR engine) | 1380 | **0** |
| `silva_coquere` (table generator) | 1315 | 2 |
| `silva_arbor_aequalitas` (comparator) | 1000 | **0** |
| `silva_nodus` (tree model) | 940 | 6 |
| `silva_quaestiones` (named query library) | 812 | **0** |
| `silva_scribere` (**tree → bytes**) | 804 | **1** |
| `silva_commissio` | 641 | **0** |
| `silva_latina_datum` | 721 | **0** |
| `silva_contextus` | 163 | **0** |
| **total** | **~13,500** | **~11** |

**Irreducibly C89, for contrast:** `silva_tabulae_c89` (17032/169),
`silva_c89_semantica` (16538/209), `silva_formator` (3962/229 — style
rules are inherently per-language), `silva_expandere` (3111/32),
`silva_lexema` (1106/87), `silva_conditio` (759/44).

**The seam:** `silva_arbor` (7310 lines, **23** coupling sites outside
its spelling table), `silva_parsare` (41), `silva_token` (3).

*Measurement note: `fontes/` counted with pattern `SILVA_LEX_[A-Z]`;
`instrumenta/` with `SILVA_LEX_[A-Z]|C89|c89`. Figures are coupling
sites, not "neutral lines".*

> **CORRECTION, 2026-08-27 — these figures are a FLOOR, not a census.**
> The G4 probe found a coupling site this measurement structurally
> could not see: `silva_generare.c:1315` requires **five structural
> node genera by name** — `ambiguus`, `error`, `conditionalis`,
> `ramus-sumptus`, `ramus-omissus` — three of which model the C
> preprocessor. A CSS grammar must declare them to be accepted at all,
> though CSS has neither a preprocessor nor ambiguity. `silva_generare`
> is scored **2** above and called neutral.
>
> The census measured **lexical** coupling (token genera) and never
> **structural** coupling (required node genera). Neutrality in one is
> not neutrality in the other. Any future census — and any S6
> estimate drawn from one — must grep for both. Recorded as the first
> S6 evidence under parcum `01M12BD0`.

Two findings worth stating plainly, because they carry the design:

- **`silva_scribere` is grammar-ignorant by declaration.** Its header
  says so: *"Ambulatio grammaticae-ignara est."* One C89 reference, an
  EOF check. `emittere(tree) == source` — the byte-exact round trip —
  comes along free.
- **`silva_quaestio` is already a CSS-like selector engine.** Its own
  header: *"machina selectorum CSS-similis super arbores silvae"*,
  `genus-tag (registro-ductus)`. Zero C89 references. Supports
  descendant, `>`, `+`, `~`, `,`, `[attr]`, `[attr^=]`, `:pseudo(…)`.
  The convergence the desideratum asks for is already built and
  already neutral.

---

## 2. Architecture

```
bytes
  ├─ css_lexare                  lib/css_lexema.c   EXISTS      Xar<CssLexema>
  ├─ css_adaptare                css/fontes/        new, tiny   Xar<SilvaToken*>
  ├─ css_arbor_parsare           css/fontes/        new, RD     SilvaNodus*
  │    └─ css_selector_parsare   css/fontes/        new         SilvaNodus* subtree
  ├─ silva_arbor_scribere_nodum  silva/             seam only   STML
  ├─ silva_scribere_nodum        silva/             UNCHANGED   bytes
  └─ css.canon                   css/grammatica/    generated   schema
```

| module | owns |
|---|---|
| `css_lexicon` | the `SilvaLexicon` instance: genus names, fixed spellings, trivia classification, EOF, identifier genus |
| `css_adaptare` | `CssLexema` → `SilvaToken*`; whitespace/comment tokens become `spatia_ante`/`spatia_post` per §6 |
| `css_arbor` | CSS Syntax L3 as recursive descent, building `SilvaNodus` |
| `css_selector` | prelude token list → selector subtree |
| `css/grammatica/css.stml` | node vocabulary, `<genera-extra>` only, **zero productions** |

`css_lexema` stays in the root tier, untouched. The shipping rule from
the parent spec holds: *a lexema module never grows tree knowledge.*

**Why the adapter is structural, not incidental.** `SilvaValor`'s union
holds `SilvaToken*` for leaves (`silva_nodus.h`). If CSS leaves live in
`SilvaNodus` trees at all, they must be `SilvaToken`s. CSS tokens carry
`origo` = FONS always, empty `scissurae`, default `standard` — dead
fields, four words, not a design compromise.

---

## 3. The lexicon seam — the only change to silva

Every coupling site in `silva_arbor.c` is *"ask the lexicon"*:

- `ORTHOGRAPHIAE[SILVA_LEX_NUMERUS_GENERUM]` — genus → fixed spelling (`:35`)
- `silva_lexema_genus_nomen(genus)` — genus → tag name (`:384`)
- eight bounds checks against `SILVA_LEX_NUMERUS_GENERUM`
- `SILVA_LEX_EOF` special case — EOF never carries a value (`:352`)
- trivia compaction — `SPATIA`/`TABULAE`/`NOVA_LINEA`/`CONTINUATIO`
  (`:1023-1034`, `:1131`, `:1198`, `:2459-2478`, `:3527`)
- `SILVA_LEX_IDENTIFICATOR` — reader-side token synthesis (`:2851`)

So the seam is one struct:

```c
nomen structura {
    i32                  numerus_generum;
    constans character* (*nomen)(s32 genus);
    constans character* (*orthographia)(s32 genus);  /* NIHIL = varia */
    s32                  genus_eof;
    s32                  genus_identificator;
    constans SilvaTriviumForma* trivia;              /* genus + character */
    i32                  numerus_triviorum;
} SilvaLexicon;
```

**The wide part, stated plainly.** `SilvaToken.genus` (`silva_token.h:241`)
and `SilvaTabSymbolum.genus_lexematis` (`silva_tabulae.h`) are both typed
`SilvaLexemaGenus`. For CSS genera to flow, that type becomes an opaque
`s32` read through the lexicon. Shallow but wide: many call sites, no
logic. `-Wconversion -Werror` finds every one, and silva's gates
(281/281, 154/154, 78/78) answer immediately. This change either
compiles clean and passes or fails loudly — there is no silent middle.

**Measured CSS consequence: the compression path does not fire.**
Silva's C lexer splits whitespace by character class, which is what
makes `<#@post-spatia n="3">` possible. CSS consumes a whole
whitespace run into one `CSS_LEX_SPATIA`, mixed characters included
(`lib/css_lexema.c:513-519`). So CSS whitespace is a *variable-spelling*
token carrying its bytes verbatim, and CSS declares zero compressible
trivia. `SilvaArborCensusCompressionis` reads all zeros. This is the
case the arbor worklog already anticipated: *"familia templorum mortua
exitum VALIDUM incompressum relinquit … praesentia asserenda est,
numquam numeri pincti."* Documents get larger; they stay correct.
**Assert presence, never pinned counts.**

---

## 4. The CSS node vocabulary

`css/grammatica/css.stml`, `<genera-extra>` only. Slot species are
silva's six: `nodus`, `token`, `lista-nodus`, `lista-token`,
`lista-mixta`, `index` (`silva_generare.c:820`).

```xml
<genera-extra>
  <!-- Structura -->
  <genus titulus="plagula"      slots="regulae:lista-nodus cauda:token"/>
  <genus titulus="regula-qualificata"
                                slots="praeludium:lista-nodus corpus:nodus"/>
  <genus titulus="regula-apud"  slots="tok_nomen:token praeludium:lista-nodus
                                       corpus:nodus tok_terminator:token"/>
  <genus titulus="declaratio"   slots="tok_nomen:token tok_colon:token
                                       valor:lista-nodus praevalentia:nodus
                                       tok_terminator:token"/>
  <genus titulus="praevalentia" slots="tok_clamor:token tok_verbum:token"/>

  <!-- Valores componentes (CSS Syntax L3) -->
  <genus titulus="lexema-servatum" slots="tok:token"/>
  <genus titulus="functio"      slots="tok_nomen:token argumenta:lista-nodus
                                       tok_clausum:token"/>
  <genus titulus="saeptum"      slots="tok_apertum:token contentum:lista-nodus
                                       tok_clausum:token"/>

  <!-- Robustitas: recuperatio spec-definita -->
  <genus titulus="regula-mala"     slots="tokens:lista-token"/>
  <genus titulus="declaratio-mala" slots="tokens:lista-token"/>
</genera-extra>
```

`plagula` and `declaratio` deliberately echo silva's names. Different
registry, same concept — the parallel is real, not a collision.

**At-rule block contents.** `@media`'s block holds rules;
`@font-face`'s holds declarations. A small table of known at-rules
decides; unknown at-rules fall back to a generic `saeptum` of
component values, which is total and honest. Growth = teaching the
table a new at-rule, never a re-parse.

---

## 5. Selectors

`css/grammatica/css.stml` carries a second cluster of genera for the
selector subtree:

```xml
<genera-extra>
  <genus titulus="selector-lista"       slots="selectores:lista-nodus"/>
  <genus titulus="selector-complexus"   slots="sinister:nodus combinator:token
                                               dexter:nodus"/>
  <genus titulus="selector-compositus"  slots="partes:lista-nodus"/>
  <genus titulus="selector-typi"        slots="tok:token"/>
  <genus titulus="selector-universalis" slots="tok:token"/>
  <genus titulus="selector-classis"     slots="tok_punctum:token tok_nomen:token"/>
  <genus titulus="selector-identitatis" slots="tok:token"/>
  <genus titulus="selector-attributi"   slots="tok_apertum:token tok_nomen:token
                                               tok_operator:token valor:token
                                               tok_clausum:token"/>
  <genus titulus="pseudo-classis"       slots="tok_colon:token tok_nomen:token
                                               argumenta:lista-nodus"/>
  <genus titulus="pseudo-elementum"     slots="tok_colon:token tok_nomen:token"/>
  <genus titulus="selector-malus"       slots="tokens:lista-token"/>
</genera-extra>
```

`selector-complexus.combinator` is the §6(a) slot: it holds `>`, `+`,
`~`, **or the whitespace token itself** for the descendant combinator.

`css_selector` consumes the component-value list a
`regula-qualificata` or `regula-apud` produced as its `praeludium`. It is a separate
module so that the engine can later serve silva's query layer as well
as real CSS (desideratum `01KY0P4ZC45Y8VYJZJBXH27H43`), and so that
§6's hard case lives in exactly one file.

---

## 6. Whitespace — where CSS genuinely fights silva

Silva's model: every token owns its `spatia_ante`/`spatia_post`,
trivia have exactly one owner, whitespace never means anything. That
last clause is **false for CSS in two places**.

**(a) Combinators.** `a b` is a descendant combinator; `a>b` and
`a > b` are the same rule.

*Resolution:* a **`combinator:token` slot** holding either the
punctuation token (`>`, `+`, `~`) *or the whitespace token itself*.
Whitespace promoted into a slot is a **value**, not trivia — so the
one-owner invariant is preserved exactly, the round trip stays
byte-exact, and `a b` differs from `ab` *structurally* rather than
only in ornament, which is correct.

**(b) Component values.** CSS Syntax L3 makes `<whitespace-token>` a
**preserved token**: inside `margin: 1px 2px` the space is content in
the spec's own object model.

*Resolution (D7, hybrid):*

> **Inside a prelude or a declaration value, whitespace is content —
> a `lexema-servatum` node. Everywhere else it is silva-style trivia.**

Trees then read the way a human reads CSS, and `selecta` queries are
not drowned in whitespace nodes. The cost is a rule with a seam that
can be got wrong, so it gets its own gate (§9.7).

---

## 7. The STML document

**No new writer and no new reader.** `silva_arbor_scribere_nodum`
already emits a complete self-describing envelope —
`<arbor grammatica="css" registrum-sigillum="…" fons="…">`
(`silva_arbor.c:2118-2135`) — and `silva_arbor_legere` validates
grammar name and seal before constructing anything
(`silva_arbor.c:3862-3883`). A seal mismatch REFUSES: a tree judged by
a false vocabulary is a lie.

CSS calls the subtree writer on its root `plagula` node and inherits
the envelope, the seal, the single-anchor discipline, and the
fragment/transclusion machinery unchanged.

**EOF trivia** is the one gap, and it closes without touching silva:
`plagula` carries a **`cauda:token`** slot holding the EOF token, so a
file's trailing whitespace is that token's `spatia_ante`. One owner
preserved, no new document section, no writer change.

**The positions law is inherited, not re-derived.** One anchor in the
document; every other position rebuilt by cursor. A document must not
be able to carry a stale offset. Corollary, learned the expensive way
in the arbor arc: **an anchor is where EMISSION BEGINS, not where the
token is** — emission starts with leading trivia. That law failed four
times in four faces; CSS inherits the fix by reusing the same code
path rather than writing a fresh anchor helper.

---

## 8. The canon

`css.canon`, generated by `silva_gen_canonem_scribere` from the same
`css.stml` — one source, two outputs, so the schema cannot rot away
from the model.

**Named limitation, v1.** That generator derives *which genera may
fill which slot* via `silva_gen_impletiones_computare`, which closes
over **productions**. A productions-free grammar has nothing to close
over, so `css.canon` constrains slots at the **species** level (*this
slot holds a node-list*) rather than the **genus** level (*this slot
holds `saeptum` or `regula-*`*). Weaker than c89's canon. Accepted for
v1; tightening it means extending `<genus>` slot syntax to carry genus
constraints (`slots="corpus:nodus(saeptum|regulae)"`), a generator
change not worth making on speculation. Recorded in §11.

---

## 9. Gates

Built on what the arbor arc already learned, not re-derived.

1. **Lexer round trip** — `css_lexemata_emittere`, exists, unchanged.
2. **Tree → bytes** — `silva_scribere_nodum`, `memcmp` against disk
   over the whole corpus.
3. **STML round trip** — write → read → comparator equal, **and**
   read → emit → `memcmp`.
4. **The separating oracle.** Gate 2 emits **directly from the parse**,
   *beside* gate 3's full round trip. Without it, a parser defect and
   a projection defect are one undifferentiated number. Every later
   finding in the arbor arc depended on this instrument; it is built
   in from the start here, not after the first confusing failure.
5. **Canon gate** on every document produced.
6. **Corpus** — `probationes/fixa/css/{componentia_2026-08-18,thema_2026-08-18,adversarius}.css`
   plus real house CSS (`lib/speculum_assets/speculum.css`,
   `knotapel/atlas/web/style.css`), plus adversarial: `}` at top level,
   unterminated everything, nested blocks, `!important` spacing
   variants, mixed line endings, non-ASCII, empty file. NUL rides
   inline in the probatio, not as a fixture.
7. **One-owner assertion** — every source byte covered exactly once
   across tree tokens and their trivia. This is the specific guard for
   §6's hybrid seam.
8. **Two write-read cycles, not one.** A compounding defect is
   invisible in a single cycle.
9. **A planted fault at each gate's birth.** Silent gate ≡ dead gate.
   The fault must fail the *assertion*, not the build: a planted fault
   that breaks compilation proves nothing about the gate.

Standing cautions that apply to every number above: judge by the
**cause tally**, not the total — a headline can hold steady while the
reasons underneath it change completely. **Interrogate the first
failure**; it may be a different gate than the one you meant to test.
Divergence counts are a **floor**, since the comparator reports only
the first per file.

---

## 10. Out of scope

- **Per-property value grammars.** `margin: 1px 2px` parses to
  component values, never to a typed box. Huge, unstable, versioned —
  the opposite of a millennium target.
- **Cascade, specificity, computed values.** Consumer concerns.
- **Selector *matching*.** This spec builds the selector *tree*.
  `silva_quaestio` is the matching engine and already exists; wiring
  it to CSS trees is a later, separate slice.
- **HTML and JS arbors.** They inherit this seam; they are not this
  spec.
- **S5** — CSS as an LR grammar. Ruled out by D1.

---

## 11. Named debts and gaps

| # | item | exit condition |
|---|---|---|
| G1 | **Layering inversion**: `css/` includes `silva.h`, so "the CSS parser depends on the C parser". Conceptually backwards; will look wrong in the census. | S6 — extract the ~13,500-line neutral substrate into its own library, after `html_arbor` has bent the seam a second time. |
| G2 | **Amalgam exports a read-only node API.** `silva.h` carries `silva_nodus_{extensionem,liberi,geometria_fida,est_fons_purus}` but **not** `silva_nodus_{creare,ponere,appendere}` or the `silva_valor_*` constructors. Anything outside `silva/` that builds trees needs it extended. | Extend via the known ritual: `fontes_generare` → `excludenda_generare` → hand-extend `amalgama/silva.h` + `CADENDA_TYPEDEF` → hospes exercise → `amalgamare.sh`. First task in the plan. |
| G3 | **Species-level canon** (§8). | Extend `<genus slots="…">` with genus constraints, when a real gate wants it. |
| G4 | **Unverified**: whether `silva_coquere` accepts a grammar with `<genera-extra>` and no productions and no `<initium>`. | Resolve as the **first** task, before anything depends on the answer. Fallbacks: a registry-only generator mode; a vestigial single production; or a hand-written `css_registrum.c` with two static tables. |

---

## 12. Why this is worth doing

The parent spec framed three web-format libraries. This slice does
something larger by accident: it establishes that rhubarb has a
**language substrate** — roughly 13,500 lines of neutral AST
machinery, with about eleven lines of accidental C coupling — and that
a new language costs a lexer, a parser, a registry, and a lexicon,
while inheriting the tree model, the byte-exact emitter, the STML
projection, the canon, the comparator, and a selector engine.

CSS is the cheapest possible proof of that claim, because its grammar
is small and its recovery is specified. HTML and JS are the payoff.
