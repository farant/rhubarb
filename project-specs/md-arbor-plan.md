# md_arbor — plan, DRAFT before interview (2026-09-02, night)

> Status: draft written from the census, the spike
> (`md-arbor-spica.md`) and Fran's decisions of 2026-09-02. The
> interview and `md-arbor-spec.md` will correct it; tasks below carry
> exit conditions, not code. House convention: inline execution, no
> subagents; every gate born with a planted fault.

**Goal:** a markdown parser on materia (third client), thorough
(CommonMark blocks and inlines, GFM tables), byte-exact both ways,
projected to STML, with HTML produced by STML template COMPOSITION over
the projected tree as the second oracle, and pythonica's structural
prose anchors as the first consumer.

**Decisions already taken (do not re-litigate at execution):**
- Consumer one = pythonica `Textus` structural anchors; thorough dialect.
- Lines are block-level tokens; inline structure is an ANALYSIS tree over
  the paragraph's tokens (shares tokens, never nodes — the B9 pattern),
  so inline needs no byte gate of its own.
- Fences opaque first; embedded-language hook later (2,540 C fences).
- Reference links resolved IN THE PARSER (CommonMark: resolution is
  parsing); the transform never joins.
- Separating oracle per CommonMark example: parser's STML projection
  (pinned, named-cause regen) + html vs spec html; byte round trip third.
- HTML transform = template COMPOSITION (dispatcher + per-genus templates,
  recursion through `de="@arg"`), AMBULATIO only as fallback.
- HTML bytes via `stml vertere` over the produced html tree.

**Global constraints:** the plan-B block verbatim (Latin, latina.h words
forbidden, chorda not NUL-terminated, i32/i64 unsigned, exit 2 = nothing
ran, never stage Fran's staging files, silva frozen, planted fault at
every gate's birth, positions = byte offsets + 1-based linea/columna,
`./tools/natura_struere.sh` after STML edits). Gates through pythonica:
`silva.commissio_umbra(..., ['md', 'materia', 'pythonica'])` once the
`md` runner exists — register it in PORTAE in the SAME task that creates
it (a runner outside the table is dead: shim lesson, three times).

**Open at interview (each decides a task's shape):**
1. Heading levels: six-case DIRIBITIO in the transform, or a
   titled-by-value element form in the engine?
2. Template self-call under the strata law: permitted when the argument
   is a strict subtree? (Fallback: AMBULATIO verb.)
3. Dialect list: which CommonMark features and GFM extensions
   (tables yes; task lists, strikethrough, autolinks?).
4. The genus vocabulary in Latin (capitulum, paragraphus, lista,
   elementum, saeptum, citatio, tabula, linea-thematica, inlinea, textus,
   emphasis, fortis, codex, nexus, imago, fractura-dura, entitas…).
5. Whitespace as trivia vs content per region (the markdown D7):
   indentation inside containers, blank lines between blocks, trailing
   spaces (hard breaks), fence contents.
6. Where the html oracle's normalizer lives and what it forgives.

---

## Arc A — the parser (materia client)

### A1 Census committed + dialect decree
Files: `md/CENSUS.md` (numbers from the 2026-09-02 census as a
committed measurement), `project-specs/md-arbor-spec.md` (from the
interview). Exit: spec §dialect lists every construct with its
CommonMark section and whether v1 builds it.

### A2 Registry, lexicon, line lexer
Files: `md/fontes/md_registrum.{c,h}` (genera + named slot enums,
count-asserted like css), `md/fontes/md_lexicon.{c,h}`,
`md/fontes/md_lexema.{c,h}` (line tokens: prefix runs — indentation,
`>`, list markers, fence markers, setext underlines, thematic breaks —
and the rest of the line as one text token), `md/compile_probationes.sh`
(auto-discovering, mensor-wired, REGISTERED in pythonica PORTAE the same
commit), `probatio_md_registrum`, `probatio_md_lexema`. Exit: lexer
round trip byte-exact over the whole house corpus (1,118 files);
registry enum count asserted against the table.

### A3 Simple blocks
Paragraphs, ATX + setext headings, thematic breaks, fenced + indented
code, blank lines, HTML blocks as opaque. Files: `md/fontes/md_arbor.{c,h}`
(`md_arbor_parsare(piscina, fons, mensura)` → `MateriaNodus*`, source not
copied — the css_arbor law), `probatio_md_arbor`. Exit: parse→emit
byte-exact on inline fixtures + the corpus subset without containers;
structure asserted (a clean file has no `mala`).

### A4 Containers
Lists (bullet, ordered, start number, tight/loose), nesting, block
quotes, lazy continuation, list-item indentation rules. Exit: the
CommonMark container examples (their inputs) round-trip byte-exact;
tight/loose recorded on the list node; pinned structure for 20 chosen
examples.

### A5 Tables (GFM) + link reference definitions
Pipe tables with alignment row; definitions collected document-wide and
RESOLVED onto link nodes (`url=`, `descriptio=`) at parse end. Exit:
tables round-trip; a reference link carries its target; an unresolved
reference stays a text run (CommonMark semantics).

### A6 The corpus gate
`probatio_md_corpus`: byte oracle over all 1,118 files + the CommonMark
example inputs; the green INTERROGATED as B5 was (structure assertions
on clean files, zero `mala`, real headings/lists counted). Exit: 1,118/1,118
byte-exact; a planted fault (skip a list) fails the assertion.

### A7 Inline analysis tree
`md/fontes/md_inlinea.{c,h}`: over a paragraph's tokens — code spans,
emphasis/strong by the delimiter-run algorithm, links/images (inline +
resolved reference), autolinks, hard/soft breaks, backslash escapes,
entities kept raw. Shares TOKENS never NODES; no byte gate by design
(header says so). Exit: the CommonMark inline examples' structure pinned
for a chosen subset; emphasis algorithm cases from the spec's §6 all
pinned.

### A8 STML projection, canon, totality, computus
`materia_arbor_scribere_nodum` / `legere` round trip with BOTH oracles
over corpus + examples (two cycles); `md/grammatica/md.canon` hand-written
with the drift guard + sigillum pin (css B7 pattern); `probatio_md_totalitas`
(random bytes, mutated/truncated corpus, depth); `md/computus.sh` +
`probatio_md_computus` golden (css computus pattern; columns identical so
phase-5 comparisons read across clients). Exit: all four gates green, each
seen red at birth.

## Arc B — the transform by composition (engine + programs)

### B1 Engine: `de="@arg"` + self-call rule + extent provenance
`lib/stml_macros.c`: argument→scope (exemplaria spec §8 door 5) with
declared capture signatures; a template may call itself when the
argument is a strict subtree (termination by descent) — or AMBULATIO if
the interview rejects self-calls; exemplaria §1.3 extent rule (positus
carried through expansion for untouched/moved content). Gate: the spike
documents in `project-specs/exhibita/` with expected html pinned;
existing L1/lint II/codex-69 gates unchanged. Exit: the nested list in
the spike produces one `<ul>` with a nested `<ul>` inside its `<li>`.

### B2 The md→html program
`md/html/md-html.stml`: `<#@md-nodus>` dispatcher (DIRIBITIO on genus)
+ per-genus templates (heading levels by the interview's decision,
lists tight/loose by CASUS on the parent's attribute, code fences with
`class="language-x"`, links with resolved `href`, text escaped by
`vertere`'s regime). Exit: the spike's three examples + 30 chosen
CommonMark examples produce the spec's html after normalization.

### B3 The html oracle
`probatio_md_html` (or a runner script): for every CommonMark example
(the spec's `spec.json`, vendored under `md/probationes/fixa/commonmark/`
with its version pinned): parse → project → expand B2 → `vertere` →
normalize → compare with expected html. Two goldens per example (A's
projection, B's html). Exit: pass count pinned and only rising; failures
listed by section; a planted fault in B2 red.

## Arc C — the consumer

### C1 pythonica structural prose anchors
`silva.Editio` twin for markdown: `Prosa(via)` with `capitulum(titulus)`,
`elementum(...)`, `replace_in(...)` resolving by md structure (extents
via `md/extenta.sh`, the formator `-extenta` pattern), stale-read guard,
all-or-nothing write, no formatter/examen. Exit: the pythonica gate edits
a spec by heading and by list item; refusals name lines.

### C2 Wire-up, spec closure, debrief (the B10 shape)
Runners registered (already, per A2), spec as-built notes, worklog with
the three questions, ledger RELATIO + instrument debrief, memory.

---

## Order and size
A1–A8 ≈ 12 tasks, B1–B3 ≈ 6, C ≈ 3: about 21, matching the estimate.
B1 can start after A3 (it needs only the spike documents); A7 and B2 are
the two hard ones. Compact before execution; re-enter via this file, the
parcum "Parsator markdown in materia", and `md-arbor-spica.md`.
