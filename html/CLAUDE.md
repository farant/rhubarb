# html — orientation

*You are reading this because you touched a file in `html/`.*

HTML parser on **materia** (fifth client, after the C89 shim, css, md
and oratio). Design: `project-specs/html-arbor-spec.md` — **§11, the
2026-09-15 addendum "As built on materia", governs the body** (the
body was written against silva); §11.11 = as built. Plan:
`project-specs/html-arbor-plan.md` (H1–H11, CLOSED 2026-09-15, one
commit each). Lexer: `lib/html_lexema.c` (root tier, 2026-08-18; total,
byte-faithful, owns raw text and RCDATA — the builder never tells it
anything). Findings at find-time: `html/fontes/html_arbor.worklog.md`.

## Praesens status (2026-09-15 — PLAN CLOSED)

- **Built:** declaration `html/grammatica/html.registrum.stml` →
  GENERATED `html_registrum_coctum.{h,c}` (10 genera, 25 loci — the
  annotations `synthesis:index` on `elementum` (O7a),
  `sedes:referentia` on `elementum`/`textus`/`referentia`/
  `commentarium` (O7b), `exemplar:referentia` and
  `praecedens:referentia` on `elementum` (O7c)); hand slot enums
  `html_registrum.h` (+ `HtmlSynthesis`); `html_lexicon` (22 rows:
  trivia INSIDE TAGS ONLY, comments are content); `html_adaptare`
  (one for one) + `HtmlLigator` (css's division rule); `html_arbor`
  (the builder: ITERATIVE over an explicit stack — the spec's DOM
  stack with byte-open frames — three pending states, tables `VACUA`
  and `CLAUSURAE_IMPLICITAE`, per-frame scope indices and table
  modes, WHATWG wrapper modes, token-less synthesized elements,
  foster parenting as an annotation, the active formatting list and
  the adoption agency; document entry `html_arbor_parsare` + fragment
  entry `html_arbor_parsare_fragmentum(…, contextus, spatium)`);
  `html.canon` (60 rules, seal `b307882e` pinned — moved `0ae63151`
  → `a69b019f` by O7a, → `5bf2be5d` by O7b, → `b307882e` by O7c);
  `html_computus` + `./html/computus.sh` + `./html/arbor.sh`;
  pythonica `PORTAE['html']`, `silva.metiri('.html')`.
- **Gates (14, `./html/compile_probationes.sh`, every one born red):**
  registrum · adaptare (byte coverage) · arbor (byte oracle + absent-slot
  rows; structure cases run as a `body` FRAGMENT, wrappers and
  synthesis in document mode) · corpus (14 files, 162,775 bytes,
  byte-identical) · stml (two cycles + comparator + direct emission of
  the re-read tree; O7 ACCEPTANCE: synthesized wrappers/table parts,
  nine fragments, and the `synthesis`, `sedes`, `exemplar` and
  `praecedens` annotations themselves read back) ·
  canon (drift guard both ways, seal pin, 37 documents) · reservatio ·
  totalitas (919 cases; depth pins) · computus (golden
  `fixa/computus/basis.tsv`) · md (md's rendered HTML — body content,
  parsed as a `body` fragment — through the whole chain, 7 inputs) ·
  exempla (the html5lib `.dat` reader over the vendored corpus: 54
  files, 428,448 bytes, 1,708 cases, 184 fragments PINNED;
  `probationes/fixa/html/FONTES.md`) · oraculum (our tree through the
  cooked view `html_coctum` against html5lib's expected trees, WHOLE
  — the O2 unwrap rule is retired, the builder synthesizes the
  wrappers; RISING pin — 887 at birth, 930 after O2b-1, 985 after
  O2b-3, 1,047 after O2b-4, 1,054 after O2b-5, 1,086 after O2b-6,
  1,112 after O4, 1,189 after O5, 1,195 after O6, 1,299 after O7a,
  1,398 after O7b, 1,504 after O7c — of 1,700, fragments 155/184;
  `ORACULUM_OMNIA=1`,
  `ORACULUM_EXEMPLUM=tests1.dat:20`) · alienum (the shared SVG/MathML
  namespace tables `html_alienum`, 28) · **circuitus** (Fran's
  question, 2026-09-15: the WHOLE html5lib corpus through the byte law
  and the STML cycle — every one of the 1,708 cases, script-on
  included, fragments through the fragment entry: direct emission
  byte-identical to the input for ALL; the two-cycle STML round trip
  with comparator and re-read emission identical for 1,672, the 36
  others refused by the STML writer for NUL bytes — pinned BY CAUSE,
  the raw-form limit 01M2KPJ0HW; any other cause is a failure). The
  runner compiles `materia/fontes` and `md/fontes` into `html/build/`
  — consumed, never contained.
- **What it found:** in itself, one byte-order bug (a pending close
  taking a later `>`; totality, H8). In the substrate, ZERO changes
  forced (css 4, oratio 5) and two measurements: the raw-form limit
  (a token value containing its own `</lex-…>` is REFUSED, not escaped
  — ledger 01M2KJ3PK2, pinned in the stml gate) and the depth (parse
  survives 100,000 levels; `materia_scribere_nodum` dies at 50,000,
  pinned RUIT_CUM; STML projection quadratic in depth, 2,000 deep =
  56 MB — 01M1FAD8). In md, sabaw's `<if⟨c⟩>…</if⟨⟩>` notation passed
  raw (pinned at one malum; md polish 01M1NBEVM0).
- **Numbers:** STML projection ≈ 15× the source; round trip ≈ 19× the
  parse (css 30×).
- **Oracle in progress (plan "Oraculum html5lib", O1–O4):** O1 DONE
  2026-09-15 — the html5lib tree-construction corpus vendored from the
  LOCAL Go module cache (`probationes/fixa/html/html5lib/`, WebKit BSD,
  no network; Fran's choice), reader `html/fontes/html_exempla.{h,c}`
  mirroring html5lib's own `support.py` TestData rules, gate
  `probatio_html_exempla`. **O2 DONE 2026-09-15:** `html_coctum` (the
  COOKED VIEW: lowercase names, decoded entities, unquoted sorted
  attributes, coalesced text, CR/NUL cooked, `<svg svg>` namespaces,
  template `content`, mala dropped) + `probatio_html_oraculum`, first
  number **887/1,700 (52 %)**, fragments 77/184, pinned rising. The
  failure classes are in the worklog: cheap rises are builder TABLE
  entries (`head` closed by `body`, the full p-closing list) and lexer
  states (`--!>`, script escaping); the expensive ones are the §6.1
  mechanisms (table synthesis, adoption agency, foreign breakout).
  **O2b in progress (Fran: burn them down, one commit each, pin
  moving):** O2b-1 builder tables (887 → 930), O2b-2/3 `--!>` cooking
  + script-data escaping in `lib/html_lexema.c` (→ 985), O2b-4 the
  shared generated entity table `lib/entitates_html` (→ 1,047; md
  consumes it too), O2b-5 scope for p/li/dd/dt/button as per-frame
  visibility indices (→ 1,054), O2b-6 foreign content (`html_alienum`
  shared by builder and cooked view: per-frame namespaces,
  self-closing acknowledged in SVG/MathML, breakout; → 1,086) DONE.
  **Fran, 2026-09-15: keep pushing conformance** ("flesh out html
  pretty well while the tests are here"), cheap chunks first: O4
  lexer raw-text set (→ 1,112) and O5 tokens the DOM ignores
  (frameset/select modes, doctype after content, repeated
  html/head/body, `</body>`/`</html>` → `elementum-malum`; → 1,189)
  and O6 end tags in three classes (own rule / in scope / stop at
  "special"; → 1,195) DONE. The cheap chunks were exhausted there.
  **O7 (design approved by Fran 2026-09-15, ledger decree 01M2M1B279):
  the expensive mechanisms as DERIVED-VIEW ANNOTATIONS on the html
  registry, materia untouched** — `synthesis:index` (O7a),
  `sedes:referentia` (O7b, foster parenting), `exemplar:referentia`
  (O7c, adoption agency); acceptance = the STML round trip stays
  whole. **O7a DONE 2026-09-15 (→ 1,299, fragments 85 → 131):** the
  wrappers html/head/body and tbody/tr/colgroup as token-less
  synthesized elements, the fragment entry point (the context is the
  frame under the stack), table parts ignored outside a table, the
  fragment case of foreign breakout (x/net: never in a fragment), a
  `<frameset>` after a synthesized body removes that body. **O7b DONE
  2026-09-16 (→ 1,398, fragments 131 → 152, ZERO regressions):**
  `sedes:referentia` on four genera = the DOM parent of a node whose
  bytes lie elsewhere; table modes per frame (`modus_tabulae`,
  `tabulae`, `partis`), foster parenting (the node stays a byte-child
  of the table part, `sedes` names the table's parent, the cooked view
  prints it before the table), `</body>`/`</html>` now CLOSE (the O5
  law reversed) and later content carries `sedes` to the element that
  was open, head content after `</head>` carries `sedes` to head,
  `<input type=hidden>` stays in the table (decided when its
  attributes are read), `form` in a table is not pushed, the spec's
  fragment-context scope rules. The corpus round trip carried every
  reference. **O7c DONE 2026-09-16 (→ 1,504 = 88 %, fragments 152 →
  155, ZERO regressions; adoption01 18/18, adoption02 2/2):** the
  adoption agency — `exemplar:referentia` (a formatting element
  reopened by the agency or by reconstruction: no tokens, name and
  attributes through the reference to the original) and
  `praecedens:referentia` (the moved node's DOM previous sibling —
  byte order cannot place it; the one addition beyond the decree),
  the stack as the spec's DOM stack with byte-open frames (O7b's
  `sedes_posterior` retired), the active formatting list, `</p>`/
  `</br>` synthesized elements. What remains is a long tail: template
  interplay (22), ~~frameset-after-content (plain-text-unsafe 14)~~
  **— that label was WRONG, see "The NUL tail" below —**
  tests19, the foreign tail (svg.dat 0/8: SVG `title` is not raw;
  `font` breakout needs attributes — readable at `>` via the fovens
  pattern), raw-text fragment contexts (tests4). Next: the foreign
  tail, then O3 wild fixtures (one ask per page). Candidate next
  parser after html (Fran's research): bash scripts, before JS.
  Ledger desideratum 01M2KPNTT0.

## The NUL tail — the label was wrong (2026-09-19)

`html/CLAUDE.md` called `plain-text-unsafe`'s 14 failures
"frameset-after-content", and the arithmetic matched exactly
(33 − 19 = 14), which made it look settled. **It was a coincidence of
totals, not evidence.** Measured, those 14 are THREE separate bugs:

| n | bug | state |
|---|---|---|
| 8 | **NUL treated as content** in wrapper modes | ✅ FIXED, pin 1504 → **1509** |
| 4 | `frameset-ok` polarity | ✅ table FIXED (→ **1510**); the 4 cases need the body-removal law, below |
| ~2 | `<frameset>` inside foreign content vanishes | open |

**Fixed.** HTML5 IGNORES a NUL character token in the wrapper modes
(parse error). We made it content, which forged a body and made the
following `<frameset>` a malum. `_textus_albus_cum_nullo` now routes a
whitespace-and-NUL run to `elementum-malum` — **malum, not
`spatium-omissum`**, because NUL is a fault and dropped whitespace is
not. The lexer makes one token per byte run, so a run is ignored whole;
splitting it would need a synthetic token (H4 forbids).

**`frameset-ok` — table now the spec's own list** (`COMPAGIS_NOCENTIA`,
2026-09-19). It was inverted: 16 head-ish tags innocuous, everything
else clearing, so `<p>` and `<div>` cleared the flag when they must
not. Flipping it *alone* measured **1509 → 1503** — because my first
list was incomplete, not because the hook is mode-blind (my earlier
note said that; it was wrong). The spec sets "not ok" for `li`, `dd`,
`dt` too, and for `<body>`. Two further rules landed with it:

- **An IGNORED token can still have effects.** A repeated `<body>`
  creates no node, but the spec still sets frameset-ok from it. So
  `<div><body><frameset>` ignores the frameset. "Ignored" means no
  NODE, not no consequence.
- **Inside a `template` the frameset is ignored** regardless of the
  flag — the spec's condition is the stack shape ("second element is
  not a body"), a different question from frameset-ok.

Net 1509 → 1510 with ZERO regressions, verified by diffing the whole
failure set, not by the total alone.

**Why the remaining 4 are NOT a frameset-ok problem.**
`<svg><path></path></svg><frameset>` expects the body and all its
content REPLACED by the frameset (expected tree is 3 lines; ours is 6).
`_corpus_fictum_removere` refuses when the body has non-malum children
— and that is the **O5 law working**: a node with bytes can never be
removed. Expressing "present in bytes, absent from the DOM" needs a new
annotation, the same shape as O7a/O7b/O7c. A decree, not a patch.

**`<frameset>` in foreign content** should be an ordinary foreign
element (`frameset` is correctly absent from `RUMPENTIA`), but the node
vanishes entirely. Guarding the body-removal branch on
`proprium == HTML_ALIENUM_NULLUM` changed nothing, so the loss happens
elsewhere — that guard was reverted rather than shipped unmeasured.

**Reading the oracle diff:** it prints the first differing line, so when
our tree is SHORTER the last common line gets flagged and the two sides
look identical. `<svg>NUL<frameset>` reported line 5 `"�"` on both
sides; the real difference was a missing line 6. Check the lengths
before believing an "identical" diff.

## Diagnostica — TIER 1 only, declared (2026-09-19)

`./tools/diagnostica.sh <x.html>` prints the compiler form with a
caret. One genus carries `diagnosticum`: **`elementum-malum`**. No C
was written for it, and **diagnostics move no seal**.

**Why a genus split had to come first.** materia keys a diagnostic on a
GENUS only (`diagnosticum` is a genus attribute; `absentia`/`vacua` are
locus attributes — there is no condition on a node's contents). And
`elementum-malum`, which O5 widened into the catch-all for everything
HTML5 ignores, answers *"does the DOM keep a node?"* while a diagnostic
must answer *"did the author err?"* Those disagree **518 times out of
531** over the house. So the benign half moved to its own genus
**`spatium-omissum`** (whitespace the DOM drops in the two wrapper
modes `ANTE_RADICEM`/`ANTE_CAPUT` — NOT after `head`, where the spec
inserts it as real text). Result: 531 → **13**, pages 260 → 10.

**Do not port crusta's `absentia` declarations here.** html's absences
are LEGAL — void elements, implied closes, self-closing, synthesized
wrappers all leave NIHIL token slots (H4). `tok_finis` absent fires on
every synthesized `html`/`head`/`body`. crusta's work because a missing
`fi` is always wrong; html has no such slot.

**The token genus is not the predicate either**: a newline before
`<head>` is a benign `lex-textus` malum; `x` inside `<frameset>` is an
erroneous one. Only the builder's mode knows, at the moment it drops.

Classes kept TOGETHER deliberately: "HTML5 ignores this" (repeated
`<html>`, doctype after content, table parts outside a table,
select/frameset) and "unmatched end tag". The first has **zero
instances** in the house *and* in `../lapide` (651 files sampled), so
splitting it would design against nothing. Another append + seal move
is cheap if it ever gets noisy.

## Laws (spec §11)

- **Absent slots are meaning** (H4): void, implied close, unclosed at
  EOF, self-closing, truncated tag = NIHIL token slots, never synthetic
  tokens. `/>` does NOT close an HTML element (HTML5); `</br>` and a
  `</p>` with no `p` in scope give a SYNTHESIZED `br`/`p` whose only
  token is the closing tag (O7c; the `</br>`-is-malum deviation is
  retired).
  **`elementum-malum` = the tokens the DOM keeps no node for** (O5):
  unmatched end tags, but also everything HTML5 "ignores" — a repeated
  `<html>`/`<body>`, a doctype after content, tags inside a frameset
  or a select, table parts outside a table mode. The bytes stay in
  place; the cooked view drops the malum. (O5 also kept `</body>` and
  `</html>` as mala with the body open to EOF; O7b reversed that: they
  CLOSE, and content after them carries `sedes`.)
- **`sedes` is the DOM parent of a node whose bytes lie elsewhere**
  (O7b, 2026-09-16): a `referentia` locus on `elementum`, `textus`,
  `referentia`, `commentarium`. The node stays where its bytes are;
  the cooked view prints it under its `sedes`, BEFORE the child of
  the sedes that is a `table` and contains it (foster parenting), else
  AFTER all children (content after `</body>`/`</html>` in the element
  that was open, head content after `</head>` in head, fragments in a
  table context). Three sources: foster parenting (current node
  `table tbody tfoot thead tr` in a table mode and a token the mode
  does not handle: `sedes` = the table's parent, the document root in
  a fragment; `input type=hidden` is decided when its attributes are
  read; `form` in a table is inserted, not pushed), content after
  `</body>`/`</html>` (`sedes` = the DOM current node: the stack no
  longer pops there, its frames become byte-closed — O7c; comments
  only once content has re-entered the body; `</body>` inside a table
  or template is a malum), head content after `</head>`, and the
  adoption agency's moves (O7c, with `praecedens`). Table modes are
  per frame
  (`modus_tabulae`, nearest `tabulae`, nearest real `partis`, own
  `pars_tabulae`), O(1): a table-part start tag clears the stack back
  to the nearest real part, `<table>` in a table mode closes the open
  table, a foreign element never sets a mode, and in a fragment whose
  mode comes from the context alone the spec's "in table scope"
  checks fail (table body accepts tr/td/th, row td/th, caption
  nothing, column group col, table everything but table).
- **Text is a node** (H6): between tags every byte is `TEXTUS`; the
  ligator binds trivia only inside tags; `FINIS` receives trivia only
  after a tag cut off at EOF.
- **The builder is iterative** — an explicit open-element stack, never
  recursion. Every PENDING state (tag, clausura, malum) is closed by the
  first token that is not its own — the H8 bug was a state that
  survived a token it did not own.
- **Implied closes: scope for `p`/`li`/`dd`/`dt`/`button`, top of the
  stack for the rest** (O2b-5, 2026-09-15; was top-only under H1):
  `<p><b>x<div>` closes p through b (button scope), `<li>a<div><li>`
  closes the li through div but `<li>a<pre><li>` does not (`pre` is
  "special", `div` is excepted). Scope is answered from PER-FRAME
  visibility indices set at push (`ScopiGradus`), never by walking
  the stack: a walk per start tag was quadratic and killed the
  100,000-deep pins on its first run. Keep it O(1) per token.
- **Foreign content by name, not by namespace** (O2b-6): the builder
  keeps each frame's own and children's namespace (`html_alienum`,
  derived from names and integration points); inside SVG/MathML a
  self-closing tag closes its element and an HTML breakout tag pops
  the foreign elements. In HTML `/>` still never closes.
- **Synthesis is an annotation, not a token** (O7a, 2026-09-15): what
  HTML5 invents — the wrappers `html`/`head`/`body` (WHATWG modes
  "before html" … "after head", tracked as `HtmlModus`) and the table
  parts `tbody`/`tr`/`colgroup` — is an `elementum` with NO tokens and
  the `synthesis` index set (`HtmlSynthesis`, names in `html_arbor`).
  H4 holds: the emitter walks through it and writes nothing; the
  cooked view prints it; the STML round trip carries the index. A real
  end tag may close a synthesized element (`</head>`, `</tbody>`: the
  closing tokens land in the fictum). A synthesized node CAN be removed
  (`<frameset>` after an invented body: its mala move up); a node with
  bytes never can (O5). Whitespace before html/head is a malum (the
  DOM drops it); a comment before html stays under the document.
  Inside an open `template` the wrapper modes are silent and table
  parts are accepted (per-frame `templi`, `in_tabula`). A FRAGMENT
  (`html_arbor_parsare_fragmentum`) puts the context UNDER the stack
  (`basis`: namespace, select, frameset, table scope) and never
  synthesizes wrappers except in an `html` context; breakout from
  foreign content never happens in a fragment (x/net's `!p.fragment`).
- **The stack is the DOM stack; bytes decide who receives** (O7c,
  2026-09-16): every frame carries `octeti` (byte-open — it can still
  take byte-children) and `receptor` (the nearest byte-open frame at
  or below it). A new node is appended to the receptor; when the DOM
  current node is another frame, it gets `sedes` = that node. A
  frame turns byte-closed when an element below it takes a closing
  tag (`</body>`, a formatting element's `</a>`) — it stays open in
  the DOM as the spec keeps it. The ACTIVE FORMATTING LIST (`Formans`:
  element, stack index validated on use, marker flag; markers for
  applet/object/marquee/template/td/th/caption, purged lazily when
  their element leaves the stack; Noah's Ark) is reconstructed before
  text and before the start tags the spec reconstructs for
  (`NON_RESTITUENTIA` names the exceptions), never in select, foreign
  content or a frameset. The ADOPTION AGENCY follows WHATWG on that
  stack: a reopened formatting element is an `exemplar` (no tokens,
  reference to the ORIGINAL — never to a clone), a moved block keeps
  its bytes and gets `sedes` = the common ancestor plus `praecedens`
  = the formatting element (or the foster sedes when the ancestor is
  a table part), the block's children are WRAPPED in the clone in the
  byte tree (byte-safe: the clone emits nothing; fostered
  byte-children stay outside, nodes whose `sedes` was the block now
  point at the clone), the stack segment is replaced in one pass and
  scope indices recomputed. The closing tag goes to the formatting
  element's closure when it is byte-open, to a malum otherwise. `a`
  and `nobr` are NOT in the implied-close table any more — the agency
  handles `<a>` (the old `a` leaves list and stack) and `<nobr>` in
  scope; leaving them there produced clones through reconstruction.
  The foster parent is the table's DOM parent (`sedes` or `pater`),
  not the frame below it. Cooked-view placement of a sedes-node: after
  its `praecedens` if set, else before the containing table, else
  after all children.
- **Nothing reserved in v1** (§11.6): the reader refuses unknown
  elements with a named cause and IGNORES unknown attributes (measured,
  pinned); extension points are named in the declaration's header —
  `synthesis` (O7a), `sedes` (O7b), `exemplar` and `praecedens` (O7c)
  are DECLARED. A `referentia` locus makes materia write `id="nodN"`
  on the target and `<sedes(> #nodN` (`<exemplar(>`, `<praecedens(>`)
  on the node: the canon declares `id` on `elementum` and `documentum`
  (the targets).

## The registry is generated

`HtmlGenus`, `HTML_GENUS_NUMERUS_GENERUM`, `HTML_REGISTRUM` come from
`html/grammatica/html.registrum.stml` via `./materia/coquere.sh
html/grammatica/html.registrum.stml -scribere`. To add a genus or
locus: append to the declaration (locus order = emission order), run
`-scribere`, add the slot enum in `html_registrum.h`, the row in
`LOCI_NOMINATI` (registrum probatio), the rule in `html.canon`, and move
the seal there by hand with a cause (done thrice, O7a: `0ae63151` →
`a69b019f`, O7b: → `5bf2be5d`, O7c: → `b307882e`; the canon gate
prints the live seal). Node creation sizes come from the registry
(`_loci_numerus`), never a hand count — the registrum probatio's
hand-built element rotted that way once (O7c) and now sizes itself
from the registry too. A hand edit of a `_coctum` file goes red with
the first divergent line.

## Birth lessons

- A planted anchor inside a comment needs `tolerans='verba'` (a comment
  is one lexeme). Plants in non-C files (canon, TSV) are done by hand.
- zsh: `$?` after a pipe is the pipe's; `set -- $spec` and `"$F"` do NOT
  split — use `${=spec}`. Every depth probe reported the same crash
  until the split was explicit.
- `CCCC` is not a numeral; `CD` is. Transcribed css files carry lines
  over 72 the formator now refuses.
- A gate over random, mutated and truncated input finds what fourteen
  clean pages cannot; a consumer of a consumer finds what neither
  parser's own gates can.

## Currere

```
./html/compile_probationes.sh            # omnes (≈ 5 s)
./html/compile_probationes.sh totalitas  # filtrum substringae
./html/computus.sh <x.html> [-machina] [-iter N]
./html/arbor.sh <x.html> [-tacitus]      # proiectio STML in stdout
```

0 sanum / 1 fractae / **2 = NULLA CURSA**. Log: `build/test_logs/html.log`.
