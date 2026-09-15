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
  chain, 7 inputs). The runner compiles `materia/fontes` and
  `md/fontes` into `html/build/` — consumed, never contained.
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
- **Next (Fran):** the html5lib oracle with a rising pin + wild
  fixtures under a sources file + the lexer's raw-text set (xmp,
  iframe, noembed, noframes, plaintext) — plan's "named after". Then JS.

## Laws (spec §11)

- **Absent slots are meaning** (H4): void, implied close, unclosed at
  EOF, self-closing, truncated tag = NIHIL token slots, never synthetic
  tokens. `/>` does NOT close an HTML element (HTML5); `</br>` is malum.
- **Text is a node** (H6): between tags every byte is `TEXTUS`; the
  ligator binds trivia only inside tags; `FINIS` receives trivia only
  after a tag cut off at EOF.
- **The builder is iterative** — an explicit open-element stack, never
  recursion. Every PENDING state (tag, clausura, malum) is closed by the
  first token that is not its own — the H8 bug was a state that
  survived a token it did not own.
- **Implied closes look at the top of the stack only** and repeat:
  `<p><b>x<div>` does NOT close p (HTML5's "button scope" = mechanism 1
  of spec §6.1, deferred by H1).
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
