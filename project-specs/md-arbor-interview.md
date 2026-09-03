# md_arbor — interview (raw Q&A), 2026-09-02/03

> Raw record of the interview that turns `md-arbor-plan.md` (draft) into
> `md-arbor-spec.md`. Questions in the order asked; answers verbatim in
> substance. Spike context: `md-arbor-spica.md`.

## Q1 — recursion shape for the md→html program

**Q.** The dispatcher picks a rule by genus; each rule transforms the
node's children by calling the dispatcher again. Under the strata law
(macros spec §2.3: calls reference only fragments defined EARLIER,
forward reference = vitium, termination by construction) that mutual
recursion is illegal in one direction. Options: (a) ONE self-calling
dispatcher holding every DIRIBITIO arm inline, self-call legal only when
the argument is a STRICT subtree of the current argument (termination by
descent, checked at fill time), earlier non-recursive helpers still
legal; (b) a declared recursive group (`<RECURSIO>`) where a marked set
may reference each other, same descent rule group-wide; (c) AMBULATIO
verb — one walk over the original tree, rule per node, `&@liberi;` =
transformed children, no strata change.

**A (Fran).** (a) Self-calling dispatcher. Engine: one exemption from
FRAGMENTUM_POSTERIUS for a call to the fragment currently being filled,
plus a descent guard (argument ⊂ current argument, strict). Helpers
defined earlier stay ordinary macros.

## Q2 — heading levels (h1..h6 from `gradus`)

**Q.** A value capture fills an attribute but cannot NAME an element.
Options: (a) six CASUS arms, no engine change; (b) an engine form that
names an element from a value (`<ELEMENTUM titulus="h&@g;">`); (c) the
parser projects six genera `capitulum-1..6` (not recommended).

**A (Fran).** "if i understand correctly i think this is the way? the
key would be composition of <<#@macros>> to make the verbose version
ergonomic and reusable." — READ AS (a) with composition: no engine
form for element-from-value; the six-way switch is written ONCE in an
earlier helper (e.g. `<#@html-h g corpus>`) that receives the
already-transformed children as a SUBTREE ARGUMENT (the lint-composition
mechanism: args carry subtrees); each dispatcher arm = one call. The
child loop + self-call lives in the dispatcher arm (it cannot live in an
earlier helper — forward reference). Consequence for B1: arguments may
carry PER/self-calls that expand during the fill.

## Census for Q3 (dialect) — git ls-files '*.md', 1,120 files, 2026-09-03

| construct | measure |
|---|---|
| YAML front matter (line 1 = `---`) | **358 files** (every memory file) — pure CommonMark: thematic break + paragraph + setext `---` = an h2 |
| task lists `- [ ]` / `- [x]` | 19 files, 770 lines (plans) |
| strikethrough `~~x~~` | 22 files |
| bare URLs (autolink extension) | 38 in 21 files; angle autolinks `<http…>` 0 |
| footnotes `[^x]` | 3 files |
| HTML block lines (`^<tag`) | 71 files, 1,989 lines; `<!--` comments 42 files; inline html tags 25 files |
| CAPS tags in prose (`<CAPS…`) | 21 files (STML examples outside fences) |
| blank line then 4-space-indented line | 1,091 sites (indented-code rule territory; most are list continuations) |
| `$$` math | 6 files |
| tabs for indentation | 0 files |
| trailing double-space | 86 lines; backslash at line end 176 lines |
| images `![` | 0 |
| entities `&x;` | 239 |
| sententiae addressed lines | 16 files |
| `=`/`---` underline-shaped lines | 3,842 (setext + thematic breaks + front-matter fences) |
| headings h4+ | 434 lines |
| underscore emphasis `_x_` | 91 |
| `~~~` fences | 0; ordered `)` delimiter 5 lines |
| fence info strings | 2,994: c 2,540 · xml 201 · bash 148 · stml 22 · js 16 · json 13 · yaml 10 · sh 9 · markdown 7 |

**Recheck:** CRLF 0 files. The "ordered lists starting ≠1" row above
was a miscount (it counted loose-list continuations); the indented-code
row is a heuristic — A3/A6 must MEASURE what the indented-code rule
yields on the corpus (count blocks, eyeball a sample) before trusting
the green.

## Q3 — dialect

**Q3a (GFM extensions beyond tables).** Options: task lists, strikethrough,
bare-URL autolinks, footnotes.
**A (Fran).** Task lists + strikethrough + bare-URL autolinks. Footnotes
stay literal text.

**Q3b (YAML front matter, 358 files).** Options: (a) front-matter block
genus at document start, contents raw; (b) pure CommonMark (h2); (c) block
genus + key/value projection.
**A (Fran).** (a) Front-matter block genus, opaque, document start only.

## Q4 — genus vocabulary

**Q.** Proposed table (bare tags like css, slots as wrapper elements):
BLOCKS documentum · praefatio · capitulum gradus=1..6 forma=atx|setext ·
paragraphus · saeptum lingua= forma=saeptum|indentatum · citatio · lista
genus=punctata|numerata initium= laxa= · elementum perfectum= · divisio ·
tabula > caput / ordo / cella ordinatio= · html · definitio-nexus titulus=
url= descriptio= · linea-vacua (blank-line token). INLINE under `<inlinea>`:
textus · emphasis · fortis · deletio · verbatim · nexus url= descriptio=
forma=inlineus|referens|auto · imago · fractura-dura · fractura-mollis ·
html-inlineum. Flagged: 'saeptum' (css {} block), 'caput' (C header).
**A (Fran).** "table as proposed looks good to me. if we need to worry
about tag collisions at some point i would probably work on some kind of
namespace thing like <md::praefatio> etc which would have a <USING
ns="md"> complement or something like that. but we're probably okay as
is for now?" → bare tags now; STML NAMESPACES (`<md::x>` + `<USING
ns="md">`) filed as a desideratum, not built.

## Q5 — whitespace (the markdown D7)

**Q.** (a) zero-trivia line model: every byte a token with one owner —
container prefixes (indentation, `>`, list marker + spacing) are tokens
of the container that consumed them, the rest of the line one text token
of the leaf block, blank lines `linea-vacua` tokens of the innermost open
container, fence contents raw line tokens, no `<ante>/<post>` anywhere;
(b) css-style hybrid (indentation + blank lines as ligator trivia);
(c) indentation as trivia, blank lines as tokens.
**A (Fran).** (a) Zero-trivia line model.

## Q6 — html oracle normalizer

**Q.** (a) C normalizer inside `probatio_md_html`: both html strings
through the house html lexer, forgiving ONLY inter-tag whitespace and
attribute order, every forgiven example counted and reported; (b) Python
port of CommonMark's normalize in a pythonica runner; (c) no normalizer,
byte-exact against cmark's html.
**A (Fran).** (a) C normalizer with named forgiveness.

## Q7 — Prosa addressing (arc C)

**Q.** (a) selectio.h CSS selectors over the md projection (one selector
language across silva/css/md) + Python helper sugar (`capitulum('Arc A')`,
`elementum(n)`) that builds selectors + text match; (b) Python helpers
only; (c) sententiae-style dotted addresses.
**A (Fran).** (a) selectio + helper sugar.

## After the interview — what the codebase read changed (2026-09-03)

Read for the spec: materia_nodus/token/arbor/scribere headers and the
INDEX/derived-token paths in `materia_arbor.c`, the emitter's
foreign-source omission (`materia_scribere.c:263`), css registry/lexicon/
runner/corpus/stml gates, `lib/stml_macros.c` surface (105 declarations),
macros spec §2/§6.1/§6.2, exemplaria spec §1.3/§2/§3/§8 (doors 5, 7,
13-19), the exemplaria interview's door-5/CASUS residue, `selectio.h`,
`html_lexema.h`, `stml_html.h`, pythonica `Textus`/`extenta`.

Refinements recorded in `md-arbor-spec.md` §13 for Fran's review:
(a) line model — lines are nodes, container markers are line prefixes
(the emitter is structural, so a container cannot interleave its
markers per line); (b) the inline tree OWNS its bytes (sharing tokens
would double bytes in emitted slots); (c) §1.3 extents deferred to the
first MUTATION consumer (the transform generates); (d) derived tokens
(source 1) carry decoded text / resolved links — the C89 origo/frons
seam reused; (e) CommonMark + GFM vendored as `spec.txt` with one C
reader, no JSON; (f) raw html passthrough needs a `vertere` raw form
or html-block examples stay known-red; (g) front matter = raw lines.
Engine increment B1 spelled as four features: `de="@arg"`, pattern
CASUS in COMMUTATIO with `<EST>` + captures→arm, `liberi` (depth-one)
application, self-call with the descent guard.
