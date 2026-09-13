# briar — spec v1.9 (literate C89 programs; `.thistle`)

*2026-09-04. v1 consolidated the design conversation of the same day
(research nota 01M1QC21ZJ in the tabularium). v1.1 folds in the
review from the ludus/pictor session (five items) with Fran's
decisions on each. Fran's decisions are marked DECISUS; everything
else is PROPOSITUM and was agreed in conversation unless marked OPEN.
Names marked (unsealed) are working names — Fran names. Every
"exists" claim cites the header it rests on. English prose, Latin
identifiers, as in pictor-spec.md.*

*v1.9 (2026-09-12) adds §4.8, the fasciculus — `-app`, a thistle built
into a double-clickable `x.app` with an icon, decisions A1–A8. It wires
three sealed libraries (fasciculum, icones, and briar's own capsula
pattern) and builds the `<briar>` element that §2 described but nothing
ever read.*

*v1.8 (2026-09-11) adds §4.7, the spectator — `-visio`, the viewer
over the facies page, decisions S1–S4. It is §4.6's F1 step 2 and
sits beside it for that reason.*

*v1.7 (2026-09-05) adds §4.6, the facies — `-html`, the literate
rendering, designed with Fran the same day and struck from the §9
deferrals. It sits under §4 rather than in a section of its own
because it consumes the fabrica's fructus: it is the fabrica's twin,
turned toward the reader instead of the compiler.*

## 1. Purpose and scope

**briar** is a command-line tool. Given a `.thistle` file — a literate
program: prose, configuration, and C89 in one text file with a
`#!/usr/bin/env briar` first line — it parses the file, derives a
compilable project from the library corpus bundled inside its own
binary, builds it with the installed clang, caches the result, and
execs it. Only clang is external. The shape is silex's: one binary
carries the whole library tree (`capsula`, exists — capsula.h) and
scaffolds projects whose dependencies are DERIVED from includes, never
declared.

**What a thistle file is for (DECISUS 2026-09-04):** a SCRIPT. The
aim is a GUI app as easy to write as a bash script. Thistle files are
not the place for load-bearing modules or infrastructure, and they
need not live inside rhubarb at all — briar must work on a file
anywhere on disk, with no repository around it (the embedded corpus
is the fallback of silex's resolution order, §5).

**DECISUS (2026-09-04):** briar is its own parser, a materia client
that delegates the inside of each region to the markdown and STML
parsers; briar is a SEPARATE binary, not a silex verb, sharing silex's
generated corpus object; the file format uses STML-shaped tags as
region delimiters; the extension is `.thistle` (format) read by
`briar` (tool) — two names because a second reader will exist.

**In scope (v1):** the format of §2; the parser of §3 with the house
gates; the fabrica of §4 for two program shapes — a plain program (a C
region defines `principale`) and an atrium app (a `<fenestra>` region,
no `principale`) — plus an optional PROBATIO region per file, built
and run on request; the binary of §5, flag-driven; a freshness gate at
birth; a first real file that opens a vitrea window with one bridge
method, run from its shebang.

**Silva is IN v1 (Fran, 2026-09-04).** briar links silva the way
silex does (decree 01M098M3G6's route) and parses every C region
with macro expansion, serving latina.h and the closure's headers to
the parser from the capsula. The fabrica therefore knows which unit
defines `principale`, renders real prototypes for the file's helpers,
partitions translation units at unit extents, and checks a method's
signature — no textual heuristics, no double compilation.

**Out of scope (v1), all named in §9:** the interpreted mode (officina
`machinula`), named chunks and any tangling beyond document order,
regions nested inside markdown constructs, examen judgment of the
regions (one call away now, but a decree about what a script must
obey comes first), flags derived by aedilis, Linux, a sealed
distribution flag, an LSP over `.thistle`, the ludus islands
vocabulary, a `briar-c89` dialect with STML tags inside C.

## 2. The format

A `.thistle` file is a SEQUENCE of parts. No root element is
required; the document is judged whole (canon's multi-root rule,
exists — canon.h). Three region kinds plus prose:

| part | begins | ends | inside |
|---|---|---|---|
| interpres | line 1 starting `#!` | end of line 1 | the interpreter line; kept in the tree, dropped from every build |
| regio (raw) | a column-0 line `<name! attrs>` — `!` glued to the name, attributes in STML syntax (the lexer form at lib/stml.c:905–1030, exists) | the first later column-0 line `</name>` | bytes VERBATIM: C, html, js, css, md |
| elementum (STML) | a column-0 line `<name attrs>` or `<name attrs/>` | the first later column-0 line `</name>`, or the `/>` when the opening line itself is self-closing (plan 1 amendment: the column-0 law governs every boundary; a multi-line self-closing element is a named vitium, `elementum-non-clausum`) | STML, handed whole to `stml_legere` (exists) |
| prosa | any other line | the next region or interpres | markdown, handed whole to `md_arbor_parsare` (exists — md/fontes/md_arbor.h) |

**Laws.**

- **Column 0 is the only place briar looks.** `<` at column 0 followed
  by a name-start byte opens a region; anything else is prose. Prose
  therefore needs no escaping: `<chorda.h>` inside a sentence is text.
  Measured 2026-09-04: the same prose as a bare STML text node is
  refused by the STML parser at the first `<`, and CommonMark shreds a
  `<c!>` block at its first blank line — both reasons this grammar is
  briar's own.
- **Markdown fences are tracked, and inside a fence every line is
  prose.** A column-0 line (up to three spaces of indentation, as
  CommonMark allows) of three or more backticks or tildes opens a
  fence; the fence closes at a line of the same character at least as
  long. While a fence is open, a column-0 `<html>` in a documentation
  example is prose, never a region. One boolean in the lexer, no
  lookahead; the inner markdown parse sees the same fence and agrees.
  Fences do not interact with raw regions (inside `<c!>` nothing
  changes); a fence still open at end of file is a named vitium.
  Fences are for DOCUMENTATION code; executable code is always
  bounded by a tag briar knows (Fran, 2026-09-04).
- **A raw region ends only at column 0.** STML's own raw scan stops at
  `</c` anywhere (`_tok_legere_contentus_crudus`, lib/stml.c:1489); a C
  string literal containing `</c>` would end the block early there.
  Briar is stricter by design.
- **An unterminated raw region runs to end of file** with a named
  vitium; a column-0 `</name>` with no open region is prose. The parse
  is therefore ALWAYS a tree (materia's "always a tree" is per-parser;
  briar's outer grammar is total by construction).
- **Every byte belongs to exactly one token**; emission of the tree is
  byte-identical to the source (md's line model, spec §3 of
  md-arbor-spec.md).
- **The `\` dedent form** (`<c!\>`, STML's kind ladder) is ACCEPTED by
  the lexer and recorded; applying the dedent is deferred (§9).
- **The transclusion line (v1.6).** Inside a `<c!>` region a line
  that reads, after leading whitespace, exactly `<<#x>>` (trailing
  whitespace allowed) is a transclusion of fragment `#x` and yields no
  line of its own. It is STML's own sigil (`<<selector>>`, stml-visio
  §2) resolved by briar as its first C consumer, aliasing by the
  2026-08-26 decree: one object, many windows. `<<#x>>` anywhere else
  in a line stays bytes (mid-line references reserved, §9); a line
  that begins `<<#` and is not that shape is a named refusal.

**v1 vocabulary.** Raw: `c`, `html`, `js`, `css`, and `md` (prose
written explicitly, the escape for prose that must start at column 0
with a `<` outside a fence; the nexus parses it as markdown like bare
prose, the fabrica produces nothing from it). STML: `fenestra`
(window: `titulus`, `latitudo`, `altitudo`; defaults = file stem, 640,
400) and the optional `briar` (`titulus` = project name = the C symbol
of the capsula, so it must be an identifier; default = file stem with
non-identifier bytes replaced by `_`). The parser accepts ANY name in
either position; `briar.canon` judges the STML vocabulary, the fabrica
refuses an unknown raw kind by name.

**Attributes on `<c!>` (v1, names unsealed).** The region tag IS the
annotation — no comment-annotation is needed while the tag wraps the
code (Fran, 2026-09-04; the comment gate of annotationes-stml-spec.md
stays available if ever needed):

| attribute | meaning |
|---|---|
| `methodus="nomen"` | a bridge method: the region defines a function `nomen` with the `InternuntiusTractator` signature (exists — internuntius.h:61), registered as internuntius method `nomen`. Named after the parameter of `internuntius_praebere`; deliberately NOT `tractator`, which ludus reserves for UI event handlers under its no-I/O lint L5 (ludus-brainstorm.md §XII) — a bridge method reads files and talks to the network, that is what it is for |
| `munus="probatio"` | the region is the file's PROBATIO: a separate translation unit and binary using credo, exactly as every `probationes/probatio_<x>.c` does |
| `id="x"` | the region is the FRAGMENT `#x` (§3.4, v1.6): it is never a root, it compiles nowhere on its own, and its lines are woven into any root at a line reading `<<#x>>`. `#` is document-space (decretum 01KZPEXT74), so the id lives in `id=` as CSS and the TS resolver expect, not in the `nomen=` once reserved here |

**The first file** (fixture `briar/probationes/fixa/salve_vitreum.thistle`):

    #!/usr/bin/env briar
    # Salve vitreum

    Include `<chorda.h>` and the window opens with one method bound
    to a button. Prose is markdown; it never needs escaping. A
    documentation example may show a tag at column 0 inside a fence:

    ```html
    <button onclick="internuntius.vocare('salve', {})">tange</button>
    ```

    <fenestra titulus="salve" latitudo="640" altitudo="400"/>

    <html!>
    <h1>salve</h1>
    <button onclick="internuntius.vocare('salve', {}).then(
        function (r) { document.body.append(r.nuntius); })">tange</button>
    </html>

    <c! methodus="salve">
    #include "chorda.h"
    #include "json.h"

    JsonValor*
    salve (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
        chorda* culpa)
    {
        JsonValor* fructus = json_objectum_creare(piscina);

        (vacuum)argumenta; (vacuum)datum; (vacuum)culpa;
        json_objectum_ponere(fructus, "nuntius",
            json_chorda_creare_literis(piscina, "salve, munde"));
        redde fructus;
    }
    </c>

    <c! munus="probatio">
    #include "credo.h"
    #include "json.h"

    s32
    principale (vacuum)
    {
        Piscina*   piscina = piscina_generare_dynamicum("probatio", 65536);
        JsonValor* r;
        chorda     culpa;

        credo_aperire(piscina);
        r = salve(NIHIL, piscina, NIHIL, &culpa);
        CREDO_NON_NIHIL(r);
        credo_imprimere_compendium();
        redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
    }
    </c>

The plain-program twin (`salve.thistle`) is prose plus one `<c!>`
defining `principale` and printing `salve, munde`, plus a probatio
region asserting on a helper.

## 3. The parser — a materia client

Lives in `briar/fontes/`, beside css and md; materia stays thin (M8):
nothing briar-specific enters `materia/`.

### 3.1 Registry and lexicon (tables, never programs — M7)

Genera (unsealed names) and their loci, in the css/md table form
(`MateriaRegistrumCoctum`, exists — materia/fontes/materia_registrum.h):

| genus | loci |
|---|---|
| `documentum` | `interpres` NODUS? · `partes` LISTA_NODUS |
| `interpres` | `tok` TOKEN (the whole line incl. newline) |
| `prosa` | `tok` TOKEN (the whole run) |
| `regio` | `apertum` TOKEN (open-tag line) · `titulus` TOKEN† · `contentum` TOKEN? (absent = empty) · `clausum` TOKEN? (absent = unterminated) · `vitium` INDEX — plan 1 amendment: no `tok_` prefixes, the loci double as canon element names |
| `elementum` | `tok` TOKEN (balanced STML bytes) · `titulus` TOKEN† |

† = DERIVED token (`fons_index` 1, md's semantic channel): the name
without `<`, `!`, `\`; the emitter omits it. Attributes are NOT parsed
by briar: §3.3 parses the open-tag line with the STML parser, so there
is never a second attribute grammar.

Token genera (`MateriaLexiconCoctum`, prefix `briar-`): `FINIS`,
`INTERPRES`, `TAG_APERTUM`, `TAG_CLAUSUM`, `CRUDUM`, `PROSA`, `STML`,
`DERIVATUM`. All VERBATIM except `FINIS`; no trivia genus — like md,
trivia do not exist in the line model. The probatio asserts both
tables by TITLE so a permutation is caught, never absorbed.

### 3.2 Lexer and arbor

Line table first (md_lexema's shape). Classification per line: `#!` on
line 1 → INTERPRES; a fence line toggles the fence state (§2); while
the fence is open every line is prose; otherwise column-0 `<` +
name-start → TAG_APERTUM candidate (the `!` decides raw); column-0
`</name>` → TAG_CLAUSUM candidate; otherwise prose. The arbor walks
lines with a one-deep state: outside a region, a TAG_APERTUM opens
`regio` (raw) or `elementum`; inside a raw region only the matching
column-0 TAG_CLAUSUM closes it, every other line joins
`tok_contentum`; for an STML element the extent comes from
`stml_lexemata_colligere` over the remainder, tracking depth to zero
(self-closing = depth zero at once). Prose lines coalesce into one
PROSA token per run. `materia_arbor_patres_figere` (exists) after
construction, so the comparator's reconstruction policy holds.

### 3.3 Projection, canon, inner trees

- **Projection**: `briar_stml_consilium` = `materia_arbor_consilium_nudum`
  + the origo hook, copied from `md_stml.c` (70 lines). `briar -arbor
  x.thistle` prints it. FIDELIS and STRUCTURALIS round trips both
  gated.
- **Canon**: `briar/grammatica/briar.canon`, hand-written, seal pinned
  with a drift guard both ways (css's B7 pattern). It judges the OUTER
  projection and, through the nexus, the STML regions' vocabulary
  (`fenestra`, `briar`; the ludus vocabulary joins later).
- **Nexus** (`briar_nexus`, beside materia): the inner trees keyed by
  region node identity — the materia design-reach pattern "outer tree
  holds bytes as ONE raw token, inner tree separate, linked by
  identity". Three inner kinds in v1: prose and `<md!>` regions →
  `md_arbor_parsare` over the token bytes; STML element →
  `stml_legere` over the STML token bytes; raw open-tag →
  `stml_legere` over the open-tag line rewritten as `<name attrs/>`
  (the `!`/`\` stripped), which yields the attributes as a normal
  `StmlNodus`. Positions: every inner line number is offset by the
  region's first line so diagnostics name the `.thistle` line.
- **C regions → silva (the fourth inner kind, IN v1).** Each C
  region is parsed by `silva_parsare_cum_expansione` (exists —
  silva/amalgama/silva.h:791) over a `SilvaExpansio` (exists — :629)
  into which briar has fed latina.h as a synthetic first source and
  the closure's headers by TEXT through `silva_includendum_praebere`
  (exists — :639: `(exp, via, textus, mensura)`), read from the
  capsula — silva never touches disk. A bare parse expands nothing
  and misparses house C (measured, MEMORY); expansion is mandatory.
  The tree gives the fabrica: top-level unit extents
  (`silva_nodus_extensionem`, exists — :377), the unit whose
  declarator is `main` after expansion (`principale` is a latina
  macro), and rendered signatures (`silva_c89_typum_scribere`,
  exists — :1953, the renderer behind legati's `caput`). This is the
  md fence hook's door, opened here: outer raw token, inner silva
  tree, linked by identity. Semantic diagnostics from silva are
  reported at `.thistle` positions through the same offset.
  **As built (plan 2, 2026-09-05):** the silva kind lives in its own
  unit `briar_silva` — the amalgam header carries stml's own enums,
  so `stml.h` and `silva.h` cannot meet in one translation unit;
  `briar_nexus.h` forward-declares the tags and includes neither. The
  region text is parsed with a PRELUDE prepended: `#include
  "latina.h"`, and for `methodus=` regions also `#include
  "internuntius.h"` + `hic_manens InternuntiusTractator
  briar_tractator_exemplar;` — the exemplar against which the fabrica
  checks a handler's type. Thistle line = `linea_initium +
  linea_silvae − praeludium − 1` (`briar_nexus_linea_silvae`). Headers
  reach silva by text through `silex_clausuram_e_contentis` over the
  prelude+region text. The first ERROR node's line (found by walking
  VALUES — a broken parse's commit root is a LIST, not a node) becomes
  `linea_erroris`; the fabrica refuses such a region.
  **Derived includes (v1.4, 2026-09-05, house headers only — Fran):**
  the nexus parses each region TWICE. Pass one with the prelude alone
  (`latina.h` + the implicit `stdio/stdlib/string` trio); silva's
  symbol table then names every function or function-like macro
  called without a declaration (`est_implicitum`) and every unknown
  named type (diagnostic `TYPUS_NOMINATUS_IGNOTUS`); each name is
  looked up in `corpus.symbola.tsv` (symbol, kind, header — the
  `include/*.h` rows of silva's identifier index, baked into the
  corpus by `tools/corpus_infixum.sh`); the headers found, sorted,
  are prepended to the prelude and pass two runs. The fabrica writes
  them as the first `#include` lines of the generated header (no
  `#line`: they are briar's) and of the probatio unit, and feeds them
  to the closure. A symbol declared in two headers (two exist:
  `Capitulum`, `Liber`) is a refusal naming both, unless the script
  includes one itself. An object-like macro or enum constant used as
  a bare value with no function from its header nearby is NOT
  derived (silva raises no unknown-identifier diagnostic for it) —
  clang names the line; the C library beyond the trio stays explicit
  (`math.h`, `time.h` are the first candidates for a hand map, §9).

### 3.4 Contextus — fragments and transclusion (v1.6, 2026-09-05)

The literate tangle: prose between pieces of one function, the pieces
named and assembled by reference. Decided with Fran 2026-09-05, each
alternative reserved rather than dropped:

| decision | chosen | reserved |
|---|---|---|
| D1 how a chunk is named | `<c! id="x">` on the region tag (no grammar change; the tag is already the annotation) | `<#x>…</#>` wrapping several parts at column 0, if a chunk ever needs its own prose (a nesting tree: arbor, canon, projection, nexus, fabrica) |
| D2 where a reference may stand | alone on its line, indentation carried (noweb's rule) | mid-line `<<#x>>`, decided together with the template call `<<#@x p="v">>` (§9) |
| D3 a second definition of the same id | refusal naming both lines | continuation with an explicit spelling, e.g. `pars="2"` |
| D4 which regions take part | C only: chunks are `<c!>` regions, references live in `<c!>` regions, the probatio included | html/js/css chunks (the resolver takes a region, not a language; widening is a fabrica table entry) |
| D5 where the weave runs | a stage between nexus and silva (below) | references as tokens of the outer tree, when the `briar-c89` dialect puts tags inside C generally (§9); the resolver and the line table survive that move |

**Words.** A region with `id=` is a *fragmentum*; one without is a
*radix* (app root or probatio root); a `<<#x>>` line is a
*transclusio*; the woven text of a root is its *contextus*; the
thistle line of each woven line lives in its *tabula linearum*.

**Pipeline.** arbor → nexus → **contextus** → silva → fabrica. One
call, `briar_contexere(piscina, nexus)`, after `briar_nexus_texere`
and before `briar_silvam_texere`: it classifies every `<c!>` region,
weaves every root, and records a refusal on the region record as
silva does (`causa` + `linea_erroris`), so the fabrica's inventory
refuses with the thistle line unchanged. Why not the fabrica: a root
holding `<<#x>>` lines is not C, silva refuses it, and derivation,
partition and prototypes all read silva's tree — the weave must run
before silva sees the text.

**Classification.** `id="x"`: non-empty, no whitespace. A fragment
carrying `methodus=` or `munus=` is refused (a chunk is never a
root). Fragments are document-global: any root may reference any
fragment; fragments may reference fragments.

**Weaving.** Depth-first over reference lines. A reference line
yields no line; in its place come the fragment's lines, each prefixed
with the reference line's leading whitespace bytes (blank lines left
blank). A fragment used twice appears twice — the alias has two
windows, and the tangled text is what the compiler sees. Every woven
line records its thistle line, root line or fragment line. The nexus
record gains `contextus` (chorda) and `lineae` (Xar of i32, one per
woven line); a root without references gets the identity table, so
every consumer reads one shape. The smallest example:

    <c! id="summa">
    summa = summa + numeri[i];
    </c>

    The loop body is the fragment above; the loop itself:

    <c!>
    i32
    summare (constans i32* numeri, i32 n)
    {
        i32 summa = ZEPHYRUM;
        i32 i;

        per (i = ZEPHYRUM; i < n; i++)
        {
            <<#summa>>
        }
        redde summa;
    }
    </c>

The root's contextus is the loop with `        summa = summa +
numeri[i];` in place of the reference line (eight spaces carried);
that woven line maps to the fragment's thistle line, every other line
to its own, and the generated `_regiones.c` shows two `#line` runs
inside `summare`.

**Refusals**, each with its thistle line:
`transclusio '#x' (linea N): fragmentum non definitum` ·
`transclusio '#x' (linea N): circulus #a -> #b -> #a` ·
`fragmentum '#x' iteratum (prima linea M)` at the second definition ·
`fragmentum '#x' (linea N): methodus/munus in fragmento - fragmentum
radix non est` ·
`transclusio malformata (linea N): '<<#x>>' sola in linea sua
exspectata` ·
`fragmentum sine id valido (linea N)`.
An unused fragment is no error; `-partes` lists it `non adhibitum`.

**Downstream.** silva parses the contextus (prelude + woven text);
`briar_nexus_linea_silvae` becomes a table lookup. In the fabrica,
directive collection walks the woven text through the table; unit
emission goes line by line with a `#line` at every run break — inside
function bodies too (silva never sees a `#line`: the fabrica adds them
after the parse); the probatio unit is emitted the same way. The closure reads woven texts,
so a fragment's `#include` lines reach the closure through the roots
that use it, and an unused fragment contributes nothing. Derived
includes (pass one/two over the woven text) and `-amalgama` need no
change. `-partes` prints every fragment: id, definition line, use
lines or `non adhibitum`; `-arbor` needs nothing (the open tag shows
`id`); cross-reference rendering belongs to `-html` (§9).

**The truth of `#line` is a fumus stage** (§5): a fixture whose
fragment holds a deliberate type error must make clang name the
fragment's thistle line. Nothing automated checked that before v1.6.

**As built (plan 4, 2026-09-05).** Unit `briar_contextus`
(`briar_contexere(piscina, nexus, &fragmenta)`,
`briar_contextus_lineam_legere`), gate `probatio_briar_contextus`
(100 assertions, golden `fixa/contextus/fragmenta.contextus`); the
fabrica emits every unit, the main and the probatio through one
mapped emitter; fumus stages VIII (run, probatio, `-partes`) and IX
(`#line` truth) green. Two findings on the way: (1) the derivation
exclusion set of 2026-09-05 (a name any region declares is never
derived) wrongly counted the PROBATIO's own includes — they never
reach the generated header — so a root could miss `piscina.h` when
the probatio included `credo.h`; the probatio is now excluded from
that set. (2) A script whose closure holds NO house library gets a
plain `aedificare.sh` whose `lib/*.c` glob matches nothing (silex's
own projects always have libraries); pure-libc scripts do not build —
ledger quaestio, fix = explicit file lists in the plain ordo as the
vitrea ordo already does. Numbers: the fragmenta amalgam is 6,006
lines; the contextus gate runs in 0.2 s. **(2) fixed the same day,
together with its twin:** `salutatio.thistle` (a plain program over
the native window library) failed to link because the plain script's
`lib/*.c` glob never picked up `lib/fenestra_macos.m`. silex's two
plain generators now take a closure (§4.4): NIHIL keeps silex's own
text byte for byte; briar's closure yields explicit `lib/*.c` and
`lib/*.m` entries and the Cocoa/Security/WebKit triple whenever a
`.m` is present (over-linking is harmless; deriving the exact
frameworks stays Fabrica's reservation, 01KZYN4VPZ). briar's plain
script goldens became structural as the vitrea ones already were;
fixture `fenestra_nativa.thistle`; fumus stage X builds salutatio
without opening it, and stage IX's fixture is pure libc again. Two
more holes fell on the way to that link: (a) silex's closure knew one
implementation file per header (`X.c`, else the `_macos.m` /
`_posix.c` / `_impl.c` suffixes, FIRST match) while `fenestra.h` is
implemented by `fenestra_macos.m` AND `fenestra_textus.c` — the
header declares the second with the house annotation `/* <aedilis
corpus="lib/fenestra_textus.c"/> */`, which aedilis reads and silex
now reads too (after the conventional twin, deduplicated by path:
`motus.h` annotates its own twin); (b) `lib/fons.c` and `lib/utf8.c`
included their headers as `"../include/x.h"`, which the scanner turned
into `include/../include/x.h` and the scaffold could not write — the
two sources now include `"x.h"` and the scanner strips the prefix. The
ludus session had named (a) and the `.m` hole in its commit 8d5c85a9;
its third, "a function used as a VALUE is not derived", stays open
(silva raises no implicit-symbol mark for a name that is not called;
§9 with the bare-macro case).

## 4. The fabrica — tree to binary

### 4.1 Build home and cache

`~/.rhubarb/briar/<titulus>-<sigillum>/`, following the
`~/.rhubarb/<app>.volumen` convention. `sigillum` = the first sixteen
hex of SHA-256 (`sigillum_computare`, exists — sigillum.h) over three
inputs, in order: the corpus stamp (`corpus.versio`), the EXACT flag
string the fabrica will write into the build script (review item 3:
flags baked into a project must be in its key; the corpus stamp
covers them only through the binary that carries them), and the file
bytes. Presence of `bin/<titulus>` under a directory with that key
means NO rebuild — content decides, never timestamps. `briar -struere
-iterum` forces. Subdirectories: `fontes/ assets/ instrumenta/
include/ lib/ vendor/ probationes/ build/ bin/` — exactly the silex
`-vitrea` scaffold, so a briar project dir is a silex project a human
can `cd` into and read.

**Stamp, as built (plan 3, 2026-09-05):** with the EMBEDDED corpus
the stamp is `corpus.versio` and the key is computed from bytes
before any parse (a hit execs in ~9 ms). With a DISK corpus (briar run
inside a rhubarb tree, or `-f`) the fabrica runs first and the stamp
is `clausura:<SHA-256 of every closure file's bytes, in closure
order>` — the key moves exactly when a file that goes into the project
changes; the house `filum` has no mtime accessor, and a content hash
is both cheaper and more honest than an mtime walk. `briar -versio`
marks a disk corpus `(discus)`.

**Flags, direction (Fran, 2026-09-04): DERIVE from the sources.** In
v1 the base flag set is the string silex's generators carry
(`lib/silex.c:741`, a literal). The structural fix is named twice on
the ledger and briar is its trigger: desideratum 01KZP0WDN9
("vexilla as data" — its stated trigger is a FOURTH consumer of the
flag set; compile_tests, aedilis, silex's generators were three,
briar's fabrica is the fourth), and the fabrica thread 01KZYN4VPZ
(frameworks derived from `#import` in the closure's `.m` files).
The engine for the derived half exists — `aedilis_derivare` +
`aedilis_scriptum_scribere` behind the `AedilisExtractor` seam
(exists — aedilis.h:38/143/178), whose rule file `aedilis.stml` already
carries per-header `-framework` rules and vendor rules — but that
rule file is NOT in the corpus bundle today (only `aedilis.canon` is),
and the extractor would have to read from the capsula. Both are named
in §9; v1 does not wait for them.

### 4.2 What is written

| output | from | note |
|---|---|---|
| `fontes/<t>.c` | generated main (§4.3) or, for a plain program, the unit that defines `principale`, cut at its silva extent, with `#include "latina.h"` and `#include "<t>_regiones.h"` prepended | every unit is preceded by `#line <n> "<via>"`; clang then reports `x.thistle:15:11` (measured 2026-09-04) |
| `fontes/<t>_regiones.c`, `include/<t>_regiones.h` | **Partition, as built (plan 2):** the header = include guard, `latina.h`, the implicit standard trio (`stdio.h`, `stdlib.h`, `string.h` — thistle files are scripts), every directive line of every non-probatio C region (`#include`, `#define`, `\` continuations) in document order, every top-level unit that declares no file-scope object (typedefs, struct/union/enum tags, prototypes), and one prototype per function definition (the definition's head up to its body + `;`); the `.c` = `latina.h`, the header, every file-scope object and every function definition except `principale`. Every unit and directive is preceded by `#line <thistle line> "<via>"`; since v1.6 (§3.4) a unit is emitted line by line and a further `#line` opens every run break the tabula linearum shows, so a woven fragment inside a function body maps to its own thistle lines. The header lives in `include/` so the probatio unit sees it through `-Iinclude`. Comments BETWEEN top-level units are dropped (extents cover tokens); comments inside units stay. A file-scope object is private to `_regiones.c` (share through functions). Probatio helpers must be `interior` (`-Wmissing-prototypes`). | one object shared by the program and the probatio |
| `probationes/probatio_<t>.c` | `#include "latina.h"` + `#include "<t>_regiones.h"` + the `munus="probatio"` region, `#line`-mapped | a SEPARATE translation unit linked against the library objects and `_regiones.o`; the file's helpers are visible through the generated header; no second `main`. Built to `bin/probatio_<t>` only by `-probatio` |
| `assets/index.html`, `assets/<t>.js`, `assets/<t>.css`, `assets/<t>.toml` | the html/js/css regions | v1: at most one region of each kind; more = refusal naming the second. A `<script src="<t>.js">` line is NOT injected — the html region is verbatim; the fixture references its assets itself |
| `instrumenta/capsula_generare.c` | corpus | as silex `-vitrea` |
| `include/ lib/ vendor/` | `silex_clausuram_colligere` (exists — silex.h:105) | seeds = the `#include "x.h"` lines of ALL C regions (probatio included — it pulls `credo.h`) ∪ the generated main's includes; `.m` twins and vendor pairs come with the closure |
| `aedificare.sh`, `probare.sh` | silex's generators (§4.4) | four-tier build script; probare = second link target with its own closure, as the scaffold's |

Prose regions produce nothing in v1; `briar -html` (the literate
rendering through `md_html_reddere`, exists) is a named pull, not a
flag.

### 4.3 The main rule (mirrors officina's `#!` rule)

The probatio region is its own translation unit and takes no part in
this rule: it may define its own `principale`, and the "two mains"
refusal counts only non-probatio C regions.

**How briar sees `principale`:** through the silva tree of §3.3 — a
top-level function definition whose declarator name is `main` after
expansion. Comments, strings, and prototypes do not count. Two such
units = the two-mains refusal, naming both `.thistle` lines.

**Method signatures are checked, not trusted.** A `methodus="nomen"`
region must define a function `nomen`; its type is compared with the
pointee type of the prelude's `briar_tractator_exemplar` through
`silva_c89_typi_compatibiles` (structural, never a rendered string);
a missing definition or a mismatch is a refusal naming the tag line
and the expected signature, before clang ever runs.

1. A non-probatio C region defines `principale` → plain program.
   briar adds only latina.h and the derived closure; `<fenestra>`
   present alongside is a refusal (two mains).
2. No `principale` and a `<fenestra>` element → atrium app. briar
   generates the main from the silex `-vitrea` template
   (`_fontem_vitreum_fingere`, lib/silex.c:1025, exists): piscina,
   `AtriumConfiguratio` from `<fenestra>` (titulus, latitudo,
   altitudo, capsula), `atrium_vexilla_legere` so `-vivum` and
   `bin/manus` work at birth, one `internuntius_praebere(inx,
   "<nomen>", <nomen>, NIHIL)` per `<c! methodus="nomen">`, the
   atrium loop. briar generates the method's prototype. User datum is
   NIHIL in v1; the volumen-bearing `Pipa` of the scaffold is deferred
   (§9, `status`).
3. Neither → refusal naming both absences.

### 4.4 silex changes (the one library touch)

**As built (plan 2, 2026-09-05, commit 4cc10d97):** five statics of
`lib/silex.c` are public in `silex.h`: `silex_clausuram_e_contentis`
(closure seeded from source TEXTS, signature unchanged) and the four
script generators `silex_ordinem_fingere`,
`silex_ordinem_probandi_fingere`, `silex_ordinem_vitreum_fingere`,
`silex_ordinem_probandi_vitreum_fingere`, each gaining a SOURCE-LIST
parameter (`constans character* constans* fontes, i32 numerus`) —
silex passes `fontes/<t>.c [+ <t>_pipa.c]`, briar `fontes/<t>.c
fontes/<t>_regiones.c`. Output for silex's own calls is byte-identical
(the four scripts of the silex gate's area compared with `cmp` before
and after). `_toml_fingere` STAYS static: briar's asset list is data
(which of html/js/css exist), so briar writes its own six-line toml.
The three flag macros `SILEX_VEXILLA_COMPILATIONIS` / `_VITREA` /
`_VENDITORIA` moved to `silex.h` (the cache key hashes them).
`lib/silex.c` stays silva-free (decree 01M098M3G6's route);
`probatio_silex` unchanged. The rule of two applied.

**Where silva enters briar:** `briar/fontes/` is a subsystem, not
`lib/`, so `briar_nexus` and `briar_fabrica` may depend on silva
directly, as officina's instrumenta do. The binary links silva's
objects from the same pool `silex_struere.sh` and `differre.sh`
share (`silva/build/`, flags + `-Wno-overlength-strings`), or the
verified amalgam `silva/amalgama/silva.c` — P3 picks by build time;
either is the decree's route.

### 4.5 Run

`briar x.thistle a b` = parse → key → build if absent
(`processus_exsequi` on `./aedificare.sh` with a deadline, exists —
processus.h:92; clang's output passes through) →
`processus_transformare` (exists — :113) into `bin/<t> a b`. The
script BECOMES the program: same PID, stdio inherited, working
directory unchanged, so relative paths resolve from where the user
ran it. A build failure prints the script's output and exits 1
without exec. `-probatio` builds `bin/probatio_<t>` (running
`probare.sh`) if absent and execs it the same way; its exit code is
the verdict.

### 4.6 Facies — the literate rendering (`-html`, v1.7, 2026-09-05)

The fabrica's twin, turned the other way. `-amalgama` flattens a
thistle into one `.c` for the compiler; `-html` unfolds it into one
page for the reader. Both consume the same `BriarFabricaFructus` in
memory, neither writes a project or runs clang.

The design rests on an observation about what briar already holds
after a normal run. Every feature below is a projection of computed
data, not new analysis: the fragment graph and its use sites are
`BriarFragmentum` (`briar_contextus.h`); the woven text and the
thistle line of each of its lines are `contextus` and `lineae`
(`briar_nexus.h`); the classification of every C token is
`silva_lexare_cruda` (silva.h), which briar already links; and the
declaration of every library symbol a region calls is already in that
region's `SilvaSemantica`, because the closure's header text is
parsed to type-check the region — the same table `_methodum_probare`
walks in `briar_fabrica.c` to find `briar_tractator_exemplar`, a
symbol that exists only because `internuntius.h` was parsed. The
scope discipline follows from this: **v1 renders what briar already
knows and adds no new analysis.**

Decided with Fran 2026-09-05, each alternative reserved rather than
dropped:

| decision | chosen | reserved |
|---|---|---|
| F1 artifact or display | a self-contained `x.html` first; the vitrea viewer as a second step over the same three chrome files | briar linking vitrea in-process (taxes every headless run of a build tool with Cocoa and WebKit) |
| F2 the page's spine | the document is the spine: one column in file order, detail arrives beside the text and is dismissed | two fixed panes; a tabbed workspace (both invite the reader out of a document whose argument arrives in sequence) |
| F3 where the markup lives | three chrome files embedded verbatim (`briar/facies/facies.{html,css,js}`), a generated body, a JSON island | an STML template program (md_html's shape — macro expansion to move a div); HTML built in C string literals (speculum's scar: "JS-in-C-strings… Latin leaks in") |
| F4 a file that does not build | always render; each refusal pinned at its thistle line | render only what fabricates; a vitia strip as the primary way to read briar's diagnostics (arrives on its own if wanted) |
| F5 symbol depth | the file's own symbols, plus the declaration of every library symbol it uses | library **definitions** — no `lib/*.c` is parsed today, and eagerly parsing a closure to answer clicks nobody makes is the wrong trade; this is what step 2 is for (§9) |
| F6 raw HTML in prose (2026-09-11) | **escaped**: prose HTML is text, through a briar-owned projection `briar/facies/md-html-facies.stml` differing from md's in exactly three `CASUS` arms (a drift gate holds that) | markdown's passthrough. Rejected because the format ALREADY refuses block-level HTML at column 0 (the lexer reads it as an STML element with a vitium), so passthrough would accept inline HTML while refusing block HTML — an accident of two layers meeting, not a rule. Escaping gives one statable rule: HTML in prose is text, HTML lives in regions. It is also the reversible direction — loosening later is free, tightening later breaks files already written |

**Words.** The rendered page is the *facies* — the word house prose
already uses for a front end (the laboratorium's experiment
commentary). `pagina` was rejected (taken: `lib/pagina.c`) and
`visio` was rejected (in this repository it names a vision document,
not a view). `lectio` was the runner-up and shares a root with
`legere`, which means parsing everywhere else in the tree.

**Pipeline.** arbor → nexus → contextus → silva → fabrica → **facies**.
One call:

    nomen structura {
        chorda involucrum;  /* facies.html, with its markers */
        chorda styli;       /* facies.css */
        chorda scriptum;    /* facies.js */
        chorda exemplar;    /* md/html/md-html.stml */
    } BriarVestis;

    chorda
    briar_faciem_fingere (
                              Piscina* piscina,
                  InternamentumChorda* intern,
                                 Xar*  nexus,      /* BriarNexusRes */
                                 Xar*  fragmenta,  /* BriarFragmentum */
        constans BriarFabricaFructus*  fructus,
                               chorda  fons,
                  constans character*  via,
                 constans BriarVestis* vestis,
                               chorda* causa);

Structures in, one string out: no I/O, no globals, so the whole
module is testable against goldens without a filesystem. The chrome
arrives as a parameter rather than being read inside, which is what
lets the gate feed fixed test chrome: editing the real CSS then
cannot move a byte golden.

**The chrome.** `briar/facies/facies.html`, `.css` and `.js` are real
files in the repository, embedded by `bin/capsula_generare` through
`tools/briar_facies_capsula.sh` into `build/capsula_facies_briar.c`
(uncompressed; a few KB), linked by `briar_struere.sh` beside the
corpus object. The consumer declares `externus constans CapsulaEmbed
capsula_facies_briar;` directly and never includes the generated
header — speculum's recorded rule, because silva cannot resolve a
quoted include containing `..` (board nota 01KY0T6T64). briar's C
therefore contains no CSS and no JavaScript; it splices files it
carries with a body it generates.

**The law of the page: it reads with JavaScript disabled.** Every
structural affordance is markup and anchors. This is what makes the
artifact publishable, greppable and archivable, and it holds the
hand-written JavaScript down to one feature.

- **Every line is addressable.** Prose paragraphs and code lines
  carry `id="l<N>"`, N the thistle line. `x.html#l117` is an address,
  so briar's diagnostics — which always name a line — become links.
- **A fragment definition** carries its use sites as markup, straight
  from `BriarFragmentum.usus`: `#fundamentum · adhibitum: 117`, each
  a link. An unused fragment says `non adhibitum`, the same judgement
  `-partes` prints.
- **A transclusion line** is a link to the definition. Both
  directions of the fragment graph are therefore plain anchors.
- **The composed view is a `<details>`.** Each root emits its
  as-written text, then its `contextus` collapsed behind a
  `<summary>`. Native disclosure: no toggle to write, no JavaScript.
  The woven view's gutter shows **thistle** line numbers, not
  composed ones, each linking home — `lineae` makes this exact. That
  gutter is the view's purpose: the code the compiler sees, every
  line naming the paragraph it came from.
- **Highlighting** is `silva_lexare_cruda` per region, one `<span>`
  per classified token. The policy — which lexeme kind takes which
  class, and the three tables that make `si`/`redde`/`per` keywords
  and `i32`/`chorda` types although the lexer sees identifiers — is
  carried from laboratorium experiment 0023 (`_classis`,
  `_claves_latinae`, `_genera_nota`, `_constantia_nota`), with its
  six themeable classes. Its principle stands: *lexare mechanica,
  colorare consilium*; TextMate-style regular-expression grammars are
  deliberately not used, because there is a real lexer. Its
  provenance warning stands too: this is vendored from silva, so a
  new lexeme kind wants a look at the table. The **transport** is not
  borrowed — 0023 emits per-line byte-range covers over a bridge for
  JavaScript to paint; the static page emits the spans itself. The
  cover model returns in step 2.
- **Symbols.** A use of a name the file defines is an `<a>` to its
  definition line — no JavaScript. A library symbol carries
  `data-s="…"`, and one JSON island holds only what this file uses:
  header and declaration, from the region's own symbol table.
- **The only JavaScript is the declaration panel** — click a
  `[data-s]`, look it up, show it beside the text, dismiss. That is
  the whole dynamic surface of v1.
- **Refusals** are `<aside>` annotations in the margin at their line,
  with the line itself marked. A region that failed to weave shows
  its as-written text and, where the `<details>` would stand, the
  reason.
- **Head matter** carries provenance: source path, briar version,
  corpus stamp, generation time — so a page found later says what
  made it.
- **The foot** carries the `-partes` inventory: fragments, derived
  includes, closure.

**Escaping is a gated concern, not an assumption.** Prose contains
`<script>` (a fenced example in `salve_vitreum.thistle` does), C
contains `<`, `&` and `"` constantly, and the island must escape
`</script`. Byte-exact slicing keeps UTF-8 intact: experiment 0023's
fixture carries a deliberate `/* æ */` for exactly this reason
(decree 01M0ATF1E1 — C emits byte columns).

**The flag.** `-html` writes `x.html` beside the thistle and prints
the path, as `-amalgama` writes `x.c`. It opens nothing: the fumus
gate must run it without a window appearing, and the shebang form
reads only the first flag after the file (`briar_imperium.h`), so
`./x.thistle -html -aperire` could not work. Opening is `open
x.html`; in step 2 a second verb, `-visio`, writes and shows.

**As built (T1–T3, 2026-09-05/11), deviations each measured:** head
matter carries no generation time — a byte golden cannot hold a clock,
so provenance is the source path plus the fabrica's content key, which
identifies the content better than a timestamp would · the island
carries the symbols briar **derived**, not every library symbol: those
are exactly the names with no `#include` line for the reader to look
at, the cost `salutatio2.thistle` pays back by hand in a table it warns
may drift; a symbol from an explicitly included header gets nothing,
because the signal is already three lines up · the `(symbol → header)`
pairing comes from `briar_silva`, which already computed and discarded
it (`BriarSymbolumDerivatum`) · the gutter is `data-n` drawn by CSS
`::before`, so what a reader copies is code, not code-with-line-numbers;
woven lines carry no `id` (they would collide with the as-written ones)
and their number is an EMPTY anchor — copy stays clean, link still
works without JavaScript · **the fabrica's cause is a FALLBACK**: when
any region carries its own cause the regions own the diagnostics —
found by a planted fault that reddened only ONE assertion because the
page-level aside was duplicating the region-level one · five refusal
fixtures are asserted against briar's OWN `causa` and `linea_erroris`
rather than pinned as goldens (less brittle, and it tests the
relationship rather than the bytes); `circulus.html` is pinned as the
one representative.

**Gates**, in two layers that fail for different reasons.
*Structural invariants*, checked over a real lex of the page with
`html_lexema` (already linked), robust against restyling: every
`id="l<N>"` unique; every `href="#frag-x"` resolving to an emitted
`id="frag-x"`; every `data-s` key present in the island; the count of
`<details>` equal to the number of roots. *Byte goldens* under
`fixa/facies/`, regenerated only with `BRIAR_FACIES_SCRIBERE=1` and a
named cause, over `salve.thistle` and `fragmenta.thistle` — the
second is line-pinned, so its use sites are law. The six `adversa/`
fixtures each get a page whose cause is anchored at the right line
(F4 made testable). Two new adversarial fixtures: escaping (`<script>`,
`&`, `"`, a literal `</script` in prose and in code) and UTF-8 (a
non-ASCII character inside a C region, asserting its bytes land
inside one span). **The assertion of record: the page and `-partes`
agree** — same fragments, same use sites, same derived includes; one
truth rendered twice, and a divergence names which is lying. Born red
by a planted fault in the line table or the escaping.

**Fumus stage XI:** `-html` over `project-specs/exempla/salutatio2.thistle`
— the file written, `id="frag-principale"` present, and
`#repositorium` reporting **both** use sites, since that fragment is
deliberately shared between the program and its probatio. Then
`-html` over a refusing fixture: the page still written, the cause
named. Counts asserted, never absences.

**Risk.** Goldens over generated markup are brittle, and a red golden
invites regeneration. The house rule holds — never without an audit
and a named cause — and the two-layer split is the mitigation: a
class rename may redden the byte goldens and must never redden the
invariants.

### 4.7 Spectator — the viewer (`-visio`, v1.8, 2026-09-11)

Step 2 of §4.6's F1. `-html` writes an artifact; `-visio` opens a
window on it. The window has to earn its existence: a browser already
has find, zoom, print and bookmarks, so a viewer that only *shows* the
page is a worse browser. What it adds is **the bridge** — a live C side
answering what a static page cannot, which is F5's reserved half:
click a library symbol, see its definition.

Decided with Fran 2026-09-11:

| decision | chosen | reserved |
|---|---|---|
| S1 where the intelligence lives | a C tool on silex's shape (`tools/briar_spectator.c`): vitrea + fenestra + briar's modules + the corpus, ~20 MB (Fran: the size is fine) | a **`.thistle`** viewer — briar building its own reader. MEASURED and rejected: the corpus is `lib/*.c` + `include/*.h` + vendor, so `briar/fontes/` is unreachable from a thistle; and the blocker is not code but the corpus **as runtime data** — `silex_fons_corporis` needs a `CapsulaEmbed*` a generated app does not link, so fixing it makes EVERY vitrea thistle app carry the 10 MB corpus · a **thin viewer forwarding to `briar` as a subprocess**: one brain, ~3 MB, but a process per click and no cached parse |
| S2 v1 scope | (a) show the page, then (b) the bridge — staged, both in v1 | (c) live reload on change, a fragment-tree sidebar, corpus-wide search — after Fran has used (b) |
| S3 how the page reaches the window | atrium's `capsula_radix`: assets from a DIRECTORY, same `capsula://` origin, the page cannot tell. The facies page is self-contained, so the directory holds exactly ONE file | opening a disk capsula by hand (atrium already does it); assembling the page in JavaScript (breaks §4.6's law that the page reads with JS off) |
| S4 one chrome or two | ONE: `facies.js` degrades bridge → island → nothing, so the same three files serve spectator, browser, and a page with scripting off | a second front end for the viewer — which is exactly what putting the chrome in files (F3) was meant to avoid |

**Words.** The viewer is the *spectator* — one who watches, from
*specto*. `visio` stays the flag (a seeing); in this repository the
bare word names a vision document, so it is not a binary's name.

**Pipeline.** The spectator is briar plus a window; it repeats no
rendering logic:

    fabricare -> briar_faciem_fingere -> <t>.html in <domus>/facies/
              -> atrium (capsula_radix = illud directorium,
                         via_initialis = "<t>.html")
              -> fenestra + vitrea + internuntius

`briar -visio x.thistle` execs `briar-spectator` as `-probatio` execs
the built binary; `briar-spectator x.thistle` works directly. briar
stays headless — if the binary is absent, `-visio` refuses BY NAME and
says which script builds it, and never opens a window itself.

**The page in the window is the page on disk, byte for byte.** One
artifact, two ways to look at it; a divergence is a bug with an
obvious test (below). It also means §4.6's law holds inside the
window: the document loads as a document, not as something JavaScript
assembles.

**The bridge** (stage b), one method:

    internuntius.vocare('facies.symbolum', {titulus: "..."})
      -> {caput, typus, definitio, via, linea} | {absens: verum}

The handler takes the header from the island data the page already
carries, finds its twin by silex's rule (`piscina.h` → `lib/piscina.c`),
reads it **from the corpus** — never from disk, so it works outside any
repository — parses it once with silva, caches the parse per file, and
returns the definition's extent verbatim. The second click anywhere in
that file is free, which is the whole argument for carrying the corpus.

**Gates.** The deliverable is a window; almost none of the work needs
one.

- The renderer is gated already (§4.6, T1–T3), unchanged.
- **The handler is a pure function** over the corpus — title in,
  definition out — so it is gated headless and hard: the twin rule, a
  symbol whose header has no twin, a symbol absent from the corpus, a
  symbol declared but not defined, and the cache returning identical
  bytes twice.
- **Byte identity**: the page the spectator writes equals the page
  `-html` writes for the same file. Two paths, one artifact — the
  cheapest possible guard against silent divergence.
- The window is `-agere`, but NOT only by hand: an atrium app exposes a
  driver port and the fumus already drives one (stage IV reads
  `atrium_portus` and uses `bin/manus` to list affordances, press by
  text, and read `textus body`). Stage XII under `-agere`: open
  `salutatio2.thistle`, assert the body carries `frag-principale` (the
  page loaded), press a `data-s` symbol, assert the body then carries
  its header (the bridge answered).

So everything except "a human looked at it and it was good" is
gateable, and that part stays Fran's.

**Reserved** beyond S1–S4: speculum's `fontes` capsula in the spectator
(Cmd+Shift+D over briar's own source — the scaffold field exists);
`-radix` as a live dev mode for the chrome, which atrium gives free.

### 4.8 Fasciculus — the `.app` (`-app`, v1.9, 2026-09-12)

A thistle that opens a window should be able to live in the Dock. `-app`
turns the program briar already builds into a double-clickable bundle
`x.app` beside the thistle, with an icon. Almost nothing is new:
**fasciculum** writes bundles and **icones** writes `.icns` from pixels
(both sealed 2026-09-12), and briar already carries files inside its
binary (§4.6's capsula). The feature is the wiring, plus two small
changes those libraries lack and one element §2 promised.

Decided with Fran 2026-09-12:

| decision | chosen | reserved |
|---|---|---|
| A1 who decodes an image | briar: `stb_image` (through `lib/imago.c`) is linked into the briar BINARY. It stays out of briar's modules, which take an `Imago` exactly as icones does (icones D7), so no gate in the briar suite links a decoder | briar never decodes — the default baked as `.icns` at build time and `-icon` accepting only `.icns`, one more command for every custom icon |
| A2 what can be bundled | any program briar builds; no test for the vitrea form. It is useful for window apps; a command-line program launched from Finder simply runs without a terminal | refusing plain programs — a structural test briar could make, with nothing gained by making it |
| A3 where | `x.app` beside the thistle, as `-html` and `-amalgama` write | the working directory; an output flag |
| A4 identity | `org.rhubarb.briar.<t>`, `<t>` the file's name with `_` turned into `-`; overridden by `identitas="…"` on a `<briar>` element; its characters validated by fasciculum | an identity flag (the identity belongs to the program, not to one invocation); a path hash for uniqueness (it would change whenever the file moves) |
| A5 name and version | `CFBundleName` = `<fenestra titulus>`, else `<t>`; `CFBundleShortVersionString` = `<briar versio>`, omitted when absent | `CFBundleVersion`; a version flag |
| A6 an existing `x.app` | replaced only when it is briar's own — its `Info.plist` identity equals this one; anything else is refused by name and left untouched | writing over the old bundle in place (fasciculum never deletes, so stale files would survive); moving it to the Trash |
| A7 where the icon comes from | `-icon <via>`, relative to the working directory > `<briar icon="…">`, relative to the thistle > the embedded default, `default-thistle.jpg` | converting the default to PNG first: 1.6 MB against 386 KB, identical pixels once decoded |
| A8 Finder's icon cache | measured by hand in the fumus; a timestamp update only if a rebuilt bundle shows a stale icon | `lsregister -f` |

**Words.** A bundle is a *fasciculus*, a little bundle, as the library
is named; the module is `briar_fasciculum`. The flag stays `-app`: a
format name, like `-html`.

**The `<briar>` element, first built here.** §2 lists an optional
`<briar titulus="…">`. Measured 2026-09-12: nothing reads it — no module
looks for the element and no fixture uses one. This section builds the
element with three optional attributes, `identitas`, `versio` and
`icon`. `titulus` stays unbuilt: renaming a project renames its capsula
symbol and its header guard, a larger change than an app needs (§9). A
second `<briar>` element is a refusal naming both lines.

**Pipeline.** `-app` is a run that stops before exec:

    parse -> fabrica -> build <domus>/bin/<t> if absent (key of §4.1, unchanged)
          -> icon source (A7) -> decode (tools/briar.c, stb) -> Imago
          -> icones_reddere -> icones_icns_codificare -> <domus>/fasciculum/<t>.icns
          -> Fasciculum { identitas, titulus, versio,
                          exsecutabile = <domus>/bin/<t>, icon = that .icns }
          -> fasciculum_reddere
          -> <dir>/<t>.app exists? briar's own: remove it; otherwise refuse
          -> fasciculum_scribere(partes, "<dir>/<t>.app") -> print the path

A vitrea program carries its page inside the binary (its capsula), so
the bundle needs only `Contents/MacOS/<t>`, `Contents/Info.plist` and
`Contents/Resources/<t>.icns`. A thistle that does not fabricate refuses
`-app` with exit 1, as every action but `-html` does.

**One module, two calls.** Everything decidable without the disk is one
pure function; the writer takes pixels already decoded:

    nomen structura {
        chorda  identitas;   /* A4, characters validated */
        chorda  titulus;     /* A5 */
        chorda  versio;      /* A5; empty = omitted */
        chorda  via_icon;    /* A7 resolved; empty = the embedded default */
        chorda  via_app;     /* <dir of the thistle>/<t>.app */
           i32  linea_briar; /* the <briar> line, 0 when absent */
    } BriarFasciculumConsilium;

    b32 briar_fasciculum_consilium (Piscina*, Xar* nexus,
            constans character* via_thistle,
            constans character* via_icon_vexilli,
            BriarFasciculumConsilium*, chorda* causa, i32* linea_causae);

    b32 briar_fasciculum_scribere (Piscina*,
            constans BriarFasciculumConsilium*, constans Imago* icon,
            constans character* exsecutabile, constans character* domus,
            chorda* causa);

The consilium reads the nexus (`briar_nexus_attributum` over the
`<fenestra>` and `<briar>` elements, as the fabrica reads `<fenestra>`).
The writer's gate feeds a synthetic `Imago` and a stub executable into a
temporary directory: no decoder, no window.

**Changes to sealed libraries**, each its own commit with its own
planted fault:

- **fasciculum** — `fasciculum_reddere` refuses an identity containing a
  character outside `A–Z a–z 0–9 . -`, with a new status
  `FASCICULUM_ERROR_IDENTITAS` appended to the enumeration and the
  identity named. Today only an empty identity is refused, and a bad
  one fails silently inside Launch Services. `fasciculum_instrumentum`
  inherits the check.
- **filum** — a helper that removes a directory tree (name unsealed:
  `filum_arborem_delere`). It uses `lstat` and removes a symbolic link AS
  a link. It cannot be built on `directorium_ambulare`, which calls
  `stat()` and so walks through a link into the directory it points at
  (`lib/iter_directoria.c`, measured by reading 2026-09-12). Refuses
  NIHIL, the empty path and `/`.

**Embedding the default.** `default-thistle.jpg` moves from the
repository root to `briar/icon/default-thistle.jpg`, embedded by a new
`tools/briar_icon_capsula.sh` (the shape of `briar_facies_capsula.sh`)
into `build/capsula_icon_briar.c`, which `briar_struere.sh` compiles into
the link. `tools/briar.c` declares `externus constans CapsulaEmbed
capsula_icon_briar;` directly, by §4.6's rule. It is kept apart from the
facies capsula so the spectator does not carry an icon it never shows.
The default decodes with `imago_caricare_ex_memoria`, a user's file with
`imago_caricare_ex_file`.

**Linking.** `briar/compile_probationes.sh` adds `plist xml base64
fasciculum icones imago_opus imago_png` to its library list (`flatura`
is already there); `imago` is not added, so no briar gate links
`stb_image`. `briar_struere.sh` adds `build/imago.o` and the icon capsula.
The spectator's rule — take from `build/` only what `briar/build/`
lacks — picks up the new library objects without a change. Binary size
is recorded at P8 (briar is 10.7 MB before).

**Flags.** `-app` joins the action flags, before the file and as the
reserved first argument after it (`./x.thistle -app`). `-icon <via>` is
an option before the file, like `-f <radix>`; with any action but `-app`
it is a refusal, as is `-icon` with no path. The shebang form therefore
cannot pass `-icon`; the `icon` attribute is its way (A7).

**Gates.**

- `imperium`: `-app` before the file and as the first argument after
  it; `-icon <via>`; `-icon` without `-app` and without a path refused.
- **`fasciculum`** (new, `probatio_briar_fasciculum`). The consilium
  over fixtures: the default identity from a name containing `_`; the
  `identitas` override; an invalid identity refused at the `<briar>`
  line; two `<briar>` elements refused naming both; the name from
  `<fenestra titulus>` and from the file; `versio` present and absent;
  icon precedence flag > attribute (resolved against the thistle's
  directory) > default. The writer, over a synthetic 64 px `Imago` and a
  stub executable: the bundle read back with `fasciculum_legere`
  (identity, name, version, executable, icon); the `.icns` structurally
  valid; a second write REPLACES (a stale file planted in the old bundle
  is gone); a foreign bundle (another identity) refused and left byte
  for byte untouched.
- Root `fasciculum` and `filum` gates: the identity check, and the tree
  removal with nested directories, files, and a link to a directory
  OUTSIDE the tree whose contents must survive.
- **Fumus, headless:** the installed briar, from outside the repository
  with a fake `HOME`: `briar -app salve_vitreum.thistle` writes
  `salve_vitreum.app` beside it; `plutil` reads the identity
  `org.rhubarb.briar.salve-vitreum`; `iconutil -c iconset` extracts the
  bundle's `.icns`, asserted by file COUNT (iconutil returns 0 on broken
  containers, icones §12.3); `-icon` with a PNG fixture; a second `-app`
  replaces; a foreign `salve_vitreum.app` is refused.
- **Fumus, `-agere`:** `open salve_vitreum.app`, the window found and
  driven through `bin/manus`, Fran's look at the icon, then a changed
  icon rebuilt and looked at again (A8).

## 5. The binary and its build

- **Flags, not verbs (DECISUS, Fran 2026-09-04: thistle files are
  scripts).** `briar [-flag] [-f <radix>] x.thistle [args…]`; house
  single-dash form. **As built (plan 3):** the flags are hand-parsed
  in `briar_imperium` (gated, 53 assertions), not by `argumenta.h`:
  that parser refuses undeclared flags and cannot stop at the file,
  so a program's own `-foo` after the file would be refused. The
  declared-options spirit is kept — exactly `-probatio -struere
  [-iterum] -arbor -partes -amalgama -html -versio -auxilium/-h -f
  <radix>` are recognized before the file; any other `-x` there is a
  refusal (exit 2) naming them. One code path: the bare form is the run.

  | flag | does |
  |---|---|
  | (none) | run: build if absent, exec |
  | `-probatio` | build the probatio if absent, exec it (exit code = verdict) |
  | `-struere [-iterum]` | build only, print the project dir; `-iterum` ignores the cache |
  | `-arbor` | print the STML projection |
  | `-partes` | print the closure, ADEST/ABEST as `silex partes` |
  | `-amalgama` | write `<t>.c` (+ `probatio_<t>.c`) beside the thistle |
  | `-html` | write `<t>.html` beside the thistle (§4.6); opens nothing |
  | `-visio` | exec `briar-spectator` on the file (§4.7); refuses by name if absent |
  | `-app` | build if absent, then write `<t>.app` beside the thistle (§4.8) |
  | `-icon <via>` | option before the file, with `-app` only: the icon's source (§4.8 A7) |
  | `-versio` | corpus stamp and the flag-string hash of §4.1 |

- **`-amalgama` (BUILT 2026-09-05, `briar_amalgama`; design banked
  the same day, three deviations measured below):** writes beside the
  thistle ONE file `<t>.c` that compiles with clang alone — line 2 of
  its banner IS the compile line (`clang <flags> <t>.c -o <t>`; the
  fumus runs exactly that line) — plus `probatio_<t>.c` by the same
  rule when a probatio region exists (two programs cannot share a
  file; "single .c" holds per program, and the escape hatch dumps
  everything, no second flag). Not the normal path. Content, in
  order: (1) the closure's headers in DEPENDENCY order (depth-first
  over each header's `#include "…"` lines, post-order — a header
  follows everything it includes; `postulata_posix.h` FIRST when
  present, codex 85); (2) the generated regions header; (3) the
  library sources, each right after its own header's position
  (`lib/<stem>.c` follows `include/<stem>.h`; sources without a header
  twin in closure order), then `<t>_regiones.c`, then `<t>.c`. Every
  file opens with `#line 1 "<via>"` so a clang error names the
  original file; local `#include "…"` lines become EMPTY lines (line
  numbers kept; the scanner is silex's rule — first non-blank `#`,
  `include`, a quoted name ending `.h`). **Deviations from the banked
  design:** (a) system includes stay IN PLACE, never hoisted —
  `lib/filum.c` and `lib/machina_posix.c` carry platform-conditional
  ones (`<io.h>` under `_WIN32`, `<uuid/uuid.h>`); hoisting would pull
  them on every platform; guards make in-place harmless and
  `postulata_posix.h` first is the only order that matters. (b) EVERY
  file-scope static of every library source is renamed, not only the
  colliders — `#define name name_<stem>` before the file's text,
  `#undef` after, plus `#undef` of every macro the file defines —
  because a user region can collide with a library static too, and
  the collision analysis buys nothing. The lists are the identifier
  index's `lib/*.c` depth-0 rows (functio, variabile, typedef,
  constans, macro) whose name has NO `include/*.h` row: the index does
  not record linkage, and "no header row" = static, since a public
  function without a header prototype does not compile under
  `-Wmissing-prototypes`. 4,614 such rows sit in `corpus.symbola.tsv`
  (third column `lib/…`; `tools/corpus_infixum.sh`; the derivation
  reader skips them). Struct tags private to a `.c` are the one thing
  the index does not list — none collided in the fixtures. (c) v1
  REFUSES, naming the file: the vitrea form (§9), `vendor/` in the
  closure (sqlite does not compile under the strict flags), `.m` in
  the closure. **Gate `probatio_briar_amalgama`:** a SYNTHETIC fabrica
  (`fixa/amalgama/fabrica`: alpha/beta with a colliding static, a
  local typedef, a file-scope variable, a macro, `postulata_posix.h`,
  a trimmed `latina.h`; hand-written `corpus.symbola.tsv`) whose
  amalgam is the byte golden `fixa/amalgama/gamma.c` (compiled by hand
  at birth with its own line 2: prints `8`); the real fixtures salve,
  derivatum, punctum checked STRUCTURALLY (every closure file exactly
  once by `#line 1`, no local include left, every `#define A A_…`
  paired with as many `#undef A`, `postulata_posix.h` the first
  `#line`, probatio in its own file with `lib/credo.c`); both
  refusals; the writer overwrites only files that begin with its own
  banner. Fumus stage VII compiles salve's and derivatum's amalgams
  (both programs and both probationes) with the banner line and runs
  them: `salve.c` 5,974 lines, `derivatum.c` 5,988 (2026-09-05).
- **The shebang form** `./x.thistle …` reaches briar as `briar
  ./x.thistle …`, so briar also recognizes its own flags as the FIRST
  argument after the file: `./x.thistle -probatio` runs the probatio.
  That reserves those five words as a program's first argument; a
  program that needs one of them as its own first argument is called
  through `briar x.thistle -- -probatio`. Everything after is passed
  through untouched.
- `tools/briar.c`: corpus = `silex_fons_corporis(piscina,
  &capsula_corpus_silicis)` (exists — silex.h:60), with silex's
  resolution order (`-f` fabrica > ascent > embedded), so inside a
  rhubarb tree it reads the DISK, like `silex iudicare`, and anywhere
  else — the normal case for a script — it reads the embedded corpus.
- `tools/briar_struere.sh`, mirroring `silex_struere.sh`: the corpus
  regeneration block is EXTRACTED into `tools/corpus_infixum.sh` and
  sourced by both, so `build/capsula_corpus_silicis.c` is generated
  once and shared; links `build/*.o` + materia + md + briar objects +
  the corpus + Cocoa/WebKit/Security; installs `~/.bin/briar`. ORDER,
  as with silex: `./compile_tests.sh` first (it builds `build/*.o`),
  then struere.
- **Freshness gate at birth** (tool-reliability doctrine):
  `tools/briar_fumus.sh` — fake `HOME` (exported), the two fixtures of
  §2 through the INSTALLED corpus-infixum path, from a directory
  OUTSIDE the repository (the script case): the plain one built AND
  run (stdout must contain `salve, munde`), its probatio run green
  via `-probatio`; the vitrea one built and linked, its probatio run
  green (`-agere` additionally launches it with `-vivum` and drives
  `bin/manus`, as `silex_semen_fumus.sh -agere`). Planted fault at
  birth: a fixture whose probatio contains `CREDO_FALSUM(VERUM)` must
  turn the gate red. Exit 2 = nothing ran. Prerequisite noted in its
  header: a stale corpus lies green.

## 6. Probationes and gates

`briar/compile_probationes.sh` (exemplar: md's; exit 0/1/2, header
guard, per-test logs), registered in pythonica's four tables
(`PORTAE`, `FORMAE`, mensurae prefix, build-dir map — pythonica/silva.py
1449/1489/2197/2286). Every gate is born red by a planted fault.

| gate | proves |
|---|---|
| `registrum` | genera and loci by title; lexicon by title; seal pinned |
| `lexema` | every fixture byte-exact through the line table; the column-0 law; `<` not followed by a name-start is prose; **a column-0 `<name>` inside a markdown fence is prose**; fence open at EOF = vitium |
| `arbor` | regions found and bounded; unterminated raw → EOF + vitium; stray `</x>` is prose; STML element extent balanced; self-closing |
| `stml` | STRUCTURALIS + FIDELIS round trips of the projection over all fixtures |
| `canon` | drift guard both ways, seal, every fixture projection judged |
| `totalitas` | random bytes, mutations, truncations, deep nesting → never crashes (`CREDO_NON_RUIT`), always a tree, emission == source |
| `nexus` | prose → md tree; element → StmlNodus; raw open tag → attributes (`methodus`, `munus`); line offsets correct (a planted error on a known `.thistle` line) |
| `fabrica` | headless: tree → project inventory + generated main + generated `probationes/probatio_<t>.c` byte-compared to goldens; `#line` lines present; main rule's three arms with the probatio unit excluded; refusal texts named |
| `computus` | bench twin, golden `fixa/computus/basis.tsv` (`COMPUTUS_SCRIBERE=1` + a named cause) |
| `amalgama` | synthetic fabrica → byte golden `fixa/amalgama/gamma.c` (`BRIAR_AMALGAMA_SCRIBERE=1` + a named cause); real fixtures structurally (once per file, no local include, `#define`/`#undef` pairs, posix first, probatio separate); vitrea + vendor refusals; writer guard |
| `contextus` | `fragmenta.thistle` (nested fragments, indentation, one used twice, one used by the probatio, one carrying an `#include`): the woven text of each root byte-compared to a golden under `fixa/contextus/`, the line table pinned at the splices, the identity table for a root without references; five adversa fixtures, one per refusal, with lines; unused fragment listed; born red by dropping the indentation prefix. Fabrica gate adds the `fragmenta` golden directory (`#line` runs inside a function body and in the probatio unit); silva gate adds derivation of a symbol that lives in a fragment; fumus adds the run, the probatio, the amalgam, and the `#line`-truth stage of §3.4 |
| `facies` | §4.6, two layers: structural invariants over a real `html_lexema` lex (unique line ids, no dangling `#frag-` link, every `data-s` in the island, `<details>` count == roots); byte goldens `fixa/facies/` (`BRIAR_FACIES_SCRIBERE=1` + a named cause) over `salve.thistle` and the line-pinned `fragmenta.thistle`; the six `adversa/` fixtures each rendering with the cause at its line; new escaping and UTF-8 fixtures; **the page and `-partes` asserted to agree**. Born red by a planted fault in the line table or the escaping. Fumus adds stage XI over `salutatio2.thistle` |
| `spectator` | §4.7 HEADLESS: the `facies.symbolum` handler as a pure function over the corpus (twin rule; header with no twin; symbol absent; declared-not-defined; cache returns identical bytes twice), and the page the spectator writes byte-compared to the page `-html` writes. The window is fumus stage XII under `-agere`, driven through `atrium_portus` with `bin/manus` |
| `fasciculum` | §4.8: the consilium (identity default and override, invalid identity and a second `<briar>` refused at their lines, name, version, icon precedence) and the writer over a synthetic `Imago` (bundle read back, `.icns` valid, own bundle replaced with a stale file gone, foreign bundle refused and untouched); `imperium` gains `-app` and `-icon`; root `fasciculum` and `filum` gain the identity check and the tree removal |
| `probatio_silex` | UNCHANGED after §4.4 — the promotion is behavior-preserving |

Plus the end-to-end `tools/briar_fumus.sh` (§5), the only gate that
compiles and runs a generated project and its probatio.

## 7. Work inventory — modules (unsealed names)

New in `briar/`, each with a probatio and a `.worklog.md`:

| module | depends on | content |
|---|---|---|
| `briar_registrum` | materia_registrum | genera + loci tables |
| `briar_lexicon` | materia_lexicon | token table, `briar-` prefix |
| `briar_lexema` | materia_token, piscina | line table; column-0 classification; fence state |
| `briar_arbor` | briar_lexema, materia_nodus, stml (lexeme stream) | regions → tree; byte-exact emission |
| `briar_stml` | materia_arbor | consilium + origo hook; `briar.canon` beside it |
| `briar_nexus` | md_arbor, stml, silva (expansion parse fed from the capsula), briar_arbor | inner trees by identity — md, STML, and silva for C; offsets; attributes |
| `briar_fabrica` | silex (promoted API), silva (extents, `main` unit, type renderer), sigillum, filum, briar_nexus | tree → project dir; main rule; unit partition; prototypes; `#line`; assets; toml; probatio unit; method signature check; the key |
| `briar_computus` | briar_arbor | bench twin |
| `briar_contextus` (v1.6) | briar_nexus | fragments and transclusion: classification, the weave, the line table, refusals; between nexus and silva |
| `briar_facies` (v1.7) | briar_contextus, briar_fabrica, silva (`silva_lexare_cruda`), capsula | fructus → one self-contained HTML page; chrome from an embedded capsula; after the fabrica, a sibling of `briar_amalgama` |
| `briar_spectator` (v1.8) | briar_facies, silva, silex, atrium, vitrea, fenestra, capsula | `tools/briar_spectator.c` + `tools/briar_spectator_struere.sh`: the window and the bridge; repeats no rendering |
| `briar_fasciculum` (v1.9) | briar_nexus, fasciculum, icones, filum, via | the consilium (pure) and the writer over a decoded `Imago`; decoding stays in `tools/briar.c` |
| `tools/briar.c` | all above, argumenta, processus, capsula | the binary |
| `tools/briar_struere.sh`, `tools/corpus_infixum.sh`, `tools/briar_fumus.sh` | — | build, shared corpus block, freshness gate |

New in v1.7: `briar/facies/facies.{html,css,js}` (the chrome, embedded
verbatim) and `tools/briar_facies_capsula.sh`.

New in v1.9: `briar/icon/default-thistle.jpg` and
`tools/briar_icon_capsula.sh`. Modified: `include/fasciculum.h` +
`lib/fasciculum.c` (identity check), `include/filum.h` + `lib/filum.c`
(tree removal), `briar/compile_probationes.sh` and `tools/briar_struere.sh`
(objects), `tools/briar_fumus.sh` (two stages).

Modified: `include/silex.h` + `lib/silex.c` (§4.4, promotion only);
`tools/silex_struere.sh` (sources the extracted corpus block);
`pythonica/silva.py` (four table rows); `canones.registrum` (briar.canon).

## 8. Phase plan (test-first; each phase ends green)

- **P0 format + parser core — DONE (plan 1, 2026-09-04).** Registry, lexicon, lexema (with fence
  state), arbor; fixtures (the two of §2 plus adversarial:
  unterminated, stray close, `<` in prose, empty regions, no
  interpres, **a fenced `<html>` example in prose followed by a real
  `<html!>` region**, a fence left open); gates registrum / lexema /
  arbor; the runner and its pythonica rows.
- **P1 projection — DONE (plan 1, 2026-09-04).** `briar_stml`, `briar.canon` (loaded by path, not registered: the `<arbor>` root is shared by every materia dialect), gates
  stml / canon / totalitas / computus. `briar -arbor` exists first as
  a shell script over the probatio objects (md's `arbor.sh` pattern).
- **P2 nexus — DONE (md and STML plan 1, 2026-09-04; silva inner kind plan 2, 2026-09-05, unit `briar_silva`, gate `silva`).** Inner trees for prose, STML elements, raw attributes
  (`methodus`, `munus`), and C regions through silva with the capsula
  as include provider (latina.h synthetic first; a bare parse is a
  planted fault here — it must be seen to misparse); offsets; gate
  nexus.
- **P3 fabrica, headless — DONE (plan 2, 2026-09-05, gate `fabrica` 149 assertions; goldens `fixa/fabrica/<t>/`; hand tool `./briar/fabrica.sh`; both shapes compiled, run and tested by hand under clang: `salve, munde`, `summa 3`, vitrea app linked in 2.1 s with a 364 KB binary and NO sqlite — a stateless app pulls no volumen).** §4.4 promotion with `probatio_silex`
  unchanged; `briar_fabrica`: unit partition at silva extents, the
  `main` unit, generated `_regiones.h` prototypes, method signature
  check, the probatio unit, the key; goldens for both program shapes
  byte-compared; gate fabrica. No clang is run in the suite.
- **P5 contextus — DONE (plan 4, 2026-09-05; T1 1031da7a, T2 467acd76, T3 0bef09a1, T4).** Unit `briar_contextus` + gate (thirteenth); nexus fields + table lookup; fabrica `#line` per run + `fragmenta` goldens; `-partes` lists fragments; fumus stages VIII/IX; spec §3.4 as built.
- **P4 the binary — DONE (plan 3, 2026-09-05; commits 9cb3bb50 corpus block, 39a03a8a binary, T4 fumus). First-run numbers (HOME redirected): `salve.thistle` cold 0.42 s inside the tree (disk corpus) / 0.68 s from outside via the shebang (embedded corpus), cache hit 9 ms; `salve_vitreum.thistle -struere` cold 1.97 s from outside; `bin/briar` 10.5 MB (silex 19.4 MB); `tools/briar_fumus.sh` 5.2 s for six stages. The first `./salve.thistle` from a directory outside the repository printed `salve, munde` — a GUI app as easy as a bash script is one `-struere` away (its window is the by-hand `-agere` stage).** `tools/briar.c` with the flags of §5,
  `corpus_infixum.sh` extraction, `briar_struere.sh`, cache dir, run,
  `-probatio`; `briar_fumus.sh` from outside the repo; the first real
  file runs from its shebang. **Record the first-bake numbers here**
  (closure size, cold build time, sqlite compile time, binary size) —
  they decide the vendor-object sharing question of §10.

- **P6 facies (§4.6) — DONE (plan 5, 2026-09-05/11; T1 3d08992f, T2 3fdc4797, T3 cff9da70, T4). MEASURED: `salutatio2.thistle` (415 lines) → 70,107 bytes of self-contained page in 0.07 s; 13 fragments, 15 transclusion links, 2 woven views, 115 decorated derived symbols, 14 headers in the island. On its FIRST real run the page found a genuine error in the example: `salutatio2.thistle`'s hand-written header table names 13 headers and briar derives 14 — `motus.h` is missing, and the file uses `constans Motus* motus` at line 267. That file's own prose says "if the two ever disagree the table is wrong", so the feature paid back the cost it was built to pay back, on day one.** `briar_facies` + gate
  (fourteenth); the three chrome files and their capsula; `-html` in
  `briar_imperium` and `tools/briar.c`; escaping and UTF-8 fixtures;
  fumus stage XI. No clang, no window, no network — the page is a
  string the suite can compare.

- **P7 spectator (§4.7) — DONE (plan 6, 2026-09-11; T1 14d251e0, T2). MEASURED: `briar-spectator` links first try by the rule "take from `build/` only what `briar/build/` lacks"; page identity with `-html` proven end to end in fumus XII (71,574 bytes, byte-equal) once the option divergence was fixed. AS BUILT, three deviations: `briar_optiones_plagulae` was extracted because the two binaries DID drift (absolute vs raw path) and no in-process assertion could see it — the real check lives in the fumus, across two binaries · identity holds GIVEN THE SAME CORPUS SOURCE (in-tree briar reads the disk corpus, the spectator always the embedded one, so the fabrica key differs) · `-parare` was added so the fumus can check identity without a window.**

- **P8 fasciculus (§4.8) — PLANNED.** The two library changes first
  (fasciculum identity check, filum tree removal), each with its planted
  fault; then `briar_fasciculum` and its gate headless; then the flags,
  the icon capsula, the link and `tools/briar.c`; then the two fumus
  stages. Record here: binary size after the link, and what A8 measured.

## 9. Named deferrals

Interpreted mode (waits on machinula piscina support, desideratum
01KYB9JMDX, and on Tier-2 `.m` scope) · examen over the regions'
silva trees (the trees exist in v1; what a SCRIPT must obey — house
codices, or a lighter set — is a decree, then one call; note silva
rejects `#line`, so judgment runs on the regions, never on the
generated files) · handler discovery by COMMENT annotation
(`/* <methodus/> */` through the annotationes-stml-spec.md collector,
never a second parser) — only if a thistle ever needs marks the region
tag cannot carry · **a `briar-c89` dialect** (Fran, 2026-09-04): STML
tags directly inside the C, JSX-like, for richer literate programming
and transpilation — a materia client of its own when pulled; its first
step is decided (D5, §3.4): the `<<#x>>` reference moves from the
contextus line scan into the outer lexer as a token, the resolver and
the line table stay ·
**template fragments** `<#@x p="@p">` + `<<#@x p="v">>` over raw C
bodies through the existing macro engine (instantiation, not alias —
the decree of stml-visio §2); wants mid-line references and slot refs
`&@p;` inside C text, "different considerations" (Fran 2026-09-05) ·
mid-line `<<#x>>` (D2) · continuation `pars=` (D3) · html/js/css
fragments (D4) · a `<#x>` wrapper element holding prose + parts (D1) · includes derived for the C library beyond the implicit trio (a hand
map `sqrt → math.h`, `time → time.h`; silva's system tables know the
symbols, not their headers) · bare object-like macros / enum constants
as derivation seeds (needs an unknown-identifier diagnostic from silva)
· flags
DERIVED by aedilis over the capsula (bundle `aedilis.stml`; extractor
over the corpus; vexilla as data — 01KZP0WDN9, trigger fired;
frameworks from `#import` — 01KZYN4VPZ) · an effects-at-the-edge lint
for bridge methods, if wanted, as a NEW codex — never ludus's L5 ·
`!\` dedent applied · multiple
assets by `via=` · app state (`status` region → the scaffold's `Pipa`
+ volumen) · Linux · sealed distribution (`-struere -ad`) · `-formare` · an LSP over `.thistle` · the ludus
islands vocabulary in STML regions once pictor's componens layer
exists · `.m` regions · **the VITREA amalgam** (2026-09-05: the banked
design assumed the capsula text was at hand; it is not —
`tools/capsula_generare.c` is a tool with everything in `main`, briar
has no in-process generator; it needs: the capsula `.c` text generated
in-process (uncompressed entries suffice, `_compress = false`), `.m`
static rows (the identifier index does not parse `.m`; three `.m`
files carry six statics each), the `-framework` triple on the banner
line, and sqlite as text under `#pragma clang diagnostic ignored
"-Weverything"` with its `-D` defines as `#define` lines — a plain
program pulling `volumen` meets the same vendor wall).
 · **briar's own modules available to THISTLES** (Fran's idea
2026-09-11, raised while designing §4.7 and deliberately NOT decided
there): put `briar/fontes/` and silva's amalgam in the corpus and a
thistle becomes a tool ABOUT C code — a linter, a formatter, a browser,
each a literate script with its prose beside it. Genuinely a widening
of what the format is for, and it should be decided on its own merits,
not as a side effect of a viewer. Two findings for whoever takes it:
(i) code availability is not the blocker — the corpus as RUNTIME DATA
is, since `silex_fons_corporis` wants a `CapsulaEmbed*` no generated
app links, so every vitrea thistle app would carry the 10 MB corpus;
(ii) briar's project key is the closure-content hash, so briar's own
source in the corpus means every briar edit invalidates every cached
project on the machine. Sizes measured: briar's modules 308 KB, silva's
amalgam 3.4 MB
· the facies alternatives reserved at F2 (two panes, tabs), F3 (an
STML template program), F4 (a vitia strip as the diagnostic reader) ·
**per-tag policy for prose HTML** (F6's reserved half): an allowlist —
`<kbd>`, `<abbr>`, `<sub>`, `<sup>`, `<mark>` rendered, everything else
escaped — is one `CASUS` per permitted tag, the shape heading levels
already use. It needs ONE thing md does not do yet: record the tag
name in the node. **md already SCANS it** (`md_lexema.c` matches
`TAGI_CRUDI` and the 62 `TAGI_BLOCORUM` to classify CommonMark's block
types; `md_inlinea.c` walks the name to find the tag boundary) and
then discards it — so this is EXPOSURE, not a parser extension, and
cannot change how any existing markdown parses. Answer a real want in
this order: does markdown already cover it (usually — `code`, `em`,
`strong`, `del`, tables with alignment, fenced blocks with a language);
should briar MINT it (heading anchors, a definition-list treatment for
glossaries — the facies generates the page, so those are its job);
only then the allowlist · migrating `md-html-facies.stml` to
`md/html/` as a general safe projection, if a second consumer wants one
· `<briar titulus>` (§2; never built, measured 2026-09-12 — renaming a
project renames its capsula symbol and header guard) · icon options:
letterboxing a non-square source (icones reserves it) and masking to the
macOS icon shape · a signed `.app` for another Mac (fasciculum D3)

Cross-references: ludus-brainstorm.md §XII (codex L5 and the
`<tractator/>` vocabulary briar deliberately does not reuse);
pictor-spec.md §6.4 (the annotation landing order). The reverse
references are the ludus session's to add.

## 10. Risks, notes, AUDIENDA

- **sqlite per project.** The four-tier script compiles `vendor/
  sqlite3.c` at `-O2` in every project dir — IF the closure pulls it.
  Measured 2026-09-05 (plan 2): the stateless vitrea fixture's closure
  has 71 files and no vendor at all (no `volumen`), cold build 2.1 s,
  binary 364 KB. The question returns only with the `status` region
  (§9); not built until then.
- **Two corpus-bearing binaries** (`silex` 19.4 MB, `briar` 10.5 MB
  — measured 2026-09-05; briar carries the silva amalgam but no GUI
  frameworks) with two freshness
  rituals; the shared `corpus_infixum.sh` keeps the generated object
  single. `briar -versio` names the stamp and the flag hash, as silex
  names its stamp.
- **Parse cost per run** is paid only on a cache miss (the key is
  computed from bytes before any parse); a hit execs at once.
- **Flags are policy carried by the binary** until §4.1's derivation
  lands; the key covers them explicitly so a rebuilt briar never
  reuses a project dir built under old flags.
- **Open vocabulary at column 0.** A markdown html block someone
  writes at column 0 in prose, OUTSIDE a fence, becomes an STML
  element; the STML parser or the canon refuses loudly. Acceptable in
  v1; the alternative (a closed registry of region names) is one table
  away if it bites.
- **Derivation never derives a name any region declares** (found
  2026-09-05 when the ludus merge added `Punctum` to
  `include/mandatum.h` while `punctum.thistle` defines its own: the
  probatio region, which sees the app's typedef only through
  `<t>_regiones.h`, derived the header — duplicate typedef at
  compile time; the fabrica golden caught it). Pass one now runs for
  ALL regions first; derivation then skips every depth-0 non-implicit
  symbol of every region's pass-one table (its own declarations and
  the headers it includes itself). A house header may freely reuse a
  name a script defines.
- **Derived includes rely on `corpus.symbola.tsv` being fresh**: the
  shared corpus block regenerates it from the identifier index when
  any `include/*.h` or `lib/*.c` is newer; the briar runner does the
  same before its gates. A disk-corpus run reads the tree's copy.
- **Reserved first argument** after the file (§5): six words a
  program cannot take as its own first argument without `--`. The
  price of `./x.thistle -probatio`; documented, not hidden.
- **`~/.bin/scribe`** is a symlink into the main tree and refuses paths
  outside it (exit 4); in the worktree use `./silva/scribe.sh`.
- **Names are unsealed.** briar and thistle (Fran's, English by
  choice), regio / prosa / interpres / elementum / nexus / fabrica,
  `methodus` and `munus`, the flag names, the promoted silex names —
  Fran names before P0 commits to `briar/`.
- **An app from Finder is not the script from a shell** (§4.8): its
  working directory is `/` and it has no terminal, so a program reading
  relative paths behaves differently from `./x.thistle`. Documented, not
  fixed.
- **A bundle is a copy.** Editing the thistle does not change an
  existing `x.app` until `-app` runs again.
- **Default identities collide by name**: two `salve.thistle` in
  different folders share `org.rhubarb.briar.salve`. Set `identitas` on
  one of them.
- **Finder's icon cache** (A8) is unmeasured until the `-agere` stage.
