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
  GENERATED `html_registrum_coctum.{h,c}` (10 genera, 18 loci); hand
  slot enums `html_registrum.h`; `html_lexicon` (22 rows: trivia INSIDE
  TAGS ONLY, comments are content); `html_adaptare` (one for one) +
  `HtmlLigator` (css's division rule); `html_arbor` (the simple builder:
  ITERATIVE over an explicit stack, three pending states, tables `VACUA`
  and `CLAUSURAE_IMPLICITAE`, top-of-stack rule); `html.canon` (53 rules,
  seal `0ae63151` pinned); `html_computus` + `./html/computus.sh` +
  `./html/arbor.sh`; pythonica `PORTAE['html']`, `silva.metiri('.html')`.
- **Gates (10, `./html/compile_probationes.sh`, every one born red):**
  registrum · adaptare (byte coverage) · arbor (byte oracle + absent-slot
  rows) · corpus (14 files, 162,775 bytes, byte-identical) · stml (two
  cycles + comparator + direct emission of the re-read tree) · canon
  (drift guard both ways, seal pin, 37 documents) · reservatio ·
  totalitas (919 cases; depth pins) · computus (golden
  `fixa/computus/basis.tsv`) · md (md's rendered HTML through the whole
  chain, 7 inputs) · exempla (the html5lib `.dat` reader over the
  vendored corpus: 54 files, 428,448 bytes, 1,708 cases, 184 fragments
  PINNED; `probationes/fixa/html/FONTES.md`) · oraculum (our tree
  through the cooked view `html_coctum` against html5lib's expected
  trees, html/head/body unwrapped when the input never names them;
  RISING pin — 887 at birth, 930 after O2b-1, 985 after O2b-3, 1,047
  after O2b-4, 1,054 after O2b-5, 1,086 after O2b-6, 1,112 after O4,
  1,189 after O5 — of 1,700, fragments 85/184; `ORACULUM_OMNIA=1`,
  `ORACULUM_EXEMPLUM=tests1.dat:20`) · alienum (the shared SVG/MathML
  namespace tables `html_alienum`, 28). The runner compiles
  `materia/fontes` and `md/fontes` into `html/build/` — consumed,
  never contained.
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
  DONE; next O6 template, then the design turn on
  `clonatum`/`reinserendum` for the adoption agency and table
  synthesis. O3 wild fixtures after. Candidate next
  parser after html (Fran's research): bash scripts, before JS. Then O3 wild fixtures (one ask per
  page), O4 the lexer's raw-text set. Then JS. Design: ledger
  desideratum 01M2KPNTT0.

## Laws (spec §11)

- **Absent slots are meaning** (H4): void, implied close, unclosed at
  EOF, self-closing, truncated tag = NIHIL token slots, never synthetic
  tokens. `/>` does NOT close an HTML element (HTML5); `</br>` is malum.
  **`elementum-malum` = the tokens the DOM keeps no node for** (O5):
  unmatched end tags, but also everything HTML5 "ignores" — a repeated
  `<html>`/`<body>`, a doctype after content, tags inside a frameset
  or a select, and `</body>`/`</html>`, which never close anything
  (the body stays open to EOF, later content lands inside it). The
  bytes stay in place; the cooked view drops the malum.
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
- **Nothing reserved in v1** (§11.6): the reader refuses unknown
  elements with a named cause and IGNORES unknown attributes (measured,
  pinned); extension points are named in the declaration's header.

## The registry is generated

`HtmlGenus`, `HTML_GENUS_NUMERUS_GENERUM`, `HTML_REGISTRUM` come from
`html/grammatica/html.registrum.stml` via `./materia/coquere.sh
html/grammatica/html.registrum.stml -scribere`. To add a genus or
locus: append to the declaration (locus order = emission order), run
`-scribere`, add the slot enum in `html_registrum.h`, the row in
`LOCI_NOMINATI` (registrum probatio), the rule in `html.canon`, and move
the seal there by hand with a cause. A hand edit of a `_coctum` file
goes red with the first divergent line.

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
