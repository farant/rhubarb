# briar — spec v1.6 (literate C89 programs; `.thistle`)

*2026-09-04. v1 consolidated the design conversation of the same day
(research nota 01M1QC21ZJ in the tabularium). v1.1 folds in the
review from the ludus/pictor session (five items) with Fran's
decisions on each. Fran's decisions are marked DECISUS; everything
else is PROPOSITUM and was agreed in conversation unless marked OPEN.
Names marked (unsealed) are working names — Fran names. Every
"exists" claim cites the header it rests on. English prose, Latin
identifiers, as in pictor-spec.md.*

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

## 5. The binary and its build

- **Flags, not verbs (DECISUS, Fran 2026-09-04: thistle files are
  scripts).** `briar [-flag] [-f <radix>] x.thistle [args…]`; house
  single-dash form. **As built (plan 3):** the flags are hand-parsed
  in `briar_imperium` (gated, 53 assertions), not by `argumenta.h`:
  that parser refuses undeclared flags and cannot stop at the file,
  so a program's own `-foo` after the file would be refused. The
  declared-options spirit is kept — exactly `-probatio -struere
  [-iterum] -arbor -partes -versio -auxilium/-h -f <radix>` are
  recognized before the file; any other `-x` there is a refusal
  (exit 2) naming the six. One code path: the bare form is the run.

  | flag | does |
  |---|---|
  | (none) | run: build if absent, exec |
  | `-probatio` | build the probatio if absent, exec it (exit code = verdict) |
  | `-struere [-iterum]` | build only, print the project dir; `-iterum` ignores the cache |
  | `-arbor` | print the STML projection |
  | `-partes` | print the closure, ADEST/ABEST as `silex partes` |
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
| `tools/briar.c` | all above, argumenta, processus, capsula | the binary |
| `tools/briar_struere.sh`, `tools/corpus_infixum.sh`, `tools/briar_fumus.sh` | — | build, shared corpus block, freshness gate |

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
+ volumen) · Linux · sealed distribution (`-struere -ad`) · `-html`
(literate rendering) · `-formare` · an LSP over `.thistle` · the ludus
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
