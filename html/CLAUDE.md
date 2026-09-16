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
  GENERATED `html_registrum_coctum.{h,c}` (10 genera, 23 loci — the
  annotations `synthesis:index` on `elementum` (O7a) and
  `sedes:referentia` on `elementum`/`textus`/`referentia`/
  `commentarium` (O7b)); hand slot enums `html_registrum.h` (+
  `HtmlSynthesis`); `html_lexicon` (22 rows: trivia INSIDE TAGS ONLY,
  comments are content); `html_adaptare` (one for one) + `HtmlLigator`
  (css's division rule); `html_arbor` (the builder: ITERATIVE over an
  explicit stack, three pending states, tables `VACUA` and
  `CLAUSURAE_IMPLICITAE`, per-frame scope indices and table modes,
  WHATWG wrapper modes, token-less synthesized elements, foster
  parenting as an annotation; document entry `html_arbor_parsare` +
  fragment entry `html_arbor_parsare_fragmentum(…, contextus,
  spatium)`); `html.canon` (58 rules, seal `5bf2be5d` pinned — moved
  `0ae63151` → `a69b019f` by O7a, → `5bf2be5d` by O7b); `html_computus`
  + `./html/computus.sh` + `./html/arbor.sh`; pythonica
  `PORTAE['html']`, `silva.metiri('.html')`.
- **Gates (14, `./html/compile_probationes.sh`, every one born red):**
  registrum · adaptare (byte coverage) · arbor (byte oracle + absent-slot
  rows; structure cases run as a `body` FRAGMENT, wrappers and
  synthesis in document mode) · corpus (14 files, 162,775 bytes,
  byte-identical) · stml (two cycles + comparator + direct emission of
  the re-read tree; O7 ACCEPTANCE: synthesized wrappers/table parts,
  five fragments, and the `synthesis` annotation itself read back) ·
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
  1,398 after O7b — of 1,700, fragments 152/184; `ORACULUM_OMNIA=1`,
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
  reference. Next O7c (adoption agency + `exemplar`), then the foreign
  tail (SVG `title` not raw, `font` breakout), then O3 wild fixtures
  (one ask per page). Candidate next parser after html (Fran's
  research): bash scripts, before JS. Ledger desideratum 01M2KPNTT0.

## Laws (spec §11)

- **Absent slots are meaning** (H4): void, implied close, unclosed at
  EOF, self-closing, truncated tag = NIHIL token slots, never synthetic
  tokens. `/>` does NOT close an HTML element (HTML5); `</br>` is malum.
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
  `</body>`/`</html>` (`sedes` = the element that was the current
  node then, `sedes_posterior`; comments only once content has
  re-entered the body; `</body>` inside a table or template is a
  malum), head content after `</head>`. Table modes are per frame
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
- **Nothing reserved in v1** (§11.6): the reader refuses unknown
  elements with a named cause and IGNORES unknown attributes (measured,
  pinned); extension points are named in the declaration's header —
  `synthesis` (O7a) and `sedes` (O7b) are DECLARED; `exemplar` remains
  named. A `referentia` locus makes materia write `id="nodN"` on the
  target and `<sedes(> #nodN` on the node: the canon declares `id` on
  `elementum` and `documentum` (the targets).

## The registry is generated

`HtmlGenus`, `HTML_GENUS_NUMERUS_GENERUM`, `HTML_REGISTRUM` come from
`html/grammatica/html.registrum.stml` via `./materia/coquere.sh
html/grammatica/html.registrum.stml -scribere`. To add a genus or
locus: append to the declaration (locus order = emission order), run
`-scribere`, add the slot enum in `html_registrum.h`, the row in
`LOCI_NOMINATI` (registrum probatio), the rule in `html.canon`, and move
the seal there by hand with a cause (done twice, O7a: `0ae63151` →
`a69b019f`, O7b: → `5bf2be5d`; the canon gate prints the live seal).
Node creation sizes come from the registry (`_loci_numerus`), never a
hand count. A hand edit of a `_coctum` file goes red with the first
divergent line.

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
