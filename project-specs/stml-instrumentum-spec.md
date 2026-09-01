# STML — instrumentum crustae (`stml`) — spec V1

**Status:** `formare` ET `expandere` AEDIFICATA 2026-09-01
(`tools/stml.c` + `tools/stml_struere.sh`, institutum `~/.bin/stml`;
§7.5.4 quoque executum eodem die). `vertere` nondum; `indagare`
in scopo (Fran 2026-09-01), nondum aedificatum — §9.4 framing
half-pager first, or v1 = non-interactive trace dump.
Sessio designationis 2026-08-29 (Fran + Opus 5).
**Parcum matris:** `01M0VNSZC6` (Formator STML verus — CLI).
**Visio:** `project-specs/stml-visio.md` §9.4 (instrumentum + canales).

---

## §0 Quid hoc est, et quid NON est

A single binary `stml` on `$PATH`, installed the way `silex` is. Two verbs:

| verb | quid facit |
|---|---|
| `formare` | reads a `.stml` file, emits the **pretty** form |
| `vertere` | reads a `.stml` file, emits **HTML** |

**NOT in V1**, and deliberately so: the §9.4 pipe machinery (`quaerere`,
`mutare`, `legere`, `spargere`/`colligere`), the stream framing contract,
cross-file transclusion, HTML→STML, and `iudicare` (`silex iudicare`
already judges `.stml` against a canon — a second implementation would
drift).

### §0.1 De contractu §9.4 dilato — cur licet

`stml-visio.md` §9.4 decrees: *"the stream framing contract is the API —
Eskil-first, half a page, decided before any subcommand ships."*

That decree exists so the framing is not retrofitted around accidental
choices. **Both V1 verbs are file-in / file-out and touch framing nowhere.**
Faith is kept by (a) choosing verb names that do not squat on the parked
pipe verbs, and (b) this paragraph, so a later session reads the deferral
as a decision rather than an oversight.

Name collision resolved deliberately: §9.4 parks `formare` for a *different*
job (rendering a stream as a human table/TSV). But `silva/formator.sh`
already makes `formator`/`formare` the house word for **this** operation on
C files. This tool takes the word; §9.4's render verb is renamed when it
lands (`tabulare` suggested). Taking the consistent name now and moving the
unbuilt thing is cheaper than the reverse.

---

## §1 Invocatio et contractus exitus

```
stml formare <via.stml> [-ad <exitus>] [-probare]
stml vertere <via.stml> [-ad <exitus>]
```

- **stdout by default.** `-ad <via>` writes to that path instead.
- `-ad` naming the input path is **REFUSED** (truncate-before-read).
- `-probare` (formare only): emit nothing; compare the pretty form to the
  file on disk. Restores the lint contract for directory sweeps.

**Exitus:**

| | filter mode (default) | `-probare` |
|---|---|---|
| `0` | emissum | conformis |
| `1` | — | divergentiae |
| `2` | recusatio (usus / illegibilis / non emittendum) | idem |

`2` is REFUSAL-LOUD: always a message on stderr naming the file, the line,
and the reason. A refusal is never silent and never partial — nothing is
written to `-ad` unless the whole document emitted.

---

## §2 Aedificatio et institutio

`tools/stml_struere.sh`, mirroring `tools/silex_struere.sh`:

- Link **only the closure**, computed by `bin/aedilis --partes` from
  `lib/stml.c` + `lib/stml_macros.c` — NOT `build/*.o` wholesale. Silex
  links everything and therefore drags in the `.m` objects and needs
  `-framework Cocoa -framework Security -framework WebKit`. `stml` needs
  none of that; it should be a small, framework-free binary. This doubles
  as a live exercise of aedilis on a real target.
- Install: `mkdir -p ~/.bin && rm -f ~/.bin/stml && cp bin/stml ~/.bin/stml`.
  **The `rm -f` is load-bearing** — macOS caches code signatures per vnode,
  and `cp` over a recently-run binary yields SIGKILL (137) on next run.

Compile flags: those in `compile_tests.sh` (which include `-std=c89`,
`-Wno-long-long`, `-Wno-overlength-strings`), NOT the shorter list in the
root CLAUDE.md.

---

## §3 Catena — legere → expandere → distribuere → emittere

Stage order established by `canon_iudicare_distributum` (`lib/canon.c:3547`).
Both verbs use the same front half.

```c
/* UNUM internamentum per catenam totam. Ante stml_legere creandum. */
si (intern == NIHIL)
{
    intern = internamentum_creare(piscina);
    si (intern == NIHIL) { /* recusare */ redde NIHIL; }
}

resultus = stml_legere(textus, piscina, intern);
si (!resultus.successus) { /* linea_erroris, columna_erroris, error */ }

expansio = stml_expandere(resultus.radix, piscina, intern);
si (!expansio.successus) { /* vitium, linea, fragmentum, loculus */ }

distributio = stml_distribuere(expansio.radix_expansa, piscina, intern);
si (!distributio.successus) { /* vitium, linea, titulus */ }

radix_emittenda = distributio.radix_distributa;   /* DOCUMENTUM */
```

### §3.1 Leges de catena — quaeque mensurata

1. **Intern: unum obiectum per catenam.** Definitions store the
   parser-interned id; calls intern with the context's; they meet at a raw
   pointer compare (`lib/stml_macros.c:122`). A mismatch makes every call
   `FRAGMENTUM_IGNOTUM` — loudly, but every call fails. Measured in
   `lib/canon.worklog.md` 2026-08-26.
2. **Gate on `successus`, NEVER on the root pointer.** Vitium VII
   (`ARGUMENTUM_ARBOREUM`) is set *softly* — the walk continues, the literal
   stays, and `radix_expansa` is non-NIHIL but wrong. Only the end guard at
   `lib/stml_macros.c:1683` catches it.
3. **Feed `resultus.radix` (DOCUMENTUM), not `elementum_radix`.** Genus in =
   genus out; feeding the element root puts document-level DOCTYPE/PI/comments
   outside the subtree, where they are never emitted.
4. **`elementum_radix` is stale after expansion** — it points into the
   original tree. Re-find it (see `_expansae_elementum_radix`,
   `silva/fontes/silva_arbor.c:6684`).
5. **`distribuere` is not optional in practice.** Optional by contract, but
   skipping it is SILENT: it changes cardinality (one wrapper → N siblings)
   and attribute presence (carry-down + tombstone-delete), invisible in the
   un-distributed tree. It is identity on documents with no anonymous
   fragments and costs one clone. Skipping means emitting a different
   document than canon judged.
6. **All three stages share the piscina**; nothing is freed individually.

### §3.2 Praeventio — porta ante parsationem

**`stml_legere` hangs forever on some inputs (§7.1).** Until the parser
guard lands, both verbs run `stml_lexemata_colligere` FIRST and refuse if
either holds:

1. any lexeme of **zero width** (`initium == finis`) — the lexer's own
   progress guard having fired, which is exactly the condition the parser
   turns into a spin;
2. a `COMMENTUM` whose extent does not end `-->`, or a `PROCESSIO` whose
   extent does not end `?>` — these SEGFAULT the parser (§7.2).

Measured: 7/7 poison inputs flagged, 9/9 safe clean, 0 false positives on
22 real `.stml` files.

**Caveat, stated honestly:** 22 files is a small sample, and this is a
heuristic over an observed symptom, not a proof of the hang condition. It is
good enough to put in front of the emitter today. It is NOT a substitute for
the guard belonging in `_liberos_legere`.

### §3.2.1 PORTA HAEC NON SUFFICIT — reparationes parsatoris praecedere DEBENT

**Decreed 2026-08-29, on finding `01M171YAEP` after this section was
written.** A bare `<` in text followed by a space and more text —
`<p>x < 10</p>`, ordinary prose — **SEGFAULTS the parser**, and the
pre-check above does NOT catch it: zero zero-width lexemes, no unterminated
comment. `stml_lexemata_colligere` walks it cleanly and hands it to a crash.

A lexeme-level heuristic cannot route around a defect the lexer itself does
not see. **Therefore `01M16YP7W1`, `01M16Z03YE`, and `01M171YAEP` must be
fixed BEFORE this tool ships, not alongside it.** The §3.2 pre-check remains
worth having as defence in depth, but it is no longer the plan.

**The naive check does not work:** `ultimum.finis < input.mensura` fails,
because the guard appends the stalled zero-width token before breaking, so
the last `finis` still equals the input length. **Zero WIDTH is the signal,
not short coverage.**

---

## §4 `formare`

Read → `stml_scribere(radix, piscina, VERUM)` → stdout or `-ad`.

**CORRECTIO (2026-09-01, at build time):** `formare` does NOT run
the §3 chain — no expansion, no distribution. Measured basis: the
pretty gate (`probatio_stml_pulchrum.c`) never calls `stml_expandere`,
so the formatter's fixed-point contract is defined over SOURCE trees.
If `formare` expanded, it would consume template definitions (the
formatter would DESTROY macro-carrying sources), collapse into the
future `expandere` verb, and `-probare` would report every
template-carrying file divergent forever. The §3 chain belongs to
`vertere` and `expandere`.

**The pretty path, not the fidelity path.** This matters: the fidelity path
(`pulchrum=FALSUM`) carries ~22 divergence classes (`01M16XGV45`) which are
IRRELEVANT here because a formatter never calls it. The pretty path is
already gated by `probationes/probatio_stml_pulchrum.c` over a real file
corpus, with PORTA A (fixed point) and PORTA B (trees congruent modulo
trivia AND form). `silva/grammatica/c89-formatted.stml` is its output.

Form changes are the formatter's **job**, not a defect: `<x a="1"></x>` →
`<x a="1"/>` and `a=""` → `a` are both fixed points and both licensed.

### §4.1 Portae

- **Punctum fixum**: run twice, `cmp`. Non-negotiable — a formatter that
  does not converge is not a formatter.
- **Relectio**: the emitted form must re-parse. (This is the gate that would
  have caught §7.3.)

### §4b `expandere` — AEDIFICATUM 2026-09-01

`stml expandere <via.stml> [-ad <exitus>]` — the §3 chain whole:
legere → `stml_expandere` (templates filled, relational commands
consumed, INDAGO leaves tombstones) → `stml_distribuere` (NOT
optional, law 5) → pretty emit. Exit 0/2 (no `-probare`).

- **Vitia are named, not numbered**: the tool carries a 25-entry
  name table (`FRAGMENTUM_IGNOTUM (fragmentum '@nemo')`,
  `ARGUMENTUM_SUPERFLUUM (loculus 'munus')`, …) plus fragmentum/
  loculus context when present. Distribution failures name the
  offending wrapper.
- **Idempotent** (measured): expansion consumes definitions and
  vocations, so `expandere` of its own output is byte-identical.
- **Runs lints from the shell**: relational commands execute during
  expansion, so a lint document's relatum comes out of stdout; with
  `<INDAGO/>` the stage stories are visible in place. This is the
  "half a debugger" property — measured live (CATENA tombstone with
  per-stage in/out counts through the CLI).
- Caveat: a lint over C code needs the arbor document in scope;
  cross-file transclusion is still deferred (§5.6), so that usage
  waits on concatenation or a future `-adiunge` flag.

### §4.2 Cautio de corpore

**Reformatting committed hand-written `.stml` files is Fran's decision, not
the tool's.** The tool must exist; the sweep is not automatic. Carried
forward from the parked desideratum verbatim.

---

## §5 `vertere` — STML → HTML

Read → (chain §3) → walk → HTML.

The premise, in Fran's words: *"you write html tags (or any tags) in stml
syntax and they are translated into html compatible syntax."* **No tag
vocabulary, no mapping table, no house dialect.** The tags are the author's.
STML supplies syntax, macros, and the formatter. The emitter's whole job is
the short list of places where STML syntax and HTML syntax disagree.

`<!DOCTYPE html>` is **emitted automatically** at the top of every output.
STML source carries no doctype line. (Decreed by Fran 2026-08-29 — this
also removes the lowercase-doctype hang from this tool's surface entirely.)

**Radix:** the emitter writes the doctype, then the document node's children
in order. Whether the author wrapped them in `<html>` is the author's
business — the tool never synthesizes structure it was not given. A fragment
file therefore yields a doctype followed by that fragment, which is what the
author asked for.

### §5.1 Tabula decretorum — omnis constructio

| constructio | probatio in arbore | emissio HTML |
|---|---|---|
| elementum ordinarium | genus ELEMENTUM + omnes exclusiones §5.2 | `<t attrs>…</t>`, clausura per §5.3 |
| textus | genus TEXTUS | escape per §5.4 |
| commentum | genus COMMENTUM | `<!--` + valor + `-->`; valor est CRUDUS, numquam re-escapatur |
| DOCTYPE | genus DOCTYPE | **IGNORATUR** — emissor suum scribit |
| PROCESSIO `<?…?>` | genus PROCESSIO | **RECUSATIO** — HTML PI non habet |
| TRANSCLUSIO `<<sel>>` | genus TRANSCLUSIO | **RECUSATIO** (§5.6) |
| fragmentum | `nodus->fragmentum` | **DISSOLVITUR** — liberi soli, nullum tagum |
| augmentatio `<% &k;>` | `augmentum_clavis != NIHIL` | **RECUSATIO** |
| elementum attributi | `attributum_titulus != NIHIL` | consumptum ut attributum; ut liberum PRAETERITUR |
| tagus crudus `<t!>` | `nodus->crudus` | tagum sine `!`; contentum **VERBATIM** (§5.5) |
| titulus punctatus `<.x>` | `titulus->datum[0] == '.'` | **RECUSATIO** — non est nomen HTML legale |
| titulus NIHIL (`< >`) | `titulus == NIHIL` | **RECUSATIO** |
| operatores captionis | `captio_directio` | **IGNORANTUR** (§5.7) |
| clausura anonyma `</>` | `clausura_anonyma` | **IGNORATUR** — emitte `</titulus>` verum |
| multilinea `<t\>` | `nodus->multilinea` | emitte `valor`; **`indentatio` IGNORATUR** (§5.8) |

### §5.2 Ramus elementi — quinque exclusiones

Three constructs carry `genus == ELEMENTUM` without being elements. The
house's own canonical list is the guard in `_spinae_liberum_unicum`
(`lib/stml.c:6518-6531`). There is **no public predicate** — hand-roll the
field tests, and prefer the field over a `titulus` string compare (`"#"`,
`"%"`, `"@"` are internal placeholders).

```c
si (nodus->genus != STML_NODUS_ELEMENTUM)  { /* alia genera */ }
si (nodus->fragmentum)                     { /* dissolve */ }
si (nodus->augmentum_clavis   != NIHIL)    { /* recusa */ }
si (nodus->attributum_titulus != NIHIL)    { /* praeteri */ }
si (nodus->titulus == NIHIL)               { /* recusa */ }
/* cetera elementa vera sunt */
```

`crudus`, `multilinea`, `captio_directio`, `clausura_anonyma` all mark
**real** elements — do not exclude them.

### §5.3 Clausura — per leges HTML, numquam per STML

**The emitter decides closure by HTML's rules, never by how STML would
write it.** A browser reads `<div/>` as an *open* div and swallows the rest
of the page.

- titulus in the **void set** (`area base br col embed hr img input link
  meta param source track wbr`) → `<br>`, bare. Children or an express
  close on a void element = **RECUSATIO** (`<br></br>` is illegal HTML).
- otherwise → `<t></t>` always, even when childless.

The void table is new; nothing in the repo has one. `html_lexema.c` knows
the *rawtext* set (`_est_crudum`) but not the void set — the lexer never
needed it.

### §5.4 Effugium — DUAE regulae, non una

**This is the trap that would silently corrupt output.** Text values are
entity-DECODED in the tree; inline attribute values are stored **RAW**.

| positus | in arbore | in emissione |
|---|---|---|
| textus | decodatum | escape `&` → `&amp;`, `<` → `&lt;`, `>` → `&gt;` |
| attributum **inscriptum** (`attributa` Xar) | **crudum** | emitte octetos ut sunt |
| attributum **elementum** (`attributum_titulus`) | **decodatum** | escape `&` → `&amp;`, `"` → `&quot;` |
| contentum crudum (`crudus`) | verbatim | **nihil** |
| commentum / PI | crudum | **nihil** |

Escaping an inline attribute value like text yields `&amp;amp;`.

**Ergo `stml_attributum_capere` MUST NOT be used by the emitter.** Its doc
comment at `lib/stml.c:4318` claims *"capere SENSUM ambobus reddit"* — it
does not (`01M16YRSZ5`, measured). It also cannot enumerate which attributes
exist, only whether a named one does.

**The attribute loop:**

1. **Inline pass** — walk `nodus->attributa`. `attr->valor` is RAW;
   re-emit the bytes unchanged inside `"`.
2. **Prefix pass** — from child 0, while
   `genus == ELEMENTUM && attributum_titulus != NIHIL`. **Stop at the first
   child failing that test** — mirror `capere`'s `frange` exactly, or the
   two surfaces disagree. Name = `attributum_titulus`; value = child[0]'s
   `valor`, DECODED, so escape it.
3. **Tombstone** — `liberi == NIHIL || xar_numerus(liberi) == ZEPHYRUM` →
   emit **nothing** for that name. Test this BEFORE reading child[0].
4. **Child pass** — skip every child with `attributum_titulus != NIHIL`.

Passes 1 and 2 cannot collide: the parser REFUSES a name given both ways
(`lib/stml.c:2874`, status 5). Two *inline* duplicates do pass the parser —
both are emitted, matching the source.

A non-prefix attribute-element is a call argument that expansion should have
consumed; post-expansion it is anomalous → **RECUSATIO**, never a silent drop.

### §5.5 Tagi crudi — `!` est obligatorium pro `<script>` et `<style>`

JavaScript is full of `<` and `&`. Without the raw marker, STML parses your
JS as markup. HTML's rawtext set is *exactly* `script`/`style`, so the
correspondence is natural — but it must be remembered.

**Therefore: a `<script>` or `<style>` that is NOT `crudus` is a
RECUSATIO.** If it was written without `!`, the content was already parsed
as markup and something is wrong upstream. Better a loud error than a
mangled page.

### §5.6 Transclusiones — recusatio, non resolutio

Transclusions survive expansion **unresolved**, and there is **no library
function** to resolve them — every consumer rolls its own (arbor keeps a
fragment hash returning the *same* token object; canon resolves
transparently for judging only).

V1 **refuses** them. A zero-argument template does the same job through the
same engine, and one mechanism beats two. Cross-file transclusion is a
later, separate project (Fran, 2026-08-29).

### §5.7 Captiones — nullum vestigium

`_processare_captiones` (`lib/stml.c:3841`) rewrites the tree into ordinary
`parens`/`liberi` nesting during `stml_legere`, and children come out in
**document order for all three directions**. Emit children in array order
and the result is correct.

**Do NOT branch on `captio_directio`.** The STML writer does
(`lib/stml.c:7808`, `7860`, `7908`) because it must reproduce author bytes.
An HTML emitter has no such duty.

### §5.8 Multilinea — emitte `valor`, ignora `indentatio`

The dedent is applied at parse time. Only the **common** prefix is stripped;
relative indentation survives; opening and closing newlines are trivia, not
content; there is always exactly one text child.

So `valor` is already the semantically correct block, and `indentatio` is
the STML file's own structural indentation at that nesting depth —
reinserting it inside `<pre>` would shift every line by however deep the tag
happened to sit, and that offset changes whenever the document is
reformatted. The library agrees: `stml_textus_valor` is documented as
reformat-invariant; `stml_textus_internus` as deliberately layout-sensitive.

### §5.9 SPATIUM ALBUM — emissor triviam legere DEBET

**The single most important structural constraint, and the one an emitter
written by intuition gets wrong.**

Whitespace lands in two different places depending on whether it contains a
newline (`_textum_tractare`, `lib/stml.c:2579-2599`):

```
<a>x</a> <b>y</b>     →  a real TEXTUS child with valor " "
<a>x</a>\n<b>y</b>    →  NO text node — the \n moves into a's spatia_post
```

An emitter that walks `liberi` alone emits `<a>x</a><b>y</b>` and renders
**"xy" where the author wrote "x y"**. Pretty-printed source puts a newline
between virtually every sibling pair, so this hits essentially every inline
element in any real document.

The bytes are all preserved — but only for an emitter that reads
`spatia_ante` / `spatia_post` / `spatia_clausurae` in addition to walking
children. **This is the first test to write.**

### §5.10 Entia — dic characterem verum

STML resolves only the five XML entities. `&nbsp;`, `&mdash;`, `&#8212;` and
every other HTML entity arrive as literal bytes beginning `&`, which the
emitter must escape to `&amp;` — so `&nbsp;` would render as the visible
text "&nbsp;".

Worse, they are **indistinguishable from their escaped spelling**:
`&nbsp;` and `&amp;nbsp;` produce identical trees (`_unescape_entities`,
`lib/stml.c:2399-2404`). Both authors' intent is in the same bytes; there is
no correct emitter.

**Ergo: `vertere` REFUSES any `&`-word-`;` shaped run in text**, with the
message *type the actual character*. Real `—`, real `…`, real non-breaking
space, UTF-8 throughout — which is better practice anyway.

A genuine `&` still works perfectly: "Tom & Jerry" escapes to
`Tom &amp; Jerry`. Only the ambiguous shape is refused.

### §5.11 Attributa `="true"` — recusatio angusta

The parser stores interned `"true"` for a bare boolean attribute
(`lib/stml.c:391`), making `<x a>` and `<x a="true">` byte-identical trees.
Parse-side loss; no emitter can recover it (`01M16XHBE2`).

Consequences are not cosmetic: `<a href>` becomes `href="true"`, a live link
to a file named `true`; `<input value>` renders the word "true";
`aria-expanded` REQUIRES the literal string and bare is invalid.

**Ergo:** when `valor == "true"` and the name is NOT in HTML's known-boolean
set (`disabled checked required readonly selected multiple autofocus hidden
async defer novalidate open reversed loop muted controls default ismap
itemscope nomodule playsinline`), **REFUSE**, naming the line and pointing at
the attribute-element form `<@aria-expanded=>true</>` — which sidesteps the
parse-side loss entirely and already exists.

---

## §6 Formae quas auctor scribit

```stml
<html lang="la">
<head>
  <meta charset="utf-8"/>
  <title>Instrumenta</title>
  <style!>
    body  { font: 14px/1.5 ui-monospace, monospace; }
    .card { border: 1px solid #ddd; padding: 1rem; }
  </style>
</head>
<body>
  <h1>Instrumenta</h1>

  <#@card nomen="@nomen" munus="@munus">
    <div class="card">
      <h3>&@nomen;</h3>
      <p>&@munus;</p>
    </div>
  </#>

  <<#@card nomen="silva"  munus="parsator C89">>
  <<#@card nomen="natura" munus="genera">>
</body>
</html>
```

No doctype line — `vertere` writes it.

### §6.0 DISTRIBUTIO — compendium listarum, gratis

`stml_distribuere` (shipped 2026-08-27) turns anonymous fragments inside a
named wrapper into N copies of that wrapper. Verified 2026-08-29:

```stml
<ul>
  <li><>alpha</><>beta</><>gamma</></li>
</ul>
```
→ `<ul><li>alpha</li><li>beta</li><li>gamma</li></ul>`

**Attributes carry down** — `<li class="item"><>a</><>b</></li>` gives both
siblings `class="item"`. This is a real ergonomic win for HTML authoring
(lists, table rows, card grids) and it costs nothing: distribution is
already law 5 of the chain (§3.1).

It applies at **any** level, including the wrapper nearest the root:
`<r><>a</><>b</></r>` → `<r>a</r><r>b</r>`.

**ACIES — singularia HTML: EMITTITUR UT SCRIPTUM (decretum Franis
2026-09-01).** `<body><>x</><>y</></body>` emits two `<body>` elements.
The "tags are yours" principle governs FULLY: `vertere` never judges
HTML validity anywhere (invalid attributes and invented tags already
pass), so singletons pass too. No singleton table exists in the
emitter. (The house-instinct alternative — refuse — was presented and
declined; do not re-add the check.)

### §6.1 Syntaxis templorum (verificata contra fixuras virides)

- **Definitio:** `<#@nomen p="@p"> … &@p; … </#>` — an attribute whose
  *value* begins `@` DECLARES the slot; the attribute NAME is not the slot
  name. Optional slot: `p="@p?"`.
- **Referentia:** `&@p;` — one grammar, all positions (whole attribute
  value, interpolated `y="pre-&@p;-post"`, in text, or as a whole text node
  where a subtree splices).
- **Vocatio inscripta:** `<<#@nomen p="valor">>`
- **Vocatio bloci:** `<<#@nomen>><@p=>valor</>` — for values containing `"`
  or wanting real layout.
- **`@param` declares, `&@param;` references. NOT interchangeable.**
- **CORRECTIO MENSURATA (2026-09-01):** call arguments address the
  SLOT name (the value after `@`), NEVER the declaring attribute's
  name. `<#@card nomen="@n">` called with `nomen="silva"` fails
  `ARGUMENTUM_SUPERFLUUM (loculus 'nomen')` — the slot is `n`. This
  spec's §6 example originally made exactly that mistake; the fix
  above names slots after their attributes (`nomen="@nomen"`), which
  is the house convention: mirror the names and the distinction
  disappears.

### §6.2 Laquei auctoris — omnes clamosi nisi primus

| laqueus | quid fit |
|---|---|
| **`</#nomen>` pro `</#>`** | **TACITUS.** The closer never carries the id. The parser faithfully preserves the malformed input and reports 0 vitia — an honest green that is lying (`lib/stml_macros.worklog.md` 2026-08-27) |
| vocatio ante definitionem | `FRAGMENTUM_POSTERIUS` (II), distinct from `IGNOTUM` (I) |
| argumenta bloci non adiacentia | the engine consumes the maximal run of argument-elements *immediately* following the call; a stranded one becomes ordinary content |
| idem nomen inscriptum ET bloco | `ARGUMENTUM_GEMINUM` — "last wins" never happens |
| subtree in positione chordae | vitium VII, **soft** — see §3.1 lex 2 |

---

## §7 Vitia substrati quae hoc opus postulat

Found 2026-08-29 designing this tool. Full detail in the tickets.

### §7.1 Circulus infinitus — `01M16YP7W1` — **OBSTAT**

`_tok_proximus` checks raw mode (`lib/stml.c:1484`) **before** the EOF check
(`1516`). Once `in_crudus` is set, `STML_TOKEN_FINIS` is unreachable and the
child loop spins at 100% CPU with flat RSS — a true fixed point, not a slow
parse. Seven confirmed inputs through two doors:

- **door 1** — genuinely unterminated raw content: `<x!>a`, `<x!>a</y>`,
  `<x!>a</>`, `<x!>a</#>`
- **door 2** — `<!` fallthrough mis-lexed as a raw tag with an EMPTY title:
  `<!x>`, `<!!>`, `<!doctype html>` (byte 2 must be uppercase `D`;
  `<!DoCtYpE>` parses, `<!dOCTYPE>` hangs)

**One guard closes both doors** — mirror the one
`stml_lexemata_colligere` already carries at `lib/stml.c:1759-1765` into the
parser's token loop. Confirmed independently by two agents.

Until then, §3.2's pre-check. **Without the fix, `exit 2` is unreachable for
these inputs and a directory sweep wedges on the first bad file.**

### §7.2 Ruina — `01M16Z03YE` — **OBSTAT**

`_tok_legere_commentum` (`lib/stml.c:428-475`) assigns `token.valor` ONLY
inside the branch that finds `-->`. No terminator → returns an
**uninitialized stack local**, which `chorda_internare` then dereferences.
`_tok_legere_processio` (`477-521`) is identical. `<!--x` — five bytes —
segfaults; so does `<?x`, and realistically `<r><!-- x</r>`.

`_tok_legere_doctype` does NOT have the bug — it assigns unconditionally
outside the loop (`563`). Three near-copies that diverged on one line;
doctype is the pattern to follow.

**More dangerous than it looks:** uninitialized reads are nondeterministic
in principle. Here it segfaults reliably; under a different stack layout it
could intern adjacent memory into the tree and return `successus=1`.

### §7.3 Attributa quota simplici — EXECUTUM 2026-09-01

**Done.** `quota_simplex` flag on `StmlTokenContext` (the §7.5.5
`non_clausum` pattern exactly: set in `_tok_legere_valor_attributi`,
cleared at the top of every `_tok_proximus`, read at the single choke
point `_parser_progredi` before the token is overwritten) →
`STML_ERROR_ATTRIBUTUM` naming the line. Single quotes INSIDE
double-quoted values stay content (pinned).

**The migration was VACUOUS, measured**: the ~15 grep candidates below
were all prose-internal quotes; a parse-based sweep of all 57 committed
`.stml`/`.canon` files through the flipped parser found ZERO refusals.
The grep-cannot-distinguish caveat below was exactly right.

`<x a='say "hi"'/>` parses, then emits `<x a="say "hi""/>` which does NOT
re-parse — on the fidelity path AND the pretty path (measured). A file that
parses cleanly becomes one that does not.

**Fran's decree (2026-08-29): single-quoted attribute wrappers are rejected
at parse time.** This dissolves the bug rather than fixing it — with only
`"` as a wrapper, a `"` can never appear inside a value, because the value
terminates at the first one. Values needing a `"` use the attribute-element
form, which already exists:

```stml
<a>
  <@attr=(>"hi"
```

**Migration:** grep finds ~15 candidate sites (`natura/natura.canon`,
`silva/c89.canon`, `natura/cocta/individua.canon`, `book_assets/librarium.stml`),
**none containing a `"`** — all mechanical rewrites to double quotes.
Two caveats: grep cannot distinguish an attribute from text or a comment, so
verify with a parse-based check before migrating; and some of those files are
GENERATED, so the generator may need the change too.

### §7.4 Non obstant, sed filata

- `01M16YRSZ5` — `capere` entity asymmetry (§5.4 routes around it)
- `01M16XHBE2` — `="true"` collapse (§5.11 refuses)
- `01M16XGV45` — fidelity-writer divergences; **irrelevant to this tool**,
  which uses the pretty path. Includes A2 (retro/farcimen attributes
  swallowed into text — a *lexer* bug in an STML construct no HTML author
  writes)
- `href=/a/b` — unquoted values accept only `[A-Za-z0-9_-]`, so this yields
  `href` with NIHIL valor plus a text node `/a/b>`, reporting success
- `a=""` gives `valor == NIHIL`, meaning **empty string, not absent**.
  An emitter skipping NIHIL drops `alt=""`, which is load-bearing for
  screen readers and is not the same as a missing `alt`

---

## §7.5 REPARATIONES POSTULATAE — quattuor, omnes parvae

Ratified by Fran 2026-08-29. §7.5.1–3 must land BEFORE the tool (§3.2.1);
§7.5.4 may land beside `formare`.

### §7.5.0 PRAEREQUISITUM — assertiones processus in credo

Ratified by Fran 2026-08-29. **Must land FIRST** — §7.5.1–3 are not testable
without it.

The three defects crash or hang `stml_legere`. `probationes/` runs
in-process, so a test feeding `<!--x` would take all 141 root tests down with
it, and a test feeding `<!doctype html>` would **wedge the runner forever** —
a red state that never returns.

**Surface** (`include/credo.h`; impl in `lib/credo.c`, which already includes
`postulata_posix.h`, so the header stays POSIX-free):

```c
CREDO_NON_RUIT   (expressio)             /* survives, no signal        */
CREDO_NON_PENDET (expressio, mora_ms)    /* terminates within deadline */
CREDO_RUIT_CUM   (expressio, signum)     /* asserts it DOES die, named */
```

**Why fork-only, not `processus_exsequi`:** `processus` is EXEC-based
(`argumenta[0]` = a path to a binary), so it would need a separate binary per
case plus argv marshalling, and the assertion would live in shell, outside
credo's reporting. `fork()` without `exec` gives the child the parent's
memory, so the expression runs in place beside the other assertions. Reuse
`processus`'s *vocabulary* (`signum` = WTERMSIG, `mora_ms`,
timeout-as-distinct-outcome), not its mechanism.

**Leges implementationis** — each of these is a real bug if skipped:

1. **Detection is `WIFSIGNALED`, never an exit code.** A crashed process has
   no exit code. `WIFEXITED`→`WEXITSTATUS`; `WIFSIGNALED`→`WTERMSIG`.
2. **`_exit()` in the child, NEVER `exit()`.** The child inherited the
   parent's stdio buffers; `exit()` flushes them and runs atexit handlers, so
   every prior line of test output is emitted twice.
3. **`fflush(NULL)` in the parent BEFORE forking** — the same duplication
   from the other side.
4. **Deadline by polling, not `alarm()`.** `waitpid(pid, &st, WNOHANG)` in a
   loop with a short `nanosleep` to the deadline, then `kill(pid, SIGKILL)`
   and reap. SIGALRM collides with any signal use in the code under test.
5. **SIGKILL for the timeout, not SIGTERM.** A wedged parser never runs a
   handler; and `WTERMSIG == SIGKILL` then unambiguously means "we killed
   it" rather than "it died".
6. **Name the signal** — SEGV(11)/BUS(10)/ABRT(6)/FPE(8)/KILL(9). "signal
   11" is a worse diagnostic than "SEGV".
7. **NEVER catch SIGSEGV in-process.** Handler + `longjmp` is the approach
   everyone reaches for and it is UNDEFINED BEHAVIOR — you may not return
   normally from a handler for a computational exception, and after a real
   fault the arena and allocator state are unknown. It also cannot detect
   hangs. Fork is the only sound mechanism.
8. fork is ~1ms; fine for dozens of cases, never in a hot loop.

**PORTA NATIVA — culpa inserta (doctrina: porta tacita = porta mortua).**
A crash detector that silently fails to detect reports green forever, which
is worse than having none. So the facility ships WITH a test that
deliberately segfaults — a null write behind an opaque function so the
optimizer cannot fold it — asserting the harness CATCHES it. Without that
there is no evidence the detector detects.

**Fructus ulterior:** `CREDO_RUIT_CUM` turns a ticket into an executable pin.
`01M16Z03YE` gets a test asserting `<!--x` dies with SEGV *today*; when
someone fixes it the test goes RED and says so. A far better encoding of
"known bug" than a comment, and the general mechanism for turning "this bit
me AGAIN" into a permanent guard.

### §7.5.1 Custos progressus — `01M16YP7W1`

Transplant the guard `stml_lexemata_colligere` already carries at
`lib/stml.c:1759-1765` into the parser's token loop (`_liberos_legere`,
`2733`):

```c
si (ctx->positus <= ante) { /* RECUSA */ }
```

**One difference from the lexer's version:** the lexer `frange`s out of a
collection loop, which is right there — the caller sees a truncated stream.
The parser must **raise an error**, or a hang is merely traded for a silent
truncation.

Closes seven confirmed inputs through both doors (unterminated raw content;
`<!` fallthrough mis-lexed as a raw tag with an empty title), and closes the
class rather than the instances.

### §7.5.2 `token.valor` non initializatum — `01M16Z03YE`

`_tok_legere_commentum` (`428-475`) and `_tok_legere_processio` (`477-521`)
assign `token.valor` ONLY inside the terminator-found branch. At EOF they
return a stack local never written, which `chorda_internare` dereferences.

Initialize before the loop (`datum` = content start, `mensura` = ZEPHYRUM),
or raise `TAG_NON_CLAUSUM` on the EOF path. **`_tok_legere_doctype` already
does this correctly at `563` — copy its shape.**

**Scope, measured 2026-08-29:** only a genuinely unterminated comment or PI
crashes. `<!-- x --->`, `<!-- x ---->`, `<!-- a -- b -->` and `<!---->` all
parse cleanly — the terminator search finds `-->` wherever it sits and extra
dashes are content.

**SEMANTICA POSTEA CORRECTA (Fran, 2026-08-29, commissum `180eceb7`).**
This fix first made unterminated comment/PI *succeed* with content-to-EOF,
matching doctype. That was the wrong convention to copy — see §7.5.5.

**`<?` note:** PROCESSIO is live but trivially — 37 sites across 37 files,
every one the identical `<?xml version="1.0" encoding="UTF-8"?>` declaration.
`<?` is NOT in the sigil registry (`stml-visio.md` §2); it is inherited XML,
never a designed STML sigil. So `?` as a tag-name prefix is available in
principle but occupied in practice. Either way this fix is independent of
what `<?` eventually means.

### §7.5.3 Titulus NIHIL — `01M171YAEP`

**This is NOT a parsing problem.** STML, like XML, does not disambiguate
prose from markup: a bare `<` in text is an ERROR and `&lt;` is the escape.
That policy is already implemented — `<p>a <b</p>` and `<p>a <</p>` both give
clean errors today. The parser already concludes "malformed"; it crashes on
the way to *saying so*, because `titulus_ptr` (`1969`) is dereferenced at
`1997` with no NIHIL guard.

**AN EMPTY TITLE IS A MODELLED STATE, NOT A DEFECT.** This is the load-bearing
fact, and two proposed fixes died on it. `stml.h` declares
`STML_STRICTUM_TITULUS_VACUUS = IV`, and `probatio_stml.c` (~4118) pins BOTH
halves of a deliberate two-layer design:

```c
r = stml_legere_ex_literis("< >x</>", piscina, intern);
CREDO_VERUM(r.successus);                       /* parser is LENIENT */
vitia = stml_strictum(r.radix, piscina);
CREDO_AEQUALIS_I32(xar_numerus(vitia), I);      /* strictum is the JUDGE */
```

The parser accepts; `stml_strictum` judges. Making the parser reject collapses
that. (The same test's comment records that `<>` used to be judged here too,
until 2026-08-27 rehabilitated it as anonymous-fragment sugar for DISTRIBUTIO.)

**Fix — guard the dereference ONLY**, at the close-tag comparison:

```c
        alioquin si (   titulus_ptr == NIHIL
                     || !chorda_aequalis(ctx->current.valor,
                                         *titulus_ptr))
        {
            _errorem_ponere(ctx, STML_ERROR_TAG_IMPROPRIE, ...);
        }
```

A NIHIL title can never match a closing name, so `TAG_IMPROPRIE` — the same
verdict given for any mismatched close — is the honest answer. The document
still parses; `stml_strictum` still reports the empty title.

**DUAE FORMAE RECUSATAE — ne quis eas iterum temptet:**

1. **Reject empty names at LEX time** in `_tok_legere_tag`. **Breaks `</>`.**
   The dispatch table in `_tok_proximus` routes `<<`, `<!--`, `<?`, `<!D`,
   `</#`, `</%`, `<#`, `<%`, `<(`, `<>` to dedicated readers — but `</>`
   falls through to `_tok_legere_tag`, and its empty name is exactly the
   signal read at `1988` to set `clausura_anonyma`. 9 uses in
   `probatio_stml_macros.c` alone.
2. **Reject a nameless OPENING tag** in `_parser_legere_elementum`. **Breaks
   the strictum layer** — measured: `probatio_stml` red at 4118 and 4123.
   It also needs `_parser_progredi` before returning NIHIL or
   `_liberos_legere` spins on the unconsumed token, trading a crash for a
   hang.

### §7.5.3.1 RELIQUUM: '&lt;!doctype html&gt;' TACITE ineptit

After §7.5.3, `<!doctype html>` no longer hangs — but it parses as the
nonsense element `<! doctype html/>` and REPORTS SUCCESS. It is lexed as a
RAW tag with an empty title, so it flows through
`_parser_legere_elementum_crudus`, which the guard above does not cover.

Not fixed in Plan A. **DECRETUM (Fran, 2026-09-01): the raw-element
parser REFUSES an empty title.** No legitimate construct reaches the
raw path titleless (`<!DOCTYPE` uppercase lexes as doctype, `<!--` as
comment; the rest is always a typo) — a parse error naming the line
replaces the silent nonsense element, for every consumer at once.
Lands as its own increment before `vertere`.

### §7.5.5 CONSTRUCTIO NON CLAUSA = ERRATUM (Fran, `180eceb7`)

Fran's question after the merge — *did we fix everything correctly in terms
of how the existing layers work?* — exposed that §7.5.1 and §7.5.2 had made
the parser more LENIENT without making anything the JUDGE. `<x!>a` and
`<!--x` succeeded with no vitium, and the writer then invented the missing
terminator.

The first proposal was two new `stml_strictum` genera. Measuring killed it:

```
<a>x        -> status 3 (TAG_NON_CLAUSUM)   <- the house convention
<x!>a  ·  <!--x  ·  <?x  ·  <!DOCTYPE x     -> all OK
```

**Unterminated is ALREADY a parse error** — for ordinary elements, the
primary construct. The other four were the outliers. So: no new vocabulary,
no new node state; make the four agree with the one already right.

The danger is concrete: forget a `-->` mid-edit and the formatter closes the
comment FOR you, swallowing what followed and then terminating it.

**Raw:** an `alioquin` in `_parser_legere_elementum_crudus`. No state — the
`/* Expect close tag */` comment sat above an `si` with no `alioquin`.

**Comment/PI/doctype:** `b32 non_clausum` on **`StmlTokenContext`**, NOT on
`StmlToken`. **14 functions construct a StmlToken with no shared
initializer**, so a 15th field there invites exactly the uninitialized-read
defect of §7.5.2. The context has 2 explicit init sites. Cleared at the top
of every `_tok_proximus` (cannot go stale); read in the parser BEFORE
`_parser_progredi`, since advancing fetches the next token and clears it.

**Result:** all five agree. Zero fallout — root 143/143, silva 50/50,
materia 5/5; no fixture depended on the lenient behavior.

### §7.5.4 Effugium nimium — decretum Franis — EXECUTUM 2026-09-01

**Done.** The `casus '"'` arm is deleted; decoder untouched. Exposure
measured first: zero committed `.stml`/`.canon` files carried
`&quot;`, zero fixture changes needed, stml suites 7/7 (pulchrum +
aurea included). Details: `lib/stml.worklog.md` 2026-09-01.

`_scribere_evasus` (`lib/stml.c:6029-6037`) escapes four characters; the
parser decodes five. **Delete the `"` case. Keep `<` and `>`.**

- **`&lt;` is REQUIRED** — `<` starts a tag; without the entity a `<` cannot
  appear in text at all. It is also what makes §7.5.3 a policy rather than a
  gap.
- **`&gt;` is OPTIONAL** — `>` is inert in text. Kept for symmetry and
  convention; it costs 2 corpus sites.
- **`&quot;` is pure damage** — 47,353 sites in `book_assets/librarium.stml`
  alone (4.8 MB of prose whose ordinary quotation marks all become entities;
  the file grows 41 KB and every quoted phrase becomes noise).

Provably safe: the parser already accepts `"` bare in text — verified that
`<x>a"b</x>` and `<x>a&quot;b</x>` produce identical trees. **Text only;**
attribute values are raw in both directions and untouched.

**This one reaches the PRETTY path too** (measured), so without it
`stml formare` mangles every quotation mark in prose. It is therefore a
`formare` correctness matter, not a cleanup.

---

## §8 Probationes

**Order matters — the first two are the ones that catch real bugs.**

1. **Spatium album (§5.9).** `<a>x</a> <b>y</b>` and `<a>x</a>\n<b>y</b>`
   must BOTH render "x y". The single most likely silent defect.
2. **Punctum fixum formatoris** — run twice, `cmp`.
3. **Relectio** — `formare` output must re-parse; `vertere` output must
   survive `html_lexema`. That is a **second oracle of a different kind**,
   not a golden made by the writer itself — the failure that let all seven
   of the fidelity defects hide.
4. **Recusationes** — one fixture per refusal in §5, each asserting the
   exit code AND that the message names a line.
5. **Clausura** — every void element bare; every non-void paired; `<div/>`
   in source emits `<div></div>`.
6. **Attributa** — inline raw vs attribute-element decoded (§5.4); tombstone
   omits; `alt=""` survives.
7. **Macros** — the §6 page end-to-end.

**Coverage guard:** every sweep must assert its own coverage (`> 0` sites
seen), never a pinned count. "N/N clean" meaning "never appeared" is exactly
how `="true"` and `<x></x>` hid for months.

**Post-stml edits:** `./tools/natura_struere.sh` (house reflex).

---

## §9 Quaestiones apertae — decreta Franis

1. **`>` et `"` nimis effugata — RESOLUTUM** (Fran, 2026-08-29): delete
   `"`, keep `<` and `>`. Full reasoning and evidence: **§7.5.4**. Note the
   correction to an earlier claim in this spec — it is NOT fidelity-path
   only; it reaches the pretty path, so it is a `formare` correctness
   matter.
2. **`<x></x>` → `<x/>` — EXTRA SCOPUM** (Fran, 2026-08-29): collapsing an
   empty tag on auto-format is INTENDED for STML. The residual question
   concerns only the fidelity writer's unqualified §4 byte-exactness claim,
   and nothing in production depends on it — the fidelity path has exactly
   four production call sites (twice in `silva_arbor.c` COMPARING two nodes,
   where canonicalization helps; twice in `forum.c` for compact emission).
   It becomes real only when `mutatio` (`01M0DKV2K1`) or a refactoring tool
   needs minimal diffs. Filed, not fixed: `01M16XGV45`.
3. **CRLF — DECLARANDUM, non solvendum.** No CRLF file round-trips: the
   parser canonicalizes CRLF→LF once over the whole document on input
   (`_crlf_canonicalizare`, `lib/stml.c:1633`), and nothing in the tree
   records it. **It converges** (measured): a CRLF file is reported
   divergent once, `-ad` writes it as LF, and every run thereafter matches.
   So the only requirement is that the line-ending change be **declared** in
   the tool's documentation and its `-probare` message, never a surprise.
   A lone `\r` not followed by `\n` survives as content.
4. **`-probare` — RATIFICATUM** (Fran, 2026-08-29). Both modes ship.

### §9.1 Inventum structurale — porta fidelitatis ad scalam plagulae NUSQUAM est

`probatio_stml_pulchrum.c` — the only test walking a real file corpus —
calls `stml_scribere(…, VERUM)` and never `FALSUM`. `probatio_stml_aurea.c`
tests `stml_textus_internus` and calls `stml_scribere` zero times.
`probatio_stml.c`'s 53 fidelity assertions are all snippets.

Yet the trivia spec §4 says, unqualified: *"This IS the fidelity oracle:
`scribere(legere(x), non-pretty) == x` for the whole corpus."*

**The contract is stated but ungated at file scale.** That is why
`silva/grammatica/c89.stml` violates it at seven sites with every test
green. Not a writer that broke — a promise nobody checks. Worth its own
decision independent of this tool.
